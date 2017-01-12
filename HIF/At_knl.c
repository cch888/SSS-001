/****************************************************************************
*                (c) Copyright 2001 - 2008 SUNEXT DESIGN, INC.
*                           All Right Reserved
*
* FILENAME:   at_knl.c
*
* DESCRIPTION:
*   This file contains kernel and schedule routines
*
*   $Revision: 445 $
*   $Date: 11/05/03 3:42p $
*
************************ SUNEXT CONFIDENTIAL *******************************/

#include ".\common\globals.h"
#include ".\common\globtype.h"
#include ".\common\chip.h"
#include ".\common\error.h"
#include ".\common\system.h"
#include ".\servo\svo.h"    /* Please include no servo file except svo.h */
#include ".\hif\mmc.h"
#include ".\hif\gp_ram.h"
#include ".\hif\gp_equ.h"
#include ".\hif\at_equ.h"
#include ".\hif\gp_mac.h"
#include ".\hif\at_knl.h"
#include ".\hif\at_util.h"
#include ".\hif\gp_util.h"
#include ".\hif\at_cmds.h"
#include ".\hif\gp_spin.h"
#include ".\hif\gp_init.h"
#include ".\hif\sv_util.h"
#include ".\hif\bf_util.h"
#include ".\dvd\dvRdDisc.h"
#include ".\dvd\dvrdtrack.h"
#include ".\hif\s2b_main.h"
#include ".\hif\rd_util.h"
#include ".\hif\gp_key.h"
#include ".\hif\gp_inqry.h"
#include ".\hif\gp_rdsub.h"
#include ".\hif\gp_read.h"
#include ".\hif\gp_mdsns.h"
#include ".\hif\gp_rhead.h"
#include ".\hif\gp_seek.h"
#include ".\hif\gp_rdtoc.h"
#include ".\hif\gp_mchst.h"
#include ".\hif\cstart.h"
#if (AUDIO_PLAYBACK == 1)
#include ".\hif\nx_audio.h"
#endif
#include ".\hif\gp_gtpfm.h"
#include ".\hif\gp_rdfmt.h"
#include ".\hif\Gp_Buf.h"

#if EN_POWER_CONTROL
#include ".\hif\powerctl.h"
#endif

#include ".\hif\Rd_Stru.h"
#if BD_ENABLE
#include ".\bd\bdbufmgr.h"
#include ".\bd\bdRdTOC.h"
#include ".\defmgmt\bdwritable\bddfmg.h"
#include ".\defmgmt\bdwritable\bdWritableRead.h"
#include ".\bd\bdRdStru.h"
#include ".\bd\bdBufMgr.h"
#include ".\bd\bdSeek.h"
#include ".\bd\bdVars.h"
#include ".\bd\bdUtil.h"
#endif

#if (ENABLE_HDDVD == 1)
#include ".\hd\hdRdTOC.h"
#include ".\hd\hdRdDisc.h"
#endif

#if E_DIAGS == 1
#include ".\hif\diag.h"
#endif /* E_DIAGS */

#if (SUN_FUNC_TEST == 1)
#include ".\oem\sun710_diag.h"
#include ".\oem\sun710_function_test.h"
#include ".\oem\sun710_oem_tbl.h"
#endif

#include ".\player\plrplayr.H"

#include ".\hif\Gp_gtcfg.h"
#include ".\hif\Gp_ststr.h"
#include ".\hif\flash.h"

#if (DVD_RAM_READ == 1)
#include ".\defmgmt\common\mrw_ram.h"
extern void copy_mode5_data_to_sram(void);
#endif

#include ".\dvd\dvGvars.h"
#include ".\dvd\dvHostRd.h"
#include ".\dvd\dvSeek.h"
#include ".\dvd\dvmacros.h"
#include ".\dvd\dvRdStru.h"
#include ".\dvd\dvRptKey.h"
#include ".\dvd\dvSndKey.h"
#include ".\dvd\dvBufMgr.h"
#include ".\dvd\dvrdtoc.h"
#include ".\dvd\dvinit.h"

#if (DVD_AWG_TEST == TRUE)
#include ".\dvd\dvinit.h"
#endif

#if (MODULE_TEST == 1)
#include ".\hif\diag.h"
#endif

#include ".\al\reg_hal.h"
#include ".\al\reg_global.h"
#include ".\al\reg_periph.h"
#include ".\al\reg_host.h"
#include ".\al\reg_dvd_decoder.h"
#include ".\al\reg_dvd_encoder.h"
#include ".\al\reg_intctrl.h"
#include ".\al\reg_servo.h"
#include ".\al\reg_ssi.h"

#if PERIPHERAL_CLK_25MHZ == 1
#include ".\al\HAL_global.h"
#include ".\common\timer.h"
#endif /* PERIPHERAL_CLK_25MHZ */

#if (MSG_OUT_MODE == MSG_USBPORT && ENABLE_UART_RXD == 1)
#include ".\common\scomm.h"
#endif

#if (SET_SLOT_IN == 1)
#include ".\oem\oem_tbl.h" //[BTC][Q36]S28R2 detect
#endif

extern void BGCheckCommandAvalible(void);
extern void RecordHostDataProcess(void);
extern void DVDRecordDataProcess(void);
extern void SyncCacheProcess(void);
extern void DVDSyncCacheProcess(void);
extern void CDCloseProcess(void);             /* Close track/session sequence */
extern void DVDCloseProcess(void);

extern void CDEraseProcess(void);                 /* Erase sequence */
extern void CDFormatProcess(void);                /* Format sequence */

extern void UpdateSystemTimer(void);
extern void ataExecuteDiagnosticCommand(void);

extern USHORT timeUp_Group1;
extern vBYTE Iram_enable_kernel_timer;

static void ProcessDiscReadyCommand(void);
static void UpdateFirmwareCommand(void);
void ClearFlashUpWorkAreaAll(void);
#if (FLASH_BY_DISC == 1)
void SaveCurrentTransferSettings(BYTE Mode);  // Mode 0: normal , Mode 1: disc flush
extern BYTE FlashByDisc_flag;
#else
void SaveCurrentTransferSettings(void);
#endif
void DisableAllInterrupts( void );

void CheckCommandAvail(void);
vBOOL CheckATAPICommand(void);
void HIFStart(void);

void HIFMain(void);
void HIF2Main(void);
void TProcessMain(void);
void WRProcessMain(void);
void BGProcessMain(void);
void ReBuildCheckSum(void);

#if (USE_TWO_KEY_FUNCTION==1)
extern void KeyPlayAudio(BYTE flg);
#endif

#if(ENABLE_UART_RXD == 1)
    extern void exec_uart_cmd(BOOL check_trap);
#endif

extern BYTE const EndOfFirmWare[16];

#if (BD_ENABLE)
static void LoadDefectListCommand(void);
#endif /* BD_ENABLE */

#if (EN_DVD_HIGH_JITTER_DETECT  == 1)
extern BYTE	HIGH_JITTER_DISC;
#endif

#if (CEProduct == 1)
extern USHORT TimeUpForRead;
#endif

#if (SET_SLOT_IN == 1)
#define R2_Enable    0x04CL  //[BTC][Q36] for BP-S28R2 USE
BOOL fg_R2Profile   = FALSE; //[BTC][Q36] for BP-S28R2 USE
extern USHORT slot_level_check; //[BTC][Q36] for SLT002 disc in/out test
#endif
#if (SUN_FUNC_TEST == 1)
/*--------------------------------------------------------------------
  Name: CheckPersistentTable()
  Description:
  Inputs:
    none
  Outputs:
    none
  Returns:
    none
--------------------------------------------------------------------*/
#define CHK_PERSISTENT_PATTERN_OFFSET 0x400L     
void CheckPersistentTable()
{
    BYTE result;

    result = IsAacsKeyValid(FLASH_BASE + PARA_BLK_START_OFFSET + PARA_AACSKEY_OFFSET, FLASH_AACSKEY_SIZE);
    if(!result)
    {
        result = IsAacsKeyValid(FLASH_BASE + PARA_BLK_BKUP_START_OFFSET + PARA_AACSKEY_OFFSET, FLASH_AACSKEY_SIZE);
        if(result)
        {
            
            vBYTE *flash_base_addr,*flash_base_addr1;
        
            flash_base_addr  = (vBYTE *)FLASH_BASE + PARA_BLK_START_OFFSET + CHK_PERSISTENT_PATTERN_OFFSET;
            flash_base_addr1  = (vBYTE *)FLASH_BASE + PARA_BLK_BKUP_START_OFFSET + CHK_PERSISTENT_PATTERN_OFFSET;

            if((*flash_base_addr != 'S')&&(*(flash_base_addr+1) != 'U')&&(*(flash_base_addr+2) != 'N'))
            {
                if((*flash_base_addr1 == 'S')&&(*(flash_base_addr1+1) == 'U')&&(*(flash_base_addr1+2) == 'N'))
                {
                    SendMsgCn(1,0,0x800F11);//Normal PARA FAIL, Backup PARA PASS
                    OpenWritableSegmentTable(ePersistent_Backup_Talbe);
                    CloseWritableSegmentTable(ePersistent_Talbe);
                }
                else
                {
                    SendMsgCn(1,0,0x800F12);//ALL PARA table FAIL
                }
            }
            SendMsgCn(1,0,0x800F10);//Normal AACS KEY fail, Backup AACS Key PASS
            OpenWritableSegmentTable(ePersistent_Backup_Talbe);
            CloseWritableSegmentTable(eAACS_Table);
        }
        else
        {
            SendMsgCn(1,0,0x800F0F);//ALL AACS KEY fail
        }
    }
}
#endif
/*--------------------------------------------------------------------
  Name: HIFMain()
  Description:
    hif main routines
  Inputs:
    none
  Outputs:
    none
  Returns:
    none
--------------------------------------------------------------------*/
#if  (BURN_IN == 1 )
BOOL Burn_in_Enable = FALSE;
void BurnIn(void);
#endif

void HIFMain(void)
{
#if (SET_SLOT_IN == 1)
	 //[S07]if(GetPersistentTable1B(R2_Enable) == 0x01) //[BTC][Q36] for BP-S28R2 USE
	 {
	 	fg_R2Profile = TRUE;
	 }
#endif

#if  (BURN_IN == 1 )
    if(Burn_in_Enable)
    {
        BurnIn();
    }
#else
		if(ipAGING == 0)//[BTC][Q36] for Aging test
		{
				send_msg5S(1,"BurnIn");    
		#if (Customer_A1 ==1)
				AgingMode = TRUE;
	  		BurnIn();
				AgingMode =FALSE;
		#else
    		BurnIn();
		#endif
		}	
    else
   	{   	
   			WRITE_REG(HAL_GLOBALREG_MAP->PBMUX1, 0x00);//[J50C]EMI 300MHz
    		send_msg5S(1,"ipHOLD == 1"); 
   	}
#endif // BURN_IN

    SCStartUp();

#if (VCPS_TEST == 1)
    DecryptVCPSKeys();
#endif

    ENABLE_INTERRUPT();

#if (SET_SLOT_IN == 1)  //[BTC][Q36]Jam when power on ,Disc auto tray out.
		if(((ipOpenSW == 1)&&(ipSW2 == 1)&&(ipCloseSW == 0)) ||
			((ipOpenSW == 0)&&(ipSW2 == 1)&&(ipCloseSW == 0)))
		{
				SendMsg5S(1, "Flag Up ");
				Iram_Auto_TrayIn_timer = 5000/20;  // 5sec
				fgSRSTEj = TRUE;
		}	
		else//[W.0E]¥X¤O¾¦½üÂk¦ì
		{
			 	SetMotorDriverState(1);//LODER_MUTE_OFF
		   	svoTray_OutputDac(150);
		   	DelaymS(150);
		   	SetMotorDriverState(0);//ALL_MUTE_ON
   		 	svoTray_OutputDac(0);
		}   
#endif	
#if (MuteSSI == 1)//[S07][U3]Jam disable SCLK & SDATA & SEN for U3 EMI 17MHz from Debug Board
	WRITE_FIELD(HAL_APB_SSI_SVSTOP, 1);
	#define WriteSSIReg_1B(addr, data) (*(vBYTE *)(HAL_APB_SSI_BaseAddress + addr) = (data))
	WriteSSIReg_1B(ADAPTER_ADDR_1,0x00);
	
	if((Inquiry_tbl[32]=='W')&&(Inquiry_tbl[33]=='.')&&(Inquiry_tbl[34]=='0')&&(Inquiry_tbl[35]=='N'))
  {
			if((GetPersistentTable1B(0x48) != 0x01)||(GetPersistentTable1B(0x4A) != 0xFD))//[W.0H]set Reg.2 for U3
			{
					OpenWritableSegmentTable(0x00);
					SetPersistentTable1B(0x48,0x01);
					SetPersistentTable1B(0x49,0x04);
					SetPersistentTable1B(0x4A,0xFD);
					CloseWritableSegmentTable(0x00);
			}
	}		
#endif

#if (SUN_FUNC_TEST == 1)
    CheckPersistentTable();//Added code for check persistent table data.    
    FunctionTest();
#endif

    for(;;)
    {
        HIFStart();
        ExitProcess();
    }
}


