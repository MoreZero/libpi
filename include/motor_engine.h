#ifndef LIBPI_MOTOR_ENGINE_H
#define LIBPI_MOTOR_ENGINE_H

#include <wiringPi.h>
#include <stdint.h>

#include "pin.h"

using namespace std;

/* 对应双路电机驱动模块 */
class MotorEngine
{
public:
    MotorEngine(Pin& pin_a_1, Pin& pin_a_2, Pin& pin_b_1, Pin& pin_b_2, Pin& pin_a_pwm, Pin& pin_b_pwm);

    void Advance(int32_t index);
    void Retreat(int32_t index);
    void Stop(int32_t index);

private:
    Pin& pin_a_1_;
    Pin& pin_a_2_;
    Pin& pin_b_1_;
    Pin& pin_b_2_;
    Pin& pin_a_pwm_;
    Pin& pin_b_pwm_;
};










#endif
