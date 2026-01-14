#pragma once

#include "GraphicsView.h"
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QWheelEvent>

class ImageViewer : public QWidget {
    Q_OBJECT

    QGraphicsScene *scene;
    GraphicsView *view;
    QGraphicsPixmapItem *currentImage{ nullptr };
    QGraphicsPixmapItem *cropLayer{ nullptr };

public:
    explicit ImageViewer(QWidget* parent);

    void loadImage(const QString& path);
};