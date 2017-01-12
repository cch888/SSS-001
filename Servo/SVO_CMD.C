/****************************************************************************
*                (c) Copyright 2001 - 2008 SUNEXT DESIGN, INC.
*                           All Rights Reserved
*
* FILENAME: SVO_CMD.C
*
* $Revision: 563 $
* $Date: 11/03/16 4:46p $
*
* DESCRIPTION
*
*
**************************** SUNEXT CONFIDENTIAL ****************************/
#include ".\common\globals.h"
#include ".\common\globtype.h"
#include ".\common\chip.h"
#include ".\common\stdlib.h"
#include ".\common\error.h"
#include ".\common\commath.h"
#include ".\common\com_im.h"
#include ".\servo\svo.h"
#include ".\servo\svoMacro.h"
#include ".\servo\svo_com.h"
#include ".\servo\svomath.h"
#include ".\servo\svo_cmd.h"
#include ".\servo\svo_lib.h"
#include ".\servo\svo_speed.h"
#include ".\servo\svo_afe.h"
#include ".\servo\svoVar.h"
#include ".\player\plrDB.h"
#include ".\player\plrServo.h"
#include ".\hif\gp_ram.h"
#include ".\oem\Oem_spd_tbl.h"
#include ".\common\vu_cfg.h"
#include ".\servo\svo_seek.h"

#if (DVD_RAM_READ == 1)
#include ".\defmgmt\dvdram\dvDefMgr.h"
#include ".\defmgmt\common\mrw_ram.h"
#endif

#if CMD_SVO_CMD_READ_GPADC == 1
#include <limits.h>
#endif

#include ".\AL\REG_HAL.h"
#include ".\al\REG_dfe.h"
#include ".\al\reg_intctrl.h"
#include ".\al\REG_wobble.h"
#include ".\al\hal_afe.h"
#include ".\al\hal_dfe.h"
#include ".\al\hal_pll.h"
#include ".\al\LAL_dfe.h"
#include ".\al\hal_wobble.h"
#include ".\al\lal.h"
#include ".\al\LAL_dfe.h"
#include ".\AL\coefs.h"

#if (BD_ENABLE == 1)
#include ".\bd\BdVars.h"
#endif

#define svoStEND        0xFF
#define svoStNOP        0xFE

const ServoStatesT  state_chk_nop[]={
    svoStNOP            ,
    svoStEND
};

const ServoStatesT  state_chk_tray[]={
    svoStInitialized    ,
    svoStIdleDiscKnown  ,
    svoStIdleDiscUnknown,
    svoStStoppingServo  ,
    svoStEmergencystop  ,
    svoStEND
};

const ServoStatesT  state_chk_start[]={
    svoStInitialized    ,
    svoStIdleDiscKnown  ,
    svoStIdleDiscUnknown,
    svoStStoppingServo  ,
    svoStEmergencystop  ,
    svoStEND
};

const ServoStatesT  state_chk_stop[]={
    svoStInitialized    ,
    svoStStartingServo  ,
    svoStTracking       ,
    svoStAccessing      ,
    svoStPausing        ,
    svoStEmergencystop  ,
    svoStEND
};

const ServoStatesT  state_chk_pause[]={
    svoStTracking       ,
    svoStPausing        ,
    svoStAccessing      ,
    svoStEmergencystop  ,
    svoStEND
};

const ServoStatesT  state_chk_seek[]={
    svoStTracking       ,
    svoStPausing        ,
    svoStAccessing      ,
    svoStEmergencystop  ,
    svoStEND
};

const ServoStatesT  state_chk_jump[]={
    svoStTracking       ,
    svoStPausing        ,
    svoStAccessing      ,
    svoStEmergencystop  ,
    svoStEND
};

#if (SERVICE_MODE == 1)
const ServoStatesT  state_chk_service[]={
    svoStServiceMode    ,
    svoStEND
};
#endif

#if (EN_LS == 1)
const ServoStatesT  state_chk_LSseek[]={
    svoStTracking       ,
    svoStPausing        ,
    svoStAccessing      ,
    svoStEmergencystop  ,
    svoStEND
};
#endif // End of (EN_LS == 1)

#define Accepted        0
#define Range           1

struct svo_cmd{
    ServoCommandsT  cmd;
    void            (*func)(void);
    BYTE            busy_flag;
    ServoStatesT    *state_tbl;
};

#if (SERVICE_MODE==1)
const struct svo_cmd svoServiceCmdProc[]={
    {svoCmdLoopControl             ,svoCmdLoopControlProc          ,ON ,(ServoStatesT *) state_chk_service},/* 0x0B */
    {svoCmdGetStatusInformation    ,svoCmdGetStatusInformationProc ,OFF,(ServoStatesT *) state_chk_nop    },/* 0x23 */
    {svoCmdGetErrorInformation     ,svoCmdGetErrorInformationProc  ,OFF,(ServoStatesT *) state_chk_nop    },/* 0x24 */
    {svoCmdServiceMode             ,svoCmdServiceModeProc          ,OFF,(ServoStatesT *) state_chk_service},/* 0x40 */
    #if CMD_DRY_READ == 1
    {svoCmdDryRead                 ,svoCmdDryReadProc              ,OFF,(ServoStatesT *) state_chk_service},/* 0x0D */
    #endif
    #if (TEST_AVERAGING_SERVO == 1) && (AFE_NORMALIZER == 1)
    {svoCmdTestAvgServo            ,svoCmdTestAvgServoProc         ,OFF,(ServoStatesT *) state_chk_service},/* 0x0F */
    #endif
    #if CMD_SVO_CMD_READ_GPADC == 1
    {svoCmdReadGpAdc               ,svoCmdReadGpAdcProc            ,OFF,(ServoStatesT *) state_chk_service},/* 0x0E */
    #endif
    {(ServoCommandsT)NOP,           NULL                           ,OFF,(ServoStatesT *) state_chk_nop    }
};
#endif

const struct svo_cmd svoCmdProc[]={
    {svoCmdInit                    ,svoPowerOnInit                 ,ON ,(ServoStatesT *) state_chk_nop    },/* 0x00 */
    {svoCmdTray                    ,svoCmdDiscOutProc              ,ON ,(ServoStatesT *) state_chk_tray   },/* 0x01 */
    {svoCmdStart                   ,svoCmdStartProc                ,ON ,(ServoStatesT *) state_chk_start  },/* 0x02 */
    {svoCmdStop                    ,svoCmdStopProc                 ,ON ,(ServoStatesT *) state_chk_nop    },/* 0x03 */
    {svoCmdPause                   ,svoCmdPauseProc                ,ON ,(ServoStatesT *) state_chk_pause  },/* 0x04 */
    {svoCmdSeek                    ,svoCmdSeekProc                 ,ON ,(ServoStatesT *) state_chk_seek   },/* 0x05 */
    {svoCmdJumpGrooves             ,svoCmdJumpGroovesProc          ,ON ,(ServoStatesT *) state_chk_jump   },/* 0x06 */
    {svoCmdVeloAdjust              ,svoCmdVeloAdjustProc           ,ON ,(ServoStatesT *) state_chk_nop    },/* 0x09 */
    {svoCmdReAdjust                ,svoCmdReAdjustProc             ,ON ,(ServoStatesT *) state_chk_nop    },/* 0x0A */
#if (CALIBRATION_MANAGEMENT == 1)
    {svoCmdReCalibrate             ,svoCmdReCalibrateProc          ,ON ,(ServoStatesT *) state_chk_seek   },/* 0x0B */
#endif
#if (HORROR_DISC_IMPROVEMENT == 1)
    {svoCmdChgRFParameter          ,svoCmdChgRFParameterProc       ,ON ,(ServoStatesT *) state_chk_nop    },/* 0x0C */
#endif
#if ( FORCE_DISK_KIND==1)
    {svoCmdForceDiscKind           ,svoCmdForceDiscKindProc        ,ON ,(ServoStatesT *) state_chk_nop    },/* 0x0E */
#endif
#if ((PCB == 690)||(PCB == 692)||(PCB == 710)||(PCB == 711)||(PCB == 812)||(PCB == 830)||(PCB == 1600)||(PCB == 815)||(PCB == 817))
    {svoCmdAdjFocActDcActSens      ,svoCmdAdjFocActDcActSensProc   ,ON ,(ServoStatesT *) state_chk_nop    },/* 0x0F */
#endif
    {svoCmdSetSpeed                ,svoCmdSetSpeedProc             ,OFF,(ServoStatesT *) state_chk_nop    },/* 0x11 */
#if (BCA_ENABLE == 1)
    {svoCmdBCASeek                 ,svoCmdBCASeekProc              ,ON ,(ServoStatesT *) state_chk_seek   },/* 0x20 */
#endif
    {svoCmdGetStatusInformation    ,svoCmdGetStatusInformationProc ,OFF,(ServoStatesT *) state_chk_nop    },/* 0x23 */
    {svoCmdGetErrorInformation     ,svoCmdGetErrorInformationProc  ,OFF,(ServoStatesT *) state_chk_nop    },/* 0x24 */

    {svoCmdLDAdjust                ,svoCmdLDAdjustProc             ,ON ,(ServoStatesT *) state_chk_nop    },/* 0x31 */
    {svoCmdDiagnostic              ,svoCmdDiagnosticProc           ,ON ,(ServoStatesT *) state_chk_nop    },/* 0x32 */
#if (SERVICE_MODE == 1)
    {svoCmdServiceMode             ,svoCmdServiceModeProc          ,OFF,(ServoStatesT *) state_chk_nop    },/* 0x40 */
#endif
#if (EN_LS == 1)
    {svoCmdSeek                    ,svoCmdLSSeekProc               ,ON ,(ServoStatesT *) state_chk_LSseek },/* 0x05 */
#endif // End of (EN_LS == 1)
    {(ServoCommandsT)NOP,           NULL                           ,OFF,(ServoStatesT *) state_chk_nop    }
}; /* svoCmdProc[] */

