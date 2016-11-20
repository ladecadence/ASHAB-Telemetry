#include "ssdvdialog.h"
#include "ui_ssdvdialog.h"

SSDVDialog::SSDVDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SSDVDialog)
{
    ui->setupUi(this);
}

SSDVDialog::~SSDVDialog()
{
    delete ui;
}
