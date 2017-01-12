/*****************************************************************************
*                (c) Copyright 2000 - 2007 SUNEXT DESIGN, INC.
*                            All Rights Reserved
*
*
* FILENAME:     $Workfile: bdWritableCache.h $
*
* DESCRIPTION   This file contains BD Writable Cache related #define(s) and typedefs
*
* $Revision: 11 $
* $Date: 10/02/10 4:13p $
*
**************************** SUNEXT CONFIDENTIAL ****************************/

#ifndef __BD_WRITABLE_CACHE_H__
#define __BD_WRITABLE_CACHE_H__

#include ".\common\globtype.h"
#include ".\player\Plrdb.h"
#include ".\oem\Oem_info.h"

/********************************************** Define for bdWritableCache ***********************************/
#define BDWT_FAST_CACHE             1
#define eBDHashTableSize            64          /* entry */
#define eBDSegmentSize              32          /* one cluster is 32 Sectors */
#define eBDSegmentBitMapFull        0xFFFFFFFFL
/* 4MB DRAM */
#define eBDCacheStartSegment        0
#define eBDCacheStartPage           (eBDCacheStartSegment * eBDSegmentSize)
#define eBDCacheSegmentCnt          MAX_BDWRITABLE_CLUSTERS
#define eBDTotalPageNo              (eBDSegmentSize * eBDCacheSegmentCnt)
#define eBD_CACHE_FULL_THRESHOLD    (eBDCacheSegmentCnt - 3)
#define eBD_UNDERRUN_THRESHOLD      8
#define eBDTotalHashListNode        eBDCacheSegmentCnt
#define eBDTotalCacheHitNode        eBDCacheSegmentCnt
#define BD_NULL_SEG                 0xFF
#define BD_NULL_ENTRY               0xFF
#define BD_NULL_LIST                0xFF
#define BD_NULL_LSN                 0xFFFFFFFF
#define BD_NULL_SA_BLOCK            0xFFFF

/* Timer for flushing write segments */
#define BD_FLUSH_WRITE_TIMER1        8000        /* 1ms * 8000 = 8s */
#define BD_UPDATE_DMA_TIMER          8000        /* 1ms * 8000 = 8s */

/* MACROs to access segment's info */
#define BD_GET_SEG_ECCPSN(SegNo)           (BdwtDisc.SegmentInfo[SegNo].BlkStartPSN & 0x0FFFFFFF)
#define BD_GET_SEG_SLRBIT(SegNo)           (BdwtDisc.SegmentInfo[SegNo].BlkStartPSN & SLR_LONG)
#define BD_GET_SEG_ECCLSN(SegNo)           BdwtDisc.SegmentInfo[SegNo].EccStartLSN
#define BD_GET_SEG_PAGE(SegNo)             BdwtDisc.SegmentInfo[SegNo].SegmentDramPageNo
#define BD_GET_SEG_TYPE(SegNo)             BdwtDisc.SegmentInfo[SegNo].SegmentType
#define BD_GET_SEG_FILLINFO(SegNo)         BdwtDisc.SegmentInfo[SegNo].BlockFilledInfo
#define BD_GET_SEG_LISTNUM(SegNo)          BdwtDisc.SegmentInfo[SegNo].ListNum
#define BD_GET_SEG_NEXTENTRY(SegNo)        BdwtDisc.SegmentInfo[SegNo].NextEntry
#define BD_GET_SEG_ATTRIBUTE(SegNo)        BdwtDisc.SegmentInfo[SegNo].Attr
#define BD_GET_SEG_SAINDEX(SegNo)          BdwtDisc.SegmentInfo[SegNo].SABlockIndex
#define BD_GET_SEG_REPPSN(SegNo)           BdwtDisc.SegmentInfo[SegNo].ReplacementPSN

