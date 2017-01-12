/****************************************************************************
*                (c) Copyright 2001 - 2007 SUNEXT DESIGN, INC.
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
*
* $Revision: 80 $
* $Date: 11/03/28 4:07p $
*
**************************** SUNEXT CONFIDENTIAL ****************************/

#include ".\common\globals.h"
#include ".\common\globtype.h"
#include ".\common\chip.h"
#include ".\AL\REG_cd_encoder.h"
#include ".\servo\svo.h"
#include ".\servo\svo_com.h"
#include ".\servo\svo_tbl.h"
#include ".\AL\REG_servo.h"
#include ".\AL\REG_write_strategy.h"
#include ".\AL\REG_global.h"
#include ".\AL\REG_periph.h"
#include ".\AL\REG_dvd_encoder.h"

#if (BD_ENABLE == 1)
/*----------------------------------------------------------------------------*/

#define DMCNT_BD   (0x48|(DFRQ_FG<<2)|(DFRQ_WBL_DEC*0x03))
#define SPINCTL_BD ((DISK_AFCG_BD << 1))

/*
 * Servo initial command data table
 */
svoRegInitTblT const bdsvoRegInitNONETbl[] =
{
    {HAL_SRV_SYNCIEN_ADR,       0x0000,       WORDSET}, /* 0xB00003E0[12:0] disable Interrupt = SEEKC */
    /* Servo, HW stepping */
    {HAL_SRV_TIMPER_ADR,          0x0064,       BYTESET}, /* 0xB0000024[7:0] Set HW stepping timer = 100us */
    {HAL_SRV_HWSTPSEEK_ADR,       0x0000,       BYTESET}, /* 0xB0000028[7:0] Set DAC value from stepper motor block */
    {HAL_SRV_SEEKSTART_ADR,       0x0010,       BYTESET}, /* 0xB0000040[7:0] Set Full step use square wave */
    {HAL_SRV_HYSLTHLD_ADR,        0x0003,       BYTESET}, /* 0xB00000B0[7:0] SLED DAC value low threshold value */
    {HAL_SRV_HYSHTHLD_ADR,        0x0003,       BYTESET}, /* 0xB00000B4[7:0] SLED DAC value high threshold value */

    /* Servo */
    {HAL_SRV_FGAT_ADR,             0x0000,       BYTESET}, /* 0xB0000344[7:0] Stop focus adjustment */
    {HAL_SRV_TGAT_ADR,             0x0000,       BYTESET}, /* 0xB000034C[7:0] Stop tracking adjustment */
    {HAL_SRV_MSKON_ADR,            0x0051|(DISK_FR2X<<HAL_SRV_FR2X_LSB),BYTESET}, /* 0xB000036C[7:0] double FG,invert FG, use FG noise mask, TR/FE loop gain excitation freq = 2.75K */
    #if (FOOTILT_DIFFERENTIAL == 1)
    {HAL_SRV_FOODIFEN_ADR,         0x0010,       BYTESET}, /* 0xB0000118[7:0] SERVO DAC Switch */
    #else
    {HAL_SRV_FOODIFEN_ADR,         0x0000,       BYTESET}, /* 0xB0000118[7:0] SERVO DAC Switch */
    #endif
    {HAL_SRV_FETHR_ADR,           0x007F,       BYTESET}, /* 0xB000011C[7:0] FE threshold value */
    {HAL_SRV_FETHREN_ADR, HAL_SRV_FETHREN,       BIT_ON }, /* 0xB0000120[7:0] FE threshold enable */
    {HAL_SRV_AFCRNG1_ADR,         0x0020,       BYTESET}, /* 0xB0000138[6:0] AFC in range low */
    {HAL_SRV_AFCRNG2_ADR,         0x0038,       BYTESET}, /* 0xB000013C[6:0] AFC in range high */
    {HAL_SRV_SRVMOD_ADR,          0x0019,       BYTESET}, /* 0xB0000140[7:0] REGTON=0,DPLLMSK=0,DECCHG=010,SRVMOD=1 */
    {HAL_SRV_AFCG_ADR,       SPINCTL_BD,       BYTESET}, /* 0xB00001DC[7:0] AFCG */
    {HAL_SRV_FGCMP_ADR,           0x013D,       WORDSET}, /* 0xB0000148[15:0] 33.8688e6/64/(2*23.14*36) = 635 */
    {HAL_SRV_FGRNG_ADR,           0x0020,       WORDSET}, /* 0xB00001F8[15:0] FG count range */
    {HAL_SRV_NDIV_ADR,            0x0002,       WORDSET}, /* 0xB00001F0[9:0] CLVN = 1 */
    {HAL_SRV_DFRQFG_ADR,       DMCNT_BD,       BYTESET}, /* 0xB00003AC[7:0] disc motor control register */
    {HAL_SRV_APCGAIN_ADR,         0x0000,       BYTESET}, /* 0xB0000174[7:0] rwAPCGAIN=0x15 */
    {HAL_SRV_APCNUM_ADR,          0x0000,       BYTESET}, /* 0xB0000178[3:0] APCNUM=1 */
    {HAL_SRV_WAITWBL_ADR,         0x000F,       BYTESET}, /* 0xB0000170[3:0] wobble sync lock time */
    {HAL_SRV_TSVSTP_ADR,          0x0000,       BYTESET}, /* 0xB000020C.7    stop PICUP register */
    {HAL_SRV_LDOFF_ADR,           0x0000,       BYTESET}, /* 0xB000021C[7:0] LDON = 0 */
    {HAL_SRV_PC_ADR,              0x0000,       BYTESET}, /* 0xB0000234[7:0] PAM duty cycle 100% */
    {HAL_SRV_PF_ADR,              0x0000,       BYTESET}, /* 0xB0000238[7:0] PAM frequency 172.2Hz */
    {HAL_SRV_DAC2SEL_ADR,         0x0020,       BYTESET}, /* 0xB000029C[7:0] output as DACDAT2/DACDAT1 */
    {HAL_SRV_FBAVS_ADR,           0x00C0,       BYTESET}, /* 0xB0000350[7:0] FBAVS,FBAVS4 = 88Hz,FBASE = 1b,FQWDSEL,FQWDSEL4 = 2.5ms */
    {HAL_SRV_TROFFSET_ADR,        0x0000,       BYTESET}, /* 0xB0000370[7:0] */
    #if (CHIP_REV <= 0xB3)
    //{HAL_SRV_FSL_ADR,             0x0030,       BYTESET}, /* 0xB0000384[6:0] FSL 6-0 Focus search standby level = 0.453 v */ //Lower FSL for focus capture
    {HAL_SRV_FSL_ADR,             0x0018,       BYTESET}, /* 0xB0000384[6:0] FSL 6-0 Focus search standby level = 0.453 v */
    #endif
    {HAL_SRV_FOKL_ADR,            0x0018,       BYTESET}, /* 0xB00003D0[7:0] FOKL 6-0 Focus OK Level = 0.226 v */
    {HAL_SRV_SHL_ADR,             0x00F6,       BYTESET}, /* 0xB000038C[7:0] Shock Threshold = 880mV,Shock Delay Time = default */
    {HAL_SRV_THOLD_ADR,           0x000C,       BYTESET}, /* 0xB0000390[7:0] Gain Up Time = 2~3ms,Hysteresis Time = 2~3ms,FHOLD = 1b,THOLD = 1b,GUPCL = 0b,XCOEF = 0b */
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
#else
    {HAL_SRV_FGSW_ADR,            0x0002,       BYTESET}, /* 0xB000039C[7:0] Foucs Gain Input = 128,Tracking Gain Input = 128,FOSW = 0b,TRSW = 0b,FDSW = 0b */
#endif
    #if (CHIP_REV <= 0xB3)
    {HAL_SRV_AFEDFCT_ADR, HAL_SRV_AFEDFCT,        BIT_ON}, /* 0xB00003B0[7:0] defect select afe */
    #else
    {HAL_SRV_DEEPDFCT_ADR, HAL_SRV_DEEPDFCT,        BIT_ON}, /* 0xB00003B0[7:0] defect select afe */
    #endif
    {HAL_SRV_ZTK_ADR,             0x0000,       BYTESET}, /* 0xB00003B4[7:0] ZTK = 0b, PGTK = 0b, PLF = 0b, RXP = 0b, TXP = 0b */
    {HAL_SRV_DVDRHHLD_ADR,        0x0001,       BYTESET}, /* 0xB00003B8.0 DVD-RAM TE/FE hold during Header */
    {HAL_SRV_SYNCI_ADR,           0x0001,       WORDSET}, /* 0xB00003E8[12:0] Clear Interrput Status except the last bit */
    {HAL_SRV_DBT_ADR,             0x0000,       BYTESET}, /* 0xB0000334[6:0] Defect Delay Time = 8.4us */

#if (CHIP_REV <= 0xB3)
    //Set HW kick/brake parameters
    {HAL_SRV_ZCEAREN_ADR,         0x0033,       BYTESET}, /* 0xB0000058[7:0] EARZCPROT */
    {HAL_SRV_BRAKELVL_ADR,        0x0000,       BYTESET}, /* 0xB000005C[7:0] BRAKELVL */
    {HAL_SRV_KICKLVL_ADR,         0x0070,       BYTESET}, /* 0xB0000060[7:0] KICKLVL */
    {HAL_SRV_BRAKEHLD_ADR,        0x0000,       BYTESET}, /* 0xB0000068[7:0] BRAKEHLD */
    {HAL_SRV_BRAKEEN_ADR,         0x00E0,       BYTESET}, /* 0xB000006C[7:0] BRAKECNT */
    {HAL_SRV_BRAKESPD_ADR,        0x0000,       BYTESET}, /* 0xB0000064[7:0] SPEEDLVL */
#endif

    /* GLOBAL_PAGE */
    {HAL_GLOBAL_SLPSRVADC_ADR,    0x0000,       BYTESET}, /* 0x10000702[7:0] No power down on CD-Servo, Servo-DAC, GP-ADC,*/
    {HAL_GLOBAL_WBLRSMP_ADR,      0x0004,       BYTESET}, /* 0x10000780[2:0] WPLTSH = SMP4 */
    {HAL_GLOBAL_WBLWSMP_ADR,      0x0003,       BYTESET}, /* 0x10000781[2:0] WBLSH  = SMP3 */
    {HAL_GLOBAL_MPDSMP_ADR,       0x0002,       BYTESET}, /* 0x10000782[2:0] MPDSH  = SMP2 */
    {HAL_GLOBAL_SPDSMP_ADR,       0x0002,       BYTESET}, /* 0x10000783[2:0] SPDSH  = SMP2 */
    {HAL_GLOBAL_WRFSMP_ADR,       0x0002,       BYTESET}, /* 0x10000784[2:0] WREFSH = SMP2 */
    {HAL_GLOBAL_RFPDSMP_ADR,      0x0005,       BYTESET}, /* 0x10000785[2:0] RFPDSH = SMP5 */
    {HAL_GLOBAL_WFPDSMP_ADR,      0x0001,       BYTESET}, /* 0x10000786[2:0] WFPDSH = SMP1 */
    {HAL_GLOBAL_RFADCFRA_ADR,     0x0000,       BYTESET}, /* 0x10000730[5:4] RFADCFRA(1-0) = 0 */

    /* PERIPHERALS PAGE */
    {HAL_PERIPH_ADCGAIN_ADR,     ADCGAIN,       BYTESET}, /* 0x100005F4.0 */

    {SVOREGTBL_END,               0x0000,       BYTESET}    /* end code */
}; /* svoRegInitbl[] */

