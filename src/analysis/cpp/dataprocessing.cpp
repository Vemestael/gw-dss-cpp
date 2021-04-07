#include "../hpp/dataprocessing.h"

QList<QList<double>> getCountOfCallsByShift(QList<QList<double>> inputData)
{
    QList<QList<double>> days;
    double shift = 0;
    QList<double> shift_avg;
    for (size_t i = 1; i < inputData.length(); i++)
    {
        shift += inputData[i-1][2];
        if (i % 8 == 0)
        {
            shift_avg.append(shift/8);
            shift = 0;
        }
        if (i % 24 == 0){
            days.append(shift_avg);
            shift_avg.clear();
        }
    }
    return days;
}
