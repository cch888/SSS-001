/****************************************************************************
*                (c) Copyright 2001 - 2008 SUNEXT DESIGN, INC.
*                           All Right Reserved
*
*
* FILENAME:   at_int.c
*
* DESCRIPTION
*   This file contains the IRQ0 & IRQ1 interrupt service routines
*
* NOTES:
*
* $Revision: 86 $
* $Date: 11/03/28 2:37p $
*
**************************** SUNEXT CONFIDENTIAL *******************************/

#include ".\common\globals.h"
#include ".\common\globtype.h"
#include ".\common\chip.h"
#include ".\common\commath.h"
#include ".\servo\svo.h"
#include ".\hif\gp_ram.h"
#include ".\hif\gp_equ.h"
#include ".\hif\at_equ.h"
#include ".\hif\gp_mac.h"
#include ".\hif\gp_read.h"
#include ".\hif\cstart.h"
#include ".\hif\rd_int.h"
#include ".\hif\bf_util.h"
#include ".\hif\gp_util.h"
#include ".\hif\at_util.h"
#include ".\hif\at_knl.h"
#include ".\hif\at_cmds.h"
#if (AUDIO_PLAYBACK == 1)
#include ".\hif\nx_audio.h"
#endif
#include ".\hif\rd_util.h"
#include ".\hif\GP_AUDIO.h"
#include ".\hif\mmc.h"
#include ".\hif\gp_inqry.h"

#if EN_POWER_CONTROL
#include ".\hif\powerctl.h"
#endif

#include ".\al\reg_hal.h"
#include ".\al\reg_global.h"
#include ".\al\reg_intctrl.h"
#include ".\al\reg_sdram.h"
#include ".\al\reg_host.h"
#include ".\hif\gp_key.h"
void CheckSramStack (void);

extern void ResetCommandFIFO(void);
extern void ataCheckDMA(void);


/*********************************************************************
* ATACommandIntProc
*
* Service ATA command. Decode and execute.
*********************************************************************/
void    ATACommandIntProc(void)
{
    ULONG CmdTime;
    BYTE    a;

    if(ataPowerMode == SLEEP_MODE){
        WRITE_REG(HAL_HSTREG_MAP->ATFEA,  0x00);  /* No Error */
        WRITE_REG(HAL_HST_DRDYW_REGISTER,  0x10);  /* set DRDY, DSC */
        ATAClearBusy();
        return;
    }

    WRITE_REG(HAL_HSTREG_MAP->ATFEA,  0x00);  /* No Error */
    WRITE_REG(HAL_HST_DRDYW_REGISTER,  0x50); /* set DRDY, DSC */

    a = READ_REG(HAL_HSTREG_MAP->ATCMD);      /* get the command code */

	if (a == ATA_NOP)
	{
		WRITE_REG(HAL_HSTREG_MAP->REQTXCNTL,0x0B40);
	}

    SendMsgCn(SEND_ATAPI_CMD,1,0x800310,a);
    CmdTime = ReadTimer(&Iram_atapi_timer);
    SendMsgCnB(SEND_ATAPI_CMD,5,0x800310,B1B(a),B4B(CmdTime)); // ATA Command, opCode = %02Xh, Time = %d ms    
    StartTimer(&Iram_atapi_timer);   

    if(a ==  ATA_IDENTIFY_PACKET_DEVICE)    /* Identify command */
    {
        WRITE_REG(HAL_HSTREG_MAP->HICTL,Iram_shHICtl);
        ataIdentifyPacketDeviceCommand();
        return;
    }
    ataCheckDMA();
    Knl_atcmd_avail = TRUE;                 /* mark ATA cmd available */
    Knl_cmd_avail = TRUE;                   /* mark command need service */
    Iram_Cmd_block.bf[0] = a;
    /* for BootCD disc no boot in CDROM device */
    if(Iram_Cmd_block.bf[0]==ATA_SET_FEATURE){
        Knl_cmd_avail = FALSE; /* did not find drive in XP */
        ataSetFeatureCommand();
    }
    /* for BootCD disc no boot in CDROM device */
}


