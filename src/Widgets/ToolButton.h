#pragma once

#include <QPushButton>

class ToolButton : public QPushButton {
    Q_OBJECT

public:
    explicit ToolButton(QWidget *parent = nullptr);
    explicit ToolButton(const QIcon& icon, QWidget *parent = nullptr);
};  