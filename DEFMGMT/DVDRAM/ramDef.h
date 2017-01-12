/****************************************************************************
*           (c) Copyright 2001 - 2010 Sunext Technology Co., Ltd.
*                           All Right Reserved
*
*
* FILENAME:   ramDef.h
*
* DESCRIPTION
*   This header file describes that the drive supports DVD-RAM specification.
*
*   $Revision: 4 $
*   $Date: 10/02/21 11:38a $
*
*************************** SUNEXT CONFIDENTIAL *****************************/

#ifndef _RAM_DEF_H_
#define _RAM_DEF_H_

/*******************************************************************/
#include ".\defmgmt\common\mrw_def.h"  /* common definition for CD-MRW , DVD+MRW and DVD-RAM */
#include ".\defmgmt\bdwritable\BdWritableCache.h"
#include ".\defmgmt\bdwritable\bdRanAccDef.h"

    #if (DVD_RAM_READ == 1)

#define LOC_DISC_TYPE_10                0         /* These are used for indexing into an array. */
#define LOC_DISC_TYPE_20                1
#define LOC_DISC_TYPE_20_8CM            2

/* 120 mm DVD-RAM disc define */
#define DVDRAM_12CM     St_12_cm

/* User Data layout is defined in SDRAM.h */
#define DVDRAM_ABS_AUX_SADDR            (DVD_ABS_AUX_START_ADDR)
#define DVDRAM_PIPO_START_BLOCK         ((DVDRAM_PIPO_START_PAGE * 0x800) >> DVD_PIPO_BLOCK_SHIFT)  // PIPO blocks are 256 byte resolution (shift of 8 bit)

#define DVDRAM_DATA_SEGA_ADDR           (DVDRAM_DATASEG_A_PAGE * 0x800)
#define DVDRAM_DATA_SEGB_ADDR           (DVDRAM_DATASEG_B_PAGE * 0x800)

#define DVDRAM_WRITE_SEED1_START        (DVDRAM_WRITE_SEED_1_SPAGE * 0x800)
#define DVDRAM_WRITE_SEED1_END          (DVDRAM_WRITE_SEED1_START + 0xBF)  /* 12*16=192 byte */
#define DVDRAM_WRITE_SEED2_START        (DVDRAM_WRITE_SEED_2_SPAGE * 0x800)
#define DVDRAM_WRITE_SEED2_END          (DVDRAM_WRITE_SEED2_START + 0xBF)  /* 12*16=192 byte */

/******************************* Define for Random Write ************************************/
#define DVDRAM_WRITE_PAGE               DVDRAM_PAGE
#define MAX_VERIFY_BLOCKS               2  /* Use 1 ECC Blocks as External RING to Verify */
#define DVDRAM_VERIFY_START_PAGE        DVDRAM_WRITE_PAGE
#define DVDRAM_VERIFY_END_PAGE          (DVDRAM_WRITE_PAGE + MAX_VERIFY_BLOCKS*ONE_DVD_BLOCK - 1)
#define DVDRAM_VERIFY_SIZE              (MAX_VERIFY_BLOCKS *  ONE_DVD_BLOCK)

/* Need to leave 1 page for Aux Data */
#define DVDRAM_VERIFY_AUX_PAGE          (DVDRAM_WRITE_PAGE + MAX_VERIFY_BLOCKS*ONE_DVD_BLOCK)
#define DVDRAM_VERIFY_AUX_ADDR          (SCSDRAM_START_ADDR + DVDRAM_VERIFY_AUX_PAGE * 0x800)
#define DVDRAM_VERIFY_AUX_SIZE          1

#define DVDRAM_VERIFY_PIPO_START_PAGE   (DVDRAM_WRITE_PAGE + MAX_VERIFY_BLOCKS*ONE_DVD_BLOCK + 1)
#define DVDRAM_VERIFY_PIPO_START_BLOCK  (DVDRAM_VERIFY_PIPO_START_PAGE*0x800 >> DVD_PIPO_BLOCK_SHIFT)
#define DVDRAM_VERIFY_PIPO_START_ADDR   (SCSDRAM_START_ADDR + DVDRAM_VERIFY_PIPO_START_PAGE * 0x800)
#define DVDRAM_VERIFY_PIPO_SIZE         ((7328*MAX_VERIFY_BLOCKS)/0x800 + 1)  // One block's PIPO size is 7328 bytes

