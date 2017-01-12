/****************************************************************************
*                (c) Copyright 2001,2002,2003, 2004, 2005 SUNEXT DESIGN, INC
*                           All Right Reserved
*
* FILENAME:   bdWritableCache.c
*
*  $Revision: 14 $
*  $Date: 10/12/21 4:16p $
*
* DESCRIPTION
*   This file contains cache buffer utility for BD-R/RE
*
* Notes:
*
**************************** SUNEXT CONFIDENTIAL ****************************/

#include ".\common\globals.h"
#include ".\common\globtype.h"
#include ".\common\sdram.h"
#include ".\hif\gp_mac.h"
#include ".\hif\rd_util.h"
#include ".\hif\sv_util.h"
#include ".\dvd\dvMacros.h"
#include ".\dvd\dvisr.h"
#include ".\dvd\dvGvars.h"


#if (BD_ENABLE == 1)
#include ".\bd\bdVars.h"
#include ".\defmgmt\bdwritable\BdWritableCache.h"
#include ".\defmgmt\bdwritable\BdDfmg.h"
#include ".\bd\bdUtil.h"

/*************************************************************************/
/*                         Declarations                                  */
/*************************************************************************/
void BDcmResetCacheHitList();
void BDcmCombineSegment(BYTE ReadSegNo, BYTE WriteSegNo);
void BDcmCopyPage(USHORT SrcPage, USHORT TgtPage, USHORT PageCount);
BYTE BDcmGetNextAvailableSegment(BYTE AccessType);
HashListT *BDcmGetNextHashListNode(BYTE SegNo);
ULONG BDcmGetSegmentPSN(ULONG BlockLSN);
void BDInitWriteList(void);
void BDInitRingBuffer(void);
void BDInitCache(void);
USHORT BDcmFindTargetPSN(LONG StartLba, USHORT Count, BYTE Access);


/*************************************************************************/
/*                         Functions                                     */
/*************************************************************************/



/*------------------------------------------------------------------------
    Description:
        Initializes the Cache Management;

    Input:  None

    Output: None
-------------------------------------------------------------------------*/
void BDcmInit(void)
{
    /* init the Ring Buffer */
    BDInitRingBuffer();
}


void BDInitCache(void)
{
    BYTE    i;

    /*--------------------------------------------------
        Clear the buffer count and start of cache
    ---------------------------------------------------*/
    DataBuffer.SectorCount      = 0;
    DataBuffer.CacheStartLBA    = 0L;
    DataBuffer.CacheEndLBA      = 0L;

    /* init the dram segment */
    for(i=0; i<eBDCacheSegmentCnt; i++)
    {
        BdwtDisc.SegmentInfo[i].EccStartLSN = 0;
        BdwtDisc.SegmentInfo[i].BlkStartPSN = 0;
        BdwtDisc.SegmentInfo[i].BlockFilledInfo = 0L;
        BdwtDisc.SegmentInfo[i].SegmentDramPageNo = i * ONE_BD_CLUSTER + eBDCacheStartPage;
        BdwtDisc.SegmentInfo[i].SegmentType = eEmptySeg;
        BdwtDisc.SegmentInfo[i].NextEntry = BD_NULL_SEG;  //To be used in write list
        BdwtDisc.SegmentInfo[i].ListNum = BD_NULL_LIST;   //To be used in write list
    }

    BdwtDisc.NextWriteSegNo = 0;
    BdwtDisc.NextReadSegNo = 0;
    BdwtDisc.WriteSegmentCount = 0;
#if BDWT_FAST_CACHE
    BdwtDisc.CacheFull = 0;
#endif

    /* init the Hash Table */
    for(i=0; i<eBDHashTableSize; i++)
    {
        BdwtDisc.HashTbl[i].SegNo = BD_NULL_SEG;
        BdwtDisc.HashTbl[i].NextNode = NULL;
        BdwtDisc.HashTbl[i].PrevNode = NULL;

    }

    /* init the Hash List Lib Node */
    for (i = 0; i < eBDTotalHashListNode; i++)
    {
        BdwtDisc.HashListLib[i].SegNo = BD_NULL_SEG;
        BdwtDisc.HashListLib[i].NextNode = NULL;
        BdwtDisc.HashListLib[i].PrevNode = NULL;
    }

    /* init the CacheHitList */
    for (i=0; i<eBDTotalCacheHitNode; i++)
    {
        BdwtDisc.CacheListLib[i].StartPSN = 0;
        BdwtDisc.CacheListLib[i].StartDramPgNo = 0;
        BdwtDisc.CacheListLib[i].Length = 0;
        BdwtDisc.CacheListLib[i].HitStatus = eMissed;
    }

    BDcmResetCacheHitList();

}
void BDInitRingBuffer(void)
{
    send_msg80(SEND_MRW_MSG,0xFD0301);

    BDStopPrebuffering();
    BDInitCache();
}

