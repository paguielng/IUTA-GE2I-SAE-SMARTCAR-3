////////////////////////////////////////////////////////
//              Inclusion des librairies              //
////////////////////////////////////////////////////////
#include <Wire.h> // Librairie pour l'utilisation de l'I2C
#define SoftwareSerial_h

///////////////////////////////////////////////////////////
//  Déclarations des ALIAS des broches analogiques (pin) //
///////////////////////////////////////////////////////////
#define CAP_LUM_PIN A0  // le terme CAP_LUM_PIN sera remplacé par le compilateur* par le terme A0 (*par le préprocesseur)
#define SENS_AR_PIN PA1 // changé de A1 à PA1 par Regis Nohe
#define DISTANCE_AV_PIN A2
#define DISTANCE_AR_PIN A3
#define NIV_BATT_PIN A4

///////////////////////////////////////////////
//  Déclarations des ALIAS des broches (pin) //
///////////////////////////////////////////////
const uint32_t PHARES_AR_PIN = D2;
const uint32_t PHARES_AV_PIN = D11;

const uint32_t CMD_GENE_AUDIO_PIN = D9;
const uint32_t TONE_PIN = D10;
const uint32_t PRES_PIN = D7;
const uint32_t LOIN_PIN = D8;

const uint32_t PWM_AVANT_PIN = D4;
const uint32_t PWM_ARRIERE_PIN = D5;
const uint32_t PWM_DROITE_PIN = D3;
const uint32_t PWM_GAUCHE_PIN = D6;

const uint16_t RX_PIN = D12; // mettre à jour les #define des codes générés par RemoteXY
const uint16_t TX_PIN = D13; // mettre à jour les #define des codes générés par RemoteXY

const uint32_t SDA_PIN = D14;
const uint32_t SCL_PIN = D15;

// Déclaration des canaux utilisés pour les PWM  //
const uint32_t CANAL_DROITE = 2;
const uint32_t CANAL_GAUCHE = 3;
const uint32_t CANAL_PHARE = 3;   // Le canal pour phares arrière
const uint32_t CANAL_AVANT = 3;   // canal 3 pour la propulsion avant
const uint32_t CANAL_ARRIERE = 2; // canal 2 pour la propulsion arrière

// Déclaration des timers            //
HardwareTimer timerDirection(TIM2);
HardwareTimer timerPropulsion(TIM3);
HardwareTimer timerPhares(TIM4);

//////////////////////////////////////////////
//        RemoteXY include library          //
//////////////////////////////////////////////

// you can enable debug logging to Serial at 115200
// #define REMOTEXY__DEBUGLOG

// RemoteXY select connection mode and include library
#define REMOTEXY_MODE__SOFTSERIAL

#include <SoftwareSerial.h>

// RemoteXY connection settings
#define REMOTEXY_SERIAL_RX 12
#define REMOTEXY_SERIAL_TX 13
#define REMOTEXY_SERIAL_SPEED 9600

#include <RemoteXY.h>

// RemoteXY GUI configuration
#pragma pack(push, 1)
uint8_t const PROGMEM RemoteXY_CONF_PROGMEM[] = // 106 bytes V19
    {255, 3, 0, 15, 0, 99, 0, 19, 0, 0, 0, 0, 31, 2, 106, 200, 200, 84, 1, 1,
     5, 0, 73, 90, 10, 7, 76, 174, 5, 14, 32, 4, 128, 19, 135, 26, 0, 0, 0, 0,
     0, 0, 200, 66, 0, 0, 0, 0, 67, 81, 155, 21, 24, 8, 6, 40, 10, 70, 2, 26,
     11, 5, 251, 17, 143, 143, 110, 17, 58, 58, 32, 2, 26, 31, 1, 240, 60, 57, 57, 17,
     39, 24, 24, 0, 2, 31, 0, 129, 12, 78, 71, 29, 69, 5, 55, 12, 64, 17, 83, 77,
     65, 84, 67, 65, 82, 0};

// this structure defines all the variables and events of your control interface
struct
{

