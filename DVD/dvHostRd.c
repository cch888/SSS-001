/****************************************************************************
*                (c) Copyright 2000 - 2005 SUNEXT DESIGN, INC.
*                           All Right Reserved
*
*
*    FILE NAME:     $Workfile: dvHostRd.c $
*
*    DESCRIPTION:   Manages the host read command and host
*                   transfer routines for DVD
*
*
*   $Revision: 120 $
*   $Date: 11/03/28 2:36p $
*
**************************** SUNEXT CONFIDENTIAL *******************************/

#include ".\common\globals.h"
#include ".\common\globtype.h"
#include ".\common\system.h"
#include ".\hif\gp_ram.h"
#include ".\hif\rd_util.h"
#include ".\oem\oem_spd_tbl.h"
#include ".\dvd\dvGvars.h"
#include ".\dvd\dvmacros.h"
#include ".\dvd\dvHostRd.h"
#include ".\dvd\dvBufMgr.h"
#include ".\dvd\dvDiscRd.h"
#include ".\dvd\dvutil.h"
#include ".\servo\SvoVar.H"
#if (ENABLE_HDDVD)
#include ".\hd\hdBufMgr.h"
#include ".\hd\hdDef.h"
#include ".\hd\hdDB.h"
#endif
#include ".\al\REG_dvd_decoder.h"

#ifdef READ10_SSI  // Read10 command support on SSI port
#if (MSG_OUT_MODE == MSG_USBPORT && ENABLE_UART_RXD == 1)
#include ".\common\scomm.h"
#endif
#endif

/*******************************************************************
    Internal function prototypes
*******************************************************************/
void dvdReadExit(void);
DiscAccessStatusE FullCacheHit(void);
DiscAccessStatusE PartialCacheHit(void);
DiscAccessStatusE CacheMiss(void);
void CheckDVDPlay(void);
BYTE dvdValidReadRequest(void);

