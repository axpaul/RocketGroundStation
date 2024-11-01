#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      m_tileServer(new TileServer(this)),
      ui(new Ui::MainWindow),
      m_status(new QLabel(this)),
      m_serialThread(new SerialPort),
      m_frameTelemetry(new TelemetryFrame),
      m_settings(new SettingsDialog),
      m_settingsInfo(new SerialPort::Settings)

{
    QString tilePath = QCoreApplication::applicationDirPath() + "/map/";
    if (!m_tileServer->startServer()) {
        qCritical() << "Impossible de démarrer le serveur de tuiles Python";
    } else {
        qDebug() << "Serveur de tuiles Python démarré avec succès";
    }

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

    delete m_frameTelemetry;
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

    connect(m_serialThread, &SerialPort::serialOpened, this, &MainWindow::openedSerial);
    connect(m_serialThread, &SerialPort::serialClosed, this, &MainWindow::closedSerial);
    //connect(m_serialThread, &SerialPort::dataEmit, this, &MainWindow::receptionData);
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

void MainWindow::receptionData(const TmFrame_t &frame, const QString &decodedString) {


        QString timestamp = QDateTime::currentDateTime().toString("[hh:mm:ss.zzz] ");

        // Générer une chaîne contenant la trame en hexadécimal pour le fichier
        QString frameFile = frame.frame.toHex(' ');

        addText(timestamp + " " + frameFile);
        addText(decodedString);

        ui->mapzone->setPosition((double)frame.lat * 1e-7, (double)frame.lon * 1e-7);

        if (m_logFile.isOpen()) {
                QTextStream out(&m_logFile);
                out << timestamp << ";" << frameFile << ";"
                    << frame.sts << ";"
                    << frame.lat << ";"
                    << frame.lon << ";"
                    << frame.alt << ";"
                    << frame.pressure << ";"
                    << frame.temp << ";"
                    << frame.accX << ";"
                    << frame.accY << ";"
                    << frame.accZ << ";"
                    << frame.annex0 << ";"
                    << frame.annex1 << "\n";
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
