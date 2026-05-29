#ifndef MOTOR_H
#define MOTOR_H

#include <Arduino.h>

void motor_init(uint8_t in1Pin, uint8_t in2Pin);
void motor(int speed);

#endif