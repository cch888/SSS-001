/****************************************************************************
*                (c) Copyright 2000 - 2008 SUNEXT DESIGN, INC.
*                           All Right Reserved
*
*
*   FILE NAME       dvDiscRd.c
*
*   DESCRIPTION     Main DVD read engine to set and control the DVD DSP to read
*                   the disc.
*
*   $Revision: 198 $
*   $Date: 11/04/07 11:56a $
*
**************************** SUNEXT CONFIDENTIAL *******************************/

#include ".\common\globtype.h"
#include ".\common\globals.h"
#include ".\common\system.h"
#include ".\common\chip.h"

#include ".\hif\gp_ram.h"

#include ".\dvd\dvGvars.h"
#include ".\dvd\dvDiscinit.h"
#include ".\dvd\dvMMC.h"
#include ".\dvd\dvMacros.h"
#include ".\dvd\dvBufMgr.h"
#include ".\dvd\dvisr.h"
#include ".\dvd\dvseek.h"
#include ".\dvd\dvinit.h"

#if (BypassDecoderEr == 1)
#include ".\dvd\dvutil.h"
#endif

#if (OPEN_SESSION == 1)
#include ".\servo\svoVar.h"
#endif

#if (CALIBRATION_MANAGEMENT == 1)
#include ".\servo\svo_com.h"
#include ".\servo\svo_calib.h"
#endif

#if (DVD_RAM_READ == 1)
#include ".\defmgmt\dvdram\dvDefMgr.h"
#include ".\defmgmt\common\mrw_ram.h"
#include ".\al\hal_dfe.h"
#endif

#include ".\al\reg_intctrl.h"
#include ".\al\reg_dvd_decoder.h"

#if (ENABLE_HDDVD)
#include ".\hd\hdinit.h"
#endif

#if (SEEK_ADJ == 1)
extern LONG last_PBA;
#endif

/*******************************************************************
    Local PROTOTYPES
*******************************************************************/
DiscAccessStatusE dvdTransferDataToBuffer(void);
DiscAccessStatusE GetDvdError(void);

BYTE dvdExtendBuffering(void);
#if (BypassDecoderEr == 1)
BYTE dvdValidBypassDvDecoderEr(void);
#endif

/*******************************************************************************
    FUNCTION DEFINITIONS
*******************************************************************************/
/*******************************************************************************

    FUNCTION        dvdStartDiscTransfer

    DESCRIPTION     Main interface to the disc transferring engine. Decodes
                    the type of transfer and initiates disc access

    GLOBAL INPUTS   dvdDiscXferParameter

    RETURNS         Status of the disc access during buffering

********************************************************************************/
DiscAccessStatusE dvdStartDiscTransfer(void)
{
    DiscAccessStatusE    DiscReadStatus;    /* Return status of disc access */

    /*---------------------------------------------------------------
        Initialize local variables
    ----------------------------------------------------------------*/
    DiscReadStatus = BUF_GOOD;

    /*---------------------------------------------------------------
        Return fail for a transfer request of 0 blocks
    ----------------------------------------------------------------*/
    if (dvdDiscRequest.EccBlockCount == 0L)
        return(GEN_FAIL);

    /*---------------------------------------------------------------
        Determine the transfer type
    ----------------------------------------------------------------*/
    switch (dvdDiscRequest.Type)
    {
        case BCA_READ:
            dvdDecFlag.fREAD_BCA = SET;

        case INIT_READ:
        case INIT_READ_AHEAD:

            if (DVD_BUFFERING() == ON)
            {
                /*---------------------------------------------------------------
                    The DVD-DSP is not stopped and a block could potentially
                    come in before the INIT_READ is processed. Stop the DVD-DSP.
                ----------------------------------------------------------------*/
                dvdStopDiscTransfer();
            }

            /*---------------------------------------------------------------
               Initialize the transfer parameters and then begin transferring
            ----------------------------------------------------------------*/
            dvdInitializeDiscTransfer();
            DiscReadStatus = dvdTransferDataToBuffer();
            return DiscReadStatus;
            break;

        case EXTEND_READ:
        case EXTEND_READ_AHEAD:

            if ( dvdExtendBuffering() == TRUE )
                DiscReadStatus = dvdTransferDataToBuffer();
            else
                DiscReadStatus = EXTEND_READ_FAIL;

            return DiscReadStatus;
            break;

        case WRITE:
            /*---------------------------------------------------------------
                Write code
            ----------------------------------------------------------------*/
            break;

        default:
            break;

    }

    return(GEN_FAIL);
}
/*******************************************************************************

    FUNCTION        dvdInitializeDiscTransfer

    DESCRIPTION     Initializes the DVD disc transfer timer, flags and transfer
                    structure

    GLOBAL INPUTS   dvdDiscXferParameter

********************************************************************************/
void dvdInitializeDiscTransfer()
{

        STOP_DVD_BLOCK_TIMER();
        dvdResetDiscFlags();

        /*---------------------------------------------------------------
            Initialize buffer to track last incoming PSN
        ----------------------------------------------------------------*/
        dvdLastDecodedPSN = dvdDiscRequest.TargetID.Long - 1L;
#if (DVD_RAM_READ == 1)
        dvdramLastDecodedLSN = dvdramDiscRequestStartLSN - 1L;
#endif
        /*---------------------------------------------------------------
            Intialize request state
        ----------------------------------------------------------------*/
        dvdDiscRequest.EccBlocksBuffered  = 0L;
        dvdDiscRequest.State              = WAITING_FOR_TRANSFER;
        dvdDecFlag.fEXTEND_READ = FALSE;
}
/*******************************************************************************

    FUNCTION        dvdStopDiscTransfer

    DESCRIPTION     Stops the current transfer of data from disc to buffer

********************************************************************************/
void dvdStopDiscTransfer()
{

    BYTE InterruptState;
	USHORT i;
    /*---------------------------------------------------------------
        Disable global interrupts
    ----------------------------------------------------------------*/
    InterruptState = GET_INT_ENABLE_MASK();
    DISABLE_INTERRUPT();

    /*---------------------------------------------------------------
        Clear DVD buffering controls and interrupts
    ----------------------------------------------------------------*/
    //DVD issue WR.
    WRITE_FIELD(HAL_DVDD_ENBUF,0x00);
    if (DVD_BUFFERING() == ON)
    {    
        for (i=0;i< 5000; i++);
    }
    //DVD issue WR.
    
    DISABLE_DVD_BUFFERING();        /* Clear the buffering start bit    */
    CLEAR_DVD_BLOCKS_TO_BUFFER();   /* Clear the DVD block count        */
    DISABLE_DVD_DECODER_INTS();       /* Disable all DVD interrupts       */
    STOP_DVD_BLOCK_TIMER();         /* Stop the block interrupt timer   */
    DISABLE_DVD_SYNC_LOST_INT();    /* Disable sync lost interrupts     */
    #if (ENABLE_RAM_DFE_ICE_DETECT == 1)
    HAL_DFE_DisableICEDetect();
    HAL_DFE_ClusterStateDisableInt();
    RWDisc.dvRAM.ContinuteIceCount = 0;
    #endif
    dvdDecFlag.fSYNC_INTERRUPT_ENABLED = FALSE;
    dvdDecFlag.fBUFI_ENABLED = FALSE;

    /*---------------------------------------------------------------
        Change disc transfer state if not complete
    ----------------------------------------------------------------*/
    if(dvdDiscRequest.State != TRANSFER_COMPLETE)
        dvdDiscRequest.State = TRANSFER_STOPPED;

    /*---------------------------------------------------------------
        If read ahead process was on insure that flag is set
        to indicate it is now off
    ----------------------------------------------------------------*/
    dvdDecFlag.fREAD_AHEAD = FALSE;

    /*---------------------------------------------------------------
       Enable global interrupts
    ----------------------------------------------------------------*/
    SET_INT_ENABLE_MASK(InterruptState);
}
/*******************************************************************************

    FUNCTION        dvdResetDiscFlags

    DESCRIPTION     Clears all neccessary dvd error, interrupt, and read flags
                    prior to buffering

********************************************************************************/
void dvdResetDiscFlags()
{

    /*---------------------------------------------------------------
        Clear DVD error flags
    ----------------------------------------------------------------*/
    dvdErrorFlag.fSEQ_ID_ERROR      = CLEAR;
    dvdErrorFlag.fUNCOR_ECC         = CLEAR;
    dvdErrorFlag.fBCA_ERROR         = CLEAR;
    dvdErrorFlag.fEDC_ERROR         = CLEAR;
    dvdErrorFlag.fID_ERROR          = CLEAR;
    dvdErrorFlag.fSEEK_FAIL         = CLEAR;
    dvdErrorFlag.fSYNC_LOST         = CLEAR;
    dvdErrorFlag.fTID_MISS          = CLEAR;
    dvdErrorFlag.fUNAUTH_READ_ERROR = CLEAR;
    dvdErrorFlag.fBANDWIDTH_ERROR   = CLEAR;
    dvdErrorFlag.fID_MISMATCH       = CLEAR;

    /*---------------------------------------------------------------
        Clear DVD block interrupt flags
    ----------------------------------------------------------------*/
    dvdDecIntFlag.fECC_DONE_INT    = CLEAR;
    dvdDecIntFlag.fECC_BLK_INT     = CLEAR;
#if (ENABLE_HDDVD == 1)
    dvdDecIntFlag.fCLUSTER_BLK_INT = CLEAR;
#endif
    /*---------------------------------------------------------------
        Clear DVD timeout flag
    ----------------------------------------------------------------*/
    dvdBlockInterruptTimer.fTIMEOUT = CLEAR;
}

