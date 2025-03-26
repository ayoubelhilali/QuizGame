#ifndef ANSWERBOX_H
#define ANSWERBOX_H

#include <QWidget>
#include <QLabel>

class AnswerBox : public QWidget
{
    Q_OBJECT

public:
    explicit AnswerBox(QString t, QString l, int c, QWidget *parent = nullptr);
    bool checkcorrecct(AnswerBox *A);
    QString getText() const { return text; }
    QLabel *getTextlabel() const{return textLbl;}

signals:
    void clicked(AnswerBox *box);

protected:
    void mousePressEvent(QMouseEvent *event) override;

private:
    QString text;
    QString letter;
    int correct;
    QLabel *backgroundLbl;
    QLabel *textLbl;
    QLabel *letterLbl;
};

#endif // ANSWERBOX_H
