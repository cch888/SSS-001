/****************************************************************************
*   (c) Copyright 2001 - 2008 SUNEXT DESIGN, INC.
*                           All Rights Reserved
*
*
* $Revision: 754 $
* $Date: 11/03/25 5:03p $
*
* DESCRIPTION: This file contains the functions related to AFE power up
*              calibration and Disk Detection!!
*              Please add function name below!!
*                  -- adj_next_offset()
*                  -- adj_next_gain()
*                  -- calAdjNextRoughOffset()
*                  -- calAdjNextFineGain()
*                  -- calAdjRRFGain()
*                  -- calAdjRRFOffset()
*                  -- calCalibrateOffset()
*                  -- calCalibrateStepsize()
*                  -- calCalibrateTEGnOs()
*                  -- calCalibrateRPBG()
*                  -- calCalibrateRPOS()
*                  -- calCalibrateRPGnOs()
*                  -- calCalibrateTEBGTROOS()
*                  -- calCalibrateMPPSPP1BG()
*                  -- calCalibrateCEBG()
*                  -- calCalibrateCEOS()
*                  -- calGetGainVref()
*                  -- feg_stop()
*                  -- teg_stop()
*                  -- read_BHO()
*                  -- read_PHO()
*                  -- febc_prepare()
*                  -- febc_stop()
*                  -- Plus_Minus_det()
*
*
**************************** SUNEXT CONFIDENTIAL ****************************/
#include ".\common\globals.h"
#include ".\common\globtype.h"
#include ".\common\chip.h"
#include ".\common\comMath.h"
#include ".\common\stdlib.h"
#include ".\common\ADC.h"
#include ".\common\system.h"
#include ".\common\iram.h"
#include ".\al\REG_HAL.h"
#include ".\al\REG_afe.h"
#include ".\al\REG_dvd_decoder.h"
#include ".\al\reg_intctrl.h"
#include ".\common\intctl.h"
#include ".\servo\svo.h"
#include ".\servo\svo_com.h"
#include ".\servo\svo_lib.h"
#include ".\servo\svo_afe.h"
#include ".\servo\svoVar.h"
#include ".\servo\svo_sta.h"
#include ".\servo\svo_calib.h"
#include ".\servo\afe_tbl.h"
#include ".\servo\svo_tbl.h"
#include ".\servo\svomacro.h"
#include ".\servo\svo_step.h"
#include ".\servo\svo_seek.h"
#include ".\servo\svo_speed.h"
#if (DEFECT_LENGTH_MEASURE == 1)
#include ".\servo\svoVar.h"
#endif
#include ".\al\lal.h"
#include ".\al\coefs.h"
#include ".\al\HAL_dsp.h"
#include ".\al\HAL_afe.h"
#include ".\servo\svo_dsp.h"
#include ".\al\REG_HAL.h"
#include ".\al\REG_afe.h"
#include ".\al\HAL_wobble.h"
#if (ENABLE_HDDVD == 1)
#include ".\hd\hdDB.h"
#endif

#define TOP_SATURATINO_MV    (((0x3D0 * GPADC_IN_RANGE) >> GPADC_BITS) + GPADC_IN_MIN)
#define BOTTOM_SATURATION_MV (((0x020 * GPADC_IN_RANGE) >> GPADC_BITS) + GPADC_IN_MIN)

/*-----------------------------------------------------------------------------
 *
 * Module: adj_next_gain()
 * Description: 1. Assume: measured p2p value/signal does not saturated
 *              2. Update next gain for gain calibration
 *----------------------------------------------------------------------------*/
uchar_return_t adj_next_gain(BYTE   cur_gain_cnt, // reg gain in count
                             BYTE   max_gain_cnt, // reg gain in count
                             USHORT meas_p2p,     // unit: mV
                             USHORT target_p2p,   // unit: mV
                             float  dB_per_cnt,   // unit: dB/count
                             float  adj_factor)   // unit:
{
    float           dB_rate;
    float           f_count;
    SHORT           new_count;
    uchar_return_t  res;
    SBYTE           changed_count;

    if (adj_factor >= 1.0)
        adj_factor = 1.0;                                                     // never do "over adjustment "

    dB_rate = CalculateXdBFromRate(ONEdB, target_p2p, meas_p2p);

    f_count = dB_rate/dB_per_cnt;                                             // unit:  count in float fromat

    //---------------------------------;
    // limit the gain changing size    ;
    //---------------------------------;
    f_count *= adj_factor;                                                    // apply adjusting factor

    if (f_count > 100.0)
        f_count = 100.0;
    if (f_count < -100.0)
        f_count = -100.0;

    //---------------------------------;
    // calculate the changing count    ;
    //---------------------------------;
    if (f_count > 0)
        changed_count = (SBYTE)(f_count + 0.5);
    else if (f_count < 0)
        changed_count = (SBYTE)(f_count - 0.5);
    else
        changed_count = 0;

    //---------------------------------;
    // limit the reg count range       ;
    //---------------------------------;
    new_count   = (SHORT)cur_gain_cnt + (SHORT)changed_count;
    res.errcode = eNOERR;     // assume there is no error

    if (new_count < 0){
        new_count = 0;
        res.errcode = eERR_adj_next_gain_low_limit;
    }

    if (new_count > max_gain_cnt){
        new_count = max_gain_cnt;
        res.errcode = eERR_adj_next_gain_high_limit;
    }

    res.val = (BYTE)new_count;
    return(res);
}// end of adj_next_gain

/*-----------------------------------------------------------------------------
 *
 * Module: calRadius2Zone()
 * Description: return zone number based on input radius
 *
 *----------------------------------------------------------------------------*/
const USHORT zone_end_radius[CAL_RADIUS_ZONE_NR - 1] = {
    UM_2_SLED_STEP(CAL_ZONE_SWITCH_POINT01),
    UM_2_SLED_STEP(CAL_ZONE_SWITCH_POINT02),
    UM_2_SLED_STEP(CAL_ZONE_SWITCH_POINT03),
    UM_2_SLED_STEP(CAL_ZONE_SWITCH_POINT04),
    UM_2_SLED_STEP(CAL_ZONE_SWITCH_POINT05),
    UM_2_SLED_STEP(CAL_ZONE_SWITCH_POINT06),
    UM_2_SLED_STEP(CAL_ZONE_SWITCH_POINT07),
    UM_2_SLED_STEP(CAL_ZONE_SWITCH_POINT08),
    UM_2_SLED_STEP(CAL_ZONE_SWITCH_POINT09),
    UM_2_SLED_STEP(CAL_ZONE_SWITCH_POINT10),
    UM_2_SLED_STEP(CAL_ZONE_SWITCH_POINT11),
    UM_2_SLED_STEP(CAL_ZONE_SWITCH_POINT12),
    UM_2_SLED_STEP(CAL_ZONE_SWITCH_POINT13),
    UM_2_SLED_STEP(CAL_ZONE_SWITCH_POINT14),
    UM_2_SLED_STEP(CAL_ZONE_SWITCH_POINT15),
    UM_2_SLED_STEP(CAL_ZONE_SWITCH_POINT16),
    UM_2_SLED_STEP(CAL_ZONE_SWITCH_POINT17),
    UM_2_SLED_STEP(CAL_ZONE_SWITCH_POINT18),
    UM_2_SLED_STEP(CAL_ZONE_SWITCH_POINT19)
};

BYTE calRadius2Zone(USHORT sled_pos)
{
    BYTE i;

    for (i = 0; i < (CAL_RADIUS_ZONE_NR - 1); i++)
    {
        if (sled_pos <= zone_end_radius[i])
        {
            break;
        }
    }
    return i;
}// end of calRadius2Zone


/*-----------------------------------------------------------------------------
 *
 * Module: calDacCount2SBYTE()
 * Description:
 * <Bits = 8>:
 * dac   (input) : 0x00,0x01,...,0x7E,0x7F,0x80,0x81,...,0xFE,0xFF
 * offset(output):    0,   1,..., 126, 127,   0,  -1,...,-126,-127
 *
 * <Bits = 7>:
 * dac   (input) : 0x00,0x01,...,0x3E,0x3F,0x40,0x41,...,0x7E,0x7F
 * offset(output):    0,   1,...,  62,  63,   0,  -1,..., -62, -63
 *
 * <Bits = 6>:
 * dac   (input) : 0x00,0x01,...,0x1E,0x1F,0x20,0x21,...,0x3E,0x3F
 * offset(output):    0,   1,...,  30,  31,   0,  -1,..., -30, -31
 *
 * ...
 *----------------------------------------------------------------------------*/
SBYTE calDacCount2SBYTE(BYTE dac_count,BYTE total_Bits)
{
    BYTE value;

    value = (1<<(total_Bits-1));
    if (dac_count >= value)
        return((SBYTE)(value-dac_count));
    else
        return((SBYTE)dac_count);
}

/*-----------------------------------------------------------------------------
 *
 * Module: calSBYTE2DacCount()
 * Description:
 * <total_Bits = 8>:
 * offset(input) : -127,-126,...,  -1,   0,   1,..., 126, 127
 * dac   (output): 0xFF,0xFE,...,0x81,0x00,0x01,...,0x7E,0x7F
 *
 * <total_Bits = 7>:
 * offset(input) :  -63, -62,...,  -1,   0,   1,...,  62,  63
 * dac   (output): 0x7F,0x7E,...,0x41,0x00,0x01,...,0x3E,0x3F
 *
 * <total_Bits = 6>:
 * offset(input) :  -31, -30,...,  -1,   0,   1,...,  30,  31
 * dac   (output): 0x3F,0x3E,...,0x21,0x00,0x01,...,0x1E,0x1F
 *
 * ...
 *----------------------------------------------------------------------------*/
BYTE calSBYTE2DacCount(SHORT sbyte,BYTE total_Bits)
{
    BYTE value;

    value = (1<<(total_Bits-1));

    if (sbyte < 0)
    {
        sbyte = (sbyte <= (-value))? (-value+1) : sbyte;
        return((BYTE)(value-sbyte));
    }
    else
    {
        sbyte = (sbyte >= value)?  (value-1) : sbyte;
        return((BYTE)sbyte);
    }
}

/*-----------------------------------------------------------------------------
 *
 * Module: calBestGain()
 * Description: Find the best gain based on step_size
 *              step_size[gain_orig] : step_size[gain_best] = current : target
 *
 *----------------------------------------------------------------------------*/
BYTE calBestGain(USHORT *step_size, BYTE gain_orig, BYTE max_gain, USHORT current, USHORT target)
{
    BYTE  i,gain_best;
    ULONG temp,min_error,error;

    if (current == 0)
    {
        SendMsg80(SHOW_DEBUG_MSG,0x310600); // calBG,DIV0
        return 0;
    }

    temp = ((ULONG)step_size[gain_orig] * target / current);
    min_error = 0xFFFFFFFF;
    for (i = 0; i <= max_gain; i++)
    {
        error = ABS(temp - step_size[i]);
        if (error < min_error)
        {
            min_error = error;
            gain_best = i;
        }
    }

    return gain_best;
}

/*-----------------------------------------------------------------------------
 *
 * Module: calAdjNextRoughOffset()
 * Description: Calculate next offset count by increasing
 *              or decreasing input moving count,
 *
 *----------------------------------------------------------------------------*/
uchar_return_t calAdjNextRoughOffset(BYTE    cur_offset_cnt,  // reg offset in count
                                     BYTE    total_Bits,      // offset field total bits
                                     SHORT   moving_count,    // unit: adc count
                                     BOOL    only_positive)
{
    SHORT           cur_offset;
    uchar_return_t  res;
    SBYTE   max_offset,min_offset;

    max_offset = (1<<(total_Bits-1))-1;
    min_offset = (only_positive? 0 : -max_offset);

    res.errcode = eNOERR;  // assume there is no error
    cur_offset = calDacCount2SBYTE(cur_offset_cnt,total_Bits);
    cur_offset += moving_count;

    if (cur_offset > max_offset)
    {
        cur_offset = max_offset;
        res.errcode = eERR_adj_next_offset_over_high_limit;
    }
    if (cur_offset < min_offset)
    {
        cur_offset = min_offset;
        res.errcode = eERR_adj_next_offset_over_low_limit;
    }

    res.val = calSBYTE2DacCount(cur_offset,total_Bits);
    return(res);
}

/*-----------------------------------------------------------------------------
 *
 * Module: calAdjNextFineGain()
 * Description: Calculate next gain count by increasing
 *              or decreasing 1 step only,
 *
 *----------------------------------------------------------------------------*/
uchar_return_t calAdjNextFineGain(BYTE    cur_gain_cnt,    // reg offset in count
                                  SBYTE   max_cnt,        // reg offset in count max limit
                                  SBYTE   min_cnt,        // reg offset in count min limit
                                  SHORT   cur_p2p,        // unit: mV
                                  SHORT   target_p2p)   // unit: mV
{
    SHORT            p2p_error,next_gain_count;
    uchar_return_t   res;

    //---------------------------------------------------------------------;
    // handling the tunning direction                                      ;
    //---------------------------------------------------------------------;
    p2p_error = target_p2p - cur_p2p;           // unit mV

    if (p2p_error > 0)//need to increase gain
    {
        if(cur_gain_cnt == max_cnt)
        {
            res.val     = max_cnt;
            res.errcode = eERR_adj_next_gain_high_limit;
            SendMsg80(SHOW_DEBUG_MSG,0x310601); // WARNING: Max. Gain Limit
            return(res);
        }
        else
        {
            next_gain_count = cur_gain_cnt + 1;
        }
    }
    else//need to reduce gain
    {
        if(cur_gain_cnt == min_cnt)  // check the limit
        {
            res.val     = min_cnt;
            res.errcode = eERR_adj_next_gain_low_limit;
            SendMsg80(SHOW_DEBUG_MSG,0x310602); // WARNING: Min. Gain Limit
            return(res);
        }
        else
        {
            next_gain_count = cur_gain_cnt - 1;
        }
    }

    //---------------------------------------------------------------------;
    // check the limit                                                     ;
    //---------------------------------------------------------------------;
    res.errcode = eNOERR;                                  // assume there is no error
    res.val = next_gain_count;
    return(res);
}

/*-----------------------------------------------------------------------------
 *
 * Module: calAdjRRFGain()
 * Description: Calculate next RRF gain
 *
 *----------------------------------------------------------------------------*/
schar_return_t calAdjRRFGain(USHORT    meas_p2p,     // unit: mV
                             USHORT    target_p2p,   // unit: mV
                             BYTE      rrf_gain)
{
    schar_return_t res;
    BYTE   rrf_gain_new;

    rrf_gain_new = calBestGain(svoRRFVar.step_size, rrf_gain, 0x1F, meas_p2p, target_p2p);

    res.val = rrf_gain_new - rrf_gain;
    SendMsgCnB(DEBUG_AUTO_TUNING_MSG,4,0x338309,F4B(res.val/2.0));
    res.errcode = eNOERR;

    return(res);
}

/*-----------------------------------------------------------------------------
 *
 * Module: calAdjRRFOffset()
 * Description: Calculate next RRF offset
 *
 *----------------------------------------------------------------------------*/
schar_return_t calAdjRRFOffset(SHORT     cur_middle,   // unit: mV
#if (NEW_RFPOS_CALIB == 1)
                               USHORT    meas_p2p,
                               USHORT    target_p2p,
#endif
                               BYTE      cur_gain)
{
    schar_return_t res;
    LONG   temp;

#if (NEW_RFPOS_CALIB == 1)
    cur_middle += svoRRFVar.rfagc_ref * (meas_p2p - target_p2p) / target_p2p;
#endif
    temp = -RoundDivide((LONG)cur_middle * 256, svoRRFVar.step_size[cur_gain]);

    if (temp >  127) temp =  127;
    if (temp < -128) temp = -128;

    res.val = temp;
    // ADJ_RF_OFFSET = %2d (mV)
    SendMsgCnB(DEBUG_AUTO_TUNING_MSG,2,0x33830A,B2B(temp*4));
    res.errcode = eNOERR;                                     // assume there is no error
    return(res);
}

/*-----------------------------------------------------------------------------
 *
 * Module: calStoreAfeVar()
 * Description: Set registers value to svoCalxxxTxVar
 *
 *----------------------------------------------------------------------------*/
void calStoreAfeVar(void)
{
    ULONG i;

    for (i = 0; i < MAX_TYPE1_OFS_NO; i++)
    {
        svoCalOfsT1Var[0][i].write_offset      = READ_REG(*(vBYTE *)AfeOffsetInfoT1[i].field_write_ofs);
#if (CHIP_REV >= 0xC0)
        svoCalOfsT1Var[0][i].land_write_offset = READ_REG(*(vBYTE *)AfeOffsetInfoT1[i].field_land_write_ofs);
#endif

        svoCalOfsT1Var[0][i].groove_offset[RECDL] = READ_REG(*(vBYTE *)AfeOffsetInfoT1[i].field_groove_ofs[RECDL]);
        svoCalOfsT1Var[0][i].groove_offset[RECDH] = READ_REG(*(vBYTE *)AfeOffsetInfoT1[i].field_groove_ofs[RECDH]);

        svoCalOfsT1Var[0][i].land_offset[RECDL] = READ_REG(*(vBYTE *)AfeOffsetInfoT1[i].field_land_ofs[RECDL]);
        svoCalOfsT1Var[0][i].land_offset[RECDH] = READ_REG(*(vBYTE *)AfeOffsetInfoT1[i].field_land_ofs[RECDH]);

        svoCalOfsT1Var[1][i] = svoCalOfsT1Var[0][i];
    }

    for (i = 0; i < MAX_TYPE2_OFS_NO; i++)
    {
        if ((i != RFCP_OS) && (i != RFCN_OS))
        {
            svoCalOfsT2Var[0][i].write_offset =
            READ_REG(*(vBYTE *)AfeOffsetInfoT2[i].field_write_ofs);

            svoCalOfsT2Var[0][i].normal_offset =
            READ_REG(*(vBYTE *)AfeOffsetInfoT2[i].field_normal_ofs);

            svoCalOfsT2Var[1][i] = svoCalOfsT2Var[0][i];
        }
    }

    for (i = 0; i < MAX_TYPE3_OFS_NO; i++)
    {
        svoCalOfsT3Var[0][i].normal_offset =
        READ_REG(*(vBYTE *)AfeOffsetInfoT3[i].field_normal_ofs);

        svoCalOfsT3Var[1][i] = svoCalOfsT3Var[0][i];
    }

    for (i = 0; i < MAX_TYPE1_GN_NO; i++)
    {
        svoCalGnT1Var[0][i].write_gain = READ_FROM_FIELD(*(BYTE *)AfeGainInfoT1[i].field_write_gn.addr,
                                 AfeGainInfoT1[i].field_write_gn.mask,
                                 AfeGainInfoT1[i].field_write_gn.start_bit);

        svoCalGnT1Var[0][i].groove_gain[RECDL] = READ_FROM_FIELD(*(BYTE *)AfeGainInfoT1[i].field_groove_gn[RECDL].addr,
                                 AfeGainInfoT1[i].field_groove_gn[RECDL].mask,
                                 AfeGainInfoT1[i].field_groove_gn[RECDL].start_bit);

        svoCalGnT1Var[0][i].groove_gain[RECDH] = READ_FROM_FIELD(*(BYTE *)AfeGainInfoT1[i].field_groove_gn[RECDH].addr,
                                 AfeGainInfoT1[i].field_groove_gn[RECDH].mask,
                                 AfeGainInfoT1[i].field_groove_gn[RECDH].start_bit);

        svoCalGnT1Var[0][i].land_gain[RECDL] = READ_FROM_FIELD(*(BYTE *)AfeGainInfoT1[i].field_land_gn[RECDL].addr,
                                 AfeGainInfoT1[i].field_land_gn[RECDL].mask,
                                 AfeGainInfoT1[i].field_land_gn[RECDL].start_bit);

        svoCalGnT1Var[0][i].land_gain[RECDH] = READ_FROM_FIELD(*(BYTE *)AfeGainInfoT1[i].field_land_gn[RECDH].addr,
                                 AfeGainInfoT1[i].field_land_gn[RECDH].mask,
                                 AfeGainInfoT1[i].field_land_gn[RECDH].start_bit);

        svoCalGnT1Var[1][i] = svoCalGnT1Var[0][i];
    }

    for (i = 0; i < MAX_TYPE2_GN_NO; i++)
    {
        svoCalGnT2Var[0][i].write_gain =
        READ_FROM_FIELD(*(BYTE *)AfeGainInfoT2[i].field_write_gn.addr,
                                 AfeGainInfoT2[i].field_write_gn.mask,
                                 AfeGainInfoT2[i].field_write_gn.start_bit);

        svoCalGnT2Var[0][i].normal_gain =
        READ_FROM_FIELD(*(BYTE *)AfeGainInfoT2[i].field_normal_gn.addr,
                                 AfeGainInfoT2[i].field_normal_gn.mask,
                                 AfeGainInfoT2[i].field_normal_gn.start_bit);

        svoCalGnT2Var[1][i] = svoCalGnT2Var[0][i];
    }

    for (i = 0; i < MAX_TYPE3_GN_NO; i++)
    {
        svoCalGnT3Var[0][i].normal_gain =
        READ_FROM_FIELD(*(BYTE *)AfeGainInfoT3[i].field_normal_gn.addr,
                                 AfeGainInfoT3[i].field_normal_gn.mask,
                                 AfeGainInfoT3[i].field_normal_gn.start_bit);

        svoCalGnT3Var[1][i] = svoCalGnT3Var[0][i];
    }

#if (AFE_NORMALIZER == 1)
    for (i = 0; i < MAX_NOR_BS_NO; i++)
    {
        svoCalBsNorVar[0][i].normal_bs =
        READ_FROM_FIELD(*(BYTE *)AfeBsInfoNor[i].field_normal_bs.addr,
                                 AfeBsInfoNor[i].field_normal_bs.mask,
                                 AfeBsInfoNor[i].field_normal_bs.start_bit);

        svoCalGnT2Var[1][i] = svoCalGnT2Var[0][i];
    }
#endif

    for (i = 0; i <= 1; i++)
    {
        svoCalMisc1Var[i].feGainDSP = 0x4000;
        svoCalMisc1Var[i].teGainDSP = 0x4000;
        svoCalMisc1Var[i].teSetPoint = 0;
    }

    calClrCalResult(TILT_DAT,FALSE);
    calClrCalResult(FEBC_DSP,FALSE);
#if COLLI_SWITCH ==1
    calClrCalResult(COLLI_POS,FALSE);
#endif
#if (DPD_COMPENSATE_OFFSET==1)
    calClrCalResult(DPD_GNOS,FALSE);
#endif
    svoCalMisc2Var.troos = READ_REG(HAL_SRVREG_MAP->TROFFSET);
    SendMsg80(DEBUG_CALIB_RESULT_MSG,0x316000); // INIT CALIB TBL
}// end of calStoreAfeVar

/*-----------------------------------------------------------------------------
 *
 * Module: calSetAfeOffset()
 * Description: Set offset register with new value and save it to svoCalOfsTxVar as well
 *
 *----------------------------------------------------------------------------*/
void calSetAfeOffset(BYTE layer,BYTE adjust_signal,BYTE type,BYTE path_select1,BYTE path_select2,BYTE new_value)
{
    switch (type)
    {
        case TYPE1:
            if ((path_select1 == WRITE_OFFSET) || (path_select1 == PATH_SELECT_ALL))
            {
                svoCalOfsT1Var[layer][adjust_signal].write_offset = new_value;
                WRITE_REG(*(BYTE *)(AfeOffsetInfoT1[adjust_signal].field_write_ofs), new_value);
#if (CHIP_REV >= 0xC0)
                // temp solution: must be replaced by individual land/groove offset calibration later
                WRITE_REG(*(BYTE *)(AfeOffsetInfoT1[adjust_signal].field_land_write_ofs), new_value);
#endif
            }

            if (path_select1 == LI_OFFSET)
            {
                svoCalOfsT1Var[layer][adjust_signal].li_offset = new_value;
                WRITE_REG(*(BYTE *)(AfeOffsetInfoT1[adjust_signal].field_groove_ofs[RECDH]),new_value);
            }

            if ((path_select1 == GROOVE_OFFSET) || (path_select1 == PATH_SELECT_ALL))
            {
                if ((path_select2 == RECDL) || (path_select2 == PATH_SELECT_ALL))
                {
                    svoCalOfsT1Var[layer][adjust_signal].groove_offset[RECDL] = new_value;
                    WRITE_REG(*(BYTE *)(AfeOffsetInfoT1[adjust_signal].field_groove_ofs[RECDL]), new_value);
                }
                if ((path_select2 == RECDH) || (path_select2 == PATH_SELECT_ALL))
                {
                    if (path_select1 == PATH_SELECT_ALL)
                    {
                        svoCalOfsT1Var[layer][adjust_signal].li_offset = new_value;
                    }
                    svoCalOfsT1Var[layer][adjust_signal].groove_offset[RECDH] = new_value;
                    WRITE_REG(*(BYTE *)(AfeOffsetInfoT1[adjust_signal].field_groove_ofs[RECDH]),new_value);
                }
            }

            if ((path_select1 == LAND_OFFSET) || (path_select1 == PATH_SELECT_ALL))
            {
                if ((path_select2 == RECDL) || (path_select2 == PATH_SELECT_ALL))
                {
                    svoCalOfsT1Var[layer][adjust_signal].land_offset[RECDL] = new_value;
                    WRITE_REG(*(BYTE *)(AfeOffsetInfoT1[adjust_signal].field_land_ofs[RECDL]), new_value);
                }
                if ((path_select2 == RECDH) || (path_select2 == PATH_SELECT_ALL))
                {
                    svoCalOfsT1Var[layer][adjust_signal].land_offset[RECDH] = new_value;
                    WRITE_REG(*(BYTE *)(AfeOffsetInfoT1[adjust_signal].field_land_ofs[RECDH]), new_value);
                }
            }
            break;

        case TYPE2:
            if ((path_select1 == WRITE_OFFSET) || (path_select1 == PATH_SELECT_ALL))
            {
                svoCalOfsT2Var[layer][adjust_signal].write_offset = new_value;
                WRITE_REG(*(BYTE *)(AfeOffsetInfoT2[adjust_signal].field_write_ofs), new_value);
            }
            if ((path_select1 == NORMAL_OFFSET) || (path_select1 == PATH_SELECT_ALL))
            {
                svoCalOfsT2Var[layer][adjust_signal].normal_offset = new_value;
                WRITE_REG(*(BYTE *)(AfeOffsetInfoT2[adjust_signal].field_normal_ofs), new_value);
            }
            break;

        case TYPE3:
            svoCalOfsT3Var[layer][adjust_signal].normal_offset = new_value;
            WRITE_REG(*(BYTE *)(AfeOffsetInfoT3[adjust_signal].field_normal_ofs), new_value);
            break;

        case TYPE4:
            svoCalOfsT4Var[adjust_signal].normal_offset = new_value;
            READ_MOD_WRITE(*(BYTE *)(AfeOffsetInfoT4[adjust_signal].field_normal_ofs.addr),
                                    (AfeOffsetInfoT4[adjust_signal].field_normal_ofs.mask),
                                    (new_value << (AfeOffsetInfoT4[adjust_signal].field_normal_ofs.start_bit)));
            break;

#if (AFE_NORMALIZER == 1)
        case NOR_BS:
            if (new_value > AfeBsInfoNor[adjust_signal].MaxBsCnt)
            {
                SendMsg80(SHOW_DEBUG_MSG,0x310603); // WARNING: Max. BS Limit
                new_value = AfeBsInfoNor[adjust_signal].MaxBsCnt;
            }
            svoCalBsNorVar[layer][adjust_signal].normal_bs = new_value;
            READ_MOD_WRITE(*(BYTE *)(AfeBsInfoNor[adjust_signal].field_normal_bs.addr),
                                    (AfeBsInfoNor[adjust_signal].field_normal_bs.mask),
                                    (new_value << (AfeBsInfoNor[adjust_signal].field_normal_bs.start_bit)));
            break;
#endif
#if (MPPBG_SPPBG_CALIB != 0)
        case TYPE2BAL:
            svoCalBalOfsT2Var[adjust_signal][path_select2].normal_offset = calDacCount2SBYTE(new_value,AfeOffsetInfoT2[adjust_signal].total_Bits);
            break;
#endif
    }
}

/*-----------------------------------------------------------------------------
 *
 * Module: calSaveAllAfeOffset()
 * Description: set same offset count to all related offset register of specificed signal
 *
 *----------------------------------------------------------------------------*/
void calSaveAllAfeOffset(BYTE adjust_signal,BYTE type,BYTE offset_cnt)
{
    switch (type)
    {
        case TYPE1:
            WRITE_REG(*(BYTE *)(AfeOffsetInfoT1[adjust_signal].field_write_ofs), offset_cnt);
#if (CHIP_REV >= 0xC0)
            WRITE_REG(*(BYTE *)(AfeOffsetInfoT1[adjust_signal].field_land_write_ofs), offset_cnt);
#endif
            WRITE_REG(*(BYTE *)(AfeOffsetInfoT1[adjust_signal].field_groove_ofs[RECDL]), offset_cnt);
            WRITE_REG(*(BYTE *)(AfeOffsetInfoT1[adjust_signal].field_groove_ofs[RECDH]), offset_cnt);
            WRITE_REG(*(BYTE *)(AfeOffsetInfoT1[adjust_signal].field_land_ofs[RECDL]), offset_cnt);
            WRITE_REG(*(BYTE *)(AfeOffsetInfoT1[adjust_signal].field_land_ofs[RECDH]), offset_cnt);
            break;

        case TYPE2:
            WRITE_REG(*(BYTE *)(AfeOffsetInfoT2[adjust_signal].field_write_ofs), offset_cnt);
            WRITE_REG(*(BYTE *)(AfeOffsetInfoT2[adjust_signal].field_normal_ofs), offset_cnt);
            break;

        case TYPE3:
            WRITE_REG(*(BYTE *)(AfeOffsetInfoT3[adjust_signal].field_normal_ofs), offset_cnt);
            break;

        case TYPE4:
            READ_MOD_WRITE(*(BYTE *)(AfeOffsetInfoT4[adjust_signal].field_normal_ofs.addr),
                                    (AfeOffsetInfoT4[adjust_signal].field_normal_ofs.mask),
                                    (offset_cnt << (AfeOffsetInfoT4[adjust_signal].field_normal_ofs.start_bit)));
            break;
    }
}

