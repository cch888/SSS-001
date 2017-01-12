/****************************************************************************
*           (c) Copyright 2001 - 2010 Sunext Technology Co., Ltd.
*                           All Right Reserved
*
*
* FILENAME: svo_com.h
*
* DESCRIPTION
*
*
*
* NOTES:
*
*
* $Revision: 140 $
* $Date: 11-05-03 15:31 $
*
*
**************************** SUNEXT CONFIDENTIAL ****************************/

#ifndef _SVO_COM_
#define _SVO_COM_

#define NG      0
#define ERR     0
#define OK      !NG

#define NOP     0xFFFFFFFFL

#define UP          1
#define DOWN        0

#define RETRY1ST    1
#define NORETRY     0

#define PP_MEAS     2
#define SBADGN_MEAS 4
#define BSGN_MEAS   8
#define RRFGN_MEAS  9

#define SEEK_BUSY                       0
#define SEEK_OK                         1
#define SEEK_TIMEOUT                    2
#define SEEK_FDROP                      3
#define SEEK_POOR_CONTROL       4

#define LAYERMASK   0x01

#define PSNMask     0x00FFFFFF
#define PSNMaskBD   0x01FFFFFF

#if (BCA_ENABLE == 1)
#define BCA_J_MASK  0x40 //indicate a BCA jump
#endif
#define LIMIT_MASK  0x20 //indicate the jump distance is limited by HW
#define SLIP_MASK   0x10 //indicate the Seek is slip from target
#define L_J_MASK    0x08 //indicate a layer jump
#define R_J_MASK    0x04 //indicate a rough search
#define F_J_MASK    0x02 //indicate a fine search
#define DIR_MASK    0x01 //indicate jump direction

#define FORW        0
#define BACK        1

#define CLOSE       0
#define OPEN        !CLOSE

#define PLUS        0
#define MINUS       1

#define TRK_OFF     0              /* Indicate that the tracking is off */
#define TRK_ON      1              /* Indicate that the tracking is on */
#define HDR_DET     2              /* Indicate that the header detection */

#define BASE_TIME   1000                /* 1ms */
#define BASE_COUNT  (1000/BASE_TIME)

