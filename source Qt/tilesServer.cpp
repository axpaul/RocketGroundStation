#include "TilesServer.h"
#include <QFile>
#include <QFileInfo>
#include <QUrl>
#include <QDir>
#include <QDebug>

TilesServer::TilesServer(QObject *parent) : QTcpServer(parent) {}

bool TilesServer::startServer(quint16 port) {
    if (!this->listen(QHostAddress::LocalHost, port)) {
        qCritical() << "Le serveur n'a pas pu démarrer!";
        return false;
    } else {
        qDebug() << "Serveur démarré sur http://localhost:" << port;
        return true;
    }
}

void TilesServer::incomingConnection(qintptr socketDescriptor) {
    QTcpSocket *socket = new QTcpSocket(this);
    connect(socket, &QTcpSocket::disconnected, socket, &QTcpSocket::deleteLater);
    socket->setSocketDescriptor(socketDescriptor);

    connect(socket, &QTcpSocket::readyRead, this, [this, socket]() {
        handleRequest(socket);
    });
}

void TilesServer::handleRequest(QTcpSocket *socket) {
    if (socket->canReadLine()) {
        QByteArray request = socket->readLine();
        QString path = QString::fromUtf8(request).split(' ')[1];
        path = QUrl::fromPercentEncoding(path.toUtf8());
        sendResponse(socket, path);
    }
}

void TilesServer::sendResponse(QTcpSocket *socket, const QString &path) {
    // Construit le chemin du fichier basé sur le dossier local ./map/
    QString resourcePath = QDir::currentPath() + "/map" + path;

    QFile file(resourcePath);
    if (file.exists() && file.open(QIODevice::ReadOnly)) {
        QByteArray response = "HTTP/1.1 200 OK\r\n";
        response += "Content-Type: image/png\r\n";
        response += "Content-Length: " + QByteArray::number(file.size()) + "\r\n";
        response += "\r\n";
        response += file.readAll();
        socket->write(response);
    } else {
        QByteArray response = "HTTP/1.1 404 Not Found\r\n";
        response += "Content-Type: text/plain\r\n";
        response += "\r\nTile not found.";
        socket->write(response);
    }
    socket->disconnectFromHost();
}
