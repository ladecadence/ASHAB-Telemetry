#ifndef LOGDIALOG_H
#define LOGDIALOG_H

#include <QDialog>
#include <QSettings>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QClipboard>
#include <QStringList>

#define LOG_DATE 0
#define LOG_HOUR 1
#define LOG_LAT 2
#define LOG_LON 3
#define LOG_ALT 4
#define LOG_BATT 5
#define LOG_TINT 6
#define LOG_TOUT 7
#define LOG_HDG 8
#define LOG_VELO 9
#define LOG_SATS 10
#define LOG_ARATE 11

namespace Ui {
class LogDialog;
}

class LogDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LogDialog(QWidget *parent = 0);
    ~LogDialog();
    void loadData();
    QStringList* get_data(int index);

private slots:
    void on_buttonBox_accepted();
    void on_tableWidget_itemSelectionChanged();

private:
    QSettings *config;
    QFile *logFile;
    Ui::LogDialog *ui;
};

#endif // LOGDIALOG_H
