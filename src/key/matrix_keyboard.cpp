#include "log.h"
#include "matrix_keyboard.h"

MatrixKeyboard* g_matrix_key_board;

void IntKeyL0() {
    LOG_DEBUG("IntKeyL0");
    g_matrix_key_board->ScanDown(0);
}
void IntKeyL1() {
    LOG_DEBUG("IntKeyL1");
    g_matrix_key_board->ScanDown(1);
}
void IntKeyL2() {
    LOG_DEBUG("IntKeyL2");
    g_matrix_key_board->ScanDown(2);
}
void IntKeyL3() {
    LOG_DEBUG("IntKeyL3");
    g_matrix_key_board->ScanDown(3);
}

MatrixKeyboard::MatrixKeyboard(Pin pin_l0, Pin pin_l1, Pin pin_l2, Pin pin_l3,
                               Pin pin_r0, Pin pin_r1, Pin pin_r2, Pin pin_r3):is_stop_(true) {
    pin_l_array_[0] = pin_l0;
    pin_l_array_[1] = pin_l1;
    pin_l_array_[2] = pin_l2;
    pin_l_array_[3] = pin_l3;
    
    pin_r_array_[0] = pin_r0;
    pin_r_array_[1] = pin_r1;
    pin_r_array_[2] = pin_r2;
    pin_r_array_[3] = pin_r3;

    
    for (int32_t i=0; i<4; ++i) {
        LOG_DEBUG("get l pin:%d", pin_l_array_[i].PinId());
        LOG_DEBUG("get r pin:%d", pin_r_array_[i].PinId());
    }


    for (int32_t i=0; i<4; ++i) {
        for (int32_t j=0; j<4; ++j) {
            key_value_[i][j] = 0;
        }
    }

}

void MatrixKeyboard::StartISR() {
    g_matrix_key_board = this;
    for (int32_t i=0; i<4; ++i) {
        pin_r_array_[i].SetOutput();
        pin_r_array_[i].SetLow();
        
        pin_l_array_[i].SetInput();
        pin_l_array_[i].PullUp();
    }
    wiringPiISR(pin_l_array_[0].PinId(), INT_EDGE_FALLING, IntKeyL0);
    wiringPiISR(pin_l_array_[1].PinId(), INT_EDGE_FALLING, IntKeyL1);
    wiringPiISR(pin_l_array_[2].PinId(), INT_EDGE_FALLING, IntKeyL2);
    wiringPiISR(pin_l_array_[3].PinId(), INT_EDGE_FALLING, IntKeyL3);
}

void MatrixKeyboard::Start() {
    for (int32_t i=0; i<4; ++i) {
        pin_r_array_[i].SetOutput();
        pin_r_array_[i].SetLow();
        
        pin_l_array_[i].SetInput();
        pin_l_array_[i].PullUp();
    }
    is_stop_ = false;

    while (!is_stop_) {
        for (int32_t i=0; i<4; ++i) {
            if (LOW == pin_l_array_[i].Level()) {
                ScanDown(i);
            }
        }
        delay(100);
    }
}

void MatrixKeyboard::StartWithoutScan() {
    for (int32_t i=0; i<4; ++i) {
        pin_r_array_[i].SetOutput();
        pin_r_array_[i].SetLow();
        
        pin_l_array_[i].SetInput();
        pin_l_array_[i].PullUp();
    }
}

int32_t MatrixKeyboard::Scan() {
    int32_t down_cnt=0;
    for (int32_t i=0; i<4; ++i) {
        if (LOW == pin_l_array_[i].Level()) {
            down_cnt += ScanDown(i);
        }
    }
    return down_cnt;
}

int32_t MatrixKeyboard::ScanDown(int32_t l) {
//    LOG_DEBUG("ScanDown l%d", l);
    int32_t down_cnt=0;
    for (int32_t i=0; i<4; ++i) {
        pin_r_array_[i].SetHigh();
    }
    for (int32_t i=0; i<4; ++i) {
        pin_r_array_[i].SetLow();
        if (LOW == pin_l_array_[l].Level()) {
            LOG_DEBUG("key l:%d r:%d", l, i);
            KeyHandleFunc& func = key_handle_matrix_[l][i];
            if (func) {
                ++down_cnt;
                func(l, i);
            }
        }
        pin_r_array_[i].SetHigh();
    }
    for (int32_t i=0; i<4; ++i) {
        pin_r_array_[i].SetLow();
    }
    return down_cnt;
}

void MatrixKeyboard::RegisterFunc(int32_t l, int32_t r, KeyHandleFunc func) {
    key_handle_matrix_[l][r] = func;
}





