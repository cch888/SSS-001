/****************************************************************************
*                (c) Copyright 2001 - 2008 SUNEXT DESIGN, INC.
*                           All Right Reserved
*
*
* $Revision: 328 $
* $Date: 11/03/28 2:37p $
* FILENAME:   sv_util.c
*
* DESCRIPTION
*   Support servo related functions for NX speed.
*
* NOTES:
*
**************************** SUNEXT CONFIDENTIAL *******************************/

#include ".\common\globals.h"
#include ".\common\globtype.h"
#include ".\common\chip.h"
#include ".\common\error.h"
#include ".\common\comMath.h"
#include ".\common\system.h"
#include ".\common\sdram.h"
#include ".\hif\gp_equ.h"
#include ".\hif\mmc.h"
#include ".\hif\sv_util.h"
#include ".\hif\s2b_main.h"
#include ".\hif\gp_mac.h"
#include ".\hif\app_if.h"
#include ".\hif\at_knl.h"
#include ".\hif\rd_util.h"
#include ".\hif\gp_rdtoc.h"
#if EN_POWER_CONTROL
#include ".\hif\powerctl.h"
#endif

#include ".\hif\at_util.h"
#include ".\hif\gp_util.h"
#include ".\hif\bf_util.h"
#include ".\hif\gp_init.h"
#include ".\hif\diag.h"
#include ".\player\plrServo.h"
#include ".\player\plrWatch.h"              /* Player:Watch */
#if(FDP_TRKSLIP_MONITOR == 1)
#include ".\player\plrStStp.h"
#endif
#include ".\servo\svovar.h"
#include ".\servo\svo_step.h"
#include ".\oem\oem_spd_tbl.h"
#include ".\oem\oem_tbl.h"
#if (SUN_FUNC_TEST == 1)
#include ".\oem\sun710_function_test.h"
#endif

#include ".\servo\svo_lib.h"
#include ".\al\reg_hal.h"
#include ".\al\reg_intctrl.h"
#include ".\common\intctl.h"
#include ".\hif\gp_key.h"


#include ".\common\constant.h"  //modify for read Q code time out sequence
extern BYTE     RD_speed_setting;           /* this variable is used by gp_gtpfm.c */

extern BYTE LastOPCOK;
extern BYTE bOPCOK;

vBYTE   Erase_RMA_First;

#if (AUDIO_PLAYBACK == 1)
extern BYTE pre_tno; /* Check tno protect */
#endif
/**************************************************************************
* Function:    GetTransferSpeed
*
* Description:
*
* Returns:
***************************************************************************/
const USHORT data_tran_speed_CD[53]=
/*dummy,    1x,    2x,    3x,    4x,
     5x,    6x,    7x,    8x,
     9x,   10x,   11x,   12x,
    13x,   14x,   15x,   16x,
    17x,   18x,   19x,   20x,
    21x,   22x,   23x,   24x,
    25x,   26x,   27x,   28x,
    29x,   30x,   31x,   32x,
    33x,   34x,   35x,   36x,
    37x,   38x,   39x,   40x,
    41x,   42x,   43x,   44x,
    45x,   46x,   47x,   48x
    49x,   50x,   51x,   52x          */
{0x0000,  SP1X,  SP2X,0x0000,  SP4X,
 0x0000,0x0000,0x0000,  SP8X,
 0x0000, SP10X,0x0000, SP12X,
 0x0000,0x0000,0x0000, SP16X,
 0x0000,0x0000,0x0000,0x0000,
 0x0000,0x0000,0x0000, SP24X,
 0x0000,0x0000,0x0000,0x0000,
 0x0000,0x0000,0x0000, SP32X,
 0x0000,0x0000,0x0000,0x0000,
 0x0000,0x0000,0x0000, SP40X,
 0x0000,0x0000,0x0000,0x0000,
 0x0000,0x0000,0x0000, SP48X,
 0x0000,0x0000,0x0000, SP52X
};

const USHORT data_tran_speed_DVD[21]=
/*dummy,     1x,      2x,       3x,      4x,
     5x,     6x,      7x,       8x,
     9x,    10x,     11x,      12x,
    13x,    14x,     15x,      16x,
    17x,    18x,     19x,      20x             */
{ 0x0000, SP1XDVD, SP2XDVD, SP2_4XDVD, SP4XDVD,
 SP5XDVD, SP6XDVD,  0x0000,   SP8XDVD,
  0x0000,SP10XDVD,  0x0000,  SP12XDVD,
  0x0000,  0x0000,  0x0000,  SP16XDVD,
  0x0000,  0x0000,  0x0000,  SP20XDVD
};

#if (BD_ENABLE)
const USHORT data_tran_speed_BD[15]=
/*dummy,     1x,      2x,     3x,     4x,     5x,     6x,     7x */
/*   8x,     9x,     10x,    11x,    12x,    13x,    14x,        */
{ 0x0000, SP1XBD, SP2XBD, SP2_2XBD, SP4XBD, 0x0000, SP6XBD, 0x0000,
  SP8XBD, 0x0000,SP10XBD, 0x0000,SP12XBD, 0x0000,SP14XBD
};
#endif  //(BD_ENABLE)

#if (ENABLE_HDDVD)
const USHORT data_tran_speed_HD[4]=
/*dummy,     1x,      2x,     4x */
{ 0x0000, SP1XHD, SP2XHD, SP4XHD };
#endif  //(ENABLE_HDDVD)

USHORT GetTransferSpeed(BYTE spd)
{

    spd &= eSpeedValue;                 // Strip off bits

    if( pdbDiscLoaded() == DVD_DISC )
    {
        return(data_tran_speed_DVD[spd]);
    }
    #if (BD_ENABLE)
    else if( pdbDiscLoaded() == BD_DISC )
    {
        return(data_tran_speed_BD[spd]);
    }
    #endif  //(BD_ENABLE)
    #if (ENABLE_HDDVD)
    else if( pdbDiscLoaded() == HD_DISC )
    {
        return(data_tran_speed_HD[spd]);
    }
    #endif  //(ENABLE_HDDVD)
    else
    {
        return(data_tran_speed_CD[spd]);
    }
}

/*********************************************************************
* Function:    ServoTrayIn
*
* Description: Call player to pull the tray in. The variable,
*              Iram_Svr_player_mode, will be set accordingly -
*              TRAY_OUT_MODE or TRAY_IN_MODE
*
* Returns:     WaitPlayerReady()
*              1 = command failed
*              0 = command suceeded
*********************************************************************/

extern USHORT   timeUp_Group1;
extern USHORT   timeUp;

#if  (BURN_IN == 1 )
extern BOOL Burn_in_Enable;
#endif

BOOL ServoTrayIn(void)
{
    BOOL retval;

    Svr_pause_reqire = FALSE;

    /* wake up the power mode */
#if(EN_POWER_CONTROL==1)
    PowerControl(eIdlePower);
#else
    ataPowerMode = IDLE_MODE;
#endif

    St_become_ready = TRUE;
    Iram_MediaTrayEvent = TRUE;
    OperationalChangeTrayEvent = GSEN_OPE_TRAY_IN;

    pdbSetClearBufInfoAll();

    HIF.PlayerIDC    = 0xD0;                            /* set command IDC 0 parameter */
    HIF.PlayerAccept = plrNewCmd;
    HIF.PlayerOpcode = plrCmdTrayIn;
    HIF_CMD_MESSAGE(PRN_HIF_TO_SIF,0);

    Iram_Svr_player_mode = EXECUTING_TRAY_IN_MODE;
    timeUp_Group1 = timeUp;

#if (LED_NUM == 1)
    ReadyLED(LED_ON);
#elif (LED_NUM == 2)
    ReadyLED(LED_FLASH);
#endif /* LED_NUM */

    retval = WaitPlayerReady();

#if  (BURN_IN == 1 )
   if(Burn_in_Enable)
      return retval;
#endif

#if (LED_NUM == 2)
    ReadyLED(LED_OFF);
#endif /* LED_NUM == 2 */

    if(retval == READY)
    {
        Iram_Svr_player_mode = TRAY_IN_MODE;
        St_door_open = FALSE;
        retval = PASS;
    }
    else
    {
        Iram_Svr_player_mode = TRAY_OUT_MODE;   /* mark tray out mode */
        St_door_open = TRUE;                    /*  door open */
        St_become_ready = FALSE;
        pdbServoSetDiscDiameter(eNoDisc);
        SpeedMD = Di_f;
        Current_read_sp = eSpStop;
        Iram_Knl_save_counter = 0;
        Svr_pause_reqire = FALSE;
        retval = FAIL;
    }

    return retval;
}


