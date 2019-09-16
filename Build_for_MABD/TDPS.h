#ifndef _TEST_H_
#define _TEST_H_
#include "mbed.h"
#include "string.h"
#include <cmath>

// error code list
#define SUCCESS               0
#define FAILED                1
#define RGB_ERROR             2
#define ARM_ERROR             3
#define RADIO_ERROR           4
#define MODE_CODE_ERROR       5
#define NO_WHITE_BALANCE      6
#define RADIO_INIT_ERROR      7
#define RADIO_CONFIRM_ERROR   8

// mode code list
#define NONE            0
#define INIT            1
#define RGB             2
#define ARM             3
#define RADIO           4
#define WHITE_BALANCE   5

int arm();
void arm_init();

char rgb();
void rgb_init();
void white_balance();
void rgb_off();

int radio();
int radio_init();
#endif
