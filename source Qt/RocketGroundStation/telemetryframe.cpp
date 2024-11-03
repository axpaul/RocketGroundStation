#include "telemetryframe.h"
#include <QDebug>

TelemetryFrame::TelemetryFrame(QObject *parent)
    : QObject{parent}
{}

TelemetryFrame::~TelemetryFrame()
{}

void TelemetryFrame::processData(bool receptionChek, const QByteArray &data)
{
    if (receptionChek){
        TmFrame_t frame = decodeFrame(data);
        QString decodedString = toString(frame);
        emit frameDecoded(frame, decodedString);
    }
}

TmFrame_t TelemetryFrame::decodeFrame(const QByteArray &data)
{
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

    frame.alt = (data[13] & 0xFF) << 8 |
                (data[12] & 0xFF);

    frame.temp = (data[15] & 0xFF) << 8 |
                 (data[14] & 0xFF);

    frame.accX = (data[17] & 0xFF) << 8 |
                 (data[16] & 0xFF);

    frame.accY = (data[19] & 0xFF) << 8 |
                 (data[18] & 0xFF);

    frame.accZ = (data[21] & 0xFF) << 8 |
                 (data[20] & 0xFF);

    frame.annex0 = (data[23] & 0xFF) << 8 |
                   (data[22] & 0xFF);

    frame.annex1 = (data[25] & 0xFF) << 8 |
                   (data[24] & 0xFF);

    // Extraction du statut (sts) de l'octet 26
    frame.sts = data[26];

    // Décoder le statut
    frame.id = (frame.sts & 0b11000000) >> 6;           // Bits 7-6 pour ID
    frame.gnssStatus = (frame.sts & 0b00111000) >> 3;   // Bits 5-3 pour GNSS Status
    frame.flightStatus = (frame.sts & 0b00000111);      // Bits 2-0 pour Flight Status

    // Initialisation des valeurs calculées en flottant
    frame.latFloat = frame.lat * 1e-7;
    frame.lonFloat = frame.lon * 1e-7;
    frame.pressureFloat = frame.pressure / 4096.0;
    frame.tempFloat = frame.temp / 100.0;
    frame.accXFloat = frame.accX / 1000.0;
    frame.accYFloat = frame.accY / 1000.0;
    frame.accZFloat = frame.accZ / 1000.0;
    frame.annex0Float = frame.annex0 * 1.0;
    frame.annex1Float = frame.annex1 * 1.0;

    return frame;
}

/*QString TelemetryFrame::toString(const TmFrame_t &frame)
{
    return QString("[Decoded data] "
                   "sts: %1 | "
                   "lat: %2 | "
                   "lon: %3 | "
                   "alt: %4 | "
                   "pressure: %5 | "
                   "temp: %6 | "
                   "annex0: %7 | "
                   "annex1: %8 | "
                   "accX: %12 | "
                   "accY: %13 | "
                   "accZ: %14 | "
                   "id: %15 | "
                   "gnssStatus: %16 | "
                   "flightStatus: %17")
        .arg(frame.sts)
        .arg(frame.lat)
        .arg(frame.lon)
        .arg(frame.alt)
        .arg(frame.pressure)
        .arg(frame.temp)
        .arg(frame.annex0)
        .arg(frame.annex1)
        .arg(frame.accX)
        .arg(frame.accY)
        .arg(frame.accZ)
        .arg(frame.id )
        .arg(frame.gnssStatus)
        .arg(frame.flightStatus);
}
*/

QString TelemetryFrame::toString(const TmFrame_t &frame) {
    return QString("<style>"
                   "td { padding: 4px 8px; } "
                   ".header { font-weight: bold; color: #4CAF50; } "
                   ".value { color: #2196F3; } "
                   ".critical { color: #FF5722; font-weight: bold; } "
                   ".ground { color: #1E90FF; font-weight: bold; } "      // Bleu pour attente au sol
                   ".flight { color: #32CD32; font-weight: bold; } "     // Vert pour vol
                   ".parachute { color: #FFA500; font-weight: bold; } "  // Orange pour déploiement parachute
                   "</style> "
                   "<b>[Decoded data]</b> "
                   "<span class='header'>sts:</span> <span class='%1'>%2</span> | "
                   "<span class='header'>lat:</span> <span class='%3'>%4</span> | "
                   "<span class='header'>lon:</span> <span class='%5'>%6</span> | "
                   "<span class='header'>alt:</span> <span class='%7'>%8</span> | "
                   "<span class='header'>pressure:</span> <span class='%9'>%10</span> | "
                   "<span class='header'>temp:</span> <span class='%11'>%12</span> | "
                   "<span class='header'>annex0:</span> <span class='%13'>%14</span> | "
                   "<span class='header'>annex1:</span> <span class='%15'>%16</span> | "
                   "<span class='header'>accX:</span> <span class='%17'>%18</span> | "
                   "<span class='header'>accY:</span> <span class='%19'>%20</span> | "
                   "<span class='header'>accZ:</span> <span class='%21'>%22</span> | "
                   "<span class='header'>id:</span> <span class='%23'>%24</span> | "
                   "<span class='header'>gnssStatus:</span> <span class='%25'>%26</span> | "
                   "<span class='header'>flightStatus:</span> <span class='%27'>%28</span>")

        .arg(frame.sts == 0 ? "critical" : "value")
        .arg(frame.sts)
        .arg(frame.latFloat < -90 || frame.latFloat > 90 ? "critical" : "value")
        .arg(frame.latFloat)
        .arg(frame.lonFloat < -180 || frame.lonFloat > 180 ? "critical" : "value")
        .arg(frame.lonFloat)
        .arg(frame.alt < 0 || frame.alt > 10000 ? "critical" : "value")
        .arg(frame.alt)
        .arg(frame.pressureFloat < 0.1 || frame.pressureFloat > 1100 ? "critical" : "value")
        .arg(frame.pressureFloat)
        .arg(frame.tempFloat > 50 ? "critical" : "value")
        .arg(frame.tempFloat)
        .arg(frame.annex0Float < 0 || frame.annex0Float > 5 ? "critical" : "value")
        .arg(frame.annex0Float)
        .arg(frame.annex1Float < 0 || frame.annex1Float > 5 ? "critical" : "value")
        .arg(frame.annex1Float)
        .arg(frame.accXFloat < -16 || frame.accXFloat > 16 ? "critical" : "value")
        .arg(frame.accXFloat)
        .arg(frame.accYFloat < -16 || frame.accYFloat > 16 ? "critical" : "value")
        .arg(frame.accYFloat)
        .arg(frame.accZFloat < -16 || frame.accZFloat > 16 ? "critical" : "value")
        .arg(frame.accZFloat)
        .arg(frame.id == 0 ? "critical" : "value")
        .arg(frame.id)
        .arg(frame.gnssStatus == 0 ? "critical" : "value")
        .arg(frame.gnssStatus)
        // Classe en fonction du statut de vol
        .arg(frame.flightStatus == 0 ? "ground" : (frame.flightStatus == 1 ? "flight" : "parachute"))
        .arg(frame.flightStatus);
}

