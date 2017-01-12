/****************************************************************************
*                (c) Copyright 2001 - 2007 SUNEXT DESIGN, INC.
*                           All Right Reserved
*
* FILENAME:   bdWriteTableRead.c
*
* DESCRIPTION
*
*
*   $Revision: 95 $
*   $Date: 11/03/28 2:36p $
*
**************************** SUNEXT CONFIDENTIAL *******************************/


/*********************************************************************
 INCLUDES
*********************************************************************/
#include ".\common\globtype.h"
#include ".\common\globals.h"
#include ".\common\chip.h"
#include ".\common\system.h"
#include ".\hif\powerctl.h"
#include ".\hif\gp_mac.h"
#include ".\hif\at_equ.h"
#include ".\bd\BdUtil.h"
#include ".\bd\bdVars.h"
#include ".\bd\bdDef.h"
#include ".\bd\bdDiscRd.h"
#include ".\bd\bdMacro.h"
#include ".\al\reg_hal.h"
#include ".\al\reg_intctrl.h"
#include ".\common\intctl.h"
#include ".\al\REG_wobble.h"


#if (CALIBRATION_MANAGEMENT == 1)
#include ".\servo\svo_com.h"
#include ".\servo\svo_calib.h"
#endif
#if (BD_ENABLE)
#include ".\defmgmt\bdwritable\BdWritableRead.h"
#include ".\defmgmt\bdwritable\BdDfmg.h"
#include ".\defmgmt\bdwritable\BdWritableCache.h"
#include ".\defmgmt\bdwritable\bdRanAccDef.h"
/*********************************************************************
 LOCAL FUNCTION PROTOTYPES
*********************************************************************/
void BdWritableRead(void);
BYTE BdGetDefectInfo(ULONG StartLba, ULONG Length);
void BdwtReadExit(void);
#if BDWT_FAST_CACHE
BYTE BDTransferHostData(USHORT tx_page, USHORT tx_len, BOOL FastCache);
#else
BYTE BDTransferHostData(USHORT tx_page, USHORT tx_len);
#endif
DiscAccessStatusE BDReadDataFromDisc(USHORT start_page,ULONG read_start_psn,ULONG blk_start_lsn,BYTE length, BYTE mode);
BYTE BDPreReadBufferingForCpuUsage(void);
/*********************************************************************
 GLOBAL VARIABLES
*********************************************************************/
extern BYTE gFirstClusterDone;
extern vBOOL SDHoldSts;
extern vBOOL FDHoldSts;


/*********************************************************************
 FUNCTIONS
*********************************************************************/

/*----------------------------------------------------------------------------
  Name: BdWritableRead10Command
  Description:
        Read_10/Read_12 Command Handler for BD-R/RE disc

  Input:  None
  Output: None
----------------------------------------------------------------------------*/
void BdWritableRead10Command(void)
{

    /*-------------------------------------------------------------------
        Initialize module variable HostXfer
    --------------------------------------------------------------------*/
    HostXfer.StartBlock.lba    = 0L;
    HostXfer.BlockCount.Long   = 0L;

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
        BdwtReadExit();
        return;
    }

    /*---------------------------------------------------------------
        Set to maximum speed
    ----------------------------------------------------------------*/
    if(SpeedMD == Set_sp_f)
    {
        SpeedMD = Special_f;
    }
#if(CEProduct == 0)
    // bypass speed up/down when CDSpeed test.
    if((SpeedMD == AutoMode) && (HostXfer.BlockCount.Long == 0x19))
    {
        SpeedMD = Special_for_CDSpeed_f;
    }
    else if((SpeedMD == Special_for_CDSpeed_f)&&(HostXfer.BlockCount.Long != 0x19))
    {
        RandomRead = TRUE; //restart the automode.
        SpeedMD = AutoMode;
    }
#endif

    BdWritableRead();
    return;
}

