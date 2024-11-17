#include "TileServer.h"
#include <QDebug>

// Constructeur
TileServer::TileServer(QObject *parent)
    : QObject(parent), server(nullptr), tilePath(QDir::currentPath() + "/map") {}

// Configure le chemin des tuiles
void TileServer::setTilePath(const QString &path) {
    tilePath = path;
}

// Démarre le serveur
void TileServer::startServer(quint16 port) {
    if (server) {
        qWarning() << "Le serveur est déjà démarré.";
        return;
    }

    server = new QTcpServer(this);

    connect(server, &QTcpServer::newConnection, this, [this]() {
        QTcpSocket *socket = server->nextPendingConnection();
        connect(socket, &QTcpSocket::readyRead, this, [this, socket]() {
            handleRequest(socket);
        });
        connect(socket, &QTcpSocket::disconnected, socket, &QTcpSocket::deleteLater);
    });

    if (!server->listen(QHostAddress::Any, port)) {
        qCritical() << "Erreur : Impossible de démarrer le serveur sur le port" << port;
        return;
    }

    qDebug() << "Serveur de tuiles démarré sur le port" << port;
}

// Arrête le serveur
void TileServer::stopServer() {
    if (server) {
        server->close();
        server->deleteLater();
        server = nullptr;
    }
    qDebug() << "Serveur de tuiles arrêté.";
}

// Gère les requêtes HTTP
void TileServer::handleRequest(QTcpSocket *socket) {
    QByteArray request = socket->readAll();
    QString requestString(request);

    QStringList requestLines = requestString.split("\r\n");
    if (requestLines.isEmpty()) {
        socket->disconnectFromHost();
        return;
    }

    QString firstLine = requestLines.first();
    QStringList requestParts = firstLine.split(" ");
    if (requestParts.size() < 2) {
        socket->disconnectFromHost();
        return;
    }

    QString filePath = requestParts[1];
    if (filePath.startsWith("/")) {
        filePath = filePath.mid(1); // Supprimer le premier '/'
    }

    QString fullPath = QDir(tilePath).filePath(filePath);

    QFile file(fullPath);
    if (file.exists() && file.open(QIODevice::ReadOnly)) {
        QByteArray response = "HTTP/1.1 200 OK\r\n";
        response += "Content-Type: image/png\r\n";
        response += "Content-Length: " + QByteArray::number(file.size()) + "\r\n";
        response += "\r\n";
        response += file.readAll();
        socket->write(response);
        socket->flush();
        qDebug() << "Servi:" << fullPath;
    } else {
        QByteArray response = "HTTP/1.1 404 Not Found\r\n";
        response += "Content-Type: text/plain\r\n";
        response += "\r\n";
        response += "Tile not found";
        socket->write(response);
        socket->flush();
        qDebug() << "Erreur 404:" << fullPath << "non trouvé";
    }

    socket->disconnectFromHost();
}
