#ifndef TELEMETRYFRAME_H
#define TELEMETRYFRAME_H

#include <QObject>
#include <QByteArray>
#include <QString>

// Structure pour représenter la trame décodée
struct TmFrame_t {
    QByteArray frame;  // Trame brute
    uint8_t sts;       // Status de la fusée
    int32_t lat;       // Latitude GNSS (en microdegrés)
    int32_t lon;       // Longitude GNSS (en microdegrés)
    int16_t alt;       // Altitude GNSS (en mètres)
    int32_t pressure;  // Pression ambiante (en pascals)
    int16_t temp;      // Température ambiante (en dixièmes de degré Celsius)
    int16_t accX;      // Accélération selon l'axe X (en milli-g)
    int16_t accY;      // Accélération selon l'axe Y (en milli-g)
    int16_t accZ;      // Accélération selon l'axe Z (en milli-g)
    int16_t annex0;    // Valeur ADC0
    int16_t annex1;    // Valeur ADC1
    uint8_t crcReceived;  // CRC reçu avec la trame
    uint8_t crcCalculated;  // CRC calculé à partir des données de la trame
    uint8_t crcCheck;  // Informe si le CRC est calculé correpond à celui recu
    uint8_t id;
    uint8_t gnssStatus;
    uint8_t flightStatus;
};

class TelemetryFrame : public QObject
{
    Q_OBJECT
public:
    explicit TelemetryFrame(QObject *parent = nullptr);
    ~TelemetryFrame();

public slots:
    void processData(bool receptionChek, const QByteArray &data); // Méthode pour traiter et décoder les données reçues

signals:
    void frameDecoded(const TmFrame_t &frame, const QString &decodedString); // Signal émis quand une trame est décodée

private:
    TmFrame_t decodeFrame(const QByteArray &data); // Méthode pour décoder les données reçues dans une structure TmFrame_t
    QString toString(const TmFrame_t &frame); // Méthode pour convertir la structure de trame en une chaîne lisible

};

#endif // TELEMETRYFRAME_H
