#include "tcpserver.h"
#include "ui_tcpserver.h"
#include "mytcpserver.h"

TcpServer::TcpServer(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::TcpServer)
{
    ui->setupUi(this);
    LoadConfig();
    MyTcpServer::ins().listen(QHostAddress(m_ip),m_port);
}

TcpServer::~TcpServer()
{
    delete ui;
}
void TcpServer::LoadConfig()
{
    QFile file(":/config/Server.config");
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

