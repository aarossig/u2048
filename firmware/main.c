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
    
    int cnt = 0;

    for(int i = 0; i < U2048_GAME_SIZE; i++)
    {
        for(int j = 0; j < U2048_GAME_SIZE; j++)
        {
            U2048Tile_t tile = U2048Tile_2;

            switch(cnt++)
            {
                case 1:
                    tile = U2048Tile_4;
                    break;
                case 2:
                    tile = U2048Tile_8;
                    break;
                case 3:
                    tile = U2048Tile_16;
                    break;
                case 4:
                    tile = U2048Tile_32;
                    break;
                case 5:
                    tile = U2048Tile_64;
                    break;
                case 6:
                    tile = U2048Tile_128;
                    break;
                case 7:
                    tile = U2048Tile_256;
                    break;
                case 8:
                    tile = U2048Tile_512;
                    break;
                case 9:
                    tile = U2048Tile_1024;
                    break;
                case 10:
                    tile = U2048Tile_2048;
                    break;
                default:
                    break;
            }
            
            U2048NewTile(&u2048, j, i, tile);
        }
    }

    U2048GameRender(&u2048);

    while(1);
    
    return 0;
}

