#include <Wire.h>
#include "oled.h"
#include "gyro.h"
#include "main.h"
#include "wifi_control.h"

unsigned long lastBlink = 0;
bool blinkOn = false;

void setup() 
{
    /* Configure I2C */
    Wire.begin(21, 22);  

    /* Initialize OLED and gyro */
    oledSetup();
    gyroSetup();

    /* Initialize ESP-NOW WiFi transmitter */
    wifiControlInit();

    /* Short delay for stability */
    delay(500);
}

void loop() 
{
    /* Call gyro IC and get relevant data */  
    gyroUpdate();
    int16_t roll = gyroGetRollPct();
    int16_t pitch = gyroGetPitchPct();

    /* Send gyro data via ESP-NOW */
    bool sent = sendGyroData(roll, pitch);

    if (!sent) 
    {
        Serial.println("Failed to send gyro data");
    }

    /* Blink toggle every 500ms */
    if (millis() - lastBlink > 500) 
    {
        lastBlink = millis();
        blinkOn = !blinkOn;
    }

    /* Update OLED with gyro data + transmission status */
    oledShowInclination(roll, pitch, sent ? TX_OK : TX_FAIL, blinkOn);

}
