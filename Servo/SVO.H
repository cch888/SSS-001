/****************************************************************************
*                (c) Copyright 2001 - 2008 SUNEXT DESIGN, INC.
*                           All Right Reserved
*
*
* $Revision: 543 $
* $Date: 11/03/29 9:41p $
*
* DESCRIPTION:  Provides visibility to typedefs, definitions, functions and variables
*               which are used between servo and other modules
*
*
**************************** SUNEXT CONFIDENTIAL ****************************/
#ifndef _SVO_
#define _SVO_

/*----------------------------------------------------------------------------*/
/* Structure and Definitions defined in servo but shared with non-servo code  */
/*----------------------------------------------------------------------------*/

//move cst to a module which contains all const of a disc ==> and a function to retrieve it
#define CD_WOBBLES_SECTOR        294
#define DVDp_WOBBLES_SECTOR      1209 // 1 ADIP frame = 52 ADIP units x 93 wobbles // 1 sector => divided by 4
#define DVDm_WOBBLES_SECTOR      208
#define DVDRAM_WOBBLES_SECTOR    232
#define BD_WOBBLES_SECTOR        434
#define HD_WOBBLES_SECTOR        312
#define HDRAM_WOBBLES_SECTOR     312

/******************* Spindle speed *******************/
/* Template
    _#LX - CLV
    _#AX - CAV
    _#PX - PCAV

*/
typedef enum
{
   /** CD Spindle speed defines **/
   NONE       =  0,
   _4LX       =  1,   /*  4xCLV   */
   _8LX       =  2,   /*  8xCLV   */
   _12LX      =  3,   /* 12x CLV  */
   _16LX      =  4,   /* 16x CLV  */
   _10AX      =  5,   /* 10x CAV  */
   #if (ENABLE_CD_12XCAV == 1)
   _12AX      =  6,   /* 12x CAV  */
   #else
   _16AX      =  6,   /* 16x CAV  */
   #endif
   _24AX      =  7,   /* 24x CAV  */
   _32AX      =  8,   /* 32x CAV  */
   _40AX      =  9,   /* 40x CAV  */
   _48AX      = 10,   /* 48x CAV  */
   _52AX      = 11,   /* 52x CAV  */


   /** DVD Spindle speed defines **/
   NONE_DVD   =  0,
   _2LX_DVD   =  1,          /* 2X CLV   */
   _3LX_DVD   =  2,          /* 3X CLV   */
   _4LX_DVD   =  3,          /* 4X CLV   */
   #if (ENABLE_DVD_4XCAV == 1)
   _4AX_DVD   =  4,          /* 4xCAV    */
   _6AX_DVD   =  5,          /* 6xCAV    */
   _8AX_DVD   =  6,          /* 8xCAV    */
   _10AX_DVD  =  7,          /* 10xCAV   */
   _12AX_DVD  =  8,          /* 12xCAV   */
   _16AX_DVD  =  9,          /* 16xCAV   */
   _20AX_DVD  = 10,          /* 20xCAV   */
   #else
   _6AX_DVD   =  4,          /* 6X CAV   */
   _8AX_DVD   =  5,          /* 8X CAV   */
   _10AX_DVD  =  6,          /* 10X CAV  */
   _12AX_DVD  =  7,          /* 12X CAV  */
   _16AX_DVD  =  8,          /* 16X CAV  */
   _20AX_DVD  =  9,          /* 20X CAV  */
   #endif

   /** DVDRAM Spindle speed defines **/
   NONE_DVDRAM   =  0,
   _1LX_DVDRAM   =  1,       /* DVDRAM_1xCLV  */
   _2LX_DVDRAM   =  2,       /* DVDRAM_2xCLV  */
   _3LX_DVDRAM   =  3,       /* DVDRAM_3xCLV  */
   _5LX_DVDRAM   =  4,       /* DVDRAM_5xCLV  */
   _5AX_DVDRAM   =  5,       /* DVDRAM_5xCAV */
   _8AX_DVDRAM   =  6,       /* DVDRAM_8xCAV */
   _12AX_DVDRAM  =  7,       /* DVDRAM_12xCAV*/
   _16AX_DVDRAM  =  8,       /* DVDRAM_16xCAV*/

   /** BD Spindle speed defines **/
#if(BD2p2XCLV == 1)
   NONE_BD       =  0,
   _1LX_BD       =  1,       /* 1X CLV */
   _2LX_BD       =  2,       /* 2X CLV */
   _3LX_BD       =  3,       /* 3X CLV */
   _4AX_BD       =  4,       /* 4xCAV  */
   _6AX_BD       =  5,       /* 6xCAV  */
   _8AX_BD       =  6,       /* 8xCAV  */
   _10AX_BD      =  7,       /* 10xCAV */
   _12AX_BD      =  8,       /* 12xCAV */
   _14AX_BD      =  9,       /* 14xCAV */
#else
   NONE_BD       =  0,
   _1LX_BD       =  1,       /* 1X CLV */
   _2LX_BD       =  2,       /* 2X CLV */
   _4AX_BD       =  3,       /* 4xCAV  */
   _6AX_BD       =  4,       /* 6xCAV  */
   _8AX_BD       =  5,       /* 8xCAV  */
   _10AX_BD      =  6,       /* 10xCAV */
   _12AX_BD      =  7,       /* 12xCAV */
   _14AX_BD      =  8,       /* 14xCAV */
#endif

   /** HD Spindle speed defines **/
   NONE_HD       =  0,
   _1LX_HD       =  1,       /* 1X CLV */
   _2LX_HD       =  2,       /* 2X CLV */
   _4AX_HD       =  3,       /* 4xCAV  */

   _UNKNOWN_SPEED = 0xFF
} ESpindleSpeed;

#define MAX_SPEED_CD            _52AX
#define MIN_CAV_SPEED_CD        _10AX
#define SPEEDTBL_CD             52
#define CLV_SPEED_CD            _16LX
#define SVO_CAL_SPEED_CD        _10AX         /* CD AJUST_SPEED */

#define MAX_SPEED_DVD           _20AX_DVD
#if (ENABLE_DVD_4XCAV == 1)
#define MIN_CAV_SPEED_DVD       _4AX_DVD
#else
#define MIN_CAV_SPEED_DVD       _6AX_DVD
#endif
#define SPEEDTBL_DVD            20
#define CLV_SPEED_DVD           _4LX_DVD
#define SVO_CAL_SPEED_DVD       _2LX_DVD     /* DVD AJUST_SPEED */

#define MAX_SPEED_DVDRAM        _16AX_DVDRAM
#define SPEEDTBL_DVDRAM         16
#define CLV_SPEED_DVDRAM        _5LX_DVDRAM
#define SVO_CAL_SPEED_DVDRAM    _2LX_DVDRAM  /* DVDRAM AJUST_SPEED */

#if (BD_ENABLE == 1)
#define MAX_SPEED_BD            _14AX_BD
#define SPEEDTBL_BD             14
#if(BD2p2XCLV == 1)
#define CLV_SPEED_BD            _3LX_BD
#else
#define CLV_SPEED_BD            _2LX_BD
#endif
#if BD_2X_STARTUP == 1
#define SVO_CAL_SPEED_BD        _2LX_BD      /* BD AJUST_SPEED  */
#else
#define SVO_CAL_SPEED_BD        _1LX_BD      /* BD AJUST_SPEED  */
#endif
#endif

#if (ENABLE_HDDVD == 1)
#define MAX_SPEED_HD            _4AX_HD
#define SPEEDTBL_HD             4
#define CLV_SPEED_HD            _2LX_HD
#define SVO_CAL_SPEED_HD        _1LX_HD      /* HD AJUST_SPEED  */
#endif


/* DSP Auto Adjust */
#define FGAJ            0
#define TGAJ            1

/*** Data Type Definitions */
typedef unsigned char ServoStatesT;
#define svoStInitialized        0
#define svoStIdleDiscKnown      1
#define svoStIdleDiscUnknown    2
#define svoStStartingServo      3
#define svoStStoppingServo      4
#define svoStTracking           5
#define svoStAccessing          6
#define svoStPausing            7
#define svoStTrayMovingIn       8
#define svoStTrayMovingOut      9
#define svoStEmergencystop      10
#define svoStServiceMode        11

typedef unsigned char ServoExecutionStateT;
#define svoReady    0
#define svoBusy     1
#define svoError    2

