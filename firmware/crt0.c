/*
 * Vector table and start code for STM32F4
 */

#include <stdlib.h>
#include <stdint.h>

/* Linker Generated Symbols ***************************************************/

extern uint32_t __stackPointer;

extern uint32_t __flashData;
extern uint32_t __dataStart;
extern uint32_t __dataEnd;

extern uint32_t __bssStart;
extern uint32_t __bssEnd;

/* C Runtime ******************************************************************/

extern int main(void);

void __crt0(void) {
    // Copy Data Region
    uint32_t *pSrc = &__flashData;
    uint32_t *pDest = &__dataStart;
    
    while(pDest < &__dataEnd)
    {
        *pDest++ = *pSrc++;
    }
    
    // Zero bss
    pDest = &__bssStart;
    
    while(pDest < &__bssEnd)
    {
        *pDest++ = 0;
    }
    
    // Call through to main
    main();
    
    // Trap an evil main that returns
    while(1);
}

/* Default Interrupt Handler **************************************************/

void __defaultInterrupt(void) {
    while(1);
}

/* Interrupt Handlers *********************************************************/

void IsrNmi() __attribute__ ((weak, alias("__defaultInterrupt")));
void IsrHardFault() __attribute__ ((weak, alias("__defaultInterrupt")));
void IsrMemManage() __attribute__ ((weak, alias("__defaultInterrupt")));
void IsrBusFault() __attribute__ ((weak, alias("__defaultInterrupt")));
void IsrUsageFault() __attribute__ ((weak, alias("__defaultInterrupt")));
void IsrSvCall() __attribute__ ((weak, alias("__defaultInterrupt")));
void IsrDebugMonitor() __attribute__ ((weak, alias("__defaultInterrupt")));
void IsrPendSv() __attribute__ ((weak, alias("__defaultInterrupt")));
void IsrSysTick() __attribute__ ((weak, alias("__defaultInterrupt")));
void IsrWwdg() __attribute__ ((weak, alias("__defaultInterrupt")));
void IsrPvd() __attribute__ ((weak, alias("__defaultInterrupt")));
void IsrTampStamp() __attribute__ ((weak, alias("__defaultInterrupt")));
void IsrRtcWkup() __attribute__ ((weak, alias("__defaultInterrupt")));
void IsrFlash() __attribute__ ((weak, alias("__defaultInterrupt")));
void IsrRcc() __attribute__ ((weak, alias("__defaultInterrupt")));
void IsrExti0() __attribute__ ((weak, alias("__defaultInterrupt")));
void IsrExti1() __attribute__ ((weak, alias("__defaultInterrupt")));
void IsrExti2() __attribute__ ((weak, alias("__defaultInterrupt")));
void IsrExti3() __attribute__ ((weak, alias("__defaultInterrupt")));
void IsrExti4() __attribute__ ((weak, alias("__defaultInterrupt")));
void IsrDma1Stream0() __attribute__ ((weak, alias("__defaultInterrupt")));
void IsrDma1Stream1() __attribute__ ((weak, alias("__defaultInterrupt")));
void IsrDma1Stream2() __attribute__ ((weak, alias("__defaultInterrupt")));
void IsrDma1Stream3() __attribute__ ((weak, alias("__defaultInterrupt")));
void IsrDma1Stream4() __attribute__ ((weak, alias("__defaultInterrupt")));
void IsrDma1Stream5() __attribute__ ((weak, alias("__defaultInterrupt")));
void IsrDma1Stream6() __attribute__ ((weak, alias("__defaultInterrupt")));
void IsrAdc() __attribute__ ((weak, alias("__defaultInterrupt")));
void IsrCan1Tx() __attribute__ ((weak, alias("__defaultInterrupt")));
void IsrCan1Rx0() __attribute__ ((weak, alias("__defaultInterrupt")));
void IsrCan1Rx1() __attribute__ ((weak, alias("__defaultInterrupt")));
void IsrCan1Sce() __attribute__ ((weak, alias("__defaultInterrupt")));
void IsrExti95() __attribute__ ((weak, alias("__defaultInterrupt")));
void IsrTim1BrkTim9() __attribute__ ((weak, alias("__defaultInterrupt")));
void IsrTim1UpTim10() __attribute__ ((weak, alias("__defaultInterrupt")));
void IsrTim1TrgComTim11() __attribute__ ((weak, alias("__defaultInterrupt")));
void IsrTim1Cc() __attribute__ ((weak, alias("__defaultInterrupt")));
void IsrTim2() __attribute__ ((weak, alias("__defaultInterrupt")));
void IsrTim3() __attribute__ ((weak, alias("__defaultInterrupt")));
void IsrTim4() __attribute__ ((weak, alias("__defaultInterrupt")));
void IsrI2C1Ev() __attribute__ ((weak, alias("__defaultInterrupt")));
void IsrI2C1Er() __attribute__ ((weak, alias("__defaultInterrupt")));
void IsrI2C2Ev() __attribute__ ((weak, alias("__defaultInterrupt")));
void IsrI2C2Er() __attribute__ ((weak, alias("__defaultInterrupt")));
void IsrSpi1() __attribute__ ((weak, alias("__defaultInterrupt")));
void IsrSpi2() __attribute__ ((weak, alias("__defaultInterrupt")));
void IsrUsart1() __attribute__ ((weak, alias("__defaultInterrupt")));
void IsrUsart2() __attribute__ ((weak, alias("__defaultInterrupt")));
void IsrUsart3() __attribute__ ((weak, alias("__defaultInterrupt")));
void IsrExti1510() __attribute__ ((weak, alias("__defaultInterrupt")));
void IsrRtcAlarm() __attribute__ ((weak, alias("__defaultInterrupt")));
void IsrOtgFsWkup() __attribute__ ((weak, alias("__defaultInterrupt")));
void IsrTim8BrkTim12() __attribute__ ((weak, alias("__defaultInterrupt")));
void IsrTim8UpTim13() __attribute__ ((weak, alias("__defaultInterrupt")));
void IsrTim8TrgComTim14() __attribute__ ((weak, alias("__defaultInterrupt")));
void IsrTim8Cc() __attribute__ ((weak, alias("__defaultInterrupt")));
void IsrDma1Stream7() __attribute__ ((weak, alias("__defaultInterrupt")));
void IsrFsmc() __attribute__ ((weak, alias("__defaultInterrupt")));
void IsrSdio() __attribute__ ((weak, alias("__defaultInterrupt")));
void IsrTim5() __attribute__ ((weak, alias("__defaultInterrupt")));
void IsrSpi3() __attribute__ ((weak, alias("__defaultInterrupt")));
void IsrUart4() __attribute__ ((weak, alias("__defaultInterrupt")));
void IsrUart5() __attribute__ ((weak, alias("__defaultInterrupt")));
void IsrTim6Dac() __attribute__ ((weak, alias("__defaultInterrupt")));
void IsrTim7() __attribute__ ((weak, alias("__defaultInterrupt")));
void IsrDma2Stream0() __attribute__ ((weak, alias("__defaultInterrupt")));
void IsrDma2Stream1() __attribute__ ((weak, alias("__defaultInterrupt")));
void IsrDma2Stream2() __attribute__ ((weak, alias("__defaultInterrupt")));
void IsrDma2Stream3() __attribute__ ((weak, alias("__defaultInterrupt")));
void IsrDma2Stream4() __attribute__ ((weak, alias("__defaultInterrupt")));
void IsrEth() __attribute__ ((weak, alias("__defaultInterrupt")));
void IsrEthWkup() __attribute__ ((weak, alias("__defaultInterrupt")));
void IsrCan2Tx() __attribute__ ((weak, alias("__defaultInterrupt")));
void IsrCan2Rx0() __attribute__ ((weak, alias("__defaultInterrupt")));
void IsrCan2Rx1() __attribute__ ((weak, alias("__defaultInterrupt")));
void IsrCan2Sce() __attribute__ ((weak, alias("__defaultInterrupt")));
void IsrOtgFs() __attribute__ ((weak, alias("__defaultInterrupt")));
void IsrDma2Stream5() __attribute__ ((weak, alias("__defaultInterrupt")));
void IsrDma2Stream6() __attribute__ ((weak, alias("__defaultInterrupt")));
void IsrDma2Stream7() __attribute__ ((weak, alias("__defaultInterrupt")));
void IsrUsart6() __attribute__ ((weak, alias("__defaultInterrupt")));
void IsrI2C3Ev() __attribute__ ((weak, alias("__defaultInterrupt")));
void IsrI2C3Er() __attribute__ ((weak, alias("__defaultInterrupt")));
void IsrOtgHsEp1Out() __attribute__ ((weak, alias("__defaultInterrupt")));
void IsrOtgHsEp1In() __attribute__ ((weak, alias("__defaultInterrupt")));
void IsrOtgHsWkup() __attribute__ ((weak, alias("__defaultInterrupt")));
void IsrOtgHs() __attribute__ ((weak, alias("__defaultInterrupt")));
void IsrDcmi() __attribute__ ((weak, alias("__defaultInterrupt")));
void IsrCryp() __attribute__ ((weak, alias("__defaultInterrupt")));
void IsrHashRng() __attribute__ ((weak, alias("__defaultInterrupt")));
void IsrFpu() __attribute__ ((weak, alias("__defaultInterrupt")));

