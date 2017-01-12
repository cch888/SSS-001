/****************************************************************************
*                (c) Copyright 2003 -2007 SUNEXT DESIGN, INC.
*                           All Right Reserved
*
* FILENAME:   burnin.c
*
* DESCRIPTION:
*   This file contains Drive Burn In functions
*
*   $Revision: 23 $
*   $Date: 08/11/03 4:01p $
*
************************ SUNEXT CONFIDENTIAL *******************************/

//#include ".\BTC\BTC.h"
#include ".\common\globals.h"
#include ".\common\globtype.h"
#include ".\common\chip.h"
#include ".\common\error.h"
#include ".\common\system.h"
#include ".\common\constant.h"
#include ".\common\commath.h"
#include ".\common\sdram.h"
#include ".\hif\gp_equ.h"
#include ".\hif\at_equ.h"
#include ".\hif\gp_mac.h"
#include ".\hif\gp_ram.h"
#include ".\hif\gp_util.h"
#include ".\hif\gp_rdcd.h"
#include ".\hif\at_knl.h"
#include ".\hif\at_util.h"
#include ".\hif\sv_util.h"
#include ".\hif\rd_util.h"
#include ".\hif\bf_util.h"
#include ".\hif\s2b_main.h"
#include ".\hif\gp_read.h"
#include ".\hif\gp_audio.h"
#include ".\common\iram.h"
#include ".\dvd\dvGvars.h"
#include ".\dvd\dvhostrd.h"
#include ".\dvd\dvBufMgr.h"
#include ".\dvd\dvmacros.h"
#include ".\oem\oem_tbl.h"
#include ".\oem\oem_spd_tbl.h"
#include ".\al\reg_hal.h"
#include ".\al\reg_intctrl.h"
#include ".\common\intctl.h"

#include ".\al\reg_hal.h"

#include ".\oem\coef_values.h"
#include ".\al\REG_HAL.h"
#include ".\al\REG_cpycmp.h"
#include ".\al\REG_audio.h"
#include ".\al\LAL_cd_dvd_codec.h"
#include ".\al\REG_dfe.h"
#include ".\al\REG_dvd_decoder.h"
#include ".\AL\REG_dvd_encoder.h"
#include ".\al\REG_global.h"
#include ".\al\REG_intctrl.h"
#include ".\al\REG_ssi.h"
#include ".\al\REG_write_strategy.h"
#include ".\AL\LAL.h"
#include ".\AL\HAL_dsp.h"
#include ".\AL\HAL_dfe.h"
#include ".\AL\HAL_lasc.h"
#include ".\AL\LAL_stream.h"
#include ".\AL\LAL_shared_sram.h"
#include ".\bd\bdDef.h"
#include ".\bd\bdVars.h"
#include ".\bd\bdMacro.h"
#include ".\bd\bdDiag.h"
#include ".\bd\bdBufMgr.h"
//#include ".\bd\bdUtil.h"
#include ".\al\REG_bd_codec.h"
#include ".\al\HAL_bd_codec.h"
#include ".\crypt\aacs.h"

#if  1//(BURN_IN == 1)
/***** Externals *****/
extern BOOL Burn_in_Enable;

/***** Variables *****/
ULONG Burn_in_timer;  //sky
BYTE  Burn_in_mode = 0; //sky
BOOL Burn_in_LED = OFF;
vBYTE Burn_in_Msg_OFF_timer;
vBYTE Burn_in_error_ID;
vBYTE Burn_in_LED_Blink_count = 0;


///sky
void cd_transfer(void);
void bd_transfer(void);
DiscAccessStatusE bd_transfer2();
DiscAccessStatusE bd_miss();
DiscAccessStatusE DVD_PARTIAL_CACHE_HIT();
DiscAccessStatusE DVD_CacheMiss();
/********** Defines *************/
#define BurnIn_FEBG_error 20
#define BurnIn_Beta_error 30

#define FEBG_CD_SPEED           e4XCLV  
#define FEBG_CD_TG_LBA          93000L  //20:40.02: 20*4500 + 40*75 + 0
//#define CD_FEBG_CHECK_RANGE     (0x04)  /* temp */
#define FEBG_DVD_SPEED          e2XDCLV
#define FEBG_DVD_TG_PSN         0x50000L
//#define DVD_FEBG_CHECK_RANGE    (0x04)  /* temp */
#define RRFG_DVD_SPEED          e2XDCLV
#define DVD_PLUS_RRFG_TG_PSN    0x70500L
#define DVD_PLUS_TARGET_BETA    (40)  //4%
#define DVD_MINUS_RRFG_TG_PSN   0x70500L
#define DVD_MINUS_TARGET_BETA   (50)  //5%
#define RRFG_CHECK_RANGE        (0x07)  /* <+12dB */ /*5220*/
#define BETA_CHECK_RANGE        (10) /* +- 1% */

/* Control Offline Servo Calibration SW */
#define EN_CAL_CD_FEBG          0x01
#define EN_CAL_DVD_FEBG         0x02
#define EN_CAL_DVD_PLUS_BETA    0x04
#define EN_CAL_DVD_MINUS_BETA   0x08
////

/************************************************************/
/* Burn_In_Mode_Detection                                   */
/* Description: TBD!                                        */
/* Input:                                                   */
/* Output:                                                  */
/************************************************************/

void Burn_In_Mode_Detection(void)
{
//BYTE bData, bCtrl, bPinMux0, i;

    //bData = ReadSCReg_1B(rwPADATA);
   // bCtrl = ReadSCReg_1B(rwPACTRL);
   // bPinMux0 = ReadSCReg_1B(rwPAMUX0);  // TBD? need to double check the BURNIN signal for SUN673

   // WriteSCReg_1B(rwPADATA, (bData & 0xfd));        /* set bit 1 = 0 */
   // WriteSCReg_1B(rwPACTRL, (bCtrl | 0x02));        /* set bit 1= 1 (input) */
   // WriteSCReg_1B(rwPAMUX0, (bPinMux0 & 0xfd));     /* set bit 1 = 0 (Enable PA1 input/ouput) */

    // wait for signal stable (10k resitor)
    //for (i = 30; i > 0; i--);
   
    #if 0
    if(ipBURNIN == 0)   
        Burn_in_Enable = TRUE;                      /* Enable Burn In test */
    else
        Burn_in_Enable = FALSE;
    #endif
    
    //resotre to original value
    //WriteSCReg_1B(rwPADATA, bData);
   // WriteSCReg_1B(rwPACTRL, bCtrl);
   // WriteSCReg_1B(rwPAMUX0, bPinMux0);
}


/************************************************************/
/* Show_Error2LED                                           */
/* Description: Control of LED for error codes              */
/* Input: blink_times                                       */
/* Output:                                                  */
/************************************************************/
void Show_Error2LED(BYTE blink_times)
{
vBOOL temp_IE;

    temp_IE = GET_INT_ENABLE_MASK();
    DISABLE_INTERRUPT();

    switch(blink_times){

        case BurnIn_LED_OFF:
            Burn_in_LED = OFF;
            St_led_at_ready = 0;
            St_led_flash_ready = 0;
///            opReadyLED = LED_OFF;//LED_ON;
            Burn_in_LED_Blink_count = 0;
            Burn_in_error_ID = 0;
            break;

        case BurnIn_LED_ON:
            Burn_in_LED = OFF;
            St_led_at_ready = 0;
            St_led_flash_ready = 0;
///            opReadyLED = LED_ON;//LED_OFF;
            Burn_in_LED_Blink_count = 0;
            Burn_in_error_ID = 0;
            break;

        case BurnIn_Tray_out_error:
        case BurnIn_Tray_in_error:
        case BurnIn_DRAM_error:
        case BurnIn_No_disc:
        case BurnIn_Start_up_error:
        case BurnIn_Set_speed_error:
        case BurnIn_Seek_error:
        case BurnIn_FEBG_error:
        case BurnIn_Beta_error:
            Burn_in_LED = ON;
            St_led_at_ready = 0;
            St_led_flash_error = 1;

            Iram_LED_Flash_timer = 400/20; //200/20;  // 20ms 
            Iram_Gp_led_at_timer = 0; //200/20;  // 20ms 
            Burn_in_Msg_OFF_timer = 0;  //sky
///            opReadyLED = LED_OFF;

            Burn_in_LED_Blink_count = 0;
            Burn_in_error_ID = blink_times-BurnIn_LED_ON;
            break;
    }
    SET_INT_ENABLE_MASK( temp_IE );
}


/************************************************************/
/* ShowError                                                */
/* Description: Control of LED for error codes              */
/* Input: Error_ID                                          */
/* Output:                                                  */
/* 1:tray_out error                                         */
/* 2:tray_in error                                          */
/* 3:no disc                                                */
/* 4:other servo on error(startup error)                    */
/* 5:setspeed error                                         */
/* 6:seek error                                             */
/************************************************************/
void ShowError(BYTE Error_ID)
{
	ULONG clock,temp_l;
	BOOL ret;

    while(1)
    {
    //ReadyLED(LED_OFF);
		SendMsg5SValue(1,"ErrID",2,Error_ID);
    	switch(Error_ID)
        {
            case BurnIn_FEBG_error:
                temp_l = 1500L;
                break;
            case BurnIn_Beta_error:
                temp_l = 4500L;
                break;
            case BurnIn_DRAM_error:
            temp_l = 3500L;
                break;
            case BurnIn_Start_up_error: // Xfer rate test error
            temp_l = 2500L;
                break;
            case BurnIn_Seek_error:
            temp_l = 1000L;
                break;
        }

        StartTimer(&clock);
        while(ReadTimer(&clock) < 500L)
            ExitProcess();

        //Show_Error2LED(Error_ID);

        StartTimer(&clock);
        while(ReadTimer(&clock) < temp_l);

        // clear LED
        //Show_Error2LED(LED_OFF);

        TrayManagerInfo.Command = TRAY_COMMAND_TRAY_OUT;
        svoIramVar.emergencyStopFlag = FALSE;
        ret = ServoTrayOut();

        TrayManagerInfo.Command = TRAY_COMMAND_TRAY_OUT;
        svoIramVar.emergencyStopFlag = FALSE;
        ret = ServoTrayIn();
    }
}

/************************************************************/
/* Wait_Insert_disc                                         */
/* Description: TBD!                                        */
/* Input:                                                   */
/* Output:                                                  */
/* 6: FEBG cal Fail                                         */
/* 5: Beta offset cal Fail                                  */
/* 4: Dram test error                                       */
/* 3: Xfer rate test error                                  */
/* 1: Full stroke test error                                */
/************************************************************/
BYTE Wait_Insert_Disc(void)
{
BOOL ret;

    //TrayManagerInfo.Command = TRAY_COMMAND_TRAY_OUT;
    svoIramVar.emergencyStopFlag = FALSE;
    StopDecoder();
    ret = ServoTrayOut();

    //if(ret == FAIL)
   //     Iram_eject_no = EJECT_ERROR;
   // else
        //Iram_eject_no = EJECT_SW_WATCH;
        //EjectKeyInfo.Disable = FALSE;//sky

   // Show_Error2LED(Error_ID);

    //monitor user push eject button or tray in
    //while (Iram_eject_no != EJECT_EXECUTING)
    while (1)
    {
    	if(EjectKeyInfo.Pressed)
        break;
        //ExitProcess();
    }

    //clear LED status
    ReadyLED(LED_OFF);

    svoIramVar.emergencyStopFlag = FALSE;
    StopDecoder();

    ret = ServoTrayIn();

    //Iram_eject_no = EJECT_SW_WATCH;
    EjectKeyInfo.Disable = FALSE;

    if(ret == FAIL)
    {
        return FAIL;
    }

    TrayinInitial();

    if(St_no_disc == TRUE)
    {
        return FAIL;
    }

    if(St_drv_ready != TRUE)
    {
        return FAIL;
    }
    else
        return PASS;
}

    #if(TRAVERSE_TEST==1)
