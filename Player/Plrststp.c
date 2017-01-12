/****************************************************************************
*                (c) Copyright 2001 - 2008 SUNEXT DESIGN, INC.
*                           All Right Reserved
*
* FILENAME: Plrststp.c
*
* $Revision: 251 $
* $Date: 11/03/16 6:00p $
*
* DESCRIPTION   : This module is responsible for starting up the disc or stopping the disc.
*                 If the disc must be started the TOC or PMA is read.
*
**************************** SUNEXT CONFIDENTIAL *******************************/

/*************************************************************************/
/*                           Include Files                               */
/*************************************************************************/
#include ".\common\globals.h"
#include ".\common\globtype.h"
#include ".\common\chip.h"
#include ".\common\commath.h"
#include ".\common\error.h"
#include ".\servo\svo.h"
#if (ENABLE_HDDVD == 1)
#include ".\hd\hdDB.h"
#endif
#include ".\player\plrDB.h"
#include ".\player\plrStStp.h"
#include ".\player\plrInfo.h"
#include ".\player\plrMisc.h"
#include ".\player\plrWatch.h"
#include ".\player\plrServo.h"
#include ".\servo\svo_com.h"
#include ".\servo\SvoVar.H"
#include ".\servo\Svo_lib.H"
#include ".\servo\svo_cmd.h"
#if (BD_ENABLE == 1)
#include ".\bd\bdDB.h"
#endif
#include ".\oem\oem_spd_tbl.h"

#if (EN_POWER_CONTROL)
#include ".\hif\powerctl.h"
#endif
/*************************************************************************/
/*                              Define                                   */
/*************************************************************************/

/* Max */
#define eMax_JmpRtyCnt     2 /* Jump Retry Count1      */
#define eMax_EFMRtyCnt     10 /* EFM  Retry Count      */
#define eMax_DiscRtyCnt    5 /* Disc Retry Count       */
#define eMax_SrchTocErrCnt 3 /* Search Toc Error Count */

#define MAX_TOCRETRY       1    /* TOC max retry count */
private BYTE  NeedTocRetry;

#define DVD_PLUS_INFO_START_ADDR   0x2A480L

extern void cssInit(void);

/*************************************************************************/
/*                         Internal Functions                            */
/*************************************************************************/
private BYTE StartUpDisc(void);
private BYTE StUD_01(void); private BYTE StUD_02(void);
private BYTE StUD_03(void); private BYTE StUD_04(void);
private BYTE StUD_05(void); private BYTE StUD_06(void);
private BYTE StUD_07(void); private BYTE StUD_08(void);
private BYTE StUD_09(void);
private BYTE StUD_11(void); private BYTE StUD_12(void);
private BYTE StUD_13(void); private BYTE StUD_14(void);
private BYTE StUD_15(void); private BYTE StUD_16(void);
private BYTE StUD_17(void); private BYTE StUD_18(void);
private BYTE StUD_19(void); private BYTE StUD_19E(void);
private BYTE StUD_20(void);
private BYTE StUD_21(void); private BYTE StUD_21E(void);
private BYTE StUD_22(void); private BYTE StUD_23(void);
private BYTE StUD_24(void); private BYTE StUD_24E(void);
private BYTE StUD_25(void);

private BYTE StUD_MiscRtyCnt(void);   private BYTE StUD_ServoRtyCnt(void);
private BYTE StUD_TocRtyCnt(void);    private BYTE StUD_DiscRetryCnt(void);
private BYTE StUD_SearchLeadIn(void);
private BYTE StopDisc(void);
private BYTE StpD_01(void);

private BYTE ErrorEnd(void);

/*************************************************************************/
/*                         Globle  Variables                             */
/*************************************************************************/
private TimeS fJumpTime;      /* Jump Time              */
private TimeS fJumpTocTime;   /* Jump to read TOC Time  */
private BYTE  fJmpRtyCnt;     /* Jump Retry Count       */
BYTE          fTocRtyCnt;     /* Toc  Retry Count       */
private BYTE  fEFMRtyCnt;     /* EFM  Retry Count       */
private BYTE  fDiscRtyCnt;    /* Disc Retry Count       */
private BYTE  fSrchTocErrCnt; /* Search Toc Error Count */
private BYTE  fSeekRtyCnt;    /* Seek Error Count       */
//private ULONG readTOC_Time;
ULONG         watchQ_Time;

BYTE    fSeekNotReachCnt;
//BYTE    RetryPMA;
TimeS preB0Time;

#if(Detect_AVCHD_DISC == 1)
extern BYTE AVCHDFlag;
#endif

/* Start Up Disc */
private const tTask fStUD[] = {
/*          Ready   Error            */
/* Ph00 */ {0      ,0                   },
/* Ph01 */ {StUD_01,ErrorEnd            },
/* Ph02 */ {StUD_02,ErrorEnd            },
/* Ph03 */ {StUD_03,ErrorEnd            },
/* Ph04 */ {StUD_04,StUD_MiscRtyCnt     },
/* Ph05 */ {StUD_05,0                   },
/* Ph06 */ {StUD_06,StUD_MiscRtyCnt     },
/* Ph07 */ {StUD_07,StUD_06             },
/* Ph08 */ {StUD_08,StUD_TocRtyCnt      },
/* Ph09 */ {StUD_09,StUD_MiscRtyCnt     },
/* Ph10 */ {0      ,0                   },
/* Ph11 */ {StUD_11,StUD_ServoRtyCnt    },
/* Ph12 */ {StUD_12,StUD_DiscRetryCnt   },
/* Ph13 */ {StUD_13,StUD_MiscRtyCnt     },
/* Ph14 */ {StUD_14,StUD_SearchLeadIn   },
/* Ph15 */ {StUD_15,ErrorEnd            },
/* Ph16 */ {StUD_16,ErrorEnd            },
/* Ph17 */ {StUD_17,StUD_SearchLeadIn   },
/* Ph18 */ {StUD_18,StUD_MiscRtyCnt     },
/* Ph19 */ {StUD_19,StUD_19E            },
/* Ph20 */ {StUD_20,StUD_MiscRtyCnt     },
/* Ph21 */ {StUD_21,StUD_21E            },
/* Ph22 */ {StUD_22,StUD_DiscRetryCnt   },
/* Ph23 */ {StUD_23,ErrorEnd            },
/* Ph24 */ {StUD_24,StUD_24E            },
/* Ph25 */ {StUD_25,0  },
};

/* Stop Disc */
private const tTask fStpD[] = {
/*          Ready  Error    */
/* Ph00 */ {0      ,0       },
/* Ph01 */ {StpD_01,ErrorEnd},
};

/* Command Table */
public const tCmd fSStpTask[] = {
    {(LPCHAR) "Idle",0          ,0    }, /* Idle    */
    {(LPCHAR) "Stup",StartUpDisc,fStUD}, /* Startup */
    {(LPCHAR) "Stop",StopDisc   ,fStpD}, /* Stop    */
};

extern BYTE TempTOCA0, TempTOCA1, TempTOCA2[3];
extern BYTE TOCtemp[99][4];

