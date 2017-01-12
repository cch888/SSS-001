/****************************************************************************
*            (c) Copyright 2001 - 2010 Sunext Technology Co., Ltd.
*                           All Rights Reserved
*
*
* FILENAME:   svo_seek.c
*
* DESCRIPTION: This file contains the functions related to seek control
*
* NOTES:       Please add function name below!!
*                -- svoPBA2Radius()
*                -- svo_Layer_Jump()
*                -- svo_Extended_Eq_Data_Set()
*
*   $Revision: 471 $
*   $Date: 11/03/28 4:07p $
*
**************************** SUNEXT CONFIDENTIAL ****************************/
#include ".\common\globals.h"
#include ".\common\globtype.h"
#include ".\common\chip.h"
#include ".\common\commath.h"
#include ".\common\stdlib.h"
#include ".\servo\svo.h"
#include ".\servo\svomacro.h"
#include ".\servo\svo_com.h"
#include ".\servo\svo_tbl.h"
#include ".\servo\svo_seek.h"
#include ".\servo\svoVar.h"
#include ".\servo\svomath.h"
#include ".\servo\svo_lib.h"
#include ".\servo\dvsvo_tbl.h"
#if (DVD_RAM_READ == 1)
#include ".\servo\dvRamSvo_tbl.h"
#endif
#include ".\servo\svo_sta.h"
#include ".\servo\svo_step.h"
#include ".\dvd\dvGvars.h"

#include ".\al\REG_intctrl.h"
#include ".\al\REG_servo.h"
#include ".\al\REG_dfe.h"
#include ".\al\hal_afe.h"
#include ".\al\hal_dfe.h"
#include ".\al\hal_dsp.h"
#include ".\al\hal_pll.h"
#include ".\al\hal_servo.h"
#include ".\al\hal_wobble.h"
#include ".\servo\svo_dsp.h"

#if (ENABLE_HDDVD == 1)
#include ".\Servo\hdSvo_tbl.h"
#include ".\hd\hdDef.h"
#endif // (ENABLE_HDDVD == 1)

#if (BD_ENABLE == 1)
#include ".\servo\bdSvo_tbl.h"
#include ".\bd\bdMacro.h"
#endif // (BD_ENABLE == 1)

/*-----------------------------------------------------------------------------
 *
 * Module: svoPBA2Radius()
 * Description: calculate the radius in unit sledge step
 *              from a given address
 *                         _____________________________________
 *              R         /
 *  steps = --------- =  / parN0 + (parRough x (F - parF0) / 64)
 *          Sled_step  \/
 *
 *----------------------------------------------------------------------------*/
USHORT svoPBA2Radius(LONG pba)
{
    LONG   temp;
    LONG   square;
    ULONG  parN0;
    USHORT steps;
    BYTE   LeadIn;

#if (DVD_RAM_READ == 1)
    if (DVDRAM && (pba < 0x30000))
    {
        //0x61 = 4 * 18 / 0.74
        //148 / 16875 = 0.74 / (18 * 4.6875)
        steps = UM_2_SLED_STEP(24000) - (0x30061 - pba) * UM_2_SLED_STEP(2775) / 67500;
    }
    else
#endif
    {
        LeadIn = FALSE;
        if (svoVar_discKindT.Bit.DiscStd == eDiscDVD)
        {
            if ((pba & 0x00400000) == 0x00400000)
            {//DVD OTP layer 1
                pba = labs(pba | 0xFF800000);
            }
            pba = pba & 0x003FFFFF;
        }
#if (BD_ENABLE == 1)
        else if(svoVar_discKindT.Bit.DiscStd == eDiscBD)
        {
            if ((pba & PBA_BDLAYER1_MASK) == PBA_BDLAYER1_MASK)
            {//BD (OTP Layer0)
                pba = labs(pba | 0xFF000000);
            }
            pba = pba & 0x00FFFFFF;
        }
#endif // (BD_ENABLE == 1)
#if (ENABLE_HDDVD == 1)
        else if(svoVar_discKindT.Bit.DiscStd == eDiscHD)
        {
            if ((pba & 0x00800000) == 0x00800000)
            {//HD OTP layer 1
                pba = labs(pba | 0xFF000000);
            }
            pba = pba & 0x007FFFFF;
            if (pba <= HD_CONNECTION_ZONE_START_ADDR)
            {
                LeadIn = TRUE;
            }
        }
#endif // (ENABLE_HDDVD == 1)
        if (LeadIn)
        {
            temp = pba - svoSeekVar.parF0_Li;
            square = svoSeekVar.parRough_Li;
            parN0 = svoSeekVar.parN0_Li;
        }
        else
        {
            temp = pba - svoSeekVar.parF0;
            square = svoSeekVar.parRough;
            parN0 = svoSeekVar.parN0;
        }

        if (temp > square)
        {
            temp /= 8;
        }
        else
        {
            square /= 8;
        }
        square = (temp * square) / 8 + parN0;
        if (square > 0)
        {
            steps = sqrt((ULONG)square);
        }
        else
        {
            steps = 0;
        }
    }
#if (MSG_OUT_MODE != MSG_DISABLE)
    if ((steps < UM_2_SLED_STEP(21500)) || (steps > UM_2_SLED_STEP(60000)))
    {
        SendMsgCnB(SHOW_DEBUG_MSG,4,0x420025,B4B(pba));
    }
#endif
    return (steps);
} /* svoPBA2Radius */

/*-----------------------------------------------------------------------------
 *
 * Module: svoRadius2PBA()
 * Description: calculate the address of layer 0 from a given radius
 *              in unit sledge step
 *
 *         (steps^2 - parN0) x 64
 *  addr = ---------------------- + parF0
 *                parRough
 *
 *----------------------------------------------------------------------------*/
LONG svoRadius2PBA(USHORT radius,BYTE layer)
{
    LONG   step;
    LONG   pba;
    BYTE   LeadIn;

    /* change radius' unit to sledge step */
    step = radius;

#if (DVD_RAM_READ == 1)
    if (DVDRAM && (step < UM_2_SLED_STEP(24000)))
    {
        //0x61 = 4 * 18 / 0.74
        //148 / 16875 = 0.74 / (18 * 4.6875)
        pba = 0x30061 - (UM_2_SLED_STEP(24000) - step) * 67500 / UM_2_SLED_STEP(2775);
    }
    else
#endif
    {
        LeadIn = FALSE;
#if (BD_ENABLE == 1)
        if ((svoVar_discKindT.Bit.DiscStd == eDiscBD) && (step < UM_2_SLED_STEP(10000)))
        {
            LeadIn = TRUE;
        }
#endif // (BD_ENABLE == 1)
#if (ENABLE_HDDVD == 1)
        if ((svoVar_discKindT.Bit.DiscStd == eDiscHD) && (step < UM_2_SLED_STEP(23800)))
        {
            LeadIn = TRUE;
        }
#endif // (ENABLE_HDDVD == 1)
        step *= step;
        if (LeadIn)
        {
            pba = (step - svoSeekVar.parN0_Li);
            if (pba >= 0x2000000)
            {
                pba = pba * 16 / (svoSeekVar.parRough_Li / 4);
            }
            else
            {
                pba = pba * 64 / svoSeekVar.parRough_Li;
            }
            pba += svoSeekVar.parF0_Li;
        }
        else
        {
            pba = (step - svoSeekVar.parN0);
            if (pba >= 0x2000000)
            {
                pba = pba * 16 / (svoSeekVar.parRough / 4);
            }
            else
            {
                pba = pba * 64 / svoSeekVar.parRough;
            }
            pba += svoSeekVar.parF0;
        }
    }

    if ((layer & LAYERMASK) == LAYERMASK)
    {
        if (
#if (BD_ENABLE == 1)
            (svoVar_discKindT.Bit.DiscStd != eDiscBD) &&
#endif
            (pdbGetDVDTrackPath() == DVDHD_PTP_DISC))
        {
            pba |= 0x80000000;
        }
        else
        {
            pba = ~pba;
        }
    }
#if (BD_ENABLE == 1)
    if(svoVar_discKindT.Bit.DiscStd == eDiscBD)
    {
        pba &= PSNMaskBD;
    }
    else
#endif
    {
        pba &= PSNMask;
    }
    return (pba);
} /* svoRadius2PBA */

/*-----------------------------------------------------------------------------
 *
 * Module: svoSectorsPerRev()
 * Description: calculate how many sectors per revolution for a given radius
 *              in unit sledge step
 *
 *      2 x BPS x PI x Sled_step x parFine
 *  v = ----------------------------------
 *                      32
 *
 *                2 x PI x R x BPS         2 x PI x R x BPS x 32               32 x R
 *  PBA_per_rev = ---------------- = ---------------------------------- = -------------------
 *                       V           2 x BPS x PI x Sled_step x parFine   Sled_step x parFine
 *
 *----------------------------------------------------------------------------*/
BYTE svoSectorsPerRev(USHORT radius)
{
    return RoundDivide(radius * 32, svoSeekVar.parFine);
} /* svoSectorsPerRev */

/*-----------------------------------------------------------------------------
 *
 * Module: svoSectors2Duration() (output unit: mSec)
 * Description: calculate how many ms is need to read given sectors under current speed
 *
 *                 sectors * 1000      sectors * 100000
 *  Duration(ms) = --------------- = --------------------
 *                 overspeed * BPS   overspeed_x100 * BPS
 *
 *----------------------------------------------------------------------------*/
LONG svoSectors2Duration(LONG sectors)
{
    LONG k;//k = 100000 * 32 / BPS

    if (svoSpeedVar.SpdOverspd == 0)
    {
        return 0;
    }

    switch (svoVar_discKindT.Bit.DiscStd)
    {
        case eDiscCD:
            k = 42667;//3200000 / 75
            break;

        case eDiscDVD:
#if (DVD_RAM_READ == 1)
            if (DVDRAM)
            {
                k = 4732;//3200000 / 676.2143122
            }
            else
#endif
            {
                k = 4733;//3200000 / 676.0817308
            }
            break;
#if (BD_ENABLE == 1)
        case eDiscBD:
            k = 1452;//3200000 / 2203.967141
            break;
#endif
#if (ENABLE_HDDVD == 1)
        case eDiscHD:
            k = 1433;//3200000 / 2233.25062
            break;
#endif
    }

    return (sectors * k / (svoSpeedVar.SpdOverspd * 32));
} /* svoSectors2Duration */

/*-----------------------------------------------------------------------------
 *
 * Module: svoDuration2Sectors() (input unit: mSec)
 * Description: calculate how sectors will be read after given ms under current speed
 *
 *            Duration(ms) * overspeed * BPS   Duration(ms) * overspeed_x100 * BPS
 *  sectors = ------------------------------ = -----------------------------------
 *                         1000                              100000
 *
 *----------------------------------------------------------------------------*/
LONG svoDuration2Sectors(LONG duration)
{
    LONG k;//k = 100000 * 32 / BPS

    switch (svoVar_discKindT.Bit.DiscStd)
    {
        case eDiscCD:
            k = 42667;//3200000 / 75
            break;

        case eDiscDVD:
#if (DVD_RAM_READ == 1)
            if (DVDRAM)
            {
                k = 4732;//3200000 / 676.2143122
            }
            else
#endif
            {
                k = 4733;//3200000 / 676.0817308
            }
            break;
#if (BD_ENABLE == 1)
        case eDiscBD:
            k = 1452;//3200000 / 2203.967141
            break;
#endif
#if (ENABLE_HDDVD == 1)
        case eDiscHD:
            k = 1433;//3200000 / 2233.25062
            break;
#endif
    }

    return ((duration * svoSpeedVar.SpdOverspd * 32) / k);
} /* svoDuration2Sectors */

// =================================================== Layer Jump ================================

/*-----------------------------------------------------------------------------
 *
 * Module: svo_Layer_Jump()
 * Description: Dual Layer Jump
 *
 *----------------------------------------------------------------------------*/
#define LJ_COLLI_2_JUMPS 0 //split colli jump to 2
#define LAYER_JUMP_SURF  0

#if 0
#define JUMP_TIMEOUT     0x80
#define JUMP_KICKSHORT   0x40
#define JUMP_WAITLONG    0x20
#define JUMP_BRAKELONG   0x10
#define JUMP_KICKLONG    0x08
#define JUMP_SPCFC       0x01

#define JUMP_BOUND       0x08
#define JUMP_UP_BND      0x80
#define JUMP_DN_BND      0x08
#define JUMP_ROC_BND     JUMP_BOUND

#define JUMP_ACTTIME     2000 //in 1us
#define SAMP_TIME        3000 //in 1us

#if(PCB == 815)
#define JL_MAX_KICK      0   //in 100us
#else
#define JL_MAX_KICK      20   //in 100us
#endif
#define JL_MIN_KICK      3    //in 100us
#define JL_MAX_WAIT      20   //in 100us
#define JL_MIN_WAIT      10   //in 100us
#define JL_MAX_BRAKE     20   //in 100us
#define JL_MIN_BRAKE     2    //in 100us