#define BURN_IN_FORCE 30
    #endif

SearchCacheResultE AgSearchCache(TransferS Request, BYTE mode)
{

    ULONG   EndLBA;
    ULONG   dvdStartBlock;
    BYTE    InterruptStatus;

    InterruptStatus =   GET_INT_ENABLE_MASK();
    DISABLE_INTERRUPT();

    dvdStartBlock = dvdGetBlockAddress(DataBuffer.CacheStartLBA);

    /*-------------------------------------------------------------------
       Cache is static if read ahead = false (no buffering is occuring)
       If cache is static determine if the starting LBA is currently in
       the buffer. If not return a cache miss status. For fast cache
       transfers check regardless if cache is static or not.
    --------------------------------------------------------------------*/
     /* To reduce the File System ReadBack Time */
     if (pdbGetDVDType() != DVD_RAM)
     {
            if (dvdStartBlock> ONE_DVD_BLOCK)
            {
                if (((PreATAPICMD == SBC_READ_10)||(PreATAPICMD == SBC_READ_12))&&(PreReadCMDLba<=Request.StartBlock.lba)&&(Request.StartBlock.lba<=dvdStartBlock))
                {
                       DataBuffer.SectorCount  = DataBuffer.SectorCount + (DataBuffer.CacheStartLBA&0x0F) +ONE_DVD_BLOCK;
                       DataBuffer.CacheStartLBA = (DataBuffer.CacheStartLBA&0xFFFFFFF0)- ONE_DVD_BLOCK;

                    if(DataBuffer.CurHostPtr < ONE_DVD_BLOCK)
                    {
                        DataBuffer.CurHostPtr = ((DataBuffer.CurHostPtr&0xFFFFFFF0)+MAX_DVD_SECTORS- ONE_DVD_BLOCK);
                    }
                    else
                    {
                        DataBuffer.CurHostPtr = (DataBuffer.CurHostPtr&0xFFFFFFF0) - ONE_DVD_BLOCK;
                    }
                    dvdStartBlock = dvdGetBlockAddress(DataBuffer.CacheStartLBA);
                }
            }			
     }
    SET_INT_ENABLE_MASK( InterruptStatus);

    if ((DVD_BUFFERING() == OFF) || (mode == IMMEDIATE_SEARCH_MODE) )
    {
        /* No data in the buffer */
        if (DataBuffer.SectorCount == 0)
            return (CACHE_MISS);

        /* Staring LBA is not among the cached blocks */
        if ( (Request.StartBlock.lba < dvdStartBlock) ||  (Request.StartBlock.lba > DataBuffer.CacheEndLBA) )
            return ( CACHE_MISS );

    }
    else
    {
        /*-------------------------------------------------------------------
           If cache is not static determine if the starting LBA is currently
           in the buffer or if it is within the specified range of blocks that
           will be cached soon. If not return a cache miss status
        --------------------------------------------------------------------*/		
		
        /* Starting LBA is not in the buffer nor among the range of blocks to be cached */
        if ( (Request.StartBlock.lba < dvdStartBlock) ||
             (Request.StartBlock.lba > (DataBuffer.CacheEndLBA + DVD_CACHE_WAIT_RANGE)) )
            return ( CACHE_MISS );


        if ( (Request.StartBlock.lba > DataBuffer.CacheEndLBA) || (DataBuffer.SectorCount == 0) )
        {
            /*-------------------------------------------------------------------
                Wait for the starting LBA to come in to the buffer
                (Request.StartLBA.lba will be less than CacheEndLBA once the
                block is loaded) If buffering stops before this return a cache miss.
            --------------------------------------------------------------------*/
            while ( (Request.StartBlock.lba > DataBuffer.CacheEndLBA) || (DataBuffer.SectorCount == 0))
            {
                if( (DVD_BUFFERING() == OFF ) || (dvdBlockInterruptTimer.fTIMEOUT == TRUE))
                    /* Caching stopped so check one last time to determine if start lba is in the buffer */
                    if( (Request.StartBlock.lba > DataBuffer.CacheEndLBA) || (DataBuffer.SectorCount == 0))
                        return ( CACHE_MISS );
                ExitProcess();
            }
        }
     }

    /*--------------------------------------------------
        Calculate the ending LBA
    --------------------------------------------------*/
    EndLBA= Request.StartBlock.lba + Request.BlockCount.Long - 1L;
    /*--------------------------------------------------
        Determine if this is a full or partial cache hit.
    --------------------------------------------------*/
    if( EndLBA <= DataBuffer.CacheEndLBA )
        return ( FULL_CACHE_HIT );
    else
        return ( PARTIAL_CACHE_HIT );

}

DiscAccessStatusE DVD_CacheMiss()
{
    DiscAccessStatusE DiscReadStatus;   /* Status of disc read */
;
    send_msg5S(1,"*CacheMiss*");

    /*---------------------------------------------------------------
        Insure read ahead buffering is enabled
    ---------------------------------------------------------------*/
    dvdDecFlag.fDISABLE_READ_AHEAD = FALSE;

    /*---------------------------------------------------------------
        Clear dvdCss.CssFlag.AgidInUse
    ---------------------------------------------------------------*/
    dvdCss.CssFlag.AgidInUse = FALSE;

    /*---------------------------------------------------------------
        Initialize the host buffer
    ---------------------------------------------------------------*/

    dvdInitializeHostBuffer(HostXfer.StartBlock.lba);

    /*---------------------------------------------------------------
        Set host transfer state to waiting
    ----------------------------------------------------------------*/
    HostXfer.State = WAITING_FOR_TRANSFER;

    /*---------------------------------------------------------------
        Start loading the buffer
    ----------------------------------------------------------------*/
    send_msg54(1,'I','N','I','T');

    DiscReadStatus = dvdLoadBuffer( HostXfer, INIT_READ);

    return (DiscReadStatus);
}
DiscAccessStatusE DVD_PARTIAL_CACHE_HIT()
{

    TransferS           NewReq;             /* Truncated host request */
    DiscAccessStatusE   DiscReadStatus;     /* State of buffer after request to load buffer  */
    ULONG               HostEndLBA;         /* Ending LBA of host transfer request */

    dvdDecFlag.fDISABLE_READ_AHEAD = FALSE;
    /*--------------------------------------------------------------
        If the StartLBA is not the last transferred lba + 1,
        call dvdAdjustHostBufferPtr to adjust the host buffer ptr to point
        to the location where the transfer is to begin.
    ---------------------------------------------------------------*/
    send_msg5S(1,"*PartialCacheHit*");

    if (HostXfer.StartBlock.lba != DataBuffer.CacheStartLBA)
    {   
    	dvdAdjustHostBufferPtr( HostXfer.StartBlock.lba );
    }

    /*---------------------------------------------------------------
        Start the host transfer.
    ---------------------------------------------------------------*/
    SendHostBlocks();

    /*---------------------------------------------------------------
        Determine last LBA of host transfer
    ---------------------------------------------------------------*/
    HostEndLBA = (HostXfer.StartBlock.lba + HostXfer.BlockCount.Long) - 1L;


    if (DVD_BUFFERING() == ON)
    {   	

        /*---------------------------------------------------------------
           Check if last host sector is in the buffer
        ---------------------------------------------------------------*/
        if (HostEndLBA <= DataBuffer.CacheEndLBA)
        {       	
            /*---------------------------------------------------------------
               If host is waiting for data restart the host transfer
               else just return a good status
            ---------------------------------------------------------------*/
            if (HostXfer.State == WAITING_FOR_TRANSFER)
            {            	
                HostXfer.State = TRANSFERRING;
                SendHostBlocks();
            }
            return (BUF_GOOD);
        }

        /*---------------------------------------------------------------
            Buffering is still on and may load more host blocks.
            Pass the host request and extend buffering
        ---------------------------------------------------------------*/
        send_msg5S(1,"*EXTEND_READ*");
        DiscReadStatus = dvdLoadBuffer( HostXfer, EXTEND_READ);
        /*---------------------------------------------------------------
            If buffering good, all host blocks have been loaded so exit
        ---------------------------------------------------------------*/
        if (DiscReadStatus == BUF_GOOD)
        {send_msg5S(1,"Jam 17");    return (DiscReadStatus);}

        /*---------------------------------------------------------------
            If the extend failed wait for buffering to stop. If there
            is an error on a host block, buffering should be stopped
        ---------------------------------------------------------------*/
		while ( (DVD_BUFFERING() == ON) && (dvdBlockInterruptTimer.fTIMEOUT == FALSE) )
        {         				
			ExitProcess();
		}

        if(dvdBlockInterruptTimer.fTIMEOUT == TRUE)
        {send_msg5S(1,"Jam 19");    StopDecoder();}
    }

    if (DVD_BUFFERING() == OFF)
    {    
        /*---------------------------------------------------------------
            If all host blocks have come into the buffer determine
            if a transfer needs to be re-initiated. Then exit
        ---------------------------------------------------------------*/
        if (HostEndLBA <= DataBuffer.CacheEndLBA)
        {        	
            if (HostXfer.State == WAITING_FOR_TRANSFER)
            {           	
                HostXfer.State = TRANSFERRING;
                SendHostBlocks();
            }

            return (BUF_GOOD);
        }

        /*---------------------------------------------------------------
            Need to restart the buffering to load remaining host blocks
            Calculate the new start address and count to buffer
        ---------------------------------------------------------------*/
        NewReq.StartBlock.lba = DataBuffer.CacheEndLBA + 1L;
        NewReq.BlockCount.Long = (HostEndLBA + 1L) - NewReq.StartBlock.lba;

        /*---------------------------------------------------------------
            Start loading the buffer
        ----------------------------------------------------------------*/
        send_msg5S(1,"*INIT_READ*");
        DiscReadStatus = dvdLoadBuffer( NewReq, INIT_READ);
    }

    return (DiscReadStatus);

}
	
