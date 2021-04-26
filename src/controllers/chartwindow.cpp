#include <gw-dss-cpp/controllers/chartwindow.h>

// little trick to force cmake link with Qt::PrintSupport
// TODO: fix it
[[maybe_unused]] static auto __unused = QPrinterInfo::defaultPrinter();

static QVector<double> genRange(int lower, int upper)
{
    QVector<double> vec;
    vec.resize(upper - 1);
    std::iota(vec.begin(), vec.end(), lower);
    return vec;
};

ChartWindow::ChartWindow(ChartType type, QVector<QVector<double>> data, QString const &lang, QWidget *parent)
    : customPlot(new QCustomPlot(this)), layout(new QGridLayout(this)), QWidget(parent)
{

    QTranslator translator;
    if(translator.load(lang, "./translations/")){
        if(!qApp->installTranslator(&translator)){
            QErrorMessage().showMessage(QErrorMessage::tr("Unable to install language"));
        }
    }
    this->setBaseSize(800, 600);
    this->setMinimumSize(800, 600);

    this->layout->addWidget(this->customPlot, 0, 0, 1, 1);

    this->plot(type, data);
};

ChartWindow::~ChartWindow(void)
{
    delete this->customPlot;
    delete this->layout;
};

void ChartWindow::plot(ChartType type, QVector<QVector<double>> data)
{
    QVector<QString> weekDays = { tr("Mon"), tr("Tue"), tr("Wed"), tr("Thu"),
                                  tr("Fri"), tr("Sat"), tr("Sun") };

    QVector<QString> labels;
    switch (type) {
    case ChartType::Timescale: {
        this->customPlot->yAxis->setLabel(tr("Timescale load"));

        QSharedPointer<QCPAxisTickerDateTime> dateTicker(new QCPAxisTickerDateTime());
        dateTicker->setDateTimeFormat("dd.MM.yyyy HH:00");

        this->decorate(true, false, true, false, true, 0);
        this->plotTimeDataGraph(dateTicker, data);
        break;
    }
    case ChartType::WeekDays: {
        this->customPlot->yAxis->setLabel(tr("Load by week days"));

        QSharedPointer<QCPAxisTickerText> xTicker(new QCPAxisTickerText());

        // setting up X-axis ticks key:value
        xTicker->addTicks(genRange(1, 8) /* 7 week days */, weekDays);

        this->decorate(false, false, false, false, true, 0);
        this->plotHistogram(xTicker, data);
        break;
    }
    case ChartType::ByShifts: {
        this->customPlot->yAxis->setLabel(tr("Load by work shifts"));

        QSharedPointer<QCPAxisTickerText> xTicker(new QCPAxisTickerText());

        QVector<QString> labels;
        for (auto &&weekDay : weekDays) {
            for (size_t i = 0; i < 3; ++i) {
                labels.append(weekDay + QString(" %1").arg(i + 1));
            }
        }

        // setting up X-axis ticks key:value
        xTicker->addTicks(genRange(1, 22) /* 7 week days * 3 shifts */, labels);

        this->decorate(false, false, false, false, true, 30);
        this->plotHistogram(xTicker, data);
        break;
    }
    case ChartType::ByHours: {
        this->customPlot->yAxis->setLabel(tr("Load by hours"));

        QSharedPointer<QCPAxisTickerText> xTicker(new QCPAxisTickerText());

        QVector<QString> labels;
        for (auto &&weekDay : weekDays) {
            for (size_t i = 0; i < 24; ++i) {
                labels.append(weekDay + QString(" %1").arg(i + 1));
            }
        }

        // setting up X-axis ticks key:value
        xTicker->addTicks(genRange(1, 169 /* 7 week days * 24 hours */), labels);

        this->decorate(true, false, true, false, true, 45);
        this->plotHistogram(xTicker, data);
        break;
    }
    }
};

void ChartWindow::plotTimeDataGraph(QSharedPointer<QCPAxisTickerDateTime> dateTicker,
                                    QVector<QVector<double>> data)
{
    // setting up ticker
    this->customPlot->xAxis->setTicker(dateTicker);

    this->customPlot->xAxis->setRange(data[0].first() - 3600 * 24, data[0].last() + 3600 * 24);
    this->customPlot->yAxis->setRange(*std::min_element(data[1].begin(), data[1].end()) * 0.9,
                                      *std::max_element(data[1].begin(), data[1].end()) * 1.05);

    // graph
    this->customPlot->addGraph();

    this->customPlot->graph()->setLineStyle(QCPGraph::lsLine);

    // combining key:value together
    QVector<QCPGraphData> timeData;
    for (size_t i = 0; i < data[0].length(); ++i) {
        timeData.append({ data[0][i], data[1][i] });
    }

    this->customPlot->graph()->data()->set(timeData);
};

void ChartWindow::plotHistogram(QSharedPointer<QCPAxisTickerText> xTicker,
                                QVector<QVector<double>> data)
{
    // setting up ticker
    this->customPlot->xAxis->setTicker(xTicker);

    // data combining
    QVector<double> combinedData;
    for (auto &&vec : data) {
        for (auto &&val : vec) {
            combinedData.push_back(val);
        }
    }

    // histogram
    QCPBars *bar = new QCPBars(this->customPlot->xAxis, this->customPlot->yAxis);
    bar->setAntialiased(false);

    this->customPlot->xAxis->setRange(0, combinedData.length() + 1);
    this->customPlot->yAxis->setRange(
            *std::min_element(combinedData.begin(), combinedData.end()) * 0.9,
            *std::max_element(combinedData.begin(), combinedData.end()) * 1.05);

    // plotting
    bar->setData(xTicker->ticks().keys().toVector(), combinedData);
};

void ChartWindow::decorate(bool zoomH, bool zoomV, bool dragH, bool dragV, bool grid,
                           double labelAngle)
{
    // setting up zoom
    this->customPlot->setInteraction(QCP::iRangeZoom, zoomH || zoomV);
    if (zoomH) {
        this->customPlot->axisRect()->setRangeZoom(Qt::Horizontal);
    }
    if (zoomV) {
        this->customPlot->axisRect()->setRangeZoom(Qt::Vertical);
    }

    // setting up drag
    this->customPlot->setInteraction(QCP::iRangeDrag, dragH || dragV);
    if (dragH) {
        this->customPlot->axisRect()->setRangeDrag(Qt::Horizontal);
    }
    if (dragV) {
        this->customPlot->axisRect()->setRangeDrag(Qt::Vertical);
    }

    // setting up grid
    if (grid) {
        this->customPlot->xAxis->grid()->setVisible(true);
        this->customPlot->yAxis->grid()->setSubGridVisible(true);
    }

    // label rotation
    this->customPlot->xAxis->setTickLabelRotation(labelAngle);
};
