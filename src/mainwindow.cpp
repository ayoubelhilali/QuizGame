#include <QTimer>
#include <QScrollArea>
#include <QProgressBar>
#include <QLabel>
#include <QPainter>
#include <QGraphicsDropShadowEffect>
#include <QMessageBox>
#include <QPixmap>
#include <QGridLayout>
#include <QDebug>
#include <memory>
#include <QButtonGroup>

#include "headers/mainwindow.h"
#include "headers/settingsdialog.h"
#include "ui_mainwindow.h"
#include "headers/hovereffect.h"
#include "headers/answerbox.h"
#include "headers/circletimer.h"
#include "headers/typingeffect.h"

using namespace std;
namespace
{
const int MIN_WIDTH = 900;
const int MIN_HEIGHT = 600;
const QColor SHADOW_COLOR(0, 194, 203);
const int SHADOW_BLUR_RADIUS = 50;
const int BUTTON_SHADOW_BLUR_RADIUS = 10;
const int BUTTON_ICON_SIZE = 30;
const QString BUTTON_STYLE = "background-color:transparent; border: 2px solid white; font-size:27; border-radius:5px";
const QString ICON_PATH = ":/Icons/";
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setMinimumSize(MIN_WIDTH, MIN_HEIGHT);

    ui->gridLayout->setAlignment(Qt::AlignCenter);

    applyShadowEffect(ui->label, SHADOW_BLUR_RADIUS, SHADOW_COLOR);

    setupButton(ui->startBtn, "startBtn");
    setupButton(ui->statsBtn, "stats-icon.svg");
    setupButton(ui->infoBtn, "info.svg");
    setupButton(ui->settingsBtn, "settings.svg");

    QPushButton *buttons[] = {ui->startBtn, ui->statsBtn, ui->infoBtn, ui->settingsBtn};
    for (QPushButton *button : buttons)
    {
        applyShadowEffect(button, BUTTON_SHADOW_BLUR_RADIUS, SHADOW_COLOR);
        new HoverEffect(button);
    }

    resize(MIN_WIDTH, MIN_HEIGHT);

    // Initialize pauseOverlay and pausecontainer to nullptr
    pauseOverlay = nullptr;
    pausecontainer = nullptr;
    exitBtn = nullptr;
    infoBtn = nullptr;
    volumetxt = nullptr;
    volumeSlider = nullptr;
    pauseBtn = nullptr;
    timer = nullptr;
}

void MainWindow::applyShadowEffect(QWidget *widget, int blurRadius, const QColor &color)
{
    auto shadowEffect = std::make_unique<QGraphicsDropShadowEffect>(this);
    shadowEffect->setBlurRadius(blurRadius);
    shadowEffect->setColor(color);
    shadowEffect->setOffset(0, 0);
    widget->setGraphicsEffect(shadowEffect.release());
}

void MainWindow::setupButton(QPushButton *button, const QString &iconName)
{
    button->setIcon(QIcon(ICON_PATH + iconName));
    button->setStyleSheet(BUTTON_STYLE);
    button->setIconSize(QSize(BUTTON_ICON_SIZE, BUTTON_ICON_SIZE));
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    QSize parentSize = event->size();
    QWidget *layoutContainer = ui->container;
    QSize layoutSize = layoutContainer->size();
    int x = (parentSize.width() - layoutSize.width()) / 2;
    int y = (parentSize.height() - layoutSize.height()) / 2;
    layoutContainer->move(x, y);
    QWidget::resizeEvent(event);

    QMainWindow::resizeEvent(event);
    if (pauseBtn) {  // Check if button exists
        pauseBtn->move(this->width()/13, this->height()/13);  // Keep it at top-left
    }
    if(pauseOverlay){
        pauseOverlay->setGeometry(0,0,this->width(),this->height());
    }
    if(pausecontainer){
        pausecontainer->setGeometry(width() / 4,height() / 4, width() / 2, height() / 2);
    }
    if(exitBtn && pausecontainer){
        exitBtn->setGeometry(pausecontainer->width()/17,pausecontainer->height()/13,BUTTON_ICON_SIZE+10,BUTTON_ICON_SIZE+10);
    }
    if(infoBtn && pausecontainer){
        infoBtn->setGeometry(pausecontainer->width()-pausecontainer->width()/8,pausecontainer->height()/13,BUTTON_ICON_SIZE+10,BUTTON_ICON_SIZE+10);
    }
    if(backbutton){
        backbutton->setGeometry(width()/7,height()/6,90,30);
    }
}

void MainWindow::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QPixmap pixmap(":/images/quiz-background.jpg");
    painter.drawPixmap(0, 0, width(), height(), pixmap);
}

