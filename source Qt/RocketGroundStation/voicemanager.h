#ifndef VOICEMANAGER_H
#define VOICEMANAGER_H

#include <QObject>
#include <QTextToSpeech>
#include <QThread>
#include <QMutex>
#include <QtDebug>
#include <QtCore/QDateTime>

#include "telemetryframe.h"

//typedef enum {PRE_FLIGHT = 0, PYRO_RDY, ASCEND, DEPLOY_ALGO, DEPLOY_TIMER, DESCEND, TOUCHDOWN, LOST} RocketStateVoice_t;

class VoiceManager : public QThread
{
    Q_OBJECT

public:
    explicit VoiceManager(QObject *parent = nullptr);
    ~VoiceManager();
      bool isEnabled() const;               // Vérifier si activé

public slots:
    void updateStatus(uint8_t newStatus); // Met à jour le statut et lit le message
    void setEnabled(bool enabled);        // Activer/désactiver
    void emitBeep();

protected:
    void run() override;

private:
    QTextToSpeech *m_speech; // Moteur de synthèse vocale
    bool m_enabled;          // État activé/désactivé
    uint8_t m_previousStatus; // Statut précédent
    QMutex m_mut;
};

#endif // VOICEMANAGER_H
