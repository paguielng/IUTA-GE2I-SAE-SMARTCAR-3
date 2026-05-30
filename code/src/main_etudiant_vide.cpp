//////////////////////////////////////////////////////////////////
//              Déclarations des librairies              /////////
//////////////////////////////////////////////////////////////////
#include <SoftwareSerial.h>     // Librairie pour l'utilisation de la liaison série avec le module Bluetooth
//#include <RemoteXY.h>           // Librairie pour l'utilisation de l'interface graphique du smartphone
#include <Wire.h>               // Librairie pour l'utilisation de l'I2C

/////////////////////////////////////////////////////////////////
//  Déclarations des ALIAS des broches analogiques (pin) /////////
//////////////////////////////////////////////////////////////////
#define CAP_LUM_PIN A0            // Association du mot CAP_LUM_PIN au numéro de broche A0  
#define SENS_AR_PIN A0           
#define DISTANCE_AV_PIN A0        
#define DISTANCE_AR_PIN A0        
#define NIV_BATT A0             
        
/////////////////////////////////////////////////////////////////
//  Déclarations des ALIAS des broches numériques (pin) /////////
/////////////////////////////////////////////////////////////////
const int  PHARES_AR_PIN = 1;   // Association du mot PHARES_AR_PIN au numéro de broche 2 équivalent à "#define PHARES_AR_PIN 2"   
const int  PWM1A_PIN = 1; 
const int  PHARES_AV_PIN = 1; 
const int  CMD_GENE_AUDIO_PIN = 1;
const int  TONE_PIN = 1;  
const int  PRES_PIN = 1;       // Association du mot PRES_PIN au numéro de broche 7 équivalent à "#define PRES_PIN 7"          
const int  LOIN_PIN = 1;
const int  PWM2A_PIN = 1;  
const int  PWM2B_PIN = 1;
const int  PWM1B_PIN = 1;
//const int  RX_PIN = 12;   // Declaration dans la section ci dessous RemoteXY connection settings avec #define
//const int  TX_PIN = 13;   // Declaration dans la section ci dessous RemoteXY connection settings avec #define
const int SDA_PIN = 1;              
const int SCL_PIN = 1;  


//////////////////////////////////////////////
//        RemoteXY include library          //
//////////////////////////////////////////////


/////////////////////////////////////////////
//           END RemoteXY include          //
/////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////////////////////
//                                 LES FONCTIONS                                                 //
///////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////
//  Gestion de la direction   //
////////////////////////////////

/////////////////////////////////
//  Gestion de la propulsion   //
/////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////
//  Gestion du bargraph du téléphone de niveau batterie ou de proximité du capteur avant  //
////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////
//    Gestion phares                  //
////////////////////////////////////////

////////////////////////////////////////////////////
//    Gestion du buzzer avec Klaxon italien       //
////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////
///// Gestion approche obstacle avant     ////////////////////
//////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
/////         Moyennage du capteur avant          ////////////////////
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
/////                 Gestion de la température                 //////
//////////////////////////////////////////////////////////////////////
 void recep_temperature (void)
 {
   static unsigned char temp_Ambiante_entier = 0;
   //static unsigned char temp_Ambiante_frac;
   //float temp;

  // Communication avec le dispositif I2C
   Wire.beginTransmission(0x48);    // Initiation de la communication I2C avec le composant d'adresse 0x48
   Wire.write(0x00);                // Acces au registre de Temperature Ambiante
   Wire.endTransmission(false);     // Réinitialisation de la communication en vue de la réception de données
   Wire.requestFrom(0x48,2);        // Demande de réception des 2 octets de température MSB : signe T6 T5 T4 T3 T2 T1 T0 / LSB : Fraction X X X X X X X X 

   // Lecture des données reçues
   if (2 <= Wire.available())       // Si les 2 octets de données sont reçus
   {
     temp_Ambiante_entier = Wire.read();  // Lecture de l'octet de poids fort MSB : signe T6 T5 T4 T3 T2 T1 T0
     //temp_Ambiante_frac = Wire.read();    // Lecture de l'octet de poids faible LSB : Fraction X X X X X X X X 
   }

  // Interprétation des données et envoie sur l'interface graphique
  //.................
 }


//////////////////////////////////////////////////////
//              Initialisation                      //
//////////////////////////////////////////////////////
void setup()
{
  //////////////////////////////////////////////////////////////////
  // Initialisation des broches numériques en entrée ou en sortie //
  //////////////////////////////////////////////////////////////////
  pinMode(PHARES_AR_PIN, INPUT);

  ///////////////////////////////////////////////////////////////////
  // Initialisation des 6 broches analogiques en entrée ou en sortie //
  ///////////////////////////////////////////////////////////////////
  pinMode(CAP_LUM_PIN, INPUT);
 
  /////////////////////////////////////////////////////////////////////
  // Initialisation communication liaison série                    //
  ///////////////////////////////////////////////////////////////////

  /////////////////////////////////////////////////////////////////////
  // Initialisation communication avec l'interface graphique        //
  ////////////////////////////////////////////////////////////////////

  /////////////////////////////////////////////////////////////////////
  // Initialisation communication I2C                                //
  /////////////////////////////////////////////////////////////////////
  Wire.setSCL(SCL_PIN);
  Wire.setSDA(SDA_PIN);
  Wire.begin();
}



///////////////////////////////////////////////////////
//                 Boucle infinie                   ///
///////////////////////////////////////////////////////
void loop()
{
  /////////////////////////////////////////////////////////////////////
  //  communication avec l'interface graphique                       //
  /////////////////////////////////////////////////////////////////////
  
  
  
  // déclaration des variables
  bool sens_av;   // Le véhicule se rapproche de l'obstacle en marche avant  => sens_av=true sinon sens_av=false
  bool sens_ar;   // Le véhicule se rapproche de l'obstacle en marche arriere  => sens_ar=false sinon sens_ar=true
  int capt_av;    // Variable contenant la moyenne de valeur de la conversion du capteur avant 
  int niv_batt;   // Variable contenant une information sur la tension batterie (8.7V batterie pleine -> 3.3V sur convertisseur; 6,5V batterie vide -> 2.5V sur convertisseur)
  bool pres;      // Variable indiquant la position pres du véhicule avec un obstacle arriere
  bool loin;      // Variable indiquant la position loin du véhicule avec un obstacle arriere
  bool freinage;  // Variable indiquant un freinage (freinage = 1)
  



//

  ///////////////////////////////////////
  //  Informations de l'environnement  //
  ///////////////////////////////////////
  pres = digitalRead (PRES_PIN);              // sommes nous dans la zone près arrière
  loin = digitalRead (LOIN_PIN);              // sommes nous dans la zone loin arrière
  sens_ar = digitalRead (SENS_AR_PIN);        // sens_ar=0 => la voiture et l'obstacle arrière se rapprochent
                                              // La valeur numérique de capt_av correspond à la distance entre la voiture et l'obstacle
                                              // sens_av=1 => la voiture et l'obstacle avant se rapprochent
                                              // niv_batt tension image de la tension batterie


  ///////////////////////////////
  //  Pilotage de la voiture   //
  ///////////////////////////////
  
}
