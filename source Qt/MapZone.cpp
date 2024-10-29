#include "MapZone.h"
#include "ui_MapZone.h"
#include <QUrl>

MapZone::MapZone(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MapZone)
{
    ui->setupUi(this);

    view = new QWebEngineView(this);

    // Chargez le fichier HTML à partir de votre système de fichiers
    //view->load(QUrl::fromLocalFile("C:/Users/paulm/OneDrive/Bureau/InterfaceReadGrounStation-main/Map.html"));  // Changez ceci pour pointer vers votre fichier HTML

    // Chargez le fichier HTML à partir des ressources Qt
    view->load(QUrl(QStringLiteral("qrc:/map.html")));

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
