#include "./ui_mainwindow.h"

#include <gw-dss-cpp/controllers/mainwindow.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      settings(QSettings::IniFormat, QSettings::UserScope, __APPLICATION_NAME__),
      cw(nullptr)
{
    ui->setupUi(this);
    this->loadWindow();
    this->loadSettings();
};

MainWindow::~MainWindow()
{
    delete this->cw;
    delete ui;
};

// window preparing

void MainWindow::loadWindow(void)
{
    this->setDate();
    this->setButtonsHandling();
};

void MainWindow::loadSettings(void)
{
    QString path = this->settings.value("dbPath", "").toString();
    if(path != "")
    {
        this->db.connectToDataBase(path);
    }else
    {
        while (path == "") {
                this->setDbPathTriggered();
                path = this->settings.value("dbPath", "").toString();
            }
    }

    QString lang = this->settings.value("lang", "en").toString();
    this->switchLangTriggered(lang);
};

void MainWindow::setDate(void)
{
    this->ui->dateStart->setDate(QDate::currentDate().addDays(-7));
    this->ui->dateEnd->setDate(QDate::currentDate());
};

void MainWindow::setButtonsHandling(void)
{
    connect(this->ui->allTime, &QPushButton::clicked, this, &MainWindow::allTimePressed);
    connect(this->ui->lastYear, &QPushButton::clicked, this, &MainWindow::lastYearPressed);
    connect(this->ui->lastHalfYear, &QPushButton::clicked, this, &MainWindow::lastHalfYearPressed);
    connect(this->ui->lastQuartYear, &QPushButton::clicked, this,
            &MainWindow::lastQuartYearPressed);
    connect(this->ui->lastMonth, &QPushButton::clicked, this, &MainWindow::lastMonthPressed);
    connect(this->ui->analyze, &QPushButton::clicked, this, &MainWindow::analyzePressed);

    connect(this->ui->graphBox, &QComboBox::currentIndexChanged, this, &MainWindow::graphTypeChanged);
    connect(this->ui->dayBox, &QComboBox::currentIndexChanged, this, &MainWindow::graphTypeChanged);
    connect(this->ui->shiftBox, &QComboBox::currentIndexChanged, this, &MainWindow::graphTypeChanged);

    connect(this->ui->actionDbPath, &QAction::triggered, this, &MainWindow::setDbPathTriggered);
    connect(this->ui->actionHourlyPayment, &QAction::triggered, this,
            &MainWindow::setHourlyPaymentTriggered);
    connect(this->ui->actionServedRequests, &QAction::triggered, this,
            &MainWindow::setServedRequestsTriggered);
    connect(this->ui->actionStaffNumber, &QAction::triggered, this,
            &MainWindow::setStaffNumberTriggered);
    connect(this->ui->actionMaxQueueLength, &QAction::triggered, this,
            &MainWindow::setMaxQueueLengthTriggered);
    connect(this->ui->actionExit, &QAction::triggered, qApp, qApp->quit);

    connect(this->ui->enLang, &QAction::triggered, this,
            [this]() { this->switchLangTriggered("en_US"); });
    connect(this->ui->ukLang, &QAction::triggered, this,
            [this]() { this->switchLangTriggered("uk_UA"); });
    connect(this->ui->ruLang, &QAction::triggered, this,
            [this]() { this->switchLangTriggered("ru_RU"); });

    connect(this->ui->actionChartTimescale, &QAction::triggered, this,
            [this]() { this->showChartTriggered(ChartType::Timescale); });
    connect(this->ui->actionChartByWeekDays, &QAction::triggered, this,
            [this]() { this->showChartTriggered(ChartType::WeekDays); });
    connect(this->ui->actionChartByWorkShifts, &QAction::triggered, this,
            [this]() { this->showChartTriggered(ChartType::ByShifts); });
    connect(this->ui->actionChartByHours, &QAction::triggered, this,
            [this]() { this->showChartTriggered(ChartType::ByHours); });
};

// pressed event handlers

void MainWindow::allTimePressed(void)
{
    if (this->db.isEmptyTable()) {
        ErrorWindow(ErrorWindow::tr("Empty dataset"));
        return;
    }
    this->ui->dateStart->setDate(this->db.getFirstDate());
};

void MainWindow::lastYearPressed(void)
{
    this->ui->dateStart->setDate(QDate::currentDate().addYears(-1));
};

void MainWindow::lastHalfYearPressed(void)
{
    this->ui->dateStart->setDate(QDate::currentDate().addMonths(-6));
};