/*--------------------------------------------------------------------
  Name: HIF2Main()
  Description:
    buffer main routines
  Inputs:
    none
  Outputs:
    none
  Returns:
    none
  Note: each process does not execute at the same time
--------------------------------------------------------------------*/
void HIF2Main(void)
{
#if (DVD_RAM_READ == 1)
    if((pdbServoGetfDiscKind() == eDVDRAMDisc) && RWDisc.dvRAM.BgPreBuffer == 1)
    {
        BYTE result;

        RWDisc.dvRAM.PrePage = DVDRAM_GET_SEG_PAGE(RWDisc.dvRAM.PreSegNo);
        send_msg5SValue(SEND_MRW_MSG, "pSgLSN", 4, RWDisc.dvRAM.PreSegNo<<24|RWDisc.dvRAM.PreLSN & 0x00ffffff);
        result = DVDRAMStartPrebuffering(RWDisc.dvRAM.PrePage, RWDisc.dvRAM.PreLSN, (DVDRAM_BUF_TOTAL_BLK_COUNT - PREBUF_THRESHOLD - 0x20)/ONE_DVD_BLOCK, eRAM_READ_MODE);
        RWDisc.dvRAM.BgPreBuffer = 0;
        send_msg5SValue(SEND_MRW_MSG, "pBuf rut", 2, RWDisc.dvRAM.BgPreBufAbort<<8 | result);

        if(RWDisc.dvRAM.BgPreBufAbort)
            RWDisc.dvRAM.BgPreBufAbort = 0;
    }
#endif
}


/*--------------------------------------------------------------------
  Name: WRProcessMain()
  Description:
    write main routines
  Inputs:
    none
  Outputs:
    none
  Returns:
    none
  Note: each process does not execute at the same time
--------------------------------------------------------------------*/
void WRProcessMain(void)
{

    ENABLE_INTERRUPT();
    for(;;)
    {
        ExitProcess();
    }
}


/*--------------------------------------------------------------------
  Name: TProcessMain()
  Description:
    system timer main routines
  Inputs:
    none
  Outputs:
    none
  Returns:
    none
--------------------------------------------------------------------*/
void TProcessMain(void)
{
    ENABLE_INTERRUPT();
    for(;;)
    {
        UpdateSystemTimer();

        BGCheckCommandAvalible();                      /* Check Command Avalible in Back Ground */

#if (SUN_FUNC_TEST == 1)
        if(FTInfo.DiableLEDController == FALSE)
            LEDController(&FTLEDControlInfo);
#endif

#if (DVD_RAM_READ == 1)
    if((pdbGetDiscKind() == eDVDRAMDisc) && RWDisc.dvRAM.BgPreBuffer == 1)
    {
        BYTE result;
		send_msg5SValue(SEND_MRW_MSG, "Bille", 4, pdbServoGetfDiscKind());

        RWDisc.dvRAM.PrePage = DVDRAM_GET_SEG_PAGE(RWDisc.dvRAM.PreSegNo);
        send_msg5SValue(SEND_MRW_MSG, "pSgLSN", 4, RWDisc.dvRAM.PreSegNo<<24|RWDisc.dvRAM.PreLSN & 0x00ffffff);
        result = DVDRAMStartPrebuffering(RWDisc.dvRAM.PrePage, RWDisc.dvRAM.PreLSN, (DVDRAM_BUF_TOTAL_BLK_COUNT - PREBUF_THRESHOLD - 0x20)/ONE_DVD_BLOCK, eRAM_READ_MODE);
        RWDisc.dvRAM.BgPreBuffer = 0;
        send_msg5SValue(SEND_MRW_MSG, "pBuf rut", 2, RWDisc.dvRAM.BgPreBufAbort<<8 | result);

        if(RWDisc.dvRAM.BgPreBufAbort)
            RWDisc.dvRAM.BgPreBufAbort = 0;
    }
#endif

 #if (BD_ENABLE == 1)
    if (pdbDiscLoaded() == BD_DISC)
    {
        //send_msg5S(SEND_MRW_MSG, "elvis t1");
        if (BdwtDisc.BgPreBuffer == 1)
        {
            PSNs StartPSN;
            BYTE result;

	 #if (EN_POWER_CONTROL == 1)
            if (CurrentPowerMode == eStandbyPower)
           {
               SetDiscReadPowerMode();
           }
        #endif

            //PreSegNo, PreLSN are increased in bdISR when we appended the buffered cluster last time
            BdwtDisc.PrePage = BD_GET_SEG_PAGE(BdwtDisc.PreSegNo);
            StartPSN = bdConvertLBAtoPSN(BdwtDisc.PreLSN);

            send_msg5SValue(SEND_MRW_MSG, "pSgLSN", 4, BdwtDisc.PreSegNo<<24|BdwtDisc.PreLSN & 0x0fffffff);
            result = BDReadDataFromDisc(BdwtDisc.PrePage, StartPSN.PSN, BdwtDisc.PreLSN,
            (eBDCacheSegmentCnt - BD_CACHE_RESERVED_CNT), ePRE_BUFFERING);

            BdwtDisc.BgPreBuffer = 0;
            send_msg5SValue(SEND_MRW_MSG, "pBuf result", 2, BdwtDisc.BgPreBufAbort<<8 | result);

            if(BdwtDisc.BgPreBufAbort)
                BdwtDisc.BgPreBufAbort = 0;
        }
    }
#endif

        ExitProcess();
    }
}

/*--------------------------------------------------------------------
  Name: BGProcessMain()
  Description:
    background process main routines
  Inputs:
    none
  Outputs:
    none
  Returns:
    none
--------------------------------------------------------------------*/
void BGProcessMain(void)
{
    ENABLE_INTERRUPT();
    for(;;)
    {
        HIF2Main();
        ExitProcess();
    }
}

/*--------------------------------------------------------------------
  Name: HIFStart()
  Description:
    Starting function from HIFMain process. This function will call
    CheckCommandAvail() to get command from host
  Inputs:
    none
  Outputs:
    none
  Returns:
    none
--------------------------------------------------------------------*/
extern ULONG CmdSeekTimer;
extern BYTE CmdSeekOk;

#if (CDPlayability == 1)
extern BYTE CDBufferAlmostEmpty;
#endif

void HIFStart(void)
{
#if (WITHOUT_SERVO == 0)
    ULONG Mount_time;
    TrayManager();
#endif // WITHOUT_SERVO

    if ( Iram_Svr_player_mode == TRAY_IN_MODE )
    {
#if (WITHOUT_SERVO == 0)
        StartTimer(&Mount_time);
        TrayinInitial();           // Move tray in and spin up.
    #if (MONITOR_OPU_TEMPERATURE == 1)
        SendMsgCn(SHOW_DEBUG_MSG,1,0x38001D,OpuTemperature);
    #endif
        SendMsgCnB(SEND_SStpTask_MSG,4,0xB00210,B4B(ReadTimer(&Mount_time)));   // Mount time = %4d (mS)
        svoSeekVar.rx_available = RX_AVAILABLE;
    #if (MODULE_TEST == 1)
        ModuleTest();
    #endif  //(MODULE_TEST == 1)

#else

    #if (DVD_AWG_TEST == TRUE)
        DvdAwgInit();          // Set up everything for AWG as if there were a DVD in the tray.
    #else
        St_no_disc = TRUE;
        St_become_ready = FALSE;
        St_drv_ready  = FALSE;
        Iram_Svr_player_mode = START_UP_MODE;
        Maxlba = 0x10000;
    #endif
#endif
    }
#if EPO_SLOT_IN
                                       //no disc  L L H
    if(!indet&&(!ipOpenSW)&&ipCloseSW)  //put disc H L H
    {
        if (disc_inside == 1)
        {
            disc_outside = 1;
            DelaymS(1);
        }
        else
        {
#if (SUN_FUNC_TEST == 1)
            /* If tray-in fail over MaxSlotInRetryCnt and stop to tray-in. */
            if (TrayIn_Error < MaxSlotInRetryCnt)
#endif    
            {
            disc_outside = 0;
            svoIramVar.emergencyStopFlag = TRUE;
            TrayManagerInfo.Command = TRAY_COMMAND_TRAY_IN;
        }
    }
    }
    else if((!ipCloseSW)&&(disc_inside != 1))
    {
        disc_outside = 0;
        disc_inside = 1;
#if (SUN_FUNC_TEST == 1)
        TrayIn_Error = 0;//clear error count
#endif
    }
    else if((indet)&&(!ipOpenSW)&&ipCloseSW)
    {
        disc_outside = 0;
        disc_inside = 0;
#if (SUN_FUNC_TEST == 1)
        /* After tray-in fail, and detect new insert to clear error count */
        if (TrayIn_Error >= MaxSlotInRetryCnt)
            TrayIn_Error = 0;//clear error count
#endif
    }
    else if((!indet&&ipOpenSW&&ipCloseSW)&&(disc_outside == 1))
    {/* if start stop unit command in process, no need to change tray command according to the tray switch */
        if (!((St_cmd_inproc)&&(Iram_Cmd_block.bf[0] == SBC_START_STOP_UNIT)))
    {
        disc_outside = 0;
        svoIramVar.emergencyStopFlag = TRUE;
        TrayManagerInfo.Command = TRAY_COMMAND_TRAY_IN;
    }
    }
#endif

#if (AUDIO_PLAYBACK == 1)
    if (St_nx_audio)
    {
        MonitorAudioNx();
    }
#if (USE_TWO_KEY_FUNCTION==1)
    else if (gSt_play_sw)
    {
        /* H/W Play key is pressed. */
        if (St_drv_ready)
        {
                KeyPlayAudio(0);
                gSt_play_sw = 0;
        }
        else
                gSt_play_sw = 0;
    }
#endif  /* USE_TWO_KEY_FUNCTION */
#endif // AUDIO_PLAYBACK

    /*-----------------------------------------------------
        After BD/HD/DVD Disc get seek command
        During 5 sec did not get another seek/read command,
        than call pause
    -----------------------------------------------------*/
    if ((CmdSeekOk == 1)&&(ReadTimer(&CmdSeekTimer)> 5000L))
    {
        Svr_pause_reqire = TRUE;
        CmdSeekOk = 0;
    }

    if (Svr_pause_reqire)                   /* if pause required */
    {
#if WITHOUT_SERVO == 0
        if(St_no_disc == FALSE)             /* Is disc inside? */
        {
            svoIramVar.knlTimeupCount = 50; //set timeout 1 sec for svo pause
            ServoPause();                   /* execute pause operation */
        }
        else
#endif
            Svr_pause_reqire = FALSE;       /* No disc inside, clear the pause request */
    }

    if (Knl_cmd_avail == TRUE)
    {
        CheckCommandAvail();
    }

    svoIramVar.knlTimeupCount = timeUp_Group1;

#if(ENABLE_UART_RXD == 1)
    exec_uart_cmd(0);
#endif

    /*-------------------------------------------------------------------
       Check for speed change.
    --------------------------------------------------------------------*/
    if((Iram_Svr_player_mode!=EXECUTING_TRAY_OUT_MODE) && (Iram_Svr_player_mode!=TRAY_OUT_MODE)
        && (Iram_Svr_player_mode!=STOP_MODE) && (Iram_Svr_player_mode!=EXECUTING_STOP_MODE))
    {
        svoIramVar.knlTimeupCount = 50; //set timeout 1 sec for svo speed change
        Set_SpeedMDCont();
        CheckReadAhead();
    }

    //if SaveLogToROMSwitch = TRUE, save log at idle.
#if (STORE_MSG_TO_FLASHROM==1)
    if((Iram_Svr_player_mode == PAUSING_MODE)&&(SaveLogToROMatIdle == TRUE))
    {
        SendMsg80(SHOW_DEBUG_MSG,0x33FF28); //saveLog
        SaveLogToROMatIdle = FALSE;
        CopyMsgFromDataAreaToROM();
    }
#endif
}