/*********************************************************************
* Function:    ServoTrayOut
*
* Description: Call player to pull the tray out. The variable,
*              Iram_Svr_player_mode, will be set accordingly -
*              TRAY_OUT_MODE or TRAY_IN_MODE
*
* Returns:     1 = command failed
*              0 = command suceeded
*********************************************************************/
extern BYTE         StreamFlag;
#if (HORROR_DISC_IMPROVEMENT == 1)
extern BYTE         VCD_Try_Case,C2CNT1;
#endif
extern  BYTE   Packet_Track;//modify for read Q code time out sequence

BOOL ServoTrayOut(void)
{
    BOOL retval;

    Svr_pause_reqire = FALSE;

    while (HIF.PlayerAccept != plrIdleCmd)
        ExitProcess();

    /* wake up the power mode */
#if(EN_POWER_CONTROL==1)
    PowerControl(eIdlePower);
#else
    ataPowerMode = IDLE_MODE;
#endif
    /* Abort all of the jobs running */

    St_door_open = TRUE;                        /*  door open */
    St_become_ready = TRUE;

    St_drv_ready = FALSE;                   /* Drive not ready */
    St_no_disc = TRUE;
    st_disc_change = FALSE;                       /* Mark cd changed */

#if (HORROR_DISC_IMPROVEMENT == 1)
    C2CNT1=0;
    VCD_Try_Case=0;
#endif
    Packet_Track = TRKNO_NOTVALID;    //modify for read Q code time out sequence

    Iram_Rw_read_mode = eUnknownMode;       /* Set read mode */
    StreamFlag &= (~ STR_ENABLE_BIT);

    ClearDataBuffer();                              /* Clear buffer data */
    SetDefaultReadSpeed();
    ClearReadAheadSettings();                   /* Read Ahead Setting clear */

    Iram_MediaTrayEvent = TRUE;
    OperationalChangeTrayEvent = GSEN_OPE_TRAY_OUT;
    svoIramVar.currentID.Layer = 0;    /* winfinal DVD-9 test fail modify */

    pdbSetClearBufInfoAll();

    HIF.PlayerIDC    = 0xD0;                    /* set command IDC 0 parameter */
    HIF.PlayerAccept = plrNewCmd;
    HIF.PlayerOpcode = plrCmdTrayOut;
    HIF_CMD_MESSAGE(PRN_HIF_TO_SIF,0);

    Iram_Svr_player_mode = EXECUTING_TRAY_OUT_MODE;

#if (LED_NUM == 1)
    ReadyLED(LED_ON);
#elif (LED_NUM == 2)
    ReadyLED(LED_FLASH);
#endif  /* LED_NUM == 2 */

    retval = WaitPlayerReady();

#if  (BURN_IN == 1 )
    if(Burn_in_Enable)
      return retval;
#endif

    ReadyLED(LED_OFF);

    Iram_Svr_player_mode = TRAY_OUT_MODE;   /* mark tray out mode */
        St_become_ready = FALSE;
        pdbServoSetDiscDiameter(eNoDisc);
        SpeedMD = Di_f;
        Current_read_sp = eSpStop;
        Iram_Knl_save_counter = 0;

    if(retval == READY)
    {
        retval = PASS;
    }
    else
    {
        retval = FAIL;
    }

#if(EN_POWER_CONTROL==1)
  #if (SET_SLOT_IN == 1)
    if(ipCloseSW==1) //[BTC][Q36]disc out die machine
  #endif
    PowerControl(eStandbyPower);
#else
    ataPowerMode = STANDBY_MODE;
#endif

    return retval;
}

/*********************************************************************
* Function:    ServoStartUp
*
* Description: Call player to start up the player machanism. It is a
*              implied read_TOC and store the TOC data into scratch
*              DRAM.
*
* Returns:     1 = command failed
*              0 = command suceeded
*********************************************************************/
BOOL ServoStartUp(void)
{
    BOOL retval;
    BYTE backup_SpeedMD;

    backup_SpeedMD = SpeedMD;
    SpeedMD = StartUp_f;

    StopDecoder();
    ReadyLED(LED_ON);
    St_no_disc   = FALSE;                               /* no disc */

    HIF.PlayerAccept = plrNewCmd;
    HIF.PlayerIDC    = 0xD1;                        /* set command IDC 1 parameter */
    HIF.PlayerOpcode = plrCmdStartUp;
    HIF_CMD_MESSAGE(PRN_HIF_TO_SIF,0);
    HIF.PlayerParameters[0] = 0;

    Iram_Svr_player_mode = EXECUTING_START_UP_MODE;
    ataPowerMode      = IDLE_MODE;

    svoIramVar.subQEnableFlag = 1;
    retval = WaitPlayerReady();
    svoIramVar.subQEnableFlag = 0;

    /* startup check disc */
    if(retval == ERROR || pdbServoGetDiscDiameter() == (eDiscDiameter)eNoDisc)
    {
        /* tray is not closed and no disc - command failed for other reason */
        Iram_Svr_player_mode = STOP_MODE;                   /* mark stop mode */
        if (pdbServoGetDiscDiameter() == (eDiscDiameter)eNoDisc)
        {
            St_no_disc = TRUE;                              /* no disc installed */
        }
        else if (pdbServoGetDiscDiameter() != eUnknownDiameter)
        {
#if (ALWAYS_HOMING == 0)
            Iram_stp_Initialized = STEP_UNINITIALIZED;
#endif
#if (COLLI_SWITCH == 1) && (COLLI_ALWAYS_HOMING == 0)
            Iram_colli_Initialized = STEP_UNINITIALIZED;
#endif
        }
        return(FAIL);                                   /* startup failed */
    }

    St_no_disc = FALSE;                                 /* disc */

    /* Startup Success */
    Iram_Svr_player_mode = EXECUTING_START_UP_MODE;     /* mark startup mode */

    if( pdbDiscLoaded() == CD_DISC )
    {
        St_blank_disc = pdbGetBlankDiscInfo();          /* check if blank disc */
        if (St_blank_disc)
        {
            ClearMaxLBA();                               /* init max lba and disc capacity */
        }
    }

    ServoPause();

    /* Set speed control for appropriate condition*/
    if(backup_SpeedMD != StartUp_f)
        SpeedMD = backup_SpeedMD;

    Iram_Knl_save_counter  = STANDBYCNT;
    return(PASS);
}


/*************************************************************************
* Function:    ServoStop()
*
* Description: Call player to stop servo.
*
* Returns:     1 = command failed
*              0 = command suceeded
**************************************************************************/
BOOL ServoStop(void)
{
    BOOL retval,FoDpFlag;

    FoDpFlag = FALSE;
    if(svoIramVar.focusDropFlag == TRUE)
    {//When drive occurs Focus Drop,Power Control can not change into Stand By Mode.
        FoDpFlag = TRUE;
    }
    Iram_Knl_save_counter = 0;

#if (BD_ENABLE == 1)
    if ( pdbGetDiscKind() == eBDRDisc || pdbGetDiscKind() == eBDRDLDisc ||
         pdbGetDiscKind() == eBDREDisc || pdbGetDiscKind() == eBDREDLDisc )
    {
        BDStopPrebuffering();
    }
    else
#endif
    {
        StopDecoder();                 /* turn off decode function */
    }

#if (AUDIO_PLAYBACK == 1)
    if (St_nx_audio == TRUE)
    {
        Iram_audio_scan_st = FALSE;
        St_nx_audio = FALSE;
        pre_tno = 0xff;
        AudioTransferDone();
    }
#endif
    /*--------------------------------------------------
        Wait until player is ready (exit process)
    ---------------------------------------------------*/
    while (HIF.PlayerAccept != plrIdleCmd){
        ExitProcess();
    }

    Current_read_sp = eSpStop;                  /* set to speed */


    HIF.PlayerIDC    = 0xD0;        /* set command IDC 0 parameter */
    HIF.PlayerAccept = plrNewCmd;
    HIF.PlayerOpcode = plrCmdStop;
    HIF_CMD_MESSAGE(PRN_HIF_TO_SIF,0);

    Iram_Svr_player_mode = EXECUTING_STOP_MODE;
    retval = WaitPlayerReady();
#if (EN_POWER_CONTROL == 1)
    if((FoDpFlag == FALSE) && (retval == READY) && (svoIramVar.svoCurrentSpeed == 0xff))
        PowerControl(eStandbyPower);
#else
    ataPowerMode         = STANDBY_MODE;
#endif
    Iram_Svr_player_mode = STOP_MODE;
    Svr_pause_reqire     = FALSE;
    SpeedMD = Di_f;

    if(retval == READY){
        return(PASS);
    }else{
        return(FAIL);
    }
}


