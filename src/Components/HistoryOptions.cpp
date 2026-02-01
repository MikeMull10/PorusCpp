#include "HistoryOptions.h"

AddPoint::AddPoint(int index, const QPointF &point, Polygon *polygon) {
    this->index = index;
    this->point = point;
    this->poly  = polygon;
}

void AddPoint::doAction() {
    if (this->index > this->poly->getNumPoints() - 1) this->poly->addPoint(this->point);
    else this->poly->insertPoint(this->index, this->point);
}

History* AddPoint::getInverse() {
    return new DeletePoint(this->index, this->point, this->poly);
}

DeletePoint::DeletePoint(int index, const QPointF &point, Polygon *polygon) {
    this->index = index;
    this->point = point;
    this->poly  = polygon;
}

void DeletePoint::doAction() {
    this->poly->removePoint(this->index);
}

History* DeletePoint::getInverse() {
    return new AddPoint(this->index, this->point, this->poly);
}

AddPolygon::AddPolygon(Polygon* polygon, QGraphicsScene* scene) : poly(polygon), scene(scene) {}

void AddPolygon::doAction() {
    this->scene->addItem(this->poly);
}

History* AddPolygon::getInverse() {
    return new DeletePolygon(this->poly, this->scene);
}

DeletePolygon::DeletePolygon(Polygon* polygon, QGraphicsScene* scene) : poly(polygon), scene(scene) {}

void DeletePolygon::doAction() {
    this->scene->removeItem(this->poly);
}

History* DeletePolygon::getInverse() {
    return new AddPolygon(this->poly, this->scene);
}
