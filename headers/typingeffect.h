#ifndef TYPINGEFFECT_H
#define TYPINGEFFECT_H

#include <QTimer>
#include <QLabel>

class TypingAnimation : public QObject {
    Q_OBJECT

public:
    TypingAnimation(QLabel *label, const QString& baseText = "Please wait", int speed = 500, QObject *parent = nullptr)
        : QObject(parent), label(label), baseText(baseText), typingSpeed(speed), dotCount(0) {

        typingTimer = new QTimer(this);
        connect(typingTimer, &QTimer::timeout, this, &TypingAnimation::updateText);
    }

    void start() {
        dotCount = 0;
        typingTimer->start(typingSpeed);
    }

    void stop() {
        typingTimer->stop();
        label->setText(baseText); // Reset to base text
    }

private slots:
    void updateText() {
        dotCount = (dotCount + 1) % 4; // 0 to 3 dots
        QString dots(dotCount, '.');
        label->setText(baseText + dots);
    }

private:
    QLabel *label;
    QString baseText;
    int typingSpeed;
    int dotCount;
    QTimer *typingTimer;
};

#endif // TYPINGEFFECT_H