BYTE svo_Layer_Jump(BYTE toLayer)
{
    BYTE  orig_FELPF;
    BYTE  tries,ret,ramping,f1_level_cur,fok;
    BYTE  jump_dir;
    SBYTE delta_fe_gn;
    LONG  foo_os;
    ULONG temp_timer;
    BYTE  temp_timeout;
    ad_dsp_t te_signal;
#if (BD_ENABLE == 1) && (COLLI_SWITCH == 1) && (LJ_COLLI_2_JUMPS == 0)
    USHORT   te_thr2;
#endif
#if (LJ_COLLI_2_JUMPS == 1)
    BYTE  f1_level_tar;
#endif
#if (LAYER_JUMP_SURF == 0)
    LONG  sample_num;
#endif
    BYTE  jump_cond;
    BYTE  i_roc, i_roc_min, i_roc_prej, i_roc_prefer;
    SHORT foc_roc_min;
    BYTE  orig_TELpfBW;

    if ((svoVar_discKindT.Bit.DiscStd == eDiscCD) || (svoVar_discKindT.Bit.Layers == eLayersSingle) || (Layer_Index == toLayer))
        return(Ready);

#if (LJ_COLLI_2_JUMPS == 1)
    f1_level_tar = LJ_FSL;
#endif
#if (SRC_ENABLE == 1)
    #if (SRC_ENABLE_FOCUS == 1)
    HAL_DSP_SRCHold(SRC_LOOP_FOCUS, ON, ON, ON);
    #endif
    HAL_DSP_SRCHold(SRC_LOOP_TRACKING, ON, ON, ON);
#endif
    HAL_DSP_SetDebug(DEBUG_CH1, DEBUG_SOURCE_RFILE, 0x24, 8);//set debug CH1 as FE_13

#if (BD_ENABLE == 1)
    if (svoVar_discKindT.Bit.DiscStd == eDiscBD)
    {
    #if (COLLI_SWITCH == 1)
        Defect_Enable(DEFECT_STATE_ONLY_FOCUS_ON|DEFECT_SOURCE_SERVO_BS);
        //TON_ON_OFF(OFF);
        WRITE_REG(HAL_SRVREG_MAP->PUSTP, 0x00);             /* tracking open loop */
        #if (CHIP_REV <= 0xB3)
        WRITE_FIELD(HAL_AFE_TOKLVL, halAFE_TOKLVL_VREF_300mV);
        #else
        WRITE_FIELD(HAL_AFE_TOKLVL, halAFE_TECP_TECN_LVL_300mV);
        #endif
        #if (LJ_COLLI_2_JUMPS == 1)
        svoStp_JumpToPosition(COLLI_NORMAL_MODE, (calGetOptColliPos(0) + calGetOptColliPos(1)) / 2);
        temp_timer = svoCalMisc1Var[!toLayer].feGainDSP * (svoCalDlVar[!toLayer].fe_factor + 3072) / 4096;
        temp_timer = (temp_timer > 0x7FFF)? 0x7FFF : temp_timer;
        WRITE_DSP_COEF(kfg, temp_timer);
        #else
        svoStp_JumpToPosition(COLLI_NORMAL_MODE, calGetOptColliPos(toLayer));
        temp_timer = svoCalMisc1Var[!toLayer].feGainDSP * (svoCalDlVar[!toLayer].fe_factor + 1024) / 2048;
        temp_timer = (temp_timer > 0x7FFF)? 0x7FFF : temp_timer;
        WRITE_DSP_COEF(kfg, temp_timer);
        #endif
        apply_FEBC(0);
        apply_TILT(TILT_INIT_DAC);
        while (svoStp_busy())
        {
            ExitProcess();
        }
        #if (LJ_COLLI_2_JUMPS == 1)
        temp_timer = svoCalMisc1Var[!toLayer].feGainDSP * (svoCalDlVar[!toLayer].fe_factor + 1024) / 2048;
        temp_timer = (temp_timer > 0x7FFF)? 0x7FFF : temp_timer;
        WRITE_DSP_COEF(kfg, temp_timer);
        f1_level_cur = LJ_FSL * 2048 / (svoCalDlVar[!toLayer].fe_factor + 1024);
        f1_level_tar = LJ_FSL * 2048 / (svoCalDlVar[toLayer].fe_factor + 1024);
        #else
        temp_timer = svoCalMisc1Var[!toLayer].feGainDSP * svoCalDlVar[!toLayer].fe_factor / 1024;
        temp_timer = (temp_timer > 0x7FFF)? 0x7FFF : temp_timer;
        WRITE_DSP_COEF(kfg, temp_timer);
        f1_level_cur = LJ_FSL * 1024 / svoCalDlVar[!toLayer].fe_factor;
        WRITE_FIELD(HAL_AFE_TEBG,       svoCalGnT3Var[toLayer][TEB_GN].normal_gain);
        WRITE_FIELD(HAL_AFE_TRKG,       svoCalGnT1Var[toLayer][TE_GN].groove_gain[RECDH]);
        WRITE_FIELD(HAL_AFE_TRKO,       svoCalOfsT1Var[toLayer][TE_OS].groove_offset[RECDH]);
        WRITE_FIELD(HAL_AFE_BLANKTRKG,  svoCalGnT1Var[toLayer][TE_GN].groove_gain[RECDL]);
        WRITE_FIELD(HAL_AFE_BLANKTRKO,  svoCalOfsT1Var[toLayer][TE_OS].groove_offset[RECDL]);
        te_thr2 = COEF_GetValue(AFE_CALC_TE_PP_TARGET_10MV);
        svoCalDlVar[toLayer].dpp_thr = te_thr2;
        afeSetTrackingMethod(TRK_METHOD_DPP);
        orig_TELpfBW = HAL_AFE_GetTELpfBW();
        HAL_AFE_SetTELpfBW(e_TELPF_660);
        if (svoVar_discKindT.Bit.Media == eMediaROM)
        {
            te_thr2 /= 3;
        }
        else
        {
            te_thr2 *= 2;
        }
        te_signal = ReadFilteredDSPADCmV(ONE_REVOLUTION, TE_ALL, ADC_TE, TRUE);

        if (te_signal.errcode == eNOERR)
        {
            svoCalDlVar[toLayer].dpp_thr += te_signal.p2p_mv;
        }
        te_thr2 += svoCalDlVar[toLayer].dpp_thr;
        #endif
    #else // (COLLI_SWITCH == 1)
        f1_level_cur = LJ_FSL;
    #endif // (COLLI_SWITCH == 1)
        jump_dir = (toLayer == 0)? UP : DOWN;
    }
    else
#endif // (BD_ENABLE == 1)
    {
        jump_dir = (toLayer == 1)? UP : DOWN;
        f1_level_cur = LJ_FSL;
    }

    SetDfeWblFrontOnHold();

    delta_fe_gn = svoCalGnT1Var[toLayer][FE_GN].groove_gain[1] - svoCalGnT1Var[!toLayer][FE_GN].groove_gain[1];
#if (PRESET_TARGET_LAYER_FCSG == 1)
    f1_level_cur = (BYTE)(CalculateRateFromXdB(ONEdB,delta_fe_gn) * f1_level_cur);
#endif
    WRITE_FIELD(HAL_SRV_SBYLVL,LJ_SB_LEVEL_HYSTERESIS);
    if (f1_level_cur < LJ_SB_LEVEL_HYSTERESIS + 1)
    {
        f1_level_cur = LJ_SB_LEVEL_HYSTERESIS + 1;
    }
#if FocusLockAid_Enable == 1
    HAL_DSP_FocusLockAid(OFF, UP);
#endif // FocusLockAid_Enable == 1

#if (NEW_FDP_DETECTION == 1)
    WRITE_FIELD(HAL_SRV_FNOKEN,0); //turn off Focus drop interrupt, will be enabled in focus_on_chk()
#endif
#if (NEW_FDP_DETECTION == 2)
    WRITE_FIELD(HAL_SRV_DSPI6EN,0); //Turn off DSP int6
#endif

    fok = FALSE;
    ramping = FALSE;
    ret = Error;
    for (tries = LJ_RETRYCNT; tries > 0; tries--)
    {
        if (!ramping)//use layer jump (faster)
        {
            apply_FEBC(0);
            if (FokStatusOK())
            {
                orig_FELPF = READ_REG(HAL_AFEREG_MAP->FCTS32) & (HAL_AFE_FEFCSFSEL_MASK|HAL_AFE_FELPSET_MASK|HAL_AFE_BPFELP_MASK);
                READ_MOD_WRITE(HAL_AFEREG_MAP->FCTS32, (HAL_AFE_FEFCSFSEL_MASK|HAL_AFE_FELPSET_MASK|HAL_AFE_BPFELP_MASK),
                    halAFE_FELPF_15kHz << HAL_AFE_FEFCSFSEL_LSB);

                jump_cond = 0;
                if ((READ_FIELD(HAL_SRV_SRCFOCEN)) &&
                    (((svoVar_discKindT.Bit.DiscStd == eDiscBD) && (svoSpeedVar.SpdMode == SPD_MODE_CAV)) ||
                    ((svoVar_discKindT.Bit.DiscStd == eDiscDVD) && (Disc_Speed.Read >= e8XDCAV))))
                {
                    if (JUMP_ROC_BND >= JUMP_BOUND || ((jump_dir == UP) && !(LJ_FailCnt & JUMP_UP_BND)) || ((jump_dir == DOWN) && !(LJ_FailCnt & JUMP_DN_BND)))
                    {
                        jump_cond |= JUMP_SPCFC;
                    }
                }
                else
                {
                    LJ_FailCnt = 0;
                }
                i_roc_prej = (BYTE) ((USHORT) JUMP_ACTTIME / ((ULONG) (READ_REG(HAL_SRVREG_MAP->REVCNT) * READ_REG(HAL_SRVREG_MAP->CAVM) * 1000 / SERVO_CLOCK) \
                                                         * (500 * (DISK_FR2X + 1)) / READ_FIELD(HAL_SRV_SRCN)));
                if (jump_cond & JUMP_SPCFC)
                {
                    foc_roc_min =  32767;
                    for (i_roc = 0; i_roc < READ_FIELD(HAL_SRV_SRCN); i_roc++)
                    {
                        if (READ_DSP_COEF((USHORT) foc_roc_0 + (i_roc << 2)) < foc_roc_min)
                        {
                            foc_roc_min = READ_DSP_COEF((USHORT) foc_roc_0 + (i_roc << 2));
                            i_roc_min = i_roc;
                        }
                    }
                    if (jump_dir == UP)
                    {
                        i_roc = i_roc_min + (READ_FIELD(HAL_SRV_SRCN) / 2 * 3) - i_roc_prej - 2 + ((LJ_FailCnt & 0xF0) >> 4);
                    }
                    else
                    {
                        i_roc = i_roc_min + (READ_FIELD(HAL_SRV_SRCN) / 2 * 3) - i_roc_prej + 1 + (LJ_FailCnt & 0x0F);
                    }
                    i_roc_prefer = i_roc;
                    while (i_roc_prefer >= READ_FIELD(HAL_SRV_SRCN)){i_roc_prefer -= READ_FIELD(HAL_SRV_SRCN);}
                }
                else
                {
                    i_roc_prefer = READ_FIELD(HAL_SRV_FGCOUNT);
                }

#if (LAYER_JUMP_SURF == 0)
                //Modify FOO offset before jump layer ***
                i_roc = (BYTE)((USHORT) i_roc_prej * SAMP_TIME / JUMP_ACTTIME);
                if (jump_cond & JUMP_SPCFC)
                {
                    i_roc_min = i_roc_prefer + READ_FIELD(HAL_SRV_SRCN) - i_roc;
                    while (i_roc_min >= READ_FIELD(HAL_SRV_SRCN)){i_roc_min -= READ_FIELD(HAL_SRV_SRCN);}
                    StartTimer(&temp_timer);
                    while ((READ_FIELD(HAL_SRV_FGCOUNT) != i_roc_min) && FokStatusOK() && ReadTimer(&temp_timer) < MS_30);
                }
                else
                {
                    if (i_roc < 2){i_roc = 2;}
                    i_roc_prefer += i_roc;
                    while (i_roc_prefer >= READ_FIELD(HAL_SRV_SRCN)){i_roc_prefer -= READ_FIELD(HAL_SRV_SRCN);}
                }

#endif
#if (LAYER_JUMP_SURF == 0)
                InitLPFIIR(0,1000);
                sample_num = 0;
                foo_os = 0;
                StartTimer(&temp_timer);
                do
                {
    #if (FOO_INVERSION == 0)
                    foo_os += LPFIIR(READ_DSP_COEF(debug1));//FE_13, FELFG output
    #else
                    foo_os -= LPFIIR(READ_DSP_COEF(debug1));//FE_13, FELFG output
    #endif
                    sample_num++;
                }
#endif
                while ((READ_FIELD(HAL_SRV_FGCOUNT) != i_roc_prefer) && FokStatusOK() && ReadTimer(&temp_timer) < MS_5);
#if (LAYER_JUMP_SURF == 0)
                foo_os = (foo_os / sample_num * READ_DSP_COEF(src_kf2) / 0x8000 * READ_DSP_COEF(kfg) / 0x8000) + HAL_SERVO_GetFoos();
#endif
                Defect_Enable(DEFECT_STATE_ALL_OFF|DEFECT_SOURCE_SERVO_BS);
                //apply focus control related parameters for target layer
                //we can't predict the RECD of target layer, always use RECDH
#if (PRESET_TARGET_LAYER_FCSG == 1)
                GAIN_RECD_HIGH;//restore in focus_on_chk()
                WRITE_FIELD(HAL_AFE_BLANKFCSG,svoCalGnT1Var[toLayer][FE_GN].groove_gain[RECDL]);
                WRITE_FIELD(HAL_AFE_FCSG,     svoCalGnT1Var[toLayer][FE_GN].groove_gain[RECDH]);
                WRITE_FIELD(HAL_AFE_WRFCSG,   svoCalGnT1Var[toLayer][FE_GN].write_gain);
                WRITE_FIELD(HAL_AFE_BSKG,     svoCalGnT2Var[toLayer][BS_GN].normal_gain);
                WRITE_FIELD(HAL_AFE_BLANKBSKG,svoCalGnT2Var[toLayer][BS_GN].write_gain);
                WRITE_DSP_COEF(kfg, svoCalMisc1Var[toLayer].feGainDSP);
#else
                WRITE_DSP_COEF(kfg, (svoCalMisc1Var[0].feGainDSP + svoCalMisc1Var[1].feGainDSP) / 2);
#endif
#if (LAYER_JUMP_SURF == 1)
                HAL_DSP_LayerJumpSurf(ON);
#endif
                //TON_ON_OFF(OFF);
                WRITE_REG(HAL_SRVREG_MAP->PUSTP, 0x00);             /* tracking open loop */
                #if (CHIP_REV <= 0xB3)
                WRITE_FIELD(HAL_AFE_TOKLVL, halAFE_TOKLVL_VREF_300mV);
                #else
                WRITE_FIELD(HAL_AFE_TOKLVL, halAFE_TECP_TECN_LVL_300mV);
                #endif
#if (AFE_NORMALIZER == 1)
                afeSvognDisable();
#endif // AFE_NORMALIZER == 1
                if (jump_dir == UP)
                {
#if (LJ_COLLI_2_JUMPS == 1)
                    HAL_SERVO_SetFSL(f1_level_tar, f1_level_cur);
#else
                    HAL_SERVO_SetFSL(LJ_FSL, f1_level_cur);
#endif
                    HAL_SERVO_SetFocUp(HAL_SRV_FSONSET);
                }
                else
                {
#if (LJ_COLLI_2_JUMPS == 1)
                    HAL_SERVO_SetFSL(f1_level_cur, f1_level_tar);
#else
                    HAL_SERVO_SetFSL(f1_level_cur, LJ_FSL);
#endif
                    HAL_SERVO_SetFocDn(HAL_SRV_FSONSET);
                }
#if (LAYER_JUMP_SURF == 0)
                HAL_SERVO_SetFoos(foo_os + ((jump_dir == UP)? LJ_FOO_OS_ADD : -LJ_FOO_OS_ADD));
#endif

#if (JL_MAX_KICK > 0)
                HAL_DSP_SetDebug(DEBUG_CH1, DEBUG_SOURCE_RGLUE, 0x26, 0);//set debug CH1 as FCS
                foo_os = HAL_SERVO_GetFoos();
                delta_fe_gn = (SBYTE)((SHORT) READ_REG(HAL_SRVREG_MAP->DLFSKLVL) * READ_DSP_COEF(kf18) / 32768);
#endif

#if (JL_MAX_KICK > 0) // kick long / short protection
                i_roc = 0;
                WRITE_FIELD(HAL_SRV_PKBOT, 0);
                WRITE_FIELD(HAL_SRV_PKBOT, 1);
    #if(FOO_INVERSION == 0)
                WRITE_REG(HAL_SRVREG_MAP->RGSEL, ((jump_dir == UP) ? halSRV_RSEL_FEPEAK : halSRV_RSEL_FEBOTTOM));
    #else
                WRITE_REG(HAL_SRVREG_MAP->RGSEL, ((jump_dir == UP) ? halSRV_RSEL_FEBOTTOM : halSRV_RSEL_FEPEAK));
    #endif

                while (1)
                {
                    while (i_roc < JL_MAX_KICK)
                    {
                        i_roc ++;
                        Delay100uS(1);
                        i_roc_min = (jump_dir == UP) ? (READ_REG(HAL_SRVREG_MAP->SRCR) - svoRampMeasVar.refFE) :
                                                       (svoRampMeasVar.refFE - READ_REG(HAL_SRVREG_MAP->SRCR));
                        if (!READ_DSP_COEF(debug1))
                        {
                            break;
                        }
                        else if (!(jump_cond & JUMP_KICKSHORT) && (svoVar_discKindT.Bit.DiscStd == eDiscBD))
                        {
                            if (!(i_roc_min & 0x80) && (i_roc_min > LJ_FSL))
                            {
                                jump_cond |= JUMP_KICKLONG;
                                break;
                            }
                        }
                    }
                    if (i_roc < JL_MIN_KICK) //kick end too early
                    {
                        jump_cond |= JUMP_KICKSHORT;
                        HAL_SERVO_SetFoos(foo_os + ((jump_dir == UP)? delta_fe_gn : -delta_fe_gn));
                    }
                    else if (!(jump_cond & JUMP_KICKSHORT) || (i_roc > JL_MIN_KICK ))
                    {
                        if (jump_cond & JUMP_KICKSHORT){HAL_SERVO_SetFoos(foo_os);}
                        break;
                    }
                }

                if ((i_roc == JL_MAX_KICK) || (jump_cond & JUMP_KICKLONG))
                {
                    jump_cond |= JUMP_TIMEOUT;
                    if (jump_dir == UP)
                    {
                        HAL_SERVO_SetFocUp(0);
                    }
                    else
                    {
                        HAL_SERVO_SetFocDn(0);
                    }
                }
                else
                {
                    Delay100uS(1);
                }
#else
#if (BD_ENABLE == 1)
                if (svoVar_discKindT.Bit.DiscStd == eDiscBD)
                {
                    Delay100uS(LJ_FCS_DELAY_BD);
                }
                else
#endif
                {
                    Delay100uS(LJ_FCS_DELAY);
                }
#endif
#if (PRESET_TARGET_LAYER_FCSG != 1)
                GAIN_RECD_HIGH;//restore in focus_on_chk()
                WRITE_FIELD(HAL_AFE_BLANKFCSG,svoCalGnT1Var[toLayer][FE_GN].groove_gain[RECDL]);
                WRITE_FIELD(HAL_AFE_FCSG,     svoCalGnT1Var[toLayer][FE_GN].groove_gain[RECDH]);
                WRITE_FIELD(HAL_AFE_WRFCSG,   svoCalGnT1Var[toLayer][FE_GN].write_gain);
                WRITE_FIELD(HAL_AFE_BSKG,     svoCalGnT2Var[toLayer][BS_GN].normal_gain);
                WRITE_FIELD(HAL_AFE_BLANKBSKG,svoCalGnT2Var[toLayer][BS_GN].write_gain);
#endif

                if (!(jump_cond & JUMP_TIMEOUT))
                {
#if FocusLockAid_Enable == 1
                    HAL_DSP_FocusLockAid(ON, jump_dir);//This must be excuted after kick pulse, if not, adjust kick level or the delay time.
#endif // FocusLockAid_Enable == 1
                    if (svoVar_discKindT.Bit.DiscStd == eDiscBD)
                    {
                        i_roc = 0;
                        while (i_roc < JL_MAX_WAIT)
                        {
                            if (READ_DSP_COEF(debug1) || !(READ_FIELD(HAL_SRV_FSRCH)))
                            {
                                break;
                            }
                            if (i_roc >= JL_MIN_WAIT) //wait too long
                            {
                                jump_cond |= JUMP_WAITLONG;
                                HAL_SERVO_SetFoos(foo_os + ((jump_dir == UP)? delta_fe_gn : -delta_fe_gn));
                            }
                            i_roc ++;
                            Delay100uS(1);
                        }
                        if (jump_cond & JUMP_WAITLONG){HAL_SERVO_SetFoos(foo_os);}
                        i_roc = 0;
                        while (READ_FIELD(HAL_SRV_FSRCH) && i_roc < JL_MAX_BRAKE)
                        {
                            if (!READ_DSP_COEF(debug1))
                            {
                                i_roc ++;
                            }
                            if (i_roc >= JL_MIN_BRAKE) //brake too long
                            {
                                HAL_DSP_FocusLockAid(OFF, jump_dir);
                                jump_cond |= JUMP_BRAKELONG;
                                HAL_SERVO_SetFoos(foo_os + ((jump_dir == UP)? delta_fe_gn : -delta_fe_gn));
                            }
                            Delay100uS(1);
                        }
                        if (jump_cond & JUMP_BRAKELONG){HAL_SERVO_SetFoos(foo_os);}
                        if (i_roc == JL_MAX_BRAKE)
                        {
                            jump_cond |= JUMP_TIMEOUT;
                        }
                    }
                    else
                    {
                        StartTimer(&temp_timer);
                        while (READ_FIELD(HAL_SRV_FSRCH) && (!READ_FIELD(HAL_SRV_PROTACT)) && (ReadTimer(&temp_timer) < LJ_WAIT_FSRCH_T));
                        if (ReadTimer(&temp_timer) >= LJ_WAIT_FSRCH_T)
                        {
                            jump_cond |= JUMP_TIMEOUT;
                        }
                    }
                }
                READ_MOD_WRITE(HAL_AFEREG_MAP->FCTS32, (HAL_AFE_FEFCSFSEL_MASK|HAL_AFE_FELPSET_MASK|HAL_AFE_BPFELP_MASK), orig_FELPF);

#if (BD_ENABLE == 1)
                if (svoVar_discKindT.Bit.DiscStd == eDiscBD)
                {
    #if (COLLI_SWITCH == 1) && (LJ_COLLI_2_JUMPS == 1)
                    svoStp_JumpToPosition(COLLI_NORMAL_NO_REST_MODE, calGetOptColliPos(toLayer));
                    temp_timer = svoCalMisc1Var[toLayer].feGainDSP * (svoCalDlVar[toLayer].fe_factor + 3072) / 4096;
                    temp_timer = (temp_timer > 0x7FFF)? 0x7FFF : temp_timer;
                    WRITE_DSP_COEF(kfg, temp_timer);
                    while (svoStp_busy())
                    {
                        ExitProcess();
                    }
                    WRITE_DSP_COEF(kfg, svoCalMisc1Var[toLayer].feGainDSP);
    #endif
                }
#endif //(BD_ENABLE == 1)
#if (LAYER_JUMP_SURF == 1)
                HAL_DSP_LayerJumpSurf(OFF);
#endif
                if (READ_FIELD(HAL_SRV_PROTACT) || (jump_cond & JUMP_TIMEOUT))
                {
#if (NEW_FDP_DETECTION == 1)
                    svoFDropIsr();
#else
                    foo_os = svoCalMisc2Var.fooos - F_SEARCH_T * FOCUS_SEARCH_KF18 / 0x20 * FOC_RAMP_FACTOR / 0x20000;
                    if (foo_os < -FOO_OFFSET_LIMIT) foo_os = -FOO_OFFSET_LIMIT;
                    HAL_SERVO_SetFoos(foo_os);
                    WRITE_FIELD(HAL_SRV_LDOFF, 0x00);
                    WRITE_FIELD(HAL_SRV_LDON, 0x00);
    #if (FOOFFSET2_ENABLE == 1)
                    WRITE_DSP_COEF(kf18,0);//bug fixed: this action must be done after LDOFF/ON when FOOFFSET2_ENABLE == 1
                    HAL_SERVO_SetFocDn(0);
    #endif
#endif
                    SendMsg80(SHOW_DEBUG_MSG,0x334010); //Layer Jump kick or brake timeout
                }
                else
                {
                    fok = focus_on_chk(&te_signal.p2p_mv);
                }

                if (!fok)
                {
                    if (jump_cond & JUMP_SPCFC)
                    {
                        i_roc = (jump_dir == UP) ? ((LJ_FailCnt & 0xF0) >> 4) : (LJ_FailCnt & 0x0F);
                        if (JUMP_ROC_BND >= JUMP_BOUND || i_roc < JUMP_ROC_BND)
                        {
                            i_roc ++;
                            i_roc &= 0x0F;
                        }
                        else
                        {
                            i_roc |= JUMP_BOUND;
                        }
                        LJ_FailCnt = (jump_dir == UP) ? ((LJ_FailCnt & 0x0F) | (i_roc << 4)) : ((LJ_FailCnt & 0xF0) | i_roc);
                    }
                    SendMsgCn(SHOW_DEBUG_MSG,1,0x334007,tries); //WARNING: Layer jump (kick) NG
                }
            }
        }
        else //use ramping (slower)
        {
            foo_os = svoCalMisc2Var.fooos - F_SEARCH_T * FOCUS_SEARCH_KF18 / 0x20 * FOC_RAMP_FACTOR / 0x20000;
            if (foo_os < -FOO_OFFSET_LIMIT) foo_os = -FOO_OFFSET_LIMIT;

            if (FokStatusOK())
            {
                Defect_Enable(DEFECT_STATE_ALL_OFF);
                //hold at bottom
                HAL_SERVO_SetFoos(foo_os);
                WRITE_FIELD(HAL_SRV_LDOFF, 0x00);
                WRITE_FIELD(HAL_SRV_LDON, 0x00);
#if (FOOFFSET2_ENABLE == 1)
                WRITE_DSP_COEF(kf18,0);//bug fixed: this action must be done after LDOFF/ON when FOOFFSET2_ENABLE == 1
                HAL_SERVO_SetFocDn(0);
#endif
                //wait 20ms for lens to stable
                WaitTimer(20);
            }

#if (NEW_FDP_DETECTION != 0)
            svoIramVar.focusDropIsrFlag = FALSE;    // clear DSP focus drop flag
#endif

#if (AFE_NORMALIZER == 1)
            afeSvognDisable();
#endif // AFE_NORMALIZER == 1
            //apply focus control related parameters for target layer
            //marked until DVDRAM DL take place
            //we can't predict the RECD of target layer, always use RECDH

            //forced RECD2 to high.
            GAIN_RECD_HIGH;//restore in focus_on_chk()
            WRITE_FIELD(HAL_AFE_BLANKFCSG,svoCalGnT1Var[toLayer][FE_GN].groove_gain[RECDL]);
            WRITE_FIELD(HAL_AFE_FCSG,     svoCalGnT1Var[toLayer][FE_GN].groove_gain[RECDH]);
            WRITE_FIELD(HAL_AFE_WRFCSG,   svoCalGnT1Var[toLayer][FE_GN].write_gain);
            WRITE_FIELD(HAL_AFE_BSKG,     svoCalGnT2Var[toLayer][BS_GN].normal_gain);
            WRITE_FIELD(HAL_AFE_BLANKBSKG,svoCalGnT2Var[toLayer][BS_GN].write_gain);
            WRITE_DSP_COEF(kfg,           svoCalMisc1Var[toLayer].feGainDSP);

#if (FOCUS_LOCK_FROM_DOWN_TO_UP==1)
            if ((jump_dir == UP) && (svoVar_discKindT.Bit.DiscStd != eDiscBD))
#else
            if (jump_dir == UP)
#endif
            {
                WRITE_FIELD(HAL_SRV_PKBOT,0);
                WRITE_FIELD(HAL_SRV_PKBOT,1);
#if (FOOFFSET2_ENABLE == 1)
                WRITE_REG(HAL_SRVREG_MAP->LDOFF,0x00);
                WRITE_REG(HAL_SRVREG_MAP->LDON, 0x00);
                WRITE_DSP_COEF(kf18,FOCUS_SEARCH_KF18);
#endif
                HAL_SERVO_SetFocUp(HAL_SRV_HISRSET);
                temp_timeout = F_SEARCH_T;
                StartTimer(&temp_timer);
                while (ReadTimer(&temp_timer) < temp_timeout)
                {
                    WRITE_REG(HAL_SRVREG_MAP->RGSEL,halSRV_RSEL_BSPEAK);
                    if (READ_REG(HAL_SRVREG_MAP->SRCR) > ((svoCalVar.bsOfsData.all16 >> 2) + (COEF_GetValue(AFE_CALC_BS_TARGET_10MV) * (2 * LJ_RETRYCNT - tries) * ((10 << DSPADC_BITS) / (2 * LJ_RETRYCNT)) / DSPADC_IN_RANGE)))
                    {//BS > (0.5 ~ 1) * BS_target
                        temp_timeout = ReadTimer(&temp_timer) + (F_PP_DELAY_T + 1) / 2;
                        if (temp_timeout > F_SEARCH_T) temp_timeout = F_SEARCH_T;
                        WRITE_FIELD(HAL_SRV_PKBOT,0);
                        WRITE_FIELD(HAL_SRV_PKBOT,1);
                    }
                }
            }

            HAL_SERVO_SetFebc(0);
#if FocusLockAid_Enable == 1
    #if(FOCUS_LOCK_FROM_DOWN_TO_UP==1)
            HAL_DSP_FocusLockAid(ON, ((jump_dir == UP) && (svoVar_discKindT.Bit.DiscStd != eDiscBD))? DOWN : UP);
    #else
            HAL_DSP_FocusLockAid(ON, (jump_dir == UP)? DOWN : UP);
    #endif //FOCUS_LOCK_FROM_DOWN_TO_UP==1
#endif // FocusLockAid_Enable == 1
#if(FOCUS_LOCK_FROM_DOWN_TO_UP==1)
            if ((jump_dir == UP) && (svoVar_discKindT.Bit.DiscStd != eDiscBD))
#else
            if (jump_dir == UP)
#endif
            {
#if (BD_ENABLE == 1)
                HAL_SERVO_SetFSL(0x7F, (svoVar_discKindT.Bit.DiscStd == eDiscBD)? FOCUS_CAPTURE_FSL_BD : FOCUS_CAPTURE_FSL);
#else
                HAL_SERVO_SetFSL(0x7F, FOCUS_CAPTURE_FSL);
#endif
                HAL_SERVO_SetFocDn(HAL_SRV_FSONSET|HAL_SRV_FGCSET);
            }
            else
            {
#if (BD_ENABLE == 1)
                HAL_SERVO_SetFSL((svoVar_discKindT.Bit.DiscStd == eDiscBD)? FOCUS_CAPTURE_FSL_BD : FOCUS_CAPTURE_FSL, 0x7F);
#else
                HAL_SERVO_SetFSL(FOCUS_CAPTURE_FSL, 0x7F);
#endif
#if (FOOFFSET2_ENABLE == 1)
                WRITE_REG(HAL_SRVREG_MAP->LDOFF,0x00);
                WRITE_REG(HAL_SRVREG_MAP->LDON, 0x00);
                WRITE_DSP_COEF(kf18,FOCUS_SEARCH_KF18);
#endif
                HAL_SERVO_SetFocUp(HAL_SRV_FSONSET|HAL_SRV_FGCSET);
            }
            WRITE_FIELD(HAL_SRV_PKBOT,0);
            WRITE_FIELD(HAL_SRV_PKBOT,1);
            temp_timeout = F_SEARCH_T * 2;
            StartTimer(&temp_timer);
            while (READ_FIELD(HAL_SRV_FSRCH) && (ReadTimer(&temp_timer) < temp_timeout))
            {
#if(FOCUS_LOCK_FROM_DOWN_TO_UP==1)
                if ((jump_dir == DOWN) || (svoVar_discKindT.Bit.DiscStd == eDiscBD))
#else
                if (jump_dir == DOWN)
#endif
                {
                    WRITE_REG(HAL_SRVREG_MAP->RGSEL,halSRV_RSEL_BSPEAK);
                    if (READ_REG(HAL_SRVREG_MAP->SRCR) > ((svoCalVar.bsOfsData.all16 >> 2) + (COEF_GetValue(AFE_CALC_BS_TARGET_10MV) * (2 * LJ_RETRYCNT - tries) * ((10 << DSPADC_BITS) / (2 * LJ_RETRYCNT)) / DSPADC_IN_RANGE)))
                    {//BS > (0.5 ~ 1) * BS_target
                        temp_timeout = ReadTimer(&temp_timer) + F_PP_DELAY_T;
                        if (temp_timeout > (F_SEARCH_T * 2)) temp_timeout = F_SEARCH_T * 2;
                        WRITE_FIELD(HAL_SRV_PKBOT,0);
                        WRITE_FIELD(HAL_SRV_PKBOT,1);
                    }
                }
            }

            if (READ_FIELD(HAL_SRV_FSRCH))
            {
                SendMsg80(SHOW_DEBUG_MSG,0x320A20); //F Catch Timeout
                ramp_stop();
                ramp_FOOS(foo_os);
            }
            else
            {
                fok = focus_on_chk(&te_signal.p2p_mv);
            }
#if (MSG_OUT_MODE != MSG_DISABLE)
            if (!fok)
            {
                SendMsgCn(SHOW_DEBUG_MSG,1,0x334008,tries); //WARNING: Layer jump (ramp) NG
            }
#endif
        }
        if (fok == OK)
        {
            ret = Ready;
            Defect_Enable(DEFECT_STATE_ONLY_FOCUS_ON);
#if (BD_ENABLE == 1) && (COLLI_SWITCH == 1) && (LJ_COLLI_2_JUMPS == 0)
            if (svoVar_discKindT.Bit.DiscStd == eDiscBD)
            {
                SendMsgCn(SHOW_DEBUG_MSG,5,0x33400A,A2B(te_signal.p2p_mv),A2B(svoCalDlVar[toLayer].dpp_thr),toLayer); //BD wrong layer chk: TEpp=%4d, thr=%4d
                te_signal.errcode = eNOERR;
                if ((te_signal.p2p_mv < te_thr2) && (te_signal.p2p_mv >= svoCalDlVar[toLayer].dpp_thr))
                {
                    te_signal = ReadFilteredDSPADCmV(ONE_REVOLUTION, TE_ALL, ADC_TE, TRUE);
                }
                if ((te_signal.errcode != eNOERR) || (te_signal.p2p_mv < svoCalDlVar[toLayer].dpp_thr))
                {
                    SendMsgCn(SHOW_DEBUG_MSG,1,0x334015,tries); //WARNING: Layer jump back
                    ret = Error;
                    apply_TILT(TILT_INIT_DAC);
#if FocusLockAid_Enable == 1
                    HAL_DSP_FocusLockAid(OFF, UP);
#endif // FocusLockAid_Enable == 1
                    if (tries == 1)
                    {//if there is no retry left, change the settings back to current layer
                        svoStp_JumpToPosition(COLLI_NORMAL_MODE, calGetOptColliPos(!toLayer));
                        while (svoStp_busy())
                        {
                            ExitProcess();
                        }
                        temp_timer = te_signal.p2p_mv;
                        te_signal = ReadFilteredDSPADCmV(ONE_REVOLUTION, TE_ALL, ADC_TE, TRUE);
                        if ((te_signal.errcode != eNOERR) || (te_signal.p2p_mv < temp_timer))
                        {
                            ret = Ready;
                        }
                        else
                        {
                            Layer_Index = toLayer;//make sure calUpdateLayer() can found Layer_Index != input.
                            calUpdateLayer(!toLayer);
                        }
                    }
                }
            }
            if (ret == Ready)
#endif
            {
                calUpdateLayer(toLayer);    //set every layer dependent parameters to target layer and prepare tracking on.
            }
        }
        else
        {   //focus NOK
#if FocusLockAid_Enable == 1
            HAL_DSP_FocusLockAid(OFF, UP);
#endif // FocusLockAid_Enable == 1

#if (SRC_ENABLE == 1) && (SRC_ENABLE_FOCUS == 1)
            if (ramping)
            {
                HAL_DSP_SRCEnable(SRC_LOOP_FOCUS, SRC_STATE_RESET);
            }
#endif
            ramping = TRUE;

            if (tries == 1)
            {
                calUpdateLayer(toLayer);
            }
            else
            {
#if (NEW_FDP_DETECTION == 1)
                foo_os = svoCalMisc2Var.fooos - F_SEARCH_T * FOCUS_SEARCH_KF18 / 0x20 * FOC_RAMP_FACTOR / 0x20000;
                if (foo_os < -FOO_OFFSET_LIMIT) foo_os = -FOO_OFFSET_LIMIT;
                temp_timer = (foo_os - FDP_FOO_OS) * 0x200000 / (FOCUS_SEARCH_KF18 * FOC_RAMP_FACTOR);
                if (temp_timer < FDP_STABLE_TIME)
                {
                    WaitTimer(FDP_STABLE_TIME - temp_timer);
                }
                ramp_FOOS(foo_os);
#else
                DelaymS(FDP_STABLE_TIME);
#endif
            }
        }
        if (ret == Ready)
        {   // jump ok! no retry needed
            break;
        }
        if (tries == 2)
        {
            ramping = TRUE;    //use ramping mode for last retry
        }
    }
    if (fok == OK)
    {
#if (SRC_ENABLE == 1) && (SRC_ENABLE_FOCUS == 1)
        HAL_DSP_SRCHold(SRC_LOOP_FOCUS, ON, ON, OFF);
#endif
#if (NEW_FDP_DETECTION != 0)
        CLEAR_INT_FIELD(HAL_SRV_FNOK,0);
        WRITE_FIELD(HAL_SRV_FNOKEN,1);    /* Enable Focus drop interrupt */
    #if (NEW_FDP_DETECTION == 2)
        WRITE_FIELD(HAL_SRV_DSPI6,0);
        WRITE_FIELD(HAL_SRV_DSPI6EN,1);
    #endif
#endif
#if (HOLD_LENS_BEFORE_TRKON == 1)
        seekSetCEEq(eRoughSeek, (svoVar_discKindT.Bit.DiscStd == eDiscCD)? CE_GAIN_DOWN_RATIO_CD : CE_GAIN_DOWN_RATIO_DVD);
        HAL_SERVO_CenterControlON(FORW);  // Turn Center-servo on
#endif
#if (TRACKOFF_SEEK_USE_BS_DEFECT == 0)
        Defect_Enable(DEFECT_STATE_ONLY_FOCUS_ON|DEFECT_SOURCE_DFE_RF);
#endif
    }
#if (BD_ENABLE == 1) && (COLLI_SWITCH == 1) && (LJ_COLLI_2_JUMPS == 0)
    if (svoVar_discKindT.Bit.DiscStd == eDiscBD)
    {
#if (ENABLE_BDIH == 1)
        if(bd_hybrid==1)
        {
            if(Layer_Index==0)
            {
                svoVar_discKindT.Bit.Media   = eMediaRW;
                svoVar_discKindT.Bit.WblType = eWblUnknown;
                LAL_StoreParams(LAL_DetectDiscKind(svoVar_discKindT), svoIramVar.svoCurrentSpeed);
                LAL_SetReadMode();
    #if (EN_POWER_CONTROL == 1)
                PowerControl(eBDReadWblPower);
    #endif
            }
            else
            {
                svoVar_discKindT.Bit.Media   = eMediaROM;
                svoVar_discKindT.Bit.WblType = eWblUnknown;
                LAL_StoreParams(LAL_DetectDiscKind(svoVar_discKindT), svoIramVar.svoCurrentSpeed);
                LAL_SetReadMode();
    #if (EN_POWER_CONTROL == 1)
                PowerControl(eBDReadWblPower);
    #endif
            }
        }
#endif
        afeSetTrackingMethod(getTrackingMethod(svoVar_discKindT));
        HAL_AFE_SetTELpfBW(orig_TELpfBW);
    }
#endif

    if (ret == Error)
    {
#if (PCB == 710)||(PCB == 711)||(PCB == 817)
#if (BD_ENABLE == 1) && (COLLI_SWITCH == 1)
        if (svoVar_discKindT.Bit.DiscStd == eDiscBD)
        {
            svoColli_Homing();
            while (svoStp_busy())
            {
                ExitProcess();
            }
            svoStp_JumpToPosition(COLLI_MICRO_NO_REST_MODE, calGetOptColliPos(Layer_Index));
            while (svoStp_busy())
            {
                ExitProcess();
            }
        }
#endif
#endif
        svoMiscVar.LayerJumpErrorFlag = 1;
    }

    return (ret);
}// end of svo_Layer_Jump
#else
BYTE svo_Layer_Jump(BYTE toLayer)
{
    BYTE   orig_FELPF;
    BYTE   tries,ret,ramping,f1_level_cur,fok;
    BYTE   jump_dir;
    SBYTE  delta_fe_gn;
    LONG   foo_os;
    ULONG  temp_timer;
    BYTE   temp_timeout;
#if (CHIP_REV >= 0xC0)
    SHORT  temp_FNGT;
#endif
#if (LJ_COLLI_2_JUMPS == 1)
    BYTE   f1_level_tar;
#endif
#if (LAYER_JUMP_SURF == 0)
    LONG   sample_num;
#endif
    ad_dsp_t te_signal;
#if (BD_ENABLE == 1)
    BYTE   orig_TELpfBW;
    #if (COLLI_SWITCH == 1) && (LJ_COLLI_2_JUMPS == 0)
    SHORT  te_thr2,pre_srfo;
    #endif
#endif

    if ((svoVar_discKindT.Bit.DiscStd == eDiscCD) || (svoVar_discKindT.Bit.Layers == eLayersSingle) || (Layer_Index == toLayer))
        return(Ready);

    HAL_DSP_SetDebug(DEBUG_CH0, DEBUG_SOURCE_RGLUE, 0x26, 8);//FCS

    // preset layer jump related settings (some of them should be moved to startup)
    // disable Early Zero Crossing; set Zero Crossing scale to 1/32; disable protection for Kick and Brake; Set Max kick time 900us; Set Max Brake time 500us.
    // //TBD:BDDL! Adapt kick/brake values for BD/HDDVD layer and make use of them
    READ_MOD_WRITE(HAL_SRVREG_MAP->EARZCPROT,HAL_SRV_ZCEAREN|HAL_SRV_ZCSEL|HAL_SRV_PROTECT|HAL_SRV_KCKBRKSEL,HAL_SRV_ZCSEL|HAL_SRV_KCKBRKSEL);
#if (LJ_COLLI_2_JUMPS == 1)
    f1_level_tar = LJ_FSL;
#endif

#if (SRC_ENABLE == 1)
    #if (SRC_ENABLE_FOCUS == 1)
    HAL_DSP_SRCHold(SRC_LOOP_FOCUS, ON, ON, ON);
    #endif
    HAL_DSP_SRCHold(SRC_LOOP_TRACKING, ON, ON, ON);
#endif

    HAL_DSP_SetDebug(DEBUG_CH1, DEBUG_SOURCE_RFILE, 0x24, 8);//set debug CH1 as FE_13

#if (BD_ENABLE == 1)
    if (svoVar_discKindT.Bit.DiscStd == eDiscBD)
    {
    #if (COLLI_SWITCH == 1)
        Defect_Enable(DEFECT_STATE_ONLY_FOCUS_ON|DEFECT_SOURCE_SERVO_BS);
        WRITE_REG(HAL_SRVREG_MAP->PUSTP, 0x00);             /* tracking open loop */
        #if (LJ_COLLI_2_JUMPS == 1)
        svoStp_JumpToPosition(COLLI_NORMAL_MODE, (calGetOptColliPos(0) + calGetOptColliPos(1)) / 2);
        temp_timer = svoCalMisc1Var[!toLayer].feGainDSP * (svoCalDlVar[!toLayer].fe_factor + 3072) / 4096;
        temp_timer = (temp_timer > 0x7FFF)? 0x7FFF : temp_timer;
        WRITE_DSP_COEF(kfg, temp_timer);
        #else
        svoStp_JumpToPosition(COLLI_NORMAL_MODE, calGetOptColliPos(toLayer));
        temp_timer = svoCalMisc1Var[!toLayer].feGainDSP * (svoCalDlVar[!toLayer].fe_factor + 1024) / 2048;
        temp_timer = (temp_timer > 0x7FFF)? 0x7FFF : temp_timer;
        WRITE_DSP_COEF(kfg, temp_timer);
        #endif

        apply_FEBC(0);
        apply_TILT(TILT_INIT_DAC);
        while (svoStp_busy())
        {
            ExitProcess();
        }

        #if (LJ_COLLI_2_JUMPS == 1)
        temp_timer = svoCalMisc1Var[!toLayer].feGainDSP * (svoCalDlVar[!toLayer].fe_factor + 1024) / 2048;
        temp_timer = (temp_timer > 0x7FFF)? 0x7FFF : temp_timer;
        WRITE_DSP_COEF(kfg, temp_timer);
        f1_level_cur = LJ_FSL * 2048 / (svoCalDlVar[!toLayer].fe_factor + 1024);
        f1_level_tar = LJ_FSL * 2048 / (svoCalDlVar[toLayer].fe_factor + 1024);
        #else // #if (LJ_COLLI_2_JUMPS == 1)
        temp_timer = svoCalMisc1Var[!toLayer].feGainDSP * svoCalDlVar[!toLayer].fe_factor / 1024;
        temp_timer = (temp_timer > 0x7FFF)? 0x7FFF : temp_timer;
        WRITE_DSP_COEF(kfg, temp_timer);
        f1_level_cur = LJ_FSL * 1024 / svoCalDlVar[!toLayer].fe_factor;
        WRITE_FIELD(HAL_AFE_TEBG,       svoCalGnT3Var[toLayer][TEB_GN].normal_gain);
        WRITE_FIELD(HAL_AFE_TRKG,       svoCalGnT1Var[toLayer][TE_GN].groove_gain[RECDH]);
        WRITE_FIELD(HAL_AFE_TRKO,       svoCalOfsT1Var[toLayer][TE_OS].groove_offset[RECDH]);
        WRITE_FIELD(HAL_AFE_BLANKTRKG,  svoCalGnT1Var[toLayer][TE_GN].groove_gain[RECDL]);
        WRITE_FIELD(HAL_AFE_BLANKTRKO,  svoCalOfsT1Var[toLayer][TE_OS].groove_offset[RECDL]);
        te_thr2 = COEF_GetValue(AFE_CALC_TE_PP_TARGET_10MV);
        svoCalDlVar[toLayer].dpp_thr = te_thr2;

        WRITE_FIELD(HAL_AFE_MUX1ADSEL, halAFE_MUX1ADSEL_SRFO);
        if (svoVar_TrackingMethod != TRK_METHOD_DPP)
        {
            te_thr2 /= 3;
            afeSetTrackingMethod(TRK_METHOD_DPP);
        }
        else
        {
            te_thr2 *= 2;
        }

        orig_TELpfBW = HAL_AFE_GetTELpfBW();
        HAL_AFE_SetTELpfBW(e_TELPF_660);
        te_signal = ReadFilteredDSPADCmV(ONE_REVOLUTION, TE_ALL, ADC_MUXOUT1, TRUE);
        if (te_signal.errcode == eNOERR)
        {
            svoCalDlVar[toLayer].dpp_thr += te_signal.p2p_mv;
            pre_srfo = te_signal.top_mv - vref_fm_mux3_mV;
        }
        te_thr2 += svoCalDlVar[toLayer].dpp_thr;
        #endif // #if (LJ_COLLI_2_JUMPS == 1)
    #else // #if (COLLI_SWITCH == 1)
        f1_level_cur = LJ_FSL;
    #endif // #if (COLLI_SWITCH == 1)
        jump_dir = (toLayer == 0)? UP : DOWN;
    }
    else
#endif // #if (BD_ENABLE == 1)
    {
        jump_dir = (toLayer == 1)? UP : DOWN;
        f1_level_cur = LJ_FSL;
    }

    SetDfeWblFrontOnHold();

    if (jump_dir == UP)
    {
        SendMsg80(DEBUG_SEEK_MSG,0x450609);     // Layer Jump UP
    }
    else
    {
        SendMsg80(DEBUG_SEEK_MSG,0x45060A);     // Layer Jump Down
    }

    WRITE_FIELD(HAL_AFE_TOKLVL, TOKLVL_WAIT_TRACK_ON);

    // Calculate new f1 level according to FE gain difference between layer 0 and layer 1
    delta_fe_gn = svoCalGnT1Var[toLayer][FE_GN].groove_gain[RECDH] - svoCalGnT1Var[!toLayer][FE_GN].groove_gain[RECDH];

    WRITE_FIELD(HAL_SRV_SBYLVL, LJ_SB_LEVEL_HYSTERESIS);
    if (f1_level_cur < LJ_SB_LEVEL_HYSTERESIS + 1)
    {
        f1_level_cur = LJ_SB_LEVEL_HYSTERESIS + 1;
    }

#if (NEW_FDP_DETECTION != 0)
    WRITE_FIELD(HAL_SRV_FNOKEN,0);      //turn off Focus drop interrupt
    #if (NEW_FDP_DETECTION == 2)
    WRITE_FIELD(HAL_SRV_DSPI6EN,0);     //turn off New Focus drop interrupt
    #endif
#endif

    fok = NG;
    ramping = FALSE;
    ret = Error;
    for (tries = LJ_RETRYCNT; tries > 0; tries--)
    {
        if (!ramping)//use layer jump (faster)
        {
            apply_FEBC(0);
            if (FokStatusOK())
            {
                orig_FELPF = READ_REG(HAL_AFEREG_MAP->FCTS32) & (HAL_AFE_FEFCSFSEL_MASK|HAL_AFE_FELPSET_MASK|HAL_AFE_BPFELP_MASK);
                READ_MOD_WRITE(HAL_AFEREG_MAP->FCTS32, (HAL_AFE_FEFCSFSEL_MASK|HAL_AFE_FELPSET_MASK|HAL_AFE_BPFELP_MASK), halAFE_FELPF_15kHz << HAL_AFE_FEFCSFSEL_LSB);

#if (LAYER_JUMP_SURF == 0)
                //Modify FOO offset before jump layer ***
                InitLPFIIR(0,1000);
                sample_num = 0;
                foo_os = 0;
                StartTimer(&temp_timer);
                do
                {
                    foo_os += LPFIIR(READ_DSP_COEF(debug1));//FE_13, FELFG output
                    sample_num++;
                }
                while (ReadTimer(&temp_timer) < MS_5);

                foo_os = (foo_os / sample_num * READ_DSP_COEF(src_kf2) / 0x8000 * READ_DSP_COEF(kfg) / 0x8000) + HAL_SERVO_GetFoos();
#endif

                Defect_Enable(DEFECT_STATE_ALL_OFF|DEFECT_SOURCE_SERVO_BS); //disable defect and change source to BS
                //apply focus control related parameters for target layer
                //we can't predict the RECD of target layer, always use RECDH
    #if (BD_ENABLE == 1) && (COLLI_SWITCH == 1)
                if (svoVar_discKindT.Bit.DiscStd == eDiscBD)
                {
                    WRITE_FIELD(HAL_AFE_BLANKFCSG,svoCalGnT1Var[!toLayer][FE_GN].groove_gain[RECDL]+3);
                    WRITE_FIELD(HAL_AFE_FCSG,     svoCalGnT1Var[!toLayer][FE_GN].groove_gain[RECDH]+3);
                }
    #endif
                WRITE_DSP_COEF(kfg, (svoCalMisc1Var[0].feGainDSP + svoCalMisc1Var[1].feGainDSP) / 2);
#if (LAYER_JUMP_SURF == 1)
                HAL_DSP_LayerJumpSurf(ON);
#endif
                WRITE_REG(HAL_SRVREG_MAP->PUSTP, 0x00);             /* tracking open loop */
#if (AFE_NORMALIZER == 1)
                afeSvognDisable();
#endif
                WRITE_FIELD(HAL_SRV_NOFOKB, 1); //Enable the waiver of the FOK = 0 requirement for focus kick.

#if (CHIP_REV >= 0xC0)
                temp_FNGT = READ_FIELD(HAL_SRV_FNGT);
                WRITE_FIELD(HAL_SRV_FNGT,0x160);  //about 2ms
#endif

                //Set layer jump command
                if (jump_dir == UP)
                {
                    HAL_SERVO_SetFSL(0x7F, f1_level_cur);
                    HAL_SERVO_SetFocUp(HAL_SRV_FSONSET);
                }
                else
                {
                    HAL_SERVO_SetFSL(f1_level_cur, 0x7F);
                    HAL_SERVO_SetFocDn(HAL_SRV_FSONRST);
                }

#if (BD_ENABLE == 1)
                if (svoVar_discKindT.Bit.DiscStd == eDiscBD)
                {
    #if (LAYER_JUMP_SURF == 0)
                    HAL_SERVO_SetFoos(foo_os + ((jump_dir == UP)? ((LJ_FCS_ADD_BD * FOCUS_SEARCH_KF18 + 0x4000) / 0x8000) : -((LJ_FCS_ADD_BD * FOCUS_SEARCH_KF18 + 0x4000) / 0x8000)));
    #endif
                    Delay100uS(1);
                    StartTimer(&temp_timer);
                    while ((READ_DSP_COEF(debug0) != 0) && (ReadTimer(&temp_timer) < MS_1));
                    Delay100uS(LJ_FCS_DELAY_BD_100uS);

                    WRITE_DSP_COEF(kb10, 0x7FFF);
                    WRITE_DSP_COEF(kb11, 0x7FFF - READ_DSP_COEF(kb12));
                }
                else
#endif
                {
#if (LAYER_JUMP_SURF == 0)
                    HAL_SERVO_SetFoos(foo_os + ((jump_dir == UP)? ((LJ_FCS_ADD * FOCUS_SEARCH_KF18 + 0x4000) / 0x8000) : -((LJ_FCS_ADD * FOCUS_SEARCH_KF18 + 0x4000) / 0x8000)));
#endif
                    Delay100uS(1);
                    StartTimer(&temp_timer);
                    while ((READ_DSP_COEF(debug0) != 0) && (ReadTimer(&temp_timer) < MS_1));
                    if (svoVar_discKindT.fDiscKind == eDVDROMDLDisc)
                    {
                        Delay1uS(LJ_FCS_DELAY_DVDROM_1uS);
                    }
                    else
                    {
                        Delay100uS(LJ_FCS_DELAY_100uS);
                    }
                }

                GAIN_RECD_HIGH;//restore in focus_on_chk()
                WRITE_FIELD(HAL_AFE_BLANKFCSG,svoCalGnT1Var[toLayer][FE_GN].groove_gain[RECDL]);
                WRITE_FIELD(HAL_AFE_FCSG,     svoCalGnT1Var[toLayer][FE_GN].groove_gain[RECDH]);
                WRITE_FIELD(HAL_AFE_WRFCSG,   svoCalGnT1Var[toLayer][FE_GN].write_gain);
                WRITE_FIELD(HAL_AFE_BSKG,     svoCalGnT2Var[toLayer][BS_GN].normal_gain);
                WRITE_FIELD(HAL_AFE_BLANKBSKG,svoCalGnT2Var[toLayer][BS_GN].write_gain);

                //Set FSL threshold for layer jump brake
                if (jump_dir == UP)
                {
#if (LJ_COLLI_2_JUMPS == 1)
                    HAL_SERVO_SetFSL(f1_level_tar, f1_level_cur);
#else
                    HAL_SERVO_SetFSL(LJ_FSL, f1_level_cur);
#endif
                }
                else
                {
#if (LJ_COLLI_2_JUMPS == 1)
                    HAL_SERVO_SetFSL(f1_level_cur, f1_level_tar);
#else
                    HAL_SERVO_SetFSL(f1_level_cur, LJ_FSL);
#endif
                }

#if (FocusLockAid_Enable == 1)
                HAL_DSP_FocusLockAid(ON, jump_dir);//This must be excuted after kick pulse, if not, adjust kick level or the delay time.
#endif

                //Wait for layer jump done
                StartTimer(&temp_timer);
                while (READ_FIELD(HAL_SRV_FSRCH) && (!READ_FIELD(HAL_SRV_PROTACT)) && (ReadTimer(&temp_timer) < ((svoVar_discKindT.Bit.DiscStd == eDiscDVD)? LJ_WAIT_FSRCH_T_DVD : LJ_WAIT_FSRCH_T)));

#if (FocusLockAid_Enable == 1)
                HAL_DSP_FocusLockAid(OFF, UP);
#endif // FocusLockAid_Enable == 1
                HAL_SERVO_SetFSL(0x7F, 0x7F);

                WRITE_FIELD(HAL_SRV_NOFOKB, 0); //Disable the waiver of the FOK = 0 requirement for focus kick
#if (CHIP_REV >= 0xC0)
                WRITE_FIELD(HAL_SRV_FNGT,temp_FNGT);
#endif

                READ_MOD_WRITE(HAL_AFEREG_MAP->FCTS32, (HAL_AFE_FEFCSFSEL_MASK|HAL_AFE_FELPSET_MASK|HAL_AFE_BPFELP_MASK), orig_FELPF);

#if (BD_ENABLE == 1)
                if (svoVar_discKindT.Bit.DiscStd == eDiscBD)
                {
    #if (COLLI_SWITCH == 1) && (LJ_COLLI_2_JUMPS == 1)
                    svoStp_JumpToPosition(COLLI_NORMAL_MODE, calGetOptColliPos(toLayer));
                    temp_timer = svoCalMisc1Var[toLayer].feGainDSP * (svoCalDlVar[toLayer].fe_factor + 3072) / 4096;
                    temp_timer = (temp_timer > 0x7FFF)? 0x7FFF : temp_timer;
                    WRITE_DSP_COEF(kfg, temp_timer);
                    while (svoStp_busy())
                    {
                        ExitProcess();
                    }
                    WRITE_DSP_COEF(kfg, svoCalMisc1Var[toLayer].feGainDSP);
    #endif
                    svo_Block_Eq_Data_Set(svoIramVar.svoCurrentSpeed, BEAM_LOW_EQ);
                }
#endif

#if (LAYER_JUMP_SURF == 1)
                HAL_DSP_LayerJumpSurf(OFF);
#endif

                //Check layer jump status
                if (READ_FIELD(HAL_SRV_PROTACT))
                {
#if (NEW_FDP_DETECTION != 0)
                    svoFDropIsr();
#endif
                    SendMsg80(SHOW_DEBUG_MSG,0x334010);     // WARNING: Layer Jump kick or brake timeout
                }
                else
                {
                    fok = focus_on_chk(&te_signal.p2p_mv);
                }
            }

            foo_os = svoCalMisc2Var.fooos - F_SEARCH_T * FOCUS_SEARCH_KF18 / 0x20 * FOC_RAMP_FACTOR / 0x20000;
            if (foo_os < -FOO_OFFSET_LIMIT) foo_os = -FOO_OFFSET_LIMIT;
            if (fok == NG)
            {
                SendMsgCn(SHOW_DEBUG_MSG,1,0x334007,tries); // WARNING: Layer jump (kick) NG (retry count = %1d)
            }
        }
        else //use ramping (slower)
        {
            foo_os = svoCalMisc2Var.fooos - F_SEARCH_T * FOCUS_SEARCH_KF18 / 0x20 * FOC_RAMP_FACTOR / 0x20000;
            if (foo_os < -FOO_OFFSET_LIMIT) foo_os = -FOO_OFFSET_LIMIT;

            if (FokStatusOK())
            {
                //hold at bottom
                WRITE_REG(HAL_SRVREG_MAP->LDOFF, 0x00);
                WRITE_REG(HAL_SRVREG_MAP->LDON, 0x00);
#if (CHIP_REV >= 0xC0)
                HAL_SERVO_SetFcs((foo_os - HAL_SERVO_GetFoos()) * 0x8000 / FOCUS_SEARCH_KF18);
                WRITE_FIELD(HAL_SRV_FCSSBYEN,0);
                WRITE_FIELD(HAL_SRV_FCSSBYEN,1);
                HAL_SERVO_SetFocDn(0);
                HAL_SERVO_SetFocStp();
#else
                HAL_SERVO_SetFoos(foo_os);
#endif
                //wait 20ms for lens to stable
                HAL_DSP_SRCEnable(SRC_LOOP_FOCUS, SRC_STATE_RESET); //Reset focus SRC to prevent wrong focus control output
                WaitTimer(20);
            }
#if (NEW_FDP_DETECTION != 0)
            svoIramVar.focusDropIsrFlag = FALSE;    // clear DSP focus drop flag
#endif

#if (AFE_NORMALIZER == 1)
            afeSvognDisable();
#endif
            //apply focus control related parameters for target layer
            //marked until DVDRAM DL take place
            //we can't predict the RECD of target layer, always use RECDH

            //forced RECD2 to high.
            GAIN_RECD_HIGH;
            WRITE_FIELD(HAL_AFE_BLANKFCSG,svoCalGnT1Var[toLayer][FE_GN].groove_gain[RECDL]);
            WRITE_FIELD(HAL_AFE_FCSG,     svoCalGnT1Var[toLayer][FE_GN].groove_gain[RECDH]);
            WRITE_FIELD(HAL_AFE_WRFCSG,   svoCalGnT1Var[toLayer][FE_GN].write_gain);
            WRITE_FIELD(HAL_AFE_BSKG,     svoCalGnT2Var[toLayer][BS_GN].normal_gain);
            WRITE_FIELD(HAL_AFE_BLANKBSKG,svoCalGnT2Var[toLayer][BS_GN].write_gain);
            WRITE_DSP_COEF(kfg,                svoCalMisc1Var[toLayer].feGainDSP);

#if (FOCUS_LOCK_FROM_DOWN_TO_UP==1)
            if ((jump_dir == UP) && (svoVar_discKindT.Bit.DiscStd != eDiscBD))
#else
            if (jump_dir == UP)
#endif
            {
                WRITE_FIELD(HAL_SRV_PKBOT,0);
                WRITE_FIELD(HAL_SRV_PKBOT,1);
                HAL_SERVO_SetFocUp(HAL_SRV_HISRSET);
                temp_timeout = F_SEARCH_T;
                StartTimer(&temp_timer);
                while (ReadTimer(&temp_timer) < temp_timeout)
                {
                    WRITE_REG(HAL_SRVREG_MAP->RGSEL,halSRV_RSEL_BSPEAK);
                    if (READ_REG(HAL_SRVREG_MAP->SRCR) > ((svoCalVar.bsOfsData.all16 >> 2) + (COEF_GetValue(AFE_CALC_BS_TARGET_10MV) * (2 * LJ_RETRYCNT - tries) * ((10 << DSPADC_BITS) / (2 * LJ_RETRYCNT)) / DSPADC_IN_RANGE)))
                    {//BS > (0.5 ~ 1) * BS_target
                        temp_timeout = ReadTimer(&temp_timer) + (F_PP_DELAY_T + 1) / 2;
                        if (temp_timeout > F_SEARCH_T) temp_timeout = F_SEARCH_T;
                        WRITE_FIELD(HAL_SRV_PKBOT,0);
                        WRITE_FIELD(HAL_SRV_PKBOT,1);
                    }
                }
            }

            HAL_SERVO_SetFebc(0);
#if (FocusLockAid_Enable == 1)
    #if (FOCUS_LOCK_FROM_DOWN_TO_UP == 1)
            HAL_DSP_FocusLockAid(ON, ((jump_dir == UP) && (svoVar_discKindT.Bit.DiscStd != eDiscBD))? DOWN : UP);
    #else
            HAL_DSP_FocusLockAid(ON, (jump_dir == UP)? DOWN : UP);
    #endif
#endif
#if (FOCUS_LOCK_FROM_DOWN_TO_UP == 1)
            if ((jump_dir == UP) && (svoVar_discKindT.Bit.DiscStd != eDiscBD))
#else
            if (jump_dir == UP)
#endif
            {
                if (svoVar_discKindT.Bit.DiscStd == eDiscBD)
                    HAL_SERVO_SetFSL(0x7F, FOCUS_CAPTURE_FSL_BD);
                else
                    HAL_SERVO_SetFSL(0x7F, FOCUS_CAPTURE_FSL_CD_DVD);
                HAL_SERVO_SetFocDn(HAL_SRV_FSONSET|HAL_SRV_FGCSET);
            }
            else
            {
                if (svoVar_discKindT.Bit.DiscStd == eDiscBD)
                    HAL_SERVO_SetFSL(FOCUS_CAPTURE_FSL_BD, 0x7F);
                else
                    HAL_SERVO_SetFSL(FOCUS_CAPTURE_FSL_CD_DVD, 0x7F);

                HAL_SERVO_SetFocUp(HAL_SRV_FSONSET|HAL_SRV_FGCSET);
            }
            WRITE_FIELD(HAL_SRV_PKBOT,0);
            WRITE_FIELD(HAL_SRV_PKBOT,1);
            temp_timeout = F_SEARCH_T * 2;
            StartTimer(&temp_timer);
            while (READ_FIELD(HAL_SRV_FSRCH) && (ReadTimer(&temp_timer) < temp_timeout))
            {
#if(FOCUS_LOCK_FROM_DOWN_TO_UP==1)
                if ((jump_dir == DOWN) || (svoVar_discKindT.Bit.DiscStd == eDiscBD))
#else
                if (jump_dir == DOWN)
#endif
                {
                    WRITE_REG(HAL_SRVREG_MAP->RGSEL,halSRV_RSEL_BSPEAK);
                    if (READ_REG(HAL_SRVREG_MAP->SRCR) > ((svoCalVar.bsOfsData.all16 >> 2) + (COEF_GetValue(AFE_CALC_BS_TARGET_10MV) * (2 * LJ_RETRYCNT - tries) * ((10 << DSPADC_BITS) / (2 * LJ_RETRYCNT)) / DSPADC_IN_RANGE)))
                    {//BS > (0.5 ~ 1) * BS_target
                        temp_timeout = ReadTimer(&temp_timer) + F_PP_DELAY_T;
                        if (temp_timeout > (F_SEARCH_T * 2)) temp_timeout = F_SEARCH_T * 2;
                        WRITE_FIELD(HAL_SRV_PKBOT,0);
                        WRITE_FIELD(HAL_SRV_PKBOT,1);
                    }
                }
            }
#if (FocusLockAid_Enable == 1)
            HAL_DSP_FocusLockAid(OFF, UP);
#endif
            HAL_SERVO_SetFSL(0x7F, 0x7F);
            if (READ_FIELD(HAL_SRV_FSRCH))
            {
                SendMsg80(SHOW_DEBUG_MSG,0x320A20); // ERROR: Focus Catch Timeout
                ramp_stop();
#if (CHIP_REV >= 0xC0)
                ramp_to(foo_os);
#else
                ramp_FOOS(foo_os);
#endif
            }
            else
            {
                fok = focus_on_chk(&te_signal.p2p_mv);
            }

            if (fok == NG)
            {
                SendMsgCn(SHOW_DEBUG_MSG,1,0x334008,tries); // WARNING: Layer jump (ramp) NG (retry count = %1d)
            }
        }

        if (fok == OK)
        {
            ret = Ready;
            Defect_Enable(DEFECT_STATE_ONLY_FOCUS_ON);

#if (BD_ENABLE == 1) && (COLLI_SWITCH == 1) && (LJ_COLLI_2_JUMPS == 0)
            if (svoVar_discKindT.Bit.DiscStd == eDiscBD)
            {
                // BD wrong layer chk in layerjump: TEpp = %4d, thr = %4d, layer = %1d
                SendMsgCn(SHOW_DEBUG_MSG,5,0x33400A,A2B(te_signal.p2p_mv),A2B(svoCalDlVar[toLayer].dpp_thr),toLayer);

                te_signal.errcode = eNOERR;

                if (te_signal.p2p_mv < te_thr2)
                {
                    te_signal = ReadFilteredDSPADCmV(ONE_REVOLUTION, TE_ALL, ADC_MUXOUT1, TRUE);
                    if (ABS(te_signal.top_mv - vref_fm_mux3_mV - pre_srfo) * 8 > pre_srfo)
                    {
                        //BD wrong layer chk in layerjump: SRFO = %4d, pre_SRFO = %4d, layer = %1d
                        SendMsgCnB(SHOW_DEBUG_MSG,5,0x334035,B2B(te_signal.top_mv - vref_fm_mux3_mV),B2B(pre_srfo),B1B(toLayer));
                        te_signal.p2p_mv = svoCalDlVar[toLayer].dpp_thr;
                    }
                }

                if ((te_signal.errcode != eNOERR) || (te_signal.p2p_mv < svoCalDlVar[toLayer].dpp_thr))
                {
                    SendMsgCn(SHOW_DEBUG_MSG,1,0x334015,tries); // WARNING: Layer jump back (retry count = %1d)
                    ret = Error;
                    apply_TILT(TILT_INIT_DAC);

                    if (tries == 1)
                    {
                        //if there is no retry left, change the settings back to current layer
                        svoStp_JumpToPosition(COLLI_NORMAL_MODE, calGetOptColliPos(!toLayer));
                        while (svoStp_busy())
                        {
                            ExitProcess();
                        }

                        temp_timer = te_signal.p2p_mv;
                        te_signal = ReadFilteredDSPADCmV(ONE_REVOLUTION, TE_ALL, ADC_BS, TRUE);
                        if ((te_signal.errcode != eNOERR) || (te_signal.p2p_mv < temp_timer))
                        {
                            ret = Ready;
                        }
                        else
                        {
                            Layer_Index = toLayer;//make sure calUpdateLayer() can found Layer_Index != input.
                            calUpdateLayer(!toLayer);
                        }
                    }
                }
            }
            if (ret == Ready)
#endif // #if (BD_ENABLE == 1) && (COLLI_SWITCH == 1) && (LJ_COLLI_2_JUMPS == 0)
            {
                calUpdateLayer(toLayer);    //set every layer dependent parameters to target layer and prepare tracking on.
            }
            if ((svoCalVar.calGainDoneBits & dif_RF_end) == dif_RF_end)
            {
                Defect_Enable(DEFECT_STATE_ONLY_FOCUS_ON | DEFECT_SOURCE_DFE_RF); //enable defect and change source back to RF
            }

#if (RP_AUTO_ADJUST == 1)
            if ( (BDRDLDISC || BDREDLDISC) && ((RPOfsAdjust & RP_ADJ_L_MASK) != RP_ADJ_L_MASK))
            {
                RPOfsAdjust |= RP_ADJ_L_MASK;
                WaitTimer(5); // Delay 5ms
            }
#endif
        }
        else
        {
            // focus NOK
#if (SRC_ENABLE == 1) && (SRC_ENABLE_FOCUS == 1)
            if (ramping)
            {
                HAL_DSP_SRCEnable(SRC_LOOP_FOCUS, SRC_STATE_RESET);
            }
#endif
            ramping = TRUE;

            if (tries == 1)
            {
                calUpdateLayer(toLayer);
            }
            else
            {
                if (HAL_SERVO_GetFooDac() != foo_os)
                {
#if (NEW_FDP_DETECTION != 0)
    #if (CHIP_REV >= 0xC0)
                    HAL_SERVO_SetFcs((FDP_FOO_OS - HAL_SERVO_GetFoos()) * 0x8000 / FOCUS_SEARCH_KF18);
        #if (FDP_PROT_PULSE_TIME != 0)
                    Delay100uS(FDP_PROT_PULSE_TIME);
        #endif
                    WRITE_FIELD(HAL_SRV_FCSSBYEN,0);
                    WRITE_FIELD(HAL_SRV_FCSSBYEN,1);
                    HAL_SERVO_SetFocDn(0);
                    HAL_SERVO_SetFocStp();
    #endif
#else
                    HAL_SERVO_SetFoos(FDP_FOO_OS);
                    WRITE_REG(HAL_SRVREG_MAP->LDOFF, 0x00);
                    WRITE_REG(HAL_SRVREG_MAP->LDON, 0x00);
#endif
                    focus_wait_stable(FDP_STABLE_TIME);
                }
            }
        }

        if (ret == Ready)
        {
            // jump ok! no retry needed
            break;
        }

        if (tries == 2)
        {
            ramping = TRUE;
        }
    }

    if (fok == OK)
    {
#if (SRC_ENABLE == 1) && (SRC_ENABLE_FOCUS == 1)
        HAL_DSP_SRCHold(SRC_LOOP_FOCUS, ON, ON, OFF);
#endif
#if (NEW_FDP_DETECTION != 0)
        CLEAR_INT_FIELD(HAL_SRV_FNOK,0);
        WRITE_FIELD(HAL_SRV_FNOKEN,1);          /* Enable Focus drop interrupt */
    #if (NEW_FDP_DETECTION == 2)
        WRITE_FIELD(HAL_SRV_DSPI6,0);
        WRITE_FIELD(HAL_SRV_DSPI6EN,1);         /* Enable New Focus drop interrupt */
    #endif
#endif
    }

#if (BD_ENABLE == 1) && (COLLI_SWITCH == 1) && (LJ_COLLI_2_JUMPS == 0)
    if (svoVar_discKindT.Bit.DiscStd == eDiscBD)
    {
        HAL_AFE_SetTELpfBW(orig_TELpfBW);
        afeSetTrackingMethod(getTrackingMethod(svoVar_discKindT));
    }
#endif

    if (ret == Error)
    {
        svoMiscVar.LayerJumpErrorFlag = 1;
    }

    return (ret);
}// end of svo_Layer_Jump
#endif

