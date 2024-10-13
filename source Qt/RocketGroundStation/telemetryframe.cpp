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

    return frame;
}

QString TelemetryFrame::toString(const TmFrame_t &frame)
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
