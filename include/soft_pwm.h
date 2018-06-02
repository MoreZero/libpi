#ifndef LIBPI_SOFT_PWM_H
#define LIBPI_SOFT_PWM_H

#include <wiringPi.h>
#include <pthread.h>
#include <stdint.h>

#include "pin.h"

class SoftPwm
{
public:
    SoftPwm(Pin& pin, int32_t mark, int32_t cycle)
            :pin_(pin), mark_(mark), cycle_(cycle), is_stop_(true){}
    void SetMark(int32_t mark);
    int32_t Start();
    void Stop();

    static void* SoftPwmThreadHandle(void *arg);

private:
    Pin&    pin_;
    int32_t mark_;
    int32_t cycle_;
    bool    is_stop_;
    pthread_t thread_ ;
};




#endif
