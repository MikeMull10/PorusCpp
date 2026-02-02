#pragma once

#include <QGraphicsEllipseItem>
#include <QGraphicsPolygonItem>
#include <QGraphicsPathItem>
#include <QPainter>
#include <QPainterPath>
#include <vector>

class Polygon : public QGraphicsPolygonItem {
private:
    bool isClosed;
    bool isSelected{ false };
    QPen edgePen;
    QBrush fillBrush;
    
    QGraphicsScene* scene;
    std::vector<QGraphicsEllipseItem*> pointItems;

public:
    Polygon(QGraphicsScene*);

    // Polygon management
    int addPoint(const QPointF &point);
    void removeLastPoint();
    void removePoint(int index);
    void clearPoints();
    void setPoints(const QPolygonF &points);

    QPointF getPoint(int index) const;
    int getClosestIndex(const QPointF& point) const;
    int getNumPoints() const;

    void insertPoint(int index, const QPointF &point);
    int insertPointBetween(const QPointF &point);
    
    // Closed/Open state
    void setClosed(bool closed);
    bool isClosedPolygon() const;

    // Selected (T/F)
    void setSelected(bool selected);
    bool getIsSelected() const;
    
    // Appearance
    void setEdgePen(const QPen &pen);
    void setFillBrush(const QBrush &brush);
    
    // Override paint to handle open/closed
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, 
               QWidget *widget = nullptr) override;
    
    // Bounding rect
    QRectF boundingRect() const override;

    void updatePointsItems();

    qreal getArea() const;
};