#include "mytcpsocket.h"
#include "mysql.h"
#include "mytcpserver.h"
MyTcpSocket::MyTcpSocket()
{
    connect(this, SIGNAL(readyRead()), this, SLOT(RecvMsg()));
    connect(this, SIGNAL(disconnected()), this, SLOT(HandleClientOffline()));
}

QString MyTcpSocket::GetName()
{
    return name;
}

void MyTcpSocket::HandleReginstRequest(PDU *pdu)
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
        QDir dir;
        dir.mkdir(QString("./File/%1").arg(str_name));

    }
    else
    {
        strcpy(res_pdu->ca_data, Register_Fail);
    }
    this->write((char *)res_pdu, res_pdu->pdu_len);
    free(res_pdu);
    res_pdu = NULL;
}

void MyTcpSocket::HandleLoginRequest(PDU *pdu)
{
    char str_name[32] = {'\0'};
    char str_pwd[32] = {'\0'};
    strncpy(str_name, pdu->ca_data, 32);
    strncpy(str_pwd, pdu->ca_data + 32, 32);
    bool res = MySql::ins().HandleLogin(str_name, str_pwd);
    PDU *res_pdu = mkPDU(0);
    res_pdu->msg_type = MSG_TYPE_LOGIN_RESPOND;
    if(res)
    {
        name =  str_name;
        strcpy(res_pdu->ca_data, Login_Success);

    }
    else
    {
        strcpy(res_pdu->ca_data, Login_Fail);
    }
    this->write((char *)res_pdu, res_pdu->pdu_len);
    free(res_pdu);
    res_pdu = NULL;
}

void MyTcpSocket::HandleSearchOnlineUserRequest()
{
    QStringList search_result = MySql::ins().HandleSearchOnlineUser();
    uint msg_len = search_result.size() * 32;
    PDU *res_pdu = mkPDU(msg_len);
    res_pdu->msg_type = MSG_TYPE_SEARCH_ONLINE_USER_RESPOND;
    for(int i = 0; i < search_result.size(); i++)
    {
        memcpy((char *)(res_pdu->ca_msg) + 32 * i, search_result[i].toStdString().c_str(), search_result[i].size());
    }
    this->write((char *)res_pdu, res_pdu->pdu_len);
    free(res_pdu);
    res_pdu = NULL;
}

void MyTcpSocket::HandleSearchUserRequest(PDU *pdu)
{
    int is_online = MySql::ins().HandleSearchUser(pdu->ca_data);
    PDU *res_pdu = mkPDU(0);
    res_pdu->msg_type = MSG_TYPE_SERACH_USER_RESPOND;
    if(is_online == -1)
    {
        strcpy(res_pdu->ca_data, Search_User_fail);
    }
    else
    {
        if(is_online == 0)
        {
            strcpy(res_pdu->ca_data, Search_User_Offline);
        }
        else
        {
            strcpy(res_pdu->ca_data, Search_User_Online);
        }
    }
    this->write((char *)res_pdu, res_pdu->pdu_len);
    free(res_pdu);
    res_pdu = NULL;
}

void MyTcpSocket::HandleAddUserRequest(PDU *pdu)
{
    char login_name[32] = {'\0'};
    char friend_name[32] = {'\0'};
    strncpy(login_name, pdu->ca_data, 32);
    strncpy(friend_name, pdu->ca_data + 32, 32);
    int result = MySql::ins().HandleAddUser(login_name, friend_name);
    PDU *res_pdu = mkPDU(0);
    res_pdu->msg_type = MSG_TYPE_ADD_USER_RESPOND;
    if(result == -1)
    {
        strcpy(res_pdu->ca_data, Unknow_Error);
    }
    if(result == 0)//已经是好友
    {
        strcpy(res_pdu->ca_data, Add_User_Exist);
    }
    if(result == 1)//对方不存在
    {
        strcpy(res_pdu->ca_data, Add_User_Not_Exist);
    }
    if(result == 2)
    {
        strcpy(res_pdu->ca_data, Add_User_Offline);
    }
    if(result == 3)
    {
        SendAddUserRequest(friend_name);
        strcpy(res_pdu->ca_data, Add_User_Request_Send);
    }
    this->write((char *)res_pdu, res_pdu->pdu_len);
    free(res_pdu);
    res_pdu = NULL;
}

void MyTcpSocket::SendAddUserRequest(QString friend_name)
{
    MyTcpSocket *friend_tcpsocket = MyTcpServer::ins().GetTcpSocketByName(friend_name);
    PDU *res_pdu = mkPDU(0);
    res_pdu->msg_type = MSG_TYPE_ADD_USER_REQUEST;
    memcpy(res_pdu->ca_data, name.toStdString().c_str(), name.size());
    friend_tcpsocket->write((char *)res_pdu, res_pdu->pdu_len);
    free(res_pdu);
    res_pdu = NULL;
}

