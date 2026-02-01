#include "GraphicsViewEdit.h"
#include <QLineF>
#include <QPainter>
#include <QApplication>
#include <QScrollBar>

#define DEBUG true

GraphicsViewEdit::GraphicsViewEdit(QGraphicsScene* scene, QWidget* parent, ImageTools* currentTool) : QGraphicsView(scene, parent) {
    this->currentTool = currentTool;
    this->setRenderHint(QPainter::Antialiasing);
    this->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
}

void GraphicsViewEdit::setHistories(std::stack<History*>* undo, std::stack<History*>* redo) {
    this->undoHistory = undo;
    this->redoHistory = redo;
}

void GraphicsViewEdit::clearSelection() {
    this->selection.clear();
}

void GraphicsViewEdit::wheelEvent(QWheelEvent* event) {
    const double scaleFactor = 1.15;
    
    if (event->angleDelta().y() > 0) {
        scale(scaleFactor, scaleFactor);
    } else {
        scale(1.0 / scaleFactor, 1.0 / scaleFactor);
    }
}

void GraphicsViewEdit::mousePressEvent(QMouseEvent* event) {
    QPointF scenePos = mapToScene(event->pos());

    if ((*this->currentTool == ImageTools::HAND && event->button() == Qt::LeftButton) ||
        event->button() == Qt::MiddleButton) {
        // PAN
        this->isPanning = true;
        this->lastPanPoint = event->pos();
        this->setCursor(Qt::ClosedHandCursor);
        event->accept();
        return;
    }

    switch (*this->currentTool) {
        case ImageTools::ZOOM: {
            if (!event->button() == Qt::LeftButton) return;
            this->isZooming = true;
            this->lastPanPoint = event->pos();

            QPen pen(Qt::red);
            pen.setWidth(2);
            pen.setCosmetic(true);
            pen.setStyle(Qt::DashLine);

            this->startPos = scenePos;
            this->zoomRect = this->scene()->addRect(QRectF(scenePos, scenePos), pen, QBrush(QColor(255, 0, 0, 50)));
            this->zoomRect->setZValue(100);
            event->accept();
            return;
        }
        case ImageTools::ADD_POINT: {
            Polygon* poly;
            int index = -1;
            if (this->selection.size() > 1) return;
            else if (this->selection.size() == 1) {
                poly = this->selection.at(0);
                if (poly->isClosedPolygon()) {              // ADD POINT BETWEEN
                    if (event->button() != Qt::LeftButton) return;
                    index = poly->insertPointBetween(scenePos);
                } else {                                    // ADD POINT TO END
                    if (event->button() == Qt::LeftButton) index = poly->addPoint(scenePos);
                    else if (event->button() == Qt::RightButton && poly->getNumPoints() > 2) {
                        index = poly->addPoint(poly->getPoint(0));
                        scenePos = poly->getPoint(0);
                    }
                }
            } else {                                         // CREATE & ADD POINT TO POLYGON
                if (event->button() != Qt::LeftButton) return;

                poly = new Polygon();
                poly->setSelected(true);
                index = poly->addPoint(scenePos);
                this->selection.push_back(poly);
                this->scene()->addItem(poly);
            }
            // ADD to UNDO STACK
            if (poly && index >= 0) undoHistory->push(new DeletePoint(index, scenePos, poly));
            return;
        }
        
        case ImageTools::DELETE_POLYGON: {
            this->selection.clear();
            QGraphicsItem* item = scene()->itemAt(scenePos, transform());
        
            Polygon* poly = dynamic_cast<Polygon*>(item);
            if (!poly) return;

            this->scene()->removeItem(poly);
            undoHistory->push(new AddPolygon(poly, scene()));
            return;
        }

        case ImageTools::SELECT_POINT: {
            this->selection.clear();
            QGraphicsItem* item = scene()->itemAt(scenePos, transform());
            Polygon* poly = dynamic_cast<Polygon*>(item);
            if (!poly) return;
            this->selection.push_back(poly);
        }
    }

    QGraphicsView::mousePressEvent(event);
}

void GraphicsViewEdit::mouseMoveEvent(QMouseEvent* event) {
    if (this->isPanning) {
        QPoint delta = event->pos() - lastPanPoint;
        lastPanPoint = event->pos();
        
        // Pan the view
        horizontalScrollBar()->setValue(horizontalScrollBar()->value() - delta.x());
        verticalScrollBar()->setValue(verticalScrollBar()->value() - delta.y());
        
        event->accept();
        return;
    } else if (this->isZooming) {
        if (this->zoomRect) {
            QRectF rect = QRectF(startPos, mapToScene(event->pos()));
            this->zoomRect->setRect(rect);
        }
    }
    
    QGraphicsView::mouseMoveEvent(event);
}

void GraphicsViewEdit::mouseReleaseEvent(QMouseEvent* event) {
    if (this->isPanning) {
        this->isPanning = false;
        this->updateCursor();
        event->accept();
        return;
    } else if (this->isZooming) {
        this->isZooming = false;
        delete zoomRect;
        zoomRect = nullptr;
        
        QPointF pos = mapToScene(event->pos());
        QRectF zoom = QRectF(this->startPos, pos).normalized();

        this->fitInView(zoom, Qt::KeepAspectRatio);
    }
    
    QGraphicsView::mouseReleaseEvent(event);
}

void GraphicsViewEdit::updateCursor() {
    switch (*this->currentTool) {
        case ImageTools::HAND:
            setCursor(Qt::OpenHandCursor);
            break;
        case ImageTools::ZOOM:
            setCursor(Qt::CrossCursor);
            break;
        case ImageTools::ADD_POINT:
        case ImageTools::ADD_POINTS_FOLLOW:
            setCursor(Qt::CrossCursor);
            break;
        case ImageTools::DELETE_POINT:
        case ImageTools::DELETE_POLYGON:
            setCursor(Qt::PointingHandCursor);
            break;
        case ImageTools::MOVE_POINT:
            setCursor(Qt::SizeAllCursor);
            break;
        case ImageTools::SELECT_POINT:
        case ImageTools::SELECT_RECT:
        case ImageTools::SELECT_LASSO:
            setCursor(Qt::ArrowCursor);
            break;
        default:
            setCursor(Qt::ArrowCursor);
    }
}