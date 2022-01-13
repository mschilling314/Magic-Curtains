// simply iconic i2c functionality to drive the motor

#pragma once

#include "nrf_twi_mngr.h"

void i2c_init(const nrf_twi_mngr_t* i2c);

void i2c_reg_write(uint8_t i2c_addr, uint8_t reg_addr, uint8_t data);

uint8_t i2c_reg_read(uint8_t i2c_addr, uint8_t reg_addr);