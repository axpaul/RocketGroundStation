#include "telemetryframe.h"
#include <QDebug>

TelemetryFrame::TelemetryFrame(QObject *parent)
    : QObject{parent}{

}

TelemetryFrame::~TelemetryFrame(){

}

void TelemetryFrame::processData(bool receptionCheck, const QByteArray &data){
    TmFrame_t frame;
    qDebug() << "[TelemetryFrame] CRC check (initial):" << receptionCheck;

    if (receptionCheck){
        frame = decodeFrame(data);
        QString decodedString = toString(frame);
        frame.crcCheck = true;
        qDebug() << "[TelemetryFrame] CRC check (after decode, valid):" << frame.crcCheck;
        emit frameDecoded(frame, decodedString);
    }
    else {
        QString decodedString = "[Error] Invalid CRC: unable to decrypt data";
        qDebug() << "[TelemetryFrame] CRC check (invalid):" << frame.crcCheck;
        emit frameDecoded(frame, decodedString);
    }
}

TmFrame_t TelemetryFrame::decodeFrame(const QByteArray &data){
    TmFrame_t frame = {};

    frame.frame = data;

    // Extraction des champs de la trame
    frame.pressure = (data[3] & 0xFF) << 24 |
                     (data[2] & 0xFF) << 16 |
                     (data[1] & 0xFF) << 8 |
                     (data[0] & 0xFF);

    frame.lat = (data[7] & 0xFF) << 24 |
                (data[6] & 0xFF) << 16 |
                (data[5] & 0xFF) << 8 |
                (data[4] & 0xFF);

    frame.lon = (data[11] & 0xFF) << 24 |
                (data[10] & 0xFF) << 16 |
                (data[9] & 0xFF) << 8 |
                (data[8] & 0xFF);

    frame.altGNSS = (data[13] & 0xFF) << 8 |
                (data[12] & 0xFF);

    frame.temp = (data[15] & 0xFF) << 8 |
                 (data[14] & 0xFF);

    frame.accX = (data[17] & 0xFF) << 8 |
                 (data[16] & 0xFF);

    frame.accY = (data[19] & 0xFF) << 8 |
                 (data[18] & 0xFF);

    frame.accZ = (data[21] & 0xFF) << 8 |
                 (data[20] & 0xFF);

    frame.gyroX = (data[23] & 0xFF) << 8 |
                 (data[22] & 0xFF);

    frame.gyroY = (data[25] & 0xFF) << 8 |
                 (data[24] & 0xFF);

    frame.gyroZ = (data[27] & 0xFF) << 8 |
                 (data[26] & 0xFF);

    frame.annex0 = (data[29] & 0xFF) << 8 |
                   (data[28] & 0xFF);

    frame.annex1 = (data[31] & 0xFF) << 8 |
                   (data[30] & 0xFF);

    // Extraction du statut (sts) de l'octet 26
    frame.sts = data[32];

    // Décodage le statut
    frame.id           = (frame.sts & 0b11000000) >> 6;  // Bits 7-6 : ID
    frame.gnssFix      = (frame.sts & 0b00100000) >> 5;  // Bit 5    : GNSS fix (0 = no fix, 1 = fix)
    frame.gnssFixType  = (frame.sts & 0b00011000) >> 3;  // Bits 4-3 : GNSS fix type (00=no fix, 01=other, 10=2D, 11=3D)
    frame.flightStatus =  frame.sts &  0b00000111;       // Bits 2-0 : Flight status (voir enum RocketState_t)


    // Initialisation des valeurs calculées en flottant
    frame.latFloat = frame.lat * 1e-7;
    frame.lonFloat = frame.lon * 1e-7;
    frame.pressureFloat = frame.pressure / 4096.0;
    frame.tempFloat = frame.temp / 100.0;
    frame.accXFloat = frame.accX * acc_factor;
    frame.accYFloat = frame.accY * acc_factor;
    frame.accZFloat = frame.accZ * acc_factor;
    frame.gyroXFloat = frame.gyroX * gyro_factor;
    frame.gyroYFloat = frame.gyroY * gyro_factor;
    frame.gyroZFloat = frame.gyroZ * gyro_factor;
    frame.annex0Float = (frame.annex0 - 2048.0) * 1.0;
    frame.annex1Float = (frame.annex1 - 2048.0) * 1.0;

    frame.altitudeBaroFloat = calculateAltitude(frame.pressureFloat);

    return frame;
}

