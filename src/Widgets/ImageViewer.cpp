#include "ImageViewer.h"
#include "ImageToolbar.h"
#include <QGraphicsPixmapItem>
#include <QVBoxLayout>

ImageViewer::ImageViewer(QWidget* parent) : QWidget(parent) {
    setObjectName("ImageViewer");

    this->scene = new QGraphicsScene(this);
    this->view = new GraphicsView(this->scene, this);
    
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(view);
    setLayout(layout);

    this->view->setDragMode(QGraphicsView::NoDrag);
    this->view->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    
    this->view->viewport()->installEventFilter(this);
}

ImageViewer::ImageViewer(ImageToolbar* toolbar, QWidget* parent) : ImageViewer(parent) {
    this->toolbar = toolbar;
    connect(this->toolbar, &ImageToolbar::toolActivated, this, &ImageViewer::onToolSwitch);
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

void ImageViewer::setToolbar(ImageToolbar* toolbar) {
    this->toolbar = toolbar;
    connect(this->toolbar, &ImageToolbar::toolActivated, this, &ImageViewer::onToolSwitch);
}

void ImageViewer::onToolSwitch(TOOL tool) {
    qDebug() << "SWAPPED";
    this->currentTool = tool;

    if (this->currentTool == TOOL::HAND) this->view->setDragMode(QGraphicsView::ScrollHandDrag);
    else this->view->setDragMode(QGraphicsView::NoDrag);
}

bool ImageViewer::eventFilter(QObject* obj, QEvent* event) {
    if (!this->toolbar || this->currentTool == TOOL::NONE) return QWidget::eventFilter(obj, event);

    // Handle Cropping
    if (this->currentTool == TOOL::CROP) {
        if(event->type() == QEvent::MouseButtonPress) {
            QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
            if (mouseEvent->button() == Qt::LeftButton) {
                this->cropStart = this->view->mapToScene(mouseEvent->pos());
                this->startCrop();
                return true;
            }
        }
        else if (event->type() == QEvent::MouseMove) {
            QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
            if (this->isDrawing) {
                QPointF currentPos = this->view->mapToScene(mouseEvent->pos());
                this->updateCrop(currentPos);
                return true;
            }
        }
        else if (event->type() == QEvent::MouseButtonRelease) {
            QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
            if (mouseEvent->button() == Qt::LeftButton && this->isDrawing) {
                this->finishCrop(this->view->mapToScene(mouseEvent->pos()));
                return true;
            }
        }
    }

    return QWidget::eventFilter(obj, event);
}

void ImageViewer::startCrop() {
    this->isDrawing = true;
    currentRect = this->scene->addRect(QRectF(this->cropStart, this->cropStart), QPen(Qt::red, 2), QBrush(QColor(255, 0, 0, 50)));
    currentRect->setZValue(100);
}

void ImageViewer::updateCrop(const QPointF& pos) {
    if (currentRect) {
        QRectF rect = QRectF(this->cropStart, pos).normalized();
        currentRect->setRect(rect);
    }
}

void ImageViewer::finishCrop(const QPointF& pos) {
    this->isDrawing = false;
    delete currentRect;
    currentRect = nullptr;

    QRectF imageSize = this->currentImage->sceneBoundingRect();
    QPointF startPos = this->cropStart;
    QPointF endPos = pos;

    QRectF crop = QRectF(startPos, endPos).normalized();
    crop = crop.intersected(imageSize);

    if (crop.isEmpty()) return;  // Don't remove crop if crop is entirely outside the image

    if (this->cropOverlay) {
        delete this->cropOverlay;
    }

    this->cropOverlay = new CropDimOverlay(imageSize);
    this->scene->addItem(this->cropOverlay);
    this->cropOverlay->setCropRect(crop);
}