/****************************************************************************
*                (c) Copyright 2001 - 2005 SUNEXT DESIGN, INC.
*                           All Rights Reserved
*
*
*
* FILENAME:   gp_ram.h
*
* DESCRIPTION
* RAM variable Declaration
*
*   $Revision: 121 $
*   $Date: 11/03/31 2:43p $
*
**************************** SUNEXT CONFIDENTIAL *******************************/

#ifndef __GP_RAM_H__
#define __GP_RAM_H__

#include ".\oem\oem_info.h"
#include ".\common\vu_cfg.h"
#include ".\common\globals.h"
#include ".\common\globtype.h"
#include ".\common\hwdef.h"
#include ".\hif\gnrl_mac.h"

#include ".\common\sdram.h"

/**********************************************************************

    Global structures

**********************************************************************/

/*  CMD_BLOCK_T defined as union for possible future structural
    organization of the ATAPI command block. */

typedef union {
    BYTE bf[12];        /* 12 BYTE buffer */
} CMD_BLOCK_T;

typedef union {
    struct
    {
        BYTE l;
        BYTE m;
        BYTE h;
        BYTE u;
    } byte;
    TimeS   msf;
    long    lba;
} ADDRESS_T;

typedef struct {
    BYTE flag;
    USHORT hstbytac;    /* Host Byte Address offset */
    USHORT atabc;       /* ATAPI Byte Count */
    USHORT hstwdcnt;    /* Number of ATAPI Words per page */
} MODES_PARA_T;

typedef struct {
    BYTE key;
    BYTE cod;
    BYTE qul;
    BYTE err;
} SENSE_T;

typedef struct {
    BYTE p_state;
    BYTE p_status;
    BYTE a_accept;
    BYTE command_IDC;
    BYTE a_command;
    BYTE param1;
    BYTE param2;
    BYTE param3;
    BYTE param4;
    BYTE param5;
    BYTE param6;
    BYTE param7;
    BYTE param8;
    BYTE param9;
    BYTE param10;
    BYTE param11;
    BYTE param12;
} PLAYER_INTERFACE_T;

typedef union
{
    struct
    {
        BYTE    session_no;
        ULONG   dram_addr;
    } s2b_toc;
} HEADER_T;

/*-----------------------------------------------------------------
    Read Buffer structure. Contains information regarding
    current data in the read buffer. Includes
    buffer pointers for both host and disc interfaces
------------------------------------------------------------------*/
typedef struct
{
    ULONG   CacheStartLBA;  /* First cached LBA in the buffer       */
    ULONG   CacheEndLBA;    /* Last cached LBA in the buffer        */
    ULONG   SectorCount;    /* Number of DVD sectors in the buffer  */
    WORD    CurHostPtr;     /* Current host buffer pointer          */
    WORD    CurDiscPtr;     /* Current decoder buffer pointer       */
} DataBufferS;

/*-----------------------------------------------------------------
    Transfer states describing state of transfer.
------------------------------------------------------------------*/
typedef enum
{
        TRANSFER_COMPLETE,      /* Transfer complete or idle state          */
        TRANSFERRING,           /* Transfer in progress                     */
        WAITING_FOR_TRANSFER,   /* Transfer on hold, waiting to resume      */
        TRANSFER_STOPPED        /* Transfer was stopped before completing   */
} TransferStatesE;

/*-----------------------------------------------------------------
    Host transfer structure. Contains information regarding
    current host transfer request
------------------------------------------------------------------*/
typedef struct
{
    ADDRESS_T       StartBlock;       /* Start LBA of the host transfer       */
    StLongU         BlockCount;     /* Total number of blocks to transfer   */
    ULONG           BlocksLeft;     /* Blocks remaining to be transferred   */
    TransferStatesE State;          /* Current decoder buffer pointer       */
} TransferS;

/**********************************************************************

    Variables

**********************************************************************/
extern vBYTE   SdramRefreshInterval ;// Save value for Power Control function
extern vBYTE   SdramRefreshIntervalSleep;

extern volatile DataBufferS DataBuffer;
extern volatile TransferS HostXfer;

extern void *PlayerAddrsPtr;

extern vBYTE        ataPowerMode;