extern BYTE CmdSeekOk;
void DVDxferTest()
{
    ULONG dvdXferTimer;
    SearchCacheResultE  SearchResult;   /* Holds the result of a cache search           */
    DiscAccessStatusE   DiscReadStatus; /* Status of buffer after request completed     */
    dvdXferTimer=0;

	//ENABLE_DVD_BUFFERING();
        /*---------------------------------------------------------------
        For zero transfer length exit with command complete.
    ----------------------------------------------------------------*/
    if (HostXfer.BlockCount.Long == 0L )
    {
        dvdReadExit();
        return;
    }

    /*-------------------------------------------------------------------
        Check if read is within a valid range
    --------------------------------------------------------------------*/
#if 0//(CEProduct == 1) && (READER_ONLY == 0)
    /*Users suddenly turn off power when to writing no recording DVD+RW disc first.
      Because driver has not update control data zero(pdbSetDVDTrackEndPSN()).
      To cause Bankend recover title fail.*/
    if(pdbGetDiscKind() != eDVDPlusRWDisc && pdbGetDiscKind() != eDVDPlusRWDLDisc)
#endif
    {
        if (dvdValidReadRequest() == FALSE)
        {
            BUILD_SENSE(0x05,0x21,0x00,0x07);
            dvdReadExit();
            return;
        }
    }
    /*---------------------------------------------------------------
        Set to maximum speed
    ----------------------------------------------------------------*/
    if(SpeedMD==Set_sp_f)
    {
        SpeedMD=Special_f;
    }

    /*if(SpeedMD == Write_f)
    {
        Iram_not_change_speed_timer = 0;
        SpeedMD = MiddleMode;
    }*/

    CmdSeekOk = 0;      /*initial flag */
	
    /*-----------------------------------------------------------------------
        Search the cache
        NOTE: If Read Ahead is disabled always set result to cache miss
    -----------------------------------------------------------------------*/
    
    SearchResult = AgSearchCache(HostXfer,WAIT_SEARCH_MODE);
    
#if (EN_POWER_CONTROL)
    if(SearchResult != FULL_CACHE_HIT)
        SetDiscReadPowerMode();
#endif

    /*---------------------------------------------------------------
        Process the search result. Each process returns the status
        of the buffer access (DiscReadStatus).

        NOTE: For FullCacheHit() DiscReadStatus is always BUF_GOOD.
    ---------------------------------------------------------------*/
    switch (SearchResult)
    {
    	case FULL_CACHE_HIT:
			SendMsg5S(1, "FULL_CACHE_HIT");  
		    if (HostXfer.StartBlock.lba != DataBuffer.CacheStartLBA)
		    {
		       dvdAdjustHostBufferPtr( HostXfer.StartBlock.lba );
		    }    
    		SendHostBlocks();
			DiscReadStatus = BUF_GOOD;
            break;
     
        case CACHE_MISS:
			SendMsg5S(1, "CACHE_MISS");
            DiscReadStatus = DVD_CacheMiss();
            break;
			
		//case FULL_CACHE_HIT:
        case PARTIAL_CACHE_HIT:
			SendMsg5S(1, "PARTIAL_CACHE_HIT");
            DiscReadStatus = DVD_PARTIAL_CACHE_HIT();
            break;
    }

    /*-----------------------------------------------------------------------
        Process the disc read status
    -----------------------------------------------------------------------*/
    if (DiscReadStatus == BUF_GOOD)
    {
    	send_msg5S(1,"AGING BUF_GOOD");     
       
    }
    else /* Error in buffering host data */
    {
    	send_msg5S(1,"AGING BUF_NOT_GOOD");
        /*---------------------------------------------------------------
            Insure transferring of remaining host blocks completes
        ---------------------------------------------------------------*/
        while ( (DataBuffer.SectorCount > 0 ) && (HostXfer.State == TRANSFERRING))
            ExitProcess();

        /*---------------------------------------------------------------
            Indicate that the host transfer was stopped and not completed
        ---------------------------------------------------------------*/
        HostXfer.State = TRANSFER_STOPPED;
        dvdDecFlag.fDISABLE_READ_AHEAD = TRUE;

        /*---------------------------------------------------------------
            Build the sense code according to type of DiscReadStatus
        ---------------------------------------------------------------*/
        dvdSetSenseCode(DiscReadStatus);
    }
    dvdReadExit();
    return;
}

/************************************************************/
/* BurnIn                                                   */
/* Description: Burn In test                                */
/* Input:                                                   */
/* Output:                                                  */
/************************************************************/
extern vBYTE rd_method;//sky cd-transfer Rate
extern vBYTE Track_No;//sky cd-transfer Rate
extern vBYTE Read_Lead_in;//sky cd-transfer Rate
ULONG skyaddr, bdaddr; //Cd transfer Rate
extern BYTE CmdSeekOk; //BD Transfer Rate
#define AGING_OFFSET                  0x37EL

USHORT	slot_level_check; //[J5zY] for SLT002 disc in/out test
extern USHORT slot_level_check; //[J5zY] for SLT002 disc in/out test