extern BYTE CmdSeekOk;
/*********************************************************************
FUNCTION    : BdWritableRead
DESCRIPTION : The main read flow for BD Writable discs.
              For BD-ROM and Non_User_Ddata (ex: Leadin zone), we use
                  sequential reading;
              For BD-R/RE, random reading is applied due to DefectMgr.
              We need to process host's request cluster by cluster to
                  get the proper replacement information.
GLOBALS     : Host request start address
              Host request length
RETURNS     : None
*********************************************************************/
void BdWritableRead(void)
{
    BdCacheListT    *pCacheHitNode;
    ULONG   bdEndLBA;
    ULONG   tx_psn;
    USHORT  tx_page, tx_len, temp_i;
    BYTE    Fua, c_num, k;
    BYTE    tx_seg, tx_segtype, j, FreeSg;
    DiscAccessStatusE   DiscReadStatus;
#if (ENABLE_DFE_ICE_DETECT == 1)
    BYTE   *IceIndictor;
#endif
    BYTE   BlocksToBeBuffered;

    /*-------------------------------------------------------------------
        Check if read is within a valid range
    --------------------------------------------------------------------*/
    // Calculate ending LBA of the host transfer
    bdEndLBA = HostXfer.StartBlock.lba + HostXfer.BlockCount.Long - 1L;
    if (bdEndLBA > pdbGetBDMaxLBA())
    {
        BUILD_SENSE(0x05,0x21,0x00,0xb3);
        BdwtReadExit();
        return;
    }

    BdwtDisc.CommandExe |= eBdReadCmd;
    {
        BdwtDisc.PreReadingOK = TRUE;
    }

    //Initialize Variables
    Fua = 0;        /* Force Unit Access */
    BdwtDisc.Stream = eBdNoStream;

    if (Iram_Cmd_block.bf[0] == SBC_READ_12)
    {
        if (Iram_Cmd_block.bf[1] & 1)   /* RelAdr == 1 */
        {
            BUILD_SENSE(0x05,0x24,0x00,0x9c);            /* INVALID FIELD IN CDB */
            BdwtReadExit();
            return;
        }

        if (Iram_Cmd_block.bf[10] & 0x80)   /* Streaming bit == 1*/
        {
            if (Iram_Cmd_block.bf[1] & 0x8) /* Fua == 1 */
            {
                BUILD_SENSE(0x05,0x24,0x00,0x9d);            /* INVALID FIELD IN CDB */
                BdwtReadExit();
                return;
            }
            BdwtDisc.Stream |= eBdReadStream;
        }
        //When Streaming Read is switch from ON to OFF,
        //clear the data in the buffer.
        else
        {
            if (Iram_Cmd_block.bf[1] & 0x08)
                Fua = 1;

            if (BdwtDisc.Stream & eBdReadStream)
            {
                if( BD_BUFFERING() == ON )
                    bdStopDiscTransfer();

                ClearDataBuffer();
            }

            BdwtDisc.Stream = eBdNoStream;
        }
    }
    else
    {
        //SBC_READ_10
        if (Iram_Cmd_block.bf[1] & 0x08)
            Fua = 1;

        //When Streaming Read is switch from ON to OFF,
        //clear the data in the buffer.
        if (BdwtDisc.Stream & eBdReadStream)
        {
            if( BD_BUFFERING() == ON )
                bdStopDiscTransfer();

            ClearDataBuffer();
        }

        BdwtDisc.Stream = eBdNoStream;
    }

    CmdSeekOk = 0;

    if ((BD_BUFFERING() == ON) && (Fua == 0))    /* Decoder is buffering */
    {
        bdEndLBA = ((HostXfer.StartBlock.lba + HostXfer.BlockCount.Long + 0x1F) & ~0x1FL);  /* align to Cluster start */
        if ((DataBuffer.CacheEndLBA < bdEndLBA) && (bdEndLBA < (DataBuffer.CacheEndLBA + HostXfer.BlockCount.Long + 0x32)))
        {   // + 0x32 is used for a margin
            //send_msg5SValue(SEND_MRW_MSG,"EndLBA",4,DataBuffer.CacheEndLBA);

            //Extend Read Loadbuffer
            HAL_BD_CODEC_ENABLE_DECODER();                   /* Turn on BD decoder */
            HAL_BD_CODEC_STREAM_PASS_ALL_OFF();             /* Disable Stream Pass All function */

            StartTimer(&tx_psn);

            while (1)
            {
                if ((DataBuffer.CacheEndLBA + 1) >= bdEndLBA)
                {
                    send_msg5S(SEND_MRW_MSG,"Block Cached!");
                    break;
                }

                if ( BD_CLUSTER_ERROR || BD_BUFFER_ERROR )
                {
                    send_msg5S(SEND_MRW_MSG,"ECC-NG");
                    break;
                }

                if (BD_BUFFERING() == OFF) /* decoder stopped */
                {
                    send_msg5S(SEND_MRW_MSG,"Decoder Stop");
                    break;
                }

                if (ReadTimer(&tx_psn) > 1000L)
                {
                    send_msg5S(SEND_MRW_MSG, "1s timeout");
                    break;
                }
                ExitProcess();
            }
            send_msg5SValue(SEND_MRW_MSG,"EndLBA",4,DataBuffer.CacheEndLBA);
        }
    }

    DISABLE_INTERRUPT();
    if(BdwtDisc.Stream & eBdReadStream)
        c_num = BDcmFindTargetPSN(HostXfer.StartBlock.lba, HostXfer.BlockCount.Long, eBD_STREAM_READ);
    else
    {
        c_num = BDcmFindTargetPSN(HostXfer.StartBlock.lba, HostXfer.BlockCount.Long, eBD_READ);

        send_msg5SValue(SEND_MRW_MSG,"c_num1 = ",1,c_num);

        if(c_num & 0x80) //there is a defeat cluster
        {
            c_num = c_num & 0x7F;
            if(BD_BUFFERING() == ON)
            {
                for (k = 0; k< c_num; k++)
                {
                    pCacheHitNode = BDcmGetCacheHitNode(k);

                    if(pCacheHitNode->HitStatus == eHit)
                    {
                        tx_seg = pCacheHitNode->SegmentNo;
                        tx_page = BD_GET_SEG_PAGE(tx_seg);

                        if(tx_page >= BdwtDisc.PrePage)
                            temp_i = tx_page - BdwtDisc.PrePage;
                        else
                            temp_i = tx_page + eBDTotalPageNo - BdwtDisc.PrePage;

                        if(temp_i <= 0x40)
                        {
                            bdStopDiscTransfer();

                            for (j = 0; j < c_num; j++)
                            {
                                pCacheHitNode = BDcmGetCacheHitNode(j);
                                pCacheHitNode->HitStatus = eMissed;
                            }

                            //send_msg5SValue(SEND_MRW_MSG,"Pre_page2",2,BdwtDisc.PrePage);
                            //send_msg5SValue(SEND_MRW_MSG,"HitStatus2",1,k);
                            break;
                        } //if(temp_i <= 0x40)
                    } //if(pCacheHitNode->HitStatus == eHit)
                } //for (k = 0; k< c_num; k++)
            } //if(BD_BUFFERING() == ON)
        } //if(c_num & 0x80)
    }
    ENABLE_INTERRUPT();

    //send_msg5SValue(SEND_MRW_MSG,"c_num = ",1,c_num);

    for (k = 0; k< c_num; k++)  /* transfer one by one cache node in for loop */
    {
        pCacheHitNode = BDcmGetCacheHitNode(k);

        tx_page = pCacheHitNode->StartDramPgNo; /* real start page inside a complete ECC block */
        tx_psn = pCacheHitNode->StartPSN;
        tx_len = pCacheHitNode->Length;
        tx_seg = pCacheHitNode->SegmentNo;
        tx_segtype = BD_GET_SEG_TYPE(tx_seg);

        BlocksToBeBuffered = (((tx_page & 0x1F) + tx_len - 1)>>5) + 1;

        //send_msg5SValue(SEND_MRW_MSG,"k = ",1,k);
        //send_msg5SValue(SEND_MRW_MSG,"tx_page",2,tx_page);
        send_msg5SValue(SEND_MRW_MSG,"tx_psn",4,tx_psn);
        send_msg5SValue(SEND_MRW_MSG,"tx_len",2,tx_len);
        send_msg5SValue(SEND_MRW_MSG,"tx_seg",1,tx_seg);
        send_msg5SValue(SEND_MRW_MSG,"tx_segtype",1,tx_segtype);
        send_msg5SValue(SEND_MRW_MSG,"HitStatus",1,pCacheHitNode->HitStatus);

        if ((Fua == 1) || (pCacheHitNode->HitStatus == eMissed))
        {
#if (EN_POWER_CONTROL == 1)
            SetDiscReadPowerMode();
#endif
            BDStopPrebuffering();

            if ( (tx_segtype == eWriteSeg))// || (tx_segtype == eWriteLocked) )
            {
                send_msg5S(SEND_MRW_MSG,"Write Seg");
            }
            else    /* ReadSeg, ReadLocked, EmptySeg */
            {
                /* start buffing */
                DataBuffer.SectorCount = 0;
                if(BD_GET_SEG_ATTRIBUTE(tx_seg) == eReplacedCluster)
                {
                    DataBuffer.CacheStartLBA = HostXfer.StartBlock.lba & BD_CLUSTER_MASK;
                }
                else
                {
                    DataBuffer.CacheStartLBA = bdConvertPSNtoLBA(tx_psn & BD_CLUSTER_MASK);
                }
                if(DataBuffer.CacheStartLBA == 0)
                    DataBuffer.CacheEndLBA = 0;
                else
                    DataBuffer.CacheEndLBA = DataBuffer.CacheStartLBA - 1;

                DiscReadStatus = BDReadDataFromDisc( BD_GET_SEG_PAGE(tx_seg),BD_GET_SEG_ECCPSN(tx_seg),
                                 (ULONG)BD_GET_SEG_ECCLSN(tx_seg), BlocksToBeBuffered, eREAD_DATA_FROM_DISC);

                if (DiscReadStatus == BUF_GOOD)
                {
                    PSNs tempPSN;
                    BdwtDisc.PreLSN = DataBuffer.CacheEndLBA + 1;
                    tempPSN = bdConvertLBAtoPSN(BdwtDisc.PreLSN);
                    BdwtDisc.PrePSN = tempPSN.PSN;
                    if (BdwtDisc.PreLSN == (pdbGetBDEndLBALayer0()+1))
                    {
                        send_msg5S(SEND_MRW_MSG,"Layer0 End");
                        bdStopDiscTransfer();
                        Svr_pause_reqire = TRUE;
                        send_msg5S(SEND_MRW_MSG,"PrebufStop-6");
                    }
                    else if ((pdbGetBDLayers() == BD_DUAL_LAYER)&&(BdwtDisc.PreLSN == (pdbGetBDMaxLBA()+1)))
                    {
                        send_msg5S(SEND_MRW_MSG,"Layer1 End");
                        bdStopDiscTransfer();
                        Svr_pause_reqire = TRUE;
                        send_msg5S(SEND_MRW_MSG,"PrebufStop-7");
                    }
                    else
                    {
                        BdwtDisc.NextReadSegNo = tx_seg + BlocksToBeBuffered;
                        if (BdwtDisc.NextReadSegNo >= eBDCacheSegmentCnt)
                            BdwtDisc.NextReadSegNo -= eBDCacheSegmentCnt;

                        BdwtDisc.PreSegNo = BdwtDisc.NextReadSegNo;
                        BdwtDisc.PrePage = BD_GET_SEG_PAGE(BdwtDisc.PreSegNo);

                        BdwtDisc.ReadDataMode = ePRE_BUFFERING;

                        send_msg5SValue(SEND_MRW_MSG,"Pre_page",2,BdwtDisc.PrePage);
                        send_msg5SValue(SEND_MRW_MSG,"Pre_lsn",4,BdwtDisc.PreLSN);
                        send_msg5SValue(SEND_MRW_MSG,"Pre_PSN",4,BdwtDisc.PrePSN);
                        send_msg5SValue(SEND_MRW_MSG,"Pre_seg",1,BdwtDisc.PreSegNo);
                    }
                }
                else
                {
                    SendMsg80(SEND_MRW_MSG,0x7100EF);
#if (XDFCT_ENABLE == 1)
                        bdStopSpecialDiscHandle();
#endif
                    BDInitCache(); //Clear Cache
                    bdSetSenseCode(DiscReadStatus);
                    BdwtReadExit();
                    return;
                }
            }
        }   /* Endif (pCacheHitNode->HitStatus == eMissed) */

        /* The requested blocks hit in the cache */
#if (ENABLE_DFE_ICE_DETECT == 1)
        IceIndictor = (BYTE *)(BD_ICE_INDICATOR_ADDR + (tx_page >> 5));  //IceIndictor is one byte revolution

        if (*IceIndictor & eICE_BLOCK) //Zero fill ICE block before sending data to Host
        {
                send_msg80(1,0x7100F0);
            ZeroFillDRAM(tx_page&(USHORT)BD_CLUSTER_MASK, ONE_BD_CLUSTER);
        #if BDWT_FAST_CACHE
            BdwtDisc.ContinueIcedCnt += tx_len;
            if (BdwtDisc.ContinueIcedCnt >= eBDTotalPageNo)
                BdwtDisc.ContinueIcedCnt = eBDTotalPageNo;
        #endif
        }
        #if BDWT_FAST_CACHE
        else
            BdwtDisc.ContinueIcedCnt = 0;
        #endif
#endif  /* ENABLE_DFE_ICE_DETECT == 1 */

        if(BD_GET_SEG_ATTRIBUTE(tx_seg) == eReplacedCluster)
            bdEndLBA = (HostXfer.StartBlock.lba -  DataBuffer.CacheStartLBA);
        else
            bdEndLBA = (bdConvertPSNtoLBA(tx_psn) -  DataBuffer.CacheStartLBA);

        DataBuffer.CacheStartLBA += bdEndLBA;
        DataBuffer.SectorCount -= bdEndLBA;

#if BDWT_FAST_CACHE
        if (BDTransferHostData(tx_page, tx_len, 0) == 0) //Transfer OK.
#else
        if (BDTransferHostData(tx_page, tx_len) == 0) //Transfer OK.
#endif
        {
            //update BufPgCMP page
            temp_i = READ_REG(HAL_HSTREG_MAP->BUFPGCMPL);
            WRITE_REG(HAL_HSTREG_MAP->BUFPGCMPL, ((temp_i + tx_len) % (Iram_usiRingEP+1)));

            for (j = 0; j < BlocksToBeBuffered; j++)
            {
                FreeSg = j + tx_seg;
                if(FreeSg >= eBDCacheSegmentCnt)
                    FreeSg -= eBDCacheSegmentCnt;

                BD_SET_SEGMENT_FULL_FILLED(FreeSg);

                BD_SET_SEG_TYPE(FreeSg, eReadSeg);
            }

            if((BD_GET_SEG_ATTRIBUTE(tx_seg) == eReplacedCluster) && (pCacheHitNode->HitStatus == eMissed))
            {
                StartTimer(&bdEndLBA);
                while((DataBuffer.SectorCount < 0x60) && (BD_BUFFERING() == ON))
                {
                    if(ReadTimer(&bdEndLBA) > 10) break;
                    ExitProcess();
                }

                BDStopPrebuffering();
                BD_SET_SEGMENT_NO_FILLED(BdwtDisc.PreSegNo);
                DataBuffer.CacheEndLBA = 0;
            }
        }
        else
        {
                send_msg80(SEND_MRW_MSG,0x7100F1);
              return;
        }
    }   /* for (k = 0; k< c_num; k++) */

    BdwtReadExit();
    return;
}


