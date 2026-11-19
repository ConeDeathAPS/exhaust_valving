//
// Created by Soren on 11/13/2026.
//

#ifndef EXHAUST_VALVING_ACTUATOR_H
#define EXHAUST_VALVING_ACTUATOR_H

class Actuator {

    int PIN_PWM;
    int PWM_CHAN;
    float currentDutyCycle;
    float openDutyCycle;
    float closedDutyCycle;

    public:
        Actuator(int pwm, int pwm_chan, float open_duty_cycle, float closed_duty_cycle);

        // Init pin states
        void init() const;

        // At this time, we don't know what direction is open/close so we'll go with right to open, left to close for now
        void setDesiredState(float newDutyCycle);

        [[nodiscard]] float getCurrentDutyCycle() const;
};


#endif //EXHAUST_VALVING_ACTUATOR_H