/*--------------------------------------------------------------------
  Name: ResetModules()
  Description:
    Resets encoder and decoder;
  Inputs:
    none
  Outputs:
    none
  Returns:
    none
--------------------------------------------------------------------*/
void ResetModules(void)
{
    /* Reset CD Decoder/Ecnoder */
    WRITE_FIELD(HAL_GLOBAL_ENCSRST,1);
    WRITE_FIELD(HAL_GLOBAL_DECSRST,1);
    WRITE_FIELD(HAL_GLOBAL_ENCSRST,0);
    WRITE_FIELD(HAL_GLOBAL_DECSRST,0);
    ResetCDDecoder();
}


/*--------------------------------------------------------------------
  Name: SCStartUp()
  Description:
    Executes the power up procedure and initialize the system.
  Inputs:
    none
  Outputs:
    none
  Returns:
    none
--------------------------------------------------------------------*/
BYTE dt;
extern BYTE bottom_of_heap;                             /* defined in heap.s */
void SCStartUp(void)
{
    BYTE FLASH_UP, cnt, rebootData[16];

#if (STORE_MSG_TO_SDRAM == 1)
    Iram_Store_Msg_dram_addr = LOGGING_MSG_START_ADDR;
    MsgStAddr = LOGGING_MSG_START_ADDR;
    MsgEdAddr   = LOGGING_MSG_END_ADDR;
    EnableLogging = TRUE;
    LoggingFull = FALSE;
    DriveHangup = FALSE;
#endif
#if (STORE_MSG_TO_FLASHROM==1)
    SaveLogToROMatIdle = FALSE;
    EnableAutoSaveLog = FALSE;
    SaveLogToROMSwitch = FALSE;
#endif
    FLASH_UP = 0;
    St_unit_atten = TRUE;

    Iram_Hw_host_blks_limit = HW_MAX_MULTB; /* init. set feature info */

    HAL_GLOBAL_SetSrcAFERefClock(halGLOBAL_AFERefCLKS_SPLL33MHz);

#if PERIPHERAL_CLK_25MHZ == 1
    HAL_GLOBAL_SetSrcARMPerRefClock(halGLOBAL_ArmPRefCLKS_SATA25MHz); /* Set peripheral clock to Xtal 25 MHz */
#endif /* PERIPHERAL_CLK_25MHZ */

    InitializeVariables();              /* init. variables */
    InitializeAllTimers();              /* init. CPU timers */

    /* for restart after flash finished */
    {
        Iram_Buf_dram_addr = TRAN_MODE_INFO_ADR;
        for ( cnt = 0; cnt < TRAN_MODE_INFO_LEN; cnt++ )
        {
            rebootData[cnt] = ReadSDRAM();
        }

        /*Normal Flash Update Command Complete!*/
        if ( (rebootData[0] == 'F') && (rebootData[1] == 'l') && (rebootData[2] == 'a') && (rebootData[3] == 's')
          && (rebootData[4] == 'h'))
        {
            St_unit_atten = FALSE;
            FLASH_UP = 1;
            bottom_of_heap = 1;
#if (FLASH_BY_DISC == 1)
            if (rebootData[5] == 'D')
            {
                FlashByDisc_flag = 1; //set this flag, and do tray after trayininitial(). //servo can't trayout before state_init_end!!
            }
#endif
        }
    }

    rebootData[0] = READ_REG(HAL_HSTREG_MAP->ATINT);  /*Task File Byte2*/
    rebootData[1] = READ_REG(HAL_HSTREG_MAP->ATSPA);  /*Task File Byte3*/
    rebootData[2] = READ_REG(HAL_HSTREG_MAP->ATBLO) >> 8;/*Task File Byte4 5*/
    rebootData[3] = READ_REG(HAL_HSTREG_MAP->ATBLO);/*Task File Byte4 5*/

    if ( (rebootData[0] == 'F') && (rebootData[1] == 'l') && (rebootData[2] == 'a') && (rebootData[3] == 's'))
    {
        /* makeing save data : Flag & Variable setting */
        rebootData[7]  = 0;                      /* 1=DMA feature enabled, 0=DMA feature disabled */
        rebootData[8]  = 0;                        /* 1=multiple_word DMA, 0= single_word DMA */
        rebootData[9]  = 0;                        /* 1= ultra DMA mode   */
        rebootData[10] = 0x00;      /* Maximum blocks to xfr due to chip limitation  */
        rebootData[11] = 0x1F;

        send_msg80(1,0x710080);
        St_unit_atten = FALSE;
        FLASH_UP = 1;
        bottom_of_heap = 1;
    }

    WRITE_REG(HAL_HSTREG_MAP->ATINT, 0x01);     /* Interrupt reason reg */
    WRITE_REG(HAL_HSTREG_MAP->ATSPA, 0x01);     /* sector number reg   */
    WRITE_REG(HAL_HSTREG_MAP->ATBLO, 0xEB14);   /* BYTE count reg  */

    //In SATA the drive is Master ONLY!
    WRITE_REG(HAL_HSTREG_MAP->CTRLS, HAL_HST_SBSYCTL|HAL_HST_SCHKC|HAL_HST_SABRTC);
    WRITE_REG(HAL_HSTREG_MAP->ATFEA,  0x01);
    WRITE_REG(HAL_HST_DRDYW_REGISTER,  0x00);   /* set DRDY, DSC */

    InitializePlayer();                             /* init. player, servo, subcode */
    InitializeTables();                             /* init. all tables in the DRAM */

    St_dma_feature = TRUE;
    St_multi_dma   = FALSE;
    St_ultra_dma   = TRUE;              /* assume Ultra DMA */

    WRITE_REG(HAL_HSTREG_MAP->RXFIFOC, 0x08);
    Iram_shHDDir &= 0xF3;
    Iram_shHDDir |= HAL_HST_DMAMODE;
    WRITE_REG(HAL_HSTREG_MAP->HDDIR, Iram_shHDDir);

    if (FLASH_UP == 1)
    {
    /*--- case of boot after Flash Up ---*/
        /* Flag ReSetting */
        St_unit_atten = FALSE; /* clear unit attentation flag */
        St_dma_feature        = rebootData[7];
        St_multi_dma          = rebootData[8];
        St_ultra_dma          = rebootData[9];

        if ( (St_dma_feature == TRUE) && (St_ultra_dma == TRUE) )       /* Check for UDMA */
        {
                WRITE_REG(HAL_HSTREG_MAP->RXFIFOC, 0x08);
                Iram_shHDDir &= 0xF3;
                Iram_shHDDir |= HAL_HST_DMAMODE;
                WRITE_REG(HAL_HSTREG_MAP->HDDIR, Iram_shHDDir);
        }
        else if ( (St_dma_feature == TRUE) && (St_ultra_dma == FALSE) )     /* MWDMA */
        {
            Iram_shHDDir &= 0xF3;
            Iram_shHDDir |= HAL_HST_DMAMODE;
            WRITE_REG(HAL_HSTREG_MAP->HDDIR, Iram_shHDDir);
        }
        else                                                                /* PIO */
        {
            Iram_shHDDir &= 0xF3;
            WRITE_REG(HAL_HSTREG_MAP->HDDIR, Iram_shHDDir);
        }

        /* Valiable ReSetting */
        BHIGH(Iram_Hw_host_blks_limit) = rebootData[10];
        BLOW (Iram_Hw_host_blks_limit) = rebootData[11];

        /* Identify Table ReSetting */
        Iram_Buf_dram_addr = IDENTIFY_ADR + (62 * 2 + 1);
        WriteSDRAM(rebootData[12]);
        Iram_Buf_dram_addr = IDENTIFY_ADR + (63 * 2 + 1);
        WriteSDRAM(rebootData[13]);
        Iram_Buf_dram_addr = IDENTIFY_ADR + (88 * 2 + 1);
        WriteSDRAM(rebootData[14]);

    }


    /* clear DRAM scratch area */
    Iram_Buf_dram_addr = TRAN_MODE_INFO_ADR;
    for ( cnt = 0; cnt < TRAN_MODE_INFO_LEN; cnt++ )
    {
        WriteSDRAM(0x00);
    }

    svoIramVar.knlTimeupCount = timeUp_Group1;
    Iram_Svr_player_mode = TRAY_IN_MODE;
    St_door_open = FALSE;           /*  door in */
    St_become_ready = TRUE;

    pdbServoSetDiscDiameter(eNoDisc);

/* Enable Group Interrupts */
    ENABLE_DINT();                  /* enable decoder int */
    ENABLE_CPYINT();                /* enable copy function int */
    ENABLE_AUDINT();                /* enable audio int */
    ENABLE_HINT();                  /* enable host int */
    ENABLE_DVD_DINT();              /* enable dvd decoder int */

#if (EN_LS == 1)
    ENABLE_LSINT();
#endif // End of (EN_LS == 1)

    ENABLE_DFEINT();

    ENABLE_OPCINT();

    Iram_enable_kernel_timer = 1;   /* enable kernel timer int */

    BUILD_SENSE(0x00,0x00,0x00,0x03);    /* NO ADDITIONAL SENSE INFO */

    if (FLASH_UP == 1)
    {
        SendStatus();
        WRITE_REG(HAL_HSTREG_MAP->ATRG,HAL_HST_SCTRG); /* after flash complete then return Command Complete to host */
    }
    else
    {
        ReBuildCheckSum();
        WRITE_REG(HAL_HSTREG_MAP->ATRG,HAL_HST_CTTRG);    /* Set ATAPI signature */
        ATAClearBusy();                 /* clr busy */
    }
    ENABLE_INTERRUPT();
    Iram_AddBufferingFlg = FALSE;
}


