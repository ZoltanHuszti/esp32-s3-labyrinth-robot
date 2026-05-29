#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <Arduino.h>

float calcError(float target, float measured);
int PI_update(float error, float dt);
void PI_reset();

#endif