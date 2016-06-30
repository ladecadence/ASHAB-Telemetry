#include "maxmindialog.h"
#include "ui_maxmindialog.h"

MaxMinDialog::MaxMinDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MaxMinDialog)
{
    ui->setupUi(this);

    // not initialized with good data
    init = false;

    // init values
    max.alt = 0;
    max.baro = 0;
    max.temp_ext = 0;
    max.temp_int = 0;
    max.a_rate = 0;
    max.speed = 0;

    min.alt = 0;
    min.baro = 0;
    min.temp_ext = 0;
    min.temp_int = 0;
    min.a_rate = 0;
    min.speed = 0;

    // prepare table
    // columns
    ui->tableWidget->setColumnCount(6);
    QStringList headers;
    headers << "Alt." << "Spd." << "Baro" << "T. Int." << "T. Ext" << "A. Rate";
    ui->tableWidget->setHorizontalHeaderLabels(headers);
    // rows
    ui->tableWidget->setRowCount(2);
    QStringList row_labels;
    row_labels << "MAX" << "MIN";
    ui->tableWidget->setVerticalHeaderLabels(row_labels);
    // items
    for (int i=0; i<2; i++)
        for (int j=0; j<6; j++)
        {
            QTableWidgetItem *newItem = new QTableWidgetItem(QTableWidgetItem::Type);
            if (i==0)
                newItem->setBackgroundColor(QColor::fromRgb(255,100,100));
            else
                newItem->setBackgroundColor(QColor::fromRgb(100,255,100));
            ui->tableWidget->setItem(i, j, newItem);
        }

    // initial values
    ui->tableWidget->item(0, 0)->setText(QString::number(max.alt));
    ui->tableWidget->item(0, 1)->setText(QString::number(max.speed));
    ui->tableWidget->item(0, 2)->setText(QString::number(max.baro));
    ui->tableWidget->item(0, 3)->setText(QString::number(max.temp_int));
    ui->tableWidget->item(0, 4)->setText(QString::number(max.temp_ext));
    ui->tableWidget->item(0, 5)->setText(QString::number(max.a_rate));

    ui->tableWidget->item(1, 0)->setText(QString::number(min.alt));
    ui->tableWidget->item(1, 1)->setText(QString::number(min.speed));
    ui->tableWidget->item(1, 2)->setText(QString::number(min.baro));
    ui->tableWidget->item(1, 3)->setText(QString::number(min.temp_int));
    ui->tableWidget->item(1, 4)->setText(QString::number(min.temp_ext));
    ui->tableWidget->item(1, 5)->setText(QString::number(min.a_rate));


    // check config and set filename
    config = new QSettings("ASHAB", "Telemetry");

    logFile = NULL;

    if (config->contains("log/filename")) {
        logFile = new QFile(config->value("log/filename").toString());

        // ok fill the table
        if (logFile->open(QIODevice::ReadOnly))
        {
            QTextStream in(logFile);

            // read all lines
            while (!in.atEnd())
            {
                QString line = in.readLine();
                // split line in fields
                QStringList list = line.split(";");
                Telemetry t;
                t.altitude = list.at(4);
                t.baro = list.at(8);
                t.temp_ext = list.at(7);
                t.temp_int = list.at(6);
                t.speed = list.at(10);
                t.a_rate = list.at(12);
                t.sats = list.at(11);

                // if first line, init values to this
                if (init == false && t.sats.toFloat() > 3)
                {
                    initData(&t);
                    init = true;
                }

                updateData(&t);

            }
        }
    }
    ui->tableWidget->resizeColumnsToContents();
}

MaxMinDialog::~MaxMinDialog()
{
    delete ui;
}

bool MaxMinDialog::isInit()
{
    return init;
}

