// takes from breadboard, to control the 3-color light
#pragma once

#include "nrf.h"
#include "microbit_v2.h"

typedef enum{
    Red = EDGE_P8, // I'm assuming orig val 8 is incorrect, since it doesn't light
    Blue = EDGE_P14, // this is, for sure, wrong, this is button A...
    Green = EDGE_P12,  // This is, for sure, correct
} color_t;

void lights_init(void);

void light_turn_on(color_t light);

void light_turn_off(color_t light);