#include <QFile>
#include <QTextStream>

#ifndef MAPZONE_H
#define MAPZONE_H

#include <QWidget>
#include <QThread>
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

    //void settingsUpdate(Settings settingsMap);

    explicit MapZone(QWidget *parent = nullptr);
    ~MapZone();

    void setPosition(double x, double y);
public slots :
    void settingsUpdate(MapZone::Settings settings);

private:
    Ui::MapZone *ui;
    Settings *m_settingsMap;
    QWebEngineView *view;


};

#endif // MAPZONE_H
