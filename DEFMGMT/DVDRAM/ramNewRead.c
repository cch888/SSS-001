/****************************************************************************
*             (c) Copyright 2001 - 2010 Sunext Technology Co., Ltd.
*                           All Right Reserved
*
* FILENAME:   ramNewRead.c
*
*  $Revision: 4 $
*  $Date: 10/12/21 4:17p $
*
* DESCRIPTION
*   This file contains read utility for DVD-RAM
*
* Notes:
*
**************************** SUNEXT CONFIDENTIAL ****************************/

#include ".\common\globals.h"
#include ".\common\globtype.h"
#include ".\common\com_im.h"
#include ".\common\sdram.h"
#include ".\servo\svo_com.h"
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
#include ".\dvd\dvHostRd.h"
#include ".\dvd\dvBufMgr.h"

#include ".\AL\REG_intctrl.h"
#include ".\AL\REG_global.h"
#include ".\AL\REG_dvd_decoder.h"
    #if (ENABLE_RAM_DFE_ICE_DETECT == 1)
#include ".\al\reg_dfe.h"
#include ".\al\HAL_dfe.h"
    #endif

/*************************************************************************/
/*                         Declarations                                  */
/*************************************************************************/
void read_common_DVDRAM(void);
void read_exit_DVDRAM(void);
BYTE DVDRAMPreReadBuffering(void);
BOOL IsCertifiedBlock(USHORT EccStartPage);
    #if DvdRamFastCache
BYTE DVDRAMTransferHostData(USHORT tx_page, USHORT tx_len, BOOL FastCache);
BYTE DVDRAMCheckPreBuffering(LONG StartLSN, USHORT Len, BOOL FastCache);
void DVDRAMReleaseReadLock(void);
    #else
BYTE DVDRAMTransferHostData(USHORT tx_page, USHORT tx_len);
BYTE DVDRAMCheckPreBuffering(LONG StartLSN, USHORT Len);
    #endif
DiscAccessStatusE DVDRAMReadDataFromDisc(USHORT start_page,ULONG blk_start_psn,ULONG blk_start_lsn,BYTE length, BYTE mode);
DiscAccessStatusE DVDRAMStartPrebuffering(USHORT start_page,ULONG blk_start_lsn,BYTE length, BYTE mode);
void DVDRAMAppendPreBuffer(void);
void RamReadSkipForPrebuffering(void);
void PrintIceIndicator(void);
DiscAccessStatusE DVDRAMReadBlockToTempBuffer(ULONG BlkStartLSN,ULONG BlkStartPSN);
BYTE ramVerifyID();
void InitializeIceIndicator(void);
BYTE  RamRetryCnt;

/*************************************************************************/
/*                         Functions                                     */
/*************************************************************************/

/*----------------------------------------------------------------------------
  Name: DVDRAMStopPrebuffering()
  Description:

  Input:  None

  Output: None
----------------------------------------------------------------------------*/
void DVDRAMStopPrebuffering()
{
    //check BG preBuffering
    if(RWDisc.dvRAM.BgPreBuffer)
    {
        RWDisc.dvRAM.BgPreBufAbort = 1;
        send_msg80(SEND_MRW_MSG,0x710221);

        while(RWDisc.dvRAM.BgPreBufAbort)
        {
            ExitProcess();
        }
    }

    if (RWDisc.dvRAM.Prebuffering == TRUE)
    {
        dvdStopDiscTransfer();
        DelaymS(10);
        RWDisc.dvRAM.Prebuffering = FALSE;
    }
}


/*----------------------------------------------------------------------------
  Name: Cmd_read_10_DVDRAM
  Description:
        Read_10/Read_12 Command Handler for DVD-RAM disc

  Input:  None

  Output: None
----------------------------------------------------------------------------*/
void Cmd_read_10_DVDRAM(void)
{
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
    if ( (Iram_Cmd_block.bf[0] == SBC_READ_10) || (Iram_Cmd_block.bf[0] == SBC_VERIFY_10) )
    {
        HostXfer.BlockCount.byte.m = Iram_Cmd_block.bf[7];
        HostXfer.BlockCount.byte.l = Iram_Cmd_block.bf[8];
    }
    else /* SBC_READ_12 */
    {
        HostXfer.BlockCount.byte.h = Iram_Cmd_block.bf[7];
        HostXfer.BlockCount.byte.m = Iram_Cmd_block.bf[8];
        HostXfer.BlockCount.byte.l = Iram_Cmd_block.bf[9];
    }

    /*-------------------------------------------------------------------
        Initial remaining blocks to transfer
    --------------------------------------------------------------------*/
    HostXfer.BlocksLeft = HostXfer.BlockCount.Long;


    if ( HostXfer.BlockCount.Long == 0 ) /* if it is zero */
    {
        read_exit_DVDRAM();
        return;
    }

    read_common_DVDRAM();

    #if (WORKAROUND_FOR_SEEDMISTHR == 1)
    if(READ_FIELD(HAL_DVDD_SEEDMISTHR) == 0)
    {
        WRITE_FIELD(HAL_DVDD_SEEDMISTHR, 2);
    }
    #endif
}

