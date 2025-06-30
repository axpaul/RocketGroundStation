#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      m_status(new QLabel(this)),
      m_serialThread(new SerialPort),
      m_frameTelemetry(new TelemetryFrame),
      m_voiceManager(new VoiceManager),
      m_settings(new SettingsDialog),
      m_settingsInfo(new SerialPort::Settings)
{
    ui->setupUi(this);

     qDebug() << "[" << QDateTime::currentDateTime().toString("dd-MM-yyyy_HH.mm.ss") << "][MAINWINDOW] " << QThread::currentThread();

    m_serialThread->start();
    m_voiceManager->start();

    initActionsConnectionsPrio();

    setSerialSettings();

    ui->statusbar->addWidget(m_status);

    initActionsConnections();
    disactivateButtonSerial();
    initGraphicAcc();
    initGraphicPressureAltitude();

    //fontLabel();

    m_voiceManager->isEnabled();
}

MainWindow::~MainWindow()
{

    if (m_serialThread) {
        m_serialThread->stop();  // Signale au thread d'arrêter sa boucle
        m_serialThread->quit();  // Stoppe la boucle d'événements, si utilisée
        m_serialThread->wait();  // Attend que le thread se termine proprement
    }

    if(m_voiceManager) {
        m_voiceManager->quit();
        m_voiceManager->wait();
    }

    delete m_status;
    delete m_frameTelemetry;

    delete m_settings;
    delete m_settingsInfo;

    delete ui;

    delete m_seriesAccX;
    delete m_seriesAccY;
    delete m_seriesAccZ;
    delete m_axisAccX;
    delete m_axisAccY;
    delete m_seriesPressure;
    delete m_seriesAltitudeGNSS;
    delete m_seriesAltitudeBaro;
    delete m_axisPressX;
    delete m_axisPressY;
    delete m_axisAltY;

}

void MainWindow::initActionsConnectionsPrio(){

    connect(this, SIGNAL(setSerialSettingsSig(SerialPort::Settings)), m_serialThread, SLOT(settingUpdate(SerialPort::Settings)));
    connect(m_serialThread, SIGNAL(errorEmit(QString)), this, SLOT(handleErrorShow(QString)));
}

void MainWindow::initActionsConnections()
{
    connect(ui->actionAbout_Qt, &QAction::triggered, qApp, &QApplication::aboutQt);
    connect(ui->actionQuit, &QAction::triggered, this, &MainWindow::close);
    connect(ui->actionAbout, &QAction::triggered, this, &MainWindow::about);
    connect(ui->actionConfigure, &QAction::triggered, m_settings, &SettingsDialog::showSetting); // set setting serial
    connect(m_settings, &SettingsDialog::applyParameter, this, &MainWindow::setSerialSettings);
    connect(ui->actionClean, &QAction::triggered, this, &MainWindow::clearConsole);

    connect(ui->actionConnect, &QAction::triggered, this, &MainWindow::openSerialPort);
    connect(ui->actionDisconnect, &QAction::triggered, this, &MainWindow::closeSerialPort);

    connect(m_serialThread, &SerialPort::serialOpened, this, &MainWindow::openedSerial);
    connect(m_serialThread, &SerialPort::serialClosed, this, &MainWindow::closedSerial);
    connect(m_serialThread, &SerialPort::dataEmit, m_frameTelemetry, &TelemetryFrame::processData);
    connect(m_frameTelemetry, &TelemetryFrame::frameDecoded, this, &MainWindow::receptionData);

    connect(ui->chronoWidget, &TimeZone::watchdogTimeout, this, &MainWindow::watchdogLostHandler);

    connect(ui->actionSpeaker, &QAction::triggered, m_voiceManager, &VoiceManager::setEnabled);
    connect(this, &MainWindow::triggerBeep, m_voiceManager, &VoiceManager::emitBeep);
    connect(this, &MainWindow::triggerSpeaker, m_voiceManager, &VoiceManager::updateStatus);
}

void MainWindow::handleErrorShow(QString error)
{
   QMessageBox::critical(this, QString("Critical Error"), error);
}

/* MainWindow Information */

void MainWindow::about(){
    QString textAbout;
    textAbout.asprintf("Serial Interface v2/v3 Interface\nVersion : %.1f", VERSION_SERIAL);
    QMessageBox::about(this,"About", textAbout);
}

void MainWindow::showStatusMessage(const QString &stringConnection)
{
    QString message;

    if (stringConnection != "" && stringConnection != m_connection)
    {
        m_connection = stringConnection;
    }

    message = QString("%1").arg(m_connection);

    m_status->setText(message);
}

/* Functions settings systems */

void MainWindow::settingShow(){
    m_settings->show();
}

