#ifndef TELEMETRY_H
#define TELEMETRY_H

#include <QString>

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

class Telemetry
{
public:
    float latitude;
    float longitude;
    float speed;
    float altitude;
    float voltage;
    float baro;
    float temp_int;
    float temp_ext;
    QString date;
    QString time;
    QString gps;
    QString msg;

    Telemetry();
    QString toString();
};

#endif // TELEMETRY_H
