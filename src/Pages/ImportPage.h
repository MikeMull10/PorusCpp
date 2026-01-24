#pragma once

#include <QWidget>
#include <QHBoxLayout>
#include <QSlider>

#include "ImageViewer.h"
#include "ImageToolbar.h"
#include "PushButton.h"
#include <unordered_map>
#include <opencv2/core.hpp>

class ImportPage : public QWidget {
    Q_OBJECT

    QHBoxLayout* mainLayout;
    QVBoxLayout* settingsLayout;
    QVBoxLayout* viewLayout;
    ImageViewer* imageViewer;
    ImageToolbar* toolbar;

    PushButton* sendBtn{ nullptr };
    ToolButton* showBackgroundBtn{ nullptr };

    QGraphicsPixmapItem* baseImage{ nullptr };
    QRectF crop;

    // Settings
    std::unordered_map<std::string, QSlider*> sliders;

    cv::Vec3b outlineColor;

public:
    explicit ImportPage(QWidget* parent = nullptr);
    void setup();
    void setupSliders();
    void addSlider(std::string key, int default_value, int min_value, int max_value, std::string tooltip = "");

    void loadImage(const QString& path);
    void updateImage();

    void setColor(QColor color);

    void setBaseImage(QGraphicsPixmapItem* image);
    QGraphicsPixmapItem* getBaseImage() const;

    void setCrop(QRectF crop);
    QRectF getCrop() const;

    QVBoxLayout* getSettingsLayout() const;
    QVBoxLayout* getViewLayout() const;
};