#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    QString ip;
    quint16 port;
    QString url;
    QString user;
    QString passwd;

    // check config and init values if empty
    config = new QSettings("ASHAB", "Telemetry");

    if (config->contains("direwolf/ip"))
    {
        ip = config->value("direwolf/ip").toString();
    } else {
        ip = "127.0.0.1";
        config->setValue("direwolf/ip", ip);
    }

    if (config->contains("direwolf/port"))
    {
        port = config->value("direwolf/port").toInt();
    } else {
        port = 8000;
        config->setValue("direwolf/port", port);
    }

    if (config->contains("tracker/url"))
    {
        url = config->value("tracker/url").toInt();
    } else {
        url = "http://ashab.space/tracker/upload-couchdb.php";
        config->setValue("tracker/url", url);
    }


    // start app
    // AWG packet engine
    lastPacket = new QDateTime(QDateTime::currentDateTimeUtc());
    timer = new QTimer();
    connect(timer, SIGNAL(timeout()), SLOT(updatePacketTime()));
    timer->start(1000);
    connectTcp(ip, port);

    telemetry = new Telemetry();

    // Create UI
    aboutDialog = new About(this);
    configDialog = new Config(this);
    logDialog = new LogDialog(this);
    maxMinDialog = new MaxMinDialog(this);

    ui->setupUi(this);

    ui->graphicsView->setStyleSheet("background: transparent;");
    QPixmap pixmap(":/Images/images/ashab-small.png");
    QGraphicsScene *scn = new QGraphicsScene(); // object defined in header
    ui->graphicsView->setScene(scn);
    scn->addPixmap(pixmap);
    ui->graphicsView->fitInView(scn->itemsBoundingRect(),Qt::KeepAspectRatio);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete telemetry;
    delete timer;
    delete lastPacket;
    delete config;
    delete awgSocket;
}

void MainWindow::connectTcp(QString host, qint16 port)
{
    // create AWG "m" message to listen to all packets
    QByteArray m_message(36, 0);
    m_message[4] = 'm';

    // connect and send message
    awgSocket = new QTcpSocket(this);
    awgSocket->connectToHost(host, port);
    // set slot
    connect (awgSocket, SIGNAL(readyRead()), SLOT(readAwgData()));
    if(awgSocket->waitForConnected() ) {
        awgSocket->write(m_message);
    }

}

void MainWindow::readAwgData()
{
    // check for valid data
    QByteArray inData;
    while (awgSocket->bytesAvailable() >0) {
        char c;
        awgSocket->getChar(&c);
        if (c!=0)
            inData.append(c);
    }
    fprintf(stderr, ">>> %s\n", inData.constData());

    // parse data
    bool parsed = telemetry->parseData(QString::fromLocal8Bit(inData.constData()));
    if (parsed) {
        if (telemetry->sats.toInt()>3) {
            QString latitude = QString::fromUtf8("Latitud: ") +
                    QString::fromUtf8("<a href=\"http://maps.google.com/maps?z=12&t=m&q=loc:") +
                    telemetry->latitude + QString::fromUtf8("+") + telemetry->longitude +
                    QString::fromUtf8("\">") +
                    telemetry->latitude + QString::fromUtf8("</a>");
            QString longitude = QString::fromUtf8("Longitud: ") +
                    QString::fromUtf8("<a href=\"http://maps.google.com/maps?z=12&t=m&q=loc:") +
                    telemetry->latitude + QString::fromUtf8("+") + telemetry->longitude +
                    QString::fromUtf8("\">") +
                    telemetry->longitude + QString::fromUtf8("</a>");

            ui->labelLat->setText(latitude);
            ui->labelLon->setText(longitude);
        }
        else
        {
            ui->labelLat->setText("Latitud: " + telemetry->latitude);
            ui->labelLon->setText("Longitud: " + telemetry->longitude);
        }
        ui->labelAlt->setText("Altitud: " + telemetry->altitude + " m");
        ui->labelHdg->setText("Heading: " + telemetry->heading + QString::fromUtf8(" º"));
        ui->labelSpd->setText("Velocidad: " + telemetry->speed + " kn");
        ui->labelBaro->setText("Baro: " + telemetry->baro + " mb");
        ui->labelBatt->setText(QString::fromUtf8("Batería: ") + telemetry->voltage + " V");
        ui->labelTin->setText("Temp. Interna: " + telemetry->temp_int + QString::fromUtf8(" ºC"));
        ui->labelTout->setText("Temp. Externa: " + telemetry->temp_ext + QString::fromUtf8(" ºC"));
        ui->labelSats->setText(QString::fromUtf8("Satélites GPS: ") + telemetry->sats);
        ui->labelAsr->setText("Ratio Ascenso: " + telemetry->a_rate + " m/s");


        // update max and min
        if (!maxMinDialog->isInit())
            maxMinDialog->initData(telemetry);
        maxMinDialog->updateData(telemetry);

        // let's log
        if (config->contains("log/filename"))
        {
            QFile log(config->value("log/filename").toString());
            if (log.open(QFile::Append)) {
                QTextStream out(&log);
                out << telemetry->toString() << endl;
                out.flush();
                log.close();
            }
        }

        // and upload
        uploadTelemetry();


    }
    delete lastPacket;
    lastPacket = new QDateTime(QDateTime::currentDateTimeUtc());

}

