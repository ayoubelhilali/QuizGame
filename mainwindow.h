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
#include "answerbox.h"

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
    void startProgressBar(QProgressBar *progressBar, QLabel *waitLabel);
    void setupButton(QPushButton *button, const QString &iconName);
    void adjustDomainLayout(QHBoxLayout *firstRowLayout, QHBoxLayout *secondRowLayout, QPushButton *buttons[]);
    void onAnswerBoxClicked(AnswerBox *box);
    void onBackButtonClicked();
    QProgressBar *createProgressBar();
    QLabel *createLabel(const QString &text, int fontSize, Qt::Alignment alignment);
    QPushButton *createDomainButton(const QString &iconName);
    template <typename T>
    void clearWidgets();

private slots:
    void on_startBtn_clicked();
    void on_settingsBtn_clicked();
    void onDomainButtonClicked();

protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    std::unique_ptr<Ui::MainWindow> ui;
};

#endif // MAINWINDOW_H
