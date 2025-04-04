/********************************************************************************
** Form generated from reading UI file 'settingsdialog.ui'
**
** Created by: Qt User Interface Compiler version 6.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SETTINGSDIALOG_H
#define UI_SETTINGSDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_SettingsDialog
{
public:
    QPushButton *closeBtn;
    QCheckBox *SoundCheckBox;
    QLabel *soundLbl;
    QLabel *musicLbl;
    QCheckBox *MusicCheckBox;
    QComboBox *LangBox;
    QLabel *LangLbl;

    void setupUi(QDialog *SettingsDialog)
    {
        if (SettingsDialog->objectName().isEmpty())
            SettingsDialog->setObjectName("SettingsDialog");
        SettingsDialog->resize(541, 374);
        closeBtn = new QPushButton(SettingsDialog);
        closeBtn->setObjectName("closeBtn");
        closeBtn->setGeometry(QRect(90, 80, 121, 31));
        closeBtn->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        closeBtn->setStyleSheet(QString::fromUtf8("background-color:transparent;\n"
"border:1px solid white;\n"
"border-radius:10px;\n"
"spacing:10px;\n"
"padding:5px 5px;"));
        QIcon icon(QIcon::fromTheme(QIcon::ThemeIcon::MediaSeekBackward));
        closeBtn->setIcon(icon);
        SoundCheckBox = new QCheckBox(SettingsDialog);
        SoundCheckBox->setObjectName("SoundCheckBox");
        SoundCheckBox->setGeometry(QRect(330, 138, 20, 20));
        QFont font;
        font.setPointSize(13);
        SoundCheckBox->setFont(font);
        SoundCheckBox->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        SoundCheckBox->setStyleSheet(QString::fromUtf8("border:none;\n"
"margin-left:30px;"));
        soundLbl = new QLabel(SettingsDialog);
        soundLbl->setObjectName("soundLbl");
        soundLbl->setGeometry(QRect(120, 140, 101, 16));
        QFont font1;
        font1.setFamilies({QString::fromUtf8("8514oem")});
        font1.setPointSize(9);
        font1.setBold(false);
        font1.setItalic(false);
        soundLbl->setFont(font1);
        soundLbl->setStyleSheet(QString::fromUtf8("font: 9pt \"8514oem\";"));
        musicLbl = new QLabel(SettingsDialog);
        musicLbl->setObjectName("musicLbl");
        musicLbl->setGeometry(QRect(120, 190, 101, 16));
        musicLbl->setFont(font1);
        musicLbl->setStyleSheet(QString::fromUtf8("font: 9pt \"8514oem\";"));
        MusicCheckBox = new QCheckBox(SettingsDialog);
        MusicCheckBox->setObjectName("MusicCheckBox");
        MusicCheckBox->setGeometry(QRect(330, 190, 20, 20));
        MusicCheckBox->setFont(font);
        MusicCheckBox->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        MusicCheckBox->setStyleSheet(QString::fromUtf8("border:none;\n"
"margin-left:30px;"));
        LangBox = new QComboBox(SettingsDialog);
        LangBox->addItem(QString());
        LangBox->addItem(QString());
        LangBox->addItem(QString());
        LangBox->setObjectName("LangBox");
        LangBox->setGeometry(QRect(270, 240, 82, 31));
        LangBox->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        LangBox->setStyleSheet(QString::fromUtf8("background-color:white;\n"
"color:blue;\n"
"max-width:150px;\n"
"padding: 5px 3px;\n"
"margin-right:100px;\n"
""));
        LangLbl = new QLabel(SettingsDialog);
        LangLbl->setObjectName("LangLbl");
        LangLbl->setGeometry(QRect(120, 240, 101, 21));
        LangLbl->setFont(font1);
        LangLbl->setStyleSheet(QString::fromUtf8("font: 9pt \"8514oem\";"));

        retranslateUi(SettingsDialog);

        QMetaObject::connectSlotsByName(SettingsDialog);
    } // setupUi

    void retranslateUi(QDialog *SettingsDialog)
    {
        SettingsDialog->setWindowTitle(QCoreApplication::translate("SettingsDialog", "Dialog", nullptr));
        closeBtn->setText(QCoreApplication::translate("SettingsDialog", "       Retour", nullptr));
        SoundCheckBox->setText(QString());
        soundLbl->setText(QCoreApplication::translate("SettingsDialog", "Sound:", nullptr));
        musicLbl->setText(QCoreApplication::translate("SettingsDialog", "Music:", nullptr));
        MusicCheckBox->setText(QString());
        LangBox->setItemText(0, QCoreApplication::translate("SettingsDialog", "Anglais", nullptr));
        LangBox->setItemText(1, QCoreApplication::translate("SettingsDialog", "Francais", nullptr));
        LangBox->setItemText(2, QCoreApplication::translate("SettingsDialog", "Arabe", nullptr));

        LangLbl->setText(QCoreApplication::translate("SettingsDialog", "Language:", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SettingsDialog: public Ui_SettingsDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SETTINGSDIALOG_H
