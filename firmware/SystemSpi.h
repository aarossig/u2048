/*
 * STM32F4 SPI Driver
 */

#ifndef SYSTEMSPI_H
#define SYSTEMSPI_H

#include <stdint.h>
#include <stdbool.h>

#include "SystemResetClock.h"

#define AddrSpi1 0x40013000
#define AddrSpi2 0x40003800
#define AddrSpi3 0x40003C00
#define AddrSpi4 0x40013400
#define AddrSpi5 0x40015000
#define AddrSpi6 0x40015400

#define SystemSpi1 (*((volatile SystemSpiModule_t *)AddrSpi1))
#define SystemSpi2 (*((volatile SystemSpiModule_t *)AddrSpi2))
#define SystemSpi3 (*((volatile SystemSpiModule_t *)AddrSpi3))
#define SystemSpi4 (*((volatile SystemSpiModule_t *)AddrSpi4))
#define SystemSpi5 (*((volatile SystemSpiModule_t *)AddrSpi5))
#define SystemSpi6 (*((volatile SystemSpiModule_t *)AddrSpi6))


/* Clock Phase ****************************************************************/

typedef enum SystemSpiClockPhase_t {
    SystemSpiClockPhase_First,
    SystemSpiClockPhase_Second
} SystemSpiClockPhase_t;

/* Clock Idle State ***********************************************************/

typedef enum SystemSpiClockIdle_t {
    SystemSpiClockIdle_Low,
    SystemSpiClockIdle_High
} SystemSpiClockIdle_t;

/* Device Mode ****************************************************************/

typedef enum SystemSpiDeviceMode_t {
    SystemSpiDeviceMode_Slave,
    SystemSpiDeviceMode_Master
} SystemSpiDeviceMode_t;

/* Clock Prescaler ************************************************************/

typedef enum SystemSpiPrescaler_t {
    SystemSpiPrescaler_2   = 0x00,
    SystemSpiPrescaler_4   = 0x01,
    SystemSpiPrescaler_8   = 0x02,
    SystemSpiPrescaler_16  = 0x03,
    SystemSpiPrescaler_32  = 0x04,
    SystemSpiPrescaler_64  = 0x05,
    SystemSpiPrescaler_128 = 0x06,
    SystemSpiPrescaler_256 = 0x07
} SystemSpiPrescaler_t;

/* Frame Endianness ***********************************************************/

typedef enum SystemSpiEndianness_t {
    SystemSpiEndianness_MsbFirst,
    SystemSpiEndianness_LsbFirst
} SystemSpiEndianness_t;

/* Frame Size *****************************************************************/

typedef enum SystemSpiFrameSize_t {
    SystemSpiFrameSize_8,
    SystemSpiFrameSize_16
} SystemSpiFrameSize_t;

/* Bidirectional Mode *********************************************************/

typedef enum SystemSpiBidirectionMode_t {
    SystemSpiBidirectionMode_2Wire,
    SystemSpiBidirectionMode_1Wire
} SystemSpiBidirectionMode_t;

/* Frame Format ***************************************************************/

typedef enum SystemSpiFrameFormat_t {
    SystemSpiFrameFormat_Motorola,
    SystemSpiFrameFormat_TI
} SystemSpiFrameFormat_t;

/* SPI Status *****************************************************************/

typedef union SystemSpiStatus_t {
    struct {
        bool     RxFull         : 1;
        bool     TxEmpty        : 1;
        unsigned /* I2S Only */ : 2;
        bool     CrcError       : 1;
        bool     ModeFault      : 1;
        bool     Overrun        : 1;
        bool     Busy           : 1;
        bool     FrameError     : 1;
        unsigned /* Pad */      : 23;
    };
    uint32_t Status    : 9;
    unsigned /* Pad */ : 23;
} SystemSpiStatus_t;

/* SPI Configuration **********************************************************/

typedef union SystemSpiConfiguration_t {
    struct {
        // SPI_CR1
        SystemSpiClockPhase_t      ClockPhase        : 1;
        SystemSpiClockIdle_t       ClockIdle         : 1;
        SystemSpiDeviceMode_t      DeviceMode        : 1;
        SystemSpiPrescaler_t       Prescaler         : 3;
        bool                       Enabled           : 1;
        SystemSpiEndianness_t      FrameEndianness   : 1;
        bool                       InternalSelect    : 1;
        bool                       SlaveManageEnable : 1;
        bool                       RxOnly            : 1;
        SystemSpiFrameSize_t       FrameSize         : 1;
        bool                       CrcNext           : 1;
        bool                       CrcEnabled        : 1;
        bool                       OutputEnabled     : 1;
        SystemSpiBidirectionMode_t BidirectionMode   : 1;
        unsigned                   /* Pad */         : 16;

        // SPI_CR2
        bool                       RxDmaEnabled      : 1;
        bool                       TxDmaEnabled      : 1;
        bool                       SSEnabled         : 1;
        unsigned                   /* Pad */         : 1;
        SystemSpiFrameFormat_t     FrameFormat       : 1;
        bool                       ErrIntEnabled     : 1;
        bool                       RxDoneIntEnabled  : 1;
        bool                       TxDoneIntEnabled  : 1;
        unsigned                   /* Pad */         : 24;
    };
    uint32_t Config1;
    uint32_t Config2;
} SystemSpiConfiguration_t;

/* SPI Register Set ***********************************************************/

typedef struct SystemSpiModule_t {
    SystemSpiConfiguration_t Config;
    SystemSpiStatus_t Status;
    uint32_t Data        : 16;
    unsigned /* Pad */   : 16;
    uint32_t Polynomial  : 16;
    unsigned /* Pad */   : 16;
    const uint32_t RxCrc : 16;
    unsigned /* Pad */   : 16;
    const uint32_t TxCrc : 16;
    unsigned /* Pad */   : 16;
} SystemSpiModule_t;

/* Write/Read *****************************************************************/

uint16_t SystemSpiWriteRead(volatile SystemSpiModule_t *spi, uint16_t data);

#endif
