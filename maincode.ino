/*
 * ============================================================
 *   ESP32 STOVE SAFETY SYSTEM
 *   Arduino IDE Compatible
 * ============================================================
 *
 *  REQUIRED LIBRARIES (Install via Arduino Library Manager):
 *    1. "HX711 Arduino Library" by bogde
 *    2. "ESP32Servo" by Kevin Harrington
 *
 *  WIRING SUMMARY:
 *    Flame Sensor DO  → GPIO 18
 *    MQ-2 AO          → GPIO 34
 *    HX711 DT         → GPIO 4
 *    HX711 SCK        → GPIO 5
 *    Servo Signal     → GPIO 13  (Power from external 5V 2A)
 *    Buzzer +         → GPIO 27
 *    LED +            → GPIO 26  (through 220Ω resistor)
 *    Servo VCC        → External 5V (NOT from ESP32)
 *    Servo GND        → External GND + ESP32 GND (common ground!)
 *
 *  CASES HANDLED:
 *    Case 1: Gas ON + Flame ON + Pot ON      → Normal cooking, do nothing
 *    Case 2: Gas ON + Flame ON + No Pot 60s  → Turn knob OFF + alarm
 *    Case 3: Gas ON + No Flame               → Immediately turn knob OFF + alarm
 *    Bonus:  MQ-2 detects LPG leak           → Immediately turn knob OFF + alarm
 * ============================================================
 */

#include <HX711.h>
#include <ESP32Servo.h>

// ─────────────────────────────────────────
//   PIN DEFINITIONS
// ─────────────────────────────────────────
#define FLAME_SENSOR_PIN   18   // Flame sensor digital output
#define MQ2_PIN            34   // MQ-2 analog output
#define HX711_DT            4   // HX711 data pin
#define HX711_SCK           5   // HX711 clock pin
#define SERVO_PIN          13   // Servo signal pin
#define BUZZER_PIN         27   // Buzzer
#define LED_PIN            26   // Status LED

// ─────────────────────────────────────────
//   TUNABLE THRESHOLDS
//   Calibrate these for your setup!
// ─────────────────────────────────────────

// Minimum grams on scale to consider a pot is present
// Weigh your lightest pot and set this below that weight
#define POT_WEIGHT_THRESHOLD   150.0f   // grams

// MQ-2 raw ADC value (0–4095) above which we flag a gas leak
// In fresh air the MQ-2 typically reads 300–600; raise during burn-in
#define GAS_LEAK_THRESHOLD     2500

// How long (ms) flame must be ON with no pot before auto-shutoff
#define NO_POT_TIMEOUT_MS      60000UL  // 60 seconds

// Servo angles — ADJUST for your physical knob travel
// Verify by hand-testing before attaching to knob!
#define SERVO_KNOB_ON_ANGLE    90   // Angle when knob is in ON position
#define SERVO_KNOB_OFF_ANGLE    0   // Angle when knob is in OFF position

// HX711 scale calibration factor
// Steps to find yours:
//   1. Upload with calibrationFactor = 1.0
//   2. Place known weight, note raw reading
//   3. calibrationFactor = raw_reading / known_weight_grams
#define SCALE_CALIBRATION_FACTOR   -7050.0f

// Number of HX711 readings to average (more = slower but smoother)
#define SCALE_AVERAGE_READINGS      3

// Buzzer frequency in Hz
#define BUZZER_FREQ_HZ            2000

// Serial baud rate
#define SERIAL_BAUD              115200

// How often the main loop runs (ms)
#define LOOP_INTERVAL_MS           300

// ─────────────────────────────────────────
//   GLOBAL OBJECTS
// ─────────────────────────────────────────
HX711  scale;
Servo  stoveServo;

// ─────────────────────────────────────────
//   STATE VARIABLES
// ─────────────────────────────────────────
bool     knobIsOff         = false;   // Has system turned the knob off?
bool     noPotTimerActive  = false;   // Is the no-pot countdown running?
uint32_t noPotStartTime    = 0;       // millis() when no-pot timer started
bool     systemArmed       = true;    // Set false to disable auto-shutoff (debug)

// ─────────────────────────────────────────
//   FUNCTION PROTOTYPES
// ─────────────────────────────────────────
void     turnKnobOff();
void     resetSystem();
void     triggerAlarm(uint8_t beeps, uint16_t freqHz, uint16_t onMs, uint16_t offMs);
void     warningBeep();
void     printStatus(bool flame, int gas, float weight, bool pot);


