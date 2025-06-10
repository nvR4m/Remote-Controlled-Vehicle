#include <Arduino.h>
#include "main.h"
#include "wifi_control.h"
#include "ultrasonic_sensor.h"

#define TRIG_PIN 13
#define ECHO_PIN 12

State currentState = STATE_COMM_FAIL;
HardwareSerial VehicleUART(2); 
bool frontLocked = false;

void printState(State state) {
    switch (state) {
        case STATE_COMM_FAIL:  Serial.println("State: Communication Fail"); break;
        case STATE_HALT:       Serial.println("State: Halt"); break;
        case STATE_T2_BACK:    Serial.println("State: T2 Back"); break;
        case STATE_T2_FRONT:   Serial.println("State: T2 Front"); break;
        case STATE_T2_RIGHT:   Serial.println("State: T2 Right"); break;
        case STATE_T2_LEFT:    Serial.println("State: T2 Left"); break;
        default:               Serial.println("State: Unknown"); break;
    }
}

void setup() {
    Serial.begin(115200);
    VehicleUART.begin(9600, SERIAL_8N1, -1, 17);

    ultrasonic_sensor_init(TRIG_PIN, ECHO_PIN);
    wifi_control_init();
}

void loop() {
    unsigned long now = millis();
    State newState = currentState;

    /* Get sensor data */
    float distanceCM = ultrasonic_sensor_read_cm();

    /* Decide driving command */
    if (!wifi_control_data_available()) {
        newState = STATE_COMM_FAIL;
    } else {
        GyroData_t data = wifi_control_get_latest();
        int16_t roll = data.rollPct;
        int16_t pitch = data.pitchPct;

        if(roll == 0 && pitch == 0) newState = STATE_HALT; 
        else if (pitch >= 30)  newState = STATE_T2_RIGHT;
        else if (pitch <= -40) newState = STATE_T2_LEFT;
        else if (roll >= 40)   newState = STATE_T2_BACK;
        else if (roll <= -40 && !frontLocked)  newState = STATE_T2_FRONT;
        else newState = STATE_HALT;

    }

    /* Compute sensor information */
    if (distanceCM <= 15 && newState == STATE_T2_FRONT)
    {
        newState = STATE_HALT;
    } 
    else
    {
        /* Do nothing */
    }

    /* Send command to Arduino UNO */
    if (newState != currentState) {
        currentState = newState;
        printState(currentState);
        VehicleUART.write((uint8_t)currentState);
    }

    Serial.print("Distance: ");
    Serial.print(distanceCM);
    Serial.println(" cm");

    delay(50);
}