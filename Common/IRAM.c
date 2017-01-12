/****************************************************************************
*                (c) Copyright 2001 - 2008 SUNEXT DESIGN, INC.
*                           All Right Reserved
*
*
*
* FILENAME:   IRAM.C
*
* DESCRIPTION
*
* This file contains Internal Variables which are allocated to the Internal RAM
* Always sort variables in the following order: Long, Short, Byte
* This eliminates the majority of packing bytes that may occur.
*
*
*   $Revision: 122 $
*   $Date: 11/01/18 10:30a $
*
**************************** SUNEXT CONFIDENTIAL *******************************/


#include ".\common\iram.h"
#include ".\servo\svo_step.h"

/**********************************************************************/
/* WARNING!!!                                                         */
/* The Internal Memory gives fast access but it is VERY expensive and */
/* the size is limited. Variables should be ordered carefully so that */
/* we do not have gaps. The ARM will pad to the nearest DWord, Word,  */
/* byte to guarantee even 32-bit address boundaries.                  */
/* Variables should be grouped as follows:                            */
/* LONG                                                               */
/* SHORT                                                              */
/* BYTE                                                               */
/**********************************************************************/


/* --------------------------------------------------------------------------*/
// Servo Variables are in Internal Memory for fast access
// Servo Variables structure is defined in SVO.H file.
struct  svoIramVarT     svoIramVar;
/* Servo bit mapped flags defined in Iram.h */
struct IramBoolT IramBool;
/* --------------------------------------------------------------------------*/

/**********************************
*            Flags                *
***********************************/
/* Bit Mapped flags for main functional blocks */
/* Refer to GP_RAM.H for bit definitions */
LedStT      Iram_LedFlag;
DriveStT    Iram_DriveSt;
RwFlagT     Iram_RwFlag;                /* Decoder Flags  */
KnlFlagT    Iram_KnlFlag;               /* Kernel Flags */
SvoFlagT    Iram_SvoFlag;               /* Servo Flags */

/**********************************
*        Host Interface           *
***********************************/
volatile CMD_BLOCK_T Iram_Cmd_block;    /* Host CDB Bytes - Command */
volatile SENSE_T Iram_Sense;            /* Request Sense return data */
volatile TransferS HostXfer;            /* Current host transfer information */

vULONG  Iram_Linear_Xfer_addr;          /* Linear Transfer Address */
vUSHORT Iram_shHstWdC;                  /* Shadow variable for HstWdC register */
vUSHORT Iram_shHstBytAC;                /* Shadow variable for HstBytAC register */
vULONG  Iram_shATB;                     /* Shadow variable for ATB register */
vUSHORT Iram_shHstBufPg;                /* Shadow variable for HstBugPg */
vUSHORT Iram_Hw_host_blks_limit;        /* Maximum number of blocks per transferred */
vUSHORT Iram_Xfr_allow;                 /* Number of Tranferrable bytes allowed in ATB registers */
vUSHORT Iram_Aud_xfer_subq_ptr = 0;     /* Current Audio Playback Page. Used to Extract and Save SubQ data */
vUSHORT Iram_Residue;                   /* Remaining bytes to transfer that are less than one block size */
vSHORT  Iram_Buf_hxfer_blk;             /* Number of pages to transfer to Host */
vBYTE   Iram_shHICtl;                   /* Shadow variable for HiCtl register */
vBYTE   Iram_shHDDir;                   /* Shadow variable for HDDir register */
vUSHORT Iram_shUMisc;                   /* Shadow variable for UMisc register */
vBYTE   Iram_RW_ECC_Status;             /* Error status */
BYTE Iram_EncoderStarted;

vBYTE   PreATAPICMD;                    /* To store the previous ATAPI command */
vULONG  PreReadCMDLba;                  /* To store the previous read start LBA */

/*** Event/Status Notification ***/
vBYTE   ataPowerMode;
vBYTE   SdramRefreshInterval ;// Save value for Power Control function
vBYTE   SdramRefreshIntervalSleep;

#if (STORE_MSG_TO_SDRAM == 1)
vULONG  Iram_Store_Msg_dram_addr;
#endif
/**********************************
*            Encoder              *
***********************************/
volatile TimeS      Iram_FoundRunOut1Time;  /* MSF for RunOut1 block */
volatile TimeS      Iram_FoundRunOut2Time;  /* MSF for RunOUt2 block */

/**********************************
*          Process                *
***********************************/
vUSHORT Iram_fTaskReq;
vBYTE   Iram_intercept_process;         // TBD?

