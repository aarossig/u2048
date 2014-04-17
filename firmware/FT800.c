/*
 * Driver for the FT800 Embedded Video Controller
 */

#include "FT800.h"

/* FT800 Instance *************************************************************/

void FT800Init(FT800_t *ft800)
{
    ft800->DisplayListAddress = FT800_DL_START;
    ft800->CommandAddress = FT800_CMD_START;
}

/* FT800 Coprocessor Commands *************************************************/

void FT800NewDisplayList(FT800_t *ft800)
{
    uint8_t newDisplayList[] = { 0x00, 0xFF, 0xFF, 0xFF };
    FT800CoprocessorCommand(ft800, newDisplayList, 4);
    
    ft800->DisplayListAddress = FT800_DL_START;
}

void FT800SwapDisplayList(FT800_t *ft800)
{
    uint8_t swapDisplayList[] = { 0x01, 0xFF, 0xFF, 0xFF };
    FT800CoprocessorCommand(ft800, swapDisplayList, 4);
}

void FT800Logo(FT800_t *ft800)
{
    uint8_t logo[] = { 0x31, 0xFF, 0xFF, 0xFF };
    FT800CoprocessorCommand(ft800, logo, 4);
}

void FT800FlushCommands(FT800_t *ft800)
{
    uint8_t address = {
        (ft800->CommandAddress) & 0xFF,
        (ft800->CommandAddress >> 8) & 0xFF,
        (ft800->CommandAddress >> 16) & 0xFF
    };
    
    FT800Write(ft800, FT800Register_CMD_WRITE, address, 3);
}

/* FT800 Commands *************************************************************/

void FT800SendCommand(FT800_t *ft800, FT800Command_t command)
{
    uint8_t commandBuf[] = { command, 0x00, 0x00 };
    FT800SpiWriteCs(ft800, commandBuf, 3);
}

/* FT800 Memory Operations ****************************************************/

void FT800CoprocessorCommand(FT800_t *ft800, uint8_t *buf, uint32_t length)
{
    FT800Write(ft800, ft800->CommandAddress, buf, length);
    ft800->CommandAddress += length;
}

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

void FT800SpiWriteCs(FT800_t *ft800, uint8_t *buf, uint32_t length)
{
    ft800->CsPort->Output.Port &= ~(1 << ft800->CsPin);
    FT800SpiWrite(ft800, buf, length);
    ft800->CsPort->Output.Port |= (1 << ft800->CsPin);
}

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

