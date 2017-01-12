/****************************************************************************
*    (c) Copyright 2001 - 2008 SUNEXT DESIGN, INC.
*                           All Rights Reserved
*
*
* FILENAME:   SVO_TBL.C
*
* DESCRIPTION
*
*
*
* NOTES:
*
* $Revision: 296 $
* $Date: 11/03/28 4:07p $
*
*
**************************** SUNEXT CONFIDENTIAL ****************************/

#include ".\common\globals.h"
#include ".\common\globtype.h"
#include ".\common\chip.h"
#include ".\servo\svo.h"
#include ".\servo\svo_com.h"
#include ".\servo\svo_tbl.h"
#include ".\servo\svo_dsp.h"
#include ".\AL\REG_servo.h"
#include ".\AL\REG_write_strategy.h"
#include ".\AL\REG_global.h"
#include ".\AL\REG_periph.h"
#include ".\AL\REG_cd_decoder.h"
#include ".\AL\REG_dvd_encoder.h"
/*----------------------------------------------------------------------------*/

#define DMCNT_CD    (0x08|(DFRQ_FG<<2)|(DFRQ_WBL_DEC*0x03))
#define SPINCTL_CD  (DISK_AFCG_CD << 1)

/*
 * Servo initial command data table
 */
svoRegInitTblT const svoRegInitNONETbl[] = {

    /* CD DECODER*/
    {HAL_CDD_MUTEOFF_ADR,          0x0010,       BYTESET},   /* 0x1000000E[7:0] MUTE_RELEASE=ON, Mute_Control=Disable, Enable Audio concealment*/
    {HAL_CDD_SCOR_ADR,             0x0000,       BYTESET},   /* 0x10000020[7:0] Clear all Interrupt status.  */
    {HAL_CDD_SCOREN_ADR,           0x0000,       BYTESET},   /* 0x10000021[7:0] disable Interrupt = SCOR */
    {HAL_CDD_SCOREDGE_ADR,         0x0000,       BYTESET},   /* 0x023 SCOREDGE=0  */

    /* SERVO PAGE */
    /* Servo, HW stepping */
    //TBD? CONDITIONAL! shouldn't this code be use HW Stepper conditional?
    {HAL_SRV_TIMPER_ADR,          0x0064,       BYTESET}, /* 0xB0000024[7:0] Set HW stepping timer = 100us */
    {HAL_SRV_HWSTPSEEK_ADR,       0x0000,       BYTESET}, /* 0xB0000028[7:0] Set DAC value from stepper motor block */
    {HAL_SRV_SEEKSTART_ADR,       0x0010,       BYTESET}, /* 0xB0000040[7:0] Set Full step use square wave */
    {HAL_SRV_HYSLTHLD_ADR,        0x0003,       BYTESET}, /* 0xB00000B0[7:0] SLED DAC value low threshold value */
    {HAL_SRV_HYSHTHLD_ADR,        0x0003,       BYTESET}, /* 0xB00000B4[7:0] SLED DAC value high threshold value */

    /* Servo */

#if (FOOTILT_DIFFERENTIAL == 1)
    {HAL_SRV_FOODIFEN_ADR,        0x0010,       BYTESET}, /* 0xB0000118[7:0] SERVO DAC Switch */
#else // (FOOTILT_DIFFERENTIAL == 1)
    {HAL_SRV_FOODIFEN_ADR,        0x0000,       BYTESET}, /* 0xB0000118[7:0] SERVO DAC Switch */
#endif // (FOOTILT_DIFFERENTIAL == 1)
    {HAL_SRV_FETHR_ADR,           0x007F,       BYTESET}, /* 0xB000011C[7:0] FE threshold value */
    {HAL_SRV_FETHREN_ADR,HAL_SRV_FETHREN,        BIT_ON}, /* 0xB0000120[7:0] FE threshold enable */
    {HAL_SRV_TONIMSK_ADR,         0x0000,       BYTESET}, /* 0xB000012C[7:0] TONIMSK=0,DPLIMSK=0,FGIMSK=0,WBLIMSK=0 */
    {HAL_SRV_AFCRNG1_ADR,         0x0028,       BYTESET}, /* 0xB0000138[6:0] AFC in range low */
    {HAL_SRV_AFCRNG2_ADR,         0x0050,       BYTESET}, /* 0xB000013C[6:0] AFC in range high */
    {HAL_SRV_SRVMOD_ADR,          0x0019,       BYTESET}, /* 0xB0000140[7:0] REGTON=0,DPLLMSK=0,DECCHG=010,SRVMOD=1 */
    {HAL_SRV_FGCMP_ADR,           0x0160,       WORDSET}, /* 0xB0000148[15:0] 33.8688e6/64/(2*23.14*36) = 635 */
    {HAL_SRV_APCNUM_ADR,          0x0000,       BYTESET}, /* 0xB0000178[3:0] APCNUM=3*/
    {HAL_SRV_WAITWBL_ADR,         0x000F,       BYTESET}, /* 0xB0000170[3:0] wobble sync lock time */
    {HAL_SRV_APCGAIN_ADR,         0x0000,       BYTESET}, /* 0xB0000174[7:0] AFCINRGEN off, APCEN off, APCGAIN=1 */
    {HAL_SRV_AFCG_ADR,        SPINCTL_CD,       BYTESET}, /* 0xB00001DC[7:0] AFCG */
    {HAL_SRV_NDIV_ADR,            0x0004,       WORDSET}, /* 0xB00001F0[9:0] CLVN = 4 */
    {HAL_SRV_FGRNG_ADR,           0x0023,       WORDSET}, /* 0xB00001F8[15:0] FG count range */
    {HAL_SRV_TSVSTP_ADR,          0x0000,       BYTESET}, /* 0xB000020C.7    stop PICUP register */
    {HAL_SRV_LDOFF_ADR,           0x0000,       BYTESET}, /* 0xB000021C[7:0] LDON = 0 */
    {HAL_SRV_PC_ADR,              0x0000,       BYTESET}, /* 0xB0000234[7:0] PAM duty cycle 100% */
    {HAL_SRV_PF_ADR,              0x0000,       BYTESET}, /* 0xB0000238[7:0] PAM frequency 172.2Hz */
    {HAL_SRV_DAC2SEL_ADR,         0x0020,       BYTESET}, /* 0xB000029C[7:0] output as DACDAT2/DACDAT1 */
    {HAL_SRV_DBT_ADR,             0x0000,       BYTESET}, /* 0xB0000334[6:0] Defect Delay Time = 0us */
    {HAL_SRV_FGAT_ADR,            0x0000,       BYTESET}, /* 0xB0000344[7:0] Stop focus adjustment */
    {HAL_SRV_TGAT_ADR,            0x0000,       BYTESET}, /* 0xB000034C[7:0] Stop tracking adjustment */
    {HAL_SRV_FBAVS_ADR,           0x00C0,       BYTESET}, /* 0xB0000350[7:0] FBAVS,FBAVS4 = 88Hz,FBASE = 1b,FQWDSEL,FQWDSEL4 = 2.5ms */
    {HAL_SRV_MSKON_ADR,           0x0051|(DISK_FR2X<<HAL_SRV_FR2X_LSB),BYTESET}, /* 0xB000036C[7:0] double FG,invert FG, use FG noise mask, TR/FE loop gain excitation freq = 2.75K */
    {HAL_SRV_TROFFSET_ADR,        0x0000,       BYTESET}, /* 0xB0000370[7:0] */
#if (CHIP_REV <= 0xB3)

    {HAL_SRV_FSL_ADR,             0x0030,       BYTESET}, /* 0xB0000384[6:0] FSL 6-0 Focus search standby level = 0.453 v */
#endif
    {HAL_SRV_FOKL_ADR,            0x0018,       BYTESET}, /* 0xB00003D0[7:0] FOKL 6-0 Focus OK Level = 0.226 v */
    {HAL_SRV_SHL_ADR,             0x00F0,       BYTESET}, /* 0xB000038C[7:0] Shock Threshold = 880mV,Shock Delay Time = 0ms */
    {HAL_SRV_THOLD_ADR,           0x002C,       BYTESET}, /* 0xB0000390[7:0] Gain Up Time = 2~3ms,Hysteresis Time = 11~12ms,,FHOLD = 1b,THOLD = 1b,GUPCL = 0b,XCOEF = 0b */
    {HAL_SRV_GUP2_ADR,            0x00C0,       BYTESET}, /* 0xB0000394[7:0] GUP2 = 1b,HYS2 = 1b,GUP1 = 0b,HYS1 = 0b,HYSGUP = 0b,RXMUTE = 0b,LoCKON = 0b,LENSDN = 0b */
#if (CHIP_REV <= 0xB3)
    {HAL_SRV_FVD_ADR,             0x0018,       BYTESET}, /* 0xB0000398[7:0] KWIDSL = 0us,FVD = 5.52KHz */
#endif
#if (FOCUS_SERVO_INPUT_GAIN == 128)&&(TRACKING_SERVO_INPUT_GAIN == 128)
    {HAL_SRV_FGSW_ADR,            0x00C2,       BYTESET}, /* 0xB000039C[7:0] Foucs Gain Input = 128,Tracking Gain Input = 128,FOSW = 0b,TRSW = 0b,FDSW = 0b */
#elif (FOCUS_SERVO_INPUT_GAIN == 128)
    {HAL_SRV_FGSW_ADR,            0x0082,       BYTESET}, /* 0xB000039C[7:0] Foucs Gain Input = 128,Tracking Gain Input = 256,FOSW = 0b,TRSW = 0b,FDSW = 0b */
#elif (TRACKING_SERVO_INPUT_GAIN == 128)
    {HAL_SRV_FGSW_ADR,            0x0042,       BYTESET}, /* 0xB000039C[7:0] Foucs Gain Input = 256,Tracking Gain Input = 128,FOSW = 0b,TRSW = 0b,FDSW = 0b */
#else // (FOCUS_SERVO_INPUT_GAIN == 128)&&(TRACKING_SERVO_INPUT_GAIN == 128)
    {HAL_SRV_FGSW_ADR,            0x0002,       BYTESET}, /* 0xB000039C[7:0] Foucs Gain Input = 128,Tracking Gain Input = 128,FOSW = 0b,TRSW = 0b,FDSW = 0b */
#endif // (FOCUS_SERVO_INPUT_GAIN == 128)&&(TRACKING_SERVO_INPUT_GAIN == 128)
    {HAL_SRV_DFRQFG_ADR,        DMCNT_CD,       BYTESET}, /* 0xB00003AC[7:0] disc motor control register CLVWBLSEL=CD/DVD, CLVSYNSEL=CDSYNC, CAVSIGEL=FG, BKAUTO=1b */
#if (CHIP_REV <= 0xB3)
    {HAL_SRV_AFEDFCT_ADR,HAL_SRV_AFEDFCT,        BIT_ON}, /* 0xB00003B0[7:0] defect select afe,servo defect select '1'--afe defect,'0'--servo defect */
#else
    {HAL_SRV_DEEPDFCT_ADR,HAL_SRV_DEEPDFCT,          BIT_ON},   /* 0xB00003B0[7:0] defect select afe,servo defect select '1'--afe defect,'0'--servo defect */
#endif
    {HAL_SRV_ZTK_ADR,             0x0000,       BYTESET}, /* 0xB00003B4[7:0] ZTK = 0b, PGTK = 0b, PLF = 0b, RXP = 0b, TXP = 0b */
    {HAL_SRV_DVDRHHLD_ADR,        0x0001,       BYTESET}, /* 0xB00003B8.0 DVD-RAM TE/FE hold during Header */
    {HAL_SRV_SYNCIEN_ADR,         0x0000,       WORDSET}, /* 0xB00003E0[12:0] disable Interrupt = SEEKC */
    {HAL_SRV_SYNCI_ADR,           0x0001,       WORDSET}, /* 0xB00003E8[12:0] Clear Interrput Status except the last bit */

#if (CHIP_REV <= 0xB3)
    //Set HW kick/brake parameters
    {HAL_SRV_ZCEAREN_ADR,         0x0033,       BYTESET}, /* 0xB0000058[7:0] EARZCPROT */
    {HAL_SRV_BRAKELVL_ADR,        0x0000,       BYTESET}, /* 0xB000005C[7:0] BRAKELVL */
    {HAL_SRV_KICKLVL_ADR,         0x0070,       BYTESET}, /* 0xB0000060[7:0] KICKLVL */
    {HAL_SRV_BRAKEHLD_ADR,        0x0000,       BYTESET}, /* 0xB0000068[7:0] BRAKEHLD */
    {HAL_SRV_BRAKEEN_ADR,         0x00E0,       BYTESET}, /* 0xB000006C[7:0] BRAKECNT */
    {HAL_SRV_BRAKESPD_ADR,        0x0000,       BYTESET}, /* 0xB0000064[7:0] SPEEDLVL */
#endif

    /* GLOBAL PAGE */
    {HAL_GLOBAL_SLPSRVADC_ADR,    0x0000,       BYTESET}, /* 0x10000702[7:0] No power down on CD-Servo, Servo-DAC, GP-ADC,*/
    {HAL_GLOBAL_WBLRSMP_ADR,      0x0004,       BYTESET}, /* 0x10000780[2:0] WPLTSH = SMP4 */
    {HAL_GLOBAL_WBLWSMP_ADR,      0x0003,       BYTESET}, /* 0x10000781[2:0] WBLSH  = SMP3 */
    {HAL_GLOBAL_MPDSMP_ADR,       0x0002,       BYTESET}, /* 0x10000782[2:0] MPDSH  = SMP2 */
    {HAL_GLOBAL_SPDSMP_ADR,       0x0002,       BYTESET}, /* 0x10000783[2:0] SPDSH  = SMP2 */
    {HAL_GLOBAL_WRFSMP_ADR,       0x0002,       BYTESET}, /* 0x10000784[2:0] WREFSH = SMP2 */
    {HAL_GLOBAL_RFPDSMP_ADR,      0x0005,       BYTESET}, /* 0x10000785[2:0] RFPDSH = SMP5 */
    {HAL_GLOBAL_WFPDSMP_ADR,      0x0001,       BYTESET}, /* 0x10000786[2:0] WFPDSH = SMP1 */
    {HAL_GLOBAL_RFADCFRA_ADR,     0x0010,       BYTESET}, /* 0x10000730[5:4] RFADCFRA(1-0) = 1 */

    /* PERIPHERALS PAGE */
    {HAL_PERIPH_ADCGAIN_ADR,     ADCGAIN,       BYTESET}, /* 0x100005F4.0 */

    {SVOREGTBL_END,               0x0000,       BYTESET}    /* end code */
}; /* svoRegInitbl[] */

