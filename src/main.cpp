#include <QApplication>
#include <QToolBar>
#include <QAction>
#include <QLabel>
#include <QStatusBar>
#include <QTextEdit>
#include <QDockWidget>
#include "Widgets/MainWindow.h"
#include "Widgets/PushButton.h"

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    MainWindow mainWindow;
    // mainWindow.setWindowTitle("PorusCpp");

    // // Central widget
    // auto *textEdit = new QTextEdit();
    // mainWindow.setCentralWidget(textEdit);

    // // Taskbar / Toolbar
    // auto *toolbar = new QToolBar("Taskbar");
    // mainWindow.addToolBar(Qt::TopToolBarArea, toolbar);

    // QAction *openAction = new QAction("Open", &mainWindow);
    // QAction *saveAction = new QAction("Save", &mainWindow);
    // toolbar->addAction(openAction);
    // toolbar->addAction(saveAction);

    // // Status bar
    // mainWindow.statusBar()->showMessage("Ready");

    // // Dockable panel (like a sidebar)
    // auto *dock = new QDockWidget("Properties", &mainWindow);
    // auto *dockContent = new QLabel("Dockable content here");
    // dock->setWidget(dockContent);
    // mainWindow.addDockWidget(Qt::RightDockWidgetArea, dock);

    // mainWindow.resize(800, 600);
    mainWindow.show();

    return app.exec();
}
