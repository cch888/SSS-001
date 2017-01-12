/*****************************************************************************
*                (c) Copyright 2000 - 2007 SUNEXT DESIGN, INC.
*                            All Rights Reserved
*
*
* FILENAME:     $Workfile: bdRanAccDef.h $
*
* DESCRIPTION   This file contains BD Random Access related #define(s) and typedefs
*
* $Revision: 16 $
* $Date: 10/02/10 4:13p $
*
**************************** SUNEXT CONFIDENTIAL ****************************/

#ifndef __BD_RANDOM_ACCESS_DEF_H__
#define __BD_RANDOM_ACCESS_DEF_H__

#include ".\common\globtype.h"
#include ".\player\Plrdb.h"
#include ".\oem\Oem_info.h"
#include ".\defmgmt\bdwritable\BdWritableCache.h"

//**********************************************************************************************
//
//                                 #Defines for BD Random Access
//
//**********************************************************************************************
#define eBdReadCmd          bit0
#define eBdWriteCmd         bit1

/******************************* Define for BD Write ************************************/
#define BD_VERIFY_BLOCKS                3  /* Use 3 clusters as External RING to Verify */
#define BD_VERIFY_START_PAGE            0x66C
#define BD_VERIFY_END_PAGE              (BD_VERIFY_START_PAGE + BD_VERIFY_BLOCKS * ONE_BD_CLUSTER - 1)
#define BD_VERIFY_SIZE                  (BD_VERIFY_BLOCKS *  ONE_BD_CLUSTER)

#define BD_VERIFY_AUX_PAGE              (BD_VERIFY_END_PAGE + 1)
#define BD_VERIFY_AUX_ADDR              (SCSDRAM_START_ADDR + BD_VERIFY_AUX_PAGE * 0x800)
#define BD_VERIFY_AUX_SIZE              3

#define BD_MODIFY_WRITE_PAGE            0x6CF

/* Flush Message :*/
#define eMsgFlushSingle     1
#define eMsgFlushAll        2
#define eMsgFlushDone       0

#define BDWT_MSG_PRN        0
/*******************************************************************
    ENUMERATIONS
*******************************************************************/
/* read/write streaming */
enum
{
    eBdNoStream    = 0x00,
    eBdReadStream  = 0x01,
    eBdWriteStream = 0x02
};


enum
{
    eNOT_BD_MODE,
    eBD_READ_MODE,
    eBD_WRITE_VERIFY_MODE,
    eBD_READ_MODIFY_WRITE_MODE
};


/*******************************************************************
    STRUCTURES
*******************************************************************/
/*-----------------------------------------------------------------
    BD Writable Global Variables
------------------------------------------------------------------*/
typedef struct
{
    BYTE    CommandExe;
    BYTE    Stream;

    ULONG   PreLSN;                     /* Pre-buffering current decoding block LSN */
    ULONG   PrePSN;                /* Pre-buffering Start decoding block LSN */
    USHORT  PrePage;                    /* Pre-buffering current decoding block page */
    BYTE    EccBuffered;                /* Pre-buffering registered blocks count */
    BYTE    PreReadingOK;               /* A flag, indicates Pre-buffering is allowed or not */
    BYTE    PreSegNo;                   /* Pre-buffering current decoding block SegNo */
    BYTE    ReadDataMode;               /* A flag, indicates data reading request is coming from Host or Drive */
    FLAG    StartBufferingOK;           /* A flag, indicates start buffering is OK or not */


    BdSegInfoT  SegmentInfo[eBDCacheSegmentCnt];
    HashListT   HashListLib[eBDTotalHashListNode];
    HashListT   HashTbl[eBDHashTableSize];
    BdCacheListT  CacheListLib[eBDTotalCacheHitNode];
    BdCacheResultT    CacheResult;

    USHORT  CacheHitNodeNum;
    BYTE    NextReadSegNo;              /* Index for next available read segment */
    BYTE    NextWriteSegNo;             /* Index for next available write segment */

    BYTE    WriteSegmentCount;          /* Total segment for write */
    BYTE    CacheAccess;

    ULONG       flush_timer;            /* recording start timer    */
    ULONG       Remain_ECC_COUNT;
    LONG        LastWriteLSN;           /* last written packet on disc */
    LONG        AppendableLSN;
    USHORT      AppendablePage;
    USHORT      TotalEccWritten;

    BYTE        FlushMessage;           /* Message for BG flush write buffer */
    BYTE        APPEND_PACKET_ALLOWED;
    BYTE        BgPreBuffer;
    BYTE        BgPreBufAbort;
    FLAG        PrevCommandIsRead;
    FLAG        PrevCommandIsWrite;
    FLAG        CpuUsage8X;

    #if BDWT_FAST_CACHE
    BYTE        CacheFull;
    USHORT      ContinueIcedCnt;
    #endif


} BdwtGlobalS;


/*-----------------------------------------------------------------
    External Functions and Variables
------------------------------------------------------------------*/
extern  BYTE CmdSeekOk;

/*-----------------------------------------------------------------
    Macros
------------------------------------------------------------------*/
#define BD_DMA_UPDATE_FLAG      BdwtDisc.SrriSBM0UpdateFlag || \
                                BdwtDisc.DflUpdateFlag


#endif

