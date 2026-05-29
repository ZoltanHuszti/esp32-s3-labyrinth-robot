#include "encoder.h"

static uint8_t encoderPinA = 0;
volatile uint32_t encoder_count = 0;

void IRAM_ATTR encoderA_ISR()
{
    encoder_count++;
}

void encoder_init(uint8_t encAPin)
{
    encoderPinA = encAPin;

    pinMode(encoderPinA, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(encoderPinA), encoderA_ISR, FALLING);
}

float motorSpeed(float dt)
{
    static uint32_t lastCount = 0;

    uint32_t countNow;

    noInterrupts();
    countNow = encoder_count;
    interrupts();

    uint32_t deltaCount = countNow - lastCount;
    lastCount = countNow;

    return deltaCount / dt;
}

uint32_t getEncoderCount()
{
    uint32_t countCopy;

    noInterrupts();
    countCopy = encoder_count;
    interrupts();

    return countCopy;
}

void resetEncoderCount()
{
    noInterrupts();
    encoder_count = 0;
    interrupts();
}