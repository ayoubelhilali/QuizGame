#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QEvent>
#include <QPushButton>


QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    void DomainsChoose();
    void questionsPage(const QString &domain);
    ~MainWindow();

private slots:
        void on_startBtn_clicked();
        void on_settingsBtn_clicked();
        void onDomainButtonClicked();
private:
    Ui::MainWindow *ui;
protected:
    void paintEvent(QPaintEvent *event) override;

};


class HoverEffect : public QObject
{
    Q_OBJECT

public:
    HoverEffect(QPushButton *button) : QObject(button), button(button), progress(0), targetProgress(0)
    {
        // Install event filter on the button
        button->installEventFilter(this);

        // Timer for smooth transition
        timer = new QTimer(this);
        connect(timer, &QTimer::timeout, this, &HoverEffect::updateBorderColor);
    }

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

#endif // MAINWINDOW_H
