#ifndef GYRO_H
#define GYRO_H

#include <stdint.h>

#define MPU_ADDR 0x68

static int16_t rollPct = 0;
static int16_t pitchPct = 0;

void gyroSetup(void);
void gyroUpdate(void);
int16_t gyroGetRollPct(void);
int16_t gyroGetPitchPct(void);

#endif
