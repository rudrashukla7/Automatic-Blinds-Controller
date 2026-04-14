# Automatic-Blinds-Controller

A simple automated blinds system using an **ESP32**, **DRV8825 stepper driver**, and a **NEMA 17 stepper motor**. The blinds are controlled via the ESP32 with precise microstepping for smooth, quiet motion. The gear interfaces with a standard #10 bead chain (4 mm beads, 2 mm spacing).  

---

## Features

- Smooth, quiet blinds motion with 1/16 microstepping  
- Adjustable motor current via DRV8825 Vref   
- Compatible with standard #10 bead chains  
- ESP32-based control with WiFi/phone control  

---

## Components Used

- **ESP32S / ESP32 Development Board (30-pin NodeMCU)**  
- **NEMA 17 Stepper Motor (1.5 A, 12 V)**  
- **DRV8825 Stepper Motor Driver**  
- **12 V rechargeable battery + buck converter (5 V to ESP32)**  
- **35V 220uF Electrolytic Capacitor**
- **Jumper wires / connectors**  
- **3D printed gear for housing and bead chain**
- **Latch Switch**

---

## Wiring Diagram

- **Battery +** → DRV8825 VMOT → LM2596 IN+  
- **Battery −** → DRV8825 motor GND → LM2596 IN− → LM2596 OUT− → ESP32 GND → DRV8825 logic GND  

- **LM2596 OUT+ (5V)** → ESP32 VIN (5V pin)  

- **ESP32 GPIO26** → DRV8825 STEP  
- **ESP32 GPIO27** → DRV8825 DIR  
- **ESP32 GPIO25** → DRV8825 EN (LOW = enable)  

- **DRV8825 RST → SLP → ESP32 3.3V**  

- **Microstepping pins:** M0=3.3 V, M1=3.3 V, M2=GND  

- **Capacitor:** + → VMOT, − → DRV8825 motor GND (place close to driver)  

- **Motor wires:** A1=Black, A2=Green, B1=Red, B2=Blue   

> Ensure all grounds are common.  

---

## How It Works

1. Upload the provided test code (`Automatic Blinds Controller Code.ino`) to the ESP32.
2. Note down the **IP Address**
3. Power the system using a **12 V battery** and a **buck converter** supplying 5 V to the ESP32.  
4. Adjust **DRV8825 Vref** slowly until the motor runs smoothly.  
5. The stepper motor moves the blinds via the 3D printed gear and bead chain.  
6. **Microstepping** is set to 1/16 by default for smooth operation.
7. Enter **IP Adress** in search engine to access website.

---

## Future Improvements

- Limit detection for top/bottom blinds positions  
- Adjustable motor speed profiles  
- Integration with smart home systems 