/*********************************************************************
* ATAPIDeviceResetCommand
*
* Execute  ATAPI Reset command (08)
*********************************************************************/
extern void ResetHandler(void);
extern BYTE pre_tno;
#if (FLASH_BY_DISC == 1)
extern void SaveCurrentTransferSettings(BYTE Mode);  // Mode 0: normal , Mode 1: disc flush
extern BYTE FlashByDisc_flag;
#else
extern void SaveCurrentTransferSettings(void);
#endif

void ATAPIDeviceResetNoFIS34(void) //No return FIS34.
{
    send_msg80(SEND_ATAPI_CMD,0x710001);

#if (FLASH_BY_DISC == 1)
    if(FlashByDisc_flag == 1)
    {
        InitATATaskFileRegisters();

        ATAClearBusy();

        WRITE_REG(HAL_HST_DRDYW_REGISTER,  0x00);

        return;
    }
#endif
    /*-----------------------------------------------------*/
    /*  Stop decoder                                       */
    /*-----------------------------------------------------*/
    /* When get ARST command then stop decoder first! */
#if (AUDIO_PLAYBACK == 1)
    if (St_nx_audio == TRUE)
    {
        Iram_audio_scan_st = FALSE;
        St_nx_audio = FALSE;
        pre_tno = 0xff;
        AudioTransferDone();
    }
#endif
    ClearDataBuffer();
    StopDecoder();
    Iram_Rw_SOTC_st = FALSE;
    St_unit_atten = TRUE;
    if ((St_cmd_inproc == TRUE) || (Iram_Send_Giveup_F == TRUE)||(Iram_St_fast_cache_hit == TRUE))    /* Fast cache hit and Reset Come in, reset/init chip */
    {
        St_cmd_inproc   = FALSE;
        St_send_pardata = FALSE;
        St_send_blkdata = FALSE;
        Iram_Send_Giveup_F = TRUE;
        ResetCommandFIFO();
        svoIramVar.knlTimeupCount = 0xFFFF;
        if(Iram_St_fast_cache_hit != TRUE)
            ATA_SRST_rcv = TRUE;

        Iram_St_fast_cache_hit = FALSE;
    }
#if (CEProduct == 1)
    else{ /* For CEProduct use only, prevent the backend does not send command any more. */
    	if (Rw_rd_monitor == TRUE){
		    ATA_SRST_rcv = TRUE;
   		}
    }
#endif
    if(ataPowerMode == SLEEP_MODE){
        if(Ata_sleep_wait) {
        #if (EN_POWER_CONTROL == 1)
			PowerControl(eStandbyPower);
        #else
            ataPowerMode = STANDBY_MODE;
        #endif
            Ata_sleep_wait = FALSE;
        }
        else {
        #if (EN_POWER_CONTROL == 1)
			PowerControl(eIdlePower);
        #else
            ataPowerMode = IDLE_MODE;
        #endif
            /* Save the current transfer mode before reset drive. */
#if (FLASH_BY_DISC == 1)
            SaveCurrentTransferSettings(0);
#else
            SaveCurrentTransferSettings();
#endif
            ResetHandler();
        }
    }

    InitATATaskFileRegisters();

    WRITE_REG(HAL_HSTREG_MAP->ATDRS, 0x00);    /* SATA signature for AHCI */
    WRITE_REG(HAL_HSTREG_MAP->ATBLO, 0xEB14);  /* BYTE count reg  */

    ATAClearBusy();

    WRITE_REG(HAL_HST_DRDYW_REGISTER,  0x00);
    //Start == For Tray is not closed after Warm-boot system.  
    if(St_door_open && (TrayManagerInfo.Executing == FALSE)) 
    {
        svoIramVar.emergencyStopFlag = TRUE;
        TrayManagerInfo.Command = TRAY_COMMAND_TRAY_IN;
    }
    //end == For Tray is not closed after Warm-boot system.  

	Svr_pause_reqire = TRUE;

    WRITE_FIELD(HAL_HST_CLRBSY,1);
    {
    	BYTE v1;
        for(v1 = 0; v1 < 0x10; v1++);                      /* Need delay to allow BUSY to be cleared before triggering Register FIS */
    }
}

