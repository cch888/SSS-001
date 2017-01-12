/****************************************************************************
*                (c) Copyright 2001 - 2005 SUNEXT DESIGN, INC.
*                           All Right Reserved
*
*
* FILENAME:   at_cmd.c
*
* DESCRIPTION
*   This file contains command handler of AT commands
*
* NOTES:
*
* $Revision: 78 $
* $Date: 11/03/21 10:10a $
*
**************************** SUNEXT CONFIDENTIAL *******************************/
#include ".\common\globals.h"
#include ".\common\globtype.h"
#include ".\common\chip.h"
#include ".\common\system.h"
#include ".\hif\gp_ram.h"
#include ".\hif\gp_equ.h"
#include ".\hif\at_equ.h"
#include ".\hif\gp_mac.h"
#include ".\hif\cstart.h"
#include ".\hif\at_util.h"
#include ".\hif\at_knl.h"
#include ".\hif\sv_util.h"
#include ".\hif\gp_audio.h"
#include ".\hif\bf_util.h"
#include ".\hif\at_cmds.h"
#include ".\hif\mmc.h"
#include ".\oem\oem_spd_tbl.h"
#include ".\al\reg_global.h"
#include ".\al\reg_intctrl.h"
#include ".\al\reg_servo.h"
#include ".\al\reg_write_strategy.h"

#if (EN_POWER_CONTROL)
#include ".\hif\powerctl.h"
#endif

/* Supported Automatic Acoustic Management */
static void AcousticSpeed(DiscSpeedT speed);
void SpinDownDisc(void);

extern vBYTE    Iram_shHDDir;

/*********************************************************************
* Function: Vendor Set_feature
*
* Provide host different operating options
**********************************************************************/

void VendorATASetFeatureCommand(void)
{
	BYTE FEAreg, SECreg;

	FEAreg = Iram_Cmd_block.bf[3];   /* read feature */
	SECreg = Iram_Cmd_block.bf[4];   /* read Sector Count */

	send_msg80(SEND_ATAPI_CMD,0x710308);
	//send_msg54(SEND_MRW_MSG, 'F','S',FEAreg , SECreg);
	SendMsgCnB(SEND_ATAPI_CMD,1,0x800600,B1B(FEAreg)); //bdVideoFlag
	SendMsgCnB(SEND_ATAPI_CMD,1,0x800601,B1B(SECreg)); //bdVideoFlag
	switch (FEAreg)
	{
		/* Supported Automatic Acoustic Management */
		case ENABLE_SATA_FEATURE:
			switch(SECreg)										/* read Sector Count reg.*/
			{
				case INIT_PWR_STATE_TRANS:						/* For SATA interface power management */
					SATA_FE |= bINTFPWR;
					break;
				case ASYN_NOTIFICATION:							/* Asynchronous Notification */
					SATA_FE |= bASYNNOTI;
					break;
				case SSP:										/* Software Settings Preservation */
					SATA_FE |= bSSP;
					break;
				default:
					send_msg80(SEND_ATAPI_CMD,0x71007A);
					return;
			}
			break;
		case DISABLE_SATA_FEATURE:
			switch(SECreg)										/* read Sector Count reg.*/
			{
				case INIT_PWR_STATE_TRANS:						/* For SATA interface power management */
					SATA_FE &= (BYTE)(~bINTFPWR);
					break;
				case ASYN_NOTIFICATION:							/* Asynchronous Notification */
					SATA_FE &= (BYTE) (~bASYNNOTI);
					break;
				case SSP:										/* Software Settings Preservation */
					SATA_FE &= (BYTE) (~bSSP);
					break;
				default:
					send_msg80(SEND_ATAPI_CMD,0x71007A);
					return;
			}
			break;
		default:
			send_msg80(SEND_ATAPI_CMD,0x71007B);
    	}
}

