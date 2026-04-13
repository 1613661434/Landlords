#ifndef LANDLORDS_USE_EXTERNAL_RCC
// #define LANDLORDS_USE_EXTERNAL_RCC
#endif

#include "loading.h"
#include "cards.h"
#include <QApplication>

#ifdef LANDLORDS_USE_EXTERNAL_RCC
#include <QResource>
#endif

int main(int argc, char* argv[])
{
    qRegisterMetaType<Cards>("Cards&");
    qRegisterMetaType<Cards>("Cards");
    qRegisterMetaType<Card>("Card");
    qRegisterMetaType<Card>("Card&");
    QApplication a(argc, argv);

#ifdef LANDLORDS_USE_EXTERNAL_RCC
    QResource::registerResource("./resource.rcc");
#endif

    Loading w;
    w.show();
    return a.exec();
}
