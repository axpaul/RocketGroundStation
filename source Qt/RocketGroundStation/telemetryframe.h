#ifndef TELEMETRYFRAME_H
#define TELEMETRYFRAME_H

#include <QObject>
#include <QByteArray>
#include <QString>

constexpr float acc_factor = 16.0 / 32768.0;

// Structure pour représenter la trame décodée
struct TmFrame_t {
    QByteArray frame;  // Trame brute
    uint8_t sts;       // Status de la fusée
    int32_t lat;       // Latitude GNSS (en microdegrés)
    int32_t lon;       // Longitude GNSS (en microdegrés)
    int16_t altGNSS;       // Altitude GNSS (en mètres)
    int32_t pressure;  // Pression ambiante (en pascals)
    int16_t temp;      // Température ambiante (en dixièmes de degré Celsius)
    int16_t accX;      // Accélération selon l'axe X (en milli-g)
    int16_t accY;      // Accélération selon l'axe Y (en milli-g)
    int16_t accZ;      // Accélération selon l'axe Z (en milli-g)
    int16_t annex0;    // Valeur ADC0
    int16_t annex1;    // Valeur ADC1
    uint8_t crcReceived;  // CRC reçu avec la trame
    uint8_t crcCalculated;  // CRC calculé à partir des données de la trame
    bool crcCheck;  // Informe si le CRC est calculé correpond à celui recu
    uint8_t id; // identification Software
    uint8_t gnssStatus; // GNSS reception
    uint8_t flightStatus; // Statut du vol

    // Champs calculés en flottant avec coefficients appliqués
    float latFloat;         // Latitude GNSS en degrés
    float lonFloat ;         // Longitude GNSS en degrés
    float pressureFloat; // Pression en mBar
    float tempFloat;      // Température en °C
    float accXFloat;     // Accélération X en g
    float accYFloat;     // Accélération Y en g
    float accZFloat;     // Accélération Z en g
    float annex0Float;    // Valeur ADC0 en V
    float annex1Float;    // Valeur ADC1 en V
    float altitudeBaroFloat; // Altitude calculée à partir de la pression mesurée

    // Constructeur par défaut pour initialiser toutes les valeurs à 0
    TmFrame_t()
        : frame(),
        sts(0), lat(0), lon(0), altGNSS(0), pressure(0), temp(0),
        accX(0), accY(0), accZ(0), annex0(0), annex1(0),
        crcReceived(0), crcCalculated(0), crcCheck(0), id(0),
        gnssStatus(0), flightStatus(0),
        latFloat(0.0f), lonFloat(0.0f), pressureFloat(0.0f), tempFloat(0.0f),
        accXFloat(0.0f), accYFloat(0.0f), accZFloat(0.0f),
        annex0Float(0.0f), annex1Float(0.0f), altitudeBaroFloat(0.0f)
    {}

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
