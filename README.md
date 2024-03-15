Station Météo Arduino

Ce projet consiste en la création d'une station météo utilisant une carte Arduino et plusieurs capteurs pour mesurer divers paramètres météorologiques. Le code est écrit en C++ pour Arduino.

Composants Utilisés:

Arduino MEGA: La carte principale qui contrôle les capteurs et les affichages.

Capteur de Température et d'Humidité DHT22: Utilisé pour mesurer la température et l'humidité de l'air.

Capteur de Pression Atmosphérique BMP180: Pour mesurer la pression atmosphérique.

Module DS3231 pour l'heure en temps réel: Permet d'obtenir l'heure actuelle.

Quatre boutons poussoirs: Pour naviguer entre différentes pages d'affichage sur l'écran LCD.

Écran LCD 16x2: Pour afficher les données météorologiques et l'heure.

Fonctionnalités:

Mesure de la Température et de l'Humidité: Le capteur DHT22 est utilisé pour mesurer la température et l'humidité de l'air.

Mesure de la Pression Atmosphérique: Le capteur BMP180 mesure la pression atmosphérique pour estimer les conditions météorologiques.

Obtention de l'Heure Actuelle: Le module DS3231 fournit l'heure actuelle en temps réel.

Navigation à l'Aide des Boutons: Les boutons poussoirs permettent de changer de page sur l'écran LCD pour afficher différentes informations météorologiques.

Affichage sur l'Écran LCD: Toutes les données météorologiques ainsi que l'heure sont affichées sur l'écran LCD 16x2 pour une lecture facile.

Schéma de Connexion:

DHT22 : 6
BMP180 : SDA/SCL
DS3231 : SDA/SCL
LCD : 
	vss -
	vdd +
	v0 potentiomètre
	rs : 3
	rw : -
	e : 4
	d0
	d1
	d2
	d3
	d4 : 5
	d5 : 6
	d6 : 7
	d7 : 8
	a : 9
	k : -
 
Boutons : 10;11;12;13

Installation:

Assurez-vous d'avoir l'IDE Arduino installé sur votre ordinateur.
Téléchargez les bibliothèques nécessaires pour les capteurs utilisés : DHT, Adafruit_BMP085_Unified, Adafruit_Sensor, et DS3231.
Téléchargez le code source de ce projet.
Connectez votre Arduino à votre ordinateur à l'aide d'un câble USB.
Chargez le code dans l'IDE Arduino et téléversez-le sur votre Arduino.

Utilisation:

Une fois le code téléversé avec succès sur votre Arduino et les composants correctement connectés, vous devriez voir les données météorologiques et l'heure actuelle s'afficher sur l'écran LCD. Utilisez les boutons poussoirs pour naviguer entre les différentes pages d'affichage et observer les variations de température, d'humidité, de pression atmosphérique et l'heure en temps réel.

Contributions:

Les contributions sous forme de rapports de bugs, d'améliorations de fonctionnalités ou de suggestions sont les bienvenues. N'hésitez pas à ouvrir une issue ou à soumettre une demande de fusion (pull request) sur le dépôt GitHub du projet.


Auteur:

Ce projet a été développé par BitMinor.