/*******************************************************************************

    FUNCTION        dvdTransferDataToBuffer

    DESCRIPTION     Starts the transferring of data from disc to buffer

    RETURNS         Status of the disc access during buffering

    NOTE            CURBUF is initialized in ClearDataBuffer().
********************************************************************************/
#if (READ_MODIFY_WRITE == 1)
extern BYTE gReadModifyWrite;
#endif
DiscAccessStatusE dvdTransferDataToBuffer()
{

    ULONG               StartPSN;
    ULONG               RemainingBlocks;
    ULONG               tmpPSN;
    ULONG               clock;
    BYTE                InterruptState;
    DiscAccessStatusE   DiscReadStatus;
    BYTE                retrycnt;
    BYTE                DelaySyncLostInt;
    BYTE                DVDDecoder_ID;
    BYTE                length;
    BYTE                buffer_error_cnt = 0;
#if (READ_MODIFY_WRITE == 1)
    ULONG RdMdfWrt_timer;
#endif
//    ULONG WaitBuf_timer;
#if (OPEN_SESSION == 1)
    BYTE pre_rf_search_count,temp_rf_search_count;
#endif
    ULONG               HostEndLBA;         /* Ending LBA of host transfer request */
    /*--------------------------------------------------------------
        initial local variables
    ---------------------------------------------------------------*/
    HostEndLBA = (HostXfer.StartBlock.lba + HostXfer.BlockCount.Long) - 1L;
    retrycnt = 0;
    DelaySyncLostInt   = FALSE;
#if (CEProduct == 1)
    length = 0x20;
#else
    length = 0x10;
#endif

    /*---------------------------------------------------------------
        Initialize retry count. If this is for a lead in block
        request there is no need for retries.
    ----------------------------------------------------------------*/

    if ( (dvdDecFlag.fREADING_LEADIN == TRUE) || (dvdDecFlag.fREAD_AHEAD == TRUE) )
    {
#if (BypassDecoderEr == 1)
    #if (OPEN_SESSION == 1)  
        if((dvdDecFlag.fREADING_SESSION_POSITION == READ_INTRO)||(dvdDecFlag.fREADING_SESSION_POSITION == READ_BORDER_IN))
        {
            dvdReadRetryCount = 1;
        }
        else
    #endif   
        {
            dvdReadRetryCount   = MAX_DVD_READ_RETRIES_FOR_CE;
        }
#else
        dvdReadRetryCount   = 0;
#endif
//if read ahead, set svoIramVar.knlTimeupCount = 50 (1 second).because this action is not in real ATAPI Read command.
//the svoIramVar.knlTimeupCount will be set to read timeout after CheckATAPICommand().
        if(dvdDecFlag.fREAD_AHEAD == TRUE)
            svoIramVar.knlTimeupCount = 50;// 1 sec
    }
    else
    {
#if (CEProduct == 1)
        dvdReadRetryCount   = MAX_DVD_READ_RETRIES_FOR_CE;
#else // IT Product
        dvdReadRetryCount   = MAX_DVD_READ_RETRIES;
#endif
    }

#if (E_DIAGS == 1)
    if(READ_PI_PO == 1)
        dvdReadRetryCount = 5;
#endif

#if (OPEN_SESSION == 1)
    pre_rf_search_count = RF_search_count;
    temp_rf_search_count = 0;
#endif

    ENABLE_DVD_DECODER();                   /* Turn on DVD decoder */

    WRITE_FIELD(HAL_DVDD_TIDDIS, 0);        /* enable target id */
    /*---------------------------------------------------------------
        Main DVD buffering loop. Continues until all requested blocks
        are buffered except for the following conditions:

            1. All host blocks are in the buffer and read ahead
               blocks are now being buffered

            2. A DVD error has occured and retry count is zero
    ----------------------------------------------------------------*/
    while( dvdDiscRequest.State != TRANSFER_COMPLETE )
    {

        if (!svoIramVar.knlTimeupCount)
        {  /* time out */
#if (BypassDecoderEr == 1)
                if(dvdValidBypassDvDecoderEr() == TRUE)
                {
                    IGNORE_DECODER_ERROR = TRUE;
                }
                else
#endif
                {
                    dvdStopDiscTransfer();                  /* stop disc transferring       */
                    dvdResetDiscFlags();
                    CLEAR_DVD_DECODER_STATUS();                 /* clear all remaining status   */
                    Svr_pause_reqire = TRUE;                /* Set pause request to servo   */
                    Iram_Svr_player_mode = PAUSING_MODE;    /* Set pause mode for player    */
                    DiscReadStatus = TIMEOUT;
                    return(DiscReadStatus);                 /* Return the disc read error   */
                }
        }
        /*---------------------------------------------------------------
             If this is an extended read do not set up for buffering and
             go directly to monitor the current buffering process
        ----------------------------------------------------------------*/
        if (dvdDecFlag.fEXTEND_READ == FALSE)
        {

            /*---------------------------------------------------------------
                Stop buffering
            ----------------------------------------------------------------*/
            if ( DVD_BUFFERING() == ON )
                dvdStopDiscTransfer();

            if(EjectKeyInfo.PressedValid || TrayManagerInfo.Executing)
                return( EJECT_PRESS );

#if (OPEN_SESSION == 1)
            /*---------------------------------------------------------------
                Monitor search times of searching RF zone
            ----------------------------------------------------------------*/
            if(pre_rf_search_count != RF_search_count)
            {
                temp_rf_search_count++;
            }
#endif
            /*---------------------------------------------------------------
                  Check for errors
            ----------------------------------------------------------------*/
            if ( DVD_BLK_ERROR || DVD_BUFFER_ERROR )
            {
                DiscReadStatus = GetDvdError();

                if (((dvdReadRetryCount == 0) &&  (!((Iram_Cmd_block.bf[0] == SBC_READ_10) && (St_cmd_inproc == TRUE))))
                      || (DiscReadStatus == UNAUTH_RD_FAIL))
                {
                    /*---------------------------------------------------------------
                          It isn't executing Read10 command and maximum number of retries have
                          performed or unauthorized read (no retries necessary) attempted.
                    ----------------------------------------------------------------*/
    #if (BypassDecoderEr == 1)
                    if(dvdValidBypassDvDecoderEr() == TRUE)
                    {
                        IGNORE_DECODER_ERROR = TRUE;
                    }
                    else
    #endif
                    {                    
                         dvdStopDiscTransfer();                  /* stop disc transferring       */
                         CLEAR_DVD_DECODER_STATUS();             /* clear all remaining status   */
                         Svr_pause_reqire = TRUE;                /* Set pause request to servo   */
                         Iram_Svr_player_mode = PAUSING_MODE;    /* Set pause mode for player    */
    
                         return(DiscReadStatus);                 /* Return the disc read error   */
                    }
                }
#if (CALIBRATION_MANAGEMENT == 1)
    #if (OPEN_SESSION == 1)
                if (((retrycnt >= 1)&&(retrycnt <= 4)) && !DVDRAM && calGetCalRequired(dvdDiscRequest.TargetLayer & LAYERMASK, Iram_stp_CurrentPosition, ITEM_ALL, READ_MODE)                 //20093019_n.25==add ServoReCalibrate times during retry.
                    &&(!((temp_rf_search_count > 0)&&(svoVar_TrackingMethod == TRK_METHOD_DPD))))
    #else
                if (((retrycnt >= 1)&&(retrycnt <= 4)) && !DVDRAM && calGetCalRequired(dvdDiscRequest.TargetLayer & LAYERMASK, Iram_stp_CurrentPosition, ITEM_ALL, READ_MODE))                 //20093019_n.25==add ServoReCalibrate times during retry.
    #endif
                {
                    (void)ServoReCalibrate();
                }
#endif /* CALIBRATION_MANAGEMENT */
#if (HORROR_DISC_IMPROVEMENT == 1)
    #if (OPEN_SESSION == 1)
                 if((retrycnt >= 2)&&(!((temp_rf_search_count > 2)&&(svoVar_TrackingMethod == TRK_METHOD_DPD))))
    #else
                 if(retrycnt >= 2)
    #endif
                 {                
#if(EN_ADEQ)
                    read_retry_option|=(READ_RETRY_ENABLE|READ_RETRY_TIMER_ENABLE|SERVO_DEFECT_RETRY_ENABLE
                                                  |HF_DEFECT_RETRY_ENABLE|DFE_RETRY_ENABLE|ADEQ_RETRY_ENABLE
                                                  |SLOW_PLL_RETRY_FORCE_LOCK_ENABLE);// ==add change ADEQ setting when read retry
#else

                    read_retry_option|=(READ_RETRY_ENABLE|READ_RETRY_TIMER_ENABLE|SERVO_DEFECT_RETRY_ENABLE
                                                  |HF_DEFECT_RETRY_ENABLE|DFE_RETRY_ENABLE
                                                  |SLOW_PLL_RETRY_FORCE_LOCK_ENABLE);
#endif
                    (void)ServoChgRFParameter();
                 }
#endif
                if(dvdReadRetryCount)
                    --dvdReadRetryCount;

                if(retrycnt<6)
                retrycnt ++;
            }

#if (BypassDecoderEr == 1)
            IGNORE_DECODER_ERROR = FALSE;

            if(dvdValidBypassDvDecoderEr() == TRUE)
            {

                if(RetryLimit>1) //if Ignore detect during playback
                {
                    if((ReadTimer(&RetryChgTime)>60000))
                    {
                        if(ReadGoodCnt>5)
                        {
                            #if (HORROR_DISC_IMPROVEMENT == 1)           
                            send_msg5S(1, "READ GOOD");
                            read_retry_option&=READ_RETRY_DISABLE;                  
                            (void)ServoChgRFParameter();
                            #endif//#if (HORROR_DISC_IMPROVEMENT == 1)

                            RetryLimit=1;  // retry 5 times
                            ReadGoodCnt=0;
                        }
                    }
                }
                if((DataBuffer.SectorCount < 0x20) && (dvdReadRetryCount <= RetryLimit))    
                {
                    IGNORE_DECODER_ERROR = TRUE;
                    send_msg5SValue(1, "Bypass 1",4,dvdReadRetryCount);
//                        if(RetryLimit < 3)
//                        {
//                            RetryLimit++;
//                        }
                    StartTimer(&RetryChgTime);  
                }

                
                send_msg54(1,'R','T',RetryLimit,dvdReadRetryCount);
                //send_msg5SValue(1, "Rlimit", 1, RetryLimit);
                //send_msg5SValue(1, "Rcount", 1, dvdReadRetryCount);
                //send_msg5SValue(1, "Rtime", 4, ReadTimer(&RetryChgTime));

                if(dvdReadRetryCount == 1)
                {
                    IGNORE_DECODER_ERROR = TRUE;
                    send_msg5S(1, "Bypassx2");
                }
            }
#endif
            /*---------------------------------------------------------------
                Disable global interrupts
            ----------------------------------------------------------------*/
            InterruptState = GET_INT_ENABLE_MASK();
            DISABLE_INTERRUPT();

            /*--------------------------------------------------------
                Set/Reload the start address for the seek and remaining
                blocks to transfer.

                NOTE: For an initial read dvdLastDecodedPSN was set in
                function dvdInitializeDiscTransfer(), where
                dvdLastDecodedPSN = dvdDiscRequest.TargetID - 1L;
            ---------------------------------------------------------*/
            StartPSN = dvdLastDecodedPSN + 1L;
            RemainingBlocks = dvdDiscRequest.EccBlockCount - dvdDiscRequest.EccBlocksBuffered;

            /*--------------------------------------------------------
                Set the current disc buffer pointer
            ---------------------------------------------------------*/
            SET_DISC_BUFFER_PTR(DataBuffer.CurDiscPtr);

            /*---------------------------------------------------------------
                Enable global interrupts
            ----------------------------------------------------------------*/
            SET_INT_ENABLE_MASK(InterruptState);

            /*---------------------------------------------------------------
                Load Target ID and set target block flag
            ----------------------------------------------------------------*/
            SET_TARGET_ID(StartPSN);

            //first time, there is no interrupt yet.
            dvdDecIntFlag.fNO_INT = TRUE;

            /*---------------------------------------------------------------
                Set the blocks to transfer count register. Insure that the
                block count does not exceed the maximum setting possible for
                BKTOBUF.
            ----------------------------------------------------------------*/
            if (RemainingBlocks > MAX_VALUE_OF_BKTOBUF)
                RemainingBlocks = MAX_VALUE_OF_BKTOBUF;

#if (ENABLE_HDDVD)
            if(pdbDiscLoaded() == HD_DISC)
            {
                if(RemainingBlocks & 0x01)
                    RemainingBlocks++; /*HD DISC must set to EVEN value*/
            }
#endif
            SET_DVD_BLOCKS_TO_BUFFER((WORD)RemainingBlocks);

            /*---------------------------------------------------------------
                Clear all DVD flags and status and set transfer state
            ----------------------------------------------------------------*/
            dvdResetDiscFlags();
            CLEAR_DVD_DECODER_STATUS();

            /*---------------------------------------------------------------
                Start DVD timer and enable DVD read interrupts
            ----------------------------------------------------------------*/

            START_DVD_BLOCK_TIMER(FIRST_BLOCK_TIMEOUT_VALUE);
            ENABLE_DVD_DECODER_INTS();

            if (dvdDecFlag.fREAD_LONG_COMMAND == TRUE)
            {
                dvdDecFlag.fBUFI_ENABLED = TRUE;
                ENABLE_BUFI_INT();
            }

#if (DVD_AWG_TEST == TRUE)
            /*---------------------------------------------------------------
                Call Seek
            ----------------------------------------------------------------*/
                // DVD Test without Servo and disc using AWG pattern
                dvdErrorFlag.fSEEK_FAIL = FALSE;
                DiscReadStatus       = BUF_GOOD;
                dvdDiscRequest.State = TRANSFERRING;
                ENABLE_DVD_BUFFERING();
#else

            /*-------------------------------------------------------------------
                During play dvd mode, if seek error or buffer error,
                adding the following length to avoid sync error.
            ---------------------------------------------------------------------*/
    #if (DVD_RAM_READ == 1)
            if(pdbGetDiscKind() != eDVDRAMDisc)
    #endif
            {
                if((StartPSN >= 0x30000) && (pdbGetDiscStatus() == eFinalizedDisc))
                {
                    length = 0x20;

                    //if(dvdDecFlag.fREAD_AHEAD == TRUE)
                    //    StartPSN -= 0x20L;
                    //else
                    if(dvdBlockInterruptTimer.fTIMEOUT!=TRUE)
                    {
#if (OPEN_SESSION==1)                     
                        if((temp_rf_search_count != 0) && (StartPSN == pdbGetDVDStartPSN(1)) && (svoVar_TrackingMethod == TRK_METHOD_DPD))
                        {
                            send_msg80(SEND_READ_DVD_MSG,0x71013E);
                        }
                        else
#endif                            
                        {
                            StartPSN -= (retrycnt * 0x20L);
                        }
                    }
                }
            }

            ReadyLED(LED_ON);

#if (OPEN_SESSION == 1)
            pre_rf_search_count = RF_search_count;
#endif
            if( (dvdSeek(StartPSN ,dvdDiscRequest.TargetLayer)) == Error)
            {
                //send_msg5S(SEND_READ_DVD_MSG, "SEEK NG!");
                /*---------------------------------------------------------------
                    Seek fail. Adjust speed/error count
                ----------------------------------------------------------------*/
    #if (E_DIAGS == 1)
                if(READ_PI_PO == 0)
    #endif
                {   
#if (OPEN_SESSION == 0)
                SpeedCount = SPEED_DOWN_FOR_SEEK_FAIL;
#endif
                }
                dvdErrorFlag.fSEEK_FAIL = TRUE;
                DiscReadStatus = SEEK_FAIL;
                dvdDecFlag.fDISABLE_READ_AHEAD = TRUE;
            }
            else
            {
                /*---------------------------------------------------------------
                    Seek successful
                ----------------------------------------------------------------*/
                //send_msg5S(SEND_READ_DVD_MSG, "SEEK OK!");
                send_msg5SValue(SEND_READ_DVD_MSG, "Adr",4,svoIramVar.currentID.SectorNum.all32);
                ENABLE_TARGET_MISS_INT();
                DiscReadStatus       = BUF_GOOD;
                dvdDiscRequest.State = TRANSFERRING;
                CLEAR_SYNC_LOST_STATUS();

                if(pdbDiscLoaded() == DVD_DISC)
                {
                    if ((pdbGetDiscKind()  == eDVDROMDisc) || (pdbGetDiscKind()  ==  eDVDROMDLDisc) || (pdbGetDiscKind()  ==  eDVDRAMDisc))
                    {
                        //dvdDecFlag.fSYNC_INTERRUPT_ENABLED = SET;
                        //ENABLE_DVD_SYNC_LOST_INT();
                    }
                    else
                    {
                        DelaySyncLostInt = TRUE;

                        StartTimer(&clock);
                        tmpPSN = dvdGetCurrentPSN(&DVDDecoder_ID);
                        while (tmpPSN < (StartPSN - length) || tmpPSN == 0xFFFFFFFFL)   // Wait for Read Target Address - 1
                        {
                            if (ReadTimer(&clock) > 150L)
                            {
                                dvdBlockInterruptTimer.fTIMEOUT = TRUE;
                                break;
                            }

                            ExitProcess();

                            tmpPSN = dvdGetCurrentPSN(&DVDDecoder_ID);
                        }

                        CLEAR_SYNC_LOST_STATUS();
                        // Now check if Wide Band PLL is locked up
                    }
                }
    #if(ENABLE_HDDVD)
                else
                {
                    send_msg5S(SEND_READ_DVD_MSG, "HD_TRANSFER");
                }
    #endif

                if (dvdDecFlag.fREAD_LONG_COMMAND == TRUE)
                    ENABLE_RAWDATA_BUFFERING();
                else
                {
                    ENABLE_DVD_BUFFERING();

    #if(ENABLE_HDDVD)
                    if(pdbDiscLoaded() == HD_DISC)  //HD DISC
                    {
                        START_DVD_BLOCK_TIMER(HD_BLOCK_TIMEOUT_VALUE);
                    }
                    else
    #endif
                    { //DVD DISC
                        if (pdbGetDiscKind()  ==  eDVDRAMDisc)
                        {
                            START_DVD_BLOCK_TIMER(DVDRAM_BLOCK_TIMEOUT_VALUE);
                        }
                        else
                        {
                            START_DVD_BLOCK_TIMER(DVD_BLOCK_TIMEOUT_VALUE);
                        }
                    }
                }
            }

#endif //DVD_AWG_TEST
        } /* if (dvdDecFlag.fEXTEND_READ) */
        else
            DiscReadStatus = BUF_GOOD;  // For extend always set disc status to good

        /*---------------------------------------------------------------
            Extend read flag no longer needed. Clear the flag to allow
            error procesing of the extended read process
        ----------------------------------------------------------------*/
        dvdDecFlag.fEXTEND_READ = CLEAR;

        /*---------------------------------------------------------------
            For pre-fetching there is no need to monitor the buffering
        ----------------------------------------------------------------*/
        if (dvdDecFlag.fREAD_AHEAD == TRUE)
            return (DiscReadStatus);


        /*-------------------------------------------------------------------
            Monitor DVD Buffering
        --------------------------------------------------------------------*/
#if (READ_MODIFY_WRITE == 1)
        StartTimer(&RdMdfWrt_timer);
#endif

        while(1)
        {
#if (READ_MODIFY_WRITE == 1)
            if((gReadModifyWrite)&&(ReadTimer(&RdMdfWrt_timer) > TimeOutValue))
            {
                dvdBlockInterruptTimer.fTIMEOUT = TRUE;
                break;
            }
#endif

            if (!svoIramVar.knlTimeupCount)
            {  /* time out */
#if (BypassDecoderEr == 1)
                
                if(dvdValidBypassDvDecoderEr() == TRUE)
                {
                    IGNORE_DECODER_ERROR = TRUE;
                }
                else
#endif
                {
                    dvdStopDiscTransfer();                  /* stop disc transferring       */
                    dvdResetDiscFlags();
                    CLEAR_DVD_DECODER_STATUS();                 /* clear all remaining status   */
                    Svr_pause_reqire = TRUE;                /* Set pause request to servo   */
                    Iram_Svr_player_mode = PAUSING_MODE;    /* Set pause mode for player    */
                    DiscReadStatus = TIMEOUT;
                    return(DiscReadStatus);                 /* Return the disc read error   */
                }
            }

            if (DelaySyncLostInt == TRUE)
            {
// DACQI was removed.
                /*---------------------------------------------------------------
                    Wait until Demodualtor Sync Lock before setting the sync lost interrupt
                    in case seek has landed in blank area prior to target ID
                ----------------------------------------------------------------*/
                if (READ_FIELD(HAL_DMOD_DMODILKS))          // Demodulator Sync Lock
                {
                    DelaySyncLostInt = FALSE;
                    CLEAR_SYNC_LOST_STATUS();
                    //dvdDecFlag.fSYNC_INTERRUPT_ENABLED = SET;
                    //ENABLE_DVD_SYNC_LOST_INT();
                }
            }

            /*---------------------------------------------------------------
                Check for Servo Skating
            ----------------------------------------------------------------*/
#if (HIF_DETECT_SKATING== 1)
            if(svoIramVar.focusDropFlag == FALSE)// if focus drop, there is no ID.
            {
                ULONG readtimer;
                ULONG CurrentPSN;
                StLongU   currentID_temp;
                DVDDecoder_ID = FALSE;
                if (READ_FIELD(HAL_DVDD_CIDI))
                {
                    if (READ_FIELD(HAL_DVDD_CIDVLD))
                    {
                        currentID_temp.Long = READ_FIELD(HAL_DVDD_CURID)&0x00FFFFFF;	// TBD? There are masks defined for this!
                        CLEAR_INT_FIELD(HAL_DVDD_CIDI, 0);
                        CurrentPSN = currentID_temp.Long;
                        DVDDecoder_ID = TRUE;
                    }
                    CLEAR_INT_FIELD(HAL_DVDD_CIDI, 0);
                }
                //tmpPSN = dvdGetCurrentPSN(&DVDDecoder_ID);
                if(DVDDecoder_ID)
                {
                    ULONG tempPSN;
                    tempPSN = (dvdLastDecodedPSN + 1L+ONE_DVD_BLOCK+2*ONE_DVD_BLOCK) - CurrentPSN;//decode end address+0x20 - current addrss.
                    readtimer = svoSectors2Duration(tempPSN);             //calculate spend time.
                    if((dvdBlockInterruptTimer.count*20)<readtimer)
                    {
                        SendMsgCn(SEND_READ_DVD_MSG,6,0x800509,A4B(CurrentPSN),A2B(dvdBlockInterruptTimer.count));//20090406_r.27 == Fixed buffer time out message length.
                        dvdErrorFlag.fTID_MISS = TRUE; //20090327_q.26== Smart play hangup
                        RecoverDecoderError(); //20090406_r.27== Decrease speedcnt when buffer time out occur.
#if (HORROR_DISC_IMPROVEMENT == 1)                                
                        BufferErrorForRetry |= TRACK_PULL_BACK;
#endif                        
                    }
                }
            }
#endif
            /*---------------------------------------------------------------
                Decoder overshoot: after seek, decoder is turned on, if there
                is no interrupt and the current ID is greater than target ID, it
                means decoder overshoot, should reset decoder and restart.
            ---------------------------------------------------------------*/
            if ((dvdDecIntFlag.fNO_INT == TRUE) && (pdbGetDiscKind()  !=  eDVDRAMDisc))
            {
                if(pdbDiscLoaded() == DVD_DISC)
                {
                    tmpPSN = dvdGetCurrentPSN(&DVDDecoder_ID);
                    if (DVDDecoder_ID == TRUE)
                    {
                        if (tmpPSN > dvdLastDecodedPSN + 1L + 0x30L)
                        {
                            dvdBlockInterruptTimer.fTIMEOUT = TRUE;
                            SendMsg80(SEND_HIF_CMD,0x80050C);
                        }
                        else
                        {
                            ULONG tempPSN;
                            ULONG readtimer;
                            tempPSN = (dvdLastDecodedPSN + 1L+ONE_DVD_BLOCK+2*ONE_DVD_BLOCK) - tmpPSN;//decode end address+0x20 - current addrss.
                            readtimer = svoSectors2Duration(tempPSN);             //calculate spend time.
                         
                            if((dvdBlockInterruptTimer.count*20)<readtimer)
                            {
                                SendMsgCn(SEND_READ_DVD_MSG,6,0x800509,A4B(tmpPSN),A2B(dvdBlockInterruptTimer.count));
                                dvdErrorFlag.fTID_MISS = TRUE; 
                                RecoverDecoderError(); 
#if (HORROR_DISC_IMPROVEMENT == 1)                                
                                BufferErrorForRetry |= TRACK_PULL_BACK;
#endif
                            }
                        }                        
                    }
                }
#if(ENABLE_HDDVD)
                else //HD
                {
                    tmpPSN = dvdGetCurrentPSN(&DVDDecoder_ID);
                    if ((DVDDecoder_ID == TRUE)&&(tmpPSN > dvdLastDecodedPSN + 1L + 0x30L))
                    {
                        dvdBlockInterruptTimer.fTIMEOUT = TRUE;
                        send_msg5S(SEND_READ_DVD_MSG, "HD_overshoot");
                    }
                }
#endif
#if (SEEK_ADJ == 1)
                if (last_PBA != 0x7FFFFFFF)
                {
                    // (SEEK_ADJ2:RPM,Pos,parfine,diff_PSN)%5d  %5d %5d %5d
                    SendMsgCnB(SHOW_DEBUG_MSG,8,0x33FF58,B2B(spdGetCurrentRPM()),B2B(Iram_stp_CurrentPosition),B2B(svoSeekVar.parFine),B2B(tmpPSN - last_PBA));
                    last_PBA = 0x7FFFFFFF;
                }
#endif
            }

            /*---------------------------------------------------------------
                Check for errors 
            ----------------------------------------------------------------*/
            if (DVD_BUFFER_ERROR||DVD_BLK_ERROR)
            {
                // Host request block tansfer done !! Drive should return Buf GOOD immediate, and stop prebuffer.
                if((HostXfer.State == TRANSFER_COMPLETE)&&((Iram_Cmd_block.bf[0] == SBC_READ_10)||(Iram_Cmd_block.bf[0] == SBC_READ_12)))//20091218_u.34==only ATAPI READ10 or READ12 command return buffer good under decoder error and host request data transfer complete. 
                {
#if (BypassDecoderEr == 1)
                    IGNORE_DECODER_ERROR = FALSE;
#endif
                    dvdStopDiscTransfer();                  /* stop disc transferring       */
                    dvdResetDiscFlags();
                    CLEAR_DVD_DECODER_STATUS();                 /* clear all remaining status   */
                    Svr_pause_reqire = TRUE;                /* Set pause request to servo   */
                    Iram_Svr_player_mode = PAUSING_MODE;    /* Set pause mode for player    */
                    return BUF_GOOD;
                }
#if (BypassDecoderEr == 1)
                if(IGNORE_DECODER_ERROR)
                {
                        //send_msg5SValue(1, "ValidBypass", 1, 0);
                        //send_msg5SValue(1, "dvdLastDecodedPSN", 4, bdLastDecodedPSN);
                        //send_msg5SValue(1, "RetryCount", 1, retrycnt);
                        {
                            WORD *D2D3Ptr;
                            BYTE idx;
                            D2D3Ptr = (WORD *)(SCSDRAM_ADDRESS + (ULONG)(DataBuffer.CurDiscPtr * 0x800) + 2);
                            for (idx = 0; idx < ONE_DVD_BLOCK; idx++)
                            {
                                *D2D3Ptr = 0x00;
                                D2D3Ptr += DVD_SECTOR_SIZE/2;
                            }

                            DataBuffer.CurDiscPtr += ONE_DVD_BLOCK;
                            DataBuffer.CurDiscPtr %= BUF_TOTAL_BLK_COUNT;
                                    
                            //force stop decoder when timeout detect.
                            if ((dvdBlockInterruptTimer.fTIMEOUT == TRUE)||(dvdErrorFlag.fTID_MISS == TRUE))
                                dvdStopDiscTransfer();
                                
                            /*---------------------------------------------------------------
                                Update count of clusters transferred, cluster count and
                                last PSN in buffer.
                            ---------------------------------------------------------------*/
                            ++dvdDiscRequest.EccBlocksBuffered;
                            DataBuffer.SectorCount  += ONE_DVD_BLOCK;
                            dvdLastDecodedPSN        += ONE_DVD_BLOCK;
                            dvdResetDiscFlags();

                            send_msg5SValue(SEND_READ_DVD_MSG, "passPSN", 4, dvdLastDecodedPSN);

                            if (DataBuffer.CacheEndLBA == 0L)
                                DataBuffer.CacheEndLBA = 0x0FL;
                            else
                                DataBuffer.CacheEndLBA += ONE_DVD_BLOCK;

                            DiscReadStatus = BUF_GOOD;
                            if (HostEndLBA <= DataBuffer.CacheEndLBA)
                            {
                                if( HostXfer.State == WAITING_FOR_TRANSFER)
                                {
                                    send_msg54(SEND_READ_DVD_MSG,'S','n','d','2');
                                    SendHostBlocks(); 
                                }
                                return (BUF_GOOD);
                            }
                            //message for debug.
                            //send_msg5SValue(1,"State",4,HostXfer.State); 
                            //send_msg5SValue(1,"SC",4,DataBuffer.SectorCount); 
                            //send_msg5SValue(1,"ECCBB",4,dvdDiscRequest.EccBlocksBuffered); 
                            //send_msg5SValue(1,"ECCBC",4,HostXfer.BlockCount.Long); 
                            if (dvdDiscRequest.EccBlocksBuffered >= dvdDiscRequest.EccBlockCount)    
                            {
                                send_msg5S(SEND_READ_DVD_MSG, "Bypass 2");
                                return (DiscReadStatus);
                            }
                            
                        }
                        if (dvdDecFlag.fREAD_AHEAD == TRUE)
                            return(DiscReadStatus);
                }
#endif                
            /*---------------------------------------------------------------
                Check for buffered block errors posted by DVD ISR
            ----------------------------------------------------------------*/

                if (DVD_BLK_ERROR)
                {
                    send_msg5S(SEND_READ_DVD_MSG,"BLKERROR");
                    break;
                }
                //20090401_d.27 == start ==Target search enhancement when target not found.
                if (dvdBlockInterruptTimer.fTIMEOUT == TRUE)
                {
#if (ENABLE_HDDVD)
                    if(pdbDiscLoaded() == HD_DISC)
                        HDDspSystemInit();
                    else
#endif
                    {
                        buffer_error_cnt++;
                        dvdDspSystemInit();
                    }

                    if(buffer_error_cnt > 3)
                    {
                        RecoverDecoderError();
                        buffer_error_cnt = 0;
                    }
                }
                break;
            }
            /*---------------------------------------------------------------
                If prefetching starts while buffering exit and send status
            ----------------------------------------------------------------*/
            if (dvdDecFlag.fREAD_AHEAD == TRUE)
                return(DiscReadStatus);

            /*---------------------------------------------------------------
                Check if all requested blocks have been buffered
            ----------------------------------------------------------------*/
            if(dvdDecIntFlag.fECC_DONE_INT)
            {
                //send_msg5S(SEND_READ_DVD_MSG, "ECCDONE");
                send_msg80(SEND_SStpTask_MSG,0x710126);
                break;
            }

#if (DVD_AWG_TEST == FALSE)
            /*---------------------------------------------------------------
                Check for eject is pressed.
            ----------------------------------------------------------------*/
            if(EjectKeyInfo.PressedValid || TrayManagerInfo.Executing)
                return( EJECT_PRESS );
#endif

#if (E_DIAGS == 1)
            /*---------------------------------------------------------------
                Conditional code. If PI/PO information has been obtained
                for a single ECC block return good status
            ----------------------------------------------------------------*/

            if( (READ_PI_PO == 1) && (WAIT_PI_PO == 1) )
            {
                //send_msg5S(SEND_READ_DVD_MSG, "BUFFOOG");
                send_msg80(SEND_SStpTask_MSG,0x710127);
                return( BUF_GOOD );
            }
#endif
            if(ATA_SRST_rcv == TRUE)//WarnBoot fail in Southbridge is ICH5 
            {
                dvdStopDiscTransfer();                  /* stop disc transferring       */
                dvdResetDiscFlags();
                CLEAR_DVD_DECODER_STATUS();                 /* clear all remaining status   */
                Svr_pause_reqire = TRUE;                /* Set pause request to servo   */
                Iram_Svr_player_mode = PAUSING_MODE;    /* Set pause mode for player    */
                DiscReadStatus = UNCOR_ECC_FAIL;
                return(DiscReadStatus);                 /* Return the disc read error   */
            }
            //ExitProcess();  //marked by elvis, 2009/01/20
        } /* End Monitor Buffering Loop */
    } /* END dvdDiscRequest.State != TRANSFER_COMPLETE */

        /*---------------------------------------------------------------
           All requested blocks have been bufferred. Stop the transfer
           and reset the disc flags
        ----------------------------------------------------------------*/
        dvdStopDiscTransfer();
        dvdResetDiscFlags();

        /*---------------------------------------------------------------
           Mark a servo pause request and return disc status
        ----------------------------------------------------------------*/
        Svr_pause_reqire = TRUE;
        return(DiscReadStatus);
}

