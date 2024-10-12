#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      m_serialThread(new SerialPort),
      m_settings(new SettingsDialog),
      m_status(new QLabel(this)),
      m_connection(new QString),
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


    qDebug() << "[" << QDateTime::currentDateTime().toString("dd-MM-yyyy_HH.mm.ss") << "][MAINWINDOW] " << QThread::currentThread();
}

MainWindow::~MainWindow()
{
    /*m_serialThread->quit();
    m_serialThread->wait();
    delete m_serialThread;*/

    delete m_settings;
    delete m_settingsInfo;
    delete m_status;
    delete ui;
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

    connect(ui->actionConnect, &QAction::triggered, this, &MainWindow::openSerialPort);
    connect(ui->actionDisconnect, &QAction::triggered, this, &MainWindow::closeSerialPort);

    connect(m_serialThread, &SerialPort::serialOpenned, this, &MainWindow::opennedSerial);
    connect(m_serialThread, &SerialPort::serialClosed, this, &MainWindow::closedSerial);
    connect(m_serialThread, &SerialPort::dataEmit, this, &MainWindow::receptionData);
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

    if (stringConnection != "" && stringConnection != *m_connection)
    {
        *m_connection = stringConnection;
    }

    message = QString("%1").arg(*m_connection);

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

void MainWindow::opennedSerial(SerialPort::Settings p) {
    qDebug() << "[" << QDateTime::currentDateTime().toString("dd-MM-yyyy_HH.mm.ss") << "][MAINWINDOW] Serial openned";
    m_serialRun = true;
    showStatusMessage(QString("Connected to %1 : %2, %3, %4, %5, %6")
                      .arg(p.name).arg(p.stringBaudRate).arg(p.stringDataBits)
                      .arg(p.stringParity).arg(p.stringStopBits).arg(p.stringFlowControl));

    activateButtonSerial();

   //ui->mapzone->setPosition(43.2184, -0.0475); // Point de passage 1
   //ui->mapzone->setPosition(43.219, -0.048); // Point de passage 2
   //ui->mapzone->setPosition(43.22, -0.049); // Point de passage 3

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

void MainWindow::receptionData(bool receptionChek, const QByteArray byteArr) {

    if (receptionChek){

        QString timestamp = QDateTime::currentDateTime().toString("[hh:mm:ss.zzz] ");

        // Générer une chaîne contenant la trame en hexadécimal
        QString frame = byteArr.toHex(' ');

        // Générer une chaîne contenant la trame en hexadécimal pour le fichier
        QString frameFile = byteArr.toHex(' ');

        trame.pressure = (byteArr[3] & 0xFF) << 24 |
        (byteArr[2] & 0xFF) << 16 |
        (byteArr[1] & 0xFF) << 8 |
        (byteArr[0] & 0xFF);
        trame.lat = (byteArr[7] & 0xFF) << 24 |
        (byteArr[6] & 0xFF) << 16 |
        (byteArr[5] & 0xFF) << 8 |
        (byteArr[4] & 0xFF);
        //trame.lat = trame.lat * 1e-7;
        trame.lon = (byteArr[11] & 0xFF) << 24 |
        (byteArr[10] & 0xFF) << 16 |
        (byteArr[9] & 0xFF) << 8 |
        (byteArr[8] & 0xFF);
        //trame.lon = trame.lon * 1e-7;
        trame.alt = (byteArr[13] & 0xFF) << 8 |
        (byteArr[12] & 0xFF);
        trame.temp = (byteArr[15] & 0xFF) << 8 |
        (byteArr[14] & 0xFF);
        trame.accX = (byteArr[17] & 0xFF) << 8 |
        (byteArr[16] & 0xFF);
        trame.accY = (byteArr[19] & 0xFF) << 8 |
        (byteArr[18] & 0xFF);
        trame.accZ = (byteArr[21] & 0xFF) << 8 |
        (byteArr[20] & 0xFF);
        trame.annex0 = (byteArr[23] & 0xFF) << 8 |
        (byteArr[22] & 0xFF);
        trame.annex1 = (byteArr[25] & 0xFF) << 8 |
        (byteArr[24] & 0xFF);
        trame.sts = byteArr[26];

        // Afficher les données décodées de trame
        QString decodedData = QString("[Decoded data] "
                                      "sts: %1 | "
                                      "lat: %2 | "
                                      "lon: %3 | "
                                      "alt: %4 | "
                                      "pressure: %5 | "
                                      "temp: %6 | "
                                      "annex0: %7 | "
                                      "annex1: %8 | "
                                      "accX: %12 | "
                                      "accY: %13 | "
                                      "accZ: %14")
                                .arg(trame.sts)
                                .arg(trame.lat)
                                .arg(trame.lon)
                                .arg(trame.alt)
                                .arg(trame.pressure)
                                .arg(trame.temp)
                                .arg(trame.annex0)
                                .arg(trame.annex1)
                                .arg(trame.accX)
                                .arg(trame.accY)
                                .arg(trame.accZ);

        addText(timestamp + " " + frame);
        addText(decodedData);
        ui->mapzone->setPosition((double)trame.lat * 1e-7, (double)trame.lon * 1e-7);

        if (m_logFile.isOpen()) {
                QTextStream out(&m_logFile);
                out << timestamp << ";" << frameFile << ";"
                    << trame.sts << ";"
                    << trame.lat << ";"
                    << trame.lon << ";"
                    << trame.alt << ";"
                    << trame.pressure << ";"
                    << trame.temp << ";"
                    << trame.accX << ";"
                    << trame.accY << ";"
                    << trame.accZ << ";"
                    << trame.annex0 << ";"
                    << trame.annex1 << "\n";
        }

    }
}

void MainWindow::addText(const QString &text) {
    ui->console->append(text); // ajoute le texte à la console

    // obtenir toutes les lignes actuellement dans la console
    QStringList allLines = ui->console->toPlainText().split("\n");

    // si plus de 10 lignes, supprimer les lignes supplémentaires
    while (allLines.size() > 500) {
        allLines.removeFirst();
    }

    // mettre à jour la console avec les 10 dernières lignes
    ui->console->clear();
    ui->console->append(allLines.join("\n"));
}
