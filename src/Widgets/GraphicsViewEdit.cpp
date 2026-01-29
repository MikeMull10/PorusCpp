#include "GraphicsViewEdit.h"

GraphicsViewEdit::GraphicsViewEdit(QGraphicsScene* scene, QWidget* parent, ImageTools* currentTool) : QGraphicsView(scene, parent) {
    this->currentTool = currentTool;
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
    QGraphicsItem* item = scene()->itemAt(scenePos, transform());
    
    Polygon* poly = dynamic_cast<Polygon*>(item);
    if (!poly) {
        return;
    }

    if (*this->currentTool == ImageTools::SELECT_POINT) {
        poly->setSelected(true);
    }
}