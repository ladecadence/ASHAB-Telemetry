#ifndef SSDVPICTUREDIALOG_H
#define SSDVPICTUREDIALOG_H

#include <QDialog>
#include <QGraphicsScene>

namespace Ui {
class SSDVPictureDialog;
}

class SSDVPictureDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SSDVPictureDialog(QWidget *parent = 0);
    ~SSDVPictureDialog();
    void loadPicture(QString picturePath);

private slots:
    void on_buttonBox_accepted();

private:
    Ui::SSDVPictureDialog *ui;
    QGraphicsScene *scn;
};

#endif // SSDVPICTUREDIALOG_H
