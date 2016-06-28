#include "telemetry.h"

Telemetry::Telemetry()
{
    latitude = "00.0000N";
    longitude = "000.0000W";
    heading = "0";
    speed = "0";
    altitude = "0";
    voltage = "0";
    baro = "0";
    temp_int = "0";
    temp_ext = "0";
    date = "01-01-1979";
    time = "00:00:00";
    sats = "";
    a_rate = "";
    gps = "";
    msg = "";
}

bool Telemetry::parseData(QString data)
{
    QStringList fields;

    fields = data.split(SEPARATOR);
    if (fields.length()>TELEMETRY_FIELDS) {
        try {
            //fields[FIELD_LAT] = fields[FIELD_LAT].split("!")[1];
            //fields[FIELD_LON] =  fields[FIELD_LON].split("O")[0];
            date = fields[FIELD_DAT];
            time = fields[FIELD_TIM];
            heading = fields[FIELD_LON].split("O")[1];
            speed = fields[FIELD_SPD];
            latitude = fields[FIELD_GPS].split("=")[1].split(",")[0];
            longitude = fields[FIELD_GPS].split("=")[1].split(",")[1];
            altitude = fields[FIELD_ALT].split("=")[1];
            voltage = fields[FIELD_VLT].split("=")[1];
            temp_int = fields[FIELD_TIN].split("=")[1];
            temp_ext = fields[FIELD_TOU].split("=")[1];
            baro = fields[FIELD_BAR].split("=")[1];
            sats = fields[FIELD_SAT].split("=")[1];
            a_rate = fields[FIELD_ASR].split("=")[1];
        }
        catch (std::exception &e)
        {
            return false;
        }

    }
    else
    {
        return false;
    }

    return true;
}

QString Telemetry::toString()
{
    QString data = "";
    data.append(date);
    data.append(";");
    data.append(time);
    data.append(";");
    data.append(latitude);
    data.append(";");
    data.append(longitude);
    data.append(";");
    data.append(altitude);
    data.append(";");
    data.append(voltage);
    data.append(";");
    data.append(temp_int);
    data.append(";");
    data.append(temp_ext);
    data.append(";");
    data.append(baro);
    data.append(";");
    data.append(heading);
    data.append(";");
    data.append(speed);
    data.append(";");
    data.append(sats);
    data.append(";");
    data.append(a_rate);

    return data;
}
