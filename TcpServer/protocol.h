#ifndef PROTOCOL_H
#define PROTOCOL_H
#include <iostream>
#include <QDebug>
#define uint unsigned int

#define Register_Success "Register Success"   //注册成功
#define Register_Fail "Register Fail" //注册时用户名已经存在，注册失败

#define Login_Success "Login Success" //登录成功
#define Login_Fail "Login Fail" //登录失败

#define Search_User_Online "User Online" //查找用户在线
#define Search_User_Offline "User Offline"  //查找用户离线
#define Search_User_fail "Account Does Not Exist"  //查找用户不存在

#define Unknow_Error "Unknow Error"
#define Add_User_Request_Send "Add User Request Send"  //添加好友请求发送成功
#define Add_User_Not_Exist "Friend Name Does Not Exist" //好友名称不存在
#define Add_User_Offline "Friend Name Offline" //对方不在线
#define Add_User_Exist "Add Friend Already Exist" //对方已经为朋友
#define Add_User_Fail "The Other Refused To Add Friends" //对方拒绝添加

#define Delete_Friend_Success "Delete Friend Success"//删除好友成功
#define Delete_Friend_Fail "Delete Friend Fail"//删除好友失败
enum MSG_TYPE
{
    MSG_TYPE_MIN = 0,
    MSG_TYPE_REGIST_REQUEST,//注册请求
    MSG_TYPE_REGIST_RESPOND,//注册回复
    MSG_TYPE_LOGIN_REQUEST,//登录请求
    MSG_TYPE_LOGIN_RESPOND,//登录回复
    MSG_TYPE_SEARCH_ONLINE_USER_REQUEST,//查找所有在线用户请求
    MSG_TYPE_SEARCH_ONLINE_USER_RESPOND,//查找所有在线用户响应
    MSG_TYPE_SERACH_USER_REQUEST,//查找用户请求
    MSG_TYPE_SERACH_USER_RESPOND,//查找用户响应
    MSG_TYPE_ADD_USER_REQUEST,//添加好友请求
    MSG_TYPE_ADD_USER_RESPOND,//添加好友响应
    MSG_TYPE_AGREE_ADD_USER,//同意添加好友
    MSG_TYPE_REFUSE_ADD_USER,//拒绝添加好友
    MSG_TYPE_FLUSH_FRIEND_LIST_REQUEST,//刷新好友列表请求
    MSG_TYPE_FLUSH_FRIEND_LIST_RESPOND,//刷新好友列表响应
    MSG_TYPE_DELETE_FRIEND_REQUEST,//删除好友请求
    MSG_TYPE_DELETE_FRIEND_RESPOND,//删除好友响应
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
