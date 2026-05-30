
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
