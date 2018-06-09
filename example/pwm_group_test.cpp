#include "log.h"
#include "pin.h"
#include "soft_pwm.h"
#include <string> 
using namespace std;
//using namespace escpplib;

int main(int argc, char** argv){
    
    LOG_INFO("start");
	wiringPiSetup();
    Pin pin2(2);
    Pin pin3(3);
    

    SoftPwmGroup pwm_group(20000, 3000);
    pwm_group.AddPin(pin2);
    pwm_group.AddPin(pin3);
    pwm_group.Start();
    
    LOG_INFO("set 1000");
    pwm_group.SetMark(0,1000);
    delay(5000);
    LOG_INFO("set 2000");
    pwm_group.SetMark(1,2000);
    delay(5000);
    LOG_INFO("set 1500");
    pwm_group.SetMark(0, 1500);
    delay(5000);
    LOG_INFO("set 2000");
    pwm_group.SetMark(1, 1000);
    delay(5000);
    LOG_INFO("set 2500");
    pwm_group.SetMark(0, 2000);
    delay(5000);
    LOG_INFO("set 3000");
    pwm_group.SetMark(1, 3000);
    delay(2000);
    LOG_INFO("end");

    return 0;
}