  // input variables
  int8_t joystick_01_x; // from -100 to 100
  int8_t joystick_01_y; // from -100 to 100
  uint8_t klaxon;       // =1 if button pressed, else =0, from 0 to 1

  // output variables
  float Bargraph;      // from 0 to 100
  char zone_texte[11]; // string UTF8 end zero

  // other variable
  uint8_t connect_flag; // =1 if wire connected, else =0

} RemoteXY;
#pragma pack(pop)

/////////////////////////////////////////////
//           END RemoteXY include          //
/////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////
//                                 DEFINITIONS DE FONCTIONS                               //
////////////////////////////////////////////////////////////////////////////////////////////
int PWM_DROITE; // commande la direction à droite
int PWM_GAUCHE; // commande la direction à gauche
int joystickX;  // La la valeur que renvoie joystick sur l'axe X (-100 ≤ valeur de l'axe X ≤ 100)

int connexion_bluetooth;
int PWM_AVANT;
int PWM_ARRIERE;

////////////////////////////////
//  Gestion de la direction   //
////////////////////////////////
void pwm_direction()
{
  int8_t joystick_x = RemoteXY.joystick_01_x;  // joystick (-100 à +100)

  if (RemoteXY.connect_flag == 1) // Si telephone connecte
  {
    if (joystick_x > 10)          // Tourner a droite
    {
      timerDirection.setCaptureCompare(CANAL_DROITE, joystick_x);  // moteur droite actif
      timerDirection.setCaptureCompare(CANAL_GAUCHE, 0);           // moteur gauche a l'arrêt
    }
    else if (joystick_x < -10)    // Tourner a gauche
    {
      timerDirection.setCaptureCompare(CANAL_DROITE, 0);                  // moteur droite à l'arrêt
      timerDirection.setCaptureCompare(CANAL_GAUCHE, (-1) * joystick_x);  // moteur gauche actif
    }
    else // zone morte (joystick centré)
    {
      timerDirection.setCaptureCompare(CANAL_DROITE, 0);  // moteur droite a l'arrêt
      timerDirection.setCaptureCompare(CANAL_GAUCHE, 0);  // moteur gauche à l'arrêt
    }
  }

  Serial.println(joystick_x);  // debuuuug
}

/////////////////////////////////
//  Gestion de la propulsion   //
/////////////////////////////////
bool pwm_propulsion(bool p, bool l, bool s_ar)
{
  bool freiner = 0;

  if (RemoteXY.connect_flag == 1)
  {
    if (RemoteXY.joystick_01_y > 10)
    {
      timerPropulsion.setCaptureCompare(CANAL_AVANT, RemoteXY.joystick_01_y);
      timerPropulsion.setCaptureCompare(CANAL_ARRIERE, 0);
    }

    else if (RemoteXY.joystick_01_y < -10)
    {
      if (l == 1)
      {
        timerPropulsion.setCaptureCompare(CANAL_AVANT, 0);
        timerPropulsion.setCaptureCompare(CANAL_ARRIERE, (RemoteXY.joystick_01_y * (-1)));
      }
      else if (p == 0)
      {
        timerPropulsion.setCaptureCompare(CANAL_AVANT, 0);
        timerPropulsion.setCaptureCompare(CANAL_ARRIERE, (RemoteXY.joystick_01_y * (-1)) / 2);
      }
      else
      {
        if (s_ar == false)
        {
          timerPropulsion.setCaptureCompare(CANAL_AVANT, 100);
          timerPropulsion.setCaptureCompare(CANAL_ARRIERE, 0);
          freiner = true;
        }
        else
        {
          timerPropulsion.setCaptureCompare(CANAL_AVANT, 0);
          timerPropulsion.setCaptureCompare(CANAL_ARRIERE, 0);
        }
      }
    }
    else
    { // si la joystick est au milieu
      if (p == 1)
      {
        if (s_ar == false)
        {
          timerPropulsion.setCaptureCompare(CANAL_AVANT, 100);
          timerPropulsion.setCaptureCompare(CANAL_ARRIERE, 0);
          freiner = true;
        }
        else
        {
          timerPropulsion.setCaptureCompare(CANAL_AVANT, 0);
          timerPropulsion.setCaptureCompare(CANAL_ARRIERE, 0);
        }
      }
      else
      {
        timerPropulsion.setCaptureCompare(CANAL_AVANT, 0);
        timerPropulsion.setCaptureCompare(CANAL_ARRIERE, 0);
      }
    }
  }
  return freiner;
}