/*********************************************************************
* Vendor Standby Immed Command
*
* spin down motor and enter standby mode
**********************************************************************/
void VendorATAStandbyImmedCommand(void)
{

    send_msg80(SEND_ATAPI_CMD,0x71007C);
    svoIramVar.emergencyStopFlag = TRUE;

    while(HIF.PlayerStatus == BUSY){
        ExitProcess();
    }

    svoIramVar.emergencyStopFlag = FALSE;

    if (St_drv_ready == TRUE)
    {
        svoIramVar.knlTimeupCount = TIMEUP_NON;
        SpinDownDisc();
    }

    St_cmd_inproc = FALSE;

#if (EN_POWER_CONTROL == 1)
    PowerControl(eStandbyPower);
#else
    ataPowerMode = STANDBY_MODE;
#endif
}


/*********************************************************************
* Function: Set_feature
*
* Provide host different operating options
**********************************************************************/
void ataSetFeatureCommand(void)
{
    BYTE FEAreg, SECreg, mode_num;

    FEAreg = READ_REG(HAL_HSTREG_MAP->ATFEA);   /* read feature register */
    SECreg = READ_REG(HAL_HSTREG_MAP->ATINT);   /* read Sector Count register */

    switch (FEAreg)
    {
        case SET_XFR_MODE:/* 0x03 */
            mode_num = SECreg & 0x07;           /* mode number */
            switch(SECreg & 0xF8)               /* read Sector Count reg.*/
            {
                case DEF_XFR_MODE:              /* PIO default xfer mode */
                    if (SECreg & 0x07)
                    {
                        /* invalid mode */
                        ATACommandFail();
                        return;
                    }
                    break;

                case DISABLE_IORDY:             /* PIO default xfer mode, diable IORDY */
                    break;

                case PIO_XFR_MODE:              /* PIO flow control xfer mode X */
                    if(mode_num > MAX_PIO)
                    {       /* invalid mode */
                        ATACommandFail();
                        return;
                    }
                    /* chech if the current pio mode is the beyond MODE_LIMITATION */
                    if(mode_num > MODE_LIMITATION)
                        {   /* limit the xfer block */
                            Iram_Hw_host_blks_limit    = HW_MAX_MULTB_COOKED;
                        }
                    else
                        {
                            Iram_Hw_host_blks_limit    = HW_MAX_MULTB;
                        }
                    St_dma_mode = FALSE;
                    St_dma_feature = FALSE;
                    St_multi_dma   = FALSE;
                    St_ultra_dma   = FALSE;     /* pressume not ultra DMA */
                    break;

                case SINGLE_WORD_DMA:
                    if(mode_num > MAX_SINGLE_DMA)
                        {
                            ATACommandFail();         /* invalid mode */
                            return;
                        }
                    Iram_Buf_dram_addr = IDENTIFY_ADR + (62*2 + 1);
                    mode_num = 0x01 << mode_num;
                    WriteSDRAM(mode_num);
                    Iram_Buf_dram_addr = IDENTIFY_ADR + (63*2 + 1);
                    WriteSDRAM(0x00);
                    Iram_Buf_dram_addr = IDENTIFY_ADR + (88*2 + 1);
                    WriteSDRAM(0x00);

                    St_dma_feature = TRUE;
                    St_multi_dma   = FALSE;
                    St_ultra_dma   = FALSE;             /* pressume not ultra DMA */
                    break;

                case MULTI_WORD_DMA:
                    if(mode_num > MAX_MULTI_DMA)
                        {       /* invalid mode */
                            ATACommandFail();
                            return;
                        }
                    /* check if the MDMA mode is beyond MODE_LIMITATION */
                    if(mode_num > MODE_LIMITATION)
                        {
                            Iram_Hw_host_blks_limit    = HW_MAX_MULTB_COOKED;
                        }
                    else
                        {
                        Iram_Hw_host_blks_limit    = HW_MAX_MULTB;
                    }

                    Iram_Buf_dram_addr = IDENTIFY_ADR + (63*2 + 1);
                    mode_num = 0x01 << mode_num;
                    WriteSDRAM(mode_num);
                    Iram_Buf_dram_addr = IDENTIFY_ADR + (62*2 + 1);
                    WriteSDRAM(0x00);
                    Iram_Buf_dram_addr = IDENTIFY_ADR + (88*2 + 1);
                    WriteSDRAM(0x00);

                    St_dma_feature = TRUE;
                    St_multi_dma   = TRUE;
                    St_ultra_dma   = FALSE;     /* pressume not ultra DMA */
                    break;

                case UDMA_MODE:

                    switch(mode_num)
/*** SATA IF does not care about the UDMA mode. Still check for valid modes as required by ATAPI ***/
                    {
                        case 0:
                        case 1:
                        case 2:
                        //Start == SATA test fail. 	
                        case 3:
                        case 4:	
                        //end ==. 	
                        case 5:
                            break;

                        default:
                        {
                            ATACommandFail();     /* invalid mode */
                            return;
                        }
                    }

                    Iram_Buf_dram_addr = IDENTIFY_ADR + (88*2 + 1);
                    mode_num = 0x01 << mode_num;
                    WriteSDRAM(mode_num);
                    Iram_Buf_dram_addr = IDENTIFY_ADR + (62*2 + 1);
                    WriteSDRAM(0x00);
                    Iram_Buf_dram_addr = IDENTIFY_ADR + (63*2 + 1);
                    WriteSDRAM(0x00);

                    St_dma_feature = TRUE;
                    St_multi_dma   = FALSE;
                    St_ultra_dma   = TRUE;              /* assume not ultra DMA */
                    break;

                default:
                    ATACommandFail();             /* xfer/mode value invalid */
                    return;
            }
			break;
/* Supported Automatic Acoustic Management */
        case ENABLE_AAM:
            if (SECreg == MAX_Preformance)
            {
                AcousticSpeed(MAX_READ_SPEED);
            }
            else if ((SECreg < MAX_Preformance) && (SECreg >= IntermediateAM_lvl4))
                    {
                        AcousticSpeed(e48XCAV);
                    }
                else if (SECreg == IntermediateAM_lvl3)
                    {
                        AcousticSpeed(e48XCAV);
                    }
                    else if (SECreg == IntermediateAM_lvl2)
                        {
                            AcousticSpeed(e40XCAV);
                        }
                        else if (SECreg == IntermediateAM_lvl1)
                            {
                                AcousticSpeed(e32XCAV);
                            }
                            else if (SECreg == MIN_AE_lvl)
                                {
                                    AcousticSpeed(e24XCAV);
                                }
                                else
                                {           /* Another value */
                                    Iram_Buf_dram_addr = IDENTIFY_ADR + 173;/* word86 bit9 set */
                                    SCSDRAM[Iram_Buf_dram_addr] |= 0x02;
                                    break;
                                }
            /* changed Identfy packet deta */
            Iram_Buf_dram_addr = IDENTIFY_ADR + 173;            /* word86 bit9 set */
            SCSDRAM[Iram_Buf_dram_addr] |= 0x02;
            Iram_Buf_dram_addr = IDENTIFY_ADR + 188;            /* word94 bit7-0 Current Automatic Acoustic Management value */
            SCSDRAM[Iram_Buf_dram_addr] = SECreg;
            break;

        case DISABLE_AAM:
            AcousticSpeed(MAX_READ_SPEED);
            /* changed Identfy packet deta */
            Iram_Buf_dram_addr = IDENTIFY_ADR + 173;            /* word86 bit9 clear */
            SCSDRAM[Iram_Buf_dram_addr] &= 0xFD;
            Iram_Buf_dram_addr = IDENTIFY_ADR + 188;            /* word94 bit7-0 Current Automatic Acoustic Management value clear */
            SCSDRAM[Iram_Buf_dram_addr] = 0xFE;
            break;
/* Supported Automatic Acoustic Management */

		case ENABLE_SATA_FEATURE:
			switch(SECreg)										/* read Sector Count reg.*/
			{
				case INIT_PWR_STATE_TRANS:						/* For SATA interface power management */
					SATA_FE |= bINTFPWR;
					send_msg80(SEND_ATAPI_CMD,0x71007D);
					break;
				case ASYN_NOTIFICATION:							/* Asynchronous Notification */
					SATA_FE |= bASYNNOTI;
					break;
				case SSP:										/* Software Settings Preservation */
					SATA_FE |= bSSP;
					break;
				default:
					ATACommandFail();
					return;
			}
			break;
		case DISABLE_SATA_FEATURE:
			switch(SECreg)										/* read Sector Count reg.*/
			{
				case INIT_PWR_STATE_TRANS:						/* For SATA interface power management */
					SATA_FE &= (BYTE)(~bINTFPWR);
					send_msg80(SEND_ATAPI_CMD,0x71007E);
					break;
				case ASYN_NOTIFICATION:							/* Asynchronous Notification */
					SATA_FE &= (BYTE) (~bASYNNOTI);
					break;
				case SSP:										/* Software Settings Preservation */
					SATA_FE &= (BYTE) (~bSSP);
					break;
				default:
					ATACommandFail();
					return;
			}
			break;

		case ENABLE_MEDIA_STATUS_NOTIFICATION:
			WRITE_FIELD(HAL_HST_TBL, 0x00);							/* set VER to 0x00 */
			if (SCSDRAM[IDENTIFY_ADR + (85*2)] & bit2)
				WRITE_FIELD(HAL_HST_TBH, 0x07);					/* set bit PEJ, LOCK and PENA */
			else
				WRITE_FIELD(HAL_HST_TBH, 0x06);
			SCSDRAM[IDENTIFY_ADR + (85*2)] |= bit2;
			break;

		case DISABLE_MEDIA_STATUS_NOTIFICATION:
			SCSDRAM[IDENTIFY_ADR + (85*2)] &= (BYTE)(~bit2);
			break;

        case DISABLE_REVERT:
        case ENABLE_REVERT:
            //send_msg5S(1,"Feature66CC");
            break;
        default:
            ATACommandFail();									/* xfer/mode value invalid */
            return;
    }
    ATACommandComplete();
}


