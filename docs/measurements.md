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