#define SEC_1       ((  1*BASE_COUNT)*1000)
#define SEC_2       ((  2*BASE_COUNT)*1000)
#define SEC_3       ((  3*BASE_COUNT)*1000)
#define SEC_4       ((  4*BASE_COUNT)*1000)
#define SEC_5       ((  5*BASE_COUNT)*1000)
#define SEC_6       ((  6*BASE_COUNT)*1000)
#define SEC_9       ((  9*BASE_COUNT)*1000)
#define SEC_10      (( 10*BASE_COUNT)*1000)
#define SEC_15      (( 15*BASE_COUNT)*1000)
#define SEC_30      (( 30*BASE_COUNT)*1000)
#define SEC_60      (( 60*BASE_COUNT)*1000)
#define MS_1        ((  1*BASE_COUNT)     )
#define MS_2        ((  2*BASE_COUNT)     )
#define MS_3        ((  3*BASE_COUNT)     )
#define MS_4        ((  4*BASE_COUNT)     )
#define MS_5        ((  5*BASE_COUNT)     )
#define MS_6        ((  6*BASE_COUNT)     )
#define MS_7        ((  7*BASE_COUNT)     )
#define MS_8        ((  8*BASE_COUNT)     )
#define MS_9        ((  9*BASE_COUNT)     )
#define MS_10       (( 10*BASE_COUNT)     )
#define MS_11       (( 11*BASE_COUNT)     )
#define MS_12       (( 12*BASE_COUNT)     )
#define MS_13       (( 13*BASE_COUNT)     )
#define MS_14       (( 14*BASE_COUNT)     )
#define MS_15       (( 15*BASE_COUNT)     )
#define MS_16       (( 16*BASE_COUNT)     )
#define MS_17       (( 17*BASE_COUNT)     )
#define MS_18       (( 18*BASE_COUNT)     )
#define MS_19       (( 19*BASE_COUNT)     )
#define MS_20       (( 20*BASE_COUNT)     )
#define MS_21       (( 21*BASE_COUNT)     )
#define MS_22       (( 22*BASE_COUNT)     )
#define MS_23       (( 23*BASE_COUNT)     )
#define MS_24       (( 24*BASE_COUNT)     )
#define MS_25       (( 25*BASE_COUNT)     )
#define MS_30       (( 30*BASE_COUNT)     )
#define MS_32       (( 32*BASE_COUNT)     )
#define MS_35       (( 35*BASE_COUNT)     )
#define MS_40       (( 40*BASE_COUNT)     )
#define MS_45       (( 45*BASE_COUNT)     )
#define MS_50       (( 50*BASE_COUNT)     )
#define MS_55       (( 55*BASE_COUNT)     )
#define MS_60       (( 60*BASE_COUNT)     )
#define MS_65       (( 65*BASE_COUNT)     )
#define MS_70       (( 70*BASE_COUNT)     )
#define MS_75       (( 75*BASE_COUNT)     )
#define MS_80       (( 80*BASE_COUNT)     )
#define MS_85       (( 85*BASE_COUNT)     )
#define MS_90       (( 90*BASE_COUNT)     )
#define MS_92       (( 92*BASE_COUNT)     )
#define MS_95       (( 95*BASE_COUNT)     )
#define MS_100      ((100*BASE_COUNT)     )
#define MS_110      ((110*BASE_COUNT)     )
#define MS_120      ((120*BASE_COUNT)     )
#define MS_150      ((150*BASE_COUNT)     )
#define MS_160      ((160*BASE_COUNT)     )
#define MS_180      ((180*BASE_COUNT)     )
#define MS_184      ((184*BASE_COUNT)     )
#define MS_200      ((200*BASE_COUNT)     )
#define MS_205      ((205*BASE_COUNT)     )
#define MS_210      ((210*BASE_COUNT)     )
#define MS_215      ((215*BASE_COUNT)     )
#define MS_220      ((220*BASE_COUNT)     )
#define MS_225      ((225*BASE_COUNT)     )
#define MS_230      ((230*BASE_COUNT)     )
#define MS_240      ((240*BASE_COUNT)     )
#define MS_250      ((250*BASE_COUNT)     )
#define MS_255      ((255*BASE_COUNT)     )
#define MS_260      ((260*BASE_COUNT)     )
#define MS_300      ((300*BASE_COUNT)     )
#define MS_350      ((350*BASE_COUNT)     )
#define MS_400      ((400*BASE_COUNT)     )
#define MS_500      ((500*BASE_COUNT)     )
#define MS_700      ((700*BASE_COUNT)     )
#define MS_800      ((800*BASE_COUNT)     )
#define MS_1000     ((1000*BASE_COUNT)    )
#define MS_1300     ((1300*BASE_COUNT)    )
#define TIMER_STOP  (0xFFFF)

/*---------------------------------------------------------------------------*/

/* Servo Eq */
#define TRACKING_EQ         0
#define TRACKING_LOW_EQ     1
#define TRACKING_DC_EQ      2
#define TRACKING_SHOCK_EQ   3
#define FOCUS_EQ            4
#define FOCUS_LOW_EQ        5
#define FOCUS_DC_EQ         6
#define FOCUS_HIGH_EQ       7
#define DISK_FG_EQ          8
#define SLED_EQ             9
#define BEAM_LOW_EQ         10
#define BEAM_HIGH_EQ        11
#define SPEED_EQ            12
#define ADJUST_EQ           13
#if 0 //not use
#define DISK_DEC_EQ         14
#define DISK_WBL_EQ         15
#endif
#define EQ_TBL_MAX          13

#define TRN_NOP             0x0FE0
#define SEQ_ILL             0x001F

