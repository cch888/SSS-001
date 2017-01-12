/****************************************************************************
*              (c) Copyright 2001 - 2010 Sunext Technology Co., Ltd.
*                           All Rights Reserved
*
*
* FILENAME:   svo_speed.c
*
* DESCRIPTION: This file contains the functions related to spindle/speed control.
*
*
* NOTES:       Functions in this file
*              -- spdChangeDiscSpeed()
*              -- spdSetFGFreq()
*              -- spdGetTargetOverspeedX100()
*
*   $Revision: 644 $
*   $Date: 11/03/29 9:41p $
*
**************************** SUNEXT CONFIDENTIAL ****************************/
#include ".\common\globals.h"
#include ".\common\globtype.h"
#include ".\common\chip.h"
#include ".\common\commath.h"
#include ".\common\com_im.h"
#include ".\hif\diag.h"
#include ".\player\plrdb.h"
#include ".\servo\svo.h"
#include ".\servo\svomacro.h"
#include ".\servo\svo_com.h"
#include ".\servo\svo_cmd.h"
#include ".\servo\svo_tbl.h"
#include ".\servo\svo_step.h"
#include ".\servo\svoVar.h"
#include ".\servo\svo_speed.h"
#include ".\servo\svo_lib.h"

#include ".\servo\dvSvo_tbl.h"
#if (DVD_RAM_READ == 1)
#include ".\servo\dvRamSvo_tbl.h"
#endif
#include ".\servo\svo_calib.h"
#include ".\servo\svo_seek.h"
#if (DVD_RAM_READ == 1)
#include ".\defmgmt\dvdram\dvDefMgr.h"
#include ".\defmgmt\common\mrw_ram.h"
#include ".\al\HAL_dfe.h"
#include ".\al\HAL_wobble.h"
#endif
#include ".\al\REG_servo.h"
#include ".\AL\HAL_pll.h"
#include ".\al\coefs.h"
#include ".\al\lal.h"
#include ".\servo\svo_dsp.h"
#include ".\servo\svo_spdtrans.h"

static USHORT last_rpm;
static SHORT  last_kd14;

/*-----------------------------------------------------------------------------
 *
 * Module: svoServoSpeedInit()
 * Description:
 *
 *----------------------------------------------------------------------------*/
void SpdInit(void)
{
    svoSpeedVar.SpdOverspd = 0;
    svoSpeedVar.SpdOldRevcnt = 0;
    svoSpeedVar.SpdOldRevcntChk = TRUE;
#if (NEW_SPINDLE_CONTROL == 1)
    svoSpeedVar.SpdDE4Ratio = 128;
    svoSpeedVar.SpdIntPara3 = SPD_INT_PARA_3;
#endif
    spdStop();
}

/*-----------------------------------------------------------------------------
 *
 * Module: spdStop()
 * Description:
 *
 *----------------------------------------------------------------------------*/
void spdStop(void)
{
    svoIramVar.svoCurrentSpeed = 0xFF;
    svoSpeedVar.SpdCurrentSpeed = 0xFF;
    svoSpeedVar.SpdMode = SPD_MODE_CAV;
    svoSpeedVar.SpdHold = FALSE;
    svoSpeedVar.SpdSetMaxRPM = FALSE;
    svoSpeedVar.SpdInRange = TRUE;
    svoSpeedVar.SpdMonitor = FALSE;
    svoSpeedVar.SpdUp = FALSE;
    svoSpeedVar.SpdDown = FALSE;
    svoSpeedVar.SpdPresetNeeded = TRUE;
    svoSpeedVar.CurrentRpm = 0;
    svoSpeedVar.TargetRpm = 0;
#if (ACC_CONTROL > 0) || (DEC_CONTROL > 0)
    svoSpeedVar.TargetRpmReal = 0;
#endif
    svoSpeedVar.SpdOverspd = 0;
#if (SPD_MAX_BRAKE_RPM > 0)
    svoSpeedVar.SpdFreeRun = FALSE;
#endif
    last_rpm = 0;
    last_kd14 = DMO_CLIP;
}

/*-----------------------------------------------------------------------------
 *
 * Module: spdChangeDiscSpeed()
 * Description:
 *
 *----------------------------------------------------------------------------*/
#if((CD_UNBALANCE_DISC_CHECK == 1)||(DVD_UNBALANCE_DISC_CHECK == 1)||(BD_UNBALANCE_DISC_CHECK == 1))
#define Unb_chk_mask    0x03
#endif // ((CD_UNBALANCE_DISC_CHECK == 1)||(DVD_UNBALANCE_DISC_CHECK == 1)||(BD_UNBALANCE_DISC_CHECK == 1))

