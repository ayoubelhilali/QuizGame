#ifndef HOVEREFFECT_H
#define HOVEREFFECT_H

#include <QMainWindow>
#include <QTimer>
#include <QEvent>
#include <QPushButton>

class HoverEffect : public QObject
{
    Q_OBJECT

public:
    HoverEffect(QPushButton *button);
protected:
    bool eventFilter(QObject *obj, QEvent *event) override
    {
        if (obj == button)
        {
            if (event->type() == QEvent::Enter)
            {
                targetProgress = 100; // Change to red
                timer->start(10); // Start animation
            }
            else if (event->type() == QEvent::Leave)
            {
                targetProgress = 0; // Change back to white
                timer->start(10); // Start animation
            }
        }
        return QObject::eventFilter(obj, event);
    }

private slots:
    void updateBorderColor()
    {
        // Smoothly transition between white and red
        if (progress < targetProgress)
            progress += 5;
        else if (progress > targetProgress)
            progress -= 5;
        else
        {
            timer->stop();
            return;
        }

        setButtonStyle(progress);
    }

private:
    QPushButton *button;
    QTimer *timer;
    int progress;         // Current color transition value (0 = white, 100 = red)
    int targetProgress;   // Target color value (100 = red, 0 = white)

    void setButtonStyle(int p)
    {
        int red = 255 - (p * 2.55);  // White → 0
        int green = 255 - ((255 - 150) * p / 100);  // White → 150
        int blue = 255 - ((255 - 255) * p / 100);   // White → 255

        button->setStyleSheet(QString("QPushButton { border: 2px solid rgb(%1, %2, %3); border-radius: 5px; }")
                                  .arg(red)
                                  .arg(green)
                                  .arg(blue));
    }
};

#endif // HOVEREFFECT_H