void MainWindow::lastQuartYearPressed(void)
{
    this->ui->dateStart->setDate(QDate::currentDate().addMonths(-3));
};

void MainWindow::lastMonthPressed(void)
{
    this->ui->dateStart->setDate(QDate::currentDate().addMonths(-1));
};

void MainWindow::analyzePressed(void)
{
    if (this->db.isEmptyTable()) {
        ErrorWindow(ErrorWindow::tr("Empty dataset"));
        return;
    }
    double cost = this->settings.value("channelCost", 0.0).toDouble();
    double servedRequirements = this->settings.value("servedRequirements", 0.0).toDouble();
    unsigned staffNumber = this->settings.value("staffNumber", 0).toInt();
    unsigned queueLength = this->settings.value("queueLength", 0).toInt();
    while (cost == 0.0) {
        this->setHourlyPaymentTriggered();
        cost = this->settings.value("channelCost", 0.0).toDouble();
    }
    while (servedRequirements == 0.0) {
        this->setServedRequestsTriggered();
        servedRequirements = this->settings.value("servedRequirements", 0.0).toDouble();
    }
    while (staffNumber == 0) {
        this->setStaffNumberTriggered();
        staffNumber = this->settings.value("staffNumber", 0).toInt();
    }
    while (queueLength == 0) {
        this->setMaxQueueLengthTriggered();
        queueLength = this->settings.value("queueLength", 0).toInt();
    }

    QVector<unsigned> personalCount(staffNumber);
    std::iota(personalCount.begin(), personalCount.end(), 1);

    QVector<QVector<double>> lambdaByShift = DataProcessing::getCountOfCallsByShift(
            this->db.getCallsCountsByDate(this->ui->dateStart->date(), this->ui->dateEnd->date()));

    if(lambdaByShift.isEmpty()) {
        ErrorWindow(ErrorWindow::tr("Empty dataset for the selected date range"));
        return;
    }

    for (size_t i = 0; i < lambdaByShift.length(); ++i) {
        for (size_t j = 0; j < lambdaByShift[i].length(); ++j) {
            QVector<QVector<double>> predicts = Predict::getCharacteristics(
                    personalCount, queueLength, lambdaByShift[i][j], servedRequirements);
            servedReqArr.append(predicts[0]);
            unservedReqArr.append(predicts[1]);
            queueLenArr.append(predicts[2]);
        }
    }

    this->ui->graphBox->setEnabled(true);
    this->ui->dayBox->setEnabled(true);
    this->ui->shiftBox->setEnabled(true);
    this->graphTypeChanged();
};

// combo box handlers

void MainWindow::graphTypeChanged(void)
{
    size_t graphBoxIndex = this->ui->graphBox->currentIndex();
    size_t dayBoxIndex = this->ui->dayBox->currentIndex();
    size_t shiftBoxIndex = this->ui->shiftBox->currentIndex();
    size_t index = (dayBoxIndex + 1) * (shiftBoxIndex + 1)  - 1;

    QVector<double> data;
    QString title = this->ui->graphBox->currentText();
    switch (graphBoxIndex)
    {
    case 0:
        data = this->servedReqArr[index];
        break;
    
    case 1:
        data = this->unservedReqArr[index];
        break;
    
    case 2:
        data = this->queueLenArr[index];
        break;
    }
    QCustomPlot *customPlot = new QCustomPlot();
    this->setPlotSettings(customPlot);
    this->plotGraph(customPlot, "some title", data);
    QGraphicsScene *scene = new QGraphicsScene();
    scene->addWidget(customPlot);
    this->ui->graphicsView->setScene(scene);
};

// triggered menu handlers

void MainWindow::setDbPathTriggered(void)
{
    QString fileName =
            QFileDialog::getOpenFileName(this, tr("Open File"), ".", tr("DB file (*.db *.sqlite)"));
    if (fileName != "") {
        this->settings.setValue("dbPath", fileName);
        this->settings.sync();
        this->db.disconnectFromDataBase();
        this->db.connectToDataBase(fileName);
    }
};

void MainWindow::setHourlyPaymentTriggered(void)
{
    bool ok;
    double cost = QInputDialog::getDouble(this, "Hourly payment", "Enter hourly payment", 0, 0,
                                          2147483647, 2, &ok);
    if (ok) {
        this->settings.setValue("channelCost", cost);
        this->settings.sync();
    }
};

