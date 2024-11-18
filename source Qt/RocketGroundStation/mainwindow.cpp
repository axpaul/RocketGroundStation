#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      m_status(new QLabel(this)),
      m_serialThread(new SerialPort),
      m_frameTelemetry(new TelemetryFrame),
      m_settings(new SettingsDialog),
      m_settingsInfo(new SerialPort::Settings)
{
    ui->setupUi(this);

    qDebug() << "Window :" << QThread::currentThread() << Qt::endl;

    m_serialRun = false;
    m_serialThread->start();

    initActionsConnectionsPrio();

    setSerialSettings();

    ui->statusbar->addWidget(m_status);

    initActionsConnections();
    disactivateButtonSerial();

    /*ui->latitudeLabel->setFixedSize(250, 25);
    ui->longitudeLabel->setFixedSize(250, 25);
    ui->altitudeLabel->setFixedSize(250, 25);
    ui->pressureLabel->setFixedSize(250, 25);
    ui->temperatureLabel->setFixedSize(250, 25);
    ui->accXLabel->setFixedSize(250, 25);
    ui->accYLabel_3->setFixedSize(250, 25);
    ui->accZLabel->setFixedSize(250, 25);
    ui->gnssStatusLabel->setFixedSize(250, 25);
    ui->crcCheckLabel->setFixedSize(250, 25);
    ui->flightStatusLabel->setFixedSize(250, 25);*/

    initGraphicAcc();
    initGraphicPressureAltitude();

    qDebug() << "[" << QDateTime::currentDateTime().toString("dd-MM-yyyy_HH.mm.ss") << "][MAINWINDOW] " << QThread::currentThread();
}

MainWindow::~MainWindow()
{
    /*m_serialThread->quit();
    m_serialThread->wait();
    delete m_serialThread;*/

    delete m_frameTelemetry;
    delete m_settings;
    delete m_settingsInfo;
    delete m_status;
    delete ui;

    delete m_seriesAccX;
    delete m_seriesAccY;
    delete m_seriesAccZ;
    delete m_axisAccX;
    delete m_axisAccY;
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
    m_serialRun = true;
    showStatusMessage(QString("Connected to %1 : %2, %3, %4, %5, %6")
                      .arg(p.name).arg(p.stringBaudRate).arg(p.stringDataBits)
                      .arg(p.stringParity).arg(p.stringStopBits).arg(p.stringFlowControl));

    activateButtonSerial();

}

