#include "circletimer.h"
#include <QPainter>
#include <QFont>

CircleTimer::CircleTimer(QWidget *parent) : QWidget(parent), timeRemaining(10), color(QColor(0, 255, 0)) {
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &CircleTimer::updateTimer);
}

void CircleTimer::startTimer() {
    timer->start(1000);  // Start timer with 1-second intervals
}

void CircleTimer::updateTimer() {
    if (timeRemaining > 0) {
        timeRemaining--;
        update();  // Refresh UI
        if(this->gettime()<=3){
            setProgressColor(QColor(255,0,0));
        }
    } else {
        timer->stop();
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
