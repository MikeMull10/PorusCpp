#include "ToolButton.h"

ToolButton::ToolButton(QWidget *parent) : QPushButton(parent) {
    setCursor(Qt::PointingHandCursor);
}

ToolButton::ToolButton(const QIcon& icon, QWidget *parent) : QPushButton(parent) {
    setCursor(Qt::PointingHandCursor);
    setIcon(icon);
    setIconSize(QSize(35, 35));
    setFixedSize(QSize(40, 40));
}
