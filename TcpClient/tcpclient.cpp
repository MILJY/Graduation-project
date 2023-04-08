#include "tcpclient.h"
#include "ui_tcpclient.h"
#include "mainwindow.h"
#include "onlineuser.h"
#include "friendchat.h"
#include "fileoperation.h"
TcpClient::TcpClient(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::TcpClient)
{
    ui->setupUi(this);
    LoadConfig();
    connect(&m_tcpsocket, SIGNAL(readyRead()), this, SLOT(ReceiveInfo()));//收信息
    connect(&m_tcpsocket, SIGNAL(connected()), this, SLOT(ShowConnect()));//与服务端连接
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
        HandleRegistRespond(pdu);

        break;
    }
    //登录
    case MSG_TYPE_LOGIN_RESPOND:
    {
        HandleLoginRespond(pdu);
        break;
    }
    case MSG_TYPE_SEARCH_ONLINE_USER_RESPOND:
    {
        OnlineUser::ins().SHowOnlineUser(pdu);
        break;
    }
    case MSG_TYPE_SERACH_USER_RESPOND:
    {
        HandleSearchUserRespond(pdu);
        break;
    }
    case MSG_TYPE_ADD_USER_RESPOND:
    {
        QMessageBox::information(this, "添加好友", pdu->ca_data);
        break;
    }
    case MSG_TYPE_ADD_USER_REQUEST:
    {
        HandleAddUserRequest(pdu);
        break;
    }
    case MSG_TYPE_AGREE_ADD_USER:
    {
        QMessageBox::information(this, "添加好友结果", QString("%1 同意了你的申请！").arg(pdu->ca_data));
        break;
    }
    case MSG_TYPE_REFUSE_ADD_USER:
    {
        QMessageBox::information(this, "添加好友结果", QString("%1 拒绝了你的申请！").arg(pdu->ca_data));
        break;
    }
    case MSG_TYPE_FLUSH_FRIEND_LIST_RESPOND:
    {
        FriendChat::ins().FlushFriendList(pdu);
        break;
    }
    case MSG_TYPE_DELETE_FRIEND_RESPOND:
    {
        QMessageBox::information(this, "删除好友", pdu->ca_data);
        break;
    }
    case MSG_TYPE_USER_ONLINE_RESPOND:
    {
        FriendChat::ins().HandlePrivateChat(pdu);
        break;
    }
    case MSG_TYPE_PRIVATE_CHAT_REQUEST:
    {
        HandlePrivateChatRequest(pdu);
        break;
    }
    case MSG_TYPE_GROUP_CHAT_REQUEST:
    {
        HandleGroupChatRequest(pdu);
        break;
    }
    case MSG_TYPE_CREATE_FOLDER_RESPOND:
    {
        QMessageBox::information(this, "Create Folder", pdu->ca_data);
        break;
    }
    case MSG_TYPE_FLUSH_FOLDER_RESPOND:
    {
        FileOperation::ins().FlushFolder(pdu);
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

TcpClient &TcpClient::ins()
{
    static TcpClient instance;
    return instance;
}

QTcpSocket &TcpClient::GetTcpSocket()
{
    return m_tcpsocket;
}

QString TcpClient::GetLoginName()
{
    return login_name;
}

QString TcpClient::GetCurrentPath()
{
    return current_path;
}

void TcpClient::SendMsgToServer(uint msg_type)
{
    QString str_name = ui->edit_name->text();

    QString str_pwd = ui->edit_password->text();
    if(str_name.isEmpty() || str_pwd.isEmpty())
    {
        if(msg_type == MSG_TYPE_LOGIN_REQUEST)
        {
            QMessageBox::critical(this, "Login", "Login Fail: Account And Password Cannot Be Empty");
        }
        if(msg_type == MSG_TYPE_REGIST_REQUEST)
            QMessageBox::critical(this, "Register", "Register Fail: Account And Password Cannot Be Empty");

    }
    else
    {
        PDU *pdu = mkPDU(0);
        pdu->msg_type = msg_type;
        login_name = str_name;
        strncpy(pdu->ca_data, str_name.toStdString().c_str(), 32);
        strncpy(pdu->ca_data + 32, str_pwd.toStdString().c_str(), 32);
        m_tcpsocket.write((char *)pdu, pdu->pdu_len);
        free(pdu);
        pdu = NULL;
    }
}

void TcpClient::HandleRegistRespond(PDU *pdu)
{
    if(strcmp(pdu->ca_data, Register_Success) == 0)
    {
        QMessageBox::information(this, "Register", Register_Success);
    }
    else
    {
        QMessageBox::critical(this, "Register", Register_Fail);
    }
}

void TcpClient::HandleLoginRespond(PDU *pdu)
{
    if(strcmp(pdu->ca_data, Login_Success) == 0)
    {
        current_path = QString("./File/%1").arg(login_name);
        QMessageBox::information(this, "Login", Login_Success);
        MainWindow::ins().show();
        this->hide();
    }
    else
    {
        QMessageBox::critical(this, "Login", Login_Fail);
    }
}

void TcpClient::HandleSearchUserRespond(PDU *pdu)
{
    if(strcmp(pdu->ca_data, Search_User_fail) == 0)
    {
        QMessageBox::information(this, "查找用户", QString("%1 does not exist!").arg(FriendChat::ins().GetSerachName()));
    }
    else
    {
        if(strcmp(pdu->ca_data, Search_User_Online) == 0)
        {
            QMessageBox::information(this, "查找用户", QString("%1 online!").arg(FriendChat::ins().GetSerachName()));
        }
        else
        {
            QMessageBox::information(this, "查找用户", QString("%1 offline!").arg(FriendChat::ins().GetSerachName()));
        }
    }
}

void TcpClient::HandleAddUserRequest(PDU *pdu)
{
    int res = QMessageBox::information(this, "添加好友请求", QString("%1 请求添加你为好友").arg(pdu->ca_data), QMessageBox::Ok, QMessageBox::No);
    PDU *res_pdu= mkPDU(0);
    if(res == QMessageBox::Ok)
    {
        res_pdu->msg_type = MSG_TYPE_AGREE_ADD_USER;
    }
    else
    {
        res_pdu->msg_type = MSG_TYPE_REFUSE_ADD_USER;
    }
    strcpy(res_pdu->ca_data, pdu->ca_data);
    m_tcpsocket.write((char *)res_pdu, res_pdu->pdu_len);
    free(res_pdu);
    res_pdu = NULL;

}

void TcpClient::HandlePrivateChatRequest(PDU *pdu)
{
    char friend_name[32] = {'\0'};
    strncpy(friend_name, pdu->ca_data, 32);
    PrivateChat *m_privatechat = FriendChat::ins().GetPrivateChatByFriendName(friend_name);
    if(m_privatechat == NULL)
    {
        m_privatechat = new PrivateChat;
        m_privatechat->SetFriendName(friend_name);
        m_privatechat->SetLoginName(TcpClient::ins().GetLoginName());
        QString title = TcpClient::ins().GetLoginName();
        title += " to ";
        title += friend_name;
        m_privatechat->SetTitle(title);
        FriendChat::ins().InsertPrivateChat(m_privatechat);
        //m_privatechat_lsit.append(m_privatechat);

    }

    m_privatechat->GetMsgInfo()->append(QString("%1:%2").arg(friend_name).arg((char *)(pdu->ca_msg)));
    if(m_privatechat->isHidden())
        m_privatechat->show();
    if(m_privatechat->isMinimized())
        m_privatechat->showNormal();
}

void TcpClient::HandleGroupChatRequest(PDU *pdu)
{
    char friend_name[32] = {'\0'};
    strncpy(friend_name, pdu->ca_data, 32);

    FriendChat::ins().GetMsgInfo()->append(QString("%1:%2").arg(friend_name).arg((char *)(pdu->ca_msg)));
    if( FriendChat::ins().isHidden())
        FriendChat::ins().show();
    if(FriendChat::ins().isMinimized())
        FriendChat::ins().showNormal();
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
