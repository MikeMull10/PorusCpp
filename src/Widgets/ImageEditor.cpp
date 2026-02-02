#include "ImageEditor.h"
#include "ToolButton.h"
#include <QKeySequence>
#include <QShortcut>
#include <QTimer>

ImageEditor::ImageEditor(QWidget* parent) : QWidget(parent) {
    this->setObjectName("ImageEditor");

    this->scene = new QGraphicsScene(this);
    this->view = new GraphicsViewEdit(this->scene, this, &this->currentTool);
    this->view->setHistories(&this->undoStack, &this->redoStack);
    this->view->setMouseTracking(true);
    this->view->viewport()->setMouseTracking(true);
    this->view->setFocusPolicy(Qt::StrongFocus);
    this->view->viewport()->setFocusPolicy(Qt::StrongFocus);
    this->view->setFocus();

    this->view->setDragMode(QGraphicsView::NoDrag);
    this->view->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    this->view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    this->view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

    this->layout = new QVBoxLayout(this);
    this->setLayout(this->layout);
    this->toolbarLayout = new QHBoxLayout();
    this->layout->addLayout(toolbarLayout);
    this->layout->addWidget(this->view);
    this->initToolbar();

    QShortcut* resetZoomShortcut = new QShortcut(QKeySequence("Ctrl+0"), this);
    connect(resetZoomShortcut, &QShortcut::activated, this, &ImageEditor::resetZoom);
}

