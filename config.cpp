#include "config.h"
#include "ui_config.h"

Config::Config(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Config)
{
    ui->setupUi(this);

    // read configuration values
    config = new QSettings("ASHAB", "Telemetry");
    if (config->contains("direwolf/ip"))
    {
        ui->direwolfHostEdit->setText(config->value("direwolf/ip").toString());
    } else {
        ui->direwolfHostEdit->setText("127.0.0.1");
    }

    if (config->contains("direwolf/port"))
    {
        ui->direwolfPortEdit->setText(config->value("direwolf/port").toString());
    } else {
        ui->direwolfPortEdit->setText(QString::number(8000));
    }

    if (config->contains("tracker/url"))
    {
        ui->serverHostEdit->setText(config->value("tracker/url").toString());
    } else {
        ui->serverHostEdit->setText("http://ashab.space/tracker/upload.php");
    }

    if (config->contains("tracker/user"))
    {
        ui->serverUserEdit->setText(config->value("tracker/user").toString());
    }

    if (config->contains("tracker/password"))
    {
        ui->serverPassEdit->setText(config->value("tracker/password").toString());
    }

    if (config->contains("log/filename"))
    {
        ui->logFileEdit->setText(config->value("log/filename").toString());
    }

}

Config::~Config()
{
    delete ui;
}

void Config::on_buttonBox_accepted()
{
    config = new QSettings("ASHAB", "Telemetry");

    // check fields and update configuration
    if (!ui->direwolfHostEdit->text().isEmpty())
        config->setValue("direwolf/ip", ui->direwolfHostEdit->text());

    if (!ui->direwolfPortEdit->text().isEmpty())
        config->setValue("direwolf/port", ui->direwolfPortEdit->text());

    if (!ui->serverHostEdit->text().isEmpty())
        config->setValue("tracker/url", ui->serverHostEdit->text());

    if (!ui->serverUserEdit->text().isEmpty())
        config->setValue("tracker/user", ui->serverUserEdit->text());

    if (!ui->serverPassEdit->text().isEmpty())
        config->setValue("tracker/password", ui->serverPassEdit->text()); 

    if (!ui->logFileEdit->text().isEmpty())
        config->setValue("log/filename", ui->logFileEdit->text());
}

void Config::on_logFileButton_clicked()
{

    QString fileName = QFileDialog::getSaveFileName(this,
        tr("Select Log File"), QDir::homePath(), tr("Log Files (*.txt *.log)"));
    if (fileName != "")
    {
        ui->logFileEdit->setText(fileName);
    }
}
