#ifndef MAPSETTINGSDIALOG_H
#define MAPSETTINGSDIALOG_H

#include <QDialog>

#include "MapZone.h"

namespace Ui {
class MapSettingsDialog;
}

class MapSettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MapSettingsDialog(QWidget *parent = nullptr);
    ~MapSettingsDialog();

private slots:

    void on_mapSourceBox_currentIndexChanged(int index);

    void on_applyPushButton_clicked();

signals:
    void applyParameters();

private:
    Ui::MapSettingsDialog *ui;

    MapZone::Settings m_CurrentMapSettings;
};

#endif // MAPSETTINGSDIALOG_H