/*********************************************************************
* Function:    ServoSeekToLeadIn
*
* Description: This is low level routine to send the lead-in seek command to servo.
*
* Globals:     param1/2/3 - the target location passed from the caller.
*
* Returns:     1 = command failed
*              0 = command suceeded
*********************************************************************/
BOOL ServoSeekToLeadIn(void)
{
    ataPowerMode = IDLE_MODE;       /* get the address of power mode set to idle mode*/
    StopDecoder();                  /* turn off decoder */
    St_audio_pause   = FALSE;       /* clear audio pausing flag */
    Svr_pause_reqire = FALSE;

    HIF.PlayerIDC    = 0xD0;        /* set command IDC */
    HIF.PlayerAccept = plrNewCmd;
    HIF.PlayerOpcode = plrCmdLeadIn;
    HIF_CMD_MESSAGE(PRN_HIF_TO_SIF,0);

    Iram_Svr_player_mode = EXECUTING_SEEK_MODE;

    if(WaitPlayerReady() == READY){
        return(PASS);
    }else{
        return(FAIL);
    }
}


/*********************************************************************
* Function:    ServoReAdjust
*
* Description: This is low level routine to send the ReAdjust Disc Type command to servo.
*
* Globals:     param1 - the disc type
*
* Returns:     1 = command failed
*              0 = command suceeded
*********************************************************************/
BOOL ServoReAdjust(discKindT ReAdjKind)
{
    StWordU DK;
    DK.Uword = ReAdjKind.fDiscKind;
    ataPowerMode = IDLE_MODE;       /* power mode set to idle mode*/

    HIF.PlayerIDC    = 0xD2;        /* set command IDC */
    HIF.PlayerAccept = plrNewCmd;
    HIF.PlayerOpcode = plrCmdReAdjust;
    HIF.PlayerParameters[0] = DK.byte.l;
    HIF.PlayerParameters[1] = DK.byte.h;
    HIF_CMD_MESSAGE(PRN_HIF_TO_SIF,0);

    if(WaitPlayerReady() == READY){
        return(PASS);
    }else{
        return(FAIL);
    }
}
/*********************************************************************
* Function:    ServoPassThrough
*
* Description: Call player to send a command directly to the Servo
*              Iram_Svr_player_mode, will be set accordingly -
*
* Returns:     WaitPlayerReady()
*              1 = command failed
*              0 = command suceeded
*********************************************************************/
BOOL ServoPassThrough(void)                                                  // Support for Player command Servo Pass Through.
{
    BYTE RetValue;
#if (P_DIAGS == 1)
    BYTE Temp;
#endif

    HIF.PlayerIDC    = 0xD0;        /* set command IDC */
    HIF.PlayerAccept = plrNewCmd;
    HIF.PlayerOpcode = plrCmdPassTrough;

    // The Player will set up the Servo Interface (SIF) and send the command to the Servo.
    // The Servo will execute Servo Command per Spec.
    // The Serve will then put Return Parameters into SIF[]
    // If Servo returns with an Error then the player will issue a "Get Error Information" to the Servo.
    //   The Servo will then put error information into SIF[]
    if(WaitPlayerReady() == READY)
        RetValue = PASS;
    else
        RetValue = FAIL;
    // Servo and Player have completed Servo Pass Through Command.
    // Any return values are stored in SIF[]

#if (P_DIAGS == 1)
    // Here save return values from Servo Interface for a later retrieval with a Transparent Receive Command.
    ClearPosReturnData();       // Clear return data before copying new data.
    for (Temp = 0; Temp < sizeof(SIF.ServoParameters); Temp++)
    {
        TRReturn.byte.ReturnParameters[Temp] = SIF.ServoParameters[ Temp ];
    }
    // Update Player and Servo States in return data.
    TRReturn.byte.PlayerStateAtEndOfTransparentSend = HIF.PlayerState;
    TRReturn.byte.ServoStateAtEndOfTransparentSend  = SIF.ServoState;
    // The data in Scratch Buffer can be retrieved with a Host-issued Transparent Send Command (Get Transparent Return Data).
#endif
    if (RetValue == FAIL)
        BUILD_SENSE(0x04,0x44,0x00,0x18);    // INTERNAL TARGET FAILURE

    AutoCommandComplete();
    return(RetValue);
}

/*********************************************************************
* Function:    ServoSeek
*
* Description: This is low level routine to send the seek command to servo.
*
* Globals:     param1/2/3 - the target location passed from the caller.
*
* Returns:     1 = command failed
*              0 = command suceeded
*********************************************************************/
extern BYTE gPowerEvent;
extern BYTE gPowerStatus;
BOOL ServoSeek(long seek_lba,BYTE mode)
{
    BYTE cPreCtrlAm1;

    #if (ENABLE_RAM_DFE_ICE_DETECT == 1)
    if (pdbServoGetfDiscKind() == eDVDRAMDisc)
    {
        ClearDVDRAMIceIndicatorAll();
    }
    #endif

    cPreCtrlAm1 = READ_REG(HAL_CDDREG_MAP->CTRLAM1);
    WRITE_FIELD(HAL_CDD_SDMON, 1);
    if(Iram_Rw_read_mode == eCDDAMode)
        WRITE_FIELD(HAL_CDD_SCDDA, 1);
    else
        WRITE_FIELD(HAL_CDD_SCDDA, 0);

#if(EN_POWER_CONTROL==1)
    if (CurrentPowerMode == eStandbyPower)
    {
    /* for wake up the power from stand by mode */
        SetDiscReadPowerMode();
    }
#else
    ataPowerMode = IDLE_MODE;       /* get the address of power mode set to idle mode*/
#endif
    if(gPowerStatus != 1)
    {
        gPowerEvent = 1;
        gPowerStatus = 1; // power status = active
    }
    StopDecoder();                  /* turn off decoder */
    St_audio_pause   = FALSE;       /* clear audio pausing flag */
    Svr_pause_reqire = FALSE;

#if DVD_AWG_TEST
    /*-----------------------------------------------
        When using a signal generator to input data
        to the OTI chip there is no need for a seek.
    ------------------------------------------------*/
    return (PASS);
#endif

    /*--------------------------------------------------
        Wait until player is ready (exit process)
    ---------------------------------------------------*/
    while (HIF.PlayerAccept != plrIdleCmd){
        ExitProcess();
    }

    if (St_door_open == TRUE)//Startup time exceed 40sec, AP WinFinal send EjectDisc command
        return FAIL;
    send_msg80(SEND_WRITE_MSG,0x710065);
    SendMsgCn(SEND_ATAPI_CMD,5,0x800002, A4B(seek_lba), mode);    //Seek_Target_ID=%Xh, SeekType=%Xh
    Target_Timelba = seek_lba;

    HIF.PlayerIDC    = 0xD5;        /* set command IDC */
    HIF.PlayerAccept = plrNewCmd;
    HIF.PlayerOpcode = plrCmdJumpToAddress;
    HIF_CMD_MESSAGE(PRN_HIF_TO_SIF,0);

    HIF.PlayerParameters[0] = (char)((char *)&seek_lba)[3];
    HIF.PlayerParameters[1] = (char)((char *)&seek_lba)[2];
    HIF.PlayerParameters[2] = (char)((char *)&seek_lba)[1];
    HIF.PlayerParameters[3] = (char)((char *)&seek_lba)[0];
    HIF.PlayerParameters[4] = (char)mode;

    Iram_Svr_player_mode = EXECUTING_SEEK_MODE;

    ReadyLED(LED_ON);
    if(WaitPlayerReady() == READY)
    {
        WRITE_REG(HAL_CDDREG_MAP->CTRLAM1, cPreCtrlAm1);
        return(PASS);
    }
    else
    {
        WRITE_REG(HAL_CDDREG_MAP->CTRLAM1, cPreCtrlAm1);
        return(FAIL);
    }
}

