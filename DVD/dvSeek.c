/****************************************************************************
*                (c) Copyright 2000- 2005 SUNEXT DESIGN, INC.
*                           All Right Reserved
*
*
*    FILE NAME:     $Workfile: dvSeek.c $
*
*    DESCRIPTION:   Manages the host DVD Seek Command
*                          and other DVD Seek related functions.
*
*
*   $Revision: 80 $
*   $Date: 11/01/05 8:32p $
*
**************************** SUNEXT CONFIDENTIAL *******************************/

#include ".\common\globals.h"
#include ".\common\globtype.h"
#include ".\common\com_im.h"
#include ".\hif\gp_util.h"
#include ".\DVD\dvGvars.h"
#include ".\DVD\dvMMC.h"
#include ".\DVD\dvHostRd.h"
#include ".\DVD\dvSeek.h"
#include ".\dvd\dvMacros.h"
#include ".\dvd\dvutil.h"
#include ".\dvd\dvdiscrd.h"
#if (ENABLE_HDDVD)
#include ".\hd\hdUtil.h"
#include ".\hd\hdDef.h"
#include ".\hd\hdDB.h"
#endif

#if (DVD_RAM_READ == 1)
#include ".\defmgmt\common\mrw_ram.h"
#endif
#include ".\al\REG_dvd_decoder.h"

#if (MSG_OUT_MODE == MSG_USBPORT) && (ENABLE_UART_RXD == 1)
#include ".\common\scomm.h"
#endif

#if (OPEN_SESSION == 1)
#include ".\servo\svoVar.h"
#endif


/*******************************************************************
    local PROTOTYPES
*******************************************************************/
PSNs dvdConvertLBAtoPSN(LONG);
void ProcessEjectRequest(void);


/*******************************************************************************
    FUNCTION DEFINITIONS
*******************************************************************************/
extern ULONG Last_PSN;


/*******************************************************************************

    FUNCTION        DVDSeek10Command

    DESCRIPTION     Command handler for DVD Seek command
                    This function will parse the command and then perform the Seek.


********************************************************************************/
extern BYTE CmdSeekOk;
ULONG CmdSeekTimer;

void DVDSeek10Command()
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

#if (ENABLE_HDDVD)
        if(pdbDiscLoaded() == HD_DISC)
        {
            StartPSN = HDConvertLBAtoPSN(StartLBA.Long);
        }
        else
#endif
#if (DVD_RAM_READ == 1)
        if (pdbGetDVDType() == DVD_RAM)
        {
            StartPSN.PSN = ConvertLSNToPSN(StartLBA.Long);
            StartPSN.Layer = eLayer0;
        }
        else
#endif
        {
            StartPSN = dvdConvertLBAtoPSN(StartLBA.Long);
        }
    READ_PI_PO = 0;
    Last_PSN = StartPSN.PSN & DVD_BLOCK_MASK;
    /*-------------------------------------------------------------------
        Check if the starting LBA is within range
    --------------------------------------------------------------------*/
#if (ENABLE_HDDVD)
    if(pdbDiscLoaded() == HD_DISC)
    {
        if (StartLBA.Long > pdbGetHDMaxLBA() )
        {
            BUILD_SENSE(0x05,0x21,0x00,0x02);    // LOGICAL BLOCK ADDRESS OUT OF RANGE
            ATASetDSC();
            AutoCommandComplete();          // Issue command complete
            return;
        }
    }
    else
#endif
    if (StartLBA.Long > pdbGetDVDMaxLBA() )
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


