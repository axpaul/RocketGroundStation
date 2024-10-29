#ifndef TILESSERVER_HPP
#define TILESSERVER_HPP

#include <QTcpServer>
#include <QTcpSocket>
#include <QObject>

class TilesServer : public QTcpServer {
    Q_OBJECT

public:
    explicit TilesServer(QObject *parent = nullptr);
    bool startServer(quint16 port = 8080);

protected:
    void incomingConnection(qintptr socketDescriptor) override;

private slots:
    void handleRequest(QTcpSocket *socket);

private:
    void sendResponse(QTcpSocket *socket, const QString &path);
};

#endif // TILESSERVER_HPP