#if (CD_DFE_IMPROVE_READ == 1)
extern  BYTE    DFE_CHG_CASE;
#endif

/*-----------------------------------------------------------------------------------------------------
    Name:           SetReadFilters

    Description:    Set Read filters.

    Inputs:         None
    Outputs:        None
    Global Inputs:  svoSpeedVar.SpdOverspd
    Global Outputs:
    Returns:        None
-----------------------------------------------------------------------------------------------------*/
void SetReadFilters(void)
{
    USHORT OverspeedX10;

    svoSpeedVar.SpdPresetNeeded = FALSE;
    if (svoSpeedVar.SpdOverspd > 0)
    {
        OverspeedX10 = (svoSpeedVar.SpdOverspd + 5) / 10;

        /* Enable the AGC and the AOC */
        if (READ_FIELD(HAL_AFE_RFAGCEN))
        {
            HAL_DFE_EnDisAgcAoc(halDFE_AgcAocEnable);
        }

        /* Set the RF LPF
         * Set the DPD Bw
         * Set the High Pass filter and moving average filter
         */
        if (svo_bReadZoneSpeedX10 != OverspeedX10)
        {
            UpdateReadFilters(OverspeedX10);
        }

        /* Set the equalizer and the non-linear equalizer for normal conditions
         * also set the clip level of the non-linear equalizer  */
#if (DVD_RAM_READ == 1)
        if (DVDRAM_embossed)
        {
            LAL_DFE_SetEqualizer(lalDFE_EqualizerDVDRAMEmbossed);
            LAL_DFE_SetNLE(lalDFE_EqualizerDVDRAMEmbossed);
            LAL_DFE_SetClipNLE(lalDFE_EqualizerDVDRAMEmbossed);
        }
        else
#endif
        {
#ifdef RETRY_CHG_EQTAPA12
            if ((svoVar_discKindT.fDiscKind == eBDREDisc) && (retry_DFE_EqualizerStrength != lalDFE_EqualizerStrengthNormal))
            {
                LAL_DFE_SetEqualizer(retry_DFE_EqualizerStrength);
            }
            else
            {
                LAL_DFE_SetEqualizer(lalDFE_EqualizerStrengthNormal);
            }
#else
            LAL_DFE_SetEqualizer(lalDFE_EqualizerStrengthNormal);
#endif
            LAL_DFE_SetNLE(lalDFE_EqualizerStrengthNormal);
            LAL_DFE_SetClipNLE(lalDFE_EqualizerStrengthNormal);
        }

        /* Enable the data slicer */
        HAL_DFE_EnDisDataSlicer(halDFE_DataSlicerEnable);

        /* Set the Slicer BandWidth */
        LAL_DFE_SetBwDataSlicer();

        /* Preset the HF or wobble PLL */

#if (EN_WOBBLE == 0)
        HAL_DFE_PresetPLLFreq(OverspeedX10, HAL_PLL_GetDfeInClkKHz(), svoVar_TrackingMethod); /// --> no clipping value !!!!!!!!!
#else // (EN_WOBBLE == 0)
    #if (WOBBLE_READ_ON_DVD_OR_CD == 1)
        if ((svoVar_discKindT.Bit.Media == eMediaROM)
        #if (DVD_RAM_READ == 1)
            || (DVDRAM_embossed)
        #endif
    #else
        if ((((svoVar_discKindT.Bit.DiscStd != eDiscBD) || (svoVar_discKindT.Bit.Media == eMediaROM))
        #if (DVD_RAM_READ == 1)
            && (!DVDRAM_rewritable)
        #endif
            )
    #endif
    #if (BD_ENABLE == 1)
            || (svo_BDR_RE_PICReading == SET)
    #endif
        )
        {
            SendMsg80(DEBUG_CALIB_ENTRYS_MSG,0x3A0134); // PresetDFEPLLFreq in SetReadFilters
            HAL_DFE_PresetPLLFreq(OverspeedX10, HAL_PLL_GetDfeInClkKHz(), svoVar_TrackingMethod); /// --> no clipping value !!!!!!!!!
        }
        else
        {
            SendMsg80(DEBUG_CALIB_ENTRYS_MSG,0x3A0135); // SetWobbleFreq in SetReadFilters
            LAL_WOBBLE_SetWobbleFreq(OverspeedX10);
            if (svoVar_discKindT.Bit.Media == eMediaUnknown)
            {
                SendMsg80(DEBUG_CALIB_ENTRYS_MSG,0x3A0134); // PresetDFEPLLFreq in SetReadFilters
                HAL_DFE_PresetPLLFreq(OverspeedX10, HAL_PLL_GetDfeInClkKHz(), svoVar_TrackingMethod); /// --> no clipping value !!!!!!!!!
            }
        }
#endif // (EN_WOBBLE == 0)

        /* Set the average run length for ROM disc (no wobble) only and
         * put the PLL on Auto Lock Mode */
        LAL_DFE_SetHfPll(FALSE);

        /* Set the fast PLL and the clip level of the HF Fast PLL*/
        LAL_DFE_SetBwFastPll();
        LAL_DFE_SetClipLvlFastPll();

#if (EN_DVDROM_HIGH_JITTER_READ_METHOD == 1)
        if((DVDHightJitter&DVDROMSL_HIGH_JITTER) == DVDROMSL_HIGH_JITTER)
        {
            //send_msg5SValue(1, "HJchang", 2,OverspeedX10 );
            LAL_DFE_SetEqualizer(lalDFE_EqualizerStrengthHigh);
            LAL_DFE_SetNLE(lalDFE_EqualizerStrengthNormal);
            LAL_DFE_SetClipNLE(lalDFE_EqualizerStrengthHigh);
            WRITE_FIELD(HAL_DFE_FPLLKICFG,4);
            DVDHightJitter = 0;
        }
#endif
    }
#if (NEW_RFPOS_CALIB == 1)
    if ((svoVar_discKindT.Bit.DiscStd == eDiscDVD) && (!DVDRAM))
    {
        if (svoRRFVar.rfagc_ref > 300)//20101015_02PQ improve RF AGC AOC monitoring and remove searchNWA workaround
        {
            WRITE_FIELD(HAL_DFE_AOCINTBW, (COEF_GetValue(DFE_HFAOCCFG_AOCINTBW) - 1));
        }
        else if (READ_FIELD(HAL_DFE_AOCINTBW) != (COEF_GetValue(DFE_HFAOCCFG_AOCINTBW)))
        {
            LAL_WriteCoef(DFE_HFAOCCFG_AOCINTBW);
        }
    }
#endif
}


/*-----------------------------------------------------------------------------------------------------
    Name:           UpdateReadFilters

    Description:    Update read related filters, meant to be used at updates of the overspeed.

    Inputs:         OverspeedX10:   overspeed in tenths (X = 50 -> OverspeedX10 = 500)
    Outputs:        None
    Global Inputs:  None
    Global Outputs:
    Returns:        None
-----------------------------------------------------------------------------------------------------*/
void UpdateReadFilters(USHORT OverspeedX10)
{
    ULONG  DfeClkKHz;
    ULONG  InpFcN1Hz;

    if (OverspeedX10 != 0)
    {
        svo_bReadZoneSpeedX10 = OverspeedX10;
        /*  Set the RF LPF
         *  getting the BW ratio from the COEF table
         */
        HAL_AFE_SetRfBw(OverspeedX10, (BYTE) COEF_GetValue( AFE_CALC_BwRfRatioX100), svoVar_TrackingMethod);

        /*  Set the DPD Bw
         *  getting the BW ratio from the COEF table
         */
        if (svoVar_TrackingMethod == TRK_METHOD_DPD)
        {
            HAL_AFE_SetDPDBw(OverspeedX10, (BYTE) COEF_GetValue(AFE_CALC_BwDpdRatioX100));
        }

        /* Update the DFE IIR High Pass filter */
        DfeClkKHz = HAL_PLL_GetDfeInClkKHz();
#if (DVD_RAM_READ == 1)
        if (DVDRAM_embossed)
        {
            InpFcN1Hz = COEF_GetValue(DFE_CALC_HFHighpass_FcN1_EMBOSSED_PIC); /* Get DVD-RAM embossed area coef. */
        }
        else
#endif
        {
            InpFcN1Hz = COEF_GetValue(DFE_CALC_HFHighpass_FcN1); /* Get normal coef */
        }

#if (BD_ENABLE == 1)
        if (svo_BDR_RE_PICReading == SET)
        {
            InpFcN1Hz = COEF_GetValue(DFE_CALC_HFHighpass_FcN1_EMBOSSED_PIC); /* Overrule with PIC coef */
            HAL_DFE_IIRHPFByPass(halDFE_IirHpfNotBypass);
        }
    #if (PCB == 710)||(PCB == 711)||(PCB == 817)
        if (svoVar_discKindT.Bit.DiscStd == eDiscBD)
            HAL_DFE_SetBitDetCFG(OverspeedX10);
    #endif
#endif
        HAL_DFE_SetIIRHPFBw(InpFcN1Hz, OverspeedX10, DfeClkKHz);

        /* Update the DFE Moving Average High Pass filter */
#if (DVD_RAM_READ == 1)
        if (DVDRAM_embossed)
        {
            InpFcN1Hz = COEF_GetValue(DFE_CALC_Movavg_FcN1_EMBOSSED);
        }
        else
#endif
        {
            InpFcN1Hz = COEF_GetValue(DFE_CALC_Movavg_FcN1);
        }
        HAL_DFE_SetMovAvHPFBw(InpFcN1Hz, OverspeedX10, DfeClkKHz);

#if (DVD_RAM_READ == 1)
        if (DVDRAM_rewritable)
        {
            InpFcN1Hz = COEF_GetValue(DFE_CALC_Movavg_FcN1_HEADER);
            HAL_DFE_SetMovAvHPFBwHeader(InpFcN1Hz, OverspeedX10, DfeClkKHz);
        }
#endif

#if (EN_WOBBLE == 1)
        /* Update the WOBBLE Low and High Pass filters */
    #if (DVD_RAM_READ == 1)
        if (!DVDRAM_embossed)
    #endif
            UpdateWobbleFilters(OverspeedX10);
#endif

#if (CHIP_REV >= 0xC0)
        HAL_SERVO_UpdateTime(OverspeedX10);
#endif
#if (NEW_FDP_DETECTION == 2) || (ENABLE_LEARN_DEFECT == 1)
        LAL_DSP_UpdateTime(OverspeedX10);   //update Focus drop detction time & Learn Defect time
#endif
    }
}


/************************************************************************
* Module: ChkUpdateReadFilters()
* Description: call UpdateReadFilters() if overspeed changed
* Returns: None
* Params: None
*************************************************************************/
void ChkUpdateReadFilters(void)
{
    static USHORT SpdMonitorSpeed = 0;
    USHORT        OverspeedX10;
#if (EN_DVDROM_HIGH_JITTER_READ_METHOD == 1)
    USHORT i,tmpjitter;
    static BYTE jcounter = 10;
#endif

    OverspeedX10 = svoSpeedVar.SpdOverspd;
    if (OverspeedX10 > 0)
    {
        OverspeedX10 = (OverspeedX10 + 5) / 10;
        if (OverspeedX10 != SpdMonitorSpeed)
        {
            SpdMonitorSpeed = OverspeedX10;
#if (EN_DVDROM_HIGH_JITTER_READ_METHOD == 1)
            DVDHightJitter = 0;
            if (svoVar_discKindT.fDiscKind == eDVDROMDisc)
            {
                if (++jcounter >= 10)
                {
                    jcounter = 0;
                    tmpjitter = 0;
                    for (i = 0; i < JITTER_SAMPLE_CNT; i++)
                    {
                        tmpjitter += HAL_DFE_ReadJitterX10();
                    }
                    tmpjitter /= i;
                    //send_msgA2(1, 2, 3, 4, tmpjitter);
                    //send_msgA2(1, 2, 3, 5, OverspeedX10);
                    if (tmpjitter > DVDROM_HIGHJITTER_THLD)
                    {
                        DVDHightJitter |= DVDROMSL_HIGH_JITTER;
                    }
                    else
                    {
                        DVDHightJitter = 0;
                    }
                }
            }
#endif
            /* check for an (absolute) step change of speed of around 5% compared to initial speed */
            if (((ULONG)(OverspeedX10 * 100) >= (ULONG)(svo_bReadZoneSpeedX10 * 105)) ||
                ((ULONG)(OverspeedX10 * 100) <= (ULONG)(svo_bReadZoneSpeedX10 *  95)))
            {
                /* update the AFE RF LPF, the AFE DPD  Bandwidth and the DFE IIR HP filter and moving average filter */
                UpdateReadFilters(OverspeedX10);
            }
        }

#if (CD_DFE_IMPROVE_READ == 1)
        if (DFE_CHG_CASE & 0x01)
            LAL_DFE_SetBwFastPll();
#endif // (CD_DFE_IMPROVE_READ == 1)
    }
    else
    {
        SpdMonitorSpeed = 0;
    }
}

#if (SERVICE_MODE == 1)
/*-----------------------------------------------------------------------------
 *
 * Module: svoCmdDecode()
 * Description:
 *
 *----------------------------------------------------------------------------*/
void svoServiceCmdDecode(void)
{
    USHORT  i;
    USHORT  j;
    BYTE    cmd;

    cmd = get_SIF(COMMAND);

    if (cmd == svoCmdIdle)
    {
        return;
    }

    for(i=0;;i++)
    {
        if (svoServiceCmdProc[i].cmd == (ServoCommandsT)NOP)
        {
            svoCmdIllegalProc(Accepted);
            break;
        }
        if (svoServiceCmdProc[i].cmd == cmd)
        {
            if (svoServiceCmdProc[i].state_tbl[0] == svoStServiceMode)
            {
                if (svoServiceCmdProc[i].busy_flag == ON)
                {
                    SIF_CMD_MESSAGE(PRN_HIF_TO_SIF,0);
                    set_SIF(EXECUTIONSTATE,svoBusy);        /* set ExecutionState "svoBusy" */
                }
                else
                {
                    SIF_CMD_MESSAGE(PRN_HIF_TO_SIF,0);
                    set_SIF(EXECUTIONSTATE,svoReady);       /* set ExecutionState "svoReady" */
                }
                for(j = 0;j < 10;j++)/* parameter get */
                {
                    svoCmdVar.cmdParam[j] = get_SIF(PARAMETER1+j);
                }
                svoServiceCmdProc[i].func();
                break;
            }
            svoCmdIllegalProc(Accepted);
            break;
        }
    }
    set_SIF(COMMAND,svoCmdIdle);        /* command clr */
}
#endif // (SERVICE_MODE == 1)

/*-----------------------------------------------------------------------------
 *
 * Module: svoCmdDecode()
 * Description:
 *
 *----------------------------------------------------------------------------*/
void svoCmdDecode(void)
{
    USHORT  i;
    USHORT  j;
    BYTE    cmd;
#if Monitor_FPDO_Linearity == 1
    SHORT FPDIP,FPDIN,FPDO;
#endif  //Monitor_FPDO_Linearity == 1

    cmd = get_SIF(COMMAND);

    if (cmd == svoCmdIdle)
    {
#if Monitor_FPDO_Linearity == 1
        if ((bEnMonitorDac>0) && (ReadTimer(&SampleTimer)>100))
        {
            if (bEnMonitorDac==1)
            {
                SendMsgCn(SHOW_DEBUG_MSG,2,0x420022,A2B(VWDCDAC));

                //FPDO = DVDFPDIP - DVDFPDINVREF = S3 - S4 ; it would be negative .
                WRITE_FIELD(HAL_AFE_MUXADSEL, halAFE_MUXADSEL_MUXOUT3); //select MUXADSEL to MUXOUT3
                //DVDFPDIP (S3)
                WRITE_FIELD(HAL_AFE_APCTEST, halAFE_APCT_M1S4_M2S3); //S4 and S3
                AfeMUXOUT(AFE_MUXTOAD3, halAFE_MUX3ADSEL_APCMUX2);
                WaitTimer(15L);
                FPDIP = (SHORT)ReadAverageAdcmV(ADC_MUXOUT3, 8);
                FPDIP += (SHORT)ReadAverageAdcmV(ADC_MUXOUT3, 8);
                FPDIP /=2;
                SendMsgCn(SHOW_DEBUG_MSG,2,0x420023,A2B(FPDIP));

                //DVDFPDINVREF (S4)
                WRITE_FIELD(HAL_AFE_APCTEST, halAFE_APCT_M1S4_M2S3); //S4 and S3
                AfeMUXOUT(AFE_MUXTOAD3, halAFE_MUX3ADSEL_APCMUX1);
                WaitTimer(15L);
                FPDIN = (SHORT)ReadAverageAdcmV(ADC_MUXOUT3, 8);
                FPDIN += (SHORT)ReadAverageAdcmV(ADC_MUXOUT3, 8);
                FPDIN /=2;
                SendMsgCn(DEBUG_KPOWER_MSG,2,0x3350C0,A2B(FPDIP));

                FPDO=FPDIP-FPDIN;
                if (FPDO < 0)
                FPDO=-FPDO;
                SendMsgCn(SHOW_DEBUG_MSG,2,0x420024,A2B(FPDO));

                bEnMonitorDac = 2;
            }
            else
            {
                bEnMonitorDac = 1;
                VWDCDAC+=255;
                if (VWDCDAC>17000) // 0x0000 ~ 0x7FFF
                {
                    VWDCDAC=0;
                    bEnMonitorDac = 0;
                    WRITE_DSP_COEF(Delta_open_loop_value, 0);
                }
                WRITE_DSP_COEF(Delta_open_loop_value, VWDCDAC);
            }
            StartTimer(&SampleTimer);
        }
#endif //Monitor_FPDO_Linearity == 1
        return;
    }

    for(i=0;;i++){
        if (svoCmdProc[i].cmd == (ServoCommandsT)NOP)
        {
            svoCmdIllegalProc(Accepted);
            break;
        }
        if (svoCmdProc[i].cmd == cmd)
        {
            if (svoCmdStatecheck(svoCmdProc[i].state_tbl) == OK)
            {
                SendMsg80(STATE_MACHINE_FLOW_MSG,0x300100+cmd);
                if (svoCmdProc[i].busy_flag == ON)
                {
                    SIF_CMD_MESSAGE(PRN_HIF_TO_SIF,0);
                    set_SIF(EXECUTIONSTATE,svoBusy);        /* set ExecutionState "svoBusy" */
                }
                else
                {
                    SIF_CMD_MESSAGE(PRN_HIF_TO_SIF,0);
                    set_SIF(EXECUTIONSTATE,svoReady);       /* set ExecutionState "svoReady" */
                }

                for(j = 0;j < 10;j++)/* parameter get */
                {
                    svoCmdVar.cmdParam[j] = get_SIF(PARAMETER1+j);
                }
                SendMsgCn(STATE_MACHINE_FLOW_MSG,10,0x300010+SIF.ServoExecutionState,svoCmdVar.cmdParam[0],svoCmdVar.cmdParam[1],
                    svoCmdVar.cmdParam[2],svoCmdVar.cmdParam[3],svoCmdVar.cmdParam[4],
                    svoCmdVar.cmdParam[5],svoCmdVar.cmdParam[6],svoCmdVar.cmdParam[7],
                    svoCmdVar.cmdParam[8],svoCmdVar.cmdParam[9]);
                svoMiscVar.atipdisc_subq_diei_flag = FALSE;
                svoCmdProc[i].func();
                break;
            }
            svoCmdIllegalProc(Accepted);
            SendMsgCn(STATE_MACHINE_FLOW_MSG,1,0x300013,A1B(cmd));
            break;
        }
    }
    set_SIF(COMMAND,svoCmdIdle);        /* command clr */
} /* svoCmdDecode */


/*-----------------------------------------------------------------------------
 *
 * Module: svoCmdIllegalProc()
 * Description:
 *
 *----------------------------------------------------------------------------*/
void svoCmdIllegalProc(char type)
{
    set_SIF(EXECUTIONSTATE,svoError);       /* set ExecutionState "svoError" */
    if(type == Range){
        svoCmdVar.commandError = eErrorRangeCheck;
    }else{
        svoCmdVar.commandError = eErrorCommandNotAccepted;
    }
} /* svoCmdIllegalProc */

/*-----------------------------------------------------------------------------
 *
 * Module: svoCmdStatecheck()
 * Description:
 *
 *----------------------------------------------------------------------------*/
BYTE svoCmdStatecheck(ServoStatesT *p)
{
    BYTE    state;
    BYTE    i;

    state = get_SIF(STATE);
    for(i=0;;i++){
        if(p[i] == state){
            return(OK);
        }
        if(p[i] == svoStNOP){
            return(OK);
        }
        if(p[i] == svoStEND){
            return(NG);
        }
    }
} /* svoCmdStatecheck */


/*-----------------------------------------------------------------------------
 *
 * Module: svoCmdDiscOutProc()
 * Description:
 *
 *----------------------------------------------------------------------------*/
void svoCmdDiscOutProc(void)
{
    if(svoCmdVar.cmdParam[0] == eMoveTrayOut){
        svoCmdVar.svoCommand = COPEN;
#if (EN_LS == 1)
    svoVar_discKindT.Bit.DiscStd = eDiscUnknown;
    bLSDetected = FALSE; // disable ls detect
    bLSFOOROC = FALSE;
    opSPGS = 0;
    //applyFocusROC = FALSE;
    WRITE_FIELD(HAL_LS_SPOKE0EN, 0);
    WRITE_FIELD(HAL_LS_SPOKEEN, 0);

    spdDisableLS(); //need to create it on the new .c file; Henry; 2008/07/14
#endif // End of (EN_LS == 1)
        return;
    }
    if(svoCmdVar.cmdParam[0] == eMoveTrayIn ){
        svoCmdVar.svoCommand = CCLOSE;
        svo_Adj_Data_Clr();
        return;
    }
    set_SIF(EXECUTIONSTATE,svoReady);       /* set ExecutionState "svoReady" */
} /* svoCmdDiscOutProc */

/*-----------------------------------------------------------------------------
 *
 * Module: svoCmdStartProc()
 * Description:
 *
 *----------------------------------------------------------------------------*/
void svoCmdStartProc(void)
{
    discKindT tempDiscKind;
    ESpindleSpeed  tempSpeed;

    svoCmdVar.svoCommand = CSTART;

    /* Initialy load some coefs (required for VREF calib), maybe move to other location ? */
    if (!(svoCalVar.discChkDoneBits & start_end))
    {
#if ((BD_ENABLE == 1)&&((PCB == 1600)||(PCB == 710)||(PCB == 711)||(PCB == 812)||(PCB == 817))) //only for 1600 DiscKind test
        tempDiscKind.fDiscKind = eBDROMDisc;
        tempSpeed = SVO_CAL_SPEED_BD;
#else
        tempDiscKind.fDiscKind = eCDROMDisc;
        tempSpeed = SVO_CAL_SPEED_CD;
#endif
    }
    else
    {
        tempDiscKind.fDiscKind = svoVar_discKindT.fDiscKind;
        if (svoVar_discKindT.Bit.DiscStd == eDiscCD)
        {
            tempSpeed = SVO_CAL_SPEED_CD;
        }
        else if (svoVar_discKindT.Bit.DiscStd == eDiscDVD)
        {
#if (DVD_RAM_READ == 1)
            if (DVDRAM)
            {
                tempSpeed = SVO_CAL_SPEED_DVDRAM;
            }
            else
#endif
            {
                tempSpeed = SVO_CAL_SPEED_DVD;
            }
        }
#if (BD_ENABLE == 1)
        else if (svoVar_discKindT.Bit.DiscStd == eDiscBD)
        {
            tempSpeed = SVO_CAL_SPEED_BD;
        }
#endif // (BD_ENABLE == 1)
#if (ENABLE_HDDVD == 1)
        else if (svoVar_discKindT.Bit.DiscStd == eDiscHD)
        {
            tempSpeed = SVO_CAL_SPEED_HD;
        }
#endif // (ENABLE_HDDVD == 1)
    }
    LAL_StoreParams(tempDiscKind, tempSpeed);
    LAL_SetReadMode();

#if (EN_POWER_CONTROL == 0)
    vWakeUpFromPowerSaveMode();
#endif
} /* svoCmdStartProc */

/*-----------------------------------------------------------------------------
 *
 * Module: svoCmdStopProc()
 * Description:
 *
 *----------------------------------------------------------------------------*/
void svoCmdStopProc(void)
{
    svoCmdVar.svoCommand = CSTOP;
} /* svoCmdStopProc */

/*-----------------------------------------------------------------------------
 *
 * Module: svoCmdPauseProc()
 * Description:
 *
 *----------------------------------------------------------------------------*/
void svoCmdPauseProc(void)
{
#if (EN_LS == 1)
    if(svoVar_discKindT.Bit.DiscStd == eDiscLS)
    {
        set_SIF(EXECUTIONSTATE, svoReady);      /* set ExecutionState "svoReady" */
        return;
    }
#endif // End of (EN_LS == 1)

    if(svoCmdVar.cmdParam[0] == ePauseOff){     /* ePauseOff */
        svoCmdVar.svoCommand = CRELSE;
        return;
    }
    if(svoCmdVar.cmdParam[0] == ePauseOn ){     /* ePauseOn  */
        svoGetIDErrCount = 0;
        svoSeekErrCnt = 0;
#if (DeadLoop_Solution == 1)
        svoWblErrCount = 0;
#endif // (DeadLoop_Solution == 1)
        svoCmdVar.svoCommand = CPAUSE;
        return;
    }
    svoCmdIllegalProc(Accepted);         /* parameter eror */
} /* svoCmdPauseProc */

/*-----------------------------------------------------------------------------
 *
 * Module: svoCmdSeekProc()
 * Description:
 *
 *----------------------------------------------------------------------------*/
#define LIMIT_TIME_DATA 450000L

const short write_target_sub_fram_CD[MAX_SPEED_CD]={
/*   4LX, 8LX, 12LX, 16LX, 10X, 16X/20X, 24X, 32X, 40X, 48X, 52X  */
     60,  100, 100,  100,  100, 100, 100, 100, 100, 100,100
};

void svoCmdSeekProc(void)
{
    long    lba;
    long    targetPSN;

#if 0// EN_LS == TRUE // LightScrite test code and it will be removed in the final LS code
    if(svoVar_discKindT.Bit.DiscStd == eDiscLS)
    {
        targetPSN  = (ULONG)svoCmdVar.cmdParam[0] << 24;
        targetPSN += (ULONG)svoCmdVar.cmdParam[1] << 16;
        targetPSN += (ULONG)svoCmdVar.cmdParam[2] << 8;
        targetPSN += (ULONG)svoCmdVar.cmdParam[3];

        LSSeek(targetPSN); //need to create it on new .c file; Henry; 2008/07/14
        set_SIF(EXECUTIONSTATE, svoReady);      /* set ExecutionState "svoReady" */
        return;
    }

#endif  // EN_LS == TRUE // LightScrite test code and it will be removed in the final LS code

    if (svoVar_discKindT.Bit.DiscStd == eDiscDVD)
    {
        targetPSN  = (ULONG)svoCmdVar.cmdParam[0] << 24;
        targetPSN += (ULONG)svoCmdVar.cmdParam[1] << 16;
        targetPSN += (ULONG)svoCmdVar.cmdParam[2] << 8;
        targetPSN += (ULONG)svoCmdVar.cmdParam[3];

        targetPSN = (targetPSN & 0xFFFFFFF0);
        SendMsgCn(DEBUG_SEEK_MSG,4,0x334020,A4B(targetPSN));
#if (DVD_RAM_READ == 1)
        if (DVDRAM)
        {
            targetPSN -= INTERFACE_DELAY_DIST_DVDRAM;
        }
        else
#endif
        {
            if (svoVar_discKindT.Bit.Media == eMediaROM)
            {
                targetPSN -= INTERFACE_DELAY_DIST_DVDROM;
            }
            else
            {
                targetPSN -= INTERFACE_DELAY_DIST_DVDR_W;
            }
        }
        svoCmdVar.targetID.SectorNum.all32 = targetPSN;                  /*store target PSN*/
        svoCmdVar.targetID.Layer = (svoCmdVar.cmdParam[4] & LAYERMASK);  /*store target Layer*/
    }
#if (BD_ENABLE == 1)
    else if (svoVar_discKindT.Bit.DiscStd == eDiscBD)
    {
        targetPSN  = (ULONG)svoCmdVar.cmdParam[0] << 24;
        targetPSN += (ULONG)svoCmdVar.cmdParam[1] << 16;
        targetPSN += (ULONG)svoCmdVar.cmdParam[2] << 8;
        targetPSN += (ULONG)svoCmdVar.cmdParam[3];

        targetPSN = (targetPSN & 0xFFFFFFF0);
        SendMsgCn(DEBUG_SEEK_MSG,4,0x334020,A4B(targetPSN));
        if (svoVar_discKindT.Bit.Media == eMediaROM)
        {
            targetPSN -= INTERFACE_DELAY_DIST_BDROM;
        }
        else
        {
            targetPSN -= INTERFACE_DELAY_DIST_BDR_W;
        }
        svoCmdVar.targetID.SectorNum.all32 = targetPSN;                  /*store target PSN*/
        svoCmdVar.targetID.Layer = (svoCmdVar.cmdParam[4] & LAYERMASK);  /*store target Layer*/
    }
#endif //(BD_ENABLE == 1)
#if (ENABLE_HDDVD == 1)
    else if (svoVar_discKindT.Bit.DiscStd == eDiscHD)
    {
        targetPSN  = (ULONG)svoCmdVar.cmdParam[0] << 24;
        targetPSN += (ULONG)svoCmdVar.cmdParam[1] << 16;
        targetPSN += (ULONG)svoCmdVar.cmdParam[2] << 8;
        targetPSN += (ULONG)svoCmdVar.cmdParam[3];

        targetPSN = (targetPSN & 0xFFFFFFF0);
        SendMsgCn(DEBUG_SEEK_MSG,4,0x334020,A4B(targetPSN));
        if (svoVar_discKindT.Bit.Media == eMediaROM)
        {
            targetPSN -= INTERFACE_DELAY_DIST_HDROM;
        }
        else
        {
            targetPSN -= INTERFACE_DELAY_DIST_HDR_W;
        }

        if (spdInCLVArea(Iram_stp_CurrentPosition))
        {
//TBD? COMMENT! Magic number good for HD too?
            targetPSN -= 0x10;
        }

        svoCmdVar.targetID.SectorNum.all32 = targetPSN;                     /*store target PSN*/

        svoCmdVar.targetID.Layer = (svoCmdVar.cmdParam[4] & LAYERMASK);     /*store target Layer*/
        SendMsgCn(DEBUG_SEEK_MSG,1,0x334021,A1B(svoCmdVar.targetID.Layer));
    }
#endif //(ENABLE_HDDVD == 1)
    else if (svoVar_discKindT.Bit.DiscStd == eDiscCD)
    {
        lba  = (LONG)svoCmdVar.cmdParam[0] << 24;
        lba += (LONG)svoCmdVar.cmdParam[1] << 16;
        lba += (LONG)svoCmdVar.cmdParam[2] << 8;
        lba += (LONG)svoCmdVar.cmdParam[3];

        if((lba + 150L) >= LIMIT_TIME_DATA)
        {
            svoCmdIllegalProc(Range);           /* parameter eror */
            return;
        }

        if (svoCmdVar.cmdParam[4] == eWriteSeek)
        {
            lba = lba - (long)write_target_sub_fram_CD[(svoVar_svoTargetSpeed-1)];
            svoMiscVar.atipdisc_subq_diei_flag = TRUE;
        }
        else
        {

            if (svoVar_discKindT.Bit.Media == eMediaROM)
            {
                lba -= INTERFACE_DELAY_DIST_CDROM;
            }
            else
            {
                lba -= INTERFACE_DELAY_DIST_CDR_W;
            }
        }
        svoCmdVar.targetID.SectorNum.all32s = lba + 150;     /*store target PBA*/
        svoCmdVar.targetID.Layer = 0;  /*store target Layer*/
    }
    else
    {
        DEBUG_ASSERT(FALSE, eASSERT_IMPOSSIBLE_DISCKIND );
    }

    svoGetIDErrCount = 0;
    svoSeekErrCnt = 0;
    fgseektime = 0;
#if (DeadLoop_Solution == 1)
    svoWblErrCount = 0;
#endif // (DeadLoop_Solution == 1)
    svoCmdVar.svoCommand = CSEEK;
} /* svoCmdSeekProc */

/*-----------------------------------------------------------------------------
 *
 * Module: svoCmdJumpGroovesProc()
 * Description:
 *
 *----------------------------------------------------------------------------*/
void svoCmdJumpGroovesProc(void)
{
    svoCmdVar.commandedTrksToGo = (((short)svoCmdVar.cmdParam[1])<<8) + ((short)svoCmdVar.cmdParam[0]);

    set_SIF(STATE,svoStAccessing);      /* set state "svoStAccessing" */
    if(svoCmdVar.commandedTrksToGo < 0 ){
        svoCmdVar.commandedTrksToGo = ABS(svoCmdVar.commandedTrksToGo);
        svoSeekVar.jumpType = BACK;
    }else{
        svoSeekVar.jumpType = FORW;
    }
    svoSeekVar.jumpType |= F_J_MASK;
#if DO_HW_SUPPORT_STEP == 1
    hw_step_motor_fine_seek();
#endif /* DO_HW_SUPPORT_STEP */

    svoCmdVar.svoCommand = CJUMP;
} /* svoCmdJumpGroovesProc */
#if (EN_LS == 1)
/*-----------------------------------------------------------------------------
 *
 * Module: svoCmdLSSeekProc()
 * Description:
 *
 *----------------------------------------------------------------------------*/
void svoCmdLSSeekProc(void)
{
    long    targetPSN;

        targetPSN  = (ULONG)svoCmdVar.cmdParam[0] << 24;
        targetPSN += (ULONG)svoCmdVar.cmdParam[1] << 16;
        targetPSN += (ULONG)svoCmdVar.cmdParam[2] << 8;
        targetPSN += (ULONG)svoCmdVar.cmdParam[3];

        svoCmdVar.targetID.SectorNum.all32 = targetPSN;               /*store target PSN*/

        svoCmdVar.svoCommand = CLSSEEK;

}/* svoCmdLSSeekProc */
#endif // End of (EN_LS == 1)
/*-----------------------------------------------------------------------------
 *
 * Module: svoPowerOnInit()
 * Description:
 *
 *----------------------------------------------------------------------------*/
void svoPowerOnInit(void)
{
    UINT intr;

    intr = GET_INT_ENABLE_MASK();
    DISABLE_INTERRUPT();
    svoInitVar();               /* SVO variable initialization */
#if (EN_LS == 1)
    bLSFOOROC = FALSE;
    WRITE_FIELD(HAL_LS_SPOKE0EN, 0);
    WRITE_FIELD(HAL_LS_SPOKEEN, 0);
#endif // End of (EN_LS == 1)

    LAL_InitServo();

    Cal_LoadOfflineCalibrationParam();
    svoAFEInitOnly();          /* AFE chip initialization */
    SpdInit();         /* servo speed? initialization */

    svoServoInit();
    SET_INT_ENABLE_MASK(intr);
} /* svoPowerOnInit */


/*-----------------------------------------------------------------------------
 *
 * Module: cd_speed_tbl()
 * Description: This table lists the supported Servo speeds for CD discs
 *              If a speed is not supported then its location must have a
 *              NOP entered.
 *----------------------------------------------------------------------------*/
const BYTE cd_speed_tbl[4][SPEEDTBL_CD+1]={{SUPPORTED_CD_CLV_SPEED},   // CLV
                                           {SUPPORTED_CD_ZCLV_SPEED},  // ZCLV
                                           {SUPPORTED_CD_CAV_SPEED},   // CAV
                                           {SUPPORTED_CD_PCAV_SPEED}}; // PCAV

/*-----------------------------------------------------------------------------
 *
 * Module: dvd_speed_tbl()
 * Description: This table lists the supported Servo speeds for DVD discs
 *              If a speed is not supported then its location must have a
 *              NOP entered.
 *----------------------------------------------------------------------------*/
const BYTE dvd_speed_tbl[4][SPEEDTBL_DVD+1]={{SUPPORTED_DVD_CLV_SPEED},   // CLV
                                             {SUPPORTED_DVD_ZCLV_SPEED},  // ZCLV
                                             {SUPPORTED_DVD_CAV_SPEED},   // CAV
                                             {SUPPORTED_DVD_PCAV_SPEED}}; // PCAV

#if (DVD_RAM_READ == 1)
/*-----------------------------------------------------------------------------
 *
 * Module: dvdram_speed_tbl()
 * Description: This table lists the supported Servo speeds for DVD discs
 *              If a speed is not supported then its location must have a
 *              NOP entered.
 *----------------------------------------------------------------------------*/
const BYTE dvdram_speed_tbl[4][SPEEDTBL_DVDRAM+1]={{SUPPORTED_DVDRAM_CLV_SPEED},   // CLV
                                                   {SUPPORTED_DVDRAM_ZCLV_SPEED},  // ZCLV
                                                   {SUPPORTED_DVDRAM_CAV_SPEED},   // CAV
                                                   {SUPPORTED_DVDRAM_PCAV_SPEED}}; // PCAV
#endif

#if (BD_ENABLE == 1)
/*-----------------------------------------------------------------------------
 *
 * Module: bd_speed_tbl()
 * Description: This table lists the supported Servo speeds for BD discs
 *              If a speed is not supported then its location must have a
 *              NOP entered.
 *----------------------------------------------------------------------------*/
const BYTE bd_speed_tbl[4][SPEEDTBL_BD+1]={{SUPPORTED_BD_CLV_SPEED},   // CLV
                                           {SUPPORTED_BD_ZCLV_SPEED},  // ZCLV
                                           {SUPPORTED_BD_CAV_SPEED},   // CAV
                                           {SUPPORTED_BD_PCAV_SPEED}}; // PCAV
#endif // (BD_ENABLE == 1)

#if (ENABLE_HDDVD == 1)
/*-----------------------------------------------------------------------------
 *
 * Module: hd_speed_tbl()
 * Description: This table lists the supported Servo speeds for BD Discs
 *              If a speed is not supported then its location must have a
 *              NOP entered.
 *----------------------------------------------------------------------------*/
const BYTE hd_speed_tbl[4][SPEEDTBL_HD+1]={{SUPPORTED_HD_CLV_SPEED},   // CLV
                                           {SUPPORTED_HD_ZCLV_SPEED},  // ZCLV
                                           {SUPPORTED_HD_CAV_SPEED},   // CAV
                                           {SUPPORTED_HD_PCAV_SPEED}}; // PCAV
#endif // (ENABLE_HDDVD == 1)

/*-----------------------------------------------------------------------------
 *
 * Module: svoCmdSetSpeedProc()
 * Description: This function will extract the servo speed based svoCmdVar.cmdParam[1]
 *              and the disc type
 *
 *----------------------------------------------------------------------------*/
#if (SPEED_LOCK_DEBUG == 1)
int     speedLockPassword;
#endif //(SPEED_LOCK_DEBUG == 1)

void svoCmdSetSpeedProc(void)
{
    BYTE    para;

    if (svoVar_discKindT.Bit.DiscStd == eDiscDVD)
    {
        if ((svoCmdVar.cmdParam[1] > 3) || (svoCmdVar.cmdParam[0] > SPEEDTBL_DVD))
        {
            svoCmdIllegalProc(Accepted);         /* parameter error */
            SendMsg80(SHOW_DEBUG_MSG,0x3310FF);
            return;
        }
        else
        {
#if (DVD_RAM_READ == 1)
            if(DVDRAM)
            {
                para = dvdram_speed_tbl[svoCmdVar.cmdParam[1]][svoCmdVar.cmdParam[0]];
                SendMsg80(SHOW_DEBUG_MSG,0x331180+para);
            }
            else
#endif
            {
                para = dvd_speed_tbl[svoCmdVar.cmdParam[1]][svoCmdVar.cmdParam[0]];
                SendMsg80(SHOW_DEBUG_MSG,0x331070+para);
            }
        }
    }
    else if ((svoVar_discKindT.Bit.DiscStd == eDiscCD) || (svoVar_discKindT.Bit.DiscStd == eDiscUnknown))
    {
        if ((svoCmdVar.cmdParam[1] > 3) || (svoCmdVar.cmdParam[0] > SPEEDTBL_CD))
        {
            svoCmdIllegalProc(Accepted);         /* parameter error */
            SendMsg80(SHOW_DEBUG_MSG,0x3310FF);
            return;
        }
        else
        {
            para = cd_speed_tbl[svoCmdVar.cmdParam[1]][svoCmdVar.cmdParam[0]];
            SendMsg80(SHOW_DEBUG_MSG,0x331000+para);
        }
    }
#if (ENABLE_HDDVD == 1)
    else if (svoVar_discKindT.Bit.DiscStd == eDiscHD)
    {
        if ((svoCmdVar.cmdParam[1] > 3) || (svoCmdVar.cmdParam[0] > SPEEDTBL_HD))
        {
            svoCmdIllegalProc(Accepted);         /* parameter error */
            SendMsg80(SHOW_DEBUG_MSG,0x3310FF);
            return;
        }
        else
        {
            para = hd_speed_tbl[svoCmdVar.cmdParam[1]][svoCmdVar.cmdParam[0]];
            SendMsg80(SHOW_DEBUG_MSG,0x3310B0+para);
        }
    }
#endif //(ENABLE_HDDVD == 1)
#if (BD_ENABLE == 1)
    else if (svoVar_discKindT.Bit.DiscStd == eDiscBD)
    {
        if ((svoCmdVar.cmdParam[1] > 3) || (svoCmdVar.cmdParam[0] > SPEEDTBL_BD))
        {
            svoCmdIllegalProc(Accepted);         /* parameter error */
            SendMsg80(SHOW_DEBUG_MSG,0x3310FF);
            return;
        }
        else
        {
            para = bd_speed_tbl[svoCmdVar.cmdParam[1]][svoCmdVar.cmdParam[0]];
            SendMsg80(SHOW_DEBUG_MSG,0x3310D0+para);
        }
    }
#endif //(BD_ENABLE == 1)

    /* speed check */
    if(para == (BYTE)NOP)
    {
        svoCmdIllegalProc(Accepted);         /* parameter error */
        SendMsg80(DEBUG_SVO_SPD_MSG,0x3310FF);
        return;
    }
    else
    {
        svoVar_svoTargetSpeed = para;
        SendMsgCn(DEBUG_SVO_SPD_MSG,1,0x331120,svoCmdVar.cmdParam[0]);
    }

    /* speed mode check */
    if (svoCmdVar.cmdParam[1]==(e_PCAV>>6)) //check if PCAV or ZCLV?
    {
        if ((svoVar_discKindT.fDiscKind == eDVDROMDisc)||(svoVar_discKindT.fDiscKind == eDVDROMDLDisc)||(svoVar_discKindT.fDiscKind == eCDROMDisc))
        {
            svoCmdIllegalProc(Accepted);         /* parameter error */
            SendMsg80(DEBUG_SVO_SPD_MSG,0x33110F); //***Invaild speed mode***
            return;
        }
        else
        {
            svoMiscVar.zoneAreaCheckFlag = TRUE; //TBD? This flage should be ZCLV mode only
        }
    }
    else
    {
        SendMsg80(DEBUG_SVO_SPD_MSG,0x331110+svoCmdVar.cmdParam[1]); //CLV/ZCLV/CAV/PCAV mode
    }

    //svoMiscVar.zoneAreaCheckFlag = FALSE;

#if (SPEED_LOCK_DEBUG == 1)
    if ((speedLockPassword >= 0x12345600) && (speedLockPassword <= 0x1234561F))
    {
        para = (speedLockPassword & 0x1F);
    }
#endif //(SPEED_LOCK_DEBUG == 1)
}

/*-----------------------------------------------------------------------------
 *
 * Module: svoCmdGetStatusInformationProc()
 * Description:
 *
 *----------------------------------------------------------------------------*/
void svoCmdGetStatusInformationProc(void)
{
    StWordU   DK;

    set_SIF(PARAMETER1, svoVar_discDiameter);
    DK.Uword = svoVar_discKindT.fDiscKind;
    set_SIF(PARAMETER2, DK.byte.l);
    set_SIF(PARAMETER3, DK.byte.h);
} /* svoCmdGetStatusInformationProc */

/*-----------------------------------------------------------------------------
 *
 * Module: svoCmdGetErrorInformationProc()
 * Description:
 *
 *----------------------------------------------------------------------------*/
void svoCmdGetErrorInformationProc(void)
{
    set_SIF(PARAMETER1, svoCmdVar.servoError);
    set_SIF(PARAMETER2, svoCmdVar.playerError);
    set_SIF(PARAMETER3, svoCmdVar.discError);
    set_SIF(PARAMETER4, svoCmdVar.commandError);
    set_SIF(PARAMETER5, svoCmdVar.trayError);

    svoCmdVar.servoError   = 0;
    svoCmdVar.playerError  = 0;
    svoCmdVar.discError    = 0;
    svoCmdVar.commandError = 0;
    svoCmdVar.trayError    = 0;
} /* svoCmdGetErrorInformationProc */

/*-----------------------------------------------------------------------------
 *
 * Module: svoCmdVeloAdjustProc()
 * Description:
 *
 *----------------------------------------------------------------------------*/
void svoCmdVeloAdjustProc(void)
{
    svoCmdVar.svoCommand = CVELO;
}

/*-----------------------------------------------------------------------------
 *
 * Module: svoCmdLDAdjustProc()
 * Description:
 *
 *----------------------------------------------------------------------------*/
void svoCmdLDAdjustProc(void)
{
   svoCmdVar.svoCommand = CLDAJ;
}

/*-----------------------------------------------------------------------------
 *
 * Module: svoCmdDiagnosticProc()
 * Description: for focus balance calibration.
 *
 *----------------------------------------------------------------------------*/
void svoCmdDiagnosticProc(void)
{
    svoCmdVar.svoCommand = CDIAG;
}



#if (SERVICE_MODE==1)
/*-----------------------------------------------------------------------------
 *
 * Module: svoCmdServiceModeProc()
 * Description:
 *
 *----------------------------------------------------------------------------*/
void svoCmdServiceModeProc(void)
{
static USHORT savedSvoInState;
static ServoStatesT savedSIFstate;

    if ((svoCmdVar.cmdParam[0] == ON) && (SIF.ServoState != svoStServiceMode))
    {//Save current situation: Maybe re-use works
        savedSvoInState = svoIramVar.svoInState;
        savedSIFstate = get_SIF(STATE);
        svoIramVar.svoInState = (WAIT_CMD_DECODE | SEQ_001); //In new table: decode
        set_SIF(STATE,svoStServiceMode);                  //From here on SERVICE
        SendMsg80(DEBUG_SERVO_TRACE_MSG,0x3E0000);  //ServiceMode On
    }
    if ((svoCmdVar.cmdParam[0] == OFF) && (SIF.ServoState == svoStServiceMode))
    {//Restore old situation: Maybe re-use works: THERE IS IS NO GUARANTEE
        svoIramVar.svoInState = savedSvoInState;
        set_SIF(STATE, savedSIFstate);
        SendMsg80(DEBUG_SERVO_TRACE_MSG,0x3E00FF);  //ServiceMode Off
    }
}


/*-----------------------------------------------------------------------------
 *
 * Module: svoCmdLoopControlProc()
 * Description: For Open/Close tracking and focus loop, On/Off Laser and spindle loop
 *              Meant to function uncontrolled, no recoveries, no system reactions
 *----------------------------------------------------------------------------*/
void svoCmdLoopControlProc(void)
{
#define TRACKINGLOOP    0x03        /* bit 1,0 */
#define TRACK_LOOPON    0x01
#define TRACK_LOOPOFF   0x02
#define FOCUSLOOP       0x0C        /* bit 3,2 */
#define FOCUS_LOOPON    0x04
#define FOCUS_LOOPOFF   0x08
#define LASERLOOP       0x30        /* bit 5,2 */
#define CDLASER_LOOPON  0x10
#define DVDLASER_LOOPON 0x20
#define LASER_LOOPOFF   0x30
#define SPINDLELOOP     0xC0        /* bit 7,6 */
#define SPINDLE_LOOPON  0x40
#define SPINDLE_LOOPOFF 0x80

    SendMsg80(DEBUG_SERVO_TRACE_MSG,0x3E,0x00,svoCmdVar.cmdParam[0]);  //Service

    switch (svoCmdVar.cmdParam[0] & TRACKINGLOOP)
    {
        case TRACK_LOOPON:
            svoCmdVar.svoCommand = SV_TRACK_ON;
            break;
        case TRACK_LOOPOFF:
            svoCmdVar.svoCommand = SV_TRACK_OFF;
            break;
        default:
            break;
    }

    switch (svoCmdVar.cmdParam[0] & FOCUSLOOP)
    {
        case FOCUS_LOOPON:
            svoCmdVar.svoCommand = SV_FOCUS_ON;
            break;
        case FOCUS_LOOPOFF:
            svoCmdVar.svoCommand = SV_FOCUS_OFF;
            break;
        default:
            break;
    }

    switch (svoCmdVar.cmdParam[0] & LASERLOOP)
    {
        case CDLASER_LOOPON:
            HAL_LASC_SetLaserOn(TRUE);
            set_SIF(EXECUTIONSTATE,svoReady); /* Direct */
            break;
        case DVDLASER_LOOPON:
            HAL_LASC_SetLaserOn(TRUE);
            set_SIF(EXECUTIONSTATE,svoReady); /* Direct */
            break;
        case LASER_LOOPOFF:
            HAL_LASC_SetLaserOn(FALSE);
            set_SIF(EXECUTIONSTATE,svoReady); /* Direct */
            break;
        default:
            break;
    }
#if 0
//TBD? COMMENT! Not implemented yet, because I don't understand the system sufficiently
    switch (svoCmdVar.cmdParam[0] & SPINDLELOOP)
    {
        case SPINDLE_LOOPON:
            svoCmdVar.svoCommand = SV_SPINDLE_ON;
            break;
        case SPINDLE_LOOPOFF:
            svoCmdVar.svoCommand = SV_SPINDLE_OFF;
            break;
        default:
            break;
    }
#endif // 0
}

    #if CMD_SVO_CMD_READ_GPADC == 1

#define SIGNALS_PER_MUX 32
#define TOTAL_NR_OF_SIGNALS  SIGNALS_PER_MUX * 6


/* Parameter 1 -> measurement mode */
typedef enum
{
   ONCE,
   AVERAGE,
   MINMAX,
   ONCE_MILIVOLTS,
   AVERAGE_MILIVOLTS,
   MINMAX_MILIVOLTS
} MeasurementMode_t;

/* Parameter 2 -> measurement condition */
typedef enum
{
   UNCONDITIONAL,
   INFOCUS
} MeasurementCondition_t;

/* Parameter 3 -> number of samples */
typedef BYTE NrOfSamples_t;


static MeasurementMode_t preProcessMeasurementMode(const MeasurementMode_t, NrOfSamples_t * );
static void preparePrintToAtapi(USHORT val1, USHORT val2);


static MeasurementMode_t preProcessMeasurementMode(const MeasurementMode_t measMode, NrOfSamples_t * pnr)
{
   MeasurementMode_t mode = measMode;
   BOOL miliVolts = mode > MINMAX;

   if (*pnr > 1)
   {
      switch (mode)
      {
         case ONCE:              mode = miliVolts? ONCE_MILIVOLTS:    ONCE;    *pnr = 1; break;
         case AVERAGE:           mode = miliVolts? AVERAGE_MILIVOLTS: AVERAGE;           break;
         case MINMAX:            mode = miliVolts? MINMAX_MILIVOLTS:  MINMAX;            break;
         case ONCE_MILIVOLTS:    mode = miliVolts? ONCE_MILIVOLTS:    ONCE;    *pnr = 1; break;
         case AVERAGE_MILIVOLTS: mode = miliVolts? AVERAGE_MILIVOLTS: AVERAGE;           break;
         case MINMAX_MILIVOLTS:  mode = miliVolts? MINMAX_MILIVOLTS:  MINMAX;            break;
      }
   }
   else
   {
      *pnr = 1;
      mode = miliVolts? ONCE_MILIVOLTS: ONCE;
   }
   return mode;
}

static void preparePrintToAtapi(USHORT val1, USHORT val2)
{
   set_SIF(PARAMETER1, (BYTE)(val1 & 0xFF));
   set_SIF(PARAMETER2, (BYTE)((val1 >> 8) & 0xFF));
   set_SIF(PARAMETER3, (BYTE)(val2 & 0xFF));
   set_SIF(PARAMETER4, (BYTE)((val2 >> 8) & 0xFF));
}

void svoCmdReadGpAdcProc(void)
{
    MeasurementMode_t MM = svoCmdVar.cmdParam[1];
    NrOfSamples_t nr = svoCmdVar.cmdParam[3];
    USHORT i, rev;
    ad_dsp_t signal;
    ADSignal_t MuxField;
    USHORT val, val2;
    ULONG top, bot, pp, avg;

    MM = preProcessMeasurementMode(MM, &nr);  /* parameters MM & nr checked and adjusted if needed */
                                             /* make connection of signal to ADC */
    val = 0; val2 = 0;

    i = nr/DISK_NFGPLS;
    i = i == 0? 1: i;

    top = bot = pp = avg = 0;
    rev = i;
    while (i--)
    {
        MuxField.mux = svoCmdVar.cmdParam[0]%SIGNALS_PER_MUX;

        signal = ReadGPADCmV(ONE_REVOLUTION, AfeRouteSignalToAD(svoCmdVar.cmdParam[0]/SIGNALS_PER_MUX, MuxField), TRUE);

        if (signal.bot_saturated || signal.top_saturated)
        {
            /* SATURATION */
            preparePrintToAtapi(val, val2);
            set_SIF(EXECUTIONSTATE,svoError); /* Direct */
            return;
        }
        else
        {
            bot += signal.bot_mv;
            top += signal.top_mv;
            avg += signal.middle_mv;
            pp  += signal.p2p_mv;
        }
    }
    signal.bot_mv    = bot/rev ;
    signal.top_mv    = top/rev;
    signal.middle_mv = avg/rev;
    signal.p2p_mv    = pp/rev;

    switch (MM)
    {
     case ONCE: /* measurementCondition*/
     case AVERAGE: /* measurementCondition, nrOfSamples, delay */
     case ONCE_MILIVOLTS:
     case AVERAGE_MILIVOLTS:
        val  = signal.middle_mv;
        val2 = 0;
        break;

     case MINMAX:  /* measurementCondition, nrOfSamples, delay */
     case MINMAX_MILIVOLTS:
        val  = signal.bot_mv;
        val2 = signal.top_mv;
        break;
    }
    SendMsgCn(SHOW_DEBUG_MSG,4,0x33FF34,A2B(val),A2B(val2));

    preparePrintToAtapi(val, val2);
    set_SIF(EXECUTIONSTATE,svoReady); /* Direct */
    return;
}


    #endif /* CMD_SVO_CMD_READ_GPADC */


    #if CMD_DRY_READ == 1
/*
 * Dry Read
 *
 * Pre:
 *      calibration must be done first
 *
 * Post:
 *      laser power measurable without disk
 *
 */

typedef enum
{
    PARAM_CDR     = 0x02,
    PARAM_CDRW          ,
    PARAM_DVDRAM  = 0x84,
    PARAM_DVDPR   = 0x8A,
    PARAM_DVDPRW        ,
    PARAM_DVDMR   = 0x92,
    PARAM_DVDMRW        ,
    PARAM_DVDPRDL = 0xCA,
    PARAM_DVDMRDL = 0xD2
} paramDiskKind_t;

typedef enum
{
    DRY_READ_OFF,
    DRY_READ_ON
} stateDryRead_t;

#define paramDiskKind  (((paramDiskKind_t)svoCmdVar.cmdParam[0])) /* disc kind */
#define stateDryRead  svoCmdVar.cmdParam[1]                     /* on/off    */

//TBD!
void svoCmdDryReadProc(void)
{

    Cal_LoadOfflineCalibrationParam();

    LAL_StoreParams(paramDiskKind, svoIramVar.svoCurrentSpeed);
    LAL_SetReadMode();

    switch(paramDiskKind)
    {
        case PARAM_CDR :
        case PARAM_CDRW :
            break;

        case PARAM_DVDRAM :
        case PARAM_DVDPR :
        case PARAM_DVDPRW :
        case PARAM_DVDMR :
        case PARAM_DVDMRW :
        case PARAM_DVDPRDL :
        case PARAM_DVDMRDL :
            break;

        default :
            /* Erroneous */
            HAL_LASC_SetLaserOn(FALSE);
            set_SIF(EXECUTIONSTATE,svoReady); /* Direct */
            return;
    }
    if ((stateDryRead_t)stateDryRead == DRY_READ_ON)
    {
        HAL_LASC_SetLaserOn(TRUE);
    }
    else /* DRY_READ_OFF */
    {
        HAL_LASC_SetLaserOn(FALSE);
    }
    set_SIF(EXECUTIONSTATE,svoReady); /* Direct */
    return;
}
    #endif // CMD_DRY_READ


    #if (TEST_AVERAGING_SERVO == 1) && (AFE_NORMALIZER == 1)

/*
 * svoCmdTestAvgServoProc
 *
 * Pre:
 *      calibration must be done first
 *
 * Post:
 *
 *
 */

typedef enum
{
   SVOGN_INIT,
   ADAPT_SVOGN_ENABLE,
   SVOGN_CTRL_SERVO_ON,
   SVOGN_CTRL_SERVO_OFF,
   ADAPT_SVOGN_DISABLE
} svogn_func_t;

typedef enum
{
   NO_CHANGE,
   CHANGE_GAIN
} changeGain_t;

#define svogn_func (svogn_func_t)svoCmdVar.cmdParam[0]
#define fixedGn          (USHORT)svoCmdVar.cmdParam[1]
#define defaultGn        (USHORT)svoCmdVar.cmdParam[2]
#define target           (USHORT)svoCmdVar.cmdParam[3]
#define threshold        (USHORT)svoCmdVar.cmdParam[4]
#define rate             (USHORT)svoCmdVar.cmdParam[5]
#define changeGain (changeGain_t)svoCmdVar.cmdParam[6]

void svoCmdTestAvgServoProc(void)
{
   switch(svogn_func)
   {
      case SVOGN_INIT :
         if (changeGain == CHANGE_GAIN)
         {
            SetSCRegField_1B(f_rw_TARGETBS,target);
            SetSCRegField_1B(f_rw_BSTHRES,threshold);
            SetSCRegField_1B(f_rw_SETSVOGN,defaultGn);
            SetSCRegField_1B(f_rw_BS2GNREG,fixedGn);
            SetSCRegField_1B(f_rw_BSRATE,rate);
         }
         else
         {
            afeSvognInit();
         }
         break;

      case ADAPT_SVOGN_ENABLE :
         afeSvognEnable();
         break;

      case SVOGN_CTRL_SERVO_ON :
         afeSvognCtrlOn();
         break;

      case SVOGN_CTRL_SERVO_OFF :
         afeSvognCtrlOff();
         break;

      case ADAPT_SVOGN_DISABLE :
         if (changeGain == CHANGE_GAIN)
         {
            SetSCRegField_1B(f_rw_SETSVOGN,defaultGn);
            SetSCRegField_1B(f_rw_BS2GNREG,fixedGn);
            afeSvognDisable();
         }
         else
         {
            afeSvognDisable();
         }
         break;

      default:
         /* error */
         break;
   }
}
    #endif // TEST_AVERAGING_SERVO
#endif /* SERVICE_MODE */

/*-----------------------------------------------------------------------------
 *
 * Module: svoCmdReAdjustProc()
 * Description: TBD!
 *
 *----------------------------------------------------------------------------*/
void svoCmdReAdjustProc(void)
{
    StWordU DK;
    SHORT   orig_pdic_gn,orig_laser_gn;
    discKindT svoVar_discKindT_org;
    ESpindleSpeed cal_speed;

    svoVar_discKindT_org = svoVar_discKindT;
    DK.byte.l = svoCmdVar.cmdParam[0];
    DK.byte.h = svoCmdVar.cmdParam[1];
    svoVar_discKindT.fDiscKind = (eDiscKind)DK.Uword;

    track_off();
    HAL_LASC_SetLaserOn(FALSE);
    switch (svoVar_discKindT.Bit.DiscStd)
    {
        case eDiscCD:
            cal_speed = SVO_CAL_SPEED_CD;
#if (EN_POWER_CONTROL == 1)
    #if (WOBBLE_READ_ON_DVD_OR_CD == 1)
            if (svoVar_discKindT.Bit.Media == eMediaROM)
            {
                PowerControl(eCDReadPower);
            }
            else
            {
                PowerControl(eCDReadWblPower);
            }
    #endif
#endif
            break;

        case eDiscDVD:
            cal_speed = SVO_CAL_SPEED_DVD;
#if (EN_POWER_CONTROL == 1)
    #if (WOBBLE_READ_ON_DVD_OR_CD == 1)
            if (svoVar_discKindT.Bit.Media == eMediaROM)
            {
                PowerControl(eDVDReadPower);
            }
            else
            {
                PowerControl(eDVDReadWblPower);
            }
    #endif
#endif
            break;
#if (BD_ENABLE == 1)
        case eDiscBD:
            cal_speed = SVO_CAL_SPEED_BD;
    #if EN_POWER_CONTROL
            PowerControl(eBDReadWblPower);
    #endif
            break;
#endif
#if (ENABLE_HDDVD == 1)
        case eDiscHD:
            cal_speed = SVO_CAL_SPEED_HD;
    #if EN_POWER_CONTROL
            if (svoVar_discKindT.Bit.Media == eMediaROM)
            {
                PowerControl(eHDReadPower);
            }
            else
            {
                PowerControl(eHDReadWblPower);
            }
    #endif
            break;
#endif
    }
    orig_pdic_gn = svoCalMisc2Var.pdic_gn;
    orig_laser_gn = svoCalMisc2Var.laser_gn;
    LAL_StoreParams(svoVar_discKindT, cal_speed);
    LAL_SetReadMode();
#if (BD_ENABLE == 1) && (ENABLE_BDRLTH == 1)
    if (svoIramVar.LTHDisclikely == 1)
    {
        svoIramVar.LTHDisclikely = 0;

    #if NEW_SERVO_CALIB_FLOW == 0
        calClrCalResult(TILT_DAT,FALSE);
        calClrCalResult(FEBC_DSP,FALSE);
        #if COLLI_SWITCH == 1
        calClrCalResult(COLLI_POS,FALSE);
        #endif
    #endif
        svoCalVar.calOfsDoneBits  &= ~(teof_end|ofaj_end);
        svoCalVar.calGainDoneBits &= ~(felg_end|teg_end|telg_end|dif_RF_end|dif_WOB_end);
    }
#endif

    if ((orig_pdic_gn != svoCalMisc2Var.pdic_gn) || (orig_laser_gn != svoCalMisc2Var.laser_gn) ||
        (svoVar_discKindT_org.Bit.Layers != svoVar_discKindT.Bit.Layers))
    {
        svoAFEInitOnly();
        svo_Adj_Data_Clr();
    }
    svoCalVar.discChkDoneBits |= disc_chk_end|stm_chk_end;
    svoCalVar.calGainDoneBits &= ~(felg_end|telg_end|dif_WOB_end);
    if (getTrackingMethod(svoVar_discKindT_org) != getTrackingMethod(svoVar_discKindT))
    {
        svoCalVar.calGainDoneBits &= ~teg_end;
    }
    svoIramVar.svoInState = (STAT_NORMAL | SVO_RESTART | SEQ_001);
}

#if (CALIBRATION_MANAGEMENT == 1)
/*-----------------------------------------------------------------------------
 *
 * Module: svoCmdReCalibrateProc()
 * Description:
 *
 *----------------------------------------------------------------------------*/
void svoCmdReCalibrateProc(void)
{
    svoCmdVar.svoCommand = CCALIB;
}
#endif /* CALIBRATION_MANAGEMENT */

#if (HORROR_DISC_IMPROVEMENT == 1)
/*-----------------------------------------------------------------------------
 *
 * Module: svoCmdChgRFParameterProc()
 * Description:
 *
 *----------------------------------------------------------------------------*/
void svoCmdChgRFParameterProc(void)
{
    svoCmdVar.svoCommand = CHGRFEQ;
}
#endif // (HORROR_DISC_IMPROVEMENT == 1)

#if (FORCE_DISK_KIND == 1)
/*-----------------------------------------------------------------------------
 *
 * Module: svoCmdForceDiscKindProc()
 * Description:
 *
 *----------------------------------------------------------------------------*/
DiscForceT DiscForce;

void svoCmdForceDiscKindProc(void)
{
    DiscForce = svoCmdVar.cmdParam[0];
    disc_kind_preset((USHORT)((svoCmdVar.cmdParam[1] << 8) | svoCmdVar.cmdParam[2]));
    svo_preset_media_par();
}
#endif // (FORCE_DISK_KIND == 1)

/*-----------------------------------------------------------------------------
 *
 * Module: svoCmdAdjFocActDcActSensProc()
 * Description:
 *
 *----------------------------------------------------------------------------*/
void svoCmdAdjFocActDcActSensProc(void)
{
    svoVar_discKindT.fDiscKind = svoCmdVar.cmdParam[0];
    svoVar_STM.CaliFocActSens  = svoCmdVar.cmdParam[1];
    switch(svoVar_discKindT.fDiscKind)
    {
        case eCDROMDisc:
        case eCDRDisc:
        case eCDRWDisc:
            svoVar_STM.NominalTime = eNominalTimeCD;
            break;

        case eDVDROMDisc:
        case eDVDROMDLDisc:
        case eDVDPlusRDisc:
        case eDVDPlusRWDisc:
        case eDVDMinusRDisc:
        case eDVDMinusRWDisc:
        case eDVDPlusRDLDisc:
        case eDVDMinusRDLDisc:
            svoVar_STM.NominalTime = eNominalTimeDVD;
            break;

        case eHDROMDisc:
#if (ENABLE_CBHD == 1)
        case eCBHDROMDisc:
#endif
        case eHDRDisc:
        case eHDRWDisc:
        case eHDRAMDisc:
        case eHDROMDLDisc:
#if (ENABLE_CBHD == 1)
        case eCBHDROMDLDisc:
#endif
        case eHDRDLDisc:
        case eHDRWDLDisc:
            svoVar_STM.NominalTime = eNominalTimeHDDVD;
            break;

        case eBDROMDisc:
        case eBDRDisc:
        case eBDREDisc:
        case eBDROMDLDisc:
        case eBDRDLDisc:
        case eBDREDLDisc:
            svoVar_STM.NominalTime = eNominalTimeBD;
            break;

        default:
            svoVar_STM.NominalTime = eNominalTimeNoDisc;
            break;
    }
}

#if (BCA_ENABLE == 1)
/*-----------------------------------------------------------------------------
 *
 * Module: svoCmdBCASeekProc()
 * Description:
 *
 *----------------------------------------------------------------------------*/
void svoCmdBCASeekProc(void)
{
    svoCmdVar.svoCommand = CBCAJP;
    svoSeekVar.roughCount = 0;// improve BCA jump (handle focus off during jump)
}
#endif // (BCA_ENABLE == 1)

/*-----------------------------------------------------------------------------
 *
 * Module: set_SIF()
 * Description:
 *
 *----------------------------------------------------------------------------*/
void set_SIF(BYTE type,BYTE code)
{
    BYTE *p;

    p = (BYTE *)&SIF;
    p = (p + type);
    *p = code;

#if (PRN_HIF_TO_SIF)
    if (type == EXECUTIONSTATE)
        SIF_CMD_MESSAGE(PRN_HIF_TO_SIF,1);
#endif // (PRN_HIF_TO_SIF)
}

/*-----------------------------------------------------------------------------
 *
 * Module: get_SIF()
 * Description:
 *
 *----------------------------------------------------------------------------*/
BYTE get_SIF(BYTE type)
{
    BYTE *p;

    p = (BYTE *)&SIF;
    p = (p + type);
    return(*p);
}


/*------------------------------------------------------------------------*/
/* The end of file                                                        */
/*------------------------------------------------------------------------*/
