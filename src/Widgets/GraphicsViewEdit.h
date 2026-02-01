#pragma once

#include "Polygon.h"
#include "HistoryOptions.h"
class ImageEditor;

#include <QGraphicsView>
#include <QWheelEvent>
#include <stack>

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

    // ZOOMING & PANNING
    bool isPanning{ false };
    bool isZooming{ false };
    QPointF startPos;
    QPoint lastPanPoint;
    QGraphicsRectItem* zoomRect{ nullptr };

    // Tool and Selected Pore(s)
    ImageTools* currentTool{ nullptr };
    std::vector<Polygon*> selection;

    // Allow Undo & Redo
    std::stack<History*>* undoHistory;
    std::stack<History*>* redoHistory;
    
public:
    explicit GraphicsViewEdit(QGraphicsScene* scene, QWidget* parent = nullptr, ImageTools* currentTool = nullptr);
    void setHistories(std::stack<History*>* undo, std::stack<History*>* redo);
    void clearSelection();
    void updateCursor();

protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;
};