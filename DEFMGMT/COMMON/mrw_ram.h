/***************************************************************************
*             (c) Copyright 2001 - 2010 Sunext Technology Co., Ltd.
*                           All Right Reserved
*
* FILENAME:   mrw_ram.h
*
* DESCRIPTION
*   global variables for mrw
*
* CHANGES:
* REF VERSION DATE      WHO     DETAIL
* --- ------- --------  ------  ---------------------------------------------
*       1.00  04/29/03  XMZ     Cleaned up
*
*   $Revision: 3 $
*   $Date: 10/02/22 7:16p $
*
**************************** SUNEXT CONFIDENTIAL ***************************/

#ifndef _MRW_RAM_H_
#define _MRW_RAM_H_

#include ".\oem\oem_info.h"
#include ".\defmgmt\common\mrw_def.h"

    #if (DVD_RAM_READ == 1)
#include ".\hif\gp_ram.h"
#include ".\defmgmt\dvdram\dvDefMgr.h"


/******************************************************
*   External Memory Variable Definitions
*******************************************************/
/*--------------DVD-RAM External Memory Variable------------------- */
extern  BOOL    DvdRamFormatType1(void);
extern  BOOL    DvdRamFormatType2(void);
extern  BOOL    DvdRamFormatType3(void);
extern  BOOL    RamDiscInitializeNoCert(void);
extern  BYTE    GetDiscCertificationFlag(void);
extern  void    DvdRamVerifyProcess(ULONG StartPSN, ULONG BlockCount);
extern  void    PrintDMA(void);
extern  void    DebugProcessDMA(void);
extern  void    InitializeDefList(void);
extern  void    DVDRAMVarsInit(void);
extern  BOOL    CheckDVDRAMCommand(void);
extern  void    DisableDVDRAMBuffer(void);
extern  BOOL    RAMWriteNoneUserDataProcess(void);

extern  BYTE    DiscTypeIndex;              /* A local version of DiscType. Will contain 0 for DVD RAM 1.0 Contains 1 for DVD RAM 2.0       */
extern  BYTE    LocalZoneNumber;            /* A local disc zone number.  Will contain 24 for DVD RAM 1.0 Contains 35 for DVD RAM 2.0       */
extern  BYTE    RamDiscStatus;              /* */
extern  long    format_size;
extern ULONG    volatile dvdLastDecodedPSN;
extern USHORT   DVDEBUFPG;
extern ULONG    dvdramLastDecodedLSN;
extern ULONG    dvdramDiscRequestStartLSN;
extern ULONG    dvdramReadAheadStartLSN;
extern const    ULONG DmaLocation[4][3];
    #endif // #if (DVD_RAM_READ == 1)