/*************************************************************************
* Function:    ServoReadSubcode
*
* Description: Calls player to read SUBCODE.
*
* Returns:     1 = command failed
*              0 = command suceeded
*
* Exit:
*               PlayerAddrsPtr : point to 10 bytes subcode buffer.
*
*************************************************************************/
BOOL ServoReadSubcode(BYTE mode,BYTE req_tno)
{
    ULONG Subq_tmup, Subq1_tmup;
    BYTE  flag,SubQ_tmp;

    if((St_cdi == TRUE) && (St_cdi_audio == FALSE)){    /* for cdi plus audio disc */
        req_tno = 2;
    }

    switch(mode){
        case 1:     /* req Current Subq */
            flag = TRUE;
            SubQ_tmp = svoIramVar.subQEnableFlag;
            svoIramVar.subQEnableFlag = 1;

            StartTimer(&Subq1_tmup);
            do{
                if((ReadTimer(&Subq1_tmup)>1000L) || (flag == FALSE)){
                    flag = FALSE;
                    break;
                }

                StartTimer(&Subq_tmup);
                pdbSetSubCodeOK(FALSE);
                while(pdbGetSubcodeOK() != TRUE)
                {
                    ExitProcess();
                    if(ReadTimer(&Subq_tmup)>300L)
                    {
                        flag = FALSE;
                        break;
                    }
                }
                RequestSubQInfo();
            }while ((Iram_Player_output[0] & 0x0F) != 0x01);

            svoIramVar.subQEnableFlag = SubQ_tmp;

            if (flag == FALSE){
                return(FAIL);       /* read subcode error */
            }
            break;
        case 2:     /* req UPC */
            if(RequestCatalogNumber() == FAIL){
                if((ServoSearchISRCAndCatalog(1,0x02) == FAIL)||(RequestCatalogNumber() == FAIL)){
                    return(FAIL);       /* read subcode error */
                }
            }
            break;
        case 3:     /* req ISRC no */
            if(RequestISRCNumber(req_tno) == FAIL)
            {
                if((ServoSearchISRCAndCatalog(req_tno,0x03) == FAIL)||(RequestISRCNumber(req_tno) == FAIL)){
                    return(FAIL);       /* read subcode error */
                }
            }
            break;
        case 0xF0:  /* req RID no */
            if(RequestRIDNumber(req_tno) == FAIL){
                if((ServoSearchISRCAndCatalog(req_tno,0xF0) == FAIL)||(RequestRIDNumber(req_tno) == FAIL)){
                    return(FAIL);       /* read subcode error */
                }
            }
            break;
        default:
            return(FAIL);               /* read subcode error */
    }
    /* svr subcode done */
    PlayerAddrsPtr = (void *) &Iram_Player_output[0];
    return(PASS);
}


/*********************************************************************
* Function:    ServoPause
*
* sets the player mechanism to PAUSING MODE.
* Also it disables the decoder
*
* Returns:     0 - No error; 1 - Error occured
* Params: none
*************************************************************************/
BOOL ServoPause(void)
{
BOOL retval;

#if (SERVO_BODE != BODE_NONE)
    //send_msg5S(SHOW_DEBUG_MSG,"noPause");
    SendMsg80(SHOW_DEBUG_MSG,0x420105); //Skip Pause command
    //Send_timef = 1;
    Svr_pause_reqire = FALSE;
    return(PASS);
#endif

    StopDecoder();

    //send_msg5S(SEND_MRW_MSG,"Pause");
    SendMsg80(SHOW_DEBUG_MSG,0x420106); //Enter Pause Mode

    if((pdbGetServoState() == svoStPausing)||( Iram_Svr_player_mode == TRAY_OUT_MODE)){ /* for tray out mode do not call pause */
        Svr_pause_reqire = FALSE;
        return(PASS);
    }

    /*--------------------------------------------------
        Wait until player is ready (exit process)
    ---------------------------------------------------*/
    while (HIF.PlayerAccept != plrIdleCmd){
        ExitProcess();
    }

    Iram_Knl_save_counter  = STANDBYCNT;

    HIF.PlayerIDC    = 0xD0;            /* set command IDC */
    HIF.PlayerAccept = plrNewCmd;
    HIF.PlayerOpcode = plrCmdPauseOn;
    HIF_CMD_MESSAGE(PRN_HIF_TO_SIF,0);
    retval = WaitPlayerReady();

    Iram_Svr_player_mode = PAUSING_MODE;
    Svr_pause_reqire = FALSE;
    if(retval == READY){
        return(PASS);
    }else{
        return(FAIL);
    }
}


/*************************************************************************
* Function:    ServoSaveSubcode
*
* Description: Save current subcode at rw_seek_blk location
*
* Returns:     void
*************************************************************************/
void ServoSaveSubcode(void)
{
    /* read subcode and save it before we really stop the disk
        the position will be saved at rw_seek_blk */

    if(ServoReadSubcode(1,0xFF) == FAIL){
        Rw_seek_blk.msf.Minute = 0;
        Rw_seek_blk.msf.Second = 2;
        Rw_seek_blk.msf.Frame  = 0;
        return;
    }

    /* MSF (hex format) will be obtained from player and saved at rw_seek_block
        It is used for destination location of read sub-channel. */

    GetMsfFromServo((TimeS *)&Rw_seek_blk.msf,(BYTE *)PlayerAddrsPtr + 7);
}

/*********************************************************************
* Function:    ServoSearchISRCAndCatalog()
*
* Description: This is low level routine to send the search ISRC catalog
*              command to servo. The return is set by the
*              WAIT_SERVO_READY routine. This is used by the
*              calling routine to determine the pass/fail status
*              of the command.
*
* Returns:     1 = command failed
*              0 = command suceeded
*********************************************************************/
BOOL ServoSearchISRCAndCatalog(BYTE tno,BYTE req)
{
BYTE    isrc_catalog[10];

#if (EN_POWER_CONTROL)
    if (CurrentPowerMode == eStandbyPower)
    {
        if ( St_drv_ready  == TRUE)
        {/* There is disc inside and the related information is collected. */
            /* for wake up the power from stand by mode */
            SetDiscReadPowerMode();
        }
        else
            PowerControl(eIdlePower);
    }
#else
    ataPowerMode = IDLE_MODE;
#endif
    StopDecoder();                         /* turn off decoder */
    St_audio_pause   = FALSE;               /* clear audio pausing flag */
    Svr_pause_reqire = FALSE;

    HIF.PlayerIDC           = 0xD2;         /* set command IDC */
    HIF.PlayerAccept        = plrNewCmd;
    HIF.PlayerOpcode        = plrCmdSearchISRCCatalog;
    HIF.PlayerParameters[0] = tno;
    HIF.PlayerParameters[1] = req;
    HIF_CMD_MESSAGE(PRN_HIF_TO_SIF,0);

    Iram_Svr_player_mode = SEARCH_ISRC_CATALOG_MODE;
    if(WaitPlayerReady() == READY){
        switch(req) {
            case 0x02:      /* format 2, UPC */
                if (pdbGetCatalogCode(&isrc_catalog[0]) == TRUE ) return(PASS);
                break;
            case 0x03:      /* format 3, ISRC */
                if (pdbGetISRCCode(&isrc_catalog[0]) == HEX2BCD(tno)) return(PASS);
                break;
            case 0xF0:      /* format F0, RID   */
                if (pdbGetRIDCode(&isrc_catalog[0]) == HEX2BCD(tno)) return(PASS);
                break;
            default:
                break;
        }
    }
    return(FAIL);
}



/*********************************************************************
* Function:    ServoSetWriteSpeed()
*
* Description:
*
* sets the CD-ROM to single speed for write, CD-R media only
*
* Returns:     0
*********************************************************************/
BOOL ServoSetWriteSpeed(BYTE speed)
{
USHORT data;

    send_msg54(SEND_WRITE_MSG,'W','s',speed ,Current_read_sp);

    StopDecoder();

    Current_read_sp = speed;
    pdbSetSpeed((DiscSpeedT)speed);

    Iram_Buf_dram_addr = MSENSE_PAGE_2A+20;
    data = GetTransferSpeed(speed);
    WriteSDRAM((BYTE)(data>>8));           /* data rate */
    WriteSDRAM((BYTE)data);                /* get lo BYTE */

    while (HIF.PlayerAccept != plrIdleCmd)
    {
        ExitProcess();
    }

    HIF.PlayerIDC    = 0xD0;            /* set command IDC */
    HIF.PlayerAccept = plrNewCmd;
    HIF.PlayerOpcode = plrCmdSpeedChg;
    HIF_CMD_MESSAGE(PRN_HIF_TO_SIF,0);

    WaitPlayerReady();

    return(PASS);
}


