#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QResizeEvent>
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
    void resizeEvent(QResizeEvent *event) override;

};
#endif
