#include "controller.h"

// Private structure to hold PI controller parameters and state
struct PIController {
    float Kp;
    float Ki;
    float integral;
    float integralLimit;
    int outputLimit;
};

static PIController leftController;
static PIController rightController;

static void PI_init(PIController &controller, float Kp, float Ki, float integralLimit, int outputLimit)
{
    controller.Kp = Kp;
    controller.Ki = Ki;
    controller.integral = 0.0f;
    controller.integralLimit = integralLimit;
    controller.outputLimit = outputLimit;
}

void controllers_init()
{
    PI_init(leftController, 0.03f, 0.3f, 10000.0f, 255);
    PI_init(rightController, 0.03f, 0.3f, 10000.0f, 255);
}

float calcError(float target, float measured)
{
    return target - measured;
}

static int PI_update(PIController &controller, float error, float dt)
{
    controller.integral += error * dt;
    controller.integral = constrain(controller.integral, -controller.integralLimit, controller.integralLimit);

    float output = controller.Kp * error + controller.Ki * controller.integral;
    output = constrain(output, -controller.outputLimit, controller.outputLimit);

    return (int)output;
}

int leftPI_update(float error, float dt)
{
    return PI_update(leftController, error, dt);
}

int rightPI_update(float error, float dt)
{
    return PI_update(rightController, error, dt);
}

static void PI_reset(PIController &controller)
{
    controller.integral = 0.0f;
}

void leftPI_reset()
{
    PI_reset(leftController);
}

void rightPI_reset()
{
    PI_reset(rightController);
}

void controllers_reset()
{
    leftPI_reset();
    rightPI_reset();
}