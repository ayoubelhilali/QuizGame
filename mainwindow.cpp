#include <QTimer>
#include <QProgressBar>
#include <QLabel>
#include <QPainter>
#include <QGraphicsDropShadowEffect>
#include <QMessageBox>
#include <QPixmap>
#include <QGridLayout>
#include <QDebug>

#include "mainwindow.h"
#include "settingsdialog.h"
#include "ui_mainwindow.h"
#include "hovereffect.h"
#include "answerbox.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setMinimumSize(800,500);

    ui->gridLayout->setAlignment(Qt::AlignCenter);

    // Create a drop shadow effect
    QGraphicsDropShadowEffect *shadowEffect = new QGraphicsDropShadowEffect(this);
    shadowEffect->setBlurRadius(50); // Adjust the blur radius
    shadowEffect->setColor(QColor(0, 194, 203)); // Shadow color (#00c2cb)
    shadowEffect->setOffset(0, 0);

    // Apply the shadow effect to the label
    ui->label->setGraphicsEffect(shadowEffect);

    ui->startBtn->setStyleSheet("background-color:transparent;"
                                "border: 2px solid white;"
                                "font-size:27;"
                                "border-radius:5px");


    // Stats Icon
    ui->statsBtn->setIcon(QIcon(":/images/Icons/stats-icon.svg"));
    ui->statsBtn->setStyleSheet("background-color:transparent;"
                                "border: 2px solid white;"
                                "font-size:100px;"
                                "border-radius:5px");
    ui->statsBtn->setIconSize(QSize(30,30));

    // Info Button
    ui->infoBtn->setIcon(QIcon(":/images/Icons/info.svg"));
    ui->infoBtn->setStyleSheet("background-color:transparent;"
                                "border: 2px solid white;"
                                "font-size:100px;"
                                "border-radius:5px");
    ui->infoBtn->setIconSize(QSize(30,30));

    // settings Icon
    ui->settingsBtn->setIcon(QIcon(":/images/Icons/settings.svg"));
    ui->settingsBtn->setStyleSheet("background-color:transparent;"
                               "border: 2px solid white;"
                               "font-size:100px;"
                               "border-radius:5px");
    ui->settingsBtn->setIconSize(QSize(30,30));

    QPushButton *buttons[]={ui->startBtn,ui->statsBtn,ui->infoBtn,ui->settingsBtn};
    for (int i = 0; i < 4; ++i) {
        // Create a drop shadow effect
        QGraphicsDropShadowEffect *shadowEffect = new QGraphicsDropShadowEffect(this);
        shadowEffect->setBlurRadius(10); // Adjust the blur radius
        shadowEffect->setColor(QColor(0, 194, 203)); // Shadow color (#00c2cb)
        shadowEffect->setOffset(0, 0); // No offset

        // Apply the shadow effect to the buttons
        buttons[i]->setGraphicsEffect(shadowEffect);
    }

    // Hover Effect t
    new HoverEffect(ui->startBtn);
    new HoverEffect(ui->statsBtn);
    new HoverEffect(ui->infoBtn);
    new HoverEffect(ui->settingsBtn);

    resize(800,500);

}

void MainWindow::resizeEvent(QResizeEvent *event){
    QSize parentSize = event->size();

    // Get the QWidget that contains the layout (make sure this widget holds the layout)
    QWidget *layoutContainer = ui->container;  // The widget containing the layout

    // Get the size of the layout container widget
    QSize layoutSize = layoutContainer->size();

    // Calculate the center position
    int x = (parentSize.width() - layoutSize.width()) / 2;
    int y = (parentSize.height() - layoutSize.height()) / 2;

    // Move the layout container widget to the center of the parent window
    layoutContainer->move(x, y);

    // Call the base class implementation if necessary
    QWidget::resizeEvent(event);
}