void MainWindow::closedSerial() {
    qDebug() << "[" << QDateTime::currentDateTime().toString("dd-MM-yyyy_HH.mm.ss")<< "][MAINWINDOW] Serial closed";
    m_serialRun = false;
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

        // Générer une chaîne contenant la trame en hexadécimal pour le fichier
        QString frameFile = frame.frame.toHex(' ');

        addText(timestamp + " " + frameFile);
        addText(decodedString);

        updateLatitude(frame.latFloat);
        updateLongitude(frame.lonFloat);
        updateAltitude(frame.altGNSS);
        updatePressure(frame.pressureFloat);
        updateTemperature(frame.tempFloat);
        updateAccelerationX(frame.accXFloat);
        updateAccelerationY(frame.accYFloat);
        updateAccelerationZ(frame.accZFloat);
        updateGnssStatus(frame.gnssStatus);
        updateFlightStatus(frame.flightStatus);
        updateCrcCheckLabel(frame.crcCheck);

        ui->mapzone->setPosition((double)frame.lat * 1e-7, (double)frame.lon * 1e-7);

        if (m_logFile.isOpen()) {
                QTextStream out(&m_logFile);
                out << timestamp << ";" << frameFile << ";"
                    << frame.sts << ";"
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

        if (frame.crcCheck){
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

void MainWindow::clearConsole() {
    ui->console->clear();
}

/* Dashboard update function */

void MainWindow::updateLatitude(float latitude) {
    QString text = QString("Latitude : %1 °").arg(latitude, 0, 'f', 7);
    ui->latitudeLabel->setText(text);
}

void MainWindow::updateLongitude(float longitude) {
    QString text = QString("Longitude : %1 °").arg(longitude, 0, 'f', 7);
    ui->longitudeLabel->setText(text);
}

void MainWindow::updateAltitude(int altitude) {
    QString text = QString("Altitude : %1 m").arg(altitude);
    ui->altitudeLabel->setText(text);

    // Colorer en rouge si l'altitude dépasse 10 000 m
    if (altitude > 10000) {
        ui->altitudeLabel->setStyleSheet("color: red;");
    } else {
        ui->altitudeLabel->setStyleSheet("color: black;");
    }
}

void MainWindow::updatePressure(float pressure) {
    QString text = QString("Pression : %1 mBar").arg(pressure, 0, 'f', 2);
    ui->pressureLabel->setText(text);

    // Coloration conditionnelle pour des valeurs anormales de pression
    if (pressure < 300 || pressure > 1200) {
        ui->pressureLabel->setStyleSheet("color: orange;");
    } else {
        ui->pressureLabel->setStyleSheet("color: black;");
    }
}

void MainWindow::updateTemperature(float temperature) {
    QString text = QString("Température : %1 °C").arg(temperature, 0, 'f', 2);
    ui->temperatureLabel->setText(text);

    // Changer en rouge si la température est critique
    if (temperature > 50) {
        ui->temperatureLabel->setStyleSheet("color: red;");
    } else {
        ui->temperatureLabel->setStyleSheet("color: black;");
    }
}

void MainWindow::updateAccelerationX(float accX) {
    QString text = QString("Accélération X : %1 g").arg(accX, 0, 'f', 3);
    ui->accXLabel->setText(text);
}

void MainWindow::updateAccelerationY(float accY) {
    QString text = QString("Accélération Y : %1 g").arg(accY, 0, 'f', 3);
    ui->accYLabel->setText(text);
}

void MainWindow::updateAccelerationZ(float accZ) {
    QString text = QString("Accélération Z : %1 g").arg(accZ, 0, 'f', 3);
    ui->accZLabel->setText(text);
}

void MainWindow::updateGnssStatus(uint8_t gnssStatus) {

    QString statusText;
    QString colorStyle;

    if (gnssStatus == 0) {
        statusText = "No signal";
        colorStyle = "color: red;";
    } else {
        statusText = "Signal Ok";
        colorStyle = "color: green;";
    }

    ui->gnssStatusLabel->setText("Statut GNSS : " + statusText);
    ui->gnssStatusLabel->setStyleSheet(colorStyle);
}

void MainWindow::updateCrcCheckLabel(bool crcCheck){
    QString statusText;
    QString colorStyle;

    if (crcCheck) {
        statusText = "OK";
        colorStyle = "color: green;";
    } else {
        statusText = "ERROR";
        colorStyle = "color: red;";
    }

    ui->crcCheckLabel->setText("CrcCheck : " + statusText);
    ui->crcCheckLabel->setStyleSheet(colorStyle);
}

void MainWindow::updateFlightStatus(uint8_t flightStatus) {
    QString statusText;
    QString colorStyle;

    // Attribuer un texte et une couleur en fonction du statut
    switch (flightStatus) {
    case PRE_FLIGHT:
        statusText = "Pré-Vol";
        colorStyle = "color: blue;";
        break;
    case PYRO_RDY:
        statusText = "Prêt Pyrotechnique";
        colorStyle = "color: red;";
        break;
    case ASCEND:
        statusText = "Ascension";
        colorStyle = "color: green;";
        break;
    case DEPLOY_ALGO:
        statusText = "Déploiement Algorithme";
        colorStyle = "color: orange;";
        break;
    case DEPLOY_TIMER:
        statusText = "Déploiement Timer";
        colorStyle = "color: purple;";
        break;
    case DESCEND:
        statusText = "Descente";
        colorStyle = "color: brown;";
        break;
    case TOUCHDOWN:
        statusText = "Atterrissage";
        colorStyle = "color: gray;";
        break;
    default:
        statusText = "Inconnu";
        colorStyle = "color: black;";
    }

    ui->flightStatusLabel->setText("Statut de Vol : " + statusText);
    ui->flightStatusLabel->setStyleSheet(colorStyle);
}

/* Chart management function */

void MainWindow::initGraphicAcc() {
    // Obtenir le QChart déjà associé au QChartView
    QChart *chart = ui->graphicsViewAcceleration->chart();
    if (!chart) {
        qDebug() << "Erreur : Le graphique (chart) n'est pas initialisé.";
        return; // Assurez-vous que le QChart existe
    }

    // Configurer le titre, légende et animation
    //chart->setTitle("Graphique des accélérations");
    chart->legend()->setVisible(true); // Affiche la légende pour identifier les séries
    chart->setAnimationOptions(QChart::AllAnimations);

    // Créer les séries pour accX, accY et accZ
    m_seriesAccX = new QLineSeries();
    m_seriesAccX->setName("Accélération X");
    chart->addSeries(m_seriesAccX);

    m_seriesAccY = new QLineSeries();
    m_seriesAccY->setName("Accélération Y");
    chart->addSeries(m_seriesAccY);

    m_seriesAccZ = new QLineSeries();
    m_seriesAccZ->setName("Accélération Z");
    chart->addSeries(m_seriesAccZ);

    // Configurer l'axe X (temps)
    m_axisAccX = new QDateTimeAxis();
    m_axisAccX->setTickCount(15);
    m_axisAccX->setFormat("hh:mm:ss");
    m_axisAccX->setTitleText("Temps");
    chart->addAxis(m_axisAccX, Qt::AlignBottom);

    // Attacher les séries à l'axe X
    m_seriesAccX->attachAxis(m_axisAccX);
    m_seriesAccY->attachAxis(m_axisAccX);
    m_seriesAccZ->attachAxis(m_axisAccX);

    // Configurer l'axe Y (accélération)
    m_axisAccY = new QValueAxis();
    m_axisAccY->setLabelFormat("%.2f");
    m_axisAccY->setTitleText("Accélération (g)");
    m_axisAccY->setRange(-16, 16); // Ajustez selon vos besoins
    chart->addAxis(m_axisAccY, Qt::AlignLeft);

    // Attacher les séries à l'axe Y
    m_seriesAccX->attachAxis(m_axisAccY);
    m_seriesAccY->attachAxis(m_axisAccY);
    m_seriesAccZ->attachAxis(m_axisAccY);

    // Définir les limites initiales de l'axe X
    m_axisAccX->setMin(QDateTime::currentDateTime().addSecs(0)); // Afficher les 30 dernières secondes
    m_axisAccX->setMax(QDateTime::currentDateTime().addSecs(120));

    QPen penX(Qt::red);
    penX.setWidth(2);
    m_seriesAccX->setPen(penX);

    QPen penY(Qt::green);
    penY.setWidth(2);
    m_seriesAccY->setPen(penY);

    QPen penZ(Qt::blue);
    penZ.setWidth(2);
    m_seriesAccZ->setPen(penZ);
}

void MainWindow::addAccelerationsAndScroll(float accX, float accY, float accZ) {
    // Obtenir le temps actuel
    qint64 currentTime = QDateTime::currentDateTime().toMSecsSinceEpoch();

    // Ajouter les données aux séries respectives
    m_seriesAccX->append(currentTime, accX);
    m_seriesAccY->append(currentTime, accY);
    m_seriesAccZ->append(currentTime, accZ);

    // Vérifier si le temps actuel dépasse la limite de l'axe X
    if (QDateTime::currentDateTime() >= m_axisAccX->max()) {
        ui->graphicsViewAcceleration->chart()->scroll(120, 0); // Faire défiler le graphique vers la droite
    }

    // Limiter le nombre de points dans chaque série pour éviter une surcharge
    if (m_seriesAccX->count() > 1000) {
        m_seriesAccX->remove(0);
        m_seriesAccY->remove(0);
        m_seriesAccZ->remove(0);
    }
}

void MainWindow::initGraphicPressureAltitude() {
    // Obtenir le QChart associé au QChartView pour la pression et l'altitude
    QChart *chart = ui->graphicsViewAltitude->chart();
    if (!chart) {
        qDebug() << "Erreur : Le graphique (chart) n'est pas initialisé.";
        return; // Vérifiez que le QChart existe
    }

    // Configurer le titre, légende et animation
    //chart->setTitle("Graphique Pression et Altitude");
    chart->legend()->setVisible(true); // Affiche la légende pour identifier les séries
    chart->setAnimationOptions(QChart::AllAnimations);

    // Créer les séries pour pression et altitude
    m_seriesPressure = new QLineSeries();
    m_seriesPressure->setName("Pression (hPa)");
    chart->addSeries(m_seriesPressure);

    m_seriesAltitudeGNSS = new QLineSeries();
    m_seriesAltitudeGNSS->setName("Altitude GNSS (m)");
    chart->addSeries(m_seriesAltitudeGNSS);

    m_seriesAltitudeBaro = new QLineSeries();
    m_seriesAltitudeBaro ->setName("Altitude Baro (m)");
    chart->addSeries(m_seriesAltitudeBaro);

    // Configurer l'axe X (temps)
    m_axisPressX = new QDateTimeAxis();
    m_axisPressX->setTickCount(15);
    m_axisPressX->setFormat("hh:mm:ss");
    m_axisPressX->setTitleText("Temps");
    chart->addAxis(m_axisPressX, Qt::AlignBottom);

    // Attacher les séries à l'axe X
    m_seriesPressure->attachAxis(m_axisPressX);
    m_seriesAltitudeGNSS->attachAxis(m_axisPressX);
    m_seriesAltitudeBaro->attachAxis(m_axisPressX);

    // Configurer l'axe Y (pression)
    m_axisPressY = new QValueAxis();
    m_axisPressY->setLabelFormat("%.2f");
    m_axisPressY->setTitleText("Pressure (hP)");
    m_axisPressY->setRange(500, 1100);
    chart->addAxis(m_axisPressY, Qt::AlignLeft);

    // Configurer l'axe Y (altitude)
    m_axisAltY = new QValueAxis();
    m_axisAltY->setLabelFormat("%.2f");
    m_axisAltY->setTitleText("Altitude (m)");
    m_axisAltY->setRange(0, 5000);
    chart->addAxis(m_axisAltY, Qt::AlignRight);

    // Attacher les séries à l'axe Y
    m_seriesPressure->attachAxis(m_axisPressY);
    m_seriesAltitudeGNSS->attachAxis(m_axisAltY);
    m_seriesAltitudeBaro->attachAxis(m_axisAltY);

    // Définir les limites initiales de l'axe X
    m_axisPressX->setMin(QDateTime::currentDateTime().addSecs(0));
    m_axisPressX->setMax(QDateTime::currentDateTime().addSecs(120));

    // Personnaliser les couleurs des séries
    QPen penPressure(Qt::magenta);
    penPressure.setWidth(2);
    m_seriesPressure->setPen(penPressure);

    QPen penAltitudeGNSS(Qt::cyan);
    penAltitudeGNSS.setWidth(2);
    m_seriesAltitudeGNSS->setPen(penAltitudeGNSS);

    QPen penAltitudeBaro(Qt::gray);
    penAltitudeBaro.setWidth(2);
    m_seriesAltitudeGNSS->setPen(penAltitudeBaro);
}

void MainWindow::addPressureAltitudeAndScroll(float pressure, float altitudeGNSS, float altitudeBaro) {
    // Obtenir le temps actuel
    qint64 currentTime = QDateTime::currentDateTime().toMSecsSinceEpoch();

    // Ajouter les données aux séries respectives
    m_seriesPressure->append(currentTime, pressure);
    m_seriesAltitudeGNSS->append(currentTime, altitudeGNSS);
    m_seriesAltitudeBaro->append(currentTime, altitudeBaro);


    // Vérifier si le temps actuel dépasse la limite de l'axe X
    if (QDateTime::currentDateTime() >= m_axisPressX->max()) {
        ui->graphicsViewAltitude->chart()->scroll(120, 0); // Faire défiler le graphique vers la droite
    }

    // Limiter le nombre de points dans chaque série
    while (m_seriesPressure->count() > 1000) {
        m_seriesPressure->remove(0);
        m_seriesAltitudeGNSS->remove(0);
    }
}


