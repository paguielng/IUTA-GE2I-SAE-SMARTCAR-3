# SmartCar — Documentation des fonctions

Projet SmartCar développé en C++ (STM32 Nucléo) dans le cadre du BUT GEII à l'IUT de Toulouse.
Pilotage via l'application **RemoteXY** (Bluetooth), avec gestion de la direction, propulsion, phares, buzzer et capteurs.

---

## Sommaire

- [Brochage](#brochage)
- [Timers PWM](#timers-pwm)
- [Fonctions](#fonctions)
  - [pwm_direction](#pwm_directionint-x)
  - [PWM_propulsion](#pwm_propulsionbool-pres-bool-loin-bool-sens_ar-int-vitesse-bool-connexion)
  - [gestion_bargraphe](#gestion_bargrapheuint32_t-nivb)
  - [bargraphe_ameliore](#bargraphe_amelioreuint32_t-nivb-uint32_t-cap_av)
  - [gestion_phares](#gestion_pharesbool-freinage-bool-lum)
  - [gestion_buzzer](#gestion_buzzerbool-pres-bool-loin)
  - [recep_temperature](#recep_temperaturevoid)
  - [setup](#setup)
  - [loop](#loop)
- [Bugs connus](#bugs-connus)

---

## Brochage

| Alias | Broche | Rôle |
|---|---|---|
| `PWM_AVANT_PIN` | D4 | Propulsion avant |
| `PWM_ARRIERE_PIN` | D5 | Propulsion arrière |
| `PWM_DROITE_PIN` | D3 | Direction droite |
| `PWM_GAUCHE_PIN` | D6 | Direction gauche |
| `PHARES_AV_PIN` | D11 | Phares avant |
| `PHARES_AR_PIN` | D2 | Phares arrière |
| `CMD_GENE_AUDIO_PIN` | D9 | Buzzer |
| `PRES_PIN` | D7 | Capteur proximité (proche) |
| `LOIN_PIN` | D8 | Capteur proximité (loin) |
| `SENS_AR_PIN` | PA1 | Capteur sens marche arrière |
| `NIV_BATT_PIN` | A4 | Niveau batterie |
| `DISTANCE_AV_PIN` | A2 | Capteur distance avant |
| `SDA_PIN` | D14 | I2C SDA (température) |
| `SCL_PIN` | D15 | I2C SCL (température) |

---

## Timers PWM

| Timer | Usage | Prescaler | Overflow | Fréquence |
|---|---|---|---|---|
| TIM2 | Direction | 40 | 100 | 20 000 Hz |
| TIM3 | Propulsion | 40 | 100 | 20 000 Hz |
| TIM4 | Phares | 40 | 100 | 20 000 Hz |

> Formule : `F = 80 000 000 / (Prescaler × Overflow)`

---

## Fonctions

### `pwm_direction(int x)`

Contrôle la direction (gauche/droite) via les canaux PWM du TIM2.

**Paramètre**
- `x` — valeur joystick axe X, entre -100 (gauche) et +100 (droite)

**Fonctionnement**

| Condition | Canal DROITE | Canal GAUCHE |
|---|---|---|
| `-10 < x < 10` (zone morte) | 0 | 0 |
| `x < -10` (gauche) | `map(abs(x), 0, 100, 0, 100)` | 0 |
| `x > 10` (droite) | 0 | `map(abs(x), 0, 100, 0, 100)` |

```cpp
void pwm_direction(int x) {
  int pwm;
  if (x > -10 && x < 10) {
    timerDirection.setCaptureCompare(CANAL_DROITE, 0);
    timerDirection.setCaptureCompare(CANAL_GAUCHE, 0);
    return;
  }
  pwm = map(abs(x), 0, 100, 0, 100);
  if (x < 0) {
    timerDirection.setCaptureCompare(CANAL_DROITE, pwm);
    timerDirection.setCaptureCompare(CANAL_GAUCHE, 0);
  } else {
    timerDirection.setCaptureCompare(CANAL_DROITE, 0);
    timerDirection.setCaptureCompare(CANAL_GAUCHE, pwm);
  }
}
```

---

### `PWM_propulsion(bool pres, bool loin, bool sens_ar, int vitesse, bool connexion)`

Gère l'avance ou le recul en tenant compte des capteurs et de la connexion Bluetooth.

**Paramètres**
- `pres` — obstacle très proche (<40 cm) → `true`
- `loin` — obstacle détecté mais à distance → `true`
- `sens_ar` — marche arrière demandée → `true`
- `vitesse` — rapport cyclique PWM (0–100)
- `connexion` — Bluetooth connecté → `true`

**Priorités (ordre décroissant)**

1. Pas de connexion → stop total
2. `sens_ar == true` → recul à `vitesse`
3. `pres == true` → stop (obstacle proche)
4. `loin == true` → avance à `vitesse / 2`
5. Voie libre → avance à `vitesse`

```cpp
void PWM_propulsion(bool pres, bool loin, bool sens_ar, int vitesse, bool connexion) {
  const uint32_t CANAL_AVANT   = 2;
  const uint32_t CANAL_ARRIERE = 3;
  if (!connexion) {
    timerPropulsion.setCaptureCompare(CANAL_AVANT, 0);
    timerPropulsion.setCaptureCompare(CANAL_ARRIERE, 0);
    return;
  }
  if (sens_ar) {
    timerPropulsion.setCaptureCompare(CANAL_AVANT, 0);
    timerPropulsion.setCaptureCompare(CANAL_ARRIERE, vitesse);
    return;
  }
  if (pres) {
    timerPropulsion.setCaptureCompare(CANAL_AVANT, 0);
    timerPropulsion.setCaptureCompare(CANAL_ARRIERE, 0);
  } else if (loin) {
    timerPropulsion.setCaptureCompare(CANAL_AVANT, vitesse / 2);
    timerPropulsion.setCaptureCompare(CANAL_ARRIERE, 0);
  } else {
    timerPropulsion.setCaptureCompare(CANAL_AVANT, vitesse);
    timerPropulsion.setCaptureCompare(CANAL_ARRIERE, 0);
  }
}
```

---

### `gestion_bargraphe(uint32_t nivB)`

Convertit la valeur brute du CAN (0–4095) en pourcentage (0–100) pour le bargraph RemoteXY.

**Paramètre**
- `nivB` — valeur brute du CAN 12 bits correspondant à la tension batterie

**Seuils**

| Valeur CAN | Tension | Bargraph |
|---|---|---|
| ≤ 3102 | ≤ 2.5V | 0% |
| ≥ 3723 | ≥ 3.0V | 100% |
| Entre les deux | — | `0.16117 × nivB − 500` |

```cpp
void gestion_bargraphe(uint32_t nivB) {
  const uint32_t x = 3102; // 2.5V
  const uint32_t y = 3723; // 3.0V
  if (nivB <= x)      RemoteXY.Bargraph = 0;
  else if (nivB >= y) RemoteXY.Bargraph = 100;
  else                RemoteXY.Bargraph = 0.16117 * nivB - 500;
}
```

---

### `bargraphe_ameliore(uint32_t nivB, uint32_t cap_av)`

Version améliorée : affiche la proximité de l'obstacle avant en priorité, revient à la batterie quand la voie est libre.

**Paramètres**
- `nivB` — valeur brute CAN de la batterie
- `cap_av` — valeur brute CAN du capteur de distance avant

**Logique**

| cap_av | Affichage |
|---|---|
| ≤ 930 (≈0.75V, très proche) | 0% |
| > 3723 (≈3V, loin) | Niveau batterie via `gestion_bargraphe()` |
| Entre les deux | `0.033 × cap_av − 30.69` |

---

### `gestion_phares(bool freinage, bool lum)`

Contrôle l'intensité des phares avant et arrière via PWM.

**Paramètres**
- `freinage` — `true` → phares AV à 100% (255/255), `false` → 30% (77/255)
- `lum` — `true` → phares AR à 30%, `false` → éteints

> ⚠️ **Bug** : `analogWrite(PHARES_AR_PIN, intensite_ar)` est absent — les phares arrière ne s'allument jamais.

```cpp
void gestion_phares(bool freinage, bool lum) {
  uint8_t intensite_av = freinage ? 255 : 77;
  analogWrite(PHARES_AV_PIN, intensite_av);

  uint8_t intensite_ar = lum ? 30 : 0;
  analogWrite(PHARES_AR_PIN, intensite_ar); // ligne manquante dans le code original
}
```

---

### `gestion_buzzer(bool pres, bool loin)`

Gère le buzzer de proximité avec une logique non-bloquante basée sur `millis()`.

**Paramètres**
- `pres` — obstacle très proche → 4 bips/sec (interval = 125 ms)
- `loin` — obstacle loin → 2 bips/sec (interval = 250 ms)

**Comportement**

| Condition | Comportement |
|---|---|
| `klaxon == 1` (bouton appuyé) | Buzzer continu (prioritaire) |
| `pres == true` | 4 bips/sec |
| `loin == true` | 2 bips/sec |
| Aucun obstacle | Silence |

> ⚠️ **Bug 1** : `loin` et `else` (silence) sont inversés dans le code original.  
> ⚠️ **Bug 2** : `buzzerState` n'est jamais remis à `false` après relâchement du klaxon.

**Version corrigée**

```cpp
void gestion_buzzer(bool pres, bool loin) {
  uint8_t klaxon = RemoteXY.klaxon;
  static uint32_t tempsPrecedent = millis();
  static bool buzzerState = false;
  uint32_t tempsActuel = millis();
  uint32_t tempsEcoule = tempsActuel - tempsPrecedent;
  uint32_t interval = 0;

  if (klaxon == 1) {
    buzzerState = true;
  } else {
    if (pres)           interval = 125;
    else if (loin)      interval = 250;
    else {
      digitalWrite(CMD_GENE_AUDIO_PIN, LOW);
      return;
    }
    if (tempsEcoule >= interval) {
      tempsPrecedent = tempsActuel;
      buzzerState = !buzzerState;
    }
  }
  digitalWrite(CMD_GENE_AUDIO_PIN, buzzerState);
}
```

---

### `recep_temperature(void)`

Lit la température depuis un capteur I2C compatible LM75 (adresse `0x48`).

**Protocole I2C**

1. `beginTransmission(0x48)` → ouvre la communication
2. `write(0x00)` → sélectionne le registre de température
3. `endTransmission(false)` → repeated start
4. `requestFrom(0x48, 2)` → demande 2 octets (MSB + LSB)
5. Lecture du MSB (partie entière signée)

> ⚠️ **Incomplet** : la lecture du LSB (fraction) et l'envoi vers `RemoteXY` sont commentés — la température n'est pas affichée.

---

### `setup()`

Initialisation unique au démarrage. Ordre d'exécution :

1. `RemoteXY_Init()` — démarrage communication Bluetooth
2. `pinMode()` — configuration GPIO entrée/sortie
3. `analogReadResolution(12)` — CAN 12 bits (0–4095)
4. Configuration TIM2 (direction), TIM3 (propulsion), TIM4 (phares) — prescaler=40, overflow=100
5. `Wire.begin()` — démarrage I2C sur D14 (SDA) / D15 (SCL)

---

### `loop()`

Boucle principale exécutée en permanence. Ordre d'exécution :

1. `RemoteXYEngine.handler()` — échange Bluetooth
2. Lecture des capteurs (`digitalRead` / `analogRead`)
3. Appels aux fonctions de pilotage : bargraph, buzzer, direction, phares

> ⚠️ **Bug** : les variables `freinage` et `lum` passées à `gestion_phares()` ne sont jamais initialisées → comportement indéterminé.

---

## Bugs connus

| Fonction | Sévérité | Description | Correction |
|---|---|---|---|
| `gestion_phares` | 🔴 Majeur | `analogWrite(PHARES_AR_PIN, ...)` manquant | Ajouter la ligne |
| `gestion_buzzer` | 🟠 Moyen | `loin` et `else` inversés | Échanger les blocs |
| `gestion_buzzer` | 🟠 Moyen | `buzzerState` non réinitialisé après klaxon | Ajouter reset dans le `else` |
| `recep_temperature` | 🟡 Mineur | LSB et affichage commentés | Décommenter et compléter |
| `loop` | 🟠 Moyen | `freinage` et `lum` non initialisés | Initialiser à `false` |

---

*Documentation générée à partir du code source SmartCar — BUT GEII IUT Toulouse*
