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
    // Google Maps
    //QString url =  QString::fromUtf8("http://maps.google.com/maps?z=12&t=m&q=loc:") +
    //                lat + QString::fromUtf8("+") + lon;

    // test
    //lat=QString::fromUtf8("43.525433N");
    //lon=QString::fromUtf8("005.667330W");

    // OpenStreetMap
    if (lat.contains("S"))
        lat.prepend(("-"));
    if (lon.contains("W"))
        lon.prepend("-");
    QString url = QString::fromUtf8("http://www.openstreetmap.org/?mlat=") + lat +
            QString::fromUtf8("&mlon=") + lon + QString::fromUtf8("&zoom=12");

    ui->webView->load(QUrl(url));

}
