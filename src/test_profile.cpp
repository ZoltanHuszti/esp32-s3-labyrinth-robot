#include "test_profile.h"

float getTargetSpeed(float targetSpeed)
{
    if(targetSpeed != 0)
    {
        return sin(millis() / 1000.0f) * targetSpeed; // Simulate a varying target speed between -5000 and 5000 counts/s
    } 
    else
    {
        uint32_t testTime = millis() % 6000;

        if (testTime < 5000)
        {
            return -5000.0f;
        }
        else
        {
            return 0.0f;
        }
    }  
}