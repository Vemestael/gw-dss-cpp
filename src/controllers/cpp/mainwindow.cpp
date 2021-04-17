#include "../hpp/mainwindow.h"
#include "./ui_mainwindow.h"

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
    this->setTablesHeaders();
    this->setButtonsHandling();
};

void MainWindow::loadSettings(void)
{
    QString path = this->settings.value("dbPath", "").toString();
    while (path == "") {
        this->setDbPathTriggered();
        path = this->settings.value("dbPath", "").toString();
    }
    this->db.connectToDataBase(path);

    QString lang = this->settings.value("lang", "en").toString();
    this->switchLangTriggered(lang);
};

void MainWindow::setDate(void)
{
    this->ui->dateStart->setDate(QDate::currentDate().addDays(-7));
    this->ui->dateEnd->setDate(QDate::currentDate());
};

void MainWindow::setTablesHeaders(void)
{
    auto tables = {
        this->ui->predictTable1, this->ui->predictTable2, this->ui->predictTable3,
        this->ui->predictTable4, this->ui->predictTable5, this->ui->predictTable6,
        this->ui->predictTable7, this->ui->costTable1,    this->ui->costTable2,
        this->ui->costTable3,    this->ui->costTable4,    this->ui->costTable5,
        this->ui->costTable6,    this->ui->costTable7,
    };
    for (auto &&table : tables) {
        table->setSpan(0, 0, 1, 2);
        table->setSpan(1, 0, 3, 1);
        table->setSpan(4, 0, 3, 1);
        table->setSpan(7, 0, 3, 1);

        // horizontal headers
        table->setItem(0, 2, new QTableWidgetItem(tr("Shift 1")));
        table->setItem(0, 3, new QTableWidgetItem(tr("Shift 2")));
        table->setItem(0, 4, new QTableWidgetItem(tr("Shift 3")));

        if (table->objectName().contains("predictTable")) {
            for (size_t i = 0; i < 10; i += 3) {
                table->setItem(i + 1, 1, new QTableWidgetItem(tr("Number of staff")));
                table->setItem(i + 2, 1, new QTableWidgetItem(tr("Requests processed")));
                table->setItem(i + 3, 1, new QTableWidgetItem(tr("Queue length")));
            }
        } else {
            for (size_t i = 0; i < 10; i += 3) {
                table->setItem(i + 1, 1, new QTableWidgetItem(tr("Personnel costs")));
                table->setItem(i + 2, 1, new QTableWidgetItem(tr("Requests cost")));
            }
        }

        auto item = [](QString const &text) {
            QTableWidgetItem *item = new QTableWidgetItem();
            item->setTextAlignment(Qt::AlignCenter);
            item->setText(text);
            return item;
        };
        table->setItem(1, 0, item(tr("Maximum requests")));
        table->setItem(4, 0, item(tr("Optimally")));
        table->setItem(7, 0, item(tr("Min. queue")));

        table->setItemDelegateForColumn(0, new VerticalTextDelegate(this));
        table->setColumnWidth(0, 10);
        table->setColumnWidth(1, 160);
    }
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
        QErrorMessage().showMessage(QErrorMessage::tr("Empty dataset"));
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
        QErrorMessage().showMessage(QErrorMessage::tr("Empty dataset"));
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
    QVector<QTableWidget *> predictTables = {
        this->ui->predictTable1, this->ui->predictTable2, this->ui->predictTable3,
        this->ui->predictTable4, this->ui->predictTable5, this->ui->predictTable6,
        this->ui->predictTable7,
    };
    QVector<QTableWidget *> costTables = {
        this->ui->costTable1, this->ui->costTable2, this->ui->costTable3, this->ui->costTable4,
        this->ui->costTable5, this->ui->costTable6, this->ui->costTable7,
    };

    QVector<unsigned> personalCount(staffNumber);
    std::iota(personalCount.begin(), personalCount.end(), 1);

    QVector<QVector<double>> lambdaByShift = DataProcessing::getCountOfCallsByShift(
            this->db.getCallsCountsByDate(this->ui->dateStart->date(), this->ui->dateEnd->date()));

    for (size_t i = 0; i < lambdaByShift.length(); ++i) {
        auto predictTable = predictTables[i];
        auto costTable = costTables[i];

        for (size_t j = 0; j < lambdaByShift[i].length(); ++j) {
            size_t index = 1;
            QVector<QVector<double>> predicts = Predict::getPredict(
                    personalCount, queueLength, lambdaByShift[i][j], servedRequirements);

            for (auto &&predict : predicts) {
                for (auto &&characteristic : predict) {
                    predictTable->setItem(
                            index, j + 2,
                            new QTableWidgetItem(QString::number(characteristic, 'f', 2)));
                    index += 1;
                }

                double channelCost = predict[0] * cost;
                double requestCost = predict[0] * cost / predict[1];
                costTable->setItem(index - 3, j + 2,
                                   new QTableWidgetItem(QString::number(channelCost, 'f', 2)));
                costTable->setItem(index - 2, j + 2,
                                   new QTableWidgetItem(QString::number(requestCost, 'f', 2)));
            }
        }
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
            this->setTablesHeaders();
            this->settings.setValue("lang", lang);
            this->settings.sync();
        } else {
            QErrorMessage().showMessage(QErrorMessage::tr("Unable to install language"));
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
    
    QString lang = this->settings.value("lang", "en").toString();
    this->cw = new ChartWindow(type, data, QString(__APPLICATION_NAME__) + "_" + lang);
    this->cw->show();
};