typedef unsigned char ServoCommandsT;
#define svoCmdIdle                      0xFF
#define svoCmdInit                      0x00
#define svoCmdTray                      0x01
#define svoCmdStart                     0x02
#define svoCmdStop                      0x03
#define svoCmdPause                     0x04
#define svoCmdSeek                      0x05
#define svoCmdJumpGrooves               0x06
#define svoCmdVeloAdjust                0x09
#define svoCmdReAdjust                  0x0A
#if (CALIBRATION_MANAGEMENT == 1)
#define svoCmdReCalibrate               0x0B
#endif /* CALIBRATION_MANAGEMENT */
#if (HORROR_DISC_IMPROVEMENT == 1)
#define svoCmdChgRFParameter            0x0C
#endif
#if CMD_DRY_READ == 1
#define svoCmdDryRead                   0x0D
#endif
#if ( P_DIAGS==1)
#define svoCmdForceDiscKind             0x0E
#endif
#define svoCmdAdjFocActDcActSens        0x0F
/* setting commands */
#define svoCmdSetSpeed                  0x11
#if(BCA_ENABLE == 1)
#define svoCmdBCASeek                   0x20
#endif
/* Inquiry commands */
#define svoCmdGetStatusInformation      0x23
#define svoCmdGetErrorInformation       0x24
#define svoCmdGetSubcodeInformation     0x26
#define svoCmdGetATIPInformation        0x27

/* Diagnostics commands */
#define svoCmdWLDAdjust                 0x30
#define svoCmdLDAdjust                  0x31
#define svoCmdDiagnostic                0x32
#if (SERVICE_MODE == 1)
#define svoCmdServiceMode               0x40
#define svoCmdLoopControl               0x0B
    #if CMD_DRY_READ == 1
#define svoCmdDryRead                   0x0D
    #endif
    #if TEST_AVERAGING_SERVO == 1
#define svoCmdTestAvgServo              0x0F
    #endif
    #if CMD_SVO_CMD_READ_GPADC == 1
#define svoCmdReadGpAdc                 0x0E
    #endif
#endif

typedef unsigned char SpeedT;
#define e1X             1
#define e2X             2
#define e4X             4
#define e8X             8
#define e10X            10
#define e12X            12
#define e16X            16
#define e24X            24
#define e28X            28
#define e32X            32
#define e40X            40
#define e48X            48

typedef enum
{
    TRK_METHOD_UNKNOWN  = 0,
    TRK_METHOD_MPP         ,
    TRK_METHOD_DPD         ,
    TRK_METHOD_DPP         ,
    TRK_METHOD_SPPO1       ,
} ETrackingMethod;

#if (BD_ENABLE)
typedef enum
{
    eRESET_PIC,
    eSET_PIC
} SetPicMode_t;
#endif //(BD_ENABLE)

#define EFMSTOP                     0
#define READMODE                    1
#define AFE_WRITE_CALIB             2
#define DVDR_PULSED_WS_WRITE        3
#define DVDRW_CDRW_WRITE            4
#define DVDRW_CDRW_DC_ERASE         5
#define DVDR_CASTLE_WS              6
#define DVDR_DL                     7
#define CDR_WRITE                   8
#define SIMULATION_WRITE            9
#define POWER_CALIB_MODE0           10
#define POWER_CALIB_MODE1           11
#define POWER_CALIB_MODE2           12
#define POWER_CALIB_MODE3           13
#define POWER_CALIB_MODE4           14
#define POWER_CALIB_MODE5           15

#define MAX_ATTSEL_AFE_WRITE_CALIB  0x1F
#define MIN_ATTSEL_AFE_WRITE_CALIB  0

#define EFM_UNKNOW_MODE             16
#define CDR_WRITE_2CH               17
#define CDRW_WRITE                  18
#define CDRW_DC_ERASE               19
#define DVDRAM_WRITE                20

#define SMP_ON                      0
#define SMP_OFF                     1
#define SMP_AUTO                    2
#define SMP_UNKNOW                  3
#define SMP_NONE                    4
#define SMP_UNCHANGE                5

#define WGATE_NORMAL        0
#define WGATE_FORCE_ON      1
#define WGATE_FORCE_OFF     2

#define FORCE_READ_WRITE_HOLD_MODE        0
#define FORCE_READ_WRITE_APC_MODE         1
#define READ_APC_WRITE_HOLD_MODE          2
#define READ_APC_WRITE_APC_HDDAC_MODE     3
#define READ_APC_WRITE_APC_MODE           4

#define WRITE_MP_HOLD_MODE      0
#define WRITE_MP_APC_MODE       1
#define WRITE_CAST_HOLD_MODE    2
#define WRITE_CAST_APC_MODE     3
#define WRITE_RW_HOLD_MODE      4
#define WRITE_RW_APC_ERASE_MODE 5
#define WRITE_RW_APC_WRITE_MODE 6

#define WRITE_DVDR_MP_MODE              0
#define WRITE_DVDR_CAST_MODE            1
#define WRITE_DVDRW_REAL_MODE           2
#define WRITE_DVDRW_TEST_MODE           3
#define WRITE_DVDR_MP_RHOLD_WAPC_MODE   4
#define WRITE_DVDR_MP_RAPC_WAPC_MODE    5
#define WRITE_DVDR_CAST_RHOLD_WAPC_MODE 6
#define WRITE_DVDR_CAST_RAPC_WAPC_MODE  7
#define WRITE_DVDR_TEST_MODE            8
#define WRITE_DVDRAM_MP_MODE            9
#define WRITE_CDR_MODE                 10
#define WRITE_CDRW_REAL_MODE           11
#define WRITE_CDRW_TEST_MODE           12

#define CD_POWER2VRDC_APC_DAC               0
#define CD_POWER2VRDC_HOLD_DAC              1
#define CD_POWER2VWDC_APC_DAC               2
#define CD_POWER2VWDC_HOLD_DAC              3
#define CD_POWER2VWDC1_ATT_DAC              4
#define CD_POWER2VWDC2_COOL_DAC             5
#define DVD_POWER2VRDC_APC_DAC              6
#define DVD_POWER2VRDC_HOLD_DAC             7
#define DVD_POWER2VWDC_APC_DAC_RW           8
#define DVD_POWER2VWDC_APC_DAC_R            9
#define DVD_POWER2VWDC_HOLD_DAC_RW          10
#define DVD_POWER2VWDC_HOLD_DAC_R           11
#define DVD_VWDC12PK1DAC_R                  12
#define DVD_VWDC12PK2DAC_RW                 13
#define DVD_VWDC2Dac_R                      14
#define DVD_VWDC2Dac_RW                     15
#define DVD_POWER2VWDC1_ATT_DAC_R           16
#define DVD_POWER2VWDC1_ATT_DAC_RW          17
#define DVD_POWER2VWDC2_COOL_DAC_RW         18
#define DVD_POWER2VWDC2_COOL_DAC_R          19
#define GPADC2Temperature                   20
#define CD_DVD_TEMP_MODE                    21
#define DVD_POWER2VWDC_APC_RAMLEV1_DAC      22
#define DVD_POWER2VWDC_APC_RAMLEV2_DAC      23

#define CD_POWER2VWDC_APC_DAC_R             24
#define CD_POWER2VWDC_APC_DAC_RW            25
#define CD_POWER2VWDC_HOLD_DAC_R            26
#define CD_POWER2VWDC_HOLD_DAC_RW           27
#define CD_POWER2VWDC1_ATT_DAC_R            28
#define CD_POWER2VWDC1_ATT_DAC_RW           29
#define CD_POWER2VWDC2_COOL_DAC_R           30
#define CD_POWER2VWDC2_COOL_DAC_RW          31
#define CD_VWDC2Dac                         32
#define DVD_VWDC22CDAC_RAM                  33
#define DVD_POWER2VWDC2_DAC_RAM             34
#define DVD_READCH_MW2FPDO                  35
#define DVD_WRITECH_MW2FPDO                 36
#define DVD_PEAKCH_MW2FPDO                  37
#define CD_READCH_MW2FPDO                   38
#define CD_WRITECH_MW2FPDO                  39
#define CD_PEAKCH_MW2FPDO                   40
#define DVD_READ_POWER2VRDC_HOLD_DAC        41
#define DVD_POWER2VRDC_APC_DAC_GAIN_1       42
#if (EN_Persistent_table_8 == 1)
#define DVD_BIASPOWER2DIFF_HOLD_DAC         78
#define HD_POWER2VRDC_APC_DAC               80
#define HD_POWER2VRDC_HOLD_DAC              81
#define BD_POWER2VRDC_APC_DAC               82
#define BD_POWER2VRDC_HOLD_DAC              83
#define BD_POWER2VWDC_APC_DAC_R             84
#endif/*(EN_Persistent_table_8 == 1)*/