void ATAPIDeviceResetCommand(void)
{
    ULONG CmdTime;

    CmdTime = ReadTimer(&Iram_atapi_timer);
    SendMsgCnB(SEND_ATAPI_CMD,4,0x710001,B4B(CmdTime)); 
    StartTimer(&Iram_atapi_timer); 
    
    ATAPIDeviceResetNoFIS34();//WarnBoot fail in Southbridge is ICH5 
    WRITE_REG(HAL_HSTREG_MAP->RQTXFSTYP,0x34);         /* Setup Dev to Host Register FIS */
    WRITE_REG(HAL_HSTREG_MAP->TXREQ,HAL_HST_TXFISREQ); /* Trigger FIS */
}

/**********************************************************************
* ataExecuteDiagnosticCommand()
*
* Execute diagnostic command performs:
*   - Diagnostic Operation
*
* Note:
*   (1) DSC bit should NOT be changed according to ATAPI Spec. Because
*       immediate seek operation may be still in process in the background
*   (2) DRV bit should be set to 0 becuase master drive is responsible
*       for reporting Diag. status. H/W will not auto clear
*       DRV bit while   diagnostic command is detected)
*   (3) No ATAPI signature will be provided
*
************************************************************************/
void ataExecuteDiagnosticCommand(void)
{
    if(ataPowerMode == SLEEP_MODE)
    {
        WRITE_REG(HAL_HSTREG_MAP->ATFEA,  0x00);  /* No Error */
        WRITE_REG(HAL_HST_DRDYW_REGISTER,  0x10);  /* set DRDY, DSC */
        ATAClearBusy();
        return;
    }
    WRITE_REG(HAL_HSTREG_MAP->HICTL,Iram_shHICtl);
    DelaymS(1);         // TBD? This delay was probably for PDIAG or DASP not needed for SATA?

    /* ATAPI signature */
    WRITE_REG(HAL_HSTREG_MAP->ATINT, 0x01);         /* Interrupt reason reg */
    WRITE_REG(HAL_HSTREG_MAP->ATSPA, 0x01);         /* sector number reg   */
    WRITE_REG(HAL_HSTREG_MAP->ATBLO, 0xEB14);       /* BYTE count reg  */
    /* SATA is always Master only */
    WRITE_REG(HAL_HSTREG_MAP->ATFEA,  0x01);
    WRITE_REG(HAL_HST_DRDYW_REGISTER,  0x00);
    WRITE_FIELD(HAL_HST_CDRDY,  0);
    ATAClearBusy();
    {
    	BYTE v1;
    	for(v1 = 0; v1 < 0x10; v1++);          	    /* Need delay to allow BUSY to be cleared before triggering Register FIS */
    }
    WRITE_REG(HAL_HSTREG_MAP->ATRG,HAL_HST_SCTRG);  /* STATUS_COMPLETION   */
}

