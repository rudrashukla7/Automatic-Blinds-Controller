# Automatic-Blinds-Controller

A automated blinds system using an **ESP32**, **DRV8825 stepper driver**, and a **NEMA 17 stepper motor**.  
The blinds are controlled wirelessly through a built-in web interface, allowing real-time control from any device on the same network.

---

## Features

- WiFi-based control via browser (no app required)
- Clean, responsive web interface hosted on ESP32
- Smooth, quiet motion using 1/16 microstepping
- Manual open / close controls
- Timed movement options (5 seconds)
- Emergency stop functionality
- Multitasking using dual-core ESP32 (motor runs independently of web server)

---

## Components Used

- **ESP32 Development Board (30-pin NodeMCU)**
- **NEMA 17 Stepper Motor (1.5A, 12V)**
- **DRV8825 Stepper Motor Driver**
- **12V battery + buck converter (5V for ESP32)**
- **35V 220µF Capacitor**
- **Jumper wires / connectors**
- **3D printed gear (for #10 bead chain)**
- **Latch switch**

---

## Wiring Diagram

- **Battery +** → DRV8825 VMOT → Buck Converter IN+  
- **Battery −** → DRV8825 GND → Buck IN− → Buck OUT− → ESP32 GND  

- **Buck OUT+ (5V)** → ESP32 VIN  

- **ESP32 GPIO26** → DRV8825 STEP  
- **ESP32 GPIO27** → DRV8825 DIR  
- **ESP32 GPIO25** → DRV8825 EN (LOW = enable)  

- **DRV8825 RST → SLP → ESP32 3.3V**  

- **Microstepping:** M0 = 3.3V, M1 = 3.3V, M2 = GND (1/16 step)  

- **Capacitor:** across VMOT and GND (close to driver)  

- **Motor:**  
  - A1 = Black  
  - A2 = Green  
  - B1 = Red  
  - B2 = Blue  

> Ensure all grounds are connected.

---

## Web Interface

<img width="387" height="591" alt="image" src="https://github.com/user-attachments/assets/995c17a3-b923-4e1d-bfa3-5598e091d076" />

The ESP32 hosts a built-in control panel accessible through your browser.

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
5. Commands trigger a motor task on Core 0  
6. Stepper motor moves blinds via gear + bead chain  
7. Stop command interrupts motion instantly  

---

## Default Settings

- Microstepping: 1/16  
- Step delay: 750 µs  
- Full rotation steps: 32000 (~10 rotations)  
- Timed movement: ~5 seconds  

---

## Libraries Used

- WiFi (ESP32 core)
- WebServer (ESP32 core)

Install them through the ESP32 board package in Arduino IDE.

---

## Uploading the Code

1. Open `Automatic Blinds Controller Code.ino` in Arduino IDE  
2. Select **Board → ESP32 Dev Module**  
3. Select the correct **COM Port**  
4. Enter your WiFi credentials in the code:
   ```cpp
   const char* ssid = "your_wifi";
   const char* password = "your_password";
   ```
5. Click **Upload**

---

## Running the System

1. Power the system with the 12V battery  
2. Open Serial Monitor to find the ESP32 IP address  
3. Enter the IP address into a web browser  
4. Use the interface to control the blinds  

---

## Future Improvements

- Limit switches for top/bottom detection  
- Position tracking (open percentage)
- Mobile app integration
- Smart home integration (Google Home / Alexa)
- Adjustable speed profiles