void spdChangeDiscSpeed(ESpindleSpeed speed)
{
    ULONG  temp_ulong;
    USHORT target_position;
    BOOL   tracking_status;
#if((CD_UNBALANCE_DISC_CHECK == 1)||(DVD_UNBALANCE_DISC_CHECK == 1)||(BD_UNBALANCE_DISC_CHECK == 1))
    ad_dsp_t ce_signal;
#endif

    if (svoSpeedVar.SpdMonitor)
    {
        spdMonitorRPM();
        svoSpeedVar.SpdMonitor = FALSE;
    }
    if ((svoSpeedVar.SpdDown) && (svoSpeedVar.SpdInRange))
    {
        svoSpeedVar.SpdDown = FALSE;
        spdSetPara();
        SendMsgCnB(SHOW_DEBUG_MSG,2,0x33102E,B2B(svoSpeedVar.SpdWaitCnt * 20)); //***speed in range (down): %d (mS)***
    }

#if (DVD_RAM_READ == 1)
    if (DVDRAM_embossed)
        return; //avoid RAM settings in embossed area
#endif

    if ((svoSpeedVar.SpdCurrentSpeed != speed) || (bSetSpeedAgain == TRUE))
    {
        svoSpeedVar.SpdSetMaxRPM = FALSE;
        svoSpeedVar.SpdHold = FALSE;
#if HORROR_DISC_IMPROVEMENT == 1
        if (fgNormalValueSaved == TRUE)
        {
            fgNormalValueChgAgain = TRUE;
        }
#endif

        if (svoSpeedVar.SpdCurrentSpeed == 0xFF)
        {
            SendMsg80(SHOW_DEBUG_MSG,0x330104);//Adjust_Spd, CurSpd=INIT_SPD
        }
        else
        {
            switch (svoVar_discKindT.Bit.DiscStd)
            {
                case eDiscCD:
                    SendMsgCnB(SHOW_DEBUG_MSG,4,0x330100+svoSpeedVar.SpdMode,F4B((float)SpeedIndexTableCD[svoSpeedVar.SpdCurrentSpeed]/100)); //Adjust_Spd, CurSpd=%2.1fX_?C?V
                    break;

                case eDiscDVD:
#if (DVD_RAM_READ == 1)
                    if (DVDRAM)
                    {
                        SendMsgCnB(SHOW_DEBUG_MSG,4,0x330100+svoSpeedVar.SpdMode,F4B((float)SpeedIndexTableDVDRAM[svoSpeedVar.SpdCurrentSpeed]/100)); //Adjust_Spd, CurSpd=%2.1fX_?C?V
                    }
                    else
#endif
                    {
#if (DVD_710Run815New == 1)
                        SendMsgCnB(SHOW_DEBUG_MSG,4,0x330100+svoSpeedVar.SpdMode,F4B((float)GetSpeedIndexTableDVD(svoSpeedVar.SpdCurrentSpeed)/100)); //Adjust_Spd, CurSpd=%2.1fX_?C?V
#else
                        SendMsgCnB(SHOW_DEBUG_MSG,4,0x330100+svoSpeedVar.SpdMode,F4B((float)SpeedIndexTableDVD[svoSpeedVar.SpdCurrentSpeed]/100)); //Adjust_Spd, CurSpd=%2.1fX_?C?V
#endif
                    }
                    break;

#if (BD_ENABLE == 1)
                case eDiscBD:
                    SendMsgCnB(SHOW_DEBUG_MSG,4,0x330100+svoSpeedVar.SpdMode,F4B((float)SpeedIndexTableBD[svoSpeedVar.SpdCurrentSpeed]/100)); //Adjust_Spd, CurSpd=%2.1fX_?C?V
                    break;
#endif // (BD_ENABLE == 1)

#if (ENABLE_HDDVD == 1)
                case eDiscHD:
                    SendMsgCnB(SHOW_DEBUG_MSG,4,0x330100+svoSpeedVar.SpdMode,F4B((float)SpeedIndexTableHD[svoSpeedVar.SpdCurrentSpeed]/100)); //Adjust_Spd, CurSpd=%2.1fX_?C?V
                    break;
#endif // (ENABLE_HDDVD == 1)

                default:
                    DEBUG_ASSERT(FALSE, eASSERT_IMPOSSIBLE_DISCKIND ); // Let's check here if we aren't running for a unknownDiscKind/DiscStd
                    break;
            }
        }
        svoSpeedVar.MaximumRpm = CAV_RPM_MAX;
        svoSpeedVar.CavSwitchPos = spdGetCavSwitchPos(speed,&svoSpeedVar.MaximumRpm);

        if (svoSpeedVar.CavSwitchPos == 0xFFFF)
        {
            svoSpeedVar.SpdMode = SPD_MODE_CAV;
        }
        else if (svoSpeedVar.CavSwitchPos == 0)
        {
            svoSpeedVar.SpdMode = SPD_MODE_CLV;
        }
        else
        {
            svoSpeedVar.SpdMode = SPD_MODE_PCAV;
        }

        target_position = Iram_stp_CurrentPosition;
        if (Iram_stp_Status == (STP_BUSY_MASK | STP_DEVICE_NORMAL))
        {
            if (Iram_stp_CurrentDirection == FORW)
            {
                target_position += (Iram_stp_JumpDistance - Iram_stp_CurrentDistance);
            }
            else
            {
                target_position -= (Iram_stp_JumpDistance - Iram_stp_CurrentDistance);
            }
        }

        svoSpeedVar.SpdCurrentSpeed = speed;
        svoSpeedVar.TargetRpm = spdGetTargetRPM(target_position, speed);
        temp_ulong = svoSpeedVar.CurrentRpm * 1000 / svoSpeedVar.TargetRpm;
        if ((temp_ulong < 1050) && (temp_ulong >= 950))
        {
#if (NEW_SPINDLE_CONTROL == 1)
            if (!svoSpeedVar.SpdInRange)
            {
                spdSetIntegrator(svoSpeedVar.TargetRpm);
            }
#endif
            svoSpeedVar.SpdInRange = TRUE;
            svoSpeedVar.SpdMonitor = FALSE;
            svoSpeedVar.SpdDown = FALSE;
    #if (ACC_CONTROL > 0) || (DEC_CONTROL > 0)
            svoSpeedVar.CurrentRpm = svoSpeedVar.TargetRpmReal;
    #else
            svoSpeedVar.CurrentRpm = svoSpeedVar.TargetRpm;
    #endif
            svoSpeedVar.SpdOverspd = spdGetTargetOverspeedX100(target_position,speed);
            temp_ulong = svoSpeedVar.TargetRpm;
        }
        else
        {
            svoSpeedVar.SpdInRange = FALSE;
            svoSpeedVar.SpdWaitCnt = 0;
            temp_ulong = svoSpeedVar.TargetRpm;
            if (svoSpeedVar.SpdMode != SPD_MODE_CAV)
            {
                temp_ulong = spdGetTargetRPM(UM_2_SLED_STEP(24000), speed);
            }
            if (bSetSpeedAgain || (temp_ulong >= svoSpeedVar.CurrentRpm))
            {
                svoSpeedVar.SpdDown = FALSE;
            }
            else
            {
                svoSpeedVar.SpdDown = TRUE;
            }
        }

        //update temp_ulong for CAVM calculation
        if (svoSpeedVar.SpdMode != SPD_MODE_CAV)
        {
            temp_ulong = spdGetTargetRPM(UM_2_SLED_STEP(58000), speed);
        }
        svoSpeedVar.MinimumRpm = temp_ulong;
        if ((!svoSpeedVar.SpdInRange) && (temp_ulong > svoSpeedVar.CurrentRpm))
        {
            temp_ulong = svoSpeedVar.CurrentRpm;
            svoSpeedVar.SpdUp = TRUE;
        }
        else
        {
            svoSpeedVar.SpdUp = FALSE;
        }
        spdSetCAVM(temp_ulong);

        if (!svoSpeedVar.SpdDown)
        {
            spdSetPara();
        }
        else
        {
    #if (ACC_CONTROL > 0) || (DEC_CONTROL > 0)
            svoSpeedVar.TargetRpmReal = svoSpeedVar.CurrentRpm;
    #endif
            spdSetFGFreq(svoSpeedVar.TargetRpm);
        }
        bSetSpeedAgain = FALSE;

        tracking_status = TRUE;

        if (speed == 0xFF)
        {
            SendMsg80(SHOW_DEBUG_MSG,0x330109);//Adjust_Spd, TarSpd=INIT_SPD
        }
        else
        {
            switch (svoVar_discKindT.Bit.DiscStd)
            {
                case eDiscCD:
                    SendMsgCnB(SHOW_DEBUG_MSG,4,0x330105+svoSpeedVar.SpdMode,F4B((float)SpeedIndexTableCD[speed]/100)); //Adjust_Spd, TarSpd=%2.1fX_?C?V
#if (CD_UNBALANCE_DISC_CHECK == 1)
                    if ((Disc_Speed.Read >= e32XCAV) && ((fUnbalanceChk_SpdUp & Unb_chk_mask) != Unb_chk_mask))
                    {
                        if (((!(fUnbalanceChk_SpdUp & 0x01)) && (speed == _16AX)) ||
                            ((!(fUnbalanceChk_SpdUp & 0x02)) && (speed == _24AX)))
                        {
                            if (tracking_status == TRUE)
                            {
                                tracking_status = FALSE;
                                track_off();
    #if (SRC_ENABLE == 1)
                                HAL_DSP_SRCEnable(SRC_LOOP_TRACKING, SRC_STATE_RESET); //Reset Tracking SRC
    #endif
                            }
                            spdWaitSpdInRange();
                            seekSetCEEq(eRoughSeekRatioOnly, 0);
                            HAL_SERVO_CenterControlON(FORW);
                            WaitTimer(80);

                            ce_signal = ReadDSP_CE_mV_Avg(ONE_REVOLUTION, TRUE);

                            HAL_SERVO_CenterControlOFF();

                            if ((speed == _16AX))
                            {
                                fUnbalanceChk_SpdUp |= (1<<0);
                                Unbalance_midspd = ce_signal.p2p_mv;
                            }
                            else if ((speed == _24AX))
                            {
                                fUnbalanceChk_SpdUp |= (1<<1);
                                Unbalance_highspd = ce_signal.p2p_mv;

                                if ((Unbalance_midspd+Unbalance_highspd)>1200)
                                {
                                    svoVar_unbalanceDiscNum = 2;
                                    InitializeReadSpeed();
                                }
                                SendMsgCn(DEBUG_DISK_ID_MSG,4,0x310312,A2B(Unbalance_midspd),A2B(Unbalance_highspd));
                                SendMsgCn(DEBUG_DISK_ID_MSG,1,0x310304,A1B(svoVar_unbalanceDiscNum)); //##UnbalanceDisc_Num==
                            }

                            if (!FokStatusOK())
                            {
                                fUnbalanceChk_SpdUp |= Unb_chk_mask;
                                svoVar_unbalanceDiscNum = 2;
                                InitializeReadSpeed();
                                SendMsgCn(DEBUG_CUSTOMER_MSG2,1,0x310304,A1B(svoVar_unbalanceDiscNum)); //##UnbalanceDisc_Num==
                            }
                        }
                    }
#endif
                    break;

                case eDiscDVD:
#if (DVD_RAM_READ == 1)
                    if (DVDRAM)
                    {
                        SendMsgCnB(SHOW_DEBUG_MSG,4,0x330105+svoSpeedVar.SpdMode,F4B((float)SpeedIndexTableDVDRAM[speed]/100)); //Adjust_Spd, TarSpd=%2.1fX_?C?V
                    }
                    else
#endif
                    {
#if (DVD_710Run815New == 1)
                        SendMsgCnB(SHOW_DEBUG_MSG,4,0x330105+svoSpeedVar.SpdMode,F4B((float)GetSpeedIndexTableDVD(speed)/100)); //Adjust_Spd, TarSpd=%2.1fX_?C?V
#else
                        SendMsgCnB(SHOW_DEBUG_MSG,4,0x330105+svoSpeedVar.SpdMode,F4B((float)SpeedIndexTableDVD[speed]/100)); //Adjust_Spd, TarSpd=%2.1fX_?C?V
#endif
#if (DVD_UNBALANCE_DISC_CHECK == 1)
                        if ((Disc_Speed.Read >= e8XDCAV) && ((fUnbalanceChk_SpdUp & Unb_chk_mask) != Unb_chk_mask))
                        {
                            if (((!(fUnbalanceChk_SpdUp & 0x01)) && (speed == _6AX_DVD)) ||
                                ((!(fUnbalanceChk_SpdUp & 0x02)) && (speed == _8AX_DVD)))
                            {
                                if (tracking_status == TRUE)
                                {
                                    tracking_status = FALSE;
                                    track_off();
                                }
                                spdWaitSpdInRange();
                                seekSetCEEq(eRoughSeekRatioOnly, 0);
                                HAL_SERVO_CenterControlON(FORW);
                                WaitTimer(80);

                                ce_signal = ReadDSP_CE_mV_Avg(ONE_REVOLUTION, TRUE);

                                HAL_SERVO_CenterControlOFF();

                                if ((speed == _6AX_DVD))
                                {
                                    fUnbalanceChk_SpdUp |= (1<<0);
                                    Unbalance_midspd = ce_signal.p2p_mv;
                                }
                                else if ((speed == _8AX_DVD))
                                {
                                    fUnbalanceChk_SpdUp |= (1<<1);
                                    Unbalance_highspd = ce_signal.p2p_mv;

                                    if (0)//((Unbalance_midspd + Unbalance_highspd) > 1400) //Turn off determine unbalance, because the value is too large for R disc(1400~2000). Maye have some bug.
                                    {
                                        svoVar_unbalanceDiscNum = 2;
                                        InitializeReadSpeed();
                                    }
                                    SendMsgCn(DEBUG_DISK_ID_MSG,4,0x310312,A2B(Unbalance_midspd),A2B(Unbalance_highspd));
                                    SendMsgCn(DEBUG_DISK_ID_MSG,1,0x310304,A1B(svoVar_unbalanceDiscNum)); //##UnbalanceDisc_Num==
                                }

                                if (!FokStatusOK())
                                {
                                    fUnbalanceChk_SpdUp |= Unb_chk_mask;
                                    svoVar_unbalanceDiscNum = 2;
                                    InitializeReadSpeed();
                                    SendMsgCn(DEBUG_CUSTOMER_MSG2,1,0x310304,A1B(svoVar_unbalanceDiscNum)); //##UnbalanceDisc_Num==
                                }
                            }
                        }
#endif//(DVD_UNBALANCE_DISC_CHECK == 1)
                    }
                    break;
#if (BD_ENABLE == 1)
                case eDiscBD:
                    SendMsgCnB(SHOW_DEBUG_MSG,4,0x330105+svoSpeedVar.SpdMode,F4B((float)SpeedIndexTableBD[speed]/100)); //Adjust_Spd, TarSpd=%2.1fX_?C?V
#if (PCB == 711) || (PCB == 710)||(PCB == 817)
    #if (BD_UNBALANCE_DISC_CHECK == 1)
                    if ((Disc_Speed.Read >= e4XDCAV) && ((fUnbalanceChk_SpdUp & Unb_chk_mask) != Unb_chk_mask))
                    {
                        if ((!(fUnbalanceChk_SpdUp & 0x01)) && (speed == _4AX_BD))
                        {
                            if (tracking_status == TRUE)
                            {
                                tracking_status = FALSE;
                                track_off();
        #if (SRC_ENABLE == 1)
                                HAL_DSP_SRCEnable(SRC_LOOP_TRACKING, SRC_STATE_RESET); //Reset Tracking SRC
        #endif
                            }
                            spdWaitSpdInRange();
                            seekSetCEEq(eRoughSeekRatioOnly, 0);
                            HAL_SERVO_CenterControlON(FORW);
                            WaitTimer(80);
                            ce_signal = ReadDSP_CE_mV_Avg(ONE_REVOLUTION, TRUE);
                            HAL_SERVO_CenterControlOFF();
                            if ((speed == _4AX_BD))
                            {
                                fUnbalanceChk_SpdUp |= (1<<1);
                                Unbalance_highspd = ce_signal.p2p_mv;

                                if ((Unbalance_midspd+Unbalance_highspd)>BD_UNB_CHK_TH)
                                {
                                    svoVar_unbalanceDiscNum = 2;
                                    InitializeReadSpeed();
                                }
                                SendMsgCn(DEBUG_DISK_ID_MSG,4,0x310312,A2B(Unbalance_midspd),A2B(Unbalance_highspd));
                                SendMsgCn(DEBUG_DISK_ID_MSG,1,0x310304,A1B(svoVar_unbalanceDiscNum)); //##UnbalanceDisc_Num==
                            }

                            if(!FokStatusOK())
                            {
                                fUnbalanceChk_SpdUp |= Unb_chk_mask;
                                svoVar_unbalanceDiscNum = 2;
                                InitializeReadSpeed();
                                SendMsgCn(DEBUG_CUSTOMER_MSG2,1,0x310304,A1B(svoVar_unbalanceDiscNum)); //##UnbalanceDisc_Num==
                            }
                        }
                    }
    #endif
#else
    #if (BD_UNBALANCE_DISC_CHECK == 1)
                    if ((Disc_Speed.Read >= e6XDCAV) && ((fUnbalanceChk_SpdUp & Unb_chk_mask) != Unb_chk_mask))
                    {
                        if (((!(fUnbalanceChk_SpdUp & 0x01)) && (speed == _4AX_BD)) ||
                            ((!(fUnbalanceChk_SpdUp & 0x02)) && (speed == _6AX_BD)))
                        {
                            if (tracking_status == TRUE)
                            {
                                tracking_status = FALSE;
                                track_off();
        #if (SRC_ENABLE == 1)
                                HAL_DSP_SRCEnable(SRC_LOOP_TRACKING, SRC_STATE_RESET); //Reset Tracking SRC
        #endif
                            }
                            spdWaitSpdInRange();
                            seekSetCEEq(eRoughSeekRatioOnly, 0);
                            HAL_SERVO_CenterControlON(FORW);
                            WaitTimer(80);
                            ce_signal = ReadDSP_CE_mV_Avg(ONE_REVOLUTION, TRUE);
                            HAL_SERVO_CenterControlOFF();

                            if ((speed == _4AX_BD))
                            {
                                fUnbalanceChk_SpdUp |= (1<<0);
                                Unbalance_midspd = ce_signal.p2p_mv;
                            }
                            else if ((speed == _6AX_BD))
                            {
                                fUnbalanceChk_SpdUp |= (1<<1);
                                Unbalance_highspd = ce_signal.p2p_mv;

                                if ((Unbalance_midspd+Unbalance_highspd)>1200)
                                {
                                    svoVar_unbalanceDiscNum = 2;
                                    InitializeReadSpeed();
                                }
                                SendMsgCn(DEBUG_DISK_ID_MSG,4,0x310312,A2B(Unbalance_midspd),A2B(Unbalance_highspd));
                                SendMsgCn(DEBUG_DISK_ID_MSG,1,0x310304,A1B(svoVar_unbalanceDiscNum)); //##UnbalanceDisc_Num==
                            }

                            if(!FokStatusOK())
                            {
                                fUnbalanceChk_SpdUp |= Unb_chk_mask;
                                svoVar_unbalanceDiscNum = 2;
                                InitializeReadSpeed();
                                SendMsgCn(DEBUG_CUSTOMER_MSG2,1,0x310304,A1B(svoVar_unbalanceDiscNum)); //##UnbalanceDisc_Num==
                            }
                        }
                    }
    #endif
#endif
                    break;
#endif
#if (ENABLE_HDDVD == 1)
                case eDiscHD:
                    SendMsgCnB(SHOW_DEBUG_MSG,4,0x330105+svoSpeedVar.SpdMode,F4B((float)SpeedIndexTableHD[speed]/100)); //Adjust_Spd, TarSpd=%2.1fX_?C?V
                    break;
#endif
            }
        }

        if (tracking_status == FALSE)
        {
            svoSpeedVar.SpdPresetNeeded = TRUE;
#if (HOLD_LENS_BEFORE_TRKON == 1)
            seekSetCEEq(eRoughSeek, (svoVar_discKindT.Bit.DiscStd == eDiscCD)? CE_GAIN_DOWN_RATIO_CD : CE_GAIN_DOWN_RATIO_DVD);
            HAL_SERVO_CenterControlON(FORW);  // Turn Center-servo on
            WaitTimer(MS_10);          // Wait for Center-servo holding-on lens
#endif
            wait_track_on(TRK_ON_WAIT_SLOW_TIMEOUT);
        }

        SetMotorDriverState(SHORT_BRAKE);
    }// if(svoSpeedVar.SpdCurrentSpeed != speed)
}// end of spdChangeDiscSpeed

