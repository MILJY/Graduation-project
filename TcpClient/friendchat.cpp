#include "friendchat.h"
#include "ui_friendchat.h"
#include "tcpclient.h"
#include "onlineuser.h"
FriendChat::FriendChat(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FriendChat)
{
    ui->setupUi(this);
    QLabel *background = new QLabel(this);
        // 加载图片并设置背景
    background->setPixmap(QPixmap(":/config/icon/222.jpg"));
        // 设置背景图片的拉伸方式
    background->setScaledContents(true);
        // 将背景设置为窗口底层
    background->lower();
    ui->all_msg->setReadOnly(true);
    ui->all_msg->setFocusPolicy(Qt::NoFocus);

    setWindowTitle(TcpClient::ins().GetLoginName());
}

FriendChat::~FriendChat()
{
    delete ui;
}

FriendChat &FriendChat::ins()
{
    static FriendChat instance;
    return instance;
}

QString FriendChat::GetSerachName()
{
    return search_name;
}

void FriendChat::FlushFriendList(PDU *pdu)
{
    if(pdu == NULL)
        return;
    uint user_cnt = pdu->msg_len / 32;
    char friend_name[32];
    ui->friend_list->clear();
    for(uint i = 0; i < user_cnt; i++)
    {
        memcpy(friend_name, (char *)(pdu->ca_msg) + i * 32, 32);

        ui->friend_list->addItem(friend_name);
    }
}

void FriendChat::HandlePrivateChat(PDU *pdu)
{
    char result[32] = {'\0'};
    char friend_name[32] = {'\0'};
    strncpy(result, pdu->ca_data, 32);
    strncpy(friend_name, pdu->ca_data + 32, 32);
    if(strcmp(result, Search_User_Offline) == 0)
    {
        QMessageBox::information(this, "提示", QString("%1 不在线，无法进行聊天").arg(friend_name));
    }
    else
    {
        if(strcmp(result, Search_User_fail) == 0)
        {
            QMessageBox::information(this, "提示", QString("%1 已注销").arg(friend_name));
        }
        else
        {
            PrivateChat *m_privatechat = GetPrivateChatByFriendName(friend_name);
            if(m_privatechat == NULL)
            {
                m_privatechat = new PrivateChat;
                m_privatechat->SetFriendName(friend_name);
                m_privatechat->SetLoginName(TcpClient::ins().GetLoginName());
                QString title = TcpClient::ins().GetLoginName();
                title += " to ";
                title += friend_name;
                m_privatechat->SetTitle(title);
                qDebug() << m_privatechat_lsit.size();
                m_privatechat_lsit.append(m_privatechat);
                qDebug() << m_privatechat_lsit.size();
            }
            if(m_privatechat->isHidden())
                m_privatechat->show();
            if(m_privatechat->isMinimized())
                m_privatechat->showNormal();
        }
    }

}

PrivateChat *FriendChat::GetPrivateChatByFriendName(const QString friend_name)
{
    QList<PrivateChat *>::iterator it = m_privatechat_lsit.begin();
    for(; it != m_privatechat_lsit.end(); it++)
    {
        if((*it)->GetFriendName() == friend_name)
        {
            return *it;
        }
    }
    return NULL;
}

void FriendChat::DeletePrivateChat(const QString friend_name)
{
    QList<PrivateChat *>::iterator it = m_privatechat_lsit.begin();
    for(; it != m_privatechat_lsit.end(); it++)
    {
        if((*it)->GetFriendName() == friend_name)
        {
            (*it)->deleteLater();
             *it = NULL;
             m_privatechat_lsit.erase(it);
            break;
        }
    }
}

void FriendChat::InsertPrivateChat(PrivateChat *m_privatechat)
{
    m_privatechat_lsit.append(m_privatechat);
}

QTextEdit *FriendChat::GetMsgInfo()
{
    return ui->all_msg;
}

