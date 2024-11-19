#ifndef TIMEZONE_H
#define TIMEZONE_H

#include <QWidget>
#include <QTimer>
#include <QDateTime>

#include <QDebug>

namespace
{
    constexpr int TICKER_PERIOD_MS = 200;
}

namespace Ui {
class TimeZone;
}

class TimeZone : public QWidget
{
    Q_OBJECT

public:
    explicit TimeZone(QWidget *parent = nullptr);
    ~TimeZone();

    void startChrono();
    void stopChrono();
    void resetChrono();
    int getChrono_ms();
    void setFlightStatus(uint8_t flightStatus);
    void resetWatchdog();
    void handleLostSignal();
    bool isChronoRunning() const;

signals:
    void watchdogTimeout();

private:

    Ui::TimeZone *ui;
    QTimer *m_ticker;
    QTimer *m_watchdog;

    int m_chrono=0;
    bool m_isChronoActivated = false;

    void refreshHour();
    void refreshChrono();
};

#endif // TIMEZONE_H
