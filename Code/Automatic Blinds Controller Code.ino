// Automatic Blinds Controller
// NEMA 17, DRV8825, 1/16 microstepping, ESP32

#include <WiFi.h>
#include <WebServer.h>

// WiFi Credentials
const char* ssid = "test";      //add your home internet name and password
const char* password = "1234";

// Pin Definitions
#define STEP 26
#define DIR 27
#define EN 25

// 1/16 microstepping
#define STEP_DELAY 750
#define STEPS_PER 32000  // 10 rotations 
#define STEPS_5SEC 3333   // ~5 seconds of movement

WebServer server(80);

// Shared between cores
volatile bool stopFlag = false;
volatile bool motorActive = false;

//Motor Command
struct MotorCommand {
  int  steps;
  bool clockwise;
};

MotorCommand pendingCmd = {0, true};
TaskHandle_t motorTaskHandle = NULL;

// HTML Page 
const char htmlPage[] PROGMEM = R"=====(
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>Blinds</title>
  <link rel="preconnect" href="https://fonts.googleapis.com">
  <link href="https://fonts.googleapis.com/css2?family=Inter:wght@300;400;500;600&display=swap" rel="stylesheet">
  <style>
    * {
      box-sizing: border-box;
      margin: 0;
      padding: 0;
    }

    body {
      font-family: 'Inter', sans-serif;
      background: #080808;
      color: #f0f0f0;
      min-height: 100vh;
      display: flex;
      flex-direction: column;
      align-items: center;
      justify-content: center;
      padding: 24px;
    }

    .card {
      background: #111111;
      border: 1px solid #1f1f1f;
      border-radius: 28px;
      padding: 36px 28px 28px;
      width: 100%;
      max-width: 340px;
    }

    /*  Header  */
    .header {
      text-align: center;
      margin-bottom: 32px;
    }

    .blind-icon {
      width: 48px;
      height: 48px;
      margin: 0 auto 14px;
      display: flex;
      flex-direction: column;
      justify-content: space-between;
    }

    .blind-slat {
      height: 5px;
      border-radius: 3px;
      background: #2a2a2a;
    }

    /* Staggered slats — top one highlighted in yellow */
    .blind-slat:nth-child(1) { background: #f5c842; width: 100%; }
    .blind-slat:nth-child(2) { width: 85%; }
    .blind-slat:nth-child(3) { width: 70%; }
    .blind-slat:nth-child(4) { width: 55%; }
    .blind-slat:nth-child(5) { width: 40%; }

    h1 {
      font-size: 1.2rem;
      font-weight: 500;
      color: #ffffff;
      letter-spacing: 0.02em;
    }

    .subtitle {
      font-size: 0.75rem;
      color: #444;
      margin-top: 5px;
      letter-spacing: 0.03em;
    }

    /*  Section Labels  */
    .section-label {
      font-size: 0.65rem;
      font-weight: 600;
      letter-spacing: 0.14em;
      color: #333;
      text-transform: uppercase;
      margin-bottom: 10px;
    }

    /*  Buttons  */
    .btn-row {
      display: flex;
      gap: 8px;
      margin-bottom: 8px;
    }

    button {
      flex: 1;
      padding: 15px 10px;
      font-size: 0.88rem;
      font-weight: 500;
      font-family: 'Inter', sans-serif;
      border: none;
      border-radius: 14px;
      cursor: pointer;
      transition: opacity 0.12s ease, transform 0.12s ease;
      letter-spacing: 0.02em;
    }

    button:active {
      transform: scale(0.95);
      opacity: 0.85;
    }

    .btn-open {
      background: #f5c842;
      color: #111;
    }

    .btn-open:hover {
      background: #f9d45a;
    }

    .btn-close {
      background: #1a1a1a;
      color: #999;
      border: 1px solid #252525;
    }

    .btn-close:hover {
      background: #1f1f1f;
    }

    /* Distinct red-tinted style to stand out from other buttons */
    .btn-stop {
      width: 100%;
      display: block;
      padding: 16px;
      background: #1e0a09;
      color: #e05252;
      border: 1px solid #2e1010;
      font-size: 0.92rem;
      border-radius: 14px;
      margin-bottom: 8px;
      cursor: pointer;
      font-family: 'Inter', sans-serif;
      font-weight: 500;
      transition: opacity 0.12s ease, transform 0.12s ease;
      letter-spacing: 0.02em;
    }

    .btn-stop:hover {
      background: #261010;
    }

    .btn-stop:active {
      transform: scale(0.95);
      opacity: 0.85;
    }

    .btn-timed {
      background: #0d1520;
      color: #5a9fd4;
      border: 1px solid #162030;
      font-size: 0.82rem;
    }

    .btn-timed:hover {
      background: #111c2a;
    }

    /*  Divider  */
    .divider {
      border: none;
      border-top: 1px solid #191919;
      margin: 18px 0;
    }

    /*  Status Bar  */
    .status-bar {
      background: #0c0c0c;
      border: 1px solid #1a1a1a;
      border-radius: 12px;
      padding: 11px 14px;
      margin-top: 16px;
      display: flex;
      align-items: center;
      gap: 10px;
    }

    /* Dot changes colour based on state: idle / active / error */
    .dot {
      width: 7px;
      height: 7px;
      border-radius: 50%;
      background: #252525;
      flex-shrink: 0;
      transition: background 0.4s ease;
    }

    .dot.active { background: #f5c842; }
    .dot.error { background: #e05252; }

    #status {
      font-size: 0.75rem;
      color: #3a3a3a;
      transition: color 0.3s;
    }

    #status.active { color: #888; }
    #status.error { color: #e05252; }

    /*  Footer  */
    .footer {
      text-align: center;
      margin-top: 20px;
      font-size: 0.65rem;
      color: #222;
      letter-spacing: 0.06em;
    }
  </style>
</head>
<body>
  <div class="card">

    <div class="header">
      <div class="blind-icon">
        <div class="blind-slat"></div>
        <div class="blind-slat"></div>
        <div class="blind-slat"></div>
        <div class="blind-slat"></div>
        <div class="blind-slat"></div>
      </div>
      <h1>Blinds Control</h1>
      <p class="subtitle">AUTOMATIC BLINDS CONTROLLER V2</p>
    </div>

    <p class="section-label">Manual</p>
    <div class="btn-row">
      <button class="btn-open"  onclick="send('/motor/cw','Opening...','active')">&#x2191; Open</button>
      <button class="btn-close" onclick="send('/motor/ccw','Closing...', 'active')">&#x2193; Close</button>
    </div>
    <button class="btn-stop" onclick="send('/motor/stop', 'Stopping...', 'error')">&#x25A0;&nbsp; Stop</button>

    <hr class="divider">

    <p class="section-label">Timed &mdash; 5 seconds</p>
    <div class="btn-row">
      <button class="btn-timed" onclick="send('/motor/cw5','Opening for 5s...', 'active')">&#x2191; Open 5s</button>
      <button class="btn-timed" onclick="send('/motor/ccw5', 'Closing for 5s...', 'active')">&#x2193; Close 5s</button>
    </div>

    <div class="status-bar">
      <div class="dot" id="dot"></div>
      <span id="status">Ready</span>
    </div>

    <p class="footer">ESP32 &bull; DRV8825 &bull; NEMA 17</p>

  </div>

  <script>
    // Sends a request to the ESP32 and updates the status bar with the response
    async function send(path, msg, state) {
      setStatus(msg, state);
      try {
        const res= await fetch(path);
        const txt = await res.text();
        setStatus(txt, '');
      } catch(e) {
        setStatus('Connection error', 'error');
      }
    }

    // Updates status text and dot colour
    function setStatus(msg, state) {
      const el= document.getElementById('status');
      const dot = document.getElementById('dot');
      el.textContent = msg;
      el.className = state || '';
      dot.className = 'dot' + (state ? ' ' + state : '');
    }
  </script>
</body>
</html>
)=====";

// Motor Task Core 0 
void motorTask(void* param) {
  disableCore0WDT(); // prevent watchdog from stopping long moves

  MotorCommand* cmd = (MotorCommand*)param;
  int  steps     = cmd->steps;
  bool clockwise = cmd->clockwise;

  digitalWrite(DIR, clockwise ? HIGH : LOW);
  delay(1); 
  digitalWrite(EN, LOW);

  for (int i = 0; i < steps; i++) {
    if (stopFlag) break; 
    digitalWrite(STEP, HIGH);
    delayMicroseconds(STEP_DELAY);
    digitalWrite(STEP, LOW);
    delayMicroseconds(STEP_DELAY);
  }

  // disable driver 
  digitalWrite(EN, HIGH); 
  motorActive = false;
  motorTaskHandle = NULL;

  // task deletes itself
  vTaskDelete(NULL); 
}

// Start motorTask Core 0
void startMotor(int steps, bool clockwise) {

  //Stops active movement
  if (motorActive) {
    stopFlag = true;

    // Let motor spin
    while (motorActive){
      delay(10); 
    } 
  }

  // Reset values
  stopFlag = false;
  motorActive = true;
  delay(10);

  pendingCmd = {steps, clockwise};

  xTaskCreatePinnedToCore(
    motorTask,       // function
    "motorTask",     // name
    2048,            // stack size
    &pendingCmd,     // argument
    1,               // priority
    &motorTaskHandle,// handle
    0                // core 0
  );
}

// Route Handlers
// Each sends a response immediately so the browser doesn't retry

void handleRoot() {
  server.send_P(200, "text/html", htmlPage); // send from flash, not RAM
}

void handleCW() {
  server.send(200, "text/plain", "Opening");
  startMotor(STEPS_PER, true);
}

void handleCCW() {
  server.send(200, "text/plain", "Closing");
  startMotor(STEPS_PER, false);
}

void handleCW5() {
  server.send(200, "text/plain", "Opening for 5s");
  startMotor(STEPS_5SEC, true);
}

void handleCCW5() {
  server.send(200, "text/plain", "Closing for 5s");
  startMotor(STEPS_5SEC, false);
}

void handleStop() {
  stopFlag = true;
  server.send(200, "text/plain", "Stopped");
  stopFlag = false; // clear flag so next command starts fresh
}

void handleNotFound() {
  server.send(404, "text/plain", "Not found");
}

// Setup
void setup() {
  Serial.begin(115200);

  pinMode(STEP, OUTPUT);
  pinMode(DIR,OUTPUT);
  pinMode(EN,OUTPUT);
  digitalWrite(EN, HIGH);

  Serial.print("Connecting to WiFi");
  WiFi.mode(WIFI_STA); // station mode
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected! IP: " + WiFi.localIP().toString());

  // Register URL routes
  server.on("/", handleRoot);
  server.on("/motor/cw", handleCW);
  server.on("/motor/ccw", handleCCW);
  server.on("/motor/stop", handleStop);
  server.on("/motor/cw5", handleCW5);
  server.on("/motor/ccw5", handleCCW5);
  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("Web server started.");
}

// Void loop
void loop() {
  server.handleClient();
}