/*********************************************************************
* SupportMaximumSpeed(void)
*
*
* Returns:  void
* Params:   void
*********************************************************************/
void SupportMaximumSpeed(void)
{
    if (pdbServoGetDiscDiameter() == eNoDisc)
    {
        return;
    }

            /* Get Max Read speed for Closed Session Disc */
            InitializeReadSpeed();

    //send_msg54(SEND_WRITE_MSG,'M','S',Disc_SupportSpeed.Write,Disc_SupportSpeed.Read);
}

/*********************************************************************
* Function:    SetReadSpeed()
*
* Description:
*   This routine will check the current servo speed
*   against the speed set by the MMC Set CD Speed
*   command.
*
*   Initiate a change of speed to the servo only when
*   the requested speed differs from the current speed.
*
*   Exceptions:
*
*   If the Set CD Speed request is equal to MAX_CAV supported
*   read speed and:
*   1) The Target is in an open session, the speed will be limited
*   to MAX_CLV_SPEED - wobble speed. This is due to the fact that
*   the servo can not seek over unwritten areas above 2x speed.
*   >>set speed to MAX_CLV_SPEED
*   2) If the Disc is Blank. This is due to the fact that the servo
*   can not seek over unwritten areas above 2x speed.
*   >>set speed to MAX_CLV_SPEED
*   3) If the Disc is a CD-I Disc the speed will be limited to 2X.
*   >>set speed to 2x
*
* Input: #High_read_sp = Max Speed-vu_cfg.h, 8, 4, 2, 1
*
* sets the CD-ROM to current setting
*********************************************************************/

BOOL SetReadSpeed(BYTE speed)
{
    SBYTE    i;
    BYTE    ret;
    BYTE    tolerance;
    BYTE    half_max_speed;
    BYTE    unbalance_check_speed, change_speed_check;

    speed = CheckSpeedLimit(speed);

#if (FIX_READ_SPEED == 1)
    if((speed != eSpStop) && (SpeedMD >= Initial_f))
    {
        if(Host_Requested_speed != 0)
        {
            speed = Host_Requested_speed;
        }
    }
#endif

    ret = TRUE;
    if(Current_read_sp == speed)
        return ret;

    tolerance = 0;
    change_speed_check = FALSE;
    unbalance_check_speed = 0;

    switch(pdbDiscLoaded())
    {
        case CD_DISC:
            {
                if(GetSpeedValue(Disc_Speed.Read) >= e32X)
                {// for high speed, check if change to the target speed for several times
                    half_max_speed = GetSpeedValue(MAX_READ_SPEED)/2;
                    change_speed_check = TRUE;
                }
            }
            break;

        case DVD_DISC:
            if(GetSpeedValue(Disc_Speed.Read) >= e12X)
            {// for high speed, check if change to the target speed for several times
                tolerance = 1;
                half_max_speed = GetSpeedValue(MAX_READ_DVD_SPEED)/2;
                change_speed_check = TRUE;
            }
            break;

#if (BD_ENABLE == 1)
        case BD_DISC:
            change_speed_check = FALSE;
            break;
#endif
#if (ENABLE_HDDVD == 1)
        case HD_DISC:
            change_speed_check = FALSE;
            break;
#endif
       default:
           return FALSE;
    }

    if(change_speed_check)
    {
        if(GetSpeedValue(speed) > GetSpeedValue(Current_read_sp))
        {// speed up
            if((GetSpeedValue(speed) - GetSpeedValue(Current_read_sp)) >= half_max_speed)
            {
                i = 0;
                do
                {
                    if(GetNextSpeed(Current_read_sp, i, ptrReadSpeedTablePointer, SpeedTableColumnSize) == speed)
                        break;
                    i++;
                }
                while(i < SpeedTableColumnSize);

                SpeedMD = Wait_f;
                BackUpSpeed = speed;
                speed = GetNextSpeed(Current_read_sp, (i / 2 + tolerance), ptrReadSpeedTablePointer, SpeedTableColumnSize);
            }
        }
    }

    SendMsgCn(SEND_HIF_CMD,2,0x800502,Disc_Speed.Read,speed); //for debug

    if(speed == eSpStop)
    {
        if(ServoStop() == FAIL)
        {
            if (svoIramVar.emergencyStopFlag == FALSE)
                ret = FALSE;
        }
    }
    else
    {
        /* Set speed control */
        if((St_cdrom_audio == TRUE)&&(svoVar_discKindT.Bit.DiscStd==eDiscCD))//CDDAE99 verify error
        {
            StopDecoder();
        }
        ServoSetSpeed(speed);
        ServoPause();
//        if(Knl_spinwt_counter == 0)
//            Knl_spinwt_counter = 10;
//        WaitTimer(Knl_spinwt_counter * 20);/* 0.2sec */
    }

    StartTimer(&RedLineTimer);
    SpeedCount = 0;
    OutputStream = 0;
    SpeedUpAccumulation = 0;

    return ret;
}

/*********************************************************************
* Function:    CheckSpeedLimit(*speed)
*
* Description: Sets the limit speed for special Disc
*
*********************************************************************/
BYTE CheckSpeedLimit(BYTE l_speed)
{
    switch(pdbDiscLoaded())
    {
#if (BD_ENABLE == 1)
        case BD_DISC:
            if (GetSpeedValue(l_speed) > GetSpeedValue(MAX_READ_BD_SPEED)) /* speed limit protect */
            {
                l_speed = MAX_READ_BD_SPEED;
            }
            break;
#endif
#if (ENABLE_HDDVD == 1)
        case HD_DISC:
            if (GetSpeedValue(l_speed) > GetSpeedValue(MAX_READ_HD_SPEED)) /* speed limit protect */
            {
                l_speed = MAX_READ_HD_SPEED;
            }
            break;
#endif
        case DVD_DISC:
            if (GetSpeedValue(l_speed) > GetSpeedValue(MAX_READ_DVD_SPEED)) /* speed limit protect */
            {
                l_speed = MAX_READ_DVD_SPEED;
            }
            break;
        case CD_DISC:
            if (GetSpeedValue(l_speed) > GetSpeedValue(MAX_READ_SPEED)) /* speed limit protect */
            {
                l_speed = MAX_READ_SPEED;
            }
            break;
    }

#if (PCB == 710)||(PCB == 711)||(PCB == 817)
    if ((GetSpeedValue(Disc_Speed.Read) < GetSpeedValue(l_speed)) &&
        (GetSpeedValue(Current_read_sp) <= GetSpeedValue(Disc_Speed.Read)))
#else
    if (GetSpeedValue(Disc_Speed.Read) < GetSpeedValue(l_speed))
#endif
    {
        l_speed = Disc_Speed.Read;
    }

    return(l_speed);
}

/*********************************************************************
* Function:    ServoSetSpeed()
*
* Description: Sets the CD-ROM to single speed
*
* Returns:     1 = command failed
*              0 = command suceeded
*********************************************************************/
BOOL ServoSetSpeed(BYTE speed)
{
USHORT data;

    SendMsgCn(DEBUG_SVO_SPD_MSG|DEBUG_CD_WRITE_MSG,1,0x331028,speed); // ***ServoSetSpeed

#if (FIX_READ_SPEED == 1)
    if((speed != eSpStop) && (SpeedMD >= Initial_f))
    {
        if(Host_Requested_speed != 0)
        {
            speed = Host_Requested_speed;
        }
    }
#endif
    if(Current_read_sp == speed)
    {
        return(PASS);
    }

#if (EN_POWER_CONTROL)
    if(Current_read_sp == eSpStop)
         SetDiscReadPowerMode(); //wake up power to set speed.
#endif

    Current_read_sp = speed;                    /* set to speed */
    pdbSetSpeed((DiscSpeedT)speed);

#if (WITHOUT_SERVO == 0)
    while (HIF.PlayerAccept != plrIdleCmd)
    {
        ExitProcess();
    }
#endif

    HIF.PlayerIDC    = 0xD2;                    /* set command IDC */
    HIF.PlayerAccept = plrNewCmd;
    HIF.PlayerOpcode = plrCmdSpeedChg;
    HIF.PlayerParameters[0] = 0;
    HIF.PlayerParameters[1] = 0;
    HIF_CMD_MESSAGE(PRN_HIF_TO_SIF,0);

    Iram_Buf_dram_addr = MSENSE_PAGE_2A+14;
    data = GetTransferSpeed(speed);
    WriteSDRAM((BYTE)(data>>8));                   /* data rate */
    WriteSDRAM((BYTE)data);                        /* get lo BYTE */

    data = (USHORT)Iram_Svr_player_mode;
    Iram_Svr_player_mode = EXECUTING_SINGLE_SPEED_MODE;

    if(WaitPlayerReady() == READY)
    {
        Iram_Svr_player_mode = (vBYTE)data;
#if (DPD_COMPENSATE_OFFSET == 1)
    #if (CEProduct == 0)   //For IT use only
        //add Pre-do DPD calibration after change speed.
        ServoCalibDpdWholeDisc();// improve whole disc pre calibration
    #endif
#endif
        return(PASS);
    }
    else
    {
        Iram_Svr_player_mode = (vBYTE)data;
        return(FAIL);
    }
}

