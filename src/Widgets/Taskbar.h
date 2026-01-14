#pragma once

#include <QWidget>
#include <QVBoxLayout>
#include <QPropertyAnimation>
#include "TaskButton.h"

enum POSITION {
    TOP,
    BOTTOM
};

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
    void addTaskButton(TaskButton* btn, POSITION pos = TOP);
};