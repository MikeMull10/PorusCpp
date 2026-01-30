#pragma once

#include "Polygon.h"
class ImageEditor;

#include <QGraphicsView>
#include <QWheelEvent>

enum class ImageTools {
    NONE,
    HAND,
    ZOOM,
    ADD_POINT,
    DELETE_POINT,
    MOVE_POINT,
    SELECT_POINT,
    SELECT_RECT,
    SELECT_LASSO,
    COMBINE_POINTS,
    ADD_POINTS_FOLLOW,
    DELETE_POLYGON
};

class GraphicsViewEdit : public QGraphicsView {
    Q_OBJECT

    ImageTools* currentTool{ nullptr };
    std::vector<Polygon*> selection;
    
public:
    explicit GraphicsViewEdit(QGraphicsScene* scene, QWidget* parent = nullptr, ImageTools* currentTool = nullptr);
    
protected:
    void mousePressEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;
};