#include <QApplication>
#include "mainwindow.h"
#include <iostream>
#include "exprtk.hpp"

int main(int argc, char **argv){
    QApplication app(argc, argv);
    MainWindow w;
    w.show();
    return app.exec();
}