void MainWindow::setSerialSettings() {
    emit setSerialSettingsSig(m_settings->settings());
}

/* Function open/close serial */

void MainWindow::openedSerial(SerialPort::Settings p) {
    qDebug() << "[" << QDateTime::currentDateTime().toString("dd-MM-yyyy_HH.mm.ss") << "][MAINWINDOW] Serial openned";
    showStatusMessage(QString("Connected to %1 : %2, %3, %4, %5, %6")
                      .arg(p.name).arg(p.stringBaudRate).arg(p.stringDataBits)
                      .arg(p.stringParity).arg(p.stringStopBits).arg(p.stringFlowControl));

    activateButtonSerial();

}

void MainWindow::closedSerial() {
    qDebug() << "[" << QDateTime::currentDateTime().toString("dd-MM-yyyy_HH.mm.ss")<< "][MAINWINDOW] Serial closed";
    showStatusMessage(QString("Disconnected"));
    disactivateButtonSerial();
}

void MainWindow::openSerialPort() {

    qDebug() << "[" << QDateTime::currentDateTime().toString("dd-MM-yyyy_HH.mm.ss")<< "][MAINWINDOW] Send Serial open";

    // Créer le nom du fichier basé sur la date et l'heure actuelles
    QString filename = "log-" + QDateTime::currentDateTime().toString("yyyyMMdd-hhmmss") + ".csv";
    m_logFile.setFileName(filename);

    if (m_logFile.open(QIODevice::Append | QIODevice::Text)) {
        QTextStream out(&m_logFile);
        // Écrire l'en-tête si le fichier est nouveau
        out << "Timestamp;Frame;Sts;Lat;Lon;Altitude;Pressure;Temperature;Acceleration X;Acceleration Y;Acceleration Z; Annex 0;Annex 1;\n";
    }
    m_serialThread->openSerial();
}

void MainWindow::closeSerialPort() {
    qDebug() << "[" << QDateTime::currentDateTime().toString("dd-MM-yyyy_HH.mm.ss")<< "][MAINWINDOW] Send Serial close";
    m_logFile.close();
    m_serialThread->closeSerial();
}

void MainWindow::activateButtonSerial(){

    ui->actionConnect->setEnabled(false);
    ui->actionDisconnect->setEnabled(true);
    ui->actionConfigure->setEnabled(false);
}

void MainWindow::disactivateButtonSerial(){

    ui->actionConnect->setEnabled(true);
    ui->actionDisconnect->setEnabled(false);
    ui->actionConfigure->setEnabled(true);
}

/* Function reception and data distribution */

void MainWindow::receptionData(const TmFrame_t &frame, const QString &decodedString) {
    QString timestamp = QDateTime::currentDateTime().toString("[hh:mm:ss.zzz] ");
    QString frameHex = frame.frame.toHex(' ');

    // Console display
    addText(timestamp + frameHex);
    addText(decodedString);

    // Update UI elements
    updateLatitude(frame.latFloat);
    updateLongitude(frame.lonFloat);
    updateAltitudeGPS(frame.altGNSS);
    updateAltitudeBaro(frame.altitudeBaroFloat);
    updatePressure(frame.pressureFloat);
    updateTemperature(frame.tempFloat);
    updateAccelerationX(frame.accXFloat);
    updateAccelerationY(frame.accYFloat);
    updateAccelerationZ(frame.accZFloat);
    updateGnssStatus(frame.gnssFix);
    updateFlightStatus(frame.flightStatus);
    updateCrcCheckLabel(frame.crcCheck);

    // Update map
    ui->mapzone->setPosition((double)frame.lat * 1e-7, (double)frame.lon * 1e-7);

    // Reset watchdog
    ui->chronoWidget->resetWatchdog();

    // Log to file
    if (m_logFile.isOpen()) {
        QTextStream out(&m_logFile);
        out << timestamp << ";" << frameHex << ";"
            << frame.sts << ";"
            << frame.flightStatus << ";"
            << frame.gnssFix << ";"
            << frame.lat << ";"
            << frame.lon << ";"
            << frame.altGNSS << ";"
            << frame.altitudeBaroFloat << ";"
            << frame.pressure << ";"
            << frame.temp << ";"
            << frame.accX << ";"
            << frame.accY << ";"
            << frame.accZ << ";"
            << frame.annex0 << ";"
            << frame.annex1 << "\n";
    }

    // Plot updates if frame is valid
    if (frame.crcCheck) {
        addAccelerationsAndScroll(frame.accXFloat, frame.accYFloat, frame.accZFloat);
        addPressureAltitudeAndScroll(frame.pressureFloat, frame.altGNSS, frame.altitudeBaroFloat);
    }
}

/* Console management function */