extern volatile CMD_BLOCK_T Iram_Cmd_block;
extern CMD_BLOCK_T      Play_Cmd_block;
extern ADDRESS_T    Rw_seek_blk;

extern vLONG        Iram_Audio_blk2_play;
extern vUSHORT      Iram_Audbuf_blkcnt;
extern TimeS        CurPlayTime;

/*Workaroud for currrent time no continue*/
extern vLONG    CurWrCount;
extern vLONG    PreWrCount;
/*Workaroud for currrent time no continue*/
extern vUSHORT      Iram_Buf_ahead_bptr;
extern vUSHORT      Iram_Knl_timer_counter;         /* used to be in cstart.asm */
extern vBYTE   PreATAPICMD;	           /* To store the previous ATAPI command */
extern vULONG  PreReadCMDLba;          /* To store the previous read start LBA */

#define TIMEUP_6S   (6000/20)
#define TIMEUP_7S   (7000/20)
#define TIMEUP_10S  (10000/20)
#define TIMEUP_30S  (30000/20)
#define TIMEUP_50S  (50000/20)
#define TIMEUP_NON  0xFFFF

#define TIME_6S   6000
#define TIME_7S  7000

#define NEGO_PWRON  0x80
#define NEGO_DIAG   0x40
#define NEGO_SRST   0x20

typedef struct
{
    BYTE Read;              /* set cd speed read */
    BYTE Write;             /* set cd speed write */
    BYTE Write2;            /* set cd speed write2 (for SPIN DOWN Control) */
    BYTE WriteMin;          /* write minimum speed (for write speed control) */
#if MARGINAL_DISC
    BYTE WriteMarginal;     /* set cd speed write for marginal disc */
#endif
#if (SEQ_SEEK_SPEED_DOWN == 1)
    BYTE SeekSpeed;         /* sequence seek speed.*/
#endif
}CD_Speed_T;

#if (SET_SLOT_IN == 1) //[BTC][Q36] for flag define
extern vBYTE DiscInFlag; //[Q36]judge disc in or disc in retry
extern vBYTE DiscInRetryFlag; //[Q36]judge disc in retry
extern vUSHORT SwitchFlag; //[Q36]udge first and second stage disc in
extern vUSHORT DiscRetryFlag; //[Q36]disc in retry accumulate
extern vBOOL   fgSRSTEj;//[Q36]Jam when power on ,Disc auto tray out.
#endif

extern vULONG   Iram_Knl_save_counter;  // Speed Up/Down timer value
#if (USE_TWO_KEY_FUNCTION==1)
extern vBYTE            Iram_PlayPressdTM;
#endif /* USE_TWO_KEY_FUNCTION */
#if (RANDOM_READ_BYPASS_CHECK_METHOD == 1) || (SEQ_SEEK_SPEED_DOWN == 1)
extern vULONG RandomCount;
#endif
extern SpeedMDT     SpeedMD;
extern CD_Speed_T volatile Disc_Speed;
extern CD_Speed_T   Disc_SupportSpeed;
extern DiscSpeedT   Management_Max_rate;
extern BYTE Special_Disc;/*  for Special Disc */
extern const BYTE* ptrReadSpeedTablePointer;
extern BYTE        BackUpSpeed;
extern vSHORT		SpeedCount;
extern BYTE        RandomRead;
extern BYTE        SpeedUpAccumulation;
extern BYTE        SpeedTableColumnSize;
extern vUSHORT      SpeedUpReady;
extern SBYTE         SpeedDownReady;
extern BYTE         APLastAskedSpeed;
extern ULONG         RedLineTimer;
extern vULONG      OutputStream;
//For Green function Item 3 -> Standby timer set to 10 mins
//when playback mode detected by SmartX
#if(PCB == 710)||(PCB == 711)||(PCB == 815)||(PCB == 817)
extern BYTE         PlaybackConfidenceLevel;
extern BYTE         InPlaybackMode;
#endif

#if (Customer_A1 ==1)
extern BYTE   CompleteStroke;
extern BYTE		AgingMode;
extern BYTE 	CompleteStrokeCount;
extern BYTE 	FEPPErrorCount;
#endif


#define bEXECUTE_ROC            bit0
#define bDETECT_UNBALANCE       bit1
#define bDETECT_DEFECT         bit0

