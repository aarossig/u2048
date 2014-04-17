/*
 * STM32F4 UART Driver Implementation
 */

#include "SystemUart.h"

/* Buffers ********************************************************************/

volatile SystemUartBuffer_t *uart1Buf;
volatile SystemUartBuffer_t *uart2Buf;
volatile SystemUartBuffer_t *uart3Buf;
volatile SystemUartBuffer_t *uart4Buf;
volatile SystemUartBuffer_t *uart5Buf;
volatile SystemUartBuffer_t *uart6Buf;

/* Initialization *************************************************************/

bool SystemUartInit(volatile SystemUartModule_t *uart,
    volatile SystemUartBuffer_t *buf,
    uint32_t baud)
{
    if(buf == NULL)
    {
        return false;
    }
    
    if(uart == &SystemUart1)
    {
        uart1Buf = buf;
        SystemNvic.SetEnable.Usart1 = true;
        SystemClockEnabled.Usart1 = true;
    }
    else if(uart == &SystemUart2)
    {
        uart2Buf = buf;
        SystemNvic.SetEnable.Usart2 = true;
        SystemClockEnabled.Usart2 = true;
    }
    else if(uart == &SystemUart3)
    {
        uart3Buf = buf;
        SystemNvic.SetEnable.Usart3 = true;
        SystemClockEnabled.Usart3 = true;
    }
    else if(uart == &SystemUart4)
    {
        uart4Buf = buf;
        SystemNvic.SetEnable.Uart4 = true;
        SystemClockEnabled.Uart4 = true;
    }
    else if(uart == &SystemUart5)
    {
        uart5Buf = buf;
        SystemNvic.SetEnable.Uart5 = true;
        SystemClockEnabled.Uart5 = true;
    }
    else if(uart == &SystemUart6)
    {
        uart6Buf = buf;
        SystemNvic.SetEnable.Usart6 = true;
        SystemClockEnabled.Usart6 = true;
    }
    else
    {
        return false;
    }

    uart->Config.Baud.Mantissa =
        F_CPU / (8 * (2 - uart->Config.OversampleMode) * baud);
    uart->Config.Baud.Fraction =
        (16 * F_CPU) / (8 * (2 - uart->Config.OversampleMode) * baud);
    
    uart->Config.RxEnabled = true;
    uart->Config.TxEnabled = true;
    uart->Config.TxDoneIntEnabled = true;
    uart->Config.RxDoneIntEnabled = true;
    uart->Config.Enabled = true;
    
    return true;
}

/* Utility ********************************************************************/

volatile SystemUartBuffer_t *SystemUartBuf(volatile SystemUartModule_t *uart)
{
    if(uart == &SystemUart1)
    {
        return uart1Buf;
    }
    else if(uart == &SystemUart2)
    {
        return uart2Buf;
    }
    else if(uart == &SystemUart3)
    {
        return uart3Buf;
    }
    else if(uart == &SystemUart4)
    {
        return uart4Buf;
    }
    else if(uart == &SystemUart5)
    {
        return uart5Buf;
    }
    else if(uart == &SystemUart6)
    {
        return uart6Buf;
    }
    else
    {
        return NULL;
    }
}

uint32_t SystemUartBytesToRead(volatile SystemUartModule_t *uart)
{
    volatile SystemUartBuffer_t *buf = SystemUartBuf(uart);
    
    if(buf != NULL)
    {
        return buf->Rx.Count;
    }
    else
    {
        return 0;
    }
}

/* Transmit *******************************************************************/

bool SystemUartTx(volatile SystemUartModule_t *uart, const uint32_t data)
{
    volatile SystemUartBuffer_t *buf = SystemUartBuf(uart);
    
    if(buf != NULL)
    {
        if(!(buf->Transmitting))
        {
            uart->Data = data;
            buf->Transmitting = true;
        }
        else
        {
            bool stored = false;
            do {
                uart->Config.TxDoneIntEnabled = false;
                stored = SystemRingBufferWrite(&(buf->Tx), data);
                uart->Config.TxDoneIntEnabled = true;
            } while(!stored);
        }
        
        return true;
    }
    else
    {
        return false;
    }
}

