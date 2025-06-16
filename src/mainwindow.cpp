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
#include <QVector>
#include <QJsonObject>
#include <QJsonArray>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QPropertyAnimation>
#include <QSettings>


#include "headers/mainwindow.h"
#include "headers/settingsdialog.h"

#include "qevent.h"
#include "ui_mainwindow.h"
#include "headers/hovereffect.h"
#include "headers/answerbox.h"
#include "headers/circletimer.h"
#include "headers/typingeffect.h"
#include "headers/geminiai.h"

using namespace std;

namespace {
const int MIN_WIDTH = 900;
const int MIN_HEIGHT = 600;
const QColor SHADOW_COLOR(0, 194, 203);
const int SHADOW_BLUR_RADIUS = 50;
const int BUTTON_SHADOW_BLUR_RADIUS = 10;
const int BUTTON_ICON_SIZE = 30;
const QString BUTTON_STYLE = "background-color:transparent; border: 2px solid white; font-size:27; border-radius:5px";
const QString ICON_PATH = ":/Icons/";
}

// Constructor and Destructor
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow),
    pauseBtn(nullptr),
    backbutton(nullptr), loadingLabel(nullptr), geminiAI(nullptr),
    highScore(0), totalGamesPlayed(0), gamesWon(0),
    averageScore(0), longestStreak(0), fastestTime(0),
    totalCorrectAnswers(0), totalIncorrectAnswers(0)
{
    ui->setupUi(this);
    initializeUI();
    pauseOverlay = nullptr;
    pausecontainer = nullptr;
    exitBtn = nullptr;
    infoBtn = nullptr;
    volumetxt = nullptr;
    volumeSlider = nullptr;
    pauseBtn = nullptr;
    timer = nullptr;
}

MainWindow::~MainWindow()
{
    // Clean up dynamically allocated objects
    delete geminiAI;
    delete timer;
}

void MainWindow::initializeUI()
{
    if (!connectToDatabase()) {
        QMessageBox::critical(this, "Database Error", "Failed to connect to database!");
    }

    setMinimumSize(MIN_WIDTH, MIN_HEIGHT);
    ui->gridLayout->setAlignment(Qt::AlignCenter);

    applyShadowEffect(ui->label, SHADOW_BLUR_RADIUS, SHADOW_COLOR);

    // Initialize buttons
    setupMainButtons();
}

template<typename T>
void MainWindow::clearWidgets()
{
    for (auto widget : this->findChildren<T*>()) {
        widget->deleteLater();
    }
}

void MainWindow::setupMainButtons()
{
    setupButton(ui->startBtn, "startBtn");
    setupButton(ui->statsBtn, "stats-icon.svg");
    setupButton(ui->infoBtn, "info.svg");
    setupButton(ui->settingsBtn, "settings.svg");

    QPushButton *buttons[] = {ui->startBtn, ui->statsBtn, ui->infoBtn, ui->settingsBtn};
    for (QPushButton *button : buttons) {
        applyShadowEffect(button, BUTTON_SHADOW_BLUR_RADIUS, SHADOW_COLOR);
        new HoverEffect(button);
    }
}

// UI Helper Methods
void MainWindow::applyShadowEffect(QWidget *widget, int blurRadius, const QColor &color)
{
    // Delete existing effect if any
    if (widget->graphicsEffect()) {
        delete widget->graphicsEffect();
    }
    auto shadowEffect = new QGraphicsDropShadowEffect(widget);
    shadowEffect->setBlurRadius(blurRadius);
    shadowEffect->setColor(color);
    shadowEffect->setOffset(0, 0);
    widget->setGraphicsEffect(shadowEffect);
}

void MainWindow::setupButton(QPushButton *button, const QString &iconName)
{
    button->setIcon(QIcon(ICON_PATH + iconName));
    button->setStyleSheet(BUTTON_STYLE);
    button->setIconSize(QSize(BUTTON_ICON_SIZE, BUTTON_ICON_SIZE));
}

void MainWindow::deleteButtons(const std::initializer_list<QPushButton *> &buttons)
{
    for (QPushButton *button : buttons) {
        if (button) {
            button->deleteLater();
        }
    }
}

QLabel *MainWindow::createLabel(const QString &text, int fontSize, Qt::Alignment alignment)
{
    auto label = new QLabel(text, this);
    label->setStyleSheet(QString("font-size: %1px; color: white;").arg(fontSize));
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

QPushButton *MainWindow::createDomainButton(const QString &iconName)
{
    auto button = new QPushButton(this);
    button->setIcon(QIcon(ICON_PATH + iconName));
    button->setStyleSheet("background-color:transparent;");
    button->setIconSize(QSize(130, 140));
    button->setCursor(Qt::PointingHandCursor);
    return button;
}

void MainWindow::replaceCentralWidgetLayout(QLayout *newLayout)
{
    QWidget *centralWidget = this->centralWidget();
    QLayout *oldLayout = centralWidget->layout();

    // Clear existing layout and widgets
    if (oldLayout) {
        QLayoutItem *item;
        while ((item = oldLayout->takeAt(0))) {
            if (QWidget *widget = item->widget()) {
                widget->setParent(nullptr);
                widget->deleteLater();
            }
            delete item;
        }
        delete oldLayout;
    }

    centralWidget->setLayout(newLayout);
}

// Event Handlers
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
    updateWidgetPositionsOnResize();
}