/*********************************************************************
* ATASoftResetService()
*
* SRST bit is set
*   - No actual reset will ocurred.
*   - No command will be stopped or aborted
*   - No perform diagnostic operation
*
*   - DSC bit of wATASts should NOT be changed according to ATAPI
*     Spec. Because immediate seek operation may be still in porcess
*     in the background
*   - DRV bit of rwATADSel should be set to 0 for master drive is
*     responsible for reporting diag. status.
*   - ATAPI signature will be provided
*********************************************************************/
void    ATASoftResetService(void)
{
    ULONG   CmdTime;
    BYTE    v1;
#if (SET_SLOT_IN == 1)
    BYTE     x; //[BTC][Q36]Jam when power on ,Disc auto tray out.
#endif

    CmdTime = ReadTimer(&Iram_atapi_timer);
    SendMsgCnB(SEND_ATAPI_CMD,4,0x710002,B4B(CmdTime)); //ATA SOFT RESET
    StartTimer(&Iram_atapi_timer);    

    /*-----------------------------------------------------*/
    /*  Stop decoder                                       */
    /*-----------------------------------------------------*/
    /* When get SRST command then stop decoder first! */
#if (AUDIO_PLAYBACK == 1)
    if (St_nx_audio == TRUE)
    {
        Iram_audio_scan_st = FALSE;
        St_nx_audio = FALSE;
        pre_tno = 0xff; /* Check tno protect */
        AudioTransferDone();
    }
#endif // AUDIO_PLAYBACK
    ClearDataBuffer();
    StopDecoder();
    if ((St_cmd_inproc == TRUE) || (Knl_cmd_avail == TRUE)||(Iram_St_fast_cache_hit == TRUE))    /* Fast cache hit and Reset Come in, reset/init chip */
    {
        St_cmd_inproc   = FALSE;
        St_send_pardata = FALSE;
        St_send_blkdata = FALSE;
        Iram_Send_Giveup_F   = TRUE;
        ResetCommandFIFO();
        svoIramVar.knlTimeupCount = 0xFFFF;
        if((Knl_cmd_avail == FALSE)&&(Iram_St_fast_cache_hit == FALSE))    /* fast cache hit case do not set ATA_SRST_rcv = TRUE */
            ATA_SRST_rcv = TRUE;
        Knl_cmd_avail = FALSE ;
        Iram_St_fast_cache_hit = FALSE;        /* clear fast cache flag */
    }
    else if (Iram_Send_Giveup_F == TRUE)
    {
        #if (EN_POWER_CONTROL == 1)
			PowerControl(eStandbyPower);
        #else
            ataPowerMode = STANDBY_MODE;
        #endif
#if (FLASH_BY_DISC == 1)
            SaveCurrentTransferSettings(0);
#else
            SaveCurrentTransferSettings();
#endif
        ResetHandler();
    }

    WRITE_REG(HAL_HSTREG_MAP->HICTL,Iram_shHICtl);
    DelaymS(1);     // TBD? delay probably not necessary for SATA

    do
    {
        v1 = READ_REG(HAL_HSTREG_MAP->RSSTAT);
    }
    while( TEST_BIT(v1, 7) );

    /* ATAPI signature */
    WRITE_REG(HAL_HSTREG_MAP->ATINT, 0x01);         /* Interrupt reason reg */
    WRITE_REG(HAL_HSTREG_MAP->ATSPA, 0x01);         /* sector number reg   */
    WRITE_REG(HAL_HSTREG_MAP->ATBLO, 0xEB14);       /* BYTE count reg  */
    /* SATA is always Master only */
    WRITE_REG(HAL_HSTREG_MAP->ATFEA,  0x01);
    WRITE_REG(HAL_HST_DRDYW_REGISTER,  0x00);
    ATAClearBusy();

    if(ataPowerMode == SLEEP_MODE){
        if(Ata_sleep_wait) {
        	#if (EN_POWER_CONTROL == 1)
				PowerControl(eStandbyPower);
        	#else
                ataPowerMode = STANDBY_MODE;
        	#endif
            Ata_sleep_wait = FALSE;
        }
        else {
        	#if (EN_POWER_CONTROL == 1)
				PowerControl(eIdlePower);
        	#else
                ataPowerMode = IDLE_MODE;
        	#endif
#if (FLASH_BY_DISC == 1)
            SaveCurrentTransferSettings(0);
#else
            SaveCurrentTransferSettings();
#endif
            ResetHandler();
        }
    }

    St_dma_mode = FALSE;                        /* set to PIO mode */
    Iram_shHDDir &= (~ HAL_HST_DMAMODE);        /* ~mskDMAMode=F3, to set PIO mode */
    WRITE_REG(HAL_HSTREG_MAP->HDDIR, Iram_shHDDir);

#if (AUDIO_PLAYBACK == 1)
    if (St_nx_audio == TRUE)
    {
        Iram_audio_scan_st = FALSE;
        St_nx_audio = FALSE;
        pre_tno = 0xff;
        AudioTransferDone();
    }
#endif // AUDIO_PLAYBACK
    ClearDataBuffer();
    StopDecoder();
    
#if (SET_SLOT_IN == 1)
	if(fgSRSTEj) //[BTC][Q36]Jam when power on ,Disc auto tray out.
	{
	   for(x=0;x<5;x++)
	   {
	   	DelaymS(250);
		DelaymS(250);
		DelaymS(250);
		DelaymS(250);
	   }
	}
#endif

    //Start == For Tray is not closed after Warm-boot system.  
    if(St_door_open && (TrayManagerInfo.Executing == FALSE)) 
    {
        svoIramVar.emergencyStopFlag = TRUE;
        TrayManagerInfo.Command = TRAY_COMMAND_TRAY_IN;
    }    
    //End == For Tray is not closed after Warm-boot system. 
   
    Svr_pause_reqire = TRUE;

    WRITE_FIELD(HAL_HST_CLRBSY,1);
    for(v1 = 0; v1 < 0x10; v1++);                      /* Need delay to allow BUSY to be cleared before triggering Register FIS */

    WRITE_REG(HAL_HSTREG_MAP->RQTXFSTYP,0x34);         /* Setup Dev to Host Register FIS */
    WRITE_REG(HAL_HSTREG_MAP->TXREQ,HAL_HST_TXFISREQ); /* Trigger FIS */
}