/*--------------------------------------------------------------------
  Name: CheckCommandAvail()
  Description:
    This routine processes host command and pre-check any error condition.
    If error exists, routine will jump to command completion routine and
    return check status to host. Otherwise, it will go to command handler
    routine and process corresponding command.

    Request Sense and Inquiry will never return error status to host
  Inputs:
    none
  Outputs:
    none
  Returns:
    none
--------------------------------------------------------------------*/
void CheckCommandAvail (void)
{
    // Enable this code for debugging only! It shows the ATAPI command received in ENGLISH
    //                                      as well as the entire CDB
//  send_msg80(SEND_ATAPI_CMD,0x50,0x02,Iram_Cmd_block.bf[0]);                                                          // Display ATAPI command in ENGLISH
//  send_msg54(SEND_ATAPI_CMD,Iram_Cmd_block.bf[0],Iram_Cmd_block.bf[1],Iram_Cmd_block.bf[2],Iram_Cmd_block.bf[3]);     // Display CDB 0,1,2,3
//  send_msg54(SEND_ATAPI_CMD,Iram_Cmd_block.bf[4],Iram_Cmd_block.bf[5],Iram_Cmd_block.bf[6],Iram_Cmd_block.bf[7]);     // Display CDB 4,5,6,7
//  send_msg54(SEND_ATAPI_CMD,Iram_Cmd_block.bf[8],Iram_Cmd_block.bf[9],Iram_Cmd_block.bf[10],Iram_Cmd_block.bf[11]);   // Display CDB 8,9,10,11

    if (Knl_cmd_avail == FALSE)
        return;

    if ( St_nx_audio || (Iram_Svr_player_mode == PLAY_TRACK_MODE) )
        CheckAudioOperation();

    Knl_cmd_avail = FALSE;
    Knl_send_compl = FALSE;
#if (AUTO_STATU_CPLT== 1)
    fKernelSendCompletionStatus = FALSE;
#endif
    if (Knl_atcmd_avail == TRUE)            /* for ATA command process */
    {
        Knl_atcmd_avail = FALSE;
        St_cmd_inproc = TRUE;
        svoIramVar.knlTimeupCount = TIMEUP_10S; /* 10s */
        switch ( Iram_Cmd_block.bf[0] )
        {
            case ATA_IDENTIFY_PACKET_DEVICE:
                ataIdentifyPacketDeviceCommand();
                break;
            case ATA_IDLE_IMMED:
            case ATA_IDLE:
                ataIdleImmedCommand();
                break;
            case ATA_STANDBY_IMMED:
                ataStandbyImmedCommand();
                break;
            case ATA_SLEEP:
                ataSleepCommand();
                break;
            case ATA_CHECK_POWER_MODE:
                ataCheckPowerModeCommand();
                break;
            case ATA_SET_FEATURE:
                ataSetFeatureCommand();
                break;
            case ATA_STANDBY:
                ataStandbyCommand();
                break;
            case ATA_GET_MEDIA_STATUS:
                ataGetMediaStatus();
                break;
            case ATA_FLUSH_CACHE:
                ataFlushCacheCommand();
                break;
            default:
                ATACommandFail();
        }
        return;
    }

#if (EN_LS == 1)
    if (pdbGetDiscKind() == eLSDisc)
    {
        switch(Iram_Cmd_block.bf[0])
        {
            case MMC_READ_DISK_INFO:
            case MMC_READ_TOC:
            case MMC_READ_CAPACITY:
            case MMC_READ_FORMAT_CAPACITIES:
                BUILD_SENSE(0x05,0x30,0x00,0x60);    /* INCOMPATIBLE MEDIUM INSTALLED */
                AutoCommandComplete();
                return;
            default:
                break;
        }
    }
#endif // End of (EN_LS == 1)

    /* Process ATAPI command. First, check for commands that do not
    report unit attention or drive not ready: Request Sense, Inquiry, */
    if (CheckATAPICommand() == FAIL)
    {
        InvalidCommand();
        return;
    }

    St_cmd_inproc = TRUE;

    if (Iram_Cmd_block.bf[0] == SPC_REQUEST_SENSE)
    {
        if(St_unit_atten == TRUE)
        {
            BUILD_SENSE(0x06,0x29,0x00,0x04);
            St_unit_atten = FALSE;
            st_disc_change = FALSE;
        }
        RequestSenseCommand();
        return;
    }

    BUILD_SENSE(0x00,0x0,0x00,0x00);

    /* commands that ignore unit attention */
    switch(Iram_Cmd_block.bf[0])
    {
        case SPC_INQUIRY:
            InquiryCommand();
            return;
        case MMC_GET_CONFIGURATION:         /* 0x46 */
            GetConfigurationCommand();
            return;
    }

    /* POWER ON, RESET, OR BUS DEVICE RESET OCCURED */
    if(St_unit_atten == TRUE)
    {
        //start == Start/Stop Unit and PreventAllowMediumRemoval command don't return Unit attention sense code.
        if((Iram_Cmd_block.bf[0] == SBC_START_STOP_UNIT) || (Iram_Cmd_block.bf[0] == SPC_PREVENT_ALLOW_MEDIUM_REMOVAL))
        {
            St_unit_atten = FALSE;
            st_disc_change = FALSE;
        }
        else
        //end ==
        {
            BUILD_SENSE(0x06,0x29,0x00,0x05);
            St_unit_atten = FALSE;
            st_disc_change = FALSE;
            AutoCommandComplete();
            return;
        }
    }

    /* CD change */
    if(st_disc_change == TRUE)
    {
        //Drive receives first ATAPI Start/Stop Unit Cmd tray out mode to return sense key after Servo startup complete.
        //Start/Stop Unit and PreventAllowMediumRemoval command don't return Unit attention sense code.
        if((Iram_Cmd_block.bf[0] == SBC_START_STOP_UNIT) || (Iram_Cmd_block.bf[0] == SPC_PREVENT_ALLOW_MEDIUM_REMOVAL))
        {
            st_disc_change = FALSE;
        }
        else
        {
            BUILD_SENSE(0x06,0x28,0x00,0x06); /* NOT READY TO READY CHANGE, MEDIUM MAY HAVE CHANGED */
            st_disc_change = FALSE;
            AutoCommandComplete();
            return;                     /* return to Main_loop; */
        }
    }

    /* Check for commands that don't report Drive not Ready */
    switch(Iram_Cmd_block.bf[0])
    {
        case SPC_READ_BUF:
            ReadBufferCommand();
            return;
        case SPC_WRITE_BUF:
            WriteBufferCommand();
            return;
        case FLASH_CODE:
            UpdateFirmwareCommand();
            AutoCommandComplete();
            return;

#if (BD_ENABLE)
        case DIAG_CMD_LOAD_DFL:
            LoadDefectListCommand();
            AutoCommandComplete();
            return;
#endif /* BD_ENABLE */

        case MMC_MECHANISM_STATUS:
            MechanismStatusCommand();
            return;
        case MMC_GET_EVENT_STATUS_NOTIFICATION:
            GetEventStatusNotificationCommand();
            return;
        case SPC_MODE_SENSE:
            ModeSenseCommand();
            return;
        case SPC_MODE_SELECT:
            ModeSelectCommand();
            return;
        case MMC_SET_CD_SPEED:
#if(EN_DVD_HIGH_JITTER_DETECT ==1)
            HIGH_JITTER_DISC &= ~0x02;
#endif
            SetCDSpeedCommand();
            return;
        /* commands that ignore unit attention */
        case MMC_GET_PERFORMANCE:           /* 0xAC */
            GetPerformanceCommand();
            return;
        case MMC_READ_FORMAT_CAPACITIES:
            ReadFormatCapacitiesCommand();
            return;
    }

    /* Check drive ready  */
    if (St_become_ready == TRUE)
    {
        /* DRIVE IS BECOMING READY */
        switch(Iram_Cmd_block.bf[0])
        {
            case SBC_START_STOP_UNIT:
            case MMC_PLAY_AUDIO_10:
            case MMC_PLAY_AUDIO_12:
            case SPC_PREVENT_ALLOW_MEDIUM_REMOVAL:
#if E_DIAGS == 1
            case DIAG_CMD_ENA:              /* 0xF0 */
            case DIAG_CMD_SEEPROM:          /* 0xF1 */
            case DIAG_CMD_MRW:              /* 0xF2 */
            case DIAG_CMD_JITTER:           /* 0xF3 */
            case DIAG_VENDOR_READ:          /* 0xF4 */
            case DIAG_VENDOR_WRITE:          /* 0xF5 */
            case DIAG_CMD_DVD:                  /* 0xF6 */
#if P_DIAGS == 1
            case DIAG_TRANSPARENT_SEND_CMD:         // 0xFD
#endif /* P_DIAGS == 1 */
            case READ_WRITE_DEVICE:              /* 0xF7 */
            case DIAG_CMD_OFFLINE_CALIBRATION: /* 0xF8 */
            case DIAG_CMD_DC_ERASE:                      /* 0xF9 */
            case DIAG_CMD_DRAM_LOG_CONTROL:
#endif /* E_DIAGS == 1 */
#if (SUN_FUNC_TEST == 1)
            case DIAG_LASER:
#endif
                ProcessDiscReadyCommand();
                break;
            default:
            	//start== HP CMPLY test TDR840 fail.
            	if((TrayManagerInfo.TrayState == TRAY_STATE_OUT_OK) || (TrayManagerInfo.TrayState == TRAY_STATE_OUT_EXECUTING))
                    BUILD_SENSE(0x02,0x3A,0x02,0x03);
            	else
                    BUILD_SENSE(0x02,0x04,0x01,0x20); /* LOGICAL UNIT IS IN PROCESS OF BECOMING READY */
                //end==
                AutoCommandComplete();
                break;
        }
    }
    else
    {
        /* DRIVE IS READY */
        switch(Iram_Cmd_block.bf[0])
        {
            case SBC_START_STOP_UNIT:
            case MMC_PLAY_AUDIO_10:
            case MMC_STOP_PLAY_SCAN:
            case SPC_PREVENT_ALLOW_MEDIUM_REMOVAL:
            case MMC_PLAY_AUDIO_12:
                ProcessDiscReadyCommand();
                return;
        }
        if ((Iram_Svr_player_mode == TRAY_OUT_MODE) || (St_drv_ready == FALSE))
        {
#if E_DIAGS == 0
            /* MEDIUM NO PRESENT */
            BUILD_SENSE(0x02,0x3A,0x00,0x21);
            AutoCommandComplete();
#else /* E_DIAGS */
            if ( (0xF0 <= Iram_Cmd_block.bf[0]) && (Iram_Cmd_block.bf[0] <= 0xF9) || (Iram_Cmd_block.bf[0] == 0xFD) || (Iram_Cmd_block.bf[0] == 0xFF) ||
                 (Iram_Cmd_block.bf[0] == MMC_PLAY_AUDIO_10) || (Iram_Cmd_block.bf[0] == MMC_PLAY_AUDIO_12)
#if (SUN_FUNC_TEST == 1)
                 ||(Iram_Cmd_block.bf[0] == 0xDF)
                 ||(Iram_Cmd_block.bf[0] == DIAG_LASER)
#endif
                 ||(Iram_Cmd_block.bf[0] == 0xFE)
                 || (Iram_Cmd_block.bf[0] == MMC_REPORT_KEY) || (Iram_Cmd_block.bf[0] == MMC_SEND_KEY))
            {
                ProcessDiscReadyCommand();
            }
            else
            {
                /* MEDIUM NO PRESENT */
                if(TrayManagerInfo.TrayState == TRAY_STATE_IN_OK)
                    BUILD_SENSE(0x02,0x3A,0x01,0x22);
                else
                    BUILD_SENSE(0x02,0x3A,0x02,0x22);
                AutoCommandComplete();
            }
#endif /* E_DIAGS */
        }
        else
        {
            ProcessDiscReadyCommand();
        }
    }
}