MainWindow::~MainWindow()
{
    // Correctly delete dynamically allocated objects in the destructor
    if (pauseOverlay) delete pauseOverlay;
    if (pausecontainer) delete pausecontainer;
    if (exitBtn) delete exitBtn;
    if (infoBtn) delete infoBtn;
    if (volumetxt) delete volumetxt;
    if (volumeSlider) delete volumeSlider;
    if (pauseBtn) delete pauseBtn;
    if(timer) delete timer;

}

void MainWindow::on_startBtn_clicked()
{
    deleteButtons({ui->startBtn, ui->statsBtn, ui->infoBtn, ui->settingsBtn});

    auto waitLabel = createLabel("Please wait", 20, Qt::AlignCenter);
    TypingAnimation *typingEffect = new TypingAnimation(waitLabel,waitLabel->text(), 200,this);
    typingEffect->start();

    auto progressBar = createProgressBar();

    auto waitLayout = new QVBoxLayout();
    ui->label->setAlignment(Qt::AlignCenter);
    waitLayout->addWidget(ui->label);
    waitLayout->addSpacing(20);
    waitLayout->addWidget(waitLabel);
    waitLayout->addWidget(progressBar);
    waitLayout->setAlignment(Qt::AlignCenter);
    waitLayout->setContentsMargins(200, 100, 200, 200);

    auto waitWidget = new QWidget(this);
    waitWidget->setLayout(waitLayout);
    waitWidget->setMaximumWidth(500);

    replaceCentralWidgetLayout(waitLayout);
    waitWidget->show();

    startProgressBar(progressBar, waitLabel,typingEffect);
}

void MainWindow::deleteButtons(const std::initializer_list<QPushButton *> &buttons)
{
    for (QPushButton *button : buttons)
    {
        delete button;
    }
}

QLabel *MainWindow::createLabel(const QString &text, int fontSize, Qt::Alignment alignment)
{
    auto label = new QLabel(text, this);
    label->setStyleSheet(QString("font-size: %1px;").arg(fontSize));
    label->setAlignment(alignment);
    label->setWordWrap(true);
    return label;
}

QProgressBar *MainWindow::createProgressBar()
{
    auto progressBar = new QProgressBar(this);
    progressBar->setStyleSheet(
        "QProgressBar {"
        "   font-size: 20px;"
        "   font-weight: bold;"
        "   color: blue;"
        "   background-color: black;"
        "   border: 2px solid #00c2cb;"
        "   border-radius: 10px;"
        "   text-align: center;"
        "}"
        "QProgressBar::chunk {"
        "   background-color: white;"
        "   border-radius: 8px;"
        "}");
    progressBar->setRange(0, 100);
    progressBar->setValue(0);
    progressBar->setAlignment(Qt::AlignCenter);
    progressBar->setMaximumWidth(700);
    return progressBar;
}

void MainWindow::replaceCentralWidgetLayout(QLayout *newLayout)
{
    QWidget *centralWidget = this->centralWidget();
    if (centralWidget && centralWidget->layout())
    {
        QLayout *oldLayout = centralWidget->layout();
        QLayoutItem *child;
        while ((child = oldLayout->takeAt(0)) != nullptr)
        {
            if (child->widget())
            {
                child->widget()->setParent(nullptr); // Detach widget from layout
            }
            delete child; // Delete the layout item
        }

        delete oldLayout; // Delete the old layout
    }
    centralWidget->setLayout(newLayout);
}

void MainWindow::startProgressBar(QProgressBar *progressBar, QLabel *waitLabel,TypingAnimation *typingeffect)
{
    auto timer = new QTimer(this);
    int duration = 1000; // Total duration in milliseconds
    int interval = 50; // Interval in milliseconds
    int steps = duration / interval;
    int stepSize = 100 / steps;
    int progress = 0;

    connect(timer, &QTimer::timeout, this, [=] () mutable {
        if (progress < 100) {
            progress += stepSize;
            progressBar->setValue(progress);
        } else {
            timer->stop();
            typingeffect->stop();
            ui->label->hide();
            waitLabel->hide();
            progressBar->hide();
            DomainsChoose();
        }
    });
    timer->start(interval);
}