/************************* new define union *************************/
typedef union
{
    #if (DVD_RAM_READ == 1)
    struct
    {
        DDSTYPE *StDDSp;                    /* A pointer to the DDS structure */
        PDLTYPE *StPDLp;                    /* A pointer to the PDL structure */
        SDLTYPE *StSDLp;                    /* A pointer to the SDL structure */
        FragListType    FragList;           /* Contains List of Read/Write Fragments. */
        ULONG   CurrentPID;
        ULONG   DefectPSN;
        ULONG   VerifyStartPSN;
        ULONG   MaxLSN;                     /* MaxLSN is decided by format size */
        ULONG   WriteStartLSN;
        ULONG   WriteStopLSN;
        USHORT  DefectiveBitMap;            /* A bitmap indicates which sectors are defective in a Block */
        USHORT  PrimarySARemainBlk;         /* Store the remain Blocks in Primary SA */
        USHORT  SupSARemainBlk;             /* Store the remain Blocks in Supplemetary SA */
        BYTE    ReplaceMode;                /* Contains eSLIP_REPLACEMENT and eLINEAR_REPLACEMENT 2 Mode */
        BYTE    DecodeMode;
        BYTE    CommandExe;
        BYTE    Stream;                     /* read/write streaming */
        FLAG    DVDRAM_DEFECT_FOUND;        /* A flag shows a defect block is found when writting */
        FLAG    SdlUpdateFlag;              /* A flag shows the status if any SDL entry has been updated */
        FLAG    ReadSkip;                   /* Read skip enabled */

        ULONG       flush_timer;            /* recording start timer    */

        MRWWriteFlagT   RAMWriteFlag;

        ULONG       Remain_ECC_COUNT;
        LONG        LastWriteLSN;           /* last written packet on disc */
        LONG        AppendableLSN;
        USHORT      AppendablePage;
        USHORT      TotalEccWritten;

        BYTE        FlushMessage;           /* Message for BG flush write buffer */
        BYTE        RecordState;            /* statement of recording   */
        BYTE        APPEND_PACKET_ALLOWED;
        BYTE        RELEASE_PACKET_ALLOWED;
        BYTE        DiscSem;
        BYTE        CopyDelayNeeded;        /* TRUE for delay if using copy functions right after reads */
        BYTE        Prebuffering;           /* indicate pre-buffering in progress */
        FLAG        ReadingDMA;             /* indicate DMA is reading */
        FLAG        BlankDMA;               /* indicate DMA is blank   */

        SegInfoRamT    SegmentInfo[eRAMCacheSegmentCnt];
        HashListT   HashListLib[eRAMTotalHashListNode];
        HashListT   HashTbl[eRAMHashTableSize];
        RamCacheListT  CacheListLib[eRAMTotalCacheHitNode];
        RamCacheResultT    CacheResult;

        USHORT  CacheHitNodeNum;

        BYTE    NextReadSegNo;              /* Index for next available read segment */
        BYTE    NextWriteSegNo;             /* Index for next available write segment */
        BYTE    WriteSegmentCount;          /* Total segment for write */
        BYTE    CacheAccess;
        BYTE    SA_DACache;

        BYTE    SAListCnt;                  /* Number of SA Block in SA Cache */
        tRamSABlock    SACacheList[MAX_SA_CACHE_BLOCKS];  /* A List to store SA Cache Block info. */

        ULONG   PreLSN;                     /* Pre-buffering current decoding block LSN */
        ULONG   PreStartLSN;                /* Pre-buffering Start decoding block LSN */
        USHORT  PrePage;                    /* Pre-buffering current decoding block page */
        BYTE    EccBuffered;                /* Pre-buffering registered blocks count */
        BYTE    PreReadingOK;               /* A flag, indicates Pre-buffering is allowed or not */
        BYTE    PreSegNo;                   /* Pre-buffering current decoding block SegNo */
        BYTE    ReadDataMode;               /* A flag, indicates data reading request is coming from Host or Drive */
        BYTE    ContinuteReadCount;         /* Counter for continuted reading */
        FLAG    StartBufferingOK;           /* A flag, indicates start buffering is OK or not */
        FLAG    IceHasBeenDetected;         /* A flag, indicates ICE detection is done or not */

        tRamBlockEntry     REntry[DVD_HOST_CMD_MAX_BLOCKS];
        tRamNextInfo       Rnxinfo;

        ULONG   SPINUP_TIMER;
        BYTE    FailCode;

        USHORT          TotalAppendLength;
        BYTE            TotalWriteListNumber;
        tRamWriteList   WriteList[eRAMCacheSegmentCnt];
        tRamWriteList   VerifyList[eRAMCacheSegmentCnt];
        FLAG            DEFECT_FOUND;
        FLAG            PrevCommandIsRead;
        FLAG            PrevCommandIsWrite;
        BYTE            NextFilledSegNo;
        BYTE            ReWriteCount;
        BYTE            ReWriteSegment[eRAMCacheSegmentCnt];
        FLAG            WGHasBeenDown;
        FLAG            WriteSpareArea;
        FLAG            HeadIdNoGood;
        FLAG            WobbleOutLock;
        FLAG            WSInterrupt;
        FLAG            WriteFail;
        FLAG            VendorWrite;
        tRamFlushInfo   FlushInfo;
        FLAG            SkippedBlock;      /* SLR bit = 1, The defective block contains real-time data and is not replaced. */
        ULONG           StreamWriteDefLSN; /* Store the defective block LSN and Flag during Stream-Write */
        BYTE            AbexMid;
    #if DISCARD_RAM_DECODE
        BYTE            ContinuteIceCount;      /* Counter for continuous ice area reading */
    #endif

        #if E_DIAGS
        BYTE            BLER_ID;
        #endif

        BYTE            BgPreBuffer;
        BYTE            BgPreBufAbort;

        #if DvdRamFastCache
        BYTE            CacheFull;
        BYTE            FastCacheXferSgNo;
        USHORT          FastCacheXferStPage;
        USHORT          FastCacheXferLen;
        #endif
    } dvRAM;
    #endif // #if (DVD_RAM_READ == 1)


    struct
    {
        MRWFmtFlagT     BGFmtFlag;
        MRWWriteFlagT   WriteFlag;
        BYTE        FormatState;            /* format statement     */
        BYTE        ReqFormatCode;          /* format request code  */
        BYTE        FormatQuickStart;
        BYTE        FormatRestart;
        BYTE        RecordState;            /* statement of recording   */
        BYTE        ReceiveFormatCommand;

        LONG        LastWritePSN;           /* last written packet on disc */

    #if (DVD_PLUSRW_BGF == 1)
        ULONG       bgf_timer;              /* background interval timer ( unit of 1 msec ) */
        vULONG      AutoBGFormatTimer;      /* background time      */

        vBYTE       BGFStopCmd;             /* ATAPI command that stops BG Format */
        BYTE        BGFStatus;              /* BG format status: none, start not run, start running, finish */

        LONG        Deice_StartPSN;
        LONG        Deice_length;

        BYTE        RWDiscSem;
        BYTE        gbMRWDiscAccess;
        BYTE        RW_BGFormatEvent;      /* for GetEventStatus Notification Cmd */
    #endif // #if (DVD_PLUSRW_BGF == 1)
    } DVDPLUSRW;

} tRWDiscU;

extern  tRWDiscU RWDisc;

#endif // #ifndef _MRW_RAM_H_
