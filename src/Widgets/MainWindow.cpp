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
#include <QFileInfo>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setWindowTitle("PORUS X (alpha)");
    resize(800, 600);
    showMaximized();

    // Settings
    this->settings = new QSettings("Porus", "PorusApp", this);
    this->initSettings();
    this->settingsPopup = new SettingsPopup(this->settings, this);
    connect(this->settingsPopup, &SettingsPopup::reloadQSS, this, [this]() {
        this->updateStylesheet();
        this->importPage->setColor(QColor(this->settings->value("outlineColor", "#ff0000").toString()));
    });

    QWidget* centralWidget = new QWidget(this);
    this->mainLayout = new QHBoxLayout(centralWidget);
    this->mainLayout->setContentsMargins(0, 0, 0, 0);
    this->mainLayout->setSpacing(0);
    
    // Taskbar on the left
    Taskbar* taskBar = new Taskbar(this);
    this->mainLayout->addWidget(taskBar);

    TaskButton* openFileBtn = new TaskButton(QIcon(":/icons/open-file-white.svg"), "Open File", taskBar);
    taskBar->addTaskButton(openFileBtn);
    connect(openFileBtn, &TaskButton::clicked, this, &MainWindow::openFile);
    connect(taskBar->getSettingsBtn(), &TaskButton::clicked, this, &MainWindow::createSettingsPopup);

    // Main content area on the right
    this->main = new QStackedWidget(centralWidget);
    this->importPage = new ImportPage(this);
    this->editPage = new EditPage(this);
    this->main->addWidget(this->importPage);
    this->main->addWidget(this->editPage);
    this->main->setCurrentWidget(this->importPage);
    this->importPage->setColor(QColor(this->settings->value("outlineColor", "#ff0000").toString()));
    
    this->mainLayout->addWidget(this->main);
    
    setCentralWidget(centralWidget);
    this->updateStylesheet();

    connect(this->importPage->getSendBtn(), &PushButton::clicked, this, [this]() {
        QPixmap pix = this->importPage->getCroppedPixmap();
        std::vector<std::vector<cv::Point>> points = this->importPage->getCannyData();
        this->editPage->load(pix, points);
        this->main->setCurrentIndex(1);
    });

    // Page Buttons & Seperator
    QFrame* seperator = new QFrame(this);
    seperator->setFrameShape(QFrame::HLine);
    seperator->setFrameShadow(QFrame::Sunken);
    taskBar->addWidget(seperator);

    TaskButton* importPageBtn = new TaskButton(QIcon(":/icons/import-white.svg"), "Import Page", taskBar);
    taskBar->addTaskButton(importPageBtn);
    connect(importPageBtn, &TaskButton::clicked, this, [this]() { this->main->setCurrentIndex(0); });

    TaskButton* editPageBtn = new TaskButton(QIcon(":/icons/import-white.svg"), "Edit Page", taskBar);
    taskBar->addTaskButton(editPageBtn);
    connect(editPageBtn, &TaskButton::clicked, this, [this]() { this->main->setCurrentIndex(1); });

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

        // Primary Dimmed
        QColor color(this->settings->value("primary").toString());
        QString darker(color.darker(120).name());
        stylesheet.replace("--darkened", darker);
        qApp->setStyleSheet(stylesheet);
    }
}

void MainWindow::initSettings() {
    QString theme = this->settings->value("theme", "light").toString();
    this->settings->setValue("theme", theme);

    QString primary = this->settings->value("primary", "#ff0000").toString();
    this->settings->setValue("primary", primary);

    QString recentDir = this->settings->value("recentDir", "").toString();
    this->settings->setValue("recentDir", recentDir);

    QString outlineColor = this->settings->value("outlineColor", "#ff0000").toString();
    this->settings->setValue("outlineColor", outlineColor);
}

void MainWindow::closeEvent(QCloseEvent* event) {
    QMainWindow::closeEvent(event);
}

void MainWindow::openFile() {
    QString fileName = QFileDialog::getOpenFileName(
        this, "Open Image", this->settings->value("recentDir").toString(), "Images (*.png *.jpg *.jpeg *.bmp)" 
    );

    if (fileName.isEmpty()) return;

    this->settings->setValue("recentDir", QFileInfo(fileName).absolutePath());

    switch (this->main->currentIndex()) {
        case 0:  // Import Page
            try {
                ImagePopup* pop = new ImagePopup(fileName, this);
                if (pop->exec() == QDialog::Accepted) {
                    this->importPage->setBaseImage(pop->getImageViewer()->getImage());
                    this->importPage->setCrop(pop->getImageViewer()->getCrop());
                    this->importPage->setup();
                    this->importPage->updateImage();
                }
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

void MainWindow::createSettingsPopup() { this->settingsPopup->exec(); }