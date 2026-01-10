#include "MainWindow.h"
#include "ToolButton.h"
#include "TitleLabel.h"
#include <QVBoxLayout>
#include <QIcon>
#include <QStyle>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setWindowTitle("Qt 6 C++ App");
    resize(800, 600);
    showMaximized();

    QWidget* centralWidget = new QWidget(this);
    QVBoxLayout* layout = new QVBoxLayout(centralWidget);

    TitleLabel *title = new TitleLabel("This is a Title!", false);
    layout->addWidget(title);
    ToolButton *button = new ToolButton(style()->standardIcon(QStyle::SP_DialogCloseButton), this);
    layout->addWidget(button);
    
    setCentralWidget(centralWidget);
}