#define CD_LD       0
#define DVD_LD      1
#define BD_LD       2
#define HD_LD       3

#define READ_CH         0
#define COOLING_CH_R    1
#define COOLING_CH_RW   2
#define WRITE_CH_R      3
#define WRITE_CH_RW     4
#define PEAK_CH_R       5
#define PEAK_CH_RW      6
#define PEAK_CH_RAM     7

#define CHECK_READ_APC_2CH    0
#define CHECK_READ_FIX_2CH    1
#define CHECK_READ_FIX_1CH    2

/* ------------------------------------------------------- *
 *     Input Para for "lib_SetLaserMode"                   *
 * ------------------------------------------------------- */
#define LASER_MODE_SHUTOFF  0
#define LASER_MODE_READ     1
#define LASER_MODE_WRITE    2
#define LASER_MODE_VRDCHOLD 3       // VRDC using hold value when WGATE is on
#define LASER_MODE_CAL1     0x81    // for VRDC offset, adjust
#define LASER_MODE_CAL2     0x82    // for VWDC1 offset
#define LASER_MODE_CAL3     0x83    // for VWDC2 Calib
#define LASER_MODE_CAL4     0x84    // for VWDC1 adj

/* ------------------------------------------------------- *
 *     Input Para for "LaserOscControl"                    *
 * ------------------------------------------------------- */
#define HOLD_POWER_FACTOR 0.85   // Make Hold power to match APC power

#if WIDER_TOK_LVL_FOR_PLAY_MODE == 1
/* ------------------------------------------------------- *
 *          Set_TOK_Mode
 * Mode1: Seek mode: TOK detects track OK/Not
 * Mode2: Play mode: Force TOK to OK
 * ------------------------------------------------------- */
#define TOK_SEEK  0
#define TOK_PLAY  1
#endif

/* ------------------------------------------------------- *
 *     Number of timeout timer                             *
 * ------------------------------------------------------- */
#define TIMEOUT_TIMER_MAX    3

/* ------------------------------------------------------- *
 *     Read Retry Option                             *
 * ------------------------------------------------------- */
#if (HORROR_DISC_IMPROVEMENT == 1)
#define READ_RETRY_ALL_OFF 0x0000

#define READ_RETRY_ENABLE 0x0001                                         //(bit 0)
#define READ_RETRY_DISABLE 0xFFFE
#define READ_RETRY_TIMER_ENABLE 0x0002                              //(bit 1)
#define READ_RETRY_TIMER_DISABLE 0xFFFD
#define READ_RETRY_RESET_ENABLE 0x0004                              //(bit 2)
#define READ_RETRY_RESET_DISABLE            0xFFFB    //20101027_02RY == Fix some code TYPO issue.

#define SERVO_DEFECT_RETRY_ENABLE 0x0008                          //(bit 3)
#define SERVO_DEFECT_RETRY_DISABLE 0xFFF7
#define HF_DEFECT_RETRY_ENABLE 0x0010                                //(bit 4)
#define HF_DEFECT_RETRY_DISABLE 0xFFEF
#define DPDGN_RETRY_ENABLE 0x0020                                       //(bit 5)
#define DPDGN_RETRY_DISABLE 0xFFDF
#define DFE_RETRY_ENABLE 0x0040                                           //(bit 6)
#define DFE_RETRY_DISABLE 0xFFBF
#if(EN_ADEQ==1)
#define ADEQ_RETRY_ENABLE 0x0080                                        //(bit 7)
#define ADEQ_RETRY_DISABLE 0xFF7F
#endif
#define SLOW_PLL_RETRY_FORCE_LOCK_ENABLE 0x0100            //(bit 8)
#define SLOW_PLL_RETRY_FORCE_LOCK_DISABLE 0xFEFF
#define FAST_PLL_RETRY_SPECIAL_ENABLE 0x0200            //(bit 9)
#define FAST_PLL_RETRY_SPECIAL_DISABLE 0xFDFF
#define FEBC_RETRY_ENABLE                   0x0400          // (bit 10)
#define FEBC_RETRY_DISABLE                  0xFBFF
#define TILT_RETRY_ENABLE                   0x0800          // (bit 11)
#define TILT_RETRY_DISABLE                  0xF7FF
#define TESPT_RETRY_ENABLE                  0x1000          // (bit 12)
#define TESPT_RETRY_DISABLE                 0xEFFF
//20101116_02VL ==start== For BDRE-DL VDD 0.4(BHE-M201 AR400) read fail, frank modify Andy confirm
#if (DVD_RETRY_USE_DPD == 1) || (BD_RETRY_USE_DPD == 1)
#define TE_DPD_RETRY_ENABLE                 0x2000          // (bit 13)
#define TE_DPD_RETRY_DISABLE                0xDFFF
#define TE_DPD_TRK_RETRY_ENABLE             0x4000          // (bit 14)
#define TE_DPD_TRK_RETRY_DISABLE            0xBFFF
//20101116_02VL ==end== For BDRE-DL VDD 0.4(BHE-M201 AR400) read fail, frank modify Andy confirm
#endif

#define SERVO_STATUS_ACTDISS_FOC            0x01
#define SERVO_STATUS_ACTDISS_TRK            0x02
#define SERVO_STATUS_FE_DISTURBANCE         0x04
#define SERVO_STATUS_TE_SHOCK               0x08
#define SERVO_STATUS_DRIVER_SHUT_DOWN       0x10

extern USHORT read_retry_option,read_retry_status;
extern ULONG   read_retry_restartTime;
extern BYTE    read_retry,dfe_retry;
extern BYTE    fgNormalValueSaved;
extern BYTE    fgNormalValueChgAgain;
extern BYTE    BufferErrorForRetry;

#define TRACK_PULL_BACK 0x01
    #endif // #if (HORROR_DISC_IMPROVEMENT == 1)

typedef enum   {ePauseOn = 0,
                ePauseOff}      ePauseMode;

typedef enum   {eTimeJump = 0,
                eDataJump}      eJumpMode;

typedef enum   {eDataSeek = 0x00,
                eWriteSeek,
                eTimeSeek,
                eLayer0 = 0x80,
                eLayer1,
                eBCASeek,
                eWLayer0 = 0xC0,
                eWLayer1}   eSeekMode;

typedef enum   {eCLV= 0,
                eCAV}           eSpeedMode;

typedef enum   {eLeftAndRight = 0,
                eLeft,
                eRight}         eVolumeChannel;

typedef enum   {eCDRom = 0,
                eEIAJ16fs = 4,
                eEIAJ162fs = 5,
                eEIAJ164fs = 6} eOutputFormat;

typedef enum   {eUnknownDiameter = 0,
                e8cm,
                eFashion,
                e12cm,
                eNoDisc}        eDiscDiameter;


typedef enum   {eNormal = 0,                    // Laser type compliance with current disk kind
                eForceCD,                       // Force to turn on 780 laser, for disk identify
                eForceDVD,                      // Force to ture on 650 laser, for disk identify
                eForceHDDVD,                    // Force to ture on 405 laser, for disk identify
                eForceBD}      eDiscLaser;      // Force to ture on 405 laser, for disk identify

/* T.B.D Must take at least 20 drives */
typedef enum   {eNominalTimeNoDisc  = 0,                    // Nominal toal substrate time for No disc
                eNominalTimeCD      = 2,                    // Nominal toal substrate time for CD disc
                eNominalTimeDVD     = 15,                   // Nominal toal substrate time for DVD disc
                eNominalTimeHDDVD   = 15,                   // Nominal toal substrate time for HD disc
                eNominalTimeBD      = 30 }  eNominalTime;   // Nominal toal substrate time for BD disc

typedef enum   {eUnknownType = 0,
                eNonATIPDisc,
                eATIPDisc,
                eNonADIPLPPDisc = 0x81,
                eLPPDisc,
                eADIPDisc,
                eBDDisc,
                eHDDisc}      eDiscType;

typedef enum   {eClose =0,
                eOpen}      eTraySwitch;

