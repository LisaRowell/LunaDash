#include "Dashboard.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication application(argc, argv);
    Dashboard mainWindow;
    mainWindow.show();
    return application.exec();
}
