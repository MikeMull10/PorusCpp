#pragma once

#include <QLabel>
#include <QMouseEvent>

class TitleLabel : public QLabel {
    Q_OBJECT

private:
    bool clickable {false};
    bool active {true};

public:
    explicit TitleLabel(const QString& text, QWidget* parent = nullptr);
    explicit TitleLabel(const QString& text, bool clickable, QWidget* parent = nullptr);

    bool isActive() const;
    bool isClickable() const;

signals:
    void clicked();

protected:
    void mousePressEvent(QMouseEvent* event) override;
};