svoRegInitTblT const bdsvoRegInit1LXTbl[] =
{
    /* SERVO*/
    {HAL_SRV_NDIV_ADR,           0x0001,       WORDSET}, /* 0xB00001F0[9:0] CLVN = 1 */
    {HAL_SRV_DECCMP_ADR,         0x0C0A,       WORDSET}, /* 0xB0000158[15:0] 33.8688e6/8790/1.25 = 3082 */
    {HAL_SRV_DBT_ADR,            0x0000,       BYTESET}, /* 0xB0000334[6:0] Defect Delay Time */
#if (CHIP_REV <= 0xB3)
    #if (PCB == 815)
        {HAL_SRV_FVD_ADR,            0x0028,       BYTESET}, /* 0xB0000398[7:0] KWIDSL = 0us,FVD = 8.40KHz */
    #else
        {HAL_SRV_FVD_ADR,            0x0018,       BYTESET}, /* 0xB0000398[7:0] KWIDSL = 0us,FVD = 5.52KHz */
    #endif
#endif
    /* Wobble page*/
//    {rwDFCTIME,         0x66,       BYTESET}, //TBD:SE1  /* 0x1026 ADIP wobble defect time*/

    {SVOREGTBL_END,              0x0000,       BYTESET}    /* end code */
}; /* svoRegInit1XTbl[] */

svoRegInitTblT const bdsvoRegInit2LXTbl[] =
{
    /* SERVO*/
    {HAL_SRV_NDIV_ADR,           0x0002,       WORDSET}, /* 0xB00001F0[9:0] CLVN = 2 */
    {HAL_SRV_DECCMP_ADR,         0x0C8A,       WORDSET}, /* 0xB0000158[15:0] 33.8688e6/8790/1.2 = 3853 */
    {HAL_SRV_DBT_ADR,            0x0000,       BYTESET}, /* 0xB0000334[6:0] Defect Delay Time */
#if (CHIP_REV <= 0xB3)
    #if (PCB == 815)
        {HAL_SRV_FVD_ADR,            0x0028,       BYTESET}, /* 0xB0000398[7:0] KWIDSL = 0us,FVD = 8.40KHz */
    #else
        {HAL_SRV_FVD_ADR,            0x0018,       BYTESET}, /* 0xB0000398[7:0] KWIDSL = 0us,FVD = 5.52KHz */
    #endif
#endif
    /* Wobble page*/
  //  {rwDFCTIME,         0x66,       BYTESET}, //TBD:SE1  /* 0x1026 ADIP wobble defect time*/

    {SVOREGTBL_END,              0x0000,       BYTESET}  /* end code */
}; /* svoRegInit2XTbl[] */

svoRegInitTblT const bdsvoRegInit3LXTbl[] =
{
    /* SERVO*/
#if(BD2p2XCLV == 1)
    {HAL_SRV_NDIV_ADR,           0x0003,       WORDSET}, /* 0xB00001F0[9:0] CLVN = 3 */
    {HAL_SRV_DECCMP_ADR,         0x0F0D,       WORDSET}, /* 0xB0000158[15:0] 33.8688e6/8790 = 3853 */
#else
    {HAL_SRV_NDIV_ADR,           0x0003,       WORDSET}, /* 0xB00001F0[9:0] CLVN = 3 */
    {HAL_SRV_DECCMP_ADR,         0x0F0D,       WORDSET}, /* 0xB0000158[15:0] 33.8688e6/8790 = 3853 */
#endif
    {HAL_SRV_DBT_ADR,            0x0000,       BYTESET}, /* 0xB0000334[6:0] Defect Delay Time */
#if (CHIP_REV <= 0xB3)
    {HAL_SRV_FVD_ADR,            0x0018,       BYTESET}, /* 0xB0000398[7:0] KWIDSL = 0us,FVD = 5.52KHz */
#endif
    /* Wobble page*/
  //  {rwDFCTIME,         0x33,       BYTESET}, //TBD:SE1  /* 0x1026 ADIP wobble defect time*/

    {SVOREGTBL_END,              0x0000,       BYTESET}  /* end code */
}; /* svoRegInit3XTbl[] */

svoRegInitTblT const bdsvoRegInit4LXTbl[] =
{
    /* SERVO*/
    {HAL_SRV_NDIV_ADR,           0x0004,       WORDSET}, /* 0xB00001F0[9:0] CLVN = 4 */
    {HAL_SRV_DECCMP_ADR,         0x0F0D,       WORDSET}, /* 0xB0000158[15:0] 33.8688e6/8790 = 3853 */
    {HAL_SRV_DBT_ADR,            0x0000,       BYTESET}, /* 0xB0000334[6:0] Defect Delay Time */
#if (CHIP_REV <= 0xB3)
    {HAL_SRV_FVD_ADR,            0x0018,       BYTESET}, /* 0xB0000398[7:0] KWIDSL = 0us,FVD = 5.52KHz */
#endif
    /* Wobble page*/
  //  {rwDFCTIME,         0x33,       BYTESET}, //TBD:SE1  /* 0x1026 ADIP wobble defect time*/

    {SVOREGTBL_END,              0x0000,       BYTESET}  /* end code */
}; /* svoRegInit4XTbl[] */

svoRegInitTblT const bdsvoRegInit25HzTbl[] =
{
    /* SERVO*/
    {HAL_SRV_NDIV_ADR,           0x0004,       WORDSET}, /* 0xB00001F0[9:0] CLVN = 4 */
    {HAL_SRV_DBT_ADR,            0x0000,       BYTESET}, /* 0xB0000334[6:0] Defect Delay Time */
#if (CHIP_REV <= 0xB3)
    {HAL_SRV_FVD_ADR,            0x0018,       BYTESET}, /* 0xB0000398[7:0] KWIDSL = 0us,FVD = 5.52KHz */
#endif
    {SVOREGTBL_END,              0x0000,       BYTESET}  /* end code */
}; /* bdsvoRegInit25HzTbl[] */

svoRegInitTblT const bdsvoRegInit40HzTbl[] =
{
    /* SERVO*/
    {HAL_SRV_NDIV_ADR,           0x0004,       WORDSET}, /* 0xB00001F0[9:0] CLVN = 4 */
    {HAL_SRV_DBT_ADR,            0x0000,       BYTESET}, /* 0xB0000334[6:0] Defect Delay Time */
#if (CHIP_REV <= 0xB3)
    {HAL_SRV_FVD_ADR,            0x0018,       BYTESET}, /* 0xB0000398[7:0] KWIDSL = 0us,FVD = 5.52KHz */
#endif
    {SVOREGTBL_END,              0x0000,       BYTESET}  /* end code */
}; /* bdsvoRegInit40HzTbl[] */

svoRegInitTblT const bdsvoRegInit60HzTbl[] =
{
    /* SERVO*/
    {HAL_SRV_NDIV_ADR,           0x0004,       WORDSET}, /* 0xB00001F0[9:0] CLVN = 4 */
    {HAL_SRV_DBT_ADR,            0x0000,       BYTESET}, /* 0xB0000334[6:0] Defect Delay Time */
#if (CHIP_REV <= 0xB3)
    {HAL_SRV_FVD_ADR,            0x0018,       BYTESET}, /* 0xB0000398[7:0] KWIDSL = 0us,FVD = 5.52KHz */
#endif
    {SVOREGTBL_END,              0x0000,       BYTESET}  /* end code */
}; /* bdsvoRegInit60HzTbl[] */

svoRegInitTblT const bdsvoRegInit80HzTbl[] =
{
    /* SERVO*/
    {HAL_SRV_NDIV_ADR,           0x0004,       WORDSET}, /* 0xB00001F0[9:0] CLVN = 4 */
    {HAL_SRV_DBT_ADR,            0x0000,       BYTESET}, /* 0xB0000334[6:0] Defect Delay Time */
#if (CHIP_REV <= 0xB3)
    {HAL_SRV_FVD_ADR,            0x0018,       BYTESET}, /* 0xB0000398[7:0] KWIDSL = 0us,FVD = 5.52KHz */
#endif
    {SVOREGTBL_END,              0x0000,       BYTESET}  /* end code */
}; /* bdsvoRegInit80HzTbl[] */
svoRegInitTblT const bdsvoRegInit4AXTbl[] =
{
    /* SERVO*/
    {HAL_SRV_NDIV_ADR,           0x0004,       WORDSET}, /* 0xB00001F0[9:0] CLVN = 4 */
    {HAL_SRV_DBT_ADR,            0x0000,       BYTESET}, /* 0xB0000334[6:0] Defect Delay Time */
#if (CHIP_REV <= 0xB3)
    {HAL_SRV_FVD_ADR,            0x0028,       BYTESET}, /* 0xB0000398[7:0] KWIDSL = 0us,FVD = 8.40KHz */
#endif
    {SVOREGTBL_END,              0x0000,       BYTESET}  /* end code */
}; /* bdsvoRegInit4AXTbl[] */
svoRegInitTblT const bdsvoRegInit6AXTbl[] =
{
    /* SERVO*/
    {HAL_SRV_NDIV_ADR,           0x0004,       WORDSET}, /* 0xB00001F0[9:0] CLVN = 4 */
    {HAL_SRV_DBT_ADR,            0x0000,       BYTESET}, /* 0xB0000334[6:0] Defect Delay Time */
#if (CHIP_REV <= 0xB3)
    {HAL_SRV_FVD_ADR,            0x0028,       BYTESET}, /* 0xB0000398[7:0] KWIDSL = 0us,FVD = 8.40KHz */
#endif
    {SVOREGTBL_END,              0x0000,       BYTESET}  /* end code */
}; /* bdsvoRegInit6AXTbl[] */
svoRegInitTblT const bdsvoRegInit8AXTbl[] =
{
    /* SERVO*/
    {HAL_SRV_NDIV_ADR,           0x0004,       WORDSET}, /* 0xB00001F0[9:0] CLVN = 4 */
    {HAL_SRV_DBT_ADR,            0x0000,       BYTESET}, /* 0xB0000334[6:0] Defect Delay Time */
#if (CHIP_REV <= 0xB3)
    {HAL_SRV_FVD_ADR,            0x0028,       BYTESET}, /* 0xB0000398[7:0] KWIDSL = 0us,FVD = 8.40KHz */
#endif
    {SVOREGTBL_END,              0x0000,       BYTESET}  /* end code */
}; /* bdsvoRegInit8AXTbl[] */
svoRegInitTblT const bdsvoRegInit10AXTbl[] =
{
    /* SERVO*/
    {HAL_SRV_NDIV_ADR,           0x0004,       WORDSET}, /* 0xB00001F0[9:0] CLVN = 4 */
    {HAL_SRV_DBT_ADR,            0x0000,       BYTESET}, /* 0xB0000334[6:0] Defect Delay Time */
#if (CHIP_REV <= 0xB3)
    {HAL_SRV_FVD_ADR,            0x0028,       BYTESET}, /* 0xB0000398[7:0] KWIDSL = 0us,FVD = 8.40KHz */
#endif
    {SVOREGTBL_END,              0x0000,       BYTESET}  /* end code */
}; /* bdsvoRegInit10AXTbl[] */
svoRegInitTblT const bdsvoRegInit12AXTbl[] =
{
    /* SERVO*/
    {HAL_SRV_NDIV_ADR,           0x0004,       WORDSET}, /* 0xB00001F0[9:0] CLVN = 4 */
    {HAL_SRV_DBT_ADR,            0x0000,       BYTESET}, /* 0xB0000334[6:0] Defect Delay Time */
#if (CHIP_REV <= 0xB3)
    {HAL_SRV_FVD_ADR,            0x0028,       BYTESET}, /* 0xB0000398[7:0] KWIDSL = 0us,FVD = 8.40KHz */
#endif
    {SVOREGTBL_END,              0x0000,       BYTESET}  /* end code */
}; /* bdsvoRegInit12AXTbl[] */
svoRegInitTblT const bdsvoRegInit14AXTbl[] =
{
    /* SERVO*/
    {HAL_SRV_NDIV_ADR,           0x0004,       WORDSET}, /* 0xB00001F0[9:0] CLVN = 4 */
    {HAL_SRV_DBT_ADR,            0x0000,       BYTESET}, /* 0xB0000334[6:0] Defect Delay Time */
#if (CHIP_REV <= 0xB3)
    {HAL_SRV_FVD_ADR,            0x0028,       BYTESET}, /* 0xB0000398[7:0] KWIDSL = 0us,FVD = 8.40KHz */
#endif
    {SVOREGTBL_END,              0x0000,       BYTESET}  /* end code */
}; /* bdsvoRegInit14AXTbl[] */

