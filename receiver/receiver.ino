#include <esp_now.h>
#include <WiFi.h>

#define RELAY_UP   2
#define RELAY_DOWN 3
#define POWER_LED  10

typedef struct {
  bool up;
  bool down;
} ControlPacket;

ControlPacket packet;

void onReceive(const uint8_t *mac, const uint8_t *data, int len) {
  memcpy(&packet, data, sizeof(packet));
  digitalWrite(RELAY_UP, packet.up ? HIGH : LOW);
  digitalWrite(RELAY_DOWN, packet.down ? HIGH : LOW);
}

void setup() {
  Serial.begin(115200);

  pinMode(RELAY_UP, OUTPUT);
  pinMode(RELAY_DOWN, OUTPUT);
  pinMode(POWER_LED, OUTPUT);

  digitalWrite(RELAY_UP, LOW);
  digitalWrite(RELAY_DOWN, LOW);
  digitalWrite(POWER_LED, HIGH);

  WiFi.mode(WIFI_STA);
  esp_now_init();
  esp_now_register_recv_cb(onReceive);
}

void loop() {
  // Nothing needed
}
