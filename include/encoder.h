#ifndef ENCODER_H
#define ENCODER_H

#include <Arduino.h>

void encoder_init(uint8_t encAPin);
float motorSpeed(float dt);
uint32_t getEncoderCount();
void resetEncoderCount();

#endif