/*******************************************************************************
    FUNCTION DEFINITIONS
*******************************************************************************/
/*******************************************************************************

    FUNCTION        DVDRead10Command

    DESCRIPTION     Command handler for DVD Read 10 and Read 12 commands
                    This function will parse the command and then call the
                    appropriate process depending upon the result of a cache
                    search. Either process, cache hit, partial cache hit
                    or cache miss, will initiate the host transfer and/or DVD
                    buffer loading. A status of the buffer loading will be
                    processed at the end of this function.

                    NOTE: Read Long diagnostic command is flagged in this
                          routine for the lower level setup for reading
                          a long DVD block (raw ecc block)


********************************************************************************/
extern BYTE CmdSeekOk;
void DVDRead10Command()
{
    ULONG XferTimer;
    SearchCacheResultE  SearchResult;   /* Holds the result of a cache search           */
    DiscAccessStatusE   DiscReadStatus; /* Status of buffer after request completed     */

    /*-------------------------------------------------------------------
        Initialize module variable HostXfer
    --------------------------------------------------------------------*/
    HostXfer.StartBlock.lba    = 0L;
    HostXfer.BlockCount.Long  = 0L;

    /*-------------------------------------------------------------------
        Get the host request starting LBA from the command block
    --------------------------------------------------------------------*/
    HostXfer.StartBlock.byte.u = Iram_Cmd_block.bf[2];
    HostXfer.StartBlock.byte.h = Iram_Cmd_block.bf[3];
    HostXfer.StartBlock.byte.m = Iram_Cmd_block.bf[4];
    HostXfer.StartBlock.byte.l = Iram_Cmd_block.bf[5];

    /*-------------------------------------------------------------------
        Get the requested host transfer length
    --------------------------------------------------------------------*/
    if ( Iram_Cmd_block.bf[0] == SBC_READ_10 )
    {
        HostXfer.BlockCount.byte.m = Iram_Cmd_block.bf[7];
        HostXfer.BlockCount.byte.l = Iram_Cmd_block.bf[8];
        if (dvdDecFlag.fSTREAMING == TRUE)
        {
            if( DVD_BUFFERING() == ON )
                dvdStopDiscTransfer();

            ClearDataBuffer();
        }
        dvdDecFlag.fSTREAMING = FALSE;
    }
    else if ( Iram_Cmd_block.bf[0] == SBC_READ_12 )/* SBC_READ_12 */
    {
        HostXfer.BlockCount.byte.h = Iram_Cmd_block.bf[7];
        HostXfer.BlockCount.byte.m = Iram_Cmd_block.bf[8];
        HostXfer.BlockCount.byte.l = Iram_Cmd_block.bf[9];
        if(Iram_Cmd_block.bf[10]&0x80)
            dvdDecFlag.fSTREAMING = TRUE;
        else
        {
            //When Streaming Read is switch from ON to OFF,
            //clear the data in the buffer.
            if (dvdDecFlag.fSTREAMING == TRUE)
            {
                if( DVD_BUFFERING() == ON )
                    dvdStopDiscTransfer();

                ClearDataBuffer();
            }

            dvdDecFlag.fSTREAMING = FALSE;
        }
        if((Iram_Cmd_block.bf[1]&0x08)&&(Iram_Cmd_block.bf[10]&0x80)){
            BUILD_SENSE(0x05,0x24,0x00,0xC0);
            dvdReadExit();
            return;
        }
    }
    #if(PCB == 710)||(PCB == 711)||(PCB == 817)
    else if ( Iram_Cmd_block.bf[0] == MMC_READ_CD )//Read DVD Flow support Read CD command.
    {
        Iram_Cmd_block.bf[1] >>=2;
        Iram_Cmd_block.bf[1] &= 0x07;
        //Expected Sector Type isn't user data 2048bytes.
        if (Iram_Cmd_block.bf[1] != 0x00
            &&  Iram_Cmd_block.bf[1] != 0x02
            &&  Iram_Cmd_block.bf[1] != 0x04){
            BUILD_SENSE(0x05,0x64,0x00,0x3E);
            dvdReadExit();
            return;
        }

        //Byte_10 Sub-Channel Data Selection Bit is setting.
        //Byte_9 User Data isn't setting.
        if ( Iram_Cmd_block.bf[10] ||  Iram_Cmd_block.bf[9] != 0x10){
            BUILD_SENSE(0x05,0x24,0x00,0xC1);
            dvdReadExit();
            return;
        }

        HostXfer.BlockCount.byte.h = Iram_Cmd_block.bf[6];
        HostXfer.BlockCount.byte.m = Iram_Cmd_block.bf[7];
        HostXfer.BlockCount.byte.l = Iram_Cmd_block.bf[8];
    }
    #endif
    /*-------------------------------------------------------------------
        Initial remaining blocks to transfer
    --------------------------------------------------------------------*/
    HostXfer.BlocksLeft = HostXfer.BlockCount.Long;

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
    if (dvdValidReadRequest() == FALSE)
    {
        BUILD_SENSE(0x05,0x21,0x00,0x07);
        dvdReadExit();
        return;
    }
    /*---------------------------------------------------------------
        Set to maximum speed
    ----------------------------------------------------------------*/
    if(SpeedMD==Set_sp_f)
    {
        SpeedMD=Special_f;
    }
#if(CEProduct == 0)
    // bypass speed up/down when CDSpeed test.
    if((SpeedMD == AutoMode) && (HostXfer.BlockCount.Long == 0x19))
    {
        SpeedMD = Special_for_CDSpeed_f;
    }
    else if((SpeedMD == Special_for_CDSpeed_f)&&(HostXfer.BlockCount.Long != 0x19))
    {
        RandomRead = TRUE; //restart the automode.
        SpeedMD = AutoMode;
    }
#endif

    CmdSeekOk = 0;      /*initial flag */

    /*-----------------------------------------------------------------------
        Search the cache
        NOTE: If Read Ahead is disabled always set result to cache miss
    -----------------------------------------------------------------------*/
#if (DVD_READ_AHEAD_ENABLED == TRUE)
    #if (ENABLE_HDDVD == 1)
    if(pdbDiscLoaded() == HD_DISC)
    {
        if (!( Iram_Cmd_block.bf[1] & FUA ))
            SearchResult = dvdSearchCache(HostXfer,WAIT_SEARCH_MODE);
        else
            SearchResult = CACHE_MISS;
    }
    else
    #endif
    {
        if (!( Iram_Cmd_block.bf[1] & FUA ))
            SearchResult = dvdSearchCache(HostXfer,WAIT_SEARCH_MODE);
        else
            SearchResult = CACHE_MISS;
    }
#else
        SearchResult = CACHE_MISS;
#endif

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
            DiscReadStatus = FullCacheHit();
            break;

        case CACHE_MISS:
            DiscReadStatus = CacheMiss();
            break;

        case PARTIAL_CACHE_HIT:
            DiscReadStatus = PartialCacheHit();
            break;

    }

    /*-----------------------------------------------------------------------
        Process the disc read status
    -----------------------------------------------------------------------*/
    if (DiscReadStatus == BUF_GOOD)
    {
       /*---------------------------------------------------------------
            All host blocks are in the buffer. Insure the transfer to
            host completes before exiting the command
       ---------------------------------------------------------------*/
        StartTimer(&XferTimer);

        while( HostXfer.State != TRANSFER_COMPLETE )
        {
            if(ReadTimer(&XferTimer) > 1500L)
            {
                //send_msg5S(SEND_READ_DVD_MSG, "1.5s timeout");
                send_msg80(SEND_READ_DVD_MSG,0x710128);
                HostXfer.State = TRANSFER_STOPPED;
                dvdSetSenseCode(TIMEOUT);
                break;
            }

#ifdef READ10_SSI  // Read10 command support on SSI port
        #if(MSG_OUT_MODE == MSG_USBPORT && ENABLE_UART_RXD == 1)
            if (s2b_atapi_cmd_received == TRUE)
            {
                if (HostXfer.BlocksLeft > 0)
                {
                    SendHostBlocks();
                }
            }
        #endif
        #endif

            ExitProcess();
        }
    }
    else /* Error in buffering host data */
    {
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

#if (RANDOM_READ_BYPASS_CHECK_METHOD == 1)
    if (RandomCount > CONTINUE_RANDOM_READ_CNT)
    {
        dvdStopDiscTransfer();
        dvdResetDiscFlags();
        //Svr_pause_reqire = TRUE;
    }
#endif

    dvdReadExit();
    return;
}
/*******************************************************************************

    FUNCTION        CacheMiss

    DESCRIPTION     Calls buffer manager to initialize the host buffer and then
                    initiates the transfer sequence.

    RETURNS         DiscReadStatus  Status of the disc read from the DVD buffer
                                    loading process

********************************************************************************/
DiscAccessStatusE CacheMiss()
{
    DiscAccessStatusE DiscReadStatus;   /* Status of disc read */
    //send_msg5S(SEND_MRW_MSG,"*CacheMiss*");
    send_msg80(SEND_MRW_MSG,0x710129);

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
#if (ENABLE_HDDVD)
    if(pdbDiscLoaded() == HD_DISC)
        hdInitializeHostBuffer(HostXfer.StartBlock.lba);
   else
#endif
        dvdInitializeHostBuffer(HostXfer.StartBlock.lba);

    /*---------------------------------------------------------------
        Set host transfer state to waiting
    ----------------------------------------------------------------*/
    HostXfer.State = WAITING_FOR_TRANSFER;

    /*---------------------------------------------------------------
        Start loading the buffer
    ----------------------------------------------------------------*/
    send_msg80(SEND_MRW_MSG,0x71002A);
#if (ENABLE_HDDVD)
    if(pdbDiscLoaded() == HD_DISC)
        DiscReadStatus = hdLoadBuffer( HostXfer, INIT_READ);
   else
#endif
        DiscReadStatus = dvdLoadBuffer( HostXfer, INIT_READ);

    return (DiscReadStatus);
}

