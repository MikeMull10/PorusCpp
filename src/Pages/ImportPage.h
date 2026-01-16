#pragma once

#include <QWidget>
#include <QHBoxLayout>
#include "ImageViewer.h"

class ImportPage : public QWidget {
    Q_OBJECT

    QHBoxLayout* mainLayout;
    ImageViewer* imageViewer;

    QGraphicsPixmapItem* baseImage{ nullptr };
    QRectF crop;

public:
    explicit ImportPage(QWidget* parent = nullptr);
    void setup();

    void loadImage(const QString& path);

    void setBaseImage(QGraphicsPixmapItem* image);
    QGraphicsPixmapItem* getBaseImage() const;

    void setCrop(QRectF crop);
    QRectF getCrop() const;
};