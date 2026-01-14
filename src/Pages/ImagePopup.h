#pragma once

#include <QWidget>
#include <QHBoxLayout>
#include <QMessageBox>
#include "ImageViewer.h"

class ImagePopup : public QDialog {
    Q_OBJECT

    ImageViewer *imageViewer;

public:
    explicit ImagePopup(const QString& path, QWidget* parent = nullptr);
};