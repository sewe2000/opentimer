#include "../include/mainwindow.hpp"

#include <QApplication>
#include <QFontDatabase>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QFontDatabase::addApplicationFont(QString(":/fonts/assets/fonts/static/RobotoMono-Bold.ttf"));
    QFontDatabase::addApplicationFont(QString(":/fonts/assets/fonts/static/RobotoMono-Medium.ttf"));
    MainWindow w;

    w.show();
    w.first_session();

    return a.exec();
}
