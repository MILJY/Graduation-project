#include "mysql.h"

MySql::MySql(QObject *parent) : QObject(parent)
{

}

MySql &MySql::ins()
{
    static MySql instance;
    return instance;
}

void MySql::init()
{
    m_db = QSqlDatabase::addDatabase("QMYSQL");
    m_db.setHostName("localhost");
    m_db.setUserName("root");
    m_db.setPassword("123");
    m_db.setDatabaseName("cloud");
    if(m_db.open())
    {
        qDebug()<<"Open DataBase Success!";
    }
    else
    {
        qDebug()<<"Open DataBase Fail!";
    }
}

int MySql::GetIdByName(QString name)
{
    if(name == NULL)
        return -1;
    QString m_sql = QString("select id from userinfo where name = \'%1\'").arg(name);
    QSqlQuery search_query;
    search_query.exec(m_sql);
    if(search_query.next())
    {
        search_query.value(0).toInt();
        return search_query.value(0).toInt();
    }
    else
        return -1;
}

bool MySql::HandleRegister(const QString name, const QString pwd)
{
    if(name == NULL || pwd == NULL)
        return false;

    QString m_sql = QString("select * from userinfo where name = \'%1\'").arg(name);
    QSqlQuery register_query;
    register_query.exec(m_sql);
    if(register_query.next())
    {
        return false;
    }
    else
    {
        m_sql = QString("insert into userinfo(name,password) values(\'%1\',\'%2\')").arg(name).arg(pwd);
        return register_query.exec(m_sql);
    }

}

bool MySql::HandleLogin(const QString name, const QString pwd)
{
    if(name == NULL || pwd == NULL)
        return false;
    QString m_sql = QString("select * from userinfo where name = \'%1\' and password = \'%2\' and online = 0").arg(name).arg(pwd);
    QSqlQuery login_query;
    login_query.exec(m_sql);
    if(login_query.next())
    {
        m_sql = QString("update userinfo set online = 1 where name = \'%1\'").arg(name);
        return login_query.exec(m_sql);
    }
    else
        return false;


}

bool MySql::HandleClientOffline(const QString name)
{
    if(name == NULL)
        return false;
    QString m_sql = QString("update userinfo set online = 0 where name = \'%1\'").arg(name);
    QSqlQuery offline_query;
    return offline_query.exec(m_sql);
}

QStringList MySql::HandleSearchOnlineUser()
{
    QStringList result;
    result.clear();
    QString m_sql = QString("select name from userinfo where online = 1");
    QSqlQuery search_query;
    search_query.exec(m_sql);
    while(search_query.next())
    {
        result.append(search_query.value(0).toString());
    }
    return result;
}

int MySql::HandleSearchUser(const QString name)
{
    if(name == NULL)
        return -1;
    QString m_sql = QString("select online from userinfo where name = \'%1\'").arg(name);
    QSqlQuery search_query;
    search_query.exec(m_sql);
    if(search_query.next())
    {
        search_query.value(0).toInt();
        return search_query.value(0).toInt();
    }
    else
        return -1;
}

int MySql::HandleAddUser(const QString login_name, const QString friend_name)
{
    if(login_name == NULL || friend_name == NULL)
        return -1;

    QString m_sql = QString("select * from userfriend where (id = (select id from userinfo where name = \'%1\') and friendid = (select id from userinfo where name = \'%2\')) or (id = (select id from userinfo where name = \'%3\') and friendid = (select id from userinfo where name = \'%4\'));").arg(login_name).arg(friend_name).arg(friend_name).arg(login_name);
    QSqlQuery search_query;
    search_query.exec(m_sql);
    if(search_query.next())
    {
        return 0;
    }
    else
    {
        m_sql = QString("select online from userinfo where name = \'%1\'").arg(friend_name);
        search_query.exec(m_sql);
        if(search_query.next())
        {
            int is_online = search_query.value(0).toInt();
            return is_online + 2;
        }
        else
            return 1;
    }
}

void MySql::HandleAgreeAddUser(const QString login_name, const QString friend_name)
{
    if(login_name == NULL || friend_name == NULL)
        return ;
    int login_name_id = GetIdByName(login_name);
    int friend_name_id = GetIdByName(friend_name);
    QString m_sql = QString("insert into userfriend(id,friendid) values(%1,%2)").arg(login_name_id).arg(friend_name_id);
    QSqlQuery query;
    query.exec(m_sql);
}

QStringList MySql::HandleFlushFriendList(const QString name)
{

    QStringList result;
    result.clear();
    if(name == NULL)
        return result;
    int name_id = GetIdByName(name);
    QString m_sql = QString("select name from userinfo where id in (select id from userfriend where friendid = %1) UNION (select name from userinfo where id in (select friendid from userfriend where id = %2))").arg(name_id).arg(name_id);
    QSqlQuery query;
    query.exec(m_sql);
    while(query.next())
    {
        result.append(query.value(0).toString());
    }
    return result;
}

QStringList MySql::HandleAllOnlineFriend(const QString name)
{
    QStringList result;
    result.clear();
    if(name == NULL)
        return result;
    int name_id = GetIdByName(name);
    QString m_sql = QString("select name from userinfo where ((online = 1 ) and (id in (select id from userfriend where friendid = %1))) UNION (select name from userinfo where ((online = 1 ) and (id in (select friendid from userfriend where id = %2))))").arg(name_id).arg(name_id);
    QSqlQuery query;
    query.exec(m_sql);
    while(query.next())
    {
        result.append(query.value(0).toString());
    }
    return result;
}

bool MySql::HandleDeleteFriend(const QString login_name, const QString friend_name)
{
    if(login_name == NULL || friend_name == NULL)
        return false;
    int login_name_id = GetIdByName(login_name);
    int friend_name_id = GetIdByName(friend_name);
    QString m_sql = QString("delete from userfriend where id in (%1,%2) and friendid in (%3,%4)").arg(login_name_id).arg(friend_name_id).arg(friend_name_id).arg(login_name_id);
    QSqlQuery query;
    return query.exec(m_sql);
}

int MySql::HandleUserIsOnline(const QString name)
{
    return HandleSearchUser(name);

}