void MyTcpSocket::SendMsgToFriend(PDU *pdu)
{
    char friend_name[32] = {'\0'};
    strncpy(friend_name, pdu->ca_data + 32, 32);
    MyTcpSocket *friend_tcpsocket = MyTcpServer::ins().GetTcpSocketByName(friend_name);
    friend_tcpsocket->write((char *)pdu, pdu->pdu_len);

}

void MyTcpSocket::SendMsgToAllOnlineFriend(PDU *pdu)
{
    char login_name[32] = {'\0'};
    strncpy(login_name, pdu->ca_data, 32);
    QStringList search_result = MySql::ins().HandleAllOnlineFriend(pdu->ca_data);
    for(int i = 0; i < search_result.size(); i++)
    {
        MyTcpSocket *friend_tcpsocket = MyTcpServer::ins().GetTcpSocketByName(search_result[i]);
        friend_tcpsocket->write((char *)pdu, pdu->pdu_len);
    }
}

void MyTcpSocket::SendAddUserResult(QString friend_name, uint msg_type)
{
    MyTcpSocket *friend_tcpsocket = MyTcpServer::ins().GetTcpSocketByName(friend_name);
    PDU *res_pdu = mkPDU(0);
    res_pdu->msg_type = msg_type;
    memcpy(res_pdu->ca_data, name.toStdString().c_str(), name.size());
    friend_tcpsocket->write((char *)res_pdu, res_pdu->pdu_len);
    free(res_pdu);
    res_pdu = NULL;
}

void MyTcpSocket::HandleAgreeAddUser(PDU *pdu)
{
    MySql::ins().HandleAgreeAddUser(pdu->ca_data, name);
    SendAddUserResult(pdu->ca_data, pdu->msg_type);
}

void MyTcpSocket::HandleRefuseAddUser(PDU *pdu)
{
    SendAddUserResult(pdu->ca_data, pdu->msg_type);
}

void MyTcpSocket::HandleFlushFriendListRequest(PDU *pdu)
{
    QStringList search_result = MySql::ins().HandleFlushFriendList(pdu->ca_data);
    uint msg_len = search_result.size() * 32;
    PDU *res_pdu = mkPDU(msg_len);
    res_pdu->msg_type = MSG_TYPE_FLUSH_FRIEND_LIST_RESPOND;
    for(int i = 0; i < search_result.size(); i++)
    {
        memcpy((char *)(res_pdu->ca_msg) + 32 * i, search_result[i].toStdString().c_str(), search_result[i].size());
    }
    this->write((char *)res_pdu, res_pdu->pdu_len);
    free(res_pdu);
    res_pdu = NULL;
}

void MyTcpSocket::HandleDeleteFriendRequest(PDU *pdu)
{
    char login_name[32] = {'\0'};
    char friend_name[32] = {'\0'};
    strncpy(login_name, pdu->ca_data, 32);
    strncpy(friend_name, pdu->ca_data + 32, 32);
    bool flag = MySql::ins().HandleDeleteFriend(login_name, friend_name);
    PDU *res_pdu = mkPDU(0);
    res_pdu->msg_type = MSG_TYPE_DELETE_FRIEND_RESPOND;
    if(flag)
    {
        strcpy(res_pdu->ca_data, Delete_Friend_Success);
    }
    else
        strcpy(res_pdu->ca_data, Delete_Friend_Fail);
    write((char *)res_pdu, res_pdu->pdu_len);
    free(res_pdu);
    res_pdu = NULL;
}

void MyTcpSocket::HandleUserIsOnlineRequest(PDU *pdu)
{
    int is_online = MySql::ins().HandleSearchUser(pdu->ca_data);
    PDU *res_pdu = mkPDU(0);
    res_pdu->msg_type = MSG_TYPE_USER_ONLINE_RESPOND;
    if(is_online == -1)
    {
        strcpy(res_pdu->ca_data, Search_User_fail);
    }
    else
    {
        if(is_online == 0)
        {
            strcpy(res_pdu->ca_data, Search_User_Offline);
        }
        else
        {
            strcpy(res_pdu->ca_data, Search_User_Online);
        }
    }
    strcpy(res_pdu->ca_data + 32, pdu->ca_data);
    this->write((char *)res_pdu, res_pdu->pdu_len);
    free(res_pdu);
    res_pdu = NULL;
}

void MyTcpSocket::HandleCreateFolderRequest(PDU *pdu)
{
    QDir dir;
    QString folder_name = QString("%1").arg(pdu->ca_data);
    QString current_path = QString("%1").arg((char *)(pdu->ca_msg));
    bool res = dir.exists(current_path);
    PDU *res_pdu = mkPDU(0);
    res_pdu->msg_type = MSG_TYPE_CREATE_FOLDER_RESPOND;
    if(res)
    {

        QString new_current_path = current_path + "/" + folder_name;
        if(dir.exists(new_current_path))
        {
            strcpy(res_pdu->ca_data, Folder_Name_Exist);
        }
        else
        {
            dir.mkdir(new_current_path);
            strcpy(res_pdu->ca_data, Create_Folder_Success);
        }
    }
    else
    {
        strcpy(res_pdu->ca_data, Create_Folder_Fail);
    }
    this->write((char *)res_pdu, res_pdu->pdu_len);
    free(res_pdu);
    res_pdu = NULL;
}

