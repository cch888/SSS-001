/****************************************************************************
*                (c) Copyright 2001 - 2008 SUNEXT DESIGN, INC.
*                           All Rights Reserved
*
*
* FILENAME:   dvSVO_TBL.C
*
* DESCRIPTION
*
*
*
* NOTES:
*
*
* $Revision: 367 $
* $Date: 11/03/28 4:07p $
*
**************************** SUNEXT CONFIDENTIAL ****************************/

#include ".\common\globals.h"
#include ".\common\globtype.h"
#include ".\common\chip.h"
#include ".\servo\svo.h"
#include ".\servo\svo_com.h"
#include ".\servo\svo_tbl.h"
#include ".\AL\REG_servo.h"
#include ".\AL\REG_write_strategy.h"
#include ".\AL\REG_global.h"
#include ".\AL\REG_periph.h"
#include ".\AL\REG_dvd_encoder.h"

/*----------------------------------------------------------------------------*/

#define DMCNT_DVD   (0x48|(DFRQ_FG<<2)|(DFRQ_WBL_DEC*0x03))
#define SPINCTL_DVD (DISK_AFCG_DVD << 1)
#define MSKON_DVD   (0x0051|(DISK_FR2X<<HAL_SRV_FR2X_LSB))
/*
 * Servo initial command data table
 */
svoRegInitTblT const dvsvoRegInitNONETbl[] =
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
    {HAL_SRV_MSKON_ADR,           MSKON_DVD,BYTESET}, /* 0xB000036C[7:0] double FG,invert FG, use FG noise mask, TR/FE loop gain excitation freq = 2.75K */
#if (FOOTILT_DIFFERENTIAL == 1)
    {HAL_SRV_FOODIFEN_ADR,         0x0010,       BYTESET}, /* 0xB0000118[7:0] SERVO DAC Switch */
#else
    {HAL_SRV_FOODIFEN_ADR,         0x0000,       BYTESET}, /* 0xB0000118[7:0] SERVO DAC Switch */
#endif
    {HAL_SRV_FETHR_ADR,           0x007F,       BYTESET}, /* 0xB000011C[7:0] FE threshold value */
    {HAL_SRV_FETHREN_ADR,HAL_SRV_FETHREN,       BIT_ON }, /* 0xB0000120[7:0] FE threshold enable */
    {HAL_SRV_AFCRNG1_ADR,         0x0020,       BYTESET}, /* 0xB0000138[6:0] AFC in range low */
    {HAL_SRV_AFCRNG2_ADR,         0x0038,       BYTESET}, /* 0xB000013C[6:0] AFC in range high */
    {HAL_SRV_SRVMOD_ADR,          0x0019,       BYTESET}, /* 0xB0000140[7:0] REGTON=0,DPLLMSK=0,DECCHG=010,SRVMOD=1 */
    {HAL_SRV_AFCG_ADR,       SPINCTL_DVD,       BYTESET}, /* 0xB00001DC[7:0] AFCG */
    {HAL_SRV_FGCMP_ADR,           0x013D,       WORDSET}, /* 0xB0000148[15:0] 33.8688e6/64/(2*23.14*36) = 635 */
    {HAL_SRV_FGRNG_ADR,           0x0020,       WORDSET}, /* 0xB00001F8[15:0] FG count range */
    {HAL_SRV_NDIV_ADR,            0x0002,       WORDSET}, /* 0xB00001F0[9:0] CLVN = 1 */
    {HAL_SRV_DFRQFG_ADR,       DMCNT_DVD,       BYTESET}, /* 0xB00003AC[7:0] disc motor control register */
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
    {HAL_SRV_DVDRHHLD_ADR,        0x0000,       BYTESET}, /* 0xB00003B8.0 DVD-RAM TE/FE hold during Header */
    {HAL_SRV_SYNCI_ADR,           0x0001,       WORDSET}, /* 0xB00003E8[12:0] Clear Interrput Status except the last bit */
    {HAL_SRV_DBT_ADR,             0x0000,       BYTESET}, /* 0xB0000334[6:0] Defect Delay Time = 8.4us */

#if (CHIP_REV <= 0xB3)
    //Set HW kick/brake parameters
    {HAL_SRV_ZCEAREN_ADR,         0x003F,       BYTESET}, /* 0xB0000058[7:0] EARZCPROT */
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


svoRegInitTblT const dvsvoRegInit2LXTbl[] =
{
    /* SERVO*/
    {HAL_SRV_NDIV_ADR,           0x0002,       WORDSET}, /* 0xB00001F0[9:0] CLVN = 2 */
    {HAL_SRV_DECCMP_ADR,         0x0C8A,       WORDSET}, /* 0xB0000158[15:0] 33.8688e6/8790/1.2 = 3853 */
    {HAL_SRV_DBT_ADR,            0x0000,       BYTESET}, /* 0xB0000334[6:0] Defect Delay Time */
#if (CHIP_REV <= 0xB3)
    {HAL_SRV_FVD_ADR,            0x0018,       BYTESET}, /* 0xB0000398[7:0] KWIDSL = 0us,FVD = 5.52KHz */
#endif
    /* Wobble page*/
  //  {rwDFCTIME,         0x66,       BYTESET}, //TBD:SE1  /* 0x1026 ADIP wobble defect time*/

    {SVOREGTBL_END,              0x0000,       BYTESET}  /* end code */
}; /* svoRegInit2XTbl[] */

