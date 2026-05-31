Cahier des charges de la SAE 1 du Semestre 2

\___\____

**SMART CAR**

&nbsp;

&nbsp;

&nbsp;

**1\. Présentation du projet**

_1.1 Contexte_

Une entreprise de jouet souhaite développer une voiture pilotée par smartphone, qui intègre plusieurs fonctionnalités comme l'anticollision, le signalement d'obstacle ……

_1.2 Objectifs_

Concevoir, vérifier et valider l'électronique et la programmation d'une voiture pilotée par smartphone afin de :

- Permettre le pilotage de la voiture à partir d'un joystick virtuel sur un smartphone​
- Donner, à ce véhicule, de l’autonomie dans certaines prises de décision (anticollision)​
- Fournir des informations visuelles et sonores au pilote​

_1.3 Description de l’existant_

La voiture est une voiture Radio Commandée déjà existante au catalogue de l'entreprise. Toute l'électronique existante a été supprimée, tout le reste est conservé (carcasse, roues, moteurs, phares …). Les éléments suivants ont été ajoutés à l'intérieur :

- Un module pont en H de référence LF298 pour alimenter le moteur de direction et le moteur de propulsion dans les 2 sens de marche
- Un buzzer passif pour générer un signal avertisseur d'obstacle ou le klaxon
- Deux capteurs infrarouges Sharp de détection d'obstacle
- 6 accumulateurs AA NiMh de 1.2V et 2300mAh en série ou 1 pack batterie de 2 accumulateurs 18650 Li ion de 3.7V de 2600mAh.

_1.4 Critères d’acceptabilité du projet_

Le chef d'entreprise souhaite qu'à la fin de l'étude, lui soient fournis :

- Un dossier de fabrication de la carte électronique (cahier de fabrication + fichiers GERBER + fichier PROTEUS)
- Un dossier technique (cahier de laboratoire + documentation technique + simulations + schémas électriques, schéma d’implantation…)
- Une démonstration avec un prototype opérationnel
- Une étude de coût unitaire pour une production de 100 exemplaires
- Une étude d'autonomie voiture à l’arrêt

**2\. Expression des besoins**

2.1 Besoins fonctionnel

- Un prototype
- Un dossier technique (cahier de laboratoire)
- Un dossier technique de fabrication (cahier de fabrication ……)
- Une démonstration du système opérationnel

2.2 Fonctionnement global attendu

- La voiture est pilotée par smartphone via liaison Bluetooth grâce à un joystick virtuel ​
- Des dispositifs de sécurité sont mis en œuvre sur la voiture tels que des capteurs de distance d’obstacle avant et arrière, et un avertisseur sonore en fonction de la proximité de l’obstacle. ​
- L'avertisseur sonore peut être activé directement comme klaxon grâce à un bouton poussoir virtuel sur le smartphone. Il est aussi possible de générer une mélodie ou des fréquences fixes.
- Un bargraphe virtuel, sur le téléphone, permet d'informer visuellement l'utilisateur de la proximité de l'obstacle à l'avant et/ou du niveau de batterie. (Le niveau de batterie sera indiqué en permanence sauf si proximité d’obstacle à l’avant)
- Détection (Près, Moyen et Loin) respectivement moins de 40cm, entre 40 et 60cm et au-delà
- L’utilisateur peut choisir d’activer, de façon permanente, les phares avant et arrière de la voiture grâce à un switch virtuel sur l’écran du smartphone et au besoin de passer dans un mode de gestion automatique grâce à un capteur de luminosité.
- Les phares arrière s’allument plus fort en cas de freinage.
- La température extérieure est reportée sur le smartphone.

2.3 Fonctionnement détaillé attendu

- Deux détecteurs de distance infrarouge (un à l’arrière et l’autre à l’avant) permettent de mesurer la distance à l’obstacle et la vitesse d'approche du véhicule, et d’identifier la zone de proximité
- Un buzzer permet d’émettre des bips plus ou moins rapprochés en fonction de la distance à l’obstacle (2 bips par seconde dans la zone Moyen et 4 bips par seconde dans la zone près). Le klaxon est prioritaire sur les bips.
- Le véhicule réduit automatiquement de 50% la consigne de vitesse dans la zone moyenne lorsque le déplacement s’effectue vers l’obstacle.
- Le véhicule freine à fond (inversion de la propulsion à consigne maximale), jusqu’à arrêt du véhicule, lorsque le véhicule s’approche de l’obstacle dans la zone près, et cela quel que soit la consigne.
- Les phares arrière s’allument plus fort à chaque freinage.
- Les phares avant et arrière s’allument de façon permanente si le switch virtuel est dans la position “ON” et automatiquement en cas de faible luminosité si le switch est en position “AUTO”
- ,,,,,,,,,,,

