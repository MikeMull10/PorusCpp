#pragma once

#include "Tools.h"
#include "GraphicsView.h"
#include "CropDimOverlay.h"

class ImageToolbar;

#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QWheelEvent>

class ImageViewer : public QWidget {
    Q_OBJECT

    QGraphicsScene *scene;
    GraphicsView *view;
    QGraphicsPixmapItem *currentImage{ nullptr };
    CropDimOverlay *cropOverlay{ nullptr };

    TOOL currentTool{ TOOL::NONE };
    QGraphicsRectItem* currentRect{ nullptr };
    QPointF cropStart;
    bool isDrawing{ false };

    ImageToolbar* toolbar{ nullptr };

public:
    explicit ImageViewer(QWidget* parent);
    explicit ImageViewer(ImageToolbar* toolbar, QWidget* parent);

    void loadImage(const QString& path);
    void setToolbar(ImageToolbar* toolbar);
    void onToolSwitch(TOOL tool);

protected:
    bool eventFilter(QObject* obj , QEvent* event) override;

private:
    void startCrop();
    void updateCrop(const QPointF& pos);
    void finishCrop(const QPointF& pos);
};