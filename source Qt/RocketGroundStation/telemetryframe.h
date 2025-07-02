#ifndef TELEMETRYFRAME_H
#define TELEMETRYFRAME_H

#include <QObject>
#include <QByteArray>
#include <QString>

constexpr float acc_factor = 16.0 / 32768.0;
constexpr float gyro_factor = 2000.0/32768.0;

// Structure pour représenter la trame décodée
struct TmFrame_t {
    QByteArray frame;     // Trame brute

    // Données brutes
    uint8_t  sts;
    int32_t  lat, lon;
    int16_t  altGNSS;
    int32_t  pressure;
    int16_t  temp;
    int16_t  accX, accY, accZ;
    int16_t  gyroX, gyroY, gyroZ;
    int16_t  annex0, annex1;

    // Métadonnées
    uint8_t  crcReceived;
    uint8_t  crcCalculated;
    bool     crcCheck;
    uint8_t  id;
    uint8_t  gnssFix;
    uint8_t  gnssFixType;
    uint8_t  flightStatus;

    // Champs calculés
    float latFloat, lonFloat;
    float pressureFloat, tempFloat;
    float accXFloat, accYFloat, accZFloat;
    float gyroXFloat, gyroYFloat, gyroZFloat;
    float annex0Float, annex1Float;
    float altitudeBaroFloat;

    // Constructeur par défaut
    TmFrame_t()
        : frame(),

        // Init brutes
        sts(0), lat(0), lon(0), altGNSS(0),
        pressure(0), temp(0),
        accX(0), accY(0), accZ(0),
        gyroX(0), gyroY(0), gyroZ(0),
        annex0(0), annex1(0),

        // Init méta
        crcReceived(0), crcCalculated(0), crcCheck(false),
        id(0), gnssFix(0), gnssFixType(0), flightStatus(0),

        // Init flottants
        latFloat(0.0f), lonFloat(0.0f),
        pressureFloat(0.0f), tempFloat(0.0f),
        accXFloat(0.0f), accYFloat(0.0f), accZFloat(0.0f),
        gyroXFloat(0.0f), gyroYFloat(0.0f), gyroZFloat(0.0f),
        annex0Float(0.0f), annex1Float(0.0f),
        altitudeBaroFloat(0.0f)
    {}
};

enum GnssFixType : uint8_t {
    GNSS_NO_FIX       = 0b00,
    GNSS_OTHER_FIX    = 0b01,
    GNSS_2D_FIX       = 0b10,
    GNSS_3D_FIX       = 0b11
};

typedef enum {PRE_FLIGHT = 0, PYRO_RDY, ASCEND, DEPLOY_ALGO, DEPLOY_TIMER, DESCEND, TOUCHDOWN, LOST} RocketState_t;

class TelemetryFrame : public QObject
{
    Q_OBJECT
public:
    explicit TelemetryFrame(QObject *parent = nullptr);
    ~TelemetryFrame();
    float calculateAltitude(float pressure);

public slots:
    void processData(bool receptionChek, const QByteArray &data); // Méthode pour traiter et décoder les données reçues

signals:
    void frameDecoded(const TmFrame_t &frame, const QString &decodedString); // Signal émis quand une trame est décodée

private:
    TmFrame_t decodeFrame(const QByteArray &data); // Méthode pour décoder les données reçues dans une structure TmFrame_t
    QString toString(const TmFrame_t &frame); // Méthode pour convertir la structure de trame en une chaîne lisible

};

#endif // TELEMETRYFRAME_H
