#include "SettingsPopup.h"
#include "PushButton.h"
#include "SubtitleLabel.h"

#include <QGuiApplication>
#include <QScreen>
#include <QColorDialog>

SettingsPopup::SettingsPopup(QSettings* settings, QWidget* parent) : QDialog(parent) {
    this->setObjectName("SettingsPopup");
    this->setWindowTitle("Settings");

    QRect screen = QGuiApplication::primaryScreen()->availableGeometry();
    this->resize(screen.width() * 0.9, screen.height() * 0.9);

    QVBoxLayout* layout = new QVBoxLayout(this);

    QHBoxLayout* primaryLayout = new QHBoxLayout();
    SubtitleLabel* primaryLabel = new SubtitleLabel("Theme Accent");
    PushButton* primaryColorBtn = new PushButton(this);
    primaryLayout->addWidget(primaryLabel);
    primaryLayout->addWidget(primaryColorBtn);
    primaryLayout->addStretch();
    primaryColorBtn->setFixedSize(QSize(30, 30));
    primaryColorBtn->setStyleSheet(QString("QPushButton { background-color: %1; border: 1px solide #2d2d2d; border-radius: 5px; }").arg(settings->value("primary", "#ff0000").toString()));
    connect(primaryColorBtn, &PushButton::clicked, this, [this, primaryColorBtn, settings]() {
        QColor init(settings->value("primary", "#ff0000").toString());
        QColor color = QColorDialog::getColor(
            init,          // initial color
            this,          // parent
            "Select Primary Color"
        );

        if (color.isValid()) {
            settings->setValue("primary", color.name());
            primaryColorBtn->setStyleSheet(QString("QPushButton { background-color: %1; }").arg(color.name()));
            emit reloadQSS();
        }
    });

    layout->addLayout(primaryLayout);

    QHBoxLayout* outlineColorLayout = new QHBoxLayout();
    SubtitleLabel* outlineColorLabel = new SubtitleLabel("Outline Color");
    PushButton* outlineColorBtn = new PushButton(this);
    outlineColorLayout->addWidget(outlineColorLabel);
    outlineColorLayout->addWidget(outlineColorBtn);
    outlineColorLayout->addStretch();
    outlineColorBtn->setFixedSize(QSize(30, 30));
    outlineColorBtn->setStyleSheet(QString("QPushButton { background-color: %1; border: 1px solide #2d2d2d; border-radius: 5px; }").arg(settings->value("outlineColor", "#ff0000").toString()));
    connect(outlineColorBtn, &PushButton::clicked, this, [this, outlineColorBtn, settings]() {
        QColor init(settings->value("outlineColor", "#ff0000").toString());
        QColor color = QColorDialog::getColor(
            init,          // initial color
            this,          // parent
            "Select Outline Color"
        );

        if (color.isValid()) {
            settings->setValue("outlineColor", color.name());
            outlineColorBtn->setStyleSheet(QString("QPushButton { background-color: %1; }").arg(color.name()));
            emit reloadQSS();
        }
    });

    layout->addLayout(outlineColorLayout);
}