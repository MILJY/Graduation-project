#ifndef PROTOCOL_H
#define PROTOCOL_H
#include <iostream>
#include <QDebug>
#define uint unsigned int

struct PDU//通讯协议
{
    uint pdu_len;//总的协议数据单元的大小
    uint msg_type;//消息类型
    char ca_data[64];
    uint msg_len;//实际消息大小
    int ca_msg[];//实际消息
};

PDU *mkPDU(uint msg_len);
#endif // PROTOCOL_H
