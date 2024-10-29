#ifndef SERIALPORT_H
#define SERIALPORT_H

#include <QThread>
#include <QSerialPort>
#include <QDateTime>
#include <QMutex>
#include <QtDebug>

#define NbTrame  27
#define CRC8_DPOLY 0x31

class SettingsDialog;

class SerialPort : public QThread
{
    Q_OBJECT

public:
    struct Settings {
        QString name;
        qint32 baudRate;
        QString stringBaudRate;
        QSerialPort::DataBits dataBits;
        QString stringDataBits;
        QSerialPort::Parity parity;
        QString stringParity;
        QSerialPort::StopBits stopBits;
        QString stringStopBits;
        QSerialPort::FlowControl flowControl;
        QString stringFlowControl;
    };

    SerialPort();
    ~SerialPort();

signals:
    void errorEmit(QString);
    void dataEmit(bool receptionChek, QByteArray data = NULL);
    void serialOpenned(SerialPort::Settings p);
    void serialClosed();

public slots:
    void openSerial();
    void closeSerial();
    void settingUpdate(SerialPort::Settings settingPort);
    void handleError(QSerialPort::SerialPortError error);

protected:
    void run() override;

private:
    uint8_t calculate_crc8(QByteArray data);
    void readingData();

    Settings *m_settingsPort;
    QSerialPort *m_serial;
    bool m_serialRun;
    QMutex mut;
};

#endif // SERIALPORT_H
