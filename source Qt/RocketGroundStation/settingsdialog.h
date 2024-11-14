#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QSerialPort>
#include "serialport.h"
#include "MapZone.h"

QT_BEGIN_NAMESPACE

namespace Ui
{
class SettingsDialog;
}

class QIntValidator;

QT_END_NAMESPACE

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(QWidget *parent = nullptr);
    ~SettingsDialog();
    MapZone::Settings mapSettings() const;
    SerialPort::Settings settings() const;

signals:
    void applyParameter();

private slots:
    void showPortInfo(int idx);
    void apply();
    void checkCustomBaudRatePolicy(int idx);
    void checkCustomDevicePathPolicy(int idx);

public slots:
    void showSetting();

private:
    void fillMapParameters();
    void fillPortsParameters();
    void fillPortsInfo();
    void updateSettings();

private:
    Ui::SettingsDialog *m_ui = nullptr;
    SerialPort::Settings m_currentSettings;
    MapZone::Settings m_currentMapSettings;
    QIntValidator *m_intValidator = nullptr;
};

#endif // SETTINGSDIALOG_H
