/****************************************************************************
*                (c) Copyright 2000 - 2005 SUNEXT DESIGN, INC.
*                           All Right Reserved
*
*
*   FILE NAME       bdDiscRd.c
*
*   DESCRIPTION     Main BD read engine to set and control the BD DSP to read
*                   the disc.
*
*   $Revision: 62 $
*   $Date: 11/03/28 2:36p $
*
**************************** SUNEXT CONFIDENTIAL *******************************/

#include ".\common\globtype.h"
#include ".\common\globals.h"
#include ".\common\system.h"
#include ".\common\chip.h"

#include ".\hif\gp_ram.h"
#include ".\at_equ.h"

#include ".\bd\bdvars.h"
#include ".\bd\bdseek.h"
#include ".\bd\bddiscrd.h"
#include ".\bd\bdMacro.h"
#include ".\bd\bddiag.h"

#include ".\AL\REG_HAL.h"
#include ".\AL\REG_bd_codec.h"
#include ".\AL\REG_demod.h"
#include ".\AL\REG_global.h"
#include ".\Al\HAL_bd_codec.h"
#include ".\AL\HAL_global.h"
#include ".\al\reg_intctrl.h"
#include ".\common\intctl.h"

#if (BD_ENABLE == 1)
#include ".\servo\SVO.H"
#endif //(BD_ENABLE == 1)

#if (CALIBRATION_MANAGEMENT == 1)
#include ".\servo\svo_com.h"
#include ".\servo\svo_calib.h"
#endif
#if (XDFCT_ENABLE == 1)
#include ".\al\LAL_dsp.h"
#include ".\servo\svo_dsp.h"
#include ".\servo\svo_lib.h"
#endif //(XDFCT_ENABLE == 1)
#if (EN_POWER_CONTROL)
#include ".\hif\powerctl.h"
#endif

#if (SUN_FUNC_TEST == 1)
#include ".\oem\sun710_function_test.h"
#endif

/*******************************************************************
    Local PROTOTYPES
*******************************************************************/
DiscAccessStatusE bdTransferDataToBuffer(void);
DiscAccessStatusE GetBdError(void);
BYTE bdExtendBuffering(void);
#if (BypassDecoderEr == 1)
BYTE BDValidBypassBDDecoderEr(void);
#endif
#if (BD_READ == 1)
/*******************************************************************************
    GLOBAL VARIABLES
*******************************************************************************/
vBOOL SDHoldSts;
vBOOL FDHoldSts;

