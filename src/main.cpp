#include <Arduino.h>

#define ENC_A_PIN 17

#define IN1_PIN 9
#define IN1_CH 0
#define IN2_PIN 10
#define IN2_CH 1

#define PWM_FREQ 20000
#define PWM_RES 8

#define samplingTime 25 // ms

volatile uint32_t encoder_count = 0;

void IRAM_ATTR encoderA_ISR()
{
  encoder_count++;
}

// motor control related functions
void motor_init(int IN1, int IN2);
void motor(int speed);
// PID control related functions
float motorSpeed(float dt);
float calcError(float target, float measured);
int PI_update(float error, float dt);

void setup()
{
  Serial.begin(115200);
  delay(1000);
  // Configure the encoder A pin as input with pull-up resistor
  pinMode(ENC_A_PIN, INPUT_PULLUP);
  // Attach the interrupt to the encoder A pin, triggering on the falling edge
  attachInterrupt(digitalPinToInterrupt(ENC_A_PIN), encoderA_ISR, FALLING);

  Serial.println("Encoder A impulse counter test");

  motor_init(IN1_PIN, IN2_PIN);
  motor(0); // Start with motor stopped
}
// ---------------------------------------------------------------------------- //
// Main loop to read encoder speed, calculate PID output, and control the motor //
//----------------------------------------------------------------------------- //
void loop()
{
  static uint32_t lastControl = millis();
  uint32_t now = millis();

  if (now - lastControl >= samplingTime)
  {
    float dt = (now - lastControl) / 1000.0f; // Convert ms to seconds
    lastControl = now;

    static float expectedSpeed = 1000.0;

    //----------------------------------------------------------------------------------//
    //test code to change target speed every 5 seconds and observe the control response //
    //----------------------------------------------------------------------------------//
    uint32_t testTime = millis() % 20000; // Loop test time every 20 seconds

    if(testTime < 5000) // Run the control loop for the first 5 seconds
    {
      expectedSpeed = 2000.0; // counts/s
    }
    else if(testTime < 5000*2) // Then change the target speed for the next 5 seconds
    {
      expectedSpeed = 500.0; // counts/s
    }
    else if(testTime < 5000*3) // Then change the target speed again for the next 5 seconds
    {
      expectedSpeed = 900.0;
    }
    else // Then change the target speed again for the next 5 seconds
    {
      expectedSpeed = 1500.0;
    }

    float speed = motorSpeed(dt);
    float error = calcError(expectedSpeed, speed);
    int pwm = PI_update(error, dt);
    motor(pwm);

    Serial.print("Target [counts/s]: ");
    Serial.print(expectedSpeed);
    Serial.print(" | Measured [counts/s]: ");
    Serial.print(speed);
    Serial.print(" | Error: ");
    Serial.print(error);
    Serial.print(" | PWM: ");
    Serial.println(pwm);
  }
}
//----------------------------------------------------------------------------//
// Function to calculate motor speed based on encoder counts and time elapsed //
//----------------------------------------------------------------------------//
//TODO#1: Implement a more robust speed calculation method: 
// - varying sampling times 
// - switching to time between pulses at low speeds instead of counts per time window for better accuracy
//TODO#2: Consider implementing a moving average filter to smooth out speed measurements and reduce noise in the control loop
//TODO#3: Explore the use of a more advanced control algorithm (e.g., PID with feedforward) for improved motor performance and response time
float motorSpeed(float dt)
{
  static uint32_t lastCount = 0;

  uint32_t countNow;

  noInterrupts();
  countNow = encoder_count;
  interrupts();

  uint32_t deltaCount = countNow - lastCount;

  float speed = deltaCount / dt;

  lastCount = countNow;

  return speed;
}
//---------------------------------------------------------------//
// Function to calculate error between target and measured speed //
//---------------------------------------------------------------//
float calcError(float target, float measured)
{
  return target - measured;
}
//--------------------------------------------------------//
// Function to calculate PI control output based on error //
//--------------------------------------------------------//
//TODO: Implement integral term for better control performance
int PI_update(float error, float dt)
{
  static float integral = 0.0f;
  static float previous_error = 0.0f;

  // PID coefficients
  const float Kp = 0.4f; // Proportional gain
  const float Ki = 0.3f; // Integral gain
  const float Kd = 0.05f; // Derivative gain

  // Calculate integral and derivative
  integral += error * dt; // Integral term
  integral = constrain(integral, -1000.0f, 1000.0f); // Anti-windup for integral term
  // float derivative = (error - previous_error) / dt; // Derivative term

  // Calculate PID output
  int output = Kp * error + Ki * integral; // + Kd * derivative;

  // Update previous error
  previous_error = error;

  return constrain(output, -255, 255); // Constrain output to PWM range
}
//------------------------------------------------------------//
// Function to initialize motor control pins and PWM channels //
//------------------------------------------------------------// 
void motor_init(int IN1, int IN2) {
  // Implementation for motor initialization
  // Set the pin modes for IN1 and IN2
  ledcSetup(IN1_CH, PWM_FREQ, PWM_RES);
  ledcSetup(IN2_CH, PWM_FREQ, PWM_RES);
  // Attach the pins to the channels
  ledcAttachPin(IN1, IN1_CH);
  ledcAttachPin(IN2, IN2_CH);
}
//--------------------------------------------------------------------------//
// Function to control motor speed and direction based on input speed value //
//--------------------------------------------------------------------------//
void motor(int speed) {
  // Implementation for motor control
  // Use dir and speed parameters to control the motor
  speed = constrain(speed, -255, 255); // Constrain speed to valid PWM range

  if(speed > 0)
  {
    ledcWrite(IN1_CH, speed);
    ledcWrite(IN2_CH, 0);
  }
  else if(speed < 0)
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