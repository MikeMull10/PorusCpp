#pragma once

#include <QWidget>
#include <QHBoxLayout>
#include <QMouseEvent>
#include <QLabel>
#include "ToolButton.h"
#include "SubtitleLabel.h"

class TaskButton : public QWidget {
    Q_OBJECT
    QIcon icon;
    QString text;

    QHBoxLayout* layout;
    SubtitleLabel* label;

public:
    explicit TaskButton(const QIcon& icon, const QString& text, QWidget *parent = nullptr);

    void setVisibility(bool extended);

signals:
    void clicked();

protected:
    void mousePressEvent(QMouseEvent* event) override;
};