#include "motor_engine.h"


MotorEngine::MotorEngine(Pin& pin_a_1, Pin& pin_a_2, Pin& pin_b_1, Pin& pin_b_2, Pin& pin_a_pwm, Pin& pin_b_pwm)
           : pin_a_1_(pin_a_1),pin_a_2_(pin_a_2),pin_b_1_(pin_b_1),pin_b_2_(pin_b_2),pin_a_pwm_(pin_a_pwm),pin_b_pwm_(pin_b_pwm) {
    pin_a_1_.SetOutput();
    pin_a_2_.SetOutput();
    pin_b_1_.SetOutput();
    pin_b_2_.SetOutput();

    pin_a_1_.SetLow();
    pin_a_2_.SetLow();
    pin_b_1_.SetLow();
    pin_b_2_.SetLow();
    

}

void MotorEngine::Advance(int32_t index) {
    if (index == 0) {
        pin_a_1_.SetHigh();
        pin_a_2_.SetLow();
    }else {
        pin_b_1_.SetHigh();
        pin_b_2_.SetLow();
    }
}

void MotorEngine::Retreat(int32_t index) {
    if (index == 0) {
        pin_a_1_.SetLow();
        pin_a_2_.SetHigh();
    }else {
        pin_b_1_.SetLow();
        pin_b_2_.SetHigh();
    }
}

void MotorEngine::Stop(int32_t index) {
    if (index == 0) {
        pin_a_1_.SetLow();
        pin_a_2_.SetLow();
    }else {
        pin_b_1_.SetLow();
        pin_b_2_.SetLow();
    }
}














































