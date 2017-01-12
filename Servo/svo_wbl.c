/****************************************************************************
*                (c) Copyright 2000 - 2008, SUNEXT DESIGN, INC.
*                            All Rights Reserved
*
*
*
* $Revision: 408 $
* $Date: 11/03/16 4:46p $
*
* DESCRIPTION
*   Contains code for the Wobble Block of the Blade chip.
*
**************************** SUNEXT CONFIDENTIAL *******************************/


//#include <stdio.h>
#include ".\common\globtype.h"
#include ".\common\globals.h"
#include ".\common\chip.h"
#include ".\common\error.h"
#include ".\common\macros.h"
#include ".\servo\svo.h"
#include ".\servo\Svo_com.h"
#include ".\servo\SvoVar.h"
#include ".\servo\Svo_seek.h"
#include ".\servo\svo_wbl.h"
#include ".\servo\isr.h"
#include ".\AL\coefs.h"
#include ".\AL\REG_HAL.h"
#include ".\AL\REG_global.h"
#include ".\AL\HAL_WOBBLE.h"
#include ".\hif\powerctl.h"
#include ".\player\plrdb.h"
#include ".\dvd\dvGvars.h"
#include ".\dvd\dvDef.h"
#include ".\AL\REG_HAL.h"
#include ".\al\REG_wobble.h"
#include ".\al\HAL_wobble.h"
#include ".\al\reg_intctrl.h"
#include ".\al\REG_servo.h"
#include ".\common\intctl.h"

extern BYTE Send_timef;
vBYTE PSN_display = 0;

#if(ENABLE_DVD_4XCAV == 1)

//                                         None, _2LX, _3LX, _4LX, _4AX, _6AX, _8AX,_10AX,_12AX,_16AX,_20AX
BYTE const cCurrSP2InxDVD[MAX_SPEED_DVD+1]={ 00,   01,   02,   02,   03,   03,   04,   04,   05,   06,   06};
#else
//                                         None, _2LX, _3LX, _4LX, _6AX, _8AX,_10AX,_12AX,_16AX,_20AX
BYTE const cCurrSP2InxDVD[MAX_SPEED_DVD+1]={ 00,   01,   02,   02,   03,   04,   04,   05,   06,   06};
#endif

extern int WtLPP_Dec_ok;

BYTE   AdipLppErrorCount; //Adj Fe2osa for DL
BYTE   WtAdipLppErrorCount;
//for monitor write adip error
BYTE WtECCAddressCount;
extern float AdipLppErrorRate;

#if (EN_WOBBLE == 1)
/*-----------------------------------------------------------------------------------------------------
    Name:           UpdateWobbleFilters

    Description:    Update wobble related filters, meant to be used at updates of the overspeed.

    Inputs:         OverspeedX10:   overspeed in tenths (X = 50 -> OverspeedX10 = 500)
    Outputs:        None
    Global Inputs:  None
    Global Outputs:
    Returns:        None
-----------------------------------------------------------------------------------------------------*/
void UpdateWobbleFilters(USHORT OverspeedX10)
{
    ULONG  PllInpFcN1Hz, AgcInpFcN1Hz;
    ULONG  LppFcN1Hz;
    ULONG  ClkKHz;
    USHORT WlpfRatioX10;
    USHORT WhpfRatioX100;
    halWBL_LimitPllT        LimitPLL;

#if (WOBBLE_READ_ON_DVD_OR_CD == 0) && (DVD_RAM_READ == 0)
    if (((svoVar_discKindT.Bit.Media == eMediaR)||(svoVar_discKindT.Bit.Media == eMediaRW)) && (svoVar_discKindT.Bit.DiscStd == eDiscBD))
#elif (WOBBLE_READ_ON_DVD_OR_CD == 0) && (DVD_RAM_READ == 1)
    if ((((svoVar_discKindT.Bit.Media == eMediaR)||(svoVar_discKindT.Bit.Media == eMediaRW)) && (svoVar_discKindT.Bit.DiscStd == eDiscBD)) || DVDRAM_rewritable)
#elif (DVD_RAM_READ == 0)
    if (svoVar_discKindT.Bit.Media != eMediaROM)
#else
    if ((svoVar_discKindT.Bit.Media != eMediaROM) && !(DVDRAM_embossed))
#endif
    {
        WlpfRatioX10 = COEF_GetValue(AFE_CALC_WlpfRatioX10);
        WhpfRatioX100 = COEF_GetValue(AFE_CALC_WhpfRatioX100);
        HAL_AFE_SetWobbleSpeed(OverspeedX10, WlpfRatioX10, WhpfRatioX100);

        ClkKHz = HAL_PLL_GetWblClkKHz();
        PllInpFcN1Hz = COEF_GetValue(WBL_CALC_PLLInpHighPassFcN1Hz);
        AgcInpFcN1Hz = COEF_GetValue(WBL_CALC_AGCInpHighPassFcN1Hz);
        LppFcN1Hz = COEF_GetValue(WBL_CALC_LPP_DRHighPassFcN1Hz);
        HAL_WOBBLE_SetHPFInp(PllInpFcN1Hz, AgcInpFcN1Hz, LppFcN1Hz, OverspeedX10, ClkKHz);
        PllInpFcN1Hz = COEF_GetValue(WBL_CALC_PLLInpLowPassFcN1Hz);
        AgcInpFcN1Hz = COEF_GetValue(WBL_CALC_AGCInpLowPassFcN1Hz);
        LppFcN1Hz = COEF_GetValue(WBL_CALC_LPP_DRLowPassFcN1Hz);
        HAL_WOBBLE_SetLPFInp(PllInpFcN1Hz, AgcInpFcN1Hz, LppFcN1Hz, OverspeedX10, ClkKHz);

        LimitPLL.Upper = COEF_GetValue(WBL_CALC_coef_Upper_limit_PLL);
        LimitPLL.Lower = COEF_GetValue(WBL_CALC_coef_Lower_limit_PLL);
        HAL_WOBBLE_SetWobbleFreqLimit(OverspeedX10, ClkKHz, &LimitPLL);
    }
    else
    {
        SendMsg80(DEBUG_WOBBLE_FLOW_MSG,0x3A0137); // UpdateWobbleFilters skip
    }
}

