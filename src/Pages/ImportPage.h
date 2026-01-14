#pragma once

#include <QWidget>
#include <QHBoxLayout>
#include "ImageViewer.h"

class ImportPage : public QWidget {
    Q_OBJECT

    QHBoxLayout* mainLayout;
    ImageViewer* imageViewer;

public:
    explicit ImportPage(QWidget* parent = nullptr);

    void loadImage(const QString& path);
};