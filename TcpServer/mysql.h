#ifndef MYSQL_H
#define MYSQL_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>
#include <QMessageBox>

class MySql : public QObject
{
    Q_OBJECT
public:
    explicit MySql(QObject *parent = nullptr);
    static MySql& ins();
    void init();
    int GetIdByName(QString name);
    bool HandleRegister(const QString name, const QString pwd);//处理注册操作
    bool HandleLogin(const QString name, const QString pwd);//处理登录操作
    bool HandleClientOffline(const QString name);//处理下线操作
    QStringList HandleSearchOnlineUser();//查询所有在线用户
    int HandleSearchUser(const QString name);//查询用户信息，-1为用户不存在，0为用户存在但不在线，1为用户存在而且在线
    int HandleAddUser(const QString login_name, const QString friend_name);//添加好友, -1为系统问题，0为已经是好友，1为对方不存在，2为对方不在线，3为对方在线
    void HandleAgreeAddUser(const QString login_name, const QString friend_name);//同意添加好友
    QStringList HandleFlushFriendList(const QString name);//刷新好友列表
    QStringList HandleAllOnlineFriend(const QString name);//查找所有在线好友
    bool HandleDeleteFriend(const QString login_name, const QString friend_name);//删除好友
    int HandleUserIsOnline(const QString name);//判断用户是否在线
    void HandleLogout(const QString name);//处理注销操作
signals:

private:
    QSqlDatabase m_db;

};

#endif // MYSQL_H
