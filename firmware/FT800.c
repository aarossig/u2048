/*
 * Driver for the FT800 Embedded Video Controller
 */

#include "FT800.h"

/* FT800 Instance *************************************************************/

void FT800Init(FT800_t *ft800)
{
    ft800->CommandAddress = 0;
}

bool FT800ColorsEqual(const FT800Color_t c1, const FT800Color_t c2)
{
    return (c1.Red == c2.Red
        && c1.Green == c2.Green
        && c1.Blue == c2.Blue);
}

/* FT800 Register Abstractions ************************************************/

uint8_t FT800ReadChipId(FT800_t *ft800)
{
    uint8_t chipId[1];
    FT800Read(ft800, FT800Register_ID, chipId, 1);
    return chipId[0];
}

void FT800SwapDisplayList(FT800_t *ft800)
{
    uint8_t dlSwap[] = { 0x02, 0x00, 0x00, 0x00 };
    FT800Write(ft800, FT800Register_DLSWAP, dlSwap, 4);
}

/* FT800 Abstracted Drawing ***************************************************/

void FT800DrawRectangle(FT800_t *ft800, FT800Point_t p1, FT800Point_t p2)
{
    FT800DlStartPrimitive(ft800, FT800PrimitiveType_Rectangle);
    FT800DlVertexI(ft800, p1.X, p1.Y, 0, 0);
    FT800DlVertexI(ft800, p2.X - 1, p2.Y - 1, 0, 0);
    FT800DlEndPrimitive(ft800);
}

/* FT800 Touchscreen **********************************************************/

void FT800GetTouchGesture(FT800_t *ft800, FT800GestureDetail_t *detail)
{
    FT800TouchDetail_t t1 = { .State = FT800TouchState_NotTouched };
    FT800TouchDetail_t t2;
    FT800TouchDetail_t tmp;
    
    while(t1.State == FT800TouchState_NotTouched)
    {
        FT800ReadTouchState(ft800, &t1);

        if(t1.State == FT800TouchState_NotTouched)
        {
            detail->Gesture = FT800Gesture_None;
            return;
        }
    }
    
    FT800ReadTouchState(ft800, &tmp);
    while(tmp.State == FT800TouchState_Touched)
    {
        t2 = tmp;
        FT800ReadTouchState(ft800, &tmp);
    }

    int deltaX = t2.X - t1.X;
    int deltaY = t2.Y - t1.Y;
    int absX = deltaX < 0 ? -deltaX : deltaX;
    int absY = deltaY < 0 ? -deltaY : deltaY;
    
    detail->Position.X = t1.X;
    detail->Position.Y = t1.Y;

    if(absX > absY && absX > FT800_SWIPE_X)
    {
        if(deltaX < 0)
            detail->Gesture = FT800Gesture_SwipeLeft;
        else
            detail->Gesture = FT800Gesture_SwipeRight;
    }
    else if(absX <= absY && absY > FT800_SWIPE_Y)
    {
        if(deltaY < 0)
            detail->Gesture = FT800Gesture_SwipeDown;
        else
            detail->Gesture = FT800Gesture_SwipeUp;
    }
    else
    {
        detail->Gesture = FT800Gesture_Touch;
    }
}

void FT800ReadTouchState(FT800_t *ft800, FT800TouchDetail_t *detail)
{
    uint8_t touchBuf[4];
    FT800Read(ft800, FT800Register_TOUCH_DIRECT_XY, touchBuf, 4);
    
    if(touchBuf[3] & 0x80)
    {
        detail->State = FT800TouchState_NotTouched;
        detail->X = 0;
        detail->Y = 0;
    }
    else
    {
        detail->State = FT800TouchState_Touched;
        detail->Y = touchBuf[0] | ((touchBuf[1] & 0x03) << 8);
        detail->X = touchBuf[2] | ((touchBuf[3] & 0x03) << 8);
    }
}

/* FT800 Display List *********************************************************/

void FT800DlStartPrimitive(FT800_t *ft800, FT800PrimitiveType_t primitive)
{
    uint8_t startPrimitive[] = { (primitive & 0x0F), 0x00, 0x00, 0x1F };
    FT800CoprocessorCommand(ft800, startPrimitive, 4);
}

void FT800DlEndPrimitive(FT800_t *ft800)
{
    uint8_t endPrimitive[] = { 0x00, 0x00, 0x00, 0x21 };
    FT800CoprocessorCommand(ft800, endPrimitive, 4);
}

