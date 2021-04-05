#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDate>
#include <QTableWidgetItem>
#include <QFileDialog>
#include <QInputDialog>
#include <QSettings>
#include <QTranslator>
#include <QErrorMessage>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    void initUi(void);
    void setDate(void);
    void setTablesHeaders(void);
    void setButtonsHandling(void);

private slots:
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
