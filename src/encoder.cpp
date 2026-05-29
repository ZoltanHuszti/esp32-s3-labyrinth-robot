#include "encoder.h"

// Encoder state variables
// This is for one motor 
static uint8_t encoderPinA = 0;
static uint8_t encoderPinB = 0;
//TODO: implement for two motors 

volatile int32_t encoder_count = 0;
static int32_t lastCount = 0;

// Non quadrature logic encoder ISR
// void IRAM_ATTR encoderA_ISR()
// {
//     encoder_count++;
// }

// Quadrature encoder ISR, x4 decoding
void IRAM_ATTR encoderA_ISR()
{
  bool a = digitalRead(encoderPinA);
  bool b = digitalRead(encoderPinB);

  if (a != b)
    encoder_count++;
  else
    encoder_count--;
}

void IRAM_ATTR encoderB_ISR()
{
  bool a = digitalRead(encoderPinA);
  bool b = digitalRead(encoderPinB);

  if (a == b)
    encoder_count++;
  else
    encoder_count--;
}

void encoder_init(uint8_t pinA, uint8_t pinB)
{
    encoderPinA = pinA;
    encoderPinB = pinB;

    pinMode(encoderPinA, INPUT_PULLUP);
    pinMode(encoderPinB, INPUT_PULLUP);

    attachInterrupt(digitalPinToInterrupt(encoderPinA), encoderA_ISR, CHANGE);
    attachInterrupt(digitalPinToInterrupt(encoderPinB), encoderB_ISR, CHANGE);


}

float motorSpeed(float dt)
{
    //static int32_t lastCount = 0;

    int32_t countNow;

    noInterrupts();
    countNow = encoder_count;
    interrupts();

    int32_t deltaCount = countNow - lastCount;
    lastCount = countNow;

    return deltaCount / dt;
}

int32_t getEncoderCount()
{
    int32_t countCopy;

    noInterrupts();
    countCopy = encoder_count;
    interrupts();

    return countCopy;
}

void resetEncoderCount()
{
    noInterrupts();
    encoder_count = 0;
    lastCount = 0;
    interrupts();
}