#include "../hpp/dataprocessing.h"

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
