#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
   // w.show();
    MainWindow window;
    window.setWindowTitle("Text to Image Converter");
    window.resize(500, 300);
    window.show();

    return a.exec();
}
