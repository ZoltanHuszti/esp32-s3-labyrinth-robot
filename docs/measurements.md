## PI controller tuning with quadrature encoder

The motor speed controller was tested after implementing quadrature encoder decoding.

### Test conditions

The PI controller tuning tests were performed without external load.

### Encoder mode

* Encoder type: quadrature encoder
* Encoder channels: A and B
* Interrupt mode: `CHANGE` on both encoder channels
* Speed unit: signed `counts/s`
* Measured wheel resolution: approximately `2569.7 counts / wheel revolution`

### Controller parameters

After testing different gain values, the following PI parameters gave the best behavior:

```cpp
Kp = 0.03f;
Ki = 0.3f;
```

These values were tested with both:

* step response target profiles
* sinusoidal / smooth bidirectional target speed profiles

### Step response test

The controller was tested with target speed step changes. The measured speed followed the target speed with acceptable transient behavior and without sustained oscillation.

The step response plots are stored in:

```text
docs/diagrams/
```

### Sinusoidal target tracking test

The controller was also tested with a smooth sinusoidal target speed profile. This test verified that the controller can follow both positive and negative speed commands.

The measured speed followed the sinusoidal target reasonably well in both directions. Around zero speed, a small dead-zone effect can be observed, most likely caused by static friction and the minimum effective PWM required to start the motor.

The sinusoidal tracking plots are stored in:

```text
docs/diagrams/
```

### Conclusion

The values `Kp = 0.03` and `Ki = 0.3` currently provide the best tested behavior for the single-motor quadrature encoder speed control setup.

The controller works for:

* positive speed control
* negative speed control
* stop transitions
* step target changes
* smooth bidirectional target tracking

Further improvements may include:

* low-speed dead-zone compensation
* feedforward PWM term
* target speed ramping
* testing under real robot load