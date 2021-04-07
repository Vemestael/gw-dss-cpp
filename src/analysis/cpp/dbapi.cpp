#include "../hpp/dbapi.h"

bool DbApi::connectToDataBase(QString path)
{
    this->db = QSqlDatabase::addDatabase("QSQLITE");
    this->db.setDatabaseName(path);
    return this->db.open();
}

void DbApi::closeDataBase(void)
{
    this->db.close();
}

QDate DbApi::getFirstDate(void)
{
    QSqlQuery query;
    query.exec("select date from Call_table order by id limit 1");
    query.next();
    return query.value(0).toDate();
}

QDate DbApi::getLastDate(void)
{
    QSqlQuery query;
    query.exec("select date from Call_table order by id desc limit 1");
    query.next();
    return query.value(0).toDate();
}

QList<QList<int>> DbApi::getCallsInfoByDate(QDate dateStart, QDate dateEnd)
{
    //    dateStart.toString("%Y-%m-%d");
    QDate firstDate = this->getFirstDate();
    QDate lastDate = this->getLastDate();

    if (dateEnd < lastDate)
    {
        lastDate = dateEnd;
    }
    if (dateStart > firstDate)
    {
        firstDate = dateStart;
    }
    int daysCount = firstDate.daysTo(lastDate);
    int weekCount;
    if (daysCount % 7 == 0)
    {
        weekCount = daysCount / 7;
    }else
    {
        weekCount = daysCount / 7 + 1;
    }
    QString queryText;
    queryText += "select strftime('%w', date) WeekDayNumber, ";
    queryText += "strftime('%H', date) HourNumber, ";
    queryText += "count(id)/" + QString("%1").arg(weekCount) + " ";
    queryText += "from Call_table ";
    queryText += "where (date BETWEEN '" + dateStart.toString(Qt::ISODate) + "' AND '" + dateEnd.toString(Qt::ISODate) + "') ";
    queryText += "group by WeekDayNumber, HourNumber";
    QList<QList<int>> results;
    QSqlQuery query;
    query.exec(queryText);
    while(query.next()){
        QList<int> result;
        result.append(query.value(0).toInt());
        result.append(query.value(1).toInt());
        result.append(query.value(2).toInt());
        results.append(result);
    }
    return results;
}
