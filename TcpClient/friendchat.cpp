#include "friendchat.h"
#include "ui_friendchat.h"
#include "protocol.h"
#include "tcpclient.h"
#include "onlineuser.h"
FriendChat::FriendChat(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FriendChat)
{
    ui->setupUi(this);
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
