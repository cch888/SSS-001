/****************************************************************************
*                (c) Copyright 2000 - 2005 SUNEXT DESIGN, INC.
*                           All Right Reserved
*
*
*    FILE NAME:     $Workfile: bdHostRd.c $
*
*    DESCRIPTION:   Manages the host read command and host
*                   transfer routines for BD
*
*
*   $Revision: 30 $
*   $Date: 11/03/28 2:36p $
*
**************************** SUNEXT CONFIDENTIAL *******************************/

#include ".\common\globals.h"
#include ".\common\globtype.h"
#include ".\common\system.h"

#include ".\hif\gp_ram.h"
#include ".\hif\rd_util.h"
#include ".\hif\at_equ.h"

#include ".\oem\oem_spd_tbl.h"

#include ".\dvd\dvDef.h"
#include ".\dvd\dvHostRd.h"
#include ".\dvd\dvBufMgr.h"

#include ".\bd\bdVars.h"
#include ".\bd\bdMacro.h"
#include ".\bd\bdHostRd.h"
#include ".\bd\bdBufMgr.h"
#include ".\bd\bdDiscRd.h"
#include ".\bd\bdUtil.h"
#if (ENABLE_AACS == 1)
#include ".\crypt\aacs.h"
#endif

#ifdef READ10_SSI  // Read10 command support on SSI port
#if (MSG_OUT_MODE == MSG_USBPORT && ENABLE_UART_RXD == 1)
#include ".\common\scomm.h"
#endif
#endif

/*******************************************************************
    Internal function prototypes
*******************************************************************/
void  bdReadExit(void);
DiscAccessStatusE bdCacheMiss();
DiscAccessStatusE bdPartialCacheHit();
DiscAccessStatusE bdFullCacheHit();
BYTE  bdValidReadRequest(void);
extern void BDInitCache(void);
/*******************************************************************************
    FUNCTION DEFINITIONS
*******************************************************************************/

#if (BD_ENABLE == 1)

