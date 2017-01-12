/****************************************************************************
*                (c) Copyright 2001 - 2008 SUNEXT DESIGN, INC.
*                           All Rights Reserved
*
*
* FILENAME:   dvRamSVO_TBL.C
*
* DESCRIPTION
*
*
*
* NOTES:
*
*
* $Revision: 9 $
* $Date: 11/03/11 2:41p $
*
**************************** SUNEXT CONFIDENTIAL ****************************/

#include ".\common\globals.h"
#include ".\common\globtype.h"
#include ".\common\chip.h"
#include ".\servo\svo.h"
#include ".\servo\svo_com.h"
#include ".\servo\svo_tbl.h"
#include ".\servo\dvRamSvo_tbl.h"
#include ".\AL\REG_servo.h"
#include ".\AL\REG_write_strategy.h"
#include ".\AL\REG_global.h"
#include ".\AL\REG_periph.h"
#include ".\al\REG_dvd_decoder.h"
#include ".\AL\REG_dvd_encoder.h"

/*----------------------------------------------------------------------------*/

#define DMCNT_DVD   (0x48|(DFRQ_FG<<2)|(DFRQ_WBL_DEC*0x03))
#define SPINCTL_DVD (DISK_AFCG_DVD << 1)

#define DFCTB_4X                        0x3E          /* 0x2E2 *//* Defect Level = 803mV,Defect Delay Time = 8.5us */
#define DFCTB_8X                        0x3E          /* 0x2E2 *//* Defect Level = 803mV,Defect Delay Time = 8.5us */
#define DFCTB_10X                       0x40
#define DFCTB_12X                       0x40
#define DFCTB_16LX                      0x40
#define DFCTB_24LX                      0x40
#define DFCTB_32LX                      0x40
#define DFCTB_40LX                      0x40
#define DFCTB_48LX                      0x40

#define DFCTB_16X                       0x40
#define DFCTB_24X                       0x40
#define DFCTB_40X                       0x40

/*
 * Servo initial command data table
 */
