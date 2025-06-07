#pragma once

void ultrasonic_sensor_init(int trigPin, int echoPin);
float ultrasonic_sensor_read_cm();