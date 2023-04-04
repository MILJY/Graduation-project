#include "tcpserver.h"
#include <QApplication>
#include "mysql.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MySql::ins().init();
    TcpServer w;
    w.show();
    return a.exec();
}
