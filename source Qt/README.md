# Interface QT :
*Version 1.3 de l'Interface, creathor Paul Miailhe*

## Classe SerialPort

La classe SerialPort hérite de QThread. Elle permet d'acquérir les différents messages reçus par le module LORA32 de manière ininterrompue, et les transmet à la classe MainWindow. Pour assurer une stabilité dans la liaison série, un CRC8 a été mis en place. Les messages ont un nombre d'octets prédéfini, ce qui permet d'améliorer la robustesse de la liaison.

## Classe MainWindow

La classe MainWindow permet de contrôler les interactions avec l'utilisateur. Elle possède une barre de menu qui permet la connexion série et a une sous-fenêtre permettant la configuration de la liaison série.

La classe contient deux widgets :
- La console, qui permet d'afficher les logs
- Un QWebEngine, qui permet d'ouvrir une carte OpenStreetMap et d'afficher les points GPS en utilisant la bibliothèque JavaScript Leaflet.