/* Supported Automatic Acoustic Management */
extern USHORT GetTransferSpeed(BYTE spd);

/*********************************************************************
* Function: AcousticSpeed
*   Input:
*       Disc Speed parameter DiscSpeedT
*   Output:
*       none
* Description:
*       This function set the Dsic Speed parameters in the Mode page
*       for Automatic Acoustic Management support
**********************************************************************/
void AcousticSpeed(DiscSpeedT speed)
{
USHORT  spdt;

    if (GetSpeedValue(Disc_SupportSpeed.Read) > GetSpeedValue(speed)){
        Management_Max_rate = Disc_SupportSpeed.Read = Disc_Speed.Read = speed;
        spdt = GetTransferSpeed(speed);
        Iram_Buf_dram_addr = MSENSE_PAGE_2A+8;
        WriteSDRAM((BYTE)(spdt >> 8));
        WriteSDRAM((BYTE) spdt      );
        spdt = GetTransferSpeed(speed);
        Iram_Buf_dram_addr = MSENSE_PAGE_2A+14;
        WriteSDRAM((BYTE)(spdt >> 8));
        WriteSDRAM((BYTE) spdt      );
    }
}

/* Supported Automatic Acoustic Management */

/*********************************************************************
* SpinDownDisc
*
* spin down motor,report error if any player module
* error occurs. Otherwise, return to caller to continue.
*********************************************************************/
void SpinDownDisc(void)
{
#if (AUDIO_PLAYBACK == 1)
    if(St_nx_audio == TRUE)
    {
        AudioTransferDone();
    }
#endif
    if(SetReadSpeed(eSpStop) == TRUE)
    {
        Iram_Svr_player_mode = STOP_MODE;
    }
    else
    {
        BUILD_SENSE(0x04,0x44,0x00,0x1c);    /* INTERNAL TARGET FAILURE */
    }
}