/*-----------------------------------------------------------------------------
 *
 * Module: seekSetCEEq()
 * Description: use CE coeffs for seeking
 *
 *----------------------------------------------------------------------------*/
// TE LFG = double pole HPF (fc = 30 Hz)
#define Damperktc1c 0x8000
#define Damperktc1d 0x7FEE
#define Damperktc1e 0x8000
#define Damperktc1f 0x7FEE

#if (FINE_SEEK_SURF_TELFG == 0)
svoWordU fineCELFG[] =
{
    0,
    0,
    0,
    0
};
#endif // (FINE_SEEK_SURF_TELFG == 0)

void seekSetCEEq(eTrackEQType type, BYTE gain_ratio)
{
    const svoWordU *ptrCEEQ;
    const svoWordU *ptrCELFG;
    ESpindleSpeed  speed;
    USHORT TempSeqPC;        //for damper coefficient load check

    speed = svoIramVar.svoCurrentSpeed;
    if (speed == 0)
    {
        return;
    }
    speed--;
#if (FINE_SEEK_SURF_TELFG == 0)
    if (type == eFineSeek)
    {
        if (svoVar_discKindT.Bit.DiscStd == eDiscDVD)
        {
    #if (DVD_RAM_READ== 1)
            if (svoVar_discKindT.Bit.Media == eMediaRAM)
            {
                ptrCEEQ = dvRamFineCEEQCoefTbl[speed];
                fineCELFG[0] = dvRamSvoEQCoefTbl[speed + 1][TRACKING_LOW_EQ][0];
                fineCELFG[1] = dvRamSvoEQCoefTbl[speed + 1][TRACKING_LOW_EQ][1];
            }
            else
    #endif
            if (svoVar_discKindT.Bit.Media == eMediaROM)
            {
                ptrCEEQ = dvFineCEEQCoefTbl[speed];
                fineCELFG[0] = dvSvoEQCoefTbl[speed + 1][TRACKING_LOW_EQ][0];
                fineCELFG[1] = dvSvoEQCoefTbl[speed + 1][TRACKING_LOW_EQ][1];
            }
            else
            {
                ptrCEEQ = rdvFineCEEQCoefTbl[speed];
                fineCELFG[0] = rdvSvoEQCoefTbl[speed + 1][TRACKING_LOW_EQ][0];
                fineCELFG[1] = rdvSvoEQCoefTbl[speed + 1][TRACKING_LOW_EQ][1];
            }
        }
        else if (svoVar_discKindT.Bit.DiscStd == eDiscCD)//CD
        {
            ptrCEEQ = FineCEEQCoefTbl[speed];
            fineCELFG[0] = svoEQCoefTbl[speed + 1][TRACKING_LOW_EQ][0];
            fineCELFG[1] = svoEQCoefTbl[speed + 1][TRACKING_LOW_EQ][1];
        }
    #if (BD_ENABLE == 1)
        else if (svoVar_discKindT.Bit.DiscStd == eDiscBD)//BD
        {
            if (svoVar_discKindT.Bit.Media == eMediaROM)
            {
                ptrCEEQ = bdFineCEEQCoefTbl[speed];
                fineCELFG[0] = bdSvoEQCoefTbl[speed + 1][TRACKING_LOW_EQ][0];
                fineCELFG[1] = bdSvoEQCoefTbl[speed + 1][TRACKING_LOW_EQ][1];
            }
            else
            {
                ptrCEEQ = bdrFineCEEQCoefTbl[speed];
                fineCELFG[0] = bdrSvoEQCoefTbl[speed + 1][TRACKING_LOW_EQ][0];
                fineCELFG[1] = bdrSvoEQCoefTbl[speed + 1][TRACKING_LOW_EQ][1];
            }
        }
    #endif
    #if (ENABLE_HDDVD == 1)
        else if(svoVar_discKindT.Bit.DiscStd == eDiscHD)
        {
            if (svoVar_discKindT.Bit.Media == eMediaROM)
            {
                ptrCEEQ = hdFineCEEQCoefTbl[speed];
                fineCELFG[0] = hdSvoEQCoefTbl[speed + 1][TRACKING_LOW_EQ][0];
                fineCELFG[1] = hdSvoEQCoefTbl[speed + 1][TRACKING_LOW_EQ][1];
            }
            else
            {
                ptrCEEQ = hdrFineCEEQCoefTbl[speed];
                fineCELFG[0] = hdrSvoEQCoefTbl[speed + 1][TRACKING_LOW_EQ][0];
                fineCELFG[1] = hdrSvoEQCoefTbl[speed + 1][TRACKING_LOW_EQ][1];
            }
        }
    #endif
        ptrCELFG = fineCELFG;
        WRITE_FIELD(HAL_SRV_PGTK,TRUE);
    }
    else
#endif // (FINE_SEEK_SURF_TELFG == 0)
    {
        if (svoVar_discKindT.Bit.DiscStd == eDiscDVD)
        {
#if (DVD_RAM_READ == 1)
            if(svoVar_discKindT.Bit.Media == eMediaRAM)
            {
                ptrCEEQ  = dvRamRoughCEEQ;
                ptrCELFG = dvRamRoughCELFG;
            }
            else
#endif
            {
                ptrCEEQ = dvRoughCEEQ;
                ptrCELFG = dvRoughCELFG;
            }
        }
        else if(svoVar_discKindT.Bit.DiscStd == eDiscCD)
        {
            ptrCEEQ = RoughCEEQ;
            ptrCELFG = RoughCELFG;
        }
#if (BD_ENABLE == 1)
        else if (svoVar_discKindT.Bit.DiscStd == eDiscBD)//BD
        {
            ptrCEEQ = bdRoughCEEQ;
            ptrCELFG = bdRoughCELFG;
        }
#endif
#if (ENABLE_HDDVD == 1)
        else if(svoVar_discKindT.Bit.DiscStd == eDiscHD)
        {
            ptrCEEQ = hdRoughCEEQ;
            ptrCELFG = hdRoughCELFG;
        }
#endif
        if (type == eRoughSeekRatioOnly)
        {
            WRITE_DSP_COEF(Ce_Eq[8], (USHORT)(((LONG)(ptrCEEQ+8)->all16s) * gain_ratio / 128));
            WRITE_DSP_COEF(Ce_Eq[9], (USHORT)(((LONG)(ptrCEEQ+9)->all16s) * gain_ratio / 128));
            return;
        }

        WRITE_FIELD(HAL_SRV_PGTK, FALSE);
    }
    SetSvoEqBank(Ce_Eq, ptrCEEQ, gain_ratio);
    if (type == eDamper)
    {
        TempSeqPC = 0;
        while( (TempSeqPC < 24) || (TempSeqPC > 50) )
        {
            TempSeqPC = (USHORT)READ_REG(HAL_SRVREG_MAP->SEQ_PC);
        }
        WRITE_DSP_COEF(ktc1c,Damperktc1c);
        WRITE_DSP_COEF(ktc1d,Damperktc1d);

        TempSeqPC = 0;
        while( (TempSeqPC < 24) || (TempSeqPC > 50) )
        {
            TempSeqPC = (USHORT)READ_REG(HAL_SRVREG_MAP->SEQ_PC);
        }
        WRITE_DSP_COEF(ktc1e,Damperktc1e);
        WRITE_DSP_COEF(ktc1f,Damperktc1f);
    }
    else
    {
        SetSvoEqBank(Ce_Low_Eq, ptrCELFG, 128);
    }
}

