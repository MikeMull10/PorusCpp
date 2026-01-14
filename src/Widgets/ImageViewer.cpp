#include "ImageViewer.h"
#include <QGraphicsPixmapItem>
#include <QVBoxLayout>

ImageViewer::ImageViewer(QWidget* parent) : QWidget(parent) {
    setObjectName("ImageViewer");

    this->scene = new QGraphicsScene(this);
    this->view = new GraphicsView(this->scene, this);
    
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(view);
    setLayout(layout);

    this->view->setDragMode(QGraphicsView::ScrollHandDrag);
    this->view->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
}

void ImageViewer::loadImage(const QString& path) {
    if (this->currentImage) {
        this->scene->removeItem(this->currentImage);
        delete this->currentImage;
    }

    QPixmap img(path);
    currentImage = this->scene->addPixmap(img);
    currentImage->setZValue(0);
}