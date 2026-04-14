# Automatic-Blinds-Controller

A WiFi-enabled automated blinds system using an **ESP32**, **DRV8825 stepper driver**, and a **NEMA 17 stepper motor**.  
The system hosts a built-in web interface that allows real-time control of blinds from any device on the same network.

---

## Features

- Web-based control (no app required)
- Clean, responsive UI hosted directly on ESP32
- Smooth motion using 1/16 microstepping
- Manual open / close controls
- Timed movement (5-second presets)
- Instant stop functionality
- Dual-core multitasking (motor control runs independently)
- Quiet and precise operation

---

## Components Used

- ESP32 Development Board (30-pin NodeMCU)
- NEMA 17 Stepper Motor (12V, 1.5A)
- DRV8825 Stepper Motor Driver
- 12V battery or power supply
- Buck converter (5V for ESP32)
- 220µF capacitor (≥35V recommended)
- Jumper wires
- 3D printed gear (for #10 bead chain)
- Latch switch

---

## Wiring Diagram

- **Battery +** → DRV8825 VMOT → Buck Converter IN+  
- **Battery −** → DRV8825 GND → Buck IN− → Buck OUT− → ESP32 GND  

- **Buck OUT+ (5V)** → ESP32 VIN  

- **ESP32 GPIO26** → DRV8825 STEP  
- **ESP32 GPIO27** → DRV8825 DIR  
- **ESP32 GPIO25** → DRV8825 EN (LOW = enable)  

- **DRV8825 RST → SLP → ESP32 3.3V**  

- **Microstepping Configuration:**  
  - M0 = 3.3V  
  - M1 = 3.3V  
  - M2 = GND  → (1/16 microstepping)

- **Capacitor:** across VMOT and GND (placed close to driver)

- **Motor Wiring:**  
  - A1 = Black  
  - A2 = Green  
  - B1 = Red  
  - B2 = Blue  

> All grounds must be connected together for proper operation.

---

## Web Interface

<img width="387" height="591" alt="Screenshot 2026-04-13 225331" src="https://github.com/user-attachments/assets/e3df4faa-b7a6-4c7a-9120-dad941d04165" />

The ESP32 hosts a web dashboard accessible through its IP address.

### Controls

- **Open / Close** → Full movement  
- **Open 5s / Close 5s** → Timed movement  
- **Stop** → Immediately halts motor  

---

## How It Works

1. ESP32 connects to WiFi using provided credentials  
2. A web server runs on port 80  
3. User accesses the ESP32 IP address in a browser  
4. Button presses send HTTP requests to the ESP32  
5. A motor task is created on Core 0  
6. Stepper motor is driven via STEP/DIR signals  
7. Stop command interrupts motion using a shared flag  

The DRV8825 driver converts STEP and DIR signals into precise motor movement, allowing smooth control with microstepping :contentReference[oaicite:0]{index=0}.

---

## Default Settings

- Microstepping: 1/16  
- Step delay: 750 µs  
- Full movement: ~10 rotations (32000 steps)  
- Timed movement: ~5 seconds (3333 steps)  

---

## 3D Printed Parts

Custom parts are used to interface with the blinds system.

### Files

- `blinds_gear.stl` – engages with #10 bead chain  
- `housing.stl` – enclosure for electronics
- `lid.stl` – close the housing  

### Notes

- Designed for #10 bead chain (4mm beads, 2mm spacing)
- Recommended material: PLA or PETG
- Ensure proper fit before installation

---

## Libraries Used

- WiFi (ESP32 core)
- WebServer (ESP32 core)

Included automatically with the ESP32 Arduino package.

---

## Uploading the Code

1. Open `Automatic Blinds Controller Code.ino` in Arduino IDE  
2. Select **Board → ESP32 Dev Module**  
3. Select the correct **COM Port**  
4. Update WiFi credentials:
   ```cpp
   const char* ssid = "your_wifi";
   const char* password = "your_password";
   ```
5. Click **Upload**

---

## Running the System

1. Power the system with a 12V supply  
2. Open Serial Monitor (115200 baud)  
3. Note the ESP32 IP address  
4. Enter the IP address into a browser  
5. Use the interface to control blinds  

---

## Future Improvements

- Limit switches for position detection  
- Position tracking (percentage open/closed)  
- Mobile app or remote access  
- Smart home integration (Alexa / Google Home)  
- Adjustable speed profiles  
