#ifndef FRIENDCHAT_H
#define FRIENDCHAT_H

#include <QWidget>
#include <QInputDialog>
#include <QDebug>
#include <QMessageBox>
#include <QList>
#include "protocol.h"
#include "privatechat.h"
namespace Ui {
class FriendChat;
}

class FriendChat : public QWidget
{
    Q_OBJECT

public:
    explicit FriendChat(QWidget *parent = nullptr);
    ~FriendChat();
    static FriendChat& ins();
    QString GetSerachName();
    void FlushFriendList(PDU *pdu);
    void HandlePrivateChat(PDU *pdu);
    PrivateChat * GetPrivateChatByFriendName(const QString friend_name);
    void DeletePrivateChat(const QString friend_name);
    void InsertPrivateChat(PrivateChat *m_privatechat);
    QTextEdit *GetMsgInfo();
private slots:
    void on_show_onlineuser_clicked();

    void on_search_user_clicked();

    void on_refresh_users_clicked();

    void on_delete_friend_clicked();

    void on_private_chat_clicked();

    void on_send_msg_clicked();

private:
    Ui::FriendChat *ui;
    QString search_name;
    QList<PrivateChat *>m_privatechat_lsit;
};

#endif // FRIENDCHAT_H
