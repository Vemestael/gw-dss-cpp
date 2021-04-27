#include <gw-dss-cpp/controllers/errorwindow.h>

ErrorWindow::ErrorWindow(QString const &msg)
{
    QErrorMessage errDialog;
    errDialog.setWindowModality(Qt::ApplicationModal);
    errDialog.setWindowTitle(tr("Error"));
    errDialog.showMessage(msg);
    errDialog.exec(); 
};