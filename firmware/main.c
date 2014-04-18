/*
 * TR2 Robot Controller Application
 */

#include <stdint.h>
#include <stdbool.h>

#include "SystemResetClock.h"
#include "SystemInterrupts.h"
#include "SystemGpio.h"
#include "SystemSpi.h"

#include "FT800.h"

int main(void)
{
    SystemIrqEnable();
    
    // Enable GPIO Modules
    SystemClockEnabled.GpioA = true;
    
    // Enable SPI Lines
    SystemGpioA.Mode.P5 = SystemGpioMode_Alternate;
    SystemGpioA.Mode.P6 = SystemGpioMode_Alternate;
    SystemGpioA.Mode.P7 = SystemGpioMode_Alternate;
    SystemGpioA.Function.P5 = 5;
    SystemGpioA.Function.P6 = 5;
    SystemGpioA.Function.P7 = 5;
    
    // Configure chip select, high
    SystemGpioA.Mode.P9 = SystemGpioMode_Output;
    SystemGpioA.Set.P9 = true;
    
    // Setup the SPI Module
    SystemClockEnabled.SPI1 = true;
    SystemSpi1.Config.SlaveManageEnable = true;
    SystemSpi1.Config.InternalSelect = true;
    SystemSpi1.Config.DeviceMode = SystemSpiDeviceMode_Master;
    SystemSpi1.Config.Prescaler = SystemSpiPrescaler_2;
    SystemSpi1.Config.ClockPhase = SystemSpiClockPhase_First;
    SystemSpi1.Config.ClockIdle = SystemSpiClockIdle_Low;
    SystemSpi1.Config.Enabled = true;
    
    FT800_t ft800 = {
        .Spi = &SystemSpi1,
        .CsPort = &SystemGpioA,
        .CsPin = 9
    };
    
    FT800Init(&ft800);
    
    FT800SendCommand(&ft800, FT800Command_Active);
    FT800SendCommand(&ft800, FT800Command_Clock48);
    FT800SendCommand(&ft800, FT800Command_Reset);
    for(volatile int i = 0; i < 10000; i++);
    
    uint8_t chipId = FT800ReadChipId(&ft800);
    while(chipId != FT800_CHIPID)
    {
        // Error communicating with the FT800
    }
    
    uint8_t gpioState[] = { 0x80 };
    FT800Write(&ft800, FT800Register_GPIO, gpioState, 1);

    uint8_t setSwizzle[] = { 0x03 };
    FT800Write(&ft800, FT800Register_SWIZZLE, setSwizzle, 1);
    
    FT800DlClearRgb(&ft800, 0, 0, 0);
    FT800DlClearCSTBuffers(&ft800, true, true, true);
    FT800DlEnd(&ft800);
    FT800CmdSwapDisplayList(&ft800);
    FT800CmdFlush(&ft800);

    uint8_t enableClock[] = { 0x05 };
    FT800Write(&ft800, FT800Register_PCLK, enableClock, 1);

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

