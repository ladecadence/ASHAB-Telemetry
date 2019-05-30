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

    if (config->contains("lora/port"))
    {
        // try to open serial port
        loraSerialPort = new QSerialPort();
        loraSerialPort->setPortName(config->value("lora/port").toString());
        loraSerialPort->setBaudRate(115200);
        if (!loraSerialPort->open(QIODevice::ReadOnly))
        {
            loraSerialPortValid = false;
        }
        else
        {
            loraSerialPortValid = true;
            // flush data
            loraSerialPort->clear();
            connect(loraSerialPort, SIGNAL(readyRead()), SLOT(readLoRaSerialData()));
        }
    }

    if (config->contains("tracker/url"))
    {
        url = config->value("tracker/url").toInt();
    } else {
        url = "http://ashab.space/tracker/upload-couchdb.php";
        config->setValue("tracker/url", url);
    }


    // start app
    // packet engine
    lastAwgPacket = new QDateTime(QDateTime::currentDateTimeUtc());
    lastLoRaPacket = new QDateTime(QDateTime::currentDateTimeUtc());
    timer = new QTimer();
    connect(timer, SIGNAL(timeout()), SLOT(updatePacketTime()));
    timer->start(1000);
    connectTcp(ip, port);


    // Create UI
    consoleDialog = new ConsoleDialog(this);
    aboutDialog = new About(this);
    configDialog = new Config(this);
    logDialog = new LogDialog(this);
    maxMinDialog = new MaxMinDialog(this);
    ssdvDialog = new SSDVDialog(this, consoleDialog);
    chartsDialog = new ChartsDialog(this, logDialog);


    ui->setupUi(this);

    ui->graphicsView->setStyleSheet("background: transparent;");
    QPixmap pixmap(":/Images/images/ashab-small.png");
    QGraphicsScene *scn = new QGraphicsScene(); // object defined in header
    ui->graphicsView->setScene(scn);
    scn->addPixmap(pixmap);
    ui->graphicsView->fitInView(scn->itemsBoundingRect(),Qt::KeepAspectRatio);

    // add keyboard shortcuts
    exitAction = new QAction(this);
    exitAction->setShortcut(Qt::Key_Q | Qt::CTRL);
    connect(exitAction, SIGNAL(triggered()), this, SLOT(on_actionSalir_triggered()));
    this->addAction(exitAction);

    // Telemetry
    telemetry = new Telemetry();

    // LoRa Serial buffer
    serialBuffer = new QByteArray();

}

MainWindow::~MainWindow()
{
    // kill dialogs
    consoleDialog->close();
    aboutDialog->close();
    configDialog->close();
    logDialog->close();
    maxMinDialog->close();
    ssdvDialog->close();

    delete consoleDialog;
    delete aboutDialog;
    delete configDialog;
    delete logDialog;
    delete maxMinDialog;
    delete ssdvDialog;

    // clear memory
    delete ui;
    delete telemetry;
    delete timer;
    delete lastAwgPacket;
    delete lastLoRaPacket;
    delete config;
    delete awgSocket;
    delete serialBuffer;
    delete loraSerialPort;

    qApp->exit();
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
    while (awgSocket->bytesAvailable() > 0) {
        char c;
        awgSocket->getChar(&c);
        if (c!=0)
            inData.append(c);
    }
    fprintf(stderr, ">>> %s\n", inData.constData());
    this->readTelemetry(QString::fromLocal8Bit(inData.constData()), Awg);
}

