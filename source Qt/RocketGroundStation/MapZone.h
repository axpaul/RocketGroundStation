#include <QFile>
#include <QTextStream>

#ifndef MAPZONE_H
#define MAPZONE_H

#include <QWidget>

#include <QDesktopServices>
#include <QtWebEngine>
#include <QWebEngineView>
#include <QUrl>

namespace Ui {
class MapZone;
}

class MapZone : public QWidget
{
    Q_OBJECT

public:

    struct Settings{
        QString location;
        QString mapSource;
    };

    explicit MapZone(QWidget *parent = nullptr);
    ~MapZone();

    void setPosition(double x, double y);
    void MapZone::settingsUpdate(MapZone::Settings settingsMap);

private:
    Ui::MapZone *ui;
    Settings *m_settingsMap;
    QWebEngineView *view;


};

#endif // MAPZONE_H
