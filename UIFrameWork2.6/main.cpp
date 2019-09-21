#include "mainwindow.h"
#include <QApplication>
#include "QLog/qlog.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qInstallMessageHandler(LogOutput);
    MainWindow w;
    w.show();
    qDebug("start");
    return a.exec();
}
