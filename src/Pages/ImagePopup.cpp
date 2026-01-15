#include "ImagePopup.h"
#include <QLabel>
#include <QPushButton>

ImagePopup::ImagePopup(const QString& path, QWidget* parent) : QDialog(parent) {
    this->setObjectName("ImagePopup");
    this->setWindowTitle("Image Preview");

    QRect screen = QGuiApplication::primaryScreen()->availableGeometry();
    this->resize(screen.width() * 0.9, screen.height() * 0.8);

    QVBoxLayout* layout = new QVBoxLayout(this);

    this->toolbar = new ImageToolbar({ TOOL::HAND, TOOL::ZOOM, TOOL::CROP, TOOL::SCALE }, this);
    layout->addWidget(this->toolbar);
    
    this->imageViewer = new ImageViewer(this->toolbar, this);
    this->imageViewer->loadImage(path);
    layout->addWidget(this->imageViewer);
    
    QPushButton* closeBtn = new QPushButton("Done", this);
    closeBtn->setCursor(Qt::PointingHandCursor);
    connect(closeBtn, &QPushButton::clicked, this, &QDialog::accept);
    layout->addWidget(closeBtn);
}