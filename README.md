# Automatic-Blinds-Controller

A simple automated blinds system using an **ESP32**, **DRV8825 stepper driver**, and a **NEMA 17 stepper motor**. The blinds are controlled via the ESP32 with precise microstepping for smooth, quiet motion. The gear interfaces with a standard #10 bead chain (4 mm beads, 2 mm spacing).  

---

## Features

- Smooth, quiet blinds motion with 1/16 microstepping  
- Adjustable motor current via DRV8825 Vref (~0.8 V recommended)  
- Compatible with standard #10 bead chains  
- ESP32-based control, easily expandable for WiFi/phone control  

---

## Components Used

- **ESP32S / ESP32 Development Board (30-pin NodeMCU)**  
- **NEMA 17 Stepper Motor (1.5 A, 12 V)**  
- **DRV8825 Stepper Motor Driver**  
- **12 V rechargeable battery + buck converter (5 V to ESP32)**  
- **Optional capacitor** (100 µF recommended near DRV8825 VMOT)  
- **Jumper wires / connectors**  
- **3D printed gear for bead chain**  

---

## Wiring Diagram

- **Battery +** → DRV8825 VMOT  
- **Battery −** → DRV8825 motor GND → Buck converter GND → ESP32 GND  
- **ESP32 GPIO26** → STEP  
- **ESP32 GPIO27** → DIR  
- **ESP32 GPIO25** → EN (LOW = enable)  
- **DRV8825 RST → SLP**  
- **Microstepping pins:** M0=3.3 V, M1=3.3 V, M2=GND  
- **Motor wires:** A1=Black, A2=Green, B1=Red, B2=Blue  

> Ensure all grounds are common to avoid buzzing or misbehavior.  

---

## How It Works

1. Upload the provided test code (`stepper_test.ino`) to the ESP32.  
2. Power the system using a **12 V battery** and a **buck converter** supplying 5 V to the ESP32.  
3. Adjust **DRV8825 Vref** slowly until the motor runs smoothly.  
4. The stepper motor moves the blinds via the 3D printed gear and bead chain.  
5. **Microstepping** is set to 1/16 by default for smooth operation.  

---

## Code Usage

- `stepper_test.ino` contains a basic stepper test  
- Adjust `stepDelay` in microseconds to control blinds speed (typical: 700–800 µs for #10 bead chain)  
- `DIR` pin controls direction of blinds  
- `EN` pin enables/disables the driver  

---

## Gear & Chain Notes

- Chain: #10 bead chain (4 mm beads + 2 mm gap)  
- Gear tooth pitch: 6 mm (matches bead pitch)  
- Ensure precise 3D printed gear design to avoid skipping  

---

## Future Improvements

- Phone/WiFi control using ESP32  
- Limit detection for top/bottom blinds positions  
- Adjustable motor speed profiles  
- Integration with smart home systems 
