#include "serialport.h"

Q_DECLARE_METATYPE(QSerialPort::SerialPortError);
Q_DECLARE_METATYPE(SerialPort::Settings);

SerialPort::SerialPort() :
    QThread(nullptr),
    m_serial(new QSerialPort(this)),
    m_settingsPort(new Settings),
    m_serialRun(false)
{
    qRegisterMetaType<QSerialPort::SerialPortError>();
    qRegisterMetaType<SerialPort::Settings>();

    connect(m_serial, &QSerialPort::errorOccurred, this, &SerialPort::handleError);
}

SerialPort::~SerialPort()
{
    m_serial->close();
    delete m_settingsPort;
    delete m_serial;
}

void SerialPort::run()
{
    exec();  // démarrage de la boucle d'événements
}

void SerialPort::settingUpdate(SerialPort::Settings settingPort)
{
    qDebug() << "[" << QDateTime::currentDateTime().toString("dd-MM-yyyy_HH.mm.ss") << "][SERIAL] settingUpdate";
    *m_settingsPort = settingPort;
}

void SerialPort::handleError(QSerialPort::SerialPortError error)
{
    qDebug() << "[" << QDateTime::currentDateTime().toString("dd-MM-yyyy_HH.mm.ss") << "][SERIAL] handleError";
    if (error != QSerialPort::NoError) {
        m_serialRun = false;
        qDebug() << "[" << QDateTime::currentDateTime().toString("dd-MM-yyyy_HH.mm.ss") << "][SERIAL] emit errorEmit";
        emit SerialPort::errorEmit(m_serial->errorString());
    }
}

void SerialPort::openSerial()
{
    m_serial->setPortName(m_settingsPort->name);
    m_serial->setBaudRate(m_settingsPort->baudRate);
    m_serial->setDataBits(m_settingsPort->dataBits);
    m_serial->setParity(m_settingsPort->parity);
    m_serial->setStopBits(m_settingsPort->stopBits);
    m_serial->setFlowControl(m_settingsPort->flowControl);

    m_serial->setReadBufferSize(512);

    if (m_serial->open(QIODevice::ReadOnly)) {
        m_serial->clear(QSerialPort::AllDirections);
        emit serialOpenned(*m_settingsPort);
        m_serialRun = true;

        connect(m_serial, &QSerialPort::readyRead, this, &SerialPort::readingData);
    }
    else {
        handleError(m_serial->error());
    }
}

void SerialPort::closeSerial()
{
    if (m_serial->isOpen()) {
        m_serial->clear(QSerialPort::AllDirections);
        m_serial->close();
        m_serialRun = false;
        emit serialClosed();
    }
}


void SerialPort::readingData() {

    if (m_serial->bytesAvailable() == NbTrame+3){

        QByteArray frame = m_serial->read(NbTrame+3);
        //m_serial->clear(QSerialPort::Input); // Clear the read buffer

        // Debugging: Print the frame
        qDebug() << "[" << QDateTime::currentDateTime().toString("hh:mm:ss.zzz") << "][SERIAL] Frame: " << frame.toHex(' ');


        // Vérifier la longueur de la trame
        if (frame.size() != NbTrame+3) {
        // La trame est trop courte, renvoyer une QByteArray vide pour indiquer une erreur
            //emit dataEmit(false, QByteArray());
            qDebug() << "[" << QDateTime::currentDateTime().toString("dd-MM-yyyy_HH.mm.ss") << "][SERIAL] Size frame is not good";
            return;
        }
       else{
        // Vérifier le premier bit
            std::uint8_t firstBit = frame.at(0);
            if (firstBit != 0xEE) {
                // Le premier bit ne correspond pas à ce qui est attendu, renvoyer une QByteArray vide pour indiquer une erreur
                qDebug() << "[" << QDateTime::currentDateTime().toString("dd-MM-yyyy_HH.mm.ss") << "][SERIAL] First bit is not good";
                //emit dataEmit(false, QByteArray());
                return;
            }
            else{
                // Extraire le CRC et les données
                std::uint8_t receivedCrc = frame.at(1);  // si le CRC est le deuxième byte
                QByteArray receivedData = frame.mid(2, frame.size() - 3);  // supprime le premier bit, le CRC et le dernier bit
                qDebug() << "[" << QDateTime::currentDateTime().toString("dd-MM-yyyy_HH.mm.ss") << "][SERIAL] Received CRC: " << QString::number(receivedCrc, 16);

                // Vérifier le CRC
                std::uint8_t calculatedCrc = calculate_crc8(receivedData);
                if (receivedCrc != calculatedCrc) {
                    // Le CRC ne correspond pas, renvoyer une QByteArray vide pour indiquer une erreur
                    qDebug() << "[" << QDateTime::currentDateTime().toString("dd-MM-yyyy_HH.mm.ss") << "][SERIAL] CRC it's not good";
                    emit dataEmit(false, QByteArray());
                }
                else {
                    emit dataEmit(true, receivedData);
                }
            }
        }
    }
}


uint8_t SerialPort::calculate_crc8(QByteArray data) {
    uint8_t crc = 0;
    for (auto byte : data) {
        uint8_t extract = byte;
        for (uint8_t tempI = 8; tempI; tempI--) {
            uint8_t sum = (crc ^ extract) & 0x01;
            crc >>= 1;
            if (sum) {
                crc ^= CRC8_DPOLY;
            }
            extract >>= 1;
        }
    }
    return crc;
}
