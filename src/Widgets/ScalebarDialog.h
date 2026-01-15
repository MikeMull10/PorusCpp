    #pragma once

    #include <QDialog>
    #include <QVBoxLayout>
    #include <QHBoxLayout>
    #include <QLabel>
    #include <QDoubleValidator>
    #include <QLineEdit>
    #include "PushButton.h"

    struct SCALE {
        float pixels{ 0 };
        float value{ 0 };
    };

    class ScalebarDialog : public QDialog {
        Q_OBJECT

    public:
        explicit ScalebarDialog(QWidget* parent);
        explicit ScalebarDialog(float pixels, QWidget* parent);

        SCALE getScale() const;

    private:
        SCALE scaleResult;

        QLineEdit* pixelsInput;
        QLineEdit* valueInput;
        PushButton* okButton;
        PushButton* cancelButton;

        void setupUI(float pixels);
    };