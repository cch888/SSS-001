/****************************************************************************
*                (c) Copyright 2000 - 2007 SUNEXT DESIGN, INC.
*                           All Right Reserved
*
*
*   FILE NAME:     dvBufMgr.c
*
*   DESCRIPTION:   Manages the DVD buffer. Provides routines for accessing
*                  and updating buffer information and status.
*
*   $Revision: 86 $
*   $Date: 10/12/21 4:17p $
*
**************************** SUNEXT CONFIDENTIAL *******************************/


/*****************************************************************************
    INCLUDE FILES
******************************************************************************/
#include ".\common\globals.h"
#include ".\common\globtype.h"
#include ".\common\vu_cfg.h"
#include ".\hif\gp_ram.h"
#include ".\hif\rd_util.h"
#include ".\dvd\dvDef.h"
#include ".\dvd\dvMacros.h"
#include ".\dvd\dvGvars.h"
#include ".\dvd\dvDiscRd.h"
#include ".\dvd\dvBufMgr.h"
#include ".\dvd\dvHostRd.h"
#include ".\dvd\dvdiscinit.h"
#include ".\dvd\dvutil.h"
#if (DVD_RAM_READ == 1)
#include ".\defmgmt\dvdram\dvDefMgr.h"
#include ".\defmgmt\dvdram\ramread.h"
#include ".\defmgmt\common\mrw_ram.h"
#endif
#include ".\al\reg_intctrl.h"
#include ".\al\REG_dvd_decoder.h"
#if (OPEN_SESSION==1)
#include ".\servo\svoVar.h"
#endif
/*****************************************************************************
    PROTOTYPES
******************************************************************************/
void dvdValidateCache(void);
void dvdInvalidateCache(void);
void dvdInitializeHostBuffer(ULONG);
void dvdCheckReadAhead(void);
void dvdAdjustHostBufferPtr(ULONG);
void dvdTestZones(void);

SearchCacheResultE dvdSearchCache(TransferS, BYTE);
static ULONG dvdGetBlockRequest(TransferS, TransferTypeE);

/*****************************************************************************
    VARIABLES
******************************************************************************/
#if (DVD_RAM_READ == 1)
ULONG dvdramReadAheadStartLSN;
ULONG dvdramDiscRequestStartLSN;
#endif

/*******************************************************************************
* FUNCTION DEFINITIONS
*******************************************************************************/