#define SEQ_MASK            0x001F
#define SEQ_001             0x0000
#define SEQ_002             0x0001
#define SEQ_003             0x0002
#define SEQ_004             0x0003
#define SEQ_005             0x0004
#define SEQ_006             0x0005
#define SEQ_007             0x0006
#define SEQ_008             0x0007
#define SEQ_009             0x0008
#define SEQ_010             0x0009
#define SEQ_011             0x000A
#define SEQ_012             0x000B
#define SEQ_013             0x000C
#define SEQ_014             0x000D
#define SEQ_015             0x000E
#define SEQ_016             0x000F
#define SEQ_017             0x0010
#define SEQ_018             0x0011
#define SEQ_019             0x0012
#define SEQ_020             0x0013
#define SEQ_021             0x0014
#define SEQ_022             0x0015
#define SEQ_023             0x0016
#define SEQ_024             0x0017
#define SEQ_025             0x0018
#define SEQ_026             0x0019
#define SEQ_027             0x001A
#define SEQ_028             0x001B
#define SEQ_029             0x001C
#define SEQ_030             0x001D
#define SEQ_END             0x001E

#define TRN_MASK            0x0FE0
#define TRN_001             0x0000
#define TRN_002             0x0020
#define TRN_003             0x0040
#define TRN_004             0x0060
#define TRN_005             0x0080
#define TRN_006             0x00A0
#define TRN_007             0x00C0
#define TRN_008             0x00E0
#define TRN_009             0x0100
#define TRN_010             0x0120
#define TRN_011             0x0140
#define TRN_012             0x0160
#define TRN_013             0x0180
#define TRN_014             0x01A0
#define TRN_015             0x01C0
#define TRN_016             0x01E0
#define TRN_017             0x0200
#define TRN_018             0x0220
#define TRN_019             0x0240
#define TRN_020             0x0260
#define TRN_021             0x0280
#define TRN_022             0x02A0
#define TRN_023             0x02C0
#define TRN_024             0x02E0
#define TRN_025             0x0300
#define TRN_026             0x0320
#define TRN_027             0x0340
#define TRN_028             0x0360
#define TRN_029             0x0380
#define TRN_030             0x03A0
#define TRN_031             0x03C0
#define TRN_032             0x03E0
#define TRN_033             0x0400
#define TRN_034             0x0420
#define TRN_035             0x0440
#define TRN_036             0x0460
#define TRN_037             0x0480
#define TRN_038             0x04A0
#define TRN_039             0x04C0
#define TRN_040             0x04E0
#define TRN_041             0x0500
#define TRN_042             0x0520
#define TRN_043             0x0540
#define TRN_044             0x0560
#define TRN_045             0x0580
#define TRN_046             0x05A0
#define TRN_047             0x05C0
#define TRN_048             0x05E0
#define TRN_049             0x0600
#define TRN_050             0x0620
#define TRN_051             0x0640
#define TRN_052             0x0660
#define TRN_053             0x0680
#define TRN_054             0x06A0
#define TRN_055             0x06C0
#define TRN_056             0x06E0
#define TRN_057             0x0700
#define TRN_058             0x0720
#define TRN_059             0x0740
#define TRN_060             0x0760
#define TRN_061             0x0780
#define TRN_062             0x07A0
#define TRN_063             0x07C0
#define TRN_064             0x07E0
#define TRN_065             0x0800

/* ABS_TRN is flag in seq_tbl entries to indicate ABSOLUTE TRN_XXX ACCESS. Otherwise relative in same TRN_XXX */
#define ABS_TRN             0x8000

#define STAT_MASK           0xF000
#define STAT_NORMAL         0x0000
#define ERR_TRN             0x1000
#define FOCUS_DROP          0x2000

#define FOK_CHK_TRN         TRN_030
#define TRK_ON_TRN          TRN_045
#define READY_TRN           TRN_055
#define MAX_TRN             ((TRN_065>>5)+1)

