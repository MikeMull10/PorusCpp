#include "ImagePopup.h"
#include <QLabel>
#include <QPushButton>

ImagePopup::ImagePopup(const QString& path, QWidget* parent) : QDialog(parent) {
    this->setObjectName("ImagePopup");
    this->setWindowTitle("Image Preview");
    this->resize(1200, 800);

    QVBoxLayout* layout = new QVBoxLayout(this);

    QLabel* messageLabel = new QLabel("Image loaded successfully!", this);
    layout->addWidget(messageLabel);
    
    this->imageViewer = new ImageViewer(this);
    this->imageViewer->loadImage(path);
    layout->addWidget(this->imageViewer);
    
    QPushButton* closeBtn = new QPushButton("Done", this);
    closeBtn->setCursor(Qt::PointingHandCursor);
    connect(closeBtn, &QPushButton::clicked, this, &QDialog::accept);
    layout->addWidget(closeBtn);
}