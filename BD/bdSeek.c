/*****************************************************************************
*                (c) Copyright 2000 - 2007 SUNEXT DESIGN, INC.
*                            All Rights Reserved
*
*
*   FILENAME:     $Workfile: bdSeek.c $
*
*   DESCRIPTION:    Manages the host BD Seek Command and other BD Seek
*                   related functions.
*
* $Revision: 26 $
* $Date: 10/12/21 4:15p $
*
**************************** SUNEXT CONFIDENTIAL ****************************/

#include ".\common\globtype.h"
#include ".\oem\oem_spd_tbl.h"
#include ".\hif\At_equ.h"
#include ".\bd\bdDef.h"
#include ".\bd\bdMacro.h"
#include ".\bd\bdVars.h"
#include ".\bd\bdseek.h"
#include ".\bd\bdUtil.h"
#include ".\bd\bdDiag.h"
extern BYTE CmdSeekOk;
extern ULONG CmdSeekTimer;

#if (BD_ENABLE)
void BDSeekCommand()
{
    StLongU StartLBA;
    PSNs   StartPSN;

    // Avoid that buffer is full and then speed down during executing seek10 command.
    SpeedCount = 0;

    /*-------------------------------------------------------------------
        Get the host request starting LBA from the command block
    --------------------------------------------------------------------*/
    StartLBA.byte.u = Iram_Cmd_block.bf[2];
    StartLBA.byte.h = Iram_Cmd_block.bf[3];
    StartLBA.byte.m = Iram_Cmd_block.bf[4];
    StartLBA.byte.l = Iram_Cmd_block.bf[5];
    StartPSN = bdConvertLBAtoPSN(StartLBA.Long);

    /*-------------------------------------------------------------------
        Check if the starting LBA is within range
    --------------------------------------------------------------------*/
    if (StartLBA.Long > pdbGetBDMaxLBA())
    {
        BUILD_SENSE(0x05,0x21,0x00,0x02);    // LOGICAL BLOCK ADDRESS OUT OF RANGE
        ATASetDSC();
        AutoCommandComplete();          // Issue command complete
        return;
    }
    ReadyLED(LED_FLASH1S);
    /*--------------------------------------------------------------------
                Set to maximum speed
    ---------------------------------------------------------------------*/
    if ( Iram_Cmd_block.bf[0] != REZERO_UNIT )
    {
        if(SpeedMD == Set_sp_f)
            SpeedMD = Special_f;
    }

    // var SeekLayer now contains correct layer seek PSN resides on.
    // var SeekPsn   now contains correct       seek PSN

//if current PSN near the Seek PSN, return successful without real seek.
    if( (BD_BUFFERING() == ON)&&(((StartLBA.Long > DataBuffer.CacheEndLBA)&&(StartLBA.Long < (DataBuffer.CacheEndLBA+2*ONE_BD_CLUSTER)) )||( (StartLBA.Long <= DataBuffer.CacheEndLBA)&&(StartLBA.Long >= DataBuffer.CacheStartLBA) )))
    {
        CmdSeekOk = 1; //seek ok set 1
        StartTimer(&CmdSeekTimer);
        BUILD_SENSE(0x00,0x00,0x00,0x02);       // "NO ADDITIONAL SENSE INFORMATION" Seek Complete
    }
    else if (BdwtDisc.CpuUsage8X)
    {
        BDcmInit();
        CmdSeekOk = 1; //seek ok set 1
        
        BDPreReadBufferingForCpuUsage();        
        
        StartTimer(&CmdSeekTimer);
        BUILD_SENSE(0x00,0x00,0x00,0x02);       // "NO ADDITIONAL SENSE INFORMATION" Seek Complete         
    }
    else
    {
    /*------------------------------------------------------------------
    After get a dvd seek command
    if servo seek ok   ==> start timer  (5sec do nothing than call pause in HIFStart())
       servo seek fail  ==> call pause
    -------------------------------------------------------------------*/
        if (!((pdbGetDiscKind() == eBDROMDisc) || (pdbGetDiscKind() == eBDROMDLDisc)))
            BDcmInit(); 

        CmdSeekOk = 0;


        if( (bdSeek(StartPSN.PSN , StartPSN.Layer)) == Error)
        {
            CmdSeekOk = 2; //seek fail set 2
            Svr_pause_reqire = TRUE;
            BUILD_SENSE(0x03,0x02,0x00,0x10);       // "NO SEEK COMPLETE"
        }
        else
        {
            CmdSeekOk = 1; //seek ok set 1
            RandomRead = TRUE;
            StartTimer(&CmdSeekTimer);
            BUILD_SENSE(0x00,0x00,0x00,0x02);       // "NO ADDITIONAL SENSE INFORMATION" Seek Complete
        }
    }
    St_cmd_inproc = FALSE;                      /* mark no cmd in proc */
    Ata_sub_burst = FALSE;

    ATASetDSC();                                /* set dsc bit */
    AutoCommandComplete();
    return;
}


