#include "lorekeepermainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    LoreKeeperMainWindow w;
    w.show();
    return a.exec();
}
