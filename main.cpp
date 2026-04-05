#include "gamepanel.h"

#include <QApplication>

int main(int argc, char* argv[])
{
    qRegisterMetaType<Cards>("Cards");
    qRegisterMetaType<Card>("Card");
    QApplication a(argc, argv);
    GamePanel w;
    w.show();
    return a.exec();
}
