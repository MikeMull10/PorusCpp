#pragma once

#include "GraphicsViewEdit.h"
#include "TaskButton.h"
#include "Polygon.h"

#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QWheelEvent>
#include <stack>
#include <opencv2/core.hpp>

class ImageEditor : public QWidget {
    Q_OBJECT

    QVBoxLayout *layout;
    QHBoxLayout *toolbarLayout;
    QGraphicsScene *scene;
    GraphicsViewEdit *view;
    QGraphicsPixmapItem *image{ nullptr };

    std::vector<ToolButton*> toolBtns;
    ImageTools currentTool{ ImageTools::NONE };
    QPointF startPos;

    std::vector<Polygon*> polygons;
    
    static constexpr uint8_t MAX_STACK_SIZE{ 20 };
    std::stack<int> undoStack;
    std::stack<int> redoStack;

public:
    explicit ImageEditor(QWidget* parent = nullptr);

    void loadImage(const QPixmap& pixmap);
    bool imageEmpty() const;
    void loadContours(std::vector<std::vector<cv::Point>> contours);

    void undo();
    void redo();
    void resetZoom();

private:
    void initToolbar();
    void setActiveTool(ToolButton *btn);
};