/*----------------------------------------------------------------------------
  Name: BdwtReadExit
  Description:
        Exit read command process
  Input:  None
  Output: None
----------------------------------------------------------------------------*/
void BdwtReadExit(void)
{
    /* Turn Off LED */
    ReadyLED(LED_OFF);
    ENABLE_INTERRUPT();                 /* Enable all interrupt */
    BdwtDisc.CommandExe &= ~eBdReadCmd;
    AutoCommandComplete();
}


/*********************************************************************
FUNCTION    : BdGetDefectInfo
DESCRIPTION : This routine to analysis a host read  command
              to obtain the Defect cluster Information.
PARAMETERS  : StartLba - Host request start address
              Length   - Host request length
 RETURNS     : TRUE: There is a defeat cluster
*********************************************************************/
BYTE BdGetDefectInfo(ULONG StartLba, ULONG Length)
{
    ReplacementInfoType     ReplacementInfo;
    ULONG   FirstBlockStartLba, LastBlockStartLba, EndLba;
    PSNs    BlockStartPba;
    USHORT  TotalBlkCnt, BlockIndex;
    BYTE    defeat_f;

    // Calculate the Information for Host Request Command
    EndLba = StartLba + Length - 1;
    FirstBlockStartLba = StartLba & BD_CLUSTER_MASK;
    LastBlockStartLba = (EndLba & BD_CLUSTER_MASK);
    TotalBlkCnt = (LastBlockStartLba - FirstBlockStartLba)/SECTORS_PER_CLUSTER + 1;

    BlockStartPba = bdConvertLBAtoPSN(FirstBlockStartLba); // First Block

    defeat_f = 0;

    // Process Cluster by Cluster
    for (BlockIndex = 0; BlockIndex < TotalBlkCnt; BlockIndex++)
    {
        if (SearchReplacementInfo(BlockStartPba.PSN, &ReplacementInfo))  // This block is found in Defect List
        {
            defeat_f = 1;
            break;
        }

        // Update StartLba and Length, shift to next block
        StartLba = (StartLba & BD_CLUSTER_MASK) + SECTORS_PER_CLUSTER;
        BlockStartPba = bdConvertLBAtoPSN(StartLba);  //we need to use convert function here in case of layer jumper.
    }

    return (defeat_f);
}