/*-----------------------------------------------------------------------------
 *
 * Module: calSetAfeGain()
 * Description: Set gain register with new value and save it to svoCalGnTxVar as well
 *
 *----------------------------------------------------------------------------*/
void calSetAfeGain(BYTE layer,BYTE adjust_signal,BYTE type,BYTE path_select1,BYTE path_select2,SBYTE new_value)
{
    BYTE max;

    switch (type)
    {
        case TYPE1: // FE_GN (0x50) .. TE_GN (0x53)
            max = AfeGainInfoT1[adjust_signal].MaxGainCnt;
            break;

        case TYPE2:// BS_GN (0x54) .. SB_GN (0x5B)
            max = AfeGainInfoT2[adjust_signal].MaxGainCnt;
            break;

        case TYPE3:// CE_GN (0x5C) .. SPP1B_GN (0x71)
        default:
            max = AfeGainInfoT3[adjust_signal].MaxGainCnt;
            break;
    }

    if (new_value > max)
    {
#if (Customer_A1 ==1)
				PositionHomeErrorCount++;	
#endif
    
        SendMsg80(SHOW_DEBUG_MSG,0x310601); // WARNING: Max. Gain Limit
        new_value = max;
    }
    if (new_value < 0)
    {
#if (Customer_A1 ==1)
				PositionHomeErrorCount++;	
#endif
    
        SendMsg80(SHOW_DEBUG_MSG,0x310602); // WARNING: Min. Gain Limit
        new_value = 0;
    }

    switch (type)
    {
        case TYPE1:
            if ((path_select1 == WRITE_GAIN) || (path_select1 == PATH_SELECT_ALL))
            {
                svoCalGnT1Var[layer][adjust_signal].write_gain = new_value;
                READ_MOD_WRITE(*(BYTE *)(AfeGainInfoT1[adjust_signal].field_write_gn.addr),
                                        (AfeGainInfoT1[adjust_signal].field_write_gn.mask),
                                        (new_value << (AfeGainInfoT1[adjust_signal].field_write_gn.start_bit)));
            }

            if (path_select1 == LI_GAIN)
            {
                svoCalGnT1Var[layer][adjust_signal].li_gain = new_value;
                READ_MOD_WRITE(*(BYTE *)(AfeGainInfoT1[adjust_signal].field_groove_gn[RECDH].addr),
                                        (AfeGainInfoT1[adjust_signal].field_groove_gn[RECDH].mask),
                                        (new_value << (AfeGainInfoT1[adjust_signal].field_groove_gn[RECDH].start_bit)));
            }

            if ((path_select1 == GROOVE_GAIN) || (path_select1 == PATH_SELECT_ALL))
            {
                if ((path_select2 == RECDL) || (path_select2 == PATH_SELECT_ALL))
                {
                    svoCalGnT1Var[layer][adjust_signal].groove_gain[RECDL] = new_value;
                    READ_MOD_WRITE(*(BYTE *)(AfeGainInfoT1[adjust_signal].field_groove_gn[RECDL].addr),
                                            (AfeGainInfoT1[adjust_signal].field_groove_gn[RECDL].mask),
                                            (new_value << (AfeGainInfoT1[adjust_signal].field_groove_gn[RECDL].start_bit)));
                }
                if ((path_select2 == RECDH) || (path_select2 == PATH_SELECT_ALL))
                {
                    if (path_select1 == PATH_SELECT_ALL)
                    {
                        svoCalGnT1Var[layer][adjust_signal].li_gain = new_value;
                    }
                    svoCalGnT1Var[layer][adjust_signal].groove_gain[RECDH] = new_value;
                    READ_MOD_WRITE(*(BYTE *)(AfeGainInfoT1[adjust_signal].field_groove_gn[RECDH].addr),
                                            (AfeGainInfoT1[adjust_signal].field_groove_gn[RECDH].mask),
                                            (new_value << (AfeGainInfoT1[adjust_signal].field_groove_gn[RECDH].start_bit)));
                }
            }

            if ((path_select1 == LAND_GAIN) || (path_select1 == PATH_SELECT_ALL))
            {
                if ((path_select2 == RECDL) || (path_select2 == PATH_SELECT_ALL))
                {
                    svoCalGnT1Var[layer][adjust_signal].land_gain[RECDL] = new_value;
                    READ_MOD_WRITE(*(BYTE *)(AfeGainInfoT1[adjust_signal].field_land_gn[RECDL].addr),
                                            (AfeGainInfoT1[adjust_signal].field_land_gn[RECDL].mask),
                                            (new_value << (AfeGainInfoT1[adjust_signal].field_land_gn[RECDL].start_bit)));
                }
                if ((path_select2 == RECDH) || (path_select2 == PATH_SELECT_ALL))
                {
                    svoCalGnT1Var[layer][adjust_signal].land_gain[RECDH] = new_value;
                    READ_MOD_WRITE(*(BYTE *)(AfeGainInfoT1[adjust_signal].field_land_gn[RECDH].addr),
                                            (AfeGainInfoT1[adjust_signal].field_land_gn[RECDH].mask),
                                            (new_value << (AfeGainInfoT1[adjust_signal].field_land_gn[RECDH].start_bit)));
                }
            }
            break;

        case TYPE2:
            if ((path_select1 == WRITE_GAIN) || (path_select1 == PATH_SELECT_ALL))
            {
                svoCalGnT2Var[layer][adjust_signal].write_gain = new_value;
                READ_MOD_WRITE(*(BYTE *)(AfeGainInfoT2[adjust_signal].field_write_gn.addr),
                                        (AfeGainInfoT2[adjust_signal].field_write_gn.mask),
                                        (new_value << (AfeGainInfoT2[adjust_signal].field_write_gn.start_bit)));
            }

            if ((path_select1 == NORMAL_GAIN) || (path_select1 == PATH_SELECT_ALL))
            {
                svoCalGnT2Var[layer][adjust_signal].normal_gain = new_value;
                READ_MOD_WRITE(*(BYTE *)(AfeGainInfoT2[adjust_signal].field_normal_gn.addr),
                                        (AfeGainInfoT2[adjust_signal].field_normal_gn.mask),
                                        (new_value << (AfeGainInfoT2[adjust_signal].field_normal_gn.start_bit)));
            }
            break;

        case TYPE3:
        default:
            svoCalGnT3Var[layer][adjust_signal].normal_gain = new_value;
            READ_MOD_WRITE(*(BYTE *)(AfeGainInfoT3[adjust_signal].field_normal_gn.addr),
                                    (AfeGainInfoT3[adjust_signal].field_normal_gn.mask),
                                    (new_value << (AfeGainInfoT3[adjust_signal].field_normal_gn.start_bit)));
#if (CHIP_REV >= 0xC0)
            if (adjust_signal == DPD_GN)
            {
                switch (path_select1)
                {
                    case 0://DPDGN1~4
                        READ_MOD_WRITE(HAL_AFEREG_MAP->RF07,HAL_AFE_DPDGN1SEL|HAL_AFE_DPDGN2SEL,HAL_AFE_DPDGN1SEL|HAL_AFE_DPDGN2SEL);
                        READ_MOD_WRITE(HAL_AFEREG_MAP->RF09,HAL_AFE_DPDGN3SEL|HAL_AFE_DPDGN4SEL,HAL_AFE_DPDGN3SEL|HAL_AFE_DPDGN4SEL);
                        break;

                    case 1://DPDGN1
                        READ_MOD_WRITE(HAL_AFEREG_MAP->RF07,HAL_AFE_DPDGN1SEL|HAL_AFE_DPDGN2SEL,HAL_AFE_DPDGN1SEL);
                        READ_MOD_WRITE(HAL_AFEREG_MAP->RF09,HAL_AFE_DPDGN3SEL|HAL_AFE_DPDGN4SEL,0);
                        break;

                    case 2://DPDGN2
                        READ_MOD_WRITE(HAL_AFEREG_MAP->RF07,HAL_AFE_DPDGN1SEL|HAL_AFE_DPDGN2SEL,HAL_AFE_DPDGN2SEL);
                        READ_MOD_WRITE(HAL_AFEREG_MAP->RF09,HAL_AFE_DPDGN3SEL|HAL_AFE_DPDGN4SEL,0);
                        break;

                    case 3://DPDGN3
                        READ_MOD_WRITE(HAL_AFEREG_MAP->RF07,HAL_AFE_DPDGN1SEL|HAL_AFE_DPDGN2SEL,0);
                        READ_MOD_WRITE(HAL_AFEREG_MAP->RF09,HAL_AFE_DPDGN3SEL|HAL_AFE_DPDGN4SEL,HAL_AFE_DPDGN3SEL);
                        break;

                    case 4://DPDGN4
                        READ_MOD_WRITE(HAL_AFEREG_MAP->RF07,HAL_AFE_DPDGN1SEL|HAL_AFE_DPDGN2SEL,0);
                        READ_MOD_WRITE(HAL_AFEREG_MAP->RF09,HAL_AFE_DPDGN3SEL|HAL_AFE_DPDGN4SEL,HAL_AFE_DPDGN4SEL);
                        break;
                }
            }
            READ_MOD_WRITE(HAL_AFEREG_MAP->RF07,HAL_AFE_DPDGN1SEL|HAL_AFE_DPDGN2SEL,0);
            READ_MOD_WRITE(HAL_AFEREG_MAP->RF09,HAL_AFE_DPDGN3SEL|HAL_AFE_DPDGN4SEL,0);
#endif
            break;
    }
}

#if (MPPBG_SPPBG_CALIB != 0)
/*-----------------------------------------------------------------------------
 *
 * Module: calGetBalOffset()
 * Description: set balance gain, load and return related offset
 *
 *----------------------------------------------------------------------------*/
BYTE calGetBalOffset(BYTE adjust_signal, BYTE path_select, SBYTE new_value)
{
    BYTE  offset, orig_value;
    float offset0, offset1;

    if (adjust_signal == MPP_OS)
        orig_value = COEF_GetValue(AFE_MB16_MPPOBG);
    else
        orig_value = COEF_GetValue(AFE_SB16_SPP1BG);

    switch (path_select)
    {
        case NORMAL_OFFSET:
            offset0 = svoCalBalOfsT2Var[adjust_signal][0].normal_offset;
            offset1 = svoCalBalOfsT2Var[adjust_signal][1].normal_offset;
            break;
    }

    offset = calSBYTE2DacCount(float2short(linear_interpolation(CalculateRateFromXdB(P50dB, orig_value-MPPBG_SPP1BG_MAX_UNB), offset0,
                                                                CalculateRateFromXdB(P50dB, orig_value+MPPBG_SPP1BG_MAX_UNB), offset1,
                                                                CalculateRateFromXdB(P50dB,new_value))),AfeOffsetInfoT2[adjust_signal].total_Bits);

    return offset;
}// end of calGetBalOffset

/*-----------------------------------------------------------------------------
 *
 * Module: calSetBalGain()
 * Description: set balance gain, load and return related offset
 *
 *----------------------------------------------------------------------------*/
BYTE calSetBalGain(BYTE adjust_signal, SBYTE new_value)
{
    BYTE offset,signal_os;

    READ_MOD_WRITE(*(BYTE *)(AfeGainInfoT3[adjust_signal].field_normal_gn.addr),
                   (AfeGainInfoT3[adjust_signal].field_normal_gn.mask),
                   (new_value << (AfeGainInfoT3[adjust_signal].field_normal_gn.start_bit)));

    switch (adjust_signal)
    {
        case MPPB_GN:
            signal_os = MPP_OS;
            break;

        case SPP1B_GN:
            signal_os = SPP1_OS;
            break;

        default:
            return 0;
    }
    offset = calGetBalOffset(signal_os, NORMAL_OFFSET, new_value);
    WRITE_REG(*(BYTE *)(AfeOffsetInfoT2[signal_os].field_normal_ofs),offset);

    return offset;
}// end of calSetBalGain
#endif // #if (MPPBG_SPPBG_CALIB != 0)

/*-----------------------------------------------------------------------------
 *
 * Module: calSetMisc()
 * Description: save new value to svoCalMiscxVar
 *
 *----------------------------------------------------------------------------*/
void calSetMisc(BYTE layer, BYTE adjust_signal, LONG new_value)
{
    switch(adjust_signal)
    {
        case FEGN_DSP:
            svoCalMisc1Var[layer].feGainDSP = (USHORT)new_value;
            SendMsgCnB(SHOW_DEBUG_MSG,6,0x317507+Layer_Index,B2B(new_value),F4B((float)new_value/32768)); //Loop gain cal(L?) kfg=0X%04X (%1.2f)
            break;

        case TEGN_DSP:
            svoCalMisc1Var[layer].teGainDSP = (USHORT)new_value;
            SendMsgCnB(SHOW_DEBUG_MSG,6,0x317509+Layer_Index,B2B(new_value),F4B((float)new_value/32768)); //Loop gain cal(L?) ktg=0X%04X (%1.2f)
            break;

        case FE_SPT:
            svoCalMisc1Var[layer].feSetPoint = (SHORT)new_value;
            // FE set-point(L0) = %3d (%4dmV)
            SendMsgCnB(SHOW_DEBUG_MSG,4,0x31750D+Layer_Index,B2B(new_value),B2B((new_value * GPADC_IN_RANGE) >> GPADC_BITS));
            break;

        case TE_SPT:
            svoCalMisc1Var[layer].teSetPoint = (SHORT)new_value;
            SendMsgCnB(SHOW_DEBUG_MSG,6,0x31750B+Layer_Index,B2B(new_value),B2B((new_value * GPADC_IN_RANGE) >> GPADC_BITS)); //TE set-point(L?) = %3d (%4dmV)
            break;

        case TRO_OS:
            if (new_value > 127) new_value = 127;
            if (new_value < -128) new_value = -128;
            svoCalMisc2Var.troos = (SBYTE)new_value;
            SendMsgCn(SHOW_DEBUG_MSG,2,0x316683,A2B(new_value)); //TRO_OS
            break;

        case FOO_OS:
            if (new_value > FOO_OFFSET_LIMIT) new_value = FOO_OFFSET_LIMIT;
            if (new_value < -FOO_OFFSET_LIMIT) new_value = -FOO_OFFSET_LIMIT;
            svoCalMisc2Var.fooos = (SHORT)new_value;
            SendMsgCn(SHOW_DEBUG_MSG,2,0x316681,A2B(new_value)); //FOO_OS
            break;

        case PDIC_GN:
            svoCalMisc2Var.pdic_gn = (SHORT)new_value;
            SendMsgCnB(DEBUG_LASER_MSG|DEBUG_CALIB_RESULT_MSG,4,0x316687,F4B((float)new_value/10)); // PDIC_GN(dB)
            break;

        case LASER_PW:
            svoCalMisc2Var.laser_gn = (SHORT)new_value;
            SendMsgCnB(DEBUG_LASER_MSG|DEBUG_CALIB_RESULT_MSG,4,0x316688,F4B((float)new_value/10)); // LASER_PW(dB)
            break;
    }
}// end of calSetMisc

/*-----------------------------------------------------------------------------
 *
 * Module: calSetRfcosShift()
 * Description: save new value to svoRRFVar.rfcos_shift_cur[]
 *
 *----------------------------------------------------------------------------*/
void calSetRfcosShift(SBYTE new_value)
{
    BYTE    rfgain, total_bits;

    if (new_value < 0)
    {
        new_value = 0;//prevent negative value
    }
    else if (new_value > svoRRFVar.rfcos_shift_max)
    {
        // WARNING: svoRRFVar.rfcos_shift too large (%2d > %2d)
        SendMsgCn(SHOW_DEBUG_MSG,2,0x31060F,A1B(new_value),A1B(svoRRFVar.rfcos_shift_max));
        new_value = svoRRFVar.rfcos_shift_max;
    }
    svoRRFVar.rfcos_shift_cur[Layer_Index] = new_value;
    total_bits = (HAL_AFE_RFCOSP_MSB-HAL_AFE_RFCOSP_LSB+1);
    rfgain = READ_FIELD(HAL_AFE_RFGN);
    WRITE_FIELD(HAL_AFE_RFCOSP,   calSBYTE2DacCount(calDacCount2SBYTE(svoCalOfsT2Var[Layer_Index][RFCP_OS].normal_offset,total_bits) + new_value,total_bits));
    WRITE_FIELD(HAL_AFE_RFCOSPWLD,calSBYTE2DacCount(calDacCount2SBYTE(svoCalOfsT2Var[Layer_Index][RFCP_OS].write_offset, total_bits) + new_value,total_bits));
    WRITE_FIELD(HAL_AFE_RFCOSN,   calSBYTE2DacCount(calDacCount2SBYTE(svoCalOfsT2Var[Layer_Index][RFCN_OS].normal_offset,total_bits) - new_value,total_bits));
    WRITE_FIELD(HAL_AFE_RFCOSNWLD,calSBYTE2DacCount(calDacCount2SBYTE(svoCalOfsT2Var[Layer_Index][RFCN_OS].write_offset, total_bits) - new_value,total_bits));
    svoRRFVar.ofs_ldoff_cur = svoRRFVar.ofs_ldoff[rfgain] + (4 * svoRRFVar.rfcos_step_size[rfgain] * new_value + (svoRRFVar.step_size[0] / 2)) / svoRRFVar.step_size[0];
    SendMsgCn(DEBUG_CALIB_RESULT_MSG,3,0x31051E,Layer_Index,A1B(new_value),A2B(svoRRFVar.ofs_ldoff_cur));
}// end of calSetRfcosShift

#if (CALIBRATION_MANAGEMENT == 1)
/*-----------------------------------------------------------------------------
 * Module: check_signal_Limit()
 * Description: Check and limit the measured signal to prevent calibrated wrong
 * input:1.sinnal:FEBC_DSP/TILT_DAT
 *       2.inputData:Current measured Data
 * output:
 *       The limited result Data
 *----------------------------------------------------------------------------*/
SHORT check_signal_Limit(BYTE signal, SHORT inputData)
{
    BYTE  cur_zone;
    SBYTE zone;
    LONG up_limit,down_limit,zonediff;

    cur_zone = calRadius2Zone(Iram_stp_CurrentPosition);

    switch(signal)
    {
        case FEBC_DSP:
            //check the FEBC dac is over current zone limit?
            //to be sure FEBC dac is in the range opt_value +/- zonediff
            zonediff = FEBC_DIFF_PER_ZONE;
            if (svoCalFebcVar[Layer_Index][cur_zone].status & CAL_R_DONE)
            {
                up_limit = svoCalFebcVar[Layer_Index][cur_zone].opt_value + zonediff;
                down_limit = svoCalFebcVar[Layer_Index][cur_zone].opt_value - zonediff;
                if (inputData > up_limit)
                    inputData = up_limit;
                else if (inputData < down_limit)
                    inputData = down_limit;
            }

            //check the FEBC dac is over nearest zone's limit?
            for (zone = cur_zone - 1; zone >= 0; zone--)
            {
                if (svoCalFebcVar[Layer_Index][zone].status & CAL_R_DONE)
                    break;
            }
            if (zone >= 0)
            {
                up_limit = svoCalFebcVar[Layer_Index][zone].opt_value + (cur_zone - zone) * zonediff;
                down_limit = svoCalFebcVar[Layer_Index][zone].opt_value - (cur_zone - zone) * zonediff;
                if(inputData > up_limit)
                    inputData = up_limit;
                else if(inputData < down_limit)
                    inputData = down_limit;
            }

            //check the FEBC dac is over nearest zone's limit?
            for (zone = cur_zone + 1; zone < CAL_RADIUS_ZONE_NR; zone++)
            {
                if (svoCalFebcVar[Layer_Index][zone].status & CAL_R_DONE)
                    break;
            }
            if (zone < CAL_RADIUS_ZONE_NR)
            {
                up_limit = svoCalFebcVar[Layer_Index][zone].opt_value + (zone - cur_zone) * zonediff;
                down_limit = svoCalFebcVar[Layer_Index][zone].opt_value - (zone - cur_zone) * zonediff;
                if(inputData > up_limit)
                    inputData = up_limit;
                else if(inputData < down_limit)
                    inputData = down_limit;
            }
            break;

        case TILT_DAT:
            //check the TILT dac is over current zone limit?
            //to be sure TILT dac is in the range opt_value +/- zonediff
            zonediff = TILT_DIFF_PER_ZONE;
            if (svoCalTiltVar[Layer_Index][cur_zone].status & CAL_R_DONE)
            {
                up_limit = svoCalTiltVar[Layer_Index][cur_zone].opt_value + zonediff;
                down_limit = svoCalTiltVar[Layer_Index][cur_zone].opt_value - zonediff;
                if (inputData > up_limit)
                    inputData = up_limit;
                else if (inputData < down_limit)
                    inputData = down_limit;
            }

            //check the TILT dac is over nearest zone's limit?
            for (zone = cur_zone - 1; zone >= 0; zone--)
            {
                if (svoCalTiltVar[Layer_Index][zone].status & CAL_R_DONE)
                    break;
            }
            if (zone >= 0)
            {
                up_limit = svoCalTiltVar[Layer_Index][zone].opt_value + (cur_zone - zone) * zonediff;
                down_limit = svoCalTiltVar[Layer_Index][zone].opt_value - (cur_zone - zone) * zonediff;
                if(inputData > up_limit)
                    inputData = up_limit;
                else if(inputData < down_limit)
                    inputData = down_limit;
            }

            //check the TILT dac is over nearest zone's limit?
            for (zone = cur_zone + 1; zone < CAL_RADIUS_ZONE_NR; zone++)
            {
                if (svoCalTiltVar[Layer_Index][zone].status & CAL_R_DONE)
                    break;
            }
            if (zone < CAL_RADIUS_ZONE_NR)
            {
                up_limit = svoCalTiltVar[Layer_Index][zone].opt_value + (zone - cur_zone) * zonediff;
                down_limit = svoCalTiltVar[Layer_Index][zone].opt_value - (zone - cur_zone) * zonediff;
                if(inputData > up_limit)
                    inputData = up_limit;
                else if(inputData < down_limit)
                    inputData = down_limit;
            }
            break;

        default:
            break;
    }
    return(inputData);
}
#endif

/*-----------------------------------------------------------------------------
 *
 * Module: UpdateTiltVar()
 * Description: update value of svoCalTiltVar
 *
 *----------------------------------------------------------------------------*/
static void UpdateTiltVar(BYTE layer, BYTE cur_zone)
{
    SBYTE i,j;
    BYTE ops_l;

    ops_l = (layer == 0);//search the other layer

    //update uncalibrated zone in the front of cur_zone
    for (i = cur_zone - 1; i >= 0; i--)
    {
        if (svoCalTiltVar[layer][i].status & CAL_R_DONE)
        {
            svoCalVar.calDoneBits |= tilt_cal_end;
            for (j = i + 1; j < cur_zone; j++)
            {
                svoCalTiltVar[layer][j].opt_value = linear_interpolation(svoCalTiltVar[layer][i].sled_pos,svoCalTiltVar[layer][i].opt_value,
                                                                  svoCalTiltVar[layer][cur_zone].sled_pos,svoCalTiltVar[layer][cur_zone].opt_value,svoCalTiltVar[layer][j].sled_pos);
            }
            break;
        }
    }
    if (i == -1)//if no CAL_R_DONE flag be found behind cur_zone current layer ==> to search it at the other layer
    {
        for (i = cur_zone - 1; i >= 0; i--)
        {
            if (svoCalTiltVar[ops_l][i].status & CAL_R_DONE)//search it at the other layer
            {
                for (j = i + 1; j < cur_zone; j++)
                {
                    svoCalTiltVar[layer][j].opt_value = linear_interpolation(svoCalTiltVar[ops_l][i].sled_pos,svoCalTiltVar[ops_l][i].opt_value,
                                                                      svoCalTiltVar[layer][cur_zone].sled_pos,svoCalTiltVar[layer][cur_zone].opt_value,svoCalTiltVar[layer][j].sled_pos);
                }
                break;
            }
        }
        if (i == -1)//if no CAL_R_DONE flag be found  ==> copy the current zone optimum to the other zone
        {
            for (i = cur_zone - 1; i >= 0; i--)
            {
                svoCalTiltVar[layer][i].opt_value = svoCalTiltVar[layer][cur_zone].opt_value;
            }
        }
    }

    //update uncalibrated zone behind cur_zone
    for (i = cur_zone + 1; i < CAL_RADIUS_ZONE_NR; i++)
    {
        if (svoCalTiltVar[layer][i].status & CAL_R_DONE)
        {
            svoCalVar.calDoneBits |= tilt_cal_end;
            for (j = cur_zone + 1; j < i; j++)
            {
                svoCalTiltVar[layer][j].opt_value = linear_interpolation(svoCalTiltVar[layer][i].sled_pos,svoCalTiltVar[layer][i].opt_value,
                                            svoCalTiltVar[layer][cur_zone].sled_pos,svoCalTiltVar[layer][cur_zone].opt_value,svoCalTiltVar[layer][j].sled_pos);
            }
            break;
        }
    }
    if (i == CAL_RADIUS_ZONE_NR) //if no CAL_R_DONE flag be found behind cur_zone current layer ==> to search it flag at the other layer
    {
        for (i = cur_zone + 1; i < CAL_RADIUS_ZONE_NR; i++)//search it at the other layer
        {
            if (svoCalTiltVar[ops_l][i].status & CAL_R_DONE)
            {
                for (j = cur_zone + 1; j < i; j++)
                {
                    svoCalTiltVar[layer][j].opt_value = linear_interpolation(svoCalTiltVar[ops_l][i].sled_pos,svoCalTiltVar[ops_l][i].opt_value,
                                                svoCalTiltVar[layer][cur_zone].sled_pos,svoCalTiltVar[layer][cur_zone].opt_value,svoCalTiltVar[layer][j].sled_pos);
                }
                break;
            }
        }
        if (i == CAL_RADIUS_ZONE_NR)//if no CAL_R_DONE flag be found  ==> copy the current zone optimum to the other zone
        {
            for (i = cur_zone + 1; i < CAL_RADIUS_ZONE_NR; i++)//copy the current zone optimum to the other zone
            {
                svoCalTiltVar[layer][i].opt_value = svoCalTiltVar[layer][cur_zone].opt_value;
            }
        }
    }
}

/*-----------------------------------------------------------------------------
 *
 * Module: UpdateFebcVar()
 * Description: update value of svoCalFebcVar
 *
 *----------------------------------------------------------------------------*/
static void UpdateFebcVar(BYTE layer, BYTE cur_zone)
{
    SBYTE i,j;

    //update uncalibrated zone in the front of cur_zone
    for (i = cur_zone - 1; i >= 0; i--)
    {
        if (svoCalFebcVar[layer][i].status & CAL_R_DONE)
        {
            svoCalVar.calDoneBits |= febc_cal_end;
            for (j = i + 1; j < cur_zone; j++)
            {
                svoCalFebcVar[layer][j].opt_value = linear_interpolation(svoCalFebcVar[layer][i].sled_pos,svoCalFebcVar[layer][i].opt_value,
                                                                  svoCalFebcVar[layer][cur_zone].sled_pos,svoCalFebcVar[layer][cur_zone].opt_value,svoCalFebcVar[layer][j].sled_pos);
            }
            break;
        }
    }
    if (i == -1)
    {
        for (i = 0; i < cur_zone; i++)
        {
            svoCalFebcVar[layer][i].opt_value = svoCalFebcVar[layer][cur_zone].opt_value;
        }
    }

    //update uncalibrated zone behind cur_zone
    for (i = cur_zone + 1; i < CAL_RADIUS_ZONE_NR; i++)
    {
        if (svoCalFebcVar[layer][i].status & CAL_R_DONE)
        {
            svoCalVar.calDoneBits |= febc_cal_end;
            for (j = cur_zone + 1; j < i; j++)
            {
                svoCalFebcVar[layer][j].opt_value = linear_interpolation(svoCalFebcVar[layer][i].sled_pos,svoCalFebcVar[layer][i].opt_value,
                                            svoCalFebcVar[layer][cur_zone].sled_pos,svoCalFebcVar[layer][cur_zone].opt_value,svoCalFebcVar[layer][j].sled_pos);
            }
            break;
        }
    }
    if (i == CAL_RADIUS_ZONE_NR)
    {
        for (i = cur_zone + 1; i < CAL_RADIUS_ZONE_NR; i++)
        {
            svoCalFebcVar[layer][i].opt_value = svoCalFebcVar[layer][cur_zone].opt_value;
        }
    }
}

/*-----------------------------------------------------------------------------
 *
 * Module: calClrCalResult()
 * Description: clear svoCalxxxVar
 *
 *----------------------------------------------------------------------------*/
