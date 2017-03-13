#include "ssdvpicturedialog.h"
#include "ui_ssdvpicturedialog.h"

SSDVPictureDialog::SSDVPictureDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SSDVPictureDialog)
{
    ui->setupUi(this);
    // scene
    scn = new QGraphicsScene();
}

SSDVPictureDialog::~SSDVPictureDialog()
{
    delete ui;
    delete scn;
}

void SSDVPictureDialog::on_buttonBox_accepted()
{

}

void SSDVPictureDialog::loadPicture(QString picturePath)
{
    pictureName = picturePath;
    ui->labelPath->setText(picturePath);
    QPixmap pixmap(picturePath);
    ui->graphicsView->setScene(scn);
    scn->addPixmap(pixmap);
    //ui->graphicsView->fitInView(scn->itemsBoundingRect(),Qt::KeepAspectRatio);
    //ui->graphicsView->scale(ui->graphicsView->width()/scn->width(), ui->graphicsView->height()/scn->height());

}

void SSDVPictureDialog::on_uploadButton_clicked()
{
    // check config and set filename
    config = new QSettings("ASHAB", "Telemetry");

    // check for auth data
    if (config->contains("tracker/user") && config->contains("tracker/password") &&
            config->contains("tracker/url"))
    {
        // disable upload button
        ui->uploadButton->setEnabled(false);
        ui->labelUpload->setText("");
        QNetworkAccessManager *networkManager = new QNetworkAccessManager(this);

        QString bound="margin";
        QByteArray data(QString("--" + bound + "\r\n").toLocal8Bit());
        data.append("Content-Disposition: form-data; name=\"image\"\r\n\r\n");
        data.append(pictureName + QString("\r\n"));
        data.append(QString("--" + bound + "\r\n").toLocal8Bit());
        data.append("Content-Disposition: form-data; name=\"uploaded\"; filename=\"");
        data.append(pictureName);
        data.append("\"\r\n");
        data.append("Content-Type: image/jpg\r\n\r\n"); //data type

        QFile file(pictureName);
        if (!file.open(QIODevice::ReadOnly)){
            return;
        }

        data.append(file.readAll());
        data.append("\r\n");
        data.append("--" + bound + "--\r\n");  //closing boundary according to rfc 1867

        file.close();


        // Auth
        QString concatenated = config->value("tracker/user").toString() + ":" + config->value("tracker/password").toString();
        QByteArray authData = concatenated.toLocal8Bit().toBase64();
        QString headerData = "Basic " + authData;

        QNetworkRequest request(config->value("tracker/url").toString());
        request.setRawHeader("Authorization", headerData.toLocal8Bit());

        //request.setHeader(QNetworkRequest::ContentTypeHeader,
        //                  "application/x-www-form-urlencoded");

        request.setRawHeader(QString("Content-Type").toLocal8Bit(),QString("multipart/form-data; boundary=" + bound).toLocal8Bit());
        request.setRawHeader(QString("Content-Length").toLocal8Bit(), QString::number(data.length()).toLocal8Bit());

        //connect(networkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(onPostAnswer(QNetworkReply*)));
        reply = networkManager->post(request, data);
        connect(reply, SIGNAL(finished()), this, SLOT  (onPostAnswer()));

        //multiPart->setParent(reply);

        // work done, clean
        // delete networkManager;
    }

    delete config;
}

void SSDVPictureDialog::onPostAnswer()
{
    // enable button
    ui->uploadButton->setEnabled(true);

    QString replyText = QString::fromUtf8(reply->readAll().constData());
    fprintf(stderr, "\n----->>>> %s", replyText.toLocal8Bit().constData());
    if (replyText.contains("You can pass") && replyText.contains("has been uploaded")) {
        fprintf(stderr, "+++ Image Uploaded!");
        ui->labelUpload->setText("Uploaded.");
    }
    else if (replyText.contains("error")) {
        ui->labelUpload->setText("Error uploading");
    }
}
