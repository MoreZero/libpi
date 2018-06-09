#ifndef LIBPI_KEYBOARD_H
#define LIBPI_KEYBOARD_H

#include <functional>
#include <stdint.h>
#include <wiringPi.h>
#include "pin.h"


typedef std::function<void(int32_t l, int32_t r)> KeyHandleFunc;

class MatrixKeyboard
{
public:
    MatrixKeyboard(Pin pin_l0, Pin pin_l1, Pin pin_l2, Pin pin_l3,
                   Pin pin_r0, Pin pin_r1, Pin pin_r2, Pin pin_r3);

    ~MatrixKeyboard() {}

    void StartISR();
    void Start();
    void ScanDown(int32_t l);

    void RegisterFunc(int32_t l, int32_t r, KeyHandleFunc func);

public:
    Pin     pin_l_array_[4];
    Pin     pin_r_array_[4];
    int32_t key_value_[4][4];
    
    KeyHandleFunc key_handle_matrix_[4][4];
    bool    is_stop_;
};


#endif
