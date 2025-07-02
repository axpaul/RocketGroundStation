# RocketGroundStation

🎯 **RocketGroundStation** est une application Qt/C++ permettant de visualiser en temps réel les données de télémétrie d'une fusée expérimentale via une liaison série. L'interface offre une lecture claire des paramètres GNSS, barométriques, d'accélération et de gyroscope, accompagnée de graphiques dynamiques.

![Demo](https://github.com/axpaul/RocketGroundStation/blob/main/video%20Qt/Test%20V%C2%B02.gif)

---

## 🎥 Aperçu vidéo

[![YouTube](https://img.shields.io/badge/Vidéo%20Demo-YouTube-red?logo=youtube)](https://youtu.be/wsO3MDKmM1w)

---

## 🚀 Fonctionnalités principales

* Connexion série en temps réel
* Vérification CRC8 des trames
* Affichage graphique de :

  * Accélérations (X, Y, Z)
  * Gyroscopes (X, Y, Z)
  * Pression (hPa)
  * Altitude GNSS / barométrique
* Statuts dynamiques (GNSS Fix, CRC, Flight Status)
* Compatible avec le mode clair et sombre
* Log CSV automatique des données reçues

---

## 📡 Format de trame

Chaque trame est transmise sous forme binaire sur le port série et suit la structure suivante :

```
+------------+------------+---------------------+--------------+
| Start Byte |   CRC8     |     Payload         |  End Byte    |
|  (0xEE)    |  (1 byte)  |  (NbTrame = 38B)    |   (ignored)  |
+------------+------------+---------------------+--------------+
```

* `Start Byte` : toujours 0xEE
* `CRC` : calculé sur le `Payload` uniquement
* `Payload` (taille fixée à 38 octets) contient :

  * Latitude (float)
  * Longitude (float)
  * Altitude GNSS (int32\_t)
  * Altitude baro (float)
  * Pression (float)
  * Température (float)
  * Acc X/Y/Z (float x3)
  * Gyro X/Y/Z (float x3)
  * GNSS Fix (uint8\_t)
  * GNSS Fix Type (uint8\_t)
  * Flight Status (uint8\_t)

Le CRC est validé via :

```cpp
uint8_t receivedCrc = frame.at(1);
QByteArray receivedData = frame.mid(2, NbTrame);
uint8_t calculatedCrc = calculate_crc8(receivedData);
```
---

## 🔧 Configuration

L'application Qt utilise les composants suivants :

* `QSerialPort` pour la communication série
* `QChart` (QtCharts) pour l'affichage des courbes
* `QDateTimeAxis` pour les axes X en temps réel
* Gestion automatique du thème sombre ou clair selon le système

---

## 📂 Enregistrement des données

Un fichier CSV est généré automatiquement au lancement dans un dossier dédié :

```csv
Timestamp;Frame;Sts;Lat;Lon;Altitude;Pressure;Temperature;Acceleration X;Acceleration Y;Acceleration Z;Gyro X;Gyro Y;Gyro Z
```

---

## 📦 Dépendances

* Qt 5.15+ ou Qt 6
* Module QtCharts

---

## 📘 Licence

Projet open-source sous licence MIT. Utilisable librement dans un cadre éducatif ou amateur.
