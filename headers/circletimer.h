#ifndef CIRCLETIMER_H
#define CIRCLETIMER_H

#include <QWidget>
#include <QTimer>

class CircleTimer : public QWidget {
    Q_OBJECT

public:
    explicit CircleTimer(QWidget *parent = nullptr);
    void startTimer();
    void stopTimer();
    int gettime(){return timeRemaining;};
    void setProgressColor(QColor );
    QColor getProgressColor();
    void setTimeRemaining(int start){
        timeRemaining=start;
        updateTimer();
        startTimer();
    }
protected:
    void paintEvent(QPaintEvent *event) override;

signals:  // Add this section
    void timeout();  // Signal emitted when timer reaches zero

private:
    int timeRemaining;  // Countdown from 10 to 0
    QTimer *timer;
    QColor color;
    QString correctAnswer;  // Store the correct answer here

private slots:
    void updateTimer();
    void onTimerFinished();

};

#endif // CIRCLETIMER_H