/*----------------------------------------------------------------------------
  Name: read_common_DVDRAM
  Description:
        Common read commands Handler

  Input:  None

  Output: None
----------------------------------------------------------------------------*/
void read_common_DVDRAM(void)
{
    RamCacheListT       *pCacheHitNode;
    ULONG   dvdEndLBA;
    ULONG   tx_lsn;
    USHORT  c_num, i, k, j;
    USHORT  tx_page, tx_len;
    BYTE    cnt;
    BYTE    Fua, mode, result;
    BYTE    tx_seg, tx_segtype;
    DiscAccessStatusE   DiscReadStatus;
    FLAG   SequentialRead = CLEAR;
    BYTE  *IceIndicator;
    BYTE   BlocksToBeBuffered, SegNo;

    /*-------------------------------------------------------------------
        Check if read is within a valid range
    --------------------------------------------------------------------*/
    // Calculate ending LBA of the host transfer
    dvdEndLBA = HostXfer.StartBlock.lba + HostXfer.BlockCount.Long - 1L;
    if (dvdEndLBA > GetDVDRAMMaxLBA())
    {
        BUILD_SENSE(0x05,0x21,0x00,0xb3);
        read_exit_DVDRAM();
        return;
    }

   if(SpeedMD == Set_sp_f)
    {
        SpeedMD=Special_f;
    }

    RWDisc.dvRAM.CommandExe |= eReadCmd;

    RWDisc.dvRAM.PreReadingOK = TRUE;

    //Initialize Variables
    //SpeedMD = Write_f;
    RAMClearCache(eReadCmd);
    mode = eMRWREAD_MODE;
    Fua = 0;        /* Force Unit Access */
    RWDisc.dvRAM.Stream &= (BYTE)~eReadStream;

    if (Iram_Cmd_block.bf[0] == SBC_READ_12)
    {
        if (Iram_Cmd_block.bf[1] & 1)   /* RelAdr == 1 */
        {
            BUILD_SENSE(0x05,0x24,0x00,0x9c);            /* INVALID FIELD IN CDB */
            read_exit_DVDRAM();
            return;
        }

        if (Iram_Cmd_block.bf[10] & 0x80)   /* Streaming bit == 1*/
        {
            if (Iram_Cmd_block.bf[1] & 0x8) /* Fua == 1 */
            {
                BUILD_SENSE(0x05,0x24,0x00,0x9d);            /* INVALID FIELD IN CDB */
                read_exit_DVDRAM();
                return;
            }
            RWDisc.dvRAM.Stream |= eReadStream;
            mode = eMRWSTREAM_MODE;
        }
        else if (Iram_Cmd_block.bf[1] & 0x8)
            Fua = 1;
    }

    /* Check for Sequential Read */
    if ((RWDisc.dvRAM.Rnxinfo.NextLSN + RWDisc.dvRAM.Rnxinfo.NextLen) == HostXfer.StartBlock.lba)
    {
          RWDisc.dvRAM.ContinuteReadCount++;
          if (RWDisc.dvRAM.ContinuteReadCount == 0xFF)
              RWDisc.dvRAM.ContinuteReadCount = 3;  //ring back from 3
    }
    else
    {
        RWDisc.dvRAM.ContinuteReadCount = 0;  //Reset count
        #if (DISCARD_RAM_DECODE == 1)
        RWDisc.dvRAM.ContinuteIceCount = 0;
        #endif
    }

    /* To improve Nero Transfer Rate, if a read cmd is behind seek cmd, start pre-buffering right away */
    if (CmdSeekOk == 1)
    {
        CmdSeekOk = 0;
        RWDisc.dvRAM.ContinuteReadCount = 3;
    }

    if (RWDisc.dvRAM.ContinuteReadCount >= 3)
          SequentialRead = SET;
    /* Check for Sequential Read */

    RWDisc.dvRAM.Rnxinfo.NextLSN = HostXfer.StartBlock.lba;
    RWDisc.dvRAM.Rnxinfo.NextLen = HostXfer.BlockCount.Long;

    //send_msg54(SEND_MRW_MSG, 'R', 'D', 'S', 'T');

    cnt = RAM_GetDefectInfo(RWDisc.dvRAM.Rnxinfo.NextLSN, RWDisc.dvRAM.Rnxinfo.NextLen);

    //send_msg5SValue(SEND_MRW_MSG,"CNT = ",1,cnt);

    for ( i = 0; i < cnt; i++)
    {
    #if DvdRamFastCache
        result = DVDRAMCheckPreBuffering(RWDisc.dvRAM.REntry[i].LSN, RWDisc.dvRAM.REntry[i].Len, 0);
    #else
        result = DVDRAMCheckPreBuffering(RWDisc.dvRAM.REntry[i].LSN, RWDisc.dvRAM.REntry[i].Len);
    #endif
        //send_msg5SValue(SEND_MRW_MSG,"Check = ",1,result);

        DISABLE_INTERRUPT();
        c_num = RAMcmFindTargetLSN(RWDisc.dvRAM.REntry[i].LSN, RWDisc.dvRAM.REntry[i].Len, (RWDisc.dvRAM.REntry[i].Attr & eSA_MASK) | eReadAccess);
        ENABLE_INTERRUPT();

        BlocksToBeBuffered = c_num;
        //send_msg5SValue(SEND_MRW_MSG,"c_num = ",1,c_num);

        for (k = 0; k< c_num; k++)  /* transfer one by one cache node in for loop */
        {
            pCacheHitNode = RAMcmGetCacheHitNode(k);

            tx_page = pCacheHitNode->StartDramPgNo; /* real start page inside a complete ECC block */
            tx_lsn = pCacheHitNode->StartLSN;
            tx_len = pCacheHitNode->Length;
            tx_seg = pCacheHitNode->SegmentNo;
            tx_segtype = DVDRAM_GET_SEG_TYPE(tx_seg);

            //send_msg5SValue(SEND_MRW_MSG,"k = ",1,k);
            //send_msg5SValue(SEND_MRW_MSG,"tx_page",2,tx_page);
            //send_msg5SValue(SEND_MRW_MSG,"tx_lsn",4,tx_lsn);
            //send_msg5SValue(SEND_MRW_MSG,"tx_len",2,tx_len);
            //send_msg5SValue(SEND_MRW_MSG,"tx_seg",1,tx_seg);
            //send_msg5SValue(SEND_MRW_MSG,"tx_segtype",1,tx_segtype);
            send_msg5SValue(SEND_MRW_MSG,"HitStatus",4,(tx_seg<<16|pCacheHitNode->HitStatus));

            if ((Fua == 1) || (pCacheHitNode->HitStatus == eMissed))
            {
    #if (DISCARD_RAM_DECODE == 1)
                RWDisc.dvRAM.ContinuteIceCount = 0;
    #endif

                send_msg5SValue(SEND_DVDRAM_ICE, "ReSt Buf", 4, RWDisc.dvRAM.PreLSN);

                DVDRAMStopPrebuffering();

                if ( (tx_segtype == eWriteSeg))// || (tx_segtype == eWriteLocked) )
                {
                    //RAMcmModifyWriteSegment(tx_seg, (RWDisc.dvRAM.REntry.Attr & eBlank));
                }
                else    /* ReadSeg, ReadLocked, EmptySeg */
                {
                    ULONG StartLSN, EndLSN;
                    BYTE  Start_Zone, Stop_Zone;

                    /* start buffing */
                    if(DVDRAM_GET_SEG_SLRBIT(tx_seg))  //Check if it is a skipped block (a defective real-time block)
                    {
                        send_msg5S(1,"SkippedBlock");
                        RWDisc.dvRAM.SkippedBlock = SET;
                    }

                    StartLSN = DVDRAM_GET_SEG_ECCLSN(tx_seg);
                    EndLSN   = StartLSN + (BlocksToBeBuffered - 1)*0x10;
                    Start_Zone = DVDRAM_LSN2ZONE(StartLSN);
                    Stop_Zone  = DVDRAM_LSN2ZONE(EndLSN);
                    if (Start_Zone == Stop_Zone)  // Requested blocks are in the same zone
                    {
                        DiscReadStatus = DVDRAMReadDataFromDisc( DVDRAM_GET_SEG_PAGE(tx_seg),(ULONG)DVDRAM_GET_SEG_ECCPSN(tx_seg),
                                     (ULONG)DVDRAM_GET_SEG_ECCLSN(tx_seg), BlocksToBeBuffered, mode);

                        if (DiscReadStatus != BUF_GOOD)
                        {
                            send_msg80(1,0x710222);

                            /* Free all of the SegmentInfo that registered at RAMcmFindTargetLSN() */
                            for (j = 0; j < c_num; j++)
                            {
                                pCacheHitNode = RAMcmGetCacheHitNode(j);
                                RAMcmFreeSegment(pCacheHitNode->SegmentNo);
                            }

                            dvdSetSenseCode(DiscReadStatus);
                            read_exit_DVDRAM();
                            return;
                        }

                        /* Update Hit Status and Full_filled information for CacheHitNode */
                        for (j = 0; j < c_num; j++)
                        {
                            pCacheHitNode = RAMcmGetCacheHitNode(j);
                            pCacheHitNode->HitStatus = eHit;
                            SegNo = pCacheHitNode->SegmentNo;
                            DVDRAM_SET_SEGMENT_FULL_FILLED(SegNo);
                        }
                    }
                    else  // Requested blocks are  not in the same zone
                    {
                        send_msg5S(1, "Zone Crossing");
                        DiscReadStatus = DVDRAMReadDataFromDisc( DVDRAM_GET_SEG_PAGE(tx_seg),(ULONG)DVDRAM_GET_SEG_ECCPSN(tx_seg),
                                         (ULONG)DVDRAM_GET_SEG_ECCLSN(tx_seg), 1, mode); // read blocks one by one

                        if (DiscReadStatus != BUF_GOOD)
                        {
                            send_msg80(1,0x710222);

                            /* Free all of the SegmentInfo that registered at RAMcmFindTargetLSN() */
                            RAMcmFreeSegment(tx_seg);
                            dvdSetSenseCode(DiscReadStatus);
                            read_exit_DVDRAM();
                            return;
                        }

                        /* Update Hit Status and Full_filled information for CacheHitNode */
                        pCacheHitNode->HitStatus = eHit;
                        DVDRAM_SET_SEGMENT_FULL_FILLED(tx_seg);
                    }

                    RWDisc.dvRAM.Prebuffering = FALSE;
                    RWDisc.dvRAM.SkippedBlock = CLEAR;
                }
            }   /* Endif (pCacheHitNode->HitStatus == eMissed) */
            /* The requested blocks hit in the cache */

            IceIndicator = (BYTE *)(DVD_RAM_APPLICATION_DATA_ADDR + (tx_page >> 4));  //IceIndicator is one byte revolution

            if ((*IceIndicator & eICE_BLOCK) || (*IceIndicator & eCERTIFIED_BLOCK)) //Zero fill ICE block before sending data to Host
            {
                send_msg80(1,0x710223);
                ZeroFillDRAM(tx_page, tx_len);
            }

            //PrintIceIndicator();

    #if DvdRamFastCache
            if (DVDRAMTransferHostData(tx_page, tx_len, 0) == 0) //Transfer OK.
    #else
            if (DVDRAMTransferHostData(tx_page, tx_len) == 0) //Transfer OK.
    #endif
            {
                USHORT page;

                //update BufPgCMP page
                page = READ_REG(HAL_HSTREG_MAP->BUFPGCMPL);
                WRITE_REG (HAL_HSTREG_MAP->BUFPGCMPL, ((page + tx_len) % (Iram_usiRingEP+1)));
                //send_msg5SValue(SEND_MRW_MSG,"page",2,page);
                //send_msg5SValue(SEND_MRW_MSG,"tx_len",2,tx_len);
                if (BlocksToBeBuffered > 0)
                    BlocksToBeBuffered--;
            }
            else
            {
                  send_msg80(SEND_MRW_MSG,0x710224);
                  return;
            }

            //send_msg54(SEND_MRW_MSG, 'R','e','a','E');

            if (tx_segtype == eReadLocked)
                DVDRAM_SET_SEG_TYPE(tx_seg, eReadSeg);

        }   /* for (k = 0; k< c_num; k++) */
    }   /* for ( i = 0; i < cnt; i++)  */

    read_exit_DVDRAM();

    StartTimer(&RWDisc.dvRAM.flush_timer);

    if (SequentialRead)
    {
        //send_msg5SValue(SEND_MRW_MSG,"SeqRead", 4, RWDisc.dvRAM.ContinuteReadCount<<24 | DataBuffer.CacheStartLBA & 0x00ffffff);

        result = DVDRAMPreReadBuffering();
        if (result != 0)
              send_msg5SValue(SEND_MRW_MSG,"result",1,result);
    }

    //CheckSramStack();

    return;

}


/*----------------------------------------------------------------------------
  Name: read_exit_DVDRAM
  Description:
        Exit read command process

  Input:  None

  Output: None
----------------------------------------------------------------------------*/
void read_exit_DVDRAM(void)
{
    ENABLE_INTERRUPT();                 /* Enable all interrupt */
    RWDisc.dvRAM.CommandExe &= ~eReadCmd;
    AutoCommandComplete();
}


/*----------------------------------------------------------------------------
  Name: process_key_in_DVDRAM
  Decription:
        Handles key press during MRW read commands processing

  Input:  None

  Output: None
----------------------------------------------------------------------------*/
static void process_key_in_DVDRAM(void)
{
    dvdStopDiscTransfer();
    if(St_send_blkdata == TRUE)
    {
        AbortHostTransfer();
        St_send_blkdata = FALSE;
    }

    St_cmd_inproc = FALSE;
    read_exit_DVDRAM();
}


/*----------------------------------------------------------------------------
  Name: DVDRAMTransferHostData
  Description:
        Handles the data transfer to host

  Input:  tx_page - start page address;
          tx_len  - number of pages to be transferred;

  Output: 0 - transfer completed;
          1 - transfer aborted;
----------------------------------------------------------------------------*/
    #if DvdRamFastCache
BYTE DVDRAMTransferHostData(USHORT tx_page, USHORT tx_len, BOOL FastCache)
    #else
BYTE DVDRAMTransferHostData(USHORT tx_page, USHORT tx_len)
    #endif
{
    #if DvdRamFastCache
    while (RWDisc.dvRAM.DiscSem != 0 && !FastCache)
    #else
    while (RWDisc.dvRAM.DiscSem != 0)
    #endif
    {
        ExitProcess();
    }

    if(tx_len == 0)
        return 0;

    HostXfer.BlockCount.Long = 0;
    HostXfer.BlocksLeft = tx_len;

    DataBuffer.CurHostPtr = tx_page;
    DataBuffer.SectorCount = tx_len;
    //send_msg5SValue(SEND_MRW_MSG,"tx_page",2,tx_page);
    //send_msg5SValue(SEND_MRW_MSG,"tx_len",4,tx_len);

    St_cmd_inproc = TRUE;

    //HostXfer.State = WAITING_FOR_TRANSFER;

    SendHostBlocks();

    #if DvdRamFastCache
    if (FastCache)
        return 0;
    #endif

    while (1)
    {
        if(EjectKeyInfo.PressedValid){
            BUILD_SENSE(0x02,0x3A,0x00,0x4b);    /* MEDIUM NOT PRESENT */
            process_key_in_DVDRAM();
            return 1;
        }

        if (St_cmd_inproc == FALSE)
            break;
        else
        {
            if (DataBuffer.SectorCount == 0)
                break;
        }

        ExitProcess();
    }

    return 0;
}


    #if DvdRamFastCache
BYTE DVDRAMCheckPreBuffering(LONG StartLSN, USHORT Len, BOOL FastCache)
    #else