/*********************************************************************
* ataCheckPowerModeCommand
*
* check current power mode
*
* Returns:  void
* Params?   none
**********************************************************************/
void ataCheckPowerModeCommand(void)
{
#if (CHIP_REV >= 0xC0)
    if (ataPowerMode != STANDBY_MODE)
    {
        WRITE_REG(HAL_HSTREG_MAP->SCNTDATA, 0xFF); /* set idle mode flag */
    }
    else
    {
        WRITE_REG(HAL_HSTREG_MAP->SCNTDATA, 0x00); /* in/going to/recovering from Standby mode */
    }
    WRITE_FIELD(HAL_HST_SCNTOVEN,0x01);
    AutoCommandComplete();
#else
    if (ataPowerMode != STANDBY_MODE)
    {
        WRITE_REG(HAL_HSTREG_MAP->ATINT, 0xFF); /* set idle mode flag */
    }
    else
    {
        WRITE_REG(HAL_HSTREG_MAP->ATINT, 0x00); /* in/going to/recovering from Standby mode */
    }

    /* Don't uses the auto command complete here to prevent the sector count overwrite by default value */
    ManualATACommandComplete();
#endif

}


/*********************************************************************
* ataIdleImmedCommand
*
* spin up motor and enter idle mode
**********************************************************************/
void ataIdleImmedCommand(void)
{
    WRITE_REG(HAL_HSTREG_MAP->ATFEA,  0x00);           /* No Error */
    WRITE_REG(HAL_HST_DRDYW_REGISTER,  0x40);          /* set DRDY */
    WRITE_REG(HAL_HSTREG_MAP->ATRG,   HAL_HST_SCTRG);  /* STATUS_COMPLETION   */

    Knl_atcmd_avail = FALSE;
    St_cmd_inproc = FALSE;

    if (St_drv_ready == TRUE)
    {
        ServoStartUp();
        Iram_Svr_player_mode = PAUSING_MODE;        /* start up success */
    }

    WRITE_REG(HAL_HST_DRDYW_REGISTER,  0x50);  /* status reg set DRDY, DSC bit*/

#if (EN_POWER_CONTROL == 1)
    PowerControl(eIdlePower);
#else
    ataPowerMode = IDLE_MODE;
#endif
}


