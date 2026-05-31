#include <Arduino.h>

#include "encoder.h"
#include "motor.h"
#include "controller.h"
#include "test_profile.h"

// ENCODER pins
#define LEFT_ENC_A_PIN 17
#define LEFT_ENC_B_PIN 18
#define RIGHT_ENC_A_PIN 19
#define RIGHT_ENC_B_PIN 20

// MOTOR pins
#define LEFT_MOTOR_IN1_PIN 9
#define LEFT_MOTOR_IN2_PIN 10 
#define RIGHT_MOTOR_IN3_PIN 11
#define RIGHT_MOTOR_IN4_PIN 12

#define SAMPLING_TIME_MS 25

void setup()
{
    Serial.begin(115200);
    delay(1000);

    encoders_init(LEFT_ENC_A_PIN, LEFT_ENC_B_PIN, RIGHT_ENC_A_PIN, RIGHT_ENC_B_PIN);
    motors_init(LEFT_MOTOR_IN1_PIN, LEFT_MOTOR_IN2_PIN, RIGHT_MOTOR_IN3_PIN, RIGHT_MOTOR_IN4_PIN);

    Serial.println("ESP32-S3 Labyrinth Robot - Dual Motor Encoder Test");
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

        //float targetSpeed = getTargetSpeed(5000.0f);
        float leftMotorMeasuredSpeed = leftMotorSpeed(dt);
        float rightMotorMeasuredSpeed = rightMotorSpeed(dt);
        //float error = calcError(targetSpeed, measuredSpeed);

        leftMotorSet(255);
        rightMotorSet(255);

        //int pwm = PI_update(error, dt);

        //motor(pwm);

        // Serial.print("Target [counts/s]: ");
        // Serial.print(targetSpeed);
        Serial.print("Left measured [counts/s]: ");
        Serial.print(leftMotorMeasuredSpeed);
        Serial.print(" | Right measured [counts/s]: ");
        Serial.println(rightMotorMeasuredSpeed);
        // Serial.print(" | Error: ");
        // Serial.print(error);
        // Serial.print(" | PWM: ");
        // Serial.println(pwm);
    }
}