#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#define VERSION_SERIAL 1.2f // Définition de la version du protocole série

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

#include "serialport.h"
#include "settingsdialog.h"
#include "telemetryframe.h"

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

signals:
    // Définition des signaux qui peuvent être émis par l'objet MainWindow
    void setSerialSettingsSig(SerialPort::Settings);
    void serialOpened(SerialPort::Settings p);
    void serialClosed();

private:
    // Définition des méthodes privées et des membres de données
    void initActionsConnections();
    void initActionsConnectionsPrio();
    void activateButtonSerial();
    void disactivateButtonSerial();
    void showStatusMessage(const QString &stringConnection);
    void clearConsole();

    void applyTelemetryPanelStyle();

    void updateLatitude(float latitude);
    void updateLongitude(float longitude);
    void updateAltitude(int altitude);
    void updatePressure(float pressure);
    void updateTemperature(float temperature);
    void updateAccelerationX(float accX);
    void updateAccelerationY(float accY);
    void updateAccelerationZ(float accZ);
    void updateGnssStatus(uint8_t gnssStatus);
    void updateFlightStatus(uint8_t flightStatus);
    void updateCrcCheckLabel(bool crcCheck);

    Ui::MainWindow *ui;
    QObject *m_parent;
    QLabel *m_status = nullptr;
    SerialPort *m_serialThread = nullptr;
    TelemetryFrame *m_frameTelemetry = nullptr;;

    QString m_connection;
    QString m_versionSW;
    bool m_serialRun;
    QFile m_logFile;
    int m_msgCounter = 0;

    SettingsDialog *m_settings = nullptr;
    SerialPort::Settings *m_settingsInfo = nullptr;
};

#endif // MAINWINDOW_H