void MaxMinDialog::initData(Telemetry *t)
{
    max.alt = t->altitude.toFloat();
    max.a_rate = t->a_rate.toFloat();
    max.temp_int = t->temp_int.toFloat();
    max.temp_ext = t->temp_ext.toFloat();
    max.speed = t->speed.toFloat();
    max.baro = t->baro.toFloat();

    min.alt = t->altitude.toFloat();
    min.a_rate = t->a_rate.toFloat();
    min.temp_int = t->temp_int.toFloat();
    min.temp_ext = t->temp_ext.toFloat();
    min.speed = t->speed.toFloat();
    min.baro = t->baro.toFloat();

    // initial values
    ui->tableWidget->item(0, 0)->setText(QString::number(max.alt));
    ui->tableWidget->item(0, 1)->setText(QString::number(max.speed));
    ui->tableWidget->item(0, 2)->setText(QString::number(max.baro));
    ui->tableWidget->item(0, 3)->setText(QString::number(max.temp_int));
    ui->tableWidget->item(0, 4)->setText(QString::number(max.temp_ext));
    ui->tableWidget->item(0, 5)->setText(QString::number(max.a_rate));

    ui->tableWidget->item(1, 0)->setText(QString::number(min.alt));
    ui->tableWidget->item(1, 1)->setText(QString::number(min.speed));
    ui->tableWidget->item(1, 2)->setText(QString::number(min.baro));
    ui->tableWidget->item(1, 3)->setText(QString::number(min.temp_int));
    ui->tableWidget->item(1, 4)->setText(QString::number(min.temp_ext));
    ui->tableWidget->item(1, 5)->setText(QString::number(min.a_rate));

    init = true;

    ui->tableWidget->resizeColumnsToContents();
}

void MaxMinDialog::updateData(Telemetry *t)
{
    // get max and min only if valid data
    if (t->sats.toFloat()>3)
    {
        bool ok;

        // altitude
        if (t->altitude.toFloat(&ok) > this->max.alt)
        {
            if (ok) {
                this->max.alt = t->altitude.toFloat();
                ui->tableWidget->item(0, 0)->setText(t->altitude);
            }
        }

        if (t->altitude.toFloat(&ok) < this->min.alt)
        {
            if (ok) {
                this->min.alt = t->altitude.toFloat();
                ui->tableWidget->item(1, 0)->setText(t->altitude);
            }
        }

        // baro
        if (t->baro.toFloat(&ok) > this->max.baro)
        {
            if (ok) {
                this->max.baro = t->baro.toFloat();
                ui->tableWidget->item(0, 2)->setText(t->baro);
            }
        }

        if (t->baro.toFloat(&ok) < this->min.baro)
        {
            if (ok) {
                this->min.baro = t->baro.toFloat();
                ui->tableWidget->item(1, 2)->setText(t->baro);
            }
        }

        // temp int
        if (t->temp_int.toFloat(&ok) > this->max.temp_int)
        {
            if (ok) {
                this->max.temp_int = t->temp_int.toFloat();
                ui->tableWidget->item(0, 3)->setText(t->temp_int);
            }
        }

        if (t->temp_int.toFloat(&ok)<this->min.temp_int)
        {
            if (ok) {
                this->min.temp_int = t->temp_int.toFloat();
                ui->tableWidget->item(1,3)->setText(t->temp_int);
            }
        }

        // temp ext
        if (t->temp_ext.toFloat(&ok) > this->max.temp_ext)
        {
            if (ok) {
                this->max.temp_ext = t->temp_ext.toFloat();
                ui->tableWidget->item(0, 4)->setText(t->temp_ext);
            }
        }

        if (t->temp_ext.toFloat(&ok)<this->min.temp_ext)
        {
            if (ok) {
                this->min.temp_ext = t->temp_ext.toFloat();
                ui->tableWidget->item(1,4)->setText(t->temp_ext);
            }
        }

        // speed
        if (t->speed.toFloat(&ok) > this->max.speed)
        {
            if (ok) {
                this->max.speed = t->speed.toFloat();
                ui->tableWidget->item(0, 1)->setText(t->speed);
            }
        }

        if (t->speed.toFloat(&ok)<this->min.speed)
        {
            if (ok) {
                this->min.speed = t->speed.toFloat();
                ui->tableWidget->item(1,1)->setText(t->speed);
            }
        }

        // asc. rate
        if (t->a_rate.toFloat(&ok) > this->max.a_rate)
        {
            if (ok) {
                this->max.a_rate = t->a_rate.toFloat();
                ui->tableWidget->item(0, 5)->setText(t->a_rate);
            }
        }

        if (t->a_rate.toFloat(&ok)<this->min.a_rate)
        {
            if (ok) {
                this->min.a_rate = t->a_rate.toFloat();
                ui->tableWidget->item(1,5)->setText(t->a_rate);
            }
        }

    ui->tableWidget->resizeColumnsToContents();
    }
}

