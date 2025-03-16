#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setFixedSize(800,500);
    w.setStyleSheet("QMainWindow::sizeGrip { width: 0px; height: 0px; }"); // Hide the size grip
    w.show();
    return a.exec();
}