extern vBYTE        ATA_SRST_rcv;
extern long         Target_Timelba;
extern long         Tg_endlba;
extern long         PreCmd_endlba;
extern BYTE         gAudioScanDirection;
extern ULONG        Buf_scan_lba;
#if (DVD_RAM_READ == 1)
extern BOOL ANEW_SEEK;
#endif
#if FORCE_ATIP_TRIGGER
extern vBOOL CheckWGateAtip;
#endif
#if CHECK_ATIP_QUALITY
extern vBOOL CheckAtipQuality;
extern vBYTE AtipSearchModeCount;
#define MAX_ATIP_SEARCH_COUNT   2
#endif

#if MARGINAL_DISC
extern vBOOL RequestSpinDown;
extern vWORD WrtChangeSPWaitTimer;
#if DETECT_TE_OFFTRACK
extern BYTE TOKLVL_BACKUP;
extern BYTE Off_Track_Counter;
#endif
#endif

#if DVD_MARGINAL_DISC
extern BYTE RequestSpeedDown;
#endif

#if (BY_PASS_C2Error_For_VCD)
extern BYTE C2ErrorCount;
extern BYTE ReadGiveUpCounter;
#endif

extern vBYTE  Iram_RW_ECC_Status;
extern vBYTE Slip_counter;  /* write error protection */
extern BOOL St_cdi_audio;  /*cdi plus audio disc */
extern vBOOL Read_subc_only; /* read subcode only */
#if GET_LAST_TRACK_INFO_BASED_ON_RAI
extern BOOL READ_RAI_EMPTY;
#endif

extern vSHORT               Iram_Buf_hxfer_blk;
extern volatile ADDRESS_T   Iram_Buf_trg_blk;
extern volatile ADDRESS_T   Iram_Buf_ahead_blk;
extern vBYTE        fKernelAutoCompletionStatus;
extern vBYTE        fKernelSendCompletionStatus;


#define RD_AHEAD_BUF_NOP            0x00
#define RD_AHEAD_BUF_REQ            0x01
#define RD_AHEAD_BUF_CHK            0x02
#define RD_AHEAD_BUF_DONE           0x03

extern vBYTE        Iram_rd_ahead_chk_req;
extern vBYTE        Iram_Rw_read_mode;

#if (SET_SLOT_IN == 1)
extern vBYTE        Iram_Auto_TrayIn_timer; //[BTC][Q36]Jam when power on ,Disc auto tray out.
#endif

extern vBYTE        Iram_LED_Flash_timer;                   /* Sets the frequency of LED flash */
extern vBYTE        Iram_Gp_led_timer;
extern vBYTE        Iram_Gp_led_at_timer;
extern vULONG       Iram_Buf_dram_addr;
extern vULONG       Iram_Linear_Xfer_addr;
extern vBYTE        Iram_Svr_player_mode;
extern volatile SENSE_T     Iram_Sense;
extern HEADER_T     Sector_ID;

#define _Nr_of_sessions     Sector_ID.s2b_toc.session_no
#define S2b_buf_dram_addr   Sector_ID.s2b_toc.dram_addr

extern vBYTE        Iram_shHDDir;
extern vBYTE        Iram_shHICtl;
extern vUSHORT      Iram_shUMisc;                 /* Shadow variable for UMisc register */
extern vUSHORT      Iram_Residue;

extern vUSHORT      Iram_Sh_wbk;
#define S2b_toc_ptr Iram_Sh_wbk

extern vUSHORT      Iram_shHstBufPg;
extern vUSHORT      Iram_shHstWdC;
extern vUSHORT      Iram_shHstBytAC;
extern vULONG       Iram_shATB;
extern vUSHORT      Iram_Xfr_allow;
extern vUSHORT      Iram_Hw_host_blks_limit;

extern BYTE         Period_pg;

extern BYTE         Channel_ctrl_mode;      /* used to be in cstart.asm */
//extern BYTE         Rw_seek_retry;
extern BYTE         Current_read_sp;


extern vBYTE        Iram_AtipAdipLppErrorAmount;
extern vBYTE        Iram_AtipAdipLppErrorCount;
extern vBYTE        Iram_ECCBlockCount;

/*  Overlay or make local Variables below are used in the UpdateTOCData().
    These are temp variables, only used in first power on or tray I/O. */

