/*
 * SMT32F4 Global Interrupts Management
 */

#ifndef SYSTEMINTERRUPTS_H
#define SYSTEMINTERRUPTS_H

#define ADDR_NVIC 0xE000E100
#define SystemNvic (*((SystemNvic_t *)ADDR_NVIC))

/* Enable/Disable Global Interrupts *******************************************/

#define SystemIrqEnable(enable) { \
    __asm__("cpsie i");           \
}

#define SystemIrqDisable() { \
    __asm__("cpsid i");      \
}

/* NVIC Set/Reset *************************************************************/

typedef struct SystemNvicRegt_t {
    union {
        struct {
            // NVIC_ISER0
            bool Wwdg            : 1;
            bool Pvd             : 1;
            bool TampStamp       : 1;
            bool RtcWkup         : 1;
            bool Flash           : 1;
            bool Rcc             : 1;
            bool Exti0           : 1;
            bool Exti1           : 1;
            bool Exti2           : 1;
            bool Exti3           : 1;
            bool Exti4           : 1;
            bool Dma1Stream0     : 1;
            bool Dma1Stream1     : 1;
            bool Dma1Stream2     : 1;
            bool Dma1Stream3     : 1;
            bool Dma1Stream4     : 1;
            bool Dma1Stream5     : 1;
            bool Dma1Stream6     : 1;
            bool Adc             : 1;
            bool Can1Tx          : 1;
            bool Can1Rx0         : 1;
            bool Can1Rx1         : 1;
            bool Can1Sce         : 1;
            bool Exti95          : 1;
            bool Tim1BrkTim9     : 1;
            bool Tim1UpTim10     : 1;
            bool Tim1TrgComTim11 : 1;
            bool Tim1Cc          : 1;
            bool Tim2            : 1;
            bool Tim3            : 1;
            bool Tim4            : 1;
            bool I2C1Ev          : 1;

            // NVIC_ISER1
            bool I2C1Er          : 1;
            bool I2C2Ev          : 1;
            bool I2C2Er          : 1;
            bool Spi1            : 1;
            bool Spi2            : 1;
            bool Usart1          : 1;
            bool Usart2          : 1;
            bool Usart3          : 1;
            bool Exti1510        : 1;
            bool RtcAlarm        : 1;
            bool OtgFsWkup       : 1;
            bool Tim8BrkTim12    : 1;
            bool Tim8UpTim13     : 1;
            bool Tim8TrgComTim14 : 1;
            bool Tim8Cc          : 1;
            bool Dma1Stream7     : 1;
            bool Fsmc            : 1;
            bool Sdio            : 1;
            bool Tim5            : 1;
            bool Spi3            : 1;
            bool Uart4           : 1;
            bool Uart5           : 1;
            bool Tim6Dac         : 1;
            bool Tim7            : 1;
            bool Dma2Stream0     : 1;
            bool Dma2Stream1     : 1;
            bool Dma2Stream2     : 1;
            bool Dma2Stream3     : 1;
            bool Dma2Stream4     : 1;
            bool Eth             : 1;
            bool EthWkup         : 1;
            bool Can2Tx          : 1;

            // NVIC_ISER2
            bool Can2Rx0         : 1;
            bool Can2Rx1         : 1;
            bool Can2Sce         : 1;
            bool OtgFs           : 1;
            bool Dma2Stream5     : 1;
            bool Dma2Stream6     : 1;
            bool Dma2Stream7     : 1;
            bool Usart6          : 1;
            bool I2C3Ev          : 1;
            bool I2C3Er          : 1;
            bool OtgHsEp1Out     : 1;
            bool OtgHsEp1In      : 1;
            bool OtgHsWkup       : 1;
            bool OtgHs           : 1;
            bool Dcmi            : 1;
            bool Cryp            : 1;
            bool HashRng         : 1;
            bool Fpu             : 1;
        };
        
        uint32_t Reg0      : 32;
        uint32_t Reg1      : 32;
        uint32_t Reg2      : 18;
        unsigned /* Pad */ : 14;
    };
} SystemNvicReg_t;

/* NVIC Priority **************************************************************/

