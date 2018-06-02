#include "soft_pwm.h"

#include <pthread.h>
#include <wiringPi.h>

void SoftPwm::SetMark(int32_t mark) {
    mark_ = mark;
}
void SoftPwm::Stop() {
    is_stop_ = true;
}
    
int32_t SoftPwm::Start() {
    if (is_stop_ == false) {	// Already running on this pin
        return -1 ;
    }

    pin_.SetOutput();
    pin_.SetLow();

    int32_t res = pthread_create (&thread_, NULL, SoftPwm::SoftPwmThreadHandle, (void *)this) ;
    if (res == 0) {
        is_stop_ = false;
    }
    return res ;
}

void* SoftPwm::SoftPwmThreadHandle(void *arg) {
    SoftPwm* soft_pwm = (SoftPwm*)arg;
   
    // 时间片轮转法 
    struct sched_param param ;
    param.sched_priority = sched_get_priority_max(SCHED_RR) ;
    pthread_setschedparam(pthread_self(), SCHED_RR, &param) ;
    piHiPri(90);

    Pin&    pin = soft_pwm->pin_;
    int32_t cycle = soft_pwm->cycle_;
    int32_t mark, unmark;

    while(!soft_pwm->is_stop_) {
        mark = soft_pwm->mark_;
        unmark = cycle - mark;
           
        if (mark != 0) {
            pin.SetHigh();
        }
        delayMicroseconds(mark);
        if (unmark != 0) {
            pin.SetLow();
        }
        delayMicroseconds(unmark);
    }

    return NULL ;

}