void MainWindow::DomainsChoose()
{
    QPushButton *backbutton = new QPushButton(this);
    backbutton->setText(" <    Back");
    backbutton->setCursor(Qt::PointingHandCursor);
    backbutton->setMinimumSize(80, 30);
    backbutton->setStyleSheet("QPushButton{"
                              "background-color:transparent;"
                              "border:1px solid white;"
                              "border-radius: 10px;"
                              "}"
                              "QPushButton:hover{"
                              "background-color:rgba(255,255,255,0.5);"
                              "}");
    backbutton->setMinimumWidth(100);
    connect(backbutton, &QPushButton::clicked, this, &MainWindow::onBackButtonClicked);

    auto domainHead = createLabel("Choose your domain: ", 28, Qt::AlignCenter);
    applyShadowEffect(domainHead, BUTTON_SHADOW_BLUR_RADIUS, SHADOW_COLOR);

    auto generalDomain = createDomainButton("general.svg");
    auto logicDomain = createDomainButton("logic.svg");
    auto techDomain = createDomainButton("tech.svg");
    auto entertainDomain = createDomainButton("entertain.svg");

    QPushButton *buttons[] = {generalDomain, logicDomain, techDomain, entertainDomain};
    for (QPushButton *button : buttons)
    {
        button->setCursor(Qt::PointingHandCursor);
        button->setMaximumWidth(140); // Increased button width
        button->setMaximumHeight(180); // Increased button height
        connect(button, &QPushButton::clicked, this, &MainWindow::onDomainButtonClicked);
        button->show();
    }

    auto domainHeadLayout = new QHBoxLayout();
    domainHeadLayout->addWidget(backbutton, Qt::AlignLeft);
    domainHeadLayout->addWidget(domainHead, Qt::AlignCenter);

    QWidget *domainheadwid = new QWidget(this);
    domainheadwid->setLayout(domainHeadLayout);
    domainheadwid->setMinimumWidth(600);

    auto domainsLayout = new QVBoxLayout(); // Changed to QVBoxLayout to handle multiple rows
    auto firstRowLayout = new QHBoxLayout();
    auto secondRowLayout = new QHBoxLayout();

    for (int i = 0; i < 4; ++i)
    {
        if (i < 3)
        {
            firstRowLayout->addWidget(buttons[i]);
        }
        else
        {
            secondRowLayout->addWidget(buttons[i]);
        }
    }

    firstRowLayout->setSpacing(20);
    secondRowLayout->setSpacing(20);
    domainsLayout->addLayout(firstRowLayout);
    domainsLayout->addLayout(secondRowLayout);

    auto mainLayout = new QVBoxLayout();
    replaceCentralWidgetLayout(mainLayout);
    mainLayout->addWidget(domainheadwid,0,Qt::AlignCenter);
    mainLayout->addLayout(domainsLayout);
    mainLayout->setContentsMargins(100, 100, 100, 100);

    generalDomain->setObjectName("general");
    logicDomain->setObjectName("logic");
    techDomain->setObjectName("tech");
    entertainDomain->setObjectName("entertain");

    // Adjust layout based on available space
    adjustDomainLayout(firstRowLayout, secondRowLayout, buttons);
}

void MainWindow::adjustDomainLayout(QHBoxLayout *firstRowLayout, QHBoxLayout *secondRowLayout, QPushButton *buttons[])
{
    int availableWidth = this->width() - 200; // Adjust based on your margins
    int buttonWidth = buttons[0]->maximumWidth() + firstRowLayout->spacing();
    int buttonsPerRow = availableWidth / buttonWidth;

    if (buttonsPerRow < 4)
    {
        firstRowLayout->removeWidget(buttons[3]);
        secondRowLayout->addWidget(buttons[3]);
    }
}

QPushButton *MainWindow::createDomainButton(const QString &iconName)
{
    auto button = new QPushButton(this);
    button->setIcon(QIcon(ICON_PATH + iconName));
    button->setStyleSheet("background-color:transparent;");
    button->setIconSize(QSize(130, 140)); // Increased icon size
    return button;
}