#define SVO_INIT            TRN_001
#define SVO_RESTART         TRN_029
#define SVO_FOUSRCH         TRN_013
#define SVO_PICHOME         TRN_019
#define SVO_FE_PP           TRN_020
#define SVO_TGAFE           TRN_034
#define SVO_RECOVER         TRN_037
#define SVO_TRON            TRN_038
    #if (CALIBRATION_MANAGEMENT == 1)
#define SVO_RECALIB                     TRN_043
    #endif
#define SVO_VCALC           TRN_048
#define SVO_SEEK            TRN_057
#define SVO_PLAY            TRN_059
#define SVO_PAUSEINI        TRN_063
#define SVO_PAUSE           TRN_064
#define SVO_JUMP            TRN_060

#define CDDVD_DISCID        TRN_007
#define WAIT_CMD_DECODE     TRN_016

//disc kind/type detection/check end flag --> discChkDoneBits
#define stm_chk_end         0x01
#define disc_chk_end        0x02
#define blank_chk_end       0x04
#define ecc_chk_end         0x08
#define velo_chk_end        0x10
#define plus_minus_chk_end  0x20
#define dvdram_v10_chk_end  0x20
#define start_end           0x40
#if ((DVD_UNBALANCE_DISC_CHECK==1) || (CD_UNBALANCE_DISC_CHECK == 1) || (BD_UNBALANCE_DISC_CHECK == 1))
#define unb_chk_end         0x80
#endif
#if (ENABLE_CBHD == 1)
#define cbhd_disc_chk_end   0x?0
#endif

//adjust end flag --> calDoneBits
#define vref_end            0x01
#define boundary_end        0x02
#define L0_blank_det        0x04
#define L1_blank_det        0x08
#define tilt_cal_end        0x10
#define febc_cal_end        0x20

//gain adjust end flag --> calGainDoneBits
#define pdic_end            0x0001         /* OPU pdic gain calibration */
#define feg_bsg_end         0x0002         /* AFE FE BS gain calibration */
#define tebg_end            0x0004         /* AFE TEBG calibration */
#define teg_end             0x0008         /* AFE TE gain calibration */
#define felg_end            0x0010         /* DSP FE loop gain calibration */
#define telg_end            0x0020         /* DSP TE loop gain calibration */
#define msppbg_end          0x0040         /* AFE MPPBG SPPBG calibration */
#define ceg_end             0x0080         /* AFE CE GnOs calibration */
#define rpg_end             0x0100         /* AFE RP GnOs calibration */
#define dif_RF_end          0x0200         /* AFE (TRK_ON) diff-RF calibration */
#define dif_WOB_end         0x0400         /* AFE (TRK_ON) diff-WOB calibration */
#define febg_end            0x0800         /* AFE FEOBG & SPP2BG calibration */
#define cebg_end            0x1000         /* AFE CEBG calibration */
#define rpbg_end            0x2000         /* AFE RPBG calibration */

/* offset adjust end flag --> calOfsDoneBits */
#define havcof_end          0x0001
#define mbeamof_end         0x0002          /* Mainbeam offset adjustment */
#define sbeamof_end         0x0004          /* Subbeam offset adjustment */
#define feof_end            0x0008          /* Focus error offset adjustment */
#define teof_end            0x0010          /* Tracking error offset adjustment */
#define bsof_end            0x0020          /* Beamstrength error offset adjustment */
#define ofaj_end            0x0040
#define rpof_end            0x0080
#define ceof_end            0x0100
#define phbhof_end          0x0200
#define rfof_end            0x0400
#define troof_end           0x0800

