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
    bool HandleRegister(const QString name, const QString pwd);//处理注册操作
    bool HandleLogin(const QString name, const QString pwd);//处理登录操作
    bool HandleClientOffline(const QString name);//处理下线操作
    QStringList HandleSearchOnlineUser();//查询所有在线用户
    int HandleSearchUser(const QString name);//查询用户信息，-1为用户不存在，0为用户存在但不在线，1为用户存在而且在线
signals:

private:
    QSqlDatabase m_db;

};

#endif // MYSQL_H