////////////////////////////////////////////////////////////////////////////////////////////
//  Gestion du bargraph du téléphone de niveau batterie ou de proximité du capteur avant  //
////////////////////////////////////////////////////////////////////////////////////////////
void gestion_bargraphe(uint32_t nivB)
{
  // Remplace x et y par les valeurs ou variables réelles
  const uint32_t x = 3102; // seuil à 2.5V = (4095*2.5)/3.3
  const uint32_t y = 3723; // seuil à 3V = (4095*3)/3.3
  if (nivB <= x)
  {
    RemoteXY.Bargraph = 0; // Barre vide
  }
  else if (nivB >= y)
  {
    RemoteXY.Bargraph = 100; // Barre plein
  }
  else
  {
    RemoteXY.Bargraph = 0.16117 * nivB - 500; // Barre entre 2.5V et 3V
  }
}

/////////////////////////////////////
//  Gestion du bargraph améliorée  //
/////////////////////////////////////
void bargraphe_ameliore(uint32_t nivB, uint32_t cap_av)
{
  const uint32_t limite_inferieur = 930;  // seuil à 0.75V = (4095*0.75)/3.3
  const uint32_t limite_superieur = 3970; // seuil à 3.2V = (4095*3.2)/3.3

  if (cap_av <= limite_inferieur)
  {
    RemoteXY.Bargraph = 0; // Barre vide
  }
  else if (cap_av > limite_superieur)
  {
    // RemoteXY.Bargraph = 100; // Barre plein
    int nivBat = analogRead(NIV_BATT_PIN); // NIV_BATT_PIN prend automatiquement la broche associé (A4)
    gestion_bargraphe(nivB);
  }
  else
  {
    RemoteXY.Bargraph = 0.033 * cap_av - 30.69; // Barre entre 2.5V et 3V
  }
}

////////////////////////////
//    Gestion phares      //
////////////////////////////

void gestion_phares(bool freinage, uint32_t lum)
{
  bool phareAv = false;
  if (lum <= 1861) // Mode jour (clair)
  {
    if (freinage)
    {
      timerPhares.setCaptureCompare(CANAL_PHARE, 100);
    }
    else
      timerPhares.setCaptureCompare(CANAL_PHARE, 0);
  }

  else if (lum > 1861) // Mode nuit (sombre)
  {
    phareAv = true;
    if (freinage)
    {
      timerPhares.setCaptureCompare(CANAL_PHARE, 100);
    }
    else
      timerPhares.setCaptureCompare(CANAL_PHARE, 30);
  }

  if (phareAv)
  {
    digitalWrite(PHARES_AV_PIN, HIGH);
  }
  else
    digitalWrite(PHARES_AV_PIN, LOW);
}

////////////////////////////////////////////////////
//    Gestion du buzzer avec Klaxon italien       //
////////////////////////////////////////////////////
void gestion_buzzer(bool p, bool l)
{
  static uint32_t tempsPrecedent = millis();
  static bool buzzerState = LOW;
  static uint32_t demiPeriod = 250;
  uint32_t tempsActuel = millis();

  // Klaxon du smartphone (prioritaire sur le reste)
  if (RemoteXY.klaxon == 1) // On peux aussi utiliser juste klaxon avec (RemoteXY.klaxon = klaxon)
  {
    buzzerState = HIGH;
  }
  // Gestion des bips
  else
  {
    // 4 bips par seconde lorsque la voiture est dans la zone près à l'avant ou à l'arrière
    if (p == true)
    {
      demiPeriod = 125; // 125ms = 4 bips/sec
    }
    else
    {
      demiPeriod = 250; // 250ms = 2 bips/sec
    }
    if (tempsActuel - tempsPrecedent >= demiPeriod)
    {
      tempsPrecedent = tempsActuel;

      // 2 bips par seconde lorsque la voiture est dans la zone moyen à l'avant ou à l'arrière
      if (l == false)
      {
        buzzerState = !buzzerState;
      }
      else
      {
        buzzerState = LOW;
      }
    }
  }
  digitalWrite(CMD_GENE_AUDIO_PIN, buzzerState);
}

