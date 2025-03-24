#include "circletimer.h"
#include <QPainter>
#include <QFont>

CircleTimer::CircleTimer(QWidget *parent) : QWidget(parent), timeRemaining(10) {
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
    } else {
        timer->stop();
    }
}

void CircleTimer::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // Set the size
    int size = qMin(width()+20, height()+20);
    QRect rect((width() - size) / 2, (height() - size) / 2, size, size);

    // Draw the background circle
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(220, 220, 220));  // Light gray background
    painter.drawEllipse(rect);

    // Draw the countdown arc
    painter.setBrush(Qt::NoBrush);
    QPen pen(QColor(50, 150, 250), 10);  // Blue progress arc
    painter.setPen(pen);

    int angle = (timeRemaining / 10.0) * 360 * 16;  // Convert to degrees (16x for Qt)
    painter.drawArc(rect, 90 * 16, -angle);  // Start from top

    // Draw the timer text
    painter.setPen(Qt::black);
    QFont font("Arial", size / 5, QFont::Bold);
    painter.setFont(font);
    painter.drawText(rect, Qt::AlignCenter, QString::number(timeRemaining));
}
