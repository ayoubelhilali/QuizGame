#ifndef ANSWERBOX_H
#define ANSWERBOX_H

#include <QWidget>
#include <QLabel>

class AnswerBox : public QWidget
{
    Q_OBJECT
    QString L;

public:
    explicit AnswerBox(QString t, QString l, int c, QWidget *parent = nullptr);
    bool checkcorrect(QString);
    QString getText() const { return text; }
    QLabel *getTextlabel() const{return textLbl;}
    void setclicked(int c){Answerclicked=c;}
    int getclicked(){return Answerclicked;}
    QString getLetter(){return L;}
    void showresult(int corr);

signals:
    void clicked(AnswerBox *box);

protected:
    void mousePressEvent(QMouseEvent *event) override;

private:
    QString text;
    QString letter;
    int correct=0;
    int Answerclicked;
    QLabel *backgroundLbl;
    QLabel *textLbl;
    QLabel *letterLbl;
};

#endif // ANSWERBOX_H
