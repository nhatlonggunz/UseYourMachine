#include "uicontroller.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    UIController w;
    w.show();
    return a.exec();
}
