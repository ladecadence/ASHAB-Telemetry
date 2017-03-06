#ifndef MAXMINDIALOG_H
#define MAXMINDIALOG_H

#include <QDialog>
#include <QSettings>
#include <QFile>
#include <QTextStream>
#include "telemetry.h"
#include "logdialog.h"

struct max_min
{
    float alt;
    float temp_int;
    float temp_ext;
    float baro;
    float speed;
    float a_rate;

};

typedef struct max_min max_min_t;

namespace Ui {
class MaxMinDialog;
}

class MaxMinDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MaxMinDialog(QWidget *parent = 0);
    ~MaxMinDialog();
    void initData(Telemetry* t);
    void updateData(Telemetry* t);
    bool isInit();

private:
    Ui::MaxMinDialog *ui;
    max_min_t max;
    max_min_t min;
    QSettings *config;
    QFile *logFile;
    bool init;
};

#endif // MAXMINDIALOG_H
