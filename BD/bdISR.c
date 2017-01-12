/****************************************************************************
*                (c) Copyright 2000 - 2007 SUNEXT DESIGN, INC.
*                           All Right Reserved
*
*
*   FILE NAME:      $Workfile: bdISR.c $
*
*   DESCRIPTION:    Manages the BD services for BD Codec interrupts
*
*
*   $Revision: 70 $
*   $Date: 11/03/28 2:36p $
*
**************************** SUNEXT CONFIDENTIAL *******************************/

/*******************************************************************
    INCLUDE FILES
*******************************************************************/
#include ".\common\globals.h"
#include ".\common\globtype.h"
#include ".\AL\REG_HAL.h"
#include ".\AL\REG_global.h"
#include ".\AL\REG_bd_codec.h"
#include ".\Al\HAL_bd_codec.h"
#include ".\bd\bdIsr.h"
#include ".\bd\bdvars.h"
#include ".\bd\bdUtil.h"
#include ".\bd\bddiscrd.h"
#include ".\bd\bdMacro.h"
#include ".\bd\bdbufmgr.h"
#include ".\hif\gp_mac.h"
#include ".\AL\REG_demod.h"
#include ".\hif\diag.h"
#include ".\servo\svo_lib.h"

#if (BD_READ == 1)

#if E_DIAGS
#include ".\bd\bdDiag.h"

extern BYTE Send_timef;
#endif

#if (SUN_FUNC_TEST == 1)
#include ".\oem\sun710_function_test.h"
#endif

#if (ENABLE_AACS == 1)
#include ".\crypt\aacs.h"

extern BOOL fBypassAuth;
extern AacsS AacsStatus;
#endif

BYTE gFirstClusterDone;

#if (ENABLE_DFE_ICE_DETECT == 1)
extern ULONG IcedClusterStAddr;
#if (ENABLE_DISCARD_ICE == 1 && BD_ENABLE == 1)
extern vBOOL SDHoldSts;
extern vBOOL FDHoldSts;
extern BYTE gFirstClusterDone;
#endif
#endif

/*******************************************************************
    PROTOTYPES
*******************************************************************/
void bdProcessCluster(void);
void bdCheckForHostTransfer(void);

#if (BD_VERIFY_ID == 1)
BYTE bdVerifyID();
#endif

