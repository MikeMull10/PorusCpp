#pragma once

#include <QWidget>
#include <QVBoxLayout>

class Taskbar : public QWidget {
    Q_OBJECT
    QVBoxLayout* layout;

public:
    explicit Taskbar(QWidget* parent = nullptr);
};