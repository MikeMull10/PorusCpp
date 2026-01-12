#pragma once

#include <QWidget>
#include <QVBoxLayout>
#include "TaskButton.h"

class Taskbar : public QWidget {
    Q_OBJECT
    QVBoxLayout* layout;
    QVBoxLayout* top;
    QVBoxLayout* bottom;

    TaskButton* menuBtn;
    bool extended{ true };

public:
    explicit Taskbar(QWidget* parent = nullptr);

    void updateWidth();
};