/*******************************************************************************

    FUNCTION        BdDecoderIrqHandler

    DESCRIPTION     Function is executed when a Decoder Interrupt has has taken
                    place whose origin is in the BD Decoder Module.

********************************************************************************/
void BdDecoderIrqHandler(void)
{
    ULONG bdIrqStatus1;
    ULONG bdIrqStatus2;
#if (ENABLE_DFE_ICE_DETECT == 1)
    BYTE IceDetected, *IceIndicator;
    BYTE i;
#endif

#if E_DIAGS
    ULONG bdErrStatus;
#endif
    BYTE bdError;

    /* Get all BD Codec interrupts status and clear all interrupts */
    bdIrqStatus1 = HAL_BD_CODEC_GET_DECODER_INT1_STATUS();
    bdIrqStatus2 = HAL_BD_CODEC_GET_DECODER_INT2_STATUS();

#if E_DIAGS
    if (bdIrqStatus1 & HAL_BDC_ERSTI)
    {
        bdErrStatus = READ_REG( HAL_BDCREG_MAP->BDESINFO );
        //send_msg5SValue(SEND_ATAPI_CMD, "EStUpd", 4, bdErrStatus);
        if((bdErrStatus & HAL_BDC_STATUPDAT) && READ_LDC_BIS == 1)
        {
            if(WAIT_LDC_BIS == 0)
            {
                WAIT_LDC_BIS = 1;
                LDC_ERRA = READ_FIELD(HAL_BDC_LDCERRATA );
                LDC_ERAS = READ_FIELD(HAL_BDC_LDCERASUR );
                BIS_C = READ_FIELD(HAL_BDC_BISCOR);
                BIS_UN_C = READ_FIELD(HAL_BDC_BISUNC);
                LDC_C = READ_FIELD(HAL_BDC_LDCCOR );
                LDC_UN_C = READ_FIELD(HAL_BDC_LDCUNC );
                MX_DFT_LEN = READ_FIELD(HAL_BDC_MXDFTLEN);
                LDC_MAX_ERR = READ_FIELD(HAL_BDC_LDCMAXERR);
                LDC_AUN_ERRATA = READ_FIELD(HAL_BDC_AUNERRATA);
                ERCO_CURRENT_ID.Long = READ_FIELD(HAL_BDC_STATSTRAD) <<5 ;

#if (ENABLE_DFE_ICE_DETECT == 1)
                if (bdDecFlag.fREADING_LEADIN == FALSE && pdbGetDiscKind() != eBDROMDisc && pdbGetDiscKind() != eBDROMDLDisc)
                {
                    if ((IcedClusterStAddr << 3) == ERCO_CURRENT_ID.Long)
                    {
#if (HF_BASED_CUTOFF == 1)
                        WRITE_FIELD(HAL_DMOD_RELINKCUT, 0);  //relinking cut off base on wobble
                        WRITE_FIELD(HAL_DMOD_DISDMDFRC, 0);  //force demod in run mode in runin area
#endif
                        LDC_ERRA = 0;
                        LDC_ERAS = 0;
                        BIS_C = 0;
                        BIS_UN_C = 0;
                        LDC_C = 0;
                        LDC_UN_C = 0;
                        MX_DFT_LEN = 0;
                        LDC_MAX_ERR = 0;
                        LDC_AUN_ERRATA = 0;
                    }
                    else if((IcedClusterStAddr << 3) > ERCO_CURRENT_ID.Long)
                    {
                        WAIT_LDC_BIS = 0;
    #if (ENABLE_DISCARD_ICE == 1)
                        HAL_BD_CODEC_Enable_DiscardIce();
    #endif
                    }
                    else
                    {
#if (HF_BASED_CUTOFF == 1)
                       if(READ_FIELD(HAL_SRV_DFCTOFF) == 1)
                       {
                           Defect_Enable(DEFECT_STATE_ALL_ON);
                       }
                       if((READ_FIELD(HAL_DFE_BITDETCFG) != 3) && (Current_read_sp >= e6XDCAV))
                       {
                	       WRITE_FIELD(HAL_DFE_BITDETCFG, 3);
                	   }
    #if(WOB_BASED_DEMODULATOR == 1)
            if(wob_based_demod==1)
            {
                WRITE_FIELD(HAL_DMOD_RELINKCUT, 0);  //relinking cut off base on demodulator
                WRITE_FIELD(HAL_DMOD_DISDMDFRC, 0);  //don't force in run mode in runin area
            }
            else
            {
                WRITE_FIELD(HAL_DMOD_RELINKCUT, 1);  //relinking cut off base on demodulator
                WRITE_FIELD(HAL_DMOD_DISDMDFRC, 1);  //don't force in run mode in runin area
            }
    #else
            WRITE_FIELD(HAL_DMOD_RELINKCUT, 1);  //relinking cut off base on demodulator
            WRITE_FIELD(HAL_DMOD_DISDMDFRC, 1);  //don't force in run mode in runin area
    #endif

    #if (ENABLE_DISCARD_ICE == 1)
                       HAL_BD_CODEC_Disable_DiscardIce();
    #endif
#endif
                    }
                }
#endif  //ENABLE_DFE_ICE_DETECT
            }
        }

#if (SUN_FUNC_TEST == 1)    
        if (fStartBuffer == TRUE)
        {   
            usBlerCount++;
            
            LDC_UN_C = READ_FIELD(HAL_BDC_LDCUNC);//PO_UNC
            LDC_ERAS = READ_FIELD(HAL_BDC_LDCERASUR);//PI_UNC            
            usSUM_PI_C1 += LDC_ERAS;
            usSUM_PO_C2 += LDC_UN_C;
            usSUM_PER_UNIT_PI_C1 += LDC_ERAS;
            if (usBlerCount == usBLER_UNIT)
            {
                if (usSUM_PER_UNIT_PI_C1 > usMAX_PI_C1)
                {
                    usMAX_PI_C1 = usSUM_PER_UNIT_PI_C1;//save the max PI
                }
                usUnitCount++;
                usBlerCount = 0; 
                usSUM_PER_UNIT_PI_C1 = 0;
            }                
        }
#endif          
    }
#endif  //E_DIAGS

    HAL_BD_CODEC_CLEAR_DECODER_STATUS();

    /*---------------------------------------------------------------
        Clear error flag
    ---------------------------------------------------------------*/
    bdError = FALSE;

    if (bdIrqStatus1 & HAL_BDC_FDCLUSDNI)
    {
        bdDecIntFlag.fECC_CLUSTER_INT = SET;

        if(bdDecFlag.fREADING_PIC && ( (pdbGetDiscKind() != eBDROMDisc) && (pdbGetDiscKind() != eBDROMDLDisc) ))
            DataBuffer.CurDiscPtr += 2;        /* Advance the disc pointer */
        else
            DataBuffer.CurDiscPtr += ONE_BD_CLUSTER;        /* Advance the disc pointer */
        DataBuffer.CurDiscPtr %= BUF_TOTAL_BLK_COUNT;   /* Insure buffer wrap */

        if (bdIrqStatus1 & HAL_BDC_AUNFLRESI)
        {
// TBD? No Code!
        }

        /*---------------------------------------------------------------
            Check for full deinterleave EDC check fail
        ---------------------------------------------------------------*/
        if ((bdIrqStatus1 & HAL_BDC_FDEDCFAII) || BD_CUR_EDC_STATUS())
        {
            bdError = TRUE;
            bdErrorFlag.fEDC_ERROR = SET;
            send_msg80(SEND_ATAPI_CMD,0x7100DD);
        }

        /*---------------------------------------------------------------
            Check for full deinterleave LDC uncorrectable (Cluster) error
        ---------------------------------------------------------------*/
        if (bdIrqStatus1 & HAL_BDC_FDLDCUNCI)
        {
            bdError = TRUE;
            bdErrorFlag.fUNCOR_LDC_ECC = SET;
            send_msg80(SEND_ATAPI_CMD,0x7100DE);
        }

        /*---------------------------------------------------------------
            Check for full deinterleave BIS uncorrectable error
        ---------------------------------------------------------------*/
        if (bdIrqStatus1 & HAL_BDC_FDBISUNCI)
        {
            bdError = TRUE;
            bdErrorFlag.fUNCOR_BIS_ECC = SET;
            send_msg80(SEND_READ_DVD_MSG,0x7100DF);
        }

        if (bdIrqStatus1 & HAL_BDC_CONTERRI)
        {
            // A continuity error is detected by small deinterleaver
//Not in interrutps            send_msg54(SEND_READ_DVD_MSG, 'S', 'q', 'I', 'D');
            bdError = TRUE;
            bdErrorFlag.fSEQ_ID_ERROR = SET;
        }

#if (BypassDecoderEr == 1)
    if (IGNORE_DECODER_ERROR == TRUE)
    {
        send_msg5S(SEND_READ_DVD_MSG, "IGNORE_DE_ER");
        bdErrorFlag.fEDC_ERROR = CLEAR;          /* Set flag         */
        bdErrorFlag.fUNCOR_LDC_ECC = CLEAR;          /* Set flag         */
        bdErrorFlag.fUNCOR_BIS_ECC = CLEAR;           /* Set flag         */
        bdErrorFlag.fSEQ_ID_ERROR = CLEAR;            /*  Set flag        */
        bdError = FALSE;                        /* Mark error       */
        IGNORE_DECODER_ERROR = FALSE;
    }
#endif

#if (E_DIAGS == 1)
        if (bdIrqStatus1 & HAL_BDC_ERAOVFLI)
        {//Erasure Overflow Interrupt (##PI_U)
// TBD? No Code!
            send_msg80(SEND_ATAPI_CMD,0x7100E0);
        }

        if (bdIrqStatus1 & HAL_BDC_AEDCFAI)
        {//Alert EDC Failure Interrupt
// TBD? No Code!
            send_msg80(SEND_ATAPI_CMD,0x7100E1);
        }

        if (bdIrqStatus1 & HAL_BDC_LWREDCFAI)
        {//Lower EDC Failure Interrupt
// TBD? No Code!
            send_msg80(SEND_ATAPI_CMD,0x7100E2);
        }

        if (bdIrqStatus1 & HAL_BDC_ALDCUNCI)
        {//Alert LDC Uncorrectable Interrupt (## PO_U)
// TBD? No Code!
            send_msg80(SEND_ATAPI_CMD,0x7100E3);
        }

        if (bdIrqStatus1 & HAL_BDC_LWRLDCUNI)
        {//Lower LDC Uncorrectable Interrupt (## PO_U)
// TBD? No Code!
            send_msg80(SEND_ATAPI_CMD,0x7100E4);
        }

        if (bdIrqStatus1 & HAL_BDC_ASERI)
        {//Alert SER Interrupt (for errata ## PI_C)
// TBD? No Code!
            send_msg80(SEND_ATAPI_CMD,0x7100E5);
        }

        if (bdIrqStatus1 & HAL_BDC_LWRSERI)
        {//Lower SER Interrupt (for errata ## PI_C)
// TBD? No Code!
            send_msg80(SEND_ATAPI_CMD,0x7100E6);
        }
#endif  /* E_DIAGS == 1 */
    }


    if (bdIrqStatus2 & HAL_BDC_BCADONE)
    {
        bdDecIntFlag.fBCA_DONE_INT = SET;
        send_msg80(SEND_READ_DVD_MSG,0x7100E7);
    }

    if (bdIrqStatus2 & HAL_BDC_BCAUN)
    {
        send_msg80(SEND_READ_DVD_MSG,0x7100E8);
        bdError = TRUE;
        bdErrorFlag.fBCA_ERROR = SET;
    }

    if (bdIrqStatus2 & HAL_BDC_FDSTPADFI)
    {
        bdDecIntFlag.fSTP_ADDR_FOUND_INT = SET;
        send_msg80(SEND_READ_DVD_MSG,0x7100E9);
    }

    if (bdIrqStatus2 & HAL_BDC_FDSTRADFI)
    {
        send_msg80(SEND_READ_DVD_MSG,0x7100EA);
    }

    if (bdIrqStatus2 & HAL_BDC_FDSTRADMI)
    {
        // Target Miss
        bdError = TRUE;
        bdErrorFlag.fTID_MISS = SET;
        send_msg80(SEND_ATAPI_CMD,0x7100EB);
    }


    if ( bdDecIntFlag.fECC_CLUSTER_INT )
    {
        bdDecIntFlag.fECC_CLUSTER_INT = CLEAR;

        #if (BD_VERIFY_ID == 1)
        if (bdVerifyID() == FALSE)
        {
            if (bdError == FALSE)
            {
                send_msg80(SEND_READ_DVD_MSG,0x7100EC);
                bdError = TRUE;
                bdErrorFlag.fID_MISMATCH = SET;
            }
        }
        #endif

#if (ENABLE_DFE_ICE_DETECT == 1)
        if (bdDecFlag.fREADING_LEADIN == FALSE && pdbGetDiscKind() != eBDROMDisc && pdbGetDiscKind() != eBDROMDLDisc)
        {
            IceDetected = FALSE;
            switch (BdwtDisc.ReadDataMode)
            {
#if (ENABLE_DISCARD_ICE == 1)
                case eREAD_DATA_FROM_DISC:
                    for(i=0;i<MAXICECNT;i++)
                    {
                        if ((ICEPSN[i]) == (bdLastDecodedPSN + 1))
                        {
                            IceDetected = TRUE;
                            break;
                        }
                    }
                    break;

                case ePRE_BUFFERING:
                    for(i=0;i<MAXICECNT;i++)
                    {
                        if ((ICEPSN[i]) == BdwtDisc.PrePSN)
                        {
                            IceDetected = TRUE;
                            break;
                        }
                    }
                    break;

#else
                case eREAD_DATA_FROM_DISC:
                    if ((IcedClusterStAddr << 3) == (bdLastDecodedPSN + 1))
                        IceDetected = TRUE;
                    break;

                case ePRE_BUFFERING:
                    if ((IcedClusterStAddr << 3) == BdwtDisc.PrePSN.PSN)
                        IceDetected = TRUE;
                    break;
#endif  //ENABLE_DISCARD_ICE
            }
#if (SUN_FUNC_TEST == 1)
            /* For function test, if the read location is ICED area and do not return it as the read fail */
            if ((BdwtDisc.ReadDataMode == eNOT_USERDATA)&&(FTInfo.DiableCDBufferFullStopDecoding == TRUE))
            {
                for(i=0;i<MAXICECNT;i++)
                {
                    if ((ICEPSN[i]) == (bdLastDecodedPSN + 1))
                    {
                        IceDetected = TRUE;
                        //send_msg5S(1,"ICE_Check");
                        break;
                    }
                }
            }
#endif

#if (HF_BASED_CUTOFF == 1)
            if(IceDetected == TRUE)
            {
               WRITE_FIELD(HAL_DMOD_RELINKCUT, 0);  //relinking cut off base on wobble
               WRITE_FIELD(HAL_DMOD_DISDMDFRC, 0);  //force demod in run mode in runin area
#if (ENABLE_DISCARD_ICE == 1)
                HAL_BD_CODEC_Enable_DiscardIce();
#endif
            }
            else
            {
               if(READ_FIELD(HAL_SRV_DFCTOFF) == 1)
               {
                   Defect_Enable(DEFECT_STATE_ALL_ON);
               }
               if((READ_FIELD(HAL_DFE_BITDETCFG) != 3) && (Current_read_sp >= e6XDCAV))
               {
                   WRITE_FIELD(HAL_DFE_BITDETCFG, 3);
               }

#if(WOB_BASED_DEMODULATOR == 1)
            if(wob_based_demod==1)
            {
                WRITE_FIELD(HAL_DMOD_RELINKCUT, 0);  //relinking cut off base on demodulator
                WRITE_FIELD(HAL_DMOD_DISDMDFRC, 0);  //don't force in run mode in runin area
            }
            else
            {
                WRITE_FIELD(HAL_DMOD_RELINKCUT, 1);  //relinking cut off base on demodulator
                WRITE_FIELD(HAL_DMOD_DISDMDFRC, 1);  //don't force in run mode in runin area
            }
#else
            WRITE_FIELD(HAL_DMOD_RELINKCUT, 1);  //relinking cut off base on demodulator
            WRITE_FIELD(HAL_DMOD_DISDMDFRC, 1);  //don't force in run mode in runin area
#endif

#if (ENABLE_DISCARD_ICE == 1)
               HAL_BD_CODEC_Disable_DiscardIce();
#endif
            }
#endif

            IcedClusterStAddr = 0L;

            if ( DataBuffer.CurDiscPtr == 0 )
            {
                IceIndicator =  (BYTE *)(BD_ICE_INDICATOR_ADDR + ((BUF_TOTAL_BLK_COUNT - ONE_BD_CLUSTER) >> 5));
            }
            else
                IceIndicator =  (BYTE *)(BD_ICE_INDICATOR_ADDR + ((DataBuffer.CurDiscPtr - ONE_BD_CLUSTER) >> 5));

            if (bdError == TRUE && IceDetected == TRUE)
            {
                IceDetected = FALSE;
                bdError = FALSE;
                bdErrorFlag.fEDC_ERROR = CLEAR;
                bdErrorFlag.fUNCOR_LDC_ECC = CLEAR;
                bdErrorFlag.fUNCOR_BIS_ECC = CLEAR;
                bdErrorFlag.fSEQ_ID_ERROR = CLEAR;
                bdErrorFlag.fID_MISMATCH = CLEAR;

                *IceIndicator = eICE_BLOCK;
            }
            else
                *IceIndicator = eNON_ICE_BLOCK;
        }
#endif  /* ENABLE_DFE_ICE_DETECT == 1 */

#if (ENABLE_AACS == 1 && BAPASS_AACS_AUTH == 0)
        if ( (fBypassAuth == FALSE) && (AacsStatus.AacsFlag.IsMediumCompliant == TRUE) && (AacsStatus.AacsFlag.AuthenticationSuccess == FALSE) )
        {
            if (bdDecFlag.fREADING_LEADIN != TRUE)
            {
                bdError = TRUE;
                bdErrorFlag.fUNAUTH_READ_ERROR = SET;
            }
        }
#endif

        if (bdError == FALSE)
        {
            if ( bdDecIntFlag.fSTP_ADDR_FOUND_INT )
            {
                bdDecIntFlag.fSTP_ADDR_FOUND_INT = CLEAR;
                bdDecIntFlag.fECC_ALL_DONE_INT = SET;
            }
            bdProcessCluster();
        }
        else
        {
            /*---------------------------------------------------------------
                Set up recovery for next read. This function sets up the
                read to be performed at a lower speed
            ---------------------------------------------------------------*/
            RecoverDecoderError();

            /*---------------------------------------------------------------
                Clear the flag fSTP_ADDR_FOUND_INT
            ----------------------------------------------------------------*/
            if ( bdDecIntFlag.fSTP_ADDR_FOUND_INT )
                bdDecIntFlag.fSTP_ADDR_FOUND_INT = CLEAR;

            if (bdDecFlag.fREAD_AHEAD == TRUE)
            {
                /*---------------------------------------------------------------
                    If read ahead clear read ahead flag and Set system for pause
                ----------------------------------------------------------------*/
                Svr_pause_reqire        = TRUE;
                Iram_Svr_player_mode    = PAUSING_MODE;
            }

            /*---------------------------------------------------------------
                Stop buffering process if there is an error and no host blocks
                are being buffered. Any error on a host block being buffered
                will be handled outside of this isr.
            ---------------------------------------------------------------*/
            /*---------------------------------------------------------------
                Stop buffering
            ----------------------------------------------------------------*/
            bdStopDiscTransfer();

            if ((pdbGetBDType() == BD_R) || (pdbGetBDType() == BD_RE))
            {
                if (READ_LDC_BIS == 0)
                    Svr_pause_reqire = TRUE;

                send_msg5S(SEND_MRW_MSG,"PrebufStop-1");
            }
            /*---------------------------------------------------------------
                Reset CurDiscPtr to previous location
            ----------------------------------------------------------------*/
            if(bdDecFlag.fREADING_PIC && ( (pdbGetDiscKind() != eBDROMDisc) && (pdbGetDiscKind() != eBDROMDLDisc) ))
            {
                if ( DataBuffer.CurDiscPtr == 0 )
                    DataBuffer.CurDiscPtr =  (BUF_TOTAL_BLK_COUNT - 2);
                else
                    DataBuffer.CurDiscPtr -= 2;
            }
            else
            {
                if ( DataBuffer.CurDiscPtr == 0 )
                    DataBuffer.CurDiscPtr =  (BUF_TOTAL_BLK_COUNT - ONE_BD_CLUSTER);
                else
                    DataBuffer.CurDiscPtr -= ONE_BD_CLUSTER;
            }
        }
        return;
    }

    if ( bdErrorFlag.fTID_MISS )
    {
        if (bdDecFlag.fREAD_AHEAD == TRUE)
        {
            /*---------------------------------------------------------------
                If read ahead set system for pause
            ----------------------------------------------------------------*/
            Svr_pause_reqire        = TRUE;
            Iram_Svr_player_mode    = PAUSING_MODE;
        }

        /*---------------------------------------------------------------
            Stop buffering
        ----------------------------------------------------------------*/
        bdStopDiscTransfer();

        return;
    }
}

