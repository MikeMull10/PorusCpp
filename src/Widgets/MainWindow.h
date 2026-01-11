#pragma once

#include <QApplication>
#include <QMainWindow>
#include <QSettings>

class MainWindow : public QMainWindow {
    Q_OBJECT
    QSettings* settings;

public:
    explicit MainWindow(QWidget *parent = nullptr);

    void updateStylesheet();
    void initSettings();

protected:
    void closeEvent(QCloseEvent* event);
};