void BurnIn(void)
{		
	ULONG addr, addr3, b_in_timer, //temp_start_timer, temp_end_timer, 
		  tray_out_t, tray_in_t;
	
	//ULONG   ClusterRequest;
	//PSNs    StartPSN, EndPSN;
	USHORT  k;
	BYTE i, j, v1, result, test_dvd, test_cd, test_bd;// , sw;
	BOOL ret,new_aging;
	BYTE rw_seek_retry;
	
#if (SET_SLOT_IN == 1)
	extern BOOL fg_R2Profile; //[J50Z] for SLT002 disc in/out test
	USHORT	m, n, p, q, x; //[J50Z] for SLT002 disc in/out test
#endif


 	result = 0; 
#if (TG_Factory_Test == 1) 
//=[START]===Special Case For Factory ======
{
	SCStartUp();
	ENABLE_INTERRUPT();
	ServoStartUp();
	ServoStop();
	EjectKeyInfo.PressedValid = FALSE;
	TrayManagerInfo.Pushed = 0;
	svoIramVar.emergencyStopFlag = FALSE;
	StopDecoder();
	//Step.1 Tray out ¨Ï¾¦½üÂk¦ì
	/*while(1)
	{
		if(EjectKeyInfo.Pressed)
		{
			ServoTrayOut();
	    	TrayManagerInfo.TrayState = TRAY_STATE_OUT_OK;
			DelaymS(250);
			break;
		}
	}*/

	//Step.2 Tray in-out Test 10 loop
	while(1)//Waiting Disc In 
	{
	    if((ipOpenSW == 1) && (ipSW2 == 0) && (ipCloseSW == 1))
	    	break;
	} 
	ret = ServoTrayIn();   
	TrayManagerInfo.TrayState = TRAY_STATE_IN_OK;

	while(1)//Waiting Disc In Already.
	{
		if((ipOpenSW == 1)&&(ipSW2 == 1)&&(ipCloseSW == 0))
			break;
	}

	//Tray in-out Test
	for(i=0; i<3; i++)
	{	     
			svoIramVar.emergencyStopFlag = FALSE;
		    StopDecoder();
		    tray_out_t = tray_in_t = 0;    				
		    StartTimer(&tray_out_t);
		    SendMsg5SValue(1, "Open time", 4, tray_out_t);
			SwitchFlag = 2;
		    ret = ServoTrayOut();    
		    while(1)//Waiting Disc Out Already
		    {	    
		         if((ipOpenSW == 1) && (ipSW2 == 0) && (ipCloseSW == 1))  	    
		        	break;
		    }
		    StartTimer(&tray_in_t);
		    SendMsg5SValue(1, "Open time", 4, tray_in_t);    

		    if((tray_in_t - tray_out_t)>3000) //daniel for BP-S26 aging test-disc out test [B10E]
		    	ShowError(13);
			
		    DelaymS(250); DelaymS(250); // Delay one second to discharge all previosu one
		         
		    svoIramVar.emergencyStopFlag = FALSE;
		    StopDecoder();
		    tray_out_t = tray_in_t = 0;     
		    StartTimer(&tray_out_t);
		    SendMsg5SValue(1, "Colse time", 4, tray_out_t);
		    ret = ServoTrayIn();
		    while(1)//Waiting Disc In Already
		    {
		    	if((ipOpenSW == 1)&&(ipSW2 == 1)&&(ipCloseSW == 0))
		          	break;
		    }
		    StartTimer(&tray_in_t);
		    SendMsg5SValue(1, "Close time", 4, tray_in_t);  

		    if((tray_in_t - tray_out_t)>3000) 
				ShowError(14); 
	}
	SwitchFlag = 2;
	ret = ServoTrayOut();    
	while(1)
	{	    
		if((ipOpenSW == 1) && (ipSW2 == 0) && (ipCloseSW == 1)) 
			break;
	}
	while(1)
	{SendMsg5S(1, " 3 ");}

}
//=[END]===Special Case For Factory ======
#endif
/////////// test DRAM
    addr = 0;
    //open_hif_msg(0xffffffff);
    //open_svo_msg(0xffffffff);
    for (i=0; i<0x20; i++)
    {
        addr = (ULONG)i * 0x10000L;
        for(j=0; j<17; j++)
        {
            SCSDRAM[addr] = 0x5a;
            addr+=(1<<j);
        }
    }

    for (i=0; i<0x20; i++)
    {
        addr = (ULONG)i * 0x10000L;
        for(j=0; j<17; j++)
        {
            v1 = SCSDRAM[addr];
            if(v1 != 0x5a)
            {
                result = 1;
                break;
            }
            addr+=(1<<j);
        }

        if(result != 0) break;
    }  

	SCStartUp();
    ENABLE_INTERRUPT();

	if(result != 0)
    {   	
        ShowError(BurnIn_DRAM_error);
        //goto DRAM_test;
    }
         

    ServoStartUp();
	DelaymS(250); DelaymS(250);
    ServoStop();
    EjectKeyInfo.PressedValid = FALSE;
    TrayManagerInfo.Pushed = 0;
    svoIramVar.emergencyStopFlag = FALSE;
    StopDecoder();
    
#if(SET_SLOT_IN == 0)//[J5zY]
  #if(Customer_Dune_HD ==1)//[J010]Bios
    if(GetPersistentTable1B(AGING_OFFSET) == 0xAA)
    {
       slot_level_check = 1;
       if(slot_level_check == 1)
       {
         for (k=0;k<15;k++)
         {
            ret = ServoTrayOut();
            DelaymS(100);
            ret = ServoTrayIn();
         }
       slot_level_check = 0;
       }
    }
  #endif
    ret = ServoTrayOut();
    TrayManagerInfo.TrayState = TRAY_STATE_OUT_OK;
#else
	if(ipSW2)//If the loader have disc in anywhere.
	{
	SwitchFlag = 2;
    ret = ServoTrayOut();
    TrayManagerInfo.TrayState = TRAY_STATE_OUT_OK;
	}
#endif

 if(GetPersistentTable1B(AGING_OFFSET) == 0xAA)
 {
    #if(SET_SLOT_IN == 0)
    	while(EjectKeyInfo.PressedValid == FALSE && TrayManagerInfo.Pushed == 0)
        	ExitProcess();
    #else
    	SwitchFlag = 2;	 //daniel for aging disc out fall[B10F_L]
    	while(1) // daniel sky for BP-S26 aging test-change disc
    	{
     		if((ipOpenSW == 0) && (ipSW2 == 0) && (ipCloseSW == 1))
      			break;     
    	}
    	while(1) // daniel sky for BP-S26 aging test-change disc
     	{
      		if((ipOpenSW == 1) && (ipSW2 == 0) && (ipCloseSW == 1))
      			break;
     	} 
    #endif
   
    //ReadyLED(LED_OFF);

    EjectKeyInfo.PressedValid = FALSE;
    TrayManagerInfo.Pushed = 0;

    svoIramVar.emergencyStopFlag = FALSE;
    StopDecoder();
    ret = ServoTrayIn();
   
    TrayManagerInfo.TrayState = TRAY_STATE_IN_OK;
	
	///////////Move sled
    for (k=0;k<100;k++)
	{           
            svoStp_Traverse(0,1);
    }

#if(SET_SLOT_IN == 1)
	if (fg_R2Profile) //[J5zY] for SLT002 disc in/out test
	{
		slot_level_check = 1; //[J5zY] for SLT002 disc in/out test
    
		for (m=0;m<300;m++){
			for (n=0;n<1;n++){
				   for (p=0;p<1;p++){
				 		for (q=0;q<1;q++)
		{ 			   		
			 send_msg5S(1,"Tray Test");    
   
			 for(x=0;x<1;x++)
			 {
			   	DelaymS(100); //DelaymS(250); DelaymS(250); DelaymS(250);   
			 }
			 tray_out_t = tray_in_t = 0;			   
			 StartTimer(&tray_out_t);
			 SendMsg5SValue(1, "Open time", 4, tray_out_t);
			   
			 ret = ServoTrayOut();
					   
			 StartTimer(&tray_in_t);   
			 SendMsg5SValue(1, "Open time", 4, tray_in_t);	
			   
			 if((tray_in_t - tray_out_t)>6000) //daniel for BP-S26 aging test-disc out test [B10E]
			 {	
			 	slot_level_check = 0; //[J50Z] for SLT002 disc in/out test
			    ShowError(13);
			 }
			   
			 //TrayManagerInfo.TrayState = TRAY_STATE_OUT_OK;
			 
			 for(x=0;x<1;x++)
			 {		   
			   	DelaymS(100); //DelaymS(250); DelaymS(250); DelaymS(250);   
			 }
					   
			 tray_out_t = tray_in_t = 0;   
			 StartTimer(&tray_out_t);
			 SendMsg5SValue(1, "Colse time", 4, tray_out_t);
			   
			 ret = ServoTrayIn();
					   
			 StartTimer(&tray_in_t);
			 SendMsg5SValue(1, "Close time", 4, tray_in_t);    
			   
			 if((tray_in_t - tray_out_t)>6000) // daniel for BP-S26 aging test-disc in test
			 {	
			 	slot_level_check = 0; //[J50Z] for SLT002 disc in/out test
			 	ShowError(14); 
			 }			   
		  }//q
		 }//p
		 }//n
		}//m
   
		   slot_level_check = 0; //[J50Z] for SLT002 disc in/out test
		   
		   //ret = ServoTrayIn(); //[J50Z] for SLT002 reset state
		   ret = ServoTrayOut();
		   TrayManagerInfo.TrayState = TRAY_STATE_OUT_OK;
		   	
		   SwitchFlag = 2;	//daniel for aging disc out fall[B10F_L]
		   while(1) // daniel sky for BP-S26 aging test-change disc
		   {
		   		if((ipOpenSW == 0) && (ipSW2 == 0) && (ipCloseSW == 1))
		   	   		break;	  
		   }
		   while(1) // daniel sky for BP-S26 aging test-change disc
		   {
		   	   if((ipOpenSW == 1) && (ipSW2 == 0) && (ipCloseSW == 1))
		   	   		break;
		   }	
		   ret = ServoTrayIn();
	    	while(1)
	    	{
	    		if(ipOpenSW == 1)
	          		break;
	    	}
		   SendMsg5S(1, "SLT BurnIn End");
	}		 
#endif
//for SLT002 BurnIn


   // ClrBitsSCReg_1B(rwCLKSTP1,  bCPYCKSTP);          /* turn on Copy clock */
    WRITE_FIELD(HAL_GLOBAL_CPYCKSTP,0);

#if(SET_SLOT_IN == 1)
	if (!fg_R2Profile) //[J5zY] for EPO disc in/out test
#endif
	{
   //Tray in-out Test
	svoIramVar.emergencyStopFlag = FALSE;
    StopDecoder();
    tray_out_t = tray_in_t = 0;    				
    StartTimer(&tray_out_t);
    SendMsg5SValue(1, "Open time", 4, tray_out_t);
    ret = ServoTrayOut();    
    while(1)
    {
    	#if(SET_SLOT_IN == 0)
          if(ipOpenSW == 0)
        #else
          if((ipSW2 == 0)&&(ipOpenSW == 1)) // daniel for BP-S26 aging test-disc in after disc out [B10E]
        #endif
          break;
    }
    StartTimer(&tray_in_t);
    SendMsg5SValue(1, "Open time", 4, tray_in_t);    
    #if(SET_SLOT_IN == 0)
			#if (Customer_A1==1)
		    if((tray_in_t - tray_out_t)>3000)
			#else
    		if((tray_in_t - tray_out_t)>2600)
			#endif
    #else
    	if((tray_in_t - tray_out_t)>3000) //daniel for BP-S26 aging test-disc out test [B10E]
    #endif
    		ShowError(13);
      
    DelaymS(250); DelaymS(250); DelaymS(250); DelaymS(250); // Delay one second to discharge all previosu one
         
    svoIramVar.emergencyStopFlag = FALSE;
    StopDecoder();
    tray_out_t = tray_in_t = 0;     
    StartTimer(&tray_out_t);
    SendMsg5SValue(1, "Colse time", 4, tray_out_t);
    ret = ServoTrayIn();
    while(1)
    {
          if(ipOpenSW == 1)
          	break;
    }
    StartTimer(&tray_in_t);
    SendMsg5SValue(1, "Close time", 4, tray_in_t);  
    #if(SET_SLOT_IN == 0)
			#if(Customer_A1==1)
		    if((tray_in_t - tray_out_t)>3000)
			#else
    		if((tray_in_t - tray_out_t)>2300)
			#endif
    #else
    	if((tray_in_t - tray_out_t)>3000) // daniel for BP-S26 aging test-disc in test
    #endif
    		ShowError(14); 
	}
	
   	OpenWritableSegmentTable(0x00); //[B1AJ] 0x80->0x00 ePersistent_Talbe
    SetPersistentTable1B(AGING_OFFSET,0x00);
   	CloseWritableSegmentTable(0x00); //[B1AJ] 0x80->0x00 ePersistent_Talbe
   new_aging = 1;		
////Tray in-out Test
}
if(new_aging != 1)
{
    #if(SET_SLOT_IN == 0)
    while(EjectKeyInfo.PressedValid == FALSE && TrayManagerInfo.Pushed == 0)
        ExitProcess();
    #else
     	SwitchFlag = 2;//[B1AJ] daniel for aging disc out fall[B10F_L] 
    	while(1) //daniel sky for BP-S26 aging test-change disc
    	{
     		if((ipOpenSW == 0) && (ipSW2 == 0) && (ipCloseSW == 1))
      			break;     
    	}
    	while(1) //daniel sky for BP-S26 aging test-change disc
     	{
      		if((ipOpenSW == 1) && (ipSW2 == 0) && (ipCloseSW == 1))
      			break;
     	} 
    #endif
    ReadyLED(LED_OFF);

    EjectKeyInfo.PressedValid = FALSE;
    TrayManagerInfo.Pushed = 0;

    svoIramVar.emergencyStopFlag = FALSE;
    StopDecoder();

    ret = ServoTrayIn();

   
    TrayManagerInfo.TrayState = TRAY_STATE_IN_OK;
}
   if(GetPersistentTable1B(AGING_OFFSET) == 0x02)//BD
   {
     test_bd = 0;
     test_cd = 1;
     test_dvd =1;
     send_msg5S(1,"BD start"); 	
   }
   /*else if(GetPersistentTable1B(AGING_OFFSET) == 0x02)//D9
   { 
    test_bd = 0;
    test_dvd = 1;
    test_cd = 1;    
     Burn_in_timer = 235;
    send_msg5S(1,"D9 start"); 
   }*/
   else if(GetPersistentTable1B(AGING_OFFSET) == 0x01)//D5
   { 
    test_bd = 0;
    test_dvd = 0;
    test_cd = 1;   
    Burn_in_timer = 242;
    send_msg5S(1,"D5 start");  
   }
   else//CD
   {
     test_bd = 0;
     test_dvd = 0;
     test_cd = 0; 
     send_msg5S(1,"CD start"); 
   }
    while(1)
    {

     
        TrayinInitial();       
        if(St_no_disc == TRUE)
        {
            //continue;
            ShowError(11);
        }

        if(St_drv_ready != TRUE)
        {
            ShowError(12);
        }

 #if 1
   
  if ((pdbDiscLoaded() == BD_DISC) && (test_bd == 0) && (test_cd == 1) && (test_dvd == 1))  //BD_Disc  sky add for aging test
  {            
            //DiscAccessStatusE   DiscReadStatus;
            //ClusterRequest = ((pdbGetBDMaxPSN(0) + 1L) - StartPSN.PSN) / ONE_BD_CLUSTER;
			DelaymS(250); DelaymS(250); DelaymS(250); DelaymS(250); // Delay one second to discharge all previosu one
			
#if 0 //JJ close xfer test
			ServoSetSpeed(e2XDCLV);
			SendMsg5S(1, "BDxferST");
            /////////////////////////////////BD Transfer Rate
            bd_transfer();                                    
            /////////////////////////////////BD Transfer Rate End
            SendMsg5S(1, "BDxferEN");
            ClusterRequest = (EndPSN.PSN - StartPSN.PSN) / ONE_BD_CLUSTER; 
            /////////// Xfer rate test
            
            addr = 0x230000L;
    		StartTimer(&tray_in_t);
    		SendMsg5SValue(1, "B_seek_t1", 4, tray_in_t);  
			rw_seek_retry = 0;
            while(ServoSeek(addr,eLayer0) == FAIL) 
            { 
            	rw_seek_retry++;
            	if (rw_seek_retry > 10)
            	{
            		SendMsg5S(1, "AG Fail 2");
            		ShowError(BurnIn_Start_up_error);
            	}	
            }
    		StartTimer(&tray_out_t);
    		SendMsg5SValue(1, "B_seek_t2", 4, tray_out_t);

            ServoPause();
            
            StartTimer(&b_in_timer);
            while(ReadTimer(&b_in_timer) < 500L)
                ExitProcess();         
            
            bdDiscRequest.Type          = INIT_READ_AHEAD;           
            bdDiscRequest.TargetID.Long = StartPSN.PSN;
            bdDiscRequest.TargetLayer   = StartPSN.Layer;
            bdDiscRequest.ClusterCount  = ClusterRequest;

            bdReadAheadStartPSN = addr - 0x10; //don't make bdReadAheadStartPSN equal to dvdLastDecodedPSN
            
            Burn_in_mode = 1;
            DataBuffer.SectorCount = 0;

            ServoSetSpeed(e2XDCLV);
            SpeedMD = BypassSpeedMD;
            //SetBitsSCReg_1B(rwDVDCTRL,bBUFMODE);
            //WRITE_REG(HAL_DVDDREG_MAP->DVDCTRL,0x00);
            //WRITE_FIELD(HAL_DVDD_BUFMODE, 1);
            DiscReadStatus = bdStartDiscTransfer();
            DiscReadStatus = BUF_GOOD;
            if (DiscReadStatus != BUF_GOOD)    
            {
            	SendMsg5S(1, "AG Fail 3");
                ShowError(BurnIn_Start_up_error);
            }            
#endif
            /////////// Full Stroke test
            ServoSetSpeed( e2XDCLV );
            addr =0x9FFF6A;//(TAG ID)=(LBA)0x900000;
            addr3=0x160000;//(TAG ID)=(LBA)0x060000;
            Burn_in_timer = 0;
			StartTimer(&b_in_timer);
            for (i=0; i<40; i++)
            {
								
                StartTimer(&tray_in_t);
                //SendMsg5SValue(1, "BD Fseek time1", 4, tray_in_t);  
				rw_seek_retry =0;
				while(ServoSeek(addr3,eLayer0) == FAIL) 
                {
                	rw_seek_retry++;
                	if(rw_seek_retry > 10)
                	{
                		SendMsg5S(1, "AG Fail 4");
						ShowError(BurnIn_Seek_error);//[J50H]marked,BP28 BD bad!!
						break;
                	}                	
                }
                tray_out_t = ReadTimer(&tray_in_t);
                SendMsg5SValue(1, "BD_Fk time2", 4, tray_out_t);  
				if(ReadTimer(&tray_in_t) > 1500)
						ShowError(BurnIn_Seek_error);

				DelaymS(200);
				
                StartTimer(&tray_in_t);
				rw_seek_retry = 0;
                while(ServoSeek(addr,eLayer0) == FAIL)
                {
                	rw_seek_retry++;
					if(rw_seek_retry > 10)
                	{
                		SendMsg5S(1, "AG Fail 5");
						ShowError(BurnIn_Seek_error);//[J50H]marked,BP28 BD bad!!
						break;
					}
                } 
                tray_out_t = ReadTimer(&tray_in_t);
                SendMsg5SValue(1, "BD_Fk time4", 4, tray_out_t);
				if(ReadTimer(&tray_in_t) > 1500)
						ShowError(BurnIn_Seek_error);
            }
			Burn_in_timer = ReadTimer(&b_in_timer);
            //SendMsg5SValue(1, "BD Burn_in_timer", 4, Burn_in_timer);
            Burn_in_timer = (Burn_in_timer - 12000)/80;
			SendMsg5SValue(1, "BD FK_AVG", 4, Burn_in_timer);

            if(Burn_in_timer > 1000 )//[J50H]350,BP28 BD bad!!
            {
            	SendMsg5S(1, "AG Fail 6");
                ShowError(BurnIn_Seek_error);
            } 
            

            /////////// Random Stroke test
            ServoSetSpeed( e2XDCLV );
            addr =0x330000;//(TAG ID)=(LBA)0x230000;
            addr3=0x87FF6A;

			StartTimer(&b_in_timer);
            for(i=0; i<40; i++)
            {
            	rw_seek_retry =0;
            	StartTimer(&tray_in_t);		
				
				while(ServoSeek(addr,eLayer0) == FAIL)
				{
		        	rw_seek_retry++;
					if(rw_seek_retry >10)
		            	ShowError(BurnIn_Seek_error);
		        }
				
				tray_out_t = ReadTimer(&tray_in_t);	
				SendMsg5SValue(1, "R-seek t4", 4, tray_out_t);
		        if(ReadTimer(&tray_in_t) > 700)
					ShowError(BurnIn_Seek_error);
				
				DelaymS(200);

				rw_seek_retry =0;
				StartTimer(&tray_in_t);				
			
		        while(ServoSeek(addr3,eLayer0) == FAIL) 
		        {
		        	rw_seek_retry++;
					if(rw_seek_retry >10)
		            	ShowError(BurnIn_Seek_error);
		        }
				
				tray_out_t = ReadTimer(&tray_in_t);
				SendMsg5SValue(1, "R-seek t2", 4, tray_out_t);
				if(ReadTimer(&tray_in_t) > 700)
					ShowError(BurnIn_Seek_error);

				addr+=0x1000L;      //increase 10 sec;
                addr3+=0x4500L;     //increase 1 min;
                if(addr  >= 0x335000)      addr = 0x340000+(BYTE)b_in_timer;        //reset seek position
                if(addr3 >= 0x970000)     addr3 = 0x580000+(BYTE)b_in_timer;      //reset seek position


            }   
			Burn_in_timer = ReadTimer(&b_in_timer);
           	Burn_in_timer = (Burn_in_timer - 8000)/80;
			SendMsg5SValue(1, "R-Seek_AVG", 4, Burn_in_timer);
				
           	if(Burn_in_timer > 500 ) 
	        {
	    	    ShowError(BurnIn_Seek_error);
	        }

            test_bd = 1;
            OpenWritableSegmentTable(0x00);  //[B1AJ] 0x80->0x00 ePersistent_Talbe
            SetPersistentTable1B(AGING_OFFSET,0xAA);
            SetPersistentTable1B(AGING_OFFSET+1,0x01);
            CloseWritableSegmentTable(0x00);  //[B1AJ] 0x80->0x00 ePersistent_Talbe
            send_msg5S(1,"ALL PASS");	
        	
   }  
   else if ((pdbDiscLoaded() != BD_DISC) && (test_bd == 0)&& (test_cd == 1)&& (test_dvd == 1))  //BD_Disc  sky add for aging test
   {
   		ShowError(15);        
   }
   
   if ((pdbDiscLoaded() == DVD_DISC) && (test_dvd == 0)&& (test_cd == 1)) //DVD_DISC
   {                            	
            //DiscAccessStatusE   DiscReadStatus;
            //BYTE count,xfer_speed;
			//long xfer_Addr;	    	
	    	DelaymS(250); DelaymS(250); DelaymS(250); DelaymS(250); // Delay one second to discharge all previosu one  	    
			#if 0
			if ((test_dvd==0)&&(pdbGetDVDLayers() == DUAL_LAYER))    /* Check if DL disc */   
            {
                ShowError(BurnIn_Start_up_error);
            }
            else if((test_dvd > 0)&&(pdbGetDVDLayers() == SINGLE_LAYER))
            {
                ShowError(BurnIn_Start_up_error);
            }
            #endif
#if 0 //JJ close xfer test			
            // JAM START
            ServoSetSpeed(e4XDCAV);                        	

			//*****1st time****
            xfer_Addr	= 0x35000;	
            StartTimer(&tray_in_t);
            SendMsg5SValue(1, "DVD seek time1", 4, tray_in_t);
			rw_seek_retry = 0;
			while(ServoSeek(xfer_Addr,eLayer0) == FAIL) 
			{
				rw_seek_retry++;
				if(rw_seek_retry >10)
            		ShowError(BurnIn_Start_up_error);
            }
			StartTimer(&tray_in_t);
            SendMsg5SValue(1, "DVD seek time2", 4, tray_in_t);
			xfer_start_timer = xfer_end_timer = 0;
			StartTimer(&xfer_start_timer);				
			SendMsg5SValue(1, "Start time", 4, xfer_start_timer);
			for(count=0;count<10;count++)
			{
				send_msg74(1, 0, 0, 0, count);

				HostXfer.StartBlock.lba = xfer_Addr+(count*0x100);	
			    HostXfer.BlockCount.Long=0x100;				
			   	HostXfer.BlocksLeft = HostXfer.BlockCount.Long;						
				
				DVDxferTest();				
			}
			StartTimer(&xfer_end_timer);	
			SendMsg5SValue(1, "END time  ", 4, xfer_end_timer);
			xfer_speed = 8388608 / (xfer_end_timer-xfer_start_timer) / 1382.4 ;
			SendMsg5SValue(1,"1st speed=",2,xfer_speed);
			if(xfer_speed<1)
			{
                         ShowError(9);
            }
			
			//*****2nd time****
            xfer_Addr	= 0x100000;	
			rw_seek_retry = 0;
			while(ServoSeek(xfer_Addr,eLayer0) == FAIL) 
		    {	
		    	rw_seek_retry++;
				if(rw_seek_retry >10)
            		ShowError(BurnIn_Start_up_error);
            }
			
			xfer_start_timer = xfer_end_timer = 0;
			StartTimer(&xfer_start_timer);				
			SendMsg5SValue(1, "Start time", 4, xfer_start_timer);
			for(count=0;count<10;count++)
			{
				send_msg74(1, 0, 0, 0, count);

				HostXfer.StartBlock.lba = xfer_Addr+(count*0x100);	
			    HostXfer.BlockCount.Long=0x100;				
			   	HostXfer.BlocksLeft = HostXfer.BlockCount.Long;			
					
				DVDxferTest();								
			}
			StartTimer(&xfer_end_timer);	
			SendMsg5SValue(1, "END time  ", 4, xfer_end_timer);
			xfer_speed = 8388608 / (xfer_end_timer-xfer_start_timer) / 1382.4 ;
			SendMsg5SValue(1,"2nd speed=",2,xfer_speed);
			if(xfer_speed<2)
			{
                ShowError(9);
            }
			
			//*****3rd time****
            xfer_Addr	= 0x1A0000;	
			rw_seek_retry =0;
			while(ServoSeek(xfer_Addr,eLayer0) == FAIL) 
			{
				rw_seek_retry++;
				if(rw_seek_retry >10)
            		ShowError(BurnIn_Start_up_error);
            }
			
			xfer_start_timer = xfer_end_timer = 0;
			StartTimer(&xfer_start_timer);				
			SendMsg5SValue(1, "Start time", 4, xfer_start_timer);
			for(count=0;count<10;count++)
			{
				send_msg74(1, 0, 0, 0, count);

				HostXfer.StartBlock.lba = xfer_Addr+(count*0x100);	
			    HostXfer.BlockCount.Long=0x100;				
			   	HostXfer.BlocksLeft = HostXfer.BlockCount.Long;						
					
				DVDxferTest();				
			}
			StartTimer(&xfer_end_timer);	
			SendMsg5SValue(1, "END time  ", 4, xfer_end_timer);
			xfer_speed = 8388608 / (xfer_end_timer-xfer_start_timer) / 1382.4 ;
			SendMsg5SValue(1,"3rd speed=",2,xfer_speed);
			if(xfer_speed<3)
			{
                ShowError(9);	
            }			
			
            // JAM END          
            /////////// Xfer rate test
            ServoSetSpeed(e4XDCAV);
            addr = 0x210000L;
            StartTimer(&tray_in_t);				
	    	SendMsg5SValue(1, "Xfer time1", 4, tray_in_t);
			rw_seek_retry = 0;
            while(ServoSeek(addr,eLayer0) == FAIL) 
            {
            	rw_seek_retry++;
            	if(rw_seek_retry >10)
					break;
                //ShowError(BurnIn_Start_up_error);//[J50H]marked,BP28 BD bad!!
            }
            StartTimer(&tray_in_t);				
	    	SendMsg5SValue(1, "Xfer time2", 4, tray_in_t);

            ServoPause();
            
            StartTimer(&b_in_timer);
            while(ReadTimer(&b_in_timer) < 500L)
                ExitProcess();

            dvdDiscRequest.Type            = INIT_READ;
            dvdDiscRequest.TargetID.Long   = addr;
            dvdDiscRequest.TargetLayer     = eLayer0;
            dvdDiscRequest.EccBlockCount   = 0x20;

            dvdReadAheadStartPSN = addr - 0x10; //don't make dvdReadAheadStartPSN equal to dvdLastDecodedPSN
            
            Burn_in_mode = 1;
            DataBuffer.SectorCount = 0;

            ServoSetSpeed(e4XDCAV);
            SpeedMD = BypassSpeedMD;
            //SetBitsSCReg_1B(rwDVDCTRL,bBUFMODE);
            WRITE_REG(HAL_DVDDREG_MAP->DVDCTRL,0x00);
            WRITE_FIELD(HAL_DVDD_BUFMODE, 1);
            DiscReadStatus = dvdStartDiscTransfer();
            if (DiscReadStatus != BUF_GOOD)    
            {           					
	       		SendMsg5SValue(1, "DVD Buf NG", 4, tray_in_t);
               	//ShowError(BurnIn_Start_up_error);
            }

            DataBuffer.SectorCount = (DataBuffer.SectorCount << 5)/Burn_in_timer;

            if (DataBuffer.SectorCount < 50) 
            {
            	SendMsg5SValue(1, "DVD Buf NG2", 4, tray_in_t);
                //ShowError(BurnIn_Start_up_error);
            }
#endif
            /////////// Full Stroke test
            ServoSetSpeed( e4XDCAV );
            if(pdbGetDVDLayers() == DUAL_LAYER)
            	addr=0x200000;
            else
            	addr=0x210000;
            
            addr3=0x33000;
            Burn_in_timer = 0;

			StartTimer(&b_in_timer);
			for (i=0; i<40; i++)
	        {	     
	                StartTimer(&tray_in_t);				
		        	//SendMsg5SValue(1, "F0seek t1", 4, tray_in_t);
					rw_seek_retry =0;
	                while(ServoSeek(addr3,eLayer0) == FAIL) 
	                {
	                	rw_seek_retry++;
						if(rw_seek_retry >10)
	                		ShowError(BurnIn_Seek_error);
	                }
					tray_out_t = ReadTimer(&tray_in_t);
					SendMsg5SValue(1, "F0 seek t2", 4, tray_out_t);
					if(ReadTimer(&tray_in_t) > 1000)
						ShowError(BurnIn_Seek_error);	                
	                
					DelaymS(200);
					
	                StartTimer(&tray_in_t);				
		        	//SendMsg5SValue(1, "F0seek t3", 4, tray_in_t);
					rw_seek_retry = 0;
	                while(ServoSeek(addr,eLayer0) == FAIL) 
	                {
	                	rw_seek_retry++;
						if(rw_seek_retry >10)
	                		ShowError(BurnIn_Seek_error);
	                }
					tray_out_t = ReadTimer(&tray_in_t);
					SendMsg5SValue(1, "F0 seek t4", 4, tray_out_t);
	                if(ReadTimer(&tray_in_t) > 1000)
						ShowError(BurnIn_Seek_error);
	        }
			Burn_in_timer = ReadTimer(&b_in_timer);
            //SendMsg5SValue(1, "B/I_time", 4, Burn_in_timer);
            Burn_in_timer = (Burn_in_timer - 8000)/80;
			SendMsg5SValue(1, "F0Seek_AVG", 4, Burn_in_timer);
				
            if(Burn_in_timer > 500 ) 
	        {
	        	ShowError(BurnIn_Seek_error);
	        }
			
			//D9 Full Seek
			if(pdbGetDVDLayers() == DUAL_LAYER)
			{
				addr =0xE05F40;//(TAG ID)=(LBA)0x230000;
				addr3=0xFCFF57;//(TAG ID)=(LBA)0x3FA08F;
				
				StartTimer(&b_in_timer);
				for (i=0; i<40; i++)
		        {	      
		                StartTimer(&tray_in_t);				
			        	//SendMsg5SValue(1, "F1seek t1", 4, tray_in_t);
						rw_seek_retry =0;
		                while(ServoSeek(addr3,eLayer1) == FAIL) 
		                {
		                	rw_seek_retry++;
							if(rw_seek_retry >10)
		                		ShowError(BurnIn_Seek_error);
		                }
						tray_out_t = ReadTimer(&tray_in_t);
						SendMsg5SValue(1, "F1 seek t2", 4, tray_out_t);
						if(ReadTimer(&tray_in_t) > 1000)
							ShowError(BurnIn_Seek_error);		                
		                
						DelaymS(200);
						
		                StartTimer(&tray_in_t);				
			        	//SendMsg5SValue(1, "F1seek t3", 4, tray_in_t);
						rw_seek_retry = 0;
		                while(ServoSeek(addr,eLayer1) == FAIL) 
		                {
		                	rw_seek_retry++;
							if(rw_seek_retry >10)
		                		ShowError(BurnIn_Seek_error);
		                }
						tray_out_t = ReadTimer(&tray_in_t);
						SendMsg5SValue(1, "F1 seek t4", 4, tray_out_t);
		                if(ReadTimer(&tray_in_t) > 1000)
							ShowError(BurnIn_Seek_error);
		        }
				Burn_in_timer = ReadTimer(&b_in_timer);
            	//SendMsg5SValue(1, "B/I_time", 4, Burn_in_timer);
            	Burn_in_timer = (Burn_in_timer - 8000)/80;
				SendMsg5SValue(1, "F1Seek_AVG", 4, Burn_in_timer);
				
            	if(Burn_in_timer > 500 ) 
	            {
	                ShowError(BurnIn_Seek_error);
	            }
			}

            /////////// Random Stroke test
            ServoSetSpeed( e4XDCAV );
			if(pdbGetDVDLayers() == DUAL_LAYER)
			{
				addr =0xE05F40;
				addr3=0xF7FF57;
			}
			else
			{
				addr =0x35000L;
            	addr3=0x180000L;
			}	
            StartTimer(&b_in_timer);
            for(i=0; i<40; i++)
            {
            	rw_seek_retry =0;
            	StartTimer(&tray_in_t);		
				if(pdbGetDVDLayers() == DUAL_LAYER)
				{
					while(dvdSeek(addr,eLayer1) == FAIL)
					{
		                	rw_seek_retry++;
							if(rw_seek_retry >10)
		                		ShowError(BurnIn_Seek_error);
		            }
				}
				else
				{
					while(ServoSeek(addr,eLayer0) == FAIL)
					{
		                	rw_seek_retry++;
							if(rw_seek_retry >10)
		                		ShowError(BurnIn_Seek_error);
		            }
				}
				tray_out_t = ReadTimer(&tray_in_t);	
				SendMsg5SValue(1, "R-seek t4", 4, tray_out_t);
		        if(ReadTimer(&tray_in_t) > 700)
					ShowError(BurnIn_Seek_error);
				
				DelaymS(200);

				rw_seek_retry =0;
				StartTimer(&tray_in_t);
				if(pdbGetDVDLayers() == DUAL_LAYER)
				{
					while(dvdSeek(addr3,eLayer1) == FAIL) 
		            {
		               	rw_seek_retry++;
						if(rw_seek_retry >10)
		               		ShowError(BurnIn_Seek_error);
		            }

				}
				else
				{
		            while(ServoSeek(addr3,eLayer0) == FAIL) 
		            {
		               	rw_seek_retry++;
						if(rw_seek_retry >10)
		               		ShowError(BurnIn_Seek_error);
		            }
				}
				tray_out_t = ReadTimer(&tray_in_t);
				SendMsg5SValue(1, "R-seek t2", 4, tray_out_t);
				if(ReadTimer(&tray_in_t) > 700)
					ShowError(BurnIn_Seek_error);
				
                
                addr+=0x1000L;      //increase 10 sec;
                addr3+=0x4500L;     //increase 1 min;
                if(pdbGetDVDLayers() == DUAL_LAYER)
				{
					if(addr  >= 0xE10000)      addr = 0xE05F40+(BYTE)b_in_timer;        //reset seek position
                	if(addr3 >= 0xFC0000)     addr3 = 0xF10000+(BYTE)b_in_timer;      //reset seek position
				}
				else
				{
                	if(addr >= 0x100000L)      addr = 0x35000L+(BYTE)b_in_timer;        //reset seek position
                	if(addr3 >= 0x1C0000L)     addr3 = 0x150000L+(BYTE)b_in_timer;      //reset seek position
				}	

            }   
			Burn_in_timer = ReadTimer(&b_in_timer);
           	Burn_in_timer = (Burn_in_timer - 8000)/80;
			SendMsg5SValue(1, "R-Seek_AVG", 4, Burn_in_timer);
				
           	if(Burn_in_timer > 500 ) 
	        {
	    	    ShowError(BurnIn_Seek_error);
	        }
            
            test_dvd=1;
            OpenWritableSegmentTable(0x00);  //[B1AJ] 0x80->0x00 ePersistent_Talbe
            SetPersistentTable1B(AGING_OFFSET,0x02);
            CloseWritableSegmentTable(0x00);  //[B1AJ] 0x80->0x00 ePersistent_Talbe
   }   
   else if ((pdbDiscLoaded() != DVD_DISC) && (test_dvd == 0) && (test_cd == 1)) //DVD_DISC
   {
   		ShowError(15);
   }
   if ((pdbDiscLoaded() == CD_DISC) && (test_cd == 0)) //CD_DISC
   {
      	//TimeS time;        	                
        Read_Lead_in = FALSE;
        DelaymS(250); DelaymS(250); DelaymS(250); DelaymS(250); // Delay one second to discharge all previosu one
#if 0 //JJ close xfer test
//////////////////////Transfer Rate
    	ServoSetSpeed( e10XCAV );
    	cd_transfer();       
////////////////////////Transfer Rate End
           
/////////// Xfer rate test
        ServoSetSpeed( e10XCAV );
        addr=314850L; //70:0:0
        StartTimer(&tray_in_t);				
	   	SendMsg5SValue(1, "CDxf1", 4, tray_in_t);
		rw_seek_retry = 0;
            while(ServoSeek(addr,0) == FAIL) 
            {    
            	rw_seek_retry++;
				if(rw_seek_retry >10)
					ShowError(BurnIn_Start_up_error);
            }
            StartTimer(&tray_in_t);				
	    	SendMsg5SValue(1, "CDxf2", 4, tray_in_t);

            ServoPause();

            StartTimer(&b_in_timer);
            while(ReadTimer(&b_in_timer) < 500L)
                ExitProcess();

            ServoSetSpeed( e10XCAV );
            SpeedMD = BypassSpeedMD;
            StartTimer(&tray_in_t);				
	    	SendMsg5SValue(1, "CDxf3", 4, tray_in_t);
			rw_seek_retry =0;
			while(ServoSeek(addr,0) == FAIL) 
            {
            	rw_seek_retry++;
				if(rw_seek_retry >10)
               		ShowError(BurnIn_Start_up_error);
            }
            StartTimer(&tray_in_t);				
	    	SendMsg5SValue(1, "CDxf4", 4, tray_in_t);
            
            svoIramVar.subQEnableFlag = 1;
            svoEnTimeRead();
            pdbSetSubQIntStatus(FALSE);
            StartTimer(&b_in_timer);
            while(1)
            {
                if(pdbGetSubQIntStatus())
                {
                    pdbGetABSTime(&time);
                    pdbSetSubQIntStatus(FALSE);
                    if ((time.Minute == 0x69) && (time.Second > 0x30)) break;
                }

                if(ReadTimer(&b_in_timer) > 4000L) //[J50H]2000,BP28 bad!
                {
                	SendMsg5SValue(1,"RD Time",4,ReadTimer(&b_in_timer));
                	ShowError(BurnIn_Start_up_error);
                }                
            }

            Burn_in_mode = 1;
            DataBuffer.SectorCount = 0;
            DataBuffer.CacheEndLBA = 315010L;
            //WriteSCReg_1B (rwDInt0, 0);
            WRITE_REG(HAL_CDDREG_MAP->DINT0, 0);
            InitBuffering();

            StartTimer(&b_in_timer);
            while (Svr_pause_reqire == FALSE)
            {
                if(ReadTimer(&b_in_timer) > 2000L) 
                {
                  ShowError(BurnIn_Start_up_error);
                }
                
            }
            b_in_timer = ReadTimer(&Burn_in_timer);
            DataBuffer.SectorCount = (DataBuffer.SectorCount << 5)/b_in_timer;
            if (DataBuffer.SectorCount < 10) //ShowError(BurnIn_Start_up_error);
            {
            	send_msg5S(1,"sky err6  databuff");
            	//ShowError(BurnIn_Start_up_error);
            }
#endif
            /////////// Full Stroke test
            ServoSetSpeed( e10XCAV );
            addr=292500L; // 65:02:00
            addr3=4500L; //1:02:00
            Burn_in_timer = 0;
			StartTimer(&b_in_timer);
            for (i=0; i<40; i++)
            {     
				
				rw_seek_retry =0;
				StartTimer(&tray_in_t);
                while(ServoSeek(addr3,0) == FAIL) 
                {
                	rw_seek_retry++;
                	if(rw_seek_retry > 10)
                 		ShowError(BurnIn_Seek_error);
                }
                tray_out_t = ReadTimer(&tray_in_t);
				SendMsg5SValue(1, "CD seek t2", 4, tray_out_t);
				if(ReadTimer(&tray_in_t) > 1000)
					ShowError(BurnIn_Seek_error);                       

				DelaymS(100);
				
                rw_seek_retry =0;
				StartTimer(&tray_in_t);
                while(ServoSeek(addr,0) == FAIL) 
                {
                	rw_seek_retry++;
					if(rw_seek_retry >10)
						ShowError(BurnIn_Seek_error);
                }
				tray_out_t = ReadTimer(&tray_in_t);
				SendMsg5SValue(1, "CD seek t4", 4, tray_out_t);
				if(ReadTimer(&tray_in_t) > 1000)
					ShowError(BurnIn_Seek_error);                
                
            }
			Burn_in_timer = ReadTimer(&b_in_timer);
            //SendMsg5SValue(1, "B/I_time", 4, Burn_in_timer);
            Burn_in_timer = (Burn_in_timer - 8000)/80;
			SendMsg5SValue(1, "CD-FSeek_AVG", 4, Burn_in_timer);
				
            if(Burn_in_timer > 500 ) 
	        {
	        	ShowError(BurnIn_Seek_error);
	        }        

            /////////// Random Stroke test
            ServoSetSpeed( e10XCAV );
            addr=2250L;
            addr3=210000L;
            StartTimer(&b_in_timer);
            for(i=0; i<40; i++)
            {
                if(ServoSeek(addr,0) == FAIL)   result++;
                if(ServoSeek(addr3,0) == FAIL)  result++;
                addr+=750L;       //increase 10 sec;
                addr3+=4500L;     //increase 1 min;
                if(addr >= 22500L)      addr = 2250L+(BYTE)b_in_timer;          //reset seek position
                if(addr3 >= 315000L)     addr3 = 210000L+(BYTE)b_in_timer;      //reset seek position

            }
			Burn_in_timer = ReadTimer(&b_in_timer);
           	Burn_in_timer = (Burn_in_timer - 8000)/80;
			SendMsg5SValue(1, "R-Seek_AVG", 4, Burn_in_timer);
				
           	if(Burn_in_timer > 500 ) 
	        {
	    	    ShowError(BurnIn_Seek_error);
	        }
			
            svoIramVar.subQEnableFlag = 0;
            test_cd = 1;
            OpenWritableSegmentTable(0x00);  //[B1AJ] 0x80->0x00 ePersistent_Talbe
            SetPersistentTable1B(AGING_OFFSET,0x01);
            CloseWritableSegmentTable(0x00);  //[B1AJ] 0x80->0x00 ePersistent_Talbe
        }//else if ((pdbDiscLoaded() == CD_DISC) && (test_cd == 0))
        else if ((pdbDiscLoaded() != CD_DISC) && (test_cd == 0)) //CD_DISC
        {
                ShowError(15);
        }

#endif
        

        //TrayManagerInfo.Command = TRAY_COMMAND_TRAY_OUT;        
        ServoStop();
        EjectKeyInfo.PressedValid = FALSE;
    	TrayManagerInfo.Pushed = 0;
        svoIramVar.emergencyStopFlag = FALSE;
        StopDecoder();
        ret = ServoTrayOut();

        if ((test_dvd == 1) || (test_cd == 1) || (test_bd == 1))
        {
            Iram_LED_Flash_timer = 0;
            //ReadyLED(LED_FLASH);
        }
        if ((test_dvd == 1) && (test_cd == 1) && (test_bd == 1))
  		{   	
  			Show_Error2LED(LED_OFF);
        	TrayManagerInfo.Command = TRAY_COMMAND_TRAY_OUT;
        	svoIramVar.emergencyStopFlag = FALSE;
        	ret = ServoTrayOut();
        	//ReadyLED(BurnIn_LED_ON);
        	while(1){};
  		}

        //Iram_eject_no = EJECT_SW_WATCH;

        //monitor user push eject button or tray in
       	// while (Iram_eject_no != EJECT_EXECUTING)
      	TrayManagerInfo.TrayState = TRAY_STATE_OUT_OK;
    #if(SET_SLOT_IN == 0)
    	while(EjectKeyInfo.PressedValid == FALSE && TrayManagerInfo.Pushed == 0)
        	ExitProcess();
    #else
    	while(1) // daniel sky for BP-S26 aging test-change disc
    	{
     		if((ipOpenSW == 0) && (ipSW2 == 0) && (ipCloseSW == 1))
      			break;     
    	}
    	while(1) // daniel sky for BP-S26 aging test-change disc
     	{
      		if((ipOpenSW == 1) && (ipSW2 == 0) && (ipCloseSW == 1))
      			break;
     	}
    #endif      

        //clear LED status
        ReadyLED(LED_OFF);

        EjectKeyInfo.PressedValid = FALSE;
    	TrayManagerInfo.Pushed = 0;

        svoIramVar.emergencyStopFlag = FALSE;
        StopDecoder();

        ret = ServoTrayIn();
       	TrayManagerInfo.TrayState = TRAY_STATE_IN_OK;
        //Iram_eject_no = EJECT_SW_WATCH;
//        EjectKeyInfo.Disable = FALSE;
//-------------------------------------------------        
        
//-------------------------------------------------
    }//while(1)

    //TrayManagerInfo.Command = TRAY_COMMAND_TRAY_OUT;
    EjectKeyInfo.PressedValid = FALSE;
    TrayManagerInfo.Pushed = 0;
    svoIramVar.emergencyStopFlag = FALSE;
    StopDecoder();
    ret = ServoTrayOut();

    //ReadyLED(LED_ON);

    
   TrayManagerInfo.TrayState = TRAY_STATE_OUT_OK;
    #if(SET_SLOT_IN == 0)
    	while(EjectKeyInfo.PressedValid == FALSE && TrayManagerInfo.Pushed == 0)
        	ExitProcess();
    #else
    	while(1) //daniel sky for BP-S26 aging test-change disc
    	{
     		if((ipOpenSW == 0) && (ipSW2 == 0) && (ipCloseSW == 1))
      			break;     
    	}
    	while(1) //daniel sky for BP-S26 aging test-change disc
     	{
      		if((ipOpenSW == 1) && (ipSW2 == 0) && (ipCloseSW == 1))
      			break;
     	}
    #endif  

    EjectKeyInfo.PressedValid = FALSE;
    TrayManagerInfo.Pushed = 0;
    svoIramVar.emergencyStopFlag = FALSE;
    StopDecoder();

    ret = ServoTrayIn();
    TrayManagerInfo.TrayState = TRAY_STATE_IN_OK;
    //#endif  // (TRAVERSE_TEST == 1 ) #if !0
}

