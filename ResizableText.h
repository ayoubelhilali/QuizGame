#ifndef RESIZABLETEXT_H
#define RESIZABLETEXT_H

#include <QWidget>
#include <QLabel>

class ResizableText : public QWidget {
    Q_OBJECT  // Required for signals and slots in Qt

public:
    explicit ResizableText(QLabel *existingLabel, QWidget *parent = nullptr);
    virtual ~ResizableText();  // Virtual destructor

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    QLabel *label;
    void updateFontSize();
};

#endif // RESIZABLETEXT_H
