#include "tcpclient.h"
#include "ui_tcpclient.h"
#include "protocol.h"
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


void TcpClient::on_send_msg_clicked()
{
    qDebug() << "clicked";
    QString str_msg = ui->lineEdit->text();
    if(str_msg.isEmpty())
    {
        QMessageBox::warning(this, "Send Message", "Send Message Cannot Be Empty!");
    }
    else
    {
        PDU *pdu = mkPDU(str_msg.size());
        pdu->msg_type = 8888;
        strcpy((char *)pdu->ca_msg, str_msg.toStdString().c_str());
        qDebug() << pdu->pdu_len << pdu->msg_type << pdu->ca_data << pdu->msg_len << (char *)pdu->ca_msg;
        m_tcpsocket.write((char *)pdu, pdu->pdu_len);
        free(pdu);
        pdu = NULL;
    }
}
