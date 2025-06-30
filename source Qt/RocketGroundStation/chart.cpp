#include "mainwindow.h"
#include "ui_mainwindow.h"

/* Chart management function */

void MainWindow::initGraphicAcc() {
    // Obtenir le QChart déjà associé au QChartView
    QChart *chart = ui->graphicsViewAcceleration->chart();
    if (!chart) {
        qDebug() << "Erreur : Le graphique (chart) n'est pas initialisé.";
        return; // Assurez-vous que le QChart existe
    }

    // Configurer le titre, légende et animation
    //chart->setTitle("Acceleration graph");
    chart->legend()->setVisible(true); // Affiche la légende pour identifier les séries
    chart->setAnimationOptions(QChart::AllAnimations);

    // Créer les séries pour accX, accY et accZ
    m_seriesAccX = new QLineSeries();
    m_seriesAccX->setName("Acceleration X");
    chart->addSeries(m_seriesAccX);

    m_seriesAccY = new QLineSeries();
    m_seriesAccY->setName("Acceleration Y");
    chart->addSeries(m_seriesAccY);

    m_seriesAccZ = new QLineSeries();
    m_seriesAccZ->setName("Acceleration Z");
    chart->addSeries(m_seriesAccZ);

    // Configurer l'axe X (temps)
    m_axisAccX = new QDateTimeAxis();
    m_axisAccX->setTickCount(15);
    m_axisAccX->setFormat("hh:mm:ss");
    m_axisAccX->setTitleText("Time");
    chart->addAxis(m_axisAccX, Qt::AlignBottom);

    // Attacher les séries à l'axe X
    m_seriesAccX->attachAxis(m_axisAccX);
    m_seriesAccY->attachAxis(m_axisAccX);
    m_seriesAccZ->attachAxis(m_axisAccX);

    // Configurer l'axe Y (accélération)
    m_axisAccY = new QValueAxis();
    m_axisAccY->setLabelFormat("%.2f");
    m_axisAccY->setTitleText("Acceleration (g)");
    m_axisAccY->setRange(-16, 16); // Ajustez selon vos besoins
    chart->addAxis(m_axisAccY, Qt::AlignLeft);

    // Attacher les séries à l'axe Y
    m_seriesAccX->attachAxis(m_axisAccY);
    m_seriesAccY->attachAxis(m_axisAccY);
    m_seriesAccZ->attachAxis(m_axisAccY);

    // Définir les limites initiales de l'axe X
    m_axisAccX->setMin(QDateTime::currentDateTime().addSecs(0)); // Afficher les 30 dernières secondes
    m_axisAccX->setMax(QDateTime::currentDateTime().addSecs(120));

    QPen penX(Qt::red);
    penX.setWidth(2);
    m_seriesAccX->setPen(penX);

    QPen penY(Qt::green);
    penY.setWidth(2);
    m_seriesAccY->setPen(penY);

    QPen penZ(Qt::blue);
    penZ.setWidth(2);
    m_seriesAccZ->setPen(penZ);
}

void MainWindow::addAccelerationsAndScroll(float accX, float accY, float accZ) {
    // Obtenir le temps actuel
    qint64 currentTime = QDateTime::currentDateTime().toMSecsSinceEpoch();

    // Ajouter les données aux séries respectives
    m_seriesAccX->append(currentTime, accX);
    m_seriesAccY->append(currentTime, accY);
    m_seriesAccZ->append(currentTime, accZ);

    // Vérifier si le temps actuel dépasse la limite de l'axe X
    if (QDateTime::currentDateTime() >= m_axisAccX->max()) {
        ui->graphicsViewAcceleration->chart()->scroll(120, 0); // Faire défiler le graphique vers la droite
    }

    // Limiter le nombre de points dans chaque série pour éviter une surcharge
    if (m_seriesAccX->count() > 1000) {
        m_seriesAccX->remove(0);
        m_seriesAccY->remove(0);
        m_seriesAccZ->remove(0);
    }
}

