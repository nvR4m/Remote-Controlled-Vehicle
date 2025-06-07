#ifndef WIFI_CONTROL_H
#define WIFI_CONTROL_H

#include <esp_now.h>
#include <WiFi.h>

// Struct to hold gyro data we will send
typedef struct {
    int16_t rollPct;
    int16_t pitchPct;
} GyroData_t;

void wifiControlInit();                 // Initialize ESP-NOW and WiFi
bool sendGyroData(int16_t roll, int16_t pitch); // Send data to receiver

#endif // WIFI_CONTROL_H