// ═════════════════════════════════════════
//   SETUP
// ═════════════════════════════════════════
void setup() {
  Serial.begin(SERIAL_BAUD);
  Serial.println();
  Serial.println(F("========================================="));
  Serial.println(F("   ESP32 Stove Safety System v1.0"));
  Serial.println(F("========================================="));

  // ── GPIO setup ──────────────────────────
  pinMode(FLAME_SENSOR_PIN, INPUT);
  pinMode(MQ2_PIN,          INPUT);
  pinMode(BUZZER_PIN,       OUTPUT);
  pinMode(LED_PIN,          OUTPUT);

  digitalWrite(BUZZER_PIN, LOW);
  digitalWrite(LED_PIN,    LOW);

  // ── Servo init ──────────────────────────
  // ESP32Servo needs allowAllTimer() before attach()
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);
  stoveServo.setPeriodHertz(50);                  // Standard 50 Hz servo
  stoveServo.attach(SERVO_PIN, 500, 2400);         // 500–2400 µs pulse range
  stoveServo.write(SERVO_KNOB_ON_ANGLE);           // Assume knob starts ON
  Serial.println(F("Servo initialised → ON position"));

  // ── HX711 init ──────────────────────────
  scale.begin(HX711_DT, HX711_SCK);
  scale.set_scale(SCALE_CALIBRATION_FACTOR);

  Serial.println(F("Taring scale — remove all weight from load cell..."));
  delay(2000);   // Give user time
  scale.tare();
  Serial.println(F("Scale tared (zeroed)"));

  // ── MQ-2 warm-up notice ─────────────────
  Serial.println(F("Waiting 3 s for MQ-2 sensor warm-up..."));
  // Blink LED during warm-up
  for (uint8_t i = 0; i < 6; i++) {
    digitalWrite(LED_PIN, !digitalRead(LED_PIN));
    delay(500);
  }
  digitalWrite(LED_PIN, LOW);

  // ── Startup beep ─────────────────────────
  triggerAlarm(2, BUZZER_FREQ_HZ, 100, 100);

  Serial.println(F("System ARMED — monitoring started."));
  Serial.println(F("========================================="));
}


// ═════════════════════════════════════════
//   MAIN LOOP
// ═════════════════════════════════════════
void loop() {
  static uint32_t lastLoopTime = 0;

  // Non-blocking interval
  if ((millis() - lastLoopTime) < LOOP_INTERVAL_MS) return;
  lastLoopTime = millis();

  // ── Read sensors ─────────────────────────
  //   Flame sensor: most modules output LOW when flame present
  bool flameDetected = (digitalRead(FLAME_SENSOR_PIN) == LOW);

  //   MQ-2: 12-bit ADC on ESP32 (0–4095)
  int  gasLevel      = analogRead(MQ2_PIN);

  //   Load cell: weight in grams
  float weight = 0.0f;
  if (scale.is_ready()) {
    weight = scale.get_units(SCALE_AVERAGE_READINGS);
    if (weight < 0) weight = 0.0f;   // Clamp negative noise
  } else {
    Serial.println(F("[WARN] HX711 not ready — skipping weight read"));
  }

  bool potPresent        = (weight > POT_WEIGHT_THRESHOLD);
  bool gasLeakDetected   = (gasLevel > GAS_LEAK_THRESHOLD);

  // Print status every loop for monitoring
  printStatus(flameDetected, gasLevel, weight, potPresent);

  // ══════════════════════════════════════
  //   SAFETY LOGIC (priority order)
  // ══════════════════════════════════════

  // ── PRIORITY 1: Gas leak (MQ-2 high, no flame) ──
  //   If gas smell detected without a flame → immediate shutoff
  if (gasLeakDetected && !flameDetected) {
    Serial.println(F("!!! GAS LEAK DETECTED — Emergency shutoff !!!"));
    turnKnobOff();
    triggerAlarm(5, BUZZER_FREQ_HZ, 400, 200);   // Urgent 5-beep alarm
    return;
  }

  // ── PRIORITY 2: Gas leak WITH flame ──
  //   Gas sensor very high even with flame → still dangerous
  if (gasLeakDetected && flameDetected) {
    Serial.println(F("!!! ABNORMAL GAS LEVEL WITH FLAME — Emergency shutoff !!!"));
    turnKnobOff();
    triggerAlarm(5, BUZZER_FREQ_HZ, 400, 200);
    return;
  }

  // ── PRIORITY 3: Gas ON but flame went out (Case 3) ──
  //   Flame sensor sees nothing — stove knob may still be open
  if (!flameDetected && !knobIsOff) {
    Serial.println(F("!!! FLAME OUT — Gas may be ON — Shutting off !!!"));
    turnKnobOff();
    triggerAlarm(3, BUZZER_FREQ_HZ, 300, 200);
    return;
  }

  // ── If knob already off, nothing more to do ──
  if (knobIsOff) {
    digitalWrite(LED_PIN, LOW);
    noPotTimerActive = false;
    return;
  }

  // ── CASE 1 & 2: Flame is ON ──────────────
  if (flameDetected) {
    if (potPresent) {
      // ── Case 1: Normal cooking ──────────
      if (noPotTimerActive) {
        Serial.println(F("Pot returned — timer reset"));
      }
      noPotTimerActive = false;
      noPotStartTime   = 0;
      digitalWrite(LED_PIN, HIGH);    // Solid LED = all OK

    } else {
      // ── Case 2: Flame ON, no pot ────────
      if (!noPotTimerActive) {
        noPotTimerActive = true;
        noPotStartTime   = millis();
        Serial.println(F("WARNING: No pot on stove — 60s timer started"));
      }

      uint32_t elapsed   = millis() - noPotStartTime;
      uint32_t remaining = (NO_POT_TIMEOUT_MS - elapsed) / 1000;

      Serial.print(F("  No-pot timer: "));
      Serial.print(elapsed / 1000);
      Serial.print(F("s / 60s  ("));
      Serial.print(remaining);
      Serial.println(F("s remaining)"));

      // Blink LED as warning
      digitalWrite(LED_PIN, (millis() / 500) % 2);

      // Warning beep every 15 seconds
      if ((elapsed % 15000) < LOOP_INTERVAL_MS) {
        warningBeep();
      }

      // Timeout reached → auto shutoff (Case 2)
      if (elapsed >= NO_POT_TIMEOUT_MS) {
        Serial.println(F("!!! 60s TIMEOUT — No pot — Turning off stove !!!"));
        turnKnobOff();
        triggerAlarm(4, BUZZER_FREQ_HZ, 300, 200);
      }
    }
  }
}


