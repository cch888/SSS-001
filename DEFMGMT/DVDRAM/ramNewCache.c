/****************************************************************************
*          (c) Copyright 2001 - 2010 Sunext Technology Co., Ltd.
*                           All Right Reserved
*
* FILENAME:   ramNewCache.c
*
*  $Revision: 4 $
*  $Date: 10/12/21 4:17p $
*
* DESCRIPTION
*   This file contains new cache buffer utility for DVD-RAM
*
* Notes:
*
**************************** SUNEXT CONFIDENTIAL ****************************/

#include ".\common\globals.h"
#include ".\common\globtype.h"
#include ".\common\sdram.h"
#include ".\common\com_im.h"
#include ".\hif\gp_mac.h"
#include ".\hif\rd_util.h"
#include ".\hif\sv_util.h"
#include ".\dvd\dvMacros.h"
#include ".\dvd\dvisr.h"
#include ".\dvd\dvGvars.h"
#include ".\dvd\dvseek.h"
#include ".\defmgmt\dvdram\ramread.h"

#if (DVD_RAM_READ == 1)
#include ".\defmgmt\dvdram\dvDefMgr.h"
#include ".\defmgmt\dvdram\ramdef.h"
#include ".\defmgmt\dvdram\ramExtern.h"
#include ".\defmgmt\common\mrw_ram.h"
#include ".\defmgmt\common\mrw_def.h"
#include ".\dvd\dvHostRd.h"
#include ".\dvd\dvBufMgr.h"

/*************************************************************************/
/*                         Declarations                                  */
/*************************************************************************/
void RAMcmResetCacheHitList();
void RAMcmCombineSegment(BYTE ReadSegNo, BYTE WriteSegNo);
void RAMcmCopyPage(USHORT SrcPage, USHORT TgtPage, USHORT PageCount);
BYTE RAMcmGetNextAvailableSegment(BYTE AccessType);
HashListT *RAMcmGetNextHashListNode(BYTE SegNo);
ULONG RAMcmGetSegmentPSN(ULONG BlockLSN);
void RAMInitWriteList(void);
void RAMInitRingBuffer(void);
void RAMInitRingBuffer_NoFlush(void);
BOOL RAMcmCheckWriteFlush(BYTE num);


/*************************************************************************/
/*                         Functions                                     */
/*************************************************************************/



/*------------------------------------------------------------------------
    Description:
        Initializes the Cache Management;

    Input:  None

    Output: None
-------------------------------------------------------------------------*/
void RAMcmInit(void)
{
    /* init the Ring Buffer */
    RAMInitRingBuffer();

    /* init the SACache */
    RAMInitSACache();
}


void RAMInitRingBuffer(void)
{
    send_msg80(SEND_MRW_MSG,0xFD0301);
    RAMInitRingBuffer_NoFlush();
}