svoRegInitTblT const dvRamSvoRegInitNONETbl[] =
{
    {HAL_SRV_SYNCIEN_ADR,         0x0000,       WORDSET}, /* 0xB00003E0[12:0] disable Interrupt = SEEKC */

    /* Servo, HW stepping */
    {HAL_SRV_TIMPER_ADR,          0x0064,       BYTESET}, /* 0xB0000024[7:0] Set HW stepping timer = 100us */
    {HAL_SRV_HWSTPSEEK_ADR,       0x0000,       BYTESET}, /* 0xB0000028[7:0] Set DAC value from stepper motor block */
    {HAL_SRV_SEEKSTART_ADR,       0x0010,       BYTESET}, /* 0xB0000040[7:0] Set Full step use square wave */
    {HAL_SRV_HYSLTHLD_ADR,        0x0003,       BYTESET}, /* 0xB00000B0[7:0] SLED DAC value low threshold value */
    {HAL_SRV_HYSHTHLD_ADR,        0x0003,       BYTESET}, /* 0xB00000B4[7:0] SLED DAC value high threshold value */

    /* Servo */
    {HAL_SRV_FGAT_ADR,            0x0000,       BYTESET}, /* 0xB0000344[7:0] Stop focus adjustment */
    {HAL_SRV_TGAT_ADR,            0x0000,       BYTESET}, /* 0xB000034C[7:0] Stop tracking adjustment */
    {HAL_SRV_MSKON_ADR,           0x0051|(DISK_FR2X<<HAL_SRV_FR2X_LSB),BYTESET}, /* 0xB000036C[7:0] double FG,invert FG, use FG noise mask, TR/FE loop gain excitation freq = 2.75K */
#if (FOOTILT_DIFFERENTIAL == 1)
    {HAL_SRV_FOODIFEN_ADR,        0x0010,       BYTESET}, /* 0xB0000118[7:0] SERVO DAC Switch */
#else
    {HAL_SRV_FOODIFEN_ADR,        0x0000,       BYTESET}, /* 0xB0000118[7:0] SERVO DAC Switch */
#endif
    {HAL_SRV_FETHR_ADR,           0x007F,       BYTESET}, /* 0xB000011C[7:0] FE threshold value */
    {HAL_SRV_FETHREN_ADR,HAL_SRV_FETHREN,       BIT_ON }, /* 0xB0000120[7:0] FE threshold enable */
    {HAL_SRV_AFCRNG1_ADR,         0x0020,       BYTESET}, /* 0xB0000138[6:0] AFC in range low */
    {HAL_SRV_AFCRNG2_ADR,         0x0038,       BYTESET}, /* 0xB000013C[6:0] AFC in range high */
    {HAL_SRV_SRVMOD_ADR,          0x0019,       BYTESET}, /* 0xB0000140[7:0] REGTON=0,DPLLMSK=0,DECCHG=010,SRVMOD=0 */
    {HAL_SRV_AFCG_ADR,       SPINCTL_DVD,       BYTESET}, /* 0xB00001DC[7:0] AFCG */
    {HAL_SRV_FGCMP_ADR,           0x013D,       WORDSET}, /* 0xB0000148[15:0] 33.8688e6/64/(2*23.14*36) = 635 */
    {HAL_SRV_FGRNG_ADR,           0x0020,       WORDSET}, /* 0xB00001F8[15:0] FG count range */
    {HAL_SRV_LANDPOL_ADR,HAL_SRV_LANDPOL,       BIT_OFF}, /* 0xB0000398[1] */
    {HAL_SRV_LANDXC_ADR,  HAL_SRV_LANDXC,       BIT_OFF}, /* 0xB0000398[0] */
    {HAL_SRV_DFRQFG_ADR,       DMCNT_DVD,       BYTESET}, /* 0xB00003AC[7:0] disc motor control register */
    {HAL_SRV_APCGAIN_ADR,         0x0035,       BYTESET}, /* 0xB0000174[7:0] rwAPCGAIN=0x15 */
    {HAL_SRV_APCNUM_ADR,          0x0001,       BYTESET}, /* 0xB0000178[3:0] APCNUM=1 */
    {HAL_SRV_WAITWBL_ADR,         0x000F,       BYTESET}, /* 0xB0000170[3:0] wobble sync lock time */
    {HAL_SRV_TSVSTP_ADR,          0x0000,       BYTESET}, /* 0xB000020C.7    stop PICUP register */
    {HAL_SRV_LDOFF_ADR,           0x0000,       BYTESET}, /* 0xB000021C[7:0] LDON = 0 */
    {HAL_SRV_PC_ADR,              0x0000,       BYTESET}, /* 0xB0000234[7:0] PAM duty cycle 100% */
    {HAL_SRV_PF_ADR,              0x0000,       BYTESET}, /* 0xB0000238[7:0] PAM frequency 172.2Hz */
    {HAL_SRV_DAC2SEL_ADR,         0x0020,       BYTESET}, /* 0xB000029C[7:0] output as DACDAT2/DACDAT1 */
    {HAL_SRV_FBAVS_ADR,           0x00C0,       BYTESET}, /* 0xB0000350[7:0] FBAVS,FBAVS4 = 88Hz,FBASE = 1b,FQWDSEL,FQWDSEL4 = 2.5ms */
    {HAL_SRV_TROFFSET_ADR,        0x0000,       BYTESET}, /* 0xB0000370[7:0] */
#if (CHIP_REV <= 0xB3)
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
    {HAL_SRV_AFEDFCT_ADR, HAL_SRV_AFEDFCT,       BIT_ON}, /* 0xB00003B0[7:0] defect select afe */
#else
    {HAL_SRV_DEEPDFCT_ADR, HAL_SRV_DEEPDFCT,       BIT_ON}, /* 0xB00003B0[7:0] defect select afe */
#endif
    {HAL_SRV_ZTK_ADR,             0x0000,       BYTESET}, /* 0xB00003B4[7:0] ZTK = 0b, PGTK = 0b, PLF = 0b, RXP = 0b, TXP = 0b */
    {HAL_SRV_DVDRHHLD_ADR,        0x0001,       BYTESET}, /* 0xB00003B8.0 DVD-RAM TE/FE hold during Header */
    {HAL_SRV_SYNCI_ADR,           0x0001,       WORDSET}, /* 0xB00003E8[12:0] Clear Interrput Status except the last bit */
    {HAL_SRV_DBT_ADR,             0x0003,       BYTESET}, /* 0xB0000334[6:0] Defect Delay Time = 8.4us */

#if (CHIP_REV <= 0xB3)
    //Set HW kick/brake parameters
    {HAL_SRV_ZCEAREN_ADR,         0x003D,       BYTESET}, /* 0xB0000058[7:0] EARZCPROT *///20090430_t.29 ==start== make use of HW layer jump kick and brake timeout protection
    {HAL_SRV_BRAKELVL_ADR,        0x0000,       BYTESET}, /* 0xB000005C[7:0] BRAKELVL */
    {HAL_SRV_KICKLVL_ADR,         0x0070,       BYTESET}, /* 0xB0000060[7:0] KICKLVL */
    {HAL_SRV_BRAKEHLD_ADR,        0x0000,       BYTESET}, /* 0xB0000068[7:0] BRAKEHLD */
    {HAL_SRV_BRAKEEN_ADR,         0x0060,       BYTESET}, /* 0xB000006C[7:0] BRAKECNT */
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

    /* Peripheral page */
    {HAL_PERIPH_ADCGAIN_ADR,     ADCGAIN,       BYTESET}, /* 0x100005F4.0 */

    /* DVD-DECODER */
    {HAL_DVDD_LGRSTDIS_ADR,HAL_DVDD_LGRSTDIS,    BIT_ON}, /* 0x10000340[2] */
    {HAL_DVDD_LANDOUT_ADR,HAL_DVDD_LANDOUT,     BIT_OFF}, /* 0x10000368[7] */
    {HAL_DVDD_MANLAND_ADR,HAL_DVDD_MANLAND,      BIT_ON}, /* 0x10000368[6] */

    {SVOREGTBL_END,               0x0000,       BYTESET}  /* end code */
}; /* dvRamSvoRegInitNONETbl[] */

svoRegInitTblT const dvRamSvoRegInit1LXTbl[] =
{
    /* SERVO*/
    {HAL_SRV_APCGAIN_ADR,        0x0035,       BYTESET}, /* 0xB0000174[6:0] AFCINRGEN on, APCEN on, APCGAIN=1/32*/
    {HAL_SRV_DBT_ADR,            0x0000,       BYTESET}, /* 0xB0000334[6:0] Defect Delay Time */
#if (CHIP_REV <= 0xB3)
    {HAL_SRV_FVD_ADR,            0x0018,       BYTESET}, /* 0xB0000398[7:0] KWIDSL = 0us,FVD = 5.52KHz */
#endif

    {SVOREGTBL_END,              0x0000,       BYTESET}    /* end code */
}; /* dvRamSvoRegInit1LXTbl[] */