void MainWindow::questionsPage(const QString &domain)
{
    pauseBtn = new QPushButton(this);
    pauseBtn->setIcon(QIcon(ICON_PATH + "pauseBtn.png"));
    pauseBtn->setStyleSheet("QPushButton {"
                            "background-color: transparent;"
                            "border-radius: 25px;"
                            "width: 50px;"
                            "height: 50px;"
                            "}");
    pauseBtn->setIconSize(QSize(50, 50));
    pauseBtn->setFixedSize(50, 50);
    pauseBtn->setCursor(Qt::PointingHandCursor);
    pauseBtn->show();

    // Set initial position
    pauseBtn->move(this->width()/13, this->height()/13);
    ispaused=false;
    connect(pauseBtn, &QPushButton::clicked, this, &MainWindow::onPauseClicked);


    auto domainNameTxt = createLabel("Domain: ", 10, Qt::AlignLeft);
    domainNameTxt->setStyleSheet("color:yellow;"
                                 "font: 9pt '8514oem';");
    auto domainName = createLabel(domain, 10, Qt::AlignLeft);
    domainName->setStyleSheet( "font:20pt 'Terminal';");

    timer = new CircleTimer(this);
    timer->setFixedSize(60,60);
    timer->setStyleSheet("margin:14px;");
    timer->startTimer();
    timer->show();

    auto scoreTxt = createLabel("Score: ", 10, Qt::AlignRight);
    scoreTxt->setStyleSheet("color:yellow;"
                            "font: 15pt 'Terminal';");
    auto score = createLabel("100", 10, Qt::AlignRight);
    score->setStyleSheet("font: 12pt 'Terminal';");

    auto questionLabel = createLabel(generateQuestion(domain), 15, Qt::AlignCenter);
    applyShadowEffect(questionLabel,SHADOW_BLUR_RADIUS,SHADOW_COLOR);
    questionLabel->setStyleSheet("font: 15pt 'Terminal';");
    questionLabel->setWordWrap(true);

    auto B1 = new AnswerBox("khadija", "A",-1, this);
    auto B2 = new AnswerBox("Halima", "B",-1,this);
    auto B3 = new AnswerBox("Fatima", "C", -1,this);
    auto B4 = new AnswerBox("Aicha", "D",-1,this);

    connect(B1, &AnswerBox::clicked, this, &MainWindow::onAnswerBoxClicked);
    connect(B2, &AnswerBox::clicked, this, &MainWindow::onAnswerBoxClicked);
    connect(B3, &AnswerBox::clicked, this, &MainWindow::onAnswerBoxClicked);
    connect(B4, &AnswerBox::clicked, this, &MainWindow::onAnswerBoxClicked);

    auto mainLayout = qobject_cast<QVBoxLayout *>(ui->centralwidget->layout());
    if (!mainLayout)
    {
        mainLayout = new QVBoxLayout(ui->centralwidget);
        ui->centralwidget->setLayout(mainLayout);
    }

    auto domainwidget=new QWidget;
    domainwidget->setFixedSize(200,80);

    auto scorewidget=new QWidget;
    scorewidget->setFixedSize(180,80);

    auto headLayout = new QHBoxLayout();
    auto scorelayout=new QHBoxLayout(scorewidget);
    auto domainlayout=new QHBoxLayout(domainwidget);

    domainlayout->addWidget(domainNameTxt);
    domainlayout->addWidget(domainName);

    scorelayout->addWidget(scoreTxt);
    scorelayout->addWidget(score);
    scorelayout->setAlignment(Qt::AlignRight);

    headLayout->addSpacing(20);
    headLayout->addWidget(domainwidget,Qt::AlignLeft);
    headLayout->addWidget(timer,Qt::AlignCenter);
    headLayout->addWidget(scorewidget,Qt::AlignRight);
    headLayout->addSpacing(20);

    auto headWidget = new QWidget(this);
    headWidget->setLayout(headLayout);
    headWidget->setMinimumWidth(700);
    headWidget->setMinimumHeight(70);

    auto answersLayout = new QGridLayout();
    answersLayout->addWidget(B1, 1, 0);
    answersLayout->addWidget(B2, 1, 1);
    answersLayout->addWidget(B3, 2, 0);
    answersLayout->addWidget(B4, 2, 1);

    mainLayout->addWidget(headWidget,0,Qt::AlignCenter);
    mainLayout->addSpacing(15);
    mainLayout->addWidget(questionLabel);
    mainLayout->addSpacing(15);
    mainLayout->addLayout(answersLayout);
    mainLayout->setContentsMargins(100, 80, 100, 80);
    setLayout(mainLayout);
}

void MainWindow::onDomainButtonClicked()
{
    clearWidgets<QPushButton>();
    clearWidgets<QLabel>();
    if(pauseBtn)
        delete pauseBtn;
    if(timer)
        delete timer;

    QPushButton *clickedButton = qobject_cast<QPushButton *>(sender());
    if (!clickedButton)
        return;

    if (clickedButton->objectName() == "entertain")
    {
        domain="Entertainment & Pop Culture";
    }
    else if (clickedButton->objectName() == "general")
    {
        domain="General Knowledge";
    }
    else if (clickedButton->objectName() == "tech")
    {
        domain="Tech & Coding";
    }
    else if (clickedButton->objectName() == "logic")
    {
        domain="Logic & Brain Teasers";
    }
    questionsPage(domain);
}

template <typename T>
void MainWindow::clearWidgets()
{
    QList<T *> widgets = this->findChildren<T *>();
    for (T *widget : widgets)
    {
        widget->hide();
        widget->deleteLater();
    }
}

void MainWindow::onAnswerBoxClicked(AnswerBox *box)
{
    box->getTextlabel()->setStyleSheet("background-color:rgba(0,0,0,0.3);");
    for (auto btn : this->findChildren<AnswerBox *>()) {
        if (btn != box) {
            btn->getTextlabel()->setStyleSheet("");
        }
    }
}

