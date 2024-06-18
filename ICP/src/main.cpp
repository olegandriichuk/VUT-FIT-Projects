/**
* @file main.cpp
* @brief Main file for the Robots project.
* @author Oleg Andriichuk (xandri07)
* @author Samuel Synek (xsynek04)
*/
#include "mainwindow.h"
#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    w.show();

    return a.exec();
}
