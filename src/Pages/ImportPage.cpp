#include "ImportPage.h"

ImportPage::ImportPage(QWidget* parent) : QWidget(parent) {
    setObjectName("ImagePage");
    this->mainLayout = new QHBoxLayout(this);
    this->imageViewer = new ImageViewer(this);
    this->mainLayout->addWidget(this->imageViewer);
}

void ImportPage::setup() {
    if (!this->baseImage || crop.isEmpty()) return;

    QRectF pixmapBounds = baseImage->boundingRect();
    QRectF intersection = crop.intersected(pixmapBounds);
    
    if (intersection.isEmpty()) return; // No overlap
    
    // Extract the cropped region from the original pixmap
    QPixmap originalPixmap = baseImage->pixmap();
    QPixmap croppedPixmap = originalPixmap.copy(intersection.toRect());
    
    this->imageViewer->loadImage(croppedPixmap);
}

void ImportPage::loadImage(const QString& path) { this->imageViewer->loadImage(path); }

void ImportPage::setBaseImage(QGraphicsPixmapItem* image) { this->baseImage = image; }
QGraphicsPixmapItem* ImportPage::getBaseImage() const { return this->baseImage; }

void ImportPage::setCrop(QRectF crop) { this->crop = crop; }
QRectF ImportPage::getCrop() const { return this->crop; }