// ═════════════════════════════════════════
//   HELPER FUNCTIONS
// ═════════════════════════════════════════

/**
 * Rotate servo to OFF position and lock state.
 */
void turnKnobOff() {
  if (!knobIsOff) {
    stoveServo.write(SERVO_KNOB_OFF_ANGLE);
    knobIsOff       = true;
    noPotTimerActive = false;
    noPotStartTime   = 0;
    digitalWrite(LED_PIN, LOW);
    Serial.println(F(">>> SERVO → OFF position <<<"));
    delay(1000);   // Let servo reach position before further actions
  }
}

/**
 * Reset the system (e.g., after user manually resets).
 * Call this from Serial command or a reset button interrupt.
 */
void resetSystem() {
  knobIsOff        = false;
  noPotTimerActive = false;
  noPotStartTime   = 0;
  stoveServo.write(SERVO_KNOB_ON_ANGLE);
  digitalWrite(LED_PIN, HIGH);
  Serial.println(F("System RESET — knob returned to ON position"));
  delay(500);
  digitalWrite(LED_PIN, LOW);
}

/**
 * Sound the buzzer N times.
 * @param beeps   Number of beeps
 * @param freqHz  Tone frequency
 * @param onMs    Beep ON duration
 * @param offMs   Beep OFF (gap) duration
 */
void triggerAlarm(uint8_t beeps, uint16_t freqHz, uint16_t onMs, uint16_t offMs) {
  for (uint8_t i = 0; i < beeps; i++) {
    // ESP32 tone() uses ledcWrite internally
    tone(BUZZER_PIN, freqHz, onMs);
    delay(onMs + offMs);
  }
  noTone(BUZZER_PIN);
}

/**
 * Single short warning beep.
 */
void warningBeep() {
  tone(BUZZER_PIN, 1500, 100);
  delay(150);
  noTone(BUZZER_PIN);
}

/**
 * Print a formatted status line to Serial Monitor.
 */
void printStatus(bool flame, int gas, float weight, bool pot) {
  Serial.print(F("[STATUS] Flame:"));
  Serial.print(flame ? F("YES") : F("NO "));

  Serial.print(F("  Gas ADC:"));
  Serial.print(gas);

  Serial.print(F("  Weight:"));
  Serial.print(weight, 1);
  Serial.print(F("g"));

  Serial.print(F("  Pot:"));
  Serial.print(pot ? F("YES") : F("NO "));

  Serial.print(F("  Knob:"));
  Serial.print(knobIsOff ? F("OFF") : F("ON "));

  if (noPotTimerActive) {
    Serial.print(F("  Timer:"));
    Serial.print((millis() - noPotStartTime) / 1000);
    Serial.print(F("s"));
  }

  Serial.println();
}


// ─────────────────────────────────────────
//   OPTIONAL: SERIAL COMMAND HANDLER
//   Open Serial Monitor at 115200 baud
//   Type 'R' + Enter → reset system
//   Type 'T' + Enter → re-tare scale
//   Type 'C' + Enter → print raw ADC values (calibration helper)
// ─────────────────────────────────────────
void serialCommandHandler() {
  if (Serial.available() > 0) {
    char cmd = Serial.read();
    Serial.read();   // consume newline

    switch (cmd) {
      case 'R':
      case 'r':
        Serial.println(F("CMD: Reset system"));
        resetSystem();
        break;

      case 'T':
      case 't':
        Serial.println(F("CMD: Re-taring scale — remove all weight now..."));
        delay(2000);
        scale.tare();
        Serial.println(F("Scale tared."));
        break;

      case 'C':
      case 'c':
        Serial.println(F("CMD: Raw calibration values —"));
        Serial.print(F("  Flame sensor raw: "));
        Serial.println(digitalRead(FLAME_SENSOR_PIN));
        Serial.print(F("  MQ-2 raw ADC:     "));
        Serial.println(analogRead(MQ2_PIN));
        if (scale.is_ready()) {
          Serial.print(F("  Scale raw units:  "));
          Serial.println(scale.get_units(5), 2);
        }
        break;

      default:
        Serial.println(F("Unknown command. Use R=reset, T=tare, C=calibrate"));
        break;
    }
  }
}
