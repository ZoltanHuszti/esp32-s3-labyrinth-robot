#ifndef ENCODER_H
#define ENCODER_H

#include <Arduino.h>

void encoders_init(uint8_t leftA, uint8_t leftB, uint8_t rightA, uint8_t rightB);

float leftMotorSpeed(float dt);
float rightMotorSpeed(float dt);

int32_t getLeftEncoderCount();
int32_t getRightEncoderCount();

void resetLeftEncoderCount();
void resetRightEncoderCount();

#endif