void ImageEditor::initToolbar() {
    // Hand Tool (Canvas)
    ToolButton *handBtn = new ToolButton(QIcon(":/icons/hand-white.svg"), this);
    handBtn->setToolTip("Hand Tool (H)");
    QShortcut *handShort = new QShortcut(QKeySequence(Qt::Key_H), this);
    connect(handShort, &QShortcut::activated, this, [this, handBtn](){ this->currentTool = ImageTools::HAND; this->setActiveTool(handBtn); });
    connect(handBtn, &ToolButton::clicked, this, [this, handBtn](){ this->currentTool = ImageTools::HAND; this->setActiveTool(handBtn); });
    toolBtns.push_back(handBtn);

    // Zoom Tool (Canvas)
    ToolButton *zoomBtn = new ToolButton(QIcon(":/icons/zoom-white.svg"), this);
    zoomBtn->setToolTip("Zoom Tool (Z)");
    QShortcut *zoomShort = new QShortcut(QKeySequence(Qt::Key_Z), this);
    connect(zoomShort, &QShortcut::activated, this, [this, zoomBtn](){ this->currentTool = ImageTools::ZOOM; this->setActiveTool(zoomBtn); });
    connect(zoomBtn, &ToolButton::clicked, this, [this, zoomBtn](){ this->currentTool = ImageTools::ZOOM; this->setActiveTool(zoomBtn); });
    toolBtns.push_back(zoomBtn);

    // Add Point
    ToolButton *addPntBtn = new ToolButton(QIcon(":/icons/add-point-white.svg"), this);
    addPntBtn->setToolTip("Add Point (A)");
    QShortcut *addPointShort = new QShortcut(QKeySequence(Qt::Key_A), this);
    connect(addPointShort, &QShortcut::activated, this, [this, addPntBtn](){ this->currentTool = ImageTools::ADD_POINT; this->setActiveTool(addPntBtn); });
    connect(addPntBtn, &ToolButton::clicked, this, [this, addPntBtn](){ this->currentTool = ImageTools::ADD_POINT; this->setActiveTool(addPntBtn); });
    toolBtns.push_back(addPntBtn);

    // Delete Point
    ToolButton *delPntBtn = new ToolButton(QIcon(":/icons/del-white.svg"), this);
    delPntBtn->setToolTip("Delete Point (D)");
    QShortcut *delPointShort = new QShortcut(QKeySequence(Qt::Key_D), this);
    connect(delPointShort, &QShortcut::activated, this, [this, delPntBtn](){ this->currentTool = ImageTools::ADD_POINT; this->setActiveTool(delPntBtn); });
    connect(delPntBtn, &ToolButton::clicked, this, [this, delPntBtn](){ this->currentTool = ImageTools::ADD_POINT; this->setActiveTool(delPntBtn); });
    toolBtns.push_back(delPntBtn);

    // Move Point
    ToolButton *movePntBtn = new ToolButton(QIcon(":/icons/move-point-white.svg"), this);
    movePntBtn->setToolTip("Move Point (G)");
    QShortcut *movePointShort = new QShortcut(QKeySequence(Qt::Key_G), this);
    connect(movePointShort, &QShortcut::activated, this, [this, movePntBtn](){ this->currentTool = ImageTools::MOVE_POINT; this->setActiveTool(movePntBtn); });
    connect(movePntBtn, &ToolButton::clicked, this, [this, movePntBtn](){ this->currentTool = ImageTools::MOVE_POINT; this->setActiveTool(movePntBtn); });
    toolBtns.push_back(movePntBtn);

    // Select Points
    ToolButton *selPntBtn = new ToolButton(QIcon(":/icons/select-single-white.svg"), this);
    selPntBtn->setToolTip("Select Point (V)");
    QShortcut *selPointShort = new QShortcut(QKeySequence(Qt::Key_V), this);
    connect(selPointShort, &QShortcut::activated, this, [this, selPntBtn](){ this->currentTool = ImageTools::SELECT_POINT; this->setActiveTool(selPntBtn); });
    connect(selPntBtn, &ToolButton::clicked, this, [this, selPntBtn](){ this->currentTool = ImageTools::SELECT_POINT; this->setActiveTool(selPntBtn); });
    toolBtns.push_back(selPntBtn);

    // Select Points using Rectangle Selection
    ToolButton *selRectBtn = new ToolButton(QIcon(":/icons/select-rect-white.svg"), this);
    selRectBtn->setToolTip("Rectangle Selection (R)");
    QShortcut *selRectShort = new QShortcut(QKeySequence(Qt::Key_R), this);
    connect(selRectShort, &QShortcut::activated, this, [this, selRectBtn](){ this->currentTool = ImageTools::SELECT_RECT; this->setActiveTool(selRectBtn); });
    connect(selRectBtn, &ToolButton::clicked, this, [this, selRectBtn](){ this->currentTool = ImageTools::SELECT_RECT; this->setActiveTool(selRectBtn); });
    toolBtns.push_back(selRectBtn);

    // Select Points using Lasso Selection
    ToolButton *selLassoBtn = new ToolButton(QIcon(":/icons/select-lasso-white.svg"), this);
    selLassoBtn->setToolTip("Lasso Selection (L)");
    QShortcut *selLassoShort = new QShortcut(QKeySequence(Qt::Key_L), this);
    connect(selLassoShort, &QShortcut::activated, this, [this, selLassoBtn](){ this->currentTool = ImageTools::SELECT_LASSO; this->setActiveTool(selLassoBtn); });
    connect(selLassoBtn, &ToolButton::clicked, this, [this, selLassoBtn](){ this->currentTool = ImageTools::SELECT_LASSO; this->setActiveTool(selLassoBtn); });
    toolBtns.push_back(selLassoBtn);

    // Combine Points
    ToolButton *combPntBtn = new ToolButton(QIcon(":/icons/merge-point-white.svg"), this);
    combPntBtn->setToolTip("Merge Point(s) (C)");
    QShortcut *combPointShort = new QShortcut(QKeySequence(Qt::Key_C), this);
    connect(combPointShort, &QShortcut::activated, this, [this, combPntBtn](){ this->currentTool = ImageTools::COMBINE_POINTS; this->setActiveTool(combPntBtn); });
    connect(combPntBtn, &ToolButton::clicked, this, [this, combPntBtn](){ this->currentTool = ImageTools::COMBINE_POINTS; this->setActiveTool(combPntBtn); });
    toolBtns.push_back(combPntBtn);

    // Add Points
    ToolButton *addFollowBtn = new ToolButton(QIcon(":/icons/add-points-follow-white.svg"), this);
    addFollowBtn->setToolTip("Add (Follow Cursor) (F)");
    QShortcut *addFollowShort = new QShortcut(QKeySequence(Qt::Key_F), this);
    connect(addFollowShort, &QShortcut::activated, this, [this, addFollowBtn](){ this->currentTool = ImageTools::ADD_POINTS_FOLLOW; this->setActiveTool(addFollowBtn); });
    connect(addFollowBtn, &ToolButton::clicked, this, [this, addFollowBtn](){ this->currentTool = ImageTools::ADD_POINTS_FOLLOW; this->setActiveTool(addFollowBtn); });
    toolBtns.push_back(addFollowBtn);

    // Delete Polygon
    ToolButton *delPolyBtn = new ToolButton(QIcon(":/icons/delete-polygon-white.svg"), this);
    delPolyBtn->setToolTip("Delete Polygon (T)");
    QShortcut *delPolyShort = new QShortcut(QKeySequence(Qt::Key_T), this);
    connect(delPolyShort, &QShortcut::activated, this, [this, delPolyBtn](){ this->currentTool = ImageTools::DELETE_POLYGON; this->setActiveTool(delPolyBtn); });
    connect(delPolyBtn, &ToolButton::clicked, this, [this, delPolyBtn](){ this->currentTool = ImageTools::DELETE_POLYGON; this->setActiveTool(delPolyBtn); });
    toolBtns.push_back(delPolyBtn);

    // Undo / Redo
    ToolButton *undoBtn = new ToolButton(QIcon(":/icons/undo-white.svg"), this);
    ToolButton *redoBtn = new ToolButton(QIcon(":/icons/redo-white.svg"), this);
    undoBtn->setToolTip("Undo Previous Action (Ctrl+Z)");
    redoBtn->setToolTip("Redo Previous Action (Ctrl+Y)");
    QShortcut *undoShort = new QShortcut(QKeySequence("Ctrl+Z"), this);
    QShortcut *redoShort = new QShortcut(QKeySequence("Ctrl+Y"), this);
    connect(undoShort, &QShortcut::activated, this, &ImageEditor::undo);
    connect(redoShort, &QShortcut::activated, this, &ImageEditor::redo);
    connect(undoBtn, &ToolButton::clicked, this, &ImageEditor::undo);
    connect(redoBtn, &ToolButton::clicked, this, &ImageEditor::redo);

    for (ToolButton *btn : toolBtns) { this->toolbarLayout->addWidget(btn); btn->setScale(1.25f); }
    this->toolbarLayout->addWidget(undoBtn);
    this->toolbarLayout->addWidget(redoBtn);
    undoBtn->setScale(1.25f);
    redoBtn->setScale(1.25f);
}

