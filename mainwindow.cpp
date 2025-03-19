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

    // Get the existing layout from the central widget
    QVBoxLayout *mainLayout = qobject_cast<QVBoxLayout*>(ui->centralwidget->layout());

    if (!mainLayout) {  // If no layout exists, create one
        mainLayout = new QVBoxLayout(ui->centralwidget);
        ui->centralwidget->setLayout(mainLayout);
    }


    // Create a horizontal layouts
    QHBoxLayout *titlelayout = new QHBoxLayout();
    titlelayout->addWidget(ui->label);
    titlelayout->setAlignment(Qt::AlignCenter);
    titlelayout->setContentsMargins(20, 40, 100, 20); // Add padding


    QHBoxLayout *startlayout = new QHBoxLayout();
    startlayout->addStretch();
    startlayout->addWidget(ui->startBtn);
    startlayout->addStretch();
    startlayout->setContentsMargins(20, 0, 100, 20); // Add padding

    QHBoxLayout *btnslayout = new QHBoxLayout();
    btnslayout->addWidget(ui->settingsBtn);
    btnslayout->addWidget(ui->infoBtn);
    btnslayout->addWidget(ui->statsBtn);
    btnslayout->setSpacing(30);
    btnslayout->setContentsMargins(20, 50, 100, 30); // Add padding

    // Add the horizontal layout inside the main layout
    mainLayout->addLayout(titlelayout);
    mainLayout->addLayout(startlayout);
    mainLayout->addLayout(btnslayout);

    mainLayout->setSpacing(10);  // Space between widgets
    mainLayout->setContentsMargins(0.2*this->width(), 40, 100, 80); // Add padding

    // Create a drop shadow effect
    QGraphicsDropShadowEffect *shadowEffect = new QGraphicsDropShadowEffect(this);
    shadowEffect->setBlurRadius(50); // Adjust the blur radius
    shadowEffect->setColor(QColor(0, 194, 203)); // Shadow color (#00c2cb)
    shadowEffect->setOffset(0, 0);

    // Apply the shadow effect to the label
    ui->label->setGraphicsEffect(shadowEffect);

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
    domainHead->setGeometry(260,60,300,100);
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
    int xPos[] = {170, 320, 470, 320};
    int yPos[] = {140, 140, 140, 270};
    for (int i = 0; i < 4; ++i) {
        buttons[i]->setStyleSheet("background-color:transparent;");
        buttons[i]->setIconSize(QSize(120,120));
        buttons[i]->setGeometry(xPos[i],yPos[i],130,130);
        buttons[i]->setCursor(Qt::PointingHandCursor);
        connect(buttons[i], &QPushButton::clicked, this, &MainWindow::onDomainButtonClicked);
        buttons[i]->show();
    }
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
    domainNameTxt->setStyleSheet("font: 9pt '8514oem';");
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
    scoreTxt->setStyleSheet("font: 9pt '8514oem';");
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
    questionLabel->setGeometry(200,150,400,80);
    questionLabel->show();

    AnswerBox *B=new AnswerBox("khadija","A",this);
    B->show();

    /*QLabel *answerlbl1 =new QLabel(this);
    QLabel *answertxt1 =new QLabel(this);
    QLabel *answerltr1 =new QLabel(this);
    answerlbl1->move(140, 210);
    answertxt1->setText("Khadija");
    answertxt1->setGeometry(167,235,172,50);
    answerltr1->setText("A");
    answerltr1->setGeometry(150,220,32,32);

    QLabel *answerlbl2 =new QLabel(this);
    QLabel *answertxt2 =new QLabel(this);
    QLabel *answerltr2 =new QLabel(this);
    answerlbl2->move(180, 210);
    answertxt2->setText("Halima");
    answertxt2->setGeometry(167,235,172,50);
    answerltr2->setText("A");
    answerltr2->setGeometry(150,220,32,32);

    QLabel *answerlbl3 =new QLabel(this);
    QLabel *answertxt3 =new QLabel(this);
    QLabel *answerltr3 =new QLabel(this);
    answerlbl3->move(140, 250);
    answertxt3->setText("Hafsa");
    answertxt3->setGeometry(167,235,172,50);
    answerltr3->setText("A");
    answerltr3->setGeometry(150,220,32,32);

    QLabel *answerlbl4 =new QLabel(this);
    QLabel *answertxt4 =new QLabel(this);
    QLabel *answerltr4 =new QLabel(this);
    answerlbl4->move(180, 250);
    answertxt4->setText("Fatima");
    answertxt4->setGeometry(167,235,172,50);
    answerltr4->setText("A");
    answerltr4->setGeometry(150,220,32,32);*/

    /*QLabel *labels[]={answerlbl1,answerlbl2,answerlbl3,answerlbl4};
    QLabel *labelstxt[]={answertxt1,answertxt2,answertxt3,answertxt4};
    QLabel *answerltr[]={answerltr1,answerltr2,answerltr2,answerltr4};

    for (int i = 0; i < 4; ++i) {
        QPixmap pixmap2(":/images/answer.svg");
        QPixmap scaledPixmap2 = pixmap2.scaled(300, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        labels[i]->setPixmap(scaledPixmap2);
        // Set QLabel size to match the final pixmap dimensions
        labels[i]->setFixedSize(scaledPixmap2.size());
        labels[i]->show();
        labelstxt[i]->setStyleSheet("background-color:rgba(0, 255, 0, 0.5);"
                                 "border-radius:10px;"
                                 "text-align:center");
        labelstxt[i]->setAlignment(Qt::AlignCenter);
        labelstxt[i]->show();

        answerltr[i]->setStyleSheet("background-color:green;"
                                 "border-radius:16px;"
                                 "text-align:center");
        answerltr[i]->setAlignment(Qt::AlignCenter);
        answerltr[i]->show();
    }*/
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

