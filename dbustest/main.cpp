#include "dbustest.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    DBusTest w;
    w.show();

    return a.exec();
}
