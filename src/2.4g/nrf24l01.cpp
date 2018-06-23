#include "nrf24l01.h"
#include <string.h>
//
//
//
//int32_t NRF24L01::Init()
//{
//    MISO_.SetOutPut();
//    MOSI_.SetInPut();
//    SCK_.SetOutPut();
//    CSN_.SetOutPut();
//    CE_.SetOutput();
//    CE_.SetLow();
//
//    SetRXMode();
//
//    return 0;
//}
//
//int32_t NRF24L01::SetRXMode()
//{
//    CE_.SetLow(); // 关闭模块，进行寄存器配置
//    SCK_.SetLow();
//    RegWrite(0x00, 0x3b); // 读模式
//    RegWrite(0x01, 0x00); // 不自动应答
//    RegWrite(0x02, 0x01); // 使用P0信道通信，使用默认地址
//    RegWrite(0x04, 0x00); // 不自动应答
//    RegWrite(0x05, 64);   // 使用64频道，共128频道
//    RegWrite(0x06, 0x07); // 1Mbps, 0dBm 
//    RegWrite(0x11, 6);    // 每次信道6字节数据
//    CE_.SetHigh();
//    return 0;
//}
//
//void NRF24L01::RegWrite(uint8_t addr, uint8_t cmd)
//{
//    CSN.SetLow();
//    SPIWrite(0xa0);
//
//
//    buff_[0] = 0xA0 + addr;
//    buff_[1] = cmd;
//    SPIWrite(2);
//}
//
//void NRF24L01::SPIWrite(uint8_t data)
//{
//    uint8_t i;
//    for (i=0; i<8; i++)
//    {
//        digitalWrite(MDI, byte&0x80);
//        SCK_.SetHigh();
//        byte<<=1;
//        byte|=(uint8_t)digitalRead(MDO);
//        SCK_.SetLow();
//    }
//    return byte;
//}
//
//
//




//-----------------------------------------------



int32_t NRF24L01::Init()
{
    //初始化SPI通道0，并设置为最大速度32000000
    int32_t ret = wiringPiSPISetup(0,500000); 
    if(ret == -1)
    {
        return -1;
    }
  
    CE_.SetOutput();
    CE_.SetLow();

    return 0;
}


int32_t NRF24L01::SetRXMode()
{
    CE_.SetLow(); // 关闭模块，进行寄存器配置
    RegWrite(0x00, 0x3b); // 读模式
    RegWrite(0x01, 0x00); // 不自动应答
    RegWrite(0x02, 0x01); // 使用P0信道通信，使用默认地址
    RegWrite(0x04, 0x00); // 不自动应答
    RegWrite(0x05, 64);   // 使用64频道，共128频道
    RegWrite(0x06, 0x07); // 1Mbps, 0dBm 
    RegWrite(0x11, 6);    // 每次信道6字节数据
    CE_.SetHigh();
    return 0;
}


int32_t NRF24L01::SetTXMode()
{
    CE_.SetLow(); // 关闭模块，进行寄存器配置
    RegWrite(0x00, 0x0a); // 读模式
    RegWrite(0x01, 0x00); // 不自动应答
    RegWrite(0x02, 0x01); // 使用P0信道通信，使用默认地址
    RegWrite(0x04, 0x00); // 不自动应答
    RegWrite(0x05, 64);   // 使用64频道，共128频道
    RegWrite(0x06, 0x07); // 1Mbps, 0dBm 
    RegWrite(0x11, 6);    // 每次信道6字节数据
    CE_.SetHigh();
    return 0;
}

void NRF24L01::Write()
{
    buff_[0] = 0xa0;
    buff_[1] = 3;
    buff_[2] = 9;
    buff_[3] = 3;
    buff_[4] = 8;
    buff_[5] = 3;
    buff_[6] = 7;
    
    SPIWrite(7);
}


void NRF24L01::SPIWrite(int32_t size)
{
    int32_t ret = wiringPiSPIDataRW(0, buff_, size); 
    LOG_DEBUG("wiringPiSPIDataRW ret:%d", ret);
}

void NRF24L01::SPIRead()
{
    uint8_t buff[1024];
    bzero(buff, 100);
    buff[0]=0x61;
    int32_t ret = wiringPiSPIDataRW(0, buff, 7); 
    LOG_DEBUG("wiringPiSPIDataRW ret:%d", ret);
    for (int32_t i=0; i<7; ++i)
    {
        LOG_INFO("recv :%d", (int32_t)buff[i]);
    }
}

void NRF24L01::RegWrite(uint8_t addr, uint8_t cmd)
{
    buff_[0] = 0x20 + addr;
    buff_[1] = cmd;
    SPIWrite(2);
}




