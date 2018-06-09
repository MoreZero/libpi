#ifndef LIBPI_SOFT_PWM_H
#define LIBPI_SOFT_PWM_H

#include <wiringPi.h>
#include <pthread.h>
#include <stdint.h>
#include <vector>

#include "pin.h"

using namespace std;

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

class SoftPwmGroup
{
public:
    SoftPwmGroup(int32_t cycle, int32_t mark_size)
            : cycle_(cycle), mark_size_(mark_size), is_stop_(true){}
    void SetMark(int32_t index, int32_t mark);
    void AddMark(int32_t index, int32_t size);
    void AddPin(Pin& pin) {pin_vec_.push_back(pin);}
    int32_t Start();
    void Stop();

    static void* SoftPwmGroupThreadHandle(void *arg);

private:
    vector<Pin>        pin_vec_;
    vector<int32_t>     mark_vec_;
    int32_t cycle_;
    int32_t mark_size_;
    bool    is_stop_;
    pthread_t thread_ ;
};







#endif
