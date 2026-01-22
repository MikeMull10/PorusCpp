#include "ImageViewer.h"
#include "ImageToolbar.h"
#include "ScalebarDialog.h"

#include <QGraphicsPixmapItem>
#include <QVBoxLayout>
#include <QApplication>
#include <QDialog>
#include <QLabel>
#include <QTimer>
#include <QShortcut>

ImageViewer::ImageViewer(QWidget* parent) : QWidget(parent) {
    setObjectName("ImageViewer");

    this->scene = new QGraphicsScene(this);
    this->view = new GraphicsView(this->scene, this);
    view->setMouseTracking(true);
    view->viewport()->setMouseTracking(true);

    view->setFocusPolicy(Qt::StrongFocus);
    view->viewport()->setFocusPolicy(Qt::StrongFocus);
    view->setFocus();
    
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(view);
    setLayout(layout);

    this->view->setDragMode(QGraphicsView::NoDrag);
    this->view->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    
    this->view->viewport()->installEventFilter(this);
    view->installEventFilter(this);

    QShortcut* undoShortcut = new QShortcut(QKeySequence("Ctrl+Z"), this);
    connect(undoShortcut, &QShortcut::activated, this, &ImageViewer::undoCrop);
    QShortcut* redoShortcut = new QShortcut(QKeySequence("Ctrl+Y"), this);
    connect(redoShortcut, &QShortcut::activated, this, &ImageViewer::redoCrop);
}

ImageViewer::ImageViewer(ImageToolbar* toolbar, QWidget* parent) : ImageViewer(parent) {
    this->toolbar = toolbar;
    connect(this->toolbar, &ImageToolbar::toolActivated, this, &ImageViewer::onToolSwitch);
}

void ImageViewer::loadImage(const QPixmap &pixmap) {
    undoStack = {};
    redoStack = {};

    if (this->currentImage) {
        this->scene->removeItem(this->currentImage);
        delete this->currentImage;
    }

    this->currentImage = this->scene->addPixmap(pixmap);
    this->currentImage->setZValue(0);
        
    QPointF pos(this->currentImage->sceneBoundingRect().width(), this->currentImage->sceneBoundingRect().height());
    this->startPos = QPointF(0.0f, 0.0f);
    this->finishCrop(pos);

    QTimer::singleShot(1, this, [this]() {
        this->view->fitInView(this->currentImage, Qt::KeepAspectRatio);
    });
}

void ImageViewer::loadImage(const QString& path) {
    QPixmap img(path);
    this->loadImage(img);
}

void ImageViewer::setOutlines(const QPixmap &image) {
    if (!this->outlines) {
        this->outlines = this->scene->addPixmap(image);
        this->outlines->setZValue(2);
        return;
    }

    this->outlines->setPixmap(image);
}

void ImageViewer::setToolbar(ImageToolbar* toolbar) {
    this->toolbar = toolbar;
    connect(this->toolbar, &ImageToolbar::toolActivated, this, &ImageViewer::onToolSwitch);
}

void ImageViewer::onToolSwitch(TOOL tool) {
    this->currentTool = tool;

    if (this->currentTool == TOOL::HAND) { this->view->setDragMode(QGraphicsView::ScrollHandDrag); qDebug() << "TEST"; }
    else this->view->setDragMode(QGraphicsView::NoDrag);
}

QGraphicsPixmapItem* ImageViewer::getImage() const { return this->currentImage; }
QRectF ImageViewer::getCrop() const { return this->cropOverlay->getCropRect(); }