void MainWindow::updatePacketTime()
{
    QDateTime *now = new QDateTime(QDateTime::currentDateTimeUtc());
    int seconds = lastPacket->secsTo(*now);
    QString message = QString::fromUtf8("Último paquete hace: ");
    if (seconds > 59) {
        int minutes = seconds / 60;
        seconds = seconds % 60;
        message.append(QString::number(minutes));
        message.append(" m, ");
        message.append(QString::number(seconds));
        message.append(" s.");
    }
    else
    {
        message.append(QString::number(seconds));
        message.append(" s.");
    }
    ui->statusBar->showMessage(message);

    // free mem
    delete now;

}

void MainWindow::on_actionAcerca_de_triggered()
{
    aboutDialog->exec();
}

void MainWindow::on_actionSalir_triggered()
{
    qApp->exit();
}

void MainWindow::on_actionConfigurar_triggered()
{
    // show config dialog
    int configChanged = configDialog->exec();

    // if Ok was pressed
    if (configChanged)
    {
        QString ip;
        quint16 port;

        if (config->contains("direwolf/ip"))
        {
            ip = config->value("direwolf/ip").toString();
        } else {
            ip = "127.0.0.1";
            config->setValue("direwolf/ip", ip);
        }

        if (config->contains("direwolf/port"))
        {
            port = config->value("direwolf/port").toInt();
        } else {
            port = 8000;
            config->setValue("direwolf/port", port);
        }

        // reconnect AGW
        awgSocket->close();
        connectTcp(ip, port);

    }

}

void MainWindow::uploadTelemetry()
{
    // check for auth data
    if (config->contains("tracker/user") && config->contains("tracker/password") &&
            config->contains("tracker/database"))
    {
        QNetworkAccessManager *networkManager = new QNetworkAccessManager(this);

        // Setup the webservice url
        QUrlQuery postData;

        // add the data
        postData.addQueryItem("telemetry", telemetry->toString());
        postData.addQueryItem("database", config->value("tracker/database").toString());
        fprintf(stderr, "%s\n", telemetry->toString().toLocal8Bit().constData());

        // Auth
        QString concatenated = config->value("tracker/user").toString() + ":" + config->value("tracker/password").toString();
        QByteArray data = concatenated.toLocal8Bit().toBase64();
        QString headerData = "Basic " + data;

        QNetworkRequest request(config->value("tracker/url").toString());
        request.setRawHeader("Authorization", headerData.toLocal8Bit());

        request.setHeader(QNetworkRequest::ContentTypeHeader,
            "application/x-www-form-urlencoded");
        connect(networkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(onPostAnswer(QNetworkReply*)));
        networkManager->post(request, postData.toString(QUrl::FullyEncoded).toUtf8());


    }


}

