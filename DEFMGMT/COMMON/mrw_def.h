/****************************************************************************
*                  (c) Copyright 2001  -  2005 SUNEXT DESIGN, INC.
*                           All Right Reserved
*
*
* FILENAME:   mrw_def.h
*
* DESCRIPTION
*   This header file describes that the drive supports Mt.Rainier specification.
*
*   $Revision: 2 $
*   $Date: 10/02/21 11:32a $
*
*************************** SUNEXT CONFIDENTIAL *****************************/

#ifndef _MRW_DEF_H_
#define _MRW_DEF_H_

#include ".\common\sdram.h"
#include ".\al\REG_HAL.h"
#include ".\al\REG_cd_decoder.h"
#include ".\al\REG_cd_encoder.h"
#include ".\al\REG_cpycmp.h"
#include ".\al\REG_host.h"

#if (DVD_RAM_READ == 1)
#define ePACKET_SIZE        32
#define CONSTANT_SIZE    /* use Constants for all MRW size */

/****************** common definition in MRW & DVD+RW ******************/
#define DEFECT_FOUND_ALWAYS_TRUE    1      /* mrw defect found, "1" means using always-verify-after-write method */
#define MRW_Philip_2120             0      /* Philip Verification Disc Protection */
//#define MRW_Detect                       /* Use for monitoring MRW start-up */

#define eHashTableSize          32          /* entry */
#define eSegmentSize            32          /* in Block */
#define eSegmentBitMapFull      0xFFFFFFFFL
/* 2MB DRAM */
#define eTempSegmentCnt         0
#define eCacheStartSegment      0
#define eCacheStartPage         (eCacheStartSegment * eSegmentSize)
#define eCacheSegmentCnt        14          /* used by CacheManagement */
#define eTotalPageNo            (eSegmentSize * (eCacheSegmentCnt + eTempSegmentCnt))
#define eFULL_THRESHOLD         11
#define eTotalHashListNode      eCacheSegmentCnt
#define eTotalCacheHitNode      eCacheSegmentCnt
#define NULL_SEG                0xFF

#define SEGQUEUE_SIZE           eCacheSegmentCnt

#define GAA_Space           1
#define DMA_Space           0

/* For MRW_WriteSTA */
#define eFinalSTA           0
#define eTempSTA            1


/* for WriteList status */
#define eWriteListIdle      0
#define eWriteListWriting   1
#define eWriteListComplete  2

/* Flush Message :*/
#define eMsgStartFlush      1
#define eMsgFlushAll        2
#define eMsgFlushDone       0

#define eByPage             0       /* Current SAList sorted by the ascending order of Page */
#define eByLBN              1       /* Current SAList sorted by the ascending orger of LBN */

/* cmCopySACache dir */
#define eFromSABuffer       eBufferAreaToUserArea
#define eToSABuffer         eUserAreaToBufferArea

#define eIOPC_DONE          0x01
#define eWRITE_PMA_DONE     0x02
#define eWRITE_MTA_DONE     0x04
#define eVERIFY_MTA_DONE    0x08

/* MRW_DecodeMode */
#define eNONMRW_MODE        0       /* non mrw read */
#define eMRWREAD_MODE       1       /* read mode    */
#define eMRWVERIFY_MODE     2       /* verify mode  */
#define eMRWSTREAM_MODE     3       /* streaming mode*/
#define eMRWCHECK_MODE      4       /* check C1/C2  */
#define eMRW_RMW_MODE       5       /* Read-Modify-Write read mode 082103_zxm */

/* SA_NODE Type: */
#define eEmptySANode        0
#define eWriteSANode        1
#define eReadSANode         2

#define MAX_RECOVER_RETRY   10
#define MAX_VERIFY_RETRY    2
#define JUMP_DEFECT         1
#define eC3Defect       0xC3
#define eC2Defect       0xC2
#define eC1Defect       0xC1
#define eNotDefect      0

/* SegType */
#define eNonWriteAttr       0x0     /* segment for read or empty        */
#define eWriteAttr          0x1     /* segment contiaining write data   */
#define eVerifyAttr         0x2     /* segment to be verified           */
#define eStreamAttr         0x4     /* segment is streaming data        */

