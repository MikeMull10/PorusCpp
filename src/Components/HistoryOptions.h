#pragma once

#include "ActionHistory.h"
#include "Polygon.h"
#include <QGraphicsScene>
#include <QPointF>

class AddPoint : public History {
    int index;
    QPointF point;
    Polygon* poly;

public:
    AddPoint(int index, const QPointF &point, Polygon *polygon);
    void doAction();
    History* getInverse();
};

class DeletePoint : public History {
    int index;
    QPointF point;
    Polygon* poly;

public:
    DeletePoint(int index, const QPointF &point, Polygon *polygon);
    void doAction();
    History* getInverse();
};

class AddPolygon : public History {
    Polygon* poly;
    QGraphicsScene* scene;

public:
    AddPolygon(Polygon *polygon, QGraphicsScene* scene);
    void doAction();
    History* getInverse();
};

class DeletePolygon : public History {
    Polygon* poly;
    QGraphicsScene* scene;

public:
    DeletePolygon(Polygon *polygon, QGraphicsScene* scene);
    void doAction();
    History* getInverse();
};