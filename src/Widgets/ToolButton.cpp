#include "ToolButton.h"
#include <QStyle>

ToolButton::ToolButton(QWidget *parent) : QToolButton(parent) {
    this->setObjectName("ToolButton");
    this->setCursor(Qt::PointingHandCursor);
    this->setProperty("active", false);
}

ToolButton::ToolButton(const QIcon& icon, QWidget *parent) : ToolButton(parent) {
    this->setIcon(icon);
    this->setIconSize(QSize(16, 16));
    this->setFixedSize(QSize(36, 36));
}

void ToolButton::paintEvent(QPaintEvent* e)
{
    QStyleOptionToolButton opt;
    initStyleOption(&opt);
    
    opt.iconSize = this->iconSize();
    opt.features = QStyleOptionToolButton::None;

    QPainter p(this);
    style()->drawComplexControl(QStyle::CC_ToolButton, &opt, &p, this);
}

void ToolButton::setActive(bool active) { this->setProperty("active", active); style()->unpolish(this); style()->polish(this); }
void ToolButton::flipActive() { this->setActive( !(this->property("active") == "true") ); }

void ToolButton::setScale(float scale) {
    this->setIconSize(QSize((int) 16 * scale, (int) 16 * scale));
    this->setFixedSize(QSize((int) 16 * scale + 20, (int) 16 * scale + 20));
}