/*******************************************************************************

    FUNCTION        bdProcessCluster

    DESCRIPTION     Processes a valid cluster

********************************************************************************/
void bdProcessCluster(void)
{
    /*---------------------------------------------------------------
        Reset BD Block Timer
    ---------------------------------------------------------------*/
    if (( bdDecFlag.fREADING_PIC ) && ((pdbGetDiscKind() != eBDROMDisc) && (pdbGetDiscKind() != eBDROMDLDisc)))
    {
        START_BD_CLUSTER_TIMER(40*BD_CLUSTER_TIMEOUT_VALUE);
    }
    else
    {
        START_BD_CLUSTER_TIMER(BD_CLUSTER_TIMEOUT_VALUE);
    }

    /*---------------------------------------------------------------
        Update count of clusters transferred, cluster count and
        last PSN in buffer.
    ---------------------------------------------------------------*/
    ++bdDiscRequest.ClustersBuffered;
    if (bdDecFlag.fREADING_PIC == FALSE || pdbGetDiscKind() == eBDROMDisc || pdbGetDiscKind() == eBDROMDLDisc)
    {
        DataBuffer.SectorCount  += ONE_BD_CLUSTER;
        bdLastDecodedPSN        += ONE_BD_CLUSTER;
    }
    else
    {
        DataBuffer.SectorCount  += 2;
        bdLastDecodedPSN        += 2;
    }

#if (XDFCT_ENABLE == 1)
    if (!(bdLastDecodedPSN >= 0x151000 && bdLastDecodedPSN <= 0x178000))
    {
        bdStopSpecialDiscHandle();
    }
#endif

    /*--------------------------------------------------------------
        Adjust the SpeedCount and SpeedUpReady.
    ---------------------------------------------------------------*/
    if(SpeedCount < SPEED_UP_COUNT_LIMIT - ONE_BD_CLUSTER)
    {
        if( SpeedCount >= 0 )
            SpeedCount += ONE_BD_CLUSTER;
        else
            SpeedCount = ONE_BD_CLUSTER;
    }

    if(SpeedUpReady > BD_SPEED_UP_READY)
        SpeedUpReady--;

    if ( pdbGetDiscKind() == eBDRDisc || pdbGetDiscKind() == eBDRDLDisc ||
         pdbGetDiscKind() == eBDREDisc || pdbGetDiscKind() == eBDREDLDisc )
    {
        if (BdwtDisc.ReadDataMode == ePRE_BUFFERING)
        {
            if (bdDecFlag.fFIRST_HOST_CLUSTER == TRUE)
            {
                bdDecFlag.fFIRST_HOST_CLUSTER = FALSE;
                gFirstClusterDone = TRUE;
            }

            BDAppendPreBuffer();
        }

        /*---------------------------------------------------------------
        Update last lba in the cache
        ---------------------------------------------------------------*/
        if (bdDecFlag.fREADING_PIC)
        {
            if (DataBuffer.CacheEndLBA == 0L)
                DataBuffer.CacheEndLBA = 1L;
            else
                DataBuffer.CacheEndLBA += 2;
        }
        else
        {
            if (DataBuffer.CacheEndLBA == 0L)
                DataBuffer.CacheEndLBA = 0x1FL;
            else
            {
                DataBuffer.CacheEndLBA += ONE_BD_CLUSTER;
            }
        }
#if(SUN_FUNC_TEST == 1)
        if(FTInfo.DiableCDBufferFullStopDecoding == FALSE)
#endif                            
        if (( bdDecIntFlag.fECC_ALL_DONE_INT ) || (DataBuffer.SectorCount >= (eBDTotalPageNo -0x60)))
        {
            /*---------------------------------------------------------------
                Disable BD interrupts and clear interrupts and enable flags
            ---------------------------------------------------------------*/
            HAL_BD_CODEC_DISABLE_DECODER_INTS();

            HAL_BD_CODEC_Disable_Buffering();

            STOP_BD_CLUSTER_TIMER();

            Svr_pause_reqire = TRUE;
            send_msg5S(SEND_MRW_MSG,"PrebufStop-2");
        }

        if (BdwtDisc.ReadDataMode == eNOT_USERDATA)
        {
            if(bdDiscRequest.ClustersBuffered == bdDiscRequest.ClusterCount)
            {
                bdDiscRequest.State    = TRANSFER_COMPLETE;     /* Transfer is complete     */

                Svr_pause_reqire        = TRUE;                 /* set system for pause     */
                Iram_Svr_player_mode    = PAUSING_MODE;
            }
        }

        return;
    }


    /*---------------------------------------------------------------
        Check if all BD clusters to satisfy the host request have
        been buffered. If so, indicate that pre-fetching has started
    ----------------------------------------------------------------*/
#if (BD_READ_AHEAD_ENABLED)
    if (bdReadAheadStartPSN == (bdLastDecodedPSN + 1))
    {
        bdDecFlag.fREAD_AHEAD = TRUE;
    }
#endif  /* BD_READ_AHEAD_ENABLED */


    /*---------------------------------------------------------------
        Check if all blocks programmed have been transferred
        and corrected.
    ----------------------------------------------------------------*/
    if( bdDecIntFlag.fECC_ALL_DONE_INT )
    {
        /*---------------------------------------------------------------
            Disable BD interrupts and clear interrupts and enable flags
        ---------------------------------------------------------------*/
        HAL_BD_CODEC_DISABLE_DECODER_INTS();

        HAL_BD_CODEC_Disable_Buffering();

        STOP_BD_CLUSTER_TIMER();

        /*---------------------------------------------------------------
            If the transfer request is not complete set state to WAIT
        ----------------------------------------------------------------*/
        if( bdDiscRequest.ClustersBuffered < bdDiscRequest.ClusterCount)
            bdDiscRequest.State = WAITING_FOR_TRANSFER;

        /*---------------------------------------------------------------
            If buffering completed during pre-fetching indicate read
            ahead is now off
        ----------------------------------------------------------------*/
#if (BD_READ_AHEAD_ENABLED)
        bdDecFlag.fREAD_AHEAD = FALSE;
#endif
    }

    /*---------------------------------------------------------------
        Check if all requested blocks have been transferred
    ----------------------------------------------------------------*/
    if( bdDiscRequest.ClustersBuffered == bdDiscRequest.ClusterCount)
    {
        bdDiscRequest.State    = TRANSFER_COMPLETE;    /* Transfer is complete     */

        Svr_pause_reqire        = TRUE;                 /* set system for pause     */
        Iram_Svr_player_mode    = PAUSING_MODE;
    }

    /*---------------------------------------------------------------
        Reset retry count
    ----------------------------------------------------------------*/
    if (bdDecFlag.fREAD_AHEAD == FALSE)
    {
         bdReadRetryCount = MAX_BD_READ_RETRIES;
    }

    /*---------------------------------------------------------------
        Check to see if a host transfer is needed. If this is a
        read lead in block there is no need for a host transfer
    ----------------------------------------------------------------*/
    if (bdDecFlag.fREADING_LEADIN == FALSE && bdDecFlag.fREADING_PIC == FALSE)
        bdCheckForHostTransfer();
}