/*******************************************************************************

    FUNCTION        BDRead10Command

    DESCRIPTION     Command handler for BD Read 10 and Read 12 commands
                    This function will parse the command and then call the
                    appropriate process depending upon the result of a cache
                    search. Either process, cache hit, partial cache hit
                    or cache miss, will initiate the host transfer and/or BD
                    buffer loading. A status of the buffer loading will be
                    processed at the end of this function.

********************************************************************************/
extern BYTE CmdSeekOk;
void BDRead10Command()
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
        if (bdDecFlag.fSTREAMING == TRUE)
        {
            if( BD_BUFFERING() == ON )
                bdStopDiscTransfer();

            ClearDataBuffer();
        }
        bdDecFlag.fSTREAMING = FALSE;
    }
    else /* SBC_READ_12 */
    {
        HostXfer.BlockCount.byte.h = Iram_Cmd_block.bf[7];
        HostXfer.BlockCount.byte.m = Iram_Cmd_block.bf[8];
        HostXfer.BlockCount.byte.l = Iram_Cmd_block.bf[9];
        if(Iram_Cmd_block.bf[10]&0x80)
            bdDecFlag.fSTREAMING = TRUE;
        else
        {
            if (bdDecFlag.fSTREAMING == TRUE)
            {
                if( BD_BUFFERING() == ON )
                    bdStopDiscTransfer();

                ClearDataBuffer();
            }
            bdDecFlag.fSTREAMING = FALSE;
        }
        if((Iram_Cmd_block.bf[1]&0x08)&&(Iram_Cmd_block.bf[10]&0x80)){
            BUILD_SENSE(0x05,0x24,0x00,0xf1);
            bdReadExit();
            return;
        }
    }

    /*-------------------------------------------------------------------
        Initial remaining blocks to transfer
    --------------------------------------------------------------------*/
    HostXfer.BlocksLeft = HostXfer.BlockCount.Long;

    /*---------------------------------------------------------------
        For zero transfer length exit with command complete.
    ----------------------------------------------------------------*/
    if (HostXfer.BlockCount.Long == 0L )
    {
        bdReadExit();
        return;
    }

    /*-------------------------------------------------------------------
        Check if read is within a valid range
    --------------------------------------------------------------------*/
    if (bdValidReadRequest() == FALSE)
    {
        BUILD_SENSE(0x05,0x21,0x00,0x81);
        bdReadExit();
        return;
    }

    if (VerifyPACRules(eDataArea,eRead) == TRUE)
    {
        BUILD_SENSE(0x03,0x31,0x00,0x81);
        bdReadExit();
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
    #if (BD_READ_AHEAD_ENABLED == TRUE)
        if (!( Iram_Cmd_block.bf[1] & FUA ))
            SearchResult = bdSearchCache(HostXfer,WAIT_SEARCH_MODE);
        else
            SearchResult = CACHE_MISS;
    #else
        SearchResult = CACHE_MISS;
    #endif

#if (EN_POWER_CONTROL == 1)
    if(SearchResult != FULL_CACHE_HIT)
        SetDiscReadPowerMode();
#endif

    /*---------------------------------------------------------------
        Process the search result. Each process returns the status
        of the buffer access (DiscReadStatus).

        NOTE: For bdFullCacheHit() DiscReadStatus is always BUF_GOOD.
    ---------------------------------------------------------------*/
    switch (SearchResult)
    {

        case FULL_CACHE_HIT:
            DiscReadStatus = bdFullCacheHit();
            break;

        case CACHE_MISS:
            DiscReadStatus = bdCacheMiss();
            break;

        case PARTIAL_CACHE_HIT:
            DiscReadStatus = bdPartialCacheHit();
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
                send_msg80(SEND_READ_DVD_MSG,0x71009C);
                HostXfer.State = TRANSFER_STOPPED;
                bdSetSenseCode(TIMEOUT);
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
        bdDecFlag.fDISABLE_READ_AHEAD = TRUE;

        /*---------------------------------------------------------------
            Build the sense code according to type of DiscReadStatus
        ---------------------------------------------------------------*/
        bdSetSenseCode(DiscReadStatus);

    }

    bdReadExit();
    return;
}
/*******************************************************************************

    FUNCTION        bdCacheMiss

    DESCRIPTION     Calls buffer manager to initialize the host buffer and then
                    initiates the transfer sequence.

    RETURNS         DiscReadStatus  Status of the disc read from the BD buffer
                                    loading process

********************************************************************************/
DiscAccessStatusE bdCacheMiss()
{
    DiscAccessStatusE DiscReadStatus;   /* Status of disc read */
    send_msg80(SEND_MRW_MSG,0x71009D);


    /*---------------------------------------------------------------
        Insure read ahead buffering is enabled
    ---------------------------------------------------------------*/
    bdDecFlag.fDISABLE_READ_AHEAD = FALSE;

#if (ENABLE_AACS == 1)
    /*---------------------------------------------------------------
        Clear AacsStatus.AacsFlag.AgidInUse
    ---------------------------------------------------------------*/
    AacsStatus.AacsFlag.AgidInUse = FALSE;
#endif

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
    send_msg80(SEND_MRW_MSG,0x710003);
    
    DiscReadStatus = bdLoadBuffer( HostXfer, INIT_READ);

    return (DiscReadStatus);
}

/*******************************************************************************

    FUNCTION        bdFullCacheHit

    DESCRIPTION     Process a host transfer when all host data is already in
                    the buffer

    RETURNS         BUF_GOOD        Indicates buffer is good

********************************************************************************/
DiscAccessStatusE bdFullCacheHit()
{
    send_msg80(SEND_MRW_MSG,0x71009E);
    /*-------------------------------------------------------------------
        If the StartLBA is not the start lba of the cache then call
        bdAdjustHostBufferPtr to adjust the host buffer ptr to point
        to the location where the transfer is to begin.
    ---------------------------------------------------------------*/
    if (HostXfer.StartBlock.lba != DataBuffer.CacheStartLBA)
        bdAdjustHostBufferPtr( HostXfer.StartBlock.lba );

    /*---------------------------------------------------------------
        Start the host transfer.
    ---------------------------------------------------------------*/
    SendHostBlocks();

    return( BUF_GOOD );
}

