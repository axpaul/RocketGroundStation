#include "mainwindow.h"
#include "ui_mainwindow.h"

/* Dashboard update function */

void MainWindow::updateLatitude(float latitude) {
    QString text = QString("Latitude : %1 °").arg(latitude, 0, 'f', 7);
    ui->latitudeLabel->setFont(QFont("Segoe UI", 10, QFont::Bold));
    ui->latitudeLabel->setText(text);
}

void MainWindow::updateLongitude(float longitude) {
    QString text = QString("Longitude : %1 °").arg(longitude, 0, 'f', 7);
    ui->longitudeLabel->setFont(QFont("Segoe UI", 10, QFont::Bold));
    ui->longitudeLabel->setText(text);
}

void MainWindow::updateAltitudeGPS(int altitude) {
    QString text = QString("Altitude gnss : %1 m").arg(altitude);
    ui->altitudeGPSLabel->setFont(QFont("Segoe UI", 10, QFont::Bold));
    ui->altitudeGPSLabel->setText(text);

    // Colorer en rouge si l'altitude dépasse 10 000 m
    if (altitude > 10000) {
        ui->altitudeGPSLabel->setStyleSheet("color: red;");
    } else {
        ui->altitudeGPSLabel->setStyleSheet("color: black;");
    }
}

void MainWindow::updateAltitudeBaro(float altitude) {
    QString text = QString("Altitude baro : %1 m").arg(altitude, 0, 'f', 2);
    ui->altitudeBaroLabel->setFont(QFont("Segoe UI", 10, QFont::Bold));
    ui->altitudeBaroLabel->setText(text);

    // Colorer en rouge si l'altitude dépasse 10 000 m
    if (altitude > 10000) {
        ui->altitudeBaroLabel->setStyleSheet("color: red;");
    } else {
        ui->altitudeBaroLabel->setStyleSheet("color: black;");
    }
}

void MainWindow::updatePressure(float pressure) {
    QString text = QString("Pressure : %1 mBar").arg(pressure, 0, 'f', 2);
    ui->pressureLabel->setFont(QFont("Segoe UI", 10, QFont::Bold));
    ui->pressureLabel->setText(text);

    // Coloration conditionnelle pour des valeurs anormales de pression
    if (pressure < 300 || pressure > 1200) {
        ui->pressureLabel->setStyleSheet("color: orange;");
    } else {
        ui->pressureLabel->setStyleSheet("color: black;");
    }
}
void MainWindow::updateTemperature(float temperature) {
    QString text = QString("Temperature : %1 °C").arg(temperature, 0, 'f', 2);
    ui->temperatureLabel->setFont(QFont("Segoe UI", 10, QFont::Bold));
    ui->temperatureLabel->setText(text);

    // Changer en rouge si la température est critique
    if (temperature > 50) {
        ui->temperatureLabel->setStyleSheet("color: red;");
    } else {
        ui->temperatureLabel->setStyleSheet("color: black;");
    }
}

void MainWindow::updateAccelerationX(float accX) {
    QString text = QString("Acceleration X : %1 g").arg(accX, 0, 'f', 3);
    ui->accXLabel->setFont(QFont("Segoe UI", 10, QFont::Bold));
    ui->accXLabel->setText(text);
}

void MainWindow::updateAccelerationY(float accY) {
    QString text = QString("Acceleration Y : %1 g").arg(accY, 0, 'f', 3);
    ui->accYLabel->setFont(QFont("Segoe UI", 10, QFont::Bold));
    ui->accYLabel->setText(text);
}

void MainWindow::updateAccelerationZ(float accZ) {
    QString text = QString("Acceleration Z : %1 g").arg(accZ, 0, 'f', 3);
    ui->accZLabel->setFont(QFont("Segoe UI", 10, QFont::Bold));
    ui->accZLabel->setText(text);
}

void MainWindow::updateGnssStatus(uint8_t gnssStatus) {

    QString statusText;
    QString colorStyle;

    if (gnssStatus == 0) {
        statusText = "LOST";
        colorStyle = "color: red;";
    } else {
        statusText = "OK";
        colorStyle = "color: green;";
    }

    ui->gnssStatusLabel->setFont(QFont("Segoe UI", 10, QFont::Bold));
    ui->gnssStatusLabel->setText("GNSS : " + statusText);
    ui->gnssStatusLabel->setStyleSheet(colorStyle);
}

void MainWindow::updateCrcCheckLabel(bool crcCheck){
    QString statusText;
    QString colorStyle;

    if (crcCheck) {
        statusText = "OK";
        colorStyle = "color: green;";
    } else {
        statusText = "ERROR";
        colorStyle = "color: red;";
    }

    ui->crcCheckLabel->setText("CRC : " + statusText);
    ui->crcCheckLabel->setFont(QFont("Segoe UI", 10, QFont::Bold));
    ui->crcCheckLabel->setStyleSheet(colorStyle);
}

void MainWindow::updateFlightStatus(uint8_t flightStatus) {
    QString statusText;
    QString colorStyle;

    // Attribuer un texte et une couleur en fonction du statut
    switch (flightStatus) {
    case PRE_FLIGHT:
        statusText = "PREFLIGHT";
        colorStyle = "color: blue;";
        if(ui->chronoWidget->isChronoRunning() || ui->chronoWidget->getChrono_ms() != 0){
            ui->chronoWidget->stopChrono();
            ui->chronoWidget->resetChrono();
        }
        break;
    case PYRO_RDY:
        statusText = "ARMED";
        colorStyle = "color: red;";
        if(ui->chronoWidget->isChronoRunning() || ui->chronoWidget->getChrono_ms() != 0){
            ui->chronoWidget->stopChrono();
            ui->chronoWidget->resetChrono();
        }
        break;
    case ASCEND:
        statusText = "ASCENT";
        colorStyle = "color: green;";
        ui->chronoWidget->startChrono();
        break;
    case DEPLOY_ALGO:
        statusText = "ALGO";
        colorStyle = "color: orange;";
        break;
    case DEPLOY_TIMER:
        statusText = "TIMER";
        colorStyle = "color: purple;";
        break;
    case DESCEND:
        statusText = "BALLISTIC";
        colorStyle = "color: brown;";
        break;
    case TOUCHDOWN:
        statusText = "LANDING";
        colorStyle = "color: gray;";
        ui->chronoWidget->stopChrono();
        break;
    case LOST:
        statusText = "LOST";
        colorStyle = "color: red;";
        break;
    default:
        statusText = "UNKNOWN";
        colorStyle = "color: black;";
    }

    ui->flightStatusLabel->setText("Flight Statut : " + statusText);
    ui->flightStatusLabel->setFont(QFont("Segoe UI", 10, QFont::Bold));
    ui->flightStatusLabel->setStyleSheet(colorStyle);
    ui->chronoWidget->setFlightStatus(flightStatus);

    // Appeler le gestionnaire pour lire la voix si nécessaire
    MainWindow::triggerSpeaker(flightStatus);
}
