#include "mainwindow.h"
#include "domain.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    qRegisterMetaType< MsgPtr >();
    QApplication a(argc, argv);
    auto w = std::make_shared< MainWindow >();
    initDomain(w);
    return a.exec();
}
