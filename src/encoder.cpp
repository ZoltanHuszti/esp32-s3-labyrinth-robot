#include "encoder.h"

// Private struct
struct Encoder  
{
    uint8_t pinA;
    uint8_t pinB;
    volatile int32_t count;
    int32_t lastCount;
}; 

static Encoder leftEncoder;
static Encoder rightEncoder;

//-------------------------//
// left motor encoder ISRs //
//-------------------------//
static void IRAM_ATTR leftEncoderA_ISR()
{
  bool a = digitalRead(leftEncoder.pinA);
  bool b = digitalRead(leftEncoder.pinB);

  if (a != b)
    leftEncoder.count++;
  else
    leftEncoder.count--;
}

static void IRAM_ATTR leftEncoderB_ISR()
{
  bool a = digitalRead(leftEncoder.pinA);
  bool b = digitalRead(leftEncoder.pinB);

  if (a == b)
    leftEncoder.count++;
  else
    leftEncoder.count--;
}

// NOTE: On some ESP32/Arduino builds, static ISR functions with IRAM_ATTR
// may cause issues. If so, remove static from the ISR declarations.

//--------------------------//
// right motor encoder ISRs //
//--------------------------//
static void IRAM_ATTR rightEncoderA_ISR() 
{
  bool a = digitalRead(rightEncoder.pinA);
  bool b = digitalRead(rightEncoder.pinB);

  if (a != b)
    rightEncoder.count++;
  else
    rightEncoder.count--;
}

static void IRAM_ATTR rightEncoderB_ISR()
{
  bool a = digitalRead(rightEncoder.pinA);
  bool b = digitalRead(rightEncoder.pinB);

  if (a == b)
    rightEncoder.count++;
  else
    rightEncoder.count--;
}

//-------------------------------------------//
// Encoder initialization and ISR attachment //
//-------------------------------------------// 

// Helper function to initialize an encoder and attach ISRs
static void attachEncoder(Encoder& enc, uint8_t pinA, uint8_t pinB)
{
    enc.pinA = pinA;
    enc.pinB = pinB;
    enc.count = 0;
    enc.lastCount = 0;

    pinMode(enc.pinA, INPUT_PULLUP);
    pinMode(enc.pinB, INPUT_PULLUP);
}

// Main initialization function for both encoders
void encoders_init(uint8_t leftA, uint8_t leftB, uint8_t rightA, uint8_t rightB) 
{
    attachEncoder(leftEncoder, leftA, leftB);
    attachEncoder(rightEncoder, rightA, rightB);

    attachInterrupt(digitalPinToInterrupt(leftEncoder.pinA), leftEncoderA_ISR, CHANGE);
    attachInterrupt(digitalPinToInterrupt(leftEncoder.pinB), leftEncoderB_ISR, CHANGE);

    attachInterrupt(digitalPinToInterrupt(rightEncoder.pinA), rightEncoderA_ISR, CHANGE);
    attachInterrupt(digitalPinToInterrupt(rightEncoder.pinB), rightEncoderB_ISR, CHANGE);
}

//---------------------------//
// Encoder utility functions //
//---------------------------//

// Helper function to calculate speed from encoder counts
static float encoderSpeed(Encoder& enc, float dt)
{
    int32_t countNow;

    noInterrupts();
    countNow = enc.count;
    interrupts();

    int32_t deltaCount = countNow - enc.lastCount;
    enc.lastCount = countNow;

    return deltaCount / dt;
}

// Wrapper functions for left and right motor speeds
float leftMotorSpeed(float dt)
{
    return encoderSpeed(leftEncoder, dt);
}

float rightMotorSpeed(float dt)
{
    return encoderSpeed(rightEncoder, dt);
}

// Helper function to get current encoder count safely
static int32_t getEncoderCount(Encoder& enc)
{
    int32_t countCopy;

    noInterrupts();
    countCopy = enc.count;
    interrupts();

    return countCopy;
}

// Wrapper functions for left and right motor encoder counts
int32_t getLeftEncoderCount()
{
    return getEncoderCount(leftEncoder);
}

int32_t getRightEncoderCount()
{
    return getEncoderCount(rightEncoder);
}

// Helper function to reset encoder counts safely
static void resetEncoderCount(Encoder& enc)
{
    noInterrupts();
    enc.count = 0;
    enc.lastCount = 0;
    interrupts();
}

// Wrapper functions for left and right motor encoder count resets
void resetLeftEncoderCount()
{
    resetEncoderCount(leftEncoder);
}

void resetRightEncoderCount()
{
    resetEncoderCount(rightEncoder);
}