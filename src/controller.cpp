#include "controller.h"

static float integral = 0.0f;

float calcError(float target, float measured)
{
    return target - measured;
}

int PI_update(float error, float dt)
{
    const float Kp = 0.03f; 
    const float Ki = 0.3f;

    integral += error * dt;
    integral = constrain(integral, -10000.0f, 10000.0f);

    float output = Kp * error + Ki * integral;

    return constrain((int)output, -255, 255);
}

void PI_reset()
{
    integral = 0.0f;
}