#include <QtGui/QApplication>
#include "mainwindow.h"
typedef QVector<int> algo;
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationName("Calculadora");
    a.setApplicationVersion("2.000");
    a.setOrganizationName("NABS Developments");
    MainWindow w;
    w.show();

    return a.exec();
}
