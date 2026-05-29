# Control Architecture

## Main control loop
Runs every 25 ms.

1. Calculate dt
2. Read encoder count difference
3. Estimate motor speed
4. Calculate speed error
5. Update PI controller
6. Apply PWM to DRV8833
7. Print log line

## Interrupts

The encoder uses quadrature decoding with two channels: `ENC_A_PIN` and `ENC_B_PIN`.

Both encoder channels are configured with `CHANGE` interrupts, so the interrupt routines run on both rising and falling edges.

The encoder count is signed:

```cpp
volatile int32_t encoder_count;
```

The ISR updates `encoder_count` depending on the detected rotation direction:

* positive rotation: `encoder_count++`
* negative rotation: `encoder_count--`

Because of this, `motorSpeed(dt)` returns signed speed in `counts/s`.

```text
positive speed -> forward rotation
negative speed -> reverse rotation
```

## Main state variables
- encoder_count
- lastControl
- lastCount
- integral
- expectedSpeed
- measuredSpeed
- pwm

## Modules
* Encoder module:
  * `encoder_init(pinA, pinB)`
  * `encoderA_ISR()`
  * `encoderB_ISR()`
  * `motorSpeed(dt)`
  * `getEncoderCount()`
  * `resetEncoderCount()`
  * signed `encoder_count`


- Motor driver modul:
    - motor_init()
    - motor(speed)

- Controller modul:
    - calcError()
    - PI_update(error, dt)

- Test / experiment modul:
    - targetSpeed lépcsőgenerálás
    - Serial logging

src/
  main.cpp
  motor.cpp
  encoder.cpp
  controller.cpp

include/
  motor.h
  encoder.h
  controller.h