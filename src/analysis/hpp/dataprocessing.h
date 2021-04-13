#pragma once
#ifndef DATAPROCESSING_H
#define DATAPROCESSING_H

#include "./dbapi.h"

class DataProcessing
{
public:
    static QVector<QVector<double>> getCountOfCallsByShift(QVector<QVector<double>> inputData);
};

#endif // DATAPROCESSING_H
