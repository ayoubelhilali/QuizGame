#ifndef TYPINGEFFECT_H
#define TYPINGEFFECT_H

#include <QTimer>
#include <QLabel>

class TypingAnimation : public QObject {
    Q_OBJECT

public:
    TypingAnimation(QLabel *label, int speed = 100, QObject *parent = nullptr)
        : QObject(parent), label(label), typingSpeed(speed), currentIndex(0) {

        originalText = label->text(); // Store the original text
        label->setText(""); // Clear the label initially

        typingTimer = new QTimer(this);
        connect(typingTimer, &QTimer::timeout, this, &TypingAnimation::updateText);
    }

    void start() {
        currentIndex = 0;
        label->setText(""); // Reset the text
        typingTimer->start(typingSpeed);
    }

private slots:
    void updateText() {
        if (currentIndex < originalText.length()) {
            label->setText(originalText.left(currentIndex + 1));
            currentIndex++;
        } else {
            currentIndex = 0;  // Reset index
            label->setText(""); // Clear text for the next loop
        }
    }

private:
    QLabel *label;
    QString originalText;
    int typingSpeed;
    int currentIndex;
    QTimer *typingTimer;
};


#endif // TYPINGEFFECT_H
