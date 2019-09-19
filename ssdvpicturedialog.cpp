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
}

void SSDVPictureDialog::on_uploadButton_clicked()
{
    // check config and set filename
    config = new QSettings("ASHAB", "Telemetry");

    // check for auth data
    if (config->contains("tracker/user")
            && config->contains("tracker/password")
            && config->contains("tracker/url"))
    {
        // disable upload button
        ui->uploadButton->setEnabled(false);
        ui->labelUpload->setText("");

        networkManager = new QNetworkAccessManager(this);

		// Setup the webservice url
        QUrlQuery postData;

        // add the data
        postData.addQueryItem("telemetry", "");
		QFileInfo fi(pictureName);
		qDebug() << fi.fileName();
        postData.addQueryItem("image", fi.fileName());
        postData.addQueryItem("database", "");

		// image data
		QFile file(pictureName);
        if (!file.open(QIODevice::ReadOnly)){
            return;
        }


		QByteArray imageData = file.readAll().toBase64();
		file.close();


        // Auth
        QString concatenated = config->value("tracker/user").toString()
                + ":" + config->value("tracker/password").toString();
        QByteArray data = concatenated.toLocal8Bit().toBase64();
        QString headerData = "Basic " + data;

        //QNetworkRequest request(config->value("tracker/url").toString());
        QNetworkRequest request;

		QUrl url(config->value("tracker/url").toString());
		url.setQuery(postData);
		request.setUrl(url);

     	request.setRawHeader("Authorization", headerData.toLocal8Bit());

		request.setHeader(QNetworkRequest::ContentTypeHeader,
							"text/plain");
        //                    "application/x-www-form-urlencoded");


        connect(networkManager, SIGNAL(finished(QNetworkReply*)),
                this, SLOT(onPostAnswer(QNetworkReply*)));

		// make the post
		qDebug() << "REQUEST---------------------";
		qDebug() << request.url().toString();
  		const QList<QByteArray>& rawHeaderList(request.rawHeaderList());
  		foreach (QByteArray rawHeader, rawHeaderList) {
    		qDebug() << request.rawHeader(rawHeader);
  		}
		qDebug() << "---------------------";

		networkManager->post(request, imageData);

    }

    delete config;
}

void SSDVPictureDialog::onPostAnswer(QNetworkReply* reply)
{
    // enable button
    ui->uploadButton->setEnabled(true);

    QString replyText = QString::fromUtf8(reply->readAll().constData());
    fprintf(stderr, "\n----->>>> %s", replyText.toLocal8Bit().constData());
    if (replyText.contains("You can pass")
            && replyText.contains("has been uploaded")) {
        fprintf(stderr, "+++ Image Uploaded!");
        ui->labelUpload->setText("Uploaded.");
    }
    else if (replyText.contains("error")) {
        ui->labelUpload->setText("Error uploading");
    }

    // clean
    reply->deleteLater();
    networkManager->deleteLater();
}
