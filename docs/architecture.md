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

The encoder contains two Hall sensors. These sensors are mechanically shifted by approximately 90 degrees. Because of this mechanical offset, the two digital encoder signals are also phase-shifted by approximately 90 degrees.

Both encoder channels are configured with `CHANGE` interrupts, so the interrupt routines run on both rising and falling edges.

The direction of rotation can be determined from the relationship between the two encoder signals. Since the A and B signals are phase-shifted, one signal leads the other depending on the direction of rotation.

When an interrupt occurs, the current states of both encoder channels are read:

```cpp
bool a = digitalRead(encoderPinA);
bool b = digitalRead(encoderPinB);

the logic:

a == b  -> one rotation direction
a != b  -> opposite rotation direction

## Main state variables

- `encoder_count`  
  Signed encoder count value. Updated by the quadrature encoder ISRs.

- `lastControl`  
  Timestamp of the previous control loop execution. Used to calculate `dt`.

- `lastCount`  
  Previous encoder count value used by `motorSpeed(dt)` to calculate `deltaCount`.

- `integral`  
  Integral state of the PI controller.

- `targetSpeed`  
  Desired motor speed in `counts/s`.

- `measuredSpeed`  
  Measured signed motor speed in `counts/s`.

- `error`  
  Speed control error:

```cpp
error = targetSpeed - measuredSpeed;

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
    - PI_reset()

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