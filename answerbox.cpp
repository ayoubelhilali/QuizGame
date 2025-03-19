#include "answerbox.h"

AnswerBox::AnswerBox(QString text, QString letter, QWidget *parent)
    : QWidget(parent) {

    // Background Label
    backgroundLbl = new QLabel(this);
    QPixmap pixmap(":/images/answer.svg");
    QPixmap scaledPixmap = pixmap.scaled(300, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    backgroundLbl->setPixmap(scaledPixmap);
    backgroundLbl->setFixedSize(scaledPixmap.size());

    // Text Label
    textLbl = new QLabel(text, this);
    textLbl->setStyleSheet("background-color:rgba(0, 255, 0, 0.5);"
                           "border-radius:10px;"
                           "text-align:center;");
    textLbl->setAlignment(Qt::AlignCenter);
    textLbl->setGeometry(27, 25, 172, 50);  // Adjust position relative to the parent

    // Letter Label
    letterLbl = new QLabel(letter, this);
    letterLbl->setStyleSheet("background-color:green;"
                             "border-radius:16px;"
                             "text-align:center;");
    letterLbl->setAlignment(Qt::AlignCenter);
    letterLbl->setGeometry(10, 10, 32, 32); // Adjust position relative to the parent

    // Set the widget size based on background label
    setFixedSize(backgroundLbl->size());
}