**3\. Contraintes**

3.1 Coûts

Aucune contrainte de coût n’est imposée. En revanche un calcul de coût unitaire pour une production de 100 cartes est attendu (composants électroniques + circuit imprimé + voiture + accessoires + main d'œuvre).

3.2 Autonomie de fonctionnement

La voiture doit avoir une autonomie d'au moins 30 minutes avec une batterie de 6 accumulateurs NiMh de capacité 2300mAh.

3.3 Délais de réalisation

2h C / 28h TD / 27h TP / 25h autonomie

3.4 Autres contraintes

\- travail en binôme

\- time sharing (voiture partagée)

\- cahier de laboratoire / de suivi à mettre à jour à la fin de CHAQUE séance sur TEAMS

\- Dossier de conception (cahier de laboratoire + documentation technique + simulations)

\- Dossier de fabrication (cahier de fabrication + fichiers GERBER + fichier PROTEUS)

\- Réalisation de portfolio

**4\. Déroulement du projet**

4.1 Séquencement

**Semaine 4**

\- Cours de présentation du projet et analyse fonctionnelle (durée 2h en amphi)

\- Création du planning du projet en vue de l’intégrer dans le cahier de laboratoire + mise en forme du cahier de fabrication en vue de son utilisation dans la nouvelle SAE (1 séance de 2h en autonomie)

\- Composition des binômes + réalisation du circuit Imprimé (1 séance de 2h tutorée suivie de 1 séance 2h en autonomie)

**Semaine 5**

\- Suite réalisation du circuit Imprimé (1 séance de 2h encadrée suivie de 1 séance de 2h d’autonomie)

\- Suite réalisation du circuit Imprimé + Elaboration du cahier de fabrication et du dossier de fabrication (Fichiers GERBER + cahier de fabrication + fichier PROTEUS) (1 séance de 1h encadrée suivie de 1 séance de 1h d’autonomie)

**Semaine 6**

\- Elaboration d'une fiche de commercialisation (1 séance de 2h turorée VdE suivie de 1 séance de 2h en autonomie pour finaliser la fiche de commercialisation).

**Semaine 7**

- Elaboration du cahier de laboratoire et du dossier de conception dans l'espace de travail du binôme sur Teams (voir SAE base roulante et « ressources pour les cahiers » dans teams équipe SmartCar) (A faire chez soi avant la semaine 10).
- Analyse fonctionnelle + étude de cout + étude d’autonomie
- Dimensionnement de la fonction alimentation (référence de tension 2,5V, les condensateurs, la led bleu visu de mise sous tension). Dimensionnement de la fonction capteur de luminosité. Dimensionnement de la fonction niveau batterie.

**Semaine 9 à 14 (Sauf semaine 10 vacances)**

\- Dimensionnement (conception), mise en œuvre et tests unitaires des différentes fonctions de la carte électronique :

- - - - Fonction Alimentation et fonction capteur de luminosité
            - Fonction niveau de batterie
            - Comparateurs pour la détection des zones arrière
            - Fonction oscillateur pour le signal du buzzer et fonction capteur de température
            - Fonction Adaptation d'impédance et fonction dérivateur pour la mesure de vitesse d'approche
            - Fonction comparateur détection d'approche obstacle arrière et fonction volume

**Semaine 16 à 17**

- Vacances

**Semaine 15 puis 18 et 22**

\- Conception, tests unitaires de la partie informatique embarquée :

Alias des E/S + Interface graphique du SmartPhone + structuration du programme

Fonction BUZZER

Fonction direction + fonction propulsion

Fonction gestion des phares + fonction gestion température

Fonction gestion du capteur avant

4.2 Documentation

\- La documentation fournie est accessible sur TEAMS ou sur moodle

\- La documentation à fournir est accessible via les liens de la nomenclature de KiCad ou directement sur internet.

4.3 Responsabilités

a) Maitrise d’ouvrage (ses missions)  les enseignants

\- définir les objectifs et contraintes du projet

\- vérifier l’avancement du projet

\- apport de conseil / support technique

\- validation systématique du dossier technique (cahier de laboratoire) au fur et à mesure de l'avancement

b) Maitrise d’œuvre (ses missions)  Le binôme

\- répondre aux objectifs et contraintes du projet.
