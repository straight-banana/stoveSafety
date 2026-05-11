/*
===========================================
 Smart Stove Safety System
 ESP32 + Flame Sensor + MQ2 + HX711 + Servo
===========================================

Features:
- Detect flame
- Detect gas leakage
- Detect pot presence
- Auto turn OFF stove if:
    1. Flame OFF while gas ON
    2. Pot absent for 60 seconds
- Activate buzzer warning

Libraries Required:
1. HX711 Library
2. ESP32Servo Library

Install from Arduino Library Manager.
*/

#include <HX711.h>
#include <ESP32Servo.h>



// ===========================================
// PIN DEFINITIONS
// ===========================================

#define FLAME_PIN      18
#define GAS_SENSOR_PIN 34

#define HX711_DT       4
#define HX711_SCK      5

#define SERVO_PIN      13

#define BUZZER_PIN     27
#define LED_PIN        26



// ===========================================
// OBJECTS
// ===========================================

HX711 scale;
Servo stoveServo;



// ===========================================
// SETTINGS
// ===========================================

// Weight threshold for pot detection
float POT_THRESHOLD = 200.0;

// Gas threshold
int GAS_THRESHOLD = 1200;

// Servo angles
int SERVO_ON_POSITION  = 90;
int SERVO_OFF_POSITION = 0;

// Pot timeout
unsigned long POT_TIMEOUT = 60000; // 60 sec

// HX711 calibration factor
float calibration_factor = -7050;



// ===========================================
// VARIABLES
// ===========================================

bool flameDetected = false;
bool gasDetected   = false;
bool potPresent    = false;

unsigned long potRemovedTime = 0;
bool timerStarted = false;



// ===========================================
// SETUP
// ===========================================

void setup()
{
    Serial.begin(115200);

    pinMode(FLAME_PIN, INPUT);
    pinMode(BUZZER_PIN, OUTPUT);
    pinMode(LED_PIN, OUTPUT);

    digitalWrite(BUZZER_PIN, LOW);
    digitalWrite(LED_PIN, LOW);



    // Servo setup
    stoveServo.attach(SERVO_PIN);
    stoveServo.write(SERVO_ON_POSITION);



    // HX711 setup
    scale.begin(HX711_DT, HX711_SCK);
    scale.set_scale(calibration_factor);
    scale.tare();



    Serial.println("Smart Stove Safety System Started");
}



// ===========================================
// LOOP
// ===========================================

void loop()
{
    readSensors();

    printStatus();

    checkSafetyLogic();

    delay(500);
}



// ===========================================
// READ SENSORS
// ===========================================

void readSensors()
{
    // -------------------------------
    // Flame Sensor
    // LOW = flame detected
    // -------------------------------
    flameDetected = (digitalRead(FLAME_PIN) == LOW);



    // -------------------------------
    // Gas Sensor
    // -------------------------------
    int gasValue = analogRead(GAS_SENSOR_PIN);

    gasDetected = (gasValue > GAS_THRESHOLD);



    // -------------------------------
    // Load Cell
    // -------------------------------
    float weight = scale.get_units(5);

    if(weight < 0)
        weight = 0;

    potPresent = (weight > POT_THRESHOLD);
}



// ===========================================
// SAFETY LOGIC
// ===========================================

void checkSafetyLogic()
{
    // =====================================
    // CASE 1:
    // GAS PRESENT BUT NO FLAME
    // =====================================

    if(gasDetected && !flameDetected)
    {
        Serial.println("WARNING: Gas detected but no flame!");

        emergencyShutdown();

        return;
    }



    // =====================================
    // CASE 2:
    // FLAME EXISTS BUT POT REMOVED
    // =====================================

    if(flameDetected)
    {
        if(!potPresent)
        {
            if(!timerStarted)
            {
                timerStarted = true;
                potRemovedTime = millis();

                Serial.println("Pot removed. Timer started.");
            }

            // Check timeout
            if(millis() - potRemovedTime >= POT_TIMEOUT)
            {
                Serial.println("Pot absent too long!");

                emergencyShutdown();

                return;
            }
        }
        else
        {
            // Pot returned
            timerStarted = false;
        }
    }
}



// ===========================================
// EMERGENCY SHUTDOWN
// ===========================================

void emergencyShutdown()
{
    // Rotate stove OFF
    stoveServo.write(SERVO_OFF_POSITION);

    // Alarm
    digitalWrite(BUZZER_PIN, HIGH);
    digitalWrite(LED_PIN, HIGH);

    Serial.println("STOVE TURNED OFF!");

    delay(5000);

    digitalWrite(BUZZER_PIN, LOW);
}



// ===========================================
// SERIAL MONITOR STATUS
// ===========================================

void printStatus()
{
    Serial.println("------------");

    Serial.print("Flame: ");
    Serial.println(flameDetected ? "YES" : "NO");

    Serial.print("Gas Leak: ");
    Serial.println(gasDetected ? "YES" : "NO");

    Serial.print("Pot Present: ");
    Serial.println(potPresent ? "YES" : "NO");

    Serial.println("------------");
}
