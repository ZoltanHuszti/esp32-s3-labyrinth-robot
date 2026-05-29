#include "motor.h"

#define IN1_CH 0
#define IN2_CH 1

#define PWM_FREQ 20000
#define PWM_RES 8

static uint8_t motorIn1Pin = 0;
static uint8_t motorIn2Pin = 0;

void motor_init(uint8_t in1Pin, uint8_t in2Pin)
{
    motorIn1Pin = in1Pin;
    motorIn2Pin = in2Pin;

    ledcSetup(IN1_CH, PWM_FREQ, PWM_RES);
    ledcSetup(IN2_CH, PWM_FREQ, PWM_RES);

    ledcAttachPin(motorIn1Pin, IN1_CH);
    ledcAttachPin(motorIn2Pin, IN2_CH);

    motor(0);
}

void motor(int speed)
{
    speed = constrain(speed, -255, 255);

    if (speed > 0)
    {
        ledcWrite(IN1_CH, speed);
        ledcWrite(IN2_CH, 0);
    }
    else if (speed < 0)
    {
        ledcWrite(IN1_CH, 0);
        ledcWrite(IN2_CH, -speed);
    }
    else
    {
        ledcWrite(IN1_CH, 0);
        ledcWrite(IN2_CH, 0);
    }
}