/********************************************************
* cd_transfer
*
* Common read commands Handler
*
* Returns:      void
* Params:       none
********************************************************/
void cd_transfer(void)
{
 	ULONG ReadCDSeekTimer;        /* For reduse the seek time */
 	ULONG xfer_start_timer, xfer_end_timer;
 	int xfer_final_timer;
 	BYTE a;
 	long h2go;
	//USHORT Sa;
	vBOOL flg;
	BYTE rw_seek_retry;
	long lba;
	//USHORT w_temp;
	SHORT  ExitPer256ms, ExitCount;

    /*****************************    FW Bug    ******************************/
    /* The read_timer is used instead of svoIramVar.knlTimeupCount.
        The voIramVar.knlTimeupCount is reset by some called functions in this function,
        and that causes voIramVar.knlTimeupCount is incorrect for this read command..*/

	ULONG read_timer;

#if (EN_POWER_CONTROL)
    SetDiscReadPowerMode();
#endif
    DelaymS(250);
////
	for(skyaddr=10000;skyaddr<=201000;skyaddr=skyaddr+100)//25
    {
    	if((skyaddr >= 10000) && (skyaddr < 80000))//1st
    	{   	 
 	 		skyaddr = 80000;
 	 		xfer_start_timer = xfer_end_timer = 0;
        	StartTimer(&xfer_start_timer);				
			SendMsg5SValue(1, "1st ST", 4, xfer_start_timer);
			rw_seek_retry =0;
 	 		while(ServoSeek(skyaddr,0) == FAIL) 
 	 		{
 	 			rw_seek_retry++;
				if(rw_seek_retry > 10)
               		ShowError(BurnIn_Start_up_error);           
         	}           
 		}
 	 	if((skyaddr >= 81000) && (skyaddr < 150000))//2nd
 		{
			StartTimer(&xfer_end_timer);				
			SendMsg5SValue(1, "1st EN", 4, xfer_end_timer);
			xfer_final_timer = xfer_end_timer - xfer_start_timer;
			//send_msg74(SHOW_DEBUG_MSG,0x51,0x66,0x95,xfer_final_timer);
			if((xfer_end_timer - xfer_start_timer)>1600)//[B008] Bios 800-->850 [B009] 900
			{
				send_msg5S(1,"1st fail"); 
	        	ShowError(BurnIn_Start_up_error);
        	}
 	 		skyaddr = 150000;
 	 		xfer_start_timer = xfer_end_timer = 0;
        	StartTimer(&xfer_start_timer);				
			SendMsg5SValue(1, "2nd ST", 4, xfer_start_timer);
			xfer_final_timer = xfer_end_timer - xfer_start_timer;
			//send_msg74(SHOW_DEBUG_MSG,0x51,0x66,0x95,xfer_final_timer);
			rw_seek_retry =0;
         	while(ServoSeek(skyaddr,0) == FAIL)
         	{
         		rw_seek_retry++;
				if(rw_seek_retry >10)

              		ShowError(BurnIn_Start_up_error); 
         	}
        }
 	 	if((skyaddr >= 151000) && (skyaddr < 200000))//3rd
 		{ 
 	 		StartTimer(&xfer_end_timer);				
			SendMsg5SValue(1, "2nd EN", 4, xfer_end_timer);
			xfer_final_timer = xfer_end_timer - xfer_start_timer;
			//send_msg74(SHOW_DEBUG_MSG,0x51,0x66,0x95,xfer_final_timer);
			//send_msg74(SHOW_DEBUG_MSG,0x51,0x66,0x95,skyaddr);
			if((xfer_end_timer - xfer_start_timer)>600)//[B008] Bios 300-->350 [B009] 400
			{
				send_msg5S(1,"2st fail"); 
				ShowError(BurnIn_Start_up_error); 	
        	}
 	 		skyaddr = 200000;
 	 		xfer_start_timer = xfer_end_timer = 0;
        	StartTimer(&xfer_start_timer);				
			SendMsg5SValue(1, "3rd ST", 4, xfer_start_timer);
			rw_seek_retry =0;
			while(ServoSeek(skyaddr,0) == FAIL)
         	{    
         		rw_seek_retry++;
				if(rw_seek_retry >10)
					ShowError(BurnIn_Start_up_error); 
         	}
        }
    	Rw_C2PO_data = FALSE;
    	Rw_subc_data = FALSE;
    	Read_subc_only = FALSE;
    	Iram_Rd_subc_flag = FALSE;
    	Rw_rdcd_cmd = FALSE;
    
    /*** save host request start address to buf_start_blk ***/
    	h2go = HostXfer.StartBlock.lba = skyaddr;//HostXfer.StartBlock.lba = PacketCommandToLBALEN4(2);
          
    /*** check requested block's track writing method ***/
    	rd_method = CheckTrackWriteMethod(HostXfer.StartBlock.lba) & 0x0F;
    
    	HostXfer.StartBlock.lba = h2go;
    
    /* host request length */
        HostXfer.BlocksLeft = h2go = 25L;       
        
        if ( h2go != 0 )
    	{
        /* Calculate the ending address, subtract one blk to get the real
           ending lba address. */
        	h2go += HostXfer.StartBlock.lba;                /* add req len to start lba */
        	h2go--;
        	Tg_endlba = h2go;
    	}
    //cd_transfer();  

////
		StartTimer(&read_timer);

		if(SpeedMD == Set_sp_f)
    	{   	
        	SpeedMD=Special_f;
    	}
    	/*else if(SpeedMD == Write_f)
    	{    	
        	Iram_not_change_speed_timer = 0;
        	SpeedMD = MiddleMode;
    	}*/
    
    HostXfer.BlockCount.Long = HostXfer.BlocksLeft;
    
    StartTimer(&ReadCDSeekTimer); /* For reduce the seek time */
    
    DISABLE_INTERRUPT();                        /* Disable all interrupt */
    LBA_SUB(HostXfer.StartBlock.lba, DataBuffer.CacheEndLBA, lba, flg);
    
    ////seek
    /* Entry point of seek operation start block is not in buffer,
        we need a seek operation */
    rw_seek_retry = 0;
    Rw_seek_blk.lba = HostXfer.StartBlock.lba;      /* initial seek block */
    //AdjustSeekOffset();                          /* execute adjust rw_seek_blk operation */

    DelaymS(25);

////
    if (lba == 0)
    {
            /* HostXfer.StartBlock = DataBuffer.CacheEndLBA,
             adjust the DataBuffer.CurHostPtr to DataBuffer.CurDiscPtr. */
              
            if ( DataBuffer.SectorCount != 0 )  /* if buffer is empty */
            {
                DataBuffer.CurHostPtr = DataBuffer.CurDiscPtr;
                if ( DataBuffer.CurHostPtr == 0)
                    DataBuffer.CurHostPtr = RING_END_PAGE;
                else
                    DataBuffer.CurHostPtr --;

                DataBuffer.SectorCount   = 1;

                ENABLE_INTERRUPT();             /* Enable all interrupt */
                DataBuffer.CacheStartLBA = HostXfer.StartBlock.lba;
                MINUS_LBA_HEADER_1_FRAME(DataBuffer.CacheStartLBA , 1);
                SendHostBlocks();               /* xfr blk to host */               
            }
        }
        else
        {                      
    
    		ENABLE_INTERRUPT();
    		a = WaitForCacheHit();           /* wait it come */
    		if ( a == 0 )
            {                               /* and no error occur in target block */
            	SendHostBlocks();                    
            }
        }

    Iram_Knl_timer_counter = 40;    /* Wait about 800 ms */
    a = GET_INT_ENABLE_MASK();     /* Improved sometime seek(Access time) test fail */
    DISABLE_INTERRUPT();
    Knl_timeout = FALSE;            /* Assume no time out */
    Rw_rd_monitor = TRUE;
    SET_INT_ENABLE_MASK(a);
    rw_seek_retry = 0x00;
    
    /*** readloop ***/
    ExitPer256ms = ExitCount = 0;
    
     while (1)
    {
        //ExitProcess();
        ExitPer256ms = ReadTimer(&read_timer)>>8;
       
        if(ExitCount < ExitPer256ms)//ExitProcess Pre 256msec
        {        	
            ExitCount = ExitPer256ms;
            goto cd_end;           
        }

        if (St_cmd_inproc == FALSE)
        {        	
            ReadCDExit();
        #if(BACKGROUND_READ_AHEAD == 1)
            if(Rw_dec_stop) {
                START_BACKGROUND_BUFFER = TRUE;
            }
        #endif
            goto cd_end;           
        }
        else
        {
            if(EjectKeyInfo.PressedValid){
                BUILD_SENSE(0x02,0x3A,0x00,0x16);    /* MEDIUM NOT PRESENT */
                ProcessEjectRequest();
                goto cd_end;                
            }                    
            if(Iram_reload == TRUE)
                continue;                       /* readloop, if dec is running */

            if ( Rw_dec_stop == FALSE)
                continue;                       /* readloop, if dec is running */

                     
        }
        /* chk_paralle_buffering: */
        /* check if buffer less than MAX-5, if it is do parallel buffer */
        

        /* reload start_block with dec_block and start seek again */
        
    }                                           /* end readloop */
	cd_end:
 		SendMsg5SValue(1, "end", 4, xfer_end_timer);
 	}//for
   StartTimer(&xfer_end_timer);				
   SendMsg5SValue(1, "3rd EN", 4, xfer_end_timer);
   xfer_final_timer = xfer_end_timer - xfer_start_timer;
   //send_msg74(SHOW_DEBUG_MSG,0x51,0x66,0x95,xfer_final_timer);
   if((xfer_end_timer - xfer_start_timer)>500)//[B008] Bios 250-->300 [B009] 350
   {
   	 send_msg5S(1,"3st fail"); 
   	 ShowError(BurnIn_Start_up_error); 
   }
}//cd_transfer

