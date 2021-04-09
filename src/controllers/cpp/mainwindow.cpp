#include "../hpp/mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , settings(QSettings::IniFormat, QSettings::UserScope, "gw-dss-cpp")
{
    ui->setupUi(this);
    this->initUi();
    this->loadSettings();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::loadSettings(void)
{
    QString path = this->settings.value("dbPath", "").toString();
    while(path == "")
    {
        this->setDbPathTriggered();
        path = this->settings.value("dbPath", "").toString();
    }
    db.connectToDataBase(path);
}

void MainWindow::initUi(void)
{
    this->setDate();
    this->setTablesHeaders();
    this->setButtonsHandling();
}

void MainWindow::setDate(void)
{
    this->ui->date_start->setDate(QDate::currentDate().addDays(-7));
    this->ui->date_end->setDate(QDate::currentDate());
}

void MainWindow::setTablesHeaders(void)
{
    auto tables = {
        this->ui->predict_table_1,
        this->ui->predict_table_2,
        this->ui->predict_table_3,
        this->ui->predict_table_4,
        this->ui->predict_table_5,
        this->ui->predict_table_6,
        this->ui->predict_table_7,
        this->ui->cost_table_1,
        this->ui->cost_table_2,
        this->ui->cost_table_3,
        this->ui->cost_table_4,
        this->ui->cost_table_5,
        this->ui->cost_table_6,
        this->ui->cost_table_7,
    };
    for (auto&& table: tables) {
        table->setSpan(0, 0, 1, 2);
        table->setSpan(1, 0, 3, 1);
        table->setSpan(4, 0, 3, 1);
        table->setSpan(7, 0, 3, 1);

        // horizontal headers
        table->setItem(0, 2, new QTableWidgetItem("Shift 1"));
        table->setItem(0, 3, new QTableWidgetItem("Shift 2"));
        table->setItem(0, 4, new QTableWidgetItem("Shift 3"));

        if (table->objectName().contains("predict_table_"))
        {
            for (size_t i = 0; i < 10; i += 3)
            {
                table->setItem(i + 1, 1, new QTableWidgetItem("Количество персонала"));
                table->setItem(i + 2, 1, new QTableWidgetItem("Обработанных заявок"));
                table->setItem(i + 3, 1, new QTableWidgetItem("Длина очереди"));
            }
        }else
        {
            for (size_t i = 0; i < 10; i += 3)
            {
                table->setItem(i + 1, 1, new QTableWidgetItem("Стоимость персонала"));
                table->setItem(i + 2, 1, new QTableWidgetItem("Стоимость заявки"));
            }
        }

        auto item =[](QString text) {
            QTableWidgetItem* item = new QTableWidgetItem();
            item->setTextAlignment(Qt::AlignCenter);
            item->setText(text);
            return item;
        };
        table->setItem(1, 0, item("Максимум заявок"));
        table->setItem(4, 0, item("Оптимально"));
        table->setItem(7, 0, item("Мин. очередь"));

        table->setItemDelegateForColumn(0, new verticalTextDelegate(this));
        table->setColumnWidth(0, 10);
        table->setColumnWidth(1, 160);
    }
}

void MainWindow::setButtonsHandling(void)
{
    connect(this->ui->all_time, &QPushButton::clicked, this, &MainWindow::allTimePressed);
    connect(this->ui->last_year, &QPushButton::clicked, this, &MainWindow::lastYearPressed);
    connect(this->ui->last_6_month, &QPushButton::clicked, this, &MainWindow::lastHalfYearPressed);
    connect(this->ui->last_3_month, &QPushButton::clicked, this, &MainWindow::lastQuartYearPressed);
    connect(this->ui->last_month, &QPushButton::clicked, this, &MainWindow::lastMonthPressed);
    connect(this->ui->analyze, &QPushButton::clicked, this, &MainWindow::analyzePressed);

    connect(this->ui->action_db_path, &QAction::triggered, this, &MainWindow::setDbPathTriggered);
    connect(this->ui->action_cost, &QAction::triggered, this, &MainWindow::setHourlyPaymentTriggered);
    connect(this->ui->action_exit, &QAction::triggered,  qApp, qApp->quit);

    connect(this->ui->en_lang, &QAction::triggered, this, [this]() {this->switchLangTriggered("en");});
    connect(this->ui->uk_lang, &QAction::triggered, this, [this]() {this->switchLangTriggered("uk");});
    connect(this->ui->ru_lang, &QAction::triggered, this, [this]() {this->switchLangTriggered("ru");});
}

void MainWindow::allTimePressed(void)
{
    if(this->db.isEmptyTable())
    {
        QErrorMessage().showMessage("Empty dataset");
        return;
    }
    this->ui->date_start->setDate(db.getFirstDate());
};

void MainWindow::lastYearPressed(void){
    this->ui->date_start->setDate(QDate::currentDate().addYears(-1));
};

void MainWindow::lastHalfYearPressed(void)
{
    this->ui->date_start->setDate(QDate::currentDate().addMonths(-6));
};

void MainWindow::lastQuartYearPressed(void)
{
    this->ui->date_start->setDate(QDate::currentDate().addMonths(-3));
};

void MainWindow::lastMonthPressed(void)
{
    this->ui->date_start->setDate(QDate::currentDate().addMonths(-1));
};

void MainWindow::analyzePressed(void)
{
    if(this->db.isEmptyTable())
    {
        QErrorMessage().showMessage("Empty dataset");
        return;
    }
    double cost = settings.value("channelCost", 0.0).toDouble();
    while(cost == 0.0)
    {
        this->setHourlyPaymentTriggered();
        cost = settings.value("channelCost", 0.0).toDouble();
    }
    QList<QTableWidget*> predictTables = {
        this->ui->predict_table_1,
        this->ui->predict_table_2,
        this->ui->predict_table_3,
        this->ui->predict_table_4,
        this->ui->predict_table_5,
        this->ui->predict_table_6,
        this->ui->predict_table_7,
    };
    QList<QTableWidget*> costTables = {
            this->ui->cost_table_1,
            this->ui->cost_table_2,
            this->ui->cost_table_3,
            this->ui->cost_table_4,
            this->ui->cost_table_5,
            this->ui->cost_table_6,
            this->ui->cost_table_7,
    };

    QVector<int> personalCount(20);
    std::iota(personalCount.begin(), personalCount.end(), 1);

    QList<QList<double>> lambdaByShift = getCountOfCallsByShift(db.getCallsInfoByDate(this->ui->date_start->date(), this->ui->date_end->date()));

    for(size_t i = 0; i < lambdaByShift.length(); i++)
    {
        QTableWidget* predictTable = predictTables[i];
        QTableWidget* costTable = costTables[i];

        for(size_t j = 0; j < lambdaByShift[i].length(); j++)
        {
            size_t index = 1;
            QList<QList<double>> predicts = getPredict(personalCount, 20, lambdaByShift[i][j], 12);

            for (auto&& predict : predicts)
            {
                for (auto&& characteristic : predict)
                {
                    predictTable->setItem(index, j + 2, new QTableWidgetItem(QString::number(characteristic, 'f', 2)));
                    index += 1;
                }

                double channelCost = predict[0] * cost;
                double requestCost = predict[0] * cost / predict[1];
                costTable->setItem(index - 3, j + 2, new QTableWidgetItem(QString::number(channelCost, 'f', 2)));
                costTable->setItem(index - 2, j + 2, new QTableWidgetItem(QString::number(requestCost, 'f', 2)));
            }
        }
    }
};

void MainWindow::setDbPathTriggered(void)
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), ".", tr("DB file (*.db *.sqlite)"));
    if (fileName != "")
    {
        settings.setValue("dbPath", fileName);
        settings.sync();
    }
};

void MainWindow::setHourlyPaymentTriggered(void)
{
    bool ok;
    double cost = QInputDialog::getDouble(this, "Hourly payment", "Enter hourly payment", 0, 0, 2147483647, 2, &ok);
    if (ok)
    {
        settings.setValue("channelCost", cost);
        settings.sync();
    }
};
void MainWindow::switchLangTriggered(QString lang)
{
    QTranslator translator;
    if(translator.load("gw-dss_" + lang, "./translation/"))
    {
        if(qApp->installTranslator(&translator))
        {
            this->ui->retranslateUi(this);
            settings.setValue("lang", lang);
            settings.sync();
        };
    };
};