/*----------------------------------------------------------------------------
  Name: BDAppendPreBuffer()
  Description:

  Input:  None

  Output: None
----------------------------------------------------------------------------*/
void BDAppendPreBuffer(void)
{
    //send_msg5SValue(SEND_MRW_MSG,"Append LSN", 4, BdwtDisc.PreSegNo<<24 | BdwtDisc.PreLSN & 0x00ffffff);
    send_msg5SValue(SEND_MRW_MSG,"Append PSN", 4, BdwtDisc.PreSegNo<<24 | BdwtDisc.PrePSN & 0x00ffffff);
    //send_msg5SValue(SEND_MRW_MSG,"PrePage", 2, BdwtDisc.PrePage);

    if ((BD_GET_SEG_TYPE(BdwtDisc.PreSegNo) == eEmptySeg) ||
        (BD_GET_SEG_TYPE(BdwtDisc.PreSegNo) == eReadSeg))
    {
        BD_SET_SEG_ECCLSN(BdwtDisc.PreSegNo, BdwtDisc.PreLSN);
        BD_SET_SEG_ECCPSN(BdwtDisc.PreSegNo, BdwtDisc.PrePSN);
        BD_SET_SEG_FILLINFO(BdwtDisc.PreSegNo, 0xFFFFFFFF);
        BD_SET_SEG_TYPE(BdwtDisc.PreSegNo, eReadSeg);

#if (CEProduct == 1)
        if ((!BDcmRegisterSegment(BdwtDisc.PreSegNo))||(DataBuffer.SectorCount> (BUF_TOTAL_BLK_COUNT-0x40))) 
#else
        if (!BDcmRegisterSegment(BdwtDisc.PreSegNo))
#endif 
        {
            bdStopDiscTransfer();
            BDcmFreeSegment(BdwtDisc.PreSegNo);
            --bdDiscRequest.ClustersBuffered;
            DataBuffer.SectorCount  -= ONE_BD_CLUSTER;
            bdLastDecodedPSN        -= ONE_BD_CLUSTER;
            DataBuffer.CacheEndLBA  -= ONE_BD_CLUSTER;
            Svr_pause_reqire = TRUE;
            send_msg5S(SEND_MRW_MSG,"PrebufStop-3");
#if BDWT_FAST_CACHE
            BdwtDisc.CacheFull = 1;
#endif
            return;
        }

        BdwtDisc.PreSegNo ++;

        BD_ADD_SEGMENT(BdwtDisc.NextReadSegNo, 1);

        if (BdwtDisc.PreSegNo >= eBDCacheSegmentCnt)
            BdwtDisc.PreSegNo = 0;

        BD_SET_SEGMENT_NO_FILLED(BdwtDisc.PreSegNo);
        BdwtDisc.PreLSN += ONE_BD_CLUSTER;

        BdwtDisc.PrePSN += ONE_BD_CLUSTER;
        BdwtDisc.PrePage = BD_GET_SEG_PAGE(BdwtDisc.PreSegNo);

#if BDWT_FAST_CACHE
        BdwtDisc.CacheFull = 0;
#endif
    }
    else
    {
        send_msg5SValue(SEND_MRW_MSG, "stp2", 4,  DataBuffer.CacheStartLBA & 0x00ffffff);
    	 send_msg5SValue(SEND_MRW_MSG,"SegType",1,BD_GET_SEG_TYPE(BdwtDisc.PreSegNo));
        bdStopDiscTransfer();
        Svr_pause_reqire = TRUE;
        send_msg5S(SEND_MRW_MSG,"PrebufStop-4");

#if BDWT_FAST_CACHE
        BdwtDisc.CacheFull = 1;
#endif
    }

    if (BdwtDisc.PreLSN == (pdbGetBDEndLBALayer0()+1))
    {
        PSNs tempPSN;
        tempPSN = bdConvertLBAtoPSN(BdwtDisc.PreLSN);
        BdwtDisc.PrePSN = tempPSN.PSN;
        send_msg5S(SEND_MRW_MSG,"Layer0 End");
        bdStopDiscTransfer();
        Svr_pause_reqire = TRUE;
        send_msg5S(SEND_MRW_MSG,"PrebufStop-5");
    }
}


/*----------------------------------------------------------------------------
  Name: process_key_in_BD
  Decription:
        Handles key press during MRW read commands processing

  Input:  None

  Output: None
----------------------------------------------------------------------------*/
static void process_key_in_BD(void)
{
    bdStopDiscTransfer();
    if(St_send_blkdata == TRUE)
    {
        AbortHostTransfer();
        St_send_blkdata = FALSE;
    }

    St_cmd_inproc = FALSE;
    BdwtReadExit();
}


/*----------------------------------------------------------------------------
  Name: BDTransferHostData
  Description:
        Handles the data transfer to host

  Input:  tx_page - start page address;
          tx_len  - number of pages to be transferred;

  Output: 0 - transfer completed;
          1 - transfer aborted;
----------------------------------------------------------------------------*/
#if BDWT_FAST_CACHE
BYTE BDTransferHostData(USHORT tx_page, USHORT tx_len, BOOL FastCache)
#else
BYTE BDTransferHostData(USHORT tx_page, USHORT tx_len)
#endif
{
    ULONG  StopAddr, bdmaxpsn;
    BYTE  temp_IE, i;

    if(tx_len == 0)
        return 0;

    if (BD_BUFFERING() == ON)
    {

        StopAddr = (tx_len + (tx_page&0x1F))>>5;
        bdmaxpsn = pdbGetBDMaxPSN(0);
        if(StopAddr > 0)
        {
            StopAddr += HAL_BD_CODEC_GetStopAddress();
            if (pdbGetBDLayers() == BD_DUAL_LAYER)
            {
                if ( (StopAddr << 5) > bdmaxpsn &&
                     (StopAddr << 5) < pdbGetBDStartPSN(1) )
                    HAL_BD_CODEC_SetStopAddress(bdmaxpsn >> 5);
                else if ( (StopAddr << 5) > pdbGetBDMaxPSN(1) )
                    HAL_BD_CODEC_SetStopAddress(pdbGetBDMaxPSN(1) >> 5);
                else
                    HAL_BD_CODEC_SetStopAddress(StopAddr);
            }
            else
            {
                if ( (StopAddr << 5) > bdmaxpsn )
                    HAL_BD_CODEC_SetStopAddress(bdmaxpsn >> 5);
                else
                    HAL_BD_CODEC_SetStopAddress(StopAddr);
            }
        }
    }

    temp_IE = GET_INT_ENABLE_MASK();
    DISABLE_INTERRUPT();

    Iram_Buf_hxfer_blk = tx_len;

    DataBuffer.CurHostPtr = tx_page;

    St_send_blkdata = TRUE;

    WRITE_REG(HAL_AHB_INTCTRLREG_MAP->ICITCR, 0x01);

    /* Clear the SATA Transmit and Recieve FIFOs */
    WRITE_REG(HAL_HSTREG_MAP->FWRST, 0x0C);
    for (i=0;i<0x05;i++);
    WRITE_REG(HAL_HSTREG_MAP->FWRST, 0);

    /*-------------------------------------------------------
        Set the starting page address
    --------------------------------------------------------*/
    WRITE_REG(HAL_HSTREG_MAP->HSTBUFPGL, tx_page);      /* start page address */

    /*-------------------------------------------------------
        Set up the page compare addresses
    --------------------------------------------------------*/
    WRITE_REG(HAL_HSTREG_MAP->BUFPGCMPL, tx_page);

    /*-------------------------------------------------------
        Set up the transfer block count
    --------------------------------------------------------*/
    WRITE_REG(HAL_HSTREG_MAP->HSTPGCL, Iram_Buf_hxfer_blk); /* block xfer number */
    WRITE_REG(HAL_HSTREG_MAP->HAVPGCL, Iram_Buf_hxfer_blk); /* Load Host Available Page Count Registers */

    /*-------------------------------------------------------
        Set up the byte offset into the page and
        word per page count
    --------------------------------------------------------*/
    WRITE_REG(HAL_HSTREG_MAP->HSTBYTACL, 0); /* offset */
    WRITE_REG(HAL_HSTREG_MAP->HSTWDCL, 0x400);     /* words count in each page */

#if (CHIP_REV >= 0xC0)
    WRITE_FIELD(HAL_HST_RESWDCL,0x00);
    WRITE_FIELD(HAL_HST_RESWDCH,0x00);
    WRITE_FIELD(HAL_HST_RESWDCEN,1);
#endif
    /*-------------------------------------------------------
        Set page offset mode and transfer direction
        (device ---> host)
    --------------------------------------------------------*/
    Iram_shHDDir &=  (~(HAL_HST_HXDIR | HAL_HST_HBODIS));
    WRITE_REG(HAL_HSTREG_MAP->HDDIR, Iram_shHDDir);

#if (ENABLE_AACS_BEF == 1)
    if (pdbDiscLoaded() == BD_DISC)
    {
        HAL_HOST_AesSetBEFBufferXferStart( ((ULONG)READ_FIELD(HAL_BDC_FDHSTBISS) << 11) + ((ULONG)tx_page << 11) + 80 );
        HAL_HOST_AesSetBEFSecOffset((tx_page & 0x001F));
    }
    HAL_HOST_StartAesBusEncryption();
#endif  // ENABLE_AACS_BEF

    /*-------------------------------------------------------
        Start the transfer
    --------------------------------------------------------*/
    HostXfer.State = TRANSFERRING;

    WRITE_REG(HAL_HSTREG_MAP->ATRG,HAL_HST_ADRTRG);

    /*-------------------------------------------------------
        LED set up
    --------------------------------------------------------*/
    #if (LED_NUM == 1)
    ReadyLED(LED_ON);
    #endif /* LED_NUM == 1 */

    #if (LED_NUM == 2)
    ReadyLED(LED_FLASH);
    #endif /* LED_NUM == 2 */

    SET_INT_ENABLE_MASK(temp_IE);

    #if BDWT_FAST_CACHE
    if (FastCache)
        return 0;
    #endif

    while (1)
    {
        if(EjectKeyInfo.PressedValid){
            BUILD_SENSE(0x02,0x3A,0x00,0x4b);    /* MEDIUM NOT PRESENT */
            process_key_in_BD();
            return 1;
        }

        if (!St_send_blkdata)
            break;

        ExitProcess();
    }

    return 0;
}

