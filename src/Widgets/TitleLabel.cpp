#include "TitleLabel.h"
#include <QStyle>

TitleLabel::TitleLabel(const QString& text, bool clickable, QWidget* parent) : QLabel(text, parent) {
    setObjectName("TitleLabel");
    setProperty("active", this->active);

    QFont currentFont = this->font();
    currentFont.setPointSize(24);
    currentFont.setBold(true);
    this->setFont(currentFont);

    this->clickable = clickable;

    if (this->clickable) {
        this->setCursor(Qt::PointingHandCursor);
    }
}

TitleLabel::TitleLabel(const QString& text, QWidget* parent) : TitleLabel(text, false, parent) {}

bool TitleLabel::isActive() const { return this->active; }
bool TitleLabel::isClickable() const { return this->clickable; }

void TitleLabel::mousePressEvent(QMouseEvent* event) {
    if (this->clickable && event->button() == Qt::LeftButton) {
        emit clicked();
        this->active = !this->active;

        // Visual Properties/Styling
        setProperty("active", this->active);
        style()->unpolish(this);
        style()->polish(this);
    }
    QLabel::mousePressEvent(event);
}