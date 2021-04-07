#ifndef DBAPI_H
#define DBAPI_H

#include <QtSql/QSql>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QtSql/QSqlDatabase>

#include <QDate>
#include <QString>
#include <QList>

class DbApi
{
public:
    DbApi(void)=default;
    ~DbApi(void)=default;
    bool connectToDataBase(QString path);
    void closeDataBase(void);
    QDate getFirstDate(void);
    QDate getLastDate(void);
    QList<QList<int>> getCallsInfoByDate(QDate dateStart, QDate dateEnd);
private:
    QSqlDatabase db;
};

#endif // DBAPI_H