typedef struct SystemNvicPriority_t {
     union {
        struct {
            // NVIC_IPR0
            unsigned /* Pad */       : 4;
            uint32_t Wwdg            : 4;
            unsigned /* Pad */       : 4;
            uint32_t Pvd             : 4;
            unsigned /* Pad */       : 4;
            uint32_t TampStamp       : 4;
            unsigned /* Pad */       : 4;
            uint32_t RtcWkup         : 4;

            // NVIC_IPR1
            unsigned /* Pad */       : 4;
            uint32_t Flash           : 4;
            unsigned /* Pad */       : 4;
            uint32_t Rcc             : 4;
            unsigned /* Pad */       : 4;
            uint32_t Exti0           : 4;
            unsigned /* Pad */       : 4;
            uint32_t Exti1           : 4;

            // NVIC_IPR2
            unsigned /* Pad */       : 4;
            uint32_t Exti2           : 4;
            unsigned /* Pad */       : 4;
            uint32_t Exti3           : 4;
            unsigned /* Pad */       : 4;
            uint32_t Exti4           : 4;
            unsigned /* Pad */       : 4;
            uint32_t Dma1Stream0     : 4;

            // NVIC_IPR3
            unsigned /* Pad */       : 4;
            uint32_t Dma1Stream1     : 4;
            unsigned /* Pad */       : 4;
            uint32_t Dma1Stream2     : 4;
            unsigned /* Pad */       : 4;
            uint32_t Dma1Stream3     : 4;
            unsigned /* Pad */       : 4;
            uint32_t Dma1Stream4     : 4;

            // NVIC_IPR4
            unsigned /* Pad */       : 4;
            uint32_t Dma1Stream5     : 4;
            unsigned /* Pad */       : 4;
            uint32_t Dma1Stream6     : 4;
            unsigned /* Pad */       : 4;
            uint32_t Adc             : 4;
            unsigned /* Pad */       : 4;
            uint32_t Can1Tx          : 4;

            // NVIC_IPR5
            unsigned /* Pad */       : 4;
            uint32_t Can1Rx0         : 4;
            unsigned /* Pad */       : 4;
            uint32_t Can1Rx1         : 4;
            unsigned /* Pad */       : 4;
            uint32_t Can1Sce         : 4;
            unsigned /* Pad */       : 4;
            uint32_t Exti95          : 4;

            // NVIC_IPR6
            unsigned /* Pad */       : 4;
            uint32_t Tim1BrkTim9     : 4;
            unsigned /* Pad */       : 4;
            uint32_t Tim1UpTim40     : 4;
            unsigned /* Pad */       : 4;
            uint32_t Tim1TrgComTim11 : 4;
            unsigned /* Pad */       : 4;
            uint32_t Tim1Cc          : 4;

            // NVIC_IPR7
            unsigned /* Pad */       : 4;
            uint32_t Tim2            : 4;
            unsigned /* Pad */       : 4;
            uint32_t Tim3            : 4;
            unsigned /* Pad */       : 4;
            uint32_t Tim4            : 4;
            unsigned /* Pad */       : 4;
            uint32_t I2C1Ev          : 4;
            
            // NVIC_IPR8
            unsigned /* Pad */       : 4;
            uint32_t I2C1Er          : 4;
            unsigned /* Pad */       : 4;
            uint32_t I2C2Ev          : 4;
            unsigned /* Pad */       : 4;
            uint32_t I2C2Er          : 4;
            unsigned /* Pad */       : 4;
            uint32_t Spi1            : 4;

            // NVIC_IPR9
            unsigned /* Pad */       : 4;
            uint32_t Spi2            : 4;
            unsigned /* Pad */       : 4;
            uint32_t Usart1          : 4;
            unsigned /* Pad */       : 4;
            uint32_t Usart2          : 4;
            unsigned /* Pad */       : 4;
            uint32_t Usart3          : 4;

            // NVIC_IPR10
            unsigned /* Pad */       : 4;
            uint32_t Exti1510        : 4;
            unsigned /* Pad */       : 4;
            uint32_t RtcAlarm        : 4;
            unsigned /* Pad */       : 4;
            uint32_t OtgFsWkup       : 4;
            unsigned /* Pad */       : 4;
            uint32_t Tim8BrkTim12    : 4;

            // NVIC_IRP11
            unsigned /* Pad */       : 4;
            uint32_t Tim8UpTim13     : 4;
            unsigned /* Pad */       : 4;
            uint32_t Tim8TrgComTim14 : 4;
            unsigned /* Pad */       : 4;
            uint32_t Tim8Cc          : 4;
            unsigned /* Pad */       : 4;
            uint32_t Dma1Stream7     : 4;

            // NVIC_IPR12
            unsigned /* Pad */       : 4;
            uint32_t Fsmc            : 4;
            unsigned /* Pad */       : 4;
            uint32_t Sdio            : 4;
            unsigned /* Pad */       : 4;
            uint32_t Tim5            : 4;
            unsigned /* Pad */       : 4;
            uint32_t Spi3            : 4;

            // NVIC_IPR13
            unsigned /* Pad */       : 4;
            uint32_t Uart4           : 4;
            unsigned /* Pad */       : 4;
            uint32_t Uart5           : 4;
            unsigned /* Pad */       : 4;
            uint32_t Tim6Dac         : 4;
            unsigned /* Pad */       : 4;
            uint32_t Tim7            : 4;

            // NVIC_IPR14
            unsigned /* Pad */       : 4;
            uint32_t Dma2Stream0     : 4;
            unsigned /* Pad */       : 4;
            uint32_t Dma2Stream1     : 4;
            unsigned /* Pad */       : 4;
            uint32_t Dma2Stream2     : 4;
            unsigned /* Pad */       : 4;
            uint32_t Dma2Stream3     : 4;
            
            // NVIC_IPR15
            unsigned /* Pad */       : 4;
            uint32_t Dma2Stream4     : 4;
            unsigned /* Pad */       : 4;
            uint32_t Eth             : 4;
            unsigned /* Pad */       : 4;
            uint32_t EthWkup         : 4;
            unsigned /* Pad */       : 4;
            uint32_t Can2Tx          : 4;

            // NVIC_IPR16
            unsigned /* Pad */       : 4;
            uint32_t Can2Rx0         : 4;
            unsigned /* Pad */       : 4;
            uint32_t Can2Rx1         : 4;
            unsigned /* Pad */       : 4;
            uint32_t Can2Sce         : 4;
            unsigned /* Pad */       : 4;
            uint32_t OtgFs           : 4;

            // NVIC_IPR17
            unsigned /* Pad */       : 4;
            uint32_t Dma2Stream5     : 4;
            unsigned /* Pad */       : 4;
            uint32_t Dma2Stream6     : 4;
            unsigned /* Pad */       : 4;
            uint32_t Dma2Stream7     : 4;
            unsigned /* Pad */       : 4;
            uint32_t Usart6          : 4;

            // NVIC_IPR18
            unsigned /* Pad */       : 4;
            uint32_t I2C3Ev          : 4;
            unsigned /* Pad */       : 4;
            uint32_t I2C3Er          : 4;
            unsigned /* Pad */       : 4;
            uint32_t OtgHsEp1Out     : 4;
            unsigned /* Pad */       : 4;
            uint32_t OtgHsEp4In      : 4;
            
            // NVIC_IPR19
            unsigned /* Pad */       : 4;
            uint32_t OtgHsWkup       : 4;
            unsigned /* Pad */       : 4;
            uint32_t OtgHs           : 4;
            unsigned /* Pad */       : 4;
            uint32_t Dcmi            : 4;
            unsigned /* Pad */       : 4;
            uint32_t Cryp            : 4;

            // NVIC_IPR20
            unsigned /* Pad */       : 4;
            uint32_t HashRng         : 4;
            unsigned /* Pad */       : 4;
            uint32_t Fpu             : 4;
            unsigned /* Pad */       : 16;
        };
        
        uint32_t Reg0      : 32;
        uint32_t Reg1      : 32;
        uint32_t Reg2      : 32;
        uint32_t Reg3      : 32;
        uint32_t Reg4      : 32;
        uint32_t Reg5      : 32;
        uint32_t Reg6      : 32;
        uint32_t Reg7      : 32;
        uint32_t Reg8      : 32;
        uint32_t Reg9      : 32;
        uint32_t Reg10     : 32;
        uint32_t Reg11     : 32;
        uint32_t Reg12     : 32;
        uint32_t Reg13     : 32;
        uint32_t Reg14     : 32;
        uint32_t Reg15     : 32;
        uint32_t Reg16     : 32;
        uint32_t Reg17     : 32;
        uint32_t Reg18     : 32;
        uint32_t Reg19     : 32;
        uint32_t Reg20     : 16;
        unsigned /* Pad */ : 16;
    };
   
} SystemNvicPriority_t;

/* NVIC Module ****************************************************************/

typedef struct SystemNvic_t {
    SystemNvicReg_t SetEnable;
    uint8_t __pad1[116];
    SystemNvicReg_t ClearEnable;
    uint8_t __pad2[116];
    SystemNvicReg_t SetPending;
    uint8_t __pad3[116];
    SystemNvicReg_t ClearPending;
    uint8_t __pad4[116];
    SystemNvicReg_t Active;
    uint8_t __pad5[116];
    SystemNvicPriority_t Priority;
} SystemNvic_t;

#endif