/**********************************
*          Decoder                *
***********************************/
volatile ADDRESS_T  Iram_Buf_trg_blk;   /* Decoder Command Target Block Address */
volatile ADDRESS_T  Iram_Buf_ahead_blk; /* Decoder Command Number of Requested Block */

vULONG  Iram_Target_lba;                /* Read Target LBA */
vULONG  Iram_Cur_lba;                   /* Read Current LBA */
vULONG  Iram_startlba_ck;               /* FPacket Starting LBA Check */
vULONG  Iram_packetsize_ck;             /* FPacket Size Check */
vLONG   Iram_Audio_blk2_play;           /* Current Audio Play Block Pointer */

/*Workaroud for currrent time no continue*/
vLONG   CurWrCount;
vLONG   PreWrCount;
/*Workaroud for currrent time no continue*/

vUSHORT Iram_Audbuf_blkcnt;             /* Number of buffered Audio Blocks ready for Play */
vUSHORT Iram_Buf_ahead_bptr;            /* Read Ahead Buffer Pointer */
vUSHORT Iram_Sh_wbk;                    /* Copy of Disc Buffer Pointer */

vBYTE   Iram_Rw_read_mode;              /* Decoder mode of current disc */
vBYTE   Iram_yell_cnt;                  /* Keeps track of Consecutive # of YBM2 Detected blocks */
vBYTE   Iram_rd_ahead_chk_req = RD_AHEAD_BUF_NOP;

vBYTE   Iram_AtipAdipLppErrorAmount;    /* The amount of ATIP error per second in ATIP */
vBYTE   Iram_AtipAdipLppErrorCount;     /* ATIP/ADIP/LPP error count variable */
vBYTE   Iram_ECCBlockCount;

/**********************************
*          Read Data Buffer       *
***********************************/
volatile DataBufferS DataBuffer;        /* Data buffer information structure */

/**********************************
*            Kernel               *
***********************************/
vULONG  Iram_system_timer;
vULONG  Iram_timer3_counter;
vULONG  Iram_Knl_save_counter;

#if (SET_SLOT_IN == 1) //[Q36] for flag define
vBYTE DiscInFlag; //judge disc in or disc in retry
vBYTE DiscInRetryFlag; //judge disc in retry
vUSHORT SwitchFlag; //judge first and second stage disc in
vUSHORT DiscRetryFlag; //disc in retry accumulate
vBOOL   fgSRSTEj;//Jam when power on ,Disc auto tray out.
#endif

#if (USE_TWO_KEY_FUNCTION == 1)
vBYTE   Iram_PlayPressdTM;
#endif /* USE_TWO_KEY_FUNCTION */

vUSHORT Iram_Knl_timer_counter;         /* Kernel counter */
vBYTE  Iram_SATA_Idle_counter = 0;
vBYTE  fKernelAutoCompletionStatus;
vBYTE  fKernelSendCompletionStatus;
#if (SET_SLOT_IN == 1)
vBYTE        Iram_Auto_TrayIn_timer; //[Q36]Jam when power on ,Disc auto tray out.
#endif
vBYTE  Iram_LED_Flash_timer;           /* LED Flash counter based on Kernel Timer */
vBYTE  Iram_Gp_led_at_timer;
vBYTE  Iram_Gp_led_timer;              /* General Purpose LED Flash Timer value */
vBYTE  Iram_enable_kernel_timer = 0;
vBYTE  Knl_atcmd_avail;
vBYTE  Knl_cmd_avail;
vBYTE  St_cmd_inproc;
vBYTE  Knl_send_compl;
vBYTE  Knl_timeout;
vBYTE  Knl_IdentAvail;                  /* Flag to indicate a Identify Device data transfer Dev to Host */
vBYTE  Knl_Atapi_Avail;                 /* Flag to indicate a Atapi data transfer Dev to Host */
vBYTE  Ata_sub_burst;
vBYTE  St_send_blkdata;
vBYTE  Get_para_data;
vBYTE  St_send_pardata;
vBYTE  Knl_deci_set;
vBYTE  Rw_dec_stop;
vBYTE  Rw_tblk_reach;
vBYTE  Rw_ecc_error;
vBYTE  Rw_init_buffer;
vBYTE  St_audio_complete;
vBYTE  St_nx_audio;
vBYTE  St_audten_int;
vBYTE  St_audio_extract;
vBYTE  St_run_in_out;
vBYTE  fReadVerifyCommand;
vBYTE  Svr_pause_reqire;
vBYTE  Iram_fSubQIntFlag;
vBYTE  Iram_fATIPIntFlag;
vBYTE  Iram_StartMultiBlock;
vBYTE  Iram_reload;
vBYTE  Iram_St_fast_cache_hit;
vBYTE  Iram_EverFKNG;
vBYTE  Iram_BdWriteStatus = E_WRITE_STATUS_IDLE;