/*******************************************************************************

    FUNCTION        ProcessDiscReadyCommand

    DESCRIPTION     Command handler for commands accessing the disc

********************************************************************************/
static void ProcessDiscReadyCommand(void)
{
    St_cmd_inproc = TRUE;
    BUILD_SENSE(0x00,0x00,0x00,0x04);        /* clear sense code first */

    /* Decode ATAPI command */
#if (BD_ENABLE)
    if (pdbDiscLoaded() == BD_DISC)
    {
        // If the requested command is no longer for BD_RAW_DFL, clear the flag.
        if (!((Iram_Cmd_block.bf[0] == MMC_READ_DISC_STRUCTURE) && (Iram_Cmd_block.bf[7] == BD_RAW_DFL)))
            BdRawDflAssembled = CLEAR;
    }
#endif

    if (pdbDiscLoaded() == DVD_DISC)            /* commands for DVD Disc */
    {
        #if (DVD_RAM_READ == 1)
        if(pdbGetDVDType() == DVD_RAM)
        {
            if  (CheckDVDRAMCommand() == TRUE) return;
        }
        #endif
    }

    if (pdbDiscLoaded() == DVD_DISC)            /* filter out CD commands for DVD Disc */
    {
        {
        switch(Iram_Cmd_block.bf[0])
            {
            case MMC_BLANK:
            case MMC_CLOSE_SESSION:
            case MMC_ERASE_10:
            case MMC_FORMAT:
            case MMC_PLAY_AUDIO_10:
            case MMC_PLAY_AUDIO_12:
            case MMC_PLAY_AUDIO_MSF:
            #if (PCB != 710)&&(PCB != 711)&&(PCB != 817)//Read DVD Flow support Read CD command.
            case MMC_READ_CD:
            #endif
            case MMC_READ_CD_MSF:
            case MMC_REPAIR_TRACK:
            case MMC_RESERVE_TRACK:
            case MMC_SEND_CUE_SHEET:
//          case MMC_SYNC_CACHE://[BTC][B001]Bios mark for winfinal [B10F]
            case MMC_WRITE_10:
            case MMC_WRITE_12:
            case MMC_WRITE_VERIFY_10:
                    BUILD_SENSE(0x02,0x30,0x00,0x29);    /*  incompatible Medium Installed */
                    AutoCommandComplete();
                    return;
            default:
                break;
            }
        }
    }

    #if(DISABLE_CD_WRITE_CMD == 1)          /*  Filter out about CD Write Command--temporary */
    if (pdbDiscLoaded() == CD_DISC)
    {
        switch(Iram_Cmd_block.bf[0])
        {
            case MMC_BLANK:
            case MMC_CLOSE_SESSION:
            case MMC_ERASE_10:
            case MMC_FORMAT:
            case MMC_REPAIR_TRACK:
            case MMC_RESERVE_TRACK:
            case MMC_SEND_CUE_SHEET:
            case MMC_SYNC_CACHE:
            case MMC_WRITE_10:
            case MMC_WRITE_12:
            case MMC_WRITE_VERIFY_10:
                    BUILD_SENSE(0x05,0x20,0x00,0x64);        /* INVALID COMMAND OP CODE */
                    AutoCommandComplete();
                    return;
            default:
                break;
        }
    }
    #endif

#if(EN_POWER_CONTROL==1)
    /* For vendor command, change the power mode into Full power mode */
        if (((Iram_Cmd_block.bf[0]&0xF0)== 0xF0) && (pdbGetDiscKind() == eUnknownKind))
        {
#if (PCB == 710)||(PCB == 711)||(PCB == 817)
            if(Iram_Cmd_block.bf[0]!=0xF3)
#endif
#if (WITHOUT_SERVO == 0)
            PowerControl(eIdlePower); //For servo do OPC using only
#else
            PowerControl(eStandbyPower);
#endif
        }
#else
    /*-----------------------------------------------------------------------
        During servo STOP mode, and get some command.
        Enable Clock Register before Servo Wake Up
            to make sure setting decoder/encoder register works!
    ------------------------------------------------------------------------*/
    if(Iram_Svr_player_mode == STOP_MODE)
    {
        switch(Iram_Cmd_block.bf[0])
        {
            case  SBC_READ_10:
            case  SBC_READ_12:
            case  SBC_VERIFY_10:
            case  SBC_VERIFY_12:
            case  MMC_READ_CD:
            case  MMC_READ_SUB_CHANNEL:
            case  MMC_PLAY_AUDIO_MSF:
            case  MMC_PLAY_AUDIO_10:
            case  MMC_PLAY_AUDIO_12:
            case  MMC_READ_CD_MSF:
            case  MMC_SCAN:
            case  MMC_STOP_PLAY_SCAN:
            case  MMC_READ_DISC_STRUCTURE:
    #if (ENABLE_AACS == 1 && CEProduct == 1)
            case  VENDOR_READ_DISC_STRUCTURE:
    #endif
                vWakeUpFromPowerSaveMode();
                break;
            default:
                break;
        }
   }
#endif

    switch(Iram_Cmd_block.bf[0])
    {
        //[BTC][B001]Bios merge [B10F]
        case  MMC_SYNC_CACHE:     //sky modify CD/DVD/BD WinFinal Test               
            if ((pdbDiscLoaded() == BD_DISC) || (pdbDiscLoaded() == DVD_DISC) || (pdbDiscLoaded() == CD_DISC))
             {
	      AutoCommandComplete();
	      return;
             }       
            break;
        case  MMC_READ_DISK_INFO:
            ReadDiscInfoCommand();
            break;

        case  MMC_READ_TRACK_INFO:
            ReadTrackInfoCommand();
            break;

        case SBC_READ_10:
            switch(pdbDiscLoaded())
            {
#if (BD_ENABLE == 1)
                case BD_DISC:
                    if (pdbGetBDType() == BD_ROM)
                        BDRead10Command();
                    else
                        BdWritableRead10Command();
                    break;
#endif
#if (ENABLE_HDDVD == 1)
                case HD_DISC:
#endif
                case DVD_DISC:
                    DVDRead10Command();
                    break;
                case CD_DISC:
                    CDRead10Command();
                    break;
            }
            break;
        case MMC_READ_CD:
	     switch(pdbDiscLoaded())
            {
#if (PCB == 710)||(PCB == 711)||(PCB == 817)
                case DVD_DISC: //Read DVD Flow support Read CD command.
                    DVDRead10Command();
                    break;
#endif
#if (BD_ENABLE == 1)
                case BD_DISC:
                    BUILD_SENSE(0x05,0x20,0x00,0x64);        /* INVALID COMMAND OP CODE */
                    AutoCommandComplete();
                    break;
#endif
                case CD_DISC:
                    ReadCDCommand();
                    break;
	        }
            break;
        case MMC_SET_READ_AHEAD:
            SetReadAheadCommand();
            break;
        case SBC_SEEK:
            switch(pdbDiscLoaded())
            {
#if (BD_ENABLE == 1)
                case BD_DISC:
                    BDSeekCommand();
                    break;
#endif
#if (ENABLE_HDDVD == 1)
                case HD_DISC:
#endif
                case DVD_DISC:
                    DVDSeek10Command();
                    break;
                case CD_DISC:
                    CDSeek10Command();
                    break;
            }
            break;
        case SPC_TEST_UNIT_READY:
            TestUnitReadyCommand();
            break;
        case SPC_MODE_SELECT:
            ModeSelectCommand();
            break;
        case MMC_READ_SUB_CHANNEL:
            ReadSubChannelCommand();
            break;
        case MMC_PLAY_AUDIO_MSF:
            PlayAudioMSFCommand();
            break;
        case MMC_PLAY_AUDIO_10:
            PlayAudio10Command();
            break;
        case MMC_READ_CD_MSF:
            ReadCDMSFCommand();
            break;
        case SPC_REQUEST_SENSE:
            RequestSenseCommand();
            break;
        case SPC_MODE_SENSE:
            ModeSenseCommand();
            break;
        case SPC_READ_BUF:
            ReadBufferCommand();
            break;
        case SPC_WRITE_BUF:
            WriteBufferCommand();
            break;
        case  MMC_READ_BUF_CAP:
            ReadBufferCapacityCommand();
            break;
        case MMC_READ_CAPACITY:
            ReadCapacityCommand();
            break;
        case SBC_START_STOP_UNIT:
            StartStopUnitCommand();
            break;
        case  MMC_SCAN:
            ScanCommand();
            break;
        case  SPC_INQUIRY:
            InquiryCommand();
            break;
        case  MMC_PAUSE_RESUME:
            PauseResumeCommand();
            break;
        case  MMC_PLAY_AUDIO_12:
            PlayAudio12Command();
            break;
        case  SPC_PREVENT_ALLOW_MEDIUM_REMOVAL:
            PreventAllowMediumRemovalCommand();
            break;
        case  SBC_READ_12:
            switch(pdbDiscLoaded())
            {
#if (BD_ENABLE == 1)
                case BD_DISC:
                    if (pdbGetBDType() == BD_ROM)
                        BDRead10Command();
                    else
                        BdWritableRead10Command();
                    break;
#endif
#if (ENABLE_HDDVD == 1)
                case HD_DISC:
#endif
                case DVD_DISC:
                    DVDRead10Command();
                    break;
                case CD_DISC:
                    CDRead12Command();
                    break;
            }
            break;
        case  READ_HEADER:
            ReadHeaderCommand();
            break;
        case  MMC_READ_TOC:
            switch(pdbDiscLoaded())
            {
#if (BD_ENABLE == 1)
                case BD_DISC:
                    BDReadTOCCommand();
                    break;
#endif
#if (ENABLE_HDDVD == 1)
                case HD_DISC:
                    HDReadTOCCommand();
                    break;
#endif
                case DVD_DISC:
                    DVDReadTOCCommand();
                    break;
                case CD_DISC:
                    CDReadTOCCommand();
                    break;
            }
            break;
        case  MMC_STOP_PLAY_SCAN:
            StopPlayScanCommand();
            break;
        case  FLASH_CODE:
            UpdateFirmwareCommand();
            AutoCommandComplete();
            break;

#if (BD_ENABLE)
        case  DIAG_CMD_LOAD_DFL:
            LoadDefectListCommand();
            AutoCommandComplete();
            break;
#endif

        case REZERO_UNIT:
            Iram_Cmd_block.bf[2] = 0;           /* Seek to 00:02:00(=LBA 0) */
            Iram_Cmd_block.bf[3] = 0;
            Iram_Cmd_block.bf[4] = 0;
            Iram_Cmd_block.bf[5] = 0;
            if (pdbDiscLoaded() == DVD_DISC)
                DVDSeek10Command();
            else
                CDSeek10Command();
            break;
        case  MMC_GET_PERFORMANCE:
            GetPerformanceCommand();
            break;
        case MMC_READ_FORMAT_CAPACITIES:
            ReadFormatCapacitiesCommand();
            break;
        case SBC_VERIFY_10:
        case SBC_VERIFY_12:
            if (pdbDiscLoaded() == DVD_DISC)
                InvalidCommand();
            else
                VerifyCommand();
            break;
        case  MMC_GET_CONFIGURATION:
            GetConfigurationCommand();
            break;
        case MMC_SET_STREAMING:
#if(EN_DVD_HIGH_JITTER_DETECT ==1)
            HIGH_JITTER_DISC &= ~0x02;
#endif
            SetStreamingCommand();
            break;

/* Support ls print command here */
#if (EN_LS == 1)
        case  LS_CMD_PRINT:             /* FC: ls print command */
            //lsPrint(); //need to create it on new .c file; Henry 2008/07/14
            break;
#endif // End of (EN_LS == 1)

/* Check if Engineering Diagnostics commands  */
/* All these commands are Vendor Unique */

#if E_DIAGS == 1
        case  DIAG_CMD_ENA:                 /* F0: Enable/Disable Diagnostics */
            diaCmdEnable();
            break;
        case  DIAG_CMD_SEEPROM:             /* F1: Serial EEPROM Read/Write Access */
            diaCmdSEEPROMAccess();
            break;
        case  DIAG_CMD_MRW:                 /* F2: MRW Test Suites */
    #if (DVD_RAM_READ == 1)
            if (pdbServoGetfDiscKind() == eDVDRAMDisc)
                diaCmdDvdram();
            else
#endif
                diaCmdMRW();
            break;
        case  DIAG_VENDOR_READ:             /* F4: Vendor Read Commands */
            diaVendorRead();
            break;
        case DIAG_CMD_DVD:                  /* F6: DVD diagnostic commands */
            diaCmdDvdDiagnostic();
            break;

#if (P_DIAGS == 1)
        case DIAG_TRANSPARENT_SEND_CMD:     /* FD: POS diagnostic commands */
            diaTransparentSend();
            break;
#endif /* P_DIAGS */

#if (SUN_FUNC_TEST == 1)
        case SUN710_DIAG_F3_CMD:
            dia_sun710_F3_cmd();
            break;

        case SUN710_DIAG_DF_CMD:
            dia_sun710_DF_cmd();
            break;
#endif

        case READ_WRITE_DEVICE:             /* F7: READ/WRITE DEVICE Command */
            diaReadWriteDevice();
            break;
        case  DIAG_CMD_OFFLINE_CALIBRATION: /* F8: OFF Line Calbration Vendor Command */
            if(St_diag_ena == FALSE)       /* F3: support sun710 F3 CMD*/
            {
                BUILD_SENSE(0x05,0x20,0x00,0xF8);    /* INVALID COMMAND OPERATION CODE */
                AutoCommandComplete();
            }
            else
                diaCmdOfflineCalibration();
            break;
        case  DIAG_CMD_DC_ERASE:            /* F9: DC Erase  */
            diaCmdDCErase();
            break;
        case  DIAG_CMD_DRAM_LOG_CONTROL:    /* FE: DRAM debugging log area control.*/
            diaDebugLogCtrl();
            break;
#if (SUN_FUNC_TEST == 1)
        case DIAG_LASER:                    /* EE: Laser Measurment */
            diaLaserMeasurment();
            break;
#endif
#endif /* E_DIAGS */
        case  MMC_READ_DISC_STRUCTURE:       /* Read Disc Structure */
            if (! ReadGenericDiscStructure())
            {
                #if (BD_ENABLE)
                if (pdbDiscLoaded() == BD_DISC)
                    ReadBDStructureCommand();
                else
                #endif  // BD_ENABLE
                #if (ENABLE_HDDVD)
                if (pdbDiscLoaded() == DVD_DISC || pdbDiscLoaded() == HD_DISC)
                #else   // != ENABLE_HDDVD
                if (pdbDiscLoaded() == DVD_DISC)
                #endif  // ENABLE_HDDVD
                    ReadDVDStructureCommand();
                else
                {
                    BUILD_SENSE(0x02,0x30,0x02,0x01);        /* CANNOT READ MEDIUM-INCOMPATIBLE FORMAT */
                    AutoCommandComplete();
                }
            }
            break;
#if (ENABLE_AACS == 1 && CEProduct == 1)
        case  VENDOR_READ_DISC_STRUCTURE:
            VendorReadDiscStructure();
            break;
#endif
        case  MMC_SEND_DISC_STRUCTURE:       /* Send Disc Structure */
            InvalidCommand();
            break;
        case  MMC_REPORT_KEY:               /* Report Key For CSS & CPRM & AACS */
#if (BD_ENABLE == 1)
            if (pdbDiscLoaded() == BD_DISC)
                BDReportKeyCommand();
            else
#endif  /* BD_ENABLE */
                ReportKeyCommand();
            break;
        case  MMC_SEND_KEY:
#if (BD_ENABLE == 1)
            if (pdbDiscLoaded() == BD_DISC)
                BDSendKeyCommand();         /* BD Send Key Management */
            else
#endif  /* BD_ENALBE */
                SendKeyCommand();           /* DVD_Send key Manegemnt */
            break;
#if (ENABLE_AACS == 1 && CEProduct == 1)
        case  VENDOR_REPORT_KEY:
            VendorReportKey();
            break;

        case  VENDOR_SEND_KEY:
            VendorSendKey();
            break;
#endif  /* ENABLE_AACS */

        default:
            InvalidCommand();
    }
}

