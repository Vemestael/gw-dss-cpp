#pragma once
#ifndef DATAPROCESSING_H
#define DATAPROCESSING_H

#include "./dbapi.h"

class DataProcessing
{
public:
    static QVector<QVector<double>> getCountOfCallsByWeekDay(QVector<QVector<double>> inputData);
    static QVector<QVector<double>> getCountOfCallsByShift(QVector<QVector<double>> inputData);
    static QVector<QVector<double>> getCountOfCallsByHour(QVector<QVector<double>> inputData);
};

#endif // DATAPROCESSING_H
