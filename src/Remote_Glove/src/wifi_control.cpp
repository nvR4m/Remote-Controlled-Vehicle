#include "wifi_control.h"

#include <esp_now.h>
#include <WiFi.h>

static uint8_t receiverMac[6] = {0x3c, 0x8a, 0x1f, 0xd3, 0x3f, 0x40};

// Track last send status globally
static volatile bool lastSendSuccess = false;
static volatile bool sendCompleted = false;

/* Response after sending data */
void onDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) 
{
    Serial.print("Last Packet Send Status: ");
    if (status == ESP_NOW_SEND_SUCCESS) 
    {
        Serial.println("Success");
        lastSendSuccess = true;
    } 
    else
    {
        Serial.println("Fail");
        lastSendSuccess = false;
    }
    sendCompleted = true; // mark send done
}

/* Initialize WiFi controller*/
void wifiControlInit() 
{
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();

    if (esp_now_init() != ESP_OK) 
    {
        Serial.println("Error initializing ESP-NOW");
        return;
    }

    esp_now_register_send_cb(onDataSent);

    esp_now_peer_info_t peerInfo = {};
    memcpy(peerInfo.peer_addr, receiverMac, 6);
    peerInfo.channel = 0;
    peerInfo.encrypt = false;

    if (esp_now_add_peer(&peerInfo) != ESP_OK) 
    {
        Serial.println("Failed to add peer");
        return;
    }

    Serial.println("ESP-NOW Initialized and peer added");
}

/* WiFi Send*/
bool sendGyroData(int16_t roll, int16_t pitch) 
{
    GyroData_t dataToSend = { .rollPct = roll, .pitchPct = pitch };

    sendCompleted = false;
    lastSendSuccess = false;

    esp_err_t result = esp_now_send(receiverMac, (uint8_t *)&dataToSend, sizeof(dataToSend));
    if (result != ESP_OK) 
    {
        Serial.println("Failed to queue send");
        return false;
    }

    uint32_t start = millis();
    while (!sendCompleted) 
    {
        if (millis() - start > 100) 
        {
            Serial.println("Send callback timeout");
            return false;
        }
        delay(1);
    }

    return lastSendSuccess;
}
