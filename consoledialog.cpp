#include "consoledialog.h"
#include "ui_consoledialog.h"

ConsoleDialog::ConsoleDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConsoleDialog)
{
    ui->setupUi(this);
    ui->textEdit->setTextColor(QColor::fromRgbF(0,1,0,1));
    ui->textEdit->append("ASHAB Telemetry. Build date: " + QString::fromLocal8Bit(__DATE__));
    ui->textEdit->append("\n");
}

ConsoleDialog::~ConsoleDialog()
{
    delete ui;
}


void ConsoleDialog::clear()
{
    ui->textEdit->clear();
}

void ConsoleDialog::append(QString str)
{   
    ui->textEdit->append(str);
}
