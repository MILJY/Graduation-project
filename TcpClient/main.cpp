#include "tcpclient.h"
#include <QApplication>
int main(int argc, char *argv[])
{

    QApplication a(argc, argv);

//    TcpClient w;
//    w.show();
    TcpClient::ins().show();
    return a.exec();
}
