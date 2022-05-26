#include "mainwindow.h"

#include <QApplication>
#include <QFontDatabase>

#include <libusb-1.0/libusb.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QFontDatabase::addApplicationFont(QString(":/fonts/assets/fonts/static/RobotoMono-Bold.ttf"));
    QFontDatabase::addApplicationFont(QString(":/fonts/assets/fonts/static/RobotoMono-Medium.ttf"));
    MainWindow w;

    w.show();
    w.first_session();

    if(libusb_init(NULL) != 0)
        w.display_error_msgbox("Couldn't initialize libusb for stackmat support!");


    return a.exec();
}
