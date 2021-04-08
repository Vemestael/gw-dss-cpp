#ifndef PREDICT_H
#define PREDICT_H

#include <qtm-core.hpp>
#include <qtm-data.hpp>

#include <QList>
#include <QString>

QList<QList<double>> getCharacteristics(QList<int> channelCountArr, int queueCount, double la, double mu, double nu=0, double n=-1);

QList<QList<QString>> getPredict(QList<int> channelCountArr, int queueCount, double la, double mu, double nu=0, double n=-1);

#endif // PREDICT_H
