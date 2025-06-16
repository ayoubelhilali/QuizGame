#include "headers/circletimer.h"
#include "headers/answerbox.h"
#include "headers/mainwindow.h"
#include <QPainter>
#include <QFont>
#include <QDebug>

CircleTimer::CircleTimer(QWidget *parent) : QWidget(parent), timeRemaining(15), color(QColor(0, 255, 0)) {
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &CircleTimer::updateTimer);
}

void CircleTimer::startTimer() {
    timer->start(1000);  // Start timer with 1-second intervals
}

void CircleTimer::stopTimer() {
    timer->stop();  // Start timer with 1-second intervals
}

//In circletimer.cpp
void CircleTimer::updateTimer() {
    MainWindow *mainWin = nullptr;
    QObject *parentObj = this->parent();
    while (parentObj) {
        mainWin = qobject_cast<MainWindow *>(parentObj);
        if (mainWin) break;  // Found MainWindow, exit loop
        parentObj = parentObj->parent();
    }

    if (!mainWin) return; // Exit if MainWindow is not found

    if (timeRemaining > 0) {
        timeRemaining--;
        update();  // Refresh UI
        if (this->gettime() <= 3) {
            setProgressColor(QColor(255, 0, 0));
        }else{
            setProgressColor(QColor(0, 255, 0));
        }
    } else {
        if (mainWin) {
            for (auto answers : mainWin->findChildren<AnswerBox *>()) {
                if (answers->getLetter() == "E") {
                    correctAnswer = answers->getText(); // Save it for later
                    break;
                }
            }

            for (auto answers : mainWin->findChildren<AnswerBox *>()) {
                answers->setEnabled(false);
                if (answers->getclicked() == 1) {
                    answers->showresult(answers->checkcorrect(correctAnswer));
                }
            }
            for (auto answers : mainWin->findChildren<AnswerBox *>()) {
                answers->setEnabled(false);
                answers->showresult(answers->checkcorrect(correctAnswer));
            }
            //✅ Modify onTimerFinished
            QTimer::singleShot(2000, this, [mainWin]() {
                int currentIndex = mainWin->getCurrentQuestionIndex();
                QVector<QJsonObject> quizQuestions = mainWin->getQuizQuestions();

                if (currentIndex < quizQuestions.size() - 1) {
                    mainWin->setCurrentQuestionIndex(currentIndex+1);
                    mainWin->displayQuestion(mainWin->getCurrentQuestionIndex());
                } else {
                    mainWin->endQuizSession();
                }
            });
        }
        if (timer) {  // Ensure timer is valid
            timer->stop();
        }
        emit timeout();  // Emit the signal when time runs out
        // Check the last clicked button
    }
}

void CircleTimer::onTimerFinished() {
    setTimeRemaining(15);
    // Get reference to MainWindow (assuming it's the top-level parent)
}



void CircleTimer::setProgressColor(QColor c=QColor(0, 255,0)){
    color=c;
}

QColor CircleTimer::getProgressColor(){
    return color;
}

void CircleTimer::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // Set the size
    int size = qMin(width(), height()) - 10;  // Ensure padding
    QRect rect((width() - size) / 2, (height() - size) / 2, size, size);

    // Draw the background circle
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(180, 180, 180,90));  // Slightly darker gray background
    painter.drawEllipse(rect);

    // Draw the countdown arc
    painter.setBrush(Qt::NoBrush);
    QPen pen(this->getProgressColor(), 4);  // Green progress arc
    painter.setPen(pen);

    int angle = (timeRemaining / 15.0) * 360 * 16;  // Convert to degrees (16x for Qt)
    painter.drawArc(rect, 90 * 16, -angle);  // Start from top

    // Draw the timer text
    painter.setPen(this->getProgressColor());
    QFont font("Arial", size / 3, QFont::Bold);  // Increase font size
    painter.setFont(font);
    painter.drawText(rect, Qt::AlignCenter, QString::number(timeRemaining));
}
