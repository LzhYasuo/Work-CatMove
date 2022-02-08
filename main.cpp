#include "Src/Gui/CatMove.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CatMove w;
    w.show();

    return a.exec();
}