#define DVDRAM_VERIFY_TOTAL_SIZE        (DVDRAM_VERIFY_SIZE + DVDRAM_VERIFY_AUX_SIZE + DVDRAM_VERIFY_PIPO_SIZE)

#define ORIGINAL_RING_ERASURE_SADDR     (SCSDRAM_START_ADDR + DVD_PIPO_START_ADDR + DVD_PIPO_OFFSET*MAX_DVD_BLOCKS)
#define EXTERNAL_RING_ERASURE_SADDR     (DVDRAM_VERIFY_PIPO_START_ADDR + DVD_PIPO_OFFSET*MAX_VERIFY_BLOCKS)

#define DVD_RAM_MODIFY_WRITE_PAGE       DVDRAM_VERIFY_START_PAGE    /* Temp page for Read-Modify-Write and Verify */
/* DVD_RAM_MODIFY_WRITE use the same RING buffer as DVDRAM_VERIFY */

//#define DVDRAM_SA_CACHE_SPAGE           (DVDRAM_WRITE_PAGE + 23)
#define DVDRAM_SA_CACHE_SPAGE           (DVDRAM_WRITE_PAGE + DVDRAM_VERIFY_TOTAL_SIZE)
#define MAX_SA_CACHE_BLOCKS             6
#define DVDRAM_SA_CACHE_AUX_PAGE        (DVDRAM_SA_CACHE_SPAGE + MAX_SA_CACHE_BLOCKS*ONE_DVD_BLOCK) //one page only
/**************** End of Random Write Define ************************************************/

#define DEFECT_LIST_START_ADDR          (SCSDRAM_START_ADDR + (DVDRAM_PAGE * 0x800))
#define DEFECT_LIST_START_PAGE          (DVDRAM_PAGE)
#define DEFECT_LIST_SIZE_PAGE           0x8B
#define DEFECT_LIST_END_PAGE            (DVDRAM_PAGE+DEFECT_LIST_SIZE_PAGE-1)

/* The following define is based on DRAM_START_DMA */
#define DRAM_START_DMA                  DVD_RAM_DMA_START                  /* Location in DRAM containing the DMA in read from the disc.   */
#define DRAM_START_DDS                  DRAM_START_DMA                     /* Start Location of DDS in DRAM. (Disc Definition Structure)   */
#define DRAM_START_PDL                  (DRAM_START_DMA+0x800UL)           /* Start Location of PDL in DRAM. (Primary Defect List)         */
#define DRAM_END_PDL                    (DRAM_START_DMA + 0x008000UL -1)   /* End   Location of PDL in DRAM.                               */
#define DRAM_START_PROTECT_ADDR         (DRAM_START_DMA + 0x008000UL )     /* Protection page start address  */
#define DRAM_START_SDL                  (DRAM_START_DMA + 0x008800UL )     /* Start Location of SDL in DRAM. (Secondary DEfect List)       */
#define DRAM_END_SDL                    (DRAM_START_SDL + 0x008000UL -1)   /* End   Location of SDL in DRAM.                               */
#define DRAM_FIRST_PDL_ENTRY            (DRAM_START_PDL + 4)
#define DRAM_FIRST_SDL_ENTRY            (DRAM_START_SDL + 24)

#define START_DDS_PAGE                  START_DMA_PAGE
#define START_PDL_PAGE                  (START_DMA_PAGE + 1)
#define START_SDL_PAGE                  (START_DMA_PAGE + 0x11)
#define DVD_RAM_TEMP_ADDR               (DRAM_START_DMA - 0x007800UL )     /* Temp addr for PDL or SDL copy */
#define DVD_RAM_TEMP_PAGE               (START_DMA_PAGE - 0x0F)            /* Temp page for PDL or SDL copy */

#define DVD_RAM_APPLICATION_DATA_ADDR   (DRAM_START_DMA - 0x8000)

#define DVD_RAM_PDL_SHIFT               2          /* 2 bit resolution */

#define DVD_RAM_START_OF_DATA_ZONE      0x31000
#define DVD_RAM_END_OF_DATA_ZONE        0x265F5F
#define DVD_RAM_MAX_LSN                 0x23051F
#define DVDRAM_DISC_ID_ZONE             0x30F00
#define DVDRAM_DRIVE_INFO1_ZONE         0x30F40
#define DVDRAM_DRIVE_INFO2_ZONE         0x30F50
#define DVDRAM_DRIVE_TEST_ZONE          0x30600

