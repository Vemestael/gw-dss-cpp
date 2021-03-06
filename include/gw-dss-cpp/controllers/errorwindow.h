#ifndef ERRORWINDOW_H
#define ERRORWINDOW_H

#include <QWidget>
#include <QString>
#include <QErrorMessage>

class ErrorWindow : public QWidget
{
    Q_OBJECT
    
public:
    explicit ErrorWindow(QString const &msg);
    ~ErrorWindow(void) = default;
};

#endif // ERRORWINDOW_H
