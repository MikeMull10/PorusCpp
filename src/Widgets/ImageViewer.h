#pragma once

#include "Tools.h"
#include "GraphicsView.h"
#include "CropDimOverlay.h"
#include "ScalebarDialog.h"

class ImageToolbar;

#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QWheelEvent>
#include <stack>

class ImageViewer : public QWidget {
    Q_OBJECT

    QGraphicsScene *scene;
    GraphicsView *view;
    QGraphicsPixmapItem *currentImage{ nullptr };
    QGraphicsPixmapItem *outlines{ nullptr };
    CropDimOverlay *cropOverlay{ nullptr };

    TOOL currentTool{ TOOL::NONE };
    QGraphicsRectItem* currentRect{ nullptr };
    QGraphicsLineItem* scaleBarItem{ nullptr };
    QPointF startPos;
    bool isDrawing{ false };

    ImageToolbar* toolbar{ nullptr };
    SCALE scale;

    std::stack<QRectF> undoStack;
    std::stack<QRectF> redoStack;

public:
    explicit ImageViewer(QWidget* parent);
    explicit ImageViewer(ImageToolbar* toolbar, QWidget* parent);

    void loadImage(const QPixmap& pixmap);
    void loadImage(const QString& path);
    void setOutlines(const QPixmap& pixmap);
    void setToolbar(ImageToolbar* toolbar);
    void onToolSwitch(TOOL tool);

    QPixmap getCurrentImage();

    QGraphicsPixmapItem* getImage() const;
    QRectF getCrop() const;

protected:
    bool eventFilter(QObject* obj , QEvent* event) override;

private:
    void startCrop();
    void updateCrop(const QPointF& pos);
    void finishCrop(const QPointF& pos);
    void undoCrop();
    void redoCrop();

    void startScaleBar();
    void updateScaleBar(QPointF& pos, bool shift);
    void finishScaleBar(const QPointF& pos, bool shift);
};