/*------------------------------------------------------------------------
    Description:
        Resets the Cache Hit List;
    Input:  None

    Output: None
-------------------------------------------------------------------------*/
void BDcmResetCacheHitList()
{
    BdwtDisc.CacheHitNodeNum = 0;
}


/*------------------------------------------------------------------------
    Description:
        Adds a new Cache hit node into the list;

    Input:  StartLSN, EndPSN - the LSN range
            HitFlag - indicates the status of segment in DBD: eHit/eMissed
            SegNo   - Segment index

    Output: TRUE  - Add success;
            FALSE - The CacheHitList will be full;
-------------------------------------------------------------------------*/
BOOL BDcmAddCacheHitNode(LONG StartPSN, USHORT Cnt, BYTE HitFlag, BYTE SegNo)
{
    BdCacheListT    *pCacheHitNode;
    USHORT Page, temp_DramPgNo;

    Page = (USHORT)(StartPSN & LAST_PSN_OF_BD_CLUSTER);
    Page += BdwtDisc.SegmentInfo[SegNo].SegmentDramPageNo;

    if (BdwtDisc.CacheAccess == eReadAccess)
    {
        if (BdwtDisc.SegmentInfo[SegNo].SegmentType == eReadSeg)
            BdwtDisc.SegmentInfo[SegNo].SegmentType = eReadLocked;

        if (BdwtDisc.CacheHitNodeNum)  /* BdwtDisc.CacheHitNodeNum > 0 */
        {
            pCacheHitNode = BDcmGetCacheHitNode(BdwtDisc.CacheHitNodeNum -1);

            /* Merge? */
            if (((pCacheHitNode->StartPSN + pCacheHitNode->Length) == StartPSN) &&
                (pCacheHitNode->HitStatus == HitFlag))
            {
                temp_DramPgNo = pCacheHitNode->StartDramPgNo + pCacheHitNode->Length;
                if(temp_DramPgNo >= eBDTotalPageNo) temp_DramPgNo -= eBDTotalPageNo;

                if(temp_DramPgNo == Page)
                {
                    /* Merge New Entry with Last Entry */
                    pCacheHitNode->Length += Cnt;
                    return TRUE;
                }
            }
        }
    }
    else    /* eWriteAccess */
    {
#if 0   // TBD
        if (BdwtDisc.SegmentInfo[SegNo].SegmentType == eWritingSeg)
        {
            /* the new write segment is in use */
            if (BdwtDisc.CacheHitNodeNum > 0)
            {
                BYTE CSegNo, i;
                for (i = 0; i < BdwtDisc.CacheHitNodeNum + 1; i++)
                {
                    CSegNo = BdwtDisc.CacheListLib[i].SegmentNo;
                    if (BdwtDisc.SegmentInfo[CSegNo].SegmentType == eWriteLocked)
                    {
                        BdwtDisc.SegmentInfo[CSegNo].SegmentType = eEmptySeg;
                        BdwtDisc.SegmentInfo[CSegNo].EccStartLSN = 0;
                        if (BdwtDisc.WriteSegmentCount != 0)
                            BdwtDisc.WriteSegmentCount --;
                    }
                }

            }
            BdwtDisc.CacheHitNodeNum = 0;
            return FALSE;
        }
#endif  /* #if 0 */
    }

    BdwtDisc.CacheListLib[BdwtDisc.CacheHitNodeNum].StartPSN = StartPSN;
    BdwtDisc.CacheListLib[BdwtDisc.CacheHitNodeNum].StartDramPgNo = Page;
    BdwtDisc.CacheListLib[BdwtDisc.CacheHitNodeNum].Length = Cnt;
    BdwtDisc.CacheListLib[BdwtDisc.CacheHitNodeNum].HitStatus = HitFlag;
    BdwtDisc.CacheListLib[BdwtDisc.CacheHitNodeNum].SegmentNo = SegNo;

    BdwtDisc.CacheHitNodeNum++;
    if (BdwtDisc.CacheHitNodeNum == eBDTotalCacheHitNode)
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
BdCacheListT *BDcmGetCacheHitNode(USHORT index)
{
    if (index < BdwtDisc.CacheHitNodeNum)
        return ((BdCacheListT *)&BdwtDisc.CacheListLib[index]);
    else
        return NULL;
}


/*----------------------------------------------------------------------------
  Name: BDcmConvertBlockMap()
  Description:
        converts a range into a 32-bit bitmap.

  Input:  Offset - start bit;
          Length - the number of contiguouse bits;

  Output: a 16-bit BitMapping flag

          Offset: 0   1   2   ...     31
          Bit:    0   1   2   ...     31

  Note:   0 < Length <= 32
          0 <= Offset < 32
---------------------------------------------------------------------------*/
ULONG BDcmConvertBlockMap(BYTE Offset, USHORT Length)
{
    ULONG map, bitptn;
    USHORT i;

    bitptn = map = 0x1L << Offset;
    for (i = 0; i < Length; i++, bitptn <<= 1)
        map |= bitptn;

    return map;
}


/*----------------------------------------------------------------------------
  Name: BDcmGetFilledInfo()
  Description:
        converts a bitmap into range;

  Input:  pBitMap - address of bit map;
          pOffset - address of offset

  Output: the number of consecutive bit '1' in the bit map;
----------------------------------------------------------------------------*/
USHORT BDcmGetFilledInfo(ULONG *pBitMap, BYTE *pOffset)
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

    Input:  LSN - The Start PSN of un-processed blocks
            Length - The number of blocks

    Output: None
-------------------------------------------------------------------------*/
void BDSetNextInfo(LONG PSN, USHORT Length)
{
    BdwtDisc.CacheResult.NextStartPSN = PSN;
    BdwtDisc.CacheResult.RemainLength = Length;
}

/*------------------------------------------------------------------------
    Description:
        Searches the cache buffer and generates a CacheHitList to return
        the information of target blocks

    Input:  StartPSN - Start PSN address
            Count - The number of blocks
            Access - Read or write access;

    Output: The number of cache hit list node;
-------------------------------------------------------------------------*/
USHORT BDcmFindTargetPSN(LONG StartLba, USHORT Count, BYTE Access)
{
    LONG PacketPSN, EndPSN;
    LONG PSN0, PSN1;
    ULONG StartPSN, EndLba, backupStartPSN, BitMap, Newbitmap = 0;
    PSNs Target;
    HashListT * pHash, *pNewHash, *pPrevHash;
    ReplacementInfoType     ReplacementInfo;
    USHORT Cnt, Blkcnt, Remain, Length;
    BYTE SegNo, Offset, Attr, cTblIndex, defeat_f, layer_jump;
    BOOL ret;

    BDcmResetCacheHitList();

    BdwtDisc.CacheAccess = Access & 0x7F;
    defeat_f = layer_jump = 0;

    EndLba = StartLba + Count - 1;
    if((StartLba <= Layer0EndLBA) && (EndLba > Layer0EndLBA))
        layer_jump = 1;
    Target = bdConvertLBAtoPSN(StartLba);
    StartPSN = Target.PSN;
    PacketPSN = StartPSN & BD_CLUSTER_MASK;  /*(StartPSN / 32L) * 32L*/
    Offset = (BYTE) (StartPSN & LAST_PSN_OF_BD_CLUSTER);      /*(StartPSN - PacketPSN)*/

    Remain = Count;

    while (Remain != 0)
    {
        Attr = eNotReplacedCluster;

        if((Access != eBD_STREAM_READ) && ((StartLba <= MaxDflEndLBA) && (EndLba >= MinDflStartLBA)))
        {
            if (SearchReplacementInfo(PacketPSN, &ReplacementInfo))  // This block is found in Defect List
            {
                if (Access == eBD_READ)
                {
                    switch(ReplacementInfo.Status1)
                    {
                        case DFL_STAT1_RAD:
                            backupStartPSN = PacketPSN;
                            PacketPSN = ReplacementInfo.ReplacementPba;
                            StartPSN = PacketPSN + Offset;
                            Attr = eReplacedCluster; //replaced block
                            defeat_f = 0x80;
                            break;

                        case DFL_STAT1_RAD_NOT_RECORDED:
                        case DFL_STAT1_NRD:
                        case DFL_STAT1_PBA:
                            Attr = eNotReplacedCluster;
                            break;

                        default:
                            send_msg5S(1,"Invalid Info");
                            break;
                    }
                }
                else if (Access == eBD_STREAM_READ)
                {
                    Attr = eNotReplacedCluster;
                }
                else if (Access == eBD_WRITE)
                {
                    switch(ReplacementInfo.Status1)
                    {
                        case DFL_STAT1_RAD:
                            backupStartPSN = PacketPSN;
                            PacketPSN = ReplacementInfo.ReplacementPba;
                            StartPSN = PacketPSN + Offset;
                            Attr = eReplacedCluster; //replaced block
                            break;

                        case DFL_STAT1_RAD_NOT_RECORDED:
                        case DFL_STAT1_NRD:
                        case DFL_STAT1_PBA:
                            Attr = eNotReplacedCluster;
                            break;

                        default:
                            send_msg5S(1,"Invalid Info");
                            break;
                    }
                }
                else if (Access == eBD_STREAM_WRITE)
                {
                }
            }
        }

        cTblIndex = BdHashing(PacketPSN);

        //send_msg5SValue(SEND_MRW_MSG,"PacketPSN",4,PacketPSN);
        //send_msg5SValue(SEND_MRW_MSG,"cTblIndex",1,cTblIndex);
        //SendMsg5SValue(SEND_MRW_MSG,"Attr", 1,Attr);

        EndPSN = StartPSN + Remain;
        PSN0 = PacketPSN + ONE_BD_CLUSTER;

        //send_msg5SValue(SEND_MRW_MSG,"Remain",4,Remain);
        //send_msg5SValue(SEND_MRW_MSG,"StartPSN",4,StartPSN);
        //send_msg5SValue(SEND_MRW_MSG,"EndPSN",4,EndPSN);


        if (EndPSN > PSN0)
        {
            Length = ONE_BD_CLUSTER - Offset;
            Remain = Remain - Length;
            EndPSN = PSN0;
        }
        else
        {
            Length = Remain;
            Remain = 0;
        }


        pPrevHash = (HashListT *) &BdwtDisc.HashTbl[cTblIndex];
        pHash = pPrevHash->NextNode;

        Newbitmap = 0L;

        /*packet in loop***************/
        while (Length != 0)
        {
            if (pHash == NULL)
            {
                /* fully missed */

                /* allocate a new segment */
                SegNo = BDcmGetNextAvailableSegment(BdwtDisc.CacheAccess);

                if (SegNo == BD_NULL_SEG)      /* no free segment */
                {
                    BDSetNextInfo(StartPSN, Length + Remain);
                    return BdwtDisc.CacheHitNodeNum;
                }

                if (SegNo != pPrevHash->SegNo)
                {
                    /* add  a new entry into hash table */
                    pNewHash = BDcmGetNextHashListNode(SegNo);
                    pNewHash->SegNo = SegNo;

                    pNewHash->NextNode = NULL;
                    pNewHash->PrevNode = pPrevHash;

                    pPrevHash->NextNode = pNewHash;
                }

                /* add a node to CacheList */
                Cnt = (USHORT) (EndPSN - StartPSN);
                if (BDcmAddCacheHitNode(StartPSN, Cnt, eMissed, SegNo) == FALSE)
                {
                    BDSetNextInfo(EndPSN, Length+ Remain);
                    return BdwtDisc.CacheHitNodeNum;
                }

                Length -= Cnt;

                /* fill Segment Info */
                BdwtDisc.SegmentInfo[SegNo].BlkStartPSN = PacketPSN;
                BdwtDisc.SegmentInfo[SegNo].BlockFilledInfo = BDcmConvertBlockMap((BYTE)(StartPSN-PacketPSN), Cnt);
                BdwtDisc.SegmentInfo[SegNo].Attr = Attr;

                break;
            }

            SegNo = pHash->SegNo;

            if (BdwtDisc.SegmentInfo[SegNo].BlkStartPSN == PacketPSN)
            {

                if (BdwtDisc.CacheAccess == eWriteAccess)
                {
                    /* WriteAccess process: */
                    Cnt = (USHORT) (EndPSN - StartPSN);
                    if (BDcmAddCacheHitNode(StartPSN, Cnt, eMissed, SegNo) == FALSE)
                    {
                        BDSetNextInfo(EndPSN, Length+ Remain);

                        return BdwtDisc.CacheHitNodeNum;
                    }

                    Newbitmap |= BDcmConvertBlockMap((BYTE)(StartPSN-PacketPSN), Cnt);
                    BdwtDisc.SegmentInfo[SegNo].BlockFilledInfo |= Newbitmap;

                    BdwtDisc.SegmentInfo[SegNo].Attr = Attr;

                    Length -= Cnt;

                    continue;
                }   /* end of WriteAccess process */


                /* ReadAccess process */

                BitMap = BdwtDisc.SegmentInfo[SegNo].BlockFilledInfo;

                Offset = 0;
                Newbitmap = 0;

                ret = TRUE;
                /*---------------------------------------------------------*/
                /* process all the blocks within the packet */
                while ((Blkcnt = BDcmGetFilledInfo(&BitMap, &Offset)) != 0)
                {

                    PSN0 = PacketPSN + Offset;
                    PSN1 = PSN0 + Blkcnt;
                    Offset += Blkcnt;       /* adjust start offset */

                    if (StartPSN < PSN0)
                    {
                        if (EndPSN <= PSN0)
                        {
                            /* fully missed */

                            Cnt = (USHORT) (EndPSN - StartPSN);
                            if (BdwtDisc.SegmentInfo[SegNo].SegmentType != eWriteSeg)
                                Newbitmap |= BDcmConvertBlockMap((BYTE)(StartPSN-PacketPSN), Cnt);

                            ret = BDcmAddCacheHitNode(StartPSN, Cnt, eMissed, SegNo);
                            Length -= Cnt;
                            StartPSN = EndPSN;
                        }
                        else if (EndPSN <= PSN1)
                        {
                            /* partially hit */
                            Cnt = (USHORT) (PSN0 - StartPSN);
                            ret = BDcmAddCacheHitNode(StartPSN, Cnt, eMissed, SegNo);

                            if (BdwtDisc.SegmentInfo[SegNo].SegmentType != eWriteSeg)
                                Newbitmap |= BDcmConvertBlockMap((BYTE)(StartPSN-PacketPSN), Cnt);

                            if (ret == FALSE)
                            {
                                BDSetNextInfo(PSN0, Length - Cnt + Remain);
                                return BdwtDisc.CacheHitNodeNum;
                            }

                            ret = BDcmAddCacheHitNode(PSN0, (EndPSN - PSN0), eHit, SegNo);
                            BdwtDisc.SegmentInfo[SegNo].Attr = Attr;

                            Length -= (EndPSN - StartPSN);
                            StartPSN = EndPSN;
                        }
                        else    /* EndPSN > PSN1 */
                        {
                            /* partially hit */

                            Cnt = (USHORT) (PSN0 - StartPSN);
                            if (BdwtDisc.SegmentInfo[SegNo].SegmentType != eWriteSeg)
                                Newbitmap |= BDcmConvertBlockMap((BYTE)(StartPSN-PacketPSN), Cnt);

                            ret = BDcmAddCacheHitNode(StartPSN, Cnt, eMissed, SegNo);
                            if (ret == FALSE)
                            {
                                BDSetNextInfo(PSN0, Length - Cnt + Remain);
                                return BdwtDisc.CacheHitNodeNum;
                            }

                            ret = BDcmAddCacheHitNode(PSN0, PSN1-PSN0, eHit, SegNo);
                            BdwtDisc.SegmentInfo[SegNo].Attr = Attr;

                            Length -= (PSN1 - StartPSN);

                            StartPSN = PSN1;
                        }

                    }
                    else if (StartPSN <= PSN1)
                    {
                        if (EndPSN <= PSN1)
                        {
                            /* fully hit */
                            Cnt = (USHORT) (EndPSN - StartPSN);
                            ret = BDcmAddCacheHitNode(StartPSN, Cnt, eHit, SegNo);
                            BdwtDisc.SegmentInfo[SegNo].Attr = Attr;
                            Length -= Cnt;
                            StartPSN = EndPSN;
                        }
                        else
                        {
                            /* partially hit */
                            Cnt = (USHORT) (PSN1 - StartPSN);
                            ret = BDcmAddCacheHitNode(StartPSN, Cnt, eHit, SegNo);
                            BdwtDisc.SegmentInfo[SegNo].Attr = Attr;

                            Length -= Cnt;
                            StartPSN = PSN1;
                        }
                    }

                    if (Length == 0)
                        break;

                    if (ret == FALSE)
                    {
                        BDSetNextInfo(StartPSN, Length + Remain);
                        return BdwtDisc.CacheHitNodeNum;
                    }

                }   /* end of while (blkcnt != 0) */

                if (Length != 0)
                {
                    Cnt = (USHORT) (EndPSN - StartPSN);

                    if (BdwtDisc.SegmentInfo[SegNo].SegmentType != eWriteSeg)
                        Newbitmap |= BDcmConvertBlockMap((BYTE)(StartPSN-PacketPSN), Cnt);

                    ret = BDcmAddCacheHitNode(StartPSN, Cnt, eMissed, SegNo);
                    BdwtDisc.SegmentInfo[SegNo].Attr = Attr;
                    Length = 0;
                }
                /*---------------------------------------------------------*/


            }
            else if (PacketPSN < BdwtDisc.SegmentInfo[SegNo].BlkStartPSN)
            {
                /* fully missed */
                BYTE newseg;

                /* allocate a new segment */
                newseg = BDcmGetNextAvailableSegment(BdwtDisc.CacheAccess);
                if (newseg == BD_NULL_SEG)     /* no free segment */
                {
                    BDSetNextInfo(StartPSN, Length + Remain);
                    return BdwtDisc.CacheHitNodeNum;
                }

                if (newseg != SegNo)
                {
                    /* add  a new entry prior to the current node */
                    pNewHash = BDcmGetNextHashListNode(newseg);
                    pNewHash->SegNo = newseg;

                    pNewHash->NextNode = pHash;
                    pNewHash->PrevNode = pHash->PrevNode;

                    (pHash->PrevNode)->NextNode = pNewHash;
                    pHash->PrevNode = pNewHash;
                }

                SegNo = newseg;

                Cnt = (USHORT) (EndPSN - StartPSN);
                /* add a node to CacheList */
                if ( BDcmAddCacheHitNode(StartPSN, Cnt, eMissed, SegNo) == FALSE)
                {
                    BDSetNextInfo(EndPSN, Length + Remain);
                    return BdwtDisc.CacheHitNodeNum;
                }

                /* fill the segment info */
                BdwtDisc.SegmentInfo[SegNo].BlkStartPSN = PacketPSN;
                BdwtDisc.SegmentInfo[SegNo].BlockFilledInfo = BDcmConvertBlockMap((BYTE)(StartPSN-PacketPSN), Cnt);
                BdwtDisc.SegmentInfo[SegNo].Attr = Attr;

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
        if(Attr == eReplacedCluster)
            PacketPSN = backupStartPSN;

        PacketPSN += ONE_BD_CLUSTER;

        if((layer_jump == 1) && (PacketPSN > LSN0EndPSN))
        {
            layer_jump = 0;
            PacketPSN = LSN1StartPSN;
        }
        StartPSN = PacketPSN;
        Offset = 0;
        BdwtDisc.SegmentInfo[SegNo].BlockFilledInfo |= Newbitmap;

    }   /* end of while (remain != 0) */

    BDSetNextInfo(EndPSN, 0);
    return  (BdwtDisc.CacheHitNodeNum | defeat_f);
}



/*------------------------------------------------------------------------
    Description:

    Input:  AccessType - for Read or Write

    Output: The new segment index
-------------------------------------------------------------------------*/
BYTE BDcmGetNextAvailableSegment(BYTE AccessType)
{
    BYTE SegNo;
    BYTE SegType;

    if (AccessType == eReadAccess)
    {

        SegNo = BdwtDisc.NextReadSegNo;
        do
        {
            SegType = BdwtDisc.SegmentInfo[SegNo].SegmentType;

            if ((SegType == eEmptySeg) || (SegType == eReadSeg))

            {
                BdwtDisc.NextReadSegNo = SegNo;
                BdwtDisc.SegmentInfo[SegNo].SegmentType = eReadLocked;
                BD_ADD_SEGMENT(BdwtDisc.NextReadSegNo, 1);
                return(SegNo);
            }

            BD_ADD_SEGMENT(SegNo, 1);

        }while(SegNo != BdwtDisc.NextReadSegNo);
    }
    else
    {
        if (BdwtDisc.WriteSegmentCount >= eBDCacheSegmentCnt)
            return BD_NULL_SEG;

        SegNo = BdwtDisc.NextWriteSegNo;
        do
        {
            SegType = BdwtDisc.SegmentInfo[SegNo].SegmentType;

            if ((SegType == eReadSeg) || (SegType == eEmptySeg))
            {
                BdwtDisc.WriteSegmentCount++;

                BdwtDisc.NextWriteSegNo = SegNo;
                BdwtDisc.SegmentInfo[SegNo].SegmentType = eWriteLocked;

                BD_ADD_SEGMENT(BdwtDisc.NextWriteSegNo, 1);
                return(SegNo);
            }

            BD_ADD_SEGMENT(SegNo, 1);

        }while(SegNo != BdwtDisc.NextWriteSegNo);
    }
    return(BD_NULL_SEG);
}


/*------------------------------------------------------------------------
    Description:
        checks if the write list reaches to the specific threshold.

    Input:  num - the threshold for write segment counter

    Output: TRUE - less, FALSE - greater than threshold
-------------------------------------------------------------------------*/
BOOL BDcmCheckWriteFlush(BYTE num)
{
    /* check if need to flush write buffers */
    if (BdwtDisc.WriteSegmentCount > num )
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
void BDcmFreeSegment(BYTE SegNo)
{
    BdwtDisc.SegmentInfo[SegNo].BlockFilledInfo = 0L;
    BdwtDisc.SegmentInfo[SegNo].SegmentType = eEmptySeg;
    BdwtDisc.SegmentInfo[SegNo].EccStartLSN = 0L;
    BdwtDisc.SegmentInfo[SegNo].BlkStartPSN = 0L;
    BdwtDisc.SegmentInfo[SegNo].NextEntry   = BD_NULL_SEG;
    BdwtDisc.SegmentInfo[SegNo].ListNum     = BD_NULL_LIST;

    /* free the corresponding HashList Node */
    BDcmGetNextHashListNode(SegNo);
}

/*------------------------------------------------------------------------
    Description:
        will merge the data of the read segment into the target write segment

    Input:  ReadSegNo - index of read segment;
            WriteSegNo - index of write segment;

    Output: None
-------------------------------------------------------------------------*/
void BDcmCombineSegment(BYTE ReadSegNo, BYTE WriteSegNo)
{

}


/*------------------------------------------------------------------------
    Description:
        Removes the HashListNode specified by its index from the hash table
        list and returns its pointer;

    Input:  Index of the HashList Node to be released;

    Output: Pointer to the released node;
-------------------------------------------------------------------------*/
HashListT *BDcmGetNextHashListNode(BYTE SegNo)
{

    HashListT * ptr1, *ptr0, *ptr;

    ptr = (HashListT *)&BdwtDisc.HashListLib[SegNo];

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

    ptr->SegNo = BD_NULL_SEG;
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
BOOL BDcmRegisterSegment(BYTE SegNo)
{
    BYTE cTblIndex;
    LONG PacketPSN,PacketLSN;
    HashListT * pHash, *pPrevHash, *pNewHash;

    PacketPSN = BdwtDisc.SegmentInfo[SegNo].BlkStartPSN;
    cTblIndex = BdHashing(PacketPSN);


    pPrevHash = (HashListT *)&BdwtDisc.HashTbl[cTblIndex];
    pHash = pPrevHash->NextNode;
    
    if (pHash == NULL)
    {
        if (SegNo != pPrevHash->SegNo)
        {
            /* add  a new entry into hash table */
            pNewHash = BDcmGetNextHashListNode(SegNo);
            pNewHash->SegNo = SegNo;

            pNewHash->NextNode = NULL;
            pNewHash->PrevNode = pPrevHash;

            pPrevHash->NextNode = pNewHash;
        }
        return TRUE;
    }
    else
    {
        /*if pHash isn't NULL, it maybe link by last defect replacement cluster,
          check this SegNo->BlkStartLSN in cache or not, if this SegNO isn't in cache, free this SegNO and pHash link.*/
            PacketLSN = BdwtDisc.SegmentInfo[pHash->SegNo].BlkStartPSN;
            SendMsgCnB(SEND_HIF_CMD,6,0x80042C,B1B(SegNo), B1B(pHash->SegNo),B4B(PacketLSN));
            //if Current Hash segno LSN isn't in cache, free this segno.
            if((PacketLSN< DataBuffer.CacheStartLBA)||((PacketLSN+ONE_BD_CLUSTER)>DataBuffer.CacheEndLBA))
            {
                BDcmFreeSegment(pHash->SegNo);//free cur pHash.
            }
        return FALSE;
    }
}


ULONG BDcmGetSegmentPSN(ULONG BlockLSN)
{
    //TBD
    return 0;
}


/*------------------------------------------------------------------------
    Description:
        If a read command is requested after writting, or a write command is
    requested after reading, reset all the Cache Manager parameters.

    Input:  CmdType - Read or Write command
    Output: NONE
-------------------------------------------------------------------------*/
void BDClearCache(BYTE CmdType)
{
    switch (CmdType)
    {
        case eWriteCmd:
            if(BdwtDisc.PrevCommandIsRead)
            {
                BDcmInit();
                BdwtDisc.PrevCommandIsRead = CLEAR;
            }
            BdwtDisc.PrevCommandIsWrite = SET;
            break;

        case eReadCmd:
            if(BdwtDisc.PrevCommandIsWrite)
            {
                BDcmInit();
                BdwtDisc.PrevCommandIsWrite = CLEAR;
            }
            BdwtDisc.PrevCommandIsRead = SET;
            break;

        default:
            break;
    }
}





#endif /* BD_ENABLE == 1 */

