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
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QWidget *container;
    QWidget *layoutWidget;
    QGridLayout *gridLayout;
    QPushButton *startBtn;
    QPushButton *infoBtn;
    QPushButton *statsBtn;
    QPushButton *settingsBtn;
    QLabel *label;
    QSpacerItem *horizontalSpacer;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->setEnabled(true);
        MainWindow->resize(621, 391);
        MainWindow->setMinimumSize(QSize(0, 0));
        MainWindow->setStyleSheet(QString::fromUtf8(""));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        container = new QWidget(centralwidget);
        container->setObjectName("container");
        container->setGeometry(QRect(0, 0, 621, 391));
        layoutWidget = new QWidget(container);
        layoutWidget->setObjectName("layoutWidget");
        layoutWidget->setGeometry(QRect(0, 0, 621, 373));
        gridLayout = new QGridLayout(layoutWidget);
        gridLayout->setObjectName("gridLayout");
        gridLayout->setVerticalSpacing(60);
        gridLayout->setContentsMargins(0, 0, 0, 20);
        startBtn = new QPushButton(layoutWidget);
        startBtn->setObjectName("startBtn");
        startBtn->setMinimumSize(QSize(140, 45));
        startBtn->setMaximumSize(QSize(170, 45));
        startBtn->setBaseSize(QSize(100, 100));
        QFont font;
        font.setFamilies({QString::fromUtf8("Sigmar One")});
        font.setPointSize(20);
        font.setStrikeOut(false);
        font.setKerning(false);
        font.setStyleStrategy(QFont::NoAntialias);
        font.setHintingPreference(QFont::PreferFullHinting);
        startBtn->setFont(font);
        startBtn->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        startBtn->setAutoFillBackground(false);
        startBtn->setStyleSheet(QString::fromUtf8("startBtn{\n"
"	padding: 10 20px;\n"
"	font-weight:normal;\n"
"}"));

        gridLayout->addWidget(startBtn, 1, 1, 1, 1);

        infoBtn = new QPushButton(layoutWidget);
        infoBtn->setObjectName("infoBtn");
        infoBtn->setMinimumSize(QSize(100, 50));
        infoBtn->setMaximumSize(QSize(120, 50));
        infoBtn->setBaseSize(QSize(100, 100));
        QFont font1;
        font1.setPointSize(39);
        font1.setKerning(false);
        infoBtn->setFont(font1);
        infoBtn->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));

        gridLayout->addWidget(infoBtn, 3, 0, 1, 1);

        statsBtn = new QPushButton(layoutWidget);
        statsBtn->setObjectName("statsBtn");
        statsBtn->setMinimumSize(QSize(100, 50));
        statsBtn->setMaximumSize(QSize(110, 50));
        statsBtn->setBaseSize(QSize(100, 100));
        QFont font2;
        font2.setPointSize(39);
        statsBtn->setFont(font2);
        statsBtn->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));

        gridLayout->addWidget(statsBtn, 3, 1, 1, 1, Qt::AlignmentFlag::AlignHCenter|Qt::AlignmentFlag::AlignVCenter);

        settingsBtn = new QPushButton(layoutWidget);
        settingsBtn->setObjectName("settingsBtn");
        settingsBtn->setMinimumSize(QSize(100, 50));
        settingsBtn->setMaximumSize(QSize(120, 50));
        settingsBtn->setBaseSize(QSize(100, 100));
        settingsBtn->setFont(font2);
        settingsBtn->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));

        gridLayout->addWidget(settingsBtn, 3, 2, 1, 1, Qt::AlignmentFlag::AlignVCenter);

        label = new QLabel(layoutWidget);
        label->setObjectName("label");
        QFont font3;
        font3.setFamilies({QString::fromUtf8("Stencil")});
        font3.setPointSize(48);
        font3.setBold(false);
        font3.setItalic(false);
        label->setFont(font3);
        label->setCursor(QCursor(Qt::CursorShape::ArrowCursor));
        label->setStyleSheet(QString::fromUtf8("font-size: 50px;\n"
"font-weight: bold;\n"
"font: 48pt \"Stencil\";\n"
"color: white;\n"
""));

        gridLayout->addWidget(label, 0, 0, 1, 3, Qt::AlignmentFlag::AlignHCenter|Qt::AlignmentFlag::AlignVCenter);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        gridLayout->addItem(horizontalSpacer, 2, 0, 1, 3);

        MainWindow->setCentralWidget(centralwidget);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        startBtn->setText(QCoreApplication::translate("MainWindow", "start", nullptr));
        infoBtn->setText(QString());
        statsBtn->setText(QString());
        settingsBtn->setText(QString());
        label->setText(QCoreApplication::translate("MainWindow", "Quiz Game", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