/*******************************************************************************

    FUNCTION        bdPartialCacheHit

    DESCRIPTION     Process host request if only some of the host data is in
                    the cache.

    RETURNS         DiscReadStatus  Status of the disc read from the BD buffer
                                    loading process

********************************************************************************/
DiscAccessStatusE bdPartialCacheHit()
{

    TransferS           NewReq;             /* Truncated host request */
    DiscAccessStatusE   DiscReadStatus;     /* State of buffer after request to load buffer  */
    ULONG               HostEndLBA;         /* Ending LBA of host transfer request */

    bdDecFlag.fDISABLE_READ_AHEAD = FALSE;
    /*--------------------------------------------------------------
        If the StartLBA is not the last transferred lba + 1,
        call bdAdjustHostBufferPtr to adjust the host buffer ptr to point
        to the location where the transfer is to begin.
    ---------------------------------------------------------------*/
    send_msg80(SEND_MRW_MSG,0x71009F);

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


    if (BD_BUFFERING() == ON)
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
        send_msg5S(SEND_READ_DVD_MSG,"*EXTEND_READ*");
        DiscReadStatus = bdLoadBuffer( HostXfer, EXTEND_READ);

        /*---------------------------------------------------------------
            If buffering good, all host blocks have been loaded so exit
        ---------------------------------------------------------------*/
        if (DiscReadStatus == BUF_GOOD)
            return (DiscReadStatus);

        /*---------------------------------------------------------------
            If the extend failed wait for buffering to stop. If there
            is an error on a host block, buffering should be stopped
        ---------------------------------------------------------------*/
        while ( (BD_BUFFERING() == ON) && (bdClusterInterruptTimer.fTIMEOUT == FALSE) )
            ExitProcess();

        if(bdClusterInterruptTimer.fTIMEOUT == TRUE)
            StopDecoder();
    }

    if (BD_BUFFERING() == OFF)
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
        send_msg5S(SEND_READ_DVD_MSG,"*INIT_READ*");
        DiscReadStatus = bdLoadBuffer( NewReq, INIT_READ);
    }

    return (DiscReadStatus);

}
/*******************************************************************************

    FUNCTION        bdReadExit

    DESCRIPTION     Issues a command complete. This function is called at
                    the completion of a BD read command

********************************************************************************/
void bdReadExit(void)
{
    ReadyLED(LED_OFF);      /* Turn off LED             */
    AutoCommandComplete();  /* Issue command complete   */
    return;
}

