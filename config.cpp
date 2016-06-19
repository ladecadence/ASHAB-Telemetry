#include "config.h"
#include "ui_config.h"

Config::Config(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Config)
{
    ui->setupUi(this);
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

}

Config::~Config()
{
    delete ui;
}

void Config::on_buttonBox_accepted()
{
    config = new QSettings("ASHAB", "Telemetry");

    // check fields
    if (!ui->serverHostEdit->text().isEmpty())
        config->setValue("tracker/url", ui->serverHostEdit->text());
}
