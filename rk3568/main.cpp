#include "rk3568controlsystem.h"
#include "loginwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //RK3568ControlSystem w;
    LoginWindow w;
    w.show();
    return a.exec();
}
