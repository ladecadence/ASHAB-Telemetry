#ifndef TELEMETRY_H
#define TELEMETRY_H

#include <QString>
#include <QStringList>
#include <QDebug>

#define TELEMETRY_FIELDS 13

#define FIELD_LAT  0
#define FIELD_LON  1
#define FIELD_SPD  2
#define FIELD_ALT  3
#define FIELD_VLT  4
#define FIELD_BAR  5
#define FIELD_TIN  6
#define FIELD_TOU  7
#define FIELD_DAT  8
#define FIELD_TIM  9
#define FIELD_GPS  10
#define FIELD_SAT  11
#define FIELD_ASR  12
#define FIELD_MSG  13

#define SEPARATOR "/"

class Telemetry
{
public:
    QString latitude;
    QString longitude;
    QString heading;
    QString speed;
    QString altitude;
    QString voltage;
    QString baro;
    QString temp_int;
    QString temp_ext;
    QString date;
    QString time;
    QString gps;
    QString sats;
    QString a_rate;
    QString msg;

    Telemetry();
    bool parseData(QString data);
    QString toString();
};

#endif // TELEMETRY_H
