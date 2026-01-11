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
    mainWindow.show();

    app.setStyleSheet("#TitleLabel[active=\"true\"] { color: red; } #TitleLabel[active=\"false\"] { color: blue; }");

    return app.exec();
}
