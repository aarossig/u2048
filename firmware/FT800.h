/*
 * Driver for the FT800 Embedded Video Controller
 */

#ifndef FT800_H
#define FT800_H

#include <stdint.h>
#include <stdbool.h>

#include "SystemGpio.h"
#include "SystemSpi.h"

#define FT800_DL_CMD_LEN 4
#define FT800_CMD_START 0x108000
#define FT800_CHIPID 0x7C

#define FT800_SWIPE_X 300
#define FT800_SWIPE_Y 350

/* FT800 Instance *************************************************************/

typedef enum FT800PrimitiveType_t {
    FT800PrimitiveType_Bitmap = 0x01,
    FT800PrimitiveType_Points,
    FT800PrimitiveType_Lines,
    FT800PrimitiveType_LineStrip,
    FT800PrimitiveType_EdgeStripRight,
    FT800PrimitiveType_EdgeStripLeft,
    FT800PrimitiveType_EdgeStripTop,
    FT800PrimitiveType_EdgeStripBottom,
    FT800PrimitiveType_Rectangle
} FT800PrimitiveType_t;

typedef enum FT800Option_t {
    FT800Option_None    = 0,
    FT800Option_CenterX = 512,
    FT800Option_CenterY = 1024
} FT800Option_t;

typedef enum FT800Font_t {
    FT800Font_BoldAliased1 = 0x10,
    FT800Font_ExtAscii1,
    FT800Font_BoldAliased2,
    FT800Font_ExtAscii2,
    FT800Font_Aliased1,
    FT800Font_Aliased2,
    FT800Font_Aliased3,
    FT800Font_Aliased4,
    FT800Font_Aliased5,
    FT800Font_Aliased6,
    FT800Font_AntiAliased1,
    FT800Font_AntiAliased2,
    FT800Font_AntiAliased3,
    FT800Font_AntiAliased4,
    FT800Font_AntiAliased5,
    FT800Font_AntiAliased6
} FT800Font_t;

typedef enum FT800TouchState_t {
    FT800TouchState_NotTouched,
    FT800TouchState_Touched
} FT800TouchState_t;

typedef enum FT800Gesture_t {
    FT800Gesture_None,
    FT800Gesture_SwipeUp,
    FT800Gesture_SwipeDown,
    FT800Gesture_SwipeLeft,
    FT800Gesture_SwipeRight,
    FT800Gesture_Touch
} FT800Gesture_t;

typedef struct FT800Point_t {
    int16_t X;
    int16_t Y;
} FT800Point_t;

typedef struct FT800Color_t {
    uint8_t Red;
    uint8_t Green;
    uint8_t Blue;
} FT800Color_t;

typedef struct FT800TouchDetail_t {
    FT800TouchState_t State;
    uint16_t X;
    uint16_t Y;
} FT800TouchDetail_t;

typedef struct FT800GestureDetail_t {
    FT800Gesture_t Gesture;
    FT800Point_t Position;
} FT800GestureDetail_t;

typedef struct FT800_t {
    volatile SystemSpiModule_t *Spi;
    volatile SystemGpioModule_t *CsPort;
    uint32_t CsPin;
    uint32_t DisplayListAddress;
    uint32_t CommandAddress;
} FT800_t;

void FT800Init(FT800_t *ft800);

bool FT800ColorsEqual(const FT800Color_t c1, const FT800Color_t c2);

/* FT800 Registers ************************************************************/

