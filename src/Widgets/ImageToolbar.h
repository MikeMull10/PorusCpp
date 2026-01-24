#pragma once

#include <QWidget>
#include <QHBoxLayout>
#include "Tools.h"
#include "ImageViewer.h"
#include "ToolButton.h"

class ImageToolbar : public QWidget {
    Q_OBJECT

    QHBoxLayout* layout;

    ToolButton* handBtn{ nullptr };
    ToolButton* zoomBtn{ nullptr };
    ToolButton* cropBtn{ nullptr };
    ToolButton* scaleBtn{ nullptr };
    ToolButton* resetZoomBtn{ nullptr };

    ToolButton* active{ nullptr };

public:
    ImageToolbar(QWidget* parent);
    ImageToolbar(const QVector<TOOL>& enabled, QWidget* parent);

    void handleClick(ToolButton* btn);

    ToolButton* getBtn(const QString& toolType);

    ToolButton* getActive();
    TOOL getActiveType() const;

signals:
    void toolActivated(TOOL tool);
};