void MainWindow::updateWidgetPositionsOnResize()
{
    if (pauseBtn) {
        pauseBtn->move(this->width()/13, this->height()/13);
    }
    if (pauseOverlay) {
        pauseOverlay->setGeometry(0, 0, this->width(), this->height());
    }
    if (pausecontainer) {
        pausecontainer->setGeometry(width() / 4, height() / 4, width() / 2, height() / 2);
    }
    if (exitBtn && pausecontainer) {
        exitBtn->setGeometry(pausecontainer->width()/17, pausecontainer->height()/13,
                             BUTTON_ICON_SIZE+10, BUTTON_ICON_SIZE+10);
    }
    if (infoBtn && pausecontainer) {
        infoBtn->setGeometry(pausecontainer->width()-pausecontainer->width()/8, pausecontainer->height()/13,
                             BUTTON_ICON_SIZE+10, BUTTON_ICON_SIZE+10);
    }
    if (backbutton) {
        backbutton->setGeometry(width()/7, height()/6, 90, 30);
    }
    if (loadingLabel) {
        int x = (this->width() - loadingLabel->width()) / 2;
        int y = (this->height() - loadingLabel->height()) / 2;
        loadingLabel->setGeometry(x, y, loadingLabel->width(), loadingLabel->height());
    }
}

void MainWindow::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QPixmap pixmap(":/images/quiz-background.jpg");
    painter.drawPixmap(0, 0, width(), height(), pixmap);
}

// Main Game Flow Methods
void MainWindow::on_startBtn_clicked()
{
    deleteButtons({ui->startBtn, ui->statsBtn, ui->infoBtn, ui->settingsBtn});

    auto waitLabel = createLabel("Please wait", 20, Qt::AlignCenter);
    TypingAnimation *typingEffect = new TypingAnimation(waitLabel, waitLabel->text(), 200, this);
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

    startProgressBar(progressBar, waitLabel, typingEffect);
}

void MainWindow::startProgressBar(QProgressBar *progressBar, QLabel *waitLabel, TypingAnimation *typingEffect)
{
    auto *progressTimer = new QTimer(this);
    int duration = 1000;
    int interval = 50;
    int steps = duration / interval;
    int stepSize = 100 / steps;
    int progress = 0;

    connect(progressTimer, &QTimer::timeout, this, [=]() mutable {
        progress += stepSize;
        progressBar->setValue(progress);

        if (progress >= 100) {
            progressTimer->stop();
            progressTimer->deleteLater();
            typingEffect->stop();
            ui->label->hide();
            waitLabel->hide();
            progressBar->hide();
            showDomainSelectionScreen();
        }
    });
    progressTimer->start(interval);
}

void MainWindow::createBackButton()
{



    backbutton = new QPushButton(this);
    backbutton->setObjectName("backButton");  // Important for safe lookup
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
    backbutton->setGeometry(width()/7,height()/6,90,30);
    backbutton->show();
}

void MainWindow::showDomainSelectionScreen()
{
    createBackButton();

    auto domainHead = createLabel("Choose your domain: ", 28, Qt::AlignCenter);
    applyShadowEffect(domainHead, BUTTON_SHADOW_BLUR_RADIUS, SHADOW_COLOR);

    geminiAI = new GeminiAI(this);

    // Create domain buttons
    auto generalDomain = createDomainButton("general.svg");
    auto logicDomain = createDomainButton("logic.svg");
    auto techDomain = createDomainButton("tech.svg");
    auto entertainDomain = createDomainButton("entertain.svg");

    QPushButton *buttons[] = {generalDomain, logicDomain, techDomain, entertainDomain};
    for (QPushButton *button : buttons)
    {
        button->setCursor(Qt::PointingHandCursor);
        button->setMaximumWidth(140);
        button->setMaximumHeight(180);
        connect(button, &QPushButton::clicked, this, &MainWindow::onDomainButtonClicked);
        button->show();
    }

    // Set object names for identification
    generalDomain->setObjectName("general");
    logicDomain->setObjectName("logic");
    techDomain->setObjectName("tech");
    entertainDomain->setObjectName("entertain");

    setupDomainSelectionLayout(domainHead, buttons);
}

void MainWindow::setupDomainSelectionLayout(QLabel *domainHead, QPushButton *buttons[])
{
    auto domainHeadLayout = new QHBoxLayout();
    domainHeadLayout->addWidget(domainHead, Qt::AlignCenter);

    QWidget *domainheadwid = new QWidget(this);
    domainheadwid->setLayout(domainHeadLayout);
    domainheadwid->setMinimumWidth(600);

    auto domainsLayout = new QVBoxLayout();
    auto firstRowLayout = new QHBoxLayout();
    auto secondRowLayout = new QHBoxLayout();

    // Distribute buttons into rows
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
    mainLayout->addWidget(domainheadwid, 0, Qt::AlignCenter);
    mainLayout->addLayout(domainsLayout);
    mainLayout->setContentsMargins(100, 100, 100, 100);

    // Adjust layout based on available space
    adjustDomainLayout(firstRowLayout, secondRowLayout, buttons);
}