/*******************************************************************************

    FUNCTION        GetDvdError

    DESCRIPTION     Returns the type of error encountered during buffering

    RETURNS         Status of the disc transfer error during buffering

********************************************************************************/
DiscAccessStatusE GetDvdError()
{

    if (dvdErrorFlag.fUNCOR_ECC)
    {
        dvdErrorFlag.fUNCOR_ECC = CLEAR;
        return(UNCOR_ECC_FAIL);
    }

    if (dvdErrorFlag.fEDC_ERROR)
    {
        dvdErrorFlag.fEDC_ERROR = CLEAR;
        return(EDC_FAIL);
    }

    if (dvdErrorFlag.fID_ERROR)
    {
        dvdErrorFlag.fID_ERROR = CLEAR;
        return(ID_FAIL);
    }

    if (dvdErrorFlag.fSEQ_ID_ERROR)
    {
        dvdErrorFlag.fSEQ_ID_ERROR = CLEAR;
        return(SEQID_FAIL);
    }

    if (dvdErrorFlag.fBCA_ERROR)
    {
        dvdErrorFlag.fBCA_ERROR = CLEAR;
        return(BCA_FAIL);
    }

    if (dvdErrorFlag.fSEEK_FAIL)
    {
        dvdErrorFlag.fSEEK_FAIL = CLEAR;
        return(SEEK_FAIL);
    }

    if (dvdErrorFlag.fSYNC_LOST)
    {
        dvdErrorFlag.fSYNC_LOST = CLEAR;
        return(SYNC_LOST_FAIL);
    }

    if (dvdErrorFlag.fTID_MISS)
    {
        dvdErrorFlag.fTID_MISS = CLEAR;
        return(TID_FAIL);
    }

    if (dvdErrorFlag.fUNAUTH_READ_ERROR)
    {
        dvdErrorFlag.fUNAUTH_READ_ERROR = CLEAR;
        return(UNAUTH_RD_FAIL);
    }

#if (DVD_VERIFY_ID == 1)
    if (dvdErrorFlag.fID_MISMATCH)
    {
        dvdErrorFlag.fID_MISMATCH = CLEAR;
        return(ID_FAIL);
    }
#endif /* DVD_VERIFY_ID */

    if (dvdBlockInterruptTimer.fTIMEOUT)
    {
        dvdBlockInterruptTimer.fTIMEOUT = CLEAR;
        return(TIMEOUT);
    }
    /*---------------------------------------------------------------
        General Fail Error - should never get here
    ----------------------------------------------------------------*/
    return(GEN_FAIL);

}