/*************************************************************************
* Function:    ReadyLED
*
* Description: Access/Controls Ready LED (Typically Green)
*              ErrorLED and ReadyLED cannot be enabled at the same time
*              op_____LED values = 0:ON, 1:OFF
*
* Returns:     void
* Params:      LED_TYPE: LED_FLASH, LED_ON, LED_OFF
*************************************************************************/
void ReadyLED(BYTE led_status)
{
    vBOOL temp_IE;

#if (SUN_FUNC_TEST == 1)
    /* Do not show LED except drive under function test */
    if (FTInfo.DiableLEDController == TRUE)
        return;
#endif

    temp_IE = GET_INT_ENABLE_MASK();
    DISABLE_INTERRUPT();

    switch(led_status){

    case LED_FLASH3S:
        St_led_flash_error = 1;
        Iram_LED_Flash_timer = PlayLED_RATE;
        Iram_Gp_led_at_timer = Prevent_RATE;
        
	#if(SUN_FUNC_TEST == 0)
        opReadyLED = LED_ON;
	#else
              HAL_DSP_LED(ON);
       #endif		
        break;
    case LED_FLASH1S:
        if ( !St_led_flash_error){
        St_led_flash_error = 1;
        Iram_LED_Flash_timer = PlayLED_RATE;
        Iram_Gp_led_at_timer = Prevent_RATE1S;
	#if(SUN_FUNC_TEST == 0)
        opReadyLED = LED_ON;
	#else
              HAL_DSP_LED(ON);
       #endif	
        }
        break;
    case LED_FLASH:
        St_led_at_ready = 0;
        if (St_led_flash_ready){
            break;
        }
        St_led_flash_ready = 1;
        Iram_Gp_led_timer = ReadyLED_RATE;
        if (St_led_flash_error == 1) break;
	#if(SUN_FUNC_TEST == 0)
        opReadyLED = LED_ON;
	#else
              HAL_DSP_LED(ON);
       #endif	
        break;
    case LED_PLAY_FLASH:
        St_led_at_ready = 0;
        St_led_flash_ready = 1;
        Iram_Gp_led_timer = PlayLED_RATE;
        if (St_led_flash_error == 1) break;
	#if(SUN_FUNC_TEST == 0)
        opReadyLED = LED_ON;
	#else
              HAL_DSP_LED(ON);
       #endif	
        break;
    case LED_ON:
        St_led_at_ready = 0;
        St_led_flash_ready = 0;
        if (St_led_flash_error == 1) break;
	#if(SUN_FUNC_TEST == 0)
        opReadyLED = LED_ON;
	#else
              HAL_DSP_LED(ON);
       #endif	
        break;
    case LED_OFF:
        St_led_at_ready = 0;
        St_led_flash_ready = 0;
        if (St_led_flash_error == 1) break;

	#if(SUN_FUNC_TEST == 0)
        opReadyLED = LED_OFF;
	#else
              HAL_DSP_LED(OFF);
       #endif			
        break;
    case LED_AXferOn:
        St_led_at_ready = 1;
        Iram_Gp_led_at_timer  = AtLED_RATE;
        break;
    case LED_INITIAL_FLASH:
        St_led_at_ready = 0;
        if (St_led_flash_ready){
            break;
        }
        St_led_flash_ready = 1;
        Iram_Gp_led_timer = INITIAL_LED_RATE;
        if (St_led_flash_error == 1) break;
	#if(SUN_FUNC_TEST == 0)
        opReadyLED = LED_ON;
	#else
              HAL_DSP_LED(ON);
       #endif	
        break;
    case LED_REAL_W_FLASH:
        St_led_at_ready = 0;
        if (St_led_flash_ready){
            break;
        }
        St_led_flash_ready = 1;
        Iram_Gp_led_timer = REAL_WRITE_LED_RATE;
        if (St_led_flash_error == 1) break;
	#if(SUN_FUNC_TEST == 0)
        opReadyLED = LED_ON;
	#else
              HAL_DSP_LED(ON);
       #endif	
        break;
        break;
    case LED_TEST_W_FLASH:
        St_led_at_ready = 0;
        if (St_led_flash_ready){
            break;
        }
        St_led_flash_ready = 1;
        Iram_Gp_led_timer = TEST_WRITE_LED_RATE;
        if (St_led_flash_error == 1) break;
	#if(SUN_FUNC_TEST == 0)
        opReadyLED = LED_ON;
	#else
              HAL_DSP_LED(ON);
       #endif	
        break;
        break;
    }
    SET_INT_ENABLE_MASK( temp_IE );
}


#if (LED_NUM == 2)
/*************************************************************************
* Function:    WriteLED
*
* Description: Access/Controls Write LED (Typically Orange)
*              op_____LED values = 0:ON, 1:OFF
*
* Returns:     void
* Params:      LED_TYPE: LED_FLASH, LED_ON, LED_OFF
*************************************************************************/
void WriteLED(BYTE led_status)
{
    vBOOL temp_IE;

    temp_IE = GET_INT_ENABLE_MASK();
    DISABLE_INTERRUPT();

    switch(led_status){
/* LED FLASH */
    case LED_FLASH:
        if (St_led_flash_write){
            break;
        }
        St_led_flash_write = 1;
        Iram_Gp_led_timer = WriteLED_RATE;
        opWriteLED = LED_ON;
        break;
/* LED ON */
    case LED_ON:
        St_led_flash_write = 0;
        opWriteLED = LED_ON;
        break;

/* LED OFF */
    default:
        St_led_flash_write = 0;
        opWriteLED = LED_OFF;
        break;
    }
    SET_INT_ENABLE_MASK( temp_IE );
}
#endif /* LED_NUM == 2 */

#if (LED_NUM == 2)
/*************************************************************************
* Function:    ErrorLED
*
* Description: Access/Controls Error LED (Typically Red)
*              opLED values = 0:ON, 1:OFF
*
* Returns:     void
* Params:      LED_TYPE: LED_FLASH, LED_ON, LED_OFF
*************************************************************************/
void ErrorLED(BYTE led_status)
{
vBOOL temp_IE;

    temp_IE = GET_INT_ENABLE_MASK();
    DISABLE_INTERRUPT();

    switch(led_status){
/* LED ON */
    case LED_ON:
        St_led_flash_error = 0;
        opErrorLED = LED_ON;
        break;
    case LED_ON1S:
        St_led_flash_error = 1;
        Iram_LED_Flash_timer = ErrorLED_RATE;
	#if(SUN_FUNC_TEST == 0)
        opReadyLED = LED_OFF;
	#else
              HAL_DSP_LED(OFF);
       #endif	
        opErrorLED = LED_ON;
        break;

/* LED OFF */
    default:
        St_led_flash_error = 0;
        opErrorLED = LED_OFF;
        break;
    }
    SET_INT_ENABLE_MASK( temp_IE );
}
#endif /* LED_NUM == 2 */