void MainWindow::adjustDomainLayout(QHBoxLayout *firstRowLayout, QHBoxLayout *secondRowLayout, QPushButton *buttons[])
{
    int availableWidth = this->width() - 200;
    int buttonWidth = buttons[0]->maximumWidth() + firstRowLayout->spacing();
    int buttonsPerRow = availableWidth / buttonWidth;

    if (buttonsPerRow < 4)
    {
        firstRowLayout->removeWidget(buttons[3]);
        secondRowLayout->addWidget(buttons[3]);
    }
}

void MainWindow::onDomainButtonClicked()
{
    clearWidgets<QPushButton>();
    clearWidgets<QLabel>();
    if (pauseBtn)
        delete pauseBtn;
    if (timer)
        delete timer;
    QPushButton *clickedButton = qobject_cast<QPushButton *>(sender());
    if (!clickedButton)
        return;
    QString domain = getDomainFromButton(clickedButton);

    // Create loading indicator with Retro Gaming Style
    loadingLabel = new QLabel("Loading", this);
    loadingLabel->setAlignment(Qt::AlignCenter);
    loadingLabel->setStyleSheet(
        "QLabel {"
        "   font-family: 'Courier New', 'Monaco', monospace;"
        "   font-size: 20px;"
        "   font-weight: bold;"
        "   color: #00ff00;"      // Bright green
        "   border-radius: 4px;"
        "   padding: 12px 20px;"
        "   text-shadow: 0 0 8px #00ff00;"
        "   text-align:right;"
        "}"
        );

    // Add glow effect animation
    QGraphicsOpacityEffect* opacityEffect = new QGraphicsOpacityEffect(loadingLabel);
    loadingLabel->setGraphicsEffect(opacityEffect);
    QPropertyAnimation* pulseAnimation = new QPropertyAnimation(opacityEffect, "opacity");
    pulseAnimation->setDuration(800);
    pulseAnimation->setStartValue(0.4);
    pulseAnimation->setEndValue(1.0);
    pulseAnimation->setEasingCurve(QEasingCurve::InOutQuad);
    pulseAnimation->setLoopCount(-1);
    pulseAnimation->start();

    // Position loading label (calculate after styling for proper size)
    loadingLabel->adjustSize();
    int x = (this->width() - loadingLabel->width()) / 2;
    int y = (this->height() - loadingLabel->height()) / 2;
    loadingLabel->setGeometry(x,y,loadingLabel->width()+70,loadingLabel->height());

    // Create typing animation with retro feel
    typingEffect = new TypingAnimation(loadingLabel, "Loading...", 180, this);
    typingEffect->start();
    loadingLabel->show();

    // Generate questions
    generateQuestionsForDomain(domain);
}

QString MainWindow::getDomainFromButton(QPushButton *button)
{
    if (button->objectName() == "entertain")
        return "Entertainment & Pop Culture";
    else if (button->objectName() == "general")
        return "General Knowledge";
    else if (button->objectName() == "tech")
        return "Tech & Coding";
    else if (button->objectName() == "logic")
        return "Logic & Brain Teasers";

    return "General Knowledge"; // Default
}

void MainWindow::generateQuestionsForDomain(const QString &domain)
{
    QString prompt = "Generate an other 5 multiple-choice questions in the domain of " + domain +
                     " in a STRICT JSON format.The response MUST be a JSON array containing JSON objects. " +
                     "Each object represents a question and must have the following keys:" +
                     "- question (string): The question text. Please minimize escaped characters." +
                     "- options (array of strings): An array of four possible answer options." +
                     "- correct_answer (string): The correct answer from the options." +
                     "Ensure the JSON is valid and UTF-8 encoded. Do NOT include any preamble text or " +
                     "Markdown code blocks. Just the raw JSON.";

    geminiAI->askQuestion(prompt, domain);
    qDebug() << "Generating questions for domain: " << domain;

    connect(geminiAI, &GeminiAI::responseReceived, this, [=]() {
        qDebug() << "GeminiAI response received! Now loading questions.";
        if (loadingLabel) {
            loadingLabel->hide();
            typingEffect->stop();
        }
            startQuizSession(domain, sessionId+1);
    });
}


void MainWindow::startQuizSession(const QString &domain, int session) {
    // Reset score and streak for new session
    currentScore = 0;
    correctStreak = 0;
    // sav
    createPauseButton();
    quizQuestions.clear(); // Clear previous questions, if any
    quizQuestions = geminiAI->getQuestionsFromDB(); // Retrieve *all* questions
    currentQuestionIndex = 0; // Start at the first question

    if (quizQuestions.isEmpty()) {
        QMessageBox::warning(this, "No Questions", "No questions found for this domain.");
        return;  // Stop the quiz if there are no questions
    }

    setupQuizLayout(domain, session); // Initial UI setup.
    displayQuestion(currentQuestionIndex); // display the first question

}

void MainWindow::createPauseButton()
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
    ispaused = false;
    connect(pauseBtn, &QPushButton::clicked, this, &MainWindow::onPauseClicked);
}