/* attributes for storing segments */
#define eWriteDelay         eWriteAttr      /* Blocks can be written later in BG  */
#define eWriteImmed         eWriteAttr      /* Blocks need be written immediately */
#define eWriteAndVerify     (eWriteAttr | eVerifyAttr)  /* Blocks need be written and verified*/
#define eWriteNoChange      0x0             /* segment's attr no change         */
#define eWriteStreaming     (eWriteAttr | eStreamAttr)
#define eVerifyOnly         eVerifyAttr

/* NULL PBN */
#define eNULL_PBN           0x0FFFFFF
#define VERIFY_BITMAP_LEN   0x500           /* total bytes of verified packet bitmap*/


/*  Read Cache Segment Structure */
typedef struct stSegInfo {
    LONG    PacketStartPBN;     /* The pasket start PBN */
    ULONG   BlockFilledInfo;    /* bitmap of occupied pagess within segment, LSB<->1st page */
    USHORT  SegmentDramPageNo;  /* The start page # of this segment */
    BYTE    SegmentType;       /* The type of this segment */
    BYTE    Frequency;          /* The access freqency of this segment */
} SegInfoT;


/* MRWSize */
typedef struct {
    USHORT GAASize;     /* GAA Size                 */
    USHORT SASize;      /* SA  Size                 */
    USHORT DASize;      /* DA  Size                 */
} tMRWSize;



/* Init Size */     //??
enum {
    eGAAInitSize = 32,      /* GAA Size */
    eSAInitSize  = 8,       /* SA  Size */
    eDAInitSize  = 136,     /* DA  Size */
    eSTAInitSize = 33       /* STA Size */
};

/* DRAM Offset in eDRAMP_DFMG */
enum {
    eDRAMO_Search = 0x0000,         /* SA/DA Area Search  */
    eDRAMO_Change = 0x0190,         /* MIP/DTB ChangeFlag */
    eDRAMO_UpDate = 0x01A0,         /* SDT UpDate         */
    eDRAMO_MIP    = 0x02A0,         /* MIP                */
    eDRAMO_SIP    = 0x0300          /* SIP                */
};

/* DRAM COPY */
enum COPYMODE {
    eUserAreaToBufferArea   =   (HAL_CPYCMP_SFLAG),       /* TFLAG = 0, SFLAG = 1 */
    eUserAreaToUserArea     =   (HAL_CPYCMP_TFLAG|HAL_CPYCMP_SFLAG),/* TFLAG = 1, SFLAG = 1 */
    eBufferAreaToBufferArea =   0x00,           /* TFLAG = 0, SFLAG = 0 */
    eBufferAreaToUserArea   =   (HAL_CPYCMP_TFLAG)        /* TFlag = 1, SFlag = 0 */
};

/* DRAM Offset in eDRAMP_CACHMGR */
enum {
    eDRAMO_DELIST  = 0x0000,        /* Defect List Start */
    eDRAMO_ENTRYBP = 0x300          /* Defect Entry Recover Memory */
};

/* Defect Entry in DRAM */
/* Offset */
enum {
    eDE_NextPBN     = 0,    /* Next PBN     */
    eDE_NextLen     = 3,    /* Next Len     */
    eDE_EntryNum    = 5,    /* Entry Num    */

    eDE_EntryDEFPBN = 6,    /* Entry-DefectPBN  */
    eDE_EntryREPPBN = 9,    /* Entry-ReplacePBN */
    eDE_EntryLen    = 12    /* Entry-Len    */
};

/* RecoverEntry */
enum {
    eRecoverEntrySize = 4,

    eRE_DTB = 0,
    eRE_BP  = 1,
    eRE_LEN = 3
};

/* Defect Entry */
enum {
    eDefectEntryMax = 32    /* Block Entry Max          */
};

/* Size */
enum {
    eDefectEnrtySize = 8    /* Entry Size   */
};

/* the bitmap size used for verification is only 2KB, each
 * bit represents 1 block; so the maximum number of blocks
 * to verify is 2K * 8 = 16K blocks. For faster response from
 * host, verify 1K blocks at a time */
enum {
    eMaxVerifyBlock = 0x400
};


