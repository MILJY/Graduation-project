#ifndef TCPCLIENT_H
#define TCPCLIENT_H
#include <QWidget>
#include <QFile>
#include <QtDebug>
#include <QMessageBox>
#include <QTcpSocket>
#include "protocol.h"
QT_BEGIN_NAMESPACE
namespace Ui { class TcpClient; }
QT_END_NAMESPACE

class TcpClient : public QWidget
{
    Q_OBJECT

public:
    TcpClient(QWidget *parent = nullptr);
    ~TcpClient();
    void LoadConfig();//加载资源文件
    static TcpClient& ins();
    QTcpSocket &GetTcpSocket();
    QString GetLoginName();
    QString GetCurrentPath();
private:
    void SendMsgToServer(uint msg_type);
    void HandleRegistRespond(PDU *pdu);//处理注册响应
    void HandleLoginRespond(PDU *pdu);//处理登录响应
    void HandleSearchUserRespond(PDU *pdu);//处理查找用户响应
    void HandleAddUserRequest(PDU *pdu);//处理添加好友请求
    void HandlePrivateChatRequest(PDU *pdu);//处理朋友发的私信请求
    void HandleGroupChatRequest(PDU *pdu);//处理群发信息请求
public slots:
    void ShowConnect();//接收连接服务端的connect信号
    void ReceiveInfo();//接收服务端发送的信息

private slots:

    void on_login_btn_clicked();

    void on_register_btn_clicked();

    void on_logout_btn_clicked();

private:
    Ui::TcpClient *ui;
    QString m_ip;
    quint16 m_port;
    QTcpSocket m_tcpsocket;
    QString login_name;//当前登录的用户名
    QString current_path;
};
#endif // TCPCLIENT_H
