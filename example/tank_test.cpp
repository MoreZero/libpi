#include "log.h"
#include "pin.h"
#include "soft_pwm.h"
#include "event.h"
#include "es_socket.h"
#include "proto_buff.h"
#include "motor_engine.h"
#include <string> 
using namespace std;
//using namespace escpplib;


class Tank
{
public:
    Tank(MotorEngine& engine):engine_(engine) {

    }

    void Advance() {
        engine_.Advance(0);
        engine_.Advance(1);
    }

    void Retreat() {
        engine_.Retreat(0);
        engine_.Retreat(1);
    }

    void Left() {
        engine_.Retreat(1);
        engine_.Advance(0);
    }
    
    void Right() {
        engine_.Advance(1);
        engine_.Retreat(0);
    }

    void Stop() {
        engine_.Stop(0);
        engine_.Stop(1);
    }
    
private:
    MotorEngine& engine_;
};

// -----------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------
using namespace std::placeholders;
    
escpplib::CEventEpoll event;

class NetTankProtoBuff
{
public:
    NetTankProtoBuff(Tank& tank):tank_(tank) {
        //proto_buff_.SetProtoHeadParseFunc(std::bind(&NetTankProtoBuff::HeadParseFunc, this, _1, _2, _3, _4, _5));
        proto_buff_.RegisterFunc(1, std::bind(&NetTankProtoBuff::Advance, this, _1, _2, _3, _4));
        proto_buff_.RegisterFunc(2, std::bind(&NetTankProtoBuff::Retreat, this, _1, _2, _3, _4));
        proto_buff_.RegisterFunc(3, std::bind(&NetTankProtoBuff::Left, this, _1, _2, _3, _4));
        proto_buff_.RegisterFunc(4, std::bind(&NetTankProtoBuff::Right, this, _1, _2, _3, _4));
        proto_buff_.RegisterFunc(5, std::bind(&NetTankProtoBuff::Stop, this, _1, _2, _3, _4));
    }

    int32_t Advance(const int8_t* head, int32_t head_size, const int8_t* body, int32_t body_size) {
        LOG_INFO("Advance head_size:%d, body_size:%d", head_size, body_size);
        tank_.Advance();
        return 0;
    }

    int32_t Retreat(const int8_t* head, int32_t head_size, const int8_t* body, int32_t body_size) {
        LOG_INFO("Retreat head_size:%d, body_size:%d", head_size, body_size);  
        tank_.Retreat();
        return 0;
    }
    
    int32_t Left(const int8_t* head, int32_t head_size, const int8_t* body, int32_t body_size) {
        LOG_INFO("Left head_size:%d, body_size:%d", head_size, body_size);  
        tank_.Left();
        return 0;
    }
    
    int32_t Right(const int8_t* head, int32_t head_size, const int8_t* body, int32_t body_size) {
        LOG_INFO("Right head_size:%d, body_size:%d", head_size, body_size);  
        tank_.Right();
        return 0;
    }

    int32_t Stop(const int8_t* head, int32_t head_size, const int8_t* body, int32_t body_size) {
        LOG_INFO("Stop head_size:%d, body_size:%d", head_size, body_size);  
        tank_.Stop();
        return 0;
    }
    
    //int32_t HeadParseFunc(const int8_t* buff, int32_t size, 
    //                             int32_t& cmd, int32_t& head_size, int32_t& body_size) {
    //    if (size < 8) return 0;
    //    if (buff[0] == ) {
    //        cmd = 1;
    //    }else if (buff[0] == 'b') {
    //        cmd = 2;
    //    }else {
    //        return -1;
    //    }

    //    head_size = 2;
    //    body_size = 6;
    //    return 1;
    //}

    int32_t Recv() {
        int8_t buf[65536];

        ssize_t cnt = socket_.Recv(buf, 65536, 0);
        if (cnt <= 0) return cnt;

        LOG_INFO("Recv cnt:%d", (int)cnt);
        int32_t ret = proto_buff_.Write(buf, cnt);
        if (ret < 0) return -1;

        return 1;
    }

    int32_t Close() {
        return socket_.Close();
    }
    escpplib::CSocket& Socket(){return socket_;}
private:
    escpplib::ProtoBuff proto_buff_;
    escpplib::CSocket socket_;

    Tank& tank_;

};

NetTankProtoBuff* g_tank_proto_buff=NULL;



int main(int argc, char** argv){
    
    LOG_INFO("start");
	wiringPiSetup();
    

    Pin pin_pwm_a(1);

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
    MotorEngine engine(pin_a_1, pin_a_2, pin_b_1, pin_b_2, pin_pwm_a, pin_pwm_a);

    Tank tank(engine);
    tank.Stop();

    escpplib::CSocket listen_socket;
    int ret = listen_socket.Bind("0.0.0.0", 50000);
    if (ret != 0){
        LOG_FATAL_MSG("listen_socket.Bind");
    }
    ret = listen_socket.Listen(1024);
    if (ret != 0){
        LOG_FATAL_MSG("listen_socket.Listen");
    }
    LOG_INFO("socket fd:%d", listen_socket.GetFd());

    if (event.RegEvent(listen_socket.GetFd(), EPOLLIN) != 0) {
        LOG_ERROR_MSG("event.RegEvent listen_socket");
    }

    
    int32_t current_fd=-1;
    while(true) {
        LOG_INFO("event.Wait(60*1000)");
        ret = event.Wait(60 * 1000);
        if (ret == -1) {
            LOG_FATAL_MSG("event.Wait error");
        }

        LOG_INFO("ret:%d fd:", ret);
        for (int i=0; i<ret; ++i) {
            struct epoll_event& events = event.epoll_event_[i];
            LOG_INFO("event fd:%d", events.data.fd);
            if (events.data.fd == listen_socket.GetFd()) {
                LOG_INFO("listen_socket event");

                NetTankProtoBuff* tank_proto_buff = new NetTankProtoBuff(tank);
                ret = listen_socket.Accept(tank_proto_buff->Socket());
                if (ret != 0){
                    LOG_ERROR_MSG("listen_socket.Listen errno:%d EAGAIN:%d", errno, EAGAIN);
                }
                LOG_INFO("new connect fd:%d", tank_proto_buff->Socket().GetFd());
    

                if (g_tank_proto_buff != NULL && g_tank_proto_buff->Socket().GetStatus() != escpplib::CSocket::CLOSED) {
                    LOG_ERROR_MSG("already connect, close");
                    tank_proto_buff->Close();
                    delete tank_proto_buff;
                    continue;
                }


                if (event.RegEvent(tank_proto_buff->Socket().GetFd(), EPOLLIN) != 0) {
                    LOG_ERROR_MSG("event.RegEvent socket");
                    tank_proto_buff->Close();
                    continue;
                }
                g_tank_proto_buff = tank_proto_buff;
                current_fd = tank_proto_buff->Socket().GetFd();

            } else if (events.data.fd == current_fd) {
                int32_t ret = g_tank_proto_buff->Recv();
                if (ret <= 0) {
                    LOG_INFO("need close fd:%d",g_tank_proto_buff->Socket().GetFd());
                    // TODO 取消注册
                    if (event.RmEvent(g_tank_proto_buff->Socket().GetFd()) != 0) {
                        LOG_ERROR_MSG("event.RegEvent socket");
                    }
                    g_tank_proto_buff->Close();

                    delete g_tank_proto_buff;
                    g_tank_proto_buff = NULL;
                }
            } else {
                LOG_INFO("bug fd");
            }
        } 
    }


    pin_pwm_a.SetOutput();

    return 0;
}
