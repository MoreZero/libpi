#include "log.h"
#include "pin.h"
#include "matrix_keyboard.h"
#include <string> 
using namespace std;
using namespace std::placeholders;

void TestKey(int32_t l, int32_t r) {
    LOG_INFO("testkey l:%d, r:%d", l, r);
}


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

    MatrixKeyboard m_k(l1,l2,l3,l4, r1,r2,r3,r4);

    m_k.RegisterFunc(2, 2, std::bind(TestKey, _1, _2));
    m_k.RegisterFunc(1, 1, std::bind(TestKey, _1, _2));

    m_k.Start();

    delay(2000000);
    LOG_INFO("end");

    return 0;
}
