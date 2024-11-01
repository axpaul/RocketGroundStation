#ifndef TILESERVER_H
#define TILESERVER_H

#include <QObject>
#include <QProcess>

class TileServer : public QObject {
    Q_OBJECT

public:
    explicit TileServer(QObject *parent = nullptr);
    ~TileServer();

    bool startServer();
    void stopServer();

private:
    QProcess *m_pythonProcess;
};

#endif // TILESERVER_H
