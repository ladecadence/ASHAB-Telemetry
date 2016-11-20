#include "ssdvdialog.h"
#include "ui_ssdvdialog.h"

SSDVDialog::SSDVDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SSDVDialog)
{
    ui->setupUi(this);

    model = new QStringListModel(this);
    model->setStringList(lista_ssdv_imgs);
    ui->listView->setModel(model);
}

SSDVDialog::~SSDVDialog()
{
    delete ui;
}

void SSDVDialog::updateStatus(QString status)
{
    ui->ssdvStatusLabel->setText(status);
}

void SSDVDialog::addImageSSDV(QString path){
    if (!lista_ssdv_imgs.contains(path))
    {
        lista_ssdv_imgs.append(path);
        model->setStringList(lista_ssdv_imgs);
    }
}