/* gbMRWDiscAccess */
/* bit 0-1: disc type (r/w or r/o)
 * bit 6-7: format type (full or partial)
 * bit 4-5: format type of old disc, used in re-starting
 *          format or format used MRW disc */
enum
{
    eMRW_Disc_NotMRW = 0x00,
    eMRW_Disc_ReadOnly = 0x01,
    eMRW_Disc_ReadWrite = 0x02,
    eMRW_Disc_AccessType = 0x03,
    eMRW_Disc_PartialFormatted = 0x40,
    eMRW_Disc_FullyFormatted = 0x80,
    eMRW_Disc_FormatType = 0xC0,
/* These values are used to format used MRW disc */
    eMRW_OldDisc_PartialFormatted = 0x10,
    eMRW_OldDisc_FullyFormatted = 0x20,
    eMRW_OldDisc_FormatType = 0x30
};

/***** MIP *****/
/* Size */
enum {
    eMIPPBN    = -182,      /* -182, MIP PBN    */
    eMIPSize   = 32,        /* MIP Size         */
    eMIPLen    = 80         /* MIP Len          */
};

/* Offset */
enum {
    eMipSign         =  0,  /* Sign 'MIP' or 'SIP'        */
    eMipFormatVer    =  3,  /* Format  Version            */
    eMipReadingVer   =  4,  /* Reading Version            */
    eMipWriteVer     =  5,  /* Write   Version            */
    eMipReserved1    =  6,  /* Reserved                   */
    eMipUpdateCount  =  8,  /* UpDate Count               */
    eMipReserved2    = 12,  /* Reserved                   */
    eMipLastLBN      = 13,  /* Last LBN in last Data Area (DM & PF v1.1) */
    eMipGAASize      = 16,  /* GAA Size                   */
    eMipSASize       = 18,  /* SA  Size                   */
    eMipDASize       = 20,  /* DA  Size                   */
    eMipReserved3    = 22,  /* Reserved                   */
    eMipDiscStats    = 24,  /* Disc Status                */
    eMipLastWAdrsPnt = 25,  /* Last Write Address Point    */
    eMipLastVAdrsPnt = 28,  /* Last Verfy Address Point    */
    eMipMDTNum       = 31,  /* MDT Num                    */
    eMipMDT          = 32,  /* MDT PBN                    */
    eMipSDT          = 56   /* SDT PBN                    */
};

/***** MDT *****/
/* Size */
enum {
    eMDTMax    = 8,         /* MDT Max  */
    eMDTSize   = 32,        /* MDT Size */
    eMDTPbn    = -438       /* "Normal" MDT start PBN */
};

/***** DTB  *****/
/* Size */
enum {
    eDTBMax    = 8,         /* DTB Max    */
    eDTBLen    = 1568,      /* DTB Size   */
    eEntryMax  = 256,       /* Entry Max  */
    eEntrySize = 6          /* Entry Size */
};

/* Offset */
enum {
    eDtbSignMDT      =  0,  /* Sign 'MDT' or 'SDT'        */
    eDtbIndex        =  3,  /* Index 7-4:Packet|3-0:Defect */
    eDtbUpdateCount  =  4,  /* UpDate Count                */
    eDtbEntryNum     =  8,  /* Entry Num                   */
    eDtbFreeBP       = 10,  /* Free  BP                    */
    eDtbUnuseBP      = 12,  /* Unuse BP                    */
    eDtbTableStatus  = 14,  /* Table Status                */
    eDtbMappingFlg   = 15,  /* Mapping Flag                */
    eDtbReserved1    = 16,  /* Reserved                   */
    eDtbFstUsedPBN   = 17,  /* Fast Used PBN               */
    eDtbReserved2    = 20,  /* Reserved                   */
    eDtbLstUsedPBN   = 21,  /* Last Used PBN               */
    eDtbReserved3    = 24,  /* Reserved                   */
    eDtbFstFreePBN   = 25,  /* Fast Free PBN               */
    eDtbReserved4    = 28,  /* Reserved                   */
    eDtbLstFreePBN   = 29,  /* Last Free PBN               */
    eDtbEntry        = 32   /* Entry Start                 */
};

/* Entry Offset */
enum {
    eEO_DefectPBN    = 0,   /* Defect  PBN        */
    eEO_ReplacePBN   = 3    /* Replace PBN        */
};

