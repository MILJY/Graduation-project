#ifndef RRIVATECHAT_H
#define RRIVATECHAT_H
#include <QTextEdit>
#include <QWidget>
#include <QLabel>
#include <QDebug>
namespace Ui {
class PrivateChat;
}

class PrivateChat : public QWidget
{
    Q_OBJECT

public:
    explicit PrivateChat(QWidget *parent = nullptr);
    ~PrivateChat();
    QString GetFriendName();
    QString GetLoginName();
    void SetFriendName(QString name);
    void SetTitle(QString title);
    void SetLoginName(QString name);
    QTextEdit *GetMsgInfo();
    void closeEvent(QCloseEvent *event);

private slots:
    void on_send_msg_clicked();

private:
    Ui::PrivateChat *ui;
    QString friend_name;
    QString login_name;
};

#endif // RRIVATECHAT_H
