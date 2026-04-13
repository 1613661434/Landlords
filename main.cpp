#include "loading.h"
#include "cards.h"
#include <QApplication>

int main(int argc, char* argv[])
{
    qRegisterMetaType<Cards>("Cards");
    qRegisterMetaType<Card>("Card");
    QApplication a(argc, argv);
    Loading w;
    w.show();
    return a.exec();
}
