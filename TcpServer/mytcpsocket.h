#ifndef MYTCPSOCKET_H
#define MYTCPSOCKET_H
#include <QTcpSocket>
#include <QDebug>
#include "protocol.h"
class MyTcpSocket : public QTcpSocket
{
    Q_OBJECT
public:
    MyTcpSocket();
    QString GetName();
private:
    void HandleReginstRequest(PDU *pdu);//处理注册请求
    void HandleLoginRequest(PDU *pdu);//处理登录请求
    void HandleSearchOnlineUserRequest();//处理查询在线用户请求
    void HandleSearchUserRequest(PDU *pdu);//处理查询用户请求
    void HandleAddUserRequest(PDU *pdu);//处理添加好友请求
    void SendAddUserRequest(QString friend_name);//向好友客户端发送添加好友请求
    void SendAddUserResult(QString friend_name, uint msg_type);//向对方客户端发送添加好友请求结果
    void HandleAgreeAddUser(PDU *pdu);//同意添加好友请求
    void HandleRefuseAddUser(PDU *pdu);//拒绝添加好友请求
    void HandleFlushFriendListRequest(PDU *pdu);//刷新好友列表请求
    void HandleDeleteFriendRequest(PDU *pdu);//删除好友请求
signals:
    void Offline(MyTcpSocket *socket); // 通过信号传送给mytcpserver用户下线通知，然后附带参数socket地址方便删除
private:
    QString name;

public slots:
    void RecvMsg();
    void HandleClientOffline();


};

#endif // MYTCPSOCKET_H
