/****************************************************************************
*                (c) Copyright 2001 - 2005 SUNEXT DESIGN, INC
*                           All Rights Reserved
*
* FILENAME:   gp_ram.c
*
*   $Revision: 89 $
*   $Date: 11/03/28 2:37p $
*
* DESCRIPTION
* RAM variable Definitions allocated to the SDRAM
*
* NOTES:
*
**************************** SUNEXT CONFIDENTIAL *******************************/

#include ".\oem\oem_info.h"
#include ".\common\globals.h"
#include ".\common\globtype.h"
#include ".\hif\gp_ram.h"
#include ".\hif\powerctl.h"

/******************************************************
*   External Memory Variable Definitions
*******************************************************/
vBYTE  St_diag_HIFTransData;
void *PlayerAddrsPtr;               /* This is a replacement pointer storage location
                                    * for param1 within the Player Interface Structure.
                                    */
vBYTE ATA_SRST_rcv;
HEADER_T Sector_ID;                 /* Four BYTE Header Field in Lead-in Area */

ADDRESS_T Rw_seek_blk;              /* "pre_seeking" block */

long Target_Timelba;
long Tg_endlba;
long PreCmd_endlba;
ULONG Buf_scan_lba;
long Maxlba;                        /* Maximum LBA of disk (Lead-out - 1) */
#if (RANDOM_READ_BYPASS_CHECK_METHOD == 1)||(SEQ_SEEK_SPEED_DOWN == 1)
vULONG RandomCount;
#endif
/******************************
    Speed control variables
*******************************/
SpeedMDT    SpeedMD;
CD_Speed_T volatile Disc_Speed;
CD_Speed_T  Disc_SupportSpeed;
DiscSpeedT  Management_Max_rate;

#if (Customer_A1 ==1)
BYTE        CompleteStroke;
BYTE				AgingMode;
BYTE 				CompleteStrokeCount;
BYTE 	FEPPErrorCount;
#endif
BYTE        Current_read_sp;        /* current read speed */
BYTE        BackUpSpeed;
BYTE        RandomRead;
BYTE        SpeedUpAccumulation;
BYTE        SpeedTableColumnSize;
BYTE        APLastAskedSpeed;
SBYTE       SpeedDownReady;
const BYTE* ptrReadSpeedTablePointer;
vSHORT    SpeedCount;
vUSHORT    SpeedUpReady;
ULONG       RedLineTimer;
vULONG      OutputStream;           // unit: 1 = 1 sector = 2k
/*****************************
       Smart Play variables
******************************/
//For Green function Item 3 -> Standby timer set to 10 mins
//when playback mode detected by SmartX
#if(PCB == 710)||(PCB == 711)||(PCB == 815)||(PCB == 817)
BYTE        PlaybackConfidenceLevel;
BYTE        InPlaybackMode;
#endif
/*****************************
       Retry variables
******************************/
//BYTE Rw_seek_retry;                 /* seek retry counts */

BYTE Special_Disc=eNormalD;
BYTE Channel_ctrl_mode;             /* Audio L/R channel control */
BYTE Period_pg;                     /* Period Page number */

#if (BY_PASS_C2Error_For_VCD)
BYTE C2ErrorCount;
BYTE ReadGiveUpCounter = 0;
#endif

BYTE gAudioScanDirection;           /* Audio Scan direction: 0: Forward, 1: Reverse */

#if FORCE_ATIP_TRIGGER
vBOOL CheckWGateAtip;
#endif
#if CHECK_ATIP_QUALITY
vBOOL CheckAtipQuality;
vBYTE AtipSearchModeCount;
#endif

#if MARGINAL_DISC
vBOOL RequestSpinDown;
vWORD WrtChangeSPWaitTimer;
    #if DETECT_TE_OFFTRACK
BYTE TOKLVL_BACKUP;
BYTE Off_Track_Counter;
    #endif
#endif

#if DVD_MARGINAL_DISC
BYTE RequestSpeedDown;
#endif