//Servo Offline Calibration end flag --> CalDoneBits
#define cd_fbdac_end                    0x00000001  /* CD Focus Balance DAC Calibration */
#define cd_febg_end                     0x00000002  /* CD Focus Balance Gain Calibration  */
#define cd_betaoffset_end               0x00000004  /* CD Beta Offset Calibration  */
#define dv_fbdac0_end                   0x00000008  /* DVD Layer 0 Focus Balance DAC Calibration */
#define dv_febg0_end                    0x00000010  /* DVD Layer 0 Focus Balance Gain Calibration  */
#define dv_fbdac1_end                   0x00000020  /* DVD Layer 1 Focus Balance DAC Calibration */
#define dv_febg1_end                    0x00000040  /* DVD Layer 1 Focus Balance Gain Calibration  */
#define dv_plus_r_betaoffset_end        0x00000080  /* DVD+R Beta Offset Calibration  */
#define dv_tiltdac_end                  0x00000100  /* DVD Tilt DAC Calibration */
#define dv_minus_r_betaoffset_end       0x00000200  /* DVD-R Beta Offset Calibration  */
#if (ENABLE_HDDVD==1)
#define hd_fbdac0_end                   0x00000400  /* HD Layer 0 Focus Balance DAC Calibration */
#define hd_febg0_end                    0x00000800  /* HD Layer 0 Focus Balance Gain Calibration  */
#define hd_fbdac1_end                   0x00001000  /* HD Layer 1 Focus Balance DAC Calibration */
#define hd_febg1_end                    0x00002000  /* HD Layer 1 Focus Balance Gain Calibration  */
#define hd_r_betaoffset_end             0x00004000  /* HD-R Beta Offset Calibration  */
#define hd_tiltdac_end                  0x00008000  /* HD Tilt DAC Calibration */
#endif
#if (BD_ENABLE==1)
#define bd_fbdac0_end                   0x00020000  /* BD Layer 0 Focus Balance DAC Calibration */
#define bd_febg0_end                    0x00040000  /* BD Layer 0 Focus Balance Gain Calibration  */
#define bd_fbdac1_end                   0x00080000  /* BD Layer 1 Focus Balance DAC Calibration */
#define bd_febg1_end                    0x00100000  /* BD Layer 1 Focus Balance Gain Calibration  */
#define bd_r_betaoffset_end             0x00200000  /* BD-R Beta Offset Calibration  */
#define bd_tiltdac_end                  0x00400000  /* BD Tilt DAC Calibration */
#endif
#if(USE_EEPROM_COLLI_POS == 0)
#define bd_barcode_end                  0x01000000  /* BD barcode */
#else //(USE_EEPROM_COLLI_POS == 0)
#define bd_rom_colli_l0_end			    0x01000000  /* BDROM Layer 0 collimator calibration */
#define bd_rom_colli_l1_end				0x02000000  /* BDROM Layer 1 collimator calibration */
#define bd_rre_colli_l0_end			    0x04000000  /* BDR/RE Layer 0 collimator calibration */
#define bd_rre_colli_l1_end			    0x08000000  /* BDR/RE Layer 1 collimator calibration */
#define bd_barcode_end                  0x10000000  /* BD barcode */
#define bd_coli_end_mask (bd_rom_colli_l0_end | bd_rom_colli_l1_end | bd_rre_colli_l0_end | bd_rre_colli_l1_end)
#endif //(USE_EEPROM_COLLI_POS == 0)


//Power Offline Calibration end flag --> CalPowerDoneBits
#define cd_kpower_end                   0x00000001  /* CD Power Calibration */
#define cd_bias_value_end               0x00000002  /* CD xmW --> cooling power Offset Dac */
#define cd_vwdc2vrdc_end                0x00000004
#define cd_vwdc2vwdc1_end               0x00000008
#define dv_kpower_end                   0x00000010  /* DVD Power Calibration */
#define dv_bias_value_end               0x00000020  /* DVD 1.5mW --> cooling power Offset Dac */
#define dv_vwdc2vrdc_end                0x00000040
#define dv_vwdc2vwdc1_end               0x00000080
#define dv_vwdc2vwdc2_end               0x00000100
#if ENABLE_HDDVD==1
#define hd_kpower_end                   0x00000200  /* HD Power Calibration */
#define hd_bias_value_end               0x00000400  /* HD power --> cooling power Offset Dac */
#define hd_vwdc2vrdc_end                0x00000800
#define hd_vwdc2vwdc1_end               0x00001000
#define hd_vwdc2vwdc2_end               0x00002000
#endif
#if BD_ENABLE
#define bd_kpower_end                   0x00004000  /* BD Power Calibration */
#define bd_bias_value_end               0x00008000  /* BD read power --> cooling power Offset Dac */
#define bd_vwdc2vrdc_end                0x00010000
#define bd_vwdc2vwdc1_end               0x00020000
#define bd_vwdc2vwdc2_end               0x00040000
#endif