void MainWindow::onPostAnswer(QNetworkReply* reply)
{
    QString replyText = QString::fromUtf8(reply->readAll().constData());
    fprintf(stderr, "----->>>> %s", replyText.toLocal8Bit().constData());
    if (replyText.contains("You can pass"))
            fprintf(stderr, "+++ Uploaded!");
}

void MainWindow::on_actionLog_triggered()
{
    logDialog->loadData();
    logDialog->show();
}


void MainWindow::on_labelLat_customContextMenuRequested(const QPoint &pos)
{
        QMenu *menu = new QMenu;
        menu->addAction(tr("Open OpenStreetMap"), this, SLOT(openOpenStreetMap()));
        menu->addAction(tr("Copy OpenStreetMap"), this, SLOT(copyOpenStreetMap()));
        menu->addSeparator();
        menu->addAction(tr("Open Google Maps"), this, SLOT(openGoogleMaps()));
        menu->addAction(tr("Copy Google Maps"), this, SLOT(copyGoogleMaps()));
        menu->exec(this->mapToGlobal(pos));
}

void MainWindow::copyGoogleMaps()
{
    QString url =  QString::fromUtf8("http://maps.google.com/maps?z=14&t=m&q=loc:") +
                    telemetry->latitude + QString::fromUtf8("+") + telemetry->longitude;

    // copy to clipboard
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(url);
}


void MainWindow::copyOpenStreetMap()
{
    // OpenStreetMap
    QString lat = telemetry->latitude;
    QString lon = telemetry->longitude;
    if (lat.contains("S"))
        lat.prepend(("-"));
    if (lon.contains("W"))
        lon.prepend("-");
    QString url = QString::fromUtf8("http://www.openstreetmap.org/?mlat=") + lat +
            QString::fromUtf8("&mlon=") + lon + QString::fromUtf8("&zoom=14");


    // copy to clipboard
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(url);
}

void MainWindow::openGoogleMaps()
{
    QString url =  QString::fromUtf8("http://maps.google.com/maps?z=14&t=m&q=loc:") +
                    telemetry->latitude + QString::fromUtf8("+") + telemetry->longitude;

    // open
    QDesktopServices::openUrl(QUrl(url));
}


void MainWindow::openOpenStreetMap()
{
    // OpenStreetMap
    QString lat = telemetry->latitude;
    QString lon = telemetry->longitude;
    if (lat.contains("S"))
        lat.prepend(("-"));
    if (lon.contains("W"))
        lon.prepend("-");
    QString url = QString::fromUtf8("http://www.openstreetmap.org/?mlat=") + lat +
            QString::fromUtf8("&mlon=") + lon + QString::fromUtf8("&zoom=14");


    // open
    QDesktopServices::openUrl(QUrl(url));
}

void MainWindow::on_labelLon_customContextMenuRequested(const QPoint &pos)
{
    QMenu *menu = new QMenu;
    menu->addAction(tr("Open OpenStreetMap"), this, SLOT(openOpenStreetMap()));
    menu->addAction(tr("Copy OpenStreetMap"), this, SLOT(copyOpenStreetMap()));
    menu->addSeparator();
    menu->addAction(tr("Open Google Maps"), this, SLOT(openGoogleMaps()));
    menu->addAction(tr("Copy Google Maps"), this, SLOT(copyGoogleMaps()));
    menu->exec(this->mapToGlobal(pos));
}

void MainWindow::on_labelLat_linkActivated(const QString &link)
{
    openOpenStreetMap();
    qDebug(link.toLocal8Bit().constData());
}

void MainWindow::on_labelLon_linkActivated(const QString &link)
{
    openOpenStreetMap();
    qDebug(link.toLocal8Bit().constData());
}

void MainWindow::on_actionMax_Min_triggered()
{
    maxMinDialog->show();
}