vBYTE Slip_counter;  			/* write error protection */
BOOL St_cdi_audio;    			/* cdi plus audio disc */
vBOOL Read_subc_only;			/* read subcode only */
CMD_BLOCK_T     Play_Cmd_block; /* H/W Play key */

#if GET_LAST_TRACK_INFO_BASED_ON_RAI
BOOL READ_RAI_EMPTY;
#endif

#if (WRITE_PROTECT == 1)
FLAG PersistentWriteProtect;
BOOL UpdatePWP;
#endif
#if (DVD_RAM_READ == 1)
BOOL ANEW_SEEK;
#endif
#if (STORE_MSG_TO_SDRAM == 1)
//ULONG NextStoreMsgAddr;
ULONG MsgStAddr;                      //SDRAM message start address
ULONG MsgEdAddr;                     //SDRAM message end address
BOOL    EnableLogging;
BOOL    LoggingFull;
BOOL    DriveHangup;
#endif
#if (STORE_MSG_TO_FLASHROM==1)
BOOL    EnableAutoSaveLog;
BOOL    SaveLogToROMSwitch;
BOOL    SaveLogToROMatIdle;
#endif
#if (BypassDecoderEr == 1)
/********************************************************************
*   When drivers occur decoder error, It shall ignore these error,
*   return original error data.
*********************************************************************/
BOOL IGNORE_DECODER_ERROR;
BYTE RetryLimit;            //limit for bypass Decode Err
ULONG RetryChgTime;        //Retry limit change timing
USHORT ReadGoodCnt;            //no C2 PO count
#endif

/********************************************************************
*   Internal Direct Access Memory Definition for servo interface
*********************************************************************/

TimeS   CurPlayTime;

vBOOL St_no_toc_info;               /* No TOC Information available */

/*******************************************************
*   Internal Memory Variable Definitions
********************************************************/


/*******************************************************
*   Internal Memory Bit Variable Definitions
********************************************************/

DiscAttributesT DiscAttributes;
DiagStT     DiagSt;             /* .........         */
CopyCmpStT  CopyCmpSt;


/* for fast cache */
ULONG   FlashLinearAdress;
BYTE    mode_p5_data[MSENSE_PAGE_5_LEN];        // Write Parameters (provided by Mode Select/Sense parameters page 5 from Host)
BYTE    DeviceBusyEvent;                /* Device Busy event */
BYTE    DeviceBusyStatus;               /* Device Busy status */
USHORT  DeviceBusyTime;
BYTE    OperationalChangeTrayEvent;

#if (E_DIAGS == 1)
//BOOL Radj_enable;
//USHORT Radj_addr1;
//USHORT Radj_addr2;
//BYTE   Radj_mask1;
//BYTE   Radj_mask2;
//BYTE   Radj_value1;
//BYTE   Radj_value2;
#endif

vULONG  Toc_addr;
vULONG  Toc_fm2_addr;
vBYTE   gPowerEvent;
vBYTE   gPowerStatus;
vBYTE   gMediaEvent;
vBYTE   gMediaStatus;
vUSHORT Xfer_Residue;                   /* (in absolute addressing mode) for Xfer Limit check */
#if (USE_TWO_KEY_FUNCTION==1)
vBYTE   gSt_play_sw;
vBYTE   gHw_play_no;
vBOOL   gPause_By_SeekCmd_flag;
#endif /* USE_TWO_KEY_FUNCTION */

vBYTE	SATA_intf_pwr_state = 0;
vBYTE	BIST_In_Process = 0;
vBYTE	LinkTxRxErr = 0;
#if (ENABLE_AUTO_ABORT == 1)
vBYTE	AutoAbort = 0;
#endif

#if ((EN_AUTO_FIS34 == 0) || (CHIP_REV <= 0xB3))
vBYTE	SendFis34Already = 1; // power on default set to 1
vULONG  BGFis34Timer = 0;
#endif
vBYTE	SATAConnect = 1;

/* These two variables are used by ataGetMediaStatus() in at_cmds.c */
vBYTE	ataKeyIsPressed = FALSE;
vBYTE	ataMediaIsChanged = FALSE;

