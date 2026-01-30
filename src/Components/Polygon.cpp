#include "Polygon.h"
#include <QGraphicsDropShadowEffect>
#include <QLineF>

qreal distanceToLineSegment(const QPointF &p, const QPointF &a, const QPointF &b);

Polygon::Polygon(bool closed) : QGraphicsPolygonItem(), isClosed(closed) {
    this->edgePen = QPen(Qt::red, 0.5);
    this->fillBrush = QBrush(QColor(255, 0, 0, 120));
    this->setFlag(QGraphicsItem::ItemIsSelectable, true);
    this->setFlag(QGraphicsItem::ItemIsMovable, false);
}

void Polygon::addPoint(const QPointF &point) {
    prepareGeometryChange();
    QPolygonF poly = polygon();
    poly << point;
    this->isClosed = point == this->getPoint(0);
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

QPointF Polygon::getPoint(int index) const {
    QPolygonF poly = polygon();
    if (index >= 0 && index < poly.size()) return poly.at(index);
    return QPointF();
}

int Polygon::getClosestIndex(const QPointF& point) const {
    int index{ -1 };
    qreal closestDistance = INFINITY;
    QPolygonF poly = polygon();

    for (int i = 0; i < poly.size(); i++) {
        qreal dist = QLineF(point, poly.at(i)).length();
        if (dist < closestDistance) {
            closestDistance = dist;
            index = i;
        }
    }

    return index;
}

int Polygon::getNumPoints() const { return polygon().size(); }

void Polygon::insertPoint(int index, const QPointF &point) {
    prepareGeometryChange();
    QPolygonF poly = polygon();  // Get copy
    poly.insert(index, point);   // Insert at index
    setPolygon(poly);            // Set back
    update();
}

void Polygon::insertPointBetween(const QPointF &point) {
    int numPoints = this->getNumPoints();
    if (numPoints < 3) return;

    qreal minDist = std::numeric_limits<qreal>::max();
    int insertIndex = -1;
    
    // Check all edges
    for (int i = 0; i < numPoints; ++i) {
        int next = (i + 1) % numPoints;
        QPointF p1 = this->getPoint(i);
        QPointF p2 = this->getPoint(next);
        
        qreal dist = distanceToLineSegment(point, p1, p2);
        
        if (dist < minDist) {
            minDist = dist;
            insertIndex = next;
        }
    }
    
    if (insertIndex >= 0) this->insertPoint(insertIndex, point);
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
            QGraphicsDropShadowEffect* glow = new QGraphicsDropShadowEffect();
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

qreal distanceToLineSegment(const QPointF &p, const QPointF &a, const QPointF &b) {
    QPointF ab = b - a;
    QPointF ap = p - a;
    
    qreal abLengthSquared = QPointF::dotProduct(ab, ab);
    if (abLengthSquared == 0) {
        // Line segment is actually a point
        return QLineF(p, a).length();
    }
    
    // Project point onto line segment
    qreal t = QPointF::dotProduct(ap, ab) / abLengthSquared;
    t = qBound(0.0, t, 1.0);  // Clamp to segment
    
    QPointF closest = a + t * ab;
    return QLineF(p, closest).length();
}