#pragma once

#include <QGraphicsEllipseItem>
#include <QGraphicsSceneMouseEvent>
#include <QPen>
#include <QBrush>

class Polygon;  // Forward declaration

class PointHandle : public QGraphicsEllipseItem {
private:
    Polygon* parentPolygon;
    int pointIndex;

public:
    PointHandle(const QPointF &pos, int index, Polygon* parent);
    
    int getPointIndex() const { return pointIndex; }
    Polygon* getParentPolygon() const { return parentPolygon; }
    
    void setHighlighted(bool highlighted);

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
};