/********************************************************
* BD_transfer
*
* Common read commands Handler
*
* Returns:      void
* Params:       none
********************************************************/
void bd_transfer(void)
{
	ULONG xfer_start_timer, xfer_end_timer;
 	SearchCacheResultE  SearchResult;   /* Holds the result of a cache search           */
 	DiscAccessStatusE   DiscReadStatus; /* Status of buffer after request completed     */
 
	//bdaddr = 0x50000;
 	for(bdaddr=0x10000;bdaddr<0x203200;bdaddr=bdaddr+0x32)
	{
	 	if((bdaddr >= 0x13200) && (bdaddr < 0x70000))
	 	{   	 
	 	 	bdaddr = 0x70000;
	 	 	xfer_start_timer = xfer_end_timer = 0;
	        StartTimer(&xfer_start_timer);				
			SendMsg5SValue(1, "1st time", 4, xfer_start_timer); 	           
	 	}
	 	
	 	if((bdaddr >= 0x73200) && (bdaddr < 0x130000))
	 	{   	 
	 	 	bdaddr = 0x130000;	 
	        StartTimer(&xfer_end_timer);				
			SendMsg5SValue(1, "1st time", 4, xfer_end_timer);
			if((26214400/(xfer_end_timer-xfer_start_timer)/3655)< 1.5) 
			{         
	           	ShowError(BurnIn_Start_up_error);
	        }
			xfer_start_timer = xfer_end_timer = 0;
	        StartTimer(&xfer_start_timer);				
			SendMsg5SValue(1, "2nd time", 4, xfer_start_timer); 	           
	 	}
	 	
	 	if((bdaddr >= 0x133200) && (bdaddr < 0x200000))
	 	{   	 
	 	 	bdaddr = 0x200000; 	 
	        StartTimer(&xfer_end_timer);				
			SendMsg5SValue(1, "2nd time", 4, xfer_end_timer);
			if((26214400/(xfer_end_timer-xfer_start_timer)/3655)< 1.5) 
			{               
	        	ShowError(BurnIn_Start_up_error);
	        }
			xfer_start_timer = xfer_end_timer = 0;
	        StartTimer(&xfer_start_timer);				
			SendMsg5SValue(1, "3rd time", 4, xfer_start_timer); 	         
	 	}
	 	
	    HostXfer.StartBlock.lba    = bdaddr;
	    HostXfer.BlockCount.Long  = 0x32L;
	 
	 	/*-------------------------------------------------------------------
	        Initial remaining blocks to transfer
	    --------------------------------------------------------------------*/
	    HostXfer.BlocksLeft = HostXfer.BlockCount.Long;
	    
	    /*---------------------------------------------------------------
	        Set to maximum speed
	    ----------------------------------------------------------------*/
	    if(SpeedMD==Set_sp_f)
	    {   	
	        SpeedMD=Special_f;
	    }

	    /*if(SpeedMD == Write_f)
	    {    	
	        Iram_not_change_speed_timer = 0;
	        SpeedMD = MiddleMode;
	    }*/

	    CmdSeekOk = 0;      /*initial flag */
	    
	    SearchResult = bdSearchCache(HostXfer,WAIT_SEARCH_MODE);
	    //SearchResult = bdSearchCache(HostXfer,IMMEDIATE_SEARCH_MODE);
	    
	    if(SearchResult != FULL_CACHE_HIT)
	    {                 
	    	#if (EN_POWER_CONTROL)
	        	SetDiscReadPowerMode();
	        #endif	        
	    }
	    
	    /*---------------------------------------------------------------
	        Process the search result. Each process returns the status
	        of the buffer access (DiscReadStatus).

	        NOTE: For bdFullCacheHit() DiscReadStatus is always BUF_GOOD.
	    ---------------------------------------------------------------*/
	    switch (SearchResult)
	    {

	        case FULL_CACHE_HIT:       
	         	if (HostXfer.StartBlock.lba != DataBuffer.CacheStartLBA)
	        		bdAdjustHostBufferPtr( HostXfer.StartBlock.lba );

	        	/*---------------------------------------------------------------
	        	Start the host transfer.
	        	---------------------------------------------------------------*/
	        	SendHostBlocks();
	        	DiscReadStatus = BUF_GOOD;
	            
	            break;

	        case CACHE_MISS:         
	            DiscReadStatus = bd_miss();          
	            break;

	        case PARTIAL_CACHE_HIT:         
	         	DiscReadStatus = bd_transfer2();
	            break;

	    } 
	 	bdReadExit(); 
	}//for        
 	StartTimer(&xfer_end_timer);				
	SendMsg5SValue(1, "3rd time", 4, xfer_end_timer);   
	if((26214400/(xfer_end_timer-xfer_start_timer)/3655)<2) 
	{                
		SendMsg5S(1, "AG Fail 1 ");
		ShowError(BurnIn_Start_up_error);  
	}
  
}




