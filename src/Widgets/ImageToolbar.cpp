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
    
    for (TOOL tool : enabled) {
        if (tool == TOOL::HAND) {
            this->layout->addWidget(this->handBtn);
            QShortcut* shortcut = new QShortcut(QKeySequence(Qt::Key_H), this);
            connect(shortcut, &QShortcut::activated, this, [this]() { handleClick(this->handBtn); });
        } else if (tool == TOOL::ZOOM) {
            this->layout->addWidget(this->zoomBtn);
            QShortcut* shortcut = new QShortcut(QKeySequence(Qt::Key_Z), this);
            connect(shortcut, &QShortcut::activated, this, [this]() { handleClick(this->zoomBtn); });
        } else if (tool == TOOL::CROP) {
            this->layout->addWidget(this->cropBtn);
            QShortcut* shortcut = new QShortcut(QKeySequence(Qt::Key_C), this);
            connect(shortcut, &QShortcut::activated, this, [this]() { handleClick(this->cropBtn); });
        } else if (tool == TOOL::SCALE) {
            this->layout->addWidget(this->scaleBtn);
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