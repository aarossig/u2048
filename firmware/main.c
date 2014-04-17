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
    SystemSpi1.Config.Prescaler = SystemSpiPrescaler_64;
    SystemSpi1.Config.ClockPhase = SystemSpiClockPhase_First;
    SystemSpi1.Config.ClockIdle = SystemSpiClockIdle_Low;
    SystemSpi1.Config.Enabled = true;
    
    for(volatile int i = 0; i < 10000; i++);

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
    
    uint8_t chipId[] = { 0 };
    while(chipId[0] != FT800_CHIPID)
        FT800Read(&ft800, FT800Register_ID, chipId, 1);
    
    uint8_t gpioState[1] = { 0x80 };
    FT800Write(&ft800, FT800Register_GPIO, gpioState, 1);

    uint8_t setClockPol[] = { 0x00 };
    FT800Write(&ft800, FT800Register_PCLK_POL, setClockPol, 1);

    uint8_t setSwizzle[] = { 0x03 };
    FT800Write(&ft800, FT800Register_SWIZZLE, setSwizzle, 1);
    
    uint8_t setColor[] = { 0xFF, 0x00, 0x00, 0x02 };
    FT800Write(&ft800, 0x100000, setColor, 4);
    
    uint8_t clear[] = { 0x07, 0x00, 0x00, 0x26 };
    FT800Write(&ft800, 0x100000 + 4, clear, 4);

    uint8_t displayEnd[] = { 0x00, 0x00, 0x00, 0x00 };
    FT800Write(&ft800, 0x100000 + 8, displayEnd, 4);
   
    uint8_t swapList[] = { 0x01 };
    FT800Write(&ft800, 0x102450, swapList, 1);
    
    uint8_t enableClock[] = { 0x05 };
    FT800Write(&ft800, FT800Register_PCLK, enableClock, 1);
    
    FT800NewDisplayList(&ft800);
    FT800Logo(&ft800);
    FT800FlushCommands(&ft800);

    while(1);
    
    return 0;
}