/*******************************************************************************
* Name: BDReadDataFromDisc
* Description:
*   Read data from BD Writable disc
* Inputs:  start_page - DRAM start page to put the buffered data
*	         blk_start_psn - the buffering target
*          length - ecc block count
* Output:  Status of the disc access during buffering
********************************************************************************/
DiscAccessStatusE BDReadDataFromDisc(USHORT start_page,ULONG read_start_psn,ULONG blk_start_lsn,BYTE length, BYTE mode)
{
    ULONG  blk_start_psn,blk_stop_psn;
    ULONG  DelayTimer;
    ULONG  SeekOKDelayTimer;
    ULONG  BgPreBufTimer;
    USHORT blk_start_page;
    BYTE   InterruptState;
    BYTE   BdwtRetryCnt,ExtraLatencyCnt;
    DiscAccessStatusE   DiscReadStatus;
    BYTE   TargetLayer;
    BYTE  *IceIndicator;
    BYTE  area;

    send_msg5SValue(SEND_MRW_MSG,"blk_s_psn",4,read_start_psn);
    //send_msg5SValue(SEND_MRW_MSG,"blk_s_lsn",4,blk_start_lsn);
    send_msg5SValue(SEND_MRW_MSG,"pg&len",4, start_page<<16|length);

    area = BdFindAddressArea(read_start_psn);

    if (mode == eREAD_DATA_FROM_DISC)
    {
        if ((area == eAREA_DA0) || (area == eAREA_DA1))
        {
            bdDiscRequest.ClusterCount = BD_PREBUFFER_LENGTH;
        }
        else
        {
            send_msg5SValue(SEND_MRW_MSG,"In Spare Area",1,area);
            bdDiscRequest.ClusterCount = 1;
        }
    }
    else
        bdDiscRequest.ClusterCount = length;

    if (mode == ePRE_BUFFERING)
    	  BdwtRetryCnt = 0;
    else
        BdwtRetryCnt   = MAX_BD_READ_RETRIES;
    DiscReadStatus = BUF_GOOD;
    bdDiscRequest.ClustersBuffered  = 0L;
    bdLastDecodedPSN = read_start_psn - 1L;
    DataBuffer.CurDiscPtr = start_page;
#if BDWT_FAST_CACHE
    BdwtDisc.ContinueIcedCnt = 0; //Clear Continue ICED Count. because buffer error will change the SDRAM data.
#endif
    bdDecFlag.fREADING_LEADIN = FALSE;

    BdwtDisc.ReadDataMode = mode;

    if(read_start_psn <= (pdbGetBDL0EndPAA()<<3))//Fixed a bug that the BD-R/RE layer should be detected by pdbGetBDL0EndPAA() length.
        TargetLayer = eLayer0;
    else
        TargetLayer = eLayer1;
    bdDecFlag.fFIRST_HOST_CLUSTER = TRUE;

    StartTimer(&DelayTimer);
    SDHoldSts = (vBOOL)READ_FIELD(HAL_BDC_HLDSDS);
    FDHoldSts = (vBOOL)READ_FIELD(HAL_BDC_HLDFDS);
    while (SDHoldSts == 0 || FDHoldSts == 0)
    {
        HAL_BD_CODEC_Disable_Buffering();

        if (ReadTimer(&DelayTimer) >= 100)
        {
            send_msg5S(SEND_READ_DVD_MSG, "SDFD HLDS Err 5");
            break;
        }

        SDHoldSts = (vBOOL)READ_FIELD(HAL_BDC_HLDSDS);
        FDHoldSts = (vBOOL)READ_FIELD(HAL_BDC_HLDFDS);
    }

    HAL_BD_CODEC_DISABLE_DECODER();

    STOP_BD_CLUSTER_TIMER();
    bdResetDiscFlags();

    HAL_BD_CODEC_ENABLE_DECODER();                  /* Turn on BD decoder */
    HAL_BD_CODEC_STREAM_PASS_ALL_OFF();             /* Disable Stream Pass All function */

    while (bdDiscRequest.ClustersBuffered < length)
    {
    	if (!svoIramVar.knlTimeupCount)
        {  /* time out */
            bdStopDiscTransfer();                   /* stop disc transferring       */
            bdResetDiscFlags();
            HAL_BD_CODEC_CLEAR_DECODER_STATUS();    /* clear all remaining status   */
            Svr_pause_reqire = TRUE;                /* Set pause request to servo   */
            Iram_Svr_player_mode = PAUSING_MODE;    /* Set pause mode for player    */
            DiscReadStatus = TIMEOUT;
            return(DiscReadStatus);                 /* Return the disc read error   */
        }

        if( BD_BUFFERING() == ON )
            bdStopDiscTransfer();

        if(EjectKeyInfo.PressedValid || TrayManagerInfo.Executing)
            return( EJECT_PRESS );

        if (( BD_CLUSTER_ERROR || BD_BUFFER_ERROR )&&(!BdwtDisc.BgPreBufAbort))
        {
            DiscReadStatus =GetBdError ();

            if (((BdwtRetryCnt == 0) &&  (!((Iram_Cmd_block.bf[0] == SBC_READ_10) && (St_cmd_inproc == TRUE))))
                || (DiscReadStatus == UNAUTH_RD_FAIL))
            {
                /*---------------------------------------------------------------
                     Maximum number of retries have performed or unauthorized
                     read (no retries necessary) attempted
                ----------------------------------------------------------------*/
                bdStopDiscTransfer();                  /* stop disc transferring       */
                HAL_BD_CODEC_CLEAR_DECODER_STATUS();             /* clear all remaining status   */
                Svr_pause_reqire = TRUE;                /* Set pause request to servo   */
                Iram_Svr_player_mode = PAUSING_MODE;    /* Set pause mode for player    */
                return(DiscReadStatus);                 /* Return the disc read error   */
            }
#if (CALIBRATION_MANAGEMENT == 1)
#if (CEProduct == 1)
            if (((BdwtRetryCnt >=1)&&(BdwtRetryCnt <= MAX_BD_READ_RETRIES -4)) && calGetCalRequired(TargetLayer & LAYERMASK, Iram_stp_CurrentPosition, ITEM_ALL, READ_MODE))//20090331_y.26==fixed the BD-R/RE disc ServoReCalibrate() issue.
#else
            if (((BdwtRetryCnt >=1)&&(BdwtRetryCnt <= MAX_BD_READ_RETRIES -1)) && calGetCalRequired(TargetLayer & LAYERMASK, Iram_stp_CurrentPosition, ITEM_ALL, READ_MODE))//20090331_y.26==fixed the BD-R/RE disc ServoReCalibrate() issue.
#endif
            {
                (void)ServoReCalibrate();
            }
#endif /* CALIBRATION_MANAGEMENT */
#if (HORROR_DISC_IMPROVEMENT == 1)
            else     // == improve BDREDL Ice detect and read with GZP disc
            {
                (void)ServoChgRFParameter(); // BDR(E) EDC error change DFE setting
            }
#endif

            /* For read 12, streaming read */
            if (BdwtDisc.Stream & eBdReadStream)
            {
                if (BdwtDisc.ReadDataMode == eREAD_DATA_FROM_DISC)
                {
                    if (BdwtRetryCnt <= (MAX_BD_READ_RETRIES - 2))
                    {
                        if (DiscReadStatus == TID_FAIL || DiscReadStatus == SEEK_FAIL || DiscReadStatus == TIMEOUT)
                        {
                            BdwtRetryCnt = MAX_BD_READ_RETRIES + 1;

                            DataBuffer.CurDiscPtr += ONE_BD_CLUSTER;
                            DataBuffer.CurDiscPtr %= BUF_TOTAL_BLK_COUNT;
                            if ( DataBuffer.CurDiscPtr == 0 )
                            {
                                IceIndicator =  (BYTE *)(BD_ICE_INDICATOR_ADDR + ((BUF_TOTAL_BLK_COUNT - ONE_BD_CLUSTER) >> 5));
                            }
                            else
                                IceIndicator =  (BYTE *)(BD_ICE_INDICATOR_ADDR + ((DataBuffer.CurDiscPtr - ONE_BD_CLUSTER) >> 5));

                            *IceIndicator = eICE_BLOCK;

                            /*---------------------------------------------------------------
                                Update count of clusters transferred, cluster count and
                                last PSN in buffer.
                            ---------------------------------------------------------------*/
                            ++bdDiscRequest.ClustersBuffered;
                            DataBuffer.SectorCount  += ONE_BD_CLUSTER;
                            bdLastDecodedPSN        += ONE_BD_CLUSTER;

                            if (DataBuffer.CacheEndLBA == 0L)
                                DataBuffer.CacheEndLBA = 0x1FL;
                            else
                            {
                                DataBuffer.CacheEndLBA += ONE_BD_CLUSTER;
                            }
                            DiscReadStatus = BUF_GOOD;

                            if (bdDiscRequest.ClustersBuffered >= length)
                            {
                                send_msg5S(1, "Bypass 3");
                                return (DiscReadStatus);
                            }
                        }
#if (BypassDecoderEr == 1)
                        else
                        {
                            IGNORE_DECODER_ERROR = TRUE;
                        }
#endif
                        send_msg5S(1, "Bypass 3");
                    }
                }
            }
            if(BdwtRetryCnt)
                --BdwtRetryCnt;
        }

        InterruptState = GET_INT_ENABLE_MASK();
        DISABLE_INTERRUPT();

        /* Initialize transfer variables */
        blk_start_psn  = read_start_psn + bdDiscRequest.ClustersBuffered * ONE_BD_CLUSTER;
        blk_start_page = (start_page + bdDiscRequest.ClustersBuffered * ONE_BD_CLUSTER) % eBDTotalPageNo;

        HAL_BD_CODEC_SetDiscPtr(blk_start_page);

        SET_INT_ENABLE_MASK(InterruptState);

        blk_stop_psn = blk_start_psn + (bdDiscRequest.ClusterCount - bdDiscRequest.ClustersBuffered)*ONE_BD_CLUSTER - 1;

        HAL_BD_CODEC_SetStartAddress((blk_start_psn >> 5));
        HAL_BD_CODEC_SetStopAddress((blk_stop_psn >> 5));

        /*---------------------------------------------------------------
        Clear all BD flags and status and set transfer state
        ----------------------------------------------------------------*/
        bdResetDiscFlags();
        HAL_BD_CODEC_CLEAR_DECODER_STATUS();

        START_BD_CLUSTER_TIMER(FIRST_CLUSTER_TIMEOUT_VALUE);
        HAL_BD_CODEC_ENABLE_DECODER_INTS();

        if (mode == ePRE_BUFFERING)
    	    ExtraLatencyCnt = 0;
        else
            ExtraLatencyCnt = MAX_BD_READ_RETRIES - BdwtRetryCnt;

        /*-------------------------------------
            seek to target
        --------------------------------------*/
#if (XDFCT_ENABLE == 1)
        if( bdSeek((blk_start_psn - (BdwtRetryCnt%4)*0x20) , TargetLayer) == Error)
#else
        if( bdSeek(blk_start_psn - ExtraLatencyCnt*0x20 , TargetLayer) == Error)
#endif //(XDFCT_ENABLE == 1)
        {
            if(!BdwtDisc.BgPreBufAbort)//Don't speed down when seek fail during BgPreBufAbort.
                SpeedCount = SPEED_DOWN_FOR_SEEK_FAIL;
            bdErrorFlag.fSEEK_FAIL = TRUE;
            DiscReadStatus = SEEK_FAIL;
        }
        else
        {
            /*---------------------------------------------------------------
                    Seek successful and start decoding
            ----------------------------------------------------------------*/
            //send_msg5S(1,"Seek OK");
            //This delay is to make sure that the ADIP already lock before target search.
            //The small de-interleaver data can get correct data to avoid tgt miss.
            StartTimer(&SeekOKDelayTimer);
            while ( (ReadTimer(&SeekOKDelayTimer) < 4) )
            {
                if ( (READ_FIELD(HAL_WBL_BDADRLCK)) && HAL_WOBBLE_GetSyncFlyWheelInLock())
                {
                    break;
                }
            }
#if (XDFCT_ENABLE == 1)
            if (BdwtRetryCnt <= (MAX_BD_READ_RETRIES - 1))
            {
                bdStartSpecialDiscHandle(blk_start_psn, BdwtRetryCnt);
            }
#endif
            HAL_BD_CODEC_ENABLE_TARGET_MISS_INT();
            DiscReadStatus       = BUF_GOOD;
            gFirstClusterDone = FALSE;

            HAL_BD_CODEC_Enable_Buffering();

            START_BD_CLUSTER_TIMER(BD_CLUSTER_TIMEOUT_VALUE);
        }

        ReadyLED(LED_OFF);

        if(BdwtDisc.BgPreBuffer == 1)
            StartTimer(&BgPreBufTimer);
        /*-------------------------------------------------------------------
                Monitor BD Buffering
        --------------------------------------------------------------------*/
        while(1)
        {
           if(BdwtDisc.BgPreBuffer == 1)
            {
                if(ReadTimer(&BgPreBufTimer) > 100 || BdwtDisc.BgPreBufAbort)
                {
                    bdStopDiscTransfer();                  /* stop disc transferring       */
                    bdResetDiscFlags();
                    HAL_BD_CODEC_CLEAR_DECODER_STATUS();   /* clear all remaining status   */

                    send_msg5S(SEND_MRW_MSG,"pBuf_Tout");

                    DiscReadStatus = TIMEOUT;
                    return(DiscReadStatus);
                }
            }
            if (!svoIramVar.knlTimeupCount)
            {  /* time out */
                bdStopDiscTransfer();                  /* stop disc transferring       */
                bdResetDiscFlags();
                HAL_BD_CODEC_CLEAR_DECODER_STATUS();                 /* clear all remaining status   */
                Svr_pause_reqire = TRUE;                /* Set pause request to servo   */
                Iram_Svr_player_mode = PAUSING_MODE;    /* Set pause mode for player    */
                DiscReadStatus = TIMEOUT;
                return(DiscReadStatus);                 /* Return the disc read error   */
            }

            if(svoIramVar.focusDropFlag == TRUE)
            {
                send_msg5S(SEND_MRW_MSG,"RdDisc foDp");
                //return(GEN_FAIL); //
            }
            /*---------------------------------------------------------------
                Check for Servo Skating
            ----------------------------------------------------------------*/
#if (HIF_DETECT_SKATING== 1)
            if(svoIramVar.focusDropFlag == FALSE)// if focus drop, there is no ID.
            {
                {//get current PSN, Don't Call BdGetCurrentPSN(), because we don't want to wait!!
                 //just using the valid ID to check.
                    ULONG CurrentPSN, readtimer;
                    ULONG AUNFLYRegister, AUNFLYStatus, AUNStatusRegister, AUNRegister, AUNEndRegister;
                    BYTE AUNDecodingStatus;
                    BYTE BdDecoder_ID = FALSE;

                    {
                        //read data and flywheel
                        AUNFLYRegister    = (ULONG) READ_REG(HAL_BDCREG_MAP->BDAUNFLY);
                        AUNFLYStatus      = (ULONG) READ_REG(HAL_BDCREG_MAP->BDAUNFLYS);
                        AUNStatusRegister = (ULONG) READ_REG(HAL_BDCREG_MAP->BDAUNS);
                        AUNRegister       = (ULONG) READ_REG(HAL_BDCREG_MAP->BDAUN);
                        AUNEndRegister    = (ULONG) READ_REG(HAL_BDCREG_MAP->BDAUNEND);

                        // check buffer full/ready
                        if ((AUNStatusRegister & HAL_BDC_AUNRDY) == HAL_BDC_AUNRDY)
                        {
                            AUNDecodingStatus = (BYTE) (AUNStatusRegister & HAL_BDC_AUNDECOS);
                            //send_msg81(SEND_ATAPI_CMD,0x5B0607, AUNDecodingStatus);

                            if ( (AUNDecodingStatus == halBDC_ErrataFree) || (AUNDecodingStatus == halBDC_Correctable))
                            {
                                //if ((READ_FIELD( HAL_BDC_AUNFLYLCK )) && (HAL_DFE_GetPllInLock()))
                                //if ((HAL_BD_CODEC_GetAUNFLYLock())&&(HAL_BD_CODEC_GetDMDLock()))
                                if((AUNFLYStatus & HAL_BDC_AUNFLYLCK) == HAL_BDC_AUNFLYLCK)
                                {
                                    //CurrentPSN = AUNRegister;
                                    CurrentPSN = AUNFLYRegister;
                                    SendMsgCn(HEADER_ID_MSG,4,0x3B060D,A4B(svoIramVar.currentID.SectorNum.all32));//CheckHeaderID AUN Address :
                                    BdDecoder_ID = TRUE;
                                }
                            }
                        }
                #if (EN_WOBBLE == 1 )
                       //--------------------------------------------------------------------------;
                       // check Wobble address                                                       ;
                       //--------------------------------------------------------------------------;
                       if(pdbGetIDOk())
                       {
                           DISABLE_INTERRUPT();
                           CurrentPSN = (Iram_DiscInfo.BD.ClusterAddress << 3);
                           svoIramVar.currentID.SectorNum.all32 = CurrentPSN & 0x01FFFFFF;
                           CurrentPSN = svoIramVar.currentID.SectorNum.all32 ;
                           //send_msg80(DEBUG_WBL_MSG,0x610101); //BD decoder wobble address
                           //send_msg5L(DEBUG_WBL_MSG, svoIramVar.currentID.SectorNum.all32);
                           pdbSetIDOk(FALSE);
                           ENABLE_INTERRUPT();
                           BdDecoder_ID = TRUE;
                       }
                #endif
                    }

                    if(BdDecoder_ID)
                    {
                        ULONG tempPSN;
                        //The readtimer change for 3 to 5 cluster because the TGTF INT for BDR/RE needs more delay.
                        tempPSN = (bdLastDecodedPSN + 1L+ONE_BD_CLUSTER+4*ONE_BD_CLUSTER) - CurrentPSN;//decode end address+0x20 - current addrss.
                        readtimer = svoSectors2Duration(tempPSN);             //calculate spend time.
                        if((bdClusterInterruptTimer.count*20)<readtimer)
                        {
                            SendMsgCn(SEND_ATAPI_CMD,6,0x800509,A4B(CurrentPSN),A2B(bdClusterInterruptTimer.count));//20090406_r.27 == Fixed buffer time out message length.
                            bdClusterInterruptTimer.fTIMEOUT = TRUE;
                            RecoverDecoderError();//20090406_r.27== Decrease speedcnt when buffer time out occur.
                        }
                    }
                }
            }
#endif

            /*---------------------------------------------------------------
                Check for buffered block errors posted by BD ISR
            ----------------------------------------------------------------*/
            if (BD_CLUSTER_ERROR)
            {
                send_msg5S(SEND_READ_DVD_MSG, "BD_CLUSTER_ERROR");
                StartTimer(&DelayTimer);
                SDHoldSts = (vBOOL)READ_FIELD(HAL_BDC_HLDSDS);
                FDHoldSts = (vBOOL)READ_FIELD(HAL_BDC_HLDFDS);
                while (SDHoldSts == 0 || FDHoldSts == 0)
                {
                    HAL_BD_CODEC_Disable_Buffering();

                    if (ReadTimer(&DelayTimer) >= 200)
                    {
                        send_msg5S(SEND_READ_DVD_MSG, "SDFD HLDS Err 6");
                        break;
                    }

                    SDHoldSts = (vBOOL)READ_FIELD(HAL_BDC_HLDSDS);
                    FDHoldSts = (vBOOL)READ_FIELD(HAL_BDC_HLDFDS);
                }

                HAL_BD_CODEC_DISABLE_DECODER();             /* Reset bd decoder */

                HAL_BD_CODEC_ENABLE_DECODER();              /* Turn on bd decoder */
                break;
            }
            /*---------------------------------------------------------------
                Check for buffering errors (non block errors)
            ----------------------------------------------------------------*/
            if (BD_BUFFER_ERROR)
            {
                send_msg5SValue(SEND_READ_DVD_MSG, "Buf Err", 4, bdErrorFlag.fSEEK_FAIL <<16 | bdErrorFlag.fTID_MISS <<8 | bdClusterInterruptTimer.fTIMEOUT);
                StartTimer(&DelayTimer);
                SDHoldSts = (vBOOL)READ_FIELD(HAL_BDC_HLDSDS);
                FDHoldSts = (vBOOL)READ_FIELD(HAL_BDC_HLDFDS);
                while (SDHoldSts == 0 || FDHoldSts == 0)
                {
                    HAL_BD_CODEC_Disable_Buffering();

                    if (ReadTimer(&DelayTimer) >= 200)
                    {
                        send_msg5S(SEND_READ_DVD_MSG, "SDFD HLDS Err 7");
                        break;
                    }

                    SDHoldSts = (vBOOL)READ_FIELD(HAL_BDC_HLDSDS);
                    FDHoldSts = (vBOOL)READ_FIELD(HAL_BDC_HLDFDS);
                }

                HAL_BD_CODEC_DISABLE_DECODER();             /* Reset bd decoder */

                HAL_BD_CODEC_ENABLE_DECODER();              /* Turn on bd decoder */
                break;
            }

            /*---------------------------------------------------------------
                Check if all requested blocks have been buffered
            ----------------------------------------------------------------*/
            if(bdDecIntFlag.fECC_ALL_DONE_INT)
                break;

            if (BdwtDisc.ReadDataMode == eREAD_DATA_FROM_DISC && bdDiscRequest.ClustersBuffered >= length)
            {
                send_msg5S(SEND_MRW_MSG,"buf OK");
                return(DiscReadStatus);
            }
            if (BdwtDisc.ReadDataMode == ePRE_BUFFERING && gFirstClusterDone == TRUE)
            {
                send_msg5S(SEND_MRW_MSG,"BD SB_OK");
                gFirstClusterDone = FALSE;
                return(DiscReadStatus);
            }



            if(EjectKeyInfo.PressedValid || TrayManagerInfo.Executing)
                return( EJECT_PRESS );

            ExitProcess();
        } /* End Monitor Buffering Loop */
    } /* END BdwtRetryCnt Loop */

    /*---------------------------------------------------------------
       All requested blocks have been bufferred. Stop the transfer
       and reset the disc flags
    ----------------------------------------------------------------*/
    bdStopDiscTransfer();
    bdResetDiscFlags();

    /*---------------------------------------------------------------
       Mark a servo pause request and return disc status
    ----------------------------------------------------------------*/
    Svr_pause_reqire = TRUE;
    return(DiscReadStatus);

}