QString MainWindow::generateQuestion(QString domain){
    QLabel *question = new QLabel("Question in the domain of: ");
    question->setText(question->text() + domain);
    return question->text();
}

void MainWindow::on_settingsBtn_clicked()
{
    SettingsDialog settingsDialog(this);
    settingsDialog.exec();
}



void MainWindow::on_infoBtn_clicked()
{
    deleteButtons({ui->startBtn, ui->statsBtn, ui->infoBtn, ui->settingsBtn});
    ui->label->deleteLater();

    QPushButton *backbutton = new QPushButton(this);
    backbutton->setText(" <    Back");
    backbutton->setCursor(Qt::PointingHandCursor);
    backbutton->setMinimumSize(80, 30);
    backbutton->setStyleSheet("QPushButton{"
                              "background-color:transparent;"
                              "border:1px solid white;"
                              "border-radius: 10px;"
                              "}"
                              "QPushButton:hover{"
                              "background-color:rgba(255,255,255,0.5);"
                              "}");
    backbutton->setMinimumWidth(100);
    connect(backbutton, &QPushButton::clicked, this, &MainWindow::onBackButtonClicked);

    auto infoHead = createLabel("Instructions", 28, Qt::AlignCenter);
    applyShadowEffect(infoHead, BUTTON_SHADOW_BLUR_RADIUS, SHADOW_COLOR);


    // Instructions content
    QString instructions =
        "<h2><b style='font-size:100px;'>Instructions for QuizGame</b></h2>"
        "<h3 style='color:yellow;'>Basic Game Setup</h3>"
        "<ul>"
        "<li style='text-align:left;'>Click on the <b>Start Game</b> button on the home screen</li>"
        "<li style='text-align:left;'>Choose your <b>quiz Domain</b></li>"
        "<li style='text-align:left;'>Select difficulty level (<b>Easy</b>, <b>Medium</b>, <b>Hard</b>)</li>"
        "</ul>"

        "<h3 style='color:yellow;'>Gameplay Instructions</h3>"
        "<b>1 - Quiz Flow: </b><br>"
        "<ul>"
        "<li style='text-align:left;'>Each question will appear one at a time</li>"
        "<li style='text-align:left;'>Read the question carefully</li>"
        "<li style='text-align:left;'>Select your answer from the multiple-choice options</li>"
        "<li  style='text-align:left;'>A timer may count down for each question</li>"
        "</ul>"

        "<b>2 - Scoring System:</b><br>"
        "<ul>"
        "<li style='text-align:left;'><span style='color:white;'>Correct answers: +10 points</span></li>"
        "<li style='text-align:left;'>Incorrect answers: 0 points</li>"
        "<li style='text-align:left;'><span style='color:orange;'>Time bonus: +5 points</span> for answering within 5 seconds</li>"
        "<li style= 'text-align:left;'><span style='color:#9B59B6;'>Streak bonus: +2 points</span> for each consecutive correct answer</li>"
        "</ul>"

        "<b>3 - Power-ups and Helps:</b><br>"
        "<ul>"
        "<li style='text-align:left;'><b> 50/50</b>: Eliminates two incorrect answers</li>"
        "<li style='text-align:left;'><b> Time Freeze</b>: Pauses the timer for 10 seconds</li>"
        "<li style='text-align:left;'><b> Hint</b>: Provides a clue about the correct answer</li>"
        "<li style='text-align:left;'><b> Skip</b>: Skip the current question without penalty</li>"
        "<li style='text-align:left;'><i>Each help option can only be used once per game</i></li>"
        "</ul>"

        "<b>4 - Special Item</b>:<br>"
        "<ul>"
        "<li style='text-align:left;'><b>Bomb</b>: After clicking on this icon. A bomb is generated in the answer section.</li>"
        "<li style='text-align:left;'>The bomb will explode randomly and eliminate one of the wrong answers.</li>"
        "</ul>"

        "<h3 style='color:#CA6F1E;'>End of Game</h3>"
        "<ul>"
        "<li style='text-align:left;'>View your final score</li>"
        "<li style='text-align:left;'>See correct answers for missed questions</li>"
        "<li style='text-align:left;'><b>Play Again</b>: Start a new game with the same settings</li>"
        "<li style='text-align:left;'><b>Return to Main Menu</b>: Exit to the home screen</li>"
        "</ul>"

        "<h3 style='color:#5D6D7E;'>Settings Options</h3>"
        "<ul>"
        "<li style='text-align:left;'>Toggle sound effects and background music</li>"
        "<li style='text-align:left;'>Adjust difficulty level</li>"
        "</ul>"
        "<p style= 'text-align:center;font-size:15px;'>©Ayoub, All rights reserved 2025 </p><br>";


    QLabel *instructionsLabel = new QLabel(instructions);
    instructionsLabel->setWordWrap(true);
    instructionsLabel->setTextFormat(Qt::RichText);
    instructionsLabel->setStyleSheet("font-size: 16px; color: white; background-color: transparent;"); // Transparent background
    instructionsLabel->setAlignment(Qt::AlignLeft); // Align text to the left


    QScrollArea *scrollArea = new QScrollArea;
    scrollArea->setWidgetResizable(true);
    scrollArea->setStyleSheet("background-color: transparent; border: none;"); // Transparent ScrollArea

    QWidget *scrollContent = new QWidget();
    scrollContent->setStyleSheet("background-color: transparent;"); // Transparent content widget
    QVBoxLayout *scrollLayout = new QVBoxLayout(scrollContent);
    scrollLayout->addWidget(instructionsLabel);
    scrollContent->setLayout(scrollLayout);
    scrollArea->setWidget(scrollContent);
    scrollLayout->setAlignment(Qt::AlignCenter);

    // -----------------------------------------------------------------------

    auto infoHeadLayout = new QHBoxLayout();
    infoHeadLayout->addWidget(backbutton, Qt::AlignLeft);
    infoHeadLayout->addWidget(infoHead, Qt::AlignCenter);

    QWidget *infoheadwid = new QWidget(this);
    infoheadwid->setLayout(infoHeadLayout);
    infoheadwid->setMinimumWidth(600);

    //Create Layout

    auto infosLayout = new QVBoxLayout(); // Changed to QVBoxLayout to handle multiple rows

    infosLayout->addWidget(scrollArea);

    auto mainLayout = new QVBoxLayout();
    replaceCentralWidgetLayout(mainLayout);

    mainLayout->addWidget(infoheadwid,0,Qt::AlignCenter);

    mainLayout->addLayout(infosLayout);

    mainLayout->setContentsMargins(100, 100, 100, 100);

}

