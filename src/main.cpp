#include <Arduino.h>

#include "encoder.h"
#include "motor.h"
#include "controller.h"
#include "test_profile.h"

#define ENC_A_PIN 17

#define IN1_PIN 9
#define IN2_PIN 10

#define SAMPLING_TIME_MS 25

void setup()
{
    Serial.begin(115200);
    delay(1000);

    encoder_init(ENC_A_PIN);
    motor_init(IN1_PIN, IN2_PIN);

    Serial.println("ESP32-S3 Labyrinth Robot - Single Motor PI Control");
}

void loop()
{
    static uint32_t lastControl = millis();

    uint32_t now = millis();

    if (now - lastControl >= SAMPLING_TIME_MS)
    {
        float dt = (now - lastControl) / 1000.0f;
        lastControl = now;

        float targetSpeed = getTargetSpeed();
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