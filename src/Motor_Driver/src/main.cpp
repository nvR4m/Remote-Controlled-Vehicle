#include <AFMotor.h>
#include <Arduino.h>

/* State Definitions */
enum State {
  STATE_COMM_FAIL = 0,
  STATE_HALT,
  STATE_T2_BACK,
  STATE_T2_FRONT,
  STATE_T2_RIGHT,
  STATE_T2_LEFT
};

AF_DCMotor motor1(1);
AF_DCMotor motor2(2);
AF_DCMotor motor3(3);
AF_DCMotor motor4(4);

const int SPEED_T2 = 255;

void stopAll() {
  motor1.run(RELEASE);
  motor2.run(RELEASE);
  motor3.run(RELEASE);
  motor4.run(RELEASE);
}

void driveFront(int speed) {
  motor1.setSpeed(speed); motor1.run(FORWARD);
  motor2.setSpeed(speed); motor2.run(FORWARD);
  motor3.setSpeed(speed); motor3.run(BACKWARD);
  motor4.setSpeed(speed); motor4.run(BACKWARD);
}

void driveBack(int speed) {
  motor1.setSpeed(speed); motor1.run(BACKWARD);
  motor2.setSpeed(speed); motor2.run(BACKWARD);
  motor3.setSpeed(speed); motor3.run(FORWARD);
  motor4.setSpeed(speed); motor4.run(FORWARD);
}

void turnLeft(int speed) {
  motor1.setSpeed(speed); motor1.run(FORWARD);
  motor2.setSpeed(speed); motor2.run(FORWARD);
  motor3.setSpeed(speed); motor3.run(FORWARD);
  motor4.setSpeed(speed); motor4.run(FORWARD);
}

void turnRight(int speed) {
  motor1.setSpeed(speed); motor1.run(BACKWARD);
  motor2.setSpeed(speed); motor2.run(BACKWARD);
  motor3.setSpeed(speed); motor3.run(BACKWARD);
  motor4.setSpeed(speed); motor4.run(BACKWARD);
}

State currentState = STATE_HALT;

void setup() {
  Serial.begin(9600);
  stopAll(); 
}

void applyState(State state) {
  switch (state) {
    case STATE_T2_FRONT: driveFront(SPEED_T2); break;
    case STATE_T2_BACK:  driveBack(SPEED_T2);  break;
    case STATE_T2_LEFT:  turnLeft(SPEED_T2);   break;
    case STATE_T2_RIGHT: turnRight(SPEED_T2);  break;
    case STATE_HALT:     stopAll();            break;
    default:             /* Do nothing */      break;
  }
}

void printState(State state) {
  switch (state) {
    case STATE_COMM_FAIL:  Serial.println("State: Communication Fail"); break;
    case STATE_HALT:       Serial.println("State: Halt"); break;
    case STATE_T2_BACK:    Serial.println("State: T2 Back"); break;
    case STATE_T2_FRONT:   Serial.println("State: T2 Front"); break;
    case STATE_T2_RIGHT:   Serial.println("State: T2 Right"); break;
    case STATE_T2_LEFT:    Serial.println("State: T2 Left"); break;
    default:               Serial.println("State: Ignored/Unknown"); break;
  }
}

void loop() {
  if (Serial.available()) {
    int received = Serial.read();
    if (received >= STATE_COMM_FAIL && received <= STATE_T2_LEFT) {
      State newState = (State)received;
      currentState = newState;
      printState(currentState);
      applyState(currentState);
    }
  }
}