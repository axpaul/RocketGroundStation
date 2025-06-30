#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#define VERSION_SERIAL 1.3f // Définition de la version du protocole série

#include <QMainWindow>
#include <QSerialPort>
#include <QMessageBox>
#include <QLabel>
#include <QtDebug>
#include <QWidget>
#include <QPixmap>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QFile>
#include <QFileDialog>
#include <QMutex>

#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QDateTimeAxis>
#include <QtCharts/QValueAxis>
#include <QtCharts/QSplineSeries>
#include <QtCharts/QAbstractAxis>

#include <QtCore/QTimer>
#include <QtCore/QDateTime>

#include "serialport.h"
#include "settingsdialog.h"
#include "telemetryframe.h"
#include "voicemanager.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class SerialPort;
class SettingsDialog;

// Définition de la classe principale MainWindow
class MainWindow : public QMainWindow
{
    Q_OBJECT // Macro nécessaire pour toute classe définissant ses propres signaux et slots

public:
    // Constructeur et destructeur
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    // Méthodes publiques
    SerialPort::Settings getSerialInfo();
    QString getSerialError();
    void addText(const QString &text);

public slots:
    // Slots qui vont être utilisés pour connecter les signaux et les méthodes.
    void handleErrorShow(QString error);
    void settingShow();
    void setSerialSettings();
    void openedSerial(SerialPort::Settings p);
    void closedSerial();
    void openSerialPort();
    void closeSerialPort();
    void receptionData(const TmFrame_t &frame, const QString &decodedString);
    void about();
    void watchdogLostHandler();

signals:
    // Définition des signaux qui peuvent être émis par l'objet MainWindow
    void setSerialSettingsSig(SerialPort::Settings);
    void serialOpened(SerialPort::Settings p);
    void serialClosed();
    void triggerBeep();
    void triggerSpeaker(uint8_t newStatus);

private:
    // Définition des méthodes privées et des membres de données
    void initActionsConnections();
    void initActionsConnectionsPrio();
    void activateButtonSerial();
    void disactivateButtonSerial();
    void showStatusMessage(const QString &stringConnection);
    void clearConsole();

    void initGraphicAcc();
    void addAccelerationsAndScroll(float accX, float accY, float accZ);
    void initGraphicPressureAltitude();
    void addPressureAltitudeAndScroll(float pressure, float altitudeGNSS, float altitudeBaro);

    void applyTelemetryPanelStyle();

    void fontLabel();
    void updateLatitude(float latitude);
    void updateLongitude(float longitude);
    void updateAltitudeGPS(int altitude);
    void updatePressure(float pressure);
    void updateTemperature(float temperature);
    void updateAccelerationX(float accX);
    void updateAccelerationY(float accY);
    void updateAccelerationZ(float accZ);
    void updateGnssStatus(uint8_t gnssStatus);
    void updateFlightStatus(uint8_t flightStatus);
    void updateCrcCheckLabel(bool crcCheck);
    void updateAltitudeBaro(float altitude);
    void updateGyroX(float gyroX);
    void updategyroY(float gyroY);
    void updategyroZ(float gyroZ);

    Ui::MainWindow *ui;
    QObject *m_parent;
    QLabel *m_status = nullptr;
    SerialPort *m_serialThread = nullptr;
    TelemetryFrame *m_frameTelemetry = nullptr;
    VoiceManager *m_voiceManager = nullptr;

    QString m_connection;
    QString m_versionSW;
    QFile m_logFile;
    int m_msgCounter = 0;

    SettingsDialog *m_settings = nullptr;
    SerialPort::Settings *m_settingsInfo = nullptr;

    QLineSeries *m_seriesAccX;
    QLineSeries *m_seriesAccY;
    QLineSeries *m_seriesAccZ;
    QDateTimeAxis *m_axisAccX;
    QValueAxis *m_axisAccY;

    QLineSeries *m_seriesPressure;
    QLineSeries *m_seriesAltitudeGNSS;
    QLineSeries *m_seriesAltitudeBaro;
    QDateTimeAxis *m_axisPressX;
    QValueAxis *m_axisPressY;
    QValueAxis *m_axisAltY;
};

#endif // MAINWINDOW_H
