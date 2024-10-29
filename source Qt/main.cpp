#include "mainwindow.h"
#include "serialport.h"
#include "settingsdialog.h"
#include "tilesserver.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    TilesServer server;
    if (!server.startServer(8080)) {
        return -1; // Échec de démarrage du serveur
    }

    MainWindow w;
    w.show();
    return a.exec();
}

