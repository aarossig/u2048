/*
 * STM32F4 Reset/Clock Driver
 */

#ifndef SYSTEMRESETCLOCK_H
#define SYSTEMRESETCLOCK_H

#include <stdint.h>
#include <stdbool.h>

#define AddrClockCtrl 0x40023800
#define AddrPll       0x40023804
#define AddrClockConf 0x40023808
#define AddrClockEn   0x40023830

#define SystemClockControl (*((volatile SystemClockControl_t *)(AddrClockCtrl)))
#define SystemPll          (*((volatile SystemPll_t *)(AddrPll)))
#define SystemClockConfig  (*((volatile SystemClockConfig_t *)(AddrClockConf)))
#define SystemClockEnabled (*((volatile SystemClockEnabled_t *)(AddrClockEn)))

/* Clock Control **************************************************************/

typedef struct SystemClockControl_t {
    bool           HsiEnabled           : 1;
    bool           HsiReady             : 1;
    unsigned       /* Pad */            : 1;
    uint32_t       HsiTrim              : 5;
    const uint32_t HsiCalibration       : 8;
    bool           HseEnabled           : 1;
    bool           HseReady             : 1;
    bool           HseBypass            : 1;
    bool           ClockSecurityEnabled : 1;
    unsigned       /* Pad */            : 4;
    bool           PllEnabled           : 1;
    bool           PllReady             : 1;
    bool           PllI2sEnabled        : 1;
    bool           PllI2sReady          : 1;
    unsigned       /* Pad */            : 4;
} SystemClockControl_t;

/* PLL Configuration **********************************************************/

typedef enum SystemPllClockSource_t {
    SystemPllClockSource_Hsi,
    SystemPllClockSource_Hse
} SystemPllClockSource_t;

typedef enum SystemPllMainPrescaler_t {
    SystemPllMainPrescaler_2,
    SystemPllMainPrescaler_4,
    SystemPllMainPrescaler_6,
    SystemPllMainPrescaler_8
} SystemPllMainPrescaler_t;

typedef struct SystemPll_t {
    uint32_t                 PllPrescaler         : 6;
    uint32_t                 Multiplier           : 9;
    unsigned                 /* Pad */            : 1;
    SystemPllMainPrescaler_t SystemClockPrescaler : 2;
    unsigned                 /* Pad */            : 4;
    SystemPllClockSource_t   ClockSource          : 1;
    unsigned                 /* Pad */            : 1;
    uint32_t                 UsbSdioPrescaler     : 4;
    unsigned                 /* Pad */            : 4;
} SystemPll_t;

/* Clock Configuration ********************************************************/

typedef enum SystemClockSource_t {
    SystemClockSource_Hsi,
    SystemClockSource_Hse,
    SystemClockSource_Pll
} SystemClockSource_t;

typedef enum SystemClockOutput_t {
    SystemClockOutput_Hsi,
    SystemClockOutput_Lse,
    SystemClockOutput_Hse,
    SystemClockOutput_Pll
} SystemClockOutput_t;

typedef enum I2sClockSource_t {
    I2sClockSource_Pll,
    I2sClockSource_External
} I2sClockSource_t;

typedef struct SystemClockConfig_t {
    SystemClockSource_t ClockSource           : 2;
    SystemClockSource_t ActiveClockSource     : 2;
    uint32_t            AhbPrescaler          : 4;
    unsigned            /* Pad */             : 2;
    uint32_t            ApbLowPrescaler       : 3;
    uint32_t            ApbHighPrescaler      : 3;
    uint32_t            RtcPrescaler          : 5;
    SystemClockOutput_t ClockOutput1          : 2;
    I2sClockSource_t    I2sClockSource        : 1;
    uint32_t            ClockOutput1Prescaler : 3;
    uint32_t            ClockOutput2Prescaler : 3;
    SystemClockOutput_t ClockOutput2          : 2;
} SystemClockConfig_t;

/* Peripheral Clocks Enabled **************************************************/

typedef struct SystemClockEnabled_t {
    // RCC_AHB1ENR
    bool     GpioA         :  1;
    bool     GpioB         :  1;
    bool     GpioC         :  1;
    bool     GpioD         :  1;
    bool     GpioE         :  1;
    bool     GpioF         :  1;
    bool     GpioG         :  1;
    bool     GpioH         :  1;
    bool     GpioI         :  1;
    unsigned /* Pad */     :  3;
    bool     Crc           :  1;
    unsigned /* Pad */     :  5;
    bool     BackupSram    :  1;
    unsigned /* Pad */     :  1;
    bool     CcmRam        :  1;
    bool     Dma1          :  1;
    bool     Dma2          :  1;
    unsigned /* Pad */     :  2;
    bool     EthMac        :  1;
    bool     EthTx         :  1;
    bool     EthRx         :  1;
    bool     EthPtp        :  1;
    bool     UsbOtgHs      :  1;
    bool     UsbOtgHsUlpi  :  1;
    unsigned /* Pad */     :  1;

    // RCC_AHB2ENR
    bool     DigCamIntf    :  1;
    unsigned /* Pad */     :  3;
    bool     Crypto        :  1;
    bool     Hash          :  1;
    bool     RandGen       :  1;
    bool     UsbOtgFs      :  1;
    unsigned /* Pad */     : 24;

    // RCC_AHB3ENR
    bool     FlexStaticMem :  1;
    unsigned /* Pad */     : 31;

    // Reserved
    unsigned /* Pad */     : 32;

    // RCC_APB1ENR
    bool     Timer2        :  1;
    bool     Timer3        :  1;
    bool     Timer4        :  1;
    bool     Timer5        :  1;
    bool     Timer6        :  1;
    bool     Timer7        :  1;
    bool     Timer12       :  1;
    bool     Timer13       :  1;
    bool     Timer14       :  1;
    unsigned /* Pad */     :  2;
    bool     WinWatchDog   :  1;
    unsigned /* Pad */     :  2;
    bool     SPI2          :  1;
    bool     SPI3          :  1;
    unsigned /* Pad */     :  1;
    bool     Usart2        :  1;
    bool     Usart3        :  1;
    bool     Uart4         :  1;
    bool     Uart5         :  1;
    bool     I2C1          :  1;
    bool     I2C2          :  1;
    bool     I2C3          :  1;
    unsigned /* Pad */     :  1;
    bool     CAN1          :  1;
    bool     CAN2          :  1;
    unsigned /* Pad */     :  1;
    bool     PowerIntf     :  1;
    bool     DAC           :  1;
    unsigned /* Pad */     :  2;

    // RCC_APB2ENR
    bool     Timer1        :  1;
    bool     Timer8        :  1;
    unsigned /* Pad */     :  2;
    bool     Usart1        :  1;
    bool     Usart6        :  1;
    unsigned /* Pad */     :  2;
    bool     ADC1          :  1;
    bool     ADC2          :  1;
    bool     ADC3          :  1;
    bool     SDIO          :  1;
    bool     SPI1          :  1;
    bool     SPI4          :  1;
    bool     SysConfig     :  1;
    unsigned /* Pad */     :  1;
    bool     Timer9        :  1;
    bool     Timer10       :  1;
    bool     Timer11       :  1;
    unsigned /* Pad */     :  1;
    bool     SPI5          :  1;
    bool     SPI6          :  1;
    unsigned /* Pad */     : 10;
} SystemClockEnabled_t;

/* Utility ********************************************************************/

typedef enum SystemClockType_t {
    SystemClockType_Hsi,
    SystemClockType_Hse,
    SystemClockType_Pll
} SystemClockType_t;

bool SystemClockEnable(SystemClockType_t clock);

#endif
