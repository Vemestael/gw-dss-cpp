#include "controllers/hpp/mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QFile file(":/styles/nord.qss");
    file.open(QFile::ReadOnly);
    QString style = QLatin1String(file.readAll());
    qApp->setStyleSheet(style);

    MainWindow w;
    w.show();
    return a.exec();
}