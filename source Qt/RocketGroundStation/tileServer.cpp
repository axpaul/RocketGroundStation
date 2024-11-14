#include "TileServer.h"
#include <QCoreApplication>
#include <QDebug>

TileServer::TileServer(QObject *parent)
    : QObject(parent), m_pythonProcess(new QProcess(this)) {}

TileServer::~TileServer() {
    stopServer();
}

bool TileServer::startServer() {
    QString pythonExecutable = "python"; // Remplace par le chemin complet de Python si nécessaire
    //QString scriptPath = QCoreApplication::applicationDirPath() + "/rc_gen.py";
    QString scriptPath = "qrc:/rc_gen.py";

    m_pythonProcess->setProgram(pythonExecutable);
    m_pythonProcess->setArguments({scriptPath});
    m_pythonProcess->setProcessChannelMode(QProcess::MergedChannels);

    // Connecte les signaux pour rediriger les sorties du serveur Python vers qCritical
    connect(m_pythonProcess, &QProcess::readyReadStandardOutput, [this]() {
        QString output = m_pythonProcess->readAllStandardOutput();
        qCritical().noquote() << output.trimmed();  // Utiliser qCritical pour afficher les logs
    });

    connect(m_pythonProcess, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, [](int exitCode, QProcess::ExitStatus exitStatus) {
                qCritical() << "Le serveur Python s'est arrêté avec le code" << exitCode;
            });

    // Démarre le processus Python
    m_pythonProcess->start();
    if (!m_pythonProcess->waitForStarted()) {
        qCritical() << "Erreur : Impossible de démarrer le serveur Python";
        return false;
    }

    qCritical() << "Serveur Python de tuiles démarré";
    return true;
}

void TileServer::stopServer() {
    if (m_pythonProcess->state() == QProcess::Running) {
        m_pythonProcess->terminate();
        m_pythonProcess->waitForFinished();
        qCritical() << "Serveur Python de tuiles arrêté";
    }
}