typedef enum   {eMoveTrayOut = 0,
                eMoveTrayIn}    eTrayMoveDir;

typedef enum   {eDemodETM = 0,
                eDemodFSM}    eDemodMode;

#define DiscStdWidth 3
typedef enum
{
    eDiscUnknown = 0,
    eDiscCD,
    eDiscDVD,
    eDiscLS,
    eDiscBD,
    eDiscHD,
    eDiscLast = eDiscHD
} eDiscStd;


#define MediaTypeWidth 3
typedef enum
{
    eMediaUnknown = 0,
    eMediaROM,
    eMediaR,
    eMediaRW,
    eMediaRAM,
    eMediaChina,
    eMediaLS
} eMediaType;

#if(DISC_MIS_JUDGE_RECOVERY)
#define RetryMediaTypeLast  eMediaRW+1
#endif

#define WblTypeWidth 2
typedef enum
{
    eWblUnknown = 0,
    eWblPlus,
    eWblMinus
} eWblType;

#define LayersWidth 2
typedef enum
{
    eLayersUnknown = 0,
    eLayersSingle,
    eLayersDouble,
    eLayersLast = eLayersDouble
    //eLayersTriple,
    //eLayersQuad,
    //eLayersLast = eLayersQuad
} eLayers;

typedef enum   {eFakeWbl = 0,
                eRealWbl}      eENCPLLSrc;

typedef enum   {eDefault = 0,
                eRawWbl,
                eDegWbl}       eRealWblSrc;

typedef enum   {eDegSrvWbl = 0,
                eSynthSrvWbl}  eSrvWblSrc;

typedef enum   {eDegSpdWbl = 0,
                eSynthSpdWbl}  eWblSpdSrc;

typedef enum   {eFakeMode = 0,
                eRealFreqMode,
                eRealFreqPhaseMode,
                eRealPhaseMode,} eENCPLLMode;

typedef enum   {eFineSeek = 0,
                eRoughSeek,
                eRoughSeekRatioOnly,
                eDamper}    eTrackEQType;

typedef enum   {eRealRX = 0,
                eFakeRX}    eJumpType;

typedef enum   {eRampSTM = 0,
                eRampFEpp,
                eRampFEBG}  eRampMeasType;

#define MediaTypeShift 0
#define WblTypeShift (MediaTypeShift + MediaTypeWidth)
#define LayersShift (WblTypeShift + WblTypeWidth)
#define DiscStdShift (LayersShift + LayersWidth)

typedef struct
{
    USHORT Media:MediaTypeWidth; // when applying use eMediaType enum
    USHORT WblType:WblTypeWidth; // when applying use eWblType enum
    USHORT Layers:LayersWidth;   // when applying use eLayers enum
    USHORT DiscStd:DiscStdWidth; // when applying use eDiscStd enum
} DiscKindPartsT;

#if(FORCE_DISK_KIND==1)
typedef USHORT DiscForceT;

extern DiscForceT DiscForce;
#define ENABLE_DISCKIND_FORCING  1
#define DISABLE_DISCKIND_FORCING 0
#endif

#define MakeDiscKind(D, L, W, M) \
        ((D<<DiscStdShift)|(L<<LayersShift)|(W<<WblTypeShift)|(M<<MediaTypeShift))

typedef enum   {eUnknownKind =      0,               //0x00
                eCDROMDisc =        MakeDiscKind(eDiscCD,  eLayersSingle, eWblUnknown, eMediaROM),
                eDiscKindFirst =    eCDROMDisc,
                eCDRDisc =          MakeDiscKind(eDiscCD,  eLayersSingle, eWblUnknown, eMediaR),
                eCDRWDisc =         MakeDiscKind(eDiscCD,  eLayersSingle, eWblUnknown, eMediaRW),
                eLSDisc =           MakeDiscKind(eDiscCD,  eLayersSingle, eWblUnknown, eMediaLS),

                eDVDROMDisc =       MakeDiscKind(eDiscDVD, eLayersSingle, eWblUnknown, eMediaROM),
                eDVDPlusRDisc =     MakeDiscKind(eDiscDVD, eLayersSingle, eWblPlus,    eMediaR),
                eDVDPlusRWDisc =    MakeDiscKind(eDiscDVD, eLayersSingle, eWblPlus,    eMediaRW),
                eDVDMinusRDisc =    MakeDiscKind(eDiscDVD, eLayersSingle, eWblMinus,   eMediaR),
                eDVDMinusRWDisc =   MakeDiscKind(eDiscDVD, eLayersSingle, eWblMinus,   eMediaRW),

                eDVDRAMDisc =       MakeDiscKind(eDiscDVD, eLayersSingle, eWblUnknown, eMediaRAM),

                eDVDROMDLDisc =     MakeDiscKind(eDiscDVD, eLayersDouble, eWblUnknown, eMediaROM),
                eDVDPlusRDLDisc =   MakeDiscKind(eDiscDVD, eLayersDouble, eWblPlus,    eMediaR),
                eDVDPlusRWDLDisc =  MakeDiscKind(eDiscDVD, eLayersDouble, eWblPlus,    eMediaRW),
                eDVDMinusRDLDisc =  MakeDiscKind(eDiscDVD, eLayersDouble, eWblMinus,   eMediaR),
                eDVDMinusRWDLDisc = MakeDiscKind(eDiscDVD, eLayersDouble, eWblMinus,   eMediaRW),

                eBDROMDisc =        MakeDiscKind(eDiscBD,  eLayersSingle, eWblUnknown, eMediaROM),
                eBDRDisc =          MakeDiscKind(eDiscBD,  eLayersSingle, eWblUnknown, eMediaR),
                eBDREDisc =         MakeDiscKind(eDiscBD,  eLayersSingle, eWblUnknown, eMediaRW),
                eBDROMDLDisc =      MakeDiscKind(eDiscBD,  eLayersDouble, eWblUnknown, eMediaROM),
                eBDRDLDisc =        MakeDiscKind(eDiscBD,  eLayersDouble, eWblUnknown, eMediaR),
                eBDREDLDisc =       MakeDiscKind(eDiscBD,  eLayersDouble, eWblUnknown, eMediaRW),

                eHDROMDisc =        MakeDiscKind(eDiscHD,  eLayersSingle, eWblUnknown, eMediaROM),
                eCBHDROMDisc =      MakeDiscKind(eDiscHD,  eLayersSingle, eWblUnknown, eMediaChina),
                eHDRDisc =          MakeDiscKind(eDiscHD,  eLayersSingle, eWblUnknown, eMediaR),
                eHDRWDisc =         MakeDiscKind(eDiscHD,  eLayersSingle, eWblUnknown, eMediaRW),
                eHDRAMDisc =        MakeDiscKind(eDiscHD,  eLayersSingle, eWblUnknown, eMediaRAM),
                eHDROMDLDisc =      MakeDiscKind(eDiscHD,  eLayersDouble, eWblUnknown, eMediaROM),
                eCBHDROMDLDisc =    MakeDiscKind(eDiscHD,  eLayersDouble, eWblUnknown, eMediaChina),
                eHDRDLDisc =        MakeDiscKind(eDiscHD,  eLayersDouble, eWblUnknown, eMediaR),
                eHDRWDLDisc =       MakeDiscKind(eDiscHD,  eLayersDouble, eWblUnknown, eMediaRW),

                eDiscKindLast
}eDiscKind;

typedef union
{
    eDiscKind fDiscKind;
    DiscKindPartsT Bit;
}discKindT;

typedef union
{
    SHORT        foc_act_dc_sens;
    BYTE         CaliFocActSens;
    eNominalTime NominalTime;
}FocActSensT;

typedef struct
{
    BYTE     ReadGain;
}AfeOPUGain;

typedef struct
{
    SHORT   ENCPLLN;
    BYTE    ENCPLLK;
    BYTE    EPLLKO;
    BYTE    EQPRANGE;
    BYTE    QPLEVEL;
    BYTE    ELFRSET;
    BYTE    ELFCSET;
    BYTE    PMWIN;
}ENCPLLData;

typedef struct
{
    BYTE    EQPRANGE;
    BYTE    QPLEVEL;
}WrENCPLLData;

typedef struct
{
    BYTE    WBLDIV;
    BYTE    WBLCLKRNG;
    BYTE    WDZONE;
}WBLSPDData;

