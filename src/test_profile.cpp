#include "test_profile.h"

float getTargetSpeed()
{
    uint32_t testTime = millis() % 20000;

    if (testTime < 5000)
    {
        return 2000.0f;
    }
    else if (testTime < 10000)
    {
        return 500.0f;
    }
    else if (testTime < 15000)
    {
        return 900.0f;
    }
    else
    {
        return 1500.0f;
    }
}