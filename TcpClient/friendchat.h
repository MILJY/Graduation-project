#ifndef FRIENDCHAT_H
#define FRIENDCHAT_H

#include <QWidget>
#include <QInputDialog>
#include <QDebug>
#include <QMessageBox>
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

private slots:
    void on_show_onlineuser_clicked();

    void on_search_user_clicked();

private:
    Ui::FriendChat *ui;
public:
    QString search_name;
};

#endif // FRIENDCHAT_H