void calClrCalResult(BYTE adjust_signal, BYTE layer_1_only)
{
    SBYTE i;

    switch (adjust_signal)
    {
        case TILT_DAT:
            for (i = 0; i < CAL_RADIUS_ZONE_NR; i++)
            {
#if (FOOTILT_DIFFERENTIAL == 1)
                svoCalTiltVar[1][i].opt_value = 0;
#else
                svoCalTiltVar[1][i].opt_value = 0x200;
#endif
                if (i == 0)
                {
                    svoCalTiltVar[1][i].sled_pos = (UM_2_SLED_STEP(22280) + zone_end_radius[i]) / 2;
                }
                else if (i == CAL_RADIUS_ZONE_NR - 1)
                {
                    svoCalTiltVar[1][i].sled_pos = (zone_end_radius[i - 1] + UM_2_SLED_STEP(58000)) / 2;
                }
                else
                {
                    svoCalTiltVar[1][i].sled_pos = (zone_end_radius[i - 1] + zone_end_radius[i]) / 2;
                }
                svoCalTiltVar[1][i].cal_timer = 0;
                svoCalTiltVar[1][i].status    = (CAL_REQUIRED);
                if (!layer_1_only)
                {
                    svoCalTiltVar[0][i] = svoCalTiltVar[1][i];
                }
            }
            break;

        case FEBC_DSP:
            for (i = 0; i < CAL_RADIUS_ZONE_NR; i++)
            {
                svoCalFebcVar[1][i].opt_value = 0;
                if (i == 0)
                {
                    svoCalFebcVar[1][i].sled_pos = (UM_2_SLED_STEP(22280) + zone_end_radius[i]) / 2;
                }
                else if (i == CAL_RADIUS_ZONE_NR - 1)
                {
                    svoCalFebcVar[1][i].sled_pos = (zone_end_radius[i - 1] + UM_2_SLED_STEP(58000)) / 2;
                }
                else
                {
                    svoCalFebcVar[1][i].sled_pos = (zone_end_radius[i - 1] + zone_end_radius[i]) / 2;
                }
                svoCalFebcVar[1][i].cal_timer = 0;
                svoCalFebcVar[1][i].status    = (CAL_REQUIRED);
                if (!layer_1_only)
                {
                    svoCalFebcVar[0][i] = svoCalFebcVar[1][i];
                }
            }
            break;

#if COLLI_SWITCH ==1
        case COLLI_POS:
            for (i = 1; i >= 0; i--)
            {
    #if (COLLI_POS_RECOVERY == 1)
                svoCalColliVar[i].startup_value =
    #endif
                svoCalColliVar[i].opt_value = getInitColliPos(i, svoVar_discKindT.Bit.DiscStd);
    #if (COLLI_TEMP_SLOPE != 0)
       #if (PCB == 815)
                svoCalColliVar[i].temperature = (OPUBarcode.BD_INI_TEMP_X10 + 5) / 10;
       #else
                svoCalColliVar[i].temperature = 25;//OPUBarcode.TEMP_BD;
       #endif
    #endif
                svoCalColliVar[i].cal_timer = 0;
                svoCalColliVar[i].status    = (CAL_REQUIRED);
                if (layer_1_only)
                {
                    if ((svoCalColliVar[0].status & CAL_R_DONE) == CAL_R_DONE)
                    {//keep colli difference the same between layers if only one layer is calibrated
                        svoCalColliVar[1].opt_value = svoCalColliVar[0].opt_value + svoCalColliVar[1].opt_value - getInitColliPos(0, svoVar_discKindT.Bit.DiscStd);
    #if (COLLI_TEMP_SLOPE != 0)
                        svoCalColliVar[1].temperature = svoCalColliVar[0].temperature;
    #endif
                    }
                    break;
                }
            }
            break;
#endif
#if (DPD_COMPENSATE_OFFSET==1)
        case DPD_GNOS:
            for (i = 0; i < 21; i++)
            {
                svoCalDpdVar[1][0][i].status = (CAL_REQUIRED);
                svoCalDpdVar[1][0][i].dpd_offset = 0;
                svoCalDpdVar[1][0][i].dpd_gain = 0;
                svoCalDpdVar[1][1][i] = svoCalDpdVar[1][0][i];
                svoCalDpdVar[1][2][i] = svoCalDpdVar[1][0][i];
                if (!layer_1_only)
                {
                    svoCalDpdVar[0][0][i] = svoCalDpdVar[1][0][i];
                    svoCalDpdVar[0][1][i] = svoCalDpdVar[1][0][i];
                    svoCalDpdVar[0][2][i] = svoCalDpdVar[1][0][i];
                }
            }
            break;
#endif//#if (DPD_COMPENSATE_OFFSET==1)
    }
}// end of calClrCalResult

/*-----------------------------------------------------------------------------
 *
 * Module: calSetCalResult()
 * Description: save new value to svoCalxxxVar
 *
 *----------------------------------------------------------------------------*/
void calSetCalResult(BYTE layer, USHORT sled_pos, BYTE adjust_signal, LONG new_value, BYTE status)
{
    BYTE  cur_zone,i;
#if ((COLLI_SWITCH == 1) && (COLLI_POS_RECOVERY == 1))
    SHORT colli_drift;
#endif

    cur_zone = calRadius2Zone(sled_pos);
    switch(adjust_signal)
    {
        case TILT_DAT:
            if ((status & CAL_SKIP) == CAL_SKIP)
            {
                SendMsg80(SHOW_DEBUG_MSG,0x310667);
                StartTimer(&svoCalTiltVar[layer][cur_zone].cal_timer);
                svoCalTiltVar[layer][cur_zone].status |= status;
                return;
            }
            else if ((status & CAL_REQUIRED) == CAL_REQUIRED)
            {
                svoCalTiltVar[layer][cur_zone].status |= status;
                return;
            }
            // <L0> TILT (Zone = %2d, Value = %5d)
            SendMsgCn(SHOW_DEBUG_MSG,3,0x3382A2+layer,A1B(cur_zone),A2B(new_value));
            svoCalTiltVar[layer][cur_zone].opt_value = new_value;
            svoCalTiltVar[layer][cur_zone].sled_pos = sled_pos;
            StartTimer(&svoCalTiltVar[layer][cur_zone].cal_timer);
            svoCalTiltVar[layer][cur_zone].status &= ~(CAL_REQUIRED | CAL_UNRELIABLE | CAL_SKIP);
            svoCalTiltVar[layer][cur_zone].status |= status;
            UpdateTiltVar(layer, cur_zone);

            //update the other layer current zone optimum value ?
            if (svoVar_discKindT.Bit.Layers == eLayersDouble)
            {
                layer = (layer == 0);//search the other layer CAL_R_DONE flag

                //search in the front of cur_zone of the other layer
                for (i = 0; i <= cur_zone; i++)
                {
                    if ((svoCalTiltVar[layer][i].status & CAL_R_DONE) == CAL_R_DONE)
                    {
                        break;
                    }
                }

                if (i == (cur_zone + 1))//CAL_R_DONE flag not be found at (zone0 <--> current zone)of the other layer
                {
                    if ((i == CAL_RADIUS_ZONE_NR) || ((svoCalTiltVar[layer][i].status & CAL_R_DONE) != CAL_R_DONE))
                    {
                        i = CAL_RADIUS_ZONE_NR;//update the other layer current zone optimum value
                    }
                }
                else if ((cur_zone == 0) || ((svoCalTiltVar[layer][cur_zone - 1].status & CAL_R_DONE) != CAL_R_DONE))
                {
                    for (i = cur_zone; i < CAL_RADIUS_ZONE_NR; i++)//search behind the cur_zone of the other layer
                    {
                        if ((svoCalTiltVar[layer][i].status & CAL_R_DONE) == CAL_R_DONE)
                        {
                            break;
                        }
                    }
                }

                //if CAL_R_DONE flag is not be found ==> update the other layer current zone optimum value
                if (i == CAL_RADIUS_ZONE_NR)
                {
                    svoCalTiltVar[layer][cur_zone].opt_value = new_value;
                    svoCalTiltVar[layer][cur_zone].sled_pos = sled_pos;
                    UpdateTiltVar(layer, cur_zone);
                }
            }
            break;

        case FEBC_DSP:
            if ((status & CAL_SKIP) == CAL_SKIP)
            {
                SendMsg80(SHOW_DEBUG_MSG,0x310668);
                StartTimer(&svoCalFebcVar[layer][cur_zone].cal_timer);
                svoCalFebcVar[layer][cur_zone].status |= status;
                return;
            }
            else if ((status & CAL_REQUIRED) == CAL_REQUIRED)
            {
                svoCalFebcVar[layer][cur_zone].status |= status;
                return;
            }
            // <L0> FEBC (Zone = %2d, Value = %5d)
            SendMsgCn(SHOW_DEBUG_MSG,3,0x3382A0+layer,A1B(cur_zone),A2B(new_value));
            svoCalFebcVar[layer][cur_zone].opt_value = (SHORT)new_value;
            svoCalFebcVar[layer][cur_zone].sled_pos = sled_pos;
            StartTimer(&svoCalFebcVar[layer][cur_zone].cal_timer);
            svoCalFebcVar[layer][cur_zone].status &= ~(CAL_REQUIRED | CAL_UNRELIABLE | CAL_SKIP);
            svoCalFebcVar[layer][cur_zone].status |= status;
            UpdateFebcVar(layer, cur_zone);
            break;
#if (COLLI_SWITCH == 1)
        case COLLI_POS:
            if ((status & CAL_SKIP) == CAL_SKIP)
            {
                SendMsg80(SHOW_DEBUG_MSG,0x310669);
                StartTimer(&svoCalColliVar[layer].cal_timer);
                svoCalColliVar[layer].status |= status;
                return;
            }
            if (svoVar_discKindT.Bit.Layers == eLayersDouble)
            {
                i = (layer == 0);
                if ((svoCalColliVar[i].status & CAL_R_DONE) == 0)
                {//keep colli difference the same between layers if only one layer is calibrated
                    svoCalColliVar[i].opt_value = (SHORT)new_value - svoCalColliVar[layer].opt_value + svoCalColliVar[i].opt_value;
    #if (COLLI_TEMP_SLOPE != 0)
                    svoCalColliVar[i].temperature = OpuTemperature;
    #endif
                }
            }
            SendMsgCn(SHOW_DEBUG_MSG,2,0x3382E0+layer,A2B(new_value));
    #if (COLLI_POS_RECOVERY == 1)
            if ((svoCalColliVar[layer].status & CAL_R_DONE) == 0)
            {
                svoCalColliVar[layer].startup_value = (SHORT)new_value;
            }
            else
            {
        #if (COLLI_TEMP_SLOPE != 0)
                svoCalColliVar[layer].startup_value = (svoCalColliVar[layer].startup_value + (OpuTemperature - svoCalColliVar[layer].temperature) * COLLI_TEMP_SLOPE);
        #endif
                colli_drift = new_value - svoCalColliVar[layer].startup_value;
                colli_drift = ABS(colli_drift);
                if (colli_drift > COLLI_POS_TOLERANCE)
                {
                    SendMsgCn(SHOW_DEBUG_MSG,4,0x3382E2,A2B(svoCalColliVar[layer].startup_value),A2B(new_value));//COLLI drift detected, startup:%5d, now:%5d
                    new_value = svoCalColliVar[layer].startup_value;
                    svoColli_Homing();
                    while (svoStp_busy())
                    {
                        ExitProcess();
                    }
                    svoStp_JumpToPosition(COLLI_NORMAL_NO_REST_MODE,new_value);
                    while (svoStp_busy())
                    {
                        ExitProcess();
                    }
                    if (svoVar_discKindT.Bit.Layers == eLayersDouble)
                    {
                        i = (layer == 0);
                        svoCalColliVar[i].opt_value = svoCalColliVar[i].startup_value;
                    }
                }
            }
    #endif
            svoCalColliVar[layer].opt_value = (SHORT)new_value;
            StartTimer(&svoCalColliVar[layer].cal_timer);
            svoCalColliVar[layer].status &= ~(CAL_REQUIRED | CAL_UNRELIABLE | CAL_SKIP);
            svoCalColliVar[layer].status |= status;
    #if (COLLI_TEMP_SLOPE != 0)
            svoCalColliVar[layer].temperature = OpuTemperature;
    #endif
            break;
#endif
#if (DPD_COMPENSATE_OFFSET == 1)
        case DPD_GNOS:
            cur_zone = svoDpdReg2Bank[READ_FIELD(HAL_AFE_DPDHSISCL)];
            i = READ_FIELD(HAL_AFE_DPDHSFCH);
            if ((status & CAL_SKIP) == CAL_SKIP)
            {
                SendMsg80(SHOW_DEBUG_MSG,0x31066F);
                svoCalDpdVar[layer][cur_zone][i].status &= (~CAL_REQUIRED);
                return;
            }
            SendMsgCn(SHOW_DEBUG_MSG,5,0x3382EF,layer,cur_zone,i,A2B(new_value));
            svoCalDpdVar[layer][cur_zone][i].dpd_offset = (SBYTE)new_value;
            svoCalDpdVar[layer][cur_zone][i].dpd_gain = (SBYTE)(new_value >> 8);
            if (svoCalDpdVar[layer][cur_zone][i].dpd_gain < 0)
            {
                svoCalDpdVar[layer][cur_zone][i].dpd_gain = 0;
            }
            if (svoVar_discKindT.Bit.DiscStd == eDiscBD)
            {
                if (svoCalDpdVar[layer][cur_zone][i].dpd_gain > 3)
                {
                    svoCalDpdVar[layer][cur_zone][i].dpd_gain = 3;
                }
            }
            else
            {
                if (svoCalDpdVar[layer][cur_zone][i].dpd_gain > 6)
                {
                   svoCalDpdVar[layer][cur_zone][i].dpd_gain = 6;
                }
            }
            svoCalDpdVar[layer][cur_zone][i].status &= ~(CAL_REQUIRED | CAL_UNRELIABLE | CAL_SKIP);
            svoCalDpdVar[layer][cur_zone][i].status |= (CAL_R_DONE | status);
            if ((svoCalDpdVar[!layer][cur_zone][i].status & CAL_R_DONE) == 0)
            {
                svoCalDpdVar[!layer][cur_zone][i].dpd_offset = (SBYTE)new_value;
                if (svoCalDpdVar[layer][cur_zone][i].dpd_gain > 4)
                {
                    svoCalDpdVar[!layer][cur_zone][i].dpd_gain = 4;
                }
                else
                {
                    svoCalDpdVar[!layer][cur_zone][i].dpd_gain = svoCalDpdVar[layer][cur_zone][i].dpd_gain;
                }
            }
            break;
#endif
    }

    calUpdateRadius(Iram_stp_CurrentPosition, TRUE);
}// end of calSetCalResult

/*-----------------------------------------------------------------------------
 *
 * Module: calGetCalStatus()
 * Description: return the status of requested adaptive calibration status at given position.
 *
 *----------------------------------------------------------------------------*/
BYTE calGetCalStatus(BYTE layer, USHORT sled_pos, USHORT range, BYTE adjust_signal)
{
    BYTE cur_zone,temp_zone;
    BYTE status,temp_status;

    status = 0;
    cur_zone = calRadius2Zone(sled_pos);
    switch(adjust_signal)
    {
        /*case TE_RATIO://not implemented yet
            cur_zone = calRadius2Zone(sled_pos);
            break;*/

        case TILT_DAT:
            if (ReadTimer(&svoCalTiltVar[layer][cur_zone].cal_timer) > TILT_RECAL_TIME)
            {
                svoCalTiltVar[layer][cur_zone].status |= CAL_REQUIRED;
            }
            if (ReadTimer(&svoCalTiltVar[layer][cur_zone].cal_timer) > TILT_SKIP_TIME)
            {
                svoCalTiltVar[layer][cur_zone].status &= (~CAL_SKIP);
            }
            status = svoCalTiltVar[layer][cur_zone].status;
            for (temp_zone = 0; temp_zone < CAL_RADIUS_ZONE_NR; temp_zone++)
            {
                if ((temp_zone != cur_zone) && (ABS(svoCalTiltVar[layer][temp_zone].sled_pos - sled_pos) <= range))
                {
                    if (ReadTimer(&svoCalTiltVar[layer][temp_zone].cal_timer) > TILT_RECAL_TIME)
                    {
                        svoCalTiltVar[layer][temp_zone].status |= CAL_REQUIRED;
                    }
                    temp_status = svoCalTiltVar[layer][temp_zone].status;
                    if ((temp_status & (CAL_R_DONE|CAL_REQUIRED|CAL_SKIP)) == CAL_R_DONE)
                    {
                        if (((temp_status & CAL_UNRELIABLE) == 0) || ((status & (CAL_R_DONE|CAL_REQUIRED)) != CAL_R_DONE))
                        {
                            status = (status & CAL_SKIP) | temp_status;
                        }
                    }
                    break;
                }
            }
            break;

        case FEBC_DSP:
            if (ReadTimer(&svoCalFebcVar[layer][cur_zone].cal_timer) > FEBC_RECAL_TIME)
            {
                svoCalFebcVar[layer][cur_zone].status |= CAL_REQUIRED;
            }

            if (ReadTimer(&svoCalFebcVar[layer][cur_zone].cal_timer) > FEBC_SKIP_TIME)
            {
                svoCalFebcVar[layer][cur_zone].status &= (~CAL_SKIP);
            }
            status = svoCalFebcVar[layer][cur_zone].status;

            for (temp_zone = 0; temp_zone < CAL_RADIUS_ZONE_NR; temp_zone++)
            {
                if ((temp_zone != cur_zone) && (ABS(svoCalFebcVar[layer][temp_zone].sled_pos - sled_pos) <= range))
                {
                    if (ReadTimer(&svoCalFebcVar[layer][temp_zone].cal_timer) > FEBC_RECAL_TIME)
                    {
                        svoCalFebcVar[layer][temp_zone].status |= CAL_REQUIRED;
                    }

                    temp_status = svoCalFebcVar[layer][temp_zone].status;
                    if ((temp_status & (CAL_R_DONE|CAL_REQUIRED|CAL_SKIP)) == CAL_R_DONE)
                    {
                        if (((temp_status & CAL_UNRELIABLE) == 0) || ((status & (CAL_R_DONE|CAL_REQUIRED)) != CAL_R_DONE))
                        {
                            status = (status & CAL_SKIP) | temp_status;
                        }
                    }
                    break;
                }
            }
            break;

#if COLLI_SWITCH ==1
        case COLLI_POS:
            if (ReadTimer(&svoCalColliVar[layer].cal_timer) > COLLI_RECAL_TIME)
            {
                svoCalColliVar[layer].status |= CAL_REQUIRED;
            }

    #if (COLLI_TEMP_SLOPE != 0)
            if (ABS(svoCalColliVar[layer].temperature - OpuTemperature) > COLLI_CALI_TEMP_THRESHOLD)//allow recalibrate colli after temperature change for more than 8 degree
            {
                svoCalColliVar[layer].status |= CAL_REQUIRED;
            }
    #endif

            if (ReadTimer(&svoCalColliVar[layer].cal_timer) > COLLI_SKIP_TIME)
            {
                svoCalColliVar[layer].status &= (~CAL_SKIP);
            }
            status = svoCalColliVar[layer].status;
            break;
#endif

#if (DPD_COMPENSATE_OFFSET == 1)
        case DPD_GNOS:
            status = svoCalDpdVar[layer][svoDpdReg2Bank[DesriedDpdSetting.DpdBwBank]][DesriedDpdSetting.DpdBwFc].status;
            break;
#endif//#if (DPD_COMPENSATE_OFFSET==1)
    }
    return status;
}

/*-----------------------------------------------------------------------------
 *
 * Module: calGetCalRequire()
 * Description: return TRUE means a calibration at given position can do good.
 *
 *----------------------------------------------------------------------------*/
BOOL calGetCalRequired(BYTE layer, USHORT sled_pos, BYTE adjust_signal, BYTE mode)
{
    BOOL allowed;
    BYTE status;

    if(svoIramVar.emergencyStopFlag || EjectKeyInfo.PressedValid || TrayManagerInfo.Executing)
        return(FALSE);

#if (TILT_SWITCH == 1)
    if ((adjust_signal == TILT_DAT) || (adjust_signal == ITEM_ALL))
    {
        allowed = FALSE;
        //if (svoVar_discKindT.Bit.DiscStd != eDiscCD)
        {   //Tilt calibration is not allowed for CD disc.
            switch (mode)
            {
    #if (USE_REALTIME_JITTER == 1)
                case JITTER_CAL:
        #if (BD_ENABLE == 1)
                    if ((!DVDRAM) && (svo_BDR_RE_PICReading == CLEAR))
        #else
                    if (!DVDRAM)
        #endif
                    {
                        allowed = TRUE;
                    }
                    break;
    #endif
                case TE_P2P_CAL:
                    if (svoVar_TrackingMethod == TRK_METHOD_DPD)
                    {
                        break;
                    }
                case TEST_WRITE_MODE:
                case WRITE_MODE:
                    allowed = TRUE;
                    break;

                case RF_P2P_CAL:
                case READ_MODE:
    #if (BD_ENABLE == 1)
                    if (svo_BDR_RE_PICReading == SET)
                    {
                        break;
                    }
    #endif
                    allowed = TRUE;
                    break;

                default:
                    break;
            }
        }

        if (allowed == TRUE)
        {
            if ((mode == TE_P2P_CAL) || (mode == TEST_WRITE_MODE))
            {   //CAL_UNRELIABLE is acceptable
                status = calGetCalStatus(layer, sled_pos, UM_2_SLED_STEP(2600), TILT_DAT);
                if (((status & (CAL_R_DONE | CAL_REQUIRED)) != CAL_R_DONE) && ((status & CAL_SKIP) == 0) && (!((status & (CAL_R_DONE | CAL_UNRELIABLE)) == CAL_R_DONE)))
                {
                    return TRUE;
                }
            }
            else
            {
                status = calGetCalStatus(layer, sled_pos, UM_2_SLED_STEP(400), TILT_DAT);
                if (((status & (CAL_R_DONE | CAL_REQUIRED | CAL_UNRELIABLE)) != CAL_R_DONE) && ((status & CAL_SKIP) == 0))
                {
                    return TRUE;
                }
            }
        }
    }
#endif

    if ((adjust_signal == FEBC_DSP) || (adjust_signal == ITEM_ALL))
    {
        allowed = FALSE;
        //if (svoVar_discKindT.Bit.DiscStd != eDiscCD)
        {   //FEBC calibration is now allowed for all disc.
            switch (mode)
            {
#if (USE_REALTIME_JITTER == 1)
                case JITTER_CAL:
    #if (BD_ENABLE == 1)
                    if ((!DVDRAM) && (svo_BDR_RE_PICReading == CLEAR))
    #else
                    if (!DVDRAM)
    #endif
                    {
                        allowed = TRUE;
                    }
                    break;
#endif
                case TE_P2P_CAL:
                    if (svoVar_TrackingMethod == TRK_METHOD_DPD)
                    {
                        break;
                    }
                case TEST_WRITE_MODE:
                case WRITE_MODE:
                    allowed = TRUE;
                    break;

                case RF_P2P_CAL:
                case READ_MODE:
#if (BD_ENABLE == 1)
                    if (svo_BDR_RE_PICReading == SET)
                    {
                        break;
                    }
#endif
                    allowed = TRUE;
                    break;

                default:
                    break;
            }
        }

        if (allowed == TRUE)
        {
            status = calGetCalStatus(layer, sled_pos, UM_2_SLED_STEP(400), FEBC_DSP);
            if (mode != TEST_WRITE_MODE)
            {
                if (((status & (CAL_R_DONE | CAL_REQUIRED)) != CAL_R_DONE) && ((status & CAL_SKIP) == 0))
                {
                    return TRUE;
                }
            }
        }
    }
#if (COLLI_SWITCH == 1)
    if ((adjust_signal == COLLI_POS) || (adjust_signal == ITEM_ALL))
    {
        allowed = FALSE;
        if (svoVar_discKindT.Bit.DiscStd == eDiscBD)
        {   //SA calib is only for BD.
            switch (mode)
            {
                case TE_P2P_CAL:
                    if (svoVar_TrackingMethod == TRK_METHOD_DPD)
                    {
                        break;
                    }
                case WRITE_MODE:
                    allowed = TRUE;
                    break;

    #if (USE_REALTIME_JITTER == 1)
                case JITTER_CAL:
    #endif
                case RF_P2P_CAL:
                case READ_MODE:
    #if (BD_ENABLE == 1)
                    if (svo_BDR_RE_PICReading == SET)
                    {
                        break;
                    }
    #endif
                    allowed = TRUE;
                    break;

                default:
                    break;
            }
        }

        if (allowed == TRUE)
        {
            status = calGetCalStatus(layer, sled_pos, UM_2_SLED_STEP(400), COLLI_POS);
            if ((mode == TE_P2P_CAL) || (mode == TEST_WRITE_MODE))
            {
                if (((status & (CAL_R_DONE | CAL_REQUIRED)) != CAL_R_DONE) && ((status & CAL_SKIP) == 0) && (!((status & (CAL_R_DONE | CAL_UNRELIABLE)) == CAL_R_DONE)))
                {
                    return TRUE;
                }
            }
            else
            {
                if (((status & (CAL_R_DONE | CAL_REQUIRED | CAL_UNRELIABLE)) != CAL_R_DONE) && ((status & CAL_SKIP) == 0))
                {
                    return TRUE;
                }
            }
        }
    }
#endif

#if (DPD_COMPENSATE_OFFSET == 1)
    if ((adjust_signal == DPD_GNOS) || (adjust_signal == ITEM_ALL))
    {
        allowed = FALSE;

        if ((svoVar_TrackingMethod == TRK_METHOD_DPD) && (svoCalVar.calGainDoneBits & teg_end) && (!DVDRAM))
        {
            switch (mode)
            {
                case TE_P2P_CAL:
                case READ_MODE:
                    allowed = TRUE;
                    break;
            }
        }

    #if (1)
        if ((allowed == TRUE) && ((svoSpeedVar.SpdInRange == TRUE) || (svoMiscVar.trackSlipCount>5)))
    #else
        if (allowed == TRUE)
    #endif
        {
            status = calGetCalStatus(layer, sled_pos, UM_2_SLED_STEP(400), DPD_GNOS);
            if ((status & (CAL_R_DONE | CAL_REQUIRED)) != CAL_R_DONE)
            {
                return TRUE;
            }
        }
    }
#endif//#if (DPD_COMPENSATE_OFFSET==1)

    return FALSE;
}

#if (COLLI_SWITCH == 1)
/*-----------------------------------------------------------------------------
 *
 * Module: calGetOptColliPos()
 * Description: get temperature compensated optimal colli position
 *
 *----------------------------------------------------------------------------*/
SHORT calGetOptColliPos(BYTE layer)
{
    #if ((MONITOR_OPU_TEMPERATURE == 1) && (COLLI_TEMP_SLOPE != 0))
        return (svoCalColliVar[layer].opt_value + (OpuTemperature - svoCalColliVar[layer].temperature) * COLLI_TEMP_SLOPE);
    #else
		return (svoCalColliVar[layer].opt_value);
	#endif
}
#endif

/*-----------------------------------------------------------------------------
 *
 * Module: calPresetLayer1()
 * Description: preset some layer 1 parameters based on layer 0
 *
 *----------------------------------------------------------------------------*/
void calPresetLayer1(void)
{
    BYTE  i;
    SBYTE gn_diff_db;

    gn_diff_db = CalculateXdBFromRate(ONEdB, svoPrvtVar_avgSRFO[0], svoPrvtVar_avgSRFO[1]);

    if (svoVar_TrackingMethod == TRK_METHOD_DPD)
    {
        svoCalGnT1Var[1][TE_GN] = svoCalGnT1Var[0][TE_GN];
    }
    else
    {
        //preset L1 TE gain based on L0 result and SRFO measurement result
        calSetAfeGain(1,TE_GN,TYPE1,PATH_SELECT_ALL,0,svoCalGnT1Var[0][TE_GN].groove_gain[0] + gn_diff_db);
        calSetAfeGain(1,TE_GN,TYPE1,PATH_SELECT_ALL,1,svoCalGnT1Var[0][TE_GN].groove_gain[1] + gn_diff_db);
        calSetAfeGain(1,TE_GN,TYPE1,WRITE_GAIN,0,svoCalGnT1Var[0][TE_GN].write_gain + gn_diff_db);
    }

    //calculate L1 CE_GN based on SRFO
    calSetAfeGain(1,CE_GN,TYPE3,0,0,svoCalGnT3Var[0][CE_GN].normal_gain + gn_diff_db);
    //calculate L1 RF_GN based on SRFO
    calSetAfeGain(1,RF_GN,TYPE2,PATH_SELECT_ALL,0,HAL_AFE_CalcRFGain(HAL_AFE_GetRfGaindBX10(0xFF,svoCalGnT2Var[0][RF_GN].normal_gain) +
                                                                     CalculateXdBFromRate(P10dB, svoPrvtVar_avgSRFO[0], svoPrvtVar_avgSRFO[1]) +
                                                                     (svoCalGnT3Var[0][RFPGA_GN].normal_gain - 0x10) * 5));
    //calculate L1 rfcos_shift based on SRFO
    svoRRFVar.rfcos_shift_cur[1] = (svoRRFVar.rfcos_shift_cur[0] * svoPrvtVar_avgSRFO[1] + svoPrvtVar_avgSRFO[0] / 2) / svoPrvtVar_avgSRFO[0];

    for (i = (DPD_GN + 1); i < MAX_TYPE3_GN_NO; i++)//Note: skip CE_GN, RP_GN, and DPD_GN
    {
        svoCalGnT3Var[1][i]    = svoCalGnT3Var[0][i];
    }

    svoCalMisc1Var[1]          = svoCalMisc1Var[0];
}