void MainWindow::initGraphicPressureAltitude() {
    // Obtenir le QChart associé au QChartView pour la pression et l'altitude
    QChart *chart = ui->graphicsViewAltitude->chart();
    if (!chart) {
        qDebug() << "Erreur : Le graphique (chart) n'est pas initialisé.";
        return; // Vérifiez que le QChart existe
    }

    // Configurer le titre, légende et animation
    //chart->setTitle("Pressure and Altitude graph");
    chart->legend()->setVisible(true); // Affiche la légende pour identifier les séries
    chart->setAnimationOptions(QChart::AllAnimations);

    // Créer les séries pour pression et altitude
    m_seriesPressure = new QLineSeries();
    m_seriesPressure->setName("Pressure (hPa)");
    chart->addSeries(m_seriesPressure);

    m_seriesAltitudeGNSS = new QLineSeries();
    m_seriesAltitudeGNSS->setName("GNSS altitude (m)");
    chart->addSeries(m_seriesAltitudeGNSS);

    m_seriesAltitudeBaro = new QLineSeries();
    m_seriesAltitudeBaro ->setName("Baro altitude (m)");
    chart->addSeries(m_seriesAltitudeBaro);

    // Configurer l'axe X (temps)
    m_axisPressX = new QDateTimeAxis();
    m_axisPressX->setTickCount(15);
    m_axisPressX->setFormat("hh:mm:ss");
    m_axisPressX->setTitleText("Time");
    chart->addAxis(m_axisPressX, Qt::AlignBottom);

    // Attacher les séries à l'axe X
    m_seriesPressure->attachAxis(m_axisPressX);
    m_seriesAltitudeGNSS->attachAxis(m_axisPressX);
    m_seriesAltitudeBaro->attachAxis(m_axisPressX);

    // Configurer l'axe Y (pression)
    m_axisPressY = new QValueAxis();
    m_axisPressY->setLabelFormat("%.2f");
    m_axisPressY->setTitleText("Pressure (hP)");
    m_axisPressY->setRange(500, 1100);
    chart->addAxis(m_axisPressY, Qt::AlignLeft);

    // Configurer l'axe Y (altitude)
    m_axisAltY = new QValueAxis();
    m_axisAltY->setLabelFormat("%.2f");
    m_axisAltY->setTitleText("Altitude (m)");
    m_axisAltY->setRange(0, 5000);
    chart->addAxis(m_axisAltY, Qt::AlignRight);

    // Attacher les séries à l'axe Y
    m_seriesPressure->attachAxis(m_axisPressY);
    m_seriesAltitudeGNSS->attachAxis(m_axisAltY);
    m_seriesAltitudeBaro->attachAxis(m_axisAltY);

    // Définir les limites initiales de l'axe X
    m_axisPressX->setMin(QDateTime::currentDateTime().addSecs(0));
    m_axisPressX->setMax(QDateTime::currentDateTime().addSecs(120));

    // Personnaliser les couleurs des séries
    QPen penPressure(Qt::magenta);
    penPressure.setWidth(2);
    m_seriesPressure->setPen(penPressure);

    QPen penAltitudeGNSS(Qt::cyan);
    penAltitudeGNSS.setWidth(2);
    m_seriesAltitudeGNSS->setPen(penAltitudeGNSS);

    QPen penAltitudeBaro(Qt::gray);
    penAltitudeBaro.setWidth(2);
    m_seriesAltitudeGNSS->setPen(penAltitudeBaro);
}

void MainWindow::addPressureAltitudeAndScroll(float pressure, float altitudeGNSS, float altitudeBaro) {
    // Obtenir le temps actuel
    qint64 currentTime = QDateTime::currentDateTime().toMSecsSinceEpoch();

    // Ajouter les données aux séries respectives
    m_seriesPressure->append(currentTime, pressure);
    m_seriesAltitudeGNSS->append(currentTime, altitudeGNSS);
    m_seriesAltitudeBaro->append(currentTime, altitudeBaro);


    // Vérifier si le temps actuel dépasse la limite de l'axe X
    if (QDateTime::currentDateTime() >= m_axisPressX->max()) {
        ui->graphicsViewAltitude->chart()->scroll(120, 0); // Faire défiler le graphique vers la droite
    }

    // Limiter le nombre de points dans chaque série
    while (m_seriesPressure->count() > 1000) {
        m_seriesPressure->remove(0);
        m_seriesAltitudeGNSS->remove(0);
    }
}
