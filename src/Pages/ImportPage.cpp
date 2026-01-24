#include "ImportPage.h"
#include "SubtitleLabel.h"
#include "Algorithm.h"
#include <QShortcut>

#include <QSpinBox>

ImportPage::ImportPage(QWidget* parent) : QWidget(parent) {
    setObjectName("ImagePage");
    this->mainLayout = new QHBoxLayout(this);
    this->settingsLayout = new QVBoxLayout();
    this->viewLayout = new QVBoxLayout();

    QHBoxLayout* toolbarLayout = new QHBoxLayout();
    this->viewLayout->addLayout(toolbarLayout);

    this->toolbar = new ImageToolbar({ TOOL::HAND, TOOL::ZOOM }, this);
    toolbarLayout->addWidget(this->toolbar);
    toolbarLayout->addStretch();

    // Show Background Button
    this->showBackgroundBtn = new ToolButton(QIcon(":/icons/background-white.svg"), this);
    showBackgroundBtn->setScale(1.5f);
    toolbarLayout->addWidget(this->showBackgroundBtn);
    connect(this->showBackgroundBtn, &ToolButton::clicked, this, [this]() { this->showBackgroundBtn->flipActive(); this->updateImage(); });
    QShortcut* keyB = new QShortcut(QKeySequence(Qt::Key_B), this);
    connect(keyB, &QShortcut::activated, this, [this]() { this->showBackgroundBtn->flipActive(); this->updateImage(); });

    // Outline Opacity
    QSlider* opacitySlider = new QSlider(Qt::Horizontal, this);
    opacitySlider->setRange(0, 100);
    opacitySlider->setValue(100);
    opacitySlider->setMaximumWidth(100);
    opacitySlider->setCursor(Qt::PointingHandCursor);
    opacitySlider->setToolTip(QString("Outlines Opacity: %1%").arg(opacitySlider->value()));
    connect(opacitySlider, &QSlider::valueChanged, this, [this, opacitySlider]() { this->updateImage(); opacitySlider->setToolTip(QString("Outlines Opacity: %1%").arg(opacitySlider->value())); });
    toolbarLayout->addWidget(opacitySlider);

    this->imageViewer = new ImageViewer(this->toolbar, this);
    this->viewLayout->addWidget(this->imageViewer);
    this->mainLayout->addLayout(this->settingsLayout);
    this->mainLayout->addLayout(this->viewLayout);
    this->mainLayout->setStretch(0, 1);
    this->mainLayout->setStretch(1, 3);
    this->setupSliders();

    this->sendBtn = new PushButton("Send to Editor");
    this->settingsLayout->addWidget(this->sendBtn);

    this->outlineColor = cv::Vec3b(0, 0, 255);
}

void ImportPage::setup() {
    if (!this->baseImage || crop.isEmpty()) return;

    QRectF pixmapBounds = baseImage->boundingRect();
    QRectF intersection = crop.intersected(pixmapBounds);
    
    if (intersection.isEmpty()) return; // No overlap
    
    // Extract the cropped region from the original pixmap
    QPixmap originalPixmap = baseImage->pixmap();
    QPixmap croppedPixmap = originalPixmap.copy(intersection.toRect());
    
    this->imageViewer->loadImage(croppedPixmap);
}

void ImportPage::setupSliders() {
    this->addSlider("Brightness",   0, -150, 150);
    this->addSlider("Contrast",     0, -100, 100);
    this->addSlider("Min Value",    0,    0, 255);
    this->addSlider("Max Value",    255,  0, 255);
    this->addSlider("Denoise",      0,    0, 10);
    this->addSlider("Blur",         1,    0, 10);
    this->addSlider("Canny Low",    50,   0, 255);
    this->addSlider("Canny High",   150,  0, 255);
    this->addSlider("Expand Edges", 0,    0, 2);

    for (const auto& [key, value] : this->sliders) {
        connect(value, &QSlider::valueChanged, this, [this]() { this->updateImage(); });
    }
}

void ImportPage::updateImage() {
    if (this->imageViewer->imageEmpty()) return;
    QPixmap img = this->imageViewer->getCurrentImage();

    cv::Mat matImg = Algorithm::pixmapToGrayMat(img);
    cv::Mat edges = Algorithm::canny(matImg,
                              this->sliders["Brightness"]->value(),
                              this->sliders["Contrast"]->value(),
                              this->sliders["Min Value"]->value(),
                              this->sliders["Max Value"]->value(),
                              this->sliders["Blur"]->value(),
                              this->sliders["Canny Low"]->value(),
                              this->sliders["Canny High"]->value(),
                              this->sliders["Denoise"]->value(),
                              this->sliders["Expand Edges"]->value(),
                              this->outlineColor,
                              this->showBackgroundBtn->property("active") == "true"
                            );
    
    QPixmap pix = Algorithm::matToPixmap(edges);
    this->imageViewer->setOutlines(pix);
}

void ImportPage::addSlider(std::string name, int default_value, int min_value, int max_value, std::string tooltip) {
    if (sliders.count(name) > 0) return;  // Check if slider already exists with the same name

    QHBoxLayout *layout = new QHBoxLayout();

    SubtitleLabel *label = new SubtitleLabel(QString::fromStdString(name));
    label->setMinimumWidth(90);
    label->setProperty("class", "SliderLabel");
    if (tooltip.length() > 0) label->setToolTip(QString::fromStdString(tooltip));

    QSlider *slider = new QSlider(Qt::Horizontal);
    slider->setRange(min_value, max_value);
    slider->setValue(default_value);
    slider->setMinimumWidth(150);
    slider->setCursor(Qt::PointingHandCursor);
    slider->setToolTip(QString("%1: %2").arg(QString::fromStdString(name)).arg(slider->value()));

    QSpinBox *spinbox = new QSpinBox(this);
    spinbox->setRange(min_value, max_value);
    spinbox->setValue(default_value);
    spinbox->setButtonSymbols(QAbstractSpinBox::NoButtons);

    // Connect Slider & Spinbox
    connect(slider, &QSlider::valueChanged, this, [spinbox](int val){
        spinbox->setValue(val);
    });
    connect(spinbox, &QSpinBox::valueChanged, this, [name, slider](int val){
        slider->setValue(val);
        slider->setToolTip(QString("%1: %2").arg(QString::fromStdString(name)).arg(val));
    });

    layout->addWidget(label);
    layout->addWidget(slider);
    layout->addWidget(spinbox);

    this->settingsLayout->addLayout(layout);

    this->sliders[name] = slider;
}

void ImportPage::loadImage(const QString& path) { this->imageViewer->loadImage(path); }

void ImportPage::setColor(QColor qcolor) {
    this->outlineColor[0] = qcolor.blue();   // B
    this->outlineColor[1] = qcolor.green();  // G
    this->outlineColor[2] = qcolor.red();    // R
}

void ImportPage::setBaseImage(QGraphicsPixmapItem* image) { this->baseImage = image; }
QGraphicsPixmapItem* ImportPage::getBaseImage() const { return this->baseImage; }

void ImportPage::setCrop(QRectF crop) { this->crop = crop; }
QRectF ImportPage::getCrop() const { return this->crop; }

QVBoxLayout* ImportPage::getSettingsLayout() const { return this->settingsLayout; }
QVBoxLayout* ImportPage::getViewLayout() const { return this->viewLayout; }