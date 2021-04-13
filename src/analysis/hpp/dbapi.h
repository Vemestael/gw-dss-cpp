#pragma once
#ifndef DBAPI_H
#define DBAPI_H

#include <QtSql/QSql>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QtSql/QSqlDatabase>

#include <QDate>
#include <QVariant>
#include <QString>
#include <QList>
#include <QStringList>
#include <QErrorMessage>

class DbApi
{
public:
    DbApi(void) = default;
    ~DbApi(void) = default;
    void connectToDataBase(QString path);
    void closeDataBase(void);
    bool isEmptyTable(void);
    QDate getFirstDate(void);
    QDate getLastDate(void);
    QList<QList<double>> getCallsInfoByDate(QDate dateStart, QDate dateEnd);

private:
    QSqlDatabase db;
    bool isValidDataBase(void);
};

#endif // DBAPI_H
