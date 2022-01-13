#include "gpio.h"

typedef struct{
  // Step 3:
  // Add register definitions here
  uint32_t ga[321];
  uint32_t OUT;
  uint32_t OUTSET;
  uint32_t OUTCLR;
  uint32_t IN;
  uint32_t DIR;
  uint32_t DIRSET;
  uint32_t DIRCLR;
  uint32_t LATCH;
  uint32_t DETECTMODE;
  uint32_t g2[118];
  uint32_t PIN_CNF[32];
} gpio_reg_t;

volatile gpio_reg_t* P0 = (gpio_reg_t*)(0x50000000);
volatile gpio_reg_t* P1 = (gpio_reg_t*)(0x50000300);

// Inputs: 
//  gpio_num - gpio number 0-31 OR (32 + gpio number)
//  dir - gpio direction (INPUT, OUTPUT)
void gpio_config(uint8_t gpio_num, gpio_direction_t dir) {
  // Implement me
  // Hint: Use proper PIN_CNF instead of DIR
  if (dir == 1){
    if (gpio_num < 32){
      P0->DIRSET = 1<<gpio_num;
    }
    else {
      P1->DIRSET = 1<<(gpio_num - 32);
    }
  }
  else if (dir == 0){
    if (gpio_num < 32){
      P0->PIN_CNF[gpio_num] = 0x0003000C;
    }
    else {
      P1->PIN_CNF[gpio_num - 32] = 0x0003000C;
    }
  }
}

// Inputs: 
//  gpio_num - gpio number 0-31 OR (32 + gpio number)
void gpio_set(uint8_t gpio_num) {
  // Implement me
  if (gpio_num < 32){
    P0->OUTSET = 1<<gpio_num;
  }
  else {
    P1->OUTSET = 1<<(gpio_num - 32);
  }
}

// Inputs: 
//  gpio_num - gpio number 0-31 OR (32 + gpio number)
void gpio_clear(uint8_t gpio_num) {
  // Implement me
  if (gpio_num < 32){
    P0->OUTCLR = 1<<gpio_num;
  }
  else {
    P1->OUTCLR = 1<<(gpio_num - 32);
  }
}

// Inputs: 
//  gpio_num - gpio number 0-31 OR (32 + gpio number)
// Output:
//  bool - pin state (true == high)
bool gpio_read(uint8_t gpio_num) {
  if (gpio_num < 32){
    uint32_t inp = P0->IN;
    inp = inp & (1<<gpio_num);
    inp = inp>>gpio_num;
    if (inp == 0) return false;
    else return true;
  }
  else {
    uint32_t inp = P1->IN;
    inp = inp & (1<<(gpio_num - 32));
    inp = inp>>(gpio_num - 32);
    if (inp == 0) return false;
    else return true;
  }
  return true;
}
