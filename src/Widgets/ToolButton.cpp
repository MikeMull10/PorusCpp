#include "ToolButton.h"
#include <QStyle>

ToolButton::ToolButton(QWidget *parent) : QToolButton(parent) {
    setObjectName("ToolButton");
    setCursor(Qt::PointingHandCursor);
    setProperty("active", false);
}

ToolButton::ToolButton(const QIcon& icon, QWidget *parent) : ToolButton(parent) {
    setIcon(icon);
    setIconSize(QSize(16, 16));
    setFixedSize(QSize(36, 36));
}

void ToolButton::paintEvent(QPaintEvent* e)
{
    QStyleOptionToolButton opt;
    initStyleOption(&opt);

    opt.iconSize = QSize(16, 16);
    opt.features = QStyleOptionToolButton::None;

    QPainter p(this);
    style()->drawComplexControl(QStyle::CC_ToolButton, &opt, &p, this);
}

void ToolButton::setActive(bool active) { this->setProperty("active", active); style()->unpolish(this); style()->polish(this); }