/* spin mode */
#define DSTARTING           0
#define DSTARTED            1
#define DSTOPPING           2
#define DSTOPPED            3

/* Lead-in Time */
#define LEAD_IN_MINUTE      94
#define LEAD_IN_SECOND      00
#define LEAD_IN_FRAME       00

#define EFM_SYNC_FREQ_CD_1X    7350
#define WOBBLE_FREQ_CD_1X      22050
#define EFM_SYNC_FREQ_DVD_1X   8790
#define WOBBLE_FREQ_DVDpRW_1X  817383
#define WOBBLE_FREQ_DVDmRW_1X  140625
#define WOBBLE_FREQ_DVDRAM_1X  156881
#define EFM_SYNC_FREQ_BD_1X    17081
#define EFM_SYNC_FREQ_HD_1X    29032
#define EFM_SYNC_FREQ_HDRAM_1X 30149
#define WOBBLE_FREQ_BD_1X      956522
#define WOBBLE_FREQ_HD_1X      696774

#define TE_ALL              (0)
#define RP_ALL              (1)
#define FE_ALL              (2)
#define BS_ALL              (3)

/* previous state */
#define NORMAL              0
#define RECOVER             1


/* Subq/Atip Interruput DI/EI Flag */
#define SUBQ_EI_DI_SERVO    0x10


/* Servo moniter system */
typedef struct seq_arg_tbl{
    USHORT      arg[3];
}SEQ_ARG_TBL;


typedef struct seq_tbl
{
    USHORT      (*func)(void);
    SEQ_ARG_TBL seq_arg;
    USHORT      next_states_no[5];
}SEQ_TBL;


typedef struct trn_tbl
{
    USHORT      next_trn_no;
    SEQ_TBL     *seq;
}TRN_TBL;


#define SIZE_ERR_LOG (10)

//-----------------------------------------------/
// define enum used for focus shock detection    /
//-----------------------------------------------/
typedef enum eFocusShockAverage
{
    FOCUS_SHOCK_AVE_ENABLE = 50,
    FOCUS_SHOCK_AVE_DISABLE
} eFocusShockAverage4Smp_t;


typedef enum eFocusShockWidth
{
    FOCUS_SHOCK_WIDTH_1400US = 60,
    FOCUS_SHOCK_WIDTH_700US
} eFocusShockWidth_t;


typedef enum eFocusShockWindow
{
    FOCUS_SHOCK_WINDOW_RESET_PEAK = 70,
    FOCUS_SHOCK_WINDOW_HOLD_PEAK
} eFocusShockWindow_t;


typedef enum eFocusShockInterrupt
{
    FOCUS_SHOCK_ENABLE_INTERRUPT = 80,
    FOCUS_SHOCK_DISABLE_CLEAR_INTERRUPT
} eFocusShockInterrupt_t;


//-----------------------------------------------/
// define enum used for error logging            /
//-----------------------------------------------/
typedef enum eLogErrorCode
{
    ERR_NO                      = 0,
    ERR_FOCUS_SHOCK_WIN_INPUT   = 1,
    ERR_FOCUS_SHOCK_WIDTH_INPUT = 2,
    ERR_FOCUS_SHOCK_AVE4_INPUT  = 3,
    ERR_FOCUS_SHOCK_IRQ_INPUT   = 4
} eLogErrorCode_t;

#endif /* _SVO_COM_ */

/*------------------------------------------------------------------------*/
/* The end of file                                                        */
/*------------------------------------------------------------------------*/
