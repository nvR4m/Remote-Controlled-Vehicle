#pragma once

#include <stdint.h>

typedef struct {
    int16_t rollPct;
    int16_t pitchPct;
} GyroData_t;

void wifi_control_init();
bool wifi_control_data_available();
GyroData_t wifi_control_get_latest();
unsigned long wifi_control_last_received();