#include "GraphicsViewEdit.h"
#include <QLineF>

#define DEBUG true

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
    // QGraphicsItem* item = scene()->itemAt(scenePos, transform());
    
    // Polygon* poly = dynamic_cast<Polygon*>(item);
    // if (!poly) {
    //     return;
    // }

    // if (*this->currentTool == ImageTools::SELECT_POINT) {
    //     poly->setSelected(true);
    // }

    switch (*this->currentTool) {
        case ImageTools::ADD_POINT:
            if (this->selection.size() > 1) return;
            else if (this->selection.size() == 1) {
                Polygon* poly = this->selection.at(0);
                if (poly->isClosedPolygon()) {              // ADD POINT BETWEEN
                    if (event->button() != Qt::LeftButton) return;
                    poly->insertPointBetween(scenePos);
                } else {                                    // ADD POINT TO END
                    if (event->button() == Qt::LeftButton) poly->addPoint(scenePos);
                    else if (event->button() == Qt::RightButton && poly->getNumPoints() > 2) poly->addPoint(poly->getPoint(0));
                }
            } else {
                // CREATE & ADD POINT TO POLYGON
                if (event->button() != Qt::LeftButton) return;

                Polygon* poly = new Polygon();
                poly->setSelected(true);
                poly->addPoint(scenePos);
                this->selection.push_back(poly);
                this->scene()->addItem(poly);
            }
            break;
    }
}