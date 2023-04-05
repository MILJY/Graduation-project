#include "tcpclient.h"
#include "ui_tcpclient.h"
#include "protocol.h"
TcpClient::TcpClient(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::TcpClient)
{
    ui->setupUi(this);
    LoadConfig();
    connect(&m_tcpsocket, SIGNAL(readyRead()), this, SLOT(ReceiveInfo()));
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

void TcpClient::ReceiveInfo()
{
    uint pdu_len = 0;
    m_tcpsocket.read((char *)&pdu_len, sizeof(uint));
    uint msg_len = pdu_len - sizeof(PDU);
    PDU *pdu = mkPDU(msg_len);
    m_tcpsocket.read((char *)pdu + sizeof(uint), pdu_len - sizeof(uint));
    //qDebug() << pdu->pdu_len << pdu->msg_type << pdu->ca_data << pdu->msg_len << (char *)pdu->ca_msg;
    switch(pdu->msg_type)
    {
    //注册
    case MSG_TYPE_REGIST_RESPOND:
    {
        if(strcmp(pdu->ca_data, Register_Success) == 0)
        {
            QMessageBox::information(this, "Register", Register_Success);
        }
        else
        {
            QMessageBox::critical(this, "Register", Register_Fail);
        }
        break;
    }
    //登录
    case MSG_TYPE_LOGIN_RESPOND:
    {
        if(strcmp(pdu->ca_data, Login_Success) == 0)
        {
            QMessageBox::information(this, "Login", Login_Success);
        }
        else
        {
            QMessageBox::critical(this, "Login", Login_Fail);
        }
        break;
    }
    default:
            break;
    }
    free(pdu);
    pdu = NULL;
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

void TcpClient::SendMsgToServer(uint msg_type)
{
    QString str_name = ui->edit_name->text();
    QString str_pwd = ui->edit_password->text();
    if(str_name.isEmpty() || str_pwd.isEmpty())
    {
        if(msg_type == MSG_TYPE_LOGIN_REQUEST)
            QMessageBox::critical(this, "Login", "Login Fail: Account And Password Cannot Be Empty");
        if(msg_type == MSG_TYPE_REGIST_REQUEST)
            QMessageBox::critical(this, "Register", "Register Fail: Account And Password Cannot Be Empty");

    }
    else
    {
        PDU *pdu = mkPDU(0);
        pdu->msg_type = msg_type;
        strncpy(pdu->ca_data, str_name.toStdString().c_str(), 32);
        strncpy(pdu->ca_data + 32, str_pwd.toStdString().c_str(), 32);
        m_tcpsocket.write((char *)pdu, pdu->pdu_len);
        free(pdu);
        pdu = NULL;
    }
}


void TcpClient::on_login_btn_clicked()
{
    SendMsgToServer(MSG_TYPE_LOGIN_REQUEST);
}

void TcpClient::on_register_btn_clicked()
{
    SendMsgToServer(MSG_TYPE_REGIST_REQUEST);
}

void TcpClient::on_logout_btn_clicked()
{

}