svoRegInitTblT const dvRamSvoRegInit2LXTbl[] =
{
    /* SERVO*/
    {HAL_SRV_APCGAIN_ADR,        0x0035,       BYTESET}, /* 0xB0000174[6:0] AFCINRGEN on, APCEN on, APCGAIN=1*/
    {HAL_SRV_DBT_ADR,            0x0000,       BYTESET}, /* 0xB0000334[6:0] Defect Delay Time */
#if (CHIP_REV <= 0xB3)
    {HAL_SRV_FVD_ADR,            0x0018,       BYTESET}, /* 0xB0000398[7:0] KWIDSL = 0us,FVD = 5.52KHz */
#endif

    {SVOREGTBL_END,              0x0000,       BYTESET}  /* end code */
}; /* dvRamSvoRegInit2LXTbl[] */

svoRegInitTblT const dvRamSvoRegInit3LXTbl[] =
{
    /* SERVO*/
    {HAL_SRV_APCGAIN_ADR,        0x0035,       BYTESET}, /* 0xB0000174[6:0] AFCINRGEN on, APCEN on, APCGAIN=1*/
    {HAL_SRV_DBT_ADR,            0x0000,       BYTESET}, /* 0xB0000334[6:0] Defect Delay Time */
#if (CHIP_REV <= 0xB3)
    {HAL_SRV_FVD_ADR,            0x0018,       BYTESET}, /* 0xB0000398[7:0] KWIDSL = 0us,FVD = 5.52KHz */
#endif

    {SVOREGTBL_END,              0x0000,       BYTESET}  /* end code */
}; /* dvRamSvoRegInit3LXTbl[] */

svoRegInitTblT const dvRamSvoRegInit5LXTbl[] =
{
    /* SERVO*/
    {HAL_SRV_APCGAIN_ADR,        0x0036,       BYTESET}, /* 0xB0000174[6:0] AFCINRGEN on, APCEN on, APCGAIN=1*/
    {HAL_SRV_DBT_ADR,            0x0003,       BYTESET}, /* 0xB0000334[6:0] Defect Delay Time = 8.4us */
#if (CHIP_REV <= 0xB3)
    {HAL_SRV_FVD_ADR,            0x0018,       BYTESET}, /* 0xB0000398[7:0] KWIDSL = 0us,FVD = 5.52KHz */
#endif

    {SVOREGTBL_END,              0x0000,       BYTESET}  /* end code */
}; /* dvRamSvoRegInit5LXTbl[] */

svoRegInitTblT const dvRamSvoRegInit5AXTbl[] =
{
    /* SERVO*/
    {HAL_SRV_APCGAIN_ADR,        0x0000,       BYTESET}, /* 0xB0000174[6:0] AFCINRGEN off, APCEN off, APCGAIN=1*/
    {HAL_SRV_DBT_ADR,            0x0000,       BYTESET}, /* 0xB0000334[6:0] Defect Delay Time */
#if (CHIP_REV <= 0xB3)
    {HAL_SRV_FVD_ADR,            0x0018,       BYTESET}, /* 0xB0000398[7:0] KWIDSL = 0us,FVD = 5.52KHz */
#endif

    {SVOREGTBL_END,              0x0000,       BYTESET}  /* end code */
}; /* dvRamSvoRegInit5XTbl[] */

svoRegInitTblT const dvRamSvoRegInit8AXTbl[] =
{
    /* SERVO*/
    {HAL_SRV_APCGAIN_ADR,        0x0000,       BYTESET}, /* 0xB0000174[6:0] AFCINRGEN off, APCEN off, APCGAIN=1*/
    {HAL_SRV_DBT_ADR,            0x0003,       BYTESET}, /* 0xB0000334[6:0] Defect Delay Time = 8.4us */
#if (CHIP_REV <= 0xB3)
    {HAL_SRV_FVD_ADR,            0x0018,       BYTESET}, /* 0xB0000398[7:0] KWIDSL = 0us,FVD = 5.52KHz */
#endif

    {SVOREGTBL_END,              0x0000,       BYTESET}  /* end code */
}; /* dvRamSvoRegInit12XTbl[] */

svoRegInitTblT const dvRamSvoRegInit12AXTbl[] =
{
    /* SERVO*/
    {HAL_SRV_APCGAIN_ADR,        0x0000,       BYTESET}, /* 0xB0000174[6:0] AFCINRGEN off, APCEN off, APCGAIN=1*/
    {HAL_SRV_DBT_ADR,            0x0003,       BYTESET}, /* 0xB0000334[6:0] Defect Delay Time = 8.4us */
#if (CHIP_REV <= 0xB3)
    {HAL_SRV_FVD_ADR,            0x0018,       BYTESET}, /* 0xB0000398[7:0] KWIDSL = 0us,FVD = 5.52KHz */
#endif

    {SVOREGTBL_END,              0x0000,       BYTESET}  /* end code */
}; /* dvRamSvoRegInit12XTbl[] */

svoRegInitTblT const dvRamSvoRegInit16AXTbl[] =
{
    /* SERVO*/
    {HAL_SRV_APCGAIN_ADR,        0x0000,       BYTESET}, /* 0xB0000174[6:0] AFCINRGEN off, APCEN off, APCGAIN=1*/
    {HAL_SRV_DBT_ADR,            0x0003,       BYTESET}, /* 0xB0000334[6:0] Defect Delay Time = 8.4us */
#if (CHIP_REV <= 0xB3)
    {HAL_SRV_FVD_ADR,            0x0028,       BYTESET}, /* 0xB0000398[7:0] KWIDSL = 0us,FVD = 8.4KHz */
#endif

    {SVOREGTBL_END,              0x0000,       BYTESET}  /* end code */
}; /* dvRamSvoRegInit16XTbl[] */

