#pragma once

#include <Arduino.h>

// ==================== State Machine ====================
enum State {
    STATE_COMM_FAIL,
    STATE_HALT,
    STATE_T2_BACK,
    STATE_T2_FRONT,
    STATE_T2_RIGHT,
    STATE_T2_LEFT
};

extern State currentState;

void printState(State state);