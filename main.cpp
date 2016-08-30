#include "gomokuwidget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    GomokuWidget w;
    w.show();

    return a.exec();
}