const svoRegInitTblT *const bdsvoRegInitTbl[MAX_SPEED_BD+1] = {
    (svoRegInitTblT *const) bdsvoRegInitNONETbl  ,   // NONE
    (svoRegInitTblT *const) bdsvoRegInit1LXTbl   ,   // _1Lx
    (svoRegInitTblT *const) bdsvoRegInit2LXTbl   ,   // _2Lx
    (svoRegInitTblT *const) bdsvoRegInit4AXTbl   ,   // _4AXCAV
    (svoRegInitTblT *const) bdsvoRegInit6AXTbl   ,   // _6AXCAV
    (svoRegInitTblT *const) bdsvoRegInit8AXTbl   ,   // _8AXCAV
    (svoRegInitTblT *const) bdsvoRegInit10AXTbl  ,   // _10AXCAV
    (svoRegInitTblT *const) bdsvoRegInit12AXTbl  ,   // _12AXCAV
    (svoRegInitTblT *const) bdsvoRegInit14AXTbl  ,   // _14AXCAV
};

/*----------------------------------------------------------------------------*/
/* Fine search KICKLVL coefficient                                            */
/*----------------------------------------------------------------------------*/
SEEK_GAIN const bdsvoKICKLVL_UL_F[] = {bdKICKLVL_EQ_UL_F};
SEEK_GAIN const bdsvoKICKLVL_L_F[]  = {bdKICKLVL_EQ_L_F};
SEEK_GAIN const bdsvoKICKLVL_M_F[]  = {bdKICKLVL_EQ_M_F};
SEEK_GAIN const bdsvoKICKLVL_H_F[]  = {bdKICKLVL_EQ_H_F};
SEEK_GAIN const bdsvoKICKLVL_UH_F[] = {bdKICKLVL_EQ_UH_F};

const SEEK_GAIN *const bdsvoKICKLVLTbl[MAX_SPEED_BD]={
    (SEEK_GAIN *const) bdsvoKICKLVL_UL_F,   // _1Lx
    (SEEK_GAIN *const) bdsvoKICKLVL_L_F,    // _2Lx
#if(BD2p2XCLV == 1)
    (SEEK_GAIN *const) bdsvoKICKLVL_L_F,    // _2p2Lx
#endif
    (SEEK_GAIN *const) bdsvoKICKLVL_M_F,    // _4AXCAV
    (SEEK_GAIN *const) bdsvoKICKLVL_H_F,    // _6AXCAV
    (SEEK_GAIN *const) bdsvoKICKLVL_H_F,    // _8AXCAV
    (SEEK_GAIN *const) bdsvoKICKLVL_H_F,    // _10AXCAV
    (SEEK_GAIN *const) bdsvoKICKLVL_H_F,    // _12AXCAV
    (SEEK_GAIN *const) bdsvoKICKLVL_UH_F,   // _14AXCAV
};

/*----------------------------------------------------------------------------*/
/* Fine search BRAKELVL coefficient                                           */
/*----------------------------------------------------------------------------*/
SEEK_GAIN const bdsvoBRAKELVL_UL_F[] = {bdBRAKELVL_EQ_UL_F};
SEEK_GAIN const bdsvoBRAKELVL_L_F[]  = {bdBRAKELVL_EQ_L_F};
SEEK_GAIN const bdsvoBRAKELVL_M_F[]  = {bdBRAKELVL_EQ_M_F};
SEEK_GAIN const bdsvoBRAKELVL_H_F[]  = {bdBRAKELVL_EQ_H_F};
SEEK_GAIN const bdsvoBRAKELVL_UH_F[] = {bdBRAKELVL_EQ_UH_F};

const SEEK_GAIN *const bdsvoBRAKELVLTbl[MAX_SPEED_BD]={
    (SEEK_GAIN *const) bdsvoBRAKELVL_UL_F,   // _1Lx
    (SEEK_GAIN *const) bdsvoBRAKELVL_L_F,    // _2Lx
#if(BD2p2XCLV == 1)
    (SEEK_GAIN *const) bdsvoBRAKELVL_L_F,    // _2p2Lx
#endif
    (SEEK_GAIN *const) bdsvoBRAKELVL_M_F,    // _4AXCAV
    (SEEK_GAIN *const) bdsvoBRAKELVL_H_F,    // _6AXCAV
    (SEEK_GAIN *const) bdsvoBRAKELVL_H_F,    // _8AXCAV
    (SEEK_GAIN *const) bdsvoBRAKELVL_H_F,    // _10AXCAV
    (SEEK_GAIN *const) bdsvoBRAKELVL_H_F,    // _12AXCAV
    (SEEK_GAIN *const) bdsvoBRAKELVL_UH_F,   // _14AXCAV
};

/*----------------------------------------------------------------------------*/
/* Fine search BRAKETIME coefficient                                          */
/*----------------------------------------------------------------------------*/
const BYTE bdsvoBRAKETIMETbl[MAX_SPEED_BD]={
    bdBRAKETIME_EQ_UL   ,   // _1Lx
    bdBRAKETIME_EQ_L    ,   // _2Lx
#if(BD2p2XCLV == 1)
    bdBRAKETIME_EQ_L    ,   // _2p2Lx
#endif
    bdBRAKETIME_EQ_M    ,   // _4AXCAV
    bdBRAKETIME_EQ_H    ,   // _6AXCAV
    bdBRAKETIME_EQ_H    ,   // _8AXCAV
    bdBRAKETIME_EQ_H    ,   // _10AXCAV
    bdBRAKETIME_EQ_H    ,   // _12AXCAV
    bdBRAKETIME_EQ_UH   ,   // _14AXCAV
};

/*----------------------------------------------------------------------------*/
/* Fine search BRAKESPD coefficient                                           */
/*----------------------------------------------------------------------------*/
const BYTE bdsvoBRAKESPDTbl[MAX_SPEED_BD]={
    bdBRAKESPD_EQ_UL   ,   // _1Lx
    bdBRAKESPD_EQ_L    ,   // _2Lx
#if(BD2p2XCLV == 1)
    bdBRAKESPD_EQ_L    ,   // _2p2Lx
#endif
    bdBRAKESPD_EQ_M    ,   // _4AXCAV
    bdBRAKESPD_EQ_H    ,   // _6AXCAV
    bdBRAKESPD_EQ_H    ,   // _8AXCAV
    bdBRAKESPD_EQ_H    ,   // _10AXCAV
    bdBRAKESPD_EQ_H    ,   // _12AXCAV
    bdBRAKESPD_EQ_UH   ,   // _14AXCAV
};

/*----------------------------------------------------------------------------*/
/* Fine search kt1B(0x0B) coefficient                                         */
/*----------------------------------------------------------------------------*/
SEEK_GAIN const bdsvokt1BKick_UL_F[] = {bdKt1BKick_EQ_UL_F};
SEEK_GAIN const bdsvokt1BKick_L_F[]  = {bdKt1BKick_EQ_L_F};
SEEK_GAIN const bdsvokt1BKick_M_F[]  = {bdKt1BKick_EQ_M_F};
SEEK_GAIN const bdsvokt1BKick_H_F[]  = {bdKt1BKick_EQ_H_F};
SEEK_GAIN const bdsvokt1BKick_UH_F[] = {bdKt1BKick_EQ_UH_F};

const SEEK_GAIN *const bdsvokt1BKickTbl[MAX_SPEED_BD]={
    (SEEK_GAIN *const) bdsvokt1BKick_UL_F,   // _1Lx
    (SEEK_GAIN *const) bdsvokt1BKick_L_F,    // _2Lx
#if(BD2p2XCLV == 1)
    (SEEK_GAIN *const) bdsvokt1BKick_L_F,    // _2p2Lx
#endif
    (SEEK_GAIN *const) bdsvokt1BKick_M_F,    // _4AXCAV
    (SEEK_GAIN *const) bdsvokt1BKick_H_F,    // _6AXCAV
    (SEEK_GAIN *const) bdsvokt1BKick_H_F,    // _8AXCAV
    (SEEK_GAIN *const) bdsvokt1BKick_H_F,    // _10AXCAV
    (SEEK_GAIN *const) bdsvokt1BKick_H_F,    // _12AXCAV
    (SEEK_GAIN *const) bdsvokt1BKick_UH_F,   // _14AXCAV
};