/*********************************************************************
* ataStandbyImmedCommand
*
* spin down motor and enter standby mode
**********************************************************************/
void ataStandbyImmedCommand(void)
{
    Knl_atcmd_avail = FALSE;

    WRITE_REG(HAL_HSTREG_MAP->ATFEA,  0x00);           /* Clear ATA Error register */
    WRITE_REG(HAL_HST_DRDYW_REGISTER,  0x50);          /* Set DRDY, & DSC in ATA Status register */
    WRITE_REG(HAL_HSTREG_MAP->ATRG,   HAL_HST_SCTRG);  /* STATUS_COMPLETION   */
    St_cmd_inproc = FALSE; //This flag needs to be cleared right away after completion.

    svoIramVar.emergencyStopFlag = TRUE;

    while(HIF.PlayerStatus == BUSY){
        ExitProcess();
    }

    svoIramVar.emergencyStopFlag = FALSE;

    if (St_drv_ready == TRUE)
    {
        svoIramVar.knlTimeupCount = TIMEUP_NON;
        SpinDownDisc();
    }
    else
    {
        while (St_become_ready == TRUE)
        {
            ExitProcess();
        }
    }

    WRITE_REG(HAL_HST_DRDYW_REGISTER,  0x50);  /*set DRV & DSC */

#if (EN_POWER_CONTROL == 1)
    PowerControl(eStandbyPower);
#else
    ataPowerMode = STANDBY_MODE;
#endif
}


/*********************************************************************
* Name: ataSleepCommand
* Description: spin down motor and enter sleep mode
* Input: None
* Output: None
*
*********************************************************************/
void ataSleepCommand(void)
{
    ULONG ready_time;

#if (EN_POWER_CONTROL == 1)
    if (CurrentPowerMode == eSleepPower)
    {
        Knl_atcmd_avail = FALSE;
        St_cmd_inproc   = FALSE;
    #if (DIPM_BY_FW == 0)
        WRITE_FIELD(HAL_HST_CMDIP, 0);
    #endif
        ATACommandComplete();
        return;
    }
#endif

    Ata_sleep_wait = TRUE;

    svoIramVar.emergencyStopFlag = TRUE;
    StartTimer(&ready_time);
    if (St_become_ready == TRUE)
    {
        while (Iram_Svr_player_mode != STOP_MODE)
        {
            if ((ReadTimer(&ready_time)> 5000L) || (EjectKeyInfo.PressedValid == FALSE)) break;

            ExitProcess();
        }
        }
    else
    {
        while (1)
        {
            if (HIF.PlayerStatus != BUSY) break;

            if (ReadTimer(&ready_time)> 5000L) break;

        ExitProcess();
    }
    }
    svoIramVar.emergencyStopFlag = FALSE;

    if (Current_read_sp)
        SpinDownDisc();

    Knl_atcmd_avail = FALSE;
    St_cmd_inproc   = FALSE;
#if (DIPM_BY_FW == 0)
    WRITE_FIELD(HAL_HST_CMDIP, 0);
#endif

    ataPowerMode = SLEEP_MODE;
    ATACommandComplete();

#if (EN_POWER_CONTROL == 1)
    PowerControl(eStandbyPower);    // it should do the eStandbyPower before eSleepPower to stop some of clock.
    //PowerControl(eSleepPower);      // speed down the system pll clock
#endif
}

