/*
 * Driver for the FT800 Embedded Video Controller
 */

#include "FT800.h"

/* FT800 Display List *********************************************************/

void FT800NewDisplayList(FT800_t *ft800)
{
    uint8_t newDisplayList[] = {
        0x00,
        0xFF,
        0xFF,
        0xFF
    };

    ft800->CsPort->Output.Port &= ~(1 << ft800->CsPin);
    FT800SpiWrite(ft800, newDisplayList, 4);
    ft800->CsPort->Output.Port |= (1 << ft800->CsPin);
}

void FT800SwapDisplayList(FT800_t *ft800)
{
    uint8_t swapDisplayList[] = {
        0x01,
        0xFF,
        0xFF,
        0xFF
    };

    ft800->CsPort->Output.Port &= ~(1 << ft800->CsPin);
    FT800SpiWrite(ft800, swapDisplayList, 4);
    ft800->CsPort->Output.Port |= (1 << ft800->CsPin);
}

/* FT800 Commands *************************************************************/

void FT800SendCommand(FT800_t *ft800, FT800Command_t command)
{
    uint8_t commandBuf[] = {
        command,
        0,
        0
    };

    ft800->CsPort->Output.Port &= ~(1 << ft800->CsPin);
    FT800SpiWrite(ft800, commandBuf, 3);
    ft800->CsPort->Output.Port |= (1 << ft800->CsPin);
}

/* FT800 Memory Operations ****************************************************/

void FT800Read(FT800_t *ft800, uint32_t address,
    uint8_t *buf, uint32_t length)
{
    uint8_t addressBuf[] = {
        (address >> 16) & 0x3F,
        (address >> 8) & 0xFF,
        (address) & 0xFF,
        0
    };
    
    ft800->CsPort->Output.Port &= ~(1 << ft800->CsPin);
    FT800SpiWrite(ft800, addressBuf, 4);
    FT800SpiWriteRead(ft800, buf, length);
    ft800->CsPort->Output.Port |= (1 << ft800->CsPin);
}

void FT800Write(FT800_t *ft800, uint32_t address,
    uint8_t *buf, uint32_t length)
{
    uint8_t addressBuf[] = {
        ((address >> 16) & 0x3F) | 0x80,
        (address >> 8) & 0xFF,
        (address) & 0xFF
    };

    ft800->CsPort->Output.Port &= ~(1 << ft800->CsPin);
    FT800SpiWrite(ft800, addressBuf, 3);
    FT800SpiWriteRead(ft800, buf, length);
    ft800->CsPort->Output.Port |= (1 << ft800->CsPin);
}

/* FT800 SPI ******************************************************************/

void FT800SpiWrite(FT800_t *ft800, uint8_t *buf, uint32_t length)
{
    for(uint32_t i = 0; i < length; i++)
    {
        SystemSpiWriteRead(ft800->Spi, buf[i]);
    }
}

void FT800SpiWriteRead(FT800_t *ft800, uint8_t *buf, uint32_t length)
{
    for(uint32_t i = 0; i < length; i++)
    {
        buf[i] = SystemSpiWriteRead(ft800->Spi, buf[i]);
    }
}