/*******************************************************************************

    FUNCTION        CheckBlockCount

    DESCRIPTION     Insures that the given block count does not exceed the
                    maximum number of DVD blocks allocated to the buffer

    INPUTS          BlockCount

    RETURNS         NewBlockCount

    NOTES           A future implementation should insure that the maximum
                    blocks allocated to the buffer does not exceed the
                    maximum value of the BKTOBUF register

********************************************************************************/
WORD CheckBlockCount(ULONG BlockCount)
{

    if( BlockCount >= MAX_DVD_BLOCKS )
        BlockCount = MAX_DVD_BLOCKS - 1;        /* Adjust the block count   */

    return((WORD)BlockCount);
}
/*******************************************************************************

    FUNCTION        dvdExtendBuffering()

    DESCRIPTION     Extends buffering by given amount. If the BKTOBUF register
                    (blocks to buffer) is less than the minimum for extending
                    buffering then extend request will be rejected

    RETURNS         TRUE            BlockCount was added to buffering count
                    FALSE           Could not extend buffering

********************************************************************************/
BYTE dvdExtendBuffering ()
{
    FLAG PreIncBKTOBUF = FALSE;
    /*---------------------------------------------------------------
        Insure BlockCount does not excede extend limit
    ----------------------------------------------------------------*/
    if (dvdDiscRequest.ExtendBlocks > MAX_VALUE_OF_BLKINC)
        dvdDiscRequest.ExtendBlocks = MAX_VALUE_OF_BLKINC;

    /*---------------------------------------------------------------
        If the number of blocks to be bufferred is less than the minimum
        allowed for an extend reject the extend request
    ----------------------------------------------------------------*/
    if ((pdbDiscLoaded() == DVD_DISC) && (pdbGetDVDType() == DVD_RAM))
    {
        send_msg5SValue(SEND_MRW_MSG,"Extend Check",2,READ_FIELD(HAL_DVDD_BKTOBUF));
        if( READ_FIELD(HAL_DVDD_BKTOBUF) <= MIN_BKTOBUF_FOR_EXTEND )
        {
            //send_msg54(SEND_MRW_MSG,'a','a','a','a');
            send_msg80(SEND_SStpTask_MSG,0x710220);
            WRITE_FIELD( HAL_DVDD_BLKINC, 1);
            PreIncBKTOBUF = TRUE;
        }
    }

    if(READ_FIELD(HAL_DVDD_BKTOBUF) <= MIN_BKTOBUF_FOR_EXTEND )
        return FALSE;

    /*---------------------------------------------------------------
        Extend bufferring. The amount loaded into BLKINC will
        increase the count in register BKTOBUF.
    ----------------------------------------------------------------*/
#if (ENABLE_HDDVD)
            if(pdbDiscLoaded() == HD_DISC)
            {
                if(dvdDiscRequest.ExtendBlocks & 0x01)
                    dvdDiscRequest.ExtendBlocks++; /*HD DISC must set to EVEN value*/
            }
#endif

    if ((PreIncBKTOBUF == TRUE) && (dvdDiscRequest.ExtendBlocks >= 1))
        WRITE_REG(HAL_DVDDREG_MAP->BLKINC,(dvdDiscRequest.ExtendBlocks - 1));
    else
        WRITE_REG(HAL_DVDDREG_MAP->BLKINC,(dvdDiscRequest.ExtendBlocks));
    dvdDiscRequest.EccBlockCount += dvdDiscRequest.ExtendBlocks;

    dvdDecFlag.fEXTEND_READ = TRUE;

    return TRUE;
}