typedef struct
{
    ServoStatesT            ServoState;
    ServoExecutionStateT    ServoExecutionState;
    ServoCommandsT          ServoCommand;
    BYTE           ServoParameters[10];
} ServoInterfaceInfoS;

extern ServoInterfaceInfoS     SIF;

typedef struct
{
    BYTE    Direction;
    TimeS   Time;
} SVOTimeS;

typedef union{
    USHORT all16;
    SHORT  all16s;
    struct{
        BYTE Lbyte;
        BYTE Hbyte;
    }bytes;
} svoWordU;

typedef union{
    LONG  all32s;
    ULONG all32;
    struct{
        BYTE byte0;
        BYTE byte1;
        BYTE byte2;
        BYTE byte3;
    }bytes;
} svoDWordU;

typedef struct
{
    BYTE        Layer;
    svoDWordU   SectorNum;
} SVOTIDInfoU;

typedef struct
{
    BYTE        SectorInfo;
    svoDWordU   SectorNum;
} SVOCIDInfoU;

/*--------------------------------------------------------------------*/
/* Variables defined in IRAM.C file but used by servo code            */
/*--------------------------------------------------------------------*/
// Servo Variables in Internal Memory for fast access, allocated in
// IRAM.C file.   Variables should be ordered carefully not to have
// gap for 32-bit address boundary, because the Internal Memory gives
// fast access but it is VERY expensive and the size is limited.
struct svoIramVarT {
SVOTIDInfoU     currentID;
SVOTimeS        currentTime;                // Iram_CurTime;
USHORT          svoInState;                 // Iram_svoInState;
USHORT          svoPreInState;
USHORT          svoOrigInState;
vUSHORT         knlTimeupCount;             // Iram_Knl_timeup_counter;
ESpindleSpeed   svoCurrentSpeed;            // Iram_svoNowSpdlSpeed;
vBYTE           subQEnableFlag;             // Iram_SubQ_EI_DI;
vBYTE           spindleStopFlag;            // Iram_disc_moter_flag;
vBYTE           spindleStopChkCnt;          // Iram_disc_stop_check_count
//#if (DVD_RAM_READ == 1)
//vBYTE           tracksize_set;
//#endif

// TBD? COMMENT! After all of variables are gathered here, we could see that many
// BYTE variables are just flags, and these could be converted to bit
// definitions in a word to save the expensive internal memory.
vBYTE           SubqAtipBypass:2;
BOOL            emergencyStopFlag:1;        // Iram_Emergencystop;
vBOOL           fineSeekDoneFlag:1;         // Iram_FINE_END;
vBOOL           subQReadyFlag:1;            // Iram_F_subqrdy;
vBOOL           atipReadyFlag:1;            // Iram_F_atiprdy;
vBOOL           headerIDReadyFlag:1;
vBOOL           testAreaReadEnableFlag:1;   // Iram_test_era_read_enab;
vBOOL           opcIsrEnableFlag:1;         // Iram_enable_opc_isr;
BOOL            knlTimeupCountFlag:1;       // Iram_Knl_timeup_counter_eidi;
vBOOL           focusDropFlag:1;            // Iram_focus_drop_flag;
BOOL            switchNextTrnFlag;
#if (NEW_FDP_DETECTION != 0)
vBOOL           focusDropIsrFlag;           // Iram_focus_drop_interrut_flag;
#endif
#if (SUPPORT_DPD == 1) || (CD_MB_ONLY_OPU == 1)
BOOL            do_cd_dpd_disc:1;           // support cd_dpd disc
#endif
#if (ENABLE_BDRLTH == 1)
BOOL            LTHDisc:1;                  // BDR LTH disc
BOOL            LTHDisclikely:1;            // BDR LTH disc
BOOL            LTHDiscRPChecked:1;         // BDR LTH disc
#endif
#if (EN_LS == 1)
vBYTE           LSSpiralTrackingFlag;       // Sprial tracking of lightscribe
#endif // End of (EN_LS == 1)
#if (ENABLE_BDIH == 1)
BOOL            IHDisc:1;                  // BDR LTH disc
BOOL            IHDisclikely:1;            // BDR LTH disc
#endif
};

/*--------------------------------------------------------------------------*/
/* Variables for seek control                                               */
/*--------------------------------------------------------------------------*/
struct  svoSeekVarT {
LONG            parN0;
LONG            parF0;
USHORT          parRough;
USHORT          parRoughInit;
USHORT          parFine;
USHORT          parFineInit;        // for speed calc
LONG            parN0_Li;
LONG            parF0_Li;
USHORT          parRough_Li;
USHORT          parFine_Li;
ULONG           parDelay1;
BYTE            parDelay2;
BYTE            parDelay3;
SVOTIDInfoU     svoTargetID;        // svoTargetID;
BYTE            advance;            // seek ahead
USHORT          targetRadius;       // traget radius in sled step;
SHORT           parFine2;
USHORT          revDur;
BYTE            roughCount;         // rough_search count;
BYTE            fineCount;          // fine_search count;
LONG            jumpDistance;       // jump distance
BYTE            jumpType;           // indicate layer,rough,or fine jump and the direction.
BYTE            rx_available;
SHORT           stpErrCount;
};

struct svoPcalVarT  /* for sunext oem power calibration variables.*/
{
    float   cdp2rOff;
    float   cdp2rSlp;
    float   cdr2sOff;
    float   cdr2sSlp;
    float   cdp2w1Off;
    float   cdp2w1Slp;
    float   cdp2w2Off;
    float   cdp2w2Slp;
    float   cdp2e1Off;
    float   cdp2e1Slp;
    float   cdp2e2Off;
    float   cdp2e2Slp;
    float   cdp2pk1Off;
    float   cdp2pk1Slp;
    float   cdp2pk2Off;
    float   cdp2pk2Slp;
    float   cdp2c1Off;
    float   cdp2c1Slp;
    float   cdp2c2Off;
    float   cdp2c2Slp;

    float   cdp2wOff;
    float   cdp2wSlp;
    float   cdp2eOff;
    float   cdp2eSlp;
    float   cdv2eOff;
    float   cdv2eSlp;
    float   cdp2pkOff;
    float   cdp2pkSlp;
    float   cdmw2fpdoOff;
    float   cdmw2fpdoSlp;
    float   cdmw2fpdo1Off;
    float   cdmw2fpdo1Slp;
    float   cdmw2fpdo2Off;
    float   cdmw2fpdo2Slp;


    SHORT   cdFBDacValue;
    #if (EN_Persistent_table_8 == 1)
    SHORT   cdFeoBGValue;
    SHORT   cdSPP2BGValue;
    #else
    SHORT   cdFeBGValue;
    #endif/* (EN_Persistent_table_8 == 1) */
    SHORT   cdRRFGValue;//not used any more
    SHORT   cdBetaOffset;
    SHORT   cdDiffDac;
    SHORT   cdVWDCtoVRDC;
    SHORT   cdVWDCtoVWDC1;

    float   dvp2rOff;
    float   dvp2rSlp;
    float   dvr2sOff;
    float   dvr2sSlp;
    float   dvp2w1Off;
    float   dvp2w1Slp;
    float   dvp2w2Off;
    float   dvp2w2Slp;
    float   dvp2e1Off;
    float   dvp2e1Slp;
    float   dvp2e2Off;
    float   dvp2e2Slp;
    float   dvv2e1Off;    //vwdc vs. Dac
    float   dvv2e1Slp;
    float   dvv2e2Off;
    float   dvv2e2Slp;
    float   dvvwdc12pk1dacOff;    //vwdc1 vs. pk1Dac
    float   dvvwdc12pk1dacSlp;
    float   dvvwdc12pk2dacOff;
    float   dvvwdc12pk2dacSlp;
    float   dvp2pk1Off;
    float   dvp2pk1Slp;
    float   dvp2pk2Off;
    float   dvp2pk2Slp;
    float   dvp2c1Off;
    float   dvp2c1Slp;
    float   dvp2c2Off;
    float   dvp2c2Slp;
    #if (EN_Persistent_table_8 == 1)
    float   dvbp2Diffdacoff;
    float   dvbp2DiffdacSlp;
    #endif
    float   dvvwdc22cdacOff;    //vwdc2 vs. cooling Dac
    float   dvvwdc22cdacSlp;
    float   dvp2c3Off;
    float   dvp2c3Slp;
    float   dvmw2fpdoOff;
    float   dvmw2fpdoSlp;
    float   dvmw2fpdo1Off;
    float   dvmw2fpdo1Slp;
    float   dvmw2fpdo2Off;
    float   dvmw2fpdo2Slp;