/*--------------------------------------------------------------------
  Name: CheckATAPICommand()
  Description:
    Verifies if ATAPI command is supported
  Inputs:
    none
  Outputs:
    none
  Returns:
    TRUE - drive supports the ATAPI command
    FALSE - drive does not support the command (will generate 05/20/00)
--------------------------------------------------------------------*/
extern USHORT   timeUp;
vBOOL CheckATAPICommand(void)
{
    timeUp_Group1= timeUp;
#if (CEProduct == 1)
  #if (BD_ENABLE == 1)
    if ( (pdbDiscLoaded() == DVD_DISC || pdbDiscLoaded() == BD_DISC) && Iram_Cmd_block.bf[0] == SBC_READ_12 )
  #else
    if (pdbDiscLoaded() == DVD_DISC && Iram_Cmd_block.bf[0] == SBC_READ_12)
  #endif
        svoIramVar.knlTimeupCount = TimeUpForRead;
    else
#endif  /* CEProduct == 1 */
    svoIramVar.knlTimeupCount = timeUp_Group1;

#if (SUPPORT_DVD_WRITE_ABILITY == P_SL_DISC)
    if(pdbServoGetDiscType() == eLPPDisc)
#elif(SUPPORT_DVD_WRITE_ABILITY == PM_SL_DISC)
    if(pdbGetDVDLayers() == DUAL_LAYER)
#elif (SUPPORT_DVD_WRITE_ABILITY == P_DL_DISC)
    if(pdbGetDiscKind()== eDVDMinusRDLDisc)
#else
    if(0)
#endif
    {    switch(Iram_Cmd_block.bf[0])
        {
            case  MMC_WRITE_12:
            case  MMC_WRITE_10:
            case  MMC_WRITE_VERIFY_10:
            case  MMC_SYNC_CACHE:
            case  MMC_ERASE_10:
            case  MMC_FORMAT:
            case  MMC_RESERVE_TRACK:
            case  MMC_CLOSE_SESSION:
            case  MMC_BLANK:
            case  MMC_SEND_OPC_INFO:
            case  MMC_SEND_DISC_STRUCTURE:
                return(FAIL);
                break;
            default:
                 break;
        }
    }

    switch(Iram_Cmd_block.bf[0])
    {
        case  MMC_WRITE_12:
        case  MMC_WRITE_10:
        case  MMC_SYNC_CACHE:
        case  MMC_ERASE_10:
        case  MMC_FORMAT:
        case  MMC_RESERVE_TRACK:
        case  MMC_CLOSE_SESSION:
        case  MMC_READ_TRACK_INFO:
        case  MMC_BLANK:
        case  MMC_SEND_OPC_INFO:
#if (CEProduct== 0)
            Iram_Knl_save_counter = STANDBYCNT;
#endif
        case  SPC_MODE_SELECT:
        case  SPC_REQUEST_SENSE:
        case  SPC_MODE_SENSE:
        case  MMC_GET_PERFORMANCE:
        case  MMC_READ_FORMAT_CAPACITIES:
        case  MMC_GET_CONFIGURATION:
        case  MMC_SET_STREAMING:
#if (USE_TWO_KEY_FUNCTION==1)
        gPause_By_SeekCmd_flag = FALSE;
#endif
#if (CEProduct == 1)
            Iram_Knl_save_counter = STANDBYCNT;
#endif
            svoIramVar.knlTimeupCount = TIMEUP_NON;
            break;
        case  SBC_START_STOP_UNIT:
#if (USE_TWO_KEY_FUNCTION==1)
        gPause_By_SeekCmd_flag = FALSE;
#endif
            svoIramVar.knlTimeupCount = TIMEUP_10S;
            Iram_Knl_save_counter = STANDBYCNT;
            break;
        case  SBC_READ_10:
        case  MMC_READ_CD:
#if (CEProduct == 1)
/* Reduce the time out limit to match the backend's spec */
            if (pdbDiscLoaded() == CD_DISC)
                svoIramVar.knlTimeupCount = 5500/20; /* 5.5sec */
            else if (Iram_Cmd_block.bf[0] == SBC_READ_10)
                svoIramVar.knlTimeupCount = TimeUpForRead;
            else
#endif

#if (Customer_A1 ==1)
                svoIramVar.knlTimeupCount = 1500/20; /* 1.5sec */
#else
                svoIramVar.knlTimeupCount = 6500/20; /* 6.5sec */
#endif

            timeUp_Group1 = svoIramVar.knlTimeupCount;
        case  MMC_SEND_CUE_SHEET:
        case  SBC_SEEK:
        case  MMC_PLAY_AUDIO_MSF:
        case  MMC_PLAY_AUDIO_10:
        case  MMC_READ_CD_MSF:
        case  MMC_SCAN:
        case  MMC_PAUSE_RESUME:
        case  MMC_PLAY_AUDIO_12:
//      case  SEND_CD_XA:
        case  SPC_PREVENT_ALLOW_MEDIUM_REMOVAL:
        case  SBC_READ_12:
        case  READ_HEADER:
        case  MMC_READ_TOC:
        case  MMC_STOP_PLAY_SCAN:
        case  REZERO_UNIT:
        case  SBC_VERIFY_10:
        case  SBC_VERIFY_12:
        case  MMC_READ_DISC_STRUCTURE:
        case  MMC_REPORT_KEY:
        case  MMC_SEND_KEY:
#if (ENABLE_AACS == 1 && CEProduct == 1)
        case  VENDOR_READ_DISC_STRUCTURE:
        case  VENDOR_REPORT_KEY:
        case  VENDOR_SEND_KEY:
#endif /* ENABLE_AACS */
#if (USE_TWO_KEY_FUNCTION==1)
        if (!((Iram_Cmd_block.bf[0] == SBC_SEEK) && (gPause_By_SeekCmd_flag == TRUE)))
            gPause_By_SeekCmd_flag = FALSE;
#endif
            Iram_Knl_save_counter = STANDBYCNT;
            break;
        case  SPC_READ_BUF:
        case  SPC_WRITE_BUF:
        case  MMC_READ_BUF_CAP:
        case  SPC_TEST_UNIT_READY:
        case  MMC_READ_CAPACITY:
        case  SPC_INQUIRY:
        case  FLASH_CODE:
        case  MMC_MECHANISM_STATUS:
        case  MMC_GET_EVENT_STATUS_NOTIFICATION:
        case  MMC_SET_CD_SPEED:
        case  MMC_READ_SUB_CHANNEL:
        case  MMC_SET_READ_AHEAD:
        case  MMC_READ_DISK_INFO:
#if (USE_TWO_KEY_FUNCTION==1)
        if (!((Iram_Cmd_block.bf[0] == SPC_TEST_UNIT_READY)
             || (Iram_Cmd_block.bf[0] == MMC_GET_EVENT_STATUS_NOTIFICATION)
             || (Iram_Cmd_block.bf[0] == MMC_READ_SUB_CHANNEL)))
            gPause_By_SeekCmd_flag = FALSE;
#endif
#if 0 //(CEProduct == 1)     // Do not Remove!,this part for BTC request
            Iram_Knl_save_counter = STANDBYCNT;
#endif
            break;

/* Support ls print command here */
#if (EN_LS == 1)
        case  LS_CMD_PRINT:                     /* FC */
#endif // End of (EN_LS == 1)

#if E_DIAGS == 1
        case  DIAG_CMD_ENA:                     /* F0 */
        case  DIAG_CMD_SEEPROM:                 /* F1 */
        case  DIAG_CMD_MRW:                     /* F2 */
        case  DIAG_CMD_JITTER:                  /* F3 */
        case  DIAG_VENDOR_READ:                 /* F4 */
        case  DIAG_VENDOR_WRITE:                /* F5 */
        case  DIAG_CMD_DVD:                     /* F6 */
#if P_DIAGS == 1
        case DIAG_TRANSPARENT_SEND_CMD:         // FD
#endif /* P_DIAGS */
        case  READ_WRITE_DEVICE:                /* F7 */
        case  DIAG_CMD_OFFLINE_CALIBRATION:     /* F8 */
        case  DIAG_CMD_DC_ERASE:                /* F9 */
        case  DIAG_CMD_DRAM_LOG_CONTROL:
#if (SUN_FUNC_TEST == 1)
        case DIAG_LASER:                        /* EE */
#endif
#if (USE_TWO_KEY_FUNCTION==1)
        gPause_By_SeekCmd_flag = FALSE;
#endif

#if (BD_ENABLE)
        case  DIAG_CMD_LOAD_DFL:
#endif
            svoIramVar.knlTimeupCount = TIMEUP_NON;
            Iram_Knl_save_counter = STANDBYCNT;
            break;
#endif /* E_DIAGS */

#if (SUN_FUNC_TEST == 1)
        case SUN710_DIAG_DF_CMD:
            svoIramVar.knlTimeupCount = TIMEUP_NON;
            Iram_Knl_save_counter = STANDBYCNT;
            break;
#endif
        default:
#if (USE_TWO_KEY_FUNCTION==1)
        gPause_By_SeekCmd_flag = FALSE;
#endif
            return(FAIL);
    }
    return(PASS);
}


/*--------------------------------------------------------------------
  Name: ResetCommandFIFO()
  Description:
    Initializes registers and UDMA mode after drive gets ATA Reset
    command or soft reset
  Inputs:
    none
  Outputs:
    none
  Returns:
    none
--------------------------------------------------------------------*/
void ResetCommandFIFO(void)
{

    if (Iram_Send_Giveup_F == TRUE)
    {
        /*  use CD disc make access time test shut down in CD-T */
        InitializeChip();
        Iram_Send_Giveup_F = FALSE;
    }
}


