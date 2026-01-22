#pragma once

#include <QWidget>
#include <QHBoxLayout>
#include <QSlider>

#include "ImageViewer.h"
#include <unordered_map>

class ImportPage : public QWidget {
    Q_OBJECT

    QHBoxLayout* mainLayout;
    QVBoxLayout* settingsLayout;
    ImageViewer* imageViewer;

    QGraphicsPixmapItem* baseImage{ nullptr };
    QRectF crop;

    // Settings
    std::unordered_map<std::string, QSlider*> sliders;

public:
    explicit ImportPage(QWidget* parent = nullptr);
    void setup();
    void setupSliders();
    void addSlider(std::string key, int default_value, int min_value, int max_value, std::string tooltip = "");

    void loadImage(const QString& path);
    void updateImage();

    void setBaseImage(QGraphicsPixmapItem* image);
    QGraphicsPixmapItem* getBaseImage() const;

    void setCrop(QRectF crop);
    QRectF getCrop() const;
};