typedef enum FT800Register_t {
    FT800Register_ID = 0x102400,
    FT800Register_FRAMES = 0x102404,
    FT800Register_CLOCK = 0x102408,
    FT800Register_FREQUENCY = 0x10240C,
    FT800Register_CPURESET = 0x10241C,
    FT800Register_GPIO_DIR = 0x10248C,
    FT800Register_GPIO = 0x102490,
    FT800Register_INT_FLAGS = 0x102498,
    FT800Register_INT_EN = 0x10249C,
    FT800Register_INT_MASK = 0x1024A0,
    FT800Register_PWM_HZ = 0x1024C0,
    FT800Register_PWM_DUTY = 0x1024C4,
    FT800Register_TRACKER = 0x109000,
    FT800Register_CMD_READ = 0x1024E4,
    FT800Register_CMD_WRITE = 0x1024E8,
    FT800Register_CMD_DL = 0x1024EC,
    FT800Register_PLAYBACK_START = 0x1024A4,
    FT800Register_PLAYBACK_LENGTH = 0x1024A8,
    FT800Register_PLAYBACK_READPTR = 0x1024AC,
    FT800Register_PLAYBACK_FREQ = 0x1024B0,
    FT800Register_PLAYBACK_FORMAT = 0x1024B4,
    FT800Register_PLAYBACK_LOOP = 0x1024B8,
    FT800Register_PLAYBACK_PLAY = 0x1024BC,
    FT800Register_VOL_PB = 0x10247C,
    FT800Register_VOL_SOUND = 0x102480,
    FT800Register_SOUND = 0x102484,
    FT800Register_REG_PLAY = 0x102488,
    FT800Register_TOUCH_MODE = 0x1024F0,
    FT800Register_TOUCH_ADC_MODE = 0x1024F4,
    FT800Register_TOUCH_CHARGE = 0x1024F8,
    FT800Register_TOUCH_SETTLE = 0x1024FC,
    FT800Register_TOUCH_OVERSAMPLE = 0x102500,
    FT800Register_TOUCH_RZTHRESH = 0x102504,
    FT800Register_TOUCH_RAW_XY = 0x102508,
    FT800Register_TOUCH_RZ = 0x10250C,
    FT800Register_TOUCH_SCREEN_XY = 0x102510,
    FT800Register_TOUCH_TAG_XY = 0x102514,
    FT800Register_TOUCH_TAG = 0x102518,
    FT800Register_TRANSFORM_A = 0x10251C,
    FT800Register_TRANSFORM_B = 0x102520,
    FT800Register_TRANSFORM_C = 0x102524,
    FT800Register_TRANSFORM_D = 0x102528,
    FT800Register_TRANSFORM_E = 0x10252C,
    FT800Register_TRANSFORM_F = 0x102530,
    FT800Register_TOUCH_DIRECT_XY = 0x102574,
    FT800Register_TOUCH_DIRECT_Z1Z1 = 0x102578,
    FT800Register_TAG_X = 0x102470,
    FT800Register_TAG_Y = 0x102474,
    FT800Register_TAG = 0x102478,
    FT800Register_DLSWAP = 0x102450,
    FT800Register_RENDERMODE = 0x102410,
    FT800Register_SNAPY = 0x102414,
    FT800Register_SNAPSHOT = 0x102418,
    FT800Register_TAP_CRC = 0x102420,
    FT800Register_TAP_MASK = 0x102424,
    FT800Register_HCYCLE = 0x102428,
    FT800Register_HOFFSET = 0x10242C,
    FT800Register_HSIZE = 0x102430,
    FT800Register_HSYNC0 = 0x102434,
    FT800Register_HSYNC1 = 0x102438,
    FT800Register_VCYCLE = 0x10243C,
    FT800Register_VOFFSET = 0x102440,
    FT800Register_VSIZE = 0x102444,
    FT800Register_VSYNC0 = 0x102448,
    FT800Register_VSYNC1 = 0x10244C,
    FT800Register_ROTATE = 0x102454,
    FT800Register_OUTBITS = 0x102458,
    FT800Register_DITHER = 0x10245C,
    FT800Register_SWIZZLE = 0x102460,
    FT800Register_CSPREAD = 0x102464,
    FT800Register_PCLK_POL = 0x102468,
    FT800Register_PCLK = 0x10246C
} FT800Register_t;

/* FT800 Register Abstractions ************************************************/

uint8_t FT800ReadChipId(FT800_t *ft800);
void FT800SwapDisplayList(FT800_t *ft800);

/* FT800 Abstracted Drawing ***************************************************/

void FT800DrawRectangle(FT800_t *ft800, FT800Point_t p1, FT800Point_t p2);

/* FT800 Touchscreen **********************************************************/

void FT800GetTouchGesture(FT800_t *ft800, FT800GestureDetail_t *detail);
void FT800ReadTouchState(FT800_t *ft800, FT800TouchDetail_t *detail);

/* FT800 Display List *********************************************************/

void FT800DlNew(FT800_t *ft800);
void FT800DlStartPrimitive(FT800_t *ft800, FT800PrimitiveType_t primitive);
void FT800DlEndPrimitive(FT800_t *ft800);
void FT800DlClearRgb(FT800_t *ft800, FT800Color_t color);
void FT800DlVertexI(FT800_t *ft800,
    uint16_t x, uint16_t y, uint8_t handle, uint8_t cell);
void FT800DlRgb(FT800_t *ft800, FT800Color_t color);
void FT800DlClearCSTBuffers(FT800_t *ft800, bool cBuf, bool sBuf, bool tBuf);
void FT800DlEnd(FT800_t *ft800);
void FT800DlSwap(FT800_t *ft800);

/* FT800 Coprocessor Commands *************************************************/

void FT800CmdNewDisplayList(FT800_t *ft800);
void FT800CmdSwapDisplayList(FT800_t *ft800);
void FT800CmdDrawText(FT800_t *ft800, FT800Point_t p, FT800Font_t font,
    FT800Option_t options, const char *str, uint32_t length);
void FT800CmdLogo(FT800_t *ft800);
void FT800CmdFlush(FT800_t *ft800);

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

void FT800CoprocessorCommand(FT800_t *ft800, uint8_t *buf, uint32_t length);

void FT800Read(FT800_t *ft800, uint32_t address,
    uint8_t *buf, uint32_t length);

void FT800Write(FT800_t *ft800, uint32_t address,
    uint8_t *buf, uint32_t length);

/* FT800 SPI ******************************************************************/

void FT800SpiWriteCs(FT800_t *ft800, uint8_t *buf, uint32_t length);
void FT800SpiWrite(FT800_t *ft800, uint8_t *buf, uint32_t length);
void FT800SpiWriteRead(FT800_t *ft800, uint8_t *buf, uint32_t length);

#endif