/*--------------------------------------------------------------------
  Name: AutoCommandComplete()
  Description:
    Does automatic command complete process, including COD, INTRQ...
  Inputs:
    none
  Outputs:
    none
  Returns:
    none
--------------------------------------------------------------------*/
#pragma arm section code = "CMD_COMP"
void    AutoCommandComplete(void)
{
    ULONG Time;

    Time = ReadTimer(&Iram_atapi_timer);
    StartTimer(&Iram_atapi_timer);
    SendMsgCn(SEND_ATAPI_CMD,8,0x800311,0,Iram_Sense.key,Iram_Sense.cod,Iram_Sense.qul,A4B(Time));
    St_cmd_inproc = FALSE;
    Ata_sub_burst = FALSE;
#if (ENABLE_AUTO_ABORT == 1)
    AutoAbort = 0;
#endif

    WRITE_FIELD(HAL_HST_ODDWRDSUP, 0);

    if (ATA_SRST_rcv == TRUE)
    {
        ATA_SRST_rcv = FALSE;
        return;
    }

#if(MSG_OUT_MODE == MSG_USBPORT && ENABLE_UART_RXD == 1)
    if (s2b_atapi_cmd_received == TRUE)
    {
        s2b_atapi_cmd_received = FALSE;
        s2bBUILD_SENSE(Iram_Sense.key, Iram_Sense.cod, Iram_Sense.qul);
        ENABLE_HINT();
        return;
    }
#endif

    SendStatus();

    WRITE_FIELD(HAL_HST_SCTRG, 1);  /* STATUS_COMPLETION   */
}
#pragma arm section code


/*--------------------------------------------------------------------
  Name: InvalidCommand()
  Description:
    Returns CC 05/20/00 for invalid ATAPI command
  Inputs:
    none
  Outputs:
    none
  Returns:
    none
--------------------------------------------------------------------*/
void InvalidCommand(void)
{
    BUILD_SENSE(0x05,0x20,0x00,0x66);        /* INVALID COMMAND OP CODE */
    AutoCommandComplete();
}


/*--------------------------------------------------------------------
  Name: DisableAllInterrupts()
  Description:
    Disables global interrupt. This function is called during firmware upgrade
  Inputs:
    none
  Outputs:
    none
  Returns:
    none
--------------------------------------------------------------------*/
void DisableAllInterrupts(void)
{
    DISABLE_INTERRUPT();                                        // Disable global interrupt.
    WRITE_REG(HAL_AHB_INTCTRLREG_MAP->ICINTSEL , 0x00);         // D-select all intividual high level Interrupts
    WRITE_REG(HAL_AHB_INTCTRLREG_MAP->ICINTEN  , 0x00);         // Enable timers only.
    WRITE_REG(HAL_AHB_INTCTRLREG_MAP->ICIENCLR , 0x0FFFFFFFF);

    TIMER0.TCNTRL.BIT.TSTRT = 0;
    TIMER1.TCNTRL.BIT.TSTRT = 0;
    TIMER2.TCNTRL.BIT.TSTRT = 0;
    TIMER3.TCNTRL.BIT.TSTRT = 0;
    TIMER4.TCNTRL.BIT.TSTRT = 0;
    TIMER5.TCNTRL.BIT.TSTRT = 0;
    TIMER6.TCNTRL.BIT.TSTRT = 0;
}


/*--------------------------------------------------------------------
  Name: ClearFlashUpWorkAreaAll()
  Description:
    Fills flash area with 0xFF pattern. This function is called
    during firmware upgrade
  Inputs:
    none
  Outputs:
    none
  Returns:
    none
--------------------------------------------------------------------*/
void ClearFlashUpWorkAreaAll( void )
{
    ULONG   addr;
    vBOOL   temp_IE;

    StopDecoder();
    temp_IE = GET_INT_ENABLE_MASK();
    DISABLE_INTERRUPT();

    for ( addr = 0L; addr < FLASH_ROM_SIZE; addr++ )
    {
        SCSDRAM[addr] = 0xFF;
    }

    SET_INT_ENABLE_MASK( temp_IE );

}


/*--------------------------------------------------------------------
  Name: UpdateFirmwareCommand()
  Description:
    Performs firmware upgrade
  Inputs:
    none
  Outputs:
    none
  Returns:
    none
--------------------------------------------------------------------*/
extern void PageSize2P5K(void);
#if(BOOT_CODE_SUPPORT)
extern BYTE    WriteBootCode;
#endif
static void UpdateFirmwareCommand(void)
{
    ULONG   k;
    USHORT  i;
    USHORT  cnt;

    if(St_diag_ena == FALSE)              /* Check the Diag Enter flag */
    {
        BUILD_SENSE(0x05,0x20,0x00,0x67);    /* INVALID COMMAND OPERATION CODE */
        return;
    }

    if (St_no_disc == FALSE)
    {
        ServoStop();
    }
    
#if (EN_POWER_CONTROL == 1)
    // Bring drives from standby mode to fast flash process.
    PowerControl(eStartUpPower);
#endif
  
    if(Iram_Page_Size_3K_Flag == TRUE)
        PageSize2P5K();

    /* Transfer Indicator */
    switch ( Iram_Cmd_block.bf[5] )
    {
        case 0x00:                                  /* Start */
            ClearFlashUpWorkAreaAll();
        case 0xFF:                                  /* Middle */
            cnt = 60;                               /* 60 * 1024Bytes = 60KBytes */
            break;

        default:
            cnt = Iram_Cmd_block.bf[5];
            break;
    }

    switch ( Iram_Cmd_block.bf[5] )
    {
        case 0x00:                                  /* Start */
            Iram_Buf_hxfer_blk = 1;
            Iram_Linear_Xfer_addr = 0L;
            break;
        default:                                    /* Middle or End */
            Iram_Linear_Xfer_addr = FlashLinearAdress;
            break;
    }


	#if(SUN_FUNC_TEST == 0)
    opReadyLED = (~opReadyLED);
	#else
         HAL_DSP_INVERT_LED();
       #endif


    i = cnt / 2;
    if ( ! GetFlashUpdateFromHost( i ) )
    {
        BUILD_SENSE(0x04,0x44,0x00,0x1b);           /* internal logical unit failure */
        return;
    }
    Iram_Linear_Xfer_addr += (cnt * 1024);          /* Update address */

	#if(SUN_FUNC_TEST == 0)
    opReadyLED = (~opReadyLED);
	#else
         HAL_DSP_INVERT_LED();
       #endif

    switch ( Iram_Cmd_block.bf[5] )
     {
        case 0x00:      /* Start  */
            vWakeUpFromPowerSaveMode();
            break;
        case 0xFF:      /* Middle */
            break;
        default:        /* End    */
// Start: Verify Model name before write to flashrom.
            k = FLASH_ROM_SIZE - 0x10L;
            for(i=0; i<9; i++)
            {
                if(SCSDRAM[k + i] != EndOfFirmWare[i])
                {
                    if (Iram_Cmd_block.bf[2] == 0) //Verify Model name before write to flashrom
                    {   // The text string(Product ID) at the end of transmitted data in SDRAM
                        //  does not comply with Product ID per OEM Table.
                        BUILD_SENSE(0x05,0x20,0x00,0x68);    /* INVALID COMMAND OPERATION CODE */
                        return;
                    }
                    else // don't care Model name is different.
                    {// overwrite boot code to update new model name
                        Iram_Cmd_block.bf[4] = 0x01; // overwrite boot code to update new model name
                        break;
                    }
                }
            }
// End: Verify Model name before write to flashrom.

// Start: Verify checksum before write to flashrom.
//chksun_DRAM+chksum_DRAM1 = nomal code.
//chksum_DRAM2 = persistent table.
//The chksum_DRAM2 is stored at the last 2 bytes of flash memory.
            if (Iram_Cmd_block.bf[3] == 1)
            {
                USHORT chksum_DRAM,chksum_DRAM1,chksum_DRAM2;
                chksum_DRAM1 = 0;

#if (PCB != 710) && ( PCB != 815) && (PCB != 711) && (PCB != 817)
                for (k = 0; k < CHECK_SUM_PROTECT_START; k++)   // Calc checksum for area in which ARM Vectors etc. reside.
                    chksum_DRAM1 += (USHORT)SCSDRAM[k];
#endif
                chksum_DRAM2 = 0;
                for (k = CHECK_SUM_PROTECT_START; k < (CHECK_SUM_PROTECT_END + 1); k++)
                    chksum_DRAM2 += (USHORT)SCSDRAM[k];
                                                              // Calc checksum for area in which Persistent Parameters reside.
                chksum_DRAM = 0;
                for (k = (CHECK_SUM_PROTECT_END + 1); k < Iram_Linear_Xfer_addr; k++)
                    chksum_DRAM += (USHORT)SCSDRAM[k];
                                                                  // Calc checksum for area in which most of the program code resides.

                chksum_DRAM = chksum_DRAM + chksum_DRAM1 + chksum_DRAM2;

                cnt = ((USHORT)Iram_Cmd_block.bf[6] << 8) | (USHORT)Iram_Cmd_block.bf[7];     // Get Checksum from CDB (supplied by host)

                if(cnt == 0)
                    cnt = ((USHORT)Iram_Cmd_block.bf[1] << 8) | (USHORT)Iram_Cmd_block.bf[2];

                if (chksum_DRAM != cnt)
                {
                    // Checksum calculated from SDRAM content is not the same as the one supplied by the host in CDB.
                    BUILD_SENSE(0x0B,BHIGH(chksum_DRAM),BLOW(chksum_DRAM),0x00);
                    return;
                }
                else
                {
                    k = Iram_Linear_Xfer_addr - 2L;
                    chksum_DRAM2 = chksum_DRAM2 + SCSDRAM[k] + SCSDRAM[k + 1L];

                    SCSDRAM[k] = (vBYTE)(chksum_DRAM2>>8);
                    SCSDRAM[k + 1L] = (vBYTE)chksum_DRAM2;
                }

            }
// End: Verify checksum before write to flashrom.

            BUILD_SENSE(0x00,0x00,0x00,0x05);            // Clear Sense Bytes

#if (FLASH_BY_DISC == 1)
            SaveCurrentTransferSettings(0);
#else
            SaveCurrentTransferSettings();
#endif

#if(MSG_OUT_MODE == MSG_USBPORT && ENABLE_UART_RXD == 1)
            if (s2b_atapi_cmd_received == TRUE)
            {
                s2bBUILD_SENSE(0x00,0x00,0x00);
            }
#endif

#if(BOOT_CODE_SUPPORT)
            if(Iram_Cmd_block.bf[4] == 1)
                WriteBootCode = 1;
            else
                WriteBootCode = 0;
#endif
#if (CEProduct == 1)
            WriteBootCode = 1;/* For CE Product, force to flash boot code */
#endif

        //must stop servo before flash write firmware!!!!
            if(St_no_disc == FALSE)
            {
                ServoStop();
            }
            DisableAllInterrupts();
            FlashWriteFW();
            break;
    }

    FlashLinearAdress = Iram_Linear_Xfer_addr;
}

