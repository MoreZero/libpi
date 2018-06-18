#include "log.h"
#include "pin.h"
#include "soft_pwm.h"
#include "motor_engine.h"
#include "es_socket.h"
#include "timer/timer.h"
#include "matrix_keyboard.h"
#include <string> 
#include <stdlib.h> 
#include <iostream> 
#include "proto_buff.h"
//using namespace escpplib;
using namespace std::placeholders;

int32_t cmd=5;

void Advance(int32_t l, int32_t r) {
    LOG_INFO("testkey l:%d, r:%d", l, r);
    cmd = 1;
}
void Retreat(int32_t l, int32_t r) {
    LOG_INFO("testkey l:%d, r:%d", l, r);
    cmd = 2;
}
void Left(int32_t l, int32_t r) {
    LOG_INFO("testkey l:%d, r:%d", l, r);
    cmd = 3;
}
void Right(int32_t l, int32_t r) {
    LOG_INFO("testkey l:%d, r:%d", l, r);
    cmd = 4;
}
void Stop(int32_t l, int32_t r) {
    LOG_INFO("testkey l:%d, r:%d", l, r);
    cmd = 5;
}


int main(int argc, char** argv){
    if (argc < 3) {
        LOG_FATAL("please input ip and port");
    }

    LOG_INFO("tank remote ctl  test");

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

    m_k.RegisterFunc(2, 2, std::bind(Advance, _1, _2));
    m_k.RegisterFunc(3, 2, std::bind(Retreat, _1, _2));
    m_k.RegisterFunc(3, 1, std::bind(Left, _1, _2));
    m_k.RegisterFunc(3, 3, std::bind(Right, _1, _2));
    m_k.RegisterFunc(2, 3, std::bind(Stop, _1, _2));
    m_k.StartWithoutScan();

    escpplib::CSocket client_socket;
    int ret = client_socket.Connect(argv[1], atoi(argv[2]));
    if (ret != 0){
        LOG_FATAL_MSG("listen_socket.Connect");
    }

    escpplib::default_proto_head_t head;
    void* buff = (void*)&head;
    size_t len = sizeof(escpplib::default_proto_head_t);
    
    int32_t old_cmd=0;
    while(1){
        int32_t cnt = m_k.Scan();
        if (cnt == 0) cmd=5;

        if (old_cmd == cmd) {
            delay(50);
            continue;
        }
        old_cmd = cmd;

        head.head_size = sizeof(escpplib::default_proto_head_t);
        head.body_size = 0;
        head.cmd = cmd;

        ret = client_socket.Send(buff, len, 0);
        LOG_INFO("client_socket.SendStr:%d", ret);
        
        delay(50);
    }


    return 0;
}
