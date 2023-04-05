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
    qDebug() << name;
    if(name == NULL)
        return false;
    QString m_sql = QString("update userinfo set online = 0 where name = \'%1\'").arg(name);
    QSqlQuery offline_query;
    return offline_query.exec(m_sql);
}
