#include "motor.h"

#define LEFT_IN1_CH 0
#define LEFT_IN2_CH 1
#define RIGHT_IN1_CH 2
#define RIGHT_IN2_CH 3

#define PWM_FREQ 20000
#define PWM_RES 8
#define PWM_MAX ((1 << PWM_RES) - 1)

struct Motor
{
    uint8_t in1Pin;
    uint8_t in2Pin;
    uint8_t ch1;
    uint8_t ch2;
};

static Motor leftMotor;
static Motor rightMotor;

// Helper function to initialize a motor and attach PWM channels
static void motorAttach(Motor& motor, uint8_t in1Pin, uint8_t in2Pin, uint8_t ch1, uint8_t ch2)
{
    motor.in1Pin = in1Pin;
    motor.in2Pin = in2Pin;
    motor.ch1 = ch1;
    motor.ch2 = ch2;

    ledcSetup(motor.ch1, PWM_FREQ, PWM_RES);
    ledcSetup(motor.ch2, PWM_FREQ, PWM_RES);

    ledcAttachPin(motor.in1Pin, motor.ch1);
    ledcAttachPin(motor.in2Pin, motor.ch2);
}

// Helper function to set motor speed and direction
static void motorSet(Motor& motor, int speed)
{
    speed = constrain(speed, -PWM_MAX, PWM_MAX);

    if (speed > 0)
    {
        ledcWrite(motor.ch1, speed);
        ledcWrite(motor.ch2, 0);
    }
    else if (speed < 0)
    {
        ledcWrite(motor.ch1, 0);
        ledcWrite(motor.ch2, -speed);
    }
    else
    {
        ledcWrite(motor.ch1, 0);
        ledcWrite(motor.ch2, 0);
    }
}

// Wrapper functions for left and right motors
void motors_init(uint8_t leftIn1, uint8_t leftIn2, uint8_t rightIn1, uint8_t rightIn2)
{
    motorAttach(leftMotor, leftIn1, leftIn2, LEFT_IN1_CH, LEFT_IN2_CH);
    motorAttach(rightMotor, rightIn1, rightIn2, RIGHT_IN1_CH, RIGHT_IN2_CH);

    motorsStop();
}

void leftMotorSet(int speed)
{
    motorSet(leftMotor, speed);
}

void rightMotorSet(int speed)
{
    motorSet(rightMotor, speed);
}

void motorsStop()
{
    leftMotorSet(0);
    rightMotorSet(0);
}

void leftMotorStop()
{
    leftMotorSet(0);
}

void rightMotorStop()
{
    rightMotorSet(0);
}