#if (ACTIVE_SLED_FOR_FINE_JUMP == 0)
/*----------------------------------------------------------------------------*/
/* Fine search ks17(0x3E) coefficient                                         */
/*----------------------------------------------------------------------------*/
SEEK_GAIN const bdsvoks17Kick_UL_F_000[]  = {bdsvoks17Kick_UL_F};
SEEK_GAIN const bdsvoks17Kick_UL_B_000[]  = {bdsvoks17Kick_UL_B};
SEEK_GAIN const bdsvoks17Kick_L_F_000[]   = {bdsvoks17Kick_L_F};
SEEK_GAIN const bdsvoks17Kick_L_B_000[]   = {bdsvoks17Kick_L_B};
SEEK_GAIN const bdsvoks17Kick_M_F_000[]   = {bdsvoks17Kick_M_F};
SEEK_GAIN const bdsvoks17Kick_M_B_000[]   = {bdsvoks17Kick_M_B};
SEEK_GAIN const bdsvoks17Kick_H_F_000[]   = {bdsvoks17Kick_H_F};
SEEK_GAIN const bdsvoks17Kick_H_B_000[]   = {bdsvoks17Kick_H_B};
SEEK_GAIN const bdsvoks17Kick_UH_F_000[]  = {bdsvoks17Kick_UH_F};
SEEK_GAIN const bdsvoks17Kick_UH_B_000[]  = {bdsvoks17Kick_UH_B};

const SEEK_GAIN *const bdsvoks17KickTbl[2][MAX_SPEED_BD]={
    {(SEEK_GAIN *const) bdsvoks17Kick_UL_F_000,   // _1Lx         FORWARD
     (SEEK_GAIN *const) bdsvoks17Kick_L_F_000,    // _2Lx
#if(BD2p2XCLV == 1)
     (SEEK_GAIN *const) bdsvoks17Kick_L_F_000,    // _2p2Lx
#endif
     (SEEK_GAIN *const) bdsvoks17Kick_M_F_000,    // _4AXCAV
     (SEEK_GAIN *const) bdsvoks17Kick_H_F_000,    // _6AXCAV
     (SEEK_GAIN *const) bdsvoks17Kick_H_F_000,    // _8AXCAV
     (SEEK_GAIN *const) bdsvoks17Kick_H_F_000,    // _10AXCAV
     (SEEK_GAIN *const) bdsvoks17Kick_H_F_000,    // _12AXCAV
     (SEEK_GAIN *const) bdsvoks17Kick_UH_F_000,   // _14AXCAV
    },
    {(SEEK_GAIN *const) bdsvoks17Kick_UL_B_000,   // _1Lx         FORWARD
     (SEEK_GAIN *const) bdsvoks17Kick_L_B_000,    // _2Lx
#if(BD2p2XCLV == 1)
    (SEEK_GAIN *const)  bdsvoks17Kick_L_B_000,    // _2p2Lx
#endif
     (SEEK_GAIN *const) bdsvoks17Kick_M_B_000,    // _4AXCAV
     (SEEK_GAIN *const) bdsvoks17Kick_H_B_000,    // _6AXCAV
     (SEEK_GAIN *const) bdsvoks17Kick_H_B_000,    // _8AXCAV
     (SEEK_GAIN *const) bdsvoks17Kick_H_B_000,    // _10AXCAV
     (SEEK_GAIN *const) bdsvoks17Kick_H_B_000,    // _12AXCAV
     (SEEK_GAIN *const) bdsvoks17Kick_UH_B_000,   // _14AXCAV
    }
};
#endif
/*----------------------------------------------------------------------------*/
/* Fine search KICKLVL coefficient                                            */
/*----------------------------------------------------------------------------*/
SEEK_GAIN const bdrsvoKICKLVL_UL_F[] = {bdrKICKLVL_EQ_UL_F};
SEEK_GAIN const bdrsvoKICKLVL_L_F[]  = {bdrKICKLVL_EQ_L_F};
SEEK_GAIN const bdrsvoKICKLVL_M_F[]  = {bdrKICKLVL_EQ_M_F};
SEEK_GAIN const bdrsvoKICKLVL_H_F[]  = {bdrKICKLVL_EQ_H_F};
SEEK_GAIN const bdrsvoKICKLVL_UH_F[] = {bdrKICKLVL_EQ_UH_F};

const SEEK_GAIN *const bdrsvoKICKLVLTbl[MAX_SPEED_BD]={
    (SEEK_GAIN *const) bdrsvoKICKLVL_UL_F,   // _1Lx
    (SEEK_GAIN *const) bdrsvoKICKLVL_L_F,    // _2Lx
#if(BD2p2XCLV == 1)
    (SEEK_GAIN *const) bdrsvoKICKLVL_L_F,    // _2p2Lx
#endif
    (SEEK_GAIN *const) bdrsvoKICKLVL_M_F,    // _4AXCAV
    (SEEK_GAIN *const) bdrsvoKICKLVL_H_F,    // _6AXCAV
    (SEEK_GAIN *const) bdrsvoKICKLVL_H_F,    // _8AXCAV
    (SEEK_GAIN *const) bdrsvoKICKLVL_H_F,    // _10AXCAV
    (SEEK_GAIN *const) bdrsvoKICKLVL_H_F,    // _12AXCAV
    (SEEK_GAIN *const) bdrsvoKICKLVL_UH_F,   // _14AXCAV
};

/*----------------------------------------------------------------------------*/
/* Fine search BRAKELVL coefficient                                           */
/*----------------------------------------------------------------------------*/
SEEK_GAIN const bdrsvoBRAKELVL_UL_F[] = {bdrBRAKELVL_EQ_UL_F};
SEEK_GAIN const bdrsvoBRAKELVL_L_F[]  = {bdrBRAKELVL_EQ_L_F};
SEEK_GAIN const bdrsvoBRAKELVL_M_F[]  = {bdrBRAKELVL_EQ_M_F};
SEEK_GAIN const bdrsvoBRAKELVL_H_F[]  = {bdrBRAKELVL_EQ_H_F};
SEEK_GAIN const bdrsvoBRAKELVL_UH_F[] = {bdrBRAKELVL_EQ_UH_F};

const SEEK_GAIN *const bdrsvoBRAKELVLTbl[MAX_SPEED_BD]={
    (SEEK_GAIN *const) bdrsvoBRAKELVL_UL_F,   // _1Lx
    (SEEK_GAIN *const) bdrsvoBRAKELVL_L_F,    // _2Lx
#if(BD2p2XCLV == 1)
    (SEEK_GAIN *const) bdrsvoBRAKELVL_L_F,    // _2p2Lx
#endif
    (SEEK_GAIN *const) bdrsvoBRAKELVL_M_F,    // _4AXCAV
    (SEEK_GAIN *const) bdrsvoBRAKELVL_H_F,    // _6AXCAV
    (SEEK_GAIN *const) bdrsvoBRAKELVL_H_F,    // _8AXCAV
    (SEEK_GAIN *const) bdrsvoBRAKELVL_H_F,    // _10AXCAV
    (SEEK_GAIN *const) bdrsvoBRAKELVL_H_F,    // _12AXCAV
    (SEEK_GAIN *const) bdrsvoBRAKELVL_UH_F,   // _14AXCAV
};

/*----------------------------------------------------------------------------*/
/* Fine search BRAKETIME coefficient                                          */
/*----------------------------------------------------------------------------*/
const BYTE bdrsvoBRAKETIMETbl[MAX_SPEED_BD]={
    bdrBRAKETIME_EQ_UL   ,   // _1Lx
    bdrBRAKETIME_EQ_L    ,   // _2Lx
#if(BD2p2XCLV == 1)
    bdrBRAKETIME_EQ_L    ,   // _2p2Lx
#endif
    bdrBRAKETIME_EQ_M    ,   // _4AXCAV
    bdrBRAKETIME_EQ_H    ,   // _6AXCAV
    bdrBRAKETIME_EQ_H    ,   // _8AXCAV
    bdrBRAKETIME_EQ_H    ,   // _10AXCAV
    bdrBRAKETIME_EQ_H    ,   // _12AXCAV
    bdrBRAKETIME_EQ_UH   ,   // _14AXCAV
};

/*----------------------------------------------------------------------------*/
/* Fine search BRAKESPD coefficient                                           */
/*----------------------------------------------------------------------------*/
const BYTE bdrsvoBRAKESPDTbl[MAX_SPEED_BD]={
    bdrBRAKESPD_EQ_UL   ,   // _1Lx
    bdrBRAKESPD_EQ_L    ,   // _2Lx
#if(BD2p2XCLV == 1)
    bdrBRAKESPD_EQ_L   ,    // _2p2Lx
#endif
    bdrBRAKESPD_EQ_M    ,   // _4AXCAV
    bdrBRAKESPD_EQ_H    ,   // _6AXCAV
    bdrBRAKESPD_EQ_H    ,   // _8AXCAV
    bdrBRAKESPD_EQ_H    ,   // _10AXCAV
    bdrBRAKESPD_EQ_H    ,   // _12AXCAV
    bdrBRAKESPD_EQ_UH   ,   // _14AXCAV
};

/*----------------------------------------------------------------------------*/
/* Fine search kt1B(0x0B) coefficient                                         */
/*----------------------------------------------------------------------------*/
SEEK_GAIN const bdrsvokt1BKick_UL_F[] = {bdrKt1BKick_EQ_UL_F};
SEEK_GAIN const bdrsvokt1BKick_L_F[]  = {bdrKt1BKick_EQ_L_F};
SEEK_GAIN const bdrsvokt1BKick_M_F[]  = {bdrKt1BKick_EQ_M_F};
SEEK_GAIN const bdrsvokt1BKick_H_F[]  = {bdrKt1BKick_EQ_H_F};
SEEK_GAIN const bdrsvokt1BKick_UH_F[] = {bdrKt1BKick_EQ_UH_F};

const SEEK_GAIN *const bdrsvokt1BKickTbl[MAX_SPEED_BD]={
    (SEEK_GAIN *const) bdrsvokt1BKick_UL_F,   // _1Lx
    (SEEK_GAIN *const) bdrsvokt1BKick_L_F,    // _2Lx
#if(BD2p2XCLV == 1)
    (SEEK_GAIN *const) bdrsvokt1BKick_L_F,    // _2p2Lx
#endif
    (SEEK_GAIN *const) bdrsvokt1BKick_M_F,    // _4AXCAV
    (SEEK_GAIN *const) bdrsvokt1BKick_H_F,    // _6AXCAV
    (SEEK_GAIN *const) bdrsvokt1BKick_H_F,    // _8AXCAV
    (SEEK_GAIN *const) bdrsvokt1BKick_H_F,    // _10AXCAV
    (SEEK_GAIN *const) bdrsvokt1BKick_H_F,    // _12AXCAV
    (SEEK_GAIN *const) bdrsvokt1BKick_UH_F,   // _14AXCAV
};
#if (ACTIVE_SLED_FOR_FINE_JUMP == 0)
/*----------------------------------------------------------------------------*/
/* Fine search ks17(0x3E) coefficient                                         */
/*----------------------------------------------------------------------------*/
SEEK_GAIN const bdrsvoks17Kick_UL_F_000[]  = {bdrsvoks17Kick_UL_F};
SEEK_GAIN const bdrsvoks17Kick_UL_B_000[]  = {bdrsvoks17Kick_UL_B};
SEEK_GAIN const bdrsvoks17Kick_L_F_000[]   = {bdrsvoks17Kick_L_F};
SEEK_GAIN const bdrsvoks17Kick_L_B_000[]   = {bdrsvoks17Kick_L_B};
SEEK_GAIN const bdrsvoks17Kick_M_F_000[]   = {bdrsvoks17Kick_M_F};
SEEK_GAIN const bdrsvoks17Kick_M_B_000[]   = {bdrsvoks17Kick_M_B};
SEEK_GAIN const bdrsvoks17Kick_H_F_000[]   = {bdrsvoks17Kick_H_F};
SEEK_GAIN const bdrsvoks17Kick_H_B_000[]   = {bdrsvoks17Kick_H_B};
SEEK_GAIN const bdrsvoks17Kick_UH_F_000[]  = {bdrsvoks17Kick_UH_F};
SEEK_GAIN const bdrsvoks17Kick_UH_B_000[]  = {bdrsvoks17Kick_UH_B};

