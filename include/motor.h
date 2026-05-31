#ifndef MOTOR_H
#define MOTOR_H

#include <Arduino.h>

void motors_init(uint8_t leftIn1, uint8_t leftIn2, uint8_t rightIn1, uint8_t rightIn2);

void leftMotorSet(int speed);
void rightMotorSet(int speed);

void motorsStop();
void leftMotorStop();
void rightMotorStop();

#endif