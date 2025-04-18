#include <esp_now.h>
#include <WiFi.h>
// Pin assignments for 6 buttons
const int buttonPins[6] = {13, 12, 14, 27, 26, 25};
uint8_t broadcastAddress[] = {0x3C, 0x8A, 0x1F, 0xD3, 0x3F, 0x40};
int decimalValue = 0;
esp_now_peer_info_t peerInfo;
String success;
// Callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
  if (status ==0){
    success = "Delivery Success :)";
  }
  else{
    success = "Delivery Fail :(";
  }
}


void setup() {
  Serial.begin(115200);

  // Set button pins as INPUT_PULLUP
  for (int i = 0; i < 6; i++) {
    pinMode(buttonPins[i], INPUT_PULLUP);
  }

  WiFi.mode(WIFI_STA);
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  esp_now_register_send_cb(OnDataSent);
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }


}

void loop() {
  String binaryString = "";

  for (int i = 0; i < 6; i++) {
    int state = digitalRead(buttonPins[i]);

    // Button pressed = LOW, so we invert it (pressed → 1)
    binaryString += (state == LOW) ? "1" : "0";
  }

  // Convert binary string to decimal
  decimalValue = strtol(binaryString.c_str(), NULL, 2);

  Serial.print("Binary: ");
  Serial.print(binaryString);
  Serial.print(" | Decimal: ");
  Serial.println(decimalValue);

  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &decimalValue, sizeof(decimalValue));
  if (result == ESP_OK) {
    Serial.println("Sent with success");
  }
  else {
    Serial.println("Error sending the data");
  }
  delay(1000);
}