BYTE DVDRAMCheckPreBuffering(LONG StartLSN, USHORT Len)
    #endif
{
    ULONG  BufferingTimer;
    ULONG  EndLSN;
    USHORT BlkToBuffer;
    BYTE   BlkIncreaseCnt;

    #if DvdRamFastCache
    if (RWDisc.dvRAM.Prebuffering == FALSE)
    {
        if(!RWDisc.dvRAM.CacheFull)
        return 1;
        else
        {
            send_msg5SValue(SEND_MRW_MSG, "CahFul", 4, RWDisc.dvRAM.ContinuteReadCount<<24|RWDisc.dvRAM.Rnxinfo.NextLSN & 0x00ffffff);
        }
    }
    #else
    if (RWDisc.dvRAM.Prebuffering == FALSE)
        return 1;
    #endif

    #if DvdRamFastCache
    if(RWDisc.dvRAM.BgPreBuffer != 0 && FastCache)
    #else
    if(RWDisc.dvRAM.BgPreBuffer != 0)
    #endif
        return 3;

    EndLSN = (StartLSN + Len + 0x0F) & ~0x0FL;  /* align to ECC start */

    //send_msg5SValue(SEND_MRW_MSG,"DVD_BUF",1,DVD_BUFFERING());

    if (DVD_BUFFERING() == ON)    /* Decoder is buffering */
    {
        //send_msg5SValue(SEND_MRW_MSG,"EndLSN",4,EndLSN);

        //If BlkToBuffer decreases to 0, decoder will stop. we don't want this situation happening during prebuffering.
        //Always feed blocks to decoder when BlkToBuffer is low.
        BlkToBuffer = READ_REG(HAL_DVDDREG_MAP->BKTOBUFL);
        if( BlkToBuffer <= 20 )
        {
            if( DataBuffer.CacheEndLBA >  EndLSN )
            {
                int RemainCacheBlkCnt;
                RemainCacheBlkCnt = (DataBuffer.CacheEndLBA - EndLSN)/ONE_DVD_BLOCK;
                if((eRAMCacheSegmentCnt - 3) > (RemainCacheBlkCnt + BlkToBuffer))
                {
                    BlkIncreaseCnt = (eRAMCacheSegmentCnt - 3) - RemainCacheBlkCnt - BlkToBuffer;
                    WRITE_REG( HAL_DVDDREG_MAP->BLKINC, BlkIncreaseCnt);
                    send_msg54(SEND_MRW_MSG,'a', RemainCacheBlkCnt, BlkToBuffer, BlkIncreaseCnt);
                }
            }
            else if( EndLSN >= DataBuffer.CacheEndLBA)
            {
                BlkIncreaseCnt = (eRAMCacheSegmentCnt - 3) - BlkToBuffer;
                WRITE_REG( HAL_DVDDREG_MAP->BLKINC, BlkIncreaseCnt);
                send_msg54(SEND_MRW_MSG,'b','b','b', BlkIncreaseCnt);
            }
        }

        //Partial Hit, Extend Read. If host requested block is going to be buffered.
    #if DvdRamFastCache
        //FastCache did not check partial hit
        if ( !FastCache && (DataBuffer.CacheEndLBA < EndLSN) && (EndLSN < (DataBuffer.CacheEndLBA + 2 + 0x30)) )
    #else
        if ( (DataBuffer.CacheEndLBA < EndLSN) && (EndLSN < (DataBuffer.CacheEndLBA + 2 + 0x30)) )
    #endif
        {
            //send_msg5SValue(SEND_MRW_MSG,"EndLBA",4,DataBuffer.CacheEndLBA);

            //Extend Read Loadbuffer
            ENABLE_DVD_DECODER();                   /* Turn on DVD decoder */

            WRITE_FIELD(HAL_DVDD_TIDDIS,0);     /* enable target id */

            StartTimer(&BufferingTimer);

            while (1)
            {
                /* Check Guard Area and then Skip reading at those area */
                if(RWDisc.dvRAM.ReadSkip == 0)
                    RamReadSkipForPrebuffering();

                if ((DataBuffer.CacheEndLBA + 1) >= EndLSN)
                {
                    send_msg5S(SEND_MRW_MSG,"Block Cached!");
                    break;
                }

                if ( DVD_BLK_ERROR || DVD_BUFFER_ERROR )
                {
//                    dvdDspSystemInit();
                    send_msg5S(SEND_MRW_MSG,"ECC-NG");
                    break;
                }

                if (DVD_BUFFERING() == OFF) /* decoder stopped */
                {
                    send_msg5S(SEND_MRW_MSG,"Decoder Stop");
                    break;
                }

                if (ReadTimer(&BufferingTimer) > 5000L)
                {
                    send_msg5S(SEND_MRW_MSG, "5s timeout");
                    break;
                }
                ExitProcess();
            }
            send_msg5SValue(SEND_MRW_MSG,"EndLBA",4,DataBuffer.CacheEndLBA);
        }
        return 0;
    }
    else
    {
        ULONG CacheUsage;

        CacheUsage = DataBuffer.CacheEndLBA - EndLSN + 1;
        send_msg5SValue(SEND_MRW_MSG, "Decdr Off", 4, RWDisc.dvRAM.BgPreBuffer<<24|CacheUsage& 0x00ffffff);

        #if 1 // Mark it temporally. Wait for verifying...
        /* If the host request quickly, cache will be maintained in low value.
            FW will start prebuffering in end of read command, not in BG task. */
        if((0x50 <= CacheUsage && CacheUsage <= PREBUF_THRESHOLD) && RWDisc.dvRAM.BgPreBuffer == 0)
        {
            RWDisc.dvRAM.BgPreBuffer = 1;
            return 4;
        }
        #endif

        return 2;
    }
}


BYTE DVDRAMPreReadBuffering(void)
{
    ULONG RdEndLSN;
    BYTE i;
    BYTE result;
    BYTE PreEccLength;

    if (RWDisc.dvRAM.WriteSegmentCount != 0)
        return 0xF0;

    if (RWDisc.dvRAM.PreReadingOK == FALSE)
        return 0xF1;

    if (DVD_BUFFERING() == ON)
        return 0xF2;

    RdEndLSN = RWDisc.dvRAM.Rnxinfo.NextLSN + RWDisc.dvRAM.Rnxinfo.NextLen;

    #if DvdRamFastCache
    if ((RWDisc.dvRAM.PreStartLSN < RdEndLSN) && (RdEndLSN < RWDisc.dvRAM.PreLSN))      /* last Read data still in Cache */
    #else
    if ( (RWDisc.dvRAM.Prebuffering== TRUE) &&
         (RWDisc.dvRAM.PreStartLSN < RdEndLSN) && (RdEndLSN < RWDisc.dvRAM.PreLSN))     /* last Read data still in Cache */
    #endif
        return 0xF3;

    if (RWDisc.dvRAM.DiscSem != 0 )
        return 0xF4;

    RWDisc.dvRAM.PreLSN = (RdEndLSN + 0x0F ) & (~0x0FL);
    RWDisc.dvRAM.PreSegNo = RWDisc.dvRAM.NextReadSegNo;
    PreEccLength = 0;

    //Free Segments and Calculate pre-buffering length
    for (i = 0; i < eRAMCacheSegmentCnt - 2; i ++)
    {
        BYTE j;
        j = i + RWDisc.dvRAM.PreSegNo;
        if (j >= eRAMCacheSegmentCnt)
            j -= eRAMCacheSegmentCnt;

        //Skip write segment
        if (DVDRAM_GET_SEG_TYPE(j) == eWriteSeg)
            break;

        RAMcmFreeSegment(j);
        PreEccLength += 1;
    }

    if (PreEccLength == 0)
        return 0xF5;

    RWDisc.dvRAM.PrePage = DVDRAM_GET_SEG_PAGE(RWDisc.dvRAM.PreSegNo);

    DataBuffer.CacheStartLBA = RWDisc.dvRAM.PreLSN;
    DataBuffer.CacheEndLBA = RWDisc.dvRAM.PreLSN - 1;  //for pre-buffering check

    result = DVDRAMStartPrebuffering(RWDisc.dvRAM.PrePage, RWDisc.dvRAM.PreLSN, PreEccLength, eRAM_READ_MODE);

    if (result == BUF_GOOD)
    {
        RWDisc.dvRAM.Prebuffering = TRUE;
        RWDisc.dvRAM.PreStartLSN = RWDisc.dvRAM.PreLSN;

        send_msg5SValue(SEND_MRW_MSG,"PreBuf LSN",4,RWDisc.dvRAM.PreLSN);
        send_msg5SValue(SEND_MRW_MSG,"PreBuf Pg",2,RWDisc.dvRAM.PrePage);
        send_msg5SValue(SEND_MRW_MSG,"PreEcc Len",2,PreEccLength);
    }
    else
    {
          send_msg5SValue(SEND_MRW_MSG,"PreBuf NG",1,result);
    }
    return result;
}


