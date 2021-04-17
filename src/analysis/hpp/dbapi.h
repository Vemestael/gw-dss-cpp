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
#include <QVector>
#include <QStringList>
#include <QErrorMessage>

class DbApi
{
public:
    DbApi(void) = default;
    ~DbApi(void) = default;

    void connectToDataBase(QString const &path);
    void disconnectFromDataBase(void);

    bool isEmptyTable(void) const;

    QDate getFirstDate(void) const;
    QDate getLastDate(void) const;
    QVector<QVector<double>> getCallsCountsByDate(QDate const &dateStart,
                                                  QDate const &dateEnd) const;
    QVector<QVector<double>> getCallsInfoByDate(QDate const &dateStart, QDate const &dateEnd) const;

private:
    QSqlDatabase db;

    bool isValidDataBase(void) const;
};

#endif // DBAPI_H
