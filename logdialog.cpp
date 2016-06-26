#include "logdialog.h"
#include "ui_logdialog.h"

LogDialog::LogDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LogDialog)
{
    ui->setupUi(this);
}

void LogDialog::loadData()
{

    // clear the table
    while (ui->tableWidget->rowCount()>0)
        ui->tableWidget->removeRow(0);

     // check config and set filename
    config = new QSettings("ASHAB", "Telemetry");

    logFile = NULL;

    if (config->contains("log/filename")) {
        logFile = new QFile(config->value("log/filename").toString());
        ui->logFileLabel->setText(config->value("log/filename").toString());
    }
    // prepare table
    ui->tableWidget->setColumnCount(13);
    QStringList headers;
    headers << "Fecha" << "Hora" << "Lat." << "Lon." << "Alt.";
    headers << "Bat." << "Temp. Int." << "Temp. Ext" << "Baro.";
    headers << "Hdg." << "Vel" << "Sats" << "Ratio Asc.";
    ui->tableWidget->setHorizontalHeaderLabels(headers);

    // ok fill the table
    if (logFile->open(QIODevice::ReadOnly))
    {
        QTextStream in(logFile);

        // read all lines
        int lineCount = 0;
        while (!in.atEnd())
        {
            QString line = in.readLine();
            // split line in fields
            QStringList list = line.split(";");
            // create row
            ui->tableWidget->insertRow(lineCount);
            for (int i=0; i<13; i++)
            {
                // insert items in row
                QTableWidgetItem *item = new QTableWidgetItem(list.at(i));
                ui->tableWidget->setItem(lineCount, i, item);
            }
            lineCount++;
        }

    }

    ui->tableWidget->resizeColumnsToContents();
}

LogDialog::~LogDialog()
{
    delete ui;
}

void LogDialog::on_buttonBox_accepted()
{

}
