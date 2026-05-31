# PlatformIO Core

[![Core](https://github.com/platformio/platformio-core/workflows/Core/badge.svg)](https://docs.platformio.org/en/latest/core/index.html)
![CI Build for PlatformIO Core](https://github.com/platformio/platformio-core/workflows/Core/badge.svg)

[![Docs](https://github.com/platformio/platformio-core/workflows/Docs/badge.svg)](https://docs.platformio.org?utm_source=github&utm_medium=core)
![CI Build for Docs](https://github.com/platformio/platformio-core/workflows/Docs/badge.svg)

[![Examples](https://github.com/platformio/platformio-core/workflows/Examples/badge.svg)](https://github.com/platformio/platformio-examples)
![CI Build for dev-platform examples](https://github.com/platformio/platformio-core/workflows/Examples/badge.svg)

[![Projects](https://github.com/platformio/platformio-core/workflows/Projects/badge.svg)](https://docs.platformio.org/en/latest/tutorials/index.html#projects)
![CI Build for the Community Projects](https://github.com/platformio/platformio-core/workflows/Projects/badge.svg)

[![PyPI Version](https://img.shields.io/pypi/v/platformio.svg)](https://pypi.python.org/pypi/platformio/)
![Latest Version](https://img.shields.io/pypi/v/platformio.svg)

[![PlatformIO Labs](https://img.shields.io/badge/PlatformIO-Labs-orange.svg)](https://piolabs.com/?utm_source=github&utm_medium=core)

## Quick Links

- [Homepage](https://platformio.org?utm_source=github&utm_medium=core)
- [PlatformIO IDE](https://platformio.org/platformio-ide?utm_source=github&utm_medium=core)
- [Registry](https://registry.platformio.org?utm_source=github&utm_medium=core)
- [Project Examples](https://github.com/platformio/platformio-examples/)
- [Docs](https://docs.platformio.org?utm_source=github&utm_medium=core)
- [Donate](https://platformio.org/donate?utm_source=github&utm_medium=core)
- [Contact Us](https://piolabs.com/?utm_source=github&utm_medium=core)

## Social

- [LinkedIn](https://www.linkedin.com/company/platformio/)
- [Twitter](https://twitter.com/PlatformIO_Org)
- [Facebook](https://www.facebook.com/platformio)
- [Community Forums](https://community.platformio.org?utm_source=github&utm_medium=core)

## Screenshot

[![PlatformIO IDE](https://raw.githubusercontent.com/platformio/platformio-web/develop/app/images/platformio-ide-laptop.png)](https://platformio.org?utm_source=github&utm_medium=core)
    
1 Présentation du projet
   ---
1.1 Contexte
   ---
Une entreprise de jouet souhaite développer une voiture pilotée par smartphone, qui intègre plusieurs fonctionnalités comme l'anticollision, le signalement d'obstacle ……

1.2 Objectifs
---
Concevoir, vérifier et valider l'électronique et la programmation d'une voiture pilotée par smartphone afin de :
- Permettre le pilotage de la voiture à partir d'un joystick virtuel sur un smartphone
- Donner, à ce véhicule, de l’autonomie dans certaines prises de décision (anticollision)
- Fournir des informations visuelles et sonores au pilote


1.3 Description de l’existant
---
La voiture est une voiture Radio Commandée déjà existante au catalogue de l'entreprise. Toute l'électronique existante a été supprimée, tout le reste est conservé (carcasse, roues, moteurs, phares …). Les éléments suivants ont été ajoutés à l'intérieur :
- Un module pont en H de référence LF298 pour alimenter le moteur de direction et le moteur de propulsion dans les 2 sens de marche
- Un buzzer passif pour générer un signal avertisseur d'obstacle ou le klaxon
- Deux capteurs infrarouges Sharp de détection d'obstacle
- 6 accumulateurs AA NiMh de 1.2V et 2300mAh en série ou 1 pack batterie de 2 accumulateurs 18650 Li ion de 3.7V de 2600mAh.

1.4 Critères d’acceptabilité du projet
---
Le chef d'entreprise souhaite qu'à la fin de l'étude, lui soient fournis :
- Un dossier de fabrication de la carte électronique (cahier de fabrication + fichiers GERBER + fichier PROTEUS)
- Un dossier technique (cahier de laboratoire + documentation technique + simulations + schémas électriques, schéma d’implantation…)
- Une démonstration avec un prototype opérationnel
- Une étude de coût unitaire pour une production de 100 exemplaires
- Une étude d'autonomie voiture à l’arrêt

2 Expression des besoins
---
2.1 Besoins fonctionnel
---
- Un prototype
- Un dossier technique (cahier de laboratoire)
- Un dossier technique de fabrication (cahier de fabrication ……)
- Une démonstration du système opérationnel

2.2 Fonctionnement global attendu
---
- La voiture est pilotée par smartphone via liaison Bluetooth grâce à un joystick virtuel
- Des dispositifs de sécurité sont mis en œuvre sur la voiture tels que des capteurs de distance d’obstacle avant et arrière, et un avertisseur sonore en fonction de la proximité de l’obstacle.
- L'avertisseur sonore peut être activé directement comme klaxon grâce à un bouton poussoir virtuel sur le smart-phone. Il est aussi possible de générer une mélodie ou des fréquences fixes.
- Un bargraphe virtuel, sur le téléphone, permet d'informer visuellement l'utilisateur de la proximité de l'obstacle à l'avant et/ou du niveau de batterie. (Le niveau de batterie sera indiqué en permanence sauf si proximité d’obs-tacle à l’avant)
- Détection (Près, Moyen et Loin) respectivement moins de 40cm, entre 40 et 60cm et au-delà
- L’utilisateur peut choisir d’activer, de façon permanente, les phares avant et arrière de la voiture grâce à un switch virtuel sur l’écran du smartphone et au besoin de passer dans un mode de gestion automatique grâce à un capteur de luminosité.
- Les phares arrière s’allument plus fort en cas de freinage.
- La température extérieure est reportée sur le smartphone.

2.3 Fonctionnement détaillé attendu
---
- Deux détecteurs de distance infrarouge (un à l’arrière et l’autre à l’avant) permettent de mesurer la distance à l’obstacle et la vitesse d'approche du véhicule, et d’identifier la zone de proximité
- Un buzzer permet d’émettre des bips plus ou moins rapprochés en fonction de la distance à l’obstacle (2 bips par seconde dans la zone Moyen et 4 bips par seconde dans la zone près). Le klaxon est prioritaire sur les bips.
- Le véhicule réduit automatiquement de 50% la consigne de vitesse dans la zone moyenne lorsque le déplace-ment s’effectue vers l’obstacle.
- Le véhicule freine à fond (inversion de la propulsion à consigne maximale), jusqu’à arrêt du véhicule, lorsque le véhicule s’approche de l’obstacle dans la zone près, et cela quel que soit la consigne.
- Les phares arrière s’allument plus fort à chaque freinage.
- Les phares avant et arrière s’allument de façon permanente si le switch virtuel est dans la position “ON” et automatiquement en cas de faible luminosité si le switch est en position “AUTO”
- ,,,,,,,,,,,

3 Contraintes
---
3.1 Coûts
---
Aucune contrainte de coût n’est imposée. En revanche un calcul de coût unitaire pour une production de 100 cartes est attendu (composants électroniques + circuit imprimé + voiture + accessoires + main d'œuvre).

3.2 Autonomie de fonctionnement
---
La voiture doit avoir une autonomie d'au moins 30 minutes avec une batterie de 6 accumulateurs NiMh de capacité 2300mAh.

3.3 Délais de réalisation
---
2h C / 28h TD / 27h TP / 25h autonomie

4 Déroulement du projet
4.1 Séquencement
   ---
- Partie électronique du projet
- Partie Numérique du projet
- Dossier de conception (cahier de laboratoire + documentation technique + simulations)
- Dossier de fabrication (cahier de fabrication + fichiers GERBER + fichier PROTEUS)
