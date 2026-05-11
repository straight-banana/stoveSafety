# ESP32 Stove Safety System Setup Guide

This tutorial will show you how to:

1. Install Arduino IDE
2. Setup ESP32 support
3. Install required libraries
4. Connect ESP32 to PC
5. Upload code
6. Open Serial Monitor
7. Start testing

Beginner friendly step-by-step guide.

# Step 1 — Download Arduino IDE

Download:

* [Arduino IDE](https://www.arduino.cc/en/software?utm_source=chatgpt.com)

Install normally like any software.

After installation:

* Open Arduino IDE

# Step 2 — Add ESP32 Board Support

## Open Preferences

In Arduino IDE:

```text id="62v5ju"
File → Preferences
```

Find:

```text id="3icynw"
Additional Boards Manager URLs
```

Paste this URL:

```text id="ngynjt"
https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
```

Click:

```text id="n9y3vb"
OK
```

# Step 3 — Install ESP32 Boards

Go to:

```text id="0ct21f"
Tools → Board → Boards Manager
```

Search:

```text id="vffp2o"
esp32
```

Install:

* ESP32 by [Espressif Systems](https://www.espressif.com/?utm_source=chatgpt.com)

Wait until installation finishes.

# Step 4 — Select Your ESP32 Board

Go to:

```text id="z2ymvr"
Tools → Board
```

Choose:

```text id="m5ql59"
ESP32 Dev Module
```

# Step 5 — Connect ESP32 to PC

Use:

* USB cable

IMPORTANT:
Some USB cables are charging-only.

If ESP32 does not appear:

* Try another cable

# Step 6 — Select COM Port

Go to:

```text id="k7n4dm"
Tools → Port
```

Select:

```text id="hlg7k2"
COMx
```

Example:

```text id="l13mbs"
COM3
COM5
COM7
```

If no port appears:

* Reconnect ESP32
* Change cable
* Install USB driver

# Step 7 — Test ESP32 with Blink Program

Create new file:

```text id="zy2i6l"
File → New
```

Paste this code:

```cpp id="z8qzq8"
void setup()
{
    pinMode(2, OUTPUT);
}

void loop()
{
    digitalWrite(2, HIGH);
    delay(1000);

    digitalWrite(2, LOW);
    delay(1000);
}
```

# Step 8 — Upload Code

Click:

```text id="7r0nbi"
Upload Button (→)
```

Bottom should show:

```text id="z99zzr"
Done Uploading
```

If upload fails:

* Hold BOOT button while uploading

# Step 9 — Open Serial Monitor

Go to:

```text id="8qomcs"
Tools → Serial Monitor
```

Set baud rate:

```text id="iyscik"
115200
```

You will see messages from ESP32.

# Step 10 — Install Required Libraries

Go to:

```text id="kr7knw"
Sketch → Include Library → Manage Libraries
```

Install these:

## 1. HX711

Search:

```text id="d2q4js"
HX711
```

Install:

* HX711 by Bogde

## 2. ESP32Servo

Search:

```text id="o9d4pv"
ESP32Servo
```

Install:

* ESP32Servo by Kevin Harrington

# Step 11 — Upload Stove Safety Code

Create new sketch.

Paste the Smart Stove Safety System code.

Click:

```text id="n6bygs"
Upload
```

Wait for:

```text id="5v5tn4"
Done Uploading
```

# Step 12 — Wiring Setup

## Flame Sensor

| Flame Sensor | ESP32   |
| ------------ | ------- |
| VCC          | 3.3V    |
| GND          | GND     |
| DO           | GPIO 18 |

## MQ-2 Gas Sensor

| MQ-2 | ESP32   |
| ---- | ------- |
| VCC  | 5V      |
| GND  | GND     |
| AO   | GPIO 34 |

## HX711

| HX711 | ESP32  |
| ----- | ------ |
| DT    | GPIO 4 |
| SCK   | GPIO 5 |
| VCC   | 3.3V   |
| GND   | GND    |

## Servo

| Servo  | ESP32       |
| ------ | ----------- |
| Signal | GPIO 13     |
| VCC    | External 5V |
| GND    | Common GND  |

# Step 13 — Testing

## Flame Test

Use lighter/fire.

Serial Monitor should show:

```text id="kjjowm"
Flame: YES
```

## Pot Detection Test

Place object on load cell.

Should show:

```text id="j5eb4h"
Pot Present: YES
```

## Gas Sensor Test

Bring gas/lighter gas near MQ-2.

Should show:

```text id="5t7j3y"
Gas Leak: YES
```

# Step 14 — Servo Calibration

If servo rotates wrong direction:

Change:

```cpp id="ylp4q5"
SERVO_ON_POSITION
SERVO_OFF_POSITION
```

Example:

```cpp id="p6s3a7"
int SERVO_ON_POSITION = 0;
int SERVO_OFF_POSITION = 90;
```

# Common Problems

## Upload Failed

Try:

* Hold BOOT button
* Change USB cable
* Reconnect ESP32

## Servo Not Moving

Check:

* External power supply
* Common GND
* Signal pin

## Random ESP32 Restart

Usually caused by:

* Servo drawing too much power

Solution:

* Separate 5V supply

# Recommended Next Steps

After basic setup works:

* Add OLED display
* Add WiFi notifications
* Add Telegram alerts
* Build enclosure
* Design PCB
* Add mobile app

# Useful Official Resources

* [Arduino Documentation](https://docs.arduino.cc/?utm_source=chatgpt.com)
* [ESP32 Arduino Core GitHub](https://github.com/espressif/arduino-esp32?utm_source=chatgpt.com)
* [ESP32Servo Library](https://github.com/madhephaestus/ESP32Servo?utm_source=chatgpt.com)
* [HX711 Library](https://github.com/bogde/HX711?utm_source=chatgpt.com)
