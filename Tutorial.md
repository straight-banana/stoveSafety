# ESP32 Stove Safety System Connections

## Components Used

* ESP32 Dev Board
* Flame Sensor
* MQ-2 Gas Sensor
* HX711 Module
* Load Cell
* MG996R Servo Motor
* Buzzer
* LEDs (optional)

# ESP32 Pinout Connections

| Component       | Module Pin | ESP32 Pin           |
| --------------- | ---------- | ------------------- |
| Flame Sensor    | VCC        | 3.3V                |
| Flame Sensor    | GND        | GND                 |
| Flame Sensor    | DO         | GPIO 18             |
| MQ-2 Gas Sensor | VCC        | 5V                  |
| MQ-2 Gas Sensor | GND        | GND                 |
| MQ-2 Gas Sensor | AO         | GPIO 34             |
| HX711           | VCC        | 3.3V                |
| HX711           | GND        | GND                 |
| HX711           | DT         | GPIO 4              |
| HX711           | SCK        | GPIO 5              |
| Servo Motor     | Signal     | GPIO 13             |
| Servo Motor     | VCC        | External 5V         |
| Servo Motor     | GND        | Common GND          |
| Buzzer          | Positive   | GPIO 27             |
| Buzzer          | Negative   | GND                 |
| LED Indicator   | Positive   | GPIO 26             |
| LED Indicator   | Negative   | 220Ω resistor → GND |

# Load Cell to HX711 Connections

Most load cells have 4 wires.

| Load Cell Wire | HX711 Pin |
| -------------- | --------- |
| Red            | E+        |
| Black          | E−        |
| White          | A−        |
| Green          | A+        |

(Some load cells may use different colors.)

# Important Servo Power Connection

Do NOT connect servo power directly to ESP32.

Use:

* External 5V 2A supply

Connections:

| Power Supply | Connect To |
| ------------ | ---------- |
| +5V          | Servo VCC  |
| GND          | Servo GND  |
| GND          | ESP32 GND  |

ESP32 GND and servo GND MUST be connected together.

# Flame Sensor Placement

Place:

* Slightly beside burner
* Facing flame

Avoid:

* Direct heat exposure

# Load Cell Placement

Place:

* Under stove grate/support

Purpose:

* Detect pot weight

# Suggested Working Logic

### Flame Sensor

Detects:

* Fire ON or OFF

### MQ-2 Gas Sensor

Detects:

* LPG leakage

### Load Cell

Detects:

* Pot present or absent

### Servo

Rotates:

* Stove knob OFF automatically

# Recommended GPIO Summary

| GPIO    | Purpose            |
| ------- | ------------------ |
| GPIO 18 | Flame sensor       |
| GPIO 34 | MQ-2 analog output |
| GPIO 4  | HX711 DT           |
| GPIO 5  | HX711 SCK          |
| GPIO 13 | Servo motor        |
| GPIO 27 | Buzzer             |
| GPIO 26 | LED                |

# Basic Power Layout

### ESP32

* Powered by USB

### Servo

* Powered by external 5V supply

### Sensors

* Powered from ESP32 3.3V/5V pins

# Recommended Additional Components

### Protection

* Heat-resistant casing
* Fuse
* Power switch

### Stability

* 1000µF capacitor across servo power
* Separate breadboard power rails

# Simple System Flow

```text id="2cgw6o"
Gas knob ON
    ↓
Check flame sensor

IF no flame:
    Turn servo OFF

ELSE:
    Check load cell

IF no pot for 60 sec:
    Turn servo OFF
    Activate buzzer
```

# Optional Upgrades

### WiFi Alerts

Using ESP32:

* Send phone notification
* Telegram alert
* Blynk app control

### OLED Display

Add:

* 0.96" OLED display

Show:

* Flame status
* Pot status
* Timer
* Gas warning

### Emergency Solenoid Valve

Advanced version:

* Use normally-closed gas solenoid valve

But for beginner projects:

* Servo-controlled knob is safer and easier.