bool SystemUartTxBuf(volatile SystemUartModule_t *uart,
    const uint32_t *data,
    const uint32_t length)
{
    volatile SystemUartBuffer_t *buf = SystemUartBuf(uart);
   
    if(buf != NULL)
    {
        for(uint32_t i = 0; i < length; i++)
        {
            if(i == 0 && !(buf->Transmitting))
            {
                uart->Data = data[i];
                buf->Transmitting = true;
            }
            else
            {
                bool stored = false;
                do {
                    uart->Config.TxDoneIntEnabled = false;
                    stored = SystemRingBufferWrite(&(buf->Tx), data[i]);
                    uart->Config.TxDoneIntEnabled = true;
                } while(!stored);
            }
        }
        
        return true;
    }
    else
    {
        return false;
    }
}

bool SystemUartTxStr(volatile SystemUartModule_t *uart,
    const char *str,
    const uint32_t length)
{
    volatile SystemUartBuffer_t *buf = SystemUartBuf(uart);
   
    if(buf != NULL)
    {
        for(uint32_t i = 0; i < length; i++)
        {
            if(i == 0 && !(buf->Transmitting))
            {
                uart->Data = str[i];
                buf->Transmitting = true;
            }
            else
            {
                bool stored = false;
                do {
                    uart->Config.TxDoneIntEnabled = false;
                    stored = SystemRingBufferWrite(&(buf->Tx), str[i]);
                    uart->Config.TxDoneIntEnabled = true;
                } while(!stored);
            }
        }
        
        return true;
    }
    else
    {
        return false;
    }
}

/* Receive ********************************************************************/

bool SystemUartRx(volatile SystemUartModule_t *uart, uint32_t *data)
{
    volatile SystemUartBuffer_t *buf = SystemUartBuf(uart);
    
    if(buf != NULL)
    {
        uart->Config.RxDoneIntEnabled = false;
        bool stored = SystemRingBufferRead(&(buf->Rx), data);
        uart->Config.RxDoneIntEnabled = true;
        return stored;
    }
    else
    {
        return false;
    }
}

bool SystemUartRxBuf(volatile SystemUartModule_t *uart,
     uint32_t *data,
     const uint32_t length)
{
    volatile SystemUartBuffer_t *buf = SystemUartBuf(uart);
    
    if(buf != NULL && buf->Rx.Count >= length)
    {
        for(uint32_t i = 0; i < length; i++)
        {
            uart->Config.RxDoneIntEnabled = false;
            SystemRingBufferRead(&(buf->Rx), &data[i]);
            uart->Config.RxDoneIntEnabled = true;
        }
        
        return true;
    }
    else
    {
        return false;
    }
}

bool SystemUartRxStr(volatile SystemUartModule_t *uart,
    char *data,
    const uint32_t length)
{
    volatile SystemUartBuffer_t *buf = SystemUartBuf(uart);
    
    if(buf != NULL && buf->Rx.Count >= length)
    {
        for(uint32_t i = 0; i < length; i++)
        {
            uart->Config.RxDoneIntEnabled = false;
            uint32_t chr = 0;
            SystemRingBufferRead(&(buf->Rx), &chr);
            data[i] = chr;
            uart->Config.RxDoneIntEnabled = true;
        }
        
        return true;
    }
    else
    {
        return false;
    }
}

/* Interrupt Handlers *********************************************************/

void handleInterrupt(volatile SystemUartModule_t *uart,
    volatile SystemUartBuffer_t *buf)
{
    if(uart->Status.TxComplete)
    {
        uint32_t nextData = 0;
        if(SystemRingBufferRead(&(buf->Tx), &nextData))
        {
            uart->Data = nextData;
        }
        else
        {
            uart->Status.TxComplete = false;
            buf->Transmitting = false;
        }
    }
    else if(uart->Status.RxComplete)
    {
        SystemRingBufferWrite(&(buf->Rx), uart->Data);
    }
}

void __attribute__ ((interrupt ("IRQ"))) IsrUsart1(void)
{
    handleInterrupt(&(SystemUart1), uart1Buf);
}

void __attribute__ ((interrupt ("IRQ"))) IsrUsart2(void)
{
    handleInterrupt(&(SystemUart2), uart2Buf);
}

void __attribute__((interrupt ("IRQ"))) IsrUsart3(void)
{
    handleInterrupt(&(SystemUart3), uart3Buf);
}

void __attribute__((interrupt ("IRQ"))) IsrUart4(void)
{
    handleInterrupt(&(SystemUart4), uart4Buf);
}

void __attribute__((interrupt ("IRQ"))) IsrUart5(void)
{
    handleInterrupt(&(SystemUart5), uart5Buf);
}

void __attribute__((interrupt ("IRQ"))) IsrUsart6(void)
{
    handleInterrupt(&(SystemUart6), uart6Buf);
}

