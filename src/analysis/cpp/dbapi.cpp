#include "../hpp/dbapi.h"

void DbApi::connectToDataBase(QString const &path)
{
    if (!this->db.isOpen()) {
        this->db = QSqlDatabase::addDatabase("QSQLITE");
        this->db.setDatabaseName(path);
        this->db.open();
        if (!this->isValidDataBase()) {
            this->disconnectFromDataBase();
            QErrorMessage().showMessage(QErrorMessage::tr("Invalid database"));
        }
    }
}

void DbApi::disconnectFromDataBase(void)
{
    this->db.close();
}

bool DbApi::isEmptyTable(void) const
{
    QSqlQuery query;
    query.exec("select count(id) from Call_table");
    query.next();
    return !query.value(0).toBool();
}

QDate DbApi::getFirstDate(void) const
{
    QSqlQuery query;
    query.exec("select date from Call_table order by id limit 1");
    query.next();
    return query.value(0).toDate();
}

QDate DbApi::getLastDate(void) const
{
    QSqlQuery query;
    query.exec("select date from Call_table order by id desc limit 1");
    query.next();
    return query.value(0).toDate();
}

QVector<QVector<double>> DbApi::getCallsCountsByDate(QDate const &dateStart, QDate const &dateEnd) const
{
    QDate firstDate = this->getFirstDate();
    QDate lastDate = this->getLastDate();

    if (dateEnd < lastDate) {
        lastDate = dateEnd;
    }
    if (dateStart > firstDate) {
        firstDate = dateStart;
    }

    int daysCount = firstDate.daysTo(lastDate);
    int weekCount;
    if (daysCount % 7 == 0) {
        weekCount = daysCount / 7;
    } else {
        weekCount = daysCount / 7 + 1;
    }

    QString queryText;
    queryText += "select strftime('%w', date) WeekDayNumber, ";
    queryText += "strftime('%H', date) HourNumber, ";
    queryText += "count(id)/" + QString("%1").arg(weekCount) + " ";
    queryText += "from Call_table ";
    queryText += "where (date BETWEEN '" + dateStart.toString(Qt::ISODate) + "' AND '"
            + dateEnd.toString(Qt::ISODate) + "') ";
    queryText += "group by WeekDayNumber, HourNumber";

    QSqlQuery query;
    query.exec(queryText);

    QVector<QVector<double>> results;
    while (query.next()) {
        results.append({ query.value(0).toDouble(), query.value(1).toDouble(),
                         query.value(2).toDouble() });
    }

    return results;
}

QVector<QVector<double>> DbApi::getCallsInfoByDate(QDate const &dateStart, QDate const &dateEnd) const
{
    QDate firstDate = this->getFirstDate();
    QDate lastDate = this->getLastDate();

    if (dateEnd < lastDate) {
        lastDate = dateEnd;
    }
    if (dateStart > firstDate) {
        firstDate = dateStart;
    }

    QString queryText;
    queryText += "select strftime('%Y-%m-%d %H', date) Day, ";
    queryText += "count(id) ";
    queryText += "from Call_table ";
    queryText += "where (date BETWEEN '" + dateStart.toString(Qt::ISODate) + "' AND '"
            + dateEnd.toString(Qt::ISODate) + "') ";
    queryText += "group by Day";

    QSqlQuery query;
    query.exec(queryText);

    QVector<double> date;
    QVector<double> avgs;
    while (query.next()) {
        date.append(query.value(0).toDateTime().toSecsSinceEpoch());
        avgs.append(query.value(1).toDouble());
    }

    return { date, avgs };
}

bool DbApi::isValidDataBase(void) const
{
    return this->db.tables(QSql::Tables).contains("Call_table");
}
