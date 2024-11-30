#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
   // w.show();
    MainWindow window;
    window.setWindowTitle("Boot Loader For LPC1788");
    window.resize(500, 300);
    window.show();

    return a.exec();
}