vLONG	DecTimeCnt;

/* variables for read track info command */
BYTE track_info_data[TRACK_INFO_LEN_BYTE];

USHORT track_info_len;
ULONG WriteListStartPSN;

#if(BACKGROUND_READ_AHEAD == 1)
BYTE START_BACKGROUND_BUFFER;
#endif

WORD PreCurHostPtr;
vSHORT  Pre_Buf_hxfer_blk;

#if (DVD_RAM_READ == 1)
#if (WORKAROUND_FOR_SEEDMISTHR == 1)
BOOL ChangeSeedMisThr;
BOOL ResetSeedMisThr;
BOOL RestSeedCount;
#endif
#if (ENABLE_RAM_DFE_ICE_DETECT == 1)
ULONG PrePID;
ULONG PreICEAdd;
ULONG CurICEAdd;
#endif
#endif

#if (EN_ADI == 1)
ULONG ADItimer;
BYTE  ADICounter;
#endif

BYTE ServoPllInited = FALSE;
//start ==To judge BD BCA valid or not.
#if(JudgeBDBCAValid == 1)
BOOL ReadBDBCAValid;
#endif
//end ==To judge BD BCA valid or not.
/*******************************************************
*  Special Disc Information Format tables
********************************************************/
/*Special Disc Information Format*/
/*-----------------------------------------------------------------------------------------------------------
|Field Name	|Indicator|Disc Restriction|Check Method|Number of Sessions|Number of Tracks|Return ID|Descriptor|
-------------------------------------------------------------------------------------------------------------
|Field Size	|   1     |         1      |      1     |         1        |        1       |     1   |     N    |
-----------------------------------------------------------------------------------------------------------*/

/*Descriptor A(Check Method is CHECK_STRING)*/
/*---------------------------------------------------
|Field Name	|Start LBA|Buffer Count|Check Size|String|
-----------------------------------------------------
|Field Size	|   4     |     1      |     1    |  N   |
-----------------------------------------------------*/

/*Descriptor B(Check Method is CHECK_TRACK_POSITION)*/
/*-----------------------------------------
|Field Name	|Start LBA|End LBA|Track Number|
-------------------------------------------
|Field Size	|   4     |   1   |     1      |
-------------------------------------------*/

/* Please check that the correctness of tables when they are modified. */

const BYTE VideoCDInformationTable[] =
{
    NEW_SDI_CHECK, DATA_CD_DISC, CHECK_STRING, PASS_THIS_ITEM, PASS_THIS_ITEM, eVideoCD, 0x00, 0x00, 0x00, 0x12, 2, 64, 'V', 'C', 'D', '\0',
    CONTINUE_SDI_CHECK, DATA_CD_DISC, CHECK_STRING, PASS_THIS_ITEM, PASS_THIS_ITEM, eVideoCD, 0x00, 0x00, 0x00, 0x12, 2, 64, 'M', 'P', 'E', 'G', 'A', 'V', '\0',
    NEW_SDI_CHECK, DATA_CD_DISC, CHECK_STRING, PASS_THIS_ITEM, PASS_THIS_ITEM, eVideoCD, 0x00, 0x00, 0x00, 0x12, 2, 64, 'S', 'V', 'C', 'D', '\0',
    CONTINUE_SDI_CHECK, DATA_CD_DISC, CHECK_STRING, PASS_THIS_ITEM, PASS_THIS_ITEM, eVideoCD, 0x00, 0x00, 0x00, 0x12, 2, 64, 'M', 'P', 'E', 'G', '2', '\0',

    END_SDI_CHECK
};

