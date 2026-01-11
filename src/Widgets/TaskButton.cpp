#include "TaskButton.h"

TaskButton::TaskButton(const QIcon& icon, const QString& text, QWidget* parent) : QWidget(parent) {
    this->icon = icon;
    this->text = text;

    this->layout = new QHBoxLayout(this);
    this->btn = new ToolButton(this->icon, this);
    this->label = new SubtitleLabel(this->text, this);
}