void MainWindow::addText(const QString &text) {
    // Ajoute le texte formaté en HTML à la console
    ui->console->appendHtml(text);

    // Limite le nombre de lignes à 500
    int maxLines = 500;

    // Vérifie si le nombre de lignes a dépassé la limite
    QTextDocument *doc = ui->console->document();
    int lineCount = doc->blockCount();

    if (lineCount > maxLines) {
        // Calculer le nombre de lignes à supprimer
        int excessLines = lineCount - maxLines;

        // Supprimer les blocs de texte excédentaires du début
        QTextCursor cursor(doc);
        cursor.movePosition(QTextCursor::Start);  // Aller au début
        for (int i = 0; i < excessLines; ++i) {
            cursor.select(QTextCursor::BlockUnderCursor);
            cursor.removeSelectedText();
            cursor.deleteChar();  // Supprimer le caractère de fin de ligne
        }
    }
}

void MainWindow::fontLabel(){
    // Appliquer un style en gras pour tous les labels
    QFont boldFont("Segoe UI", 9, QFont::Bold);

    // Labels de position
    ui->latitudeLabel->setFont(boldFont);
    ui->latitudeLabel->setText("Latitude : -- °");

    ui->longitudeLabel->setFont(boldFont);
    ui->longitudeLabel->setText("Longitude : -- °");

    // Labels d'altitude et pression
    ui->altitudeGPSLabel->setFont(boldFont);
    ui->altitudeGPSLabel->setText("Altitude gnss : -- m");

    ui->altitudeBaroLabel->setFont(boldFont);
    ui->altitudeBaroLabel->setText("Altitude baro : -- m");

    ui->pressureLabel->setFont(boldFont);
    ui->pressureLabel->setText("Pressure : -- hPa");

    ui->temperatureLabel->setFont(boldFont);
    ui->temperatureLabel->setText("Temperature : -- °C");

    // Labels d'accélération
    ui->accXLabel->setFont(boldFont);
    ui->accXLabel->setText("Acceleration X: -- g");

    ui->accYLabel->setFont(boldFont);
    ui->accYLabel->setText("Acceleration Y: -- g");

    ui->accZLabel->setFont(boldFont);
    ui->accZLabel->setText("Acceleration Z: -- g");

    // Labels d'état
    ui->gnssStatusLabel->setFont(boldFont);
    ui->gnssStatusLabel->setText("GNSS : Wait");
    ui->gnssStatusLabel->setStyleSheet("color: black;");

    ui->crcCheckLabel->setFont(boldFont);
    ui->crcCheckLabel->setText("CRC : Wait");
    ui->crcCheckLabel->setStyleSheet("color: black;");

    ui->flightStatusLabel->setFont(boldFont);
    ui->flightStatusLabel->setText("Flight Status : Wait");
    ui->flightStatusLabel->setStyleSheet("color: black;");

    //Labels d'état
    ui->gyroXLabel->setFont(boldFont);
    ui->gyroXLabel->setText("Motor Pressure : -- Bar");
    ui->gyroXLabel->setStyleSheet("color: black;");

    ui->gyroYLabel->setFont(boldFont);
    ui->gyroYLabel->setText("Tank Pressure : -- Bar");
    ui->gyroYLabel->setStyleSheet("color: black;");

}

void MainWindow::clearConsole() {
    ui->console->clear();

    // Réinitialiser les graphes
    if (m_seriesAccX) m_seriesAccX->clear();
    if (m_seriesAccY) m_seriesAccY->clear();
    if (m_seriesAccZ) m_seriesAccZ->clear();
    if (m_seriesPressure) m_seriesPressure->clear();
    if (m_seriesAltitudeGNSS) m_seriesAltitudeGNSS->clear();
    if (m_seriesAltitudeBaro) m_seriesAltitudeBaro->clear();

    // Recentrer les graphes
    QDateTime currentTime = QDateTime::currentDateTime();
    if (m_axisAccX) {
        m_axisAccX->setMin(currentTime);
        m_axisAccX->setMax(currentTime.addSecs(120));
    }
    if (m_axisPressX) {
        m_axisPressX->setMin(currentTime);
        m_axisPressX->setMax(currentTime.addSecs(120));
    }

    // Réinitialiser les labels de l'interface
    fontLabel();

    // Ajouter un message dans la console
    addText("<span style='color: gray;'>Interface reset to initial state.</span>");

    // Log dans la console pour le débogage
    qDebug() << "[" << QDateTime::currentDateTime().toString("dd-MM-yyyy_HH.mm.ss")
             << "][MAINWINDOW] Interface reset.";
}

void MainWindow::watchdogLostHandler()
{
    updateFlightStatus(LOST);
}