/*----------------------------------------------------------------------------
  Name: BDStopPrebuffering()
  Description:

  Input:  None

  Output: None
----------------------------------------------------------------------------*/
void BDStopPrebuffering(void)
{

    //check BG preBuffering
    if(BdwtDisc.BgPreBuffer)
    {
        BdwtDisc.BgPreBufAbort = 1;
        svoIramVar.emergencyStopFlag = TRUE;
        send_msg80(SEND_MRW_MSG,0x7100F2);

        while(BdwtDisc.BgPreBufAbort)
        {
            ExitProcess();
        }
        svoIramVar.emergencyStopFlag = FALSE;
    }

    bdStopDiscTransfer();
}


/*----------------------------------------------------------------------------
  Name: BdwtInitializeVar
  Description:
            Initializes all the BD writable discs related variables after mounting is successful.
  Input:  None
  Output: None
----------------------------------------------------------------------------*/
void BdwtInitializeVar(void)
{
    ClearDataBuffer();
    BdwtDisc.CommandExe = 0;
    BdwtDisc.Stream = eBdNoStream;
    BdwtDisc.PrevCommandIsRead  = CLEAR;
    BdwtDisc.PrevCommandIsWrite = CLEAR;
    BdwtDisc.BgPreBuffer = 0;
    BdwtDisc.BgPreBufAbort = 0;
    #if BDWT_FAST_CACHE
    BdwtDisc.CacheFull = 0;
    BdwtDisc.ContinueIcedCnt = 0;
    #endif
}

