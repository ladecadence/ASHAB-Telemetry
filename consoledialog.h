#ifndef CONSOLEDIALOG_H
#define CONSOLEDIALOG_H

#include <QDialog>
#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include "telemetry.h"

namespace Ui {
class ConsoleDialog;
}

class ConsoleDialog : public QDialog
{
    Q_OBJECT 

public:
    explicit ConsoleDialog(QWidget *parent = 0);
    ~ConsoleDialog();
    void append(QString string);
    void clear();

private:
    Ui::ConsoleDialog *ui;
};

#endif // CONSOLEDIALOG_H
