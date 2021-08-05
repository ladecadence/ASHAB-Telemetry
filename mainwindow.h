#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QSettings>
#include <QDateTime>
#include <QTimer>
#include <QUrlQuery>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QDesktopServices>
#include <QSerialPort>
#include <QShortcut>
#include "about.h"
#include "config.h"
#include "telemetry.h"
#include "logdialog.h"
#include "maxmindialog.h"
#include "ssdvdialog.h"
#include "lorassdv.h"
#include "consoledialog.h"
#include "chartsdialog.h"


enum Source {
    Awg, LoRa
};

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void connectTcp(QString host, quint16 port);
    About *aboutDialog;
    Config *configDialog;
    LogDialog *logDialog;
    MaxMinDialog *maxMinDialog;
    SSDVDialog *ssdvDialog;
    ConsoleDialog *consoleDialog;
    ChartsDialog *chartsDialog;

private:
    Ui::MainWindow *ui;
    qint16 port;
    QString ip;
    Telemetry *telemetry;
    QSettings *config;
    QDateTime *lastLoRaPacket;
    QTimer *timer;
    QAction *exitAction;
    bool loraSerialPortValid;
    QSerialPort *loraSerialPort;
    QByteArray *serialBuffer;
    QNetworkAccessManager *networkManager;
	
	uint32_t encodeCallsign(char* callsign);

private slots:
    void updateSerialPorts();
    void readLoRaSerialData();
    bool readTelemetry(QString data, int source);
    void updatePacketTime();
    void on_actionAcerca_de_triggered();
    void on_actionSalir_triggered();
    void on_actionConfigurar_triggered();
    void uploadTelemetry();
    void onPostAnswer(QNetworkReply* reply);
    void on_actionLog_triggered();
    void on_labelLat_customContextMenuRequested(const QPoint &pos);
    void openGoogleMaps();
    void openOpenStreetMap();
    void copyGoogleMaps();
    void copyOpenStreetMap();
    void on_labelLon_customContextMenuRequested(const QPoint &pos);
    void on_labelLat_linkActivated(const QString &link);
    void on_labelLon_linkActivated(const QString &link);
    void on_actionMax_Min_triggered();
    void on_actionSSDV_triggered();
    void on_actionConsole_triggered();
    void on_actionGr_ficos_triggered();
};

#endif // MAINWINDOW_H
