#include "wnd.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Wnd w;
    w.show();

    return a.exec();
}
