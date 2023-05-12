#include "onlineuser.h"
#include "ui_onlineuser.h"
#include "tcpclient.h"
OnlineUser::OnlineUser(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OnlineUser)
{
    ui->setupUi(this);
    QLabel *background = new QLabel(this);
    // 加载图片并设置背景
    background->setPixmap(QPixmap(":/config/icon/222.jpg"));
    // 设置背景图片的拉伸方式
    background->setScaledContents(true);
    // 将背景设置为窗口底层
    background->lower();
    setWindowTitle(TcpClient::ins().GetLoginName());
}

OnlineUser::~OnlineUser()
{
    delete ui;
}

OnlineUser &OnlineUser::ins()
{
    static OnlineUser instance;
    return instance;
}

void OnlineUser::SHowOnlineUser(PDU *pdu)
{
    if(pdu == NULL)
        return;
    uint user_cnt = pdu->msg_len / 32;
    char user_name[32];
    ui->online_user->clear();
    for(uint i = 0; i < user_cnt; i++)
    {
        memcpy(user_name, (char *)(pdu->ca_msg) + i * 32, 32);

        ui->online_user->addItem(user_name);
    }
}

void OnlineUser::on_add_friend_clicked()
{
    QListWidgetItem *item = ui->online_user->currentItem();
    if(item == NULL)
    {
        QMessageBox::information(this, "提示", "请选择要添加的好友");
    }
    else
    {
        QString friend_name = item->text();
        if(friend_name == TcpClient::ins().GetLoginName())
        {
            QMessageBox::critical(this, "错误", "不能添加自己，操作失败！");
        }
        else
        {
            QString login_name = TcpClient::ins().GetLoginName();
            PDU *pdu = mkPDU(0);
            pdu->msg_type = MSG_TYPE_ADD_USER_REQUEST;
            memcpy(pdu->ca_data, login_name.toStdString().c_str(), login_name.size());
            memcpy(pdu->ca_data + 32, friend_name.toStdString().c_str(), friend_name.size());
            TcpClient::ins().GetTcpSocket().write((char *)pdu, pdu->pdu_len);
            free(pdu);
            pdu = NULL;
        }
    }

    hide();
}
