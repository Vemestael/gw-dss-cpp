#pragma once
#ifndef CHARTWINDOW_H
#define CHARTWINDOW_H

#include <algorithm>
#include <numeric>

#include <QWidget>
#include <QGridLayout>
#include <QVector>
#include <QString>
#include <QDateTime>

#include <qcustomplot.h>

enum class ChartType { Timescale, WeekDays, ByShifts, ByHours };

static QVector<double> genRange(int lower, int upper);

class ChartWindow : public QWidget
{
    Q_OBJECT

public:
    explicit ChartWindow(ChartType type, QVector<QVector<double>> data, QWidget *parent = nullptr);
    ~ChartWindow(void);

private:
    QGridLayout *layout;
    QCustomPlot *customPlot;

    void plot(ChartType type, QVector<QVector<double>> data);
    void plotTimeDataGraph(QSharedPointer<QCPAxisTickerDateTime> dateTicker,
                           QVector<QVector<double>> data);
    void plotHistogram(QSharedPointer<QCPAxisTickerText> xTicker, QVector<QVector<double>> data);
    void decorate(bool zoomH, bool zoomV, bool dragH, bool dragV, bool grid, double labelAngle);
};

#endif // CHARTWINDOW_H
