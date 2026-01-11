#include "Taskbar.h"
#include "ToolButton.h"

Taskbar::Taskbar(QWidget* parent) : QWidget(parent) {
    this->layout = new QVBoxLayout(this);
}