/**************************************************************************
// Name        : plrStrtstpSetIdle
// Description : This function will initialize the plrStrtstp state machine
//               to idle mode.
***************************************************************************/
extern BYTE Current_read_sp;
public void plrStrtstpSetIdle(void)
{
    /* Variable Initiaize */
    plrTaskCmdReq(eMsgID_SStp,eSStpIdle);
    plrTaskClr(eMsgID_SStp);

    Current_read_sp = eSpStop;
}

/************************************************************************
// Name        : ErrorEnd
// Description : This is a continuous process Error End.
*************************************************************************/
private BYTE ErrorEnd()
{
    /* Error */
    return(Error);
}

/************************************************************************
// Name        : StartUpDisc
// Description : This function determines the procedure in starting up.
*************************************************************************/
#if(DeadLoop_Solution==1)
extern SBYTE svo_ERR_STARTUP;
#endif
private BYTE StartUpDisc(void)
{
    /* Variables Set*/
    fJmpRtyCnt = 0;
    fTocRtyCnt = 0;
    fSrchTocErrCnt = 0;
    fSeekRtyCnt = 0;
    pdbSetPlayerStatus(eStStartingUp);
/* Recovery the Power Mode */
#if (EN_POWER_CONTROL)
    /* Recovery the Power Mode */
    if (St_drv_ready  == TRUE)
    {
        if (CurrentPowerMode == eStandbyPower)
        {   /* There is disc inside and the related information is collected. */
            /* for wake up the power from stand by mode */
            SetDiscReadPowerMode();
        }
    }
    else
    {
    #if (EN_POWER_CONTROL == 1)
        PowerControl(eStartUpPower);
    #endif
    }
#endif

    if (fRawModeWritten == TRUE){
        plrMeReq(ePhase05);
        return(Busy);
    }

#if 0//(DeadLoop_Solution==1)
    if(svo_ERR_STARTUP == TRUE){
         svo_ERR_STARTUP = FALSE;
         plrMeReq(ePhase23);
    } else
#endif
    plrMeReq(ePhase01);

    return(Busy);
}


/*------------------------------------------------------------------------
// Phase01(From Cmd)-Set Speed
-------------------------------------------------------------------------*/
private BYTE StUD_01()
{
    tWatchSC ServoCmd;  /* Servo Command */
    char i;

    fSeekRtyCnt = 0;

    /* temp variables for fGetTOCState != eDECODE_TOC_DONE */
    /* will use them to make up a complete TOC when time out */
    TempTOCA0 = 0;
    TempTOCA1 = 0;
    TempTOCA2[0] = 0;
    TempTOCA2[1] = 0;
    TempTOCA2[2] = 0;

    for (i = (MaxNumberOfTracks-1) ; i >= 0; i--)
    {
        TOCtemp[i][0] = 0xff;
        TOCtemp[i][1] = 0xff;
        TOCtemp[i][2] = 0xff;
        TOCtemp[i][3] = 0xff;
    }

    if ((St_cmd_inproc == TRUE) && (Iram_Cmd_block.bf[0] == 0x1B)){   // 0x1B: START_STOP_UNIT Cmd
        svoIramVar.knlTimeupCount = 600;    // 12sec
    }

    switch (svoVar_discKindT.Bit.DiscStd)
    {
        case eDiscCD:
            svoVar_svoTargetSpeed = SVO_CAL_SPEED_CD;
            break;

        case eDiscDVD:
#if (DVD_RAM_READ == 1)
            if(pdbGetDiscKind() == eDVDRAMDisc)
                svoVar_svoTargetSpeed = SVO_CAL_SPEED_DVDRAM;
            else
#endif
            svoVar_svoTargetSpeed = SVO_CAL_SPEED_DVD;
            break;
#if (BD_ENABLE == 1)
        case eDiscBD:
            svoVar_svoTargetSpeed = SVO_CAL_SPEED_BD;
            break;
#endif
    }

    /* Servo Watch */
    ServoCmd.Cmd = svoCmdStart;
    ServoCmd.Len = 0;
    plrWatchReq(ePhaseInc,eWatch_ServoReadyStatus,(BYTE *) &ServoCmd);

    return(Busy);
}

/*------------------------------------------------------------------------
// Phase02(From Ph01)-Set Spinning up
-------------------------------------------------------------------------*/
private BYTE StUD_02()
{
//    tWatchSC ServoCmd;  /* Servo Command */

    /* set actual speed to spin up speed( N = 2) */
    /* Servo Watch */

//    ServoCmd.Len = 0;

    /* For SEEK TEST */
//        plrWatchReq(ePhaseInc,eWatch_ServoStatus,(BYTE *) &ServoCmd);

    plrMeReq(ePhaseInc);
    return(Busy);
}

/*------------------------------------------------------------------------
// Phase03(From Ph02)-Request Setting Info
-------------------------------------------------------------------------*/
private BYTE StUD_03()
{
    tWatchSC ServoCmd;  /* Servo Command */

    /* Servo Watch */
    ServoCmd.Cmd = svoCmdGetStatusInformation;
    ServoCmd.Len = 0;
    plrWatchReq(ePhaseInc,eWatch_ServoStatus,(BYTE *) &ServoCmd);
    return(Busy);
}

