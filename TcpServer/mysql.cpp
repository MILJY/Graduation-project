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
    //QString m_sql = QString("select * from userinfo where name = %1 and password = %2").arg(name).arg(pwd);
    QString m_sql = QString("insert into userinfo(name,password) values(\'%1\',\'%2\')").arg(name).arg(pwd);
    QSqlQuery register_query;
    return register_query.exec(m_sql);


}
