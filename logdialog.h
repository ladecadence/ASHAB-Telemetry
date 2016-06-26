#ifndef LOGDIALOG_H
#define LOGDIALOG_H

#include <QDialog>
#include <QSettings>
#include <QFile>
#include <QTextStream>

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

private slots:
    void on_buttonBox_accepted();

private:
    QSettings *config;
    QFile *logFile;
    Ui::LogDialog *ui;
};

#endif // LOGDIALOG_H