/*******************************************************************************
* Name: DVDRAMReadDataFromDisc
* Description:
*   Read data from DVD-RAM disc
* Inputs:  start_page - DRAM start page to put the buffered data
*            blk_start_psn - the buffering target
*          length - ecc block count
* Output:  Status of the disc access during buffering
********************************************************************************/
DiscAccessStatusE DVDRAMReadDataFromDisc(USHORT start_page,ULONG read_start_psn,ULONG blk_start_lsn,BYTE length, BYTE mode)
{
    BYTE retry, target_zone, InterruptState, sectors_per_track;
    USHORT blk_start_page;
    ULONG  blk_start_psn,targetPSN, temp;
    DiscAccessStatusE   DiscReadStatus;

    send_msg5SValue(SEND_MRW_MSG,"blk_s_psn",4,read_start_psn);
    send_msg5SValue(SEND_MRW_MSG,"blk_s_lsn",4,blk_start_lsn);
    send_msg5SValue(SEND_MRW_MSG,"pg&len",4, start_page<<16|length);

    retry = 0;
    targetPSN = temp = 0;
    dvdDiscRequest.EccBlockCount = length;

    RamRetryCnt   = MAX_DVDRAM_READ_RETRIES;
    DiscReadStatus = BUF_GOOD;
    dvdDiscRequest.EccBlocksBuffered  = 0L;
    dvdramLastDecodedLSN = blk_start_lsn - 1L;
    #if (ENABLE_RAM_DFE_ICE_DETECT == 1)
    dvdramDecodedStrPSN = ConvertLSNToPSN(blk_start_lsn);
    #endif
    DataBuffer.CurDiscPtr = start_page;

    dvdDecFlag.fREADING_LEADIN = FALSE;
    RWDisc.dvRAM.IceHasBeenDetected =  CLEAR;
    RWDisc.dvRAM.ReadDataMode = eREAD_DATA_FROM_DISC;

    ENABLE_DVD_DECODER();                   /* Turn on DVD decoder */

    WRITE_FIELD(HAL_DVDD_TIDDIS,0);     /* enable target id */

    while (dvdDiscRequest.EccBlocksBuffered < length)
    {
        blk_start_psn = read_start_psn + dvdDiscRequest.EccBlocksBuffered * ONE_DVD_BLOCK;
        blk_start_page = (start_page + dvdDiscRequest.EccBlocksBuffered * ONE_DVD_BLOCK) % eRAMTotalPageNo;


          if (!svoIramVar.knlTimeupCount)
        {  /* time out */
            dvdStopDiscTransfer();                  /* stop disc transferring       */
            dvdResetDiscFlags();
            CLEAR_DVD_DECODER_STATUS();                 /* clear all remaining status   */
            Svr_pause_reqire = TRUE;                /* Set pause request to servo   */
            Iram_Svr_player_mode = PAUSING_MODE;    /* Set pause mode for player    */
            DiscReadStatus = TIMEOUT;
            return(DiscReadStatus);                 /* Return the disc read error   */
        }

        if( DVD_BUFFERING() == ON )
            dvdStopDiscTransfer();

        if ( DVD_BLK_ERROR || DVD_BUFFER_ERROR )
        {
            DiscReadStatus = GetDvdError();

            if ( (RamRetryCnt == 0) || (DiscReadStatus == UNAUTH_RD_FAIL) )
            {
                /*---------------------------------------------------------------
                     Maximum number of retries have performed or unauthorized
                     read (no retries necessary) attempted
                ----------------------------------------------------------------*/
                dvdStopDiscTransfer();                  /* stop disc transferring       */
                CLEAR_DVD_DECODER_STATUS();             /* clear all remaining status   */
                Svr_pause_reqire = TRUE;                /* Set pause request to servo   */
                Iram_Svr_player_mode = PAUSING_MODE;    /* Set pause mode for player    */
                return(DiscReadStatus);                 /* Return the disc read error   */
            }
            --RamRetryCnt;

            retry = MAX_DVDRAM_READ_RETRIES - RamRetryCnt;

    #if (WORKAROUND_FOR_SEEDMISTHR == 1)
            RestSeedCount = retry;
    #endif

            target_zone = DVDRAM_PSN2ZONE(blk_start_psn);
            if (retry == 1)
                targetPSN = blk_start_psn;
            sectors_per_track = DVDRAMZoneTable[target_zone].SectorsPerRev;

            if (retry < 8)
            {
                temp = targetPSN - ((sectors_per_track / 14) * retry);
            }

            if (temp > DVDRAMZoneTable[target_zone].StartUserSectorPSN - (sectors_per_track/2))
            {
                targetPSN = temp;
            }
            else
            {
                targetPSN = DVDRAMZoneTable[target_zone].StartUserSectorPSN - (sectors_per_track/2);
            }

        }

        InterruptState = GET_INT_ENABLE_MASK();
        DISABLE_INTERRUPT();

        /* Initialize transfer variables */
        STOP_DVD_BLOCK_TIMER();
        dvdResetDiscFlags();

        SET_DISC_BUFFER_PTR(blk_start_page);

        SET_TARGET_ID(blk_start_psn);

        SET_DVD_BLOCKS_TO_BUFFER(length - dvdDiscRequest.EccBlocksBuffered);

        /*---------------------------------------------------------------
        Clear all DVD flags and status and set transfer state
        ----------------------------------------------------------------*/
        dvdResetDiscFlags();
        CLEAR_DVD_DECODER_STATUS();

        START_DVD_BLOCK_TIMER(FIRST_BLOCK_TIMEOUT_VALUE);
        ENABLE_DVD_DECODER_INTS();

        SET_INT_ENABLE_MASK(InterruptState);

        RWDisc.dvRAM.IceHasBeenDetected = CLEAR;


        /*-------------------------------------
            seek to target
        --------------------------------------*/
        if (retry >= 1)
        {
            blk_start_psn = targetPSN;
        }

        if( dvdSeek(blk_start_psn, 0) == Error)
        {
            SpeedCount = SPEED_DOWN_READY_LIMIT;// speed down
            dvdErrorFlag.fSEEK_FAIL = TRUE;
            DiscReadStatus = SEEK_FAIL;
    #if (WORKAROUND_FOR_SEEDMISTHR == 1)
            RestSeedCount = 0;
    #endif
        }
        else
        {
            /*---------------------------------------------------------------
                    Seek successful and start decoding
            ----------------------------------------------------------------*/
            ENABLE_TARGET_MISS_INT();
            DiscReadStatus       = BUF_GOOD;
            CLEAR_SYNC_LOST_STATUS();
            ENABLE_DVD_DECODER_INTS();
            ENABLE_BUFI_INT();
            ENABLE_DVD_BUFFERING();

            START_DVD_BLOCK_TIMER(DVDRAM_BLOCK_TIMEOUT_VALUE);
    #if (WORKAROUND_FOR_SEEDMISTHR == 1)
            RestSeedCount = 0;
            if (ChangeSeedMisThr == TRUE)
            {
                send_msg5S(SEND_MRW_MSG,"SEEDMISTHR_2");
                WRITE_FIELD(HAL_DVDD_SEEDMISTHR,2);
                ChangeSeedMisThr = FALSE;
            }
    #endif
            //send_msg5S(1,"Seek OK");
        }

    #if (ENABLE_RAM_DFE_ICE_DETECT == 1)
        HAL_DFE_EnableICEDetect(); //enable Iced area detect
        HAL_DFE_SetIceDetectStAddr(READ_FIELD( HAL_DVDD_TID));
        HAL_DFE_ClusterStateEnableInt();
    #endif

        ReadyLED(LED_OFF);

        /*-------------------------------------------------------------------
                Monitor DVD Buffering
        --------------------------------------------------------------------*/
        while(1)
        {
            if (!svoIramVar.knlTimeupCount)
            {  /* time out */
                dvdStopDiscTransfer();                  /* stop disc transferring       */
                dvdResetDiscFlags();
                CLEAR_DVD_DECODER_STATUS();                 /* clear all remaining status   */
                Svr_pause_reqire = TRUE;                /* Set pause request to servo   */
                Iram_Svr_player_mode = PAUSING_MODE;    /* Set pause mode for player    */
                DiscReadStatus = TIMEOUT;
                return(DiscReadStatus);                 /* Return the disc read error   */
            }

            //if(pdbServoGetDiscKind() == eDVDRAMDisc && RWDisc.dvRAM.ReadSkip == 0)  //check ram disc
                //RamReadSkip();

            if(svoIramVar.focusDropFlag == TRUE)
            {
                send_msg5S(SEND_MRW_MSG,"RdDisc foDp");
                dvdErrorFlag.fUNCOR_ECC = TRUE;
                break;
                //return(GEN_FAIL);
            }

            /*---------------------------------------------------------------
                Check for buffered block errors posted by DVD ISR
            ----------------------------------------------------------------*/
            if (DVD_BLK_ERROR)
            {
//                dvdDspSystemInit();
                break;
            }
            /*---------------------------------------------------------------
                Check for buffering errors (non block errors)
            ----------------------------------------------------------------*/
            if (DVD_BUFFER_ERROR)
            {
                send_msg5SValue(SEND_MRW_MSG, "BUF_error", 4, dvdErrorFlag.fSEEK_FAIL<<24 | dvdErrorFlag.fSYNC_LOST<<16 | dvdErrorFlag.fTID_MISS<<8 | dvdBlockInterruptTimer.fTIMEOUT);

                //if (((READ_REG(HAL_DVDDREG_MAP->DVDIRQS3) & 0x10) != 0) || (dvdBlockInterruptTimer.fTIMEOUT == TRUE))
                {
//                    dvdDspSystemInit();
                }
                break;
            }

            /*---------------------------------------------------------------
                Check if all requested blocks have been buffered
            ----------------------------------------------------------------*/
            if(dvdDecIntFlag.fECC_DONE_INT)
                break;

            /*---------------------------------------------------------------
                When disable decoder in ICE area, there is no dvdDecIntFlag.fECC_DONE_INT
            ----------------------------------------------------------------*/
            if(dvdDiscRequest.EccBlocksBuffered >= length)
            {
                send_msg5S(SEND_MRW_MSG, "RdDsc ok");
                break;
            }

            ExitProcess();
        } /* End Monitor Buffering Loop */
    } /* END RamRetryCnt Loop */

    /*---------------------------------------------------------------
       All requested blocks have been bufferred. Stop the transfer
       and reset the disc flags
    ----------------------------------------------------------------*/
    dvdStopDiscTransfer();
    dvdResetDiscFlags();

    /*---------------------------------------------------------------
       Mark a servo pause request and return disc status
    ----------------------------------------------------------------*/
    Svr_pause_reqire = TRUE;
    return(DiscReadStatus);

}


BOOL IsCertifiedBlock(USHORT EccStartPage)
{
    BYTE   *IceIndicator;

    IceIndicator = (BYTE *)(DVD_RAM_APPLICATION_DATA_ADDR + (EccStartPage >> 4));  //IceIndicator is one byte revolution
    if ((*IceIndicator & eCERTIFIED_BLOCK) == eCERTIFIED_BLOCK)
        return TRUE;
    else
        return FALSE;
}

void PrintIceIndicator(void)
{
    BYTE   *IceIndicator;
    BYTE   i;

    IceIndicator = (BYTE *)DVD_RAM_APPLICATION_DATA_ADDR;

    send_msg5S(SEND_MRW_MSG,"Indicator");

    for (i=0; i<eRAMCacheSegmentCnt; i+=4)
    {
        send_msg54(SEND_MRW_MSG,*(IceIndicator+0),*(IceIndicator+1),*(IceIndicator+2),*(IceIndicator+3));
        IceIndicator+=4;
    }
}