svoRegInitTblT const svoRegInit4LXTbl[] = {
    /* SERVO*/
    {HAL_SRV_NDIV_ADR,           0x0004,       WORDSET}, /* 0xB00001F0[9:0] CLVN = 4 */
    {HAL_SRV_DECCMP_ADR,         0x1200,       WORDSET}, /* 0xB0000158[15:0] DECCMP 33.8688e6/7350 = 4608 */
    {HAL_SRV_DBT_ADR,            0x0000,       BYTESET}, /* 0xB0000334[6:0] Defect Delay Time= 8.4us */
#if (CHIP_REV <= 0xB3)
    {HAL_SRV_FVD_ADR,            0x0018,       BYTESET}, /* 0xB0000398[7:0] KWIDSL = 0us,FVD = 5.52KHz */
#endif
    {SVOREGTBL_END,              0x0000,       BYTESET}    /* end code */
}; /* svoRegInit4XTbl[] */

svoRegInitTblT const svoRegInit8LXTbl[] = {
    /* SERVO*/
    {HAL_SRV_NDIV_ADR,           0x0008,       WORDSET}, /* 0xB00001F0[9:0] CLVN = 8 */
    {HAL_SRV_DECCMP_ADR,         0x1200,       WORDSET}, /* 0xB0000158[15:0] DECCMP 33.8688e6/7350 = 4608 */
    {HAL_SRV_DBT_ADR,            0x0000,       BYTESET}, /* 0xB0000334[6:0] Defect Delay Time= 8.4us */
#if (CHIP_REV <= 0xB3)
    {HAL_SRV_FVD_ADR,            0x0018,       BYTESET}, /* 0xB0000398[7:0] KWIDSL = 0us,FVD = 5.52KHz */
#endif
    {SVOREGTBL_END,              0x0000,       BYTESET}    /* end code */
}; /* svoRegInit8XTbl[] */

svoRegInitTblT const svoRegInit12LXTbl[] = {
    /* SERVO*/
    {HAL_SRV_NDIV_ADR,           0x000C,       WORDSET}, /* 0xB00001F0[9:0] CLVN = 12 */
    {HAL_SRV_DECCMP_ADR,         0x1200,       WORDSET}, /* 0xB0000158[15:0] DECCMP 33.8688e6/7350 = 4608 */
    {HAL_SRV_DBT_ADR,            0x0000,       BYTESET}, /* 0xB0000334[6:0] Defect Delay Time= 8.4us */
#if (CHIP_REV <= 0xB3)
    {HAL_SRV_FVD_ADR,            0x0018,       BYTESET}, /* 0xB0000398[7:0] KWIDSL = 0us,FVD = 5.52KHz */
#endif
    {SVOREGTBL_END,              0x0000,       BYTESET}    /* end code */
}; /* svoRegInit12XTbl[] */