/*******************************************************************************

    FUNCTION        bdCheckForHostTransfer

    DESCRIPTION     Updates the host side of the buffer and determines if a
                    host transfer should be initiated

********************************************************************************/
void bdCheckForHostTransfer()
{
    ULONG OffsetAdjustment; /* Adjustment of host pointer into first block */

    /*---------------------------------------------------------------
        Update last lba in the cache
    ---------------------------------------------------------------*/
    if (DataBuffer.CacheEndLBA == 0L)
        DataBuffer.CacheEndLBA = 0x1FL;
    else
    {
        DataBuffer.CacheEndLBA += ONE_BD_CLUSTER;
    }
    /*---------------------------------------------------------------
        Exit if host is not waiting for a transfer to be initiated.
    ---------------------------------------------------------------*/
    if( HostXfer.State != WAITING_FOR_TRANSFER)
        return;

    /*---------------------------------------------------------------
        Intitiate a host transfer
    ---------------------------------------------------------------*/

    /*-------------------------------------------------------------------
        If this is the first cluster loaded (on cache miss only):

            1. Clear first cluster flag
            2. Reset the error count for speed error recovery
            3. Adjust the current host pointer to the starting LBA
               within this first block and adjust SectorCount.

    --------------------------------------------------------------------*/
    if ( bdDecFlag.fFIRST_HOST_CLUSTER )
    {
        bdDecFlag.fFIRST_HOST_CLUSTER = FALSE;
        OffsetAdjustment = DataBuffer.CacheStartLBA - (DataBuffer.CacheStartLBA & BD_CLUSTER_MASK);
        DataBuffer.CurHostPtr += OffsetAdjustment;
        DataBuffer.SectorCount -= OffsetAdjustment;
    }

    SendHostBlocks();
}

