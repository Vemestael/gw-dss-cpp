#pragma once
#ifndef PREDICT_H
#define PREDICT_H

#include <qtm-core.hpp>
#include <qtm-data.hpp>

#include <QVector>
#include <QString>

class Predict
{
private:
    static QVector<QVector<double>> getCharacteristics(QVector<unsigned> channelCountArr, unsigned queueCount,
                                                   double la, double mu, double nu = 0,
                                                   double n = -1);
public:
    static QVector<QVector<double>> getPredict(QVector<unsigned> channelCountArr, unsigned queueCount, double la,
                                           double mu, double nu = 0, double n = -1);
};

#endif // PREDICT_H
