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
#include "about.h"
#include "config.h"
#include "telemetry.h"
#include "logdialog.h"
#include "mapdialog.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void connectTcp(QString host, qint16 port);
    About *aboutDialog;
    Config *configDialog;
    LogDialog *logDialog;
    MapDialog *mapDialog;

private:
    Ui::MainWindow *ui;
    qint16 port;
    QString ip;
    QTcpSocket *awgSocket;
    QDataStream *awgData;
    Telemetry *telemetry;
    QSettings *config;
    QDateTime *lastPacket;
    QTimer *timer;

private slots:
    void readAwgData();
    void updatePacketTime();
    void on_actionAcerca_de_triggered();
    void on_actionSalir_triggered();
    void on_actionConfigurar_triggered();
    void uploadTelemetry();
    void onPostAnswer(QNetworkReply* reply);
    void on_actionLog_triggered();
    void on_actionMapa_triggered();
};

#endif // MAINWINDOW_H
