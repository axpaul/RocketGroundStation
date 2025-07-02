#include "timeZone.h"
#include "ui_timeZone.h"
#include "telemetryframe.h"

TimeZone::TimeZone(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TimeZone),
    m_ticker(new QTimer),
    m_watchdog(new QTimer)
{
    ui->setupUi(this);

    connect(m_ticker, &QTimer::timeout, this, &TimeZone::refreshHour);
    m_ticker->start(TICKER_PERIOD_MS);

    connect(m_watchdog, &QTimer::timeout, this, &TimeZone::handleLostSignal);

    m_watchdog->setInterval(2000);  // 2 secondes
    m_watchdog->setSingleShot(true);  // Le watchdog sera réinitialisé manuellement
}

TimeZone::~TimeZone(){
    delete ui;
}

void TimeZone::startChrono(){
    m_isChronoActivated = true;
    m_watchdog->start();  // Lancer le watchdog en même temps que le chrono
}
void TimeZone::stopChrono(){
    m_isChronoActivated = false;
    m_watchdog->stop();  // Arrêter le watchdog
}

void TimeZone::resetChrono(){
    m_chrono = 0;
    refreshChrono();
}

int TimeZone::getChrono_ms(){
    return m_chrono;
}

bool TimeZone::isChronoRunning() const{
    return m_isChronoActivated;
}

void TimeZone::refreshHour(){
    QDateTime TodayDate = QDateTime::currentDateTime();
    ui->hour_lbl->setText(TodayDate.toString("hh:mm:ss"));

    if (m_isChronoActivated)
    {
        m_chrono += TICKER_PERIOD_MS;
        refreshChrono();
    }
}

void TimeZone::refreshChrono(){
    QTime time(0, 0, 0, 0);
    time = time.addMSecs(m_chrono);

    // Affichage des minutes, secondes et millisecondes
    ui->time_lbl->setText(time.toString("mm:ss.zz"));  // Format "mm:ss.zz" pour inclure les millisecondes
}

void TimeZone::resetWatchdog(){
    if (m_watchdog->isActive()) {
        m_watchdog->stop();  // Arrêter le watchdog si déjà actif
    }
    m_watchdog->start();  // Relancer le watchdog
}

void TimeZone::handleLostSignal(){
    // Stop chrono
    stopChrono();

    // Émettre le signal pour notifier MainWindow
    emit watchdogTimeout();

    // Optionnel : Appliquer un style spécifique
    ui->time_lbl->setStyleSheet("color: red;");
}


void TimeZone::setFlightStatus(uint8_t flightStatus){
    QString colorStyle;

    // Définir les couleurs en fonction du statut
    switch (flightStatus) {
    case PRE_FLIGHT:
        colorStyle = "color: green;";
        break;
    case PYRO_RDY:
        colorStyle = "color: red;";
        break;
    case ASCEND:
        colorStyle = "color: cyan;";
        break;
    case DEPLOY_ALGO:
        colorStyle = "color: orange;";
        break;
    case DEPLOY_TIMER:
        colorStyle = "color: purple;";
        break;
    case DESCEND:
        colorStyle = "color: brown;";
        break;
    case TOUCHDOWN:
        colorStyle = "color: gray;";
        break;
    default:
        colorStyle = "color: black;";
        break;
    }

    // Appliquer le style de couleur au chrono
    ui->time_lbl->setStyleSheet(colorStyle);
}