void RAMInitRingBuffer_NoFlush(void)
{
    BYTE    i;
    send_msg80(SEND_MRW_MSG,0xFD0301);
    DVDRAMStopPrebuffering();

    /* init the dram segment */
    for(i=0; i<eRAMCacheSegmentCnt; i++)
    {
        RWDisc.dvRAM.SegmentInfo[i].EccStartLSN = 0;
        RWDisc.dvRAM.SegmentInfo[i].BlkStartPSN = 0;
        RWDisc.dvRAM.SegmentInfo[i].BlockFilledInfo = 0L;
        RWDisc.dvRAM.SegmentInfo[i].SegmentDramPageNo = i * 16 + eRAMCacheStartPage;
        RWDisc.dvRAM.SegmentInfo[i].SegmentType = eEmptySeg;
        RWDisc.dvRAM.SegmentInfo[i].NextEntry = RAM_NULL_SEG;  //To be used in write list
        RWDisc.dvRAM.SegmentInfo[i].ListNum = RAM_NULL_LIST;   //To be used in write list
    }

    RWDisc.dvRAM.NextWriteSegNo = 0;
    RWDisc.dvRAM.NextReadSegNo = 0;
    RWDisc.dvRAM.WriteSegmentCount = 0;


    /* init the Hash Table */
    for(i=0; i<eRAMHashTableSize; i++)
    {
        RWDisc.dvRAM.HashTbl[i].SegNo = RAM_NULL_SEG;
        RWDisc.dvRAM.HashTbl[i].NextNode = NULL;
        RWDisc.dvRAM.HashTbl[i].PrevNode = NULL;

    }

    /* init the Hash List Lib Node */
    for (i = 0; i < eRAMTotalHashListNode; i++)
    {
        RWDisc.dvRAM.HashListLib[i].SegNo = RAM_NULL_SEG;
        RWDisc.dvRAM.HashListLib[i].NextNode = NULL;
        RWDisc.dvRAM.HashListLib[i].PrevNode = NULL;
    }

    /* init the CacheHitList */
    for (i=0; i<eRAMTotalCacheHitNode; i++)
    {
        RWDisc.dvRAM.CacheListLib[i].StartLSN = 0;
        RWDisc.dvRAM.CacheListLib[i].StartDramPgNo = 0;
        RWDisc.dvRAM.CacheListLib[i].Length = 0;
        RWDisc.dvRAM.CacheListLib[i].HitStatus = eMissed;
    }

    RAMcmResetCacheHitList();

    /* init the WriteList */
    RAMInitWriteList();

    RWDisc.dvRAM.APPEND_PACKET_ALLOWED = FALSE;
    RWDisc.dvRAM.RELEASE_PACKET_ALLOWED = FALSE;

    /* init the ReWrite segment */
    RWDisc.dvRAM.ReWriteCount = 0;
    for(i=0; i<eRAMCacheSegmentCnt; i++)
    {
        RWDisc.dvRAM.ReWriteSegment[i] = RAM_NULL_SEG;
    }
}

// TBD!
void RAMInitWriteList(void)
{
    BYTE i;

    RWDisc.dvRAM.TotalWriteListNumber = 0;

    for (i = 0; i < eRAMCacheSegmentCnt; i++)
    {
        RWDisc.dvRAM.WriteList[i].Start = RAM_NULL_SEG;
        RWDisc.dvRAM.WriteList[i].End   = RAM_NULL_SEG;
        RWDisc.dvRAM.WriteList[i].Size  = 0;
        RWDisc.dvRAM.WriteList[i].Action = 0;
    }
}

//TBD!
void RAMInitSACache(void)
{
    BYTE i;

    RWDisc.dvRAM.SAListCnt = 0;
    RWDisc.dvRAM.WriteSpareArea = CLEAR;

    for (i = 0; i < MAX_SA_CACHE_BLOCKS; i++)
    {
        RWDisc.dvRAM.SACacheList[i].BlkStartLSN = RAM_NULL_LSN;
        RWDisc.dvRAM.SACacheList[i].BlockFilledInfo = 0;
        RWDisc.dvRAM.SACacheList[i].ReplacementPSN = 0;
    }
}


/*------------------------------------------------------------------------
    Description:
        Resets the Cache Hit List;
    Input:  None

    Output: None
-------------------------------------------------------------------------*/
void RAMcmResetCacheHitList()
{
    RWDisc.dvRAM.CacheHitNodeNum = 0;
}


/*------------------------------------------------------------------------
    Description:
        Adds a new Cache hit node into the list;

    Input:  StartLSN, EndLSN - the LSN range
            HitFlag - indicates the status of segment in DRAM: eHit/eMissed
            SegNo   - Segment index

    Output: TRUE  - Add success;
            FALSE - The CacheHitList will be full;
-------------------------------------------------------------------------*/
BOOL RAMcmAddCacheHitNode(LONG StartLSN, USHORT Cnt, BYTE HitFlag, BYTE SegNo)
{
    USHORT Page;

    Page = (USHORT)(StartLSN & LAST_LBA_OF_DVDBLOCK);
    Page += RWDisc.dvRAM.SegmentInfo[SegNo].SegmentDramPageNo;

    RWDisc.dvRAM.CacheListLib[RWDisc.dvRAM.CacheHitNodeNum].StartLSN = StartLSN;
    RWDisc.dvRAM.CacheListLib[RWDisc.dvRAM.CacheHitNodeNum].StartDramPgNo = Page;
    RWDisc.dvRAM.CacheListLib[RWDisc.dvRAM.CacheHitNodeNum].Length = Cnt;
    RWDisc.dvRAM.CacheListLib[RWDisc.dvRAM.CacheHitNodeNum].HitStatus = HitFlag;
    RWDisc.dvRAM.CacheListLib[RWDisc.dvRAM.CacheHitNodeNum].SegmentNo = SegNo;


    if (RWDisc.dvRAM.CacheAccess == eReadAccess)
    {
        if (RWDisc.dvRAM.SegmentInfo[SegNo].SegmentType == eReadSeg)
            RWDisc.dvRAM.SegmentInfo[SegNo].SegmentType = eReadLocked;
    }
    else    /* eWriteAccess */
    {
        // do nothing
    }

    RWDisc.dvRAM.CacheHitNodeNum++;
    if (RWDisc.dvRAM.CacheHitNodeNum == eRAMTotalCacheHitNode)
        return FALSE;
    else
        return TRUE;

}