const SEEK_GAIN *const bdrsvoks17KickTbl[2][MAX_SPEED_BD]={
    {(SEEK_GAIN *const) bdrsvoks17Kick_UL_F_000,   // _1Lx         FORWARD
     (SEEK_GAIN *const) bdrsvoks17Kick_L_F_000,    // _2Lx
#if(BD2p2XCLV == 1)
     (SEEK_GAIN *const) bdrsvoks17Kick_L_F_000,    // _2p2Lx
#endif
     (SEEK_GAIN *const) bdrsvoks17Kick_M_F_000,    // _4AXCAV
     (SEEK_GAIN *const) bdrsvoks17Kick_H_F_000,    // _6AXCAV
     (SEEK_GAIN *const) bdrsvoks17Kick_H_F_000,    // _8AXCAV
     (SEEK_GAIN *const) bdrsvoks17Kick_H_F_000,    // _10AXCAV
     (SEEK_GAIN *const) bdrsvoks17Kick_H_F_000,    // _12AXCAV
     (SEEK_GAIN *const) bdrsvoks17Kick_UH_F_000,   // _14AXCAV
    },
    {(SEEK_GAIN *const) bdrsvoks17Kick_UL_B_000,   // _1Lx         BACKWARD
     (SEEK_GAIN *const) bdrsvoks17Kick_L_B_000,    // _2Lx
#if(BD2p2XCLV == 1)
     (SEEK_GAIN *const) bdrsvoks17Kick_L_B_000,    // _2p2Lx
#endif
     (SEEK_GAIN *const) bdrsvoks17Kick_M_B_000,    // _4AXCAV
     (SEEK_GAIN *const) bdrsvoks17Kick_H_B_000,    // _6AXCAV
     (SEEK_GAIN *const) bdrsvoks17Kick_H_B_000,    // _8AXCAV
     (SEEK_GAIN *const) bdrsvoks17Kick_H_B_000,    // _10AXCAV
     (SEEK_GAIN *const) bdrsvoks17Kick_H_B_000,    // _12AXCAV
     (SEEK_GAIN *const) bdrsvoks17Kick_UH_B_000,   // _14AXCAV
    }
};
#endif

//**************************************************************************************************
// FOR BD-ROM DISC
//**************************************************************************************************
const svoWordU bdTracking_Shock_Eq_Init[] = {
    0x0000,                       /* kt30  (0x1A) */
    0x0000,                       /* kt31  (0x1B) */
    0x0000,                       /* kt32  (0x1C) */
    0x0000,                       /* kt33  (0x1D) */
    0x0000,                       /* kt34  (0x1E) */
    0x0000                        /* kt35  (0x1F) */
};

const svoWordU bdFocus_High_Eq_Init[] = {
    0x7FFF,                       /* kf30  (0x2C) */
    0x8000,                       /* kf31  (0x2D) */
    0x7FE8                        /* kf32  (0x2E) */
};

const svoWordU bdBeam_Low_Eq_Init[] = {
    0x1780,                       /* kb10  (0x45) */
    0x7FE9,                       /* kb11  (0x46) */
    0x0080                        /* kb12  (0x47) */
};

const svoWordU bdBeam_High_Eq_Init[] = {
    0x0000,                       /* Kb20  (0x48) */
    0x0000,                       /* Kb21  (0x49) */
    0x0000                        /* Kb22  (0x4A) */
};

const svoWordU bdAdjust_Eq_Init[] = {
    FOCUS_SEARCH_KF18,            /* Kf18  (0x28) */
    0x7FFF,                       /* Kfb20 (0x55) */
    0x1000,                       /* Kfb21 (0x56) */
    0x0800,                       /* Krb10 (0x65) */
    0x0000                        /* Ks17  (0x3E) */
};

// UL-Group Coefficients (1XCLV, 25Hz):
const svoWordU bdTracking_Eq_UL[]      = {bdTRACKING_EQ_UL     };
const svoWordU bdTracking_Low_Eq_UL[]  = {bdTRACKING_LOW_EQ_UL };
const svoWordU bdTracking_DC_Eq_UL[]   = {bdTRK_DC_EQ_UL       };
const svoWordU bdFocus_Eq_UL[]         = {bdFOCUS_EQ_UL        };
const svoWordU bdFocus_Low_Eq_UL[]     = {bdFOCUS_LOW_EQ_UL    };
const svoWordU bdFocus_DC_Eq_UL[]      = {bdFOCUS_DC_EQ_UL     };
const svoWordU bdSpeed_Eq_UL[]         = {bdSPEED_EQ_UL        };
const svoWordU bdSled_Eq_UL[]          = {bdSLED_EQ_UL         };
const svoWordU bdDisk_Fg_Eq_UL[]       = {bdDISK_FG_EQ_UL      };

// L-Group Coefficients (2XCLV, 40Hz):
const svoWordU bdTracking_Eq_L[]       = {bdTRACKING_EQ_L      };
const svoWordU bdTracking_Low_Eq_L[]   = {bdTRACKING_LOW_EQ_L  };
const svoWordU bdTracking_DC_Eq_L[]    = {bdTRK_DC_EQ_L        };
const svoWordU bdFocus_Eq_L[]          = {bdFOCUS_EQ_L         };
const svoWordU bdFocus_Low_Eq_L[]      = {bdFOCUS_LOW_EQ_L     };
const svoWordU bdFocus_DC_Eq_L[]       = {bdFOCUS_DC_EQ_L      };
const svoWordU bdSpeed_Eq_L[]          = {bdSPEED_EQ_L         };
const svoWordU bdSled_Eq_L[]           = {bdSLED_EQ_L          };
const svoWordU bdDisk_Fg_Eq_L[]        = {bdDISK_FG_EQ_L       };

// M-Group Coefficients (3XCLV, 60Hz):
const svoWordU bdTracking_Eq_M[]       = {bdTRACKING_EQ_M      };
const svoWordU bdTracking_Low_Eq_M[]   = {bdTRACKING_LOW_EQ_M  };
const svoWordU bdTracking_DC_Eq_M[]    = {bdTRK_DC_EQ_M        };
const svoWordU bdFocus_Eq_M[]          = {bdFOCUS_EQ_M         };
const svoWordU bdFocus_Low_Eq_M[]      = {bdFOCUS_LOW_EQ_M     };
const svoWordU bdFocus_DC_Eq_M[]       = {bdFOCUS_DC_EQ_M      };
const svoWordU bdSpeed_Eq_M[]          = {bdSPEED_EQ_M         };
const svoWordU bdSled_Eq_M[]           = {bdSLED_EQ_M          };
const svoWordU bdDisk_Fg_Eq_M[]        = {bdDISK_FG_EQ_M       };

// H-Group Coefficients (4XCLV, 80Hz):
const svoWordU bdTracking_Eq_H[]       = {bdTRACKING_EQ_H      };
const svoWordU bdTracking_Low_Eq_H[]   = {bdTRACKING_LOW_EQ_H  };
const svoWordU bdTracking_DC_Eq_H[]    = {bdTRK_DC_EQ_H        };
const svoWordU bdFocus_Eq_H[]          = {bdFOCUS_EQ_H         };
const svoWordU bdFocus_Low_Eq_H[]      = {bdFOCUS_LOW_EQ_H     };
const svoWordU bdFocus_DC_Eq_H[]       = {bdFOCUS_DC_EQ_H      };
const svoWordU bdSpeed_Eq_H[]          = {bdSPEED_EQ_H         };
const svoWordU bdSled_Eq_H[]           = {bdSLED_EQ_H          };
const svoWordU bdDisk_Fg_Eq_H[]        = {bdDISK_FG_EQ_H       };

// UH-Group Coefficients (TBD):
const svoWordU bdTracking_Eq_UH[]      = {bdTRACKING_EQ_UH     };
const svoWordU bdTracking_Low_Eq_UH[]  = {bdTRACKING_LOW_EQ_UH };
const svoWordU bdTracking_DC_Eq_UH[]   = {bdTRK_DC_EQ_UH       };
const svoWordU bdFocus_Eq_UH[]         = {bdFOCUS_EQ_UH        };
const svoWordU bdFocus_Low_Eq_UH[]     = {bdFOCUS_LOW_EQ_UH    };
const svoWordU bdFocus_DC_Eq_UH[]      = {bdFOCUS_DC_EQ_UH     };
const svoWordU bdSpeed_Eq_UH[]         = {bdSPEED_EQ_UH        };
const svoWordU bdSled_Eq_UH[]          = {bdSLED_EQ_UH         };
const svoWordU bdDisk_Fg_Eq_UH[]       = {bdDISK_FG_EQ_UH      };
// Disc Servo Eq for Decoder Mode

