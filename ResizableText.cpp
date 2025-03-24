#include "resizabletext.h"
#include <QResizeEvent>
#include <QFont>

ResizableText::ResizableText(QLabel *existingLabel, QWidget *parent)
    : QWidget(parent), label(existingLabel) {
    updateFontSize(); // Set initial font size
}

ResizableText::~ResizableText() {
    // Destructor (if needed)
}

void ResizableText::resizeEvent(QResizeEvent *event) {
    updateFontSize();
    QWidget::resizeEvent(event);
}

void ResizableText::updateFontSize() {
    if (!label) return; // Safety check

    int height = this->height(); // Get window height
    QFont font = label->font();

    // Scale font based on window height
    int newSize = height / 10; // Adjust as needed
    font.setPixelSize(newSize);

    label->setFont(font);
}
