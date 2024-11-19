#ifndef VOICEMANAGER_H
#define VOICEMANAGER_H

#include <QObject>
#include <QTextToSpeech>
#include "telemetryframe.h"

//typedef enum {PRE_FLIGHT = 0, PYRO_RDY, ASCEND, DEPLOY_ALGO, DEPLOY_TIMER, DESCEND, TOUCHDOWN, LOST} RocketStateVoice_t;

class VoiceManager : public QObject
{
    Q_OBJECT

public:
    explicit VoiceManager(QObject *parent = nullptr);
    ~VoiceManager();

    void updateStatus(uint8_t newStatus); // Met à jour le statut et lit le message
    void setEnabled(bool enabled);        // Activer/désactiver
    bool isEnabled() const;               // Vérifier si activé

private:
    QTextToSpeech *m_speech; // Moteur de synthèse vocale
    bool m_enabled;          // État activé/désactivé
    uint8_t m_previousStatus; // Statut précédent
};

#endif // VOICEMANAGER_H
