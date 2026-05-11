# Smart Stove Safety System

An IoT-based stove safety system using ESP32 that detects unsafe cooking conditions and automatically turns off the stove knob to prevent gas leaks, fire hazards, and wasted gas.

## Features

- Detects stove flame
- Detects gas leakage
- Detects pot/pan presence
- Automatically turns OFF stove if:
  - Gas is ON but flame is OFF
  - Pot is removed for more than 1 minute
- Buzzer warning system
- Servo-controlled stove knob
- ESP32-based smart controller

---

# Components Used

## Microcontroller
- ESP32 Dev Board

## Sensors
- Flame Sensor Module
- MQ-2 / MQ-5 Gas Sensor
- Load Cell
- HX711 Amplifier Module

## Output Devices
- MG996R Servo Motor
- Buzzer
- LED Indicator

## Power
- 5V 2A External Power Supply

---

# System Logic

## Case 1
Gas ON + Flame ON + Pot Present

Result:
- Normal operation

## Case 2
Gas ON + Flame ON + Pot Removed

Result:
- Start timer
- If no pot for 60 seconds:
  - Rotate knob OFF
  - Activate buzzer

## Case 3
Gas ON + No Flame

Result:
- Immediately rotate knob OFF
- Activate buzzer

---

# Pin Connections

| Component | Module Pin | ESP32 Pin |
|---|---|---|
| Flame Sensor | DO | GPIO 18 |
| MQ-2 Sensor | AO | GPIO 34 |
| HX711 DT | DT | GPIO 4 |
| HX711 SCK | SCK | GPIO 5 |
| Servo Signal | SIG | GPIO 13 |
| Buzzer | + | GPIO 27 |
| LED | + | GPIO 26 |

---

# Load Cell Wiring

| Load Cell Wire | HX711 Pin |
|---|---|
| Red | E+ |
| Black | E− |
| White | A− |
| Green | A+ |

---

# Power Connections

## ESP32
- Powered using USB

## Servo Motor
- Powered using external 5V supply

IMPORTANT:
- Do NOT power servo directly from ESP32
- Connect all GND lines together

---

# Working Flow

```text
START

Read flame sensor
Read gas sensor
Read load cell

IF gas ON AND flame OFF:
    Turn stove OFF
    Activate buzzer

IF gas ON AND flame ON:
    IF pot absent:
        Start timer

        IF timer > 60 sec:
            Turn stove OFF
            Activate buzzer

    ELSE:
        Reset timer
