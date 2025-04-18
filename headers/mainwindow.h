#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QResizeEvent>
#include <QEvent>
#include <QPushButton>
#include <QProgressBar>
#include <QLabel>
#include <memory>
#include <QHBoxLayout>
#include <QSlider>

#include "answerbox.h"
#include "circletimer.h"
#include "geminiai.h"
#include "typingeffect.h"
#include "settingsdialog.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void DomainsChoose();
    void questionsPage(const QString &domain);
    void clearWidgets();
    void applyShadowEffect(QWidget *widget, int blurRadius, const QColor &color);
    void deleteButtons(const std::initializer_list<QPushButton *> &buttons);
    void replaceCentralWidgetLayout(QLayout *newLayout);
    void startProgressBar(QProgressBar *progressBar, QLabel *waitLabel,TypingAnimation *typing);
    void setupButton(QPushButton *button, const QString &iconName);
    void adjustDomainLayout(QHBoxLayout *firstRowLayout, QHBoxLayout *secondRowLayout, QPushButton *buttons[]);
    void onAnswerBoxClicked(AnswerBox *box);
    void onBackButtonClicked();
    void onPauseClicked();
    void onResumeclicked();
    void on_restartBtn_clicked();
    QString generateQuestion(QString domain);
    QProgressBar *createProgressBar();
    QLabel *createLabel(const QString &text, int fontSize, Qt::Alignment alignment);
    QPushButton *createDomainButton(const QString &iconName);
    template <typename T>
    void clearWidgets();
    void pausewindow();

private slots:
    void on_startBtn_clicked();
    void on_settingsBtn_clicked();
    void on_infoBtn_clicked();
    void on_statsBtn_clicked();
    void onDomainButtonClicked();
    void updateVolume(int value) ;

protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    std::unique_ptr<Ui::MainWindow> ui;
    QPushButton *pauseBtn = nullptr;
    QPushButton *exitBtn=nullptr;
    QPushButton *infoBtn=nullptr;
    QPushButton *backbutton=nullptr;
    CircleTimer *timer=nullptr;
    bool ispaused;
    QWidget *pauseOverlay=nullptr;
    QWidget *pausewidget=nullptr;
    QWidget *pausecontainer=nullptr;
    QVBoxLayout *pauselayout=nullptr;
    QLabel *volumetxt=nullptr;
    QSlider *volumeSlider=nullptr;
    QString domain;
    GeminiAI *geminiAI;
    SettingsDialog *settingsDialog;
    int highScore=0, totalGamesPlayed=0,  gamesWon=0,
         averageScore=0,  longestStreak=0,  fastestTime=-1,
         totalCorrectAnswers=0,  totalIncorrectAnswers=0;
};
#endif // MAINWINDOW_H
