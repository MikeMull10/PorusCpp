#pragma once

#include <QGraphicsItem>
#include <QPainter>

class CropDimOverlay : public QGraphicsItem {
public:
    QRectF imageRect;
    QRectF cropRect;

    CropDimOverlay(const QRectF& imageRect) : imageRect(imageRect) {
        setZValue(1);
        setAcceptedMouseButtons(Qt::NoButton);
    }

    void setCropRect(const QRectF& rect) {
        cropRect = rect;
        prepareGeometryChange();
        update();
    }

    QRectF boundingRect() const override { return imageRect; }

    void paint(QPainter* p, const QStyleOptionGraphicsItem*, QWidget*) override {
        QPainterPath overlay;
        overlay.addRect(imageRect);

        QPainterPath hole;
        hole.addRect(cropRect);

        QPainterPath dimmed = overlay.subtracted(hole);

        p->setPen(Qt::NoPen);
        p->setBrush(QColor(0, 0, 0, 120));
        p->drawPath(dimmed);
    }

    QRectF getCropRect() { return this->cropRect; }
};
