#pragma once
#ifndef DATAPROCESSING_H
#define DATAPROCESSING_H

#include "./dbapi.h"

QList<QList<double>> getCountOfCallsByShift(QList<QList<double>> inputData);

#endif // DATAPROCESSING_H