/*********************************************************************
* Name: ataStandbyCommand
* Description: spin down motor and enter standby mode
* Input: None
* Output: None
*
**********************************************************************/
void ataStandbyCommand()
{
    if (St_drv_ready == TRUE)
    {
        SpinDownDisc();
    }

#if (EN_POWER_CONTROL == 1)
        PowerControl(eStandbyPower);
#else
        ataPowerMode = STANDBY_MODE;
#endif
    ATACommandComplete();
}

/*********************************************************************
* Name: ataGetMediaStatus
* Description: Host use this command to check media status, required by
*              Removable Media Status Notification feature.
* Input: None
* Output: None
*
**********************************************************************/
void ataGetMediaStatus()
{
	BYTE ErrorReg = 0;

	if (SCSDRAM[IDENTIFY_ADR + (85*2)] & bit2)	/* Removable Media Status Notification feature */
	{
		if (St_no_disc == TRUE)
			ErrorReg |= bit1;					/* NM:No Media */

		if (ataMediaIsChanged == TRUE)
		{
			ataMediaIsChanged = FALSE;			/* Only report this to host once for each media change */
			ErrorReg |= bit5;					/* MC:Media Change */
		}

		if (ataKeyIsPressed == TRUE)
		{
			ataKeyIsPressed = FALSE;			/* Only report this to host once for each key press */
			ErrorReg |= bit3;					/* MCR:Media Change Request */
		}

        WRITE_REG(HAL_HSTREG_MAP->ATFEA,  ErrorReg);           /* No Error */
	    Knl_atcmd_avail = FALSE;
	    St_cmd_inproc = FALSE;
        WRITE_REG(HAL_HSTREG_MAP->ATBLO,  0xEB14);         /* ATAPI Signature */
        WRITE_REG(HAL_HST_DRDYW_REGISTER,  0x50);           /* set DRDY, DSC */

	    ATAClearBusy();
	    ATAGenerateHIRQ();                      /* set intrq to interrupt host */

        WRITE_REG(HAL_HSTREG_MAP->ATRG,   HAL_HST_SCTRG);  /* STATUS_COMPLETION   */
		return;
	}

	ATACommandComplete();
}

/*********************************************************************
* Name: ataFlushCacheCommand
* Description:
* Input: None
* Output: None
*
**********************************************************************/
void ataFlushCacheCommand()
{
    ATACommandComplete();
}
/*********************************************************************
* Name: ATACommandComplete()
* Description: send status to host for a successful ata commands
* Input: None
* Output: None
*
**********************************************************************/
void    ATACommandComplete(void)
{
    ULONG CmdTime;
    
    Knl_atcmd_avail = FALSE;
    St_cmd_inproc = FALSE;
    WRITE_REG(HAL_HSTREG_MAP->TSKCMP1, HAL_HST_CDRDY);
    WRITE_REG(HAL_HSTREG_MAP->ATFEA,  0x00);           /* No Error */
    WRITE_REG(HAL_HSTREG_MAP->ATBLO,  0xEB14);         /* ATAPI Signature */
    WRITE_REG(HAL_HST_DRDYW_REGISTER,  0x50);           /* set DRDY, DSC */

    ATAClearBusy();
    ATAGenerateHIRQ();                      /* set intrq to interrupt host */

    WRITE_REG(HAL_HSTREG_MAP->ATRG,   HAL_HST_SCTRG);  /* STATUS_COMPLETION   */
    CmdTime = ReadTimer(&Iram_atapi_timer);
    SendMsgCnB(SEND_ATAPI_CMD,4,0x710613,B4B(CmdTime));
    StartTimer(&Iram_atapi_timer);    
}

