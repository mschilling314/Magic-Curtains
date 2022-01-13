// the .c file to actually spin the motor
#include "motor.h"
#include "i2c.h"

void setMotorSpeed(motor m, motor_direction dir, uint8_t speed){
    uint8_t pwr = speed;
    if (pwr > 127) pwr = 127;
    if (dir == Forward) pwr = pwr + 128;
    //printf("pwr: %u", pwr);
    i2c_reg_write(MOTOR_ADDRESS, m, pwr);
    //printf("Motor Address: %u, Motor: %u, Power:  %u\n",
     //      MOTOR_ADDRESS,
    //       MOTOR_POWER,
       //    pwr);
}

void enableMotor(motor_power p){
    i2c_reg_write(MOTOR_ADDRESS, MOTOR_POWER, p);
}