extern vULONG Toc_addr;
extern vULONG Toc_fm2_addr;

#define Toc_lead_out        (*(ULONG  *)(&HostXfer.StartBlock.lba))
#define Toc_1st_trk_addr    (*(ULONG volatile *)(&DataBuffer.CacheEndLBA))

extern long         Maxlba;
extern long         buf_h2come_blk;

extern vBYTE        Iram_Player_output[12];
extern vBOOL        St_no_toc_info;             /* No TOC Information available */

#if (USE_TWO_KEY_FUNCTION==1)
extern vBYTE    gSt_play_sw;
extern vBYTE    gHw_play_no;
extern vBOOL    gPause_By_SeekCmd_flag;

#define HW_PLAY_STOP_OFF                0
#define HW_PLAY_STOP_ON                 1
#endif  /* USE_TWO_KEY_FUNCTION */
extern vBOOL    Iram_Rd_subc_flag;          /* read data with subcode */

extern vBOOL  Iram_Diff_Time_Flag;          /*  ATIP Time and Qcode PTime different */

/*Automatic change from 2.5K to 3k page for C2PO*/
extern vBOOL  Iram_Page_Size_3K_Flag;
extern vUSHORT  DRAM_AUX_SIZE;              /* DRAM AUX Size */
extern vUSHORT  RING_END_PAGE;              /* last page the SP can access*/
extern vUSHORT  BUF_TOTAL_BLK_COUNT;        /* total pages */
extern vUINT    DATA_START_ADDR;            /* Data start address */
extern vUINT    AUX_START_ADDR;             /* Aux start address  */
extern vUSHORT  AUX_HEADER_ADDR1;           /* M1 block header offset in page */
extern vUSHORT  AUX_HEADER_ADDR2;           /* M2F1 block header offset in page */
/*Automatic change from 2.5K to 3k page for C2PO*/

/*********************************************************************
*
*       Bit Variable Definitions
*
*********************************************************************/

volatile  typedef struct
{
    BYTE MasterMode:1;              /* 1=master mode, 0=slave mode,     */
    BYTE SlaveDetct:1;              /* 1=slave detected,                */
    BYTE AtcmdAvail:1;              /* 1=ATA command available,         */
    BYTE CmdAvail:1;                /* 1=ATAPI command available,       */
    BYTE CmdInProcess:1;                /* 1=ATAPI command in process */
    BYTE ZeroLenght:1;              /* 1=request length is zero; zero blk transfer */
    BYTE SendCompl:1;               /* 1=final ATA status had been sent */
    BYTE Timeout:1;                 /* 1=timer 2 timeout happened */
    BYTE PwrRstIgnrDasp:1;          /* 0=timer2 ISR ignores DASP- for */
    BYTE IdentAvail:1;              /* 1=ATAPI identify available */
    BYTE InquAvail:1;               /* 1=ATAPI Inquiry available */
    BYTE GesnAvail:1;               /* 1=ATAPI Get Event Status available */
    BYTE GetConfigAvail:1;          /* 1=ATAPI Get Configuration available */
    BYTE become_ready:1;            /* 1=become_ready */
    BYTE AtaSubBrst:1;              /* 1=data/para xfr piece by piece */
    BYTE sleep_wait:1;              /* 1= sleep wait */
    BYTE SendBlkData2Host:1;        /* 1=sending block data to host */
    BYTE GetParaDta:1;              /* 1=waiting to receive parameter data */
    BYTE SendParaD2Host:1;          /* 1=sending parameter data to host */
    BYTE MsfFormat:1;               /* 1=the format is in MSF, 0=LBA format */
    BYTE DmaActive:1;               /* 1=DMA mode active, 0=PIO mode active */
    BYTE DmaFeature:1;              /* 1=DMA feature enabled, 0=DMA feature disabled */
    BYTE MultiDma:1;                /* 1=multiple_word DMA, 0=single_word DMA */
    BYTE UltraDma:1;                /* 1= ultra DMA mode */
} KnlFlagT;

