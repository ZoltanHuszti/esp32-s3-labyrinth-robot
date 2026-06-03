## PI controller measurements

The closed-loop motor speed control system was tested with two target signal types:

- step response target signal
- sinusoidal target signal

The measurements were performed with quadrature encoder feedback and signed speed measurement in `counts/s`.

### Step response test

The step response test was used to tune the PI controller.

Based on the tests the best controller parameters are:

```cpp
Kp = 0.03f;
Ki = 0.3f;

The test with the sinusoidal target showed that the systems characteristic is non-linear 

## Dual motor open-loop and encoder test

Both motor driver outputs and both quadrature encoders were tested successfully.

The motor sides and encoder directions were correct on the first test.

Measured open-loop speed at the tested PWM level:

- Left motor: approximately 8320 counts/s
- Right motor: approximately 8160–8200 counts/s

The difference between the two motors is small and acceptable for the current prototype.

----------------------------------------------------------------------------------------
------------------------## Two-motor power supply voltage test--------------------------
----------------------------------------------------------------------------------------

A two-motor motor driver test was performed to observe how the battery voltage and buck converter output voltage behave during high motor load transients.

### Test setup

The test was performed with two DC motors connected through the motor driver.

The motor command was a repeated full-scale PWM step:

```cpp
uint32_t testTime = millis() % 6000;

if (testTime < 4000)
{
    leftMotorSet(255);
    rightMotorSet(255);
}
else
{
    leftMotorSet(0);
    rightMotorSet(0);
}
```

This means:

```text
0–4 s:   both motors at maximum PWM
4–6 s:   both motors stopped
repeat
```

This is a relatively harsh test because both motors are started from 0 PWM to maximum PWM at the same time.

### Measured signals

The following voltages were measured:

* battery voltage
* buck converter output voltage

The plots are stored in:

```text
docs/diagrams/
```

### Results

The battery voltage showed visible transient voltage drops when both motors were switched on.

Approximate observed battery behavior:

```text
nominal battery voltage: around 7.10 V
lowest observed dips: around 6.80–6.85 V
voltage drop: approximately 250–300 mV
```

This shows that the motor startup current is clearly visible on the battery side.

The buck converter output voltage remained much more stable.

Approximate observed buck output behavior:

```text
buck output voltage: around 5.15–5.16 V
observed variation: only a few millivolts
```

This suggests that the buck converter is able to maintain a stable output voltage during the tested two-motor PWM transients.

### Interpretation

The motor current transients affect the battery voltage, but the buck converter output remains stable in this test.

This is a good result for the ESP32-S3 supply, because the microcontroller is powered from the regulated buck output rather than directly from the motor supply line.

However, this test was performed without real robot floor load. Under real driving conditions, the motors may draw more current due to:

* robot weight
* wheel-ground friction
* acceleration
* turning
* sudden direction changes
* wheel blocking or partial stalling

Therefore, the same voltage measurement should later be repeated with the robot driving on the floor.

### Recommended hardware improvements

To improve robustness, additional decoupling and bulk capacitance should be added.

Recommended capacitors near the motor driver supply:

```text
470 µF – 1000 µF electrolytic or low-ESR capacitor
+
100 nF ceramic capacitor
```

Recommended capacitors near the buck converter / ESP32-S3 supply:

```text
100 µF electrolytic or low-ESR capacitor
+
100 nF ceramic capacitor
```

The motor driver ground, ESP32-S3 ground, buck converter ground, and battery ground must be common.

The motor current path should use sufficiently thick wires, and the high-current motor wiring should be kept as separate as reasonably possible from sensitive logic/sensor wiring.

### Conclusion

The two-motor full-PWM test shows that the battery voltage experiences short transient drops, but the buck converter output remains stable.

The current power supply setup appears acceptable for unloaded two-motor testing. Further testing is required under real robot load on the floor.
