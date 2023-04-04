#include "mytcpserver.h"

MyTcpServer::MyTcpServer()
{

}
MyTcpServer &MyTcpServer::ins()
{
    static MyTcpServer instance;
    return instance;
}

void MyTcpServer::incomingConnection(qintptr handle)
{
    qDebug()<<"New Client Connect!";
    MyTcpSocket *m_tcpsocket = new MyTcpSocket;
    m_tcpsocket->setSocketDescriptor(handle);
    m_tcpsocket_list.append(m_tcpsocket);
    //delete m_tcpsocket;
}