/*-----------------------------------------------------------------------------
 *
 * Module: calUpdateLayer( BYTE tolayer)
 * Description: update layer dependent parameters
 *              tolayer: the target layer
 *
 *----------------------------------------------------------------------------*/
void calUpdateLayer(BYTE tolayer)
{
    ULONG layer;
    SBYTE rfaoc;

    if (tolayer == 1)
    {
        layer = 1;
    }
    else
    {
        layer = 0;
    }

    if (Layer_Index != layer)
    {
        //send_msg81(DEBUG_CALIB_RESULT_MSG,0x55000C,tolayer);
        Layer_Index = layer;

        WRITE_FIELD(HAL_AFE_BLANKFCSG,  svoCalGnT1Var[layer][FE_GN].groove_gain[RECDL]);
        WRITE_FIELD(HAL_AFE_FCSG,       svoCalGnT1Var[layer][FE_GN].groove_gain[RECDH]);
        WRITE_DSP_COEF(ktg,             svoCalMisc1Var[layer].teGainDSP);

        calSetAfeOffset(layer,MPP_OS,TYPE2,PATH_SELECT_ALL,0,calSetBalGain(MPPB_GN, svoCalGnT3Var[layer][MPPB_GN].normal_gain));
        calSetAfeOffset(layer,SPP1_OS,TYPE2,PATH_SELECT_ALL,0,calSetBalGain(SPP1B_GN, svoCalGnT3Var[layer][SPP1B_GN].normal_gain));

        WRITE_FIELD(HAL_AFE_TEBG,       svoCalGnT3Var[layer][TEB_GN].normal_gain);
        WRITE_FIELD(HAL_AFE_TRKG,       svoCalGnT1Var[layer][TE_GN].groove_gain[RECDH]);
        WRITE_FIELD(HAL_AFE_TRKO,       svoCalOfsT1Var[layer][TE_OS].groove_offset[RECDH]);
        WRITE_FIELD(HAL_AFE_BLANKTRKG,  svoCalGnT1Var[layer][TE_GN].groove_gain[RECDL]);
        WRITE_FIELD(HAL_AFE_BLANKTRKO,  svoCalOfsT1Var[layer][TE_OS].groove_offset[RECDL]);
        WRITE_DSP_COEF(kfg,             svoCalMisc1Var[layer].feGainDSP);
        //WRITE_FIELD(HAL_AFE_FE2OSA,     svoCalOfsT2Var[layer][FE2_OS].normal_offset);

#if (DPD_COMPENSATE_OFFSET==1)
        if (svoVar_TrackingMethod == TRK_METHOD_DPD)
        {
            calSetDPDTEOS(layer);
        }
#endif

        WRITE_FIELD(HAL_AFE_BSKG,       svoCalGnT2Var[layer][BS_GN].normal_gain);
        WRITE_FIELD(HAL_AFE_BLANKBSKG,  svoCalGnT2Var[layer][BS_GN].write_gain);
#if (AFE_NORMALIZER == 1)
        WRITE_FIELD(HAL_AFE_TARGETBS,   svoCalBsNorVar[layer][TAR_BS].normal_bs);
        WRITE_FIELD(HAL_AFE_BSTHRES,    svoCalBsNorVar[layer][THR_BS].normal_bs);
#endif
        WRITE_FIELD(HAL_AFE_RPOS,       svoCalOfsT3Var[layer][RP_OS].normal_offset);
        WRITE_FIELD(HAL_AFE_RP2OS,      svoCalOfsT3Var[layer][RP2_OS].normal_offset);
        WRITE_FIELD(HAL_AFE_RPBG,       svoCalGnT3Var[layer][RPB_GN].normal_gain);
        WRITE_FIELD(HAL_AFE_RP2GN,      svoCalGnT3Var[layer][RP_GN].normal_gain);

        WRITE_FIELD(HAL_AFE_CEGN,       svoCalGnT3Var[layer][CE_GN].normal_gain);
        WRITE_FIELD(HAL_AFE_CEOS,       svoCalOfsT3Var[layer][CE_OS].normal_offset);
        WRITE_FIELD(HAL_AFE_DPDGN,      svoCalGnT3Var[layer][DPD_GN].normal_gain);
#if (CHIP_REV >= 0xC0)
        //update DPDGN1~4
        READ_MOD_WRITE(HAL_AFEREG_MAP->RF07,HAL_AFE_DPDGN1SEL|HAL_AFE_DPDGN2SEL,HAL_AFE_DPDGN1SEL|HAL_AFE_DPDGN2SEL);
        READ_MOD_WRITE(HAL_AFEREG_MAP->RF09,HAL_AFE_DPDGN3SEL|HAL_AFE_DPDGN4SEL,HAL_AFE_DPDGN3SEL|HAL_AFE_DPDGN4SEL);
        READ_MOD_WRITE(HAL_AFEREG_MAP->RF07,HAL_AFE_DPDGN1SEL|HAL_AFE_DPDGN2SEL,0);
        READ_MOD_WRITE(HAL_AFEREG_MAP->RF09,HAL_AFE_DPDGN3SEL|HAL_AFE_DPDGN4SEL,0);
#endif
#if (EN_WOBBLE == 1)
        WRITE_FIELD(HAL_AFE_WDIFFGAIN,  svoCalGnT3Var[layer][WOB_GN].normal_gain);
#endif
        WRITE_FIELD(HAL_AFE_RFAGCREG,   svoCalGnT3Var[layer][RFPGA_GN].normal_gain);
#if (NEW_RFPOS_CALIB == 1)
    #if (NEW_RFPOS_CALIB_BRING_AGCVREF_TO_ZERO == 0)
        rfaoc = svoRRFVar.rfaoc_ref[layer] - svoRRFVar.rfagc_ref * 256 / svoRRFVar.step_size[svoCalGnT3Var[layer][RFPGA_GN].normal_gain];
        calSetAfeOffset(layer,RFPGA_OS,TYPE3,0,0,calSBYTE2DacCount(rfaoc,HAL_AFE_RFAOCREG_MSB-HAL_AFE_RFAOCREG_LSB+1));
    #else
        WRITE_FIELD(HAL_AFE_RFAOCREG,   svoCalOfsT3Var[layer][RFPGA_OS].normal_offset);
        rfaoc = calDacCount2SBYTE(svoCalOfsT3Var[layer][RFPGA_OS].normal_offset,HAL_AFE_RFAOCREG_MSB-HAL_AFE_RFAOCREG_LSB+1);
    #endif
#else
        WRITE_FIELD(HAL_AFE_RFAOCREG,   svoCalOfsT3Var[layer][RFPGA_OS].normal_offset);
        rfaoc = calDacCount2SBYTE(svoCalOfsT3Var[layer][RFPGA_OS].normal_offset,HAL_AFE_RFAOCREG_MSB-HAL_AFE_RFAOCREG_LSB+1);
#endif
        WRITE_FIELD(HAL_AFE_RFGN,       svoCalGnT2Var[layer][RF_GN].normal_gain);
        calSetRfcosShift(svoRRFVar.rfcos_shift_cur[layer]);
        //HAL_DFE_SetBkDefectThr(COEF_GetValue(DFE_CALC_Target_pp),(svoRRFVar.modulation[Layer_Index] == 0? (BYTE)COEF_GetValue(DFE_CALC_M) : svoRRFVar.modulation[Layer_Index]),servo_debug[2],servo_debug[2],servo_debug[2],servo_debug[2]);
        if (svoCalVar.calGainDoneBits & dif_RF_end)
        {
            HAL_DFE_SetAgcAocBoundary(COEF_GetValue(DFE_CALC_Coef_Max_dyn_gain), COEF_GetValue(DFE_CALC_Coef_Min_dyn_gain),
                                      COEF_GetValue(DFE_CALC_Coef_Max_dyn_Offset), COEF_GetValue(DFE_CALC_Coef_Min_dyn_Offset));  /* Set RF AGC/AOC Boundaries*/

            HAL_DFE_SetRfAocCalOffset(rfaoc);
            HAL_DFE_SetRfAgcCalGain(svoCalGnT3Var[layer][RFPGA_GN].normal_gain);
        }
        else
        {
            HAL_DFE_SetAgcAocBoundary(0xFF, 0xFF, 0xFF, 0xFF);  /* Set RF AGC/AOC to max Boundaries, full range */
        }

#if (COLLI_SWITCH == 1)
        if (svoVar_discKindT.Bit.DiscStd == eDiscBD)
        {
            svoStp_JumpToPosition(COLLI_NORMAL_NO_REST_MODE, calGetOptColliPos(layer));
    #if (OPU == BDPL2)
            HAL_LASC_SetAPCTarget(calGetOptColliPos(layer));
    #endif
            while (svoStp_busy())
            {
                ExitProcess();
            }
        }
#endif
        calUpdateRadius(Iram_stp_CurrentPosition, TRUE);

        if (((svoVar_discKindT.Bit.DiscStd == eDiscDVD) && (pdbGetDVDTrackPath() == DVDHD_OTP_DISC))
#if (ENABLE_HDDVD == 1)
        || ((svoVar_discKindT.Bit.DiscStd == eDiscHD) && (pdbGetHDTrackPath() == DVDHD_OTP_DISC))
#endif
#if (BD_ENABLE == 1)
        || (svoVar_discKindT.Bit.DiscStd == eDiscBD)
#endif
        )
        {
            svoStp_SetStepThreshold((layer == 0)? FORW : BACK);
        }
    }
}

/*-----------------------------------------------------------------------------
 *
 * Module: calUpdateRadius()
 * Description: Update radius dependent parameters based on given sledge position
 *
 *----------------------------------------------------------------------------*/
void calUpdateRadius(USHORT stp_position, BOOL force_update)
{
    BYTE    i;
    SHORT  NewValue;
    static SHORT  preFEBCValue = 0;
    static USHORT pre_stp_position = 0;
    static USHORT pre_stp_position_sp = 0;

    if ((ABS(stp_position - pre_stp_position_sp) > UM_2_SLED_STEP(15)))
    {
        if (svoSpeedVar.SpdMode != SPD_MODE_CAV)
        {
            spdUpdateRPM(stp_position);
        }

        if ((svoSpeedVar.SpdMode != SPD_MODE_CLV) || (svoSpeedVar.SpdInRange == FALSE) || (svoSpeedVar.SpdHold))
        {
            svoSpeedVar.SpdOverspd = spdGetOverSpdX100(svoSpeedVar.SpdInRange? svoSpeedVar.TargetRpm : svoSpeedVar.CurrentRpm,stp_position);
            ChkUpdateReadFilters();
        }

        if (svoStp_busy() && (ABS(stp_position - pre_stp_position_sp) > UM_2_SLED_STEP(300)))
        {
            svoSpeedVar.SpdPresetNeeded = TRUE;
        }
        pre_stp_position_sp = stp_position;
    }

    if (((ABS(stp_position - pre_stp_position) > UM_2_SLED_STEP(100))) || force_update)
    {
        pre_stp_position = stp_position;
#if (TILT_SWITCH == 1)
        if (((svoCalVar.calDoneBits & tilt_cal_end) == tilt_cal_end) || force_update)
        {
            if (stp_position <= svoCalTiltVar[Layer_Index][0].sled_pos)
            {
                i = 0;
                NewValue = svoCalTiltVar[Layer_Index][i].opt_value;
            }
            else if (stp_position >= svoCalTiltVar[Layer_Index][CAL_RADIUS_ZONE_NR - 1].sled_pos)
            {
                i = CAL_RADIUS_ZONE_NR - 1;
                NewValue = svoCalTiltVar[Layer_Index][i].opt_value;
            }
            else
            {
                for(i=1;i<CAL_RADIUS_ZONE_NR - 1;i++)
                {
                    if (stp_position < svoCalTiltVar[Layer_Index][i].sled_pos)
                    {
                        break;
                    }
                }
                NewValue = svoCalTiltVar[Layer_Index][i-1].opt_value + RoundDivide((LONG)(svoCalTiltVar[Layer_Index][i].opt_value - svoCalTiltVar[Layer_Index][i-1].opt_value) * (stp_position - svoCalTiltVar[Layer_Index][i-1].sled_pos),
                                                                     svoCalTiltVar[Layer_Index][i].sled_pos - svoCalTiltVar[Layer_Index][i-1].sled_pos);
            }

            if ((svoPreTiltValue != NewValue) || force_update)
            {
                apply_TILT(NewValue);
                svoPreTiltValue = NewValue;
                SendMsgCn(DEBUG_DVD_WRITE_MSG,4,0x380106,A2B(stp_position),A2B(NewValue));
            }
        }
#endif
        if (((svoCalVar.calDoneBits & febc_cal_end) == febc_cal_end) || force_update)
        {
            if (stp_position <= svoCalFebcVar[Layer_Index][0].sled_pos)
            {
                i = 0;
                NewValue = svoCalFebcVar[Layer_Index][i].opt_value;
            }
            else if (stp_position >= svoCalFebcVar[Layer_Index][CAL_RADIUS_ZONE_NR - 1].sled_pos)
            {
                i = CAL_RADIUS_ZONE_NR - 1;
                NewValue = svoCalFebcVar[Layer_Index][i].opt_value;
            }
            else
            {
                for (i=1;i<CAL_RADIUS_ZONE_NR - 1;i++)
                {
                    if (stp_position < svoCalFebcVar[Layer_Index][i].sled_pos)
                    {
                        break;
                    }
                }
                NewValue = svoCalFebcVar[Layer_Index][i-1].opt_value + RoundDivide((LONG)(svoCalFebcVar[Layer_Index][i].opt_value - svoCalFebcVar[Layer_Index][i-1].opt_value) * (stp_position - svoCalFebcVar[Layer_Index][i-1].sled_pos),
                                                                     svoCalFebcVar[Layer_Index][i].sled_pos - svoCalFebcVar[Layer_Index][i-1].sled_pos);
            }

            if ((preFEBCValue != NewValue) || force_update)
            {
                apply_FEBC(NewValue);
                preFEBCValue = NewValue;
                SendMsgCn(DEBUG_DVD_WRITE_MSG,4,0x380107,A2B(stp_position),A2B(NewValue));
            }
        }
#if (BD_ENABLE == 1)
        if (svoVar_discKindT.Bit.DiscStd == eDiscBD) //&& (svoVar_discKindT.Bit.Layers == eLayersDouble))
        {

            if (svoCalMisc1Var[Layer_Index].teSetPoint != 0)
                {
                    calSetMisc(Layer_Index, TE_SPT, 0);
                    apply_TESPT(svoCalMisc1Var[Layer_Index].teSetPoint);
                    SendMsg80(SHOW_DEBUG_MSG,0x317520); // ReStore TE offset
                }

            if (READ_DSP_COEF(ktg)!= svoCalMisc1Var[Layer_Index].teGainDSP)
                {
#if (DEFECT_LENGTH_MEASURE == 1)
                     if (svoDfMeasVar.defect_length > NO_SPD_UP_DEFECT_LENGTH)
                     {
                                                               // Don't  restore ktg
                     }
                     else
#endif
                     {

                         SendMsg80(SHOW_DEBUG_MSG,0x317519); // ReStore ktg1
                         WRITE_DSP_COEF(ktg, (svoCalMisc1Var[Layer_Index].teGainDSP));

                     }
                }

            if (READ_DSP_COEF(kfg) != svoCalMisc1Var[Layer_Index].feGainDSP)
                {
#if (DEFECT_LENGTH_MEASURE == 1)
                    if (svoDfMeasVar.defect_length > NO_SPD_UP_DEFECT_LENGTH)
                    {
                                                              // Don't  restore kfg
                    }
                    else
#endif
                    {
                        SendMsg80(SHOW_DEBUG_MSG,0x31751A); // Restore kfg
                        WRITE_DSP_COEF(kfg, svoCalMisc1Var[Layer_Index].feGainDSP);
                    }
                }   //restore origninal EQ or loop gain value only for BDROM DL



        }

#endif
    }
}// end of calUpdateRadius

#if (MSG_OUT_MODE != MSG_DISABLE)
/*-----------------------------------------------------------------------------
 *
 * Module: calUartPrintOffsetSignal()
 * Description:
 *
 *----------------------------------------------------------------------------*/
void calUartPrintOffsetSignal(BYTE adjust_signal, BYTE type)
{
    if (type == TYPE1)
    {
        switch(adjust_signal)
        {
            case FE_OS:
                SendMsg80(DEBUG_AUTO_TUNING_MSG,0x31656A);
                break;

            case TE_OS:
                SendMsg80(DEBUG_AUTO_TUNING_MSG,0x31656B);
                break;

            default:
                break;
        }
    }
    else if(type == TYPE2)
    {
        switch(adjust_signal)
        {
            case MPP_OS:
                SendMsg80(DEBUG_AUTO_TUNING_MSG,0x316500);
                break;

            case SPP1_OS:
                SendMsg80(DEBUG_AUTO_TUNING_MSG,0x316540);
                break;

            case FEO_OS:
                SendMsg80(DEBUG_AUTO_TUNING_MSG,0x316520);
                break;

            case SPP2_OS:
                SendMsg80(DEBUG_AUTO_TUNING_MSG,0x316550);
                break;

            case SBAD_OS:
                SendMsg80(DEBUG_AUTO_TUNING_MSG,0x316570);
                break;

            case SRF_OS:
                SendMsg80(DEBUG_AUTO_TUNING_MSG,0x316590);
                break;

           case FE2_OS:
                SendMsg80(DEBUG_AUTO_TUNING_MSG,0x316530);
                break;

           case TE2_OS:
                SendMsg80(DEBUG_AUTO_TUNING_MSG,0x316560);
                break;

            case BS2_OS:
                SendMsg80(DEBUG_AUTO_TUNING_MSG,0x316580);
                break;

            case RADO_OS:
                SendMsg80(DEBUG_AUTO_TUNING_MSG,0x3165B0);
                break;

            case RBCO_OS:
                SendMsg80(DEBUG_AUTO_TUNING_MSG,0x3165B1);
                break;

            case MBVREF_OS:
                SendMsg80(DEBUG_AUTO_TUNING_MSG,0x3165C1);
                break;

            case SBVREF_OS:
                SendMsg80(DEBUG_AUTO_TUNING_MSG,0x3165C2);
                break;

            case BS_OS:
                SendMsg80(DEBUG_AUTO_TUNING_MSG,0x3165C3);
                break;

//            case WRFO_OS:
//                send_msg80(DEBUG_AUTO_TUNING_MSG,0x5165A1);
//                break;

            default:
                break;
        }
    }
    else if(type == TYPE3)
    {
        switch(adjust_signal)
        {
            case CE_OS:
                SendMsg80(DEBUG_AUTO_TUNING_MSG,0x3165E5);
                break;

            case PHS_OS:
                SendMsg80(DEBUG_AUTO_TUNING_MSG,0x3165E6);
                break;

            case BHS_OS:
                SendMsg80(DEBUG_AUTO_TUNING_MSG,0x3165E7);
                break;

            case RP_OS:
                SendMsg80(DEBUG_AUTO_TUNING_MSG,0x3165E8);
                break;

            case RP2_OS:
                SendMsg80(DEBUG_AUTO_TUNING_MSG,0x3165E9);
                break;

            case RFP_OS:
                SendMsg80(DEBUG_AUTO_TUNING_MSG,0x3165EB);
                break;

            case VREF_OS:
                SendMsg80(DEBUG_AUTO_TUNING_MSG,0x3165EC);
                break;

            default:
                break;
        }
    }

    else
    {
        switch(adjust_signal)
        {
#if (CHIP_REV < 0xC1)
            case DPDGM_OS:
                SendMsg80(DEBUG_AUTO_TUNING_MSG,0x3165EF);
                break;
#endif
        }
    }
}
#endif // MSG_OUT_MODE

// To save space of stack, move some local variables here.
struct svoCalOfsT1VarT tempCalOfsT1Var;
struct svoCalOfsT2VarT tempCalOfsT2Var;
struct svoCalOfsT3VarT tempCalOfsT3Var;
/*-----------------------------------------------------------------------------
 *
 * Module: calCalibrateOffset()
 * Description: calibrte offset and store the result into corresponding variable
 *
 * DAC          :   0xFF                            0x80/0x7F                                0x00
 * offset value :   (more negative)               (zero)/(more positive)                    (zero)
 *                  |<--------( - offset value)-------->|<--------( + offset value)---------->|
 *
 *----------------------------------------------------------------------------*/