/*-----------------------------------------------------------------------------
 *
 * Module: spdHoldRPM()
 * Description: hold rotation speed, prevent spdUpdateRPM() until spdUnhold() or spdChangeDiscSpeed()
 *----------------------------------------------------------------------------*/
void spdHoldRPM(USHORT rpm)
{
    if (rpm * 8 < svoSpeedVar.MinimumRpm * 7)
    {
        spdSetCAVM(rpm);
        svoSpeedVar.SpdUp = TRUE;
    }
    SendMsgCn(SHOW_DEBUG_MSG,2,0x33010B,A2B(rpm));  // Hold_Spd, TargetRPM = %5u (rpm)
    svoSpeedVar.SpdHold = TRUE;
#if (ACC_CONTROL > 0) || (DEC_CONTROL > 0)
    svoSpeedVar.TargetRpmReal = svoSpeedVar.CurrentRpm;
#endif
    spdSetFGFreq(rpm);
    spdUpdateRPM(Iram_stp_CurrentPosition);
}

/*-----------------------------------------------------------------------------
 *
 * Module: spdUnhold()
 * Description: restore target RPM for current speed
 *
 *----------------------------------------------------------------------------*/
void spdUnhold(void)
{
    if (svoSpeedVar.SpdHold)
    {
        SendMsg80(SHOW_DEBUG_MSG,0x33010C);//Unhold_Spd
        svoSpeedVar.SpdHold = FALSE;
        spdUpdateRPM(Iram_stp_CurrentPosition);
    }
}

/*-----------------------------------------------------------------------------
 *
 * Module: spdSetMaxRPM()
 * Description: set maximum RPM limit
 *----------------------------------------------------------------------------*/
void spdSetMaxRPM(USHORT rpm)
{
    svoSpeedVar.MaximumRpm = (rpm == 0)? CAV_RPM_MAX : rpm;
    svoSpeedVar.CavSwitchPos = spdGetCavSwitchPos(svoSpeedVar.SpdCurrentSpeed,&svoSpeedVar.MaximumRpm);

    if (svoSpeedVar.MaximumRpm == rpm)
    {
        svoSpeedVar.SpdSetMaxRPM = TRUE;
        SendMsgCn(DEBUG_SVO_SPD_MSG,2,0x33010D,A2B(rpm));//Set Maxumum RPM=%5u
    }
    else
    {
        svoSpeedVar.SpdSetMaxRPM = FALSE;
        SendMsg80(DEBUG_SVO_SPD_MSG,0x33010E);//Set Maxumum RPM=default
    }

    if (svoSpeedVar.CavSwitchPos == 0xFFFF)
    {
        svoSpeedVar.SpdMode = SPD_MODE_CAV;
    }
    else if (svoSpeedVar.CavSwitchPos == 0)
    {
        svoSpeedVar.SpdMode = SPD_MODE_CLV;
    }
    else
    {
        svoSpeedVar.SpdMode = SPD_MODE_PCAV;
    }
    spdUpdateRPM(Iram_stp_CurrentPosition);
}

/*-----------------------------------------------------------------------------
 *
 * Module: spdUpdateRPM()
 * Description: update rotation speed
 *
 *----------------------------------------------------------------------------*/
void spdUpdateRPM(USHORT stp_position)
{
    USHORT RpmRatio,target_position,spdlimit;
    USHORT TargetRpm;

    TargetRpm = spdGetTargetRPM(stp_position, svoSpeedVar.SpdCurrentSpeed);

    if (TargetRpm == 0)
    {
        send_msg5S(1,"spdUpdateRPM");
        return;
    }

    if (svoSpeedVar.SpdMonitor)
    {
        spdMonitorRPM();
        svoSpeedVar.SpdMonitor = FALSE;
    }

    if ((svoSpeedVar.SpdUp) && (svoSpeedVar.SpdHold == FALSE))
    {
        if (svoSpeedVar.CurrentRpm * 8 >= svoSpeedVar.MinimumRpm * 7)
        {
            svoSpeedVar.SpdUp = FALSE;
            spdSetCAVM(svoSpeedVar.MinimumRpm);
        }
    }

    spdlimit = SPD_IN_RANGE_LIMIT_X_1000;
    if (TargetRpm > (SPD_IN_RANGE_MAX_DIFF_RPM * 1000 / SPD_IN_RANGE_LIMIT_X_1000))
    {
        spdlimit = SPD_IN_RANGE_MAX_DIFF_RPM * 1000 / TargetRpm;
    }
    else if (TargetRpm < (SPD_IN_RANGE_MIN_DIFF_RPM * 1000 / SPD_IN_RANGE_LIMIT_X_1000))
    {
        spdlimit = SPD_IN_RANGE_MIN_DIFF_RPM * 1000 / TargetRpm;
    }
    RpmRatio = svoSpeedVar.CurrentRpm * 1000 / TargetRpm;
    if ((RpmRatio >= (1000 - spdlimit)) && (RpmRatio < (1000 + spdlimit)))
    {
        svoSpeedVar.CurrentRpm = TargetRpm;
        if (svoSpeedVar.SpdInRange == FALSE)
        {
            svoSpeedVar.SpdInRange = TRUE;
#if (NEW_SPINDLE_CONTROL == 1)
            spdSetIntegrator(TargetRpm);
#endif
            target_position = Iram_stp_CurrentPosition;
            if (Iram_stp_Status == (STP_BUSY_MASK | STP_DEVICE_NORMAL))
            {
                if (Iram_stp_CurrentDirection == FORW)
                {
                    target_position += (Iram_stp_JumpDistance - Iram_stp_CurrentDistance);
                }
                else
                {
                    target_position -= (Iram_stp_JumpDistance - Iram_stp_CurrentDistance);
                }
            }

            svoSpeedVar.SpdOverspd = spdGetTargetOverspeedX100(target_position,svoSpeedVar.SpdCurrentSpeed);
            if (svoSpeedVar.SpdMode != SPD_MODE_CAV)
            {
                UpdateReadFilters((svoSpeedVar.SpdOverspd + 5) / 10);
            }
            ChkUpdateReadFilters();
    #if (MSG_OUT_MODE != MSG_DISABLE)
            if (!svoSpeedVar.SpdDown)
            {
                //***speed in range (update): %d (mS)***
                SendMsgCnB(DEBUG_SVO_SPD_MSG,2,0x33102F,B2B(svoSpeedVar.SpdWaitCnt * 20));
            }
    #endif
        }
    }
    else
    {
        if (svoSpeedVar.SpdInRange)
        {
            svoSpeedVar.SpdWaitCnt = 0;
        }
        svoSpeedVar.SpdInRange = FALSE;
    }
#if (ACC_CONTROL > 0) || (DEC_CONTROL > 0)
    svoSpeedVar.TargetRpmReal = svoSpeedVar.CurrentRpm;
#endif
    spdSetFGFreq(TargetRpm);
}