/*********************************************************************
* Function:    ServoAdjustWritePowerStart
*
* Description: This is low level routine to send the Laser calibration
*               command to servo.
*
* Globals:     param1/2/3 - the target location passed from the caller.
*
* Returns:     1 = command failed
*              0 = command suceeded
*********************************************************************/
#if E_DIAGS == 1
BOOL ServoAdjustWritePowerStart()
{
tWatchSC ServoCmd;  /* Servo Command   */

    ataPowerMode = IDLE_MODE;                                    /* get the address of power mode set to idle mode*/
    StopDecoder();                                         /* turn off decoder */
    St_audio_pause   = FALSE;                               /* clear audio pausing flag */
    Svr_pause_reqire = FALSE;

    while (HIF.PlayerAccept != plrIdleCmd){
        ExitProcess();
    }

    ServoCmd.Cmd     = svoCmdWLDAdjust;
    ServoCmd.Len     = 0;

    if(plrHIFWatchReq(eWatch_ServoStatus,(BYTE *) &ServoCmd) == READY) {
        return(PASS);
    }else{
        return(FAIL);
    }
}
#endif /* E_DIAGS */

/*********************************************************************
*  WaitPlayerReady()
*
*   Called by commands in process and waiting for player. Main loop
*   is re-activated.
*
* Returns:  PASS or FAIL
**********************************************************************/
vBOOL WaitPlayerReady(void)
{
#if (WITHOUT_SERVO)
    return (PASS);
#endif

#if EN_POWER_CONTROL
    if(CurrentPowerMode == eStandbyPower)
        return (PASS);
#endif

    while(1)
    {
        /*---------------------------------------------------------------
            CHECK IF PLAYER IS READY
        ----------------------------------------------------------------*/
        if ((HIF.PlayerAccept != plrNewCmd) && (HIF.PlayerStatus != BUSY))
        {
            svoIramVar.emergencyStopFlag = FALSE;
            break;
        }

        /*---------------------------------------------------------------
            FOCUS DROP HANDLING
        ----------------------------------------------------------------*/
        if ((Iram_Svr_player_mode == EXECUTING_SEEK_MODE) && (svoIramVar.focusDropFlag == TRUE))
        {
            send_msg80(SEND_HIF_CMD,0x710066);

            svoIramVar.focusDropFlag = FALSE;
            svoIramVar.emergencyStopFlag = TRUE;
            svoIramVar.knlTimeupCountFlag = FALSE;

            while (HIF.PlayerStatus == BUSY)
                ExitProcess();
            svoIramVar.emergencyStopFlag = FALSE;
        }

#if (DEBUG_SVO_BUSY_EJECT == TRUE)
        /*---------------------------------------------------------------
            STOP WAITING IF THERE IS AN EJECT. ONLY FOR DEBUG WHEN
            SERVO HAS LARGE TIME OUT VALUES OR TAKES TOO LONG TO
            PROCESS SERVO COMMANDS
        ----------------------------------------------------------------*/
        if(EjectKeyInfo.PressedValid)
        {
            svoIramVar.emergencyStopFlag = TRUE;

            while (HIF.PlayerStatus == BUSY)
                ExitProcess();
            svoIramVar.emergencyStopFlag = FALSE;
        }
#endif

        ExitProcess();
    }

#if (PRN_HIF_TO_SIF)
    HIF_CMD_MESSAGE(PRN_HIF_TO_SIF,1);
#endif

    return(HIF.PlayerStatus);
}

/*********************************************************************
* Function:    GetHIFToPlayerStatus
*
* Description: Returns player status

* Returns:     HIF.PlayerStatus
*********************************************************************/
BOOL GetHIFToPlayerStatus(void)
{
    return(HIF.PlayerStatus);
}


/*********************************************************************
* Function:    ServoLaserPowerCalibration
*
* Description: This is low level routine to send the Laser calibration
*               command to servo.
*
* Globals:     param1/2/3 - the target location passed from the caller.
*
* Returns:     1 = command failed
*              0 = command suceeded
*********************************************************************/
#if PersistentTable_Revision>=8
BOOL ServoLaserPowerCalibration(BYTE calib_no, BYTE mode, BYTE gain)
#else
BOOL ServoLaserPowerCalibration(BYTE calib_no)
#endif
{
    tWatchSC ServoCmd;  /* Servo Command   */

    ataPowerMode = IDLE_MODE;                                    /* get the address of power mode set to idle mode*/
    StopDecoder();                                         /* turn off decoder */
    St_audio_pause   = FALSE;                               /* clear audio pausing flag */
    Svr_pause_reqire = FALSE;

    while (HIF.PlayerAccept != plrIdleCmd){
        ExitProcess();
    }

    ServoCmd.Cmd     = svoCmdLDAdjust;
#if PersistentTable_Revision>=8
    ServoCmd.Len     = 3;
    ServoCmd.Para[0] = calib_no;
    ServoCmd.Para[1] = mode;
    ServoCmd.Para[2] = gain;
#else
    ServoCmd.Len     = 1;
    ServoCmd.Para[0] = calib_no;
#endif
    if(plrHIFWatchReq(eWatch_ServoStatus,(BYTE *) &ServoCmd) == READY) {
        return(PASS);
    }else{
        return(FAIL);
    }
}

/*********************************************************************
* Function:    ServoCalibration
*
* Description: This is low level routine to send the focus balance calibration
*               command to servo.
*
* Returns:     1 = command failed
*              0 = command suceeded
*********************************************************************/
BOOL ServoCalibration(BYTE calib_no)
{
    tWatchSC ServoCmd;  /* Servo Command   */

    ataPowerMode = IDLE_MODE;                           /* get the address of power mode set to idle mode*/
    StopDecoder();                                      /* turn off decoder */
    St_audio_pause   = FALSE;                           /* clear audio pausing flag */
    Svr_pause_reqire = FALSE;

    while (HIF.PlayerAccept != plrIdleCmd)
    {
        ExitProcess();
    }

    ServoCmd.Cmd     = svoCmdDiagnostic;
    ServoCmd.Len     = 1;
    ServoCmd.Para[0] = calib_no;

    if (plrHIFWatchReq(eWatch_ServoStatus,(BYTE *) &ServoCmd) == READY)
    {
        return(PASS);
    }
    else
    {
        return(FAIL);
    }
}

#if (CALIBRATION_MANAGEMENT == 1)
/*********************************************************************
* Function:    ServoReCalibrate
*
* Description: This is low level routine to send the focus balance calibration
*               command to servo.
*
* Returns:     1 = command failed
*              0 = command suceeded
*********************************************************************/
BOOL ServoReCalibrate(void)
{
    tWatchSC ServoCmd;  /* Servo Command   */

    ataPowerMode = IDLE_MODE;                           /* get the address of power mode set to idle mode*/
    StopDecoder();                                      /* turn off decoder */
    St_audio_pause   = FALSE;                           /* clear audio pausing flag */
    Svr_pause_reqire = FALSE;

    while (HIF.PlayerAccept != plrIdleCmd)
    {
        ExitProcess();
    }

    ServoCmd.Cmd     = svoCmdReCalibrate;
    ServoCmd.Len     = 0;

    if (plrHIFWatchReq(eWatch_ServoStatus,(BYTE *) &ServoCmd) == READY)
    {
        return(PASS);
    }
    else
    {
        return(FAIL);
    }
}
#endif /* CALIBRATION_MANAGEMENT */

#if (HORROR_DISC_IMPROVEMENT == 1) /* HORROR_DISC_IMPROVEMENT */
/*********************************************************************
* Function:    ServoChgRFParameter
*
* Description: This is low level routine to send the RF paraters change
*               command to servo.
*
* Returns:     1 = command failed
*              0 = command suceeded
*********************************************************************/
BOOL ServoChgRFParameter(void)
{
    tWatchSC ServoCmd;  /* Servo Command   */

    ataPowerMode = IDLE_MODE;                           /* get the address of power mode set to idle mode*/
    StopDecoder();                                      /* turn off decoder */
    St_audio_pause   = FALSE;                           /* clear audio pausing flag */
    Svr_pause_reqire = FALSE;

    while (HIF.PlayerAccept != plrIdleCmd)
    {
        ExitProcess();
    }

    ServoCmd.Cmd     = svoCmdChgRFParameter;
    ServoCmd.Len     = 0;

    if (plrHIFWatchReq(eWatch_ServoStatus,(BYTE *) &ServoCmd) == READY)
    {
        return(PASS);
    }
    else
    {
        return(FAIL);
    }
}
#endif /* HORROR_DISC_IMPROVEMENT */

