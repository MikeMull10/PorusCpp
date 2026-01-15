#include "MainWindow.h"
#include "ToolButton.h"
#include "TitleLabel.h"
#include "Taskbar.h"
#include "ImagePopup.h"

#include <QShortcut>
#include <QVBoxLayout>
#include <QIcon>
#include <QStyle>
#include <QFile>
#include <QApplication>
#include <QFileDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setWindowTitle("PORUS X");
    resize(800, 600);
    showMaximized();

    // Settings
    this->settings = new QSettings("Porus", "PorusApp", this);
    this->initSettings();

    QWidget* centralWidget = new QWidget(this);
    this->mainLayout = new QHBoxLayout(centralWidget);
    this->mainLayout->setContentsMargins(0, 0, 0, 0);
    this->mainLayout->setSpacing(0);
    
    // Taskbar on the left
    Taskbar* taskBar = new Taskbar(this);
    this->mainLayout->addWidget(taskBar);

    TaskButton* openFileBtn = new TaskButton(QIcon(":/icons/open-file-white.svg"), "Open File", taskBar);
    taskBar->addTaskButton(openFileBtn);
    connect(openFileBtn, &TaskButton::clicked, this, [this]() { this->openFile(); });

    // Main content area on the right
    this->main = new QStackedWidget(centralWidget);
    this->importPage = new ImportPage(this);
    this->main->addWidget(this->importPage);
    this->main->setCurrentWidget(this->importPage);
    
    this->mainLayout->addWidget(this->main);
    
    setCentralWidget(centralWidget);
    this->updateStylesheet();

    // --- Keybinds ---
    QShortcut* quitShortcut = new QShortcut(QKeySequence("Ctrl+Q"), this);
    quitShortcut->setContext(Qt::ApplicationShortcut);
    connect(quitShortcut, &QShortcut::activated, this, &MainWindow::close);
    QShortcut* openShortcut = new QShortcut(QKeySequence("Ctrl+O"), this);
    connect(openShortcut, &QShortcut::activated, this, &MainWindow::openFile);
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

void MainWindow::openFile() {
    QString fileName = QFileDialog::getOpenFileName(
        this, "Open Image", "", "Images (*.png *.jpg *.jpeg *.bmp)" 
    );

    if (fileName.isEmpty()) return;

    switch (this->main->currentIndex()) {
        case 0:  // Import Page
            try {
                ImagePopup* pop = new ImagePopup(fileName, this);
                if (pop->exec() == QDialog::Accepted) {}
                delete pop;
            } catch (const std::exception& e) {
                QMessageBox::critical(this, "Error", QString("Failed to load image: %1").arg(e.what()));
            } catch (...) {
                QMessageBox::critical(this, "Error", 
                    "An unknown error occurred while loading the image");
            }
            break;
    }
}