const svoRegInitTblT *const dvRamSvoRegInitTbl[MAX_SPEED_DVDRAM+1] = {
    (svoRegInitTblT *const) dvRamSvoRegInitNONETbl  ,   // NONE
    (svoRegInitTblT *const) dvRamSvoRegInit1LXTbl   ,   // _1Lx
    (svoRegInitTblT *const) dvRamSvoRegInit2LXTbl   ,   // _2Lx
    (svoRegInitTblT *const) dvRamSvoRegInit3LXTbl   ,   // _3Lx
    (svoRegInitTblT *const) dvRamSvoRegInit5LXTbl   ,   // _5Lx
    (svoRegInitTblT *const) dvRamSvoRegInit5AXTbl   ,   // _5Ax
    (svoRegInitTblT *const) dvRamSvoRegInit8AXTbl   ,   // _8Ax
    (svoRegInitTblT *const) dvRamSvoRegInit12AXTbl  ,   // _12Ax
    (svoRegInitTblT *const) dvRamSvoRegInit16AXTbl  ,   // _16Ax
};

//**************************************************************************************************
// FOR RAM DISC
//**************************************************************************************************
/* Coefficients shared with DVD-ROM */
extern const svoWordU dvTracking_Shock_Eq_Init[];
extern const svoWordU dvFocus_High_Eq_Init[];
extern const svoWordU dvBeam_Low_Eq_Init[];
extern const svoWordU dvBeam_High_Eq_Init[];
extern const svoWordU dvAdjust_Eq_Init[];

extern const svoWordU dvTracking_DC_Eq_UL[];
extern const svoWordU dvTracking_DC_Eq_L[];
extern const svoWordU dvTracking_DC_Eq_M[];
extern const svoWordU dvTracking_DC_Eq_H[];
extern const svoWordU dvTracking_DC_Eq_UH[];

extern const svoWordU dvFocus_DC_Eq_UL[];
extern const svoWordU dvFocus_DC_Eq_L[];
extern const svoWordU dvFocus_DC_Eq_M[];
extern const svoWordU dvFocus_DC_Eq_H[];
extern const svoWordU dvFocus_DC_Eq_UH[];

extern const svoWordU dvDisk_Fg_Eq_UL[];
extern const svoWordU dvDisk_Fg_Eq_L[];
extern const svoWordU dvDisk_Fg_Eq_M[];
extern const svoWordU dvDisk_Fg_Eq_H[];
extern const svoWordU dvDisk_Fg_Eq_UH[];

/* 1X CLV */
const svoWordU dvRamTracking_Eq_UL[]      = {dvRamTRACKING_EQ_UL    };
const svoWordU dvRamTracking_Low_Eq_UL[]  = {dvRamTRACKING_LOW_EQ_UL};
const svoWordU dvRamFocus_Eq_UL[]         = {dvRamFOCUS_EQ_UL       };
const svoWordU dvRamFocus_Low_Eq_UL[]     = {dvRamFOCUS_LOW_EQ_UL   };
const svoWordU dvRamSled_Eq_UL[]          = {dvSLED_EQ_UL           };
const svoWordU dvRamSpeed_Eq_UL[]         = {dvRamSPEED_EQ_UL       };

/* 2X CLV */
const svoWordU dvRamTracking_Eq_L[]       = {dvRamTRACKING_EQ_L     };
const svoWordU dvRamTracking_Low_Eq_L[]   = {dvRamTRACKING_LOW_EQ_L };
const svoWordU dvRamFocus_Eq_L[]          = {dvRamFOCUS_EQ_L        };
const svoWordU dvRamFocus_Low_Eq_L[]      = {dvRamFOCUS_LOW_EQ_L    };
const svoWordU dvRamSled_Eq_L[]           = {dvRamSLED_EQ_L         };
const svoWordU dvRamSpeed_Eq_L[]          = {dvRamSPEED_EQ_L        };

/* 3X CLV, 5X CLV and 5X CAV */
const svoWordU dvRamTracking_Eq_M[]       = {dvRamTRACKING_EQ_M     };
const svoWordU dvRamTracking_Low_Eq_M[]   = {dvRamTRACKING_LOW_EQ_M };
const svoWordU dvRamFocus_Eq_M[]          = {dvRamFOCUS_EQ_M        };
const svoWordU dvRamFocus_Low_Eq_M[]      = {dvRamFOCUS_LOW_EQ_M    };
const svoWordU dvRamSled_Eq_M[]           = {dvRamSLED_EQ_M         };
const svoWordU dvRamSpeed_Eq_M[]          = {dvRamSPEED_EQ_M        };

/* 8X CAV and 12X CAV */
const svoWordU dvRamTracking_Eq_H[]       = {dvRamTRACKING_EQ_H     };
const svoWordU dvRamTracking_Low_Eq_H[]   = {dvRamTRACKING_LOW_EQ_H };
const svoWordU dvRamFocus_Eq_H[]          = {dvRamFOCUS_EQ_H        };
const svoWordU dvRamFocus_Low_Eq_H[]      = {dvRamFOCUS_LOW_EQ_H    };
const svoWordU dvRamSled_Eq_H[]           = {dvRamSLED_EQ_H         };
const svoWordU dvRamSpeed_Eq_H[]          = {dvRamSPEED_EQ_H        };

