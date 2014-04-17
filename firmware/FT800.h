/*
 * Driver for the FT800 Embedded Video Controller
 */

#ifndef FT800_H
#define FT800_H

#include <stdint.h>

#include "SystemGpio.h"
#include "SystemSpi.h"

typedef struct FT800_t {
    volatile SystemSpiModule_t *Spi;
    volatile SystemGpioModule_t *CsPort;
    uint32_t CsPin;
} FT800_t;

/* FT800 Commands *************************************************************/

typedef enum FT800Command_t {
    FT800Command_Active = 0x00,
    FT800Command_Standby = 0x41,
    FT800Command_Sleep = 0x42,
    FT800Command_PowerDown = 0x60,
    FT800Command_ClockExternal = 0x44,
    FT800Command_Clock48 = 0x62,
    FT800Command_Clock36 = 0x61,
    FT800Command_Reset = 0x68
} FT800Command_t;

void FT800SendCommand(FT800_t *ft800, FT800Command_t command);

/* FT800 Memory Operations ****************************************************/

void FT800Read(FT800_t *ft800, uint32_t address,
    uint8_t *buf, uint32_t length);

void FT800Write(FT800_t *ft800, uint32_t address,
    uint8_t *buf, uint32_t length);

/* FT800 SPI ******************************************************************/

void FT800SpiWrite(FT800_t *ft800, uint8_t *buf, uint32_t length);
void FT800SpiWriteRead(FT800_t *ft800, uint8_t *buf, uint32_t length);

#endif
