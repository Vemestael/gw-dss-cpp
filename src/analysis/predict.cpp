#include <gw-dss-cpp/analysis/predict.h>

QVector<QVector<double>> Predict::getCharacteristics(QVector<unsigned> channelCountArr,
                                                     unsigned queueCount, double la, double mu,
                                                     double nu, double n)
{
    QVector<double> avgQueueArr, countServedReqArr, countUnservedReqArr;

    for (auto &&channelCount : channelCountArr) {
        qtm::qtm qm(channelCount, queueCount, la, mu, nu, n);
        qm.calc_final_states();
        countServedReqArr.append(qtm::qtm_data::calc_avg_count_served_req(qm) * 8);
        countUnservedReqArr.append(qtm::qtm_data::calc_avg_count_unserved_req(qm));
        avgQueueArr.append(qtm::qtm_data::calc_avg_queue(qm));
    }

    QVector<QVector<double>> characteristics;
    characteristics.append(countServedReqArr);
    characteristics.append(countUnservedReqArr);
    characteristics.append(avgQueueArr);

    return characteristics;
}