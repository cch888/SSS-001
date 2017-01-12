/****************************************************************************
*                (c) Copyright 2001-2004 SUNEXT DESIGN, INC.
*                            All Rights Reserved
*
* FILE NAME:   powerctl.h
*
* DESCRIPTION
*
*
*
*
*************************** SUNEXT CONFIDENTIAL ********************************/

#ifndef __POWERCTL_H__
#define __POWERCTL_H__

#include ".\oem\oem_info.h"
#include ".\AL\REG_HAL.h"
#include ".\AL\REG_global.h"

#if EN_POWER_CONTROL
typedef enum PowerModes
{
    eChipResetPower = 0,    /* Power On/Reset Chip Default settings. Default = everything is ON */
    eStandbyPower,          /* Motor is not spining. Lowest Power, but able to respond to commands */
    eCDReadPower,           /* CD Read Power Settings */
#if (WOBBLE_READ_ON_DVD_OR_CD == 1)
    eCDReadWblPower,        /* CD Read with Wobble Power Settings */
#endif
    eDVDReadPower,          /* DVD Read Power Settings */
#if (WOBBLE_READ_ON_DVD_OR_CD == 1)
    eDVDReadWblPower,       /* DVD Read with Wobble Power Settings */
#endif
    ePlayAudioPower,        /* CD Play Audio Power Settings */
    eIdlePower,             /* Idle, no Command Power Settings */
    eSleepPower,            /* Sleep Command Power Settings. Lowest Power, requires reset to recover */
    eBDReadPower,           /* BD Read Power Settings */
    eBDReadWblPower,        /* BD Read with Wobble Power Settings */
#if (ENABLE_HDDVD == 1)
    eHDReadPower,           /* HD Read Power Settings */
    eHDReadWblPower,        /* HD Read with Wobble Power Settings */
#endif    
    eLSWritePower,          /* Light Scribe Power Settings */
    eStartUpPower,          /* Servo start up Power Settings */
}   PowerModesT;

#define WobbleCLKStop (READ_FIELD(HAL_GLOBAL_WBLACKSTP) || READ_FIELD(HAL_GLOBAL_WBLCKSTP) \
        || READ_FIELD(HAL_GLOBAL_WBLICKSTP) || READ_FIELD(HAL_GLOBAL_WBLLCKSTP))

#define BDDecCLKStop (READ_FIELD(HAL_GLOBAL_BDCCKSTP) || READ_FIELD(HAL_GLOBAL_BDCKSTP) \
        ||  READ_FIELD(HAL_GLOBAL_BDDCKSTP) )

extern BOOL PowerControl(BYTE);
extern void SetDiscReadPowerMode(void);
extern BYTE CurrentPowerMode;
#if (AFE_POWER_DOWN == 1)
extern void    AFEPowerOn(void);
extern void    AFEPowerDown(void);
#endif /* AFE_POWER_DOWN */

extern void SetSdramRefreshInterval(void);

#endif // EN_POWER_CONTROL


#endif