//if current PSN near the Seek PSN, return successful without real seek.
    if( (DVD_BUFFERING() == ON)&&(((StartLBA.Long > DataBuffer.CacheEndLBA)&&(StartLBA.Long < (DataBuffer.CacheEndLBA+2*ONE_DVD_BLOCK)) )||( (StartLBA.Long <= DataBuffer.CacheEndLBA)&&(StartLBA.Long >= DataBuffer.CacheStartLBA) )))
    {
        CmdSeekOk = 0; //without real seek.
        StartTimer(&CmdSeekTimer);
        BUILD_SENSE(0x00,0x00,0x00,0x02);       // "NO ADDITIONAL SENSE INFORMATION" Seek Complete
    }
    else
    {
        // var SeekLayer now contains correct layer seek PSN resides on.
        // var SeekPsn   now contains correct       seek PSN

        /*------------------------------------------------------------------
        After get a dvd seek command
        if servo seek ok   ==> start timer  (5sec do nothing than call pause in HIFStart())
           servo seek fail  ==> call pause
        -------------------------------------------------------------------*/
        CmdSeekOk = 0;


        if( (dvdSeek(StartPSN.PSN , StartPSN.Layer)) == Error)
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
    Function:       dvdSeek
    Description:    Issues a DVD seek and sets up player mode
                    if seek is good else issues a reseek until
                    retries are exhuasted

    Returns:        Error or Ready

******************************************************************/
BYTE  dvdSeek(ULONG TargetID, BYTE Layer)
{
BYTE retry_cnt ;
ULONG retry_timer;
#if (OPEN_SESSION == 1)
BYTE pre_rf_search_count,temp_rf_search_count;
#endif

    StartTimer(&retry_timer);

    if( DVD_BUFFERING() == ON )
        dvdStopDiscTransfer();

    if(SpeedMD == StartUp_f)
        retry_cnt = 0;
    else
        retry_cnt = 5;

#if (OPEN_SESSION == 1)
    temp_rf_search_count=0;
#endif

    while(1)
    {
        if(EjectKeyInfo.PressedValid||TrayManagerInfo.Executing)
        {
//            BUILD_SENSE(0x02,0x3A,0x00,0x03);            /* MEDIUM NOT PRESENT */
//            ProcessEjectRequest();                     //TBD
            SendMsgCn(SEND_ATAPI_CMD,1,0x800005, FALSE);   //Seek End, Success=%u
            return(Error);
        }

        if(ReadTimer(&retry_timer) >= 3000)
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
            
    #if (EN_ADEQ == 1)//Disable ADEQ when random access, for ipeak
            if ((pdbGetDiscKind() == eDVDMinusRDLDisc) && (Disable_ADEQ == 0))
            {
                Disable_ADEQ |= 0x80;
            }
    #endif
        }
    }
    else
    {   //if any sequential read coming

        if(SpeedMD == Special_for_SeekSpeed)
        {
            if(HostXfer.BlockCount.Long == 0x19)
            {
                SpeedMD = Special_for_CDSpeed_f;
                SpeedCount = DVD_SPEED_UP_READY -0x200;
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

#if (OPEN_SESSION == 1)
        pre_rf_search_count = RF_search_count;
#endif

        send_msg80(SEND_ATAPI_CMD,0x800001);   //Seek Start
        if (ServoSeek(TargetID, Layer) == PASS)
        {
             /* Seek DVD was successful */
            Svr_pause_reqire = FALSE;               /* Clear pause request flag */
            Iram_Svr_player_mode = READING_MODE;    /* Change to reading_mode   */
#if (DVD_RAM_READ == 1) && (WORKAROUND_FOR_SEEDMISTHR == 1)
            if (pdbGetDVDType() == DVD_RAM)
            {
                if (ResetSeedMisThr == TRUE && RestSeedCount >= 5)
                {
                    ResetSeedMisThr = FALSE;
                    RestSeedCount = 0;
                    WRITE_FIELD(HAL_DVDD_SEEDMISTHR,2);
                }
                else
                {
                WRITE_FIELD(HAL_DVDD_SEEDMISTHR,0);
                }
            }
#endif
            SendMsgCn(SEND_ATAPI_CMD,1,0x800005, TRUE);   //Seek End, Success=%u
            return(Ready);
        }
        else
        {
             /* DVD seek failed */
#if (OPEN_SESSION == 1)
            if(RF_search_count != pre_rf_search_count)
            {
                temp_rf_search_count++;
            }
#endif
            if (retry_cnt--  == 0)  /* Update retry count and if exhausted return error */
            {
#if (OPEN_SESSION == 1)
                if (svoVar_TrackingMethod == TRK_METHOD_DPD)
                {
                    if(temp_rf_search_count<2)
                    {
                        SpeedCount = SPEED_DOWN_FOR_SEEK_FAIL;    // speed down
                    }
                }
                else
#endif
                {
                    SpeedCount = SPEED_DOWN_FOR_SEEK_FAIL;    // speed down
                }
                SendMsgCn(SEND_ATAPI_CMD,1,0x800005, FALSE);   //Seek End, Success=%u
                return(Error);
            }
        }

#if 0//DvdRamFastCache
        if(RWDisc.dvRAM.BgPreBuffer == 1)
        {
            if(ReadTimer(&BgPreBufTimer) > 50 || RWDisc.dvRAM.BgPreBufAbort)
            {
                SendMsgCn(SEND_ATAPI_CMD,1,0x800005, FALSE);   //Seek End, Success=%u
                return(Error);
            }
        }
#endif

        ExitProcess();
    } /* end of While */
}


