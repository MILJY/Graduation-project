#include "mytcpsocket.h"
#include "protocol.h"
MyTcpSocket::MyTcpSocket()
{
    connect(this, SIGNAL(readyRead()), this, SLOT(RecvMsg()));
}

void MyTcpSocket::RecvMsg()
{
    qDebug() << this->bytesAvailable();
    uint pdu_len = 0;
    this->read((char *)&pdu_len, sizeof(uint));
    uint msg_len = pdu_len - sizeof(PDU);
    PDU *pdu = mkPDU(msg_len);
    this->read((char *)pdu + sizeof(uint), pdu_len - sizeof(uint));
    qDebug() << pdu->pdu_len << pdu->msg_type << pdu->ca_data << pdu->msg_len << (char *)pdu->ca_msg;
    free(pdu);
    pdu = NULL;
}