#if (BD_VERIFY_ID == 1)
/*******************************************************************************

    FUNCTION        bdVerifyID

    DESCRIPTION     Verifies the actual ID with target

********************************************************************************/
BYTE bdVerifyID()
{
    ULONG TargetID;
    ULONG ActualID;
    ULONG *AuxPtr;

    if (BdwtDisc.CpuUsage8X)
        return TRUE;

    if (pdbGetDiscKind() == eBDROMDisc || pdbGetDiscKind() == eBDROMDLDisc)
    {
        TargetID = bdLastDecodedPSN + 1;
        AuxPtr = (ULONG *) (SCSDRAM + BD_CUR_AUX_ADDRESS());
    }
    else
    {
        switch (BdwtDisc.ReadDataMode)
        {
            case eNOT_USERDATA:
            case eREAD_DATA_FROM_DISC:
                TargetID = bdLastDecodedPSN + 1;
                AuxPtr = (ULONG *) (SCSDRAM + BD_CUR_AUX_ADDRESS());
                break;

            case ePRE_BUFFERING:
                TargetID = BdwtDisc.PrePSN;
                AuxPtr = (ULONG *) (SCSDRAM + BD_CUR_AUX_ADDRESS());
                break;

        }
    }

    ActualID = *AuxPtr;
    ActualID = SWAPEND_ULONG(ActualID);

    if (TargetID == ActualID)
        return TRUE;
    else
    {
        send_msg5SValue(1, "TargetID", 4, TargetID);
        send_msg5SValue(1, "ActualID", 4, ActualID);
        return FALSE;
    }
}

