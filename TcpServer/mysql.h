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
    bool HandleRegister(const QString name, const QString pwd);
signals:

private:
    QSqlDatabase m_db;

};

#endif // MYSQL_H