/*------------------------------------------------------------------------
    Description:
        Returns the pointer to the specific cache hit list node

    Input:  index - index of target node;

    Output: pointer to the cache hit list node;

-------------------------------------------------------------------------*/
RamCacheListT *RAMcmGetCacheHitNode(USHORT index)
{
    if (index < RWDisc.dvRAM.CacheHitNodeNum)
        return ((RamCacheListT *)&RWDisc.dvRAM.CacheListLib[index]);
    else
        return NULL;
}


/*----------------------------------------------------------------------------
  Name: RAMcmConvertBlockMap()
  Description:
        converts a range into a 16-bit bitmap.

  Input:  Offset - start bit;
          Length - the number of contiguouse bits;

  Output: a 16-bit BitMapping flag

          Offset: 0   1   2   ...     15
          Bit:    0   1   2   ...     15

  Note:   0 < Length <= 16
          0 <= Offset < 16
---------------------------------------------------------------------------*/
USHORT RAMcmConvertBlockMap(BYTE Offset, USHORT Length)
{
    USHORT map, bitptn;
    USHORT i;

    bitptn = map = 0x1L << Offset;
    for (i = 0; i < Length; i++, bitptn <<= 1)
        map |= bitptn;

    return map;
}


/*----------------------------------------------------------------------------
  Name: RAMcmGetFilledInfo()
  Description:
        converts a bitmap into range;

  Input:  pBitMap - address of bit map;
          pOffset - address of offset

  Output: the number of consecutive bit '1' in the bit map;
----------------------------------------------------------------------------*/
USHORT RAMcmGetFilledInfo(USHORT *pBitMap, BYTE *pOffset)
{
    USHORT BitCnt = 0;

    while (1)
    {
        if ((*pBitMap) & 0x01)
            BitCnt ++;          /* count the consecutive '1' */
        else
        {
            if (BitCnt != 0)    /* end of consecutive '1' */
                break;

            (*pOffset) ++;
            if ((*pBitMap) == 0)        /* all the bits '1' are counted */
                break;
        }
        (*pBitMap) >>= 1;
    }
    return BitCnt;
}


/*------------------------------------------------------------------------
    Description:

    Input:  LSN - The Start LSN of un-processed blocks
            Length - The number of blocks

    Output: None
-------------------------------------------------------------------------*/
void RAMSetNextInfo(LONG LSN, USHORT Length)
{
    RWDisc.dvRAM.CacheResult.NextStartLSN = LSN;
    RWDisc.dvRAM.CacheResult.RemainLength = Length;
}