/*-----------------------------------------------------------------------------
 *
 * Module: spdSetPara()
 * Description: load all parameters for svoSpeedVar.SpdCurrentSpeed
 *
 *----------------------------------------------------------------------------*/
void spdSetPara(void)
{
    ESpindleSpeed speed;

    SetDfeWblFrontOnHold();
    speed = svoSpeedVar.SpdCurrentSpeed;
#if (SRC_ENABLE == 1)
    #if (SRC_ENABLE_FOCUS == 1)
    HAL_DSP_SRCEnable(SRC_LOOP_FOCUS, SRC_STATE_OFF); //Disable Focus SRC
    HAL_DSP_SRCEnable(SRC_LOOP_FOCUS, SRC_STATE_RESET); //Reset Tracking SRC
    #endif
    HAL_DSP_SRCEnable(SRC_LOOP_TRACKING, SRC_STATE_OFF); //Disable Tracking SRC
    HAL_DSP_SRCEnable(SRC_LOOP_TRACKING, SRC_STATE_RESET); //Reset Tracking SRC
    svoSpeedVar.SpdSRCEnable = FALSE;
#endif //#if SRC_ENABLE
    switch (svoVar_discKindT.Bit.DiscStd)
    {
        case eDiscCD:
#if (SRC_ENABLE == 1)
            if (speed >= MIN_SPEED_ENABLE_SRC_CD)
            {
                svoSpeedVar.SpdSRCEnable = TRUE;
            }
#endif //#if SRC_ENABLE
            break;

        case eDiscDVD:
#if (SRC_ENABLE == 1)
            if (speed >= MIN_SPEED_ENABLE_SRC_DVD)
            {
                svoSpeedVar.SpdSRCEnable = TRUE;
            }
#endif //#if SRC_ENABLE
            break;

#if (BD_ENABLE == 1)
        case eDiscBD:
    #if (SRC_ENABLE == 1)
            if (speed >= MIN_SPEED_ENABLE_SRC_BD)
            {
                svoSpeedVar.SpdSRCEnable = TRUE;
            }
    #endif //#if SRC_ENABLE
            break;
#endif // (BD_ENABLE == 1)

#if (ENABLE_HDDVD == 1)
        case eDiscHD:
    #if (SRC_ENABLE == 1)
            if (speed >= MIN_SPEED_ENABLE_SRC_HD)
            {
                svoSpeedVar.SpdSRCEnable = TRUE;
            }
    #endif //#if SRC_ENABLE
            break;
#endif // (ENABLE_HDDVD == 1)

        default:
            DEBUG_ASSERT(FALSE, eASSERT_IMPOSSIBLE_DISCKIND ); // Let's check here if we aren't running for a unknownDiscKind/DiscStd
            break;
    }

    svoIramVar.svoCurrentSpeed = speed;
    LAL_StoreParams(LAL_DetectDiscKind(svoVar_discKindT), speed);
    LAL_SetReadMode();

    DEBUG_ASSERT(speed != NONE, eASSERT_IMPOSSIBLE_SPINDLE_SPEED);

#if (PLANT_MODELING_SW == 1)
    svoSpeedVar.SpdSRCEnable = FALSE;
#endif

#if (SRC_ENABLE == 1)
    if (svoSpeedVar.SpdSRCEnable)
    {
    #if (SRC_ENABLE_FOCUS == 1)
        HAL_DSP_SRCEnable(SRC_LOOP_FOCUS, SRC_STATE_ON); //Enable Focus SRC
    #endif // (SRC_ENABLE_FOCUS == 1)
        HAL_DSP_SRCEnable(SRC_LOOP_TRACKING, SRC_STATE_ON); //Enable Tracking SRC
    }
#endif
    svo_bReadZoneSpeedX10 = 0;//force UpdateReadFilters()
    SetReadFilters();
}// end of spdSetPara

/*-----------------------------------------------------------------------------
 *
 * Module: spdSetCAVM()
 * Description: Calculate and set CAVM for the minimum RPM
 *
 *----------------------------------------------------------------------------*/
void spdSetCAVM(USHORT MinRpm)
{
    USHORT cavm;

    if (MinRpm < 300)
    {
        MinRpm = 300;
    }
    cavm = (SERVO_CLOCK * 60 / 0xCCCC) / MinRpm + 1;
    if (READ_FIELD(HAL_SRV_MDIV) != cavm)
    {
        WRITE_FIELD(HAL_SRV_MDIV, cavm);
        svoSpeedVar.SpdOldRevcnt = READ_REG(HAL_SRVREG_MAP->REVCNT);
        svoSpeedVar.SpdOldRevcntChk = FALSE;
    }
} // spdSetCAVM

/*-----------------------------------------------------------------------------
 *
 * Module: spdSetKd14()
 * Description: adapt DE4_msb to prevent DMO jump when changing kd14
 *
 *----------------------------------------------------------------------------*/
void spdSetKd14(SHORT kd14)
{
    if (kd14)
    {
#if (NEW_SPINDLE_CONTROL == 1)
        WRITE_DSP_COEF(DE4_msb, READ_DSP_COEF(DE4_msb) * last_kd14 / kd14);
#endif
        last_kd14 = kd14;
    }
#if (SPD_MAX_BRAKE_RPM > 0)
    else if (svoSpeedVar.SpdFreeRun)
    {
        kd14 = 0;
    }
    else
    {
        kd14 = last_kd14;
    }
#endif
    WRITE_DSP_COEF(kdf14, kd14);
}

#if (NEW_SPINDLE_CONTROL == 1)
/*-----------------------------------------------------------------------------
 *
 * Module: spdGetIntegrator()
 * Description: Calculate and set DE4_msb for the input RPM
 *
 *----------------------------------------------------------------------------*/
void spdSetIntegrator(USHORT rpm)
{
    LONG temp_long;

    #if (DISC_8CM == 1)
    if (svoVar_discDiameter == e8cm)
    {
        temp_long = (rpm * rpm / SPD_INT_PARA_1_8CM + rpm * SPD_INT_PARA_2_8CM / 262144);
    }
    else
    #endif
    {
        temp_long = (rpm * rpm / SPD_INT_PARA_1 + rpm * SPD_INT_PARA_2 / 262144);
    }
    temp_long = temp_long * svoSpeedVar.SpdDE4Ratio / 128 + svoSpeedVar.SpdIntPara3;
    temp_long = temp_long * 0x80 * DMODAC_FACTOR;
    temp_long /= last_kd14;

    if (temp_long > 0x7FFF) temp_long = 0x7FFF;

    WRITE_DSP_COEF(DE4_msb, temp_long);
}

/*-----------------------------------------------------------------------------
 *
 * Module: spdAdaptIntegrator()
 * Description: adapt svoSpeedVar.SpdDE4Ratio & svoSpeedVar.SpdIntPara3
 *
 *----------------------------------------------------------------------------*/
void spdAdaptIntegrator(void)
{
    LONG temp_long,temp_long2;

    temp_long2 = READ_DSP_COEF(DE4_msb) * READ_DSP_COEF(kdf14);
    {
        temp_long2 /= (0x80 * DMODAC_FACTOR);
        temp_long = svoSpeedVar.TargetRpm;
    #if (DISC_8CM == 1)
        if (svoVar_discDiameter == e8cm)
        {
            temp_long = (temp_long * temp_long / SPD_INT_PARA_1_8CM + temp_long * SPD_INT_PARA_2_8CM / 262144);
        }
        else
    #endif
        {
            temp_long = (temp_long * temp_long / SPD_INT_PARA_1 + temp_long * SPD_INT_PARA_2 / 262144);
        }

        if (svoSpeedVar.TargetRpm >= 3600)
        {
            temp_long = (temp_long2 - svoSpeedVar.SpdIntPara3) * 128 / temp_long;

            if (temp_long > 255) temp_long = 255;
            if (temp_long < 64) temp_long = 64;

            svoSpeedVar.SpdDE4Ratio = temp_long;
        }
        else
        {
            temp_long = temp_long2 - (temp_long * svoSpeedVar.SpdDE4Ratio / 128);

            svoSpeedVar.SpdIntPara3 = temp_long;
        }
    }
    //svoSpeedVar.SpdDE4Ratio = %3u, svoSpeedVar.SpdIntPara3 = %3u
    SendMsgCn(SHOW_DEBUG_MSG,3,0x330111,A1B(svoSpeedVar.SpdDE4Ratio),A2B(svoSpeedVar.SpdIntPara3));
}
#endif

/*-----------------------------------------------------------------------------
 *
 * Module: spdSetFGFreq()
 * Description: Convert rpm to FGCMP and set into register
 *
 *----------------------------------------------------------------------------*/