errcode_t calCalibrateOffset(BYTE    adjust_signal,       // adjusted signal
                             BYTE    type,                // offset type
                             BYTE    path_selected,       // path switched by WLDON/RECD/NORMAL
                             SHORT   target_mv,           // unit: mV
                             BOOL    polarity,            // polarity between offset & signal
                             BYTE    start_from_cur)      //'0' : offset cali. from 0 DAC ; '1' : offset cali. from current offset DAC
{
    uchar_return_t  next_off_ret;
    errcode_t       ErrCode;
    ad_dsp_t signal_ret;
    SHORT middle_mv;
    SHORT move_count,ref_move_cnt;
    SHORT sCurrentError,sPreviousError;
    float float_k;
    BYTE  bCurOffsetCnt,bPreOffsetCnt;
    BYTE  use_Bits,orig_LPF;
    BYTE  bCurrentLevel,bPreviousLevel;
    BYTE  i,phase,recd_status,monitor_signal;
    BOOL  only_positive;

#if (MSG_OUT_MODE != MSG_DISABLE)
    calUartPrintOffsetSignal(adjust_signal,type);
#endif
    SendMsgCn(DEBUG_AUTO_TUNING_MSG,2,0x316401,A2B(target_mv)); //Return_mV

    ErrCode = eNOERR;
    only_positive = FALSE;

    /*--- retrieve AFE OFFSET information -----------------*/
    recd_status = RECDH;

    // store the original offset value for each offset register
    switch (type)
    {
        case TYPE1:
            tempCalOfsT1Var.write_offset = READ_REG(*(vBYTE *)(AfeOffsetInfoT1[adjust_signal].field_write_ofs));
#if (CHIP_REV >= 0xC0)
            tempCalOfsT1Var.land_write_offset = READ_REG(*(vBYTE *)(AfeOffsetInfoT1[adjust_signal].field_land_write_ofs));
#endif
            tempCalOfsT1Var.groove_offset[RECDL] = READ_REG(*(vBYTE *)(AfeOffsetInfoT1[adjust_signal].field_groove_ofs[RECDL]));
            tempCalOfsT1Var.groove_offset[RECDH] = READ_REG(*(vBYTE *)(AfeOffsetInfoT1[adjust_signal].field_groove_ofs[RECDH]));
            tempCalOfsT1Var.land_offset[RECDL] = READ_REG(*(vBYTE *)(AfeOffsetInfoT1[adjust_signal].field_land_ofs[RECDL]));
            tempCalOfsT1Var.land_offset[RECDH] = READ_REG(*(vBYTE *)(AfeOffsetInfoT1[adjust_signal].field_land_ofs[RECDH]));
            if(path_selected == WRITE_OFFSET)
            {
                last_cal_result = tempCalOfsT1Var.write_offset;
            }
            else //NORMAL_OFFSET
            {
                if (path_selected == GROOVE_OFFSET)
                {
                    last_cal_result = tempCalOfsT1Var.groove_offset[recd_status];
                }
                else    //LAND_OFFSET
                {
                    last_cal_result = tempCalOfsT1Var.land_offset[recd_status];
                }
            }
            use_Bits = AfeOffsetInfoT1[adjust_signal].total_Bits;
            break;

        case TYPE2:
            tempCalOfsT2Var.write_offset = READ_REG(*(vBYTE *)(AfeOffsetInfoT2[adjust_signal].field_write_ofs));
            tempCalOfsT2Var.normal_offset = READ_REG(*(vBYTE *)(AfeOffsetInfoT2[adjust_signal].field_normal_ofs));
            if(path_selected == WRITE_OFFSET)
            {
                last_cal_result = tempCalOfsT2Var.write_offset;
            }
            else
            {
                last_cal_result = tempCalOfsT2Var.normal_offset;
            }
            use_Bits = AfeOffsetInfoT2[adjust_signal].total_Bits;
            break;

        case TYPE3:
            tempCalOfsT3Var.normal_offset = READ_REG(*(vBYTE *)(AfeOffsetInfoT3[adjust_signal].field_normal_ofs));
            last_cal_result = tempCalOfsT3Var.normal_offset;
            use_Bits = AfeOffsetInfoT3[adjust_signal].total_Bits;
            break;

        case TYPE4:
            tempCalOfsT3Var.normal_offset = (READ_REG(*(vBYTE *)(AfeOffsetInfoT4[adjust_signal].field_normal_ofs.addr)) & AfeOffsetInfoT4[adjust_signal].field_normal_ofs.mask) >> AfeOffsetInfoT4[adjust_signal].field_normal_ofs.start_bit;
            last_cal_result = tempCalOfsT3Var.normal_offset;
            use_Bits = AfeOffsetInfoT4[adjust_signal].total_Bits;
            if (
#if (CHIP_REV < 0xC1)
               (adjust_signal == DPDGM_OS) ||
#endif
               (adjust_signal == APC_OS))
            {
                only_positive = TRUE;
                use_Bits++;
            }
            break;
    }
    ref_move_cnt = ((use_Bits <= 5)? 4 : (4 << (use_Bits - 5)));

    /*------------- program AFE MUX OUT  --------------------*/
    monitor_signal = afeSetCalibMUX(type,adjust_signal);

    //set highest LPF to prevent slow respose, change polarity when signal is inverted.
    switch (monitor_signal)
    {
        case ADC_TE:
            orig_LPF = READ_REG(HAL_AFEREG_MAP->FCTS38) & (HAL_AFE_TEFCSFSEL_MASK | HAL_AFE_TELPSET_MASK);
        #if (CHIP_REV >= 0xC1)
            READ_MOD_WRITE(HAL_AFEREG_MAP->FCTS38,(HAL_AFE_TEFCSFSEL_MASK | HAL_AFE_TELPSET_MASK),(halAFE_TELPF_300kHz << HAL_AFE_TEFCSFSEL_LSB) | HAL_AFE_TELPSET);
        #else
            READ_MOD_WRITE(HAL_AFEREG_MAP->FCTS38,(HAL_AFE_TEFCSFSEL_MASK | HAL_AFE_TELPSET_MASK),(halAFE_TELPF_330kHz << HAL_AFE_TEFCSFSEL_LSB) | HAL_AFE_TELPSET);
        #endif
            if (READ_FIELD(HAL_AFE_TEINV))
            {
                polarity = (!polarity);
            }
            break;

        case ADC_FE:
            orig_LPF = READ_REG(HAL_AFEREG_MAP->FCTS32) & (HAL_AFE_FEFCSFSEL_MASK | HAL_AFE_FELPSET_MASK);
        #if (CHIP_REV >= 0xC1)
            READ_MOD_WRITE(HAL_AFEREG_MAP->FCTS32,(HAL_AFE_FEFCSFSEL_MASK | HAL_AFE_FELPSET_MASK),(halAFE_FELPF_300kHz << HAL_AFE_FEFCSFSEL_LSB) | HAL_AFE_FELPSET);
        #else
            READ_MOD_WRITE(HAL_AFEREG_MAP->FCTS32,(HAL_AFE_FEFCSFSEL_MASK | HAL_AFE_FELPSET_MASK),(halAFE_FELPF_330kHz << HAL_AFE_FEFCSFSEL_LSB) | HAL_AFE_FELPSET);
        #endif
            if (READ_FIELD(HAL_AFE_FEINV))
            {
                polarity = (!polarity);
            }
            break;

        case ADC_CE:
            orig_LPF = READ_REG(HAL_AFEREG_MAP->FCTS36) & HAL_AFE_CEFCSFSEL_MASK;
#if (CHIP_REV < 0xC0)
            READ_MOD_WRITE(HAL_AFEREG_MAP->FCTS36,HAL_AFE_CEFCSFSEL_MASK,(halAFE_CELPF_50kHz << HAL_AFE_CEFCSFSEL_LSB));
#else
            READ_MOD_WRITE(HAL_AFEREG_MAP->FCTS36,HAL_AFE_CEFCSFSEL_MASK,(halAFE_CELPF_300kHz << HAL_AFE_CEFCSFSEL_LSB));
#endif
            if (READ_FIELD(HAL_AFE_CEINV))
            {
                polarity = (!polarity);
            }
            break;

        case ADC_BS:
            orig_LPF = READ_REG(HAL_AFEREG_MAP->BSRP09) & (HAL_AFE_BSLP_MASK | HAL_AFE_BSLPSET_MASK);
            #if( CHIP_REV <= 0xB3)
            READ_MOD_WRITE(HAL_AFEREG_MAP->BSRP09,(HAL_AFE_BSLP_MASK | HAL_AFE_BSLPSET_MASK),(halAFE_BSLPF_330kHz << HAL_AFE_BSLP_LSB) | HAL_AFE_BSLPSET);
            #else
            READ_MOD_WRITE(HAL_AFEREG_MAP->BSRP09,(HAL_AFE_BSLP_MASK | HAL_AFE_BSLPSET_MASK),(halAFE_BSLPF_300kHz << HAL_AFE_BSLP_LSB) | HAL_AFE_BSLPSET);
            #endif
            if (READ_FIELD(HAL_AFE_BSINV))
            {
                polarity = (!polarity);
            }
            break;

        case ADC_RP:
            orig_LPF = READ_REG(HAL_AFEREG_MAP->BSRP14) & (HAL_AFE_RPLP_MASK | HAL_AFE_RPLPSET_MASK);
            #if( CHIP_REV <= 0xB3)
            READ_MOD_WRITE(HAL_AFEREG_MAP->BSRP14,(HAL_AFE_RPLP_MASK | HAL_AFE_RPLPSET_MASK),(halAFE_RPLPF_330kHz << HAL_AFE_RPLP_LSB) | HAL_AFE_RPLPSET);
            #else
            READ_MOD_WRITE(HAL_AFEREG_MAP->BSRP14,(HAL_AFE_RPLP_MASK | HAL_AFE_RPLPSET_MASK),(halAFE_RPLPF_300kHz << HAL_AFE_RPLP_LSB) | HAL_AFE_RPLPSET);
            #endif
            if (READ_FIELD(HAL_AFE_RPINV))
            {
                polarity = (!polarity);
            }
            break;
    }

    phase = GET_FIRST_SAMPLE;

    if (start_from_cur == CALIB_FROM_CURRENT)
    {
        bCurOffsetCnt = last_cal_result;
    }
    else
    {
        bCurOffsetCnt = 0;
    }
    calSaveAllAfeOffset(adjust_signal,type,bCurOffsetCnt); //fill all related register with same value

    bPreviousLevel = SIGNAL_UNKNOW;

    for (i = 0; i < MAX_OFFSET_LOOP; i++)
    {
        // The 1st rough-search, DAC Count = %d
        SendMsgCnB(DEBUG_AUTO_TUNING_MSG,1,0x3165D0+phase,B1B(calDacCount2SBYTE(bCurOffsetCnt,use_Bits)));
        Delay100uS(10);
        switch (monitor_signal)
        {
            case ADC_SPECIAL_RFDFE:
                signal_ret = readADCmVPerRevs(NO_REVOLUTION, FALSE, RF_ADC);
                break;

            case ADC_SPECIAL_PN:
                signal_ret = readPNmV(ADC_MUXOUT1, ADC_MUXOUT2, NO_REVOLUTION, FALSE);
                break;

            case ADC_SPECIAL_APC:
                signal_ret = readPNmV(ADC_VRPOW, ADC_VWPOW, NO_REVOLUTION, FALSE);
                signal_ret.middle_mv = MIN(signal_ret.top_mv,signal_ret.bot_mv);
                break;

            default:
                signal_ret = ReadGPADCmV(NO_REVOLUTION, monitor_signal, FALSE);
                break;
        }
        middle_mv = signal_ret.middle_mv; //mV

        if (type == TYPE4)
        {
            signal_ret.top_saturated = (middle_mv == GPADC_IN_MAX);
            signal_ret.bot_saturated = (middle_mv == GPADC_IN_MIN);
        }
        SendMsgCn(DEBUG_AUTO_TUNING_MSG,2,0x316421,A2B(middle_mv)); //Return_mV
        sCurrentError = (polarity)? (middle_mv - target_mv) : (target_mv - middle_mv);
        bCurrentLevel = (sCurrentError > 0)? SIGNAL_ABOVE_TARGET : SIGNAL_BELOW_TARGET;

        if (sCurrentError == 0)
        {
            //very lucky, end of search
            break;
        }
        else
        {
            switch (phase)
            {
                case GET_FIRST_SAMPLE:
                    if (!(signal_ret.top_saturated || signal_ret.bot_saturated))
                    {
                        phase++;
                    }
                    else if ((bCurrentLevel != bPreviousLevel) && (bPreviousLevel != SIGNAL_UNKNOW))
                    {
                        ref_move_cnt = ref_move_cnt / 8 + 1;
                    }
                    move_count = (sCurrentError >= 0)? -ref_move_cnt : ref_move_cnt;
                    // save first sample
                    bPreviousLevel = bCurrentLevel;
                    bPreOffsetCnt = bCurOffsetCnt;
                    sPreviousError = sCurrentError;
                    break;

                case GET_SECOND_SAMPLE:
                    if (signal_ret.top_saturated || signal_ret.bot_saturated)
                    {
                        ref_move_cnt = ref_move_cnt / 2 + 1;
                        move_count = (sCurrentError >= 0)? -ref_move_cnt : ref_move_cnt;
                        break;
                    }
                    else
                    {
                        // use first sample and current sample to calculate the slope
                        float_k = (sCurrentError - sPreviousError);
                        float_k = float_k / (calDacCount2SBYTE(bPreOffsetCnt,use_Bits) - calDacCount2SBYTE(bCurOffsetCnt,use_Bits));
                        move_count = 255;
                        if ((float_k > 5) && (i < 2))
                        {
                            SendMsg80(SHOW_DEBUG_MSG,0x310604); // WARNING: Polarity Wrong
                            //only show warning because wrong detection is seen on RFCN_OS calibration
                            //polarity = (!polarity);
                            //sCurrentError = -sCurrentError;
                        }
                        else if (float_k < 0)
                        {
                            move_count = float2short((float)(sCurrentError)/float_k);
                        }
                        if (ABS(move_count) <= ((ref_move_cnt < 8)? 8 : ref_move_cnt))
                        {
                            phase++;
                            bPreviousLevel = SIGNAL_UNKNOW;
                            if ((move_count > 1) || (move_count < -1))
                            {
                                break;
                            }
                            /*else go to case FINE_ADJUST:*/
                        }
                        else
                        {
                            bPreOffsetCnt = bCurOffsetCnt;
                            sPreviousError = sCurrentError;
                            move_count = (sCurrentError >= 0)? -ref_move_cnt : ref_move_cnt;
                            break;
                        }
                    }

                case FINE_ADJUST:
                    //--------------------------------------------------------------------------;
                    //check target crossing and minimum error count!!
                    //--------------------------------------------------------------------------;
                    if ((bCurrentLevel != bPreviousLevel) && (bPreviousLevel != SIGNAL_UNKNOW)) //cross target detected!!
                    {
                        // end of search
                        if (ABS(sCurrentError) > ABS(sPreviousError))
                        {
                            bCurOffsetCnt = bPreOffsetCnt;
                            sCurrentError = sPreviousError;
                            calSaveAllAfeOffset(adjust_signal,type,bCurOffsetCnt); //fill all related register with same value
                        }
                        phase = TARGET_FOUND;
                        move_count = 0;
                        break;
                    }
                    bPreviousLevel = bCurrentLevel;
                    sPreviousError = sCurrentError;
                    bPreOffsetCnt = bCurOffsetCnt;

                    //--------------------------------------------------------------------------;
                    // calculate next cnt setting based on the measured and given value         ;
                    //--------------------------------------------------------------------------;
                    move_count = (sCurrentError > 0)? -1 : 1;
                    break;
            }
            if (phase != TARGET_FOUND)
            {
                //If target is found, don't need to calculate next offset count.
                next_off_ret = calAdjNextRoughOffset(bCurOffsetCnt,use_Bits,move_count,only_positive);
                bCurOffsetCnt = next_off_ret.val;
            }
            if ((next_off_ret.errcode != eNOERR) && (bCurOffsetCnt == bPreOffsetCnt))
            {
                // end of search
                ErrCode = next_off_ret.errcode;
                phase = TARGET_FOUND;
            }
            if (phase == TARGET_FOUND)
            {
                // end of search
                break;
            }
            calSaveAllAfeOffset(adjust_signal,type,bCurOffsetCnt); //fill all related register with same value
        }
    }// for i loop...

    switch (monitor_signal)
    {
        case ADC_TE:
            READ_MOD_WRITE(HAL_AFEREG_MAP->FCTS38,(HAL_AFE_TEFCSFSEL_MASK | HAL_AFE_TELPSET_MASK),orig_LPF);
            break;

        case ADC_FE:
            READ_MOD_WRITE(HAL_AFEREG_MAP->FCTS32,(HAL_AFE_FEFCSFSEL_MASK | HAL_AFE_FELPSET_MASK),orig_LPF);
            break;

        case ADC_CE:
            READ_MOD_WRITE(HAL_AFEREG_MAP->FCTS36,HAL_AFE_CEFCSFSEL_MASK,orig_LPF);
            break;

        case ADC_BS:
            READ_MOD_WRITE(HAL_AFEREG_MAP->BSRP09,(HAL_AFE_BSLP_MASK | HAL_AFE_BSLPSET_MASK),orig_LPF);
            break;

        case ADC_RP:
            READ_MOD_WRITE(HAL_AFEREG_MAP->BSRP14,(HAL_AFE_RPLP_MASK | HAL_AFE_RPLPSET_MASK),orig_LPF);
            break;
    }

    last_cal_recd = recd_status;

    if (i < MAX_OFFSET_LOOP)
    {
        //we've enter fine search, the result should be better then default.
        last_cal_result = bCurOffsetCnt;
        SendMsgCnB(DEBUG_AUTO_TUNING_MSG,1,0x316428,B1B(calDacCount2SBYTE(last_cal_result,use_Bits))); ////Last_DAC_result==
    }
    else
    {
        ErrCode = eLOOP_expire_calibrate_TE_offset;
        SendMsg80(DEBUG_AUTO_TUNING_MSG,0x316429); //Out of calibrate loop
    }

    /* set the final offset value into target offset register and restore the original offset value to other offset register */
    switch (type)
    {
        case TYPE1:
            WRITE_REG(*(BYTE *)(AfeOffsetInfoT1[adjust_signal].field_write_ofs),tempCalOfsT1Var.write_offset);
#if (CHIP_REV >= 0xC0)
            WRITE_REG(*(BYTE *)(AfeOffsetInfoT1[adjust_signal].field_land_write_ofs),tempCalOfsT1Var.land_write_offset);
#endif
            WRITE_REG(*(BYTE *)(AfeOffsetInfoT1[adjust_signal].field_groove_ofs[RECDL]),tempCalOfsT1Var.groove_offset[RECDL]);
            WRITE_REG(*(BYTE *)(AfeOffsetInfoT1[adjust_signal].field_groove_ofs[RECDH]),tempCalOfsT1Var.groove_offset[RECDH]);
            WRITE_REG(*(BYTE *)(AfeOffsetInfoT1[adjust_signal].field_land_ofs[RECDL]),tempCalOfsT1Var.land_offset[RECDL]);
            WRITE_REG(*(BYTE *)(AfeOffsetInfoT1[adjust_signal].field_land_ofs[RECDH]),tempCalOfsT1Var.land_offset[RECDH]);
            break;

        case TYPE2:
            WRITE_REG(*(BYTE *)(AfeOffsetInfoT2[adjust_signal].field_write_ofs),tempCalOfsT2Var.write_offset);
            WRITE_REG(*(BYTE *)(AfeOffsetInfoT2[adjust_signal].field_normal_ofs),tempCalOfsT2Var.normal_offset);
            break;

        case TYPE3:
            WRITE_REG(*(BYTE *)(AfeOffsetInfoT3[adjust_signal].field_normal_ofs),tempCalOfsT3Var.normal_offset);
            break;

        case TYPE4:
            READ_MOD_WRITE(*(BYTE *)(AfeOffsetInfoT4[adjust_signal].field_normal_ofs.addr),
                                    (AfeOffsetInfoT4[adjust_signal].field_normal_ofs.mask),
                                    (tempCalOfsT3Var.normal_offset << (AfeOffsetInfoT4[adjust_signal].field_normal_ofs.start_bit)));
            break;
    }
    /* set the final offset value into target offset rigister and restore the original offset value to other offset register */

    return(ErrCode);
}

/*-----------------------------------------------------------------------------
 *
 * Module: calCalibrateStepsize()
 * Description: measure the mV_diff of 256 offset dac with different gain
 *              set max_gain = 0 means measure stepsize with current gain
 *
 *----------------------------------------------------------------------------*/
errcode_t calCalibrateStepsize(BYTE    adjust_offset,       // adjusted signal
                               BYTE    adjust_gain,
                               BYTE    type,                // offset type
                               BYTE    max_gain,
                               //SHORT   max_limit_mv,
                               //SHORT   min_limit_mv,
                               USHORT  *stepsize)
{
    data_field_t gn_field;
    errcode_t    ErrCode;
    ad_dsp_t     signal_ret;
    ULONG        os_field;
    BYTE         monitor_signal,total_bit;
    BYTE         orig_INPUT,orig_BPLPF,orig_gn,orig_os;
    SBYTE        i;
    SHORT        os_diff,os_max,os_min,sum_mv,max_limit_mv,min_limit_mv;

    GAIN_RECD_HIGH;
    ErrCode = eNOERR;
    monitor_signal = afeSetCalibMUX(type, adjust_offset);

    switch (type)
    {
        case TYPE1:
            //switch off the input, so we can do this calibration even with focus on.
            switch (adjust_offset)
            {
                case TE_OS:
                    orig_INPUT = READ_REG(HAL_AFEREG_MAP->FCTS36);
                    READ_MOD_WRITE(HAL_AFEREG_MAP->FCTS36, (HAL_AFE_TEMPP_MASK|HAL_AFE_TESP1_MASK|HAL_AFE_TEDPD_MASK), 0);
                    orig_BPLPF = READ_FIELD(HAL_AFE_BPTELP);
                    WRITE_FIELD(HAL_AFE_BPTELP,1);
                    break;
            }
            gn_field = AfeGainInfoT1[adjust_gain].field_groove_gn[1];
            os_field = AfeOffsetInfoT1[adjust_offset].field_groove_ofs[1];
            total_bit = AfeOffsetInfoT1[adjust_offset].total_Bits;
            break;

        case TYPE2:
            gn_field = AfeGainInfoT2[adjust_gain].field_normal_gn;
            os_field = AfeOffsetInfoT2[adjust_offset].field_normal_ofs;
            total_bit = AfeOffsetInfoT2[adjust_offset].total_Bits;
            break;

        case TYPE3:
            //switch off the input, so we can do this calibration even with focus on.
            switch (adjust_offset)
            {
                case CE_OS:
                    orig_INPUT = READ_REG(HAL_AFEREG_MAP->FCTS33);
                    READ_MOD_WRITE(HAL_AFEREG_MAP->FCTS33, (HAL_AFE_CEMPP_MASK|HAL_AFE_CESP1_MASK), 0);
                    orig_BPLPF = READ_FIELD(HAL_AFE_BPCELP);
                    WRITE_FIELD(HAL_AFE_BPCELP,1);
                    break;

                case RP2_OS:
                    WRITE_FIELD(HAL_AFE_RPPGN,0);
                    WRITE_FIELD(HAL_AFE_RPGN,0);
                    WRITE_FIELD(HAL_AFE_RPOS,0);
                case RP_OS:
                    orig_INPUT = READ_REG(HAL_AFEREG_MAP->BSRP10);
                    READ_MOD_WRITE(HAL_AFEREG_MAP->BSRP10, (HAL_AFE_RPSRF_MASK|HAL_AFE_RPSBA_MASK|HAL_AFE_RPPHS_MASK|HAL_AFE_RPBHS_MASK), 0);
                    orig_BPLPF = READ_FIELD(HAL_AFE_BPRPLP);
                    WRITE_FIELD(HAL_AFE_BPRPLP,1);
                    break;

                case RFPGA_OS:
#if (NEW_RFPOS_CALIB_BRING_AGCVREF_TO_ZERO == 1)
                case RFP_OS:
#endif
                    orig_INPUT = READ_FIELD(HAL_AFE_RFGN);
                    WRITE_FIELD(HAL_AFE_RFGN, 0);
                    break;
            }
            gn_field = AfeGainInfoT3[adjust_gain].field_normal_gn;
            os_field = AfeOffsetInfoT3[adjust_offset].field_normal_ofs;
            total_bit = AfeOffsetInfoT3[adjust_offset].total_Bits;
            break;
    }

    orig_gn = READ_ADR_1B(gn_field.addr);
    orig_os = READ_ADR_1B(os_field);

    for (i = max_gain; i >= 0; i--)
    {
        if (max_gain > 0)
        {
            READ_MOD_WRITE(*(BYTE *)gn_field.addr,gn_field.mask,i << gn_field.start_bit);
        }
        if (i == max_gain)
        {
            WRITE_ADR_1B(os_field, (1 << (total_bit - 1)) - 1);
            Delay100uS(1);
            switch (monitor_signal)
            {
                case ADC_SPECIAL_RFDFE:
                    signal_ret = readADCmVPerRevs(NO_REVOLUTION, FALSE, RF_ADC);
                    break;

                case ADC_SPECIAL_PN:
                    signal_ret = readPNmV(ADC_MUXOUT1, ADC_MUXOUT2, NO_REVOLUTION, FALSE);
                    break;

                default:
                    signal_ret = ReadGPADCmV(NO_REVOLUTION, monitor_signal, FALSE);
                    break;
            }
            os_max = signal_ret.middle_mv;
            WRITE_ADR_1B(os_field, (1 << total_bit) - 1);
            Delay100uS(1);
            switch (monitor_signal)
            {
                case ADC_SPECIAL_RFDFE:
                    signal_ret = readADCmVPerRevs(NO_REVOLUTION, FALSE, RF_ADC);
                    break;

                case ADC_SPECIAL_PN:
                    signal_ret = readPNmV(ADC_MUXOUT1, ADC_MUXOUT2, NO_REVOLUTION, FALSE);
                    break;

                default:
                    signal_ret = ReadGPADCmV(NO_REVOLUTION, monitor_signal, FALSE);
                    break;
            }
            os_min = signal_ret.middle_mv;
            os_diff = ABS(os_max - os_min) / 2;

            max_limit_mv = (os_max + os_min + os_diff) / 2;
            min_limit_mv = (os_max + os_min - os_diff) / 2;

            calCalibrateOffset(adjust_offset,         // adjusted field
                               type,                  // type of offset structure
                               NORMAL_OFFSET,
                               max_limit_mv,          // target mv       unit: mV
                               1,                     // polarity between offset & signal
                               CALIB_FROM_0);
            os_max = last_cal_result;
            calCalibrateOffset(adjust_offset,         // adjusted field
                               type,                  // type of offset structure
                               NORMAL_OFFSET,
                               min_limit_mv,          // target mv       unit: mV
                               1,                     // polarity between offset & signal
                               CALIB_FROM_0);
            os_min = last_cal_result;
            os_diff = calDacCount2SBYTE(os_max,total_bit) - calDacCount2SBYTE(os_min,total_bit);

            if (os_diff == 0)
            {
                SendMsg80(SHOW_DEBUG_MSG,0x316671);//WARNING: Stepsize calib err, os_diff = 0
                ErrCode = eERR_signal_abnormal;
                for (i = 0; i <= max_gain; i++)
                {
                    stepsize[i] = 0x7FFF;
                }
                break;
            }
        }
        else if (monitor_signal == ADC_SPECIAL_RFDFE)
        {
            //adjust os_max, os_min due to the AGC_ref might be away from middle.
            os_max = calDacCount2SBYTE(os_max,total_bit);
            os_min = calDacCount2SBYTE(os_min,total_bit);
            if (os_diff > 0)
            {
                os_diff = (max_limit_mv - min_limit_mv) * 256 / stepsize[i + 1];
                os_max = ((os_max + os_min) + os_diff - (sum_mv - max_limit_mv - min_limit_mv) * 256 / stepsize[i + 1]) / 2;
            }
            else
            {
                os_diff = (min_limit_mv - max_limit_mv) * 256 / stepsize[i + 1];
                os_max = ((os_max + os_min) + os_diff + (sum_mv - max_limit_mv - min_limit_mv) * 256 / stepsize[i + 1]) / 2;
            }
            os_min = os_max - os_diff;
            os_max = calSBYTE2DacCount(os_max,total_bit);
            os_min = calSBYTE2DacCount(os_min,total_bit);
        }

        WRITE_ADR_1B(os_field, os_max);
        Delay100uS(1);
        switch (monitor_signal)
        {
            case ADC_SPECIAL_RFDFE:
                signal_ret = readADCmVPerRevs(NO_REVOLUTION, FALSE, RF_ADC);
                break;

            case ADC_SPECIAL_PN:
                signal_ret = readPNmV(ADC_MUXOUT1, ADC_MUXOUT2, NO_REVOLUTION, FALSE);
                break;

            default:
                signal_ret = ReadGPADCmV(NO_REVOLUTION, monitor_signal, FALSE);
                break;
        }
        sum_mv = signal_ret.middle_mv;
        stepsize[i] = signal_ret.middle_mv;

        WRITE_ADR_1B(os_field, os_min);
        Delay100uS(1);
        switch (monitor_signal)
        {
            case ADC_SPECIAL_RFDFE:
                signal_ret = readADCmVPerRevs(NO_REVOLUTION, FALSE, RF_ADC);
                break;

            case ADC_SPECIAL_PN:
                signal_ret = readPNmV(ADC_MUXOUT1, ADC_MUXOUT2, NO_REVOLUTION, FALSE);
                break;

            default:
                signal_ret = ReadGPADCmV(NO_REVOLUTION, monitor_signal, FALSE);
                break;
        }
        sum_mv += signal_ret.middle_mv;
        stepsize[i] -= signal_ret.middle_mv;
        stepsize[i] = stepsize[i] * 256 / ABS(os_diff);
        SendMsgCn(DEBUG_CALIB_RESULT_MSG,2,0x316651+i,A2B(stepsize[i]));//stepsize(OS=256, GN= i)==>
    }

    WRITE_ADR_1B(gn_field.addr,orig_gn);
    WRITE_ADR_1B(os_field,orig_os);

    for (i = 0; i <= max_gain; i++)
    {
        if (i == 0)
        {
            if (stepsize[i] == 0)
            {
                SendMsgCn(SHOW_DEBUG_MSG,4,0x316672,A2B(stepsize[i]),0x00,0x01); //WARNING: stepsize abnormal, %5d ==> %5d
                stepsize[i] = 1;
						#if (Customer_A1 ==1)
								PositionHomeErrorCount++;	
						#endif				
            }
        }
        else if (stepsize[i] <= stepsize[i - 1])
        {
            SendMsgCnB(SHOW_DEBUG_MSG,4,0x316672,B2B(stepsize[i]),B2B(stepsize[i - 1] + 1)); //WARNING: stepsize abnormal, %5d ==> %5d
            stepsize[i] = stepsize[i - 1] + 1;
				#if (Customer_A1 ==1)
						PositionHomeErrorCount++;	
				#endif			
        }
    }

    switch (type)
    {
        case TYPE1:
            switch (adjust_offset)
            {
                case TE_OS:
                    WRITE_REG(HAL_AFEREG_MAP->FCTS36, orig_INPUT);
                    WRITE_FIELD(HAL_AFE_BPTELP,orig_BPLPF);
                    break;
            }
            break;

        case TYPE2:
            break;

        case TYPE3:
            switch (adjust_offset)
            {
                case CE_OS:
                    WRITE_REG(HAL_AFEREG_MAP->FCTS33, orig_INPUT);
                    WRITE_FIELD(HAL_AFE_BPCELP,orig_BPLPF);
                    break;

                case RP2_OS:
                    LAL_WriteCoef(AFE_BSRP12_RPPGN);
                    LAL_WriteCoef(AFE_BSRP13_RPGN);
                    WRITE_FIELD(HAL_AFE_RPOS,svoCalOfsT3Var[Layer_Index][RP_OS].normal_offset);
                case RP_OS:
                    WRITE_REG(HAL_AFEREG_MAP->BSRP10, orig_INPUT);
                    WRITE_FIELD(HAL_AFE_BPRPLP,orig_BPLPF);
                    break;

                case RFPGA_OS:
#if (NEW_RFPOS_CALIB_BRING_AGCVREF_TO_ZERO == 1)
                case RFP_OS:
#endif
                    WRITE_FIELD(HAL_AFE_RFGN, orig_INPUT);
                    break;
            }
            break;
    }

    GAIN_RECD;
    return (ErrCode);
}

/*-----------------------------------------------------------------------------
 *
 * Module: calCalibrateTEGnOs()
 * Description: calibrate TE gain and offset based on given mV and TEO(svoCalVar.teOfsData.all16)
 *
 *----------------------------------------------------------------------------*/
