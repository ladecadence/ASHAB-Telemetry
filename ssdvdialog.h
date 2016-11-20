#ifndef SSDVDIALOG_H
#define SSDVDIALOG_H

#include <QDialog>
#include <QStringList>
#include <QStringListModel>

namespace Ui {
class SSDVDialog;
}

class SSDVDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SSDVDialog(QWidget *parent = 0);
    ~SSDVDialog();
    void updateStatus(QString status);
    void addImageSSDV(QString path);

private:
    Ui::SSDVDialog *ui;
    QStringList lista_ssdv_imgs;
    QStringListModel *model;
};

#endif // SSDVDIALOG_H