/* Entry Status1 */
enum {
    eES1_UsedEntry       = 0x00,    /* Used               */
    eES1_UsedEntryNotRep = 0x10,    /* Used Not Replaced  */
    eES1_FreeEntry       = 0x20,    /* Free               */
    eES1_UnusableEntry   = 0x30     /* Unusable           */
};

/* Entry Status2 */
enum {
    eES2_Replaced        = 0x00,    /* Replaced           */
    eES2_NotReplaced     = 0x10     /* Not Replaced       */
};
enum {
    eES2_SingleBlock     = 0x00,    /* Single block       */
    eES2_Packet          = 0x20     /* Packet(32block)    */
};

enum {
    eSdDTB  = 0,                    /* DTB Top    */
    eSdTEMP = eMDTMax * eDTBMax     /* TEMP       */
};

/***** Block Entry *****/
/* Offset */
enum {
    eBE_NextLBN     = 0,    /* Next LBN     */
    eBE_NextLen     = 3,    /* Next Len     */
    eBE_EntryNum    = 5,    /* Entry Num    */
    eBE_EntryLBN    = 6,    /* Entry-LBN    */
    eBE_EntryPBN    = 9,    /* Entry-PBN    */
    eBE_EntryLen    = 12,   /* Entry-Len    */
    eBE_EntryAttr   = 14    /* Entry-Attr   */
};

/* Size */
enum {
    eBlockEntrySize = 9     /* Entry Size   */
};

/* for CheckDefectInDTB */
enum {
    eFoundDefect,       /* At least one defect in DA registerred */
    eNoDefect,          /* No defect of DA in DTB */
    eNotFound           /* Not found, can't determine */
};


/***** Entry Assign *****/
/* flg */
enum {
    eDfmgRep   = 0x00,  /* Replace     */
    eDfmgNoRep = 0x10   /* Non Replace */
};


/***** Get Defect Info *****/
/* flg */
enum  {
    eDfmgRead  = 0,     /* Read    */
    eDfmgWrite = 1,     /* Write   */
    eDfmgDAOnly= 2,     /* DA Only */
    eDfmgSWrite= 3,     /* For Write_Streaming */
    eSAWriteOnly = 4
};


/* read/write streaming */
enum
{
    eNoStream = 0x00,
    eReadStream = 0x01,
    eWriteStream = 0x02
};


enum
{
    eBGFinalizeSTA,
    eBGFinalizeLeadOut,
    eBGFinalizePMA,
    eBGFinalizeLeadIn,
    eBGFinalizeMTA,
    eBGFinalizeDone
};

enum {
    eWROffset = 8   /* Write Offset */
};

/* All the possible error types of DTB */
enum ERRTYPE {
    eErrOK                      = 0,

    eErrSignature               = 1,
    eErrPacketNumber            = 2,
    eErrDTBNumber               = 3,

    eErrNumOfDTEntry            = 4,
    eErrBPOf1stFreeEntry        = 5,
    eErrBPOf1stUnusableEntry    = 6,
    eErrNumOfUsedEntry          = 7,
    eErrNumOfFreeEntry          = 8,
    eErrNumOfUnusableEntry      = 9,

    eErrPBNOfUsedOrder          = 10,
    eErrPBNOfFirstUsed          = 11,
    eErrPBNOfLastUsed           = 12,
    eErrStatusOfUsedEntry       = 13,
    eErrPBNOfDefectiveOrder     = 14,
    eErrPBNOfDefectiveRange     = 15,
    eErrPBNOfDefectiveNotInDA   = 16,
    eErrPBNOfReplacedNotInSA    = 17,

    eErrPBNOfFreeOrder          = 18,
    eErrPBNOfFirstFree          = 19,
    eErrPBNOfLastFree           = 20,
    eErrStatusOfFreeEntry       = 21,
    eErrPBNOfReplacementOrder   = 22,
    eErrPBNOfReplacementRange   = 23,
   eErrPBNOfReplacementNotInSA = 24,

    eErrStatusOfUnusableEntry   = 25,
    eErrPBNOfUnusableNotInSA    = 26,

