#include "voicemanager.h"
#include "telemetryframe.h"
#include <QSoundEffect>

VoiceManager::VoiceManager(QObject *parent)
    : QThread(nullptr),
    m_speech(new QTextToSpeech(this)),
    m_enabled(true), // Par défaut activé
    m_previousStatus(static_cast<RocketState_t>(-1)) // Initialiser à un état invalide
{
    m_speech->setRate(-0.10);    // Vitesse normale
    m_speech->setPitch(0.10);   // Tonalité normale
    m_speech->setVolume(1.0);  // Volume maximum

    // Set the English language
    m_speech->setLocale(QLocale(QLocale::English, QLocale::UnitedStates));
    //m_speech->setLocale(QLocale(QLocale::French, QLocale::France));

    qDebug() << "[" << QDateTime::currentDateTime().toString("dd-MM-yyyy_HH.mm.ss") << "][VOICEMANAGER] " << QThread::currentThread();
}

VoiceManager::~VoiceManager(){
    quit();  // Signale au thread de s'arrêter
    wait();  // Attend que le thread termine
    delete m_speech;
}

void VoiceManager::run(){
    exec();  // démarrage de la boucle d'événements
}

void VoiceManager::updateStatus(uint8_t newStatus){
    // Lire uniquement si le statut a changé
    if (newStatus == m_previousStatus || !m_enabled)
        return;
    QString voiceMessage;

    // Déterminer le message en fonction du statut
    switch (newStatus) {
    case PRE_FLIGHT:
        voiceMessage = " The rocket pre-flight.";
        break;
    case PYRO_RDY:
        voiceMessage = " Pyrotechnics ready to go.";
        break;
    case ASCEND:
        voiceMessage = " Lift off, we have a Lift off.";
        break;
    case DEPLOY_ALGO:
        voiceMessage = " Algorithm-based deployment.";
        break;
    case DEPLOY_TIMER:
        voiceMessage = " Timer-based deployment.";
        break;
    case DESCEND:
        voiceMessage = " Rocket in ballistic phase.";
        break;
    case TOUCHDOWN:
        voiceMessage = " The rocket has landed.";
        break;
    case LOST:
        voiceMessage = " Signal lost.";
        break;
    default:
        voiceMessage = " Unknown status.";
    }

    // Lire le message si disponible
    if (!voiceMessage.isEmpty()) {
        m_speech->say(voiceMessage);
    }

    // Mettre à jour le statut précédent
    m_previousStatus = newStatus;

}

void VoiceManager::setEnabled(bool enabled){
    m_enabled = !enabled;
}

bool VoiceManager::isEnabled() const{
    return m_enabled;
}

void VoiceManager::emitBeep(){
    // Jouer un bip sonore
    //QSound::play(":/song/bip/sound/bleep-126625.wav");
    //play(":/song/bip/sound/beep-21.wav");
}