/*------------------------------------------------------------------------
    Description:
        Searches the DRAM buffer and generates a CacheHitList to return
        the information of target blocks

    Input:  StartLSN - Start LSN address
            Count - The number of blocks
            Access - Read or write access;

    Output: The number of cache hit list node;
-------------------------------------------------------------------------*/
USHORT RAMcmFindTargetLSN(LONG StartLSN, USHORT Count, BYTE Access)
{
    BYTE cTblIndex;
    USHORT Cnt, Blkcnt, Remain, Length;
    HashListT * pHash, *pNewHash, *pPrevHash;
    BYTE SegNo, Offset;
    LONG PacketLSN, PacketPSN, EndLSN;
    LONG LSN0, LSN1;
    USHORT BitMap, Newbitmap = 0;
    BOOL ret;

    RAMcmResetCacheHitList();

    RWDisc.dvRAM.SA_DACache = Access & eSA_MASK;

    Access &= ~eSA_MASK;
    RWDisc.dvRAM.CacheAccess = Access;

    PacketLSN = StartLSN & DVD_BLOCK_MASK;  /*(StartLSN / 16L) * 16L*/
    Offset = (BYTE) (StartLSN & 0x0F);      /*(StartLSN - PacketLSN)*/

    Remain = Count;

    while (Remain != 0)
    {
        cTblIndex = RamHashing(PacketLSN);

        //send_msg5SValue(SEND_MRW_MSG,"PacketLSN",4,PacketLSN);
        //send_msg5SValue(SEND_MRW_MSG,"cTblIndex",1,cTblIndex);

        EndLSN = StartLSN + Remain;
        LSN0 = PacketLSN + 16L;

        //send_msg5SValue(SEND_MRW_MSG,"Remain",4,Remain);
        //send_msg5SValue(SEND_MRW_MSG,"StartLSN",4,StartLSN);
        //send_msg5SValue(SEND_MRW_MSG,"EndLSN",4,EndLSN);


        if (EndLSN > LSN0)
        {
            Length = 16 - Offset;
            Remain = Remain - Length;
            EndLSN = LSN0;
        }
        else
        {
            Length = Remain;
            Remain = 0;
        }


        pPrevHash = (HashListT *) &RWDisc.dvRAM.HashTbl[cTblIndex];
        pHash = pPrevHash->NextNode;

        Newbitmap = 0L;

        /*packet in loop***************/
        while (Length != 0)
        {
            if (pHash == NULL)
            {
                /* fully missed */

                /* allocate a new segment */
                SegNo = RAMcmGetNextAvailableSegment(Access);

                if (SegNo == RAM_NULL_SEG)      /* no free segment */
                {
                    RAMSetNextInfo(StartLSN, Length + Remain);
                    return RWDisc.dvRAM.CacheHitNodeNum;
                }

                if (SegNo != pPrevHash->SegNo)
                {
                    /* add  a new entry into hash table */
                    pNewHash = RAMcmGetNextHashListNode(SegNo);
                    pNewHash->SegNo = SegNo;

                    pNewHash->NextNode = NULL;
                    pNewHash->PrevNode = pPrevHash;

                    pPrevHash->NextNode = pNewHash;
                }

                /* add a node to CacheList */
                Cnt = (USHORT) (EndLSN - StartLSN);
                if (RAMcmAddCacheHitNode(StartLSN, Cnt, eMissed, SegNo) == FALSE)
                {
                    RAMSetNextInfo(EndLSN, Length+ Remain);
                    return RWDisc.dvRAM.CacheHitNodeNum;
                }

                Length -= Cnt;

                /* fill Segment Info */
                RWDisc.dvRAM.SegmentInfo[SegNo].BlkStartPSN = RAMcmGetSegmentPSN(PacketLSN);
                RWDisc.dvRAM.SegmentInfo[SegNo].EccStartLSN = PacketLSN;
                RWDisc.dvRAM.SegmentInfo[SegNo].BlockFilledInfo = RAMcmConvertBlockMap((BYTE)(StartLSN-PacketLSN), Cnt);

                break;
            }

            SegNo = pHash->SegNo;

            PacketPSN = RAMcmGetSegmentPSN(PacketLSN);
            if (RWDisc.dvRAM.SegmentInfo[SegNo].BlkStartPSN == PacketPSN)
            {

                if (Access == eWriteAccess)
                {
                    /* WriteAccess process: */

                    if ((RWDisc.dvRAM.SegmentInfo[SegNo].SegmentType == eReadSeg))
                    {
                        BYTE newseg;

                        newseg = RAMcmGetNextAvailableSegment(eWriteAccess);

                        send_msg5SValue(1,"newseg",1,newseg);

                        if (newseg == RAM_NULL_SEG)     /* no free segment */
                        {
                            RAMSetNextInfo(StartLSN, Length + Remain);
                            return RWDisc.dvRAM.CacheHitNodeNum;
                        }

                        if (newseg != SegNo)
                        {
                            /* add  a new entry for writing */
                            pNewHash = RAMcmGetNextHashListNode(newseg);
                            pNewHash->SegNo = newseg;

                            pNewHash->NextNode = pHash->NextNode;
                            pNewHash->PrevNode = pHash->PrevNode;

                            (pHash->PrevNode)->NextNode = pNewHash;
                            if (pHash->NextNode != NULL)
                                (pHash->NextNode)->PrevNode = pNewHash;

                            pHash->NextNode = NULL;
                            pHash->PrevNode = NULL;

                            /* merge the existing read/WriteLocked segment
                               into the new write segment, then invalidate
                               the old segment */
                            RAMcmCombineSegment(SegNo, newseg);

                            SegNo = newseg;
                        }
                    }

                    Cnt = (USHORT) (EndLSN - StartLSN);
                    if (RAMcmAddCacheHitNode(StartLSN, Cnt, eMissed, SegNo) == FALSE)
                    {
                        send_msg5SValue(1,"Add Failing",1,RWDisc.dvRAM.CacheHitNodeNum);
                        RAMSetNextInfo(EndLSN, Length+ Remain);

                        return RWDisc.dvRAM.CacheHitNodeNum;
                    }

                    Newbitmap |= RAMcmConvertBlockMap((BYTE)(StartLSN-PacketLSN), Cnt);
                    RWDisc.dvRAM.SegmentInfo[SegNo].BlockFilledInfo |= Newbitmap;

                    Length -= Cnt;

                    continue;
                }   /* end of WriteAccess process */


                /* ReadAccess process */

                BitMap = RWDisc.dvRAM.SegmentInfo[SegNo].BlockFilledInfo;

                Offset = 0;
                Newbitmap = 0;

                ret = TRUE;
                /*---------------------------------------------------------*/
                /* process all the blocks within the packet */
                while ((Blkcnt = RAMcmGetFilledInfo(&BitMap, &Offset)) != 0)
                {

                    LSN0 = PacketLSN + Offset;
                    LSN1 = LSN0 + Blkcnt;
                    Offset += Blkcnt;       /* adjust start offset */

                    if (StartLSN < LSN0)
                    {
                        if (EndLSN <= LSN0)
                        {
                            /* fully missed */

                            Cnt = (USHORT) (EndLSN - StartLSN);
                            if (RWDisc.dvRAM.SegmentInfo[SegNo].SegmentType != eWriteSeg)
                                Newbitmap |= RAMcmConvertBlockMap((BYTE)(StartLSN-PacketLSN), Cnt);

                            ret = RAMcmAddCacheHitNode(StartLSN, Cnt, eMissed, SegNo);
                            Length -= Cnt;
                            StartLSN = EndLSN;
                        }
                        else if (EndLSN <= LSN1)
                        {
                            /* partially hit */
                            Cnt = (USHORT) (LSN0 - StartLSN);
                            ret = RAMcmAddCacheHitNode(StartLSN, Cnt, eMissed, SegNo);

                            if (RWDisc.dvRAM.SegmentInfo[SegNo].SegmentType != eWriteSeg)
                                Newbitmap |= RAMcmConvertBlockMap((BYTE)(StartLSN-PacketLSN), Cnt);

                            if (ret == FALSE)
                            {
                                RAMSetNextInfo(LSN0, Length - Cnt + Remain);
                                return RWDisc.dvRAM.CacheHitNodeNum;
                            }

                            ret = RAMcmAddCacheHitNode(LSN0, (EndLSN - LSN0), eHit, SegNo);

                            Length -= (EndLSN - StartLSN);
                            StartLSN = EndLSN;
                        }
                        else    /* EndLSN > LSN1 */
                        {
                            /* partially hit */

                            Cnt = (USHORT) (LSN0 - StartLSN);
                            if (RWDisc.dvRAM.SegmentInfo[SegNo].SegmentType != eWriteSeg)
                                Newbitmap |= RAMcmConvertBlockMap((BYTE)(StartLSN-PacketLSN), Cnt);

                            ret = RAMcmAddCacheHitNode(StartLSN, Cnt, eMissed, SegNo);
                            if (ret == FALSE)
                            {
                                RAMSetNextInfo(LSN0, Length - Cnt + Remain);
                                return RWDisc.dvRAM.CacheHitNodeNum;
                            }

                            ret = RAMcmAddCacheHitNode(LSN0, LSN1-LSN0, eHit, SegNo);

                            Length -= (LSN1 - StartLSN);

                            StartLSN = LSN1;
                        }

                    }
                    else if (StartLSN <= LSN1)
                    {
                        if (EndLSN <= LSN1)
                        {
                            /* fully hit */
                            Cnt = (USHORT) (EndLSN - StartLSN);
                            ret = RAMcmAddCacheHitNode(StartLSN, Cnt, eHit, SegNo);
                            Length -= Cnt;
                            StartLSN = EndLSN;
                        }
                        else
                        {
                            /* partially hit */
                            Cnt = (USHORT) (LSN1 - StartLSN);
                            ret = RAMcmAddCacheHitNode(StartLSN, Cnt, eHit, SegNo);

                            Length -= Cnt;
                            StartLSN = LSN1;
                        }
                    }

                    if (Length == 0)
                        break;

                    if (ret == FALSE)
                    {
                        RAMSetNextInfo(StartLSN, Length + Remain);
                        return RWDisc.dvRAM.CacheHitNodeNum;
                    }

                }   /* end of while (blkcnt != 0) */

                if (Length != 0)
                {
                    Cnt = (USHORT) (EndLSN - StartLSN);

                    if (RWDisc.dvRAM.SegmentInfo[SegNo].SegmentType != eWriteSeg)
                        Newbitmap |= RAMcmConvertBlockMap((BYTE)(StartLSN-PacketLSN), Cnt);

                    ret = RAMcmAddCacheHitNode(StartLSN, Cnt, eMissed, SegNo);
                    Length = 0;
                }
                /*---------------------------------------------------------*/


            }
            else if (PacketPSN < RWDisc.dvRAM.SegmentInfo[SegNo].BlkStartPSN)
            {
                /* fully missed */
                BYTE newseg;

                /* allocate a new segment */
                newseg = RAMcmGetNextAvailableSegment(Access);
                if (newseg == RAM_NULL_SEG)     /* no free segment */
                {
                    RAMSetNextInfo(StartLSN, Length + Remain);
                    return RWDisc.dvRAM.CacheHitNodeNum;
                }

                if (newseg != SegNo)
                {
                    /* add  a new entry prior to the current node */
                    pNewHash = RAMcmGetNextHashListNode(newseg);
                    pNewHash->SegNo = newseg;

                    pNewHash->NextNode = pHash;
                    pNewHash->PrevNode = pHash->PrevNode;

                    (pHash->PrevNode)->NextNode = pNewHash;
                    pHash->PrevNode = pNewHash;
                }

                SegNo = newseg;

                Cnt = (USHORT) (EndLSN - StartLSN);
                /* add a node to CacheList */
                if ( RAMcmAddCacheHitNode(StartLSN, Cnt, eMissed, SegNo) == FALSE)
                {
                    RAMSetNextInfo(EndLSN, Length + Remain);
                    return RWDisc.dvRAM.CacheHitNodeNum;
                }

                /* fill the segment info */
                RWDisc.dvRAM.SegmentInfo[SegNo].BlkStartPSN = RAMcmGetSegmentPSN(PacketLSN);
                RWDisc.dvRAM.SegmentInfo[SegNo].EccStartLSN = PacketLSN;
                RWDisc.dvRAM.SegmentInfo[SegNo].BlockFilledInfo = RAMcmConvertBlockMap((BYTE)(StartLSN-PacketLSN), Cnt);

                Length -= Cnt;
                break;

            }
            else
            {
                /* check the succeeding nodes */
                pPrevHash = pHash;
                pHash = pPrevHash->NextNode;

            }

        }   /* end of while (Length != 0) */

        /*packet in loop***************/

        PacketLSN += 16L;

        StartLSN = PacketLSN;
        Offset = 0;
        RWDisc.dvRAM.SegmentInfo[SegNo].BlockFilledInfo |= Newbitmap;

    }   /* end of while (remain != 0) */

    RAMSetNextInfo(EndLSN, 0);
    return  RWDisc.dvRAM.CacheHitNodeNum;
}



