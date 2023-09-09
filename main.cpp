#include "Dashboard.h"

#include <QApplication>
#include <QString>

int main(int argc, char *argv[])
{
    QApplication application(argc, argv);

    // For now we default to loading the file 'Dashboard.ldash' in the working
    // directory, but at some point this should change to a filename coming
    // from arguements and the user should be able to double click a .ldash
    // file to start running with that dashboard.
    const QString configFileName = "Dashboard.ldash";
    Dashboard dashboard(configFileName);
    dashboard.show();

    return application.exec();
}