extern vBYTE  Knl_atcmd_avail;
extern vBYTE  Knl_cmd_avail;
extern vBYTE  St_cmd_inproc;
extern vBYTE  Knl_send_compl;
extern vBYTE  Knl_timeout;
extern vBYTE  Knl_IdentAvail;
extern vBYTE  Knl_Atapi_Avail;
extern vBYTE  Ata_sub_burst;
extern vBYTE  St_send_blkdata;
extern vBYTE  Get_para_data;
extern vBYTE  St_send_pardata;

#define St_zero_length      Iram_KnlFlag.ZeroLenght
#define St_become_ready     Iram_KnlFlag.become_ready
#define Ata_sleep_wait      Iram_KnlFlag.sleep_wait
#define St_MSF              Iram_KnlFlag.MsfFormat
#define St_dma_mode         Iram_KnlFlag.DmaActive
#define St_dma_feature      Iram_KnlFlag.DmaFeature
#define St_multi_dma        Iram_KnlFlag.MultiDma
#define St_ultra_dma        Iram_KnlFlag.UltraDma

volatile typedef struct
{
    BYTE DeciSet:1;                 /* 1=decoder interrupt occurred,    b_3 */
    BYTE DecoderStop:1;             /* 1=decoder stop, 0=decoder running */
    BYTE TargetBlkRchd:1;               /* 1=target block reached */
    BYTE EccError:1;                    /* 1=ECC error */
    BYTE MonitorMode:1;             /* 1=in monitor mode */
    BYTE ReadCdCmd:1;                   /* 1=read CD command 0=read command */
    BYTE ModeChange:1;              /* 1=disc mode changed */
    BYTE InitBuffer:1;              /* 1=disc mode changed */
    BYTE RdCdDaDisc:1;              /* 1=reading CD-DA disc */
    BYTE RdCdrwSubDta:1;                /* 1=read CD with R-W subcode data */
    BYTE RdC2POData:1;              /* 1=read CD with C2PO data */
#if (SUN_FUNC_TEST == 1)
    BYTE RdC2POData296:1;           /* 1=read CD with C2PO data 296 bytes */
    BYTE RdC2PODataTaggle:1;        /* 1=read CD with C2PO data Taggle flag */    
#endif
    BYTE EnBlkLmt:1;                    /* 1=need to enable block limitation */
    BYTE PCAVSpeed:1;               /* 1= ClosedArea read */
    BYTE PlayAudioComplt:1;         /* 1=play audio completed */
    BYTE NxAudio:1;                 /* 1=Nx Audio State */
    BYTE AudtenInt:1;                   /* 1=Audio Data Transfer Int State */
    BYTE AudioExtract:1;                /* 1=AudioExtract */
    BYTE AudioPause:1;              /* 1=audio pausing */
    BYTE Method2:1;                 /* 1=Method 2 State for Packet Read */
    BYTE RunInOut:1;                    /* 1=Run-in / Run-out State of Pre-Gap */
    BYTE RStart:1;              /* 1:CD I/F start to read */
    BYTE RdMiss:1;              /* 1:Target Miss */
    BYTE VerifyFlag:1;          /* 1= Verify cmd is active        */
} RwFlagT;

extern vBYTE  Knl_deci_set;
extern vBYTE  Rw_dec_stop;
extern vBYTE  Rw_tblk_reach;
extern vBYTE  Rw_ecc_error;
extern vBYTE  Rw_init_buffer;
extern vBYTE  St_audio_complete;
extern vBYTE  St_nx_audio;
extern vBYTE  St_audten_int;
extern vBYTE  St_audio_extract;
extern vBYTE  St_run_in_out;
extern vBYTE  fReadVerifyCommand;

#define Rw_rd_monitor           Iram_RwFlag.MonitorMode
#define Rw_rdcd_cmd             Iram_RwFlag.ReadCdCmd
#define Rw_mode_change          Iram_RwFlag.ModeChange
#define Rw_cdda_disc            Iram_RwFlag.RdCdDaDisc
#define Rw_subc_data            Iram_RwFlag.RdCdrwSubDta
#define Rw_C2PO_data            Iram_RwFlag.RdC2POData
#if (SUN_FUNC_TEST == 1)
#define RdC2POData_296          Iram_RwFlag.RdC2POData296
#define RdC2PODataTaggle        Iram_RwFlag.RdC2PODataTaggle
#endif
#define PartialCAV_Speed        Iram_RwFlag.PCAVSpeed
#define St_audio_pause          Iram_RwFlag.AudioPause
#define St_method_2             Iram_RwFlag.Method2
#define fRStart                 Iram_RwFlag.RStart
#define fRdMiss                 Iram_RwFlag.RdMiss

