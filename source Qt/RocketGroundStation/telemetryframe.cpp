#include "telemetryframe.h"
#include <QDebug>

TelemetryFrame::TelemetryFrame(QObject *parent)
    : QObject{parent}
{}

TelemetryFrame::~TelemetryFrame()
{}

void TelemetryFrame::processData(bool receptionCheck, const QByteArray &data)
{
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
    frame.accXFloat = frame.accX * acc_factor;
    frame.accYFloat = frame.accY * acc_factor;
    frame.accZFloat = frame.accZ * acc_factor;
    frame.annex0Float = frame.annex0 * 1.0;
    frame.annex1Float = frame.annex1 * 1.0;

    return frame;
}

QString TelemetryFrame::toString(const TmFrame_t &frame) {
    return QString("<style>"
                   "td { padding: 4px 8px; } "
                   ".header { font-weight: bold; color: #4CAF50; } "
                   ".value { color: #022d9c; } "
                   ".critical { color: #FF5722; font-weight: bold; } "
                   ".pre_flight { color: #1E90FF; font-weight: bold; } "  // Bleu clair pour PRE_FLIGHT
                   ".pyro_rdy { color: #FF0000; font-weight: bold; } "    // Rouge pour PYRO_RDY
                   ".ascend { color: #32CD32; font-weight: bold; } "      // Vert pour ASCEND
                   ".deploy_algo { color: #FFA500; font-weight: bold; } " // Orange pour DEPLOY_ALGO
                   ".deploy_timer { color: #800080; font-weight: bold; } "// Violet pour DEPLOY_TIMER
                   ".descend { color: #8B4513; font-weight: bold; } "     // Marron pour DESCEND
                   ".touchdown { color: #808080; font-weight: bold; } "   // Gris pour TOUCHDOWN
                   "</style> "
                   "<b>[Decoded data]</b> "
                   "<span class='header'>sts:</span> <span class='%1'>%2</span> | "
                   "<span class='header'>lat:</span> <span class='%3'>%4 °</span> | "
                   "<span class='header'>lon:</span> <span class='%5'>%6 °</span> | "
                   "<span class='header'>alt:</span> <span class='%7'>%8 m</span> | "
                   "<span class='header'>pressure:</span> <span class='%9'>%10 mBar</span> | "
                   "<span class='header'>temp:</span> <span class='%11'>%12 °C</span> | "
                   "<span class='header'>annex0:</span> <span class='%13'>%14 V</span> | "
                   "<span class='header'>annex1:</span> <span class='%15'>%16 V</span> | "
                   "<span class='header'>accX:</span> <span class='%17'>%18 g</span> | "
                   "<span class='header'>accY:</span> <span class='%19'>%20 g</span> | "
                   "<span class='header'>accZ:</span> <span class='%21'>%22 g</span> | "
                   "<span class='header'>id:</span> <span class='%23'>%24</span> | "
                   "<span class='header'>gnssStatus:</span> <span class='%25'>%26</span> | "
                   "<span class='header'>flightStatus:</span> <span class='%27'>%28</span>")

        .arg(frame.sts == 0 ? "critical" : "value",
                 QString::number(frame.sts),
                 frame.latFloat < -90 || frame.latFloat > 90 ? "critical" : "value",
                 QString::number(frame.latFloat, 'f', 7),
                 frame.lonFloat < -180 || frame.lonFloat > 180 ? "critical" : "value",
                 QString::number(frame.lonFloat, 'f', 7),
                 frame.alt < 0 || frame.alt > 10000 ? "critical" : "value",
                 QString::number(frame.alt),
                 frame.pressureFloat < 0.1 || frame.pressureFloat > 1200 ? "critical" : "value",
                 QString::number(frame.pressureFloat, 'f', 2),
                 frame.tempFloat > 50 ? "critical" : "value",
                 QString::number(frame.tempFloat, 'f', 2),
                 frame.annex0Float < 0 || frame.annex0Float > 5 ? "critical" : "value",
                 QString::number(frame.annex0Float, 'f', 2),
                 frame.annex1Float < 0 || frame.annex1Float > 5 ? "critical" : "value",
                 QString::number(frame.annex1Float, 'f', 2),
                 frame.accXFloat < -16 || frame.accXFloat > 16 ? "critical" : "value",
                 QString::number(frame.accXFloat, 'f', 5),
                 frame.accYFloat < -16 || frame.accYFloat > 16 ? "critical" : "value",
                 QString::number(frame.accYFloat, 'f', 5),
                 frame.accZFloat < -16 || frame.accZFloat > 16 ? "critical" : "value",
                 QString::number(frame.accZFloat, 'f', 5),
                 frame.id == 0 ? "critical" : "value",
                 QString::number(frame.id),
                 frame.gnssStatus == 0 ? "critical" : "value",
                 QString::number(frame.gnssStatus),
                 frame.flightStatus == PRE_FLIGHT ? "pre_flight" :
                     frame.flightStatus == PYRO_RDY ? "pyro_rdy" :
                     frame.flightStatus == ASCEND ? "ascend" :
                     frame.flightStatus == DEPLOY_ALGO ? "deploy_algo" :
                     frame.flightStatus == DEPLOY_TIMER ? "deploy_timer" :
                     frame.flightStatus == DESCEND ? "descend" :
                     "touchdown",
                 QString::number(frame.flightStatus));
}