// 2.2 - Change setupQuizLayout to setup the GridLayout
void MainWindow::setupQuizLayout(const QString &domain, int session)
{
    // Check for existing layout
    // Create header elements (domain name, score, timer)
    // These are STATIC and won't change with each question
    auto domainNameTxt = createLabel("Domain: ", 10, Qt::AlignLeft);
    domainNameTxt->setStyleSheet("color:yellow; font: 9pt '8514oem';");

    auto domainName = createLabel(domain, 10, Qt::AlignLeft);
    domainName->setStyleSheet("font:20pt 'Terminal';");

    // Create timer
    timer = new CircleTimer(this);
    timer->setFixedSize(60, 60);
    timer->setStyleSheet("margin:14px;");
    timer->show();

    // Create score display
    auto scoreTxt = createLabel("Score: ", 10, Qt::AlignRight);
    scoreTxt->setStyleSheet("color:yellow; font: 15pt 'Terminal';");

    score = createLabel("0", 10, Qt::AlignRight);
    score->setStyleSheet("font: 12pt 'Terminal';");

    auto mainLayout = qobject_cast<QVBoxLayout *>(ui->centralwidget->layout());
    if (!mainLayout)
    {
        mainLayout = new QVBoxLayout(ui->centralwidget);
        ui->centralwidget->setLayout(mainLayout);
    }


    // Create domain and score widgets
    auto domainwidget = new QWidget;
    domainwidget->setFixedSize(200, 80);

    auto scorewidget = new QWidget;
    scorewidget->setFixedSize(180, 80);

    // Create layouts
    auto headLayout = new QHBoxLayout();
    auto scorelayout = new QHBoxLayout(scorewidget);
    auto domainlayout = new QHBoxLayout(domainwidget);

    // Add widgets to domain layout
    domainlayout->addWidget(domainNameTxt);
    domainlayout->addWidget(domainName);

    // Add widgets to score layout
    scorelayout->addWidget(scoreTxt);
    scorelayout->addWidget(score);
    scorelayout->setAlignment(Qt::AlignRight);

    // Add widgets to head layout
    headLayout->addSpacing(20);
    headLayout->addWidget(domainwidget, Qt::AlignLeft);
    headLayout->addWidget(timer, Qt::AlignCenter);
    headLayout->addWidget(scorewidget, Qt::AlignRight);
    headLayout->addSpacing(20);

    auto headWidget = new QWidget(this);
    headWidget->setLayout(headLayout);
    headWidget->setMinimumWidth(700);
    headWidget->setMinimumHeight(70);

    mainLayout->addWidget(headWidget, 0, Qt::AlignCenter);
    mainLayout->addSpacing(15);

    // Create answer grid layout
    answersLayout = new QGridLayout(); // Here the magic starts
    answersLayout->setSpacing(20);
    mainLayout->addSpacing(15);
    mainLayout->addLayout(answersLayout);
    mainLayout->setContentsMargins(100, 80, 100, 80);
}

void MainWindow::displayQuestion(int index) {
    // Clear existing widgets (question label, answer boxes)
    clearWidgets<AnswerBox>();
    if (auto label = findChild<QLabel*>("questionLabel"))
        label->deleteLater();

    // Check if index is within the valid range
    if (index < 0 || index >= quizQuestions.size()) {
        qDebug() << "Invalid question index: " << index;
        endQuizSession(); // End the quiz if the index is out of bounds
        return;
    }

    // Record when question is displayed
    answerTime = QTime::currentTime();

    if (timer) {
        timer->setTimeRemaining(15); // Reset the timer
        timer->startTimer(); // Restart timer
        connect(timer, &CircleTimer::timeout, this, [this]() {
            disconnect(timer, &CircleTimer::timeout, this, nullptr);
            // Time's up - treat as incorrect answer
            handleAnswer(false);
        });
    }
    question = quizQuestions[index]; // Get the current question

    // Create question label
    auto questionLabel = createLabel(question["question"].toString(), 15, Qt::AlignCenter);
    applyShadowEffect(questionLabel, SHADOW_BLUR_RADIUS, SHADOW_COLOR);
    questionLabel->setStyleSheet("font: 15pt 'Terminal';");
    questionLabel->setMaximumWidth(700);
    questionLabel->setWordWrap(true);
    questionLabel->setObjectName("questionLabel");  // Important for finding it later

    // Create answer boxes
    AnswerBox *B5 = new AnswerBox(question["correct_answer"].toString(), "E", -1, this);
    AnswerBox *B1 = new AnswerBox(question["options"][0].toString(), "A", -1, this);
    AnswerBox *B2 = new AnswerBox(question["options"][1].toString(), "B", -1, this);
    AnswerBox *B3 = new AnswerBox(question["options"][2].toString(), "C", -1, this);
    AnswerBox *B4 = new AnswerBox(question["options"][3].toString(), "D", -1, this);

    connect(B1, &AnswerBox::clicked, this, &MainWindow::onAnswerBoxClicked);
    connect(B2, &AnswerBox::clicked, this, &MainWindow::onAnswerBoxClicked);
    connect(B3, &AnswerBox::clicked, this, &MainWindow::onAnswerBoxClicked);
    connect(B4, &AnswerBox::clicked, this, &MainWindow::onAnswerBoxClicked);

    // Add to mainLayout
    QVBoxLayout* mainLayout = qobject_cast<QVBoxLayout*>(ui->centralwidget->layout());
    if (mainLayout) {
        // Chercher l’index de answersLayout
        int answersIndex = -1;
        for (int i = 0; i < mainLayout->count(); ++i) {
            QLayoutItem* item = mainLayout->itemAt(i);
            if (item && item->layout() == answersLayout) {
                answersIndex = i;
                break;
            }
        }

        if (answersIndex != -1) {
            mainLayout->insertWidget(answersIndex, questionLabel);  // Question AVANT les réponses
        } else {
            mainLayout->addWidget(questionLabel); // Fallback
        }

        if (answersLayout) {
            answersLayout->addWidget(B1, 1, 0);
            answersLayout->addWidget(B2, 1, 1);
            answersLayout->addWidget(B3, 2, 0);
            answersLayout->addWidget(B4, 2, 1);
        }

        B1->show();
        B2->show();
        B3->show();
        B4->show();
    }

    timer->startTimer();
}