////////////////////////////////////////////////////
//    Gestion du buzzer améliorée                 //
////////////////////////////////////////////////////
void gestion_buzzer_ameliore(bool p, bool l, bool sens_av, uint32_t capt_av_moy)
{
  static uint32_t tempsPrecedent = millis();
  static bool buzzerState = LOW;
  static uint32_t demiPeriod = 250;
  uint32_t tempsActuel = millis();

  // Klaxon du smartphone (prioritaire sur le reste)
  if (RemoteXY.klaxon == 1)
  {
    buzzerState = HIGH;
  }
  else
  {
    // Détermination de la demi-période selon la zone
    if (p == true)
    {
      demiPeriod = 125; // 4 bips/sec en zone près
    }
    else
    {
      demiPeriod = 250; // 2 bips/sec en zone moyen
    }

    if (tempsActuel - tempsPrecedent >= demiPeriod)
    {
      tempsPrecedent = tempsActuel;

      // Capteur arrière : bip si obstacle détecté
      bool bip_ar = (l == false);

      // Capteur avant : bip uniquement si en approche
      bool bip_av = (capt_av_moy >= 930) && (sens_av == true);

      if (bip_ar || bip_av)
      {
        buzzerState = !buzzerState;
      }
      else
      {
        buzzerState = LOW;
      }
    }
  }

  digitalWrite(CMD_GENE_AUDIO_PIN, buzzerState);
}

////////////////////////////////////////////
//    Gestion approche obstacle avant     //
////////////////////////////////////////////
bool gestion_sens_avant(uint32_t capt_av_moy)
{
  static uint32_t tempPrecedent = 0;
  static uint32_t valeurPrecedente = 0;
  uint32_t tempActuel = millis();
  bool approche = false;

  if (tempActuel - tempPrecedent >= 100)
  {
    tempPrecedent = tempActuel;

    if (capt_av_moy > valeurPrecedente + 5)
    {
      approche = true;
    }

    valeurPrecedente = capt_av_moy;
  }

  return approche;
}

/////////////////////////////////////////
//    Moyennage du capteur avant       //
/////////////////////////////////////////
uint32_t gestion_capteur_av(uint32_t capt_av)
{
  static uint32_t tempPrecedent = 0;
  static uint32_t tab[10] = {0};
  static uint8_t indextab = 0;
  uint32_t tempActuel = millis();
  uint32_t somme = 0;

  // Acquisition toutes les 10 ms
  if (tempActuel - tempPrecedent >= 10)
  {
    tempPrecedent = tempActuel;

    // Stockage dans le tableau avec index glissant
    tab[indextab] = capt_av;
    indextab++;
    if (indextab >= 10)
      indextab = 0;
  }

  // Calcul de la moyenne sur les 10 valeurs
  for (uint8_t indextab_boule = 0; indextab_boule < 10; indextab_boule++)
  {
    somme += tab[indextab_boule];
  }

  return somme / 10;
}