const svoWordU *const bdSvoEQCoefTbl[MAX_SPEED_BD+1][EQ_TBL_MAX+1] =
{
/* NONE */
    {NULL,                                        /* TRACKING_EQ          */
     NULL,                                        /* TRACKING_LOW_EQ      */
     NULL,                                        /* TRACKING_DC_EQ       */
     (svoWordU *const) bdTracking_Shock_Eq_Init,  /* TRACKING_SHOCK_EQ    */
     NULL,                                        /* FOCUS_EQ             */
     NULL,                                        /* FOCUS_LOW_EQ         */
     NULL,                                        /* FOCUS_DC_EQ          */
     (svoWordU *const) bdFocus_High_Eq_Init,      /* FOCUS_HIGH_EQ        */
     NULL,                                        /* DISK_FG_EQ           */
     NULL,                                        /* SLED_EQ              */
     (svoWordU *const) bdBeam_Low_Eq_Init,        /* BEAM_LOW_EQ          */
     (svoWordU *const) bdBeam_High_Eq_Init,       /* BEAM_HIGH_EQ         */
     NULL,                                        /* SPEED_EQ             */
     (svoWordU *const) bdAdjust_Eq_Init,          /* ADJUST_EQ            */
    },

/* X1 CLV */
    {(svoWordU *const) bdTracking_Eq_UL,          /* TRACKING_EQ          */
     (svoWordU *const) bdTracking_Low_Eq_UL,      /* TRACKING_LOW_EQ      */
     (svoWordU *const) bdTracking_DC_Eq_UL,       /* TRACKING_DC_EQ       */
     NULL,                                        /* TRACKING_SHOCK_EQ    */
     (svoWordU *const) bdFocus_Eq_UL,             /* FOCUS_EQ             */
     (svoWordU *const) bdFocus_Low_Eq_UL,         /* FOCUS_LOW_EQ         */
     (svoWordU *const) bdFocus_DC_Eq_UL,          /* FOCUS_DC_EQ          */
     NULL,                                        /* FOCUS_HIGH_EQ        */
     (svoWordU *const) bdDisk_Fg_Eq_UL,           /* DISK_FG_EQ           */
     (svoWordU *const) bdSled_Eq_UL,              /* SLED_EQ              */
     NULL,                                        /* BEAM_LOW_EQ          */
     NULL,                                        /* BEAM_HIGH_EQ         */
     (svoWordU *const) bdSpeed_Eq_UL,             /* SPEED_EQ             */
     NULL,                                        /* ADJUST_EQ            */
    },

/* X2 CLV */
    {(svoWordU *const) bdTracking_Eq_L,           /* TRACKING_EQ          */
     (svoWordU *const) bdTracking_Low_Eq_L,       /* TRACKING_LOW_EQ      */
     (svoWordU *const) bdTracking_DC_Eq_L,        /* TRACKING_DC_EQ       */
     NULL,                                        /* TRACKING_SHOCK_EQ    */
     (svoWordU *const) bdFocus_Eq_L,              /* FOCUS_EQ             */
     (svoWordU *const) bdFocus_Low_Eq_L,          /* FOCUS_LOW_EQ         */
     (svoWordU *const) bdFocus_DC_Eq_L,           /* FOCUS_DC_EQ          */
     NULL,                                        /* FOCUS_HIGH_EQ        */
     (svoWordU *const) bdDisk_Fg_Eq_L,            /* DISK_FG_EQ           */
     (svoWordU *const) bdSled_Eq_L,               /* SLED_EQ              */
     NULL,                                        /* BEAM_LOW_EQ          */
     NULL,                                        /* BEAM_HIGH_EQ         */
     (svoWordU *const) bdSpeed_Eq_L,              /* SPEED_EQ             */
     NULL,                                        /* ADJUST_EQ            */
    },
/* X3 CLV */
#if(BD2p2XCLV == 1)
     {(svoWordU *const) bdTracking_Eq_L,           /* TRACKING_EQ          */
     (svoWordU *const) bdTracking_Low_Eq_L,       /* TRACKING_LOW_EQ      */
     (svoWordU *const) bdTracking_DC_Eq_L,        /* TRACKING_DC_EQ       */
     NULL,                                        /* TRACKING_SHOCK_EQ    */
     (svoWordU *const) bdFocus_Eq_L,              /* FOCUS_EQ             */
     (svoWordU *const) bdFocus_Low_Eq_L,          /* FOCUS_LOW_EQ         */
     (svoWordU *const) bdFocus_DC_Eq_L,           /* FOCUS_DC_EQ          */
     NULL,                                        /* FOCUS_HIGH_EQ        */
     (svoWordU *const) bdDisk_Fg_Eq_L,            /* DISK_FG_EQ           */
     (svoWordU *const) bdSled_Eq_L,               /* SLED_EQ              */
     NULL,                                        /* BEAM_LOW_EQ          */
     NULL,                                        /* BEAM_HIGH_EQ         */
     (svoWordU *const) bdSpeed_Eq_L,              /* SPEED_EQ             */
     NULL,                                        /* ADJUST_EQ            */
    },
#endif
/* X4 CLV */
//    {(svoWordU *const) bdTracking_Eq_H,           /* TRACKING_EQ          */
//    (svoWordU *const) bdTracking_Low_Eq_H,       /* TRACKING_LOW_EQ      */
//    (svoWordU *const) bdTracking_DC_Eq_H,        /* TRACKING_DC_EQ       */
//    NULL,                                        /* TRACKING_SHOCK_EQ    */
//     (svoWordU *const) bdFocus_Eq_H,              /* FOCUS_EQ             */
//     (svoWordU *const) bdFocus_Low_Eq_H,          /* FOCUS_LOW_EQ         */
//     (svoWordU *const) bdFocus_DC_Eq_H,           /* FOCUS_DC_EQ          */
//     NULL,                                        /* FOCUS_HIGH_EQ        */
//     (svoWordU *const) bdDisk_Fg_Eq_H,            /* DISK_FG_EQ           */
//     (svoWordU *const) bdSled_Eq_H,               /* SLED_EQ              */
//     NULL,                                        /* BEAM_LOW_EQ          */
//     NULL,                                        /* BEAM_HIGH_EQ         */
//     (svoWordU *const) bdSpeed_Eq_H,              /* SPEED_EQ             */
//     NULL,                                        /* ADJUST_EQ            */
//    },

/* 4XCAV */
    {(svoWordU *const) bdTracking_Eq_M,           /* TRACKING_EQ          */
     (svoWordU *const) bdTracking_Low_Eq_M,       /* TRACKING_LOW_EQ      */
     (svoWordU *const) bdTracking_DC_Eq_M,        /* TRACKING_DC_EQ       */
     NULL,                                        /* TRACKING_SHOCK_EQ    */
     (svoWordU *const) bdFocus_Eq_M,              /* FOCUS_EQ             */
     (svoWordU *const) bdFocus_Low_Eq_M,          /* FOCUS_LOW_EQ         */
     (svoWordU *const) bdFocus_DC_Eq_M,           /* FOCUS_DC_EQ          */
     NULL,                                        /* FOCUS_HIGH_EQ        */
     (svoWordU *const) bdDisk_Fg_Eq_M,            /* DISK_FG_EQ           */
     (svoWordU *const) bdSled_Eq_M,               /* SLED_EQ              */
     NULL,                                        /* BEAM_LOW_EQ          */
     NULL,                                        /* BEAM_HIGH_EQ         */
     (svoWordU *const) bdSpeed_Eq_M,              /* SPEED_EQ             */
     NULL,                                        /* ADJUST_EQ            */
    },


/* 6XCAV */
    {(svoWordU *const) bdTracking_Eq_H,           /* TRACKING_EQ          */
     (svoWordU *const) bdTracking_Low_Eq_H,       /* TRACKING_LOW_EQ      */
     (svoWordU *const) bdTracking_DC_Eq_H,        /* TRACKING_DC_EQ       */
     NULL,                                        /* TRACKING_SHOCK_EQ    */
     (svoWordU *const) bdFocus_Eq_H,              /* FOCUS_EQ             */
     (svoWordU *const) bdFocus_Low_Eq_H,          /* FOCUS_LOW_EQ         */
     (svoWordU *const) bdFocus_DC_Eq_H,           /* FOCUS_DC_EQ          */
     NULL,                                        /* FOCUS_HIGH_EQ        */
     (svoWordU *const) bdDisk_Fg_Eq_H,            /* DISK_FG_EQ           */
     (svoWordU *const) bdSled_Eq_H,               /* SLED_EQ              */
     NULL,                                        /* BEAM_LOW_EQ          */
     NULL,                                        /* BEAM_HIGH_EQ         */
     (svoWordU *const) bdSpeed_Eq_H,              /* SPEED_EQ             */
     NULL,                                        /* ADJUST_EQ            */
    },

/* 8XCAV */
    {(svoWordU *const) bdTracking_Eq_H,           /* TRACKING_EQ          */
     (svoWordU *const) bdTracking_Low_Eq_H,       /* TRACKING_LOW_EQ      */
     (svoWordU *const) bdTracking_DC_Eq_H,        /* TRACKING_DC_EQ       */
     NULL,                                        /* TRACKING_SHOCK_EQ    */
     (svoWordU *const) bdFocus_Eq_H,              /* FOCUS_EQ             */
     (svoWordU *const) bdFocus_Low_Eq_H,          /* FOCUS_LOW_EQ         */
     (svoWordU *const) bdFocus_DC_Eq_H,           /* FOCUS_DC_EQ          */
     NULL,                                        /* FOCUS_HIGH_EQ        */
     (svoWordU *const) bdDisk_Fg_Eq_H,            /* DISK_FG_EQ           */
     (svoWordU *const) bdSled_Eq_H,               /* SLED_EQ              */
     NULL,                                        /* BEAM_LOW_EQ          */
     NULL,                                        /* BEAM_HIGH_EQ         */
     (svoWordU *const) bdSpeed_Eq_H,              /* SPEED_EQ             */
     NULL,                                        /* ADJUST_EQ            */
    },


/* 10XCAV */
    {(svoWordU *const) bdTracking_Eq_H,           /* TRACKING_EQ          */
     (svoWordU *const) bdTracking_Low_Eq_H,       /* TRACKING_LOW_EQ      */
     (svoWordU *const) bdTracking_DC_Eq_H,        /* TRACKING_DC_EQ       */
     NULL,                                        /* TRACKING_SHOCK_EQ    */
     (svoWordU *const) bdFocus_Eq_H,              /* FOCUS_EQ             */
     (svoWordU *const) bdFocus_Low_Eq_H,          /* FOCUS_LOW_EQ         */
     (svoWordU *const) bdFocus_DC_Eq_H,           /* FOCUS_DC_EQ          */
     NULL,                                        /* FOCUS_HIGH_EQ        */
     (svoWordU *const) bdDisk_Fg_Eq_H,            /* DISK_FG_EQ           */
     (svoWordU *const) bdSled_Eq_H,               /* SLED_EQ              */
     NULL,                                        /* BEAM_LOW_EQ          */
     NULL,                                        /* BEAM_HIGH_EQ         */
     (svoWordU *const) bdSpeed_Eq_H,              /* SPEED_EQ             */
     NULL,                                        /* ADJUST_EQ            */
    },
/* 12XCAV */
    {(svoWordU *const) bdTracking_Eq_H,           /* TRACKING_EQ          */
     (svoWordU *const) bdTracking_Low_Eq_H,       /* TRACKING_LOW_EQ      */
     (svoWordU *const) bdTracking_DC_Eq_H,        /* TRACKING_DC_EQ       */
     NULL,                                        /* TRACKING_SHOCK_EQ    */
     (svoWordU *const) bdFocus_Eq_H,              /* FOCUS_EQ             */
     (svoWordU *const) bdFocus_Low_Eq_H,          /* FOCUS_LOW_EQ         */
     (svoWordU *const) bdFocus_DC_Eq_H,           /* FOCUS_DC_EQ          */
     NULL,                                        /* FOCUS_HIGH_EQ        */
     (svoWordU *const) bdDisk_Fg_Eq_H,            /* DISK_FG_EQ           */
     (svoWordU *const) bdSled_Eq_H,               /* SLED_EQ              */
     NULL,                                        /* BEAM_LOW_EQ          */
     NULL,                                        /* BEAM_HIGH_EQ         */
     (svoWordU *const) bdSpeed_Eq_H,              /* SPEED_EQ             */
     NULL,                                        /* ADJUST_EQ            */
    },
/* 14XCAV */
    {(svoWordU *const) bdTracking_Eq_UH,          /* TRACKING_EQ          */
     (svoWordU *const) bdTracking_Low_Eq_UH,      /* TRACKING_LOW_EQ      */
     (svoWordU *const) bdTracking_DC_Eq_UH,       /* TRACKING_DC_EQ       */
     NULL,                                        /* TRACKING_SHOCK_EQ    */
     (svoWordU *const) bdFocus_Eq_UH,             /* FOCUS_EQ             */
     (svoWordU *const) bdFocus_Low_Eq_UH,         /* FOCUS_LOW_EQ         */
     (svoWordU *const) bdFocus_DC_Eq_UH,          /* FOCUS_DC_EQ          */
     NULL,                                        /* FOCUS_HIGH_EQ        */
     (svoWordU *const) bdDisk_Fg_Eq_UH,           /* DISK_FG_EQ           */
     (svoWordU *const) bdSled_Eq_UH,              /* SLED_EQ              */
     NULL,                                        /* BEAM_LOW_EQ          */
     NULL,                                        /* BEAM_HIGH_EQ         */
     (svoWordU *const) bdSpeed_Eq_UH,             /* SPEED_EQ             */
     NULL,                                        /* ADJUST_EQ            */
    },

};
#if (FINE_SEEK_SURF_TELFG == 0)
const svoWordU bdFine_CE_Eq_UL[] = {bdFINE_CE_EQ_UL };
const svoWordU bdFine_CE_Eq_L[]  = {bdFINE_CE_EQ_L  };
const svoWordU bdFine_CE_Eq_M[]  = {bdFINE_CE_EQ_M  };
const svoWordU bdFine_CE_Eq_H[]  = {bdFINE_CE_EQ_H  };
const svoWordU bdFine_CE_Eq_UH[] = {bdFINE_CE_EQ_UH };

