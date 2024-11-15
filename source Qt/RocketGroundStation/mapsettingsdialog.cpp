#include "mapsettingsdialog.h"
#include "ui_mapsettingsdialog.h"

MapSettingsDialog::MapSettingsDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::MapSettingsDialog)
{
    ui->setupUi(this);
    ui->mapSourceBox->addItem("Online");
    ui->mapSourceBox->addItem("Offline");
    ui->mapSourceBox->setCurrentIndex(0);

    ui->mapLocationBox->addItem("Camp de Ger");
    ui->mapLocationBox->addItem("Bourg-St-Bernard");
    ui->mapLocationBox->setCurrentIndex(0);
    ui->mapLocationBox->setDisabled(true);

}

MapSettingsDialog::~MapSettingsDialog()
{
    delete ui;
}

void MapSettingsDialog::on_mapSourceBox_currentIndexChanged(int index)
{
    if(index==0)
    {
       ui->mapLocationBox->setDisabled(true);
    }
    else if(index==1)
    {
      ui->mapLocationBox->setDisabled(false);
    }
}


void MapSettingsDialog::on_applyPushButton_clicked()
{

}

