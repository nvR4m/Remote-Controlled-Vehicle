#include <esp_now.h>
#include <WiFi.h>

int receivedValue = 0;
bool newData = false;

// Updated callback with new signature (requires esp_now_recv_info_t)
void OnDataRecv(const esp_now_recv_info_t *info, const uint8_t *incomingData, int len) {
  if (len == sizeof(receivedValue)) {
    memcpy(&receivedValue, incomingData, sizeof(receivedValue));
    newData = true;
  }
}

void setup() {
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();  // Clean start

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_register_recv_cb(OnDataRecv);
  Serial.println("ESP-NOW Receiver Ready");
}

void loop() {
  if (newData) {
    Serial.print("Received Decimal Value: ");
    Serial.println(receivedValue);
    newData = false;
  }
  delay(1000);
}
