#include "MainWindow.h"
#include "ToolButton.h"
#include "TitleLabel.h"
#include <QVBoxLayout>
#include <QIcon>
#include <QStyle>
#include <QFile>
#include <QApplication>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setWindowTitle("PORUS X");
    resize(800, 600);
    showMaximized();

    // Settings
    this->settings = new QSettings("Porus", "PorusApp", this);
    this->initSettings();

    QWidget* centralWidget = new QWidget(this);
    QVBoxLayout* layout = new QVBoxLayout(centralWidget);

    TitleLabel *title = new TitleLabel("This is a Title!", true, this);
    layout->addWidget(title);
    ToolButton *button = new ToolButton(style()->standardIcon(QStyle::SP_DialogCloseButton), this);
    layout->addWidget(button);
    
    setCentralWidget(centralWidget);
    this->updateStylesheet();
}

void MainWindow::updateStylesheet() {
    QFile file(":/theme.qss");
    if (file.open(QIODevice::ReadOnly)) {
        QString stylesheet = file.readAll();
        stylesheet.replace("--primary", this->settings->value("primary").toString());
        qApp->setStyleSheet(stylesheet);
    }
}

void MainWindow::initSettings() {
    QString theme = this->settings->value("theme", "light").toString();
    this->settings->setValue("theme", theme);

    QString primary = this->settings->value("primary", "#ff0000").toString();
    this->settings->setValue("primary", primary);
}

void MainWindow::closeEvent(QCloseEvent* event) {
    // saveSettings();
    QMainWindow::closeEvent(event);
}