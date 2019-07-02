#ifndef SSDVPICTUREDIALOG_H
#define SSDVPICTUREDIALOG_H

#include <QDialog>
#include <QSettings>
#include <QUrlQuery>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QHttpMultiPart>
#include <QHttpPart>
#include <QFile>
#include <QFileInfo>
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
    void on_uploadButton_clicked();
    void onPostAnswer(QNetworkReply *reply);

private:
    Ui::SSDVPictureDialog *ui;
    QSettings *config;
    QGraphicsScene *scn;
    QString pictureName;
    QNetworkAcessManager *manager;
    QNetworkReply *reply;
};

#endif // SSDVPICTUREDIALOG_H