/* 16X CAV */
const svoWordU dvRamTracking_Eq_UH[]      = {dvRamTRACKING_EQ_UH    };
const svoWordU dvRamTracking_Low_Eq_UH[]  = {dvRamTRACKING_LOW_EQ_UH};
const svoWordU dvRamFocus_Eq_UH[]         = {dvRamFOCUS_EQ_UH       };
const svoWordU dvRamFocus_Low_Eq_UH[]     = {dvRamFOCUS_LOW_EQ_UH   };
const svoWordU dvRamSled_Eq_UH[]          = {dvRamSLED_EQ_UH        };
const svoWordU dvRamSpeed_Eq_UH[]         = {dvRamSPEED_EQ_UH       };


/* Init */
#define     dvRamSvoEQCoefTbl_Init \
    {NULL,                                         /* TRACKING_EQ          */  \
     NULL,                                         /* TRACKING_LOW_EQ      */  \
     NULL,                                         /* TRACKING_DC_EQ       */  \
     (svoWordU *const) dvTracking_Shock_Eq_Init,   /* TRACKING_SHOCK_EQ    */  \
     NULL,                                         /* FOCUS_EQ             */  \
     NULL,                                         /* FOCUS_LOW_EQ         */  \
     NULL,                                         /* FOCUS_DC_EQ          */  \
     (svoWordU *const) dvFocus_High_Eq_Init,       /* FOCUS_HIGH_EQ        */  \
     NULL,                                         /* DISK_FG_EQ           */  \
     NULL,                                         /* SLED_EQ              */  \
     (svoWordU *const) dvBeam_Low_Eq_Init,         /* BEAM_LOW_EQ          */  \
     (svoWordU *const) dvBeam_High_Eq_Init,        /* BEAM_HIGH_EQ         */  \
     NULL,                                         /* SPEED_EQ             */  \
     (svoWordU *const) dvAdjust_Eq_Init            /* ADJUST_EQ            */  \
    }

/* 1X CLV */
#define     dvRamSvoEQCoefTbl_UL \
    {(svoWordU *const) dvRamTracking_Eq_UL,        /* TRACKING_EQ          */  \
     (svoWordU *const) dvRamTracking_Low_Eq_UL,    /* TRACKING_LOW_EQ      */  \
     (svoWordU *const) dvTracking_DC_Eq_UL,        /* TRACKING_DC_EQ       */  \
     NULL,                                         /* TRACKING_SHOCK_EQ    */  \
     (svoWordU *const) dvRamFocus_Eq_UL,           /* FOCUS_EQ             */  \
     (svoWordU *const) dvRamFocus_Low_Eq_UL,       /* FOCUS_LOW_EQ         */  \
     (svoWordU *const) dvFocus_DC_Eq_UL,           /* FOCUS_DC_EQ          */  \
     NULL,                                         /* FOCUS_HIGH_EQ        */  \
     (svoWordU *const) dvDisk_Fg_Eq_UL,            /* DISK_FG_EQ           */  \
     (svoWordU *const) dvRamSled_Eq_UL,            /* SLED_EQ              */  \
     NULL,                                         /* BEAM_LOW_EQ          */  \
     NULL,                                         /* BEAM_HIGH_EQ         */  \
     (svoWordU *const) dvRamSpeed_Eq_UL,           /* SPEED_EQ             */  \
     NULL                                          /* ADJUST_EQ            */  \
    }

/* 2X CLV */
#define     dvRamSvoEQCoefTbl_L \
    {(svoWordU *const) dvRamTracking_Eq_L,         /* TRACKING_EQ          */  \
     (svoWordU *const) dvRamTracking_Low_Eq_L,     /* TRACKING_LOW_EQ      */  \
     (svoWordU *const) dvTracking_DC_Eq_L,         /* TRACKING_DC_EQ       */  \
     NULL,                                         /* TRACKING_SHOCK_EQ    */  \
     (svoWordU *const) dvRamFocus_Eq_L,            /* FOCUS_EQ             */  \
     (svoWordU *const) dvRamFocus_Low_Eq_L,        /* FOCUS_LOW_EQ         */  \
     (svoWordU *const) dvFocus_DC_Eq_L,            /* FOCUS_DC_EQ          */  \
     NULL,                                         /* FOCUS_HIGH_EQ        */  \
     (svoWordU *const) dvDisk_Fg_Eq_L,             /* DISK_FG_EQ           */  \
     (svoWordU *const) dvRamSled_Eq_L,             /* SLED_EQ              */  \
     NULL,                                         /* BEAM_LOW_EQ          */  \
     NULL,                                         /* BEAM_HIGH_EQ         */  \
     (svoWordU *const) dvRamSpeed_Eq_L,            /* SPEED_EQ             */  \
     NULL                                          /* ADJUST_EQ            */  \
    }

/* 3X CLV, 5X CLV and 5X CAV */
#define     dvRamSvoEQCoefTbl_M \
    {(svoWordU *const) dvRamTracking_Eq_M,         /* TRACKING_EQ          */  \
     (svoWordU *const) dvRamTracking_Low_Eq_M,     /* TRACKING_LOW_EQ      */  \
     (svoWordU *const) dvTracking_DC_Eq_M,         /* TRACKING_DC_EQ       */  \
     NULL,                                         /* TRACKING_SHOCK_EQ    */  \
     (svoWordU *const) dvRamFocus_Eq_M,            /* FOCUS_EQ             */  \
     (svoWordU *const) dvRamFocus_Low_Eq_M,        /* FOCUS_LOW_EQ         */  \
     (svoWordU *const) dvFocus_DC_Eq_M,            /* FOCUS_DC_EQ          */  \
     NULL,                                         /* FOCUS_HIGH_EQ        */  \
     (svoWordU *const) dvDisk_Fg_Eq_M,             /* DISK_FG_EQ           */  \
     (svoWordU *const) dvRamSled_Eq_M,             /* SLED_EQ              */  \
     NULL,                                         /* BEAM_LOW_EQ          */  \
     NULL,                                         /* BEAM_HIGH_EQ         */  \
     (svoWordU *const) dvRamSpeed_Eq_M,            /* SPEED_EQ             */  \
     NULL                                          /* ADJUST_EQ            */  \
    }


