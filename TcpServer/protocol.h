#ifndef PROTOCOL_H
#define PROTOCOL_H
#include <iostream>
#include <QDebug>
#define uint unsigned int

#define Register_Success "Register Success"   //注册成功
#define Register_Fail "Register Fail" //注册时用户名已经存在，注册失败

#define Login_Success "Login Success" //登录成功
#define Login_Fail "Login Fail" //登录失败

enum MSG_TYPE
{
    MSG_TYPE_MIN = 0,
    MSG_TYPE_REGIST_REQUEST,//注册请求
    MSG_TYPE_REGIST_RESPOND,//注册回复
    MSG_TYPE_LOGIN_REQUEST,//登录请求
    MSG_TYPE_LOGIN_RESPOND,//登录回复
    MSG_TYPE_MAX = 0x00ffffff,
};
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
