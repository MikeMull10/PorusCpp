#include "ImportPage.h"

ImportPage::ImportPage(QWidget* parent) : QWidget(parent) {
    setObjectName("ImagePage");
    this->mainLayout = new QHBoxLayout(this);
    this->imageViewer = new ImageViewer(this);
    this->mainLayout->addWidget(this->imageViewer);
}

void ImportPage::loadImage(const QString& path) { this->imageViewer->loadImage(path); }