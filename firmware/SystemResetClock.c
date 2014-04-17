/*
 * STM32F4 Reset/Clock Driver Implementation
 */

#include "SystemResetClock.h"

#define CLOCK_TIMEOUT 1000000

bool SystemClockEnable(SystemClockType_t clock)
{
    volatile uint32_t timer = CLOCK_TIMEOUT;

    switch(clock)
    {
        case SystemClockType_Hsi:
            SystemClockControl.HsiEnabled = true;
            while(!SystemClockControl.HsiReady && timer--);
            break;
        case SystemClockType_Hse:
            SystemClockControl.HseEnabled = true;
            while(!SystemClockControl.HseReady && timer--);
            break;
        case SystemClockType_Pll:
            SystemClockControl.PllEnabled = true;
            while(!SystemClockControl.PllReady && timer--);
            break;
        default:
            break;
    }

    return timer > 0;
}