/*******************************************************************************
* Name: DVDRAMStartPrebuffering
* Description:
*   Pre-buffer data from DVD-RAM disc
* Inputs:  start_page - DRAM start page to put the buffered data
*            blk_start_psn - the buffering target
*          length - ecc block count
* Output:  Status of the disc access during buffering
* Note  : 1. No retry used in prebuffering.
*         2. Exit this function when first block has been buffered.
********************************************************************************/
DiscAccessStatusE DVDRAMStartPrebuffering(USHORT start_page,ULONG blk_start_lsn,BYTE length, BYTE mode)
{
    ULONG blk_start_psn;
    BYTE  InterruptState;
    DiscAccessStatusE   DiscReadStatus;
    ULONG BgPreBufTimer;

    send_msg5SValue(SEND_MRW_MSG,"blk_s_lsn",4,blk_start_lsn);
    send_msg5SValue(SEND_MRW_MSG,"pBf LnPg", 4, length <<16 | start_page);

    blk_start_psn = ConvertLSNToPSN(blk_start_lsn);

    DiscReadStatus = BUF_GOOD;
    dvdDiscRequest.EccBlocksBuffered  = 0L;
    dvdramLastDecodedLSN = blk_start_lsn - 1L;
    #if (ENABLE_RAM_DFE_ICE_DETECT == 1)
    dvdramDecodedStrPSN = ConvertLSNToPSN(blk_start_lsn);
    #endif

    DataBuffer.CurDiscPtr = start_page;

    dvdDecFlag.fREADING_LEADIN = FALSE;
    RWDisc.dvRAM.IceHasBeenDetected =  CLEAR;
    RWDisc.dvRAM.ReadDataMode = ePRE_BUFFERING;
    RWDisc.dvRAM.StartBufferingOK = FALSE;

    ENABLE_DVD_DECODER();                   /* Turn on DVD decoder */

    WRITE_FIELD(HAL_DVDD_TIDDIS,0);     /* enable target id */

    if( DVD_BUFFERING() == ON )
        dvdStopDiscTransfer();

    InterruptState = GET_INT_ENABLE_MASK();
    DISABLE_INTERRUPT();

    /* Initialize transfer variables */
    STOP_DVD_BLOCK_TIMER();

    SET_DISC_BUFFER_PTR(start_page);

    SET_TARGET_ID(blk_start_psn);

    SET_DVD_BLOCKS_TO_BUFFER(length);

    /*---------------------------------------------------------------
    Clear all DVD flags and status and set transfer state
    ----------------------------------------------------------------*/
    dvdResetDiscFlags();
    CLEAR_DVD_DECODER_STATUS();

    START_DVD_BLOCK_TIMER(DVDRAM_BLOCK_TIMEOUT_VALUE);
    ENABLE_DVD_DECODER_INTS();

    SET_INT_ENABLE_MASK(InterruptState);
    /*-------------------------------------
        seek to target
    --------------------------------------*/
    if( dvdSeek(blk_start_psn, 0) == Error)
    {
        //Down_SpeedCnt = 0xFF;
        dvdErrorFlag.fSEEK_FAIL = TRUE;
        dvdStopDiscTransfer();                  /* stop disc transferring       */
        dvdResetDiscFlags();
        CLEAR_DVD_DECODER_STATUS();             /* clear all remaining status   */
        return (SEEK_FAIL);
    }
    else
    {
        /*---------------------------------------------------------------
                Seek successful and start decoding
        ----------------------------------------------------------------*/
        ENABLE_TARGET_MISS_INT();
        DiscReadStatus       = BUF_GOOD;
        CLEAR_SYNC_LOST_STATUS();
        ENABLE_DVD_DECODER_INTS();
        ENABLE_BUFI_INT();
        ENABLE_DVD_BUFFERING();
        START_DVD_BLOCK_TIMER(DVDRAM_BLOCK_TIMEOUT_VALUE);
    }

    send_msg5S(SEND_MRW_MSG,"seek ok");

    #if (ENABLE_RAM_DFE_ICE_DETECT == 1)
    HAL_DFE_EnableICEDetect(); //enable Iced area detect
    HAL_DFE_SetIceDetectStAddr(READ_FIELD( HAL_DVDD_TID));
    HAL_DFE_ClusterStateEnableInt();
    #endif


    if(RWDisc.dvRAM.BgPreBuffer == 1)
        StartTimer(&BgPreBufTimer);

    /*-------------------------------------------------------------------
            Monitor DVD Buffering
    --------------------------------------------------------------------*/
    while(1)
    {
        if (!svoIramVar.knlTimeupCount)
        {  /* time out */
            dvdStopDiscTransfer();                  /* stop disc transferring       */
            dvdResetDiscFlags();
            CLEAR_DVD_DECODER_STATUS();                 /* clear all remaining status   */
            Svr_pause_reqire = TRUE;                /* Set pause request to servo   */
            Iram_Svr_player_mode = PAUSING_MODE;    /* Set pause mode for player    */
            DiscReadStatus = TIMEOUT;
            send_msg5SValue(SEND_MRW_MSG,"timeOut",1,DiscReadStatus);
            return(DiscReadStatus);                 /* Return the disc read error   */
        }

        if(svoIramVar.focusDropFlag == TRUE)
        {
            send_msg5S(SEND_MRW_MSG,"PreBuf foDp");
            return(GEN_FAIL);
        }

        /*---------------------------------------------------------------
            Check for buffered block errors posted by DVD ISR
        ----------------------------------------------------------------*/
        if (DVD_BLK_ERROR)
        {
            send_msg5SValue(SEND_MRW_MSG,"BLK_error", 4, dvdErrorFlag.fSEQ_ID_ERROR<<24 | dvdErrorFlag.fUNAUTH_READ_ERROR<<16 | dvdErrorFlag.fID_MISMATCH<<8 | dvdErrorFlag.fID_ERROR);
//            dvdDspSystemInit();
            dvdStopDiscTransfer();                  /* stop disc transferring       */
            dvdResetDiscFlags();
            CLEAR_DVD_DECODER_STATUS();             /* clear all remaining status   */
            DiscReadStatus = TIMEOUT;
            break;
        }
        /*---------------------------------------------------------------
            Check for buffering errors (non block errors)
        ----------------------------------------------------------------*/
        if (DVD_BUFFER_ERROR)
        {
            //if (((READ_REG(HAL_DVDDREG_MAP->DVDIRQS3) & 0x10) != 0) || (dvdBlockInterruptTimer.fTIMEOUT == TRUE))
//            {
//                dvdDspSystemInit();
//            }
            send_msg5SValue(SEND_MRW_MSG,"BUF_err", 4, dvdErrorFlag.fSEEK_FAIL<<24 | dvdErrorFlag.fSYNC_LOST<<16 | dvdErrorFlag.fTID_MISS<<8 | dvdBlockInterruptTimer.fTIMEOUT);
            dvdStopDiscTransfer();                  /* stop disc transferring       */
            dvdResetDiscFlags();
            CLEAR_DVD_DECODER_STATUS();             /* clear all remaining status   */
            DiscReadStatus = TIMEOUT;
            break;
        }

        /*---------------------------------------------------------------
            Check if first blocks have been buffered
        ----------------------------------------------------------------*/
        if(RWDisc.dvRAM.StartBufferingOK)
        {
            send_msg5SValue(SEND_MRW_MSG,"SB_OK", 4, RWDisc.dvRAM.Prebuffering<<16|READ_REG(HAL_DVDDREG_MAP->BKTOBUFL));
            break;
        }

        if(RWDisc.dvRAM.BgPreBuffer == 1)
        {
            if(ReadTimer(&BgPreBufTimer) > 100 || RWDisc.dvRAM.BgPreBufAbort)
            {
                dvdStopDiscTransfer();                  /* stop disc transferring       */
                dvdResetDiscFlags();
                CLEAR_DVD_DECODER_STATUS();             /* clear all remaining status   */
                send_msg5SValue(SEND_MRW_MSG,"pBuf_Tout", 2, READ_REG(HAL_DVDDREG_MAP->BKTOBUFL));
                DiscReadStatus = TIMEOUT;
                break;
            }
        }

        //protection for ICE -> NonICE, FW can't decode data out due to calling dvdStopDiscTransfer()
        if(DVD_BUFFERING() == OFF)
        {
            dvdStopDiscTransfer();                  /* stop disc transferring       */
            dvdResetDiscFlags();
            CLEAR_DVD_DECODER_STATUS();             /* clear all remaining status   */
            send_msg5SValue(SEND_MRW_MSG,"pBuf OFF", 2, READ_REG(HAL_DVDDREG_MAP->BKTOBUFL));
            DiscReadStatus = GEN_FAIL;
            break;
        }
        //===================

        ExitProcess();
    } /* End Monitor Buffering Loop */

    return(DiscReadStatus);
}

/*----------------------------------------------------------------------------
  Name: DVDRAMAppendPreBuffer()
  Description:

  Input:  None

  Output: None
----------------------------------------------------------------------------*/
void DVDRAMAppendPreBuffer(void)
{
    send_msg5SValue(SEND_MRW_MSG,"Append LSN", 4, RWDisc.dvRAM.PreSegNo<<24 | RWDisc.dvRAM.PreLSN & 0x00ffffff);

    if ((DVDRAM_GET_SEG_TYPE(RWDisc.dvRAM.PreSegNo) == eEmptySeg) ||
        (DVDRAM_GET_SEG_TYPE(RWDisc.dvRAM.PreSegNo) == eReadSeg))
    {
        DVDRAM_SET_SEG_ECCLSN(RWDisc.dvRAM.PreSegNo, RWDisc.dvRAM.PreLSN);
        DVDRAM_SET_SEG_ECCPSN(RWDisc.dvRAM.PreSegNo, ConvertLSNToPSN(RWDisc.dvRAM.PreLSN));
        DVDRAM_SET_SEG_FILLINFO(RWDisc.dvRAM.PreSegNo, 0xFFFF);
        DVDRAM_SET_SEG_TYPE(RWDisc.dvRAM.PreSegNo, eReadSeg);

        if (RAMcmRegisterSegment(RWDisc.dvRAM.PreSegNo))  //Segment is registered OK.
        {
            RWDisc.dvRAM.PreSegNo ++;
            RamADD_SEGMENT(RWDisc.dvRAM.NextReadSegNo, 1);
            if (RWDisc.dvRAM.PreSegNo >= eRAMCacheSegmentCnt)
                RWDisc.dvRAM.PreSegNo = 0;

            RWDisc.dvRAM.PreLSN += ONE_DVD_BLOCK;
            RWDisc.dvRAM.PrePage = DVDRAM_GET_SEG_PAGE(RWDisc.dvRAM.PreSegNo);
        }
        else
        {
            RWDisc.dvRAM.Prebuffering = FALSE;
            dvdStopDiscTransfer();
            Svr_pause_reqire = TRUE;                /* Set pause request to servo   */
        }

    #if DvdRamFastCache
        RWDisc.dvRAM.CacheFull = 0;
    #endif
    }
    else
    {
        send_msg5SValue(SEND_MRW_MSG, "stp2", 4, RWDisc.dvRAM.Prebuffering<<24 | DataBuffer.CacheStartLBA & 0x00ffffff);
        send_msg5SValue(SEND_MRW_MSG,"SegType",1,DVDRAM_GET_SEG_TYPE(RWDisc.dvRAM.PreSegNo));
        RWDisc.dvRAM.Prebuffering = FALSE;
        dvdStopDiscTransfer();
        Svr_pause_reqire = TRUE;                /* Set pause request to servo   */

    #if DvdRamFastCache
        RWDisc.dvRAM.CacheFull = 1;
    #endif
    }
}


/*******************************************************************************
* Name: RamReadSkipForPrebuffering
* Description : Disbale read buffering at current zone end, and enable at next zone start
* Inputs:       none
* Outputs:      none
*******************************************************************************/
void RamReadSkipForPrebuffering(void)
{
    DDSTYPE * pDDS;
    BYTE i,CurZone;

    if ( RWDisc.dvRAM.PreLSN >= ( GetDVDRAMMaxLBA() + 1 ) )
    {
        send_msg80(SEND_MRW_MSG,0x710225);
        RWDisc.dvRAM.Prebuffering = FALSE;
        dvdStopDiscTransfer();
        return;
    }

    //EndLBA = HostXfer.StartBlock.lba + HostXfer.BlockCount.Long - 1;
    i = DVDRAM_LSN2ZONE(HostXfer.StartBlock.lba);
    CurZone = DVDRAM_LSN2ZONE(RWDisc.dvRAM.PreLSN);
    if (i == CurZone) //In the same zone
        return;

    //CurZone is the new one.
    //CurZone = DVDRAM_LSN2ZONE(RWDisc.dvRAM.PreLSN);
    pDDS = (DDSTYPE *) DRAM_START_DDS;

    //We are now going across zones
    //Current position should be at first guard block of (CurZone-1)
    if ( RWDisc.dvRAM.PreLSN == (SWAPEND_ULONG(pDDS->StartLsn[CurZone])))
    {
        send_msg80(SEND_MRW_MSG,0x710226);

        /* Disable DVD buffering */
        RWDisc.dvRAM.ReadSkip = 1;
        dvdStopDiscTransfer();

        DVDRAMStartPrebuffering(DataBuffer.CurDiscPtr, RWDisc.dvRAM.PreLSN, 20, eRAM_READ_MODE);

    }
}