/*------------------------------------------------------------------------
// Phase04(From Ph03,Ph04,Ph23)-Request Status Info
-------------------------------------------------------------------------*/
private BYTE StUD_04()
{
    StWordU DK;

    /* Initial the Next session Start Time that get from B0 as 00:00.00
        For protect the current B0 is the same as previos B0 in function ReadTOCDecode()*/
    mTimeSet(preB0Time,0,0,0);

    plrGetServoStatusInfo();
    DK.Uword = pdbGetDiscKind();
    SendMsg80(SEND_READ_DVD_MSG,0x5C0000+pdbGetDiscKind()); // Disc_Kind,XXX
    SendMsg80(SEND_READ_DVD_MSG,0xDD0200+pdbServoGetDiscDiameter()); // Disc_Diamete,XXX
    SendMsg80(SEND_READ_DVD_MSG,0xDD0300+pdbServoGetDiscType()); // Disc_Type,XXX

#if (TEMP_FINISH_TRON_AFTER_TRKSVO_ON == 1)
    plrMeReq(ePhase21);
    return(Busy);
#endif

#if (EN_LS == 1)
    if(pdbGetDiscKind() == eLSDisc )
    {
        plrMeReq(ePhase21);
    }
    else
#endif // End of (EN_LS == 1)
    /* get Disc Type */
    if (pdbServoGetDiscType() == (eDiscType)eUnknownType)
    {
        /* get Disc Diameter */
        if (pdbServoGetDiscDiameter() == (eDiscDiameter)eNoDisc)
        {
            plrMeReq(ePhase21);
        }
        else
        {
            return(Error);
        }
    }
    else
    {
#if (USE_BS_DEFECT_FOR_STARTUP==1)
        Defect_Enable(DEFECT_SOURCE_UNFREEZE);
#endif
        if(pdbGetServoReadyStatus() == TRUE)
        {
            switch(pdbDiscLoaded())
            {
#if (BD_ENABLE)
                case BD_DISC:
                    plrMeReq(ePhase25);
                    break;
#endif
#if (ENABLE_HDDVD)
                case HD_DISC:
#endif
                case DVD_DISC:
#if (STARTUP_AT_LOWEST_CAV == 1)//[W.0Q]Modify DVD+R startup set speed issue from STAND BY mode.
                    {
                        if(pdbDiscLoaded() == DVD_DISC)
                        {
                    #if (DVD_RAM_READ == 1)
                            if(pdbGetDiscKind() == eDVDRAMDisc)
                            {
                                svoVar_svoTargetSpeed = SVO_CAL_SPEED_DVDRAM;
                                spdChangeDiscSpeed(SVO_CAL_SPEED_DVDRAM);     /* Set to Lowest CLV speed */
                                Current_read_sp = eDVDRAMIdleSpeed;
                                SpeedMD = AutoMode;
                            }
                            else
                    #endif
                            {
                                /* Set to Lowest CAV speed */
                    #if((DVD_710Run815 == 1) && (DVD_710Run815New == 0))
                        #if (Detect_AVCHD_DISC == 1)
                                if (AVCHDFlag == TRUE)
                                {
                                    if(svoVar_svoTargetSpeed != MAX_READ_AVCHD_SPEED)
                                    {
                                        //send_msg5S(1, "+Chg4");
                                        plrServoSetSpeed(MAX_READ_AVCHD_SPEED,ePhase25);
                                        return(Busy);
                                    }
                                }
                                else
                        #endif
                                {
                                    if((pdbGetDVDType() == DVD_PLUS_R)||(pdbGetDVDType() == DVD_PLUS_RW)||(pdbGetDVDType() == DVD_PLUS_R_DUAL))
                                    {
                                        if(svoVar_svoTargetSpeed != e3XDCLV)
                                        {
                                            //send_msg5S(1, "+Chg24");
                                            plrServoSetSpeed(eDVDSpinUpSpeed,ePhase25);
                                            return(Busy);
                                        }
                                    }
                                    else
                                    {
                                        if(svoVar_svoTargetSpeed != eDVDSpinUpSpeed)
                                        {
                                            //send_msg5S(1, "-Chg2");
                                            plrServoSetSpeed(eDVDSpinUpSpeed,ePhase25);
                                            return(Busy);
                                        }
                                    }
                                }
                    #else
                        #if (Detect_AVCHD_DISC == 1)
                                if (AVCHDFlag == TRUE)
                                {
                                    if(svoVar_svoTargetSpeed != MAX_READ_AVCHD_SPEED)
                                    {
                                        //send_msg5S(1, "+Chg4");
                                        plrServoSetSpeed(MAX_READ_AVCHD_SPEED,ePhase25);
                                        return(Busy);
                                    }
                                }
                                else
                        #endif
                                {
                                    #if (DVD_710Run815New == 0)
                                    svoVar_svoTargetSpeed = MIN_CAV_SPEED_DVD;
                                    spdChangeDiscSpeed(MIN_CAV_SPEED_DVD);
                                    Current_read_sp = eDVDIdleSpeed;
                                    SpeedMD = AutoMode;
                                    #else
                                    svoVar_svoTargetSpeed = eDVDIdleSpeed;
                                    spdChangeDiscSpeed(eDVDIdleSpeed);
                                    Current_read_sp = eDVDIdleSpeed;
                                    SpeedMD = AutoMode;
                                    #endif
                                }
                    #endif
                            }
                        }
                    }
#endif
                    plrMeReq(ePhase25);
                    break;
                case CD_DISC:
                    plrMeReq(ePhase21);
                    break;
            }
        }
        else
        {
            switch(pdbDiscLoaded())
            {
#if (BD_ENABLE)
                case BD_DISC:
                    pdbInitBDDataBase();
                    plrMeReq(ePhase25);
                    break;
#endif

#if (ENABLE_HDDVD)
                case HD_DISC:
                    pdbInitHdDvdDataBase();
                    plrServoSeek(HD_CONTROL_DATA_1_START_ADDRESS, eWLayer0, ePhase25);
                    break;
#endif

                case DVD_DISC:
                    pdbInitDvdDataBase();
                    fJmpRtyCnt = 0;
                    fSeekRtyCnt = 0;
                    fDiscRtyCnt = 0;
#if (DETECT_DVD_CLV_TRACK_PITCH == 1)
                    if ((pdbGetDiscKind() == eDVDROMDisc)||(pdbGetDiscKind() == eDVDROMDLDisc))
                    {
                        plrServoSeek(DATA_START_PSN_DVDROM, eWLayer0, ePhase24);
                    }
                    else if (pdbGetDiscKind() == eDVDRAMDisc)
                    {
                        plrServoSeek(START_ADDRESS_OF_CONTROL_DATA, eWLayer0, ePhase25);
                    }
#else
                    if ((pdbGetDiscKind() == eDVDRAMDisc)||(pdbGetDiscKind() == eDVDROMDisc))
                    {
                        plrServoSeek(START_ADDRESS_OF_CONTROL_DATA, eWLayer0, ePhase25);
                    }
#endif
                    else
                    {
#if (LPP_AUTO_LEARN_TEST==1)
                        if ( pdbServoGetDiscType() == (eDiscType)eLPPDisc)
                        {
                            if(pdbGetDiscKind() == eDVDMinusRWDisc)
                            {
                                svoVar_optLPPTH3 = MRWDEF_HDRTHRSH3;
                                svoVar_optLPPTH4 = MRWDEF_HDRTHRSH4;
                            }
                            else if(pdbGetDiscKind() == eDVDMinusRDLDisc)
                            {
                                svoVar_optLPPTH3 = MRDLDEF_HDRTHRSH3;
                                svoVar_optLPPTH4 = MRDLDEF_HDRTHRSH4;
                            }
                            else//eDVDMinusRDisc
                            {
                                if(pdbGetSpeed() == e8XDPCAV)
                                {

                                }
                                else
                                {
                                    svoVar_optLPPTH3 = MRDEF_HDRTHRSH3;
                                    svoVar_optLPPTH4 = MRDEF_HDRTHRSH4;
                                }
                            }
                        }
#endif  //end of LPP_AUTO_LEARN_TEST
                        plrServoSeek(START_ADDRESS_OF_CONTROL_DATA, eWLayer0, ePhase25);
                    }
                    break;

                case CD_DISC:
                    plrTaskInit(eMsgID_Info);
                    fEFMRtyCnt = 0;
                    fDiscRtyCnt= 0;
                    /* Sub Code Ok Watch */
                    StartTimer(&watchQ_Time);
                    plrWatchReq(ePhaseInc,eWatch_SubCodeOk,0);
                    break;
            }
        }
    }
    return(Busy);
}