/*******************************************************************************

    FUNCTION        FullCacheHit

    DESCRIPTION     Process a host transfer when all host data is already in
                    the buffer

    RETURNS         BUF_GOOD        Indicates buffer is good

********************************************************************************/
DiscAccessStatusE FullCacheHit()
{
    send_msg80(SEND_MRW_MSG,0x71002B);
    /*-------------------------------------------------------------------
        If the StartLBA is not the start lba of the cache then call
        dvdAdjustHostBufferPtr to adjust the host buffer ptr to point
        to the location where the transfer is to begin.
    ---------------------------------------------------------------*/
    if (HostXfer.StartBlock.lba != DataBuffer.CacheStartLBA)
    {
#if (ENABLE_HDDVD)
        if(pdbDiscLoaded() == HD_DISC)
            hdAdjustHostBufferPtr( HostXfer.StartBlock.lba );
       else
#endif
            dvdAdjustHostBufferPtr( HostXfer.StartBlock.lba );
    }

    /*---------------------------------------------------------------
        Start the host transfer.
    ---------------------------------------------------------------*/
    SendHostBlocks();

    return( BUF_GOOD );
}

/*******************************************************************************

    FUNCTION        PartialCacheHit

    DESCRIPTION     Process host request if only some of the host data is in
                    the cache.

    RETURNS         DiscReadStatus  Status of the disc read from the DVD buffer
                                    loading process

********************************************************************************/
DiscAccessStatusE PartialCacheHit()
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
    send_msg80(SEND_MRW_MSG,0x71002C);

    if (HostXfer.StartBlock.lba != DataBuffer.CacheStartLBA)
    {
#if (ENABLE_HDDVD)
            if(pdbDiscLoaded() == HD_DISC)
                hdAdjustHostBufferPtr( HostXfer.StartBlock.lba );
           else
#endif
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
        //send_msg5S(SEND_READ_DVD_MSG,"*EXTEND_READ*");
        send_msg80(SEND_READ_DVD_MSG,0x71012A);
#if (ENABLE_HDDVD)
        if(pdbDiscLoaded() == HD_DISC)
            DiscReadStatus = hdLoadBuffer( HostXfer, EXTEND_READ);
       else
#endif
            DiscReadStatus = dvdLoadBuffer( HostXfer, EXTEND_READ);

        /*---------------------------------------------------------------
            If buffering good, all host blocks have been loaded so exit
        ---------------------------------------------------------------*/
        if (DiscReadStatus == BUF_GOOD)
            return (DiscReadStatus);

        /*---------------------------------------------------------------
            If the extend failed wait for buffering to stop. If there
            is an error on a host block, buffering should be stopped
        ---------------------------------------------------------------*/
        while ( (DVD_BUFFERING() == ON) && (dvdBlockInterruptTimer.fTIMEOUT == FALSE) )
            ExitProcess();

        if(dvdBlockInterruptTimer.fTIMEOUT == TRUE)
            StopDecoder();
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
        //send_msg5S(SEND_READ_DVD_MSG,"*INIT_READ*");
        send_msg80(SEND_READ_DVD_MSG,0x71012B);
#if (ENABLE_HDDVD)
        if(pdbDiscLoaded() == HD_DISC)
            DiscReadStatus = hdLoadBuffer( NewReq, INIT_READ);
       else
#endif
            DiscReadStatus = dvdLoadBuffer( NewReq, INIT_READ);
    }

    return (DiscReadStatus);

}
/*******************************************************************************

    FUNCTION        dvdReadExit

    DESCRIPTION     Issues a command complete. This function is called at
                    the completion of a DVD read command

********************************************************************************/
void dvdReadExit(void)
{
    ReadyLED(LED_OFF);      /* Turn off LED             */
    AutoCommandComplete();  /* Issue command complete   */
    return;
}

