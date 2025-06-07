#include <WiFi.h>
#include <esp_now.h>

// ==================== HC-SR04 Pins ====================
#define TRIG_PIN 13
#define ECHO_PIN 12

// ==================== Data Structure ====================
typedef struct {
    int16_t rollPct;
    int16_t pitchPct;
} GyroData_t;

GyroData_t latestData = {0, 0};
unsigned long lastReceived = 0;

// ==================== State Machine ====================
enum State {
    STATE_COMM_FAIL,
    STATE_HALT,
    STATE_T1_BACK,
    STATE_T2_BACK,
    STATE_T1_FRONT,
    STATE_T2_FRONT,
    STATE_T1_RIGHT,
    STATE_T2_RIGHT,
    STATE_T1_LEFT,
    STATE_T2_LEFT
};

State currentState = STATE_COMM_FAIL;

// ==================== HardwareSerial ====================
HardwareSerial GloveUART(2);  // Use UART2 on ESP32 (GPIO17 TX, GPIO16 RX usually)

// ==================== Print State ====================
void printState(State state) {
    switch (state) {
        case STATE_COMM_FAIL:  Serial.println("State: Communication Fail"); break;
        case STATE_HALT:       Serial.println("State: Halt"); break;
        case STATE_T1_BACK:    Serial.println("State: T1 Back"); break;
        case STATE_T2_BACK:    Serial.println("State: T2 Back"); break;
        case STATE_T1_FRONT:   Serial.println("State: T1 Front"); break;
        case STATE_T2_FRONT:   Serial.println("State: T2 Front"); break;
        case STATE_T2_RIGHT:   Serial.println("State: T2 Right"); break;
        case STATE_T2_LEFT:    Serial.println("State: T2 Left"); break;
    }
}

// ==================== ESP-NOW Callback ====================
void onDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len) {
    if (len == sizeof(GyroData_t)) {
        memcpy(&latestData, incomingData, sizeof(GyroData_t));
        lastReceived = millis();
    }
}

// ==================== Distance Sensor Function ====================
float readDistanceCM() {
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);

    long duration = pulseIn(ECHO_PIN, HIGH, 30000); // 30ms timeout
    float distance = duration * 0.0343 / 2.0;
    return distance;
}

// ==================== Setup ====================
void setup() {
    Serial.begin(115200);  // USB Serial for debugging
    GloveUART.begin(9600, SERIAL_8N1, -1, 17);  // UART2: RX not used (-1), TX on GPIO17

    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);

    WiFi.mode(WIFI_STA);
    WiFi.disconnect();

    if (esp_now_init() != ESP_OK) {
        Serial.println("ESP-NOW initialization failed");
        return;
    }

    esp_now_register_recv_cb(onDataRecv);
    Serial.println("ESP-NOW Receiver Initialized");
}

// ==================== Main Loop ====================
void loop() {
    unsigned long now = millis();
    State newState = currentState;

    if (now - lastReceived > 2000) {
        newState = STATE_COMM_FAIL;
    } else {
        int16_t roll = latestData.rollPct;
        int16_t pitch = latestData.pitchPct;

        // Roll has priority over pitch
        if (roll >= 80)        newState = STATE_T2_BACK;
        else if (roll >= 40)   newState = STATE_T1_BACK;
        else if (roll <= -80)  newState = STATE_T2_FRONT;
        else if (roll <= -40)  newState = STATE_T1_FRONT;
        else if (pitch >= 30)  newState = STATE_T2_RIGHT;
        else if (pitch <= -40) newState = STATE_T2_LEFT;
        else                   newState = STATE_HALT;
    }

    if (newState != currentState) {
        currentState = newState;
        printState(currentState);

        // Send state as a single byte over UART2 (GPIO17 TX)
        GloveUART.write((uint8_t)currentState);
    }

    // Read and print distance
    float distanceCM = readDistanceCM();
    Serial.print("Distance: ");
    Serial.print(distanceCM);
    Serial.println(" cm");

    delay(50);
}