/*******************************************************************************

    FUNCTION        bdFastCacheTransfer

    DESCRIPTION     An immediate search and transfer for BD read commands
                    handled from the command interrupt routine.

********************************************************************************/
BYTE bdFastCacheTransfer()
{
#if (CEProduct == 1)    
    ULONG preStartLBA,preLength;
#endif
    SearchCacheResultE  SearchResult;   /* Holds the result of a cache search   */
    ADDRESS_T temp;
    BYTE fPreReadIsStream;  //20090321_s.25==start==change reason, When Streaming read is switch from
                            //ON to OFF, clear the data in the buffer.

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
#if (CEProduct == 1)
    preStartLBA = HostXfer.StartBlock.lba;
    preLength = HostXfer.BlockCount.Long;
#endif    
    temp.lba = HostXfer.StartBlock.lba + HostXfer.BlockCount.Long;    // current cmd start lba of sequential read = pre-cmd start lba + pre-required length
    HostXfer.StartBlock.lba    = 0L;
    HostXfer.BlockCount.Long  = 0L;
    fPreReadIsStream = FALSE;
    switch (pdbGetDiscKind())
    {
        case eBDRDisc:
        case eBDRDLDisc:
        case eBDREDisc:
        case eBDREDLDisc:
            if (BdwtDisc.Stream & eBdReadStream)
                fPreReadIsStream = TRUE;
            break;

        case eBDROMDisc:
        case eBDROMDLDisc:
            if (bdDecFlag.fSTREAMING == TRUE)
                fPreReadIsStream = TRUE;
            break;

    }

    /*-------------------------------------------------------------------
        Get the requested host transfer length
    --------------------------------------------------------------------*/
    if ( Iram_Cmd_block.bf[0] == SBC_READ_10 )
    {
        HostXfer.BlockCount.byte.m = Iram_Cmd_block.bf[7];
        HostXfer.BlockCount.byte.l = Iram_Cmd_block.bf[8];
        if (fPreReadIsStream == TRUE)
        {
            if( BD_BUFFERING() == ON )
                bdStopDiscTransfer();

            ClearDataBuffer();
        }
    }
    else /* SBC_READ_12 */
    {
        HostXfer.BlockCount.byte.h = Iram_Cmd_block.bf[7];
        HostXfer.BlockCount.byte.m = Iram_Cmd_block.bf[8];
        HostXfer.BlockCount.byte.l = Iram_Cmd_block.bf[9];
        if ( (Iram_Cmd_block.bf[10] & 0x80 == 0x00) && (fPreReadIsStream == TRUE) )
        {
            if( BD_BUFFERING() == ON )
                bdStopDiscTransfer();

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
#if BDWT_FAST_CACHE
    if ( pdbGetDiscKind() == eBDRDisc || pdbGetDiscKind() == eBDRDLDisc ||
         pdbGetDiscKind() == eBDREDisc || pdbGetDiscKind() == eBDREDLDisc )
    {

#if (CEProduct == 1)//for CE BE playback       
        if ((BdwtDisc.CpuUsage8X) && (HostXfer.StartBlock.lba > 0x2100))
        {
            //send_msg5S(1,"CPU_dis8X");
            BdwtDisc.CpuUsage8X = FALSE;
        }

        //Here we monitor the Host's behavior.
        //If it is a random read, we should clear data buffer to prevent old data
        //being over-written by decoder pre-fetching.

        //Check for random read
        //If it's a non-sequential read and Not doing CpuUsage8X
//        send_msg5SValue(1,"templba",4,pdbGetBDClusterStart(temp.lba)+ONE_BD_CLUSTER);
//        send_msg5SValue(1,"Hxfrlba",4,HostXfer.StartBlock.lba);
//        send_msg5SValue(1,"Prelba",4,pdbGetBDClusterStart(preStartLBA));
//        send_msg5SValue(1,"1StaLBA",4,DataBuffer.CacheStartLBA);
//        send_msg5SValue(1,"1ectorC",4,DataBuffer.SectorCount);            
//        send_msg5SValue(1,"1ostPtr",4,DataBuffer.CurHostPtr);            

        if((HostXfer.StartBlock.lba != temp.lba) && (!BdwtDisc.CpuUsage8X))
        {
            //send_msg5S(1,"non-sequential");
            //                   |preStartLBA  ~  preStartLBA+ONE_BD_CLUSTER|
            // non-sequential    |<----- sequential read range  ----------->|  non-sequential
            if((pdbGetBDClusterStart(temp.lba)+ONE_BD_CLUSTER ) < pdbGetBDClusterStart(HostXfer.StartBlock.lba)
                || (pdbGetBDClusterStart(preStartLBA)) > pdbGetBDClusterStart(HostXfer.StartBlock.lba)
                )
            {
                if (BD_BUFFERING() == ON)
                {
                    bdStopDiscTransfer();
                    Svr_pause_reqire = TRUE;
                    send_msg5S(SEND_MRW_MSG,"PrebufStop-6");

                }
                BDInitCache(); //Clear Cache
                return (FALSE);
            }
//if prestart lba == current startlba, restore buffer count and point.
            if ((pdbGetBDClusterStart(preStartLBA)) == pdbGetBDClusterStart(HostXfer.StartBlock.lba))
            {
                DataBuffer.SectorCount  = DataBuffer.SectorCount + preLength;//
                DataBuffer.CacheStartLBA = (DataBuffer.CacheStartLBA - preLength);
                {
                    DataBuffer.CurHostPtr = (DataBuffer.CurHostPtr - preLength);
                    if(DataBuffer.CurHostPtr>BUF_TOTAL_BLK_COUNT)
                        DataBuffer.CurHostPtr = DataBuffer.CurHostPtr%BUF_TOTAL_BLK_COUNT;
                }
//                send_msg5SValue(1,"2StaLBA",4,DataBuffer.CacheStartLBA);
//                send_msg5SValue(1,"2ectorC",4,DataBuffer.SectorCount);            
//                send_msg5SValue(1,"2ostPtr",4,DataBuffer.CurHostPtr);            

            }
            else
            {
                send_msg5S(SEND_MRW_MSG,"nonstop");
                send_msg5SValue(SEND_MRW_MSG,"templba",4,pdbGetBDClusterStart(temp.lba)+ONE_BD_CLUSTER);
                send_msg5SValue(SEND_MRW_MSG,"Hxfrlba",4,HostXfer.StartBlock.lba);
                send_msg5SValue(SEND_MRW_MSG,"Prelba",4,pdbGetBDClusterStart(preStartLBA));
            }
        }
#else        
        if ((BdwtDisc.CpuUsage8X) && (HostXfer.StartBlock.lba > 0x2100))
        {
            //send_msg5S(1,"CPU_dis8X");
            BdwtDisc.CpuUsage8X = FALSE;
        }

        if((HostXfer.StartBlock.lba != temp.lba) && (!BdwtDisc.CpuUsage8X))
        {
            if ((DataBuffer.CacheStartLBA > HostXfer.StartBlock.lba) || (((temp.lba & BD_CLUSTER_MASK) + ONE_BD_CLUSTER) < HostXfer.StartBlock.lba))
            {
                if (BD_BUFFERING() == ON)
                    bdStopDiscTransfer();

                //send_msg5S(1,"bdstop");
                BDInitCache(); //Clear Cache
                return (CACHE_MISS);
            }
        }
#endif
        SearchResult = BdwtSearchCache(HostXfer);

        if (SearchResult == FULL_CACHE_HIT)
        {
            RandomRead = FALSE;
            return TRUE;
        }
        else
        {
            return FALSE;
        }
    }
    else
#endif
    SearchResult = bdSearchCache(HostXfer,IMMEDIATE_SEARCH_MODE);

    /*-----------------------------------------------------------------------
        Check if random read for BD.
        If the require data of current read cmd is overlaping pre-read cmd, it isn't
        RandomRead because of the pre-read cmd required data is still in the buffer possibly.
    -----------------------------------------------------------------------*/
    if((HostXfer.StartBlock.lba > temp.lba) || (HostXfer.StartBlock.lba < DataBuffer.CacheStartLBA))
        {
            RandomRead = TRUE;
        }
        else
        {
            RandomRead = FALSE;
        }
#if (SEQ_SEEK_SPEED_DOWN == 1)
    if (RandomRead)
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
    bdFullCacheHit();

    return TRUE;

}
/*******************************************************************************

    FUNCTION        bdSetSenseCode

    DESCRIPTION     Builds the appropriate sense code for the type of error
                    reported

    PARAMETERS      ErrorStatus - Type of error reported

********************************************************************************/
void bdSetSenseCode( DiscAccessStatusE ErrorStatus)
{

    switch( ErrorStatus )
    {

        case UNCOR_ECC_FAIL:    /* L-EC uncorrectable error */
            BUILD_SENSE(0x03,0x11,0x05,0x04);
            break;

        case UNAUTH_RD_FAIL:    /* Read of scrambled sector without authentication */
            BUILD_SENSE(0x05,0x6F,0x03,0x09);
            break;

        case SEQID_FAIL:        /* Block sequence error */
            BUILD_SENSE(0x03,0x14,0x04,0x05);
            break;

        case ID_FAIL:           /* ID, CRC or ECC error */
            //BUILD_SENSE(0x03,0x10,0x00,0x06);
            BUILD_SENSE(0x03,0x11,0x05,0x05); //* L-EC uncorrectable error
            break;

        case SEEK_FAIL:         /* No seek complete */
            BUILD_SENSE(0x03,0x02,0x00,0x07);
            break;

        case TIMEOUT:           /* Insufficient time for operation */
            BUILD_SENSE(0x03,0x11,0x05,0x07);
            break;

        case TID_FAIL:          /* Internal target failure */
            //BUILD_SENSE(0x04,0x44,0x00,0x10);
            BUILD_SENSE(0x03,0x11,0x01,0x10); // read retries exhausted
            break;

        case EDC_FAIL:          /* ID, CRC or ECC error */
            //BUILD_SENSE(0x03,0x10,0x00,0x08);
            BUILD_SENSE(0x03,0x11,0x05,0x06); //* L-EC uncorrectable error
            break;

        case SYNC_LOST_FAIL:    /* Re-synchronization error  */
            BUILD_SENSE(0x03,0x11,0x07,0x09);
            break;

        case EJECT_PRESS:       /* TRAY OPEN */
            BUILD_SENSE(0x02,0x3A,0x02,0x02);       //==start==change reason
                                                    //For WHQL Optical Logo test, 0x02/0x3A/0x02 is expected
                                                    //when tray is open.
            break;
        default:                /* Unrecovered read error */
            BUILD_SENSE(0x03,0x11,0x00,0x10);
    }
}


/*******************************************************************************

    FUNCTION        bdValidReadRequest

    DESCRIPTION     Determines if read request is valid

    PARAMETERS      Uses global parameter HostXfer

    RETURNS         TRUE    Valid read range from starting lba to end lba
                    FALSE   Invalid read range from starting lba to end lba

********************************************************************************/
BYTE bdValidReadRequest()
{
    BYTE Status;
    ULONG bdEndLBA;

    Status = TRUE;

    // Test if disc is blank
    if (St_blank_disc == TRUE)
        return FALSE;

    // Calculate ending LBA of the host transfer
    bdEndLBA = HostXfer.StartBlock.lba + HostXfer.BlockCount.Long - 1L;

    if (pdbGetDiscStatus() == eFinalizedDisc)
    {
        // Check end LBA of request
        if (bdEndLBA > pdbGetBDMaxLBA())
            Status = FALSE;
    }

     return Status;
}

#endif // BD_ENABLE
