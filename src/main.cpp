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
    delay(2000);

    encoders_init(LEFT_ENC_A_PIN, LEFT_ENC_B_PIN, RIGHT_ENC_A_PIN, RIGHT_ENC_B_PIN);
    motors_init(LEFT_MOTOR_IN1_PIN, LEFT_MOTOR_IN2_PIN, RIGHT_MOTOR_IN3_PIN, RIGHT_MOTOR_IN4_PIN);
    controllers_init();

    Serial.println("ESP32-S3 Labyrinth Robot - Dual Motor Control Test");
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

        float leftTargetSpeed; // Target speed in counts/s (example value, adjust as needed)
        float rightTargetSpeed; // Target speed in counts/s (example value, adjust as needed)

        uint32_t testTime = millis() % 6000;

        if(testTime < 4000)
        {
            leftTargetSpeed = -3000.0f;
            rightTargetSpeed = 3000.0f;
        }
        else
        {
            leftTargetSpeed = 0.0f;
            rightTargetSpeed = 0.0f;
        }
        
        float leftMotorMeasuredSpeed = leftMotorSpeed(dt);
        float rightMotorMeasuredSpeed = rightMotorSpeed(dt);

        float leftError = calcError(leftTargetSpeed, leftMotorMeasuredSpeed);
        float rightError = calcError(rightTargetSpeed, rightMotorMeasuredSpeed);

        int leftPWM = leftPI_update(leftError, dt);
        int rightPWM = rightPI_update(rightError, dt);

        leftMotorSet(leftPWM);
        rightMotorSet(rightPWM);

        Serial.print("TargetL [counts/s]: ");
        Serial.print(leftTargetSpeed);
        Serial.print(" | MeasuredL [counts/s]: ");
        Serial.print(leftMotorMeasuredSpeed);
        Serial.print(" | ErrorL: ");
        Serial.print(leftError);
        Serial.print(" | PWML: ");
        Serial.print(leftPWM);

        Serial.print(" || TargetR [counts/s]: ");
        Serial.print(rightTargetSpeed);
        Serial.print(" | MeasuredR [counts/s]: ");
        Serial.print(rightMotorMeasuredSpeed);
        Serial.print(" | ErrorR: ");
        Serial.print(rightError);
        Serial.print(" | PWMR: ");
        Serial.println(rightPWM);
    }
}