#pragma once

#include <QWidget>
#include <QHBoxLayout>
#include "ToolButton.h"
#include "SubtitleLabel.h"

class TaskButton : public QWidget {
    Q_OBJECT
    QIcon icon;
    QString text;

    QHBoxLayout* layout;
    ToolButton* btn;
    SubtitleLabel* label;

public:
    explicit TaskButton(const QIcon& icon, const QString& text, QWidget *parent = nullptr);
};