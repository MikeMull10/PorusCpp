#include "Polygon.h"
#include <QGraphicsDropShadowEffect>

Polygon::Polygon(bool closed) : QGraphicsPolygonItem(), isClosed(closed) {
    edgePen = QPen(Qt::blue, 2);
    fillBrush = QBrush(Qt::transparent);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemIsMovable, true);
}

void Polygon::addPoint(const QPointF &point) {
    prepareGeometryChange();
    QPolygonF poly = polygon();
    poly << point;
    setPolygon(poly);
    update();
}

void Polygon::removeLastPoint() {
    prepareGeometryChange();
    QPolygonF poly = polygon();
    if (!poly.isEmpty()) {
        poly.pop_back();
        setPolygon(poly);
    }
    update();
}

void Polygon::clearPoints() {
    prepareGeometryChange();
    setPolygon(QPolygonF());
    update();
}

void Polygon::setPoints(const QPolygonF &points) {
    prepareGeometryChange();
    setPolygon(points);
    update();
}

void Polygon::setClosed(bool closed) {
    if (isClosed != closed) {
        isClosed = closed;
        update();
    }
}

bool Polygon::isClosedPolygon() const { return this->isClosed; }

void Polygon::setSelected(bool selected) { this->isSelected = selected; }
bool Polygon::getIsSelected() const { return this->isSelected; }

void Polygon::setEdgePen(const QPen &pen) {
    edgePen = pen;
    update();
}

void Polygon::setFillBrush(const QBrush &brush) {
    fillBrush = brush;
    update();
}

void Polygon::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,  QWidget *widget) {
    Q_UNUSED(option);
    Q_UNUSED(widget);
    
    QPolygonF poly = polygon();
    if (poly.isEmpty()) {
        return;
    }
    
    painter->setPen(edgePen);
    
    if (isClosed) {
        // Draw as closed polygon with fill
        painter->setBrush(fillBrush);
        painter->drawPolygon(poly);
    } else {
        // Draw as open polyline (no fill, no auto-close)
        painter->setBrush(Qt::NoBrush);
        painter->drawPolyline(poly);
    }
}

QRectF Polygon::boundingRect() const {
    QPolygonF poly = polygon();
    if (poly.isEmpty()) {
        return QRectF();
    }
    
    // Add some padding for pen width
    qreal penWidth = edgePen.widthF();
    return poly.boundingRect().adjusted(-penWidth, -penWidth, penWidth, penWidth);
}

void Polygon::updateHandleAppearance() {
    for (PointHandle* handle : handles) {
        if (this->isSelected) {
            // Highly visible for selected polygon
            handle->setBrush(QBrush(QColor(255, 200, 0)));  // Bright yellow/orange
            handle->setPen(QPen(Qt::black, 3));  // Strong contrast
            handle->setRect(-6, -6, 12, 12);  // Larger
            handle->setZValue(1000);  // Always on top
            
            // Optional: add glow
            auto* glow = new QGraphicsDropShadowEffect();
            glow->setBlurRadius(10);
            glow->setColor(Qt::yellow);
            glow->setOffset(0, 0);
            handle->setGraphicsEffect(glow);
        } else {
            // Subtle for unselected polygon
            handle->setBrush(QBrush(QColor(100, 150, 255, 180)));
            handle->setPen(QPen(Qt::white, 1));
            handle->setRect(-4, -4, 8, 8);
            handle->setZValue(10);
            handle->setGraphicsEffect(nullptr);  // Remove glow
        }
    }
}

QVariant Polygon::itemChange(GraphicsItemChange change, const QVariant &value) {
    if (change == ItemSelectedHasChanged) {
        if (value.toBool()) {
            // Selected - create handles
            editMode = true;
            // createHandles();
        } else {
            // Deselected - remove handles
            editMode = false;
            // clearHandles();
        }
    }
    return QGraphicsPolygonItem::itemChange(change, value);
}