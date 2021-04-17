#pragma once
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <numeric>

#include <QMainWindow>
#include <QDate>
#include <QTableWidgetItem>
#include <QFileDialog>
#include <QInputDialog>
#include <QSettings>
#include <QTranslator>
#include <QString>
#include <QList>
#include <QVector>
#include <QLocale>

#include "../../analysis/hpp/dbapi.h"
#include "../../analysis/hpp/dataprocessing.h"
#include "../../analysis/hpp/predict.h"
#include "verticatextdelegate.h"
#include "chartwindow.h"

#define __APPLICATION_NAME__ "gw-dss-cpp"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    DbApi db;
    QSettings settings;
    ChartWindow *cw;

    // window preparing
    void loadWindow(void);
    void loadSettings(void);
    void setDate(void);
    void setTablesHeaders(void);
    void setButtonsHandling(void);

    // pressed event handlers
    void allTimePressed(void);
    void lastYearPressed(void);
    void lastHalfYearPressed(void);
    void lastQuartYearPressed(void);
    void lastMonthPressed(void);
    void analyzePressed(void);

    // triggered menu handlers
    void setDbPathTriggered(void);
    void setHourlyPaymentTriggered(void);
    void setServedRequestsTriggered(void);
    void setStaffNumberTriggered(void);
    void setMaxQueueLengthTriggered(void);
    void switchLangTriggered(QString const &lang);
    void showChartTriggered(ChartType type);
};
#endif // MAINWINDOW_H
