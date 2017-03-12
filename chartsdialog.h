#ifndef CHARTSDIALOG_H
#define CHARTSDIALOG_H

#include <QDialog>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCore/QDateTime>
#include <QtCharts/QDateTimeAxis>
#include <QtCharts/QValueAxis>
#include "logdialog.h"

QT_CHARTS_USE_NAMESPACE

namespace Ui {
class ChartsDialog;
}

class ChartsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ChartsDialog(QWidget *parent = 0, LogDialog *log = NULL);
    ~ChartsDialog();
    void update_chart();

private slots:
    void on_radioButtonAltitude_toggled(bool checked);

    void on_radioButtonTint_toggled(bool checked);

    void on_radioButtonText_toggled(bool checked);

    void on_radioButtonBaro_toggled(bool checked);

    void on_radioButtonBatt_toggled(bool checked);

    void on_radioButtonSats_toggled(bool checked);

    void on_radioButtonArate_toggled(bool checked);

private:
    Ui::ChartsDialog *ui;
    LogDialog *logDialog;
    QLineSeries *lineSeries;
    QChart *chart;
    int chart_type;
    QDateTimeAxis *axisX;
    QValueAxis *axisY;
    QStringList *chartTitles;
    QStringList *chartValues;
};

#endif // CHARTSDIALOG_H
