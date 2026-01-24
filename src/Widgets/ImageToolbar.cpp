#include "ImageToolbar.h"
#include <QShortcut>

#include <unordered_map>

std::unordered_map<std::string, TOOL> toolTypes = {
    {"HAND", TOOL::HAND},
    {"ZOOM", TOOL::ZOOM},
    {"CROP", TOOL::CROP},
    {"SCALE", TOOL::SCALE}
};

ImageToolbar::ImageToolbar(QWidget* parent) : QWidget(parent) {
    setObjectName("ImageToolbar");
    this->layout = new QHBoxLayout(this);
}

ImageToolbar::ImageToolbar(const QVector<TOOL>& enabled, QWidget* parent) : QWidget(parent) {
    setObjectName("ImageToolbar");
    this->layout = new QHBoxLayout(this);

    this->handBtn = new ToolButton(QIcon(":/icons/hand-white.svg"), this);
    this->handBtn->setProperty("tool", "HAND");
    this->handBtn->setToolTip("Hand Tool (H)");
    this->zoomBtn = new ToolButton(QIcon(":/icons/zoom-white.svg"), this);
    this->zoomBtn->setProperty("tool", "ZOOM");
    this->zoomBtn->setToolTip("Zoom (Z)");
    this->cropBtn = new ToolButton(QIcon(":/icons/crop-white.svg"), this);
    this->cropBtn->setProperty("tool", "CROP");
    this->cropBtn->setToolTip("Crop (C)");
    this->scaleBtn = new ToolButton(QIcon(":/icons/ruler-white.svg"), this);
    this->scaleBtn->setProperty("tool", "SCALE");
    this->scaleBtn->setToolTip("Set Scale (R)");
    this->resetZoomBtn = new ToolButton(QIcon(":/icons/reset-zoom-white.svg"), this);
    this->resetZoomBtn->setProperty("tool", "RESET_ZOOM");
    this->resetZoomBtn->setToolTip("Reset Zoom (Ctrl+0)");

    this->handBtn->hide(); this->handBtn->setScale(1.5f);
    this->zoomBtn->hide(); this->zoomBtn->setScale(1.5f);
    this->cropBtn->hide(); this->cropBtn->setScale(1.5f);
    this->scaleBtn->hide(); this->scaleBtn->setScale(1.5f);
    this->resetZoomBtn->hide(); this->resetZoomBtn->setScale(1.5f);
    
    for (TOOL tool : enabled) {
        if (tool == TOOL::HAND) {
            this->layout->addWidget(this->handBtn);
            this->handBtn->show();
            QShortcut* shortcut = new QShortcut(QKeySequence(Qt::Key_H), this);
            connect(shortcut, &QShortcut::activated, this, [this]() { handleClick(this->handBtn); });
        } else if (tool == TOOL::ZOOM) {
            this->layout->addWidget(this->zoomBtn);
            this->zoomBtn->show();
            QShortcut* shortcut = new QShortcut(QKeySequence(Qt::Key_Z), this);
            connect(shortcut, &QShortcut::activated, this, [this]() { handleClick(this->zoomBtn); });
            this->layout->addWidget(this->resetZoomBtn);
            this->resetZoomBtn->show();
        } else if (tool == TOOL::CROP) {
            this->layout->addWidget(this->cropBtn);
            this->cropBtn->show();
            QShortcut* shortcut = new QShortcut(QKeySequence(Qt::Key_C), this);
            connect(shortcut, &QShortcut::activated, this, [this]() { handleClick(this->cropBtn); });
        } else if (tool == TOOL::SCALE) {
            this->layout->addWidget(this->scaleBtn);
            this->scaleBtn->show();
            QShortcut* shortcut = new QShortcut(QKeySequence(Qt::Key_R), this);
            connect(shortcut, &QShortcut::activated, this, [this]() { handleClick(this->scaleBtn); });
        }
    }
    this->layout->addStretch();

    connect(this->handBtn, &ToolButton::clicked, this, [this]() { handleClick(this->handBtn); });
    connect(this->zoomBtn, &ToolButton::clicked, this, [this]() { handleClick(this->zoomBtn); });
    connect(this->cropBtn, &ToolButton::clicked, this, [this]() { handleClick(this->cropBtn); });
    connect(this->scaleBtn, &ToolButton::clicked, this, [this]() { handleClick(this->scaleBtn); });
}

void ImageToolbar::handleClick(ToolButton* btn) {
    this->handBtn->setActive(false);
    this->zoomBtn->setActive(false);
    this->cropBtn->setActive(false);
    this->scaleBtn->setActive(false);
    btn->setActive(true);
    this->active = btn;

    TOOL current = getActiveType();
    emit toolActivated(current);
}

ToolButton* ImageToolbar::getBtn(const QString& toolType) {
    if (toolType == "HAND") return this->handBtn;
    else if (toolType == "ZOOM") return this->zoomBtn;
    else if (toolType == "CROP") return this->cropBtn;
    else if (toolType == "SCALE") return this->scaleBtn;
    else if (toolType == "RESET_ZOOM") return this->resetZoomBtn;
    return nullptr;
}

ToolButton* ImageToolbar::getActive() { return this->active; }

TOOL ImageToolbar::getActiveType() const {
    if (!this->active) return TOOL::NONE;

    std::string key = this->active->property("tool").toString().toStdString();

    auto it = toolTypes.find(key);
    if (it == toolTypes.end())
        return TOOL::NONE;

    return it->second;
}