/*******************************************************************************

    FUNCTION        dvdFastCacheTransfer

    DESCRIPTION     An immediate search and transfer for DVD read commands
                    handled from the command interrupt routine. This function
                    also determines whether to restart the read ahead process
                    by setting the flag dvdDecFlag.fRESTART_READ_AHEAD

********************************************************************************/
BYTE dvdFastCacheTransfer()
{
    SearchCacheResultE  SearchResult;   /* Holds the result of a cache search   */
    ADDRESS_T temp;
#if (RANDOM_READ_BYPASS_CHECK_METHOD == 1)
    BYTE DvdRandomRead = FALSE;
#endif

    /*-------------------------------------------------------------------
        If the FUA bit in command byte 1 is set there will be no
        transfer from the cache
    --------------------------------------------------------------------*/
    if ( Iram_Cmd_block.bf[1] & FUA )
    {
        RandomRead = TRUE;
        return FALSE;
    }

    /*-------------------------------------------------------------------
        Initialize module variables
    --------------------------------------------------------------------*/
    temp.lba = HostXfer.StartBlock.lba + HostXfer.BlockCount.Long;    // current cmd start lba of sequential read = pre-cmd start lba + pre-required length
    HostXfer.StartBlock.lba    = 0L;
    HostXfer.BlockCount.Long  = 0L;

    /*-------------------------------------------------------------------
        Get the requested host transfer length
    --------------------------------------------------------------------*/
    if ( Iram_Cmd_block.bf[0] == SBC_READ_10 )
    {
        HostXfer.BlockCount.byte.m = Iram_Cmd_block.bf[7];
        HostXfer.BlockCount.byte.l = Iram_Cmd_block.bf[8];
        if (dvdDecFlag.fSTREAMING == TRUE)
        {
            if( DVD_BUFFERING() == ON )
                dvdStopDiscTransfer();

            ClearDataBuffer();
        }
    }
    else /* SBC_READ_12 */
    {
        HostXfer.BlockCount.byte.h = Iram_Cmd_block.bf[7];
        HostXfer.BlockCount.byte.m = Iram_Cmd_block.bf[8];
        HostXfer.BlockCount.byte.l = Iram_Cmd_block.bf[9];
        if ( (Iram_Cmd_block.bf[10] & 0x80 == 0x00) && (dvdDecFlag.fSTREAMING == TRUE) )
        {
            if( DVD_BUFFERING() == ON )
                dvdStopDiscTransfer();

            ClearDataBuffer();
        }
    }

    /*-------------------------------------------------------------------
        Initial remaining blocks to transfer
    --------------------------------------------------------------------*/
    HostXfer.BlocksLeft = HostXfer.BlockCount.Long;

    /*-------------------------------------------------------------------
        Exit if the transfer length is zero (no transfer)
    --------------------------------------------------------------------*/
    if (HostXfer.BlockCount.Long == 0L )
        return FALSE;

    /*-------------------------------------------------------------------
        Get the host request starting LBA from the command block
    --------------------------------------------------------------------*/
    HostXfer.StartBlock.byte.u = Iram_Cmd_block.bf[2];
    HostXfer.StartBlock.byte.h = Iram_Cmd_block.bf[3];
    HostXfer.StartBlock.byte.m = Iram_Cmd_block.bf[4];
    HostXfer.StartBlock.byte.l = Iram_Cmd_block.bf[5];

    /*-----------------------------------------------------------------------
        Search the cache. Exit if not a full cache hit
    -----------------------------------------------------------------------*/
#if (ENABLE_HDDVD)
    if(pdbDiscLoaded() == HD_DISC)
        SearchResult = hdSearchCache(HostXfer,IMMEDIATE_SEARCH_MODE);
   else
#endif
        SearchResult = dvdSearchCache(HostXfer,IMMEDIATE_SEARCH_MODE);

    /*-----------------------------------------------------------------------
        Check if random read for DVD.
        If the require data of current read cmd is overlaping pre-read cmd, it isn't
        RandomRead because of the pre-read cmd required data is still in the buffer possibly.
    -----------------------------------------------------------------------*/
    if((HostXfer.StartBlock.lba > temp.lba) || (HostXfer.StartBlock.lba < DataBuffer.CacheStartLBA))
    {
        RandomRead = TRUE;
#if (RANDOM_READ_BYPASS_CHECK_METHOD == 1)
        DvdRandomRead = TRUE;
#endif
    }
    else
    {
        RandomRead = FALSE;
#if (EN_ADEQ == 1)//Disable ADEQ when random access, for ipeak
        if ((Disable_ADEQ & 0x80) == 0x80)
        {
            if ((Disable_ADEQ & 0x01) == 0x01)
            {
                Disable_ADEQ = 1;
            }
            else
            {
                Disable_ADEQ = 0;
            }
        }
#endif

#if (RANDOM_READ_BYPASS_CHECK_METHOD == 1)
        DvdRandomRead = FALSE;
#endif
    }
#if (RANDOM_READ_BYPASS_CHECK_METHOD == 1)||(SEQ_SEEK_SPEED_DOWN == 1)
#if (RANDOM_READ_BYPASS_CHECK_METHOD == 1)
    if (DvdRandomRead)
#else
    if(RandomRead)
#endif
    {
        RandomCount++;
        if (RandomCount == 0xFFFFFFFF)  //wrap back
            RandomCount = CONTINUE_RANDOM_READ_CNT + 1;
    }
    else
    {   //if any sequential read coming
        RandomCount = 0;
    }
#endif

    if (SearchResult != FULL_CACHE_HIT)
        return FALSE;

    /*---------------------------------------------------------------
        Process the full cache hit (this starts the host transfer)
    ---------------------------------------------------------------*/
    FullCacheHit();

    return TRUE;

}
/*******************************************************************************

    FUNCTION        dvdSetSenseCode

    DESCRIPTION     Builds the appropriate sense code for the type of error
                    reported

    PARAMETERS      ErrorStatus - Type of error reported

********************************************************************************/
void dvdSetSenseCode( DiscAccessStatusE ErrorStatus)
{

    switch( ErrorStatus )
    {

        case UNCOR_ECC_FAIL:    /* L-EC uncorrectable error */
            BUILD_SENSE(0x03,0x11,0x05,0x03);
            break;

        case UNAUTH_RD_FAIL:    /* Read of scrambled sector without authentication */
            BUILD_SENSE(0x05,0x6F,0x03,0x08);
            break;

        case SEQID_FAIL:        /* Block sequence error */
            BUILD_SENSE(0x03,0x14,0x04,0x04);
            break;

        case ID_FAIL:           /* ID, CRC or ECC error */
            BUILD_SENSE(0x03,0x10,0x00,0x05);
            break;

        case SEEK_FAIL:         /* No seek complete */
            BUILD_SENSE(0x03,0x02,0x00,0x06);
            break;

        case TIMEOUT:           /* Insufficient time for operation */
            BUILD_SENSE(0x03,0x11,0x05,0x05);
            break;

        case TID_FAIL:          /* Internal target failure */
            //BUILD_SENSE(0x04,0x44,0x00,0x10);
            BUILD_SENSE(0x03,0x11,0x01,0x06); // read retries exhausted
            break;

        case EDC_FAIL:          /* ID, CRC or ECC error */
            BUILD_SENSE(0x03,0x10,0x00,0x07);
            break;

        case SYNC_LOST_FAIL:    /* Re-synchronization error  */
            BUILD_SENSE(0x03,0x11,0x07,0x08);
            break;
        case EJECT_PRESS:       /* TRAY OPEN */
            BUILD_SENSE(0x02,0x3A,0x02,0x01);
            break;
        default:                /* Unrecovered read error */
            BUILD_SENSE(0x03,0x11,0x00,0x09);
    }
}