/*------------------------------------------------------------------------
    Description:

    Input:  AccessType - for Read or Write

    Output: The new segment index
-------------------------------------------------------------------------*/
BYTE RAMcmGetNextAvailableSegment(BYTE AccessType)
{
    BYTE SegNo;
    BYTE SegType;

    if (AccessType == eReadAccess)
    {

        SegNo = RWDisc.dvRAM.NextReadSegNo;
        do
        {
            SegType = RWDisc.dvRAM.SegmentInfo[SegNo].SegmentType;

            if ((SegType == eEmptySeg) || (SegType == eReadSeg))

            {
                RWDisc.dvRAM.NextReadSegNo = SegNo;
                RWDisc.dvRAM.SegmentInfo[SegNo].SegmentType = eReadLocked;
                RamADD_SEGMENT(RWDisc.dvRAM.NextReadSegNo, 1);
                return(SegNo);
            }
            if (RWDisc.dvRAM.SA_DACache & eSA_MASK)      /* SA segment */
            {
                RamSUB_SEGMENT(SegNo, 1);
            }
            else
            {
                RamADD_SEGMENT(SegNo, 1);
            }

        }while(SegNo != RWDisc.dvRAM.NextReadSegNo);
    }
    else
    {
        if (RWDisc.dvRAM.WriteSegmentCount >= eRAMCacheSegmentCnt)
            return RAM_NULL_SEG;

        SegNo = RWDisc.dvRAM.NextWriteSegNo;
        do
        {
            SegType = RWDisc.dvRAM.SegmentInfo[SegNo].SegmentType;

            if ((SegType == eReadSeg) || (SegType == eEmptySeg))
            {
                RWDisc.dvRAM.WriteSegmentCount++;

                RWDisc.dvRAM.NextWriteSegNo = SegNo;
                RWDisc.dvRAM.SegmentInfo[SegNo].SegmentType = eWriteLocked;

                RamADD_SEGMENT(RWDisc.dvRAM.NextWriteSegNo, 1);
                return(SegNo);
            }

            if (RWDisc.dvRAM.SA_DACache & eSA_MASK)      /* SA segment */
            {
                RamSUB_SEGMENT(SegNo, 1);
            }
            else
            {
                RamADD_SEGMENT(SegNo, 1);
            }

        }while(SegNo != RWDisc.dvRAM.NextWriteSegNo);
    }
    return(RAM_NULL_SEG);
}


