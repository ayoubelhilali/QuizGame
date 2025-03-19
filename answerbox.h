#ifndef ANSWERBOX_H
#define ANSWERBOX_H

#include <QWidget>
#include <QLabel>
#include <QPixmap>

class AnswerBox : public QWidget {
    Q_OBJECT
public:
    explicit AnswerBox(QString text, QString letter, QWidget *parent = nullptr);

private:
    QLabel *backgroundLbl;
    QLabel *textLbl;
    QLabel *letterLbl;
};

#endif // ANSWERBOX_H
