#pragma once

#include <QToolButton>
#include <QPainter>
#include <QPaintEvent>
#include <QStyle>
#include <QStyleOptionToolButton>

class ToolButton : public QToolButton {
    Q_OBJECT

public:
    explicit ToolButton(QWidget *parent = nullptr);
    explicit ToolButton(const QIcon& icon, QWidget *parent = nullptr);

    void setActive(bool active);
    void flipActive();

    void setScale(float scale);

protected:
    void paintEvent(QPaintEvent* e) override;
};  