/*--------------------------------------------------------------------
  Name: SaveCurrentTransferSettings()
  Description:
    Saves Current Transfer Mode setting value into DRAM Area .
            0x7E90~6 : Flash Up via IDE execution Flag
            0x7E97   : for St_dma_feature
            0x7E98   : for St_multi_dma
            0x7E99   : for St_ultra_dma
            0x7E9A~B : for Iram_Hw_host_blks_limit
            0x7E9C   : for IDENTIFY WORD-62(Lower Byte)
            0x7E9D   : for IDENTIFY WORD-63(Lower Byte)
            0x7E9E   : for IDENTIFY WORD-88(Lower Byte)
  Inputs:
    none
  Outputs:
    none
  Returns:
    none
--------------------------------------------------------------------*/
#if (FLASH_BY_DISC == 1)
void SaveCurrentTransferSettings( BYTE Mode)
{
    BYTE    tranData[15], i;

    for ( i = 0; i < TRAN_MODE_INFO_LEN; i++ )
    {
        tranData[i] = 0;
    }

    /* mark Flash Up execution */
    tranData[0] = 'F';
    tranData[1] = 'l';
    tranData[2] = 'a';
    tranData[3] = 's';
    tranData[4] = 'h';
    if (Mode == 0)
        tranData[5] = 'U';
    else if (Mode ==1)
        tranData[5] = 'D';
    tranData[6] = 0;

    /* makeing save data : Flag & Variable setting */
    tranData[7]  = St_dma_feature;                      /* 1=DMA feature enabled, 0=DMA feature disabled */
    tranData[8]  = St_multi_dma;                        /* 1=multiple_word DMA, 0=single_word DMA        */
    tranData[9]  = St_ultra_dma;                        /* 1= ultra DMA mode                             */
    tranData[10] = BHIGH(Iram_Hw_host_blks_limit);      /* Maximum blocks to xfr due to chip limitation  */
    tranData[11] = BLOW (Iram_Hw_host_blks_limit);

    /* makeing save data : Identify table setting */
    Iram_Buf_dram_addr = IDENTIFY_ADR + (62 * 2 + 1);
    tranData[12] = ReadSDRAM();                           /* Single Word DMA Transfer          */
    Iram_Buf_dram_addr = IDENTIFY_ADR + (63 * 2 + 1);
    tranData[13] = ReadSDRAM();                           /* Multi Word DMA Transfer           */
    Iram_Buf_dram_addr = IDENTIFY_ADR + (88 * 2 + 1);
    tranData[14] = ReadSDRAM();                           /* ENTER UDMA mode are not supported */

    /* save data into DRAM */
    Iram_Buf_dram_addr = TRAN_MODE_INFO_ADR;
    for ( i = 0; i < TRAN_MODE_INFO_LEN; i++ )
    {
        WriteSDRAM(tranData[i]);
    }

}
#else
void SaveCurrentTransferSettings()
{
    BYTE    tranData[15], i;

    for ( i = 0; i < TRAN_MODE_INFO_LEN; i++ )
    {
        tranData[i] = 0;
    }

    /* mark Flash Up execution */
    tranData[0] = 'F';
    tranData[1] = 'l';
    tranData[2] = 'a';
    tranData[3] = 's';
    tranData[4] = 'h';
    tranData[5] = 'U';
    tranData[6] = 0;

    /* makeing save data : Flag & Variable setting */
    tranData[7]  = St_dma_feature;                      /* 1=DMA feature enabled, 0=DMA feature disabled */
    tranData[8]  = St_multi_dma;                        /* 1=multiple_word DMA, 0=single_word DMA        */
    tranData[9]  = St_ultra_dma;                        /* 1= ultra DMA mode                             */
    tranData[10] = BHIGH(Iram_Hw_host_blks_limit);      /* Maximum blocks to xfr due to chip limitation  */
    tranData[11] = BLOW (Iram_Hw_host_blks_limit);

    /* makeing save data : Identify table setting */
    Iram_Buf_dram_addr = IDENTIFY_ADR + (62 * 2 + 1);
    tranData[12] = ReadSDRAM();                           /* Single Word DMA Transfer          */
    Iram_Buf_dram_addr = IDENTIFY_ADR + (63 * 2 + 1);
    tranData[13] = ReadSDRAM();                           /* Multi Word DMA Transfer           */
    Iram_Buf_dram_addr = IDENTIFY_ADR + (88 * 2 + 1);
    tranData[14] = ReadSDRAM();                           /* ENTER UDMA mode are not supported */

    /* save data into DRAM */
    Iram_Buf_dram_addr = TRAN_MODE_INFO_ADR;
    for ( i = 0; i < TRAN_MODE_INFO_LEN; i++ )
    {
        WriteSDRAM(tranData[i]);
    }

}
#endif

/********************** Command Processing subroutines ********************/
 /*---- MMC Sync Cache Command ----*/
void SyncCacheCommand(void)
{
     //[BTC][B001]Bios merge [B10F]
     if (pdbDiscLoaded() == BD_DISC) //winFinal test
     {
	AutoCommandComplete();
	return;
     }   
    InvalidCommand();
}

/*---- MMC Write 10/12 Command ----*/
void WriteCommand(void)
{
    InvalidCommand();
}

/*---- MMC Format Unit Command ----*/
void FormatUnitCommand(void)
{
    InvalidCommand();
}

/*---- MMC Erase 10 Command ----*/
void Erase10Command(void)
{
    InvalidCommand();
}

/*---- MMC Reserve Track Command ----*/
void ReserveTrackCommand(void)
{
    InvalidCommand();
}

/*---- MMC Close Track/Session Command ----*/
void CloseSessionTrackCommand(void)
{
    InvalidCommand();
}

/*---- MMC Read Disc Info Command ----*/
void ReadDiscInfoCommand(void)
{

    switch(pdbDiscLoaded())
    {
        case CD_DISC:
            CDReadDiscInfoCommand();
            break;
        case DVD_DISC:
            DVDReadDiscInfoCommand();
            break;
#if(BD_ENABLE)
        case BD_DISC:
            BDReadDiscInfoCommand();
            break;
#endif
#if(ENABLE_HDDVD)
        case HD_DISC:
            HDReadDiscInfoCommand();
            break;
#endif
        default:
            InvalidCommand();
            break;
    }
}

/*---- MMC Read Track Info Command ----*/
void ReadTrackInfoCommand(void)
{
    switch(pdbDiscLoaded())
    {
        case CD_DISC:
            CDReadTrackInfoCommand();
            break;
        case DVD_DISC:
            DVDReadTrackInfoCommand();
            break;
        case BD_DISC:
#if (BD_ENABLE)
            BDReadTrackInfoCommand();
            break;
#endif
        case HD_DISC:
#if (ENABLE_HDDVD)
            HDReadTrackInfoCommand();
            break;
#endif
        default:
            InvalidCommand();
            break;
    }

}

/*---- MMC Blank Command ----*/
void BlankCommand(void)
{
    InvalidCommand();
}

/*---- Erase Process ----*/
void EraseProcess(void)
{
}

/*---- Erase 10 Process ----*/
void Erase10Process(void)
{
}

/*---- Format Process ----*/
void FormatProcess(void)
{
}

/*---- Close Process ----*/
void CloseProcess(void)
{
}
/********************** End of Command Processing subroutines ****************/


#if (BD_ENABLE)
/*--------------------------------------------------------------------
  Name: LoadDefectListCommand()
  Description:
  Inputs:
  Outputs:
  Returns:
--------------------------------------------------------------------*/
static void LoadDefectListCommand(void)
{
    //ULONG   k;
    USHORT  i;
    USHORT  cnt;

    send_msg80(1,0x710081);

    /* Transfer Indicator */
    switch ( Iram_Cmd_block.bf[5] )
    {
        case 0x00:                                  /* Start */
            ClearFlashUpWorkAreaAll();
        case 0xFF:                                  /* Middle */
            cnt = 60;                               /* 60 * 1024Bytes = 60KBytes */
            break;

        default:
            cnt = Iram_Cmd_block.bf[5];
            break;
    }

    switch ( Iram_Cmd_block.bf[5] )
    {
        case 0x00:                                  /* Start */
            Iram_Buf_hxfer_blk = 1;
            Iram_Linear_Xfer_addr = 0L;
            break;
        default:                                    /* Middle or End */
            Iram_Linear_Xfer_addr = FlashLinearAdress;
            break;
    }

	#if(SUN_FUNC_TEST == 0)
    opReadyLED = (~opReadyLED);
	#else
         HAL_DSP_INVERT_LED();
       #endif

    i = cnt / 2;
    if ( ! GetFlashUpdateFromHost( i ) )
    {
        BUILD_SENSE(0x04,0x44,0x00,0x1b);           /* internal logical unit failure */
        return;
    }
    Iram_Linear_Xfer_addr += (cnt * 1024);          /* Update address */

	#if(SUN_FUNC_TEST == 0)
    opReadyLED = (~opReadyLED);
	#else
         HAL_DSP_INVERT_LED();
       #endif

    switch ( Iram_Cmd_block.bf[5] )
     {
        case 0x00:      /* Start  */
            vWakeUpFromPowerSaveMode();
            break;
        case 0xFF:      /* Middle */
            break;
        default:        /* End    */
            BUILD_SENSE(0x00,0x00,0x00,0x06);            // Clear Sense Bytes
            break;
    }

    FlashLinearAdress = Iram_Linear_Xfer_addr;
}
#endif /* BD_ENABLE */

/*--------------------------------------------------------------------
  Name: BYTE CheckDiscStatusForEjectKeyPressed(void)
  Description: check if it can execute tray-in/out operation of eject-key in current disc status.
  Inputs:
  Outputs:
  Returns: TRUE, it can tray-in/out in current disc status.
              FALSE, it can't tray-in/out in current disc status.
--------------------------------------------------------------------*/
BYTE CheckDiscStatusForEjectKeyPressed(void)
{
    return TRUE;
}

/*--------------------------------------------------------------------
  Name: void CheckReadAhead(void)
  Description: check if it needs to pre-buffering.
  Inputs:
  Outputs:
  Returns:
--------------------------------------------------------------------*/
void CheckReadAhead(void)
{
    if((ataPowerMode != STANDBY_MODE) && (TrayManagerInfo.TrayState == TRAY_STATE_IN_OK))
    {
        switch(pdbDiscLoaded())
        {
#if(BACKGROUND_READ_AHEAD == 1)
            case CD_DISC:
                CDCheckReadAhead();
            break;
#endif

#if (DVD_READ_AHEAD_ENABLED == TRUE)
            case DVD_DISC:
                    dvdCheckReadAhead();
            break;
#endif

#if  (BD_READ == 1) && (BD_READ_AHEAD_ENABLED == TRUE)
            case BD_DISC:
                if (pdbGetBDType() == BD_ROM)
                    bdCheckReadAhead();
            break;
#endif

#if  (ENABLE_HDDVD == 1) && (DVD_READ_AHEAD_ENABLED == TRUE)
            case HD_DISC:
                hdCheckReadAhead();
            break;
#endif
            default:
                //send_msg5SValue(1, "#ChRA", 4, (ataPowerMode << 16) + (TrayManagerInfo.TrayState << 8) + pdbDiscLoaded());// for debug
            break;
        }
    }
}

/*--------------------------------------------------------------------
  Name: void ReBuildCheckSum(void)
  Description: Rebuild Check Sum of Protect Area after update FW by a ROM WRITER 
  Inputs:
  Outputs:
  Returns:
--------------------------------------------------------------------*/
void ReBuildCheckSum(void)
{
    BYTE EofSting[16] = {FIRMW_INQ_PRODUCT};

    if((EndOfFirmWare[14] == EofSting[14])&&(EndOfFirmWare[15] == EofSting[15]))
    { // update by a ROM WRITER
        ULONG   Index;
        vULONG *Flash_addr4B;
        vBYTE  *flash_addr;
        USHORT chksum_DRAM;
        
        DisableAllInterrupts();

        flash_addr = (vBYTE *)FLASH_BASE;
        Flash_addr4B = (vULONG *)FLASH_BASE;
            
        chksum_DRAM = 0;

        for (Index = (CHECK_SUM_PROTECT_START ); Index < (CHECK_SUM_PROTECT_END + 1); Index++)
        {
            chksum_DRAM += (USHORT)flash_addr[Index];     // Calculate area from top of Persistent Table to top of Flash
        }

        chksum_DRAM = chksum_DRAM + flash_addr[FLASH_ROM_SIZE-2] + flash_addr[FLASH_ROM_SIZE-1];

        for(Index = 0; Index < FLASH_ROM_SIZE/4; Index++)
        {
            SCSDRAM32[Index] = *Flash_addr4B;
            Flash_addr4B++;
        }

        Index = FLASH_ROM_SIZE - 2L;

        SCSDRAM[Index] = (vBYTE)(chksum_DRAM>>8);
        SCSDRAM[Index + 1L] = (vBYTE)chksum_DRAM;

        FlashWriteFW(); // Update check sum to EOF
    }
}