void MainWindow::on_statsBtn_clicked(){
    deleteButtons({ui->startBtn, ui->statsBtn, ui->infoBtn, ui->settingsBtn});
    ui->label->deleteLater();

    backbutton = new QPushButton(this);
    backbutton->setText(" <    Back");
    backbutton->setCursor(Qt::PointingHandCursor);
    backbutton->setMinimumSize(80, 30);
    backbutton->setStyleSheet("QPushButton{"
                              "background-color:transparent;"
                              "border:1px solid white;"
                              "border-radius: 10px;"
                              "}"
                              "QPushButton:hover{"
                              "background-color:rgba(255,255,255,0.5);"
                              "}");
    backbutton->setMinimumWidth(100);
    backbutton->setGeometry(width()/7,height()/6,90,30);
    backbutton->show();
    connect(backbutton, &QPushButton::clicked, this, &MainWindow::onBackButtonClicked);

    auto statsHead = createLabel("Stats", 28, Qt::AlignCenter);
    applyShadowEffect(statsHead, BUTTON_SHADOW_BLUR_RADIUS, SHADOW_COLOR);
    statsHead->setStyleSheet("font-weight:bold;font-size:40px;");

    auto statsHeadLayout = new QHBoxLayout();
    statsHeadLayout->addWidget(statsHead, Qt::AlignCenter);

    QWidget *statsHeadWid = new QWidget(this);
    statsHeadWid->setLayout(statsHeadLayout);
    statsHeadWid->setMinimumWidth(600);

    // Prepare the stats text
    QString statsText =
        "<h2 ><b>Your Stats:</b></h2>"
        "<ul style='display: flex; flex-direction: column; gap: 15px;'>"
        "<li style='margin-bottom: 10px;text-align:left;'><b>High Score:  </b><span style='color:yellow;'>" + QString::number(highScore) + "</span></li>"
        "<li style='margin-bottom: 10px;text-align:left;'><b>Total Games Played:  </b> " + QString::number(totalGamesPlayed) + "</li>"
        "<li style='margin-bottom: 10px;text-align:left;'><b>Games Won:  </b> " + QString::number(gamesWon) + "</li>"
        "<li style='margin-bottom: 10px;text-align:left;'><b>Average Score:  </b> " + QString::number(averageScore) + "</li>"
        "<li style='margin-bottom: 10px;text-align:left;'><b>Longest Streak:  </b> " + QString::number(longestStreak) + " correct in a row</li>"
        "<li style='margin-bottom: 10px;text-align:left;'><b>Fastest Time:  </b> " + QString::number(fastestTime) + " minutes</li>"
        "<li style='margin-bottom: 10px;text-align:left;'><b>Total Correct Answers:  </b> " + QString::number(totalCorrectAnswers) + "</li>"
        "<li style='margin-bottom: 10px;text-align:left;'><b>Total Incorrect Answers:  </b> " + QString::number(totalIncorrectAnswers) + "</li>"
        "</ul><br>";

    QLabel *statsLabel = new QLabel(statsText);
    statsLabel->setWordWrap(true);
    statsLabel->setTextFormat(Qt::RichText);
    statsLabel->setStyleSheet("font-size: 16px; color: white; background-color: transparent;"); // Transparent background
    statsLabel->setAlignment(Qt::AlignCenter); // Align text to the left

    auto statsLayout = new QVBoxLayout(); // Changed to QVBoxLayout to handle multiple rows

    statsLayout->addWidget(statsLabel);

    auto mainLayout = new QVBoxLayout();
    replaceCentralWidgetLayout(mainLayout);

    mainLayout->addWidget(statsHeadWid,0,Qt::AlignCenter);

    mainLayout->addLayout(statsLayout);

    mainLayout->setContentsMargins(200, 85, 200, 100);

}

