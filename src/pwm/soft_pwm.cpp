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
    if (is_stop_ == false) {    // Already running on this pin
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

// -----------------------------------------------------------------------------

void SoftPwmGroup::SetMark(int32_t index, int32_t mark) {
    mark_vec_[index]=mark;
}

void SoftPwmGroup::Stop() {
    is_stop_ = true;
}
    
int32_t SoftPwmGroup::Start() {
    if (is_stop_ == false) {    // Already running on this pin
        return -1 ;
    }

    for (uint32_t i=0; i<pin_vec_.size(); ++i) {
        mark_vec_.push_back(0);
        pin_vec_[i].SetOutput();
        pin_vec_[i].SetLow();
    }

    int32_t res = pthread_create(&thread_, NULL, SoftPwmGroup::SoftPwmGroupThreadHandle, (void *)this) ;
    if (res == 0) {
        is_stop_ = false;
    }
    return res ;
}

void* SoftPwmGroup::SoftPwmGroupThreadHandle(void *arg) {
    SoftPwmGroup* soft_pwm_group = (SoftPwmGroup*)arg;
   
    // 时间片轮转法 
    struct sched_param param ;
    param.sched_priority = sched_get_priority_max(SCHED_RR) ;
    pthread_setschedparam(pthread_self(), SCHED_RR, &param) ;
    piHiPri(90);

    vector<Pin>  pin_vec = soft_pwm_group->pin_vec_;
    int32_t cycle = soft_pwm_group->cycle_;
    int32_t mark_size = soft_pwm_group->mark_size_;
    int32_t mark;
    int32_t used_mark;

    while(!soft_pwm_group->is_stop_) {
        used_mark = 0;
        for (uint32_t i=0; i<pin_vec.size(); ++i) {
            mark = soft_pwm_group->mark_vec_[i];
            if (mark == 0) {
                delayMicroseconds(mark_size);
            }else {
                Pin& pin = pin_vec[i];
                pin.SetHigh();
                delayMicroseconds(mark);
                pin.SetLow();
                delayMicroseconds(mark_size-mark);
            }
            used_mark += mark_size;
        }
        
        delayMicroseconds(cycle-used_mark);
    }

    return NULL ;
}












