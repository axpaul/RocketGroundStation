#ifndef TILESERVER_H
#define TILESERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QFile>
#include <QDir>

class TileServer : public QObject {
    Q_OBJECT

public:
    explicit TileServer(QObject *parent = nullptr);

    // Définit le répertoire contenant les tuiles
    void setTilePath(const QString &path);

public slots:
    void startServer(quint16 port);
    void stopServer();

private:
    QTcpServer *server; // Instance de QTcpServer
    QString tilePath; // Chemin des tuiles

    // Gestion des connexions
    void handleRequest(QTcpSocket *socket);
};

#endif // TILESERVER_H