volatile typedef struct
{
    BYTE LedFlashReady:1;               /* 1:Flash Ready LED - works in conjunction with LedFlash */
    BYTE LedFlashWrite:1;               /* 1:Flash Write LED - works in conjunction with LedFlash */
    BYTE LedFlashError:1;               /* 1:Flash Error LED - works in conjunction with LedFlash */
    BYTE Ledatready:1;                  /* 1:ready LED - Auto ON from flash */
} LedStT;

#define St_led_flash_ready      Iram_LedFlag.LedFlashReady
#define St_led_flash_write      Iram_LedFlag.LedFlashWrite
#define St_led_flash_error      Iram_LedFlag.LedFlashError
#define St_led_at_ready         Iram_LedFlag.Ledatready

volatile typedef struct
{
    BYTE DoorOpen:1;                    /* 1=door is open */
    BYTE LoadDisc:1;                    /* 1=Load Disc State */
    BYTE UnitAttn:1;                    /* 1=unit attention */
    BYTE CdChange:1;                    /* 1=CD disc had been changed */
    BYTE NoDisc:1;                      /* 1=no disc in tray */
    BYTE DrvRdy:1;                      /* 1=drive is ready */
    BYTE CDTextRdy:1;                   /* 1=CD TEXT is ready */
    BYTE TocRdy:1;                      /* 1=TOC is ready */
    BYTE LockCd:1;                      /* 1=Prevent CD from being removed State */
    BYTE CmdOpenReq:1;                  /* 1=Media Event Open Request */
} DriveStT;

#define St_door_open            Iram_DriveSt.DoorOpen
#define St_load_disc            Iram_DriveSt.LoadDisc
#define St_unit_atten           Iram_DriveSt.UnitAttn
#define st_disc_change          Iram_DriveSt.CdChange
#define St_no_disc              Iram_DriveSt.NoDisc
#define St_drv_ready            Iram_DriveSt.DrvRdy
#define St_CDText_ready         Iram_DriveSt.CDTextRdy
#define St_toc_ready            Iram_DriveSt.TocRdy
#define St_cd_prevent           Iram_DriveSt.LockCd

volatile typedef struct
{
    BYTE StartStopImm:1;        /* 1= immediete start/stop */
    BYTE ImmdteMode:1;          /* 1=immediate command */
    BYTE SvoPauseReq:1;         /* 1=servo pause is required */
} SvoFlagT;
extern vBYTE  Svr_pause_reqire;

volatile typedef struct
{
    BYTE BlankDisc:1;               // 1 = Disc is blank
    BYTE CdromData:1;               // 1 = the CD disc is data mode (mode 1)
    BYTE CdromAudio:1;              // 1 = the CD disc is audio
    BYTE CdromMixed:1;              // 1 = the CD disc is mixed mode
    BYTE CdromHybd:1;               // 1 = the CD disc is hybrid mode
    BYTE CdromPlus:1;               // 1 = the CD disc is CD+
    BYTE CdromIRdy:1;               // 1 = the CD disc is CD-I
    BYTE CdromI:1;                  // 1 = the CD disc is CD-I
    BYTE CdromExtra:1;              // 1 = the CD disc is CD-Extra
    BYTE Video:1;                   // 1 = VIDEO CD
    BYTE Size12cm:1;                // 1 = 12cm disc,    0 = 8cm disc
    BYTE SizeFashion:1;             // 1 = Fashion disc, 0 = 8cm disc
} DiscAttributesT;

#define St_blank_disc           DiscAttributes.BlankDisc
#define St_cdrom_data           DiscAttributes.CdromData
#define St_cdrom_audio          DiscAttributes.CdromAudio
#define St_cdrom_mixed          DiscAttributes.CdromMixed
#define St_cdrom_hybrid         DiscAttributes.CdromHybd
#define St_cdrom_cdplus         DiscAttributes.CdromPlus
#define St_cdi_ready            DiscAttributes.CdromIRdy
#define St_cdi                  DiscAttributes.CdromI
#define St_cdrom_extra          DiscAttributes.CdromExtra
#define St_cdrom_video          DiscAttributes.Video
#define St_12_cm                DiscAttributes.Size12cm
#define St_Fashion              DiscAttributes.SizeFashion


