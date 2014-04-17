/*
 * STM32F4 UART Driver
 */

#ifndef SYSTEMUART_H
#define SYSTEMUART_H

#include <stdbool.h>
#include <stdint.h>

#include "SystemRingBuffer.h"
#include "SystemInterrupts.h"
#include "SystemResetClock.h"

#define AddrUart1  0x40011000
#define AddrUart2  0x40004400
#define AddrUart3  0x40004800
#define AddrUart4  0x40004C00
#define AddrUart5  0x40005000
#define AddrUart6  0x40011400

#define SystemUart1 (*((volatile SystemUartModule_t *)AddrUart1))
#define SystemUart2 (*((volatile SystemUartModule_t *)AddrUart2))
#define SystemUart3 (*((volatile SystemUartModule_t *)AddrUart3))
#define SystemUart4 (*((volatile SystemUartModule_t *)AddrUart4))
#define SystemUart5 (*((volatile SystemUartModule_t *)AddrUart5))
#define SystemUart6 (*((volatile SystemUartModule_t *)AddrUart6))

/* UART Receiver Mode *********************************************************/

typedef enum SystemUartRxMode_t {
    SystemUartRxMode_Active,
    SystemUartRxMode_Mute
} SystemUartRxMode_t;

/* UART Parity Mode ***********************************************************/

typedef enum SystemUartParity_t {
    SystemUartParity_Odd,
    SystemUartParity_Even
} SystemUartParity_t;

/* UART Wakeup Mode ***********************************************************/

typedef enum SystemUartWakeup_t {
    SystemUartWakeup_Idle,
    SystemUartWakeup_Address
} SystemUartWakeup_t;

/* UART Frame Length **********************************************************/

typedef enum SystemUartFrameLength_t {
    SystemUartFrameLength_8,
    SystemUartFrameLength_9
} SystemUartFrameLength_t;

/* UART Oversample ************************************************************/

typedef enum SystemUartOversample_t {
    SystemUartOversample_16,
    SystemUartOversample_8
} SystemUartOversample_t;

/* UART Line Break Detection Length *******************************************/

typedef enum SystemUartBreakLength_t {
    SystemUartBreakLength_10,
    SystemUartBreakLength_11
} SystemUartBreakLength_t;

/* UART Clock Phase ***********************************************************/

typedef enum SystemUartClockPhase_t {
    SystemUartClockPhase_First,
    SystemUartClockPhase_Second
} SystemUartClockPhase_t;

/* Uart Clock Idle State ******************************************************/

typedef enum SystemUartClockIdle_t {
    SystemUartClockIdle_Low,
    SystemUartClockIdle_High
} SystemUartClockIdle_t;

/* UART Stop Bits *************************************************************/

typedef enum SystemUartStopBits_t {
    SystemUartStopBits_One,
    SystemUartStopBits_Half,
    SystemUartStopBits_Two,
    SystemUartStopBits_OneAndHalf
} SystemUartStopBits_t;

/* UART Sample Bit Method *****************************************************/

typedef enum SystemUartBitSample_t {
    SystemUartBitSample_3,
    SystemUartBitSample_1
} SystemUartBitSample_t;

/* UART Configuration *********************************************************/