/***************************************************************************************************************
* Name: DVDRAMReadBlockToTempBuffer
* Description:
*   Read one ecc block from DVD-RAM disc and the buffering start page is not in RING BUFFER.
* Inputs:  BlkStartPSN - the buffering target
* Output:  Status of the disc access during buffering
* Note  :  1. Retry is necessary.
           2. The buffering area is out of RING BUFFER. Before buffering, we need to switch RING BUFFER to
              a Temp Buffer, and be sure to switch back to RING BUFFER when we are going to leave this function.
           3. If we can not read this block back successfully, we may write the wrong data to disc.
              That will result in a fatal error of data corruption.
***************************************************************************************************************/
DiscAccessStatusE DVDRAMReadBlockToTempBuffer(ULONG BlkStartLSN,ULONG BlkStartPSN)
{
    BYTE  *IceIndicator, i, retry, target_zone, sectors_per_track;
    BYTE  InterruptState;
    ULONG targetPSN, temp, TargetID;
    DiscAccessStatusE   DiscReadStatus;

    send_msg5SValue(SEND_MRW_MSG,"BlkLSN",4,BlkStartLSN);
    send_msg5SValue(SEND_MRW_MSG,"BlkPSN",4,BlkStartPSN);

    retry = 0;
    targetPSN = temp = 0;
    TargetID = BlkStartPSN;
    RamRetryCnt   = MAX_DVDRAM_READ_RETRIES;
    DiscReadStatus = BUF_GOOD;
    dvdDiscRequest.EccBlocksBuffered  = 0L;
    dvdramLastDecodedLSN = BlkStartLSN - 1L;  //For ID verification
    #if (ENABLE_RAM_DFE_ICE_DETECT == 1)
    dvdramDecodedStrPSN = ConvertLSNToPSN(BlkStartLSN);
    #endif
    DataBuffer.CurDiscPtr = DVD_RAM_MODIFY_WRITE_PAGE;

    dvdDecFlag.fREADING_LEADIN = FALSE;
    RWDisc.dvRAM.IceHasBeenDetected =  CLEAR;
    RWDisc.dvRAM.ReadDataMode = eREAD_MODIFY_WRITE;

    /* switch to temp buffer area */
    Iram_usiRingSP = DVDRAM_VERIFY_START_PAGE;
    Iram_usiRingEP = DVDRAM_VERIFY_END_PAGE;

    WRITE_REG(HAL_DVDDREG_MAP->PIPOSRTL, DVDRAM_VERIFY_PIPO_START_BLOCK);  /* PI/PO start blk. */
    WRITE_REG(HAL_DVDDREG_MAP->STARTBKL, Iram_usiRingSP);           /* ring start blk.  */
    WRITE_REG(HAL_DVDDREG_MAP->ENDBKL, Iram_usiRingEP);             /* ring end blk.    */

    /* Initialize ICE Indicator */
    IceIndicator = (BYTE *)(DVD_RAM_APPLICATION_DATA_ADDR);
    for (i = 0; i < MAX_DVD_BLOCKS; i++)
        *IceIndicator++ = 0;

    ENABLE_DVD_DECODER();                   /* Turn on DVD decoder */

    WRITE_FIELD(HAL_DVDD_TIDDIS,0);     /* enable target id */

    while (dvdDiscRequest.EccBlocksBuffered < 1) //we just need to read one block.
    {
        if (!svoIramVar.knlTimeupCount)
        {  /* time out */
            dvdStopDiscTransfer();                  /* stop disc transferring       */
            dvdResetDiscFlags();
            CLEAR_DVD_DECODER_STATUS();                 /* clear all remaining status   */
            Svr_pause_reqire = TRUE;                /* Set pause request to servo   */
            Iram_Svr_player_mode = PAUSING_MODE;    /* Set pause mode for player    */
            DiscReadStatus = TIMEOUT;
            SwitchBackDVDRAMBuffer();
            return(DiscReadStatus);                 /* Return the disc read error   */
        }

        if( DVD_BUFFERING() == ON )
            dvdStopDiscTransfer();

        if ( DVD_BLK_ERROR || DVD_BUFFER_ERROR )
        {
            DiscReadStatus = GetDvdError();
            send_msg5SValue(SEND_MRW_MSG,"RetryCnt",1,RamRetryCnt);

            if ( (RamRetryCnt == 0) || (DiscReadStatus == UNAUTH_RD_FAIL) )
            {
                /*---------------------------------------------------------------
                     Maximum number of retries have performed or unauthorized
                     read (no retries necessary) attempted
                ----------------------------------------------------------------*/
                dvdStopDiscTransfer();                  /* stop disc transferring       */
                CLEAR_DVD_DECODER_STATUS();             /* clear all remaining status   */
                Svr_pause_reqire = TRUE;                /* Set pause request to servo   */
                Iram_Svr_player_mode = PAUSING_MODE;    /* Set pause mode for player    */
                SwitchBackDVDRAMBuffer();
                return(DiscReadStatus);                 /* Return the disc read error   */
            }

            --RamRetryCnt;

            retry = MAX_DVDRAM_READ_RETRIES - RamRetryCnt;
            target_zone = DVDRAM_PSN2ZONE(BlkStartPSN);
            if (retry == 1)
                targetPSN = BlkStartPSN;
            sectors_per_track = DVDRAMZoneTable[target_zone].SectorsPerRev;
            if (retry < 8)
            {
                temp = targetPSN - 1;
            }

            if ((target_zone == 0) || (temp > DVDRAMZoneTable[target_zone].StartSectorPSN+sectors_per_track))
            {
                targetPSN = temp;
            }
            else
            {
                targetPSN = DVDRAMZoneTable[target_zone].StartSectorPSN+sectors_per_track;
            }
        }

        InterruptState = GET_INT_ENABLE_MASK();
        DISABLE_INTERRUPT();

        /* Initialize transfer variables */
        STOP_DVD_BLOCK_TIMER();

        dvdDiscRequest.EccBlocksBuffered  = 0L;

        SET_DISC_BUFFER_PTR(DVD_RAM_MODIFY_WRITE_PAGE);

        SET_TARGET_ID(TargetID);

        SET_DVD_BLOCKS_TO_BUFFER(1);

        /*---------------------------------------------------------------
        Clear all DVD flags and status and set transfer state
        ----------------------------------------------------------------*/
        dvdResetDiscFlags();
        CLEAR_DVD_DECODER_STATUS();

        START_DVD_BLOCK_TIMER(FIRST_BLOCK_TIMEOUT_VALUE);
        ENABLE_DVD_DECODER_INTS();

        SET_INT_ENABLE_MASK(InterruptState);
        /*-------------------------------------
            seek to target
        --------------------------------------*/
        if (retry >= 1)
        {
            BlkStartPSN = targetPSN;
        }

        if( dvdSeek(BlkStartPSN, 0) == Error)
        {
            SpeedCount = SPEED_DOWN_READY_LIMIT;// speed down
            dvdErrorFlag.fSEEK_FAIL = TRUE;
            DiscReadStatus = SEEK_FAIL;
        }
        else
        {
            /*---------------------------------------------------------------
                    Seek successful and start decoding
            ----------------------------------------------------------------*/
            ENABLE_TARGET_MISS_INT();
            DiscReadStatus       = BUF_GOOD;
            CLEAR_SYNC_LOST_STATUS();
            ENABLE_DVD_DECODER_INTS();
            ENABLE_DVD_BUFFERING();
            ENABLE_BUFI_INT();
            START_DVD_BLOCK_TIMER(DVDRAM_BLOCK_TIMEOUT_VALUE);
        }

        ReadyLED(LED_OFF);

        /*-------------------------------------------------------------------
                Monitor DVD Buffering
        --------------------------------------------------------------------*/
        while(1)
        {
            if (!svoIramVar.knlTimeupCount)
            {  /* time out */
                dvdStopDiscTransfer();                  /* stop disc transferring       */
                dvdResetDiscFlags();
                CLEAR_DVD_DECODER_STATUS();                 /* clear all remaining status   */
                Svr_pause_reqire = TRUE;                /* Set pause request to servo   */
                Iram_Svr_player_mode = PAUSING_MODE;    /* Set pause mode for player    */
                DiscReadStatus = TIMEOUT;
                SwitchBackDVDRAMBuffer();
                return(DiscReadStatus);                 /* Return the disc read error   */
            }

            /*---------------------------------------------------------------
                Check for buffered block errors posted by DVD ISR
            ----------------------------------------------------------------*/
            if (DVD_BLK_ERROR)
            {
//                dvdDspSystemInit();
                WRITE_REG(HAL_DVDDREG_MAP->PIPOSRTL, DVDRAM_VERIFY_PIPO_START_BLOCK);  /* PI/PO start blk. */
                WRITE_REG(HAL_DVDDREG_MAP->STARTBKL, Iram_usiRingSP);           /* ring start blk.  */
                WRITE_REG(HAL_DVDDREG_MAP->ENDBKL, Iram_usiRingEP);             /* ring end blk.    */
                break;
            }
            /*---------------------------------------------------------------
                Check for buffering errors (non block errors)
            ----------------------------------------------------------------*/
            if (DVD_BUFFER_ERROR)
            {
                WRITE_REG(HAL_DVDDREG_MAP->PIPOSRTL, DVDRAM_VERIFY_PIPO_START_BLOCK);  /* PI/PO start blk. */
                WRITE_REG(HAL_DVDDREG_MAP->STARTBKL, Iram_usiRingSP);           /* ring start blk.  */
                WRITE_REG(HAL_DVDDREG_MAP->ENDBKL, Iram_usiRingEP);             /* ring end blk.    */
                break;
            }

            /*---------------------------------------------------------------
                Check if all requested blocks have been buffered
            ----------------------------------------------------------------*/
            if(dvdDiscRequest.EccBlocksBuffered >= 1)
                break;

            ExitProcess();
        } /* End Monitor Buffering Loop */
    } /* END RamRetryCnt Loop */

    /*---------------------------------------------------------------
       All requested blocks have been bufferred. Stop the transfer
       and reset the disc flags
    ----------------------------------------------------------------*/
    dvdStopDiscTransfer();
    dvdResetDiscFlags();

    /*---------------------------------------------------------------
       Mark a servo pause request and return disc status
    ----------------------------------------------------------------*/
    Svr_pause_reqire = TRUE;
    SwitchBackDVDRAMBuffer();
    return(DiscReadStatus);
}


