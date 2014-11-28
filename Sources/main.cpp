#include <QtGui>
#include <QApplication>
#include <QTranslator>
#include <QDebug>

#include "gui/fotolab.h"

int main(int argc, char *argv[]){
    QApplication a(argc, argv);

    QTranslator translator;
    translator.load("FotoLab_sk");
    a.installTranslator(&translator);

    FotoLab w;
    w.show();
    return a.exec();
}