void MainWindow::onBackButtonClicked()
{
    // Clean up the pause window widgets if they exist
    if (pauseOverlay) {
        pauseOverlay->deleteLater();
        pauseOverlay = nullptr;
    }
    if (pausecontainer) {
        pausecontainer->deleteLater();
        pausecontainer = nullptr;
    }
    if (exitBtn) {
        exitBtn->deleteLater();
        exitBtn = nullptr;
    }
    if (infoBtn) {
        infoBtn->deleteLater();
        infoBtn = nullptr;
    }
    if (volumetxt) {
        volumetxt->deleteLater();
        volumetxt = nullptr;
    }
    if (volumeSlider) {
        volumeSlider->deleteLater();
        volumeSlider = nullptr;
    }
    if(pauseBtn){
        delete pauseBtn;
        pauseBtn = nullptr;
    }
    if(timer){
        delete timer;
        timer=nullptr;
    }

    // Clear the central widget
    QWidget *centralWidget = this->centralWidget();
    QLayout* layout = centralWidget->layout();
    if(layout) {
        QLayoutItem* item;
        while((item = layout->takeAt(0))) {
            QWidget* widget = item->widget();
            if(widget) {
                layout->removeWidget(widget);
                widget->deleteLater();
            }
            delete item;
        }
        delete layout;
    }
    centralWidget->setLayout(nullptr);

    // Recreate the home page layout
    ui->setupUi(this);
    setMinimumSize(MIN_WIDTH, MIN_HEIGHT);

    ui->gridLayout->setAlignment(Qt::AlignCenter);

    applyShadowEffect(ui->label, SHADOW_BLUR_RADIUS, SHADOW_COLOR);

    setupButton(ui->startBtn, "startBtn");
    setupButton(ui->statsBtn, "stats-icon.svg");
    setupButton(ui->infoBtn, "info.svg");
    setupButton(ui->settingsBtn, "settings.svg");

    QPushButton *buttons[] = {ui->startBtn, ui->statsBtn, ui->infoBtn, ui->settingsBtn};
    for (QPushButton *button : buttons)
    {
        applyShadowEffect(button, BUTTON_SHADOW_BLUR_RADIUS, SHADOW_COLOR);
        new HoverEffect(button);
    }
    resize(width()+1,height());
}

