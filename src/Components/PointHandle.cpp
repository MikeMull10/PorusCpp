#include "PointHandle.h"
#include "Polygon.h"

PointHandle::PointHandle(const QPointF &pos, int index, Polygon* parent) 
    : QGraphicsEllipseItem(-5, -5, 10, 10),  // 10x10 circle centered at origin
      parentPolygon(parent), 
      pointIndex(index) {
    setPos(pos);
    setBrush(QBrush(Qt::blue));
    setPen(QPen(Qt::white, 2));
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
    setZValue(100);  // Draw on top of polygons
}

QVariant PointHandle::itemChange(GraphicsItemChange change, const QVariant &value) {
    if (change == ItemPositionHasChanged && scene() && parentPolygon) {
        // Update polygon point when handle moves
        QPointF newPos = value.toPointF();
        // parentPolygon->updatePoint(pointIndex, newPos);
    }
    return QGraphicsEllipseItem::itemChange(change, value);
}

void PointHandle::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    if (event->button() == Qt::RightButton) {
        // Right-click to delete point
        if (parentPolygon) {
            // parentPolygon->removePoint(pointIndex);
        }
        event->accept();
        return;
    }
    QGraphicsEllipseItem::mousePressEvent(event);
}

void PointHandle::setHighlighted(bool highlighted) {
    if (highlighted) {
        setBrush(QBrush(QColor(255, 200, 0)));  // Yellow/orange
        setPen(QPen(Qt::black, 3));
        setRect(-6, -6, 12, 12);  // Bigger
        setZValue(1000);
    } else {
        setBrush(QBrush(Qt::blue));
        setPen(QPen(Qt::white, 2));
        setRect(-5, -5, 10, 10);  // Normal size
        setZValue(100);
    }
}