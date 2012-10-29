#include "window.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    Window window;

    QCoreApplication::setOrganizationName("Nizhibitsky Eugene");
    QCoreApplication::setApplicationName("MMP Board");

    window.show();
    return app.exec();
}