volatile typedef struct
{
    BYTE DiagCmdEna:1;                  /* 1=Diag command enable */
    BYTE DiagReadData:1;                /* 1=Diag read data */
    BYTE HIFTransData:1;                /* 1=Diag read/write buffer transfer data  */
} DiagStT;
extern vBYTE  St_diag_HIFTransData;

//#define St_diag_HIFTransData    DiagSt.HIFTransData   /*Modify variable declaration(BIT -> BYTE)*/
#define St_diag_ena             DiagSt.DiagCmdEna
#define St_diag_rd_data         DiagSt.DiagReadData


volatile typedef struct
{
    BYTE CpyDone:1;                 /* 1 : Copy Completed */
    BYTE CmpDone:1;                 /* 1 : Compare Completed */
    BYTE PFilDone:1;                    /* 1 : Pattern Fill Completed */
    BYTE DCpyDone:1;                    /* 1 : Decrementing Copy Completed */
} CopyCmpStT;

#define St_copy_done            CopyCmpSt.CpyDone
#define St_comp_done            CopyCmpSt.CmpDone
#define St_pfil_done            CopyCmpSt.PFilDone
#define St_dcpy_done            CopyCmpSt.DCpyDone


extern ULONG   FlashLinearAdress;

/* Set Streaming Command(B6h) definition */
typedef struct {
    ULONG       StartLBA;       /* the first address For the performance request is being made                                   */
    ULONG       EndLBA;         /* the last address For the performance request is being made                                    */
    ULONG       RD_Size;        /* HOST AP's Buffer Size, in KBytes (Read Data Rate = (ReadSize / ReadTime))                     */
    ULONG       RD_Time;        /* time of HOST AP' Buffer becoming EMPTY, in msec                                               */
    ULONG       WR_Size;        /* HOST AP's Buffer Size, in KBytes (Write Data Rate = (WriteSize / WriteTime))                  */
    ULONG       WR_Time;        /* time of HOST AP' Buffer becoming FULL, in msec                                                */
} sStreamPara;


extern KnlFlagT     Iram_KnlFlag;       /* Kernal Flag Bit Field */
extern RwFlagT      Iram_RwFlag;        /* Read/Write Flag Bit Field */
extern LedStT       Iram_LedFlag;       /* Led Flag Bit Field */
extern DriveStT     Iram_DriveSt;       /* Drive Status Flag Bit Field */
extern DiscAttributesT  DiscAttributes; /* Disc Attributes(Flags) Bit Field */
extern DiagStT      DiagSt;             /* Diag Cmd Flag bit filed*/
extern CopyCmpStT   CopyCmpSt;          /* Copy/Comp Flag Bit Field */
extern vBYTE        Iram_St_fast_cache_hit;

//extern StSpecialSPDFlagT    StSpecialSPDFlag;

extern BYTE     mode_p5_data[MSENSE_PAGE_5_LEN];
extern BYTE     DeviceBusyEvent;    /* Device Busy event */
extern BYTE     DeviceBusyStatus;   /* Device Busy status */
extern USHORT   DeviceBusyTime;
extern BYTE     OperationalChangeTrayEvent;

#define GetWPM_Byte(n)      	mode_p5_data[n]      // Write Parameters (provided by Mode Select/Sense parameters page 5 from Host)
#define GSEN_OPE_NOTHING        0x00
#define GSEN_OPE_TRAY_IN        0x01
#define GSEN_OPE_TRAY_OUT       0x02

extern USHORT   Iram_usiRingSP;
extern USHORT   Iram_usiRingEP;
extern vUSHORT Xfer_Residue;                   /* (in absolute addressing mode) for Xfer Limit check */
#if (E_DIAGS == 1)
extern BOOL Radj_enable;
extern USHORT Radj_addr1;
extern USHORT Radj_addr2;
extern BYTE Radj_mask1;
extern BYTE Radj_mask2;
extern BYTE Radj_value1;
extern BYTE Radj_value2;
#endif

