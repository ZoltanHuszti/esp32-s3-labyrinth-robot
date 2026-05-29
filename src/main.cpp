#include <Arduino.h>

#include "encoder.h"
#include "motor.h"
#include "controller.h"
#include "test_profile.h"

// Encoder pins for single motor quadrature encoder 
// Motor A
#define ENC_A_PIN 17
#define ENC_B_PIN 18
// Motor B
// TODO: define pins and implement for both motors with quadrature logic

// Motor driver pins
// Motor A
#define IN1_PIN 9
#define IN2_PIN 10
// Motor B 
// #define IN3_PIN 11
// #define IN4_PIN 12

#define SAMPLING_TIME_MS 25

void setup()
{
    Serial.begin(115200);
    delay(1000);

    encoder_init(ENC_A_PIN, ENC_B_PIN);
    motor_init(IN1_PIN, IN2_PIN);

    Serial.println("ESP32-S3 Labyrinth Robot - Single Motor PI Control");
    delay(2000);
}

void loop()
{
    static uint32_t lastControl = millis();

    uint32_t now = millis();

    if (now - lastControl >= SAMPLING_TIME_MS)
    {
        float dt = (now - lastControl) / 1000.0f;
        lastControl = now;

        float targetSpeed = getTargetSpeed(5000.0f);
        float measuredSpeed = motorSpeed(dt);
        float error = calcError(targetSpeed, measuredSpeed);

        int pwm = PI_update(error, dt);

        motor(pwm);

        Serial.print("Target [counts/s]: ");
        Serial.print(targetSpeed);
        Serial.print(" | Measured [counts/s]: ");
        Serial.print(measuredSpeed);
        Serial.print(" | Error: ");
        Serial.print(error);
        Serial.print(" | PWM: ");
        Serial.println(pwm);
    }
}