void ImageEditor::setActiveTool(ToolButton *active) {
    for (ToolButton *btn : this->toolBtns) btn->setActive(false);
    active->setActive(true);

    this->view->updateCursor();
}

void ImageEditor::loadImage(const QPixmap &pixmap) {
    while (!undoStack.empty()) undoStack.pop();
    while (!redoStack.empty()) redoStack.pop();
    this->view->clearSelection();

    if (this->image) {
        this->scene->removeItem(this->image);
        delete this->image;
        this->image = nullptr;
    }

    this->image = this->scene->addPixmap(pixmap);
    this->image->setZValue(0);

    QGraphicsPixmapItem* imagePtr(this->image);
    QTimer::singleShot(1, this, [this, imagePtr]() {
        if (imagePtr && this->view) {  // Check if still valid
            this->view->fitInView(imagePtr, Qt::KeepAspectRatio);
        }
    });
}

void ImageEditor::loadContours(std::vector<std::vector<cv::Point>> contours) {
    // Clear existing polygons first
    for (Polygon* poly : this->polygons) {
        this->scene->removeItem(poly);
        delete poly;
    }
    this->polygons.clear();
    
    // Now add new polygons
    for (const std::vector<cv::Point> &contour : contours) {
        if (contour.size() < 3) continue;
        
        Polygon* poly = new Polygon(this->scene);
        
        // Build QPolygonF and set all points at once
        QPolygonF qPoly;
        qPoly.reserve(contour.size());
        
        for (const cv::Point &pt : contour) {
            qPoly << QPointF(pt.x, pt.y);
        }
        
        poly->setPoints(qPoly);

        if (poly->getNumPoints() < 3 || poly->getArea() <= 1.0f) continue;
        
        this->polygons.push_back(poly);
        this->scene->addItem(poly);
    }
}

void ImageEditor::undo() {
    if (undoStack.empty()) return;

    History* action = undoStack.top();
    undoStack.pop();
    action->doAction();
    redoStack.push(action->getInverse());
}

void ImageEditor::redo() {
    if (redoStack.empty()) return;

    History* action = redoStack.top();
    redoStack.pop();
    action->doAction();
    undoStack.push(action->getInverse());
}

void ImageEditor::resetZoom() {
    if (!this->image) return;
    this->view->fitInView(this->image, Qt::KeepAspectRatio);
}