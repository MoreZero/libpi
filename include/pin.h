#ifndef LIBPI_PIN_H
#define LIBPI_PIN_H

#include <stdint.h>
#include <wiringPi.h>

class Pin
{
public:
    Pin(int32_t pin_id):pin_id_(pin_id) {}
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
    void SetHigh() {
        digitalWrite(pin_id_, HIGH) ;
    }
    void SetLow() {
        digitalWrite(pin_id_, LOW) ;
    }
    int32_t Level() {
	    return digitalRead(pin_id_);
    }
private:
    int32_t pin_id_;
};


#endif