void MainWindow::setServedRequestsTriggered(void)
{
    bool ok;
    double count = QInputDialog::getDouble(
            this, "Count served requirements",
            "Enter count of served requirements per hour by one person", 0, 0, 2147483647, 2, &ok);
    if (ok) {
        this->settings.setValue("servedRequirements", count);
        this->settings.sync();
    }
};

void MainWindow::setStaffNumberTriggered(void)
{
    bool ok;
    unsigned count = QInputDialog::getInt(this, "Staff number", "Enter number of staff", 0, 0,
                                          2147483647, 2, &ok);
    if (ok) {
        this->settings.setValue("staffNumber", count);
        this->settings.sync();
    }
};

void MainWindow::setMaxQueueLengthTriggered(void)
{
    bool ok;
    unsigned queueLength = QInputDialog::getInt(
            this, "Max queue length", "Enter maximum queue length", 0, 0, 2147483647, 2, &ok);
    if (ok) {
        this->settings.setValue("queueLength", queueLength);
        this->settings.sync();
    }
};

void MainWindow::switchLangTriggered(QString const &lang)
{
    QTranslator translator;
    if (translator.load(QString(__APPLICATION_NAME__) + "_" + lang, "./translations/")) {
        if (qApp->installTranslator(&translator)) {
            this->ui->retranslateUi(this);
            this->settings.setValue("lang", lang);
            this->settings.sync();
        } else {
            ErrorWindow(ErrorWindow::tr("Unable to install language"));
        }
    };
};

void MainWindow::showChartTriggered(ChartType type)
{
    QVector<QVector<double>> data;
    switch (type) {
    case ChartType::Timescale:
        data = db.getCallsInfoByDate(this->ui->dateStart->date(), this->ui->dateEnd->date());
        break;
    case ChartType::WeekDays:
        data = DataProcessing::getCountOfCallsByWeekDay(this->db.getCallsCountsByDate(
                this->ui->dateStart->date(), this->ui->dateEnd->date()));
        break;
    case ChartType::ByShifts:
        data = DataProcessing::getCountOfCallsByShift(this->db.getCallsCountsByDate(
                this->ui->dateStart->date(), this->ui->dateEnd->date()));
        break;
    case ChartType::ByHours:
        data = DataProcessing::getCountOfCallsByHour(this->db.getCallsCountsByDate(
                this->ui->dateStart->date(), this->ui->dateEnd->date()));
        break;
    }
    
    if(data.isEmpty() || data[0].isEmpty()) {
        ErrorWindow(ErrorWindow::tr("Empty dataset for the selected date range"));
        return;
    }

    QString lang = this->settings.value("lang", "en").toString();
    this->cw = new ChartWindow(type, data, QString(__APPLICATION_NAME__) + "_" + lang);
    this->cw->show();
};

void MainWindow::setPlotSettings(QCustomPlot *customPlot)
{
    // Fixed size to fill graphicsView
    customPlot->setFixedHeight(385);
    customPlot->setFixedWidth(1265);

    customPlot->setInteraction(QCP::iRangeZoom, true);
    customPlot->setInteraction(QCP::iRangeDrag, true);

    customPlot->axisRect()->setRangeZoom(Qt::Horizontal);
    customPlot->axisRect()->setRangeDrag(Qt::Horizontal);
    
    customPlot->xAxis->grid()->setVisible(true);
    customPlot->yAxis->grid()->setSubGridVisible(true);
};

void MainWindow::plotGraph(QCustomPlot *customPlot, QString title, QVector<double> data)
{
    QVector<QString> labels;
    QSharedPointer<QCPAxisTickerText> xTicker(new QCPAxisTickerText());

    QVector<double> range = genRange(1, data.length() + 1);

    for(auto&& rangeElement : range)
    {
        labels.append(QString::number(rangeElement));
    }

    xTicker->addTicks(range, labels);

    customPlot->xAxis->setTicker(xTicker);

    customPlot->xAxis->setRange(0, data.length() + 1);
    customPlot->yAxis->setRange(
            *std::min_element(data.begin(), data.end()) * 0.9,
            *std::max_element(data.begin(), data.end()) * 1.05);

    customPlot->yAxis->setLabel(title);
    customPlot->addGraph();

    customPlot->graph()->setLineStyle(QCPGraph::lsLine);

    QVector<QCPGraphData> graphData;
    for (size_t i = 0; i < data.length(); ++i) {
        graphData.append({double(i + 1), data[i]});
    }

    customPlot->graph()->data()->set(graphData);
};