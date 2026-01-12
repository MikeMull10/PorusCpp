#include "Taskbar.h"
#include "ToolButton.h"

Taskbar::Taskbar(QWidget* parent) : QWidget(parent) {
    this->setObjectName("Taskbar");
    this->setProperty("class", "Taskbar");

    this->layout = new QVBoxLayout(this);
    this->layout->setContentsMargins(0, 0, 0, 0);

    QWidget *topWrapper = new QWidget(this);
    QWidget *bottomWrapper = new QWidget(this);
    this->top = new QVBoxLayout(topWrapper);
    this->bottom = new QVBoxLayout(bottomWrapper);
    this->top->setContentsMargins(0, 0, 0, 0);
    this->bottom->setContentsMargins(0, 0, 0, 0);

    this->layout->addWidget(topWrapper);
    this->layout->addStretch();
    this->layout->addWidget(bottomWrapper);

    this->menuBtn = new TaskButton(QIcon(":/icons/menu-white.svg"), "", this);
    this->top->addWidget(menuBtn);
    connect(menuBtn, &TaskButton::clicked, this, [this]() {
        this->extended = !this->extended;
        this->updateWidth();
    });

    TaskButton *settings = new TaskButton(QIcon(":/icons/setting-white.svg"), "Settings", this);
    this->bottom->addWidget(settings);

    widthAnimation = new QPropertyAnimation(this, "maximumWidth");
    widthAnimation->setDuration(200);
    widthAnimation->setEasingCurve(QEasingCurve::InOutQuad);

    this->updateWidth();
}

void Taskbar::updateWidth() {
    int targetWidth = this->extended ? 250 : 40;  // 40 = icon size (36) + 2 * margin size (2)
    widthAnimation->stop();
    widthAnimation->setStartValue(this->width());
    widthAnimation->setEndValue(targetWidth);
    disconnect(widthAnimation, &QPropertyAnimation::finished, this, nullptr);
    connect(widthAnimation, &QPropertyAnimation::finished, this, [this, targetWidth]() {
        this->setFixedWidth(targetWidth);
    });
    widthAnimation->start();

    for (int a = 0; a < 2; a++) {
        QVBoxLayout *lay = (a == 0) ? this->top : this->bottom;

        for (int i = 0; i < lay->count(); i++) {
            QWidget* widget = lay->itemAt(i)->widget();
            if (!widget) continue;

            TaskButton* bar = qobject_cast<TaskButton*>(widget);
            if (bar) bar->setVisibility(this->extended);
        }
    }
}