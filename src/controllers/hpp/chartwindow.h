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

enum class ChartType
{
    Timescale, WeekDays, ByShifts, ByHours
};

class ChartWindow : public QWidget
{
    Q_OBJECT

public:
    explicit ChartWindow(ChartType type, QVector<QVector<double>> data, QWidget *parent = nullptr);
    ~ChartWindow(void);

private:
    QGridLayout *layout;
    QCustomPlot *customPlot;
    QCPGraph *graphic;


    void initPlot(ChartType type, QVector<QVector<double>> data);
};

#endif // CHARTWINDOW_H