bool ImageViewer::eventFilter(QObject* obj, QEvent* event)
{
    if (!toolbar || currentTool == TOOL::NONE || currentTool == TOOL::HAND)
        return QWidget::eventFilter(obj, event);

    // --- Mouse handling ---
    if (event->type() != QEvent::MouseButtonPress &&
        event->type() != QEvent::MouseMove &&
        event->type() != QEvent::MouseButtonRelease) {
        return QWidget::eventFilter(obj, event);
    }

    QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);

    if (event->type() != QEvent::MouseMove &&
        mouseEvent->button() != Qt::LeftButton) {
        return false;
    }

    bool shiftHeld = QApplication::keyboardModifiers() & Qt::ShiftModifier;

    QPointF scenePos = view->mapToScene(mouseEvent->pos());

    if (event->type() == QEvent::MouseButtonPress) {
        startPos = scenePos;

        if (currentTool == TOOL::CROP)
            startCrop();
        else if (currentTool == TOOL::SCALE)
            startScaleBar();

        return true;
    }

    else if (event->type() == QEvent::MouseMove && isDrawing) {
        if (currentTool == TOOL::CROP)
            updateCrop(scenePos);
        else if (currentTool == TOOL::SCALE)
            updateScaleBar(scenePos, shiftHeld);

        return true;
    }

    else if (event->type() == QEvent::MouseButtonRelease && isDrawing) {
        if (currentTool == TOOL::CROP)
            finishCrop(scenePos);
        else if (currentTool == TOOL::SCALE)
            finishScaleBar(scenePos, shiftHeld);

        return true;
    }

    return QWidget::eventFilter(obj, event);
}

void ImageViewer::startCrop() {
    this->isDrawing = true;

    QPen pen(Qt::red);
    pen.setWidth(2);
    pen.setCosmetic(true);
    pen.setStyle(Qt::SolidLine);

    this->currentRect = this->scene->addRect(QRectF(this->startPos, this->startPos), pen, QBrush(QColor(255, 0, 0, 50)));
    this->currentRect->setZValue(100);
}

void ImageViewer::updateCrop(const QPointF& pos) {
    if (this->currentRect) {
        QRectF rect = QRectF(this->startPos, pos).normalized();
        this->currentRect->setRect(rect);
    }
}

void ImageViewer::finishCrop(const QPointF& pos) {
    this->isDrawing = false;
    delete currentRect;
    currentRect = nullptr;

    QRectF imageSize = this->currentImage->sceneBoundingRect();
    QRectF crop = QRectF(this->startPos, pos).normalized().intersected(imageSize);
    if (crop.isEmpty()) return;

    if (!this->cropOverlay) {
        this->cropOverlay = new CropDimOverlay(imageSize);
        this->scene->addItem(this->cropOverlay);
    }

    // Save **current state** to undo stack before changing
    undoStack.push(this->cropOverlay->getCropRect());
    redoStack = {};

    this->cropOverlay->setCropRect(crop);
}

void ImageViewer::undoCrop() {
    if (undoStack.size() <= 1 || !this->cropOverlay) return;

    QRectF previous = undoStack.top();
    undoStack.pop();

    // Save current crop to redo
    redoStack.push(this->cropOverlay->getCropRect());

    this->cropOverlay->setCropRect(previous);
}

void ImageViewer::redoCrop() {
    if (redoStack.empty() || !this->cropOverlay) return;

    QRectF next = redoStack.top();
    redoStack.pop();

    // Save current crop to undo
    undoStack.push(this->cropOverlay->getCropRect());

    this->cropOverlay->setCropRect(next);
}

void ImageViewer::startScaleBar() {
    this->isDrawing = true;
    this->scaleBarItem = this->scene->addLine(QLineF(this->startPos, this->startPos), QPen(Qt::red, 2));
    this->scaleBarItem->setZValue(100);
}

void ImageViewer::updateScaleBar(QPointF& pos, bool shift) {
    if (!this->scaleBarItem) return;

    if (shift) {
        float diffX = std::abs(this->startPos.x() - pos.x());
        float diffY = std::abs(this->startPos.y() - pos.y());

        if (diffX > diffY) {
            pos.setY(this->startPos.y());
        } else {
            pos.setX(this->startPos.x());
        }
    }

    QLineF line = QLineF(this->startPos, pos);
    this->scaleBarItem->setLine(line);
}

void ImageViewer::finishScaleBar(const QPointF& pos, bool shift) {
    this->isDrawing = false;
    qreal length = this->scaleBarItem->line().length();

    delete scaleBarItem;
    scaleBarItem = nullptr;

    ScalebarDialog* popup = new ScalebarDialog(static_cast<float>(length), this);
    if (popup->exec() == QDialog::Accepted) {
        SCALE result = popup->getScale();
        // TODO: Do stuff with the scale
    }
    delete popup;
}

QPixmap ImageViewer::getCurrentImage() { return this->currentImage->pixmap(); }