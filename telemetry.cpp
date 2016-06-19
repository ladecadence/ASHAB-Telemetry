#include "telemetry.h"

Telemetry::Telemetry()
{
    latitude = 0;
    longitude = 0;
    speed = 0;
    altitude = 0;
    voltage = 0;
    baro = 0;
    temp_int = 0;
    temp_ext = 0;
    date = "";
    time = "";
    gps = "";
    msg = "";
}

QString Telemetry::toString()
{
    QString data = "TEST";

    return data;
}