void FriendChat::on_show_onlineuser_clicked()
{
    PDU *pdu = mkPDU(0);
    pdu->msg_type = MSG_TYPE_SEARCH_ONLINE_USER_REQUEST;
    TcpClient::ins().GetTcpSocket().write((char *)pdu, pdu->pdu_len);
    free(pdu);
    pdu = NULL;
    OnlineUser::ins().show();
}

void FriendChat::on_search_user_clicked()
{
    search_name = QInputDialog::getText(this, "搜索", "用户名：");
    if(search_name.isEmpty())
    {
        QMessageBox::critical(this, "error", "用户名不能为空！");
    }
    else
    {
        PDU *pdu = mkPDU(0);
        pdu->msg_type = MSG_TYPE_SERACH_USER_REQUEST;
        memcpy(pdu->ca_data, search_name.toStdString().c_str(), search_name.size());
        TcpClient::ins().GetTcpSocket().write((char *)pdu, pdu->pdu_len);
        free(pdu);
        pdu = NULL;
        qDebug() << search_name;
    }

}

void FriendChat::on_refresh_users_clicked()
{
    PDU *pdu = mkPDU(0);
    pdu->msg_type = MSG_TYPE_FLUSH_FRIEND_LIST_REQUEST;
    memcpy(pdu->ca_data, TcpClient::ins().GetLoginName().toStdString().c_str(), TcpClient::ins().GetLoginName().size());
    TcpClient::ins().GetTcpSocket().write((char *)pdu, pdu->pdu_len);
    free(pdu);
    pdu = NULL;
}

void FriendChat::on_delete_friend_clicked()
{
    QListWidgetItem *item = ui->friend_list->currentItem();
    if(item == NULL)
    {
        QMessageBox::information(this, "提示", "请选择要删除的好友");
    }
    else
    {
        QString friend_name = item->text();

        QString login_name = TcpClient::ins().GetLoginName();
        PDU *pdu = mkPDU(0);
        pdu->msg_type = MSG_TYPE_DELETE_FRIEND_REQUEST;
        memcpy(pdu->ca_data, login_name.toStdString().c_str(), login_name.size());
        memcpy(pdu->ca_data + 32, friend_name.toStdString().c_str(), friend_name.size());
        TcpClient::ins().GetTcpSocket().write((char *)pdu, pdu->pdu_len);
        free(pdu);
        pdu = NULL;
    }

}

void FriendChat::on_private_chat_clicked()
{
    QListWidgetItem *item = ui->friend_list->currentItem();
    if(item == NULL)
    {
        QMessageBox::information(this, "提示", "请选择要私聊的好友");
    }
    else
    {
        QString friend_name = item->text();
        PDU *pdu = mkPDU(0);
        pdu->msg_type = MSG_TYPE_USER_ONLINE_REQUEST;
        memcpy(pdu->ca_data, friend_name.toStdString().c_str(), friend_name.size());
        TcpClient::ins().GetTcpSocket().write((char *)pdu, pdu->pdu_len);
        free(pdu);
        pdu = NULL;
    }
}

void FriendChat::on_send_msg_clicked()
{
    QString msg_content = ui->send_msg_content->text();
    if(msg_content.isEmpty())
    {
        QMessageBox::information(this, "提示", "发送内容不能为空!");
    }
    else
    {
        ui->send_msg_content->clear();
        ui->all_msg->append(QString("%1:%2").arg(TcpClient::ins().GetLoginName()).arg(msg_content));
        PDU *pdu = mkPDU(msg_content.size() + 1);
        pdu->msg_type = MSG_TYPE_GROUP_CHAT_REQUEST;
        memcpy(pdu->ca_data, TcpClient::ins().GetLoginName().toStdString().c_str(), TcpClient::ins().GetLoginName().size());
        memcpy((char *)(pdu->ca_msg), msg_content.toStdString().c_str(), msg_content.size());
        TcpClient::ins().GetTcpSocket().write((char *)pdu, pdu->pdu_len);
        free(pdu);
        pdu = NULL;
    }
}