/*******************************************************************************
    FUNCTION        ramVerifyID
    DESCRIPTION     Verifies the actual ID with target for DVD-RAM
    NOTE : Different reading mode has its own ID setting, we need to seperate them.
           1. read Control Data and DMA : setting is the same as general DVD.
           2. need to take care, if ReadDataMode = eREAD_DATA_FROM_DISC,
                                                   ePRE_BUFFERING,
                                                   eREAD_MODIFY_WRITE
********************************************************************************/
BYTE ramVerifyID()
{
    ULONG TargetID;
    ULONG ActualID;
    ULONG NextSecID;
    ULONG *AuxPtr;
    BYTE  *IceIndicator;
    #if (DVD_RAM_CERTIFIED_FLAG_DETECTION == 1)
    BYTE  *DramAddressP;
    #endif

    switch (RWDisc.dvRAM.ReadDataMode)
    {
        case eNOT_USERDATA:
            TargetID = dvdLastDecodedPSN + 1;
            AuxPtr = (ULONG *) (CUR_AUX_ADDRESS());
            break;

        case eREAD_DATA_FROM_DISC:
        case eREAD_ERROR_RATE:
            TargetID = dvdramLastDecodedLSN + 1;
            AuxPtr = (ULONG *) (CUR_AUX_ADDRESS());
            break;

        case ePRE_BUFFERING:
            TargetID = RWDisc.dvRAM.PreLSN;
            AuxPtr = (ULONG *) (CUR_AUX_ADDRESS());
            break;

        case eREAD_MODIFY_WRITE:
            TargetID = dvdramLastDecodedLSN + 1;
            AuxPtr = (ULONG *) (DVDRAM_VERIFY_AUX_ADDR);
            break;
    }

    #if (DVD_RAM_CERTIFIED_FLAG_DETECTION == 1)
    if(DataBuffer.CurDiscPtr == 0)
        DramAddressP = (BYTE *) (SCSDRAM_ADDRESS + DVDRAM_ERROR_CODE_END_ADDR -12);
    else
        DramAddressP = (BYTE *) (SCSDRAM_ADDRESS + DVDRAM_ERASURE_FLAGS_START_ADDR + ((DataBuffer.CurDiscPtr >> 4) * DVDRAM_ELSI_SIZE) - 13);
    send_msg5SValue(SEND_MRW_MSG,"page",4, DataBuffer.CurDiscPtr);
    send_msg5SValue(SEND_MRW_MSG,"cert flag",1, (BYTE)* DramAddressP);
    #endif

    ActualID = *AuxPtr;
    ActualID = SWAPEND_ULONG(ActualID) & 0x00FFFFFF;


    if ((ActualID < ONE_DVD_BLOCK) && (RWDisc.dvRAM.ReadDataMode != eREAD_ERROR_RATE)) //Certification Block sector 0's DataID
    {
        AuxPtr += 4; //move to sector 1's DataID
        NextSecID = SWAPEND_ULONG(*AuxPtr) & 0x00FFFFFF;
        if ((NextSecID < ONE_DVD_BLOCK) && (NextSecID == ((ActualID+0x01) & 0x0000000F))) //Double check
        {
            send_msg5SValue(SEND_MRW_MSG,"certified",4,NextSecID);
            IceIndicator = (BYTE *)(DVD_RAM_APPLICATION_DATA_ADDR + ((READ_REG(HAL_DVDDREG_MAP->CURECCL)) >> 4));  //IceIndicator is one byte revolution
            *IceIndicator |= eCERTIFIED_BLOCK; //Mark it as an CERTIFIED block
            dvdErrorFlag.fSEQ_ID_ERROR = CLEAR;
            return TRUE;
        }
        else
        {
            send_msg5SValue(SEND_MRW_MSG,"Invalid ID",4,NextSecID);
            return FALSE;
        }
    }

    if (RWDisc.dvRAM.ReadDataMode != eNOT_USERDATA)
        ActualID -= 0x31000;

    if (READ_REG(HAL_DVDDREG_MAP->POUCOR) != 0) //if PO_U, print error messages
    {
        BYTE  i;

        send_msg80(1,0x710227);
        // Show debug messages for error IDs
        send_msg5SValue(1,"TargetID",4,TargetID);
        send_msg5SValue(1,"ActualID",4,ActualID);

        send_msg5SValue(1,"PI_C",2,READ_REG(HAL_DVDDREG_MAP->PISCORL ));
        send_msg5SValue(1,"PI_UN_C",1,READ_REG(HAL_DVDDREG_MAP->PIUCOR));
        send_msg5SValue(1,"PO_C",2,READ_REG(HAL_DVDDREG_MAP->POSCORL));
        send_msg5SValue(1,"PO_UN_C",1,READ_REG(HAL_DVDDREG_MAP->POUCOR));

        for (i = 0; i < 15; i++)
        {
            AuxPtr += 4; //move to sector 1's DataID
            NextSecID = SWAPEND_ULONG(*AuxPtr) & 0x00FFFFFF;
            if (RWDisc.dvRAM.ReadDataMode != eNOT_USERDATA)
                NextSecID -= 0x31000;
            send_msg5SValue(1,"ShowSecID",4,NextSecID);
        }
    }

    if(RWDisc.dvRAM.ReadDataMode == eREAD_ERROR_RATE)
        return TRUE;

    if (TargetID == ActualID)
        return TRUE;
    else
        return FALSE;
}


void InitializeIceIndicator(void)
{
    BYTE *BufferPtr;
    BYTE i;

    BufferPtr = (BYTE *)(DVD_RAM_APPLICATION_DATA_ADDR);

    for (i=0; i< eRAMCacheSegmentCnt; i++)
        *BufferPtr++ = 0;
}

    #if DvdRamFastCache
/*******************************************************************************

    FUNCTION        DVDRAMSearchCache

    DESCRIPTION     Searches the DVDRAM cache to determine if the requested host
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

    RETURNS         CACHE_MISS or FULL_CACHE_HIT / no PARTIAL_CACHE_HIT

********************************************************************************/
SearchCacheResultE DVDRAMSearchCache(TransferS Request, BYTE mode)
{
    RamCacheListT       *pCacheHitNode;
    BYTE *  IceIndicator;
    ULONG   tx_lsn;
    USHORT c_num, BlocksToBeBuffered, k;
    USHORT NextContPage;
    USHORT  tx_page, tx_len;
    BYTE    tx_seg, tx_segtype;
    BYTE    InterruptStatus;
    BYTE    i, cnt, result;

    RWDisc.dvRAM.FastCacheXferStPage = 0xffff;
    RWDisc.dvRAM.FastCacheXferLen = 0xffff;

    /* check prebuffering ON */
    /* RWDisc.dvRAM.Prebuffering = FALSE in DVDRAMAppendPreBuffer() read lock, must check another codition */
    if (RWDisc.dvRAM.Prebuffering == FALSE && RWDisc.dvRAM.ContinuteReadCount < 3)
    {
        send_msg54(SEND_MRW_MSG, 'm', '1', RWDisc.dvRAM.Prebuffering, RWDisc.dvRAM.ContinuteReadCount);
        return (CACHE_MISS);
    }

    /*--------------------------------------------------
        prebuffering should be ON, RWDisc.dvRAM.PreLSN = next to be decoded address
    --------------------------------------------------*/
    if(RWDisc.dvRAM.PreLSN <=  (Request.StartBlock.lba + Request.BlockCount.Long - 1))
    {
        send_msg80(SEND_MRW_MSG,0x710228);
        return (CACHE_MISS);
    }

    //EndLSN = (Request.StartBlock.lba + Request.BlockCount.Long + 0x0F) & ~0x0FL;  /* align to ECC start */
    //CacheUsage = DataBuffer.CacheEndLBA - EndLSN + 1;

    /*--------------------------------------------------
        No data in the buffer
    --------------------------------------------------*/
    //CacheUsage = 0 means after fast cache there is no data inside
    if (Request.StartBlock.lba > DataBuffer.CacheEndLBA)
    {
        send_msg80(SEND_MRW_MSG,0x710229);
        return (CACHE_MISS);
    }

    /*--------------------------------------------------
        Search link list in cache
    --------------------------------------------------*/
    InterruptStatus =   GET_INT_ENABLE_MASK();
    DISABLE_INTERRUPT();

    /*-------------------------------------------------------------------
       Cache is static if read ahead = false (no buffering is occuring)
       If cache is static determine if the starting LBA is currently in
       the buffer. If not return a cache miss status. For fast cache
       transfers check regardless if cache is static or not.
    --------------------------------------------------------------------*/
    cnt = RAM_GetDefectInfo(Request.StartBlock.lba, Request.BlockCount.Long);
    //send_msg5SValue(SEND_MRW_MSG,"CNT = ",1,cnt);

    for ( i = 0; i < cnt; i++)
    {
        result = DVDRAMCheckPreBuffering(RWDisc.dvRAM.REntry[i].LSN, RWDisc.dvRAM.REntry[i].Len, 1);
        send_msg5SValue(SEND_MRW_MSG,"CkPBuf", 1, result);

        c_num = RAMcmFindTargetLSN(RWDisc.dvRAM.REntry[i].LSN, RWDisc.dvRAM.REntry[i].Len, (RWDisc.dvRAM.REntry[i].Attr & eSA_MASK) | eReadAccess);


        BlocksToBeBuffered = c_num;
        //send_msg5SValue(SEND_MRW_MSG,"c_num = ", 1, c_num);

        for (k = 0; k< c_num; k++)  /* transfer one by one cache node in for loop */
        {
            pCacheHitNode = RAMcmGetCacheHitNode(k);

            tx_page = pCacheHitNode->StartDramPgNo; /* real start page inside a complete ECC block */
            tx_lsn = pCacheHitNode->StartLSN;
            tx_len = pCacheHitNode->Length;
            tx_seg = pCacheHitNode->SegmentNo;
            tx_segtype = DVDRAM_GET_SEG_TYPE(tx_seg);

            //send_msg5SValue(SEND_MRW_MSG,"k = ", 1, k);
            //send_msg5SValue(SEND_MRW_MSG,"tx_page", 2, tx_page);
            //send_msg5SValue(SEND_MRW_MSG,"tx_lsn", 4, tx_lsn);
            //send_msg5SValue(SEND_MRW_MSG,"tx_len", 2, tx_len);
            //send_msg5SValue(SEND_MRW_MSG,"tx_seg", 1, tx_seg);
            //send_msg5SValue(SEND_MRW_MSG,"tx_segtype", 1 ,tx_segtype);
            //send_msg54(SEND_MRW_MSG, 'C', 'H', 't', pCacheHitNode->HitStatus);

            if (tx_segtype == eWriteSeg || (pCacheHitNode->HitStatus == eMissed))
            {
                SET_INT_ENABLE_MASK( InterruptStatus);
                send_msg80(SEND_MRW_MSG,0x71022A);
                return (CACHE_MISS);
            }

            if(i == 0 && k == 0)
            {
                RWDisc.dvRAM.FastCacheXferSgNo = tx_seg;
                RWDisc.dvRAM.FastCacheXferStPage = pCacheHitNode->StartDramPgNo;
                RWDisc.dvRAM.FastCacheXferLen = pCacheHitNode->Length;
            }
            else
            {
                NextContPage = (RWDisc.dvRAM.FastCacheXferStPage + RWDisc.dvRAM.FastCacheXferLen) % DVDRAM_BUF_TOTAL_BLK_COUNT;
                if(NextContPage == tx_page)
                    RWDisc.dvRAM.FastCacheXferLen += tx_len;
                else
                {
                    send_msg5SValue(SEND_MRW_MSG,"NoContPg", 4, RWDisc.dvRAM.FastCacheXferLen<<16 | tx_page);
                    SET_INT_ENABLE_MASK( InterruptStatus);
                    send_msg80(SEND_MRW_MSG,0x71,0x0,0x2B);
                    return (CACHE_MISS);
                }
            }

            /* The requested blocks hit in the cache */
            IceIndicator = (BYTE *)(DVD_RAM_APPLICATION_DATA_ADDR + (tx_page >> 4));  //IceIndicator is one byte revolution

            //Zero fill use variable that was set in ISR, FW can't call ZeroFillDRAM() here
            if ((*IceIndicator & eICE_BLOCK) || (*IceIndicator & eCERTIFIED_BLOCK))
            {
                send_msg54(SEND_MRW_MSG, 'Z', 'R', 'O', '6');
                return (CACHE_MISS);
            }
        }   /* for (k = 0; k< c_num; k++) */
    }   /* for ( i = 0; i < cnt; i++)  */

    RWDisc.dvRAM.Rnxinfo.NextLSN = Request.StartBlock.lba;
    RWDisc.dvRAM.Rnxinfo.NextLen = Request.BlockCount.Long;

    SET_INT_ENABLE_MASK( InterruptStatus);

    return ( FULL_CACHE_HIT );
}


/*******************************************************************************

    FUNCTION        DVDRAMFullCacheHit

    DESCRIPTION     Process a host transfer when all host data is already in
                    the buffer

    RETURNS         BUF_GOOD        Indicates buffer is good

********************************************************************************/
DiscAccessStatusE DVDRAMFullCacheHit()
{
    USHORT page;

    send_msg5SValue(SEND_MRW_MSG,"RamFHit", 4, RWDisc.dvRAM.FastCacheXferStPage<<16 | RWDisc.dvRAM.FastCacheXferLen);

    if (DVDRAMTransferHostData(RWDisc.dvRAM.FastCacheXferStPage, RWDisc.dvRAM.FastCacheXferLen, 1) == 0) //Transfer OK.
    {
        //update BufPgCMP page
        page = READ_REG(HAL_HSTREG_MAP->BUFPGCMPL);
        WRITE_REG(HAL_HSTREG_MAP->BUFPGCMPL, ((page + RWDisc.dvRAM.FastCacheXferLen) % (Iram_usiRingEP+1)));
        //send_msg5SValue(SEND_MRW_MSG,"page",2,page);
        //send_msg5SValue(SEND_MRW_MSG,"tx_len",2,tx_len);
    }

    DVDRAMReleaseReadLock();

    RWDisc.dvRAM.CommandExe &= ~eReadCmd;

    return( BUF_GOOD );
}

