#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QWidget>
#include <QFile>
#include <QtDebug>
#include <QMessageBox>
#include <QTcpSocket>
QT_BEGIN_NAMESPACE
namespace Ui { class TcpClient; }
QT_END_NAMESPACE

class TcpClient : public QWidget
{
    Q_OBJECT

public:
    TcpClient(QWidget *parent = nullptr);
    ~TcpClient();
    void LoadConfig();

public slots:
    void ShowConnect();//接收连接服务端的connect信号
    void ReceiveInfo();//接收服务端发送的信息

private slots:

    //void on_send_msg_clicked();

    void on_login_btn_clicked();

    void on_register_btn_clicked();

    void on_logout_btn_clicked();

private:
    Ui::TcpClient *ui;
    QString m_ip;
    quint16 m_port;
    QTcpSocket m_tcpsocket;
};
#endif // TCPCLIENT_H
