// CustomGraphicsView.h
#pragma once
#include <QGraphicsView>
#include <QWheelEvent>

class GraphicsView : public QGraphicsView {
    Q_OBJECT
    
public:
    explicit GraphicsView(QGraphicsScene* scene, QWidget* parent = nullptr);
    
protected:
    void wheelEvent(QWheelEvent* event) override;
};