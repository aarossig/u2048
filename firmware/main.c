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

int main(void)
{
    SystemIrqEnable();
    
    FT800_t ft800;
    
    if(!Gameduino2Init(&ft800))
    {
        // Error communicating with the Gameduino
    }
    
    for(int i = 0; i < 480; i += 10)
    {
        for(int j = 0; j < 262; j += 10)
        {
            FT800DlNew(&ft800);
            FT800DlClearCSTBuffers(&ft800, true, true, true);
            FT800DlRgb(&ft800, 0, 255, 255);
            
            FT800Point_t p1 = { .X = i, .Y = j };
            FT800Point_t p2 = { .X = i + 10, .Y = j + 10 };
            FT800DrawRectangle(&ft800, p1, p2);
            
            FT800DlEnd(&ft800);
            FT800CmdSwapDisplayList(&ft800);
            FT800CmdFlush(&ft800);
            
            for(int i = 0; i < 10000; i++);
        }
    }
    
    while(1);
    
    return 0;
}

