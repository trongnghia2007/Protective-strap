#include <Wire.h>
#include <MPU6050.h>
#include <SoftwareSerial.h>

MPU6050 mpu;

// Pin definitions
const int BUZZER_PIN = 5;       // Buzzer
const int TRIGGER_PIN = 6;      // Relay/MOSFET for gas release
const int SIM800_TX = 7;        // SIM800L TX
const int SIM800_RX = 8;        // SIM800L RX

SoftwareSerial sim800(SIM800_RX, SIM800_TX);

bool fallDetected = false;

void setup() {
  Serial.begin(9600);
  Wire.begin();

  // Initialize MPU6050
  Serial.println("Initializing MPU6050...");
  mpu.initialize();
  if (!mpu.testConnection()) {
    Serial.println("MPU6050 connection failed!");
    while (1);
  }
  Serial.println("MPU6050 connected.");

  // Pin setup
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(TRIGGER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);
  digitalWrite(TRIGGER_PIN, LOW);

  // Initialize SIM800L
  sim800.begin(9600);
  delay(1000);
  sendCommand("AT");
  sendCommand("AT+CMGF=1"); // SMS mode
}

void loop() {
  int16_t ax, ay, az;
  int16_t gx, gy, gz;
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

  // Calculate inclination angle (rough estimation)
  float angleX = atan2(ay, az) * 180 / PI;
  float angleY = atan2(ax, az) * 180 / PI;

  // Debug
  Serial.print("AngleX: "); Serial.print(angleX);
  Serial.print(" | AngleY: "); Serial.println(angleY);

  // Simple fall detection (threshold-based)
  if (abs(angleX) > 70 || abs(angleY) > 70) { 
    if (!fallDetected) {
      fallDetected = true;
      handleFall();
    }
  } else {
    fallDetected = false;
  }

  delay(200);
}

void handleFall() {
  Serial.println("FALL DETECTED!");

  // 1. Trigger buzzer
  digitalWrite(BUZZER_PIN, HIGH);

  // 2. Trigger gas release (relay/mosfet)
  digitalWrite(TRIGGER_PIN, HIGH);
  // delay(2000); // Keep open for 2s
  digitalWrite(TRIGGER_PIN, LOW);

  // 3. Send SMS alert
  sendSMS("+1234567890", "Emergency! Fall detected. Please check immediately.");

  // 4. Keep buzzer on for alert
  delay(5000);
  digitalWrite(BUZZER_PIN, LOW);
}

void sendCommand(String cmd) {
  sim800.println(cmd);
  delay(500);
  while (sim800.available()) {
    Serial.write(sim800.read());
  }
}

void sendSMS(String number, String text) {
  sendCommand("AT+CMGF=1");
  delay(500);
  sim800.print("AT+CMGS=\"");
  sim800.print(number);
  sim800.println("\"");
  delay(500);
  sim800.print(text);
  delay(500);
  sim800.write(26); // CTRL+Z
  delay(2000);
}
