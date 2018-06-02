

#include "log.h"
#include "pin.h"
#include "soft_pwm.h"
#include <string> 
using namespace std;
//using namespace escpplib;

int main(int argc, char** argv){
    
    LOG_INFO("start");
	wiringPiSetup();
    Pin pin(25);
    LOG_INFO("set 1000");
    SoftPwm pwm(pin, 1000, 20000);
    pwm.Start();
    delay(10000);
    LOG_INFO("set 2000");
    pwm.SetMark(2000);
    delay(10000);
    LOG_INFO("set 1000");
    pwm.SetMark(1000);
    delay(10000);
    LOG_INFO("set 2000");
    pwm.SetMark(2000);
    delay(10000);
    LOG_INFO("end");

    return 0;
}
