/****************************************************************************
*                (c) Copyright 2000 - 2007 SUNEXT DESIGN, INC.
*                           All Right Reserved
*
*
*   FILE NAME:     bdBufMgr.c
*
*   DESCRIPTION:   Manages the BD buffer. Provides routines for accessing
*                  and updating buffer information and status.
*
*   $Revision: 25 $
*   $Date: 10/12/21 4:15p $
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
#include ".\hif\at_equ.h"

#include ".\dvd\dvbufmgr.h"

#include ".\bd\bdDef.h"
#include ".\bd\bdMacro.h"
#include ".\bd\bdVars.h"
#include ".\bd\bdbufmgr.h"
#include ".\bd\bdutil.h"
#include ".\bd\bddiscrd.h"
#include ".\bd\bddiag.h"

#if (ENABLE_AACS == 1)
#include ".\crypt\aacs.h"
#endif

#include ".\al\reg_intctrl.h"
#include ".\common\intctl.h"

#if (ENABLE_AACS == 1)
extern BOOL fBypassAuth;
extern AacsS AacsStatus;
#endif

/*****************************************************************************
    PROTOTYPES
******************************************************************************/
void bdInitializeHostBuffer(ULONG);
void bdCheckReadAhead(void);
void bdAdjustHostBufferPtr(ULONG);

SearchCacheResultE bdSearchCache(TransferS, BYTE);
//static ULONG bdGetClusterRequest(TransferS, TransferTypeE);

a


