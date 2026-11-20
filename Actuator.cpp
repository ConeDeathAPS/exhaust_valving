#include <Arduino.h>
#include "./Actuator.h"

//
// Created by Soren on 11/13/2026.
//

Actuator::Actuator(const int pwm, const int pwm_chan, const float open_duty_cycle, const float closed_duty_cycle) {
    PIN_PWM = pwm;
    PWM_CHAN = pwm_chan;
    currentDutyCycle = closed_duty_cycle; // Initialize in a closed state
    openDutyCycle = open_duty_cycle;
    closedDutyCycle = closed_duty_cycle;
}

void Actuator::init() const {
    pinMode(PIN_PWM, OUTPUT);
    ledcSetup(PWM_CHAN, 100, 12);
    ledcAttachPin(2, PWM_CHAN);
    const auto dutyValue = static_cast<uint32_t>(std::round(4095.0f * closedDutyCycle));
    ledcWrite(PWM_CHAN, dutyValue);
}

void Actuator::setDesiredState(const float newDutyCycle) {
    if (currentDutyCycle != newDutyCycle) {
        const float clampedDuty = constrain(newDutyCycle, 0.0f, 1.0f);
        const auto dutyValue = static_cast<uint32_t>(std::round(4095.0f * clampedDuty));
        ledcWrite(PWM_CHAN, dutyValue);
        Serial.println(newDutyCycle);
        currentDutyCycle = newDutyCycle;
    }
}

float Actuator::getCurrentDutyCycle() const {
    return currentDutyCycle;
}


