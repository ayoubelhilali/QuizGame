#include "hovereffect.h"

HoverEffect::HoverEffect(QPushButton *button) : QObject(button), button(button), progress(0), targetProgress(0)
{
    // Install event filter on the button
    button->installEventFilter(this);

    // Timer for smooth transition
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &HoverEffect::updateBorderColor);
}