#if (P_DIAGS==1)
/*********************************************************************
* Function:    ServoForceDiscKind
*
* Description: This is low level routine to send the force disc kind
*               command to servo.
*
* Returns:     1 = command failed
*              0 = command suceeded
*********************************************************************/
BOOL ServoForceDiscKind(BYTE force_flag, USHORT disc_kind)
{
    tWatchSC ServoCmd;  /* Servo Command   */

    ataPowerMode = IDLE_MODE;                           /* get the address of power mode set to idle mode*/
    StopDecoder();                                      /* turn off decoder */
    St_audio_pause   = FALSE;                           /* clear audio pausing flag */
    Svr_pause_reqire = FALSE;

    while (HIF.PlayerAccept != plrIdleCmd)
    {
        ExitProcess();
    }

    ServoCmd.Cmd     = svoCmdForceDiscKind;
    ServoCmd.Len     = 4;
    ServoCmd.Para[0] = force_flag;
    ServoCmd.Para[1] = HBYTE(disc_kind);
    ServoCmd.Para[2] = LBYTE(disc_kind);

    if (plrHIFWatchReq(eWatch_ServoStatus,(BYTE *) &ServoCmd) == READY)
    {
        return(PASS);
    }
    else
    {
        return(FAIL);
    }
}
#endif

#if ((PCB == 690)||(PCB == 692)||(PCB == 710)||(PCB == 711)||(PCB == 812)||(PCB == 830)||(PCB == 1600)||(PCB == 815)||(PCB == 817))
/*********************************************************************
* Function:    ServoAdjFocDCActSens
*
* Description: Calculate foc_act_DC_sens after disc judge and store
               this parameter in the persistent table
*
* Returns:     1 = command failed
*              0 = command suceeded
*********************************************************************/
BOOL ServoAdjFocDcActSens(BYTE disc_type, BYTE ExeCmd)
{
    tWatchSC ServoCmd;  /* Servo Command   */

    ataPowerMode = IDLE_MODE;                           /* get the address of power mode set to idle mode*/
    StopDecoder();                                      /* turn off decoder */
    St_audio_pause   = FALSE;                           /* clear audio pausing flag */
    Svr_pause_reqire = FALSE;

    while (HIF.PlayerAccept != plrIdleCmd)
    {
        ExitProcess();
    }

    ServoCmd.Cmd     = svoCmdAdjFocActDcActSens;
    ServoCmd.Len     = 2;
    ServoCmd.Para[0] = disc_type;
    ServoCmd.Para[1] = ExeCmd;

    if (plrHIFWatchReq(eWatch_ServoStatus,(BYTE *) &ServoCmd) == READY)
    {
        return(PASS);
    }
    else
    {
        return(FAIL);
    }
}
#endif

#if ((DETECT_ECC_DISC == 1)||(DVD_UNBALANCE_DISC_CHECK == 1) || (CD_UNBALANCE_DISC_CHECK == 1)|| (BD_UNBALANCE_DISC_CHECK == 1))
/*********************************************************************
* Function:  CheckUnbalancedEccentricityDisc()
* Description: Check the current speed against max speeds for
*           Eccentricity and Unbalanced Discs. This check is
*           valid for both Read and Write Speeds.
* Exceptions:
* Input: Speed to check
* Output:   Speed after checks and update
*********************************************************************/
BYTE CheckUnbalancedEccentricityDisc(BYTE Curspeed)
{
BYTE    spdt, spdindx;

    /* Get Speed Index to for accessing the Unbalanced and Eccentric disc speed tables */
    switch(pdbDiscLoaded())
    {
        case   CD_DISC:
            spdindx = eECC_UNB_MAX_SPD_CD;
            break;
        case   DVD_DISC:
            if(pdbGetDiscKind() == eDVDRAMDisc)
                spdindx = eECC_UNB_MAX_SPD_RAM;
            else
                spdindx = eECC_UNB_MAX_SPD_DVD;
            break;
        case   BD_DISC:
            spdindx = eECC_UNB_MAX_SPD_BD;
            break;
        default:
            spdindx = eECC_UNB_MAX_SPD_CD;
            break;
    }

#if (DETECT_ECC_DISC == 1)
/*** Check for Eccentricity Disc ***/
    if (svoVar_eccDiscNum != 0)         /* Check if Eccentric Disc is in system */
    {
        spdt = ECCDiscSpd_tbl[spdindx][svoVar_eccDiscNum];  /* Get Initial Speed based on Eccentricity */
        if (spdt < Curspeed)            /* Check if Eccentric disc speed is less than current speed */
        {
            Curspeed = spdt;                /* Update Speed based on Eccentric disc */
            SendMsgCn(DEBUG_CD_WRITE_MSG,1,0x37000D,B1B((Curspeed&(eSpeedValue))));
        }
    }
#endif // DETECT_ECC_DISC

#if((CD_UNBALANCE_DISC_CHECK == 1)||(DVD_UNBALANCE_DISC_CHECK == 1)||(BD_UNBALANCE_DISC_CHECK == 1))
/*** Check for Unabalanced Disc ***/
    if (svoVar_unbalanceDiscNum != 0)   /* Check if Unbalanced Disc is in system */
    {
        spdt = UnBalDiscSpd_tbl[spdindx][svoVar_unbalanceDiscNum];  /* Get Initial Speed based on Balance  */
        if (spdt < Curspeed)            /* Check if Unbalanced disc speed is less than current speed */
        {
            Curspeed = spdt;            /* Update Speed based on Unbalanced disc */
        }
    }

#endif /* CD_UNBALANCE_DISC_CHECK == 1 */

#if ((EN_WOBBLE == 1) && (WOBBLE_READ_ON_DVD_OR_CD == 1))
    /*** Need to check limits on CD-RW media ***/
    /*** Some media has minimum speed limits ***/
    /*** and others have max speed limits    ***/
    if(pdbGetDiscKind() == eCDRWDisc)
    {
        switch (pdbGetATIPDiscSubType())
        {
        case 0x00:                      /* CD-RW: Normal-Speed */
            if (Curspeed > MAX_CDRW_SPD)
            {
            Curspeed = MAX_CDRW_SPD;    /* CD-RW Speed Limit */
            }
            break;
            case 0x01:                  /* HS-RW: High-Speed */
            if (Curspeed > MAX_HSRW_SPD)
            {
            Curspeed = MAX_HSRW_SPD;    /* HS-RW Speed Limit */
            }
            break;
        case 0x02:                      /* US-RW: Ultra-Speed */
        case 0x03:                      /* US-RW: Ultra-Speed */
            if (Curspeed < MIN_USRW_SPD)
            {
            Curspeed = MIN_USRW_SPD;    /* US-RW Speed Limit */
            }
            break;
        }
    }
#endif//((EN_WOBBLE == 1) && (WOBBLE_READ_ON_DVD_OR_CD == 1))

return (Curspeed);  /* Return the maximum speed based on input and Disc */
}
#endif // (DETECT_ECC_DISC == 1) || (DVD_UNBALANCE_DISC_CHECK == 1) || (CD_UNBALANCE_DISC_CHECK == 1)

#if (BCA_ENABLE == 1)
/*********************************************************************
* Function:    ServoBCASeek
*
* Description: This is low level routine to do sled jump to target
*              sled position
*
* Returns:     1 = command failed
*              0 = command suceeded
*********************************************************************/
BOOL ServoBCASeek(SHORT steps, BYTE flag)
{
    tWatchSC ServoCmd;  /* Servo Command   */

    ataPowerMode = IDLE_MODE;   /* get the address of power mode set to idle mode*/
    St_audio_pause   = FALSE;   /* clear audio pausing flag */
    Svr_pause_reqire = FALSE;

    while (HIF.PlayerAccept != plrIdleCmd)
    {
        ExitProcess();
    }

    ServoCmd.Cmd     = svoCmdBCASeek;
    ServoCmd.Len     = 3;
    ServoCmd.Para[0] = HBYTE(steps);    /* sled jump steps high byte */
    ServoCmd.Para[1] = LBYTE(steps);    /* sled jump steps low byte */
    ServoCmd.Para[2] = flag;            /* tracking on/off */

    if (plrHIFWatchReq(eWatch_ServoStatus,(BYTE *) &ServoCmd) == READY)
    {
        return(PASS);
    }
    else
    {
        return(FAIL);
    }
}
#endif


/*------------------------------------------------------------------------*/
/* The end of file                                                        */
/*------------------------------------------------------------------------*/