/* 8X CAV and 12X CAV */
#define     dvRamSvoEQCoefTbl_H \
    {(svoWordU *const) dvRamTracking_Eq_H,         /* TRACKING_EQ          */  \
     (svoWordU *const) dvRamTracking_Low_Eq_H,     /* TRACKING_LOW_EQ      */  \
     (svoWordU *const) dvTracking_DC_Eq_H,         /* TRACKING_DC_EQ       */  \
     NULL,                                         /* TRACKING_SHOCK_EQ    */  \
     (svoWordU *const) dvRamFocus_Eq_H,            /* FOCUS_EQ             */  \
     (svoWordU *const) dvRamFocus_Low_Eq_H,        /* FOCUS_LOW_EQ         */  \
     (svoWordU *const) dvFocus_DC_Eq_H,            /* FOCUS_DC_EQ          */  \
     NULL,                                         /* FOCUS_HIGH_EQ        */  \
     (svoWordU *const) dvDisk_Fg_Eq_H,             /* DISK_FG_EQ           */  \
     (svoWordU *const) dvRamSled_Eq_H,             /* SLED_EQ              */  \
     NULL,                                         /* BEAM_LOW_EQ          */  \
     NULL,                                         /* BEAM_HIGH_EQ         */  \
     (svoWordU *const) dvRamSpeed_Eq_H,            /* SPEED_EQ             */  \
     NULL                                          /* ADJUST_EQ            */  \
    }

/* 16X CAV */
#define     dvRamSvoEQCoefTbl_UH \
    {(svoWordU *const) dvRamTracking_Eq_UH,        /* TRACKING_EQ          */  \
     (svoWordU *const) dvRamTracking_Low_Eq_UH,    /* TRACKING_LOW_EQ      */  \
     (svoWordU *const) dvTracking_DC_Eq_UH,        /* TRACKING_DC_EQ       */  \
     NULL,                                         /* TRACKING_SHOCK_EQ    */  \
     (svoWordU *const) dvRamFocus_Eq_UH,           /* FOCUS_EQ             */  \
     (svoWordU *const) dvRamFocus_Low_Eq_UH,       /* FOCUS_LOW_EQ         */  \
     (svoWordU *const) dvFocus_DC_Eq_UH,           /* FOCUS_DC_EQ          */  \
     NULL,                                         /* FOCUS_HIGH_EQ        */  \
     (svoWordU *const) dvDisk_Fg_Eq_UH,            /* DISK_FG_EQ           */  \
     (svoWordU *const) dvRamSled_Eq_UH,            /* SLED_EQ              */  \
     NULL,                                         /* BEAM_LOW_EQ          */  \
     NULL,                                         /* BEAM_HIGH_EQ         */  \
     (svoWordU *const) dvRamSpeed_Eq_UH,           /* SPEED_EQ             */  \
     NULL                                          /* ADJUST_EQ            */  \
    }

/*----------------------------------------------------------------------------*/
/* Fine search KICKLVL coefficient                                            */
/*----------------------------------------------------------------------------*/
SEEK_GAIN const dvRamSvoKICKLVL_UL_F[] = {dvRamKICKLVL_EQ_UL_F};
SEEK_GAIN const dvRamSvoKICKLVL_L_F[]  = {dvRamKICKLVL_EQ_L_F};
SEEK_GAIN const dvRamSvoKICKLVL_M_F[]  = {dvRamKICKLVL_EQ_M_F};
SEEK_GAIN const dvRamSvoKICKLVL_H_F[]  = {dvRamKICKLVL_EQ_H_F};
SEEK_GAIN const dvRamSvoKICKLVL_UH_F[] = {dvRamKICKLVL_EQ_UH_F};

const SEEK_GAIN *const dvRamSvoKICKLVLTbl[MAX_SPEED_DVDRAM]={
    (SEEK_GAIN *const) dvRamSvoKICKLVL_UL_F,   // 1XCLV
    (SEEK_GAIN *const) dvRamSvoKICKLVL_L_F,    // 2XCLV
    (SEEK_GAIN *const) dvRamSvoKICKLVL_M_F,    // 3XCLV
    (SEEK_GAIN *const) dvRamSvoKICKLVL_M_F,    // 5XCLV
    (SEEK_GAIN *const) dvRamSvoKICKLVL_M_F,    // 5XCAV
    (SEEK_GAIN *const) dvRamSvoKICKLVL_H_F,    // 8XCAV
    (SEEK_GAIN *const) dvRamSvoKICKLVL_H_F,    // 12XCAV
    (SEEK_GAIN *const) dvRamSvoKICKLVL_UH_F,   // 16XCAV
};

