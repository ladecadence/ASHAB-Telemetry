#ifndef SSDVDIALOG_H
#define SSDVDIALOG_H

#include <QDialog>

namespace Ui {
class SSDVDialog;
}

class SSDVDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SSDVDialog(QWidget *parent = 0);
    ~SSDVDialog();

private:
    Ui::SSDVDialog *ui;
};

#endif // SSDVDIALOG_H
