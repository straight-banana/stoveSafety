### 1. Wireless Kitchen Fire Prevention System Using Electrochemical CO₂ Gas Sensor for Smart Home
**Authors:** Kweon, S.-J., Park, J.-H., Park, C.-O., Yoo, H.-J., & Ha, S.
**Journal:** *MDPI Sensors*, Vol. 22, No. 11, 2022
**DOI:** [10.3390/s22113965](https://doi.org/10.3390/s22113965)
**Link:** https://pmc.ncbi.nlm.nih.gov/articles/PMC9182822/

**Summary:** Presents a two-module wireless system — a sensor module that detects CO₂ near the gas stove and transmits results wirelessly, and an alarm module placed elsewhere that alerts the user. The sensor module uses a cost-efficient electrochemical CO₂ sensor and an in-situ algorithm to determine stove status, with on-off keying (OOK) wireless communication to extend battery life. Highly relevant as it validates sensor-based stove state detection without directly controlling gas valves.

---

### 2. The Gas Fire Temperature Measurement for Detection of an Object's Presence on Top of the Burner
**Authors:** Milecki, A. & Rybarczyk, D.
**Journal:** *MDPI Sensors*, Vol. 20, No. 7, 2020
**DOI:** [10.3390/s20072139](https://doi.org/10.3390/s20072139)
**Link:** https://www.mdpi.com/1424-8220/20/7/2139

**Summary:** Investigates temperature measurement above a gas burner to recognize pot removal and subsequently cut off gas supply. Tests thermocouple and IR diode sensors to find optimal placement, ultimately proposing a microprocessor-based algorithm for pot-absence recognition over the burner. Directly validates the load-cell-based pot detection concept in your system with a complementary thermal approach.

---

### 3. Development of a Low-Cost Low-Energy Intelligent Reminder System for Unextinguished Gas Stoves
**Authors:** Tamaki, M. & Premachandra, C.
**Journal:** *IEEE Consumer Electronics Magazine*, Vol. 10, No. 3, pp. 29–33, 2021
**DOI:** [10.1109/MCE.2020.3023004](https://doi.org/10.1109/MCE.2020.3023004)
**Link:** https://ieeexplore.ieee.org/document/9194284/

**Summary:** Proposes a smart reminder system for unattended gas stoves. Unlike conventional systems, it does not immediately extinguish the flame when the stove is left unattended — instead it monitors for user presence and produces periodic alerts, and only extinguishes fire if the user does not return. Mirrors your Case 2 logic (no-pot 60-second timer with escalating alarm before shutoff).

---

### 4. Automatic Gas OFF Mechanism for LPG Gas Stove
**Authors:** Bangera, S. D., Mohan, K., & Vinod Raj
**Journal:** *IJSRCSEIT*, Vol. 7, Issue 3, pp. 543–546, 2021
**DOI:** [10.32628/CSEIT2173119](https://doi.org/10.32628/CSEIT2173119)
**Link:** https://ijsrcseit.com/home/issue/view/article.php?id=CSEIT2173119

**Summary:** Designs a microcontroller-based embedded system using an Arduino Mega, ultrasonic sensor, and servo motor to automatically rotate the gas knob OFF when no cooking pot is detected on the stove grate, preventing gas misuse. Closest architecture to your project — same servo-knob-turn approach combined with object detection.

---

### 5. Automatic Gas Cooker Control System
**Authors:** Olaniyi, O. M. et al.
**Journal:** *IJAREEIE*, Vol. 6, Issue 7, 2017
**DOI:** [10.15662/IJAREEIE.2017.0607002](https://doi.org/10.15662/IJAREEIE.2017.0607002)
**Link:** https://www.ijareeie.com/upload/2017/july/2_Automatic_n.pdf

**Summary:** Proposes an Arduino Uno-based system incorporating a servo motor, flame sensor, utensil sensor, and buzzer alarm. During ignition mode the system checks whether a utensil is placed on the burner via a push-button utensil sensor, and takes action if the stove is unattended. One of the earliest academic implementations of the exact hardware combination (Arduino + servo + flame sensor + utensil detection) you are building.

---

### 6. Intelligent IoT-Based Fire and Gas Leakage Detection System: Integrating Sensor Fusion, Edge AI, and Visual Verification
**Journal:** *IJERT*, Vol. 15, Issue 4, 2025
**Link:** https://www.ijert.org/intelligent-iot-based-fire-and-gas-leakage-detection-system-integrating-sensor-fusion-edge-ai-and-visual-verification-ijertv15is041584

**Summary:** Uses an ESP32 microcontroller to process data from a multi-sensor array (gas, flame, temperature) via a sensor fusion algorithm, achieving high accuracy in hazardous event prediction. Integrates an ESP32-CAM module for real-time visual confirmation of leakage or fire, reducing false positives — for example, distinguishing a cigarette's smoke from an actual fire. Excellent reference for upgrading your system with sensor fusion and camera-based verification.

---

### 7. LPG Smart Guard: An IoT-Based Solution for Real-Time Gas Cylinder Monitoring and Safety in Smart Homes
**Journal:** *MDPI Engineering Proceedings*, Vol. 82, No. 1, 2024
**DOI:** [10.3390/engproc82010009](https://doi.org/10.3390/engproc82010009)
**Link:** https://www.mdpi.com/2673-4591/82/1/9

**Summary:** Presents an IoT-based LPG monitoring system using an MQ135 gas sensor for leakage detection, a load cell to monitor cylinder weight, and a DHT22 temperature sensor — all connected to a NodeMCU microcontroller that transmits data to a cloud MySQL database via HTTP. Directly validates the load cell + MQ gas sensor combination that your system uses, and shows how to extend it to cloud monitoring.

---

### 8. The Smart Gas Leakage Detection with Monitoring and LPG Weight Measurement
**Journal:** *IARJSET*, Vol. 8, Issue 6, 2021
**DOI:** [10.17148/IARJSET.2021.86158](https://doi.org/10.17148/IARJSET.2021.86158)
**Link:** https://iarjset.com/wp-content/uploads/2021/07/IARJSET.2021.86158.pdf

**Summary:** Uses an MQ-6 gas sensor with a SnO₂ electrochemical membrane to detect propane and butane, converting gas concentration to voltage sent to a microcontroller. Combines this with a strain gauge load cell (CZL-601) for LPG cylinder weight monitoring and displays alerts on an LCD and mobile app. Validates the dual-sensor approach (MQ gas + load cell) at the core of your design.

---

### 9. Automatic Gas Cooking Control System Based on Microcontroller
**Journal:** *IJERT*, Vol. 5, Issue 2, 2016
**DOI:** [10.17577/IJERTV5IS020221](https://doi.org/10.17577/IJERTV5IS020221)
**Link:** https://www.ijert.org/research/automatic-gas-cooking-control-system-based-on-microcontroller-IJERTV5IS020221.pdf

**Summary:** Implements a microcontroller system that uses a servo motor to open and close a gas valve at precise angles (45°, 90°, 135°, 180°) corresponding to gas flow levels. In the event of gas leakage, the microcontroller signals the buzzer and servo to immediately rotate to 0° to close the valve. A foundational reference for the servo-controlled gas knob shutoff mechanism central to your build.

---

### 10. IoT-Based Smart Fire Detection System Using ESP32
**Journal:** *ResearchGate / IJASRE*, 2025
**DOI:** [10.31695/IJASRE.2025.9.1.1](https://doi.org/10.31695/IJASRE.2025.9.1.1)
**Link:** https://www.researchgate.net/publication/388173172_IoT-Based_Smart_Fire_Detection_System_using_ESP32

**Summary:** Combines an LM393 flame sensor, DHT11 temperature/humidity sensor, and MQ-2 smoke sensor interfaced with an ESP32 microcontroller. Two seconds after sensors detect a fire, the buzzer and LED automatically activate, information is displayed on an LCD, and real-time alerts are pushed to users via a mobile application. A direct ESP32 implementation reference that mirrors your exact hardware stack (MQ-2 + flame sensor + ESP32 + buzzer + LED).

---

> **Tip for your project report:** Papers #2, #3, and #4 are the strongest citations for justifying your three safety cases (pot detection, unattended stove timer, and immediate gas-off shutoff). Papers #1 and #6 support future upgrade paths like WiFi alerts and sensor fusion.