    float dvv2tOff;
    float dvv2tSlp;

    SHORT   dvFBDac0Value;
    #if (EN_Persistent_table_8 == 1)
    SHORT   dvFeoBG0Value;
    SHORT   dvSPP2BG0Value;
    #else
    SHORT   dvFeBG0Value;
    #endif/* (EN_Persistent_table_8 == 1) */
    SHORT   dvFBDac1Value;

    #if (EN_Persistent_table_8 == 1)
    SHORT   dvFeoBG1Value;
    SHORT   dvSPP2BG1Value;
    #else
    SHORT   dvFeBG1Value;
    #endif/* (EN_Persistent_table_8 == 1) */
    SHORT   dvRRFGValue;//not used any more
    SHORT   dvBetaOffset;
    SHORT   dvDiffDac;
    SHORT   dvVWDCtoVRDC;
    SHORT   dvVWDCtoVWDC1;

    SHORT   dvTiltDacValue[5];
    BYTE    dvTiltDacNum;

    SHORT LDMode;
    SHORT FPDO;
    SHORT VRDC_VWDCX;

    BOOL    fgLoadCalEeprom;
    BOOL    fgCalAutoMode;
    BOOL    fgCalDVDMode;
    ULONG   CalDoneBits;  //Offline Calibration Done Bits
    ULONG   CalPowerDoneBits;  //Power Offline Calibration Done Bits

    #if (EN_Persistent_table_8 == 1)
    float   bdp2rOff;
    float   bdp2rSlp;
    float   bdr2sOff;
    float   bdr2sSlp;
    float   bdp2w1Off;
    float   bdp2w1Slp;
    float   bdp2w2Off;
    float   bdp2w2Slp;
    float   bdp2e1Off;
    float   bdp2e1Slp;
    float   bdp2e2Off;
    float   bdp2e2Slp;
    float   bdv2e1Off;    //vwdc vs. Dac
    float   bdv2e1Slp;
    float   bdv2e2Off;
    float   bdv2e2Slp;

    float   bdp2c1Slp;
    float   bdp2c1Off;
    float   bdp2c2Slp;
    float   bdp2c2Off;

    SHORT   bdDiffDac;
    #endif

    #if (EN_Persistent_table_8 == 1)
    float   hdp2rOff;
    float   hdp2rSlp;
    float   hdr2sOff;
    float   hdr2sSlp;
    float   hdp2w1Off;
    float   hdp2w1Slp;
    float   hdp2w2Off;
    float   hdp2w2Slp;
    float   hdp2e1Off;
    float   hdp2e1Slp;
    float   hdp2e2Off;
    float   hdp2e2Slp;
    float   hdv2e1Off;    //vwdc vs. Dac
    float   hdv2e1Slp;
    float   hdv2e2Off;
    float   hdv2e2Slp;

    float   hdp2c1Slp;
    float   hdp2c1Off;
    float   hdp2c2Slp;
    float   hdp2c2Off;

    SHORT   hdDiffDac;
    #endif
};

#if (OPU == DXX811)
/* TDK Dxx811 barcode data */
struct OPUBarcodeT{
    BYTE    SerialNum[5];
    USHORT  ProductionDay;
    BYTE    TesterNumber;
    BYTE    LC_UPPER_ELEMENT;
    BYTE    LC_LOWER_ELEMENT;
    BYTE    LC_SA;
    BYTE    LC_ASO;
    BYTE    LC_AS45;
    BYTE    LC_COMA;
    BYTE    THETAT;
};
/* TDK Dxx811 barcode parameter data */
typedef struct{
    BYTE   IC_SYMBOL;
    SHORT  Initial_value_U;
    SHORT  Initial_value_L;
    SHORT  PARA_SA;
    SHORT  PARA_AS0;
    SHORT  PARA_AS45;
    SHORT  PARA_COMA;
} OPUBarcodeP;
#elif (OPU == SOHBP4)
struct OPUBarcodeT{
    USHORT    serial_0L;
    USHORT    serial_0H;
    USHORT    serial_1L;
    USHORT    serial_1H;
};
#elif (OPU == KES410A)
/* SONY KES-410 barcode data */
struct OPUBarcodeT{
    USHORT    serial_0L;
    USHORT    serial_0H;
    USHORT    serial_1L;
    USHORT    serial_1H;
    BYTE      RAD_SKEW_DVD;
    BYTE      TAN_SKEW_DVD;
    BYTE      READPOWER_CD;
    BYTE      READPOWER_DVD;
    BYTE      READPOWER_BDL0;
    BYTE      READPOWER_BDL1;
    BYTE      HFMF_BD;
    BYTE      HFMF_SS_BD;
    BYTE      TILT_SENS_CD;
    BYTE      TILT_SENS_DVD;
    BYTE      TILT_SENS_BD;
    BYTE      HFMP_BD0SL;
    BYTE      HFMP_BD1DL;
    BYTE      FFCG_FFC_CD;
    BYTE      FFCG_FFC_DVD;
    BYTE      FFCG_FFC_BD;
    BYTE      IVG_CDDVD;               //[6:4]CD, [2:0]DVD
    BYTE      IVG_BD;                  //[2:0]BD
    BYTE      RAD_SKEW_BD;
    BYTE      RAD_SKEW_CD;
    USHORT    SA_ACT_BD_L0;            //32[1:0]&33[7:0] total 10bits
    USHORT    SA_ACT_BD_L1;            //34[1:0]&35[7:0] total 10bits
    USHORT    SONY_410_CHECK_SUM;      //44[7:0]&45[7:4] total 12bits
};
#elif (OPU == HOPB1300)
/* Hitachi B1300 barcode data */
struct OPUBarcodeT{
    USHORT    OPUTYPE;
    BYTE      RAD_SKEW_BD;
    BYTE      TAN_SKEW_BD;
    BYTE      RAD_SKEW_DVD;
    BYTE      TAN_SKEW_DVD;
    SHORT     SA_ACT_BD_L0;
    SHORT     SA_ACT_BD_L1;
};
#elif (OPU == SFBD412)||(OPU == SFBD414)
/* SANYO SF-BD412 barcode data */
struct OPUBarcodeT{
    BYTE      OPU_FEATURE;
    BYTE      MANU_CONTROL_CODE;
    BYTE      PROD_LINE_CODE;
    BYTE      LABEL_ISSUE_YEAR;
    BYTE      LABEL_ISSUE_MONTH;
    BYTE      LABEL_ISSUE_DATE;
    BYTE      TAN_SKEW_CD;
    BYTE      RAD_SKEW_CD;
    BYTE      TAN_SKEW_DVD;
    BYTE      RAD_SKEW_DVD;
    BYTE      TAN_SKEW_BD;
    BYTE      RAD_SKEW_BD;
//    BYTE      RPWR_CD;
//    BYTE      RPWR_DVD;
//    BYTE      RPWR_BDSL;
//    BYTE      RPWR_BDDL;
//    BYTE      HFMF_CD;
//    BYTE      HFMF_DVD;
//    BYTE      HFMF_BD;
//    BYTE      IOP_CD;
//    BYTE      IOP_DVD;
//    BYTE      IOP_BDSL;
    float     DF_CD;
    float     DF_DVD;
    float     DF_BDSL;
    BYTE      TEMP_BD;
    SHORT      SA_ACT_BD_L0;
    SHORT      SA_ACT_BD_L1;
    BYTE      CHECK_SUM_H;
    BYTE      CHECK_SUM_L;
};
#elif (OPU == SOHBP7)
struct OPUBarcodeT{
    BYTE      OPU_FEATURE;
    BYTE      MANU_CONTROL_CODE;
    BYTE      PROD_LINE_CODE;
    BYTE      LABEL_ISSUE_YEAR;
    BYTE      LABEL_ISSUE_MONTH;
    BYTE      LABEL_ISSUE_DATE;
    BYTE      TAN_SKEW_CD;
    BYTE      RAD_SKEW_CD;
    BYTE      TAN_SKEW_DVD;
    BYTE      RAD_SKEW_DVD;
    BYTE      TAN_SKEW_BD;
    BYTE      RAD_SKEW_BD;
//    BYTE      RPWR_CD;
//    BYTE      RPWR_DVD;
//    BYTE      RPWR_BDSL;
//    BYTE      RPWR_BDDL;
//    BYTE      HFMF_CD;
//    BYTE      HFMF_DVD;
//    BYTE      HFMF_BD;
//    BYTE      IOP_CD;
//    BYTE      IOP_DVD;
//    BYTE      IOP_BDSL;
    float     DF_CD;
    float     DF_DVD;
    float     DF_BDSL;
    BYTE      TEMP_BD;
    SHORT      SA_ACT_BD_L0;
    SHORT      SA_ACT_BD_L1;
    BYTE      CHECK_SUM_H;
    BYTE      CHECK_SUM_L;
};
#elif (OPU == BDPL2)
/* Pioneer BDPL2 barcode data */
struct OPUBarcodeT{
    BYTE      OPU_TYPE;
    ULONG     OPU_SERIAL;
    BYTE      OPU_QRVER;
    BYTE      BDSL_IV;
    BYTE      BDSL_IO;
    BYTE      BDSL_PL0;
    BYTE      BDSL_PL1;
    BYTE      BDSL_IGAIN;
    BYTE      BDSL_HMA;
    BYTE      BDSL_HMF;
    BYTE      BDDL_IV;
    BYTE      BDDL_IO;
    BYTE      BDDL_PL0;
    BYTE      BDDL_PL1;
    BYTE      BDDL_IGAIN;
    BYTE      BDDL_HMA;
    BYTE      BDDL_HMF;
    BYTE      DV_IV;
    BYTE      DV_IO;
    BYTE      DV_P;
    BYTE      DV_IGAIN;
    BYTE      DV_HMA;
    BYTE      DV_HMF;
    BYTE      CD_IV;
    BYTE      CD_IO;
    BYTE      CD_P;
    BYTE      CD_IGAIN;
    BYTE      CD_HMA;
    BYTE      CD_HMF;
    USHORT    SA_ACT_BD_L0;
    USHORT    SA_ACT_BD_L1;
    USHORT    DVSA;
    USHORT    CDSA;
    float     DF_BD;
    float     DF_DV;
    float     DF_CD;
    BYTE      BD_APPK_X10;
    BYTE      CD_DPPK_X10;
    SHORT     BD_RAD_TILT_X100;
    SHORT     DV_RAD_TILT_X100;
    SHORT     BD_TAN_TILT_X100;
    SHORT     DV_TAN_TILT_X100;
    SHORT     BD_INI_TEMP_X10;
    SHORT     DV_INI_TEMP_X10;
    SHORT     CD_INI_TEMP_X10;
    BYTE      CHECK_SUM_H;
    BYTE      CHECK_SUM_L;
};
#endif // OPU