bool MainWindow::readTelemetry(QString data, int source)
{
    // parse data
    bool parsed = telemetry->parseData(data);
    // and update UI
    if (parsed) {
        if (telemetry->sats.toInt()>3) {
            QString latitude = QString::fromUtf8("Latitud: ") +
                    QString::fromUtf8("<a href=\"http://maps.google.com/maps?z=12&t=m&q=loc:") +
                    telemetry->latitude + QString::fromUtf8("+") + telemetry->longitude +
                    QString::fromUtf8("\" style=\"color: #33ccff;\">") +
                    telemetry->latitude + QString::fromUtf8("</a>");
            QString longitude = QString::fromUtf8("Longitud: ") +
                    QString::fromUtf8("<a href=\"http://maps.google.com/maps?z=12&t=m&q=loc:") +
                    telemetry->latitude + QString::fromUtf8("+") + telemetry->longitude +
                    QString::fromUtf8("\" style=\"color: #33ccff;\">") +
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
        ui->labelAsr->setText("Ratio Asc.: " + telemetry->a_rate + " m/s");


        // update max and min
        if (!maxMinDialog->isInit())
		{
            maxMinDialog->initData(telemetry);
		}
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

        // console
        // consoleDialog->append(telemetry->toString());

        // and upload (if it's valid)
        if (telemetry->sats.toInt() >= 0)
            uploadTelemetry();

        if (source == Awg)
        {
            delete lastAwgPacket;
            lastAwgPacket = new QDateTime(QDateTime::currentDateTimeUtc());
        }
        else if (source == LoRa)
        {
            delete lastLoRaPacket;
            lastLoRaPacket = new QDateTime(QDateTime::currentDateTimeUtc());
        }

        return true;
    }
    else
        return false;

}

void MainWindow::readLoRaSerialData()
{
    // debug
    // consoleDialog->append("Serial data available");

    // try to sync to telemetry packet
    //QByteArray data = loraSerialPort->peek(255);
    //if (data.contains("$$")) {
    //    int pos = data.indexOf("$$", 0);
    //    // read up to telemetry marker
    //    loraSerialPort->read(pos);
    //}

    // try to receive 255 bytes
	// if we have data, store it up to 255 bytes
	if (loraSerialPort->bytesAvailable() > 0)
	{
		// check how much we need to read
		int dif = 255 - serialBuffer->length();
		if (dif > 0) {
			serialBuffer->append(
					loraSerialPort->read(
						loraSerialPort->bytesAvailable()>dif?dif:loraSerialPort->bytesAvailable()
						)
					);
		}
	}

	// if data is not 255 bytes, wait for more
	if (serialBuffer->length() < 255)
	{
		return;
	}

    // qDebug() << serialBuffer->constData() << "\n------\n";

    // check what we have

    // debug packet
    if (serialBuffer->at(0) == '#' &&
            serialBuffer->at(1) == '#')
    {
        serialBuffer->clear();
        return;
    } 

	// telemetry packet
	else if ((serialBuffer->at(0) =='$') &&
         (serialBuffer->at(1) == '$') &&
         (serialBuffer->at(serialBuffer->length()-1) == 0))
    {
        // qDebug() << "Telem detected";
        // telemetry packet (ends in EOL)
        if (serialBuffer->contains('\n'))
        {
            // get EOL position
            int eol_pos = serialBuffer->indexOf('\n');
            // get data
            QByteArray data = serialBuffer->mid(0, eol_pos);
            // and delete that data from the buffer
            serialBuffer->clear();

            // ok, parse telemetry
            consoleDialog->append("Telemetry Packet!");
            consoleDialog->append(QString::fromLocal8Bit(data.constData()));

            // parse telemetry
            this->readTelemetry(data.constData(), LoRa);

            return;
        }
    }
    // no telemetry, SSDV packet?
	else if ((serialBuffer->at(0) == 0x66) && (serialBuffer->length() == 255))
    {
        // ok, SSDV packet
        int image_num = (int)serialBuffer->at(SSDV_HEADER_IMAGE);
        int image_packet = (serialBuffer->at(SSDV_HEADER_PACKET_MSB) << 8) + serialBuffer->at(SSDV_HEADER_PACKET_LSB);
        int is_last_packet = (serialBuffer->at(SSDV_HEADER_FLAGS) & 0b00000100) >> 2;

        //qDebug() << "SSDV Packet!";
        //qDebug() << "Image: " << image_num;
        //qDebug() << "Packet: " << image_packet;
        //qDebug() << "Flags: " << QString::number(serialData.at(SSDV_HEADER_FLAGS), 2);

        if (is_last_packet)
            qDebug() << "Last SSDV packet";

        QString status = QString("");
        status.append("Recibido paquete ").append(QString::number(image_packet)).
                append(" de imagen ").
                append(QString::number(image_num));

        // update status msg
        ssdvDialog->updateStatus(status);

        // output file
        bool file_opened = false;
        QString path = QString("");
        if (config->contains("lora/imgpath"))
        {
            path.append(config->value("lora/imgpath").toString());
        }

        QString fileName;
        fileName.append(path).append(QDir::separator()).
                append(QString("ssdv").
                       append(QString::number(image_num)).
                       append(".bin"));


        QFile ssdv_file(fileName);

        //qDebug() << QFileInfo(ssdv_file).fileName();
        // first packet? open or append
        if (image_packet == 0)
        {
            if (ssdv_file.open(QFile::WriteOnly | QFile::Truncate))
                file_opened = true;
        }
        else
        {
            if (ssdv_file.open(QFile::Append))
                file_opened = true;
        }
        if (file_opened)
        {
            ssdv_file.seek(image_packet*256);
            ssdv_file.putChar(0x55);
            ssdv_file.write(serialBuffer->constData(), 255);
            ssdv_file.flush();
            ssdv_file.close();
        }

        // add to img recv list
        ssdvDialog->addImageSSDV(fileName);

        // if last packet, decode and open it
        //if (is_last_packet)
        //{
        //    ssdvDialog->decodeSSDV(fileName);
        //}
        if (image_packet == 0)
        {
            ssdvDialog->decodeSSDV(fileName);
            ssdvDialog->showImage(fileName.append(".jpg"));
        }
        else
        {
            ssdvDialog->decodeSSDV(fileName);
            ssdvDialog->updateImage(fileName.append(".jpg"));
        }

        // ok, next packet
        serialBuffer->clear();

    }
    else
    {
        // try to sync to LoRa or telemetry packet
		if (serialBuffer->contains("$$"))
		{
			int index = serialBuffer->indexOf("$$");
            serialBuffer->remove(0, index-1);
		}
    }

    // update packet time
    delete lastLoRaPacket;
    lastLoRaPacket = new QDateTime(QDateTime::currentDateTimeUtc());

}

void MainWindow::updatePacketTime()
{
    QDateTime *now = new QDateTime(QDateTime::currentDateTimeUtc());

    // AWG
    int awgSeconds = lastAwgPacket->secsTo(*now);
    QString message = QString::fromUtf8("Último paquete AWG hace: ");
    if (awgSeconds > 59) {
        int minutes = awgSeconds / 60;
        awgSeconds = awgSeconds % 60;
        message.append(QString::number(minutes));
        message.append(" m, ");
        message.append(QString::number(awgSeconds));
        message.append(" s.");
    }
    else
    {
        message.append(QString::number(awgSeconds));
        message.append(" s.");
    }

    // LoRa
    if (loraSerialPortValid) {
        message.append(" / Último paquete LoRa hace: ");
        int loraSeconds = lastLoRaPacket->secsTo(*now);

        if (loraSeconds > 59) {
            int loraMinutes = loraSeconds / 60;
            loraSeconds = loraSeconds % 60;
            message.append(QString::number(loraMinutes));
            message.append(" m, ");
            message.append(QString::number(loraSeconds));
            message.append(" s.");
        }
        else
        {
            message.append(QString::number(loraSeconds));
            message.append(" s.");
        }
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
        postData.addQueryItem("image", "");
        postData.addQueryItem("database", config->value("tracker/database").toString());
        //fprintf(stderr, "%s\n", telemetry->toString().toLocal8Bit().constData());

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

        // work done, clean
        // delete networkManager;

    }


}

void MainWindow::onPostAnswer(QNetworkReply* reply)
{
    QString replyText = QString::fromUtf8(reply->readAll().constData());
    fprintf(stderr, "\n----->>>> %s", replyText.toLocal8Bit().constData());
    if (replyText.contains("You can pass") && replyText.contains("inserted")) {
        fprintf(stderr, "+++ Uploaded!");
        consoleDialog->append("Telemetry uploaded to the server!\n");
    }
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

uint32_t MainWindow::encodeCallsign(char* callsign) 
{
	uint32_t x;
	char *c;
	
	/* Point c at the end of the callsign, maximum of 6 characters */
	for(x = 0, c = callsign; x < SSDV_MAX_CALLSIGN && *c; x++, c++);
	
	/* Encode it backwards */
	x = 0;
	for(c--; c >= callsign; c--)
	{
		x *= 40;
		if(*c >= 'A' && *c <= 'Z') x += *c - 'A' + 14;
		else if(*c >= 'a' && *c <= 'z') x += *c - 'a' + 14;
		else if(*c >= '0' && *c <= '9') x += *c - '0' + 1;
	}
	
	return(x);
}

void MainWindow::on_labelLat_linkActivated(const QString &link)
{
	qDebug() << link;
    openOpenStreetMap();
}

void MainWindow::on_labelLon_linkActivated(const QString &link)
{
	qDebug() << link;
    openOpenStreetMap();
}

void MainWindow::on_actionMax_Min_triggered()
{
    maxMinDialog->show();
}

void MainWindow::on_actionSSDV_triggered()
{
    ssdvDialog->show();
}

void MainWindow::on_actionConsole_triggered()
{
    consoleDialog->show();
}

void MainWindow::on_actionGr_ficos_triggered()
{
    chartsDialog->show();
}
