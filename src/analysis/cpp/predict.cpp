#include "../hpp/predict.h"

QList<QList<double>> getCharacteristics(QList<int> channelCountArr, int queueCount, double la,
                                        double mu, double nu, double n)
{
    QList<double> avgQueueArr, countServedReqArr;

    for (auto &&channelCount : channelCountArr) {
        qtm::qtm qm(channelCount, queueCount, la, mu, nu, n);
        qm.calc_final_states();
        avgQueueArr.append(qtm::qtm_data::calc_avg_queue(qm));
        countServedReqArr.append(qtm::qtm_data::calc_avg_count_served_req(qm));
    }

    QList<QList<double>> characteristics;
    characteristics.append(countServedReqArr);
    characteristics.append(avgQueueArr);

    return characteristics;
}

QList<QList<double>> getPredict(QList<int> channelCountArr, int queueCount, double la, double mu,
                                double nu, double n)
{
    QList<QList<double>> characteristics =
            getCharacteristics(channelCountArr, queueCount, la, mu, nu, n);

    double nMax = 0, lMin = 20, optimalityCoefficient = 0;
    int nMaxIndex = 0, lMinIndex = 0, optimalityCoefficientIndex = 0;

    for (size_t i = 0; i < channelCountArr.length(); i++) {
        double n = characteristics[0][i];
        double l = characteristics[1][i];
        double coefficient = 0;
        if (l != 0) {
            coefficient = n / l;
        }
        if (n > nMax) {
            nMax = n;
            nMaxIndex = i;
        }
        if (l < lMin) {
            lMin = l;
            lMinIndex = i;
        }
        if (coefficient > optimalityCoefficient) {
            optimalityCoefficient = coefficient;
            optimalityCoefficientIndex = i;
        }
    }

    QList<QList<double>> predict;
    predict.append({
            double(channelCountArr[nMaxIndex]),
            nMax,
            characteristics[1][nMaxIndex],
    });
    predict.append({
            double(channelCountArr[optimalityCoefficientIndex]),
            characteristics[0][optimalityCoefficientIndex],
            characteristics[1][optimalityCoefficientIndex],
    });
    predict.append({
            double(channelCountArr[lMinIndex]),
            characteristics[0][lMinIndex],
            lMin,
    });

    return predict;
}