/*-----------------------------------------------------------------------------
 *
 * Module: seekSetSeekParas()
 * Description:
 *
 *----------------------------------------------------------------------------*/
void seekSetSeekParas(void)
{
    svoSeekVar.roughCount = 0;
    svoSeekVar.fineCount = 0;
    //Add check counter of searching RF & detecting noRF to limit servo seek retry.
    svoSeekVar.revDur = (USHORT)((ULONG)30000000 / svoSpeedVar.CurrentRpm);
    svoSeekVar.parFine2 = (SHORT)((ULONG)svoSeekVar.targetRadius * 256 / svoSeekVar.parRough);
    switch (svoVar_discKindT.Bit.DiscStd)
    {
        case eDiscCD:
            svoSeekVar.parDelay1 = 1600*JUMP_DELAY_FIX_CD;
            break;

        case eDiscDVD:
            if ((pdbGetDVDTrackPath() == DVDHD_OTP_DISC) && (svoSeekVar.svoTargetID.Layer == 1))
            {
                svoSeekVar.parFine2 = -svoSeekVar.parFine2; //OTP layer 1
            }
            svoSeekVar.parDelay1 = 1600*JUMP_DELAY_FIX_DVD;
            break;

#if (ENABLE_HDDVD == 1)
        case eDiscHD:
            if (svoSeekVar.svoTargetID.SectorNum.all32<HD_CONNECTION_ZONE_START_ADDR)
            {
                svoMiscVar.HD_System_leadInOutFlag = PRESET_HD_SYSTEM_LEAD_IN;
                svoSeekVar.parFine2 = (SHORT)((ULONG)svoSeekVar.targetRadius * 256 / svoSeekVar.parRough_Li);
            }
            else if (svoSeekVar.svoTargetID.SectorNum.all32>HD_SYSTEM_LEADOUT_START_ADDR)
            {
                svoMiscVar.HD_System_leadInOutFlag = PRESET_HD_SYSTEM_LEAD_OUT;
            }
            else
            {
                svoMiscVar.HD_System_leadInOutFlag = HD_SYSTEM_LEAD_TRACK;
            }

            if ((pdbGetHDTrackPath() == DVDHD_OTP_DISC) && (svoSeekVar.svoTargetID.Layer == 1))
            {
                svoSeekVar.parFine2 = -svoSeekVar.parFine2; //OTP layer 1
            }
            svoSeekVar.parDelay1 = 1600*JUMP_DELAY_FIX_HD;
            break;
#endif // (ENABLE_HDDVD == 1)
#if (BD_ENABLE == 1)
        case eDiscBD:
            if (svoSeekVar.svoTargetID.Layer == 1)
            {
                svoSeekVar.parFine2 = -svoSeekVar.parFine2; //OTP layer 1
            }
            if (svoVar_discKindT.Bit.Media == eMediaROM)
            {
                svoSeekVar.parDelay1 = 1600*JUMP_DELAY_FIX_BDROM ;
            }
            else
            {
                svoSeekVar.parDelay1 = 1600*JUMP_DELAY_FIX_BDR_W;
            }
            break;
#endif // (BD_ENABLE == 1)
    }
    svoSeekVar.jumpDistance = 0;
    svoSeekVar.jumpType = 0;
}