/* Vector Table ***************************************************************/

__attribute__ ((section(".interruptVectorTable")))
const void *__vectorTable[] = {
    &__stackPointer,
    &__crt0,
    &IsrNmi,
    &IsrHardFault,
    &IsrMemManage,
    &IsrBusFault,
    &IsrUsageFault,
    NULL,
    NULL,
    NULL,
    NULL,
    &IsrSvCall,
    &IsrDebugMonitor,
    NULL,
    &IsrPendSv,
    &IsrSysTick,
    &IsrWwdg,
    &IsrPvd,
    &IsrTampStamp,
    &IsrRtcWkup,
    &IsrFlash,
    &IsrRcc,
    &IsrExti0,
    &IsrExti1,
    &IsrExti2,
    &IsrExti3,
    &IsrExti4,
    &IsrDma1Stream0,
    &IsrDma1Stream1,
    &IsrDma1Stream2,
    &IsrDma1Stream3,
    &IsrDma1Stream4,
    &IsrDma1Stream5,
    &IsrDma1Stream6,
    &IsrAdc,
    &IsrCan1Tx,
    &IsrCan1Rx0,
    &IsrCan1Rx1,
    &IsrCan1Sce,
    &IsrExti95,
    &IsrTim1BrkTim9,
    &IsrTim1UpTim10,
    &IsrTim1TrgComTim11,
    &IsrTim1Cc,
    &IsrTim2,
    &IsrTim3,
    &IsrTim4,
    &IsrI2C1Ev,
    &IsrI2C1Er,
    &IsrI2C2Ev,
    &IsrI2C2Er,
    &IsrSpi1,
    &IsrSpi2,
    &IsrUsart1,
    &IsrUsart2,
    &IsrUsart3,
    &IsrExti1510,
    &IsrRtcAlarm,
    &IsrOtgFsWkup,
    &IsrTim8BrkTim12,
    &IsrTim8UpTim13,
    &IsrTim8TrgComTim14,
    &IsrTim8Cc,
    &IsrDma1Stream7,
    &IsrFsmc,
    &IsrSdio,
    &IsrTim5,
    &IsrSpi3,
    &IsrUart4,
    &IsrUart5,
    &IsrTim6Dac,
    &IsrTim7,
    &IsrDma2Stream0,
    &IsrDma2Stream1,
    &IsrDma2Stream2,
    &IsrDma2Stream3,
    &IsrDma2Stream4,
    &IsrEth,
    &IsrEthWkup,
    &IsrCan2Tx,
    &IsrCan2Rx0,
    &IsrCan2Rx1,
    &IsrCan2Sce,
    &IsrOtgFs,
    &IsrDma2Stream5,
    &IsrDma2Stream6,
    &IsrDma2Stream7,
    &IsrUsart6,
    &IsrI2C3Ev,
    &IsrI2C3Er,
    &IsrOtgHsEp1Out,
    &IsrOtgHsEp1In,
    &IsrOtgHsWkup,
    &IsrOtgHs,
    &IsrDcmi,
    &IsrCryp,
    &IsrHashRng,
    &IsrFpu
};

