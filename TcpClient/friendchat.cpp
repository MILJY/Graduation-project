#include "friendchat.h"
#include "ui_friendchat.h"
#include "tcpclient.h"
#include "onlineuser.h"
FriendChat::FriendChat(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FriendChat)
{
    ui->setupUi(this);
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