#define BD_SET_SEG_ECCPSN(SegNo, Psn)      BdwtDisc.SegmentInfo[SegNo].BlkStartPSN = Psn
#define BD_SET_SEG_ECCLSN(SegNo, Lsn)      BdwtDisc.SegmentInfo[SegNo].EccStartLSN = Lsn
#define BD_SET_SEG_PAGE(SegNo, Pg)         BdwtDisc.SegmentInfo[SegNo].SegmentDramPageNo = Pg
#define BD_SET_SEG_TYPE(SegNo, Type)       BdwtDisc.SegmentInfo[SegNo].SegmentType = Type
#define BD_SET_SEG_FILLINFO(SegNo, fill)   BdwtDisc.SegmentInfo[SegNo].BlockFilledInfo = fill
#define BD_SET_SEG_LISTNUM(SegNo, Num)     BdwtDisc.SegmentInfo[SegNo].ListNum = Num
#define BD_SET_SEG_NEXTENTRY(SegNo, Next)  BdwtDisc.SegmentInfo[SegNo].NextEntry = Next
#define BD_SET_SEG_ATTRIBUTE(SegNo, Atb)   BdwtDisc.SegmentInfo[SegNo].Attr = Atb
#define BD_SET_SEG_SAINDEX(SegNo, Sindex)  BdwtDisc.SegmentInfo[SegNo].SABlockIndex = Sindex
#define BD_SET_SEG_REPPSN(SegNo, Reppsn)   BdwtDisc.SegmentInfo[SegNo].ReplacementPSN = Reppsn

#define BD_SEGMENT_FULL_FILLED(SegNo)      (BdwtDisc.SegmentInfo[SegNo].BlockFilledInfo == eBDSegmentBitMapFull)
#define BD_SET_SEGMENT_FULL_FILLED(SegNo)  BdwtDisc.SegmentInfo[SegNo].BlockFilledInfo = eBDSegmentBitMapFull
#define BD_SET_SEGMENT_NO_FILLED(SegNo)    BdwtDisc.SegmentInfo[SegNo].BlockFilledInfo = 0

#define BD_GET_CACHE_PAGE(index)           BdwtDisc.CacheListLib[index].StartDramPgNo
#define BD_GET_CACHE_PBN(index)            BdwtDisc.CacheListLib[index].StartPBN
#define BD_GET_CACHE_LEN(index)            BdwtDisc.CacheListLib[index].Length
#define BD_GET_CACHE_HITSTATUS(index)      BdwtDisc.CacheListLib[index].HitStatus
#define BD_GET_CACHE_SEGNO(index)          BdwtDisc.CacheListLib[index].SegmentNo


/********************************************/
/* Macro to generate hash cache table index */
#define BdHashing( PSN )  (BYTE)((PSN >> 5) & 0x3F)    /* 64 entries with ECC cluster start address */
                                 /*     ^^^^   ^^^^^        */
                               /*one Cluster */ /* 64 entries */

/* Macro to adjust SegmentInfo index */
#define BD_ADD_SEGMENT( SegNo, offset ){       \
    SegNo += offset;                           \
    if (SegNo >= eBDCacheSegmentCnt)          \
        SegNo -= eBDCacheSegmentCnt;          \
}

/* Macro to adjust SegmentInfo index */
#define BD_SUB_SEGMENT( SegNo, offset ){       \
    if (SegNo < offset)                        \
        SegNo += eBDCacheSegmentCnt;          \
    SegNo -= offset;                           \
}


#define BD_NEXT_SEG_NO(SegNo)  {           \
    if (SegNo == eBDCacheSegmentCnt - 1)  \
        SegNo = 0;                      \
    else                                \
        SegNo++;                        \
}

#define BD_ADD_PAGE(page, offset)  {   \
    page += offset;                     \
    page %= eBDTotalPageNo;    \
}


#define BD_LINK_LIST_FIFO_NUM        5          //Chip provides 5 entries for encoder link list FIFO.
#define SW_DEFECT_FOUND_FLAG      0x80000000    //Indicate a defective block is found during Stream-Write

/********************************************/

typedef struct {
    BYTE    Start;    /* The Number of the first Segment in the Write List */
    BYTE    End;      /* The Number of the last Segment in the Write List */
    BYTE    Size;     /* The total Segments in the Write List */
    BYTE    Action;   /* A Flag indicates the action status of the write list */
} tBdWriteList;

typedef struct {
    BYTE    StartSegNo;    /* The Number of the first Segment in the Write List */
    BYTE    Size;          /* The total Segments in the Write List */
} tBdFlushInfo;


