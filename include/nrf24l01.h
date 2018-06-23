#ifndef LIBPI_NRF24L01_H
#define LIBPI_NRF24L01_H

#include <wiringPi.h>
#include <wiringPiSPI.h>
#include <stdint.h>

#include "pin.h"
#include "log.h"

using namespace std;





//
//class NRF24L01
//{
//public:
//    NRF24L01(Pin& CE):CE_(CE){}
//    int32_t Init();
//    
//
//    int32_t SetRXMode();
//    void SPIWrite(int32_t size);
//    void SPIRead();
//    void RegWrite(uint8_t addr, uint8_t cmd);
//
//private:
//    Pin& MISO_;
//    Pin& MOSI_;
//    Pin& SCK_;
//    Pin& CSN_;
//    Pin& CE_;
//    uint8_t buff_[1024];
//};


class NRF24L01
{
public:
    NRF24L01(Pin& CE):CE_(CE){}
    int32_t Init();
    

    int32_t SetRXMode();
    int32_t SetTXMode();
    void SPIWrite(int32_t size);
    void SPIRead();
    void RegWrite(uint8_t addr, uint8_t cmd);

    void Write();

private:
    Pin& CE_;
    uint8_t buff_[1024];
};





#endif