/*******************************************************************************

    FUNCTION        dvdSearchCache

    DESCRIPTION     Searches the DVD cache to determine if the requested host
                    sectors are within the current buffer

    PARAMETERS      Request -   TransferS structure containing the starting LBA
                                and transfer count

                    Mode    -   0 = Wait mode. In this mode, if buffering is
                                on and the next block to be buffered is the
                                requested block the function will wait
                                until the block is buffered or buffering stops
                                before reporting cache status

                                1 = Immediate mode. In this mode if the requested
                                block is not in the cache even if buffering is
                                on, a cache miss is reported

    RETURNS         Either CACHE_MISS, FULL_CACHE_HIT, or PARTIAL_CACHE_HIT

********************************************************************************/
SearchCacheResultE dvdSearchCache(TransferS Request, BYTE mode)
{

    ULONG EndLBA;
    ULONG dvdStartBlock;
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
                if (((PreATAPICMD == SBC_READ_10)||(PreATAPICMD == SBC_READ_12))&&(PreReadCMDLba<=Request.StartBlock.lba)&&(Request.StartBlock.lba<dvdStartBlock))
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

/*******************************************************************************

    FUNCTION        dvdAdjustHostBufferPtr

    DESCRIPTION     Adjusts the host buffer pointer (DataBuffer.CurHostPtr) to
                    the starting sector that will be transferred to the host.

    RETURNS         Number of freed blocks from the buffer after the host
                    pointer has been advanced.

    NOTES           DataBuffer.SectorCount is modified when DataBuffer.CurHostPtr
                    is adjusted. When CurHostPtr is advanced to the starting sector
                    of a transfer, SectorCount will be decremented for each
                    advanced sector. Likewise SectorCount is increased as the
                    CurHostPtr is adjusted backward within the cache.

                    The host pointer adjustement will never decrement below
                    sector 0 of the ECC block address that DataBuffer.CurHostPtr
                    points to.

********************************************************************************/
void dvdAdjustHostBufferPtr( ULONG StartLBA)
{
    BYTE    InterruptStatus;
    USHORT  Displacement;

    /*-------------------------------------------------------------------
        If the StartLBA is not the next lba after the last transferred
        lba, determine where the StartLBA is within the buffer and adjust
        the HostBufferIndex to that position. Then update the
        SectorsAvailable count accordingly.
    --------------------------------------------------------------------*/
    /*-------------------------------------------------------------------
       Copy interrupt status and Disable Interrupts
    --------------------------------------------------------------------*/
    InterruptStatus =   GET_INT_ENABLE_MASK();
    DISABLE_INTERRUPT();


    if (StartLBA == DataBuffer.CacheEndLBA)
    {
        DataBuffer.CurHostPtr += (DataBuffer.CacheEndLBA - DataBuffer.CacheStartLBA);
        DataBuffer.SectorCount   = 1;
    }
    else if (StartLBA < DataBuffer.CacheStartLBA)
    {
        Displacement = (DataBuffer.CacheStartLBA - StartLBA);
        DataBuffer.CurHostPtr   -= Displacement;
        DataBuffer.SectorCount  += Displacement;
    }
    else /* (StartLBA > DataBuffer.CacheStartLBA) */
    {
        Displacement = StartLBA - DataBuffer.CacheStartLBA;
        DataBuffer.CurHostPtr   += Displacement;
        DataBuffer.SectorCount  -= Displacement;
    }

    /*-------------------------------------------------------------------
        Insure CurHostPtr wraps to proper buffer location if it exceeds
        the buffer size
    --------------------------------------------------------------------*/
        DataBuffer.CurHostPtr %= MAX_DVD_SECTORS;

    /*-------------------------------------------------------------------
        Set the new start of cache.
    --------------------------------------------------------------------*/
        DataBuffer.CacheStartLBA = StartLBA;

    /*-------------------------------------------------------------------
        Reload previous interrupt status
    --------------------------------------------------------------------*/
    SET_INT_ENABLE_MASK( InterruptStatus );

}
/*******************************************************************************

    FUNCTION        dvdInitializeHostBuffer()

    DESCRIPTION     Places the buffer in a static state by halting the buffering
                    if on, clearing the buffer and initializing the start and
                    end values of the cache

    PARAMETERS      StartLBA    Used to initial the start and end of the cache

********************************************************************************/
void dvdInitializeHostBuffer( ULONG StartLBA )
{
    ULONG StartBlock;

    /*---------------------------------------------------------------
        If buffering is on stop transferring data to buffer
    ---------------------------------------------------------------*/
    if (DVD_BUFFERING() == ON)
        dvdStopDiscTransfer();

    /*---------------------------------------------------------------
        Clear the buffer and mark no host blocks are in the buffer
    ---------------------------------------------------------------*/
    ClearDataBuffer();
    dvdDecFlag.fREAD_AHEAD = FALSE;

    /*---------------------------------------------------------------
        Set flag for the first ECC block to be loaded into the
        buffer. Once the block is loaded and this flag is TRUE
        the host pointer will be adjusted to the starting lba within
        the ecc block. This is done in the ECC block interrupt
        service routine dvdCheckForHostTransfer().
    ----------------------------------------------------------------*/
    dvdDecFlag.fFIRST_HOST_BLOCK = TRUE;

    /*---------------------------------------------------------------
        Initialize the start and end of cache lba values. When the
        first block comes in CacheEndLBA is updated.
    ----------------------------------------------------------------*/
    DataBuffer.CacheStartLBA = StartLBA;
    StartBlock = dvdGetBlockAddress(StartLBA);
    if (StartBlock != 0L)
        DataBuffer.CacheEndLBA   =  StartBlock - 1L;

}

/*******************************************************************************

    FUNCTION        dvdGetBlockRequest

    DESCRIPTION     Returns the number of dvd blocks to be loaded into the
                    buffer based on the given request (LBA and Length) and load type

    PARAMETERS      HostRequest     Transfer structure cotaining request information
                    LoadType        Type of disc transfer to be performed


    RETURNS         BlockRequest    Number of DVD blocks requested for loading into
                                    the buffer

********************************************************************************/
static ULONG dvdGetBlockRequest(TransferS Request, TransferTypeE LoadType)
{
    ULONG   RequestStartBlock;   /* Starting dvd block of the request (LBA) */
    ULONG   RequestEndBlock;    /* Ending dvd block of the request   (LBA) */
    ULONG   BlockRequest;       /* Number of blocks to buffer */
    ULONG   BlocksLeft;
    ULONG   LastBufferBlock;
    ULONG   MaxRecordedLBA;
    PSNs    StartPSN;
    BYTE    temp, track;
    PSNs    MaxRecordedPSN;     /* The current Maximum recorded PSN */

    BlockRequest = 0L;

    /*---------------------------------------------------------------
        Determine the starting and ending dvd blocks of the request
        (These addresses are the LBA of the first sector of each block)
    ----------------------------------------------------------------*/
    RequestStartBlock = dvdGetBlockAddress(Request.StartBlock.lba);
    RequestEndBlock = dvdGetBlockAddress(Request.StartBlock.lba + Request.BlockCount.Long - 1L);

    track = pdbGetDVDHighestRecordedTrackNumber();

    if (pdbGetDVDTrackState(track) == eTRACK_STATE_INCOMPLETE)
        //MaxRecordedLBA = pdbGetDVDTrackNextWritableAddress(track) - 1L;
        MaxRecordedLBA = dvdConvertPSNtoLBA(pdbGetDVDTrackRecordedPSN(track));
    else
        MaxRecordedLBA = pdbGetDVDMaxLBA();

    if( (LoadType == INIT_READ) || (LoadType == INIT_READ_AHEAD) )
    {

        /*---------------------------------------------------------------
            Get the number of DVD blocks needed to satisfy the request
        ----------------------------------------------------------------*/
        BlockRequest = ((RequestEndBlock - RequestStartBlock) / SECTORS_PER_DVDBLOCK) + 1L;

        /*---------------------------------------------------------------
            If this is a init read without css in progress set up
            starting location of read ahead
        ----------------------------------------------------------------*/
        if ((LoadType == INIT_READ) && (dvdCss.CssFlag.AgidInUse == FALSE))
        {
            /*---------------------------------------------------------------
                Calculate the PSN of the first read ahead block. This check
                is done in the DVD block ISR to indicate when read ahead
                blocks are being bufferred

                PSN of first read ahead block = request end block + ONE_DVD_BLOCK
            ----------------------------------------------------------------*/
            if (pdbGetDVDLayers() == DUAL_LAYER)
            {
                if((RequestEndBlock + ONE_DVD_BLOCK) <= pdbGetDVDEndLBALayer0())
                {
                    StartPSN = dvdConvertLBAtoPSN(RequestEndBlock + ONE_DVD_BLOCK);
                    dvdReadAheadStartPSN = StartPSN.PSN;

                    /*---------------------------------------------------------------
                        If read ahead is enable add pre-fetch blocks to the count
                        ----------------------------------------------------------------*/
#if (DVD_READ_AHEAD_ENABLED == TRUE)
                    if ((RequestEndBlock +(MAX_DVD_SECTORS - DVD_PAD_SECTORS)) <= pdbGetDVDEndLBALayer0())
                    {
                        if ((RequestEndBlock + MAX_DVD_SECTORS - DVD_PAD_SECTORS) >= MaxRecordedLBA)
                        {
                            BlockRequest += (MaxRecordedLBA - RequestEndBlock)/SECTORS_PER_DVDBLOCK;
                            dvdDecFlag.fDISABLE_READ_AHEAD = TRUE;
                            /* For correct drive hang-up by the wrong dvdReadAheadPSN */
                                MaxRecordedPSN = dvdConvertLBAtoPSN(dvdGetBlockAddress(MaxRecordedLBA));
                                if (dvdReadAheadStartPSN >  MaxRecordedPSN.PSN)
                                    StartPSN = dvdConvertLBAtoPSN(dvdGetBlockAddress(MaxRecordedLBA+1));
                            /* For correct drive hang-up by the wrong dvdReadAheadPSN */
                            dvdReadAheadStartPSN = StartPSN.PSN;
                        }
                        else
                            BlockRequest = (MAX_DVD_BLOCKS - DVD_PAD_BLOCKS);
                    }
                    else
                    {
                        if (MaxRecordedLBA < pdbGetDVDEndLBALayer0())
                        {
                            if (dvdGetBlockAddress(MaxRecordedLBA) >= RequestEndBlock)
                                BlockRequest += (MaxRecordedLBA - RequestEndBlock)/SECTORS_PER_DVDBLOCK;

                            dvdDecFlag.fDISABLE_READ_AHEAD = TRUE;
                            StartPSN = dvdConvertLBAtoPSN(dvdGetBlockAddress(MaxRecordedLBA+1));
                            dvdReadAheadStartPSN = StartPSN.PSN;
                        }
                        else
                            BlockRequest += (pdbGetDVDEndLBALayer0() - RequestEndBlock)/SECTORS_PER_DVDBLOCK;
                    }
#endif
                }
                else if(RequestStartBlock > pdbGetDVDEndLBALayer0())
                {
                    //if((RequestEndBlock + ONE_DVD_BLOCK) <= pdbGetDVDMaxLBA())
                    if ((RequestEndBlock + ONE_DVD_BLOCK) <= MaxRecordedLBA)
                    {
                        StartPSN = dvdConvertLBAtoPSN(RequestEndBlock + ONE_DVD_BLOCK);
                        dvdReadAheadStartPSN = StartPSN.PSN;
                        /*---------------------------------------------------------------
                        If read ahead is enable add pre-fetch blocks to the count
                        ----------------------------------------------------------------*/
#if (DVD_READ_AHEAD_ENABLED == TRUE)
                            if ((MAX_DVD_BLOCKS - DVD_PAD_BLOCKS) > (MaxRecordedLBA - RequestEndBlock)/SECTORS_PER_BLOCK)
                            {
                                BlockRequest += (MaxRecordedLBA - RequestEndBlock)/SECTORS_PER_BLOCK;
                                dvdDecFlag.fDISABLE_READ_AHEAD = TRUE;
                                /* For correct drive hang-up by the wrong dvdReadAheadPSN */
                                MaxRecordedPSN = dvdConvertLBAtoPSN(dvdGetBlockAddress(MaxRecordedLBA));
                                if (dvdReadAheadStartPSN >  MaxRecordedPSN.PSN)
                                    StartPSN = dvdConvertLBAtoPSN(dvdGetBlockAddress(MaxRecordedLBA+1));
                                /* For correct drive hang-up by the wrong dvdReadAheadPSN */
                                dvdReadAheadStartPSN = StartPSN.PSN;
                            }
                            else
                                BlockRequest = (MAX_DVD_BLOCKS - DVD_PAD_BLOCKS);
#endif
                    }
                    else
                    {
                        dvdDecFlag.fDISABLE_READ_AHEAD = TRUE;
                        StartPSN = dvdConvertLBAtoPSN(RequestEndBlock);
                        dvdReadAheadStartPSN = StartPSN.PSN;
                    }
                }
                else
                {
                     dvdDecFlag.fDISABLE_READ_AHEAD = TRUE;
                     StartPSN = dvdConvertLBAtoPSN(RequestEndBlock);
                     dvdReadAheadStartPSN = StartPSN.PSN;
                }
            }
            else
            {
                if((RequestEndBlock + ONE_DVD_BLOCK) <= MaxRecordedLBA)
                {
                    StartPSN = dvdConvertLBAtoPSN(RequestEndBlock + ONE_DVD_BLOCK);
                    dvdReadAheadStartPSN = StartPSN.PSN;

#if (DVD_READ_AHEAD_ENABLED == TRUE)
                    /*---------------------------------------------------------------
                        If read ahead is enable add pre-fetch blocks to the count
                    ----------------------------------------------------------------*/
                        if ((MAX_DVD_BLOCKS - DVD_PAD_BLOCKS) > (MaxRecordedLBA - RequestEndBlock)/SECTORS_PER_BLOCK)
                        {
                            BlockRequest += (MaxRecordedLBA - RequestEndBlock)/SECTORS_PER_BLOCK;
                            dvdDecFlag.fDISABLE_READ_AHEAD = TRUE;
                            /* For correct drive hang-up by the wrong dvdReadAheadPSN */
                            MaxRecordedPSN = dvdConvertLBAtoPSN(dvdGetBlockAddress(MaxRecordedLBA));
                            if (dvdReadAheadStartPSN >  MaxRecordedPSN.PSN)
                                StartPSN = dvdConvertLBAtoPSN(dvdGetBlockAddress(MaxRecordedLBA+1));
                            /* For correct drive hang-up by the wrong dvdReadAheadPSN */
                            dvdReadAheadStartPSN = StartPSN.PSN;
                        }
                        else
                            BlockRequest = (MAX_DVD_BLOCKS - DVD_PAD_BLOCKS);
#endif
                }
                else
                {
                    dvdDecFlag.fDISABLE_READ_AHEAD = TRUE;
                    StartPSN = dvdConvertLBAtoPSN(RequestEndBlock);
                    dvdReadAheadStartPSN = StartPSN.PSN;
                }
            }
        }
    }
    else if (LoadType == EXTEND_READ)
    {
        /*-------------------------------------------------------------------
           Determine number of blocks left to buffer and the last block
           to be bufferred. Disable interrupts since buffering is assumed to
           be on and values could be updated.
        --------------------------------------------------------------------*/
        temp =   GET_INT_ENABLE_MASK();
        DISABLE_INTERRUPT();

        BlocksLeft = ((dvdDiscRequest.EccBlockCount - dvdDiscRequest.EccBlocksBuffered) * SECTORS_PER_DVDBLOCK);
        LastBufferBlock = dvdGetBlockAddress(DataBuffer.CacheEndLBA) + BlocksLeft;

        SET_INT_ENABLE_MASK( temp );

        if (RequestEndBlock <= LastBufferBlock)
            BlockRequest = (MAX_DVD_BLOCKS - DVD_PAD_BLOCKS - 1) - ((LastBufferBlock - RequestEndBlock)/SECTORS_PER_DVDBLOCK);
        else
            BlockRequest = (MAX_DVD_BLOCKS - DVD_PAD_BLOCKS - 1) + ((RequestEndBlock - LastBufferBlock)/SECTORS_PER_DVDBLOCK);


        if (RequestStartBlock < pdbGetDVDEndLBALayer0())
        {
            if (MaxRecordedLBA > pdbGetDVDEndLBALayer0())
            {
                if ((LastBufferBlock + BlockRequest * SECTORS_PER_DVDBLOCK) > pdbGetDVDEndLBALayer0())
                {
                    if (LastBufferBlock <= pdbGetDVDEndLBALayer0())
                        BlockRequest = (pdbGetDVDEndLBALayer0() - LastBufferBlock)/SECTORS_PER_DVDBLOCK;
                    else
                        BlockRequest = 0;

                    if (BlockRequest == 0)
                        dvdDecFlag.fDISABLE_READ_AHEAD = TRUE;
                }
            }
            else
            {
                if ((LastBufferBlock + BlockRequest * SECTORS_PER_DVDBLOCK) > MaxRecordedLBA)
                {
                    if (LastBufferBlock <= MaxRecordedLBA)
                        BlockRequest = (MaxRecordedLBA - LastBufferBlock)/SECTORS_PER_DVDBLOCK;
                    else
                        BlockRequest = 0;

                    dvdDecFlag.fDISABLE_READ_AHEAD = TRUE;
                }
            }
        }
        else
        {
            if ((LastBufferBlock + BlockRequest * SECTORS_PER_DVDBLOCK) > MaxRecordedLBA)
            {
                if (LastBufferBlock <= MaxRecordedLBA)
                    BlockRequest = (MaxRecordedLBA - LastBufferBlock)/SECTORS_PER_DVDBLOCK;
                else
                    BlockRequest = 0;

                if (BlockRequest == 0)
                    dvdDecFlag.fDISABLE_READ_AHEAD = TRUE;
            }
        }

        /*---------------------------------------------------------------
            If buffering is on then current blocks being bufferred may be
            host blocks. If the last block of the request is not in the
            buffer not all host blocks have been buffered. Inidicate which
            is the last PSN of the requested host blocks.
         ----------------------------------------------------------------*/
        if (DVD_BUFFERING() == ON)
        {

                /*---------------------------------------------
                    When ECC interrupt comes in for the last host block,
                    Set read ahead to FALSE and will not to be TRUE
                    Must disable interrupt to avoid this situation
                --------------------------------------------*/
            temp =   GET_INT_ENABLE_MASK();
            DISABLE_INTERRUPT();

            if (RequestEndBlock > DataBuffer.CacheEndLBA)
            {
                dvdDecFlag.fREAD_AHEAD = FALSE;
                StartPSN = dvdConvertLBAtoPSN(RequestEndBlock + ONE_DVD_BLOCK);
                dvdReadAheadStartPSN = StartPSN.PSN;
            }

            SET_INT_ENABLE_MASK( temp );
        }
    }
    else if (LoadType == EXTEND_READ_AHEAD)
    {
        /*---------------------------------------------------------------
            For an extend read ahead just determine the number of read
            ahead blocks to extend
        ----------------------------------------------------------------*/
        BlockRequest = (Request.BlockCount.Long/SECTORS_PER_DVDBLOCK);

    }

    if(BlockRequest > (MAX_DVD_BLOCKS - DVD_PAD_BLOCKS))
        BlockRequest = (MAX_DVD_BLOCKS - DVD_PAD_BLOCKS);

    return (BlockRequest);

}

/*******************************************************************************

    FUNCTION        dvdCheckReadAhead

    DESCRIPTION     Determines if pre-fetching can be initiated. Will turn on
                    buffering if buffering is off. If the buffering is on
                    a determination is made to extend the current read

********************************************************************************/
extern BYTE CmdSeekOk;
void dvdCheckReadAhead()
{

    TransferS   Request;            /* Logical request for pre-fetch */
    BYTE        InterruptStatus;    /* Temp interrupt status */
    ULONG       BlocksRemaining;    /* Number of dvd blocks remaining to be buffered */
    BYTE        LoadType;           /* type of buffer loading action */
    WORD        SectorCount;        /* Saved buffer count */
    ULONG       MaxRecordedLBA;
    BYTE        track;

#if (E_DIAGS == 1)
    if (READ_PI_PO == 1) return;
#endif

    if (dvdDecFlag.fDISABLE_READ_AHEAD)
        return;

    /*-------------------------------------------------------------------
        Do not start read ahead if eject has been issued
    --------------------------------------------------------------------*/
    if (EjectKeyInfo.PressedValid || TrayManagerInfo.Executing)
        return;

#if( DVD_ZONE_TEST == TRUE )
    if( dvdTest.TestComplete != TRUE )
    {
        dvdTestZones();
        return;
    }
#endif
    /*-------------------------------------------------------------------
        when excuting DVD Seek Command, do not enable read ahead
    --------------------------------------------------------------------*/
    if(CmdSeekOk != 0)
        return;

#if (RANDOM_READ_BYPASS_CHECK_METHOD == 1)
    if (RandomCount > CONTINUE_RANDOM_READ_CNT)
    {
        send_msg80(SEND_MRW_MSG,0x710100);
        return;
    }
#endif

#if (DVD_DISABLE_CSS_CHECK == FALSE)
    /*-------------------------------------------------------------------
      If disc is copy protected and CSS authentication has not passed
      do not pre-fetch
    --------------------------------------------------------------------*/
    #if (ENABLE_CPRM == 1 && BCA_ENABLE == 1)
    if ( (pdbDVDDiscCSS() == eCSS_CPS) && (dvdCss.CssFlag.AuthenticationSuccess == FALSE) )
    #else
    if ( (pdbDVDDiscCSS() == TRUE) && (dvdCss.CssFlag.AuthenticationSuccess == FALSE) )
    #endif
        return;
#endif

    if ((DVD_BUFFERING() == OFF) || (dvdBlockInterruptTimer.fTIMEOUT == TRUE) )
    {

        /*-------------------------------------------------------------------
           If a time out has occured insure that all decoding
           stops including the clearing of interrupts and status flags.
        --------------------------------------------------------------------*/
        if (dvdBlockInterruptTimer.fTIMEOUT == TRUE)
            dvdStopDiscTransfer();

        /*-------------------------------------------------------------------
           Because the DataBuffer.SectorCount may modify in dvdSearchCache()
           that called in interrupt service routine, we must disable interrupt
           before using it.
        --------------------------------------------------------------------*/
        InterruptStatus =   GET_INT_ENABLE_MASK();
        DISABLE_INTERRUPT();

        SectorCount = DataBuffer.SectorCount;

        SET_INT_ENABLE_MASK( InterruptStatus );

        /*-------------------------------------------------------------------
           If buffer is full exit. Subtract one dvd block to compensate
           a partial ecc block in the buffer.

           Example: (MAX_DVD_SECTORS - DVD_PAD_SECTORS) = 200h.  If
           SectorCount = 1F3h, the buffer is full since a partial takes
           a full ecc block location. Therfore
            if (1F3h > (200h - 10h) or (1F3 > 1F0) return since buffer is full

        --------------------------------------------------------------------*/
        if ( SectorCount > (MAX_DVD_SECTORS - DVD_PAD_SECTORS - ONE_DVD_BLOCK))
            return;

        /*-------------------------------------------------------------------
            Get the next starting address to initiate buffering. If this is
            an initial read (after startup and read lead in) CacheEndLBA + 1
            = LBA 0x01 which will load the DVD block starting at LBA 0x000
        --------------------------------------------------------------------*/
#if (DVD_RAM_READ == 1)
        if ((pdbDiscLoaded() == DVD_DISC) && (pdbGetDVDType() == DVD_RAM))
        {
            InterruptStatus =   GET_INT_ENABLE_MASK();
            DISABLE_INTERRUPT();

            if (DataBuffer.CacheEndLBA == 0L) //may be re-set by read command
            {
                dvdDecFlag.fDISABLE_READ_AHEAD = TRUE;
                send_msg80(SEND_MRW_MSG,0x71,0x00,27);
                SET_INT_ENABLE_MASK( InterruptStatus );
                return;
            }
            SET_INT_ENABLE_MASK( InterruptStatus );
        }
#endif

        if (DataBuffer.CacheEndLBA == 0L)
            Request.StartBlock.lba = 0L;
        else
            Request.StartBlock.lba   = DataBuffer.CacheEndLBA + 1L ;

        /*-------------------------------------------------------------------
           If attempting to read the same block exceedes the maximum
           attempts allowed then do not initiate read ahead
        --------------------------------------------------------------------*/
        if( Request.StartBlock.lba == dvdLastReadAheadTarget)
        {
            if (dvdReadAheadTargetRetries == 0)
                return;

            --dvdReadAheadTargetRetries;
        }
        else
        {
            dvdLastReadAheadTarget      = Request.StartBlock.lba;
            dvdReadAheadTargetRetries   = MAX_DVD_READ_RETRIES;
            if ((pdbDiscLoaded() == DVD_DISC) && (pdbGetDVDType() == DVD_RAM))
                dvdReadAheadTargetRetries   = 0;
        }

        /*-------------------------------------------------------------------
           Set the buffer load type and turn read ahead flag on
        --------------------------------------------------------------------*/
        LoadType                 = INIT_READ_AHEAD;
        send_msg80(SEND_READ_DVD_MSG,0x710101);
        dvdDecFlag.fREAD_AHEAD  = TRUE;

    }
    else
    {
        /*-------------------------------------------------------------------
           Copy interrupt status and Disable Interrupts
        --------------------------------------------------------------------*/
        InterruptStatus =   GET_INT_ENABLE_MASK();
        DISABLE_INTERRUPT();

        /*-------------------------------------------------------------------
            Determine the sector count if all remaining blocks to be
            bufferred come in.
        --------------------------------------------------------------------*/
        BlocksRemaining = dvdDiscRequest.EccBlockCount - dvdDiscRequest.EccBlocksBuffered;
        SectorCount = DataBuffer.SectorCount + (WORD)(BlocksRemaining * SECTORS_PER_DVDBLOCK);

        Request.StartBlock.lba = DataBuffer.CacheEndLBA + 1;
        /*-------------------------------------------------------------------
            Reload previous interrupt status
        --------------------------------------------------------------------*/
        SET_INT_ENABLE_MASK( InterruptStatus );

        /*-------------------------------------------------------------------
            If the presumed sector count equals or exceeds the buffer size
            do not extend.

           NOTE: Add one dvd block to sector count to compensate for a
           partial DVD block
        --------------------------------------------------------------------*/
        if ( SectorCount > (MAX_DVD_SECTORS - DVD_PAD_SECTORS - ONE_DVD_BLOCK))
            return;
        else
        {
            LoadType = EXTEND_READ_AHEAD;
        }

    }

    /*-------------------------------------------------------------------
        Calculate the transfer count. For Read Ahead there are no
        partial blocks in the request so mask out the lower nibble
    --------------------------------------------------------------------*/
    Request.BlockCount.Long = ((MAX_DVD_SECTORS - DVD_PAD_SECTORS - SectorCount) & DVD_BLOCK_MASK);

    track = pdbGetDVDHighestRecordedTrackNumber();

    if (pdbGetDVDTrackState(track) == eTRACK_STATE_INCOMPLETE)
        MaxRecordedLBA = dvdConvertPSNtoLBA(pdbGetDVDTrackRecordedPSN(track));
    else
        MaxRecordedLBA = pdbGetDVDMaxLBA();

    if (Request.StartBlock.lba > MaxRecordedLBA) return;

    if((Request.StartBlock.lba + Request.BlockCount.Long) > MaxRecordedLBA)
    {
        Request.BlockCount.Long = MaxRecordedLBA - Request.StartBlock.lba;
    }

    if((Request.StartBlock.lba <= pdbGetDVDEndLBALayer0()) && (pdbGetDVDLayers() == DUAL_LAYER)
        && (pdbGetDVDTrackPath() == DVDHD_OTP_DISC)) // ABEX disc occurs mosatic problem in layer jump reading.
    {
        ULONG RequestEndBlklba;
        ULONG BlockLong;
        if( LoadType == EXTEND_READ_AHEAD)
        {
            RequestEndBlklba = Request.StartBlock.lba + (BlocksRemaining * SECTORS_PER_DVDBLOCK) - 1;
            BlockLong = pdbGetDVDEndLBALayer0() - RequestEndBlklba;
            if(Request.BlockCount.Long > BlockLong)
            {
                Request.BlockCount.Long = BlockLong;
            }
        }
        if((Request.StartBlock.lba + Request.BlockCount.Long) > pdbGetDVDEndLBALayer0())
        {
            Request.BlockCount.Long = pdbGetDVDEndLBALayer0() - Request.StartBlock.lba;
        }
    }
    /*-------------------------------------------------------------------
        If the requested block count does not equal 0 request
        add more blocks to the buffer
    --------------------------------------------------------------------*/
    if( Request.BlockCount.Long != 0 )
    {
        /*---------------------------------------------------
            During servo STOP mode, and get some command.
            Enable Clock Register before Servo Wake Up
                to make sure setting decoder/encoder register works!
        ----------------------------------------------------*/
         if(Iram_Svr_player_mode == STOP_MODE)
            vWakeUpFromPowerSaveMode();

#if (EN_POWER_CONTROL == 1)
        if (CurrentPowerMode == eStandbyPower)
        {
            SetDiscReadPowerMode();
        }
#endif

        send_msg5SValue(SEND_READ_DVD_MSG,"*READ_AHEAD*",1,LoadType);

        dvdLoadBuffer( Request, LoadType );
    }

    return;
}

/*******************************************************************************

    FUNCTION        dvdLoadBuffer

    DESCRIPTION     Translates the host request to a DVD ecc block based request
                    and then initiates the disc transfer.

    PARAMETERS      HostRequest     Transfer structure cotaining host request information
                    LoadType        Type of disc transfer to be performed

    GLOBAL UPDATES  dvdDiscRequest

    RETURNS         DiscReadStatus  Status of the disc read / buffer operation

********************************************************************************/
DiscAccessStatusE dvdLoadBuffer(TransferS Request, TransferTypeE LoadType)
{

    ULONG   BlockRequest;       /* Number of blocks to buffer */
    ULONG   BlockAddress;        /* Temp variable */
    PSNs    StartPSN;           /* Starting PSN of the requested blocks */
    PSNs    LastPSN;            /* Last PSN of the requested blocks */
    ULONG   SavedBlockRequest;  /* Truncated block request if in a layer crossing */
    ULONG   MaxPSN;             /* Maximum PSN value in layer */
    DiscAccessStatusE   DiscReadStatus;     /* Status of the DVD disc access */
    ULONG   BlocksLeft;
    ULONG   LastBufferBlock;
    BYTE    temp;

#if (OPEN_SESSION == 1)
    RF_search_count = 0;//clear on rf counter when begin new read process
#endif
    /*---------------------------------------------------------------
        Check what kind of disc is inserted to pick the proper
        functions to do the logical to physical translations
    ----------------------------------------------------------------*/

    /*---------------------------------------------------------------
        Determine the number of DVD blocks to load
    ----------------------------------------------------------------*/
    BlockRequest = dvdGetBlockRequest(Request, LoadType);

    /*---------------------------------------------------------------
       Convert the start address of the request to PSN and layer
    ----------------------------------------------------------------*/
    BlockAddress = dvdGetBlockAddress(Request.StartBlock.lba);
    StartPSN = dvdConvertLBAtoPSN(BlockAddress);

    /*---------------------------------------------------------------
       Convert the end address of the request to PSN and layer
    ----------------------------------------------------------------*/
    if (BlockRequest > 0)
        BlockAddress  = (BlockAddress + (BlockRequest * SECTORS_PER_DVDBLOCK)) - 1L;

    if(BlockAddress > pdbGetDVDMaxLBA())
        LastPSN  = dvdConvertLBAtoPSN(pdbGetDVDMaxLBA());
    else
        LastPSN  = dvdConvertLBAtoPSN(BlockAddress);

    /*---------------------------------------------------------------
       Determine maximum PSN of the layer that LastPSN is within
    ----------------------------------------------------------------*/
    if (LastPSN.Layer == eLayer1)
        MaxPSN = pdbGetDVDMaxPSN(1);
    else
        MaxPSN = pdbGetDVDMaxPSN(0);


   if( StartPSN.Layer == LastPSN.Layer )
   {
        if( (LoadType == EXTEND_READ) || (LoadType == EXTEND_READ_AHEAD) )
        {
            dvdDiscRequest.Type = LoadType;
            dvdDiscRequest.ExtendBlocks = (BYTE) BlockRequest;

        }
        else
        {

            /*---------------------------------------------------------------
               Insure that the request will not extend beyond the maximum
               capcity of the disc/layer
            ----------------------------------------------------------------*/
            if ( LastPSN.PSN > MaxPSN )
                BlockRequest = ((MaxPSN + 1L) - StartPSN.PSN) / ONE_DVD_BLOCK;

            /*---------------------------------------------------------------
                Set up the Disc Transfer control parameters
            ----------------------------------------------------------------*/
            dvdDiscRequest.Type            = LoadType;
            dvdDiscRequest.TargetID.Long   = StartPSN.PSN;
            dvdDiscRequest.TargetLayer     = StartPSN.Layer;
            dvdDiscRequest.EccBlockCount   = BlockRequest;
        }
        /*---------------------------------------------------------------
            Call the main DVD disc transfer engine
        ----------------------------------------------------------------*/
        DiscReadStatus = dvdStartDiscTransfer();
    }
    else /* (StartPSN.Layer != LastPSN.Layer) */
    {
        /*---------------------------------------------------------------
            Layer crossing is in this request
        ----------------------------------------------------------------*/
        /*---------------------------------------------------------------
           Truncate the block request to the end of layer 0.
        ----------------------------------------------------------------*/
        if( (LoadType == EXTEND_READ) || (LoadType == EXTEND_READ_AHEAD) )
        {

            temp =   GET_INT_ENABLE_MASK();
            DISABLE_INTERRUPT();

            BlocksLeft = ((dvdDiscRequest.EccBlockCount - dvdDiscRequest.EccBlocksBuffered) * SECTORS_PER_DVDBLOCK);
          LastBufferBlock = DataBuffer.CacheEndLBA + BlocksLeft;
          if(LastBufferBlock > pdbGetDVDEndLBALayer0())
          {
                dvdDiscRequest.EccBlockCount -= (LastBufferBlock - pdbGetDVDEndLBALayer0()) / ONE_DVD_BLOCK;
                BlockRequest = 0;
            }
          else
          {
                BlockRequest = (pdbGetDVDEndLBALayer0() - LastBufferBlock) / ONE_DVD_BLOCK;
            }

            SET_INT_ENABLE_MASK( temp );

            dvdDiscRequest.Type = LoadType;
            dvdDiscRequest.ExtendBlocks = (BYTE) BlockRequest;

            DiscReadStatus = dvdStartDiscTransfer();

        }
        else
        {
            SavedBlockRequest = BlockRequest;
            BlockRequest = ((pdbGetDVDMaxPSN(0) + 1L) - StartPSN.PSN) / ONE_DVD_BLOCK;

            /*---------------------------------------------------------------
                Set up the Disc Transfer control parameters
            ----------------------------------------------------------------*/
            dvdDiscRequest.Type            = LoadType;
            dvdDiscRequest.TargetID.Long   = StartPSN.PSN;
            dvdDiscRequest.EccBlockCount   = BlockRequest;
            dvdDiscRequest.TargetLayer     = StartPSN.Layer;

            /*---------------------------------------------------------------
                Call the main DVD disc transfer engine
            ----------------------------------------------------------------*/
            DiscReadStatus = dvdStartDiscTransfer();

            /*---------------------------------------------------------------
                If the buffering status was good and not all host blocks
                have been buffered continue to load blocks from the second layer
                If all host blocks are in the buffer let the next restart of
                pre-fetching cache blocks from the next layer
            ----------------------------------------------------------------*/
            if ((DiscReadStatus == BUF_GOOD) && (dvdDecFlag.fREAD_AHEAD == FALSE))
            {
                /*---------------------------------------------------------------
                    Complete the block request by buffering the remaining blocks
                    that reside on the next layer.
                ----------------------------------------------------------------*/
                BlockRequest = SavedBlockRequest - BlockRequest;
                StartPSN.PSN = pdbGetDVDStartPSN(1);

                /*---------------------------------------------------------------
                    Set up the Disc Transfer control parameters
                ----------------------------------------------------------------*/
                dvdDiscRequest.Type            = LoadType;
                dvdDiscRequest.TargetID.Long   = StartPSN.PSN;
                dvdDiscRequest.EccBlockCount   = BlockRequest;
                dvdDiscRequest.TargetLayer     = LastPSN.Layer;         /*read next layer */

                /*---------------------------------------------------------------
                    Call the main DVD disc transfer engine
                ----------------------------------------------------------------*/
                DiscReadStatus = dvdStartDiscTransfer();
            }
        }
    }

    return(DiscReadStatus);
}

#if( DVD_ZONE_TEST == TRUE )

/*******************************************************************************

    FUNCTION        dvdTestZone

    DESCRIPTION     Tests CTF values for reading specified ranges on the disc

********************************************************************************/
void dvdTestZones()
{

    BYTE range;         /* Number of ranges on the disc to test */
    int loop;           /* Number of iterations on a particular range */
    BYTE ctfval;        /* CTF value */
    ULONG StartPSN;
    ULONG BlockRequest;
    DiscAccessStatusE   DiscReadStatus;     /* Status of the DVD disc access */
    BYTE InitCtf,EndCtf;

    /*---------------------------------------------------------------
        Start at zone 3 (Begins at LBA 0), maximize block count.
        Insure read ahead will not interfere with this test by
        setting read ahead start psn to last psn of disc (layer)
    ----------------------------------------------------------------*/
    StartPSN = 0x30000;
    BlockRequest = 0x3FF;
    dvdReadAheadStartPSN = pdbGetDVDMaxPSN(0);
    ClearDataBuffer();

    /*---------------------------------------------------------------
        Range test
    ----------------------------------------------------------------*/
    for (range = 0; range < 3; range++)
    {

        if( range == 0 ){ InitCtf = 0xD0; EndCtf = 0xE8; }
        if( range == 1 ){ InitCtf = 0xD8; EndCtf = 0xF0; }
        if( range == 2 ){ InitCtf = 0xE0; EndCtf = 0xFE; }

        /*---------------------------------------------------------------
            CTF change loop
        ----------------------------------------------------------------*/
        for (ctfval = InitCtf; ctfval <= EndCtf; ctfval++)
        {

            /*---------------------------------------------------------------
                Set CTV value and number of times to read a range per CTF value
            ----------------------------------------------------------------*/
            WriteSCReg_1B(rwCTF1, ctfval);
            loop = 2;

            /*---------------------------------------------------------------
                Initialize counts
            ----------------------------------------------------------------*/
            dvdTest.PICCount = 0L;
            dvdTest.PIUCount = 0L;
            dvdTest.POCCount = 0L;
            dvdTest.POUCount = 0L;
            dvdTest.BlockCount = 0;

            /*---------------------------------------------------------------
                Read Range
            ----------------------------------------------------------------*/
            while( loop-- > 0 )
            {

                /*---------------------------------------------------------------
                    Set up the Disc Transfer control parameters
                ----------------------------------------------------------------*/
                dvdDiscRequest.Type            = INIT_READ;
                dvdDiscRequest.TargetID.Long   = StartPSN;
                dvdDiscRequest.TargetLayer     = eLayer0;
                dvdDiscRequest.EccBlockCount   = BlockRequest;

                /*---------------------------------------------------------------
                    Call the main DVD disc transfer engine
                ----------------------------------------------------------------*/
                DiscReadStatus = dvdStartDiscTransfer();

                if(DiscReadStatus == GEN_FAIL)
                    return;
            }

            /*---------------------------------------------------------------
                Display total correctables/uncorrectables for range
            ----------------------------------------------------------------*/
            DVD_MESSAGE1(PRN_DVDHIF,"TOTALS FOR CTF = ",1,ctfval);
            if (dvdTest.BlockCount != 0)
            {
                DVD_MESSAGE1(PRN_DVDHIF,"TOTAL BLOCKS READ: ",2,dvdTest.BlockCount);
                DVD_MESSAGE2(PRN_DVDHIF,"PIC = ",4, dvdTest.PICCount," PIU = ",4, dvdTest.PIUCount);
                DVD_MESSAGE2(PRN_DVDHIF,"POC = ",4, dvdTest.POCCount," POU = ",4, dvdTest.POUCount);
                DVD_MESSAGE(PRN_DVDHIF,"------------------------------");
            }
            else
                DVD_MESSAGE(PRN_DVDHIF,"NO BLKS BUFFERED !");
        }

        /*---------------------------------------------------------------
            Set to starting psn to next range. Insure range remains
            within the end psn
        ----------------------------------------------------------------*/
        StartPSN += 0xF0000;
        if ( (StartPSN + (BlockRequest * SECTORS_PER_DVDBLOCK)) > pdbGetDVDMaxPSN(0) )
            BlockRequest = ((pdbGetDVDMaxPSN(0) & DVD_BLOCK_MASK) - StartPSN) >> 4;

    }

    dvdTest.TestComplete = TRUE;
    ClearDataBuffer();
}
#endif /* DVD_TEST_ZONE */

