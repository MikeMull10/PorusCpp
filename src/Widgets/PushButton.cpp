#include "PushButton.h"

PushButton::PushButton(QWidget *parent) : QPushButton(parent) {
    setCursor(Qt::PointingHandCursor);
}

PushButton::PushButton(const QString& text, QWidget *parent) : QPushButton(text, parent) {
    setCursor(Qt::PointingHandCursor);
}
