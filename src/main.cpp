#include <QApplication>

#include <gw-dss-cpp/controllers/mainwindow.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QFile file(":/styles/nord.qss");
    file.open(QFile::ReadOnly);
    QString style = QLatin1String(file.readAll());
    qApp->setStyleSheet(style);

    a.setWindowIcon(QIcon(":/icons/icon-64.png"));

    MainWindow w;
    w.show();
    return a.exec();
}