const svoWordU (*const bdFineCEEQCoefTbl[MAX_SPEED_BD]) =
{
    (svoWordU *const)bdFine_CE_Eq_UL,  //_1LX_BD
    (svoWordU *const)bdFine_CE_Eq_L,   //_2LX_BD
#if(BD2p2XCLV == 1)
    (svoWordU *const)bdFine_CE_Eq_L,   //_2p2LX_BD
#endif
    (svoWordU *const)bdFine_CE_Eq_M,   //_4AXCAV_BD
    (svoWordU *const)bdFine_CE_Eq_M,   //_6AXCAV_BD
    (svoWordU *const)bdFine_CE_Eq_H,   //_8AXCAV_BD
    (svoWordU *const)bdFine_CE_Eq_H,   //_10AXCAV_BD
    (svoWordU *const)bdFine_CE_Eq_H,   //_12AXCAV_BD
    (svoWordU *const)bdFine_CE_Eq_UH,  //_14AXCAV_BD
};
#endif
const svoWordU bdRoughCEEQ[]     = {bdROUGH_CE_EQ_R};
const svoWordU bdRoughCELFG[]    = {bdROUGH_CE_LOW_EQ_R};

//**************************************************************************************************
// FOR BD+R/-R/+RW/-RW DISC
//**************************************************************************************************
// UL-Group Coefficients (1XCLV, 25Hz):
const svoWordU bdrTracking_Eq_UL[]      = {bdrTRACKING_EQ_UL     };
const svoWordU bdrTracking_Low_Eq_UL[]  = {bdrTRACKING_LOW_EQ_UL };
const svoWordU bdrFocus_Eq_UL[]         = {bdrFOCUS_EQ_UL        };
const svoWordU bdrFocus_Low_Eq_UL[]     = {bdrFOCUS_LOW_EQ_UL    };
const svoWordU bdrSpeed_Eq_UL[]         = {bdrSPEED_EQ_UL        };

// L-Group Coefficients (2XCLV, 40Hz):
const svoWordU bdrTracking_Eq_L[]       = {bdrTRACKING_EQ_L      };
const svoWordU bdrTracking_Low_Eq_L[]   = {bdrTRACKING_LOW_EQ_L  };
const svoWordU bdrFocus_Eq_L[]          = {bdrFOCUS_EQ_L         };
const svoWordU bdrFocus_Low_Eq_L[]      = {bdrFOCUS_LOW_EQ_L     };
const svoWordU bdrSpeed_Eq_L[]          = {bdrSPEED_EQ_L         };

// M-Group Coefficients (3XCLV, 60Hz):
const svoWordU bdrTracking_Eq_M[]       = {bdrTRACKING_EQ_M      };
const svoWordU bdrTracking_Low_Eq_M[]   = {bdrTRACKING_LOW_EQ_M  };
const svoWordU bdrFocus_Eq_M[]          = {bdrFOCUS_EQ_M         };
const svoWordU bdrFocus_Low_Eq_M[]      = {bdrFOCUS_LOW_EQ_M     };
const svoWordU bdrSpeed_Eq_M[]          = {bdrSPEED_EQ_M         };

// H-Group Coefficients (4XCLV, 80Hz):
const svoWordU bdrTracking_Eq_H[]       = {bdrTRACKING_EQ_H      };
const svoWordU bdrTracking_Low_Eq_H[]   = {bdrTRACKING_LOW_EQ_H  };
const svoWordU bdrFocus_Eq_H[]          = {bdrFOCUS_EQ_H         };
const svoWordU bdrFocus_Low_Eq_H[]      = {bdrFOCUS_LOW_EQ_H     };
const svoWordU bdrSpeed_Eq_H[]          = {bdrSPEED_EQ_H         };

// UH-Group Coefficients (TBD):
const svoWordU bdrTracking_Eq_UH[]      = {bdrTRACKING_EQ_UH     };
const svoWordU bdrTracking_Low_Eq_UH[]  = {bdrTRACKING_LOW_EQ_UH };
const svoWordU bdrFocus_Eq_UH[]         = {bdrFOCUS_EQ_UH        };
const svoWordU bdrFocus_Low_Eq_UH[]     = {bdrFOCUS_LOW_EQ_UH    };
const svoWordU bdrSpeed_Eq_UH[]         = {bdrSPEED_EQ_UH        };

// Disc Servo Eq for Wobble Mode