struct FOCSENST {
    BYTE      CD;
    BYTE      DVD;
    BYTE      HD;
    BYTE      BD;
};
#if ((OPU == KES410A)||(OPU == KEM350AAA)||(OPU == SOHBP4)||(OPU == DXX811)||(OPU == HOPB1300)||(OPU == SFBD412)||(OPU == SFBD414)||(OPU == BDPL2)||(OPU == SOHBP7))
extern struct   OPUBarcodeT  OPUBarcode;
#endif //((OPU == KES410A)||(OPU == KEM350AAA)||(OPU == SOHBP4)||(OPU == DXX811))
extern struct   FOCSENST  FOCSENS;

#define UNEXECUTION_WOPC_INDEX          255

/*for Write Parameter State */
#define WRITE_PARM_NULL_STATE           0x0000
#define TEMP00_STATE                    0x8000
#define TEMP01_STATE                    0x4000
#define TEMP02_STATE                    0x2000
#define TEMP03_STATE                    0x1000
#define TEMP04_STATE                    0x0800
#define TEMP05_STATE                    0x0400
#define TEMP06_STATE                    0x0200
#define TEMP07_STATE                    0x0100
#define TEMP08_STATE                    0x0080
#define TEMP09_STATE                    0x0040
#define TEMP0A_STATE                    0x0020
#define TEMP0B_STATE                    0x0010
#define TEMP0C_STATE                    0x0008
#define TEMP0D_STATE                    0x0004
#define CNT_MASK                        0xFFFC
#define CNT_NO2_STATE                   0x0002
#define CNT_NO1_STATE                   0x0001
typedef enum _EBdWriteStatus
{
    E_WRITE_STATUS_IDLE,
    E_WRITE_STATUS_START_ERROR,
    E_WRITE_STATUS_WRITING,
    E_WRITE_STATUS_STOP_ERROR,
    E_WRITE_STATUS_FINISHED
} EBdWriteStatus;
/*--------------------------------------------------------------------*/
/* Variables defined by non-servo code but shared with servo code     */
/*--------------------------------------------------------------------*/
extern BYTE     CarinaTestEnable;
extern BYTE     CarinaTable;
extern USHORT   CarinaDeltaP;
extern BYTE     Support_Speed;


/*-------------------------------------------------------------------*/
/* Variables defined by servo code but shared with non-servo code    */
/*-------------------------------------------------------------------*/
extern  USHORT      Iram_system_timer_counter;
extern  ULONG       Pre_Iram_system_timer;
extern  ULONG       svoVar_TimeCodeCheckTimer;
extern  BOOL        svoVar_getDVDFormatflag;

extern vBOOL        svoVar_EccCheckEnable;
extern vUSHORT      svoRunOut;                  // (tracks)
extern vUSHORT     svoRunOutCTN;                  // (tracks)

extern  BYTE        svo_Var_motorDriverState;   // Iram_diver_state;
extern  eDiscDiameter   svoVar_discDiameter;    // Iram_Disc_Diameter;
extern  discKindT   svoVar_discKindT;

extern  FocActSensT svoVar_STM;                 // Focus Actuator Sensitivity structure for production test

#if (Customer_A1 ==1)
extern BYTE 		PositionHomeErrorCount;
#endif
extern BYTE 		MediaRecovery;


extern  ESpindleSpeed svoVar_svoTargetSpeed;    // svoSpdlSpeed;

extern  BYTE        svoVar_powerSaveFlag;       // save_timer_ei;
extern  ETrackingMethod svoVar_TrackingMethod;  // Tracking method;
#if (CALIBRATION_MANAGEMENT == 0)
extern  BYTE        svoVar_TiltStepperZone;     // tilt calibation stepper zone index;
#endif
extern  SHORT       svoPreTiltValue;

extern  BYTE        svoVar_bByPassFOKCheck;
#if (PDIC_BASE_ON_SRFO == 1)
extern  BYTE        svoVar_bPdicMode;
extern  BYTE        svoVar_bLascMode;
#endif
extern  BYTE        CALIBRATE_SYSTEM_LEAD_IN;
#if (ENABLE_HDDVD==1)
extern  BYTE        RP2GN;
extern  BYTE        RP2OS;
#endif

#if (DETECT_ECC_DISC == 1)
extern  ecc_code_t  svoVar_eccDiscNum;          // ecc_disc_no
#endif

#ifdef BYPASS_POWERSAVE_ISR
extern BOOL    svoFgBaseOnPowerSaveFlag;
#endif

extern struct svoIramVarT svoIramVar;
extern struct svoSeekVarT svoSeekVar;
extern struct svoPcalVarT svoPcalVar;

extern BYTE    svoDiagStatus;
extern SHORT   svoDiagReturnValue1;
extern SHORT   svoDiagReturnValue2;
extern SHORT   svoDiagReturnValue3;
#if (EN_Persistent_table_8 == 1)
extern SHORT   svoDiagReturnValue4;
#endif
extern SHORT   ReadCAL_Dac;
extern SHORT   WriteCAL_Dac;
extern SHORT   PeakCAL_Dac;
extern SHORT   Channel_Mode;
extern SHORT   CHECK_POWER_MODE;

extern USHORT  fConut_NUM;
extern BOOL    fCheck_Pluse_Power;
extern BOOL    fAPC;

extern SHORT   svoVar_sPwrCalDac[15];
extern SHORT   svoVar_sPwrCalValue[15];

extern BYTE    svoVar_bPwrCalCount;

#if LPP_AUTO_LEARN_TEST == 1
extern BYTE    svoVar_optLPPTH3;
extern BYTE    svoVar_optLPPTH4;
#endif
extern BYTE    svo_bDiscTblNo;
extern BYTE    svo_bSMPTblNo;
extern BOOL    svo_fgDramStgyFill;

extern BYTE    bFocMoveReference;
extern BYTE    bLayerErrorCount;

