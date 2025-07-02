#include "mainwindow.h"
#include "ui_mainwindow.h"

/* Dashboard update functions */

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
    QString text = QString("Altitude GNSS : %1 m").arg(altitude);
    ui->altitudeGPSLabel->setFont(QFont("Segoe UI", 10, QFont::Bold));
    ui->altitudeGPSLabel->setText(text);
    ui->altitudeGPSLabel->setStyleSheet(altitude > 10000 ? "color: red;" : "");
}

void MainWindow::updateAltitudeBaro(float altitude) {
    QString text = QString("Altitude baro : %1 m").arg(altitude, 0, 'f', 2);
    ui->altitudeBaroLabel->setFont(QFont("Segoe UI", 10, QFont::Bold));
    ui->altitudeBaroLabel->setText(text);
    ui->altitudeBaroLabel->setStyleSheet(altitude > 10000 ? "color: red;" : "");
}

void MainWindow::updatePressure(float pressure) {
    QString text = QString("Pression : %1 hPa").arg(pressure, 0, 'f', 2);
    ui->pressureLabel->setFont(QFont("Segoe UI", 10, QFont::Bold));
    ui->pressureLabel->setText(text);
    ui->pressureLabel->setStyleSheet((pressure < 300 || pressure > 1200) ? "color: orange;" : "");
}

void MainWindow::updateTemperature(float temperature) {
    QString text = QString("Température : %1 °C").arg(temperature, 0, 'f', 2);
    ui->temperatureLabel->setFont(QFont("Segoe UI", 10, QFont::Bold));
    ui->temperatureLabel->setText(text);
    ui->temperatureLabel->setStyleSheet(temperature > 50 ? "color: red;" : "");
}

void MainWindow::updateAccelerationX(float accX) {
    QString text = QString("Accélération X : %1 g").arg(accX, 0, 'f', 3);
    ui->accXLabel->setFont(QFont("Segoe UI", 10, QFont::Bold));
    ui->accXLabel->setText(text);
}

void MainWindow::updateAccelerationY(float accY) {
    QString text = QString("Accélération Y : %1 g").arg(accY, 0, 'f', 3);
    ui->accYLabel->setFont(QFont("Segoe UI", 10, QFont::Bold));
    ui->accYLabel->setText(text);
}

void MainWindow::updateAccelerationZ(float accZ) {
    QString text = QString("Accélération Z : %1 g").arg(accZ, 0, 'f', 3);
    ui->accZLabel->setFont(QFont("Segoe UI", 10, QFont::Bold));
    ui->accZLabel->setText(text);
}

void MainWindow::updateGyroX(float gyroX) {
    QString text = QString("Gyro X : %1 °/s").arg(gyroX, 0, 'f', 2);
    ui->gyroXLabel->setFont(QFont("Segoe UI", 10, QFont::Bold));
    ui->gyroXLabel->setText(text);
}

void MainWindow::updateGyroY(float gyroY) {
    QString text = QString("Gyro Y : %1 °/s").arg(gyroY, 0, 'f', 2);
    ui->gyroYLabel->setFont(QFont("Segoe UI", 10, QFont::Bold));
    ui->gyroYLabel->setText(text);
}

void MainWindow::updateGyroZ(float gyroZ) {
    QString text = QString("Gyro Z : %1 °/s").arg(gyroZ, 0, 'f', 2);
    ui->gyroZLabel->setFont(QFont("Segoe UI", 10, QFont::Bold));
    ui->gyroZLabel->setText(text);
}

void MainWindow::updateGnssStatus(uint8_t gnssFix, uint8_t gnssFixType) {
    QString statusText;
    QString colorStyle;

    if (gnssFix == 0) {
        statusText = "NO FIX";
        colorStyle = "color: red;";
    } else {
        switch (gnssFixType) {
        case GNSS_2D_FIX:
            statusText = "2D FIX";
            colorStyle = "color: green;";
            break;
        case GNSS_3D_FIX:
            statusText = "3D FIX";
            colorStyle = "color: green;";
            break;
        case GNSS_OTHER_FIX:
            statusText = "OTHER";
            colorStyle = "color: green;";
            break;
        default:
            statusText = "NO FIX";
            colorStyle = "color: orange;";
            break;
        }
    }

    ui->gnssStatusLabel->setFont(QFont("Segoe UI", 10, QFont::Bold));
    ui->gnssStatusLabel->setText("GNSS : " + statusText);
    ui->gnssStatusLabel->setStyleSheet(colorStyle);
}

void MainWindow::updateCrcCheckLabel(bool crcCheck) {
    QString statusText = crcCheck ? "OK" : "ERROR";
    QString colorStyle = crcCheck ? "color: green;" : "color: red;";

    ui->crcCheckLabel->setFont(QFont("Segoe UI", 10, QFont::Bold));
    ui->crcCheckLabel->setText("CRC : " + statusText);
    ui->crcCheckLabel->setStyleSheet(colorStyle);
}

void MainWindow::updateFlightStatus(uint8_t flightStatus) {
    QString statusText;
    QString colorStyle;

    switch (flightStatus) {
    case PRE_FLIGHT:
        statusText = "PREFLIGHT";
        colorStyle = "color: green;";
        ui->chronoWidget->stopChrono();
        ui->chronoWidget->resetChrono();
        break;
    case PYRO_RDY:
        statusText = "ARMED";
        colorStyle = "color: red;";
        ui->chronoWidget->stopChrono();
        ui->chronoWidget->resetChrono();
        break;
    case ASCEND:
        statusText = "ASCENT";
        colorStyle = "color: cyan;";
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
        colorStyle = "";
        break;
    }

    ui->flightStatusLabel->setFont(QFont("Segoe UI", 10, QFont::Bold));
    ui->flightStatusLabel->setText("Flight Statut : " + statusText);
    ui->flightStatusLabel->setStyleSheet(colorStyle);
    ui->chronoWidget->setFlightStatus(flightStatus);

    MainWindow::triggerSpeaker(flightStatus);
}