/*------------------------------------------------------------------------
// Phase05(From Ph04,Ph11,Disc Retry)-Check EFM
-------------------------------------------------------------------------*/
private BYTE StUD_05()
{
    SubQCodeS SubCode;
    LONG      tmpLBA;
#if (OPEN_SESSION==1)
    tWatchSC ServoCmd;  /* Servo Command */
    SHORT    Number;
#endif

    if (pdbDiscLoaded() == DVD_DISC)
    {
        plrMeReq(ePhase25);         /* End of phase */
        return(Busy);
    }

    SIF_CMD_MESSAGE(PRN_HIF_TO_SIF,0);
    if(get_SIF(EXECUTIONSTATE) == svoBusy)
        set_SIF(EXECUTIONSTATE,svoReady);
    svoCmdVar.svoCommand = CNOP;
    set_SIF(COMMAND,svoCmdIdle);        /* command clr */

    if( (!(svoCalVar.discChkDoneBits & velo_chk_end))&&(fJmpRtyCnt < (eMax_JmpRtyCnt*2)) )
    {
        Defect_Enable(DEFECT_SOURCE_FREEZE_BS);//set this whatever "USE_BS_DEFECT_FOR_STARTUP" is

#if (OPEN_SESSION==1)
        if (ReadTimer(&watchQ_Time) >= 50)
        {
            fJmpRtyCnt++;
            SendMsgCn(SEND_SStpTask_MSG,2,0x800494, 1, fJmpRtyCnt);

            Number = -100*fJmpRtyCnt;
            /* Servo Watch - Misc Status issue JUMP time to NextSessionTime command to servo */
            ServoCmd.Cmd = svoCmdJumpGrooves;
            ServoCmd.Len = 3;
            ServoCmd.Para[0] = (BYTE) Number;
            ServoCmd.Para[1] = (BYTE)(Number >> 8);
            ServoCmd.Para[2] = eTimeJump;
            plrWatchReq(ePhase03,eWatch_ServoStatus,(BYTE *) &ServoCmd);
            return(Busy);
        }
#endif
        mTimeSet(fJumpTime,0,5,0);
        tmpLBA = MSF2LBA(&fJumpTime);
        plrMiscReq(ePhase20,eMiscTime,tmpLBA);
        return(Busy);
    }
    else if(fJmpRtyCnt < (eMax_JmpRtyCnt*2))
    {
#if (OPEN_SESSION==1)
        if (ReadTimer(&watchQ_Time) >= 50)
        {
            fJmpRtyCnt++;
            SendMsgCn(SEND_SStpTask_MSG,2,0x800494, 4, fJmpRtyCnt);

            Number = -100*fJmpRtyCnt;
            /* Servo Watch - Misc Status issue JUMP time to NextSessionTime command to servo */
            ServoCmd.Cmd = svoCmdJumpGrooves;
            ServoCmd.Len = 3;
            ServoCmd.Para[0] = (BYTE) Number;
            ServoCmd.Para[1] = (BYTE)(Number >> 8);
            ServoCmd.Para[2] = eTimeJump;
            plrWatchReq(ePhase03,eWatch_ServoStatus,(BYTE *) &ServoCmd);
            return(Busy);
        }
#endif
    }
    Defect_Enable(DEFECT_SOURCE_UNFREEZE);//set this whatever "USE_BS_DEFECT_FOR_STARTUP" is

    /*****************************    FW Bug    ******************************/
    /* If the non-ATIP disc is recognized as ATIP disc, it will loop between phase5 and
        phase11 because of the pdbGetATIPTimeBin() is smaller than 97:48.0. */
    if( fSeekRtyCnt > 4 )
        return(Error);
    /**************************** End FW Bug  ******************************/

    NeedTocRetry = 0;
    fRawModeWritten = FALSE;
    /* get current Tno BYTE of Subcode from plrdb.c */
    pdbGetSubCode(&SubCode);
    //Seek to Lead-in
    pdbSetSessionCounter( 0 );
    mTimeSet(fJumpTime,98,20,0);
    fJumpTocTime = fJumpTime;
    tmpLBA = MSF2LBA(&fJumpTime);
    plrMiscReq(ePhase07,eMiscTime,tmpLBA);
    fJmpRtyCnt = 0;

    return(Busy);
}

/*------------------------------------------------------------------------
// Phase06(From Ph05,Toc Retry)-Pre Jump TOC
-------------------------------------------------------------------------*/
private BYTE StUD_06()
{
    LONG      tmpLBA;
    BYTE      jump_gap;

    SIF_CMD_MESSAGE(PRN_HIF_TO_SIF,0);
    if(get_SIF(EXECUTIONSTATE) == svoBusy)
        set_SIF(EXECUTIONSTATE,svoReady);
    svoCmdVar.svoCommand = CNOP;
    set_SIF(COMMAND,svoCmdIdle);        /* command clr */

    //Use retry seek to Lead-in
    fJumpTime = fJumpTocTime;
    if(pdbGetSessionCounter() == 0)
        jump_gap = 20;
    else
        jump_gap = 15;

    fJmpRtyCnt++;

    if((fJmpRtyCnt < eMax_JmpRtyCnt)||((pdbGetSessionCounter() == 0)&&(fJmpRtyCnt<(eMax_JmpRtyCnt*2))))
    {
        mathAddAddressToMSF(&fJumpTime,jump_gap*75*fJmpRtyCnt,&fJumpTime);//+jump 15 or 20 sec
        tmpLBA = MSF2LBA(&fJumpTime);
        plrMiscReq(ePhase07,eMiscTime,tmpLBA);
        return(Busy);
     }
     else
     {
        if(pdbGetSessionCounter()>0)
        {
            pdbSetReadInfoMethod(eReadTOC);

            if(pdbServoGetDiscType() == (eDiscType)eATIPDisc)
            {//Force to set finalized Disc
                pdbSetDiscStatus(eFinalizedDisc);
                pdbSetFinalSessCloseInfo(eSessionFinalized);
            }

            /* Misc Request(data track, Seek to 0:5:0) */
            mTimeSet(fJumpTime,0,5,0);
            tmpLBA = MSF2LBA(&fJumpTime);
            plrMiscReq(ePhase21,eMiscTime,tmpLBA);
            fJmpRtyCnt = 0;
            return(Busy);
        }
#if (DISC_MIS_JUDGE_START_UP_RECOVERY)
        RetryDiscLaser = eDiscUnknown;//Don't need to try next Laser
#endif
        return(Error);
     }
}

/*------------------------------------------------------------------------
// Phase07(From Ph06)- Jump First Leadin
-------------------------------------------------------------------------*/
private BYTE StUD_07()
{
    SIF_CMD_MESSAGE(PRN_HIF_TO_SIF,0);
    if(get_SIF(EXECUTIONSTATE) == svoBusy)
        set_SIF(EXECUTIONSTATE,svoReady);
    svoCmdVar.svoCommand = CNOP;
    set_SIF(COMMAND,svoCmdIdle);        /* command clr */

    /* Read TOC */
    plrInfoReq(ePhaseInc,eInfoReadTOC);
    return(Busy);
}

