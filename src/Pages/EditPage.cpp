#include "EditPage.h"

EditPage::EditPage(QWidget* parent) : QWidget(parent) {
    this->setObjectName("EditPage");

    this->mainLayout = new QHBoxLayout(this);
    this->setLayout(this->mainLayout);

    this->imageEditor = new ImageEditor(this);
    this->mainLayout->addWidget(this->imageEditor);
}

void EditPage::load(const QPixmap &pixmap, std::vector<std::vector<cv::Point>> contours) {
    this->imageEditor->loadImage(pixmap);
    this->imageEditor->loadContours(contours);
}