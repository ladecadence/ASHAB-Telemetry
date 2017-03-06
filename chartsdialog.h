#ifndef CHARTSDIALOG_H
#define CHARTSDIALOG_H

#include <QDialog>
#include "logdialog.h"

namespace Ui {
class ChartsDialog;
}

class ChartsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ChartsDialog(QWidget *parent = 0, LogDialog *log = NULL);
    ~ChartsDialog();

private slots:
    void on_radioButtonAltitude_toggled(bool checked);

    void on_radioButtonTint_toggled(bool checked);

    void on_radioButtonText_toggled(bool checked);

    void on_radioButtonBaro_toggled(bool checked);

    void on_radioButtonBatt_toggled(bool checked);

    void on_radioButtonSats_toggled(bool checked);

private:
    Ui::ChartsDialog *ui;
    LogDialog *logDialog;
};

#endif // CHARTSDIALOG_H