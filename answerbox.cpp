#include "answerbox.h"

AnswerBox::AnswerBox(QString text, QString letter,int c=-1, QWidget *parent)
    : QWidget(parent) {

    // Background Label
    backgroundLbl = new QLabel(this);
    QPixmap pixmap(":/images/answer.svg");
    QPixmap scaledPixmap = pixmap.scaled(4*this->width(), 4*this->height(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    backgroundLbl->setPixmap(scaledPixmap);
    backgroundLbl->setFixedSize(scaledPixmap.size());

    // Text Label
    textLbl = new QLabel(text, this);
    textLbl->setStyleSheet("font-size:35px;"
                           "border-radius:12px;"
                           "text-align:center;");

    // Letter Label
    letterLbl = new QLabel(letter, this);
    letterLbl->setStyleSheet("font-size:15px;"
                             "font-weight:bold;"
                             "border-radius:16px;"
                             "text-align:center;");
    letterLbl->setAlignment(Qt::AlignCenter);
    letterLbl->setGeometry(13, 12, 38, 38); // Adjust position relative to the parent
    if(c==1){
        textLbl->setStyleSheet("background-color:rgba(0, 255, 0, 0.5);"
                               "border-radius:12px;");
        letterLbl->setStyleSheet("background-color:green;"
                                 "border-radius:19px;");
    }else if(c==0){
        textLbl->setStyleSheet("background-color:rgba(255, 0, 0, 0.5);"
                               "border-radius:12px;");
        letterLbl->setStyleSheet("background-color:red;"
                                 "border-radius:19px;");
    }else{
        textLbl->setStyleSheet("background-color:rgba(0, 0, 255, 0.1);"
                               "border-radius:12px;");
        letterLbl->setStyleSheet("background-color:blue;"
                                 "border-radius:19px;");
    }
    textLbl->setAlignment(Qt::AlignCenter);
    textLbl->setGeometry(32, 30, 204, 62);  // Adjust position relative to the parent
    // Set the widget size based on background label
    setFixedSize(backgroundLbl->size());
}