/*----------------------------------------------------------------------------*/
/* Fine search BRAKELVL coefficient                                           */
/*----------------------------------------------------------------------------*/
SEEK_GAIN const dvRamSvoBRAKELVL_UL_F[] = {dvRamBRAKELVL_EQ_UL_F};
SEEK_GAIN const dvRamSvoBRAKELVL_L_F[]  = {dvRamBRAKELVL_EQ_L_F};
SEEK_GAIN const dvRamSvoBRAKELVL_M_F[]  = {dvRamBRAKELVL_EQ_M_F};
SEEK_GAIN const dvRamSvoBRAKELVL_H_F[]  = {dvRamBRAKELVL_EQ_H_F};
SEEK_GAIN const dvRamSvoBRAKELVL_UH_F[] = {dvRamBRAKELVL_EQ_UH_F};

const SEEK_GAIN *const dvRamSvoBRAKELVLTbl[MAX_SPEED_DVDRAM]={
    (SEEK_GAIN *const) dvRamSvoBRAKELVL_UL_F,   // 1XCLV
    (SEEK_GAIN *const) dvRamSvoBRAKELVL_L_F,    // 2XCLV
    (SEEK_GAIN *const) dvRamSvoBRAKELVL_M_F,    // 3XCLV
    (SEEK_GAIN *const) dvRamSvoBRAKELVL_M_F,    // 5XCLV
    (SEEK_GAIN *const) dvRamSvoBRAKELVL_M_F,    // 5XCAV
    (SEEK_GAIN *const) dvRamSvoBRAKELVL_H_F,    // 8XCAV
    (SEEK_GAIN *const) dvRamSvoBRAKELVL_H_F,    // 12XCAV
    (SEEK_GAIN *const) dvRamSvoBRAKELVL_UH_F,   // 16XCAV
};

/*----------------------------------------------------------------------------*/
/* Fine search BRAKETIME coefficient                                          */
/*----------------------------------------------------------------------------*/
const BYTE dvRamSvoBRAKETIMETbl[MAX_SPEED_DVDRAM]={
    dvRamBRAKETIME_EQ_UL   ,   // 1XCLV
    dvRamBRAKETIME_EQ_L    ,   // 2XCLV
    dvRamBRAKETIME_EQ_M    ,   // 3XCLV
    dvRamBRAKETIME_EQ_M    ,   // 5XCLV
    dvRamBRAKETIME_EQ_M    ,   // 5XCAV
    dvRamBRAKETIME_EQ_H    ,   // 8XCAV
    dvRamBRAKETIME_EQ_H    ,   // 12XCAV
    dvRamBRAKETIME_EQ_UH   ,   // 16XCAV
};

/*----------------------------------------------------------------------------*/
/* Fine search BRAKESPD coefficient                                           */
/*----------------------------------------------------------------------------*/
const WORD dvRamSvoBRAKESPDTbl[MAX_SPEED_DVDRAM]={
    dvRamBRAKESPD_EQ_UL    ,   // 1XCLV
    dvRamBRAKESPD_EQ_L     ,   // 2XCLV
    dvRamBRAKESPD_EQ_M     ,   // 3XCLV
    dvRamBRAKESPD_EQ_M     ,   // 5XCLV
    dvRamBRAKESPD_EQ_M     ,   // 5XCAV
    dvRamBRAKESPD_EQ_H     ,   // 8XCAV
    dvRamBRAKESPD_EQ_H     ,   // 12XCAV
    dvRamBRAKESPD_EQ_UH    ,   // 16XCAV
};

/*----------------------------------------------------------------------------*/
/* Fine search kt1B(0x0B) coefficient                                         */
/*----------------------------------------------------------------------------*/
SEEK_GAIN const dvRamSvokt1BKick_UL_F[] = {dvRamKt1BKick_EQ_UL_F};
SEEK_GAIN const dvRamSvokt1BKick_L_F[]  = {dvRamKt1BKick_EQ_L_F};
SEEK_GAIN const dvRamSvokt1BKick_M_F[]  = {dvRamKt1BKick_EQ_M_F};
SEEK_GAIN const dvRamSvokt1BKick_H_F[]  = {dvRamKt1BKick_EQ_H_F};
SEEK_GAIN const dvRamSvokt1BKick_UH_F[] = {dvRamKt1BKick_EQ_UH_F};

const SEEK_GAIN *const dvRamSvokt1BKickTbl[MAX_SPEED_DVDRAM]={
    (SEEK_GAIN *const) dvRamSvokt1BKick_UL_F,   // 1XCLV
    (SEEK_GAIN *const) dvRamSvokt1BKick_L_F,    // 2XCLV
    (SEEK_GAIN *const) dvRamSvokt1BKick_M_F,    // 3XCLV
    (SEEK_GAIN *const) dvRamSvokt1BKick_M_F,    // 5XCLV
    (SEEK_GAIN *const) dvRamSvokt1BKick_M_F,    // 5XCAV
    (SEEK_GAIN *const) dvRamSvokt1BKick_H_F,    // 8XCAV
    (SEEK_GAIN *const) dvRamSvokt1BKick_H_F,    // 12XCAV
    (SEEK_GAIN *const) dvRamSvokt1BKick_UH_F,   // 16XCAV
};

