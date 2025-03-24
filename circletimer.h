#ifndef CIRCLETIMER_H
#define CIRCLETIMER_H

#include <QWidget>
#include <QTimer>

class CircleTimer : public QWidget {
    Q_OBJECT

public:
    explicit CircleTimer(QWidget *parent = nullptr);
    void startTimer();

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    int timeRemaining;  // Countdown from 10 to 0
    QTimer *timer;      // Timer to update every second

private slots:
    void updateTimer();
};

#endif // CIRCLETIMER_H