DiscAccessStatusE bd_transfer2()
{
         //TransferS           NewReq;             /* Truncated host request */
         DiscAccessStatusE   DiscReadStatus;     /* State of buffer after request to load buffer  */
         ULONG               HostEndLBA;         /* Ending LBA of host transfer request */

    bdDecFlag.fDISABLE_READ_AHEAD = FALSE;
    
    if (HostXfer.StartBlock.lba != DataBuffer.CacheStartLBA)
        bdAdjustHostBufferPtr( HostXfer.StartBlock.lba );
        
     /*---------------------------------------------------------------
        Start the host transfer.
    ---------------------------------------------------------------*/
    SendHostBlocks();

    /*---------------------------------------------------------------
        Determine last LBA of host transfer
    ---------------------------------------------------------------*/
    HostEndLBA = (HostXfer.StartBlock.lba + HostXfer.BlockCount.Long) - 1L;
    
    
    
    /*---------------------------------------------------------------
           Check if last host sector is in the buffer
        ---------------------------------------------------------------*/
        if (HostEndLBA <= DataBuffer.CacheEndLBA)
        {
            /*---------------------------------------------------------------
               If host is waiting for data restart the host transfer
               else just return a good status
            ---------------------------------------------------------------*/
            if (HostXfer.State == WAITING_FOR_TRANSFER)
            {
                HostXfer.State = TRANSFERRING;
                SendHostBlocks();
            }
            return (BUF_GOOD);
        }
        else
        {
        /*---------------------------------------------------------------
            Buffering is still on and may load more host blocks.
            Pass the host request and extend buffering
        ---------------------------------------------------------------*/
        send_msg5S(SEND_READ_DVD_MSG,"*EXTEND_READ*");
        DiscReadStatus = bdLoadBuffer( HostXfer, EXTEND_READ);

        /*---------------------------------------------------------------
            If buffering good, all host blocks have been loaded so exit
        ---------------------------------------------------------------*/
        if (DiscReadStatus == BUF_GOOD)
            return (BUF_GOOD);
        else
        {
        /*---------------------------------------------------------------
            If the extend failed wait for buffering to stop. If there
            is an error on a host block, buffering should be stopped
        ---------------------------------------------------------------*/
        while ( (BD_BUFFERING() == ON) && (bdClusterInterruptTimer.fTIMEOUT == FALSE) )
            ExitProcess();

        if(bdClusterInterruptTimer.fTIMEOUT == TRUE)
            StopDecoder();   
         
         
         
         return (BUF_GOOD);
         }
        }
}

DiscAccessStatusE bd_miss()
{
  DiscAccessStatusE DiscReadStatus;   /* Status of disc read */
    send_msg5S(SEND_MRW_MSG,"*CacheMiss*");


    /*---------------------------------------------------------------
        Insure read ahead buffering is enabled
    ---------------------------------------------------------------*/
    bdDecFlag.fDISABLE_READ_AHEAD = FALSE;


  AacsStatus.AacsFlag.AgidInUse = FALSE;

    /*---------------------------------------------------------------
        Initialize the host buffer
    ---------------------------------------------------------------*/
    bdInitializeHostBuffer(HostXfer.StartBlock.lba);

    /*---------------------------------------------------------------
        Set host transfer state to waiting
    ----------------------------------------------------------------*/
    HostXfer.State = WAITING_FOR_TRANSFER;

    /*---------------------------------------------------------------
        Start loading the buffer
    ----------------------------------------------------------------*/
    send_msg54(SEND_MRW_MSG,'I','N','I','T');
    DiscReadStatus = bdLoadBuffer( HostXfer, INIT_READ);

    return (DiscReadStatus);	
}
#endif // BURN_IN
