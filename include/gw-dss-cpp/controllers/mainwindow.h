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
#include <QGraphicsView>
#include <QGraphicsScene>

#include <qcustomplot/qcustomplot.h>

#include <gw-dss-cpp/analysis/dbapi.h>
#include <gw-dss-cpp/analysis/dataprocessing.h>
#include <gw-dss-cpp/analysis/predict.h>

#include <gw-dss-cpp/controllers/verticaltextdelegate.h>
#include <gw-dss-cpp/controllers/chartwindow.h>
#include <gw-dss-cpp/controllers/errorwindow.h>

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
    QVector<QVector<double>> servedReqArr;
    QVector<QVector<double>> unservedReqArr;
    QVector<QVector<double>> queueLenArr;
    QVector<QVector<double>> reqCostArr;
    QVector<QVector<double>> staffCostArr;
    QCustomPlot *customPlot;

    // window preparing
    void loadWindow(void);
    void loadSettings(void);
    void setDate(void);
    void setSpinBoxValue(void);
    void setButtonsHandling(void);

    // pressed event handlers
    void allTimePressed(void);
    void lastYearPressed(void);
    void lastHalfYearPressed(void);
    void lastQuartYearPressed(void);
    void lastMonthPressed(void);
    void analyzePressed(void);

    // combo box handlers
    void graphTypeChanged(void);
    void additionalGraphChanged(QComboBox *obj);

    // check box handlers
    void addGraphChecked(int state);

    // spin box handlers
    template<typename spinBox>
    void spinBoxChanged(spinBox obj);

    // triggered menu handlers
    void setDbPathTriggered(void);
    void setHourlyPaymentTriggered(void);
    void setRequestCostTriggered(void);
    void setServedRequestsTriggered(void);
    void setStaffNumberTriggered(void);
    void setMaxQueueLengthTriggered(void);
    void switchLangTriggered(QString const &lang);
    void showChartTriggered(ChartType type);

    // plot handlers
    void setPlotSettings(QCustomPlot *customPlot);
    void plotGraph(QCustomPlot *customPlot, QString const &name, QVector<double> data);
};

template<typename spinBox>
void MainWindow::spinBoxChanged(spinBox obj)
{
    this->settings.setValue(obj->objectName(), obj->value());
    this->settings.sync();
};

#endif // MAINWINDOW_H
