#pragma once

#include <QWidget>
#include <QVBoxLayout>
#include <QPropertyAnimation>
#include "TaskButton.h"

class Taskbar : public QWidget {
    Q_OBJECT
    QVBoxLayout* layout;
    QVBoxLayout* top;
    QVBoxLayout* bottom;

    TaskButton* menuBtn;
    bool extended{ true };

    QPropertyAnimation* widthAnimation = nullptr;

public:
    explicit Taskbar(QWidget* parent = nullptr);

    void updateWidth();
};