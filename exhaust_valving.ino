#include "./Actuator.h"

//
// Created by Soren on 11/13/2026.
//

/* GLOBAL VARS */
// The MOSFET will invert these duty cycles, so these are opposite to what the actuator docs say
constexpr float OPEN_DUTY_CYCLE = .10;
constexpr float CLOSED_DUTY_CYCLE = .90;

// Valve open trigger from the ECM
constexpr int ECM_TRIGGER = A2;
// Valve open trigger from the manual override
constexpr int MANUAL_TRIGGER = A1;
// LED indicator - open
constexpr int LED_OPEN = D8;
// LED indicator - closed
constexpr int LED_CLOSED = D7;

static Actuator leftHandActuator(D2, 0, OPEN_DUTY_CYCLE, CLOSED_DUTY_CYCLE);
static Actuator rightHandActuator(D5, 1, OPEN_DUTY_CYCLE, CLOSED_DUTY_CYCLE);

void setup() {
    Serial.begin(115200);
    pinMode(ECM_TRIGGER, INPUT_PULLUP);
    pinMode(MANUAL_TRIGGER, INPUT_PULLUP);
    pinMode(LED_OPEN, OUTPUT);
    pinMode(LED_CLOSED, OUTPUT);
    leftHandActuator.init();
    rightHandActuator.init();
}

void loop() {
    if (digitalRead(MANUAL_TRIGGER) == LOW || digitalRead(ECM_TRIGGER) == LOW) {
        leftHandActuator.setDesiredState(OPEN_DUTY_CYCLE);
        rightHandActuator.setDesiredState(OPEN_DUTY_CYCLE);
    } else if (leftHandActuator.getCurrentDutyCycle() == OPEN_DUTY_CYCLE) {
        leftHandActuator.setDesiredState(CLOSED_DUTY_CYCLE);
        rightHandActuator.setDesiredState(CLOSED_DUTY_CYCLE);
    }

    // Set LED states
    if (leftHandActuator.getCurrentDutyCycle() == CLOSED_DUTY_CYCLE && rightHandActuator.getCurrentDutyCycle() == CLOSED_DUTY_CYCLE) {
        digitalWrite(LED_OPEN, LOW);
        digitalWrite(LED_CLOSED, HIGH);
    } else if (leftHandActuator.getCurrentDutyCycle() == OPEN_DUTY_CYCLE && rightHandActuator.getCurrentDutyCycle() == OPEN_DUTY_CYCLE) {
        digitalWrite(LED_OPEN, HIGH);
        digitalWrite(LED_CLOSED, LOW);
    }
}