svoRegInitTblT const svoRegInit16LXTbl[] = {
    /* SERVO*/
    {HAL_SRV_NDIV_ADR,           0x0010,       WORDSET}, /* 0xB00001F0[9:0] CLVN = 16 */
    {HAL_SRV_DECCMP_ADR,         0x1200,       WORDSET}, /* 0xB0000158[15:0] DECCMP 33.8688e6/7350 = 4608 */
    {HAL_SRV_DBT_ADR,            0x0000,       BYTESET}, /* 0xB0000334[6:0] Defect Delay Time= 8.4us */
#if (CHIP_REV <= 0xB3)
    {HAL_SRV_FVD_ADR,            0x0018,       BYTESET}, /* 0xB0000398[7:0] KWIDSL = 0us,FVD = 5.52KHz */
#endif
    {SVOREGTBL_END,              0x0000,       BYTESET}    /* end code */
}; /* svoRegInit16LXTbl[] */

svoRegInitTblT const svoRegInit10AXTbl[] = { /* 6.96x-16.8x */
    /* SERVO*/
    {HAL_SRV_DBT_ADR,            0x0000,       BYTESET}, /* 0xB0000334[6:0] Defect Delay Time= 8.4us */
#if (CHIP_REV <= 0xB3)
    {HAL_SRV_FVD_ADR,            0x0018,       BYTESET}, /* 0xB0000398[7:0] KWIDSL = 0us,FVD = 5.52KHz */
#endif
    {SVOREGTBL_END,              0x0000,       BYTESET}    /* end code */
}; /* svoRegInit10ATbl[] */

#if(ENABLE_CD_12XCAV ==1)
svoRegInitTblT const svoRegInit12AXTbl[] = { /* 6.96x-16.8x */
    /* SERVO*/
    {HAL_SRV_DBT_ADR,            0x0000,       BYTESET}, /* 0xB0000334[6:0] Defect Delay Time= 8.4us */
#if (CHIP_REV <= 0xB3)
    {HAL_SRV_FVD_ADR,            0x0018,       BYTESET}, /* 0xB0000398[7:0] KWIDSL = 0us,FVD = 5.52KHz */
#endif
    /* WRITE_STRATEGY*/
    {HAL_WS_WSRAMRDEN_ADR,      HAL_WS_FASTEN,       BIT_OFF},   /* 0x10000FB1[7:0] *//* bFASTEN = 0 */
    {HAL_WS_TBGM_ADR,            0x0071,       BYTESET}, /* 0x10000FBB[7:0] TBGN = 7, TBGM = 1, (N+1)/(M+1) = 4 */
    {HAL_WS_TBGL_ADR,            0x0019,       BYTESET}, /* 0x10000FBC[6:0] 0xFBC TBGK = 1, TBGL = 4, TBGPWR On */

    {SVOREGTBL_END,              0x0000,       BYTESET}    /* end code */
}; /* svoRegInit12ATbl[] */
#else // (ENABLE_CD_12XCAV ==1)
svoRegInitTblT const svoRegInit16AXTbl[] = { /* 6.96x-16.8x */
    /* SERVO*/
    {HAL_SRV_DBT_ADR,            0x0000,       BYTESET}, /* 0xB0000334[6:0] Defect Delay Time= 8.4us */
#if (CHIP_REV <= 0xB3)
    {HAL_SRV_FVD_ADR,            0x0018,       BYTESET}, /* 0xB0000398[7:0] KWIDSL = 0us,FVD = 5.52KHz */
#endif
    {SVOREGTBL_END,              0x0000,       BYTESET}    /* end code */
}; /* svoRegInit16XTbl[] */
#endif // (ENABLE_CD_12XCAV ==1)
svoRegInitTblT const svoRegInit24AXTbl[] = { /* 9.28x-22.4x */
    /* SERVO*/
    {HAL_SRV_DBT_ADR,            0x0000,       BYTESET}, /* 0xB0000334[6:0] Defect Delay Time= 8.4us */
#if (CHIP_REV <= 0xB3)
    {HAL_SRV_FVD_ADR,            0x0018,       BYTESET}, /* 0xB0000398[7:0] KWIDSL = 0us,FVD = 5.52KHz */
#endif
    {SVOREGTBL_END,              0x0000,       BYTESET}    /* end code */
}; /* svoRegInit24XTbl[] */

svoRegInitTblT const svoRegInit32AXTbl[] = { /* x-32x */
    /* SERVO*/
    {HAL_SRV_DBT_ADR,            0x0000,       BYTESET}, /* 0xB0000334[6:0] Defect Delay Time= 8.4us */
#if (CHIP_REV <= 0xB3)
    {HAL_SRV_FVD_ADR,            0x0018,       BYTESET}, /* 0xB0000398[7:0] KWIDSL = 0us,FVD = 5.52KHz */
#endif
    {SVOREGTBL_END,              0x0000,       BYTESET}    /* end code */
}; /* svoRegInit32XTbl[] */

svoRegInitTblT const svoRegInit40AXTbl[] = { /* x-40x */
    /* SERVO*/
    {HAL_SRV_DBT_ADR,            0x0000,       BYTESET}, /* 0xB0000334[6:0] Defect Delay Time= 8.4us */
#if (CHIP_REV <= 0xB3)
    {HAL_SRV_FVD_ADR,            0x0018,       BYTESET}, /* 0xB0000398[7:0] KWIDSL = 0us,FVD = 5.52KHz */
#endif
    {SVOREGTBL_END,              0x0000,       BYTESET}    /* end code */
}; /* svoRegInit40XTbl[] */

svoRegInitTblT const svoRegInit48AXTbl[] = { /* x-48x */
    /* SERVO*/
    {HAL_SRV_DBT_ADR,            0x0000,       BYTESET}, /* 0xB0000334[6:0] Defect Delay Time= 8.4us */
#if (CHIP_REV <= 0xB3)
    {HAL_SRV_FVD_ADR,            0x0018,       BYTESET}, /* 0xB0000398[7:0] KWIDSL = 0us,FVD = 5.52KHz */
#endif
    {SVOREGTBL_END,              0x0000,       BYTESET}    /* end code */
}; /* svoRegInit48XTbl[] */

svoRegInitTblT const svoRegInit52AXTbl[] = { /* x-52x */
    /* SERVO*/
    {HAL_SRV_DBT_ADR,            0x0000,       BYTESET}, /* 0xB0000334[6:0] Defect Delay Time= 8.4us */
#if (CHIP_REV <= 0xB3)
    {HAL_SRV_FVD_ADR,            0x0018,       BYTESET}, /* 0xB0000398[7:0] KWIDSL = 0us,FVD = 5.52KHz */
#endif
    {SVOREGTBL_END,              0x0000,       BYTESET}    /* end code */
}; /* svoRegInit52XTbl[] */

const svoRegInitTblT *const svoRegInitTbl[] = {
    (svoRegInitTblT *const) svoRegInitNONETbl,   // NONE
    (svoRegInitTblT *const) svoRegInit4LXTbl,    // _4Lx
    (svoRegInitTblT *const) svoRegInit8LXTbl,    // _8LX
    (svoRegInitTblT *const) svoRegInit12LXTbl,   // _12x
    (svoRegInitTblT *const) svoRegInit16LXTbl,   // _16Lx
    (svoRegInitTblT *const) svoRegInit10AXTbl,   // _10Ax
#if (ENABLE_CD_12XCAV == 1)
    (svoRegInitTblT *const) svoRegInit12AXTbl,   // _12Ax
#else // (ENABLE_CD_12XCAV == 1)
    (svoRegInitTblT *const) svoRegInit16AXTbl,   // _16Ax
#endif // (ENABLE_CD_12XCAV == 1)
    (svoRegInitTblT *const) svoRegInit24AXTbl,   // _24Ax
    (svoRegInitTblT *const) svoRegInit32AXTbl,   // _32Ax
    (svoRegInitTblT *const) svoRegInit40AXTbl,   // _40Ax
    (svoRegInitTblT *const) svoRegInit48AXTbl,   // _48Ax
    (svoRegInitTblT *const) svoRegInit52AXTbl,   // _52Ax
};

/*----------------------------------------------------------------------------*/
/*
 * Initial digital equalizer coefficients table
 */
/*----------------------------------------------------------------------------*/
const WORD  Tracking_Eq[]={             /* 0 */
    kt11,       /* 0x00 */
    kt12,       /* 0x01 */
    kt13,       /* 0x02 */
    kt14,       /* 0x03 */
    kt15,       /* 0x04 */
    kt16,       /* 0x05 */
    kt17,       /* 0x06 */
    kt18,       /* 0x07 */
    kt19,       /* 0x08 */
    kt1A,       /* 0x09 */
    src_kt2,
    (WORD)EQ_END,
    (WORD)EQ_END
};