const BYTE VideoDVDInformationTable[] =
{
    NEW_SDI_CHECK, ALL_KINDS_OF_DVD_DISCS, CHECK_STRING, PASS_THIS_ITEM, PASS_THIS_ITEM, eVideoDVD, 0x00, 0x00, 0x01, 0x00, 2, 16, 'A', 'U', 'D', 'I', 'O', '_', 'T', 'S', '\0',
    NEW_SDI_CHECK, ALL_KINDS_OF_DVD_DISCS, CHECK_STRING, PASS_THIS_ITEM, PASS_THIS_ITEM, eVideoDVD, 0x00, 0x00, 0x01, 0x00, 2, 16, 'V', 'I', 'D', 'E', 'O', '_', 'T', 'S', '\0',

    END_SDI_CHECK
};

#if( Detect_AVCHD_DISC == 1)
const BYTE AVCHDInformationTable[] =
{
    NEW_SDI_CHECK, ALL_KINDS_OF_DVD_DISCS, CHECK_STRING, PASS_THIS_ITEM, PASS_THIS_ITEM, eAVCHD, 0x00, 0x00, 0x00, 0x10, 1, 16, 'B', 'E', 'A', '0','1', '\0',

    END_SDI_CHECK
};
#endif

const BYTE FlashDiscInformationTable[] =
{
    NEW_SDI_CHECK, ALL_KINDS_OF_CD_DISCS, CHECK_STRING, PASS_THIS_ITEM, PASS_THIS_ITEM, eFlashDisc, 0x00, 0x00,0x00, 0x10, 4, 32, DISC_FW_UPDATE_STRING, '\0',
    NEW_SDI_CHECK, ALL_KINDS_OF_DVD_DISCS, CHECK_STRING, PASS_THIS_ITEM, PASS_THIS_ITEM, eFlashDisc, 0x00, 0x00,0x00, 0x10, 4, 32, DISC_FW_UPDATE_STRING, '\0',

    END_SDI_CHECK
};

const BYTE SpecialDiscInformationTable[] =
{
    NEW_SDI_CHECK, ALL_KINDS_OF_CD_DISCS, CHECK_STRING, PASS_THIS_ITEM, PASS_THIS_ITEM, eWinBench_99, 0x00, 0x00,0x00, 0x10, 4, 32, 'C', 'D', ' ', 'W', 'i', 'n', 'B', 'e', 'n', 'c', 'h', ' ', '9', '9', '\0',
    NEW_SDI_CHECK, ALL_KINDS_OF_CD_DISCS, CHECK_STRING, PASS_THIS_ITEM, PASS_THIS_ITEM, eCdTach2, 0x00, 0x00,0x00, 0x10, 1, 32, 'C', 'D', 'T', 'A', 'C', 'H', '2', '\0',
    NEW_SDI_CHECK, ALL_KINDS_OF_CD_DISCS, CHECK_STRING, PASS_THIS_ITEM, PASS_THIS_ITEM, eCDI, 0x00, 0x00,0x00, 0x10, 1, 32, 'C', 'D', '-', 'I', '\0',
    NEW_SDI_CHECK, NON_WOBBLE_DISC, CHECK_TRACK_POSITION, 1, 23, eABEX702, 0x00, 0x00,0xD3, 0xD1, 0x00, 0x00, 0xF7, 0xD9, 2,
    CONTINUE_SDI_CHECK, NON_WOBBLE_DISC, CHECK_STRING, 1, 23, eABEX702, 0x00, 0x00, 0x00, 0x10, 1, 32, 0x30, 0x32, 0x73, 0x31, 0x36, 0x66, 0x20, '\0',

    END_SDI_CHECK
};

const BYTE VideoBDInformationTable[] =
{
    NEW_SDI_CHECK, ALL_KINDS_OF_BD_DISCS, CHECK_STRING, PASS_THIS_ITEM, PASS_THIS_ITEM, eVideoBD, 0x00, 0x00, 0x01, 0x42, 1, 16, 'B', 'D', 'M', 'V', '\0',
    NEW_SDI_CHECK, ALL_KINDS_OF_BD_DISCS, CHECK_STRING, PASS_THIS_ITEM, PASS_THIS_ITEM, eVideoBD, 0x00, 0x00, 0x01, 0x43, 1, 64, 'B', 'D', 'M', 'V', '\0',

    END_SDI_CHECK
};
