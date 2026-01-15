#include "ImagePopup.h"
#include <QGuiApplication>
#include <QLabel>
#include <QPushButton>

ImagePopup::ImagePopup(const QString& path, QWidget* parent) : QDialog(parent) {
    this->setObjectName("ImagePopup");
    this->setWindowTitle("Image Preview");

    QRect screen = QGuiApplication::primaryScreen()->availableGeometry();
    this->resize(screen.width() * 0.9, screen.height() * 0.9);

    QVBoxLayout* layout = new QVBoxLayout(this);

    this->toolbar = new ImageToolbar({ TOOL::HAND, TOOL::ZOOM, TOOL::CROP, TOOL::SCALE }, this);
    layout->addWidget(this->toolbar);
    
    this->imageViewer = new ImageViewer(this->toolbar, this);
    layout->addWidget(this->imageViewer);
    this->imageViewer->loadImage(path);
    
    QPushButton* closeBtn = new QPushButton("Done", this);
    closeBtn->setCursor(Qt::PointingHandCursor);
    connect(closeBtn, &QPushButton::clicked, this, &QDialog::accept);
    layout->addWidget(closeBtn);
}