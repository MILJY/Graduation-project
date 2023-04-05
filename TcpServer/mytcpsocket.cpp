#include "mytcpsocket.h"
#include "mysql.h"

MyTcpSocket::MyTcpSocket()
{
    connect(this, SIGNAL(readyRead()), this, SLOT(RecvMsg()));
    connect(this, SIGNAL(disconnected()), this, SLOT(HandleClientOffline()));
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