const WORD  Tracking_Low_Eq[]={         /* 1 */
    kt1c,       /* 0x60 */
    kt1d,       /* 0x61 */
    kt1e,       /* 0x62 */
    kt1f,       /* 0x63 */
    (WORD)EQ_END,
    (WORD)EQ_END
};

const WORD  Tracking_DC_Eq[]={          /* 2 */
    kt20,       /* 0x0C */
    kt21,       /* 0x0D */
    kt22,       /* 0x0E */
    (WORD)EQ_END,
    (WORD)EQ_END
};

const WORD  Tracking_Shock_Eq[]={       /* 3 */
    kt30,       /* 0x1A */
    kt31,       /* 0x1B */
    kt32,       /* 0x1C */
    kt33,       /* 0x1D */
    kt34,       /* 0x1E */
    kt35,       /* 0x1F */
    (WORD)EQ_END,
    (WORD)EQ_END
};

const WORD  Focus_Eq[]={                /* 4 */
    kf11,       /* 0x20 */
    kf12,       /* 0x21 */
    kf13,       /* 0x22 */
    kf14,       /* 0x23 */
    kf15,       /* 0x24 */
    kf16,       /* 0x25 */
    kf17,       /* 0x26 */
    src_kf2,
    (WORD)EQ_END,
    (WORD)EQ_END
};

const WORD  Focus_Low_Eq[]={            /* 5 */
    kf1c,       /* 0x58 */
    kf1d,       /* 0x59 */
    kf1e,       /* 0x5A */
    kf1f,       /* 0x5B */
    (WORD)EQ_END,
    (WORD)EQ_END
};

const WORD  Focus_DC_Eq[]={             /* 6 */
    kf20,       /* 0x29 */
    kf21,       /* 0x2A */
    kf22,       /* 0x2B */
    (WORD)EQ_END,
    (WORD)EQ_END
};

const WORD  Focus_High_Eq[]={           /* 7 */
    kf30,       /* 0x2C */
    kf31,       /* 0x2D */
    kf32,       /* 0x2E */
    (WORD)EQ_END,
    (WORD)EQ_END
};


const WORD  Disk_Fg_Eq[]={              /* 8 */
    kdf10,       /* 0x40 */
    kdf11,       /* 0x41 */
    kdf12,       /* 0x42 */
    kdf13,       /* 0x43 */
    kdf14,       /* 0x44 */
    kdf15,       /* 0x2F */
    kdf16,       /* 0x3F */
    (WORD)EQ_END,
    (WORD)EQ_END
};

const WORD  Sled_Eq[]={                 /* 9 */
    ks10,       /* 0x37 */
    ks11,       /* 0x38 */
    ks12,       /* 0x39 */
    ks13,       /* 0x3A */
    ks14,       /* 0x3B */
    ks15,       /* 0x3C */
    ks16,       /* 0x3D */
    (WORD)EQ_END,
    (WORD)EQ_END
};

const WORD  Beam_Low_Eq[]={             /* 10 */
    kb10,       /* 0x45 */
    kb11,       /* 0x46 */
    kb12,       /* 0x47 */
    (WORD)EQ_END,
    (WORD)EQ_END
};

const WORD  Beam_High_Eq[]={            /* 11 */
    kb20,       /* 0x48 */
    kb21,       /* 0x49 */
    kb22,       /* 0x4A */
    (WORD)EQ_END,
    (WORD)EQ_END
};

const WORD  Speed_Eq[]={                /* 12 */
    kspd_0,      /* 0x70 */
    kspd_4,      /* 0x71 */
    kspd_8,      /* 0x72 */
    kspd_16,     /* 0x73 */
    kspd_32,     /* 0x74 */
    kspd_64,     /* 0x75 */
    kspd_128,    /* 0x76 */
    kspd_256,    /* 0x77 */
    kspd_512,    /* 0x78 */
    kspd_1024,   /* 0x79 */
    (WORD)EQ_END,
    (WORD)EQ_END
};

const WORD  Adjust_Eq[]={               /* 13 */
    kf18,       /* 0x28 */
    kfb20,      /* 0x55 */
    kfb21,      /* 0x56 */
    krb10,      /* 0x65 */
    ks17,       /* 0x3E */
    (WORD)EQ_END,
    (WORD)EQ_END
};

#if 0 //not use
const WORD  Disk_Dec_Eq[]={             /* 14 */
    kdd10,       /* 0xFA */
    kdd11,       /* 0xFB */
    kdd12,       /* 0xFC */
    kdd13,       /* 0xFD */
    kdd14,       /* 0xFE */
    kdd15,       /* 0xFF */
    kdd16,       /* 0x0F */
    (WORD)EQ_END,
    (WORD)EQ_END
};

const WORD  Disk_Wbl_Eq[]={             /* 15 */
    kdw10,       /* 0x50 */
    kdw11,       /* 0x51 */
    kdw12,       /* 0x52 */
    kdw13,       /* 0x53 */
    kdw14,       /* 0x54 */
    kdw15,       /* 0x5C */
    kdw16,       /* 0x64 */
    (WORD)EQ_END,
    (WORD)EQ_END
};
#endif

WORD *const EQ_addr[EQ_TBL_MAX+1] = {
    (WORD *const) Tracking_Eq,
    (WORD *const) Tracking_Low_Eq,
    (WORD *const) Tracking_DC_Eq,
    (WORD *const) Tracking_Shock_Eq,
    (WORD *const) Focus_Eq,
    (WORD *const) Focus_Low_Eq,
    (WORD *const) Focus_DC_Eq,
    (WORD *const) Focus_High_Eq,
    (WORD *const) Disk_Fg_Eq,
    (WORD *const) Sled_Eq,
    (WORD *const) Beam_Low_Eq,
    (WORD *const) Beam_High_Eq,
    (WORD *const) Speed_Eq,
    (WORD *const) Adjust_Eq,
};

const WORD  Ce_Eq[]={
    ktwuxc11,    /* 0x2C */
    ktwuxc12,    /* 0x3C */
    ktwuxc13,    /* 0x4C */
    ktwuxc14,    /* 0x5C */
    ktwuxc15,    /* 0x6C */
    ktwuxc16,    /* 0x7C */
    ktwuxc17,    /* 0x8C */
    ktwuxc18,    /* 0x9C */
    ktwuxc19,    /* 0xAC */
    ktwuxc1A,    /* 0xBC */
    (WORD)EQ_END,
    (WORD)EQ_END
};

const WORD  Ce_Low_Eq[]={
    ktc1c,       /* 0xD4 */
    ktc1d,       /* 0xDC */
    ktc1e,       /* 0xE4 */
    ktc1f,       /* 0xEC */
    (WORD)EQ_END,
    (WORD)EQ_END
};

const WORD  Ce_Lpf_Eq[]={
    kc20,
    kc21,
    kc22,
    (WORD)EQ_END,
    (WORD)EQ_END
};

const WORD  Tracking_U_Eq[]={
    ktu11,       /* 0x00 */
    ktu12,       /* 0x01 */
    ktu13,       /* 0x02 */
    ktu14,       /* 0x03 */
    ktu15,       /* 0x04 */
    ktu16,       /* 0x05 */
    ktu17,       /* 0x06 */
    ktu18,       /* 0x07 */
    ktu19,       /* 0x08 */
    ktu1A,       /* 0x09 */
    (WORD)EQ_END,
    (WORD)EQ_END
};

const WORD  Tracking_W_Eq[]={
    ktw11,       /* 0x00 */
    ktw12,       /* 0x01 */
    ktw13,       /* 0x02 */
    ktw14,       /* 0x03 */
    ktw15,       /* 0x04 */
    ktw16,       /* 0x05 */
    ktw17,       /* 0x06 */
    ktw18,       /* 0x07 */
    ktw19,       /* 0x08 */
    ktw1A,       /* 0x09 */
    (WORD)EQ_END,
    (WORD)EQ_END
};

const WORD  Focus_U_Eq[]={
    kfu11,       /* 0x20 */
    kfu12,       /* 0x21 */
    kfu13,       /* 0x22 */
    kfu14,       /* 0x23 */
    kfu15,       /* 0x24 */
    kfu16,       /* 0x25 */
    kfu17,       /* 0x26 */
    (WORD)EQ_END,
    (WORD)EQ_END
};

const WORD  Focus_W_Eq[]={
    kfw11,       /* 0x20 */
    kfw12,       /* 0x21 */
    kfw13,       /* 0x22 */
    kfw14,       /* 0x23 */
    kfw15,       /* 0x24 */
    kfw16,       /* 0x25 */
    kfw17,       /* 0x26 */
    (WORD)EQ_END,
    (WORD)EQ_END
};

