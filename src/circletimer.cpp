#include "headers/circletimer.h"
#include "headers/answerbox.h"
#include "headers/mainwindow.h"
#include <QPainter>
#include <QFont>

CircleTimer::CircleTimer(QWidget *parent) : QWidget(parent), timeRemaining(10), color(QColor(0, 255, 0)) {
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &CircleTimer::updateTimer);
}

void CircleTimer::startTimer() {
    timer->start(1000);  // Start timer with 1-second intervals
}

void CircleTimer::stopTimer() {
    timer->stop();  // Start timer with 1-second intervals
}

void CircleTimer::updateTimer() {
    // Get reference to MainWindow (assuming it's the top-level parent)
    MainWindow *mainWin = nullptr;
    QObject *parentObj = this->parent();
    while (parentObj) {
        mainWin = qobject_cast<MainWindow *>(parentObj);
        if (mainWin) break;  // Found MainWindow, exit loop
        parentObj = parentObj->parent();
    }

    if (timeRemaining > 0) {
        timeRemaining--;
        update();  // Refresh UI
        if (this->gettime() <= 3) {
            setProgressColor(QColor(255, 0, 0));  // Change color to red
        }else{
            setProgressColor(QColor(0, 255, 0));
        }
    } else {
        if (mainWin) {  // Ensure mainWin is valid
            for (auto answers : mainWin->findChildren<AnswerBox *>()) {
                answers->setEnabled(false);  // Disable buttons
            }
        }
        if (timer) {  // Ensure timer is valid
            timer->stop();
        }
        // Check the last clicked button
    }
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

    int angle = (timeRemaining / 10.0) * 360 * 16;  // Convert to degrees (16x for Qt)
    painter.drawArc(rect, 90 * 16, -angle);  // Start from top

    // Draw the timer text
    painter.setPen(this->getProgressColor());
    QFont font("Arial", size / 3, QFont::Bold);  // Increase font size
    painter.setFont(font);
    painter.drawText(rect, Qt::AlignCenter, QString::number(timeRemaining));
}
