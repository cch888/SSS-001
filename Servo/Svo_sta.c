/****************************************************************************
*            (c) Copyright 2001 - 2010 Sunext Technology Co., Ltd.
*                           All Rights Reserved
*
*
* $Revision: 2711 $
* $Date: 11/05/10 9:36a $
*
* DESCRIPTION:   Servo State functions
*
**************************** SUNEXT CONFIDENTIAL ****************************/

#include ".\common\globtype.h"
#include ".\common\globals.h"
#if (SET_SLOT_IN == 1)
#include ".\common\GLOBTYPE.H" //[Q36]
#endif
#include ".\common\chip.h"
#include ".\common\macros.h"
#include ".\common\stdlib.h"
#include ".\servo\svo.h"
#include ".\servo\svo_afe.h"
#include ".\common\error.h"
#include ".\common\commath.h"
#include ".\common\system.h"
#include ".\common\scomm.h"
#include ".\common\ADC.h"
#include ".\common\com_im.h"
#include ".\player\plrdb.h"
#include ".\hif\gp_ram.h"
#include ".\hif\diag.h"
#include ".\AL\REG_servo.h"
#include ".\AL\HAL_debug.h"
#if (BD_ENABLE == 1)
#include ".\BD\bddiag.h"
#include ".\BD\bdVars.h"
#include ".\BD\bdDiscRd.h"
#endif // #if (BD_ENABLE == 1)

#include ".\servo\svo_com.h"
#include ".\servo\svo_tbl.h"
#include ".\servo\svomath.h"
#include ".\servo\svo_cmd.h"
#include ".\servo\svo_lib.h"
#include ".\servo\svo_calib.h"
#include ".\servo\svo_speed.h"
#include ".\servo\svo_seek.h"
#include ".\servo\svo_step.h"
#include ".\servo\svoVar.h"
#include ".\servo\svo_sta.h"
#include ".\servo\afe_tbl.h"
#include ".\firmw_id.h"
#include ".\dvd\dvcfg.h"
#include ".\oem\Oem_spd_tbl.h"
#include ".\dvd\dvDef.h"
#include ".\dvd\dvGvars.h"
#if (ENABLE_HDDVD == 1)
#include ".\hd\hdDef.h"
#include ".\hd\hdDB.h"
#endif // #if (ENABLE_HDDVD == 1)

#if ((EN_WOBBLE == 1) && (BD_ENABLE == 1))
#include ".\bd\bdDef.h"
#include ".\bd\bdVars.h"
#endif // #if (BD_ENABLE == 1) && (EN_WOBBLE == 1)

#if (DVD_RAM_READ == 1)
#include ".\defmgmt\dvdram\dvDefMgr.h"
#include ".\defmgmt\common\mrw_ram.h"
#endif

#include ".\DVD\dvMacros.h"
#include ".\oem\oem_tbl.h"

#include ".\al\REG_global.h"
#include ".\al\REG_dfe.h"
#include ".\al\REG_servo.h"
#include ".\al\REG_periph.h"
#include ".\al\REG_dvd_decoder.h"
#include ".\al\REG_bd_codec.h"
#include ".\al\REG_write_strategy.h"
#include ".\al\REG_servodsp.h"
#include ".\al\reg_intctrl.h"
#if (BD_ENABLE == 1)
#include ".\al\REG_bd_codec.h"
#include ".\al\hal_BD_CODEC.h"
#endif //(BD_ENABLE == 1)

#include ".\al\hal_afe.h"
#include ".\al\hal_dfe.h"
#include ".\al\hal_global.h"
#include ".\al\hal_lasc.h"
#include ".\al\hal_pll.h"
#include ".\al\hal_servo.h"
#include ".\al\hal_wobble.h"
#include ".\al\lal.h"
#include ".\al\lal_dfe.h"
#include ".\al\coefs.h"
#include ".\AL\REG_ssi.h"
#include ".\servo\svo_dsp.h"

#include ".\servo\svoMacro.h"
#include ".\common\constant.h"

#if (EN_LS == 1)
#include ".\LS\ls_svo.h"
#endif // End of (EN_LS == 1)

#if  SERVOKEYPARAMETER == 1

extern BYTE ServoParaSwitch;
extern BYTE VendorServoParaSwitch;
#endif
extern void set_uart_trap(void);
extern SBYTE  GuardRevolution10x(ULONG TargetID);
//extern vBYTE Iram_WGateUp;

#if (SET_SLOT_IN == 1)
	extern USHORT slot_level_check; //[Q36] for SLT002 disc in/out test
#endif

void apply_FEBC(SHORT valFEBC);
void WtLPP_Adj_Slice(void);

#include ".\servo\dvsvo_tbl.h"
#if (DVD_RAM_READ == 1)
#include ".\servo\dvRamSvo_tbl.h"
#endif

#include ".\common\intctl.h"   // TBD? should this still be here?

// PLL Check State variables
#define MAX_NR_RF_LOCK_TRY_OUT          3
BYTE NrTryOutCheckRfPll;
lalDFE_DFEEqualizerStrength_t mEqSettings;
#if (EN_WOBBLE == 1)
#define MAX_NR_WBL_LOCK_TRY_OUT         10
#define MAX_NR_SYNC_FLYWHEEL_TRY_OUT    10
BOOL StateCheckWobblePll;
BYTE NrTryOutCheckWobblePll;
BYTE NrTryOutSyncFlyWheel;
#endif // (EN_WOBBLE == 1)

#if DDisc ==1
BYTE   DualDisc = 0; //0:init, 1:DVD STM ok and possible dualdisc, 2:
#endif

#if (DO_HW_SUPPORT_STEP == 1)
extern BOOL StepMotorSupport_seek(SHORT, SHORT);
extern vULONG   Iram_system_timer;
#endif

private BYTE    svoPrvtVar_atipErrorCount;      // atip_error_count;

#define STM_BD  0
#define STM_DVD 1
#define STM_CD  2
static struct    svoSTMVarT      svoSTMVar;

#define DESIRED_RF_TOP  700 //mv
#define DESIRED_RF_AMP 1400 //mv

int UP_LPP_Slvel=0;
int NRUP_LPP_Slvel=0;
int WtNRUP_LPP_Slvel=0;
int WtUP_LPP_Slvel=0;
int WtLPP_Dec_ok=0;
int WtLPP_Decing=0;
int WtLPP_Er_large;

#if (SERVO_DEBUG_VARIABLE != 0)
vLONG servo_debug[SERVO_DEBUG_VARIABLE];
#endif

#if (SUN710_TRAY_PROFILE == 1) || ( EEPROM_TRAY_PROFILE ==1)
BYTE fTuneMode;
#endif
#if (SRC_RESET_DC_LPF == 1)
BYTE  SRC_need_ON=0;
#endif // (SRC_RESET_DC_LPF == 1)
extern  BYTE   Packet_Track;
#if DEFECT_LENGTH_MEASURE == 1
extern BYTE CD_Scratch_Disc;//  == modify for SCD-2383
#endif
#if (HORROR_DISC_IMPROVEMENT == 1)
BYTE TryDFE_Param;//== modify for DQA Disc to retry DFE parameter
#endif
//for sledge jump accuracy verification
//#define SLEDGE_JUMP_ADJ                     // use servo_debug[1~4]
#ifdef SLEDGE_JUMP_ADJ
#define SLEDGE_JUMP_ADJ_MIN 1
#define SLEDGE_JUMP_ADJ_MAX 20
float addr_diff[SLEDGE_JUMP_ADJ_MAX - SLEDGE_JUMP_ADJ_MIN + 1];
ULONG test_cnt;
#endif

#if (SET_SLOT_IN == 1) //[Q36]
extern BOOL fg_R2Profile;
#endif
/*-----------------------------------------------------------------------------
 *
 * Module: state_cmd_init()
 * Description:
 *
 *----------------------------------------------------------------------------*/
seq_exit_t state_cmd_init(void)
{
    SendMsg80(DEBUG_CALIB_ENTRYS_MSG,0x3A0100); //>>> STATE_CMD_INIT <<<

    svoSetServoOff();                   /* set all servo off */

    svo_Adj_Data_Clr();
    HAL_LASC_SetPdic();
#if (EN_ADI == 1)
    servo_status = 0;
    #if (EN_ADI == 1)
    HAL_DSP_ACTDISSConfig();
    #endif
#endif

    svoStp_Init();
    SetMotorDriverState(SHORT_BRAKE);

    WRITE_DSP_COEF(dmfbk_level, (USHORT)((SHORT)DISC_BRAKE_LEVEL*DMODAC_FACTOR));

    WRITE_DSP_COEF(kdf14,DISC_STOP_BRAKE_FORCE3);

    svoIramVar.spindleStopChkCnt = 0;
    svoIramVar.spindleStopFlag = DSTOPPING;
    spdSetCAVM(0);
    svoSpeedVar.fgCount = 0;
    StartFGTimer();

    svoVar_getDVDFormatflag = 0;

#if (LCD_SWITCH == 1)
    #if (PCB == 691)
    opMIO2 = 1; //opRSTB //reset LCD value
    #endif
#endif


    set_SIF(STATE,svoStInitialized);    /* set state "svoStInitialized" */

    SetDelayTimer(ARG1);

#if (CHIP_REV <= 0xB3)
    AfeMUXOUT(AFE_MUXOUT1, halAFE_MUX1SEL_TE);
    AfeMUXOUT(AFE_MUXOUT2, halAFE_MUX2SEL_FE);
    AfeMUXOUT(AFE_MUXOUT3, halAFE_MUX3SEL_BS);
#else
    AfeMUXOUT(AFE_MUXOUT1, halAFE_MUX1SEL_TE, 0);
    AfeMUXOUT(AFE_MUXOUT2, halAFE_MUX2SEL_FE, 0);
    AfeMUXOUT(AFE_MUXOUT3, halAFE_MUX3SEL_BS, 0);
#endif

#ifdef JTT
    AfeMUXOUT(AFE_MUXOUT1, (unsigned char)(READ_ADR_2B(0xb00009d0)));
    AfeMUXOUT(AFE_MUXOUT2, (unsigned char)(READ_ADR_2B(0xb00009d4)));
    AfeMUXOUT(AFE_MUXOUT3, (unsigned char)(READ_ADR_2B(0xb00009d8)));
#endif
#ifdef SLEDGE_JUMP_ADJ
    servo_debug[1] = SLEDGE_JUMP_ADJ_MIN;
#endif

    return(SEQ_EX0);
}

/*-----------------------------------------------------------------------------
 *
 * Module: state_disc_fg_chk()
 * Description:
 *
 *----------------------------------------------------------------------------*/
seq_exit_t state_disc_fg_chk(void)
{
    if (svoIramVar.svoPreInState != svoIramVar.svoInState)
    {
        SetTimeout(0,ARG1);
    }

    if (timeout_chk(0) == eTimerTimeout)
    {
        SetMotorDriverState(ALL_MUTE_ON);
        svoIramVar.spindleStopFlag = DSTOPPED;
        StopFGTimer();
        return(SEQ_EX1);
    }

    if (svoSpeedVar.fgPulseTime != 0xFFFF)
    {
        SetTimeout(0, TIMER_STOP);
        svoIramVar.spindleStopFlag = DSTARTED;
        return(SEQ_EX0);
    }
    return(SEQ_BUSY);
}

/*-----------------------------------------------------------------------------
 *
 * Module: state_init_end()
 * Description:
 *
 *----------------------------------------------------------------------------*/
seq_exit_t state_init_end(void)
{
    StopFGTimer();
    return(SEQ_EX0);
}


/*-----------------------------------------------------------------------------
 *
 * Module: state_step_init_chk()
 * Description:
 *
 *----------------------------------------------------------------------------*/
seq_exit_t state_step_init_chk(void)
{
    SendMsg80(DEBUG_CALIB_ENTRYS_MSG,0x3A0101); //>>> STATE_STEP_INIT_CHK <<<

    if ((svoCalVar.calGainDoneBits & feg_bsg_end) == feg_bsg_end)
    {
        if (Layer_Index == 1)
        {
            calUpdateLayer(0);
        }
        svoSeekVar.svoTargetID.Layer = 0;
    }
    svoSetLDOff();
#if (BD_ENABLE == 1)
    if (svo_BDR_RE_PICReading == SET)
    {
        //Exiting the PIC zone
        SendMsg80(SHOW_DEBUG_MSG,0x334700);
        svo_BDR_RE_PICReading = CLEAR;
        LAL_PLL_SetPll();
        LAL_SetResetPIC(eRESET_PIC, svo_bReadZoneSpeedX10);
    }
#endif
    SetMotorDriverState(SHORT_BRAKE);

#if DO_HW_SUPPORT_STEP == 1
    //---------------------------------------------------------------;
    //---------------------------------------------------------------;
    // Call from     PIC_HOME_TBL                                    ;
    //---------------------------------------------------------------;
    //---------------------------------------------------------------;
    if (Iram_stp_Initialized == STEP_UNINITIALIZED)
    {
        //-----------------------------------------------------------;
        // move inward open loop for a distance                      ;
        //-----------------------------------------------------------;
        FillMicroSteppingTable(115);        // 45%   (unit 2^-8)
        WaitTimer(200);

        //-----------------------------------------------------------;
        // move to home position                                     ;
        //-----------------------------------------------------------;
        FillMicroSteppingTable(192);        // 192 --> or 75%   (unit 2^-8)
        if (StepMotorSupport_seek(-3700,    // distance and direction to go
                                    800)    // allow max speed               (unit in micro-step per 100 ms)
            == TRUE)
        {
            Iram_stp_Initialized = STEP_INITIALIZED;
        }
    }// if( Iram_stp_Initialized == STEP_UNINITIALIZED )

    FillMicroSteppingTable(0);              // 256 --> 1.0 or 100%   (unit 2^-8)
    return(SEQ_EX0);

#else // DO_HW_SUPPORT_STEP == 1
    #if (NO_FRONT_PANEL == 0)
    if (ipCloseSW == 1)
    {
        return(SEQ_EX0);
    }
    #endif

    if (Iram_stp_Initialized == STEP_UNINITIALIZED)
    {
#if 0 //to verify sledge control module
        {
            ULONG i;

            for (i = 0; i < 50000; i++)
            {
                svoStp_verify();
                send_msgA4(1,9,0,0,i);
            }
        }
#endif
        svoStp_Homing();
    }
    else
    {
        svoStp_JumpToPosition(STEP_SAFE_SLOW_MODE, svoStepVar.StepHomePos);
    }

    return(SEQ_EX0);
#endif // DO_HW_SUPPORT_STEP == 1
}

/*-----------------------------------------------------------------------------
 *
 * Module: state_step_home_chk()
 * Description:
 *
 *----------------------------------------------------------------------------*/
seq_exit_t state_step_home_chk(void)
{
#if DO_HW_SUPPORT_STEP == 1
    //---------------------------------------------------------------;
    // works are done inside state_step_init_chk                     ;
    //---------------------------------------------------------------;
    svoIramVar.currentID.SectorNum.all32s = svoRadius2PBA(Iram_stp_CurrentPosition, 0);
    return(SEQ_EX0);
#else // DO_HW_SUPPORT_STEP == 1
    USHORT target_pos;

    if ((svoCalVar.discChkDoneBits & blank_chk_end) == blank_chk_end)
    {
        target_pos = Recorded_stp_Position[Layer_Index];
    }
    else
    {
        target_pos = svoStepVar.StepHomePos;
    }

    if (!svoStp_busy())
    {
        if (ABS((SHORT)Iram_stp_CurrentPosition - target_pos) < 32)//less than 1 full step
        {
            svoStp_StopControl();
            HAL_SERVO_CenterControlOFF();
            svoIramVar.currentID.SectorNum.all32s = svoRadius2PBA(Iram_stp_CurrentPosition, 0);
            return(SEQ_EX0);
        }
        else
        {
            svoStp_JumpToPosition(STEP_SAFE_SLOW_MODE, target_pos);
        }
    }

    SetDelayTimer(ARG1);
    return(SEQ_BUSY);
#endif // DO_HW_SUPPORT_STEP == 1
}

/*-----------------------------------------------------------------------------
 *
 * Module: state_diaCmd_sled_move()
 * Description:
 *              ARG1:direction
 *              ARG2:timeout
 *
 *----------------------------------------------------------------------------*/
seq_exit_t state_diaCmd_sled_move(void)
{
    USHORT target_pos;

    SetMotorDriverState(SHORT_BRAKE);
    if (ARG1 == FORW)
    {
        //Jump to the out most position without hitting the wall
        target_pos = (STEP_OUTWARD_POSITION + HOMING_DISTANCE - 384);
    }
    else
    {
        //radius 23mm should be reachable for every loader
        target_pos = UM_2_SLED_STEP(23000);
    }
    svoStp_JumpToPosition(STEP_SAFE_SLOW_MODE, target_pos);

    SetTimeout(0, ARG2);
    while(svoStp_busy());
    {
        if (timeout_chk(0) == eTimerTimeout)   /* time out check */
        {
            svoDiagStatus = 0x80;
            return(SEQ_EX1);
        }
    }

    svoStp_StopControl();
    SetMotorDriverState(ALL_MUTE_ON);
    svoDiagStatus = 0x00;
    SetTimeout(0, TIMER_STOP);
    return(SEQ_EX0);
}

#if (CALIBRATION_MANAGEMENT == 1)
/*-----------------------------------------------------------------------------
 *
 * Module: state_at_layer_0()
 * Description:return SEQ_EX0 if at layer 0
 *             return SEQ_EX1 if at layer 1
 *
 *----------------------------------------------------------------------------*/
seq_exit_t state_at_layer_0(void)
{
    if (Layer_Index == 0)
    {
        return(SEQ_EX0);
    }
    else
    {
        return(SEQ_EX1);
    }
}
#else // (CALIBRATION_MANAGEMENT == 1)
BYTE DPDCTFFC[5] = {
    CAL_DPDCTFFC_TEST_POINT0,
    CAL_DPDCTFFC_TEST_POINT1,
    CAL_DPDCTFFC_TEST_POINT2,
    CAL_DPDCTFFC_TEST_POINT3,
    CAL_DPDCTFFC_TEST_POINT4
};

/*-----------------------------------------------------------------------------
 *
 * Module: state_caltilt_stepmove1()
 * Description:
 *
 *----------------------------------------------------------------------------*/
seq_exit_t state_caltilt_stepmove1(void)
{
    if (svoVar_TiltStepperZone > 0)
    {
        svoStp_JumpToPosition(STEP_SAFE_SLOW_MODE, CalTestPos[svoVar_TiltStepperZone]);
    }
    return(SEQ_EX0);
}

/*-----------------------------------------------------------------------------
 *
 * Module: state_caltilt_stepmove2()
 * Description:
 *
 *----------------------------------------------------------------------------*/
seq_exit_t state_caltilt_stepmove2(void)
{
    if (!svoStp_busy())
    {
        HAL_SERVO_CenterControlOFF();

        /*----- DPD block set up ------------------*/
        /*  Set the DPD Bw and HPF corner freq. */
        /* !!! need convertion of speed to OverspeedX10 !!! */
        SetDelayTimer(MS_100);
        return(SEQ_EX0);
    }

    SetDelayTimer(ARG1);
    return(SEQ_BUSY);
}

/*-----------------------------------------------------------------------------
 *
 * Module: state_caltilt_check()
 * Description:
 *
 *----------------------------------------------------------------------------*/
seq_exit_t state_caltilt_check(void)
{
    svoVar_TiltStepperZone++;

    if ((Layer_Index == 1) || ((svoVar_TiltStepperZone == 1) && (svoVar_discDiameter == e8cm))) //8cm disc only calibrate 1 point.
    {
        svoVar_TiltStepperZone = 5;
    }

    if (svoVar_TiltStepperZone == 5)
    {
        svoVar_TiltStepperZone = 0;
        svoStp_JumpToPosition(STEP_SAFE_SLOW_MODE, CalTestPos[svoVar_TiltStepperZone]);
        /*----- DPD block set up ------------------*/
        /*  Set the DPD Bw and HPF corner freq. */
        /* !!! need convertion of speed to OverspeedX10 !!! */

        return(SEQ_EX0);
    }
    else
    {
        return(SEQ_EX1);
    }
}
#endif // CALIBRATION_MANAGEMENT


/*-----------------------------------------------------------------------------
 *
 * Module: state_aj_init()
 * Description:
 *
 *----------------------------------------------------------------------------*/
seq_exit_t state_aj_init(void)
{
    SendMsg80(DEBUG_AUTO_TUNING_MSG,0x316100); //***AJINIT_TBL***
    SendMsg80(DEBUG_CALIB_ENTRYS_MSG,0x3A0105); //>>> STATE_AJ_INIT <<<

    if ((svoCalVar.calGainDoneBits & feg_bsg_end) == feg_bsg_end)
    {
        if (Layer_Index == 1)
        {
            calUpdateLayer(0);
        }
    }

#if(ERROR_CAL_START_UP_MONITOR ==1)
    svoMiscVar.calibrationErrorCount = 0;
#endif

    svoSetLDOff();
    HAL_LASC_SetPdic();
    SetMotorDriverState(SHORT_BRAKE);

#if (PCB != 691)
    if ((svoVar_discKindT.Bit.Media == eMediaRW) || (svoVar_discKindT.Bit.Media == eMediaRAM))
    {
        HAL_LASC_SetOscen(OSCEN_RWDISC);
    }
    else
    {
        HAL_LASC_SetOscen(OSCEN_RDISC);
    }
#endif // (PCB != 691)

    WRITE_FIELD(HAL_GLOBAL_WBLSRST, FALSE);  // turn off Write strategy reset bit, so that wgate is controllable
    HAL_DFE_EnDisBlankDetection(halDFE_BlankDetectDisable);     /* Disable blank detection */
    /*disable holding FE signal during defect */
    /* Disable black and white defect */
    /* servo defect off */
    Defect_Enable(DEFECT_SOURCE_UNFREEZE|DEFECT_STATE_ALL_OFF|DEFECT_SOURCE_SERVO_BS);
#if (USE_BS_DEFECT_FOR_STARTUP==1)
    if(svoVar_discKindT.Bit.DiscStd != eDiscCD)
        Defect_Enable(DEFECT_SOURCE_FREEZE_BS);
#endif
    SendMsg80(DEBUG_AUTO_TUNING_MSG,0x315402); //Preset Defect detection disable
#if (AFE_POWER_DOWN == 1)
    AFEPowerOn();
#endif

    return(SEQ_EX0);
}

/*-----------------------------------------------------------------------------
 *
 * Module: state_dspofs_set()
 * Description: set DSP offsets
 *
 *----------------------------------------------------------------------------*/
seq_exit_t state_dspofs_set(void)
{
    BYTE  i;
#if (OPU_BEAM_LANDING_MEAS == 1)
    ad_dsp_t signal_ret;
#endif

    SendMsg80(DEBUG_CALIB_ENTRYS_MSG,0x3A010A); //>>> STATE_DSPOF_INIT <<<

    if ((svoCalVar.calOfsDoneBits & ofaj_end) == 0)
    {
        for (i = (BS_GN + 1); i < MAX_TYPE2_GN_NO; i++)//Note: L1 BS_GN have been calibrated
        {
            svoCalGnT2Var[1][i] = svoCalGnT2Var[0][i];
        }

        for (i = 0; i < MAX_TYPE1_OFS_NO; i++)
        {
            svoCalOfsT1Var[1][i] = svoCalOfsT1Var[0][i];
        }

        for (i = 0; i < MAX_TYPE2_OFS_NO; i++)
        {
            svoCalOfsT2Var[1][i] = svoCalOfsT2Var[0][i];
        }

        for (i = 0; i < MAX_TYPE3_OFS_NO; i++)
        {
            svoCalOfsT3Var[1][i] = svoCalOfsT3Var[0][i];
        }

#if (DVD_RAM_READ == 1)
        // Force LAND to low to prevent from TE polarity change.
        if (DVDRAM)
        {
            WRITE_FIELD(HAL_AFE_LANDSEL,0);
        }
#endif

        SendMsgCn(SHOW_DEBUG_MSG,10,0x3165F1,A2B(svoCalVar.ceOfsData.all16),
            A2B(svoCalVar.feOfsData.all16),A2B(svoCalVar.teOfsData.all16),
            A2B(svoCalVar.rpOfsData.all16),A2B(svoCalVar.bsOfsData.all16));

        svoCalVar.calOfsDoneBits |= ofaj_end;
#if (OPU_BEAM_LANDING_MEAS == 1)
        signal_ret = readSegmentABCD(NO_REVOLUTION,FALSE);
        svoBeamLandingVar.segment_ref_mv[0] = signal_ret.middle_mv;//A
        svoBeamLandingVar.segment_ref_mv[1] = signal_ret.p2p_mv;   //B
        svoBeamLandingVar.segment_ref_mv[2] = signal_ret.top_mv;   //C
        svoBeamLandingVar.segment_ref_mv[3] = signal_ret.bot_mv;   //D
#endif
    }

    WRITE_REG(HAL_SRVREG_MAP->RGSEL, halSRV_RSEL_FEO);  /* write Focus Offset Data data */
    WRITE_REG(HAL_SRVREG_MAP->DASET, svoCalVar.feOfsData.all16);

    WRITE_REG(HAL_SRVREG_MAP->RGSEL, halSRV_RSEL_TEO);  /* write Tracking Offset Data data */
    WRITE_REG(HAL_SRVREG_MAP->DASET, svoCalVar.teOfsData.all16);

    WRITE_REG(HAL_SRVREG_MAP->RGSEL, halSRV_RSEL_RPO);  /* write Ripple Offset Data data */
    WRITE_REG(HAL_SRVREG_MAP->DASET, svoCalVar.rpOfsData.all16);

    WRITE_REG(HAL_SRVREG_MAP->RGSEL, halSRV_RSEL_BSO);  /* write Beam Strength Offset Data data */
    WRITE_REG(HAL_SRVREG_MAP->DASET, svoCalVar.bsOfsData.all16);

    return(SEQ_EX0);
}

/*-----------------------------------------------------------------------------
 *
 * Module: state_focus_init()
 * Description:
 *
 *----------------------------------------------------------------------------*/
seq_exit_t state_focus_init(void)
{
    SendMsg80(DEBUG_CALIB_ENTRYS_MSG,0x3A010B); //>>> STATE_FOCUS_INIT <<<

    /* Do not use DC filter for focus/tracking error signal when detecting defects */
    Defect_Enable(DEFECT_STATE_ALL_OFF);
                                            /* normal slew rate, No gain-up */
    svoMiscVar.focusRampCount = 0;          /* set # of times (up and down) to do focus search */
    svoMiscVar.focusRetryCount = 0;
    HAL_SERVO_SetFebc(0);    //no in focus close loop, set FEBC to 0 directly.

    GAIN_RECD_HIGH;

#if (SERVICE_MODE==1)
    if (SIF.ServoState == svoStServiceMode) set_SIF(EXECUTIONSTATE, Ready);
#endif

    svoGetIDErrCount = 0;

    return(SEQ_EX0);
}

/*-----------------------------------------------------------------------------
 *
 * Module: state_nodisc()
 * Description:
 *
 *----------------------------------------------------------------------------*/
seq_exit_t state_nodisc(void)
{
    nodisc_set();
    HAL_LASC_SetPdic();   /* set ATTON/GAINUP/OPU GAINSW */
    svoSetServoOff();
#if (AFE_POWER_DOWN == 1)
    AFEPowerDown();
#endif
    set_SIF(STATE, svoStIdleDiscUnknown);   /* set state "svoStIdleDiscUnknown" */

    return(SEQ_EX0);
}

/*-----------------------------------------------------------------------------
 *
 * Module: state_unknowndisc()
 * Description:
 *
 *----------------------------------------------------------------------------*/
seq_exit_t state_unknowndisc(void)
{
    if (svoVar_discKindT.fDiscKind != eDVDRAMDisc)
        svoVar_discKindT.fDiscKind = eUnknownKind;
    HAL_LASC_SetPdic();
    svoSetServoOff();
#if (AFE_POWER_DOWN == 1)
    AFEPowerDown();
#endif

    set_SIF(STATE, svoStIdleDiscUnknown);   /* set state "svoStIdleDiscUnknown" */
    svoCmdVar.discError = eErrorUnknownDisc;
    svoCmdVar.svoCommand = CNOP;
    set_SIF(EXECUTIONSTATE,svoError);       /* set ExecutionState "svoError" */

#if (Customer_A1 ==1)
		CompleteStroke = TRUE;
#endif

    return(SEQ_EX0);
}

/*-----------------------------------------------------------------------------
 *
 * Module: state_cal_ramp_down()
 * Description: return SEQ_EX1 if all offset calibrations (focus off) are done
 *
 *----------------------------------------------------------------------------*/
seq_exit_t state_cal_ramp_down(void)
{
    if ((svoCalVar.calOfsDoneBits & (havcof_end|mbeamof_end|sbeamof_end|feof_end|teof_end|bsof_end|rpof_end|ceof_end|phbhof_end|rfof_end)) ==
        (havcof_end|mbeamof_end|sbeamof_end|feof_end|teof_end|bsof_end|rpof_end|ceof_end|phbhof_end|rfof_end))
    {
        return(SEQ_EX1);
    }
    return(SEQ_EX0);
}

/*-----------------------------------------------------------------------------
 *
 * Module: state_focus_move()
 * Description: ARG1: ramp limit position (mSec)
 *              ARG2: 0(OFF)    : No Focus ON;
 *                    1(ON)     : Focus ON;
 *                    2(PP_MEAS): No Focus ON and measure FE P2P
 *              ARG3: Up/Down
 *
 *----------------------------------------------------------------------------*/
seq_exit_t state_focus_move(void)
{
    BYTE    temp,temp_FSL,first_layer;
    SHORT   temp_kf18,timeout,foo_dac;
#if (FocusLockAid_Enable == 1)
    LONG    fcs_gain;
#endif

    Defect_Enable(DEFECT_STATE_ALL_OFF);

#if (BD_ENABLE == 1)
    if (svoVar_discKindT.Bit.DiscStd == eDiscBD)
    {//BD layer 1 is below layer 0
        first_layer = (ARG3 == UP)? 1 : 0;// fix bugs in FEpp measurement
    }
    else
#endif
    {
        first_layer = (ARG3 == UP)? 0 : 1;// fix bugs in FEpp measurement
    }

#ifdef DBG_FEP2P_USE_SLEWRATE
    if (ARG2 == PP_MEAS)
    {
        for (DebugFEOCnt = 0; DebugFEOCnt < 200; DebugFEOCnt++)
        {
            DebugFEORaw[DebugFEOCnt] = 0;
            DebugFEOSlew[DebugFEOCnt] = 0;
            DebugFEOStage[DebugFEOCnt] = 0;
        }
        DebugFEOCnt = 0;
    }
#endif

#if (DVD_RAM_READ == 1)
    DISABLE_HDR_HOLD_SERVO;
#endif

    temp = ARG2;
    //force DL disc to focusing-on on layer0 during startup focus search
#if (FOCUS_LOCK_FROM_DOWN_TO_UP == 1)
    if ((temp == ON) && (svoVar_discKindT.Bit.Layers == eLayersDouble) && (svoVar_discKindT.Bit.DiscStd != eDiscBD) && (!(svoCalVar.discChkDoneBits & start_end)))
#else
    if ((temp == ON) && ((svoVar_discKindT.Bit.Layers == eLayersDouble) && ((svoVar_discKindT.Bit.DiscStd == eDiscBD) || (!(svoCalVar.discChkDoneBits & start_end)))))
#endif
    {
        if (first_layer != Layer_Index)
        {
            temp = OFF;
        }
    }
    temp_kf18 = FOCUS_SEARCH_KF18;

    if (temp == ON)
    {
        //make sure LD is ON to avoid recovery fail//from 720
        WRITE_REG(HAL_SRVREG_MAP->LDON, 0x00);

        if (svoMiscVar.focusRetryCount)
        {
            //reduce ramping velocity when focus catch fail, minimum to 1/3
            temp_kf18 = (LONG)temp_kf18 * (3 * FOCUS_RETRY - 2 * svoMiscVar.focusRetryCount) / (3 * FOCUS_RETRY);
        }
        temp = HAL_SRV_FSONSET | HAL_SRV_FGCSET;    /* set focus servo ON operation */
    }
    else if (temp == OFF)
    {
        temp = HAL_SRV_HISRSET;                     /* No focus servo ON operation & faster*/
    }
    else
    {
        temp = 0x00;                /* No focus servo ON operation */
    }

    foo_dac = ramp_stop();
    if (READ_DSP_COEF(kf18) != temp_kf18)
    {
#if (CHIP_REV >= 0xC0)
        HAL_SERVO_SetFcs((foo_dac - HAL_SERVO_GetFoos()) * 0x8000 / temp_kf18);
        WRITE_FIELD(HAL_SRV_FCSSBYEN,0);
        WRITE_FIELD(HAL_SRV_FCSSBYEN,1);
#else
    #if (FOOFFSET2_ENABLE == 1)
        WRITE_REG(HAL_SRVREG_MAP->LDOFF,0x00);
        WRITE_REG(HAL_SRVREG_MAP->LDON, 0x00);
    #endif
#endif
        WRITE_DSP_COEF(kf18,temp_kf18);
    }

    timeout = ARG1 * (FOCUS_SEARCH_KF18 * FOC_RAMP_FACTOR / 0x20) / 0x20000;

    //reduce FSL when S-curve not found, minimum to 2/3
#if (BD_ENABLE == 1)
    temp_FSL = (((svoVar_discKindT.Bit.DiscStd == eDiscBD) && (ARG3 == UP))? FOCUS_CAPTURE_FSL_BD : FOCUS_CAPTURE_FSL_CD_DVD) * ((FSRCH_RETRY * 3) - svoMiscVar.focusRampCount) / (FSRCH_RETRY * 3);
#else
    temp_FSL = FOCUS_CAPTURE_FSL_CD_DVD * ((FSRCH_RETRY * 3) - svoMiscVar.focusRampCount) / (FSRCH_RETRY * 3);
#endif

    if (ARG3 == UP)
    {
        HAL_SERVO_SetFSL(temp_FSL, 0x7F);
        HAL_SERVO_SetFocUp(temp);    /* Lens up */
        timeout = svoCalMisc2Var.fooos + timeout;
        timeout = (timeout > foo_dac)? timeout - foo_dac : 0;
        svoRampMeasVar.pre_timeout = ARG1 * 2 * FOCUS_SEARCH_KF18 / temp_kf18 + 1;
    }
    else
    {
        HAL_SERVO_SetFSL(0x7F, temp_FSL);
        HAL_SERVO_SetFocDn(temp);    /* Lens down */
        timeout = svoCalMisc2Var.fooos - timeout;
        timeout = (foo_dac > timeout)? foo_dac - timeout : 0;
    }

#if (FocusLockAid_Enable == 1)
    #if (DVD_RAM_READ == 1) || (ENABLE_DVDRAM_DETECTION == 1)
    // DVD-RAM header noise on FE causes abnormal pulses on FOO if focus-lock-aid is ON.
    // To prevent from abnormal pulses on FOO (bug# 5482):
    // (a) Turn focus-lock-aid off during focus-search if a DVD disc is loaded.
    // (b) Turn focus-lock-aid off all time if a DVD-RAM disc is loaded.
    if ((temp & HAL_SRV_FSONSET) && (!DVDRAM) && (!POSSIBLE_DVDRAM))
    {
        HAL_DSP_FocusLockAid(ON, ARG3);
    }
    #else
    HAL_DSP_FocusLockAid((temp & HAL_SRV_FSONSET)? ON : OFF, ARG3);
    #endif
    if (svoMiscVar.focusRetryCount >= (FOCUS_RETRY / 2))
    {//increase focus lock aid gain after few retries
        fcs_gain = READ_DSP_COEF(fcs_vel_gain);
        fcs_gain = fcs_gain * 2;
        if (fcs_gain > 32767) fcs_gain = 32767;
        if (fcs_gain < -32768) fcs_gain = -32768;
        WRITE_DSP_COEF(fcs_vel_gain,fcs_gain);
    }
#else
    HAL_DSP_FocusLockAid(OFF, ARG3);
#endif // #if (FocusLockAid_Enable == 1)

    timeout = ((timeout << 22) / (temp_kf18 * FOC_RAMP_FACTOR)) + 1;
    if (temp == HAL_SRV_HISRSET)
    {
        timeout = (timeout + 1) / 2;
        svoRampMeasVar.pre_timeout = (svoRampMeasVar.pre_timeout + 1) / 2;
    }

    if (ARG2 != PP_MEAS)
    {
        SetTimeout(0,timeout);
        WRITE_FIELD(HAL_SRV_PKBOT,0);
        WRITE_FIELD(HAL_SRV_PKBOT,1);
    }
    else
    {
#if (FE_SLEW_RATE_LIMITER == 1)
        if (svoVar_discKindT.Bit.DiscStd == eDiscDVD)
        {
            WRITE_FIELD(HAL_AFE_MUX2ADSEL, halAFE_MUX2ADSEL_FE);
        }
        else
#endif
        {
            WRITE_FIELD(HAL_AFE_MUX2ADSEL, halAFE_MUX2ADSEL_FEO);
        }
        WRITE_FIELD(HAL_AFE_MUX1ADSEL, halAFE_MUX1ADSEL_SRFO);

        svoRampMeasVar.refFE = 0;
        for(temp = 0; temp < 32; temp++)
        {
            WRITE_REG(HAL_SRVREG_MAP->RGSEL, halSRV_RSEL_FE);      /* read FE data */
            svoRampMeasVar.refFE += READ_REG(HAL_SRVREG_MAP->SRCR);
        }
        svoRampMeasVar.refFE = RoundDivide(svoRampMeasVar.refFE, temp);
#if (STREAM_STM == 0)
        svoRampMeasVar.refFEO = (((ReadAverageAdcmV(ADC_MUXOUT2,7) - GPADC_IN_MIN) << GPADC_BITS) + GPADC_IN_RANGE / 2) / GPADC_IN_RANGE;
#else
        svoRampMeasVar.refFE = (((ReadAverageAdcmV(ADC_FE,7) - GPADC_IN_MIN) << GPADC_BITS) + GPADC_IN_RANGE / 2) / GPADC_IN_RANGE;
        svoRampMeasVar.refBS = (((ReadAverageAdcmV(ADC_BS,7) - GPADC_IN_MIN) << GPADC_BITS) + GPADC_IN_RANGE / 2) / GPADC_IN_RANGE;
#endif
        svoRampMeasVar.refSRFO = (((ReadAverageAdcmV(ADC_MUXOUT1,7) - GPADC_IN_MIN) << GPADC_BITS) + GPADC_IN_RANGE / 2) / GPADC_IN_RANGE;

#if (MEAS_SUBSTRATE_P2P == 1)
        if ((ARG3 == UP) && (svoPrvtVar_focusP2PCount > 0))
        {
            WRITE_FIELD(HAL_SRV_PKBOT,0);
            WRITE_FIELD(HAL_SRV_PKBOT,1);
            WaitTimer(100);
            WRITE_FIELD(HAL_SRV_PKBOT,0);

            WRITE_REG(HAL_SRVREG_MAP->RGSEL,halSRV_RSEL_FEBOTTOM);    /* read Bottom data */
            fe_bottom = READ_REG(HAL_SRVREG_MAP->SRCR);
            WRITE_REG(HAL_SRVREG_MAP->RGSEL,halSRV_RSEL_FEPEAK);  /* read Peak data */
            // SU: %d, %d
            SendMsgCnB(DEBUG_SEEK_MSG,2,0x3FAA0D,B1B(bFocMoveReference-fe_bottom),B1B(READ_REG(HAL_SRVREG_MAP->SRCR)-bFocMoveReference));
        }
#endif
        svoPrvtVar_focusP2PCount++;
#if (STREAM_STM == 0)
        RampMeas(eRampFEpp, timeout, ARG3, svoVar_discKindT);
#else
        RampStream(eRampFEpp,timeout, ARG3, svoVar_discKindT);
#endif

        //hold lens at current height before ExitProcess()
        ramp_stop();

        if (svoRampMeasVar.FEpp[0] == 0)
        {
            svoPrvtVar_focusP2PCount--;
            cal_retry++;
            SendMsgCn(SHOW_DEBUG_MSG,1,0x315010,A1B(cal_retry)); // FE_PP_MEASURE retry ==> %d
        }
        else
        {
            if (svoRampMeasVar.FEpp[1] > 0)
            {
                if (svoVar_discKindT.Bit.DiscStd == eDiscCD)
                {
                    if (svoRampMeasVar.FEpp[1] > svoRampMeasVar.FEpp[0])
                    {
                        svoRampMeasVar.FEpp[0] = svoRampMeasVar.FEpp[1];
                        svoRampMeasVar.BSpp[0] = svoRampMeasVar.BSpp[1];
                    }
                    svoRampMeasVar.FEpp[1] = 0;
                    svoRampMeasVar.BSpp[1] = 0;
                }
            }

            if (svoVar_discKindT.Bit.Layers == eLayersSingle)
            {
                if (svoRampMeasVar.FEpp[1] > 0)
                {
                    if (svoRampMeasVar.FEpp[1] > svoRampMeasVar.FEpp[0])
                    {
                        svoRampMeasVar.FEpp[0] = svoRampMeasVar.FEpp[1];
                        svoRampMeasVar.BSpp[0] = svoRampMeasVar.BSpp[1];
                    }
                    bLayerErrorCount++;
                    // FE_PP_MEASURE wrong layer detected (ErrorCount = %d)
                    SendMsgCn(SHOW_DEBUG_MSG,1,0x315012,A1B(bLayerErrorCount));
                    // FE_PP_SL_wrong_layer: <L0 / 1> FEpp = %4d (mV), BSpp = %3d (mV), SRFO = %3d (mV)
                    SendMsgCnB(DEBUG_AUTO_TUNING_MSG,6,0x315018+first_layer,B2B(((ULONG)svoRampMeasVar.FEpp[0] * DSPADC_IN_RANGE)>> DSPADC_BITS),
                                                                            B2B(((ULONG)svoRampMeasVar.BSpp[0] * DSPADC_IN_RANGE) >> DSPADC_BITS),
                                                                            B2B(((ULONG)svoRampMeasVar.SRFOpp[0] * GPADC_IN_RANGE) >> GPADC_BITS));
                    // FE_PP_SL_wrong_layer: <L0 / 1> FEpp = %4d (mV), BSpp = %3d (mV), SRFO = %3d (mV)
                    SendMsgCnB(DEBUG_AUTO_TUNING_MSG,6,0x315018+(!first_layer),B2B(((ULONG)svoRampMeasVar.FEpp[1] * DSPADC_IN_RANGE)>> DSPADC_BITS),
                                                                               B2B(((ULONG)svoRampMeasVar.BSpp[1] * DSPADC_IN_RANGE) >> DSPADC_BITS),
                                                                               B2B(((ULONG)svoRampMeasVar.SRFOpp[1] * GPADC_IN_RANGE) >> GPADC_BITS));
                }

                if ((svoPrvtVar_maxFEpp[0] > 0) && ((svoPrvtVar_maxFEpp[0] * 2) <= svoRampMeasVar.FEpp[0]))
                {
                    svoPrvtVar_focusP2PCount = 0;
                    svoPrvtVar_avgFOO = 0;
                    svoPrvtVar_avgBSpp[0] = 0;
                    svoPrvtVar_avgSRFO[0] = 0;
                    svoPrvtVar_avgFEpp[0] = 0;
                    SendMsg80(SHOW_DEBUG_MSG,0x315014); //FE_PP_MEASURE clear old result
                }
                svoPrvtVar_avgFOO += svoRampMeasVar.LayerFoo;
                svoPrvtVar_avgSRFO[0] += svoRampMeasVar.SRFOpp[0];
                svoPrvtVar_avgBSpp[0] += svoRampMeasVar.BSpp[0];
                svoPrvtVar_avgFEpp[0] += svoRampMeasVar.FEpp[0];
#if (FE_SLEW_RATE_LIMITER == 2)
                    if (svoPrvtVar_focusP2PCount == 1)
                    {
                        SlewRateShift.bytes.byte0 = (svoPrvtVar_avgFEpp[0] * SlewRateShift.bytes.byte0  + 67)/ 133; //133 == 1250 mv
                    }
#endif
                if (svoPrvtVar_maxFEpp[0] < svoRampMeasVar.FEpp[0])
                {
                    svoPrvtVar_maxFEpp[0] = svoPrvtVar_maxFEpp[1] = svoRampMeasVar.FEpp[0];
                }
    #if (STREAM_STM == 0)
                // FE PP MEASURE: <L0> FEpp = %4d (mV), BSpp = %3d (mV), SRFO = %3d (mV)
                SendMsgCnB(DEBUG_AUTO_TUNING_MSG,6,0x315000,B2B(((ULONG)svoRampMeasVar.FEpp[0] * DSPADC_IN_RANGE)>> DSPADC_BITS),
                                                            B2B(((ULONG)svoRampMeasVar.BSpp[0] * DSPADC_IN_RANGE) >> DSPADC_BITS),
                                                            B2B(((ULONG)svoRampMeasVar.SRFOpp[0] * GPADC_IN_RANGE) >> GPADC_BITS));
    #else
                SendMsgCnB(DEBUG_AUTO_TUNING_MSG,6,0x315000,B2B(((ULONG)svoRampMeasVar.FEpp[0] * GPADC_IN_RANGE)>> GPADC_BITS),
                    B2B(((ULONG)svoRampMeasVar.BSpp[0] * GPADC_IN_RANGE) >> GPADC_BITS),B2B(((ULONG)svoRampMeasVar.SRFOpp[0] * GPADC_IN_RANGE) >> GPADC_BITS));
    #endif
#ifdef DBG_FEP2P_USE_SLEWRATE
                if (ARG2 == PP_MEAS)
                {
                    for (DebugFEOCnt = 0; DebugFEOCnt < 200; DebugFEOCnt++)
                    {
                        // "Debug:FEPPMes_Use_SlewLimit:FeoRaw[] = %d, FeoSlew[] = d, FeoStage[] = %d"
                       SendMsgCnB(DEBUG_AUTO_TUNING_MSG,6,0x315016,B2B(DebugFEORaw[DebugFEOCnt]),
                                                                   B2B(DebugFEOSlew[DebugFEOCnt]),
                                                                   B1B(DebugFEOStage[DebugFEOCnt]));
                    }
                }
#endif
            }
            else
            {
                if (svoRampMeasVar.FEpp[0] > 0)
                {
    #if (STREAM_STM == 0)
                    // FE PP MEASURE: <Lx> FEpp = %4d (mV), BSpp = %3d (mV), SRFO = %3d (mV)
                    SendMsgCnB(DEBUG_AUTO_TUNING_MSG,6,0x315000+first_layer,B2B(((ULONG)svoRampMeasVar.FEpp[0] * DSPADC_IN_RANGE)>> DSPADC_BITS),
                                                                            B2B(((ULONG)svoRampMeasVar.BSpp[0] * DSPADC_IN_RANGE) >> DSPADC_BITS),
                                                                            B2B(((ULONG)svoRampMeasVar.SRFOpp[0] * GPADC_IN_RANGE) >> GPADC_BITS));
    #else
                    SendMsgCnB(DEBUG_AUTO_TUNING_MSG,6,0x315000+first_layer,B2B(((ULONG)svoRampMeasVar.FEpp[0] * GPADC_IN_RANGE)>> GPADC_BITS),
                        B2B(((ULONG)svoRampMeasVar.BSpp[0] * GPADC_IN_RANGE) >> GPADC_BITS),B2B(((ULONG)svoRampMeasVar.SRFOpp[0] * GPADC_IN_RANGE) >> GPADC_BITS));
    #endif
                }

                if (svoRampMeasVar.FEpp[1] == 0)
                {
                    svoPrvtVar_focusP2PCount--;
                    cal_retry++;
                    if ((svoCalVar.discChkDoneBits & disc_chk_end) == 0)
                    {
                        bLayerErrorCount++;
                        // FE_PP_MEASURE wrong layer detected (ErrorCount = %d)
                        SendMsgCn(SHOW_DEBUG_MSG,1,0x315012,A1B(bLayerErrorCount));
                    }
                    SendMsgCn(SHOW_DEBUG_MSG,1,0x315010,A1B(cal_retry)); // FE_PP_MEASURE retry ==> %d
                    // FE_PP_DL_wrong_layer: <L0 / 1> FEpp = %4d (mV), BSpp = %3d (mV), SRFO = %3d (mV)
                    SendMsgCnB(DEBUG_AUTO_TUNING_MSG,6,0x31501A+first_layer,B2B(((ULONG)svoRampMeasVar.FEpp[0] * DSPADC_IN_RANGE)>> DSPADC_BITS),
                                                                            B2B(((ULONG)svoRampMeasVar.BSpp[0] * DSPADC_IN_RANGE) >> DSPADC_BITS),
                                                                            B2B(((ULONG)svoRampMeasVar.SRFOpp[0] * GPADC_IN_RANGE) >> GPADC_BITS));
                    // FE_PP_DL_wrong_layer: <L0 / 1> FEpp = %4d (mV), BSpp = %3d (mV), SRFO = %3d (mV)
                    SendMsgCnB(DEBUG_AUTO_TUNING_MSG,6,0x31501A+(!first_layer),B2B(((ULONG)svoRampMeasVar.FEpp[1] * DSPADC_IN_RANGE)>> DSPADC_BITS),
                                                                               B2B(((ULONG)svoRampMeasVar.BSpp[1] * DSPADC_IN_RANGE) >> DSPADC_BITS),
                                                                               B2B(((ULONG)svoRampMeasVar.SRFOpp[1] * GPADC_IN_RANGE) >> GPADC_BITS));
                }
                else
                {
                    if (((svoPrvtVar_maxFEpp[first_layer] > 0) && ((svoPrvtVar_maxFEpp[first_layer] * 2) <= svoRampMeasVar.FEpp[0])) ||
                        ((svoPrvtVar_maxFEpp[!first_layer] > 0) && ((svoPrvtVar_maxFEpp[!first_layer] * 2) <= svoRampMeasVar.FEpp[1])))
                    {
                        svoPrvtVar_focusP2PCount = 0;
                        svoPrvtVar_avgFOO = 0;
                        svoPrvtVar_avgBSpp[0] = 0;
                        svoPrvtVar_avgBSpp[1] = 0;
                        svoPrvtVar_avgSRFO[0] = 0;
                        svoPrvtVar_avgSRFO[1] = 0;
                        svoPrvtVar_avgFEpp[0] = 0;
                        svoPrvtVar_avgFEpp[1] = 0;
                        svoPrvtVar_maxFEpp[0] = 0;
                        svoPrvtVar_maxFEpp[1] = 0;
                        SendMsg80(SHOW_DEBUG_MSG,0x315014); //FE_PP_MEASURE clear old result
                    }
                    svoPrvtVar_avgFOO += svoRampMeasVar.LayerFoo;

                    svoPrvtVar_avgSRFO[first_layer] += svoRampMeasVar.SRFOpp[0];
                    svoPrvtVar_avgBSpp[first_layer] += svoRampMeasVar.BSpp[0];
                    svoPrvtVar_avgFEpp[first_layer] += svoRampMeasVar.FEpp[0];

                    if (svoPrvtVar_maxFEpp[first_layer] < svoRampMeasVar.FEpp[0])
                    {
                        svoPrvtVar_maxFEpp[first_layer] = svoRampMeasVar.FEpp[0];
                    }

                    svoPrvtVar_avgSRFO[!first_layer] += svoRampMeasVar.SRFOpp[1];
                    svoPrvtVar_avgBSpp[!first_layer] += svoRampMeasVar.BSpp[1];
                    svoPrvtVar_avgFEpp[!first_layer] += svoRampMeasVar.FEpp[1];

                    if (svoPrvtVar_maxFEpp[!first_layer] < svoRampMeasVar.FEpp[1])
                    {
                        svoPrvtVar_maxFEpp[!first_layer] = svoRampMeasVar.FEpp[1];
                    }
    #if (STREAM_STM == 0)
                    // (FE PP MEASURE Lx): FEpp = %4d (mV), BSpp = %3d (mV), SRFO = %3d (mV)
                    SendMsgCnB(DEBUG_AUTO_TUNING_MSG,6,0x315000+(!first_layer),B2B(((ULONG)svoRampMeasVar.FEpp[1] * DSPADC_IN_RANGE)>> DSPADC_BITS),
                                                                               B2B(((ULONG)svoRampMeasVar.BSpp[1] * DSPADC_IN_RANGE) >> DSPADC_BITS),
                                                                               B2B(((ULONG)svoRampMeasVar.SRFOpp[1] * GPADC_IN_RANGE) >> GPADC_BITS));
    #else
                    SendMsgCnB(DEBUG_AUTO_TUNING_MSG,6,0x315000+(!first_layer),B2B(((ULONG)svoRampMeasVar.FEpp[1] * GPADC_IN_RANGE)>> GPADC_BITS),
                        B2B(((ULONG)svoRampMeasVar.BSpp[1] * GPADC_IN_RANGE) >> GPADC_BITS),B2B(((ULONG)svoRampMeasVar.SRFOpp[1] * GPADC_IN_RANGE) >> GPADC_BITS));
    #endif
                }
            }
        }

        if (cal_retry > 10)
        {
            if (svoPrvtVar_focusP2PCount > 1)
            {
                svoPrvtVar_avgFEpp[0] = (svoPrvtVar_avgFEpp[0] * FOCUS_PP_READ_COUNT) / svoPrvtVar_focusP2PCount;
                svoPrvtVar_avgFEpp[1] = (svoPrvtVar_avgFEpp[1] * FOCUS_PP_READ_COUNT) / svoPrvtVar_focusP2PCount;
                svoPrvtVar_avgBSpp[0] = (svoPrvtVar_avgBSpp[0] * FOCUS_PP_READ_COUNT) / svoPrvtVar_focusP2PCount;
                svoPrvtVar_avgBSpp[1] = (svoPrvtVar_avgBSpp[1] * FOCUS_PP_READ_COUNT) / svoPrvtVar_focusP2PCount;
                svoPrvtVar_avgSRFO[0] = (svoPrvtVar_avgSRFO[0] * FOCUS_PP_READ_COUNT) / svoPrvtVar_focusP2PCount;
                svoPrvtVar_avgSRFO[1] = (svoPrvtVar_avgSRFO[1] * FOCUS_PP_READ_COUNT) / svoPrvtVar_focusP2PCount;
                svoPrvtVar_avgFOO     = (svoPrvtVar_avgFOO * FOCUS_PP_READ_COUNT) / svoPrvtVar_focusP2PCount;
                svoPrvtVar_focusP2PCount = FOCUS_PP_READ_COUNT;
            }
            else if ((svoRampMeasVar.FEpp[0] > 0) && ((svoCalVar.discChkDoneBits & disc_chk_end) == disc_chk_end))
            {
                svoPrvtVar_avgFEpp[0] = svoRampMeasVar.FEpp[0] * FOCUS_PP_READ_COUNT;
                svoPrvtVar_avgFEpp[1] = svoPrvtVar_avgFEpp[0];
                svoPrvtVar_avgBSpp[0] = svoRampMeasVar.BSpp[0] * FOCUS_PP_READ_COUNT;
                svoPrvtVar_avgBSpp[1] = svoPrvtVar_avgBSpp[0];
                svoPrvtVar_avgSRFO[0] = svoRampMeasVar.SRFOpp[0] * FOCUS_PP_READ_COUNT;
                svoPrvtVar_avgSRFO[1] = svoPrvtVar_avgSRFO[0];
#if (BD_ENABLE == 1)
                if (svoVar_discKindT.Bit.DiscStd == eDiscBD)
                {
                    //because the colli pos is set for L0, L1 signals become smaller than they should be
                    if (Layer_Index == 1)
                    {
                        svoPrvtVar_avgBSpp[0] /= BS_FACTOR_BD_L1;
                        svoPrvtVar_avgFEpp[0] = svoPrvtVar_avgFEpp[0] * 1024 / svoCalDlVar[0].fe_factor;
                        svoPrvtVar_avgSRFO[0] /= SRFO_FACTOR_BD_L1;
                    }
                    else
                    {
                        svoPrvtVar_avgBSpp[1] /= BS_FACTOR_BD_L1;
                        svoPrvtVar_avgFEpp[1] = svoPrvtVar_avgFEpp[1] * 1024 / svoCalDlVar[1].fe_factor;
                        svoPrvtVar_avgSRFO[1] /= SRFO_FACTOR_BD_L1;
                    }
                }
#endif // (BD_ENABLE == 1)
                svoPrvtVar_avgFOO     = svoRampMeasVar.LayerFoo * FOCUS_PP_READ_COUNT;
                svoPrvtVar_focusP2PCount = FOCUS_PP_READ_COUNT;
            }
#if (DVD_RAM_READ == 1) && (FE_SLEW_RATE_LIMITER == 2)
            else if ((svoVar_discKindT.Bit.DiscStd == eDiscDVD) && (svoVar_discKindT.Bit.Layers == eLayersSingle) &&
                ((svoCalGnT3Var[Layer_Index][FEOB_GN].normal_gain != 0x10) || (svoCalGnT3Var[Layer_Index][SPP2B_GN].normal_gain != 0x10)))
            {
                calSetAfeGain(Layer_Index,FEOB_GN,TYPE3,0,0,0x10);
                calSetAfeGain(Layer_Index,SPP2B_GN,TYPE3,0,0,0x10);
                cal_retry = 0;
            }
#endif

            else
            {
                svoPrvtVar_avgBSpp[0] = svoPrvtVar_avgBSpp[1] = 0;
                svoPrvtVar_avgFEpp[0] = svoPrvtVar_avgFEpp[1] = 0;
                if (svoVar_discKindT.Bit.Media == eMediaRW)
                {
                    svoPrvtVar_avgSRFO[0] = svoPrvtVar_avgSRFO[1] = 0;
                }
                else
                {
                    temp = (COEF_GetValue(AFE_CALC_NODISC_SRFO_TH) + 1);
                    svoPrvtVar_avgSRFO[0] = svoPrvtVar_avgSRFO[1] = (ULONG)(FOCUS_PP_READ_COUNT << DSPADC_BITS) * temp / DSPADC_IN_RANGE + 1;
                }

                svoPrvtVar_focusP2PCount = FOCUS_PP_READ_COUNT;
                svoPrvtVar_avgFOO = svoCalMisc2Var.fooos * FOCUS_PP_READ_COUNT;
                SendMsg80(SHOW_DEBUG_MSG,0x315011);   // FE_PP_MEASURE error
            }
        }

        if ((fepp_wrong_layer_chkcnt < 5) && (bLayerErrorCount > 5) && (svoVar_discKindT.Bit.DiscStd != eDiscCD))
        {
            cal_retry = 0;
            fepp_wrong_layer_chkcnt++;//from 720

#if (FOOFFSET2_ENABLE == 1)
            // Move lens to initial position
            ramp_FOOS(0);
#endif
            svoSetLDOff();

            switch(svoVar_discKindT.Bit.Layers)
            {
                case eLayersSingle:
                    svoVar_discKindT.Bit.Layers = eLayersDouble;
                    break;

                case eLayersDouble:
                    svoVar_discKindT.Bit.Layers = eLayersSingle;
                    break;

                default:
                    break;
            }
            svoVar_discKindT.Bit.Media = eMediaUnknown;//So RWdisc_check will re-judge RW or ROM again

            switch(svoVar_discKindT.Bit.DiscStd)
            {
                case eDiscDVD:
                    svoVar_svoTargetSpeed = SVO_CAL_SPEED_DVD;
                    break;

#if (ENABLE_HDDVD==1)
                case eDiscHD:
                    svoVar_svoTargetSpeed = SVO_CAL_SPEED_HD;
                    break;
#endif
#if (BD_ENABLE == 1)
                case eDiscBD:
                    svoVar_svoTargetSpeed = SVO_CAL_SPEED_BD;
                    break;
#endif
                default:
                    DEBUG_ASSERT(FALSE, eASSERT_IMPOSSIBLE_DISCKIND );
                    break;
            }
            bSetSpeedAgain = TRUE;
            spdChangeDiscSpeed(svoVar_svoTargetSpeed);

            // === Load Parameters according to disc kind ======
            temp = svoVar_TrackingMethod;
            svoAFEInitOnly();
            svoVar_TrackingMethod = temp;

            svo_Adj_Data_Clr();

            svo_preset_media_par();

            SendMsg80(SHOW_DEBUG_MSG,0x315013);   // FE_PP_MEASURE layer nr error

	#if (Customer_A1 ==1)
	FEPPErrorCount++;
	if(FEPPErrorCount >=3)
	{
		CompleteStroke = TRUE;
		return(SEQ_EX3);
	}
	#endif
			
            return(SEQ_EX2);
        }

#if (MEAS_SUBSTRATE_P2P == 1)
        if (ARG3 == DOWN)
        {
            WRITE_FIELD(HAL_SRV_PKBOT,0);
            WRITE_FIELD(HAL_SRV_PKBOT,1);
            WaitTimer(100);
            WRITE_FIELD(HAL_SRV_PKBOT,0);

            WRITE_REG(HAL_SRVREG_MAP->RGSEL,halSRV_RSEL_FEBOTTOM);      /* read Bottom data */
            fe_bottom = READ_REG(HAL_SRVREG_MAP->SRCR);
            WRITE_REG(HAL_SRVREG_MAP->RGSEL,halSRV_RSEL_FEPEAK);        /* read Peak data */
            // SU: %d, %d
            SendMsgCnB(DEBUG_SEEK_MSG,2,0x3FAA0D,B1B(bFocMoveReference-fe_bottom),B1B(READ_REG(HAL_SRVREG_MAP->SRCR)-bFocMoveReference));
        }
#endif
    }
    return(SEQ_EX0);
}

/*-----------------------------------------------------------------------------
 *
 * Module: state_fsrch_chk()
 * Description: return SEQ_EX0 when leave ramping mode
 *              ARG3: Up/Down
 *
 *----------------------------------------------------------------------------*/
seq_exit_t state_fsrch_chk(void)
{
    BYTE temp_reg;
    USHORT cur_timeout,new_timeout;

    if (svoIramVar.svoPreInState != svoIramVar.svoInState)
    {
        WRITE_FIELD(HAL_SRV_PKBOT,0);
        WRITE_FIELD(HAL_SRV_PKBOT,1);
        svoRampMeasVar.timeout = eTimerTimeout;
    }

    if (!READ_FIELD(HAL_SRV_FSRCH))
    {
        WRITE_DSP_COEF(kf18, FOCUS_SEARCH_KF18);

#if (FocusLockAid_Enable == 1)
        HAL_DSP_FocusLockAid(OFF, UP);  // disable focus lock aid during STM
#endif
        SetTimeout(0,TIMER_STOP);

        return(SEQ_EX0);
    }

    temp_reg = (ARG3 == UP)? HAL_SERVO_GetFocUp() : HAL_SERVO_GetFocDn();
    cur_timeout = timeout_chk(0);
    if (cur_timeout <= svoRampMeasVar.timeout)
    {
        SetTimeout(0,TIMER_STOP);
#if (SRC_ENABLE == 1) && (SRC_ENABLE_FOCUS == 1)
        HAL_DSP_SRCEnable(SRC_LOOP_FOCUS, SRC_STATE_RESET); //reset Focus SRC
#endif
#if (FocusLockAid_Enable == 1)
        HAL_DSP_FocusLockAid(OFF, UP);  // disable focus lock aid during STM
#endif
        HAL_SERVO_SetFSL(0x7F, 0x7F);
        if (temp_reg & HAL_SRV_FSONSET_MASK)
        {
            SendMsg80(SHOW_DEBUG_MSG,0x320A20); //F Catch Timeout
        }

        return(SEQ_EX1);
    }

    if (ARG3 == UP)
    {
        if (cur_timeout <= svoRampMeasVar.pre_timeout)
        {
            WRITE_REG(HAL_SRVREG_MAP->RGSEL,halSRV_RSEL_BSPEAK);
            if (READ_REG(HAL_SRVREG_MAP->SRCR) > ((svoCalVar.bsOfsData.all16 >> 2) + (COEF_GetValue(AFE_CALC_BS_TARGET_10MV) * (FSRCH_RETRY + svoMiscVar.focusRampCount) * ((10 << DSPADC_BITS) / (2 * FSRCH_RETRY)) / DSPADC_IN_RANGE)))
            {//BS > (0.5 ~ 1) * BS_target
                new_timeout = F_PP_DELAY_T;
                if (temp_reg & HAL_SRV_HISRSET_MASK)
                {
                    new_timeout = (F_PP_DELAY_T + 1) / 2;
                }
                new_timeout = new_timeout * FOCUS_SEARCH_KF18 / READ_DSP_COEF(kf18);
                if (cur_timeout <= new_timeout)
                {
                    svoRampMeasVar.timeout = eTimerTimeout;
                }
                else
                {
                    svoRampMeasVar.timeout = cur_timeout - new_timeout;
                }
            }
        }
        WRITE_FIELD(HAL_SRV_PKBOT,0);
        WRITE_FIELD(HAL_SRV_PKBOT,1);
    }

    return(SEQ_BUSY);
}

/*-----------------------------------------------------------------------------
 *
 * Module: state_fok_chk()
 * Description: return SEQ_EX0 when FOK = TRUE
 *
 *----------------------------------------------------------------------------*/
 #define FOCUS_LOCK_AID_TUNING 0
#if (FOCUS_LOCK_AID_TUNING > 0)
ULONG ok_cnt[4][FOCUS_LOCK_AID_TUNING],total_cnt;
#endif
seq_exit_t state_fok_chk(void)
{
    BYTE     first_layer,fok;
    ad_dsp_t te_signal;
#if (BD_ENABLE == 1) && (COLLI_SWITCH == 1)
    USHORT   te_thr,te_thr2;
    BYTE     tries;
#endif

#if (BD_ENABLE == 1)
    if (svoVar_discKindT.Bit.DiscStd == eDiscBD)
    {//BD layer 1 is below layer 0
        first_layer = (ARG3 == UP)? 1 : 0;// fix bugs in FEpp measurement
    }
    else
#endif // (BD_ENABLE == 1)
    {
        first_layer = (ARG3 == UP)? 0 : 1;// fix bugs in FEpp measurement
    }

#if (BD_ENABLE == 1) && (COLLI_SWITCH == 1)
    if ((svoVar_discKindT.Bit.DiscStd == eDiscBD) && (svoVar_discKindT.Bit.Layers == eLayersDouble))
    {
        afeSetTrackingMethod(TRK_METHOD_DPP);
        te_thr2 = COEF_GetValue(AFE_CALC_TE_PP_TARGET_10MV);
        if ((svoVar_discKindT.fDiscKind == eBDROMDLDisc) && (svoCalVar.discChkDoneBits & start_end))
        {
            te_thr2 /= 3;
        }
        else
        {
            te_thr2 *= 2;
        }
    }
#endif

    fok = focus_on_chk(&te_signal.p2p_mv);
#if (FOCUS_LOCK_AID_TUNING > 0)
    if ((fok == OK) || (servo_debug[0] && (svoMiscVar.focusRetryCount == FOCUS_RETRY) && (svoMiscVar.focusRetryCount++)))
#else
    if (fok == OK)
#endif
    {
#if (FOCUS_LOCK_AID_TUNING > 0)
        if (servo_debug[0])
        {
            if ((total_cnt == 0) && (servo_debug[1] == 0) && (servo_debug[2] == 0))
            {
                svoMiscVar.focusRetryCount++;
            }
            ok_cnt[servo_debug[2]][servo_debug[1]] += svoMiscVar.focusRetryCount;
            servo_debug[1]++;
            if (servo_debug[1] == FOCUS_LOCK_AID_TUNING)
            {
                servo_debug[2]++;
                if (servo_debug[2] == 4)
                {
                    total_cnt++;
                    send_msgA4(1,0x091000,total_cnt);
                    for (servo_debug[2] = 0; servo_debug[2] < 4; servo_debug[2]++)
                    {
                        for (servo_debug[1] = 0; servo_debug[1] < FOCUS_LOCK_AID_TUNING; servo_debug[1]++)
                        {
                            send_msg74(1,0x090000 + (servo_debug[2] << 8) + servo_debug[1],(float)ok_cnt[servo_debug[2]][servo_debug[1]] / total_cnt);
                        }
                    }
                    servo_debug[2] = 0;
                }
                servo_debug[1] = 0;
            }
            WRITE_REG(HAL_SRVREG_MAP->LDOFF, 0x00);
            WRITE_REG(HAL_SRVREG_MAP->LDON, 0x00);
#if (NEW_FDP_DETECTION != 0)
            svoFDropIsr();
#endif
            svoMiscVar.focusDropCount = 0;
            svoMiscVar.focusRampCount = 0;
            svoMiscVar.focusRetryCount = 0;
            SendMsg80(SHOW_DEBUG_MSG,0x320A1D);     // F Catch NG

            return SEQ_EX1;
        }
        else
        {
            total_cnt = 0;
            for (servo_debug[2] = 0; servo_debug[2] < 4; servo_debug[2]++)
            {
                for (servo_debug[1] = 0; servo_debug[1] < FOCUS_LOCK_AID_TUNING; servo_debug[1]++)
    {
                    ok_cnt[servo_debug[2]][servo_debug[1]] = 0;
                }
            }
            servo_debug[1] = 0;
            servo_debug[2] = 0;
        }
#endif

        if (svoVar_discKindT.Bit.Layers == eLayersDouble)
        {
#if (FOCUS_LOCK_FROM_DOWN_TO_UP == 1)
            if (svoVar_discKindT.Bit.DiscStd != eDiscBD)
#endif
            {
                calUpdateLayer(first_layer);// fix bugs in FEpp measurement
            }
#if (BD_ENABLE == 1) && (COLLI_SWITCH == 1)
            if (svoVar_discKindT.Bit.DiscStd == eDiscBD)
            {
                for (tries = 1; tries <= 2; tries++)
                {
                    if (tries == 1)
                    {
                        te_signal.errcode = eNOERR;
                        te_thr = svoCalDlVar[Layer_Index].dpp_thr;
                        te_thr2 += te_thr;
                        if ((te_signal.p2p_mv < te_thr2) && (te_signal.p2p_mv >= te_thr))
                        {
                            te_signal = ReadFilteredDSPADCmV(ONE_REVOLUTION, TE_ALL, ADC_TE, TRUE);
                        }
                    }
                    else
                    {
                        if (te_signal.errcode == eNOERR)
                        {
                            te_thr = te_signal.p2p_mv;
                        }
                        te_signal = ReadFilteredDSPADCmV(ONE_REVOLUTION, TE_ALL, ADC_TE, TRUE);
                    }
                    // BD wrong layer chk in fok_check: TEpp = %4d, thr = %4d, layer = %1d
                    SendMsgCn(SHOW_DEBUG_MSG,5,0x33400B,A2B(te_signal.p2p_mv),A2B(te_thr),Layer_Index);

                    if ((te_signal.errcode != eNOERR) || (te_signal.p2p_mv < te_thr))
                    {
                        SendMsg80(SHOW_DEBUG_MSG,0x33400C); // WARNING: Wrong layer!
                        if (tries == 1)
                        {
                            if (!FokStatusOK()) break;

                            svoStp_JumpToPosition(COLLI_NORMAL_MODE, calGetOptColliPos(!Layer_Index));
                            while (svoStp_busy())
                            {
                                ExitProcess();
                            }
                        }
                        else
                        {
                            if (te_signal.errcode == eNOERR)
                            {
                                svoCalDlVar[Layer_Index].dpp_thr = (te_thr + svoCalDlVar[Layer_Index].dpp_thr) / 2;
                            }
                            svoStp_JumpToPosition(COLLI_NORMAL_MODE, calGetOptColliPos(Layer_Index));
                            while (svoStp_busy())
                            {
                                ExitProcess();
                            }
                        }
                    }
                    else
                    {
                        if (tries == 2)
                        {
                            calUpdateLayer(!Layer_Index);
                        }
                        break;
                    }
                }
            }
#endif // #if (BD_ENABLE == 1) && (COLLI_SWITCH == 1)
        }
    }

#if (BD_ENABLE == 1) && (COLLI_SWITCH == 1)
    if ((svoVar_discKindT.Bit.DiscStd == eDiscBD) && (svoVar_discKindT.Bit.Layers == eLayersDouble))
    {
        afeSetTrackingMethod(getTrackingMethod(svoVar_discKindT));
    }
#endif// == start == change BD wrong layer detection measurement from 1/8 rev to 1 rev

    if ((fok == OK) && FokStatusOK())
    {
#if (ENABLE_LEARN_DEFECT == 1)//from720
        LAL_DSP_SetLearnDefect(lalDSP_On);
#endif
        WRITE_REG(HAL_SRVREG_MAP->DMSV, 0x00);  /* CAV servo ON, accel/decel OFF */
        Defect_Enable(DEFECT_STATE_ONLY_FOCUS_ON|DEFECT_SOURCE_SERVO_BS);//open focus defect hold
        apply_FOOS(svoCalMisc2Var.fooos);
        calUpdateRadius(Iram_stp_CurrentPosition, TRUE);    //update FEBC

#if (DVD_RAM_READ == 1)
        if(DVDRAM)
        {
            /* Disable black and white defect */
            /* Disable TE and FE Hold at defect */
            Defect_Enable(DEFECT_STATE_ALL_OFF);
        }
#endif

        SendMsg80(SHOW_DEBUG_MSG,0x33FF02); // >>> FOCUS OK <<<

#if (SRC_ENABLE == 1) && (SRC_ENABLE_FOCUS == 1)
        HAL_DSP_SRCHold(SRC_LOOP_FOCUS, ON, ON, OFF);
#endif

#if (EN_ADI == 1)
//        if (HAL_DSP_SetActFocDissInt())
//        {
//            SendMsg80(SHOW_DEBUG_MSG,0x33FF44);     //enable FocADI
//        }
#endif

        svoMiscVar.focusRampCount = 0;
        svoMiscVar.focusRetryCount = 0;

        svoVar_bByPassFOKCheck = FALSE;
#if (NEW_FDP_DETECTION != 0)
        CLEAR_INT_FIELD(HAL_SRV_FNOK,0);
        WRITE_FIELD(HAL_SRV_FNOKEN,1);    /* Enable Focus drop interrupt */
    #if (NEW_FDP_DETECTION == 2)
        WRITE_FIELD(HAL_SRV_DSPI6,0);
        WRITE_FIELD(HAL_SRV_DSPI6EN,1);         /* Enable New Focus drop interrupt */
    #endif
#endif
//20101116_02VL ==start== For BDRE-DL VDD 0.4(BHE-M201 AR400) read fail, frank modify Andy confirm
#if 0//#if (BD_RETRY_USE_DPD == 1)//TBD:need check later
    if ((svoVar_discKindT.fDiscKind == eBDREDLDisc) &&
        ((read_retry_option & (TE_DPD_RETRY_ENABLE|TE_DPD_TRK_RETRY_ENABLE)) == (TE_DPD_RETRY_ENABLE|TE_DPD_TRK_RETRY_ENABLE)))
    {
        read_retry_option &= TE_DPD_RETRY_DISABLE;
        SetEqRegsForASpeed(svoVar_discKindT, svoIramVar.svoCurrentSpeed);
    }
#endif
//20101116_02VL ==end== For BDRE-DL VDD 0.4(BHE-M201 AR400) read fail, frank modify Andy confirm

#if (HOLD_LENS_BEFORE_TRKON == 1)// improve CE control
        if (svoCalVar.calGainDoneBits & ceg_end)//fix DVDRAM detection problem
        {
            seekSetCEEq(eRoughSeek, (svoVar_discKindT.Bit.DiscStd == eDiscCD)? CE_GAIN_DOWN_RATIO_CD : CE_GAIN_DOWN_RATIO_DVD);
            HAL_SERVO_CenterControlON(FORW);  // Turn Center-servo on
        }
#endif// improve CE control
        svoIramVar.focusDropFlag = FALSE;
        spdUnhold();

        return SEQ_EX0;
    }
    else
    {
        svoMiscVar.focusDropCount++;
        SendMsg80(SHOW_DEBUG_MSG,0x320A1D);
#if (Customer_A1 ==1)
				PositionHomeErrorCount++;	
#endif

        return SEQ_EX1;
    }
}

/*-----------------------------------------------------------------------------
 *
 * Module: state_focus_count()
 * Description: Can't see layer
 *
 *----------------------------------------------------------------------------*/
seq_exit_t state_focus_count(void)
{
#if (CHIP_REV < 0xC0)
    LONG foo_os;
#endif
    svoMiscVar.focusRampCount++;
    if ((svoMiscVar.focusRampCount > (BYTE)ARG2) || (svoIramVar.knlTimeupCount == 0))
    {
        //"ERROR: maximum allowed focusRampCount reached, CMD time last: %4u ms, cur_RPM: %u, focusRampCount: %d", 2 2 1
        SendMsgCnB(SHOW_DEBUG_MSG,5,0x33FF65,B2B(svoIramVar.knlTimeupCount*20),B2B(spdGetCurrentRPM()),B1B(svoMiscVar.focusRampCount));
        svoMiscVar.focusDropCount = 0;
        svoMiscVar.focusRampCount = 0;
        svoMiscVar.focusRetryCount = 0;
        svoMiscVar.forceFocusOffFlag = TRUE;
        if (svoCalVar.discChkDoneBits & start_end)
        {
        return(SEQ_EX1);
    }
        else
        {
            return (SEQ_EX2);
        }
    }
    else if (svoMiscVar.focusRampCount == (BYTE)ARG2)
    {
        SendMsgCn(DEBUG_SEEK_MSG,1,0x3FAA3D,A1B(svoMiscVar.focusRampCount));
        svoStp_Homing();
        while (svoStp_busy())
        {
            ExitProcess();
        }
    }

    if (svoCalMisc2Var.fooos != svoPrvtVar_avgFOO)
    {
        calSetMisc(0,FOO_OS,svoPrvtVar_avgFOO);
#if (CHIP_REV < 0xC0)
        foo_os = svoCalMisc2Var.fooos - F_SEARCH_T * FOCUS_SEARCH_KF18 / 0x20 * FOC_RAMP_FACTOR / 0x20000;
        if (foo_os < -FOO_OFFSET_LIMIT) foo_os = -FOO_OFFSET_LIMIT;
        ramp_FOOS(foo_os);
#else
        ramp_FOOS(svoPrvtVar_avgFOO);
#endif
    }

    return (SEQ_EX0);
}

/*-----------------------------------------------------------------------------
 *
 * Module: state_focus_retry()
 * Description: Can't catch focus
 *
 *----------------------------------------------------------------------------*/
seq_exit_t state_focus_retry(void)
{
    USHORT new_rpm,TargetRpm;

#if (SRC_ENABLE == 1) && (SRC_ENABLE_FOCUS == 1)
    HAL_DSP_SRCEnable(SRC_LOOP_FOCUS, SRC_STATE_RESET); //reset Focus SRC
#endif

    svoMiscVar.focusRetryCount++;
#if (FOCUS_LOCK_AID_TUNING == 0)
    if (svoMiscVar.focusRetryCount > 2)
#else
    if ((svoMiscVar.focusRetryCount > 2) && (servo_debug[0] == 0))
#endif
    {
        // reduce RPM when focus retry for VDD disc
        spdUnhold();
        TargetRpm = spdGetTargetRPM(Iram_stp_CurrentPosition, svoSpeedVar.SpdCurrentSpeed);
        TargetRpm = (TargetRpm*2)/5; //TargetRpm/2.5
        new_rpm = svoSpeedVar.CurrentRpm - F_RETRY_SPIN_DOWN_RPM;

        //if (new_rpm < F_RETRY_MIN_RPM) new_rpm = F_RETRY_MIN_RPM;
        if (new_rpm < TargetRpm)
            new_rpm = TargetRpm;

        spdHoldRPM(new_rpm);
        SendMsgCn(SHOW_DEBUG_MSG,2,0x320A21,A2B(new_rpm));//spin down to %4d RPM for focus recovery
    }

    if ((svoMiscVar.focusRetryCount > (BYTE)ARG2)||(svoIramVar.knlTimeupCount == 0))//from 720
    {
        spdUnhold();
#if (CHIP_REV >= 0xC0)
        HAL_SERVO_SetFcs((FDP_FOO_OS - HAL_SERVO_GetFoos()) * 0x8000 / FOCUS_SEARCH_KF18);
    #if (FDP_PROT_PULSE_TIME != 0)
        Delay100uS(FDP_PROT_PULSE_TIME);
    #endif
        WRITE_FIELD(HAL_SRV_FCSSBYEN,0);
        WRITE_FIELD(HAL_SRV_FCSSBYEN,1);
        HAL_SERVO_SetFocDn(0);
        HAL_SERVO_SetFocStp();
#elif (NEW_FDP_DETECTION == 0)
        //hold lens at bottom
        HAL_SERVO_SetFoos(FDP_FOO_OS);
        WRITE_FIELD(HAL_SRV_LDOFF, 0x00);
        WRITE_FIELD(HAL_SRV_LDON, 0x00);
#endif
        focus_wait_stable(FDP_STABLE_TIME);

        //"ERROR: maximum allowed focusRetryCount reached, CMD time last: %4u ms, cur_RPM: %u, focusRetryCount: %d", 2 2 1
        SendMsgCnB(SHOW_DEBUG_MSG,5,0x33FF66,B2B(svoIramVar.knlTimeupCount*20),B2B(spdGetCurrentRPM()),B1B(svoMiscVar.focusRetryCount));
        svoMiscVar.focusDropCount = 0;
        svoMiscVar.focusRampCount = 0;
        svoMiscVar.focusRetryCount = 0;
        svoMiscVar.forceFocusOffFlag = TRUE;
        if (svoCalVar.discChkDoneBits & start_end)
        {
        return(SEQ_EX1);
    }
        else
        {
            return (SEQ_EX2);
        }
    }
    return (SEQ_EX0);
}

/*-----------------------------------------------------------------------------
 *
 * Module: state_focus_hold()
 * Description: hold lens at the given position
 *              ARG1: hold position (ms)
 *              ARG3: Up/Down
 *
 *----------------------------------------------------------------------------*/
seq_exit_t state_focus_hold(void)
{
    LONG foo_os,foo_dac;

    foo_os = svoCalMisc2Var.fooos - ARG1 * (FOCUS_SEARCH_KF18 * FOC_RAMP_FACTOR / 0x20) / 0x20000;
    foo_dac = HAL_SERVO_GetFooDac();

    if (((ARG3 == DOWN)? (foo_dac <= foo_os) : (foo_dac >= foo_os)) || (timeout_chk(0) == eTimerTimeout)) /* time out check */
    {
        SetTimeout(0, TIMER_STOP);

        //hold lens at current position
        if (foo_dac < -FOO_OFFSET_LIMIT) foo_dac = -FOO_OFFSET_LIMIT;
#if (FOOFFSET2_ENABLE == 1)
        WRITE_DSP_COEF(kf18,0);
        HAL_SERVO_SetFoos(foo_dac);
#else
        WRITE_REG(HAL_SRVREG_MAP->LDOFF, 0x00);
        HAL_SERVO_SetFoos(foo_dac);
        WRITE_REG(HAL_SRVREG_MAP->LDON, 0x00);
#endif

        return(SEQ_EX0);
    }
    return(SEQ_BUSY);
}

/*-----------------------------------------------------------------------------
 *
 * Module: state_discedge_chk()
 * Description: Pull lens to the ramp bottom position and check current sledge position,
 *              at disc edge return SEQ_EX1, else return SEQ_EX0.
 *
 * Input: ARG1 = 1, first focus drop
 *        ARG1 = 0, focus drop
 *        ARG1 = 2, focus retry
 *----------------------------------------------------------------------------*/
seq_exit_t state_discedge_chk(void)
{
    LONG   foo_os;
    SHORT  jump_distance;
    seq_exit_t  ret_seq;

    foo_os = svoCalMisc2Var.fooos - F_SEARCH_T * FOCUS_SEARCH_KF18 / 0x20 * FOC_RAMP_FACTOR / 0x20000;
    if (foo_os < -FOO_OFFSET_LIMIT) foo_os = -FOO_OFFSET_LIMIT;

    WRITE_DSP_COEF(kf18,FOCUS_SEARCH_KF18);

    if (ARG1 == 2)
    {
#if (CHIP_REV >= 0xC0)
        HAL_SERVO_SetFcs((foo_os - HAL_SERVO_GetFoos()) * 0x8000 / FOCUS_SEARCH_KF18);
        WRITE_FIELD(HAL_SRV_FCSSBYEN,0);
        WRITE_FIELD(HAL_SRV_FCSSBYEN,1);
#else
        HAL_SERVO_SetFoos(foo_os);
        WRITE_REG(HAL_SRVREG_MAP->LDOFF, 0x00);
        WRITE_REG(HAL_SRVREG_MAP->LDON, 0x00);
#endif
    }
    else
    {
        if (ARG1 == 1)//first drop
        {
#if (BD_ENABLE == 1)
            if (svoVar_discKindT.Bit.DiscStd == eDiscBD)
            {
                HAL_SERVO_SetTilt(TILT_INIT_DAC);
            }
#endif
            //TON_ON_OFF(OFF);
            svoSeekVar.jumpType |= SLIP_MASK;
            HAL_SERVO_SetFebc(0);    //not in focus close loop, set FEBC to 0 directly.
            WRITE_FIELD(HAL_SRV_REGTON,0);
        }
#if (CHIP_REV >= 0xC0)
        HAL_SERVO_SetFcs((FDP_FOO_OS - HAL_SERVO_GetFoos()) * 0x8000 / FOCUS_SEARCH_KF18);
    #if (FDP_PROT_PULSE_TIME != 0)
        Delay100uS(FDP_PROT_PULSE_TIME);
    #endif
        WRITE_FIELD(HAL_SRV_FCSSBYEN,0);
        WRITE_FIELD(HAL_SRV_FCSSBYEN,1);
        HAL_SERVO_SetFocDn(0);
        HAL_SERVO_SetFocStp();
#elif (NEW_FDP_DETECTION == 0)
        //hold lens at bottom
        HAL_SERVO_SetFoos(FDP_FOO_OS);
        WRITE_FIELD(HAL_SRV_LDOFF, 0x00);
        WRITE_FIELD(HAL_SRV_LDON, 0x00);
#endif
#if (FOOFFSET2_ENABLE == 1)
            WRITE_DSP_COEF(kf18,0);//bug fixed: this action must be done after LDOFF/ON when FOOFFSET2_ENABLE == 1
            HAL_SERVO_SetFocDn(0x00);
#endif
    }

#if (NEW_FDP_DETECTION != 0)
    svoIramVar.focusDropIsrFlag = FALSE;
#endif

    jump_distance = svoStp_recovery_jump_dist();
    if (jump_distance == 0)
    {
        if (ARG1 != 1)
        {
            // jump toward radius 28.0 mm for 12cm disc,   jump toward radius 25.0 mm for 8cm disc
            if (Iram_stp_CurrentPosition > ((svoVar_discDiameter == e8cm)? UM_2_SLED_STEP(25500):UM_2_SLED_STEP(28000)))
            {
                jump_distance = ((svoVar_discDiameter == e8cm)? -UM_2_SLED_STEP(500) : -UM_2_SLED_STEP(1000));
            }
            else
            {
                jump_distance = ((svoVar_discDiameter == e8cm)? UM_2_SLED_STEP(500) : UM_2_SLED_STEP(1000));
            }

#if (DVD_RAM_READ == 1)
            if (DVDRAM_embossed)
            {
                jump_distance = -UM_2_SLED_STEP(20);
            }
#endif

            /* To sled home position after 3 times retry */
            if ((svoMiscVar.focusRetryCount == 4) && (St_drv_ready == 1))
            {
                jump_distance = svoStepVar.StepHomePos - Iram_stp_CurrentPosition;
            }
        }
    }

    ret_seq = SEQ_EX0;
    if (jump_distance != 0)
    {
        SendMsgCn(DEBUG_SEEK_MSG,2,0x3FAA39,A2B(jump_distance));//Focus recovery (move_sled %d)
#if (ACTIVE_SLED_FOR_FINE_JUMP == 1)
        svoStp_SetTimerMode( STEP_DISABLE_TIMER );
#endif // (ACTIVE_SLED_FOR_FINE_JUMP == 1)
        svoStp_JumpSteps(STEP_NORMAL_MODE, jump_distance);

        if (ARG1 == 1)
        {
            HAL_SERVO_SetFebc(0);    //not in focus close loop, set FEBC to 0 directly.
        }

        ret_seq = SEQ_EX1;
    }

    if (ARG1 != 2)
    {
        focus_wait_stable(FDP_STABLE_TIME);
    }

#if (AFE_NORMALIZER == 1)
    afeSvognDisable();
#endif

    GAIN_RECD_HIGH;
    return (ret_seq);
}

/*-----------------------------------------------------------------------------
 *
 * Module: state_step_end_chk()
 * Description:
 *
 *----------------------------------------------------------------------------*/
seq_exit_t state_step_end_chk(void)
{
#if DO_HW_SUPPORT_STEP == 1
    //---------------------------------------------------------------;
    // works are done inside state_step_init_chk                     ;
    //---------------------------------------------------------------;
    return(SEQ_EX0);
#else // DO_HW_SUPPORT_STEP == 1
    if (!svoStp_busy())
    {
        svoStp_StopControl();
        return(SEQ_EX0);
    }
    SetDelayTimer(ARG1);
    return(SEQ_BUSY);
#endif // DO_HW_SUPPORT_STEP == 1
}

/*-----------------------------------------------------------------------------
 *
 * Module: state_disc_chk2()
 * Description: try to lock wobble for CDR(W)
 *
 *----------------------------------------------------------------------------*/
seq_exit_t state_disc_chk2(void)
{
#if ((BD_ENABLE == 1) && (BD_WOBBLE_DET == 1))
    ULONG wob_osx100,wob_osx100_sum;
    BYTE  smp_cnt;
    ULONG wob_pll_lock_cnt,wob_sync_lock_cnt;
    SHORT orig_pdic_gn, orig_laser_gn;
#endif

    SendMsg80(DEBUG_CALIB_ENTRYS_MSG,0x3A011A); // >>> STATE_DISC_CHK2 <<<

    if ((svoCalVar.discChkDoneBits & disc_chk_end) == disc_chk_end)
    {
        SendMsg80(DEBUG_CALIB_ENTRYS_MSG,0x3A0002); // ==>Calib SKIP!!
#if(FORCE_DISK_KIND == 1)
        if (!(svoCalVar.discChkDoneBits & start_end))
            svo_preset_media_par();
#endif // (FORCE_DISK_KIND == 1)
        return(SEQ_EX1);
    }

/* Only check BDROM because BDR/RE wobble already check in state_pll_chk but not for BDROM */
/* if BDR/RE mis-detection as BDROM, it can be recovery from here */
/* Check 20 times because wrong tracking method cause wobble pll not always lock */
#if ((BD_ENABLE == 1) && (BD_WOBBLE_DET == 1))
    if ((svoVar_discKindT.Bit.DiscStd == eDiscBD) && (svoVar_discKindT.Bit.Media == eMediaROM))
    {
        orig_pdic_gn = svoCalMisc2Var.pdic_gn;
        orig_laser_gn = svoCalMisc2Var.laser_gn;
        smp_cnt = 0;
        wob_pll_lock_cnt = wob_sync_lock_cnt = 0;
        wob_osx100 = wob_osx100_sum = 0;
        do
        {
            /* Check wobble frequency */
            wob_osx100 = HAL_WOBBLE_GetOverspeed(HAL_PLL_GetWblClkKHz());

            /* Check wobble pll lock status */
            if (HAL_WOBBLE_GetPllInLock(HAL_PLL_GetWblClkKHz()))
            {
                wob_pll_lock_cnt++;
                /* Check BD bit sync & word sync lock status */
                if (HAL_WOBBLE_GetSyncFlyWheelInLock())
                    wob_sync_lock_cnt++;
            }

            wob_osx100_sum += wob_osx100;
            smp_cnt ++;
            WaitTimer(1);
        }while(smp_cnt < 20);

        wob_osx100 = wob_osx100_sum/smp_cnt;
        SendMsgCnB(SHOW_DEBUG_MSG,4,0x41022E,B2B(wob_osx100),B2B(wob_pll_lock_cnt)); //WOBBLE speed x 100

        if (wob_pll_lock_cnt >= 2)
        {
    #if 0//(DISC_MIS_JUDGE_RECOVERY)
            svoReStartUp(eMediaRW);
    #else
            svoVar_discKindT.Bit.Media = eMediaRW;
            svoCalVar.calOfsDoneBits  &= (~teof_end);
            svoCalVar.calGainDoneBits &= (~teg_end);
            SendMsg80(SHOW_DEBUG_MSG,0x320A1C);
            track_off();
            afeSetTrackingMethod(getTrackingMethod(svoVar_discKindT));
            if (svoCalVar.calGainDoneBits & rpg_end)
                svoSeekVar.rx_available = RX_AVAILABLE;
            LAL_StoreParams(LAL_DetectDiscKind(svoVar_discKindT), svoIramVar.svoCurrentSpeed);
            LAL_SetReadMode();
    #endif
            if ((orig_pdic_gn != svoCalMisc2Var.pdic_gn) || (orig_laser_gn != svoCalMisc2Var.laser_gn))
            {
                svoCalVar.calOfsDoneBits = vref_end;
                svoCalVar.calGainDoneBits = 0;
                svoCalVar.calOfsDoneBits = 0;
                return(SEQ_EX3);
            }
            else
            {
                return(SEQ_EX2);
            }
        }
    }
#endif

#if (WOBBLE_READ_ON_DVD_OR_CD == 1)
    if (svoVar_discKindT.Bit.DiscStd == eDiscCD)
    {// Prepare for state_disc_chk3
        //switch AFE wobble LPF BW to 1.1 MHz
        WRITE_FIELD(HAL_AFE_WOBLPFEN, 1);   //enable wob lpf before ADC Buf
        WRITE_FIELD(HAL_AFE_WOBLPFBW, 0);   //set wob lpf before ADC Buf

        ATIPPLLSetting.CDATIPPhaseGain = COEF_GetValue( WBL_CALC_CDATIPPhaseGain );
        ATIPPLLSetting.DTOOn           = COEF_GetValue( WBL_CALC_DTOOn );
        ATIPPLLSetting.CDATIPLPF       = COEF_GetValue( WBL_CALC_CDATIPLPF );
        ATIPPLLSetting.CDATIPKP        = COEF_GetValue( WBL_CALC_CDATIPKP );
        HAL_WOBBLE_SetATIPPLL(&ATIPPLLSetting);

        LAL_WOBBLE_SetPLL(eHighGainMode);
        WRITE_FIELD(HAL_DFE_PLLMODE, halDFE_PLLModeAutomatic);    /* Set the semi-synchronous PLL from wobble */

        ENABLE_WBLINT();

        HAL_WOBBLE_SetPllOnHold(halWBL_WblPllNotInHold);

        // Disable flywheel
        HAL_WOBBLE_EnableFlywheels(FALSE);
        Delay100uS(2);

        // Enable flywheel
        HAL_WOBBLE_EnableFlywheels(TRUE);

        svoEnAtipRead();
        svoAtipOkCnt = 0;

        SetDelayTimer(ARG1);
        return(SEQ_EX0);
    }
    else
#endif
    {
        svoCalVar.discChkDoneBits |= disc_chk_end;
        svo_preset_media_par();
        return(SEQ_EX1);
    }
}

#if (WOBBLE_READ_ON_DVD_OR_CD == 1)
/*-----------------------------------------------------------------------------
 *
 * Module: state_disc_chk3()
 * Description: wait wobble lock for CDR(W)
 *
 *----------------------------------------------------------------------------*/
seq_exit_t state_disc_chk3(void)
{
    if (svoIramVar.svoPreInState != svoIramVar.svoInState)
    {
        SendMsg80(DEBUG_CALIB_ENTRYS_MSG,0x3A011B); // >>> STATE_DISC_CHK3 <<<
        SetTimeout(0, ARG2);
    }

    if (timeout_chk(0) == eTimerTimeout)
    {   /* time out check */
#if (EN_POWER_CONTROL == 1)
    #if (WOBBLE_READ_ON_DVD_OR_CD == 1)
        PowerControl(eCDReadPower);
    #endif
#endif
        if ((svoVar_discKindT.fDiscKind == eCDRDisc) || (svoVar_discKindT.fDiscKind == eCDRWDisc))
        {
            svoSetLDOff();
            svo_Adj_Data_Clr();
            svoIramVar.svoCurrentSpeed = 0xff;
            svoCalVar.discChkDoneBits |= disc_chk_end;
            svoVar_discKindT.fDiscKind = eCDROMDisc;
            SendMsg80(SHOW_DEBUG_MSG,0x310041);
            return(SEQ_EX2);    /* restart offset ajust */
        }
        else
        {
            SendMsg80(SHOW_DEBUG_MSG,0x310041);
            svoCalVar.discChkDoneBits |= disc_chk_end;
            svo_preset_media_par();
            return(SEQ_EX0);
        }
    }

    if (svoIramVar.atipReadyFlag == TRUE)
    {
        //
        // not need to do disc kind change blocking for cd_dpd because the atip flag
        //
        if (svoVar_discKindT.fDiscKind == eCDROMDisc)
        {
            svoVar_discKindT.fDiscKind = eCDRDisc;
            SendMsg80(SHOW_DEBUG_MSG,0x3100A7); // ATIP_check==>CD-R Disc
        }

        SetTimeout(0, TIMER_STOP);
        svoCalVar.discChkDoneBits |= disc_chk_end;
        svo_preset_media_par();
        return(SEQ_EX0);
    }

    SetDelayTimer(ARG1);
    return(SEQ_BUSY);
}
#endif

/*-----------------------------------------------------------------------------
 *
 * Module: state_focus_err()
 * Description:
 *
 *----------------------------------------------------------------------------*/
seq_exit_t state_focus_err(void)
{
    svoCmdVar.svoCommand = CNOP;
    set_SIF(EXECUTIONSTATE,svoError);  /* set ExecutionState "svoError" */
    svoCmdVar.servoError = eErrorFocus;

#if (Customer_A1 ==1)
		CompleteStroke = TRUE;
	#endif

#if (FDP_TRKSLIP_MONITOR == 1)
    if (svoMiscVar.forceFocusOffFlag == TRUE)
    {
        return(SEQ_EX0);
    }
#endif

    svoSetServoOff();
#if (AFE_POWER_DOWN == 1)
    AFEPowerDown();
#endif

    return(SEQ_EX0);
}

/*-----------------------------------------------------------------------------
 *
 * Module: state_focus_track_off_err()
 * Description:
 *
 *----------------------------------------------------------------------------*/
seq_exit_t state_focus_track_off_err(void)
{
    svoCmdVar.svoCommand = CNOP;
    set_SIF(EXECUTIONSTATE,svoError);  /* set ExecutionState "svoError" */
    svoCmdVar.servoError = eErrorFocus;

    svoFocusOff();

    return(SEQ_EX0);
}

/*-----------------------------------------------------------------------------
 *
 * Module: state_focus_off()
 * Description:
 *
 *----------------------------------------------------------------------------*/
seq_exit_t state_focus_off(void)
{
#if (EN_WOBBLE == 1)
    if (svoVar_discKindT.fDiscKind == eBDDisc)
        HAL_WOBBLE_EnableNewWblAddInterrupt(FALSE);     /* Disable ATIP Interrupt - Stop ATUP decoding */
#endif
    WRITE_FIELD(HAL_CDD_SCOREN, 0);                 /* Disable SUB-Q interrupt - Stop SubQ decoding */

#if (FDP_TRKSLIP_MONITOR == 1)
    if (svoMiscVar.forceFocusOffFlag == TRUE)
    {
        svoFocusOff();
    }
    else
#endif
    {
//20101224_030U ==start== Fix HS-CDRW OPU lens hit disc issue, Frank modify Andy confirm
//        HAL_SERVO_SetFoos(0);
//20101224_030U ==end== Fix HS-CDRW OPU lens hit disc issue, Frank modify Andy confirm
        svoSetLDOff();
    }

#if (BD_ENABLE == 1)
    if (svoVar_discKindT.Bit.DiscStd == eDiscBD)
    {
        HAL_SERVO_SetTilt(TILT_INIT_DAC);
    }
#endif
    return(SEQ_EX0);
}

/*-----------------------------------------------------------------------------
 *
 * Module: state_servo_off()
 * Description:
 *
 *----------------------------------------------------------------------------*/
seq_exit_t state_servo_off(void)
{
     /* Disable black and white defect */
    /* Do not use DC filter for focus/tracking error signal when detecting defects */
    Defect_Enable(DEFECT_STATE_ALL_OFF);
    SetDfeWblFrontOnHold();
    set_SIF(STATE,svoStStoppingServo);              /* set state "svoStStoppingServo" */
    svoSetServoOff();

    return(SEQ_EX0);
}

/*-----------------------------------------------------------------------------
 *
 * Module: state_disc_start()
 * Description:
 *
 *----------------------------------------------------------------------------*/
seq_exit_t state_disc_start(void)
{
    SHORT delta_rpm_100ms;

    if (svoIramVar.spindleStopFlag == DSTARTED)
    {
        return(SEQ_EX0);
    }
    svoSpeedVar.SpdPresetNeeded = TRUE;
    //SendMsg80(SHOW_DEBUG_MSG,0x33FF0A); //Disc Diameter Check Start

    SetMotorDriverState(SHORT_BRAKE);

    if (DMO_CLIP_SLED_MOVING < svoSpeedVar.Kd14Fg)
    {
        while (svoStp_busy())
        {
            ExitProcess();
        }
    }

    svoIramVar.spindleStopFlag = DSTARTING;
    if ((svoVar_discDiameter == eUnknownDiameter) || (svoVar_discDiameter == eNoDisc))
    {

		#if (Customer_A1 ==1)
				if(svoVar_discKindT.Bit.DiscStd==0)
				{
						WRITE_DSP_COEF(kdf14,0x700);
				}
				else
				{
		        WRITE_DSP_COEF(kdf14,DISC_KICK_FORCE);
		    }
		#else
        WRITE_DSP_COEF(kdf14,DISC_KICK_FORCE);
		#endif
	
    }
    else
    {
        WRITE_DSP_COEF(kdf14,(svoSpeedVar.Kd14Fg * 0x80)/DMODAC_FACTOR);
    }
    WRITE_DSP_COEF(dmfbk_level, (short)DISC_KICK_LEVEL*DMODAC_FACTOR);
    WRITE_REG(HAL_SRVREG_MAP->DMFBK, HAL_SRV_DMFK); /* set to accelerate */

    delta_rpm_100ms = SvoDiameterJudge((svoVar_discDiameter == eUnknownDiameter) || (svoVar_discDiameter == eNoDisc));

    StopFGTimer();

    if (delta_rpm_100ms == (SHORT)(0x8000))
    {
        SendMsg80(SHOW_DEBUG_MSG,0x310004);     //DISC KICK time out
        return(SEQ_EX1);
    }
    else
    {
        svoIramVar.spindleStopFlag = DSTARTED;
        }

    if ((svoVar_discDiameter == eUnknownDiameter) || (svoVar_discDiameter == eNoDisc))
        {
        if (delta_rpm_100ms >= (SHORT)(NO_DISC_TH * 100))
        {   /* No Disc */
            svoVar_discDiameter = eNoDisc;
            SendMsg80(SHOW_DEBUG_MSG,0x310003); //DISK_Diameter==> no  Disc
            return(SEQ_EX1);
        }

#if (DISC_8CM == 1)
        if (delta_rpm_100ms >= (SHORT)(DISC_8CM_TH * 100))
        {   /* 8cm Disc */
            svoVar_discDiameter = e8cm;
            SendMsg80(SHOW_DEBUG_MSG,0x310001);//DISK_Diameter==> 8cm Disc
        }
        else
#endif // (DISC_8CM == 1)
        {
            svoVar_discDiameter = e12cm;
            SendMsg80(SHOW_DEBUG_MSG,0x310002);//DISK_Diameter==> 12cm Disc
        }
        //SendMsg80(SHOW_DEBUG_MSG,0x33FF0B); //Disc Diameter Check End

		#if (Customer_A1 ==1)
				if((svoVar_discKindT.Bit.DiscStd == eDiscUnknown)&&((svoVar_discDiameter==e12cm)||(svoVar_discDiameter==e8cm)))
				{
		///			SendMsgCnB(SHOW_DEBUG_MSG,4,0x90,0x00,0x00,B4B(5566));
						CompleteStroke = TRUE;
						return(SEQ_EX1);
				}
		#endif		
    }
    return(SEQ_EX0);
}

/*-----------------------------------------------------------------------------
 *
 * Module: state_disc_err()
 * Description:
 *
 *----------------------------------------------------------------------------*/
seq_exit_t state_disc_err(void)
{
    if(((svoIramVar.svoInState & TRN_MASK) == CSEEK))
    {
        svoCmdVar.svoCommand = CNOP;
        set_SIF(EXECUTIONSTATE,svoError);       /* set ExecutionState "svoError" */
        svoCmdVar.playerError = eErrorAccess;
        return(SEQ_EX0);
    }

    svoCmdVar.svoCommand = CNOP;
    set_SIF(EXECUTIONSTATE,svoError);       /* set ExecutionState "svoError" */
    svoCmdVar.servoError = eErrorDiscSpeed;
    return(SEQ_EX0);
}

/*-----------------------------------------------------------------------------
 *
 * Module: state_disc_stop_wait()
 * Description:
 *
 *----------------------------------------------------------------------------*/
seq_exit_t state_disc_stop_wait(void)
{
    static USHORT prevFgPulseTime;

    if (timeout_chk(0) == eTimerTimeout)
    {   /* time out check */
        SendMsg80(SHOW_DEBUG_MSG,0x31000C);//DISKSTOP ==> Timeout
        discstop();
        svoIramVar.spindleStopFlag = DSTOPPED;
        return(SEQ_EX1);
    }

    if (svoIramVar.svoPreInState != svoIramVar.svoInState)
    {
        prevFgPulseTime = svoSpeedVar.fgPulseTime;
        SetTimeout(1, DISC_STOP_NO_FG_TIME);
    }

    if (svoIramVar.spindleStopFlag != DSTOPPED)
    {
        if (svoSpeedVar.fgPulseTime == prevFgPulseTime)
        {
            if (timeout_chk(1) == eTimerTimeout)
            {
                SetTimeout(0, TIMER_STOP);
                discstop();
                svoIramVar.spindleStopFlag = DSTOPPED;
                return(SEQ_EX0);
            }
        }
        else
        {
            SetTimeout(1, DISC_STOP_NO_FG_TIME);
        }
        prevFgPulseTime = svoSpeedVar.fgPulseTime;
    }
    else
    {
        SetTimeout(0, TIMER_STOP);
        SetDelayTimer(MS_100);
        return(SEQ_EX0);
    }

    SetDelayTimer(ARG1);
    return(SEQ_BUSY);
}

/*-----------------------------------------------------------------------------
 *
 * Module: state_disc_stop()
 * Description:
 *
 *----------------------------------------------------------------------------*/
seq_exit_t state_disc_stop(void)
{
    USHORT step_target;
    SHORT  dmfbk_val;

#if  SERVOKEYPARAMETER == 1
    if((svoIramVar.svoInState & TRN_MASK) == TRN_017)//TRAYOUT_TBL
    {
        if(((VendorServoParaSwitch == TRUE)||(ServoParaSwitch==TRUE))
           #if (SUN710_TRAY_PROFILE == 1) || ( EEPROM_TRAY_PROFILE ==1)
            &&(fTuneMode ==0)   // if tuning tray profile, don't update key parameter!!
            #endif
            )
        {
            SendMsg80(SHOW_DEBUG_MSG,0x33FF27); //savekeypara
            SetServoKeyParameter();
            ServoParaSwitch = FALSE;
        }
    #if (STORE_MSG_TO_FLASHROM==1)
        if(SaveLogToROMSwitch == TRUE)
        {
            SendMsg80(SHOW_DEBUG_MSG,0x33FF28); //saveLog
            CopyMsgFromDataAreaToROM();
            SaveLogToROMSwitch = FALSE;
        }
    #endif
    }
#elif (STORE_MSG_TO_FLASHROM==1)
    if((svoIramVar.svoInState & TRN_MASK) == TRN_017)//TRAYOUT_TBL
    {
#if (SUN710_TRAY_PROFILE == 1) || ( EEPROM_TRAY_PROFILE ==1)
        if(fTuneMode ==0)   // if tuning tray profile, don't update message!!
    #endif
        {
            if(SaveLogToROMSwitch == TRUE)
            {
                send_msg5S(1,"saveLog");//Temp close for hang issue
                CopyMsgFromDataAreaToROM();
                SaveLogToROMSwitch = FALSE;
            }
        }
    }
#endif
#if (BD_ENABLE == 1)
    if (svo_BDR_RE_PICReading == SET)
    {
        //Exiting the PIC zone
        SendMsg80(SHOW_DEBUG_MSG,0x334700);
        svo_BDR_RE_PICReading = CLEAR;
        LAL_PLL_SetPll();
        LAL_SetResetPIC(eRESET_PIC, svo_bReadZoneSpeedX10);
    }
#endif
    if (Iram_stp_Initialized == STEP_INITIALIZED)
    {
        if ((svoIramVar.svoInState & TRN_MASK) == COPEN)
        {
#if DOUBLE_LENS
            if (IS_OUTER_LENS(svoVar_discKindT.Bit.DiscStd))
            {
                Iram_stp_CurrentPosition -= STEP_LENS_OFFSET;
            }
#endif // DOUBLE_LENS
            step_target = STEP_TRAY_OUT_POSITION;
        }
        else
        {
            step_target = svoStepVar.StepHomePos;
#if (DVD_RAM_READ == 1)
            if (DVDRAM)
            {
                target_zone = 1;
            }
#endif
        }

        if (Iram_stp_CurrentPosition != step_target)
        {
            SetMotorDriverState(SHORT_BRAKE);
            svoStp_JumpToPosition(STEP_SAFE_SLOW_MODE, step_target);
        }
    }

    if (get_driver_state() == ALL_MUTE_ON)
    {
        return(SEQ_EX1);
    }

    if ((svoIramVar.svoInState & TRN_MASK) == COPEN)
    {
        vWakeUpChipOnlyADDA();
    }

    if (svoIramVar.spindleStopFlag == DSTOPPED)
    {
        return(SEQ_EX1);
    }

    SetMotorDriverState(SHORT_BRAKE);

    dmfbk_val = (SHORT)DISC_BRAKE_LEVEL*DMODAC_FACTOR;
    WRITE_DSP_COEF(dmfbk_level, dmfbk_val);
    WRITE_REG(HAL_SRVREG_MAP->DMFBK, HAL_SRV_DMBK);   /* set to decelerate */
#if (((DMO_CLIP_SLED_MOVING * 0x80) / DMODAC_FACTOR) < DISC_STOP_BRAKE_FORCE3)
    WRITE_DSP_COEF(kdf14, (DMO_CLIP_SLED_MOVING/DMODAC_FACTOR) * (DISC_KICK_MAX / 0x80));
    while (svoStp_busy())
    {
        ExitProcess();
    }
#endif
    WRITE_DSP_COEF(kdf14,DISC_STOP_BRAKE_FORCE3);
    SetMotorDriverState(REVERSE_BRAKE);



    spdStop();

    svoIramVar.spindleStopChkCnt = 0;
    svoIramVar.spindleStopFlag = DSTOPPING; /* Error check STOP */
    svoSpeedVar.fgCount = 0;
    StartFGTimer();
    SetTimeout(0, ARG2);
    svoMiscVar.zoneAreaCheckFlag = FALSE; //TRUE; //TBD? This flage should be ZCLV mode used only, we should review the ZCLV function in the future

    if ((svoIramVar.svoInState & TRN_MASK) == CSTOP)
    {
        set_SIF(STATE, svoStStoppingServo); /* set state "svoStStoppingServo" */
    }

    svoIramVar.svoCurrentSpeed = 0xFF; // clear current speed information!!
    svoSpeedVar.SpdCurrentSpeed = 0xFF;

    return(SEQ_EX0);
}

/*-----------------------------------------------------------------------------
 *
 * Module: state_disk_svoON()
 * Description:
 *
 *----------------------------------------------------------------------------*/
seq_exit_t state_disk_svoON(void)
{
    spdSetKd14(svoSpeedVar.Kd14Fg);
    WRITE_FIELD(HAL_SRV_IFOK,1);

    if ((svoCalVar.calGainDoneBits & feg_bsg_end) == FALSE)
    {
        spdHoldRPM(FEpp_MEAS_RPM);
    }

#if (SRC_ENABLE == 1)
    WRITE_DSP_COEF(frot8x, 40*8); //init at 40Hz
#endif

    return(SEQ_EX0);
}

/*-----------------------------------------------------------------------------
 *
 * Module: state_disc_off()
 * Description:
 *
 *----------------------------------------------------------------------------*/
seq_exit_t state_disc_off(void)
{
    if (!svoStp_busy())
    {
        HAL_SERVO_SetFoos(0);

        StartDiscStopWithTrayTimer();
        SetMotorDriverState(ALL_MUTE_ON);

        if(ARG3 == 0)
        {
            if ((svoCalVar.discChkDoneBits & disc_chk_end) == disc_chk_end)
            {
                set_SIF(STATE, svoStIdleDiscKnown);         /* set state "svoStIdleDiscKnown" */
            }
            else
            {
                set_SIF(STATE, svoStIdleDiscUnknown);       /* set state "svoStIdleDiscUnknown" */
            }
#if (AFE_POWER_DOWN == 1)
            AFEPowerDown();   /* AFE Power OFF */
#endif /* AFE_POWER_DOWN */
        }
        return(SEQ_EX0);
    }
    SetDelayTimer(MS_1);
    return(SEQ_BUSY);
}

/*********************************************************
* Module: state_dsp_start()
*
* Description: For TG/FG adjustment use
*
* Returns: SHORT
* Params:  void
**********************************************************/
typedef struct Dsp_adjust_tbl{
    ULONG       command;
    BYTE        sw;
    WORD        end_flag;
    BYTE        read_addr;
    USHORT      write_addr;
    USHORT      init;
}DSP_ADJUST_TBL;

const DSP_ADJUST_TBL dsp_adjust_tbl[] = {
    {HAL_SRV_FGAT_ADR, (HAL_SRV_FGAT|HAL_SRV_FGST|(HAL_SRV_FGW&0x01)), HAL_SRV_FGEND, halSRV_RSEL_FGA, kfg, 0x4000},   /* focus gain adjustment */
    {HAL_SRV_TGAT_ADR, (HAL_SRV_TGAT|HAL_SRV_TGST|(HAL_SRV_TGW&0x00)), HAL_SRV_TGEND, halSRV_RSEL_TGA, ktg, 0x4000},   /* tracking gain adjustment */
};

typedef struct State_dsp_adjust_tbl{
    ULONG           adjust_end;
    DSP_ADJUST_TBL  *sub_tbl;
    void            (*func_prepare)(void);  //executed in the beginning of AGC
    SHORT           (*func_stop)(void);  //executed at the end of AGC
}STATE_DSP_ADJUST_TBL;

const STATE_DSP_ADJUST_TBL  state_dsp_adjust_tbl[]={
    {felg_end, (DSP_ADJUST_TBL *)&dsp_adjust_tbl[FGAJ],   fgdsp_prepare,   feg_stop},   /* FEGA */
    {telg_end, (DSP_ADJUST_TBL *)&dsp_adjust_tbl[TGAJ],   tgdsp_prepare,   teg_stop},   /* TEGA */
};

#if (AGC_VERIFY_TEST!=0)
static BYTE agc_count = AGC_VERIFY_TEST;
#endif

// TBD!
seq_exit_t state_dsp_start(void)
{
    STATE_DSP_ADJUST_TBL *now_state_dsp;

#if (PLANT_MODELING_SW==1)
    return(SEQ_EX1);
#endif

    now_state_dsp = (STATE_DSP_ADJUST_TBL *)&state_dsp_adjust_tbl[ARG3];
    if ((svoCalVar.calGainDoneBits & (now_state_dsp->adjust_end)) == (now_state_dsp->adjust_end))
    {
        return(SEQ_EX1);
    }

    switch (ARG3)
    {
        case FGAJ:
            SendMsg80(DEBUG_CALIB_ENTRYS_MSG,0x33FF06); //==Focus loop Gain Adjustment Start==

#if (SRC_ENABLE == 1) && (SRC_ENABLE_FOCUS == 1)
            HAL_DSP_SRCHold(SRC_LOOP_FOCUS, ON, ON, ON);
#endif
            break;

        case TGAJ:
            if ((Layer_Index == 1) && !(svoCalVar.calGainDoneBits & teg_end))
            {
                return(SEQ_EX1);
            }
            SendMsg80(DEBUG_CALIB_ENTRYS_MSG,0x33FF07); //==Track loop Gain Adjustment Start==

#if (SRC_ENABLE == 1)
            HAL_DSP_SRCHold(SRC_LOOP_TRACKING, ON, ON, ON);
#endif
            break;
    }
#if ((BD_2X_STARTUP == 1) && (BD_UNBALANCE_DISC_CHECK))
    if((svoVar_unbalanceDiscNum != 0)&&(svoVar_discKindT.Bit.DiscStd == eDiscBD)
       &&(svoIramVar.svoCurrentSpeed == _2LX_BD))
       spdHoldRPM(svoSpeedVar.SpdInRange? svoSpeedVar.TargetRpm : svoSpeedVar.CurrentRpm);
#endif


    /* Write Adjustment Init data */
    WRITE_REG(HAL_SRVREG_MAP->RGSEL,(now_state_dsp->sub_tbl)->read_addr);
#if (AGC_VERIFY_TEST==0)
    WRITE_REG(HAL_SRVREG_MAP->DASET,(now_state_dsp->sub_tbl)->init);
#else
    if (agc_count & 0x01)
        WRITE_REG(HAL_SRVREG_MAP->DASET, (now_state_dsp->sub_tbl)->init * 1.3);    /* write register data */
    else
        WRITE_REG(HAL_SRVREG_MAP->DASET, (now_state_dsp->sub_tbl)->init * 0.7);    /* write register data */
#endif

    now_state_dsp->func_prepare();

    WRITE_REG(*(BYTE *)((now_state_dsp->sub_tbl)->command), (now_state_dsp->sub_tbl)->sw);

    SetDelayTimer(ARG1);
    SetTimeout(0, ARG2);
	
#if (KFGTG_VAL_WI_BOUNDARY == 1)
  	if (ARG3 == FGAJ)
  	{
  		KFG_OK_LEVEL_mv=(((LONG)(0xFFFF-(USHORT)READ_DSP_COEF(kf10))*GPADC_IN_RANGE)/128/FOCUS_SERVO_INPUT_GAIN)*KFG_THRE_FACTOR;
  		if (KFG_OK_LEVEL_mv <KFG_mv_LOW_LIMIT) 
  			KFG_OK_LEVEL_mv=KFG_mv_LOW_LIMIT;
  	}	
    if (ARG3 == TGAJ)
    {
		KTG_OK_LEVEL_mv=(((LONG)(0xFFFF-(USHORT)READ_DSP_COEF(kt10))*GPADC_IN_RANGE)/128/TRACKING_SERVO_INPUT_GAIN)*KTG_THRE_FACTOR;
		if (KTG_OK_LEVEL_mv <KTG_mv_LOW_LIMIT) 
			KTG_OK_LEVEL_mv=KTG_mv_LOW_LIMIT;
    }	
  	bsamplecnt=0;
  	LadTE=0;
  	LadFE=0;
#endif	
    return(SEQ_EX0);
}

/*-----------------------------------------------------------------------------
 *
 * Module: state_dsp_end_chk()
 * Description:
 *
 *----------------------------------------------------------------------------*/
seq_exit_t state_dsp_end_chk(void)
{
    STATE_DSP_ADJUST_TBL *now_state_dsp;
#if (KFGTG_VAL_WI_BOUNDARY == 1)
	ad_dsp_t   te_signal,fe_signal;
	USHORT	   dsp_result;
#endif

    if (timeout_chk(0) == eTimerTimeout)
    {   /* time out check */
        SendMsg80(DEBUG_AUTO_TUNING_MSG,0x317503); //Auto_Adjust_State==TIMEOUT!!!
        return(SEQ_EX1);
    }

    now_state_dsp = (STATE_DSP_ADJUST_TBL *) &state_dsp_adjust_tbl[ARG3];

#if (KFGTG_VAL_WI_BOUNDARY == 1)
		if (ARG3==FGAJ)
		{
			if (bsamplecnt<KFG_OK_MES_WIN)
			{ //FEp2p 
			    fe_signal = ReadDSPADCmV(NO_REVOLUTION, FE_ALL, TRUE);
				if (fe_signal.errcode == eNOERR)
				{
					LadFE += fe_signal.p2p_mv;
					bsamplecnt++;
				}
			}
			
			if (bsamplecnt>=KFG_OK_MES_WIN && (bsamplecnt >0))
			{
				LadFE /=bsamplecnt;
				bsamplecnt=0;
				if (LadFE > KFG_OK_LEVEL_mv)
				{
			        SendMsg80(DEBUG_AUTO_TUNING_MSG,0x317510); //FE Amplitute over boundary
					SetTimeout(0, TIMER_STOP);
					return(SEQ_EX1);
				}
				LadFE=0;
				WRITE_REG(HAL_SRVREG_MAP->RGSEL,(now_state_dsp->sub_tbl)->read_addr);	/* Read Adjustment data */
				dsp_result = READ_REG(HAL_SRVREG_MAP->SRCR);
				if ((dsp_result > FGA_KFG_UPPER_LIMIT) || (dsp_result < FGA_KFG_LOWER_LIMIT))
				{
					SendMsg80(DEBUG_AUTO_TUNING_MSG,0x317512);  //DSP cal over Boundary
					SetTimeout(0, TIMER_STOP);
					return(SEQ_EX0);
				}
				
			}
		}

		if (ARG3==TGAJ)
		{
			if (bsamplecnt<KTG_OK_MES_WIN)
			{ //TEp2p 
				te_signal = ReadDSPADCmV(NO_REVOLUTION, TE_ALL, TRUE);
				if ((te_signal.errcode == eNOERR))
				{
					LadTE += te_signal.p2p_mv;
					bsamplecnt++;
				}
			}
			
			if (bsamplecnt>=KTG_OK_MES_WIN  && (bsamplecnt >0))
			{
				LadTE /=bsamplecnt;
				bsamplecnt=0;
				if (LadTE > KTG_OK_LEVEL_mv)
				{
			        SendMsg80(DEBUG_AUTO_TUNING_MSG,0x317511); //TE Amplitute over boundary
					SetTimeout(0, TIMER_STOP);
					return(SEQ_EX1);
				}
				LadTE=0;
				WRITE_REG(HAL_SRVREG_MAP->RGSEL,(now_state_dsp->sub_tbl)->read_addr);	/* Read Adjustment data */
				dsp_result = READ_REG(HAL_SRVREG_MAP->SRCR);
				if ((dsp_result > TGA_KTG_UPPER_LIMIT) || (dsp_result < TGA_KTG_LOWER_LIMIT))
				{
			        SendMsg80(DEBUG_AUTO_TUNING_MSG,0x317513); //DSP cal over Boundary
					SetTimeout(0, TIMER_STOP);
					return(SEQ_EX0);
				}
			}
		}
#endif

    if( READ_REG(HAL_SRVREG_MAP->SVSTATUS)&((now_state_dsp->sub_tbl)->end_flag) )
    {
        SendMsg80(DEBUG_AUTO_TUNING_MSG,0x317504); //Auto_Adjust_State==OK!!!
        SetTimeout(0, TIMER_STOP);
        return(SEQ_EX0);
    }

    return(SEQ_BUSY);
}

/*-----------------------------------------------------------------------------
 *
 * Module: state_dsp_stop()
 * Description:
 *
 *----------------------------------------------------------------------------*/
#if AFE_REG_DUMP
void disp_afe_para(void);
#endif
seq_exit_t state_dsp_stop(void)
{
    STATE_DSP_ADJUST_TBL *now_state_dsp;
    seq_exit_t ret_seq;
    USHORT     dsp_result;
#if  SERVOKEYPARAMETER == 1
    USHORT     tempjitter;
#endif

    if (ARG2 == 0)
    {
        now_state_dsp = (STATE_DSP_ADJUST_TBL *)&state_dsp_adjust_tbl[ARG3];
    }
    else
    {
        if ((svoIramVar.svoInState & TRN_MASK) == TRN_046)
        {
            now_state_dsp = (STATE_DSP_ADJUST_TBL *)&state_dsp_adjust_tbl[FGAJ];
        }
        else
        {
            now_state_dsp = (STATE_DSP_ADJUST_TBL *)&state_dsp_adjust_tbl[TGAJ];
        }
    }

    if ((now_state_dsp->func_stop() == OK) && ((ARG2 == 1) || FokStatusOK()))
    {
        ret_seq = SEQ_EX0;
    }
    else
    {
        SendMsg80(SHOW_DEBUG_MSG,0x317516);//Auto_Adjust_State==Error!!
        ret_seq = SEQ_EX1;
    }

    if (ARG2 == 0)
    {
        if (ret_seq == SEQ_EX0)
        {
            WRITE_REG(HAL_SRVREG_MAP->RGSEL,(now_state_dsp->sub_tbl)->read_addr);   /* Read Adjustment data */

#if (AGC_VERIFY_TEST!=0)
            SendMsgCnB(SHOW_DEBUG_MSG,4,0x33FF43,B1B(ARG2),B1B(ARG3),B1B(agc_count),B1B(READ_REG(HAL_SRVREG_MAP->SRCR)));//##AUTO_ADJ_Value=
            if (--agc_count > 0)
            {
                svoIramVar.svoInState = (svoIramVar.svoInState & (~SEQ_MASK)) | SEQ_001;
                return (SEQ_BUSY);
            }
            else
            {
                agc_count = AGC_VERIFY_TEST;
            }
#endif // (AGC_VERIFY_TEST!=0)

            dsp_result = READ_REG(HAL_SRVREG_MAP->SRCR);

            if ((svoVar_discKindT.Bit.Layers == eLayersSingle) || (Layer_Index == 1))
            {   /*dual layer disc, set end flag only when layer1 is done*/
                svoCalVar.calGainDoneBits |= (now_state_dsp->adjust_end);

#if AFE_REG_DUMP
                if (ARG3 == TGAJ)
                {
                    disp_afe_para();
                }
#endif
            }
            if (ARG3 == FGAJ)
            {
                if(ARG1 == NG)
                {
                    dsp_result = 0x4000;//(now_state_dsp->sub_tbl)->init;
                    SendMsg80(SHOW_DEBUG_MSG,0x320A1B);
                }
                else if (dsp_result > FGA_KFG_UPPER_LIMIT)
                {
                    dsp_result = FGA_KFG_UPPER_LIMIT;
                    SendMsg80(SHOW_DEBUG_MSG,0x320A1A);
                }
                else if (dsp_result < FGA_KFG_LOWER_LIMIT)
                {
                    dsp_result = FGA_KFG_LOWER_LIMIT;
                    SendMsg80(SHOW_DEBUG_MSG,0x320A19);
                }

#if (SERVO_BODE != BODE_NONE)
                dsp_result = 0x4000;
#endif
            }

            if (ARG3 == TGAJ)
            {
                if(ARG1 == NG)
                {
                    dsp_result = 0x4000;//(now_state_dsp->sub_tbl)->init;
                    SendMsg80(SHOW_DEBUG_MSG,0x320A18);
                }
                else if (dsp_result > TGA_KTG_UPPER_LIMIT)
                {
                    dsp_result = TGA_KTG_UPPER_LIMIT;
                    SendMsg80(SHOW_DEBUG_MSG,0x320A17);
                }
                else if (dsp_result < TGA_KTG_LOWER_LIMIT)
                {
                    dsp_result = TGA_KTG_LOWER_LIMIT;
                    SendMsg80(SHOW_DEBUG_MSG,0x320A16);
                }

#if (SERVO_BODE != BODE_NONE)
                dsp_result = 0x4000;
#endif
            }
            WRITE_DSP_COEF((now_state_dsp->sub_tbl)->write_addr, dsp_result);

            switch(ARG3)
            {
                case FGAJ:
                    calSetMisc(Layer_Index,FEGN_DSP,dsp_result);
                    SendMsg80(DEBUG_CALIB_ENTRYS_MSG,0x33FF08); //"Focus loop Gain Adjustment End"
#if (SRC_ENABLE == 1) && (SRC_ENABLE_FOCUS == 1)
                    HAL_DSP_SRCHold(SRC_LOOP_FOCUS, ON, ON, OFF);
#endif
                    break;
                case TGAJ:
                    calSetMisc(Layer_Index,TEGN_DSP,dsp_result);
                    SendMsg80(DEBUG_CALIB_ENTRYS_MSG,0x33FF09); //"Track loop Gain Adjustment End"
#if  SERVOKEYPARAMETER == 1
                    tempjitter = HAL_DFE_ReadJitterX10();
                    if (tempjitter > 250)  tempjitter = 250;
                    svoPrvtVar_jitter = tempjitter;
#endif
#if (SRC_ENABLE == 1)
                    HAL_DSP_SRCHold(SRC_LOOP_TRACKING, ON, ON, OFF);
#endif//SRC_ENABLE
                    break;
            }
        }
    }
    else
    {
        WRITE_DSP_COEF((now_state_dsp->sub_tbl)->write_addr, (now_state_dsp->sub_tbl)->init);
    }
#if ((BD_2X_STARTUP == 1) && (BD_UNBALANCE_DISC_CHECK))
    if((svoVar_unbalanceDiscNum != 0)&&(svoVar_discKindT.Bit.DiscStd == eDiscBD)
       &&(svoIramVar.svoCurrentSpeed == _2LX_BD))
       spdUnhold();
#endif
    return(ret_seq);
}

/*-----------------------------------------------------------------------------
 *
 * Module: state_trksvo_on()
 * Description: wait slow flag and tracking on
 *
 *----------------------------------------------------------------------------*/
seq_exit_t state_trksvo_on(void)
{
    SendMsg80(SHOW_DEBUG_MSG,0x33FF00); //---Track On---


#if (Customer_A1 ==1)//[W.0J]Walker
		if((CompleteStrokeCount<2)&&((svoIramVar.svoInState & TRN_MASK) == TRN_038))//[J50Z]Walker
		{

			
			if((svoVar_discKindT.Bit.DiscStd == eDiscBD)&&(svoVar_discKindT.Bit.Layers == eLayersDouble)&&((PositionHomeErrorCount > 20)||(CompleteStroke)))
			{
					PositionHomeErrorCount = 0;			
	        return(SEQ_EX4);
			}
			else if((svoVar_discKindT.Bit.DiscStd == eDiscBD)&&(svoVar_discKindT.Bit.Layers == eLayersSingle)&&((PositionHomeErrorCount > 9)||(CompleteStroke)))
			{
				if((svoVar_discKindT.Bit.Media   == eMediaROM)&&(svoIramVar.LTHDisc == TRUE))
				{
					MediaRecovery = TRUE;
				}

				PositionHomeErrorCount = 0; 		
							return(SEQ_EX4);
			}
			else if (((svoVar_discKindT.fDiscKind == eDVDROMDisc)||(svoVar_discKindT.fDiscKind == eDVDROMDLDisc))&&((PositionHomeErrorCount > 9)||(CompleteStroke)))
			{
				PositionHomeErrorCount = 0; 		
							return(SEQ_EX4);							
			}			
			else if ((svoVar_discKindT.fDiscKind == eDVDMinusRWDisc)&&((PositionHomeErrorCount > 20)||(CompleteStroke))) //[S51A]daniel for A1 disc (DVD-RW) retry too much times
			{
				PositionHomeErrorCount = 0; 		
							return(SEQ_EX4);							
			}			
			else if (((svoVar_discKindT.fDiscKind == eDVDPlusRDisc)||(svoVar_discKindT.fDiscKind == eDVDPlusRWDisc)||(svoVar_discKindT.fDiscKind == eDVDMinusRDisc))&&((PositionHomeErrorCount > 9)||(CompleteStroke)))
				{
				PositionHomeErrorCount = 0; 		
							return(SEQ_EX4);							
				}
			else if (((svoVar_discKindT.fDiscKind == eDVDPlusRDLDisc)||(svoVar_discKindT.fDiscKind == eDVDPlusRWDLDisc)||(svoVar_discKindT.fDiscKind == eDVDMinusRDLDisc)||(svoVar_discKindT.fDiscKind == eDVDMinusRWDLDisc))&&((PositionHomeErrorCount > 20)||(CompleteStroke))) //[W0L]daniel for A1 disc (DVD+R DL) retry too much times
			{
					PositionHomeErrorCount = 0;			
	        return(SEQ_EX4);
			}
			else if ((svoVar_discKindT.Bit.DiscStd == eDiscCD)&&((PositionHomeErrorCount > 16)||(CompleteStroke)))
			{
				PositionHomeErrorCount = 0;			
	                        return(SEQ_EX4);
			}

						
		}
#endif

#if(ERROR_CAL_START_UP_MONITOR ==1)
    if(((svoIramVar.svoInState & TRN_MASK) == SVO_TRON)&&(svoMiscVar.calibrationErrorCount >= 12))
    {
        track_off();
        svoFocusOff();
        svoAFEInitOnly();
        svo_Adj_Data_Clr();

    #if (EN_POWER_CONTROL == 1)
        PowerControl(eStartUpPower);
    #endif
        Layer_Index = 0;
        RetryDiscLaser = eDiscUnknown;
        send_msg5SValue(1, "ReDoSTM", 1, svoMiscVar.calibrationErrorCount);
        return(SEQ_EX3);
    }
#endif

#if (HOLD_LENS_BEFORE_TRKON == 1)
    seekSetCEEq(eDamper, (svoVar_discKindT.Bit.DiscStd == eDiscCD)? CE_GAIN_DOWN_RATIO_CD: CE_GAIN_DOWN_RATIO_DVD);
    HAL_SERVO_CenterControlON(FORW);  // Turn Center-servo on
    WaitTimer(MS_10);          // Wait for Center-servo holding-on lens
#else
    seekSetCEEq(eRoughSeek, 0);
#endif

#if (DVD_RAM_READ == 1)
    if (DVDRAM_rewritable)
    {
        if ((Iram_stp_CurrentPosition - STEP_HOME_POSITION_DVDRAM) < -UM_2_SLED_STEP(800))
        {
            // Sledge is possible not in re-writable area due to skating.
            // Move sledge back to home-position for sure.
            svoStp_JumpToPosition(STEP_SAFE_SLOW_MODE, STEP_HOME_POSITION_DVDRAM);

            while(svoStp_busy())
            {
                ExitProcess();
            }
            svoIramVar.currentID.SectorNum.all32s = svoRadius2PBA(Iram_stp_CurrentPosition, 0);
        }
    }
#endif
#if (OPEN_SESSION==1)
    if (((svoVar_TrackingMethod != TRK_METHOD_DPP)
    #if (WOBBLE_READ_ON_DVD_OR_CD == 0)
        || (svoVar_discKindT.Bit.DiscStd == eDiscCD) || (svoVar_discKindT.Bit.DiscStd == eDiscDVD)
    #endif
        ) && (RFpp_chk() == FALSE))
    {
        HAL_SERVO_CenterControlOFF();
        //Move a little distance after find RF zone and modify search rf flow.
        return(SEQ_EX4);
    }
#endif

    wait_track_on(TRK_ON_WAIT_SLOW_TIMEOUT);

#if (FDP_TRKSLIP_MONITOR == 0)
    svoMiscVar.trackSlipCount = 0;
#endif

    return(SEQ_EX0);
}

/*-----------------------------------------------------------------------------
 *
 * Module: state_track_err()
 * Description:
 *
 *----------------------------------------------------------------------------*/
seq_exit_t state_track_err(void)
{
    svoCmdVar.svoCommand = CNOP;
    set_SIF(EXECUTIONSTATE,svoError);               /* set ExecutionState "svoError" */
    svoCmdVar.servoError = eErrorOffTrack;

#if (Customer_A1 ==1)
		CompleteStroke = TRUE;
	#endif
	
    return(SEQ_EX0);
}

/*-----------------------------------------------------------------------------
 *
 * Module: state_pll_init()
 * Description:
 *
 *----------------------------------------------------------------------------*/
seq_exit_t state_pll_init(void)
{
    SendMsg80(DEBUG_CALIB_ENTRYS_MSG,0x3A011C); // >>> STATE_PLL_INIT <<<

    //Initialize pll_chk "state" variables
    NrTryOutCheckRfPll = 0;
    mEqSettings = lalDFE_EqualizerStrengthNormal;
#if (EN_WOBBLE == 1)
    StateCheckWobblePll = TRUE;
    NrTryOutCheckWobblePll = 0;
    NrTryOutSyncFlyWheel = 0;
#endif // (EN_WOBBLE == 1)

    return(SEQ_EX0);
}

/*-----------------------------------------------------------------------------
 *
 * Module: state_trksvo_chk()
 * Description:
 *
 *----------------------------------------------------------------------------*/
seq_exit_t state_trksvo_chk(void)
{
    BYTE pass;

    pass = FALSE;
    if (svoIramVar.svoPreInState != svoIramVar.svoInState)
    {
        SendMsg80(DEBUG_CALIB_ENTRYS_MSG,0x3A0118); //>>> STATE_TRKSVO_CHK <<<

#if (DVD_RAM_READ == 1)
        if (DVDRAM)
        {
            USHORT WobbleSpeed;
            SHORT  offset, i;

            // Enable PID ready interrupt
            WRITE_FIELD(HAL_DVDD_PIDEN,1);

            // Header flywheel is on, read an set correct header offset value.
            if (HAL_WOBBLE_GetSyncFlyWheelInLock())
            {
                // Check if the loaded disc is a V1.0 disc.
                if (!(svoCalVar.discChkDoneBits & dvdram_v10_chk_end))
                {
                    WobbleSpeed = HAL_WOBBLE_GetOverspeed(16);

                    // DVDRAM_VER_DET: WobbleSpeed (X100) = %d
                    SendMsgCnB(SHOW_DEBUG_MSG,2,0x331320,B2B(WobbleSpeed));

                    if (WobbleSpeed < DVDRAM_V10_WOB_SPD_TH)
                    {
                        DiscTypeIndex   = LOC_DISC_TYPE_10;
                        LocalZoneNumber = NUM_ZONES_RAM_10;
                        memcpy(DVDRAMZoneTable, DVDRAMZoneTableV10, sizeof(DVDRAMZoneTableV10));
                        svo_preset_media_par();
                        SetSvoEqBank(Tracking_Eq, dvRamSvoEQCoefTbl[svoIramVar.svoCurrentSpeed][TRACKING_EQ], TRACKING_FACTOR_DVDRAM_VER_10);
                        SetSvoEqBank(Tracking_U_Eq, dvRamSvoEQCoefTbl[svoIramVar.svoCurrentSpeed][TRACKING_EQ], TRACKING_GAINUP_FACTOR_DVDRAM * TRACKING_FACTOR_DVDRAM_VER_10 / 128);
                        spdUpdateRPM(Iram_stp_CurrentPosition);
                        SendMsg80(SHOW_DEBUG_MSG,0x331321);       // DVDRAM_VER_DET ==> DVD-RAM V1.0 Disc
                    }
                    else
                    {
    #if (DISC_8CM == 1)
                        if (svoVar_discDiameter == e8cm)
                        {
                            // 8cm Disc
                            DiscTypeIndex   = LOC_DISC_TYPE_20_8CM;
                            LocalZoneNumber = NUM_ZONES_RAM_20_8CM;
                            memcpy(DVDRAMZoneTable, DVDRAMZoneTableV20_8CM, sizeof(DVDRAMZoneTableV20_8CM));
                            SendMsg80(SHOW_DEBUG_MSG,0x331323);   // DVDRAM_VER_DET ==> DVD-RAM V2.0 8cm Disc
                        }
                        else
    #endif
                        {
                            // 12cm Disc
                            SendMsg80(SHOW_DEBUG_MSG,0x331322);   // DVDRAM_VER_DET ==> DVD-RAM V2.0 12cm Disc
                        }
                    }

                    svoCalVar.discChkDoneBits |= dvdram_v10_chk_end;
                }

                WaitTimer(10);  // To let AOC stablize
                offset = 0;
                for (i=0; i<64; i++)
                {
                    offset += READ_FIELD(HAL_DFE_ODHDRMON);
                    Delay100uS(1);
                }
                RfHeaderOffset = offset/64;
                WRITE_FIELD(HAL_DFE_OFFDYNHDR, RfHeaderOffset);
                WRITE_FIELD(HAL_DFE_PREOFFHDR, 1);
            }
        }
#endif

        SetTimeout(0,MS_500);
    }

    if (timeout_chk(0) == eTimerTimeout)
    {
#if (Customer_A1 ==1)
				PositionHomeErrorCount++;	
#endif
    
        SendMsgCn(SHOW_DEBUG_MSG,1,0x3339FF,A1B(svoGetIDErrCount));   // WARNING: SubQ/PSN Check Timeout

        svoGetIDErrCount++;

#if (WOBBLE_READ_ON_DVD_OR_CD == 1) && (EN_WOBBLE == 1)
    #if (DISCID_RECOVERY == 1)
        // 1. We only need to set low enough "DVDR_TH"/"DVDRDL_TH" values to guarantee not miss-judge DVDR as DVDROM (which has no protection to re-judge)
        // 2. Disc Type change should only for Discs judged as DVD+-R/RW use. If Layer0 judge Disc pass, disc type should not be changed again on Layer1.
        #if (DVD_RAM_READ == 1)
        if ((svoVar_discKindT.Bit.DiscStd == eDiscDVD)&&(svoVar_discKindT.Bit.Media != eMediaROM)&&(Layer_Index == 0)&&(!DVDRAM))
        #else
        if ((svoVar_discKindT.Bit.DiscStd == eDiscDVD)&&(svoVar_discKindT.Bit.Media != eMediaROM)&&(Layer_Index == 0))
        #endif
        {
            if (svoGetIDErrCount <= 2)
            {
                switch (svoVar_discKindT.Bit.WblType)
                {
                    case  eWblMinus:
                        svoVar_discKindT.Bit.WblType = eWblPlus;
                        SendMsg80(SHOW_DEBUG_MSG,0x31009D);   // == Try DVD+ Disc ==>
                        break;

                    case  eWblPlus:
                        svoVar_discKindT.Bit.WblType = eWblMinus;
                        SendMsg80(SHOW_DEBUG_MSG,0x31009E);   // == Try DVD- Disc ==>
                        break;
                }
                afeSetWobbleForASpeed(svoIramVar.svoCurrentSpeed); // This function will do a subset of LAL_SetReadMode (wobble and specific afe coefs)
                svoCalVar.calGainDoneBits &= (~dif_WOB_end);
                track_off();

                return(SEQ_EX3);
            }
            else
            {
                if ((svoVar_discKindT.Bit.Media != eMediaRW)&&(svoVar_discKindT.Bit.WblType == eWblPlus))
                {
                    svoVar_discKindT.Bit.WblType = eWblUnknown;
                    svoVar_discKindT.Bit.Media   = eMediaROM;

                    track_off();

                    LAL_StoreParams(svoVar_discKindT, svoIramVar.svoCurrentSpeed);
                    LAL_SetReadMode();

                    afeSetTrackingMethod(getTrackingMethod(svoVar_discKindT));

                    svoCalVar.calGainDoneBits &= ~(tebg_end | teg_end | rpg_end);
                    SendMsg80(SHOW_DEBUG_MSG,0x33FF41+svoVar_discKindT.Bit.Layers-1);  // ADIP/LPP_check==>DVD-ROM xL Disc
                    return(SEQ_EX2);       // Go back to DVDR detection table
                }
            }
        }
        if ((svoGetIDErrCount < 3) && (!(svoCalVar.discChkDoneBits & start_end)) && (Layer_Index == 0))
        {
            SetTimeout(0, SEC_1);
            return(SEQ_BUSY);
        }
        else
        {
            svoGetIDErrCount = 0;
            return(SEQ_EX0);
        }
    #else  //(DISCID_RECOVERY == 1)
        return(SEQ_EX1);
    #endif //(DISCID_RECOVERY == 1)
#else
        switch (svoVar_discKindT.Bit.DiscStd)
        {
            case eDiscCD:
            case eDiscDVD:
            case eDiscBD:
                if (svoGetIDErrCount < 3)
                {
                    if (St_drv_ready == FALSE)
                    {
                        svoCalVar.calGainDoneBits &= (~(dif_RF_end | dif_WOB_end));
                        SendMsg80(SHOW_DEBUG_MSG,0x33FF29); //retry diffRF
                    }
                    track_off();
                    svoStp_JumpSteps(STEP_SAFE_SLOW_MODE,-UM_2_SLED_STEP(350));

                    while (svoStp_busy())
                    {
                        ExitProcess();
                    }
                    return(SEQ_EX3);
                }
                else
                {
                    //If Address(ID/SubQ) always can't read, then return fail
    #if (ENABLE_BDRLTH == 1)
                    if ((St_drv_ready) || (svoIramVar.LTHDisclikely == 1))
    #else
                    if (St_drv_ready)
    #endif
                    {
                        SendMsg80(SHOW_DEBUG_MSG,0x33FF2A); //Pass trksvo_chk
                        pass = TRUE;
                    }
                    else
                    {
                        svoGetIDErrCount = 0;
                        SendMsg80(SHOW_DEBUG_MSG,0x33FF4D); //trksvo_chk NG
                        return(SEQ_EX1);
                    }
                }
                break;
        }
#endif
    }

    if (svoVar_discKindT.Bit.DiscStd != eDiscCD)
    {
        if (CheckHeaderID() == TRUE)
        {
            pass = TRUE;
            SendMsg80(SHOW_DEBUG_MSG,0x334011); //CheckHeaderID() is TRUE
            if ((svoVar_discKindT.Bit.Layers == eLayersDouble) && (Layer_Index != svoIramVar.currentID.Layer))
            {
                SendMsg80(SHOW_DEBUG_MSG,0x33400C);
                if (svoCalVar.discChkDoneBits & start_end)
                {
                    calUpdateLayer(!Layer_Index);
                }
                else
                {
                    svoStartUpWrongLayer();

                    return(SEQ_EX4);
                }
            }
#if (ENABLE_HDDVD == 1)
            if (svoVar_discKindT.Bit.DiscStd == eDiscHD)
            {
                if (svoCmdVar.demodMode == eDemodETM)
                {
                    SendMsg80(SHOW_DEBUG_MSG,0x334013); //HD DEMOD ETM OK!
                }
    #if (ENABLE_CBHD == 1)
                else if (!(svoCalVar.discChkDoneBits & cbhd_disc_chk_end)&&(svoCmdVar.demodMode == eDemodFSM))
                {
                    svoVar_discKindT.Bit.Media = eMediaChina;
                    svoCalVar.discChkDoneBits |= cbhd_disc_chk_end;  /* CBHD is already check */
                    SendMsg80(SHOW_DEBUG_MSG,0x334014); //HD DEMOD FSM OK!
                }
    #endif
            }
#endif
        }
        else
        {
            SendMsg80(DEBUG_AUTO_TUNING_MSG,0x334012); //CheckHeaderID() is FALE
        }
    }
    else
    {
        //Need to check if we need to turn off SDefect to make servo work properly */
#if ((EN_WOBBLE == 1) && (WOBBLE_READ_ON_DVD_OR_CD == 1)) // Support ATIP Only when wobble is enabled
        if (((svoCalVar.discChkDoneBits & disc_chk_end) && (checkTimeData() == TRUE)) ||
            ((READ_FIELD(HAL_CDD_SCOREN) != 0) && (svoIramVar.subQReadyFlag == TRUE)) ||
            (HAL_WOBBLE_GetNewAddIntEnable() && (svoIramVar.atipReadyFlag == TRUE)))
#else
        if (((svoCalVar.discChkDoneBits & disc_chk_end) && (checkTimeData() == TRUE)) ||
            ((READ_FIELD(HAL_CDD_SCOREN) != 0) && (svoIramVar.subQReadyFlag == TRUE)))
#endif // ((EN_WOBBLE == 1) && (WOBBLE_READ_ON_DVD_OR_CD == 1))
        {
            SendMsg80(DEBUG_CALIB_ENTRYS_MSG,0x3A0000);// ==>Calib Pass:-))
            pass = TRUE;
        }
    }

    if (pass == TRUE)
    {
        svoGetIDErrCount = 0;

#if WIDER_TOK_LVL_FOR_PLAY_MODE == 1
        Set_TOK_Mode(TOK_PLAY);
#else
        #if (CHIP_REV <= 0xB3)
        WRITE_FIELD(HAL_AFE_TOKLVL, halAFE_TOKLVL_VREF_600mV);
        #else
        WRITE_FIELD(HAL_AFE_TOKLVL, halAFE_TECP_TECN_LVL_600mV);
        #endif
#endif
        SetTimeout(0, TIMER_STOP);

        /* need to turn on defect detection */
        /* Use DC filter for focus/tracking error signal when detecting defects */
        /* Enable TE and FE Hold at defect */
        Defect_Enable(DEFECT_STATE_ALL_ON|DEFECT_SOURCE_DFE_RF);

        return(SEQ_EX0);
    }
    SetDelayTimer(ARG1);
    return(SEQ_BUSY);
}

/*-----------------------------------------------------------------------------
 *
 * Module: state_play_time()
 * Description:
 *
 *----------------------------------------------------------------------------*/
seq_exit_t state_play_time(void)
{
    SeekBusy = FALSE;

    if(svoVar_discKindT.Bit.DiscStd == eDiscCD)
    {
        svoEnTimeRead();   // strange: why not reading address in case of CD?
    }
    else
    {
        CheckHeaderID();   // Update current PSN address for Servo.
    }

    set_SIF(STATE, svoStTracking);  /* set state "svoStTracking" */

//#if DEFECT_LENGTH_MEASURE == 1
 //   libEnableDFCTInt();
//#endif

#if (DeadLoop_Solution == 1)
    svoWblErrCount = 0;
#endif


#if (EN_ADEQ == 1) || (EN_BD_ADEQ == 1)
    if (
    #if (EN_BD_ADEQ == 1)
        (svoVar_discKindT.Bit.DiscStd == eDiscBD)
        #if (EN_ADEQ == 1)
        || ((svoVar_discKindT.Bit.DiscStd == eDiscDVD) && ((svoVar_discKindT.Bit.Media == eMediaR) || (svoVar_discKindT.Bit.Media == eMediaRW)) && (pdbGetDiscStatus() == eFinalizedDisc))
        #endif
    #else
        ((svoVar_discKindT.Bit.DiscStd == eDiscDVD) && ((svoVar_discKindT.Bit.Media == eMediaR) || (svoVar_discKindT.Bit.Media == eMediaRW)) && (pdbGetDiscStatus() == eFinalizedDisc))
    #endif
       )
    {
        WRITE_FIELD(HAL_DFE_ADEQDIS,1);
        if (Disable_ADEQ == 0)
        {
            InitAdaptEq();
            WRITE_FIELD(HAL_DFE_ADEQDIS,0);//turn on ADEQ
        }
    }
#endif


//== start ==Set HF PLL mode to be ForceInLockAid mode when DVD ready retry
#if (HORROR_DISC_IMPROVEMENT == 1)
            if ((read_retry_option&(READ_RETRY_ENABLE|SLOW_PLL_RETRY_FORCE_LOCK_ENABLE))
                ==(READ_RETRY_ENABLE|SLOW_PLL_RETRY_FORCE_LOCK_ENABLE))
            {
                WRITE_FIELD(HAL_DFE_PLLMODE, halDFE_PLLModeForceInLockaid);
            }
#endif
//== end ==Set HF PLL mode to be ForceInLockAid mode when DVD ready retry

//<<SERVO_BODE function>>
//CH1 connects MUX2; CH2 connects MUX1; SOURCE connects GPADC1
#if (SERVO_BODE == FOCUS_BODE)
    // focus Bode
#if (CHIP_REV <= 0xB3)
    AfeMUXOUT(AFE_MUXOUT1, halAFE_MUX1SEL_MUXOUT3);  //MUX1 outputs MUX3(FE1)
    AfeMUXOUT(AFE_MUXOUT2, halAFE_MUX2SEL_FE);  //MUX2 outputs FE
    AfeMUXOUT(AFE_MUXOUT3, halAFE_MUX3SEL_FE1); //MUX3 outputs FE1
#else
    AfeMUXOUT(AFE_MUXOUT1, halAFE_MUX1SEL_MUXOUT3, 0);  // MUX1 outputs MUX3 (FE1)
    AfeMUXOUT(AFE_MUXOUT2, halAFE_MUX2SEL_FE2, 0);      // MUX2 outputs FE2
    AfeMUXOUT(AFE_MUXOUT3, halAFE_MUX3SEL_FE1, 0);      // MUX3 outputs FE1
#endif
    WRITE_FIELD(HAL_AFE_FEGPADCEN,1);           //93C[6]=1:Enable FEGPADC Offset for FE (FE=FE1-GPADC)
    WRITE_FIELD(HAL_AFE_TEGPADCEN,0);           //942[6]=1:Disable TEGPADC Offset for TE (TE=TE1-GPADC)
    WRITE_FIELD(HAL_AFE_FE2BYPASS,0);           //93C[7]=0:Disable bypass FE2 Gain and Offset Adjustment
    AfeMUXOUT(AFE_MUXOUT3, halAFE_MUX3SEL_FE1, 0); //MUX3 outputs FE1
#endif //(SERVO_BODE == FOCUS_BODE)

#if (SERVO_BODE == TRACKING_BODE)
    // tracking Bode
#if (CHIP_REV <= 0xB3)
    AfeMUXOUT(AFE_MUXOUT1, halAFE_MUX1SEL_MUXOUT3);  //MUX1 outputs MUX3(TE1)
    AfeMUXOUT(AFE_MUXOUT2, halAFE_MUX2SEL_TE);  //MUX2 outputs TE
    AfeMUXOUT(AFE_MUXOUT3, halAFE_MUX3SEL_TE1); //MUX3 outputs TE1
#else
    AfeMUXOUT(AFE_MUXOUT1, halAFE_MUX1SEL_MUXOUT3, 0);  // MUX1 outputs MUX3 (TE1)
    AfeMUXOUT(AFE_MUXOUT2, halAFE_MUX2SEL_TE2, 0);      // MUX2 outputs TE2
    AfeMUXOUT(AFE_MUXOUT3, halAFE_MUX3SEL_TE1, 0);      // MUX3 outputs TE1
#endif
    WRITE_FIELD(HAL_AFE_FEGPADCEN,0);           //93C[6]=0:Disable FEGPADC Offset for FE (FE=FE1-GPADC)
    WRITE_FIELD(HAL_AFE_TEGPADCEN,1);           //942[6]=1:Enable TEGPADC Offset for TE (TE=TE1-GPADC)
    WRITE_FIELD(HAL_AFE_TE2BYPASS,0);           //942[7]=0:Disable bypass TE2 Gain and Offset Adjustment
#endif //(SERVO_BODE == TRACKING_BODE)



    return(SEQ_EX0);
}

/*-----------------------------------------------------------------------------
 *
 * Module: state_play_init()
 * Description:
 *
 *----------------------------------------------------------------------------*/
seq_exit_t state_play_init(void)
{
#if (BD_ENABLE == 1)
    BYTE Temp;
#endif

#if (BD_ENABLE == 1)
    if(svo_BDR_RE_PICReading == SET)
        return(SEQ_EX2);
#endif

    switch (svoVar_discKindT.Bit.DiscStd)
    {
        case eDiscCD:
            break;

        case eDiscDVD:
            // reset CIDI: Current ID Ready Interrupt Flag
            CLEAR_INT_FIELD(HAL_DVDD_CIDI, 0);
            break;
#if (BD_ENABLE == 1)
        case eDiscBD:
            //Clear sub data buffer by reading the AUNFLGBT field
            Temp = (BYTE)READ_REG(HAL_BDCREG_MAP->BDAUNS);
            Temp = (BYTE)READ_FIELD(HAL_BDC_AUN);
            Temp = READ_FIELD(HAL_BDC_AUNFLGBT);
            break;
#endif
#if (ENABLE_HDDVD == 1)
        case eDiscHD:
            // reset CIDI: Current ID Ready Interrupt Flag
            CLEAR_INT_FIELD(HAL_DVDD_CIDI, 0);
            break;
#endif
    }
    return(SEQ_EX0);
}

/*-----------------------------------------------------------------------------
 *
 * Module: state_subq_err()
 * Description:
 *
 *----------------------------------------------------------------------------*/
seq_exit_t state_subq_err(void)
{
    seq_exit_t ret;

    ret = SEQ_BUSY;

    svoIramVar.knlTimeupCountFlag = TRUE;
    svoGetIDErrCount++;

    switch((svoIramVar.svoInState & TRN_MASK))
    {
        case SVO_SEEK:
            svoCmdVar.svoCommand = CSEEK;
            break;
        case SVO_PLAY:
            if(svoCmdVar.svoCommand == CNOP)
                break;
        case SVO_PAUSEINI:
        case SVO_PAUSE:
            if(svoCmdVar.svoCommand == CNOP){
                svoCmdVar.svoCommand = CPAUSE;
            }else{
                if(svoCmdVar.svoCommand == CSEEK)
                    fgseektime = 0;
                svoGetIDErrCount = 0;
                svoSeekErrCnt = 0;
#if (DeadLoop_Solution == 1)
                svoWblErrCount = 0;
#endif
            }
            break;
        default:
            break;
    }
    if (FokStatusOK())
    {
        ret = SEQ_EX3;
    }
    else
    {
        send_msg1('W','H','0','7');
        ret = SEQ_EX2;
    }

    return(ret);
}

/*-----------------------------------------------------------------------------
 *
 * Module: state_sectorID_chk2()
 * Description:
 *
 *----------------------------------------------------------------------------*/
seq_exit_t state_sectorID_chk2(void)
{
    seq_exit_t ret_seq=SEQ_BUSY;
    USHORT     current_radius;
    SHORT      modulation,modulation_min,modulation_max;//20101020_02QL == improve RF AGC AOC monitoring and reset to prevent RFAGC/AOC is locked by HF defect and cause read fail.

#if (BD_ENABLE == 1)
    #if (E_DIAGS == 1) && (TUNING_ENVIRONMENT_ON==1)
    if (READ_LDC_BIS == 1)
    {//Do not recover during tuning
        return(SEQ_EX0);
    }
    #endif
#endif

    if (svoIramVar.svoPreInState != svoIramVar.svoInState)
    {
#if (OPEN_SESSION==1)
        if (((svoVar_TrackingMethod != TRK_METHOD_DPP)
    #if (WOBBLE_READ_ON_DVD_OR_CD == 0)
            || (svoVar_discKindT.Bit.DiscStd == eDiscCD) || (svoVar_discKindT.Bit.DiscStd == eDiscDVD)
    #endif
            ) && (RFpp_chk() == FALSE))
        {
            return(SEQ_EX1);
        }
#endif
#if (DVD_RAM_READ == 1)
        if (DVDRAM_rewritable)
        {//Reset TX speed variables for skating detection for DVDRAM discs
            TXSpdMonitorInit();
        }
        //else
#endif
        if (READ_FIELD(HAL_DFE_AGCON))//20101015_02PQ ==start== improve RF AGC AOC monitoring and remove searchNWA workaround
        {
#if (NEW_RFPOS_CALIB == 1)
            modulation = (LONG)(100*DESIRED_RF_AMP)/(DESIRED_RF_TOP - (LONG)((svoRRFVar.step_size[READ_FIELD(HAL_DFE_GDDATGMON)] * (READ_FIELD(HAL_DFE_ODDATGMON) - svoRRFVar.ofs_ldoff_cur))/256) - svoRRFVar.rfagc_ref);
#else
            modulation = (LONG)(100*DESIRED_RF_AMP)/(DESIRED_RF_TOP - (LONG)((svoRRFVar.step_size[READ_FIELD(HAL_DFE_GDDATGMON)] * (READ_FIELD(HAL_DFE_ODDATGMON) - svoRRFVar.ofs_ldoff_cur))/256));
#endif
            //SendMsg5SValue(1,"SetIMod2",4,modulation);
            modulation_max = 0x7FFF;
            modulation_min = svoRRFVar.modulation[Layer_Index];
            if (modulation_min == 0)
            {
                modulation_min = COEF_GetValue(DFE_CALC_M);
            }

            modulation_min = modulation_min * 3 / 5;
            if (!HAL_GLOBAL_GetRfPresent())
            {
                modulation_min = modulation_min * 5 / 4;
                modulation_max = modulation_min * 2;
            }

            if ((modulation <= modulation_min) || (modulation > modulation_max))
            {
                SendMsg5SValue(1,"SidRstAgc",1,READ_FIELD(HAL_DFE_GDDATGMON));
                HAL_DFE_LoadCalRfAgcAoc();
            }
            //20101020_02QL ==end== improve RF AGC AOC monitoring and reset to prevent RFAGC/AOC is locked by HF defect and cause read fail.
        }//201010

#if (DETECT_ECC_DISC == 1)
        if(  (svoVar_discKindT.Bit.DiscStd == eDiscCD)&&(svoVar_eccDiscNum >= eECC_210_um) )
        {
            if((ARG1*3) < MS_200)
                SetTimeout(0,ARG1*3);
            else
                SetTimeout(0,MS_200);
        }
        else
#endif
        {
            if( (svoVar_discKindT.Bit.DiscStd == eDiscCD)&&((Packet_Track == WR_METHOD_FP)||(Packet_Track == WR_METHOD_VP)||(Packet_Track == TRKNO_INITVALID)) )    //  modify for read Q time-out time
                SetTimeout(0,MS_200);
            else
                SetTimeout(0,ARG1);
        }
    }

    if (svoVar_discKindT.Bit.DiscStd == eDiscCD)
        ret_seq = state_subq_chk2();
    else
        ret_seq = state_headerID_chk2();

    if ((ret_seq == SEQ_BUSY) || (ret_seq == SEQ_EX4))
    {
        if (FokStatusOK())
        {   /* true if FOK detected */
#if (DVD_RAM_READ == 1)
            if (DVDRAM_rewritable && READ_FIELD(HAL_WBL_PLLLCKS))
            {//Reset TX speed variables to prevent wrong skating detection for DVD-RAM
                TXSpdMonitorInit();
            }

            if ((svoStepVar.SledCounter >= TRACK_SLIP_DETECT_THRESHOLD) ||
                ((DVDRAM_rewritable && (!READ_FIELD(HAL_WBL_PLLLCKS))) ? (TXSpdMonitor() == NG) : 0))
#else
            if (svoStepVar.SledCounter >= TRACK_SLIP_DETECT_THRESHOLD)
#endif
            {
                SendMsgCn(SHOW_DEBUG_MSG,1,0x3F0120,0x82);
                ClearAddressRead();
                track_slip_protection(TRUE);
                ret_seq = SEQ_EX3;
            }
        }
        else
        {
            svoIramVar.svoOrigInState = svoIramVar.svoInState;
            svoSlipSign = ON;
            svoIramVar.knlTimeupCountFlag = TRUE;
            ret_seq = SEQ_EX2;
        }
    }
    else if (ret_seq == SEQ_EX0)
    {
#if (FDP_TRKSLIP_MONITOR == 0)
        svoMiscVar.trackSlipCount = 0;
#endif
#if (DETECT_DVD_CLV_TRACK_PITCH == 1)
        if ((svoStepVar.StepPosCal == FALSE) && (svoVar_discKindT.Bit.DiscStd != eDiscCD)
            &&(!((svoVar_discKindT.Bit.DiscStd == eDiscDVD)&&(svoVar_discKindT.Bit.Media == eMediaROM))))
#else
        if ((svoStepVar.StepPosCal == FALSE) && (svoVar_discKindT.Bit.DiscStd != eDiscCD))
#endif
        {
            current_radius = svoPBA2Radius(svoIramVar.currentID.SectorNum.all32s);
            SendMsgCn(DEBUG_DISK_ID_MSG,8,0x310219,A4B(svoSeekVar.parFine),A4B(svoSeekVar.parRough));
#if (OPU_HomeSW > 0)
            Iram_stp_CurrentPosition = STEP_INWARD_POSITION + current_radius - Iram_stp_CurrentPosition;
            SendMsgCn(DEBUG_DISK_ID_MSG,2,0x31021C,A2B(Iram_stp_CurrentPosition));
#else // OPU_HomeSW
            Iram_stp_CurrentPosition = STEP_OUTWARD_POSITION + current_radius - Iram_stp_CurrentPosition;
            SendMsgCn(DEBUG_DISK_ID_MSG,2,0x31021D,A2B(Iram_stp_CurrentPosition));
#endif // OPU_HomeSW
            svoStp_SetCurrentPosition(current_radius);
            svoStepVar.StepPosCal = TRUE;
            svoCalVar.discChkDoneBits |= velo_chk_end;
        }
#if 0 //print address info during tracking
        send_msg54(SHOW_DEBUG_MSG,svoIramVar.currentID.Layer,
                                  svoIramVar.currentID.SectorNum.bytes.byte2,
                                  svoIramVar.currentID.SectorNum.bytes.byte1,
                                  svoIramVar.currentID.SectorNum.bytes.byte0);
#endif
#if (SRC_RESET_DC_LPF == 1)
    #if (SRC_ENABLE == 1)
        if ((svoSpeedVar.SpdSRCEnable == TRUE) && (SRC_need_ON == 2) && (svoStepVar.SledCounter < 1))
        {

            //send_msg5S(1,"==SRCON==");
            //send_msg5SValue(1,"==SledC==", 4, svoStepVar.SledCounter);
            SRC_need_ON=0xFF;
            HAL_DSP_SRCHold(SRC_LOOP_TRACKING, ON, ON, OFF);
            HAL_DSP_SRCEnable(SRC_LOOP_TRACKING, SRC_STATE_ON);
        }
    #endif // (SRC_ENABLE == 1)
#endif //(SRC_RESET_DC_LPF == 1)

    }
#if (OPEN_SESSION==1)
    else if(ret_seq == SEQ_EX1)
    {
        //if ((svoVar_discKindT.Bit.DiscStd == eDiscCD)&&(pdbGetServoReadyStatus() == FALSE) )
        if (svoVar_discKindT.Bit.DiscStd == eDiscCD)
        {
            return(SEQ_EX0);
        }
    }
#endif

    return(ret_seq);
}

/*-----------------------------------------------------------------------------
 *
 * Module: state_sectorID_chk()
 * Description:
 *
 *----------------------------------------------------------------------------*/
#if (SEEK_ADJ==1)
BYTE seek_adj_flag;
#endif
seq_exit_t state_sectorID_chk(void)
{
    seq_exit_t ret_seq;

    ret_seq = SEQ_BUSY;
    if (svoIramVar.svoPreInState != svoIramVar.svoInState)
    {
        //send_msg80(DEBUG_CALIB_ENTRYS_MSG,0x5A012C); // >>> STATE_SECTORID_CHK <<<
#if (DVD_RAM_READ == 1)
        if (DVDRAM_rewritable)
        {//Reset TX speed variables for skating detection for DVDRAM discs
            TXSpdMonitorInit();
        }
#endif

#if (DETECT_ECC_DISC == 1)
        if((svoVar_discKindT.Bit.DiscStd == eDiscCD) && (svoVar_eccDiscNum >= eECC_210_um))
        {
            if ((ARG1*3) < MS_200)
                SetTimeout(0,ARG1*3);
            else
                SetTimeout(0,MS_200);
        }
        else
#endif
        {
            if ((svoVar_discKindT.Bit.DiscStd == eDiscCD) && ((Packet_Track == WR_METHOD_FP) || (Packet_Track == WR_METHOD_VP) || (Packet_Track == TRKNO_INITVALID)) )    // modify for read Q time-out time
                SetTimeout(0,MS_200);
            else
                SetTimeout(0,ARG1);
        }

#if (OPEN_SESSION==1)
        if (((svoVar_TrackingMethod != TRK_METHOD_DPP)
    #if (WOBBLE_READ_ON_DVD_OR_CD == 0)
            || (svoVar_discKindT.Bit.DiscStd == eDiscCD) || (svoVar_discKindT.Bit.DiscStd == eDiscDVD)
    #endif
            ) && (RFpp_chk() == FALSE))
        {
            ret_seq = SEQ_EX4;
        }
#endif
    }

    if (((svoIramVar.svoInState & TRN_MASK) == SVO_PAUSEINI) ||
        ((svoIramVar.svoInState & TRN_MASK) == SVO_PAUSE))
    {
        if (svoVar_abort_pause_if_applyable == TRUE)
        {
#if (DeadLoop_Solution == 1)
            if (svoWblErrCount > 0)
                svoWblErrCount--;
#endif // (DeadLoop_Solution == 1)
            ret_seq = SEQ_EX0;
        }
    }

    if (ret_seq == SEQ_BUSY)
    {
        if (svoVar_discKindT.Bit.DiscStd == eDiscCD)
            ret_seq = state_subq_chk();
        else
            ret_seq = state_headerID_chk();
    }

    if (ret_seq == SEQ_BUSY)
    {
        if (FokStatusOK())
        {  /* true if FOK detected */
#if (DVD_RAM_READ == 1)
            if (DVDRAM_rewritable && READ_FIELD(HAL_WBL_PLLLCKS))
            {//Reset TX speed variables to prevent wrong skating detection for DVD-RAM
                TXSpdMonitorInit();
            }

            if ((svoStepVar.SledCounter >= TRACK_SLIP_DETECT_THRESHOLD) ||
                ((DVDRAM_rewritable && (!READ_FIELD(HAL_WBL_PLLLCKS))) ? (TXSpdMonitor() == NG) : 0))
#else
            if (svoStepVar.SledCounter >= TRACK_SLIP_DETECT_THRESHOLD)
#endif
            {
                SendMsgCn(SHOW_DEBUG_MSG,1,0x3F0120,0x81);
                ClearAddressRead();
                track_slip_protection(TRUE);
                ret_seq = SEQ_EX3;
            }
        }
        else
        {
            svoIramVar.svoOrigInState = svoIramVar.svoInState;
            svoSlipSign = ON;
            svoIramVar.knlTimeupCountFlag = TRUE;
            ret_seq = SEQ_EX2;
        }
    }
    else if (ret_seq != SEQ_EX0)
    {
        if ((svoIramVar.svoInState & TRN_MASK) == CSEEK)
        {
            if ((svoSeekVar.roughCount == 0) && (svoSeekVar.fineCount == 0) && (svoGetIDErrCount == 0) && ((svoStepVar.StepPosCal == TRUE) || DVDRAM || (svoVar_discKindT.Bit.DiscStd == eDiscDVD)))
            {
                svoIramVar.currentID.Layer = Layer_Index;
                svoIramVar.currentID.SectorNum.all32s =
                svoRadius2PBA(Iram_stp_CurrentPosition,Layer_Index);
                SendMsg80(SHOW_DEBUG_MSG,0x33400F); // WANRING: Blind Jump
                return (SEQ_EX0);
            }

            if ((svoSeekVar.jumpType & L_J_MASK) == L_J_MASK)
            {
                svoSeekVar.jumpType &= (~L_J_MASK);
                calUpdateLayer(!Layer_Index);
                SetTimeout(0,ARG1);
                SendMsg80(SHOW_DEBUG_MSG,0x33400D); //***try other layer***
                return(SEQ_BUSY);
            }
        }
    }
    else
    {
#if (FDP_TRKSLIP_MONITOR == 0)
        svoMiscVar.trackSlipCount = 0;
#endif
#if (SEEK_ADJ==1)
        if (seek_adj_flag)
        {
            seek_adj_flag = 0;
            SendMsgCnB(SHOW_DEBUG_MSG,10,0x33FF40,B2B(spdGetCurrentRPM()),B2B(Iram_stp_CurrentPosition),B2B(svoSeekVar.parFine),B2B(svoSeekVar.jumpDistance),B2B(TimerMeasureStop()));
        }
#endif
    }

    return(ret_seq);
}

/*-----------------------------------------------------------------------------
 *
 * Module: state_subq_chk()
 * Description:
 *
 *----------------------------------------------------------------------------*/
BYTE wblBPFRecover_ct;
BYTE ADEQ_Recover_ct;
seq_exit_t state_subq_chk(void)
{
    if (timeout_chk(0) == eTimerTimeout)
    {   /* time out check */
        //send_msg54(SHOW_DEBUG_MSG,'R','e','a','l');
        track_slip_protection(TRUE);

        HAL_DFE_EnDisHfDefect(halDFE_HfDefectEnable);               /* Enable HF defect */
        /* Enable black and white defect */
        Defect_Enable(DEFECT_STATE_ALL_ON|DEFECT_SOURCE_DFE_RF);
        SendMsg80(SHOW_DEBUG_MSG,0x334001);   // WARNING: SubQ/PSN Check Timeout
        if(Packet_Track == TRKNO_INITVALID)    //  modify for read Q time-out time//Packet Track's SubQ address may different
            return(SEQ_EX0);

        return(SEQ_EX1);
    }
    if(checkTimeData() == TRUE)
    {

        svoPrvtVar_atipErrorCount = 0;
#if WIDER_TOK_LVL_FOR_PLAY_MODE == 1
        Set_TOK_Mode(TOK_PLAY);
#endif
        if ((svoSeekVar.roughCount + svoSeekVar.fineCount) > 0)
        {
            Defect_Enable(DEFECT_STATE_ALL_ON|DEFECT_SOURCE_DFE_RF);
        }
        HAL_DFE_EnDisHfDefect(halDFE_HfDefectEnable);               /* Enable HF defect */

#if (DeadLoop_Solution == 1)
        if (svoWblErrCount > 0)
            svoWblErrCount--;
#endif

        return(SEQ_EX0);
    }

    return(SEQ_BUSY);
}

/*-----------------------------------------------------------------------------
 *
 * Module: state_subq_chk2()
 * Description:
 *
 *----------------------------------------------------------------------------*/
seq_exit_t state_subq_chk2(void)
{
    if(checkTimeData() == TRUE)
    {
#if WIDER_TOK_LVL_FOR_PLAY_MODE == 1
        Set_TOK_Mode(TOK_PLAY);
#endif
        svoEnTimeRead();
    }
    else
    {
        if (timeout_chk(0) == eTimerTimeout)
        {   /* time out check */
            if(Packet_Track == TRKNO_INITVALID)    //  modify for read Q time-out time//Packet Track's SubQ address may different
                return(SEQ_EX0);

            return(SEQ_EX1);
        }
        return(SEQ_BUSY);
    }

//    if(svoMiscVar.leadInOutFlag == LEAD_OUT)
//    {       /* lead out area */
//        if(svoCmdVar.svoCommand == CNOP)
//        {
//            svoSeekVar.svoTargetID.Layer = 0;
//            svoSeekVar.svoTargetID.SectorNum.all32s = svoIramVar.currentID.SectorNum.all32s - 10;
//            svoSeekVar.advance = 0;
//            svoSeekVar.targetRadius = svoPBA2Radius(svoSeekVar.svoTargetID.SectorNum.all32s);
//            return(SEQ_EX4);
//        }
//    }

#if (DeadLoop_Solution == 1)
    if (svoWblErrCount > 0)
        svoWblErrCount--;
#endif

    return(SEQ_EX0);
}

/*-----------------------------------------------------------------------------
 *
 * Module: state_pll_chk()
 * Description: This function will check the PLL locks and the lock status and
 *              the recoveries
 *              EX0:pll ok
 *              EX1:pll fail
 *              EX2:special
 *              EX3:track slip
 *              EX4:no RF for DPD or no wobble media
 *
 *----------------------------------------------------------------------------*/
extern vBYTE        Iram_Rw_read_mode;
BYTE                PLL_InLockaid;
seq_exit_t state_pll_chk(void)
{
    seq_exit_t ret_seq;
#if (DISC_MIS_JUDGE_RECOVERY)
    SHORT orig_pdic_gn, orig_laser_gn;
#endif

#if (Customer_A1 ==1)//[W.0J]Walker
		if((CompleteStrokeCount<2)&&((svoIramVar.svoInState & TRN_MASK) == TRN_038))//[J50Z]Walker
		{


				if((svoVar_discKindT.Bit.DiscStd == eDiscBD)&&(svoVar_discKindT.Bit.Layers == eLayersDouble)&&((PositionHomeErrorCount > 20)||(CompleteStroke)))
				{
						PositionHomeErrorCount = 0;			
		        return(SEQ_EX4);
				}
			else if((svoVar_discKindT.Bit.DiscStd == eDiscBD)&&(svoVar_discKindT.Bit.Layers == eLayersSingle)&&((PositionHomeErrorCount > 9)||(CompleteStroke)))
				{
				if((svoVar_discKindT.Bit.Media   == eMediaROM)&&(svoIramVar.LTHDisc == TRUE))
				{
					MediaRecovery = TRUE;
				}

					PositionHomeErrorCount = 0; 		
								return(SEQ_EX4);
				}
				else if (((svoVar_discKindT.fDiscKind == eDVDROMDisc)||(svoVar_discKindT.fDiscKind == eDVDROMDLDisc))&&((PositionHomeErrorCount > 9)||(CompleteStroke)))
				{
					PositionHomeErrorCount = 0; 		
								return(SEQ_EX4);							
				}			
				else if ((svoVar_discKindT.fDiscKind == eDVDMinusRWDisc)&&((PositionHomeErrorCount > 20)||(CompleteStroke))) //[S51A]daniel for A1 disc (DVD-RW) retry too much times
				{
						PositionHomeErrorCount = 0;			
		        return(SEQ_EX4);
				}						
				else if (((svoVar_discKindT.fDiscKind == eDVDPlusRDisc)||(svoVar_discKindT.fDiscKind == eDVDPlusRWDisc)||(svoVar_discKindT.fDiscKind == eDVDMinusRDisc))&&((PositionHomeErrorCount > 9)||(CompleteStroke)))
				{
					PositionHomeErrorCount = 0; 		
								return(SEQ_EX4);							
				}
				else if (((svoVar_discKindT.fDiscKind == eDVDPlusRDLDisc)||(svoVar_discKindT.fDiscKind == eDVDPlusRWDLDisc)||(svoVar_discKindT.fDiscKind == eDVDMinusRDLDisc)||(svoVar_discKindT.fDiscKind == eDVDMinusRWDLDisc))&&((PositionHomeErrorCount > 20)||(CompleteStroke))) //[W0L]daniel for A1 disc (DVD+R DL) retry too much times
				{
						PositionHomeErrorCount = 0;			
		        return(SEQ_EX4);
				}						
				else if ((svoVar_discKindT.Bit.DiscStd == eDiscCD)&&((PositionHomeErrorCount > 16)||(CompleteStroke)))
				{
						PositionHomeErrorCount = 0;			
		        return(SEQ_EX4);
				}						
						
		}
#endif		

    if (svoIramVar.svoPreInState != svoIramVar.svoInState)
    {
        //send_msg80(SHOW_DEBUG_MSG,0x5A0120); //>>> STATE_PLL_CHK <<<
#if (DVD_RAM_READ == 1)
        if (DVDRAM_rewritable)
        {//Reset TX speed variables for skating detection for DVDRAM discs
            TXSpdMonitorInit();
        }
#endif
        SetTimeout(0,ARG1);
    }

    // check whether we are dealing with a ROM disk (no wobble present)
#if (EN_WOBBLE == 1)
    #if (WOBBLE_READ_ON_DVD_OR_CD == 1)
    if ((svoVar_discKindT.Bit.Media == eMediaROM)
        #if (DVD_RAM_READ == 1)
        || (DVDRAM_embossed)
        #endif
    #else
    if ((((svoVar_discKindT.Bit.DiscStd != eDiscBD)||(svoVar_discKindT.Bit.Media == eMediaROM))
        #if (DVD_RAM_READ == 1)
        && (!DVDRAM_rewritable)
        #endif
        )
    #endif
    #if (BD_ENABLE == 1)
        ||(svo_BDR_RE_PICReading == SET)
    #endif
       )
#endif // (EN_WOBBLE == 1)
    {
        if (HAL_DFE_GetPllOnHold())
        {
            SendMsg80(SHOW_DEBUG_MSG,0x400000);//RF PLL still on hold !!!
            SetDfeWblFrontOnHoldTracking();
            SetDfeWblFrontSemiSynchrAfterTracking();
            Delay100uS(1);
            SetDfeWblFrontBitSynchr();
        }

        // Check whether the PLL is in lock (phase and frequency lock)
        if (HAL_DFE_GetPllInLock())
        {
            SendMsgCnB(DEBUG_DFE_MSG,4,0x400001,B4B(READ_REG(HAL_DFEREG_MAP->HFPLLS))); // HFPLL in lock (HFPLLS)
            SendMsgCnB(DEBUG_DFE_MSG,4,0x400004,B4B(READ_FIELD(HAL_DFE_INTEGRAT)));     // HFPLLFreq
            //unreset Fast PLL, enable Black and white defect detection and put back the HPF in the processing path
            if (PLL_InLockaid > 0)
            {
                PLL_InLockaid--;
                if (PLL_InLockaid == 0)
                {
                    SetDfeWblFrontOnHoldTracking();
                    SetDfeWblFrontSemiSynchrAfterTracking();
                    Delay100uS(1);
                }
            }
            SetDfeWblFrontBitSynchr();

            // we can exit the state
            ret_seq = SEQ_EX0;
        }
        else
        {
            if (timeout_chk(0) == eTimerTimeout)
            {
#if (EN_ADEQ == 1)
                InitAdaptEq();//turn off ADEQ
#endif

#if (OPEN_SESSION==1)//For Discs with too less recorded Data which causes track pitch measurement fail
                        //CD's first Leadin should not bypass this PLL check
                if (((svoVar_TrackingMethod != TRK_METHOD_DPP)
    #if (WOBBLE_READ_ON_DVD_OR_CD == 0)
                     || ((svoVar_discKindT.Bit.DiscStd == eDiscCD) && (svoSeekVar.svoTargetID.SectorNum.all32s >= 0)) || (svoVar_discKindT.Bit.DiscStd == eDiscDVD)
    #endif
                     ) && (RFpp_chk() == FALSE))

                {
                    ret_seq = SEQ_EX4;
                }
                else
#endif // (OPEN_SESSION==1)
                if (NrTryOutCheckRfPll < MAX_NR_RF_LOCK_TRY_OUT)
                {
                    SendMsgCn(DEBUG_DFE_MSG,1,0x400002,A1B(mEqSettings)); // RF PLL New equalizer settings
                    SendMsgCnB(DEBUG_DFE_MSG,4,0x400004,B4B(READ_FIELD(HAL_DFE_INTEGRAT)));     // HFPLLFreq

#if (BD_ENABLE == 1)
                    if(svoVar_discKindT.Bit.DiscStd == eDiscBD)
                    {
                        if (svo_BDR_RE_PICReading == CLEAR)
                        {
                            SetDfeWblFrontOnHoldTracking();
                            //New equaliser settings applied
                            LAL_DFE_SetEqualizer(mEqSettings);
                            LAL_DFE_SetNLE(mEqSettings);
                            LAL_DFE_SetClipNLE(mEqSettings);

                            //evaluate the next settings for the PLL Equalizer (sequence is Normal->Low->High)
                            mEqSettings = ((mEqSettings == lalDFE_EqualizerStrengthNormal) ? lalDFE_EqualizerStrengthLow  :
                                          ((mEqSettings == lalDFE_EqualizerStrengthLow   ) ? lalDFE_EqualizerStrengthHigh : lalDFE_EqualizerStrengthNormal));
                            SetTimeout(0,ARG1);
                            SetDfeWblFrontSemiSynchrAfterTracking();
                        }
                        else
                        {
//                            if(Iram_stp_CurrentPosition > UM_2_SLED_STEP(PZONE2_RADIUS_MIDDLE))
//                            {
//                                svo_BDR_RE_PICReading = CLEAR;
//                                LAL_PLL_SetPll();
//                                LAL_SetResetPIC(eRESET_PIC, svo_bReadZoneSpeedX10);
//                            }
//                            else
//                            {
//                                LAL_PLL_SetPll();
//                                LAL_SetResetPIC(eSET_PIC, svo_bReadZoneSpeedX10);
//                            }
                            SetDfeWblFrontOnHoldTracking();
                            SetDfeWblFrontSemiSynchrAfterTracking();
                            // larger timeout for PIC
                            SetTimeout(0,ARG1*3);
                        }
                    }
                    else
#endif //(BD_ENABLE == 1)
                    {
#if DEFECT_LENGTH_MEASURE == 1
                        if ( (svoVar_discKindT.Bit.DiscStd == eDiscCD)&&(CD_Scratch_Disc == 0)&&((St_cdrom_audio == TRUE)||(Iram_Rw_read_mode == eCDDAMode)) )//For reading on scratch
#else
                        if ( (svoVar_discKindT.Bit.DiscStd == eDiscCD)&&((St_cdrom_audio == TRUE)||(Iram_Rw_read_mode == eCDDAMode)) )
#endif
                        {
                            if (PLL_InLockaid < 0xff)
                                PLL_InLockaid++;
                        }

                        SetDfeWblFrontOnHoldTracking();

//    #if (DVD_RAM_READ == 1)
//                        if (!(DVDRAM_embossed))
//    #endif
//                        {//New equaliser settings applied
//                            LAL_DFE_SetEqualizer(mEqSettings);
//                            LAL_DFE_SetNLE(mEqSettings);
//                            LAL_DFE_SetClipNLE(mEqSettings);
//                            //evaluate the next settings for the PLL Equalizer (sequence is Normal->Low->High)
//                            mEqSettings = ((mEqSettings == lalDFE_EqualizerStrengthNormal) ? lalDFE_EqualizerStrengthLow  :
//                                          ((mEqSettings == lalDFE_EqualizerStrengthLow   ) ? lalDFE_EqualizerStrengthHigh : lalDFE_EqualizerStrengthNormal));
//                        }
                        SetDfeWblFrontSemiSynchrAfterTracking();

                        //reset the time out
                        //SetTimeout(0,ARG1);
                        SetTimeout(0,ARG1*3);//Enlarge Timeout setting as BD used for a specific short DVDR on a specific Drive
                    }
                    NrTryOutCheckRfPll++;
                    ret_seq = SEQ_BUSY;
                }
                else
                {
                    SendMsg80(SHOW_DEBUG_MSG,0x000002);
								#if (Customer_A1 ==1)
										PositionHomeErrorCount++;	
								#endif
								
                    PLL_InLockaid = 0;

#if (BD_ENABLE == 1)
                    if (svo_BDR_RE_PICReading == SET && (Iram_stp_CurrentPosition > UM_2_SLED_STEP(PZONE2_RADIUS_MIDDLE)))
                    {
                        svo_BDR_RE_PICReading = CLEAR;
                        LAL_PLL_SetPll();
                        LAL_SetResetPIC(eRESET_PIC, svo_bReadZoneSpeedX10);
                    }
#endif // (BD_ENABLE==1)
                    // all possible settings have been tried unsucessfully : manual lock could also be tried
                    SetDfeWblFrontOnHold();

                    ret_seq = SEQ_EX1;
                }
            }
            else
            {
                SetDelayTimer(MS_1);
                ret_seq = SEQ_BUSY;
            }
        }
    }
#if (EN_WOBBLE == 1)
    else
    // we are dealing with a NON-ROM disk (wobble present) and no PIC read
    {
        // Check if the AGC & PLL is on hold?
        if (!READ_FIELD(HAL_WBL_WBLAGCON))
        {
            SendMsg80(SHOW_DEBUG_MSG,0x410213); // WOBBLE AGC still on hold!!!
            HAL_WOBBLE_SetAGCOnOff(halWBL_WobbleAGC_Enable);
            WaitTimer(MS_5);
        }
        // check the PLL is on hold
        if (HAL_WOBBLE_GetPllOnHold())
        {
            SendMsg80(SHOW_DEBUG_MSG,0x410210); // WOBBLE PLL still on hold!!!
            HAL_WOBBLE_SetPllOnHold(halWBL_WblPllNotInHold);
        }

        // First we check wobble PLL in lock
        if (StateCheckWobblePll)
        {
            // Check whether the WOBBLE PLL is in lock (check wobble PLL lock)
            if (HAL_WOBBLE_GetPllInLock(HAL_PLL_GetWblClkKHz()))
            {
                SendMsg80(DEBUG_WBL_MSG,0x410211);   //WOBBLE PLL in lock
                //unreset Fast PLL, enable Black and white defect detection and put back the HPF in the processing path
                SetDfeWblFrontBitSynchr();

                //reset the time out
                SetTimeout(0,ARG1);

                // next time we get to this state it will be to check the bit sync flywheel
                StateCheckWobblePll = FALSE;
                ret_seq = SEQ_BUSY;
            }
            else
            {
                if (timeout_chk(0) == eTimerTimeout)
                {
                    if (NrTryOutCheckWobblePll < MAX_NR_WBL_LOCK_TRY_OUT)
                    {
                        SendMsgCn(SHOW_DEBUG_MSG,1,0x410012,A1B(NrTryOutCheckWobblePll)); // WOBBLE PLL Check STATE: New try out, number:
                        HAL_WOBBLE_SetPllOnHold(halWBL_WblPllInHold);
                        //Other Wobble PLL settings could be applied here
//                        if (svoVar_discKindT.Bit.DiscStd == eDiscBD)
//                        {
//                            if(NrTryOutCheckWobblePll&0x01)
//                                WRITE_FIELD(HAL_WBL_LCKAIDPH, (SBYTE)COEF_GetValue(WBL_CALC_coef_LOCK_AID_PH) );
//                            else
//                                WRITE_FIELD(HAL_WBL_LCKAIDPH, (SBYTE)-COEF_GetValue(WBL_CALC_coef_LOCK_AID_PH) );
//                        }
    #if (DVD_RAM_READ == 1)
                        if(DVDRAM && !zone_change)
                        {
                            if(NrTryOutCheckWobblePll<2 && HAL_WOBBLE_GetSyncFlyWheelInLock() == 1)
                            {
                                WRITE_FIELD(HAL_WBL_WBLFREQ, WblPllFreqPreset);
                            }
                            else
                            {
                                SetDfeWblFrontSemiSynchrAfterTracking();
                            }
                        }
                        else
    #endif
                        {
                            SetDfeWblFrontSemiSynchrAfterTracking();
                        }

                        // Increase the number of try out counter
                        NrTryOutCheckWobblePll++;

                        //reset the time out
                        SetTimeout(0,ARG1);

                        ret_seq = SEQ_BUSY;
                    }
                    else
                    {
                        SendMsg80(SHOW_DEBUG_MSG,0x400013); // WOBBLE PLL Check STATE: ERROR after retrial !!!
    #if (BD_ENABLE == 1)
                        if ((svo_BDR_RE_PICReading == SET) && (Iram_stp_CurrentPosition > UM_2_SLED_STEP(PZONE2_RADIUS_MIDDLE)))
                        {
                            svo_BDR_RE_PICReading = CLEAR;
                            LAL_SetResetPIC(eRESET_PIC, svo_bReadZoneSpeedX10);
                        }
    #endif
                        // all possible settings have been tried unsucessfully : manual lock could also be tried
                        SetDfeWblFrontOnHold();
                        ret_seq = SEQ_EX1;
                    }
                }
                else
                {
                    SetDelayTimer(MS_1);
                    ret_seq = SEQ_BUSY;
                }
            }
        }
        // then we check the bit sync flywheel
        else
        {
            //send_msg80(DEBUG_WBL_MSG,0x600020);  // WOBBLE PLL Sync Flywheel State

            // check the bit sync flywheel
            if (HAL_WOBBLE_GetSyncFlyWheelInLock())
            {
                SendMsg80(DEBUG_WBL_MSG,0x400021);  //WOBBLE PLL Sync Flywheel in lock

                // enable the interrupts
    #if (DVD_RAM_READ == 1)
                if (DVDRAM)
                {
                    LAL_WOBBLE_SetHeaderDetectMode(MLH);
                    HAL_WOBBLE_SetAGCOnOff(halWBL_WobbleAGC_Enable);       /* Enable Wobble AGC */
                    HAL_WOBBLE_SetEnableHeaderOn();
                }
                else
    #endif
                {
                    EnableWobbleAdressReading(TRUE);
                    HAL_WOBBLE_SetValidFlyWheelAddress(TRUE);
                }
                // we can exit the state
                ret_seq = SEQ_EX0;
            }
            else
            {
                //send_msg80(DEBUG_WBL_MSG,0x610222);  // WOBBLE bit Sync and word Sync Flywheel out of lock
    #if (DVD_RAM_READ == 1)
                if (DVDRAM)
                {
                    BYTE WblFreq;

                    WblFreq = READ_FIELD(HAL_WBL_WBLFREQ) >> 8;
                    if (!READ_FIELD(HAL_WBL_PLLLCKS) || (WblFreq<READ_FIELD(HAL_WBL_PLLINTLOT)) || (WblFreq>READ_FIELD(HAL_WBL_PLLINTHIT)))
                    {
                        // disable flywheel
                        HAL_WOBBLE_EnableFlywheels(FALSE);

                        // return back to previous state to check the wobble PLL
                        StateCheckWobblePll = TRUE;

                        HAL_WOBBLE_SetPllOnHold(halWBL_WblPllNotInHold);
                        SetDfeWblFrontSemiSynchrAfterTracking();

                        //reset the time out to 0: we don't want to wait
                        SetTimeout(0,0);

                        ret_seq = SEQ_BUSY;
                    }
                }
    #endif

                if (timeout_chk(0) == eTimerTimeout)
                {
                    if (NrTryOutSyncFlyWheel < MAX_NR_SYNC_FLYWHEEL_TRY_OUT)
                    {
    #if (ENABLE_BDRLTH == 1)
                        if ((svoIramVar.LTHDisclikely == 1) && (NrTryOutSyncFlyWheel > MAX_NR_SYNC_FLYWHEEL_TRY_OUT / 2))
                        {
                            SendMsg80(SHOW_DEBUG_MSG,0xEE4400);
                            ret_seq = SEQ_EX0;
                        }
                        else
    #endif
                        if (!HAL_WOBBLE_GetPllInLock(HAL_PLL_GetWblClkKHz()))
                        {
                            //send_msg80(SHOW_DEBUG_MSG,0x610223);  // WOBBLE PLL not in lock
    #if (DVD_RAM_READ == 1)
                            if (DVDRAM)
                            {
                                RWDisc.dvRAM.WobbleOutLock = TRUE;
                            }
    #endif
                            // disable flywheel
                            HAL_WOBBLE_EnableFlywheels(FALSE);

                            // return back to previous state to check the wobble PLL
                            StateCheckWobblePll = TRUE;

                            //reset the time out to 0: we don't want to wait
                            SetTimeout(0,0);

                            ret_seq = SEQ_BUSY;
                        }
                        else
                        {
                            SendMsgCn(SHOW_DEBUG_MSG,1,0x400024,A1B(NrTryOutSyncFlyWheel)); // WOBBLE PLL Sync Flywheel STATE: New try out, number:
                            // disable flywheel
                            HAL_WOBBLE_EnableFlywheels(FALSE);

                            Delay100uS(2);

                            // enable flywheel
                            HAL_WOBBLE_EnableFlywheels(TRUE);

                            //reset the time out
                            SetTimeout(0,ARG1);

                            ret_seq = SEQ_BUSY;
                        }
                        // Increase the number of try out counter
                        NrTryOutSyncFlyWheel++;
                    }
                    else
                    {
                        SendMsg80(SHOW_DEBUG_MSG,0x400025); // WOBBLE PLL Sync Flywheel STATE: ERROR after retrial !!!

                        // all possible settings have been tried unsucessfully
                        SetDfeWblFrontOnHold();

                        HAL_WOBBLE_SetValidFlyWheelAddress(FALSE);

                        // next time we get to this state it will be to check wobble PLL in lock
                        // ion a new pll check sequence
                        StateCheckWobblePll = TRUE;

                        SetTimeout(0,0);
                        ret_seq = SEQ_BUSY;
                    }
                }
                else
                {
                    SetDelayTimer(MS_1);
                    ret_seq = SEQ_BUSY;
                }
            }
        }
        SendMsgCnB(DEBUG_WBL_MSG,4,0x410002,B4B(READ_REG(HAL_WBLREG_MAP->WBLPLLFRQ)));//WobbleFreq
    }
#endif // (EN_WOBBLE == 1)

    if (ret_seq != SEQ_BUSY)
    {
        SetTimeout(0, TIMER_STOP);
        NrTryOutCheckRfPll = 0;
#if (EN_WOBBLE == 1)
        StateCheckWobblePll = TRUE;
        NrTryOutCheckWobblePll = 0;
        NrTryOutSyncFlyWheel = 0;
#endif // (EN_WOBBLE == 1)
        if ((!(svoCalVar.discChkDoneBits & start_end)) && ((svoIramVar.svoInState & TRN_MASK) == SVO_TRON))
        {
            if ((ret_seq == SEQ_EX1) && (++cal_retry <= 3))
            {
                SendMsg80(SHOW_DEBUG_MSG,0x33FF2B); //TRON again
		#if(Customer_A1==1)
			if(cal_retry>1)
			{

				CompleteStroke = TRUE;
				PositionHomeErrorCount = 0;			
			        return(SEQ_EX4);
			}

		#endif				
                svoCalVar.calGainDoneBits &= ~(dif_RF_end|dif_WOB_end);
                ClearAddressRead();
                track_slip_protection(TRUE);
                ret_seq = SEQ_EX3;
            }
            else
            {
                cal_retry = 0;
#if (DISC_MIS_JUDGE_RECOVERY)
                if (ret_seq == SEQ_EX1)
                {
                    if ((svoCalVar.discChkDoneBits & disc_chk_end) == 0)
                    {
                        orig_pdic_gn = svoCalMisc2Var.pdic_gn;
                        orig_laser_gn = svoCalMisc2Var.laser_gn;
                        if (svoReStartUp(eMediaUnknown) == TRUE)
                        {
                            if ((orig_pdic_gn != svoCalMisc2Var.pdic_gn) || (orig_laser_gn != svoCalMisc2Var.laser_gn))
                            {
                                svoCalVar.calOfsDoneBits = vref_end;
                                svoCalVar.calGainDoneBits = 0;
                                svoCalVar.calOfsDoneBits = 0;
                            }
                            ret_seq = SEQ_EX2;
                        }
                    }
                }
#endif
            }
        }
        if (ret_seq == SEQ_EX1)
        {
            if (svoVar_discKindT.Bit.DiscStd == eDiscCD)
            {
#if (OPEN_SESSION==1)//For jump to first Leadin which has no data in it
                if ((svoSeekVar.svoTargetID.SectorNum.all32s < 0) && (RFpp_chk() == FALSE))
                {
                    NrTryOutCheckRfPll = MAX_NR_RF_LOCK_TRY_OUT;
                }
#endif // (OPEN_SESSION==1)
            }
        }
    }
    else
    {
#if (DVD_RAM_READ == 1)
        if (DVDRAM_rewritable && READ_FIELD(HAL_WBL_PLLLCKS))
        {//Reset TX speed variables to prevent wrong skating detection for DVD-RAM
            TXSpdMonitorInit();
        }

        if ((svoStepVar.SledCounter >= TRACK_SLIP_DETECT_THRESHOLD) ||
            ((DVDRAM_rewritable && (!READ_FIELD(HAL_WBL_PLLLCKS))) ? (TXSpdMonitor() == NG) : 0))
#else
        if (svoStepVar.SledCounter >= TRACK_SLIP_DETECT_THRESHOLD)
#endif
        {
            SendMsgCn(SHOW_DEBUG_MSG,1,0x3F0120,0x83);
            ClearAddressRead();
            track_slip_protection(TRUE);
            ret_seq = SEQ_EX3;
        }
    }
    return(ret_seq);
}

/*-----------------------------------------------------------------------------
 *
 * Module: state_headerID_chk()
 * Description: This function will check and read DVD/BD/HD header ID
 *
 *----------------------------------------------------------------------------*/
extern  ULONG   svoPrvtVar_timeoutTimers[TIMEOUT_TIMER_MAX];    // time_out_timer[2];
extern volatile eENCPLLMode  wblENCPLLMode;

seq_exit_t state_headerID_chk(void)
{
    BYTE       temp;
    SHORT      modulation,modulation_min,modulation_max;//20101020_02QL == improve RF AGC AOC monitoring and reset to prevent RFAGC/AOC is locked by HF defect and cause read fail.
//20101014_02PK ==start== Show ADEQ learing values if subQ/PSN read timeout, frank modify, frank confirm
    #if (EN_ADEQ == 1)||(EN_BD_ADEQ == 1)
    BYTE tap_temp[5];
    #endif
//20101014_02PK ==end== Show ADEQ learing values if subQ/PSN read timeout, frank modify, frank confirm

    if (CheckHeaderID() == TRUE)
    {
        if (svoVar_discKindT.Bit.Layers == eLayersDouble)
        {
            if (Layer_Index != svoIramVar.currentID.Layer)
            {
                SendMsg80(SHOW_DEBUG_MSG,0x33400C);
                calUpdateLayer(!Layer_Index);
            }
        }
#if WIDER_TOK_LVL_FOR_PLAY_MODE == 1
        Set_TOK_Mode(TOK_PLAY);
#else
        #if (CHIP_REV <= 0xB3)
        WRITE_FIELD(HAL_AFE_TOKLVL, halAFE_TOKLVL_VREF_1125mV);
        #else
        WRITE_FIELD(HAL_AFE_TOKLVL, halAFE_TECP_TECN_LVL_1100mV);
        #endif
#endif
        HAL_DFE_EnDisHfDefect(halDFE_HfDefectEnable);               /* Enable HF defect */
#if (DeadLoop_Solution == 1)
        if (svoWblErrCount > 0)
            svoWblErrCount--;
#endif

        return(SEQ_EX0);
    }

    if (timeout_chk(0) == eTimerTimeout)
    {
        /* Enable black and white defect */
        Defect_Enable(DEFECT_STATE_ALL_ON|DEFECT_SOURCE_DFE_RF);
        HAL_DFE_EnDisHfDefect(halDFE_HfDefectEnable);               /* Enable HF defect */
        SendMsg80(SHOW_DEBUG_MSG,0x334001);   // WARNING: SubQ/PSN Check Timeout

        //Sometimes reading will become very bad, we need to reset some DFE settings at that time.
        temp = READ_FIELD(HAL_DFE_IIRHPFBW);
        SendMsgCnB(SHOW_DEBUG_MSG,2,0x33FF74,B1B(temp),B1B(READ_FIELD(HAL_DFE_BETAMSEN)));
        if (READ_FIELD(HAL_DFE_BETAMSEN) == 1)
            WRITE_FIELD(HAL_DFE_BETAMSEN, 0);
        else
            WRITE_FIELD(HAL_DFE_BETAMSEN, 1);
        DelaymS(1);//a small delay
        if (READ_FIELD(HAL_DFE_BETAMSEN) == 1)
            WRITE_FIELD(HAL_DFE_BETAMSEN, 0);
        else
            WRITE_FIELD(HAL_DFE_BETAMSEN, 1);
        WRITE_FIELD(HAL_DFE_IIRHPFBW, temp);
        //Sometimes reading will become very bad, we need to reset some DFE settings at that time.
        //20101014_02PK ==start== Show ADEQ learing values if subQ/PSN read timeout, frank modify, frank confirm
        WRITE_FIELD(HAL_DFE_ADQCFBSEL, 0);
        tap_temp[0] = READ_FIELD(HAL_DFE_ADQCFBPRE);
        WRITE_FIELD(HAL_DFE_ADQCFBSEL, 1);
        tap_temp[1] = READ_FIELD(HAL_DFE_ADQCFBPRE);
        WRITE_FIELD(HAL_DFE_ADQCFBSEL, 2);
        tap_temp[2] = READ_FIELD(HAL_DFE_ADQCFBPRE);
        WRITE_FIELD(HAL_DFE_ADQCFBSEL, 3);
        tap_temp[3] = READ_FIELD(HAL_DFE_ADQCFBPRE);
        WRITE_FIELD(HAL_DFE_ADQCFBSEL, 4);
        tap_temp[4] = READ_FIELD(HAL_DFE_ADQCFBPRE);
        //20101022_02R0 ==start== modify message frank modify frank confirm
        SendMsgCnB(SHOW_DEBUG_MSG,11,0x320A24,B1B(tap_temp[0]),B1B(tap_temp[1]),B1B(tap_temp[2]),B1B(tap_temp[3]),B1B(tap_temp[4]),B4B(READ_FIELD(HAL_DFE_INTEGRAT)),B2B(svo_bReadZoneSpeedX10));
        //20101022_02R0 ==end== modify message frank modify frank confirm
        //20101021_02QU ===start=== reset ADEQ value to avoid wrong ADEQ TAP values, frank modify frank confirm
        #if (EN_BD_ADEQ == 1)||(EN_ADEQ == 1)
            InitAdaptEq();
        #endif
        //20101021_02QU ===end=== reset ADEQ value to avoid wrong ADEQ TAP values
        //20101014_02PK ==end== Show ADEQ learing values if subQ/PSN read timeout, frank modify, frank confirm
            if ((!DVDRAM) && READ_FIELD(HAL_DFE_AGCON))//20101015_02PQ ==start== improve RF AGC AOC monitoring and remove searchNWA workaround
            {
#if (NEW_RFPOS_CALIB == 1)
                modulation = (LONG)(100*DESIRED_RF_AMP)/(DESIRED_RF_TOP - (LONG)((svoRRFVar.step_size[READ_FIELD(HAL_DFE_GDDATGMON)] * (READ_FIELD(HAL_DFE_ODDATGMON) - svoRRFVar.ofs_ldoff_cur))/256) - svoRRFVar.rfagc_ref);
#else
                modulation = (LONG)(100*DESIRED_RF_AMP)/(DESIRED_RF_TOP - (LONG)((svoRRFVar.step_size[READ_FIELD(HAL_DFE_GDDATGMON)] * (READ_FIELD(HAL_DFE_ODDATGMON) - svoRRFVar.ofs_ldoff_cur))/256));
#endif
                //SendMsg5SValue(1,"SetIMod3",4,modulation);
                modulation_max = 0x7FFF;
                modulation_min = svoRRFVar.modulation[Layer_Index];
                if (modulation_min == 0)
                {
                    modulation_min = COEF_GetValue(DFE_CALC_M);
                }

                modulation_min = modulation_min * 3 / 5;
                if (!HAL_GLOBAL_GetRfPresent())
                {
                    modulation_min = modulation_min * 5 / 4;
                    modulation_max = modulation_min * 2;
                }

                if ((modulation <= modulation_min) || (modulation > modulation_max))
                {
                    SendMsg5SValue(1,"SidRstAgc2",1,READ_FIELD(HAL_DFE_GDDATGMON));
                    HAL_DFE_LoadCalRfAgcAoc();
                }
                //20101020_02QL ==end== improve RF AGC AOC monitoring and reset to prevent RFAGC/AOC is locked by HF defect and cause read fail.
            }//20101015_02PQ ==start== improve RF AGC AOC monitoring and remove searchNWA workaround

#if (DVD_RAM_READ == 1)
        if (DVDRAM_rewritable)
        {
            //dvdDspSystemInit();
            SetDfeWblFrontOnHoldTracking();
            SetDfeWblFrontSemiSynchrAfterTracking();
            Delay100uS(1);
            SetDfeWblFrontBitSynchr();
        }
#endif
        return(SEQ_EX1);
    }

    return(SEQ_BUSY);
}

/*-----------------------------------------------------------------------------
 *
 * Module: state_headerID_chk2()
 * Description: This function will check and read DVD/BD/HD header ID for Play
 *
 *----------------------------------------------------------------------------*/
seq_exit_t state_headerID_chk2(void)
{
    SHORT      modulation,modulation_min,modulation_max;//20101020_02QL == improve RF AGC AOC monitoring and reset to prevent RFAGC/AOC is locked by HF defect and cause read fail.
    BYTE       temp;
//20101014_02PK ==start== Show ADEQ learing values if subQ/PSN read timeout, frank modify, frank confirm
    #if (EN_ADEQ == 1)||(EN_BD_ADEQ == 1)
    BYTE tap_temp[5];
    #endif
//20101014_02PK ==end== Show ADEQ learing values if subQ/PSN read timeout, frank modify, frank confirm


    if (CheckHeaderID() == TRUE)
    {
        if (svoVar_discKindT.Bit.Layers == eLayersDouble)
        {
            if (Layer_Index != svoIramVar.currentID.Layer)
            {
                SendMsg80(SHOW_DEBUG_MSG,0x33400C);
                calUpdateLayer(!Layer_Index);
            }
        }
#if WIDER_TOK_LVL_FOR_PLAY_MODE == 1
        Set_TOK_Mode(TOK_PLAY);
#else
        #if (CHIP_REV <= 0xB3)
        WRITE_FIELD(HAL_AFE_TOKLVL, halAFE_TOKLVL_VREF_600mV);
        #else
        WRITE_FIELD(HAL_AFE_TOKLVL, halAFE_TECP_TECN_LVL_600mV);
        #endif
#endif
#if (DeadLoop_Solution == 1)
        if(svoWblErrCount > 0)
            svoWblErrCount--;
#endif

        return(SEQ_EX0);
    }

    if(timeout_chk(0) == eTimerTimeout)
    {
#if (OPEN_SESSION==1)
        if ((St_drv_ready == TRUE) || (svoVar_TrackingMethod == TRK_METHOD_DPP))
#endif
        {
#if (EN_WOBBLE == 1)
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
#endif
            {
                // Check whether the PLL is in lock (phase and frequency lock)
                if (!HAL_DFE_GetPllInLock())
                {
                    SendMsgCnB(DEBUG_DFE_MSG,4,0x400101,B4B(READ_REG(HAL_DFEREG_MAP->HFPLLS))); // HFPLL not in LOCK (HFPLLS)
                    return(SEQ_EX4);
                }
            }
        }

        temp = READ_FIELD(HAL_DFE_IIRHPFBW);
        SendMsgCnB(SHOW_DEBUG_MSG,2,0x33FF74,B1B(temp),B1B(READ_FIELD(HAL_DFE_BETAMSEN)));
        //Sometimes reading will become very bad, we need to reset some DFE settings at that time.
        if (READ_FIELD(HAL_DFE_BETAMSEN) == 1)
            WRITE_FIELD(HAL_DFE_BETAMSEN, 0);
        else
            WRITE_FIELD(HAL_DFE_BETAMSEN, 1);
        DelaymS(1);//a small delay
        if (READ_FIELD(HAL_DFE_BETAMSEN) == 1)
            WRITE_FIELD(HAL_DFE_BETAMSEN, 0);
        else
            WRITE_FIELD(HAL_DFE_BETAMSEN, 1);
        WRITE_FIELD(HAL_DFE_IIRHPFBW, temp);
        //Sometimes reading will become very bad, we need to reset some DFE settings at that time.
        //20101014_02PK ==start== Show ADEQ learing values if subQ/PSN read timeout, frank modify, frank confirm
        WRITE_FIELD(HAL_DFE_ADQCFBSEL, 0);
        tap_temp[0] = READ_FIELD(HAL_DFE_ADQCFBPRE);
        WRITE_FIELD(HAL_DFE_ADQCFBSEL, 1);
        tap_temp[1] = READ_FIELD(HAL_DFE_ADQCFBPRE);
        WRITE_FIELD(HAL_DFE_ADQCFBSEL, 2);
        tap_temp[2] = READ_FIELD(HAL_DFE_ADQCFBPRE);
        WRITE_FIELD(HAL_DFE_ADQCFBSEL, 3);
        tap_temp[3] = READ_FIELD(HAL_DFE_ADQCFBPRE);
        WRITE_FIELD(HAL_DFE_ADQCFBSEL, 4);
        tap_temp[4] = READ_FIELD(HAL_DFE_ADQCFBPRE);
        //20101022_02R0 ==start== modify message frank modify frank confirm
        SendMsgCnB(SHOW_DEBUG_MSG,11,0x320A24,B1B(tap_temp[0]),B1B(tap_temp[1]),B1B(tap_temp[2]),B1B(tap_temp[3]),B1B(tap_temp[4]),B4B(READ_FIELD(HAL_DFE_INTEGRAT)),B2B(svo_bReadZoneSpeedX10));
        //20101022_02R0 ==end== modify message frank modify frank confirm
        //20101021_02QU ===start=== reset ADEQ value to avoid wrong ADEQ TAP values, frank modify frank confirm
        #if (EN_BD_ADEQ == 1)||(EN_ADEQ == 1)
            InitAdaptEq();
        #endif
        //20101021_02QU ===end=== reset ADEQ value to avoid wrong ADEQ TAP values
        //20101014_02PK ==end== Show ADEQ learing values if subQ/PSN read timeout, frank modify, frank confirm
            if ((!DVDRAM) && READ_FIELD(HAL_DFE_AGCON))//20101015_02PQ ==start== improve RF AGC AOC monitoring and remove searchNWA workaround
            {
#if (NEW_RFPOS_CALIB == 1)
                modulation = (LONG)(100*DESIRED_RF_AMP)/(DESIRED_RF_TOP - (LONG)((svoRRFVar.step_size[READ_FIELD(HAL_DFE_GDDATGMON)] * (READ_FIELD(HAL_DFE_ODDATGMON) - svoRRFVar.ofs_ldoff_cur))/256) - svoRRFVar.rfagc_ref);
#else
                modulation = (LONG)(100*DESIRED_RF_AMP)/(DESIRED_RF_TOP - (LONG)((svoRRFVar.step_size[READ_FIELD(HAL_DFE_GDDATGMON)] * (READ_FIELD(HAL_DFE_ODDATGMON) - svoRRFVar.ofs_ldoff_cur))/256));
#endif
                //SendMsg5SValue(1,"SetIMod3",4,modulation);
                modulation_max = 0x7FFF;
                modulation_min = svoRRFVar.modulation[Layer_Index];
                if (modulation_min == 0)
                {
                    modulation_min = COEF_GetValue(DFE_CALC_M);
                }

                modulation_min = modulation_min * 3 / 5;
                if (!HAL_GLOBAL_GetRfPresent())
                {
                    modulation_min = modulation_min * 5 / 4;
                    modulation_max = modulation_min * 2;
                }

                if ((modulation <= modulation_min) || (modulation > modulation_max))
                {
                    SendMsg5SValue(1,"SidRstAgc2",1,READ_FIELD(HAL_DFE_GDDATGMON));
                    HAL_DFE_LoadCalRfAgcAoc();
                }
                //20101020_02QL ==end== improve RF AGC AOC monitoring and reset to prevent RFAGC/AOC is locked by HF defect and cause read fail.
            }//20101015_02PQ ==end== improve RF AGC AOC monitoring and remove searchNWA workaround

#if (DVD_RAM_READ == 1)
        if (DVDRAM_rewritable)
        {
            //dvdDspSystemInit();
            SetDfeWblFrontOnHoldTracking();
            SetDfeWblFrontSemiSynchrAfterTracking();
            Delay100uS(1);
            SetDfeWblFrontBitSynchr();
        }
#endif
        return(SEQ_EX1);
    }

    return(SEQ_BUSY);
}

/*-----------------------------------------------------------------------------
 *
 * Module: state_layer_chk()
 * Description: This function will check current layer with target layer for DVDs
 *             Sector Information
 *    : B7    |     B6    |      B5     |    B4
 *    Sector     Tracking  Reflectivity    Reserved
 *    Format      Method
 *
 *      B3    |     B2    |      B1     |    B0
 *         00:Data                         0:Layer0
 *         01:Lead-in                      1:Layer1
 *         10:Lead-out
 *         11:Middle
 *----------------------------------------------------------------------------*/
seq_exit_t state_layer_chk(void)
{
    //send_msg80(DEBUG_CALIB_ENTRYS_MSG,0x5A012D); // >>> STATE_LAYER_CHK <<<
    SendMsgCn(DEBUG_SEEK_MSG,5,0x334207,svoIramVar.currentID.Layer,A4B(svoIramVar.currentID.SectorNum.all32));
    //send_qcode(SHOW_DEBUG_MSG);
#ifdef SLEDGE_JUMP_ADJ
    if ((svoSeekVar.jumpType & SLIP_MASK) != SLIP_MASK)
    {
        if (servo_debug[2] == 3)
        {
            servo_debug[4] = svoIramVar.currentID.SectorNum.all32s;
        }
        if ((servo_debug[2] == 5) && (servo_debug[4] != 0))
        {
            if (servo_debug[1] == 1)
            {
                test_cnt++;
            }
            servo_debug[4] -= svoIramVar.currentID.SectorNum.all32s;
            addr_diff[servo_debug[1] - SLEDGE_JUMP_ADJ_MIN] += servo_debug[4];
            send_msg74(1,0x020300+servo_debug[1],addr_diff[servo_debug[1] - SLEDGE_JUMP_ADJ_MIN] / test_cnt);
            servo_debug[1] ++;
            if (servo_debug[1] > SLEDGE_JUMP_ADJ_MAX)
            {
                send_msgA4(1,2,4,6,test_cnt);
                servo_debug[1] = SLEDGE_JUMP_ADJ_MIN;
            }
            servo_debug[4] = 0;
        }
    }
#endif
    if(svoVar_discKindT.Bit.DiscStd == eDiscCD)
        return(SEQ_EX0);

    if ((svoIramVar.svoInState & TRN_MASK) == SVO_PAUSE)
    {
        if (svoVar_abort_pause_if_applyable == TRUE)
            return(SEQ_EX0);
    }

    svoMiscVar.LayerjumpRetryCount = 0;
    svoMiscVar.LayerJumpErrorFlag = 0;

    if ((svoIramVar.currentID.Layer == svoSeekVar.svoTargetID.Layer)
         || (svoVar_discKindT.Bit.Layers == eLayersSingle))
    {
        return(SEQ_EX0);
    }
    else
    {
        SendMsg80(DEBUG_SEEK_MSG,0x334205+svoSeekVar.svoTargetID.Layer);
        svoSeekVar.jumpType = L_J_MASK;
        return(SEQ_EX1);
    }
}// end of state_layer_chk

/*-----------------------------------------------------------------------------
 *
 * Module: state_layerjump_chk()
 * Description: This function will check current layer with target layer
 *              including a potential layer jump
 *              EX0: servo startup complete
 *              EX1: preparing for L1 startup
 *              EX2: wrong layer detected
 *              EX3: preparing for L1 startup for BD (NEW_BDDL_START_FLOW == 1)
 *
 *----------------------------------------------------------------------------*/
seq_exit_t state_layerjump_chk(void)
{// Function is only called within DSP TE Gain Adjust
    eTimerResult    ftimeout;
#if ((BD_ENABLE == 1) && (NEW_BDDL_START_FLOW == 1))
    SBYTE           gain_diff;
#endif

    SendMsg80(DEBUG_CALIB_ENTRYS_MSG,0x3A0122);// >>> STATE_LAYERJUMP_CHK <<<

    if (svoVar_discKindT.Bit.Layers == eLayersDouble)
    {
        if (svoIramVar.svoPreInState != svoIramVar.svoInState)
        {
            SetTimeout(0,ARG1);
        }

        ftimeout = timeout_chk(0);

        if ((CheckHeaderID() == TRUE) || (ftimeout == eTimerTimeout))
        {
            if ((ftimeout != eTimerTimeout) && (Layer_Index != svoIramVar.currentID.Layer))
            {
                SendMsg80(SHOW_DEBUG_MSG,0x33400C);
                if (svoCalVar.discChkDoneBits & start_end)
                {
                    calUpdateLayer(!Layer_Index);
                }
                else
                {
                    svoStartUpWrongLayer();
                    return (SEQ_EX2);
                }
            }

            if ((svoCalVar.calGainDoneBits & teg_end) != teg_end)
            {
                svoSeekVar.svoTargetID.Layer = 1;
                if (Layer_Index == 0)
                {
                    calPresetLayer1();
                }
#if ((BD_ENABLE == 1) && (NEW_BDDL_START_FLOW == 1))
                if (svoVar_discKindT.Bit.DiscStd == eDiscBD)
                {//remeasure FEpp with colli set to L1.
    #if (ENABLE_BDIH == 1)
        #if (FORCE_READ_BDIH == 1)
                    cal_retry = 0;
                    //while(1);
                    return (SEQ_EX0);
        #endif
    #endif
                    svoCalVar.calGainDoneBits &= (~feg_bsg_end);
                    track_off();
                    svoFocusOff();
                    svoStp_JumpToPosition(STEP_SAFE_SLOW_MODE, svoStepVar.StepHomePos);
    #if (COLLI_SWITCH == 1)
                    svoStp_JumpToPosition(COLLI_NORMAL_NO_REST_MODE, calGetOptColliPos(svoSeekVar.svoTargetID.Layer));
    #endif
    #if (CHIP_REV >= 0xC0)
                    ramp_to(svoCalMisc2Var.fooos - F_CAL_DN_T * (FOCUS_SEARCH_KF18 * FOC_RAMP_FACTOR / 0x20) / 0x20000);
    #else
                    ramp_FOOS(svoCalMisc2Var.fooos - F_CAL_DN_T * (FOCUS_SEARCH_KF18 * FOC_RAMP_FACTOR / 0x20) / 0x20000);
    #endif
                    calUpdateLayer(svoSeekVar.svoTargetID.Layer);

                    //reduce MB_GN/SB_GN if necessary to prevent FEpp saturation
                    gain_diff = COEF_GetValue(AFE_FCTS07_FCSG) - svoCalGnT1Var[1][FE_GN].groove_gain[RECDH];
                    if (gain_diff > -2)
                    {
                        gain_diff += 2;
                        if (gain_diff > svoCalGnT2Var[0][MB_GN].normal_gain) gain_diff = svoCalGnT2Var[0][MB_GN].normal_gain;
                        if (gain_diff > svoCalGnT2Var[0][SB_GN].normal_gain) gain_diff = svoCalGnT2Var[0][SB_GN].normal_gain;
                        WRITE_FIELD(HAL_AFE_MBGUPSEL,svoCalGnT2Var[0][MB_GN].normal_gain - gain_diff);
                        WRITE_FIELD(HAL_AFE_SBGUPSEL,svoCalGnT2Var[0][SB_GN].normal_gain - gain_diff);
                    }
                    return (SEQ_EX3);
                }
#endif
                (void)svo_Layer_Jump(svoSeekVar.svoTargetID.Layer);

                return (SEQ_EX1);
            }
        }
        else
        {
            return (SEQ_BUSY);
        }
    }
    cal_retry = 0;
    svoCalVar.discChkDoneBits |= start_end;
    return (SEQ_EX0);
}// end of state_layer_chk

/*-----------------------------------------------------------------------------
 *
 * Module: state_rough_chk()
 * Description: This function will check if a rough search is needed(SEQ_EX1) or not(Ready)
 *
 *----------------------------------------------------------------------------*/
seq_exit_t state_rough_chk(void)
{
    static BYTE addrErrCnt = 0;
    USHORT current_radius,pll_radius;
    LONG   temp_min_jumpDistance;
    LONG   temp_max_jumpDistance;
    LONG   temp_jumpDistance;
    BYTE   temp_jumpType;
    SHORT  positionError;
    USHORT par_adj;
#if (BD_ENABLE == 1)
    LONG PicZoneEndRadius;
#endif // (BD_ENABLE == 1)
    USHORT ParRoughMax;
    USHORT ParRoughMin;

    //send_msg80(DEBUG_CALIB_ENTRYS_MSG,0x5A012A); // >>> STATE_ROUGH_CHK <<<

    if ((svoIramVar.svoInState & TRN_MASK) == SVO_PAUSE)
    {
        if (svoVar_abort_pause_if_applyable == TRUE)
            return(SEQ_EX0);  // time_chk: make it done anyway
    }

    switch (svoVar_discKindT.Bit.DiscStd)
    {
        case eDiscDVD:
            temp_min_jumpDistance = ROUGH_THRESHOLD_DVD;
            break;

#if (BD_ENABLE == 1)
        case eDiscBD:
            temp_min_jumpDistance = ROUGH_THRESHOLD_BD;
            break;
#endif

#if (ENABLE_HDDVD == 1)
        case eDiscHD:
            temp_min_jumpDistance = ROUGH_THRESHOLD_HD;
            break;
#endif

        case eDiscCD:
        default:
            temp_min_jumpDistance = ROUGH_THRESHOLD_CD;
            break;
    }

    if (svoSeekVar.roughCount >= 2)
    {
        temp_min_jumpDistance = temp_min_jumpDistance * (1 + (svoSeekVar.roughCount - 1) * 3 / (MAX_ROUGH_JUMP - 1));
    }

    current_radius = svoPBA2Radius(svoIramVar.currentID.SectorNum.all32s);

#if (BD_ENABLE == 1)
    if ((svo_BDR_RE_PICReading == SET) && (bdDecFlag.fREADING_PIC == CLEAR))
    {
        PicZoneEndRadius = svoPBA2Radius(PIC_L0_MAX_ADDR);
        temp_jumpDistance = (svoSeekVar.targetRadius - PicZoneEndRadius) + 24*(PicZoneEndRadius - current_radius);
    }
    else if ((svo_BDR_RE_PICReading == CLEAR) && (bdDecFlag.fREADING_PIC == SET) && (svoVar_discKindT.Bit.Media != eMediaROM))
    // jump to fixed radius if going to PIC zone in case of BD-R/RE
    {
        temp_jumpDistance = UM_2_SLED_STEP(PIC_RADIUS_MIDDLE) - current_radius;
    }
    else
#endif // (BD_ENABLE == 1)
    {
        temp_jumpDistance = (svoSeekVar.targetRadius - current_radius);
    }

    if (temp_jumpDistance < 0)
    {
        temp_jumpDistance = -temp_jumpDistance;
        temp_jumpType = BACK;
    }
    else
    {
        temp_jumpType = FORW;
    }

    if (svoCalVar.discChkDoneBits & velo_chk_end)
    {
        if (svoStepVar.StepPosCal == FALSE)
        {
#if (OPU_HomeSW > 0)
            Iram_stp_CurrentPosition = STEP_INWARD_POSITION + current_radius - Iram_stp_CurrentPosition;
            // Measured STEP_INWARD_POSITION = %2d
            SendMsgCn(DEBUG_DISK_ID_MSG,2,0x31021C,A2B(Iram_stp_CurrentPosition));
#else
            Iram_stp_CurrentPosition = STEP_OUTWARD_POSITION + current_radius - Iram_stp_CurrentPosition;
            // Measured STEP_OUTWARD_POSITION = %2d
            SendMsgCn(DEBUG_DISK_ID_MSG,2,0x31021D,A2B(Iram_stp_CurrentPosition));
#endif
            svoStp_SetCurrentPosition(current_radius);
            svoStepVar.StepPosCal = TRUE;
        }

        /* check the error between sledge position and address */
        positionError = current_radius - Iram_stp_CurrentPosition;

        if (svoSeekVar.roughCount > 0)
        {
            if (ABS(svoSeekVar.targetRadius - svoSeekVar.stpErrCount - Iram_stp_CurrentPosition) > UM_2_SLED_STEP(300))
            {
                svoSeekVar.jumpType |= SLIP_MASK;
            }
            else
            {
                svoSeekVar.jumpType &= (~SLIP_MASK);
            }
        }
        svoSeekVar.stpErrCount = positionError;

        positionError = (SHORT)positionError / MICRO_STEP_PER_ELE_REV * MICRO_STEP_PER_ELE_REV;
#ifndef SLEDGE_JUMP_ADJ
        if (ABS(positionError) > (current_radius / 50))//more than 2% error
        {
            pll_radius = svo_get_radius_from_pll();
#if (DETECT_DVD_TRACK_PITCH == 1)
            if ((svoVar_discKindT.Bit.DiscStd == eDiscDVD) && (current_radius > UM_2_SLED_STEP(36875)) && (current_radius > pll_radius))
            {
                par_adj = (pll_radius - UM_2_SLED_STEP(24000)) * 100 / (current_radius - UM_2_SLED_STEP(24000));
                if ((par_adj > 80) && (par_adj < 92))
                {//marginal DVD disc with 6.4um trackpitch detected
                    if (addrErrCnt >= 3)
                    {
                        svoSeekVar.rx_available = FALSE;//RP is really unstable on marginal disc
                        svoSeekVar.parRoughInit = svoSeekVar.parRough = UM_2_SLED_STEP(33104) * UM_2_SLED_STEP(33104) / 16384; //track pitch 0.74um, 3060-->track pitch 0.64um
                        addrErrCnt = 0;
                        svoSeekVar.targetRadius = svoPBA2Radius(svoSeekVar.svoTargetID.SectorNum.all32s);
                        SendMsg80(DEBUG_SEEK_MSG,0x3FAA0C);
                        return(SEQ_BUSY);
                    }
                    else
                    {
                        SendMsgCnB(DEBUG_SEEK_MSG,1,0x3FAA0B,B1B(addrErrCnt));
                        addrErrCnt++;
                        return(SEQ_EX2);
                    }
                }
            }
#endif
            if (((current_radius >= UM_2_SLED_STEP(21500)) && (current_radius <= UM_2_SLED_STEP(60000))) &&
                ((ABS(pll_radius - current_radius) <= 256) || ((ABS(pll_radius - Iram_stp_CurrentPosition) > 256) && (addrErrCnt >= 3))))
            {//adjust sledge position by addr
                SendMsgCnB(SHOW_DEBUG_MSG,1,0x334401,B1B(positionError/128));
                SendMsgCnB(SHOW_DEBUG_MSG,2,0x334402,B2B(Iram_stp_CurrentPosition));
                svoStp_SetCurrentPosition(Iram_stp_CurrentPosition + positionError);
                SendMsgCnB(SHOW_DEBUG_MSG,2,0x334403,B2B(Iram_stp_CurrentPosition));
            }
            else if (addrErrCnt <= 5)
            {//The address might be incorrect
                SendMsgCn(DEBUG_SEEK_MSG,1,0x3FAA0A,addrErrCnt);
                addrErrCnt++;

                return(SEQ_EX2);    /* go back to state_sectorID_chk */
            }
            else if ((svoVar_discKindT.Bit.DiscStd == eDiscCD) && (pll_radius > UM_2_SLED_STEP(30000)))
            {
                /*            (steps^2 - parN0) x 64 *
                 * parRough = ---------------------- *
                 *                 addr - parF0      */
                svoSeekVar.parRough = ((ULONG)pll_radius * pll_radius - svoSeekVar.parN0) * 16 / ((svoIramVar.currentID.SectorNum.all32 - svoSeekVar.parF0) / 4);
                if (svoSeekVar.parRough > UM_2_SLED_STEP(49931) * UM_2_SLED_STEP(49931) / 4096)// 64 min
                {
                    // WARNING: ParRough over limit: %2d
                    SendMsgCnB(SHOW_DEBUG_MSG,2,0x3FAA07,B2B(svoSeekVar.parRough));
                    svoSeekVar.parRough = UM_2_SLED_STEP(49931) * UM_2_SLED_STEP(49931) / 4096;
                }
                if (svoSeekVar.parRough < UM_2_SLED_STEP(36465) * UM_2_SLED_STEP(36465) / 4096)//120 min
                {
                    // WARNING: ParRough below limit: %2d
                    SendMsgCnB(SHOW_DEBUG_MSG,2,0x3FAA06,B2B(svoSeekVar.parRough));
                    svoSeekVar.parRough = UM_2_SLED_STEP(36465) * UM_2_SLED_STEP(36465) / 4096;
                }
                svoSeekVar.parRoughInit = svoSeekVar.parRough;
                svoSeekVar.targetRadius = svoPBA2Radius(svoSeekVar.svoTargetID.SectorNum.all32s);
            }
            //else use the address but not update Iram_stp_CurrentPosition
        }
        else
#endif
        if ((svoSeekVar.jumpType & (LIMIT_MASK|R_J_MASK)) == R_J_MASK)
        {
#ifndef SLEDGE_JUMP_ADJ
            if (current_radius > (Iram_stp_CurrentPosition + MICRO_STEP_PER_ELE_REV / 8))
                svoStp_SetCurrentPosition(Iram_stp_CurrentPosition + 1);
            if (current_radius < (Iram_stp_CurrentPosition - MICRO_STEP_PER_ELE_REV / 8))
                svoStp_SetCurrentPosition(Iram_stp_CurrentPosition - 1);
#endif
            if ((temp_jumpDistance > UM_2_SLED_STEP(50)) && (svoSeekVar.jumpDistance > UM_2_SLED_STEP(5000))
                && ((svoSeekVar.jumpType & SLIP_MASK) != SLIP_MASK))
            {//parRough adjustment needed
                par_adj = (svoSeekVar.parRough >> 10) + 1;    //1/1024 about 30um difference for full stroke.
#if (DISC_8CM == 1) /* 8 cm disc */
                if ((svoVar_discKindT.Bit.DiscStd == eDiscCD) && (svoVar_discDiameter == e8cm))
                {
                    ParRoughMax = svoSeekVar.parRoughInit + (svoSeekVar.parRoughInit >> 3);
                    ParRoughMin = svoSeekVar.parRoughInit - (svoSeekVar.parRoughInit >> 3);
                }
                else
#endif
                {
                    ParRoughMax = svoSeekVar.parRoughInit + (svoSeekVar.parRoughInit >> 6);
                    ParRoughMin = svoSeekVar.parRoughInit - (svoSeekVar.parRoughInit >> 6);
                }

                if ((svoSeekVar.jumpType & DIR_MASK) == temp_jumpType)
                {//parRough too small
                    if (svoSeekVar.parRough <= ParRoughMax)   // parRough High limit
                    {
                        svoSeekVar.parRough += par_adj;
                    }
                }
                else
                {//parRough too large
                    if (svoSeekVar.parRough >= ParRoughMin)  // parRough Low limit
                    {
                       svoSeekVar.parRough -= par_adj;
                    }
                }
            }
            else if (((svoSeekVar.jumpType & SLIP_MASK) == SLIP_MASK )&&( svoSeekVar.roughCount <= 2))
            {
                par_adj = svoSeekVar.parRough >> 9;    //1/512bout 60m difference for full stroke.
                if ((svoSeekVar.parRough >= (svoSeekVar.parRoughInit - (svoSeekVar.parRoughInit >>7))) && (svoVar_discKindT.Bit.DiscStd == eDiscDVD))
                {
                    svoSeekVar.parRough -= par_adj;
                }
            }
        }
    }
    addrErrCnt = 0;

    if (temp_jumpType == FORW)
    {
#if (DISC_8CM == 0)
        //set the outter boundary 59000um
        temp_max_jumpDistance = UM_2_SLED_STEP(58000) - Iram_stp_CurrentPosition;
#else /* DISC_8CM */
        if(svoVar_discDiameter != e8cm)
        {
            //set the outter boundary 59000um
            temp_max_jumpDistance = UM_2_SLED_STEP(58000);
        }
        else
        {
            //set the outter boundary 38500um
            temp_max_jumpDistance = UM_2_SLED_STEP(38500);
        }
        temp_max_jumpDistance = temp_max_jumpDistance - Iram_stp_CurrentPosition;
#endif  /* DISC_8CM */
    }
    else
    {
        //set the inner boundary 22500um
        temp_max_jumpDistance = Iram_stp_CurrentPosition - UM_2_SLED_STEP(22500);
    }

#ifdef SLEDGE_JUMP_ADJ
    servo_debug[2] = 0;
    if (Iram_stp_CurrentPosition < UM_2_SLED_STEP(28000))
    {
        servo_debug[3] = (rand() & 0xFFF);
        servo_debug[3] += UM_2_SLED_STEP(30000);
        temp_max_jumpDistance = servo_debug[3] - Iram_stp_CurrentPosition;
        servo_debug[2] = 2;
    }
    if (Iram_stp_CurrentPosition > UM_2_SLED_STEP(53000))
    {
        servo_debug[2] = 4;
        temp_max_jumpDistance = Iram_stp_CurrentPosition - servo_debug[3];
    }
#endif

    if (temp_max_jumpDistance < (temp_min_jumpDistance << 1))
    {
        temp_max_jumpDistance = (temp_min_jumpDistance << 1);
    }
    if (temp_jumpDistance > temp_max_jumpDistance)
    {
#ifdef SLEDGE_JUMP_ADJ
        servo_debug[2]++;
        if (servo_debug[2] == 3)
        {
            servo_debug[4] = 0;
        }
#endif
        SendMsg80(DEBUG_SEEK_MSG,0x3FAA09);
        temp_jumpDistance = temp_max_jumpDistance;
        temp_jumpType |= LIMIT_MASK;
    }

#if (DVD_RAM_READ == 1)
    if (DVDRAM)
    {
        TrackPitchChange = FALSE;
        if (((svoIramVar.currentID.SectorNum.all32 >= 0x30000) && (svoSeekVar.svoTargetID.SectorNum.all32 < 0x30000)) ||
            ((svoIramVar.currentID.SectorNum.all32 < 0x30000) && (svoSeekVar.svoTargetID.SectorNum.all32 >= 0x30000)))
        {
            TrackPitchChange = TRUE;
            temp_jumpDistance += UM_2_SLED_STEP(200);

            temp_jumpType |= LIMIT_MASK;
            if (temp_jumpDistance < temp_min_jumpDistance)
            {//always use rough search to jump cross rewrite area and embossed area.
                temp_jumpDistance = temp_min_jumpDistance;
            }
        }
        else
        {
            if ((svoSeekVar.svoTargetID.SectorNum.all32s > 0x030000) &&
                (svoSeekVar.svoTargetID.SectorNum.all32s <= 0x030F80) && (temp_jumpType == BACK))
            {  //Prevent jumping into the Embossed Area when jump to disc inside to read DMA data
                temp_jumpType |= LIMIT_MASK;
                temp_jumpDistance = (temp_jumpDistance > UM_2_SLED_STEP(350)) ? (temp_jumpDistance - UM_2_SLED_STEP(350)) : 0;
            }
        }

        zone_change = 0;
        if (current_pba > 0x30000)
            current_zone = DVDRAM_PSN2ZONE(current_pba);
        else
            current_zone = 0xff;  // Set zone number to -1 if in embossed area.

        zone_change  = target_zone - current_zone;

        //if (DVDRAM_rewritable)
        //{
        //    WRITE_FIELD(HAL_DVDD_TRACKSIZE, DVDRAMZoneTable[current_zone].SectorsPerRev);
        //    svoIramVar.tracksize_set = 2;
        //}
    }
#endif // #if (DVD_RAM_READ == 1)

    if ((temp_jumpDistance >= temp_min_jumpDistance)
#if (FDP_TRKSLIP_MONITOR == 1)
        || (svoMiscVar.forceTrackOffFlag)
#endif
       )
    {   //Rough search needed
        if (svoSeekVar.roughCount >= MAX_ROUGH_JUMP)
        {
            //too many rough jump, return fail.
            SendMsg80(SHOW_DEBUG_MSG,0x334210);//ERROR: maximum allowed rough jumps reached
            return(SEQ_EX3);
        }
        if (temp_jumpType&DIR_MASK)
            SendMsgCnB(DEBUG_SEEK_MSG,4,0x334201,B4B(temp_jumpDistance));
        else
            SendMsgCnB(DEBUG_SEEK_MSG,4,0x334200,B4B(temp_jumpDistance));


#if (HORROR_DISC_IMPROVEMENT == 1)
    #if DEFECT_LENGTH_MEASURE == 1
        if (CD_Scratch_Disc==0)  // modify for TVD-621
    #endif
            TryDFE_Param=TryDFE_Param&0xFE; //== modify for DQA Disc to retry DFE parameter
#endif
        svoSeekVar.jumpDistance = temp_jumpDistance;
        svoSeekVar.jumpType = temp_jumpType;
        svoSeekVar.jumpType |= R_J_MASK;
        svoSeekVar.roughCount++;
        return (SEQ_EX1);
    }
    else
    {   //Fine search needed
        return (SEQ_EX0);
    }
}// end of state_rough_chk

/*-----------------------------------------------------------------------------
 *
 * Module: state_fine_chk()
 * Description: This function will check if target is reached(SEQ_EX0) or a fine search is needed(SEQ_EX1)
 *
 *----------------------------------------------------------------------------*/
#if (SEEK_ADJ==1)
LONG last_PBA;
#endif
seq_exit_t state_fine_chk(void)
{
    BYTE   temp_IE;
    LONG   delta_addr;
#if (BD_ENABLE == 1)
    LONG   delta_addr1;
    LONG   delta_addr2;
#endif // (BD_ENABLE == 1)
    LONG   delta_Q_32;
    LONG   delay_Q_32;
    LONG   seek_window;
    LONG   temp_jumpDistance;
    LONG   temp_fine_threshold;
    USHORT par_adj;
    BYTE   temp_jumpType;
    BOOL   LeadIn = FALSE;

    //send_msg80(DEBUG_CALIB_ENTRYS_MSG,0x5A0126); // >>> STATE_FINE_CHK <<<

    if ((svoIramVar.svoInState & TRN_MASK) == SVO_PAUSE)
    {
        if (svoVar_abort_pause_if_applyable == TRUE)
            return(SEQ_EX0);  // time_chk: make it done anyway
    }

    temp_IE = GET_INT_ENABLE_MASK();    /* disable interrupts */
    DISABLE_INTERRUPT();
    switch (svoVar_discKindT.Bit.DiscStd)
    {
        case eDiscCD:
#if (DETECT_ECC_DISC == 1)
            if(svoVar_eccDiscNum >= eECC_210_um)
                temp_fine_threshold = (FINE_THRESHOLD_CD/(svoVar_eccDiscNum-0.5));
            else
#endif
                temp_fine_threshold = FINE_THRESHOLD_CD;
            delta_addr = svoSeekVar.svoTargetID.SectorNum.all32s -
                         svoIramVar.currentID.SectorNum.all32s;
            break;
#if (BD_ENABLE == 1)
        case eDiscBD:
    #if (DETECT_ECC_DISC ==1)
            if(svoVar_eccDiscNum >= eECC_210_um)
                temp_fine_threshold = (FINE_THRESHOLD_BD/(svoVar_eccDiscNum-0.5));
            else
    #endif
            temp_fine_threshold = FINE_THRESHOLD_BD;
            if ((bdDecFlag.fREADING_PIC == SET) && (svo_BDR_RE_PICReading == SET))
            {
               delta_addr = ((svoSeekVar.svoTargetID.SectorNum.all32s & 0x00FFFFFE) -
                         (svoIramVar.currentID.SectorNum.all32s & 0x00FFFFFE))*24;
            }
            else if ((bdDecFlag.fREADING_PIC == CLEAR) && (svo_BDR_RE_PICReading == SET) && (svoSeekVar.svoTargetID.SectorNum.all32s > PIC_L0_MAX_ADDR))
            {
                ULONG CurrentAddress;
                CurrentAddress = (ULONG)(READ_REG(HAL_BDC_AUNFLY_REGISTER));
                delta_addr1 = ((svoSeekVar.svoTargetID.SectorNum.all32s & 0x00FFFFFE) - PIC_L0_MAX_ADDR);
                delta_addr2 = (PIC_L0_MAX_ADDR - (CurrentAddress & 0x00FFFFFE))*24;
                delta_addr = (LONG)((float)delta_addr1 + 1.04*delta_addr2);
            }
            else
            {
                delta_addr = (svoSeekVar.svoTargetID.SectorNum.all32s & 0x00FFFFFE) -
                         (svoIramVar.currentID.SectorNum.all32s & 0x00FFFFFE);
            }
            break;
#endif // (BD_ENABLE == 1)
#if (ENABLE_HDDVD == 1)
        case eDiscHD:
            temp_fine_threshold = FINE_THRESHOLD_HD;
            delta_addr = (svoSeekVar.svoTargetID.SectorNum.all32s & 0x007FFFFF) -
                         (svoIramVar.currentID.SectorNum.all32s & 0x007FFFFF);
            LeadIn = (svoMiscVar.HD_System_leadInOutFlag == PRESET_HD_SYSTEM_LEAD_IN);
            break;
#endif // (ENABLE_HDDVD == 1)

        case eDiscDVD:
#if (DVD_RAM_READ == 1)
            if (DVDRAM)
            {
                temp_fine_threshold = FINE_THRESHOLD_DVDRAM;
            }
            else
#endif
            {
                temp_fine_threshold = FINE_THRESHOLD_DVD;
            }
            delta_addr = (svoSeekVar.svoTargetID.SectorNum.all32s & 0x003FFFFF) -
                         (svoIramVar.currentID.SectorNum.all32s & 0x003FFFFF);
            break;
    }

    SET_INT_ENABLE_MASK(temp_IE);   /* restore interrupts */

    temp_jumpDistance = svoSeekVar.parFine2 * UM_2_SLED_STEP(10000);
    temp_jumpDistance = ABS(temp_jumpDistance);
    if (delta_addr > temp_jumpDistance) delta_addr = temp_jumpDistance;
    if (delta_addr < -temp_jumpDistance) delta_addr = -temp_jumpDistance;

#if (DVD_RAM_READ == 1)
    if (zone_change)
    {
        spdUpdateRPM(Iram_stp_CurrentPosition);
    }
#endif

    if ((delta_addr < 0) || (delta_addr > svoSeekVar.parDelay3))
    {
        //calculate jump tracks
#if (DVD_RAM_READ == 1)
        if (DVDRAM)
        {
            delta_Q_32 = trk32_calc_DVDRAM(delta_addr,svoSeekVar.svoTargetID.SectorNum.all32s);
            delay_Q_32 = trk32_calc_DVDRAM(svoSeekVar.parDelay3,svoSeekVar.svoTargetID.SectorNum.all32s);
            if (svoSeekVar.svoTargetID.SectorNum.all32s < 0x30000)
            {
                delta_Q_32 = (LONG)delta_Q_32 / 2;
                delay_Q_32 = delay_Q_32 >> 1; /* delay_Q_32 always positive */
            }
        }
        else
#endif
        {
            if (!LeadIn)
            {
                delta_Q_32 = delta_addr * svoSeekVar.parFine / (svoSeekVar.targetRadius - delta_addr / svoSeekVar.parFine2);
                delay_Q_32 = svoSeekVar.parDelay3 * svoSeekVar.parFine / svoSeekVar.targetRadius;
            }
            else
            {
                delta_Q_32 = delta_addr * svoSeekVar.parFine_Li / (svoSeekVar.targetRadius - delta_addr / svoSeekVar.parFine2);
                delay_Q_32 = svoSeekVar.parDelay3 * svoSeekVar.parFine_Li / svoSeekVar.targetRadius;
            }
        }
        delay_Q_32 += (svoSeekVar.parDelay1 + ABS(delta_Q_32) * svoSeekVar.parDelay2) / svoSeekVar.revDur + 1;

        temp_jumpDistance = delta_Q_32 - delay_Q_32;
        temp_jumpDistance = temp_jumpDistance / 32;

#if (DVD_RAM_READ == 1)
        if (DVDRAM && (svoSeekVar.svoTargetID.SectorNum.all32s >= 0x30000))
        {
            temp_jumpDistance = (LONG)temp_jumpDistance / 2;
            delay_Q_32 = delay_Q_32 >> 1; /* delay_Q_32 always positive */
        }
#endif

        seek_window = (delay_Q_32 >> 2); /* delay_Q_32 /= 4 preset seek window to delay_Q_32 / 4*/
#if (ENABLE_UP_DOWN_JUMP_CONTROL == 1)
        if (svoSeekVar.rx_available == FALSE)
        {/* because one small forward jump will become 2 jump, so we increase seek window by 1 delay_Q. */
            seek_window += delay_Q_32;
        }
#endif // (ENABLE_UP_DOWN_JUMP_CONTROL == 1)
        seek_window = seek_window >> 5;

        //seek_window += ((svoSeekVar.fineCount < 6)? (svoSeekVar.fineCount / 2) : 3;
        if (svoSeekVar.fineCount > 6)
            seek_window++;

        if ((temp_jumpDistance < seek_window) || (delta_addr < 0))
        {
            if (delta_addr >= 0)
            {//before target but the calculation result is jump backward or jump forward less than seek_window -> don't jump and return SEQ_EX0.
                temp_jumpDistance = 0;
            }
            else
            {
                temp_jumpDistance --;
            }
        }
    }
    else
    {
        temp_jumpDistance = 0;
    }

    /* Protection for CDROM boundary detect fail case */
    if (svoVar_discKindT.Bit.DiscStd == eDiscCD)
    {
        if ((!svo_BoundaryDet)&&(svoVar_discKindT.fDiscKind == eCDROMDisc)&&(svoSeekVar.svoTargetID.SectorNum.all32s <= svoSeekVar.parDelay3)&&(svoIramVar.currentID.SectorNum.all32s<0))
        {
            temp_jumpDistance = 0;
        }
    }

    if ((svoIramVar.svoInState & TRN_MASK) == SVO_PAUSE)
    {
        if (
#if (SEEK_ADJ==1)
            (temp_jumpDistance > -servo_debug[0])
#else // (SEEK_ADJ==1)
    #if (ENABLE_UP_DOWN_JUMP_CONTROL == 1)
            ((svoSeekVar.rx_available == FALSE) && (temp_jumpDistance >= -UP_DOWN_JUMP_LIMIT1)) ||// only jump back for pause and at least 20 tracks, and only active during write
    #endif // of (ENABLE_UP_DOWN_JUMP_CONTROL == 1)
            (temp_jumpDistance > -PAUSE_DISTANCE) // only jump back for pause and at least 5 tracks
#endif // (SEEK_ADJ==1)
           )
        {
            temp_jumpDistance = 0;
        }
#if (SEEK_ADJ==1)
        else
        {
            temp_jumpDistance = -servo_debug[0];
        }
#endif // (SEEK_ADJ==1)
    }

#if (DVD_RAM_READ == 1)
    if (zone_change)
    {
        const StructureOfGroupsType *pTargetZone = &DVDRAMZoneTable[target_zone];
        ULONG                       SeekLimitPSN;

        SeekLimitPSN = pTargetZone->StartUserSectorPSN + pTargetZone->SectorsPerRev*8;
        if (svoSeekVar.svoTargetID.SectorNum.all32s < SeekLimitPSN)
        {
            temp_jumpDistance += (SeekLimitPSN-svoSeekVar.svoTargetID.SectorNum.all32s+pTargetZone->SectorsPerRev/2)/pTargetZone->SectorsPerRev/2;
        }
    }
#endif

    if (temp_jumpDistance == 0)
    {
        //reach target
#if DO_HW_SUPPORT_STEP == 1
        hw_step_motor_normal_play();
#endif // DO_HW_SUPPORT_STEP == 1
        if ((svoIramVar.svoInState & TRN_MASK) != SVO_PAUSE)
        {
            SendMsgCnB(DEBUG_SEEK_MSG,6,0x334006,B4B(ReadTimer(&seektime)),B1B(svoSeekVar.roughCount),B1B(svoSeekVar.fineCount));
            SendMsgCn(DEBUG_SEEK_MSG,4,0x334030,A2B(svoSeekVar.parRough),A2B(svoSeekVar.parFine));
        }

        if (svoVar_discKindT.Bit.DiscStd == eDiscCD)
        {
            svoIramVar.subQEnableFlag &= (~ SUBQ_EI_DI_SERVO);
        }

        SetTimeout(0, TIMER_STOP);
        svoSeekVar.jumpType = 0;
#if (SEEK_ADJ==1)
        last_PBA = svoIramVar.currentID.SectorNum.all32s;
#endif // (SEEK_ADJ==1)
        return(SEQ_EX0);
    }
    //Add check counter of searching RF & detecting noRF to limit servo seek retry.

    if (svoSeekVar.fineCount >= MAX_FINE_JUMP)
    {
        //too many fine jump, return fail.
        SendMsg80(SHOW_DEBUG_MSG,0x334211);//ERROR: maximum allowed fine jumps reached
        return(SEQ_EX3);
    }

#if (SEEK_ADJ == 1)
    seek_adj_flag = 1;
    TimerMeasureStart();
#endif
    if ((svoSeekVar.svoTargetID.Layer == 1) &&
       (((svoVar_discKindT.Bit.DiscStd == eDiscDVD) && (pdbGetDVDTrackPath() == DVDHD_OTP_DISC))
#if (ENABLE_HDDVD == 1)
    || ((svoVar_discKindT.Bit.DiscStd == eDiscHD) && (pdbGetHDTrackPath() == DVDHD_OTP_DISC))
#endif // (ENABLE_HDDVD == 1)
#if (BD_ENABLE == 1)
    || (svoVar_discKindT.Bit.DiscStd == eDiscBD)
#endif // (BD_ENABLE == 1)
     ))
    {//change jump direction for OTP layer 1
        temp_jumpDistance = -temp_jumpDistance;
    }

    if (temp_jumpDistance < 0)
    {
        temp_jumpDistance = -temp_jumpDistance;
        temp_jumpType = BACK;
    }
    else
    {
        temp_jumpType = FORW;
    }

    if (((svoSeekVar.jumpType & (LIMIT_MASK|F_J_MASK)) == F_J_MASK) && (svoSeekVar.jumpDistance > 30) && (!LeadIn))
    {//parFine adjustment needed
        par_adj = svoSeekVar.parFine >> 10;    //1/1024.
        if ((svoSeekVar.jumpType & DIR_MASK) == temp_jumpType)
        {//parFine too small
            if (svoSeekVar.parFine < ((ULONG)svoSeekVar.parFineInit * 11 / 10))
                svoSeekVar.parFine += par_adj;
        }
        else
        {//parFine too large
            if (svoSeekVar.parFine > ((ULONG)svoSeekVar.parFineInit * 9 / 10))
                svoSeekVar.parFine -= par_adj;
        }
    }

#if (ENABLE_UP_DOWN_JUMP_CONTROL == 1)
  // TBD? SC5120 has no actuator break pulse, initial acceleration needed to ensure jump in right direction
  // (wrt track) is so large that small actuator jumps overshoot.
  // A solution is force a double jump (back and forth) to compensate
  // note temp_jumpDistance is in units of tracks here
    if ((svoSeekVar.rx_available == FALSE) && (temp_jumpDistance <= UP_DOWN_JUMP_LIMIT1)
    #if (DVD_RAM_READ == 1)
        && (!DVDRAM)
    #endif
    #if (SEEK_ADJ==1)
        && ((svoIramVar.svoInState & TRN_MASK) != SVO_PAUSE)
    #endif // (SEEK_ADJ==1)
       )
    {
        BYTE extra_jumpDir;

        delay_Q_32 = delay_Q_32 >> 5; /* delay_Q_32 /= 32, now the unit of delay_Q_32 is one track */
        /* set extra_JumpDir based on current radius to prevent fall out of disc */
        if (Iram_stp_CurrentPosition > UM_2_SLED_STEP(28000))
        {
            extra_jumpDir = BACK;
        }
        else
        {
            extra_jumpDir = FORW;
        }

        if (temp_jumpType == extra_jumpDir)
        {
            temp_jumpDistance = temp_jumpDistance + UP_DOWN_JUMP_LIMIT2 + delay_Q_32;
        }
        else
        {
            temp_jumpDistance = UP_DOWN_JUMP_LIMIT2 + delay_Q_32;
        }
        temp_jumpType = (extra_jumpDir | LIMIT_MASK);
    }
#endif // of (ENABLE_UP_DOWN_JUMP_CONTROL == 1)

    if ((svoIramVar.svoInState & TRN_MASK) != SVO_PAUSE)
    {
        if (temp_jumpType & DIR_MASK)
            SendMsgCnB(DEBUG_SEEK_MSG,4,0x334101,B4B(temp_jumpDistance));
        else
            SendMsgCnB(DEBUG_SEEK_MSG,4,0x334100,B4B(temp_jumpDistance));
    }
    if (temp_jumpDistance > temp_fine_threshold)
    {
        temp_jumpDistance = temp_fine_threshold;
        temp_jumpType |= LIMIT_MASK;
    }
    svoSeekVar.jumpDistance = temp_jumpDistance;
    svoSeekVar.jumpType = temp_jumpType;
    svoSeekVar.jumpType |= F_J_MASK;
#if (OPEN_SESSION==1)
    if ((RF_search_count != 0) && (svoVar_TrackingMethod == TRK_METHOD_DPD) && (svoVar_discKindT.Bit.Media == eMediaR)
        && (svoVar_discKindT.Bit.DiscStd == eDiscDVD) &&((temp_jumpType&DIR_MASK) == FORW) && (svoSeekVar.svoTargetID.Layer == 1))
    {
        svoSeekVar.parDelay1 = ((((LONG)(svoSeekVar.parDelay1 - (16000))) > 0) ? svoSeekVar.parDelay1 - (16000) : 0 );
        SendMsg80(SHOW_DEBUG_MSG,0x334109);
    }
    else
#endif
    if (++svoSeekVar.fineCount >= 3)    //increase seek window after 3 fine jumps;
    {
        svoSeekVar.parDelay1 += (1600*50);    //increase seek window by 5.0 msec.
    }
    return(SEQ_EX1);
}// end of state_fine_chk

/*-----------------------------------------------------------------------------
 *
 * Module: state_afe_vref_aj1()
 * Description: Measure vref_fm_mux3_mV and v21_fm_mux1_mV
 *
 *----------------------------------------------------------------------------*/

seq_exit_t state_afe_vref_aj1(void)
{
    ad_dsp_t  signal_ret;
#if (SERVO_DEBUG_VARIABLE != 0)
    ULONG  temp;
    USHORT i;

    for (i = 0; i < SERVO_DEBUG_VARIABLE; i++)
    {
        temp = (ULONG)&servo_debug[i];
        SendMsgCnB(SHOW_DEBUG_MSG,5,0x3FAA08,B1B(i),B4B(temp));
    }
    #if (SEEK_ADJ == 1)
    servo_debug[0] = 20;//preset pause jump distance for SEEK_ADJ
    WRITE_FIELD(HAL_GLOBAL_SEN4OUT,1);
    #endif // (SEEK_ADJ == 1)
#endif // (SERVO_DEBUG_VARIABLE != 0)

//Initial/Reset/Force hold SRC!!!
//This should be executed after set HAL_SRV_FR2X
//Execute init_SRC always to put SRC in safe state!, independent of SRC_ENABLE
    //----------------------------------------------------;
    // initial SRC                                          ;
    //----------------------------------------------------;
    init_SRC();
    SendMsg80(DEBUG_AUTO_TUNING_MSG,0x315FFF); //***VREF_TBL***
    SendMsg80(DEBUG_CALIB_ENTRYS_MSG,0x3A0102);//>>> STATE_AFE_VREF_AJ <<<

    vWakeUpFromPowerSaveMode();    /* correct the trayin error while standby mode */

    EFM_SMP_Control(EFMSTOP,SMP_ON);
    SetMotorDriverState(SHORT_BRAKE);

    if((svoCalVar.calDoneBits & vref_end) == vref_end)
    {
        return(SEQ_EX1);
    }

    //#if (AFE_POWER_DOWN == 1)
    //AFEPowerOn(); /* AFE Power ON */
    //#endif /* AFE_POWER_DOWN */

    WRITE_FIELD(HAL_AFE_MUX1ADSEL,halAFE_MUX1ADSEL_VREFI);
    Delay100uS(10);
    vref_fm_mux3_mV = ReadAverageAdcmV(ADC_MUXOUT1, 5);
    SetMotorDriverState(SHORT_BRAKE); //Joe: need check
    svoCalVar.calDoneBits |= vref_end;

    //measure V21 (mV) and save to v21_fm_mux1_mV
#if (CHIP_REV >= 0xC1)
    WRITE_FIELD(HAL_AFE_PDDPD,0);
    WRITE_FIELD(HAL_AFE_EQDOSEL,halAFE_EQDOSEL0);
#else
    WRITE_FIELD(HAL_AFE_EQSVOSEL,halAFE_EQSVOSEL3);
    WRITE_FIELD(HAL_AFE_EQDOSEL,halAFE_EQDOSEL1);
#endif
    WRITE_FIELD(HAL_AFE_RFTPEN,TRUE);
    WRITE_FIELD(HAL_AFE_MUX1ADSEL,halAFE_MUX1ADSEL_RFTP1);
    Delay100uS(10);
    signal_ret = ReadGPADCmV(NO_REVOLUTION, ADC_MUXOUT1, FALSE);
    v21_fm_mux1_mV = signal_ret.middle_mv;
    SendMsgCn(SHOW_DEBUG_MSG,4,0x316005,A2B(vref_fm_mux3_mV),A2B(v21_fm_mux1_mV));
    WRITE_FIELD(HAL_AFE_RFTPEN,FALSE);

    return(SEQ_EX0);
}// end of state_afe_vref_aj1

/*-----------------------------------------------------------------------------
 *
 * Module: state_trk_recover()
 * Description: switch track open loop for a while then close loop again
 *
 *----------------------------------------------------------------------------*/
seq_exit_t state_trk_recover(void)
{
    ClearAddressRead();
    track_slip_protection(TRUE);

    return(SEQ_EX0);
}

/*-----------------------------------------------------------------------------
 *
 * Module: state_disc_chg()
 * Description:Designer can change speed down/up according different Disc/Speed/Servo TRN
 *             After recovery ready, recover to the original speed
 *----------------------------------------------------------------------------*/
#if (PCB == 692)
                                    //  0       1       2       3      4       5       6       7       8       9       10      11      12      13      14
                                    // kt1c    kt1d    kt1e    kt1f   kt11    kt12    kt13    kt14    kt15    kt16    kt17    kt18    kt19    kt1A   src_kt2
const WORD cdTRACKING_EQ_BWHIGH[15]={ 0x8031, 0x7FEB, 0x8031, 0x7FEB, 0x2000, 0xE1B7, 0x644F, 0x4000, 0xC36E, 0x0000, 0x3227, 0x0000, 0x7C1F, 0x7C1F, 0x03CF };
#endif

seq_exit_t state_disc_chg(void)
{
#if (PCB == 692)
    #if (SRC_ENABLE == 1)
    LONG           src_kx_val;
    #endif
#endif

    SetTimeout(0, TIMER_STOP);
    SetTimeout(1, TIMER_STOP);

    if (ARG2 == 1)
    {
        if (svoVar_discKindT.Bit.Media != eMediaROM)
        {//No media rom
            //TON_ON_OFF(OFF);        /* Reset Atip Decoder (for CD-R/CD-RW) */
            //TON_ON_OFF(ON);
            return(SEQ_EX0);
        }
    }

    if (ARG3 == OFF)
    {
#if (DVD_RAM_READ == 1)
        if(DVDRAM_rewritable)
        {
            WblPllFreqPreset = 0;
            LAL_WOBBLE_SetHeaderDetectMode(SlicedWobble);
        }
#endif
    }
    else
    {
        svoIramVar.knlTimeupCountFlag = FALSE;
        if (svoSlipSign == ON)
        {
#if 0//just an example (FDP_TRKSLIP_MONITOR == 1)
            if (svoMiscVar.trackSlipCount >= FORCE_TRK_OFF_TH)
            {
                svoStp_SetTimerMode( STEP_DISABLE_TIMER );
                track_off();
                svoMiscVar.forceTrackOffFlag = TRUE;
                if (get_SIF(EXECUTIONSTATE) == svoBusy)
                {
                    svoCmdVar.svoCommand = CNOP;
                    set_SIF(EXECUTIONSTATE, svoError);  /* set ExecutionState "svoError" */
                    svoCmdVar.playerError = eErrorAccess;
                }
                return SEQ_EX0;
            }
#endif
            SendMsg80(STATE_MACHINE_FLOW_MSG,0x30001E);
#if (PCB == 692)
            if(svoVar_discKindT.Bit.DiscStd == eDiscCD)
            {
                if( (svoIramVar.svoCurrentSpeed ==_4LX)||(svoIramVar.svoCurrentSpeed ==_10AX) )
                {
                    if(READ_DSP_COEF(src_kf2) != cdTRACKING_EQ_BWHIGH[14])
                    {
                        WRITE_DSP_COEF(kt1c, cdTRACKING_EQ_BWHIGH[0]);
                        WRITE_DSP_COEF(kt1d, cdTRACKING_EQ_BWHIGH[1]);
                        WRITE_DSP_COEF(kt1e, cdTRACKING_EQ_BWHIGH[2]);
                        WRITE_DSP_COEF(kt1f, cdTRACKING_EQ_BWHIGH[3]);
                        WRITE_DSP_COEF(kt11, cdTRACKING_EQ_BWHIGH[4]);
                        WRITE_DSP_COEF(kt12, cdTRACKING_EQ_BWHIGH[5]);
                        WRITE_DSP_COEF(kt13, cdTRACKING_EQ_BWHIGH[6]);
                        WRITE_DSP_COEF(kt14, cdTRACKING_EQ_BWHIGH[7]);
                        WRITE_DSP_COEF(kt15, cdTRACKING_EQ_BWHIGH[8]);
                        WRITE_DSP_COEF(kt16, cdTRACKING_EQ_BWHIGH[9]);
                        WRITE_DSP_COEF(kt17, cdTRACKING_EQ_BWHIGH[10]);
                        WRITE_DSP_COEF(kt18, cdTRACKING_EQ_BWHIGH[11]);
                        WRITE_DSP_COEF(kt19, cdTRACKING_EQ_BWHIGH[12]);
                        WRITE_DSP_COEF(kt1A, cdTRACKING_EQ_BWHIGH[13]);
                        WRITE_DSP_COEF(src_kt2, cdTRACKING_EQ_BWHIGH[14])  ;

    #if (SRC_ENABLE == 1)
                        src_kx_val = ((LONG)READ_DSP_COEF(src_kt2) * 0x8000) / SRC_TRK_PRESCALE;
                        if (src_kx_val > 32767) src_kx_val = 32767;
                        else if (src_kx_val < -32768) src_kx_val = -32768;
                        WRITE_DSP_COEF(src_kt,(USHORT)src_kx_val);
                        WRITE_DSP_COEF(src_trk_prescale, SRC_TRK_PRESCALE);
    #endif
                    }
//                    else//Recover to original EQ if need
//                    {
//                        svo_Block_Eq_Data_Set(svoIramVar.svoCurrentSpeed, TRACKING_LOW_EQ);
//                        svo_Block_Eq_Data_Set(svoIramVar.svoCurrentSpeed, TRACKING_EQ);
//                    }
                }
            }
#endif//#if (PCB == 692)
            svoIramVar.svoInState = svoIramVar.svoOrigInState;
            svoSlipSign = OFF;
            return(SEQ_BUSY);
        }// == start == Code clean up and move a focus drop recovery code from Diag.c to state_disc_chg()
        //else if (svoVar_discKindT.Bit.DiscStd == eDiscCD)
        else if ((svoVar_discKindT.Bit.DiscStd == eDiscCD)||(svoVar_discKindT.Bit.DiscStd == eDiscBD))//==start,end==If BD occurs focus drop, then also release and re-lock DFE block sync as CD case
        {
            if( ((svoIramVar.svoInState & STAT_MASK) == FOCUS_DROP) && ((svoIramVar.svoInState & TRN_MASK) >= READY_TRN) )
            {
                SetDfeWblFrontOnHoldTracking();
                LAL_DFE_SetEqualizer(lalDFE_EqualizerStrengthNormal);//lalDFE_EqualizerStrengthLow,lalDFE_EqualizerStrengthNormal,lalDFE_EqualizerStrengthHigh
                SetDfeWblFrontSemiSynchrAfterTracking();

                Delay100uS(5);
                if (!HAL_DFE_GetPllInLock())
                {
                    Delay100uS(5);
                }
                SetDfeWblFrontBitSynchr();
            }
        }//== end == Code clean up and move a focus drop recovery code from Diag.c to state_disc_chg()
    }

    return SEQ_EX0;
}// end of state_disc_chg

/*-----------------------------------------------------------------------------
 *
 * Module: state_seek_retry()
 * Description:
 *
 *----------------------------------------------------------------------------*/
seq_exit_t state_seek_retry(void)
{
#if (DeadLoop_Solution == 1)
    if(svoVar_discKindT.Bit.DiscStd == eDiscCD)
    {
        if(svoWblErrCount < 3)
        {
            svoWblErrCount++;
            send_msg1('W','H','3','3');
            return(SEQ_EX3);
        }
        else
        {
            //this error will stop the disc
    #if (EN_WOBBLE == 1)                 // Support ATIP Only when wobble is enabled
            HAL_WOBBLE_EnableNewWblAddInterrupt(FALSE);     /* Disable ATIP Interrupt - Stop ATUP decoding */
    #endif // (EN_WOBBLE == 1)
            WRITE_FIELD(HAL_CDD_SCOREN, 0);                 /* Disable SUB-Q interrupt - Stop SubQ decoding */
            set_SIF(EXECUTIONSTATE,svoError);
            svoWblErrCount = 0;
            svo_ERR_STARTUP = TRUE;
            return(SEQ_EX1);
        }
    }
    else
    {// DVD/HD/BD
        if(svoWblErrCount < 3)
        {
            svoWblErrCount++;
            svoIramVar.svoInState = (svoIramVar.svoInState & TRN_MASK) | ARG1;
            if(svoVar_discKindT.Bit.Layers == eLayersSingle)
            {
                if((svoWblErrCount > 2) ||
                    ((((svoSeekVar.svoTargetID.SectorNum.all32 & 0x00FFFFFF) < 0x0030000) || ((
    #if (ENABLE_HDDVD == 1)
                    (((svoSeekVar.svoTargetID.SectorNum.all32 & 0x00FFFFFF) > 0x0073DBFF)&&(svoVar_discKindT.Bit.DiscStd == eDiscHD))||
    #endif // (ENABLE_HDDVD == 1)
                    (((svoSeekVar.svoTargetID.SectorNum.all32 & 0x00FFFFFF) > 0x00260000)&&(svoVar_discKindT.Bit.DiscStd == eDiscDVD)))&&(svoSeekErrCnt > 0)))))
                {
                    switch((svoIramVar.svoInState & TRN_MASK))
                    {
                        case SVO_SEEK:
                            svoCmdVar.svoCommand = CSEEK;
                            break;
                        case SVO_PLAY:
                            if(svoCmdVar.svoCommand == CNOP)
                                break;
                        case SVO_PAUSEINI:
                        case SVO_PAUSE:
                            if(svoCmdVar.svoCommand == CNOP)
                            {
                                svoCmdVar.svoCommand = CPAUSE;
                            }
                            else
                            {
                                if(svoCmdVar.svoCommand == CSEEK)
                                    fgseektime = 0;

                                svoGetIDErrCount = 0;
                                svoSeekErrCnt = 0;
    #if (DeadLoop_Solution == 1)
                                svoWblErrCount = 0;
    #endif
                            }
                            break;
                        default:
                            break;
                    }
                    return(SEQ_EX2);
                }
            }
        }
        else
        {
            //this error will stop the disc
            set_SIF(EXECUTIONSTATE,svoError);
            svoWblErrCount = 0;
            svo_ERR_STARTUP = TRUE;
            return(SEQ_EX1);
        }
    }
    return(SEQ_BUSY);
#else /*deadloop solution */
    return(SEQ_EX3);
#endif   /*deadloop solution */
}

/*-----------------------------------------------------------------------------
 *
 * Module: state_search_rf_zone()
 * Description: search written area around svoSeekVar.targetRadius, if no written area is found then
 *              ARG1=1: jump to known written area Recorded_stp_Position[Layer_Index]
 *              ARG1=0: jump to svoSeekVar.targetRadius
 *
 *----------------------------------------------------------------------------*/
seq_exit_t state_search_rf_zone(void)
{
    SHORT  target_position,limit_position;
    static BYTE rf_tries;
    SHORT  move_distance;//Add search RF flow whose action is that go back to original position and keep move 1mm to zone which is possible to has RF when doing last search.

    if (svoIramVar.svoPreInState != svoIramVar.svoInState)
    {
        track_off();
        svoSeekVar.jumpType |= LIMIT_MASK;
        rf_tries = 0;
    }

    // Move sled in and out
    move_distance = UM_2_SLED_STEP(2000);// Add search RF flow whose action is that go back to original position and keep move 1mm to zone which is possible to has RF when doing last search.
    rf_tries++;
    svoMiscVar.searchRFCount++;

    SendMsgCn(DEBUG_SEEK_MSG,1,0x3FAA38,rf_tries);//Search RF jump %d

    // Move a little distance after find RF zone and modify search rf flow.
#if (OPEN_SESSION == 1)
    if (RF_search_count < 0xFF)
    {
        RF_search_count++;
    }
#endif

    if ((rf_tries >= 3) || (svoMiscVar.searchRFCount >= 6))
    {
        if(svoMiscVar.searchRFCount >= 10)//Treat this as "NO RF FOUND" case
        {
            svoMiscVar.searchRFCount = 0;
            return(SEQ_EX1);
        }

        if (ARG1 == 1)
        {
            target_position = Recorded_stp_Position[Layer_Index];
        }
        else
        {
            target_position = svoSeekVar.targetRadius;
        }
        svoStp_JumpToPosition(STEP_NORMAL_MODE, target_position);
    }
    else
    {
        if (rf_tries & 0x01)
        {
            move_distance = -move_distance;
        }
        if ((Layer_Index == 1) && (pdbGetDVDTrackPath() == DVDHD_OTP_DISC))
        {
            move_distance = -move_distance;
        }
        target_position = svoSeekVar.targetRadius + move_distance;

        if (target_position < UM_2_SLED_STEP(22500))
        {
            target_position = UM_2_SLED_STEP(22500);
        }
#if (DISC_8CM == 0)
        limit_position = UM_2_SLED_STEP(59000);
#else /* DISC_8CM */
        if(svoVar_discDiameter != e8cm)
        {
            limit_position = UM_2_SLED_STEP(59000);
        }
        else
        {
            limit_position = UM_2_SLED_STEP(38500);
        }
#endif  /* DISC_8CM */
        if (target_position > limit_position)
        {
            target_position = limit_position;
        }
        svoStp_JumpToPosition(STEP_SEARCH_RF_MODE, target_position);
    }
    //send_msgA2(1,2,3,4,target_position);
    while (svoStp_busy())
    {
        ExitProcess();
    }
    //send_msgA2(1,2,3,5,Iram_stp_CurrentPosition);

    if (RFpp_chk() == TRUE)
    {
        //send_msg5SValue(1, "RF FOUND", 1, rf_tries);
        if (ARG1 == 1)
        {
            wait_track_on(TRK_ON_WAIT_SLOW_TIMEOUT);
            svoStp_SetTimerMode( STEP_READ_TIMER );
            svoSeekVar.jumpType |= (LIMIT_MASK|SLIP_MASK);//skip seek parameter adjustment
        }
        return (SEQ_EX0);
    }
    else
    {
        //send_msg5SValue(1, "NO RF FOUND", 1, rf_tries);
        if (rf_tries >= 3)
        {
            if ((St_drv_ready == FALSE) && ((svoIramVar.svoInState & TRN_MASK) == SVO_TRON) && (Layer_Index == 1))
            {
                svoStp_JumpToPosition(STEP_SAFE_SLOW_MODE, Recorded_stp_Position[0]);

                while (svoStp_busy())
                {
                    ExitProcess();
                }
                WaitTimer(20);
                HAL_SERVO_CenterControlOFF();
                svoIramVar.currentID.SectorNum.all32s = svoRadius2PBA(Iram_stp_CurrentPosition, 0);
                SetDelayTimer(10);

                svoSeekVar.svoTargetID.Layer = 0;

                svoCalVar.discChkDoneBits |= blank_chk_end;
                svoCalVar.calGainDoneBits |= (tebg_end | ceg_end | teg_end | rpg_end | rpbg_end | dif_RF_end | dif_WOB_end | felg_end | telg_end);

                SendMsg80(SHOW_DEBUG_MSG,0x33FF55); //"L1 track on fail due to no RF, skip calibrations, jump to L0"

                (void)svo_Layer_Jump(svoSeekVar.svoTargetID.Layer);
                 return(SEQ_EX2);
            }
            else
            {
                 return(SEQ_EX1);
            }
        }
    }
    return (SEQ_BUSY);
}

/*-----------------------------------------------------------------------------
 *
 * Module: state_err_trk_off()
 * Description:
 *
 *----------------------------------------------------------------------------*/
seq_exit_t state_err_trk_off(void)
{
//    #if (DEFECT_LENGTH_MEASURE == 1)
//    libDisableDFCTInt();
//    #endif

    svoStp_SetTimerMode( STEP_DISABLE_TIMER );
    track_off();
    SledStop();
    ClearAddressRead();

#if (FDP_TRKSLIP_MONITOR == 1)
    svoMiscVar.forceTrackOffFlag = TRUE;
#endif
    set_SIF(EXECUTIONSTATE, svoError);  /* set ExecutionState "svoError" */
    if(svoCmdVar.svoCommand != CNOP)
    {
        svoCmdVar.svoCommand = CNOP;
        svoCmdVar.playerError = eErrorAccess;
        //set_SIF(STATE, svoStTracking);      /* set state "svoStTracking" */
    }
    else
    {
        svoCmdVar.playerError = eErrorAccess;
    }
    return(SEQ_EX0);
}

/*-----------------------------------------------------------------------------
 *
 * Module: state_trk_off()
 * Description:
 *
 *----------------------------------------------------------------------------*/
seq_exit_t state_trk_off(void)
{
//    #if (DEFECT_LENGTH_MEASURE == 1)
//    libDisableDFCTInt();
//    #endif

    SendMsg80(SHOW_DEBUG_MSG,0x33FF03); // >>> Track Off <<<

    track_off();
    SledStop();
    ClearAddressRead();

    if (svoSlipSign == ON)
    {
        svoStp_SetTimerMode( STEP_DISABLE_TIMER );
        if (svoMiscVar.trackSlipCount > 2)
            WaitTimer(100L);         /*wait 100ms*/
        else if (svoMiscVar.trackSlipCount > 0)
            WaitTimer(20L);          /*wait 20ms*/

        SetTimeout(0,TRK_ON_WAIT_SLOW_TIMEOUT);
        do
        {
            if (timeout_chk(0) == eTimerTimeout)       /* time out check */
            {
                //SetDfeWblFrontOnHold();
                //ClearAddressRead();
                SendMsg80(DEBUG_SEEK_MSG,0x334004);   // WARNING: SlowChk Timeout
                break;
            }
        }
        while(READ_FIELD(HAL_SRV_SLOW) == 0);
        SetTimeout(0,TIMER_STOP);
    }
    else
    {
        SetDelayTimer(ARG1);
    }

    return(SEQ_EX0);
}


extern BYTE const Inquiry_tbl[];

/*-----------------------------------------------------------------------------
 *
 * Module: state_tray_in()
 * Description:
 *
 *----------------------------------------------------------------------------*/
seq_exit_t state_tray_in(void)
{
   USHORT uBakSvoInStateTmp;
   USHORT uBakSvoCommandTmp;

#if (MESSAGE_ERR == 1) && (ENABLE_UART_TXD == 1)
    char   buf[5];

    buf[0] = (char)Inquiry_tbl[32];
    buf[1] = (char)Inquiry_tbl[33];
    buf[2] = (char)Inquiry_tbl[34];
    buf[3] = (char)Inquiry_tbl[35];
    buf[4] = (char)0x00;
#endif

    if (ARG3)
    {   //Only executed during tray-in, not during initialize
        vWakeUpFromPowerSaveMode();

        uBakSvoInStateTmp = svoIramVar.svoInState;
        uBakSvoCommandTmp = svoCmdVar.svoCommand;
        svoPowerOnInit();
        svoSetLDOff();
        svoIramVar.svoInState = uBakSvoInStateTmp;
        svoCmdVar.svoCommand  = uBakSvoCommandTmp;
    }

    set_SIF(STATE,svoStTrayMovingIn);       /* set state "svoStTrayMovingIn" */
    svoVar_discDiameter = eUnknownDiameter;

#if (EPO_SLOT_IN == 1)
    if ((indet) && ipCloseSW)
    {
        return(SEQ_EX1);
    }
#endif

    /*---------------------------------------------
        Temporary reset these variables here !!
        TBD? COMMENT! Need to re-organize later !!
    ----------------------------------------------*/

    //LAL_InitServo(); //tray-in/out test

#if (FORCE_DISK_KIND == 1)
    if( DiscForce == DISABLE_DISCKIND_FORCING )
#endif
    {
        svoVar_discKindT.fDiscKind = eUnknownKind;
    }

    WRITE_FIELD(HAL_SRV_FSL, svoRegValueFromTbl(NONE, HAL_SRV_FSL_ADR));   /* Set Stand-by Level */
    WRITE_FIELD(HAL_SRV_FOKL, svoRegValueFromTbl(NONE, HAL_SRV_FOKL_ADR)); /* Set FOK Level */

#if (FOOTILT_DIFFERENTIAL == 1)
    WRITE_FIELD(HAL_SRV_FOODIFEN, 1); // FOO/TILT differential
#else
    WRITE_FIELD(HAL_SRV_FOODIFEN, 0);
#endif

    svo_Adj_Data_Clr();
#if (Customer_A1 ==1)
		CompleteStrokeCount = 0;
		CompleteStroke =0;
		PositionHomeErrorCount = 0;
		FEPPErrorCount= 0;
#endif
	MediaRecovery= 0;

    //Only at init or trayin, but not in svo_Adj_Data_Clr
    svoCalVar.fmThrData.all16 = 0;
    svoCalVar.fmDelData.all16 = 0;

    svoStepVar.StepPosCal = FALSE;
#if (ALWAYS_HOMING == 1)
  #if (SET_SLOT_IN == 0)
    Iram_stp_Initialized = STEP_UNINITIALIZED;
  #else
    //Iram_stp_Initialized = STEP_UNINITIALIZED; //[Q36]power on move sledge one times
  #endif

#endif // (ALWAYS_HOMING == 1)
#if (COLLI_SWITCH == 1) && (COLLI_ALWAYS_HOMING == 1)
    Iram_colli_Initialized = STEP_UNINITIALIZED;
#endif

#if (NO_FRONT_PANEL == 1)
    return(SEQ_EX1);
#endif

    SetMotorDriverState(LODER_MUTE_OFF);

    #if (SUN710_TRAY_PROFILE == 1)  ||( EEPROM_TRAY_PROFILE ==1)
    SendMsg80(SHOW_DEBUG_MSG,0x300014);
    return(SEQ_EX0);
    #else

    #if (SET_SLOT_IN == 1)
		if(slot_level_check == 1) //[Q36] for SLT002 disc in/out test
		{
			svoTray_OutputDac(R2_TRAY_IN_LEVEL4);
		}
		else
    #endif
    	{
    svoTray_OutputDac(TRAY_IN_LEVEL1);
    	}
    SetDelayTimer(ARG1);
    SetTimeout(0, ARG2);

    return(SEQ_EX0);
    #endif
}

/*-----------------------------------------------------------------------------
 *
 * Module: state_tray_out_wait()
 * Description:
 *
 *----------------------------------------------------------------------------*/
seq_exit_t state_tray_out_wait(void)
{
    SHORT ans;
    ans=CheckDiscStopWithTrayTimer(ARG1);
    switch (ans)
    {
        case Ready: return(SEQ_EX0);
        case Busy:  return(SEQ_BUSY);
        case Error:
        default:    return(SEQ_EX1);
    }
}

/*-----------------------------------------------------------------------------
 *
 * Module: state_tray_out()
 * Description:
 *
 *----------------------------------------------------------------------------*/
#if (SET_SLOT_IN == 1) 
ULONG ToutClock; //[W.0J]Jam tray out protect function for mechanism 2011/5/27
#endif
seq_exit_t state_tray_out(void)
{
    /*reference bolck(9CBh.6) inside AFE has to be power_up allowing tray DAC active*/
    vWakeUpFromPowerSaveMode();

    //TBD! Correct params for next function//TBD:SE1
    LAL_InitServo(); //tray-in/out test
    svoIramVar.currentID.SectorNum.all32 = 0;
    svoIramVar.svoCurrentSpeed = 0xFF;
    svoIramVar.focusDropFlag = FALSE;

    set_SIF(STATE,svoStTrayMovingOut);      /* set state "svoStTrayMovingOut" */
    SetMotorDriverState(LODER_MUTE_OFF);

    #if (SUN710_TRAY_PROFILE == 1)  ||( EEPROM_TRAY_PROFILE ==1)
    SendMsg80(SHOW_DEBUG_MSG,0x300016);
    return(SEQ_EX0);
    #else
	
    #if (SET_SLOT_IN == 1) //[Q36]
	  if(fg_R2Profile)
	  {
		  if(slot_level_check == 1) //for SLT002 disc in/out test
		  {
			  svoTray_OutputDac(R2_TRAY_OUT_LEVEL3);
		  }
		  else
		  {
			  svoTray_OutputDac(R2_TRAY_OUT_LEVEL1);
		  }
	  }
	  else
    #endif	  

    svoTray_OutputDac(TRAY_OUT_LEVEL1);

    SetDelayTimer(ARG1);
    SetTimeout(0, ARG2);
    #if (SET_SLOT_IN == 1)
		StartTimer(&ToutClock);//[W.0J]Jam tray out protect function for mechanism 2011/5/27
		#endif
#if (EN_LS == 1)
    bLSDetected = FALSE; // not a LS disc
#endif // End of (EN_LS == 1)
    return(SEQ_EX0);
    #endif
}

/*-----------------------------------------------------------------------------
 *
 * Module: state_tray_in_step_up()
 * Description:
 *
 *----------------------------------------------------------------------------*/
seq_exit_t state_tray_in_step_up(void)
{
    ULONG clock1;
    SHORT dac;

    for(;;){
        if(timeout_chk(0) == eTimerTimeout)                            /* time out check */
        {
            SetTimeout(0, TIMER_STOP);
            return(SEQ_EX1);
        }

        if(ipCloseSW == ARG2)
        {
        
        #if (SET_SLOT_IN == 1) //[Q36]
        	if(fg_R2Profile)
            {
              if(slot_level_check == 1) //for SLT002 disc in/out test
              {
                svoTray_OutputDac(R2_TRAY_IN_LEVEL4);
              }
              else
              {
				svoTray_OutputDac(R2_TRAY_IN_LEVEL3);
              }
            }
			else
        #endif	
            svoTray_OutputDac(TRAY_IN_LEVEL3);
            break;
        }

        dac = (SHORT)svoGetTrayDac() - TRAY_IN_STEP_UP;
		
        #if (SET_SLOT_IN == 1) //[Q36]
			if(fg_R2Profile)
			{
				if(dac < R2_TRAY_IN_LEVEL2)
				{
					if(slot_level_check==1) //for SLT002 disc in/out test
					{
						dac = R2_TRAY_IN_LEVEL4;
					}
					else
					{
						dac = R2_TRAY_IN_LEVEL2;
					}
				}
			}
			else
        #endif	
        	{
        if(dac < TRAY_IN_LEVEL2){
            dac = TRAY_IN_LEVEL2;
        }
        	}
        svoTray_OutputDac(dac);

        StartTimer(&clock1);
        for(;;)
        {
            if(ReadTimer(&clock1) >= (ULONG)ARG1)
                break;

            if(cancel_flag_check() == NG)
                return(SEQ_BUSY);

            ExitProcess();
        }
    }

    SetTimeout(0, TIMER_STOP);

#if (SET_SLOT_IN == 1) //[Q36]
	  if(fg_R2Profile)
	  {
			svoTray_OutputDac(-150); //Thomas test OK //-170 & 300 is always reach, but gear seems bending		
			SetDelayTimer(MS_220);
			if(ipSW2 == ARG2)//[W.0J]Jam ¨¾§¨¤ù¾÷¨î 2011/5/27 
			{
					svoTray_OutputDac(R2_TRAY_OUT_LEVEL2); 	
					SetDelayTimer(MS_150);
			}
	  }
	  else
#endif

    SetDelayTimer(TI_HOLD);

    return(SEQ_EX0);
}// end of state_tray_in_step_up

/*-----------------------------------------------------------------------------
 *
 * Module: state_tray_out_step_dn()
 * Description:
 *
 *----------------------------------------------------------------------------*/
seq_exit_t state_tray_out_step_dn(void)
{
    ULONG clock1;
    SHORT dac;
		
    #if (SET_SLOT_IN == 1)
		if(DiscInFlag == 5) //[Q36]drawin retry state
		{}
		else
		DiscInFlag = 3; //[Q36]differentiate disc in flow
    #endif
    
		
    for(;;)
    {
        if(timeout_chk(0) == eTimerTimeout) {                           /* time out check */
            SetTimeout(0, TIMER_STOP);
            return(SEQ_EX1);
        }

    #if (SET_SLOT_IN == 1) //[Q36]
			   ////////for Slot-In////////
				if(slot_level_check == 1) //for SLT002 disc in/out test 1/3 disc out state
				{
					if((ipOpenSW == ARG2)&&(ipSW2 == ARG3)&&(SwitchFlag == 2))//[J50Z]
					{		   
						svoTray_OutputDac(0);		   
						SetMotorDriverState(ALL_MUTE_ON);  
						break;
					}
				}
				else
				{
					if((ipOpenSW == ARG3)&&(ipSW2 == ARG2)&&(SwitchFlag == 1)) //[X35]first state disc out stop state
					{		
					//svoTray_OutputDac(0);
					//SetMotorDriverState(ALL_MUTE_ON);
						DelaymS(250); //[X35]first state delay disc out
						break;
					}
					else if((ipOpenSW == ARG3)&&(ipSW2 == ARG2)&&(SwitchFlag == 2))//[X35]second state disc out stop state
					{		 
						svoTray_OutputDac(0);		   
						SetMotorDriverState(ALL_MUTE_ON);  
						break;
					}
				}
     #else
        if(ipOpenSW == ARG2)
        {
            break;
        }
     #endif

        dac = (SHORT)svoGetTrayDac() - TRAY_OUT_STEP_DN;

     #if (SET_SLOT_IN == 1) //[Q36]
				if(fg_R2Profile)
				{
					if(dac < R2_TRAY_OUT_LEVEL2)
					{
							if(slot_level_check == 1) //[J5zY] for SLT002 disc in/out test
							{
								dac = R2_TRAY_OUT_LEVEL3;
							}
							else
							{
								dac = R2_TRAY_OUT_LEVEL2;
							}
					}
				}
				else
     #endif	
        {
        		if(dac < TRAY_OUT_LEVEL2)
            		dac = TRAY_OUT_LEVEL2;
        }
        svoTray_OutputDac(dac);

        StartTimer(&clock1);
        for(;;)
        {
            if(ReadTimer(&clock1) >= (ULONG)ARG1)
                break;

            if(cancel_flag_check() == NG)
                return(SEQ_BUSY);

            ExitProcess();
        }
    }
	
  #if (SET_SLOT_IN == 1) //[Q36]
	if(fg_R2Profile)
    svoTray_OutputDac(R2_TRAY_OUT_LEVEL2);
	else
  #endif	
    svoTray_OutputDac(TRAY_OUT_LEVEL2);
    
    #if (SET_SLOT_IN == 1)
    //[W.0J]Jam tray out protect function for mechanism 2011/5/27
		if(ReadTimer(&ToutClock) < (MS_800)	)
		{		send_msg5S(1,"JJ");	
			  SetMotorDriverState(1);//LODER_MUTE_OFF
		   	svoTray_OutputDac(180);
		   	DelaymS(200);
		   	SetMotorDriverState(0);//ALL_MUTE_ON
   		 	svoTray_OutputDac(0);
   	}
   	#endif
		
    SetTimeout(0, TIMER_STOP);
    return(SEQ_EX0);
}// end state_tray_out_step_dn

/*-----------------------------------------------------------------------------
 *
 * Module: state_tray_stop()
 * Description:
 *
 *----------------------------------------------------------------------------*/
seq_exit_t state_tray_stop(void)
{
#if (SUN710_TRAY_PROFILE)
    Tray_Phase = 0;
#endif
    set_SIF(STATE,svoStIdleDiscUnknown);        /* set state "svoStIdleDiscUnknown" */
    trayoff();

    //if ((svoIramVar.svoInState & TRN_MASK) == COPEN)
    //    {
    //    vPowerSaveInNoDiscMode();
    //    }

    return(SEQ_EX0);
}

/*-----------------------------------------------------------------------------
 *
 * Module: state_tray_out_hold()
 * Description:
 *
 *----------------------------------------------------------------------------*/
seq_exit_t state_tray_out_hold(void)
{
    ULONG clock1;
		
#if (SET_SLOT_IN == 1) //[Q36]
	if(fg_R2Profile)
		svoTray_OutputDac(R2_TRAY_OUT_LEVEL2);
	else
#endif	
#if (Customer_A1 ==1)
		svoTray_OutputDac(TRAY_OUT_LEVEL2-60);
#else
    svoTray_OutputDac(TRAY_OUT_LEVEL2);
#endif

    StartTimer(&clock1);
    for(;;)
    {
        if(ReadTimer(&clock1) >= (ULONG)ARG1)
        {
            break;
        }

        ExitProcess();
    }
    return(SEQ_EX0);
}

#if(SUN710_TRAY_PROFILE == 1)
/*-----------------------------------------------------------------------------
 *
 * Module: etTrayPersistentTable1B()
 * Description:
 * if fTuneMode == FALSE, return FLASHROM data,
 *    fTuneMode == TRUE,  return DRAM data,
 *----------------------------------------------------------------------------*/

SHORT GetTrayPersistentTable1B(USHORT ADDR)
{
    if(fTuneMode == TRUE)
        return (ReadEepromMap_1B(OEM_TRAY_BASE_ADR+ADDR));
    else
        return (EEPROM_Table[OEM_TRAY_BASE_ADR+(ADDR)]);
}
/*-----------------------------------------------------------------------------
 *
 * Module: state_tray_in_stage1()
 * Description:
 *
 *----------------------------------------------------------------------------*/
seq_exit_t state_tray_in_stage1(void)
{
   ULONG clock1;
   SHORT tray_in_level;
   SHORT tray_in_time;

    tray_in_level = -(GetTrayPersistentTable1B(0x38 + Tray_Phase)*4);
    tray_in_time  = GetTrayPersistentTable1B(Tray_Phase);

    svoTray_OutputDac(tray_in_level);
    Tray_Phase ++;
    if(Tray_Phase==3)
        tray_in_time = ARG1;
    else if(Tray_Phase==4)
        tray_in_time = MS_300;

    StartTimer(&clock1);
    for(;;)
    {
        if(ipOpenSW == ARG2)
        {
            break;
        }

        if(ReadTimer(&clock1) >= (ULONG)tray_in_time)
            break;

        ExitProcess();
    }

    if(Tray_Phase < 4 )
        return(SEQ_BUSY);
    else
    {
        Tray_Phase = 0;
        SetTimeout(0, ARG3);
        return(SEQ_EX0);
    }

}

/*-----------------------------------------------------------------------------
 *
 * Module: state_tray_in_stage2()
 * Description:
 *
 *----------------------------------------------------------------------------*/
seq_exit_t state_tray_in_stage2(void)
{
    ULONG clock1;
    SHORT tray_in_level;
    SHORT tray_in_time;

    tray_in_level = -(GetTrayPersistentTable1B( 0x3C + Tray_Phase)*4);

    svoTray_OutputDac(tray_in_level);
    Tray_Phase ++;
    if(Tray_Phase==1)
        tray_in_time = ARG1;
    else if(Tray_Phase==2)
        tray_in_time = ARG3-ARG1;

    StartTimer(&clock1);
    for(;;)
    {
        if(timeout_chk(0) == eTimerTimeout)                            /* time out check */
        {
            SetTimeout(0, TIMER_STOP);
            return(SEQ_EX1);
        }

        if(ipCloseSW == ARG2)
        {
            break;
        }

        if(ReadTimer(&clock1) >= (ULONG)tray_in_time)
            break;

        if(cancel_flag_check() == NG)
             return(SEQ_BUSY);

        ExitProcess();
    }

    if(Tray_Phase < 2 )
        return(SEQ_BUSY);
    else
    {
        SetTimeout(0, TIMER_STOP);
        Tray_Phase = 0;
        return(SEQ_EX0);
    }

}// end of state_tray_in_stage2

/*-----------------------------------------------------------------------------
 *
 * Module: state_tray_in_stage3()
 * Description:
 *
 *----------------------------------------------------------------------------*/
seq_exit_t state_tray_in_stage3(void)
{
    ULONG clock1;
    SHORT tray_in_level;
    SHORT tray_in_time;
    BYTE  TrInS3L3;

    TrInS3L3 = 0;

    tray_in_level = -(GetTrayPersistentTable1B( 0x3F + Tray_Phase)*4);
    tray_in_time  = GetTrayPersistentTable1B( 0x02 + Tray_Phase);

    Tray_Phase ++;
    if( (Tray_Phase==3)&&(!TrInS3L3) )
    {
        if(ipCloseSW == ARG2)
        {
            TrInS3L3 = 0;
        }
        else
        {
            TrInS3L3 = 1;
            svoTray_OutputDac(tray_in_level);
        }
    }
    else
    {
        svoTray_OutputDac(tray_in_level);
    }

    StartTimer(&clock1);
    for(;;)
    {
        if( (Tray_Phase==3)&&(!TrInS3L3) )
            break;

        if(ReadTimer(&clock1) >= (ULONG)tray_in_time)
            break;

        ExitProcess();
    }

    if(Tray_Phase < 3 )
        return(SEQ_BUSY);
    else
    {
        TrInS3L3 = 0;
        Tray_Phase = 0;
        SendMsg80(SHOW_DEBUG_MSG,0x300015);
        return(SEQ_EX0);
    }
}

/*-----------------------------------------------------------------------------
 *
 * Module: state_tray_out_stage1()
 * Description:
 *
 *----------------------------------------------------------------------------*/
seq_exit_t state_tray_out_stage1(void)
{
   ULONG clock1;
   SHORT tray_out_level;
   SHORT tray_out_time;

    if(svoVar_discKindT.fDiscKind == eUnknownKind)
    {
        tray_out_level = GetTrayPersistentTable1B( 0x46 + Tray_Phase)*4;
        tray_out_time  = GetTrayPersistentTable1B( 0x08 + Tray_Phase);
    }
    else
    {
        tray_out_level = GetTrayPersistentTable1B( 0x42 + Tray_Phase)*4;
        tray_out_time  = GetTrayPersistentTable1B( 0x05 + Tray_Phase);
    }

    Tray_Phase ++;
    svoTray_OutputDac(tray_out_level);

    if(Tray_Phase==4)
        tray_out_time = ARG1;
    StartTimer(&clock1);
    for(;;)
    {
        if(ipCloseSW == ARG2) //close = 1 not close
        {
            break;
        }

        if(ReadTimer(&clock1) >= (ULONG)tray_out_time)
            break;

        ExitProcess();
    }

    if(Tray_Phase < 4 )
        return(SEQ_BUSY);
    else
    {
        Tray_Phase = 0;
        return(SEQ_EX0);
    }
}

/*-----------------------------------------------------------------------------
 *
 * Module: state_tray_out_stage2()
 * Description:
 *
 *----------------------------------------------------------------------------*/
seq_exit_t state_tray_out_stage2(void)
{
    ULONG clock1,clock2;
    SHORT tray_out_level;
    SHORT tray_out_time;
    BYTE  TrOutRty;

    TrOutRty = 0;

    if(svoVar_discKindT.fDiscKind == eUnknownKind)
    {
        tray_out_level = GetTrayPersistentTable1B( 0x4F + Tray_Phase)*4;
        tray_out_time  = GetTrayPersistentTable1B( 0x0F + Tray_Phase);
    }
    else
    {
        tray_out_level = GetTrayPersistentTable1B( 0x4A + Tray_Phase)*4;
        tray_out_time  = GetTrayPersistentTable1B( 0x0B + Tray_Phase);
    }

    Tray_Phase ++;
    if(Tray_Phase==5)
    {
        tray_out_time = ARG1;
        SetTimeout(0, ARG1);
    }

    svoTray_OutputDac(tray_out_level);

    StartTimer(&clock1);
    for(;;)
    {
        if( (Tray_Phase==5)&&(timeout_chk(0)== eTimerTimeout) )        /* time out check */
        {
            SetTimeout(0, TIMER_STOP);
            return(SEQ_EX1);
        }

        if(ipOpenSW == ARG2)
        {
            Tray_Out_S2_Time = ReadTimer(&clock1);
            break;
        }

        if( (Tray_Phase==5)&&(!TrOutRty) )
        {
            if(ReadTimer(&clock1) >= (ULONG)SEC_2)
            {
                StartTimer(&clock2);
                svoTray_OutputDac(-(GetTrayPersistentTable1B( 0x31)*4));
                TrOutRty = 1;
                SendMsg80(SHOW_DEBUG_MSG,0x300018);

                for(;;)
                {
                    if( ReadTimer(&clock2) >= (ULONG)GetTrayPersistentTable1B( 0x33) )
                        break;

                    if(cancel_flag_check() == NG)
                        return(SEQ_BUSY);

                    ExitProcess();
                }
                svoTray_OutputDac(GetTrayPersistentTable1B( 0x32)*4);
            }
        }
        else
        {
            if(ReadTimer(&clock1) >= (ULONG)tray_out_time)
                break;
        }

        if(cancel_flag_check() == NG)
             return(SEQ_BUSY);

        ExitProcess();
    }

    if(Tray_Phase < 5 )
        return(SEQ_BUSY);
    else
    {
        SetTimeout(0, TIMER_STOP);
        Tray_Phase = 0;
        TrOutRty = 0;
        return(SEQ_EX0);
    }

}// end state_tray_out_stage2

/*-----------------------------------------------------------------------------
 *
 * Module: state_tray_out_stage3()
 * Description:
 *
 *----------------------------------------------------------------------------*/
seq_exit_t state_tray_out_stage3(void)
{
    ULONG clock1;
    SHORT tray_out_level;
    SHORT tray_out_time;
    BYTE  TrOutS3L5;

    TrOutS3L5 = 0;

    if( Tray_Out_S2_Time > ((GetTrayPersistentTable1B(0x34)<<8)|GetTrayPersistentTable1B(0x35)) )   /* Tight Tray */
    {
        if(svoVar_discKindT.fDiscKind == eUnknownKind)
        {
            tray_out_level = GetTrayPersistentTable1B( 0x6D + Tray_Phase)*4;
            tray_out_time  = GetTrayPersistentTable1B( 0x2C + Tray_Phase);
        }
        else
        {
            tray_out_level = GetTrayPersistentTable1B( 0x68 + Tray_Phase)*4;
            tray_out_time  = GetTrayPersistentTable1B( 0x27 + Tray_Phase);
        }
    }
    else if( Tray_Out_S2_Time < ((GetTrayPersistentTable1B(0x36)<<8)|GetTrayPersistentTable1B(0x37)) )   /* Loose Tray */
    {
        if(svoVar_discKindT.fDiscKind == eUnknownKind)
        {
            tray_out_level = GetTrayPersistentTable1B( 0x63 + Tray_Phase)*4;
            tray_out_time  = GetTrayPersistentTable1B( 0x22 + Tray_Phase);
        }
        else
        {
            tray_out_level = GetTrayPersistentTable1B( 0x5E + Tray_Phase)*4;
            tray_out_time  = GetTrayPersistentTable1B( 0x1D + Tray_Phase);
        }
    }
    else    /* Normal Tray */
    {
        if(svoVar_discKindT.fDiscKind == eUnknownKind)
        {
            tray_out_level = GetTrayPersistentTable1B( 0x59 + Tray_Phase)*4;
            tray_out_time  = GetTrayPersistentTable1B( 0x18 + Tray_Phase);
        }
        else
        {
            tray_out_level = GetTrayPersistentTable1B( 0x54 + Tray_Phase)*4;
            tray_out_time  = GetTrayPersistentTable1B( 0x13 + Tray_Phase);
        }
    }

    Tray_Phase ++;
    if(Tray_Phase==2)
        tray_out_level = -tray_out_level;
    if( (Tray_Phase==5)&&(!TrOutS3L5) )
    {
        if(ipOpenSW == ARG2)
        {
            TrOutS3L5 = 0;
        }
        else
        {
            TrOutS3L5 = 1;
            svoTray_OutputDac(tray_out_level);
        }
    }
    else
    {
        svoTray_OutputDac(tray_out_level);
    }

    StartTimer(&clock1);
    for(;;)
    {
        if( (Tray_Phase==5)&&(!TrOutS3L5) )
            break;

        if(ReadTimer(&clock1) >= (ULONG)tray_out_time)
            break;

        ExitProcess();
    }

    if(Tray_Phase < 5 )
        return(SEQ_BUSY);
    else
    {
        Tray_Phase = 0;
        TrOutS3L5 = 0;
        SendMsg80(SHOW_DEBUG_MSG,0x300017);
        return(SEQ_EX0);
    }

}

#endif //#if(SUN710_TRAY_PROFILE)

/*-----------------------------------------------------------------------------
 *
 * Module: state_tray_err()
 * Description:
 *
 *----------------------------------------------------------------------------*/
seq_exit_t state_tray_err(void)
{
    svoCmdVar.svoCommand = CNOP;
    set_SIF(EXECUTIONSTATE,svoError);       /* set ExecutionState "svoError" */
    if(ARG1 == CLOSE){
        svoCmdVar.trayError = eErrorTrayOut;
    }else{
        svoCmdVar.trayError = eErrorTrayIn;
    }
    return(SEQ_EX0);
}

#if (SERVICE_MODE == 1)
const USHORT service_command_tbl[]={
    SV_TRACK_OFF,
    SV_TRACK_ON,
    SV_FOCUS_OFF,
    SV_FOCUS_ON,
    CNOP
};


seq_exit_t state_service_cmd_new_req(void)
{
    int     i;

    if(get_SIF(EXECUTIONSTATE) == svoError)
    {
        svoIramVar.knlTimeupCountFlag = FALSE;
        svoCmdVar.svoCommand = CNOP;
        return(SEQ_EX1);
    }
    if(svoCmdVar.svoCommand != CNOP)
    {                       /* new command */
        for(i=0;;i++)
        {
            if(service_command_tbl[i] == CNOP)
            {
                break;
            }
            if(svoCmdVar.svoCommand == service_command_tbl[i])
            {
                svoIramVar.svoInState = ((svoIramVar.svoInState & (~ TRN_MASK)) | service_command_tbl[i]);
                svoCmdVar.svoCommand = CNOP;
                return(SEQ_EX0);
            }
        }
        svoCmdVar.svoCommand = CNOP;
        set_SIF(EXECUTIONSTATE,svoError);       /* set ExecutionState "svoError" */
        return(SEQ_EX1);
    }

    if(get_SIF(EXECUTIONSTATE) == svoBusy) set_SIF(EXECUTIONSTATE,svoReady);
    return(SEQ_EX1);
}
#endif // (SERVICE_MODE == 1)

/*-----------------------------------------------------------------------------
 *
 * Module: state_cmd_new_req()
 * Description: TBD!
 *
 *----------------------------------------------------------------------------*/
const USHORT command_tbl[]={
    COPEN,
    CCLOSE,
    CSTART,
    CSTOP,
    CPAUSE,
    CRELSE,
    CSEEK,
    CJUMP,
    CVELO,
#if (CALIBRATION_MANAGEMENT == 1)
    CCALIB,
#endif /* CALIBRATION_MANAGEMENT */
#if (HORROR_DISC_IMPROVEMENT == 1)
    CHGRFEQ,
#endif // (HORROR_DISC_IMPROVEMENT == 1)
    CLDAJ,
    CDIAG,   //for focus balance calibration.
#if (BCA_ENABLE == 1)
    CBCAJP,
#endif
    CNOP
};


seq_exit_t state_cmd_new_req(void)
{
    BYTE    i;

    svoVar_abort_pause_if_applyable = FALSE;

    if (get_SIF(EXECUTIONSTATE) == svoError)
    {
        svoIramVar.knlTimeupCountFlag = FALSE;
        svoCmdVar.svoCommand = CNOP;
        return(SEQ_EX1);
    }
    if (svoCmdVar.svoCommand != CNOP)
    {   /* new command */
        for (i=0;;i++)
        {
            if (command_tbl[i] == CNOP)
            {
                break;
            }
            if (svoCmdVar.svoCommand == command_tbl[i])
            {
                svoIramVar.svoInState = ((svoIramVar.svoInState & (~ TRN_MASK)) | command_tbl[i]);
                if (svoCmdVar.svoCommand == CSEEK)
                {
#if (FDP_TRKSLIP_MONITOR == 1)
                    if (svoMiscVar.forceFocusOffFlag)
                    {
                        svoMiscVar.forceFocusOffFlag = FALSE;
                        svoVar_bByPassFOKCheck = FALSE;
                    }
#endif
                    set_SIF(STATE,svoStAccessing);
                    svoSeekVar.svoTargetID = svoCmdVar.targetID;
                    switch (svoVar_discKindT.Bit.DiscStd)
                    {
                        case eDiscDVD:
                            if (svoVar_discKindT.Bit.Media == eMediaROM)
                            {
                                svoSeekVar.advance = INTERFACE_DELAY_TIME_DVDROM;
                            }
#if (DVD_RAM_READ == 1)
                            else if (DVDRAM)
                            {
                                svoSeekVar.advance = INTERFACE_DELAY_TIME_DVDRAM;
                            }
#endif
                            else
                            {
                                svoSeekVar.advance = INTERFACE_DELAY_TIME_DVDR_W;
                            }
                            break;

                        case eDiscCD:
                            if (svoVar_discKindT.Bit.Media == eMediaROM)
                            {
                                svoSeekVar.advance = INTERFACE_DELAY_TIME_CDROM;
                            }
                            else
                            {
                                svoSeekVar.advance = INTERFACE_DELAY_TIME_CDR_W;
                            }
                            break;
#if (ENABLE_HDDVD == 1)
                        case eDiscHD:
                            if ((svoVar_discKindT.Bit.Media == eMediaROM) || (svoVar_discKindT.Bit.Media == eMediaChina))
                            {
                                svoSeekVar.advance = INTERFACE_DELAY_TIME_HDROM;
                            }
                            else
                            {
                                svoSeekVar.advance = INTERFACE_DELAY_TIME_HDR_W;
                            }
                            break;
#endif
#if (BD_ENABLE == 1)
                        case eDiscBD:
                            if (svoVar_discKindT.Bit.Media == eMediaROM)
                            {
                                svoSeekVar.advance = INTERFACE_DELAY_TIME_BDROM;
                            }
                            else
                            {
                                svoSeekVar.advance = INTERFACE_DELAY_TIME_BDR_W;
                            }
                            break;
#endif
                    }
                    svoSeekVar.targetRadius = svoPBA2Radius(svoSeekVar.svoTargetID.SectorNum.all32s);
#if (DVD_RAM_READ == 1)
                    if (!DVDRAM)
#endif
                    {
                        svoSeekVar.svoTargetID.SectorNum.all32s -=
                           ((ULONG)svoSeekVar.targetRadius * svoSpeedVar.CurrentRpm / svoSeekVar.parFine * svoSeekVar.advance / 18750L);
                    }

                    if ((svoSeekVar.targetRadius > UM_2_SLED_STEP(57600)) && (svoVar_TrackingMethod == TRK_METHOD_DPD))
                    {//prevent jump to blank area
                        svoSeekVar.targetRadius -= ((svoSeekVar.targetRadius - UM_2_SLED_STEP(57600)) / 8);
                    }

                    svoSeekVar.advance = 0;
                }

                if (svoVar_svoTargetSpeed != svoIramVar.svoCurrentSpeed)
                {
                    SendMsgCn(DEBUG_SVO_SPD_MSG,1,0x331123,svoVar_svoTargetSpeed); //CurrentSpeed not match with Targetspeed,TargetSpeed=
                    StartTimer(&svoVar_TimeCodeCheckTimer);
                }
                svoCmdVar.svoCommand = CNOP;
                return(SEQ_EX0);
            }
        }
        svoCmdVar.svoCommand = CNOP;
        set_SIF(EXECUTIONSTATE,svoError);       /* set ExecutionState "svoError" */
        return(SEQ_EX1);
    }

    if(get_SIF(EXECUTIONSTATE) == svoBusy)
    {
        svoIramVar.knlTimeupCountFlag = FALSE;
        set_SIF(EXECUTIONSTATE,svoReady);       /* set ExecutionState "svoReady" */
    }
    return(SEQ_EX1);
}

/*-----------------------------------------------------------------------------
 *
 * Module: state_fine_start()
 * Description:fine search start
 *
 *----------------------------------------------------------------------------*/
seq_exit_t state_fine_start(void)
{
    BYTE       jump_ok;
    eJumpType  jump_type;

    if ((svoVar_abort_pause_if_applyable == TRUE) && ((svoIramVar.svoInState & TRN_MASK) == SVO_PAUSE))
    {
        return (SEQ_EX0);
    }
    else
    {
        if (svoSeekVar.rx_available == FALSE)
        {
            jump_type = eFakeRX;
        }
        else
        {
            jump_type = eRealRX;
        }

        jump_ok = track_jump(jump_type,svoSeekVar.jumpType & DIR_MASK,svoSeekVar.jumpDistance);

        if (jump_ok)
        {
            svoSeekErrCnt = 0;

            return(SEQ_EX0);
        }
        else
        {
            return(SEQ_EX1);
        }
    }
}

/*-----------------------------------------------------------------------------
 *
 * Module: state_fine_err()
 * Description:
 *
 *----------------------------------------------------------------------------*/
seq_exit_t state_fine_err(void)
{
    HAL_SERVO_CenterControlON(FORW);

    wait_track_on(TRK_ON_WAIT_SLOW_TIMEOUT);

    /* Use DC filter for focus/tracking error signal when detecting defects */
    Defect_Enable(DEFECT_STATE_ALL_ON|DEFECT_SOURCE_DFE_RF);

#if DO_HW_SUPPORT_STEP == 1
    hw_step_motor_normal_play();
#else /* DO_HW_SUPPORT_STEP */
    svoStp_SetTimerMode( STEP_READ_TIMER );
#endif /* DO_HW_SUPPORT_STEP */

    if (svoSeekErrCnt < 4)
    {
        svoSeekErrCnt++;
        SetDfeWblFrontSemiSynchr();
        return(SEQ_EX0);
    }
    else
    {
        SetDfeWblFrontOnHold();
        return(SEQ_EX1);
    }
}

/*-----------------------------------------------------------------------------
 *
 * Module: state_err_trksvo_on()
 * Description:
 *
 *----------------------------------------------------------------------------*/
seq_exit_t state_err_trksvo_on(void)
{
    wait_track_on(TRK_ON_WAIT_SLOW_TIMEOUT);

    SetDfeWblFrontSemiSynchr();
    //Delay100uS(1);
    //SetDfeWblFrontBitSynchr();

    //TON_ON_OFF(ON);
    SetTimeout(0, TIMER_STOP);
    SetTimeout(1, TIMER_STOP);
#if(FDP_TRKSLIP_MONITOR == 0)
    if(ARG3 == 0)
    {
        svoCmdVar.svoCommand = CNOP;
        set_SIF(EXECUTIONSTATE, svoError);  /* set ExecutionState "svoError" */
        svoCmdVar.playerError = eErrorAccess;
        set_SIF(STATE, svoStTracking);      /* set state "svoStTracking" */
    }
    /* Use DC filter for focus/tracking error signal when detecting defects */
    Defect_Enable(DEFECT_STATE_ALL_ON|DEFECT_SOURCE_DFE_RF);

    return(SEQ_EX0);
#else
    if ((svoMiscVar.trackSlipCount >= FORCE_TRK_OFF_TH) || (svoIramVar.knlTimeupCount == 0))
    {
        svoMiscVar.forceTrackOffFlag = TRUE;
        svoMiscVar.trackSlipCount = 0;
        return(SEQ_EX2);
    }
    else
    {
        if(ARG3 == 0)
        {
            svoCmdVar.svoCommand = CNOP;
            set_SIF(EXECUTIONSTATE, svoError);  /* set ExecutionState "svoError" */
            svoCmdVar.playerError = eErrorAccess;
            set_SIF(STATE, svoStTracking);      /* set state "svoStTracking" */
        }
        /* Use DC filter for focus/tracking error signal when detecting defects */
        Defect_Enable(DEFECT_STATE_ALL_ON|DEFECT_SOURCE_DFE_RF);

        return(SEQ_EX0);
    }
#endif

}

/*-----------------------------------------------------------------------------
 *
 * Module: state_jmpcnt_set()
 * Description:
 *
 *----------------------------------------------------------------------------*/
seq_exit_t state_jmpcnt_set(void)
{
    if(svoCmdVar.commandedTrksToGo < FINE_THRESHOLD_CD)
    {
        svoSeekVar.jumpDistance = (LONG)svoCmdVar.commandedTrksToGo;
    }
    else
    {
        svoSeekVar.jumpDistance = FINE_THRESHOLD_CD;
    }
    return(SEQ_EX0);
}

/*-----------------------------------------------------------------------------
 *
 * Module: state_jmpcnt_chk()
 * Description:
 *
 *----------------------------------------------------------------------------*/
seq_exit_t state_jmpcnt_chk(void)
{
    svoCmdVar.commandedTrksToGo -= (short)svoSeekVar.jumpDistance;
    if(svoCmdVar.commandedTrksToGo == 0)
    {
        SetTimeout(0, TIMER_STOP);
        return(SEQ_EX0);
    }
    SetDelayTimer(ARG1);
    return(SEQ_EX1);
}

/*-----------------------------------------------------------------------------
 *
 * Module: state_jump_init()
 * Description:
 *
 *----------------------------------------------------------------------------*/
seq_exit_t state_jump_init(void)
{
    SetTimeout(0, ARG2);
    return(SEQ_EX0);
}

/*-----------------------------------------------------------------------------
 *
 * Module: state_seek_init()
 * Description:
 *
 *----------------------------------------------------------------------------*/
#if (SERVO_SEEK_TEST == 1)
BYTE seektimedir = 1;
#endif

seq_exit_t state_seek_init(void)
{
    SeekBusy = TRUE;

#if (SERVO_SEEK_TEST == 1)
    svoSeekVar.svoTargetID.Layer = 0;
    if(seektimedir != 0)
    {
        svoSeekVar.svoTargetID.SectorNum.all32 = 0x00040000;
        seektimedir = 0;
    }
    else
    {
        svoSeekVar.svoTargetID.SectorNum.all32 = 0x00210000;
        seektimedir = 1;
    }
#endif //SERVO_SEEK_TEST
    SendMsgCn(DEBUG_SEEK_MSG,6,0x334000,svoIramVar.currentID.Layer,A4B(svoSeekVar.svoTargetID.SectorNum.all32),
        A1B(svoIramVar.svoCurrentSpeed));
    if (fgseektime ==0)
    {
        StartTimer(&seektime);
        fgseektime = 1;
    }

#if ((BD_ENABLE == 1) && (COLLI_SWITCH == 1) && (COLLI_REST_TIME > 0))
    if ((svoGetIDErrCount > 4) || (ReadTimer(&seektime) > (((svoVar_discKindT.Bit.Layers == eLayersDouble) && (svoVar_discKindT.Bit.DiscStd == eDiscBD))? (MS_1000 + COLLI_REST_TIME) : MS_1000)))
#else
    if ((svoGetIDErrCount > 4) || (ReadTimer(&seektime) > MS_1000))
#endif
    {
        SendMsg80(SHOW_DEBUG_MSG,0x33400E); // WARNING: Seek Timout
        svoGetIDErrCount = 0;
        fgseektime = 0;
        SetDfeWblFrontOnHold();
        return(SEQ_EX1);
    }

    switch (svoVar_discKindT.Bit.DiscStd)
    {
        case eDiscCD:
            if (svoCmdVar.cmdParam[4] != eWriteSeek)
            {
                svoIramVar.subQEnableFlag |= SUBQ_EI_DI_SERVO;
            }
            break;

        case eDiscDVD:
            // reset CIDI: Current ID Ready Interrupt Flag
            CLEAR_INT_FIELD(HAL_DVDD_CIDI, 0);
#if LPP_AUTO_LEARN_TEST == 1
            if (svoVar_discKindT.Bit.WblType == eWblMinus)
            {
                WRITE_REG(HAL_DVDDREG_MAP->MEDCTRL,HAL_DVDD_ENDVDD);
            }
#endif // LPP_AUTO_LEARN_TEST == 1
            break;
#if (ENABLE_HDDVD == 1)
        case eDiscHD:
            // reset CIDI: Current ID Ready Interrupt Flag
            CLEAR_INT_FIELD(HAL_DVDD_CIDI, 0);
            break;
#endif // (ENABLE_HDDVD == 1)
#if (BD_ENABLE == 1)
        case eDiscBD:
            //Clear sub data buffer by reading the AUNFLGBT field
            (void)READ_FIELD(HAL_BDC_AUNFLGBT);
            break;
#endif // (BD_ENABLE == 1)
    }
    seekSetSeekParas();
#if (XDFCT_ENABLE == 1)
    LAL_DSP_XDFCT_Enable(lalDSP_Disable,0);
#endif //(XDFCT_ENABLE == 1)
#if (DVD_RAM_READ == 1)
    if (DVDRAM)
    {
        if (svoSeekVar.svoTargetID.SectorNum.all32s > 0x30000)
            target_zone = DVDRAM_PSN2ZONE(svoSeekVar.svoTargetID.SectorNum.all32s);
        else
            target_zone = 0xff;  // Set zone number to -1 if in embossed area.
    }
#endif

#if (BD_ENABLE == 1)
    if ((svo_BDR_RE_PICReading == SET) && (bdDecFlag.fREADING_PIC == CLEAR))
        return(SEQ_EX2);
#if (BD_ECC_IMPROVEMENT==1)
    else if((svoVar_eccDiscNum >= eECC_140_um) && (svoVar_discKindT.Bit.DiscStd == eDiscBD))
        return(SEQ_EX3);
#endif
    else
#endif // (BD_ENABLE == 1)
        return(SEQ_EX0);
}

/*-----------------------------------------------------------------------------
 *
 * Module: state_pause_start()
 * Description:
 *
 *----------------------------------------------------------------------------*/
seq_exit_t state_pause_start(void)
{

 #if 1  //[W0K] Shin
   AfeMUXOUT(AFE_MUXOUT1, halAFE_MUX1SEL_TE, 0);
   AfeMUXOUT(AFE_MUXOUT2, halAFE_MUX2SEL_FE, 0);
 #endif


     #if 1  //[W0K] Shin 
      WRITE_FIELD(HAL_AFE_EQDOSEL, halAFE_EQDOSEL6);
      WRITE_FIELD(HAL_AFE_RFANATSEL, halAFE_RFANATSEL3);
      WRITE_FIELD(HAL_AFE_RFADCTEN, 0);
      WRITE_FIELD(HAL_AFE_PDEQBUF, 0);//power  EQDOP/N buffer
     #endif


  #if 0   //[S0A] Shin debug
      //HAL_debug_MIO0(DFE_blankdefect0_blackdot_detected); //MIO 0~3
      HAL_debug_MIO0(DFE_blankdefect0_early_blackdot_detected); //MIO 0~3
  #endif

  

    if (dvdDecFlag.fREADING_LEADIN == TRUE)
    {
        return(SEQ_EX2);
    }
#if (FDP_TRKSLIP_MONITOR == 1)
    if ((svoMiscVar.forceTrackOffFlag) || (svoMiscVar.forceFocusOffFlag))
    {
        return(SEQ_EX3);
    }
#endif
    if(svoGetIDErrCount > 6)
    {
        svoGetIDErrCount = 0;
        SetDfeWblFrontOnHold();
        return(SEQ_EX1);
    }

    //SetDfeWblFrontSemiSynchrAfterTracking(); // expected that we are already in lock

    set_SIF( EXECUTIONSTATE, svoReady );        /* set ExecutionState "svoReady" */
    set_SIF( STATE, svoStPausing );             /* set state "svoStPausing" */
    SetTimeout( 0, TIMER_STOP );

    return(SEQ_EX0);
}

/*-----------------------------------------------------------------------------
 *
 * Module: state_pause_time()
 * Description:
 *
 *----------------------------------------------------------------------------*/
seq_exit_t state_pause_time(void)
{
    LONG  Current_pba;

    if ((svoIramVar.svoInState & TRN_MASK) == SVO_PAUSEINI)
    {
        if (svoVar_abort_pause_if_applyable == TRUE)
            return(SEQ_EX0);
    }
    Current_pba = svoIramVar.currentID.SectorNum.all32s - (PAUSE_DISTANCE * 32) * Iram_stp_CurrentPosition / svoSeekVar.parFine;

    switch (svoVar_discKindT.Bit.DiscStd)
    {
        case eDiscCD:
            if(Current_pba < 150L)
            {
                if((svoVar_discKindT.fDiscKind == eCDROMDisc)||(svoIramVar.svoCurrentSpeed > CLV_SPEED_CD))
                {
                    Current_pba = 150L;
                }
            }
            break;

        case eDiscDVD:
            if(Current_pba < 0x2F000)
            {
                Current_pba = 0x2F000;
            }
            break;
#if (BD_ENABLE == 1)
        case eDiscBD:
            if(Current_pba < 0xF0000)
            {
                Current_pba = 0xF0000;
            }
            break;
#endif // (BD_ENABLE == 1)
#if (ENABLE_HDDVD == 1)
        case eDiscHD:
            if(Current_pba < HD_REFERENCE_CODE_ZONE_START_ADDR)
            {
                Current_pba = HD_REFERENCE_CODE_ZONE_START_ADDR;
            }
            break;
#endif // (ENABLE_HDDVD == 1)
    }


#if (DVD_RAM_READ == 1)
    if (DVDRAM_rewritable)
    {
        const StructureOfGroupsType *pTargetZone;
        ULONG                       PauseLimitPSN;

        pTargetZone  = &DVDRAMZoneTable[DVDRAM_PSN2ZONE(Current_pba)];
        PauseLimitPSN = pTargetZone->EndUserSectorPSN - pTargetZone->SectorsPerRev*34;
        if (Current_pba > PauseLimitPSN)
        {
            Current_pba = PauseLimitPSN;
        }
    }
#endif
    SendMsgCn(SHOW_DEBUG_MSG,4,0x331119,A4B(Current_pba));
    svoSeekVar.svoTargetID.SectorNum.all32s = Current_pba;
    svoSeekVar.advance = 0;
    svoSeekVar.targetRadius = svoPBA2Radius(Current_pba);
    seekSetSeekParas();

    return(SEQ_EX0);
}

/*-----------------------------------------------------------------------------
 *
 * Module: state_pause_init()
 * Description:
 *
 *----------------------------------------------------------------------------*/
seq_exit_t state_pause_init(void)
{
    //send_msg80(DEBUG_SEEK_MSG,0x534502);  // ***STATE_PAUSE_INIT***

    if (dvdDecFlag.fREADING_LEADIN == TRUE)
    {
        return (SEQ_EX1);
    }
    if(svoGetIDErrCount > 0)
        svoGetIDErrCount -= 1;

    svoSeekVar.roughCount = 1;
    svoSeekVar.fineCount = 0;

    return(SEQ_EX0);
}

#if (BCA_ENABLE == 1)
/*-----------------------------------------------------------------------------
 *
 * Module: state_BCA_jump_init()
 * Description:
 *
 *----------------------------------------------------------------------------*/
seq_exit_t state_BCA_jump_init(void)
{
    SHORT sled_target;//improve BCA jump (handle focus off during jump)

    if (++svoSeekVar.roughCount > 3)
    {
        return(SEQ_EX1);
    }

    sled_target = ((svoCmdVar.cmdParam[0]<<8)|svoCmdVar.cmdParam[1]);

    if (sled_target < Iram_stp_CurrentPosition)
    {
        svoSeekVar.jumpDistance = Iram_stp_CurrentPosition - sled_target;
        svoSeekVar.jumpType = BACK;
    }
    else
    {
        svoSeekVar.jumpDistance = sled_target - Iram_stp_CurrentPosition;
        svoSeekVar.jumpType = FORW;
    }

    if (svoCmdVar.cmdParam[2] == OFF)
    {
        HAL_DFE_EnDisAgcAocHold(halDFE_AgcAocHoldDefectDisable);
        svoSeekVar.jumpType |= BCA_J_MASK;
    }
    else
    {
        HAL_DFE_EnDisAgcAocHold(halDFE_AgcAocHoldDefectEnable);
        svoSeekVar.jumpType |= R_J_MASK;
    }

    return(SEQ_EX0);
}
#endif

/*-----------------------------------------------------------------------------
 *
 * Module: state_rough_start()
 * Description:
 *
 *----------------------------------------------------------------------------*/
seq_exit_t state_rough_start(void)
{
    SHORT jump_steps;
#if DO_HW_SUPPORT_STEP == 1
    BYTE  tp;
#endif //DO_HW_SUPPORT_STEP

#if (ACTIVE_SLED_FOR_FINE_JUMP == 1)
    svoStp_SetTimerMode( STEP_DISABLE_TIMER );
#endif // (ACTIVE_SLED_FOR_FINE_JUMP == 1)

#if (TRACKOFF_SEEK_USE_BS_DEFECT == 1)
    Defect_Enable(DEFECT_STATE_ONLY_FOCUS_ON|DEFECT_SOURCE_SERVO_BS);
#else
    Defect_Enable(DEFECT_STATE_ONLY_FOCUS_ON);
#endif

#if ((BD_2X_STARTUP == 1)&&(BD_UNBALANCE_DISC_CHECK))
    if((svoVar_unbalanceDiscNum != 0)&&(svoVar_discKindT.Bit.DiscStd == eDiscBD)
       &&(svoIramVar.svoCurrentSpeed == _2LX_BD))
       spdHoldRPM( svoSpeedVar.CurrentRpm*0.7);
#endif

    /*Hold data slicer, agc, and disable defect action in seek*/
    SetDfeWblFrontOnHold();

    if ((svoSeekVar.jumpType & DIR_MASK) == FORW)
    {
        jump_steps = svoSeekVar.jumpDistance;
    }
    else
    {
        jump_steps = -svoSeekVar.jumpDistance;
    }

    track_off();

#if DO_HW_SUPPORT_STEP == 1
    //---------------------------------------------------------------------------------------------;
    // call seeking FW which use H/W (emulated by F/W) step motor support block                    ;
    //---------------------------------------------------------------------------------------------;
    if (StepMotorSupport_seek(jump_steps, 3400) == TRUE)
    {
        return(SEQ_EX0);
    }
    else
    {
        return(SEQ_EX0);
    }
#else // DO_HW_SUPPORT_STEP == 1
    if ((svoSeekVar.jumpType & BCA_J_MASK) == 0)
    {
    #if (RP_AUTO_ADJUST == 1)
        if ((svoVar_discKindT.Bit.DiscStd == eDiscBD) && (BDRDLDISC || BDREDLDISC) && ((RPOfsAdjust & RP_ADJ_R_MASK) != RP_ADJ_R_MASK))
        {
            RPOfsAdjust |= RP_ADJ_R_MASK;
        }
    #endif
        svoStp_JumpSteps(STEP_NORMAL_MODE, jump_steps);
    }
    else
    {
        svoStp_JumpSteps(STEP_SAFE_SLOW_MODE, jump_steps);
    }

    #if (DVD_RAM_READ == 1)
    if (DVDRAM_rewritable)
    {
        //
        //Disable header hold function during rough seek because HEADERHLD is not correct.
        //And disabling TE-header-hold function can prevent SLOW flag from being not reliable for DVDRAM.
        //
        DISABLE_HDR_HOLD_SERVO; //Disable header hold function for servo signals
        WRITE_FIELD(HAL_SRV_DVDRHHLD, 0); //Disable header hold function for focus and tracking
    }
    #endif

    return(SEQ_EX0);
#endif // DO_HW_SUPPORT_STEP == 1
}

/*-----------------------------------------------------------------------------
 *
 * Module: state_rough_end_chk()
 * Description:
 *
 *----------------------------------------------------------------------------*/
seq_exit_t state_rough_end_chk(void)
{
#if DO_HW_SUPPORT_STEP == 1
    return(SEQ_EX0);                    // no need to check. It's alway ready. just return SEQ_EX0
#else // DO_HW_SUPPORT_STEP == 1
    if (svoIramVar.svoPreInState != svoIramVar.svoInState)
    {
        SetTimeout(0,ARG1);
    }

    if (!svoStp_busy())
    {
    #if (BCA_ENABLE == 1)
        if ((svoSeekVar.jumpType & BCA_J_MASK) == BCA_J_MASK)
        {
            spdDiscSpeedWaitStable(10);
            return (SEQ_EX2);
        }
    #endif

        return(SEQ_EX0);
    }

    if (timeout_chk(0) == eTimerTimeout)
    {
        SetDfeWblFrontOnHold();
        ClearAddressRead();
        SendMsg80(DEBUG_SEEK_MSG,0x334003);   //***Rough_Seek_Timeout1***//
    #if (RP_AUTO_ADJUST == 1)
        if ((BDRDLDISC || BDREDLDISC) && ((RPOfsAdjust&RP_ADJ_R_MASK) == RP_ADJ_R_MASK))
        {
            RPOfsAdjust &= (~RP_ADJ_R_MASK);
        }
    #endif
        return(SEQ_EX1);
    }

    SetDelayTimer(MS_1);
    return(SEQ_BUSY);
#endif // DO_HW_SUPPORT_STEP == 1
}


#if DO_HW_SUPPORT_STEP == 1
  #define SLOW_WAIT_CNT        (0x10000000)
#endif /* DO_HW_SUPPORT_STEP */
/*-----------------------------------------------------------------------------
 *
 * Module: state_slow_chk()
 * Description:
 *
 *----------------------------------------------------------------------------*/
seq_exit_t state_slow_chk(void)
{
#if DO_HW_SUPPORT_STEP == 1
    wait_track_on(TRK_ON_WAIT_SLOW_TIMEOUT);
    return(SEQ_EX0);
#else //DO_HW_SUPPORT_STEP == 1
    #if ((BD_ENABLE == 1) || (ENABLE_HDDVD == 1) || (DVD_RAM_READ == 1))
    errcode_t ret;
    USHORT    te_target;
    BYTE      lead_in = NON;
        #if (DVD_RAM_READ == 1)
    discKindT tempDiscKind;
        #endif
    #endif

    #if (BD_ENABLE == 1)
    if ((svoVar_discKindT.Bit.DiscStd == eDiscBD) && (svo_BDR_RE_PICReading != bdDecFlag.fREADING_PIC) &&
        ((svoVar_discKindT.Bit.Media == eMediaR) || (svoVar_discKindT.Bit.Media == eMediaRW)) && (svoSeekVar.jumpType != L_J_MASK))
    {
        //Exiting the PIC zone
        if ((bdDecFlag.fREADING_PIC == FALSE) && (svoSeekVar.svoTargetID.SectorNum.all32s >= PIC_L0_MAX_ADDR))
        {
            SendMsg80(SHOW_DEBUG_MSG,0x334700);
            svo_BDR_RE_PICReading = CLEAR;
            LAL_PLL_SetPll();
            LAL_SetResetPIC(eRESET_PIC, svo_bReadZoneSpeedX10);
            SetReadFilters();  // already happened in state_rough_end_chk but we now just changed "zone"
            lead_in = OFF;
        }
        // Entering the PIC zone
        else if ((bdDecFlag.fREADING_PIC == TRUE) && (svoSeekVar.svoTargetID.SectorNum.all32s < PIC_L0_MAX_ADDR))
        {
            SendMsg80(SHOW_DEBUG_MSG,0x334701);
            Defect_Enable(DEFECT_SOURCE_FREEZE_BS);
            svo_BDR_RE_PICReading = SET;
            LAL_PLL_SetPll();
            LAL_SetResetPIC(eSET_PIC, svo_bReadZoneSpeedX10);
            SetReadFilters();  // already happened in state_rough_end_chk but we now just changed "zone"
            lead_in = ON;
        }
    }
    #endif //(BD_ENABLE == 1)
    #if (ENABLE_HDDVD == 1)
    if (svoVar_discKindT.Bit.DiscStd == eDiscHD)
    {
        if ((svoSeekVar.svoTargetID.SectorNum.all32 < HD_CONNECTION_ZONE_START_ADDR) && (svoIramVar.currentID.SectorNum.all32s >= HD_CONNECTION_ZONE_START_ADDR))
        {
            lead_in = ON;
        }

        if ((svoSeekVar.svoTargetID.SectorNum.all32 >= HD_CONNECTION_ZONE_START_ADDR) && (svoIramVar.currentID.SectorNum.all32s < HD_CONNECTION_ZONE_START_ADDR))
        {
            lead_in = OFF;
        }
    }
    #endif
    #if (DVD_RAM_READ == 1)
    if (DVDRAM)
    {
        if (svoSeekVar.svoTargetID.SectorNum.all32 > 0x30000)
        {
            if (svoVar_TrackingMethod != TRK_METHOD_DPP)
            {
                afeSetTrackingMethod(TRK_METHOD_DPP);
                tempDiscKind.fDiscKind = eDVDRAMDisc;
                LAL_SetDiscTypeSelection(tempDiscKind);
                set_DVDRAM_rewritable_mode();
        #if (CHIP_REV >= 0xB2)
                LAL_WriteCoef(AFE_FCTS35_CEBG12DB);
        #endif

                // Restore RF AGC & AOC boundary for rewritable area.
                HAL_DFE_SetAgcAocBoundary(COEF_GetValue(DFE_CALC_Coef_Max_dyn_gain),
                                          COEF_GetValue(DFE_CALC_Coef_Min_dyn_gain),
                                          COEF_GetValue(DFE_CALC_Coef_Max_dyn_Offset),
                                          COEF_GetValue(DFE_CALC_Coef_Min_dyn_Offset));
                ENABLE_DVD_DECODER_INTS();
                lead_in = OFF;
            }
        }
        else
        {
            if (svoVar_TrackingMethod != TRK_METHOD_DPD)
            {
                afeSetTrackingMethod(TRK_METHOD_DPD);
                tempDiscKind.fDiscKind = eDVDROMDisc;
                LAL_SetDiscTypeSelection(tempDiscKind);
                set_DVDRAM_embossed_mode();
        #if (CHIP_REV >= 0xB2)
                LAL_WriteCoef(AFE_FCTS35_CEBG12DB_EMBOSSED);
        #endif
                // Re-set RF AGC & AOC boundary for embossed area.
                HAL_DFE_SetAgcAocBoundary(COEF_GetValue(DFE_CALC_Coef_Max_dyn_gain)+0x6,
                                          COEF_GetValue(DFE_CALC_Coef_Min_dyn_gain)+0x6,
                                          COEF_GetValue(DFE_CALC_Coef_Max_dyn_Offset)-0x14,
                                          COEF_GetValue(DFE_CALC_Coef_Min_dyn_Offset)+0x40);
                ENABLE_DVD_DECODER_INTS();
                lead_in = ON;
            }
        }
    }
    #endif

    #if (BD_ENABLE == 1) || (ENABLE_HDDVD == 1) || (DVD_RAM_READ == 1)
    if (lead_in == ON)
    {
        if (!CALIBRATE_SYSTEM_LEAD_IN)
        {
            HAL_SERVO_CenterControlOFF();
            WaitTimer(5);

        #if (DVD_RAM_READ == 1)
            if (DVDRAM)
            {
                // Preset a proper TE gain for DVDRAM embossed area
                WRITE_FIELD(HAL_AFE_TRKG,TRKG_EMBOSSED);
                if ((READ_FIELD(HAL_AFE_CESP1) == 1) && (READ_FIELD(HAL_AFE_CEMPP) == 1))
                {
                    LAL_WriteCoef(AFE_INIT_FCTS34_CEOS);
                    LAL_WriteCoef(AFE_FCTS33_CEBG);
                    // CE balance calibration
                    ret = calCalibrateCEBG();
                    if (ret == eNOERR)
                    {
                        SendMsgCnB(DEBUG_CALIB_RESULT_MSG,4,0x3166B3,F4B((float)(last_cal_result - 0x10) / 2));//CEBG = %2.1f (dB)
                        calSetAfeGain(1,CEB_GN,TYPE3,0,0,last_cal_result);  // Borrow space from layer 1.
                    }
                    else
                    {
                        // ==> Calib ERROR!!!! Errcode: %1d, In STAT:%1d, TRN:%1d, SEQ:%1d
                        SendMsgCnB(SHOW_DEBUG_MSG,4,0x3A0001,B1B(ret),B1B(((svoIramVar.svoInState & STAT_MASK) >> 12)),B1B(((svoIramVar.svoInState & TRN_MASK ) >> 5) + 1),B1B((svoIramVar.svoInState & SEQ_MASK) + 1));
                    }
                    // CE offset calibration
                    ret = calCalibrateCEOS();
                    if (ret == eNOERR)
                    {
                        SendMsgCn(DEBUG_CALIB_RESULT_MSG,1,0x310515,A1B(last_cal_result));//<L1> CE_OS = %02Xh
                        calSetAfeOffset(1,CE_OS,TYPE3,0,0,last_cal_result);  // Borrow space from layer 1.
                    }
                    else
                    {
                        // ==> Calib ERROR!!!! Errcode: %1d, In STAT:%1d, TRN:%1d, SEQ:%1d
                        SendMsgCnB(SHOW_DEBUG_MSG,4,0x3A0001,B1B(ret),B1B(((svoIramVar.svoInState & STAT_MASK) >> 12)),B1B(((svoIramVar.svoInState & TRN_MASK ) >> 5) + 1),B1B((svoIramVar.svoInState & SEQ_MASK) + 1));
                    }
                    // RP gain & offset calibration
                    te_target = COEF_GetValue(AFE_CALC_RP_PP_TARGET_10MV) * 10;
                    ret = calCalibrateRPGnOs(te_target);
                    if (ret == eNOERR)
                    {
                        calSetAfeGain(Layer_Index,RP_GN,TYPE3,0,0,last_cal_result);
                        calSetAfeOffset(Layer_Index,RP2_OS,TYPE3,0,0,last_cal_result2);
                        SendMsgCn(DEBUG_CALIB_RESULT_MSG,4,0x310532+Layer_Index,svoCalGnT3Var[Layer_Index][RPB_GN].normal_gain,svoCalGnT3Var[Layer_Index][RP_GN].normal_gain,svoCalOfsT3Var[Layer_Index][RP_OS].normal_offset,svoCalOfsT3Var[Layer_Index][RP2_OS].normal_offset);
                    }
                    else
                    {
                        // ==> Calib ERROR!!!! Errcode: %1d, In STAT:%1d, TRN:%1d, SEQ:%1d
                        SendMsgCnB(SHOW_DEBUG_MSG,4,0x3A0001,B1B(ret),B1B(((svoIramVar.svoInState & STAT_MASK) >> 12)),B1B(((svoIramVar.svoInState & TRN_MASK ) >> 5) + 1),B1B((svoIramVar.svoInState & SEQ_MASK) + 1));
                    }
                }
            }
        #endif
            te_target = COEF_GetValue(AFE_CALC_TE_PP_TARGET_10MV) * 10;
            ret = calCalibrateTEGnOs(te_target);
            HAL_SERVO_CenterControlON(FORW);
        #if (DVD_RAM_READ == 1)
            if (DVDRAM)
            {
#if (DPD_COMPENSATE_OFFSET==1)
                calSetCalResult(Layer_Index, Iram_stp_CurrentPosition, DPD_GNOS, 0x00, CAL_R_DONE);
#endif
            }
        #endif
            calSetAfeGain(Layer_Index, TE_GN, TYPE1, LI_GAIN, 0, last_cal_result);
            calSetAfeOffset(Layer_Index, TE_OS, TYPE1, LI_OFFSET, 0, last_cal_result2);
            CALIBRATE_SYSTEM_LEAD_IN = 1;
            WaitTimer(MS_10);
        }
        else
        {
            WRITE_FIELD(HAL_AFE_TRKG,svoCalGnT1Var[0][TE_GN].li_gain);
            WRITE_FIELD(HAL_AFE_TRKO,svoCalOfsT1Var[0][TE_OS].li_offset);
        #if (DVD_RAM_READ == 1)
            if (DVDRAM)
            {
                WRITE_FIELD(HAL_AFE_CEBG,     svoCalGnT3Var[1][CEB_GN].normal_gain);
                WRITE_FIELD(HAL_AFE_CEOS,     svoCalOfsT3Var[1][CE_OS].normal_offset);
                //After rough seek, OPU's kinetica energy will move lens far away from center position.
                //More delay can cancel it.
                WaitTimer(15L); //Delay 15ms
            }
        #endif
        }
    }
    if (lead_in == OFF)
    {
        WRITE_FIELD(HAL_AFE_BLANKTRKG, svoCalGnT1Var[0][TE_GN].groove_gain[RECDL]);
        WRITE_FIELD(HAL_AFE_TRKG,svoCalGnT1Var[0][TE_GN].groove_gain[RECDH]);
        WRITE_FIELD(HAL_AFE_BLANKTRKO, svoCalOfsT1Var[0][TE_OS].groove_offset[RECDL]);
        WRITE_FIELD(HAL_AFE_TRKO,svoCalOfsT1Var[0][TE_OS].groove_offset[RECDH]);
        #if (DVD_RAM_READ == 1)
        if (DVDRAM)
        {
            WRITE_FIELD(HAL_AFE_CEBG,     svoCalGnT3Var[0][CEB_GN].normal_gain);
            WRITE_FIELD(HAL_AFE_CEOS,     svoCalOfsT3Var[0][CE_OS].normal_offset);
            //After rough seek, OPU's kinetica energy will move lens far away from center position.
            //More delay can cancel it.
            WaitTimer(15L); //Delay 15ms
        }
        #endif
    }
    #endif

#if (OPEN_SESSION==1)
    if (((svoVar_TrackingMethod != TRK_METHOD_DPP)
    #if (WOBBLE_READ_ON_DVD_OR_CD == 0)
        || (svoVar_discKindT.Bit.DiscStd == eDiscCD) || (svoVar_discKindT.Bit.DiscStd == eDiscDVD)
    #endif
        ) && (RFpp_chk() == FALSE))
    {
        HAL_SERVO_CenterControlOFF();
        //Move a little distance after find RF zone and modify search rf flow.
        return(SEQ_EX4);
    }
#endif

#if (ENABLE_LEARN_DEFECT == 1)
    LAL_DSP_SetLearnDefect(lalDSP_ResetOn);
#endif
#if (RP_AUTO_ADJUST == 1)
    if ((BDRDLDISC) || (BDREDLDISC))
    {
        if ((RPOfsAdjust & RP_ADJ_L_MASK) == RP_ADJ_L_MASK)
            RPOfsAdjust &= (~RP_ADJ_L_MASK);
        if ((RPOfsAdjust & RP_ADJ_R_MASK) == RP_ADJ_R_MASK)
            RPOfsAdjust &= (~RP_ADJ_R_MASK);
    }
#endif

    wait_track_on(TRK_ON_WAIT_SLOW_TIMEOUT);

    return(SEQ_EX0);
#endif //DO_HW_SUPPORT_STEP == 1
}

/*-----------------------------------------------------------------------------
 *
 * Module: state_tok_chk()
 * Description: check TE amplitude and TXSPEED to confirm no track crossing
 *              This state can find track slip in 2ms
 *
 *----------------------------------------------------------------------------*/
#define TE_OK_WINDOW     250    //unit:mV
seq_exit_t state_tok_chk(void)
{
    USHORT max_TE,min_TE;//TE_offset;
    BYTE   tok_result;
#if (DVDDL_TILT_CAL_PRO == 1)
    BYTE  cur_zone;
    cur_zone = calRadius2Zone(Iram_stp_CurrentPosition);
#endif

    //from 720
    if (svoVar_discKindT.Bit.DiscStd == eDiscBD)//711 DVD can't use so large TELPF, need check again
        HAL_AFE_SetTELpfBW(e_TELPF_300);

    if (svoVar_TrackingMethod == TRK_METHOD_DPD)
    {
#if (CHIP_REV >= 0xC1)
        WRITE_FIELD(HAL_AFE_DPDCPFC, halAFE_DPDCPFBW3_400kHz);
#else
        WRITE_FIELD(HAL_AFE_DPDCPFC, halAFE_DPDCPFBW_400kHz);
#endif
    }
    //Disable TX deglitch to prevent TEZC to be filted and cause wrong TXSPEED measurment
    if ((svoVar_discKindT.Bit.DiscStd == eDiscDVD) && (!DVDRAM))
    {
        WRITE_FIELD(HAL_SRV_TXRXFILT,0);
    }

    if (svoIramVar.svoPreInState != svoIramVar.svoInState)
    {
        if (SIF.ServoState != svoStPausing)
            SendMsg80(DEBUG_CALIB_ENTRYS_MSG,0x3A012B); // >>> STATE_TOK_CHK <<<

        TXSpdMonitorInit();
        SetTimeout(0,ARG1);
#if (BD_ECC_IMPROVEMENT==1)
        if((svoVar_discKindT.Bit.DiscStd == eDiscBD)&&(svoVar_eccDiscNum >= eECC_140_um)&&(svoVar_discKindT.Bit.Media != eMediaROM))
            SetTimeout(1,SEC_2);
        else
            SetTimeout(1,SEC_1);
#else
        SetTimeout(1,SEC_1);
#endif
    }

    tok_result = Busy;
    //TE_offset = CONVERT_DSPOFS_TO_ADCMV(svoCalVar.teOfsData.all16);//from 720

    if (timeout_chk(0) == eTimerTimeout)
    {
        SendMsg80(SHOW_DEBUG_MSG,0x334005);   // WARNING: TOK Chk Timeout
        svoMiscVar.trackSlipCount ++;//from 720

	#if (Customer_A1 ==1)
		if(svoMiscVar.trackSlipCount>2)
		PositionHomeErrorCount++;
	#endif                
		
		//[W.0R]Walker
                if((svoIramVar.LTHDisc == TRUE)&&(svoVar_discKindT.Bit.Media   == eMediaROM)&&(svoMiscVar.trackSlipCount>2))
		{
			MediaRecovery = TRUE;
			return(SEQ_EX2);
		}
	
		
        tok_result = Error;
    }
    else if (((TXSpdMonitor() == NG) || (svoStepVar.SledCounter >= TRACK_SLIP_DETECT_THRESHOLD))
#if (BD_ENABLE == 1) && (BD_ECC_IMPROVEMENT != 1)
        && ((svoVar_discKindT.Bit.DiscStd != eDiscBD) || ((svoVar_discKindT.Bit.DiscStd == eDiscBD) && (svo_BDR_RE_PICReading != SET)))
#endif
        )
    {
        SendMsgCn(SHOW_DEBUG_MSG,1,0x3F0120,(svoStepVar.SledCounter >= TRACK_SLIP_DETECT_THRESHOLD)? 0x84 : svoSeekVar.jumpType);    // Servo_error_in_Servo,eErrorRadialSkating: 0x%02X
        
        	#if (Customer_A1 ==1)
		if(svoMiscVar.trackSlipCount>3)
		PositionHomeErrorCount++;
	#endif                

        WRITE_FIELD(HAL_SRV_PKBOT,0);
        svoMiscVar.trackSlipCount ++;//from 720
        tok_result = Error;
    }
    else
    {
#if(BD_ECC_IMPROVEMENT == 1)
        if((svoTXSpdVar.monitor_cnt < 10)&&(svoVar_eccDiscNum >=2)&&(svoVar_discKindT.Bit.DiscStd == eDiscBD)&&(svoVar_discKindT.Bit.Media != eMediaROM))
            return (SEQ_BUSY);
#endif
        //Rising edge to reset Peak/Bottom measurement
        WRITE_FIELD(HAL_SRV_PKBOT,0);
        WRITE_FIELD(HAL_SRV_PKBOT,1);

        // The delay time is the period of Peak/Bottom measurement.
        Delay100uS(1);  // 100us. The period of Peak/Bottom measurement.
        WRITE_FIELD(HAL_SRV_PKBOT,0);
        WRITE_REG(HAL_SRVREG_MAP->RGSEL,halSRV_RSEL_TEPEAK);
        max_TE = READ_REG(HAL_SRVREG_MAP->SRCR);
        WRITE_REG(HAL_SRVREG_MAP->RGSEL,halSRV_RSEL_TEBOTTOM);
        min_TE = READ_REG(HAL_SRVREG_MAP->SRCR);
        max_TE = ((max_TE * DSPADC_IN_RANGE) >> DSPADC_BITS) + DSPADC_IN_MIN;
        min_TE = ((min_TE * DSPADC_IN_RANGE) >> DSPADC_BITS) + DSPADC_IN_MIN;

        if ((max_TE < (CONVERT_DSPOFS_TO_ADCMV(svoCalVar.teOfsData.all16) + TE_OK_WINDOW)) &&
            (min_TE > (CONVERT_DSPOFS_TO_ADCMV(svoCalVar.teOfsData.all16) - TE_OK_WINDOW)))
        {
            //Check TE amplitude in TOK windows continuously for 1 times to make sure tracking servo stable.
            tok_result = Ready;
        }
    }

#if (OPEN_SESSION==1)
    if ((tok_result != Busy) && (!DVDRAM))
    {
        if (((svoVar_TrackingMethod != TRK_METHOD_DPP)
    #if (WOBBLE_READ_ON_DVD_OR_CD == 0)
            || (svoVar_discKindT.Bit.DiscStd == eDiscCD) || (svoVar_discKindT.Bit.DiscStd == eDiscDVD)
    #endif
            ) && (RFpp_chk() == FALSE))
        {
            LAL_WriteCoef(AFE_FCTS38_TEFCSFSEL);
            LAL_WriteCoef(AFE_FCTS38_TELPSET);
            track_off();
            return(SEQ_EX4);
        }
    }
#endif

    if (tok_result == Error)
    {
        if ((timeout_chk(1) == eTimerTimeout) && (svoStepVar.SledCounter < TRACK_SLIP_DETECT_THRESHOLD))
        {
            tok_result = Ready;
        }
#if (FDP_TRKSLIP_MONITOR == 1)
        else if (((svoMiscVar.trackSlipCount >= FORCE_TRK_OFF_TH) && (svoMiscVar.forceTrackOffFlag == TRUE)) ||
                 (!svoIramVar.knlTimeupCount)
                )
        {
            LAL_WriteCoef(AFE_FCTS38_TEFCSFSEL);
            LAL_WriteCoef(AFE_FCTS38_TELPSET);
            send_msg80(SHOW_DEBUG_MSG,0x33FF53); //Tok chk timeout
            return(SEQ_EX1);
        }
#endif
        else
        {
            track_slip_protection(FALSE);
            TXSpdMonitorInit();
            ClearAddressRead();
            SetTimeout(0, ARG1);
        }
    }

    if (tok_result == Ready)
    {
        SetTimeout(0, TIMER_STOP);
        SetTimeout(1, TIMER_STOP);

        if (svoVar_TrackingMethod == TRK_METHOD_DPD)
        {
            LAL_WriteCoef(AFE_RF25_DPDCPFC);
        }

        //from 720
        if (((svoIramVar.svoInState & TRN_MASK) == SVO_SEEK) && ((svoSeekVar.jumpType & R_J_MASK) == R_J_MASK)
            && ((svoVar_discKindT.Bit.DiscStd == eDiscDVD)||(svoVar_discKindT.Bit.DiscStd == eDiscCD)))
        {
            // Keep tracking servo on gain up mode to reduce TE oscillation amplitude.
            // It can prevent reverse fine seek action.
            Delay100uS(10); //Delay 1ms
        }

        //clear unconditional hysteresis and gainup
#if (HORROR_DISC_IMPROVEMENT == 1)
        if ((BufferErrorForRetry&TRACK_PULL_BACK == TRACK_PULL_BACK) && (svoVar_discKindT.Bit.Media == eMediaROM))
        {
            WRITE_REG(HAL_SRVREG_MAP->FVKICM, HAL_SRV_FVKHYSC );    /* set continuous hysteresis */
        }
        else
#endif
        {
            WRITE_REG(HAL_SRVREG_MAP->FVKICM, 0x00);
        }
        WRITE_REG(HAL_SRVREG_MAP->FVKICK, 0x00);

        if (svoSpeedVar.SpdPresetNeeded == TRUE)
        {
            SetReadFilters();
        }

        SetDfeWblFrontSemiSynchr();

        //TON_ON_OFF(ON);//from 720

#if ((BD_2X_STARTUP==1)&&(BD_UNBALANCE_DISC_CHECK==1))
        if((svoVar_unbalanceDiscNum != 0)&&(svoVar_discKindT.Bit.DiscStd == eDiscBD)
            &&(svoIramVar.svoCurrentSpeed == _2LX_BD))
        {
            if((svoSpeedVar.SpdSetMaxRPM == TRUE)&&(St_drv_ready == 1))
                spdSetMaxRPM(0);
            if((svoSpeedVar.SpdHold == TRUE))
            spdUnhold();
         }
#endif
#if DO_HW_SUPPORT_STEP == 1
        hw_step_motor_normal_play();
#else
        svoStp_SetTimerMode( STEP_READ_TIMER );
#endif

#if (SRC_ENABLE == 1)
        HAL_DSP_SRCHold(SRC_LOOP_TRACKING, ON, ON, OFF);
#endif // (SRC_ENABLE == 1)
#if (FDP_TRKSLIP_MONITOR == 1)
        if (svoMiscVar.trackSlipCount)
        {
            svoMiscVar.trackSlipCount--;
        }
#endif
#if (BD_ECC_IMPROVEMENT == 1)
        if (svoSpeedVar.SpdHold)
            spdUnhold();
#endif

        //from 720
        if (svoVar_TrackingMethod == TRK_METHOD_DPD)
        {
            WRITE_FIELD(HAL_AFE_RXTOK, 0); //Enable XTOK mute RX
        }
        WRITE_FIELD(HAL_AFE_TOKLVL, TOKLVL_TRACK_ON);
        LAL_WriteCoef(AFE_FCTS38_TEFCSFSEL);
        LAL_WriteCoef(AFE_FCTS38_TELPSET);

        if ((svoVar_discKindT.Bit.DiscStd == eDiscDVD) && (!DVDRAM))
        {
            if (svoSeekVar.rx_available == FALSE)
            {
                READ_MOD_WRITE(HAL_SRVREG_MAP->SREG2,(HAL_SRV_GUP1|HAL_SRV_HYS1|HAL_SRV_HYS2|HAL_SRV_GUP2),0);
            }
            else
            {
                READ_MOD_WRITE(HAL_SRVREG_MAP->SREG2,(HAL_SRV_GUP1|HAL_SRV_HYS1|HAL_SRV_HYS2|HAL_SRV_GUP2),(HAL_SRV_HYS1|HAL_SRV_GUP1));
            }
        }
        return (SEQ_EX0);
    }

    return (SEQ_BUSY);
}

/*-----------------------------------------------------------------------------
 *
 * Module: state_rough_err()
 * Description:
 *
 *----------------------------------------------------------------------------*/
seq_exit_t state_rough_err(void)
{
    SledStop();

    return(SEQ_EX0);
}

/*-----------------------------------------------------------------------------
 *
 * Module: state_seek_end()
 * Description:
 *
 *----------------------------------------------------------------------------*/
seq_exit_t state_seek_end(void)
{
    if (svoCmdVar.cmdParam[4] == eWriteSeek)
    {
        svoCmdVar.cmdParam[4] = 0xFF; //Set to unknown eSeekMode
        return(SEQ_EX1);
    }
    else
    {
        svoCmdVar.cmdParam[4] = 0xFF;
        return(SEQ_EX0);
    }
}

/*-----------------------------------------------------------------------------
 *
 * Module: state_disc_speed()
 * Description:
 *
 *----------------------------------------------------------------------------*/
seq_exit_t state_disc_speed(void)
{
    BYTE    zone_speed;

    if ((svoIramVar.svoInState & TRN_MASK) == SVO_PAUSE)
    {
        if (svoVar_abort_pause_if_applyable == TRUE)
        {
            return(SEQ_EX0);
        }
    }

    zone_speed = get_zone_speed();

    SendMsgCn(DEBUG_WBL_MSG,1,0x410001,A1B(zone_speed));//zone speed=

    if ((zone_speed != svoSpeedVar.SpdCurrentSpeed) || (bSetSpeedAgain == TRUE))
    {
        SendMsg80(DEBUG_WBL_MSG,0x4100FE);//Current Speed not match with Target speed
        spdChangeDiscSpeed(zone_speed);        /* Set servo EQ */
        return(SEQ_EX1);
    }//if(zone_speed != svoIramVar.svoCurrentSpeed)

    return(SEQ_EX0);
}

/*-----------------------------------------------------------------------------
 *
 * Module: state_calib_needed()
 * Description: return SEQ_EX0 the calibration is needed
 *              ARG1:COLLI_POS, TILT_DAT, FEBC_DSP
 *              ARG2:JITTER_CAL, TE_P2P_CAL, RF_P2P_CAL
 *
 *----------------------------------------------------------------------------*/
seq_exit_t state_calib_needed(void)
{
    if (calGetCalRequired(Layer_Index, Iram_stp_CurrentPosition, ARG1, ARG2) == TRUE)
    {
        return SEQ_EX0;
    }
    else
    {
        return SEQ_EX1;
    }
}

#if (DVD_RAM_READ == 1) || (ENABLE_DVDRAM_DETECTION == 1)
/*-----------------------------------------------------------------------------
 *
 * Module: state_dvdram_pp_det()
 * Description: Detect if a DVD-RAM disc is loaded.
 *
 *----------------------------------------------------------------------------*/
seq_exit_t state_dvdram_pp_det()
{
    float    pp_ratio;
    ad_dsp_t te_signal;
    ETrackingMethod orig_TrackingMethod;
    BYTE     retry, orig_TELpfBw;

    if (((svoCalVar.discChkDoneBits & blank_chk_end) == 0) && (svoVar_discKindT.Bit.DiscStd == eDiscDVD) && (svoVar_discKindT.Bit.Layers == eLayersSingle) && (svoVar_discKindT.Bit.Media == eMediaRW))
    {
        HAL_SERVO_CenterControlOFF();
        orig_TrackingMethod = svoVar_TrackingMethod;
        orig_TELpfBw = HAL_AFE_GetTELpfBW();
        afeSetTrackingMethod(TRK_METHOD_DPP);
        HAL_AFE_SetTELpfBW(e_TELPF_50);

        retry = 0;
        while (retry < 3)
        {
            // Move sledge to 'STEP_HOME_POSITION_DVDRAM' for DVD-RAM detection.
            svoStp_JumpToPosition(STEP_NO_CE_SLOW_MODE, STEP_HOME_POSITION_DVDRAM);

            while (svoStp_busy())
            {
                ExitProcess();
            }
            WaitTimer(20);

            te_signal = ReadFilteredDSPADCmV(ONE_REVOLUTION, TE_ALL, ADC_BS, TRUE);

            if (te_signal.errcode == eNOERR)
                break;

#if(ERROR_CAL_START_UP_MONITOR ==1)
            svoMiscVar.calibrationErrorCount++;
#endif
            SendMsgCnB(SHOW_DEBUG_MSG,4,0x3A0001,B1B(te_signal.errcode),B1B(((svoIramVar.svoInState & STAT_MASK) >> 12)),B1B(((svoIramVar.svoInState & TRN_MASK ) >> 5) + 1),B1B((svoIramVar.svoInState & SEQ_MASK) + 1));
            if (++retry > 3)
            {
                afeSetTrackingMethod(orig_TrackingMethod);
                HAL_AFE_SetTELpfBW(orig_TELpfBw);
                return (SEQ_EX0);
            }
        }

        afeSetTrackingMethod(orig_TrackingMethod);
        HAL_AFE_SetTELpfBW(orig_TELpfBw);

        pp_ratio = (te_signal.top_mv - CONVERT_DSPOFS_TO_ADCMV(svoCalVar.bsOfsData.all16)) * CalculateRateFromXdB(ONEdB, 0x10 - READ_FIELD(HAL_AFE_BSKG));
        SendMsgCn(DEBUG_DISK_ID_MSG,6,0x310055,A2B(te_signal.p2p_mv),A4B(pp_ratio));

        // Calculate the ratio of PP to BS.
        pp_ratio = (float)te_signal.p2p_mv / pp_ratio;
        SendMsgCn(SHOW_DEBUG_MSG,4,0x310058,A4B(pp_ratio));

        if (pp_ratio > DVDRAM_TH)
        {
            // DVD-RAM is detected!
            SendMsg80(SHOW_DEBUG_MSG,0x3100DA); //PP_det==>DVD-RAM Disc
            svoVar_discKindT.fDiscKind = eDVDRAMDisc;
            svoStepVar.StepHomePos = STEP_HOME_POSITION_DVDRAM;
        #if (DVD_RAM_READ == 1)
            set_DVDRAM_mode();
            return (SEQ_EX1);
        #else
            #if (DISC_MIS_JUDGE_START_UP_RECOVERY)
            RetryDiscLaser = eDiscUnknown;
            #endif
            return (SEQ_EX2);
        #endif
        }

        // Not a DVD-RAM disc.
        svoStp_JumpToPosition(STEP_SAFE_SLOW_MODE, Recorded_stp_Position[Layer_Index]);

        while (svoStp_busy())
        {
            ExitProcess();
        }
        WaitTimer(10);
    }

    return (SEQ_EX0);
}
#endif // #if (DVD_RAM_READ == 1) || (ENABLE_DVDRAM_DETECTION == 1)


/*-----------------------------------------------------------------------------
 *
 * Module: state_written_chk()
 * Description: return SEQ_EX0 if HF present for 1 revolution, otherwise return Error
 *
 *----------------------------------------------------------------------------*/
seq_exit_t state_written_chk(void)
{
    BYTE i,blank_count;
    seq_exit_t ret_seq;
#if (NEW_SERVO_CALIB_FLOW == 0)
    SHORT retry_distance;
#endif

    ret_seq = SEQ_EX0;

#if (NEW_SERVO_CALIB_FLOW == 1)
    if ((svoIramVar.svoInState & TRN_MASK) == TRN_031)
    {
        if (svoRRFVar.modulation[Layer_Index] == 0)
        {
            ret_seq = SEQ_EX1;
        }
    }
    else
#endif
    if ((svoVar_discKindT.Bit.Media != eMediaROM) || (svoVar_discKindT.Bit.DiscStd == eDiscDVD))
    {
        blank_count = 0;
#if (NEW_SERVO_CALIB_FLOW == 0)
        if ((svoIramVar.svoInState & TRN_MASK)== SVO_TGAFE)
        {   //Tracking off, measure for one revolution
    #if (CHIP_REV < 0xC0)
            svoSpeedVar.fgCount = (DISK_NFGPLS * (DISK_FR2X + 1) / 2);
    #else
            svoSpeedVar.fgCount = DISK_NFGPLS;
    #endif
            StartFGTimer();
            while (svoSpeedVar.fgCount)
            {
                if (HAL_GLOBAL_GetRfPresent() == 0)
                {
                    if (++blank_count > FEBC_TILT_RECD_CHK_TH)
                    {
                        if ((svoVar_TrackingMethod == TRK_METHOD_DPD) && (++cal_retry < 10))
                        {
                            // Move sled out and in
                            if (cal_retry & 0x01)
                            {
                                retry_distance = -UM_2_SLED_STEP(200* (cal_retry + 3) / 2);
                            }
                            else
                            {
                                retry_distance = (UM_2_SLED_STEP(100+200 * (cal_retry+2) / 2));
                            }
                            SendMsgCn(DEBUG_SEEK_MSG,2,0x3FAA3A,retry_distance); //Written Check (move_sled %d)
                            svoStp_JumpSteps(STEP_SAFE_SLOW_MODE, retry_distance);

                            while (svoStp_busy())
                            {
                                ExitProcess();
                            }
                            WaitTimer(20);
                            HAL_SERVO_CenterControlOFF();
                            return (SEQ_BUSY);
                        }
                        ret_seq = SEQ_EX1;
                        break;
                    }
                }
                WaitTimer(1);
            }
            cal_retry = 0;
        }
        else
#endif
        {   //Tracking on, measure 10 samples.
            for (i = 0; i < FEBC_TILT_RECD_CHK_CNT; i++)
            {
                if (HAL_GLOBAL_GetRfPresent() == 0)
                {
                    if (++blank_count > FEBC_TILT_RECD_CHK_TH)
                    {
                        ret_seq = SEQ_EX1;
                        break;
                    }
                }
            }
        }
    }

    if (ret_seq == SEQ_EX1)
    {
        SendMsg80(DEBUG_DISK_ID_MSG,0x31003D);
    }
    else
    {
        SendMsg80(DEBUG_DISK_ID_MSG,0x31003E);
#if (DVD_RAM_READ == 1) && (NEW_SERVO_CALIB_FLOW == 0)
        if (DVDRAM && ((svoIramVar.svoInState & TRN_MASK)== SVO_TGAFE))
        {
            // Wobble processor is not in lock mode in TGAFE.
            // Treat written DVD-RAM disc as blank to skip
            // TILT (RF_P2P) and FEBC (RF_P2P) calibration.
            SendMsg80(DEBUG_DISK_ID_MSG,0x31003C); //Treat written DVD-RAM as blank in TGAFE!
            ret_seq = SEQ_EX1;
        }
#endif
    }
    return (ret_seq);
}

/*-----------------------------------------------------------------------------
 *
 * Module: state_dif_RF_gain_calib()
 * Description: DIF RF Gain calibration
 *
 *----------------------------------------------------------------------------*/
#define POSITIVE_0p5_DB    (1.0592537252)
//#define POSITIVE_0p75_DB (1.0901844924)
//#define POSITIVE_1_DB    (1.1220184543)

seq_exit_t state_dif_RF_gain_calib(void)
{
    errcode_t ret;
    SBYTE  RfGainPeakPeak, RfOffsetPeakPeak;
    USHORT TargetPeakPeakmV;
    BYTE   ExpectedModX100;
    BYTE   calfactor;
    BYTE   new_RFGN;
    SBYTE  RFCOS_shift;
    SHORT  temp_RfOffset;
#if 0 //debug message
    ad_dsp_t  Result;
#endif

    SendMsg80(SHOW_DEBUG_MSG,0x3A011F); //RF GAIN Calibration
#if 0//Monitor Signals/
    WRITE_FIELD(HAL_AFE_MUXTP1SEL, halAFE_MUXTP1SEL0);// DIGTP1(3)-VSS;DIGTP1(2)-BLANK;DIGTP1(1)-XTOK;DIGTP1(0)-VSS;
    //WRITE_FIELD(HAL_AFE_MUXTP2SEL, halAFE_MUXTP2SEL0);// DIGTP2(3)-VSS;DIGTP2(2)-BLANKI;DIGTP2(1)-RC;DIGTP2(0)-VSS;
    Set_MIO2_TO_AFESignal(halAFE_AFE_MIO_SEL2);//Monitor DIGTP1
    //Set_MIO2_TO_AFESignal(halAFE_AFE_MIO_SEL3);//Monitor DIGTP2
#endif

    /* Disable the AGC/AOC and give control to the AFE */
    HAL_DFE_EnDisAgcAoc(halDFE_AgcAocDisable);                  /* Disable RF AGC/AOC */
    HAL_AFE_AgcAocControl(halAFE_RfAgcAoc_AfeControl);          /* AFE control on AGC/AOC */
    HAL_DFE_EnDisBlankDetection(halDFE_BlankDetectDisable);     /* Disable blank detection */

    if ((svoCalVar.calGainDoneBits & dif_RF_end) == 0)
    {
        /* Disable the defect detectors */
        HAL_DFE_EnDisHfDefect(halDFE_HfDefectDisable);              /* Disable HF defect */
        HAL_DFE_EnDisAgcAocHold(halDFE_AgcAocHoldDefectDisable);            // Enable Freezing of AGC Dynamic Gain/AOC Hold During Defect

        /* Disable black and white defect and open BS defect for focus drop caused by defect and unstable RF(ABEX BB26) */
        Defect_Enable(DEFECT_SOURCE_SERVO_BS);

#if (CHIP_REV >= 0xC0)
        /* Don't preset auto data field in RF calibration*/
        WRITE_FIELD(HAL_DFE_PREAUTOD, FALSE);
#endif

        /* RF gain Calibration */
        TargetPeakPeakmV = (USHORT)(COEF_GetValue(DFE_CALC_Target_pp));

        ExpectedModX100 = COEF_GetValue(DFE_CALC_M);
#if (ENABLE_BDRLTH == 1)
        if (svoIramVar.LTHDisc == 1)
        {
            ExpectedModX100 = ExpectedModX100 * 3 / 4;//LTH disc have lower modulation
        }
#endif

        WRITE_FIELD(HAL_AFE_RFAOCREG,svoCalOfsT3Var[Layer_Index][RFPGA_OS].normal_offset);

        //start with smaller gain to prevent saturation
        WRITE_FIELD(HAL_AFE_RFAGCREG,(svoCalGnT3Var[Layer_Index][RFPGA_GN].normal_gain > 8)?  svoCalGnT3Var[Layer_Index][RFPGA_GN].normal_gain - 8 : 0);

        ret = HAL_DFE_CalibrateDiffRfGain(TargetPeakPeakmV, ExpectedModX100, &RfGainPeakPeak, &RfOffsetPeakPeak);
        new_RFGN = READ_FIELD(HAL_AFE_RFGN);

        if (ARG2 == TRK_OFF)
        {
            RFCOS_shift = -(LONG)RfOffsetPeakPeak * svoRRFVar.step_size[0] / (2 * svoRRFVar.rfcos_step_size[new_RFGN]);
            if (RFCOS_shift != 0)
            {
                temp_RfOffset = RfOffsetPeakPeak - svoRRFVar.ofs_ldoff_cur;
                calSetRfcosShift(svoRRFVar.rfcos_shift_cur[Layer_Index] + RFCOS_shift);
                RfOffsetPeakPeak = temp_RfOffset + svoRRFVar.ofs_ldoff_cur;
            }
            if (ret == eERR_RF_not_present)
            {
                svoRRFVar.modulation[Layer_Index] = 0;
                SendMsg80(SHOW_DEBUG_MSG,0x310031);       // Blank_check==> Blank
            }
            else
            {
                SendMsg80(SHOW_DEBUG_MSG,0x310032);       // Blank_check==> Recorded
                if (ret == eNOERR)
                {
                    LAL_AFE_GetDPDInpValues(Layer_Index, svoPrvtVar_maxSRFO, svoRampMeasVar.SRFO_gain - svoCalMisc2Var.pdic_gn - svoCalMisc2Var.laser_gn);
                }
            }
#if (OPEN_SESSION==1)
    #if (CDDVDNewBlankChk == 1)
            if (((svoVar_discKindT.Bit.DiscStd == eDiscDVD) && (svoVar_TrackingMethod == TRK_METHOD_DPD)) || (svoVar_discKindT.Bit.DiscStd == eDiscCD))
    #else
            if ((svoVar_discKindT.Bit.DiscStd == eDiscDVD) && (svoVar_TrackingMethod == TRK_METHOD_DPD))
    #endif
            {
                if ((ret == eERR_RF_not_present) || (ret == eLOOP_expire_calibrate))
                {
                    SendMsg80(SHOW_DEBUG_MSG,0x310674);//DIF_RF_BLANK_OR_CACULATE_FAIL
                    track_off_rf_dif_fail = TRUE;
                    WRITE_FIELD(HAL_AFE_RFAOCREG, 0);
                    return(SEQ_EX0);
                }
                else
                {
                    track_off_rf_dif_fail = FALSE;
                }
            }
            else
            {
                track_off_rf_dif_fail = FALSE;
            }
#endif
        }
#if (OPEN_SESSION==1)
        if (ARG2 == TRK_ON)
        {
            if (ret == eNOERR)
            {
                Recorded_stp_Position[Layer_Index] = Iram_stp_CurrentPosition;
            }
            else if ((ret == eERR_RF_not_present) || (ret == eLOOP_expire_calibrate))
            {
                if ((svoVar_TrackingMethod != TRK_METHOD_DPP) || (svoVar_discKindT.Bit.DiscStd == eDiscCD))
                {
                    SendMsg80(SHOW_DEBUG_MSG,0x310674);//DIF_RF_BLANK_OR_CACULATE_FAIL
                    if (cal_retry++ < ((svoVar_discKindT.Bit.DiscStd == eDiscBD)? 10 : 20))
                    {
                        track_off();
                        return(SEQ_EX1);
                    }
                }
            }
            cal_retry = 0;
        }
#endif

        //send_msg24(SHOW_DEBUG_MSG,0x5B0501,0,RfGainPeakPeak,RfOffsetPeakPeak,ExpectedModX100); // ==>Calib Pass:-))

        // 1. This protection just want to increase/decrease "6dB" of RFGN
        // 2. HAL_DFE_GDDATGMON changes frequently, so use HAL_AFE_RFAGCREG
        // 3. After change RFGN, we must re-set RFCP_OS/RFCN_OS which are used to get Dark level,
        //    and RFPGA_OS which is used to reach zero level.
        // 4. If Change RFINGN, instead of RFGN, we will also need to calibrate RFCP_OS/RFCN_OS
        //    which are used to reach zero level.
        // 5. Prevent AGC gain from too close corner.
#if (CHIP_REV < 0xC0)
        if (((RfGainPeakPeak >= ((ARG2 == TRK_ON)? (0x10+12) : (0x10+11))) && (new_RFGN < 3)) || //Gain Max(31)-6
            ((RfGainPeakPeak <= ((ARG2 == TRK_ON)? (0x10-6) : (0x10-13))) && (new_RFGN > 0)))//Gain min(0)+8
#else
        if ((((RfGainPeakPeak >= ((ARG2 == TRK_ON)? (0x10+6) : (0x10+5))) && (new_RFGN < (HAL_AFE_RFGN_MASK >> HAL_AFE_RFGN_LSB))) || //Gain Max(31)-6
             ((RfGainPeakPeak <= ((ARG2 == TRK_ON)? (0x10-4) : (0x10-7))) && (new_RFGN > 0))))
#endif
        {
            if (RfGainPeakPeak <= 0x10)
            {
#if (CHIP_REV < 0xC0)
                RfGainPeakPeak += 11;
#else
                RfGainPeakPeak += 5;
#endif
                new_RFGN--;
            }
            else
            {
#if (CHIP_REV < 0xC0)
                RfGainPeakPeak -= 11;
#else
                RfGainPeakPeak -= 5;
#endif
                new_RFGN++;
            }
            calSetAfeGain(Layer_Index,RF_GN,TYPE2,PATH_SELECT_ALL,0,new_RFGN);
            calSetRfcosShift(svoRRFVar.rfcos_shift_cur[Layer_Index]);
            WRITE_FIELD(HAL_AFE_RFAGCREG, RfGainPeakPeak);
            //Change offset will cause RFtop_saturate or RFbot_saturate, set offset to be 0mv
            WRITE_FIELD(HAL_AFE_RFAOCREG, 0);

            SendMsgCn(SHOW_DEBUG_MSG,1,0x33830E,new_RFGN);     //Retry;Adjust RFGN=%d
            return(SEQ_BUSY);
        }

        calSetAfeGain(Layer_Index,RFPGA_GN,TYPE3,0,0,RfGainPeakPeak);
        calSetAfeOffset(Layer_Index,RFPGA_OS,TYPE3,0,0,calSBYTE2DacCount(RfOffsetPeakPeak,HAL_AFE_RFAOCREG_MSB-HAL_AFE_RFAOCREG_LSB+1));
#if (NEW_RFPOS_CALIB_BRING_AGCVREF_TO_ZERO == 0)
        svoRRFVar.rfaoc_ref[Layer_Index] = RfOffsetPeakPeak + RoundDivide(svoRRFVar.rfagc_ref * 256, svoRRFVar.step_size[RfGainPeakPeak]);
#endif
        if ((svoIramVar.svoInState & TRN_MASK) == TRN_038)
        {
            SendMsgCn(DEBUG_CALIB_RESULT_MSG,2,0x310520+Layer_Index,svoCalGnT3Var[Layer_Index][RFPGA_GN].normal_gain,svoCalOfsT3Var[Layer_Index][RFPGA_OS].normal_offset);
        }

        //Moved for RF-based TILT/FEBC ***
#if (BD_ENABLE == 1)
        if (svoVar_discKindT.Bit.DiscStd == eDiscBD)
        {
            LAL_WriteCoef(DFE_BLSDETCFG_BLSDUTHR); // recalled from Coef
        }
#endif
        if (svoVar_discKindT.Bit.DiscStd == eDiscCD)
        {
            calfactor = (BYTE)((ULONG)TargetPeakPeakmV*256/2000*POSITIVE_0p5_DB);
        }
        else
        {
            calfactor = (BYTE)((ULONG)TargetPeakPeakmV*256/2000); //179 = 0xB3
        }
        WRITE_FIELD(HAL_DFE_AGCTHRHI, calfactor + 5);
        WRITE_FIELD(HAL_DFE_AGCTHRLO, calfactor - 2);

#if (MAX_READ_SPEED > e32XCAV)//Improve CDRW's OD at 40X, 48X
        if (svoVar_discKindT.Bit.DiscStd == eDiscCD)
            WRITE_FIELD(HAL_DFE_OFFSETBND, 0x20);//set AOC bound
        else
#endif
            WRITE_FIELD(HAL_DFE_OFFSETBND, 0x03);//set AOC bound
        //Moved for RF-based TILT/FEBC &&&

#if (DVD_RAM_READ == 1)
        RfHeaderOffset = RfOffsetPeakPeak - COEF_GetValue(DFE_CALC_HFAOC_OFFSET_HEADER);
#else
        RfHeaderOffset = RfOffsetPeakPeak;
#endif
        /* The gains and offsets are known, the ranges can be set */
        HAL_DFE_SetAgcAocBoundary(COEF_GetValue(DFE_CALC_Coef_Max_dyn_gain), COEF_GetValue(DFE_CALC_Coef_Min_dyn_gain),
                                  COEF_GetValue(DFE_CALC_Coef_Max_dyn_Offset), COEF_GetValue(DFE_CALC_Coef_Min_dyn_Offset));  /* Set RF AGC/AOC Boundaries*/

        HAL_DFE_SetRfAocCalOffset(RfOffsetPeakPeak,RfHeaderOffset);
        HAL_DFE_SetRfAgcCalGain(RfGainPeakPeak);

        HAL_AFE_AgcAocControl(halAFE_RfAgcAoc_DfeControl);          /* DFE control on AGC/AOC */
        /* Enable the HF defect detectors and switch on blank detection */
        //HAL_DFE_SetBkDefectThr(COEF_GetValue(DFE_CALC_Target_pp),(svoRRFVar.modulation[Layer_Index] == 0? (BYTE)COEF_GetValue(DFE_CALC_M) : svoRRFVar.modulation[Layer_Index]),165,139,162,135);
        HAL_DFE_EnDisAgcAocHold(halDFE_AgcAocHoldDefectEnable);            // Enable Freezing of AGC Dynamic Gain/AOC Hold During Defect
#if 0
        HAL_DFE_EnDisHfDefect(halDFE_HfDefectEnable);                /* Enable HF defect and the AGC/AOC is hold at blank area*/
        HAL_DFE_EnDisBlankDetection(halDFE_BlankDetectEnable);
#endif

#if (HOLD_TE_BLANK==1)
        if(svoVar_discKindT.Bit.DiscStd != eDiscCD)
#elif (BD_ENABLE == 1)
        if(svoVar_discKindT.Bit.DiscStd == eDiscBD)
#else
        if(0)
#endif
        {
            HAL_DFE_EnDisBlankDetection(halDFE_BlankDetectEnable);
        }

        if (ARG2 == TRK_ON) //&& HAL_GLOBAL_GetRfPresent()) // will be enabled when the blank detection is ok
        {
            HAL_DFE_EnDisAgcAoc(halDFE_AgcAocEnable);               /* Enable RF AGC/AOC */

#if 0 //debug message
            Result = readADCmVPerRevs(ONE_REVOLUTION, TRUE, RF_ADC);//this line stall one revolution, should be removed later.
            SendMsgCnB(SHOW_DEBUG_MSG,3,0x010109,B2B(TargetPeakPeakmV),B1B(ExpectedModX100));
            SendMsgCnB(SHOW_DEBUG_MSG,4,0x01010A,B2B(Result.top_mv),B2B(Result.p2p_mv));
            SendMsgCnB(SHOW_DEBUG_MSG,3,0x01010C,B1B(Layer_Index),B1B(READ_FIELD(HAL_DFE_GDDATGMON)),B1B(READ_FIELD(HAL_DFE_ODDATGMON)));
            SendMsgCnB(SHOW_DEBUG_MSG,3,0x01010D,B1B(Layer_Index),B1B(READ_FIELD(HAL_AFE_RFAGCREG)),
                B1B(calDacCount2SBYTE(READ_FIELD(HAL_AFE_RFAOCREG),HAL_AFE_RFAOCREG_MSB-HAL_AFE_RFAOCREG_LSB+1)));
#endif
            // <Lx> DPD_GN = %02Xh (%4.1f dB), RFIN_GN = %02Xh (%2d dB), RF_GN = %02Xh (%2d dB)
            SendMsgCnB(DEBUG_CALIB_RESULT_MSG,9,0x310580+Layer_Index,
                B1B(svoCalGnT3Var[Layer_Index][DPD_GN].normal_gain),F4B((svoCalGnT3Var[Layer_Index][DPD_GN].normal_gain-4)*1.5),
                B1B(svoCalGnT3Var[Layer_Index][RFIN_GN].normal_gain),B1B(AFE_RFIN_GN[svoCalGnT3Var[Layer_Index][RFIN_GN].normal_gain]),
                B1B(svoCalGnT2Var[Layer_Index][RF_GN].normal_gain),B1B(AFE_RF_GN[svoCalGnT2Var[Layer_Index][RF_GN].normal_gain]));

            if ((ret == eNOERR)||(ret == eERR_RF_not_present)||(ret == eLOOP_expire_calibrate))
            {
                if ((svoVar_discKindT.Bit.Layers == eLayersSingle) || (Layer_Index == 1))
                {
                    svoCalVar.calGainDoneBits |= dif_RF_end;
                }
            }
        }
        else
        {
            //Base on target level to set RECD threshold
            //WRITE_FIELD(HAL_DFE_BLSDLTHR, (SBYTE)((LONG)-TargetPeakPeakmV * 256 / (2000 * 4))); //Now called from Coef
            //WRITE_FIELD(HAL_DFE_BLSDUTHR, (SBYTE)((LONG)TargetPeakPeakmV * 256 / (2000 * 4))); //Now called from Coef
        }
#if (CHIP_REV >= 0xC0)
        /* Preset auto data field */
        WRITE_FIELD(HAL_DFE_PREAUTOD, TRUE);
#endif
    }
    else
    {
        send_msg80(DEBUG_CALIB_ENTRYS_MSG,0x5A0002);  // SKIP

        //send_msg54(1,0xAAAAAF,READ_FIELD(HAL_DFE_BLSDEN));
        //send_msg54(1,READ_FIELD(HAL_AFE_RFAGCEN),READ_FIELD(HAL_AFE_RFAOCEN),READ_FIELD(HAL_DFE_AGCON),READ_FIELD(HAL_DFE_AOCON));
        //send_msg54(1,READ_FIELD(HAL_AFE_RFAGCREG),READ_FIELD(HAL_AFE_RFAOCREG),READ_FIELD(HAL_AFE_RFPOS),READ_FIELD(HAL_AFE_RFADCTEN));
        //send_msg54(1,READ_FIELD(HAL_AFE_RFHSSEL),READ_FIELD(HAL_AFE_RFHSFC),READ_FIELD(HAL_AFE_RFPOS),READ_FIELD(HAL_AFE_RFADCTEN));
        //send_msg5SValue(1,"DFEAGCGN",1,HAL_DFE_GetRfAgcCalGain());
        //send_msg5SValue(1,"DFEAOCGN",1,HAL_DFE_GetRfAocCalOffset());

        RfGainPeakPeak = svoCalGnT3Var[Layer_Index][RFPGA_GN].normal_gain;
        RfOffsetPeakPeak = calDacCount2SBYTE(svoCalOfsT3Var[Layer_Index][RFPGA_OS].normal_offset,8);

        //send_msg5SValue(1,"DFEAGCSt",1,RfGainPeakPeak);
        //send_msg5SValue(1,"DFEAOCSt",1,RfOffsetPeakPeak);
        WRITE_FIELD(HAL_AFE_RFAOCREG, calSBYTE2DacCount(RfOffsetPeakPeak,8));
        WRITE_FIELD(HAL_AFE_RFAGCREG, RfGainPeakPeak);

        HAL_DFE_SetRfAocCalOffset(RfOffsetPeakPeak,RfHeaderOffset); /* Set the AOC offset to the calibrated peak to peak value */
        HAL_DFE_SetRfAgcCalGain(RfGainPeakPeak);                    /* Set the AGC gain to the calibrated peak to peak value */
        HAL_AFE_AgcAocControl(halAFE_RfAgcAoc_DfeControl);          /* DFE control on AGC/AOC */
        HAL_DFE_EnDisAgcAocHold(halDFE_AgcAocHoldDefectEnable);     /* Enable Freezing of AGC Dynamic Gain/AOC Hold During Defect */

#if (HOLD_TE_BLANK==1)
        if(svoVar_discKindT.Bit.DiscStd != eDiscCD)
#elif (BD_ENABLE == 1)
        if(svoVar_discKindT.Bit.DiscStd == eDiscBD)
#else
        if(0)
#endif
        {
            HAL_DFE_EnDisBlankDetection(halDFE_BlankDetectEnable);
        }

        if (ARG2 == TRK_ON)
        {
            HAL_DFE_EnDisAgcAoc(halDFE_AgcAocEnable);               /* Enable RF AGC/AOC */
        }
    }

    if (ARG2 == TRK_OFF)
    {
        if (svoVar_discKindT.Bit.DiscStd == eDiscBD)
        {
            //WRITE_FIELD(HAL_DFE_BLSDLTHR, (SBYTE)((LONG)-TargetPeakPeakmV * 256 / (2000 * 4)));
            WRITE_FIELD(HAL_DFE_BLSDUTHR, (SBYTE)((LONG)TargetPeakPeakmV * 256 / (2000 * 4))); //later restore from Coef
        }
    }

    SetDelayTimer(ARG1);
    return(SEQ_EX0);
}

/*-----------------------------------------------------------------------------
 *
 * Module: state_dif_wbl_gain_calib()
 * Description: DIF Wobble Gain calibration
 *
 *----------------------------------------------------------------------------*/
seq_exit_t state_dif_wbl_gain_calib(void)
{
#if (EN_WOBBLE == 1)
    #if (PLUS_MINUS_DET_TEST != 0)
    USHORT loop;
    ULONG  DebugMsgFlag;
    #endif /* PLUS_MINUS_DET_TEST */
    #if (BD_ENABLE == 1) && (ENABLE_BDRLTH == 1) && (BD_USE_TE_PP_DPD == 0)
    ad_dsp_t signal_ret;
    #endif

    WRITE_DSP_COEF(kfg, svoCalMisc1Var[Layer_Index].feGainDSP);//fill back original kfg//kfg gain down re fill back

    #if (PLUS_MINUS_DET_TEST != 0)
    if ((ARG3 == NORMAL) && (svoVar_discKindT.Bit.DiscStd == eDiscDVD))
    {
        DebugMsgFlag = svoDebugMsgFlag;
        open_svo_msg(DEBUG_DISK_ID_MSG);
        for (loop = 0; loop < PLUS_MINUS_DET_TEST; loop++)
        {
            wait_track_on(TRK_ON_WAIT_SLOW_TIMEOUT);
            WaitTimer(20);

            (void)Plus_Minus_det();

            svoVar_discKindT.Bit.WblType = eWblUnknown;
            svoCalVar.discChkDoneBits   &= ~plus_minus_chk_end;
            WRITE_FIELD(HAL_SRV_TSVSTP,FALSE); //check if this OK!
            WaitTimer(100);
        }
        set_svo_msg(DebugMsgFlag);
    }
    #endif /* PLUS_MINUS_DET_TEST */

    switch (svoVar_discKindT.Bit.DiscStd)
    {
        case eDiscDVD:
    #if (DVD_RAM_READ == 1)
            if (DVDRAM)
            {
                wbl_gain_calib();
                return(SEQ_EX0);
            }
    #endif
    #if (WOBBLE_READ_ON_DVD_OR_CD == 1)
            wbl_gain_calib();
            if (!Plus_Minus_det())
            {
                return(SEQ_EX2);
            }
    #endif
            break;

    #if (WOBBLE_READ_ON_DVD_OR_CD == 1)
        case eDiscCD:
        #if (EN_POWER_CONTROL == 1)
            PowerControl(eCDReadWblPower);
        #endif
            wbl_gain_calib();
            break;
    #endif

    #if (BD_ENABLE == 1)
        case eDiscBD:
        #if (ENABLE_BDRLTH == 1) && (BD_USE_TE_PP_DPD == 0)
            #if (APP_TE_SUPPORT == 1)
            if (((READ_REG(HAL_AFEREG_MAP->MB16) & (HAL_AFE_MPPAH|HAL_AFE_MPPBG)) != (HAL_AFE_MPPAH|HAL_AFE_MPPBG)) &&
                ((READ_REG(HAL_AFEREG_MAP->MB15) & (HAL_AFE_MPPA|HAL_AFE_MPPB|HAL_AFE_MPPC|HAL_AFE_MPPD)) != (HAL_AFE_MPPA|HAL_AFE_MPPB)))
            {
            #endif
            if ((Layer_Index == 0) && (svoVar_discKindT.Bit.Media != eMediaROM) && (!svoIramVar.LTHDiscRPChecked))
            {
                svoIramVar.LTHDiscRPChecked = TRUE;
                if (svoRRFVar.modulation[0] == 0)
                {
                    svoIramVar.LTHDisclikely = 1;
                }
                else
                {
                    signal_ret = ReadGPADCmV(ONE_REVOLUTION, ADC_RP, TRUE);

                    if (signal_ret.errcode != eNOERR)
                    {//measurement fail, set LTHDisclikely = 1 to read BCA to judge LTH and HTL.
                        svoIramVar.LTHDisclikely = 1;
                    }
                    else if (signal_ret.middle_mv > vref_fm_mux3_mV + COEF_GetValue(AFE_CALC_RP_PP_TARGET_10MV) * 10 / 8)
                    {
                        send_msg5S(SHOW_DEBUG_MSG,"TE polarity err");
                        svoIramVar.LTHDisclikely = 1;
                        svoIramVar.LTHDisc = ~svoIramVar.LTHDisc;
                        LAL_SetAFEandDFEforLTH();
            #if (NEW_SERVO_CALIB_FLOW == 0)
                        calClrCalResult(TILT_DAT,FALSE);
                        calClrCalResult(FEBC_DSP,FALSE);
                #if COLLI_SWITCH ==1
                        calClrCalResult(COLLI_POS,FALSE);
                #endif
            #endif
                        svoCalVar.calOfsDoneBits  &= ~(teof_end);
                        svoCalVar.calGainDoneBits &= ~(felg_end|teg_end|telg_end|dif_RF_end|dif_WOB_end);
                        return(SEQ_EX3);
                    }
                }
            }
            #if (APP_TE_SUPPORT == 1)
            }
            #endif
        #endif
            wbl_gain_calib();
            break;
    #endif
    }
#else    //EN_WOBBLE
    WRITE_DSP_COEF(kfg, svoCalMisc1Var[Layer_Index].feGainDSP);//fill back original kfg//kfg gain down re fill back
#endif

    return(SEQ_EX0);
}

#if (EN_WOBBLE == 1)
void wbl_gain_calib(void)
{
    errcode_t ret;
    BYTE table_index;

    if ((svoCalVar.calGainDoneBits & dif_WOB_end) == 0)
    {
        SendMsg80(SHOW_DEBUG_MSG,0x3A0123); //WBL GAIN Calibration

        HAL_WOBBLE_SetPllOnHold(halWBL_WblPllInHold);

        /* open AGC loop for calibration */
        HAL_WOBBLE_SetAGCOnOff(halWBL_WobbleAGC_Disable);

        /* Wobble VGA3 controlled by WBVGA3REG */
        HAL_AFE_WobVga3Control(halAFE_WobVga3_WobVga3RegControl);

        /* set target Wobble AGC */
        WRITE_FIELD(HAL_AFE_WBVGA3REG,COEF_GetValue(WBL_CALC_Target_AGC));

        /* Wobble gain Calibration */
        table_index = COEF_GetValue(WBL_CALC_WdiffGainIndexInit);
        ret = HAL_WOBBLE_CalibrateDiffWobbleGain((WBL_ADC_IN_RANGE * ((USHORT)COEF_GetValue(WBL_WBLAGCCFG_WAGCTHRHI)+(USHORT)COEF_GetValue(WBL_WBLAGCCFG_WAGCTHRLO))) / 512, &table_index);

        if (ret == eNOERR)
        {
            if ((svoVar_discKindT.Bit.Layers == eLayersSingle) || (Layer_Index == 1))
            {
                svoCalVar.calGainDoneBits |= dif_WOB_end;
            }
        }
        calSetAfeGain(Layer_Index,WOB_GN,TYPE3,0,0,table_index);
        SendMsgCn(DEBUG_CALIB_RESULT_MSG,1,0x310530+Layer_Index,svoCalGnT3Var[Layer_Index][WOB_GN].normal_gain);

        /* Preset dynamic gain from Coef. table */
        LAL_WriteCoef(WBL_AGCDYNGA_WBLDYNGA);

        /* Wobble VGA3 controlled by DFE */
        HAL_AFE_WobVga3Control(halAFE_WobVga3_DfeControl);

        /* close AGC loop for calibration */
        HAL_WOBBLE_SetAGCOnOff(halWBL_WobbleAGC_Enable);
        WaitTimer(10);

        HAL_WOBBLE_SetAGCRange(15,20); //coefs

        HAL_WOBBLE_SetPllOnHold(halWBL_WblPllNotInHold);

    #if (DVD_RAM_READ == 1)
        if(DVDRAM)
        {
            WblPllFreqPreset = 0;
            SetDfeWblFrontSemiSynchr();
            HAL_WOBBLE_SetEnableHeaderOn();
            WaitTimer(20);
        }
    #endif
    }
}
#endif // (EN_WOBBLE == 1)

/*-----------------------------------------------------------------------------
 *
 * Module: state_blank_chk()
 * Description: determine blank or written and adjust focus gain and sbad gain based it
 *
 *   FE_GN:     If a blank   disk is detected, then keep blank  gain and increase normal gain
 *              if a written disk is detected, then keep normal gain and decrease blank  gain
 *   BS_GN:     If a blank   disk is detected, then increase normal gain and write gain
 *              if a written disk is detected, then do nothing
 *
 *----------------------------------------------------------------------------*/
seq_exit_t state_blank_chk(void)
{
    BOOL  rf_present;
    SBYTE delta_fe_gn;
    SBYTE delta_sbad_gn;
    USHORT target_position;
    BYTE move_times;
#if ((BD_2X_STARTUP == 1)&&(BD_UNBALANCE_DISC_CHECK))
    ad_dsp_t ce_signal;
#endif

#if 0   //monitor max & min peak
    USHORT iii;
    SendMsg80(SHOW_DEBUG_MSG,0x320A14);
    for (iii=0; iii<300;iii++){
        SendMsgCnB(SHOW_DEBUG_MSG,4,0x33FF3D,B4B(READ_REG(HAL_DFEREG_MAP->HFAGCMON2)));
    }
#endif

    if ((svoCalVar.discChkDoneBits & blank_chk_end) == blank_chk_end)
    {
        return (SEQ_EX0);
    }

    if (svoRRFVar.modulation[Layer_Index] == 0)
    {
        rf_present = FALSE;
#if (CDDVDNewBlankChk == 1)
        RecordedCount = 0;
#endif
    }
    else
    {
#if(CDDVDNewBlankChk == 1)
        ++RecordedCount;
#endif
        rf_present = TRUE;
        Recorded_stp_Position[Layer_Index] = Iram_stp_CurrentPosition;
    }

    if (cal_retry == 0)
    {
#if ((BD_2X_STARTUP == 1)&&(BD_UNBALANCE_DISC_CHECK))
        if ((Layer_Index == 0)&&(svoVar_discKindT.Bit.DiscStd == eDiscBD)
            &&(svoIramVar.svoCurrentSpeed == _2LX_BD))
        {
            while (!svoSpeedVar.SpdInRange)
            {
                if (svoSpeedVar.SpdMonitor)
                {
                    spdMonitorRPM();
                    svoSpeedVar.SpdMonitor = FALSE;
                }
                ExitProcess();
            }
            ce_signal = ReadGPADCmV_Avg(ONE_REVOLUTION, ADC_CE, TRUE);
            Unbalance_lowspd = ce_signal.p2p_mv;
            if (Unbalance_lowspd > UNBBD_TH)
            {
                svoVar_unbalanceDiscNum = 2;
                spdSetMaxRPM(UNB_MAX_RPM);//set max rpm during start up
            }
        }
#endif

        if (svoCalGnT1Var[Layer_Index][FE_GN].groove_gain[RECDL] == svoCalGnT1Var[Layer_Index][FE_GN].groove_gain[RECDH])
        {
            delta_fe_gn = (SBYTE)COEF_GetValue(AFE_CALC_FE_GAINUP_BLANK);
            delta_sbad_gn = (SBYTE)COEF_GetValue(AFE_CALC_SBAD_GAINUP_BLANK);

            if (rf_present == FALSE)
            {
                calSetAfeGain(Layer_Index,FE_GN,TYPE1,GROOVE_GAIN,RECDH,(SBYTE)svoCalGnT1Var[Layer_Index][FE_GN].groove_gain[RECDH] + delta_fe_gn);
                calSetAfeGain(Layer_Index,FE_GN,TYPE1,LAND_GAIN,  RECDH,(SBYTE)svoCalGnT1Var[Layer_Index][FE_GN].land_gain[RECDH] + delta_fe_gn);
                calSetAfeGain(Layer_Index,BS_GN,TYPE2,NORMAL_GAIN,0,    (SBYTE)svoCalGnT2Var[Layer_Index][BS_GN].normal_gain + delta_sbad_gn);
            }
            else
            {
                calSetAfeGain(Layer_Index,FE_GN,TYPE1,GROOVE_GAIN,RECDL,(SBYTE)svoCalGnT1Var[Layer_Index][FE_GN].groove_gain[RECDL] - delta_fe_gn);
                calSetAfeGain(Layer_Index,FE_GN,TYPE1,LAND_GAIN,  RECDL,(SBYTE)svoCalGnT1Var[Layer_Index][FE_GN].land_gain[RECDL] - delta_fe_gn);
                calSetAfeGain(Layer_Index,FE_GN,TYPE1,WRITE_GAIN, 0,    (SBYTE)svoCalGnT1Var[Layer_Index][FE_GN].write_gain - delta_fe_gn);
                calSetAfeGain(Layer_Index,BS_GN,TYPE2,WRITE_GAIN, 0,    (SBYTE)svoCalGnT2Var[Layer_Index][BS_GN].write_gain - delta_sbad_gn);
            }
        }
        if (rf_present == FALSE)
        {
            svoCalVar.calDoneBits |= ((Layer_Index == 1)? L1_blank_det : L0_blank_det);
        }
        SendMsgCn(DEBUG_CALIB_RESULT_MSG,3,0x31050E,Layer_Index,svoCalGnT1Var[Layer_Index][FE_GN].groove_gain[0],svoCalGnT1Var[Layer_Index][FE_GN].groove_gain[1],svoCalGnT2Var[Layer_Index][BS_GN].normal_gain);
    }
    SendMsgCn(DEBUG_CALIB_RESULT_MSG,3,0x31052E,Layer_Index,svoCalGnT3Var[Layer_Index][RFPGA_GN].normal_gain,svoCalOfsT3Var[Layer_Index][RFPGA_OS].normal_offset,svoCalGnT3Var[Layer_Index][DPD_GN].normal_gain);

#if(CDDVDNewBlankChk == 1)
    if ((rf_present == FALSE) || (track_off_rf_dif_fail == TRUE) || ((cal_retry != 0) && (RecordedCount < 2) && (svoVar_discKindT.Bit.DiscStd != eDiscBD)))
#else
    if (rf_present == FALSE)
#endif
    {   // Blank Disc
        switch (svoVar_discKindT.Bit.DiscStd)
        {
            case eDiscBD:
                target_position = UM_2_SLED_STEP(24000);//PSN=0x100000
#if (DISC_8CM == 1)
                if (svoVar_discDiameter == e8cm)
                    move_times = 3;
                else
#endif
                    move_times = 5;
                break;

            case eDiscDVD:
                target_position = UM_2_SLED_STEP(23000);//132A
                if (Layer_Index == 1)
                {
#if (DISC_8CM == 1)
                    if (svoVar_discDiameter == e8cm)
                        target_position = UM_2_SLED_STEP(35000);
                    else
#endif
                        target_position = UM_2_SLED_STEP(55000);
                }
                move_times = 20;
                break;

            case eDiscCD:
            default:
                target_position = UM_2_SLED_STEP(24000);//132A
                move_times = 8;
                break;
        }

        if (cal_retry <= move_times)
        {
            if (svoVar_discKindT.Bit.DiscStd == eDiscBD)
            {
#if (DISC_8CM == 1)
                if (svoVar_discDiameter == e8cm)
                    target_position += UM_2_SLED_STEP(4650 * cal_retry);
                else
#endif
                    target_position += UM_2_SLED_STEP(6800 * cal_retry);
            }
            else if(svoVar_discKindT.Bit.DiscStd == eDiscDVD)
            {
                target_position += UM_2_SLED_STEP(200 * cal_retry);
            }
            else//CD
            {
                target_position += UM_2_SLED_STEP(400 * cal_retry);
            }
            cal_retry++;

            svoStp_JumpToPosition(STEP_SAFE_SLOW_MODE, target_position);

            while (svoStp_busy())
            {
                ExitProcess();
            }
            WaitTimer(20);
            HAL_SERVO_CenterControlOFF();
            svoIramVar.currentID.SectorNum.all32s = svoRadius2PBA(Iram_stp_CurrentPosition, 0);
            SetDelayTimer(10);

            return (SEQ_EX3);
        }
        else
        {
            cal_retry = 0;
            if (svoVar_discKindT.Bit.DiscStd == eDiscBD)
            {
                svoStp_JumpToPosition(STEP_SAFE_SLOW_MODE, Recorded_stp_Position[Layer_Index]);

                while (svoStp_busy())
                {
                    ExitProcess();
                }
                WaitTimer(20);
                HAL_SERVO_CenterControlOFF();
                svoIramVar.currentID.SectorNum.all32s = svoRadius2PBA(Iram_stp_CurrentPosition, 0);
                SetDelayTimer(10);
            }
            else if ((svoVar_discKindT.Bit.DiscStd == eDiscDVD) && (Layer_Index == 1))
            {
                svoStp_JumpToPosition(STEP_SAFE_SLOW_MODE, Recorded_stp_Position[0]);
                while (svoStp_busy())
                {
                    ExitProcess();
                }
                WaitTimer(20);
                HAL_SERVO_CenterControlOFF();
                svoIramVar.currentID.SectorNum.all32s = svoRadius2PBA(Iram_stp_CurrentPosition, 0);
                SetDelayTimer(10);

                svoSeekVar.svoTargetID.Layer = 0;

                svoCalVar.discChkDoneBits |= blank_chk_end;
                svoCalVar.calGainDoneBits |= (tebg_end | ceg_end | teg_end | rpg_end | rpbg_end | dif_RF_end | dif_WOB_end | felg_end | telg_end);

                SendMsg80(SHOW_DEBUG_MSG,0x33FF2C); //DVD L1 blank, skip calibrations
                SendMsg80(SHOW_DEBUG_MSG,0x33FF3F); //DVD L1 blank, jump to L0

                (void)svo_Layer_Jump(svoSeekVar.svoTargetID.Layer);

                return (SEQ_EX4);
            }
            else
            {
#if (DISC_MIS_JUDGE_START_UP_RECOVERY)
                //If Disc is mis-judged, then most cases are lower than these thresholds. For these cases, retry next Laser type is needed
                if(  ((svoVar_discKindT.Bit.DiscStd == eDiscCD)&&(((float)svoPrvtVar_maxFEpp_0dB/svoPrvtVar_maxSRFOpp_0dB) >= MIN_BLANK_CD_FESRFO_RATIO))
                   ||((svoVar_discKindT.Bit.DiscStd == eDiscDVD)&&(((float)svoPrvtVar_maxFEpp_0dB/svoPrvtVar_maxSRFOpp_0dB) >= MIN_BLANK_DVD_FESRFO_RATIO))
                  )
                {
    #if DDisc == 1
                    if (DualDisc != 0)
                    {
                        RetryDiscLaser = eDiscDVD;
                    }
                    else
    #endif
                    {
                        RetryDiscLaser = eDiscUnknown;
                    }
                }
#endif
                return (SEQ_EX2);
            }
        }
    }
    else
    {
        cal_retry = 0;
    }

    svoSeekVar.targetRadius = Iram_stp_CurrentPosition;
    if ((svoVar_discKindT.Bit.Layers == eLayersSingle) || (Layer_Index == 1))
    {
        svoCalVar.discChkDoneBits |= blank_chk_end;
    }

    return (SEQ_EX0);
}

/*-----------------------------------------------------------------------------
 *
 * Module: state_RWdisc_check()
 * Description: RW disc detection!!
 *
 *----------------------------------------------------------------------------*/
seq_exit_t state_RWdisc_check(void)
{
    SHORT  disckind_rw_th;
    SHORT  orig_pdic_gn, orig_laser_gn;
    USHORT temp_fe_srfo_ratio;

#if (DVD_RAM_READ == 1)
    if ((DVDRAM) && (svoPrvtVar_avgBSpp[0] < 50))
    {
        // avgBSpp is less than 50 mV.
        // FE PP measurement failed during DVD-RAM re-start processes.
        svoVar_discKindT.fDiscKind = eUnknownKind;
        SendMsg80(SHOW_DEBUG_MSG,0x3100CE);  //RW_check==>DVD-RAM FE_PP error
        return(SEQ_EX1);
    }
#endif

    if ((svoCalVar.discChkDoneBits & disc_chk_end) == disc_chk_end)
    {
        return(SEQ_EX0);
    }
    SendMsgCn(DEBUG_CALIB_RESULT_MSG,2,0x310506+Layer_Index,svoCalOfsT2Var[Layer_Index][FEO_OS].normal_offset,svoCalOfsT2Var[Layer_Index][SPP2_OS].normal_offset);

    temp_fe_srfo_ratio = svoPrvtVar_maxFEpp_0dB * 100 / svoPrvtVar_maxSRFOpp_0dB;
    //SendMsg80(SHOW_DEBUG_MSG,0x33FF0D); //RWdisc_check
    // RWDisc_Check: FE_0dB = %4u (mV), SRFO_0dB = %4u (mV), FE/SRFO_ratio (X100) = %3d
    SendMsgCn(SHOW_DEBUG_MSG,6,0x3100BC,A2B(svoPrvtVar_maxFEpp_0dB),A2B(svoPrvtVar_maxSRFOpp_0dB),
                                                                 A2B(temp_fe_srfo_ratio));
    orig_pdic_gn = svoCalMisc2Var.pdic_gn;
    orig_laser_gn = svoCalMisc2Var.laser_gn;

    if ((temp_fe_srfo_ratio < COEF_GetValue(AFE_CALC_MIN_FE_SRFO_RATIO)) ||
        (svoPrvtVar_maxFEpp_0dB <= 1) || (svoPrvtVar_maxSRFOpp_0dB <= 1))
    {
        SendMsg80(SHOW_DEBUG_MSG,0x3100BD);  //RWDisc_check==>Wrong Disc Type
        svoVar_discKindT.fDiscKind = eUnknownKind;

	//[W.0R]
        if((svoRampMeasVar.STM > 600) && (svoRampMeasVar.STM < 1000))
	{
		MediaRecovery = 0x10;/// CD  identfied as DVD
	}


	#if (Customer_A1==1)
	{
		Iram_stp_Initialized = FALSE;	
		CompleteStroke = TRUE;
		return(SEQ_EX4);
	}
	#else
	{
        return(SEQ_EX1);
    }
	#endif
		
    }

    //When BD disc, gain_factor could be times 3, depending on STM setting
    switch (svoVar_discKindT.Bit.DiscStd)
    {
        case eDiscCD:
            if (svoVar_discKindT.Bit.Media == eMediaUnknown)
            {
                if (svoPrvtVar_maxSRFOpp_0dB <= COEF_GetValue(AFE_CALC_CDROM_SRFO_TH))
                {
#if(!DISC_MIS_JUDGE_RECOVERY)
                    if (svoPrvtVar_maxSRFOpp_0dB <= COEF_GetValue(AFE_CALC_NODISC_SRFO_TH))
                    {
                        SendMsg80(SHOW_DEBUG_MSG,0x3100CF);  //RWDisc_check==>No Disc
                        svoVar_discKindT.fDiscKind = eUnknownKind;
                        return(SEQ_EX1);
                    }
#endif
                    SendMsg80(SHOW_DEBUG_MSG,0x3100C0); //RWDisc_check==>CD-RW Disc
                    svoVar_discKindT.Bit.Media = eMediaRW;
                    LAL_StoreParams(LAL_DetectDiscKind(svoVar_discKindT), svoIramVar.svoCurrentSpeed);
                    LAL_SetReadMode();
                }
                else
                {
                    SendMsg80(SHOW_DEBUG_MSG,0x3100C1);  //RWDisc_check==>CD-ROM/R Disc
                    svoVar_discKindT.Bit.Media = eMediaROM;
                    LAL_StoreParams(LAL_DetectDiscKind(svoVar_discKindT), svoIramVar.svoCurrentSpeed);
                    LAL_SetReadMode();
                }
            }
            break;

        case eDiscDVD:
            if (svoVar_discKindT.Bit.Media == eMediaUnknown)
            {
                if (fepp_wrong_layer_chkcnt >= 5)
                {
                    send_msg5S(1, "WrongDiscType")
                    svoVar_discKindT.fDiscKind = eUnknownKind;
                    return (SEQ_EX1);
                }

                if (svoVar_discKindT.Bit.Layers == eLayersDouble)
                    disckind_rw_th = COEF_GetValue(AFE_CALC_DVDROMDL_SRFO_TH);
                else
                    disckind_rw_th = COEF_GetValue(AFE_CALC_DVDROM_SRFO_TH);

                if (svoPrvtVar_maxSRFOpp_0dB <= disckind_rw_th)
                {
                    if (svoPrvtVar_maxSRFOpp_0dB <= COEF_GetValue(AFE_CALC_NODISC_SRFO_TH))
                    {
                        SendMsg80(SHOW_DEBUG_MSG,0x3100CF);  //RWDisc_check==>No Disc
                        svoVar_discKindT.fDiscKind = eUnknownKind;
                        return(SEQ_EX1);
                    }
                    SendMsg80(SHOW_DEBUG_MSG,0x3100C2+svoVar_discKindT.Bit.Layers-1);  //RWDisc_check==>DVD RW xL Disc
                    svoVar_discKindT.Bit.Media   = eMediaRW;
                    svoVar_discKindT.Bit.WblType = eWblMinus;

                    LAL_StoreParams(LAL_DetectDiscKind(svoVar_discKindT), svoIramVar.svoCurrentSpeed);
                    LAL_SetReadMode();

                }
                else
                {
                    SendMsg80(SHOW_DEBUG_MSG,0x3100C4+svoVar_discKindT.Bit.Layers-1);  //RWDisc_check==>DVD-ROM/R xL Disc
                    svoVar_discKindT.Bit.Media   = eMediaUnknown;
                    svoVar_discKindT.Bit.WblType = eWblUnknown;
#if (WOBBLE_READ_ON_DVD_OR_CD == 0) || (EN_WOBBLE == 0)
                    svoVar_discKindT.Bit.Media   = eMediaROM;
#endif
                    LAL_StoreParams(LAL_DetectDiscKind(svoVar_discKindT), svoIramVar.svoCurrentSpeed);
                    LAL_SetReadMode();
                }
            }
            break;
#if (BD_ENABLE == 1)
        case eDiscBD:
            if (svoVar_discKindT.Bit.Media == eMediaUnknown)
            {
                if (svoVar_discKindT.Bit.Layers == eLayersDouble)
                {
                    disckind_rw_th = COEF_GetValue(AFE_CALC_BDROMDL_SRFO_TH);
                }
                else
                {
                    disckind_rw_th = COEF_GetValue(AFE_CALC_BDROM_SRFO_TH);

		#if (Customer_A1 ==1)
			if(MediaRecovery == TRUE)
				disckind_rw_th = 1100;//[W.0Q]Walker
		#endif
                }

                if (svoPrvtVar_maxSRFOpp_0dB <= disckind_rw_th)
                {//SRFOpp level below threshold: pretend RW
                    if (svoPrvtVar_maxSRFOpp_0dB <= COEF_GetValue(AFE_CALC_NODISC_SRFO_TH))
                    {//SRFOpp level indicates nodisc
                        SendMsg80(SHOW_DEBUG_MSG,0x3100CF);  //RWDisc_check==>No Disc
                        svoVar_discKindT.fDiscKind = eUnknownKind;
                        return(SEQ_EX1);
                    }

                    SendMsg80(SHOW_DEBUG_MSG,0x3100C6+svoVar_discKindT.Bit.Layers-1);  //RW_check==>BD-R/RE xL Disc

                    svoVar_discKindT.Bit.Media   = eMediaRW;
                    svoVar_discKindT.Bit.WblType = eWblUnknown;
    #if(USE_EEPROM_COLLI_POS == 1)
//                    UpdateColliPos();
//                    svoStp_JumpToPosition(COLLI_NORMAL_MODE, calGetOptColliPos(Layer_Index));
//                    while (svoStp_busy())
//                    {
//                        ExitProcess();
//                    }
    #endif
                    LAL_StoreParams(LAL_DetectDiscKind(svoVar_discKindT), svoIramVar.svoCurrentSpeed);
                    LAL_SetReadMode();

    #if (PCB == 830)
                    HAL_LASC_SetLaserOn(FALSE);
    #endif
                }
                else
                {
                    SendMsg80(SHOW_DEBUG_MSG,0x3100C8+svoVar_discKindT.Bit.Layers-1);   //RW_check==>BD-ROM xL Disc

                    svoVar_discKindT.Bit.Media   = eMediaROM;
                    svoVar_discKindT.Bit.WblType = eWblUnknown;
                    LAL_StoreParams(LAL_DetectDiscKind(svoVar_discKindT), svoIramVar.svoCurrentSpeed);
                    LAL_SetReadMode();
                }
            }
            break;
#endif
#if (ENABLE_HDDVD==1)
        case eDiscHD:
            //TBD! Review needed HDRO
            if (svoVar_discKindT.Bit.Media == eMediaUnknown)
            {
                if (svoVar_discKindT.Bit.Layers == eLayersDouble)
                {
                    disckind_rw_th = COEF_GetValue(AFE_CALC_HDROMDL_SRFO_TH);
                }
                else
                {
                    disckind_rw_th = COEF_GetValue(AFE_CALC_HDROM_SRFO_TH);
                }

                if (svoPrvtVar_maxSRFOpp_0dB <= disckind_rw_th)
                {//SRFOpp level below threshold: pretend RW
    #if(!DISC_MIS_JUDGE_RECOVERY)
                    if (svoPrvtVar_maxSRFOpp_0dB <= COEF_GetValue(AFE_CALC_NODISC_SRFO_TH))
                    {//SRFOpp level indicates nodisc
                        SendMsg80(SHOW_DEBUG_MSG,0x3100CF);  //RWDisc_check==>No Disc
                        svoVar_discKindT.fDiscKind = eUnknownKind;
                        return(SEQ_EX1);
                    }
    #endif
                    SendMsg80(SHOW_DEBUG_MSG,0x3100CA+svoVar_discKindT.Bit.Layers-1);

                    svoVar_discKindT.Bit.Media   = eMediaRW;
                    svoVar_discKindT.Bit.WblType = eWblUnknown;
                    LAL_StoreParams(LAL_DetectDiscKind(svoVar_discKindT), svoIramVar.svoCurrentSpeed);
                    LAL_SetReadMode();
                }
                else
                {
                    SendMsg80(SHOW_DEBUG_MSG,0x3100CC+svoVar_discKindT.Bit.Layers-1);
                    svoVar_discKindT.Bit.Media   = eMediaUnknown;
                    svoVar_discKindT.Bit.WblType = eWblUnknown;
                    svoVar_discKindT.Bit.Media   = eMediaROM;
                    LAL_StoreParams(LAL_DetectDiscKind(svoVar_discKindT), svoIramVar.svoCurrentSpeed);
                    LAL_SetReadMode();
    #if (EN_POWER_CONTROL == 1)
                    PowerControl(eHDReadPower);
    #endif
                }
            }
            break;
#endif
    }
    afeSetTrackingMethod(getTrackingMethod(svoVar_discKindT));
    svoCalVar.discChkDoneBits |= stm_chk_end;
    if ((orig_pdic_gn == svoCalMisc2Var.pdic_gn) && (orig_laser_gn == svoCalMisc2Var.laser_gn))
    {
        fepp_wrong_layer_chkcnt = 0;
        return(SEQ_EX0);
    }
    else
    {
        if (orig_pdic_gn != svoCalMisc2Var.pdic_gn)
        {
            svoCalVar.calOfsDoneBits &= (~havcof_end);
        }
        return(SEQ_EX2);
    }
}

/*-----------------------------------------------------------------------------
 *
 * Module: state_teg_cali()
 * Description:
 *
 *----------------------------------------------------------------------------*/
seq_exit_t state_teg_cali(void)
{
    errcode_t ret;
    USHORT    te_target;
    SBYTE     wr_diff,temp_sbyte;
    BYTE      layer;

    SendMsg80(DEBUG_CALIB_ENTRYS_MSG,0x3A0113);    //>>> STATE_TEG_CALI <<<

    if ((svoCalVar.calGainDoneBits & teg_end) == teg_end)
    {
        SendMsg80(DEBUG_CALIB_ENTRYS_MSG,0x3A0002); // ==>Calib SKIP!!
        return(SEQ_EX0);
    }

#if (OPEN_SESSION==1)
    if (((svoVar_TrackingMethod != TRK_METHOD_DPP)
    #if (WOBBLE_READ_ON_DVD_OR_CD == 0)
        || (svoVar_discKindT.Bit.DiscStd == eDiscCD) || (svoVar_discKindT.Bit.DiscStd == eDiscDVD)
    #endif
        ) && (RFpp_chk() == FALSE))
    {
        return(SEQ_EX4);
    }
#endif

    SendMsgCnB(DEBUG_AUTO_TUNING_MSG,3,0x316701,B1B(READ_FIELD(HAL_AFE_TRKG)-16),
        B1B(READ_FIELD(HAL_AFE_BLANKTRKG)-16),B1B(READ_FIELD(HAL_AFE_WRTRKG)-16)); // TRKG, BLANKTRKG, WRTRKG(dB)
    te_target = COEF_GetValue(AFE_CALC_TE_PP_TARGET_10MV) * 10;
    wr_diff   = (SBYTE)COEF_GetValue(AFE_CALC_TE_BLANK_GAINUP);

    SendMsgCn(DEBUG_AUTO_TUNING_MSG,1,0x316704,A1B(wr_diff)); // WR_DIFF(dB)

    layer = Layer_Index;

    temp_sbyte = 1;
    HAL_SERVO_CenterControlOFF();
    WaitTimer(5);
    do
    {
        ret = calCalibrateTEGnOs(te_target);
        if (ret != eNOERR)
        {
            temp_sbyte--;
#if(ERROR_CAL_START_UP_MONITOR ==1)
            svoMiscVar.calibrationErrorCount++;
#endif
            SendMsgCnB(SHOW_DEBUG_MSG,4,0x3A0001,B1B(ret),B1B(((svoIramVar.svoInState & STAT_MASK) >> 12)),B1B(((svoIramVar.svoInState & TRN_MASK ) >> 5) + 1),B1B((svoIramVar.svoInState & SEQ_MASK) + 1));
        }
    }
    while ((ret != eNOERR) && (temp_sbyte >= 0));

    /* set written_gain = blank_gain - written_to_blank_diff */
    calSetAfeGain(layer, TE_GN, TYPE1, PATH_SELECT_ALL, last_cal_recd, last_cal_result);
    if (last_cal_recd == RECDL)
    {
        temp_sbyte = last_cal_result + wr_diff;
    }
    else
    {
        temp_sbyte = (SBYTE)last_cal_result - wr_diff;
    }
    calSetAfeGain(layer, TE_GN, TYPE1, PATH_SELECT_ALL, !last_cal_recd, temp_sbyte);
    calSetAfeGain(layer, TE_GN, TYPE1, WRITE_GAIN, 0, svoCalGnT1Var[layer][TE_GN].groove_gain[RECDL]);

    calSetAfeOffset(layer, TE_OS, TYPE1, PATH_SELECT_ALL, PATH_SELECT_ALL, last_cal_result2);
#if (DPD_COMPENSATE_OFFSET==1)
    if (svoVar_TrackingMethod == TRK_METHOD_DPD)
    {
        calSetCalResult(layer, Iram_stp_CurrentPosition, DPD_GNOS, 0x00, CAL_R_DONE);
    }
#endif

    if (ret == eNOERR)
    {
        if (((svoVar_discKindT.Bit.Layers == eLayersSingle)
#if (DVD_RAM_READ == 1)
             && (!DVDRAM)
#endif
            ) || (layer == 1))
        {
            svoCalVar.calGainDoneBits |= teg_end;
        }
    }
    else
    {
#if(ERROR_CAL_START_UP_MONITOR ==1)
        svoMiscVar.calibrationErrorCount++;
#endif
        SendMsgCnB(SHOW_DEBUG_MSG,4,0x3A0001,B1B(ret),B1B(((svoIramVar.svoInState & STAT_MASK) >> 12)),B1B(((svoIramVar.svoInState & TRN_MASK ) >> 5) + 1),B1B((svoIramVar.svoInState & SEQ_MASK) + 1));
    }

    if ((svoIramVar.svoInState & TRN_MASK) == TRN_034)
    {
        SendMsgCn(DEBUG_CALIB_RESULT_MSG,4,0x310526+layer,svoCalGnT1Var[layer][TE_GN].groove_gain[0],svoCalGnT1Var[layer][TE_GN].groove_gain[1],svoCalOfsT1Var[layer][TE_OS].groove_offset[0],svoCalOfsT1Var[layer][TE_OS].groove_offset[1]);
    }

    return(SEQ_EX0);
}

/*-----------------------------------------------------------------------------
 *
 * Module: state_te_stepsize_meas()
 * Description: measure TE_step_size[]
 *
 *----------------------------------------------------------------------------*/
seq_exit_t state_te_stepsize_meas(void)
{
#if (DVD_RAM_READ == 1)
    // Force LAND to low (a) to use GROOVE gain register
    //                   (b) to prevent from TE polarity change
    if (DVDRAM)
    {
        WRITE_FIELD(HAL_AFE_LANDSEL,0);
    }
#endif

    calCalibrateStepsize(TE_OS,
                         TE_GN,
                         TYPE1,
                         0x1F,
                         TE_step_size);

#if (DVD_RAM_READ == 1)
    // Restore LAND control.
    if (DVDRAM)
    {
        WRITE_FIELD(HAL_AFE_LANDSEL,1);
    }
#endif
    return(SEQ_EX0);
}

/*-----------------------------------------------------------------------------
 *
 * Module: state_chk_audio()
 * Description:
 *
 *----------------------------------------------------------------------------*/
seq_exit_t state_chk_audio(void)
{
    long    work;

    svoStepVar.SledCounter = 0;
    return(SEQ_EX1);    //Disable this function, directly return(SEQ_EX1), to fix a pause release bug

    if(HAL_GLOBAL_GetRfPresent() == OFF){ /* blank disc */
        return(SEQ_EX1);
    }
    if(GetSubQCont() == 0x00){  /* audio track */
        return(SEQ_EX1);
    }

    work = svoSeekVar.svoTargetID.SectorNum.all32s - svoIramVar.currentID.SectorNum.all32s;
    if(work < 5L){  /* pause address - actual address < 5frames */
        return(SEQ_EX1);
    }
    svoCmdVar.commandedTrksToGo = -1;   /* 1track back */
    return(SEQ_EX0);
}

/*-----------------------------------------------------------------------------
 *
 * Module: state_lens_shift_cali()
 * Description: AFE TROOS calibration. Adjust TROOS to cancel radial beam landing
 *              AFE TEBG calibration. Adjust TEBG to balance TE signal (DPP only)
 *              AFE CEGN calibration. Adjust CEGN to meet target CE sensitivity
 *
 *----------------------------------------------------------------------------*/
seq_exit_t state_lens_shift_cali(void)
{
    errcode_t ret;
    USHORT    ce_sen_mv_mm;
    BYTE      cal_en;
    SBYTE     gain_adj;
#if (OPU_BEAM_LANDING_MEAS == 1)
    ad_dsp_t  signal_ret;
    USHORT    a_mV,b_mV,c_mV,d_mV;
    BYTE      rad_beamlanding,tan_beamlanding;
#endif

    SendMsg80(DEBUG_CALIB_ENTRYS_MSG,0x3A0111); // >>> STATE_LENS_SHIFT_CALI <<<
    cal_en = 0;

    if ((svoVar_TrackingMethod == TRK_METHOD_DPP) && ((svoCalVar.calGainDoneBits & tebg_end) == 0))
    {
#if (BARCODE_APPKt == 1)//set TEBG based on BARCODE
        if (((READ_REG(HAL_AFEREG_MAP->MB16) & (HAL_AFE_MPPAH|HAL_AFE_MPPBG)) == (HAL_AFE_MPPAH|HAL_AFE_MPPBG)) &&
            ((READ_REG(HAL_AFEREG_MAP->MB15) & (HAL_AFE_MPPA|HAL_AFE_MPPB|HAL_AFE_MPPC|HAL_AFE_MPPD)) == (HAL_AFE_MPPA|HAL_AFE_MPPB)))
        {
            // TEBG = APPKt*km/ks
            //send_msg74(SHOW_DEBUG_MSG,0xee3300,CalculateRateFromXdB(ONEdB,(COEF_GetValue(AFE_MB15_MPPOPGN)-1)*6));
            //send_msg74(SHOW_DEBUG_MSG,0xee3301,CalculateRateFromXdB(ONEdB,(COEF_GetValue(AFE_SB02_SPP1PGNA)-1)*6));
            //tebgtemp = OPUBarcode.BD_APPK_X10*CalculateRateFromXdB(ONEdB,(COEF_GetValue(AFE_MB15_MPPOPGN)-1)*6)/CalculateRateFromXdB(ONEdB,(COEF_GetValue(AFE_SB02_SPP1PGNA)-1)*6)/10;
            //send_msg74(SHOW_DEBUG_MSG,0xee3302,tebgtemp);
            gain_adj = CalculateXdBFromRate(P50dB, OPUBarcode.BD_APPK_X10, 10) + (6 * (COEF_GetValue(AFE_MB15_MPPOPGN) - COEF_GetValue(AFE_SB02_SPP1PGNA))) + 0x10;
            calSetAfeGain(Layer_Index,TEB_GN,TYPE3,0,0,gain_adj);
//            send_msg81(SHOW_DEBUG_MSG,0x51060D,gain_adj);
            svoCalVar.calGainDoneBits |= tebg_end;
        }
        else
#endif
        {
            cal_en |= CALIB_TEBG;
        }
    }

    if ((svoCalVar.calOfsDoneBits & troof_end) == 0)
    {
        cal_en |= CALIB_TROOS;
    }
    else
    {
        WRITE_REG(HAL_SRVREG_MAP->TROFFSET,svoCalMisc2Var.troos);
    }

    if ((Layer_Index == 0) && ((svoCalVar.calGainDoneBits & ceg_end) == 0))
    {
        cal_en |= CALIB_CEGN;
    }

    if (cal_en == 0)
    {
        //send_msg80(DEBUG_CALIB_ENTRYS_MSG,0x5A0002); // SKIP
        return(SEQ_EX0);
    }

    HAL_SERVO_CenterControlOFF();
    WaitTimer(5);
#if (OPU_BEAM_LANDING_MEAS == 1)
    if (Layer_Index == 0)
    {
        signal_ret = readSegmentABCD(ONE_REVOLUTION,TRUE);
        if (signal_ret.errcode == eNOERR)
        {
            a_mV = svoBeamLandingVar.segment_ref_mv[0] - signal_ret.middle_mv;//A
            b_mV = svoBeamLandingVar.segment_ref_mv[1] - signal_ret.p2p_mv;   //B
            c_mV = svoBeamLandingVar.segment_ref_mv[2] - signal_ret.top_mv;   //C
            d_mV = svoBeamLandingVar.segment_ref_mv[3] - signal_ret.bot_mv;   //D

            //Segment meas(mV), SAN:%3d, SBN:%3d, SCN:%3d, SDN:%3d
            SendMsgCn(DEBUG_AUTO_TUNING_MSG,8,0x338515,A2B(a_mV),A2B(b_mV),A2B(c_mV),A2B(d_mV));

            //radial BEAM LANDING = ((A+D)-(B+C))/(A+B+C+D)
            rad_beamlanding = RoundDivide(((a_mV+d_mV)-(b_mV+c_mV))*100,a_mV+b_mV+c_mV+d_mV);
            //tangential BEAM LANDING = ((A+B)-(C+D))/(A+B+C+D)
            tan_beamlanding = RoundDivide(((a_mV+b_mV)-(c_mV+d_mV))*100,a_mV+b_mV+c_mV+d_mV);
            SendMsgCn(SHOW_DEBUG_MSG,2,0x338516,A1B(rad_beamlanding),A1B(tan_beamlanding));//Beam Landing meas, rad:??%, tan:??%
        }
    }
#endif
    ret = calCalibrateTEBGTROOS(vref_fm_mux3_mV, &ce_sen_mv_mm, cal_en);

    if (ret == eNOERR)
    {
        if (cal_en & CALIB_TROOS)
        {
            svoCalVar.calOfsDoneBits |= troof_end;
#if (BD_ENABLE == 1)
            if (svoVar_discKindT.Bit.DiscStd == eDiscBD)
            {
                gain_adj = UM_2_SLED_STEP(((SBYTE)last_cal_result2 - svoCalMisc2Var.troos) * BD_HD_TRO_SENSITIVITY);
            }
            else
#endif
            {
                gain_adj = UM_2_SLED_STEP(((SBYTE)last_cal_result2 - svoCalMisc2Var.troos) * CD_DVD_TRO_SENSITIVITY);
            }
#if (TRO_INVERSION == 1)
            svoStepVar.StepHomePos -= gain_adj;
    #if (OPEN_SESSION == 1)
            Recorded_stp_Position[0] -= gain_adj;
            Recorded_stp_Position[1] -= gain_adj;
    #endif
#else
            svoStepVar.StepHomePos += gain_adj;
    #if (OPEN_SESSION == 1)
            Recorded_stp_Position[0] += gain_adj;
            Recorded_stp_Position[1] += gain_adj;
    #endif
#endif
#if (NEW_MPPSPPTE_BG_CALI == 1)
            gain_adj = (SBYTE)last_cal_result2 / TROOS_MPPSPPBG_FACTOR;
            if (gain_adj != 0)
            {
                if (READ_FIELD(HAL_AFE_CEINV) == 0)
                {
                    gain_adj = -gain_adj;
                }
                calSetAfeGain(Layer_Index,MPPB_GN,TYPE3,0,0,svoCalGnT3Var[Layer_Index][MPPB_GN].normal_gain + gain_adj);
                calSetAfeGain(Layer_Index,SPP1B_GN,TYPE3,0,0,svoCalGnT3Var[Layer_Index][SPP1B_GN].normal_gain + gain_adj);
                calSetAfeOffset(Layer_Index,MPP_OS,TYPE2,PATH_SELECT_ALL,0,calSetBalGain(MPPB_GN, svoCalGnT3Var[Layer_Index][MPPB_GN].normal_gain));
                calSetAfeOffset(Layer_Index,SPP1_OS,TYPE2,PATH_SELECT_ALL,0,calSetBalGain(SPP1B_GN, svoCalGnT3Var[Layer_Index][SPP1B_GN].normal_gain));
                if (cal_en & CALIB_CEGN)
                {
                    ce_sen_mv_mm = ce_sen_mv_mm * CalculateRateFromXdB(P10dB,gain_adj * 2.5);
                }
            }
#endif
        }
        if (cal_en & CALIB_TEBG)
        {
            if ((svoCalVar.calGainDoneBits & msppbg_end) == msppbg_end)
            {
                SendMsgCnB(SHOW_DEBUG_MSG,12,0x3166B6,F4B((float)(svoCalGnT3Var[Layer_Index][TEB_GN].normal_gain-16)/2.0),
                    F4B((float)(svoCalGnT3Var[Layer_Index][MPPB_GN].normal_gain-16)/2.0),F4B((float)(svoCalGnT3Var[Layer_Index][SPP1B_GN].normal_gain-16)/2.0));
            }
            if ((svoVar_discKindT.Bit.Layers == eLayersSingle) || (Layer_Index == 1))
            { /*dual layer disc layer 1 or signal layer disc*/
                svoCalVar.calGainDoneBits |= tebg_end;
            }
        }
    }
    else
    {
#if(ERROR_CAL_START_UP_MONITOR ==1)
        svoMiscVar.calibrationErrorCount++;
#endif
        SendMsgCnB(SHOW_DEBUG_MSG,4,0x3A0001,B1B(ret),B1B(((svoIramVar.svoInState & STAT_MASK) >> 12)),B1B(((svoIramVar.svoInState & TRN_MASK ) >> 5) + 1),B1B((svoIramVar.svoInState & SEQ_MASK) + 1));
        if (++cal_retry < 3)
        {
            return(SEQ_BUSY);
        }
    }
    cal_retry = 0;

    if (cal_en & CALIB_TROOS)
    {
        calSetMisc(0,TRO_OS,(SBYTE)last_cal_result2);
    }

    if (cal_en & CALIB_TEBG)
    {
        calSetAfeGain(Layer_Index,TEB_GN,TYPE3,0,0,last_cal_result);
        SendMsgCnB(SHOW_DEBUG_MSG,4,0x3166B7+Layer_Index,F4B((float)(svoCalGnT3Var[Layer_Index][TEB_GN].normal_gain-16)/2.0));
    }

    if (cal_en & CALIB_CEGN)
    {
        //adjust CE gain
        gain_adj = CalculateXdBFromRate(ONEdB, CE_SEN_TARGET_MV_MM, ce_sen_mv_mm);
        gain_adj += READ_FIELD(HAL_AFE_CEGN);
        calSetAfeGain(Layer_Index,CE_GN,TYPE3,0,0,gain_adj);
        SendMsgCnB(SHOW_DEBUG_MSG,5,0x3166B1,F4B((float)(svoCalGnT3Var[Layer_Index][CEB_GN].normal_gain-16)/2.0),B1B(svoCalGnT3Var[Layer_Index][CE_GN].normal_gain-16));
    }

    WRITE_DSP_COEF(ktg, 0x4000);             /* Ktg output disable */
    return(SEQ_EX0);                      // use push-pull servo for cd_dpd disc. done sucessfully
}// end of state_lens_shift_cali

#if (MPPBG_SPPBG_CALIB == 2)
/*-----------------------------------------------------------------------------
 *
 * Module: state_mpp_sppbg_cali()
 * Description: AFE MPP SPP1 balance gain calibration. Adjust MPPBG & SPP1BG to balance MPP and SPP1
 *
 *----------------------------------------------------------------------------*/
//#define TROOS_LIMIT_CDDVD ((SHORT)((TROOS_LENS_LIMIT_CDDVD + (CD_DVD_TRO_SENSITIVITY / 2)) / CD_DVD_TRO_SENSITIVITY))
seq_exit_t state_mpp_sppbg_cali(void)
{
    errcode_t ret;
    SBYTE     i;

    if (svoIramVar.svoPreInState != svoIramVar.svoInState)
    {
        if ((Layer_Index == 1) || ((svoCalVar.calGainDoneBits & msppbg_end) == msppbg_end))
        {
            return(SEQ_EX0);
        }
        WaitTimer(100);
        SendMsg80(DEBUG_CALIB_ENTRYS_MSG,0x3A0110); // >>> STATE_MSPPBG_CALI <<<
    }
#if (NEW_MPPSPPTE_BG_CALI == 1) && (MPPBG_SPPBG_CALIB != 0)
    #if 0 //debug code to measure TROOS_MPPSPPBG_FACTOR
    for (i = -1; i >= -TROOS_LIMIT_CDDVD; i--)
    {
        WRITE_FIELD(HAL_SRV_TROFFSET,i);
        WaitTimer(10);
    }
    svoCurvefitCalVar.cal_index = 0;
    for (i = -TROOS_LIMIT_CDDVD; i <= TROOS_LIMIT_CDDVD; i++)
    {
        WRITE_FIELD(HAL_SRV_TROFFSET,i);
        WaitTimer(50);
        if (ABS(i) > TROOS_LIMIT_CDDVD - 9)
        {
            ret = calCalibrateMPPSPP1BG(vref_fm_mux3_mV);
            svoCurvefitCalVar.item[svoCurvefitCalVar.cal_index] = last_cal_result + last_cal_result2;
            svoCurvefitCalVar.cost[svoCurvefitCalVar.cal_index] = i;
            svoCurvefitCalVar.cal_index++;
        }
    }
    for (i = TROOS_LIMIT_CDDVD - 1; i > 0; i--)
    {
        WRITE_FIELD(HAL_SRV_TROFFSET,i);
        WaitTimer(10);
    }
    WRITE_FIELD(HAL_SRV_TROFFSET,i);
    LineFit(svoCurvefitCalVar.cal_index, svoCurvefitCalVar.item, svoCurvefitCalVar.cost, svoCurvefitCalVar.cof);
    if (READ_FIELD(HAL_AFE_CEINV) == 0)
    {
        svoCurvefitCalVar.cof[1] = -svoCurvefitCalVar.cof[1];
    }
    SendMsgCnB(SHOW_DEBGU_MSG,4,0x3166BC,F4B(svoCurvefitCalVar.cof[1] * 2));//TROOS_MPPSPPBG_FACTOR = %2.1f
    #endif
#endif
    ret = calCalibrateMPPSPP1BG(vref_fm_mux3_mV);
    if (ret != eNOERR)
    {
#if(ERROR_CAL_START_UP_MONITOR ==1)
        svoMiscVar.calibrationErrorCount++;
#endif
        SendMsgCnB(SHOW_DEBUG_MSG,4,0x3A0001,B1B(ret),B1B(((svoIramVar.svoInState & STAT_MASK) >> 12)),B1B(((svoIramVar.svoInState & TRN_MASK ) >> 5) + 1),B1B((svoIramVar.svoInState & SEQ_MASK) + 1));
        if (++cal_retry < 3)
        {
            return(SEQ_BUSY);
        }
    }
    cal_retry = 0;
    svoCalVar.calGainDoneBits |= msppbg_end;

    if (last_cal_result < 0x10 - MPPBG_MAX_UNB)
    {
        SendMsgCn(SHOW_DEBUG_MSG,1,0x3166BA,last_cal_result - 0x10);//WARNING: MPPBG out of limit: %1d
        last_cal_result = 0x10 - MPPBG_MAX_UNB;
    }
    if (last_cal_result > 0x10 + MPPBG_MAX_UNB)
    {
        SendMsgCn(SHOW_DEBUG_MSG,1,0x3166BA,last_cal_result - 0x10);//WARNING: MPPBG out of limit: %1d
        last_cal_result = 0x10 + MPPBG_MAX_UNB;
    }
    if (last_cal_result2 < 0x10 - SPP1BG_MAX_UNB)
    {
        SendMsgCn(SHOW_DEBUG_MSG,1,0x3166BB,last_cal_result2 - 0x10);//WARNING: SPP1BG out of limit: %1d
        last_cal_result2 = 0x10 - SPP1BG_MAX_UNB;
    }
    if (last_cal_result2 > 0x10 + SPP1BG_MAX_UNB)
    {
        SendMsgCn(SHOW_DEBUG_MSG,1,0x3166BB,last_cal_result2 - 0x10);//WARNING: SPP1BG out of limit: %1d
        last_cal_result2 = 0x10 + SPP1BG_MAX_UNB;
    }

    if ((READ_FIELD(HAL_AFE_FESP1) == 0) && (READ_FIELD(HAL_AFE_TESP1) == 0) && (READ_FIELD(HAL_AFE_CESP1) == 0))
    {//SPP1 is not used, skip SPP1BG calibration result.
        last_cal_result2 = 0x10;
    }

    calSetAfeGain(Layer_Index,MPPB_GN,TYPE3,0,0,last_cal_result);
    calSetAfeGain(Layer_Index,SPP1B_GN,TYPE3,0,0,last_cal_result2);
    calSetAfeOffset(Layer_Index,MPP_OS,TYPE2,PATH_SELECT_ALL,0,calSetBalGain(MPPB_GN, last_cal_result));
    calSetAfeOffset(Layer_Index,SPP1_OS,TYPE2,PATH_SELECT_ALL,0,calSetBalGain(SPP1B_GN, last_cal_result2));
    //adapt TEBG, CEBG, CEGN
    i = (last_cal_result - last_cal_result2) / 2;
    if (i != 0)
    {
        if (svoVar_TrackingMethod != TRK_METHOD_DPD)
        {
            calSetAfeGain(Layer_Index,TEB_GN,TYPE3,0,0,svoCalGnT3Var[Layer_Index][TEB_GN].normal_gain + i);
        }
        calSetAfeGain(Layer_Index,CEB_GN,TYPE3,0,0,svoCalGnT3Var[Layer_Index][CEB_GN].normal_gain + i);
    }
    i = (last_cal_result - 0x10) / 4;
    if (i != 0)
    {
        calSetAfeGain(Layer_Index,CE_GN,TYPE3,0,0,svoCalGnT3Var[Layer_Index][CE_GN].normal_gain - i);
    }

    SendMsgCnB(SHOW_DEBUG_MSG,12,0x3166B6,F4B((float)(svoCalGnT3Var[Layer_Index][TEB_GN].normal_gain-16)/2.0),
        F4B((float)(svoCalGnT3Var[Layer_Index][MPPB_GN].normal_gain-16)/2.0),F4B((float)(svoCalGnT3Var[Layer_Index][SPP1B_GN].normal_gain-16)/2.0));
#if (NEW_MPPSPPTE_BG_CALI == 0)
    SendMsgCnB(SHOW_DEBUG_MSG,5,0x3166B1,F4B((float)(svoCalGnT3Var[Layer_Index][CEB_GN].normal_gain-16)/2.0),B1B(svoCalGnT3Var[Layer_Index][CE_GN].normal_gain-16));

    //if MPP,SPP1BG is out of +-2dB, recalibrate TEBG and CEGN
    if ((last_cal_result < (0x10 - 4)) || (last_cal_result > (0x10 + 4)) || (last_cal_result2 < (0x10 - 4)) || (last_cal_result2 > (0x10 + 4)))
    {
        svoCalVar.calGainDoneBits &= (~tebg_end);
        return SEQ_EX1;
    }
#endif
    return(SEQ_EX0);
}
#elif (MPPBG_SPPBG_CALIB == 1)
/*-----------------------------------------------------------------------------
 *
 * Module: state_mpp_sppbg_cali()
 * Description: AFE MPP SPP1 balance gain calibration. Adjust MPPBG & SPP1BG to balance TE signal!
 *
 *----------------------------------------------------------------------------*/
#define TRKG_STEP 2    //unit dB
#define MAX_MSPPBG_LOOP 10
seq_exit_t state_mpp_sppbg_cali(void)
{
    SBYTE    i,j;
    BYTE     k,signal;
    ad_dsp_t te_signal;
    USHORT   target_mid;
    BYTE     use_Bits;
    SBYTE    orig_TRKG;

    if ((Layer_Index == 1) || (svoVar_TrackingMethod == TRK_METHOD_DPD) || ((svoCalVar.calGainDoneBits & msppbg_end) == msppbg_end))
    {
        return(SEQ_EX0);
    }

    SendMsg80(DEBUG_CALIB_ENTRYS_MSG,0x3A0110); // >>> STATE_MSPPBG_CALI <<<

    target_mid = vref_fm_mux3_mV;
    orig_TRKG = j = READ_FIELD(HAL_AFE_TRKG);

    for (k = 0; k < MAX_MSPPBG_LOOP; k++)
    {
        for (i = 0; i < 3; i++)
        {
            calSetBalGain(MPPB_GN, svoCalGnT3Var[0][MPPB_GN].normal_gain + MPPBG_MAX_UNB * (i - 1));
            calSetBalGain(SPP1B_GN, svoCalGnT3Var[0][SPP1B_GN].normal_gain - MPPBG_MAX_UNB * (i - 1));
            WRITE_FIELD(HAL_AFE_TEBG, svoCalGnT3Var[0][TEB_GN].normal_gain + MPPBG_MAX_UNB * (i - 1));
            DelaymS(1);
            te_signal = ReadFilteredDSPADCmV(ONE_REVOLUTION, TE_ALL, ADC_TE, TRUE);

            if ((te_signal.top_saturated == TRUE) || (te_signal.bot_saturated == TRUE))
            {
                SendMsg80(SHOW_DEBUG_MSG,0x310610); // WARNING: TE Saturated
                if (j > 0)
                {
                    j -= TRKG_STEP;
                    if (j < 0) j = 0;
                    WRITE_FIELD(HAL_AFE_TRKG,j);
                    break;
                }
            }
            svoCurvefitCalVar.cost[i] = te_signal.middle_mv;
            svoCurvefitCalVar.item[i] = MPPBG_MAX_UNB * (i - 1);
        }
        if (i == 3) //break measure loop
        {
            break;
        }
    }

    if (k >= MAX_MSPPBG_LOOP)
    {
        SendMsg80(DEBUG_AUTO_TUNING_MSG,0x316429); //Out of calibrate loop
    }

    // restore the TRKG
    WRITE_FIELD(HAL_AFE_TRKG,orig_TRKG);

    LineFit(3, svoCurvefitCalVar.item, svoCurvefitCalVar.cost, svoCurvefitCalVar.cof);
    //send_msg74(1,0x020303,(float)target_mid);
    //send_msg74(1,0x020304,((float)target_mid - svoCurvefitCalVar.cof[0]) / svoCurvefitCalVar.cof[1]);
    i = float2short(((float)target_mid - svoCurvefitCalVar.cof[0]) / svoCurvefitCalVar.cof[1]);
    if (i > MPPBG_MAX_UNB)
    {
        SendMsgCn(SHOW_DEBUG_MSG,1,0x3166BA,i);
        i = MPPBG_MAX_UNB;
    }
    if (i < -MPPBG_MAX_UNB)
    {
        SendMsgCn(SHOW_DEBUG_MSG,1,0x3166BA,i);
        i = -MPPBG_MAX_UNB;
    }

    calSetAfeGain(Layer_Index,MPPB_GN,TYPE3,0,0,svoCalGnT3Var[0][MPPB_GN].normal_gain + i);
    calSetAfeOffset(Layer_Index,MPP_OS,TYPE2,PATH_SELECT_ALL,0,calSetBalGain(MPPB_GN, svoCalGnT3Var[0][MPPB_GN].normal_gain));
    calSetAfeGain(Layer_Index,SPP1B_GN,TYPE3,0,0,svoCalGnT3Var[0][SPP1B_GN].normal_gain - i);
    calSetAfeOffset(Layer_Index,SPP1_OS,TYPE2,PATH_SELECT_ALL,0,calSetBalGain(SPP1B_GN, svoCalGnT3Var[0][SPP1B_GN].normal_gain));
    calSetAfeGain(Layer_Index,TEB_GN,TYPE3,0,0,svoCalGnT3Var[0][TEB_GN].normal_gain + i);
    calSetAfeGain(Layer_Index,CEB_GN,TYPE3,0,0,svoCalGnT3Var[0][CEB_GN].normal_gain + i);

    SendMsgCnB(SHOW_DEBUG_MSG,12,0x3166B6,F4B((float)(svoCalGnT3Var[Layer_Index][TEB_GN].normal_gain-16)/2.0),
        F4B((float)(svoCalGnT3Var[Layer_Index][MPPB_GN].normal_gain-16)/2.0),F4B((float)(svoCalGnT3Var[Layer_Index][SPP1B_GN].normal_gain-16)/2.0));

    svoCalVar.calGainDoneBits |= msppbg_end;

    return(SEQ_EX0);
}// end of state_mpp_sppbg_cali
#endif

/*-----------------------------------------------------------------------------
 *
 * Module: state_CEBG_cali()
 * Description: calibrate CEB_GN, find CEB_GN with minimum TE component
 *
 *----------------------------------------------------------------------------*/
seq_exit_t state_CEBG_cali(void)
{
    errcode_t   ret;

    if ((svoCalVar.calGainDoneBits & cebg_end) == cebg_end)
    {
        return(SEQ_EX0);
    }

    if ((READ_FIELD(HAL_AFE_CESP1) == 0) || (READ_FIELD(HAL_AFE_CEMPP) == 0)
#if (APP_TE_SUPPORT == 1)
     || (((READ_REG(HAL_AFEREG_MAP->MB16) & (HAL_AFE_MPPAH|HAL_AFE_MPPBG)) == (HAL_AFE_MPPAH|HAL_AFE_MPPBG)) &&
         ((READ_REG(HAL_AFEREG_MAP->MB15) & (HAL_AFE_MPPA|HAL_AFE_MPPB|HAL_AFE_MPPC|HAL_AFE_MPPD)) == (HAL_AFE_MPPA|HAL_AFE_MPPB)) &&
         ((READ_REG(HAL_AFEREG_MAP->SB14) & (HAL_AFE_SPP1E|HAL_AFE_SPP1F|HAL_AFE_SPP1G|HAL_AFE_SPP1H)) == (HAL_AFE_SPP1E|HAL_AFE_SPP1F)))
#endif
       )
    {
        svoCalVar.calGainDoneBits |= cebg_end;
        return(SEQ_EX0);
    }
    SendMsg80(DEBUG_CALIB_ENTRYS_MSG,0x3A0129); // >>> STATE_CEBG_CALI <<<
    ret = calCalibrateCEBG();

    if (ret == eNOERR)
    {
        svoCalVar.calGainDoneBits |= cebg_end;
    }
    else
    {
#if(ERROR_CAL_START_UP_MONITOR ==1)
        svoMiscVar.calibrationErrorCount++;
#endif
        SendMsgCnB(SHOW_DEBUG_MSG,4,0x3A0001,B1B(ret),B1B(((svoIramVar.svoInState & STAT_MASK) >> 12)),B1B(((svoIramVar.svoInState & TRN_MASK ) >> 5) + 1),B1B((svoIramVar.svoInState & SEQ_MASK) + 1));
        if (++cal_retry < 3)
        {
            return(SEQ_BUSY);
        }
    }
    cal_retry = 0;

    calSetAfeGain(Layer_Index,CEB_GN,TYPE3,0,0,last_cal_result);
    return(SEQ_EX0);
}

/*-----------------------------------------------------------------------------
 *
 * Module: state_CEOS_cali()
 * Description: calibrate CE_OS
 *
 *----------------------------------------------------------------------------*/
seq_exit_t state_CEOS_cali(void)
{
    errcode_t   ret;

    if ((svoCalVar.calGainDoneBits & ceg_end) == ceg_end)
    {
        return(SEQ_EX0);
    }

    SendMsg80(DEBUG_CALIB_ENTRYS_MSG,0x3A0127); // >>> STATE_CEOS_CALI <<<

    ret = calCalibrateCEOS();

    if (ret == eNOERR)
    {
        if ((svoVar_discKindT.Bit.Layers == eLayersSingle) || (Layer_Index == 1))
        {
            svoCalVar.calGainDoneBits |= ceg_end;
        }
    }
    else
    {
#if(ERROR_CAL_START_UP_MONITOR ==1)
        svoMiscVar.calibrationErrorCount++;
#endif
        SendMsgCnB(SHOW_DEBUG_MSG,4,0x3A0001,B1B(ret),B1B(((svoIramVar.svoInState & STAT_MASK) >> 12)),B1B(((svoIramVar.svoInState & TRN_MASK ) >> 5) + 1),B1B((svoIramVar.svoInState & SEQ_MASK) + 1));
        if (++cal_retry < 3)
        {
            return(SEQ_BUSY);
        }
    }
    cal_retry = 0;

    calSetAfeOffset(Layer_Index,CE_OS,TYPE3,0,0,last_cal_result);

    SendMsgCn(DEBUG_CALIB_RESULT_MSG,2,0x310524+Layer_Index,svoCalGnT3Var[Layer_Index][CE_GN].normal_gain,svoCalOfsT3Var[Layer_Index][CE_OS].normal_offset);
    return(SEQ_EX0);
}

/*-----------------------------------------------------------------------------
 *
 * Module: state_vcalc_init()
 * Description: prepare scanning velocity (svoSeekVar.parFine) calibration for CD
 *
 *----------------------------------------------------------------------------*/
seq_exit_t state_vcalc_init(void)
{
    USHORT current_radius;
#if (DETECT_DVD_CLV_TRACK_PITCH == 1)
    USHORT ParFine;
#endif

    if ((svoCalVar.discChkDoneBits & velo_chk_end) == velo_chk_end)
    {
        return(SEQ_EX1);
    }

    if (svoVar_discKindT.Bit.DiscStd == eDiscCD)
    {
        svoIramVar.subQEnableFlag = 0;

        SendMsg80(DEBUG_DISK_ID_MSG,0x310201); //***Linear_Velocity_Calibration_START***
        SendMsgCn(DEBUG_DISK_ID_MSG,3,0x31020C,(BYTE)svoIramVar.currentTime.Time.Minute,(BYTE)svoIramVar.currentTime.Time.Second,(BYTE)svoIramVar.currentTime.Time.Frame);                                           //***Linear_Velocity_Calibration_START***
        return(SEQ_EX0);
    }
    else
    {
        current_radius = svoPBA2Radius(svoIramVar.currentID.SectorNum.all32s);
#if (DETECT_DVD_CLV_TRACK_PITCH == 1)
        if ((svoVar_discKindT.Bit.DiscStd == eDiscDVD) && (svoVar_discKindT.Bit.Media == eMediaROM))
        {
            ParFine = svo_get_parFine_from_pll(current_radius);
            if (ParFine * 100 < svoSeekVar.parFineInit * 95)
            {
//                send_msgA4(DEBUG_DISK_ID_MSG,0x510218,ParFine);
                svoSeekVar.parFineInit = svoSeekVar.parFine = ParFine;
                SendMsg80(DEBUG_DISK_ID_MSG,0x310201); //***Linear_Velocity_Calibration_START***
                return(SEQ_EX0);
            }
        }
#endif
    }
    SendMsgCn(DEBUG_DISK_ID_MSG,4,0x310219,A2B(svoSeekVar.parFine),A2B(svoSeekVar.parRough));
#if (OPU_HomeSW > 0)
    Iram_stp_CurrentPosition = STEP_INWARD_POSITION + current_radius - Iram_stp_CurrentPosition;
    SendMsgCn(DEBUG_DISK_ID_MSG,2,0x31021C,A2B(Iram_stp_CurrentPosition));
#else // OPU_HomeSW
    Iram_stp_CurrentPosition = STEP_OUTWARD_POSITION + current_radius - Iram_stp_CurrentPosition;
    SendMsgCn(DEBUG_DISK_ID_MSG,2,0x31021D,A2B(Iram_stp_CurrentPosition));
#endif // OPU_HomeSW
    svoStp_SetCurrentPosition(current_radius);
    svoStepVar.StepPosCal = TRUE;
    svoCalVar.discChkDoneBits |= velo_chk_end;
    return(SEQ_EX1);
}

/*-----------------------------------------------------------------------------
 *
 * Module: state_velocity_meas()
 * Description: measure scanning velocity
 *
 *----------------------------------------------------------------------------*/
seq_exit_t state_velocity_meas(void)
{
    float   velocity;
    USHORT  current_radius;
    LONG    current_PBA;
    BYTE    temp_IE;
    ULONG   fg_pulse_sum, fg_pulse_rev, pll_freq, pll_sample, PLL_adr;
    USHORT  rpm,fg_pulse[DISK_NFGPLS],pre_fg_count;

    fg_pulse_sum = fg_pulse_rev = pll_freq = pll_sample = 0;
    //radius[m] = radius[step] * Step
    //velocity[m/sec] = 2 * PI * radius[m] * motor_freq(rev/sec) / overspeed
    //parFine         = 16 * Vel / (75 * PI * Step)

    //HF
    //CDoverspeed  = HAL_PLL_GetDfeInClkKHz() * HAL_DFE_INTEGRAT * 1000 / (8192 * CD_BITRATE_N1)
    //DVDoverspeed = HAL_PLL_GetDfeInClkKHz() * HAL_DFE_INTEGRAT * 1000 / (8192 * DVD_BITRATE_N1)
    //Wobble
    //CDoverspeed   = HAL_PLL_GetWblClkKHz() * HAL_WBL_ATPLLFRQ * 1000 / (262144 * CD_WBL_FREQ_N1)
    //DVD+overspeed = HAL_PLL_GetWblClkKHz() * HAL_WBL_ATPLLFRQ * 1000 / (524288 * DVD_PR_PRW_WBL_FREQ_N1)
    //DVD-overspeed = HAL_PLL_GetWblClkKHz() * HAL_WBL_ATPLLFRQ * 1000 / (3047424 * DVD_MR_MRW_WBL_FREQ_N1)

    //HAL_DFE_INTEGRAT (or HAL_WBL_ATPLLFRQ) = pll_freq
    //velocity[m/sec] = 2 * PI * radius[m] * motor_freq(rev/sec) / overspeed
    //                = 2 * PI * radius[m] * rpm / 60(sec/min) / overspeed
    //                = 2 * PI * radius[m] * rpm * 8192 * CD_BITRATE_N1 / (60 * HAL_PLL_GetDfeInClkKHz() * HAL_DFE_INTEGRAT * 1000)
    //                = 2 * PI * radius[step] * Step * rpm * 8192 * CD_BITRATE_N1 / (60 * HAL_PLL_GetDfeInClkKHz() * HAL_DFE_INTEGRAT * 1000)
    //CD
    //parFine         = 16 * velocity / (75 * PI * Step)
    //                = 16 * 2 * PI * radius[step] * Step * rpm * 8192 * CD_BITRATE_N1 / (60 * HAL_PLL_GetDfeInClkKHz() * HAL_DFE_INTEGRAT * 1000 * 75 * PI * Step)
    //                = 16 * 2 * radius[step] * rpm * 8192 * CD_BITRATE_N1 / (60 * HAL_PLL_GetDfeInClkKHz() * HAL_DFE_INTEGRAT * 1000 * 75)
    //svoSeekVar.parFine = (current_radius * rpm * 8192 * CD_BITRATE_N1) / (15 * HAL_PLL_GetDfeInClkKHz() * HAL_DFE_INTEGRAT * 125 * 75)
    //svoSeekVar.parFine = (current_radius * rpm * 8192 * CD_BITRATE_N1) / (HAL_PLL_GetDfeInClkKHz() * pll_freq * 140625)

    //DVD
    //svoSeekVar.parFine = (current_radius * rpm * 8192 * DVD_BITRATE_N1) / (15 * HAL_PLL_GetDfeInClkKHz() * HAL_DFE_INTEGRAT * 125 * (DVD_BITRATE_N1 / 38688))
    //svoSeekVar.parFine = (current_radius * rpm * 8192 * 12896) / (HAL_PLL_GetDfeInClkKHz() * pll_freq * 625)
    //svoSeekVar.parFine = (current_radius * rpm * 169030) / (HAL_PLL_GetDfeInClkKHz() * pll_freq )

    PLL_adr = HAL_DFE_INTEGRAT_ADR;
#if (EN_WOBBLE == 1)
    if ((svoVar_discKindT.Bit.Media != eMediaROM) && (!HAL_DFE_GetPllInLock()))
    {
        switch (svoVar_discKindT.Bit.DiscStd)
        {
    #if (WOBBLE_READ_ON_DVD_OR_CD == 1)
            case eDiscCD:
                PLL_adr = HAL_WBL_ATPLLFRQ_ADR;
                break;

            case eDiscDVD:
    #endif
    #if (BD_ENABLE == 1)
            case eDiscBD:
    #endif
    #if (ENABLE_HDDVD == 1)
            case eDiscHD:
    #endif
                PLL_adr = HAL_WBL_WBLFREQ_ADR;
                break;
        }
    }
#endif
    svoSpeedVar.fgCount = 2 * DISK_NFGPLS;//Now calculate 2 revolutions due to possible bad FG duty
    pre_fg_count = svoSpeedVar.fgCount;
    svoSpeedVar.fgPulseTime = 0xFFFF;
    StartFGTimer();
    do
    {
        pll_sample++;
        pll_freq += READ_ADR_4B(PLL_adr);
        if (svoSpeedVar.fgCount != pre_fg_count)
        {
            pre_fg_count = svoSpeedVar.fgCount;
            if (pre_fg_count < DISK_NFGPLS)
            {
                fg_pulse_rev -= fg_pulse[pre_fg_count % DISK_NFGPLS];
            }
            fg_pulse[pre_fg_count % DISK_NFGPLS] = svoSpeedVar.fgPulseTime;
            fg_pulse_rev += svoSpeedVar.fgPulseTime;
            if (pre_fg_count <= DISK_NFGPLS)
            {
                fg_pulse_sum += fg_pulse_rev;
    }
        }
    }
    while (pre_fg_count && (pll_sample < 0xFFFFFF));

    temp_IE = GET_INT_ENABLE_MASK();    /* disable interrupts */
    DISABLE_INTERRUPT();

    current_PBA = svoIramVar.currentID.SectorNum.all32s;

    SET_INT_ENABLE_MASK(temp_IE);       /* restore interrupts */

    current_radius = svoPBA2Radius(current_PBA);

    pll_freq /= pll_sample;
    rpm = (SERVO_CLOCK * 60L / (DISK_FR2X + 1)) / (fg_pulse_sum * READ_REG(HAL_SRVREG_MAP->CAVM) / (DISK_NFGPLS + 1));

    switch (svoVar_discKindT.Bit.DiscStd)
    {
        case eDiscCD:
#if (WOBBLE_READ_ON_DVD_OR_CD == 1)
            if (PLL_adr != HAL_DFE_INTEGRAT_ADR)
            {
                svoSeekVar.parFine = ((float)current_radius * rpm * 262144 * CD_WBL_FREQ_N1) / ((float)HAL_PLL_GetWblClkKHz() * pll_freq * 140625);
            }
            else
#endif
            {
                svoSeekVar.parFine = ((float)current_radius * rpm * 8192 * CD_BITRATE_N1) / ((float)HAL_PLL_GetDfeInClkKHz() * pll_freq * 140625);
            }

            // 16 / (75 * PI) = 128 / 1885
            //minimum velocity = 1100 mm/s//15935
            if (svoSeekVar.parFine < ((ULONG)1280 * 1100 * MICRO_STEP_PER_ELE_REV * ELE_REV_PER_MACH_REV) / (1885 * DISTANCE_PER_MACH_REV / 100))
                svoSeekVar.parFine = ((ULONG)1280 * 1100 * MICRO_STEP_PER_ELE_REV * ELE_REV_PER_MACH_REV) / (1885 * DISTANCE_PER_MACH_REV / 100);
            //maximum velocity = 1500 mm/s//21730
            if (svoSeekVar.parFine > ((ULONG)1280 * 1500 * MICRO_STEP_PER_ELE_REV * ELE_REV_PER_MACH_REV) / (1885 * DISTANCE_PER_MACH_REV / 100))
                svoSeekVar.parFine = ((ULONG)1280 * 1500 * MICRO_STEP_PER_ELE_REV * ELE_REV_PER_MACH_REV) / (1885 * DISTANCE_PER_MACH_REV / 100);

            svoSeekVar.parFineInit = svoSeekVar.parFine;

            //    2 x BPS x PI x Sled_step x parFine
            //v = ----------------------------------
            //                   32
            velocity = (float)svoSeekVar.parFine * (float)(2.0 * 75 * PI * DISTANCE_PER_MACH_REV / (32000000.0 * MICRO_STEP_PER_ELE_REV * ELE_REV_PER_MACH_REV));

            //parRough = 4 * track_pitch * parFine / Sled_step
            //assume track_pitch = 1.4um
            svoSeekVar.parRough = (USHORT)((ULONG)(4 * 14 * MICRO_STEP_PER_ELE_REV * ELE_REV_PER_MACH_REV) * svoSeekVar.parFine / (10 * DISTANCE_PER_MACH_REV));
            svoSeekVar.parRoughInit  = svoSeekVar.parRough;
            break;

        case eDiscDVD:
#if (WOBBLE_READ_ON_DVD_OR_CD == 1)
            if (PLL_adr != HAL_DFE_INTEGRAT_ADR)
            {
#endif
            {
                svoSeekVar.parFine = ((float)current_radius * rpm * 169030) / ((float)HAL_PLL_GetDfeInClkKHz() * pll_freq );
            }

            // 16 / (676.08 * PI) = 4 / 531
            //minimum velocity = 3 m/s 4821
            if (svoSeekVar.parFine < ((ULONG)400 * 3000 * MICRO_STEP_PER_ELE_REV * ELE_REV_PER_MACH_REV) / (531 * DISTANCE_PER_MACH_REV / 10))
                svoSeekVar.parFine = ((ULONG)400 * 3000 * MICRO_STEP_PER_ELE_REV * ELE_REV_PER_MACH_REV) / (531 * DISTANCE_PER_MACH_REV / 10);
            //maximum velocity = 4 m/s 6428
            if (svoSeekVar.parFine > ((ULONG)400 * 4000 * MICRO_STEP_PER_ELE_REV * ELE_REV_PER_MACH_REV) / (531 * DISTANCE_PER_MACH_REV / 10))
                svoSeekVar.parFine = ((ULONG)400 * 4000 * MICRO_STEP_PER_ELE_REV * ELE_REV_PER_MACH_REV) / (531 * DISTANCE_PER_MACH_REV / 10);

            svoSeekVar.parFineInit = svoSeekVar.parFine;

            //    2 x BPS x PI x Sled_step x parFine
            //v = ----------------------------------
            //                   32
            velocity = (float)svoSeekVar.parFine * (float)(2.0 * 676.0817308 * PI * DISTANCE_PER_MACH_REV / (32000000.0 * MICRO_STEP_PER_ELE_REV * ELE_REV_PER_MACH_REV));

            //parRough = 4 * track_pitch * parFine / Sled_step
            //assume track_pitch = 0.74um
            svoSeekVar.parRough = (USHORT)((ULONG)(4 * 74 * MICRO_STEP_PER_ELE_REV * ELE_REV_PER_MACH_REV) * svoSeekVar.parFine / (100 * DISTANCE_PER_MACH_REV));
            svoSeekVar.parRoughInit  = svoSeekVar.parRough;
            break;
    }

#if (MSG_OUT_MODE != MSG_DISABLE)
    SendMsgCn(DEBUG_DISK_ID_MSG,4,0x310202,A4B(velocity));
    SendMsgCn(DEBUG_DISK_ID_MSG,4,0x310219,A2B(svoSeekVar.parFine),A2B(svoSeekVar.parRough));
    #if (OPU_HomeSW > 0)
    Iram_stp_CurrentPosition = STEP_INWARD_POSITION + current_radius - Iram_stp_CurrentPosition;
    SendMsgCn(DEBUG_DISK_ID_MSG,2,0x31021C,A2B(Iram_stp_CurrentPosition));
    #else // OPU_HomeSW
    Iram_stp_CurrentPosition = STEP_OUTWARD_POSITION + current_radius - Iram_stp_CurrentPosition;
    SendMsgCn(DEBUG_DISK_ID_MSG,2,0x31021D,A2B(Iram_stp_CurrentPosition));
    #endif // OPU_HomeSW
#endif // (MSG_OUT_MODE != MSG_DISABLE)
    svoStp_SetCurrentPosition(current_radius);
    svoStepVar.StepPosCal = TRUE;
    svoCalVar.discChkDoneBits |= velo_chk_end;

    //store address and sledge position for track pitch calculation
    svoCurvefitCalVar.item[17] = current_PBA - svoSeekVar.parF0;
    svoCurvefitCalVar.cost[17] = current_radius;
    svoCurvefitCalVar.cal_index = 18;

    return(SEQ_EX0);
}

/*-----------------------------------------------------------------------------
 *
 * Module: state_boundary_det()
 * Description: Lead in boundary detection for CDROM and prepare track pitch calibration for 12cm CD
 *
 *----------------------------------------------------------------------------*/
#define DENSITY_CALC_JUMP_DISTANCE_12CM  UM_2_SLED_STEP(9000)
#define DENSITY_CALC_JUMP_DISTANCE_8CM   UM_2_SLED_STEP(4000)
seq_exit_t state_boundary_det(void)
{
    LONG    current_PBA;
    BYTE    temp_IE;
    BYTE    LIBoundary_TimeOut;
    seq_exit_t ret_seq;

    svoIramVar.subQEnableFlag = 1;

    LIBoundary_TimeOut = FALSE;
    //if (((svoCalVar.calDoneBits & boundary_end) == boundary_end) || (svoVar_discKindT.fDiscKind != eCDROMDisc))
    if (((svoCalVar.calDoneBits & boundary_end) == boundary_end) || (svoVar_discKindT.fDiscKind == eCDRWDisc))//Because CDROM might be missjudged as CDR
    {
        svoCalVar.calDoneBits |= boundary_end;

        SendMsg80(DEBUG_DISK_ID_MSG,0x310214); //Don't Need Lead-in Boundary Detect!!!
        ret_seq = SEQ_EX0;
    }
    else
    {
        if (!svo_BoundaryDet)
        {
            SendMsg80(DEBUG_DISK_ID_MSG,0x31020F);  //Lead-in_Boundary_Detect_START
        }
        temp_IE = GET_INT_ENABLE_MASK();    /* disable interrupts */
        DISABLE_INTERRUPT();

        current_PBA = svoIramVar.currentID.SectorNum.all32s;

        SET_INT_ENABLE_MASK(temp_IE);       /* restore interrupts */

        if (current_PBA >= 0)
        {
            svoSeekVar.jumpType = F_J_MASK | BACK;
            svoSeekVar.jumpDistance = current_PBA * svoSeekVar.parFine / (32L * Iram_stp_CurrentPosition);
            svoSeekVar.jumpDistance += (svoSpeedVar.CurrentRpm / (60000 / 400) + cal_retry);//jump target = 0:0:0 - 400ms following time
            if (svoSeekVar.jumpDistance > FINE_THRESHOLD_CD)
            {
                svoSeekVar.jumpDistance = FINE_THRESHOLD_CD;
            }
            if (svo_BoundaryDet)
            {
                cal_retry++;
            }
            svo_BoundaryDet = TRUE;
            ret_seq = SEQ_EX1;
        }
        else if (svo_BoundaryDet == FALSE)
        {
            svoSeekVar.jumpType = F_J_MASK | FORW;
            svoSeekVar.jumpDistance = FINE_THRESHOLD_CD / 2;
            cal_retry++;
            ret_seq = SEQ_EX1;
        }
        else
        {
            svoIramVar.subQReadyFlag = FALSE;
            SetTimeout(0, ARG1);
            do{
                if (timeout_chk(0) == eTimerTimeout)
                {     // time out check
                    SendMsg80(SHOW_DEBUG_MSG,0x310213);   //Boundary_Detect_TimeOut!!!
                    LIBoundary_TimeOut = TRUE;
                    svo_BoundaryDet = FALSE;
                }
                if (checkTimeData())
                {
                    svoIramVar.subQReadyFlag = FALSE;
                    if (BufInfo.SubQ.Tno == 0)
                    {
                        if ((svoIramVar.currentTime.Time.Minute!=0) &&
                            (svoIramVar.currentTime.Time.Second!=0) &&
                            (svoIramVar.currentTime.Time.Frame !=0))
                        {
                            if (current_PBA <= svoIramVar.currentID.SectorNum.all32s)
                            {
                                current_PBA = svoIramVar.currentID.SectorNum.all32s;
                            }
                            else if (current_PBA > -10)//99:59:65
                            {
                                svo_BoundaryDet = FALSE;
                            }
                        }
                    }
                    else
                    {
                        svo_BoundaryDet = FALSE;
                    }
                }
            }while (svo_BoundaryDet);

            SetTimeout(0, TIMER_STOP);

            if ((current_PBA >= 0) || (LIBoundary_TimeOut == TRUE))
            {   //Error LI boundary protection
                SendMsgCn(DEBUG_DISK_ID_MSG,3,0x31020C,(BYTE)svoIramVar.currentTime.Time.Minute,(BYTE)svoIramVar.currentTime.Time.Second,(BYTE)svoIramVar.currentTime.Time.Frame);
                if (LIBoundary_TimeOut == TRUE)
                {
                    cal_retry = 6;
                }
                ret_seq = SEQ_EX1;
            }
            else
            {
                current_PBA += svoLIBoundary;

#if (DETECT_CDR == 1)
                if ((svoVar_discKindT.Bit.Media == eMediaR) && (current_PBA < 449995))
                {
                    svoVar_discKindT.Bit.Media = eMediaROM;
                    SendMsg80(SHOW_DEBUG_MSG,0x310041);
    #if (EN_POWER_CONTROL == 1)
        #if (WOBBLE_READ_ON_DVD_OR_CD == 1)
                    PowerControl(eCDReadPower);
        #endif
    #endif
                    LAL_StoreParams(LAL_DetectDiscKind(svoVar_discKindT), svoIramVar.svoCurrentSpeed);
                    LAL_SetReadMode();
                }
#endif
                SendMsgCn(DEBUG_DISK_ID_MSG,3,0x310210,(BYTE)(current_PBA / 4500),(BYTE)((current_PBA / 75) % 60),(BYTE)(current_PBA % 75));
                svoLIBoundary = current_PBA + 1;
                svoCalVar.calDoneBits |= boundary_end;
                ret_seq = SEQ_EX0;
            }
        }
    }

    if (ret_seq == SEQ_EX1)
    {
        if (cal_retry >= 6)
        {
            SendMsg80(SHOW_DEBUG_MSG,0x310217);   //Boundary_Detect_Error!!!
            svo_BoundaryDet = FALSE;
            ret_seq = SEQ_EX0;
        }
    }

    if (ret_seq == SEQ_EX0)
    {
        SendMsg80(DEBUG_DISK_ID_MSG,0x310215); //***Trk_Pitch_Cal_Start***

        svoSeekVar.targetRadius = Iram_stp_CurrentPosition + DENSITY_CALC_JUMP_DISTANCE_12CM;
        svoSeekVar.svoTargetID.SectorNum.all32s = svoRadius2PBA(svoSeekVar.targetRadius, 0);
        svoSeekVar.jumpType = R_J_MASK | FORW;
#if (DISC_8CM == 1)
        if (svoVar_discDiameter == e8cm)
        {
            svoSeekVar.jumpDistance = DENSITY_CALC_JUMP_DISTANCE_8CM;
        }
        else
#endif
        {
            svoSeekVar.jumpDistance = DENSITY_CALC_JUMP_DISTANCE_12CM;
        }
    }

    return(ret_seq);
}

/*-----------------------------------------------------------------------------
 *
 * Module: state_trackpitch_meas()
 * Description: track pitch calibration part 3, read current address, position then calculate svoSeekVar.parRough
 *
 *----------------------------------------------------------------------------*/
seq_exit_t state_trackpitch_meas(void)
{
    float   f1_f2_2,s1_s2_2,temp_f1,temp_f2;
    SHORT   pll_radius, positionError;

#if DO_HW_SUPPORT_STEP == 1
    signed_int_read_t StepMotorSupport_Location;
#endif /* DO_HW_SUPPORT_STEP */

    //sledge correction
    pll_radius = svo_get_radius_from_pll();
    positionError = (pll_radius - Iram_stp_CurrentPosition) / MICRO_STEP_PER_ELE_REV * MICRO_STEP_PER_ELE_REV;
    if (ABS(positionError) > (pll_radius / 20))//more than 5% error
    {
        svoStp_SetCurrentPosition(Iram_stp_CurrentPosition + positionError);
    }
    //store address and sledge position for track pitch calculation
    svoCurvefitCalVar.item[svoCurvefitCalVar.cal_index] = svoIramVar.currentID.SectorNum.all32s - svoSeekVar.parF0;
#if DO_HW_SUPPORT_STEP == 1
    StepMotorSupport_Location = read_stploc();
    svoCurvefitCalVar.cost[svoCurvefitCalVar.cal_index] = StepMotorSupport_Location.val;
#else
    svoCurvefitCalVar.cost[svoCurvefitCalVar.cal_index] = Iram_stp_CurrentPosition;
#endif
    if (ABS((SHORT)svoCurvefitCalVar.cost[svoCurvefitCalVar.cal_index - 1] - Iram_stp_CurrentPosition) >= (DENSITY_CALC_JUMP_DISTANCE_12CM / 8))
    {
        SendMsgCnB(DEBUG_AUTO_TUNING_MSG,7,0x3FAA03B,B1B(svoCurvefitCalVar.cal_index - 16 - 1),
            B4B((LONG)svoCurvefitCalVar.item[svoCurvefitCalVar.cal_index - 1] + svoSeekVar.parF0),
            B2B(SLED_STEP_2_UM((USHORT)svoCurvefitCalVar.cost[svoCurvefitCalVar.cal_index - 1])));
        SendMsgCnB(DEBUG_AUTO_TUNING_MSG,7,0x3FAA03B,B1B(svoCurvefitCalVar.cal_index - 16),
            B4B((LONG)svoCurvefitCalVar.item[svoCurvefitCalVar.cal_index] + svoSeekVar.parF0),
            B2B(SLED_STEP_2_UM((USHORT)svoCurvefitCalVar.cost[svoCurvefitCalVar.cal_index])));
        /*--------------------------------------------------------------------------------------------------------/
        /                                           ____________________________________________________________  /
        /            64 x (F1+F2) x (s1-s2)^2     /16384 x (s1-s2)^2 x ((s1-s2)^2 x F1 x F2 + (F1-F2)^2 x parN0)  /
        / parRough = ------------------------ +  / -------------------------------------------------------------  /
        /                   (F1-F2)^2          \/                           (F1-F2)^4                             /
        /                                                                                                         /
        /--------------------------------------------------------------------------------------------------------*/
        f1_f2_2 = svoCurvefitCalVar.item[svoCurvefitCalVar.cal_index] - svoCurvefitCalVar.item[svoCurvefitCalVar.cal_index-1];
        f1_f2_2 = f1_f2_2 * f1_f2_2;
        s1_s2_2 = svoCurvefitCalVar.cost[svoCurvefitCalVar.cal_index] - svoCurvefitCalVar.cost[svoCurvefitCalVar.cal_index-1];
        s1_s2_2 = s1_s2_2 * s1_s2_2;
        temp_f1 = svoCurvefitCalVar.item[svoCurvefitCalVar.cal_index] * svoCurvefitCalVar.item[svoCurvefitCalVar.cal_index-1];
        temp_f1 = (float)16384 * s1_s2_2 * (s1_s2_2 * temp_f1 + f1_f2_2 * (float)svoSeekVar.parN0);
        temp_f1 = temp_f1 / (f1_f2_2 * f1_f2_2);
        temp_f2 = svoCurvefitCalVar.item[svoCurvefitCalVar.cal_index] + svoCurvefitCalVar.item[svoCurvefitCalVar.cal_index-1];
        temp_f2 = (float)64 * temp_f2 * s1_s2_2 / f1_f2_2;
        svoSeekVar.parRough = (USHORT)(temp_f2 + fsqrt(temp_f1));

        switch (svoVar_discKindT.Bit.DiscStd)
        {
            case eDiscCD:
                if (svoSeekVar.parRough > UM_2_SLED_STEP(49931) * UM_2_SLED_STEP(49931) / 4096)// 64 min
                {
                    SendMsgCnB(SHOW_DEBUG_MSG,2,0x3FAA07,B2B(svoSeekVar.parRough));
                    svoSeekVar.parRough = UM_2_SLED_STEP(49931) * UM_2_SLED_STEP(49931) / 4096;
                }
                if (svoSeekVar.parRough < UM_2_SLED_STEP(36465) * UM_2_SLED_STEP(36465) / 4096)//120 min
                {
                    SendMsgCnB(SHOW_DEBUG_MSG,2,0x3FAA06,B2B(svoSeekVar.parRough));
                    svoSeekVar.parRough = UM_2_SLED_STEP(36465) * UM_2_SLED_STEP(36465) / 4096;
                }
                break;
        }

        if (svoCurvefitCalVar.cal_index - 17 > 1)
        {
            svoSeekVar.parRough = (svoSeekVar.parRough + svoSeekVar.parRoughInit) / 2;
        }
        svoSeekVar.parRoughInit = svoSeekVar.parRough;

#if (MSG_OUT_MODE != MSG_DISABLE)
        /*-----------------------------------------------------------------------------------------/
        / track pitch(m) = parRough * Sled_step / (4 * parFine)                                    /
        /                                                                                          /
        / Sled_step = DISTANCE_PER_MACH_REV / (MICRO_STEP_PER_ELE_REV * ELE_REV_PER_MACH_REV)[um]  /
        /-----------------------------------------------------------------------------------------*/
        temp_f1 = (float)DISTANCE_PER_MACH_REV / (4 * MICRO_STEP_PER_ELE_REV * ELE_REV_PER_MACH_REV);
        temp_f1 = temp_f1 * svoSeekVar.parRough / svoSeekVar.parFine;
        SendMsgCn(DEBUG_DISK_ID_MSG,6,0x31021A,A4B(temp_f1),A2B(svoSeekVar.parRough));
#endif
    }
    else
    {
        SendMsg80(DEBUG_DISK_ID_MSG,0x3A0002); // ==>Calib SKIP!!
    }

    if (svoCurvefitCalVar.cal_index - 17 < 2)
    {
        if (svoCurvefitCalVar.cost[svoCurvefitCalVar.cal_index] > Recorded_stp_Position[Layer_Index])
        {
            svoSeekVar.jumpType = R_J_MASK | BACK;
            svoSeekVar.jumpDistance = svoCurvefitCalVar.cost[svoCurvefitCalVar.cal_index] - Recorded_stp_Position[Layer_Index];
        }
        else
        {
            svoSeekVar.jumpType = R_J_MASK | FORW;
            svoSeekVar.jumpDistance = Recorded_stp_Position[Layer_Index] - svoCurvefitCalVar.cost[svoCurvefitCalVar.cal_index];
        }
        if (svoSeekVar.jumpDistance > (DENSITY_CALC_JUMP_DISTANCE_12CM / 8))
        {
            svoCurvefitCalVar.cal_index++;
            svoSeekVar.targetRadius = Recorded_stp_Position[Layer_Index];
            svoSeekVar.svoTargetID.SectorNum.all32s = svoRadius2PBA(svoSeekVar.targetRadius, 0);

            return(SEQ_EX0);
        }
    }
    return(SEQ_EX1);
}

/*-----------------------------------------------------------------------------
 *
 * Module: state_vcalc_err()
 * Description:
 *
 *----------------------------------------------------------------------------*/
seq_exit_t state_vcalc_err(void)
{
    svoCalVar.discChkDoneBits |= velo_chk_end;
    SetTimeout(0, TIMER_STOP);

    svoIramVar.subQEnableFlag = 1;
    return(SEQ_EX0);
}

/*-----------------------------------------------------------------------------
 *
 * Module: state_adjflag_check()
 * Description: Check if calibration has been done!
 *
 *----------------------------------------------------------------------------*/
seq_exit_t state_ofsadjflag_check(void)
{
    SendMsgCn(DEBUG_CALIB_ENTRYS_MSG,1,0x3A0109,A1B(ARG3)); // >>> STATE_OFSADJFLAG_CHECK <<<
    if ((svoCalVar.calOfsDoneBits & ARG3) == ARG3)
    {
        SendMsg80(DEBUG_CALIB_ENTRYS_MSG,0x3A0002); // ==>Calib SKIP!!
        return(SEQ_EX0);
    }
    else
    {
        SendMsg80(DEBUG_CALIB_ENTRYS_MSG,0x3A0003); // Calib RUN!!
        switch (ARG3)
        {
            case mbeamof_end:
                SendMsg80(DEBUG_CALIB_ENTRYS_MSG,0x33FE03);// == Meam-beam Offset Calibration ==
                break;

            case sbeamof_end:
                SendMsg80(DEBUG_CALIB_ENTRYS_MSG,0x33FE04);// == Side-beam Offset Calibration ==
                break;

            case feof_end:
                SendMsg80(DEBUG_CALIB_ENTRYS_MSG,0x33FE05);// == Servo Signals Offset Calibration ==
                break;

            case rfof_end:
                if ((svoIramVar.svoInState & SEQ_MASK) == SEQ_001)
                {
                    SendMsg80(DEBUG_CALIB_ENTRYS_MSG,0x33FE06);// == RF and DPD Signals Offset Calibration ==
                    cal_retry = 0;
                }
                break;

            case rpof_end:
                if (READ_FIELD(HAL_AFE_RFGN) != 0)
                {
                    return (SEQ_EX0);
                }
                break;
        }
        return(SEQ_EX1);
    }
}//end of state_adjflag_check

/*-----------------------------------------------------------------------------
 *
 * Module: state_adjflag_set()
 * Description: Set offset calibration flags!
 *
 *----------------------------------------------------------------------------*/
seq_exit_t state_ofsadjflag_set(void)
{
    svoCalVar.calOfsDoneBits |= ARG3;
    switch (ARG3)
    {
        case mbeamof_end:
            SendMsgCn(DEBUG_CALIB_RESULT_MSG,4,0x31050A+Layer_Index,svoCalOfsT2Var[Layer_Index][FEO_OS].normal_offset,svoCalOfsT2Var[Layer_Index][SRF_OS].normal_offset,svoCalOfsT2Var[Layer_Index][RADO_OS].normal_offset,svoCalOfsT2Var[Layer_Index][RBCO_OS].normal_offset);
            break;

        case sbeamof_end:
            SendMsgCn(DEBUG_CALIB_RESULT_MSG,2,0x31050C+Layer_Index,svoCalOfsT2Var[Layer_Index][SPP2_OS].normal_offset,svoCalOfsT2Var[Layer_Index][SBAD_OS].normal_offset);
            break;

        case feof_end:
            SendMsgCn(DEBUG_CALIB_RESULT_MSG,3,0x31052A+Layer_Index,svoCalOfsT1Var[Layer_Index][FE_OS].groove_offset[0],svoCalOfsT1Var[Layer_Index][FE_OS].groove_offset[1],svoCalOfsT2Var[Layer_Index][FE2_OS].normal_offset);
            break;

        case teof_end:
            SendMsgCn(DEBUG_CALIB_RESULT_MSG,3,0x310510+Layer_Index,svoCalOfsT1Var[Layer_Index][TE_OS].groove_offset[0],svoCalOfsT1Var[Layer_Index][TE_OS].groove_offset[1],svoCalOfsT2Var[Layer_Index][TE2_OS].normal_offset);
            break;

        case bsof_end:
            SendMsgCn(DEBUG_CALIB_RESULT_MSG,2,0x310512+Layer_Index,svoCalOfsT2Var[Layer_Index][BS_OS].normal_offset,svoCalOfsT2Var[Layer_Index][BS2_OS].normal_offset);
            break;

        case phbhof_end:
            // <L0 / 1> PHS_OS = %02Xh, BHS_OS = %02Xh
            SendMsgCn(DEBUG_CALIB_RESULT_MSG,2,0x310516+Layer_Index,svoCalOfsT3Var[Layer_Index][PHS_OS].normal_offset,svoCalOfsT3Var[Layer_Index][BHS_OS].normal_offset);
            // <L0> RPBG = %02Xh, RP_GN = %02Xh, RP_OS = %02Xh, RP2_OS = %02Xh
            SendMsgCn(DEBUG_CALIB_RESULT_MSG,4,0x310532+Layer_Index,svoCalGnT3Var[Layer_Index][RPB_GN].normal_gain,svoCalGnT3Var[Layer_Index][RP_GN].normal_gain,svoCalOfsT3Var[Layer_Index][RP_OS].normal_offset,svoCalOfsT3Var[Layer_Index][RP2_OS].normal_offset);
            break;

        case rfof_end:
#if (CHIP_REV >= 0xC0)
            // <L0 / 1> RFREFP_OS = %02Xh, RFREFN_OS = %02Xh
            SendMsgCn(DEBUG_CALIB_RESULT_MSG,2,0x310562,svoCalOfsT4Var[RFREFP_OS].normal_offset,svoCalOfsT4Var[RFREFN_OS].normal_offset);
#endif
            // <L0 / 1> RFCP_OS = %02Xh, RFCN_OS = %02Xh, RFP_OS = %02Xh
            SendMsgCn(DEBUG_CALIB_RESULT_MSG,3,0x31051A,svoCalOfsT2Var[Layer_Index][RFCP_OS].normal_offset,svoCalOfsT2Var[Layer_Index][RFCN_OS].normal_offset,svoCalOfsT3Var[Layer_Index][RFP_OS].normal_offset);
#if (CHIP_REV >= 0xC0)
            // <L0> svoRRFVar.ofs_ldoff[0-7] = {%3d, %3d, %3d, %3d, %3d, %3d, %3d, %3d}
            SendMsgCn(DEBUG_CALIB_RESULT_MSG,8,0x310564,svoRRFVar.ofs_ldoff[0],svoRRFVar.ofs_ldoff[1],svoRRFVar.ofs_ldoff[2],svoRRFVar.ofs_ldoff[3],
                                                        svoRRFVar.ofs_ldoff[4],svoRRFVar.ofs_ldoff[5],svoRRFVar.ofs_ldoff[6],svoRRFVar.ofs_ldoff[7]);
#else
            // <L0> svoRRFVar.ofs_ldoff[0-3] = {%3d, %3d, %3d, %3d}
            SendMsgCn(DEBUG_CALIB_RESULT_MSG,4,0x31051C,svoRRFVar.ofs_ldoff[0],svoRRFVar.ofs_ldoff[1],svoRRFVar.ofs_ldoff[2],svoRRFVar.ofs_ldoff[3]);
#endif
            // <L0> svoRRFVar.rfcos_step_size[0-3] = {%5u, %5u, %5u, %5u}
            SendMsgCn(DEBUG_CALIB_RESULT_MSG,8,0x31051D,A2B(svoRRFVar.rfcos_step_size[0]),A2B(svoRRFVar.rfcos_step_size[1]),A2B(svoRRFVar.rfcos_step_size[2]),A2B(svoRRFVar.rfcos_step_size[3]));
#if (CHIP_REV >= 0xC0)
            // <L0> svoRRFVar.rfcos_step_size[4-7] = {%5u, %5u, %5u, %5u}
            SendMsgCn(DEBUG_CALIB_RESULT_MSG,8,0x31051B,A2B(svoRRFVar.rfcos_step_size[4]),A2B(svoRRFVar.rfcos_step_size[5]),A2B(svoRRFVar.rfcos_step_size[6]),A2B(svoRRFVar.rfcos_step_size[7]));
#endif
            SendMsgCn(DEBUG_CALIB_RESULT_MSG,5,0x31052C+Layer_Index,svoCalOfsT3Var[Layer_Index][DPDCH1_OS].normal_offset,svoCalOfsT3Var[Layer_Index][DPDCH2_OS].normal_offset,svoCalOfsT3Var[Layer_Index][DPDCH3_OS].normal_offset,svoCalOfsT3Var[Layer_Index][DPDCH4_OS].normal_offset,svoCalOfsT4Var[DPDGM_OS].normal_offset);
            break;
    }
    return(SEQ_EX0);
}//end of state_adjflag_check

#if (NEW_RFPOS_CALIB == 1)
/*-----------------------------------------------------------------------------
 *
 * Module: state_RFPOS_cali()
 * Description: find RFP_OS gives linear RFO(P-N) around +-1000
 *
 *----------------------------------------------------------------------------*/
seq_exit_t state_RFPOS_cali(void)
{
    SBYTE   rfpos;
    BYTE    orig_rfhssel;

    #if (DVDRAM_USE_OLD_RFPOS_CALIB == 1)//20101022_02R7 ==start== replace 02QZ with new compiler switch DVDRAM_USE_OLD_RFPOS_CALIB and fix some code bugs
    if (DVDRAM)
    {
        return (SEQ_EX1);;
    }
    #endif//20101022_02R7 ==end== replace 02QZ with new compiler switch DVDRAM_USE_OLD_RFPOS_CALIB and fix some code bugs

    WRITE_FIELD(HAL_AFE_RFGN,0);
    WRITE_FIELD(HAL_AFE_RFAGCREG,0x10);
    if (calCalibrateStepsize(RFPGA_OS, RFPGA_GN, TYPE3, 0x00, svoRRFVar.step_size) == eNOERR)//20101030_02SO prevenet calCalibrateRPOS() calibration fail
    {
        orig_rfhssel = READ_FIELD(HAL_AFE_RFHSSEL);
        WRITE_FIELD(HAL_AFE_RFHSSEL, halAFE_RFLPFBk_Bypass);
        svoCurvefitCalVar.cal_index = 0;
        for (rfpos = -30; rfpos <= 30; rfpos += 6)
        {
            WRITE_FIELD(HAL_AFE_RFPOS, calSBYTE2DacCount(rfpos,6));
            calCalibrateOffset(RFPGA_OS,              // adjusted field
                               TYPE3,                 // type of offset structure
                               NORMAL_OFFSET,         // offset type
                               0,                     // target level       unit: mV
                               TRUE,                  // polarity between offset & signal
                               CALIB_FROM_0);
            WRITE_FIELD(HAL_AFE_EQDOSEL, halAFE_EQDOSEL6);//RFOP/N
            WRITE_FIELD(HAL_AFE_RFTPEN, TRUE);
            WRITE_FIELD(HAL_AFE_MUX1ADSEL, halAFE_MUX1ADSEL_RFTP1);
            WRITE_FIELD(HAL_AFE_MUX2ADSEL, halAFE_MUX2ADSEL_RFTP2);
            WRITE_FIELD(HAL_AFE_RFAOCREG, calSBYTE2DacCount(calDacCount2SBYTE(last_cal_result,8) + (1000 * 256) / svoRRFVar.step_size[0],8));
            Delay100uS(1);
            svoCurvefitCalVar.cost[svoCurvefitCalVar.cal_index] = readPNmV(ADC_MUXOUT1, ADC_MUXOUT2, NO_REVOLUTION, FALSE).middle_mv;
            WRITE_FIELD(HAL_AFE_RFAOCREG, calSBYTE2DacCount(calDacCount2SBYTE(last_cal_result,8) - (1000 * 256) / svoRRFVar.step_size[0],8));
            Delay100uS(1);
            svoCurvefitCalVar.cost[svoCurvefitCalVar.cal_index] -= readPNmV(ADC_MUXOUT1, ADC_MUXOUT2, NO_REVOLUTION, FALSE).middle_mv;
            svoCurvefitCalVar.item[svoCurvefitCalVar.cal_index] = rfpos;
            svoCurvefitCalVar.cal_index++;
        }
        WRITE_FIELD(HAL_AFE_RFHSSEL, orig_rfhssel);
        CurveFit(svoCurvefitCalVar.cal_index, svoCurvefitCalVar.item, svoCurvefitCalVar.cost, svoCurvefitCalVar.cof);
        rfpos = (SBYTE)float2short(svoCurvefitCalVar.cof[1] / (-2 * svoCurvefitCalVar.cof[2]));

#if 0 /* for debug */
        {
            BYTE i;

            for (i = 0; i < svoCurvefitCalVar.cal_index; i++)
            {
                SendMsgCnB(SHOW_DEBUG_MSG,4,0x338101,F4B(svoCurvefitCalVar.item[i]));     // SET_FEBG_GAIN ==> %4.0f (dB)
            }
            for (i = 0; i < svoCurvefitCalVar.cal_index; i++)
            {
                SendMsgCnB(SHOW_DEBUG_MSG,4,0x338105,F4B(svoCurvefitCalVar.cost[i]));    // FEO-balance ==> %4.2f (%)
            }
            for (i = 0; i < 2; i++)
            {
                SendMsgCnB(SHOW_DEBUG_MSG,4,0x338201+i,F4B(svoCurvefitCalVar.cof[i]));  // Y=A*X^2+B*X+C; A = %4.2f
            }
            // OPTIMUM_FEOBG ==> %4.2f
            SendMsgCnB(SHOW_DEBUG_MSG,4,0x33FF4C,F4B(svoCurvefitCalVar.cof[1] / (-2 * svoCurvefitCalVar.cof[2])));
        }
#endif // 0

        calSetAfeOffset(0,RFP_OS,TYPE3,NORMAL_OFFSET,0,calSBYTE2DacCount(rfpos,6));
    }
    calCalibrateStepsize(RFPGA_OS, RFPGA_GN, TYPE3, 0x1F, svoRRFVar.step_size);//20101030_02SO prevenet calCalibrateRPOS() calibration fail

    WRITE_FIELD(HAL_AFE_RFGN,(HAL_AFE_RFGN_MASK >> HAL_AFE_RFGN_LSB));

#if (NEW_RFPOS_CALIB == 1) && (NEW_RFPOS_CALIB_BRING_AGCVREF_TO_ZERO == 1)
    WRITE_FIELD(HAL_AFE_RFAGCREG, HAL_AFE_RFAGCREG_MASK >> HAL_AFE_RFAGCREG_LSB);
    calCalibrateOffset(RFPGA_OS,              // adjusted field
                       TYPE3,                 // type of offset structure
                       NORMAL_OFFSET,         // offset type
                       0,                     // target level       unit: mV
                       TRUE,                  // polarity between offset & signal
                       CALIB_FROM_0);
    WRITE_FIELD(HAL_AFE_RFAOCREG, last_cal_result);
    calCalibrateStepsize(RFP_OS, RFPGA_GN, TYPE3, 0x00, &svoRRFVar.rfpos_step_size);
    SendMsgCnB(DEBUG_CALIB_RESULT_MSG,2,0x310565,B2B(svoRRFVar.rfpos_step_size));//"<L0> svoRRFVar.rfpos_step_size = %4d (mV*256dac)"
    for (orig_rfhssel = 0; orig_rfhssel < 3; orig_rfhssel++)
    {
        WRITE_FIELD(HAL_AFE_RFHSSEL, orig_rfhssel + 1);
        for (rfpos = 0; rfpos < 32; rfpos++)
        {
            SHORT target_mv;

            WRITE_FIELD(HAL_AFE_RFHSFC, rfpos);
            target_mv = calGetGainVref(HAL_AFE_RFAGCREG_ADR,HAL_AFE_RFAGCREG_MASK,HAL_AFE_RFAGCREG_LSB,ADC_SPECIAL_RFDFE);
            SendMsgCn(DEBUG_CALIB_RESULT_MSG,4,0x310563,A1B(orig_rfhssel),A1B(rfpos),A2B(target_mv));//<L0> svoRRFVar.rfagc_ref[%u][%2u] = %4d (mV)
            if (target_mv > 1270) target_mv = 1270;
            if (target_mv < -1280) target_mv = -1280;
            svoRRFVar.rfagc_ref_LPF[orig_rfhssel][rfpos] = RoundDivide(target_mv, 10);
        }
    }
    HAL_AFE_SetRfBw(svo_bReadZoneSpeedX10, COEF_GetValue(AFE_CALC_BwRfRatioX100), svoVar_TrackingMethod);
    SendMsgCnB(DEBUG_CALIB_RESULT_MSG,4,0x310563,B1B(READ_FIELD(HAL_AFE_RFHSSEL) == 3),B1B(READ_FIELD(HAL_AFE_RFHSFC)),B2B(svoRRFVar.rfagc_ref));//<L0> svoRRFVar.rfagc_ref[%u][%2u] = %4d (mV)
#endif

    WRITE_FIELD(HAL_AFE_RFTPEN,FALSE);
    WRITE_FIELD(HAL_AFE_RFANATSEL, halAFE_RFANATSEL0);
    LAL_WriteCoef(DFE_HFAMPMCFG);

    return (SEQ_EX0);
}// end of state_RFPOS_cali
#endif

/*-----------------------------------------------------------------------------
 *
 * Module: state_RFCOS_size_cali()
 * Description: find svoRRFVar.rfcos_step_size[], and svoRRFVar.ofs_ldoff[] for different RF_GN(RFGNO)
 *
 *----------------------------------------------------------------------------*/
seq_exit_t state_RFCOS_size_cali(void)
{
    BYTE     rfgain_sw, total_bits, rfcos_delta;
    ad_dsp_t signal_ret;
    SHORT    RF_diff,target_diff;
    float    temp_float;

    total_bits = (HAL_AFE_RFCOSP_MSB-HAL_AFE_RFCOSP_LSB+1);
    rfgain_sw = READ_FIELD(HAL_AFE_RFGN);
    svoRRFVar.ofs_ldoff[rfgain_sw] = calDacCount2SBYTE(svoCalOfsT3Var[0][RFPGA_OS].normal_offset,(HAL_AFE_RFAOCREG_MSB-HAL_AFE_RFAOCREG_LSB+1));
#if (NEW_RFPOS_CALIB == 1)
    svoRRFVar.ofs_ldoff[rfgain_sw] += svoRRFVar.rfagc_ref * 256 / svoRRFVar.step_size[HAL_AFE_RFAGCREG_MASK >> HAL_AFE_RFAGCREG_LSB];
#endif

    target_diff = 1200;
    if (rfgain_sw == (HAL_AFE_RFGN_MASK >> HAL_AFE_RFGN_LSB))
    {
        svoRRFVar.rfcos_shift_max = (HAL_AFE_RFCOSP_MASK >> (HAL_AFE_RFCOSP_LSB + 1)) - calDacCount2SBYTE(svoCalOfsT2Var[0][RFCP_OS].normal_offset,total_bits);
        rfcos_delta = (HAL_AFE_RFCOSN_MASK >> (HAL_AFE_RFCOSN_LSB + 1)) + calDacCount2SBYTE(svoCalOfsT2Var[0][RFCN_OS].normal_offset,total_bits);
        if (svoRRFVar.rfcos_shift_max > rfcos_delta)
        {
            svoRRFVar.rfcos_shift_max = rfcos_delta;
        }
#if (CHIP_REV < 0xC0)
        rfcos_delta = 4;
#else
        rfcos_delta = 3;
#endif
    }
    else
    {
#if (CHIP_REV < 0xC0)
        rfcos_delta = target_diff * 64 * 2 / svoRRFVar.rfcos_step_size[rfgain_sw + 1];//*2 due to 6dB per RFGN
#else
        rfcos_delta = (target_diff * 64 * 11 / 8) / svoRRFVar.rfcos_step_size[rfgain_sw + 1];//*11/8 due to 3dB per RFGN
#endif
        if (rfcos_delta > 8)
        {
            target_diff = target_diff * 8 / rfcos_delta;
            rfcos_delta = 8;
        }
    }

    if (rfcos_delta > svoRRFVar.rfcos_shift_max)
    {
        RF_diff = svoCalGnT3Var[Layer_Index][RFIN_GN].normal_gain;
        if (RF_diff > 0)
        {
            RF_diff--;
            calSetAfeGain(Layer_Index,RFIN_GN,TYPE3,0,0,RF_diff);
            if (RF_diff != 1)
            {
                calSetAfeGain(Layer_Index,RF_GN,TYPE2,PATH_SELECT_ALL,0,svoCalGnT2Var[Layer_Index][RF_GN].normal_gain+1);
            }
            return(SEQ_EX2);
        }
        rfcos_delta = svoRRFVar.rfcos_shift_max;
    }
    if (rfcos_delta == 0)
    {
        svoRRFVar.rfcos_step_size[rfgain_sw] = 0x7FFF;
        while (rfgain_sw > 0)
        {
            rfgain_sw--;
            svoRRFVar.rfcos_step_size[rfgain_sw] = 0x7FFF;
        }
        WRITE_FIELD(HAL_AFE_RFGN,rfgain_sw);
        temp_float = ((float)100 / COEF_GetValue(DFE_CALC_M) - 0.5) * (COEF_GetValue(DFE_CALC_Target_pp) * svoRRFVar.step_size[0] * 64) / (svoRRFVar.step_size[0x10] * svoRRFVar.rfcos_step_size[rfgain_sw]);
        calSetRfcosShift((BYTE)(temp_float * svoPrvtVar_avgSRFO[0] / svoPrvtVar_maxSRFO + 0.5));
        return (SEQ_EX1);
    }
    //measure svoRRFVar.rfcos_step_size[rfgain_sw]
    (void)afeSetCalibMUX(TYPE3, RFP_OS);
    WRITE_FIELD(HAL_AFE_RFAGCREG,0);
#if (NEW_RFPOS_CALIB == 1)
    WRITE_FIELD(HAL_AFE_RFAOCREG,calSBYTE2DacCount(svoRRFVar.ofs_ldoff[rfgain_sw] - (svoRRFVar.rfagc_ref - (target_diff / 2)) * 256 / svoRRFVar.step_size[0], (HAL_AFE_RFAOCREG_MSB-HAL_AFE_RFAOCREG_LSB+1)));
#else
    WRITE_FIELD(HAL_AFE_RFAOCREG,calSBYTE2DacCount(svoRRFVar.ofs_ldoff[rfgain_sw] + target_diff * (256 / 2) / svoRRFVar.step_size[0], (HAL_AFE_RFAOCREG_MSB-HAL_AFE_RFAOCREG_LSB+1)));
#endif
    WRITE_FIELD(HAL_AFE_RFCOSP,svoCalOfsT2Var[0][RFCP_OS].normal_offset);
    WRITE_FIELD(HAL_AFE_RFCOSN,svoCalOfsT2Var[0][RFCN_OS].normal_offset);
    Delay100uS(10);

    signal_ret = readADCmVPerRevs(NO_REVOLUTION, FALSE, RF_ADC);
    RF_diff = signal_ret.middle_mv;
    WRITE_FIELD(HAL_AFE_RFCOSP,calSBYTE2DacCount(calDacCount2SBYTE(svoCalOfsT2Var[0][RFCP_OS].normal_offset,total_bits) + rfcos_delta,total_bits));
    WRITE_FIELD(HAL_AFE_RFCOSN,calSBYTE2DacCount(calDacCount2SBYTE(svoCalOfsT2Var[0][RFCN_OS].normal_offset,total_bits) - rfcos_delta,total_bits));
    Delay100uS(10);

    signal_ret = readADCmVPerRevs(NO_REVOLUTION, FALSE, RF_ADC);
    // RFCOS_step_size calib: RF_GN = %1d, RFCOS_dif = %2d, RF_ADC1 = %4d, RF_ADC2 = %4d
    SendMsgCn(DEBUG_AUTO_TUNING_MSG,6,0x31664F,A1B(rfgain_sw),A1B(rfcos_delta),A2B(RF_diff),A2B(signal_ret.middle_mv));
    RF_diff -= signal_ret.middle_mv;
    LAL_WriteCoef(DFE_HFAMPMCFG);
    if (rfcos_delta !=0)
    {
        svoRRFVar.rfcos_step_size[rfgain_sw] = ABS(RF_diff) * 64 / rfcos_delta;
        if (svoRRFVar.rfcos_step_size[rfgain_sw] == 0)
        {
            svoRRFVar.rfcos_step_size[rfgain_sw] = 0x7FFF;
        }
    }

    if (rfgain_sw > 0)
    {
        rfgain_sw--;
        WRITE_FIELD(HAL_AFE_RFGN,rfgain_sw);
        WRITE_FIELD(HAL_AFE_RFCOSP,svoCalOfsT2Var[0][RFCP_OS].normal_offset);
        WRITE_FIELD(HAL_AFE_RFCOSN,svoCalOfsT2Var[0][RFCN_OS].normal_offset);

        return(SEQ_EX0);
    }
    else
    {
        //Load original RFGN and relative calibrated RFxx_OS
        rfgain_sw = svoCalGnT2Var[0][RF_GN].normal_gain;
        WRITE_FIELD(HAL_AFE_RFGN,rfgain_sw);

        //calculate svoRRFVar.rfcos_shift_cur for layer 0
        temp_float = ((float)100 / COEF_GetValue(DFE_CALC_M) - 0.5) * (COEF_GetValue(DFE_CALC_Target_pp) * svoRRFVar.step_size[0] * 64) / (svoRRFVar.step_size[0x10] * svoRRFVar.rfcos_step_size[rfgain_sw]);
        calSetRfcosShift((BYTE)(temp_float * svoPrvtVar_avgSRFO[0] / svoPrvtVar_maxSRFO + 0.5));

        return(SEQ_EX1);
    }
}// end of state_RFCOS_size_cali

/*-----------------------------------------------------------------------------
 *
 * Module: state_afeinof_cali()
 * Description: AFE input offset calibration,
 * Input: ARG1: FEO_OS/MPP_OS/SPP1_OS/SPP2_OS/SBAD_OS/TE2_OS TE_OS/FE_OS
 *        ARG2: DO_WGOS NO_WGOS
 *        ARG3: TYPE1 TYPE2 TYPE3
 *
 *----------------------------------------------------------------------------*/
seq_exit_t state_afeinof_cali(void)
{
    SHORT     target_mv;
    errcode_t ret,last_err;
    BYTE      orig_value;
    BYTE      nr_calibs;
    SBYTE     retry;
    BOOL      polarity;
#if (EN_POWER_CONTROL == 1)
    BYTE      orig_prdn;
#endif

    last_err = eNOERR;
    polarity = TRUE;
    target_mv = vref_fm_mux3_mV;

#if (DVD_RAM_READ == 1)
    // Force LAND to low (a) to use GROOVE gain register
    //                   (b) to prevent from TE polarity change
    if (DVDRAM)
    {
        WRITE_FIELD(HAL_AFE_LANDSEL,0);
    }
#endif

    // Pre-setting all the channel
    switch (ARG3)
    {
        case TYPE1:
            switch (ARG1)
            {
                case TE_OS:
#if (CHIP_REV >= 0xC1)
                    WRITE_FIELD(HAL_AFE_DPDRISEN, 0);
                    WRITE_FIELD(HAL_AFE_DPDFALEN, 0);
#else
                    WRITE_FIELD(HAL_AFE_DPDCPHLD, 1);
#endif
                    orig_value = READ_FIELD(HAL_AFE_TRKG);
                    WRITE_FIELD(HAL_AFE_TRKG, HAL_AFE_TRKG_MASK >> HAL_AFE_TRKG_LSB);
                    target_mv = calGetGainVref(HAL_AFE_TRKG_ADR,HAL_AFE_TRKG_MASK,HAL_AFE_TRKG_LSB,ADC_TE);
                    break;

                case FE_OS:
                    orig_value = READ_FIELD(HAL_AFE_FCSG);
                    WRITE_FIELD(HAL_AFE_FCSG, HAL_AFE_FCSG_MASK >> HAL_AFE_FCSG_LSB);
                    if (ARG2 != TEMP_OS)
                    {
                        target_mv = calGetGainVref(HAL_AFE_FCSG_ADR,HAL_AFE_FCSG_MASK,HAL_AFE_FCSG_LSB,ADC_FE);
                    }
                    break;
            }
            break;

        case TYPE2:
            switch (ARG1)
            {
                case MPP_OS:
#if (APP_TE_SUPPORT == 1)
                    if (((READ_REG(HAL_AFEREG_MAP->MB16) & (HAL_AFE_MPPAH|HAL_AFE_MPPBG)) == (HAL_AFE_MPPAH|HAL_AFE_MPPBG)) &&
                        ((READ_REG(HAL_AFEREG_MAP->MB15) & (HAL_AFE_MPPA|HAL_AFE_MPPB|HAL_AFE_MPPC|HAL_AFE_MPPD)) == (HAL_AFE_MPPA|HAL_AFE_MPPB)))
                    {
                        //if MPP is set to H - G for APP, change it back to (A + D) - (B + C) and store the MPP_OS for TRO offset calibration.
                        READ_MOD_WRITE(HAL_AFEREG_MAP->MB16, HAL_AFE_MPPAH|HAL_AFE_MPPBG, 0);
                        READ_MOD_WRITE(HAL_AFEREG_MAP->MB15, HAL_AFE_MPPA|HAL_AFE_MPPB|HAL_AFE_MPPC|HAL_AFE_MPPD, HAL_AFE_MPPA|HAL_AFE_MPPB|HAL_AFE_MPPC|HAL_AFE_MPPD);
                        retry = 3;
                        do
                        {
                            ret = calCalibrateOffset(ARG1,                  // adjusted field
                                                     ARG3,                  // type of offset structure
                                                     NORMAL_OFFSET,         // offset type
                                                     target_mv,             // target level       unit: mV
                                                     polarity,              // polarity between offset & signal
                                                     CALIB_FROM_0);
                            if (ret != eNOERR)
                            {
                                retry--;
    #if(ERROR_CAL_START_UP_MONITOR ==1)
                                svoMiscVar.calibrationErrorCount++;
    #endif
                                SendMsgCnB(SHOW_DEBUG_MSG,4,0x3A0001,B1B(ret),B1B(((svoIramVar.svoInState & STAT_MASK) >> 12)),B1B(((svoIramVar.svoInState & TRN_MASK ) >> 5) + 1),B1B((svoIramVar.svoInState & SEQ_MASK) + 1));
                            }
                        }
                        while ((ret != eNOERR) && (retry >= 0));
                        svoVar_mpp_os = last_cal_result;
                        READ_MOD_WRITE(HAL_AFEREG_MAP->MB16, HAL_AFE_MPPAH|HAL_AFE_MPPBG, HAL_AFE_MPPAH|HAL_AFE_MPPBG);
                        READ_MOD_WRITE(HAL_AFEREG_MAP->MB15, HAL_AFE_MPPA|HAL_AFE_MPPB|HAL_AFE_MPPC|HAL_AFE_MPPD, HAL_AFE_MPPA|HAL_AFE_MPPB);
                    }
#endif
#if (MPPBG_SPPBG_CALIB != 0)
                    orig_value = COEF_GetValue(AFE_MB16_MPPOBG);
                    WRITE_FIELD(HAL_AFE_MPPOBG,0x10 - MPPBG_MAX_UNB);
#endif
                    break;
#if (MPPBG_SPPBG_CALIB != 0)
                case SPP1_OS:
                    orig_value = COEF_GetValue(AFE_SB16_SPP1BG);
                    WRITE_FIELD(HAL_AFE_SPP1BG,0x10 - SPP1BG_MAX_UNB);
                    break;
#endif
                case SRF_OS:
                case SBAD_OS:
                    if (ARG2 == TEMP_OS)
                    {//set target lower giving larger room for signal measurement
                        nr_calibs = READ_FIELD(HAL_AFE_BSPGN) * 6 + READ_FIELD(HAL_AFE_BSKG) - (16 + 6);
                        if ((READ_REG(HAL_AFEREG_MAP->BSRP05) & (HAL_AFE_BSSRF_MASK | HAL_AFE_BSSBA_MASK)) == (HAL_AFE_BSSRF_MASK | HAL_AFE_BSSBA_MASK))
                        {
                            nr_calibs += 6;
                        }
                        target_mv = BOT_SATURATE_LIM + 100;

                        if (nr_calibs > 0)
                        {
                            target_mv = VREF_IN_MV - (VREF_IN_MV - target_mv) * CalculateRateFromXdB(ONEdB,-nr_calibs);
                        }
                    }
                    break;

                case TE2_OS:
                    //The goal is set TE_target to reference of TE2GNREG.
#if (CHIP_REV >= 0xC1)
                    WRITE_FIELD(HAL_AFE_DPDRISEN, 0);
                    WRITE_FIELD(HAL_AFE_DPDFALEN, 0);
#else
                    WRITE_FIELD(HAL_AFE_DPDCPHLD, 1);
#endif
                    orig_value = READ_FIELD(HAL_AFE_TE2GNREG);
                    WRITE_FIELD(HAL_AFE_TE2GNREG, HAL_AFE_TE2GNREG_MASK >> HAL_AFE_TE2GNREG_LSB);
#if (AFE_NORMALIZER == 1)
                    WRITE_FIELD(HAL_AFE_TE2AUTOEN, 0);
#endif
                    target_mv = calGetGainVref(HAL_AFE_TE2GNREG_ADR,HAL_AFE_TE2GNREG_MASK,HAL_AFE_TE2GNREG_LSB,ADC_TE);
                    break;

                case FE2_OS:
                    //The goal is set FE_target to reference of FE2GNREG.
                    orig_value = READ_FIELD(HAL_AFE_FE2GNREG);
                    WRITE_FIELD(HAL_AFE_FE2GNREG, HAL_AFE_FE2GNREG_MASK >> HAL_AFE_FE2GNREG_LSB);
#if (AFE_NORMALIZER == 1)
                    WRITE_FIELD(HAL_AFE_FE2AUTOEN, 0);
#endif
                    target_mv = calGetGainVref(HAL_AFE_FE2GNREG_ADR,HAL_AFE_FE2GNREG_MASK,HAL_AFE_FE2GNREG_LSB,ADC_FE);
                    break;

                case BS_OS:
                    //The goal is set BS_target to reference of BSKG.
                    orig_value = READ_FIELD(HAL_AFE_BSKG);
                    WRITE_FIELD(HAL_AFE_BSKG, HAL_AFE_BSKG_MASK >> HAL_AFE_BSKG_LSB);
                    target_mv = calGetGainVref(HAL_AFE_BSKG_ADR,HAL_AFE_BSKG_MASK,HAL_AFE_BSKG_LSB,ADC_BS);
                    break;

                case BS2_OS:
                    //The goal is set BS_target to reference of BS2GNREG.
                    orig_value = READ_FIELD(HAL_AFE_BS2GNREG);
                    WRITE_FIELD(HAL_AFE_BS2GNREG, HAL_AFE_BS2GNREG_MASK >> HAL_AFE_BS2GNREG_LSB);
#if (AFE_NORMALIZER == 1)
                    WRITE_FIELD(HAL_AFE_BS2AUTOEN, 0);
#endif
                    target_mv = calGetGainVref(HAL_AFE_BS2GNREG_ADR,HAL_AFE_BS2GNREG_MASK,HAL_AFE_BS2GNREG_LSB,ADC_BS);
#if (AFE_NORMALIZER == 1)
                    target_mv -= 40;
#endif
                    break;

                case RFCP_OS:
                    target_mv = v21_fm_mux1_mV;
                    break;

                case RFCN_OS:
                    orig_value = READ_FIELD(HAL_AFE_RFGN);
                    WRITE_FIELD(HAL_AFE_RFGN,(HAL_AFE_RFGN_MASK >> HAL_AFE_RFGN_LSB));
                    target_mv = 0;
                    break;

                case RADO_OS:
                case RBCO_OS:
#if (EN_POWER_CONTROL == 1)
                    orig_prdn=READ_FIELD(HAL_AFE_PDWOBALL);
                    orig_prdn|=(READ_FIELD(HAL_AFE_PDWOB)<<1);
                    orig_prdn|=(READ_FIELD(HAL_AFE_PDINPWBL)<<2);
                    orig_prdn|=(READ_FIELD(HAL_AFE_PDREADWOB)<<3);
                    orig_prdn |= (READ_FIELD(HAL_AFE_PDWRTWOB)<<4);
                    orig_prdn |= (READ_FIELD(HAL_AFE_PDWPP12)<<5);
                    WRITE_FIELD(HAL_AFE_PDWOBALL,0);
                    WRITE_FIELD(HAL_AFE_PDWOB,0);
                    WRITE_FIELD(HAL_AFE_PDINPWBL,0);
                    WRITE_FIELD(HAL_AFE_PDREADWOB,0);
                    WRITE_FIELD(HAL_AFE_PDWRTWOB,0);
                    WRITE_FIELD(HAL_AFE_PDWPP12,0);
#else
                    WRITE_FIELD(HAL_AFE_PDWOB,0);  //HAL_AFEREG_MAP->WOBAPC24,0x100009A2 ,wobble block power up
#endif
                    // RSH=1 ; WSH=1
                    WRITE_FIELD(HAL_AFE_WBLWSHSEL, 0x00); //WBLWSH2 = WBLWSH
                    WRITE_FIELD(HAL_AFE_WSHSEL, 0x03);    //WSH = 1
                    WRITE_FIELD(HAL_AFE_WBLRSHSEL, 0x00); //WBLRSH2 = WBLWSH
                    WRITE_FIELD(HAL_AFE_RSHSEL, 0x03);    //RSH = 1
                #if (CHIP_REV <= 0xB3)
                    WRITE_FIELD(HAL_AFE_WBLWOUTS, 0x02);  //WBLWOUT = WLDON
                #else
                    WRITE_FIELD(HAL_AFE_WBLWOUT, 0x02);  //WBLWOUT = WLDON
                #endif
                    WRITE_FIELD(HAL_AFE_WBLROUT, 0x02);   //WBLROUT = not WLDON

                    target_mv = -800;//give more working range
                    polarity = FALSE;
                    break;

                default:
                    break;
            }
            break;

        case TYPE3:
            switch (ARG1)
            {
                case PHS_OS://20101101_02T6 ==start== prevent PHS/BHS offset calibration problem when using RFRP
                    HAL_AFE_SetPhBhDrooprate(e_PHS, 9999);
                    break;

                case BHS_OS:
                    HAL_AFE_SetPhBhDrooprate(e_BHS, 9999);
                    break;//20101101_02T6 ==end== prevent PHS/BHS offset calibration problem when using RFRP

                case RP_OS:
                    orig_value = READ_FIELD(HAL_AFE_RPGN);
                    WRITE_FIELD(HAL_AFE_RPGN, HAL_AFE_RPGN_MASK >> HAL_AFE_RPGN_LSB);
                    target_mv = calGetGainVref(HAL_AFE_RPGN_ADR,HAL_AFE_RPGN_MASK,HAL_AFE_RPGN_LSB,ADC_RP);
                    break;

                case RP2_OS:
                    orig_value = READ_FIELD(HAL_AFE_RP2GN);
                    WRITE_FIELD(HAL_AFE_RP2GN, HAL_AFE_RP2GN_MASK >> HAL_AFE_RP2GN_LSB);
                    target_mv = calGetGainVref(HAL_AFE_RP2GN_ADR,HAL_AFE_RP2GN_MASK,HAL_AFE_RP2GN_LSB,ADC_RP);
                    break;

                case RFP_OS:
#if ((NEW_RFPOS_CALIB == 0) || (DVDRAM_USE_OLD_RFPOS_CALIB == 1))
                    WRITE_FIELD(HAL_AFE_RFGN,0);
                    retry = 3;
                    do
                    {
                        ret = calCalibrateStepsize(RFPGA_OS, RFPGA_GN, TYPE3, 0x1F, svoRRFVar.step_size);
                        if (ret != eNOERR)
                        {
                            retry--;
#if(ERROR_CAL_START_UP_MONITOR ==1)
                            svoMiscVar.calibrationErrorCount++;
#endif
                            SendMsgCnB(SHOW_DEBUG_MSG,4,0x3A0001,B1B(ret),B1B(((svoIramVar.svoInState & STAT_MASK) >> 12)),B1B(((svoIramVar.svoInState & TRN_MASK ) >> 5) + 1),B1B((svoIramVar.svoInState & SEQ_MASK) + 1));
                        }
                    }
                    while ((ret != eNOERR) && (retry >= 0));

                    if (ret == eNOERR)
                    {
                        //The goal is bring RFAGC center to DFE RF ADC 0mV.
                        //-----------------------------------------------------------------------;
                        // RFAGCREG = -8dB, RF = A
                        // RFAGCREG = +7.5dB, RF = B
                        // RFAGCREG = -infinity dB, RF = C
                        // The goal is to bring C to 0. So the calibration target is A - C (when RFAGCREG = -8dB)
                        // 15.5dB * (A - C) = B - C
                        // (15.5dB) * C - C = 15.5dB * A - B
                        // C = (15.5dB * A - B) / (15.5dB - 1)
                        // target = A - C
                        //        = A - (15.5dB * A - B) / (15.5dB - 1)
                        //        = ((15.5dB - 1) * A - 15.5dB * A + B) / (15.5dB - 1)
                        //        = (B - A) / (15.5dB - 1)
                        //-----------------------------------------------------------------------;

                        WRITE_FIELD(HAL_AFE_RFAOCREG,0);
                        WRITE_FIELD(HAL_AFE_RFAGCREG,HAL_AFE_RFAGCREG_MASK >> HAL_AFE_RFAGCREG_LSB);//max gain
                        Delay100uS(2);
                        target_mv = readADCmVPerRevs(NO_REVOLUTION, FALSE, RF_ADC).middle_mv;//B

                        WRITE_FIELD(HAL_AFE_RFAGCREG,0);//min gain
                        Delay100uS(2);
                        //readADCmVPerRevs(NO_REVOLUTION, FALSE, RF_ADC);//A
                        target_mv = (SHORT)((LONG)(target_mv - readADCmVPerRevs(NO_REVOLUTION, FALSE, RF_ADC).middle_mv) * svoRRFVar.step_size[0] / (svoRRFVar.step_size[HAL_AFE_RFAGCREG_MASK >> HAL_AFE_RFAGCREG_LSB] - svoRRFVar.step_size[0]));
                    }
                    else
                    {
                        target_mv = 0;
                    }
#endif
                    break;

                case RFPGA_OS:
                    WRITE_FIELD(HAL_AFE_RFAGCREG,HAL_AFE_RFAGCREG_MASK >> HAL_AFE_RFAGCREG_LSB);
                    target_mv = 0;
                    break;

                case CE_OS:
                    orig_value = READ_FIELD(HAL_AFE_CEGN);
                    WRITE_FIELD(HAL_AFE_CEGN, HAL_AFE_CEGN_MASK >> HAL_AFE_CEGN_LSB);
                    target_mv = calGetGainVref(HAL_AFE_CEGN_ADR,HAL_AFE_CEGN_MASK,HAL_AFE_CEGN_LSB,ADC_CE);
                    break;

                case DPDCH1_OS:
                case DPDCH2_OS:
                case DPDCH3_OS:
                case DPDCH4_OS:
                    WRITE_FIELD(HAL_AFE_DPDGN, MAX(svoCalGnT3Var[0][DPD_GN].normal_gain,svoCalGnT3Var[1][DPD_GN].normal_gain));
#if (CHIP_REV >= 0xC0)
                    //update DPDGN1~4
                    READ_MOD_WRITE(HAL_AFEREG_MAP->RF07,HAL_AFE_DPDGN1SEL|HAL_AFE_DPDGN2SEL,HAL_AFE_DPDGN1SEL|HAL_AFE_DPDGN2SEL);
                    READ_MOD_WRITE(HAL_AFEREG_MAP->RF09,HAL_AFE_DPDGN3SEL|HAL_AFE_DPDGN4SEL,HAL_AFE_DPDGN3SEL|HAL_AFE_DPDGN4SEL);
                    READ_MOD_WRITE(HAL_AFEREG_MAP->RF07,HAL_AFE_DPDGN1SEL|HAL_AFE_DPDGN2SEL,0);
                    READ_MOD_WRITE(HAL_AFEREG_MAP->RF09,HAL_AFE_DPDGN3SEL|HAL_AFE_DPDGN4SEL,0);
#endif
                    WRITE_FIELD(HAL_AFE_PDDPD, FALSE);   //970[5]=0,Not Power down the whole DPD block
                    READ_MOD_WRITE(HAL_AFEREG_MAP->RF20, HAL_AFE_DPD12EN_MASK|HAL_AFE_DPD34EN_MASK, HAL_AFE_DPD12EN|HAL_AFE_DPD34EN);
                    orig_value = READ_FIELD(HAL_AFE_DPDHSSEL);
                    WRITE_FIELD(HAL_AFE_DPDHSSEL,0);//bypass EQ
                    target_mv = 0;
                    break;
#if (CHIP_REV >= 0xC0)
                case TX_OS:
                    return SEQ_EX0;//temperary skip TXOS calibration
                    polarity = READ_FIELD(HAL_AFE_TXINV);
                    break;
#endif
            }
            break;

        case TYPE4:
            switch (ARG1)
            {
#if (CHIP_REV < 0xC1)
                case DPDGM_OS:
    #if (CHIP_REV >= 0xC0)
                    target_mv = 2500; // for C0
    #else
                    target_mv = 2400; //based on CVT measurement
    #endif
                    break;
#endif
#if (CHIP_REV >= 0xC0)
                case RFREFP_OS:
                    target_mv = v21_fm_mux1_mV;
                    WRITE_FIELD(HAL_AFE_RFINGN,(2 << (HAL_AFE_RFINGN_MSB - HAL_AFE_RFINGN_LSB)) - 1);
                    orig_value = READ_FIELD(HAL_AFE_RFCOSP);
                    WRITE_FIELD(HAL_AFE_RFCOSP,0);
                    break;

                case RFREFN_OS:
                    target_mv = v21_fm_mux1_mV;
                    WRITE_FIELD(HAL_AFE_RFINGN,(2 << (HAL_AFE_RFINGN_MSB - HAL_AFE_RFINGN_LSB)) - 1);
                    orig_value = READ_FIELD(HAL_AFE_RFCOSN);
                    WRITE_FIELD(HAL_AFE_RFCOSN,0);
                    break;
#endif
            }
            break;
    }
    //-----------------------------------------------------------------------;
    // calculate for the case that WLDON = 0                                 ;
    //-----------------------------------------------------------------------;
    retry = 3;
    do
    {
        ret = calCalibrateOffset(ARG1,                  // adjusted field
                                 ARG3,                  // type of offset structure
                                 NORMAL_OFFSET,         // offset type
                                 target_mv,             // target level       unit: mV
                                 polarity,              // polarity between offset & signal
                                 CALIB_FROM_0);
        if (ret != eNOERR)
        {
            retry--;
#if(ERROR_CAL_START_UP_MONITOR ==1)
            svoMiscVar.calibrationErrorCount++;
#endif
#if (CHIP_REV >= 0xC0)
        if ((ARG3 == TYPE4) && ((ARG1 == RFREFP_OS) || (ARG1 == RFREFN_OS)) || (ARG2 == TEMP_OS))
            // ==> Calib touch limitation! Never mind. Result: 33, In STAT:0, TRN:20, SEQ:7
            SendMsgCn(SHOW_DEBUG_MSG,4,0x3A0004,ret,(svoIramVar.svoInState&STAT_MASK)>>12,((svoIramVar.svoInState&TRN_MASK)>>5)+1,(svoIramVar.svoInState&SEQ_MASK)+1);
        else
#endif
            SendMsgCnB(SHOW_DEBUG_MSG,4,0x3A0001,B1B(ret),B1B(((svoIramVar.svoInState & STAT_MASK) >> 12)),B1B(((svoIramVar.svoInState & TRN_MASK ) >> 5) + 1),B1B((svoIramVar.svoInState & SEQ_MASK) + 1));
        }
    }
    while ((ret != eNOERR) && (retry >= 0));

#if (MPPBG_SPPBG_CALIB != 0)
    // Calib MPP_OS, SPP1_OS with different MPPB_GN, SPP1B_GN
    if ((ARG1 < MAX_TYPE2BAL_OFS_NO) && (ARG3 == TYPE2))
    {
        calSetAfeOffset(0,ARG1,TYPE2BAL,NORMAL_OFFSET,0,last_cal_result);
        if (ARG1 == MPP_OS)
        {
            WRITE_FIELD(HAL_AFE_MPPOBG,orig_value + MPPBG_MAX_UNB);
        }
        else
        {
            WRITE_FIELD(HAL_AFE_SPP1BG,orig_value + SPP1BG_MAX_UNB);
        }
        Delay100uS(10);
        retry = 3;
        do
        {
            ret = calCalibrateOffset(ARG1,                  // adjusted field
                                     ARG3,                  // type of offset structure
                                     NORMAL_OFFSET,         // offset type
                                     target_mv,             // target level       unit: mV
                                     polarity,              // polarity between offset & signal
                                     CALIB_FROM_CURRENT);
            if (ret != eNOERR)
            {
                retry--;
                last_err = ret;
    #if(ERROR_CAL_START_UP_MONITOR ==1)
                svoMiscVar.calibrationErrorCount++;
    #endif
                SendMsgCnB(SHOW_DEBUG_MSG,4,0x3A0001,B1B(ret),B1B(((svoIramVar.svoInState & STAT_MASK) >> 12)),B1B(((svoIramVar.svoInState & TRN_MASK ) >> 5) + 1),B1B((svoIramVar.svoInState & SEQ_MASK) + 1));
            }
        }
        while ((ret != eNOERR) && (retry >= 0));

        calSetAfeOffset(0,ARG1,TYPE2BAL,NORMAL_OFFSET,1,last_cal_result);

        if (ARG1 == MPP_OS)
        {
            last_cal_result = calSetBalGain(MPPB_GN,orig_value);
        }
        else
        {
            last_cal_result = calSetBalGain(SPP1B_GN,orig_value);
        }
        // MPP_OS (%ddB) = %02Xh, MPP_OS (%ddB) = %02Xh, MPP_OS (%ddB) = %02Xh (READ MODE)
        // SPP1_OS (%ddB) = %02Xh, SPP1_OS (%ddB) = %02Xh, SPP1_OS (%ddB) = %02Xh (READ MODE)
        SendMsgCn(DEBUG_CALIB_RESULT_MSG,6,0x310538+ARG1, (orig_value-MPPBG_MAX_UNB-16)/2, calSBYTE2DacCount(svoCalBalOfsT2Var[ARG1][0].normal_offset,8),
                                                            (orig_value-16)/2, last_cal_result,
                                                            (orig_value+MPPBG_MAX_UNB-16)/2, calSBYTE2DacCount(svoCalBalOfsT2Var[ARG1][1].normal_offset,8));
    }
#endif

    if (ARG2 == TEMP_OS)
    {
        switch (ARG3)
        {
            case TYPE1:
                WRITE_ADR_1B(AfeOffsetInfoT1[ARG1].field_groove_ofs[RECDL],last_cal_result);
                WRITE_ADR_1B(AfeOffsetInfoT1[ARG1].field_groove_ofs[RECDH],last_cal_result);
                WRITE_ADR_1B(AfeOffsetInfoT1[ARG1].field_land_ofs[RECDL],last_cal_result);
                WRITE_ADR_1B(AfeOffsetInfoT1[ARG1].field_land_ofs[RECDH],last_cal_result);
                break;

            case TYPE2:
                WRITE_ADR_1B(AfeOffsetInfoT2[ARG1].field_normal_ofs,last_cal_result);
                break;

            case TYPE3:
                WRITE_ADR_1B(AfeOffsetInfoT3[ARG1].field_normal_ofs,last_cal_result);
                break;
        }
    }
    else
    {
        calSetAfeOffset(0,ARG1,ARG3,PATH_SELECT_ALL,PATH_SELECT_ALL,last_cal_result);
    }

    if ((ARG1 == SPP2_OS) && (ARG2 == NO_WGOS) && (ARG3 == TYPE2))
    {
        // <L0 / 1> FEO_OS = %02Xh, SPP2_OS = %02Xh
        SendMsgCn(DEBUG_CALIB_RESULT_MSG,2,0x310506+Layer_Index,svoCalOfsT2Var[Layer_Index][FEO_OS].normal_offset,svoCalOfsT2Var[Layer_Index][SPP2_OS].normal_offset);
    }

    switch (ARG3)
    {
        case TYPE1:
            switch (ARG1)
            {
                case TE_OS:
#if (CHIP_REV >= 0xC1)
                    WRITE_FIELD(HAL_AFE_DPDRISEN, 0);
                    WRITE_FIELD(HAL_AFE_DPDFALEN, 0);
#else
                    WRITE_FIELD(HAL_AFE_DPDCPHLD,0);
#endif
                    WRITE_FIELD(HAL_AFE_TRKG,orig_value);

                    // TE_OS: GROOVE = %02Xh, LAND = %02Xh, WRITE = %02Xh
                    SendMsgCn(DEBUG_CALIB_RESULT_MSG,3,0x3105C0+ARG1,svoCalOfsT1Var[0][ARG1].groove_offset[RECDH],svoCalOfsT1Var[0][ARG1].land_offset[RECDH],svoCalOfsT1Var[0][ARG1].write_offset);
                    break;

                case FE_OS:
                    WRITE_FIELD(HAL_AFE_FCSG,orig_value);
                    break;
            }
            break;

        case TYPE2:
            switch (ARG1)
            {
                case TE2_OS:
                    svoCalVar.teOfsData.all16 = CONVERT_ADCMV_TO_DSPOFS(target_mv);
#if (CHIP_REV >= 0xC1)
                    WRITE_FIELD(HAL_AFE_DPDRISEN, 1);
                    WRITE_FIELD(HAL_AFE_DPDFALEN, 1);
#else
                    WRITE_FIELD(HAL_AFE_DPDCPHLD,0);
#endif
                    WRITE_FIELD(HAL_AFE_TE2GNREG,orig_value);
#if (AFE_NORMALIZER == 1)
                    if (svoVar_TrackingMethod != TRK_METHOD_DPD)
                    {
                        WRITE_FIELD(HAL_AFE_TE2AUTOEN, 1);
                    }
#endif
                    break;

                case FE2_OS:
                    svoCalVar.feOfsData.all16 = CONVERT_ADCMV_TO_DSPOFS(target_mv);
                    WRITE_FIELD(HAL_AFE_FE2GNREG,orig_value);
#if (AFE_NORMALIZER == 1)
                    WRITE_FIELD(HAL_AFE_FE2AUTOEN, 1);
#endif
                    break;

                case BS_OS:
                    WRITE_FIELD(HAL_AFE_BSKG,orig_value);
                    break;

                case BS2_OS:
                    svoCalVar.bsOfsData.all16 = CONVERT_ADCMV_TO_DSPOFS(target_mv);
                    WRITE_FIELD(HAL_AFE_BS2GNREG,orig_value);
#if (AFE_NORMALIZER == 1)
                    WRITE_FIELD(HAL_AFE_BS2AUTOEN, 1);
#endif
                    break;

                case SRF_OS:
#if 0//RFIN_GN, RF_GN measurement
                    if (ARG2 != TEMP_OS)
                    {
                        orig_value = READ_FIELD(HAL_AFE_RFHSSEL);
                        WRITE_FIELD(HAL_AFE_RFHSSEL,halAFE_RFLPFBk_Bypass);
                        WRITE_FIELD(HAL_AFE_RFINSEL,halAFE_RFINSEL3);//select SRFO as RF input
                        WRITE_FIELD(HAL_AFE_RFAGCREG,0x10);
                        WRITE_FIELD(HAL_AFE_RFAOCREG,0x00);
                        afeSetCalibMUX(TYPE3, RFP_OS);
                        {
                            SBYTE rfingn,rfgn,srfos;

                            for (rfingn = 3; rfingn >= 0; rfingn--)
                            {
                                WRITE_FIELD(HAL_AFE_RFINGN,rfingn);
                                WRITE_FIELD(HAL_AFE_RFGN,3);
    #if (CHIP_REV >= 0xC0)
                                ret = calCalibrateOffset(RFREFP_OS,             // adjusted field
                                                         TYPE4,                 // type of offset structure
                                                         NORMAL_OFFSET,         // offset type
                                                         v21_fm_mux1_mV,        // target level       unit: mV
                                                         FALSE,                 // polarity between offset & signal
                                                         CALIB_FROM_CURRENT);
                                WRITE_FIELD(HAL_AFE_RFVREFPOS,last_cal_result);
                                ret = calCalibrateOffset(RFREFN_OS,             // adjusted field
                                                         TYPE4,                 // type of offset structure
                                                         NORMAL_OFFSET,         // offset type
                                                         v21_fm_mux1_mV,        // target level       unit: mV
                                                         TRUE,                  // polarity between offset & signal
                                                         CALIB_FROM_CURRENT);
                                WRITE_FIELD(HAL_AFE_RFVREFNOS,last_cal_result);
    #endif
                                ret = calCalibrateOffset(RFCP_OS,               // adjusted field
                                                         TYPE2,                 // type of offset structure
                                                         NORMAL_OFFSET,         // offset type
                                                         v21_fm_mux1_mV,        // target level       unit: mV
                                                         TRUE,                  // polarity between offset & signal
                                                         CALIB_FROM_0);
                                WRITE_FIELD(HAL_AFE_RFCOSP,last_cal_result);
                                ret = calCalibrateOffset(RFCN_OS,               // adjusted field
                                                         TYPE2,                 // type of offset structure
                                                         NORMAL_OFFSET,         // offset type
                                                         0,                     // target level       unit: mV
                                                         TRUE,                  // polarity between offset & signal
                                                         CALIB_FROM_0);
                                WRITE_FIELD(HAL_AFE_RFCOSN,last_cal_result);
                                for (rfgn = (HAL_AFE_RFGN_MASK >> HAL_AFE_RFGN_LSB); rfgn >= 0; rfgn--)
                                {
                                    WRITE_FIELD(HAL_AFE_RFGN,rfgn);
                                    SendMsgCn(SHOW_DEBUG_MSG,2,0x310584,rfingn,rfgn);
                                    for (srfos = -25; srfos <= 25; srfos++)
                                    {
                                        WRITE_FIELD(HAL_AFE_SRFOSA,calSBYTE2DacCount(calDacCount2SBYTE(svoCalOfsT2Var[Layer_Index][SRF_OS].normal_offset,8)+srfos,8));
                                        WRITE_FIELD(HAL_AFE_RFTPEN,TRUE);
                                        WRITE_FIELD(HAL_AFE_RFANATSEL, halAFE_RFANATSEL2);//RFVGAP/N
                                        WRITE_FIELD(HAL_AFE_EQDOSEL, halAFE_EQDOSEL5);
                                        WRITE_FIELD(HAL_AFE_MUX1ADSEL, halAFE_MUX1ADSEL_SRFO);
                                        WaitTimer(1);
                                        send_msgA2(SHOW_DEBUG_MSG|DEBUG_SERVO_TRACE_MSG,0x090100+srfos,ReadAverageAdcmV(ADC_MUXOUT1, 7));
                                        WRITE_FIELD(HAL_AFE_MUX1ADSEL, halAFE_MUX1ADSEL_RFTP1);
                                        WaitTimer(1);
                                        send_msgA2(SHOW_DEBUG_MSG|DEBUG_SERVO_TRACE_MSG,0x091100+srfos,readPNmV(ADC_MUXOUT1, ADC_MUXOUT2, NO_REVOLUTION, FALSE).middle_mv);
                                        WRITE_FIELD(HAL_AFE_RFANATSEL, halAFE_RFANATSEL3);//RFAGCP/N
                                        WRITE_FIELD(HAL_AFE_EQDOSEL, halAFE_EQDOSEL5);
                                        WaitTimer(1);
                                        send_msgA2(SHOW_DEBUG_MSG|DEBUG_SERVO_TRACE_MSG,0x092100+srfos,readPNmV(ADC_MUXOUT1, ADC_MUXOUT2, NO_REVOLUTION, FALSE).middle_mv);
                                        WRITE_FIELD(HAL_AFE_EQDOSEL, halAFE_EQDOSEL6);//RFOP/N
                                        WaitTimer(1);
                                        send_msgA2(SHOW_DEBUG_MSG|DEBUG_SERVO_TRACE_MSG,0x093100+srfos,readPNmV(ADC_MUXOUT1, ADC_MUXOUT2, NO_REVOLUTION, FALSE).middle_mv);
                                        WRITE_FIELD(HAL_AFE_RFTPEN,FALSE);
                                        WRITE_FIELD(HAL_AFE_RFANATSEL, halAFE_RFANATSEL0);
                                        WaitTimer(1);
                                        send_msgA2(SHOW_DEBUG_MSG|DEBUG_SERVO_TRACE_MSG,0x094100+srfos,readADCmVPerRevs(NO_REVOLUTION, FALSE, RF_ADC).middle_mv);
                                    }
                                    WRITE_FIELD(HAL_AFE_SRFOSA,svoCalOfsT2Var[Layer_Index][SRF_OS].normal_offset);
                                }
                            }
                        }
                        WRITE_FIELD(HAL_AFE_RFINGN,svoCalGnT3Var[Layer_Index][RFIN_GN].normal_gain);
                        WRITE_FIELD(HAL_AFE_RFGN,svoCalGnT2Var[Layer_Index][RF_GN].normal_gain);
                        LAL_WriteCoef(DFE_HFAMPMCFG);
                        LAL_WriteCoef(AFE_INIT_RF04_RFINSEL);
                        WRITE_FIELD(HAL_AFE_RFHSSEL,orig_value);
                    }
#endif
                    break;

                case RFCN_OS:
                    WRITE_FIELD(HAL_AFE_RFGN,orig_value);
                case RFCP_OS:
                    WRITE_FIELD(HAL_AFE_RFTPEN,FALSE);
                    WRITE_FIELD(HAL_AFE_RFANATSEL, halAFE_RFANATSEL0);
                    break;

#if (EN_POWER_CONTROL == 1)
                case RADO_OS:
                case RBCO_OS:
                    WRITE_FIELD(HAL_AFE_PDWOBALL,orig_prdn);
                    WRITE_FIELD(HAL_AFE_PDWOB,(orig_prdn>>1));
                    WRITE_FIELD(HAL_AFE_PDINPWBL,(orig_prdn>>2));
                    WRITE_FIELD(HAL_AFE_PDREADWOB,(orig_prdn>>3));
                    WRITE_FIELD(HAL_AFE_PDWRTWOB,(orig_prdn>>4));
                    WRITE_FIELD(HAL_AFE_PDWPP12,(orig_prdn>>5));
                    break;
#endif
            }
            break;

        case TYPE3:
            switch (ARG1)
            {
                case CE_OS:
                    svoCalVar.ceOfsData.all16 = CONVERT_ADCMV_TO_DSPOFS(target_mv);
                    WRITE_FIELD(HAL_AFE_CEGN,orig_value);
                    SendMsgCn(DEBUG_CALIB_RESULT_MSG,1,0x310514+Layer_Index,svoCalOfsT3Var[Layer_Index][CE_OS].normal_offset);
                    break;

                case RP_OS:
                    WRITE_FIELD(HAL_AFE_RPGN,orig_value);
                    break;

                case RP2_OS:
                    svoCalVar.rpOfsData.all16 = CONVERT_ADCMV_TO_DSPOFS(target_mv);
                    WRITE_FIELD(HAL_AFE_RP2GN,orig_value);
                    //RP adaptive slicer RPpp and RCINV related settings
                    target_mv = COEF_GetValue(AFE_CALC_RP_PP_TARGET_10MV) * 10;
                    WRITE_FIELD(HAL_AFE_RPPOL,!READ_FIELD(HAL_AFE_RCINV));
                    HAL_AFE_SetPhBhDrooprate(e_RPPHBH, (float)target_mv / 1000);
                    target_mv = target_mv / (4 * 3300 / 16);
                    if (svoVar_discKindT.Bit.DiscStd == eDiscDVD)
                    {
                        WRITE_FIELD(HAL_AFE_RCTH,7);
                        target_mv ++;
                    }
                    else
                    {
                        if (target_mv > 0) target_mv--;
                    }
                    WRITE_FIELD(HAL_AFE_RCTH2,target_mv);
                    SendMsgCn(DEBUG_CALIB_RESULT_MSG,4,0x310532+Layer_Index,svoCalGnT3Var[Layer_Index][RPB_GN].normal_gain,svoCalGnT3Var[Layer_Index][RP_GN].normal_gain,svoCalOfsT3Var[Layer_Index][RP_OS].normal_offset,svoCalOfsT3Var[Layer_Index][RP2_OS].normal_offset);
                    break;

#if ((NEW_RFPOS_CALIB == 0) || (DVDRAM_USE_OLD_RFPOS_CALIB == 1))
                case RFP_OS:
                    WRITE_FIELD(HAL_AFE_RFGN, (HAL_AFE_RFGN_MASK >> HAL_AFE_RFGN_LSB));
                    LAL_WriteCoef(DFE_HFAMPMCFG);
                    break;
#endif
                case RFPGA_OS:
#if (NEW_RFPOS_CALIB == 1) && (NEW_RFPOS_CALIB_BRING_AGCVREF_TO_ZERO == 0)
                    if (READ_FIELD(HAL_AFE_RFGN) == (HAL_AFE_RFGN_MASK >> HAL_AFE_RFGN_LSB))
                    {
                        for (retry = 0; retry < 3; retry++)
                        {
                            WRITE_FIELD(HAL_AFE_RFHSSEL, retry + 1);
                            for (nr_calibs = 0; nr_calibs < 32; nr_calibs++)
                            {
                                WRITE_FIELD(HAL_AFE_RFHSFC, nr_calibs);
                                target_mv = calGetGainVref(HAL_AFE_RFAGCREG_ADR,HAL_AFE_RFAGCREG_MASK,HAL_AFE_RFAGCREG_LSB,ADC_SPECIAL_RFDFE);
                                SendMsgCn(DEBUG_CALIB_RESULT_MSG,4,0x310563,A1B(retry),A1B(nr_calibs),A2B(target_mv));//<L0> svoRRFVar.rfagc_ref[%u][%2u] = %4d (mV)
                                if (target_mv > 1270) target_mv = 1270;
                                if (target_mv < -1280) target_mv = -1280;
                                svoRRFVar.rfagc_ref_LPF[retry][nr_calibs] = RoundDivide(target_mv, 10);
                            }
                        }
                        HAL_AFE_SetRfBw(svo_bReadZoneSpeedX10, COEF_GetValue(AFE_CALC_BwRfRatioX100), svoVar_TrackingMethod);
                        SendMsgCnB(DEBUG_CALIB_RESULT_MSG,4,0x310563,B1B(READ_FIELD(HAL_AFE_RFHSSEL) == 3),B1B(READ_FIELD(HAL_AFE_RFHSFC)),B2B(svoRRFVar.rfagc_ref));//<L0> svoRRFVar.rfagc_ref[%u][%2u] = %4d (mV)
                    }
#endif
                    LAL_WriteCoef(DFE_HFAMPMCFG);
                    break;

                case DPDCH1_OS:
                case DPDCH2_OS:
                case DPDCH3_OS:
                case DPDCH4_OS:
                    if (svoVar_TrackingMethod != TRK_METHOD_DPD)
                    {
                        WRITE_FIELD(HAL_AFE_PDDPD, TRUE);   //970[5]=1,Power down the whole DPD block
                        READ_MOD_WRITE(HAL_AFEREG_MAP->RF20, HAL_AFE_DPD12EN_MASK|HAL_AFE_DPD34EN_MASK, 0);
                    }
                    WRITE_FIELD(HAL_AFE_DPDGN, svoCalGnT3Var[Layer_Index][DPD_GN].normal_gain);
#if (CHIP_REV >= 0xC0)
                    //update DPDGN1~4
                    READ_MOD_WRITE(HAL_AFEREG_MAP->RF07,HAL_AFE_DPDGN1SEL|HAL_AFE_DPDGN2SEL,HAL_AFE_DPDGN1SEL|HAL_AFE_DPDGN2SEL);
                    READ_MOD_WRITE(HAL_AFEREG_MAP->RF09,HAL_AFE_DPDGN3SEL|HAL_AFE_DPDGN4SEL,HAL_AFE_DPDGN3SEL|HAL_AFE_DPDGN4SEL);
                    READ_MOD_WRITE(HAL_AFEREG_MAP->RF07,HAL_AFE_DPDGN1SEL|HAL_AFE_DPDGN2SEL,0);
                    READ_MOD_WRITE(HAL_AFEREG_MAP->RF09,HAL_AFE_DPDGN3SEL|HAL_AFE_DPDGN4SEL,0);
#endif
                    WRITE_FIELD(HAL_AFE_RFTPEN,FALSE);
                    WRITE_FIELD(HAL_AFE_DPDANATEN, FALSE);
                    WRITE_FIELD(HAL_AFE_DPDHSSEL,orig_value);
                    break;
#if (CHIP_REV >= 0xC0) /* Temp set 0 to avoid seek poor for some IC */
                case TX_OS:
                    calSetAfeOffset(0,ARG1,ARG3,PATH_SELECT_ALL,PATH_SELECT_ALL,0);
                    break;
#endif
            }
            break;

        case TYPE4:
            switch (ARG1)
            {
#if (CHIP_REV < 0xC1)
                case DPDGM_OS:
                    WRITE_FIELD(HAL_AFE_RFTPEN,FALSE);
                    break;
#endif
#if (CHIP_REV >= 0xC0)
                case RFREFP_OS:
                    WRITE_FIELD(HAL_AFE_RFINGN,svoCalGnT3Var[Layer_Index][RFIN_GN].normal_gain);
                    WRITE_FIELD(HAL_AFE_RFCOSP,orig_value);
                    break;

                case RFREFN_OS:
                    WRITE_FIELD(HAL_AFE_RFINGN,svoCalGnT3Var[Layer_Index][RFIN_GN].normal_gain);
                    WRITE_FIELD(HAL_AFE_RFCOSN,orig_value);
                    break;
#endif
            }
            break;
    }

#if (DVD_RAM_READ == 1)
    // Restore LAND control.
    if (DVDRAM)
    {
        WRITE_FIELD(HAL_AFE_LANDSEL,1);
    }
#endif
    //TBD:need add later
    //if ((ARG3 == TYPE2) && (ARG2 != TEMP_OS) && ((last_err == eERR_adj_next_offset_over_high_limit) || (last_err == eERR_adj_next_offset_over_low_limit)))
    //{
    //    if ((ARG1 == MPP_OS) || (ARG1 == FEO_OS))
    //    {
    //        calSetMisc(0,MAX_MB_GN,svoCalMisc2Var.max_mb_gn - 1);
    //        return SEQ_EX1;
    //    }
    //    if ((ARG1 == SPP1_OS) || (ARG1 == SPP2_OS))
    //    {
    //        calSetMisc(0,MAX_SB_GN,svoCalMisc2Var.max_sb_gn - 1);
    //        return SEQ_EX1;
    //    }
    //}

    return(SEQ_EX0);
}

/*-----------------------------------------------------------------------------
 *
 * Module:      state_mbsbvrefof_cali()
 * Description: calibration MBVREFOS and SBVREFOS.
 * Input:       ARG2: DO_WGOS, NO_WGOS
 *              ARG3: CALIB_FROM_0, CALIB_FROM_CURRENT
 *
 *----------------------------------------------------------------------------*/
seq_exit_t state_mbsbvrefof_cali(void)
{
    errcode_t ret;
    SBYTE     retry;

    if ((svoCalVar.calOfsDoneBits & havcof_end) == havcof_end)
    {
        return(SEQ_EX0);
    }
    //send_msg80(DEBUG_AUTO_TUNING_MSG,0x516500);

    //-----------------------------------------------------------------------;
    // calculate for the case that WLDON = 0                                 ;
    //-----------------------------------------------------------------------;
    /*
    EFM_SMP_Control(READMODE,SMP_ON);
    DelaymS(1);
    */
    //open_svo_msg(DEBUG_AUTO_TUNING_MSG);
    WRITE_FIELD(HAL_AFE_SRFOSA,0);
    WRITE_FIELD(HAL_AFE_MBGUPSEL,0x1F);
    WRITE_FIELD(HAL_AFE_SBADOSA,0);
    WRITE_FIELD(HAL_AFE_SBGUPSEL,0x1F);
    retry = 3;
    do
    {
        ret = calCalibrateOffset(MBVREF_OS,                 // adjusted field
                                 TYPE2,                     // type of offset structure
                                 NORMAL_OFFSET,             // offset mode
                                 vref_fm_mux3_mV,           // target level       unit: mV
                                 1,                         // polarity between offset & signal
                                 ARG3);                     // offset cali. from current offset DAC or from 0 DAC
        if (ret != eNOERR)
        {
            retry--;
#if(ERROR_CAL_START_UP_MONITOR ==1)
            svoMiscVar.calibrationErrorCount++;
#endif
            SendMsgCnB(SHOW_DEBUG_MSG,4,0x3A0001,B1B(ret),B1B(((svoIramVar.svoInState & STAT_MASK) >> 12)),B1B(((svoIramVar.svoInState & TRN_MASK ) >> 5) + 1),B1B((svoIramVar.svoInState & SEQ_MASK) + 1));
        }
    }
    while ((ret != eNOERR) && (retry >= 0));

    calSetAfeOffset(0,MBVREF_OS,TYPE2,PATH_SELECT_ALL,0,last_cal_result);
    calSetAfeOffset(0,VREF_OS,TYPE3,0,0,last_cal_result);
    WRITE_FIELD(HAL_AFE_SRFOSA,svoCalOfsT2Var[0][SRF_OS].normal_offset);
    WRITE_FIELD(HAL_AFE_MBGUPSEL, svoCalGnT2Var[0][MB_GN].normal_gain);
    retry = 3;
    do
    {
        ret = calCalibrateOffset(SBVREF_OS,                 // adjusted field
                                 TYPE2,                     // type of offset structure
                                 NORMAL_OFFSET,             // offset mode
                                 vref_fm_mux3_mV,           // target level       unit: mV
                                 1,                         // polarity between offset & signal
                                 ARG3);                     // offset cali. from current offset DAC or from 0 DAC

        if (ret != eNOERR)
        {
            retry--;
#if(ERROR_CAL_START_UP_MONITOR ==1)
            svoMiscVar.calibrationErrorCount++;
#endif
            SendMsgCnB(SHOW_DEBUG_MSG,4,0x3A0001,B1B(ret),B1B(((svoIramVar.svoInState & STAT_MASK) >> 12)),B1B(((svoIramVar.svoInState & TRN_MASK ) >> 5) + 1),B1B((svoIramVar.svoInState & SEQ_MASK) + 1));
        }
    }
    while ((ret != eNOERR) && (retry >= 0));

    //close_svo_msg(DEBUG_AUTO_TUNING_MSG);
    calSetAfeOffset(0,SBVREF_OS,TYPE2,PATH_SELECT_ALL,0,last_cal_result);
    WRITE_FIELD(HAL_AFE_SBADOSA,svoCalOfsT2Var[0][SBAD_OS].normal_offset);
    WRITE_FIELD(HAL_AFE_SBGUPSEL, svoCalGnT2Var[0][SB_GN].normal_gain);

    SendMsgCn(DEBUG_CALIB_RESULT_MSG,3,0x310500,svoCalOfsT2Var[0][MBVREF_OS].normal_offset,svoCalOfsT3Var[0][VREF_OS].normal_offset,svoCalOfsT2Var[0][SBVREF_OS].normal_offset);

    if (ARG2 == DO_WGOS)
    {
        svoCalVar.calOfsDoneBits |= havcof_end;
    }

    return(SEQ_EX0);
}

/*-----------------------------------------------------------------------------
 *
 * Module: state_fe_pp_meas_init()
 * Description:
 *
 *----------------------------------------------------------------------------*/
seq_exit_t state_fe_pp_meas_init(void)
{
//    HAL_LASC_SetLaserOn(TRUE);
    GAIN_RECD;

    //make sure sled and colli is in place.
    while(svoStp_busy());
    {
        ExitProcess();
    }

    if ((svoCalVar.calGainDoneBits & feg_bsg_end) == feg_bsg_end)
    {
  	HAL_LASC_SetLaserOn(TRUE);
        return(SEQ_EX1);
    }

    SendMsg80(SHOW_DEBUG_MSG,0x33FF0C); //Measure FE P2P
    LAL_WriteCoef(AFE_FCTS07_FCSG);
    LAL_WriteCoef(AFE_FCTS05_BLANKFCSG);
    LAL_WriteCoef(AFE_BSRP25_BSKG);
    LAL_WriteCoef(AFE_BSRP24_BLANKBSKG);

    return(SEQ_EX0);
}

/*-----------------------------------------------------------------------------
 *
 * Module: state_disc_chk_cunt()
 * Description:
 *
 *----------------------------------------------------------------------------*/
seq_exit_t state_disc_chk_cunt(void)
{
    float factor_0db;
    SBYTE orig_MBGN,orig_SBGN,adj;
#if (DVD_RAM_READ == 1)
    SBYTE adj1;
#endif

    if (svoRampMeasVar.saturated == TRUE)
    {
        orig_MBGN = READ_FIELD(HAL_AFE_MBGUPSEL);
        orig_SBGN = READ_FIELD(HAL_AFE_SBGUPSEL);
        adj = (orig_MBGN < 2)? orig_MBGN : 2;
        adj = (orig_SBGN < adj)? orig_SBGN : adj;

        if (adj > 0)
        {
            if (Layer_Index == 1)
            {
                WRITE_FIELD(HAL_AFE_MBGUPSEL,orig_MBGN-adj);
                WRITE_FIELD(HAL_AFE_SBGUPSEL,orig_SBGN-adj);
            }
            else
            {
                if (svoPrvtVar_focusP2PCount > 0)
                {
                    svoPrvtVar_maxSRFO = (svoPrvtVar_avgSRFO[0] > svoPrvtVar_avgSRFO[1])? svoPrvtVar_avgSRFO[0] : svoPrvtVar_avgSRFO[1];
                    svoPrvtVar_maxSRFO = ((ULONG)svoPrvtVar_maxSRFO * GPADC_IN_RANGE) / (svoPrvtVar_focusP2PCount << GPADC_BITS);
                    svoPrvtVar_avgFOO /= svoPrvtVar_focusP2PCount;
                    calSetMisc(0,FOO_OS,svoPrvtVar_avgFOO);

                    /* Calculate Main/Sub Beam Input Gain */
                    LAL_AFE_GetMbSbInpValues(svoPrvtVar_maxSRFO, svoRampMeasVar.SRFO_gain - svoCalMisc2Var.pdic_gn - svoCalMisc2Var.laser_gn);
                }

#if (DVD_RAM_READ == 1)
                // FE_pp_measure function sometimes measure worst value from noise, it cause FE_pp too large and MB_GN too low for DVDRAM
                // So, we use MIN_MBGN_DVDRAM to protect MB_GN too low.
                if ((svoCalGnT2Var[Layer_Index][MB_GN].normal_gain < MIN_MBGN_DVDRAM) && (orig_MBGN > MIN_MBGN_DVDRAM)
                    && (svoVar_discKindT.Bit.DiscStd == eDiscDVD) && (svoVar_discKindT.Bit.Media  == eMediaRW))
                {
                    adj1 = MIN_MBGN_DVDRAM - svoCalGnT2Var[Layer_Index][MB_GN].normal_gain;
                    calSetAfeGain(Layer_Index,MB_GN,TYPE2,PATH_SELECT_ALL,0,MIN_MBGN_DVDRAM);
                    calSetAfeGain(Layer_Index,SB_GN,TYPE2,PATH_SELECT_ALL,0,READ_FIELD(HAL_AFE_SBGUPSEL)+adj1);
                }
#endif

                if ((svoCalGnT2Var[Layer_Index][MB_GN].normal_gain + adj) > orig_MBGN)
                {
                    calSetAfeGain(Layer_Index,MB_GN,TYPE2,PATH_SELECT_ALL,0,orig_MBGN-adj);
                    calSetAfeGain(Layer_Index,SB_GN,TYPE2,PATH_SELECT_ALL,0,orig_SBGN-adj);
                }

#if (DVD_RAM_READ == 1)
                if (DVDRAM)
                {
                    // For DVD-RAM discs, skip peak/bottom-hold and RP offset calibration.
                    svoCalVar.calOfsDoneBits = rpof_end;
                }
                else
#endif
                {
                svoCalVar.calOfsDoneBits = 0;
            }
            }
#if (FE_SLEW_RATE_LIMITER == 2)
            SlewRateShift.all32 = HW_FE_SLEW_RATE;
#endif
            // <L0 / 1> MB_GN = %02Xh, SB_GN = %02Xh
            SendMsgCn(DEBUG_CALIB_RESULT_MSG,2,0x310550+Layer_Index,READ_FIELD(HAL_AFE_MBGUPSEL),READ_FIELD(HAL_AFE_SBGUPSEL));
            return (SEQ_EX2);
        }
    }

    if (ARG1 == 1)
    {
        svoPrvtVar_avgSRFO[0] = 0;
        svoPrvtVar_avgFOO = 0;
        svoPrvtVar_focusP2PCount = 0;
    }

    if (svoPrvtVar_focusP2PCount >= FOCUS_PP_READ_COUNT)
    {
        // Hold lens at current position.
        ramp_stop();
#if (FOOFFSET2_ENABLE == 1)
        // Move lens to initial position
        ramp_FOOS(0);
#endif

        // Calculate FEpp, BSpp and SRFO.
#if (STREAM_STM == 0)
        svoPrvtVar_avgBSpp[0] = ((ULONG)svoPrvtVar_avgBSpp[0] * DSPADC_IN_RANGE) / (svoPrvtVar_focusP2PCount << DSPADC_BITS);
#else
        svoPrvtVar_avgBSpp[0] = ((ULONG)svoPrvtVar_avgBSpp[0] * GPADC_IN_RANGE) / (svoPrvtVar_focusP2PCount << GPADC_BITS);
#endif
        factor_0db = CalculateRateFromXdB(P10dB,-svoRampMeasVar.SRFO_gain);
        if (svoPrvtVar_avgBSpp[0] == 0)
            svoPrvtVar_avgBSpp[0] = 1;

#if (STREAM_STM == 0)
        svoPrvtVar_avgFEpp[0] = ((ULONG)svoPrvtVar_avgFEpp[0] * DSPADC_IN_RANGE) / (svoPrvtVar_focusP2PCount << DSPADC_BITS);
#else
        svoPrvtVar_avgFEpp[0] = ((ULONG)svoPrvtVar_avgFEpp[0] * GPADC_IN_RANGE) / (svoPrvtVar_focusP2PCount << GPADC_BITS);
#endif
        svoPrvtVar_maxFEpp[0] = svoPrvtVar_avgFEpp[0];
        svoPrvtVar_avgSRFO[0] = ((ULONG)svoPrvtVar_avgSRFO[0] * GPADC_IN_RANGE) / (svoPrvtVar_focusP2PCount << GPADC_BITS);

        // <L0> FEpp = %4d (mV), BSpp = %3d (mV), SRFO = %3d (mV)
        SendMsgCn(DEBUG_DISK_ID_MSG,6,0x316301,A2B(svoPrvtVar_avgFEpp[0]), A2B(svoPrvtVar_avgBSpp[0]), A2B(svoPrvtVar_avgSRFO[0]));

        if (svoVar_discKindT.Bit.Layers == eLayersDouble)
        {
#if (STREAM_STM == 0)
            svoPrvtVar_avgBSpp[1] = ((ULONG)svoPrvtVar_avgBSpp[1] * DSPADC_IN_RANGE) / (svoPrvtVar_focusP2PCount << DSPADC_BITS);
            svoPrvtVar_avgFEpp[1] = ((ULONG)svoPrvtVar_avgFEpp[1] * DSPADC_IN_RANGE) / (svoPrvtVar_focusP2PCount << DSPADC_BITS);
#else
            svoPrvtVar_avgBSpp[1] = ((ULONG)svoPrvtVar_avgBSpp[1] * GPADC_IN_RANGE) / (svoPrvtVar_focusP2PCount << GPADC_BITS);
            svoPrvtVar_avgFEpp[1] = ((ULONG)svoPrvtVar_avgFEpp[1] * GPADC_IN_RANGE) / (svoPrvtVar_focusP2PCount << GPADC_BITS);
#endif
            svoPrvtVar_avgSRFO[1] = ((ULONG)svoPrvtVar_avgSRFO[1] * GPADC_IN_RANGE) / (svoPrvtVar_focusP2PCount << GPADC_BITS);
#if (BD_ENABLE == 1)
            if (svoVar_discKindT.Bit.DiscStd == eDiscBD)
            {
                if (Layer_Index == 0)
                {
                    //because the colli pos is set for L0, L1 signals become smaller than they should be
    #if ((BD_ENABLE == 1) && (NEW_BDDL_START_FLOW == 1))
                    svoCalDlVar[0].fe_0db = svoPrvtVar_avgFEpp[0] * factor_0db;
                    svoCalDlVar[0].srfo_0db = svoPrvtVar_avgSRFO[0] * factor_0db;
                    svoCalDlVar[1].fe_0db = svoPrvtVar_avgFEpp[1] * factor_0db;
                    svoCalDlVar[1].srfo_0db = svoPrvtVar_avgSRFO[1] * factor_0db;
    #endif
                    svoPrvtVar_avgBSpp[1] *= BS_FACTOR_BD_L1;
                    svoPrvtVar_avgFEpp[1] = svoPrvtVar_avgFEpp[1] * svoCalDlVar[1].fe_factor / 1024;
                    svoPrvtVar_avgSRFO[1] *= SRFO_FACTOR_BD_L1;
                }
                else
                {
    #if ((BD_ENABLE == 1) && (NEW_BDDL_START_FLOW == 1))
                    SendMsgCnB(SHOW_DEBUG_MSG,8,0x31630A,F4B((float)svoPrvtVar_avgFEpp[1] * factor_0db / svoCalDlVar[1].fe_0db),
                        F4B((float)svoPrvtVar_avgSRFO[1] * factor_0db / svoCalDlVar[1].srfo_0db));//<L1> FE_FACTOR_BD=%1.2f, SRFO_FACTOR_BD=%1.2f
                    SendMsgCnB(SHOW_DEBUG_MSG,8,0x316309,F4B((float)svoCalDlVar[0].fe_0db / (svoPrvtVar_avgFEpp[0] * factor_0db)),
                        F4B((float)svoCalDlVar[0].srfo_0db / (svoPrvtVar_avgSRFO[0] * factor_0db)));//<L0> FE_FACTOR_BD=%1.2f, SRFO_FACTOR_BD=%1.2f
                    svoCalDlVar[0].fe_factor = 1024 * svoCalDlVar[0].fe_0db / (svoPrvtVar_avgFEpp[0] * factor_0db);
                    svoCalDlVar[1].fe_factor = 1024 * svoPrvtVar_avgFEpp[1] * factor_0db / svoCalDlVar[1].fe_0db;
    #endif
                    svoPrvtVar_avgBSpp[0] *= BS_FACTOR_BD_L1;
                    svoPrvtVar_avgFEpp[0] = svoPrvtVar_avgFEpp[0] * svoCalDlVar[0].fe_factor / 1024;
                    svoPrvtVar_avgSRFO[0] *= SRFO_FACTOR_BD_L1;
                }
            }
#endif // (BD_ENABLE == 1)
            if (svoPrvtVar_avgBSpp[1] == 0)
                svoPrvtVar_avgBSpp[1] = 1;

            svoPrvtVar_maxFEpp[1] = svoPrvtVar_avgFEpp[1];

 // <L1> FEpp = %4d (mV), BSpp = %3d (mV), SRFO = %3d (mV)
            SendMsgCn(DEBUG_DISK_ID_MSG,6,0x316302,A2B(svoPrvtVar_avgFEpp[1]), A2B(svoPrvtVar_avgBSpp[1]), A2B(svoPrvtVar_avgSRFO[1]));
        }

        svoPrvtVar_avgFOO /= svoPrvtVar_focusP2PCount;
#ifdef FEBG_MEASURE
        orig_MBGN = READ_FIELD(HAL_AFE_FEBG);
        if (fe_gn[orig_MBGN] == 0)
        {
            fe_gn[orig_MBGN] = svoPrvtVar_avgFEpp[Layer_Index];
            if ((svoVar_discKindT.Bit.Layers == eLayersDouble) && (svoVar_discKindT.Bit.DiscStd != eDiscBD))
            {
                fe_gn[orig_MBGN] += svoPrvtVar_avgFEpp[1];
            }
            if (orig_MBGN > 0)
            {
                orig_MBGN--;
                WRITE_FIELD(HAL_AFE_FEBG, orig_MBGN);
                fe_gn[orig_MBGN] = 0;
            }
            else
            {
                LAL_WriteCoef(AFE_FCTS31_FEBG);
            }
            svoPrvtVar_focusP2PCount = 0;
            svoPrvtVar_avgFOO = 0;
            svoPrvtVar_avgBSpp[0] = 0;
            svoPrvtVar_avgBSpp[1] = 0;
            svoPrvtVar_avgSRFO[0] = 0;
            svoPrvtVar_avgSRFO[1] = 0;
            svoPrvtVar_avgFEpp[0] = 0;
            svoPrvtVar_avgFEpp[1] = 0;
            svoPrvtVar_maxFEpp[0] = 0;
            svoPrvtVar_maxFEpp[1] = 0;
            bFocMoveReference = 0x80;
            bLayerErrorCount = 0;
    #if (FE_SLEW_RATE_LIMITER == 2)
            SlewRateShift.all32 = HW_FE_SLEW_RATE;
    #endif
            return (SEQ_EX1);
        }
#endif

        if (Layer_Index == 0)
        {
#if 0
            //To measure FEpp,SRFOpp,and BSpp with different colli position to determine FE_FACTOR_BD_L1, BS_FACTOR_BD_L1 and SRFO_FACTOR_BD_L1.
            //Beware that the L1 result already multiplied by XX_L1_FACTOR_BD.
            if (Iram_colli_CurrentPosition != (calGetOptColliPos(0) + ((calGetOptColliPos(0) < calGetOptColliPos(1))? -32 : 32))
            {
                if (calGetOptColliPos(0) < calGetOptColliPos(1))
                {
                    if (Iram_colli_CurrentPosition <= (calGetOptColliPos(1) + 640))
                    {
                        if (Iram_colli_CurrentPosition == calGetOptColliPos(0))
                        {
                            svoStp_JumpSteps(COLLI_NORMAL_MODE,-640);
                        }
                        else
                        {
                            svoStp_JumpSteps(COLLI_NORMAL_MODE,96);
                        }
                    }
                    else
                    {
                        svoStp_JumpToPosition(COLLI_NORMAL_MODE,(calGetOptColliPos(0) - 32));
                    }
                }
                else
                {
                    if (Iram_colli_CurrentPosition >= (calGetOptColliPos(1) - 640))
                    {
                        if (Iram_colli_CurrentPosition == calGetOptColliPos(0))
                        {
                            svoStp_JumpSteps(COLLI_NORMAL_MODE,+640);
                        }
                        else
                        {
                            svoStp_JumpSteps(COLLI_NORMAL_MODE,-96);
                        }
                    }
                    else
                    {
                        svoStp_JumpToPosition(COLLI_NORMAL_MODE,(calGetOptColliPos(0) + 32));
                    }
                }
                while (svoStp_busy())
                {
                    ExitProcess();
                }
                send_msgA2(SHOW_DEBUG_MSG,0x540101,Iram_colli_CurrentPosition);

                svoPrvtVar_focusP2PCount = 0;
                svoPrvtVar_avgFOO = 0;
                svoPrvtVar_avgBSpp[0] = 0;
                svoPrvtVar_avgBSpp[1] = 0;
                svoPrvtVar_avgSRFO[0] = 0;
                svoPrvtVar_avgSRFO[1] = 0;
                svoPrvtVar_avgFEpp[0] = 0;
                svoPrvtVar_avgFEpp[1] = 0;
                svoPrvtVar_maxFEpp[0] = 0;
                svoPrvtVar_maxFEpp[1] = 0;
                bFocMoveReference = 0x80;
                bLayerErrorCount = 0;
    #if (FE_SLEW_RATE_LIMITER == 2)
                SlewRateShift.all32 = HW_FE_SLEW_RATE;
    #endif
                return(SEQ_EX1);
            }
#endif // #if 0
            calSetMisc(0,FOO_OS,svoPrvtVar_avgFOO);

            // STM Calibrates PDIC gain and gaincal calibrates MBGUPSEL. These calibrations
            // influence the measured SRFO Level, so the measured SRFO should be adapted.
            svoPrvtVar_maxFEpp_0dB = svoPrvtVar_avgFEpp[0];
            svoPrvtVar_maxSRFO = svoPrvtVar_avgSRFO[0];
            if ((svoVar_discKindT.Bit.Layers == eLayersDouble) && (svoPrvtVar_avgSRFO[1] > svoPrvtVar_maxSRFO))
            {
                svoPrvtVar_maxSRFO = svoPrvtVar_avgSRFO[1];
                svoPrvtVar_maxFEpp_0dB = svoPrvtVar_avgFEpp[1];
            }
            svoPrvtVar_maxSRFOpp_0dB = svoPrvtVar_maxSRFO * factor_0db;
            svoPrvtVar_maxFEpp_0dB = svoPrvtVar_maxFEpp_0dB * factor_0db;
        }
        else
        {
            WRITE_FIELD(HAL_AFE_MBGUPSEL, svoCalGnT2Var[Layer_Index][MB_GN].normal_gain);
            WRITE_FIELD(HAL_AFE_SBGUPSEL, svoCalGnT2Var[Layer_Index][SB_GN].normal_gain);
            WRITE_FIELD(HAL_AFE_SRFOSA,   svoCalOfsT2Var[Layer_Index][SRF_OS].normal_offset);
            WRITE_FIELD(HAL_AFE_SBADOSA,  svoCalOfsT2Var[Layer_Index][SBAD_OS].normal_offset);
            WRITE_FIELD(HAL_AFE_FEOOSA,   svoCalOfsT2Var[Layer_Index][FEO_OS].normal_offset);
            WRITE_FIELD(HAL_AFE_SPP2OSA,  svoCalOfsT2Var[Layer_Index][SPP2_OS].normal_offset);
            WRITE_FIELD(HAL_AFE_BLANKFCSO,svoCalOfsT1Var[Layer_Index][FE_OS].groove_offset[RECDL]);
            WRITE_FIELD(HAL_AFE_FCSO,     svoCalOfsT1Var[Layer_Index][FE_OS].groove_offset[RECDH]);
            WRITE_FIELD(HAL_AFE_BLNDFCSO, svoCalOfsT1Var[Layer_Index][FE_OS].land_offset[RECDL]);
            WRITE_FIELD(HAL_AFE_LNDFCSO,  svoCalOfsT1Var[Layer_Index][FE_OS].land_offset[RECDH]);
        }

        //*** divided by zero protection ***
        if (svoPrvtVar_avgFEpp[0] == 0)
            svoPrvtVar_avgFEpp[0] = 1;

        if (svoPrvtVar_maxFEpp_0dB == 0)
            svoPrvtVar_maxFEpp_0dB = 1;

        if (svoPrvtVar_maxSRFOpp_0dB == 0)
            svoPrvtVar_maxSRFOpp_0dB = 1;

        cal_retry = 0;

        return (SEQ_EX0);
    }
    return (SEQ_EX1);
}

/*-----------------------------------------------------------------------------
 *
 * Module: state_FE_p2p_init()
 * Description:
 *
 *----------------------------------------------------------------------------*/
seq_exit_t state_FE_p2p_init(void)
{
    svoPrvtVar_focusP2PCount = 0;
    svoPrvtVar_avgFOO = 0;
    svoPrvtVar_avgBSpp[0] = 0;
    svoPrvtVar_avgBSpp[1] = 0;
    svoPrvtVar_avgSRFO[0] = 0;
    svoPrvtVar_avgSRFO[1] = 0;
    svoPrvtVar_avgFEpp[0] = 0;
    svoPrvtVar_avgFEpp[1] = 0;
    svoPrvtVar_maxFEpp[0] = 0;
    svoPrvtVar_maxFEpp[1] = 0;
    bFocMoveReference = 0x80;
    bLayerErrorCount = 0;
    cal_retry = 0;
#if (FE_SLEW_RATE_LIMITER == 2)
    SlewRateShift.all32 = HW_FE_SLEW_RATE;
#endif
    svoRampMeasVar.SRFO_gain = (READ_FIELD(HAL_AFE_MBGUPSEL) + READ_FIELD(HAL_AFE_SRFOBGA) - 32) * 10 + svoCalMisc2Var.pdic_gn + svoCalMisc2Var.laser_gn;

#if (AFE_NORMALIZER == 1)
    afeSvognInit();
    afeSvognDisable();
    afeSvognCtrlOn();
#endif // AFE_NORMALIZER == 1

    spdWaitSpdInRange();

    return(SEQ_EX0);
}

/*-----------------------------------------------------------------------------
 *
 * Module: state_FEpp_ReMeas()
 * Description: Determine if FEpp re-measure is necessary.
 *              When disk kind changed to RW type, AFE setting need to be reload
 *              In this case, Re-measure is needed.
 *
 *----------------------------------------------------------------------------*/
seq_exit_t state_FEpp_ReMeas(void)
{
    if ((svoCalVar.calGainDoneBits & feg_bsg_end) == feg_bsg_end)
    {
        return(SEQ_EX1);
    }

    if (!(svoCalVar.discChkDoneBits & start_end))
    {
#if 0 //debug code to measure ACC / DEC
        ULONG  timer;
        USHORT cur_rpm,cur_time;

        svoCurvefitCalVar.cal_index = 0;
        spdHoldRPM(1000);
        spdWaitSpdInRange();
        WaitTimer(2500);
        spdHoldRPM(8000);
        StartTimer(&timer);
        svoCurvefitCalVar.cal_index = 0;
        while (!svoSpeedVar.SpdInRange)
        {
            WaitTimer(10);
            cur_rpm = spdGetCurrentRPM();
            cur_time = ReadTimer(&timer);
            send_msgA2(1,0x090000+cur_time,spdGetCurrentRPM());
            if (cur_rpm > svoCurvefitCalVar.cal_index * (8000 - 1000) / 20 + 1000)
            {
                svoCurvefitCalVar.item[svoCurvefitCalVar.cal_index] = cur_rpm;
                svoCurvefitCalVar.cost[svoCurvefitCalVar.cal_index] = cur_time;
                svoCurvefitCalVar.cal_index++;
            }
            if (svoSpeedVar.SpdMonitor)
            {
                spdMonitorRPM();
                svoSpeedVar.SpdMonitor = FALSE;
            }
        }
        CurveFit(svoCurvefitCalVar.cal_index, svoCurvefitCalVar.item, svoCurvefitCalVar.cost, svoCurvefitCalVar.cof);
        send_msg74(SHOW_DEBUG_MSG,0x09FFFF,fsqrt(1.0 / svoCurvefitCalVar.cof[2]));
        WaitTimer(2500);
        spdHoldRPM(1000);
        StartTimer(&timer);
        svoCurvefitCalVar.cal_index = 0;
        while (!svoSpeedVar.SpdInRange)
        {
            WaitTimer(10);
            cur_rpm = spdGetCurrentRPM();
            cur_time = ReadTimer(&timer);
            send_msgA2(1,0x090000+cur_time,spdGetCurrentRPM());
            if (cur_rpm < svoCurvefitCalVar.cal_index * (1000 - 8000) / 20 + 8000)
            {
                svoCurvefitCalVar.item[svoCurvefitCalVar.cal_index] = cur_rpm;
                svoCurvefitCalVar.cost[svoCurvefitCalVar.cal_index] = cur_time;
                svoCurvefitCalVar.cal_index++;
            }
            if (svoSpeedVar.SpdMonitor)
            {
                spdMonitorRPM();
                svoSpeedVar.SpdMonitor = FALSE;
            }
        }
        CurveFit(svoCurvefitCalVar.cal_index, svoCurvefitCalVar.item, svoCurvefitCalVar.cost, svoCurvefitCalVar.cof);
        send_msg74(SHOW_DEBUG_MSG,0x09FFFF,fsqrt(1.0 / svoCurvefitCalVar.cof[2]));
#endif
#if 0 //debug code to measure SPD_INT_PARA_1~3 for 12/8cm disc
        ULONG timer,cnt;
        float spd_int;
        BYTE  index;

        svoCurvefitCalVar.cal_index = 0;
        for (TargetRPM = 1000; TargetRPM <= 8000; TargetRPM += 500)
        {
            spdHoldRPM(TargetRPM);
            spdWaitSpdInRange();
            WaitTimer(2500);
            spd_int = 0;
            cnt = 0;
            StartTimer(&timer);
            do
            {
                spd_int += READ_DSP_COEF(DE4_msb);
                cnt++;
            }
            while (ReadTimer(&timer) < 50);
            svoCurvefitCalVar.item[svoCurvefitCalVar.cal_index] = TargetRPM;
            svoCurvefitCalVar.cost[svoCurvefitCalVar.cal_index] = spd_int / cnt;
            svoCurvefitCalVar.cal_index++;
        }
        index = svoCurvefitCalVar.cal_index;
        for (TargetRPM = 8000; TargetRPM >= 1000; TargetRPM -= 500)
        {
            spdHoldRPM(TargetRPM);
            spdWaitSpdInRange();
            WaitTimer(2500);
            spd_int = 0;
            cnt = 0;
            StartTimer(&timer);
            do
            {
                spd_int += READ_DSP_COEF(DE4_msb);
                cnt++;
            }
            while (ReadTimer(&timer) < 50);
            index--;
            svoCurvefitCalVar.cost[index] = (spd_int / cnt + svoCurvefitCalVar.cost[index]) * svoSpeedVar.Kd14Fg / (0x80 * DMODAC_FACTOR * 2);
        }
        CurveFit(svoCurvefitCalVar.cal_index, svoCurvefitCalVar.item, svoCurvefitCalVar.cost, svoCurvefitCalVar.cof);
        for (index = 0; index < svoCurvefitCalVar.cal_index; index++)
        {
            //SPD_INT_PARA_X meas: RPM = %.0f, DE4_msb = %.4f
            SendMsgCnB(SHOW_DEBUG_MSG,8,0x316342,F4B(svoCurvefitCalVar.item[index]),F4B(svoCurvefitCalVar.cost[index]));
        }
        //SPD_INT_PARA_X meas: SPD_INT_PARA_1 = %.1f, SPD_INT_PARA_2 = %.1f, SPD_INT_PARA_3 = %.1f
        SendMsgCnB(SHOW_DEBUG_MSG,12,0x316343,F4B(1.0 / svoCurvefitCalVar.cof[2]),F4B(svoCurvefitCalVar.cof[1] * 262144),F4B(svoCurvefitCalVar.cof[0]));
#endif
    }
    spdUnhold();
    SendMsg80(DEBUG_AUTO_TUNING_MSG,0x316340); //****FE_pp_ADJ_TBL****
    SendMsg80(DEBUG_CALIB_ENTRYS_MSG,0x3A0108); //>>> STATE_FE_PP_ADJ <<<
    return(SEQ_EX0);
}

/*-----------------------------------------------------------------------------
 *
 * Module: state_FE_gn_adjust()
 * Description: adjust FE_GN based on svoPrvtVar_avgFEpp[]
 *
 *----------------------------------------------------------------------------*/
seq_exit_t state_FE_gn_adjust(void)
{
    USHORT fe_target;
    SBYTE  layer;
    SBYTE  gain_orig;
    SBYTE  gain_adj;

    SendMsg80(SHOW_DEBUG_MSG,0x33FE02);                                     // == FE and BS Gain Adjust ==

    fe_target = COEF_GetValue(AFE_CALC_FE_PP_TARGET_10MV) * 10;
    gain_orig = COEF_GetValue(AFE_FCTS07_FCSG);
    SendMsgCn(DEBUG_AUTO_TUNING_MSG,2,0x316303,A2B(fe_target)); // Target_FEpp(mV)

    layer = (svoVar_discKindT.Bit.Layers == eLayersSingle)? 0 : 1;
    for (; layer >= Layer_Index; layer--)
    {
        gain_adj = CalculateXdBFromRate(ONEdB, fe_target, svoPrvtVar_avgFEpp[layer]);
        gain_adj -= (READ_FIELD(HAL_AFE_MBGUPSEL) + READ_FIELD(HAL_AFE_SRFOBGA) - 32 + RoundDivide(svoCalMisc2Var.pdic_gn + svoCalMisc2Var.laser_gn - svoRampMeasVar.SRFO_gain, 10));
        SendMsgCn(DEBUG_AUTO_TUNING_MSG,1,0x316305+layer,A1B(gain_adj));    // FE_Change_Gain Lx = %1d (dB)

        calSetAfeGain(layer,FE_GN,TYPE1,PATH_SELECT_ALL,PATH_SELECT_ALL, gain_orig + gain_adj);
    }

#if (CHIP_REV >= 0xC0)
    //WRITE_DSP_COEF(fcs_gain_in, (DSPADC_IN_RANGE << (16 - DSPADC_BITS)) * 7 / fe_target);

//Temperaly disable new FDP protection to prevent len hit noise. Need to check SCRATCH_PROT_FOO_CD, SCRATCH_PROT_FOO_DVD, SCRATCH_PROT_FOOBD later
    //svoPrvtVar_FEDG_thr = (fe_target << (DSPADC_BITS - 3)) * 3 / DSPADC_IN_RANGE;//set FEDG_thr to 3/4 of FE S-curve
    svoPrvtVar_FEDG_thr = 0x7F;//
    //WRITE_DSP_COEF(FEDG_thr, svoPrvtVar_FEDG_thr);

    #if (FOO_INVERSION == 0)
    WRITE_DSP_COEF(FEDG_pol,0x7FFF);
    #else
    WRITE_DSP_COEF(FEDG_pol,0x8000);
    #endif

    switch (svoVar_discKindT.Bit.DiscStd)
    {
        case eDiscCD:
            svoPrvtVar_ProtFoos = SCRATCH_PROT_FOO_CD;
            svoPrvtVar_FEDG_thr = svoPrvtVar_FEDG_thr * 7 / 5;
            break;

        case eDiscDVD:
            svoPrvtVar_ProtFoos = SCRATCH_PROT_FOO_DVD;
            break;
    #if (BD_ENABLE == 1)
        case eDiscBD:
            if (svoVar_discKindT.Bit.Layers == eLayersDouble)
            {
                //colli set to one layer the other layer's FE-S-curve is not symmetrical
                svoPrvtVar_ProtFoos = 0;
            }
            else
                svoPrvtVar_ProtFoos = SCRATCH_PROT_FOO_BD;
            break;
    #endif
    #if (ENABLE_HDDVD == 1)
        case eDiscHD:
            svoPrvtVar_ProtFoos = SCRATCH_PROT_FOO_HD;
            break;
    #endif
    }
    WRITE_DSP_COEF(FEDG_thr, svoPrvtVar_FEDG_thr);
#endif

    return(SEQ_EX0);
}

/*-----------------------------------------------------------------------------
 *
 * Module: state_BS_gn_adjust()
 * Description: adjust BS_GN based on svoPrvtVar_avgBSpp[]
 *
 *----------------------------------------------------------------------------*/
seq_exit_t state_BS_gn_adjust(void)
{
    USHORT bs_target;
#if (AFE_NORMALIZER == 1)
    USHORT bs_target_nor;
#endif // AFE_NORMALIZER == 1
    SBYTE  layer;
    SBYTE  gain_orig;
    SBYTE  gain_adj;

#if (DVD_RAM_READ == 1)
    // For DVD-RAM, decrease avgBSpp level by 20% to eliminate the effect of header noise while FE PP measuring.
    if(DVDRAM)
    {
        svoPrvtVar_avgBSpp[0] = (svoPrvtVar_avgBSpp[0] * 4) / 5;
    }
#endif
    bs_target = COEF_GetValue(AFE_CALC_BS_TARGET_10MV) * 10;
    SendMsgCn(DEBUG_AUTO_TUNING_MSG,2,0x316304,A2B(bs_target)); // Target_BSpp(mV)
    gain_orig = COEF_GetValue(AFE_BSRP25_BSKG);
    layer = (svoVar_discKindT.Bit.Layers == eLayersSingle)? 0 : 1;

    for (; layer >= Layer_Index; layer--)
    {
        gain_adj = CalculateXdBFromRate(ONEdB, bs_target, svoPrvtVar_avgBSpp[layer]);
#if (AFE_NORMALIZER == 1)
        bs_target_nor = ((svoPrvtVar_avgBSpp[layer] << DSPADC_BITS) * CalculateRateFromXdB(ONEdB,gain_adj)) / DSPADC_IN_RANGE;
#endif // AFE_NORMALIZER == 1
        gain_adj -= (READ_FIELD(HAL_AFE_MBGUPSEL) + READ_FIELD(HAL_AFE_SRFOBGA) - 32 + RoundDivide(svoCalMisc2Var.pdic_gn + svoCalMisc2Var.laser_gn - svoRampMeasVar.SRFO_gain, 10));
        SendMsgCn(DEBUG_AUTO_TUNING_MSG,1,0x316307+layer,A1B(gain_adj)); // BS_Change_Gain L0(dB)
        calSetAfeGain(layer,BS_GN,TYPE2,PATH_SELECT_ALL,0, gain_orig + gain_adj);
        SendMsgCnB(SHOW_DEBUG_MSG,2,0x316312+layer,B1B(svoCalGnT1Var[layer][FE_GN].write_gain-16),
            B1B(svoCalGnT2Var[layer][BS_GN].normal_gain-16));// FCSG, BSKG Lx(dB)
#if (AFE_NORMALIZER == 1)
        calSetAfeOffset(layer,THR_BS,NOR_BS,0,0,(bs_target_nor + 4) / 8);
        bs_target_nor += ((vref_fm_mux3_mV - DSPADC_IN_MIN) << DSPADC_BITS) / DSPADC_IN_RANGE;
        if (bs_target_nor + svoCalBsNorVar[layer][THR_BS].normal_bs > 0xFE)
        {
            SendMsg80(SHOW_DEBUG_MSG,0x320A13);
            gain_adj = (bs_target_nor + svoCalBsNorVar[layer][THR_BS].normal_bs - 0xFD) / 2;
            calSetAfeOffset(layer,THR_BS,NOR_BS,0,0,svoCalBsNorVar[layer][THR_BS].normal_bs - gain_adj);
            bs_target_nor -= gain_adj;
        }
        calSetAfeOffset(layer,TAR_BS,NOR_BS,0,0,bs_target_nor);
        WRITE_FIELD(HAL_AFE_BSRATE,1); //BS Update rate = 33.8688MHz/512 = 66.15KHz
#endif // AFE_NORMALIZER == 1
    }

    svoCalVar.calGainDoneBits |= feg_bsg_end;

    return(SEQ_EX0);
}

/*-----------------------------------------------------------------------------
 *
 * Module: state_rp_bal_cali()
 * Description:
 *      signal(s)          RP1, RP
 *      calibration tools  a, offset
 *      run RP avg calibration
 *
 *----------------------------------------------------------------------------*/
#define INIT_RP_OS 80
seq_exit_t state_rp_bal_cali(void)
{
    BYTE      RPsource;
    errcode_t ret;

    if ((svoCalVar.calGainDoneBits & rpbg_end) == rpbg_end)
    {
        return(SEQ_EX0);
    }
    SendMsg80(DEBUG_AUTO_TUNING_MSG,0x3A0128); //>>> STATE_RPBG_CALI <<<
    ret = eNOERR;
    RPsource = READ_REG(HAL_AFEREG_MAP->BSRP10) & (HAL_AFE_RPSRF_MASK|HAL_AFE_RPSBA_MASK|HAL_AFE_RPPHS_MASK|HAL_AFE_RPBHS_MASK);
    if (ARG1 == ON)
    {//focus on
        if ((RPsource == (HAL_AFE_RPSRF_MASK|HAL_AFE_RPSBA_MASK)) || (RPsource == (HAL_AFE_RPSBA_MASK|HAL_AFE_RPBHS_MASK)))
        {
            ret = calCalibrateRPBG(CONVERT_DSPOFS_TO_ADCMV(svoCalVar.rpOfsData.all16));
            //TBD:need check for 711
            //if ((svoVar_discKindT.Bit.DiscStd == eDiscDVD) && (svoVar_discKindT.Bit.Layers == eLayersDouble) && (ret == eNOERR))
            //
            //    last_cal_result += 6;//For DL part of the SBAD is comming from other layer
            //}
        }
        else
        {
            SendMsg80(DEBUG_CALIB_ENTRYS_MSG,0x3A0002); // ==>Calib SKIP!!
            return(SEQ_EX0);
        }
    }
    else
    { //RP based on SRFO (PHSO,BHSO also based on SRFO) only, we adjust SRFO_OS during focus off to adjust RPB_GN
        if ((RPsource == (HAL_AFE_RPSRF_MASK|HAL_AFE_RPPHS_MASK)) || (RPsource == (HAL_AFE_RPPHS_MASK|HAL_AFE_RPBHS_MASK)))
        {
            ret = calCalibrateRPBG(0);
            //TBD:need check for 711
            if (ret == eNOERR)
            {
                last_cal_result -= 2;//SRFO middle is roughly 1dB lower than PHSO middle when tracking off.
            }
        }
        else
        {
            SendMsg80(DEBUG_CALIB_ENTRYS_MSG,0x3A0002); // ==>Calib SKIP!!
            return(SEQ_EX0);
        }
    }

    if (ret == eNOERR)
    {
        if ((ARG1 == OFF) || (svoVar_discKindT.Bit.Layers == eLayersSingle) || (Layer_Index == 1))
        {
            svoCalVar.calGainDoneBits |= rpbg_end;
        }
    }
    else
    {
#if(ERROR_CAL_START_UP_MONITOR ==1)
        svoMiscVar.calibrationErrorCount++;
#endif
        SendMsgCnB(SHOW_DEBUG_MSG,4,0x3A0001,B1B(ret),B1B(((svoIramVar.svoInState & STAT_MASK) >> 12)),B1B(((svoIramVar.svoInState & TRN_MASK ) >> 5) + 1),B1B((svoIramVar.svoInState & SEQ_MASK) + 1));
        if (++cal_retry < 3)
        {
            return(SEQ_BUSY);
        }
    }
    cal_retry = 0;
    calSetAfeGain(Layer_Index,RPB_GN,TYPE3,0,0,last_cal_result);

    return SEQ_EX0;
}

/*-----------------------------------------------------------------------------
 *
 * Module: state_rpg_cali()
 * Description:
 *      run RP gain calibration for both normal power and added power case ;
 *
 *----------------------------------------------------------------------------*/
seq_exit_t state_rpg_cali(void)
{
    errcode_t ret;
    BYTE      RPsource;
    USHORT    bCurOffsetCnt,target_p2p;

    if (svoCalVar.calGainDoneBits & rpg_end)
    {
        return(SEQ_EX0);
    }

    SendMsg80(DEBUG_CALIB_ENTRYS_MSG,0x3A0114); // >>> STATE_RPGnOs_CALI <<<

    RPsource = READ_REG(HAL_AFEREG_MAP->BSRP10) & (HAL_AFE_RPSRF_MASK|HAL_AFE_RPSBA_MASK|HAL_AFE_RPPHS_MASK|HAL_AFE_RPBHS_MASK);
    if ((RPsource != (HAL_AFE_RPSRF_MASK|HAL_AFE_RPSBA_MASK)) && (RPsource != (HAL_AFE_RPSBA_MASK|HAL_AFE_RPBHS_MASK)))
    {
        if (((RPsource == HAL_AFE_RPSRF_MASK) || (RPsource == HAL_AFE_RPSBA_MASK)) && (Layer_Index == 0))
        {
            bCurOffsetCnt = calDacCount2SBYTE(READ_FIELD(HAL_AFE_RPOS),8) + (READ_FIELD(HAL_AFE_RPSRF)? INIT_RP_OS : -INIT_RP_OS);
            WRITE_FIELD(HAL_AFE_RPOS,calSBYTE2DacCount(bCurOffsetCnt,8));
        }
        ret = calCalibrateRPOS();
        if (ret != eNOERR)
        {
#if(ERROR_CAL_START_UP_MONITOR ==1)
            svoMiscVar.calibrationErrorCount++;
#endif
            SendMsgCnB(SHOW_DEBUG_MSG,4,0x3A0001,B1B(ret),B1B(((svoIramVar.svoInState & STAT_MASK) >> 12)),B1B(((svoIramVar.svoInState & TRN_MASK ) >> 5) + 1),B1B((svoIramVar.svoInState & SEQ_MASK) + 1));
            if (++cal_retry < 3)
            {
                return(SEQ_BUSY);
            }
        }
        calSetAfeOffset(Layer_Index,RP_OS,TYPE3,0,0,last_cal_result);
    }

    target_p2p = COEF_GetValue(AFE_CALC_RP_PP_TARGET_10MV) * 10;
    ret = calCalibrateRPGnOs(target_p2p);

    if (ret != eNOERR)
    {
#if(ERROR_CAL_START_UP_MONITOR ==1)
        svoMiscVar.calibrationErrorCount++;
#endif
        SendMsgCnB(SHOW_DEBUG_MSG,4,0x3A0001,B1B(ret),B1B(((svoIramVar.svoInState & STAT_MASK) >> 12)),B1B(((svoIramVar.svoInState & TRN_MASK ) >> 5) + 1),B1B((svoIramVar.svoInState & SEQ_MASK) + 1));
        if (++cal_retry < 3)
        {
            return(SEQ_BUSY);
        }
    }
    cal_retry = 0;
    if ((svoVar_discKindT.Bit.Layers == eLayersSingle) || (Layer_Index == 1))
    {
#if 0//for Y-error adjustment, insert a BDR(E) ECC disc to measure and adjust Y-error
        while (servo_debug[7])
        {
            ReadYerrRevs(FOUR_REVOLUTIONS);
        }
#endif
        svoCalVar.calGainDoneBits |= rpg_end;
        svoSeekVar.rx_available = RX_AVAILABLE;
    }
    calSetAfeGain(Layer_Index,RP_GN,TYPE3,0,0,last_cal_result);
    calSetAfeOffset(Layer_Index,RP2_OS,TYPE3,0,0,last_cal_result2);
    SendMsgCn(DEBUG_CALIB_RESULT_MSG,4,0x310532+Layer_Index,svoCalGnT3Var[Layer_Index][RPB_GN].normal_gain,svoCalGnT3Var[Layer_Index][RP_GN].normal_gain,svoCalOfsT3Var[Layer_Index][RP_OS].normal_offset,svoCalOfsT3Var[Layer_Index][RP2_OS].normal_offset);

#if (RP_AUTO_ADJUST == 1)
    if ((BDRDLDISC || BDREDLDISC) && (Layer_Index == 1))
    {
        float GainX;

        GainX = CalculateRateFromXdB(ONEdB,-16+svoCalGnT3Var[0][RP_GN].normal_gain);
        CalRP2Gain[0] = (2*65536) / (5*GainX); //CalRP2Gain=dBToX * 65536(Q16) /2.5(each step of RP2Ofs)

        GainX = CalculateRateFromXdB(ONEdB,-16+svoCalGnT3Var[1][RP_GN].normal_gain);
        CalRP2Gain[1] = (2*65536) / (5*GainX); //CalRP2Gain=dBToX * 65536(Q16) /2.5(each step of RP2Ofs)
    }
#endif

    return(SEQ_EX0);
}// end of state_rpg_cali

#if (DETECT_ECC_DISC == 1)
/*-----------------------------------------------------------------------------
 *
 * Module: state_ecc_chk1()
 * Description:
 *
 *----------------------------------------------------------------------------*/
BYTE ECC_CHK_retry;
seq_exit_t state_ecc_chk1(void)
{
    USHORT temp_tesrcdb = 0x00;//from 720
    ECC_CHK_retry=0;

    switch(svoVar_discKindT.Bit.DiscStd)
    {
    #if (BD_ENABLE == 1)
        case eDiscBD:
        #if (BD_ECC_IMPROVEMENT == 1)
            WRITE_FIELD(HAL_AFE_TEPGN, 0x03);
            WRITE_FIELD(HAL_AFE_BPTXLP, 0x01);
        #endif
        #if ((MAX_READ_BD_SPEED <= e2XDCLV) && (PCB != 815))
            svoCalVar.discChkDoneBits |= ecc_chk_end; //skip ECC detecttion
        #else
            temp_tesrcdb = 0x1C;//from 720
        #endif
            break;
    #endif

    #if (ENABLE_HDDVD == 1)
        case eDiscHD:
        #if (MAX_READ_HD_SPEED <= e2XDCLV)
            svoCalVar.discChkDoneBits |= ecc_chk_end; //skip ECC detecttion
        #else
            temp_tesrcdb = 0x1C;//from 720
        #endif
            break;
    #endif

        case eDiscDVD:
    #if (DVD_RAM_READ == 1)
            if (DVDRAM)
            {
        #if ((MAX_READ_RAM_SPEED <= e5XDCLV)||(MAX_READ_RAM_SPEED == e5XDCAV))
                svoCalVar.discChkDoneBits |= ecc_chk_end; //skip ECC detecttion
        #else
                temp_tesrcdb = 0x40;//from 720
        #endif
            }
            else
    #endif
            {
                 // non-DVDRAM
    #if ((MAX_READ_DVD_SPEED <= e4XDCAV)&& (PCB != 815))
                svoCalVar.discChkDoneBits |= ecc_chk_end; //skip ECC detect
    #else
                temp_tesrcdb = 0x2C;//from 720
    #endif
            }
            break;

        case eDiscCD:
    #if ((MAX_READ_SPEED < e32XCAV)&&(PCB != 815))
            svoCalVar.discChkDoneBits |= ecc_chk_end; //skip ECC detecttion
    #else
            temp_tesrcdb = 0x50;//from 720
    #endif
            break;
    }

    if (svoCalVar.discChkDoneBits & ecc_chk_end)
    {
    #if (BD_ECC_IMPROVEMENT==1)
        if( svoVar_discKindT.Bit.DiscStd == eDiscBD)
        {
            LAL_WriteCoef(AFE_FCTS37_TEPGN);
            LAL_WriteCoef(AFE_INIT_FCTS39_BPTXLP);

        }
   #endif
        SendMsg80(DEBUG_CALIB_ENTRYS_MSG,0x3A0002); // ==> Calib SKIP!!
        return(SEQ_EX1);
    }

    SendMsg80(DEBUG_DISK_ID_MSG,0x33FE14);  // == ECC Disc Detection ==

    //Set TX de-bouncing frequency according formula * safety factor//from 720
    WRITE_FIELD(HAL_SRV_BFILT,0);
    WRITE_FIELD(HAL_SRV_TESRCDB, temp_tesrcdb);

    #if (ELECTRONIC_DAMPER_FOR_ECC_DET == 1)
    seekSetCEEq(eDamper, ELECTRONIC_DAMPER_GAIN);
    HAL_SERVO_CenterControlON(FORW);
    WaitTimer(MS_10);                 // Wait for damper stabilizing
    #else
    seekSetCEEq(eRoughSeek, 0);
    #endif

    svoVar_eccDiscNum = eECC_normal;

    #if (SRC_ENABLE == 1)
    HAL_DSP_SRCEnable(SRC_LOOP_TRACKING, SRC_STATE_OFF);    //Disable Tracking SRC
    #endif

    SetDelayTimer(ARG1);
    return(SEQ_EX0);
}

/*-----------------------------------------------------------------------------
 *
 * Module: state_ecc_chk2()
 * Description:
 *
 *----------------------------------------------------------------------------*/
seq_exit_t state_ecc_chk2(void)
{
    svoVar_EccCheckEnable = 1;// measure 1 revolution
    svoSpeedVar.fgCount    = (DISK_NFGPLS * svoVar_EccCheckEnable) + 1;
    StartFGTimer();

    SetTimeout(0, svoVar_EccCheckEnable * 64000 / svoSpeedVar.TargetRpm + 50);
    return(SEQ_EX0);
}

/*-----------------------------------------------------------------------------
 *
 * Module: state_ecc_chk3()
 * Description:
 *
 *----------------------------------------------------------------------------*/
seq_exit_t state_ecc_chk3(void)
{
    USHORT runout;

    if (timeout_chk(0) == eTimerTimeout)
    {
        StopFGTimer();
        svoVar_EccCheckEnable = 0;
        HAL_SERVO_CenterControlOFF();
        svoVar_eccDiscNum = eECC_140_um;                    // Runout = 140um
        LAL_WriteCoef(SRV_TESRCCTL_TESRCDB);
        svoCalVar.discChkDoneBits |= ecc_chk_end;

    #if (SRC_ENABLE == 1)
        if (svoSpeedVar.SpdSRCEnable == TRUE)
            HAL_DSP_SRCEnable(SRC_LOOP_TRACKING, SRC_STATE_ON); //Enable Tracking SRC
    #endif

    #if (BD_ECC_IMPROVEMENT == 1)
        if (svoVar_discKindT.Bit.DiscStd == eDiscBD)
        {
            LAL_WriteCoef(AFE_FCTS37_TEPGN);
            LAL_WriteCoef(AFE_INIT_FCTS39_BPTXLP);
        }
    #endif
        SendMsg80(DEBUG_DISK_ID_MSG,0x310103);              // ECC_CHECK_TIMEOUT!

        return (SEQ_EX1);
    }

    if ((svoVar_EccCheckEnable == 0) && (svoSpeedVar.fgCount == 0))
    {
        ECC_CHK_retry++;
        switch (svoVar_discKindT.Bit.DiscStd)
        {
            case eDiscDVD:
    #if (DVD_RAM_READ == 1)
                if (DVDRAM)
                {
                    runout = (USHORT)((float)svoRunOut*1.23f); // Convert from # of tracks to micro-meters
                }
                else
    #endif
                {
                    runout = (USHORT)((float)svoRunOut*0.74f); // Convert from # of tracks to micro-meters
                }

                if (runout > DVD_ECC150_TH)
                    svoVar_eccDiscNum = eECC_210_um;   // DVD ECC_150um discs
                else if (runout > DVD_ECC100_TH)
                    svoVar_eccDiscNum = eECC_140_um;   // DVD ECC_100um discs
                else if (runout > DVD_ECC50_TH)
                    svoVar_eccDiscNum = eECC_070_um;   // DVD ECC_50um discs
                else
                    svoVar_eccDiscNum = eECC_normal;   // DVD ECC_normal discs
                break;

    #if (BD_ENABLE == 1)
            case eDiscBD:
                runout = (USHORT)((float)svoRunOut*0.32f); // Convert from # of tracks to micro-meters

                if (runout > BD_ECC150_TH)
                    svoVar_eccDiscNum = eECC_210_um;   // BD ECC_150um discs
                else if (runout > BD_ECC100_TH)
                    svoVar_eccDiscNum = eECC_140_um;   // BD ECC_100um discs
                else if (runout > BD_ECC75_TH)
                    svoVar_eccDiscNum = eECC_070_um;   // BD ECC_75um discs
                else
                    svoVar_eccDiscNum = eECC_normal;   // DVD ECC_normal discs

                #if (BD_ECC_IMPROVEMENT == 1)
                if (svoVar_discKindT.Bit.DiscStd == eDiscBD)
                {
                    LAL_WriteCoef(AFE_FCTS37_TEPGN);
                    LAL_WriteCoef(AFE_INIT_FCTS39_BPTXLP);
                }
                #endif
                break;
    #endif
    #if (ENABLE_HDDVD==1)
            case eDiscHD:
                runout = (USHORT)((float)svoRunOut*0.4f); // Convert from # of tracks to micro-meters

                if (runout > HD_ECC150_TH)
                    svoVar_eccDiscNum = eECC_210_um;   // HD ECC_150um discs
                else if (runout > HD_ECC100_TH)
                    svoVar_eccDiscNum = eECC_140_um;   // HD ECC_100um discs
                else if (runout > HD_ECC75_TH)
                    svoVar_eccDiscNum = eECC_070_um;   // HD ECC_75um discs
                else
                    svoVar_eccDiscNum = eECC_normal;   // DVD ECC_normal discs
                break;
    #endif
            case eDiscCD:
                runout = (USHORT)((float)svoRunOut*1.6f);  // Convert from # of tracks to micro-meters

                if ((runout > CD_ECC280_TH*2)&&(ECC_CHK_retry<3))
                {
                    //send_msg5S(SHOW_DEBUG_MSG,"CHK ECC again");
                    //send_msgA2(SHOW_DEBUG_MSG,0x510102,runout);               // ECC_DISC(um)
                    return(SEQ_EX2);
                }
                else if (runout > CD_ECC280_TH)
                    svoVar_eccDiscNum = eECC_280_um;       // ECC_280um discs
                else if (runout > CD_ECC210_TH)
                    svoVar_eccDiscNum = eECC_210_um;       // ECC_210um discs
                else if (runout > CD_ECC140_TH)
                    svoVar_eccDiscNum = eECC_140_um;       // ECC_140um discs
                else if (runout > CD_ECC070_TH)
                    svoVar_eccDiscNum = eECC_070_um;       // ECC_070um discs
                else
                    svoVar_eccDiscNum = eECC_normal;       // ECC_normal discs
                break;
        }
        // ECC_DISC = %2d(um), ECC_DISC_Det ==> xxx Disc
        SendMsgCn(SHOW_DEBUG_MSG,2,0x310110+svoVar_eccDiscNum,(runout)>>8,(runout));
        LAL_WriteCoef(SRV_TESRCCTL_TESRCDB);
        svoCalVar.discChkDoneBits |= ecc_chk_end;

    #if (ELECTRONIC_DAMPER_FOR_ECC_DET == 1)
        HAL_SERVO_CenterControlOFF();
    #endif
    #if (SRC_ENABLE == 1)
        if (svoSpeedVar.SpdSRCEnable == TRUE)
            HAL_DSP_SRCEnable(SRC_LOOP_TRACKING, SRC_STATE_ON); //Enable Tracking SRC
    #endif

        return (SEQ_EX0);
    }
    else
    {
        return (SEQ_BUSY);
    }
}
#endif//#if (DETECT_ECC_DISC == 1)

/*-----------------------------------------------------------------------------
 *
 * Module: state_select_cailMode()
 * Description:
 *
 *----------------------------------------------------------------------------*/
seq_exit_t state_select_cailMode(void)
{
    if (svoCmdVar.cmdParam[0] == ARG1)
        return(SEQ_EX0);
    else
        return(SEQ_EX1);
}

/*-----------------------------------------------------------------------------
 *
 * Module: state_FEBG_cal()
 * Description: Use curve fitting to find the FE balance gain which gives symmetrical FE
 *              ARG1:ON-on line FEBG calibration during startup; OFF-off line FEBG calibration in the factory
 *              ARG2:Timeout
 *
 *----------------------------------------------------------------------------*/
//debug code to perform FEBG calibration with different TILT. Set it > 0 to enable. Scan range = TILT_INIT_DAC +- FEBG_TILT_SCAN_RANGE
#define FEBG_TILT_SCAN_RANGE 0

//debug code to perform FEBG calibration with different COLLI. Set it > 0 to enable. Scan range = calGetOptColliPos(0) +- FEBG_COLLI_SCAN_RANGE
#define FEBG_COLLI_SCAN_RANGE 0

#define FEBG_CALIB_POINTS  6
#define FEBG_SAMPLE_POINTS 2 //better be even
const float FEBGain[FEBG_CALIB_POINTS] = {0x0B,0x0D,0x0F,0x11,0x13,0x15};  /* FEBG= -2.5dB ~ +2.5dB */

seq_exit_t state_FEBG_cal(void)
{
    BYTE    opt_FEBG;
    BYTE    orig_FESEL,orig_FEOBG,orig_SPP2BG;
    BYTE    i,ramp_cnt;
    SBYTE   index;
    USHORT  max_FEO,max_SPP2,cur_FEO,cur_SPP2;
    SHORT   FEO_ref[FEBG_CALIB_POINTS],SPP2_ref[FEBG_CALIB_POINTS];
    float   temp_float;

    SendMsg80(DEBUG_OFFLINE_CALIB_MSG,0x338100); //****FEBG_CALIB_START!!!****

    svoRampMeasVar.saturated = FALSE;//in case FEBG is skipped.
    if (ARG1 == OFF)
    {
        // off line calibration, focus off and prepare for ramping.
        Defect_Enable(DEFECT_STATE_ALL_OFF);

        WRITE_REG(HAL_SRVREG_MAP->LDOFF, 0);   /* set LD to OFF (21CH) */
        WRITE_REG(HAL_SRVREG_MAP->LDON, 0);

        //20101217_0300 ==start== Fix issue that it will get wrong FEO Vref during FEBG calibration.
        HAL_SERVO_SetFocDn(HAL_SRV_HISRSET);
        DelaymS(F_CAL_DN_T / 2);
        ramp_stop();
        WaitTimer(MS_100);                          //wait 100 ms for lens to stand still.
        //20101217_0300 ==end== Fix issue that it will get wrong FEO Vref during FEBG calibration.
    }
    else
    {
        if (((svoCalVar.calGainDoneBits & febg_end) == febg_end)
#if (DVD_RAM_READ == 1)
    #if (FE_SLEW_RATE_LIMITER == 2)
            || (DVDRAM)
    #else
            || ((svoVar_discKindT.Bit.DiscStd == eDiscDVD) && (svoVar_discKindT.Bit.Layers == eLayersSingle))
    #endif
#endif
           )
        {
            // If support DVD-RAM disc, skip FEBG calibration.
            // Note: Because of header, S-curve is possible not balanced for DVD-RAM disc.
            SendMsg80(DEBUG_CALIB_ENTRYS_MSG,0x3A0002); // ==> Calib SKIP!!
            return (SEQ_EX0);
        }

#if (FEBG_COLLI_SCAN_RANGE > 0)
        if (Iram_colli_CurrentPosition == calGetOptColliPos(0))
        {
            //hold lens at current height before ExitProcess()
            ramp_stop();
            svoStp_JumpToPosition(COLLI_NORMAL_MODE,calGetOptColliPos(0) - (FEBG_COLLI_SCAN_RANGE / 64 * 64 + 32));
        }
        else if ((Iram_colli_CurrentPosition + 4) > (calGetOptColliPos(0) + FEBG_COLLI_SCAN_RANGE))
        {
            svoStp_JumpToPosition(COLLI_NORMAL_MODE,calGetOptColliPos(0));
        }
        else
        {
            svoStp_JumpToPosition(COLLI_NORMAL_MODE,Iram_colli_CurrentPosition + 64);
        }

        while (svoStp_busy())
        {
            ExitProcess();
        }
        send_msgA2(SHOW_DEBUG_MSG,0x538215,Iram_colli_CurrentPosition);
#endif

#if (FEBG_TILT_SCAN_RANGE > 0)
        if (HAL_SERVO_GetTilt() == TILT_INIT_DAC)
        {
            //hold lens at current height before ExitProcess()
            ramp_stop();
            apply_TILT(TILT_INIT_DAC - (FEBG_TILT_SCAN_RANGE / 4 * 4 + 2));
        }
        else if ((HAL_SERVO_GetTilt() + 4) > (TILT_INIT_DAC + FEBG_TILT_SCAN_RANGE))
        {
            apply_TILT(TILT_INIT_DAC);
        }
        else
        {
            apply_TILT(HAL_SERVO_GetTilt() + 4);
        }
        send_msgA2(SHOW_DEBUG_MSG,0x538211,HAL_SERVO_GetTilt());
#endif
    }

    SetTimeout(0, ARG2);

    orig_FESEL = READ_REG(HAL_AFEREG_MAP->FCTS30); // 93AH bit7 ~bit3 select the FE input
    if ((orig_FESEL & HAL_AFE_FESP2N_MASK) || (orig_FESEL & HAL_AFE_FESP2_MASK))
    {
        WRITE_REG(HAL_AFEREG_MAP->FCTS30, orig_FESEL & (~HAL_AFE_FEFEO_MASK));
    }

    //store original FEBG, restore them when calibration fail
    orig_FEOBG = READ_FIELD(HAL_AFE_FEOBG);
    orig_SPP2BG = READ_FIELD(HAL_AFE_SPP2BG);

    //reduce FEO and SPP2 6dB to prevent saturation when FEBG are not optimal
    i = READ_FIELD(HAL_AFE_FEOPGN);
    if (i > 0)
    {
        WRITE_FIELD(HAL_AFE_FEOPGN, i - 1);
    }

    i = READ_FIELD(HAL_AFE_SPP2PGNA);
    if (i > 0)
    {
        WRITE_FIELD(HAL_AFE_SPP2PGNA, i - 1);
    }

    WRITE_FIELD(HAL_AFE_MUX2ADSEL, halAFE_MUX2ADSEL_FEO);
    WRITE_FIELD(HAL_AFE_MUX1ADSEL, halAFE_MUX1ADSEL_SRFO);

    //20101217_0300 ==start== Fix issue that it will get wrong FEO Vref during FEBG calibration.
    for (index = 0; index < FEBG_CALIB_POINTS; index++)
    {
        WRITE_FIELD(HAL_AFE_FEOBG,(BYTE)FEBGain[index]);
        WRITE_FIELD(HAL_AFE_SPP2BG,(BYTE)FEBGain[index]);
        Delay100uS(8);
        SPP2_ref[index] = 0;
        for (i = 0; i < 32; i++)
        {
            WRITE_REG(HAL_SRVREG_MAP->RGSEL, halSRV_RSEL_FE);       /* read FE data */
            SPP2_ref[index] += READ_REG(HAL_SRVREG_MAP->SRCR);
        }
        SPP2_ref[index] = RoundDivide(SPP2_ref[index], i);
        FEO_ref[index] = (((ReadAverageAdcmV(ADC_MUXOUT2,7) - GPADC_IN_MIN) << GPADC_BITS) + GPADC_IN_RANGE / 2) / GPADC_IN_RANGE;
    }
    svoRampMeasVar.refSRFO = (((ReadAverageAdcmV(ADC_MUXOUT1,7) - GPADC_IN_MIN) << GPADC_BITS) + GPADC_IN_RANGE / 2) / GPADC_IN_RANGE;

    HAL_SERVO_SetFocUp(HAL_SRV_HISRSET);
    DelaymS((F_CAL_DN_T - 2 * F_SEARCH_T) / 2);
    ramp_stop();
    //20101217_0300 ==end== Fix issue that it will get wrong FEO Vref during FEBG calibration.

    svoDiagStatus = 0x00;
    ramp_cnt = 0;
    max_FEO = max_SPP2 = 0;
    for (index = 0; index < (FEBG_CALIB_POINTS * FEBG_SAMPLE_POINTS); index++)
    {
        if ((timeout_chk(0) == eTimerTimeout) || (svoRampMeasVar.saturated == TRUE))
        {
            svoDiagStatus = 0x80;
            break;
        }

        ramp_cnt++;
        if ((index % FEBG_SAMPLE_POINTS) == 0)
        {
            i = index / FEBG_SAMPLE_POINTS;

            WRITE_FIELD(HAL_AFE_FEOBG,(BYTE)FEBGain[i]);
            WRITE_FIELD(HAL_AFE_SPP2BG,(BYTE)FEBGain[i]);

            svoCurvefitCalVar.cost[i] = 0;
            svoCurvefitCalVar.item[i] = 0;
            // CALIB_Index = %d, SET_FEBGAIN ==> %4.2f (dB)
            SendMsgCnB(DEBUG_OFFLINE_CALIB_MSG,5,0x338102,B1B(i),F4B((float)(((FEBGain[i])*0.5f)-8)));

            svoRampMeasVar.refFE = SPP2_ref[i];
            svoRampMeasVar.refFEO = FEO_ref[i];
            SendMsgCnB(DEBUG_OFFLINE_CALIB_MSG,7,0x338119,B1B((BYTE)FEBGain[i]),B1B(READ_FIELD(HAL_AFE_FEOBG)),B1B(READ_FIELD(HAL_AFE_SPP2BG))
                                                   ,B2B(svoRampMeasVar.refFE),B2B(svoRampMeasVar.refFEO));
        }
#if (FOOFFSET2_ENABLE == 1)
        WRITE_REG(HAL_SRVREG_MAP->LDOFF,0x00);
        WRITE_REG(HAL_SRVREG_MAP->LDON, 0x00);
        WRITE_DSP_COEF(kf18,FOCUS_SEARCH_KF18);
#endif
        if (ramp_cnt & 0x01)
        {
            HAL_SERVO_SetFocUp(0);
        }
        else
        {
            HAL_SERVO_SetFocDn(0);
        }

        RampMeas(eRampFEBG, F_SEARCH_T*4, (ramp_cnt & 0x01)? UP : DOWN, svoVar_discKindT);
        if (ARG1 == ON)
        {
            svoPrvtVar_avgSRFO[0] += svoRampMeasVar.SRFOpp[0];
            svoPrvtVar_avgFOO += svoRampMeasVar.LayerFoo;
            svoPrvtVar_focusP2PCount++;
        }

        //hold lens at current height before ExitProcess()
        ramp_stop();
        i = index / FEBG_SAMPLE_POINTS;

        SendMsg80(DEBUG_OFFLINE_CALIB_MSG,0x338104-(ramp_cnt & 0x01)); //ramp up/down
#if (DVD_RAM_READ == 1)
        if(!((svoVar_discKindT.Bit.DiscStd == eDiscDVD) && (svoVar_discKindT.Bit.Layers == eLayersSingle)))
#endif
        {
            //check FEOpp,SPP2pp to prevent bad sample due to defect
            temp_float = CalculateRateFromXdB(P01dB, (0x10-(BYTE)FEBGain[i]) * (50/2)) * 40;
            cur_FEO = (svoRampMeasVar.FEpp[0] + svoRampMeasVar.FEpp[1]) * temp_float;
            cur_SPP2 = (svoRampMeasVar.BSpp[0] + svoRampMeasVar.BSpp[1]) * temp_float;
            if ((max_FEO + max_SPP2) != 0)
            {
                if ((cur_FEO * 3 < max_FEO * 2) || (cur_SPP2 * 3 < max_SPP2 * 2))
                {
                    index--;
                    SendMsg80(SHOW_DEBUG_MSG,0x338110);//Bad FEBG sample detected
                    ExitProcess();
                    continue;
                }
                if ((max_FEO * 3 < cur_FEO * 2) || (max_SPP2 * 3 < cur_SPP2 * 2))
                {
                    max_FEO = max_SPP2 = 0;
                    index = -1;
                    SendMsg80(SHOW_DEBUG_MSG,0x338110);//Bad FEBG sample detected
                    ExitProcess();
                    continue;
                }
            }
            if (cur_FEO > max_FEO) max_FEO = cur_FEO;
            if (cur_SPP2 > max_SPP2) max_SPP2 = cur_SPP2;
        }
        //store measure result
        temp_float = (((float)(svoRampMeasVar.FEpp[0] - svoRampMeasVar.FEpp[1]) * 100) / (svoRampMeasVar.FEpp[0] + svoRampMeasVar.FEpp[1]));
        SendMsgCnB(DEBUG_OFFLINE_CALIB_MSG,4,0x338105,F4B(temp_float)); // FEO-balance ==> %4.2f (%)
        svoCurvefitCalVar.cost[i] += temp_float;
        temp_float = (((float)(svoRampMeasVar.BSpp[0] - svoRampMeasVar.BSpp[1]) * 100) / (svoRampMeasVar.BSpp[0] + svoRampMeasVar.BSpp[1]));
        SendMsgCnB(DEBUG_OFFLINE_CALIB_MSG,4,0x338106,F4B(temp_float)); // SPP2-balance ==> %4.2f (%)
        svoCurvefitCalVar.item[i] += temp_float;

        if ((index % FEBG_SAMPLE_POINTS) == FEBG_SAMPLE_POINTS - 1)
        {
            svoCurvefitCalVar.cost[i] *= (1.0 / FEBG_SAMPLE_POINTS);    // unit = %
            svoCurvefitCalVar.item[i] *= (1.0 / FEBG_SAMPLE_POINTS);    // unit = %
        }
        ExitProcess();
    }
    SetTimeout(0, TIMER_STOP);
    LAL_WriteCoef(AFE_MB18_FEOPGN);
    LAL_WriteCoef(AFE_SB05_SPP2PGNA);
    WRITE_REG(HAL_AFEREG_MAP->FCTS30,orig_FESEL);

    if (svoDiagStatus == 0x00)
    {
        //FEOBG
        LineFit(FEBG_CALIB_POINTS, FEBGain, svoCurvefitCalVar.cost, svoCurvefitCalVar.cof);

#if 1 /* for debug */
        for (i = 0; i < FEBG_CALIB_POINTS; i++)
        {
            SendMsgCnB(SHOW_DEBUG_MSG,4,0x338101,F4B((FEBGain[i] - 16) / 2));                  // SET_FEBG_GAIN ==> %4.0f (dB)
        }
        for (i = 0; i < FEBG_CALIB_POINTS; i++)
        {
           SendMsgCnB(SHOW_DEBUG_MSG,4,0x338105,F4B(svoCurvefitCalVar.cost[i]));    // FEO-balance ==> %4.2f (%)
        }
        for (i = 0; i < 2; i++)
        {
            SendMsgCnB(SHOW_DEBUG_MSG,4,0x338201+i,F4B(svoCurvefitCalVar.cof[i]));  // Y=A*X^2+B*X+C; A = %4.2f
        }
        // OPTIMUM_FEOBG ==> %4.2f
        SendMsgCnB(SHOW_DEBUG_MSG,4,0x33FF4C,F4B(-svoCurvefitCalVar.cof[0] / svoCurvefitCalVar.cof[1]));
#endif // 0
        opt_FEBG = (SBYTE)(-svoCurvefitCalVar.cof[0] / svoCurvefitCalVar.cof[1] + 0.5);
        opt_FEBG = (opt_FEBG > FEBGain[FEBG_CALIB_POINTS-1])? FEBGain[FEBG_CALIB_POINTS-1] : opt_FEBG;
        opt_FEBG = (opt_FEBG < FEBGain[0])? FEBGain[0] : opt_FEBG;
        calSetAfeGain(Layer_Index,FEOB_GN,TYPE3,0,0,opt_FEBG);
        // OPTIMUM_FEOBG  ==> 0x%02Xh
        SendMsgCn(DEBUG_OFFLINE_CALIB_MSG|DEBUG_CALIB_RESULT_MSG,1,0x338107,opt_FEBG);
        svoDiagReturnValue1 = opt_FEBG;
        svoDiagReturnValue2 = (SHORT)((float)opt_FEBG * svoCurvefitCalVar.cof[1] + svoCurvefitCalVar.cof[0]);//unit = 1/1024
        // Min_FEO_DIFFVALUE ==> %2d (%)
        SendMsgCn(DEBUG_OFFLINE_CALIB_MSG,2,0x338109,A2B(svoDiagReturnValue2));

        //SPP2BG
        if ((orig_FESEL & HAL_AFE_FESP2N_MASK) || (orig_FESEL & HAL_AFE_FESP2_MASK))
        {
            LineFit(FEBG_CALIB_POINTS, FEBGain, svoCurvefitCalVar.item, svoCurvefitCalVar.cof);

#if 1 /* for debug */
            for (i = 0; i < FEBG_CALIB_POINTS; i++)
            {
                SendMsgCnB(SHOW_DEBUG_MSG,4,0x338101,F4B((FEBGain[i] - 16) / 2));                        // SET_FEBG_GAIN ==> %4.0f (dB)
            }
            for (i = 0; i < FEBG_CALIB_POINTS; i++)
            {
                SendMsgCnB(SHOW_DEBUG_MSG,4,0x338106,F4B(svoCurvefitCalVar.item[i]));   // SPP2-balance ==> %4.2f (%)
            }
            for (i = 0; i < 2; i++)
            {
                SendMsgCnB(SHOW_DEBUG_MSG,4,0x338201+i,F4B(svoCurvefitCalVar.cof[i]));  // Y=A*X^2+B*X+C; C = %4.2f
            }
            // OPTIMUM_SPP2BG ==> %4.2f
            SendMsgCnB(SHOW_DEBUG_MSG,4,0x338118,F4B(-svoCurvefitCalVar.cof[0] / svoCurvefitCalVar.cof[1]));
#endif // #if 0

            opt_FEBG = (SBYTE)(-svoCurvefitCalVar.cof[0] / svoCurvefitCalVar.cof[1] + 0.5);
            opt_FEBG = (opt_FEBG > FEBGain[FEBG_CALIB_POINTS-1])? FEBGain[FEBG_CALIB_POINTS-1] : opt_FEBG;
            opt_FEBG = (opt_FEBG < FEBGain[0])? FEBGain[0] : opt_FEBG;
            calSetAfeGain(Layer_Index,SPP2B_GN,TYPE3,0,0,opt_FEBG);
            // OPTIMUM_SPP2BG ==> 0x%02Xh
            SendMsgCn(DEBUG_OFFLINE_CALIB_MSG|DEBUG_CALIB_RESULT_MSG,1,0x338108,opt_FEBG);

#if (EN_Persistent_table_8 == 1)
            svoDiagReturnValue3 = opt_FEBG;
            svoDiagReturnValue4 = (SHORT)((float)opt_FEBG * svoCurvefitCalVar.cof[1] + svoCurvefitCalVar.cof[0]);//unit = 1/1024
            // Min_SPP2O_DIFFVALUE ==> %2d (%)
            SendMsgCn(DEBUG_OFFLINE_CALIB_MSG,2,0x33810A,A2B(svoDiagReturnValue4));
#else
            svoDiagReturnValue1 |= (opt_FEBG << 8);
#endif
        }

        if (svoVar_discKindT.Bit.DiscStd == eDiscDVD)
        {
#if (EN_Persistent_table_8 == 1)
            svoPcalVar.dvFeoBG0Value = (SHORT)svoDiagReturnValue1;
            svoPcalVar.dvSPP2BG0Value = (SHORT)svoDiagReturnValue3;
#else
            svoPcalVar.dvFeBG0Value = (SHORT)svoDiagReturnValue1;
#endif
        }
        else
        {
#if (EN_Persistent_table_8 == 1)
            svoPcalVar.cdFeoBGValue = (SHORT)svoDiagReturnValue1;
            svoPcalVar.cdSPP2BGValue = (SHORT)svoDiagReturnValue3;
#else
            svoPcalVar.cdFeBGValue = (SHORT)svoDiagReturnValue1;
#endif
        }
    }
    else
    {
        WRITE_FIELD(HAL_AFE_FEOBG,orig_FEOBG);
        WRITE_FIELD(HAL_AFE_SPP2BG,orig_SPP2BG);
    }

    if (ramp_cnt & 0x01)
    {
        HAL_SERVO_SetFocDn(0);
        i = svoRampMeasVar.saturated;
        RampMeas(eRampFEBG, F_SEARCH_T*2, DOWN, svoVar_discKindT);
        svoRampMeasVar.saturated = i;

        if (ARG1 == ON)
        {
            svoPrvtVar_avgSRFO[0] += svoRampMeasVar.SRFOpp[0];
            svoPrvtVar_avgFOO += svoRampMeasVar.LayerFoo;
            svoPrvtVar_focusP2PCount++;
        }
    }

    if (ARG1 == OFF)
    {// off line calibration, try to focus on before return.
        HAL_SERVO_SetFebc(0);    //not in focus close loop, set FEBC to 0 directly.
        HAL_SERVO_SetFocUp(HAL_SRV_FSONSET);    /* focus lens up, (88H) */
        DelaymS(F_SEARCH_T*2);

        calUpdateRadius(Iram_stp_CurrentPosition, TRUE);    //update FEBC
        Defect_Enable(DEFECT_STATE_ALL_ON|DEFECT_SOURCE_DFE_RF);
        if (!FokStatusOK())
        {
            //Hold FOO output at layer position
            WRITE_REG(HAL_SRVREG_MAP->LDOFF, 0);
            WRITE_REG(HAL_SRVREG_MAP->LDON, 0);
        }
    }
#if (FEBG_COLLI_SCAN_RANGE > 0)
    else if (Iram_colli_CurrentPosition != calGetOptColliPos(0))
    {
        return (SEQ_EX2);
    }
#endif
#if (FEBG_TILT_SCAN_RANGE > 0)
    else if (HAL_SERVO_GetTilt() != TILT_INIT_DAC)
    {
        return (SEQ_EX2);
    }
#endif

    if (svoDiagStatus == 0x00)
    {
        if (ARG1 == ON)
        {
            svoCalVar.calGainDoneBits |= febg_end;
        }
        return(SEQ_EX0);
    }
    else
    {
        SendMsg80(SHOW_DEBUG_MSG,0x33810B); //WARNING: FEBG calib fail
#if (Customer_A1 ==1)
				PositionHomeErrorCount++;	
				CompleteStroke = TRUE;
#endif
        
        return(SEQ_EX1);
    }
}

#if (DVDDL_TILT_CAL_PRO == 1)
    BYTE     TILT_retries;
#endif
/*-----------------------------------------------------------------------------
 *
 * Module: state_FEBC_TILT_cal()
 * Description: Calibration using curve fitting, retry automatically until 3 retries or timeout
 *              ARG1:FEBC_DSP, TILT_DAT, COLLI_POS, TE_SETPT
 *              ARG2:JITTER_CAL, TE_P2P_CAL, RF_P2P_CAL
 *              ARG3:Timeout
 *----------------------------------------------------------------------------*/
seq_exit_t state_FEBC_TILT_cal(void)
{
    float    temp_float,temp_float2;
    BYTE     sampling,retries,orig_TELpfBw;
    ad_dsp_t meas_signal;
#if (DVD_RAM_READ == 1)
    BYTE     track_type;  // 0: groove tracks; 1: land tracks; 2: all tracks
#endif

    SendMsg80(DEBUG_AUTO_TUNING_MSG,0x338200); //****CALIB_START!!!****
#if (NEW_FDP_DETECTION != 0)
    WRITE_FIELD(HAL_SRV_FNOKEN,0);  //turn off Focus drop interrupt
    #if (NEW_FDP_DETECTION == 2)
    WRITE_FIELD(HAL_SRV_DSPI6EN,0); //turn off New Focus drop interrupt
    #endif
#endif

    SetTimeout(0, ARG3);
#if ((BD_2X_STARTUP == 1) && (BD_UNBALANCE_DISC_CHECK))
    if((svoVar_unbalanceDiscNum != 0)&&(svoVar_discKindT.Bit.DiscStd == eDiscBD)
       &&(svoIramVar.svoCurrentSpeed == _2LX_BD))
       spdHoldRPM(svoSpeedVar.SpdInRange? svoSpeedVar.TargetRpm : svoSpeedVar.CurrentRpm);
#endif

    switch (ARG1)
    {
        case FEBC_DSP:
#if (DVD_RAM_READ == 1) && (DVDRAM_FEBC_ON_FCSO == 1)
            if(DVDRAM)
            {
                svoCurvefitCalVar.item_max  = FCSO_MAX_VAL;
                svoCurvefitCalVar.item_min  = FCSO_MIN_VAL;
                svoCurvefitCalVar.item_step = FCSO_STEP_VAL;
                svoCurvefitCalVar.orig_item = calDacCount2SBYTE(svoCalOfsT1Var[0][FE_OS].groove_offset[RECDH],8);
            }
            else
#endif
            {
                svoCurvefitCalVar.item_max  = FEBC_MAX_DAC;
                svoCurvefitCalVar.item_min  = FEBC_MIN_DAC;
                switch (svoVar_discKindT.Bit.DiscStd)
                {
                    case eDiscCD:
                        svoCurvefitCalVar.item_step = FEBC_STEP_DAC_CD;
                        break;
#if (ENABLE_HDDVD == 1)
                    case eDiscHD:
                        svoCurvefitCalVar.item_step = FEBC_STEP_DAC_HD;
                        break;
#endif
#if (BD_ENABLE == 1)
                    case eDiscBD:
                        svoCurvefitCalVar.item_step = FEBC_STEP_DAC_BD;
                        break;
#endif
                    default:
#if (DVD_RAM_READ == 1)
                        if (DVDRAM)
                            svoCurvefitCalVar.item_step = FEBC_STEP_DAC_RAM;
                        else
#endif
                            svoCurvefitCalVar.item_step = FEBC_STEP_DAC;
                        break;
                }
                svoCurvefitCalVar.orig_item = HAL_SERVO_GetFebc();
            }
            break;

        case TILT_DAT:
            svoCurvefitCalVar.item_max  = TILT_MAX_DAC;
            svoCurvefitCalVar.item_min  = TILT_MIN_DAC;
            switch (svoVar_discKindT.Bit.DiscStd)
            {
                case eDiscCD:
                    svoCurvefitCalVar.item_step = TILT_STEP_DAC_CD;
                    break;
#if (ENABLE_HDDVD == 1)
                case eDiscHD:
                    svoCurvefitCalVar.item_step = TILT_STEP_DAC_HD;
                    break;
#endif
#if (BD_ENABLE == 1)
                case eDiscBD:
                    svoCurvefitCalVar.item_step = TILT_STEP_DAC_BD;
                    break;
#endif
                default:
#if (DVD_RAM_READ == 1)
                    if (DVDRAM)
                        svoCurvefitCalVar.item_step = TILT_STEP_DAC_RAM;
                    else
#endif
                        svoCurvefitCalVar.item_step = TILT_STEP_DAC;
                    break;
            }
            svoCurvefitCalVar.orig_item = HAL_SERVO_GetTilt();
            break;

#if (COLLI_SWITCH == 1)
        case COLLI_POS:
            svoCurvefitCalVar.item_max  = COLLI_MAX_POS;
            svoCurvefitCalVar.item_min  = COLLI_MIN_POS;
            svoCurvefitCalVar.item_step = COLLI_STEP_POS;
            svoCurvefitCalVar.orig_item = Iram_colli_CurrentPosition;
            break;
#endif

        case TE_SETPT:
            svoCurvefitCalVar.item_max  = TESPT_MAX_VAL;
            svoCurvefitCalVar.item_min  = TESPT_MIN_VAL;
            svoCurvefitCalVar.item_step = TESPT_STEP_VAL;
            svoCurvefitCalVar.orig_item = svoCalMisc1Var[Layer_Index].teSetPoint;
            break;
    }

    switch (ARG2)
    {
#if (USE_REALTIME_JITTER == 1)
        case JITTER_CAL:
            // still to check whether it should not better be done in the retry
            //SetDfeWblFrontSemiSynchr(); // is already performed in state_trksvo_on
            SetTimeout(1, MS_20);
            while (!HAL_DFE_GetPllInLock())
            {
                if (timeout_chk(1) == eTimerTimeout)
                {
                    SendMsg80(SHOW_DEBUG_MSG,0x320A12);
                    break;
                }
                ExitProcess();
            }
            SetTimeout(1,TIMER_STOP);
            SetDfeWblFrontBitSynchr();

            svoCurvefitCalVar.up_curve   = TRUE;
            svoCurvefitCalVar.worst_cost = 260; //jitter over 26% are not realiable anymore
            svoCurvefitCalVar.delta_cost = FEBC_TILT_JITTER_DELTA_COST;
    #if (DVD_RAM_READ == 1)
            track_type = 0; // groove tracks first
    #endif
            break;
#endif

        case TE_P2P_CAL:
            svoCurvefitCalVar.up_curve   = FALSE;
            svoCurvefitCalVar.worst_cost = 0;
            svoCurvefitCalVar.delta_cost = FEBC_TILT_TE_P2P_DELTA_COST;
            //set same gain to prevent TEpp change due to unstable RECD transition
            WRITE_FIELD(HAL_AFE_TRKG,svoCalGnT1Var[Layer_Index][TE_GN].groove_gain[RECDL]);
            WRITE_FIELD(HAL_AFE_TRKO,svoCalOfsT1Var[Layer_Index][TE_OS].groove_offset[RECDL]);
            orig_TELpfBw = HAL_AFE_GetTELpfBW();
            HAL_AFE_SetTELpfBW(e_TELPF_300);
#if (DVD_RAM_READ == 1)
            track_type = 2; // all tracks
#endif
            break;

        case RF_P2P_CAL:
            svoCurvefitCalVar.up_curve   = FALSE;
            svoCurvefitCalVar.worst_cost = 0;
            svoCurvefitCalVar.delta_cost = FEBC_TILT_RF_P2P_DELTA_COST;
#if (DVD_RAM_READ == 1)
            track_type = 2; // all tracks
#endif
            break;
    }

    for (retries = 0; retries < 3; retries++)
    {
        svoCurvefitCalVar.calDoneBits = svoCalVar.calDoneBits;
        svoCalVar.calDoneBits      &= ~(tilt_cal_end | febc_cal_end);
        sampling                    = TRUE;
        svoCurvefitCalVar.cal_index = 0;
        svoCurvefitCalVar.opt_cost  = svoCurvefitCalVar.worst_cost;
        svoCurvefitCalVar.item[0]   = svoCurvefitCalVar.orig_item;
        svoCurvefitCalVar.ramp_mode = RAMPING_UP;

        while (sampling)
        {
            switch (ARG1)
            {
                case FEBC_DSP:
#if (DVD_RAM_READ == 1) && (DVDRAM_FEBC_ON_FCSO == 1)
                    if(DVDRAM)
                    {
                        BYTE fcso;

                        fcso = calSBYTE2DacCount(svoCurvefitCalVar.item[svoCurvefitCalVar.cal_index], 8);
                        if ((track_type == 0)||(track_type == 2))
                        {
                            WRITE_FIELD(HAL_AFE_FCSO,fcso);
                            WRITE_FIELD(HAL_AFE_BLANKFCSO,fcso);
                        }
                        if ((track_type == 1)||(track_type == 2))
                        {
                            WRITE_FIELD(HAL_AFE_LNDFCSO,fcso);
                            WRITE_FIELD(HAL_AFE_BLNDFCSO,fcso);
                        }
                    }
                    else
#endif
                    {
                        apply_FEBC((SHORT)svoCurvefitCalVar.item[svoCurvefitCalVar.cal_index]);
                    }
                    break;

                case TILT_DAT:
                    apply_TILT((SHORT)svoCurvefitCalVar.item[svoCurvefitCalVar.cal_index]);
                    break;

#if (COLLI_SWITCH == 1)
                case COLLI_POS:
                    svoStp_JumpToPosition(COLLI_NORMAL_NO_REST_MODE,svoCurvefitCalVar.item[svoCurvefitCalVar.cal_index]);
    #if (OPU == BDPL2)
                    HAL_LASC_SetAPCTarget(svoCurvefitCalVar.item[svoCurvefitCalVar.cal_index]);
    #endif
                    while (svoStp_busy())
                    {
                        ExitProcess();
                    }
                    break;
#endif

                case TE_SETPT:
                    apply_TESPT((SHORT)svoCurvefitCalVar.item[svoCurvefitCalVar.cal_index]);
                    break;
            }
            SendMsgCnB(DEBUG_AUTO_TUNING_MSG,4,0x338210+ARG1,F4B(svoCurvefitCalVar.item[svoCurvefitCalVar.cal_index]));

            WaitTimer(MS_5);

            switch (ARG2)
            {
                case JITTER_CAL:
#if (USE_REALTIME_JITTER == 1)
                    meas_signal = ReadAvgJitterPerRevs(FOUR_REVOLUTIONS);

                    if (meas_signal.errcode == eNOERR)
                    {
    #if (DVD_RAM_READ == 1)
                        if (DVDRAM)
                        {
                            temp_float = (track_type == LAND_TRACK)? (float)meas_signal.p2p_mv : (float)meas_signal.middle_mv;
                        }
                        else
    #endif
                        {
                            temp_float = (float)meas_signal.middle_mv;
                        }

                        SendMsgCnB(DEBUG_AUTO_TUNING_MSG,4,0x338220+ARG2,F4B(temp_float));
                    }
                    else
                    {
                        SendMsg80(SHOW_DEBUG_MSG,0x338313);
                        svoCurvefitCalVar.ramp_mode = RAMPING_END;
                    }
                    break;
#endif

                case TE_P2P_CAL:
#if (DVD_RAM_READ == 1)
                    if (DVDRAM_rewritable)
                    {
                        meas_signal = ReadGPADCmV_Avg(ONE_REVOLUTION, ADC_TE, TRUE);  // Headers cause noise on TE in DVDRAM rewritable area.
                    }
                    else
#endif
                    {
                        meas_signal = ReadFilteredDSPADCmV(ONE_REVOLUTION, TE_ALL, ADC_TE, TRUE);
                    }

                    if (meas_signal.errcode == eNOERR)
                    {
                        temp_float = (float)meas_signal.p2p_mv;
                        SendMsgCnB(DEBUG_AUTO_TUNING_MSG,4,0x338220+ARG2,F4B(temp_float));
                    }
                    else
                    {
                        SendMsgCn(SHOW_DEBUG_MSG,1,0x338312,meas_signal.errcode);
                        svoCurvefitCalVar.ramp_mode = RAMPING_END;
                    }
                    break;

                case RF_P2P_CAL:
                    meas_signal = readRFppFromAGC_1rev(TRUE);

                    if ((meas_signal.errcode != eNOERR) || (meas_signal.p2p_mv < FEBC_TILT_RF_P2P_MIN))
                    {
                        SendMsgCn(SHOW_DEBUG_MSG,1,0x338311,meas_signal.errcode);
										#if (Customer_A1 ==1)
												PositionHomeErrorCount++;	
										#endif

                    #if (DVDDL_TILT_CAL_PRO == 1)
                        if ((ARG1 == TILT_DAT) && (meas_signal.errcode ==  eLIMIT_blank_calibrate_TILT)
                            && (svoVar_discKindT.Bit.DiscStd == eDiscDVD) && (svoVar_discKindT.Bit.Layers != eMediaROM))
                        {
                            SpeedCount = SPEED_DOWN_FOR_SEEK_FAIL;    // speed down get more stable
                            retries = 2;
                            TILT_retries++ ;
                            // if RF modulation too small not retry
                        }
                    #endif

                        svoCurvefitCalVar.ramp_mode = RAMPING_END;
                    }
                    else
                    {
                        temp_float = (float)meas_signal.p2p_mv;
                        SendMsgCnB(DEBUG_AUTO_TUNING_MSG,4,0x338220+ARG2,F4B(temp_float));
                    }
                    break;
            }

            if (svoCurvefitCalVar.ramp_mode != RAMPING_END)
            {
                svoCurvefitCalVar.cost[svoCurvefitCalVar.cal_index] = temp_float;
                if (svoCurvefitCalVar.up_curve)
                    svoCurvefitCalVar.opt_cost = MIN(svoCurvefitCalVar.opt_cost,temp_float);
                else
                    svoCurvefitCalVar.opt_cost = MAX(svoCurvefitCalVar.opt_cost,temp_float);

                svoCurvefitCalVar.cal_index++;
                if (svoCurvefitCalVar.cal_index == 1)
                {
                    if (svoCurvefitCalVar.item[0] < (svoCurvefitCalVar.item_max + svoCurvefitCalVar.item_min) / 2)
                        svoCurvefitCalVar.ramp_mode = RAMPING_UP;
                    else
                        svoCurvefitCalVar.ramp_mode = RAMPING_DOWN;
                }
                else
                {
                    svoCurvefitCalVar.ramp_mode = calib_data_analyser(svoCurvefitCalVar.item,
                                                                      svoCurvefitCalVar.cost,
                                                                      svoCurvefitCalVar.cal_index,
                                                                      FEBC_TILT_MIN_SAMPLES,
                                                                      svoCurvefitCalVar.item_min,
                                                                      svoCurvefitCalVar.item_max,
                                                                      (svoCurvefitCalVar.opt_cost * svoCurvefitCalVar.delta_cost),
                                                                      svoCurvefitCalVar.up_curve);
                }

                if (svoCurvefitCalVar.cal_index >= FEBC_TILT_MAX_SAMPLES)
                {
                    SendMsg80(SHOW_DEBUG_MSG,0x338240);
                    svoCurvefitCalVar.ramp_mode = RAMPING_END;
                }

                if (timeout_chk(0) == eTimerTimeout)
                {
                    SendMsg80(SHOW_DEBUG_MSG,0x338241);
                    svoCurvefitCalVar.ramp_mode = RAMPING_END;
                    retries = 2;
                }
            }

            switch (svoCurvefitCalVar.ramp_mode)
            {
                case RAMPING_UP:
                    temp_float = svoCurvefitCalVar.item[svoCurvefitCalVar.cal_index - 1] + svoCurvefitCalVar.item_step;
                    if (temp_float > svoCurvefitCalVar.item_max)
                        temp_float = svoCurvefitCalVar.item_max;
                    svoCurvefitCalVar.item[svoCurvefitCalVar.cal_index] = temp_float;
                    break;

                case RAMPING_DOWN:
                    temp_float = svoCurvefitCalVar.item[0] - svoCurvefitCalVar.item_step;
                    if (temp_float < svoCurvefitCalVar.item_min)
                        temp_float = svoCurvefitCalVar.item_min;
                    svoCurvefitCalVar.item[svoCurvefitCalVar.cal_index] = temp_float;
                    break;

                case RAMPING_RETRY:
                case RAMPING_FAIL:
                case RAMPING_END:
                default:
                    sampling = FALSE;
                    break;
            }
        }

        svoCalVar.calDoneBits = svoCurvefitCalVar.calDoneBits;
        if (svoCurvefitCalVar.up_curve)
        {
            temp_float = MIN(svoCurvefitCalVar.cost[svoCurvefitCalVar.cal_index-1],svoCurvefitCalVar.cost[0]);
            temp_float2 = (float)svoCurvefitCalVar.opt_cost * (1 + svoCurvefitCalVar.delta_cost);
            if (temp_float < temp_float2)
            {
                temp_float = temp_float2;
            }
            if (temp_float > svoCurvefitCalVar.worst_cost)
            {
                temp_float = svoCurvefitCalVar.worst_cost;
            }
        }
        else
        {
            temp_float = MAX(svoCurvefitCalVar.cost[svoCurvefitCalVar.cal_index-1],svoCurvefitCalVar.cost[0]);
            temp_float2 = (float)svoCurvefitCalVar.opt_cost * (1 - svoCurvefitCalVar.delta_cost);
            if (temp_float > temp_float2)
            {
                temp_float = temp_float2;
            }
            if (temp_float < svoCurvefitCalVar.worst_cost)
            {
                temp_float = svoCurvefitCalVar.worst_cost;
            }
        }

        curve_fit_data_filter(svoCurvefitCalVar.item, svoCurvefitCalVar.cost, &svoCurvefitCalVar.cal_index, temp_float, svoCurvefitCalVar.up_curve);
        svoDiagStatus = 0x80;
        svoDiagReturnValue1 = svoCurvefitCalVar.orig_item;
        if (svoCurvefitCalVar.cal_index >= (FEBC_TILT_MIN_SAMPLES-2))
        {
            CurveFit(svoCurvefitCalVar.cal_index, svoCurvefitCalVar.item, svoCurvefitCalVar.cost, svoCurvefitCalVar.cof);
            temp_float2 = svoCurvefitCalVar.cof[2] * svoCurvefitCalVar.item_step * svoCurvefitCalVar.item_step /
                          (svoCurvefitCalVar.opt_cost * svoCurvefitCalVar.delta_cost);
            temp_float2 = (svoCurvefitCalVar.up_curve? temp_float2 : -temp_float2);
            if (temp_float2 >= 0.007)
            {
                temp_float = float2short(svoCurvefitCalVar.cof[1] / (-2 * svoCurvefitCalVar.cof[2]));
                if (temp_float > svoCurvefitCalVar.item[svoCurvefitCalVar.cal_index - 1])
                    temp_float = svoCurvefitCalVar.item[svoCurvefitCalVar.cal_index - 1];
                if (temp_float < svoCurvefitCalVar.item[0])
                    temp_float = svoCurvefitCalVar.item[0];

                svoDiagReturnValue1 = float2short(temp_float);
                svoDiagReturnValue2 = (SHORT)(svoCurvefitCalVar.cof[2] * temp_float * temp_float + svoCurvefitCalVar.cof[1] * temp_float + svoCurvefitCalVar.cof[0] + 0.5);
                svoDiagStatus = 0x00;
            }
#if 1 /* for debug */
            {
                BYTE i;

                SendMsg80(SHOW_DEBUG_MSG,0x320A11);
                for (i = 0; i < svoCurvefitCalVar.cal_index; i++)
                {
                    SendMsgCnB(SHOW_DEBUG_MSG,4,0x338210+ARG1,F4B(svoCurvefitCalVar.item[i]));
                }
                for (i = 0; i < svoCurvefitCalVar.cal_index; i++)
                {
                    SendMsgCnB(SHOW_DEBUG_MSG,4,0x338220+ARG2,F4B(svoCurvefitCalVar.cost[i]));
                }
                for (i = 0; i < 3; i++)
                {
                    SendMsgCnB(SHOW_DEBUG_MSG,4,0x338201+i,F4B(svoCurvefitCalVar.cof[i]));
                }
                SendMsgCnB(SHOW_DEBUG_MSG,4,0x338250+ARG1,F4B(svoCurvefitCalVar.cof[1] / (-2 * svoCurvefitCalVar.cof[2])));
                temp_float2 = temp_float2 >= 0? temp_float2 : -temp_float2;
                SendMsgCnB(SHOW_DEBUG_MSG,4,0x338204,F4B(temp_float2));
                temp_float = fsqrt((0.1 * (float)(svoCurvefitCalVar.item_step * svoCurvefitCalVar.item_step) / temp_float2));
                SendMsgCnB(SHOW_DEBUG_MSG,4,0x338205,F4B(temp_float));
                SendMsg80(SHOW_DEBUG_MSG,0x320A10);
            }
#endif // 1
        }

        switch (ARG1)
        {
            case FEBC_DSP:
#if (DVD_RAM_READ == 1) && (DVDRAM_FEBC_ON_FCSO == 1)
                if(DVDRAM)
                {
                    if (svoDiagStatus == 0x00)
                    {
                        SendMsgCn(DEBUG_AUTO_TUNING_MSG,2,0x338230+ARG1,A2B(svoDiagReturnValue1));
                        if(track_type == 0)
                        {
                            calSetAfeOffset(0,FE_OS,TYPE1,GROOVE_OFFSET,PATH_SELECT_ALL,calSBYTE2DacCount(svoDiagReturnValue1,8));
                            svoDiagStatus = 0x80;
                            track_type = 1;
                            retries = 0;
                        }
                        else if(track_type == 1)
                        {
                            calSetAfeOffset(0,FE_OS,TYPE1,LAND_OFFSET,PATH_SELECT_ALL,calSBYTE2DacCount(svoDiagReturnValue1,8));
                        }
                        else if(track_type == 2)
                        {
                            calSetAfeOffset(0,FE_OS,TYPE1,PATH_SELECT_ALL,PATH_SELECT_ALL,calSBYTE2DacCount(svoDiagReturnValue1,8));
                        }
                    }
                }
                else
#endif
                {
                    if (svoDiagReturnValue1 > FEBC_MAX_OPT_DAC)
                        svoDiagReturnValue1 = FEBC_MAX_OPT_DAC;
                    if (svoDiagReturnValue1 < FEBC_MIN_OPT_DAC)
                        svoDiagReturnValue1 = FEBC_MIN_OPT_DAC;

#if (CALIBRATION_MANAGEMENT == 1)
                    svoDiagReturnValue1 = check_signal_Limit(ARG1, svoDiagReturnValue1);
#endif
                    apply_FEBC(svoDiagReturnValue1);
                    if (svoDiagStatus == 0x00)
                    {
                        SendMsgCn(DEBUG_AUTO_TUNING_MSG,2,0x338230+ARG1,A2B(svoDiagReturnValue1));
                        calSetCalResult(Layer_Index,Iram_stp_CurrentPosition,ARG1,svoDiagReturnValue1,CAL_R_DONE);
                    }
                    else
                    {
                        //mark CAL_SKIP so the calibration will not redo for a while.
                        calSetCalResult(Layer_Index,Iram_stp_CurrentPosition,ARG1,0,CAL_SKIP);
                    }
                }
                break;

            case TILT_DAT:
#if (DVDDL_TILT_CAL_PRO == 1)
                if ((meas_signal.errcode ==  eLIMIT_blank_calibrate_TILT) && ((svoIramVar.svoInState & TRN_MASK) == CCALIB)
                    && (svoVar_discKindT.Bit.DiscStd == eDiscDVD) && (svoVar_discKindT.Bit.Layers != eMediaROM))
                {
                    break;
                    //RF modulation too small not updata TILT DAT
                }
#endif

#if (CALIBRATION_MANAGEMENT == 1)
                svoDiagReturnValue1 = check_signal_Limit(ARG1, svoDiagReturnValue1);
#endif
                apply_TILT(svoDiagReturnValue1);
                svoPreTiltValue = svoDiagReturnValue1;
                if (svoDiagStatus == 0x00)
                {
                    SendMsgCn(DEBUG_AUTO_TUNING_MSG,2,0x338230+ARG1,A2B(svoDiagReturnValue1));
                    calSetCalResult(Layer_Index,Iram_stp_CurrentPosition,ARG1,svoDiagReturnValue1,((ARG2 == TE_P2P_CAL)? (CAL_UNRELIABLE|CAL_R_DONE) : CAL_R_DONE));
                }
                else
                {
                    //mark CAL_SKIP so the calibration will not redo for a while.
                    calSetCalResult(Layer_Index,Iram_stp_CurrentPosition,ARG1,0,CAL_SKIP);
                }
                break;

#if (COLLI_SWITCH == 1)
            case COLLI_POS:
                svoStp_JumpToPosition(COLLI_NORMAL_NO_REST_MODE,svoDiagReturnValue1);
                while (svoStp_busy())
                {
                    ExitProcess();
                }
#if (OPU == BDPL2)  //APCtarget compensate with BD
                HAL_LASC_SetAPCTarget(svoDiagReturnValue1);
#endif
                if (svoDiagStatus == 0x00)
                {
                    SendMsgCn(DEBUG_AUTO_TUNING_MSG,2,0x338230+ARG1,A2B(svoDiagReturnValue1));
                    calSetCalResult(Layer_Index,Iram_stp_CurrentPosition,ARG1,svoDiagReturnValue1,((ARG2 == TE_P2P_CAL)? (CAL_UNRELIABLE|CAL_R_DONE) : CAL_R_DONE));
                }
                else
                {
                    //mark CAL_SKIP so the calibration will not redo for a while.
                    calSetCalResult(Layer_Index,Iram_stp_CurrentPosition,ARG1,0,CAL_SKIP);
                }
                break;
#endif

            case TE_SETPT:
                if (svoDiagStatus == 0x00)
                {
                    //+-300mV max
                    if (svoDiagReturnValue1 > 128)
                        svoDiagReturnValue1 = 128;
                    if (svoDiagReturnValue1 < -128)
                        svoDiagReturnValue1 = -128;
                    SendMsgCn(DEBUG_AUTO_TUNING_MSG,2,0x338230+ARG1,A2B(svoDiagReturnValue1));  // Optimum TE_SETPT: %2d
                    apply_TESPT(0);
                    calSetMisc(Layer_Index, TE_SPT, svoDiagReturnValue1);
                }
                else
                {
                    // Calibration failed. Restore TE set-point settings.
                    apply_TESPT(svoCurvefitCalVar.orig_item);
                }
                break;
        }

        if ((svoDiagStatus == 0x00) || (timeout_chk(0) == eTimerTimeout))
        {
            break;  //end of retries
        }
        SendMsgCnB(SHOW_DEBUG_MSG,1,0x338242,B1B(retries+1));
    }
#if ((BD_2X_STARTUP == 1) && (BD_UNBALANCE_DISC_CHECK))
    if((svoVar_unbalanceDiscNum != 0)&&(svoVar_discKindT.Bit.DiscStd == eDiscBD)
       &&(svoIramVar.svoCurrentSpeed == _2LX_BD))
       spdUnhold();
#endif

#if (DVDDL_TILT_CAL_PRO == 1)
    if ((ARG1 == TILT_DAT) && (meas_signal.errcode ==  eLIMIT_blank_calibrate_TILT) && ( (svoIramVar.svoInState & TRN_MASK) == CCALIB)
        && (svoVar_discKindT.Bit.DiscStd == eDiscDVD) && (svoVar_discKindT.Bit.Layers != eMediaROM) && (TILT_retries <=5) )
    {
        return(SEQ_EX2);
        // if RF modulation too small not retry calibration
    }
    else
    {
        TILT_retries = 0 ;
    }
#endif

    switch (ARG2)
    {
        case TE_P2P_CAL:
            WRITE_FIELD(HAL_AFE_TRKG,svoCalGnT1Var[Layer_Index][TE_GN].groove_gain[RECDH]);
            WRITE_FIELD(HAL_AFE_TRKO,svoCalOfsT1Var[Layer_Index][TE_OS].groove_offset[RECDH]);
            HAL_AFE_SetTELpfBW(orig_TELpfBw);
            break;
    }

    SetTimeout(0,TIMER_STOP);

#if (NEW_FDP_DETECTION != 0)
    CLEAR_INT_FIELD(HAL_SRV_FNOK,0);
    WRITE_FIELD(HAL_SRV_FNOKEN,1);          /* Enable Focus drop interrupt */
    #if (NEW_FDP_DETECTION == 2)
    WRITE_FIELD(HAL_SRV_DSPI6,0);
    WRITE_FIELD(HAL_SRV_DSPI6EN,1);         /* Enable New Focus drop interrupt */
    #endif
#endif
    if (svoDiagStatus == 0x00)
    {
        return(SEQ_EX0);
    }
    else
    {
#if (DVD_RAM_READ == 1)
        if ((DVDRAM) && (ARG1 == FEBC_DSP))
        {
            calSetAfeOffset(0,FE_OS,TYPE1,PATH_SELECT_ALL,PATH_SELECT_ALL,svoCalOfsT1Var[0][FE_OS].groove_offset[RECDH]);

            svoDiagReturnValue1 = 0;
            apply_FEBC(svoDiagReturnValue1);
            SendMsgCn(DEBUG_AUTO_TUNING_MSG,2,0x338230+ARG1,A2B(svoDiagReturnValue1));
            calSetCalResult(Layer_Index,Iram_stp_CurrentPosition,FEBC_DSP,svoDiagReturnValue1,CAL_R_DONE);
    #if (NEW_SERVO_CALIB_FLOW == 0)
            if ((svoIramVar.svoInState & TRN_MASK) == SVO_TGAFE)
                svoCalVar.calGainDoneBits &= ~teg_end;
    #endif
        }
#endif
        return(SEQ_EX1);
    }
}

/*-----------------------------------------------------------------------------
 *
 * Module: state_diaCmd_RRFGCal()
 * Description:
 *
 *----------------------------------------------------------------------------*/
#if 0
#define Drooprate_800   0x3F
#define Drooprate_400   0x2F
#define Drooprate_200   0x1F
#define Drooprate_100   0x0F
#define Drooprate_50    0x0E
#define Drooprate_25    0x0D
#define Drooprate_12P5  0x0C
#define ini_afe_table   0x4A
#endif

seq_exit_t state_diaCmd_RRFGCal(void)
{
//TBD! Need to be covered by DFE
    return(SEQ_EX0);
}

/*-----------------------------------------------------------------------------
 *
 * Module: state_diaCmd_BETAoffsetCal()
 * Description:
 *
 *----------------------------------------------------------------------------*/
#define High_PassFilter  1
#define Low_passFilter   0
#define Select_Filter      High_PassFilter
seq_exit_t state_diaCmd_BETACal(void)
{
   return(SEQ_EX0);
}

/*-----------------------------------------------------------------------------
 *
 * Module: state_diaCmd_ReadBETA()
 * Description:
 *
 *----------------------------------------------------------------------------*/
seq_exit_t state_diaCmd_ReadBETA(void)
{
    USHORT  num,cnt,i;
    long    adPHO,adBHO,PH_BH,adREF,adVREFMB;
    float   std_beta,xBeta,tempData,betaoff;

    SendMsg80(DEBUG_OFFLINE_CALIB_MSG,0x338500);
    num=32;
    xBeta=0.0;

    SetTimeout(0, ARG2);

    //Select Reference Level
    WRITE_FIELD(HAL_AFE_MUX1ADSEL,halAFE_MUX1ADSEL_VREFI);

    WaitTimer(20L);

    if (pdbDiscLoaded() == DVD_DISC)
        betaoff=svoPcalVar.dvBetaOffset*0.001;
    else//CD
        betaoff=svoPcalVar.cdBetaOffset*0.001;

    SendMsgCn(DEBUG_OFFLINE_CALIB_MSG,2,0x338502,A2B(svoPcalVar.dvBetaOffset));

    for (cnt=0;cnt<150;cnt++)
    {
        adVREFMB=0.0;
        for (i=0;i<num;i++)
        {   //get adVREFMB 32 times
            adVREFMB += ReadSingleAdcmV(ADC_MUXOUT1);
        }
        adREF=adVREFMB/num;
        adPHO = Read_PHO(32);
        adBHO = Read_BHO(32);
        PH_BH=adPHO-adBHO;

#if 0
        if (timeout_chk(0) == eTimerTimeout ||(cnt <=100 && (PH_BH)<100))   /* time out check */
        {
            SetTimeout(0, TIMER_STOP);
            svoDiagStatus=0x80;
            return(SEQ_EX1);
        }
#else // 0
        if (timeout_chk(0) == eTimerTimeout)   /* time out check */
        {
            send_msg1('E','R','R','!');
            SetTimeout(0, TIMER_STOP);
            svoDiagStatus=0x80;
            return(SEQ_EX1);
        }
#endif // 0

        if(adBHO<1500L)
            std_beta = ((float)adPHO + (float)adBHO - (float)(adREF*2.0f))/((float)adPHO - (float)adBHO);
        else
            std_beta = -1.0f;

        tempData = std_beta + betaoff;
        xBeta += tempData;
   }

   xBeta = (xBeta*100.0f)/cnt;

   svoDiagReturnValue1 = (int)(xBeta*10);    //10X
   SendMsgCn(DEBUG_OFFLINE_CALIB_MSG,2,0x338501,A2B(svoDiagReturnValue1));
   svoDiagStatus = 0x00;

   return(SEQ_EX0);
}

/*******************************************************************************

    FUNCTION        WtLPP_Adj_Slice
    DESCRIPTION     Adjust LPP slice level for -R write
    PARAMETERS      Level:(1):7 (2):9 (3):5
    RETURNS         void
    NOTES

********************************************************************************/
#if LPP_AUTO_LEARN_TEST == 1
void WtLPP_Adj_Slice(void)
{
#if (WOBBLE_READ_ON_DVD_OR_CD == 1)
    BYTE WLPP_level;

    if (READ_FIELD(HAL_WS_WGATEMON) && ((pdbServoGetfDiscKind() == eDVDMinusRWDisc) || (pdbGetSpeed() != e8XDPCAV)))
    {
        if(WtLPP_Dec_ok==0) //WtLPP Dec NO OK
        {
            WtLPP_Decing++;
            //send_msg1('D','e','c','o');
        }
        else //WtLPP Dec ok
        {
            WtLPP_Dec_ok=0;
            WtLPP_Decing=0;
            //send_msg1('D','e','o','k');
#if 1
            if(Iram_AtipAdipLppErrorAmount>20)
            {
                WtLPP_Er_large++;
                if(WtLPP_Er_large>40)
                {
                    if(WtUP_LPP_Slvel==0)
                    {
                        WLPP_level = GetSCRegField_1B(f_rw_HDRTHRSH4)+1;
                        if(WLPP_level > mskHDRTHRSH4)
                            WLPP_level = mskHDRTHRSH4;
                        SetSCRegField_1B(f_rw_HDRTHRSH4,WLPP_level);
                        WtUP_LPP_Slvel=1;
                        WtLPP_Decing=0;
                        WtLPP_Er_large=0;
                    }
                    else if(WtUP_LPP_Slvel==1)
                    {
                         WLPP_level = GetSCRegField_1B(f_rw_HDRTHRSH4)-2;
                         if(WLPP_level < LPP_HDRTHRSH_DIF)
                            WLPP_level = LPP_HDRTHRSH_DIF;
                         SetSCRegField_1B(f_rw_HDRTHRSH4,WLPP_level);
                         WtUP_LPP_Slvel=2;
                         WtLPP_Decing=0;
                         WtLPP_Er_large=0;
                    }
                    else
                    {
                        WLPP_level = GetSCRegField_1B(f_rw_HDRTHRSH4)+1;
                        if(WLPP_level > mskHDRTHRSH4)
                            WLPP_level = mskHDRTHRSH4;
                        SetSCRegField_1B(f_rw_HDRTHRSH4,WLPP_level);
                        WtUP_LPP_Slvel=0;
                        WtLPP_Decing=0;
                        WtLPP_Er_large=0;
                    }
                }
            }
            else
            {
                WtLPP_Er_large=0;
            }
#endif // #if 1
        }

        if(WtLPP_Decing>50) //WtLPP_Decing fail
        {
            if(WtNRUP_LPP_Slvel==0)
            {
                WLPP_level = GetSCRegField_1B(f_rw_HDRTHRSH4)+2;
                if(WLPP_level > mskHDRTHRSH4)
                    WLPP_level = mskHDRTHRSH4;
                SetSCRegField_1B(f_rw_HDRTHRSH4,WLPP_level);
                WtNRUP_LPP_Slvel=1;
                WtLPP_Decing=0;
            }
            else if(WtNRUP_LPP_Slvel==1)
            {
                WLPP_level = GetSCRegField_1B(f_rw_HDRTHRSH4)-4;
                if(WLPP_level < LPP_HDRTHRSH_DIF)
                    WLPP_level = LPP_HDRTHRSH_DIF;
                SetSCRegField_1B(f_rw_HDRTHRSH4,WLPP_level);
                WtNRUP_LPP_Slvel=2;
                WtLPP_Decing=0;
            }
            else
            {
                WLPP_level = GetSCRegField_1B(f_rw_HDRTHRSH4)+2;
                if(WLPP_level > mskHDRTHRSH4)
                    WLPP_level = mskHDRTHRSH4;
                SetSCRegField_1B(f_rw_HDRTHRSH4,WLPP_level);
                WtNRUP_LPP_Slvel=0;
                WtLPP_Decing=0;
            }
        }
        WaitTimer(1L);
    }
#endif // (WOBBLE_READ_ON_DVD_OR_CD == 1)
}
#endif // LPP_AUTO_LEARN_TEST

#if (SERVICE_MODE==1)
seq_exit_t state_laser_on(void)
{
    HAL_LASC_SetLaserOn(TRUE);
    return(SEQ_EX0);
}

/*-----------------------------------------------------------------------------
 *
 * Module: state_serv_foc_chk()
 * Description: If in focus it's Ready
 *
 *----------------------------------------------------------------------------*/
seq_exit_t state_serv_foc_chk(void)
{
    if (FokStatusOK())
        return(SEQ_EX0);
    else
        return(SEQ_EX1);
}

/*-----------------------------------------------------------------------------
 *
 * Module: state_urJump_init()
 * Description: Initialze seek variables for jump track command from UART
 *
 *----------------------------------------------------------------------------*/
seq_exit_t state_urJump_init(void)
{
    svoSeekVar.svoTargetID.SectorNum.all32 = svoIramVar.currentID.SectorNum.all32;
    svoSeekVar.advance = 0;
    return(SEQ_EX0);
}
#endif // SERVICE_MODE

/*-----------------------------------------------------------------------------
 *
 * Module: state_layerjump_start()
 * Description: This function will perform DVD layer jump
 *
 *----------------------------------------------------------------------------*/
seq_exit_t state_layerjump_start(void)
{
    //send_msg80(DEBUG_CALIB_ENTRYS_MSG,0x5A012E); // >>> STATE_LAYERJUMP_START <<<

    if ((svoIramVar.svoInState & TRN_MASK) == SVO_PAUSE)
    {
        if (svoVar_abort_pause_if_applyable == TRUE)
        {
            return(SEQ_EX0);  // time_chk: make it done anyway
        }
    }

    if (svo_Layer_Jump(svoSeekVar.svoTargetID.Layer) == Ready)
    {
        return(SEQ_EX0);
    }
    else
    {
        return(SEQ_EX1);
    }
}

/*-----------------------------------------------------------------------------
 *
 * Module: state_layerjump_err()
 * Description: DVD Layer Jump error handling
 *
 *----------------------------------------------------------------------------*/
seq_exit_t state_layerjump_err(void)
{
    svoMiscVar.LayerjumpRetryCount++;
    if (!FokStatusOK())
    {
        return(SEQ_EX1);
    }

    if((svoIramVar.svoInState & TRN_MASK) == SVO_SEEK){
        svoIramVar.focusDropFlag = 1;
    }

    svoIramVar.fineSeekDoneFlag = TRUE;

    if (FokStatusOK())
    {
        DISABLE_INTERRUPT();

        wait_track_on(TRK_ON_WAIT_SLOW_TIMEOUT);

        //SetDfeWblFrontSemiSynchr();
        //Delay100uS(1);
        //SetDfeWblFrontBitSynchr();

#if DO_HW_SUPPORT_STEP == 1
        hw_step_motor_normal_play();
#else
        svoStp_SetTimerMode( STEP_READ_TIMER );
#endif

        ENABLE_INTERRUPT();
    }
    return(SEQ_EX0);
}

/*-----------------------------------------------------------------------------
 *
 * Module: state_pp_det()
 * Description: EX0: normal
 *              EX1: wrong layer detected (BD only)
 *              EX2: not DualDisc (DDisc == 1)
 *              EX3: BD LTH detected (BD only)
 *              EX4: HD detected
 *----------------------------------------------------------------------------*/
seq_exit_t state_pp_det(void)
{
    seq_exit_t ret_seq;
    USHORT avgSRFOpp;
    ad_dsp_t te_signal;
#if (BD_USE_TE_PP_DPD == 1)
    SHORT TEpp_diff_pp_dpd;
#endif
    USHORT PPRatioX100;
#if (((BD_ENABLE == 1) && (BD_WOBBLE_DET == 0)) || (MPP_TE_DETECT_DVDR == 1))
    USHORT pp_ratio_th;
#endif
    BYTE  orig_TELpfBw;
    SHORT orig_pdic_gn, orig_laser_gn;
#if ((BD_ENABLE == 1) && (BD_WOBBLE_DET == 1) && (BD_USE_TE_PP_DPD == 0))
    ULONG wob_osx100,wob_osx100_sum;
    BYTE  tmp_wdiffgain,tmp_agchpffc,tmp_agchpfdf,tmp_agciout;
    ULONG wob_pll_lock_cnt,wob_sync_lock_cnt;
    BYTE  org_media;
    BYTE  tmp_LCKDETPER,tmp_LCKDETTHR;
    USHORT waitCount;
    SHORT smp_cnt;
    BYTE  tmp_LCKAIDPH;
#endif

    ret_seq = SEQ_EX0;

    if (((svoCalVar.discChkDoneBits & disc_chk_end) == disc_chk_end)
#if (ENABLE_BDRLTH == 1)
        || (svoIramVar.LTHDisclikely == 1)
#endif
       )
    {
        return(ret_seq);
    }

    orig_pdic_gn = svoCalMisc2Var.pdic_gn;
    orig_laser_gn = svoCalMisc2Var.laser_gn;
    orig_TELpfBw = HAL_AFE_GetTELpfBW();
    HAL_AFE_SetTELpfBW(e_TELPF_300); //M.H. Note: 300KHZ is 330KHz for SC6230

    WRITE_FIELD(HAL_AFE_MUX1ADSEL, halAFE_MUX1ADSEL_SRFO);
    switch (svoVar_discKindT.Bit.DiscStd)
    {
#if (BD_ENABLE == 1)
        case eDiscBD:
    #if (PCB != 830)
        #if (APP_TE_SUPPORT == 1)
            if (((READ_REG(HAL_AFEREG_MAP->MB16) & (HAL_AFE_MPPAH|HAL_AFE_MPPBG)) == (HAL_AFE_MPPAH|HAL_AFE_MPPBG)) &&
                ((READ_REG(HAL_AFEREG_MAP->MB15) & (HAL_AFE_MPPA|HAL_AFE_MPPB|HAL_AFE_MPPC|HAL_AFE_MPPD)) == (HAL_AFE_MPPA|HAL_AFE_MPPB)) &&
                ((READ_REG(HAL_AFEREG_MAP->SB14) & (HAL_AFE_SPP1E|HAL_AFE_SPP1F|HAL_AFE_SPP1G|HAL_AFE_SPP1H)) == (HAL_AFE_SPP1E|HAL_AFE_SPP1F)))
            {
                //use APP/SRFO ratio
                afeSetTrackingMethod(TRK_METHOD_DPP);
            }
            else
        #endif
            {
                //use MPP/SRFO ratio
                afeSetTrackingMethod(TRK_METHOD_MPP);
            }
        #if ((BD_WOBBLE_DET == 1) && (BD_USE_TE_PP_DPD == 0))

            #if (BD_2X_STARTUP == 1)
            if (svoVar_discKindT.Bit.Media == eMediaROM)
                waitCount = 100;
            else
                waitCount = 600;//For BDR/RE DL ECC
            #else
                waitCount = 100;
            #endif//BD_2X_STARTUP

            //store original settings
            tmp_agciout = READ_FIELD(HAL_AFE_AGCIOUT);
            tmp_wdiffgain = READ_FIELD(HAL_AFE_WDIFFGAIN);
            tmp_agchpffc = READ_FIELD(HAL_AFE_AGCHPFFC);
            tmp_agchpfdf = READ_FIELD(HAL_AFE_AGCHPFDF);
            tmp_LCKDETPER = READ_FIELD(HAL_WBL_LCKDETPER);
            tmp_LCKDETTHR = READ_FIELD(HAL_WBL_LCKDETTHR);
            tmp_LCKAIDPH = READ_FIELD(HAL_WBL_LCKAIDPH);
            org_media = svoVar_discKindT.Bit.Media;
            svoVar_discKindT.Bit.Media = eMediaUnknown;

            /* Count the number of wobble pll lock to detect BD disc type */
            /* Filter out track crossing signal and leave wobble signal */
            /* Can add some diff wob gain if wobble amp. still too small */
            #if(PCB == 815)
            //WRITE_FIELD(HAL_AFE_AGCIOUT,halAFE_AGCAmptdSetlTime_0p06ms);/* set AGC1/AGC2 Amplitude Settling Time(Attack Rate)*/
            WRITE_FIELD(HAL_AFE_AGCIOUT,halAFE_AGCAmptdSetlTime_0p5ms);
            WRITE_FIELD(HAL_AFE_WDIFFGAIN, halAFE_WblSumAmp_9p5dB);     /* set diff wobble gain */
            WRITE_FIELD(HAL_AFE_AGCHPFFC, halAFE_AGCHPF_1500kHz7);        /* Set AGC1/AGC2 HPF B.W.*/
            WRITE_FIELD(HAL_AFE_AGCHPFDF,0x00);                         /* AGC1/AGC2 HPF B.W. is determined by AGCHPFFC */
            #else
            WRITE_FIELD(HAL_AFE_WDIFFGAIN, 0x04);   /* set diff wobble gain */
            WRITE_FIELD(HAL_AFE_AGCHPFFC, 0x00);    /* Set AGC1/AGC2 HPF B.W. to 100kHz */
            WRITE_FIELD(HAL_AFE_AGCHPFDF,0x00);     /* AGC1/AGC2 HPF B.W. is determined by AGCHPFFC */
            #endif

            #if (BD_2X_STARTUP == 1)
            WRITE_FIELD(HAL_WBL_LCKDETPER, 0x02);//If this value is too low (<= 1), then ROM may be judged to R/RW
            WRITE_FIELD(HAL_WBL_LCKDETTHR, 0x08);
            #endif
            //WRITE_FIELD(HAL_AFE_WOBLPFBW,0x00);
            //WRITE_FIELD(HAL_AFE_WBVGA3REG,0x17);

            SetDfeWblFrontSemiSynchrAfterTracking();
        #endif
            break;
    #else
            //SRFO based RWdisc_check is more reliable than TE ratio based pp_det on BD discs
            //No difference in PPRatio on BDROM and BDR/E discs
            send_msg81(DEBUG_DISK_ID_MSG,0x510057,2);         //...PP Detection SKIP...
            HAL_AFE_SetTELpfBW(orig_TELpfBw);
            return(ret_seq);
    #endif
#endif
#if (ENABLE_HDDVD == 1)
        case eDiscHD:
            afeSetTrackingMethod(TRK_METHOD_MPP);
            break;
#endif
        case eDiscCD:
#if ((DDisc == 0) && (SUPPORT_DPD == 0) && (DETECT_CDR == 0))
            HAL_AFE_SetTELpfBW(orig_TELpfBw);
            return(ret_seq);
#else
            if (svoVar_TrackingMethod == TRK_METHOD_SPPO1)
            {
                WRITE_FIELD(HAL_AFE_MUX1ADSEL, halAFE_MUX1ADSEL_SBAD);
            }
            else
            {
                afeSetTrackingMethod(TRK_METHOD_MPP);
            }
            break;
#endif
        case eDiscDVD:
#if ((ENABLE_HDDVD == 1) && (FORCE_HD_START_UP_FLOW == 0))
            if (svoVar_discKindT.Bit.Media != eMediaRW)//HDRW doesn't exist yet
            {
                afeSetTrackingMethod(TRK_METHOD_DPD);
                te_signal = ReadFilteredDSPADCmV(ONE_REVOLUTION,TE_ALL,ADC_MUXOUT1,TRUE);
                avgSRFOpp = te_signal.top_mv - vref_fm_mux3_mV;
                PPRatioX100 = (te_signal.p2p_mv * 100) / avgSRFOpp;

                svoPrvtVar_TEBSRatioX100 = PPRatioX100 ;
                if (te_signal.errcode != eNOERR)
                {
    #if(ERROR_CAL_START_UP_MONITOR ==1)
                    svoMiscVar.calibrationErrorCount++;
    #endif
                    SendMsgCnB(SHOW_DEBUG_MSG,4,0x3A0001,B1B(te_signal.errcode),B1B(((svoIramVar.svoInState & STAT_MASK) >> 12)),B1B(((svoIramVar.svoInState & TRN_MASK ) >> 5) + 1),B1B((svoIramVar.svoInState & SEQ_MASK) + 1));
                    if (++cal_retry < 5)
                    {
                        HAL_AFE_SetTELpfBW(orig_TELpfBw);
                        return(SEQ_BUSY);
                    }
                }
                SendMsgCn(DEBUG_DISK_ID_MSG,4,0x310055,A2B(te_signal.p2p_mv),A2B(avgSRFOpp));
                SendMsgCn(SHOW_DEBUG_MSG,4,0x310058,A4B(PPRatioX100));

                if (svoVar_discKindT.Bit.Layers == eLayersSingle)
                {
                    pp_ratio_th = HD_TH * 100;
                }
                else
                {
                    pp_ratio_th = HDDL_TH * 100;
                }

                if (PPRatioX100 > pp_ratio_th)
                {
                    SendMsg80(SHOW_DEBUG_MSG,0x320A0F);
                }
                else  //HD disc
                {
                    SendMsg80(SHOW_DEBUG_MSG,0x320A0E);
                    svoVar_discKindT.Bit.DiscStd = eDiscHD;
                    svoCalVar.calDoneBits = vref_end;
                    cal_retry = 0;
                    track_off();
                    svoSetLDOff();
                    return(SEQ_EX4);
                }
            }
#endif
    #if (MPP_TE_DETECT_DVDR == 1) || (OPU == KEM350AAA)
            if (svoVar_discKindT.Bit.Media != eMediaRW)
            {
                afeSetTrackingMethod(TRK_METHOD_MPP);
                break;
            }
    #endif
            cal_retry = 0;
            HAL_AFE_SetTELpfBW(orig_TELpfBw);
            return(ret_seq);
    }
#if (AFE_NORMALIZER == 1)
    WRITE_FIELD(HAL_AFE_TE2AUTOEN, 0);
#endif
    SendMsgCn(DEBUG_CALIB_ENTRYS_MSG | DEBUG_DISK_ID_MSG,0,0x3A010D); //Disc Kind Detection
    SendMsg80(DEBUG_COEF_MSG,0x3C0000+svoVar_discKindT.fDiscKind);  // Disc_Kind,XXX

#if (BD_USE_TE_PP_DPD == 1)
    if ((svoVar_discKindT.Bit.DiscStd == eDiscBD) && (svoRRFVar.modulation[Layer_Index] != 0))
    {
        te_signal = ReadMPPDPDPhase(ONE_REVOLUTION);

        if (te_signal.errcode != eNOERR)
        {
    #if (AFE_NORMALIZER == 1)
            WRITE_FIELD(HAL_AFE_TE2AUTOEN, 1);
    #endif
            HAL_AFE_SetTELpfBW(orig_TELpfBw);
    #if(ERROR_CAL_START_UP_MONITOR ==1)
            svoMiscVar.calibrationErrorCount++;
    #endif
            SendMsgCnB(SHOW_DEBUG_MSG,4,0x3A0001,B1B(te_signal.errcode),B1B(((svoIramVar.svoInState & STAT_MASK) >> 12)),B1B(((svoIramVar.svoInState & TRN_MASK ) >> 5) + 1),B1B((svoIramVar.svoInState & SEQ_MASK) + 1));
            if (++cal_retry < 5)
            {
                return(SEQ_BUSY);
            }
        }
        TEpp_diff_pp_dpd = te_signal.middle_mv;

        SendMsgCn(DEBUG_DISK_ID_MSG,2,0x310066,A2B(TEpp_diff_pp_dpd));//TEpp_diff (PP+-DPD) = %4d (mV)

    #if (SERVOKEYPARAMETER == 1)
        svoPrvtVar_pp_dpd_diff_mv = TEpp_diff_pp_dpd;
    #endif
    }
#endif

    te_signal = ReadFilteredDSPADCmV(ONE_REVOLUTION,TE_ALL,ADC_MUXOUT1,TRUE);
    avgSRFOpp = te_signal.top_mv - vref_fm_mux3_mV;
    PPRatioX100 = (te_signal.p2p_mv*100) / avgSRFOpp;

#if (AFE_NORMALIZER == 1)
    WRITE_FIELD(HAL_AFE_TE2AUTOEN, 1);
#endif
    HAL_AFE_SetTELpfBW(orig_TELpfBw);

    if (te_signal.errcode != eNOERR)
    {
#if(ERROR_CAL_START_UP_MONITOR ==1)
        svoMiscVar.calibrationErrorCount++;
#endif
        SendMsgCnB(SHOW_DEBUG_MSG,4,0x3A0001,B1B(te_signal.errcode),B1B(((svoIramVar.svoInState & STAT_MASK) >> 12)),B1B(((svoIramVar.svoInState & TRN_MASK ) >> 5) + 1),B1B((svoIramVar.svoInState & SEQ_MASK) + 1));
        if (++cal_retry < 5)
        {
            return(SEQ_BUSY);
        }
    }
    cal_retry = 0;

    SendMsgCn(DEBUG_DISK_ID_MSG,4,0x310055,A2B(te_signal.p2p_mv),A2B(avgSRFOpp));
    SendMsgCnB(SHOW_DEBUG_MSG,2,0x310058,F4B((float)PPRatioX100/100));
    svoPrvtVar_TEBSRatioX100 = PPRatioX100 ;

    switch (svoVar_discKindT.Bit.DiscStd)
    {
        case eDiscCD:
#if DDisc == 1
            if (DualDisc != 0)
            {
                if (PPRatioX100 < CD_DUAL_TH*100)
                {
                    svoCalVar.calDoneBits = vref_end;
                    SendMsg80(SHOW_DEBUG_MSG,0x320A0D);
                    track_off();
                    svoSetLDOff();
                    return(SEQ_EX2);
                }
                SendMsg80(SHOW_DEBUG_MSG,0x320A0C);
            }
#endif

#if (SUPPORT_DPD == 1)
            if (((svoCalVar.calDoneBits & L0_blank_det) == 0) &&
                ((PPRatioX100 <= CD_DPD_TP_TH*100)
    #if (PCB == 710)||(PCB == 711)||(PCB == 817)
              || ((svoPrvtVar_maxSRFOpp_0dB > Beall_SRFOpp_0dB_MinTH) && (svoPrvtVar_maxSRFOpp_0dB < Beall_SRFOpp_0dB_MaxTH) && (PPRatioX100 < Beall_DPD_TP_TH*100))
    #elif (PCB == 815)
              || ((svoPrvtVar_maxSRFOpp_0dB > Beall_SRFOpp_0dB_MinTH) &&
                  ((svoPrvtVar_maxSRFOpp_0dB < Beall_SRFOpp_0dB_MaxTH) || (svoPrvtVar_maxFEpp_0dB < Beall_FEpp_0dB_MaxTH)) &&
                  (PPRatioX100 < Beall_DPD_TP_TH*100))
    #endif
               ))
            {
                SendMsg80(SHOW_DEBUG_MSG,0x310404);
                svoIramVar.do_cd_dpd_disc = TRUE;               /* use DPD servo*/
                SendMsg80(SHOW_DEBUG_MSG,0x3100C1);
                LAL_StoreParams(LAL_DetectDiscKind(svoVar_discKindT), svoIramVar.svoCurrentSpeed);
                LAL_SetReadMode();
    #if (EN_POWER_CONTROL == 1)
        #if (WOBBLE_READ_ON_DVD_OR_CD == 1)
                PowerControl(eCDReadPower);
        #endif
    #endif
            }
            else
            {
                SendMsg80(DEBUG_DISK_ID_MSG,0x310405); // cd_dpd_DETECITON==> Non cd_dpd disc
                svoIramVar.do_cd_dpd_disc = FALSE;
            }
#endif // if SUPPORT_DPD == 1

#if (DETECT_CDR == 1)
            if ((svoVar_discKindT.Bit.Media!=eMediaRW)&&(svoIramVar.do_cd_dpd_disc == FALSE))
            {
                if ((svoCalVar.calDoneBits & L0_blank_det) || (PPRatioX100 > CDROM_TH*100))
                {
                    SendMsg80(SHOW_DEBUG_MSG,0x3100A7); // ATIP_check==>CD-R Disc
                    svoVar_discKindT.Bit.Media = eMediaR;
                    LAL_StoreParams(LAL_DetectDiscKind(svoVar_discKindT), svoIramVar.svoCurrentSpeed);
                    LAL_SetReadMode();
                }
            }
#endif
            break;

#if (BD_ENABLE == 1)
        case eDiscBD:
            if ((svoVar_discKindT.Bit.Layers == eLayersDouble) && (PPRatioX100 < BD_WRONG_LAYER_TH*100))
            {//lock to layer 1 instead of layer 0
                SendMsg80(SHOW_DEBUG_MSG,0x33400C);
                svoStartUpWrongLayer();
                ret_seq = SEQ_EX1;
            }
            else
            {
    #if ((BD_WOBBLE_DET == 1) && (BD_USE_TE_PP_DPD == 0))
                if (svoCalVar.calDoneBits & L0_blank_det)
                {
                    wob_pll_lock_cnt = wob_sync_lock_cnt = waitCount;
                }
                else
                {
                    smp_cnt = 0;
                    wob_pll_lock_cnt = wob_sync_lock_cnt = 0;
                    wob_osx100 = wob_osx100_sum = 0;

                    do
                    {
        #if (BD_2X_STARTUP == 1)
                        if(smp_cnt&0x01)
                            WRITE_FIELD(HAL_WBL_LCKAIDPH, (SBYTE)COEF_GetValue(WBL_CALC_coef_LOCK_AID_PH) );
                        else
                            WRITE_FIELD(HAL_WBL_LCKAIDPH, (SBYTE)-COEF_GetValue(WBL_CALC_coef_LOCK_AID_PH) );
        #endif
                        /* Check wobble frequency */
                        wob_osx100 = HAL_WOBBLE_GetOverspeed(HAL_PLL_GetWblClkKHz());
                        SendMsgCnB(DEBUG_WBL_MSG,4,0x41022E,B4B(wob_osx100)); //WOBBLE overspeed x 100

                        /* Check wobble pll lock status */
                        if (HAL_WOBBLE_GetPllInLock(HAL_PLL_GetWblClkKHz()))
                        {
                            wob_pll_lock_cnt++;
                            /* Check BD bit sync & word sync lock status */
                            if (HAL_WOBBLE_GetSyncFlyWheelInLock())
                                wob_sync_lock_cnt++;
                        }

                        wob_osx100_sum += wob_osx100;
                        smp_cnt ++;

                        if(wob_pll_lock_cnt >= 1)
                            break;

                        WaitTimer(1);

                    }while(smp_cnt < waitCount);
                    //}while(smp_cnt < 200);

                    wob_osx100 = wob_osx100_sum/smp_cnt;
//                    SendMsgCn(SHOW_DEBUG_MSG,4,0x41022E,A2B(wob_osx100),A2B(wob_pll_lock_cnt)); //WOBBLE speed x 100
                    SendMsgCnB(SHOW_DEBUG_MSG,4,0x41022E,B2B(wob_osx100),B2B(smp_cnt));
        #if (PCB == 710)||(PCB == 711)||(PCB == 815)||(PCB == 817)
//                    svoPrvtVar_wbllockcnt = wob_pll_lock_cnt;
                    svoPrvtVar_wbllockcnt = smp_cnt;
        #endif

                    // restore original settings
                    WRITE_FIELD(HAL_AFE_AGCIOUT,tmp_agciout);
                    WRITE_FIELD(HAL_AFE_WDIFFGAIN, tmp_wdiffgain);
                    WRITE_FIELD(HAL_AFE_AGCHPFFC, tmp_agchpffc);
                    WRITE_FIELD(HAL_AFE_AGCHPFDF,tmp_agchpfdf);
                    svoVar_discKindT.Bit.Media = org_media;

                    WRITE_FIELD(HAL_WBL_LCKDETPER, tmp_LCKDETPER);
                    WRITE_FIELD(HAL_WBL_LCKDETTHR,tmp_LCKDETTHR);
                    WRITE_FIELD(HAL_WBL_LCKAIDPH, tmp_LCKAIDPH);
                    //WRITE_FIELD(HAL_AFE_WOBLPFBW,tmp_woblpfbw);
                    //WRITE_FIELD(HAL_AFE_WBVGA3REG,0x10);
                    //WRITE_FIELD(HAL_WS_GAINSWR, halWS_GAINSWR_Zero);
                }

                if (wob_pll_lock_cnt < 1)//T.B.D
    #else
                if (svoVar_discKindT.Bit.Layers == eLayersSingle)
                {
                    pp_ratio_th = BDROM_TH * 100;
                }
                else
                {
                    pp_ratio_th = BDROMDL_TH * 100;
                }
                if (svoVar_discKindT.Bit.Media == eMediaRW)
                {
                    pp_ratio_th = pp_ratio_th * 3 / 4;
                }
        #if (BD_USE_TE_PP_DPD == 1)
                if (((svoCalVar.calDoneBits & L0_blank_det) == 0) && (TEpp_diff_pp_dpd < 0) && (PPRatioX100 <= pp_ratio_th))
        #else
                if (((svoCalVar.calDoneBits & L0_blank_det) == 0) && (PPRatioX100 <= pp_ratio_th))
        #endif
    #endif
                {   //BDROM
                    SendMsg80(SHOW_DEBUG_MSG,0x3100D0+svoVar_discKindT.Bit.Layers-1);   //PP_det==>BD-ROM xL Disc

                    if (svoVar_discKindT.Bit.Media != eMediaROM)
                    {
                        svoVar_discKindT.Bit.Media = eMediaROM;
                        LAL_StoreParams(LAL_DetectDiscKind(svoVar_discKindT), svoIramVar.svoCurrentSpeed);
                        LAL_SetReadMode();
                    }
                }
                else
                {   //BDR/RE
                    if ((svoVar_discKindT.Bit.Media != eMediaRW)&&(TEpp_diff_pp_dpd > 0))
                    {
                        svoVar_discKindT.Bit.Media = eMediaRW;
                        LAL_StoreParams(LAL_DetectDiscKind(svoVar_discKindT), svoIramVar.svoCurrentSpeed);
                        LAL_SetReadMode();
                    }

    #if (ENABLE_BDRLTH == 1)
        #if (BD_USE_TE_PP_DPD == 1)
                    if ((svoRRFVar.modulation[Layer_Index] == 0)&&(svoVar_discKindT.Bit.Layers==eLayersSingle))
                    {
                        svoIramVar.LTHDisclikely = 1;
                    }
                    else if ((TEpp_diff_pp_dpd < 0)&&(svoVar_discKindT.Bit.Layers==eLayersSingle))
        #else
                    if (PPRatioX100 > BDRLTH_TH*100)
        #endif
                    {
                        SendMsg80(SHOW_DEBUG_MSG,0x3100DB);//PP_det==>BD-R LTH Disc
                        svoIramVar.LTHDisclikely = 1;
                        svoIramVar.LTHDisc = 1;
                        LAL_SetAFEandDFEforLTH();
                        svoCalVar.calOfsDoneBits &= ~(ofaj_end|teof_end);
                        ret_seq = SEQ_EX3;
                    }
    #endif //ENABLE_BDRLTH
                    if(TEpp_diff_pp_dpd > 0)
                    {
                        SendMsg80(SHOW_DEBUG_MSG,0x3100D2+svoVar_discKindT.Bit.Layers-1);   //PP_det==>BD-R/RE xL Disc
                    }
                    else
                    { 
                        if(!svoIramVar.LTHDisclikely)
                        {
                            SendMsg80(SHOW_DEBUG_MSG,0x3100D0+svoVar_discKindT.Bit.Layers-1);   //PP_det==>BD-ROM xL Disc
                        }
                    }
                }
            }
    #if (EN_BD_ADEQ == 1)
            InitAdaptEq();
    #endif
            break;
#endif // (BD_ENABLE == 1)

        case eDiscDVD:
#if (MPP_TE_DETECT_DVDR == 1)//maybe change to OPU dependency? below code needs DPP based TE on DVD.
            if (svoVar_discKindT.Bit.Layers == eLayersSingle)
            {
                pp_ratio_th = DVDROM_TH * 100;
            }
            else
            {
                pp_ratio_th = DVDROMDL_TH * 100;
            }

            if (((svoCalVar.calDoneBits & L0_blank_det) == 0) && (PPRatioX100 <= pp_ratio_th))
            {   //DVDROM
                SendMsg80(SHOW_DEBUG_MSG,0x3100D4+svoVar_discKindT.Bit.Layers-1);   //PP_det==>DVD-ROM xL Disc
    #if (EN_POWER_CONTROL == 1)
        #if (WOBBLE_READ_ON_DVD_OR_CD == 1)
                PowerControl(eDVDReadPower);
        #endif
    #endif
                if (svoVar_discKindT.Bit.Media != eMediaROM)
                {
                    svoVar_discKindT.Bit.Media = eMediaROM;
                    LAL_StoreParams(LAL_DetectDiscKind(svoVar_discKindT), svoIramVar.svoCurrentSpeed);
                    LAL_SetReadMode();
                }
            }
            else
            {   //DVD R/RW
                if (svoVar_discKindT.Bit.Media == eMediaRW)
                {   //DVD RW
                    SendMsg80(SHOW_DEBUG_MSG,0x3100D8+svoVar_discKindT.Bit.Layers-1);   //PP_det==>DVD RW xL Disc
                }
                else
                {   //DVD R
                    SendMsg80(SHOW_DEBUG_MSG,0x3100D6+svoVar_discKindT.Bit.Layers-1);   //PP_det==>DVD R xL Disc
                    svoVar_discKindT.Bit.Media = eMediaR;
                    svoVar_discKindT.Bit.WblType = eWblMinus;
                    LAL_StoreParams(LAL_DetectDiscKind(svoVar_discKindT), svoIramVar.svoCurrentSpeed);
                    LAL_SetReadMode();
                }
#if (EN_ADEQ == 1)
                InitAdaptEq();
    #endif
            }
#endif //
            break;
#if (ENABLE_HDDVD == 1)
        case eDiscHD:
            if (svoVar_discKindT.Bit.Layers == eLayersSingle)
            {
                pp_ratio_th = HDROM_TH * 100;
            }
            else
            {
                pp_ratio_th = HDROMDL_TH * 100;
            }

            if (((svoCalVar.calDoneBits & L0_blank_det) == 0) && (PPRatioX100 <= pp_ratio_th))
            {
                SendMsg80(SHOW_DEBUG_MSG,0x320A0A);
                svoVar_discKindT.Bit.Media = eMediaROM;
            }
            else //HDR/RW
            {
                SendMsg80(SHOW_DEBUG_MSG,0x320A09);
                svoVar_discKindT.Bit.Media = eMediaR;
            }
            break;
#endif //ENABLE_HDDVD
    }
    afeSetTrackingMethod(getTrackingMethod(svoVar_discKindT));
    if ((orig_pdic_gn != svoCalMisc2Var.pdic_gn) || (orig_laser_gn != svoCalMisc2Var.laser_gn))
    {
        svoCalVar.calOfsDoneBits = vref_end;
        svoCalVar.calGainDoneBits = 0;
        svoCalVar.calOfsDoneBits = 0;
        return(SEQ_EX3);
    }
    else
    {
        return(ret_seq);
    }
}


/*********************************************************
* Module: state_STM_start()
*
* Description: Start Judge CD/DVD/BD
*
* Returns: SHORT
* Params: void
**********************************************************/
seq_exit_t state_STM_start(void)
{
    BYTE i;

    SendMsg80(SHOW_DEBUG_MSG,0x33FF05); // >>> STATE_stm_start <<<

#if(SET_SLOT_IN == 1) //[Q36]
    if ((ipOpenSW == 0) && (ipSW2 == 0) && (ipCloseSW == 1)) //[X35]NoDisc disable laser diode
    {
     return(SEQ_EX3);
    }
#endif	

#if (EN_POWER_CONTROL ==0)
    vWakeUpFromPowerSaveMode();
#endif

    HAL_DFE_EnDisBlankDetection(halDFE_BlankDetectDisable);     /* Disable blank detection */
    /* Disable black and white defect */
    /* Do not use DC filter for focus/tracking error signal when detecting defects */
    Defect_Enable(DEFECT_STATE_ALL_OFF);

#if (DVD_RAM_READ == 1)
    DISABLE_HDR_HOLD_SERVO;
#endif

    cal_retry = 0;
    fepp_wrong_layer_chkcnt = 0;

    for (i = STM_BD; i <= STM_CD; i++)
    {
        svoSTMVar.STM_maxFEpp[i] = 0;
        svoSTMVar.STM_maxSRFO[i] = 0;
        svoSTMVar.LayerFoo[i] = 0;
        svoSTMVar.STM_LayerNumber[1] = eLayersUnknown;
    }
    svoSTMVar.STM_BypassRamp = 0;

#if DDisc ==1
    DualDisc=0;
#endif

    if ((svoCalVar.discChkDoneBits & stm_chk_end) == stm_chk_end)
    {
        SendMsg80(SHOW_DEBUG_MSG,0x31001E); //CDDVD_iden_Ready
        SendMsg80(DEBUG_CALIB_ENTRYS_MSG,0x3A0002); //==>Calib SKIP!!
        WRITE_FIELD(HAL_SRV_PDMOENB,0x0000); //enable PDM port , it will be turnoff in power down mode

        if (svoVar_discDiameter == eNoDisc)
        {
            //vPowerSaveInNoDiscMode();
        }
        else
        {
            afeSetTrackingMethod(getTrackingMethod(svoVar_discKindT));
            switch (svoVar_discKindT.Bit.DiscStd)
            {
                case eDiscDVD:
                    spdChangeDiscSpeed(SVO_CAL_SPEED_DVD);
                    break;

                case eDiscCD:
                    spdChangeDiscSpeed(SVO_CAL_SPEED_CD);
                    break;
#if (BD_ENABLE == 1)
                case eDiscBD:
                    spdChangeDiscSpeed(SVO_CAL_SPEED_BD);
                    break;
#endif // (BD_ENABLE == 1)
#if (ENABLE_HDDVD == 1)
                case eDiscHD:
                    spdChangeDiscSpeed(SVO_CAL_SPEED_HD);
                    /* TDK OPU Dxx811 triple read */
                    break;
#endif // (ENABLE_HDDVD == 1)
            }
        }
        return(SEQ_EX2);
    }
#if (AFE_POWER_DOWN == 1)
    AFEPowerOn();
#endif // (AFE_POWER_DOWN == 1)

#if (DEBUG_OUTPUT_FULL_RANGE_FOO_TRO == 1)//This code is used to check FOC/RAD output (mV/dac)
    {
        SHORT dac;

        dac = 512;
        GIO0(1);
        WRITE_REG(HAL_SRVREG_MAP->FOODAT,dac);
        WRITE_REG(HAL_SRVREG_MAP->TRODAT,dac);
    #if FOOTILT_DIFFERENTIAL == 0
        HAL_SERVO_SetTilt(dac);
    #endif
        WRITE_FIELD(HAL_SRV_TRODACSW,1);
        WRITE_FIELD(HAL_SRV_FOODACSW,1);

        for (dac = 512; dac >= 0; dac--)
        {
            WRITE_REG(HAL_SRVREG_MAP->FOODAT,dac);
            WRITE_REG(HAL_SRVREG_MAP->TRODAT,dac);
    #if FOOTILT_DIFFERENTIAL == 0
            HAL_SERVO_SetTilt(dac);
    #endif
            Delay100uS(10);
        }
        GIO0(0);
        for (dac = 0; dac <= 256; dac++)
        {
            WRITE_REG(HAL_SRVREG_MAP->FOODAT,dac);
            WRITE_REG(HAL_SRVREG_MAP->TRODAT,dac);
    #if FOOTILT_DIFFERENTIAL == 0
            HAL_SERVO_SetTilt(dac);
    #endif
            Delay100uS(10);
        }
        GIO0(1);
        DelaymS(100);
        GIO0(0);
        for (dac = 257; dac <= 768; dac++)
        {
            WRITE_REG(HAL_SRVREG_MAP->FOODAT,dac);
            WRITE_REG(HAL_SRVREG_MAP->TRODAT,dac);
    #if FOOTILT_DIFFERENTIAL == 0
            HAL_SERVO_SetTilt(dac);
    #endif
            Delay100uS(10);
        }
        GIO0(1);
        DelaymS(100);
        GIO0(0);
        for (dac = 769; dac <= 1023; dac++)
        {
            WRITE_REG(HAL_SRVREG_MAP->FOODAT,dac);
            WRITE_REG(HAL_SRVREG_MAP->TRODAT,dac);
    #if FOOTILT_DIFFERENTIAL == 0
            HAL_SERVO_SetTilt(dac);
    #endif
            Delay100uS(10);
        }
        GIO0(1);
        for (dac = 1023; dac >= 512; dac--)
        {
            WRITE_REG(HAL_SRVREG_MAP->FOODAT,dac);
            WRITE_REG(HAL_SRVREG_MAP->TRODAT,dac);
    #if FOOTILT_DIFFERENTIAL == 0
            HAL_SERVO_SetTilt(dac);
    #endif
            Delay100uS(10);
        }

        WRITE_FIELD(HAL_SRV_TRODACSW,0);
        WRITE_FIELD(HAL_SRV_FOODACSW,0);
        GIO0(0);
    }
#endif /* DEBUG_OUTPUT_FULL_RANGE_FOO_TRO */
#if 0 //output full range step dac, help to determine HW gain setting and SLED_DZ
    {
        SHORT dac;

        for (dac = -511;dac <=511; dac++)
        {
            GIO0(dac & 0x01);
            svoStp_OutputDac(dac, dac);
    #if COLLI_SWITCH == 1
            Colli_OutputDac(dac, dac);
    #endif
            Delay100uS(10);
        }
        GIO0(0);
        svoStp_OutputDac(0, 0);
    #if COLLI_SWITCH == 1
        Colli_OutputDac(0, 0);
    #endif
    }
#endif
    svoVar_discKindT.fDiscKind = eUnknownKind;

    return(SEQ_EX0);
}

/*********************************************************
* Module: state_STM_measure()
*
* Description: Judge CD/DVD/BD
*
* Returns: SHORT
* Params: void
**********************************************************/
#define STM_TUNING                      0       // 0: normal FW
                                                // (1~255): do STM with different lasers STM_TUNING times, print the FE,SRFO result of the 3 lasers
#if (MONITOR_FMD_LEVEL==1)&&((PCB == 710)||(PCB == 711)||(PCB == 817))
#define FMD_PP_ABNORMAL_LEVEL       1000//1000mV. This should be defined by factory.
#define FMD_DC_ABNORMAL_LEVEL       1000//1000mV. This should be defined by factory.
#endif
seq_exit_t state_STM_measure(void)
{
    USHORT     diskIdenDownTime, diskIdenUpTime;
    USHORT     FEp2p,BSp2p;//from720
    USHORT     foo_sensitivity;
    errcode_t  ret;
    seq_exit_t ret_seq;
    USHORT     stm_cd_th, stm_dvd_th, stm_no_disc_th;
    discKindT tempDiscKind;
    USHORT     maxFEpp,maxSRFO,maxBS;
    BYTE       temp_byte;
#if (DDisc==1)
    float FESRFO_ratio_CD,FESRFO_ratio_DVD;
  #if (PCB == 815)
    float FESRFO_ratio_DVDL1;
  #endif
#endif

#if (PCB == 691)
    SHORT FPDIN, FPDIP,FPDO;
#endif // (PCB == 691)
#if (MONITOR_FMD_LEVEL==1)&&((PCB == 710)||(PCB == 711)||(PCB == 817))
    ULONG vrpowsmptimer;
    SHORT vrpow,vrpowmax,vrpowmin,vrpowdc;
#endif

    FEp2p = BSp2p = 0;
    stm_cd_th      = COEF_GetValue(AFE_CALC_STM_CD_TH);
    stm_dvd_th     = COEF_GetValue(AFE_CALC_STM_DVD_TH);
    stm_no_disc_th = COEF_GetValue(AFE_CALC_STM_NO_DISC_TH);
    tempDiscKind.fDiscKind = eUnknownKind;
    svoVar_discKindT.fDiscKind = eUnknownKind;
    svoRampMeasVar.STM = 0;

    #if (Customer_A1 ==1)//[W.0R]
	  if((MediaRecovery == 0x10)&&(ARG1== eForceDVD)&&(CompleteStrokeCount==2))
		{
				MediaRecovery = 0;
				return SEQ_EX1;
		}
	  #endif


#if (DISC_MIS_JUDGE_START_UP_RECOVERY == 1)
    switch (RetryDiscLaser)
    {
        case eDiscBD: // Only retry BD laser
            if (ARG1 != eForceBD)
            {
                return SEQ_EX1;
            }
            break;

        case eDiscDVD: // Only retry DVD laser
            if (ARG1 != eForceDVD)
            {
                return SEQ_EX1;
            }
            break;

        case eDiscCD: // Only retry CD laser
            if (ARG1 != eForceCD)
            {
                return SEQ_EX1;
            }
            break;
    }
#endif // #if (DISC_MIS_JUDGE_START_UP_RECOVERY == 1)

#ifdef STM_DEBUG_MSG
    #if (CHIP_REV <= 0xB3)
    AfeMUXOUT(AFE_MUXOUT1, halAFE_MUX1SEL_SRFO);
    AfeMUXOUT(AFE_MUXOUT2, halAFE_MUX2SEL_FEO);
    #else
    AfeMUXOUT(AFE_MUXOUT1, halAFE_MUX1SEL_SRFO, 0);
    AfeMUXOUT(AFE_MUXOUT2, halAFE_MUX2SEL_FEO, 0);
    #endif
#endif

#if (FocusLockAid_Enable == 1)
    HAL_DSP_FocusLockAid(OFF, UP);  //disable focus lock aid during STM
    HAL_SERVO_SetFSL(0x7F, 0x7F);
#endif // (FocusLockAid_Enable == 1)

    HAL_LASC_SetLaserOn(FALSE);

    /* select laser type and ramp up/down time */
    switch (ARG1)
    {
#if (BD_ENABLE == 1)
        case eForceBD:
            svoStepVar.StepHomePos = STEP_HOME_POSITION_BD;
            diskIdenDownTime = F_IDEN_DN_T_BD;
            diskIdenUpTime = F_IDEN_UP_T_BD;
            foo_sensitivity = F_IDEN_SENSITIVITY_BD;
            SendMsg80(DEBUG_DISK_ID_MSG,0x31008E); // STM BD LASER
            tempDiscKind.Bit.DiscStd = eDiscBD;  //Force BD Laser
            LAL_StoreParams(LAL_DetectDiscKind(tempDiscKind), SVO_CAL_SPEED_BD);
    #if (COLLI_SWITCH == 1)
        #if ((OPU == KEM350AAA) || (OPU == BDPL2))
            svoCalColliVar[0].opt_value = getInitColliPos(0, tempDiscKind.Bit.DiscStd);
            svoCalColliVar[1].opt_value = getInitColliPos(1, tempDiscKind.Bit.DiscStd);
        #endif // (OPU == KEM350AAA)
            if (Iram_colli_Initialized == STEP_UNINITIALIZED)
            {
                svoColli_Homing();
                while (svoStp_busy())
                {
                    ExitProcess();
                }
            }
            svoStp_JumpToPosition(COLLI_NORMAL_NO_REST_MODE, (calGetOptColliPos(0) + calGetOptColliPos(1)) / 2);
        #if (OPU == BDPL2)
            HAL_LASC_SetAPCTarget((calGetOptColliPos(0) + calGetOptColliPos(1)) / 2);
        #endif
    #endif //  COLLI_SWITCH ==1

    #if (OPU == KEM350AAA)
        #if (ND_FILTER_SW == 1)
            HAL_LASC_ActivateNDFilter(NDFILTERCLOSE);
        #endif // (ND_FILTER_SW == 1)
    #endif
    #if(PCB == 817)
            BD_SLEEP_DIS;
    #endif
            DVDLD_EN;
            CDLD_DIS;
            break;
#endif // (BD_ENABLE == 1)
#if (ENABLE_HDDVD == 1)
        case eForceHDDVD:
            svoStepVar.StepHomePos = STEP_HOME_POSITION_HD;
            diskIdenDownTime = F_IDEN_DN_T_HD;
            diskIdenUpTime = F_IDEN_UP_T_HD;
            foo_sensitivity = F_IDEN_SENSITIVITY_HD;
            SendMsg80(DEBUG_DISK_ID_MSG,0x3100B0); // STM HD LASER
            tempDiscKind.Bit.DiscStd = eDiscHD; //Force HD Laser
            LAL_StoreParams(LAL_DetectDiscKind(tempDiscKind), SVO_CAL_SPEED_HD);
    #if (LCD_SWITCH == 1)
            HAL_LASC_SetLCD();
    #endif
            break;
#endif // (ENABLE_HDDVD == 1)
        case eForceDVD:
            svoStepVar.StepHomePos = STEP_HOME_POSITION_DVD;
            diskIdenDownTime = F_IDEN_DN_T_DVD;
            diskIdenUpTime = F_IDEN_UP_T_DVD;
            foo_sensitivity = F_IDEN_SENSITIVITY_DVD;
            SendMsg80(DEBUG_DISK_ID_MSG,0x31008F); // STM DVD LASER
            tempDiscKind.Bit.DiscStd = eDiscDVD; //Force DVD Laser
            LAL_StoreParams(LAL_DetectDiscKind(tempDiscKind), SVO_CAL_SPEED_DVD);

#if (COLLI_SWITCH == 1)
    #if ((OPU == KEM350AAA) || (OPU == BDPL2))
            svoCalColliVar[0].opt_value = getInitColliPos(0, tempDiscKind.Bit.DiscStd);
            svoCalColliVar[1].opt_value = getInitColliPos(1, tempDiscKind.Bit.DiscStd);
            svoStp_JumpToPosition(COLLI_NORMAL_NO_REST_MODE,(calGetOptColliPos(0) + calGetOptColliPos(1)) / 2);
    #endif // (OPU == KEM350AAA)
#endif //  COLLI_SWITCH ==1

#if (LCD_SWITCH == 1)
            HAL_LASC_SetLCD();
#endif
    #if(PCB == 817)
            BD_SLEEP_EN;
    #endif
            DVDLD_EN;
            CDLD_DIS;
            break;

        case eForceCD:
        default:
            svoStepVar.StepHomePos = STEP_HOME_POSITION_CD;
            diskIdenDownTime = F_IDEN_DN_T_CD;
            diskIdenUpTime = F_IDEN_UP_T_CD;
            foo_sensitivity = F_IDEN_SENSITIVITY_CD;
            SendMsg80(DEBUG_DISK_ID_MSG,0x310090); // STM CD LASER
            tempDiscKind.Bit.DiscStd = eDiscCD; //Force CD Laser
            LAL_StoreParams(LAL_DetectDiscKind(tempDiscKind), SVO_CAL_SPEED_CD);
#if (COLLI_SWITCH == 1)
    #if ((OPU == KEM350AAA) || (OPU == BDPL2))
            svoCalColliVar[0].opt_value = getInitColliPos(0, tempDiscKind.Bit.DiscStd);
            svoCalColliVar[1].opt_value = getInitColliPos(1, tempDiscKind.Bit.DiscStd);
            svoStp_JumpToPosition(COLLI_NORMAL_NO_REST_MODE,calGetOptColliPos(0));
    #endif // (OPU == KEM350AAA)
#endif //  COLLI_SWITCH ==1

#if (LCD_SWITCH == 1)
            HAL_LASC_SetLCD();
#endif // LCD_SWITCH == 1
    #if(PCB == 817)
            BD_SLEEP_EN;
    #endif
            CDLD_EN;  //edwin
    #if(OPU == SFBD414)
            WaitTimer(10);
    #endif
            DVDLD_DIS;
            break;
    }

    /*move slep to start up position base on disc type we want to judge*/
#if (DOUBLE_LENS == 1)
    svoStp_JumpToPosition(STEP_SAFE_SLOW_MODE, svoStepVar.StepHomePos - (IS_OUTER_LENS(tempDiscKind.Bit.DiscStd)? STEP_LENS_OFFSET : 0));
#else
    svoStp_JumpToPosition(STEP_SAFE_SLOW_MODE, svoStepVar.StepHomePos);
#endif

    LAL_SetReadMode();
#if (CHIP_REV >= 0xC0)
    HAL_LASC_SetOscen(OSCEN_RWDISC);
#else
    HAL_LASC_SetOscen(OSCEN_ON);
#endif

#if (MONITOR_FMD_LEVEL==1)&&((PCB == 710)||(PCB == 711)||(PCB == 817))
    StartTimer(&vrpowsmptimer);
    vrpowmax = ReadAverageAdcmV(ADC_VRPOW, 0);
    vrpowmin = vrpowmax;
    do
    {
        vrpow=ReadAverageAdcmV(ADC_VRPOW, 0);
        if (vrpow>vrpowmax)
            vrpowmax = vrpow;
        if (vrpow<vrpowmin)
            vrpowmin = vrpow;
        //SendMsgCn(DEBUG_LASER_MSG,8,0x3165FA, A4B(vrpowmax),A4B(vrpowmin));// VRPOW_MAX = %4d, VRPOW_MIN = %4d
    }
    while (ReadTimer(&vrpowsmptimer)<100);
    vrpow = (vrpowmax-vrpowmin);
    vrpowdc = (vrpowmax+vrpowmin)/2;
#endif

#if ((PCB != 691) && (PCB != 1600))// && (PCB != 830))--AM
    LAL_LASC_LaserOffsetAdj();
#endif

    if (ARG1 == eForceBD)
    {
        FMTHROFFSET_BD = svoCalVar.fmThrData.all16;
#if (MONITOR_FMD_LEVEL==1)&&((PCB == 710)||(PCB == 711)||(PCB == 817))
        FMD_PPLVL_BD = vrpow;
        FMD_DCLVL_BD = vrpowdc;
#endif
    }
    else if (ARG1 == eForceDVD)
    {
        FMTHROFFSET_DVD = svoCalVar.fmThrData.all16;
#if (MONITOR_FMD_LEVEL==1)&&((PCB == 710)||(PCB == 711)||(PCB == 817))
        FMD_PPLVL_DVD = vrpow;
        FMD_DCLVL_DVD = vrpowdc;
#endif
    }
    else
    {
        FMTHROFFSET_CD = svoCalVar.fmThrData.all16;
#if (MONITOR_FMD_LEVEL==1)&&((PCB == 710)||(PCB == 711)||(PCB == 817))
        FMD_PPLVL_CD = vrpow;
        FMD_DCLVL_CD = vrpowdc;
#endif
    }

#if (MONITOR_FMD_LEVEL==1)&&((PCB == 710)||(PCB == 711)||(PCB == 817))
    // VRPOW_MAX = %4d, VRPOW_MIN = %4d
    SendMsgCnB(DEBUG_LASER_MSG, 8,0x3165FA, B4B(vrpowmax), B4B(vrpowmin));
    if (vrpow>FMD_PP_ABNORMAL_LEVEL)
    {
        SendMsg80(DEBUG_LASER_MSG,0x3165FB);    // ERROR: VRPOW PP Over Limit
    }
    else if (vrpowdc>FMD_DC_ABNORMAL_LEVEL)
    {
        SendMsg80(DEBUG_LASER_MSG,0x3165FC);    // ERROR: VRPOW DC Over Limit
    }
    if ((vrpow>FMD_PP_ABNORMAL_LEVEL)||(vrpowdc>FMD_DC_ABNORMAL_LEVEL))
        return(SEQ_EX1);
#endif

    HAL_LASC_SetLaserOn(TRUE);

#if (PCB == 691)
    FPDIP = FPDIN = FPDO = 0;
    //FPDO = DVDFPDIP - DVDFPDINVREF = S3 - S4 ; it would be negative .
    WRITE_FIELD(HAL_AFE_MUXADSEL, halAFE_MUXADSEL_MUXOUT3); //select MUXADSEL to MUXOUT3
    //DVDFPDIP (S3)
    WRITE_FIELD(HAL_AFE_APCTEST, halAFE_APCT_M1S4_M2S3); //S4 and S3
    AfeMUXOUT(AFE_MUXTOAD3, halAFE_MUX3ADSEL_APCMUX2);
    WaitTimer(5L);
    FPDIP = (SHORT)ReadAverageAdcmV(ADC_MUXOUT3, 6);
    send_msg74(DEBUG_KPOWER_MSG,0x5350BF,FPDIP);

    //DVDFPDINVREF (S4)
    WRITE_FIELD(HAL_AFE_APCTEST, halAFE_APCT_M1S4_M2S3); //S4 and S3
    AfeMUXOUT(AFE_MUXTOAD3, halAFE_MUX3ADSEL_APCMUX1);
    WaitTimer(5L);
    FPDIN = (SHORT)ReadAverageAdcmV(ADC_MUXOUT3, 6);
    send_msg74(DEBUG_KPOWER_MSG,0x5350C0,FPDIN);

    FPDO=FPDIN - FPDIP;
    if (FPDO < 0)
        FPDO= 0;
    send_msg74(DEBUG_KPOWER_MSG,0x535090,FPDO);
    if (FPDO > 575)
    {
        send_msg5S(1,"over limit 575");
    }
#endif // (PCB == 691)

    // ---------------------------------------------------;
    // Init Afe Registers                                 ;
    // ---------------------------------------------------;
    LAL_WriteCoef(AFE_MB11_MBGUPSEL_STM);
    LAL_WriteCoef(AFE_MB18_FEOPGN_STM);

    /*move lens to bottom and do offset calibration*/
    if (svoSTMVar.STM_BypassRamp == 0)
    {
        //---------------------------------;
        // move lens down                  ;
        //---------------------------------;
#if (FOOFFSET2_ENABLE == 1)
        WRITE_DSP_COEF(kf18,0);
        HAL_SERVO_SetFocDn(0);
#endif
#if (CHIP_REV >= 0xC0)
        ramp_to(-diskIdenDownTime * READ_DSP_COEF(kf18) * (FOC_RAMP_FACTOR / 0x20) / 0x20000);
#else
        ramp_FOOS(-diskIdenDownTime * (FOCUS_SEARCH_KF18 * FOC_RAMP_FACTOR / 0x20) / 0x20000);
#endif
        //wait colli and sledge ready
        while (svoStp_busy())
        {
            ExitProcess();
        }

        //wait few msec for lens to stable
        //WaitTimer(40);//from 720
        SetTimeout(0,40);
        while (timeout_chk(0) != eTimerTimeout)
        {
            ExitProcess();
        }

        // ---------------------------------------------------;
        // AFE offset Calibration                             ;
        // ---------------------------------------------------;
        ret = calCalibrateOffset(SRF_OS,                // adjusted signal
                                 TYPE2,                 // type of offset structure
                                 NORMAL_OFFSET,         // path selected
                                 BOT_SATURATE_LIM + 100,// target mv
                                 1,                     // polarity between offset & signal
                                 CALIB_FROM_CURRENT);
        calSetAfeOffset(0,SRF_OS,TYPE2,NORMAL_OFFSET,0,last_cal_result);

        ret = calCalibrateOffset(FEO_OS,                // adjusted signal
                                 TYPE2,                 // type of offset structure
                                 NORMAL_OFFSET,         // path selected
                                 vref_fm_mux3_mV,       // target mv
                                 1,                     // polarity between offset & signal
                                 CALIB_FROM_CURRENT);
        calSetAfeOffset(0,FEO_OS,TYPE2,NORMAL_OFFSET,0,last_cal_result);

        SendMsgCn(DEBUG_CALIB_RESULT_MSG,2,0x310502+Layer_Index,svoCalOfsT2Var[Layer_Index][SRF_OS].normal_offset,svoCalOfsT2Var[Layer_Index][FEO_OS].normal_offset);

#ifdef OPU_OUTPUT_MONITOR
        //HAL_LASC_SetLaserOn(TRUE);//TBD:need to check, should not be open again
        HAL_DSP_SetDebug(DEBUG_CH0, DEBUG_SOURCE_RGLUE, 0x33,0x08); //V_THRESH
        HAL_DSP_SetDebug(DEBUG_CH1, DEBUG_SOURCE_RGLUE, 0x34,0x08); //V_DELTA0
        HAL_DSP_SetDebug(DEBUG_CH2, DEBUG_SOURCE_RGLUE, 0x35,0x08); //V_DELTA1
        HAL_DSP_SetDebug(DEBUG_CH3, DEBUG_SOURCE_RGLUE, 0x36,0x08); //V_DELTA2
        DelaymS(1);
        if (ARG1 == eForceBD)
        {
            ADCVRPOW_BD = ReadAverageAdcmV(ADC_VRPOW, 6);
            VDELTAX_BD = (SHORT)READ_DSP_COEF(debug2);
            //SendMsgCn(SHOW_DEBUG_MSG,4,0xFFFF81,A2B(ADCVRPOW_BD),A2B(VDELTAX_BD));
            SendMsgCnB(SHOW_DEBUG_MSG,5,0xFFFF84,B2B(ADCVRPOW_BD),B2B(VDELTAX_BD),B1B(GetOpuTemperature()));
        }
        else if (ARG1 == eForceHDDVD)
        {
            //
        }
        else if (ARG1 == eForceDVD)
        {
            ADCVRPOW_DVD = ReadAverageAdcmV(ADC_VRPOW, 6);
            VDELTAX_DVD = (SHORT)READ_DSP_COEF(debug1);
            //SendMsgCn(SHOW_DEBUG_MSG,4,0xFFFF82,A2B(ADCVRPOW_DVD),A2B(VDELTAX_DVD));
            SendMsgCnB(SHOW_DEBUG_MSG,5,0xFFFF85,B2B(ADCVRPOW_DVD),B2B(VDELTAX_DVD),B1B(GetOpuTemperature()));
        }
        else
        {
            ADCVRPOW_CD = ReadAverageAdcmV(ADC_VRPOW, 6);
            VDELTAX_CD = (SHORT)READ_DSP_COEF(debug3);
            //SendMsgCn(SHOW_DEBUG_MSG,4,0xFFFF83,A2B(ADCVRPOW_CD),A2B(VDELTAX_CD));
            SendMsgCnB(SHOW_DEBUG_MSG,5,0xFFFF86,B2B(ADCVRPOW_CD),B2B(VDELTAX_CD),B1B(GetOpuTemperature()));
        }
#endif // #ifdef OPU_OUTPUT_MONITOR

        /*measure FE_FocMove_Reference*/
        svoPrvtVar_maxFEpp[0] = 0;
        svoPrvtVar_maxFEpp[1] = 0;
        svoRampMeasVar.refFE = 0;
        svoRampMeasVar.SRFO_gain = (READ_FIELD(HAL_AFE_MBGUPSEL) + READ_FIELD(HAL_AFE_SRFOBGA) - 32) * 10 + svoCalMisc2Var.pdic_gn + svoCalMisc2Var.laser_gn;

        for(temp_byte = 0; temp_byte < 32; temp_byte++)
        {
            WRITE_REG(HAL_SRVREG_MAP->RGSEL, halSRV_RSEL_FE);      /* read FE data */
            svoRampMeasVar.refFE += READ_REG(HAL_SRVREG_MAP->SRCR);
        }
        svoRampMeasVar.refFE = RoundDivide(svoRampMeasVar.refFE, temp_byte);
        // FE_FocMove_Reference = %2d
        SendMsgCn(DEBUG_CALIB_RESULT_MSG,2,0x31630C,A2B(svoRampMeasVar.refFE));

        /* measure SRFO, FE or FEO reference */

        WRITE_FIELD(HAL_AFE_BSPGN,halAFE_BSPGN_0dB);//0dB
        WRITE_FIELD(HAL_AFE_BSKG,0x10);//0dB
        WRITE_REG(HAL_AFEREG_MAP->BSRP05, HAL_AFE_BSSRF_MASK);//BS outputs SRFO

        WRITE_FIELD(HAL_AFE_MUX1ADSEL, halAFE_MUX1ADSEL_SRFO);
#if (STREAM_STM == 0)
    #if (FE_SLEW_RATE_LIMITER == 1)
        if (tempDiscKind.Bit.DiscStd == eDiscDVD)
        {
            WRITE_FIELD(HAL_AFE_MUX2ADSEL, halAFE_MUX2ADSEL_FE);
        }
        else
    #endif
        {
            WRITE_FIELD(HAL_AFE_MUX2ADSEL, halAFE_MUX2ADSEL_FEO);
        }
        svoRampMeasVar.refFEO  = ReadAverageAdcmV(ADC_MUXOUT2,7); //in mV
#else
        svoRampMeasVar.refFE = ReadAverageAdcmV(ADC_FE,7); //in mV
        svoRampMeasVar.refBS = ReadAverageAdcmV(ADC_BS,7); //in mV
#endif
        /* measure SRFO reference */
        svoRampMeasVar.refSRFO = ReadAverageAdcmV(ADC_MUXOUT1,7); //in mV
        // STM FE_ref = %2d (mV), SRFO_ref = %4d (mV)
        SendMsgCnB(DEBUG_DISK_ID_MSG,4,0x310098,B2B(svoRampMeasVar.refFEO),B2B(svoRampMeasVar.refSRFO));
        // transfer mv to DAC
#if (STREAM_STM == 0)
        svoRampMeasVar.refFEO  = (((svoRampMeasVar.refFEO  - GPADC_IN_MIN) << GPADC_BITS) + GPADC_IN_RANGE / 2) / GPADC_IN_RANGE ;
#else
        svoRampMeasVar.refFE   = (((svoRampMeasVar.refFE   - GPADC_IN_MIN) << GPADC_BITS) + GPADC_IN_RANGE / 2) / GPADC_IN_RANGE ;
        svoRampMeasVar.refBS   = (((svoRampMeasVar.refBS   - GPADC_IN_MIN) << GPADC_BITS) + GPADC_IN_RANGE / 2) / GPADC_IN_RANGE ;
#endif
        svoRampMeasVar.refSRFO = (((svoRampMeasVar.refSRFO - GPADC_IN_MIN) << GPADC_BITS) + GPADC_IN_RANGE / 2) / GPADC_IN_RANGE ;

        //--------------------------------------------------;
        // move lens up and measure peaks magnitude         ;
        //--------------------------------------------------;
#if (FOOFFSET2_ENABLE == 1)
        WRITE_REG(HAL_SRVREG_MAP->LDOFF,0x00);
        WRITE_REG(HAL_SRVREG_MAP->LDON, 0x00);
        WRITE_DSP_COEF(kf18,FOCUS_SEARCH_KF18);
#endif
        HAL_SERVO_SetFocUp(0);

#if (STREAM_STM == 0)
        RampMeas(eRampSTM, diskIdenUpTime, UP, tempDiscKind);
#else
        RampStream(eRampSTM,diskIdenUpTime, UP, tempDiscKind);
#endif

        // Move lens to initial position & hold lens at initial position
        ramp_stop();
    #if (CHIP_REV >= 0xC0)
        ramp_to(0);
    #else
        ramp_FOOS(0);
    #endif

        // FOODAC Pre = %2d, main = %2d, SRFOpp_Pre = %4d (mV)
        SendMsgCnB(DEBUG_DISK_ID_MSG,8,0x31000A,B2B(svoRampMeasVar.SubstFoo),B2B(svoRampMeasVar.LayerFoo),
                                                B4B((svoRampMeasVar.SubstSRFO * GPADC_IN_RANGE) >> GPADC_BITS));
    #if (STREAM_STM == 0)
        // STM_L0_pp: SRFO = %4d (mV), FE = %4d (mV), BS = %4d (mV)
        SendMsgCnB(DEBUG_DISK_ID_MSG,12,0x310086,B4B((svoRampMeasVar.SRFOpp[0] * GPADC_IN_RANGE) >> GPADC_BITS),
                                                 B4B((svoRampMeasVar.FEpp[0] * DSPADC_IN_RANGE) >> DSPADC_BITS),
                                                 B4B((svoRampMeasVar.BSpp[0] * DSPADC_IN_RANGE) >> DSPADC_BITS));

        // STM_L1_pp: SRFO = %4d (mV), FE = %4d (mV), BS = %4d (mV)
        SendMsgCnB(DEBUG_DISK_ID_MSG,12,0x310087,B4B((svoRampMeasVar.SRFOpp[1] * GPADC_IN_RANGE) >> GPADC_BITS),
                                                 B4B((svoRampMeasVar.FEpp[1] * DSPADC_IN_RANGE) >> DSPADC_BITS),
                                                 B4B((svoRampMeasVar.BSpp[1] * DSPADC_IN_RANGE) >> DSPADC_BITS));
    #else
        SendMsgCnB(DEBUG_DISK_ID_MSG,12,0x310086,B4B((svoRampMeasVar.SRFOpp[0] * GPADC_IN_RANGE) >> GPADC_BITS),
            B4B((svoRampMeasVar.FEpp[0] * GPADC_IN_RANGE) >> GPADC_BITS),B4B((svoRampMeasVar.BSpp[0] * GPADC_IN_RANGE) >> GPADC_BITS));
        SendMsgCnB(DEBUG_DISK_ID_MSG,12,0x310087,B4B((svoRampMeasVar.SRFOpp[1] * GPADC_IN_RANGE) >> GPADC_BITS),
            B4B((svoRampMeasVar.FEpp[1] * GPADC_IN_RANGE) >> GPADC_BITS),B4B((svoRampMeasVar.BSpp[1] * GPADC_IN_RANGE) >> GPADC_BITS));
    #endif
        // STM Layer0Timer = %4d, Layer1timer = %4d
        SendMsgCnB(DEBUG_DISK_ID_MSG,8,0x3100A0,B4B(svoRampMeasVar.LayerTime[0]),B4B(svoRampMeasVar.LayerTime[1]));

        if (svoRampMeasVar.SubstSRFO * svoRampMeasVar.SRFOpp[0] != 0)
        {
            svoRampMeasVar.STM = (svoRampMeasVar.LayerFoo - svoRampMeasVar.SubstFoo) * foo_sensitivity / 1000;
            SendMsgCn(DEBUG_DISK_ID_MSG,2,0x310089,A2B(svoRampMeasVar.STM));      // STM = %2d (um)
        }

        SendMsgCn(DEBUG_DISK_ID_MSG,2,0x310089,A2B(svoRampMeasVar.STM));      // STM = %2d (um)

        /* Take the biggest */
        maxFEpp = FEp2p = (svoRampMeasVar.FEpp[0] > svoRampMeasVar.FEpp[1])? svoRampMeasVar.FEpp[0] : svoRampMeasVar.FEpp[1];
#if (STREAM_STM == 0)
        maxFEpp = (maxFEpp * DSPADC_IN_RANGE) >> DSPADC_BITS;
#else
        maxFEpp = (maxFEpp * GPADC_IN_RANGE) >> GPADC_BITS;
#endif
        maxSRFO = (svoRampMeasVar.SRFOpp[0] > svoRampMeasVar.SRFOpp[1])? svoRampMeasVar.SRFOpp[0] : svoRampMeasVar.SRFOpp[1]; //ADC
        maxSRFO = (maxSRFO * GPADC_IN_RANGE) >> GPADC_BITS; //express in mV
        maxBS = BSp2p = (svoRampMeasVar.BSpp[0] > svoRampMeasVar.BSpp[1])? svoRampMeasVar.BSpp[0] : svoRampMeasVar.BSpp[1];
#if (STREAM_STM == 0)
        maxBS = (maxBS * DSPADC_IN_RANGE) >> DSPADC_BITS;
#else
        maxBS = (maxBS * GPADC_IN_RANGE) >> GPADC_BITS;
#endif

        //increase SRFO about 2dB to prevent abnormally low SRFO during STM cause signal saturation during FEpp measurement.
        svoPrvtVar_maxSRFO = maxSRFO * 5 / 4;

        SendMsgCn(DEBUG_DISK_ID_MSG,4,0x310088,A2B(svoRampMeasVar.STM),A2B(maxSRFO));
       // STM (max): SRFO = %d (mV), FE = %d (mV), BS = %d (mV)
        SendMsgCnB(DEBUG_DISK_ID_MSG,6,0x31009F,B2B(maxSRFO),B2B(maxFEpp),B2B(maxBS));

        switch (ARG1)
        {
            case eForceBD:
                svoSTMVar.STM[STM_BD] = svoRampMeasVar.STM;
                svoSTMVar.STM_maxFEpp[STM_BD] = maxFEpp;
                svoSTMVar.STM_maxSRFO[STM_BD] = maxSRFO;//from 720, original use svoPrvtVar_maxSRFO
                svoSTMVar.LayerFoo[STM_BD] = svoRampMeasVar.LayerFoo;
                svoSTMVar.STM_LayerNumber[STM_BD] = (svoRampMeasVar.LayerTime[1] != 0L)? eLayersDouble : eLayersSingle;
                break;

            case eForceDVD:
                svoSTMVar.STM[STM_DVD] = svoRampMeasVar.STM;
                svoSTMVar.STM_maxFEpp[STM_DVD] = maxFEpp;
                svoSTMVar.STM_maxSRFO[STM_DVD] = maxSRFO;//from 720, original use svoPrvtVar_maxSRFO
                svoSTMVar.LayerFoo[STM_DVD] = svoRampMeasVar.LayerFoo;
                svoSTMVar.STM_LayerNumber[STM_DVD] = ((svoRampMeasVar.LayerTime[1] != 0L) && ((svoRampMeasVar.LayerTime[1] - svoRampMeasVar.LayerTime[0]) < 10L))? eLayersDouble : eLayersSingle;
                break;

            case eForceCD:
                svoSTMVar.STM[STM_CD] = svoRampMeasVar.STM;
                svoSTMVar.STM_maxFEpp[STM_CD] = maxFEpp;
                svoSTMVar.STM_maxSRFO[STM_CD] = maxSRFO;//from 720, original use svoPrvtVar_maxSRFO
                svoSTMVar.LayerFoo[STM_CD] = svoRampMeasVar.LayerFoo;
                svoSTMVar.STM_LayerNumber[STM_CD] = eLayersSingle;
                break;
        }
    }
    else//svoSTMVar.STM_BypassRamp = 1
    {
        switch (ARG1)
        {
            case eForceBD:
                svoRampMeasVar.STM = svoSTMVar.STM[STM_BD];
                svoVar_discKindT.Bit.DiscStd = eDiscBD;
                maxFEpp = svoSTMVar.STM_maxFEpp[STM_BD];
                svoPrvtVar_maxSRFO = svoSTMVar.STM_maxSRFO[STM_BD];
                svoRampMeasVar.LayerFoo = svoSTMVar.LayerFoo[STM_BD];
                svoVar_discKindT.Bit.Layers = svoSTMVar.STM_LayerNumber[STM_BD];
                break;

            case eForceDVD:
                svoRampMeasVar.STM = svoSTMVar.STM[STM_DVD];
                svoVar_discKindT.Bit.DiscStd = eDiscDVD;
                maxFEpp = svoSTMVar.STM_maxFEpp[STM_DVD];
                svoPrvtVar_maxSRFO = svoSTMVar.STM_maxSRFO[STM_DVD];
                svoRampMeasVar.LayerFoo = svoSTMVar.LayerFoo[STM_DVD];
                svoVar_discKindT.Bit.Layers = svoSTMVar.STM_LayerNumber[STM_DVD];
                break;

            case eForceCD:
                svoRampMeasVar.STM = svoSTMVar.STM[STM_CD];
                svoVar_discKindT.Bit.DiscStd = eDiscCD;
                maxFEpp = svoSTMVar.STM_maxFEpp[STM_CD];
                svoPrvtVar_maxSRFO = svoSTMVar.STM_maxSRFO[STM_CD];
                svoRampMeasVar.LayerFoo = svoSTMVar.LayerFoo[STM_CD];
                svoVar_discKindT.Bit.Layers = svoSTMVar.STM_LayerNumber[STM_CD];
                break;
        }
        svoRampMeasVar.SRFO_gain = (READ_FIELD(HAL_AFE_MBGUPSEL) + READ_FIELD(HAL_AFE_SRFOBGA) - 32) * 10 + svoCalMisc2Var.pdic_gn + svoCalMisc2Var.laser_gn;
    }//svoSTMVar.STM_BypassRamp = 1

#if (DISC_MIS_JUDGE_START_UP_RECOVERY == 1)
    //ignore STM result if RetryDiscLaser set to one media
    switch (RetryDiscLaser)
    {
        case eDiscCD:
            svoRampMeasVar.STM = stm_cd_th;
            break;
        case eDiscDVD:
            svoRampMeasVar.STM = stm_dvd_th;
            break;
        case eDiscBD:
            svoRampMeasVar.STM = stm_dvd_th - 1;
            break;
    }
#endif

    LAL_WriteCoef(AFE_BSRP07_BSPGN);
    LAL_WriteCoef(AFE_BSRP25_BSKG);
    LAL_WriteCoef(AFE_BSRP05);
    LAL_WriteCoef(AFE_MB18_FEOPGN);

    #if (STM_TUNING > 0)
    if (1)
    {
        ret_seq = SEQ_EX1;
    }
    else
    #endif
    if (svoSTMVar.STM_BypassRamp == 1)
    {
        ret_seq = SEQ_EX0;
    }
#if (FORCE_DISK_KIND == 0)
    else if ((maxSRFO == 0) || (maxFEpp == 0))
    {
        ret_seq = SEQ_EX1;
    }
#endif
    else if (ARG1 == eForceBD)
    {
#if (FORCE_DISK_KIND == 1)
        if (svoVar_discKindT.Bit.DiscStd == eDiscBD)
            ret_seq = SEQ_EX0;
        else
            ret_seq = SEQ_EX1;
#else // (FORCE_DISK_KIND==1)
        if ((svoRampMeasVar.STM < stm_dvd_th) && (svoRampMeasVar.STM > stm_no_disc_th) &&
            ((maxFEpp > MIN_STM_FEpp_BD) || (maxFEpp > (maxSRFO * MIN_STM_FESRFO_RATIO_BD))))
        {
            svoVar_discKindT.Bit.DiscStd = eDiscBD;
            svoVar_discKindT.Bit.Layers = svoSTMVar.STM_LayerNumber[STM_BD];

            if (svoVar_discKindT.Bit.Layers == eLayersDouble)
            {   // BD Dual Layer
                SendMsg80(SHOW_DEBUG_MSG,0x3100A2); // STM==>BD DL Disc
            }
            else
            {   // BD Single Layer
                SendMsg80(SHOW_DEBUG_MSG,0x3100A3); // STM==>BD SL Disc
            }
            ret_seq = SEQ_EX0;
        }
        else
        {
#if 0 //TBD:need check ,this function still has problem
            if ((svoRampMeasVar.STM <= stm_no_disc_th) && (svoStmSubMissRty < STM_BD_PROTECTION_RETRY) &&
                ((maxFEpp > MIN_STM_FEpp_BD) || ((maxFEpp > (maxSRFO * MIN_STM_FESRFO_RATIO_BD) && (maxFEpp > MIN_STM_FEpp_BD / 2)))))
            {//Miss substrate
                svoStmSubMissRty++;
                SendMsg80(SHOW_DEBUG_MSG,0x3100EF);         // STM ==> Retry DVD LASER again (Miss substrate)
                return (SEQ_BUSY);
            }
            else
#endif
            {
                ret_seq = SEQ_EX1;
            }
        }
#endif // (FORCE_DISK_KIND==1)
    }
    else if (ARG1 == eForceHDDVD)
    {
#if (FORCE_DISK_KIND == 1)
        if (svoVar_discKindT.Bit.DiscStd == eDiscHD)
            ret_seq = SEQ_EX0;
        else
            ret_seq = SEQ_EX1;
#else // (FORCE_DISK_KIND == 1)
        if (svoVar_discKindT.Bit.DiscStd == eDiscHD)
        {
            if (svoRampMeasVar.LayerTime[1] != 0L)
            {
                svoVar_discKindT.Bit.Layers = eLayersDouble;
                SendMsg80(SHOW_DEBUG_MSG,0x3100B1);     // STM ==> HD DL Disc
            }
            else
            {
                svoVar_discKindT.Bit.Layers = eLayersSingle;
                SendMsg80(SHOW_DEBUG_MSG,0x3100B2);     // STM ==> HD SL Disc
            }
            ret_seq = SEQ_EX0;
        }
        else
        {
            /* now STM can not identify HD disc so return NG directlly*/
            ret_seq = SEQ_EX1;
        }
#endif // (FORCE_DISK_KIND == 1)
    }
    else if (ARG1 == eForceDVD)
    {
#if (FORCE_DISK_KIND == 1)
        if (svoVar_discKindT.Bit.DiscStd == eDiscDVD)
            ret_seq = SEQ_EX0;
        else
            ret_seq = SEQ_EX1;
#else // #if (FORCE_DISK_KIND == 1)
        if ((svoRampMeasVar.STM >= stm_dvd_th) && (svoRampMeasVar.STM < stm_cd_th) &&
            ((maxFEpp > MIN_STM_FEpp_DVD) ||
            ((maxFEpp > (maxSRFO * MIN_STM_FESRFO_RATIO_DVD) && (maxFEpp > MIN_STM_FEpp_DVD / 2)))))
        {
            svoVar_discKindT.Bit.DiscStd = eDiscDVD;
            svoVar_discKindT.Bit.Layers = svoSTMVar.STM_LayerNumber[STM_DVD];

            if (svoVar_discKindT.Bit.Layers == eLayersDouble)
            {   // DVD Dual Layer
    #if ((DDisc == 1)&&(PCB==815))
                //SendMsgCn(DEBUG_DISK_ID_MSG,4,0x310088,A2B(svoRampMeasVar.FEpp[0]),A2B(svoRampMeasVar.SRFOpp[0]));
                //SendMsgCn(DEBUG_DISK_ID_MSG,4,0x310088,A2B(svoRampMeasVar.FEpp[1]),A2B(svoRampMeasVar.SRFOpp[1]));
                FESRFO_ratio_DVD = (svoRampMeasVar.FEpp[0] * 100) / svoRampMeasVar.SRFOpp[0];
                FESRFO_ratio_DVDL1 = (svoRampMeasVar.FEpp[1] * 100) / svoRampMeasVar.SRFOpp[0];
                //send_msgA2(1,0x51009A,FESRFO_ratio_DVD);
                //send_msgA2(1,0x51009B,FESRFO_ratio_DVDL1);
                //send_msg74(1,0x51009B,(FESRFO_ratio_DVDL1/FESRFO_ratio_DVD));

                if (((DualDisc == 0) || (DualDisc == 4))&& (svoRampMeasVar.STM > DualDisc_STM_DVDLD_TH)
                                                        && (FESRFO_ratio_DVDL1 > (FESRFO_ratio_DVD*FESRFO_ratio_TH*2)))
                {
                    if (DualDisc == 0)
                        DualDisc = 1;
                    FESRFO_ratio_DVD = (maxFEpp * 100) / maxSRFO;
                    SendMsg80(SHOW_DEBUG_MSG,0x320A08);
                    ret_seq = SEQ_EX1;//SEQ_EX1 need use CD LD to ID Disc
                }
                else
    #endif
                {
                    SendMsg80(SHOW_DEBUG_MSG,0x310091); // STM==>DVD DL Disc
                    ret_seq = SEQ_EX0;
                }
            }
    #if (SACD == 1)
            else if ((svoRampMeasVar.LayerTime[1] != 0L) && ((svoRampMeasVar.LayerTime[1] - svoRampMeasVar.LayerTime[0]) < (svoRampMeasVar.LayerTime[0] + 10)) &&
                     ((svoRampMeasVar.LayerTime[1] - svoRampMeasVar.LayerTime[0]) > (svoRampMeasVar.LayerTime[0] - 10)) &&
                     (svoRampMeasVar.SRFOpp[1] > svoRampMeasVar.SRFOpp[0] / 2))
            {
                // Hybrid SACD
                SendMsg80(SHOW_DEBUG_MSG,0x3100A4);// STM==>SACD Disc

        #if (DISC_MIS_JUDGE_START_UP_RECOVERY == 1)
                if (RetryDiscLaser == eDiscDVD)
                    RetryDiscLaser = eDiscCD;
        #endif
                ret_seq = SEQ_EX1;  //SEQ_EX1 need use CD LD to ID Disc
            }
    #endif
            else
            {
                // DVD Single Layer
    #if DDisc == 1
                if (((DualDisc == 0) || (DualDisc == 4))&&(svoRampMeasVar.STM > DualDisc_STM_DVDLD_TH)
                    && (FEp2p != 0) && (BSp2p != 0))
                {
                    if (DualDisc == 0)
                        DualDisc = 1;
                    FESRFO_ratio_DVD = (maxFEpp * 100) / maxSRFO;
                    SendMsg80(SHOW_DEBUG_MSG,0x320A08);
                    ret_seq = SEQ_EX1;//SEQ_EX1 need use CD LD to ID Disc
                }
                else
    #endif // DDisc == 1
                {
    #if DDisc == 1
                    DualDisc = 0;
    #endif // DDisc == 1
                    SendMsg80(SHOW_DEBUG_MSG,0x310092); // STM==>DVD SL Disc
                    ret_seq = SEQ_EX0;
                }
            }
        }
        else
        {
#if 0 //TBD:need check ,this function still has problem
            if ((svoRampMeasVar.STM <= stm_no_disc_th) && (svoStmSubMissRty < STM_DVD_PROTECTION_RETRY) &&
                ((maxFEpp > MIN_STM_FEpp_DVD) || ((maxFEpp > (maxSRFO * MIN_STM_FESRFO_RATIO_DVD) && (maxFEpp > MIN_STM_FEpp_DVD / 2)))))
            {//Miss substrate
                svoStmSubMissRty++;
                SendMsg80(SHOW_DEBUG_MSG,0x310097);         // STM ==> Retry DVD LASER again (Miss substrate)
                return (SEQ_BUSY);
            }
            else
#endif
            {
    #if((DDisc == 1)&&(PCB==815))
                if (((DualDisc == 0) || (DualDisc == 4)) && (svoRampMeasVar.STM < stm_cd_th*1.1))
                {
                    if (DualDisc == 0)
                        DualDisc = 1;
                    FESRFO_ratio_DVD = (maxFEpp * 100) / maxSRFO;
                    SendMsg80(SHOW_DEBUG_MSG,0x320A08);
                }
    #endif
            ret_seq = SEQ_EX1;
            }
        }
#endif // (FORCE_DISK_KIND == 1)
    }
    else
    {
#if (FORCE_DISK_KIND == 1)
        if (svoVar_discKindT.Bit.DiscStd == eDiscCD)
            ret_seq = SEQ_EX0;
        else
            ret_seq = SEQ_EX1;
#else // (FORCE_DISK_KIND == 1)
        /* No reflection */
#if 0 //TBD:need check ,this function still has problem
        if ((svoRampMeasVar.STM <= stm_no_disc_th) && (svoStmSubMissRty < STM_CD_PROTECTION_RETRY) &&
            ((maxFEpp > MIN_STM_FEpp_CD) || (maxFEpp > (maxSRFO * MIN_STM_FESRFO_RATIO_CD))))
        {//Miss substrate
            svoStmSubMissRty++;
            SendMsg80(SHOW_DEBUG_MSG,0x310096);         // STM ==> Retry CD LASER again (Miss substrate)
            return (SEQ_BUSY);
        }
#endif
        if ((svoRampMeasVar.STM >= stm_cd_th) &&
            ((maxFEpp > MIN_STM_FEpp_CD) || (maxFEpp > (maxSRFO * MIN_STM_FESRFO_RATIO_CD))))
        {
            svoVar_discKindT.Bit.DiscStd = eDiscCD;
            svoVar_discKindT.Bit.Layers = eLayersSingle;
            SendMsg80(SHOW_DEBUG_MSG,0x310094);// STM==>CD DISC
            ret_seq = SEQ_EX0;
        }
    #if DDisc == 1
        else if ((svoRampMeasVar.STM > stm_no_disc_th) && ((DualDisc == 1) || (DualDisc == 4)))
        {
            if ((svoRampMeasVar.STM < (stm_no_disc_th * 4)) && (DualDisc == 1))
                DualDisc=4;
            else
                DualDisc=2;

            if ((FEp2p !=0) && (BSp2p!=0))
            {
                FESRFO_ratio_CD = (maxFEpp * 100) / maxSRFO;
                //send_msg5S(SHOW_DEBUG_MSG,"FEBS ratio DVD CD");
                //send_msgA2(1,0x51009A,FESRFO_ratio_DVD);
                //send_msgA2(1,0x51009B,FESRFO_ratio_CD);
                //send_msg74(1,0x51009B,(FESRFO_ratio_CD/FESRFO_ratio_DVD));
                if ((svoRampMeasVar.STM > DualDisc_STM_CDLD_TH)
                    ||((svoRampMeasVar.STM > (DualDisc_STM_CDLD_TH*0.97))&&(FESRFO_ratio_CD > FESRFO_ratio_DVD * FESRFO_ratio_TH))
                  ||((svoRampMeasVar.LayerTime[1]!=0)&&(svoRampMeasVar.STM > (DualDisc_STM_CDLD_TH*0.9)) && (FESRFO_ratio_CD > FESRFO_ratio_DVD * FESRFO_ratio_TH)))
                {
                    svoVar_discKindT.Bit.DiscStd = eDiscCD;
                    svoVar_discKindT.Bit.Layers = eLayersSingle;
                    DualDisc = 3;
                    SendMsg80(SHOW_DEBUG_MSG,0x320A07); // Dual Disc
                    ret_seq = SEQ_EX0;
                }
                else
                {
                    svoSTMVar.STM_BypassRamp = 1;
                    RetryDiscLaser = eDiscDVD;
                    ret_seq = SEQ_EX4;
                }
            }
            else
            {
                svoSTMVar.STM_BypassRamp = 1;
                RetryDiscLaser = eDiscDVD;
                ret_seq = SEQ_EX4;
            }
        }
    #endif // DDisc == 1
        else
        {
            svoVar_discKindT.fDiscKind = eUnknownKind;
            SendMsg80(SHOW_DEBUG_MSG,0x310095);         // STM ==> UNKNOWN
            ret_seq = SEQ_EX1;
        }
#endif // (FORCE_DISK_KIND == 1)
    }

    svoStmSubMissRty = 0;

    svoSetLDOff();

    if (ret_seq == SEQ_EX0) /* Disc judge OK */
    {
        calSetMisc(0,FOO_OS,svoRampMeasVar.LayerFoo);

#if 0 //retain this code if customer needed
        // for PCBA test after assembly at production line before power calibration is done,
        // force disc as DVD single layer to do PCBA test
        if ((svoPcalVar.CalPowerDoneBits & dv_kpower_end) != dv_kpower_end)
        {
            svoVar_discKindT.Bit.DiscStd = eDiscDVD;
            svoVar_discKindT.Bit.Layers = eLayersSingle;
        }
#endif

#if (DISC_MIS_JUDGE_RECOVERY == 1)
        svoMiscVar.retryMediaType = (1<<eMediaROM) | (1<<eMediaRW);
#endif

        SendMsg80(SHOW_DEBUG_MSG,0x3100B3);             // STM OK
    }
    else /* Disc judge NG or No Disc */
    {
#if((OPU == HOPB1300)||(OPU == SFBD412)||(OPU == SFBD414))
        WRITE_DSP_COEF(Thresh_open_loop_value,0x0000);
        WRITE_DSP_COEF(Thresh_reference,0x0000);
        //force CD
        WRITE_DSP_COEF(Att2_0,0x0000);
        WRITE_DSP_COEF(Att2_1,0x0000);
        WRITE_DSP_COEF(Offset2_0,0x0000);
        WRITE_DSP_COEF(Offset2_1,0x0000);
        //force DVD
        WRITE_DSP_COEF(Att0_0,0x0000);
        WRITE_DSP_COEF(Att0_1,0x0000);
        WRITE_DSP_COEF(Offset0_0,0x0000);
        WRITE_DSP_COEF(Offset0_1,0x0000);
        //force BD
        WRITE_DSP_COEF(Att1_0,0x0000);
        WRITE_DSP_COEF(Att1_1,0x0000);
        WRITE_DSP_COEF(Offset1_0,0x0000);
        WRITE_DSP_COEF(Offset1_1,0x0000);

        LAL_LASC_ClearLaserOutputs(); //Clear the output of all channel
#endif
        if (ret_seq == SEQ_EX1)
        {
            SendMsg80(SHOW_DEBUG_MSG,0x31008A); //STM NG! Try next laser

#ifdef STM_DEBUG_MSG
            if (ARG1 == eForceDVD)
            {
                PB.DATA.BIT.B7 = 1; // use windows to trigger fail case
                DelaymS(200);
                PB.DATA.BIT.B7 = 0;
                send_msg5SValue(1, "DVDSTMF", 2, svoRampMeasVar.SRFOpp[0]);
            }
#endif
        }
        else
        {
            SendMsg80(SHOW_DEBUG_MSG,0x31008B); //STM No reflection or Unknown!
        }
    }
    return(ret_seq);
}

/*********************************************************
* Module: state_STM_end()
*
* Description: Judge CD/DVD/BD
*
* Returns: SHORT
* Params: void
**********************************************************/
//#define QUICK_KICK_TUNING  //will ignore STM result so RD can check quick kick function
seq_exit_t state_STM_end(void)
{
#if (NODISC_RETRY == 1)
    USHORT    pre_fgCount;
    SHORT     delta_rpm_100ms;
    #endif
    //SHORT dmfbk_val;
#ifdef SLEDGE_JUMP_ADJ
    BYTE      temp;

    for (temp = 0; temp < 10; temp++)
    {
        addr_diff[temp] = 0;
    }
    servo_debug[1] = SLEDGE_JUMP_ADJ_MIN;
    servo_debug[4] = 0;
    test_cnt = 0;
#endif
#ifndef QUICK_KICK_TUNING
    if (svoVar_discKindT.fDiscKind == eUnknownKind)
#endif
    {
#if (PCB == 710)||(PCB == 711)
        //DVD/CD photo diode sleep mode
        SW_INPUT();
        //BD photo diode sleep mode
        SWB1(1);
        SWB2(1);
#elif (PCB == 817)
        //DVD/CD photo diode sleep mode
        DVDCD_STB_STB;
        //BD photo diode sleep mode
        BD_SLEEP_EN;
#endif
#if (NODISC_RETRY == 1)
        if (++cal_retry > STM_MAX_RETRIES)
        {
            if ((RetryDiscLaser == eDiscUnknown) && ((svoSTMVar.STM_maxFEpp[STM_BD] > 200) || (svoSTMVar.STM_maxFEpp[STM_DVD] > 200) || (svoSTMVar.STM_maxFEpp[STM_CD] > 200)))
            {
                svoSTMVar.STM_BypassRamp = 1;
                if ((svoSTMVar.STM_maxFEpp[STM_BD]>=svoSTMVar.STM_maxFEpp[STM_DVD])&&(svoSTMVar.STM_maxFEpp[STM_BD]>=svoSTMVar.STM_maxFEpp[STM_CD]))
                    RetryDiscLaser = eDiscBD;
                else if ((svoSTMVar.STM_maxFEpp[STM_DVD]>=svoSTMVar.STM_maxFEpp[STM_BD])&&(svoSTMVar.STM_maxFEpp[STM_DVD]>=svoSTMVar.STM_maxFEpp[STM_CD]))
                    RetryDiscLaser = eDiscDVD;
                else
                    RetryDiscLaser = eDiscCD;
                return(SEQ_EX2);
            }

    #if (ALWAYS_HOMING == 0)
            Iram_stp_Initialized = STEP_UNINITIALIZED;
    #endif // (ALWAYS_HOMING == 1)
    #if (COLLI_SWITCH == 1) && (COLLI_ALWAYS_HOMING == 0)
            Iram_colli_Initialized = STEP_UNINITIALIZED;
    #endif
            cal_retry = 0;
            return SEQ_EX1;
        }

        SendMsg80(SHOW_DEBUG_MSG,0x310093);//STM==>NO DISC

        //OUICKKICK start
        WRITE_DSP_COEF(kdf14,DISC_KICK_FORCE);
        WRITE_DSP_COEF(dmfbk_level, (short)DISC_KICK_LEVEL*DMODAC_FACTOR);
        WRITE_REG(HAL_SRVREG_MAP->DMFBK, HAL_SRV_DMFK);

        //Quick kick to detect no disc & change disc angle for STM retry
        delta_rpm_100ms = SvoDiameterJudge(0);

        if (delta_rpm_100ms == 0x8000)
        {
            delta_rpm_100ms = 0;
        }

        if (delta_rpm_100ms >= (SHORT)(NO_DISC_TH * 100))
        {
            cal_retry = 0;
            return SEQ_EX1;
        }
        else
        {
            //BRAKE
            SetMotorDriverState(REVERSE_BRAKE);
            dmfbk_val = (SHORT)DISC_BRAKE_LEVEL*DMODAC_FACTOR;
            WRITE_DSP_COEF(dmfbk_level, dmfbk_val);
            WRITE_REG(HAL_SRVREG_MAP->DMFBK, HAL_SRV_DMBK);   /* set to decelerate */
            WRITE_DSP_COEF(kdf14,DISC_STOP_BRAKE_FORCE3);

            //wait disc stop
            SetTimeout(0,DISC_STOP_NO_FG_TIME);
            while (timeout_chk(0) != eTimerTimeout)
            {
                if (pre_fgCount != svoSpeedVar.fgCount)
                {
                    pre_fgCount = svoSpeedVar.fgCount;
                    SetTimeout(0,DISC_STOP_NO_FG_TIME);
                }
            }
            WRITE_REG(HAL_SRVREG_MAP->DMFBK, 0);
            SetMotorDriverState(SHORT_BRAKE);

            StopFGTimer();

    #if (ALWAYS_HOMING == 0)
            if (cal_retry == 1)
            {
                svoStp_Homing();
                while (svoStp_busy())
                {
                    ExitProcess();
                }
            }
    #endif // (ALWAYS_HOMING == 1)
    #if (COLLI_SWITCH == 1) && (COLLI_ALWAYS_HOMING == 0)
            Iram_colli_Initialized = STEP_UNINITIALIZED;
    #endif
            if (cal_retry >= (STM_MAX_RETRIES > 0)? (STM_MAX_RETRIES -1):1)
            {
                svoStp_JumpSteps(STEP_SAFE_SLOW_MODE, UM_2_SLED_STEP(2000));
                while (svoStp_busy())
                {
                    ExitProcess();
                }
                svoStp_StopControl();
                svoIramVar.currentID.SectorNum.all32s = svoRadius2PBA(Iram_stp_CurrentPosition, 0);
            }
            return SEQ_EX2;
        }
#else // NODISC_RETRY

	#if (Customer_A1 == 1)
	      return SEQ_EX0;
	#else
        return SEQ_EX1;
	#endif


#endif // NODISC_RETRY
    }

#if (ENABLE_HDDVD == 1)
    #if (FORCE_HD_START_UP_FLOW == 1)//Force disc as HDROM
        #if (ENABLE_CBHD == 1)
    SendMsg80(SHOW_DEBUG_MSG,0x33FF2E); //Force as CBHDROM
    svoVar_discKindT.fDiscKind    = eCBHDROMDisc;
    svoCmdVar.demodMode           = eDemodFSM;
        #else // (ENABLE_CBHD == 1)
    SendMsg80(SHOW_DEBUG_MSG,0x33FF2F); //Force as HDROM
    svoVar_discKindT.fDiscKind    = eHDROMDisc;
    svoCmdVar.demodMode           = eDemodETM;
        #endif // (ENABLE_CBHD == 1)
    svoVar_discKindT.Bit.Media    = eMediaROM;
    svoVar_discKindT.Bit.WblType  = eWblUnknown;
    svoVar_discKindT.Bit.Layers   = eLayersSingle; //Force as single layer disc
    svoVar_discKindT.Bit.DiscStd  = eDiscHD;
    svoVar_discDiameter           = e12cm;
        #if (ENABLE_CBHD == 1)
    svoCalVar.discChkDoneBits |= stm_chk_end|blank_chk_end|plus_minus_chk_end|cbhd_disc_chk_end;
        #else // (ENABLE_CBHD == 1)
    svoCalVar.discChkDoneBits |= stm_chk_end|blank_chk_end|plus_minus_chk_end;
        #endif // (ENABLE_CBHD == 1)
    #endif // (FORCE_HD_START_UP_FLOW == 1)
#endif //(ENABLE_HDDVD == 1)

    // === Load Parameters according to disc kind ======
    svoAFEInitOnly(); //svoAFEInit();
    svo_Adj_Data_Clr();
#if (CHIP_REV >= 0xC0)
    HAL_LASC_SetOscen(OSCEN_RWDISC);
#else
    HAL_LASC_SetOscen(OSCEN_ON);
#endif

    svo_preset_media_par();
    bSetSpeedAgain = TRUE;
#if (HORROR_DISC_IMPROVEMENT == 1)
    TryDFE_Param=0;//== modify for DQA Disc to retry DFE parameter
#endif
#if DEFECT_LENGTH_MEASURE == 1
    CD_Scratch_Disc=0;//  == modify for SCD-2383
#endif

    afeSetTrackingMethod(getTrackingMethod(svoVar_discKindT));

    switch (svoVar_discKindT.Bit.DiscStd)
    {
        case eDiscCD:
#if DOUBLE_LENS
            if (IS_OUTER_LENS(svoVar_discKindT.Bit.DiscStd))
            {
                Iram_stp_CurrentPosition += STEP_LENS_OFFSET;
            }
#endif // DOUBLE_LENS
#if (DISC_MIS_JUDGE_START_UP_RECOVERY)
            RetryDiscLaser = eDiscCD;
#endif
#if (EN_POWER_CONTROL == 1)
    #if (WOBBLE_READ_ON_DVD_OR_CD == 1)
            PowerControl(eCDReadWblPower);
    #else
            PowerControl(eCDReadPower);
    #endif
#endif

            DISABLE_DVD_DECODER();
            spdChangeDiscSpeed(SVO_CAL_SPEED_CD);
            SendMsg80(SHOW_DEBUG_MSG,0x310094); // STM==>CD DISC
            break;

        case eDiscDVD:
#if DOUBLE_LENS
            if (IS_OUTER_LENS(svoVar_discKindT.Bit.DiscStd))
            {
                Iram_stp_CurrentPosition += STEP_LENS_OFFSET;
            }
#endif // DOUBLE_LENS
#if (DISC_MIS_JUDGE_START_UP_RECOVERY)
            RetryDiscLaser = eDiscDVD;
#endif
#if (EN_POWER_CONTROL == 1)
    #if (WOBBLE_READ_ON_DVD_OR_CD == 1)
            PowerControl(eDVDReadWblPower);
    #else
            PowerControl(eDVDReadPower);
    #endif
#endif
            ENABLE_DVD_DECODER();
            svoVar_svoTargetSpeed = SVO_CAL_SPEED_DVD;
            spdChangeDiscSpeed(SVO_CAL_SPEED_DVD);

            if (svoVar_discKindT.Bit.Layers == eLayersSingle)
            {
                SendMsg80(SHOW_DEBUG_MSG,0x310092); // STM==>DVD SL DISC
            }
            else
            {
                SendMsg80(SHOW_DEBUG_MSG,0x310091);//STM==>DVD DL DISC
            }
            break;
#if (BD_ENABLE == 1)
        case eDiscBD:
            /* SONY OPU KES410 triple read */
#if (DISC_MIS_JUDGE_START_UP_RECOVERY)
            RetryDiscLaser = eDiscBD;
#endif
    #if (EN_POWER_CONTROL == 1)
            PowerControl(eBDReadWblPower);
    #endif
            svoVar_svoTargetSpeed = SVO_CAL_SPEED_BD;
            spdChangeDiscSpeed(SVO_CAL_SPEED_BD);
    #if DOUBLE_LENS
            if (IS_OUTER_LENS(svoVar_discKindT.Bit.DiscStd))
            {
                Iram_stp_CurrentPosition += STEP_LENS_OFFSET;
            }
    #endif // DOUBLE_LENS
    #if (COLLI_SWITCH == 1)
            svoStp_JumpToPosition(COLLI_NORMAL_NO_REST_MODE, calGetOptColliPos(0));
        #if (OPU == BDPL2)
            HAL_LASC_SetAPCTarget(calGetOptColliPos(0));
        #endif
    #endif // (COLLI_SWITCH == 1)
            if (svoVar_discKindT.Bit.Layers == eLayersSingle)
            {
                SendMsg80(DEBUG_DISK_ID_MSG,0x3100A3); // STM==>BD SL Disc
            }
            else
            {
                SendMsg80(DEBUG_DISK_ID_MSG,0x3100A2); // STM==>BD DL Disc
            }
            break;
#endif // (BD_ENABLE == 1)
#if (ENABLE_HDDVD == 1)
        case eDiscHD:
    #if (EN_POWER_CONTROL == 1)
            PowerControl(eHDReadWblPower);
    #endif
            ENABLE_DVD_DECODER();
            svoVar_svoTargetSpeed = SVO_CAL_SPEED_HD;
            spdChangeDiscSpeed(SVO_CAL_SPEED_HD);
    #if DOUBLE_LENS
            if (IS_OUTER_LENS(svoVar_discKindT.Bit.DiscStd))
            {
                Iram_stp_CurrentPosition += STEP_LENS_OFFSET;
            }
    #endif // DOUBLE_LENS
            if (svoVar_discKindT.Bit.Layers == eLayersSingle)
            {
                SendMsg80(DEBUG_DISK_ID_MSG,0x3100B2); //STM==>HDDVD SL Disc
            }
            else
            {
                SendMsg80(DEBUG_DISK_ID_MSG,0x3100B1); //STM==>HDDVD SL Disc
            }
            break;
#endif // (ENABLE_HDDVD == 1)
    }
    WRITE_FIELD(HAL_AFE_PDEQBUF, 1); //0x10000971.7 set"1" to power down EQBUF
    HAL_DFE_DisableICEDetect(); //disable Iced area detect
#if (ENABLE_DFE_ICE_DETECT == 1)
    HAL_DFE_ClusterStateDisableInt();
#endif // (ENABLE_DFE_ICE_DETECT == 1)
    //Set_MIO3_TO_GPIO();
    //GIO3(0);
    //Set_MIO3_TO_MIO();
    //WRITE_FIELD(HAL_GLOBAL_M3SEL,halGLOBAL_MIO3_dfe_defect);

    svoCalVar.calDoneBits |= vref_end;
    Recorded_stp_Position[0] = Recorded_stp_Position[1] = svoStepVar.StepHomePos;

#if (PCB == 1600)
    //Need to turn on PDID
    HAL_LASC_PdicOn();
#endif

#if (PCB == 710)||(PCB == 711)||(PCB == 817)
#ifdef LD_LIFE_TEST
    if(LDLifeTestProcedure()==FAIL)
    {
        HAL_LASC_SetLaserOn(FALSE);
        return SEQ_EX1;
    }
#endif
#endif

    return(SEQ_EX0);
}

#if AFE_REG_DUMP
void disp_afe_para(void)
{
    const BYTE afeadd[]={0xa0,0xa1,0xa5,0xa6,0xa8,0xa9,0xb3,0xb4,0xb6,0xb7,0xb9,0xba,0xbb,0xbc,0xbd,0xbe,0x8a,0x8b,
                         0x8c,0x8e,0x8f,0x90,0x97,0x98,0x9b,0x9c,0x9d,0x84,0x85,0x88,0x91,0x92,0x95,0xff};
    BYTE i;
    USHORT reg_d_2B;
    BYTE   reg_d_1B;

    for(i=0;i<255;i++)
    {
        if (afeadd[i] == 0xff)
           break;

        reg_d_1B=READ_REG(*(vBYTE *)HAL_AFE_BaseAddress+afeadd[i]);
        SendMsgCnB(SHOW_DEBUG_MSG,4,0x3FAA00,B1B(AFE_PAGE>>8),B1B(afeadd[i]),B2B(reg_d_1B));
    }

    reg_d_2B = HAL_SERVO_GetFebc();
    SendMsgCn(SHOW_DEBUG_MSG,4,0x3FAA01,A4B(reg_d_2B));
    WRITE_REG(HAL_SRVREG_MAP->RGSEL,halSRV_RSEL_FGA);
    reg_d_2B=READ_REG(HAL_SRVREG_MAP->SRCR);
    SendMsgCn(SHOW_DEBUG_MSG,4,0x3FAA02,A4B(reg_d_2B));
    WRITE_REG(HAL_SRVREG_MAP->RGSEL,halSRV_RSEL_TGA);
    reg_d_2B=READ_REG(HAL_SRVREG_MAP->SRCR);
    SendMsgCn(SHOW_DEBUG_MSG,4,0x3FAA03,A4B(reg_d_2B));
    WRITE_REG(HAL_SRVREG_MAP->RGSEL,halSRV_RSEL_FEO);
    reg_d_2B=READ_REG(HAL_SRVREG_MAP->SRCR);
    SendMsgCn(SHOW_DEBUG_MSG,4,0x3FAA04,A4B(reg_d_2B));
    WRITE_REG(HAL_SRVREG_MAP->RGSEL,halSRV_RSEL_TEO);
    reg_d_2B=READ_REG(HAL_SRVREG_MAP->SRCR);
    SendMsgCn(SHOW_DEBUG_MSG,4,0x3FAA05,A4B(reg_d_2B));
}
#endif // AFE_REG_DUMP

#if (HORROR_DISC_IMPROVEMENT == 1)
/*-----------------------------------------------------------------------------
 *
 * Module: state_RFEQ_chg()
 * Description:
 *
 *----------------------------------------------------------------------------*/
BYTE Kp,Ki,CLIPLVL;    //TryDFE_Param b0:FPLL b1:MAHPFBW b2:CLIPLVL
    #if DEFECT_LENGTH_MEASURE == 1
extern BYTE CD_Scratch_Disc;
    #endif
extern WORD DMDERACNT,Jitter;
//ULONG ErrorRetry_LBA = 0xAAAAAAAA;
seq_exit_t state_RFEQ_chg(void)
{
    BYTE    temp_DPDGN, temp;
    BYTE       i;
    LONG       SmartRtyNewValue[Smart_Retry_Para_NO];
    const SBYTE *ptrRetryPara;
    #if (EN_ADEQ == 1)
    BYTE tap_temp[5];
    #endif
    #if (DROOP_RATE_DECREASE == 1)
    BYTE retry__DECFBWPA, retry__DECFBWIA;
    #endif

    if (!FokStatusOK())
    {
        return(SEQ_EX1);
    }

    switch (svoVar_discKindT.Bit.DiscStd)
    {
        case eDiscCD:
            //Each retry from 1st dfe parameters choice
//            retry_lba = HostXfer.StartBlock.lba;
//            if(ErrorRetry_LBA != retry_lba)
//            {
//                ErrorRetry_LBA = retry_lba;
//                read_retry = 0;
//            }
//            else
//            {
//                read_retry++;
//            }
            //Each retry from last retry choice
            read_retry++;//read_retry from 0~0xff
            send_msg5SValue(SHOW_DEBUG_MSG,"defect_C", 4, svoDfMeasVar.defect_count);
            if(svoDfMeasVar.defect_count>3)
            {
                 if((read_retry >=1)&&(read_retry<=2))
                 {
                      HAL_DFE_EnDisHfDefect(halDFE_HfDefectDisable);// disable
                 }
                 else if((read_retry >=3)&&(read_retry<=4))
                 {
                      HAL_DFE_EnDisHfDefect(halDFE_HfDefectEnable); // enable
                 }



                if((read_retry >=1)&&(read_retry<=3))
                {
                      if (((svoCalMisc1Var[Layer_Index].teGainDSP)*0.8) <= TGA_KTG_LOWER_LIMIT)
                      {
                           WRITE_DSP_COEF(ktg, TGA_KTG_LOWER_LIMIT);
                      }
                      else
                      {
                           WRITE_DSP_COEF(ktg, (svoCalMisc1Var[Layer_Index].teGainDSP)*0.8);
                      }
                 }
                    //else if (read_retry == 3)

             }


            if(read_retry >= 9)
            {
                read_retry=0;
            }
             SendMsgCnB(SHOW_DEBUG_MSG,1,0x33FF63,B1B(read_retry));
             
            //~~~#DFE    parameter : RLPB/viterbi(BD only) , NEQ(cliplevel,TAPgain) , AdaptiveEQ , movingavg , PLL Kp/Ki
#if 0//new change dfe parameters rule
//            if(St_cdrom_video == TRUE)
//                return(SEQ_EX1);
            SendMsgCnB(SHOW_DEBUG_MSG,1,0x33FF5A,B1B(read_retry));

            switch(read_retry&0x0F)
            {
                case 2:
                    WRITE_FIELD(HAL_DFE_FPLLKPCFG,3);
                    WRITE_FIELD(HAL_DFE_FPLLKICFG,6);
                    Kp=3;
                    Ki=6;
                    WRITE_FIELD(HAL_DFE_MAHPFBW, halDFE_MovAvHPFBW_512_adc_samples);//6
                    TryDFE_Param = 0x01;
                    break;

                case 3:
                    WRITE_FIELD(HAL_DFE_FPLLKPCFG,4);
                    WRITE_FIELD(HAL_DFE_FPLLKICFG,4);
                    Kp=4;
                    Ki=4;
                    WRITE_FIELD(HAL_DFE_MAHPFBW, halDFE_MovAvHPFBW_512_adc_samples);//6
                    TryDFE_Param = 0x01;
                    break;

                case 4:
                    WRITE_FIELD(HAL_DFE_FPLLKPCFG,4);
                    WRITE_FIELD(HAL_DFE_FPLLKICFG,6);
                    Kp=4;
                    Ki=6;
                    WRITE_FIELD(HAL_DFE_MAHPFBW, halDFE_MovAvHPFBW_512_adc_samples);//6
                    TryDFE_Param = 0x01;
                    break;

                case 5:
                    LAL_WriteCoef( DFE_HFFPLLCFG_FPLLKPCFG );
                    LAL_WriteCoef( DFE_HFFPLLCFG_FPLLKICFG );
                    WRITE_FIELD(HAL_DFE_MAHPFBW, halDFE_MovAvHPFBW_208_adc_samples);//3
                    TryDFE_Param = 0x02;
                    break;

                case 6:
                    WRITE_FIELD(HAL_DFE_FPLLKPCFG,3);
                    WRITE_FIELD(HAL_DFE_FPLLKICFG,6);
                    Kp=3;
                    Ki=6;
                    WRITE_FIELD(HAL_DFE_MAHPFBW, halDFE_MovAvHPFBW_208_adc_samples);//3
                    TryDFE_Param = 0x03;
                    break;

                case 7:
                    WRITE_FIELD(HAL_DFE_FPLLKPCFG,4);
                    WRITE_FIELD(HAL_DFE_FPLLKICFG,4);
                    Kp=4;
                    Ki=4;
                    WRITE_FIELD(HAL_DFE_MAHPFBW, halDFE_MovAvHPFBW_208_adc_samples);//3
                    TryDFE_Param = 0x03;
                    break;

                case 8:
                    WRITE_FIELD(HAL_DFE_FPLLKPCFG,4);
                    WRITE_FIELD(HAL_DFE_FPLLKICFG,6);
                    Kp=4;
                    Ki=6;
                    WRITE_FIELD(HAL_DFE_MAHPFBW, halDFE_MovAvHPFBW_208_adc_samples);//3
                    TryDFE_Param = 0x03;
                    read_retry = 0x0F;
                    break;

                default:
                    if(read_retry > 8)
                        read_retry = 0;
                    LAL_WriteCoef( DFE_HFFPLLCFG_FPLLKPCFG );
                    LAL_WriteCoef( DFE_HFFPLLCFG_FPLLKICFG );
                    WRITE_FIELD(HAL_DFE_MAHPFBW, halDFE_MovAvHPFBW_512_adc_samples);//6
                    WRITE_FIELD(HAL_DFE_CLIPLVL, COEF_GetValue(DFE_CALC_CLIPLVL_Normal) );
                    TryDFE_Param = 0;
                    break;
            }
    #if DEFECT_LENGTH_MEASURE == 1
            if( (CD_Scratch_Disc == 1)&&(READ_FIELD(HAL_DFE_FPLLKICFG)<4) ) //Fro CD_Scratch_Disc == 1
            {
                Kp = READ_FIELD(HAL_DFE_FPLLKPCFG);
                Ki = 4;
                WRITE_FIELD(HAL_DFE_FPLLKPCFG,Kp);
                WRITE_FIELD(HAL_DFE_FPLLKICFG,Ki);
                TryDFE_Param |= 0x01;
            }
    #endif
#else





#endif//#if 1
            //~~~#defect parameter : decrease sensitivity
            //~~~#servo  parameter : DPDgain , loopgain , TE/FE offset
            return(SEQ_EX1);
//            break;
#if (BD_ENABLE == 1)
        case eDiscBD:
            if (bdDecFlag.fREADING_LEADIN == TRUE)
            {
                //return(SEQ_EX1);
            }

    #if(WOB_BASED_DEMODULATOR == 1)
            if(svoVar_discKindT.Bit.Media != eMediaROM)
            {
                if((read_retry >=2)&&(read_retry<=5))
                {
                    wob_based_demod = 1;
//                    send_msg5S(1,"wob_base_demod");
                }
                else
                {
                    wob_based_demod = 0;
//                    send_msg5S(1,"rf_base_demod");
                }
            }
    #endif

            if((svoDfMeasVar.defect_count>=3)&&(READ_FIELD(HAL_SRV_DEEPDFCT)!=0)&&(read_retry>=5))
             {

               WRITE_FIELD(HAL_SRV_DEEPDFCT, FALSE);
               WRITE_FIELD(HAL_SRV_DBT,  0x0A);//set BS defect delay time
               //send_msg5S(1,"useBS");


             }
            else if((svoDfMeasVar.defect_count>=3)&&(READ_FIELD(HAL_SRV_DEEPDFCT)!=1)&&(read_retry<5))
             {
               WRITE_FIELD(HAL_SRV_DEEPDFCT, TRUE);
               WRITE_FIELD(HAL_SRV_DBT,  0x00);

             }

            if(svoDfMeasVar.defect_count>=3)
            {
                if((read_retry >=1)&&(read_retry<=2))
                {
                   HAL_DFE_EnDisHfDefect(halDFE_HfDefectDisable);// disable
                }
                else if((read_retry >=3)&&(read_retry<=4))
                {
                   HAL_DFE_EnDisHfDefect(halDFE_HfDefectEnable); // enable
                }
                else if((read_retry >=5)&&(read_retry<=6))
                {
                   HAL_DFE_EnDisHfDefect(halDFE_HfDefectDisable);// disable
                }
                else if((read_retry >=7)&&(read_retry<=8))
                {
                   HAL_DFE_EnDisHfDefect(halDFE_HfDefectEnable); // enable
                }
            }
            //~~~#DFE    parameter : RLPB/viterbi(BD only) , NEQ(cliplevel,TAPgain) , AdaptiveEQ , movingavg , PLL Kp/Ki
            if ((svoVar_discKindT.fDiscKind == eBDREDLDisc)||(svoVar_discKindT.fDiscKind == eBDRDLDisc))
            {
#if 0  //will enable later to cover white defect disc
                if (((bdClusterInterruptTimer.fTIMEOUT == TRUE)||(bdErrorFlag.fTID_MISS))&&(defect_length <= defect_length_limit))
                {
                    if (read_retry == 2)
                    {
                        if (((svoCalMisc1Var[Layer_Index].teGainDSP)*1.4) >= BD_TGA_KTG_UPPER_LIMIT)
                        {
                           WRITE_DSP_COEF(ktg, BD_TGA_KTG_UPPER_LIMIT);
                        }
                        else
                        {
                           WRITE_DSP_COEF(ktg, (svoCalMisc1Var[Layer_Index].teGainDSP)*1.4);
                        }
                        calSetMisc(Layer_Index, TE_SPT, -20); /* write tracking Offset Data data */
                        apply_TESPT(svoCalMisc1Var[Layer_Index].teSetPoint);
                    }
                    else if (read_retry == 3)
                    {
                        calSetMisc(Layer_Index, TE_SPT, 20); /* write tracking Offset Data data */
                        apply_TESPT(svoCalMisc1Var[Layer_Index].teSetPoint);
                    }
                    else if (read_retry == 4)
                    {
                        if (((svoCalMisc1Var[Layer_Index].teGainDSP)*1.4) >= BD_TGA_KTG_UPPER_LIMIT)
                        {
                           WRITE_DSP_COEF(ktg, BD_TGA_KTG_UPPER_LIMIT);
                        }
                        else
                        {
                           WRITE_DSP_COEF(ktg, (svoCalMisc1Var[Layer_Index].teGainDSP)*1.4);
                        }
                        calSetMisc(Layer_Index, TE_SPT, -30); /* write tracking Offset Data data */
                        apply_TESPT(svoCalMisc1Var[Layer_Index].teSetPoint);
                    }
                    else if (read_retry == 5)
                    {
                        calSetMisc(Layer_Index, TE_SPT, 30); /* write tracking Offset Data data */
                        apply_TESPT(svoCalMisc1Var[Layer_Index].teSetPoint);
                    }
                    else if (read_retry == 6)
                    {
                        if (((svoCalMisc1Var[Layer_Index].teGainDSP)*1.4) >= BD_TGA_KTG_UPPER_LIMIT)
                        {
                            WRITE_DSP_COEF(ktg, BD_TGA_KTG_UPPER_LIMIT);
                        }
                        else
                        {
                            WRITE_DSP_COEF(ktg, (svoCalMisc1Var[Layer_Index].teGainDSP)*1.4);
                        }
                        calSetMisc(Layer_Index, TE_SPT, -45); /* write tracking Offset Data data */
                        apply_TESPT(svoCalMisc1Var[Layer_Index].teSetPoint);
                    }
                    else if (read_retry == 7)
                    {
                        calSetMisc(Layer_Index, TE_SPT, 45); /* write tracking Offset Data data */
                        apply_TESPT(svoCalMisc1Var[Layer_Index].teSetPoint);
                    }
                    else if (read_retry == 9)
                    {
                        calSetMisc(Layer_Index, TE_SPT, 0); /* write tracking Offset Data data */
                        apply_TESPT(svoCalMisc1Var[Layer_Index].teSetPoint);

                    }
                    SendMsgCn(SHOW_DEBUG_MSG,2,0x31751E,A2B(svoCalMisc1Var[Layer_Index].teSetPoint));
                } //Read retry TE offset
                else
                {
                    if (svoCalMisc1Var[Layer_Index].teSetPoint != 0)
                    {
                        calSetMisc(Layer_Index, TE_SPT, 0);
                        apply_TESPT(svoCalMisc1Var[Layer_Index].teSetPoint);
                        SendMsg80(SHOW_DEBUG_MSG,0x317520); // ReStore TE offset
                    }
                }
#endif
                if (svoVar_discKindT.fDiscKind == eBDREDLDisc) // only for BDRE DL //== For MTBF BDROMDL retry
                {
                    if (READ_FIELD(HAL_DFE_BITDETCFG)==0x02)
                    {
                        WRITE_FIELD(HAL_DFE_BITDETCFG,0x03);
                    }
                    else if (READ_FIELD(HAL_DFE_BITDETCFG)==0x03)
                    {
                        WRITE_FIELD(HAL_DFE_BITDETCFG,0x02);
                    }
                    //==start== improve BDREDL Ice detect and read with GZP disc
                    if((BdwtDisc.ContinueIcedCnt >= 0x200)&&(READ_FIELD(HAL_DFE_ICEHFTHRL) < 0x16))
                    {
                       if(read_retry == 0)
                       {
                          WRITE_FIELD(HAL_DFE_ICEHFTHRL , 0x10);  //ICE area HF Threshold Low
                       }
                       else if(read_retry >= 1)
                       {
                          WRITE_FIELD(HAL_DFE_ICEHFTHRL , 0x16);  //ICE area HF Threshold Low
                       }
                       SendMsg80(1,0x33FF5C);//chg ICEth LOW

                    }
                    //==end== improve BDREDL Ice detect and read with GZP disc
                    // == start == Retry for BDREDL disc change DFE EQTAPA1A2
                #ifdef RETRY_CHG_EQTAPA12
                    if((read_retry==4)||(read_retry==5))    // == start == gain up focus gain to cover some bad disc(2kHz oscillation in focus loop) in GZP
                    {
                        //send_msg5S(1,"Chg A12_L");
                        LAL_DFE_SetEqualizer(lalDFE_EqualizerStrengthLow);
                        retry_DFE_EqualizerStrength = lalDFE_EqualizerStrengthLow;
                    }
                    else if((read_retry==6)||(read_retry==7))
                    {
                        //send_msg5S(1,"Chg A12_H");
                        retry_DFE_EqualizerStrength = lalDFE_EqualizerStrengthHigh;
                        LAL_DFE_SetEqualizer(lalDFE_EqualizerStrengthHigh);
                    }
                    else
                    {
                        //send_msg5S(1,"Chg A12_N");
                        retry_DFE_EqualizerStrength = lalDFE_EqualizerStrengthNormal;
                        LAL_DFE_SetEqualizer(lalDFE_EqualizerStrengthNormal);
                    }
                #endif

#if (EN_BD_ADEQ == 1)
                    if ((read_retry >= 3) && (read_retry <= 5))
                    {
                        Disable_ADEQ = 1;
                    }
                    else if (read_retry >= 6)
                    {
                        Disable_ADEQ = 0;
                    }

                    if((read_retry >= 6) && (read_retry <= 8))
                    {
                        WRITE_FIELD(HAL_DFE_ADQORTCND, 1);
                    }
                    else
                    {
                        WRITE_FIELD(HAL_DFE_ADQORTCND, 0);
                    }
#endif
                }

                if((svoVar_discKindT.fDiscKind == eBDRDLDisc)&&(svoIramVar.svoCurrentSpeed < _3LX_BD ))//_4AX_BD
                {
#if(EN_BD_ADEQ == 1)
                     if ((read_retry >= 3) && (read_retry <= 5))
                     {
                             Disable_ADEQ = 1;
                     }
                     else if(read_retry >= 6)
                     {
                             Disable_ADEQ = 0;
                     }
#endif

#ifdef RETRY_CHG_EQTAPA12

                    if((read_retry==4)||(read_retry==5))
                    {
                        send_msg5S(1,"EQTAP12_L");
                        LAL_DFE_SetEqualizer(lalDFE_EqualizerStrengthLow);
                        retry_DFE_EqualizerStrength = lalDFE_EqualizerStrengthLow;
                    }
                    else if((read_retry==6)||(read_retry==7))
                    {
                        send_msg5S(1,"EQTAP12_H");
                        retry_DFE_EqualizerStrength = lalDFE_EqualizerStrengthHigh;
                        LAL_DFE_SetEqualizer(lalDFE_EqualizerStrengthHigh);
                    }
                    else
                    {
                        send_msg5S(1,"EQTAP12_M");
                        retry_DFE_EqualizerStrength = lalDFE_EqualizerStrengthNormal;
                        LAL_DFE_SetEqualizer(lalDFE_EqualizerStrengthNormal);
                    }
#endif

                    if (read_retry == 2)
                    {
                        calSetMisc(Layer_Index, TE_SPT, -20); /* write tracking Offset Data data */
                        apply_TESPT(svoCalMisc1Var[Layer_Index].teSetPoint);
                    }
                    else if (read_retry == 3)
                    {
                        calSetMisc(Layer_Index, TE_SPT, 20); /* write tracking Offset Data data */
                        apply_TESPT(svoCalMisc1Var[Layer_Index].teSetPoint);
                    }
                    else if (read_retry == 4)
                    {
                        calSetMisc(Layer_Index, TE_SPT, -30); /* write tracking Offset Data data */
                        apply_TESPT(svoCalMisc1Var[Layer_Index].teSetPoint);
                    }
                    else if (read_retry == 5)
                    {
                        calSetMisc(Layer_Index, TE_SPT, 30); /* write tracking Offset Data data */
                        apply_TESPT(svoCalMisc1Var[Layer_Index].teSetPoint);
                    }
                    else if (read_retry == 6)
                    {
                        calSetMisc(Layer_Index, TE_SPT, -45); /* write tracking Offset Data data */
                        apply_TESPT(svoCalMisc1Var[Layer_Index].teSetPoint);
                    }
                    else if (read_retry == 7)
                    {
                        calSetMisc(Layer_Index, TE_SPT, 45); /* write tracking Offset Data data */
                        apply_TESPT(svoCalMisc1Var[Layer_Index].teSetPoint);
                    }
                    else if (read_retry == 9)
                    {
                        calSetMisc(Layer_Index, TE_SPT, 0); /* write tracking Offset Data data */
                        apply_TESPT(svoCalMisc1Var[Layer_Index].teSetPoint);

                    }
                    SendMsgCn(SHOW_DEBUG_MSG,2,0x31751E,A2B(svoCalMisc1Var[Layer_Index].teSetPoint));
                } //Read retry TE offset
                else
                {
                    if (svoCalMisc1Var[Layer_Index].teSetPoint != 0)
                    {
                        calSetMisc(Layer_Index, TE_SPT, 0);
                        apply_TESPT(svoCalMisc1Var[Layer_Index].teSetPoint);
                        SendMsg80(SHOW_DEBUG_MSG,0x317520); // ReStore TE offset
                    }
                }
            }
            //~~~#defect parameter : decrease sensitivity
            //~~~#servo  parameter : DPDgain , loopgain , TE/FE offset
            if (svoVar_discKindT.fDiscKind == eBDROMDLDisc) // ==start== Change servo parameters to improve BDROM performance when there is white defect/oil on disc surface
            {
#if (EN_BD_ADEQ == 1)
                if((read_retry >= 3) && (read_retry <= 5))
                {
                    Disable_ADEQ = 1;
                    WRITE_FIELD(HAL_DFE_VITTYP,0);
                }
                else if (read_retry >= 6)
                {
                    Disable_ADEQ = 0;
                    WRITE_FIELD(HAL_DFE_VITTYP,1);
                }

                if(svoVar_discKindT.fDiscKind == eBDROMDLDisc)
                {
                    if((read_retry >= 6) && (read_retry <= 8))
                    {
                        WRITE_FIELD(HAL_DFE_ADQORTCND, 0);
                    }
                    else
                    {
                        WRITE_FIELD(HAL_DFE_ADQORTCND, 1);
                    }
                }
                else if(svoVar_discKindT.fDiscKind == eBDROMDisc)
                {
                    if((read_retry == 8) || (read_retry == 9))
                    {
                        WRITE_FIELD(HAL_DFE_ADQORTCND, 0);
                    }
                    else
                    {
                        WRITE_FIELD(HAL_DFE_ADQORTCND, 1);
                    }
                }
#endif

#if 0
                if (((bdClusterInterruptTimer.fTIMEOUT == TRUE)||(bdErrorFlag.fTID_MISS))&&(defect_length <= (2*NO_SPD_UP_DEFECT_LENGTH))
                   &&(svoVar_discKindT.fDiscKind == eBDROMDLDisc))
                {
                    if (read_retry == 2)
                    {
                        if (((svoCalMisc1Var[Layer_Index].teGainDSP)*1.5) >= BD_TGA_KTG_UPPER_LIMIT)
                        {
                           WRITE_DSP_COEF(ktg, BD_TGA_KTG_UPPER_LIMIT);
                        }
                        else
                        {
                           WRITE_DSP_COEF(ktg, (svoCalMisc1Var[Layer_Index].teGainDSP)*1.5);
                        }
                        calSetMisc(Layer_Index, TE_SPT, -20); /* write tracking Offset Data data */
                        apply_TESPT(svoCalMisc1Var[Layer_Index].teSetPoint);
                    }
                    else if (read_retry == 3)
                    {
                        calSetMisc(Layer_Index, TE_SPT, 20); /* write tracking Offset Data data */
                        apply_TESPT(svoCalMisc1Var[Layer_Index].teSetPoint);

                    }
                   else if (read_retry == 4)
                   {
                        if (((svoCalMisc1Var[Layer_Index].teGainDSP)*1.5) >= BD_TGA_KTG_UPPER_LIMIT)
                    {
                           WRITE_DSP_COEF(ktg, BD_TGA_KTG_UPPER_LIMIT);
                        }
                        else
                        {
                           WRITE_DSP_COEF(ktg, (svoCalMisc1Var[Layer_Index].teGainDSP)*1.5);
                        }
                        calSetMisc(Layer_Index, TE_SPT, -40); /* write tracking Offset Data data */
                        apply_TESPT(svoCalMisc1Var[Layer_Index].teSetPoint);
                    }
                    else if (read_retry == 5)
                    {
                        calSetMisc(Layer_Index, TE_SPT, 40); /* write tracking Offset Data data */
                        apply_TESPT(svoCalMisc1Var[Layer_Index].teSetPoint);
                    }
                    else if (read_retry == 6)
                    {
                        if (((svoCalMisc1Var[Layer_Index].teGainDSP)*1.5) >= BD_TGA_KTG_UPPER_LIMIT)
                        {
                           WRITE_DSP_COEF(ktg, BD_TGA_KTG_UPPER_LIMIT);
                        }
                        else
                        {
                           WRITE_DSP_COEF(ktg, (svoCalMisc1Var[Layer_Index].teGainDSP)*1.5);
                        }
                        calSetMisc(Layer_Index, TE_SPT, -35); /* write tracking Offset Data data */
                        apply_TESPT(svoCalMisc1Var[Layer_Index].teSetPoint);
                    }
                    else if (read_retry == 7)
                    {
                        calSetMisc(Layer_Index, TE_SPT, 35); /* write tracking Offset Data data */
                        apply_TESPT(svoCalMisc1Var[Layer_Index].teSetPoint);
                    }
                    else if (read_retry == 9)
                    {
                        calSetMisc(Layer_Index, TE_SPT, 0); /* write tracking Offset Data data */
                        apply_TESPT(svoCalMisc1Var[Layer_Index].teSetPoint);

                    }
                    SendMsgCn(SHOW_DEBUG_MSG,2,0x31751E,A2B(svoCalMisc1Var[Layer_Index].teSetPoint));
                } //Read retry TE offset
                else
                {
                    if (read_retry == 6)    /* force to shake TE offset */
                    {
                        if (((svoCalMisc1Var[Layer_Index].teGainDSP)*1.5) >= BD_TGA_KTG_UPPER_LIMIT)
                        {
                            WRITE_DSP_COEF(ktg, BD_TGA_KTG_UPPER_LIMIT);
                        }
                        else
                        {
                            WRITE_DSP_COEF(ktg, (svoCalMisc1Var[Layer_Index].teGainDSP)*1.5);
                        }
                        calSetMisc(Layer_Index, TE_SPT, -40); /* write tracking Offset Data data */
                        apply_TESPT(svoCalMisc1Var[Layer_Index].teSetPoint);
                    }
                    else if (read_retry == 7)
                    {
                        calSetMisc(Layer_Index, TE_SPT, 40); /* write tracking Offset Data data */
                        apply_TESPT(svoCalMisc1Var[Layer_Index].teSetPoint);
                    }
                    else
                    {
                        if (svoCalMisc1Var[Layer_Index].teSetPoint != 0)
                        {
                            calSetMisc(Layer_Index, TE_SPT, 0);
                            apply_TESPT(svoCalMisc1Var[Layer_Index].teSetPoint);
                            SendMsg80(SHOW_DEBUG_MSG,0x317520); // ReStore TE offset
                        }
                    }
                    SendMsgCn(SHOW_DEBUG_MSG,2,0x31751E,A2B(svoCalMisc1Var[Layer_Index].teSetPoint));
                }
#endif
                if((read_retry==4)||(read_retry==5))    // == start == gain up focus gain to cover some bad disc(2kHz oscillation in focus loop) in GZP
                {
                    if(svoIramVar.svoCurrentSpeed == _4AX_BD)
                    {
                        send_msg5S(1,"Change kf16/17");
                        WRITE_DSP_COEF(kf16,(svo_Eq_Tbl_Data(_4AX_BD,kf16) * 1.25));
                        WRITE_DSP_COEF(kf17,(svo_Eq_Tbl_Data(_4AX_BD,kf17) * 1.25));
                    }
                }
                else if((read_retry==7)||(read_retry==8))
                {
                    if(svoIramVar.svoCurrentSpeed == _4AX_BD)
                    {
                        send_msg5S(1,"Restore kf16/17");
                        WRITE_DSP_COEF(kf16,svo_Eq_Tbl_Data(_4AX_BD,kf16));
                        WRITE_DSP_COEF(kf17,svo_Eq_Tbl_Data(_4AX_BD,kf17));
                    }
                }

                if((read_retry==7)||(read_retry==8))//==modify BD speed down & retry counter
                {
                    send_msg5S(1,"Change ktg");
                    if(svoIramVar.svoCurrentSpeed == _4AX_BD)
                    {
                        if( ((svoCalMisc1Var[Layer_Index].teGainDSP)*0.4) <= TGA_KTG_LOWER_LIMIT )  //==start== Cover bad disc(white defect on disc surface) from GZP
                        {
                            WRITE_DSP_COEF(ktg,TGA_KTG_LOWER_LIMIT);
                        }
                        else
                        {
                            WRITE_DSP_COEF(ktg, (svoCalMisc1Var[Layer_Index].teGainDSP)*0.4);
                        }
                    }
                    else if(svoIramVar.svoCurrentSpeed == CLV_SPEED_BD)
                    {
                        if( ((svoCalMisc1Var[Layer_Index].teGainDSP)*0.6) <= TGA_KTG_LOWER_LIMIT )
                        {
                            WRITE_DSP_COEF(ktg,TGA_KTG_LOWER_LIMIT);
                        }
                        else
                        {
                            WRITE_DSP_COEF(ktg, (svoCalMisc1Var[Layer_Index].teGainDSP)*0.6);
                        }
                    }    //==start== Cover bad disc(white defect on disc surface) from GZP
                }

                if(svoVar_discKindT.fDiscKind == eBDROMDLDisc)
                {
                    if( (read_retry==6)||(read_retry==9) )
                    {
                        if (READ_FIELD(HAL_DFE_BITDETCFG)==0x02)
                        {
                            WRITE_FIELD(HAL_DFE_BITDETCFG,0x03);
                        }
                        else if (READ_FIELD(HAL_DFE_BITDETCFG)==0x03)
                        {
                            WRITE_FIELD(HAL_DFE_BITDETCFG,0x02);
                        }
                    }
                }

            }

            if (svoVar_discKindT.fDiscKind == eBDREDisc)
            {

                    if (((bdClusterInterruptTimer.fTIMEOUT == TRUE)||(bdErrorFlag.fTID_MISS))&&(svoDfMeasVar.defect_length <= NO_SPD_UP_DEFECT_LENGTH))
                    {
                        if (read_retry == 2)
                        {
//                            if (((svoCalMisc1Var[Layer_Index].teGainDSP)*1.4) >= BD_TGA_KTG_UPPER_LIMIT)
//                            {
//                               WRITE_DSP_COEF(ktg, BD_TGA_KTG_UPPER_LIMIT);
//                            }
//                            else
//                            {
//                               WRITE_DSP_COEF(ktg, (svoCalMisc1Var[Layer_Index].teGainDSP)*1.4);
//                            }
                            calSetMisc(Layer_Index, TE_SPT, -20); /* write tracking Offset Data data */
                            apply_TESPT(svoCalMisc1Var[Layer_Index].teSetPoint);
                        }
                        else if (read_retry == 3)
                        {
                            calSetMisc(Layer_Index, TE_SPT, 20); /* write tracking Offset Data data */
                            apply_TESPT(svoCalMisc1Var[Layer_Index].teSetPoint);
                        }
                        else if (read_retry == 4)
                        {
//                            if (((svoCalMisc1Var[Layer_Index].teGainDSP)*1.4) >= BD_TGA_KTG_UPPER_LIMIT)
//                            {
//                               WRITE_DSP_COEF(ktg, BD_TGA_KTG_UPPER_LIMIT);
//                            }
//                            else
//                            {
//                               WRITE_DSP_COEF(ktg, (svoCalMisc1Var[Layer_Index].teGainDSP)*1.4);
//                            }
                            calSetMisc(Layer_Index, TE_SPT, -30); /* write tracking Offset Data data */
                            apply_TESPT(svoCalMisc1Var[Layer_Index].teSetPoint);
                        }
                        else if (read_retry == 5)
                        {
                            calSetMisc(Layer_Index, TE_SPT, 30); /* write tracking Offset Data data */
                            apply_TESPT(svoCalMisc1Var[Layer_Index].teSetPoint);
                        }
                        else if (read_retry == 6)
                        {
//                            if (((svoCalMisc1Var[Layer_Index].teGainDSP)*1.4) >= BD_TGA_KTG_UPPER_LIMIT)
//                            {
//                                WRITE_DSP_COEF(ktg, BD_TGA_KTG_UPPER_LIMIT);
//                            }
//                            else
//                            {
//                                WRITE_DSP_COEF(ktg, (svoCalMisc1Var[Layer_Index].teGainDSP)*1.4);
//                            }
                            calSetMisc(Layer_Index, TE_SPT, -45); /* write tracking Offset Data data */
                            apply_TESPT(svoCalMisc1Var[Layer_Index].teSetPoint);
                        }
                        else if (read_retry == 7)
                        {
                            calSetMisc(Layer_Index, TE_SPT, 45); /* write tracking Offset Data data */
                            apply_TESPT(svoCalMisc1Var[Layer_Index].teSetPoint);
                        }
                        else if (read_retry == 9)
                        {
                            calSetMisc(Layer_Index, TE_SPT, 0); /* write tracking Offset Data data */
                            apply_TESPT(svoCalMisc1Var[Layer_Index].teSetPoint);

                        }
                        SendMsgCn(SHOW_DEBUG_MSG,2,0x31751E,A2B(svoCalMisc1Var[Layer_Index].teSetPoint));
                    } //Rea
                    else
                    {
                        if (svoCalMisc1Var[Layer_Index].teSetPoint != 0)
                        {
                            calSetMisc(Layer_Index, TE_SPT, 0);
                            apply_TESPT(svoCalMisc1Var[Layer_Index].teSetPoint);
                            SendMsg80(SHOW_DEBUG_MSG,0x317520); // ReStore TE offset
                        }
                    }


#if(EN_BD_ADEQ == 1)
                if ((read_retry >= 3) && (read_retry <= 5))
                {
                        Disable_ADEQ = 1;
                }
                else if(read_retry >= 6)
                {
                        Disable_ADEQ = 0;
                }
#endif

#ifdef RETRY_CHG_EQTAPA12

                if((read_retry==4)||(read_retry==5))
                {
                    send_msg5S(1,"EQTAP12_L");
                    LAL_DFE_SetEqualizer(lalDFE_EqualizerStrengthLow);
                    retry_DFE_EqualizerStrength = lalDFE_EqualizerStrengthLow;
                }
                else if((read_retry==6)||(read_retry==7))
                {
                    send_msg5S(1,"EQTAP12_H");
                    retry_DFE_EqualizerStrength = lalDFE_EqualizerStrengthHigh;
                    LAL_DFE_SetEqualizer(lalDFE_EqualizerStrengthHigh);
                }
                else
                {
                    send_msg5S(1,"EQTAP12_M");
                    retry_DFE_EqualizerStrength = lalDFE_EqualizerStrengthNormal;
                    LAL_DFE_SetEqualizer(lalDFE_EqualizerStrengthNormal);
                }
#endif

#if 1  //will enable later to cover white defect disc
                if (svoVar_discKindT.fDiscKind == eBDREDisc)
                {
                    //FE gain up
                    if ((read_retry>=5)&&(read_retry<=7))       //gain up focus gain to cover some bad disc(2kHz oscillation in focus loop) in GZP
                    {
                        if (svoIramVar.svoCurrentSpeed >= _4AX_BD)
                        {
                            SendMsg80(SHOW_DEBUG_MSG,0x31751B); // Change kfg
                            WRITE_DSP_COEF(kfg, svoCalMisc1Var[Layer_Index].feGainDSP * 5 / 4);
                        }
                    }
                    else if ((read_retry==8)||(read_retry==9))
                    {
                        if (svoIramVar.svoCurrentSpeed >= _4AX_BD)
                        {
                            SendMsg80(SHOW_DEBUG_MSG,0x31751A); // Restore kfg
                            WRITE_DSP_COEF(kfg, svoCalMisc1Var[Layer_Index].feGainDSP);
                        }
                    }

                    //TE gain down
                    if ((read_retry >= 8)&&(read_retry <= 9))
                    {
                        SendMsg80(SHOW_DEBUG_MSG,0x31751C);     // Change ktg
                        if (svoIramVar.svoCurrentSpeed >= _4AX_BD)
                        {
                            if (((svoCalMisc1Var[Layer_Index].teGainDSP)*0.4) <= BD_TGA_KTG_LOWER_LIMIT)
                            {
                                WRITE_DSP_COEF(ktg,BD_TGA_KTG_LOWER_LIMIT);
                            }
                            else
                            {
                                WRITE_DSP_COEF(ktg, (svoCalMisc1Var[Layer_Index].teGainDSP)*0.4);
                            }

                        }
                        else if (svoIramVar.svoCurrentSpeed == CLV_SPEED_BD)
                        {
                            if (((svoCalMisc1Var[Layer_Index].teGainDSP)*0.4) <= BD_TGA_KTG_LOWER_LIMIT)
                            {
                                WRITE_DSP_COEF(ktg,BD_TGA_KTG_LOWER_LIMIT);
                            }
                            else
                            {
                                WRITE_DSP_COEF(ktg, (svoCalMisc1Var[Layer_Index].teGainDSP)*0.4);
                            }
                         }
                    }
                }
#endif
            }
            if (svoIramVar.LTHDisc == TRUE)
            {
                 if ((read_retry==6)||(read_retry==9))
                    {
                        if (READ_FIELD(HAL_DFE_BITDETCFG)==0x02)
                        {
                            WRITE_FIELD(HAL_DFE_BITDETCFG,0x03);
                        }
                        else if (READ_FIELD(HAL_DFE_BITDETCFG)==0x03)
                        {
                            WRITE_FIELD(HAL_DFE_BITDETCFG,0x02);
                        }
                    }
                 if ((read_retry >= 1) && (read_retry <= 5))
                    WRITE_FIELD(HAL_DFE_SLCLVLB,0x01);
                 else
                    WRITE_FIELD(HAL_DFE_SLCLVLB,0x00);

#if(EN_BD_ADEQ == 1)
            if ((read_retry >= 3) && (read_retry <= 5))
            {
                    Disable_ADEQ = 1;
            }
            else if(read_retry >= 6)
            {
                    Disable_ADEQ = 0;
            }
#endif

#ifdef RETRY_CHG_EQTAPA12

                if((read_retry==4)||(read_retry==5))
                {
                    send_msg5S(1,"EQTAP12_L");
                    LAL_DFE_SetEqualizer(lalDFE_EqualizerStrengthLow);
                    retry_DFE_EqualizerStrength = lalDFE_EqualizerStrengthLow;
                }
                else if((read_retry==6)||(read_retry==7))
                {
                    send_msg5S(1,"EQTAP12_H");
                    retry_DFE_EqualizerStrength = lalDFE_EqualizerStrengthHigh;
                    LAL_DFE_SetEqualizer(lalDFE_EqualizerStrengthHigh);
                }
                else
                {
                    send_msg5S(1,"EQTAP12_M");
                    retry_DFE_EqualizerStrength = lalDFE_EqualizerStrengthNormal;
                    LAL_DFE_SetEqualizer(lalDFE_EqualizerStrengthNormal);
                }
#endif



            }
            read_retry++;
            SendMsgCnB(SHOW_DEBUG_MSG,1,0x33FF63,B1B(read_retry));
            if(read_retry >= 9)
            {
                read_retry=0;
            }

            break;
#endif
        case eDiscDVD:
            //~~~#DFE    parameter : RLPB/viterbi(BD only) , NEQ(cliplevel,TAPgain) , AdaptiveEQ , movingavg , PLL Kp/Ki
            //~~~#defect parameter : decrease sensitivity
            //~~~#servo  parameter : DPDgain , loopgain , TE/FE offset

            //Sometimes reading will become very bad, we need to reset some DFE settings at that time.
            if (((read_retry % 4) == 0) || ((PO_UN_C == 1111) || (PO_UN_C == 4444)))
            {
                temp = READ_FIELD(HAL_DFE_IIRHPFBW);
                SendMsgCnB(SHOW_DEBUG_MSG,2,0x33FF74,B1B(temp),B1B(READ_FIELD(HAL_DFE_BETAMSEN)));
                if (READ_FIELD(HAL_DFE_BETAMSEN) == 1)
                    WRITE_FIELD(HAL_DFE_BETAMSEN, 0);
                else
                    WRITE_FIELD(HAL_DFE_BETAMSEN, 1);
                DelaymS(1);//a small delay
                if (READ_FIELD(HAL_DFE_BETAMSEN) == 1)
                    WRITE_FIELD(HAL_DFE_BETAMSEN, 0);
                else
                    WRITE_FIELD(HAL_DFE_BETAMSEN, 1);
                WRITE_FIELD(HAL_DFE_IIRHPFBW, temp);
            }
            //Sometimes reading will become very bad, we need to reset some DFE settings at that time.

            //20101015_02PU ==start== Modify servo defect retry action timing to prevent it not to be done due to other retrys.
            /* Time up and reset */
            if ((read_retry_option & READ_RETRY_ENABLE) == READ_RETRY_ENABLE)
            {
                if (((read_retry_option & READ_RETRY_TIMER_ENABLE) == READ_RETRY_TIMER_ENABLE) && (read_retry_restartTime == 0))
                {
                    StartTimer(&read_retry_restartTime);
                    SendMsg80(SHOW_DEBUG_MSG,0x33FF32); //RetryTimerStart
                }
                /* Modify HF defect detecter droop rate and use BS servo defect */
                if ((read_retry_option&SERVO_DEFECT_RETRY_ENABLE)==SERVO_DEFECT_RETRY_ENABLE)
                {
                    Defect_Enable(DEFECT_SOURCE_FREEZE_BS);
                    read_retry_status |= SERVO_DEFECT_RETRY_ENABLE;
                }
            }
            //20101015_02PU ==end== Modify servo defect retry action timing to prevent it not to be done due to other retrys.

        //20101027_02RY ==start== Do read retry reset before write action
        //20101027_02YM ==start== Fix issue that read retry reset action will not be done.
        #if (DVD_RAM_READ == 1)
            if (((!DVDRAM) && ((read_retry_option & READ_RETRY_ENABLE) == READ_RETRY_ENABLE))
                && ((read_retry_option&READ_RETRY_RESET_ENABLE)!=READ_RETRY_RESET_ENABLE))
        #else
            if (((read_retry_option & READ_RETRY_ENABLE) == READ_RETRY_ENABLE)
                && ((read_retry_option&READ_RETRY_RESET_ENABLE)!=READ_RETRY_RESET_ENABLE))
        #endif
        //20101027_02YM ==end== Fix issue that read retry reset action will not be done.
        //20101027_02RY ==end== Do read retry reset before write action
            {
                     /* Special case for changing FAST PLL setting */
                if (((PO_UN_C == 0) && (PI_UN_C == 0) && ((PI_E > 150) || ((read_retry_option&TESPT_RETRY_ENABLE) == TESPT_RETRY_ENABLE) ||
                    ((read_retry_option&FAST_PLL_RETRY_SPECIAL_ENABLE) == FAST_PLL_RETRY_SPECIAL_ENABLE)))
                    ||((PO_C == 0) && (PI_C == 0) && ((PI_E > 150) || ((read_retry_option&TESPT_RETRY_ENABLE) == TESPT_RETRY_ENABLE) ||
                    ((read_retry_option&FAST_PLL_RETRY_SPECIAL_ENABLE) == FAST_PLL_RETRY_SPECIAL_ENABLE))))
                {
                    read_retry_option &= READ_RETRY_ENABLE;
                    read_retry_option |= FAST_PLL_RETRY_SPECIAL_ENABLE;

                    read_retry_status &= FAST_PLL_RETRY_SPECIAL_ENABLE;
                    read_retry_status |= FAST_PLL_RETRY_SPECIAL_ENABLE;
                }
                else
                {
                    read_retry_option &= TESPT_RETRY_DISABLE;
                    read_retry_status &= TESPT_RETRY_DISABLE;

                    read_retry_option &= FAST_PLL_RETRY_SPECIAL_DISABLE;
                    read_retry_status &= FAST_PLL_RETRY_SPECIAL_DISABLE;
                }
#if (EN_ADEQ == 1)
                if (((read_retry_option&FAST_PLL_RETRY_SPECIAL_ENABLE) == FAST_PLL_RETRY_SPECIAL_ENABLE)
                    || ((read_retry_option&TESPT_RETRY_ENABLE) == TESPT_RETRY_ENABLE) || (read_retry > 8))
                {
                    InitAdaptEq();
                    Disable_ADEQ = 1;
                    WRITE_FIELD(HAL_DFE_ADQCFBSEL, 0);
                    tap_temp[0] = READ_FIELD(HAL_DFE_ADQCFBPRE);
                    WRITE_FIELD(HAL_DFE_ADQCFBSEL, 1);
                    tap_temp[1] = READ_FIELD(HAL_DFE_ADQCFBPRE);
                    WRITE_FIELD(HAL_DFE_ADQCFBSEL, 2);
                    tap_temp[2] = READ_FIELD(HAL_DFE_ADQCFBPRE);
                    WRITE_FIELD(HAL_DFE_ADQCFBSEL, 3);
                    tap_temp[3] = READ_FIELD(HAL_DFE_ADQCFBPRE);
                    WRITE_FIELD(HAL_DFE_ADQCFBSEL, 4);
                    tap_temp[4] = READ_FIELD(HAL_DFE_ADQCFBPRE);
                    SendMsgCnB(SHOW_DEBUG_MSG,2,0x320A22+READ_FIELD(HAL_DFE_ADEQDIS),B2B(READ_FIELD(HAL_DFE_ADQLKCOEF)));
                    //20101022_02R0 ==start== modify message frank modify frank confirm
                    SendMsgCnB(SHOW_DEBUG_MSG,11,0x320A24,B1B(tap_temp[0]),B1B(tap_temp[1]),B1B(tap_temp[2]),B1B(tap_temp[3]),B1B(tap_temp[4]),B4B(READ_FIELD(HAL_DFE_INTEGRAT)),B2B(svo_bReadZoneSpeedX10));
                    //20101022_02R0 ==end== modify message frank modify frank confirm
                }
#endif
#if (DVD_RETRY_USE_DPD == 1)
                if (((svoVar_discKindT.fDiscKind == eDVDMinusRDLDisc) || (svoVar_discKindT.fDiscKind == eDVDMinusRDisc) || (svoVar_discKindT.fDiscKind == eDVDPlusRDLDisc)) &&
                    (pdbGetDiscStatus() == eFinalizedDisc) /*&& (!(svoCalVar.calDoneBits & (L1_blank_det | L0_blank_det)))*/ && ((PO_UN_C == 1111) || (PO_UN_C == 4444)
                    ||(((PI_C >= 750) ||(PI_E > 750)) && ((read_retry_option&TE_DPD_RETRY_ENABLE) == TE_DPD_RETRY_ENABLE))))
                {
                    if ((svoVar_TrackingMethod != TRK_METHOD_DPD) && (svoCalDPPDPDVar[Layer_Index].dpd_gn_cal_err == eNOERR))//20101210_02Z7 == If DPD TEGNOS calibration got error during startup, don't do DPD_TE_read_retry.
                    {
                        read_retry_option |= TE_DPD_RETRY_ENABLE;
                        read_retry_status |= TE_DPD_RETRY_ENABLE;
                        SetEqRegsForASpeed(LAL_DetectDiscKind(svoVar_discKindT), svoIramVar.svoCurrentSpeed);
                    }
                }
                else
                {
                    read_retry_option &= TE_DPD_RETRY_DISABLE;
                    read_retry_status &= TE_DPD_RETRY_DISABLE;
                }
#endif

                //Add unhold RFAGC tp prevent RFpp to be locked in small range.
                if((((BYTE)READ_REG(HAL_DFEREG_MAP->HFDETS)) == 0x03) || (read_retry > 8))
                {
                    if (read_retry & 0x03)
                    {
                        HAL_DFE_LoadCalRfAgcAoc();
                        WRITE_FIELD(HAL_DFE_DISHLDAGC, 1);
                    }
                    else
                    {
                        WRITE_FIELD(HAL_DFE_DISHLDAGC, 0);
                    }
                }
                else
                {
                    WRITE_FIELD(HAL_DFE_DISHLDAGC, 0);
                }
            }

            //send_msg5SValue(1, "retryP", 2, read_retry_option);
            //send_msg5SValue(1, "retryS", 2, read_retry_status);
            //send_msg5SValue(1, "retryT", 4, ReadTimer(&read_retry_restartTime));
            //20101027_02RY ==start== Do read retry reset before write action
            if (((read_retry_status != READ_RETRY_ALL_OFF)&&((read_retry_option&READ_RETRY_ENABLE)!=READ_RETRY_ENABLE))
                ||(((read_retry_option&READ_RETRY_TIMER_ENABLE)==READ_RETRY_TIMER_ENABLE)&&(ReadTimer(&read_retry_restartTime) >40000L))
                ||((read_retry_option&READ_RETRY_RESET_ENABLE)==READ_RETRY_RESET_ENABLE))
            {
                if (((read_retry_option&SERVO_DEFECT_RETRY_ENABLE)==SERVO_DEFECT_RETRY_ENABLE)
                    || ((read_retry_option&READ_RETRY_RESET_ENABLE)==READ_RETRY_RESET_ENABLE))
                {
                    Defect_Enable(DEFECT_SOURCE_UNFREEZE);
                }
        #if (DROOP_RATE_DECREASE == 1)
                if (((read_retry_option&HF_DEFECT_RETRY_ENABLE)==HF_DEFECT_RETRY_ENABLE)
                    || ((read_retry_option&READ_RETRY_RESET_ENABLE)==READ_RETRY_RESET_ENABLE))
                {
                    LAL_WriteCoef(DFE_HFDETCFG1_DECFBWPA);
                    LAL_WriteCoef(DFE_HFDETCFG2_DECFBWIA);
                }
        #endif

                if (((read_retry_option&DPDGN_RETRY_ENABLE)==DPDGN_RETRY_ENABLE)
                    || ((read_retry_option&READ_RETRY_RESET_ENABLE)==READ_RETRY_RESET_ENABLE))
                {
                    WRITE_FIELD(HAL_AFE_DPDGN,svoCalGnT3Var[Layer_Index][DPD_GN].normal_gain);
        #if (CHIP_REV >= 0xC0)
                    //update DPDGN1~4
                    READ_MOD_WRITE(HAL_AFEREG_MAP->RF07,HAL_AFE_DPDGN1SEL|HAL_AFE_DPDGN2SEL,HAL_AFE_DPDGN1SEL|HAL_AFE_DPDGN2SEL);
                    READ_MOD_WRITE(HAL_AFEREG_MAP->RF09,HAL_AFE_DPDGN3SEL|HAL_AFE_DPDGN4SEL,HAL_AFE_DPDGN3SEL|HAL_AFE_DPDGN4SEL);
                    READ_MOD_WRITE(HAL_AFEREG_MAP->RF07,HAL_AFE_DPDGN1SEL|HAL_AFE_DPDGN2SEL,0);
                    READ_MOD_WRITE(HAL_AFEREG_MAP->RF09,HAL_AFE_DPDGN3SEL|HAL_AFE_DPDGN4SEL,0);
        #endif
                }

                if (fgNormalValueSaved == TRUE)
                {
                    for (i=0;i<Smart_Retry_Para_NO;i++)
                    {
                        if (Smart_Retry_Info[i].reg_length == 4)
                        {
                            READ_MOD_WRITE(*(ULONG *)(Smart_Retry_Info[i].addr),
                                                     (Smart_Retry_Info[i].mask),
                                                     (SmartRtyNormalValue[i] << (Smart_Retry_Info[i].start_bit)));
                        }
                        else
                        {
                            READ_MOD_WRITE(*(BYTE *)(Smart_Retry_Info[i].addr),
                                                    (Smart_Retry_Info[i].mask),
                                                    (SmartRtyNormalValue[i] << (Smart_Retry_Info[i].start_bit)));
                        }
                    }
                }

        #if (EN_ADEQ == 1)
                if (((read_retry_option&ADEQ_RETRY_ENABLE)==ADEQ_RETRY_ENABLE)
                    || ((read_retry_option&READ_RETRY_RESET_ENABLE)==READ_RETRY_RESET_ENABLE))
        #endif
                {
                    InitAdaptEq();
                }

                if (((read_retry_option&FAST_PLL_RETRY_SPECIAL_ENABLE)==FAST_PLL_RETRY_SPECIAL_ENABLE)
                    || ((read_retry_option&READ_RETRY_RESET_ENABLE)==READ_RETRY_RESET_ENABLE))
                {
                    TryDFE_Param=TryDFE_Param&0xFE;
                    LAL_WriteCoef(DFE_HFFPLLCFG_FPLLKPCFG);
                    LAL_WriteCoef(DFE_HFFPLLCFG_FPLLKICFG);
                }

                if (((read_retry_option&TESPT_RETRY_ENABLE)==TESPT_RETRY_ENABLE)
                    || ((read_retry_option&READ_RETRY_RESET_ENABLE)==READ_RETRY_RESET_ENABLE))
                {
                    calSetMisc(0, TE_SPT, 0); /* write tracking Offset Data data */
                    calSetMisc(1, TE_SPT, 0); /* write tracking Offset Data data */
                    apply_TESPT(svoCalMisc1Var[Layer_Index].teSetPoint);
                }

                if (((read_retry_option&TE_DPD_RETRY_ENABLE)==TE_DPD_RETRY_ENABLE)
                    || ((read_retry_option&READ_RETRY_RESET_ENABLE)==READ_RETRY_RESET_ENABLE))
                {
                    if (svoVar_TrackingMethod == TRK_METHOD_DPD)
                    {
                        read_retry_option &= TE_DPD_RETRY_DISABLE;
                        read_retry_status &= TE_DPD_RETRY_DISABLE;
                        SetEqRegsForASpeed(LAL_DetectDiscKind(svoVar_discKindT), svoIramVar.svoCurrentSpeed);
                    }
                }

                read_retry_option = READ_RETRY_ALL_OFF;
                read_retry_status = READ_RETRY_ALL_OFF;
                read_retry_restartTime= 0;
                read_retry = 0;
                dfe_retry = 0;
                FEBC_retry = 0;
                Tilt_retry = 0;
                BufferErrorForRetry = 0;
                WRITE_FIELD(HAL_DMOD_CDVCOLKSP, halDMOD_SlowConfOutLock);
                SendMsg80(SHOW_DEBUG_MSG,0x33FF30); //StopSmartRetry
                return (SEQ_EX1);
            }
            //20101027_02RY ==end== Do read retry reset before write action
            else if ((read_retry_option&READ_RETRY_ENABLE)==READ_RETRY_ENABLE)
            {
                read_retry++;
                // SmartRetry: %u, Rtime: %4d
                SendMsgCnB(SHOW_DEBUG_MSG,5,0x320A06,B1B(read_retry),B4B(ReadTimer(&read_retry_restartTime)));

        #if (DROOP_RATE_DECREASE == 1)
                if ((read_retry_option&HF_DEFECT_RETRY_ENABLE)==HF_DEFECT_RETRY_ENABLE)
                {
                    retry__DECFBWPA=READ_FIELD(HAL_DFE_DECFBWPA)+1;
                    retry__DECFBWIA=READ_FIELD(HAL_DFE_DECFBWIA)+1;

                    if (retry__DECFBWPA>=0x0F)
                        retry__DECFBWPA = 0x0F;
                    if (retry__DECFBWIA>=0x0F)
                        retry__DECFBWIA = 0x0F;

                    WRITE_FIELD(HAL_DFE_DECFBWPA,retry__DECFBWPA);
                    WRITE_FIELD(HAL_DFE_DECFBWIA,retry__DECFBWIA);
                    // ATTBWPA: %02X, DECFBWPA: %02X, DECFBWIA: %02X
                    SendMsgCn(SHOW_DEBUG_MSG,3,0x320A03,READ_FIELD(HAL_DFE_ATTFBWPA),READ_FIELD(HAL_DFE_DECFBWPA),READ_FIELD(HAL_DFE_DECFBWIA));
                    read_retry_status |= HF_DEFECT_RETRY_ENABLE;
                }
        #endif

                /* increase DPDGN */
                if ((read_retry_option&DPDGN_RETRY_ENABLE)==DPDGN_RETRY_ENABLE)
                {
                    if (read_retry<5)
                    {
                        temp_DPDGN = READ_FIELD(HAL_AFE_DPDGN)+1;
                        if (temp_DPDGN>0x0F)
                            temp_DPDGN = 0x0F;

                        WRITE_FIELD(HAL_AFE_DPDGN, temp_DPDGN);
        #if (CHIP_REV >= 0xC0)
                        //update DPDGN1~4
                        READ_MOD_WRITE(HAL_AFEREG_MAP->RF07,HAL_AFE_DPDGN1SEL|HAL_AFE_DPDGN2SEL,HAL_AFE_DPDGN1SEL|HAL_AFE_DPDGN2SEL);
                        READ_MOD_WRITE(HAL_AFEREG_MAP->RF09,HAL_AFE_DPDGN3SEL|HAL_AFE_DPDGN4SEL,HAL_AFE_DPDGN3SEL|HAL_AFE_DPDGN4SEL);
                        READ_MOD_WRITE(HAL_AFEREG_MAP->RF07,HAL_AFE_DPDGN1SEL|HAL_AFE_DPDGN2SEL,0);
                        READ_MOD_WRITE(HAL_AFEREG_MAP->RF09,HAL_AFE_DPDGN3SEL|HAL_AFE_DPDGN4SEL,0);
        #endif
                        SendMsgCn(SHOW_DEBUG_MSG,1,0x320A02,READ_FIELD(HAL_AFE_DPDGN)); // DPDGN: %02X
                    }
                    read_retry_status |= DPDGN_RETRY_ENABLE;
                }

                /* Change DFE read parameter */
                if ((read_retry_option&DFE_RETRY_ENABLE)==DFE_RETRY_ENABLE)
                {
                    //send_msg5SValue(1, "V-save", 1, fgNormalValueSaved);
                    //send_msg5SValue(1, "V-chg", 1, fgNormalValueChgAgain);
                    dfe_retry++;
                    if (dfe_retry > MaxRetryNR)
                        dfe_retry = 1;
                    SendMsgCnB(SHOW_DEBUG_MSG,1,0x33FF5E,B1B(dfe_retry));
                    if (fgNormalValueSaved == FALSE)//save original value
                    {
                        for (i = 0; i < Smart_Retry_Para_NO; i++)
                        {
                            if (Smart_Retry_Info[i].reg_length == 4)
                            {
                                SmartRtyNormalValue[i] = ((READ_ADR_4B(Smart_Retry_Info[i].addr))&Smart_Retry_Info[i].mask)>>Smart_Retry_Info[i].start_bit;
                            }
                            else
                            {
                                SmartRtyNormalValue[i] = ((READ_ADR_1B(Smart_Retry_Info[i].addr))&Smart_Retry_Info[i].mask)>>Smart_Retry_Info[i].start_bit;
                            }
                        }
                        SendMsgCnB(SHOW_DEBUG_MSG,8,0x33FF5F,B1B(SmartRtyNormalValue[0]),B1B(SmartRtyNormalValue[1]),B1B(SmartRtyNormalValue[2]),B1B(SmartRtyNormalValue[3]),
                                            B1B(SmartRtyNormalValue[4]),B1B(SmartRtyNormalValue[5]),B1B(SmartRtyNormalValue[6]),B1B(SmartRtyNormalValue[7]));
                        SendMsgCnB(SHOW_DEBUG_MSG,4,0x33FF75,B1B(SmartRtyNormalValue[8]),B1B(SmartRtyNormalValue[9]),B1B(SmartRtyNormalValue[10]),B1B(SmartRtyNormalValue[11]));
                        fgNormalValueSaved = TRUE;
                    }
                    else if (fgNormalValueChgAgain == TRUE)//save original value after speed change
                    {
                        for (i = 0; i < Smart_Retry_Para_NO; i++)
                        {
                            if (Smart_Retry_Info[i].updated_after_spdchg == 1)
                            {
                                if (Smart_Retry_Info[i].reg_length == 4)
                                {
                                    SmartRtyNormalValue[i] = ((READ_ADR_4B(Smart_Retry_Info[i].addr))&Smart_Retry_Info[i].mask)>>Smart_Retry_Info[i].start_bit;
                                }
                                else
                                {
                                    SmartRtyNormalValue[i] = ((READ_ADR_1B(Smart_Retry_Info[i].addr))&Smart_Retry_Info[i].mask)>>Smart_Retry_Info[i].start_bit;
                                }
                            }
                        }
                        SendMsgCnB(SHOW_DEBUG_MSG,8,0x33FF60,B1B(SmartRtyNormalValue[0]),B1B(SmartRtyNormalValue[1]),B1B(SmartRtyNormalValue[2]),B1B(SmartRtyNormalValue[3]),
                                            B1B(SmartRtyNormalValue[4]),B1B(SmartRtyNormalValue[5]),B1B(SmartRtyNormalValue[6]),B1B(SmartRtyNormalValue[7]));
                        SendMsgCnB(SHOW_DEBUG_MSG,4,0x33FF76,B1B(SmartRtyNormalValue[8]),B1B(SmartRtyNormalValue[9]),B1B(SmartRtyNormalValue[10]),B1B(SmartRtyNormalValue[11]));
                        fgNormalValueChgAgain = FALSE;
                    }

                    ptrRetryPara = &Smart_Retry_Para_DVD[dfe_retry-1][0];

                    for (i = 0; i < Smart_Retry_Para_NO; i++)
                    {
                        SmartRtyNewValue[i] = (SmartRtyNormalValue[i]+(*(SBYTE *)(ptrRetryPara+i)));
                        if (SmartRtyNewValue[i] > (LONG)Smart_Retry_Info[i].max_value)
                            SmartRtyNewValue[i] = Smart_Retry_Info[i].max_value;
                        else if (SmartRtyNewValue[i] < (LONG)Smart_Retry_Info[i].min_value)
                            SmartRtyNewValue[i] = Smart_Retry_Info[i].min_value;

                        if ((*(SBYTE *)(ptrRetryPara+i)) != 0)
                        {
                            if (Smart_Retry_Info[i].reg_length == 4)
                            {
                                READ_MOD_WRITE(*(ULONG *)(Smart_Retry_Info[i].addr),
                                                           (Smart_Retry_Info[i].mask),
                                                           (SmartRtyNewValue[i] << (Smart_Retry_Info[i].start_bit)));
                            }
                            else
                            {
                                READ_MOD_WRITE(*(BYTE *)(Smart_Retry_Info[i].addr),
                                                           (Smart_Retry_Info[i].mask),
                                                           (SmartRtyNewValue[i] << (Smart_Retry_Info[i].start_bit)));
                            }
                            WRITE_FIELD(HAL_DFE_ADEQPRGOLD, 1);
                        }
                    }
                    SendMsgCnB(SHOW_DEBUG_MSG,8,0x33FF61,B1B(SmartRtyNewValue[0]),B1B(SmartRtyNewValue[1]),B1B(SmartRtyNewValue[2]),B1B(SmartRtyNewValue[3]),
                                        B1B(SmartRtyNewValue[4]),B1B(SmartRtyNewValue[5]),B1B(SmartRtyNewValue[6]),B1B(SmartRtyNewValue[7]));
                    SendMsgCnB(SHOW_DEBUG_MSG,4,0x33FF75,B1B(SmartRtyNewValue[8]),B1B(SmartRtyNewValue[9]),B1B(SmartRtyNewValue[10]),B1B(SmartRtyNewValue[11]));
                    read_retry_status |= DFE_RETRY_ENABLE;
                }

                if (((read_retry_option&FEBC_RETRY_ENABLE)==FEBC_RETRY_ENABLE) && (FEBC_retry < FebcRetryNR))
                {
                    FEBC_retry++;

                    if (FEBC_retry == 1)
                    {
                        currZone=calRadius2Zone(Iram_stp_CurrentPosition);
                        initFEBC = svoCalFebcVar[1][currZone].opt_value;
                        svoCalFebcVar[1][currZone].opt_value += 0x02;

                        SendMsgCnB(SHOW_DEBUG_MSG,8,0x33FF72,B2B(initFEBC),B1B(currZone));
                    }
                    else if ((FEBC_retry > 1) && (FEBC_retry < FebcRetryNR))
                    {
                        svoCalFebcVar[1][currZone].opt_value = (FEBC_retry%2) ? (svoCalFebcVar[1][currZone].opt_value + (FEBC_retry+FEBC_retry)) : (svoCalFebcVar[1][currZone].opt_value - (FEBC_retry+FEBC_retry));
                    }
                    else if (FEBC_retry == FebcRetryNR)
                    {
                        svoCalFebcVar[1][currZone].opt_value = initFEBC;
                    }
                    SendMsgCnB(SHOW_DEBUG_MSG,8,0x33FF69,B1B(FEBC_retry),B2B(svoCalFebcVar[1][currZone].opt_value));
                    read_retry_status |= FEBC_RETRY_ENABLE;
                }

                // DVDRW and DVDRDL enable ADEQ when read retry
                /* Change ADEQ setting */
        #if (EN_ADEQ == 1)
                if ((read_retry_option&ADEQ_RETRY_ENABLE)==ADEQ_RETRY_ENABLE)
                {
                    if ((!(READ_FIELD(HAL_DFE_ADEQDIS))) && ((svoVar_discKindT.Bit.Media== eMediaR) || (svoVar_discKindT.Bit.Media== eMediaRW)))
                    {
                        if ((read_retry >=1) && (read_retry <=3))
                        {
                            WRITE_FIELD(HAL_DFE_ADQLKCOEF, 520);
                        }
                        else if ((read_retry >=4) && (read_retry <=7))
                        {
                            WRITE_FIELD(HAL_DFE_ADQLKCOEF, 128);
                        }
                        else if ((read_retry >=8) && ((read_retry%3) == 0))
                        {
                            InitAdaptEq();
                            Disable_ADEQ = 1;//ADEQ off
                        }
                    }
                    else if ((READ_FIELD(HAL_DFE_ADEQDIS)) && ((svoVar_discKindT.Bit.Media== eMediaR) || (svoVar_discKindT.Bit.Media== eMediaRW))
                        && (read_retry >10) && (!DVDRAM) )
                    {
                        Disable_ADEQ = 0;//ADEQ on
                    }
                    SendMsgCnB(SHOW_DEBUG_MSG,2,0x320A22+(Disable_ADEQ & 0x01),B2B(READ_FIELD(HAL_DFE_ADQLKCOEF)));
                    read_retry_status |= ADEQ_RETRY_ENABLE;
                }
        #endif // #if (EN_ADEQ == 1)

                /* Change HF SLOW PLL setting */
                // == start ==Set HF PLL mode to be ForceInLockAid mode when DVD ready retry
        #if (DVD_RAM_READ == 1)
                if (!DVDRAM)
        #endif
                {
                    if (read_retry > 8)
                    {
                        if ((read_retry % 2) == 0)
                        {
                            read_retry_option &= (SLOW_PLL_RETRY_FORCE_LOCK_DISABLE);
                            read_retry_status &= (SLOW_PLL_RETRY_FORCE_LOCK_DISABLE);
                        }
                        else
                        {
                            read_retry_option |= SLOW_PLL_RETRY_FORCE_LOCK_ENABLE;
                            read_retry_status |= SLOW_PLL_RETRY_FORCE_LOCK_ENABLE;
                        }
                    }
                }
                // == end ==Set HF PLL mode to be ForceInLockAid mode when DVD ready retry

                /* Special case for changing FAST PLL setting */
                if ((read_retry_option&FAST_PLL_RETRY_SPECIAL_ENABLE) == FAST_PLL_RETRY_SPECIAL_ENABLE)
                {
                    if ((read_retry >=1) && (read_retry <=4))
                    {
                        if (COEF_GetValue(DFE_HFFPLLCFG_FPLLKICFG) >= 5)
                        {
                            Ki=4;
                        }
                        else
                        {
                            Ki=5;
                        }
                        Kp=3;
                        WRITE_FIELD(HAL_DMOD_CDVCOLKSP, halDMOD_FastConfOutLock);
                        TryDFE_Param=TryDFE_Param|0x01;
                    }
                    else if ((read_retry >=5) && (read_retry <=9))
                    {
                        Ki=3;
                        Kp=3;
                        TryDFE_Param=TryDFE_Param&0xFE;
                    }
                    else if ((read_retry >=10) && (read_retry <=12))
                    {
                        Ki=4;
                        Kp=4;
                        TryDFE_Param=TryDFE_Param&0xFE;
                    }
                    else
                    {
                        WRITE_FIELD(HAL_DMOD_CDVCOLKSP, halDMOD_SlowConfOutLock);
                        Ki=COEF_GetValue(DFE_HFFPLLCFG_FPLLKICFG);
                        Kp=3;
                        TryDFE_Param=TryDFE_Param&0xFE;
                        read_retry =0;
                    }
                    WRITE_FIELD(HAL_DFE_FPLLKPCFG,Kp);
                    WRITE_FIELD(HAL_DFE_FPLLKICFG,Ki);
                    SendMsgCnB(SHOW_DEBUG_MSG,2,0x33FF5D,B1B(Kp),B1B(Ki));

                }
            }

            /* Special case for changing TE set point */
            if ((read_retry_option&TESPT_RETRY_ENABLE) == TESPT_RETRY_ENABLE)
            {
                if ((read_retry >=1) && (read_retry <=3))
                {
                    calSetMisc(Layer_Index, TE_SPT, 50); /* write tracking Offset Data data */
                    apply_TESPT(svoCalMisc1Var[Layer_Index].teSetPoint);//20101027_02RY == Fix some code TYPO issue.
                }
                else if ((read_retry >=4) && (read_retry <=7))
                {
                    calSetMisc(Layer_Index, TE_SPT, 70); /* write tracking Offset Data data */
                    apply_TESPT(svoCalMisc1Var[Layer_Index].teSetPoint);//20101027_02RY == Fix some code TYPO issue.
                    SpeedCount +=2 ;
                }
                else if ((read_retry >=8) && (read_retry <=9))
                {
                    calSetMisc(Layer_Index, TE_SPT, 20); /* write tracking Offset Data data */
                    apply_TESPT(svoCalMisc1Var[Layer_Index].teSetPoint);//20101027_02RY == Fix some code TYPO issue.
                    SpeedCount ++ ;
                }
                else
                {
                    calSetMisc(Layer_Index, TE_SPT, 0); /* write tracking Offset Data data */
                    apply_TESPT(svoCalMisc1Var[Layer_Index].teSetPoint);//20101027_02RY == Fix some code TYPO issue.
                    read_retry = 0;
                }
            }
            break;

        default:
            break;
    }
    return(SEQ_EX0);
}
#endif // HORROR_DISC_IMPROVEMENT

/*-----------------------------------------------------------------------------------------------------
    Name:           state_AFE_InputGainCal()

    Description:    Input Gain Calibration

    Inputs:         None
    Outputs:        None
    Global Inputs:  SRFO signal amplitude (svoPrvtVar_maxSRFO)
    Global Outputs: None
    Returns:        SHORT
-----------------------------------------------------------------------------------------------------*/
seq_exit_t state_AFE_InputGainCal(void)
{
    SHORT SRFOGaindBX10,orig_pdic_gn,orig_lasc_mode;

    if (Layer_Index == 1)
    {
        return(SEQ_EX0);
    }

    HAL_LASC_SetLaserOn(TRUE);

    /* check if the value of svoPrvtVar_maxSRFO is not equal to zero */
    if (svoPrvtVar_maxSRFO == 0)
    {
        svoPrvtVar_maxSRFO = 1;//prevent divided by 0
        /* svoPrvtVar_maxSRFO is NULL!!!! */
        SendMsg80(SHOW_DEBUG_MSG,0x3B0100); // Gain Calibration Error: SRFO_peak is NULL!
    }
    orig_lasc_mode = svoVar_bLascMode;
    orig_pdic_gn = svoCalMisc2Var.pdic_gn;

#if (PDIC_BASE_ON_SRFO == 1)
    LAL_LASC_GetPdic(svoPrvtVar_maxSRFO, svoRampMeasVar.SRFO_gain - svoCalMisc2Var.laser_gn, (svoCalVar.calGainDoneBits & pdic_end) == pdic_end);
#endif
    if (orig_lasc_mode != svoVar_bLascMode)
    {
        HAL_LASC_SetLaserOn(FALSE);
        HAL_LASC_SetLaserOn(TRUE);
    }
    SRFOGaindBX10 = svoRampMeasVar.SRFO_gain - svoCalMisc2Var.pdic_gn - svoCalMisc2Var.laser_gn;

    /* Calculate Main/Sub Beam Input Gain */
    LAL_AFE_GetMbSbInpValues(svoPrvtVar_maxSRFO, SRFOGaindBX10);
    SendMsgCn(DEBUG_CALIB_RESULT_MSG,2,0x310550+Layer_Index,svoCalGnT2Var[Layer_Index][MB_GN].normal_gain,svoCalGnT2Var[Layer_Index][SB_GN].normal_gain);

    if (ARG1 == CALIB_ALL)
    {
        svoCalVar.calGainDoneBits |= pdic_end;

        /* Calculate DPD Input Gain */
        LAL_AFE_GetDPDInpValues(0, svoPrvtVar_avgSRFO[0], SRFOGaindBX10);
        if (svoPrvtVar_avgSRFO[1] > 0)
        {
            LAL_AFE_GetDPDInpValues(1, svoPrvtVar_avgSRFO[1], SRFOGaindBX10);
        }
        else
        {
            svoCalGnT3Var[1][DPD_GN] = svoCalGnT3Var[0][DPD_GN];
        }

        /* Calculate Wobble Input Gain */
        LAL_AFE_GetWobbleValues(svoPrvtVar_maxSRFO, SRFOGaindBX10);

        /* Calculate RF input 5V buffer gain selection and RF Coarse gain */
        LAL_AFE_GetRFInpValues(svoPrvtVar_avgSRFO[0], svoPrvtVar_avgSRFO[1], SRFOGaindBX10);

        // <Lx> DPD_GN = %02Xh (%4.1f dB), RFIN_GN = %02Xh (%2d dB), RF_GN = %02Xh (%2d dB)
        SendMsgCnB(DEBUG_CALIB_RESULT_MSG,9,0x310580+Layer_Index,
            B1B(svoCalGnT3Var[Layer_Index][DPD_GN].normal_gain),F4B((svoCalGnT3Var[Layer_Index][DPD_GN].normal_gain-4)*1.5),
            B1B(svoCalGnT3Var[Layer_Index][RFIN_GN].normal_gain),B1B(AFE_RFIN_GN[svoCalGnT3Var[Layer_Index][RFIN_GN].normal_gain]),
            B1B(svoCalGnT2Var[Layer_Index][RF_GN].normal_gain),B1B(AFE_RF_GN[svoCalGnT2Var[Layer_Index][RF_GN].normal_gain]));

        SendMsgCn(DEBUG_DISK_ID_MSG,4,0x310583,READ_FIELD(HAL_AFE_RADGNB),READ_FIELD(HAL_AFE_RADPGNB),READ_FIELD(HAL_AFE_RBCGNB),READ_FIELD(HAL_AFE_RBCPGNB));
    }
    /* Set the next sequence */
    if ((orig_pdic_gn == svoCalMisc2Var.pdic_gn) && (orig_lasc_mode == svoVar_bLascMode))
    {
        return SEQ_EX0;
    }
    else
    {
        svoCalVar.calGainDoneBits &= (~feg_bsg_end);
        svoCalVar.calOfsDoneBits &= (~havcof_end);
        return SEQ_EX1;
    }
}

/*------------------------------------------------------------------------*/
/* The end of file                                                        */
/*------------------------------------------------------------------------*/
