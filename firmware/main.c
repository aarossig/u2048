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

    FT800Color_t strColor = { .Red = 255, .Green = 255, .Blue = 255 };
    FT800Point_t strPoint = { .X = 10, .Y = 10 };
    
    char *touch = "Touch";
    char *noTouch = "No Touch";

    char *touchStatus = NULL;
    char *touchCoords[20];
    
    while(1)
    {
        uint8_t touchBuf[4];
        FT800Read(&ft800, FT800Register_TOUCH_DIRECT_XY, touchBuf, 4);
        
        if(touchBuf[3] & 0x80)
        {
            touchStatus = noTouch;
        }
        else
        {
            touchStatus = touch;
        }
        
        uint16_t touchY = touchBuf[0] | ((touchBuf[1] & 0x03) << 8);
        uint16_t touchX = touchBuf[2] | ((touchBuf[3] & 0x03) << 8);
        
        sprintf(touchCoords, "X: %03d Y: %03d", touchX, touchY);

        FT800CmdNewDisplayList(&ft800);
        
        FT800DlClearCSTBuffers(&ft800, true, true, true);
        FT800DlRgb(&ft800, strColor);
        
        strPoint.Y = 10;

        FT800CmdDrawText(&ft800, strPoint, FT800Font_AntiAliased3,
            FT800Option_None, touchStatus, strlen(touchStatus));

        strPoint.Y += 60;

        FT800CmdDrawText(&ft800, strPoint, FT800Font_AntiAliased3,
            FT800Option_None, touchCoords, strlen(touchCoords));
        
        FT800DlEnd(&ft800);
        FT800CmdSwapDisplayList(&ft800);
        FT800CmdFlush(&ft800);
    }
    
    /*
    U2048_t u2048;
    U2048Init(&u2048, &ft800);
    U2048GameRender(&u2048);

    for(int i = 0; i < 1000000; i++);
    
    U2048NewTile(&u2048, 1, 0, U2048Tile_16);
    U2048NewTile(&u2048, 3, 1, U2048Tile_32);
    U2048NewTile(&u2048, 3, 2, U2048Tile_32);
    U2048NewTile(&u2048, 2, 1, U2048Tile_8);
    U2048NewTile(&u2048, 2, 2, U2048Tile_8);
    U2048NewTile(&u2048, 0, 3, U2048Tile_4);
    U2048NewTile(&u2048, 1, 3, U2048Tile_4);
    U2048NewTile(&u2048, 2, 3, U2048Tile_4);
    U2048NewTile(&u2048, 3, 3, U2048Tile_4);

    for(int i = 0; i < 1000000; i++);
    U2048Action(&u2048, U2048Action_SwipeRight);
    for(int i = 0; i < 1000000; i++);
    U2048Action(&u2048, U2048Action_SwipeLeft);
    for(int i = 0; i < 1000000; i++);
    U2048Action(&u2048, U2048Action_SwipeUp);
    for(int i = 0; i < 1000000; i++);
    U2048Action(&u2048, U2048Action_SwipeDown);
    */
    
    while(1);
    
    return 0;
}