const svoWordU *const bdrSvoEQCoefTbl[MAX_SPEED_BD+1][EQ_TBL_MAX+1] =
{
/* NONE */
    {NULL,                                         /* TRACKING_EQ          */
     NULL,                                         /* TRACKING_LOW_EQ      */
     NULL,                                         /* TRACKING_DC_EQ       */
     (svoWordU *const) bdTracking_Shock_Eq_Init,   /* TRACKING_SHOCK_EQ    */
     NULL,                                         /* FOCUS_EQ             */
     NULL,                                         /* FOCUS_LOW_EQ         */
     NULL,                                         /* FOCUS_DC_EQ          */
     (svoWordU *const) bdFocus_High_Eq_Init,       /* FOCUS_HIGH_EQ        */
     NULL,                                         /* DISK_FG_EQ           */
     NULL,                                         /* SLED_EQ              */
     (svoWordU *const) bdBeam_Low_Eq_Init,         /* BEAM_LOW_EQ          */
     (svoWordU *const) bdBeam_High_Eq_Init,        /* BEAM_HIGH_EQ         */
     NULL,                                         /* SPEED_EQ             */
     (svoWordU *const) bdAdjust_Eq_Init,           /* ADJUST_EQ            */
    },

/* X1 CLV */
    {(svoWordU *const) bdrTracking_Eq_UL,          /* TRACKING_EQ          */
     (svoWordU *const) bdrTracking_Low_Eq_UL,      /* TRACKING_LOW_EQ      */
     (svoWordU *const) bdTracking_DC_Eq_UL,        /* TRACKING_DC_EQ       */
     NULL,                                         /* TRACKING_SHOCK_EQ    */
     (svoWordU *const) bdrFocus_Eq_UL,             /* FOCUS_EQ             */
     (svoWordU *const) bdrFocus_Low_Eq_UL,         /* FOCUS_LOW_EQ         */
     (svoWordU *const) bdFocus_DC_Eq_UL,           /* FOCUS_DC_EQ          */
     NULL,                                         /* FOCUS_HIGH_EQ        */
     (svoWordU *const) bdDisk_Fg_Eq_UL,            /* DISK_FG_EQ           */
     (svoWordU *const) bdSled_Eq_UL,               /* SLED_EQ              */
     NULL,                                         /* BEAM_LOW_EQ          */
     NULL,                                         /* BEAM_HIGH_EQ         */
     (svoWordU *const) bdrSpeed_Eq_UL,             /* SPEED_EQ             */
     NULL,                                         /* ADJUST_EQ            */
    },

/* X2 CLV */
    {(svoWordU *const) bdrTracking_Eq_L,           /* TRACKING_EQ          */
     (svoWordU *const) bdrTracking_Low_Eq_L,       /* TRACKING_LOW_EQ      */
     (svoWordU *const) bdTracking_DC_Eq_L,         /* TRACKING_DC_EQ       */
     NULL,                                         /* TRACKING_SHOCK_EQ    */
     (svoWordU *const) bdrFocus_Eq_L,              /* FOCUS_EQ             */
     (svoWordU *const) bdrFocus_Low_Eq_L,          /* FOCUS_LOW_EQ         */
     (svoWordU *const) bdFocus_DC_Eq_L,            /* FOCUS_DC_EQ          */
     NULL,                                         /* FOCUS_HIGH_EQ        */
     (svoWordU *const) bdDisk_Fg_Eq_L,             /* DISK_FG_EQ           */
     (svoWordU *const) bdSled_Eq_L,                /* SLED_EQ              */
     NULL,                                         /* BEAM_LOW_EQ          */
     NULL,                                         /* BEAM_HIGH_EQ         */
     (svoWordU *const) bdrSpeed_Eq_L,              /* SPEED_EQ             */
     NULL,                                         /* ADJUST_EQ            */
    },

/* X3 CLV */
#if(BD2p2XCLV == 1)
     {(svoWordU *const) bdrTracking_Eq_L,           /* TRACKING_EQ          */
     (svoWordU *const) bdrTracking_Low_Eq_L,       /* TRACKING_LOW_EQ      */
     (svoWordU *const) bdTracking_DC_Eq_L,         /* TRACKING_DC_EQ       */
     NULL,                                         /* TRACKING_SHOCK_EQ    */
     (svoWordU *const) bdrFocus_Eq_L,              /* FOCUS_EQ             */
     (svoWordU *const) bdrFocus_Low_Eq_L,          /* FOCUS_LOW_EQ         */
     (svoWordU *const) bdFocus_DC_Eq_L,            /* FOCUS_DC_EQ          */
     NULL,                                         /* FOCUS_HIGH_EQ        */
     (svoWordU *const) bdDisk_Fg_Eq_L,             /* DISK_FG_EQ           */
     (svoWordU *const) bdSled_Eq_L,                /* SLED_EQ              */
     NULL,                                         /* BEAM_LOW_EQ          */
     NULL,                                         /* BEAM_HIGH_EQ         */
     (svoWordU *const) bdrSpeed_Eq_L,              /* SPEED_EQ             */
     NULL,                                         /* ADJUST_EQ            */
    },
#endif

/* X4 CLV */
//    {(svoWordU *const) bdrTracking_Eq_H,           /* TRACKING_EQ          */
//     (svoWordU *const) bdrTracking_Low_Eq_H,       /* TRACKING_LOW_EQ      */
//     (svoWordU *const) bdTracking_DC_Eq_H,         /* TRACKING_DC_EQ       */
//     NULL,                                         /* TRACKING_SHOCK_EQ    */
//     (svoWordU *const) bdrFocus_Eq_H,              /* FOCUS_EQ             */
//     (svoWordU *const) bdrFocus_Low_Eq_H,          /* FOCUS_LOW_EQ         */
//     (svoWordU *const) bdFocus_DC_Eq_H,            /* FOCUS_DC_EQ          */
//     NULL,                                         /* FOCUS_HIGH_EQ        */
//     (svoWordU *const) bdDisk_Fg_Eq_H,             /* DISK_FG_EQ           */
//     (svoWordU *const) bdSled_Eq_H,                /* SLED_EQ              */
//     NULL,                                         /* BEAM_LOW_EQ          */
//     NULL,                                         /* BEAM_HIGH_EQ         */
//     (svoWordU *const) bdrSpeed_Eq_H,              /* SPEED_EQ             */
//     NULL,                                         /* ADJUST_EQ            */
//    },

/* 4XCAV */
    {(svoWordU *const) bdrTracking_Eq_M,           /* TRACKING_EQ          */
     (svoWordU *const) bdrTracking_Low_Eq_M,       /* TRACKING_LOW_EQ      */
     (svoWordU *const) bdTracking_DC_Eq_M,         /* TRACKING_DC_EQ       */
     NULL,                                         /* TRACKING_SHOCK_EQ    */
     (svoWordU *const) bdrFocus_Eq_M,              /* FOCUS_EQ             */
     (svoWordU *const) bdrFocus_Low_Eq_M,          /* FOCUS_LOW_EQ         */
     (svoWordU *const) bdFocus_DC_Eq_M,            /* FOCUS_DC_EQ          */
     NULL,                                         /* FOCUS_HIGH_EQ        */
     (svoWordU *const) bdDisk_Fg_Eq_M,             /* DISK_FG_EQ           */
     (svoWordU *const) bdSled_Eq_M,                /* SLED_EQ              */
     NULL,                                         /* BEAM_LOW_EQ          */
     NULL,                                         /* BEAM_HIGH_EQ         */
     (svoWordU *const) bdrSpeed_Eq_M,              /* SPEED_EQ             */
     NULL,                                         /* ADJUST_EQ            */
    },


/* 6XCAV */
    {(svoWordU *const) bdrTracking_Eq_H,           /* TRACKING_EQ          */
     (svoWordU *const) bdrTracking_Low_Eq_H,       /* TRACKING_LOW_EQ      */
     (svoWordU *const) bdTracking_DC_Eq_H,         /* TRACKING_DC_EQ       */
     NULL,                                         /* TRACKING_SHOCK_EQ    */
     (svoWordU *const) bdrFocus_Eq_H,              /* FOCUS_EQ             */
     (svoWordU *const) bdrFocus_Low_Eq_H,          /* FOCUS_LOW_EQ         */
     (svoWordU *const) bdFocus_DC_Eq_H,            /* FOCUS_DC_EQ          */
     NULL,                                         /* FOCUS_HIGH_EQ        */
     (svoWordU *const) bdDisk_Fg_Eq_H,             /* DISK_FG_EQ           */
     (svoWordU *const) bdSled_Eq_H,                /* SLED_EQ              */
     NULL,                                         /* BEAM_LOW_EQ          */
     NULL,                                         /* BEAM_HIGH_EQ         */
     (svoWordU *const) bdrSpeed_Eq_H,              /* SPEED_EQ             */
     NULL,                                         /* ADJUST_EQ            */
    },

/* 8XCAV */
    {(svoWordU *const) bdrTracking_Eq_H,           /* TRACKING_EQ          */
     (svoWordU *const) bdrTracking_Low_Eq_H,       /* TRACKING_LOW_EQ      */
     (svoWordU *const) bdTracking_DC_Eq_H,         /* TRACKING_DC_EQ       */
     NULL,                                         /* TRACKING_SHOCK_EQ    */
     (svoWordU *const) bdrFocus_Eq_H,              /* FOCUS_EQ             */
     (svoWordU *const) bdrFocus_Low_Eq_H,          /* FOCUS_LOW_EQ         */
     (svoWordU *const) bdFocus_DC_Eq_H,            /* FOCUS_DC_EQ          */
     NULL,                                         /* FOCUS_HIGH_EQ        */
     (svoWordU *const) bdDisk_Fg_Eq_H,             /* DISK_FG_EQ           */
     (svoWordU *const) bdSled_Eq_H,                /* SLED_EQ              */
     NULL,                                         /* BEAM_LOW_EQ          */
     NULL,                                         /* BEAM_HIGH_EQ         */
     (svoWordU *const) bdrSpeed_Eq_H,              /* SPEED_EQ             */
     NULL,                                         /* ADJUST_EQ            */
    },

/* 10XCAV */
    {(svoWordU *const) bdrTracking_Eq_H,           /* TRACKING_EQ          */
     (svoWordU *const) bdrTracking_Low_Eq_H,       /* TRACKING_LOW_EQ      */
     (svoWordU *const) bdTracking_DC_Eq_H,         /* TRACKING_DC_EQ       */
     NULL,                                         /* TRACKING_SHOCK_EQ    */
     (svoWordU *const) bdrFocus_Eq_H,              /* FOCUS_EQ             */
     (svoWordU *const) bdrFocus_Low_Eq_H,          /* FOCUS_LOW_EQ         */
     (svoWordU *const) bdFocus_DC_Eq_H,            /* FOCUS_DC_EQ          */
     NULL,                                         /* FOCUS_HIGH_EQ        */
     (svoWordU *const) bdDisk_Fg_Eq_H,             /* DISK_FG_EQ           */
     (svoWordU *const) bdSled_Eq_H,                /* SLED_EQ              */
     NULL,                                         /* BEAM_LOW_EQ          */
     NULL,                                         /* BEAM_HIGH_EQ         */
     (svoWordU *const) bdrSpeed_Eq_H,              /* SPEED_EQ             */
     NULL,                                         /* ADJUST_EQ            */
    },
/* 12XCAV */
    {(svoWordU *const) bdrTracking_Eq_H,           /* TRACKING_EQ          */
     (svoWordU *const) bdrTracking_Low_Eq_H,       /* TRACKING_LOW_EQ      */
     (svoWordU *const) bdTracking_DC_Eq_H,         /* TRACKING_DC_EQ       */
     NULL,                                         /* TRACKING_SHOCK_EQ    */
     (svoWordU *const) bdrFocus_Eq_H,              /* FOCUS_EQ             */
     (svoWordU *const) bdrFocus_Low_Eq_H,          /* FOCUS_LOW_EQ         */
     (svoWordU *const) bdFocus_DC_Eq_H,            /* FOCUS_DC_EQ          */
     NULL,                                         /* FOCUS_HIGH_EQ        */
     (svoWordU *const) bdDisk_Fg_Eq_H,             /* DISK_FG_EQ           */
     (svoWordU *const) bdSled_Eq_H,                /* SLED_EQ              */
     NULL,                                         /* BEAM_LOW_EQ          */
     NULL,                                         /* BEAM_HIGH_EQ         */
     (svoWordU *const) bdrSpeed_Eq_H,              /* SPEED_EQ             */
     NULL,                                         /* ADJUST_EQ            */
    },
/* 14XCAV */
    {(svoWordU *const) bdrTracking_Eq_UH,          /* TRACKING_EQ          */
     (svoWordU *const) bdrTracking_Low_Eq_UH,      /* TRACKING_LOW_EQ      */
     (svoWordU *const) bdTracking_DC_Eq_UH,        /* TRACKING_DC_EQ       */
     NULL,                                         /* TRACKING_SHOCK_EQ    */
     (svoWordU *const) bdrFocus_Eq_UH,             /* FOCUS_EQ             */
     (svoWordU *const) bdrFocus_Low_Eq_UH,         /* FOCUS_LOW_EQ         */
     (svoWordU *const) bdFocus_DC_Eq_UH,           /* FOCUS_DC_EQ          */
     NULL,                                         /* FOCUS_HIGH_EQ        */
     (svoWordU *const) bdDisk_Fg_Eq_UH,            /* DISK_FG_EQ           */
     (svoWordU *const) bdSled_Eq_UH,               /* SLED_EQ              */
     NULL,                                         /* BEAM_LOW_EQ          */
     NULL,                                         /* BEAM_HIGH_EQ         */
     (svoWordU *const) bdrSpeed_Eq_UH,             /* SPEED_EQ             */
     NULL,                                         /* ADJUST_EQ            */
    },
};
#if (FINE_SEEK_SURF_TELFG == 0)
const svoWordU bdrFine_CE_Eq_UL[] = {bdrFINE_CE_EQ_UL };
const svoWordU bdrFine_CE_Eq_L[]  = {bdrFINE_CE_EQ_L  };
const svoWordU bdrFine_CE_Eq_M[]  = {bdrFINE_CE_EQ_M  };
const svoWordU bdrFine_CE_Eq_H[]  = {bdrFINE_CE_EQ_H  };
const svoWordU bdrFine_CE_Eq_UH[] = {bdrFINE_CE_EQ_UH };

const svoWordU (*const bdrFineCEEQCoefTbl[MAX_SPEED_BD]) =
{
    (svoWordU *const)bdrFine_CE_Eq_UL,  //_1LX_BD
    (svoWordU *const)bdrFine_CE_Eq_L,   //_2LX_BD
#if(BD2p2XCLV == 1)
    (svoWordU *const)bdrFine_CE_Eq_L,   //_2p2LX_BD
#endif
    (svoWordU *const)bdrFine_CE_Eq_M,   //_4AXCAV_BD
    (svoWordU *const)bdrFine_CE_Eq_M,   //_6AXCAV_BD
    (svoWordU *const)bdrFine_CE_Eq_H,   //_8AXCAV_BD
    (svoWordU *const)bdrFine_CE_Eq_H,   //_10AXCAV_BD
    (svoWordU *const)bdrFine_CE_Eq_H,   //_12AXCAV_BD
    (svoWordU *const)bdrFine_CE_Eq_UH,  //_14AXCAV_BD
};
#endif
#endif // BD_ENABLE
/*------------------------------------------------------------------------*/
/* The end of file                                                        */
/*------------------------------------------------------------------------*/
