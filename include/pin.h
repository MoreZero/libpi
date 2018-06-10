#ifndef LIBPI_PIN_H
#define LIBPI_PIN_H

#include <stdint.h>
#include <wiringPi.h>

class Pin
{
public:
    Pin(int32_t pin_id):pin_id_(pin_id) {}
    Pin(const Pin& pin) {
        pin_id_ = pin.pin_id_;
    }
    Pin() {}
    ~Pin() {
        SetOutput();
        SetLow();
    }
    void SetOutput() {
        pinMode(pin_id_, OUTPUT);
    }
    void SetInput() {
        pinMode(pin_id_, INPUT);
    }
    void SetPwm() {
        pinMode(pin_id_, PWM_OUTPUT);
    }
    void SetPwmRange(int32_t range) {
        pwmSetRange(range);
    }
    void SetPwmValue(int32_t value) {
        pwmWrite(pin_id_, value);
    }
    void SetPwmClock(int32_t value) {
        pwmSetClock(value);
    }

    void PullUp() {
        pullUpDnControl(pin_id_, PUD_UP);
    }
    void PullDn() {
        pullUpDnControl(pin_id_, PUD_DOWN);
    }

    void SetHigh() {
        digitalWrite(pin_id_, HIGH) ;
    }
    void SetLow() {
        digitalWrite(pin_id_, LOW) ;
    }
    int32_t Level() {
        return digitalRead(pin_id_);
    }
    int32_t PinId() { return pin_id_;}
private:
    int32_t pin_id_;
};


#endif
