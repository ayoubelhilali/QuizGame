#include "headers/answerbox.h"
#include <QMouseEvent>

AnswerBox::AnswerBox(QString t, QString l, int c, QWidget *parent)
    : QWidget(parent), text(t), letter(l), correct(c)
{
    // Background Label
    backgroundLbl = new QLabel(this);
    QPixmap pixmap(":/Icons/answer.svg");
    QPixmap scaledPixmap = pixmap.scaled(4 * this->width(), 4 * this->height(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    backgroundLbl->setPixmap(scaledPixmap);
    backgroundLbl->setFixedSize(scaledPixmap.size());

    // Text Label
    textLbl = new QLabel(text, this);
    textLbl->setStyleSheet("font-size:35px; border-radius:12px; text-align:center;");
    textLbl->setAlignment(Qt::AlignCenter);
    textLbl->setGeometry(32, 30, 204, 62); // Adjust position relative to the parent

    // Letter Label
    letterLbl = new QLabel(letter, this);
    letterLbl->setStyleSheet("font-size:15px; font-weight:bold; border-radius:16px; text-align:center;");
    letterLbl->setAlignment(Qt::AlignCenter);
    letterLbl->setGeometry(13, 12, 38, 38); // Adjust position relative to the parent

    textLbl->setStyleSheet("background-color:rgba(0, 0, 255, 0.1); border-radius:12px;");
    letterLbl->setStyleSheet("background-color:blue; border-radius:19px;");
    textLbl->setAlignment(Qt::AlignCenter);
    textLbl->setGeometry(32, 30, 204, 62); // Adjust position relative to the parent

    // Set the widget size based on background label
    setFixedSize(backgroundLbl->size());
    setCursor(Qt::PointingHandCursor);
}

void AnswerBox::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    emit clicked(this);
}

bool AnswerBox::checkcorrecct(AnswerBox *A)
{
    if (true)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