void spdSetFGFreq(USHORT rpm)
{
    BYTE  cur_afcg;
    USHORT pre_target_rpm;
    ULONG fg_cmp, kd10_factor, kd10_temp;
#if (NEW_SPINDLE_CONTROL == 1)
    ULONG kd16_temp;
#endif

#if (PRN_SPEED_CALC == 1)
    if ((svoSpeedVar.SpdMode == SPD_MODE_PCAV) && ((svoSpeedVar.TargetRpm == svoSpeedVar.MaximumRpm) != (rpm == svoSpeedVar.MaximumRpm)))
    {
        if (rpm == svoSpeedVar.MaximumRpm)
            SendMsg80(SHOW_DEBUG_MSG,0x330013); //CAV_area
        else
            SendMsg80(SHOW_DEBUG_MSG,0x330012); //CLV_area
    }
    SendMsgCnB(SHOW_DEBUG_MSG,4,0x331023,B4B(rpm)); // ***FGfreq->RPM = %4d
#endif
    pre_target_rpm = svoSpeedVar.TargetRpm;
    svoSpeedVar.TargetRpm = rpm;

#if (ACC_CONTROL > 0) || (DEC_CONTROL > 0)
    if (rpm > svoSpeedVar.TargetRpmReal)
    {
    #if (ACC_CONTROL > 0)
        svoSpeedVar.TargetRpmReal += ((10*ACC_CONTROL*ACC_CONTROL-18*ACC_CONTROL+9) / (svoSpeedVar.TargetRpmReal + ACC_CONTROL) + 1);
        if (svoSpeedVar.TargetRpmReal > rpm)
        {
            svoSpeedVar.TargetRpmReal = rpm;
        }
        rpm = svoSpeedVar.TargetRpmReal;
    #else
        svoSpeedVar.TargetRpmReal = rpm;
    #endif
    }
    else
    {
    #if (DEC_CONTROL > 0)
        svoSpeedVar.TargetRpmReal -= ((10*DEC_CONTROL*DEC_CONTROL-18*DEC_CONTROL+9) / (svoSpeedVar.TargetRpmReal + DEC_CONTROL) + 1);
        if (svoSpeedVar.TargetRpmReal < rpm)
        {
            svoSpeedVar.TargetRpmReal = rpm;
        }
        rpm = svoSpeedVar.TargetRpmReal;
    #else
        svoSpeedVar.TargetRpmReal = rpm;
    #endif
    }
    if (svoSpeedVar.SpdInRange == TRUE)
    {
        svoSpeedVar.CurrentRpm = rpm;
    }
#endif

    if (rpm > 0)
    {
        fg_cmp = ((ULONG)(SERVO_CLOCK * 60L / ((DISK_FR2X + 1) * DISK_NFGPLS)) / ((ULONG)rpm * READ_FIELD(HAL_SRV_MDIV)));
    }
    else
    {
        fg_cmp = 0xFFFF;
    }

#if (NEW_SPINDLE_CONTROL == 0)
    fg_cmp = fg_cmp - ((svoSpeedVar.LostFg * CAVM_REF) / READ_FIELD(HAL_SRV_MDIV));
#endif

#if 0 //debug code for RD to change spindle motor freq on the fly
    //motor_freq = normal_motor_freq * (100 + servo_debug[?]) / 100
    //servo_debug[?] must > -100.
    if (servo_debug[?])
    {
        fg_cmp = fg_cmp * 100 / (100 + servo_debug[?]);
    }
#endif

    kd10_factor = (FGCMP_REF * FGCMP_REF * CAVM_REF) / (fg_cmp * READ_FIELD(HAL_SRV_MDIV));

    kd10_temp = (kd10_factor * svoSpeedVar.Kd10Fg) / fg_cmp;
#if (NEW_SPINDLE_CONTROL == 1)
    kd16_temp = (kd10_factor * svoSpeedVar.Kd16Fg) / fg_cmp;
#endif
    cur_afcg = READ_FIELD(HAL_SRV_AFCG);
    while (kd10_temp > 0x7FFF)
    {
        if (cur_afcg == 0)
        {
            SendMsgCn(SHOW_DEBUG_MSG,4,0x33102B,A4B(kd10_temp));
            kd10_temp = 0x7FFF;
            break;
        }
        else
        {
            WRITE_FIELD(HAL_SRV_AFCG, --cur_afcg);
            kd10_temp /= 2;
            svoSpeedVar.Kd10Fg /= 2;
#if (NEW_SPINDLE_CONTROL == 1)
            kd16_temp /= 2;
            svoSpeedVar.Kd16Fg /= 2;
#endif
        }
    }

    if (svoSpeedVar.SpdInRange)
    {
        while (cur_afcg > svoSpeedVar.SpdAfcg)
        {
#if (NEW_SPINDLE_CONTROL == 1)
            if ((kd10_temp < 0x20) || (kd16_temp < 0x08))
#else
            if (kd10_temp < 0x20)
#endif
            {
                //kd10 underflow is not a big problem
                SendMsgCn(DEBUG_SVO_SPD_MSG,4,0x33102C,A4B(kd10_temp)); // kd10 underflow = %4X
                break;
            }
            else
            {
                WRITE_FIELD(HAL_SRV_AFCG, --cur_afcg);
                kd10_temp /= 2;
                svoSpeedVar.Kd10Fg /= 2;
#if (NEW_SPINDLE_CONTROL == 1)
                kd16_temp /= 2;
                svoSpeedVar.Kd16Fg /= 2;
#endif
            }
        }
#if (NEW_SPINDLE_CONTROL == 1)
        if (kd16_temp > 0x7FFF)
        {
            kd16_temp = 0x7FFF;
        }
    #if (LABELTAG_ENABLE == 1)
        if (svoLBTvar.WriteFlag == TRUE)
        {
            WRITE_DSP_COEF(kdf16, kd16_temp / 4);
        }
        else
    #endif
        {
            WRITE_DSP_COEF(kdf16, kd16_temp);
        }
#endif
    }
    else
    {
        while (kd10_temp < svoSpeedVar.Kd10FgMin)
        {
            if (cur_afcg == 7)
            {
                //kd10 underflow is not a big problem
                SendMsgCn(DEBUG_SVO_SPD_MSG,4,0x33102C,A4B(kd10_temp));
                break;
            }
            else
            {
                WRITE_FIELD(HAL_SRV_AFCG, ++cur_afcg);
                kd10_temp *= 2;
                svoSpeedVar.Kd10Fg *= 2;
#if (NEW_SPINDLE_CONTROL == 1)
                kd16_temp *= 2;
                svoSpeedVar.Kd16Fg *= 2;
#endif
            }
        }
#if (NEW_SPINDLE_CONTROL == 1)
        if (ABS(pre_target_rpm - svoSpeedVar.TargetRpm) > SPD_IN_RANGE_MIN_DIFF_RPM)
        {
            spdSetIntegrator(rpm);
        }
#endif
    }
#if (SPD_MAX_BRAKE_RPM > 0)
    if ((!svoSpeedVar.SpdInRange) && (svoSpeedVar.CurrentRpm > SPD_MAX_BRAKE_RPM) && (svoSpeedVar.CurrentRpm > rpm) && (!DVDRAM))
    {
        if (!svoSpeedVar.SpdFreeRun)
        {
            SendMsg80(SHOW_DEBUG_MSG,0x331031); //***SpdFreeRun spin down on***
            svoSpeedVar.SpdFreeRun = TRUE;
            spdSetKd14(0);
        }
    }
    else if (svoSpeedVar.SpdFreeRun)
    {
        SendMsg80(SHOW_DEBUG_MSG,0x331032); //***SpdFreeRun spin down off***
        svoSpeedVar.SpdFreeRun = FALSE;
        spdSetKd14(0);
    }
#endif
    WRITE_DSP_COEF(kdf10, kd10_temp);
#if (NEW_SPINDLE_CONTROL == 1)
    if (kd16_temp > 0x7FFF)
    {
        kd16_temp = 0x7FFF;
    }
    WRITE_DSP_COEF(kdf16, kd16_temp);
#endif
    WRITE_REG(HAL_SRVREG_MAP->FGRNG,(fg_cmp / 3));   // +/-33%
    if (cur_afcg > 0) //correct the effect from residual quantisation error
    {
        fg_cmp = fg_cmp - (1 << (cur_afcg- 1));
    }
    WRITE_REG(HAL_SRVREG_MAP->FGCMP,fg_cmp);
}// end of spdSetFGFreq

/*-----------------------------------------------------------------------------
 *
 * Module: spdGetTargetOverspeedX100()
 * Description: calculate overspeed from stepper position.
 *
 *  Input Paramter: StepPos:        Stepper position
 *                  speed:          Spindle speed profile
 *  Ouput Paramater: OverspeedX100
 *----------------------------------------------------------------------------*/
USHORT spdGetTargetOverspeedX100(USHORT StepPos, ESpindleSpeed speed)
{
    USHORT CavSwitchPos,rpm;

    if ((speed == NONE) || (speed == 0xFF))
    {
        return 0;
    }

    if (speed != svoSpeedVar.SpdCurrentSpeed)
    {
        rpm = CAV_RPM_MAX;
        CavSwitchPos = spdGetCavSwitchPos(speed,&rpm);
    }
    else if (svoSpeedVar.SpdHold == FALSE)
    {
        CavSwitchPos = svoSpeedVar.CavSwitchPos;
    }
    else
    {
        CavSwitchPos = 0xFFFF;
    }

    if (StepPos >= CavSwitchPos)
    {
        switch (svoVar_discKindT.Bit.DiscStd)
        {
            case eDiscCD:
                return SpeedIndexTableCD[speed];

            case eDiscDVD:
#if (DVD_RAM_READ == 1)
                if (DVDRAM)
                {
                    return SpeedIndexTableDVDRAM[speed];
                }
#endif
#if (DVD_710Run815New == 1)
                return GetSpeedIndexTableDVD(speed);
#else
                return SpeedIndexTableDVD[speed];
#endif

#if (BD_ENABLE == 1)
            case eDiscBD:
                return SpeedIndexTableBD[speed];
#endif // (BD_ENABLE == 1)

#if (ENABLE_HDDVD == 1)
            case eDiscHD:
                return SpeedIndexTableHD[speed];
#endif // (ENABLE_HDDVD == 1)
        }
    }

    return spdGetOverSpdX100(spdGetTargetRPM(StepPos, speed), StepPos);
}

