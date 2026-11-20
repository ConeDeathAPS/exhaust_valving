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
    delay(500);
    Serial.print("\nMANUAL TRIGGER STATE = ");
    Serial.print(digitalRead(MANUAL_TRIGGER));
    Serial.print("\nECM TRIGGER STATE = ");
    Serial.print(digitalRead(ECM_TRIGGER));
    if (digitalRead(MANUAL_TRIGGER) == LOW || digitalRead(ECM_TRIGGER) == LOW) {
        // Open has been commanded, so open the valves if they are closed
        if (leftHandActuator.getCurrentDutyCycle() == CLOSED_DUTY_CYCLE) {
            Serial.println("OPEN LEFT");
            leftHandActuator.setDesiredState(OPEN_DUTY_CYCLE);
        }
        if (rightHandActuator.getCurrentDutyCycle() == CLOSED_DUTY_CYCLE) {
            Serial.println("OPEN RIGHT");
            rightHandActuator.setDesiredState(OPEN_DUTY_CYCLE);
        }
    } else {
        if (leftHandActuator.getCurrentDutyCycle() == OPEN_DUTY_CYCLE) {
            Serial.println("CLOSE LEFT");
            leftHandActuator.setDesiredState(CLOSED_DUTY_CYCLE);
        }
        if (rightHandActuator.getCurrentDutyCycle() == OPEN_DUTY_CYCLE) {
            Serial.println("CLOSE RIGHT");
            rightHandActuator.setDesiredState(CLOSED_DUTY_CYCLE);
        }
    }

    // Set LED states
    // Due to how the switch we are using works, LOW actually turns on the LEDs.
    // The current switch also has syncronous LEDs meaning we cannot control them independently
    if (leftHandActuator.getCurrentDutyCycle() != rightHandActuator.getCurrentDutyCycle()) {
        // Turn off both LEDs
        digitalWrite(LED_OPEN, HIGH);
        digitalWrite(LED_CLOSED, HIGH);
    } else {
        digitalWrite(LED_OPEN, LOW);
        digitalWrite(LED_CLOSED, LOW);
    }
}
