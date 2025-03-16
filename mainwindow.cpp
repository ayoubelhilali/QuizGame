#include "mainwindow.h"
#include "settingsdialog.h"
#include "ui_mainwindow.h"
#include <QTimer>
#include <QProgressBar>
#include <QLabel>
#include <QPainter>
#include <QGraphicsDropShadowEffect>
#include <QMessageBox>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

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
    int duration = 2000;  // 5 seconds
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

    // Apply the shadow effect to the startBtn
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
    QLabel *domainName=new QLabel(this);
    domainName->setText("Domain: " + domain);
    domainName->show();
}

void MainWindow::onDomainButtonClicked() {
    // Identify which button was clicked
    QPushButton *clickedButton = qobject_cast<QPushButton*>(sender());
    if (!clickedButton) return;

    if (clickedButton->objectName() == "entertain") {
        questionsPage("entertain");
    } else if (clickedButton->objectName()=="general") {
        questionsPage("general");
    } else if (clickedButton->objectName()=="tech") {
        questionsPage("tech");
    } else if (clickedButton->objectName()=="logic") {
        questionsPage("logic");
    }
}

void MainWindow::on_settingsBtn_clicked()
{
    SettingsDialog settingsDialog(this);
    settingsDialog.exec();    // Open the settings as a modal window
}