/*------------------------------------------------------------------------
    Description:
        checks if the write list reaches to the specific threshold.

    Input:  num - the threshold for write segment counter

    Output: TRUE - less, FALSE - greater than threshold
-------------------------------------------------------------------------*/
BOOL RAMcmCheckWriteFlush(BYTE num)
{
    /* check if need to flush write buffers */
    if (RWDisc.dvRAM.WriteSegmentCount > num )
        return TRUE;
    else
        return FALSE;

}
/*------------------------------------------------------------------------
    Description:
        will free the specified segment

    Input:  SegNo - the index of segment to be released;

    Output: None
-------------------------------------------------------------------------*/
void RAMcmFreeSegment(BYTE SegNo)
{
    RWDisc.dvRAM.SegmentInfo[SegNo].BlockFilledInfo = 0L;
    RWDisc.dvRAM.SegmentInfo[SegNo].SegmentType = eEmptySeg;
    RWDisc.dvRAM.SegmentInfo[SegNo].EccStartLSN = 0L;
    RWDisc.dvRAM.SegmentInfo[SegNo].BlkStartPSN = 0L;
    RWDisc.dvRAM.SegmentInfo[SegNo].NextEntry   = RAM_NULL_SEG;
    RWDisc.dvRAM.SegmentInfo[SegNo].ListNum     = RAM_NULL_LIST;

    /* free the corresponding HashList Node */
    RAMcmGetNextHashListNode(SegNo);
}

