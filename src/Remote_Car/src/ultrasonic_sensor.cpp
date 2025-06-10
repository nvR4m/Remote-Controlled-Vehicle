#include "ultrasonic_sensor.h"
#include <Arduino.h>

static int trigPin_;
static int echoPin_;

void ultrasonic_sensor_init(int trigPin, int echoPin) {
    trigPin_ = trigPin;
    echoPin_ = echoPin;
    pinMode(trigPin_, OUTPUT);
    pinMode(echoPin_, INPUT);
}

/* Determine distance to the object */
float ultrasonic_sensor_read_cm() {
    /* Send sound pulse */
    digitalWrite(trigPin_, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin_, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin_, LOW);

    /* Calculate distance depending on time */
    long duration = pulseIn(echoPin_, HIGH, 30000);
    float distance = duration * 0.0343 / 2.0;
    return distance;
}