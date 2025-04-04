#include <QTimer>
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
}

void MainWindow::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QPixmap pixmap(":/images/quiz-background.jpg");
    painter.drawPixmap(0, 0, width(), height(), pixmap);
}

MainWindow::~MainWindow()
{
}

void MainWindow::on_startBtn_clicked()
{
    deleteButtons({ui->startBtn, ui->statsBtn, ui->infoBtn, ui->settingsBtn});

    auto waitLabel = createLabel("Please wait ...", 20, Qt::AlignCenter);
    TypingAnimation *typingEffect = new TypingAnimation(waitLabel, 30); // Speed: 50ms per character
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
        delete centralWidget->layout();
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
    if (!pauseBtn) {  // Prevent creating multiple times
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
    }


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

void MainWindow::onBackButtonClicked()
{
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

    resize(MIN_WIDTH+10, MIN_HEIGHT);

}

void MainWindow::pausewindow(){
    // Create the pause overlay
    pauseOverlay = new QWidget(this);
    pauseOverlay->setStyleSheet("background-color: rgba(0, 0, 0, 0.7);");
    pauseOverlay->setGeometry(0, 0, width(), height());
    pauseOverlay->show(); // Show overlay

    // Container for pause menu
    QWidget *pausecontainer = new QWidget(pauseOverlay);
    pausecontainer->setGeometry(width() / 4, height() / 4, width() / 2, height() / 2);
    pausecontainer->setStyleSheet("background-color: rgba(0, 0, 0, 0.4);"
                                  "border:1px solid white;"
                                  "border-radius:10px;");
    pausecontainer->show();
    // ✅ Set Layout Correctly
    QVBoxLayout *overlayLayout = new QVBoxLayout(pausecontainer);
    pausecontainer->setLayout(overlayLayout); // **Important!**

    QLabel *pauseLabel = new QLabel("Pause", pausecontainer);
    pauseLabel->setStyleSheet("color: white; font-size: 24px;"
                              "border:none;");
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
        "    background: #bfbfbf;"  // ➕ Remaining slider track
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
