#include "TaskButton.h"
#include <QStyle>

TaskButton::TaskButton(const QIcon& icon, const QString& text, QWidget* parent) : QWidget(parent) {
    this->setProperty("class", "TaskButton");
    this->setCursor(Qt::PointingHandCursor);

    this->icon = icon;
    this->text = text;

    this->layout = new QHBoxLayout(this);
    this->layout->setContentsMargins(2, 2, 2, 2);

    this->label = new SubtitleLabel(this->text, this);
    this->label->setObjectName("TaskButtonLabel");

    QLabel* iconLabel = new QLabel(this);
    iconLabel->setPixmap(icon.pixmap(16, 16));
    iconLabel->setFixedSize(36, 36);
    iconLabel->setAlignment(Qt::AlignCenter);
    this->layout->addWidget(iconLabel);

    if (text.length() > 0) this->layout->addWidget(this->label);
    else this->label->hide();
    this->layout->addStretch();
}

void TaskButton::setVisibility(bool extended) {
    if (this->text.length() == 0) return;
    extended ? this->label->show() : this->label->hide();
}

void TaskButton::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) emit clicked();
    QWidget::mousePressEvent(event);
}