errcode_t calCalibrateTEGnOs(USHORT target_p2p)
{
    data_field_t    target_field;
    errcode_t       ErrCode;
    ad_dsp_t        te_signal;
    USHORT          te_p2p;
    SHORT           target_mid,CurOffset,SumMid,ofs_step,temp_gnos;
    ULONG           target_field2;
    BOOL            gain_adj_needed;
    BYTE            i, j, k;
    BYTE            RECDcheckcount;
    BYTE            use_Bits;
    BYTE            orig_TELpfBW,orig_gn,orig_os;
    BYTE            orig_DPDCPFC;
    USHORT          pre_tep2p;
    struct          svoDpdBankFcVarT DpdSettingCurUsed;

    target_mid = CONVERT_DSPOFS_TO_ADCMV(svoCalVar.teOfsData.all16);
    SendMsgCn(DEBUG_AUTO_TUNING_MSG,4,0x316710,A2B(target_p2p),A2B(target_mid));
    HAL_SERVO_CenterControlOFF();
    //---------------------------------------------------;
    // Blank Area check                                  ;
    //---------------------------------------------------;
    ErrCode = eNOERR;
    RECDcheckcount = 0;

    DpdSettingCurUsed.DpdBwBank = READ_FIELD(HAL_AFE_DPDHSISCL);
    DpdSettingCurUsed.DpdBwFc   = READ_FIELD(HAL_AFE_DPDHSFCH);
    DpdSettingCurUsed.DpdHpfBw   = READ_FIELD(HAL_AFE_DPDINFC);

    if ((svoCalVar.calGainDoneBits & teg_end) == teg_end)
    {
        WRITE_FIELD(HAL_AFE_DPDHSISCL, DesriedDpdSetting.DpdBwBank);
        WRITE_FIELD(HAL_AFE_DPDHSFCH, DesriedDpdSetting.DpdBwFc);
        WRITE_FIELD(HAL_AFE_DPDHSFCL, DesriedDpdSetting.DpdBwFc);
        WRITE_FIELD(HAL_AFE_DPDINFC, DesriedDpdSetting.DpdHpfBw);
    }

    for (i = 0; i < TEGNOS_RECD_CHK_CNT; i++)
    {
        if (HAL_GLOBAL_GetRfPresent() == ON)
            RECDcheckcount++;
        WaitTimer(1);
    }

    if (1)//if (RECDcheckcount > (((svoVar_discKindT.Bit.DiscStd == eDiscDVD) && (svoVar_TrackingMethod == TRK_METHOD_DPD))? TEGNOS_RECD_CHK_TH_DVDDPD : TEGNOS_RECD_CHK_TH))
    {
        last_cal_recd = TRUE;   // recorded area
    }
    else
    {
        last_cal_recd = FALSE;  // blank area
        if (svoVar_TrackingMethod == TRK_METHOD_DPD)
        {
            SendMsg80(SHOW_DEBUG_MSG,0x310670);//Blank skip TEG
            if (svoIramVar.currentID.Layer == 1)
            {
                svoCalVar.calGainDoneBits |= teg_end;
            }
            target_field = AfeGainInfoT1[TE_GN].field_groove_gn[last_cal_recd];
            last_cal_result = READ_FROM_FIELD(*(BYTE *)target_field.addr,target_field.mask, target_field.start_bit);
            target_field2 = AfeOffsetInfoT1[TE_OS].field_groove_ofs[last_cal_recd];
            last_cal_result2 = READ_REG(*(vBYTE *)target_field2);
            ErrCode = eLIMIT_blank_trkg_calibrate_TE_gain;
            WRITE_FIELD(HAL_AFE_DPDHSISCL, DpdSettingCurUsed.DpdBwBank);
            WRITE_FIELD(HAL_AFE_DPDHSFCH, DpdSettingCurUsed.DpdBwFc);
            WRITE_FIELD(HAL_AFE_DPDHSFCL, DpdSettingCurUsed.DpdBwFc);
            WRITE_FIELD(HAL_AFE_DPDINFC, DpdSettingCurUsed.DpdHpfBw);
            return(ErrCode);
        }
        //load blank settings to written register due to RECD will be forced high during calibration
        WRITE_FIELD(HAL_AFE_BSKG, svoCalGnT2Var[Layer_Index][BS_GN].write_gain);
        WRITE_FIELD(HAL_AFE_BSKO, svoCalOfsT2Var[Layer_Index][BS_OS].write_offset);
        WRITE_FIELD(HAL_AFE_FCSG, svoCalGnT1Var[Layer_Index][FE_GN].groove_gain[RECDL]);
        WRITE_FIELD(HAL_AFE_FCSO, svoCalOfsT1Var[Layer_Index][FE_OS].groove_offset[RECDL]);
    }

    target_field = AfeGainInfoT1[TE_GN].field_groove_gn[last_cal_recd];
    last_cal_result = READ_FROM_FIELD(*(BYTE *)target_field.addr,target_field.mask, target_field.start_bit);
    last_cal_result = (last_cal_result > 3)? last_cal_result - 3 : 0;
    target_field = AfeGainInfoT1[TE_GN].field_groove_gn[RECDH];
    orig_gn = READ_FROM_FIELD(*(BYTE *)target_field.addr,target_field.mask, target_field.start_bit);
    READ_MOD_WRITE(*(BYTE *)target_field.addr,target_field.mask,last_cal_result << target_field.start_bit);

    target_field2 = AfeOffsetInfoT1[TE_OS].field_groove_ofs[last_cal_recd];
    last_cal_result2 = READ_REG(*(vBYTE *)target_field2);
    target_field2 = AfeOffsetInfoT1[TE_OS].field_groove_ofs[RECDH];
    orig_os = READ_REG(*(vBYTE *)target_field2);
    WRITE_REG(*(BYTE *)target_field2, last_cal_result2);

    use_Bits = AfeOffsetInfoT1[TE_OS].total_Bits;

#if (DVD_RAM_READ == 1)
    // Force LAND to low (a) to use GROOVE gain register
    //                   (b) to prevent from TE polarity change
    if (DVDRAM)
    {
        WRITE_FIELD(HAL_AFE_LANDSEL,0);
    }
#endif

    /* Force TELP to 300kHz */
    orig_TELpfBW = HAL_AFE_GetTELpfBW();
    HAL_AFE_SetTELpfBW(e_TELPF_300); //M.H. Note: 300KHZ is 330KHz for SC6230

    /* Force DPD Charge Pump fc to 200kHz */
    if (svoVar_TrackingMethod == TRK_METHOD_DPD)
    {
        orig_DPDCPFC = READ_FIELD(HAL_AFE_DPDCPFC);
#if (CHIP_REV >= 0xC1)
        WRITE_FIELD(HAL_AFE_DPDCPFC,halAFE_DPDCPFBW2_150kHz);
#else
        WRITE_FIELD(HAL_AFE_DPDCPFC,halAFE_DPDCPFBW_200kHz);
#endif
    }

    /* Force to use NORMAL gain register, in case RECD toggle during calibration */
    GAIN_RECD_HIGH;

    //---------------------------------------------------;
    // TE gain & offset adjust                           ;
    //---------------------------------------------------;
    for (i = 0; i < TEGNOS_MAX_LOOP; i++)
    {
        CurOffset = calDacCount2SBYTE(last_cal_result2,use_Bits);
        gain_adj_needed = FALSE;
        te_p2p = 0;
        SumMid = 0;
        // TEGNOS CAL: LOOP %d: TRKG = %2d (dB)
        SendMsgCnB(DEBUG_AUTO_TUNING_MSG,2,0x316713,B1B(i),B1B(last_cal_result-16));
        k = 0;
        for (j = 0; j < TEGNOS_MAX_SAMPLES; j++)
        {
            WaitTimer(MS_1);

#if (DVD_RAM_READ == 1)
            if (DVDRAM_rewritable)
            {
                // Headers will cause noise on TE in DVD-RAM rewritable area.
                // Use average method to minimize the header noise.
                te_signal = ReadGPADCmV_Avg(ONE_REVOLUTION, ADC_TE, TRUE);
            }
            else
#endif
            {
                te_signal = ReadFilteredDSPADCmV(ONE_REVOLUTION, TE_ALL, ADC_TE, TRUE);
                if (svoVar_TrackingMethod == TRK_METHOD_DPD)
                {
                    te_signal.middle_mv = te_signal.top_mv;
                }
            }
            // TEGnOs CAL: TE_OS = %4d (mV), TE_MID = %4d (mV), TE_P2P = %4d (mV)
            SendMsgCnB(DEBUG_AUTO_TUNING_MSG,6,0x316715,B2B(CurOffset*5),B2B(te_signal.middle_mv),B2B(te_signal.p2p_mv));
            if ((svoVar_discKindT.Bit.DiscStd == eDiscDVD) && (svoVar_TrackingMethod == TRK_METHOD_DPD))
            {
                if (((j != 0) && (ABS(pre_tep2p - te_signal.p2p_mv) > (target_p2p / 3))) || (te_signal.p2p_mv < target_p2p / 10))
                {
                    SendMsg80(SHOW_DEBUG_MSG,0x310673);//PPerror skip TEG
                    if (svoIramVar.currentID.Layer == 1)
                    {
                        svoCalVar.calGainDoneBits |= teg_end;
                    }
                    last_cal_result = orig_gn;
                    last_cal_result2 = orig_os;
                    ErrCode = eLIMIT_blank_trkg_calibrate_TE_gain;
                    WRITE_FIELD(HAL_AFE_DPDHSISCL, DpdSettingCurUsed.DpdBwBank);
                    WRITE_FIELD(HAL_AFE_DPDHSFCH, DpdSettingCurUsed.DpdBwFc);
                    WRITE_FIELD(HAL_AFE_DPDHSFCL, DpdSettingCurUsed.DpdBwFc);
                    WRITE_FIELD(HAL_AFE_DPDINFC, DpdSettingCurUsed.DpdHpfBw);
                    return(ErrCode);
                }
                pre_tep2p = te_signal.p2p_mv;
            }

            if (te_signal.errcode != eNOERR)
            {
                ErrCode = te_signal.errcode;
                break;
            }
            if ((te_signal.top_saturated == TRUE) || (te_signal.bot_saturated == TRUE) || ((j == 0) && ((te_signal.p2p_mv < target_p2p / 2) || (te_signal.p2p_mv > target_p2p))))
            {
                gain_adj_needed = TRUE;
                break;
            }

            te_p2p += te_signal.p2p_mv;
            SumMid += (te_signal.middle_mv - target_mid);
            svoCurvefitCalVar.cost[k] = (float)te_signal.middle_mv - target_mid;
            svoCurvefitCalVar.item[k] = (float)CurOffset;

            k = j + 1;
            if (k >= TEGNOS_MIN_SAMPLES)
            {
                //calculate offset
                LineFit(k, svoCurvefitCalVar.item, svoCurvefitCalVar.cost, svoCurvefitCalVar.cof);
                if (svoCurvefitCalVar.cof[1] == 0)
                    temp_gnos = 0x7FFF;
                else
                    temp_gnos = float2short(-svoCurvefitCalVar.cof[0] / svoCurvefitCalVar.cof[1]);

                if ((temp_gnos >= svoCurvefitCalVar.item[0]) && (temp_gnos <= svoCurvefitCalVar.item[j]))
                {
                    //optimal TE_OS within range, calibration finished.
                    break;
                }
            }

            if (k < TEGNOS_MAX_SAMPLES)
            {
                //calculate the TE_OS distance between each samples, larger TE_GN smaller ofs_step
                ofs_step = (TOP_SATURATE_LIM - BOT_SATURATE_LIM - te_signal.p2p_mv) * (256 / (TEGNOS_MAX_SAMPLES + 1)) / TE_step_size[last_cal_result] + 1;
                if (ofs_step > (256 / (TEGNOS_MIN_SAMPLES + 1)))
                    ofs_step = (256 / (TEGNOS_MIN_SAMPLES + 1));

                if (READ_FIELD(HAL_AFE_TEINV)? (((SumMid > 0) || (svoCurvefitCalVar.item[0] == -127)) && (svoCurvefitCalVar.item[j] < 127)) : (((SumMid < 0) || (svoCurvefitCalVar.item[0] == -127)) && (svoCurvefitCalVar.item[j] < 127)))
                {
                    CurOffset = svoCurvefitCalVar.item[j] + ofs_step;
                }
                else
                {
                    for (; k > 0; k--)
                    {
                        svoCurvefitCalVar.cost[k] = svoCurvefitCalVar.cost[k - 1];
                        svoCurvefitCalVar.item[k] = svoCurvefitCalVar.item[k - 1];
                    }
                    CurOffset = svoCurvefitCalVar.item[k] - ofs_step;
                }
                WRITE_REG(*(BYTE *)target_field2, calSBYTE2DacCount(CurOffset,use_Bits));
            }
            else
            {
                //the default offset is too far from optimal offset.
                SendMsg80(SHOW_DEBUG_MSG,0x310605); // WARNING: TEOS Max. Samples
            }
        }

        if (gain_adj_needed)
        {
            if ((te_signal.top_saturated == TRUE) || (te_signal.bot_saturated == TRUE))
            {
                //------------------------------------------------------------------;
                // If saturation occurs, then lower the gain to remove this problem ;
                //------------------------------------------------------------------;
                temp_gnos = last_cal_result - TEGNOS_DB_REDUCE_AT_SAT;
                if (temp_gnos > 0x1F) temp_gnos = 0x1F;
                if (temp_gnos < 0) temp_gnos = 0;
            }
            else
            {   //p2p too far from target
                temp_gnos = (LONG)(target_mid - te_signal.middle_mv) * 256 / TE_step_size[last_cal_result];
                CurOffset += (READ_FIELD(HAL_AFE_TEINV)? (-temp_gnos) : (temp_gnos));
                if (CurOffset > 127)  CurOffset = 127;
                if (CurOffset < -127) CurOffset = -127;
                last_cal_result2 = calSBYTE2DacCount(CurOffset,8);
                temp_gnos = calBestGain(TE_step_size, last_cal_result, 0x1F, te_signal.p2p_mv, target_p2p * 3 / 4);
            }

            if (temp_gnos == last_cal_result)
            {
                ErrCode = eLIMIT_trkg_calibrate_TE_gain;
                break;
            }
            last_cal_result = temp_gnos;
            WRITE_REG(*(BYTE *)target_field2, last_cal_result2);
            READ_MOD_WRITE(*(BYTE *)target_field.addr, target_field.mask, last_cal_result << target_field.start_bit);
        }
        else
        {   //calculate calib. result from the 3 samples
            if (ErrCode == eNOERR)
            {
                //store offset
                if (temp_gnos >  127)
                {
                    temp_gnos =  127;
                    ErrCode = eERR_adj_next_offset_over_high_limit;
                }
                if (temp_gnos < -127)
                {
                    temp_gnos = -127;
                    ErrCode = eERR_adj_next_offset_over_low_limit;
                }
                last_cal_result2 = calSBYTE2DacCount(temp_gnos,use_Bits);

                //calculate gain
                last_cal_result = calBestGain(TE_step_size, last_cal_result, 0x1F, te_p2p, (target_p2p * k));
            }
            break;
        }
    }
    if (i >= TEGNOS_MAX_LOOP)
    {
        ErrCode = eLOOP_expire_calibrate_TE_gain;
    }

    if (ErrCode != eNOERR)
    {
        WRITE_FIELD(HAL_AFE_DPDHSISCL, DpdSettingCurUsed.DpdBwBank);
        WRITE_FIELD(HAL_AFE_DPDHSFCH, DpdSettingCurUsed.DpdBwFc);
        WRITE_FIELD(HAL_AFE_DPDHSFCL, DpdSettingCurUsed.DpdBwFc);
        WRITE_FIELD(HAL_AFE_DPDINFC, DpdSettingCurUsed.DpdHpfBw);
    }
    WRITE_REG(*(BYTE *)target_field2, orig_os);
    READ_MOD_WRITE(*(BYTE *)target_field.addr, target_field.mask, orig_gn << target_field.start_bit);

    GAIN_RECD;
    if (last_cal_recd == FALSE)
    {
        WRITE_FIELD(HAL_AFE_BSKG, svoCalGnT2Var[Layer_Index][BS_GN].normal_gain);
        WRITE_FIELD(HAL_AFE_BSKO, svoCalOfsT2Var[Layer_Index][BS_OS].normal_offset);
        WRITE_FIELD(HAL_AFE_FCSG, svoCalGnT1Var[Layer_Index][FE_GN].groove_gain[RECDH]);
        WRITE_FIELD(HAL_AFE_FCSO, svoCalOfsT1Var[Layer_Index][FE_OS].groove_offset[RECDH]);
    }

    HAL_AFE_SetTELpfBW(orig_TELpfBW);

    if (svoVar_TrackingMethod == TRK_METHOD_DPD)
    {
        WRITE_FIELD(HAL_AFE_DPDCPFC,orig_DPDCPFC);
        last_cal_result += 2;//DPD is noisy with the higher freq. LPF//TBD:need check later//hugo debug
    }

#if (DVD_RAM_READ == 1)
    // Restore LAND control.
    if (DVDRAM)
    {
        WRITE_FIELD(HAL_AFE_LANDSEL,1);
    }
#endif
    // TRKG = %d (dB), TE_OS = %d (mV), RECD = %d, FEBC = %d
    SendMsgCn(SHOW_DEBUG_MSG,3,0x316714,last_cal_result-16,(calDacCount2SBYTE(last_cal_result2,use_Bits)*5)>>8,calDacCount2SBYTE(last_cal_result2,use_Bits)*5);
    return(ErrCode);
}

/*-----------------------------------------------------------------------------
 *
 * Module: calCalibrateRPBG()
 * Description: target_mid == 0: RP based on SRFO only, performed during focus off.
 *                               the optimal RPBG should make RP doesn't shift when SRFO shift
 *              target_mid != 0: RP based on SRFO and SBAD, performed during focus on.
 *                               the optimal RPBG should make RP doesn't shift after focus on
 *----------------------------------------------------------------------------*/
#define MAX_RPBG_LOOP   9
#define MIN_RPBG_LOOP   2
#define MAX_RPBG_SAMPLES    3
#define RPBG_STEP_SIZE  3
#define RPGN_STEP_SIZE  2
errcode_t calCalibrateRPBG(USHORT target_mid)
{
    errcode_t ErrCode;
    ad_dsp_t  rp_signal;
    SHORT     mid_err,sum_err;
    SBYTE     loop,index,i,origRPBG,curRPBG,origRPGN;
    SBYTE     optRPBG;
    BYTE      saturated;

    ErrCode = eNOERR;
    SendMsgCn(DEBUG_AUTO_TUNING_MSG,2,0x316801,A2B(target_mid));    //RP_Bal_Gain_CALIBRATE_START!!
    optRPBG = origRPBG = curRPBG = READ_FIELD(HAL_AFE_RPBG);

    if (target_mid == 0)
    {
        ErrCode = calCalibrateOffset(SRF_OS,                // adjusted field
                                     TYPE2,                 // type of offset structure
                                     NORMAL_OFFSET,         // offset type
                                     vref_fm_mux3_mV + 700, // target level       unit: mV
                                     TRUE,                  // polarity between offset & signal
                                     CALIB_FROM_CURRENT);
    }
    else
    {
        origRPGN = READ_FIELD(HAL_AFE_RP2GN);
        WRITE_FIELD(HAL_AFE_RP2GN, ((origRPGN > 6)? (origRPGN - 6) : 0));
    }

    loop = 0;
    sum_err = 0;
    index = 0;
    i = 0;
    while ((loop++ < MAX_RPBG_LOOP) && (ErrCode == eNOERR))
    {
        SendMsgCn(DEBUG_AUTO_TUNING_MSG,1,0x316816,A1B(curRPBG));
        saturated = 0;
        if (target_mid == 0)
        {
            WRITE_FIELD(HAL_AFE_SRFOSA,last_cal_result);
            Delay100uS(10);
            rp_signal = ReadGPADCmV(NO_REVOLUTION, ADC_RP, FALSE);
            if (rp_signal.errcode == eNOERR)
            {
                saturated |= ((rp_signal.top_saturated << 1) | rp_signal.bot_saturated);

                mid_err = rp_signal.middle_mv;
                WRITE_FIELD(HAL_AFE_SRFOSA,svoCalOfsT2Var[Layer_Index][SRF_OS].normal_offset);
                Delay100uS(10);
                rp_signal = ReadGPADCmV(NO_REVOLUTION, ADC_RP, FALSE);
                mid_err -= rp_signal.middle_mv;
                SendMsgCn(DEBUG_AUTO_TUNING_MSG,2,0x31681A,A2B(mid_err));
            }
        }
        else
        {
            rp_signal = ReadFilteredDSPADCmV(ONE_REVOLUTION, RP_ALL, ADC_RP, TRUE);

            mid_err = rp_signal.middle_mv - target_mid;
            SendMsgCn(DEBUG_AUTO_TUNING_MSG,2,0x31681A,A2B(rp_signal.middle_mv));
        }

        if (rp_signal.errcode != eNOERR)
        {
            ErrCode = rp_signal.errcode;
            break;
        }

        saturated |= ((rp_signal.top_saturated << 1) | rp_signal.bot_saturated);
        if (saturated != 0)
        {
				#if (Customer_A1 ==1)
						PositionHomeErrorCount++;	
				#endif        
            SendMsg80(SHOW_DEBUG_MSG,0x310607); // WARNING: RP Saturated
            if (saturated == 0x03)
            {//both top and bottom are saturated
                ErrCode = eERR_read_adc_p2p_mid_SAT;
                break;
            }
            index = 0;
            sum_err = 0;
            i = 0;
        }

        svoCurvefitCalVar.cost[i] = (float)mid_err;
        svoCurvefitCalVar.item[i] = (float)curRPBG;
        sum_err += mid_err;
        index++;

        if (index >= MIN_RPBG_LOOP)
        {
            //calculate offset
            if (index > MAX_RPBG_SAMPLES)
            {
                if (READ_FIELD(HAL_AFE_RPINV)? (sum_err > 0) : (sum_err < 0))
                {
                    LineFit(MAX_RPBG_SAMPLES, &svoCurvefitCalVar.item[index - MAX_RPBG_SAMPLES], &svoCurvefitCalVar.cost[index - MAX_RPBG_SAMPLES], svoCurvefitCalVar.cof);
                }
                else
                {
                    LineFit(MAX_RPBG_SAMPLES, svoCurvefitCalVar.item, svoCurvefitCalVar.cost, svoCurvefitCalVar.cof);
                }
            }
            else
            {
                LineFit(index, svoCurvefitCalVar.item, svoCurvefitCalVar.cost, svoCurvefitCalVar.cof);
            }
            optRPBG = float2short(-svoCurvefitCalVar.cof[0] / svoCurvefitCalVar.cof[1]);
            if (optRPBG < svoCurvefitCalVar.item[0])
            {
                optRPBG = svoCurvefitCalVar.item[0];
            }
            else if (optRPBG > svoCurvefitCalVar.item[index - 1])
            {
                optRPBG = svoCurvefitCalVar.item[index - 1];
            }
            else
            {
                //optimal RPBG within range, calibration finished.
                break;
            }
        }

        if (READ_FIELD(HAL_AFE_RPINV)? (sum_err > 0) : (sum_err < 0))
        {
            if (svoCurvefitCalVar.item[index - 1] >= 0x1F) break;
            i = index;
            curRPBG = svoCurvefitCalVar.item[index - 1] + (RPBG_STEP_SIZE * (MAX_RPBG_LOOP - loop) / MAX_RPBG_LOOP + 1);
            if (curRPBG > 0x1F) curRPBG = 0x1F;
        }
        else
        {
            if (svoCurvefitCalVar.item[0] <= 0) break;
            for (i = index; i > 0; i--)
            {
                svoCurvefitCalVar.cost[i] = svoCurvefitCalVar.cost[i - 1];
                svoCurvefitCalVar.item[i] = svoCurvefitCalVar.item[i - 1];
            }
            curRPBG = svoCurvefitCalVar.item[0] - (RPBG_STEP_SIZE * (MAX_RPBG_LOOP - loop) / MAX_RPBG_LOOP + 1);
            if (curRPBG < 0) curRPBG = 0;
        }
        WRITE_FIELD(HAL_AFE_RPBG,curRPBG);
        if (saturated != 0)
        {
            index = 0;
            sum_err = 0;
            i = 0;
        }
    }
    //restore original settings
    WRITE_FIELD(HAL_AFE_RPBG,origRPBG);

    if (loop > MAX_RPBG_LOOP)
    {
        SendMsg80(SHOW_DEBUG_MSG,0x310606); // WARNING: RPBG Max. Samples
        ErrCode = eLOOP_expire_calibrate_TE_gain;
    }

    last_cal_result = optRPBG;
    if (target_mid == 0)
    {
        HAL_AFE_SetPhBhDrooprate(e_PHS, (float)COEF_GetValue(AFE_CALC_PhsSlopeX100) / 100);
        HAL_AFE_SetPhBhDrooprate(e_BHS, (float)COEF_GetValue(AFE_CALC_BhsSlope));
    }
    else
    {
        WRITE_FIELD(HAL_AFE_RP2GN, origRPGN);
    }

    return(ErrCode);
}// end of calCalibrateRPBG

/*-----------------------------------------------------------------------------
 *
 * Module: calCalibrateRPOS()
 * Description: Calibrate AFE RP offset for single input
 *
 *----------------------------------------------------------------------------*/
errcode_t calCalibrateRPOS(void)
{
    errcode_t       ErrCode;
    ad_dsp_t        rp_signal;
    BYTE            i,saturated;
    SHORT           target_mid,adjustment,bCurOffsetCnt;
    USHORT          RP_step_size[1];
    SBYTE           origRPGN,curRPGN;

    target_mid = CONVERT_DSPOFS_TO_ADCMV(svoCalVar.rpOfsData.all16);
    SendMsgCn(DEBUG_AUTO_TUNING_MSG,2,0x316802,A2B(target_mid));   // RP_Offset_CALIBRATE_START!!

    origRPGN = curRPGN = READ_FIELD(HAL_AFE_RPGN);
    curRPGN = (curRPGN > 6)? curRPGN - 6 : 0;
    WRITE_FIELD(HAL_AFE_RPGN,curRPGN);
    do
    {
        bCurOffsetCnt = calDacCount2SBYTE(READ_FIELD(HAL_AFE_RPOS),8);

        ErrCode = calCalibrateStepsize(RP_OS,
                                       RP_GN,
                                       TYPE3,
                                       0x00,
                                       RP_step_size);

        WRITE_FIELD(HAL_AFE_RPOS,calSBYTE2DacCount(bCurOffsetCnt,8));
        bCurOffsetCnt = calDacCount2SBYTE(READ_FIELD(HAL_AFE_RPOS),8);

        saturated = FALSE;
        i = 0;
        while ((i++ < RPG_MAX_LOOP) && (ErrCode == eNOERR))
        {
            rp_signal = ReadFilteredDSPADCmV(ONE_REVOLUTION, RP_ALL, ADC_RP, TRUE);

            SendMsgCn(DEBUG_AUTO_TUNING_MSG,1,0x316815,A1B(i));
            // RPOS = %1d, RP_MID = %2d (mV)
            SendMsgCn(DEBUG_AUTO_TUNING_MSG,3,0x316818,A1B(bCurOffsetCnt),A2B(rp_signal.middle_mv));
            if (rp_signal.errcode != eNOERR)
            {
                SendMsgCn(SHOW_DEBUG_MSG,1,0x31682F,rp_signal.errcode);
                ErrCode = rp_signal.errcode;
                break;
            }
            adjustment = RoundDivide((LONG)(target_mid - rp_signal.middle_mv) * 256, RP_step_size[0]);
            if ((rp_signal.top_saturated == TRUE) || (rp_signal.bot_saturated == TRUE))
            {
                adjustment = adjustment * 3 / 2;
            }
            bCurOffsetCnt += READ_FIELD(HAL_AFE_RPINV)? -adjustment : adjustment;

            if (bCurOffsetCnt > 127)
            {
                bCurOffsetCnt = 127;
                ErrCode = eERR_adj_next_offset_over_high_limit;
            }

            if (bCurOffsetCnt < -127)
            {
                bCurOffsetCnt = -127;
                ErrCode = eERR_adj_next_offset_over_low_limit;
            }

            WRITE_FIELD(HAL_AFE_RPOS,calSBYTE2DacCount(bCurOffsetCnt,8));

            if ((rp_signal.top_saturated == TRUE) || (rp_signal.bot_saturated == TRUE))
            {
                //------------------------------------------------------------------;
                // If saturation occurs, then lower the gain to remove this problem ;
                //------------------------------------------------------------------;
                SendMsg80(SHOW_DEBUG_MSG,0x310607); // WARNING: RP Saturated
#if (Customer_A1 ==1)
			PositionHomeErrorCount++;	
		#endif
                
                if ((curRPGN > 0) && (rp_signal.p2p_mv > (GPADC_IN_RANGE * 2 / 3)))
                {
                    saturated = TRUE;
                    //reduce the RPGN
                    curRPGN -= RPGN_STEP_SIZE;
                    if (curRPGN < 0) curRPGN = 0;
                    WRITE_FIELD(HAL_AFE_RPGN,curRPGN);
                    break;
                }
            }
            else
            {
                break;
            }
        }
    }
    while ((saturated == TRUE) && (ErrCode == eNOERR));

    if (i > RPG_MAX_LOOP)
        ErrCode = eLOOP_expire_calibrate_TE_gain;

    last_cal_result = calSBYTE2DacCount(bCurOffsetCnt,8);

    //restore RPGN
    WRITE_FIELD(HAL_AFE_RPGN,origRPGN);

    return(ErrCode);
}// end of calCalibrateRPOS

/*-----------------------------------------------------------------------------
 *
 * Module: calCalibrateRPGnOs()
 * Description: Calibrate AFE RP gain and offset
 *
 *----------------------------------------------------------------------------*/
errcode_t calCalibrateRPGnOs(USHORT target_p2p)
{
    ad_dsp_t        rp_signal;
    BYTE            i;
    SBYTE           bCurGainCnt;
    SHORT           target_mid,adjustment,bCurOffsetCnt;
    errcode_t       ErrCode;
    USHORT          RP_step_size[0x1F];

    target_mid = CONVERT_DSPOFS_TO_ADCMV(svoCalVar.rpOfsData.all16);
    SendMsgCn(DEBUG_AUTO_TUNING_MSG,4,0x316811,A2B(target_mid),A2B(target_p2p));
    //---------------------------------------------------;
    // initialize RPG                                    ;
    //---------------------------------------------------;
    bCurGainCnt = READ_FIELD(HAL_AFE_RP2GN);
    bCurOffsetCnt = calDacCount2SBYTE(READ_FIELD(HAL_AFE_RP2OS),8);

    ErrCode = calCalibrateStepsize(RP2_OS,
                                   RP_GN,
                                   TYPE3,
                                   0x1F,
                                   RP_step_size);

    i = 0;
    while ((i++ < RPG_MAX_LOOP) && (ErrCode == eNOERR))
    {
        rp_signal = ReadFilteredDSPADCmV(ONE_REVOLUTION, RP_ALL, ADC_RP, TRUE);
        SendMsgCn(DEBUG_AUTO_TUNING_MSG,1,0x316815,A1B(i));//RPG_LOOP==>
        // RPGN = %d, RPOS = %d, RP_MID = %2d (mV), RP_P2P = %2d (mV)
        SendMsgCn(DEBUG_AUTO_TUNING_MSG,6,0x316817,A1B(bCurGainCnt),A1B(bCurOffsetCnt),
            A2B(rp_signal.middle_mv),A2B(rp_signal.p2p_mv));
        if (rp_signal.errcode != eNOERR)
        {
            SendMsgCn(SHOW_DEBUG_MSG,1,0x31682F,rp_signal.errcode);
            ErrCode = rp_signal.errcode;
            break;
        }
        last_cal_recd = (rp_signal.bot_mv > RPG_RECD_CHK_TH);
        adjustment = RoundDivide((LONG)(target_mid - rp_signal.middle_mv) * 256, RP_step_size[bCurGainCnt]);
        bCurOffsetCnt += READ_FIELD(HAL_AFE_RPINV)? -adjustment : adjustment;
        if (bCurOffsetCnt > 127)
        {
            bCurOffsetCnt = 127;
            ErrCode = eERR_adj_next_offset_over_high_limit;
        }
        if (bCurOffsetCnt < -127)
        {
            bCurOffsetCnt = -127;
            ErrCode = eERR_adj_next_offset_over_low_limit;
        }
        WRITE_FIELD(HAL_AFE_RP2OS,calSBYTE2DacCount(bCurOffsetCnt,8));

        if ((rp_signal.top_saturated == TRUE) || (rp_signal.bot_saturated == TRUE))
        {
            //------------------------------------------------------------------;
            // If saturation occurs, then lower the gain to remove this problem ;
            //------------------------------------------------------------------;
            SendMsg80(SHOW_DEBUG_MSG,0x310607); // WARNING: RP Saturated
#if (Customer_A1 ==1)
			PositionHomeErrorCount++;	
		#endif
            
            if (bCurGainCnt == 0)
            {
                ErrCode = eLIMIT_rpg_calibrate_RP_gain;
                break;
            }
            else
            {
                if (bCurGainCnt > RPG_DB_REDUCE_AT_SAT)
                {
                    bCurGainCnt = bCurGainCnt - RPG_DB_REDUCE_AT_SAT;
                }
                else
                {
                    bCurGainCnt = 0;
                }
            }
            WRITE_FIELD(HAL_AFE_RP2GN,bCurGainCnt);
        }
        else
        {
            bCurGainCnt = calBestGain(RP_step_size, bCurGainCnt, 0x1F, rp_signal.p2p_mv, target_p2p);
            WRITE_FIELD(HAL_AFE_RP2GN,bCurGainCnt);
            if (i >= MIN_RPBG_LOOP)
                break;
        }
    }

    if (i > RPG_MAX_LOOP)
        ErrCode = eLOOP_expire_calibrate_TE_gain;

    last_cal_result = bCurGainCnt;
    last_cal_result2 = calSBYTE2DacCount(bCurOffsetCnt,8);

    return(ErrCode);
}// end of calCalibrateRPGnOs

/*-----------------------------------------------------------------------------
 *
 * Module: calCalibrateTEBGTROOS()
 * Description: calibrate TROOS based on CE(MPP based)
 *              calibrate TEBG
 *              measure CE sensitivity
 *
 *----------------------------------------------------------------------------*/
