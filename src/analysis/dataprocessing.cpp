#include <gw-dss-cpp/analysis/dataprocessing.h>

QVector<QVector<double>>
DataProcessing::getCountOfCallsByWeekDay(QVector<QVector<double>> inputData)
{
    QVector<QVector<double>> days;
    double day = 0;
    QVector<double> dayAvg;

    for (size_t i = 1; i <= inputData.length(); ++i) {
        day += inputData[i - 1][2];
        if (i % 24 == 0) {
            dayAvg.append(day);
            days.append(dayAvg);
            dayAvg.clear();
            day = 0;
        }
    }

    return days;
}

QVector<QVector<double>> DataProcessing::getCountOfCallsByShift(QVector<QVector<double>> inputData)
{
    QVector<QVector<double>> days;
    QVector<double> shiftAvg;
    double shift = 0;

    for (size_t i = 1; i <= inputData.length(); ++i) {
        shift += inputData[i - 1][2];
        if (i % 8 == 0) {
            shiftAvg.append(shift / 8);
            shift = 0;
        }
        if (i % 24 == 0) {
            days.append(shiftAvg);
            shiftAvg.clear();
        }
    }

    return days;
}

QVector<QVector<double>> DataProcessing::getCountOfCallsByHour(QVector<QVector<double>> inputData)
{
    QVector<QVector<double>> days;
    QVector<double> day;
    double shift = 0;

    for (size_t i = 1; i <= inputData.length(); ++i) {
        day.append(inputData[i - 1][2]);
        if (i % 24 == 0) {
            days.append(day);
            day.clear();
        }
    }

    return days;
}