/*******************************************************************************

    FUNCTION        dvdInitFlags

    DESCRIPTION     Clears all dvd error, read, and interrupt flags

********************************************************************************/
void dvdInitFlags()
{

    dvdResetErrorFlags();

    /*---------------------------------------------------------------
        Clear DVD decoder flags
    ----------------------------------------------------------------*/
    dvdDecFlag.fEXTEND_READ             = CLEAR;
    dvdDecFlag.fFIRST_HOST_BLOCK        = CLEAR;
    dvdDecFlag.fREAD_BCA                = CLEAR;
    dvdDecFlag.fSYNC_INTERRUPT_ENABLED  = CLEAR;
    dvdDecFlag.fREAD_LONG_COMMAND       = CLEAR;
    dvdDecFlag.fREADING_LEADIN          = CLEAR;
    dvdDecFlag.fEXTEND_READ             = CLEAR;
    dvdDecFlag.fREAD_AHEAD              = CLEAR;
    dvdDecFlag.fBUFI_ENABLED            = CLEAR;
//    dvdDecFlag.fLOG_PIPO                = CLEAR;
//    dvdDecFlag.fIGNORE_ECC_ERRORS       = CLEAR;
    dvdDecFlag.fDISABLE_READ_AHEAD      = SET;

    /*---------------------------------------------------------------
        Clear DVD decoder interrupt flags
    ----------------------------------------------------------------*/
#if (ENABLE_HDDVD == 1)
    dvdDecIntFlag.fCLUSTER_BLK_INT = CLEAR;
#endif
    dvdDecIntFlag.fECC_BLK_INT     = CLEAR;
    dvdDecIntFlag.fECC_DONE_INT    = CLEAR;
    dvdDecIntFlag.fBUF_BLK_INT     = CLEAR;
    dvdDecIntFlag.fBUF_DONE_INT    = CLEAR;
    dvdDecIntFlag.fNO_INT          = TRUE;
}

