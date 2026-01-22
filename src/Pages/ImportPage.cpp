#include "ImportPage.h"
#include "SubtitleLabel.h"
#include "Algorithm.h"
#include <QSpinBox>

ImportPage::ImportPage(QWidget* parent) : QWidget(parent) {
    setObjectName("ImagePage");
    this->mainLayout = new QHBoxLayout(this);
    this->settingsLayout = new QVBoxLayout();
    this->imageViewer = new ImageViewer(this);
    this->mainLayout->addLayout(this->settingsLayout);
    this->mainLayout->addWidget(this->imageViewer);

    this->mainLayout->setStretch(0, 1);
    this->mainLayout->setStretch(1, 3);
    this->setupSliders();
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
                              this->sliders["Expand Edges"]->value()
                            );
    
    qDebug() << "edges empty?" << edges.empty();
    qDebug() << "edges size:" << edges.cols << edges.rows;
    qDebug() << "edges type:" << edges.type();
    double minVal = 0.0;
    double maxVal = 0.0;

    cv::minMaxLoc(edges, &minVal, &maxVal);

    qDebug() << "Min/Max:" << minVal << maxVal;

    
    QPixmap pix = Algorithm::matToPixmap(edges);
    this->imageViewer->setOutlines(pix);
}

void ImportPage::addSlider(std::string name, int default_value, int min_value, int max_value, std::string tooltip) {
    if (sliders.count(name) > 0) return;

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

    QSpinBox *spinbox = new QSpinBox(this);
    spinbox->setRange(min_value, max_value);
    spinbox->setValue(default_value);
    spinbox->setButtonSymbols(QAbstractSpinBox::NoButtons);

    // Connect Slider & Spinbox
    connect(slider, &QSlider::valueChanged, this, [spinbox](int val){
        spinbox->setValue(val);
    });
    connect(spinbox, &QSpinBox::valueChanged, this, [slider](int val){
        slider->setValue(val);
    });

    layout->addWidget(label);
    layout->addWidget(slider);
    layout->addWidget(spinbox);

    this->settingsLayout->addLayout(layout);

    this->sliders[name] = slider;
}

void ImportPage::loadImage(const QString& path) { this->imageViewer->loadImage(path); }

void ImportPage::setBaseImage(QGraphicsPixmapItem* image) { this->baseImage = image; }
QGraphicsPixmapItem* ImportPage::getBaseImage() const { return this->baseImage; }

void ImportPage::setCrop(QRectF crop) { this->crop = crop; }
QRectF ImportPage::getCrop() const { return this->crop; }