/////////////////////////////////////
//    Gestion de la température    //
/////////////////////////////////////
void recep_temperature(void)
{
  static uint32_t tempPrecedent = 0;
  uint32_t tempActuel = millis();

  // Lecture toutes les 3 secondes
  if (tempActuel - tempPrecedent >= 3000)
  {
    tempPrecedent = tempActuel;

    int8_t temp_Ambiante_entier = 0;
    uint8_t temp_Ambiante_frac = 0;

    // Communication avec le dispositif I2C
    Wire.beginTransmission(0x48); // Initiation de la communication I2C avec le composant d'adresse 0x48
    Wire.write(0x00);             // Acces au registre de Temperature Ambiante
    Wire.endTransmission(false);  // Réinitialisation de la communication en vue de la réception de données
    Wire.requestFrom(0x48, 2);    // Demande de réception des 2 octets de température MSB : signe T6 T5 T4 T3 T2 T1 T0 / LSB : Fraction X X X X X X X X

    // Lecture des données reçues
    if (Wire.available() >= 2) // Si les 2 octets de données sont reçus
    {
      temp_Ambiante_entier = Wire.read(); // Lecture de l'octet de poids fort MSB : signe T6 T5 T4 T3 T2 T1 T0
      temp_Ambiante_frac = Wire.read();   // Lecture de l'octet de poids faible LSB : Fraction X X X X X X X X
    }

    // Envoi sur le smartphone
    // La fraction vaut 0.5 si le bit 7 du LSB est à 1, sinon 0
    float temp = temp_Ambiante_entier + ((temp_Ambiante_frac & 0x80) ? 0.5f : 0.0f);

    // Interprétation des données et envoie sur l'interface graphique
    snprintf(RemoteXY.zone_texte, sizeof(RemoteXY.zone_texte), "%.1f C", temp);
  }
}

///////////////////////////////////////////////////////////////////////
//              Initialisations                                      //
///////////////////////////////////////////////////////////////////////
void setup()
{
  /////////////////////////////////////////////////////////////////////
  // Initialisation de la communication avec linterface graphique    //
  /////////////////////////////////////////////////////////////////////
  RemoteXY_Init(); // initialization by macros

  /////////////////////////////////////////////////////////////////////
  // Initialisation de la résolution des CANs                        //
  /////////////////////////////////////////////////////////////////////
  analogReadResolution(12); // Code de l’initialisation du CAN à 12 bits

  /////////////////////////////////////////////////////////////////////
  // Initialisation des broches en entrée ou en sortie               //
  /////////////////////////////////////////////////////////////////////
  pinMode(CMD_GENE_AUDIO_PIN, OUTPUT); // CMD_GENE_AUDIO_PIN est configurée en sortie
  pinMode(PRES_PIN, INPUT);            // PRES_PIN est configurée en sortie
  pinMode(LOIN_PIN, INPUT);            // LOIN_PIN est configurée en sortie
  pinMode(SENS_AR_PIN, INPUT);         // SENS_AR_PIN est configurée en sortie
  pinMode(PHARES_AV_PIN, OUTPUT);

  /////////////////////////////////////////////////////////////////////
  // Initialisation des timer PWM Direction                          //
  /////////////////////////////////////////////////////////////////////

  timerDirection.setMode(CANAL_DROITE, TIMER_OUTPUT_COMPARE_PWM1, PWM_DROITE_PIN);
  timerDirection.setMode(CANAL_GAUCHE, TIMER_OUTPUT_COMPARE_PWM1, PWM_GAUCHE_PIN);
  timerDirection.setPrescaleFactor(44);
  timerDirection.setOverflow(100);
  timerDirection.setCaptureCompare(CANAL_DROITE, 0);
  timerDirection.setCaptureCompare(CANAL_GAUCHE, 0);
  timerDirection.resume();

  ////////////////////////////////////////////////////////////////////
  // Initialisation des timer PWM Propulsion                        //
  ////////////////////////////////////////////////////////////////////

  // timerPropulsion.setPrescaleFactor(44);
  // timerPropulsion.setOverflow(100);

  // timerPropulsion.setMode(CANAL_AVANT, TIMER_OUTPUT_COMPARE_PWM1, PWM_AVANT_PIN);
  // timerPropulsion.setMode(CANAL_ARRIERE, TIMER_OUTPUT_COMPARE_PWM1, PWM_ARRIERE_PIN);

  // timerPropulsion.setCaptureCompare(CANAL_AVANT, 0);
  // timerPropulsion.setCaptureCompare(CANAL_ARRIERE, 0);

  // timerPropulsion.resume();

  ////////////////////////////////////////////////////////////////////
  // Initialisation des timer PWM Phares                            //
  ////////////////////////////////////////////////////////////////////
  // timerPhares.setPrescaleFactor(1600); // calculé en TD
  // timerPhares.setOverflow(100);

  // timerPhares.setMode(CANAL_PHARE, TIMER_OUTPUT_COMPARE_PWM1, PHARES_AR_PIN);
  // timerPhares.setCaptureCompare(CANAL_PHARE, 0);

  // timerPhares.resume();

  /////////////////////////////////////////////////////////////////////
  // Initialisation communication liaison série                      //
  /////////////////////////////////////////////////////////////////////
  Serial.begin(115200);
  /////////////////////////////////////////////////////////////////////
  // Initialisation communication I2C avec capteur de temperataure   //
  /////////////////////////////////////////////////////////////////////
  Wire.setSCL(SCL_PIN);
  Wire.setSDA(SDA_PIN);
  Wire.begin();
}

