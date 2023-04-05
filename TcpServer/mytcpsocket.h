#ifndef MYTCPSOCKET_H
#define MYTCPSOCKET_H
#include <QTcpSocket>
#include <QDebug>
class MyTcpSocket : public QTcpSocket
{
    Q_OBJECT
public:
    MyTcpSocket();

signals:
    void Offline(MyTcpSocket *socket); // 通过信号传送给mytcpserver用户下线通知，然后附带参数socket地址方便删除
private:
    QString name;

public slots:
    void RecvMsg();
    void HandleClientOffline();


};

#endif // MYTCPSOCKET_H