#define NUM_ZONES_RAM_10                24
#define NUM_ZONES_RAM_20                35
#define NUM_ZONES_RAM_20_8CM            14

#define MIN_SECTORS_RAM_10              17
#define MAX_SECTORS_RAM_10              40
#define MIN_SECTORS_RAM_20              25
#define MAX_SECTORS_RAM_20              59

#define FIRST_DATA_FIELD_NUMBER_RAM_10  0x31000L
#define FIRST_DATA_FIELD_NUMBER_RAM_20  0x31000L

#define RECORDING_TYPE_BIT              bit4

/* Location of Defect Management Areas(DMA) on the disc. The PSN addresses of the DMAs on the disc. */
#define DMA_1_RAM_10                    0x030F80UL      /* For DVD-RAM Ver. 1.0 */
#define DMA_2_RAM_10                    0x030FC0UL
#define DMA_3_RAM_10                    0x16B480UL
#define DMA_4_RAM_10                    0x16B4C0UL

#define DMA_1_RAM_20                    0x030F80UL      /* For DVD-RAM Ver. 2.0 */
#define DMA_2_RAM_20                    0x030FC0UL
#define DMA_3_RAM_20                    0x265F60UL
#define DMA_4_RAM_20                    0x265FC0UL

#define DMA_1_RAM_20_8CM                0x030F80UL      /* For 80 mm DVD-RAM Ver. 2.0 */
#define DMA_2_RAM_20_8CM                0x030FC0UL
#define DMA_3_RAM_20_8CM                0x0E1220UL
#define DMA_4_RAM_20_8CM                0x0E1280UL

#define MAX_PDL_ENTRY                   7679            /* (15*2048 -  4)/ 4 */
#define MAX_SDL_ENTRY                   3837            /* (15*2048 - 24)/ 8 */

#define DVDRAM_LAST_LSN                 (DVDRAM_12CM == TRUE ? (pdbGetDVDVersion() == DVD_VER_1_0 ? 0x12998F : 0x23051F) : 0x0AE6EF)
#define DVDRAM_TOTAL_LSN                (DVDRAM_LAST_LSN + 1)

#define PHYSICAL_SECTOR_NUMBER_OF_ZONE0 0x88E0      /* 0x31000 - 0x398DF */
#define ZONE0_START_PSN                 0x31000
#define ZONE0_END_PSN                   0x398DF

#define P_LIST                          0x00000000L
#define G1_LIST                         0x80000000L
#define G2_LIST                         0xC0000000L
#define SLR_LONG                        0x40000000L

#define P_LIST_BYTE                     0x00
#define G1_LIST_BYTE                    0x80
#define G2_LIST_BYTE                    0xC0
#define SLR_BYTE                        0x40

#define DVDRAM_FLUSH_WRITE_TIMER        5000        /* 1ms * 5000 = 5s */

#define eRAM_FULL_THRESHOLD             (26*ONE_DVD_BLOCK)    /* 75% of 34 Ecc Block */

/* The Disc NG Level is described in Annex R/Table R.1-1 of DVD-RAM spec. Version 2.1 */
enum
{
    eDiscGood       ,
    eDiscDamaged    ,  /* NGLevel_12, Read control data fail, Disc is damaged, cannot do anything */
    eDiscWriteFail  ,  /* NGLevel_3,  Write error occur, disc can be read only */
    eDiscDMAFail    ,  /* NGLevel_4,  DMA NG, disc can be re-initialized with certification only */
    eDiscInProgress ,  /* NGLevel_5,  In-progress =1, disc can be re-initialized with certification only */
    eDiscSDLFail    ,  /* NGLevel_67, Update SDL fail, send "Warning" to host */
    eDiscSAFull        /* NGLevel_8,  Spare sectors or defect list entries are exhausted, Replacement cannot be performed */
};

enum
{
    eManuCertified = 0x01,
    eUserCertified = 0x02,
    eInProgress    = 0x80
};

enum
{
    eNON_REPLACEMENT,
    eSLIP_REPLACEMENT,
    eLINEAR_REPLACEMENT
};

