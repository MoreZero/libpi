#include "log.h"
#include "pin.h"
#include "soft_pwm.h"
#include "matrix_keyboard.h"
#include <string> 
using namespace std;
using namespace std::placeholders;

void TestKey(int32_t l, int32_t r) {
    LOG_INFO("testkey l:%d, r:%d", l, r);
}


class MechanicalArm 
{
public:
    MechanicalArm(SoftPwmGroup& pwm_group, MatrixKeyboard& matrix_keyboard)
        :pwm_group_(pwm_group), matrix_keyboard_(matrix_keyboard) {

    }
    
    void Start(Pin& pin0, Pin& pin1, Pin& pin2, Pin& pin3) {

        pwm_group_.AddPin(pin0);
        pwm_group_.AddPin(pin1);
        pwm_group_.AddPin(pin2);
        pwm_group_.AddPin(pin3);

        matrix_keyboard_.RegisterFunc(0, 0, std::bind(&MechanicalArm::ServoLeft0, this, _1, _2));
        matrix_keyboard_.RegisterFunc(1, 0, std::bind(&MechanicalArm::ServoRight0, this, _1, _2));
        
        matrix_keyboard_.RegisterFunc(0, 1, std::bind(&MechanicalArm::ServoLeft1, this, _1, _2));
        matrix_keyboard_.RegisterFunc(1, 1, std::bind(&MechanicalArm::ServoRight1, this, _1, _2));
        
        matrix_keyboard_.RegisterFunc(0, 2, std::bind(&MechanicalArm::ServoLeft2, this, _1, _2));
        matrix_keyboard_.RegisterFunc(1, 2, std::bind(&MechanicalArm::ServoRight2, this, _1, _2));
        
        matrix_keyboard_.RegisterFunc(0, 3, std::bind(&MechanicalArm::ServoLeft3, this, _1, _2));
        matrix_keyboard_.RegisterFunc(1, 3, std::bind(&MechanicalArm::ServoRight3, this, _1, _2));
        
        
        pwm_group_.Start();
        pwm_group_.SetMark(0, 1500);
        pwm_group_.SetMark(1, 1500);
        pwm_group_.SetMark(2, 1500);
        pwm_group_.SetMark(3, 1500);
        debug_size_ = 1500;
        matrix_keyboard_.Start();
    }
private:
    void ServoLeft0(int32_t l, int32_t r) {
        pwm_group_.AddMark(0, 20);
    }
    void ServoRight0(int32_t l, int32_t r) {
        pwm_group_.AddMark(0, -20);
    }

    void ServoLeft1(int32_t l, int32_t r) {
        pwm_group_.AddMark(1, 20);
    }
    void ServoRight1(int32_t l, int32_t r) {
        pwm_group_.AddMark(1, -20);
    }

    void ServoLeft2(int32_t l, int32_t r) {
        pwm_group_.AddMark(2, 20);
    }
    void ServoRight2(int32_t l, int32_t r) {
        pwm_group_.AddMark(2, -20);
    }

    void ServoLeft3(int32_t l, int32_t r) {
        pwm_group_.AddMark(3, 20);
        debug_size_ += 10;
        LOG_DEBUG("debug_size:%d", debug_size_);
    }
    void ServoRight3(int32_t l, int32_t r) {
        pwm_group_.AddMark(3, -20);
        debug_size_ -= 10;
        LOG_DEBUG("debug_size:%d", debug_size_);
    }

private:
    SoftPwmGroup&   pwm_group_;
    MatrixKeyboard& matrix_keyboard_;
    int debug_size_;
};


int main(int argc, char** argv){
    
    LOG_INFO("start");
	wiringPiSetup();

    Pin l1(25);
    Pin l2(29);
    Pin l3(24);
    Pin l4(28);
    
    Pin r1(23);
    Pin r2(27);
    Pin r3(22);
    Pin r4(26);

    Pin servo0(0);
    Pin servo1(2);
    Pin servo2(3);
    Pin servo3(1);


    MatrixKeyboard m_k(l1,l2,l3,l4, r1,r2,r3,r4);
    SoftPwmGroup pwm_group(20000, 3000);
    
    MechanicalArm arm(pwm_group, m_k);
    arm.Start(servo0, servo1, servo2, servo3);


    delay(2000000);
    LOG_INFO("end");

    return 0;
}
