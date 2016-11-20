#include "config.h"
#include "ui_config.h"

Config::Config(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Config)
{
    ui->setupUi(this);

    // get serial ports
    Q_FOREACH(QSerialPortInfo port, QSerialPortInfo::availablePorts()) {
            ui->serialPortCombo->addItem(port.systemLocation());
    }

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

    if (config->contains("lora/port"))
    {
        ui->serialPortCombo->setCurrentText(config->value("lora/port").toString());
    }

    if (config->contains("lora/imgpath"))
    {
        ui->imgPathEdit->setText(config->value("lora/imgpath").toString());
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

    if (config->contains("tracker/database"))
    {
        ui->serverDbEdit->setText(config->value("tracker/database").toString());
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

    if (!ui->serialPortCombo->currentText().isEmpty())
        config->setValue("lora/port", ui->serialPortCombo->currentText());

    if (!ui->imgPathEdit->text().isEmpty())
        config->setValue("lora/imgpath", ui->imgPathEdit->text());

    if (!ui->serverHostEdit->text().isEmpty())
        config->setValue("tracker/url", ui->serverHostEdit->text());

    if (!ui->serverUserEdit->text().isEmpty())
        config->setValue("tracker/user", ui->serverUserEdit->text());

    if (!ui->serverPassEdit->text().isEmpty())
        config->setValue("tracker/password", ui->serverPassEdit->text());

    if (!ui->serverDbEdit->text().isEmpty())
        config->setValue("tracker/database", ui->serverDbEdit->text());

    // if it's empty but we have a config value, it was erased, erase it.
    if (ui->logFileEdit->text().isEmpty() && config->contains("log/filename"))
        config->remove("log/filename");
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

void Config::on_imgPathButton_clicked()
{
    QString imgPath = QFileDialog::getExistingDirectory(this,
                                                   tr("Select SSDV Images folder"),
                                                   QDir::homePath(),
                                                   QFileDialog::ShowDirsOnly);

    if (imgPath != "")
    {
        ui->imgPathEdit->setText(imgPath);
    }
}