#endif //(BD_VERIFY_ID == 1)

#if (ENABLE_DISCARD_ICE == 1)
/*********************************************************************
* Function:     ICEAreaDecoder()
*
* Description:  Function is executed when BD codec Discard enable,
*               this function instead of bdProcessCluster.
*
* Returns:      none
* Params:       none
*********************************************************************/
void ICEAreaDecoder(void)
{
    ULONG TempPsn;
    BYTE IceDetected,*IceIndicator,i;
    vBOOL DisclFullSts,DisclPartSts;

            if ( (pdbGetDiscKind() == eBDRDisc || pdbGetDiscKind() == eBDRDLDisc ||
                  pdbGetDiscKind() == eBDREDisc || pdbGetDiscKind() == eBDREDLDisc)
                  && (bdDecFlag.fREADING_LEADIN == FALSE) )
            {
                SDHoldSts = (vBOOL)READ_FIELD(HAL_BDC_HLDSDS);
                FDHoldSts = (vBOOL)READ_FIELD(HAL_BDC_HLDFDS);
                DisclFullSts = (vBOOL)READ_FIELD(HAL_BDC_DISCLFULL);
                DisclPartSts = (vBOOL)READ_FIELD(HAL_BDC_DISCLPART);

                if ((SDHoldSts == 0 && FDHoldSts == 0)&&(DisclFullSts && DisclPartSts))
                {
                #if E_DIAGS
                    if ((WAIT_LDC_BIS == 0) && (READ_LDC_BIS == 1))
                    {
                        WAIT_LDC_BIS = 1;
                        LDC_ERRA = 0;
                        LDC_ERAS = 0;
                        //BIS_C = 0);
                        //BIS_UN_C = 0;
                        LDC_C = 0;
                        LDC_UN_C = 0;
                        MX_DFT_LEN = 0;
                        LDC_MAX_ERR = 0;
                        LDC_AUN_ERRATA = 0;
                        ERCO_CURRENT_ID.Long = (IcedClusterStAddr << 3);
                    }

                #endif  /* E_DIAGS */

                     IceDetected = FALSE;
                    switch (BdwtDisc.ReadDataMode)
                    {
                        case eREAD_DATA_FROM_DISC:
                            TempPsn = (bdLastDecodedPSN + 1);
                            break;
                        case ePRE_BUFFERING:
                            TempPsn = BdwtDisc.PrePSN;

                            break;
                    }

#if (SUN_FUNC_TEST == 1)
                    /* For function test, if the read location is ICED area and do not return it as the read fail */
                    if ((BdwtDisc.ReadDataMode == eNOT_USERDATA)&&(FTInfo.DiableCDBufferFullStopDecoding == TRUE))
                    {
                        TempPsn = (bdLastDecodedPSN + 1);
                    }
#endif                    

                    for(i=0;i<MAXICECNT;i++)
                    {
                        if ((ICEPSN[i]) == (TempPsn))
                        {
                            IceDetected = TRUE;
                            break;
                        }
                    }
                    if (IceDetected)
                    {
                        send_msg5SValue(SEND_READ_DVD_MSG,"ICEPsn2",4,bdLastDecodedPSN+1);

                        DataBuffer.CurDiscPtr += ONE_BD_CLUSTER;
                        DataBuffer.CurDiscPtr %= BUF_TOTAL_BLK_COUNT;
                        if ( DataBuffer.CurDiscPtr == 0 )
                        {
                            IceIndicator =  (BYTE *)(BD_ICE_INDICATOR_ADDR + ((BUF_TOTAL_BLK_COUNT - ONE_BD_CLUSTER) >> 5));
                        }
                        else
                            IceIndicator =  (BYTE *)(BD_ICE_INDICATOR_ADDR + ((DataBuffer.CurDiscPtr - ONE_BD_CLUSTER) >> 5));

                        bdErrorFlag.fEDC_ERROR = CLEAR;
                        bdErrorFlag.fUNCOR_LDC_ECC = CLEAR;
                        bdErrorFlag.fUNCOR_BIS_ECC = CLEAR;
                        bdErrorFlag.fSEQ_ID_ERROR = CLEAR;
                        bdErrorFlag.fID_MISMATCH = CLEAR;

                        *IceIndicator = eICE_BLOCK;

                        /*---------------------------------------------------------------
                            Reset BD Block Timer
                        ---------------------------------------------------------------*/
                        {
                            START_BD_CLUSTER_TIMER(BD_CLUSTER_TIMEOUT_VALUE);
                        }

                        /*---------------------------------------------------------------
                            Update count of clusters transferred, cluster count and
                            last PSN in buffer.
                        ---------------------------------------------------------------*/
                        ++bdDiscRequest.ClustersBuffered;
                        DataBuffer.SectorCount  += ONE_BD_CLUSTER;
                        bdLastDecodedPSN        += ONE_BD_CLUSTER;

                        /*--------------------------------------------------------------
                            Adjust the SpeedCount and SpeedUpReady.
                        ---------------------------------------------------------------*/
                        if(SpeedCount < SPEED_UP_COUNT_LIMIT - ONE_BD_CLUSTER)
                        {
                            if( SpeedCount >= 0 )
                                SpeedCount += ONE_BD_CLUSTER;
                            else
                                SpeedCount = ONE_BD_CLUSTER;
                        }

                        if(SpeedUpReady > BD_SPEED_UP_READY)
                            SpeedUpReady--;

                        if (BdwtDisc.ReadDataMode == ePRE_BUFFERING)
                        {
                            if (bdDecFlag.fFIRST_HOST_CLUSTER == TRUE)
                            {
                                bdDecFlag.fFIRST_HOST_CLUSTER = FALSE;
                                gFirstClusterDone = TRUE;
                            }

                            BDAppendPreBuffer();
                        }

                        if (DataBuffer.CacheEndLBA == 0L)
                            DataBuffer.CacheEndLBA = 0x1FL;
                        else
                            DataBuffer.CacheEndLBA += ONE_BD_CLUSTER;

                        TempPsn = (ULONG)(HAL_BD_CODEC_GetStopAddress() << 5);
                        for(i=0;i<MAXICECNT;i++)
                        {
                            if ((ICEPSN[i]) == (TempPsn))
                            {
                                bdDecIntFlag.fECC_ALL_DONE_INT = SET;
                            /*---------------------------------------------------------------
                                Disable BD interrupts and clear interrupts and enable flags
                            ---------------------------------------------------------------*/
                                HAL_BD_CODEC_DISABLE_DECODER_INTS();
                                HAL_BD_CODEC_Disable_Buffering();
                                STOP_BD_CLUSTER_TIMER();
                                break;
                            }
                        }
                    }
                //target miss
                    if(READ_LDC_BIS == 0)
                    {
                        switch (BdwtDisc.ReadDataMode)
                        {
                            case eREAD_DATA_FROM_DISC:
                                if ( IcedClusterStAddr<<3 > ((bdLastDecodedPSN + 1)+MAXICECNT*ONE_BD_CLUSTER))
                                {
                                    bdErrorFlag.fTID_MISS = SET;
                                    send_msg80(SEND_ATAPI_CMD,0x7100ED);
                                }
                                break;
                            case ePRE_BUFFERING:
                                if ( IcedClusterStAddr<<3 > ((bdLastDecodedPSN + 1)+MAXICECNT*ONE_BD_CLUSTER))
                                {
                                    bdErrorFlag.fTID_MISS = SET;
                                    send_msg80(SEND_ATAPI_CMD,0x7100EE);
                                }
                                break;
                        }

#if (SUN_FUNC_TEST == 1)
                        /* For function test, if the read location is ICED area need to care about the target miss issue. */
                        if ((BdwtDisc.ReadDataMode == eNOT_USERDATA)&&(FTInfo.DiableCDBufferFullStopDecoding == TRUE))
                        {
                            bdErrorFlag.fTID_MISS = SET;
                        }
#endif
                    }
                }
            }
}
#endif  /* ENABLE_DISCARD_ICE == 1 */

#endif // BD_READ
