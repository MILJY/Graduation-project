#include "mytcpsocket.h"
#include "protocol.h"
#include "mysql.h"

MyTcpSocket::MyTcpSocket()
{
    connect(this, SIGNAL(readyRead()), this, SLOT(RecvMsg()));
}

void MyTcpSocket::RecvMsg()
{
    //qDebug() << this->bytesAvailable();
    uint pdu_len = 0;
    this->read((char *)&pdu_len, sizeof(uint));
    uint msg_len = pdu_len - sizeof(PDU);
    PDU *pdu = mkPDU(msg_len);
    this->read((char *)pdu + sizeof(uint), pdu_len - sizeof(uint));
    //qDebug() << pdu->pdu_len << pdu->msg_type << pdu->ca_data << pdu->msg_len << (char *)pdu->ca_msg;

    //qDebug() << str_name << str_pwd << pdu->msg_type;
    if(pdu->msg_type == MSG_TYPE_REGIST_REQUEST)
    {
        char str_name[32] = {'\0'};
        char str_pwd[32] = {'\0'};
        strncpy(str_name, pdu->ca_data, 32);
        strncpy(str_pwd, pdu->ca_data + 32, 32);
        bool res = MySql::ins().HandleRegister(str_name, str_pwd);
        PDU *res_pdu = mkPDU(0);
        res_pdu->msg_type = MSG_TYPE_REGIST_RESPOND;
        if(res)
        {
            strcpy(res_pdu->ca_data, Register_Success);

        }
        else
        {
            strcpy(res_pdu->ca_data, Register_Fail);
        }
        write((char *)res_pdu, res_pdu->pdu_len);
        free(res_pdu);
        res_pdu = NULL;
    }
    free(pdu);
    pdu = NULL;
}
