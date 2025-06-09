#include "gyro.h"
#include <Wire.h>
#include <math.h>

/* Initialize gyro IC */
void gyroSetup(void) 
{
    Wire.beginTransmission(MPU_ADDR);
    Wire.write(0x6B);  
    Wire.write(0x00); 
    Wire.endTransmission();
    delay(100);
}

/* Read 2 bytes from register */
static int16_t readMPURegister16(uint8_t reg) 
{
    Wire.beginTransmission(MPU_ADDR);
    Wire.write(reg);
    Wire.endTransmission(false);
    Wire.requestFrom(MPU_ADDR, 2);
    return (Wire.read() << 8) | Wire.read();
}

void gyroUpdate(void) 
{
    /* Read raw data */
    int16_t ax = readMPURegister16(0x3B);
    int16_t ay = readMPURegister16(0x3D);
    int16_t az = readMPURegister16(0x3F);

    /* Convers to g units */
    float xg = ax / 16384.0f;
    float yg = ay / 16384.0f;
    float zg = az / 16384.0f;

    /* Calcualte roll and pitch angles */
    float roll  = atan2f(yg, sqrtf(xg * xg + zg * zg)) * 180.0f / M_PI;
    float pitch = atan2f(-xg, sqrtf(yg * yg + zg * zg)) * 180.0f / M_PI;

    /* Constain angles to 90 degrees */
    if (roll  >  90.0f) roll  =  90.0f;
    if (roll  < -90.0f) roll  = -90.0f;
    if (pitch >  90.0f) pitch =  90.0f;
    if (pitch < -90.0f) pitch = -90.0f;

    /* Convert to percentage values */
    rollPct  = (int16_t)(roll  / 90.0f * 100.0f) * (roll < 0 ? 2.7f : 1.8f);
    pitchPct = (int16_t)((pitch / 90.0f * 100.0f) * (pitch < 0 ? 1.5f : 2.2f));

    /* Limit roll and pitch values */
    if(roll >= -20 && roll <= 20){ rollPct = 0;}
    if(pitch >= -20 && pitch <= 20){ pitchPct = 0;}
}

/* Getter for roll percentage */
int16_t gyroGetRollPct(void) 
{
    if(rollPct > 100) rollPct = 100;
    else if(rollPct < -100) rollPct = -100;
    return rollPct;
}

/* Getter for pitch percentage */
int16_t gyroGetPitchPct(void) 
{
    if(pitchPct > 100) pitchPct = 100;
    else if(pitchPct < -100) pitchPct = -100;
    return pitchPct;
}
