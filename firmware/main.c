/*
 * Embedded u2048
 */

#include <stdint.h>
#include <stdbool.h>

#include "SystemResetClock.h"
#include "SystemInterrupts.h"
#include "SystemGpio.h"

#include "Gameduino2.h"
#include "FT800.h"

#include "u2048.h"

int main(void)
{
    SystemIrqEnable();
    
    FT800_t ft800;
    if(!Gameduino2Init(&ft800))
    {
        // Error communicating with the Gameduino
    }

    for(int i = 0; i < 100000; i++);
    
    U2048_t u2048;
    U2048Init(&u2048, &ft800);
    U2048GameRender(&u2048);
    
    while(1);
    
    return 0;
}

