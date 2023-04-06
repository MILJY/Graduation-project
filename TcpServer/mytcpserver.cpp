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
    connect(m_tcpsocket, SIGNAL(Offline(MyTcpSocket *)), this, SLOT(DeleteSocket(MyTcpSocket *)));
}

MyTcpSocket *MyTcpServer::GetTcpSocketByName(QString frient_name)
{
    QList<MyTcpSocket*>::iterator it = m_tcpsocket_list.begin();
    for(; it != m_tcpsocket_list.end(); it++)
    {
        if((*it)->GetName() == frient_name)
        {
            return *it;
        }
    }
    return NULL;
}

void MyTcpServer::DeleteSocket(MyTcpSocket *mytcpsocket)
{
    qDebug() << m_tcpsocket_list.size();
    QList<MyTcpSocket *>::iterator it = m_tcpsocket_list.begin();
    for(; it != m_tcpsocket_list.end(); it++)
    {
        if(*it == mytcpsocket)
        {
            (*it)->deleteLater();
            *it = NULL;
            m_tcpsocket_list.erase(it);
            break;
        }
    }
    qDebug() << m_tcpsocket_list.size();
}
