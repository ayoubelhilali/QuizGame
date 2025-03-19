/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QPushButton *startBtn;
    QLabel *label;
    QPushButton *statsBtn;
    QPushButton *infoBtn;
    QPushButton *settingsBtn;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(878, 584);
        MainWindow->setStyleSheet(QString::fromUtf8(""));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        startBtn = new QPushButton(centralwidget);
        startBtn->setObjectName("startBtn");
        startBtn->setGeometry(QRect(330, 240, 174, 41));
        QFont font;
        font.setPointSize(16);
        startBtn->setFont(font);
        startBtn->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        startBtn->setStyleSheet(QString::fromUtf8("border: 2px solid white; /* Initial border color */\n"
"border-radius: 5px;\n"
"width:150px;\n"
"    "));
        label = new QLabel(centralwidget);
        label->setObjectName("label");
        label->setGeometry(QRect(250, 100, 271, 71));
        QFont font1;
        font1.setFamilies({QString::fromUtf8("Stencil")});
        font1.setPointSize(48);
        font1.setBold(false);
        font1.setItalic(false);
        label->setFont(font1);
        label->setCursor(QCursor(Qt::CursorShape::ArrowCursor));
        label->setStyleSheet(QString::fromUtf8("font-size: 50px;\n"
"font-weight: bold;\n"
"font: 48pt \"Stencil\";\n"
"color: white;\n"
""));
        statsBtn = new QPushButton(centralwidget);
        statsBtn->setObjectName("statsBtn");
        statsBtn->setGeometry(QRect(520, 340, 80, 51));
        QFont font2;
        font2.setPointSize(39);
        statsBtn->setFont(font2);
        statsBtn->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        infoBtn = new QPushButton(centralwidget);
        infoBtn->setObjectName("infoBtn");
        infoBtn->setGeometry(QRect(370, 340, 80, 51));
        infoBtn->setFont(font2);
        infoBtn->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        settingsBtn = new QPushButton(centralwidget);
        settingsBtn->setObjectName("settingsBtn");
        settingsBtn->setGeometry(QRect(210, 340, 80, 122));
        settingsBtn->setFont(font2);
        settingsBtn->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        settingsBtn->setStyleSheet(QString::fromUtf8("padding:20px;"));
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 878, 17));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        startBtn->setText(QCoreApplication::translate("MainWindow", "Start", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "Quiz Game", nullptr));
        statsBtn->setText(QString());
        infoBtn->setText(QString());
        settingsBtn->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