enum
{
    eNOT_RAM_MODE,
    eRAM_READ_MODE,
    eRAM_WRITE_VERIFY_MODE,
    eRAM_READ_MODIFY_WRITE_MODE
};

enum
{
    eReplaced    = 0x00,
    eNotReplaced = 0x40
};


enum
{
    ePrimaryFull       = 0x01,
    eSupplementaryFull = 0x02
};


enum
{
    eRamFlushDone = 0x01,
    eRamFlushAll  = 0x02
};


enum
{
    eNotWriteProtected = 0x00,
    eWriteProtected    = 0x80
};


/* read/write streaming */
enum
{
    eRamNoStream    = 0x00,
    eRamReadStream  = 0x01,
    eRamWriteStream = 0x02
};

enum
{
    eDVDRAM_TDV_RA513CW     = 0x02,       //ECC150,UNB1.0
    eDVDRAM_TDV_RA52x_533CW = 0x03,       //scratch disc, DEV1.0,defect
    eDVDRAM_TDV_RA521CW     = 0x04,       //scratch disc
    eDVDRAM_TDV_RA525CW     = 0x05,       //defect disc

};


enum {
    eDA = 0,            /* DA AREA                  */
    eSA = 0x80,         /* SA AREA                  */
    eSA_MASK = 0x80
};


/********************************************** Define for DVDRAM_NEW_CACHE ***********************************/
#define eRAMHashTableSize          64          /* entry */
#define eRAMSegmentSize            16          /* in Sector */
#define eRAMSegmentBitMapFull      0xFFFFL
/* 2MB DRAM */
#define eRAMCacheStartSegment      0
#define eRAMCacheStartPage         (eRAMCacheStartSegment * eRAMSegmentSize)
#define eRAMCacheSegmentCnt        (DVDRAM_BUF_TOTAL_BLK_COUNT/ONE_DVD_BLOCK)     /* used by CacheManagement */
#define eRAMTotalPageNo            (eRAMSegmentSize * eRAMCacheSegmentCnt)
#define eRAMFULL_THRESHOLD         (eRAMCacheSegmentCnt - 3)
#define eRAMUNDERRUN_THRESHOLD      16


#define eRAMTotalHashListNode      eRAMCacheSegmentCnt
#define eRAMTotalCacheHitNode      eRAMCacheSegmentCnt
#define RAM_NULL_SEG               0xFF
#define RAM_NULL_ENTRY             0xFF
#define RAM_NULL_LIST              0xFF
#define RAM_NULL_LSN               0xFFFFFFFF
#define RAM_NULL_SA_BLOCK          0xFFFF

/* DVD-RAM Timer for flushing write segments */

#define DVDRAM_FLUSH_WRITE_TIMER1          80         /* 1ms * 5000 = 5s */

/* DVD-RAM Timer for Update SDL */
#define DVDRAM_UPDATE_SDL_TIMER            5000       /* 1ms * 5000 = 5s */


/* DVD-RAM Write Flags */
#define     fDVDRAM_WriteStartError        RWDisc.dvRAM.RAMWriteFlag.bWriteStartError
#define     fDVDRAM_FirstWrite             RWDisc.dvRAM.RAMWriteFlag.bFirstWrite

/* MACROs to access segment's info */
#define DVDRAM_GET_SEG_ECCPSN(SegNo)           (RWDisc.dvRAM.SegmentInfo[SegNo].BlkStartPSN & 0x00FFFFFF)
#define DVDRAM_GET_SEG_SLRBIT(SegNo)           (RWDisc.dvRAM.SegmentInfo[SegNo].BlkStartPSN & SLR_LONG)
#define DVDRAM_GET_SEG_ECCLSN(SegNo)           RWDisc.dvRAM.SegmentInfo[SegNo].EccStartLSN
#define DVDRAM_GET_SEG_PAGE(SegNo)             RWDisc.dvRAM.SegmentInfo[SegNo].SegmentDramPageNo
#define DVDRAM_GET_SEG_TYPE(SegNo)             RWDisc.dvRAM.SegmentInfo[SegNo].SegmentType
#define DVDRAM_GET_SEG_FILLINFO(SegNo)         RWDisc.dvRAM.SegmentInfo[SegNo].BlockFilledInfo
#define DVDRAM_GET_SEG_LISTNUM(SegNo)          RWDisc.dvRAM.SegmentInfo[SegNo].ListNum
#define DVDRAM_GET_SEG_NEXTENTRY(SegNo)        RWDisc.dvRAM.SegmentInfo[SegNo].NextEntry
#define DVDRAM_GET_SEG_ATTRIBUTE(SegNo)        RWDisc.dvRAM.SegmentInfo[SegNo].Attr
#define DVDRAM_GET_SEG_SAINDEX(SegNo)          RWDisc.dvRAM.SegmentInfo[SegNo].SABlockIndex
#define DVDRAM_GET_SEG_REPPSN(SegNo)           RWDisc.dvRAM.SegmentInfo[SegNo].ReplacementPSN