#if (ACTIVE_SLED_FOR_FINE_JUMP == 0)
/*----------------------------------------------------------------------------*/
/* Fine search ks17(0x3E) coefficient                                         */
/*----------------------------------------------------------------------------*/
SEEK_GAIN const dvRamSvoks17Kick_UL_F_000[]  = {dvRamSvoks17Kick_UL_F};
SEEK_GAIN const dvRamSvoks17Kick_UL_B_000[]  = {dvRamSvoks17Kick_UL_B};
SEEK_GAIN const dvRamSvoks17Kick_L_F_000[]   = {dvRamSvoks17Kick_L_F};
SEEK_GAIN const dvRamSvoks17Kick_L_B_000[]   = {dvRamSvoks17Kick_L_B};
SEEK_GAIN const dvRamSvoks17Kick_M_F_000[]   = {dvRamSvoks17Kick_M_F};
SEEK_GAIN const dvRamSvoks17Kick_M_B_000[]   = {dvRamSvoks17Kick_M_B};
SEEK_GAIN const dvRamSvoks17Kick_H_F_000[]   = {dvRamSvoks17Kick_H_F};
SEEK_GAIN const dvRamSvoks17Kick_H_B_000[]   = {dvRamSvoks17Kick_H_B};
SEEK_GAIN const dvRamSvoks17Kick_UH_F_000[]  = {dvRamSvoks17Kick_UH_F};
SEEK_GAIN const dvRamSvoks17Kick_UH_B_000[]  = {dvRamSvoks17Kick_UH_B};

const SEEK_GAIN *const dvRamSvoks17KickTbl[2][MAX_SPEED_DVDRAM]={
    {(SEEK_GAIN *const) dvRamSvoks17Kick_UL_F_000,   // 1XCLV         FORWARD
     (SEEK_GAIN *const) dvRamSvoks17Kick_L_F_000,    // 2XCLV
     (SEEK_GAIN *const) dvRamSvoks17Kick_M_F_000,    // 3XCLV
     (SEEK_GAIN *const) dvRamSvoks17Kick_M_F_000,    // 5XCLV
     (SEEK_GAIN *const) dvRamSvoks17Kick_M_F_000,    // 5XCAV
     (SEEK_GAIN *const) dvRamSvoks17Kick_H_F_000,    // 8XCAV
     (SEEK_GAIN *const) dvRamSvoks17Kick_H_F_000,    // 12XCAV
     (SEEK_GAIN *const) dvRamSvoks17Kick_UH_F_000,   // 16XCAV
    },
    {(SEEK_GAIN *const) dvRamSvoks17Kick_UL_B_000,   // 1XCLV        BACKWARD
     (SEEK_GAIN *const) dvRamSvoks17Kick_L_B_000,    // 2XCLV
     (SEEK_GAIN *const) dvRamSvoks17Kick_M_B_000,    // 3XCLV
     (SEEK_GAIN *const) dvRamSvoks17Kick_M_B_000,    // 5XCLV
     (SEEK_GAIN *const) dvRamSvoks17Kick_M_B_000,    // 5XCAV
     (SEEK_GAIN *const) dvRamSvoks17Kick_H_B_000,    // 8XCAV
     (SEEK_GAIN *const) dvRamSvoks17Kick_H_B_000,    // 12XCAV
     (SEEK_GAIN *const) dvRamSvoks17Kick_UH_B_000,   // 16XCAV
    }
};
#endif  //(ACTIVE_SLED_FOR_FINE_JUMP == 0)

#if (FINE_SEEK_SURF_TELFG == 0)
const svoWordU dvRamFine_CE_Eq_UL[] = {dvRamFINE_CE_EQ_UL };
const svoWordU dvRamFine_CE_Eq_L[]  = {dvRamFINE_CE_EQ_L  };
const svoWordU dvRamFine_CE_Eq_M[]  = {dvRamFINE_CE_EQ_M  };
const svoWordU dvRamFine_CE_Eq_H[]  = {dvRamFINE_CE_EQ_H  };
const svoWordU dvRamFine_CE_Eq_UH[] = {dvRamFINE_CE_EQ_UH };

const svoWordU (*const dvRamFineCEEQCoefTbl[MAX_SPEED_DVDRAM]) =
{
    (svoWordU *const)dvRamFine_CE_Eq_UL,  // 1XCLV
    (svoWordU *const)dvRamFine_CE_Eq_L,   // 2XCLV
    (svoWordU *const)dvRamFine_CE_Eq_M,   // 3XCLV
    (svoWordU *const)dvRamFine_CE_Eq_M,   // 5XCLV
    (svoWordU *const)dvRamFine_CE_Eq_M,   // 5XCAV
    (svoWordU *const)dvRamFine_CE_Eq_H,   // 8XCAV
    (svoWordU *const)dvRamFine_CE_Eq_H,   // 12XCAV
    (svoWordU *const)dvRamFine_CE_Eq_UH   // 16XCAV
};
#endif /* FINE_SEEK_SURF_TELFG == 0 */

const svoWordU dvRamRoughCEEQ[]  = {dvROUGH_CE_EQ_RAM};
const svoWordU dvRamRoughCELFG[] = {dvROUGH_CE_LOW_EQ_RAM};

const svoWordU *const dvRamSvoEQCoefTbl[MAX_SPEED_DVDRAM+1][EQ_TBL_MAX+1] =
{
    dvRamSvoEQCoefTbl_Init,   // Init
    dvRamSvoEQCoefTbl_UL,     // 1XCLV
    dvRamSvoEQCoefTbl_L,      // 2XCLV
    dvRamSvoEQCoefTbl_M,      // 3XCLV
    dvRamSvoEQCoefTbl_M,      // 5XCLV
    dvRamSvoEQCoefTbl_M,      // 5XCAV
    dvRamSvoEQCoefTbl_H,      // 8XCAV
    dvRamSvoEQCoefTbl_H,      // 12XCAV
    dvRamSvoEQCoefTbl_UH,     // 16XCAV
};

/*------------------------------------------------------------------------*/
/* The end of file                                                        */
/*------------------------------------------------------------------------*/