QString TelemetryFrame::toString(const TmFrame_t &frame) {
    auto cls = [](bool ok) { return ok ? "value" : "critical"; };
    auto fmt = [](double val, int prec = 2) { return QString::number(val, 'f', prec); };

    QString gnssFixStr   = frame.gnssFix ? "Fix" : "No fix";
    QString gnssFixClass = cls(frame.gnssFix);

    QString gnssFixTypeStr = "No fix";
    QString gnssFixTypeClass = "critical";
    if (frame.gnssFixType == GNSS_2D_FIX)      { gnssFixTypeStr = "2D";   gnssFixTypeClass = "value"; }
    else if (frame.gnssFixType == GNSS_3D_FIX) { gnssFixTypeStr = "3D";   gnssFixTypeClass = "value"; }
    else if (frame.gnssFixType == GNSS_OTHER_FIX) { gnssFixTypeStr = "Other"; }

    QString flightStr, flightClass = "critical";
    switch (frame.flightStatus) {
    case PRE_FLIGHT:    flightStr = "PRE_FLIGHT";    flightClass = "pre_flight"; break;
    case PYRO_RDY:      flightStr = "PYRO_RDY";      flightClass = "pyro_rdy"; break;
    case ASCEND:        flightStr = "ASCEND";        flightClass = "ascend"; break;
    case DEPLOY_ALGO:   flightStr = "DEPLOY_ALGO";   flightClass = "deploy_algo"; break;
    case DEPLOY_TIMER:  flightStr = "DEPLOY_TIMER";  flightClass = "deploy_timer"; break;
    case DESCEND:       flightStr = "DESCEND";       flightClass = "descend"; break;
    case TOUCHDOWN:     flightStr = "TOUCHDOWN";     flightClass = "touchdown"; break;
    case LOST:          flightStr = "LOST";          break;
    default:            flightStr = "UNKNOWN";       break;
    }

    return QString(
               "<style>"
               ".value { color: #00BCD4; } "
               ".critical { color: #FF5722; font-weight: bold; } "
               ".header { font-weight: bold; color: #E0E0E0; } "
               ".pre_flight { color:#29B6F6; font-weight:bold; } "
               ".pyro_rdy { color:#E53935; font-weight:bold; } "
               ".ascend { color:#66BB6A; font-weight:bold; } "
               ".deploy_algo { color:#FFA726; font-weight:bold; } "
               ".deploy_timer { color:#AB47BC; font-weight:bold; } "
               ".descend { color:#8D6E63; font-weight:bold; } "
               ".touchdown { color:#BDBDBD; font-weight:bold; }"
               "</style>"
               "<b>[Telemetry]</b> "
               "State: <span class='%1'>%2</span> | "
               "Fix: <span class='%3'>%4</span> | "
               "FixType: <span class='%5'>%6</span> | "
               "STS: <span class='%7'>%8</span> | "
               "Lat: <span class='%9'>%10°</span> | "
               "Lon: <span class='%11'>%12°</span> | "
               "Alt: <span class='%13'>%14m</span> | "
               "Baro: <span class='%15'>%16m</span> | "
               "Press: <span class='%17'>%18mBar</span> | "
               "Temp: <span class='%19'>%20°C</span> | "
               "AccX: <span class='%21'>%22g</span> | "
               "AccY: <span class='%23'>%24g</span> | "
               "AccZ: <span class='%25'>%26g</span> | "
               "GyroX: <span class='%27'>%28°/s</span> | "
               "GyroY: <span class='%29'>%30°/s</span> | "
               "GyroZ: <span class='%31'>%32°/s</span>")
        .arg(
            flightClass, flightStr,
            gnssFixClass, gnssFixStr,
            gnssFixTypeClass, gnssFixTypeStr,
            cls(frame.sts), QString::number(frame.sts),
            cls(frame.latFloat >= -90 && frame.latFloat <= 90), fmt(frame.latFloat, 6),
            cls(frame.lonFloat >= -180 && frame.lonFloat <= 180), fmt(frame.lonFloat, 6),
            cls(frame.altGNSS >= 0 && frame.altGNSS < 20000), QString::number(frame.altGNSS),
            cls(frame.altitudeBaroFloat >= 0 && frame.altitudeBaroFloat < 20000), fmt(frame.altitudeBaroFloat),
            cls(frame.pressureFloat > 0.1 && frame.pressureFloat < 1200), fmt(frame.pressureFloat),
            cls(frame.tempFloat <= 50), fmt(frame.tempFloat),
            cls(frame.accXFloat >= -16 && frame.accXFloat <= 16), fmt(frame.accXFloat, 5),
            cls(frame.accYFloat >= -16 && frame.accYFloat <= 16), fmt(frame.accYFloat, 5),
            cls(frame.accZFloat >= -16 && frame.accZFloat <= 16), fmt(frame.accZFloat, 5),
            cls(frame.gyroXFloat >= -2000 && frame.gyroXFloat <= 2000), fmt(frame.gyroXFloat),
            cls(frame.gyroYFloat >= -2000 && frame.gyroYFloat <= 2000), fmt(frame.gyroYFloat),
            cls(frame.gyroZFloat >= -2000 && frame.gyroZFloat <= 2000), fmt(frame.gyroZFloat)
            );
}


float TelemetryFrame::calculateAltitude(float pressure) {
    // Constante : Pression au niveau de la mer (standard)
    const float basePressure = 1013.25; // en hPa

    // Vérifier que la pression est valide (non nulle ou négative)
    if (pressure <= 0) {
        qWarning() << "La pression mesurée est invalide.";
        return 0.0; // Altitude non calculable
    }

    // Constantes
    const float factor = 1.0 / 5.255; // Exposant de la formule simplifiée

    // Calcul de l'altitude
    float altitude = 44330.0 * (1.0 - pow(pressure / basePressure, factor));
    return altitude;
}
