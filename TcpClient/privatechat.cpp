#include "privatechat.h"
#include "ui_privatechat.h"
#include "friendchat.h"
#include "tcpclient.h"
PrivateChat::PrivateChat(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PrivateChat)
{
    ui->setupUi(this);
    ui->private_msg->setReadOnly(true);
    ui->private_msg->setFocusPolicy(Qt::NoFocus);
}
PrivateChat::~PrivateChat()
{
    delete ui;
}

QString PrivateChat::GetFriendName()
{
    return friend_name;
}

void PrivateChat::SetFriendName(QString name)
{
    friend_name = name;
}
void PrivateChat::closeEvent(QCloseEvent *event)
{
    FriendChat::ins().DeletePrivateChat(friend_name);
}
void PrivateChat::SetTitle(QString title)
{
    setWindowTitle(title);
}

void PrivateChat::SetLoginName(QString name)
{
    login_name = name;
}

QTextEdit *PrivateChat::GetMsgInfo()
{
    return ui->private_msg;
}


void PrivateChat::on_send_msg_clicked()
{
    QString msg_content = ui->chat_msg->text();
    if(msg_content.isEmpty())
    {
        QMessageBox::information(this, "提示", "发送内容不能为空!");
    }
    else
    {
        ui->chat_msg->clear();
        ui->private_msg->append(QString("%1:%2").arg(login_name).arg(msg_content));
        PDU *pdu = mkPDU(msg_content.size() + 1);
        pdu->msg_type = MSG_TYPE_PRIVATE_CHAT_REQUEST;
        memcpy(pdu->ca_data, login_name.toStdString().c_str(), login_name.size());
        memcpy(pdu->ca_data + 32, friend_name.toStdString().c_str(), friend_name.size());
        memcpy((char *)(pdu->ca_msg), msg_content.toStdString().c_str(), msg_content.size());
        TcpClient::ins().GetTcpSocket().write((char *)pdu, pdu->pdu_len);
        free(pdu);
        pdu = NULL;
    }
}