/*******************************************************************************

    FUNCTION        DVDRAMReleaseReadLock

    DESCRIPTION     Call this function after data has transferred to release
                    eReadLocked segment into eReadSeg.

    RETURNS         None

********************************************************************************/
void DVDRAMReleaseReadLock(void)
{
    USHORT sg, SgCnt, FreeSg;
    BYTE    InterruptStatus;

    InterruptStatus =   GET_INT_ENABLE_MASK();
    DISABLE_INTERRUPT();

    SgCnt = ((RWDisc.dvRAM.FastCacheXferStPage + RWDisc.dvRAM.FastCacheXferLen - 1) >> 4) -
        (RWDisc.dvRAM.FastCacheXferStPage >> 4) + 1;

    //send_msg5SValue(SEND_MRW_MSG, "Reles", 4, RWDisc.dvRAM.FastCacheXferStPage<<16 | SgCnt <<8 | (BYTE) RWDisc.dvRAM.FastCacheXferLen);

    for(sg = 0; sg < SgCnt; sg++)
    {
        FreeSg = RWDisc.dvRAM.FastCacheXferSgNo + sg;
        if(FreeSg >= eRAMCacheSegmentCnt)
            FreeSg %= eRAMCacheSegmentCnt;

        //send_msg54(SEND_MRW_MSG, 'f', 'e', DVDRAM_GET_SEG_TYPE(FreeSg), FreeSg);

        if(DVDRAM_GET_SEG_TYPE(FreeSg) == eReadLocked)
        {
            DVDRAM_SET_SEG_TYPE(FreeSg, eReadSeg);
        }
    }

    SET_INT_ENABLE_MASK( InterruptStatus);

}
    #endif  // #if DvdRamFastCache

    #if (ENABLE_RAM_DFE_ICE_DETECT == 1)
/*******************************************************************************
* Name: DVDRAMIceDetection
* Description : Detect ICE setcor in DFE interrupt.
* Inputs:       none
* Outputs:      none
*******************************************************************************/
void DVDRAMIceDetection(void)
{
    BYTE RAMICEStat, Zone, HFamp;
    HFamp = READ_FIELD(HAL_DFE_ICEHFAMP);
    RAMICEStat = (BYTE)READ_FIELD(HAL_DFE_CLUSST0);
    if ((RAMICEStat == 0x02) && (HFamp < 0x04))
    {
        HAL_DFE_ClusterStateDisableInt();
        Zone = DVDRAM_PSN2ZONE(RWDisc.dvRAM.CurrentPID);
        if ((RWDisc.dvRAM.CurrentPID >= (DVDRAMZoneTable[Zone].EndUserSectorPSN + 1)) //we are reaching to Guard Area.
            || (RWDisc.dvRAM.CurrentPID < (DVDRAMZoneTable[Zone].StartUserSectorPSN)))
        {
            send_msg5SValue(SEND_DVDRAM_ICE,"GuardArea",4,RWDisc.dvRAM.CurrentPID);
            return;
        }

        if (READ_FIELD(HAL_DFE_ICESTRAD) < READ_FIELD(HAL_DFE_ICEDSTRAD))
        {
            send_msg5SValue(SEND_DVDRAM_ICE,"InvalidAdd",4,READ_FIELD(HAL_DFE_ICESTRAD));
            return;
        }

        CurICEAdd = READ_FIELD(HAL_DFE_ICESTRAD) & DVD_BLOCK_MASK;
        #if 0//for debug
        send_msg5SValue(SEND_DVDRAM_ICE,"CurrentPID",4,RWDisc.dvRAM.CurrentPID);
        send_msg5SValue(SEND_DVDRAM_ICE,"ECCISTA1",4,READ_REG(HAL_DFEREG_MAP->ICESTA1));
        send_msg5SValue(SEND_DVDRAM_ICE,"ECCISTA2",4,READ_REG(HAL_DFEREG_MAP->ICESTA2));
        send_msg5SValue(SEND_DVDRAM_ICE, "CurDsPtr", 2, DataBuffer.CurDiscPtr);
        #endif
        SetDVDRAMIceIndicator();
    }
}

/*******************************************************************************
* Name: SetDVDRAMIceIndicator
* Description : DVDRAM Ice Indicator is setting.
* Inputs:       none
* Outputs:      none
*******************************************************************************/
void SetDVDRAMIceIndicator(void)
{
    BYTE   *IceIndicator, offset;
    USHORT  pg_no;
    WORD    Ptr,Ptr1,Ptr2;

    IceFound = TRUE;
    offset = ((CurICEAdd - dvdramDecodedStrPSN) & 0xf0) >> 4;    //block offset
    pg_no = (DataBuffer.CurDiscPtr >> 4);

    Ptr1 = (DataBuffer.CurHostPtr >> 4);
    Ptr2 = pg_no + offset;

    if(Ptr2 >= MAX_DVD_BLOCKS)
        Ptr2 = Ptr2 % MAX_DVD_BLOCKS;

    send_msg5SValue(SEND_DVDRAM_ICE,"CurHostPtr",4,Ptr1);
    send_msg5SValue(SEND_DVDRAM_ICE,"CurIcePtr",4,Ptr2);

    IceIndicator = (BYTE *) (DVD_RAM_APPLICATION_DATA_ADDR + Ptr2);  //IceIndicator is one byte revolution

    *IceIndicator |= eICE_BLOCK; //Mark it as an ICE block

         #if (DISCARD_RAM_DECODE == 1)
    RAMICEAreaDecoder();
         #endif

    /* clear next indicator */
    if((ULONG) IceIndicator == (DVD_RAM_APPLICATION_DATA_ADDR + MAX_DVD_BLOCKS - 1))
        IceIndicator = (BYTE *) (DVD_RAM_APPLICATION_DATA_ADDR);
    else
        IceIndicator++;

     *IceIndicator = eNON_ICE_BLOCK;

    if(Ptr1 > Ptr2)
    {
        Ptr = Ptr1 - Ptr2;

        if(Ptr == 1)
        {
            send_msg54(SEND_MRW_MSG, 'O','V','R','U');
            dvdStopDiscTransfer();
        }
    }
}

/*******************************************************************************
* Name: ClearDVDRAMIceIndicatorAll
* Description : DVDRAM Ice Indicator is clearing.
* Inputs:       none
* Outputs:      none
*******************************************************************************/
void ClearDVDRAMIceIndicatorAll(void)
{
    BYTE *IceIndicator;
    BYTE i;

    IceFound = FALSE;
    send_msg5SValue(SEND_DVDRAM_ICE, "ClearIndAll", 1, 0);
    IceIndicator = (BYTE *)(DVD_RAM_APPLICATION_DATA_ADDR);
    for (i = 0; i < MAX_DVD_BLOCKS; i++)
        *IceIndicator++ = 0;
}

        #if (DISCARD_RAM_DECODE == 1)
/*********************************************************************
* Function:     RAMICEAreaDecoder()
*
* Description:  Function is executed when RAM decode Discard enable,
*               this function instead of .
*
* Returns:      none
* Params:       none
*********************************************************************/
void RAMICEAreaDecoder(void)
{
    BYTE    InterruptState;

    if (dvdDecFlag.fREADING_LEADIN == FALSE)
    {
        if(RWDisc.dvRAM.ContinuteReadCount >= 3)
        {
            send_msg5SValue(SEND_DVDRAM_ICE, "CurICEAdd", 4, CurICEAdd);
            send_msg5SValue(SEND_DVDRAM_ICE, "PreICEAdd", 4, PreICEAdd);

            if(CurICEAdd == (PreICEAdd + ONE_DVD_BLOCK))
            {
                if (RWDisc.dvRAM.ContinuteIceCount == 0xFF)
                    RWDisc.dvRAM.ContinuteIceCount = DIS_DVDDEC_ICE_CONT + 1;     //ring back from 4
                else
                    RWDisc.dvRAM.ContinuteIceCount++;
            }
            else
            {
                //RWDisc.dvRAM.ContinuteIceCount = 0;
            }

            PreICEAdd = READ_FIELD(HAL_DFE_ICESTRAD) & DVD_BLOCK_MASK;
        }

        if(RWDisc.dvRAM.ContinuteIceCount >= DIS_DVDDEC_ICE_CONT && RWDisc.dvRAM.ReadDataMode == ePRE_BUFFERING)
        {
            if((READ_REG(HAL_DVDDREG_MAP->DVDIRQC1) == 0) && (READ_REG(HAL_DVDDREG_MAP->DVDIRQC2) == 0))
            {
                DataBuffer.CurDiscPtr += ONE_DVD_BLOCK;     /* Advance the disc pointer */
                DataBuffer.CurDiscPtr %= MAX_DVD_SECTORS;   /* Insure buffer wrap */

                /*---------------------------------------------------------------
                Reset DVD Block Timer
                ---------------------------------------------------------------*/
                START_DVD_BLOCK_TIMER (DVDRAM_BLOCK_TIMEOUT_VALUE);
                /*---------------------------------------------------------------
                    Update count of blocks transferred, buffer count and
                    last PSN in buffer.
                ---------------------------------------------------------------*/
                ++dvdDiscRequest.EccBlocksBuffered;
                DataBuffer.SectorCount  += ONE_DVD_BLOCK;
                dvdLastDecodedPSN       += ONE_DVD_BLOCK;
                dvdramLastDecodedLSN    += ONE_DVD_BLOCK;
                dvdramDecodedStrPSN     += ONE_DVD_BLOCK;

                if (RWDisc.dvRAM.ReadDataMode == ePRE_BUFFERING)
                {
                    RWDisc.dvRAM.StartBufferingOK = TRUE;
                    /* Register new decoded Segment for DVD-RAM pre-buffering */
                    DVDRAMAppendPreBuffer();
                }


                /* In DVDRAM_NEW_CACHE, we don't use dvdDiscRequest info, and Never SendHostBlock in ISR. */
                /* All we need to do is update CacheEndLBA */
                if (DataBuffer.CacheEndLBA == 0L)
                    DataBuffer.CacheEndLBA = 0x0FL;
                else
                    DataBuffer.CacheEndLBA += ONE_DVD_BLOCK;
                send_msg80(SEND_DVDRAM_ICE,0x71022C);

            }

            if(READ_REG(HAL_DVDDREG_MAP->DVDIRQC1) & HAL_DVDD_ECCEN)
            {
                //disable DVD decorder ECCI and BUFI
                InterruptState = GET_INT_ENABLE_MASK();
                DISABLE_INTERRUPT();
                WRITE_REG(HAL_DVDDREG_MAP->DVDIRQC1, 0x00);
                WRITE_REG(HAL_DVDDREG_MAP->DVDIRQC2, 0x00);

                SET_INT_ENABLE_MASK(InterruptState);
                send_msg5S(SEND_DVDRAM_ICE, "bypas_dvdD");
            }
        }
    }
}
        #endif  // #if (DISCARD_RAM_DECODE == 1)
    #endif // #if (ENABLE_RAM_DFE_ICE_DETECT == 1)
#endif /* #if (DVD_RAM_READ == 1) */