#if (FINE_SEEK_SURF_TELFG == 0)
const svoWordU Fine_CE_Eq_UL[] = {CDFINE_CE_EQ_UL };
const svoWordU Fine_CE_Eq_L[]  = {CDFINE_CE_EQ_L  };
const svoWordU Fine_CE_Eq_M[]  = {CDFINE_CE_EQ_M  };
const svoWordU Fine_CE_Eq_H[]  = {CDFINE_CE_EQ_H  };
const svoWordU Fine_CE_Eq_UH[] = {CDFINE_CE_EQ_UH };

const svoWordU (*const FineCEEQCoefTbl[MAX_SPEED_CD]) =
{
    (svoWordU *const)Fine_CE_Eq_M,  // _4Lx
    (svoWordU *const)Fine_CE_Eq_L,   // _8Lx
    (svoWordU *const)Fine_CE_Eq_M,   // _12Lx
    (svoWordU *const)Fine_CE_Eq_H,   // _16Lx
    (svoWordU *const)Fine_CE_Eq_M,  // _10Ax
#if (ENABLE_CD_12XCAV == 1)
    (svoWordU *const)Fine_CE_Eq_L,   // _12AX
#endif // (ENABLE_CD_12XCAV == 1)
    (svoWordU *const)Fine_CE_Eq_M,   // _16AX
    (svoWordU *const)Fine_CE_Eq_M,   // _24Ax
    (svoWordU *const)Fine_CE_Eq_H,   // _32Ax
    (svoWordU *const)Fine_CE_Eq_H,   // _40Ax
    (svoWordU *const)Fine_CE_Eq_UH,  // _48Ax
    (svoWordU *const)Fine_CE_Eq_UH   // _52Ax
};
#endif // (FINE_SEEK_SURF_TELFG == 0)
const svoWordU RoughCEEQ[] = {CDROUGH_CE_EQ_R };

const svoWordU RoughCELFG[] = {CDROUGH_CE_LOW_EQ_R};

const svoWordU Tracking_Shock_Eq_Init[]={
    0x0000,                     /* kt30  (0x1A) */
    0x0000,                     /* kt31  (0x1B) */
    0x0000,                     /* kt32  (0x1C) */
    0x0000,                     /* kt33  (0x1D) */
    0x0000,                     /* kt34  (0x1E) */
    0x0000                      /* kt35  (0x1F) */
};

const svoWordU Focus_High_Eq_Init[]={
    0x7FFF,                     /* kf30  (0x2C) */
    0x0000,                     /* kf21  (0x2D) */
    0x0000                      /* kf32  (0x2E) */
};

const svoWordU Beam_Low_Eq_Init[]={
    0x5C00,                       /* kb10  (0x45) */
    0x7FA4,                       /* kb11  (0x46) */
    0x0080                        /* kb12  (0x47) */
};

const svoWordU Beam_High_Eq_Init[]={
    0x0000,                     /* Kb20  (0x48) */
    0x0000,                     /* Kb21  (0x49) */
    0x0000                      /* Kb22  (0x4A) */
};

const svoWordU Adjust_Eq_Init[]={
    FOCUS_SEARCH_KF18,          /* Kf18  (0x28) */
    0x7FFF,                     /* Kfb20 (0x55) */
    0x1000,                     /* Kfb21 (0x56) */
    0x0800,                     /* Krb10 (0x65) */
    0x0000                      /* Ks17  (0x3E) */
};

#if (CHIP_REV >= 0xC0)
const svoWordU CELPF_1kHz[] = {CDCELPF_normal};
const svoWordU CELPF_100Hz[] = {CDCELPF_unbal};
#endif

// UL-Group Coefficients (4XCLV, 10XCAV):
const svoWordU Tracking_Eq_UL[]       = {TRACKING_EQ_UL    };
const svoWordU Tracking_Low_Eq_UL[]   = {TRACKING_LOW_EQ_UL};
const svoWordU Tracking_DC_Eq_UL[]    = {TRK_DC_EQ_UL      };
const svoWordU Focus_Eq_UL[]          = {FOCUS_EQ_UL       };
const svoWordU Focus_Low_Eq_UL[]      = {FOCUS_LOW_EQ_UL   };
const svoWordU Focus_DC_Eq_UL[]       = {FOCUS_DC_EQ_UL    };
const svoWordU Speed_Eq_UL[]          = {SPEED_EQ_UL       };
const svoWordU Sled_Eq_UL[]           = {SLED_EQ_UL        };
const svoWordU Disk_Fg_Eq_UL[]        = {DISK_FG_EQ_UL     };

// L-Group Coefficients (8XCLV, 12XCAV, 16XCAV):
const svoWordU Tracking_Eq_L[]        = {TRACKING_EQ_L     };
const svoWordU Tracking_Low_Eq_L[]    = {TRACKING_LOW_EQ_L };
const svoWordU Tracking_DC_Eq_L[]     = {TRK_DC_EQ_L       };
const svoWordU Focus_Eq_L[]           = {FOCUS_EQ_L        };
const svoWordU Focus_Low_Eq_L[]       = {FOCUS_LOW_EQ_L    };
const svoWordU Focus_DC_Eq_L[]        = {FOCUS_DC_EQ_L     };
const svoWordU Speed_Eq_L[]           = {SPEED_EQ_L        };
const svoWordU Sled_Eq_L[]            = {SLED_EQ_L         };
const svoWordU Disk_Fg_Eq_L[]         = {DISK_FG_EQ_L      };

// M-Group Coefficients(10XCLV, 12XCLV, 24XCAV):
const svoWordU Tracking_Eq_M[]        = {TRACKING_EQ_M     };
const svoWordU Tracking_Low_Eq_M[]    = {TRACKING_LOW_EQ_M };
const svoWordU Tracking_DC_Eq_M[]     = {TRK_DC_EQ_M       };
const svoWordU Focus_Eq_M[]           = {FOCUS_EQ_M        };
const svoWordU Focus_Low_Eq_M[]       = {FOCUS_LOW_EQ_M    };
const svoWordU Focus_DC_Eq_M[]        = {FOCUS_DC_EQ_M     };
const svoWordU Speed_Eq_M[]           = {SPEED_EQ_M        };
const svoWordU Sled_Eq_M[]            = {SLED_EQ_M         };
const svoWordU Disk_Fg_Eq_M[]         = {DISK_FG_EQ_M      };

// H-Group Coefficients (16XCLV, 24XCLV, 32XCLV, 40XCLV, 32XCAV, 40XCAV):
const svoWordU Tracking_Eq_H[]        = {TRACKING_EQ_H     };
const svoWordU Tracking_Low_Eq_H[]    = {TRACKING_LOW_EQ_H };
const svoWordU Tracking_DC_Eq_H[]     = {TRK_DC_EQ_H       };
const svoWordU Focus_Eq_H[]           = {FOCUS_EQ_H        };
const svoWordU Focus_Low_Eq_H[]       = {FOCUS_LOW_EQ_H    };
const svoWordU Focus_DC_Eq_H[]        = {FOCUS_DC_EQ_H     };
const svoWordU Speed_Eq_H[]           = {SPEED_EQ_H        };
const svoWordU Sled_Eq_H[]            = {SLED_EQ_H         };
const svoWordU Disk_Fg_Eq_H[]         = {DISK_FG_EQ_H      };

// UH-Group Coefficients (48XCLV, 48XCAV, 52XCAV):
const svoWordU Tracking_Eq_UH[]       = {TRACKING_EQ_UH    };
const svoWordU Tracking_Low_Eq_UH[]   = {TRACKING_LOW_EQ_UH};
const svoWordU Tracking_DC_Eq_UH[]    = {TRK_DC_EQ_UH      };
const svoWordU Focus_Eq_UH[]          = {FOCUS_EQ_UH       };
const svoWordU Focus_Low_Eq_UH[]      = {FOCUS_LOW_EQ_UH   };
const svoWordU Focus_DC_Eq_UH[]       = {FOCUS_DC_EQ_UH    };
const svoWordU Speed_Eq_UH[]          = {SPEED_EQ_UH       };
const svoWordU Sled_Eq_UH[]           = {SLED_EQ_UH        };
const svoWordU Disk_Fg_Eq_UH[]        = {DISK_FG_EQ_UH     };

