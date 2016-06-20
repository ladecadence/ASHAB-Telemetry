#ifndef CONFIG_H
#define CONFIG_H

#include <QDialog>
#include <QSettings>
#include <QFileDialog>

namespace Ui {
class Config;
}

class Config : public QDialog
{
    Q_OBJECT

public:
    explicit Config(QWidget *parent = 0);
    ~Config();

private slots:
    void on_buttonBox_accepted();

    void on_logFileButton_clicked();

private:
    Ui::Config *ui;
    QSettings *config;
};

#endif // CONFIG_H