/*********************************************************************
* EjectSwitchISR()
*
* Eject Switch Interrupts: Unused
*********************************************************************/
void    EjectSwitchISR(void)
{
#if (EN_POWER_CONTROL == 0)
    vBOOL temp_IE; /* write register protect */
#endif
    if( ataPowerMode == SLEEP_MODE || ataPowerMode == STANDBY_MODE)
    {
        if(Ata_sleep_wait)
        {
            ataPowerMode = STANDBY_MODE;
            Ata_sleep_wait = FALSE;
            return;
        }
        else
        {
#if (EN_POWER_CONTROL)
            PowerControl(eIdlePower);
#else
            temp_IE = GET_INT_ENABLE_MASK();
            DISABLE_INTERRUPT();
            /* Initial default power */
            WRITE_REG(HAL_GLOBALREG_MAP->CLKSTP1,0x00); /* Enable all Clocks */
            WRITE_REG(HAL_GLOBALREG_MAP->CLKSTP2,0x00);
            WRITE_REG(HAL_GLOBALREG_MAP->CLKSTP3,0x00);
            WRITE_REG(HAL_GLOBALREG_MAP->CLKSTP4,0x00);
            WRITE_REG(HAL_GLOBALREG_MAP->CLKSTP5,0x00);

            WRITE_REG(HAL_GLOBALREG_MAP->PWRDWN0,0x00);  /* normal power mode, all blocks enabled */
            WRITE_REG(HAL_GLOBALREG_MAP->PWRDWN1,EFMOSCEN);         /* Enable EFM and OSCEN IO pads */
            WRITE_REG(HAL_GLOBALREG_MAP->PWRDWN2,0x00);
            WRITE_REG(HAL_GLOBALREG_MAP->PWRDWN3,0x00);

            SET_INT_ENABLE_MASK(temp_IE);
#endif
        }
    }
    else
    {
#if (MSG_OUT_MODE == MSG_USBPORT)
        if (hifDebugMsgFlag & SEND_SRAM_EMPTY_STACK)
            CheckSramStack();
#endif
    }

    if(EjectKeyInfo.Disable == FALSE)
    {
        EjectKeyInfo.Pressed = TRUE;
    }
    DISABLE_EXT_INT();
}

/*********************************************************************
* CheckSramStack()
* Description: this function is used for check sram empty space in each stack.
                     After running some process, and push Eject Key,
                     it will send every stack empty space.
* Input: None
* Output: None
*********************************************************************/
extern ULONG hif_stack[];
extern ULONG player_stack[];
extern ULONG servo_stack[];
extern ULONG tp_stack[(TIMER_STACK_SIZE) / 4];// TBD? why did this get changed to /4?