/*********************************************************************
* Name:ATACommandFail()
* Description: send status to host for a failed ata commands
* Input: None
* Output: None
*
**********************************************************************/
void    ATACommandFail(void)
{
    ULONG CmdTime;
    
    Knl_atcmd_avail = FALSE;
    St_cmd_inproc = FALSE;

    WRITE_REG(HAL_HSTREG_MAP->ATFEA,  0x04);           /* set ABRT bit */
    WRITE_REG(HAL_HSTREG_MAP->ATBLO,  0xEB14);         /* ATAPI Signature */
    WRITE_REG(HAL_HST_DRDYW_REGISTER,  0x51);           /* set DRDY, DSC, & CHECK */
    WRITE_REG(HAL_HSTREG_MAP->ATRG,   HAL_HST_SCTRG);
    
    CmdTime = ReadTimer(&Iram_atapi_timer);
    SendMsgCnB(SEND_ATAPI_CMD,4,0x710614,B4B(CmdTime));
    StartTimer(&Iram_atapi_timer);    
}

#if (CHIP_REV < 0xC0)
/*********************************************************************
* Name:ManualATACommandComplete()
* Description: send status to host for a successful ata commands
* Input: None
* Output: None
* The different pf Manual mode & Auto mode of ATA command are:
* 1:No send the ATAPI Signature
* 2:No over write the sector number
**********************************************************************/
void    ManualATACommandComplete(void)
{
    /* Start of Manual Sequence of Command Complete */
    Knl_atcmd_avail = FALSE;
    St_cmd_inproc = FALSE;
    WRITE_REG(HAL_HSTREG_MAP->TSKCMP1, HAL_HST_CDRDY);
    WRITE_REG(HAL_HSTREG_MAP->ATFEA,  0x00);           /* No Error */
    //WRITE_REG(HAL_HSTREG_MAP->ATBLO,  0xEB14);         /* ATAPI Signature */
    /* For manual mode, no need to send 0xEB14 */
    WRITE_REG(HAL_HSTREG_MAP->ATBLO,  0x00); 
    WRITE_REG(HAL_HST_DRDYW_REGISTER,  0x50);           /* set DRDY, DSC */

    ATAClearBusy();
    ATAGenerateHIRQ();                      /* set intrq to interrupt host */
        
    if (READ_FIELD(HAL_HST_CCHECK))
    	WRITE_FIELD(HAL_HST_CCHECK,0x01);

    if (READ_FIELD(HAL_HST_CUCRCR)&&READ_FIELD(HAL_HST_UCRCERR))
    {
    	WRITE_FIELD(HAL_HST_CCHECK,0x01);
    	WRITE_FIELD(HAL_HST_SENSEW,HAL_HST_USENSEW);
    }

    if (READ_FIELD(HAL_HST_CCORR))
    	WRITE_FIELD(HAL_HST_CORRW,0x01);

    if (READ_FIELD(HAL_HST_CDRDY))
    	WRITE_FIELD(HAL_HST_DRDYR,0x01);
    /* Manual Mode, skip the over write the sector count */
    //WRITE_REG(HAL_HSTREG_MAP->ATINT,0x03);
    
    WRITE_FIELD(HAL_HST_BSY,0x00);
    WRITE_FIELD(HAL_HST_HINTRQR,0x01);

    if (READ_FIELD(HAL_HST_AUTOAC))
    	WRITE_FIELD(HAL_HST_AUTOA0,0x01);

    WRITE_FIELD(HAL_HST_AUTOSC,0x00);
    WRITE_FIELD(HAL_HST_SCTRG,0x00);

    Iram_shUMisc = Iram_shUMisc|0x0800;    /* Bit 1000_0B10h.3 (R) - HINTRQR - Host Interrupt Request */
    WRITE_REG(HAL_HSTREG_MAP->UMISCR,Iram_shUMisc); 
    
    /* Send Manual FIS34 */
    WRITE_REG(HAL_HSTREG_MAP->RQTXFSTYP, 0x34);      //DToHRegFIS
    WRITE_REG(HAL_HSTREG_MAP->TXREQ,HAL_HST_TXFISREQ); /* Trigger FIS */
    /* End of  Manual Sequence of Command Complete */
}
#endif
