#ifndef PROTOCOL_H
#define PROTOCOL_H
#include <iostream>
#include <QDebug>
#define uint unsigned int

#define Register_Success "Register Success"   //注册成功
#define Register_Fail "Register Fail" //注册时用户名已经存在，注册失败

#define Login_Success "Login Success" //登录成功
#define Login_Fail "Login Fail" //登录失败

#define Logout_Success "Logout Success" //注销成功
#define Logout_Fail "Logout Fail" //注销失败

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

#define Create_Folder_Success "Create Folder Success" //创建文件夹成功
#define Folder_Name_Exist "Folder Name Exist" //文件夹已存在
#define Create_Folder_Fail "Create Folder Fail" //创建文件夹失败
#define Path_Not_Exist "Path Not Exist" //文件路径不存在

#define Delete_File_Success "Delete File Success" //删除文件成功
#define Delete_File_Fail "Delete File Fail" //删除文件失败

#define Rename_File_Success "Rename File Success" //重命名文件成功
#define Rename_File_Fail "Rename File Fail" //重命名文件失败

#define Entry_File_Success "Entry File Success" //进入文件夹成功
#define Entry_File_Fail "Entry File Fail" //进入文件夹失败

#define Return_Path_Success "Return Path Success" //返回上一级目录成功

#define Upload_File_Success "Upload File Success"//上传文件成功
#define Upload_File_Fail "Upload File Fail"//上传文件失败
#define Upload_File_Start "Upload File Start" //开始上传文件
#define Share_File_Success "Share File Success"
enum MSG_TYPE
{
    MSG_TYPE_MIN = 0,
    MSG_TYPE_REGIST_REQUEST,//注册请求
    MSG_TYPE_REGIST_RESPOND,//注册回复
    MSG_TYPE_LOGIN_REQUEST,//登录请求
    MSG_TYPE_LOGIN_RESPOND,//登录回复
    MSG_TYPE_LOGOUT_REQUEST,//注销请求
    MSG_TYPE_LOGOUT_RESPOND,//注销响应
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
    MSG_TYPE_USER_ONLINE_REQUEST,//好友是否在线请求
    MSG_TYPE_USER_ONLINE_RESPOND,//好友是否在线响应
    MSG_TYPE_PRIVATE_CHAT_REQUEST,//私聊信息请求
    MSG_TYPE_GROUP_CHAT_REQUEST,//群聊信息请求
    MSG_TYPE_CREATE_FOLDER_REQUEST,//创建文件夹请求
    MSG_TYPE_CREATE_FOLDER_RESPOND,//创建文件夹响应
    MSG_TYPE_FLUSH_FOLDER_REQUEST,//刷新文件夹请求
    MSG_TYPE_FLUSH_FOLDER_RESPOND,//刷新文件夹响应
    MSG_TYPE_DELETE_FOLDER_REQUEST,//删除文件夹请求
    MSG_TYPE_DELETE_FLODER_RESPOND,//删除文件夹响应
    MSG_TYPE_RENAME_FILE_REQUEST,//重命名请求
    MSG_TYPE_RENAME_FILE_RESPOND,//重命名响应
    MSG_TYPE_ENTRY_FILE_REQUEST,//进入文件夹请求
    MSG_TYPE_ENTRY_FILE_RESPOND,//进入文件夹响应
    MSG_TYPE_RETURN_PATH_REQUEST,//返回上一级目录请求
    MSG_TYPE_RETURN_PATH_RESPOND,//返回上一级目录响应
    MSG_TYPE_UPLOAD_FILE_REQUEST,//上传文件请求
    MSG_TYPE_UPLOAD_FILE_RESPOND,//上传文件响应
    MSG_TYPE_DOWNLOAD_FILE_REQUEST,//下载文件请求
    MSG_TYPE_DOWNLOAD_FILE_RESPOND,//下载文件响应
    MSG_TYPE_ONLINE_FRIEND_REQUEST,//查找所有在线好友请求
    MSG_TYPE_ONLINE_FRIEND_RESPOND,//查找所有在线好友响应
    MSG_TYPE_SHARE_FILE_REQUEST,
    MSG_TYPE_SHARE_FILE_RESPOND,
    MSG_TYPE_SHARE_FILE_NOTE,
    MSG_TYPE_SHARE_FILE_NOTE_RESPOND,
    MSG_TYPE_MAX = 0x00ffffff,
};

struct FileInfo{
    char file_name[32];
    int file_type;
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
