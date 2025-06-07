#include "wifi_control.h"
#include <WiFi.h>
#include <esp_now.h>

static GyroData_t latestData = {0, 0};
static unsigned long lastReceived = 0;

void onDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len) {
    if (len == sizeof(GyroData_t)) {
        memcpy(&latestData, incomingData, sizeof(GyroData_t));
        lastReceived = millis();
    }
}

void wifi_control_init() {
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    if (esp_now_init() != ESP_OK) {
        Serial.println("ESP-NOW initialization failed");
        return;
    }
    esp_now_register_recv_cb(onDataRecv);
    Serial.println("ESP-NOW Receiver Initialized");
}

bool wifi_control_data_available() {
    return (millis() - lastReceived) < 2000;
}

GyroData_t wifi_control_get_latest() {
    return latestData;
}

unsigned long wifi_control_last_received() {
    return lastReceived;
}