#if ((DVD_UNBALANCE_DISC_CHECK==1) || (CD_UNBALANCE_DISC_CHECK == 1)||(BD_UNBALANCE_DISC_CHECK == 1))
extern BYTE    svoVar_unbalanceDiscNum;    // unbalance_disc_no
extern BYTE    fUnbalanceChk_SpdUp;
extern USHORT  Unbalance_lowspd;
extern USHORT  Unbalance_midspd;
extern USHORT  Unbalance_highspd;
#endif

#if (TRAVERSE_TEST == 1 )
extern SHORT   Error_1;
#endif

extern BYTE    svoPrvtVar_focusP2PCount;                       // focus_pp_count;
extern SHORT   svoPrvtVar_avgFOO;
extern USHORT  svoPrvtVar_avgFEpp[2];                          // FEpp_level
extern USHORT  svoPrvtVar_maxFEpp[2];                          // max FEpp_level
extern USHORT  svoPrvtVar_avgBSpp[2];                          // BSpp_level
extern USHORT  svoPrvtVar_avgSRFO[2];                          // SRFO_level
extern USHORT  svoPrvtVar_maxSRFO;                             // SRFO_level
extern USHORT  svoPrvtVar_maxSRFOpp_0dB;                       // SRFO_level in 0db
extern USHORT  svoPrvtVar_maxFEpp_0dB;
extern BYTE    svoPrvtVar_TEBSRatioX100;                       //
#if (SERVOKEYPARAMETER == 1)
extern USHORT  svoPrvtVar_RFpp;                                //
extern USHORT  RSETPOINT;                                      //
extern USHORT  ADCVRPOW;                                       //
extern USHORT  VDELTAX;                                        //
extern USHORT  svoPrvtVar_maxFEpp_0dB;
extern USHORT  svoPrvtVar_MPPO_0dB;
extern BYTE    svoPrvtVar_MPPOSRFORatioX100;
extern USHORT  svoPrvtVar_SledHomeIndex;                       // 20090305_s.22 == add sledge home info to key parameter
extern SHORT   LTHRPJGLVL;                                     // LTH RP judge level //20090324_f.26 == add FEPPL0/L1 and LTHRPJGLVL
extern BYTE    svoPrvtVar_troos;                               // 20090420_i.29 savekeyparameter troos;
extern BYTE    svoPrvtVar_wbllockcnt;                          //
extern BYTE    svoPrvtVar_jitter;                              //
extern SHORT   svoPrvtVar_pp_dpd_diff_mv;                      // TEpp(PP+DPD)-TEpp(PP-DPD)
#endif

extern BYTE         svoStmSubMissRty;

#if (EN_ADI == 1)
extern BYTE    servo_status;
#endif

// added Vendor IOPTest command.
extern USHORT  ADCVRPOW_CD;                                       //
extern USHORT  VDELTAX_CD;                                        //
extern USHORT  ADCVRPOW_DVD;                                       //
extern USHORT  VDELTAX_DVD;                                        //
extern USHORT  ADCVRPOW_BD;                                       //
extern USHORT  VDELTAX_BD;                                        //
extern USHORT  FMTHROFFSET_CD;
extern USHORT  FMTHROFFSET_DVD;
extern USHORT  FMTHROFFSET_BD;
extern USHORT  FMD_PPLVL_CD;
extern USHORT  FMD_PPLVL_DVD;
extern USHORT  FMD_PPLVL_BD;
extern USHORT  FMD_DCLVL_CD;
extern USHORT  FMD_DCLVL_DVD;
extern USHORT  FMD_DCLVL_BD;
extern USHORT  FGPULSECOUNT;

#if (BD_ENABLE == 1)
extern BOOL    svo_BDR_RE_PICReading;
#endif //(BD_ENABLE == 1)


/*-------------------------------------------------------------------*/
/* Function defined by servo code but shared with non-servo code     */
/*-------------------------------------------------------------------*/
extern void    svoRLdON(BYTE power_on);
extern void    afeMUXOUT(USHORT mux_field_addr,BYTE mux_field_mask, BYTE mux_start_bit, BYTE mux_selected);

#if (TRAVERSE_TEST == 1 )
extern void    svoStp_Traverse(BYTE,BYTE);
#endif

// for shock-proof exaclink
#define FOCUS_ERROR_THRESHOLD               0x28 // 375mV
#define FOCUS_ERROR_COUNTER_THRESHOLD       0x10
#if DETECT_TE_OFFTRACK
#define OFF_TRACK_COUNTER_THRESHOLD         0x10
#endif

#if (SUPPORT_DPD == 1)
#define LIMIT_SPEED_CLOSE_4_DPD_TE  (4320/2)
#endif

#if (MONITOR_OPU_TEMPERATURE == 1)
extern ULONG   svoReadTempTimer;
extern SBYTE   OpuTemperature;
#endif /* MONITOR_OPU_TEMPERATURE=1 */


extern void vWakeUpChipOnlyADDA(void);
extern void vWakeUpFromPowerSaveMode(void);

#define DVDROM  (svoVar_discKindT.fDiscKind == eDVDROMDisc)
#define DVDPR   (svoVar_discKindT.fDiscKind == eDVDPlusRDisc)
#define DVDMR   (svoVar_discKindT.fDiscKind == eDVDMinusRDisc)
#define DVDPRW  (svoVar_discKindT.fDiscKind == eDVDPlusRWDisc)
#define DVDMRW  (svoVar_discKindT.fDiscKind == eDVDMinusRWDisc)
#define DVDRAM  (svoVar_discKindT.fDiscKind == eDVDRAMDisc)

#define BDRDLDisc    (svoVar_discKindT.fDiscKind == eBDRDLDisc)
#define BDREDLDisc   (svoVar_discKindT.fDiscKind == eBDREDLDisc)

#define DVDRAM_SPIN_ZONE1   0x040000
#define DVDRAM_SPIN_ZONE2   0x080000
#define DVDRAM_SPIN_ZONE3   0x0C0000
#define DVDRAM_SPIN_ZONE4   0x100000
#define DVDRAM_SPIN_ZONE5   0x180000

#define POSSIBLE_DVDRAM (((svoCalVar.discChkDoneBits & blank_chk_end) == 0) && (svoVar_discKindT.Bit.DiscStd == eDiscDVD) && (svoVar_discKindT.Bit.Media == eMediaRW) && (svoVar_discKindT.Bit.Layers == eLayersSingle))

#if ((PCB == 690)||(PCB == 692))
#define RX_AVAILABLE ((svoVar_discKindT.Bit.DiscStd != eDiscDVD) &&         \
                      ((svoVar_discKindT.Bit.DiscStd == eDiscCD) ||         \
                       (svoVar_discKindT.Bit.Media == eMediaROM)))
#elif (PCB == 710)||(PCB == 711)||(PCB == 817)
#define RX_AVAILABLE ((svoVar_discKindT.Bit.DiscStd == eDiscCD) ||          \
                      ((svoVar_discKindT.Bit.Media == eMediaROM) &&         \
                       (svoVar_discKindT.Bit.DiscStd == eDiscBD)))
#elif (PCB == 815)
#define RX_AVAILABLE (svoVar_discKindT.Bit.DiscStd == eDiscCD)
#else
#define RX_AVAILABLE ((svoVar_discKindT.Bit.DiscStd == eDiscCD) ||          \
                      (svoVar_discKindT.Bit.Media == eMediaROM) ||          \
                      ((svoVar_discKindT.Bit.DiscStd == eDiscDVD) &&        \
                       (svoVar_discKindT.Bit.Media == eMediaR) &&           \
                       (pdbGetDiscStatus() == eFinalizedDisc)))
#endif

extern struct svoRRFVarT svoRRFVar;
extern struct svoRampMeasVarT svoRampMeasVar;
#if (STREAM_STM == 1)
extern struct streamMeasVarT streamMeasVar;
#endif

    #if (RP_AUTO_ADJUST == 1)
extern BYTE  RPOfsAdjust;
extern LONG  CalRP2Gain[2];
extern LONG  RPAcc;
extern LONG  RPAccPrv;
    #endif

#if (DISC_MIS_JUDGE_START_UP_RECOVERY)
extern BYTE RetryDiscLaser;
#endif

#endif /* _SVO_ */


/*------------------------------------------------------------------------*/
/* The end of file                                                        */
/*------------------------------------------------------------------------*/
