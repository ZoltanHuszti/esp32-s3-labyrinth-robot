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
The encoder ISR increments encoder_count on each falling edge of encoder A.

## Main state variables
- encoder_count
- lastControl
- lastCount
- integral
- expectedSpeed
- measuredSpeed
- pwm