#define TEBG_POINTS (((TEBG_CAL_END_POINT-TEBG_CAL_BEGIN_POINT)/TEBG_CAL_STEP)+1)
#define TROOS_SHIFT_CDDVD  ((LONG)((LENS_SHIFT_UM + (CD_DVD_TRO_SENSITIVITY / 2)) / CD_DVD_TRO_SENSITIVITY))
#define TROOS_LIMIT_CDDVD ((SHORT)((TROOS_LENS_LIMIT_CDDVD + (CD_DVD_TRO_SENSITIVITY / 2)) / CD_DVD_TRO_SENSITIVITY))
#if (ENABLE_HDDVD == 1) || (BD_ENABLE == 1)
#define TROOS_SHIFT_BDHD   ((LONG)((LENS_SHIFT_UM + (BD_HD_TRO_SENSITIVITY / 2)) / BD_HD_TRO_SENSITIVITY))
#define TROOS_LIMIT_BDHD  ((SHORT)((TROOS_LENS_LIMIT_BDHD  + (BD_HD_TRO_SENSITIVITY / 2)) / BD_HD_TRO_SENSITIVITY))
#endif

errcode_t calCalibrateTEBGTROOS(USHORT target_mv, USHORT *ce_sen_mv_mm, BYTE cal_en)
{
    errcode_t ErrCode;
    ad_dsp_t te_signal;
    SBYTE    tebg,tebg_shift;
    SBYTE    index,troos,troos_mid,troos_limit,troos_shift;
    BOOL     saturated;
    BYTE      orig_TRKG,orig_TELpfBW,phase,retries,cal_ok;
#if (CHIP_REV < 0xC0)
    BYTE      orig_CELPF;
#endif

    HAL_SERVO_CenterControlOFF();

    /* Force to use NORMAL gain register, in case RECD toggle during calibration */
    GAIN_RECD_HIGH;

#if (DVD_RAM_READ == 1)
    // Force LAND to low (a) to use GROOVE's registers
    //                   (b) to prevent from TE polarity change
    if (DVDRAM)
    {
        WRITE_FIELD(HAL_AFE_LANDSEL,0);
    }
#endif

    //-------------------------------------------------------------/
    // preset a lower TE gain and TELPF to prevent TE saturation   /
    //-------------------------------------------------------------/
    orig_TRKG = READ_FIELD(HAL_AFE_TRKG);
#if (APP_TE_SUPPORT == 1)
    if (((READ_REG(HAL_AFEREG_MAP->MB16) & (HAL_AFE_MPPAH|HAL_AFE_MPPBG)) == (HAL_AFE_MPPAH|HAL_AFE_MPPBG)) &&
        ((READ_REG(HAL_AFEREG_MAP->MB15) & (HAL_AFE_MPPA|HAL_AFE_MPPB|HAL_AFE_MPPC|HAL_AFE_MPPD)) == (HAL_AFE_MPPA|HAL_AFE_MPPB)))
    {
        WRITE_FIELD(HAL_AFE_TRKG,(orig_TRKG > 8)? orig_TRKG-8: 0);
    }
    else
#endif
    {
        WRITE_FIELD(HAL_AFE_TRKG,(orig_TRKG > 3)? orig_TRKG-3: 0);
    }
    orig_TELpfBW = HAL_AFE_GetTELpfBW();
    HAL_AFE_SetTELpfBW(e_TELPF_15);

    //-------------------------------------------------------------/
    // preset a lower CELPF to reduce CE noise                     /
    //-------------------------------------------------------------/
#if (CHIP_REV < 0xC0)
    orig_CELPF = READ_REG(HAL_AFEREG_MAP->FCTS36) & HAL_AFE_CEFCSFSEL_MASK;
    READ_MOD_WRITE(HAL_AFEREG_MAP->FCTS36,HAL_AFE_CEFCSFSEL_MASK,(halAFE_CELPF_1kHz << HAL_AFE_CEFCSFSEL_LSB));
#else
//Bug #5659: use CELPF filter in DSP
//    WRITE_DSP_COEF(kc20, CELPF_1kHz[0]);
//    WRITE_DSP_COEF(kc21, CELPF_1kHz[1]);
//    WRITE_DSP_COEF(kc22, CELPF_1kHz[2]);
#endif

    //set MPPO to MUX1ADC
    WRITE_FIELD(HAL_AFE_MUX1ADSEL, halAFE_MUX1ADSEL_MPPO);

    //-------------------------------------------------------------/
    // TRO output controled by TRODAT                              /
    //-------------------------------------------------------------/
    troos = troos_mid = READ_FIELD(HAL_SRV_TROFFSET);
    last_cal_result = READ_FIELD(HAL_AFE_TEBG);

#if (ENABLE_HDDVD == 1) || (BD_ENABLE == 1)
    if ((svoVar_discKindT.Bit.DiscStd == eDiscBD) || (svoVar_discKindT.Bit.DiscStd == eDiscHD))
    {
        troos_shift = TROOS_SHIFT_BDHD;
        troos_limit = TROOS_LIMIT_BDHD;
    }
    else
#endif
    {
        troos_shift = TROOS_SHIFT_CDDVD;
        troos_limit = TROOS_LIMIT_CDDVD;
    }

    SendMsgCn(DEBUG_AUTO_TUNING_MSG,2,0x316675,A1B(troos_shift),A1B(troos_limit));

    saturated = FALSE;
    ErrCode = eNOERR;
    retries = 10;
    cal_ok = 0;
    while (retries > 0)
    {
        for (phase = 0; phase < 2; phase++)
        {
            if (phase == 0)
            {
                //-------------------------------------------------------------/
                // move lens inward smoothly (TRO_INVERSION = 0)               /
                //-------------------------------------------------------------/
                while (troos < (troos_mid+troos_shift))
                {
                    troos++;
                    WRITE_FIELD(HAL_SRV_TROFFSET,troos);
                    Delay100uS(LENS_SHIFT_T/troos_shift);
                }
            }
            else
            {
                //-------------------------------------------------------------/
                // move lens outward smoothly (TRO_INVERSION = 0)              /
                //-------------------------------------------------------------/
                while (troos > (troos_mid-troos_shift))
                {
                    troos--;
                    WRITE_FIELD(HAL_SRV_TROFFSET,troos);
                    Delay100uS(LENS_SHIFT_T/troos_shift);
                }
            }
            WaitTimer(MS_25);

            index = 0;
            if ((cal_en & CALIB_TEBG) && (saturated == FALSE))
            {
                tebg_shift = (svoCalGnT3Var[Layer_Index][MPPB_GN].normal_gain - svoCalGnT3Var[Layer_Index][SPP1B_GN].normal_gain) / 2;

                if (tebg_shift > (0x1F - TEBG_CAL_BEGIN_POINT)) tebg_shift = (0x1F - TEBG_CAL_BEGIN_POINT);
                if (tebg_shift < (-TEBG_CAL_END_POINT)) tebg_shift = (-TEBG_CAL_END_POINT);

                for (tebg = tebg_shift + TEBG_CAL_BEGIN_POINT; tebg >= tebg_shift + TEBG_CAL_END_POINT; tebg += TEBG_CAL_STEP)
                {
                    WRITE_FIELD(HAL_AFE_TEBG, tebg);

                    //simulation shows sending (TEBG+12)^2 to curve fit can lower the error from +-0.7 to +-0.1!
                    svoCurvefitCalVar.item[index] = (tebg + 12) * (tebg + 12);
                    WaitTimer(MS_1);

#if (DVD_RAM_READ == 1)
                    // Headers cause noise on TE in DVD-RAM rewritable area.
                    // Use average method to minimize the header noise.
                    if (DVDRAM_rewritable)
                    {
                        te_signal = ReadGPADCmV_Avg(ONE_REVOLUTION, ADC_TE, TRUE);
                    }
                    else
#endif
                    {
                        te_signal = ReadFilteredDSPADCmV(ONE_REVOLUTION, TE_ALL, ADC_TE, TRUE);
                    }

                    if (te_signal.errcode != eNOERR)
                    {
                        break;
                    }

                    if ((te_signal.top_saturated == TRUE) || (te_signal.bot_saturated == TRUE))
                    {
                        saturated = TRUE;
                        break;
                    }

                    if (phase == 0)
                    {
                        svoCurvefitCalVar.cost[index] = te_signal.middle_mv;
                    }
                    else
                    {
                        svoCurvefitCalVar.cost[index] -= te_signal.middle_mv;
                    }
                    index++;
                }

                if (te_signal.errcode != eNOERR)
                {
                    break;
                }

                if (saturated)
                {
                    //-------------------------------------------------------------/
                    // Saturation happen. smaller the gain                         /
                    //-------------------------------------------------------------/
                    SendMsg80(SHOW_DEBUG_MSG,0x310608); // WARNING: TEBG_cal Saturated
                    index = READ_FIELD(HAL_AFE_TRKG);

                    if (index >= 1)
                    {
                        index--; // reduce 1 dB
                        WRITE_FIELD(HAL_AFE_TRKG,index);
                        saturated = FALSE;
                        break;
                    }
                }
                if (phase == 1)
                {
                    cal_ok |= CALIB_TEBG;
                }
            }

            if (phase == 0)
            {
                index = TEBG_POINTS;
            }
            else
            {
                index = TEBG_POINTS + 1;
            }

            if (cal_en & CALIB_CEGN)
            {
#if (CHIP_REV < 0xC0)
                WRITE_FIELD(HAL_SRV_TSVFWD, 0); //set TE source to CE
                te_signal = ReadFilteredDSPADCmV(ONE_REVOLUTION, TE_ALL, ADC_MUXOUT1, TRUE);
                WRITE_REG(HAL_SRVREG_MAP->PUSTP, 0x00); //set TE source back to TE
#else
                seekSetCEEq(eRoughSeekRatioOnly, 0);
                HAL_SERVO_CenterControlON(FORW);
                te_signal = ReadDSP_CE_mV_Avg(ONE_REVOLUTION, TRUE);
                HAL_SERVO_CenterControlOFF();
#endif
                if (te_signal.errcode != eNOERR)
                {
                    break;
                }

                if ((te_signal.top_saturated == TRUE) || (te_signal.bot_saturated == TRUE))
                {
                    SendMsg80(SHOW_DEBUG_MSG,0x31060A);//WARNING: CE Saturated
								#if (Customer_A1 ==1)
										PositionHomeErrorCount++;	
								#endif                    
                    if (READ_FIELD(HAL_AFE_CEGN) >= 1)
                    {
                        index = READ_FIELD(HAL_AFE_CEGN)-1;
                        WRITE_FIELD(HAL_AFE_CEGN,index);// reduce 1 dB
                        saturated = FALSE;
                        break;
                    }
                }
                //CALIB_CEGN
                svoCurvefitCalVar.cost[index+2] = te_signal.middle_mv;//CE
                if (phase == 1)
                {
                    cal_ok |= CALIB_CEGN;
                }
            }

            if (cal_en & CALIB_TROOS)
            {
    #if (APP_TE_SUPPORT == 1)
                WRITE_FIELD(HAL_AFE_MUX1ADSEL, halAFE_MUX1ADSEL_MPPO);//set MUX1ADC to MPPO
                calSetBalGain(MPPB_GN, 0x10);
                if (((READ_REG(HAL_AFEREG_MAP->MB16) & (HAL_AFE_MPPAH|HAL_AFE_MPPBG)) == (HAL_AFE_MPPAH|HAL_AFE_MPPBG)) &&
                    ((READ_REG(HAL_AFEREG_MAP->MB15) & (HAL_AFE_MPPA|HAL_AFE_MPPB|HAL_AFE_MPPC|HAL_AFE_MPPD)) == (HAL_AFE_MPPA|HAL_AFE_MPPB)))
                {
                    //if MPP is set to H - G for APP, change it back to (A + D) - (B + C) for TRO offset calibratio
                    READ_MOD_WRITE(HAL_AFEREG_MAP->MB16, HAL_AFE_MPPAH|HAL_AFE_MPPBG, 0);
                    READ_MOD_WRITE(HAL_AFEREG_MAP->MB15, HAL_AFE_MPPA|HAL_AFE_MPPB|HAL_AFE_MPPC|HAL_AFE_MPPD, HAL_AFE_MPPA|HAL_AFE_MPPB|HAL_AFE_MPPC|HAL_AFE_MPPD);
                    WRITE_FIELD(HAL_AFE_MPPOSA, svoVar_mpp_os);
                    te_signal = ReadGPADCmV(ONE_REVOLUTION, ADC_MUXOUT1, TRUE);
                    READ_MOD_WRITE(HAL_AFEREG_MAP->MB16, HAL_AFE_MPPAH|HAL_AFE_MPPBG, HAL_AFE_MPPAH|HAL_AFE_MPPBG);
                    READ_MOD_WRITE(HAL_AFEREG_MAP->MB15, HAL_AFE_MPPA|HAL_AFE_MPPB|HAL_AFE_MPPC|HAL_AFE_MPPD, HAL_AFE_MPPA|HAL_AFE_MPPB);
                    WRITE_FIELD(HAL_AFE_MPPOSA, svoCalOfsT2Var[Layer_Index][MPP_OS].normal_offset);
                }
                else
    #endif
                {
                    te_signal = ReadGPADCmV(ONE_REVOLUTION, ADC_MUXOUT1, TRUE);
                }

                if (te_signal.errcode != eNOERR)
                {
                    break;
                }
                if ((te_signal.top_saturated == TRUE) || (te_signal.bot_saturated == TRUE))
                {
                    SendMsg80(SHOW_DEBUG_MSG,0x310611);//WARNING: MPP Saturated
								#if (Customer_A1 ==1)
										PositionHomeErrorCount++;	
								#endif	                    
                }
                svoCurvefitCalVar.cost[index] = te_signal.middle_mv;//MPPO
                svoCurvefitCalVar.item[index] = troos;
                if (phase == 1)
                {
                    cal_ok |= CALIB_TROOS;
                }
            }

        }

        if ((phase >= 2) || (te_signal.errcode != eNOERR))
        {
            //cal ok or error
            break;
        }
        retries--;
    }

    if (te_signal.errcode != eNOERR)
    {
        ErrCode = te_signal.errcode;
    }

    //-------------------------------------------------------------/
    // restore TE gain, TELPF, and CELPF                           /
    //-------------------------------------------------------------/
    WRITE_FIELD(HAL_AFE_TRKG, orig_TRKG);
    HAL_AFE_SetTELpfBW(orig_TELpfBW);
#if (CHIP_REV < 0xC0)
    READ_MOD_WRITE(HAL_AFEREG_MAP->FCTS36,HAL_AFE_CEFCSFSEL_MASK,orig_CELPF);
#else
//Bug #5659: use CELPF filter in DSP
//    WRITE_DSP_COEF(kc20, CELPF_allpass[0]);
//    WRITE_DSP_COEF(kc21, CELPF_allpass[1]);
//    WRITE_DSP_COEF(kc22, CELPF_allpass[2]);
#endif

    if (cal_en & CALIB_TEBG)
    {
        if (saturated)
        {
            //calibration fail, use original TEBG
            ErrCode = eLIMIT_trkg_sppg_calibration;
        }
        else if (cal_ok & CALIB_TEBG)
        {
            CurveFit(TEBG_POINTS, svoCurvefitCalVar.cost, svoCurvefitCalVar.item, svoCurvefitCalVar.cof);
            tebg = (SBYTE)(fsqrt(svoCurvefitCalVar.cof[0]) + (0.5 - 12));
            if (tebg > tebg_shift + TEBG_CAL_BEGIN_POINT)
            {
                SendMsgCn(SHOW_DEBUG_MSG,1,0x31060D,tebg);
                tebg = tebg_shift + TEBG_CAL_BEGIN_POINT;
                ErrCode = eERR_adj_next_gain_high_limit;
            }
            else if (tebg < tebg_shift + TEBG_CAL_END_POINT)
            {
                SendMsgCn(SHOW_DEBUG_MSG,1,0x31060D,tebg);
                tebg = tebg_shift + TEBG_CAL_END_POINT;
                ErrCode = eERR_adj_next_gain_low_limit;
            }

            last_cal_result = tebg;
#if 0 /* for debug */
            for (index = 0; index < TEBG_POINTS; index++)
            {
                send_msg74(SHOW_DEBUG_MSG,'s','p','p',svoCurvefitCalVar.item[index]);
            }
            for (index = 0; index < TEBG_POINTS; index++)
            {
                send_msg74(SHOW_DEBUG_MSG,'d','T','E',svoCurvefitCalVar.cost[index]);
            }
            send_msg74(SHOW_DEBUG_MSG,'o','p','t',fsqrt(svoCurvefitCalVar.cof[0])-12);
#endif
        }
        WRITE_FIELD(HAL_AFE_TEBG, last_cal_result);
    }

    GAIN_RECD;

    if (cal_en & CALIB_CEGN)
    {
        if (svoCurvefitCalVar.cost[TEBG_POINTS + 3] < svoCurvefitCalVar.cost[TEBG_POINTS + 2])
        {
            SendMsg80(DEBUG_CALIB_RESULT_MSG,0x31060B); // WARNING: CE Polarity Wrong
            *ce_sen_mv_mm = 0xFFFF;//set ce_sen_mv_mm to maximum to have lower CE gain
        }
        else if (cal_ok & CALIB_CEGN)
        {
            *ce_sen_mv_mm = (svoCurvefitCalVar.cost[TEBG_POINTS + 3] - svoCurvefitCalVar.cost[TEBG_POINTS + 2]) * 1000 / (2 * LENS_SHIFT_UM);
        }
        else
        {
            *ce_sen_mv_mm = CE_SEN_TARGET_MV_MM;
        }
        SendMsgCn(DEBUG_CALIB_RESULT_MSG,2,0x31667A,A2B(*ce_sen_mv_mm)); //CE_sensitivity(mV/mm)
    }

    if (cal_en & CALIB_TROOS)
    {
        svoCurvefitCalVar.item[0] = svoCurvefitCalVar.item[TEBG_POINTS];
        svoCurvefitCalVar.cost[0] = svoCurvefitCalVar.cost[TEBG_POINTS];
        svoCurvefitCalVar.item[1] = svoCurvefitCalVar.item[TEBG_POINTS + 1];
        svoCurvefitCalVar.cost[1] = svoCurvefitCalVar.cost[TEBG_POINTS + 1];
        // CE_point_one = %4f (mV), CE_point_two = %4f (mV)
        SendMsgCnB(DEBUG_AUTO_TUNING_MSG,8,0x316677,F4B(svoCurvefitCalVar.cost[0]),F4B(svoCurvefitCalVar.cost[1]));
        //find TROOS
        LineFit(2, svoCurvefitCalVar.item, svoCurvefitCalVar.cost, svoCurvefitCalVar.cof);
        //CE = svoCurvefitCalVar.cof[1] * TROOS + svoCurvefitCalVar.cof[0]
        troos_mid = float2short(((float)target_mv - svoCurvefitCalVar.cof[0]) / svoCurvefitCalVar.cof[1]);
        SendMsgCn(DEBUG_AUTO_TUNING_MSG,1,0x316679,A1B(troos_mid));

#if  SERVOKEYPARAMETER == 1
        svoPrvtVar_troos = troos_mid;
#endif

#if 0 /* for debug */
        for (index = 0; index < 2; index++)
        {
            send_msg74(SHOW_DEBUG_MSG,'t','r','o',svoCurvefitCalVar.item[index]);
        }
        for (index = 0; index < 2; index++)
        {
            send_msg74(SHOW_DEBUG_MSG,'m','p','p',svoCurvefitCalVar.cost[index]);
        }
        send_msg74(SHOW_DEBUG_MSG,'o','p','t',troos_mid);
#endif

        if (ABS(troos_mid) > troos_limit)
        {
				#if (Customer_A1 ==1)
						PositionHomeErrorCount++;	
				#endif        
            SendMsg80(SHOW_DEBUG_MSG,0x310609); // WARNING: TROOS Too Large
            if (troos_mid < 0)
                troos_mid = -troos_limit;
            else
                troos_mid = troos_limit;
        }
    }

    //-------------------------------------------------------------/
    // move lens back to troos result smoothly                     /
    //-------------------------------------------------------------/
    while (troos != troos_mid)
    {
        if (troos > troos_mid)
            troos--;
        else
            troos++;

        WRITE_FIELD(HAL_SRV_TROFFSET,troos);
        Delay100uS(LENS_SHIFT_T / troos_shift);
    }
    last_cal_result2 = troos_mid;

#if (DVD_RAM_READ == 1)
    // Restore LAND control.
    if (DVDRAM)
    {
        WRITE_FIELD(HAL_AFE_LANDSEL,1);
    }
#endif

    return(ErrCode);
}// end of calCalibrateTEBGTROOS

#if (MPPBG_SPPBG_CALIB == 2)
/*-----------------------------------------------------------------------------
 *
 * Module: calCalibrateMPPSPP1BG()
 * Description: Calibrate MPP & SPP1 balance gain
 *
 *----------------------------------------------------------------------------*/
#define MPPSPP1BG_STEP 8    //unit 0.5dB
errcode_t calCalibrateMPPSPP1BG(USHORT target_mv)
{
    errcode_t ErrCode;
    BYTE      i,origMPPBG,origSPP1BG,origMPPOS,origSPP1OS;
    ad_dsp_t  signal_ret;
    SBYTE     dir;

    SendMsgCn(DEBUG_AUTO_TUNING_MSG,2,0x316840,A2B(target_mv));//MPPBG_SPP1BG_CALIBRATE_START!! MPP SPP1 target = %4d (mV)
    ErrCode = eNOERR;
    WRITE_FIELD(HAL_AFE_MUX1ADSEL, halAFE_MUX1ADSEL_MPPO);
    WRITE_FIELD(HAL_AFE_MUX2ADSEL, halAFE_MUX2ADSEL_SPPO1);
    origMPPBG = READ_FIELD(HAL_AFE_MPPOBG);
    origMPPOS = READ_FIELD(HAL_AFE_MPPOSA);
    origSPP1BG = READ_FIELD(HAL_AFE_SPP1BG);
    origSPP1OS = READ_FIELD(HAL_AFE_SPP1OSA);
    last_cal_result = 0x10;
    last_cal_result2 = 0x10;
    for (i = 0; i < 2; i++)
    {
        if (last_cal_result != origMPPBG)
        {
            calSetBalGain(MPPB_GN, last_cal_result);
        }
        if (last_cal_result2 != origSPP1BG)
        {
            calSetBalGain(SPP1B_GN, last_cal_result2);
        }
        signal_ret = readPNmV(ADC_MUXOUT1, ADC_MUXOUT2, ONE_REVOLUTION, TRUE);
        if (signal_ret.errcode != eNOERR)
        {
            ErrCode = signal_ret.errcode;
            break;
        }
        //MPPBG_P?  = %2.1f (dB), MPP_MID  = %4d (mV)
        SendMsgCnB(DEBUG_AUTO_TUNING_MSG,6,0x316841+i,F4B((float)(last_cal_result-16)/2),B2B(signal_ret.top_mv));
        svoCurvefitCalVar.item[0 + i] = last_cal_result;
        svoCurvefitCalVar.cost[0 + i] = signal_ret.top_mv - target_mv;
        last_cal_result += ((signal_ret.top_mv >= target_mv)? MPPSPP1BG_STEP : -MPPSPP1BG_STEP);
        //SPP1BG_P? = %2.1f (dB), SPP1_MID = %4d (mV)
        SendMsgCnB(DEBUG_AUTO_TUNING_MSG,6,0x316843+i,F4B((float)(last_cal_result2-16)/2),B2B(signal_ret.bot_mv));
        svoCurvefitCalVar.item[2 + i] = last_cal_result2;
        svoCurvefitCalVar.cost[2 + i] = signal_ret.bot_mv - target_mv;
        last_cal_result2 += ((signal_ret.bot_mv >= target_mv)? MPPSPP1BG_STEP : -MPPSPP1BG_STEP);
    }
    WRITE_FIELD(HAL_AFE_MPPOBG,origMPPBG);
    WRITE_FIELD(HAL_AFE_MPPOSA,origMPPOS);
    WRITE_FIELD(HAL_AFE_SPP1BG,origSPP1BG);
    WRITE_FIELD(HAL_AFE_SPP1OSA,origSPP1OS);

    if (ErrCode == eNOERR)
    {
        dir = ((svoCurvefitCalVar.cost[0] > 0)? 1 : -1);
        last_cal_result = 0x10 + dir;
        while (1)
        {
            if (linear_interpolation(CalculateRateFromXdB(P50dB,svoCurvefitCalVar.item[0]), svoCurvefitCalVar.cost[0],
                                     CalculateRateFromXdB(P50dB,svoCurvefitCalVar.item[1]), svoCurvefitCalVar.cost[1],
                                     CalculateRateFromXdB(P50dB,last_cal_result)) * dir < 0)
            {
                break;
            }
            last_cal_result += dir;
            if (last_cal_result > 0x1F)
            {
                break;
            }
        }
        last_cal_result -= dir;

        dir = ((svoCurvefitCalVar.cost[2] > 0)? 1 : -1);
        last_cal_result2 = 0x10 + dir;
        while (1)
        {
            if (linear_interpolation(CalculateRateFromXdB(P50dB,svoCurvefitCalVar.item[2]), svoCurvefitCalVar.cost[2],
                                     CalculateRateFromXdB(P50dB,svoCurvefitCalVar.item[3]), svoCurvefitCalVar.cost[3],
                                     CalculateRateFromXdB(P50dB,last_cal_result2)) * dir < 0)
            {
                break;
            }
            last_cal_result2 += dir;
            if (last_cal_result2 > 0x1F)
            {
                break;
            }
        }
        last_cal_result2 -= dir;
    }
    else
    {
        last_cal_result = origMPPBG;
        last_cal_result2 = origSPP1BG;
    }

    return ErrCode;
}
#endif

/*-----------------------------------------------------------------------------
 *
 * Module: calCalibrateCEBG()
 * Description: Calibrate CE balance gain
 *
 *----------------------------------------------------------------------------*/
#define CEGN_STEP 2    //unit dB
#define MAX_CEBG_LOOP 10
errcode_t calCalibrateCEBG(void)
{
    ad_dsp_t ce_signal;
    USHORT   mpp_ce_p2p_mv;
    SHORT    gain_adj;
    BYTE     i,orig_BPCELP;
    SBYTE    orig_CEGN, cur_CEGN;

    orig_CEGN = cur_CEGN = READ_FIELD(HAL_AFE_CEGN);

    SendMsg80(DEBUG_AUTO_TUNING_MSG,0x316830); //CE_Bal_Gain_CALIBRATE_START!!
    //Bypass CELPF to allow TE to pass through
    orig_BPCELP = READ_FIELD(HAL_AFE_BPCELP);
    last_cal_result = READ_FIELD(HAL_AFE_CEBG);
    WRITE_FIELD(HAL_AFE_BPCELP,1);

#if (CHIP_REV >= 0xB2) && (DVD_RAM_READ == 1)
    if (DVDRAM_embossed)
    {
        LAL_WriteCoef(AFE_FCTS35_CEBG12DB_EMBOSSED);
    }
    else
    {
        LAL_WriteCoef(AFE_FCTS35_CEBG12DB);
    }
#endif


    for (i = 0; i < MAX_CEBG_LOOP; i++)
    {
        //measure TE amplitude in CE_MPP
        WRITE_FIELD(HAL_AFE_CESP1,0);
        Delay100uS(8);

        ce_signal = ReadGPADCmV(ONE_REVOLUTION, ADC_CE, TRUE);

        if (ce_signal.errcode != eNOERR)
        {
            SendMsgCn(SHOW_DEBUG_MSG,1,0x31683A,ce_signal.errcode);
            return ce_signal.errcode;
        }
        if ((ce_signal.top_saturated == TRUE) || (ce_signal.bot_saturated == TRUE))
        {
				#if (Customer_A1 ==1)
						PositionHomeErrorCount++;	
				#endif        
            SendMsg80(SHOW_DEBUG_MSG,0x31060A); // WARNING: CE Saturated
            if (cur_CEGN > 0)
            {
                cur_CEGN -= CEGN_STEP;
                if (cur_CEGN <= 0) cur_CEGN = 0;
                WRITE_FIELD(HAL_AFE_CEGN,cur_CEGN);
                continue;
            }
        }
        mpp_ce_p2p_mv = ce_signal.p2p_mv;
        SendMsgCn(DEBUG_AUTO_TUNING_MSG,2,0x316831,A2B(mpp_ce_p2p_mv)); //CE_MPP_P2P(mV)  =

        //measure TE amplitude in CE_SPP1
        WRITE_FIELD(HAL_AFE_CESP1,1);
        WRITE_FIELD(HAL_AFE_CEMPP,0);
        Delay100uS(8);

        ce_signal = ReadGPADCmV(ONE_REVOLUTION, ADC_CE, TRUE);

        if (ce_signal.errcode != eNOERR)
        {
            SendMsgCn(SHOW_DEBUG_MSG,1,0x31683A,ce_signal.errcode);
            return ce_signal.errcode;
        }
        if ((ce_signal.top_saturated == TRUE) || (ce_signal.bot_saturated == TRUE))
        {
				#if (Customer_A1 ==1)
						PositionHomeErrorCount++;	
				#endif        
            SendMsg80(SHOW_DEBUG_MSG,0x31060A); // WARNING: CE Saturated
            if (cur_CEGN > 0)
            {
                cur_CEGN -=  CEGN_STEP;
                if (cur_CEGN <= 0) cur_CEGN = 0;
                WRITE_FIELD(HAL_AFE_CEGN,cur_CEGN);
            }
            else
            {
                break;
            }
        }
        else
        {
            break;
        }
    }
    SendMsgCn(DEBUG_AUTO_TUNING_MSG,2,0x316832,A2B(ce_signal.p2p_mv)); //CE_SPP1_P2P(mV) =
    WRITE_FIELD(HAL_AFE_CEMPP,1);

    //Restore CELPF.
    WRITE_FIELD(HAL_AFE_BPCELP,orig_BPCELP);

    //Restore CEGN.
    WRITE_FIELD(HAL_AFE_CEGN,orig_CEGN);

    if (i < MAX_CEBG_LOOP)
    {
        gain_adj = CalculateXdBFromRate(P50dB, mpp_ce_p2p_mv, ce_signal.p2p_mv);
        last_cal_result += gain_adj;
        SendMsgCn(DEBUG_AUTO_TUNING_MSG,1,0x316833,B1B((last_cal_result - 0x10)/2)); //CE_Bal_Gain(dB) =
        return eNOERR;
    }
    else
    {
        SendMsg80(DEBUG_AUTO_TUNING_MSG,0x316429); //Out of calibrate loop
        return eLOOP_expire_calibrate;
    }
}