/*------------------------------------------------------------------------
// Phase08(From Ph05,Ph07,Ph10,Ph19)- Read TOC
-------------------------------------------------------------------------*/
private BYTE StUD_08()
{
    LONG    tmpLBA;
    ULONG   jump_gap;

    SIF_CMD_MESSAGE(PRN_HIF_TO_SIF,0);
    if(get_SIF(EXECUTIONSTATE) == svoBusy)
        set_SIF(EXECUTIONSTATE,svoReady);
    svoCmdVar.svoCommand = CNOP;
    set_SIF(COMMAND,svoCmdIdle);        /* command clr */

    fJmpRtyCnt = 0;
    /* get next session status */
    if(pdbGetNextSessionAvailable() == TRUE){
        /* get next session start time */
        fJumpTime = pdbGetNextSessStartTime();
        NeedTocRetry = 0;
        if(fJumpTime.Minute == 0xFF){
            /* Misc Request(data track, Seek to 0:2:0) */
            pdbSetNextSessionAvailable(FALSE);
            mTimeSet(fJumpTime,0,5,0);
            tmpLBA = MSF2LBA(&fJumpTime);
            fJmpRtyCnt = 0;
            plrMiscReq(ePhase21,eMiscTime,tmpLBA);
            return(Busy);
        }
        else{
            /* set next toc time to next session start time + 0:4:0 */
            /* Change start time from +0:2:0 to + 0:4:0. To prevent  following the gap area. */
            mathAddAddressToMSF(&fJumpTime,300,&fJumpTime);
        }

        mathSubtractAddressFromMSF(&fJumpTime,4500,&fJumpTime); /* -60 sec = 4500 frames */
        fJumpTocTime = fJumpTime;
        tmpLBA = MSF2LBA_Plus(&fJumpTime);
        fJmpRtyCnt = 0;
        plrMiscReq(ePhase07,eMiscTime,tmpLBA);
    }//if(pdbGetNextSessionAvailable() == TRUE)
    else {

        fJumpTime = pdbGetNextSessStartTime();
        if (fJumpTime.Minute != 0xFF)
        {
            if (++NeedTocRetry < MAX_TOCRETRY)
            {
                pdbSetNextSessionAvailable(TRUE);

                SendMsgCn(SEND_SStpTask_MSG,1,0x800495, NeedTocRetry);
                mathAddAddressToMSF(&fJumpTime,300,&fJumpTime);         /* +4 sec */
                mathSubtractAddressFromMSF(&fJumpTime,4500,&fJumpTime); /* -60 sec = 4500 frames */
                jump_gap = (60/(MAX_TOCRETRY+1))*75*NeedTocRetry;       /* jump_gap = (60/(MAX_TOCRETRY+1))*NeedTocRetry sec */
                mathAddAddressToMSF(&fJumpTime,jump_gap,&fJumpTime);    /* +(jump_gap/75) sec */
                fJumpTocTime = fJumpTime;
                tmpLBA = MSF2LBA_Plus(&fJumpTime);
                fJmpRtyCnt = 0;
                plrMiscReq(ePhase07,eMiscTime,tmpLBA);
                return(Busy);
            }
        }

        /* Single session disc */
        pdbSetReadInfoMethod(eReadTOC);

        if( (pdbServoGetDiscType() == (eDiscType)eATIPDisc)&&(pdbGetSessionCounter()) )
        {//Force to set finalized Disc
            pdbSetDiscStatus(eFinalizedDisc);
            pdbSetFinalSessCloseInfo(eSessionFinalized);
        }

        /* Misc Request(data track, Seek to 0:2:0) */
        mTimeSet(fJumpTime,0,5,0);
        tmpLBA = MSF2LBA(&fJumpTime);
        fJmpRtyCnt = 0;
        plrMiscReq(ePhase21,eMiscTime,tmpLBA);
    }
    return(Busy);
}

/*------------------------------------------------------------------------
// Phase09(From Ph08)-Seek Next Session Leadin
-------------------------------------------------------------------------*/
private BYTE StUD_09()
{
//    tWatchSC ServoCmd;  /* Servo Command */
//    SHORT Number = -110;
//
//    /* Servo Watch -
//       Misc Status issue JUMP time to NextSessionTime command to servo */
//    ServoCmd.Cmd = svoCmdJumpGrooves;
//    ServoCmd.Len = 3;
//    ServoCmd.Para[0] = (BYTE) Number;
//    ServoCmd.Para[1] = (BYTE)(Number >> 8);
//    ServoCmd.Para[2] = eTimeJump;
//    plrWatchReq(ePhase07,eWatch_ServoStatus,(BYTE *) &ServoCmd);
    return(Busy);
}

/*------------------------------------------------------------------------
// Phase11(From Ph05)-Search for leadin
-------------------------------------------------------------------------*/
private BYTE StUD_11()
{
//    /* Misc Status  issue JUMP time to 99:0:0 command to servo */
//    fEFMRtyCnt = 0;
//    fSeekRtyCnt++;
//    /* Me Request(Dummy) */
//    plrMeReq(ePhase05);
    return(Busy);
}

/*------------------------------------------------------------------------
// Phase12(From Ph05)-Read Disc Info
-------------------------------------------------------------------------*/

private BYTE StUD_12()
{
//LONG    tmpLBA;
//
///* H/W debug */
//    fJmpRtyCnt = 0;
//    /*issue JUMP time to PMA start time - 0:0:10 command to servo */
//    fJumpTime = pdbGetStartTimePMA();
//    /* Misc Request(Jump PMA ) */
//    tmpLBA = MSF2LBA(&fJumpTime);
//    plrMiscReq(ePhaseInc,eMiscTime,tmpLBA);
    return(Busy);
///* H/W debug */
}

/*------------------------------------------------------------------------
// Phase13(From Ph12)-Jump PMA Start Time
-------------------------------------------------------------------------*/
private BYTE StUD_13()
{
//    /* Read PMA */
//    plrInfoReq(ePhaseInc,eInfoReadPMA);
    return(Busy);
}

//extern BYTE fMode0Count;
/*------------------------------------------------------------------------
// Phase14(From Ph13)-Read PMA
-------------------------------------------------------------------------*/
private BYTE StUD_14()
{
//    LONG    tmpLBA;
//
//        /* To recognize the CDR/W as blank disc property */
//    if((pdbGetPMAEmptyStatus() == ePMANotEmpty)&&(pdbGetHighestRecordedTrkNumber()>0)){
//        if(pdbGetLowestRecordedTrackNumber()> 1){//first track number should be 1
//             RetryPMA++;
//             if(RetryPMA<6)
//             {
//                plrMeReq(ePhase12);
//                return(Busy);
//            }
//        }
//
//       //recover to the original setting in svo_afe_field_tbl_4LX[]
//        pdbSetReadInfoMethod(eReadPMA);
//        /* Sort PMA */
//        plrInfoReq(ePhaseInc,eInfoSortPMA);
//    }
//    else {//PMA empty
//        RetryPMA++;
//        if(RetryPMA<6)
//        {
//            plrMeReq(ePhase12);
//            return(Busy);
//        }
//       //recover to the original setting in svo_afe_field_tbl_4LX[]
//        /* To recognize the CDR/W as blank disc property */
//        pdbSetPMAEmptyStatus(ePMAEmpty);
//        /* Misc Request(search for correct leadin) */
//        mTimeSet(fJumpTime,99,0,0);
//        tmpLBA = MSF2LBA(&fJumpTime);
//        plrMiscReq(ePhase18,eMiscTime,tmpLBA);
//        fJmpRtyCnt = 0;
//        fMode0Count = 0;
//    }
    return(Busy);
}
//
/*------------------------------------------------------------------------
// Phase15(From Ph14)-Sort PMA (Count Session)
-------------------------------------------------------------------------*/
private BYTE StUD_15()
{
//    /* Check Sessions */
//    plrInfoReq(ePhaseInc,eInfoCheckSessions);
    return(Busy);
}

