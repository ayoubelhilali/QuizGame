#include "settingsdialog.h"
#include "ui_SettingsDialog.h"
#include <QPainter>
#include <QSettings>
#include <iostream>
using namespace std ;

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog) {
    ui->setupUi(this);  // Load the designed UI
    setWindowTitle("Settings");
    setWindowIcon(QIcon(":/images/Icons/settings.svg"));
    setFixedSize(600,400);

    loadSettings(); // Load saved settings

    // Connect signals to save settings when changed
    connect(ui->SoundCheckBox, &QCheckBox::toggled, this, &SettingsDialog::saveSettings);
    connect(ui->MusicCheckBox, &QCheckBox::toggled, this, &SettingsDialog::saveSettings);
    connect(ui->LangBox, &QComboBox::currentIndexChanged, this, &SettingsDialog::saveSettings);


    // styling the checkbox
    qApp->setStyleSheet(
        "QCheckBox {"
        "    font-size: 16px;"
        "    color: white;"
        "   border:2px solid black;"
        "}"
        "QCheckBox::indicator {"
        "    width: 20px;"
        "    height: 20px;"
        "}"
        "QCheckBox::indicator:unchecked {"
        "    background-color: white;"
        "}"
        "QCheckBox::indicator:checked {"
        "   background-color:white;"
        "   image: url(:/images/Icons/checked.png);"
        "}"
        );
    // Example: Connect a close button
    connect(ui->closeBtn, &QPushButton::clicked, this, &QDialog::accept);
}

void SettingsDialog::saveSettings() {
    QSettings settings("MyCompany", "MyApp");
    settings.setValue("sound", ui->SoundCheckBox->isChecked());
    settings.setValue("Music", ui->MusicCheckBox->isChecked());
    settings.setValue("language", ui->LangBox->currentIndex());
}

void SettingsDialog::loadSettings() {
    QSettings settings("MyCompany", "MyApp");
    ui->SoundCheckBox->setChecked(settings.value("sound", true).toBool());  // Default: true
    ui->MusicCheckBox->setChecked(settings.value("vibration", true).toBool());  // Default: true
    int index=settings.value("language",0).toInt();
    ui->LangBox->setCurrentIndex(index);
}

void SettingsDialog::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QPixmap pixmap(":/images/quiz-background.jpg");
    painter.drawPixmap(0, 0, width(), height(), pixmap); // Resizes dynamically
}

SettingsDialog::~SettingsDialog() {
    delete ui;
}
