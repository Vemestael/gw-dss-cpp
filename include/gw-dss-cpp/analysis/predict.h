#pragma once
#ifndef PREDICT_H
#define PREDICT_H

#include <QVector>
#include <QString>

#include <qtm-calc/core.hpp>
#include <qtm-calc/data.hpp>

class Predict
{
public:
    static QVector<QVector<double>> getCharacteristics(QVector<unsigned> channelCountArr,
                                                       unsigned queueCount, double la, double mu,
                                                       double nu = 0, double n = -1);
};

#endif // PREDICT_H