/**********************************
*            Player               *
***********************************/
public tFinalSessionInfoS   Iram_FinalSessionInfo;
volatile tDiscInfoS  Iram_DiscInfo;

/*Automatic change from 2.5K to 3k page for C2PO*/
//TDB? Only one of these variables are used in a _im.c file
vUINT   DATA_START_ADDR;           /* Data start address */
vUINT   AUX_START_ADDR;            /* Aux start address  */
vUSHORT DRAM_AUX_SIZE;             /* DRAM AUX Size */
vUSHORT RING_END_PAGE;             /* last page the SP can access*/
vUSHORT BUF_TOTAL_BLK_COUNT;       /* total pages */
vUSHORT AUX_HEADER_ADDR1;          /* M1 block header offset in page */
vUSHORT AUX_HEADER_ADDR2;          /* M2F1 block header offset in page */
vBOOL   Iram_Page_Size_3K_Flag;
/*Automatic change from 2.5K to 3k page for C2PO*/
vBYTE   Iram_Svr_player_mode;
vBYTE   Iram_Player_output[12];

vBOOL   Iram_Rd_subc_flag;          /* for read data with subcode */
vBOOL  Iram_Diff_Time_Flag;         /* for ATIP Time and Qcode PTime different */


/**********************************
*            Servo                *
***********************************/
vUSHORT Iram_write_value_D;
vUSHORT Iram_peak_value_D;
vUSHORT Iram_hold_value_D;
vULONG Iram_chkBHO;

/*** Stepper ***/
#if COLLI_SWITCH == 1
vUSHORT Iram_colli_StepPosition;
vSHORT  Iram_colli_CurrentPosition;
#endif
vSHORT  Iram_stp_CurrentPosition;       /* Stepper Current Positions [represent radius in the unit of sledge micro step (4.6875um)] */
vUSHORT Iram_stp_JumpDistance;
vUSHORT Iram_stp_CurrentDistance;
vUSHORT Iram_stp_BrakeDistance;         /* Stepper Braking  Distance */
vUSHORT Iram_stp_StepPosition;
vULONG  Iram_stp_MoveInterval; //(10^6 / 8192)us
vULONG  Iram_stp_CurrentTime;  //(10^6 / 8192)us
vBYTE   Iram_stp_Status;
vBYTE   Iram_stp_CurrentSpeedPhase;
vBYTE   Iram_stp_CurrentDirection;
#if DO_HW_SUPPORT_STEP == 1
vSHORT   StepMotorSupport_SLED;
#endif // DO_HW_SUPPORT_STEP
volatile eStepTimerMode Iram_stp_TimerMode;
volatile eStepProfile   Iram_stp_CurrentProfile;
volatile eInitialized   Iram_stp_Initialized;
#if (COLLI_SWITCH == 1)
volatile eInitialized   Iram_colli_Initialized;
#endif

/**********************************
*            Eject Key            *
***********************************/
struct ButtonInfo_S volatile EjectKeyInfo;

/**********************************
*            Misc                 *
***********************************/
volatile DiscSpeedT     Iram_fRequestedSpeed;
ULONG    Iram_atapi_timer;
enum {eRegiMax = 16};

vULONG  Iram_Buf_dram_addr;             // Temp address holder for WriteSDRAM()
ULONG   svoPrvtVar_delayTimer;          // delay_timer;
ULONG   svoPrvtVar_timeoutTimers[TIMEOUT_TIMER_MAX];    // time_out_timer[2];

#if (MSG_OUT_MODE == MSG_USBPORT)
ULONG       svoDebugMsgFlag;            /* svoDebbugMsgFlag */
ULONG       hifDebugMsgFlag;            /* svoDebbugMsgFlag */
#endif /* MSG_OUT_MODE == MSG_USBPORT */

#if Monitor_FPDO_Linearity ==1
ULONG  SampleTimer;
#endif

USHORT      Iram_usiRingSP;
USHORT      Iram_usiRingEP;

#if (ENABLE_RAM_DFE_ICE_DETECT == 1)
vBOOL  IceFound;
#endif
//Get event status notification fail of Command set
vUSHORT    STANDBYCNT;
vUSHORT    CMD_IDLE_CNT;                   /* no any command request from host*/



/*------------------------------------------------------------------------*/
/* The end of file                                                        */
/*------------------------------------------------------------------------*/
