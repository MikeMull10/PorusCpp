#include "Polygon.h"
#include "CloseGap.h"

#include <QGraphicsDropShadowEffect>
#include <QGraphicsScene>
#include <QLineF>

qreal distanceToLineSegment(const QPointF &p, const QPointF &a, const QPointF &b);
bool isValidClosure(const QPolygonF&);
double calculatePolygonArea(const QPolygonF&);
QPolygonF calculateConvexHull(const QPolygonF&);

Polygon::Polygon(QGraphicsScene* scene) : QGraphicsPolygonItem() {
    this->scene = scene;
    this->edgePen = QPen(Qt::red, 0.5);
    this->fillBrush = QBrush(QColor(255, 0, 0, 120));
    this->setFlag(QGraphicsItem::ItemIsSelectable, true);
    this->setFlag(QGraphicsItem::ItemIsMovable, false);
}

int Polygon::addPoint(const QPointF &point) {
    this->prepareGeometryChange();
    QPolygonF poly = polygon();
    poly << point;
    this->isClosed = point == this->getPoint(0);
    this->setPolygon(poly);
    this->updatePointsItems();
    this->update();
    return poly.size() - 1;
}

void Polygon::removeLastPoint() {
    prepareGeometryChange();
    QPolygonF poly = polygon();
    if (!poly.isEmpty()) {
        poly.pop_back();
        setPolygon(poly);
    }
    this->updatePointsItems();
    update();
}

void Polygon::removePoint(int index) {
    this->prepareGeometryChange();
    QPolygonF poly = polygon();
    if (!poly.isEmpty()) {
        poly.removeAt(index);
        this->setPolygon(poly);
    }
    this->isClosed = this->getPoint(0) == this->getPoint(poly.size() - 1);
    this->updatePointsItems();
    this->update();
}

void Polygon::clearPoints() {
    prepareGeometryChange();
    setPolygon(QPolygonF());
    update();
}

void Polygon::setPoints(const QPolygonF &points) {
    prepareGeometryChange();

    QPolygonF newPoints = points;

    if (newPoints.size() > 2) {
        QPointF p1 = newPoints.at(0);
        QPointF p2 = newPoints.at(newPoints.size() - 1);
        double distance = QLineF(p1, p2).length();
        this->isClosed = distance <= 1.0f;
        
        if (this->isClosed) {
            newPoints.pop_back();
            newPoints.append(points.first());

            if (!isValidClosure(newPoints)) {
                this->edgePen = QPen(Qt::blue, 0.5);
                this->fillBrush = QBrush(QColor(0, 0, 255, 120));
                newPoints = closePolygon(newPoints);
            }
        }
    }
    this->updatePointsItems();
    setPolygon(newPoints);
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
    this->isClosed = this->getPoint(0) == this->getPoint(poly.size() - 1);
    this->updatePointsItems();
    update();
}

int Polygon::insertPointBetween(const QPointF &point) {
    int numPoints = this->getNumPoints();
    if (numPoints < 3) return -1;

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
    
    return insertIndex;
}

void Polygon::setClosed(bool closed) {
    if (isClosed != closed) {
        isClosed = closed;
        update();
    }
}
bool Polygon::isClosedPolygon() const { return this->isClosed; }

void Polygon::setSelected(bool selected) {
    this->isSelected = selected;
    this->updatePointsItems();
}
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

void Polygon::updatePointsItems() {
    for (QGraphicsEllipseItem* item : this->pointItems) delete item;
    this->pointItems.clear();
    if (!this->isSelected) return;

    QPolygonF poly = polygon();
    
    const qreal r = 0.25f;  // radius

    for (int i = 0; i < poly.size(); ++i) {
        const QPointF& p = poly[i];

        QGraphicsEllipseItem* item = this->scene->addEllipse(
            -r, -r, 2*r, 2*r,
            QPen(Qt::white),
            QBrush(Qt::black)
        );
        item->setZValue(10);

        // Position in scene coordinates
        item->setPos(p);

        this->pointItems.push_back(item);
    }
}

qreal Polygon::getArea() const { return calculatePolygonArea(polygon()); }

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

bool isValidClosure(const QPolygonF& polygon) {
    if (polygon.size() < 3) return false;
    
    // Ensure polygon is closed
    QPolygonF closed = polygon;
    if (closed.first() != closed.last()) {
        closed.append(closed.first());
    }
    
    // Calculate actual polygon area
    double actualArea = calculatePolygonArea(closed);
    
    // Calculate convex hull
    QPolygonF hull = calculateConvexHull(closed);
    double hullArea = calculatePolygonArea(hull);
    
    // Solidity
    double solidity = actualArea / (hullArea + 1e-6);
    
    return solidity > 0.35;
}

double calculatePolygonArea(const QPolygonF& polygon) {
    if (polygon.size() < 3) return 0.0;
    
    double area = 0.0;
    int n = polygon.size();
    
    for (int i = 0; i < n; i++) {
        int j = (i + 1) % n;
        area += polygon[i].x() * polygon[j].y();
        area -= polygon[j].x() * polygon[i].y();
    }
    
    return std::abs(area) / 2.0;
}

QPolygonF calculateConvexHull(const QPolygonF& polygon) {
    if (polygon.size() < 3) return polygon;
    
    // Convert to std::vector for easier manipulation
    std::vector<QPointF> points;
    for (int i = 0; i < polygon.size(); i++) {
        points.push_back(polygon.at(i));
    }
    
    // Sort points by x, then y
    std::sort(points.begin(), points.end(), [](const QPointF& a, const QPointF& b) {
        return a.x() < b.x() || (a.x() == b.x() && a.y() < b.y());
    });
    
    // Andrew's monotone chain algorithm
    auto cross = [](const QPointF& O, const QPointF& A, const QPointF& B) {
        return (A.x() - O.x()) * (B.y() - O.y()) - (A.y() - O.y()) * (B.x() - O.x());
    };
    
    std::vector<QPointF> hull;
    
    // Build lower hull
    for (const auto& p : points) {
        while (hull.size() >= 2 && cross(hull[hull.size()-2], hull[hull.size()-1], p) <= 0) {
            hull.pop_back();
        }
        hull.push_back(p);
    }
    
    // Build upper hull
    size_t lowerSize = hull.size();
    for (int i = points.size() - 2; i >= 0; i--) {
        while (hull.size() > lowerSize && cross(hull[hull.size()-2], hull[hull.size()-1], points[i]) <= 0) {
            hull.pop_back();
        }
        hull.push_back(points[i]);
    }
    
    hull.pop_back(); // Remove duplicate point

    QPolygonF ret;
    for (int i = 0; i < hull.size(); i++) {
        ret.append(hull.at(i));
    }
    
    return ret;
}