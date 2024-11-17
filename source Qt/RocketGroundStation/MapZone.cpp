#include "MapZone.h"
#include "ui_MapZone.h"
#include <QUrl>

MapZone::MapZone(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MapZone),
    m_settingsMap(new Settings)
{
    QString mapSource = "mapTarbes.html";

    ui->setupUi(this);

    view = new QWebEngineView(this);

    // Chargez le fichier HTML à partir de votre système de fichiers
    //view->load(QUrl::fromLocalFile("C:/Users/paulm/OneDrive/Bureau/InterfaceReadGrounStation-main/Map.html"));  // Changez ceci pour pointer vers votre fichier HTML

    // Chargez le fichier HTML à partir des ressources Qt
    //if(QString::compare(m_settingsMap->mapSource, QString("Camp de Ger - Tarbes")))

    view->load(QUrl(mapSource));

    // Vérifier si le chargement a réussi
    connect(view, &QWebEngineView::loadFinished, [this](bool success) {
        if (success) {
            qDebug() << "[" << QDateTime::currentDateTime().toString("dd-MM-yyyy_HH.mm.ss") << "][MAINWINDOW] Le chargement du fichier HTML a réussi";

        } else {
            qDebug() << "[" << QDateTime::currentDateTime().toString("dd-MM-yyyy_HH.mm.ss") << "][MAINWINDOW] Le chargement du fichier HTML a échoué.";
        }
    });
    // Lier la fonction setPosition au contexte JavaScript de la page
    connect(view->page(), &QWebEnginePage::loadFinished, [this]() {
        view->page()->runJavaScript("function setPosition(x, y) { addMarker(x, y); }");
    });

    ui->map_layout->addWidget(view);
}

MapZone::~MapZone()
{
    view->close();
    delete view; // Supprimer l'instance de QWebEngineView
    delete ui;
}

void MapZone::setPosition(double x, double y)
{
    QString javaScriptCode = QString("setPosition(%1, %2)").arg(x).arg(y);
    view->page()->runJavaScript(javaScriptCode);
}

void MapZone::settingsUpdate(MapZone::Settings settingsMap)
{
    QString mapSource = "qrc:/mapTarbes.html";

    qDebug() << "[" << QDateTime::currentDateTime().toString("dd-MM-yyyy_HH.mm.ss") << "][SERIAL] settingUpdate for Map";
    *m_settingsMap = settingsMap;


    if(m_settingsMap->mapSource == "Offline")
    {
        if(m_settingsMap->location == "Camp de Ger")
        {
            mapSource = "qrc:/mapTarbes.html";
        }else if(m_settingsMap->location == "Bourg-St-Bernard")
        {
            mapSource = "qrc:/mapBourg.html";
        }

    }


    view->load(QUrl(mapSource)); // Remplacez par le chemin réel

    // Connecter au signal pour exécuter JavaScript après le chargement de la page
    connect(view, &QWebEngineView::loadFinished, [this](bool success) {
        if (success) {
            qDebug() << "[" << QDateTime::currentDateTime().toString("dd-MM-yyyy_HH.mm.ss") << "][MAINWINDOW] Le chargement du fichier HTML a réussi";

        } else {
            qDebug() << "[" << QDateTime::currentDateTime().toString("dd-MM-yyyy_HH.mm.ss") << "][MAINWINDOW] Le chargement du fichier HTML a échoué.";
        }
    });
    // Lier la fonction setPosition au contexte JavaScript de la page
    connect(view->page(), &QWebEnginePage::loadFinished, [this]() {
        view->page()->runJavaScript("function setPosition(x, y) { addMarker(x, y); }");
    });

}


