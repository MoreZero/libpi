#include "log.h"
#include "pin.h"
#include "nrf24l01.h"
#include <string> 
using namespace std;
//using namespace escpplib;

int main(int argc, char** argv){
    
    LOG_INFO("start");
	wiringPiSetup();

    Pin pin(3);
    NRF24L01 nrf24l01(pin);
    int32_t ret = nrf24l01.Init();
    LOG_INFO("Init ret:%d", ret);
    nrf24l01.SetRXMode();

    for (int i=0; i<100; i++) {
        nrf24l01.SPIRead();
        delay(1000);    
    }

    LOG_INFO("end");
    return 0;
}
