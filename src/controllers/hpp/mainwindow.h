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

    void loadSettings(void);
    void initUi(void);
    void setDate(void);
    void setTablesHeaders(void);
    void setButtonsHandling(void);

    void allTimePressed(void);
    void lastYearPressed(void);
    void lastHalfYearPressed(void);
    void lastQuartYearPressed(void);
    void lastMonthPressed(void);
    void analyzePressed(void);

    void setDbPathTriggered(void);
    void setHourlyPaymentTriggered(void);
    void switchLangTriggered(QString lang);
};
#endif // MAINWINDOW_H
