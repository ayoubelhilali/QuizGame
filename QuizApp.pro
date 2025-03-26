QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

TARGET = QuizApp.pro

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/answerbox.cpp \
    src/circletimer.cpp \
    src/hovereffect.cpp \
    src/main.cpp \
    src/mainwindow.cpp \
    src/questionsgenerator.cpp \
    src/settingsdialog.cpp \

HEADERS += \
    headers/answerbox.h \
    headers/circletimer.h \
    headers/hovereffect.h \
    headers/mainwindow.h \
    headers/questionsgenerator.h \
    headers/settingsdialog.h \

FORMS += \
    mainwindow.ui \
    settingsdialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    images/login-image.png \
    images/main-background.jpg

RESOURCES += \
    resources.qrc
