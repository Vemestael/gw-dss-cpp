#include "./ui_mainwindow.h"

#include <gw-dss-cpp/controllers/mainwindow.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      settings(QSettings::IniFormat, QSettings::UserScope, __APPLICATION_NAME__),
      cw(nullptr),
      customPlot(nullptr)
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
    this->setSpinBoxValue();
    this->addGraphChecked(Qt::Unchecked);
    this->ui->addGraphBox->setVisible(false);
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

void MainWindow::setSpinBoxValue(void)
{
    this->ui->channelCost->setValue(this->settings.value("channelCost", 0.0).toDouble());
    this->ui->requestCost->setValue(this->settings.value("requestCost", 0.0).toDouble());
    this->ui->servedRequirements->setValue(this->settings.value("servedRequirements", 0.0).toDouble());
    this->ui->staffNumber->setValue(this->settings.value("staffNumber", 1).toInt());
    this->ui->queueLength->setValue(this->settings.value("queueLength", 0).toInt());
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

    connect(this->ui->graphBox_1, &QComboBox::currentIndexChanged, this,
            [this]() { this->additionalGraphChanged(this->ui->graphBox_1);});
    connect(this->ui->graphBox_2, &QComboBox::currentIndexChanged, this,
            [this]() { this->additionalGraphChanged(this->ui->graphBox_2);});

    connect(this->ui->addGraphBox, &QCheckBox::stateChanged, this, &MainWindow::addGraphChecked);

    connect(this->ui->channelCost, &QDoubleSpinBox::valueChanged, this,
            [this]() { this->spinBoxChanged(this->ui->channelCost); });
    connect(this->ui->servedRequirements, &QDoubleSpinBox::valueChanged, this,
            [this]() { this->spinBoxChanged(this->ui->servedRequirements); });
    connect(this->ui->staffNumber, &QSpinBox::valueChanged, this,
            [this]() { this->spinBoxChanged(this->ui->staffNumber); });
    connect(this->ui->queueLength, &QSpinBox::valueChanged, this,
            [this]() { this->spinBoxChanged(this->ui->queueLength); });

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
    double channelCost = this->settings.value("channelCost", 0.0).toDouble();
    double requestCost = this->settings.value("requestCost", 0.0).toDouble();
    double servedRequirements = this->settings.value("servedRequirements", 0.0).toDouble();
    unsigned staffNumber = this->settings.value("staffNumber", 0).toInt();
    unsigned queueLength = this->settings.value("queueLength", 0).toInt();
    while (channelCost == 0.0) {
        this->setHourlyPaymentTriggered();
        channelCost = this->settings.value("channelCost", 0.0).toDouble();
    }
    while (requestCost == 0.0) {
        this->setRequestCostTriggered();
        requestCost = this->settings.value("requestCost", 0.0).toDouble();
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

    servedReqArr.clear();
    unservedReqArr.clear();
    queueLenArr.clear();
    reqCostArr.clear();
    staffCostArr.clear();

    this->setSpinBoxValue();

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
    for(auto&& servedReqByShift : servedReqArr)
    {
        QVector<double> reqCostByStaffCount;
        for(auto&& servedReqByStaffCount : servedReqByShift)
        {
            reqCostByStaffCount.append(servedReqByStaffCount * requestCost);
        }
        reqCostArr.append(reqCostByStaffCount);
    }
    for(size_t i = 0; i < 21; i++){
        QVector<double> staffCostByShift;
        for(auto&& personalCountByShift : personalCount)
        {
            staffCostByShift.append(personalCountByShift * channelCost);
        }
        staffCostArr.append(staffCostByShift);
    }

    this->ui->graphBox->setEnabled(true);
    this->ui->dayBox->setEnabled(true);
    this->ui->shiftBox->setEnabled(true);
    this->ui->addGraphBox->setVisible(true);
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
    QString legendName = this->ui->graphBox->currentText();
    QString graphName;
    switch (graphBoxIndex)
    {
    case 0:
        data = this->servedReqArr[index];
        graphName = tr("Кількість заявок, що були обслуговані");
        break;
    
    case 1:
        data = this->unservedReqArr[index];
        graphName = tr("Кількість заявок, що не були обслуговані");
        break;
    
    case 2:
        data = this->queueLenArr[index];
        graphName = tr("Довжина черги");
        break;

    case 3:
        data = this->reqCostArr[index];
        graphName = tr("Дохід від заявок, що були обслуговані");
        break;

    case 4:
        data = this->staffCostArr[index];
        graphName = tr("Витрати на персонал");
        break;
    }
    delete this->customPlot;
    this->customPlot = new QCustomPlot();
    this->setPlotSettings(this->customPlot);
    this->plotGraph(this->customPlot, graphName, legendName, data);
    QGraphicsScene *scene = new QGraphicsScene();
    scene->addWidget(this->customPlot);
    this->ui->graphicsView->setScene(scene);
};

void MainWindow::additionalGraphChanged(QComboBox *obj)
{
    int index = obj->currentIndex();
    if(index != 0){
        QVector<double> data;
        switch (index)
        {
        case 1:
            data = this->servedReqArr[index];
            break;
        
        case 2:
            data = this->unservedReqArr[index];
            break;
        
        case 3:
            data = this->queueLenArr[index];
            break;
        case 4:
            data = this->reqCostArr[index];
            break;
        case 5:
            data = this->staffCostArr[index];
            break;
        }
        this->plotGraph(this->customPlot, "", obj->currentText(), data);
        this->customPlot->replot(QCustomPlot::rpQueuedRefresh);
    }
};

// check box handlers

void MainWindow::addGraphChecked(int state)
{
    switch (state)
    {
    case Qt::Unchecked:
        this->ui->graphBox_1->setVisible(false);
        this->ui->graphBox_2->setVisible(false);
        this->ui->labelGraphBox_1->setVisible(false);
        this->ui->labelGraphBox_2->setVisible(false);
        this->ui->graphBox_1->setCurrentIndex(0);
        this->ui->graphBox_2->setCurrentIndex(0);
        if(this->customPlot != nullptr)
            this->graphTypeChanged();
        break;
    
    case Qt::Checked:
        this->ui->graphBox_1->setVisible(true);
        this->ui->graphBox_2->setVisible(true);
        this->ui->labelGraphBox_1->setVisible(true);
        this->ui->labelGraphBox_2->setVisible(true);
        break;
    }
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

void MainWindow::setRequestCostTriggered(void)
{
    bool ok;
    double cost = QInputDialog::getDouble(this, "Hourly payment", "Enter hourly payment", 0, 0,
                                          2147483647, 2, &ok);
    if (ok) {
        this->settings.setValue("requestCost", cost);
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
    customPlot->setMinimumHeight(325);
    customPlot->setMaximumHeight(360);
    customPlot->setMinimumWidth(1240);

    customPlot->setInteraction(QCP::iRangeZoom, true);
    customPlot->setInteraction(QCP::iRangeDrag, true);

    customPlot->xAxis->grid()->setVisible(true);
    customPlot->yAxis->grid()->setSubGridVisible(true);

    customPlot->xAxis->setLabel(tr("Кількість співробітників"));

    customPlot->xAxis2->setVisible(true);
    customPlot->xAxis2->setTickLabels(false);
    QFont font("sans", 14);
    customPlot->xAxis2->setLabelFont(font);

    customPlot->legend->setVisible(true);
};

void MainWindow::plotGraph(QCustomPlot *customPlot, QString const &graphName, QString const &legendName, QVector<double> data)
{
    if (graphName != "") {
        customPlot->xAxis2->setLabel(graphName);
    }

    QVector<QCPGraphData> graphData;
    for (size_t i = 0; i < data.length(); ++i) {
        graphData.append({double(i + 1), data[i]});
    }

    QCPGraph *graph = customPlot->addGraph();
    graph->data()->set(graphData);
    graph->keyAxis()->setRange(-1, data.length() + 1);
    graph->valueAxis()->setRange(-0.25, *std::max_element(data.begin(), data.end()) * 1.05);
    graph->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, QPen(Qt::black), QBrush(Qt::white), 6));
    switch (customPlot->graphCount())
    {
        case 1:
            graph->setPen(QPen(Qt::black));
            break;
        case 2:
            graph->setPen(QPen(Qt::red));
            break;
        case 3:
            graph->setPen(QPen(Qt::blue));
            break;
        default:
            graph->setPen(QPen(Qt::black));
            break;
    }
    graph->setName(legendName);
};