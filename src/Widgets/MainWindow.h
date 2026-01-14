#pragma once

#include "ImportPage.h"

#include <QApplication>
#include <QMainWindow>
#include <QSettings>
#include <QHBoxLayout>
#include <QStackedWidget>

class MainWindow : public QMainWindow {
    Q_OBJECT
    QSettings* settings;

    QHBoxLayout* mainLayout{ nullptr };
    QStackedWidget* main{ nullptr };
    ImportPage* importPage{ nullptr };

public:
    explicit MainWindow(QWidget *parent = nullptr);

    void updateStylesheet();
    void initSettings();

    void openFile();

protected:
    void closeEvent(QCloseEvent* event);
};
