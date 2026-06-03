#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <Arduino.h>

void controllers_init();

float calcError(float target, float measured);

int leftPI_update(float error, float dt);
int rightPI_update(float error, float dt);

void leftPI_reset();
void rightPI_reset();
void controllers_reset();

#endif