/*-----------------------------------------------------------------------------
 *
 * Module: calCalibrateCEOS()
 * Description: Calibrate AFE CE offset
 *
 *----------------------------------------------------------------------------*/
errcode_t calCalibrateCEOS(void)
{
    ad_dsp_t        ce_signal;
    BYTE            i;
    SHORT           target_mid,adjustment,bCurOffsetCnt;
    errcode_t       ErrCode;
    USHORT          CE_step_size[1];

    target_mid = CONVERT_DSPOFS_TO_ADCMV(svoCalVar.ceOfsData.all16);
    SendMsg80(DEBUG_AUTO_TUNING_MSG,0x316834); //CE_Offset_CALIBRATE_START!!

    bCurOffsetCnt = calDacCount2SBYTE(READ_FIELD(HAL_AFE_CEOS),8);

    SendMsgCn(DEBUG_AUTO_TUNING_MSG,2,0x316835,A2B(target_mid));
    ErrCode = calCalibrateStepsize(CE_OS,
                                   CE_GN,
                                   TYPE3,
                                   0x00,
                                   CE_step_size);

#if (CHIP_REV >= 0xC0)
        seekSetCEEq(eRoughSeekRatioOnly, 0);
        HAL_SERVO_CenterControlON(FORW);
#endif

    i = 0;
    while ((i++ < RPG_MAX_LOOP) && (ErrCode == eNOERR))
    {
#if (CHIP_REV >= 0xC0)
        ce_signal = ReadDSP_CE_mV_Avg(ONE_REVOLUTION, TRUE);
#else
        ce_signal = ReadGPADCmV(ONE_REVOLUTION, ADC_CE, TRUE);
#endif
        SendMsgCnB(DEBUG_AUTO_TUNING_MSG,5,0x316836,B1B(i),B2B(bCurOffsetCnt*5),B2B(ce_signal.middle_mv));

        if (ce_signal.errcode != eNOERR)
        {
            SendMsgCn(SHOW_DEBUG_MSG,1,0x316839,ce_signal.errcode);
            ErrCode = ce_signal.errcode;
            break;
        }
        adjustment = RoundDivide((LONG)(target_mid - ce_signal.middle_mv) * 256, CE_step_size[0]);
        bCurOffsetCnt += READ_FIELD(HAL_AFE_CEINV)? -adjustment : adjustment;
        if (bCurOffsetCnt > 127)
        {
            bCurOffsetCnt = 127;
            ErrCode = eERR_adj_next_offset_over_high_limit;
        }
        if (bCurOffsetCnt < -127)
        {
            bCurOffsetCnt = -127;
            ErrCode = eERR_adj_next_offset_over_low_limit;
        }
        WRITE_FIELD(HAL_AFE_CEOS,calSBYTE2DacCount(bCurOffsetCnt,8));

        if ((ce_signal.top_saturated == TRUE) || (ce_signal.bot_saturated == TRUE))
        {
				#if (Customer_A1 ==1)
						PositionHomeErrorCount++;	
				#endif        
            //------------------------------------------------------------------;
            // If saturation occurs, then lower the gain to remove this problem ;
            //------------------------------------------------------------------;
            SendMsg80(SHOW_DEBUG_MSG,0x31060A); // WARNING: CE Saturated
        }
        else
        {
            break;
        }
    }
#if (CHIP_REV >= 0xC0)
    HAL_SERVO_CenterControlOFF();
#endif

    if (i > RPG_MAX_LOOP)
        ErrCode = eLOOP_expire_calibrate_TE_gain;

    last_cal_result = calSBYTE2DacCount(bCurOffsetCnt,8);

    return(ErrCode);
}

/*-----------------------------------------------------------------------------
 *
 * Module: calGetGainVref()
 * Description: Find the reference level of the selected gain
 *
 *----------------------------------------------------------------------------*/
SHORT calGetGainVref(ULONG addr,BYTE mask,BYTE start_bit,BYTE monitor_signal)
{
    SHORT Vref;
    BYTE  max_gn,orig_LPF,orig_gn;

    //set highest LPF to prevent slow respose, change polarity when signal is inverted.
    switch (monitor_signal)
    {
        case ADC_TE:
            orig_LPF = READ_REG(HAL_AFEREG_MAP->FCTS38) & (HAL_AFE_TEFCSFSEL_MASK | HAL_AFE_TELPSET_MASK);
        #if (CHIP_REV >= 0xC1)
            READ_MOD_WRITE(HAL_AFEREG_MAP->FCTS38,(HAL_AFE_TEFCSFSEL_MASK | HAL_AFE_TELPSET_MASK),(halAFE_TELPF_300kHz << HAL_AFE_TEFCSFSEL_LSB) | HAL_AFE_TELPSET);
        #else
            READ_MOD_WRITE(HAL_AFEREG_MAP->FCTS38,(HAL_AFE_TEFCSFSEL_MASK | HAL_AFE_TELPSET_MASK),(halAFE_TELPF_330kHz << HAL_AFE_TEFCSFSEL_LSB) | HAL_AFE_TELPSET);
        #endif
            break;

        case ADC_FE:
            orig_LPF = READ_REG(HAL_AFEREG_MAP->FCTS32) & (HAL_AFE_FEFCSFSEL_MASK | HAL_AFE_FELPSET_MASK);
        #if (CHIP_REV >= 0xC1)
            READ_MOD_WRITE(HAL_AFEREG_MAP->FCTS32,(HAL_AFE_FEFCSFSEL_MASK | HAL_AFE_FELPSET_MASK),(halAFE_FELPF_300kHz << HAL_AFE_FEFCSFSEL_LSB) | HAL_AFE_FELPSET);
        #else
            READ_MOD_WRITE(HAL_AFEREG_MAP->FCTS32,(HAL_AFE_FEFCSFSEL_MASK | HAL_AFE_FELPSET_MASK),(halAFE_FELPF_330kHz << HAL_AFE_FEFCSFSEL_LSB) | HAL_AFE_FELPSET);
        #endif
            break;

        case ADC_CE:
            orig_LPF = READ_REG(HAL_AFEREG_MAP->FCTS36) & HAL_AFE_CEFCSFSEL_MASK;
            READ_MOD_WRITE(HAL_AFEREG_MAP->FCTS36,HAL_AFE_CEFCSFSEL_MASK,(halAFE_CELPF_50kHz << HAL_AFE_CEFCSFSEL_LSB));
            break;

        case ADC_BS:
            orig_LPF = READ_REG(HAL_AFEREG_MAP->BSRP09) & (HAL_AFE_BSLP_MASK | HAL_AFE_BSLPSET_MASK);
            #if(CHIP_REV <= 0xB3)
            READ_MOD_WRITE(HAL_AFEREG_MAP->BSRP09,(HAL_AFE_BSLP_MASK | HAL_AFE_BSLPSET_MASK),(halAFE_BSLPF_330kHz << HAL_AFE_BSLP_LSB) | HAL_AFE_BSLPSET);
            #else
            READ_MOD_WRITE(HAL_AFEREG_MAP->BSRP09,(HAL_AFE_BSLP_MASK | HAL_AFE_BSLPSET_MASK),(halAFE_BSLPF_300kHz << HAL_AFE_BSLP_LSB) | HAL_AFE_BSLPSET);
            #endif
            break;

        case ADC_RP:
            orig_LPF = READ_REG(HAL_AFEREG_MAP->BSRP14) & (HAL_AFE_RPLP_MASK | HAL_AFE_RPLPSET_MASK);
            #if(CHIP_REV <= 0xB3)
            READ_MOD_WRITE(HAL_AFEREG_MAP->BSRP14,(HAL_AFE_RPLP_MASK | HAL_AFE_RPLPSET_MASK),(halAFE_RPLPF_330kHz << HAL_AFE_RPLP_LSB) | HAL_AFE_RPLPSET);
            #else
            READ_MOD_WRITE(HAL_AFEREG_MAP->BSRP14,(HAL_AFE_RPLP_MASK | HAL_AFE_RPLPSET_MASK),(halAFE_RPLPF_300kHz << HAL_AFE_RPLP_LSB) | HAL_AFE_RPLPSET);
            #endif
            break;

        case ADC_SPECIAL_RFDFE:
            orig_LPF = READ_FIELD(HAL_AFE_RFGN);
            WRITE_FIELD(HAL_AFE_RFGN, 0);
            break;
    }

    //-----------------------------------------------------------------------;
    // GAIN = minimum, monitor_signal = A
    // GAIN = maximum, monitor_signal = B
    // GAIN = -infinity dB, monitor_signal = C
    // (A - C) / minimum = (B - C) / maximum
    // (maximum / minimum) * C - C = (maximum / minimum) * A - B
    // C = ((maximum / minimum) * A - B) / ((maximum / minimum) - 1)
    // C = (maximum * A - minimum * B) / (maximum - minimum)
    //-----------------------------------------------------------------------;
    max_gn = (mask >> start_bit);
    orig_gn = READ_ADR_1B(addr);
    READ_MOD_WRITE(*(BYTE *)addr,mask,max_gn << start_bit);
    Delay100uS((monitor_signal == ADC_SPECIAL_RFDFE)? 1 : 8);
    if (monitor_signal == ADC_SPECIAL_RFDFE)
    {
        Vref = readADCmVPerRevs(NO_REVOLUTION, FALSE, RF_ADC).middle_mv;
    }
    else
    {
        Vref = ReadAverageAdcmV(monitor_signal, 7);//B
    }
    READ_MOD_WRITE(*(BYTE *)addr,mask,0);
    Delay100uS((monitor_signal == ADC_SPECIAL_RFDFE)? 1 : 8);
    if (monitor_signal == ADC_SPECIAL_RFDFE)
    {
        Vref = ((readADCmVPerRevs(NO_REVOLUTION, FALSE, RF_ADC).middle_mv * svoRRFVar.step_size[max_gn] - Vref * svoRRFVar.step_size[0]) / (svoRRFVar.step_size[max_gn] - svoRRFVar.step_size[0]));
    }
    else
    {
        Vref = (SHORT)(((float)ReadAverageAdcmV(monitor_signal, 7) * CalculateRateFromXdB(ONEdB,max_gn) - (float)Vref) / (CalculateRateFromXdB(ONEdB,max_gn) - 1.0));
    }
    WRITE_ADR_1B(addr,orig_gn);

    switch (monitor_signal)
    {
        case ADC_TE:
            READ_MOD_WRITE(HAL_AFEREG_MAP->FCTS38,(HAL_AFE_TEFCSFSEL_MASK | HAL_AFE_TELPSET_MASK),orig_LPF);
            break;

        case ADC_FE:
            READ_MOD_WRITE(HAL_AFEREG_MAP->FCTS32,(HAL_AFE_FEFCSFSEL_MASK | HAL_AFE_FELPSET_MASK),orig_LPF);
            break;

        case ADC_CE:
            READ_MOD_WRITE(HAL_AFEREG_MAP->FCTS36,HAL_AFE_CEFCSFSEL_MASK,orig_LPF);
            break;

        case ADC_BS:
            READ_MOD_WRITE(HAL_AFEREG_MAP->BSRP09,(HAL_AFE_BSLP_MASK | HAL_AFE_BSLPSET_MASK),orig_LPF);
            break;

        case ADC_RP:
            READ_MOD_WRITE(HAL_AFEREG_MAP->BSRP14,(HAL_AFE_RPLP_MASK | HAL_AFE_RPLPSET_MASK),orig_LPF);
            break;

        case ADC_SPECIAL_RFDFE:
            WRITE_FIELD(HAL_AFE_RFGN, orig_LPF);
            break;
    }

    return Vref;
}

#if (DPD_COMPENSATE_OFFSET == 1)
/*-----------------------------------------------------------------------------
 *
 * Module: calSetDPDTEOS()
 * Description: set AFE DPD TE offset
 *
 *----------------------------------------------------------------------------*/
void calSetDPDTEOS(BYTE layer)
{
    static BYTE pre_dpd_bank,pre_dpd_fc,pre_layer,pre_status;
    BYTE        temp_dpd_bank,temp_dpd_fc;

    temp_dpd_bank = svoDpdReg2Bank[READ_FIELD(HAL_AFE_DPDHSISCL)];
    temp_dpd_fc = READ_FIELD(HAL_AFE_DPDHSFCH);

    if ((temp_dpd_fc != pre_dpd_fc) || (temp_dpd_bank != pre_dpd_bank) || (layer != pre_layer) ||
        (svoCalDpdVar[layer][temp_dpd_bank][temp_dpd_fc].status != pre_status))
    {
        pre_dpd_fc = temp_dpd_fc;
        pre_dpd_bank = temp_dpd_bank;
        pre_layer = layer;
        pre_status = svoCalDpdVar[layer][temp_dpd_bank][temp_dpd_fc].status;
    #if (DVD_RAM_READ == 1)
        if (DVDRAM)
        {
            WRITE_FIELD(HAL_AFE_TRKO, calSBYTE2DacCount(svoCalDpdVar[layer][temp_dpd_bank][temp_dpd_fc].dpd_offset
                                     +calDacCount2SBYTE(svoCalOfsT1Var[layer][TE_OS].li_offset,8),8));
            WRITE_FIELD(HAL_AFE_TRKG, svoCalDpdVar[layer][temp_dpd_bank][temp_dpd_fc].dpd_gain
                                     +svoCalGnT1Var[layer][TE_GN].li_gain);
        }
        else
    #endif
        {
            WRITE_FIELD(HAL_AFE_TRKO, calSBYTE2DacCount(svoCalDpdVar[layer][temp_dpd_bank][temp_dpd_fc].dpd_offset
                                     +calDacCount2SBYTE(svoCalOfsT1Var[layer][TE_OS].groove_offset[RECDH],8),8));
            WRITE_FIELD(HAL_AFE_TRKG, svoCalDpdVar[layer][temp_dpd_bank][temp_dpd_fc].dpd_gain
                                     +svoCalGnT1Var[layer][TE_GN].groove_gain[RECDH]);
            WRITE_FIELD(HAL_AFE_BLANKTRKO, calSBYTE2DacCount(svoCalDpdVar[layer][temp_dpd_bank][temp_dpd_fc].dpd_offset
                                          +calDacCount2SBYTE(svoCalOfsT1Var[layer][TE_OS].groove_offset[RECDL],8),8));
            WRITE_FIELD(HAL_AFE_BLANKTRKG, svoCalDpdVar[layer][temp_dpd_bank][temp_dpd_fc].dpd_gain
                                          +svoCalGnT1Var[layer][TE_OS].groove_gain[RECDL]);
        }
    }
}

/*-----------------------------------------------------------------------------
 *
 * Module: calGetDPDCalRadius()
 * Description: Caculate radius for calibrating AFE DPD TE offset
 *
 *----------------------------------------------------------------------------*/
USHORT calGetDPDCalRadius(USHORT start_radius, USHORT end_radius)
{
    USHORT pre_radius,temp_radius,found_radius,BwDPDRatioX100;
    BYTE   pre_bank,pre_fc,temp_bank,temp_fc;
    BOOL   found;
    struct svoDpdBankFcVarT DpdSettingForCal;

    found = FALSE;
    found_radius = 0;
    pre_bank = 0xFF;
    pre_fc = 0xFF;
    BwDPDRatioX100 = COEF_GetValue(AFE_CALC_BwDpdRatioX100);
    temp_radius = start_radius;
    while (1)
    {
        DpdSettingForCal = HAL_AFE_CalDPDBw((spdGetTargetOverspeedX100(temp_radius, svoVar_svoTargetSpeed) + 5) / 10, BwDPDRatioX100);
        temp_bank = svoDpdReg2Bank[DpdSettingForCal.DpdBwBank];
        temp_fc = DpdSettingForCal.DpdBwFc;

        if ((temp_bank != pre_bank) || (temp_fc!= pre_fc))
        {
            if (!found)
            {
                if (((svoCalDpdVar[0][temp_bank][temp_fc].status & CAL_REQUIRED) == CAL_REQUIRED) && ((svoCalDpdVar[1][temp_bank][temp_fc].status & CAL_REQUIRED) == CAL_REQUIRED))
                {
                    svoCalDpdVar[Layer_Index][temp_bank][temp_fc].status &= (~CAL_REQUIRED);//improve whole disc pre calibration
                    found = TRUE;
                    found_radius = temp_radius;
                }
            }
            else
            {
                temp_radius = pre_radius;
                break;
            }
            pre_bank = temp_bank;
            pre_fc = temp_fc;
        }
        pre_radius = temp_radius;
        if (temp_radius == end_radius)
        {
            break;
        }
        if (start_radius < end_radius)
        {
            temp_radius += UM_2_SLED_STEP(500);
            if (temp_radius > end_radius)
            {
                temp_radius = end_radius;
            }
        }
        else
        {
            temp_radius -= UM_2_SLED_STEP(500);
            if (temp_radius < end_radius)
            {
                temp_radius = end_radius;
            }
        }
    }

    if (found)
    {
        found_radius = (found_radius + temp_radius) / 2;
    }
    return found_radius;
}
#endif//#if (DPD_COMPENSATE_OFFSET==1)

/*-----------------------------------------------------------------------------
 *
 * Module: feg_stop()
 * Description:
 *
 *----------------------------------------------------------------------------*/
SHORT feg_stop(void) // TBD? __inline
{
    WRITE_REG(HAL_SRVREG_MAP->FGA,0x00);    /* stop focus gain adjustment */
    return(OK);
}// end of feg_stop

/*-----------------------------------------------------------------------------
 *
 * Module: teg_stop()
 * Description:
 *
 *----------------------------------------------------------------------------*/
SHORT teg_stop(void) // TBD? __inline
{
    WRITE_REG(HAL_SRVREG_MAP->TGA,0x00);    /* stop tracking gain adjustment */
    return(OK);
}// end of teg_stop

/*-----------------------------------------------------------------------------
 *
 * Module: Read_BHO()
 * Description:
 *
 *----------------------------------------------------------------------------*/
USHORT  Read_BHO(BYTE num)
{
    ULONG i,adBHO;

    adBHO=0;
    for (i=0;i<num;i++)
    {   //get BHO 32 times
        adBHO += ReadSingleAdcmV(ADC_BHSO);
    }
    return (adBHO / num);
}

/*-----------------------------------------------------------------------------
 *
 * Module: Read_BHO()
 * Description:
 *
 *----------------------------------------------------------------------------*/
USHORT  Read_PHO(BYTE num)
{
    ULONG i,adPHO;

    adPHO=0;
    for (i=0;i<num;i++)//get PHO 32 times
    {
        adPHO += ReadSingleAdcmV(ADC_PHSO);
    }
    return (adPHO / num);
}

//----------------------------------------------------;
// init SRC                                           ;
//----------------------------------------------------;
void init_SRC(void)
{
    HAL_DSP_SRCEnable(SRC_LOOP_ALL, SRC_STATE_OFF);     //Disable SRC
    HAL_DSP_SRCEnable(SRC_LOOP_ALL, SRC_STATE_RESET);   //reset SRC
    HAL_DSP_TachoMultConfig(SRC_TACHO_NORMAL, SRC_SYNC_LEGACY, DISK_NFGPLS); //Tacho config
    HAL_DSP_TachoMultEnable(ON);
    HAL_DSP_SRCConfig(SRC_LOOP_ALL, SRC_STRENGTH_NORMAL); //SRC config
    HAL_DSP_SRCHold(SRC_LOOP_ALL, ON, ON, ON);  //Force holding SRC!
}

#if (WOBBLE_READ_ON_DVD_OR_CD == 1) && (EN_WOBBLE == 1)
/*-----------------------------------------------------------------------------
 *
 * Module: Plus_Minus_det()
 * Description: The new Plus/Minus detection double check wobble even the disk
 *              detection result is ROM in DVDR detection procedure
 *
 *----------------------------------------------------------------------------*/
BOOL Plus_Minus_det(void)
{
#define MAX_NR_WBL_LOCK_PLUSMIN         2

    BYTE   mediaTypeChange;
    USHORT OverspeedX100_target, OverspeedX100_temp, i, NrTryOutCheckWobblePll;
    BOOL   calibration_done, lock_flag, prev_lock_flag;

    if (svoVar_discKindT.Bit.DiscStd != eDiscDVD)
    {
        SendMsg80(DEBUG_DISK_ID_MSG,0x310071); // PLUS_MINUS_DET SKIP(not DVD)
        return(TRUE);
    }

    if ((svoCalVar.discChkDoneBits & plus_minus_chk_end) == plus_minus_chk_end)
    {
        SendMsg80(DEBUG_DISK_ID_MSG,0x310072); // PLUS_MINUS_DET SKIP(check end)
        return(TRUE);
    }

    if (svoVar_discKindT.Bit.WblType != eWblUnknown)
    {
        SendMsg80(DEBUG_DISK_ID_MSG,0x310073); // PLUS_MINUS_DET SKIP(wbl type detected)
        return(TRUE);
    }

    //TBD? Comment! Temporary fix for ROM discs
    if (svoVar_discKindT.Bit.Media == eMediaROM)
    {
        SendMsg80(DEBUG_DISK_ID_MSG,0x310075); // PLUS_MINUS_DET SKIP(eMediaROM)
        return(TRUE);
    }

    SendMsg80(DEBUG_DISK_ID_MSG,0x310050); // DVDR_PLUS/MINUS_Detection

    calibration_done = FALSE;

    // ============= Try Minus Disc Settings ======================
    svoVar_discKindT.Bit.WblType = eWblMinus;

    OverspeedX100_target = spdGetOverSpdX100(spdGetCurrentRPM(),Iram_stp_CurrentPosition);
    SendMsgCn(DEBUG_DISK_ID_MSG,2,0x310054,A2B(OverspeedX100_target));
    afeSetWobbleForASpeed(SVO_CAL_SPEED_DVD);

    if (svoRRFVar.modulation[Layer_Index] == 0)
    {
        WRITE_FIELD(HAL_WBL_LCKDETPER, COEF_GetValue(WBL_PLLLCKCFG_LCKDETPER) * 2);
    }

    NrTryOutCheckWobblePll = 0;
    lock_flag = FALSE;

    while ((lock_flag == FALSE) && (NrTryOutCheckWobblePll < MAX_NR_WBL_LOCK_PLUSMIN))
    {
        SendMsgCn(DEBUG_WBL_MSG,1,0x410212,A1B(NrTryOutCheckWobblePll)); //WOBBLE PLL Check STATE: New try out, number:
        HAL_WOBBLE_SetPllOnHold(halWBL_WblPllInHold);
        SetDfeWblFrontSemiSynchrAfterTracking();
        NrTryOutCheckWobblePll++;
        for (i=0; i<20; i++)
        {
            prev_lock_flag = lock_flag;
            WaitTimer(1L);
            OverspeedX100_temp = HAL_WOBBLE_GetOverspeed(HAL_PLL_GetWblClkKHz());
            SendMsgCn(DEBUG_DISK_ID_MSG,2,0x310053,A2B(OverspeedX100_temp));
            lock_flag = HAL_WOBBLE_GetPllInLock(HAL_PLL_GetWblClkKHz());
            if ((lock_flag == TRUE)&&(prev_lock_flag == TRUE))
            {
                SendMsg80(DEBUG_WBL_MSG,0x410211);//WOBBLE PLL in lock
                break;
            }
        }
    }

    if ( (lock_flag == TRUE) && (OverspeedX100_temp >= (OverspeedX100_target - 20)) && (OverspeedX100_temp <= (OverspeedX100_target + 20)))
    {
        if(svoVar_discKindT.Bit.Media == eMediaR)
        {
            SendMsg80(SHOW_DEBUG_MSG,0x3100E4+svoVar_discKindT.Bit.Layers-1); // Plus_Minus_check==>DVD-R xL Disc
        }
        else
        {
            SendMsg80(SHOW_DEBUG_MSG,0x3100E6+svoVar_discKindT.Bit.Layers-1); // Plus_Minus_check==>DVD-RW xL Disc
        }
        SendMsgCnB(SHOW_DEBUG_MSG,4,0x420021,B4B(HAL_WOBBLE_GetPLLFreq()));
        calibration_done = TRUE;
        mediaTypeChange = 0;
    }
    else
    {
        // ============= Try Plus Disc Settings ======================
        svoVar_discKindT.Bit.WblType = eWblPlus;

        OverspeedX100_target = spdGetOverSpdX100(spdGetCurrentRPM(),Iram_stp_CurrentPosition);
        SendMsgCn(DEBUG_DISK_ID_MSG,2,0x310052,A2B(OverspeedX100_target));
        afeSetWobbleForASpeed(SVO_CAL_SPEED_DVD);
        if (svoRRFVar.modulation[Layer_Index] == 0)
        {
            WRITE_FIELD(HAL_WBL_LCKDETPER, COEF_GetValue(WBL_PLLLCKCFG_LCKDETPER) * 2);
        }

        svoCalVar.calGainDoneBits &= (~dif_WOB_end);
        wbl_gain_calib();

        NrTryOutCheckWobblePll = 0;
        lock_flag = FALSE;

        while ((lock_flag == FALSE) && (NrTryOutCheckWobblePll < MAX_NR_WBL_LOCK_PLUSMIN))
        {
            SendMsgCn(DEBUG_WBL_MSG,1,0x410212,A1B(NrTryOutCheckWobblePll));   //WOBBLE PLL Check STATE: New try out, number:
            HAL_WOBBLE_SetPllOnHold(halWBL_WblPllInHold);
            SetDfeWblFrontSemiSynchrAfterTracking();
            NrTryOutCheckWobblePll++;
            for (i=0; i<20; i++)
            {
                prev_lock_flag = lock_flag;
                WaitTimer(1L);
                OverspeedX100_temp = HAL_WOBBLE_GetOverspeed(HAL_PLL_GetWblClkKHz());
                SendMsgCn(DEBUG_DISK_ID_MSG,2,0x310051,A2B(OverspeedX100_temp));
                lock_flag = HAL_WOBBLE_GetPllInLock(HAL_PLL_GetWblClkKHz());
                if ((lock_flag == TRUE)&&(prev_lock_flag == TRUE))
                {
                    SendMsg80(DEBUG_WBL_MSG,0x410211);   //WOBBLE PLL in lock
                    break;
                }
            }
        }

        if ( (lock_flag == TRUE) && (OverspeedX100_temp >= (OverspeedX100_target - 20)) && (OverspeedX100_temp <= (OverspeedX100_target + 20)))
        {
            if (svoVar_discKindT.Bit.Media == eMediaR)
            {
                SendMsg80(SHOW_DEBUG_MSG,0x3100E0+svoVar_discKindT.Bit.Layers-1); // Plus_Minus_check==>DVD+R xL Disc
            }
            else
            {
                SendMsg80(SHOW_DEBUG_MSG,0x3100E2+svoVar_discKindT.Bit.Layers-1); // Plus_Minus_check==>DVD+RW xL Disc
            }
            SendMsgCnB(SHOW_DEBUG_MSG,4,0x420021,B4B(HAL_WOBBLE_GetPLLFreq()));
            calibration_done = TRUE;
            mediaTypeChange = 0;
        }
    }

    if (calibration_done == FALSE)
    {
        svoVar_discKindT.Bit.Media = eMediaROM;
        svoVar_discKindT.Bit.WblType = eWblUnknown;
        LAL_StoreParams(LAL_DetectDiscKind(svoVar_discKindT), svoIramVar.svoCurrentSpeed);
        LAL_SetReadMode();
        SendMsg80(SHOW_DEBUG_MSG,0x3100E8+svoVar_discKindT.Bit.Layers-1); //Plus_Minus_check==>DVD-ROM xL Disc
        mediaTypeChange = 1;
    }
    else
    {
        UpdateWobbleFilters(svo_bReadZoneSpeedX10);
    }

    WRITE_FIELD(HAL_WBL_LCKDETPER, COEF_GetValue(WBL_PLLLCKCFG_LCKDETPER));
    svoCalVar.discChkDoneBits |= plus_minus_chk_end;

    if (mediaTypeChange == 1)
    {
        WRITE_FIELD(HAL_SRV_LDON, 0x01);    // Release tracking
        afeSetTrackingMethod(getTrackingMethod(svoVar_discKindT));
        svoCalVar.calOfsDoneBits  &= ~(troof_end | teof_end);
        svoCalVar.calGainDoneBits &= ~(tebg_end | teg_end | rpg_end);
        return(FALSE);       // Go back to DVDR detection table
    }
    ENABLE_WBLINT();

    return (TRUE);
}
#endif //(WOBBLE_READ_ON_DVD_OR_CD == 1) && (EN_WOBBLE == 1)

/*------------------------------------------------------------------------*/
/* The end of file                                                        */
/*------------------------------------------------------------------------*/
