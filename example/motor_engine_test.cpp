#include "log.h"
#include "pin.h"
#include "soft_pwm.h"
#include "motor_engine.h"
#include <string> 
using namespace std;
//using namespace escpplib;

int main(int argc, char** argv){
    
    LOG_INFO("start");
	wiringPiSetup();
    

    Pin pin_pwm_a(1);
    Pin pin_pwm_b(23);
    //SoftPwm pwm_a(pin_pwm_a, 9, 20);
    //SoftPwm pwm_b(pin_pwm_b, 9, 20);
    //pwm_a.Start();
    //pwm_b.Start();
    
    //SoftPwmGroup pwm_group(200, 100);
    //pwm_group.AddPin(pin_pwm_a);
    //pwm_group.AddPin(pin_pwm_b);
    //pwm_group.Start();
    //pwm_group.SetMark(0,90);
    //pwm_group.SetMark(1,90);

    LOG_INFO("start pwm");
    pin_pwm_a.SetPwm();
    pin_pwm_a.SetPwmRange(200);
    pin_pwm_a.SetPwmValue(100);
    //pin_pwm_a.SetPwmClock(384);
    pin_pwm_a.SetPwmClock(192);
    LOG_INFO("end start pwm");

    Pin pin_a_1(29);
    Pin pin_a_2(28);
    Pin pin_b_1(24);
    Pin pin_b_2(25);
    MotorEngine engine(pin_a_1, pin_a_2, pin_b_1, pin_b_2, pin_pwm_a, pin_pwm_b);

    engine.Advance(0);
    engine.Advance(1);
    delay(3000);
   // engine.Retreat(1);
   // engine.Retreat(0);
   // delay(3000);
   // engine.Stop(1);
   // engine.Stop(0);
    delay(300000);
    pin_pwm_a.SetOutput();

    return 0;
}
