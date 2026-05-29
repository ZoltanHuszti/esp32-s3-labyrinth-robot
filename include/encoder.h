#ifndef ENCODER_H
#define ENCODER_H

#include <Arduino.h>

void encoder_init(uint8_t pinA, uint8_t pinB);
float motorSpeed(float dt);
int32_t getEncoderCount();
void resetEncoderCount();

#endif