#pragma once

#include <QWidget>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QSettings>
#include <QDialog>

class SettingsPopup : public QDialog {
    Q_OBJECT

public:
    explicit SettingsPopup(QSettings* settings, QWidget* parent);

signals:
    void reloadQSS();
};