void MyTcpSocket::HandleFlushFolderRequest(PDU *pdu)
{
    QString current_path = QString("%1").arg((char *)(pdu->ca_msg));
    QDir dir;
    PDU *res_pdu = NULL;
    if(!dir.exists(current_path))
    {
        res_pdu = mkPDU(0);
        strcpy(res_pdu->ca_data, Path_Not_Exist);
    }
    else
    {
        dir.setPath(current_path);
        QFileInfoList fileinfo_list = dir.entryInfoList();
        int file_cnt = fileinfo_list.size();
        res_pdu = mkPDU(file_cnt * sizeof(FileInfo));
        for(int  i = 0; i < file_cnt; i++)
        {
            QString file_name = fileinfo_list[i].fileName();
            FileInfo *fileinfo = (FileInfo *)(res_pdu->ca_msg) + i;
            memcpy(fileinfo->file_name, file_name.toStdString().c_str(), file_name.size());
            if(fileinfo_list[i].isDir())
                fileinfo->file_type = 0;
            else fileinfo->file_type = 1;
        }
    }
    res_pdu->msg_type = MSG_TYPE_FLUSH_FOLDER_RESPOND;
    this->write((char *)res_pdu, res_pdu->pdu_len);
    free(res_pdu);
    res_pdu = NULL;
}

void MyTcpSocket::HandleDeleteFileRequest(PDU *pdu)
{

}

void MyTcpSocket::RecvMsg()
{
    uint pdu_len = 0;
    this->read((char *)&pdu_len, sizeof(uint));
    uint msg_len = pdu_len - sizeof(PDU);
    PDU *pdu = mkPDU(msg_len);
    this->read((char *)pdu + sizeof(uint), pdu_len - sizeof(uint));
    //qDebug() << pdu->pdu_len << pdu->msg_type << pdu->ca_data << pdu->msg_len << (char *)pdu->ca_msg;

    //qDebug() << str_name << str_pwd << pdu->msg_type;
    switch (pdu->msg_type)
    {
    case MSG_TYPE_REGIST_REQUEST:
    {
        HandleReginstRequest(pdu);
        break;
    }
    case MSG_TYPE_LOGIN_REQUEST:
    {
        HandleLoginRequest(pdu);
        break;
    }
    case MSG_TYPE_SEARCH_ONLINE_USER_REQUEST:
    {
        HandleSearchOnlineUserRequest();
        break;
    }
    case MSG_TYPE_SERACH_USER_REQUEST:
    {
        HandleSearchUserRequest(pdu);
        break;
    }
    case MSG_TYPE_ADD_USER_REQUEST:
    {
        HandleAddUserRequest(pdu);
        break;
    }
    case MSG_TYPE_AGREE_ADD_USER:
    {
        HandleAgreeAddUser(pdu);
        break;
    }
    case MSG_TYPE_REFUSE_ADD_USER:
    {
        HandleRefuseAddUser(pdu);
        break;
    }
    case MSG_TYPE_FLUSH_FRIEND_LIST_REQUEST:
    {
        HandleFlushFriendListRequest(pdu);
        break;
    }
    case MSG_TYPE_DELETE_FRIEND_REQUEST:
    {
        HandleDeleteFriendRequest(pdu);
        break;
    }
    case MSG_TYPE_USER_ONLINE_REQUEST:
    {
        HandleUserIsOnlineRequest(pdu);
        break;
    }
    case MSG_TYPE_PRIVATE_CHAT_REQUEST:
    {
        SendMsgToFriend(pdu);
        break;
    }
    case MSG_TYPE_GROUP_CHAT_REQUEST:
    {
        SendMsgToAllOnlineFriend(pdu);
        break;
    }
    case MSG_TYPE_CREATE_FOLDER_REQUEST:
    {
        HandleCreateFolderRequest(pdu);
        break;
    }
    case MSG_TYPE_FLUSH_FOLDER_REQUEST:
    {
        HandleFlushFolderRequest(pdu);
        break;
    }
    case MSG_TYPE_DELETE_FLODER_RESPOND:
    {
        HandleDeleteFileRequest(pdu);
        break;
    }
    default:
            break;
    }

    free(pdu);
    pdu = NULL;
}

void MyTcpSocket::HandleClientOffline()
{
    bool res = MySql::ins().HandleClientOffline(name);
    if(res)
    {
        emit Offline(this);
    }

}