/*------------------------------------------------------------------------
    Description:
        will merge the data of the read segment into the target write segment

    Input:  ReadSegNo - index of read segment;
            WriteSegNo - index of write segment;

    Output: None
-------------------------------------------------------------------------*/
void RAMcmCombineSegment(BYTE ReadSegNo, BYTE WriteSegNo)
{

}


/*------------------------------------------------------------------------
    Description:
        Removes the HashListNode specified by its index from the hash table
        list and returns its pointer;

    Input:  Index of the HashList Node to be released;

    Output: Pointer to the released node;
-------------------------------------------------------------------------*/
HashListT *RAMcmGetNextHashListNode(BYTE SegNo)
{

    HashListT * ptr1, *ptr0, *ptr;

    ptr = (HashListT *)&RWDisc.dvRAM.HashListLib[SegNo];

    ptr1 = ptr->NextNode;
    ptr0 = ptr->PrevNode;

    if (ptr1 != NULL)
    {
        ptr1->PrevNode = ptr0;
    }

    if (ptr0 != NULL)
    {
        ptr0->NextNode = ptr1;
    }

    ptr->SegNo = RAM_NULL_SEG;
    ptr->NextNode = NULL;
    ptr->PrevNode = NULL;

    return (ptr);
}


/*------------------------------------------------------------------------
    Description:
        registers the new segment which has new read data

    Input:  SegNo - Segment # for Read

    Output: TRUE/FALSE
-------------------------------------------------------------------------*/
BOOL RAMcmRegisterSegment(BYTE SegNo)
{
    BYTE cTblIndex, CurSegNo;
    LONG PacketLSN;
    HashListT * pHash, *pPrevHash, *pNewHash;

    PacketLSN = RWDisc.dvRAM.SegmentInfo[SegNo].EccStartLSN;
    cTblIndex = RamHashing(PacketLSN);

    pPrevHash = &RWDisc.dvRAM.HashTbl[cTblIndex];
    pHash = pPrevHash->NextNode;

    //Register Segment is used only in Pre-buffering.
    //We force pHash must be equal to NULL.
    if (pHash == NULL)
    {
        if (SegNo != pPrevHash->SegNo)
        {
            /* add  a new entry into hash table */
            pNewHash = RAMcmGetNextHashListNode(SegNo);
            pNewHash->SegNo = SegNo;

            pNewHash->NextNode = NULL;
            pNewHash->PrevNode = pPrevHash;

            pPrevHash->NextNode = pNewHash;
            return TRUE;
        }
        else
        {
            send_msg5S(1,"Seg Fail-1");
            send_msg5SValue(SEND_MRW_MSG,"SegNo",1,SegNo);
            send_msg5SValue(SEND_MRW_MSG,"PreSegNo",1,pPrevHash->SegNo);
            return FALSE;
        }
    }
    else
    {
        CurSegNo = pHash->SegNo;
        send_msg5S(1,"Seg Fail-2");
        send_msg5SValue(SEND_MRW_MSG,"CurSegNo",1,CurSegNo);
        send_msg5SValue(SEND_MRW_MSG,"CurLSN",4,RWDisc.dvRAM.SegmentInfo[CurSegNo].EccStartLSN);
        send_msg5SValue(SEND_MRW_MSG,"SegNo",1,SegNo);
        return FALSE;
    }
}


