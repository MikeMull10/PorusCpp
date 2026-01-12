#include "ToolButton.h"

ToolButton::ToolButton(QWidget *parent) : QPushButton(parent) {
    setObjectName("ToolButton");
    setCursor(Qt::PointingHandCursor);
}

ToolButton::ToolButton(const QIcon& icon, QWidget *parent) : ToolButton(parent) {
    setIcon(icon);
    setIconSize(QSize(16, 16));
    setFixedSize(QSize(36, 36));
}
