#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QSettings>
#include <QDateTime>
#include <QTimer>
#include "about.h"
#include "config.h"

#define FIELD_LAT = 0;
#define FIELD_LON = 1;
#define FIELD_SPD = 2;
#define FIELD_ALT = 3;
#define FIELD_VLT = 4;
#define FIELD_BAR = 5;
#define FIELD_TIN = 6;
#define FIELD_TOU = 7;
#define FIELD_DAT = 8;
#define FIELD_TIM = 9;
#define FIELD_GPS = 10;
#define FIELD_MSG = 11;

struct telem_data {
   float latitude;
   float longitude;
   float speed;
   float altitude;
   float voltage;
   float baro;
   float temp_int;
   float temp_ext;
   char date[10];
   char time[9];
   char gps[50];
   char msg[128];
};

typedef struct telem_data telem_data_t;



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

private:
    Ui::MainWindow *ui;
    qint16 port;
    QString ip;
    QTcpSocket *awgSocket;
    QDataStream *awgData;
    telem_data_t data;
    QSettings *config;
    QDateTime *lastPacket;
    QTimer *timer;

private slots:
    void readAwgData();
    void updatePacketTime();
    void on_actionAcerca_de_triggered();
    void on_actionSalir_triggered();
    void on_actionConfigurar_triggered();
};

#endif // MAINWINDOW_H
