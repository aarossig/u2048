/*
 * Embedded u2048
 */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "SystemResetClock.h"
#include "SystemInterrupts.h"
#include "SystemGpio.h"

#include "Gameduino2.h"
#include "FT800.h"

#include "u2048.h"

int main(void)
{
    SystemIrqEnable();

    // Configure the PLL for 42MHz
    SystemPll.PllPrescaler = 40;
    SystemPll.Multiplier = 210;
    SystemPll.SystemClockPrescaler = SystemPllMainPrescaler_2;
    SystemPll.ClockSource = SystemPllClockSource_Hsi;
    
    bool pllEnabled = SystemClockEnable(SystemClockType_Pll);
    while(!pllEnabled)
    {
        // Error switching to PLL clock source
    }

    
    FT800_t ft800;
    if(!Gameduino2Init(&ft800))
    {
        // Error communicating with the Gameduino
    }
    
    U2048_t u2048;
    U2048Init(&u2048, &ft800);
    U2048GameRender(&u2048);
    
    for(int i = 0; i < U2048_GAME_SIZE; i++)
    {
        U2048NewTile(&u2048, i, 3, U2048Tile_2);
    }

    for(int i = 0; i < 1000000; i++);

    U2048Action(&u2048, U2048Action_SwipeRight);
    U2048GameRender(&u2048);
    for(int i = 0; i < 1000000; i++);

    U2048Action(&u2048, U2048Action_SwipeRight);
    U2048GameRender(&u2048);
    for(int i = 0; i < 1000000; i++);
    
    U2048GameRender(&u2048);
    
    while(1);
    
    return 0;
}