/*****************************************************************
    Function:       bdSeek
    Description:    Issues a BD seek and sets up player mode
                    if seek is good else issues a reseek until
                    retries are exhuasted

    Returns:        Error or Ready

******************************************************************/
BYTE bdSeek(ULONG TargetID, BYTE Layer)
{
#if BDWT_FAST_CACHE
    ULONG   BgPreBufTimer;
#endif
    ULONG   retry_timer;
    BYTE    retry_cnt;

    StartTimer(&retry_timer);

    if( BD_BUFFERING() == ON )
        bdStopDiscTransfer();

    if (SpeedMD == StartUp_f)
        retry_cnt = 0;
    else
        retry_cnt = 5;

#if BDWT_FAST_CACHE
    StartTimer(&BgPreBufTimer);
#endif

    while(1)
    {
        if(EjectKeyInfo.PressedValid || TrayManagerInfo.Executing)
        {
 //           BUILD_SENSE(0x02,0x3A,0x00,0x08);            /* MEDIUM NOT PRESENT */
            SendMsgCn(SEND_ATAPI_CMD,1,0x800005, FALSE);   //Seek End, Success=%u
            return(Error);
        }

        if(ReadTimer(&retry_timer) >= 3000*10)
        {
            SpeedCount = SPEED_DOWN_FOR_SEEK_FAIL;    // speed down
            SendMsgCn(SEND_ATAPI_CMD,1,0x800005, FALSE);   //Seek End, Success=%u
            return(Error);
        }
#if (SEQ_SEEK_SPEED_DOWN == 1)
    if (RandomRead)
    {
        if(SpeedCount > 0) // don't count the read retry seek.
        {
            RandomCount++;
            if (RandomCount == 0xFFFFFFFF)  //wrap back
                RandomCount = CONTINUE_RANDOM_READ_CNT + 1;
            
            if (RandomCount > CONTINUE_RANDOM_READ_CNT )
                SpeedMD = Special_for_SeekSpeed;
        }
    }
    else
    {   //if any sequential read coming
        
        if(SpeedMD == Special_for_SeekSpeed)
        {   
            if(HostXfer.BlockCount.Long == 0x19)
            {
                SpeedMD = Special_for_CDSpeed_f;
                SpeedCount = BD_SPEED_UP_READY -0x200;
            }
            else                
            {
                SpeedMD = AutoMode;
                SpeedCount = 0;
            }
        }
           
        RandomCount = 0;
    }
#endif
        Set_SpeedMDCont();

        send_msg80(SEND_ATAPI_CMD,0x800001);   //Seek Start
        if (ServoSeek(TargetID, Layer) == PASS)
        {
             /* Seek BD was successful */
            Svr_pause_reqire = FALSE;               /* Clear pause request flag */
            Iram_Svr_player_mode = READING_MODE;    /* Change to reading_mode   */
            SendMsgCn(SEND_ATAPI_CMD,1,0x800005, TRUE);   //Seek End, Success=%u
            return(Ready);
        }
        else
        {
            if(EjectKeyInfo.PressedValid || TrayManagerInfo.Executing)
                return(Error);

             /* BD seek failed */
            if (retry_cnt--  == 0)  /* Update retry count and if exhausted return error */
            {
                SpeedCount = SPEED_DOWN_FOR_SEEK_FAIL;    // speed down
                SendMsgCn(SEND_ATAPI_CMD,1,0x800005, FALSE);   //Seek End, Success=%u
                return(Error);
            }
        }

#if BDWT_FAST_CACHE
        if (BdwtDisc.BgPreBuffer == 1)
        {
            if(ReadTimer(&BgPreBufTimer) > 50 || BdwtDisc.BgPreBufAbort)
            {
                SendMsgCn(SEND_ATAPI_CMD,1,0x800005, FALSE);   //Seek End, Success=%u
                return(Error);
            }
        }
#endif

        ExitProcess();
    } /* end of While */
}

#endif  //(BD_ENABLE)

