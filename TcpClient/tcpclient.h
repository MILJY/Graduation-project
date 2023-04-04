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
    void ShowConnect();

private slots:

    void on_send_msg_clicked();

private:
    Ui::TcpClient *ui;
    QString m_ip;
    quint16 m_port;
    QTcpSocket m_tcpsocket;
};
#endif // TCPCLIENT_H
