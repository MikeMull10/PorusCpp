#include "MainWindow.h"
#include "ToolButton.h"
#include "TitleLabel.h"
#include "Taskbar.h"

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
    QHBoxLayout* mainLayout = new QHBoxLayout(centralWidget);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);
    
    // Taskbar on the left
    Taskbar* taskBar = new Taskbar(this);
    mainLayout->addWidget(taskBar);

    // Main content area on the right
    QWidget* main = new QWidget(centralWidget);
    QVBoxLayout* layout = new QVBoxLayout(main);

    TitleLabel *title = new TitleLabel("This is a Title!", true, this);
    layout->addWidget(title);
    ToolButton *button = new ToolButton(style()->standardIcon(QStyle::SP_DialogCloseButton), this);
    layout->addWidget(button);
    
    mainLayout->addWidget(main);
    
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