#define DVDRAM_SET_SEG_ECCPSN(SegNo, Psn)      RWDisc.dvRAM.SegmentInfo[SegNo].BlkStartPSN = Psn
#define DVDRAM_SET_SEG_ECCLSN(SegNo, Lsn)      RWDisc.dvRAM.SegmentInfo[SegNo].EccStartLSN = Lsn
#define DVDRAM_SET_SEG_PAGE(SegNo, Pg)         RWDisc.dvRAM.SegmentInfo[SegNo].SegmentDramPageNo = Pg
#define DVDRAM_SET_SEG_TYPE(SegNo, Type)       RWDisc.dvRAM.SegmentInfo[SegNo].SegmentType = Type
#define DVDRAM_SET_SEG_FILLINFO(SegNo, fill)   RWDisc.dvRAM.SegmentInfo[SegNo].BlockFilledInfo = fill
#define DVDRAM_SET_SEG_LISTNUM(SegNo, Num)     RWDisc.dvRAM.SegmentInfo[SegNo].ListNum = Num
#define DVDRAM_SET_SEG_NEXTENTRY(SegNo, Next)  RWDisc.dvRAM.SegmentInfo[SegNo].NextEntry = Next
#define DVDRAM_SET_SEG_ATTRIBUTE(SegNo, Atb)   RWDisc.dvRAM.SegmentInfo[SegNo].Attr = Atb
#define DVDRAM_SET_SEG_SAINDEX(SegNo, Sindex)  RWDisc.dvRAM.SegmentInfo[SegNo].SABlockIndex = Sindex
#define DVDRAM_SET_SEG_REPPSN(SegNo, Reppsn)   RWDisc.dvRAM.SegmentInfo[SegNo].ReplacementPSN = Reppsn

#define DVDRAM_SEGMENT_FULL_FILLED(SegNo)      (RWDisc.dvRAM.SegmentInfo[SegNo].BlockFilledInfo == eRAMSegmentBitMapFull)
#define DVDRAM_SET_SEGMENT_FULL_FILLED(SegNo)  RWDisc.dvRAM.SegmentInfo[SegNo].BlockFilledInfo = eRAMSegmentBitMapFull
#define DVDRAM_SET_SEGMENT_NO_FILLED(SegNo)    RWDisc.dvRAM.SegmentInfo[SegNo].BlockFilledInfo = 0

#define DVDRAM_GET_CACHE_PAGE(index)           RWDisc.dvRAM.CacheListLib[index].StartDramPgNo
#define DVDRAM_GET_CACHE_PBN(index)            RWDisc.dvRAM.CacheListLib[index].StartPBN
#define DVDRAM_GET_CACHE_LEN(index)            RWDisc.dvRAM.CacheListLib[index].Length
#define DVDRAM_GET_CACHE_HITSTATUS(index)      RWDisc.dvRAM.CacheListLib[index].HitStatus
#define DVDRAM_GET_CACHE_SEGNO(index)          RWDisc.dvRAM.CacheListLib[index].SegmentNo


/********************************************/
/* Macro to generate hash cache table index */
#define RamHashing( PSN )  (BYTE)((PSN >> 4) & 0x3F)    /* 64 entries with ECC block start address */
                                 /*     ^^^^   ^^^^^        */
                                 /*one ECC */ /* 64 entries */

/* Macro to adjust SegmentInfo index */
#define RamADD_SEGMENT( SegNo, offset ){       \
    SegNo += offset;                           \
    if (SegNo >= eRAMCacheSegmentCnt)          \
        SegNo -= eRAMCacheSegmentCnt;          \
}

