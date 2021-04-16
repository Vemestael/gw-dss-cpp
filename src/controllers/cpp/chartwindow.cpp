#include "../hpp/chartwindow.h"

ChartWindow::ChartWindow(ChartType type, QVector<QVector<double>> data, QWidget *parent)
: QWidget(parent)
{
    this->setBaseSize(800, 600);
    this->setMinimumSize(800, 600);
    this->layout = new QGridLayout(this);
    this->initPlot(type, data);
};

ChartWindow::~ChartWindow(void)
{
    delete this->graphic;
    delete this->customPlot;
    delete this->layout;
};

void ChartWindow::initPlot(ChartType type, QVector<QVector<double>> data)
{
    this->customPlot = new QCustomPlot();
    this->layout->addWidget(this->customPlot, 0, 0, 1, 1);
    QCPBars *bar = new QCPBars(this->customPlot->xAxis, this->customPlot->yAxis);
    QVector<double> ticks;
    QVector<QString> labels;
    QVector<QString> weekDays = {tr("Mon"), tr("Tue"), tr("Wed"), tr("Thu"), tr("Fri"), tr("Sat"), tr("Sun")};
    QSharedPointer<QCPAxisTickerText> textTicker(new QCPAxisTickerText);
    switch(type) {
        case ChartType::Timescale:
            this->customPlot->yAxis->setLabel(tr("Timescale load"));
            break;
        case ChartType::WeekDays:
            this->customPlot->yAxis->setLabel(tr("Load by week days"));
            ticks.resize(7);
            labels = weekDays;
            break;
        case ChartType::ByShifts:
            this->customPlot->yAxis->setLabel(tr("Load by work shifts"));
            ticks.resize(21);
            for(auto&& weekDay : weekDays)
            {
                for(size_t i = 0; i < 3; ++i)
                {
                    labels.append(weekDay + QString(" %1").arg(i + 1));
                }
            }
            break;
        case ChartType::ByHours:
            this->customPlot->yAxis->setLabel(tr("Load by hours"));
            ticks.resize(168);
            for(auto&& weekDay : weekDays)
            {
                for(size_t i = 0; i < 24; ++i)
                {
                    labels.append(weekDay + QString(" %1").arg(i + 1));
                }
            }
            break;
    }
    bar->setAntialiased(false);
    std::iota(ticks.begin(), ticks.end(), 1);
    textTicker->addTicks(ticks, labels);
    this->customPlot->xAxis->setTicker(textTicker);
    this->customPlot->xAxis->setTickLabelRotation(60);
    this->customPlot->xAxis->setSubTicks(false);
    this->customPlot->xAxis->setTickLength(0, 4);
    this->customPlot->xAxis->grid()->setVisible(true);
    this->customPlot->yAxis->grid()->setSubGridVisible(true);
    this->customPlot->setInteraction(QCP::iRangeZoom, true);
    this->customPlot->setInteraction(QCP::iRangeDrag, true);
    this->customPlot->axisRect()->setRangeDrag(Qt::Horizontal);
    this->customPlot->axisRect()->setRangeZoom(Qt::Horizontal);

    if(type != ChartType::Timescale){
        QVector<double> barData;
        for(QVector<double> dataEl : data)
        {
            for(double dataVal : dataEl)
            {
                barData.push_back(dataVal);
            }
        }

        this->customPlot->xAxis->setRange(0, barData.length());
        this->customPlot->yAxis->setRange(*std::min_element(barData.begin(), barData.end()) * 0.9, *std::max_element(barData.begin(), barData.end()) * 1.05);
        bar->setData(ticks, barData);
    }else
    {
        QSharedPointer<QCPAxisTickerDateTime> dateTicker(new QCPAxisTickerDateTime);
        dateTicker->setDateTimeFormat("dd MM yyyy HH");
        this->customPlot->xAxis->setTicker(dateTicker);
        this->customPlot->xAxis->setRange(*data[0].begin(), *data[0].end());
        this->customPlot->yAxis->setRange(*std::min_element(data[1].begin(), data[1].end()) * 0.9, *std::max_element(data[1].begin(), data[1].end()) * 1.05);
        bar->setData(data[0], data[1]);
        this->customPlot->rescaleAxes();
    }
};