/*------------------------------------------------------------------------
// Phase16(From Ph15,Ph16)-Check Sessions (Search B0)
-------------------------------------------------------------------------*/
private BYTE StUD_16()
{
//    LONG    tmpLBA;
//
//    if(!pdbGetCDRW()){//for CDR
//        if(pdbGetFinalSessCloseInfo() == eSessionClosed){
//            /* Check PMA */
//            plrInfoReq(ePhaseInc,eInfoCheckPMA);
//            return(Busy);
//        }
//    }
//
//    /* Misc Request(data track, Seek to 0:2:0) */
//    mTimeSet(fJumpTime,0,2,0);
//    tmpLBA = MSF2LBA(&fJumpTime);
//    plrMiscReq(ePhase21,eMiscTime,tmpLBA);
//    fJmpRtyCnt = 0;
    return(Busy);
}

/*------------------------------------------------------------------------
// Phase17(From Ph16)-Check last track (Search subcode)
-------------------------------------------------------------------------*/
private BYTE StUD_17()
{
//    LONG    tmpLBA;
//
//    mTimeSet(fJumpTime,0,2,0);
//    tmpLBA = MSF2LBA(&fJumpTime);
//    plrMiscReq(ePhase21,eMiscTime,tmpLBA);
//    fJmpRtyCnt = 0;
    return(Busy);
}

/*------------------------------------------------------------------------
// Phase18(From Ph14,Ph19,Serch Lead In)-Search for leadin
-------------------------------------------------------------------------*/
private BYTE StUD_18()
{
//    /* Misc Status  issue JUMP time to 99:0:0 command to servo */
//    StartTimer(&readTOC_Time);
//    fEFMRtyCnt = 0;
//    fSeekNotReachCnt = 0;
//    /* Misc Request(EFM) */
//    plrMiscReq(ePhaseInc,eMiscNoEFMOrMode0,0L);
    return(Busy);
}

/*------------------------------------------------------------------------
// Phase19(From Ph18,Ph19)-
-------------------------------------------------------------------------*/
private BYTE StUD_19()
{
//    LONG    tmpLBA;
//
//    switch(pdbGetNoEFMOrMode0Status()){     /* NoEFM => Mode0 */
//        case  eNoMode0Found:
//            if(fEFMRtyCnt == eMax_EFMRtyCnt){
//                /* Read TOC */
//                plrInfoReq(ePhase08,eInfoReadTOC);
//            }
//            else {
//                fEFMRtyCnt++;
//                /* Misc Request(EFM) */
//                plrMiscReq(ePhaseSame,eMiscNoEFMOrMode0,0L);
//            }
//            break;
//        case eMode0Found:
//            /* Q mode 0 protection */
//            if(++fMode0Count <= 4)   // mode 0 over 4 times
//            {
//                fJmpRtyCnt = 0;
//                plrMiscReq(ePhaseSame, eMiscNoEFMOrMode0, 0L);
//                break;
//            }
//
//            /* Misc Request(data track, Seek to 0:2:0) */
//            mTimeSet(fJumpTime,0,2,0);
//            tmpLBA = MSF2LBA(&fJumpTime);
//            plrMiscReq(ePhase21,eMiscTime,tmpLBA);
//            fJmpRtyCnt = 0;
//            break;
//        default:
//            fMode0Count = 0;    /* Q mode 0 protection */
//            fEFMRtyCnt = 0;
//            if (fSrchTocErrCnt == eMax_SrchTocErrCnt) {
//                /* Misc Request(data track, Seek to 0:2:0) */
//                mTimeSet(fJumpTime,0,2,0);
//                tmpLBA = MSF2LBA(&fJumpTime);
//                plrMiscReq(ePhase21,eMiscTime,tmpLBA);
//
//                pdbSetErrorGroupDiscInfo(eErrorPMA);
//                fJmpRtyCnt = 0;
//                return(Error);
//            }
//            else if(ReadTimer(&readTOC_Time) > 120)//120 ms
//            {
//                fSrchTocErrCnt++;
//                if(fSrchTocErrCnt == 1)
//                {
//                    mTimeSet(fJumpTime,98,0,0);
//                }
//                else if (fSrchTocErrCnt == 2)
//                {
//                    mTimeSet(fJumpTime,98,30,0);
//                }
//                else
//                {
//                    mTimeSet(fJumpTime,99,0,0);
//                }
//
//                tmpLBA = MSF2LBA(&fJumpTime);
//                plrMiscReq(ePhase18,eMiscTime,tmpLBA);
//                fJmpRtyCnt = 0;
//                fMode0Count = 0;
//            }
//            else
//            {
//                /* Misc Request(EFM) */
//                plrMiscReq(ePhaseSame,eMiscNoEFMOrMode0,0L);
//            }
//    }
    return(Busy);
}
/*-----------------------------------------------------------------------*/
private BYTE StUD_19E()
{
//    LONG tmpLBA;
//
//    /* Misc Request(search for correct leadin) */
//    /*mTimeSet(fJumpTime,99,0,0);
//    tmpLBA = MSF2LBA(&fJumpTime);
//    plrMiscReq(ePhase18,eMiscTime,tmpLBA);
//    fJmpRtyCnt = 0;
//    return(Busy);*/
//
//    if (fJmpRtyCnt < 3)
//    {
//        fJmpRtyCnt++;
//        fSeekNotReachCnt = 0;
//
//        if(fJmpRtyCnt == 1)
//        {
//            mTimeSet(fJumpTime,98,0,0);
//        }
//        else if (fJmpRtyCnt == 2)
//        {
//            mTimeSet(fJumpTime,98,30,0);
//        }
//        else
//        {
//            mTimeSet(fJumpTime,99,0,0);
//        }
//        tmpLBA = MSF2LBA(&fJumpTime);
//        plrMiscReq(ePhase18,eMiscTime,tmpLBA);
//        //fJmpRtyCnt = 0;
//        return(Busy);
//    }
//    else
        return(Error);
}

/*------------------------------------------------------------------------
// Phase20(From Ph05,Ph09,Ph16,Ph17,Ph19)-
-------------------------------------------------------------------------*/
private BYTE StUD_20()
{
    tWatchSC ServoCmd;  /* Servo Command */

    SIF_CMD_MESSAGE(PRN_HIF_TO_SIF,0);
    if(get_SIF(EXECUTIONSTATE) == svoBusy)
        set_SIF(EXECUTIONSTATE,svoReady);
    svoCmdVar.svoCommand = CNOP;
    set_SIF(COMMAND,svoCmdIdle);        /* command clr */

    /* Servo Watch */
    ServoCmd.Cmd = svoCmdVeloAdjust;
    ServoCmd.Len = 0;

    WRITE_FIELD(HAL_CDD_SUBQITPL, 0);       /* Disable SubQ interpolation */

    fEFMRtyCnt = 0;
    fDiscRtyCnt= 0;
    fJmpRtyCnt = 0;
    plrWatchReq(ePhase03,eWatch_ServoStatus,(BYTE *) &ServoCmd);
    return(Busy);
}