    eErrMappingFlag0            = 27,
    eErrMappingFlag1            = 28
};

/*******************************************************************/
#define MinOf(n1, n2, n3) ((n1>n2)?((n2>n3)?n3:n2):((n1>n3)?n3:n1))
#define MaxOf(n1, n2, n3) ((n1<n2)?((n2<n3)?n3:n2):((n1<n3)?n3:n1))

#define MOD_RING_PAGE(page)             {\
    if (page > (Iram_usiRingEP + 1))        \
        page = page - Iram_usiRingEP - 1 + Iram_usiRingSP;\
}

/*******************************************************************/


/*******************************************************************/
/* common definition in MRW & DVD+RW */

/* for MRWDiscCmdExe */

/* Cache Hit List Node */
typedef struct stCacheList {
    LONG    StartPBN;
    USHORT  StartDramPgNo;
    USHORT  Length;
    BYTE    HitStatus;
    BYTE    SegmentNo;
} CacheListT;


#endif /* #if (DVD_RAM_READ == 1) */
/*******************************************************************/

#define eNO_WAIT_MODE               0
#define eWAIT_MODE                  1

#define fDVDWriteStartError         RWDisc.DVDPLUSRW.WriteFlag.bWriteStartError
#define fDVDRWFirstWrite            RWDisc.DVDPLUSRW.WriteFlag.bFirstWrite
#define fDVDCopyComplete            RWDisc.DVDPLUSRW.WriteFlag.bCopyComplete
#define fDVDZoneSwitchOK            RWDisc.DVDPLUSRW.WriteFlag.bZoneSwitchOK
#define fDVDBGFRestartRequest       RWDisc.DVDPLUSRW.WriteFlag.bBGFRestartRequest
#define fDVDTrackSizeChanged        RWDisc.DVDPLUSRW.WriteFlag.bTrackSizeChanged
#define fDVDFDCBChanged             RWDisc.DVDPLUSRW.WriteFlag.bFDCBChanged

/* MRWDiscSem: Disc access Semaphore */
#define eReadDisc                   0x01
#define eWriteDisc                  0x02
#define eVerifyDisc                 0x04
#define eDeiceDisc                  0x08
#define eModifyReadDisc             0x10

typedef struct
{
    USHORT      bWriteStartError:1;
    USHORT      bFirstWrite:1;
    USHORT      bCopyComplete:1;
    USHORT      bZoneSwitchOK:1;
    USHORT      bTrackSizeChanged:1;
    USHORT      bBGFRestartRequest:1;
    USHORT      bFDCBChanged:1;
} MRWWriteFlagT;

typedef struct
{
    USHORT      bReqFormatValid:1;      /* format request valid */
    USHORT      bExeFormatProcess:1;    /* FormatProcess() is in execution */
    USHORT      bBGFormatValid:1;       /* backgorund enable / disable */
    USHORT      bFormatAbort:1;         /* format abort request */
    USHORT      bVerifyAbort:1;         /* verify abort request */
    USHORT      bReqFrmtSem:1;          /* format request semaph    */
    USHORT      bMRW_StopBGFormatFlag:1;
    USHORT      bMRW_EarlyEject:1;

} MRWFmtFlagT;

#define     fDVDReqFormatValid          RWDisc.DVDPLUSRW.BGFmtFlag.bReqFormatValid
#define     fDVDExeFormatProcess        RWDisc.DVDPLUSRW.BGFmtFlag.bExeFormatProcess
#define     fDVDBGFormatValid           RWDisc.DVDPLUSRW.BGFmtFlag.bBGFormatValid
#define     fDVDFormatAbort             RWDisc.DVDPLUSRW.BGFmtFlag.bFormatAbort
#define     fDVDVerifyAbort             RWDisc.DVDPLUSRW.BGFmtFlag.bVerifyAbort
#define     fDVDReqFrmtSem              RWDisc.DVDPLUSRW.BGFmtFlag.bReqFrmtSem
#define     fDVDStopBGFormatFlag        RWDisc.DVDPLUSRW.BGFmtFlag.bMRW_StopBGFormatFlag
#define     fDVDEarlyEject              RWDisc.DVDPLUSRW.BGFmtFlag.bMRW_EarlyEject

#endif
