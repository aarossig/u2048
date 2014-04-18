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

    
    U2048_t u2048;
    U2048Init(&u2048, &ft800);
    U2048GameRender(&u2048);
    
    /*
    FT800DlNew(&ft800);
    FT800DlClearCSTBuffers(&ft800, true, true, true);
    
    FT800Color_t white = { .Red = 255, .Green = 255, .Blue = 255 };
    FT800DlRgb(&ft800, white);
    
    FT800Point_t p1 = { .X = 100, .Y = 100 };

    FT800CmdDrawText(&ft800, p1, FT800Font_AntiAliased6,
            FT800Option_CenterX,
            "2", 1);
    FT800CmdFlush(&ft800);
    FT800CmdSwapDisplayList(&ft800);
    FT800CmdFlush(&ft800);
    */

    
    while(1);
    
    return 0;
}