/*------------------------------------------------------------------------
// Phase21(Ph20)-
-------------------------------------------------------------------------*/
private BYTE StUD_21()
{
    SIF_CMD_MESSAGE(PRN_HIF_TO_SIF,0);
    if(get_SIF(EXECUTIONSTATE) == svoBusy)
        set_SIF(EXECUTIONSTATE,svoReady);
    svoCmdVar.svoCommand = CNOP;
    set_SIF(COMMAND,svoCmdIdle);        /* command clr */

#if (STARTUP_AT_LOWEST_CAV == 1)
    if(pdbGetServoReadyStatus() == TRUE)//Spin down -> up
    {
        if(pdbDiscLoaded() == CD_DISC)
        {
            /* Set to Lowest CAV speed */
            svoVar_svoTargetSpeed = MIN_CAV_SPEED_CD;
            spdChangeDiscSpeed(MIN_CAV_SPEED_CD);
            Current_read_sp = eCDIdleSpeed;
            SpeedMD = AutoMode;
        }
    }
#endif

    WRITE_FIELD(HAL_CDD_SUBQITPL, 1);       /* Enable SubQ interpolation */
    pdbSetServoReadyStatus(TRUE);           /* Set Servo Ready Status to TRUE */
    return(Ready);
}

/*------------------------------------------------------------------------
// Phase21E(Ph20)-
-------------------------------------------------------------------------*/
private BYTE StUD_21E()
{
#if (OPEN_SESSION==1)
    tWatchSC ServoCmd;  /* Servo Command */
    SHORT    Number;
#endif

    SIF_CMD_MESSAGE(PRN_HIF_TO_SIF,0);
    if(get_SIF(EXECUTIONSTATE) == svoBusy)
        set_SIF(EXECUTIONSTATE,svoReady);
    svoCmdVar.svoCommand = CNOP;
    set_SIF(COMMAND,svoCmdIdle);        /* command clr */

#if (OPEN_SESSION==1)
    if( (fJmpRtyCnt < (eMax_JmpRtyCnt)) )
    {
        fJmpRtyCnt++;
        SendMsgCn(SEND_SStpTask_MSG,2,0x800494, 3, fJmpRtyCnt);

        Number = -100*fJmpRtyCnt;
        /* Servo Watch - Misc Status issue JUMP time to NextSessionTime command to servo */
        ServoCmd.Cmd = svoCmdJumpGrooves;
        ServoCmd.Len = 3;
        ServoCmd.Para[0] = (BYTE) Number;
        ServoCmd.Para[1] = (BYTE)(Number >> 8);
        ServoCmd.Para[2] = eTimeJump;
        plrWatchReq(ePhaseSame,eWatch_ServoStatus,(BYTE *) &ServoCmd);
        return(Busy);
    }
#endif

#if (STARTUP_AT_LOWEST_CAV == 1)
    if(pdbGetServoReadyStatus() == TRUE)//Spin down -> up
    {
        if(pdbDiscLoaded() == CD_DISC)
        {
            /* Set to Lowest CAV speed */
            svoVar_svoTargetSpeed = MIN_CAV_SPEED_CD;
            spdChangeDiscSpeed(MIN_CAV_SPEED_CD);
            Current_read_sp = eCDIdleSpeed;
            SpeedMD = AutoMode;
        }
    }
#endif

    WRITE_FIELD(HAL_CDD_SUBQITPL, 1);       /* Enable SubQ interpolation */
    pdbSetServoReadyStatus(TRUE);           /* Set Servo Ready Status to TRUE */
    return(Ready);
}

/*------------------------------------------------------------------------
// Phase22(From Response,Ph22)-
-------------------------------------------------------------------------*/
private BYTE StUD_22()
{
    {
        if(((pdbGetDiscKind() == eCDRDisc ) && (pdbGetCDRW() == TRUE))||
           ((pdbGetDiscKind() == eCDRWDisc) && (pdbGetCDRW() != TRUE)))
        {
            /* Servo Watch */
            if(pdbGetCDRW() == TRUE){
                fReAdjustType.fDiscKind = eCDRWDisc;
            }else{
                fReAdjustType.fDiscKind = eCDRDisc;
            }
            plrServoReAdjust(fReAdjustType, ePhase01);
        }else{
            plrMeReq(ePhase12);
        }
    }
    return(Busy);
}

/************************************************************************
// Name        : StUD_23
// Description : This function is only used in DeadLoop_Solution.
*************************************************************************/
private BYTE StUD_23()
{
#if(DeadLoop_Solution==1)
    tWatchSC ServoCmd;  /* Servo Command */

      /* Servo Watch */
    ServoCmd.Cmd = svoCmdInit;
    ServoCmd.Len = 0;
    plrWatchReq(ePhase01,eWatch_ServoStatus,(BYTE *) &ServoCmd);
#endif
    return(Busy);
}
/*------------------------------------------------------------------------
// Phase24  - (From Ph24) svoCmdVeloAdjust
-------------------------------------------------------------------------*/
private BYTE StUD_24()
{
/* sequential read */

    tWatchSC ServoCmd;  /* Servo Command */

    /* Servo Watch */
    ServoCmd.Cmd = svoCmdVeloAdjust;
    ServoCmd.Len = 0;

    WRITE_FIELD(HAL_CDD_SUBQITPL, 0);       /* Disable SubQ interpolation */

    /* Inner data read other flag clear */
    plrWatchReq(ePhase25,eWatch_ServoStatus,(BYTE *) &ServoCmd);

    return(Busy);
}
/*------------------------------------------------------*/
private BYTE StUD_24E()
{
    if (fJmpRtyCnt < 3)
    {
        fJmpRtyCnt++;

        if ((pdbGetDiscKind() == eDVDRAMDisc) ||( pdbGetDiscKind() == eDVDROMDisc))
        {
            plrServoSeek(START_ADDRESS_OF_CONTROL_DATA, eWLayer0, ePhaseSame);
        }
        else
        {
            plrServoSeek(START_ADDRESS_OF_CONTROL_DATA, eWLayer0, ePhaseSame);
        }
        return(Busy);
    }
    else
    {
        fJmpRtyCnt = 0;
        return(Error);
    }
}
/*------------------------------------------------------*/

/*------------------------------------------------------------------------
// Phase25(From Ph24)-
//      DVD  Read Leadin Normal Data
-------------------------------------------------------------------------*/
private BYTE StUD_25()
{
    if(pdbGetServoReadyStatus() == FALSE)//First time to start-up
    {
#if (ENABLE_AACS == 1)
        if (pdbDiscLoaded() == BD_DISC || pdbDiscLoaded() == HD_DISC)
            InitializeAacsFlags();   /* Initialize AACS flags */
        else
#endif
    /* Inner data read other flag clear */
        cssInit();          /* Css Inital */
    }
    //Move...//[W.0Q]Modify DVD+R startup set speed issue from STAND BY mode.

    WRITE_FIELD(HAL_CDD_SUBQITPL, 1);       /* Enable SubQ interpolation */
    pdbSetServoReadyStatus(TRUE); /* Set Servo Ready Status to TRUE */
    return(Ready);
}