#if (DVD_RAM_READ == 1)
/*-----------------------------------------------------------------------------
 *
 * Module: spdGetDVDRAMOverspeedX100()
 * Description: calculate overspeed for specified zone.
 *
 *  Input Paramter:  zone
 *  Ouput Paramater: OverspeedX100
 *
 *  BPS = 676.214 (at 1X)
 *  rmp = 60 * BPS * OverSpeed / SectorsPerRev
 *      = 28401 * OverSpeedX100 / (70 * SectorsPerRev)
 *
 *  OverSpeedX100 = rpm * 70 * SectorsPerRev / 28401
 *
 *----------------------------------------------------------------------------*/
USHORT spdGetDVDRAMOverspeedX100(BYTE zone)
{
    USHORT OverSpeedX100;

    OverSpeedX100 = spdGetCurrentRPM() * 70UL * DVDRAMZoneTable[zone].SectorsPerRev / 28401UL;

    return OverSpeedX100;
}
#endif

/*-----------------------------------------------------------------------------
 *
 * Module: spdGetTargetRPM()
 * Description: return target spindle motor rotation speed in RPM for all medias and all speeds at given radius
 *
 *     2 x BPS x PI x Sled_step x parFine
 * V = ----------------------------------
 *                     32
 *
 * 2 * PI * R * rpm / (60 * V) = overspeed
 * rpm = (60 * V * overspeed) / (2 * PI * R)
 *     = (60 * BPS * parFine * overspeed) / (32 * R[in sled_step])
 *     = (15 * BPS * parFine * overspeed) / (8 * R[in sled_step])
 *
 * HD's BPS = 64800000 / 29016 = 2233.25
 * BD's BPS = 66000000 / 29946 = 2203.97
 * DVD's BPS = 26156250 / 38688 = 676.08
 * CD's BPS = 75
 * parFine:svoSeekVar.parFineInit
 *----------------------------------------------------------------------------*/
USHORT spdGetTargetRPM(USHORT StepPos, ESpindleSpeed speed)
{
    BOOL   cav;
    ULONG  rpm;
#if (DVD_RAM_READ == 1)
    LONG   addr;
    BYTE   zone;
#endif

    if ((speed == 0) || (speed == 0xFF)) return 0;

    if ((svoSpeedVar.SpdHold == TRUE) && (speed == svoSpeedVar.SpdCurrentSpeed))
    {
        return svoSpeedVar.TargetRpm;
    }

    if (StepPos < UM_2_SLED_STEP(MIN_SLED_FOLLOW_RADIUS))
    {
        StepPos = UM_2_SLED_STEP(MIN_SLED_FOLLOW_RADIUS);
    }
    cav = FALSE;
    switch (svoVar_discKindT.Bit.DiscStd)
    {
        case eDiscCD:
            if (speed > CLV_SPEED_CD)
            {
                cav = TRUE;
                StepPos = UM_2_SLED_STEP(CD_CAV_RADIUS_REF);
            }
            rpm = SpeedIndexTableCD[speed] * 45 / 4;//75 * 15 / 100
            break;

         case eDiscDVD:
#if (DVD_RAM_READ == 1)
            if (DVDRAM)
            {
                if (speed > CLV_SPEED_DVDRAM)
                {
                    //sectors per second for DVDRAM 1X = 676.214.. ~ 9467 / 14 = BPS
                    //rmp = 60 * BPS * overspeed / SectorsPerRev
                    //    = 60 * 9467 / 14 * SpeedIndexTableDVDRAM / 100 / SectorsPerRev
                    //    = 28401 * SpeedIndexTableDVDRAM / (70 * SectorsPerRev);
                    rpm = 28401UL * SpeedIndexTableDVDRAM[speed] / (70 * ((DiscTypeIndex == LOC_DISC_TYPE_10)? MAX_SECTORS_RAM_10 : MAX_SECTORS_RAM_20));
                }
                else
                {
                    if (SeekBusy)
                    {
                        addr = svoSeekVar.svoTargetID.SectorNum.all32;
                    }
                    else
                    {
                        addr = svoIramVar.currentID.SectorNum.all32;

                        if (ABS(svoPBA2Radius(addr) - StepPos) > UM_2_SLED_STEP(800))
                        {
                            addr = svoRadius2PBA(StepPos, 0);
                        }
                    }
                    zone = DVDRAM_PSN2ZONE(addr);
                    //sectors per second for DVDRAM 1X = 676.214.. ~ 9467 / 14
                    //rmp = 60 * BPS * overspeed / SectorsPerRev
                    //    = 60 * 9467 / 14 * SpeedIndexTableDVDRAM / 100 / SectorsPerRev
                    //    = 28401 * SpeedIndexTableDVDRAM / (70 * SectorsPerRev);
                    rpm = 28401UL * SpeedIndexTableDVDRAM[speed] / (70 * DVDRAMZoneTable[zone].SectorsPerRev);
                }
                if (rpm > svoSpeedVar.MaximumRpm)
                {
                    rpm = svoSpeedVar.MaximumRpm;
                }
                return (USHORT)rpm;
            }
            else
#endif
            {
                if (speed > CLV_SPEED_DVD)
                {
                    cav = TRUE;
                    if (svoVar_discKindT.Bit.Layers == eLayersDouble)
                    {
                        StepPos = UM_2_SLED_STEP(DVDDL_CAV_RADIUS_REF);
                    }
                    else
                    {
                        StepPos = UM_2_SLED_STEP(DVDSL_CAV_RADIUS_REF);
                    }
                }
#if (DVD_710Run815New == 1)
                rpm = (ULONG)GetSpeedIndexTableDVD(speed) * 47968 / 473; //676.08 * 15 / 100
#else
                rpm = (ULONG)SpeedIndexTableDVD[speed] * 47968 / 473; //676.08 * 15 / 100
#endif
            }
            break;

#if (BD_ENABLE == 1)
        case eDiscBD:
            if (speed > CLV_SPEED_BD)
            {
                cav = TRUE;
                if (svoVar_discKindT.Bit.Layers == eLayersDouble)
                {
                    StepPos = UM_2_SLED_STEP(BDDL_CAV_RADIUS_REF);
                }
                else
                {
                    StepPos = UM_2_SLED_STEP(BDSL_CAV_RADIUS_REF);
                }
            }
            rpm = (ULONG)SpeedIndexTableBD[speed] * 53887 / 163; //2203.97 * 15 / 100
            break;
#endif // (BD_ENABLE == 1)

#if (ENABLE_HDDVD == 1)
        case eDiscHD:
            if (speed > CLV_SPEED_HD)
            {
                cav = TRUE;
                if (svoVar_discKindT.Bit.Layers == eLayersDouble)
                {
                    StepPos = UM_2_SLED_STEP(HDDL_CAV_RADIUS_REF);
                }
                else
                {
                    StepPos = UM_2_SLED_STEP(HDSL_CAV_RADIUS_REF);
                }
            }
            rpm = (ULONG)SpeedIndexTableHD[speed] * 27134 / 81; //2233.25 * 15 / 100
            break;
#endif // (ENABLE_HDDVD == 1)
    }
    rpm = rpm * svoSeekVar.parFineInit / (8 * StepPos);
    if (rpm > svoSpeedVar.MaximumRpm)
    {
        rpm = svoSpeedVar.MaximumRpm;
    }

#if (PRN_SPEED_CALC == 1)
    SendMsgCnB(SHOW_DEBUG_MSG,4,0x331300,B4B(rpm));  // TargetRPM = %d (rpm)
#endif

    return (USHORT)rpm;
}

