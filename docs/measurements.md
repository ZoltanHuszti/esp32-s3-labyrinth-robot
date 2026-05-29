# Measurements

This file documents motor control measurements, test conditions, controller parameters, and observations.

## Measurement 001 - Single motor PI step response

### Date
2026-05-28

### Goal
Test the closed-loop PI speed controller response to changing target speeds.

### Hardware setup
- MCU: ESP32-S3
- Motor driver: DRV8833
- Motor: N20E encoder DC motor
- Encoder input: Channel A, falling edge only
- Power supply: USB for ESP32, external supply/battery for motor driver

### Firmware setup
- Sampling time: 25 ms
- Speed unit: counts/s
- Encoder counting: single channel, falling edge
- Controller: PI

### Controller parameters
- Kp = 0.4
- Ki = 0.3
- Integral limit = ±1000
- PWM limit = ±255

### Target speed sequence
- 0–5 s: 2000 counts/s
- 5–10 s: 500 counts/s
- 10–15 s: 900 counts/s
- 15–20 s: 1500 counts/s

### Log file
`log/COM3_2026_05_28.15.49.39.993.txt`

### Plot file
`docs/diagrams/kp04ki03.png`

### Observations
- The controller reaches the target speed after a short transient.
- Some overshoot appears after large target changes.
- Measured speed is quantized due to encoder pulse counting in a 25 ms window.
- Speed values jump in approximately 40 counts/s steps.

### Conclusion
The PI controller works and can track changing target speeds. The remaining ripple is likely caused mainly by measurement quantization and partly by mechanical effects (I guess).

### Next steps
- Try lower Ki to reduce overshoot.
- Add a low-pass filter or moving average to measured speed.
- Test fixed PWM response to separate mechanical ripple from control ripple.
- Later test quadrature encoder counting for higher resolution.