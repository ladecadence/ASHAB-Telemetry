#include "chartsdialog.h"
#include "ui_chartsdialog.h"

ChartsDialog::ChartsDialog(QWidget *parent, LogDialog *log) :
    QDialog(parent),
    ui(new Ui::ChartsDialog)
{
    ui->setupUi(this);
    logDialog = log;
}

ChartsDialog::~ChartsDialog()
{
    delete ui;
}

void ChartsDialog::on_radioButtonAltitude_toggled(bool checked)
{
    if (checked) {
        //qDebug() << logDialog->get_data(LOG_ALT)->join(":");
    }
}

void ChartsDialog::on_radioButtonTint_toggled(bool checked)
{
    if (checked) {
        qDebug() << logDialog->get_data(LOG_TINT)->join(":");
    }
}


void ChartsDialog::on_radioButtonText_toggled(bool checked)
{
    if (checked) {
        //qDebug() << logDialog->get_data(LOG_TEXT)->join(":");
    }
}


void ChartsDialog::on_radioButtonBaro_toggled(bool checked)
{
    if (checked) {
        //qDebug() << logDialog->get_data(LOG_BARO)->join(":");
    }
}


void ChartsDialog::on_radioButtonBatt_toggled(bool checked)
{
    if (checked) {
        //qDebug() << logDialog->get_data(LOG_BATT)->join(":");
    }
}


void ChartsDialog::on_radioButtonSats_toggled(bool checked)
{
    if (checked) {
        //qDebug() << logDialog->get_data(LOG_SATS)->join(":");
    }
}