#if BDWT_FAST_CACHE
/*******************************************************************************

    FUNCTION        BdwtSearchCache

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
SearchCacheResultE BdwtSearchCache(TransferS Request)
{
    ULONG   RequestEndLBA;
    BYTE *  IceIndictor;
    BYTE    cnt, sg, SgCnt, FreeSg;

    if (DataBuffer.CacheEndLBA == 0)
    {
        return (CACHE_MISS);
    }

    /*--------------------------------------------------
        No data in the buffer
    --------------------------------------------------*/
    //CacheUsage = 0 means after fast cache there is no data inside
    if ((Request.StartBlock.lba < DataBuffer.CacheStartLBA) || (Request.StartBlock.lba > DataBuffer.CacheEndLBA))
    {
        send_msg54(SEND_MRW_MSG,'m','i','s','2');
        return (CACHE_MISS);
    }

    RequestEndLBA = (Request.StartBlock.lba + Request.BlockCount.Long - 1);
    /*--------------------------------------------------
        prebuffering should be ON, BdwtDisc.PreLSN = next to be decoded address
    --------------------------------------------------*/
    if(DataBuffer.CacheEndLBA < RequestEndLBA)
    {
        send_msg54(SEND_MRW_MSG,'m','i','s','3');
        return (CACHE_MISS);
    }

    if((MaxDflEndLBA != 0) && ((Request.StartBlock.lba <= MaxDflEndLBA) && (RequestEndLBA >= MinDflStartLBA)))
    {
        if(BdGetDefectInfo(Request.StartBlock.lba, Request.BlockCount.Long))
        {
            send_msg5S(SEND_MRW_MSG,"CacheMiss-4");
            return (CACHE_MISS);
        }
    }