void MainWindow::handleAnswer(bool isCorrect) {
    // Calculate response time (seconds)
    int responseTime = answerTime.secsTo(QTime::currentTime());
    int pointsEarned = 0;

    if (isCorrect) {
        pointsEarned = 10; // Base points

        // Time bonus (answered within 5 seconds)
        if (responseTime <= 5) {
            pointsEarned += 5;
        }

        // Streak bonus
        if (correctStreak > 0) {
            int streakBonus = 2 * correctStreak;
            pointsEarned += streakBonus;
        }

        correctStreak++;
    } else {
        correctStreak = 0; // Reset streak on wrong answer
    }

    // Update score
    currentScore += pointsEarned;
    updateScoreDisplay();

    // Create feedback label with rich styling
    QLabel* feedbackLabel = new QLabel(this);
    feedbackLabel->setAlignment(Qt::AlignCenter);
    feedbackLabel->setAttribute(Qt::WA_DeleteOnClose);

    // Base style with semi-transparent background
    QString baseStyle = "QLabel {"
                        "    font-family: 'Arial';"
                        "    font-size: 18px;"
                        "    font-weight: bold;"
                        "    padding: 15px 25px;"
                        "    border-radius: 15px;"
                        "    margin: 5px;"
                        "    background-color: rgba(0, 0, 0, 150);"  // Semi-transparent black background
                        "}";

    // Conditional styling based on correctness
    if (isCorrect) {
        feedbackLabel->setStyleSheet(baseStyle +
                                     "color: #ffffff;"
                                     "border: 2px solid #00ff00;");
    } else {
        feedbackLabel->setStyleSheet(baseStyle +
                                     "color: #ffffff;"
                                     "border: 2px solid #ff0000;");
    }

    // Add drop shadow for depth
    QGraphicsDropShadowEffect* shadow = new QGraphicsDropShadowEffect(feedbackLabel);
    shadow->setBlurRadius(15);
    shadow->setOffset(3, 3);
    shadow->setColor(QColor(0, 0, 0, 200));
    feedbackLabel->setGraphicsEffect(shadow);

    // Build the feedback text with HTML formatting
    QString feedbackText = QString("<div style='text-align:center;'>"
                                   "<p style='font-size:24px; margin-bottom:10px;'>"
                                   "%1</p>"
                                   "<p style='font-size:16px;'>"
                                   "Base: %2</p>").arg(
                                   isCorrect ? "✓ Correct!" : "✗ Incorrect",
                                   isCorrect ? "+10" : "+0");

    // Add time bonus if applicable
    if (isCorrect && responseTime <= 5) {
        feedbackText += QString("<p style='font-size:16px; color:%1;'>"
                                "Speed Bonus: +5</p>").arg("#88ff88");
    }

    // Add streak bonus if applicable
    if (isCorrect && correctStreak > 0) {
        feedbackText += QString("<p style='font-size:16px; color:%1;'>"
                                "Streak Bonus: +%2</p>").arg("#88ff88",
                                 QString::number(2 * (correctStreak - (isCorrect ? 0 : 1))));
    }

    feedbackText += QString("<p style='font-size:20px; margin-top:10px; font-weight:bold; color:%1;'>"
                            "Total: +%2</p></div>").arg(isCorrect ? "#00ff00" : "#ff0000",
                             QString::number(pointsEarned));

    feedbackLabel->setText(feedbackText);
    feedbackLabel->adjustSize();

    // Position the label centered above the answer boxes
    int x = (width() - feedbackLabel->width()) / 2;
    int y = height() / 2 - feedbackLabel->height() - 20;  // Position above center
    feedbackLabel->move(x, y);
    feedbackLabel->show();

    // Animate the appearance
    QPropertyAnimation* fadeIn = new QPropertyAnimation(feedbackLabel, "windowOpacity");
    fadeIn->setDuration(500);
    fadeIn->setStartValue(0);
    fadeIn->setEndValue(1);
    fadeIn->start(QAbstractAnimation::DeleteWhenStopped);

    // Move to next question after delay
    QTimer::singleShot(1500, this, [this, feedbackLabel]() {
        feedbackLabel->deleteLater();
        currentQuestionIndex++;
        if (currentQuestionIndex < quizQuestions.size()) {
            displayQuestion(currentQuestionIndex);
        } else {
            endQuizSession();
        }
    });
}

