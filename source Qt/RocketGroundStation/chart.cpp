#include "mainwindow.h"
#include "ui_mainwindow.h"

/* Chart management function */

void MainWindow::initGraphicAcc() {
    QChart *chart = ui->graphicsViewAcceleration->chart();
    if (!chart) {
        qDebug() << "Erreur : Le graphique (chart) n'est pas initialisé.";
        return;
    }

    chart->legend()->setVisible(true);
    chart->setAnimationOptions(QChart::AllAnimations);

    // --- Séries Accélération ---
    m_seriesAccX = new QLineSeries();
    m_seriesAccX->setName("Acceleration X");
    chart->addSeries(m_seriesAccX);

    m_seriesAccY = new QLineSeries();
    m_seriesAccY->setName("Acceleration Y");
    chart->addSeries(m_seriesAccY);

    m_seriesAccZ = new QLineSeries();
    m_seriesAccZ->setName("Acceleration Z");
    chart->addSeries(m_seriesAccZ);

    // --- Séries Gyroscope ---
    m_seriesGyroX = new QLineSeries();
    m_seriesGyroX->setName("Gyro X");
    chart->addSeries(m_seriesGyroX);

    m_seriesGyroY = new QLineSeries();
    m_seriesGyroY->setName("Gyro Y");
    chart->addSeries(m_seriesGyroY);

    m_seriesGyroZ = new QLineSeries();
    m_seriesGyroZ->setName("Gyro Z");
    chart->addSeries(m_seriesGyroZ);

    // --- Axe X Temps ---
    m_axisAccX = new QDateTimeAxis();
    m_axisAccX->setTickCount(15);
    m_axisAccX->setFormat("hh:mm:ss");
    m_axisAccX->setTitleText("Time");
    chart->addAxis(m_axisAccX, Qt::AlignBottom);

    // Lier toutes les séries à l'axe X
    m_seriesAccX->attachAxis(m_axisAccX);
    m_seriesAccY->attachAxis(m_axisAccX);
    m_seriesAccZ->attachAxis(m_axisAccX);
    m_seriesGyroX->attachAxis(m_axisAccX);
    m_seriesGyroY->attachAxis(m_axisAccX);
    m_seriesGyroZ->attachAxis(m_axisAccX);

    // --- Axe Y gauche pour l'accélération ---
    m_axisAccY = new QValueAxis();
    m_axisAccY->setLabelFormat("%.2f");
    m_axisAccY->setTitleText("Acceleration (g)");
    m_axisAccY->setRange(-16, 16);
    chart->addAxis(m_axisAccY, Qt::AlignLeft);

    m_seriesAccX->attachAxis(m_axisAccY);
    m_seriesAccY->attachAxis(m_axisAccY);
    m_seriesAccZ->attachAxis(m_axisAccY);

    // --- Axe Y droit pour le gyroscope ---
    m_axisGyroY = new QValueAxis();
    m_axisGyroY->setLabelFormat("%.2f");
    m_axisGyroY->setTitleText("Gyro (°/s)");
    m_axisGyroY->setRange(-500, 500); // Adapte selon les valeurs de tes gyros
    chart->addAxis(m_axisGyroY, Qt::AlignRight);

    m_seriesGyroX->attachAxis(m_axisGyroY);
    m_seriesGyroY->attachAxis(m_axisGyroY);
    m_seriesGyroZ->attachAxis(m_axisGyroY);

    // --- Limites initiales de l’axe X ---
    QDateTime now = QDateTime::currentDateTime();
    m_axisAccX->setMin(now);
    m_axisAccX->setMax(now.addSecs(120));

    // --- Couleurs ---
    m_seriesAccX->setPen(QPen(Qt::red, 2));
    m_seriesAccY->setPen(QPen(Qt::green, 2));
    m_seriesAccZ->setPen(QPen(Qt::blue, 2));

    m_seriesGyroX->setPen(QPen(Qt::cyan, 2));
    m_seriesGyroY->setPen(QPen(Qt::magenta, 2));
    m_seriesGyroZ->setPen(QPen(Qt::yellow, 2));

    // Appliquer un style sombre au fond du graphique et de la zone de dessin
    applyChartTheme(chart, m_isDarkMode);

}

void MainWindow::addAccelerationsAndScroll(float accX, float accY, float accZ,
                                           float gyroX, float gyroY, float gyroZ) {
    // Obtenir le temps actuel
    qint64 currentTime = QDateTime::currentDateTime().toMSecsSinceEpoch();

    // Ajouter les données aux séries d'accélération
    m_seriesAccX->append(currentTime, accX);
    m_seriesAccY->append(currentTime, accY);
    m_seriesAccZ->append(currentTime, accZ);

    // Ajouter les données aux séries de gyroscope
    m_seriesGyroX->append(currentTime, gyroX);
    m_seriesGyroY->append(currentTime, gyroY);
    m_seriesGyroZ->append(currentTime, gyroZ);

    // Vérifier si le temps actuel dépasse la limite de l'axe X
    if (QDateTime::currentDateTime() >= m_axisAccX->max()) {
        ui->graphicsViewAcceleration->chart()->scroll(120, 0); // Défilement horizontal
    }

    // Limiter le nombre de points pour éviter surcharge mémoire
    const int maxPoints = 1000;
    if (m_seriesAccX->count() > maxPoints) {
        m_seriesAccX->remove(0);
        m_seriesAccY->remove(0);
        m_seriesAccZ->remove(0);

        m_seriesGyroX->remove(0);
        m_seriesGyroY->remove(0);
        m_seriesGyroZ->remove(0);
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

    QPen penAltitudeBaro(Qt::yellow);
    penAltitudeBaro.setWidth(2);
    m_seriesAltitudeBaro->setPen(penAltitudeBaro);

    // Appliquer un style sombre au fond du graphique et de la zone de dessin
    applyChartTheme(chart, m_isDarkMode);
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

void MainWindow::applyChartTheme(QChart *chart, bool darkMode) {
    if (!chart) return;

    if (darkMode) {
        chart->setBackgroundBrush(QBrush(Qt::black));
        chart->setPlotAreaBackgroundBrush(QBrush(QColor(30, 30, 30)));
        chart->setPlotAreaBackgroundVisible(true);
        chart->legend()->setLabelColor(Qt::white);

        for (QAbstractAxis *axis : chart->axes()) {
            QPen axisPen(Qt::white);
            axis->setLinePen(axisPen);
            axis->setLabelsBrush(QBrush(Qt::white));
            axis->setTitleBrush(QBrush(Qt::white));
        }
    } else {
        chart->setBackgroundBrush(QBrush(Qt::white));
        chart->setPlotAreaBackgroundBrush(QBrush(Qt::white));
        chart->setPlotAreaBackgroundVisible(true);
        chart->legend()->setLabelColor(Qt::black);

        for (QAbstractAxis *axis : chart->axes()) {
            QPen axisPen(Qt::black);
            axis->setLinePen(axisPen);
            axis->setLabelsBrush(QBrush(Qt::black));
            axis->setTitleBrush(QBrush(Qt::black));
        }
    }
}