/*******************************************************************************
* FUNCTION DEFINITIONS
*******************************************************************************/
#if (BD_ENABLE == 1)
static ULONG bdGetClusterRequest(TransferS, TransferTypeE);
/*******************************************************************************

    FUNCTION        bdSearchCache

    DESCRIPTION     Searches the BD cache to determine if the requested host
                    sectors are within the current buffer

    PARAMETERS      Request -   TransferS structure containing the starting LBA
                                and transfer count

                    Mode    -   0 = Wait mode. In this mode, if buffering is
                                on and the next cluster to be buffered is the
                                requested cluster the function will wait
                                until the cluster is buffered or buffering stops
                                before reporting cache status

                                1 = Immediate mode. In this mode if the requested
                                cluster is not in the cache even if buffering is
                                on, a cache miss is reported

    RETURNS         Either CACHE_MISS, FULL_CACHE_HIT, or PARTIAL_CACHE_HIT

********************************************************************************/
SearchCacheResultE bdSearchCache(TransferS Request, BYTE mode)
{

    ULONG   EndLBA;
    ULONG   bdStartCluster;
    BYTE    InterruptStatus;

    InterruptStatus = GET_INT_ENABLE_MASK();
    DISABLE_INTERRUPT();

    bdStartCluster = pdbGetBDClusterStart(DataBuffer.CacheStartLBA);

    /*-------------------------------------------------------------------
       Cache is static if read ahead = false (no buffering is occuring)
       If cache is static determine if the starting LBA is currently in
       the buffer. If not return a cache miss status. For fast cache
       transfers check regardless if cache is static or not.
    --------------------------------------------------------------------*/
     /* To reduce the File System ReadBack Time */
	if (bdStartCluster > ONE_BD_CLUSTER)
    {
		if (((PreATAPICMD == SBC_READ_10)||(PreATAPICMD == SBC_READ_12))&&(PreReadCMDLba<=Request.StartBlock.lba)&&(Request.StartBlock.lba<bdStartCluster))
		{
		    DataBuffer.SectorCount  = DataBuffer.SectorCount + (DataBuffer.CacheStartLBA & 0x1F) + ONE_BD_CLUSTER;
		    DataBuffer.CacheStartLBA = pdbGetBDClusterStart(DataBuffer.CacheStartLBA) - ONE_BD_CLUSTER;

		    if (DataBuffer.CurHostPtr < ONE_BD_CLUSTER)
		    {
			    DataBuffer.CurHostPtr = ((DataBuffer.CurHostPtr & BD_CLUSTER_MASK) + BUF_TOTAL_BLK_COUNT - ONE_BD_CLUSTER);
		    }
		    else
		    {
		  	    DataBuffer.CurHostPtr = (DataBuffer.CurHostPtr & BD_CLUSTER_MASK) - ONE_BD_CLUSTER;
		    }
	        bdStartCluster = pdbGetBDClusterStart(DataBuffer.CacheStartLBA);
		}
	}
	SET_INT_ENABLE_MASK( InterruptStatus);

    if ((BD_BUFFERING() == OFF) || (mode == IMMEDIATE_SEARCH_MODE) || (READ_LDC_BIS==1))
    {
        /* No data in the buffer */
        if (DataBuffer.SectorCount == 0)
            return (CACHE_MISS);

        /* Staring LBA is not among the cached blocks */
        if ( (Request.StartBlock.lba < bdStartCluster) ||  (Request.StartBlock.lba > DataBuffer.CacheEndLBA) )
            return ( CACHE_MISS );

    }
    else
    {
        /*-------------------------------------------------------------------
           If cache is not static determine if the starting LBA is currently
           in the buffer or if it is within the specified range of clusters that
           will be cached soon. If not return a cache miss status
        --------------------------------------------------------------------*/

        /* Starting LBA is not in the buffer nor among the range of clusters to be cached */
        if ( (Request.StartBlock.lba < bdStartCluster) ||
             (Request.StartBlock.lba > (DataBuffer.CacheEndLBA + BD_CACHE_WAIT_RANGE)) )
            return ( CACHE_MISS );


        if ( (Request.StartBlock.lba > DataBuffer.CacheEndLBA) || (DataBuffer.SectorCount == 0) )
        {
            /*-------------------------------------------------------------------
                Wait for the starting LBA to come in to the buffer
                (Request.StartLBA.lba will be less than CacheEndLBA once the
                cluster is loaded) If buffering stops before this return a cache miss.
            --------------------------------------------------------------------*/
            while ( (Request.StartBlock.lba > DataBuffer.CacheEndLBA) || (DataBuffer.SectorCount == 0))
            {
                if( (BD_BUFFERING() == OFF ) || (bdClusterInterruptTimer.fTIMEOUT == TRUE))
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
    EndLBA = Request.StartBlock.lba + Request.BlockCount.Long - 1L;

    /*--------------------------------------------------
        Determine if this is a full or partial cache hit.
    --------------------------------------------------*/
    if( EndLBA <= DataBuffer.CacheEndLBA )
        return ( FULL_CACHE_HIT );
    else
        return ( PARTIAL_CACHE_HIT );

}

/*******************************************************************************

    FUNCTION        bdAdjustHostBufferPtr

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
                    sector 0 of the cluster address that DataBuffer.CurHostPtr
                    points to.

********************************************************************************/
void bdAdjustHostBufferPtr( ULONG StartLBA)
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
        DataBuffer.CurHostPtr %= BUF_TOTAL_BLK_COUNT;

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

    FUNCTION        bdInitializeHostBuffer()

    DESCRIPTION     Places the buffer in a static state by halting the buffering
                    if on, clearing the buffer and initializing the start and
                    end values of the cache

    PARAMETERS      StartLBA    Used to initial the start and end of the cache

********************************************************************************/
void bdInitializeHostBuffer( ULONG StartLBA )
{
    ULONG StartBlock;

    /*---------------------------------------------------------------
        If buffering is on stop transferring data to buffer
    ---------------------------------------------------------------*/
    if (BD_BUFFERING() == ON)
        bdStopDiscTransfer();

    /*---------------------------------------------------------------
        Clear the buffer and mark no host blocks are in the buffer
    ---------------------------------------------------------------*/
    ClearDataBuffer();
    bdDecFlag.fREAD_AHEAD = FALSE;

    /*---------------------------------------------------------------
        Set flag for the first cluster to be loaded into the
        buffer. Once the cluster is loaded and this flag is TRUE
        the host pointer will be adjusted to the starting lba within
        the cluster. This is done in the cluster interrupt
        service routine bdCheckForHostTransfer().
    ----------------------------------------------------------------*/
    bdDecFlag.fFIRST_HOST_CLUSTER = TRUE;

    /*---------------------------------------------------------------
        Initialize the start and end of cache lba values. When the
        first block comes in CacheEndLBA is updated.
    ----------------------------------------------------------------*/
    DataBuffer.CacheStartLBA = StartLBA;
    StartBlock = pdbGetBDClusterStart(StartLBA);
    if (StartBlock != 0L)
        DataBuffer.CacheEndLBA   =  StartBlock - 1L;

}
/*******************************************************************************

    FUNCTION        bdGetClusterRequest

    DESCRIPTION     Returns the number of bd clusters to be loaded into the
                    buffer based on the given request (LBA and Length) and load type

    PARAMETERS      Request         Transfer structure cotaining request information
                    LoadType        Type of disc transfer to be performed


    RETURNS         ClusterRequest  Number of BD clusters requested for loading into
                                    the buffer

********************************************************************************/
static ULONG bdGetClusterRequest(TransferS Request, TransferTypeE LoadType)
{
    ULONG   RequestStartCluster;    /* Starting bd cluster of the request (LBA) */
    ULONG   RequestEndCluster;      /* Ending bd cluster of the request   (LBA) */
    ULONG   ClusterRequest;         /* Number of clusters to buffer */
    ULONG   ClustersLeft;
    ULONG   LastBufferCluster;
    ULONG   MaxRecordedLBA;
    PSNs    StartPSN;
    BYTE    temp, track;
    PSNs    MaxRecordedPSN;         /* The current Maximum recorded PSN */

    ClusterRequest = 0L;

    /*---------------------------------------------------------------
        Determine the starting and ending bd clusters of the request
        (These addresses are the LBA of the first sector of each cluster)
    ----------------------------------------------------------------*/
    RequestStartCluster = pdbGetBDClusterStart(Request.StartBlock.lba);
    RequestEndCluster = pdbGetBDClusterStart(Request.StartBlock.lba + Request.BlockCount.Long - 1L);

    track = pdbGetBDHighestRecordedTrackNumber();

    if (pdbGetBDTrackState(track) == eTRACK_STATE_INCOMPLETE)
        MaxRecordedLBA = bdConvertPSNtoLBA(pdbGetBDTrackRecordedPSN(track));
    else
        MaxRecordedLBA = pdbGetBDMaxLBA();

    if( (LoadType == INIT_READ) || (LoadType == INIT_READ_AHEAD) )
    {

        /*---------------------------------------------------------------
            Get the number of BD clusters needed to satisfy the request
        ----------------------------------------------------------------*/
        ClusterRequest = ((RequestEndCluster - RequestStartCluster) / SECTORS_PER_CLUSTER) + 1L;

        /*---------------------------------------------------------------
            If this is a init read without aacs in progress set up
            starting location of read ahead
        ----------------------------------------------------------------*/
#if (ENABLE_AACS == 1)
        if ((LoadType == INIT_READ) && (AacsStatus.AacsFlag.AgidInUse == FALSE))
#else
        if (LoadType == INIT_READ)
#endif  /* ENABLE_AACS == 1 */
        {
            /*---------------------------------------------------------------
                Calculate the PSN of the first read ahead cluster. This check
                is done in the BD cluster ISR to indicate when read ahead
                clusters are being bufferred

                PSN of first read ahead cluster = request end cluster + ONE_BD_CLUSTER
            ----------------------------------------------------------------*/
            if (pdbGetBDLayers() == BD_DUAL_LAYER)
            {
                if((RequestEndCluster + ONE_BD_CLUSTER) <= pdbGetBDEndLBALayer0())
                {
                    StartPSN = bdConvertLBAtoPSN(RequestEndCluster + ONE_BD_CLUSTER);
                    bdReadAheadStartPSN = StartPSN.PSN;

                    /*---------------------------------------------------------------
                        If read ahead is enable add pre-fetch blocks to the count
                        ----------------------------------------------------------------*/
                    #if (BD_READ_AHEAD_ENABLED == TRUE)
                    if ((RequestEndCluster +(BUF_TOTAL_BLK_COUNT - BD_PAD_SECTORS)) <= pdbGetBDEndLBALayer0())
                    {
                        if ((RequestEndCluster + BUF_TOTAL_BLK_COUNT - BD_PAD_SECTORS) >= MaxRecordedLBA)
                        {
                            ClusterRequest += (MaxRecordedLBA - RequestEndCluster)/SECTORS_PER_CLUSTER;
                            bdDecFlag.fDISABLE_READ_AHEAD = TRUE;
                            /* For correct drive hang-up by the wrong bdReadAheadPSN */
                                MaxRecordedPSN = bdConvertLBAtoPSN(pdbGetBDClusterStart(MaxRecordedLBA));
                                if (bdReadAheadStartPSN >  MaxRecordedPSN.PSN)
                                    StartPSN = bdConvertLBAtoPSN(pdbGetBDClusterStart(MaxRecordedLBA+1));
                            /* For correct drive hang-up by the wrong bdReadAheadPSN */
                            bdReadAheadStartPSN = StartPSN.PSN;
                        }
                        else
                            ClusterRequest = (BUF_TOTAL_BLK_COUNT/SECTORS_PER_CLUSTER - BD_PAD_CLUSTERS);
                    }
                    else
                    {
                        if (MaxRecordedLBA < pdbGetBDEndLBALayer0())
                        {
                            if (pdbGetBDClusterStart(MaxRecordedLBA) >= RequestEndCluster)
                                ClusterRequest += (MaxRecordedLBA - RequestEndCluster)/SECTORS_PER_CLUSTER;

                            bdDecFlag.fDISABLE_READ_AHEAD = TRUE;
                            StartPSN = bdConvertLBAtoPSN(pdbGetBDClusterStart(MaxRecordedLBA+1));
                            bdReadAheadStartPSN = StartPSN.PSN;
                        }
                        else
                            ClusterRequest += (pdbGetBDEndLBALayer0() - RequestEndCluster)/SECTORS_PER_CLUSTER;
                    }
                    #endif
                }
                else if(RequestStartCluster > pdbGetBDEndLBALayer0())
                {
                    if ((RequestEndCluster + ONE_BD_CLUSTER) <= MaxRecordedLBA)
                    {
                        StartPSN = bdConvertLBAtoPSN(RequestEndCluster + ONE_BD_CLUSTER);
                        bdReadAheadStartPSN = StartPSN.PSN;
                        /*---------------------------------------------------------------
                        If read ahead is enabled add pre-fetch blocks to the count
                        ----------------------------------------------------------------*/
                        #if (BD_READ_AHEAD_ENABLED == TRUE)
                            if ((BUF_TOTAL_BLK_COUNT/SECTORS_PER_CLUSTER - BD_PAD_CLUSTERS) > (MaxRecordedLBA - RequestEndCluster)/SECTORS_PER_BLOCK)
                            {
                                ClusterRequest += (MaxRecordedLBA - RequestEndCluster)/SECTORS_PER_CLUSTER;
                                bdDecFlag.fDISABLE_READ_AHEAD = TRUE;
                                /* For correct drive hang-up by the wrong bdReadAheadPSN */
                                MaxRecordedPSN = bdConvertLBAtoPSN(pdbGetBDClusterStart(MaxRecordedLBA));
                                if (bdReadAheadStartPSN >  MaxRecordedPSN.PSN)
                                    StartPSN = bdConvertLBAtoPSN(pdbGetBDClusterStart(MaxRecordedLBA+1));
                                /* For correct drive hang-up by the wrong bdReadAheadPSN */
                                bdReadAheadStartPSN = StartPSN.PSN;
                            }
                            else
                                ClusterRequest = (BUF_TOTAL_BLK_COUNT/SECTORS_PER_CLUSTER - BD_PAD_CLUSTERS);
                        #endif
                    }
                    else
                    {
                        bdDecFlag.fDISABLE_READ_AHEAD = TRUE;
                        StartPSN = bdConvertLBAtoPSN(RequestEndCluster);
                        bdReadAheadStartPSN = StartPSN.PSN;
                    }
                }
                else
                {
                     bdDecFlag.fDISABLE_READ_AHEAD = TRUE;
                     StartPSN = bdConvertLBAtoPSN(RequestEndCluster);
                     bdReadAheadStartPSN = StartPSN.PSN;
                }
            }
            else
            {
                if((RequestEndCluster + ONE_BD_CLUSTER) <= MaxRecordedLBA)
                {
                    StartPSN = bdConvertLBAtoPSN(RequestEndCluster + ONE_BD_CLUSTER);
                    bdReadAheadStartPSN = StartPSN.PSN;

                    /*---------------------------------------------------------------
                        If read ahead is enable add pre-fetch blocks to the count
                    ----------------------------------------------------------------*/
                    #if (BD_READ_AHEAD_ENABLED == TRUE)
                        if ((BUF_TOTAL_BLK_COUNT/SECTORS_PER_CLUSTER - BD_PAD_CLUSTERS) > (MaxRecordedLBA - RequestEndCluster)/SECTORS_PER_CLUSTER)
                        {
                            ClusterRequest += (MaxRecordedLBA - RequestEndCluster)/SECTORS_PER_CLUSTER;
                            bdDecFlag.fDISABLE_READ_AHEAD = TRUE;
                            /* For correct drive hang-up by the wrong bdReadAheadPSN */
                            MaxRecordedPSN = bdConvertLBAtoPSN(pdbGetBDClusterStart(MaxRecordedLBA));
                            if (bdReadAheadStartPSN >  MaxRecordedPSN.PSN)
                                StartPSN = bdConvertLBAtoPSN(pdbGetBDClusterStart(MaxRecordedLBA+1));
                            /* For correct drive hang-up by the wrong bdReadAheadPSN */
                            bdReadAheadStartPSN = StartPSN.PSN;
                        }
                        else
                            ClusterRequest = (BUF_TOTAL_BLK_COUNT/SECTORS_PER_CLUSTER - BD_PAD_CLUSTERS);
                    #endif
                }
                else
                {
                    bdDecFlag.fDISABLE_READ_AHEAD = TRUE;
                    StartPSN = bdConvertLBAtoPSN(RequestEndCluster);
                    bdReadAheadStartPSN = StartPSN.PSN;
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

        ClustersLeft = ((bdDiscRequest.ClusterCount - bdDiscRequest.ClustersBuffered) * SECTORS_PER_CLUSTER);
        LastBufferCluster = pdbGetBDClusterStart(DataBuffer.CacheEndLBA) + ClustersLeft;

        SET_INT_ENABLE_MASK( temp );

        if (RequestEndCluster <= LastBufferCluster)
            ClusterRequest = (BUF_TOTAL_BLK_COUNT/SECTORS_PER_CLUSTER - BD_PAD_CLUSTERS - 1) - ((LastBufferCluster - RequestEndCluster)/SECTORS_PER_CLUSTER);
        else
            ClusterRequest = (BUF_TOTAL_BLK_COUNT/SECTORS_PER_CLUSTER - BD_PAD_CLUSTERS - 1) + ((RequestEndCluster - LastBufferCluster)/SECTORS_PER_CLUSTER);


        if (RequestStartCluster < pdbGetBDEndLBALayer0())
        {
            if (MaxRecordedLBA > pdbGetBDEndLBALayer0())
            {
                if ((LastBufferCluster + ClusterRequest * SECTORS_PER_CLUSTER) > pdbGetBDEndLBALayer0())
                {
                    if (LastBufferCluster <= pdbGetBDEndLBALayer0())
                        ClusterRequest = (pdbGetBDEndLBALayer0() - LastBufferCluster)/SECTORS_PER_CLUSTER;
                    else
                        ClusterRequest = 0;

                    if (ClusterRequest == 0)
                        bdDecFlag.fDISABLE_READ_AHEAD = TRUE;
                }
            }
            else
            {
                if ((LastBufferCluster + ClusterRequest * SECTORS_PER_CLUSTER) > MaxRecordedLBA)
                {
                    if (LastBufferCluster <= MaxRecordedLBA)
                        ClusterRequest = (MaxRecordedLBA - LastBufferCluster)/SECTORS_PER_CLUSTER;
                    else
                        ClusterRequest = 0;

                    bdDecFlag.fDISABLE_READ_AHEAD = TRUE;
                }
            }
        }
        else
        {
            if ((LastBufferCluster + ClusterRequest * SECTORS_PER_CLUSTER) > MaxRecordedLBA)
            {
                if (LastBufferCluster <= MaxRecordedLBA)
                    ClusterRequest = (MaxRecordedLBA - LastBufferCluster)/SECTORS_PER_CLUSTER;
                else
                    ClusterRequest = 0;

                if (ClusterRequest == 0)
                    bdDecFlag.fDISABLE_READ_AHEAD = TRUE;
            }
        }

        /*---------------------------------------------------------------
            If buffering is on then current blocks being bufferred may be
            host blocks. If the last block of the request is not in the
            buffer not all host blocks have been buffered. Inidicate which
            is the last PSN of the requested host blocks.
         ----------------------------------------------------------------*/
        if (BD_BUFFERING() == ON)
        {

            /*-----------------------------------------------------------
                When ECC interrupt comes in for the last host block,
                Set read ahead to FALSE and will not to be TRUE
                Must disable interrupt to avoid this situation
            -------------------------------------------------------------*/
            temp = GET_INT_ENABLE_MASK();
            DISABLE_INTERRUPT();

            if (RequestEndCluster > DataBuffer.CacheEndLBA)
            {
                bdDecFlag.fREAD_AHEAD = FALSE;
                StartPSN = bdConvertLBAtoPSN(RequestEndCluster + ONE_BD_CLUSTER);
                bdReadAheadStartPSN = StartPSN.PSN;
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
        ClusterRequest = (Request.BlockCount.Long/SECTORS_PER_CLUSTER);

    }

    if (ClusterRequest > (BUF_TOTAL_BLK_COUNT/SECTORS_PER_CLUSTER - BD_PAD_CLUSTERS))
        ClusterRequest = (BUF_TOTAL_BLK_COUNT/SECTORS_PER_CLUSTER - BD_PAD_CLUSTERS);

    return (ClusterRequest);

}

/*******************************************************************************

    FUNCTION        bdCheckReadAhead

    DESCRIPTION     Determines if pre-fetching can be initiated. Will turn on
                    buffering if buffering is off. If the buffering is on
                    a determination is made to extend the current read

********************************************************************************/
extern BYTE CmdSeekOk;
void bdCheckReadAhead()
{

    TransferS   Request;            /* Logical request for pre-fetch */
    BYTE        InterruptStatus;    /* Temp interrupt status */
    ULONG       ClustersRemaining;  /* Number of bd clusters remaining to be buffered */
    BYTE        LoadType;           /* type of buffer loading action */
    WORD        SectorCount;        /* Saved buffer count */
    ULONG       MaxRecordedLBA;
    BYTE        track;

#if (E_DIAGS == 1)
	if (READ_LDC_BIS == 1) 
       return; /* TBD! DVD HAS TOO!! Explain here WHY this is here */
#endif

    if (bdDecFlag.fDISABLE_READ_AHEAD)
        return;

    /*-------------------------------------------------------------------
        Do not start read ahead if eject has been issued
    --------------------------------------------------------------------*/
    if (EjectKeyInfo.PressedValid)
        return;

    /*-------------------------------------------------------------------
        when excuting BD Seek Command, do not enable read ahead
    --------------------------------------------------------------------*/
    if(CmdSeekOk != 0)
        return;

    /*-------------------------------------------------------------------
      If disc is copy protected and AACS authentication has not passed
      do not pre-fetch
    --------------------------------------------------------------------*/

#if (ENABLE_AACS == 1 && BAPASS_AACS_AUTH == 0)
    if ( (fBypassAuth == FALSE) && (AacsStatus.AacsFlag.IsMediumCompliant == TRUE) && (AacsStatus.AacsFlag.AuthenticationSuccess == FALSE) )
    {
        send_msg80(SEND_READ_DVD_MSG,0x710083);
        return;
    }
#endif

    if ((BD_BUFFERING() == OFF) || (bdClusterInterruptTimer.fTIMEOUT == TRUE) )
    {

        /*-------------------------------------------------------------------
           If a time out has occured insure that all decoding
           stops including the clearing of interrupts and status flags.
        --------------------------------------------------------------------*/
        if (bdClusterInterruptTimer.fTIMEOUT == TRUE)
            bdStopDiscTransfer();

        /*-------------------------------------------------------------------
           Because the DataBuffer.SectorCount may modify in bdSearchCache()
           that called in interrupt service routine, we must disable interrupt
           before using it.
        --------------------------------------------------------------------*/
        InterruptStatus =   GET_INT_ENABLE_MASK();
        DISABLE_INTERRUPT();

        SectorCount = DataBuffer.SectorCount;

        SET_INT_ENABLE_MASK( InterruptStatus );

        /*-------------------------------------------------------------------
           If buffer is full exit. Subtract one bd cluster to compensate
           a partial cluster in the buffer.

           Example: (*SECTORS_PER_CLUSTER - BD_PAD_CLUSTERS)*SECTORS_PER_CLUSTER
           = 200h.  If SectorCount = 1F3h, the buffer is full since a partial
           takes a full cluster location. Therfore
            if (1F3h > (200h - 10h) or (1F3 > 1F0) return since buffer is full

        --------------------------------------------------------------------*/
        if ( SectorCount > (BUF_TOTAL_BLK_COUNT - BD_PAD_SECTORS - ONE_BD_CLUSTER))
            return;

        /*-------------------------------------------------------------------
            Get the next starting address to initiate buffering. If this is
            an initial read (after startup and read lead in) CacheEndLBA + 1
            = LBA 0x01 which will load the BD cluster starting at LBA 0x000
        --------------------------------------------------------------------*/
        if (DataBuffer.CacheEndLBA == 0L)
            Request.StartBlock.lba = 0L;
        else
            Request.StartBlock.lba   = DataBuffer.CacheEndLBA + 1L ;

        /*-------------------------------------------------------------------
           If attempting to read the same cluster exceedes the maximum
           attempts allowed then do not initiate read ahead
        --------------------------------------------------------------------*/
        if( Request.StartBlock.lba == bdLastReadAheadTarget)
        {
            if (bdReadAheadTargetRetries == 0)
                return;

            --bdReadAheadTargetRetries;
        }
        else
        {
            bdLastReadAheadTarget      = Request.StartBlock.lba;
            bdReadAheadTargetRetries   = MAX_BD_READ_RETRIES;
        }

        /*-------------------------------------------------------------------
           Set the buffer load type and turn read ahead flag on
        --------------------------------------------------------------------*/
        LoadType                = INIT_READ_AHEAD;
        bdDecFlag.fREAD_AHEAD   = TRUE;

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
        ClustersRemaining = bdDiscRequest.ClusterCount - bdDiscRequest.ClustersBuffered;
        SectorCount = DataBuffer.SectorCount + (WORD)(ClustersRemaining * SECTORS_PER_CLUSTER);

        Request.StartBlock.lba = DataBuffer.CacheEndLBA + 1;
        /*-------------------------------------------------------------------
            Reload previous interrupt status
        --------------------------------------------------------------------*/
        SET_INT_ENABLE_MASK( InterruptStatus );

        /*-------------------------------------------------------------------
            If the presumed sector count equals or exceeds the buffer size
            do not extend.

           NOTE: Add one BD cluster to sector count to compensate for a
           partial BD cluster
        --------------------------------------------------------------------*/
        if ( SectorCount > (BUF_TOTAL_BLK_COUNT - BD_PAD_SECTORS - ONE_BD_CLUSTER))
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
    Request.BlockCount.Long = ((BUF_TOTAL_BLK_COUNT - BD_PAD_SECTORS - SectorCount) & BD_CLUSTER_MASK);

    track = pdbGetBDHighestRecordedTrackNumber();

    if (pdbGetBDTrackState(track) == eTRACK_STATE_INCOMPLETE)
        MaxRecordedLBA = bdConvertPSNtoLBA(pdbGetBDTrackRecordedPSN(track));
    else
        MaxRecordedLBA = pdbGetBDMaxLBA();

    if (Request.StartBlock.lba > MaxRecordedLBA) return;

    if((Request.StartBlock.lba + Request.BlockCount.Long) > MaxRecordedLBA)
    {
        Request.BlockCount.Long = MaxRecordedLBA - Request.StartBlock.lba;
    }

    if((Request.StartBlock.lba <= pdbGetBDEndLBALayer0()) && (pdbGetBDLayers() == BD_DUAL_LAYER))
    {
        ULONG RequestEndBlklba;
        ULONG BlockLong;
        if( LoadType == EXTEND_READ_AHEAD)
        {
            RequestEndBlklba = Request.StartBlock.lba + (ClustersRemaining * SECTORS_PER_CLUSTER) - 1;
            BlockLong = pdbGetBDEndLBALayer0() - RequestEndBlklba;
            if(Request.BlockCount.Long > BlockLong)
            {
                Request.BlockCount.Long = BlockLong;
            }
        }
        if((Request.StartBlock.lba + Request.BlockCount.Long) > pdbGetBDEndLBALayer0())
        {
            Request.BlockCount.Long = pdbGetBDEndLBALayer0() - Request.StartBlock.lba;
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

        bdLoadBuffer( Request, LoadType );
    }

    return;
}

/*******************************************************************************

    FUNCTION        bdLoadBuffer

    DESCRIPTION     Translates the host request to a BD cluster based request
                    and then initiates the disc transfer.

    PARAMETERS      Request         Transfer structure cotaining host request information
                    LoadType        Type of disc transfer to be performed

    GLOBAL UPDATES  bdDiscRequest

    RETURNS         DiscReadStatus  Status of the disc read / buffer operation

********************************************************************************/
DiscAccessStatusE bdLoadBuffer(TransferS Request, TransferTypeE LoadType)
{
    ULONG   ClusterRequest;                 /* Number of clusters to buffer */
    ULONG   ClusterAddress;
    PSNs    StartPSN;                       /* Starting PSN of the requested clusters */
    PSNs    LastPSN;                        /* Last PSN of the requested clusters */
    ULONG   SavedClusterRequest;            /* Truncated block request if in a layer crossing */
    ULONG   MaxPSN;                         /* Maximum PSN value in layer */
    DiscAccessStatusE   DiscReadStatus;     /* Status of the BD disc access */
    ULONG   ClustersLeft;
    ULONG   LastBufferCluster;
    BYTE    temp;


    /*---------------------------------------------------------------
        Determine the number of BD clusters to load
    ----------------------------------------------------------------*/
    ClusterRequest = bdGetClusterRequest(Request, LoadType);

    /*---------------------------------------------------------------
       Convert the start address of the request to PSN and layer
    ----------------------------------------------------------------*/
    ClusterAddress = pdbGetBDClusterStart(Request.StartBlock.lba);
    StartPSN = bdConvertLBAtoPSN(ClusterAddress);

    /*---------------------------------------------------------------
       Convert the end address of the request to PSN and layer
    ----------------------------------------------------------------*/
    if (ClusterRequest > 0)
        ClusterAddress  = (ClusterAddress + (ClusterRequest * SECTORS_PER_CLUSTER)) - 1L;

    if(ClusterAddress > pdbGetBDMaxLBA())
        LastPSN  = bdConvertLBAtoPSN(pdbGetBDMaxLBA());
    else
        LastPSN  = bdConvertLBAtoPSN(ClusterAddress);

    /*---------------------------------------------------------------
       Determine maximum PSN of the layer that LastPSN is within
    ----------------------------------------------------------------*/
    if (LastPSN.Layer == eLayer1)
        MaxPSN = pdbGetBDMaxPSN(1);
    else
        MaxPSN = pdbGetBDMaxPSN(0);


    if( StartPSN.Layer == LastPSN.Layer )
    {
        if( (LoadType == EXTEND_READ) || (LoadType == EXTEND_READ_AHEAD) )
        {
            bdDiscRequest.Type = LoadType;
            bdDiscRequest.ExtendClusters = (BYTE) ClusterRequest;
        }
        else
        {

            /*---------------------------------------------------------------
               Insure that the request will not extend beyond the maximum
               capcity of the disc/layer
            ----------------------------------------------------------------*/
            if ( LastPSN.PSN > MaxPSN )
                ClusterRequest = ((MaxPSN + 1L) - StartPSN.PSN) / ONE_BD_CLUSTER;

            /*---------------------------------------------------------------
                Set up the Disc Transfer control parameters
            ----------------------------------------------------------------*/
            bdDiscRequest.Type              = LoadType;
            bdDiscRequest.TargetID.Long     = StartPSN.PSN;
            bdDiscRequest.TargetLayer       = StartPSN.Layer;
            bdDiscRequest.ClusterCount      = ClusterRequest;
        }
        /*---------------------------------------------------------------
            Call the main BD disc transfer engine
        ----------------------------------------------------------------*/
        DiscReadStatus = bdStartDiscTransfer();
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

            ClustersLeft = ((bdDiscRequest.ClusterCount - bdDiscRequest.ClustersBuffered) * SECTORS_PER_CLUSTER);
            LastBufferCluster = DataBuffer.CacheEndLBA + ClustersLeft;
            if(LastBufferCluster > pdbGetBDEndLBALayer0())
            {
                bdDiscRequest.ClusterCount -= (LastBufferCluster - pdbGetBDEndLBALayer0()) / ONE_BD_CLUSTER;
                ClusterRequest = 0;
            }
            else
            {
                ClusterRequest = (pdbGetBDEndLBALayer0() - LastBufferCluster) / ONE_BD_CLUSTER;
            }

            SET_INT_ENABLE_MASK( temp );

            bdDiscRequest.Type = LoadType;
            bdDiscRequest.ExtendClusters = (BYTE) ClusterRequest;

            DiscReadStatus = bdStartDiscTransfer();

        }
        else
        {
            SavedClusterRequest = ClusterRequest;
            ClusterRequest = ((pdbGetBDMaxPSN(0) + 1L) - StartPSN.PSN) / ONE_BD_CLUSTER;

            /*---------------------------------------------------------------
                Set up the Disc Transfer control parameters
            ----------------------------------------------------------------*/
            bdDiscRequest.Type            = LoadType;
            bdDiscRequest.TargetID.Long   = StartPSN.PSN;
            bdDiscRequest.ClusterCount    = ClusterRequest;
            bdDiscRequest.TargetLayer     = StartPSN.Layer;

            /*---------------------------------------------------------------
                Call the main BD disc transfer engine
            ----------------------------------------------------------------*/
            DiscReadStatus = bdStartDiscTransfer();

            /*---------------------------------------------------------------
                If the buffering status was good and not all host blocks
                have been buffered continue to load blocks from the second layer
                If all host blocks are in the buffer let the next restart of
                pre-fetching cache blocks from the next layer
            ----------------------------------------------------------------*/
            if ((DiscReadStatus == BUF_GOOD) && (bdDecFlag.fREAD_AHEAD == FALSE))
            {
                /*---------------------------------------------------------------
                    Complete the block request by buffering the remaining blocks
                    that reside on the next layer.
                ----------------------------------------------------------------*/
                ClusterRequest = SavedClusterRequest - ClusterRequest;
                StartPSN.PSN = pdbGetBDStartPSN(1);

                /*---------------------------------------------------------------
                    Set up the Disc Transfer control parameters
                ----------------------------------------------------------------*/
                bdDiscRequest.Type            = LoadType;
                bdDiscRequest.TargetID.Long   = StartPSN.PSN;
                bdDiscRequest.ClusterCount    = ClusterRequest;
                bdDiscRequest.TargetLayer     = LastPSN.Layer; 		/*read next layer */

                /*---------------------------------------------------------------
                    Call the main BD disc transfer engine
                ----------------------------------------------------------------*/
                DiscReadStatus = bdStartDiscTransfer();
            }
        }
    }

    return(DiscReadStatus);
}

#endif // BD_ENABLE