//////////////////////////////////////////////////////
//                 Boucle infinie                   //
//////////////////////////////////////////////////////
void loop()
{
  // déclaration des variables
  bool sens_av;  // Le véhicule se rapproche de l'obstacle en marche avant  => sens_av=true sinon sens_av=false
  bool sens_ar;  // Le véhicule se rapproche de l'obstacle en marche arriere  => sens_ar=false sinon sens_ar=true
  bool pres;     // Variable indiquant la position pres du véhicule avec un obstacle arriere
  bool loin;     // Variable indiquant la position loin du véhicule avec un obstacle arriere
  bool freinage; // Variable indiquant un freinage (freinage = 1)
  int capt_av;   // Variable contenant la moyenne de valeur de la conversion du capteur avant
  int niv_batt;  // Variable contenant une information sur la tension batterie (8.7V batterie pleine -> 3.3V sur convertisseur; 6,5V batterie vide -> 2.5V sur convertisseur)
  int nivBat;
  bool lum;
  int capt_av_moy; // Variable contenant la moyenne de valeur de la conversion du capteur avant
  int cap_lum;     // Variable contenant la valeur de la conversion du capteur de luminosité
  int frein;       // Variable indiquant un freinage (frein = 1)
  int cap_avant;   // Variable contenant la valeur de la conversion du capteur avant

  /////////////////////////////////////////////////
  //  communication avec l'interface graphique   //
  /////////////////////////////////////////////////
  RemoteXYEngine.handler();

  ///////////////////////////////////////
  //  Informations de l'environnement  //
  ///////////////////////////////////////
  pres = digitalRead(PRES_PIN);      // Présence d'un obstacle à moins de 40cm (pres = '1')
  loin = digitalRead(LOIN_PIN);      // Présence d'un obstacle à plus de 40cm (LOIN = '10')
  nivBat = analogRead(NIV_BATT_PIN); // NIV_BATT_PIN prend automatiquement la broche associé (A4)
  capt_av = analogRead(DISTANCE_AV_PIN);
  sens_ar = digitalRead(SENS_AR_PIN);
  cap_lum = analogRead(CAP_LUM_PIN);
  cap_avant = analogRead(DISTANCE_AV_PIN);

  //////////////////////////////////////
  //  Jeu de test pour gestio_buzzer  //
  //////////////////////////////////////
  loin = 1;
  pres = 0;
  RemoteXY.klaxon = 0;

  ///////////////////////////////
  //  Pilotage de la voiture   //
  ///////////////////////////////
  gestion_bargraphe(nivBat);
  gestion_buzzer(pres, loin);
  // gestion_buzzer_ameliore(pres, loin, sens_av, capt_av_moy);
  // capt_av_moy = gestion_capteur_av(capt_av);
  // sens_av = gestion_sens_avant(capt_av_moy);
  // bargraphe_ameliore(nivBat, capt_av_moy);
  pwm_direction();

  // RemoteXY.connect_flag = true;
  // RemoteXY.joystick_01_y = 70; // Valeur de test pour la propulsion (-100 à 100)

  // frein = pwm_propulsion(pres, loin, sens_ar);
  // pwm_propulsion(pres, loin, sens_ar);

  // gestion_phares(frein, cap_lum);
  // recep_temperature();
}