/*-----------------------------------------------------------------------------
 *
 * Module: seekSetFineJumpParas()
 * Description: set Kt1B, KICKLVL, BRAKELVL, BRAKEHLD, BRAKESPD, and Ks17
 *
 *----------------------------------------------------------------------------*/
void seekSetFineJumpParas(USHORT TracksToGo)
{
    ESpindleSpeed  speed;
    const SEEK_GAIN *Kt1B_tbl,*KICKLVL_tbl,*BRAKELVL_tbl;
#if (ACTIVE_SLED_FOR_FINE_JUMP == 0)
    const SEEK_GAIN *Ks17_tbl;
#endif // (ACTIVE_SLED_FOR_FINE_JUMP == 0)
    BYTE  BRAKEHLD_val;
    BYTE  BRAKESPD_val;
    BYTE  BRAKELVL_val;
#if ((CHIP_REV >= 0xC0) && (NEW_FINE_SEEK_FEATURE == 1))
    BYTE  BRKLIM_val;
#endif

    speed = svoIramVar.svoCurrentSpeed;
    if (speed == 0)
    {
        return;
    }
    speed--;

    if (svoVar_discKindT.Bit.DiscStd == eDiscCD)
    {
        Kt1B_tbl = svokt1BKickTbl[speed];
        KICKLVL_tbl = svoKICKLVLTbl[speed];
        BRAKELVL_tbl = svoBRAKELVLTbl[speed];
        BRAKEHLD_val = svoBRAKETIMETbl[speed];
#if (ACTIVE_SLED_FOR_FINE_JUMP == 0)
        Ks17_tbl = svoks17KickTbl[svoSeekVar.jumpType & DIR_MASK][speed];
#endif // (ACTIVE_SLED_FOR_FINE_JUMP == 0)
        BRAKESPD_val = svoBRAKESPDTbl[speed];
    }
    else if(svoVar_discKindT.Bit.DiscStd == eDiscDVD)
    {
#if (DVD_RAM_READ == 1)
        if (svoVar_discKindT.Bit.Media == eMediaRAM)
        {
            Kt1B_tbl = dvRamSvokt1BKickTbl[speed];
            KICKLVL_tbl = dvRamSvoKICKLVLTbl[speed];
            BRAKELVL_tbl = dvRamSvoBRAKELVLTbl[speed];
            BRAKEHLD_val = dvRamSvoBRAKETIMETbl[speed];
    #if (ACTIVE_SLED_FOR_FINE_JUMP == 0)
            Ks17_tbl = dvRamSvoks17KickTbl[svoSeekVar.jumpType & DIR_MASK][speed];
    #endif // (ACTIVE_SLED_FOR_FINE_JUMP == 0)
            BRAKESPD_val = dvRamSvoBRAKESPDTbl[speed];
        }
        else
#endif
        if (svoVar_discKindT.Bit.Media == eMediaROM)
        {
            Kt1B_tbl = dvsvokt1BKickTbl[speed];
            KICKLVL_tbl = dvsvoKICKLVLTbl[speed];
            BRAKELVL_tbl = dvsvoBRAKELVLTbl[speed];
            BRAKEHLD_val = dvsvoBRAKETIMETbl[speed];
#if (ACTIVE_SLED_FOR_FINE_JUMP == 0)
            Ks17_tbl = dvsvoks17KickTbl[speed];
#endif // (ACTIVE_SLED_FOR_FINE_JUMP == 0)
            BRAKESPD_val = dvsvoBRAKESPDTbl[speed];
        }
        else
        {
            Kt1B_tbl = rdvsvokt1BKickTbl[speed];
            KICKLVL_tbl = rdvsvoKICKLVLTbl[speed];
            BRAKELVL_tbl = rdvsvoBRAKELVLTbl[speed];
            BRAKEHLD_val = rdvsvoBRAKETIMETbl[speed];
#if (ACTIVE_SLED_FOR_FINE_JUMP == 0)
            Ks17_tbl = rdvsvoks17KickTbl[speed];
#endif // (ACTIVE_SLED_FOR_FINE_JUMP == 0)
            BRAKESPD_val = rdvsvoBRAKESPDTbl[speed];
        }
    }
#if (ENABLE_HDDVD == 1)
    else if (svoVar_discKindT.Bit.DiscStd == eDiscHD)
    {
        if (svoVar_discKindT.Bit.Media == eMediaROM)
        {
            Kt1B_tbl = hdsvokt1BKickTbl[speed];
            KICKLVL_tbl = hdsvoKICKLVLTbl[speed];
            BRAKELVL_tbl = hdsvoBRAKELVLTbl[speed];
            BRAKEHLD_val = hdsvoBRAKETIMETbl[speed];
    #if (ACTIVE_SLED_FOR_FINE_JUMP == 0)
            Ks17_tbl = hdsvoks17KickTbl[svoSeekVar.jumpType & DIR_MASK][speed];
    #endif // (ACTIVE_SLED_FOR_FINE_JUMP == 0)
            BRAKESPD_val = hdsvoBRAKESPDTbl[speed];
        }
        else
        {
            Kt1B_tbl = hdrsvokt1BKickTbl[speed];
            KICKLVL_tbl = hdrsvoKICKLVLTbl[speed];
            BRAKELVL_tbl = hdrsvoBRAKELVLTbl[speed];
            BRAKEHLD_val = hdrsvoBRAKETIMETbl[speed];
    #if (ACTIVE_SLED_FOR_FINE_JUMP == 0)
            Ks17_tbl = hdrsvoks17KickTbl[svoSeekVar.jumpType & DIR_MASK][speed];
    #endif // (ACTIVE_SLED_FOR_FINE_JUMP == 0)
            BRAKESPD_val = hdrsvoBRAKESPDTbl[speed];
        }
    }
#endif // (ENABLE_HDDVD == 1)
#if (BD_ENABLE == 1)
    else if (svoVar_discKindT.Bit.DiscStd == eDiscBD)
    {
        if (svoVar_discKindT.Bit.Media == eMediaROM)
        {
            Kt1B_tbl = bdsvokt1BKickTbl[speed];
            KICKLVL_tbl = bdsvoKICKLVLTbl[speed];
            BRAKELVL_tbl = bdsvoBRAKELVLTbl[speed];
            BRAKEHLD_val = bdsvoBRAKETIMETbl[speed];
    #if (ACTIVE_SLED_FOR_FINE_JUMP == 0)
            Ks17_tbl = bdsvoks17KickTbl[svoSeekVar.jumpType & DIR_MASK][speed];
    #endif // (ACTIVE_SLED_FOR_FINE_JUMP == 0)
            BRAKESPD_val = bdsvoBRAKESPDTbl[speed];
        }
        else
        {
            Kt1B_tbl = bdrsvokt1BKickTbl[speed];
            KICKLVL_tbl = bdrsvoKICKLVLTbl[speed];
            BRAKELVL_tbl = bdrsvoBRAKELVLTbl[speed];
            BRAKEHLD_val = bdrsvoBRAKETIMETbl[speed];
    #if (ACTIVE_SLED_FOR_FINE_JUMP == 0)
            Ks17_tbl = bdrsvoks17KickTbl[svoSeekVar.jumpType & DIR_MASK][speed];
    #endif // (ACTIVE_SLED_FOR_FINE_JUMP == 0)
            BRAKESPD_val = bdrsvoBRAKESPDTbl[speed];
        }
    }
#endif // (BD_ENABLE == 1)

    HAL_DSP_SetKspdStart(TracksToGo);
#if ((CHIP_REV >= 0xC0) && (NEW_FINE_SEEK_FEATURE == 1))
    BRKLIM_val = (BRAKESPD_val - (READ_DSP_COEF(kspd_0) & 0x00FF)) * 3 / 2;
#endif

    //set Kt1B
    for(;;Kt1B_tbl++)      /* servo EQ (kt1B) set */
    {
        if (Kt1B_tbl->jump_count > TracksToGo)
        {
#if (DVD_RAM_READ == 1)
            if (DVDRAM_embossed)
            {
                // Reduce Kt1B for embossed area to compensate the track-pitch & TE slope difference.
                WRITE_DSP_COEF(kt1B, -Kt1B_tbl->gain_data * 128 / TE_SLOPE_FACTOR_DVDRAM_EMBOSS);
            }
            else
#endif
#if (SUPPORT_DPD == 1)
            if ((svoVar_discKindT.Bit.DiscStd == eDiscCD) && (svoIramVar.do_cd_dpd_disc == TRUE))
            {
                WRITE_DSP_COEF(kt1B, -Kt1B_tbl->gain_data * 128 / TRACKING_FACTOR_CD_DPD);
            }
            else
#endif
            {
#if (CD_MB_ONLY_OPU == 1)
               if (svoVar_discKindT.Bit.Media != eMediaROM)
               {
                   WRITE_DSP_COEF(kt1B, -Kt1B_tbl->gain_data * 128 / TRACKING_FACTOR_MB_ONLY);
               }
               else
#endif
               {
                   WRITE_DSP_COEF(kt1B, -Kt1B_tbl->gain_data);
               }
           }
#if (SEEK_ADJ == 1)
            if (servo_debug[1]) WRITE_DSP_COEF(kt1B, -servo_debug[1]);
#endif
            break;
        }
    }

    //set KICKLVL
    for(;;KICKLVL_tbl++)
    {   /* set servo kick level reg */
        if(KICKLVL_tbl->jump_count > TracksToGo)
        {
#if (PCB == 692)
            if (svoVar_discKindT.Bit.DiscStd == eDiscDVD)
            {
                WRITE_REG(HAL_SRVREG_MAP->KICKLVL, KICKLVL_tbl->gain_data);
            }
            else
            {
                //temp, will be replaced by coef
                if (KICKLVL_tbl->gain_data > 0x3F)
                {
                    WRITE_REG(HAL_SRVREG_MAP->KICKLVL, 0x7F);
                }
                else
                {
                    WRITE_REG(HAL_SRVREG_MAP->KICKLVL, KICKLVL_tbl->gain_data * 2);
                }
            }
#else
            WRITE_REG(HAL_SRVREG_MAP->KICKLVL, KICKLVL_tbl->gain_data);
#endif
#if (SEEK_ADJ == 1)
            if (servo_debug[2]) WRITE_REG(HAL_SRVREG_MAP->KICKLVL, servo_debug[2]);
#endif
            break;
        }
    }

    //set BRAKELVL
    for(;;BRAKELVL_tbl++)
    {      /* set servo brake level reg */
        if(BRAKELVL_tbl->jump_count > TracksToGo)
        {
            BRAKELVL_val = (BRAKELVL_tbl->gain_data * READ_DSP_COEF(ktg) + 0x4000) / 0x8000;
#if (DVD_RAM_READ == 1)
            if (DVDRAM_embossed)
            {
                // Reduce BRAKELVL for embossed area to compensate the track-pitch difference.
                BRAKELVL_val = BRAKELVL_val * TRK_PITCH_FACTOR_DVDRAM_EMBOSS / 128;
            }
#endif
            if (BRAKELVL_val > 0x80)
            {
                BRAKELVL_val = 0x80;
            }
#if (SEEK_ADJ == 1)
            if (servo_debug[3]) BRAKELVL_val = servo_debug[3];
#endif
            break;
        }
    }

#if (ACTIVE_SLED_FOR_FINE_JUMP == 0)
    //set Ks17
    for(;;Ks17_tbl++)
    {   /* servo EQ (ks17) set */
        if(Ks17_tbl->jump_count > TracksToGo)
        {
            // use -ks17
            WRITE_DSP_COEF(ks17, (0x10000 - Ks17_tbl->gain_data));
            break;
        }
    }
#endif // (ACTIVE_SLED_FOR_FINE_JUMP == 0)

    //set BRAKESPD
    if (TracksToGo == 1)
    {
#if ((CHIP_REV < 0xC0) || (NEW_FINE_SEEK_FEATURE == 0))
        WRITE_FIELD(HAL_SRV_TROSW, 0);
#endif
#if (DVD_RAM_READ == 1)
        if (DVDRAM)
        {
    #if ((CHIP_REV < 0xC0) || (NEW_FINE_SEEK_FEATURE == 0))
            BRAKESPD_val = (BRAKESPD_val < 0x3F) ? (BRAKESPD_val << 1) : 0x7F;
    #else
            BRAKESPD_val = (BRAKESPD_val < 0x7F) ? (BRAKESPD_val << 1) : 0xFF;
    #endif
        }
        else
#endif
        {
            BRAKESPD_val = BRAKESPD_val * 3;
        }
#if ((CHIP_REV < 0xC0) || (NEW_FINE_SEEK_FEATURE == 0))
        if (BRAKESPD_val > 0x80)
        {
            BRAKELVL_val = BRAKELVL_val * BRAKESPD_val / (0x80 * 2);
            BRAKESPD_val = 0x80;
        }
        else
#endif
        {
            BRAKELVL_val = BRAKELVL_val * 1 / 3;
        }
#if ((CHIP_REV >= 0xC0) && (NEW_FINE_SEEK_FEATURE == 1))
        BRKLIM_val = (BRAKESPD_val - (READ_DSP_COEF(kspd_0) & 0x00FF)) * 3 / 2;
#endif
    }
#if ((CHIP_REV < 0xC0) || (NEW_FINE_SEEK_FEATURE == 0))
    else
    {
        WRITE_FIELD(HAL_SRV_TROSW, 1);
    }
#endif
    WRITE_FIELD(HAL_SRV_BRAKELVL,256 - BRAKELVL_val);
    WRITE_FIELD(HAL_SRV_BRAKESPD,BRAKESPD_val);
#if ((CHIP_REV >= 0xC0) && (NEW_FINE_SEEK_FEATURE == 1))
    WRITE_FIELD(HAL_SRV_BRKLIM, BRKLIM_val);
#endif

    //set BRAKEHLD
#if (SEEK_ADJ == 1)
    if (servo_debug[4]) WRITE_REG(HAL_SRVREG_MAP->BRAKEHLD, servo_debug[4]);
    else
#endif
    WRITE_REG(HAL_SRVREG_MAP->BRAKEHLD, BRAKEHLD_val);
    //WRITE_FIELD(HAL_SRV_ANTIBRAK, 0x00);

}
/*------------------------------------------------------------------------*/
/* The end of file                                                        */
/*------------------------------------------------------------------------*/