void MainWindow::updateScoreDisplay() {
    if (score) {
        score->setText(QString::number(currentScore));

        // Simple animation using stylesheet changes
        score->setStyleSheet("font: bold 20pt 'Terminal'; color: #00FF00;");
        QTimer::singleShot(300, this, [this]() {
            score->setStyleSheet("font: bold 20pt 'Terminal'; color: white;");
        });
    }
}


void MainWindow::saveHighScore(int newScore) {
    QSettings settings("MyCompany", "MyApp");
    // Get current high score (default to 0 if none exists)
    int currentHigh = settings.value("highScore", 0).toInt();

    // Update if new score is higher
    if (newScore > currentHigh) {
        settings.setValue("highScore", newScore);
    }
}


void MainWindow::endQuizSession() {
    QSettings settings("MyCompany", "MyApp");
    int previousHigh = settings.value("highScore", 0).toInt();
    bool isNewHighScore = (currentScore > previousHigh);

    if (isNewHighScore) {
        settings.setValue("highScore", currentScore);
    }

    // Create container with explicit lifetime management
    QWidget* container = new QWidget();
    container->setWindowFlags(Qt::Widget); // Ensure it's not a window
    container->setGeometry(rect());
    container->setParent(this); // Attach to main window
    container->setAttribute(Qt::WA_DeleteOnClose); // Auto-delete when closed

    // Create results label
    QLabel* resultsLabel = new QLabel(container);
    resultsLabel->setAlignment(Qt::AlignCenter);
    resultsLabel->setWordWrap(true);

    // Base style
    QString style = "QLabel {"
                    "    font-family: 'Arial';"
                    "    font-size: 18px;"
                    "    font-weight: bold;"
                    "    padding: 15px 25px;"
                    "    border-radius: 15px;"
                    "    margin: 5px;"
                    "    background-color: rgba(0, 0, 0, 150);"  // Semi-transparent black background
                    "}";

    // Add border based on score
    if (isNewHighScore) {
        style += "border: 3px solid #4CAF50;";
    } else {
        style += "border: 2px solid #2196F3;";
    }
    style += "}";

    resultsLabel->setStyleSheet(style);

    // Add shadow effect
    QGraphicsDropShadowEffect* shadow = new QGraphicsDropShadowEffect(resultsLabel);
    shadow->setBlurRadius(20);
    shadow->setOffset(4, 4);
    shadow->setColor(QColor(0, 0, 0, 200));
    resultsLabel->setGraphicsEffect(shadow);

    // Build the results text
    QString resultsText = QString("<div style='text-align:center;'>"
                                  "<p style='font-size:28px; margin-bottom:15px;'>"
                                  "%1</p>"
                                  "<p style='font-size:20px;'>Your score: <b>%2</b></p>"
                                  "<p style='font-size:20px;'>High score: <b>%3</b></p>")
                              .arg(isNewHighScore ? "🎉 New High Score! 🎉" : "Quiz Completed",
                                   QString::number(currentScore),
                                   QString::number(qMax(currentScore, previousHigh)));

    if (isNewHighScore) {
        resultsText += QString("<p style='font-size:24px; color:#4CAF50; margin-top:15px;'>"
                               "Congratulations!</p>");
    }
    resultsText += "</div>";

    resultsLabel->setText(resultsText);
    resultsLabel->adjustSize();
    resultsLabel->move((width() - resultsLabel->width())/2,
                       (height() - resultsLabel->height())/3);

    // Disable interactive elements
    for (auto btn : findChildren<QPushButton*>()) btn->setEnabled(false);
    for (auto box : findChildren<AnswerBox*>()) box->setEnabled(false);
    if (timer) timer->stopTimer();

    container->raise();
    container->show();

    // Use a member variable to track the container
    m_endQuizContainer = container; // QPointer<QWidget> m_endQuizContainer; in header

    // Capture by value, not reference
    QTimer::singleShot(3000, this, [this]() {
        this->cleanupQuizSession();
    });
}