void ComresetIntProc(void)
{
    ULONG CmdTime;
    USHORT i;

    CmdTime = ReadTimer(&Iram_atapi_timer);
    SendMsgCnB(SEND_ATAPI_CMD,4,0x710602,B4B(CmdTime)); //COMRESET
    StartTimer(&Iram_atapi_timer);    

    SATA_FE &= (BYTE)(~bINTFPWR); //Disable DIPM by default
#if (DIPM_BY_FW == 0)
	WRITE_FIELD(HAL_HST_DIPM_EN,0x00);
#endif
    SATA_FE &= (BYTE)(~bASYNNOTI);//Disable AN by default
    SATA_FE |= (BYTE)(bSSP);//Enable SSP
    SATA_intf_pwr_state = 0;

    //We are in COMRESET. If SATA bus is sleeping, then wake it up.
    if ((READ_REG(HAL_HSTREG_MAP->SATASTAT0)&(HAL_HST_SLUMBER|HAL_HST_PARTIAL)) != 0)
    {
        SendMsg80(1,0x710404);               // Wake Up SATA Bus From PMREQ mode
        WRITE_FIELD(HAL_HST_DWCLKON, 1);
        WRITE_FIELD(HAL_HST_TXWKUPREQ, 1);
        //DelaymS(1);
        for (i=0; i<1000; i++)
        {
            if((READ_REG(HAL_HSTREG_MAP->SATASTAT1)&(HAL_HST_LINKRDY|HAL_HST_PHYRDY))==(HAL_HST_LINKRDY|HAL_HST_PHYRDY))
                break;
        }

        #if ((EN_AUTO_FIS34 == 1) &&(CHIP_REV >= 0xC0))
        ATAClearBusy();
        WRITE_FIELD(HAL_HST_COMRSTINT,0); /* Clear ComReset Status before send FIS34 */
        WRITE_REG(HAL_HSTREG_MAP->ATRG, HAL_HST_CTTRG);  /* Set ATAPI signature */
        WRITE_REG(HAL_HSTREG_MAP->RQTXFSTYP, 0x34);      //DToHRegFIS
        WRITE_FIELD(HAL_HST_TXFISREQ, 0x01);
        #endif
        
    }
    else
    {
#if ((EN_AUTO_FIS34 == 0) || (CHIP_REV <= 0xB3))          
        for (i=0; i<10000; i++); //This delay is only suitable for manual FIS34 mode.
#endif        
    }

    /* Auto FIS34 Work Around.*/
    if((READ_REG(HAL_HSTREG_MAP->SATASTAT1)&(HAL_HST_LINKRDY|HAL_HST_PHYRDY))==(HAL_HST_LINKRDY|HAL_HST_PHYRDY))
    {
#if ((EN_AUTO_FIS34 == 0) || (CHIP_REV <= 0xB3))        
        SendMsg80(1,0x710604);//PHYRDYWA!
        //WRITE_FIELD(HAL_HST_DISF34CNT, 1);
        ATAClearBusy();
        
        WRITE_FIELD(HAL_HST_COMRSTINT,0); /* Clear ComReset Status before send FIS34 */
        WRITE_REG(HAL_HSTREG_MAP->ATRG, HAL_HST_CTTRG);  /* Set ATAPI signature */
        WRITE_REG(HAL_HSTREG_MAP->RQTXFSTYP, 0x34);      //DToHRegFIS
        WRITE_FIELD(HAL_HST_TXFISREQ, 0x01);
        WRITE_FIELD(HAL_HST_AFIS34DIS, 1);
        SendFis34Already = 1;
#endif
        St_unit_atten = TRUE; //Indicate a Bus RESET has occured.
    }
//20101123_02WR ==start== 1. DELL PC insprion560, after boot up by disc and warmboot drive disappear issue: Reset Sata block during comreset interrupt and PHY not ready.
//20101221_030B ==start==Remove 02WR item1.(this issue can be fixed in version:02YA+02YZ).This workaround will cause T3500 S3 fail.(Drive send unkonw FIS after PHY ready).
//    else
//    {
//        WRITE_REG(HAL_HSTREG_MAP->FWRST, HAL_HST_TXFIFPTR | HAL_HST_RXFIFPTR | HAL_HST_RAM | HAL_HST_HST);
//        WRITE_REG(HAL_HSTREG_MAP->FWRST, 0x00);
//    //    SendMsg80(1,0x710615); //"SATA Block Reset"
//    }
//20101221_030B ==end==Remove 02WR item1.(this issue can be fixed in version:02YA+02YZ).This workaround will cause T3500 S3 fail.(Drive send unkonw FIS after PHY ready).
//20101123_02WR ==end== 1.DELL PC insprion560, after boot up by disc and warmboot drive disappear issue: Reset Sata block during comreset interrupt and PHY not ready.
    //Boot CD fail in Boston,NARRA5 PC. TC modify, Elvis comfirmed.
    if ((St_cmd_inproc == TRUE)  ||(Iram_St_fast_cache_hit == TRUE))//|| READ_REG(HAL_HSTREG_MAP->RAMWDCCL)
    {
        BIST_In_Process = 0;

        #if (ENABLE_SATA_FE == 1)
        if (SATA_FE & bSSP)
            ;//SaveCurrentTransferSettings();
        else SATA_FE |= bSSP;
        #endif

        WRITE_REG(HAL_HSTREG_MAP->FWRST, HAL_HST_TXFIFPTR | HAL_HST_RXFIFPTR | HAL_HST_RAM | HAL_HST_HST);
        WRITE_REG(HAL_HSTREG_MAP->FWRST, 0x00);
        ATAPIDeviceResetNoFIS34();
    }

    if(St_door_open && (TrayManagerInfo.Executing == FALSE))
    {
        svoIramVar.emergencyStopFlag = TRUE;
        TrayManagerInfo.Command = TRAY_COMMAND_TRAY_IN;
    }

    EjectKeyInfo.Disable = FALSE;
    TrayManagerInfo.PersistentPreventState = 0;
    TrayManagerInfo.TrayPreventAllowState = TRAY_PREVENT_ALLOW_STATE_UNLOCKED;
    
    //For Dell boot test. We will set DISSYNCP back to 0 when any ATAPI cmd is recept.
    WRITE_FIELD(HAL_HST_DISSYNCP, 0x01);  //Reset link layer when receiving SYNC primitive
}