/*******************************************************************************
    FUNCTION DEFINITIONS
*******************************************************************************/
/*****************************************************************************

    FUNCTION        bdStartDiscTransfer

    DESCRIPTION     Main interface to the disc transferring engine. Decodes
                    the type of transfer and initiates disc access

    GLOBAL INPUTS   bdDiscXferParameter

    RETURNS         Status of the disc access during buffering

*****************************************************************************/
DiscAccessStatusE bdStartDiscTransfer(void)
{
    DiscAccessStatusE    DiscReadStatus;    /* Return status of disc access */
    ULONG   DelayTimer;

    /*---------------------------------------------------------------
        Initialize local variables
    ----------------------------------------------------------------*/
    DiscReadStatus = BUF_GOOD;

    /*---------------------------------------------------------------
        Return fail for a transfer request of 0 blocks
    ----------------------------------------------------------------*/
    if (bdDiscRequest.ClusterCount == 0L)
        return(GEN_FAIL);

    /*---------------------------------------------------------------
        Determine the transfer type
    ----------------------------------------------------------------*/
    switch (bdDiscRequest.Type)
    {
        case INIT_READ:
        case INIT_READ_AHEAD:

            if (BD_BUFFERING() == ON)
            {
                /*---------------------------------------------------------------
                    The BD Codec is not stopped and a block could potentially
                    come in before the INIT_READ is processed. Stop the BD Codec.
                ----------------------------------------------------------------*/
                bdStopDiscTransfer();
            }

            StartTimer(&DelayTimer);
            SDHoldSts = (vBOOL)READ_FIELD(HAL_BDC_HLDSDS);
            FDHoldSts = (vBOOL)READ_FIELD(HAL_BDC_HLDFDS);
            while (SDHoldSts == 0 || FDHoldSts == 0)
            {
                HAL_BD_CODEC_Disable_Buffering();

                if (ReadTimer(&DelayTimer) >= 100)
                {
                    send_msg5S(SEND_READ_DVD_MSG, "SDFD HLDS Err 1");
                    break;
                }
                SDHoldSts = (vBOOL)READ_FIELD(HAL_BDC_HLDSDS);
                FDHoldSts = (vBOOL)READ_FIELD(HAL_BDC_HLDFDS);
            }

            HAL_BD_CODEC_DISABLE_DECODER();

            /*---------------------------------------------------------------
               Initialize the transfer parameters and then begin transferring
            ----------------------------------------------------------------*/
            bdInitializeDiscTransfer();
            DiscReadStatus = bdTransferDataToBuffer();

            return DiscReadStatus;
            break;

        case EXTEND_READ:
        case EXTEND_READ_AHEAD:

            if ( bdExtendBuffering() == TRUE )
                DiscReadStatus = bdTransferDataToBuffer();
            else
                DiscReadStatus = EXTEND_READ_FAIL;

            return DiscReadStatus;
            break;

        case WRITE:
            /*---------------------------------------------------------------
                Write code
            ----------------------------------------------------------------*/
            break;

        default:
            break;

    }

    return(GEN_FAIL);
}
/*******************************************************************************

    FUNCTION        bdInitializeDiscTransfer

    DESCRIPTION     Initializes the BD disc transfer timer, flags and transfer
                    structure

    GLOBAL INPUTS   BD disc transfer parameters

********************************************************************************/
void bdInitializeDiscTransfer()
{
    STOP_BD_CLUSTER_TIMER();
    bdResetDiscFlags();

    /*---------------------------------------------------------------
        Initialize buffer to track last incoming PSN
    ----------------------------------------------------------------*/
    bdLastDecodedPSN = bdDiscRequest.TargetID.Long - 1L;

    /*---------------------------------------------------------------
        Intialize request state
    ----------------------------------------------------------------*/
    bdDiscRequest.ClustersBuffered  = 0L;
    bdDiscRequest.State             = WAITING_FOR_TRANSFER;
    bdDecFlag.fEXTEND_READ          = FALSE;
}
/*******************************************************************************

    FUNCTION        bdStopDiscTransfer

    DESCRIPTION     Stops the current transfer of data from disc to buffer

********************************************************************************/
void bdStopDiscTransfer()
{

    BYTE InterruptState;

    send_msg80(SEND_READ_DVD_MSG,0x71009B);

#if (EN_POWER_CONTROL)
    if(BDDecCLKStop)
        return;
#endif

#if (XDFCT_ENABLE == 1)
    bdStopSpecialDiscHandle();
#endif //(XDFCT_ENABLE == 1)

    /*---------------------------------------------------------------
        Disable global interrupts
    ----------------------------------------------------------------*/
    InterruptState = GET_INT_ENABLE_MASK();
    DISABLE_INTERRUPT();

    /*---------------------------------------------------------------
        Clear BD buffering controls and interrupts
    ----------------------------------------------------------------*/
    HAL_BD_CODEC_Disable_Buffering();           /* Disable BD decoder buffering */
    HAL_BD_CODEC_DISABLE_DECODER_INTS();        /* Disable all BD decoder interrupts       */
    STOP_BD_CLUSTER_TIMER();                    /* Stop the cluster interrupt timer   */

    /*---------------------------------------------------------------
        Return to normal mode
    ----------------------------------------------------------------*/
    if ( bdDecFlag.fREADING_PIC )
    {
        HAL_GLOBAL_ClrPicMode();
    }

    /*---------------------------------------------------------------
        Change disc transfer state if not complete
    ----------------------------------------------------------------*/
    if(bdDiscRequest.State != TRANSFER_COMPLETE)
        bdDiscRequest.State = TRANSFER_STOPPED;

    /*---------------------------------------------------------------
        If read ahead process was on insure that flag is set
        to indicate it is now off
    ----------------------------------------------------------------*/
    bdDecFlag.fREAD_AHEAD = FALSE;

    /*---------------------------------------------------------------
       Enable global interrupts
    ----------------------------------------------------------------*/
    SET_INT_ENABLE_MASK(InterruptState);
}
/*******************************************************************************

    FUNCTION        bdResetDiscFlags

    DESCRIPTION     Clears all neccessary bd error, interrupt, and read flags
                    prior to buffering

********************************************************************************/
void bdResetDiscFlags()
{

    /*---------------------------------------------------------------
        Clear BD error flags
    ----------------------------------------------------------------*/
    bdErrorFlag.fSEQ_ID_ERROR       = CLEAR;
    bdErrorFlag.fUNCOR_LDC_ECC      = CLEAR;
    bdErrorFlag.fUNCOR_BIS_ECC      = CLEAR;
    bdErrorFlag.fBCA_ERROR          = CLEAR;
    bdErrorFlag.fEDC_ERROR          = CLEAR;
    bdErrorFlag.fID_ERROR           = CLEAR;
    bdErrorFlag.fSEEK_FAIL          = CLEAR;
    bdErrorFlag.fTID_MISS           = CLEAR;
    bdErrorFlag.fUNAUTH_READ_ERROR  = CLEAR;
    bdErrorFlag.fID_MISMATCH        = CLEAR;
    /*---------------------------------------------------------------
        Clear BD cluster interrupt flags
    ----------------------------------------------------------------*/
    bdDecIntFlag.fBCA_DONE_INT          = CLEAR;
    bdDecIntFlag.fECC_CLUSTER_INT       = CLEAR;
    bdDecIntFlag.fECC_ALL_DONE_INT      = CLEAR;
    bdDecIntFlag.fSTP_ADDR_FOUND_INT    = CLEAR;

    /*---------------------------------------------------------------
        Clear BD timeout flag
    ----------------------------------------------------------------*/
    bdClusterInterruptTimer.fTIMEOUT    = CLEAR;
}
/*******************************************************************************

    FUNCTION        bdTransferDataToBuffer

    DESCRIPTION     Starts the transferring of data from disc to buffer

    RETURNS         Status of the disc access during buffering

    NOTE            CURBUF is initialized in ClearDataBuffer().
********************************************************************************/
DiscAccessStatusE bdTransferDataToBuffer(void)
{
    ULONG               StartPSN;
    ULONG               StopPSN;
    ULONG               RemainingClusters;
    ULONG               DelayTimer;
    ULONG               ErrorTimer;
    BYTE                InterruptState;
    DiscAccessStatusE   DiscReadStatus;
    eDiscKind           disc;

    /*--------------------------------------------------------------
        initial local variables
    ---------------------------------------------------------------*/

    /*---------------------------------------------------------------
        Initialize retry count.
    ----------------------------------------------------------------*/
    if(bdDecFlag.fREADING_PIC == SET)
        bdDecFlag.fREAD_AHEAD = FALSE;


    if ( bdDecFlag.fREAD_AHEAD == TRUE )
    {
    #if (BypassDecoderEr == 1)
        bdReadRetryCount   = 3;
    #else
        bdReadRetryCount   = 0;
    #endif
    }
    else
    {
        if (( bdDecFlag.fREADING_PIC ) && ((disc != eBDROMDisc) && (disc != eBDROMDLDisc)))
            bdReadRetryCount   = MAX_BD_READ_RETRIES;
        else
            bdReadRetryCount   = MAX_BD_READ_RETRIES;
    }


#if (E_DIAGS == 1)
    if(READ_LDC_BIS == 1) /* When ERRCO reading always 5 retries */
        bdReadRetryCount = 5;
#endif

    HAL_BD_CODEC_ENABLE_DECODER();              /* Turn on bd decoder */

    HAL_BD_CODEC_STREAM_PASS_ALL_OFF();             /* Disable Stream Pass All function */

    /*---------------------------------------------------------------
        Enter into PIC or BCA mode
    ----------------------------------------------------------------*/
    disc = pdbGetDiscKind();

    if(bdDecFlag.fREADING_PIC)
    {
        WRITE_FIELD(HAL_BDC_ESFREMSIS, halBDC_ESF_Off);      //for PIC read
        WRITE_FIELD(HAL_BDC_ESFREMDIS, halBDC_ESF_Off);      //for PIC read
        WRITE_FIELD(HAL_BDC_ESFFILLSG, halBDC_ESF_Off);      //for PIC read
        WRITE_FIELD(HAL_BDC_ESFFILLDG, halBDC_ESF_Off);      //for PIC read
    }

    /*---------------------------------------------------------------
        Main BD buffering loop. Continues until all requested blocks
        are buffered except for the following conditions:

            1. All host blocks are in the buffer and read ahead
               blocks are now being buffered

            2. A BD error has occured and retry count is zero
    ----------------------------------------------------------------*/
    while( bdDiscRequest.State != TRANSFER_COMPLETE )
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
        /*---------------------------------------------------------------
             If this is an extended read do not set up for buffering and
             go directly to monitor the current buffering process
        ----------------------------------------------------------------*/
        if (bdDecFlag.fEXTEND_READ == FALSE)
        {
            if (svo_BDR_RE_PICReading == SET)
            {
                HAL_GLOBAL_SetPicMode();

                //improve target miss in PIC area
                StartTimer(&DelayTimer);
                SDHoldSts = (vBOOL)READ_FIELD(HAL_BDC_HLDSDS);
                FDHoldSts = (vBOOL)READ_FIELD(HAL_BDC_HLDFDS);
                while (SDHoldSts == 0 || FDHoldSts == 0)
                {
                    HAL_BD_CODEC_Disable_Buffering();

                    if (ReadTimer(&DelayTimer) >= 100)
                    {
                        send_msg5S(SEND_READ_DVD_MSG, "SDFD HLDS Err 4");
                        break;
                    }
                    SDHoldSts = (vBOOL)READ_FIELD(HAL_BDC_HLDSDS);
                    FDHoldSts = (vBOOL)READ_FIELD(HAL_BDC_HLDFDS);
                }

                HAL_BD_CODEC_DISABLE_DECODER();         /* Reset bd decoder */
                HAL_BD_CODEC_ENABLE_DECODER();
                HAL_BD_CODEC_STREAM_PASS_ALL_OFF();     /* Disable Stream Pass All function */
            }

            /*---------------------------------------------------------------
                Stop buffering
            ----------------------------------------------------------------*/
            if ( BD_BUFFERING() == ON )
                bdStopDiscTransfer();
            /*---------------------------------------------------------------
                Eject press
            ----------------------------------------------------------------*/            if(EjectKeyInfo.PressedValid || TrayManagerInfo.Executing)
            if(EjectKeyInfo.PressedValid || TrayManagerInfo.Executing)
            {
                //BUILD_SENSE(0x02,0x3A,0x00,0x07);            /* MEDIUM NOT PRESENT */
                return EJECT_PRESS;
            }

            /*---------------------------------------------------------------
                  Check for errors
            ----------------------------------------------------------------*/
            if ( BD_CLUSTER_ERROR || BD_BUFFER_ERROR )
            {
                DiscReadStatus = GetBdError();

                if (((bdReadRetryCount == 0) &&  (!((Iram_Cmd_block.bf[0] == SBC_READ_10) && (St_cmd_inproc == TRUE))))
                      || (DiscReadStatus == UNAUTH_RD_FAIL))
                {
                    /*---------------------------------------------------------------
                          It isn't executing Read10 command and maximum number of retries have
                          performed or unauthorized read (no retries necessary) attempted.
                    ----------------------------------------------------------------*/
                     bdStopDiscTransfer();                      /* stop disc transferring       */
                     HAL_BD_CODEC_CLEAR_DECODER_STATUS();       /* clear all remaining status   */
                     Svr_pause_reqire = TRUE;                   /* Set pause request to servo   */
                     Iram_Svr_player_mode = PAUSING_MODE;       /* Set pause mode for player    */
                     return(DiscReadStatus);                    /* Return the disc read error   */
                }
#if (CALIBRATION_MANAGEMENT == 1)
#if (CEProduct == 1)
                if (((bdReadRetryCount >=1)&&(bdReadRetryCount <= MAX_BD_READ_RETRIES -4)) && calGetCalRequired(bdDiscRequest.TargetLayer & LAYERMASK, Iram_stp_CurrentPosition, ITEM_ALL, READ_MODE))
#else
                if (((bdReadRetryCount >=1)&&(bdReadRetryCount <= MAX_BD_READ_RETRIES -1)) && calGetCalRequired(bdDiscRequest.TargetLayer & LAYERMASK, Iram_stp_CurrentPosition, ITEM_ALL, READ_MODE))                //20093019_n.25==add ServoReCalibrate times during retry.
#endif
                {
                     SendMsgCnB(SEND_ATAPI_CMD,4,0x80041A,B4B(bdReadRetryCount)); // BD calibrate

                    (void)ServoReCalibrate();
                }
#endif /* CALIBRATION_MANAGEMENT */
#if (HORROR_DISC_IMPROVEMENT == 1)  // == start == Change servo parameters to improve BDROM performance when there is white defect/oil on disc surface
                    (void)ServoChgRFParameter();
#endif    //== end == Change servo parameters to improve BDROM performance when there is white defect/oil on disc surface
                if(bdReadRetryCount)
                    --bdReadRetryCount;
            }

#if (BypassDecoderEr == 1)
            if(bdDecFlag.fSTREAMING == TRUE)
            {
                IGNORE_DECODER_ERROR = FALSE;

                if((BDValidBypassBDDecoderEr() == TRUE)&&(bdDecFlag.fREADING_LEADIN != TRUE))
                {
                    //Because ABEX BD playing rate is almost 40M/s, so condition(DataBuffer.SectorCount < 0x40) will often be true.
                    //This will let bypass working at first retry. Cancel code below temporarily.
                    //if(bdLastDecodedPSN > 0x100B00)
                    //{
                    //    if(DataBuffer.SectorCount < 0x40)
                    //    {
                    //        IGNORE_DECODER_ERROR = TRUE;
                            //send_msg5S(1, "Bypass 1");
                    //    }
                    //}

                    if(bdReadRetryCount < 2)
                    {
                        if(DataBuffer.SectorCount < 0x40)
                        {
                            IGNORE_DECODER_ERROR = TRUE;
                            //send_msg5S(1, "Bypass 2");
                        }
                    }
                }
            }
#endif




            /*---------------------------------------------------------------
                Disable global interrupts
            ----------------------------------------------------------------*/
            InterruptState = GET_INT_ENABLE_MASK();
            DISABLE_INTERRUPT();

            /*--------------------------------------------------------
                Set/Reload the start address for the seek and remaining
                blocks to transfer.

                NOTE: For an initial read bdLastDecodedPSN was set in
                function bdInitializeDiscTransfer(), where
                bdLastDecodedPSN = bdDiscRequest.TargetID - 1L;
            ---------------------------------------------------------*/
            StartPSN = bdLastDecodedPSN + 1L;
            RemainingClusters = bdDiscRequest.ClusterCount - bdDiscRequest.ClustersBuffered;

            if (bdDecFlag.fREADING_PIC == FALSE || pdbGetDiscKind() == eBDROMDisc || pdbGetDiscKind() == eBDROMDLDisc)
            {
                StopPSN = StartPSN + RemainingClusters * SECTORS_PER_CLUSTER - 1L;
            }
            else
            {
                StopPSN = StartPSN + RemainingClusters * 2 - 1;
            }

            /*--------------------------------------------------------
                Set the current disc buffer pointer
            ---------------------------------------------------------*/
            HAL_BD_CODEC_SetDiscPtr(DataBuffer.CurDiscPtr);

            /*---------------------------------------------------------------
                Enable global interrupts
            ----------------------------------------------------------------*/
            SET_INT_ENABLE_MASK(InterruptState);

            /*---------------------------------------------------------------
                Load Stream Start & Stop Address
            ----------------------------------------------------------------*/
            if (bdDecFlag.fREADING_PIC == CLEAR || pdbGetDiscKind() == eBDROMDisc || pdbGetDiscKind() == eBDROMDLDisc)
            {
                HAL_BD_CODEC_SetStartAddress((StartPSN >> 5));
                HAL_BD_CODEC_SetStopAddress((StopPSN >> 5));
            }
            else
            {
                HAL_BD_CODEC_SetStartAddress((StartPSN >> 1));
                HAL_BD_CODEC_SetStopAddress((StopPSN >> 1));
            }

            /*---------------------------------------------------------------
                Clear all BD flags and status and set transfer state
            ----------------------------------------------------------------*/
            bdResetDiscFlags();
            HAL_BD_CODEC_CLEAR_DECODER_STATUS();

            /*---------------------------------------------------------------
                Start BD timer and enable BD read interrupts
            ----------------------------------------------------------------*/
            if (( bdDecFlag.fREADING_PIC ) && ((disc != eBDROMDisc) && (disc != eBDROMDLDisc)))
            {
                START_BD_CLUSTER_TIMER(1*FIRST_CLUSTER_TIMEOUT_VALUE);
            }
            else
            {
                START_BD_CLUSTER_TIMER(FIRST_CLUSTER_TIMEOUT_VALUE);
            }

            HAL_BD_CODEC_ENABLE_DECODER_INTS();

#if (SUN_FUNC_TEST == 1)
            if (fBlerRead == TRUE)/* Need to enable this or interrupt will not be triggled */
            {
                WRITE_FIELD(HAL_BDC_ERSTE,0x01);
            }
#endif

            /*---------------------------------------------------------------
                Call Seek
            ----------------------------------------------------------------*/
            #if (BD_AWG_TEST == TRUE)
                // DVD Test without Servo and disc using AWG pattern
                bdErrorFlag.fSEEK_FAIL = FALSE;
                DiscReadStatus       = BUF_GOOD;
                bdDiscRequest.State = TRANSFERRING;

                HAL_BD_CODEC_Enable_Buffering();
            #else

            ReadyLED(LED_ON);

#if (XDFCT_ENABLE == 1)
            if( (bdSeek((StartPSN - 2 * 0x20), bdDiscRequest.TargetLayer)) == Error)
#else
            if( (bdSeek(StartPSN - ((MAX_BD_READ_RETRIES - bdReadRetryCount) * 0x20), bdDiscRequest.TargetLayer)) == Error)
#endif
            {
            #if (E_DIAGS == 1)
                /*---------------------------------------------------------------
                    Seek fail. Adjust speed/error count
                ----------------------------------------------------------------*/
                if(READ_LDC_BIS == 0) /*Only speeddown when not ERRCO reading */
            #endif
                SpeedCount = SPEED_DOWN_FOR_SEEK_FAIL;
                bdErrorFlag.fSEEK_FAIL = TRUE;
                DiscReadStatus = SEEK_FAIL;
                bdDecFlag.fDISABLE_READ_AHEAD = TRUE;
            }
            else
            {
                /*---------------------------------------------------------------
                    Seek successful
                ----------------------------------------------------------------*/
                //send_msg80(SEND_SStpTask_MSG,0x710094);

                HAL_BD_CODEC_ENABLE_TARGET_MISS_INT();
                DiscReadStatus       = BUF_GOOD;
                bdDiscRequest.State = TRANSFERRING;

                HAL_BD_CODEC_Enable_Buffering();

                if (( bdDecFlag.fREADING_PIC ) && ((disc != eBDROMDisc) && (disc != eBDROMDLDisc)))
                {
                    START_BD_CLUSTER_TIMER(3*BD_CLUSTER_TIMEOUT_VALUE);
                }
                else
                {
                    START_BD_CLUSTER_TIMER(BD_CLUSTER_TIMEOUT_VALUE);
                }
            }
            #endif //BD_AWG_TEST
        } /* if (bdDecFlag.fEXTEND_READ) */
        else
            DiscReadStatus = BUF_GOOD;  // For extend always set disc status to good

        /*---------------------------------------------------------------
            Extend read flag no longer needed. Clear the flag to allow
            error procesing of the extended read process
        ----------------------------------------------------------------*/
        bdDecFlag.fEXTEND_READ = CLEAR;

        /*---------------------------------------------------------------
            For pre-fetching there is no need to monitor the buffering
        ----------------------------------------------------------------*/
        if (bdDecFlag.fREAD_AHEAD == TRUE)
            return (DiscReadStatus);


        /*-------------------------------------------------------------------
            Monitor BD Buffering
        --------------------------------------------------------------------*/
        StartTimer(&ErrorTimer);
        while(1)
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

            /*---------------------------------------------------------------
                Check for Servo Skating
            ----------------------------------------------------------------*/
#if (HIF_DETECT_SKATING== 1)
            if((svoIramVar.focusDropFlag == FALSE)&&(bdDecFlag.fREADING_PIC == CLEAR))// if focus drop, there is no ID.
            {
                {//get current PSN
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

                    //tmpPSN = BdGetCurrentPSN(&BdDecoder_ID);

                    //if(tmpPSN!=0xFFFFFFFF)
                    if(BdDecoder_ID)
                    {
                        ULONG tempPSN;
                        tempPSN = (bdLastDecodedPSN + 1L+ONE_BD_CLUSTER+2*ONE_BD_CLUSTER) - CurrentPSN;//decode end address+0x20 - current addrss.
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
                Check for buffered cluster errors posted by BD ISR
                Check for buffering errors (non cluster errors)
            ----------------------------------------------------------------*/
            if (BD_CLUSTER_ERROR ||BD_BUFFER_ERROR)
            {
                if(BD_BUFFER_ERROR)
                    send_msg5SValue(SEND_READ_DVD_MSG, "Buf Err", 4, bdErrorFlag.fSEEK_FAIL <<16 | bdErrorFlag.fTID_MISS <<8 | bdClusterInterruptTimer.fTIMEOUT);

                // Host request block tansfer done !! Drive should return Buf GOOD immediate, and stop prebuffer.
                if((HostXfer.State == TRANSFER_COMPLETE)&&((Iram_Cmd_block.bf[0] == SBC_READ_10)||(Iram_Cmd_block.bf[0] == SBC_READ_12)))//20091218_u.34==only ATAPI READ10 or READ12 command return buffer good under decoder error and host request data transfer complete.
                {
#if (BypassDecoderEr == 1)
                    IGNORE_DECODER_ERROR = FALSE;
#endif
                    bdStopDiscTransfer();                   /* stop disc transferring       */
                    bdResetDiscFlags();
                    HAL_BD_CODEC_CLEAR_DECODER_STATUS();    /* clear all remaining status   */
                    Svr_pause_reqire = TRUE;                /* Set pause request to servo   */
                    Iram_Svr_player_mode = PAUSING_MODE;    /* Set pause mode for player    */
                    return BUF_GOOD;
                }

                StartTimer(&DelayTimer);
                SDHoldSts = (vBOOL)READ_FIELD(HAL_BDC_HLDSDS);
                FDHoldSts = (vBOOL)READ_FIELD(HAL_BDC_HLDFDS);
                while (SDHoldSts == 0 || FDHoldSts == 0)
                {
                    HAL_BD_CODEC_Disable_Buffering();

                    if (ReadTimer(&DelayTimer) >= 200)
                    {
                        send_msg5S(SEND_READ_DVD_MSG, "SDFD HLDS Err 2");
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
                If prefetching starts while buffering exit and send status
            ----------------------------------------------------------------*/
            if (bdDecFlag.fREAD_AHEAD == TRUE)
                return(DiscReadStatus);

            /*---------------------------------------------------------------
                Check if all requested blocks have been buffered
            ----------------------------------------------------------------*/
            if (bdDecIntFlag.fECC_ALL_DONE_INT == SET)
                break;

            #if (BD_AWG_TEST == FALSE)
            if(EjectKeyInfo.PressedValid || TrayManagerInfo.Executing)
                return( EJECT_PRESS );
            #endif

            #if (E_DIAGS == 1)
            /*---------------------------------------------------------------
                Conditional code. If ERRORCORRECTION information has been obtained
                for a single Cluster return good status
            ----------------------------------------------------------------*/

            if (READ_LDC_BIS == 1)
            {
                if(READ_LDC_BIS == 1)
                    return( BUF_GOOD );
            }
            #endif

            if (BD_BUFFERING() == OFF)
            {
                if (ReadTimer(&ErrorTimer) >= 1000)
                    break;
            }

            ExitProcess();
        } /* End Monitor Buffering Loop */
    } /* END dvdDiscRequest.State != TRANSFER_COMPLETE */

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
/*******************************************************************************

    FUNCTION        GetBdError

    DESCRIPTION     Returns the type of error encountered during buffering

    RETURNS         Status of the disc transfer error during buffering

********************************************************************************/
DiscAccessStatusE GetBdError(void)
{

    if (bdErrorFlag.fUNCOR_LDC_ECC)
    {
        bdErrorFlag.fUNCOR_LDC_ECC = CLEAR;
        return(UNCOR_ECC_FAIL);
    }

    if (bdErrorFlag.fUNCOR_BIS_ECC)
    {
        bdErrorFlag.fUNCOR_BIS_ECC = CLEAR;
        return(UNCOR_ECC_FAIL);
    }

    if (bdErrorFlag.fEDC_ERROR)
    {
        bdErrorFlag.fEDC_ERROR = CLEAR;
        return(EDC_FAIL);
    }

    if (bdErrorFlag.fID_ERROR)
    {
        bdErrorFlag.fID_ERROR = CLEAR;
        return(ID_FAIL);
    }

    if (bdErrorFlag.fSEQ_ID_ERROR)
    {
        bdErrorFlag.fSEQ_ID_ERROR = CLEAR;
        return(SEQID_FAIL);
    }

    if (bdErrorFlag.fBCA_ERROR)
    {
        bdErrorFlag.fBCA_ERROR = CLEAR;
        return(BCA_FAIL);
    }

    if (bdErrorFlag.fSEEK_FAIL)
    {
        bdErrorFlag.fSEEK_FAIL = CLEAR;
        return(SEEK_FAIL);
    }

    if (bdErrorFlag.fTID_MISS)
    {
        /*don't clear in this moment, servo will check this flag to do retry function,
          clear the error flag in bdResetDiscFlags() in read flow*/
       // bdErrorFlag.fTID_MISS = CLEAR;
        return(TID_FAIL);
    }

    if (bdErrorFlag.fUNAUTH_READ_ERROR)
    {
        bdErrorFlag.fUNAUTH_READ_ERROR = CLEAR;
        return(UNAUTH_RD_FAIL);
    }

    if (bdErrorFlag.fID_MISMATCH)
    {
        bdErrorFlag.fID_MISMATCH = CLEAR;
        return(ID_FAIL);
    }

    if (bdClusterInterruptTimer.fTIMEOUT)
    {
         /*don't clear in this moment, servo will check this flag to do retry function,
          clear the error flag in bdResetDiscFlags() in read flow*/
        //bdClusterInterruptTimer.fTIMEOUT = CLEAR;
        return(TIMEOUT);
    }
    /*---------------------------------------------------------------
        General Fail Error - should never get here
    ----------------------------------------------------------------*/
    return(GEN_FAIL);

}

/*******************************************************************************

    FUNCTION        bdExtendBuffering()

    DESCRIPTION     Extends buffering by given amount.

    RETURNS         TRUE            BlockCount was added to buffering count
                    FALSE           Could not extend buffering

********************************************************************************/
BYTE bdExtendBuffering(void)
{
    ULONG TmpAddr;

    if ((bdDiscRequest.ClusterCount - bdDiscRequest.ClustersBuffered) <= BD_MIN_BKTOBUF_FOR_EXTEND)
        return FALSE;

    bdDiscRequest.ClusterCount += bdDiscRequest.ExtendClusters;
    TmpAddr = (bdDiscRequest.TargetID.Long >> 5) + bdDiscRequest.ClusterCount;

    HAL_BD_CODEC_SetStopAddress(TmpAddr);

    bdDecFlag.fEXTEND_READ = TRUE;

    return TRUE;
}

/*******************************************************************************

    FUNCTION        bdInitFlags

    DESCRIPTION     Clears all bd error, read, and interrupt flags

********************************************************************************/
void bdInitFlags(void)
{

    bdResetErrorFlags();

    /*---------------------------------------------------------------
        Clear DVD decoder flags
    ----------------------------------------------------------------*/
    bdDecFlag.fFIRST_HOST_CLUSTER       = CLEAR;
    bdDecFlag.fREADING_PIC              = CLEAR;
    bdDecFlag.fEXTEND_READ              = CLEAR;
    bdDecFlag.fREAD_AHEAD               = CLEAR;
//    bdDecFlag.fIGNORE_ECC_ERRORS        = CLEAR;
    bdDecFlag.fDISABLE_READ_AHEAD       = SET;

    /*---------------------------------------------------------------
        Clear BD decoder interrupt flags
    ----------------------------------------------------------------*/
    bdDecIntFlag.fBCA_DONE_INT          = CLEAR;
    bdDecIntFlag.fECC_CLUSTER_INT       = CLEAR;
    bdDecIntFlag.fECC_ALL_DONE_INT      = CLEAR;
    bdDecIntFlag.fSTP_ADDR_FOUND_INT    = CLEAR;
}

/*******************************************************************************

    FUNCTION        bdResetErrorFlags

    DESCRIPTION     Clears all bd error flags

********************************************************************************/
void bdResetErrorFlags()
{
    /*---------------------------------------------------------------
        Clear DVD error flags
    ----------------------------------------------------------------*/
    bdErrorFlag.fSEQ_ID_ERROR       = CLEAR;
    bdErrorFlag.fUNCOR_LDC_ECC      = CLEAR;
    bdErrorFlag.fUNCOR_BIS_ECC      = CLEAR;
    bdErrorFlag.fBCA_ERROR          = CLEAR;
    bdErrorFlag.fEDC_ERROR          = CLEAR;
    bdErrorFlag.fID_ERROR           = CLEAR;
    bdErrorFlag.fSEEK_FAIL          = CLEAR;
    bdErrorFlag.fTID_MISS           = CLEAR;
    bdErrorFlag.fUNAUTH_READ_ERROR  = CLEAR;
    bdErrorFlag.fID_MISMATCH        = CLEAR;
}

/*******************************************************************************

    FUNCTION        GetUcdPointer

    DESCRIPTION     Return the UCD pointer, which points to the starting
                    absolute address of the UCD field in each BIS block.

********************************************************************************/
ULONG GetUcdPointer(WORD PageNo)
{
    return (ULONG)(SCSDRAM + BD_AUX_ADDRESS(PageNo) + BD_AUX_UCD_OFFSET);
}

#if (BypassDecoderEr == 1)
BYTE BDValidBypassBDDecoderEr()
{
    BYTE Status = FALSE;

    if ((bdLastDecodedPSN > 0x100000) && (pdbGetDiscKind() == eBDROMDisc || pdbGetDiscKind() == eBDROMDLDisc))
        Status = TRUE;

    return Status;
}
#endif

#if (XDFCT_ENABLE == 1)
/*******************************************************************************

    FUNCTION        bdStopSpecialDiscHandle

    DESCRIPTION     Stops SpecialDiscHandle

********************************************************************************/
void bdStopSpecialDiscHandle()
{
    LAL_DSP_XDFCT_Enable(lalDSP_Disable,0);
}

/*******************************************************************************

    FUNCTION        bdStartSpecialDiscHandle

    DESCRIPTION     Starts SpecialDiscHandle

********************************************************************************/
void bdStartSpecialDiscHandle(ULONG blk_start_psn, BYTE BdwtRetryCnt)
{
    send_msg5SValue(1,"BDEC",1, BdwtRetryCnt);//SV1
    if ((blk_start_psn <= 0x178000) && (blk_start_psn >= 0x151000))
    {
        //Turn on DSP code here.
        LAL_DSP_XDFCT_Enable(lalDSP_Disable,0);
        WRITE_DSP_COEF(xdfct_hold_wait,90); //1msec wait delay
        WRITE_DSP_COEF(xdfct_TE_hold_level,0x0000);//zero kick
        WRITE_DSP_COEF(xdfct_FE_hold_level,0x0000);//zero kick
        WRITE_DSP_COEF(xdfct_hold_end,0x14); //44usec kick pulse
        WRITE_DSP_COEF(xdfct_FE_hold_time,0); //44usec kick pulse
        WRITE_DSP_COEF(xdfct_TE_hold_time,0x14); //44usec kick pulse

        switch(BdwtRetryCnt % 8)
        {
            case 7:
                WRITE_DSP_COEF(xdfct_TE_hold_level,0xffe0);//small reverse kick
                LAL_DSP_XDFCT_Enable(lalDSP_Enable_TE,0x0028);
                break;
            case 6:
                WRITE_DSP_COEF(xdfct_TE_hold_level,0xffc0);//small reverse kick
                LAL_DSP_XDFCT_Enable(lalDSP_Enable_TE,0x0028);
                break;
            case 5:
                WRITE_DSP_COEF(xdfct_TE_hold_level,0x0000);//small reverse kick
                LAL_DSP_XDFCT_Enable(lalDSP_Enable_TE,0x0028);
                break;
            case 4:
                LAL_DSP_XDFCT_Enable(lalDSP_Disable,0);
                break;
            case 3:
                WRITE_DSP_COEF(xdfct_TE_hold_level,0xffd0);//small reverse kick
                LAL_DSP_XDFCT_Enable(lalDSP_Enable_FE,0x0028);
                break;
            case 2:
                WRITE_DSP_COEF(xdfct_TE_hold_level,0xffa0);//small reverse kick
                LAL_DSP_XDFCT_Enable(lalDSP_Enable_FE,0x0028);
                break;
            case 1:
                WRITE_DSP_COEF(xdfct_TE_hold_level,0xff70);//small reverse kick
                LAL_DSP_XDFCT_Enable(lalDSP_Enable_FE,0x0028);
                break;
            case 0:
                LAL_DSP_XDFCT_Enable(lalDSP_Disable,0);
                break;
            default:
                LAL_DSP_XDFCT_Enable(lalDSP_Disable,0);
                break;
        }

    }
}
#endif //(XDFCT_ENABLE == 1)

#endif // BD_READ