/*------------------------------------------------------------------------
// Misc Retry Count
-------------------------------------------------------------------------*/
private BYTE StUD_MiscRtyCnt()
{
    LONG    tmpLBA;
    BYTE    jump_gap;
#if (OPEN_SESSION==1)
    tWatchSC ServoCmd;  /* Servo Command */
    SHORT    Number;
#endif


    if( (fJmpRtyCnt < eMax_JmpRtyCnt)||(((plrGetPhase()== ePhase20)||(plrGetPhase()== ePhase04))&&(fJmpRtyCnt < (eMax_JmpRtyCnt*2))) ){
        fJmpRtyCnt++;

        SIF_CMD_MESSAGE(PRN_HIF_TO_SIF,0);
        if(get_SIF(EXECUTIONSTATE) == svoBusy)
            set_SIF(EXECUTIONSTATE,svoReady);
        svoCmdVar.svoCommand = CNOP;
        set_SIF(COMMAND,svoCmdIdle);        /* command clr */

#if (OPEN_SESSION==1)
        if ((plrGetPhase()== ePhase20)||(plrGetPhase()== ePhase04))
        {
            SendMsgCn(SEND_SStpTask_MSG,2,0x800494, 2, fJmpRtyCnt);
            Number = -100*fJmpRtyCnt;
            /* Servo Watch - Misc Status issue JUMP time to NextSessionTime command to servo */
            ServoCmd.Cmd = svoCmdJumpGrooves;
            ServoCmd.Len = 3;
            ServoCmd.Para[0] = (BYTE) Number;
            ServoCmd.Para[1] = (BYTE)(Number >> 8);
            ServoCmd.Para[2] = eTimeJump;
            plrWatchReq(ePhase03,eWatch_ServoStatus,(BYTE *) &ServoCmd);
        }
        else
#endif
        {
            jump_gap = 5;
            if(fJmpRtyCnt&0x01)
                mathAddAddressToMSF(&fJumpTime,jump_gap*75*fJmpRtyCnt,&fJumpTime);//+jump 5 sec
            else
                mathSubtractAddressFromMSF(&fJumpTime,jump_gap*75*fJmpRtyCnt,&fJumpTime);//-jump 5 sec
            tmpLBA = MSF2LBA(&fJumpTime);
            plrMiscReq(ePhaseSame,eMiscTime,tmpLBA);
        }
        return(Busy);
    }
    else {
        if (plrGetPhase()== ePhase06){
            plrMeReq(ePhase06);
#if (DISC_MIS_JUDGE_START_UP_RECOVERY)
            RetryDiscLaser = eDiscUnknown;//Don't need to try next Laser
#endif
            return(Busy);
        }
        else if (plrGetPhase()== ePhase20){
            //plrMeReq(ePhase21);
            plrMeReq(ePhase05);
            return(Busy);
        }
        else
            return(Error);
    }
}

/*------------------------------------------------------------------------
// Servo Retry Count
-------------------------------------------------------------------------*/
private BYTE StUD_ServoRtyCnt()
{
    tWatchSC ServoCmd;  /* Servo Command */
    SHORT Number = -90;

    if(fJmpRtyCnt < eMax_JmpRtyCnt) {
        fJmpRtyCnt++;
        /* Servo Watch */
        ServoCmd.Cmd = svoCmdJumpGrooves;
        ServoCmd.Len = 3;
        ServoCmd.Para[0] = (BYTE) Number;
        ServoCmd.Para[1] = (BYTE)(Number >> 8);
        ServoCmd.Para[2] = eTimeJump;
        plrWatchReq(ePhaseSame,eWatch_ServoStatus,(BYTE *) &ServoCmd);
        return(Busy);
    }
    else {
        return(Error);
    }
}

/*------------------------------------------------------------------------
// Toc Retry Count
-------------------------------------------------------------------------*/
private BYTE StUD_TocRtyCnt()
{
    LONG    tmpLBA;
    BYTE    Rety;

    if(fTocRtyCnt < eMax_TocRtyCnt){
        fTocRtyCnt++;
        fJmpRtyCnt = 0;
        Rety = fTocRtyCnt % 5;

        if (fTocRtyCnt == eMax_TocRtyCnt)
        {
            send_msg5S(1,"TocRty=Max");
            /* Speed Set Only (Non Watch) */
            plrServoSetSpeed(e10XCAV,ePhase06);
        }
        else
        {
            mTimeSet(fJumpTime,0,2+(Rety * 10),0);
            tmpLBA = MSF2LBA(&fJumpTime);
            plrMiscReq(ePhase06,eMiscTime,tmpLBA);
        }
        return(Busy);
    }
    else
        return(Error);
}

/*------------------------------------------------------------------------
// Disc Retry Count
-------------------------------------------------------------------------*/
private BYTE StUD_DiscRetryCnt()
{
    if (fDiscRtyCnt < eMax_DiscRtyCnt){
        fDiscRtyCnt++;
        /* Me Request(dummy) */
        if(pdbDiscLoaded() == CD_DISC)
            plrMeReq(ePhase05);     /*CD Disc*/
        else
            plrMeReq(ePhase25);     /*DVD Disc*/

        return(Busy);
    }
    else
    {
        /* set error group Disc Info error */
        pdbSetErrorGroupDiscInfo(eErrorDiscInfo);
        return(Error);
    }
}

/*------------------------------------------------------------------------
// Search Lead In
-------------------------------------------------------------------------*/
private BYTE StUD_SearchLeadIn()
{
//    LONG    tmpLBA;
//
//    /* Non PMA Quick Erased Disc */
//    /* Misc Request(search for correct leadin) */
//    mTimeSet(fJumpTime,99,0,0);
//    tmpLBA = MSF2LBA(&fJumpTime);
//    plrMiscReq(ePhase18,eMiscTime,tmpLBA);
//    fJmpRtyCnt = 0;
//    fMode0Count = 0;    /* Q mode 0 protection */
    return(Busy);
}

/************************************************************************
// Name        : StopDisc
// Description : This function stops the disc motor.
/                It stops reading the disc info, reading TOC,
//               reading PMA and it stops the jump process.
//               (only on the Servo Subsystem side - on the basic engine side
//               the jump process will be finished).
*************************************************************************/
private BYTE StopDisc(void)
{
    tWatchSC ServoCmd;  /* Servo Command */

    SpeedMD = Di_f;
    Current_read_sp = eSpStop;                  /* set to speed */
    /* set player status to eSTOPPING */
    pdbSetPlayerStatus(eStStopping );
    /* Servo Watch */
    ServoCmd.Cmd = svoCmdStop;
    ServoCmd.Len = 0;
    plrWatchReq(ePhase01,eWatch_ServoStatus,(BYTE *) &ServoCmd);
    return(Busy);
}

/*------------------------------------------------------------------------
// Phase01(From Cmd)
-------------------------------------------------------------------------*/
private BYTE StpD_01()
{
    /* set player status to eTrayIn */
    pdbSetPlayerStatus(eStTrayIn);
    return(Ready);
}


/*------------------------------------------------------------------------*/
/* The end of file                                                        */
/*------------------------------------------------------------------------*/
