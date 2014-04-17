/*
 * STM32F4 SPI Driver Implementation
 */

#include "SystemSpi.h"

uint16_t SystemSpiWriteRead(volatile SystemSpiModule_t *spi, uint16_t data)
{
    spi->Data = data;
    while(!spi->Status.TxEmpty);
    while(!spi->Status.RxFull);
    return spi->Data;
}
