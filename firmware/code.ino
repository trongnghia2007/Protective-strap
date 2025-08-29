#include <Wire.h>
#include <MPU6050.h>

MPU6050 mpu;

// Pin definitions (chỉnh lại cho ESP32)
const int BUZZER_PIN = 25;
const int TRIGGER_PIN = 26;
const int SIM800_TX = 17;   // ESP32 TX -> SIM800 RX
const int SIM800_RX = 16;   // ESP32 RX -> SIM800 TX

HardwareSerial sim800(1);
bool fallDetected = false;

void setup() {
  Serial.begin(115200);
  Wire.begin();

  Serial.println("Initializing MPU6050...");
  mpu.initialize();
  if (!mpu.testConnection()) {
    Serial.println("MPU6050 connection failed!");
    while (1);
  }
  Serial.println("MPU6050 connected.");

  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(TRIGGER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);
  digitalWrite(TRIGGER_PIN, LOW);

  // SIM800 init
  sim800.begin(9600, SERIAL_8N1, SIM800_RX, SIM800_TX);
  delay(1000);
  sendCommand("AT");
  sendCommand("AT+CMGF=1"); // SMS mode
}

void loop() {
  int16_t ax, ay, az;
  int16_t gx, gy, gz;
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

  float angleX = atan2(ay, az) * 180 / PI;
  float angleY = atan2(ax, az) * 180 / PI;
  float acc = sqrt(ax*ax + ay*ay + az*az) / 16384.0; // g

  Serial.printf("AngleX: %.2f | AngleY: %.2f | Acc: %.2f g\n", angleX, angleY, acc);

  // điều kiện phát hiện ngã
  if ((acc < 0.4 || acc > 2.5) && (abs(angleX) > 70 || abs(angleY) > 70)) {
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

  digitalWrite(BUZZER_PIN, HIGH);

  digitalWrite(TRIGGER_PIN, HIGH);
  delay(2000);
  digitalWrite(TRIGGER_PIN, LOW);

  sendSMS("+1234567890", "Emergency! Fall detected. Please check immediately.");

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
