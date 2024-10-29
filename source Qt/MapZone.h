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
    explicit MapZone(QWidget *parent = nullptr);
    ~MapZone();

    void setPosition(double x, double y);

private:
    Ui::MapZone *ui;

    QWebEngineView *view;


};

#endif // MAPZONE_H