/*-----------------------------------------------------------------------------------------------------
    Name:           spdGetOverSpdX100

    Description:    Calculate Overspeed of given SledPosition and RPM

    Inputs:         RPM & SledPosition

    Outputs:        none
    Global Inputs:  none
    Global Outputs: none
    Returns:        Overspeed * 100
-----------------------------------------------------------------------------------------------------*/
USHORT spdGetOverSpdX100(USHORT rpm, USHORT StepPosition)
{
    ULONG temp_OverspeedX100;

    if (rpm == 0)
    {
        return 0;
    }
    switch (svoVar_discKindT.Bit.DiscStd)
    {
        case eDiscCD:
            /************************************************************************************
            *                                                                                   *
            *      2 x BPS x PI x Sled_step x parFine                                           *
            *  v = ----------------------------------                                           *
            *                      32                                                           *
            *                                                                                   *
            * BPS = 75 (CD)                                                                     *
            *                                                                                   *
            * 100 * N = 100 * 2 * PI * R * rpm / (60 * V)                                       *
            *         = 100 * 2 * PI * R * rpm * 32 / (60 * 2 * BPS * PI * Sled_step * parFine) *
            *         = 160 * R_step * rpm / (3 * BPS * parFine)                                *
            *         = 32 * R_step * rpm / 45 * parFine                                        *
            *                                                                                   *
            ************************************************************************************/

            temp_OverspeedX100 = (USHORT)((ULONG)32 * StepPosition * rpm / ((ULONG)45 * svoSeekVar.parFineInit));
            break;

        case eDiscDVD:
#if (DVD_RAM_READ == 1)
            if (DVDRAM)
            {
                LONG addr;
                BYTE zone;

                addr = (SeekBusy)? svoSeekVar.svoTargetID.SectorNum.all32s:  svoIramVar.currentID.SectorNum.all32;
                if (ABS(svoPBA2Radius(addr) - StepPosition) > UM_2_SLED_STEP(800))
                {
                    addr = svoRadius2PBA(StepPosition, 0);
                }
                zone = DVDRAM_PSN2ZONE(addr);

                temp_OverspeedX100 = 70 * DVDRAMZoneTable[zone].SectorsPerRev * rpm / 28401;
            }
            else
#endif
            /************************************************************************************
            *                                                                                   *
            *      2 x BPS x PI x Sled_step x parFine                                           *
            *  v = ----------------------------------                                           *
            *                      32                                                           *
            *                                                                                   *
            * BPS = 26156250 / 38688 = 676.08 (DVD)                                             *
            *                                                                                   *
            * 100 * N = 100 * 2 * PI * R * rpm / (60 * V)                                       *
            *         = 100 * 2 * PI * R * rpm * 32 / (60 * 2 * BPS * PI * Sled_step * parFine) *
            *         = 160 * R_step * rpm / (3 * BPS * parFine)                                *
            *         = 25 * R_step * rpm / 317 * parFine                                       *
            *                                                                                   *
            ************************************************************************************/

            temp_OverspeedX100 = (USHORT)((ULONG)25 * StepPosition * rpm / ((ULONG)317 * svoSeekVar.parFineInit));
            break;

#if (BD_ENABLE == 1)
        case eDiscBD:
            /************************************************************************************
            *                                                                                   *
            *      2 x BPS x PI x Sled_step x parFine                                           *
            *  v = ----------------------------------                                           *
            *                      32                                                           *
            *                                                                                   *
            * BPS = 66000000 / 29946 = 2203.97 (BD)                                             *
            *                                                                                   *
            * 100 * N = 100 * 2 * PI * R * rpm / (60 * V)                                       *
            *         = 100 * 2 * PI * R * rpm * 32 / (60 * 2 * BPS * PI * Sled_step * parFine) *
            *         = 160 * R_step * rpm / (3 * BPS * parFine)                                *
            *         = 28  * R_step * rpm / 1157 * parFine                                     *
            *                                                                                   *
            ************************************************************************************/
            temp_OverspeedX100 = (USHORT)((ULONG)28 * StepPosition * rpm / ((ULONG)1157 * svoSeekVar.parFineInit));
            break;
#endif // (BD_ENABLE == 1)
#if (ENABLE_HDDVD == 1)
        case eDiscHD:
            /************************************************************************************
            *                                                                                   *
            *      2 x BPS x PI x Sled_step x parFine                                           *
            *  v = ----------------------------------                                           *
            *                      32                                                           *
            *                                                                                   *
            * BPS = 64800000 / 29016 = 2233.25 (HD)                                             *
            *                                                                                   *
            * 100 * N = 100 * 2 * PI * R * rpm / (60 * V)                                       *
            *         = 100 * 2 * PI * R * rpm * 32 / (60 * 2 * BPS * PI * Sled_step * parFine) *
            *         = 160 * R_step * rpm / (3 * BPS * parFine)                                *
            *         = 32 * R_step * rpm  / 1340 * parFine                                     *
            *                                                                                   *
            ************************************************************************************/
            temp_OverspeedX100 = (USHORT)((ULONG)32 * StepPosition * rpm / ((ULONG)1340 * svoSeekVar.parFineInit));
            break;
#endif // (ENABLE_HDDVD == 1)
    }
    temp_OverspeedX100++;//for float part
#if (PRN_SPEED_CALC == 1)
    // spdGetOverSpdX100 = %d, MotorFrequency = %d (Hz), CurrentPosition = %d
    SendMsgCnB(SHOW_DEBUG_MSG,6,0x331301,B2B(temp_OverspeedX100),B2B(rpm/60),B2B(StepPosition));
#endif // (PRN_SPEED_CALC == 1)
    return temp_OverspeedX100;
}

/*-----------------------------------------------------------------------------
 *
 * Module: spdGetCurrentRPM()
 * Description: return current spindle motor rotation speed in RPM
 *
 *----------------------------------------------------------------------------*/
USHORT spdGetCurrentRPM(void)
{
    ULONG  RevCounts;

    RevCounts = READ_REG(HAL_SRVREG_MAP->REVCNT);

    if (RevCounts != svoSpeedVar.SpdOldRevcnt)
    {
        svoSpeedVar.SpdOldRevcnt = RevCounts;
        if (svoSpeedVar.SpdOldRevcntChk)
        {
            RevCounts *= READ_FIELD(HAL_SRV_MDIV);
            if (RevCounts > 0)
            {
                last_rpm = (USHORT)((ULONG)(SERVO_CLOCK * 60) / RevCounts);
            }
            else
            {
                last_rpm = 0;
            }
        }
        else
        {
            svoSpeedVar.SpdOldRevcntChk = TRUE;
        }
    }

    return last_rpm;
}//end of spdGetCurrentRPM

/*-----------------------------------------------------------------------------
 *
 * Module: spdGetCavSwitchPos()
 * Description: calculate CavSwitchPos for given speed and given max_RPM
 *
 *----------------------------------------------------------------------------*/
USHORT spdGetCavSwitchPos(ESpindleSpeed speed, USHORT* rpm)
{
    BYTE   CLVSpeed;
    ULONG  temp_ulong;
    USHORT CavSwitchPos;

    switch (svoVar_discKindT.Bit.DiscStd)
    {
        case eDiscCD:
            CLVSpeed = CLV_SPEED_CD;
            if (speed <= CLVSpeed)
            {
                temp_ulong = SpeedIndexTableCD[speed] * 45 / 4;//75 * 15 / 100
                *rpm = MIN(CD_PCAV_RPM,*rpm);
            }
            break;

        case eDiscDVD:
#if (DVD_RAM_READ == 1)
            if (DVDRAM)
            {
                USHORT rpm_temp;

                // Set rpm so that speed of zone 0 is equal to 6x at maximum.
                CLVSpeed = CLV_SPEED_DVDRAM;
                if (speed <= CLVSpeed)
                {
                    rpm_temp = 28401UL * 60 / (7 * DVDRAMZoneTable[0].SectorsPerRev) + 1;
                    *rpm = MIN(rpm_temp,*rpm);
                    CavSwitchPos = (28401UL * SpeedIndexTableDVDRAM[speed] + 70 * (*rpm) - 1) / (70 * (*rpm));
                    if (CavSwitchPos > DVDRAMZoneTable[0].SectorsPerRev)
                    {
                        CavSwitchPos -= DVDRAMZoneTable[0].SectorsPerRev;
                        if (CavSwitchPos >= ((DiscTypeIndex == LOC_DISC_TYPE_10)? NUM_ZONES_RAM_10 : NUM_ZONES_RAM_20))
                        {
                            CavSwitchPos = 0xFFFF;
                        }
                    }
                    else
                    {
                        CavSwitchPos = 0;
                    }
                }
            }
            else
#endif
            {
                CLVSpeed = CLV_SPEED_DVD;
                if (speed <= CLVSpeed)
                {
                    *rpm = MIN((svoVar_discKindT.Bit.Layers == eLayersDouble)? DVDDL_PCAV_RPM : DVDSL_PCAV_RPM, *rpm);
#if (DVD_710Run815New == 1)
                    temp_ulong = GetSpeedIndexTableDVD(speed) * 47968 / 473; //676.08 * 15 / 100
#else
                    temp_ulong = SpeedIndexTableDVD[speed] * 47968 / 473; //676.08 * 15 / 100
#endif
                }
            }
            break;

#if (BD_ENABLE == 1)
        case eDiscBD:
            CLVSpeed = CLV_SPEED_BD;
            if (speed <= CLVSpeed)
            {
                *rpm = MIN((svoVar_discKindT.Bit.Layers == eLayersDouble)? BDDL_PCAV_RPM : BDSL_PCAV_RPM, *rpm);
                temp_ulong = SpeedIndexTableBD[speed] * 53887 / 163; //2203.97 * 15 / 100
            }
            break;
#endif // (BD_ENABLE == 1)

#if (ENABLE_HDDVD == 1)
        case eDiscHD:
            CLVSpeed = CLV_SPEED_HD;
            if (speed <= CLVSpeed)
            {
                *rpm = MIN((svoVar_discKindT.Bit.Layers == eLayersDouble)? HDDL_PCAV_RPM : HDSL_PCAV_RPM, *rpm);
                temp_ulong = SpeedIndexTableHD[speed] * 27134 / 81; //2233.25 * 15 / 100
            }
            break;
#endif // (ENABLE_HDDVD == 1)

        default:
            DEBUG_ASSERT(FALSE, eASSERT_IMPOSSIBLE_DISCKIND ); // Let's check here if we aren't running for a unknownDiscKind/DiscStd
            break;
    }

    if (speed > CLVSpeed)
    {//CAV
        CavSwitchPos = 0xFFFF;
    }
    else
#if (DVD_RAM_READ == 1)
    if (!DVDRAM)
#endif
    {
        CavSwitchPos = temp_ulong * svoSeekVar.parFineInit / (8 * *rpm);
        if (CavSwitchPos <= UM_2_SLED_STEP(23000))
        {
            CavSwitchPos = 0;
        }
        else if (CavSwitchPos >= UM_2_SLED_STEP(58000))
        {
            CavSwitchPos = 0xFFFF;
        }
    }
    return CavSwitchPos;
}//end of spdGetCavSwitchPos

/*-----------------------------------------------------------------------------
 *
 * Module: spdInCLVArea()
 * Description: Check if given radius is in CLV area
 *
 *----------------------------------------------------------------------------*/
BOOL spdInCLVArea(SHORT step_pos)
{
    BOOL result;

#if (DVD_RAM_READ == 1)
    if (DVDRAM)
    {
        LONG addr;

        addr = (SeekBusy)? svoSeekVar.svoTargetID.SectorNum.all32s:  svoIramVar.currentID.SectorNum.all32;
        if (ABS(svoPBA2Radius(addr) - step_pos) > UM_2_SLED_STEP(800))
        {
            addr = svoRadius2PBA(step_pos, 0);
        }
        step_pos = DVDRAM_PSN2ZONE(addr);
    }
#endif
    result = (step_pos >= svoSpeedVar.CavSwitchPos);

    return result;
}

/***************************************************************************************
 * Module: spdDiscSpeedWaitStable(BYTE errorratio)
 * Description: Wait spindle for target speed
 *
 * Input  : ratio for slope of rpm variation (%/Sec)
 * Output : none
 ***************************************************************************************/
