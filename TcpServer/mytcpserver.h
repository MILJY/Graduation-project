#ifndef MYTCPSERVER_H
#define MYTCPSERVER_H
#include <QTcpServer>
#include <QList>
#include "mytcpsocket.h"

class MyTcpServer : public QTcpServer
{
    Q_OBJECT
public:
    MyTcpServer();
    static MyTcpServer &ins();
    virtual void incomingConnection(qintptr handle);

private:
    QList<MyTcpSocket*>m_tcpsocket_list;
};

#endif // MYTCPSERVER_H