void MainWindow::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QPixmap pixmap(":/images/quiz-background.jpg");
    painter.drawPixmap(0, 0, width(), height(), pixmap); // Resizes dynamically
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_startBtn_clicked() {
    // delete the btn after clicked
    delete ui->startBtn;
    delete ui->statsBtn;
    delete ui->infoBtn;
    delete ui->settingsBtn;

    // Create the label dynamically
    QLabel *waitlabel=new QLabel(this);
    waitlabel->setText("Please wait ...");
    waitlabel->setGeometry(360,80,300,300);
    waitlabel->setStyleSheet("font-size:20px;");
    waitlabel->show();

    // create the progress Bar dynamically
    QProgressBar *progressBar=new QProgressBar(this);
    progressBar->setGeometry(200,250,400,30);
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
        "}"
    );
    progressBar->setRange(0,100);
    progressBar->setValue(0);
    progressBar->show();

    QHBoxLayout *waitlayout=new QHBoxLayout(this);
    waitlayout->addWidget(ui->label);
    waitlayout->addWidget(waitlabel);
    waitlayout->addWidget(progressBar);

    QTimer *timer = new QTimer(this);  // Create a timer
    int duration = 1000;  // 5 seconds
    int interval = 50;  // Update every 50ms
    int steps = duration / interval;  // Total steps
    int stepSize = 100 / steps;  // Progress increase per step
    int *progress = new int(0);  // Track progress

    connect(timer, &QTimer::timeout, this, [=]() mutable {
        if (*progress < 100) {
            *progress += stepSize;
            progressBar->setValue(*progress);
        } else {
            timer->stop();  // Stop when reaching 100%
            delete progress;
            delete waitlabel;
            delete progressBar;
            delete ui->label;
            DomainsChoose();
        }
    });
    timer->start(interval);  // Start the timer
};

void MainWindow::DomainsChoose(){
    // Create domain heading
    QLabel *domainHead=new QLabel(this);
    domainHead->setText("Choose your domain: ");
    domainHead->setStyleSheet("font-size:28px;"
                              "background-color:transparent;");
    // Create a drop shadow effect
    QGraphicsDropShadowEffect *shadowEffect5 = new QGraphicsDropShadowEffect(this);
    shadowEffect5->setBlurRadius(10); // Adjust the blur radius
    shadowEffect5->setColor(QColor(0, 194, 203)); // Shadow color (#00c2cb)
    shadowEffect5->setOffset(0, 0); // No offset

    domainHead->setGraphicsEffect(shadowEffect5);
    domainHead->show();

    // Create Domains buttons
    QPushButton *generalDomain = new QPushButton(this);
    generalDomain->setIcon(QIcon(":/images/general.svg"));

    QPushButton *logicDomain = new QPushButton(this);
    logicDomain->setIcon(QIcon(":/images/logic.svg"));


    // Create Domains buttons
    QPushButton *techDomain = new QPushButton(this);
    techDomain->setIcon(QIcon(":/images/tech.svg"));


    // Create Domains buttons
    QPushButton *entertainDomain = new QPushButton(this);
    entertainDomain->setIcon(QIcon(":/images/entertain.svg"));

    // Set properties
    QPushButton *buttons[] = {generalDomain, logicDomain, techDomain, entertainDomain};
    for (int i = 0; i < 4; ++i) {
        buttons[i]->setStyleSheet("background-color:transparent;");
        buttons[i]->setIconSize(QSize(120,120));
        buttons[i]->setCursor(Qt::PointingHandCursor);
        connect(buttons[i], &QPushButton::clicked, this, &MainWindow::onDomainButtonClicked);
        buttons[i]->show();
    }

    // posistionate with Qgrid & Qvboxlayout:
    QHBoxLayout *domainheadlayout=new QHBoxLayout(this);
    domainheadlayout->addWidget(domainHead);
    domainheadlayout->setAlignment(Qt::AlignCenter);

    QGridLayout *domainslayout=new QGridLayout(this);
    domainslayout->addWidget(generalDomain,0,0);
    domainslayout->addWidget(logicDomain,0,1);
    domainslayout->addWidget(techDomain,0,2);
    domainslayout->addWidget(entertainDomain,1,0);
    QVBoxLayout *mainLayout=new QVBoxLayout(this);

    if (this->centralWidget()->layout()) {
        // Remove the existing layout
        QLayout *oldLayout = this->centralWidget()->layout();
        delete oldLayout;  // Delete the old layout to avoid memory leaks
    }
    mainLayout->addLayout(domainheadlayout);
    mainLayout->addLayout(domainslayout);
    mainLayout->setContentsMargins(100,100,100,100);
    this->centralWidget()->setLayout(mainLayout);

    // Assign names to buttons
    generalDomain->setObjectName("general");
    logicDomain->setObjectName("logic");
    techDomain->setObjectName("tech");
    entertainDomain->setObjectName("entertain");

}