/*******************************************************************************

    FUNCTION        dvdValidReadRequest

    DESCRIPTION     Determines if read request is valid

    PARAMETERS      Uses global parameter HostXfer

    RETURNS         TRUE    Valid read range from starting lba to end lba
                    FALSE   Invalid read range from starting lba to end lba

********************************************************************************/
BYTE dvdValidReadRequest()
{
    BYTE Status = FALSE;
    BYTE track;
    ULONG dvdEndLBA;

    // Test if disc is blank
    if (St_blank_disc == TRUE)
        return Status;

    //start== HP CMPLY test TDR840 fail.
    if(HostXfer.StartBlock.lba > pdbGetDVDMaxLBA())
        return Status;
    //end==

    // Calculate ending LBA of the host transfer
    dvdEndLBA = HostXfer.StartBlock.lba + HostXfer.BlockCount.Long - 1L;

#if (ENABLE_HDDVD == 1)
    // -------------------------------------------------
    // HDDVD DISC
    // -------------------------------------------------
    if(pdbDiscLoaded() == HD_DISC)
    {
        if (dvdEndLBA <= pdbGetHDMaxLBA())
            Status = TRUE;

        return Status;
    }
#endif

    // -------------------------------------------------
    // DVD ROM OR FINALIZED DISC
    // -------------------------------------------------
    if ((pdbGetDVDType() == DVD_ROM) || (pdbGetDiscStatus() == eFinalizedDisc))
    {
        // Check end LBA of request
        if (dvdEndLBA <= pdbGetDVDMaxLBA())
            Status = TRUE;
    }
    // -------------------------------------------------
    // Possible Multi Session Disc (non-finalized disc)
    // -------------------------------------------------
    else
    {
        track = pdbGetDVDHighestRecordedTrackNumber();
        // Check end LBA of request with last written lba of track
       if((pdbGetDiscKind() == eDVDMinusRWDisc)&&(Info.Media.DVD.RMAData.RMD.Format2.DiscStatus ==  RestricedOverWrite))
            {
            if (dvdEndLBA <= dvdConvertPSNtoLBA(pdbGetDVDTrackEndPSN(track)))
                return TRUE;
            }
        else if (pdbGetDVDTrackState(track) == eTRACK_STATE_COMPLETE)
        {
            //==start== Correct open session DVD-R DL disc max lab.
            if ((pdbGetDiscKind() == eDVDMinusRDLDisc)&&(pdbGetDVDRMARMDFormat() == Format4_RMD))
            {
                //20090319_j.25 ==start== Correct DVD-R DL format valid address.
                ULONG TrackStartLBAL1,TempULONG;
                TempULONG = pdbGetDVDRZoneLJA(track);    // Layer Jump PSN
                TempULONG = (~TempULONG)&0x00FFFFFFL;
                TrackStartLBAL1 = dvdConvertPSNtoLBA(TempULONG);
                if(((dvdEndLBA >= dvdConvertPSNtoLBA(pdbGetDVDTrackStartPSN(1)))&&(dvdEndLBA<=dvdConvertPSNtoLBA(pdbGetDVDRZoneLJA(track))))
                    ||((dvdEndLBA >= dvdConvertPSNtoLBA(TrackStartLBAL1))&&(dvdEndLBA <= dvdConvertPSNtoLBA(pdbGetDVDTrackRecordedPSN(1)))))
                    Status = TRUE;
                //20090319_j.25 ==start== Correct DVD-R DL format valid address.
            }
            else
            {
                if (dvdEndLBA <= dvdConvertPSNtoLBA(pdbGetDVDTrackEndPSN(track)))
                    Status = TRUE;
            }
        }
        else
        {
            if (pdbGetDVDTrackRecordedPSN(track) != 0)
            {
                if (dvdEndLBA <= dvdConvertPSNtoLBA(pdbGetDVDTrackRecordedPSN(track)))
                    Status = TRUE;
            }
            else
            {
                if (dvdEndLBA <= dvdConvertPSNtoLBA(pdbGetDVDTrackEndPSN(track - 1)))
                    Status = TRUE;
            }
        }

        track = 1;
        do
        {
            if(dvdEndLBA > dvdConvertPSNtoLBA(pdbGetDVDTrackRecordedPSN(track)))
                track++;
        else
            break;
        }while(track <= pdbGetDVDHighestRecordedTrackNumber());

        if((pdbGetDVDTrackState(track) == eTRACK_STATE_EMPTY)||(pdbGetDVDTrackState(track) == eTRACK_STATE_BLANK))
            Status = FALSE;
    }

     return Status;
}