void CheckSramStack (void)
{
    /*---------------------------------------------------------------------
    *   If you want to use this function,
    *        you have to make sure each stack start address in (project name).MAP.
    *   And make define in following line :
    *---------------------------------------------------------------------*/
    BYTE *ptr, temp, temp1;
    USHORT i;

    send_msg80(SEND_SRAM_EMPTY_STACK,0x71007F);
    //send HIF empty stacks
    for(i = 0; i< HIF_STACK_SIZE; i ++)
    {
        ptr = ((BYTE *)(SRAM_BASE + (ULONG)(hif_stack) + i));
        temp = *ptr;

        if(i == 0)
            temp1 = temp;

        if(temp != temp1)
        {
            SendMsgCnB(SEND_HIF_CMD,2,0x800E01,B2B(i));
            break;
        }
    }
    //send PLR empty stacks
    for(i = 0; i< PLAYER_STACK_SIZE; i ++)
    {
        ptr = ((BYTE *)(SRAM_BASE + (ULONG)(player_stack) + i));
        temp = *ptr;

        if(i == 0)
            temp1 = temp;

        if(temp != temp1)
        {
            SendMsgCnB(SEND_HIF_CMD,2,0x800E02,B2B(i));
            break;
        }
        #if ((EN_AUTO_FIS34 == 1) &&(CHIP_REV >= 0xC0))
        ATAClearBusy();
        WRITE_FIELD(HAL_HST_COMRSTINT,0); /* Clear ComReset Status before send FIS34 */
        WRITE_REG(HAL_HSTREG_MAP->ATRG, HAL_HST_CTTRG);  /* Set ATAPI signature */
        WRITE_REG(HAL_HSTREG_MAP->RQTXFSTYP, 0x34);      //DToHRegFIS
        WRITE_FIELD(HAL_HST_TXFISREQ, 0x01);
        #endif
    }
    //send SVO empty stacks
    for(i = 0; i< SERVO_STACK_SIZE; i ++)
    {
        ptr = ((BYTE *)(SRAM_BASE + (ULONG)(servo_stack) + i));
        temp = *ptr;

        if(i == 0)
            temp1 = temp;

        if(temp != temp1)
        {
            SendMsgCnB(SEND_HIF_CMD,2,0x800E03,B2B(i));
            break;
        }
    }
    //send TP empty stacks
    for(i = 0; i< TIMER_STACK_SIZE; i ++)
    {
        ptr = ((BYTE *)(SRAM_BASE + (ULONG)(tp_stack) + i));
        temp = *ptr;

        if(i == 0)
            temp1 = temp;

        if(temp != temp1)
        {
            SendMsgCnB(SEND_HIF_CMD,2,0x800E04,B2B(i));
            break;
        }
    }
}



