#include "GraphicsView.h"

GraphicsView::GraphicsView(QGraphicsScene* scene, QWidget* parent)
    : QGraphicsView(scene, parent) {
}

void GraphicsView::wheelEvent(QWheelEvent* event) {
    const double scaleFactor = 1.15;
    
    if (event->angleDelta().y() > 0) {
        scale(scaleFactor, scaleFactor);
    } else {
        scale(1.0 / scaleFactor, 1.0 / scaleFactor);
    }
}