ULONG RAMcmGetSegmentPSN(ULONG BlockLSN)
{
	  ULONG BlockPSN, RepPSN;
	  ULONG *SDLp;

    BlockPSN = ConvertLSNToPSN(BlockLSN);
    if (RWDisc.dvRAM.SA_DACache == eDA)
   	   return BlockPSN;
    else
    {
   	    SDLp = SearchSdlEntry(BlockPSN);
   	    if ((SWAPEND_ULONG(*SDLp) & SLR_LONG) || (RWDisc.dvRAM.Stream & eReadStream)) //SLR Bit = 1, a skipped block
   	    {
   	        /* The block contain real-time (video) data, partial corrected is allowed. */
   	        /* See DVD-RAM spec Ver 2.1 , section 5.6.8 Read Procedure */
   	        return (BlockPSN | SLR_LONG);
   	    }
   	    else
   	    {
   	        SDLp++; //move to replacement PSN
   	        RepPSN  = SWAPEND_ULONG(*SDLp) & 0x00FFFFFF;
   	        return RepPSN;
   	    }
    }
}


/*------------------------------------------------------------------------
    Description:
        If a read command is requested after writting, or a write command is
    requested after reading, reset all the Cache Manager parameters.

    Input:  CmdType - Read or Write command
    Output: NONE
-------------------------------------------------------------------------*/
void RAMClearCache(BYTE CmdType)
{
    switch (CmdType)
    {
        case eWriteCmd:
            if(RWDisc.dvRAM.PrevCommandIsRead)
            {
                RAMcmInit();
                RWDisc.dvRAM.PrevCommandIsRead = CLEAR;
            }
            RWDisc.dvRAM.PrevCommandIsWrite = SET;
            break;

        case eReadCmd:
            if(RWDisc.dvRAM.PrevCommandIsWrite)
            {
                RAMcmInit();
                RWDisc.dvRAM.PrevCommandIsWrite = CLEAR;
            }
            RWDisc.dvRAM.PrevCommandIsRead = SET;
            break;

        default:
            break;
    }
}


#endif /* #if (DVD_RAM_READ == 1) */

