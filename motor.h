// a truly original file, .h for the motor
#pragma once

#include <stdint.h>

typedef enum{
    Left = 33, // originally 8448, but that's actually wrong
    Right = 32, // originally 8192, but that's actually wrong
} motor;

typedef enum{
    Forward = 0,
    Reverse = 1,
} motor_direction;

typedef enum{
    On = 1, // was originally 28673, wrong tho
    Off = 0, // was originally 28672, wrong tho
} motor_power;

// reason original addresses were wrong is because of the difference in
// our i2c write and the original .ts version, namely they packaged
// the reg_addr and data into a single word of input, we give them sep.

static const uint8_t MOTOR_ADDRESS = 89;
static const uint8_t MOTOR_POWER = 112;

void setMotorSpeed(motor m, motor_direction dir, uint8_t speed);

void enableMotor(motor_power p);