#pragma once

#include "ImageEditor.h"

#include <opencv2/core.hpp>
#include <QWidget>
#include <QHBoxLayout>
#include <QSlider>

class EditPage : public QWidget {
    Q_OBJECT

    QHBoxLayout *mainLayout;
    ImageEditor *imageEditor;

public:
    explicit EditPage(QWidget* parent = nullptr);

    void load(const QPixmap &pixmap, std::vector<std::vector<cv::Point>> contours);
};