#if (ENABLE_BDWT_BG_PREBUFFER)
    if ((BD_BUFFERING() == OFF) && (BdwtDisc.BgPreBuffer == 0))
    {

        cnt = (DataBuffer.CacheEndLBA + 1 - ((Request.StartBlock.lba + Request.BlockCount.Long + 0x1F) & ~0x1FL)) >> 5;
    /* If the host request quickly, cache will be maintained in low value.
            FW will start prebuffering in end of read command, not in BG task. */
        if(((BD_CACHE_RESERVED_CNT + 1) <= cnt) && (cnt <= BD_PREBUF_THRESHOLD))
        {
            send_msg5SValue(SEND_MRW_MSG, "BgPreBufON",4,DataBuffer.CacheEndLBA);
            BdwtDisc.BgPreBuffer = 1;
        }
    }
#endif
    if(Request.StartBlock.lba > DataBuffer.CacheStartLBA)
    {
        cnt = Request.StartBlock.lba - DataBuffer.CacheStartLBA;
        DataBuffer.SectorCount -= cnt;
        DataBuffer.CurHostPtr += cnt;
        DataBuffer.CurHostPtr %= BUF_TOTAL_BLK_COUNT;
        DataBuffer.CacheStartLBA = Request.StartBlock.lba;
    }

    /* The requested blocks hit in the cache */
    IceIndictor = (BYTE *)(BD_ICE_INDICATOR_ADDR + (DataBuffer.CurHostPtr >> 5));  //IceIndictor is one byte revolution

    //Zero fill use variable that was set in ISR, FW can't call ZeroFillDRAM() here
    if (*IceIndictor & eICE_BLOCK)
    {
        if (BdwtDisc.ContinueIcedCnt >= eBDTotalPageNo)
        {
            send_msg54(SEND_MRW_MSG, 'Z', 'R', 'O', '7');
        }
        else
        {
            send_msg54(SEND_MRW_MSG, 'Z', 'R', 'O', '6');
            return (CACHE_MISS);
        }
    }
    else
        BdwtDisc.ContinueIcedCnt = 0;

    send_msg5SValue(SEND_MRW_MSG,"BDFHit", 4, DataBuffer.SectorCount <<16 | Request.BlockCount.Long);

    sg = DataBuffer.CurHostPtr >> 5;
    FreeSg = DataBuffer.CurHostPtr & 0x1F;
    if (BDTransferHostData(DataBuffer.CurHostPtr, Request.BlockCount.Long, 1) == 0) //Transfer OK.
    {
        //update BufPgCMP page
        SgCnt = READ_REG(HAL_HSTREG_MAP->BUFPGCMPL);
        WRITE_REG(HAL_HSTREG_MAP->BUFPGCMPL, ((SgCnt + Request.BlockCount.Long) % BUF_TOTAL_BLK_COUNT));
    }

    SgCnt = ((FreeSg + Request.BlockCount.Long - 1) >> 5) + 1;

    for(cnt = 0; cnt < SgCnt; cnt++)
    {
        FreeSg = sg + cnt;
        if(FreeSg >= eBDCacheSegmentCnt)
            FreeSg -= eBDCacheSegmentCnt;

        BD_SET_SEG_TYPE(FreeSg, eReadSeg);
    }

    return ( FULL_CACHE_HIT );
}
#endif  // BDWT_FAST_CACHE

BYTE BDPreReadBufferingForCpuUsage (void)
{
    PSNs StartPSN;
    BYTE result;

    BdwtDisc.PreLSN = 0;
    StartPSN = bdConvertLBAtoPSN(BdwtDisc.PreLSN);
    BdwtDisc.PreSegNo = BdwtDisc.NextReadSegNo;
    BdwtDisc.PrePage = 0;

    DataBuffer.CacheStartLBA = BdwtDisc.PreLSN;
    DataBuffer.CacheEndLBA = BdwtDisc.PreLSN - 1;  //for pre-buffering check

    result = BDReadDataFromDisc(BdwtDisc.PrePage, StartPSN.PSN, BdwtDisc.PreLSN, BD_PREBUFFER_LENGTH, ePRE_BUFFERING);

    if (result == BUF_GOOD)
    {

        send_msg5SValue(SEND_MRW_MSG,"PreBuf LSN",4,BdwtDisc.PreLSN);
        send_msg5SValue(SEND_MRW_MSG,"PreBuf Pg",2,BdwtDisc.PrePage);
    }
    else
    {
    	  send_msg5SValue(SEND_MRW_MSG,"PreBuf NG",1,result);
    }
    return result;
}


#endif /* BD_ENABLE */