void MainWindow::pausewindow(){
    // Create the pause overlay
    pauseOverlay = new QWidget(this);
    pauseOverlay->setStyleSheet("background-color: rgba(0, 0, 0, 0.7);");
    pauseOverlay->setGeometry(0, 0, width(), height());
    pauseOverlay->show(); // Show overlay


    // Container for pause menu
    pausecontainer = new QWidget(pauseOverlay);
    pausecontainer->setGeometry(width() / 4, height() / 4, width() / 2, height() / 2);
    pausecontainer->setStyleSheet("background-color: rgba(0, 0, 0, 0.4);"
                                  "border:1px solid white;"
                                  "border-radius:10px;");
    pausecontainer->show();
    //
    QVBoxLayout *overlayLayout = new QVBoxLayout(pausecontainer);
    pausecontainer->setLayout(overlayLayout); // **Important!**

    // exit btn
    exitBtn=new QPushButton(pausecontainer);
    exitBtn->setIcon(QIcon(ICON_PATH + "exit.png"));
    exitBtn->setStyleSheet(BUTTON_STYLE);
    exitBtn->setIconSize(QSize(BUTTON_ICON_SIZE, BUTTON_ICON_SIZE));
    exitBtn->setGeometry(pausecontainer->width()/17,pausecontainer->height()/13,BUTTON_ICON_SIZE+10,BUTTON_ICON_SIZE+10);
    exitBtn->setCursor(Qt::PointingHandCursor);
    connect(exitBtn,&QPushButton::clicked,this,&MainWindow::onBackButtonClicked);
    exitBtn->show();

    // info btn
    infoBtn=new QPushButton(pausecontainer);
    infoBtn->setIcon(QIcon(ICON_PATH + "information.png"));
    infoBtn->setStyleSheet(BUTTON_STYLE);
    infoBtn->setIconSize(QSize(BUTTON_ICON_SIZE-10, BUTTON_ICON_SIZE-10));
    infoBtn->setGeometry(pausecontainer->width()-pausecontainer->width()/8,pausecontainer->height()/13,BUTTON_ICON_SIZE+10,BUTTON_ICON_SIZE+10);
    infoBtn->setCursor(Qt::PointingHandCursor);
    infoBtn->show();

    QLabel *pauseLabel = new QLabel("Pause", pausecontainer);
    pauseLabel->setStyleSheet("color: white; font-size: 24px;"
                              "border-color:transparent;");
    pauseLabel->setMaximumHeight(30);
    pauseLabel->setAlignment(Qt::AlignCenter);

    // Setup volume elements in a horizontal layout
    QHBoxLayout *volumelayout=new QHBoxLayout(pausecontainer);
    volumetxt=new QLabel("Volume:");
    volumetxt->setStyleSheet("border:none;"
                             "font: 9pt '8514oem';");
    volumeSlider = new QSlider(Qt::Horizontal, this);
    volumeSlider->setRange(0, 100);
    volumeSlider->setValue(50);  // Default volume
    volumeSlider->setCursor(Qt::PointingHandCursor);
    volumeSlider->setStyleSheet(
        "QSlider::groove:horizontal {"
        "    border: 1px solid #999999;"
        "    height: 8px;"
        "    background: #e0e0e0;"
        "    border-radius: 4px;"
        "}"
        "QSlider::sub-page:horizontal {"
        "    background: #4CAF50;"  // ✅ Volume progress color (green)
        "    border-radius: 4px;"
        "}"
        "QSlider::add-page:horizontal {"
        "    background: transparent;"  //r  ➕ Remaining slider track
        "    border-radius: 4px;"
        "}"
        "QSlider::handle:horizontal {"
        "    background: white;"
        "    border: 1px solid #5c5c5c;"
        "    width: 16px;"
        "    margin: -5px 0;"  // Center the handle vertically
        "    border-radius: 8px;"
        "}"
        "QSlider::handle:horizontal:hover {"
        "    background: #d6d6d6;"  // ✨ Hover effect
        "    border: 1px solid #3c3c3c;"
        "}"
        );

    connect(volumeSlider, &QSlider::valueChanged, this, &MainWindow::updateVolume);
    volumelayout->addWidget(volumetxt);
    volumelayout->addWidget(volumeSlider);

    // Buttons
    QPushButton *resumeBtn = new QPushButton(pausecontainer);
    setupButton(resumeBtn, "play-button.png");
    resumeBtn->setCursor(Qt::PointingHandCursor);
    connect(resumeBtn, &QPushButton::clicked, this, &MainWindow::onResumeclicked);

    QPushButton *restartBtn = new QPushButton(pausecontainer);
    setupButton(restartBtn, "reload.png");
    restartBtn->setCursor(Qt::PointingHandCursor);
    connect(restartBtn,&QPushButton::clicked,this,&MainWindow::on_restartBtn_clicked);

    QPushButton *settings = new QPushButton(pausecontainer);
    setupButton(settings, "settings.png");
    settings->setCursor(Qt::PointingHandCursor);
    connect(settings,&QPushButton::clicked,this,&MainWindow::on_settingsBtn_clicked);

    // ✅ Add buttons in a horizontal layout
    QHBoxLayout *buttonslayout = new QHBoxLayout();
    buttonslayout->addWidget(settings);
    buttonslayout->addWidget(resumeBtn);
    buttonslayout->addWidget(restartBtn);
    buttonslayout->setSpacing(40);

    // ✅ Add everything to the vertical layout
    overlayLayout->addWidget(pauseLabel);
    overlayLayout->addLayout(volumelayout);
    overlayLayout->addLayout(buttonslayout);
}

void MainWindow::updateVolume(int value) {
    volumetxt->setText(QString("Volume: %1%").arg(value));
    // If using QMediaPlayer for sound
    // mediaPlayer->setVolume(value);
}

void MainWindow::onPauseClicked(){
    if(ispaused){
        timer->startTimer();
    }else{
        timer->stopTimer();
        pausewindow();
    }
    ispaused=!ispaused;
}
void MainWindow::onResumeclicked(){
    pauseOverlay->hide();
    timer->startTimer();
}
void MainWindow::on_restartBtn_clicked(){
    timer->setTimeRemaining(10);
    pauseOverlay->hide();
}
