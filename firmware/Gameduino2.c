/*
 * Gameduino 2 on the STM32F4 Nucleo Board
 */

#include "Gameduino2.h"

/* Initialization *************************************************************/

bool Gameduino2Init(FT800_t *ft800)
{
    // Enable GPIO Lines
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
    
    ft800->Spi = &SystemSpi1;
    ft800->CsPort = &SystemGpioA;
    ft800->CsPin = 9;
    
    FT800Init(ft800);

    FT800SendCommand(ft800, FT800Command_Active);
    FT800SendCommand(ft800, FT800Command_Clock48);
    FT800SendCommand(ft800, FT800Command_Reset);
    for(volatile int i = 0; i < 10000; i++);

    uint8_t chipId = FT800ReadChipId(ft800);

    if(chipId != FT800_CHIPID)
    {
        return false;
    }
    
    uint8_t gpioDir[] = { 0x83 };
    FT800Write(ft800, FT800Register_GPIO_DIR, gpioDir, 1);

    uint8_t gpioState[] = { 0x80 };
    FT800Write(ft800, FT800Register_GPIO, gpioState, 1);
    
    uint8_t setSwizzle[] = { 0x03 };
    FT800Write(ft800, FT800Register_SWIZZLE, setSwizzle, 1);

    uint8_t setPolarity[] = { 0x01 };
    FT800Write(ft800, FT800Register_PCLK_POL, setPolarity, 1);

    Gameduino2Blank(ft800);

    uint8_t enableClock[] = { 0x05 };
    FT800Write(ft800, FT800Register_PCLK, enableClock, 1);
    
    return true;
}

void Gameduino2Blank(FT800_t *ft800)
{
    FT800Color_t black = { .Red = 0, .Green = 0, .Blue = 0 };

    FT800DlClearRgb(ft800, black);
    FT800DlClearCSTBuffers(ft800, true, true, true);
    FT800DlEnd(ft800);
    FT800CmdSwapDisplayList(ft800);
    FT800CmdFlush(ft800);
}

