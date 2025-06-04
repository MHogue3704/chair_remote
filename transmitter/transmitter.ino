#include <esp_now.h>
#include <WiFi.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define BUTTON_UP    0
#define BUTTON_DOWN  1
#define BATTERY_PIN  2
#define CHRG_PIN     3
#define OLED_ADDR    0x3C

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

typedef struct {
  bool up;
  bool down;
} ControlPacket;

ControlPacket packet;

uint8_t receiverMAC[] = {0x30, 0xAE, 0xA4, 0x00, 0x00, 0x00}; // Update this!

void sendCommand(bool up, bool down) {
  packet.up = up;
  packet.down = down;
  esp_now_send(receiverMAC, (uint8_t *)&packet, sizeof(packet));
}

void setup() {
  pinMode(BUTTON_UP, INPUT_PULLUP);
  pinMode(BUTTON_DOWN, INPUT_PULLUP);
  pinMode(CHRG_PIN, INPUT);

  Serial.begin(115200);
  WiFi.mode(WIFI_STA);

  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
    Serial.println("OLED init failed");
    while (1);
  }

  display.clearDisplay();
  display.display();

  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW init failed");
    return;
  }

  esp_now_peer_info_t peerInfo;
  memcpy(peerInfo.peer_addr, receiverMAC, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;
  esp_now_add_peer(&peerInfo);
}

void loop() {
  bool upPressed = !digitalRead(BUTTON_UP);
  bool downPressed = !digitalRead(BUTTON_DOWN);
  bool charging = !digitalRead(CHRG_PIN);

  sendCommand(upPressed, downPressed);
  updateDisplay(upPressed, downPressed, charging);
  delay(100);
}

void updateDisplay(bool up, bool down, bool charging) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);

  float battery = readBatteryVoltage();
  display.printf("Battery: %.2fV\n", battery);
  display.printf("Charging: %s\n", charging ? "Yes" : "No");
  display.printf("Status: %s\n", up ? "UP" : down ? "DOWN" : "IDLE");

  display.display();
}

float readBatteryVoltage() {
  int raw = analogRead(BATTERY_PIN);
  float voltage = (raw / 4095.0) * 3.3 * 2.0;
  return voltage;
}