void MainWindow::cleanupQuizSession() {
    // Safe cleanup method
    if (m_endQuizContainer) {
        m_endQuizContainer->close(); // Will trigger delete due to WA_DeleteOnClose
        m_endQuizContainer = nullptr;
    }

    // Clear other widgets
    clearWidgets<AnswerBox>();

    if (pauseBtn) {
        pauseBtn->deleteLater();
        pauseBtn = nullptr;
    }

    if (timer) {
        timer->deleteLater();
        timer = nullptr;
    }

    if (backbutton) {
        backbutton->deleteLater();
        backbutton = nullptr;
    }

    // Clear central widget
    QWidget* central = centralWidget();
    if (central) {
        QLayout* layout = central->layout();
        if (layout) {
            QLayoutItem* item;
            while ((item = layout->takeAt(0))) {
                if (QWidget* widget = item->widget()) {
                    widget->setParent(nullptr);
                    widget->deleteLater();
                }
                delete item;
            }
        }
        // Don't delete the layout - it's owned by the central widget
    }

    showDomainSelectionScreen();
}
void MainWindow::createQuizPageLayout(QLabel *domainNameTxt, QLabel *domainName, QLabel *scoreTxt,
                                      QLabel *score, QLabel *questionLabel,
                                      AnswerBox *B1, AnswerBox *B2, AnswerBox *B3, AnswerBox *B4)
{
    auto mainLayout = qobject_cast<QVBoxLayout *>(ui->centralwidget->layout());
    if (!mainLayout)
    {
        mainLayout = new QVBoxLayout(ui->centralwidget);
        ui->centralwidget->setLayout(mainLayout);
    }

    // Create domain and score widgets
    auto domainwidget = new QWidget;
    domainwidget->setFixedSize(200, 80);

    auto scorewidget = new QWidget;
    scorewidget->setFixedSize(180, 80);

    // Create layouts
    auto headLayout = new QHBoxLayout();
    auto scorelayout = new QHBoxLayout(scorewidget);
    auto domainlayout = new QHBoxLayout(domainwidget);

    // Add widgets to domain layout
    domainlayout->addWidget(domainNameTxt);
    domainlayout->addWidget(domainName);

    // Add widgets to score layout
    scorelayout->addWidget(scoreTxt);
    scorelayout->addWidget(score);
    scorelayout->setAlignment(Qt::AlignRight);

    // Add widgets to head layout
    headLayout->addSpacing(20);
    headLayout->addWidget(domainwidget, Qt::AlignLeft);
    headLayout->addWidget(timer, Qt::AlignCenter);
    headLayout->addWidget(scorewidget, Qt::AlignRight);
    headLayout->addSpacing(20);

    auto headWidget = new QWidget(this);
    headWidget->setLayout(headLayout);
    headWidget->setMinimumWidth(700);
    headWidget->setMinimumHeight(70);

    // Create answer grid layout
    auto answersLayout = new QGridLayout();
    answersLayout->addWidget(B1, 1, 0);
    answersLayout->addWidget(B2, 1, 1);
    answersLayout->addWidget(B3, 2, 0);
    answersLayout->addWidget(B4, 2, 1);

    // Build final layout
    mainLayout->addWidget(headWidget, 0, Qt::AlignCenter);
    mainLayout->addSpacing(15);
    mainLayout->addWidget(questionLabel);
    mainLayout->addSpacing(15);
    mainLayout->addLayout(answersLayout);
    mainLayout->setContentsMargins(100, 80, 100, 80);
    setLayout(mainLayout);
}

void MainWindow::onAnswerBoxClicked(AnswerBox *box) {
    // Disable all answer boxes to prevent multiple clicks
    for (auto btn : this->findChildren<AnswerBox *>()) {
        btn->setEnabled(false);
    }

    // Highlight the selected answer
    box->getTextlabel()->setStyleSheet("background-color: rgba(0,0,0,0.3);");
    box->setclicked(1);

    // Highlight correct answer in green and incorrect ones in red
    QString correctAnswer = question["correct_answer"].toString();
    for (auto btn : this->findChildren<AnswerBox *>()) {
        if (btn->getText() == correctAnswer) {
            btn->getTextlabel()->setStyleSheet(
                "background-color: rgba(0,255,0,100);"
                "border: 2px solid #00FF00;"
                );
        } else {
            btn->getTextlabel()->setStyleSheet(
                "background-color: rgba(255,0,0,100);"
                "border: 2px solid #FF0000;"
                );
        }
    }

    // Stop and disconnect the timer
    if (timer) {
        timer->stopTimer();
        disconnect(timer, &CircleTimer::timeout, this, nullptr);
    }

    // Check if the selected answer is correct
    bool isCorrect = (box->getText() == correctAnswer);

    // Add visual feedback for the selected answer
    if (isCorrect) {
        box->getTextlabel()->setStyleSheet(
            "background-color: rgba(0,255,0,150);"
            "border: 3px solid #00FF00;"
            );
    } else {
        box->getTextlabel()->setStyleSheet(
            "background-color: rgba(255,0,0,150);"
            "border: 3px solid #FF0000;"
            );
    }

    // Process the answer
    handleAnswer(isCorrect);
}

QJsonObject MainWindow::generateQuestion(QString domain, int session) {
    QVector<QJsonObject> questions = geminiAI->getQuestionsFromDB();

    if (questions.isEmpty()) {
        qDebug() << "❌ No questions available in the database.";
        QJsonObject q;
        q["question"] = "No questions available.";
        return q;
    }

    QVector <QJsonObject> q = questions.mid(questions.size()-3,questions.size());

    qDebug() << "Domain of the question:" << q[session]["domain"].toString();
    qDebug() << "? Question:" << q[session]["question"].toString();
    qDebug() << "# Options:" << q[session]["options"].toArray();
    qDebug() << "$ Correct answer:" << q[session]["correct_answer"].toString();

    return q[session];
}

