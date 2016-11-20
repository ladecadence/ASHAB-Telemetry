#ifndef SSDVDIALOG_H
#define SSDVDIALOG_H

#include <QDialog>
#include <QStringList>
#include <QStringListModel>
#include <QAbstractItemView>
#include <QFileInfo>
#include "ssdv.h"
#include "rs8.h"
#include "ssdvpicturedialog.h"

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

private slots:
    void on_listView_doubleClicked(const QModelIndex &index);

private:
    Ui::SSDVDialog *ui;
    SSDVPictureDialog *pictureDialog;
    QStringList lista_ssdv_imgs;
    QStringListModel *model;
};

#endif // SSDVDIALOG_H