/*******************************************************************************

    FUNCTION        dvdResetErrorFlags

    DESCRIPTION     Clears all dvd error flags

********************************************************************************/
void dvdResetErrorFlags()
{
    /*---------------------------------------------------------------
        Clear DVD error flags
    ----------------------------------------------------------------*/
    dvdErrorFlag.fSEQ_ID_ERROR      = CLEAR;
    dvdErrorFlag.fUNCOR_ECC         = CLEAR;
    dvdErrorFlag.fBCA_ERROR         = CLEAR;
    dvdErrorFlag.fEDC_ERROR         = CLEAR;
    dvdErrorFlag.fID_ERROR          = CLEAR;
    dvdErrorFlag.fSEEK_FAIL         = CLEAR;
    dvdErrorFlag.fSYNC_LOST         = CLEAR;
    dvdErrorFlag.fTID_MISS          = CLEAR;
    dvdErrorFlag.fUNAUTH_READ_ERROR = CLEAR;
    dvdErrorFlag.fBANDWIDTH_ERROR   = CLEAR;
    dvdErrorFlag.fID_MISMATCH       = CLEAR;

}

#if (BypassDecoderEr == 1)
BYTE dvdValidBypassDvDecoderEr()
{
    BYTE Status = FALSE;
    BYTE track;
    PSNs NWAPSN;

    #if(ENABLE_HDDVD)
    if(pdbDiscLoaded() == HD_DISC)
    {
        return FALSE;
    }
    #endif
#if (CEProduct == 0)
//support bypass decoder with READ10 command for CEProduct.
    if(dvdDecFlag.fSTREAMING == FALSE)
        return FALSE;
#endif        
    if (dvdDecFlag.fREADING_LEADIN == TRUE)
        return FALSE;
    if ((dvdLastDecodedPSN > DVD_START_OF_DATA_ZONE) && (pdbGetDVDType() == DVD_ROM))
    {
        if(dvdLastDecodedPSN > 0x30200)
        {
            Status = TRUE;
        }
    }
    else
    {
        track = pdbGetDVDHighestRecordedTrackNumber();
        if (pdbGetDVDTrackDiagStatus2(track) & eTS_NWA_valid)       // 0x08
        {
            NWAPSN = dvdConvertLBAtoPSN(pdbGetDVDTrackNextWritableAddress(track));
            //send_msg5SValue(1,"NWAValid",4,NWAPSN);
        }
        else
        {
            NWAPSN = dvdConvertLBAtoPSN(pdbGetDVDTrackEndPSN(track));
            //send_msg5SValue(1,"NWAdone",4,NWAPSN);
        }

        if((dvdLastDecodedPSN > 0x34100) && (dvdLastDecodedPSN < (NWAPSN.PSN - 0x300)))
            Status = TRUE;
    }
    return Status;
}
#endif
