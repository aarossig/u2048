/*
 * STM32 Real-time Clock Driver
 */

#ifndef SYSTEMWALLCLOCK_H
#define SYSTEMWALLCLOCK_H

#include <stdint.h>

#define AddrRtc 0x40002800

#define SystemRtc (*((volatile SystemWallClockModule_t *)AddrRtc))

/* Time and Date **************************************************************/

typedef enum SystemWallClockAmPm_t {
    SystemWallClockAmPm_Am,
    SystemWallClockAmPm_Pm
} SystemWallClockAmPm_t;

typedef union SystemWallClockTimeDate_t {
    struct {
        // Time
        uint32_t SecondUnits       : 4;
        uint32_t SecondTens        : 3;
        unsigned /* Pad */         : 1;
        uint32_t MinuteUnits       : 4;
        uint32_t MinuteTens        : 3;
        unsigned /* Pad */         : 1;
        uint32_t HourUnits         : 4;
        uint32_t HourTens          : 2;
        SystemWallClockAmPm_t AmPm : 1;
        unsigned /* Pad */         : 9;
        
        // Date
        uint32_t DayUnits          : 4;
        uint32_t DayTens           : 2;
        unsigned /* Pad */         : 2;
        uint32_t MonthUnits        : 4;
        uint32_t MonthTens         : 1;
        uint32_t WeekDay           : 3;
        uint32_t YearUnits         : 4;
        uint32_t YearTens          : 4;
        unsigned /* Pad */         : 8;
    };

    uint32_t Time;
    uint32_t Date;
} SystemWallClockTimeDate_t;

/* Control ********************************************************************/

typedef enum SystemWallClockWakeSource_t {
    SystemWallClockWakeSource_Rtc16,
    SystemWallClockWakeSource_Rtc8,
    SystemWallClockWakeSource_Rtc4,
    SystemWallClockWakeSource_Rtc2,
    SystemWallClockWakeSource_Spare1,
    SystemWallClockWakeSource_Spare2 = 0x06
} SystemWallClockWakeSource_t;

typedef enum SystemWallClockEventEdge_t {
    SystemWallClockEventEdge_Rising,
    SystemWallClockEventEdge_Falling
} SystemWallClockEventEdge_t;

typedef enum SystemWallClockFormat_t {
    SystemWallClockFormat_24,
    SystemWallClockFormat_AmPm
} SystemWallClockFormat_t;

typedef enum SystemWallClockCalOutput_t {
    SystemWallClockCalOutput_512,
    SystemWallClockCalOutput_1
} SystemWallClockCalOutput_t;

typedef enum SystemWallClockOutPol_t {
    SystemWallClockOutPol_High,
    SystemWallClockOutPol_Low
} SystemWallClockOutPol_t;

typedef enum SystemWallClockOut_t {
    SystemWallClockOut_Disabled,
    SystemWallClockOut_AlarmA,
    SystemWallClockOut_AlarmB,
    SystemWallClockOut_Wakeup
} SystemWallClockOut_t;

typedef union SystemWallClockControl_t {
    struct {
        SystemWallClockWakeSource_t WakeSource                : 3;
        SystemWallClockEventEdge_t  EventEdge                 : 1;
        bool                        RefClockDetectionEnabled  : 1;
        bool                        BypassShadowRegisters     : 1;
        SystemWallClockFormat_t     Format                    : 1;
        bool                        DigitalCalEnabled         : 1;
        bool                        AlarmAEnabled             : 1;
        bool                        AlarmBEnabled             : 1;
        bool                        WakeupEnabled             : 1;
        bool                        TimestampEnable           : 1;
        bool                        AlarmAInterruptEnabled    : 1;
        bool                        AlarmBInterruptEnabled    : 1;
        bool                        WakeupInterruptEnabled    : 1;
        bool                        TimestampInterruptEnabled : 1;
        bool                        AddOneHour                : 1;
        bool                        SubtractOneHour           : 1;
        bool                        Backup                    : 1;
        unsigned                    /* Pad */                 : 1;
        SystemWallClockCalOutput_t  CalOutput                 : 1;
        SystemWallClockOutPol_t     OutputPolarity            : 1;
        SystemWallClockOut_t        Output                    : 2;
        unsigned                    /* Pad */                 : 8;
    };

    uint32_t Control;
} SystemWallClockControl_t;

/* Initialization/Status ******************************************************/

typedef union SystemWallClockInit_t {
    struct {
        bool     AlarmAWrite           : 1;
        bool     AlarmBWrite           : 1;
        bool     WakeupWrite           : 1;
        bool     ShiftOperationPending : 1;
        bool     CalendarInitialized   : 1;
        bool     CalendarSynchronized  : 1;
        bool     CalendarUpdateAllowed : 1;
        bool     InitializationMode    : 1;
        bool     AlarmAFlag            : 1;
        bool     AlarmBFlag            : 1;
        bool     WakeupFlag            : 1;
        bool     TimestampFlag         : 1;
        bool     TimestampOverflowFlag : 1;
        bool     TimestampTamperFlag   : 1;
        unsigned /* Pad */             : 2;
        bool     RecalibrationPending  : 1;
        unsigned /* Pad */             : 15;
    };

    uint32_t Init;
} SystemWallClockInit_t;

/* Prescaler ******************************************************************/

typedef union SystemWallClockPrescaler_t {
    struct {
        uint32_t SynchronousPrescaler  : 15;
        unsigned /* Pad */             : 1;
        uint32_t AsynchronousPrescaler : 7;
        unsigned /* Pad */             : 9;
    };

    uint32_t Prescaler;
} SystemWallClockPrescaler_t;

/* Calibration ****************************************************************/

typedef union SystemWallClockCalibration_t {
    struct {
        uint32_t Calibration : 5;
        unsigned /* Pad */   : 2;
        bool     Sign        : 1;
        unsigned /* Pad */   : 24;
    };

    uint32_t Cal;
} SystemWallClockCalibration_t;

/* Real-time Clock Module *****************************************************/

typedef struct SystemWallClockModule_t {
    SystemWallClockTimeDate_t    TimeDate;
    SystemWallClockControl_t     Control;
    SystemWallClockInit_t        Initialization;
    SystemWallClockPrescaler_t   Prescaler;
    SystemWallClockCalibration_t Calibration;
    uint32_t                     AlarmA;
    uint32_t                     AlarmB;
    uint32_t                     Protection;
    uint32_t                     Subsecond;
    uint32_t                     ShiftSecond;
    uint32_t                     TimestampTime;
    uint32_t                     TimestampDate;
    uint32_t                     TimestampSubsecond;
    uint32_t                     RtcCalibration;
    uint32_t                     Tamper;
    uint32_t                     AlarmASubsecond;
    uint32_t                     AlarmBSubsecond;
    uint32_t                     BackupRegisters;
} SystemWallClockModule_t;

#endif
