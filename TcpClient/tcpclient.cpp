#include "tcpclient.h"
#include "ui_tcpclient.h"

TcpClient::TcpClient(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::TcpClient)
{
    ui->setupUi(this);
    LoadConfig();
    connect(&m_tcpsocket, SIGNAL(connected()), this, SLOT(ShowConnect()));
    m_tcpsocket.connectToHost(m_ip, m_port);
}

TcpClient::~TcpClient()
{
    delete ui;
}
void TcpClient::ShowConnect()
{
    QMessageBox::information(this, "Connect Server", "Connect to the server Success");
}
void TcpClient::LoadConfig()
{
    QFile file(":/config/Client.config");
    if(file.open(QIODevice::ReadOnly))
    {
        int idx = 0;
        while(!file.atEnd())
        {
            QByteArray line=file.readLine();
            QString tmp_str=line.replace("\r\n","");
            if(idx == 0)
                m_ip = tmp_str;
            else
            {
                m_port = tmp_str.toUShort();
            }
            idx ++;
        }
        file.close();
    }
    else
    {
        QMessageBox::critical(this,"open config", "open config fail");
    }
}