void FT800DlVertexI(FT800_t *ft800,
    uint16_t x, uint16_t y, uint8_t handle, uint8_t cell)
{
    union {
        struct {
            uint32_t cell   : 7;
            uint32_t handle : 5;
            uint32_t y      : 9;
            uint32_t x      : 9;
            uint32_t cmd    : 2;
        } arguments;
        
        uint8_t command[4];
    } vertex;

    vertex.arguments.cell = cell;
    vertex.arguments.handle = handle;
    vertex.arguments.y = y;
    vertex.arguments.x = x;
    vertex.arguments.cmd = 0x02;
    
    FT800CoprocessorCommand(ft800, vertex.command, 4);
}

void FT800DlClearRgb(FT800_t *ft800, FT800Color_t color)
{
    uint8_t clearRgb[] = { color.Blue, color.Green, color.Red, 0x02 };
    FT800CoprocessorCommand(ft800, clearRgb, 4);
}

void FT800DlRgb(FT800_t *ft800, FT800Color_t color)
{
    uint8_t rgb[] = { color.Blue, color.Green, color.Red, 0x04 };
    FT800CoprocessorCommand(ft800, rgb, 4);
}

void FT800DlClearCSTBuffers(FT800_t *ft800, bool cBuf, bool sBuf, bool tBuf)
{
    uint8_t clearBufs[] = {
        (tBuf == 1) | ((sBuf == 1) << 1) | ((cBuf == 1) << 2),
        0x00,
        0x00,
        0x26
    };
    
    FT800CoprocessorCommand(ft800, clearBufs, 4);
}

void FT800DlEnd(FT800_t *ft800)
{
    uint8_t dlEnd[] = { 0x00, 0x00, 0x00, 0x00 };
    FT800CoprocessorCommand(ft800, dlEnd, 4);
}

/* FT800 Coprocessor Commands *************************************************/

void FT800CmdNewDisplayList(FT800_t *ft800)
{
//    uint8_t newDisplayList[] = { 0x00, 0xFF, 0xFF, 0xFF };
//    FT800CoprocessorCommand(ft800, newDisplayList, 4);

    for(volatile int i = 0; i < 1000; i++);

    uint8_t newDisplayList[] = { 0x00, 0x00 };
    FT800Write(ft800, FT800Register_CMD_DL, newDisplayList, 2);
}

void FT800CmdSwapDisplayList(FT800_t *ft800)
{
    uint8_t swapDisplayList[] = { 0x01, 0xFF, 0xFF, 0xFF };
    FT800CoprocessorCommand(ft800, swapDisplayList, 4);
}

void FT800CmdDrawText(FT800_t *ft800, FT800Point_t p, FT800Font_t font,
    FT800Option_t options, const char *str, uint32_t length)
{
    uint32_t address = FT800_CMD_START + ft800->CommandAddress;

    uint8_t addressBuf[] = {
        ((address >> 16) & 0x3F) | 0x80,
        (address >> 8) & 0xFF,
        (address) & 0xFF
    };
    
    uint8_t showText[] = {
        0x0C, 0xFF, 0xFF, 0xFF,
        p.X, (p.X >> 8),
        p.Y, (p.Y >> 8),
        font, (font >> 8),
        options, (options >> 8)
    };

    uint8_t terminator[] = { 0x00, 0x00, 0x00, 0x00 };

    ft800->CsPort->Output.Port &= ~(1 << ft800->CsPin);
    FT800SpiWrite(ft800, addressBuf, 3);
    FT800SpiWrite(ft800, showText, 12);
    FT800SpiWrite(ft800, (uint8_t *)str, length);
    FT800SpiWrite(ft800, terminator, 4 - (length % 4));
    ft800->CsPort->Output.Port |= (1 << ft800->CsPin);
    
    ft800->CommandAddress += 12 + length + (4 - (length % 4));
    ft800->CommandAddress %= 4096;
}

void FT800CmdLogo(FT800_t *ft800)
{
    uint8_t logo[] = { 0x31, 0xFF, 0xFF, 0xFF };
    FT800CoprocessorCommand(ft800, logo, 4);
}

void FT800CmdFlush(FT800_t *ft800)
{
    uint8_t address[] = {
        ft800->CommandAddress,
        ft800->CommandAddress >> 8,
        ft800->CommandAddress >> 16
    };
    
    FT800Write(ft800, FT800Register_CMD_WRITE, address, 3);
    
    uint8_t addressBuf[4];
    uint32_t currentAddress = 0;

    while(currentAddress != ft800->CommandAddress)
    {
        FT800Read(ft800, FT800Register_CMD_READ, addressBuf, 4);
        
        currentAddress = addressBuf[0]
            | (addressBuf[1] << 8)
            | (addressBuf[2] << 16)
            | (addressBuf[3] << 24);
    }
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
    FT800Write(ft800, FT800_CMD_START + ft800->CommandAddress, buf, length);
    ft800->CommandAddress += length;
    ft800->CommandAddress %= 4096;
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