void MainWindow::questionsPage(const QString &domain){


    QLabel *domainNameTxt=new QLabel(this);
    domainNameTxt->setText("Domain: ");
    domainNameTxt->setGeometry(120,100,80,30);
    domainNameTxt->setStyleSheet("font: 9pt '8514oem';"
                                 "color:yellow;");
    domainNameTxt->show();

    QLabel *domainName=new QLabel(this);
    domainName->setText(domain);
    domainName->setGeometry(200,85,250,60);
    domainName->setStyleSheet("font: 9pt '8514oem';");
    domainName->setWordWrap(true);
    domainName->show();

    QLabel *scoreTxt=new QLabel(this);
    scoreTxt->setText("Score: ");
    scoreTxt->setGeometry(540,100,100,30);
    scoreTxt->setStyleSheet("font: 9pt '8514oem';"
                            "color:yellow;");
    scoreTxt->show();

    QLabel *score=new QLabel(this);
    score->setText("100");
    score->setGeometry(600,100,100,30);
    score->setStyleSheet("font: 9pt '8514oem';");
    score->show();

    QLabel *questionLabel =new QLabel(this);
    questionLabel->setText("What is the first muslim woman in the world?");
    questionLabel->setStyleSheet("font: 15pt 'Terminal';");
    questionLabel->setWordWrap(true);
    questionLabel->setAlignment(Qt::AlignCenter);
    questionLabel->setGeometry(200,150,400,80);
    questionLabel->show();



    AnswerBox *B1=new AnswerBox("khadija","A",1,this);
    B1->show();
    AnswerBox *B2=new AnswerBox("Halima","B",0,this);
    B2->show();
    AnswerBox *B3=new AnswerBox("Fatima","C",-1,this);
    B3->show();
    AnswerBox *B4=new AnswerBox("Aicha","D",-1,this);
    B4->show();

    // Get the existing layout from the central widget
    QVBoxLayout *mainLayout = qobject_cast<QVBoxLayout*>(ui->centralwidget->layout());

    if (!mainLayout) {  // If no layout exists, create one
        mainLayout = new QVBoxLayout(ui->centralwidget);
        ui->centralwidget->setLayout(mainLayout);
    }

    QHBoxLayout *headlayout=new QHBoxLayout(this);
    headlayout->addSpacing(20);
    headlayout->addWidget(domainNameTxt);
    headlayout->addWidget(domainName);
    headlayout->addStretch();
    headlayout->addWidget(scoreTxt);
    headlayout->addWidget(score);
    headlayout->addSpacing(20);

    // Create a wrapper widget
    QWidget *headWidget = new QWidget(this); // Assuming 'this' is the parent widget

    // Set the QHBoxLayout as the layout of the wrapper widget
    headWidget->setLayout(headlayout);

    // Set the maximum width for the wrapper widget
    int maximumWidth = 900; // Replace 500 with your desired maximum width in pixels
    headWidget->setMaximumWidth(maximumWidth);


    QGridLayout *answerslayout =new QGridLayout(this);
    answerslayout->addWidget(B1,1,0);
    answerslayout->addWidget(B2,1,1);
    answerslayout->addWidget(B3,2,0);
    answerslayout->addWidget(B4,2,1);

    mainLayout->addWidget(headWidget);
    mainLayout->addSpacing(15);
    mainLayout->addWidget(questionLabel);
    mainLayout->addSpacing(15);
    mainLayout->addLayout(answerslayout);
    mainLayout->setContentsMargins(140,80,140,80);
    setLayout(mainLayout);
}

void MainWindow::onDomainButtonClicked() {
    // Clean the window:
    QList<QPushButton*> buttons = this->findChildren<QPushButton*>();
    for (QPushButton *btn : buttons) {
        btn->hide();       // Hide button immediately
        btn->deleteLater();
    }
    QList<QLabel*> labels = this->findChildren<QLabel*>();
    for (QLabel *label : labels) {
        label->hide();        // Hide the label
        label->deleteLater();
    }
    // Identify which button was clicked
    QPushButton *clickedButton = qobject_cast<QPushButton*>(sender());
    if (!clickedButton) return;
    if (clickedButton->objectName() == "entertain") {
        questionsPage("Entertainment & Pop Culture ");
    } else if (clickedButton->objectName()=="general") {
        questionsPage("General Knowledge");
    } else if (clickedButton->objectName()=="tech") {
        questionsPage("Tech & Coding");
    } else if (clickedButton->objectName()=="logic") {
        questionsPage("Logic & Brain Teasers");
    }


}

void MainWindow::on_settingsBtn_clicked()
{
    SettingsDialog settingsDialog(this);
    settingsDialog.exec();    // Open the settings as a modal window
}