// Info and Stats Pages
void MainWindow::on_infoBtn_clicked()
{
    deleteButtons({ui->startBtn, ui->statsBtn, ui->infoBtn, ui->settingsBtn});
    ui->label->deleteLater();

    createBackButton();

    auto infoHead = createLabel("Instructions", 28, Qt::AlignCenter);
    applyShadowEffect(infoHead, BUTTON_SHADOW_BLUR_RADIUS, SHADOW_COLOR);

    // Instructions content
    QString instructions = createInstructionsContent();

    QLabel *instructionsLabel = new QLabel(instructions);
    instructionsLabel->setWordWrap(true);
    instructionsLabel->setTextFormat(Qt::RichText);
    instructionsLabel->setStyleSheet("font-size: 16px; color: white; background-color: transparent;");
    instructionsLabel->setAlignment(Qt::AlignLeft);

    QScrollArea *scrollArea = new QScrollArea;
    scrollArea->setWidgetResizable(true);
    scrollArea->setStyleSheet("background-color: transparent; border: none;");

    QWidget *scrollContent = new QWidget();
    scrollContent->setStyleSheet("background-color: transparent;");
    QVBoxLayout *scrollLayout = new QVBoxLayout(scrollContent);
    scrollLayout->addWidget(instructionsLabel);
    scrollContent->setLayout(scrollLayout);
    scrollArea->setWidget(scrollContent);
    scrollLayout->setAlignment(Qt::AlignCenter);

    auto infoHeadLayout = new QHBoxLayout();
    infoHeadLayout->addWidget(infoHead, Qt::AlignCenter);

    QWidget *infoheadwid = new QWidget(this);
    infoheadwid->setLayout(infoHeadLayout);
    infoheadwid->setMinimumWidth(600);

    auto infosLayout = new QVBoxLayout();
    infosLayout->addWidget(scrollArea);

    auto mainLayout = new QVBoxLayout();
    replaceCentralWidgetLayout(mainLayout);
    mainLayout->addWidget(infoheadwid, 0, Qt::AlignCenter);
    mainLayout->addLayout(infosLayout);
    mainLayout->setContentsMargins(100, 100, 100, 100);
}

QString MainWindow::createInstructionsContent()
{
    return "<h2><b style='font-size:100px;'>Instructions for QuizGame</b></h2>"
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
           "<li style= 'text-align:left;'><span style='color:orange;'>Streak bonus: +2 points</span> for each consecutive correct answer</li>"
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

           "<h3 style='color:yellow;'>End of Game</h3>"
           "<ul>"
           "<li style='text-align:left;'>View your final score</li>"
           "<li style='text-align:left;'>See correct answers for missed questions</li>"
           "<li style='text-align:left;'><b>Play Again</b>: Start a new game with the same settings</li>"
           "<li style='text-align:left;'><b>Return to Main Menu</b>: Exit to the home screen</li>"
           "</ul>"

           "<h3 style='color:yellow;'>Settings Options</h3>"
           "<ul>"
           "<li style='text-align:left;'>Toggle sound effects and background music</li>"
           "<li style='text-align:left;'>Adjust difficulty level</li>"
           "</ul>"
           "<p style= 'text-align:center;font-size:15px;'>©Ayoub, All rights reserved 2025 </p><br>";
}

void MainWindow::on_statsBtn_clicked(){
    deleteButtons({ui->startBtn, ui->statsBtn, ui->infoBtn, ui->settingsBtn});
    ui->label->deleteLater();

    createBackButton();

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
        "<li style='margin-bottom: 10px;text-align:left;'><b>High Score:  </b><span>" + QString::number(highScore) + "</span></li>"
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
    if(backbutton){
        backbutton->deleteLater();
        backbutton=nullptr;
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

    // exit Btn
    exitBtn = new QPushButton(pausecontainer);
    exitBtn->setIcon(QIcon(ICON_PATH + "exit.png"));
    exitBtn->setStyleSheet(BUTTON_STYLE);
    exitBtn->setIconSize(QSize(BUTTON_ICON_SIZE, BUTTON_ICON_SIZE));
    exitBtn->setGeometry(pausecontainer->width()/17,pausecontainer->height()/13,BUTTON_ICON_SIZE+10,BUTTON_ICON_SIZE+10);
    exitBtn->setCursor(Qt::PointingHandCursor);
    exitBtn->setMinimumSize(80, 30);
    connect(exitBtn, &QPushButton::clicked, this, &MainWindow::onBackButtonClicked);
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

void MainWindow::on_settingsBtn_clicked()
{
    SettingsDialog settingsDialog(this);
    settingsDialog.exec();
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
bool MainWindow::connectToDatabase()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("mydatabase.db");

    if (!db.open()) {
        qDebug() << "❌ Error: Could not open database" << db.lastError();
        return false;
    }

    if (!createMcqTable()) {
        qDebug() << "❌ Error: Could not create table";
        return false;
    }

    return true;
}

bool MainWindow::createMcqTable()
{
    QSqlQuery query;
    QString createTableQuery = R"(
        CREATE TABLE IF NOT EXISTS mcq_questions (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            question TEXT,
            domain TEXT,
            option1 TEXT,
            option2 TEXT,
            option3 TEXT,
            option4 TEXT,
            correct_answer TEXT
        )
    )";

    if (!query.exec(createTableQuery)) {
        qDebug() << " Failed to create table:" << query.lastError().text();
        return false;
    }

    return true;
}