void spdDiscSpeedWaitStable(BYTE errorratio)
{
    ULONG   spdwaittimeout;
    USHORT  nextfg;
    SHORT   CurRpm;
    LONG    RpmSlopeLimit;// RPM/Sec
    BYTE    samples,enough;

    SendMsg80(DEBUG_CD_WRITE_MSG|DEBUG_DVD_WRITE_MSG,0x331200); // ***spdDiscSpeedWaitStable***

    if (svoSpeedVar.TargetRpm == 0)
        return;

    svoCurvefitCalVar.cal_index = 0;
    svoSpeedVar.fgCount = 0xFFFF;
    samples = svoSpeedVar.TargetRpm / 1200 + 1;
    if (samples < 3) samples = 3;
    enough = FALSE;
    nextfg = 0xFFFF;
    StartFGTimer();
    StartTimer(&spdwaittimeout);
    do
    {
        svoCurvefitCalVar.item[svoCurvefitCalVar.cal_index] = ReadTimer(&spdwaittimeout);
        CurRpm = spdGetCurrentRPM();
        svoCurvefitCalVar.cost[svoCurvefitCalVar.cal_index] = CurRpm;

        if ((!enough) && (!svoSpeedVar.SpdDown) && (svoCurvefitCalVar.cal_index == 0) && ((ABS(CurRpm - svoSpeedVar.TargetRpm) * 100 / svoSpeedVar.TargetRpm) < (errorratio / 2)))
        {
            break;
        }
        svoCurvefitCalVar.cal_index++;
        if (svoCurvefitCalVar.cal_index >= samples)
        {
            svoCurvefitCalVar.cal_index = 0;
            enough = TRUE;
        }
        if ((enough == TRUE) && (svoSpeedVar.SpdInRange == TRUE))
        {
            RpmSlopeLimit = svoSpeedVar.TargetRpm * errorratio;
            if (RpmSlopeLimit > (SPD_MAX_RPM_SLOPE_LIMIT * 100)) RpmSlopeLimit = (SPD_MAX_RPM_SLOPE_LIMIT * 100);
            LineFit(samples, svoCurvefitCalVar.item, svoCurvefitCalVar.cost, svoCurvefitCalVar.cof);
            svoCurvefitCalVar.delta_cost = svoCurvefitCalVar.cof[1] * (1000 * 100);
            if ((ABS(svoCurvefitCalVar.delta_cost) <= RpmSlopeLimit) && ((ABS(CurRpm - svoSpeedVar.TargetRpm) * 100) < (errorratio * svoSpeedVar.TargetRpm / 2)))
            {
                // SpeedErrorRatio = %4.2f (%%)
                SendMsgCnB(DEBUG_CD_WRITE_MSG|DEBUG_DVD_WRITE_MSG,4,0x331203,F4B(svoCurvefitCalVar.delta_cost / svoSpeedVar.TargetRpm));
                break;
            }
        }

        nextfg -= DISK_NFGPLS;
        do
        {
            ExitProcess();
            spdUnhold();
            if (svoSpeedVar.SpdMonitor)
            {
                spdMonitorRPM();
                svoSpeedVar.SpdMonitor = FALSE;
            }

            if ((svoSpeedVar.SpdDown) && (svoSpeedVar.SpdInRange) && (svoSpeedVar.SpdHold == FALSE))
            {
                svoSpeedVar.SpdDown = FALSE;
                spdSetPara();
                SetDfeWblFrontSemiSynchrAfterTracking();
                // ***speed in range (down): %d (mS)***
                SendMsgCnB(SHOW_DEBUG_MSG,2,0x33102E,B2B(svoSpeedVar.SpdWaitCnt * 20));
            }
            if (ReadTimer(&spdwaittimeout) > 6000)
            {
            	SendMsg80(SHOW_DEBUG_MSG,0x331033); //***spdDiscSpeedWaitStable timeout***
        		break;
            }
        }
        while (svoSpeedVar.fgCount > nextfg);
    }
    while (ReadTimer(&spdwaittimeout) < 6000);//6 sec

    StopFGTimer();
}

/*-----------------------------------------------------------------------------
 *
 * Module: spdMonitorRPM()
 * Description: Monitor current RPM during RPM changing
 *
 *----------------------------------------------------------------------------*/
void spdMonitorRPM(void)
{
    USHORT temp_ushort,target_position,spdlimit;

    target_position = Iram_stp_CurrentPosition;
    if (Iram_stp_Status == (STP_BUSY_MASK | STP_DEVICE_NORMAL))
    {
        if (Iram_stp_CurrentDirection == FORW)
        {
            target_position += Iram_stp_JumpDistance - Iram_stp_CurrentDistance;
        }
        else
        {
            target_position -= Iram_stp_JumpDistance - Iram_stp_CurrentDistance;
        }
    }

    if (svoSpeedVar.SpdWaitCnt > 500)
    {
        //500 * 20 = 10000ms = 10sec
        //Speed TIMEOUT, targetRPM:%5u, currentRPM:%5u, integrator:%4d, kdf14:%5d
#if (NEW_SPINDLE_CONTROL == 1)
        SendMsgCnB(SHOW_DEBUG_MSG,8,0x33FF23,B2B(svoSpeedVar.TargetRpm),B2B(spdGetCurrentRPM()),B2B(READ_DSP_COEF(DE4_msb)),B2B(READ_DSP_COEF(kdf14)));
#else
        SendMsgCnB(SHOW_DEBUG_MSG,8,0x33FF23,B2B(svoSpeedVar.TargetRpm),B2B(spdGetCurrentRPM()),B2B(0),B2B(READ_DSP_COEF(kdf14)));
#endif
#if (ACC_CONTROL > 0) || (DEC_CONTROL > 0)
        if (svoSpeedVar.TargetRpmReal != svoSpeedVar.TargetRpm)
        {
            svoSpeedVar.TargetRpmReal = svoSpeedVar.TargetRpm;
            spdSetFGFreq(svoSpeedVar.TargetRpm);
        }
#endif
        svoSpeedVar.CurrentRpm = svoSpeedVar.TargetRpm;
    }
    else
    {
        svoSpeedVar.CurrentRpm = spdGetCurrentRPM();
    }

    spdlimit = SPD_IN_RANGE_LIMIT_X_1000;
    if (svoSpeedVar.TargetRpm > (SPD_IN_RANGE_MAX_DIFF_RPM * 1000 / SPD_IN_RANGE_LIMIT_X_1000))
    {
        spdlimit = SPD_IN_RANGE_MAX_DIFF_RPM * 1000 / svoSpeedVar.TargetRpm;
    }
    else if (svoSpeedVar.TargetRpm < (SPD_IN_RANGE_MIN_DIFF_RPM * 1000 / SPD_IN_RANGE_LIMIT_X_1000))
    {
        spdlimit = SPD_IN_RANGE_MIN_DIFF_RPM * 1000 / svoSpeedVar.TargetRpm;
    }

    temp_ushort = svoSpeedVar.CurrentRpm * 1000 / svoSpeedVar.TargetRpm;

    if ((svoSpeedVar.SpdUp) && (svoSpeedVar.SpdHold == FALSE))
    {
        if (svoSpeedVar.CurrentRpm * 8 >= svoSpeedVar.MinimumRpm * 7)
        {
            svoSpeedVar.SpdUp = FALSE;
            spdSetCAVM(svoSpeedVar.MinimumRpm);
            spdSetFGFreq(svoSpeedVar.TargetRpm);
        }
    }

    if ((temp_ushort >= (1000 - spdlimit)) && (temp_ushort < (1000 + spdlimit)))
    {
        svoSpeedVar.SpdInRange = TRUE;
        spdSetFGFreq(svoSpeedVar.TargetRpm);
#if (NEW_SPINDLE_CONTROL == 1)
        spdSetIntegrator(svoSpeedVar.TargetRpm);
#endif
#if (ACC_CONTROL > 0) || (DEC_CONTROL > 0)
        svoSpeedVar.CurrentRpm = svoSpeedVar.TargetRpmReal;
#else
        svoSpeedVar.CurrentRpm = svoSpeedVar.TargetRpm;
#endif
        svoSpeedVar.SpdOverspd = spdGetTargetOverspeedX100(target_position,svoSpeedVar.SpdCurrentSpeed);
        if (svoSpeedVar.SpdMode != SPD_MODE_CAV)
        {
            UpdateReadFilters((svoSpeedVar.SpdOverspd + 5) / 10);
        }
        ChkUpdateReadFilters();
#if (MSG_OUT_MODE != MSG_DISABLE)
        if (!svoSpeedVar.SpdDown)
        {
            // ***speed in range (update): %d (mS)***
            SendMsgCnB(DEBUG_SVO_SPD_MSG,2,0x33102F,B2B(svoSpeedVar.SpdWaitCnt * 20));
        }
#endif
    }
    else
    {
        svoSpeedVar.SpdInRange = FALSE;
#if (MSG_OUT_MODE != MSG_DISABLE)
        if ((svoSpeedVar.SpdWaitCnt & 0x1F) == 0)//every 640ms
        {
            SendMsgCn(SHOW_DEBUG_MSG,2,0x33102A,A2B(temp_ushort));//***speed monitor: current/target = %4u/1000***
        }
#endif
        if (temp_ushort > 1000)
        {
            temp_ushort = svoSpeedVar.CurrentRpm * 61 / 64;
        }
        else
        {
            temp_ushort = svoSpeedVar.CurrentRpm * 67 / 64;
        }
        svoSpeedVar.SpdOverspd = spdGetOverSpdX100(temp_ushort,target_position);
        ChkUpdateReadFilters();
    }
#if (SPD_MAX_BRAKE_RPM > 0)
    if ((svoSpeedVar.SpdFreeRun) && ((svoSpeedVar.SpdInRange) || (svoSpeedVar.CurrentRpm <= SPD_MAX_BRAKE_RPM)))
    {
        SendMsg80(SHOW_DEBUG_MSG,0x331032); //***SpdFreeRun spin down off***
        svoSpeedVar.SpdFreeRun = FALSE;
        spdSetKd14(0);
    }
#endif
}

/*-----------------------------------------------------------------------------
 *
 * Module: spdWaitSpdInRange()
 * Description: wait svoSpeedVar.SpdInRange == TRUE.
 *              Don't need timeout because svoSpeedVar.SpdInRange itself has a timeout
 *
 *----------------------------------------------------------------------------*/
void spdWaitSpdInRange(void)
{
    while (!svoSpeedVar.SpdInRange)
    {
        if (svoSpeedVar.SpdMonitor)
        {
            spdMonitorRPM();
            svoSpeedVar.SpdMonitor = FALSE;
        }
        ExitProcess();
        if (cancel_flag_check())
        {
            return;
        }
    }
}
/*------------------------------------------------------------------------*/
/* The end of file                                                        */
/*------------------------------------------------------------------------*/