/* Macro to adjust SegmentInfo index */
#define RamSUB_SEGMENT( SegNo, offset ){       \
    if (SegNo < offset)                        \
        SegNo += eRAMCacheSegmentCnt;          \
    SegNo -= offset;                           \
}


#define RamNEXT_SEG_NO(SegNo)  {           \
    if (SegNo == eRAMCacheSegmentCnt - 1)  \
        SegNo = 0;                      \
    else                                \
        SegNo++;                        \
}

#define RamADD_PAGE(page, offset)  {   \
    page += offset;                     \
    page %= DVDRAM_BUF_TOTAL_BLK_COUNT;    \
}

#define DVDRAM_COPY_DELAY_AFTER_READ       {\
    if (RWDisc.dvRAM.CopyDelayNeeded == TRUE) \
    {                               \
        DelaymS(5);                 \
        RWDisc.dvRAM.CopyDelayNeeded = FALSE; \
    }                               \
}

#define DVD_HOST_CMD_MAX_BLOCKS   11 //we will limit host cmd length not over than 0xA0
#define LINK_LIST_FIFO_NUM        8 //Chip provides 8 entries for encoder link list FIFO.
#define eSupSA                    0x8000 //Indicate a SA block is in Primary Area or Supplementary Area
#define SW_DEFECT_FOUND_FLAG      0x80000000 //Indicate a defective block is found during Stream-Write

/********************************************/

typedef struct {
    BYTE    Start;         /* The Number of the first Segment in the Write List */
    BYTE    End;           /* The Number of the last Segment in the Write List */
    BYTE    Size;          /* The total Segments in the Write List */
    BYTE    Action;        /* A Flag indicates the action status of the write list */
} tRamWriteList;

typedef struct {
    BYTE    StartSegNo;    /* The Number of the first Segment in the Write List */
    BYTE    Size;          /* The total Segments in the Write List */
} tRamFlushInfo;


/* Cache Hit List Node */
typedef struct {
    LONG    StartLSN;
    USHORT  StartDramPgNo;
    USHORT  Length;
    BYTE    HitStatus;
    BYTE    SegmentNo;
} RamCacheListT;


typedef struct {
    LONG    NextStartLSN;        /* the start LSN of uncompleted blocks */
    USHORT  RemainLength;        /* the number of uncompleted blocks */
} RamCacheResultT;


/*  Read Cache Segment Structure */
typedef struct {
    ULONG    EccStartLSN;        /* The ECC start LSN */
    ULONG    BlkStartPSN;        /* The ECC start PSN */
    ULONG    ReplacementPSN;     /* If SDL block, we will save the replaced info */
    USHORT   BlockFilledInfo;    /* bitmap of occupied pagess within segment, LSB<->1st page */
    USHORT   SegmentDramPageNo;  /* The start page # of this segment */
    USHORT   SABlockIndex;       /* Replacement block index, used for ReWrite process */
    BYTE     SegmentType;        /* The type of this segment */
    BYTE     Attr;               /* The Attribute of this segment, DA or SA, ... */
    BYTE     NextEntry;          /* The Next segment in a Write List */
    BYTE     ListNum;            /* The list number that this segment belongs to */
} SegInfoRamT;


/* Next Info */
typedef struct {
    LONG NextLSN;       /* Next LSN                 */
    LONG NextLen;       /* Next Len                 */
} tRamNextInfo;

/* Block Entry */
typedef struct {
    ULONG   LSN;         /* Logical Address          */
    ULONG   DefPSN;      /* Defect PSN in SDL Entry  */
    USHORT  Len;         /* Block Length             */
    BYTE    Attr;        /* SA/DA Area               */
} tRamBlockEntry;

typedef struct {
    ULONG  BlkStartLSN;
    ULONG  ReplacementPSN;
    USHORT BlockFilledInfo;
} tRamSABlock;


enum
{
    eRamData_ID,
    eRamHeader_ID
};

#define DIS_DVDDEC_ICE_CONT     4

/********************************************** Define for DVDRAM_NEW_CACHE ***********************************/


#define PID_BAD_COUNTER_THR     0x0A
#define PREBUF_THRESHOLD        0x150
#define DvdRamFastCache         0

    #else /* #if (DVD_RAM_READ == 1) */
#define DvdRamFastCache         0

    #endif /* #if (DVD_RAM_READ == 1) */

#endif /* #ifndef _RAM_DEF_H_ */