typedef struct SystemUartConfiguration_t {
    union {
        struct {
            //USART_BRR
            uint32_t Fraction  : 4;
            uint32_t Mantissa  : 12;
            unsigned /* Pad */ : 16;
        } Baud;
        uint32_t BaudReg   : 16;
        unsigned /* Pad */ : 16;
    };
    
    union {
        struct {
            // USART_CR1
            bool                    SendBreak           : 1;
            SystemUartRxMode_t      RxMode              : 1;
            bool                    RxEnabled           : 1;
            bool                    TxEnabled           : 1;
            bool                    IdleIntEnabled      : 1;
            bool                    RxDoneIntEnabled    : 1;
            bool                    TxDoneIntEnabled    : 1;
            bool                    TxEmptyIntEnabled   : 1;
            bool                    ParityErrIntEnabled : 1;
            SystemUartParity_t      ParityMode          : 1;
            bool                    ParityEnabled       : 1;
            SystemUartWakeup_t      WakeupMode          : 1;
            SystemUartFrameLength_t FrameLength         : 1;
            bool                    Enabled             : 1;
            unsigned                /* Pad */           : 1;
            SystemUartOversample_t  OversampleMode      : 1;
            unsigned                /* Pad */           : 16;
            
            // USART_CR2
            uint32_t                Address             : 4;
            unsigned                /* Pad */           : 1;
            SystemUartBreakLength_t LineBreakLength     : 1;
            bool                    LineBreakIntEnabled : 1;
            unsigned                /* Pad */           : 1;
            bool                    LastClockSent       : 1;
            SystemUartClockPhase_t  ClockPhase          : 1;
            SystemUartClockIdle_t   ClockIdle           : 1;
            bool                    ClockEnabled        : 1;
            SystemUartStopBits_t    StopBits            : 2;
            bool                    LinEnabled          : 1;
            unsigned                /* Pad */           : 17;

            // USART_CR3
            bool                  ErrIntEnabled         : 1;
            bool                  IrdaEnabled           : 1;
            bool                  IrdaLowPower          : 1;
            bool                  HalfDuplex            : 1;
            bool                  NackEnabled           : 1;
            bool                  SmartCardEnabled      : 1;
            bool                  RxDmaEnabled          : 1;
            bool                  TxDmaEnabled          : 1;
            bool                  RtsEnabled            : 1;
            bool                  CtsEnabled            : 1;
            bool                  CtsIntEnabled         : 1;
            SystemUartBitSample_t BitSampleMethod       : 1;
            unsigned              /* Pad */             : 20;
            unsigned              /* IrDA/Smart Card */ : 32;
        };
        
        uint32_t Config1;
        uint32_t Config2;
        uint32_t Config3;
        unsigned /* IrDA/Smart Card */ : 32;
    };
} SystemUartConfiguration_t;

/* UART Status ****************************************************************/

typedef struct SystemUartStatus_t {
    bool ParityError   : 1;
    bool FramingError  : 1;
    bool NoiseDetected : 1;
    bool OverrunError  : 1;
    bool IdleDetected  : 1;
    bool RxComplete    : 1;
    bool TxComplete    : 1;
    bool TxEmpty       : 1;
    bool LinBreak      : 1;
    bool Cts           : 1;
    unsigned /* Pad */ : 22;
} SystemUartStatus_t;

/* UART Register Set **********************************************************/

typedef struct SystemUartModule_t {
    SystemUartStatus_t Status;
    uint32_t Data      : 9;
    unsigned /* Pad */ : 23;
    SystemUartConfiguration_t Config;
} SystemUartModule_t;

/* UART Buffers ***************************************************************/

typedef struct SystemUartBuffer_t {
    SystemRingBuffer_t Tx;
    bool               Transmitting;
    SystemRingBuffer_t Rx;
} SystemUartBuffer_t;

/* UART Driver API ************************************************************/

/* Initialization *************************************************************/

bool SystemUartInit(volatile SystemUartModule_t *uart,
    volatile SystemUartBuffer_t *buf,
    uint32_t baud);

/* Utility ********************************************************************/

uint32_t SystemUartBytesToRead(volatile SystemUartModule_t *uart);

/* Transmit *******************************************************************/

bool SystemUartTx(volatile SystemUartModule_t *uart, const uint32_t data);

bool SystemUartTxBuf(volatile SystemUartModule_t *uart,
    const uint32_t *data,
    const uint32_t length);

bool SystemUartTxStr(volatile SystemUartModule_t *uart,
    const char *str,
    const uint32_t length);

/* Receive ********************************************************************/

bool SystemUartRx(volatile SystemUartModule_t *uart, uint32_t *data);

bool SystemUartRxBuf(volatile SystemUartModule_t *uart,
     uint32_t *data,
     const uint32_t length);

bool SystemUartRxStr(volatile SystemUartModule_t *uart,
    char *data,
    const uint32_t length);

#endif

