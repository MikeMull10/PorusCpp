#include "ScalebarDialog.h"

ScalebarDialog::ScalebarDialog(float pixels, QWidget* parent) : QDialog(parent) {
    this->setObjectName("ScalebarDialog");
    this->setupUI(pixels);
}

ScalebarDialog::ScalebarDialog(QWidget* parent) : ScalebarDialog(0.0, parent) {}
    
void ScalebarDialog::setupUI(float pixels) {
    setWindowTitle("Set Scale Bar");
    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    // Pixels input
    QHBoxLayout* pixelsLayout = new QHBoxLayout();
    QLabel* pixelsLabel = new QLabel("Pixels:", this);
    pixelsInput = new QLineEdit(this);
    pixelsInput->setValidator(new QDoubleValidator(0, 10e12, 5, this));
    pixelsInput->setText(QString::number(pixels, 'f', 5));
    pixelsLayout->addWidget(pixelsLabel);
    pixelsLayout->addWidget(pixelsInput);
    mainLayout->addLayout(pixelsLayout);

    // Value input
    QHBoxLayout* valueLayout = new QHBoxLayout();
    QLabel* valueLabel = new QLabel("Value:", this);
    valueInput = new QLineEdit(this);
    valueInput->setValidator(new QDoubleValidator(0, 10e12, 5, this));
    valueLayout->addWidget(valueLabel);
    valueLayout->addWidget(valueInput);
    mainLayout->addLayout(valueLayout);

    // Buttons
    QHBoxLayout* buttonsLayout = new QHBoxLayout();
    okButton = new PushButton("OK", this);
    cancelButton = new PushButton("Cancel", this);
    buttonsLayout->addWidget(okButton);
    buttonsLayout->addWidget(cancelButton);
    mainLayout->addLayout(buttonsLayout);

    connect(okButton, &PushButton::clicked, this, [this]() {
        scaleResult.pixels = pixelsInput->text().toFloat();
        scaleResult.value = valueInput->text().toFloat();
        accept();
    });

    connect(cancelButton, &PushButton::clicked, this, &QDialog::reject);
}

SCALE ScalebarDialog::getScale() const { return this->scaleResult; }
