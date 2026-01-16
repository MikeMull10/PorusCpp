#pragma once

#include <QWidget>
#include <QHBoxLayout>
#include <QMessageBox>
#include "Tools.h"
#include "ImageViewer.h"
#include "ImageToolbar.h"

class ImagePopup : public QDialog {
    Q_OBJECT
    
    ImageToolbar* toolbar;
    ImageViewer* imageViewer;

public:
    explicit ImagePopup(const QString& path, QWidget* parent = nullptr);

    ImageViewer* getImageViewer() const;
};