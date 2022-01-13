#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <math.h>

#include "microbit_v2.h"
#include "nrf.h"

#include "light.h"
#include "motor.h"

static const uint8_t STATE_SHIFT = 15;

typedef enum{
    Up,
    Down,
    Transing,
} curtain_State;

struct machine_State{
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
    uint8_t endTransSec;
    uint8_t endTransMin;
    float light;
    float temp;
    curtain_State cur;
    curtain_State dest;
    bool butA;
    bool butB;
    uint8_t priority;
};

void print_state(struct machine_State* state);

void algo(struct machine_State* state);