/* Cache Hit List Node */
typedef struct {
    LONG    StartPSN;
    USHORT  StartDramPgNo;
    USHORT  Length;
    BYTE    HitStatus;
    BYTE    SegmentNo;
} BdCacheListT;


typedef struct {
    LONG    NextStartPSN;       /* the start LSN of uncompleted blocks */
    USHORT  RemainLength;       /* the number of uncompleted blocks */
} BdCacheResultT;


typedef enum SegmentType {
    eEmptySeg,                  /* Segment is empty */
    eReadSeg,                   /* Segment contians data read from disc */
    eWriteSeg,                  /* Segment contains data to be written onto disc */
    eWriteLocked,               /* Segment to be used for receiving write data */
    eReadLocked,                /* Segment to be used for keeping read data */
    eWritingSeg,                /* Segment contains data being written */
    eReWriteSeg
} SegmentT;

/*  Read Cache Segment Structure */
typedef struct {
    ULONG    EccStartLSN;        /* The ECC start LSN */
    ULONG    BlkStartPSN;        /* The ECC start PSN */
    ULONG    BlockFilledInfo;    /* bitmap of occupied pagess within segment, LSB<->1st page */
    USHORT   SegmentDramPageNo;  /* The start page # of this segment */
    USHORT   SABlockIndex;       /* Replacement block index, used for ReWrite process */
    BYTE     SegmentType;        /* The type of this segment */
    BYTE     Attr;               /* The Attribute of this segment, DA or SA, ... */
    BYTE     NextEntry;          /* The Next segment in a Write List */
    BYTE     ListNum;            /* The list number that this segment belongs to */
} BdSegInfoT;


/* Hash List Node */
typedef struct stHashList {
    BYTE    SegNo;
    struct  stHashList  *NextNode;
    struct  stHashList  *PrevNode;
} HashListT;

enum
{
    eNOT_USERDATA,
    eREAD_DATA_FROM_DISC,  /* Request from Host    */
    ePRE_BUFFERING,        /* Request from Drive   */
    eREAD_MODIFY_WRITE,    /* Used in RMW mode     */
    eREAD_ERROR_RATE       /* Used in Bler command */
};

enum
{
    eNON_ICE_BLOCK = 0x00,
    eICE_BLOCK = 0x80,
    eCERTIFIED_BLOCK = 0x40
};

enum
{
    eLIST_IDLE,
    eLIST_IN_PROGRESS,
    eLIST_MERGED
};


enum
{
    eMissed,
    eHit
};

enum
{
    eWriteCmd = 0x01,
    eReadCmd  = 0x02
};

enum {
    eReadAccess,
    eWriteAccess
};
/********************************************** Define for bdWritableCache ***********************************/


/********************************************** Extern for bdWritableCache ***********************************/
extern  void    BDcmInit(void);
extern  void    BDcmResetCacheHitList(void);
extern  BOOL    BDcmAddCacheHitNode(LONG StartPSN, USHORT Cnt, BYTE HitFlag, BYTE SegNo);
extern  BdCacheListT * BDcmGetCacheHitNode(USHORT index);
extern  USHORT  BDcmGetFilledInfo(ULONG *pBitMap, BYTE *pOffset);
extern  void    BDSetNextInfo(LONG LSN, USHORT Length);
extern  USHORT BDcmFindTargetPSN(LONG StartLba, USHORT Count, BYTE Access);
extern  BYTE    BDcmGetNextAvailableSegment(BYTE AccessType);
extern  void    BDcmFreeSegment(BYTE SegNo);
extern  void    BDcmCopyPage(USHORT SrcPage, USHORT TgtPage, USHORT PageCount);
extern  HashListT *BDcmGetNextHashListNode(BYTE SegNo);
extern  BOOL    BDcmRegisterSegment(BYTE SegNo);
extern  void    BDcmModifyWriteSegment(BYTE SegNo);
extern  void    BDClearCache(BYTE CmdType);
/********************************************** Extern for bdWritableCache ***********************************/



#endif  //__BD_WRITABLE_CACHE_H__