/*-----------------------------------------------------------------------------
 *
 * Module: IDValidIntProc
 * Description:  Interrupt handler ( ISR ) for DVD Wobble Block
                 This ISR is executed if any of the interrupt status bits
                 in register WINT1 are active and if the corresponding bits
                 in register WINTEN1 are enabled.

 *----------------------------------------------------------------------------*/
void IDValidIntProc(void)
{

    BYTE    IDDATA[16];
    ULONG   WobbleBlockAddress;
    BYTE    ADIPLPPError;
    BOOL    pregroove_data_ready = FALSE;
    ULONG   status;

    pdbSetIDIntStatus(TRUE);
    status = READ_REG(HAL_WBLREG_MAP->WBADBUFS);
    pdbSetIDOk(FALSE);

    // for OPC
    //send_msg5S(DEBUG_WBL_MSG, "IDValidIntProc");

    if(svoVar_discKindT.Bit.WblType == eWblMinus)
    {
        WRITE_FIELD(HAL_WBL_LPERCOON,TRUE);
        /*
        send_msg5S(DEBUG_WBL_MSG, "WBADBUFS");
        send_msg5L(SHOW_DEBUG_MSG, status);
        send_msg5S(DEBUG_WBL_MSG, "LPFLYADR");
        send_msg5L(SHOW_DEBUG_MSG, READ_REG(HAL_WBLREG_MAP->LPFLYADR));
        send_msg5S(DEBUG_WBL_MSG, "ADRFLYS");
        send_msg5L(SHOW_DEBUG_MSG, READ_REG(HAL_WBLREG_MAP->ADRFLYS));
        send_msg5S(DEBUG_WBL_MSG, "LPFLYS");
        send_msg5L(SHOW_DEBUG_MSG, READ_REG(HAL_WBLREG_MAP->LPFLYS));
        send_msg5S(DEBUG_WBL_MSG, "LPFLYCFG2");
        send_msg5L(SHOW_DEBUG_MSG, READ_REG(HAL_WBLREG_MAP->LPFLYCFG2));
        */

        if (((status & HAL_WBL_WBADBFRDY) == HAL_WBL_WBADBFRDY) &&
              READ_FIELD(HAL_WBL_LPWDLCK) && HAL_WOBBLE_GetSyncFlyWheelInLock())
              /* temporary LPWDLCK because flyaddresslock LPADLCK indication not working */
        {
            SendMsg80(DEBUG_WBL_MSG,0x3FAA22);   // FLYADR_OK
            WobbleBlockAddress = HAL_WOBBLE_GetWblAdd(IDDATA, status);
            if((WobbleBlockAddress>0x01E800) && (WobbleBlockAddress<0x26F000)) //spd test
            {
                pdbSetIDOk(TRUE);
                pdbSetWblAddress(WobbleBlockAddress);
            }
            else
            {
                ADIPLPPError=1;
                (void) READ_REG(HAL_WBLREG_MAP->ADBFRDEND); //release the channel buffer
            }
            if (!((status & HAL_WBL_WBLUNCORB) == HAL_WBL_WBLUNCORB) && (!((status & HAL_WBL_WBLUNCOR) == HAL_WBL_WBLUNCOR)))
            {
                SendMsg80(DEBUG_WBL_MSG,0x3FAA21);   // DATA_NOK1
            }
            else
            {
                ADIPLPPError = 1;
                SendMsg80(DEBUG_WBL_MSG,0x3FAA20);   // DATA_NOK2
            }
        }
        else
        {
            SendMsg80(DEBUG_WBL_MSG,0x3FAA1F);   // FLYADR_NOK
            ADIPLPPError=1;
            (void) READ_REG(HAL_WBLREG_MAP->ADBFRDEND); //release the channel buffer
        }
        Iram_ECCBlockCount++;
        if(ADIPLPPError)
        {
            Iram_AtipAdipLppErrorCount++;
            AdipLppErrorCount++;
        }
        else
        {
            Iram_AtipAdipLppErrorCount--;
            AdipLppErrorCount--;
        }
        if (Iram_ECCBlockCount == DecECCBlockAmount)
        {
            Iram_AtipAdipLppErrorAmount = Iram_AtipAdipLppErrorCount;
            Iram_AtipAdipLppErrorCount = 0;
            Iram_ECCBlockCount =0;
        }
    }
    else
    {
        IDDATA[0]=IDDATA[1]=IDDATA[2]=IDDATA[3]=IDDATA[4]=0;
        if (!((status & HAL_WBL_WBADBFRDY) == HAL_WBL_WBADBFRDY)&&
              (svoVar_discKindT.Bit.DiscStd == eDiscDVD)&&HAL_WOBBLE_GetSyncFlyWheelInLock())
        {
            pregroove_data_ready = TRUE;
            HAL_WOBBLE_SetValidFlyWheelAddress(TRUE);
        }
        else if (((status & HAL_WBL_WBADBFRDY) == HAL_WBL_WBADBFRDY) &&
            ((svoVar_discKindT.Bit.DiscStd == eDiscBD)?READ_FIELD(HAL_WBL_BDADRLCK):
                ((svoVar_discKindT.Bit.DiscStd == eDiscHD)?READ_FIELD(HAL_WBL_HDWAPADRL):1))
            && HAL_WOBBLE_GetSyncFlyWheelInLock())
        {
            pregroove_data_ready = TRUE;
        }
        else
        {
            pregroove_data_ready = FALSE;
        }

        if(pregroove_data_ready)
        {
//            send_msg5S(DEBUG_WBL_MSG, "PRE_OK");
            if((((svoVar_discKindT.Bit.DiscStd == eDiscDVD) || (svoVar_discKindT.Bit.DiscStd == eDiscBD)) && !(status&HAL_WBL_WBLUNCOR)) ||
                 ((svoVar_discKindT.Bit.DiscStd == eDiscHD) && (status&HAL_WBL_ERRFREE)))
            {
//                send_msg5S(DEBUG_WBL_MSG, "GetWblAdd");
                WobbleBlockAddress = HAL_WOBBLE_GetWblAdd(IDDATA, status);
                pdbSetIDOk(TRUE);
                pdbSetWblAddress(WobbleBlockAddress);
            }
            else
            {
                SendMsg80(DEBUG_WBL_MSG,0x3FAA1E);   // PRE_NOK
                ADIPLPPError = 1;
                (void) READ_REG(HAL_WBLREG_MAP->ADBFRDEND); //release the channel buffer
            }
        }
        else
        {
            ADIPLPPError = 1;
        }

        if (svoVar_discKindT.Bit.DiscStd == eDiscBD)
        {
            Iram_ECCBlockCount++;

            if (ADIPLPPError == 1)
            {
                Iram_AtipAdipLppErrorCount++;
            }

            if (Iram_ECCBlockCount == 3)
            {
                Iram_AtipAdipLppErrorAmount = Iram_AtipAdipLppErrorCount;
                Iram_AtipAdipLppErrorCount = 0;
                Iram_ECCBlockCount =0;
            }
        }
    }
}

/*-----------------------------------------------------------------------------
 *
 * Module: IDSyncLostIntProc
 * Description:  Interrupt handler ( ISR ) for Word Sync Flywheel Out of Lock
                 This ISR is executed if any of the interrupt status bits
                 in register WBLINT are active and if the corresponding bits
                 in register WBLINT are enabled.

 *----------------------------------------------------------------------------*/
void IDSyncLostIntProc(void)
{
    #if (RESYNC_MESSAGES == 1)
    SendMsgCnB(SHOW_DEBUG_MSG,4,0x33FF24,B1B(READ_FIELD(HAL_WBL_BDADRCFD)),B1B(READ_FIELD(HAL_WBL_BDWDCFD)),B1B(READ_FIELD(HAL_WBL_BDBTCFD)),B1B(READ_FIELD(HAL_WBL_PLLLCKS)));
    #else
    SendMsg80(DEBUG_DVD_WRITE_MSG,0x33FF47); //Word Sync Flywheel Out of Lock Interrupt
    #endif
}
#endif // (EN_WOBBLE == 1)

/*------------------------------------------------------------------------*/
/* The end of file                                                        */
/*------------------------------------------------------------------------*/
