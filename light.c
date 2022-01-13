// implements light.h, but as the .c file

#include "gpio.h"
#include "light.h"

void light_turn_on(color_t light){
    gpio_clear(light);
}

void light_turn_off(color_t light){
    gpio_set(light);
}

void lights_init(void){
    gpio_config(Red,1);
    gpio_config(Blue,1);
    gpio_config(Green,1);
    light_turn_off(Red);
    light_turn_off(Blue);
    light_turn_off(Green);
}