/********************************************/
/*              Read Equalizers             */
/********************************************/
const svoWordU (*const svoEQCoefTbl[MAX_SPEED_CD+1][EQ_TBL_MAX+1]) = {
/* NONE */
    {NULL,                                      /* TRACKING_EQ          */
     NULL,                                      /* TRACKING_LOW_EQ      */
     NULL,                                      /* TRACKING_DC_EQ       */
     (svoWordU *const) Tracking_Shock_Eq_Init,  /* TRACKING_SHOCK_EQ    */
     NULL,                                      /* FOCUS_EQ             */
     NULL,                                      /* FOCUS_LOW_EQ         */
     NULL,                                      /* FOCUS_DC_EQ          */
     (svoWordU *const) Focus_High_Eq_Init,      /* FOCUS_HIGH_EQ        */
     NULL,                                      /* DISK_FG_EQ           */
     NULL,                                      /* SLED_EQ              */
     (svoWordU *const) Beam_Low_Eq_Init,        /* BEAM_LOW_EQ          */
     (svoWordU *const) Beam_High_Eq_Init,       /* BEAM_HIGH_EQ         */
     NULL,                                      /* SPEED_EQ             */
     (svoWordU *const) Adjust_Eq_Init,          /* ADJUST_EQ            */
    },

/* X4 CLV */
    {(svoWordU *const) Tracking_Eq_UL,          /* TRACKING_EQ          */
     (svoWordU *const) Tracking_Low_Eq_UL,      /* TRACKING_LOW_EQ      */
     (svoWordU *const) Tracking_DC_Eq_UL,       /* TRACKING_DC_EQ       */
     NULL,                                      /* TRACKING_SHOCK_EQ    */
     (svoWordU *const) Focus_Eq_UL,             /* FOCUS_EQ             */
     (svoWordU *const) Focus_Low_Eq_UL,         /* FOCUS_LOW_EQ         */
     (svoWordU *const) Focus_DC_Eq_UL,          /* FOCUS_DC_EQ          */
     NULL,                                      /* FOCUS_HIGH_EQ        */
     (svoWordU *const) Disk_Fg_Eq_UL,           /* DISK_FG_EQ           */
     (svoWordU *const) Sled_Eq_UL,              /* SLED_EQ              */
     NULL,                                      /* BEAM_LOW_EQ          */
     NULL,                                      /* BEAM_HIGH_EQ         */
     (svoWordU *const) Speed_Eq_UL,             /* SPEED_EQ             */
     NULL,                                      /* ADJUST_EQ            */
    },

/* X8 CLV */
    {(svoWordU *const) Tracking_Eq_L,           /* TRACKING_EQ          */
     (svoWordU *const) Tracking_Low_Eq_L,       /* TRACKING_LOW_EQ      */
     (svoWordU *const) Tracking_DC_Eq_L,        /* TRACKING_DC_EQ       */
     NULL,                                      /* TRACKING_SHOCK_EQ    */
     (svoWordU *const) Focus_Eq_L,              /* FOCUS_EQ             */
     (svoWordU *const) Focus_Low_Eq_L,          /* FOCUS_LOW_EQ         */
     (svoWordU *const) Focus_DC_Eq_L,           /* FOCUS_DC_EQ          */
     NULL,                                      /* FOCUS_HIGH_EQ        */
     (svoWordU *const) Disk_Fg_Eq_L,            /* DISK_FG_EQ           */
     (svoWordU *const) Sled_Eq_L,               /* SLED_EQ              */
     NULL,                                      /* BEAM_LOW_EQ          */
     NULL,                                      /* BEAM_HIGH_EQ         */
     (svoWordU *const) Speed_Eq_L,              /* SPEED_EQ             */
     NULL,                                      /* ADJUST_EQ            */
    },

/* X12 CLV */
    {(svoWordU *const) Tracking_Eq_M,           /* TRACKING_EQ          */
     (svoWordU *const) Tracking_Low_Eq_M,       /* TRACKING_LOW_EQ      */
     (svoWordU *const) Tracking_DC_Eq_M,        /* TRACKING_DC_EQ       */
     NULL,                                      /* TRACKING_SHOCK_EQ    */
     (svoWordU *const) Focus_Eq_M,              /* FOCUS_EQ             */
     (svoWordU *const) Focus_Low_Eq_M,          /* FOCUS_LOW_EQ         */
     (svoWordU *const) Focus_DC_Eq_M,           /* FOCUS_DC_EQ          */
     NULL,                                      /* FOCUS_HIGH_EQ        */
     (svoWordU *const) Disk_Fg_Eq_M,            /* DISK_FG_EQ           */
     (svoWordU *const) Sled_Eq_M,               /* SLED_EQ              */
     NULL,                                      /* BEAM_LOW_EQ          */
     NULL,                                      /* BEAM_HIGH_EQ         */
     (svoWordU *const) Speed_Eq_M,              /* SPEED_EQ             */
     NULL,                                      /* ADJUST_EQ            */
    },

/* X16 CLV */
    {(svoWordU *const) Tracking_Eq_H,           /* TRACKING_EQ          */
     (svoWordU *const) Tracking_Low_Eq_H,       /* TRACKING_LOW_EQ      */
     (svoWordU *const) Tracking_DC_Eq_H,        /* TRACKING_DC_EQ       */
     NULL,                                      /* TRACKING_SHOCK_EQ    */
     (svoWordU *const) Focus_Eq_H,              /* FOCUS_EQ             */
     (svoWordU *const) Focus_Low_Eq_H,          /* FOCUS_LOW_EQ         */
     (svoWordU *const) Focus_DC_Eq_H,           /* FOCUS_DC_EQ          */
     NULL,                                      /* FOCUS_HIGH_EQ        */
     (svoWordU *const) Disk_Fg_Eq_H,            /* DISK_FG_EQ           */
     (svoWordU *const) Sled_Eq_H,               /* SLED_EQ              */
     NULL,                                      /* BEAM_LOW_EQ          */
     NULL,                                      /* BEAM_HIGH_EQ         */
     (svoWordU *const) Speed_Eq_H,              /* SPEED_EQ             */
     NULL,                                      /* ADJUST_EQ            */
    },

/* X10 CAV */
    {(svoWordU *const) Tracking_Eq_UL,          /* TRACKING_EQ          */
     (svoWordU *const) Tracking_Low_Eq_UL,      /* TRACKING_LOW_EQ      */
     (svoWordU *const) Tracking_DC_Eq_UL,       /* TRACKING_DC_EQ       */
     NULL,                                      /* TRACKING_SHOCK_EQ    */
     (svoWordU *const) Focus_Eq_UL,             /* FOCUS_EQ             */
     (svoWordU *const) Focus_Low_Eq_UL,         /* FOCUS_LOW_EQ         */
     (svoWordU *const) Focus_DC_Eq_UL,          /* FOCUS_DC_EQ          */
     NULL,                                      /* FOCUS_HIGH_EQ        */
     (svoWordU *const) Disk_Fg_Eq_UL,           /* DISK_FG_EQ           */
     (svoWordU *const) Sled_Eq_UL,              /* SLED_EQ              */
     NULL,                                      /* BEAM_LOW_EQ          */
     NULL,                                      /* BEAM_HIGH_EQ         */
     (svoWordU *const) Speed_Eq_UL,             /* SPEED_EQ             */
     NULL,                                      /* ADJUST_EQ            */
    },

#if (ENABLE_CD_12XCAV == 1)
/* X12 CAV */
    {(svoWordU *const) Tracking_Eq_L,           /* TRACKING_EQ          */
     (svoWordU *const) Tracking_Low_Eq_L,       /* TRACKING_LOW_EQ      */
     (svoWordU *const) Tracking_DC_Eq_L,        /* TRACKING_DC_EQ       */
     NULL,                                      /* TRACKING_SHOCK_EQ    */
     (svoWordU *const) Focus_Eq_L,              /* FOCUS_EQ             */
     (svoWordU *const) Focus_Low_Eq_L,          /* FOCUS_LOW_EQ         */
     (svoWordU *const) Focus_DC_Eq_L,           /* FOCUS_DC_EQ          */
     NULL,                                      /* FOCUS_HIGH_EQ        */
     (svoWordU *const) Disk_Fg_Eq_L,            /* DISK_FG_EQ           */
     (svoWordU *const) Sled_Eq_L,               /* SLED_EQ              */
     NULL,                                      /* BEAM_LOW_EQ          */
     NULL,                                      /* BEAM_HIGH_EQ         */
     (svoWordU *const) Speed_Eq_L,              /* SPEED_EQ             */
     NULL,                                      /* ADJUST_EQ            */
    },
#else // (ENABLE_CD_12XCAV == 1)
/* X16 CAV */
    {(svoWordU *const) Tracking_Eq_L,           /* TRACKING_EQ          */
     (svoWordU *const) Tracking_Low_Eq_L,       /* TRACKING_LOW_EQ      */
     (svoWordU *const) Tracking_DC_Eq_L,        /* TRACKING_DC_EQ       */
     NULL,                                      /* TRACKING_SHOCK_EQ    */
     (svoWordU *const) Focus_Eq_L,              /* FOCUS_EQ             */
     (svoWordU *const) Focus_Low_Eq_L,          /* FOCUS_LOW_EQ         */
     (svoWordU *const) Focus_DC_Eq_L,           /* FOCUS_DC_EQ          */
     NULL,                                      /* FOCUS_HIGH_EQ        */
     (svoWordU *const) Disk_Fg_Eq_L,            /* DISK_FG_EQ           */
     (svoWordU *const) Sled_Eq_L,               /* SLED_EQ              */
     NULL,                                      /* BEAM_LOW_EQ          */
     NULL,                                      /* BEAM_HIGH_EQ         */
     (svoWordU *const) Speed_Eq_L,              /* SPEED_EQ             */
     NULL,                                      /* ADJUST_EQ            */
    },
#endif // (ENABLE_CD_12XCAV == 1)

/* X24 CAV */
    {(svoWordU *const) Tracking_Eq_M,           /* TRACKING_EQ          */
     (svoWordU *const) Tracking_Low_Eq_M,       /* TRACKING_LOW_EQ      */
     (svoWordU *const) Tracking_DC_Eq_M,        /* TRACKING_DC_EQ       */
     NULL,                                      /* TRACKING_SHOCK_EQ    */
     (svoWordU *const) Focus_Eq_M,              /* FOCUS_EQ             */
     (svoWordU *const) Focus_Low_Eq_M,          /* FOCUS_LOW_EQ         */
     (svoWordU *const) Focus_DC_Eq_M,           /* FOCUS_DC_EQ          */
     NULL,                                      /* FOCUS_HIGH_EQ        */
     (svoWordU *const) Disk_Fg_Eq_M,            /* DISK_FG_EQ           */
     (svoWordU *const) Sled_Eq_M,               /* SLED_EQ              */
     NULL,                                      /* BEAM_LOW_EQ          */
     NULL,                                      /* BEAM_HIGH_EQ         */
     (svoWordU *const) Speed_Eq_M,              /* SPEED_EQ             */
     NULL,                                      /* ADJUST_EQ            */
    },

/* X32 CAV */
    {(svoWordU *const) Tracking_Eq_H,           /* TRACKING_EQ          */
     (svoWordU *const) Tracking_Low_Eq_H,       /* TRACKING_LOW_EQ      */
     (svoWordU *const) Tracking_DC_Eq_H,        /* TRACKING_DC_EQ       */
     NULL,                                      /* TRACKING_SHOCK_EQ    */
     (svoWordU *const) Focus_Eq_H,              /* FOCUS_EQ             */
     (svoWordU *const) Focus_Low_Eq_H,          /* FOCUS_LOW_EQ         */
     (svoWordU *const) Focus_DC_Eq_H,           /* FOCUS_DC_EQ          */
     NULL,                                      /* FOCUS_HIGH_EQ        */
     (svoWordU *const) Disk_Fg_Eq_H,            /* DISK_FG_EQ           */
     (svoWordU *const) Sled_Eq_H,               /* SLED_EQ              */
     NULL,                                      /* BEAM_LOW_EQ          */
     NULL,                                      /* BEAM_HIGH_EQ         */
     (svoWordU *const) Speed_Eq_H,              /* SPEED_EQ             */
     NULL,                                      /* ADJUST_EQ            */
    },

/* X40 CAV */
    {(svoWordU *const) Tracking_Eq_UH,          /* TRACKING_EQ          */
     (svoWordU *const) Tracking_Low_Eq_UH,      /* TRACKING_LOW_EQ      */
     (svoWordU *const) Tracking_DC_Eq_UH,       /* TRACKING_DC_EQ       */
     NULL,                                      /* TRACKING_SHOCK_EQ    */
     (svoWordU *const) Focus_Eq_UH,             /* FOCUS_EQ             */
     (svoWordU *const) Focus_Low_Eq_UH,         /* FOCUS_LOW_EQ         */
     (svoWordU *const) Focus_DC_Eq_UH,          /* FOCUS_DC_EQ          */
     NULL,                                      /* FOCUS_HIGH_EQ        */
     (svoWordU *const) Disk_Fg_Eq_UH,           /* DISK_FG_EQ           */
     (svoWordU *const) Sled_Eq_UH,              /* SLED_EQ              */
     NULL,                                      /* BEAM_LOW_EQ          */
     NULL,                                      /* BEAM_HIGH_EQ         */
     (svoWordU *const) Speed_Eq_UH,             /* SPEED_EQ             */
     NULL,                                      /* ADJUST_EQ            */
    },

/* X48 CAV */
    {(svoWordU *const) Tracking_Eq_UH,          /* TRACKING_EQ          */
     (svoWordU *const) Tracking_Low_Eq_UH,      /* TRACKING_LOW_EQ      */
     (svoWordU *const) Tracking_DC_Eq_UH,       /* TRACKING_DC_EQ       */
     NULL,                                      /* TRACKING_SHOCK_EQ    */
     (svoWordU *const) Focus_Eq_UH,             /* FOCUS_EQ             */
     (svoWordU *const) Focus_Low_Eq_UH,         /* FOCUS_LOW_EQ         */
     (svoWordU *const) Focus_DC_Eq_UH,          /* FOCUS_DC_EQ          */
     NULL,                                      /* FOCUS_HIGH_EQ        */
     (svoWordU *const) Disk_Fg_Eq_UH,           /* DISK_FG_EQ           */
     (svoWordU *const) Sled_Eq_UH,              /* SLED_EQ              */
     NULL,                                      /* BEAM_LOW_EQ          */
     NULL,                                      /* BEAM_HIGH_EQ         */
     (svoWordU *const) Speed_Eq_UH,             /* SPEED_EQ             */
     NULL,                                      /* ADJUST_EQ            */
    },

/* X52 CAV */
    {(svoWordU *const) Tracking_Eq_UH,          /* TRACKING_EQ          */
     (svoWordU *const) Tracking_Low_Eq_UH,      /* TRACKING_LOW_EQ      */
     (svoWordU *const) Tracking_DC_Eq_UH,       /* TRACKING_DC_EQ       */
     NULL,                                      /* TRACKING_SHOCK_EQ    */
     (svoWordU *const) Focus_Eq_UH,             /* FOCUS_EQ             */
     (svoWordU *const) Focus_Low_Eq_UH,         /* FOCUS_LOW_EQ         */
     (svoWordU *const) Focus_DC_Eq_UH,          /* FOCUS_DC_EQ          */
     NULL,                                      /* FOCUS_HIGH_EQ        */
     (svoWordU *const) Disk_Fg_Eq_UH,           /* DISK_FG_EQ           */
     (svoWordU *const) Sled_Eq_UH,              /* SLED_EQ              */
     NULL,                                      /* BEAM_LOW_EQ          */
     NULL,                                      /* BEAM_HIGH_EQ         */
     (svoWordU *const) Speed_Eq_UH,             /* SPEED_EQ             */
     NULL,                                      /* ADJUST_EQ            */
    }
};