svoRegInitTblT const dvsvoRegInit3LXTbl[] =
{
    /* SERVO*/
#if DVD2p4XCLV==1
    {HAL_SRV_NDIV_ADR,           0x0003,       WORDSET}, /* 0xB00001F0[9:0] CLVN = 3 */
    {HAL_SRV_DECCMP_ADR,         0x12D0,       WORDSET}, /* 0xB0000158[15:0] 33.8688e6/(8790*2.4/CLVN) = 4816 */
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

svoRegInitTblT const dvsvoRegInit4LXTbl[] =
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


#if(ENABLE_DVD_4XCAV ==1)
svoRegInitTblT const dvsvoRegInit4AXTbl[] =
{
    /* SERVO*/
    {HAL_SRV_NDIV_ADR,           0x0004,       WORDSET}, /* 0xB00001F0[9:0] CLVN = 4 */
    {HAL_SRV_DBT_ADR,            0x0000,       BYTESET}, /* 0xB0000334[6:0] Defect Delay Time */
#if (CHIP_REV <= 0xB3)
    {HAL_SRV_FVD_ADR,            0x0018,       BYTESET}, /* 0xB0000398[7:0] KWIDSL = 0us,FVD = 5.52KHz */
#endif

    {SVOREGTBL_END,              0x0000,       BYTESET}  /* end code */
}; /* svoRegInit4XTbl[] */
#endif

svoRegInitTblT const dvsvoRegInit6AXTbl[] =
{
    /* SERVO*/
    {HAL_SRV_NDIV_ADR,           0x0006,       WORDSET}, /* 0xB00001F0[9:0] CLVN = 6 */
    {HAL_SRV_DBT_ADR,            0x0000,       BYTESET}, /* 0xB0000334[6:0] Defect Delay Time */
#if (CHIP_REV <= 0xB3)
    {HAL_SRV_FVD_ADR,            0x0018,       BYTESET}, /* 0xB0000398[7:0] KWIDSL = 0us,FVD = 5.52KHz */
#endif

    {SVOREGTBL_END,              0x0000,       BYTESET}  /* end code */
}; /* svoRegInit6XTbl[] */

svoRegInitTblT const dvsvoRegInit8AXTbl[] =
{
    /* SERVO*/
    {HAL_SRV_NDIV_ADR,           0x0008,       WORDSET}, /* 0xB00001F0[9:0] CLVN = 8 */
    {HAL_SRV_DBT_ADR,            0x0000,       BYTESET}, /* 0xB0000334[6:0] Defect Delay Time = 8.4us */
#if (CHIP_REV <= 0xB3)
    {HAL_SRV_FVD_ADR,            0x0018,       BYTESET}, /* 0xB0000398[7:0] KWIDSL = 0us,FVD = 5.52KHz */
#endif

    {SVOREGTBL_END,              0x0000,       BYTESET}  /* end code */
}; /* svoRegInit8XTbl[] */

svoRegInitTblT const dvsvoRegInit10AXTbl[] =
{
    /* SERVO*/
    {HAL_SRV_NDIV_ADR,           0x000A,       WORDSET}, /* 0xB00001F0[9:0] CLVN = 10 */
    {HAL_SRV_DBT_ADR,            0x0000,       BYTESET}, /* 0xB0000334[6:0] Defect Delay Time = 8.4us */
#if (CHIP_REV <= 0xB3)
    {HAL_SRV_FVD_ADR,            0x0018,       BYTESET}, /* 0xB0000398[7:0] KWIDSL = 0us,FVD = 5.52KHz */
#endif

    {SVOREGTBL_END,              0x0000,       BYTESET}  /* end code */
}; /* svoRegInit10XTbl[] */

svoRegInitTblT const dvsvoRegInit12AXTbl[] =
{
    /* SERVO*/
    {HAL_SRV_NDIV_ADR,           0x0006,       WORDSET}, /* 0xB00001F0[9:0] CLVN = 6 */
    {HAL_SRV_DBT_ADR,            0x0000,       BYTESET}, /* 0xB0000334[6:0] Defect Delay Time = 8.4us */
#if (CHIP_REV <= 0xB3)
    {HAL_SRV_FVD_ADR,            0x0018,       BYTESET}, /* 0xB0000398[7:0] KWIDSL = 0us,FVD = 5.52KHz */
#endif

    {SVOREGTBL_END,              0x0000,       BYTESET}  /* end code */
}; /* svoRegInit12XTbl[] */

svoRegInitTblT const dvsvoRegInit16AXTbl[] =
{
    /* SERVO*/
    {HAL_SRV_NDIV_ADR,           0x0008,       WORDSET}, /* 0xB00001F0[9:0] CLVN = 8 */
    {HAL_SRV_DBT_ADR,            0x0000,       BYTESET}, /* 0xB0000334[6:0] Defect Delay Time = 8.4us */
#if (CHIP_REV <= 0xB3)
    {HAL_SRV_FVD_ADR,            0x0018,       BYTESET}, /* 0xB0000398[7:0] KWIDSL = 0us,FVD = 5.52KHz */
#endif

    {SVOREGTBL_END,              0x0000,       BYTESET}  /* end code */
}; /* svoRegInit16XTbl[] */

svoRegInitTblT const dvsvoRegInit20AXTbl[] =
{
    /* SERVO*/
    {HAL_SRV_NDIV_ADR,           0x000A,       WORDSET}, /* 0xB00001F0[9:0] CLVN = 10 */
    {HAL_SRV_DBT_ADR,            0x0000,       BYTESET}, /* 0xB0000334[6:0] Defect Delay Time = 8.4us */
#if (CHIP_REV <= 0xB3)
    {HAL_SRV_FVD_ADR,            0x0018,       BYTESET}, /* 0xB0000398[7:0] KWIDSL = 0us,FVD = 5.52KHz */
#endif

    {SVOREGTBL_END,              0x0000,       BYTESET}  /* end code */
}; /* svoRegInit20XTbl[] */

const svoRegInitTblT *const dvsvoRegInitTbl[MAX_SPEED_DVD+1] = {
    (svoRegInitTblT *const) dvsvoRegInitNONETbl  ,   // NONE
    (svoRegInitTblT *const) dvsvoRegInit2LXTbl   ,   // _2Lx
    (svoRegInitTblT *const) dvsvoRegInit3LXTbl   ,   // _3Lx
    (svoRegInitTblT *const) dvsvoRegInit4LXTbl   ,   // _4Lx
#if(ENABLE_DVD_4XCAV ==1)
    (svoRegInitTblT *const) dvsvoRegInit4AXTbl   ,   // _4AX
#endif
    (svoRegInitTblT *const) dvsvoRegInit6AXTbl   ,   // _6Ax
    (svoRegInitTblT *const) dvsvoRegInit8AXTbl   ,   // _8AX
    (svoRegInitTblT *const) dvsvoRegInit10AXTbl  ,   // _10Ax
    (svoRegInitTblT *const) dvsvoRegInit12AXTbl  ,   // _12Ax
    (svoRegInitTblT *const) dvsvoRegInit16AXTbl  ,   // _16Ax
    (svoRegInitTblT *const) dvsvoRegInit20AXTbl  ,   // _20Ax
};

/*----------------------------------------------------------------------------*/
/* Fine search KICKLVL coefficient                                            */
/*----------------------------------------------------------------------------*/
SEEK_GAIN const dvsvoKICKLVL_UL_F[] = {dvKICKLVL_EQ_UL_F};
SEEK_GAIN const dvsvoKICKLVL_L_F[]  = {dvKICKLVL_EQ_L_F};
SEEK_GAIN const dvsvoKICKLVL_M_F[]  = {dvKICKLVL_EQ_M_F};
SEEK_GAIN const dvsvoKICKLVL_H_F[]  = {dvKICKLVL_EQ_H_F};
SEEK_GAIN const dvsvoKICKLVL_UH_F[] = {dvKICKLVL_EQ_UH_F};

const SEEK_GAIN *const dvsvoKICKLVLTbl[MAX_SPEED_DVD]={
    (SEEK_GAIN *const) dvsvoKICKLVL_L_F,    // _2Lx
#if(DVD2p4XCLV == 1)
    (SEEK_GAIN *const) dvsvoKICKLVL_L_F,    // _2.4Lx
#else
    (SEEK_GAIN *const) dvsvoKICKLVL_M_F,    // _3Lx
#endif
    (SEEK_GAIN *const) dvsvoKICKLVL_M_F,    // _4Lx
#if(ENABLE_DVD_4XCAV ==1)
    (SEEK_GAIN *const) dvsvoKICKLVL_L_F,    // _4Ax
#endif
    (SEEK_GAIN *const) dvsvoKICKLVL_M_F,    // _6Ax
    (SEEK_GAIN *const) dvsvoKICKLVL_M_F,    // _8AX
    (SEEK_GAIN *const) dvsvoKICKLVL_H_F,    // _10Ax
    (SEEK_GAIN *const) dvsvoKICKLVL_H_F,    // _12Ax
    (SEEK_GAIN *const) dvsvoKICKLVL_H_F,    // _16Ax
    (SEEK_GAIN *const) dvsvoKICKLVL_UH_F,   // _20Ax
};

/*----------------------------------------------------------------------------*/
/* Fine search BRAKELVL coefficient                                           */
/*----------------------------------------------------------------------------*/
SEEK_GAIN const dvsvoBRAKELVL_UL_F[] = {dvBRAKELVL_EQ_UL_F};
SEEK_GAIN const dvsvoBRAKELVL_L_F[]  = {dvBRAKELVL_EQ_L_F};
SEEK_GAIN const dvsvoBRAKELVL_M_F[]  = {dvBRAKELVL_EQ_M_F};
SEEK_GAIN const dvsvoBRAKELVL_H_F[]  = {dvBRAKELVL_EQ_H_F};
SEEK_GAIN const dvsvoBRAKELVL_UH_F[] = {dvBRAKELVL_EQ_UH_F};

const SEEK_GAIN *const dvsvoBRAKELVLTbl[MAX_SPEED_DVD]={
    (SEEK_GAIN *const) dvsvoBRAKELVL_L_F,    // _2Lx
#if(DVD2p4XCLV == 1)
    (SEEK_GAIN *const) dvsvoBRAKELVL_L_F,    // _2.4Lx
#else
    (SEEK_GAIN *const) dvsvoBRAKELVL_M_F,    // _3Lx
#endif
    (SEEK_GAIN *const) dvsvoBRAKELVL_M_F,    // _4Lx
#if(ENABLE_DVD_4XCAV ==1)
    (SEEK_GAIN *const) dvsvoBRAKELVL_L_F,    // _4Ax
#endif
    (SEEK_GAIN *const) dvsvoBRAKELVL_M_F,    // _6Ax
    (SEEK_GAIN *const) dvsvoBRAKELVL_M_F,    // _8AX
    (SEEK_GAIN *const) dvsvoBRAKELVL_H_F,    // _10Ax
    (SEEK_GAIN *const) dvsvoBRAKELVL_H_F,    // _12Ax
    (SEEK_GAIN *const) dvsvoBRAKELVL_H_F,    // _16Ax
    (SEEK_GAIN *const) dvsvoBRAKELVL_UH_F,   // _20Ax
};

/*----------------------------------------------------------------------------*/
/* Fine search BRAKETIME coefficient                                          */
/*----------------------------------------------------------------------------*/
const BYTE dvsvoBRAKETIMETbl[MAX_SPEED_DVD]={
    dvBRAKETIME_EQ_L    ,   // _2Lx
#if(DVD2p4XCLV == 1)
    dvBRAKETIME_EQ_L    ,   // _2.4Lx
#else
    dvBRAKETIME_EQ_M    ,   // _3Lx
#endif
    dvBRAKETIME_EQ_M    ,   // _4Lx
#if(ENABLE_DVD_4XCAV ==1)
    dvBRAKETIME_EQ_L    ,   // _4Ax
#endif
    dvBRAKETIME_EQ_M    ,   // _6Ax
    dvBRAKETIME_EQ_M    ,   // _8AX
    dvBRAKETIME_EQ_H    ,   // _10Ax
    dvBRAKETIME_EQ_H    ,   // _12Ax
    dvBRAKETIME_EQ_H    ,   // _16Ax
    dvBRAKETIME_EQ_UH   ,   // _20Ax
};

/*----------------------------------------------------------------------------*/
/* Fine search BRAKESPD coefficient                                           */
/*----------------------------------------------------------------------------*/
const WORD dvsvoBRAKESPDTbl[MAX_SPEED_DVD]={
    dvBRAKESPD_EQ_L    ,   // _2Lx
#if(DVD2p4XCLV == 1)
    dvBRAKESPD_EQ_L    ,   // _2.4Lx
#else
    dvBRAKESPD_EQ_M    ,   // _3Lx
#endif
    dvBRAKESPD_EQ_M    ,   // _4Lx
#if(ENABLE_DVD_4XCAV ==1)
    dvBRAKESPD_EQ_L    ,   // _4Ax
#endif
    dvBRAKESPD_EQ_M    ,   // _6Ax
    dvBRAKESPD_EQ_M    ,   // _8AX
    dvBRAKESPD_EQ_H    ,   // _10Ax
    dvBRAKESPD_EQ_H    ,   // _12Ax
    dvBRAKESPD_EQ_H    ,   // _16Ax
    dvBRAKESPD_EQ_UH   ,   // _20Ax
};

/*----------------------------------------------------------------------------*/
/* Fine search kt1B(0x0B) coefficient                                         */
/*----------------------------------------------------------------------------*/
SEEK_GAIN const dvsvokt1BKick_UL_F[] = {dvKt1BKick_EQ_UL_F};
SEEK_GAIN const dvsvokt1BKick_L_F[]  = {dvKt1BKick_EQ_L_F};
SEEK_GAIN const dvsvokt1BKick_M_F[]  = {dvKt1BKick_EQ_M_F};
SEEK_GAIN const dvsvokt1BKick_H_F[]  = {dvKt1BKick_EQ_H_F};
SEEK_GAIN const dvsvokt1BKick_UH_F[] = {dvKt1BKick_EQ_UH_F};

const SEEK_GAIN *const dvsvokt1BKickTbl[MAX_SPEED_DVD]={
    (SEEK_GAIN *const) dvsvokt1BKick_L_F,    // _2Lx
#if(DVD2p4XCLV == 1)
    (SEEK_GAIN *const) dvsvokt1BKick_L_F,    // _2.4Lx
#else
    (SEEK_GAIN *const) dvsvokt1BKick_M_F,    // _3Lx
#endif
    (SEEK_GAIN *const) dvsvokt1BKick_M_F,    // _4Lx
#if(ENABLE_DVD_4XCAV ==1)
    (SEEK_GAIN *const) dvsvokt1BKick_L_F,    // _4Ax
#endif
    (SEEK_GAIN *const) dvsvokt1BKick_M_F,    // _6Ax
    (SEEK_GAIN *const) dvsvokt1BKick_M_F,    // _8AX
    (SEEK_GAIN *const) dvsvokt1BKick_H_F,    // _10Ax
    (SEEK_GAIN *const) dvsvokt1BKick_H_F,    // _12Ax
    (SEEK_GAIN *const) dvsvokt1BKick_H_F,    // _16Ax
    (SEEK_GAIN *const) dvsvokt1BKick_UH_F,   // _20Ax
};

#if (ACTIVE_SLED_FOR_FINE_JUMP == 0)
/*----------------------------------------------------------------------------*/
/* Fine search ks17(0x3E) coefficient                                         */
/*----------------------------------------------------------------------------*/
SEEK_GAIN const dvsvoks17Kick_UL_F_000[]  = {dvsvoks17Kick_UL_F};
SEEK_GAIN const dvsvoks17Kick_UL_B_000[]  = {dvsvoks17Kick_UL_B};
SEEK_GAIN const dvsvoks17Kick_L_F_000[]   = {dvsvoks17Kick_L_F};
SEEK_GAIN const dvsvoks17Kick_L_B_000[]   = {dvsvoks17Kick_L_B};
SEEK_GAIN const dvsvoks17Kick_M_F_000[]   = {dvsvoks17Kick_M_F};
SEEK_GAIN const dvsvoks17Kick_M_B_000[]   = {dvsvoks17Kick_M_B};
SEEK_GAIN const dvsvoks17Kick_H_F_000[]   = {dvsvoks17Kick_H_F};
SEEK_GAIN const dvsvoks17Kick_H_B_000[]   = {dvsvoks17Kick_H_B};
SEEK_GAIN const dvsvoks17Kick_UH_F_000[]  = {dvsvoks17Kick_UH_F};
SEEK_GAIN const dvsvoks17Kick_UH_B_000[]  = {dvsvoks17Kick_UH_B};

const SEEK_GAIN *const dvsvoks17KickTbl[MAX_SPEED_DVD]={
     (SEEK_GAIN *const) dvsvoks17Kick_L_F_000,    // _2Lx
#if(DVD2p4XCLV == 1)
     (SEEK_GAIN *const) dvsvoks17Kick_L_F_000,    // _2.4Lx
#else
     (SEEK_GAIN *const) dvsvoks17Kick_M_F_000,    // _3Lx
#endif
     (SEEK_GAIN *const) dvsvoks17Kick_M_F_000,    // _4Lx
    #if (ENABLE_DVD_4XCAV ==1)
     (SEEK_GAIN *const) dvsvoks17Kick_L_F_000,    // _4Ax
    #endif
     (SEEK_GAIN *const) dvsvoks17Kick_M_F_000,    // _6Ax
     (SEEK_GAIN *const) dvsvoks17Kick_M_F_000,    // _8AX
     (SEEK_GAIN *const) dvsvoks17Kick_H_F_000,    // _10AX
     (SEEK_GAIN *const) dvsvoks17Kick_H_F_000,    // _12Ax
     (SEEK_GAIN *const) dvsvoks17Kick_H_F_000,    // _16Ax
     (SEEK_GAIN *const) dvsvoks17Kick_UH_F_000,   // _20Ax
    }
#endif

/*----------------------------------------------------------------------------*/
/* Fine search KICKLVL coefficient                                            */
/*----------------------------------------------------------------------------*/
SEEK_GAIN const rdvsvoKICKLVL_UL_F[] = {rdvKICKLVL_EQ_UL_F};
SEEK_GAIN const rdvsvoKICKLVL_L_F[]  = {rdvKICKLVL_EQ_L_F};
SEEK_GAIN const rdvsvoKICKLVL_M_F[]  = {rdvKICKLVL_EQ_M_F};
SEEK_GAIN const rdvsvoKICKLVL_H_F[]  = {rdvKICKLVL_EQ_H_F};
SEEK_GAIN const rdvsvoKICKLVL_UH_F[] = {rdvKICKLVL_EQ_UH_F};

const SEEK_GAIN *const rdvsvoKICKLVLTbl[MAX_SPEED_DVD]={
    (SEEK_GAIN *const) rdvsvoKICKLVL_L_F,    // _2Lx
#if(DVD2p4XCLV == 1)
    (SEEK_GAIN *const) rdvsvoKICKLVL_L_F,    // _2.4Lx
#else
    (SEEK_GAIN *const) rdvsvoKICKLVL_M_F,    // _3Lx
#endif
    (SEEK_GAIN *const) rdvsvoKICKLVL_M_F,    // _4Lx
#if(ENABLE_DVD_4XCAV ==1)
    (SEEK_GAIN *const) rdvsvoKICKLVL_L_F,    // _4Ax
#endif
    (SEEK_GAIN *const) rdvsvoKICKLVL_M_F,    // _6Ax
    (SEEK_GAIN *const) rdvsvoKICKLVL_M_F,    // _8AX
    (SEEK_GAIN *const) rdvsvoKICKLVL_H_F,    // _10Ax
    (SEEK_GAIN *const) rdvsvoKICKLVL_H_F,    // _12Ax
    (SEEK_GAIN *const) rdvsvoKICKLVL_H_F,    // _16Ax
    (SEEK_GAIN *const) rdvsvoKICKLVL_UH_F,   // _20Ax
};

/*----------------------------------------------------------------------------*/
/* Fine search BRAKELVL coefficient                                           */
/*----------------------------------------------------------------------------*/
SEEK_GAIN const rdvsvoBRAKELVL_UL_F[] = {rdvBRAKELVL_EQ_UL_F};
SEEK_GAIN const rdvsvoBRAKELVL_L_F[]  = {rdvBRAKELVL_EQ_L_F};
SEEK_GAIN const rdvsvoBRAKELVL_M_F[]  = {rdvBRAKELVL_EQ_M_F};
SEEK_GAIN const rdvsvoBRAKELVL_H_F[]  = {rdvBRAKELVL_EQ_H_F};
SEEK_GAIN const rdvsvoBRAKELVL_UH_F[] = {rdvBRAKELVL_EQ_UH_F};

const SEEK_GAIN *const rdvsvoBRAKELVLTbl[MAX_SPEED_DVD]={
    (SEEK_GAIN *const) rdvsvoBRAKELVL_L_F,    // _2Lx
#if(DVD2p4XCLV == 1)
    (SEEK_GAIN *const) rdvsvoBRAKELVL_L_F,    // _2.4Lx
#else
    (SEEK_GAIN *const) rdvsvoBRAKELVL_M_F,    // _3Lx
#endif
    (SEEK_GAIN *const) rdvsvoBRAKELVL_M_F,    // _4Lx
#if(ENABLE_DVD_4XCAV ==1)
    (SEEK_GAIN *const) rdvsvoBRAKELVL_L_F,    // _4Ax
#endif
    (SEEK_GAIN *const) rdvsvoBRAKELVL_M_F,    // _6Ax
    (SEEK_GAIN *const) rdvsvoBRAKELVL_M_F,    // _8AX
    (SEEK_GAIN *const) rdvsvoBRAKELVL_H_F,    // _10Ax
    (SEEK_GAIN *const) rdvsvoBRAKELVL_H_F,    // _12Ax
    (SEEK_GAIN *const) rdvsvoBRAKELVL_H_F,    // _16Ax
    (SEEK_GAIN *const) rdvsvoBRAKELVL_UH_F,   // _20Ax
};

/*----------------------------------------------------------------------------*/
/* Fine search BRAKETIME coefficient                                          */
/*----------------------------------------------------------------------------*/
const BYTE rdvsvoBRAKETIMETbl[MAX_SPEED_DVD]={
    rdvBRAKETIME_EQ_L    ,   // _2Lx
#if(DVD2p4XCLV == 1)
    rdvBRAKETIME_EQ_L    ,   // _2.4Lx
#else
    rdvBRAKETIME_EQ_M    ,   // _3Lx
#endif
    rdvBRAKETIME_EQ_M    ,   // _4Lx
#if(ENABLE_DVD_4XCAV ==1)
    rdvBRAKETIME_EQ_L    ,   // _4Ax
#endif
    rdvBRAKETIME_EQ_M    ,   // _6Ax
    rdvBRAKETIME_EQ_M    ,   // _8AX
    rdvBRAKETIME_EQ_H    ,   // _10Ax
    rdvBRAKETIME_EQ_H    ,   // _12Ax
    rdvBRAKETIME_EQ_H    ,   // _16Ax
    rdvBRAKETIME_EQ_UH   ,   // _20Ax
};

/*----------------------------------------------------------------------------*/
/* Fine search BRAKESPD coefficient                                           */
/*----------------------------------------------------------------------------*/
const WORD rdvsvoBRAKESPDTbl[MAX_SPEED_DVD]={
    rdvBRAKESPD_EQ_L    ,   // _2Lx
#if(DVD2p4XCLV == 1)
    rdvBRAKESPD_EQ_L    ,   // _2.4Lx
#else
    rdvBRAKESPD_EQ_M    ,   // _3Lx
#endif
    rdvBRAKESPD_EQ_M    ,   // _4Lx
#if(ENABLE_DVD_4XCAV ==1)
    rdvBRAKESPD_EQ_L    ,   // _4Ax
#endif
    rdvBRAKESPD_EQ_M    ,   // _6Ax
    rdvBRAKESPD_EQ_M    ,   // _8AX
    rdvBRAKESPD_EQ_H    ,   // _10Ax
    rdvBRAKESPD_EQ_H    ,   // _12Ax
    rdvBRAKESPD_EQ_H    ,   // _16Ax
    rdvBRAKESPD_EQ_UH   ,   // _20Ax
};

/*----------------------------------------------------------------------------*/
/* Fine search kt1B(0x0B) coefficient                                         */
/*----------------------------------------------------------------------------*/
SEEK_GAIN const rdvsvokt1BKick_UL_F[] = {rdvKt1BKick_EQ_UL_F};
SEEK_GAIN const rdvsvokt1BKick_L_F[]  = {rdvKt1BKick_EQ_L_F};
SEEK_GAIN const rdvsvokt1BKick_M_F[]  = {rdvKt1BKick_EQ_M_F};
SEEK_GAIN const rdvsvokt1BKick_H_F[]  = {rdvKt1BKick_EQ_H_F};
SEEK_GAIN const rdvsvokt1BKick_UH_F[] = {rdvKt1BKick_EQ_UH_F};

const SEEK_GAIN *const rdvsvokt1BKickTbl[MAX_SPEED_DVD]={
    (SEEK_GAIN *const) rdvsvokt1BKick_L_F,    // _2Lx
#if(DVD2p4XCLV == 1)
    (SEEK_GAIN *const) rdvsvokt1BKick_L_F,    // _2.4Lx
#else
    (SEEK_GAIN *const) rdvsvokt1BKick_M_F,    // _3Lx
#endif
    (SEEK_GAIN *const) rdvsvokt1BKick_M_F,    // _4Lx
#if(ENABLE_DVD_4XCAV ==1)
    (SEEK_GAIN *const) rdvsvokt1BKick_L_F,    // _4Ax
#endif
    (SEEK_GAIN *const) rdvsvokt1BKick_M_F,    // _6Ax
    (SEEK_GAIN *const) rdvsvokt1BKick_M_F,    // _8Ax
    (SEEK_GAIN *const) rdvsvokt1BKick_H_F,    // _10Ax
    (SEEK_GAIN *const) rdvsvokt1BKick_H_F,    // _12Ax
    (SEEK_GAIN *const) rdvsvokt1BKick_H_F,    // _16Ax
    (SEEK_GAIN *const) rdvsvokt1BKick_UH_F,   // _20Ax
};

#if (ACTIVE_SLED_FOR_FINE_JUMP == 0)
/*----------------------------------------------------------------------------*/
/* Fine search ks17(0x3E) coefficient                                         */
/*----------------------------------------------------------------------------*/
SEEK_GAIN const rdvsvoks17Kick_UL_F_000[]  = {rdvsvoks17Kick_UL_F};
SEEK_GAIN const rdvsvoks17Kick_UL_B_000[]  = {rdvsvoks17Kick_UL_B};
SEEK_GAIN const rdvsvoks17Kick_L_F_000[]   = {rdvsvoks17Kick_L_F};
SEEK_GAIN const rdvsvoks17Kick_L_B_000[]   = {rdvsvoks17Kick_L_B};
SEEK_GAIN const rdvsvoks17Kick_M_F_000[]   = {rdvsvoks17Kick_M_F};
SEEK_GAIN const rdvsvoks17Kick_M_B_000[]   = {rdvsvoks17Kick_M_B};
SEEK_GAIN const rdvsvoks17Kick_H_F_000[]   = {rdvsvoks17Kick_H_F};
SEEK_GAIN const rdvsvoks17Kick_H_B_000[]   = {rdvsvoks17Kick_H_B};
SEEK_GAIN const rdvsvoks17Kick_UH_F_000[]  = {rdvsvoks17Kick_UH_F};
SEEK_GAIN const rdvsvoks17Kick_UH_B_000[]  = {rdvsvoks17Kick_UH_B};

const SEEK_GAIN *const rdvsvoks17KickTbl[MAX_SPEED_DVD]={
    (SEEK_GAIN *const) rdvsvoks17Kick_L_F_000,    // _2Lx
    #if(DVD2p4XCLV == 1)
    (SEEK_GAIN *const) rdvsvoks17Kick_L_F_000,    // _2.4Lx
    #else
    (SEEK_GAIN *const) rdvsvoks17Kick_M_F_000,    // _3Lx
    #endif
    (SEEK_GAIN *const) rdvsvoks17Kick_M_F_000,    // _4Lx
    #if (ENABLE_DVD_4XCAV ==1)
    (SEEK_GAIN *const) rdvsvoks17Kick_L_F_000,    // _4Ax
    #endif
    (SEEK_GAIN *const) rdvsvoks17Kick_M_F_000,    // _6Ax
    (SEEK_GAIN *const) rdvsvoks17Kick_M_F_000,    // _8AX
    (SEEK_GAIN *const) rdvsvoks17Kick_H_F_000,    // _10AX
    (SEEK_GAIN *const) rdvsvoks17Kick_H_F_000,    // _12Ax
    (SEEK_GAIN *const) rdvsvoks17Kick_H_F_000,    // _16Ax
    (SEEK_GAIN *const) rdvsvoks17Kick_UH_F_000,   // _20Ax
};
#endif

//**************************************************************************************************
// FOR DVD-ROM DISC
//**************************************************************************************************
const svoWordU dvTracking_Shock_Eq_Init[] = {
    0x0000,                       /* kt30  (0x1A) */
    0x0000,                       /* kt31  (0x1B) */
    0x0000,                       /* kt32  (0x1C) */
    0x0000,                       /* kt33  (0x1D) */
    0x0000,                       /* kt34  (0x1E) */
    0x0000                        /* kt35  (0x1F) */
};

const svoWordU dvFocus_High_Eq_Init[] = {
    0x7FFF,                       /* kf30  (0x2C) */
    0x8000,                       /* kf31  (0x2D) */
    0x7FE8                        /* kf32  (0x2E) */
};

const svoWordU dvBeam_Low_Eq_Init[] = {
    0x1780,                       /* kb10  (0x45) */
    0x7FE9,                       /* kb11  (0x46) */
    0x0080                        /* kb12  (0x47) */
};

const svoWordU dvBeam_High_Eq_Init[] = {
    0x0000,                       /* Kb20  (0x48) */
    0x0000,                       /* Kb21  (0x49) */
    0x0000                        /* Kb22  (0x4A) */
};

const svoWordU dvAdjust_Eq_Init[] = {
    FOCUS_SEARCH_KF18,            /* Kf18  (0x28) */
    0x7FFF,                       /* Kfb20 (0x55) */
    0x1000,                       /* Kfb21 (0x56) */
    0x0800,                       /* Krb10 (0x65) */
    0x0000                        /* Ks17  (0x3E) */
};

// UL-Group Coefficients (1XCLV):
const svoWordU dvTracking_Eq_UL[]      = {dvTRACKING_EQ_UL     };
const svoWordU dvTracking_Low_Eq_UL[]  = {dvTRACKING_LOW_EQ_UL };
const svoWordU dvTracking_DC_Eq_UL[]   = {dvTRK_DC_EQ_UL       };
const svoWordU dvFocus_Eq_UL[]         = {dvFOCUS_EQ_UL        };
const svoWordU dvFocus_Low_Eq_UL[]     = {dvFOCUS_LOW_EQ_UL    };
const svoWordU dvFocus_DC_Eq_UL[]      = {dvFOCUS_DC_EQ_UL     };
const svoWordU dvSpeed_Eq_UL[]         = {dvSPEED_EQ_UL        };
const svoWordU dvSled_Eq_UL[]          = {dvSLED_EQ_UL         };//12~35Hz
const svoWordU dvDisk_Fg_Eq_UL[]       = {dvDISK_FG_EQ_UL      };

// L-Group Coefficients (2XCLV, 2.4XCLV, 4XCAV):
const svoWordU dvTracking_Eq_L[]       = {dvTRACKING_EQ_L      };
const svoWordU dvTracking_Low_Eq_L[]   = {dvTRACKING_LOW_EQ_L  };
const svoWordU dvTracking_DC_Eq_L[]    = {dvTRK_DC_EQ_L        };
const svoWordU dvFocus_Eq_L[]          = {dvFOCUS_EQ_L         };
const svoWordU dvFocus_Low_Eq_L[]      = {dvFOCUS_LOW_EQ_L     };
const svoWordU dvFocus_DC_Eq_L[]       = {dvFOCUS_DC_EQ_L      };
const svoWordU dvSpeed_Eq_L[]          = {dvSPEED_EQ_L         };
const svoWordU dvSled_Eq_L[]           = {dvSLED_EQ_L          };//22~80Hz
const svoWordU dvDisk_Fg_Eq_L[]        = {dvDISK_FG_EQ_L       };

// M-Group Coefficients (3XCLV, 4XCLV, 5XCLV, 6XCLV, 6XCAV, 8XCAV):
const svoWordU dvTracking_Eq_M[]       = {dvTRACKING_EQ_M      };
const svoWordU dvTracking_Low_Eq_M[]   = {dvTRACKING_LOW_EQ_M  };
const svoWordU dvTracking_DC_Eq_M[]    = {dvTRK_DC_EQ_M        };
const svoWordU dvFocus_Eq_M[]          = {dvFOCUS_EQ_M         };
const svoWordU dvFocus_Low_Eq_M[]      = {dvFOCUS_LOW_EQ_M     };
const svoWordU dvFocus_DC_Eq_M[]       = {dvFOCUS_DC_EQ_M      };
const svoWordU dvSpeed_Eq_M[]          = {dvSPEED_EQ_M         };
const svoWordU dvSled_Eq_M[]           = {dvSLED_EQ_M          };//37~160Hz
const svoWordU dvDisk_Fg_Eq_M[]        = {dvDISK_FG_EQ_M       };

// H-Group Coefficients (8XCLV, 10XCLV, 12XCLV, 16XCLV, 10XCAV, 12XCAV, 16XCAV):
const svoWordU dvTracking_Eq_H[]       = {dvTRACKING_EQ_H      };
const svoWordU dvTracking_Low_Eq_H[]   = {dvTRACKING_LOW_EQ_H  };
const svoWordU dvTracking_DC_Eq_H[]    = {dvTRK_DC_EQ_H        };
const svoWordU dvFocus_Eq_H[]          = {dvFOCUS_EQ_H         };
const svoWordU dvFocus_Low_Eq_H[]      = {dvFOCUS_LOW_EQ_H     };
const svoWordU dvFocus_DC_Eq_H[]       = {dvFOCUS_DC_EQ_H      };
const svoWordU dvSpeed_Eq_H[]          = {dvSPEED_EQ_H         };
const svoWordU dvSled_Eq_H[]           = {dvSLED_EQ_H          };//74~160Hz
const svoWordU dvDisk_Fg_Eq_H[]        = {dvDISK_FG_EQ_H       };

// UH-Group Coefficients (20XCLV, 20XCAV):
const svoWordU dvTracking_Eq_UH[]      = {dvTRACKING_EQ_UH     };
const svoWordU dvTracking_Low_Eq_UH[]  = {dvTRACKING_LOW_EQ_UH };
const svoWordU dvTracking_DC_Eq_UH[]   = {dvTRK_DC_EQ_UH       };
const svoWordU dvFocus_Eq_UH[]         = {dvFOCUS_EQ_UH        };
const svoWordU dvFocus_Low_Eq_UH[]     = {dvFOCUS_LOW_EQ_UH    };
const svoWordU dvFocus_DC_Eq_UH[]      = {dvFOCUS_DC_EQ_UH     };
const svoWordU dvSpeed_Eq_UH[]         = {dvSPEED_EQ_UH        };
const svoWordU dvSled_Eq_UH[]          = {dvSLED_EQ_UH         };//155~185Hz
const svoWordU dvDisk_Fg_Eq_UH[]       = {dvDISK_FG_EQ_UH      };
// Disc Servo Eq for Decoder Mode

const svoWordU *const dvSvoEQCoefTbl[MAX_SPEED_DVD+1][EQ_TBL_MAX+1] =
{
/* NONE */
    {NULL,                                        /* TRACKING_EQ          */
     NULL,                                        /* TRACKING_LOW_EQ      */
     NULL,                                        /* TRACKING_DC_EQ       */
     (svoWordU *const) dvTracking_Shock_Eq_Init,  /* TRACKING_SHOCK_EQ    */
     NULL,                                        /* FOCUS_EQ             */
     NULL,                                        /* FOCUS_LOW_EQ         */
     NULL,                                        /* FOCUS_DC_EQ          */
     (svoWordU *const) dvFocus_High_Eq_Init,      /* FOCUS_HIGH_EQ        */
     NULL,                                        /* DISK_FG_EQ           */
     NULL,                                        /* SLED_EQ              */
     (svoWordU *const) dvBeam_Low_Eq_Init,        /* BEAM_LOW_EQ          */
     (svoWordU *const) dvBeam_High_Eq_Init,       /* BEAM_HIGH_EQ         */
     NULL,                                        /* SPEED_EQ             */
     (svoWordU *const) dvAdjust_Eq_Init,          /* ADJUST_EQ            */
    },

/* X2 CLV */
    {(svoWordU *const) dvTracking_Eq_L,           /* TRACKING_EQ          */
     (svoWordU *const) dvTracking_Low_Eq_L,       /* TRACKING_LOW_EQ      */
     (svoWordU *const) dvTracking_DC_Eq_L,        /* TRACKING_DC_EQ       */
     NULL,                                        /* TRACKING_SHOCK_EQ    */
     (svoWordU *const) dvFocus_Eq_L,              /* FOCUS_EQ             */
     (svoWordU *const) dvFocus_Low_Eq_L,          /* FOCUS_LOW_EQ         */
     (svoWordU *const) dvFocus_DC_Eq_L,           /* FOCUS_DC_EQ          */
     NULL,                                        /* FOCUS_HIGH_EQ        */
     (svoWordU *const) dvDisk_Fg_Eq_L,            /* DISK_FG_EQ           */
     (svoWordU *const) dvSled_Eq_L,               /* SLED_EQ              */
     NULL,                                        /* BEAM_LOW_EQ          */
     NULL,                                        /* BEAM_HIGH_EQ         */
     (svoWordU *const) dvSpeed_Eq_L,              /* SPEED_EQ             */
     NULL,                                        /* ADJUST_EQ            */
    },
#if(DVD2p4XCLV == 1)
/* X2.4 CLV */
    {(svoWordU *const) dvTracking_Eq_L,           /* TRACKING_EQ          */
     (svoWordU *const) dvTracking_Low_Eq_L,       /* TRACKING_LOW_EQ      */
     (svoWordU *const) dvTracking_DC_Eq_L,        /* TRACKING_DC_EQ       */
     NULL,                                        /* TRACKING_SHOCK_EQ    */
     (svoWordU *const) dvFocus_Eq_L,              /* FOCUS_EQ             */
     (svoWordU *const) dvFocus_Low_Eq_L,          /* FOCUS_LOW_EQ         */
     (svoWordU *const) dvFocus_DC_Eq_L,           /* FOCUS_DC_EQ          */
     NULL,                                        /* FOCUS_HIGH_EQ        */
     (svoWordU *const) dvDisk_Fg_Eq_L,            /* DISK_FG_EQ           */
     (svoWordU *const) dvSled_Eq_L,               /* SLED_EQ              */
     NULL,                                        /* BEAM_LOW_EQ          */
     NULL,                                        /* BEAM_HIGH_EQ         */
     (svoWordU *const) dvSpeed_Eq_L,              /* SPEED_EQ             */
     NULL,                                        /* ADJUST_EQ            */
    },
#else
/* X3 CLV */
    {(svoWordU *const) dvTracking_Eq_M,           /* TRACKING_EQ          */
     (svoWordU *const) dvTracking_Low_Eq_M,       /* TRACKING_LOW_EQ      */
     (svoWordU *const) dvTracking_DC_Eq_M,        /* TRACKING_DC_EQ       */
     NULL,                                        /* TRACKING_SHOCK_EQ    */
     (svoWordU *const) dvFocus_Eq_M,              /* FOCUS_EQ             */
     (svoWordU *const) dvFocus_Low_Eq_M,          /* FOCUS_LOW_EQ         */
     (svoWordU *const) dvFocus_DC_Eq_M,           /* FOCUS_DC_EQ          */
     NULL,                                        /* FOCUS_HIGH_EQ        */
     (svoWordU *const) dvDisk_Fg_Eq_M,            /* DISK_FG_EQ           */
     (svoWordU *const) dvSled_Eq_M,               /* SLED_EQ              */
     NULL,                                        /* BEAM_LOW_EQ          */
     NULL,                                        /* BEAM_HIGH_EQ         */
     (svoWordU *const) dvSpeed_Eq_M,              /* SPEED_EQ             */
     NULL,                                        /* ADJUST_EQ            */
    },
#endif
/* X4 CLV */
    {(svoWordU *const) dvTracking_Eq_M,           /* TRACKING_EQ          */
     (svoWordU *const) dvTracking_Low_Eq_M,       /* TRACKING_LOW_EQ      */
     (svoWordU *const) dvTracking_DC_Eq_M,        /* TRACKING_DC_EQ       */
     NULL,                                        /* TRACKING_SHOCK_EQ    */
     (svoWordU *const) dvFocus_Eq_M,              /* FOCUS_EQ             */
     (svoWordU *const) dvFocus_Low_Eq_M,          /* FOCUS_LOW_EQ         */
     (svoWordU *const) dvFocus_DC_Eq_M,           /* FOCUS_DC_EQ          */
     NULL,                                        /* FOCUS_HIGH_EQ        */
     (svoWordU *const) dvDisk_Fg_Eq_M,            /* DISK_FG_EQ           */
     (svoWordU *const) dvSled_Eq_M,               /* SLED_EQ              */
     NULL,                                        /* BEAM_LOW_EQ          */
     NULL,                                        /* BEAM_HIGH_EQ         */
     (svoWordU *const) dvSpeed_Eq_M,              /* SPEED_EQ             */
     NULL,                                        /* ADJUST_EQ            */
    },

#if(ENABLE_DVD_4XCAV == 1)
/* X4 CAV */
    {(svoWordU *const) dvTracking_Eq_L,           /* TRACKING_EQ          */
     (svoWordU *const) dvTracking_Low_Eq_L,       /* TRACKING_LOW_EQ      */
     (svoWordU *const) dvTracking_DC_Eq_L,        /* TRACKING_DC_EQ       */
     NULL,                                        /* TRACKING_SHOCK_EQ    */
     (svoWordU *const) dvFocus_Eq_L,              /* FOCUS_EQ             */
     (svoWordU *const) dvFocus_Low_Eq_L,          /* FOCUS_LOW_EQ         */
     (svoWordU *const) dvFocus_DC_Eq_L,           /* FOCUS_DC_EQ          */
     NULL,                                        /* FOCUS_HIGH_EQ        */
     (svoWordU *const) dvDisk_Fg_Eq_L,            /* DISK_FG_EQ           */
     (svoWordU *const) dvSled_Eq_M,               /* SLED_EQ              */
     NULL,                                        /* BEAM_LOW_EQ          */
     NULL,                                        /* BEAM_HIGH_EQ         */
     (svoWordU *const) dvSpeed_Eq_L,              /* SPEED_EQ             */
     NULL,                                        /* ADJUST_EQ            */
    },
#endif

/* X6 CAV */
    {(svoWordU *const) dvTracking_Eq_M,           /* TRACKING_EQ          */
     (svoWordU *const) dvTracking_Low_Eq_M,       /* TRACKING_LOW_EQ      */
     (svoWordU *const) dvTracking_DC_Eq_M,        /* TRACKING_DC_EQ       */
     NULL,                                        /* TRACKING_SHOCK_EQ    */
     (svoWordU *const) dvFocus_Eq_M,              /* FOCUS_EQ             */
     (svoWordU *const) dvFocus_Low_Eq_M,          /* FOCUS_LOW_EQ         */
     (svoWordU *const) dvFocus_DC_Eq_M,           /* FOCUS_DC_EQ          */
     NULL,                                        /* FOCUS_HIGH_EQ        */
     (svoWordU *const) dvDisk_Fg_Eq_M,            /* DISK_FG_EQ           */
     (svoWordU *const) dvSled_Eq_M,               /* SLED_EQ              */
     NULL,                                        /* BEAM_LOW_EQ          */
     NULL,                                        /* BEAM_HIGH_EQ         */
     (svoWordU *const) dvSpeed_Eq_M,              /* SPEED_EQ             */
     NULL,                                        /* ADJUST_EQ            */
    },

/* X8 CAV */
    {(svoWordU *const) dvTracking_Eq_M,           /* TRACKING_EQ          */
     (svoWordU *const) dvTracking_Low_Eq_M,       /* TRACKING_LOW_EQ      */
     (svoWordU *const) dvTracking_DC_Eq_M,        /* TRACKING_DC_EQ       */
     NULL,                                        /* TRACKING_SHOCK_EQ    */
     (svoWordU *const) dvFocus_Eq_M,              /* FOCUS_EQ             */
     (svoWordU *const) dvFocus_Low_Eq_M,          /* FOCUS_LOW_EQ         */
     (svoWordU *const) dvFocus_DC_Eq_M,           /* FOCUS_DC_EQ          */
     NULL,                                        /* FOCUS_HIGH_EQ        */
     (svoWordU *const) dvDisk_Fg_Eq_M,            /* DISK_FG_EQ           */
     (svoWordU *const) dvSled_Eq_H,               /* SLED_EQ              */
     NULL,                                        /* BEAM_LOW_EQ          */
     NULL,                                        /* BEAM_HIGH_EQ         */
     (svoWordU *const) dvSpeed_Eq_M,              /* SPEED_EQ             */
     NULL,                                        /* ADJUST_EQ            */
    },

/* X10 CAV */
    {(svoWordU *const) dvTracking_Eq_H,           /* TRACKING_EQ          */
     (svoWordU *const) dvTracking_Low_Eq_H,       /* TRACKING_LOW_EQ      */
     (svoWordU *const) dvTracking_DC_Eq_H,        /* TRACKING_DC_EQ       */
     NULL,                                        /* TRACKING_SHOCK_EQ    */
     (svoWordU *const) dvFocus_Eq_H,              /* FOCUS_EQ             */
     (svoWordU *const) dvFocus_Low_Eq_H,          /* FOCUS_LOW_EQ         */
     (svoWordU *const) dvFocus_DC_Eq_H,           /* FOCUS_DC_EQ          */
     NULL,                                        /* FOCUS_HIGH_EQ        */
     (svoWordU *const) dvDisk_Fg_Eq_H,            /* DISK_FG_EQ           */
     (svoWordU *const) dvSled_Eq_H,               /* SLED_EQ              */
     NULL,                                        /* BEAM_LOW_EQ          */
     NULL,                                        /* BEAM_HIGH_EQ         */
     (svoWordU *const) dvSpeed_Eq_H,              /* SPEED_EQ             */
     NULL,                                        /* ADJUST_EQ            */
    },

/* X12 CAV */
    {(svoWordU *const) dvTracking_Eq_H,           /* TRACKING_EQ          */
     (svoWordU *const) dvTracking_Low_Eq_H,       /* TRACKING_LOW_EQ      */
     (svoWordU *const) dvTracking_DC_Eq_H,        /* TRACKING_DC_EQ       */
     NULL,                                        /* TRACKING_SHOCK_EQ    */
     (svoWordU *const) dvFocus_Eq_H,              /* FOCUS_EQ             */
     (svoWordU *const) dvFocus_Low_Eq_H,          /* FOCUS_LOW_EQ         */
     (svoWordU *const) dvFocus_DC_Eq_H,           /* FOCUS_DC_EQ          */
     NULL,                                        /* FOCUS_HIGH_EQ        */
     (svoWordU *const) dvDisk_Fg_Eq_H,            /* DISK_FG_EQ           */
     (svoWordU *const) dvSled_Eq_H,               /* SLED_EQ              */
     NULL,                                        /* BEAM_LOW_EQ          */
     NULL,                                        /* BEAM_HIGH_EQ         */
     (svoWordU *const) dvSpeed_Eq_H,              /* SPEED_EQ             */
     NULL,                                        /* ADJUST_EQ            */
    },

/* X16 CAV */
    {(svoWordU *const) dvTracking_Eq_H,           /* TRACKING_EQ          */
     (svoWordU *const) dvTracking_Low_Eq_H,       /* TRACKING_LOW_EQ      */
     (svoWordU *const) dvTracking_DC_Eq_H,        /* TRACKING_DC_EQ       */
     NULL,                                        /* TRACKING_SHOCK_EQ    */
     (svoWordU *const) dvFocus_Eq_H,              /* FOCUS_EQ             */
     (svoWordU *const) dvFocus_Low_Eq_H,          /* FOCUS_LOW_EQ         */
     (svoWordU *const) dvFocus_DC_Eq_H,           /* FOCUS_DC_EQ          */
     NULL,                                        /* FOCUS_HIGH_EQ        */
     (svoWordU *const) dvDisk_Fg_Eq_H,            /* DISK_FG_EQ           */
     (svoWordU *const) dvSled_Eq_H,               /* SLED_EQ              */
     NULL,                                        /* BEAM_LOW_EQ          */
     NULL,                                        /* BEAM_HIGH_EQ         */
     (svoWordU *const) dvSpeed_Eq_H,              /* SPEED_EQ             */
     NULL,                                        /* ADJUST_EQ            */
    },

/* X20 CAV */
    {(svoWordU *const) dvTracking_Eq_UH,          /* TRACKING_EQ          */
     (svoWordU *const) dvTracking_Low_Eq_UH,      /* TRACKING_LOW_EQ      */
     (svoWordU *const) dvTracking_DC_Eq_UH,       /* TRACKING_DC_EQ       */
     NULL,                                        /* TRACKING_SHOCK_EQ    */
     (svoWordU *const) dvFocus_Eq_UH,             /* FOCUS_EQ             */
     (svoWordU *const) dvFocus_Low_Eq_UH,         /* FOCUS_LOW_EQ         */
     (svoWordU *const) dvFocus_DC_Eq_UH,          /* FOCUS_DC_EQ          */
     NULL,                                        /* FOCUS_HIGH_EQ        */
     (svoWordU *const) dvDisk_Fg_Eq_UH,           /* DISK_FG_EQ           */
     (svoWordU *const) dvSled_Eq_UH,              /* SLED_EQ              */
     NULL,                                        /* BEAM_LOW_EQ          */
     NULL,                                        /* BEAM_HIGH_EQ         */
     (svoWordU *const) dvSpeed_Eq_UH,             /* SPEED_EQ             */
     NULL,                                        /* ADJUST_EQ            */
    }
};
#if (FINE_SEEK_SURF_TELFG == 0)
const svoWordU dvFine_CE_Eq_UL[] = {dvFINE_CE_EQ_UL };
const svoWordU dvFine_CE_Eq_L[]  = {dvFINE_CE_EQ_L  };
const svoWordU dvFine_CE_Eq_M[]  = {dvFINE_CE_EQ_M  };
const svoWordU dvFine_CE_Eq_H[]  = {dvFINE_CE_EQ_H  };
const svoWordU dvFine_CE_Eq_UH[] = {dvFINE_CE_EQ_UH };

const svoWordU (*const dvFineCEEQCoefTbl[MAX_SPEED_DVD]) =
{
    (svoWordU *const)dvFine_CE_Eq_L,   //_2LX_DVD
    #if(DVD2p4XCLV == 1)
    (svoWordU *const)dvFine_CE_Eq_L,   //_2.4LX_DVD
    #else
    (svoWordU *const)dvFine_CE_Eq_M,   //_3LX_DVD
    #endif
    (svoWordU *const)dvFine_CE_Eq_M,   //_4LX_DVD
    #if(ENABLE_DVD_4XCAV == 1)
    (svoWordU *const)dvFine_CE_Eq_L,   //_4AX_DVD
    #endif
    (svoWordU *const)dvFine_CE_Eq_M,   //_6AX_DVD
    (svoWordU *const)dvFine_CE_Eq_M,   //_8AX_DVD
    (svoWordU *const)dvFine_CE_Eq_H,   //_10AX_DVD
    (svoWordU *const)dvFine_CE_Eq_H,   //_12AX_DVD
    (svoWordU *const)dvFine_CE_Eq_H,   //_16AX_DVD
    (svoWordU *const)dvFine_CE_Eq_UH   //_20AX_DVD
};
#endif

const svoWordU dvRoughCEEQ[]     = {dvROUGH_CE_EQ_R};
const svoWordU dvRoughCELFG[]    = {dvROUGH_CE_LOW_EQ_R};

//**************************************************************************************************
// FOR DVD+R/-R/+RW/-RW DISC
//**************************************************************************************************
// UL-Group Coefficients (1XCLV):
const svoWordU rdvTracking_Eq_UL[]      = {rdvTRACKING_EQ_UL     };
const svoWordU rdvTracking_Low_Eq_UL[]  = {rdvTRACKING_LOW_EQ_UL };
const svoWordU rdvFocus_Eq_UL[]         = {rdvFOCUS_EQ_UL        };
const svoWordU rdvFocus_Low_Eq_UL[]     = {rdvFOCUS_LOW_EQ_UL    };
const svoWordU rdvSpeed_Eq_UL[]         = {rdvSPEED_EQ_UL        };

// L-Group Coefficients (2XCLV, 2.4XCLV, 4XCAV):
const svoWordU rdvTracking_Eq_L[]       = {rdvTRACKING_EQ_L      };
const svoWordU rdvTracking_Low_Eq_L[]   = {rdvTRACKING_LOW_EQ_L  };
const svoWordU rdvFocus_Eq_L[]          = {rdvFOCUS_EQ_L         };
const svoWordU rdvFocus_Low_Eq_L[]      = {rdvFOCUS_LOW_EQ_L     };
const svoWordU rdvSpeed_Eq_L[]          = {rdvSPEED_EQ_L         };

// M-Group Coefficients (3XCLV, 4XCLV, 5XCLV, 6XCLV, 6XCAV, 8XCAV):
const svoWordU rdvTracking_Eq_M[]       = {rdvTRACKING_EQ_M      };
const svoWordU rdvTracking_Low_Eq_M[]   = {rdvTRACKING_LOW_EQ_M  };
const svoWordU rdvFocus_Eq_M[]          = {rdvFOCUS_EQ_M         };
const svoWordU rdvFocus_Low_Eq_M[]      = {rdvFOCUS_LOW_EQ_M     };
const svoWordU rdvSpeed_Eq_M[]          = {rdvSPEED_EQ_M         };

// H-Group Coefficients (8XCLV, 10XCLV, 12XCLV, 16XCLV, 10XCAV, 12XCAV, 16XCAV):
const svoWordU rdvTracking_Eq_H[]       = {rdvTRACKING_EQ_H      };
const svoWordU rdvTracking_Low_Eq_H[]   = {rdvTRACKING_LOW_EQ_H  };
const svoWordU rdvFocus_Eq_H[]          = {rdvFOCUS_EQ_H         };
const svoWordU rdvFocus_Low_Eq_H[]      = {rdvFOCUS_LOW_EQ_H     };
const svoWordU rdvSpeed_Eq_H[]          = {rdvSPEED_EQ_H         };

// UH-Group Coefficients (20XCLV, 20XCAV):
const svoWordU rdvTracking_Eq_UH[]      = {rdvTRACKING_EQ_UH     };
const svoWordU rdvTracking_Low_Eq_UH[]  = {rdvTRACKING_LOW_EQ_UH };
const svoWordU rdvFocus_Eq_UH[]         = {rdvFOCUS_EQ_UH        };
const svoWordU rdvFocus_Low_Eq_UH[]     = {rdvFOCUS_LOW_EQ_UH    };
const svoWordU rdvSpeed_Eq_UH[]         = {rdvSPEED_EQ_UH        };

// Disc Servo Eq for Wobble Mode

const svoWordU *const rdvSvoEQCoefTbl[MAX_SPEED_DVD+1][EQ_TBL_MAX+1] =
{
/* NONE */
    {NULL,                                         /* TRACKING_EQ          */
     NULL,                                         /* TRACKING_LOW_EQ      */
     NULL,                                         /* TRACKING_DC_EQ       */
     (svoWordU *const) dvTracking_Shock_Eq_Init,   /* TRACKING_SHOCK_EQ    */
     NULL,                                         /* FOCUS_EQ             */
     NULL,                                         /* FOCUS_LOW_EQ         */
     NULL,                                         /* FOCUS_DC_EQ          */
     (svoWordU *const) dvFocus_High_Eq_Init,       /* FOCUS_HIGH_EQ        */
     NULL,                                         /* DISK_FG_EQ           */
     NULL,                                         /* SLED_EQ              */
     (svoWordU *const) dvBeam_Low_Eq_Init,         /* BEAM_LOW_EQ          */
     (svoWordU *const) dvBeam_High_Eq_Init,        /* BEAM_HIGH_EQ         */
     NULL,                                         /* SPEED_EQ             */
     (svoWordU *const) dvAdjust_Eq_Init,           /* ADJUST_EQ            */
    },

/* X2 CLV */
    {(svoWordU *const) rdvTracking_Eq_L,           /* TRACKING_EQ          */
     (svoWordU *const) rdvTracking_Low_Eq_L,       /* TRACKING_LOW_EQ      */
     (svoWordU *const) dvTracking_DC_Eq_L,         /* TRACKING_DC_EQ       */
     NULL,                                         /* TRACKING_SHOCK_EQ    */
     (svoWordU *const) rdvFocus_Eq_L,              /* FOCUS_EQ             */
     (svoWordU *const) rdvFocus_Low_Eq_L,          /* FOCUS_LOW_EQ         */
     (svoWordU *const) dvFocus_DC_Eq_L,            /* FOCUS_DC_EQ          */
     NULL,                                         /* FOCUS_HIGH_EQ        */
     (svoWordU *const) dvDisk_Fg_Eq_L,             /* DISK_FG_EQ           */
     (svoWordU *const) dvSled_Eq_L,                /* SLED_EQ              */
     NULL,                                         /* BEAM_LOW_EQ          */
     NULL,                                         /* BEAM_HIGH_EQ         */
     (svoWordU *const) rdvSpeed_Eq_L,              /* SPEED_EQ             */
     NULL,                                         /* ADJUST_EQ            */
    },
#if(DVD2p4XCLV == 1)
/* X2.4 CLV */
    {(svoWordU *const) rdvTracking_Eq_L,           /* TRACKING_EQ          */
     (svoWordU *const) rdvTracking_Low_Eq_L,       /* TRACKING_LOW_EQ      */
     (svoWordU *const) dvTracking_DC_Eq_L,         /* TRACKING_DC_EQ       */
     NULL,                                         /* TRACKING_SHOCK_EQ    */
     (svoWordU *const) rdvFocus_Eq_L,              /* FOCUS_EQ             */
     (svoWordU *const) rdvFocus_Low_Eq_L,          /* FOCUS_LOW_EQ         */
     (svoWordU *const) dvFocus_DC_Eq_L,            /* FOCUS_DC_EQ          */
     NULL,                                         /* FOCUS_HIGH_EQ        */
     (svoWordU *const) dvDisk_Fg_Eq_L,             /* DISK_FG_EQ           */
     (svoWordU *const) dvSled_Eq_L,                /* SLED_EQ              */
     NULL,                                         /* BEAM_LOW_EQ          */
     NULL,                                         /* BEAM_HIGH_EQ         */
     (svoWordU *const) rdvSpeed_Eq_L,              /* SPEED_EQ             */
     NULL,                                         /* ADJUST_EQ            */
    },
#else
/* X3 CLV */
    {(svoWordU *const) rdvTracking_Eq_M,           /* TRACKING_EQ          */
     (svoWordU *const) rdvTracking_Low_Eq_M,       /* TRACKING_LOW_EQ      */
     (svoWordU *const) dvTracking_DC_Eq_M,         /* TRACKING_DC_EQ       */
     NULL,                                         /* TRACKING_SHOCK_EQ    */
     (svoWordU *const) rdvFocus_Eq_M,              /* FOCUS_EQ             */
     (svoWordU *const) rdvFocus_Low_Eq_M,          /* FOCUS_LOW_EQ         */
     (svoWordU *const) dvFocus_DC_Eq_M,            /* FOCUS_DC_EQ          */
     NULL,                                         /* FOCUS_HIGH_EQ        */
     (svoWordU *const) dvDisk_Fg_Eq_M,             /* DISK_FG_EQ           */
     (svoWordU *const) dvSled_Eq_M,                /* SLED_EQ              */
     NULL,                                         /* BEAM_LOW_EQ          */
     NULL,                                         /* BEAM_HIGH_EQ         */
     (svoWordU *const) rdvSpeed_Eq_M,              /* SPEED_EQ             */
     NULL,                                         /* ADJUST_EQ            */
    },
#endif
/* X4 CLV */
    {(svoWordU *const) rdvTracking_Eq_M,           /* TRACKING_EQ          */
     (svoWordU *const) rdvTracking_Low_Eq_M,       /* TRACKING_LOW_EQ      */
     (svoWordU *const) dvTracking_DC_Eq_M,         /* TRACKING_DC_EQ       */
     NULL,                                         /* TRACKING_SHOCK_EQ    */
     (svoWordU *const) rdvFocus_Eq_M,              /* FOCUS_EQ             */
     (svoWordU *const) rdvFocus_Low_Eq_M,          /* FOCUS_LOW_EQ         */
     (svoWordU *const) dvFocus_DC_Eq_M,            /* FOCUS_DC_EQ          */
     NULL,                                         /* FOCUS_HIGH_EQ        */
     (svoWordU *const) dvDisk_Fg_Eq_M,             /* DISK_FG_EQ           */
     (svoWordU *const) dvSled_Eq_M,                /* SLED_EQ              */
     NULL,                                         /* BEAM_LOW_EQ          */
     NULL,                                         /* BEAM_HIGH_EQ         */
     (svoWordU *const) rdvSpeed_Eq_M,              /* SPEED_EQ             */
     NULL,                                         /* ADJUST_EQ            */
    },

#if(ENABLE_DVD_4XCAV == 1)
/* X4 CAV */
    {(svoWordU *const) rdvTracking_Eq_L,           /* TRACKING_EQ          */
     (svoWordU *const) rdvTracking_Low_Eq_L,       /* TRACKING_LOW_EQ      */
     (svoWordU *const) dvTracking_DC_Eq_L,         /* TRACKING_DC_EQ       */
     NULL,                                         /* TRACKING_SHOCK_EQ    */
     (svoWordU *const) rdvFocus_Eq_L,              /* FOCUS_EQ             */
     (svoWordU *const) rdvFocus_Low_Eq_L,          /* FOCUS_LOW_EQ         */
     (svoWordU *const) dvFocus_DC_Eq_L,            /* FOCUS_DC_EQ          */
     NULL,                                         /* FOCUS_HIGH_EQ        */
     (svoWordU *const) dvDisk_Fg_Eq_L,             /* DISK_FG_EQ           */
     (svoWordU *const) dvSled_Eq_M,                /* SLED_EQ              */
     NULL,                                         /* BEAM_LOW_EQ          */
     NULL,                                         /* BEAM_HIGH_EQ         */
     (svoWordU *const) rdvSpeed_Eq_L,              /* SPEED_EQ             */
     NULL,                                         /* ADJUST_EQ            */
    },
#endif

/* X6 CAV */
    {(svoWordU *const) rdvTracking_Eq_M,           /* TRACKING_EQ          */
     (svoWordU *const) rdvTracking_Low_Eq_M,       /* TRACKING_LOW_EQ      */
     (svoWordU *const) dvTracking_DC_Eq_M,         /* TRACKING_DC_EQ       */
     NULL,                                         /* TRACKING_SHOCK_EQ    */
     (svoWordU *const) rdvFocus_Eq_M,              /* FOCUS_EQ             */
     (svoWordU *const) rdvFocus_Low_Eq_M,          /* FOCUS_LOW_EQ         */
     (svoWordU *const) dvFocus_DC_Eq_M,            /* FOCUS_DC_EQ          */
     NULL,                                         /* FOCUS_HIGH_EQ        */
     (svoWordU *const) dvDisk_Fg_Eq_M,             /* DISK_FG_EQ           */
     (svoWordU *const) dvSled_Eq_M,                /* SLED_EQ              */
     NULL,                                         /* BEAM_LOW_EQ          */
     NULL,                                         /* BEAM_HIGH_EQ         */
     (svoWordU *const) rdvSpeed_Eq_M,              /* SPEED_EQ             */
     NULL,                                         /* ADJUST_EQ            */
    },

/* X8 CAV */
    {(svoWordU *const) rdvTracking_Eq_M,           /* TRACKING_EQ          */
     (svoWordU *const) rdvTracking_Low_Eq_M,       /* TRACKING_LOW_EQ      */
     (svoWordU *const) dvTracking_DC_Eq_M,         /* TRACKING_DC_EQ       */
     NULL,                                         /* TRACKING_SHOCK_EQ    */
     (svoWordU *const) rdvFocus_Eq_M,              /* FOCUS_EQ             */
     (svoWordU *const) rdvFocus_Low_Eq_M,          /* FOCUS_LOW_EQ         */
     (svoWordU *const) dvFocus_DC_Eq_M,            /* FOCUS_DC_EQ          */
     NULL,                                         /* FOCUS_HIGH_EQ        */
     (svoWordU *const) dvDisk_Fg_Eq_M,             /* DISK_FG_EQ           */
     (svoWordU *const) dvSled_Eq_H,                /* SLED_EQ              */
     NULL,                                         /* BEAM_LOW_EQ          */
     NULL,                                         /* BEAM_HIGH_EQ         */
     (svoWordU *const) rdvSpeed_Eq_M,              /* SPEED_EQ             */
     NULL,                                         /* ADJUST_EQ            */
    },

/* X10 CAV */
    {(svoWordU *const) rdvTracking_Eq_H,           /* TRACKING_EQ          */
     (svoWordU *const) rdvTracking_Low_Eq_H,       /* TRACKING_LOW_EQ      */
     (svoWordU *const) dvTracking_DC_Eq_H,         /* TRACKING_DC_EQ       */
     NULL,                                         /* TRACKING_SHOCK_EQ    */
     (svoWordU *const) rdvFocus_Eq_H,              /* FOCUS_EQ             */
     (svoWordU *const) rdvFocus_Low_Eq_H,          /* FOCUS_LOW_EQ         */
     (svoWordU *const) dvFocus_DC_Eq_H,            /* FOCUS_DC_EQ          */
     NULL,                                         /* FOCUS_HIGH_EQ        */
     (svoWordU *const) dvDisk_Fg_Eq_H,             /* DISK_FG_EQ           */
     (svoWordU *const) dvSled_Eq_H,                /* SLED_EQ              */
     NULL,                                         /* BEAM_LOW_EQ          */
     NULL,                                         /* BEAM_HIGH_EQ         */
     (svoWordU *const) rdvSpeed_Eq_H,              /* SPEED_EQ             */
     NULL,                                         /* ADJUST_EQ            */
    },

/* X12 CAV */
    {(svoWordU *const) rdvTracking_Eq_H,           /* TRACKING_EQ          */
     (svoWordU *const) rdvTracking_Low_Eq_H,       /* TRACKING_LOW_EQ      */
     (svoWordU *const) dvTracking_DC_Eq_H,         /* TRACKING_DC_EQ       */
     NULL,                                         /* TRACKING_SHOCK_EQ    */
     (svoWordU *const) rdvFocus_Eq_H,              /* FOCUS_EQ             */
     (svoWordU *const) rdvFocus_Low_Eq_H,          /* FOCUS_LOW_EQ         */
     (svoWordU *const) dvFocus_DC_Eq_H,            /* FOCUS_DC_EQ          */
     NULL,                                         /* FOCUS_HIGH_EQ        */
     (svoWordU *const) dvDisk_Fg_Eq_H,             /* DISK_FG_EQ           */
     (svoWordU *const) dvSled_Eq_H,                /* SLED_EQ              */
     NULL,                                         /* BEAM_LOW_EQ          */
     NULL,                                         /* BEAM_HIGH_EQ         */
     (svoWordU *const) dvSpeed_Eq_H,               /* SPEED_EQ             */
     NULL,                                         /* ADJUST_EQ            */
    },

/* X16 CAV */
    {(svoWordU *const) rdvTracking_Eq_H,           /* TRACKING_EQ          */
     (svoWordU *const) rdvTracking_Low_Eq_H,       /* TRACKING_LOW_EQ      */
     (svoWordU *const) dvTracking_DC_Eq_H,         /* TRACKING_DC_EQ       */
     NULL,                                         /* TRACKING_SHOCK_EQ    */
     (svoWordU *const) rdvFocus_Eq_H,              /* FOCUS_EQ             */
     (svoWordU *const) rdvFocus_Low_Eq_H,          /* FOCUS_LOW_EQ         */
     (svoWordU *const) dvFocus_DC_Eq_H,            /* FOCUS_DC_EQ          */
     NULL,                                         /* FOCUS_HIGH_EQ        */
     (svoWordU *const) dvDisk_Fg_Eq_H,             /* DISK_FG_EQ           */
     (svoWordU *const) dvSled_Eq_H,                /* SLED_EQ              */
     NULL,                                         /* BEAM_LOW_EQ          */
     NULL,                                         /* BEAM_HIGH_EQ         */
     (svoWordU *const) rdvSpeed_Eq_H,              /* SPEED_EQ             */
     NULL,                                         /* ADJUST_EQ            */
    },

/* X20 CAV */
    {(svoWordU *const) rdvTracking_Eq_UH,          /* TRACKING_EQ          */
     (svoWordU *const) rdvTracking_Low_Eq_UH,      /* TRACKING_LOW_EQ      */
     (svoWordU *const) dvTracking_DC_Eq_UH,        /* TRACKING_DC_EQ       */
     NULL,                                         /* TRACKING_SHOCK_EQ    */
     (svoWordU *const) rdvFocus_Eq_UH,             /* FOCUS_EQ             */
     (svoWordU *const) rdvFocus_Low_Eq_UH,         /* FOCUS_LOW_EQ         */
     (svoWordU *const) dvFocus_DC_Eq_UH,           /* FOCUS_DC_EQ          */
     NULL,                                         /* FOCUS_HIGH_EQ        */
     (svoWordU *const) dvDisk_Fg_Eq_UH,            /* DISK_FG_EQ           */
     (svoWordU *const) dvSled_Eq_UH,               /* SLED_EQ              */
     NULL,                                         /* BEAM_LOW_EQ          */
     NULL,                                         /* BEAM_HIGH_EQ         */
     (svoWordU *const) rdvSpeed_Eq_UH,             /* SPEED_EQ             */
     NULL,                                         /* ADJUST_EQ            */
    }
};

#if (FINE_SEEK_SURF_TELFG == 0)
const svoWordU rdvFine_CE_Eq_UL[] = {rdvFINE_CE_EQ_UL };
const svoWordU rdvFine_CE_Eq_L[]  = {rdvFINE_CE_EQ_L  };
const svoWordU rdvFine_CE_Eq_M[]  = {rdvFINE_CE_EQ_M  };
const svoWordU rdvFine_CE_Eq_H[]  = {rdvFINE_CE_EQ_H  };
const svoWordU rdvFine_CE_Eq_UH[] = {rdvFINE_CE_EQ_UH };

const svoWordU (*const rdvFineCEEQCoefTbl[MAX_SPEED_DVD]) =
{
    (svoWordU *const)rdvFine_CE_Eq_L,   //_2LX_DVD
    #if(DVD2p4XCLV == 1)
    (svoWordU *const)rdvFine_CE_Eq_L,   //_2.4LX_DVD
    #else
    (svoWordU *const)rdvFine_CE_Eq_M,   //_3LX_DVD
    #endif
    (svoWordU *const)rdvFine_CE_Eq_M,   //_4LX_DVD
    #if(ENABLE_DVD_4XCAV == 1)
    (svoWordU *const)rdvFine_CE_Eq_L,   //_4AX_DVD
    #endif
    (svoWordU *const)rdvFine_CE_Eq_M,   //_6AX_DVD
    (svoWordU *const)rdvFine_CE_Eq_M,   //_8AX_DVD
    (svoWordU *const)rdvFine_CE_Eq_H,   //_10AX_DVD
    (svoWordU *const)rdvFine_CE_Eq_H,   //_12AX_DVD
    (svoWordU *const)rdvFine_CE_Eq_H,   //_16AX_DVD
    (svoWordU *const)rdvFine_CE_Eq_UH   //_20AX_DVD
};
#endif

/*------------------------------------------------------------------------*/
/* The end of file                                                        */
/*------------------------------------------------------------------------*/
