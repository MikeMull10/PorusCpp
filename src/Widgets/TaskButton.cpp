#include "TaskButton.h"
#include <QStyle>

TaskButton::TaskButton(const QIcon& icon, const QString& text, QWidget* parent) : QPushButton(parent) {
    this->setObjectName("TaskButton");
    this->setProperty("class", "TaskButton");
    this->setCursor(Qt::PointingHandCursor);
    this->setAttribute(Qt::WA_Hover, true);

    this->icon = icon;
    this->text = text;

    setIcon(this->icon);
    setIconSize(QSize(24, 24));
    setText(this->text);

    this->extended = !text.isEmpty();
}

void TaskButton::setVisibility(bool extended) {
    if (this->text.isEmpty()) return;
    
    this->extended = extended;
    
    if (this->extended) {
        setText(this->text);
    } else {
        setText("");
    }
}
void TaskButton::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) emit clicked();
    QWidget::mousePressEvent(event);
}