#if (WRITE_PROTECT == 1)
extern FLAG PersistentWriteProtect;
extern BOOL UpdatePWP;
#endif

#define bSSP		bit6					/* Software Settings Preservation */
#define bASYNNOTI	bit5					/* Asynchronous Notification */
#define bINTFPWR	bit3					/* Device-initiated interface power state transitions */
#define SATA_FE		SCSDRAM[IDENTIFY_ADR + (79*2)]
#define	bIPMREQ_P	bit3
#define bIPMACK_P	bit2
#define bIPMREQ_S	bit1
#define bIPMACK_S	bit0
#define SATA_STANDBYCNT	    90	/*f:100ms, 9 seconds */
/* If SATA is idle and DIPM Feature is enabled, system will count 4 seconds to enter */
/* PMREQ_partial mode. If SATA is still idle, system will enter PMREQ_slumber mode   */
/* after 5 seconds. The total waiting time to slumber mode is 9 seconds.             */

extern vBYTE Iram_SATA_Idle_counter;
extern vBYTE BIST_In_Process;
extern vBYTE SATA_intf_pwr_state;
extern vBYTE LinkTxRxErr;
#if (ENABLE_AUTO_ABORT == 1)
extern vBYTE	AutoAbort;
#endif

#if ((EN_AUTO_FIS34 == 0)|| (CHIP_REV <= 0xB3))
extern vBYTE	SendFis34Already;
extern vULONG  BGFis34Timer;
#endif
extern vBYTE	SATAConnect;

#if (BypassDecoderEr == 1)
/********************************************************************
*   When drivers occur decoder error, It shall ignore these error,
*   return original error data.
*********************************************************************/
extern BOOL IGNORE_DECODER_ERROR;
extern BYTE RetryLimit;            //limit for bypass Decode Err
extern ULONG RetryChgTime;        //Retry limit change timing
extern USHORT ReadGoodCnt;        //no PO C2 count
#endif

#if(BACKGROUND_READ_AHEAD == 1)
extern BYTE START_BACKGROUND_BUFFER;
#endif

/* These two variables are used by ataGetMediaStatus() in at_cmds.c */
extern vBYTE	ataKeyIsPressed;
extern vBYTE	ataMediaIsChanged;

extern vLONG	DecTimeCnt;
extern const BYTE VideoCDInformationTable[];
extern const BYTE VideoDVDInformationTable[];
extern const BYTE FlashDiscInformationTable[];
extern const BYTE SpecialDiscInformationTable[];
extern const BYTE VideoBDInformationTable[];
#if (Detect_AVCHD_DISC == 1)
extern const BYTE AVCHDInformationTable[];
#endif
extern ULONG WriteListStartPSN;

extern WORD PreCurHostPtr;
extern vSHORT  Pre_Buf_hxfer_blk;
#if (DVD_RAM_READ == 1)
#if (WORKAROUND_FOR_SEEDMISTHR == 1)
extern BOOL ChangeSeedMisThr;
extern BOOL ResetSeedMisThr;
extern BOOL RestSeedCount;
#endif
#if (ENABLE_RAM_DFE_ICE_DETECT == 1)
extern ULONG PrePID;
extern ULONG PreICEAdd;
extern ULONG CurICEAdd;
#endif
#endif
extern BYTE ServoPllInited;

#if (STORE_MSG_TO_SDRAM == 1)
//extern ULONG NextStoreMsgAddr;
extern ULONG MsgStAddr;                      //SDRAM message start address
extern ULONG MsgEdAddr;                     //SDRAM message end address
extern BOOL    EnableLogging;
extern BOOL    LoggingFull;
extern BOOL    DriveHangup;
#endif
#if (STORE_MSG_TO_FLASHROM==1)
extern BOOL    EnableAutoSaveLog;
extern BOOL    SaveLogToROMSwitch;
extern BOOL    SaveLogToROMatIdle;
#endif

//start ==To judge BD BCA valid or not.
#if(JudgeBDBCAValid == 1)
extern BOOL ReadBDBCAValid;
#endif

#if (EN_ADI == 1)
extern ULONG ADItimer;
extern BYTE ADICounter;
#endif

//end ==To judge BD BCA valid or not.
#endif /* __GP_RAM_H__ */