/*----------------------------------------------------------------------------*/
/* Fine search KICKLVL coefficient                                            */
/*----------------------------------------------------------------------------*/
SEEK_GAIN const svoKICKLVL_UL_F[] = {KICKLVL_EQ_UL_F};
SEEK_GAIN const svoKICKLVL_L_F[]  = {KICKLVL_EQ_L_F};
SEEK_GAIN const svoKICKLVL_M_F[]  = {KICKLVL_EQ_M_F};
SEEK_GAIN const svoKICKLVL_H_F[]  = {KICKLVL_EQ_H_F};
SEEK_GAIN const svoKICKLVL_UH_F[] = {KICKLVL_EQ_UH_F};

const SEEK_GAIN *const svoKICKLVLTbl[MAX_SPEED_CD] =
{
    (SEEK_GAIN *const) svoKICKLVL_UL_F,  // _4Lx
    (SEEK_GAIN *const) svoKICKLVL_L_F,   // _8Lx
    (SEEK_GAIN *const) svoKICKLVL_M_F,   // _12Lx
    (SEEK_GAIN *const) svoKICKLVL_H_F,   // _16Lx
    (SEEK_GAIN *const) svoKICKLVL_UL_F,  // _10Ax
    (SEEK_GAIN *const) svoKICKLVL_L_F,   // _12Ax/_16Ax
    (SEEK_GAIN *const) svoKICKLVL_M_F,   // _24Ax
    (SEEK_GAIN *const) svoKICKLVL_H_F,   // _32Ax
    (SEEK_GAIN *const) svoKICKLVL_H_F,   // _40Ax
    (SEEK_GAIN *const) svoKICKLVL_UH_F,  // _48Ax
    (SEEK_GAIN *const) svoKICKLVL_UH_F,  // _52Ax
};
/*----------------------------------------------------------------------------*/
/* End Fine search KICKLVL coefficient                                        */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Fine search BRAKELVL coefficient                                            */
/*----------------------------------------------------------------------------*/
SEEK_GAIN const svoBRAKELVL_UL_F[] = {BRAKELVL_EQ_UL_F};
SEEK_GAIN const svoBRAKELVL_L_F[]  = {BRAKELVL_EQ_L_F};
SEEK_GAIN const svoBRAKELVL_M_F[]  = {BRAKELVL_EQ_M_F};
SEEK_GAIN const svoBRAKELVL_H_F[]  = {BRAKELVL_EQ_H_F};
SEEK_GAIN const svoBRAKELVL_UH_F[] = {BRAKELVL_EQ_UH_F};

