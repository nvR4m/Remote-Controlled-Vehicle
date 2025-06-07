#ifndef OLED_H
#define OLED_H

#include <Arduino.h>         
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// OLED configuration
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_ADDR 0x3C

enum TxStatus {
    TX_NONE,
    TX_OK,
    TX_FAIL
};

extern Adafruit_SSD1306 display;

void oledSetup();
void oledShowInclination(int rollPct, int pitchPct);
void oledShowMessage(const char* msg);
void oledShowInclination(int rollPct, int pitchPct, TxStatus txStatus = TX_NONE, bool blinkOn = false);

#endif
