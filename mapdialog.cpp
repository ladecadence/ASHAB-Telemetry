#include "mapdialog.h"
#include "ui_mapdialog.h"

MapDialog::MapDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MapDialog)
{
    ui->setupUi(this);
}

MapDialog::~MapDialog()
{
    delete ui;
}

void MapDialog::updateMap(QString lat, QString lon)
{
    QString url =  QString::fromUtf8("http://maps.google.com/maps?z=12&t=m&q=loc:") +
                    lat + QString::fromUtf8("+") + lon;
    ui->webView->load(QUrl(url));

}