const SEEK_GAIN *const svoBRAKELVLTbl[MAX_SPEED_CD] =
{
    (SEEK_GAIN *const) svoBRAKELVL_UL_F,  // _4Lx
    (SEEK_GAIN *const) svoBRAKELVL_L_F,   // _8Lx
    (SEEK_GAIN *const) svoBRAKELVL_M_F,   // _12Lx
    (SEEK_GAIN *const) svoBRAKELVL_H_F,   // _16Lx
    (SEEK_GAIN *const) svoBRAKELVL_UL_F,  // _10Ax
    (SEEK_GAIN *const) svoBRAKELVL_L_F,   // _12Ax/_16Ax
    (SEEK_GAIN *const) svoBRAKELVL_M_F,   // _24Ax
    (SEEK_GAIN *const) svoBRAKELVL_H_F,   // _32Ax
    (SEEK_GAIN *const) svoBRAKELVL_H_F,   // _40Ax
    (SEEK_GAIN *const) svoBRAKELVL_UH_F,  // _48Ax
    (SEEK_GAIN *const) svoBRAKELVL_UH_F,  // _52Ax
};
/*----------------------------------------------------------------------------*/
/* End Fine search BRAKELVL coefficient                                       */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Fine search BRAKETIME coefficient                                            */
/*----------------------------------------------------------------------------*/
const BYTE svoBRAKETIMETbl[MAX_SPEED_CD] =
{
    BRAKETIME_EQ_UL,    // _4Lx
    BRAKETIME_EQ_L,     // _8Lx
    BRAKETIME_EQ_M,     // _12Lx
    BRAKETIME_EQ_H,     // _16Lx
    BRAKETIME_EQ_UL,    // _10Ax
    BRAKETIME_EQ_L,     // _12Ax/_16Ax
    BRAKETIME_EQ_M,     // _24Ax
    BRAKETIME_EQ_H,     // _32Ax
    BRAKETIME_EQ_H,     // _40Ax
    BRAKETIME_EQ_UH,    // _48Ax
    BRAKETIME_EQ_UH,    // _52Ax
};

/*----------------------------------------------------------------------------*/
/* Fine search BRAKESPD coefficient                                          */
/*----------------------------------------------------------------------------*/
const WORD svoBRAKESPDTbl[MAX_SPEED_CD] =
{
    BRAKESPD_EQ_UL,    // _4Lx
    BRAKESPD_EQ_L,     // _8Lx
    BRAKESPD_EQ_M,     // _12Lx
    BRAKESPD_EQ_H,     // _16Lx
    BRAKESPD_EQ_UL,    // _10Ax
    BRAKESPD_EQ_L,     // _12Ax/_16Ax
    BRAKESPD_EQ_M,     // _24Ax
    BRAKESPD_EQ_H,     // _32Ax
    BRAKESPD_EQ_H,     // _40Ax
    BRAKESPD_EQ_UH,    // _48Ax
    BRAKESPD_EQ_UH,    // _52Ax
};

/*----------------------------------------------------------------------------*/
/* End Fine search BRAKETIME coefficient                                       */
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Fine search kt1B(0x0B) coefficient                                         */
/*----------------------------------------------------------------------------*/
SEEK_GAIN const svokt1BKick_UL_F[] = {Kt1BKick_EQ_UL_F};
SEEK_GAIN const svokt1BKick_L_F[]  = {Kt1BKick_EQ_L_F};
SEEK_GAIN const svokt1BKick_M_F[]  = {Kt1BKick_EQ_M_F};
SEEK_GAIN const svokt1BKick_H_F[]  = {Kt1BKick_EQ_H_F};
SEEK_GAIN const svokt1BKick_UH_F[] = {Kt1BKick_EQ_UH_F};

const SEEK_GAIN *const svokt1BKickTbl[MAX_SPEED_CD] =
{
    (SEEK_GAIN *const) svokt1BKick_UL_F,  // _4Lx
    (SEEK_GAIN *const) svokt1BKick_L_F,   // _8Lx
    (SEEK_GAIN *const) svokt1BKick_M_F,   // _12Lx
    (SEEK_GAIN *const) svokt1BKick_H_F,   // _16Lx
    (SEEK_GAIN *const) svokt1BKick_UL_F,  // _10Ax
    (SEEK_GAIN *const) svokt1BKick_L_F,   // _12Ax/_16Ax
    (SEEK_GAIN *const) svokt1BKick_M_F,   // _24Ax
    (SEEK_GAIN *const) svokt1BKick_H_F,   // _32Ax
    (SEEK_GAIN *const) svokt1BKick_H_F,   // _40Ax
    (SEEK_GAIN *const) svokt1BKick_UH_F,  // _48Ax
    (SEEK_GAIN *const) svokt1BKick_UH_F,  // _52Ax
};

#if (ACTIVE_SLED_FOR_FINE_JUMP == 0)
/*----------------------------------------------------------------------------*/
/* Fine search ks17(0x3E) coefficient                                         */
/*----------------------------------------------------------------------------*/
SEEK_GAIN const svoks17Kick_UL_F_000[]  = {svoks17Kick_UL_F};
SEEK_GAIN const svoks17Kick_UL_B_000[]  = {svoks17Kick_UL_B};
SEEK_GAIN const svoks17Kick_L_F_000[]   = {svoks17Kick_L_F};
SEEK_GAIN const svoks17Kick_L_B_000[]   = {svoks17Kick_L_B};
SEEK_GAIN const svoks17Kick_M_F_000[]   = {svoks17Kick_M_F};
SEEK_GAIN const svoks17Kick_M_B_000[]   = {svoks17Kick_M_B};
SEEK_GAIN const svoks17Kick_H_F_000[]   = {svoks17Kick_H_F};
SEEK_GAIN const svoks17Kick_H_B_000[]   = {svoks17Kick_H_B};
SEEK_GAIN const svoks17Kick_UH_F_000[]  = {svoks17Kick_UH_F};
SEEK_GAIN const svoks17Kick_UH_B_000[]  = {svoks17Kick_UH_B};

const SEEK_GAIN *const svoks17KickTbl[2][MAX_SPEED_CD]={
    {(SEEK_GAIN *const) svoks17Kick_M_F_000,  // _4Lx       FORW
     (SEEK_GAIN *const) svoks17Kick_L_F_000,   // _8LX
     (SEEK_GAIN *const) svoks17Kick_M_F_000,   // _12Lx
     (SEEK_GAIN *const) svoks17Kick_H_F_000,   // _16Lx
     (SEEK_GAIN *const) svoks17Kick_M_F_000,  // _10Ax
     (SEEK_GAIN *const) svoks17Kick_M_F_000,   // _12Ax/_16Ax
     (SEEK_GAIN *const) svoks17Kick_M_F_000,   // _24Ax
     (SEEK_GAIN *const) svoks17Kick_H_F_000,   // _32Ax
     (SEEK_GAIN *const) svoks17Kick_H_F_000,   // _40Ax
     (SEEK_GAIN *const) svoks17Kick_UH_F_000,  // _48Ax
     (SEEK_GAIN *const) svoks17Kick_UH_F_000}, // _52Ax
/*-----------------------------------------------------------*/
    {(SEEK_GAIN *const) svoks17Kick_M_B_000,  // _4Lx      BACK
     (SEEK_GAIN *const) svoks17Kick_L_B_000,   // _8LX
     (SEEK_GAIN *const) svoks17Kick_M_B_000,   // _12Lx
     (SEEK_GAIN *const) svoks17Kick_H_B_000,   // _16Lx
     (SEEK_GAIN *const) svoks17Kick_M_B_000,  // _10Ax
     (SEEK_GAIN *const) svoks17Kick_M_B_000,   // _12Ax/_16Ax
     (SEEK_GAIN *const) svoks17Kick_M_B_000,   // _24Ax
     (SEEK_GAIN *const) svoks17Kick_H_B_000,   // _32Ax
     (SEEK_GAIN *const) svoks17Kick_H_B_000,   // _40Ax
     (SEEK_GAIN *const) svoks17Kick_UH_B_000,  // _48Ax
     (SEEK_GAIN *const) svoks17Kick_UH_B_000}  // _52Ax
};
#endif // (ACTIVE_SLED_FOR_FINE_JUMP == 0)
/*------------------------------------------------------------------------*/
/* The end of file                                                        */
/*------------------------------------------------------------------------*/
