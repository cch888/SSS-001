/****************************************************************************
*                (c) Copyright 2001 - 2007 SUNEXT DESIGN, INC.
*                           All Right Reserved
*
*
* FILENAME:   BD_Diag.c
*
* DESCRIPTION
*   This file contains BD Diagnostic commands.
*
*   $Revision: 56 $
*   $Date: 11/03/28 2:36p $
*
* NOTES:
*
**************************** SUNEXT  CONFIDENTIAL *******************************/
#include ".\common\globals.h"
#include ".\common\globtype.h"
#include ".\common\chip.h"

#include ".\hif\Diag.h"
#include ".\dvd\dvDef.h"
#include ".\dvd\dvGvars.h"
#include ".\bd\bdDef.h"
#include ".\bd\bdVars.h"
#include ".\bd\bdMacro.h"
#include ".\bd\bdDiag.h"
#include ".\al\REG_bd_codec.h"
#include ".\al\HAL_bd_codec.h"
#if (CALIBRATION_MANAGEMENT == 1)
#include ".\servo\svo_calib.h"
#include ".\servo\svo_com.h"
#include ".\servo\svo_seek.h"
#include ".\servo\svoVAR.h"
#endif

extern BYTE DiscLayer;
extern BOOL ResetDecoder;
extern BYTE SkipErrRateNum;
extern BYTE Addr_mode;
extern ULONG Last_PSN;
extern PSNs  bdConvertLBAtoPSN(ULONG LBA);
extern vBOOL SDHoldSts;
extern vBOOL FDHoldSts;
#if BD_ENABLE && E_DIAGS
StLongU BlerERR;
StLongU BlerLDC;
StLongU BlerBIS;
StLongU BlerControl;
StLongU BlerId;
StLongU BlerAun;

#if (ENABLE_DFE_ICE_DETECT == 1)
extern ULONG IcedClusterStAddr;
#endif

void diaBDVendorSetSpeedSeek(void)
{
    ULONG   ClusterRequest;
    StLongU StartAdr;
    PSNs    StartPSN, EndPSN;
    BYTE    seek_retry_count;
    BYTE    tempByte;
    ULONG   LSN0StartPSN;
    ULONG   DelayTimer;

    LDC_C = 0;
    LDC_UN_C = 0;
    LDC_ERRA = 0;
    LDC_ERAS = 0;
    BlerId.Long = 0;
    LDC_AUN_ERRATA = 0;
#if (ENABLE_DFE_ICE_DETECT == 1)
    IcedClusterStAddr = 0;
#endif
    BIS_C = 0;
    BIS_UN_C = 0;
    DiscLayer = 0;
    ResetDecoder = FALSE;
    switch(pdbGetDiscKind())
    {
        case eBDROMDisc:
        case eBDROMDLDisc:
            LSN0StartPSN = BD_DATAZONE_START_PSN;
            break;

        default:
            LSN0StartPSN = pdbGetBDLsn0StartPSN();
            break;
    }

    ClearDataBuffer();

    //e1XDCLV,  e2XDCLV,  e4XDCLV
    if (Iram_Cmd_block.bf[8] != 0x00)
    {
        SkipErrRateNum = 1;

        if ( (Iram_Cmd_block.bf[2] & 0x78) == 0x00)     //use old definition
        {
            switch(Iram_Cmd_block.bf[8])
            {
                case 1:
                    tempByte = e1XDCLV;
                    break;

                case 2:
                   tempByte = e2XDCLV;
                   break;
                case 3:

                    tempByte = e3XDCLV;
                    break;

                case 4:
                    tempByte = e4XDCLV;
                    break;

                default:
                    SkipErrRateNum = 0;
                    BUILD_SENSE(0x05,0x24,0x00,0x70);   /* INVALID PARAMETER IN CDB */
                    return;
            }
        }
        else if ((Iram_Cmd_block.bf[2] & 0x78) == 0x10)//CAV mode
        {
            switch(Iram_Cmd_block.bf[8])
            {
                case 1:
                    tempByte = e1XDCAV; break;
                case 2:
                    tempByte = e2XDCAV; break;
                case 3:
                    tempByte = e3XDCAV; break;
                case 4:
                    tempByte = e4XDCAV; break;
                case 5:
                    tempByte = e5XDCAV; break;
                case 6:
                    tempByte = e6XDCAV; break;
                case 7:
                    tempByte = e7XDCAV; break;
                case 8:
                    tempByte = e8XDCAV; break;
                case 12:
                    tempByte = e12XDCAV; break;
                case 14:
                    tempByte = e14XDCAV; break;
                default:
                    SkipErrRateNum = 0;
                    BUILD_SENSE(0x05,0x24,0x00,0x66);                    /* INVALID PARAMETER IN CDB */
                    return;
            }
        }
        else if ((Iram_Cmd_block.bf[2] & 0x78) == 0x08) //CLV mode
        {
            switch(Iram_Cmd_block.bf[8])
            {
                case 1:
                    tempByte = e1XDCLV;
                    break;

                case 2:
                    tempByte = e2XDCLV;
                    break;

               case 3:
                    tempByte = e3XDCLV;
                    break;


                case 4:
                    tempByte = e4XDCLV;
                    break;

                default:
                    SkipErrRateNum = 0;
                    BUILD_SENSE(0x05,0x24,0x00,0x6f);   /* INVALID PARAMETER IN CDB */
                    return;
            }
        }
        else if ((Iram_Cmd_block.bf[2] & 0x78) == 0x78)
        {
                SkipErrRateNum = 0;
                BUILD_SENSE(0x05,0x24,0x00,0x84);       /* INVALID PARAMETER IN CDB */
                return;
        }

        ServoPause();
        SpeedMD = BypassSpeedMD;

        if(ServoSetSpeed(tempByte) == FAIL)
        {
            BUILD_SENSE(0x05,0x24,0x00,0x85);           /* INVALID PARAMETER IN CDB */
            return;
        }
        ServoPause();
    } //if (Iram_Cmd_block.bf[8] != 0x00)

    StartAdr.byte.u = Iram_Cmd_block.bf[4]; // Get the host request starting address from the command block
    StartAdr.byte.h = Iram_Cmd_block.bf[5];
    StartAdr.byte.m = Iram_Cmd_block.bf[6];
    StartAdr.byte.l = Iram_Cmd_block.bf[7];

    //LBA / PSN depends on Iram_Cmd_block.bf[2] bit 1
    if (Iram_Cmd_block.bf[2] & 0x02)
        Addr_mode = 1;   //PSN mode
    else
        Addr_mode = 0;   //LBA mode

    if ((Addr_mode == 0) || (pdbGetBDLayers() == BD_DUAL_LAYER))
    {
        if (Addr_mode == 1) //PSN mode in dual layer disc
        {
            if(StartAdr.Long <= pdbGetBDMaxPSN(0))
            {
                StartAdr.Long = StartAdr.Long - LSN0StartPSN;
                DiscLayer = 0;
            }
            else
            {   /* Layer 1 */
                StartAdr.Long = bdConvertPSNtoLBA(StartAdr.Long);
                DiscLayer = 1;
            }
        }

        StartPSN = bdConvertLBAtoPSN(StartAdr.Long);
        bdReadAheadStartPSN = StartPSN.PSN - ONE_BD_CLUSTER;   //don't make bdReadAheadStartPSN equal to bdLastDecodedPSN

        if (StartAdr.Long > pdbGetBDMaxLBA())
        {
            BUILD_SENSE(0x05,0x21,0x00,0x40);
            return;
        }
    }
    else //PSN mode in single layer disc
    {
        if (pdbGetBDLayers() == BD_DUAL_LAYER)
        {
            if (StartAdr.Long > pdbGetBDMaxPSN(1))
            {
                BUILD_SENSE(0x05,0x21,0x00,0x41);
                return;
            }

            StartPSN.PSN = StartAdr.Long;
            StartPSN.Layer = (StartPSN.PSN & 0x1000000) ? eLayer1 : eLayer0;

            bdReadAheadStartPSN = pdbGetBDMaxPSN(1);    //don't make bdReadAheadStartPSN equal to bdLastDecodedPSN
        }
        else
        {
            if (StartAdr.Long > pdbGetBDMaxPSN(0))
            {
                BUILD_SENSE(0x05,0x21,0x00,0x42);
                return;
            }
            StartPSN.PSN = StartAdr.Long;
            StartPSN.Layer = eLayer0;
            bdReadAheadStartPSN = pdbGetBDMaxPSN(0);    //don't make bdReadAheadStartPSN equal to bdLastDecodedPSN
        }
    }

    StartPSN.PSN = pdbGetBDClusterStart(StartPSN.PSN);
    EndPSN.PSN = (StartPSN.Layer == eLayer1) ? pdbGetBDMaxPSN(1) : pdbGetBDMaxPSN(0);
    EndPSN.Layer = StartPSN.Layer;
    Last_PSN = StartPSN.PSN;
    ClusterRequest = (EndPSN.PSN - StartPSN.PSN) / ONE_BD_CLUSTER;

    if (ClusterRequest == 0L)
    {
        BUILD_SENSE(0x05,0x24,0x00,0x86);               /* INVALID PARAMETER IN CDB */
        return;
    }

#if (DPD_COMPENSATE_OFFSET_DIAG==1)
    if (svoVar_TrackingMethod == TRK_METHOD_DPD)
    {
        if (StartPSN.Layer != Layer_Index)
        {
            ServoSeek(StartPSN.PSN,StartPSN.Layer);
        }
        ServoCalibDpdWholeDisc();
    }
    #if 0//(CALIBRATION_MANAGEMENT == 1)
    else
    #endif
#endif//#if (DPD_COMPENSATE_OFFSET_DIAG==1)

#if (CALIBRATION_MANAGEMENT == 1)
    if (svoVar_discDiameter == e12cm)
    {
        if (StartPSN.Layer != Layer_Index)//improve whole disc pre calibration
        {
            ServoSeek(StartPSN.PSN,StartPSN.Layer);
        }
        ServoCalibWholeDisc();
    }
#endif//#if (CALIBRATION_MANAGEMENT == 1)

    bdDiscRequest.Type          = INIT_READ;
    bdDiscRequest.TargetID.Long = StartPSN.PSN;
    bdDiscRequest.TargetLayer   = StartPSN.Layer;
    bdDiscRequest.ClusterCount  = ClusterRequest;

    WAIT_LDC_BIS = 0;
    READ_LDC_BIS = 1;
    WRITE_REG(HAL_HSTREG_MAP->BUFPGCMPL, 0x100);

    if (BD_BUFFERING() == ON)
    {
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
            send_msg5S(SEND_READ_DVD_MSG, "SDFD HLDS Err 8");
            break;
        }
        SDHoldSts = (vBOOL)READ_FIELD(HAL_BDC_HLDSDS);
        FDHoldSts = (vBOOL)READ_FIELD(HAL_BDC_HLDFDS);
    }
#if (ENABLE_DISCARD_ICE == 1)
    HAL_BD_CODEC_Disable_DiscardIce();
#endif
    HAL_BD_CODEC_DISABLE_DECODER();

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

    HAL_BD_CODEC_ENABLE_DECODER();          /* Turn on BD decoder */
    HAL_BD_CODEC_STREAM_PASS_ALL_OFF();     /* Disable Stream Pass All function */

    /*--------------------------------------------------------
        Set the current disc buffer pointer
    ---------------------------------------------------------*/
    HAL_BD_CODEC_SetDiscPtr(DataBuffer.CurDiscPtr);

    /*---------------------------------------------------------------
        Load Stream Start & Stop Address
    ----------------------------------------------------------------*/
   if (pdbGetDiscKind() == eBDROMDisc || pdbGetDiscKind() == eBDROMDLDisc)
   {
        HAL_BD_CODEC_SetStartAddress((StartPSN.PSN >> 5));
        HAL_BD_CODEC_SetStopAddress((EndPSN.PSN >> 5));
    }
    else
    {
        HAL_BD_CODEC_SetStartAddress((StartPSN.PSN >> 5));
        HAL_BD_CODEC_SetStopAddress((EndPSN.PSN >> 1));
    }

    /*---------------------------------------------------------------
        Clear all BD flags and status and set transfer state
    ----------------------------------------------------------------*/
    HAL_BD_CODEC_CLEAR_DECODER_STATUS();


    //HAL_BD_CODEC_ENABLE_DECODER_INTS();
    WRITE_REG(HAL_BDCREG_MAP->BDDINT1E, HAL_BDC_ERSTE|HAL_BDC_FDEDCFAIE|HAL_BDC_FDLDCUNCE|HAL_BDC_FDBISUNCE|HAL_BDC_CONTERRE);
    WRITE_REG(HAL_BDCREG_MAP->BDDINT2E, HAL_BDC_FDSTPADFE);

    /*--------------------------------------------------------
        Set the current disc buffer pointer
    ---------------------------------------------------------*/
    HAL_BD_CODEC_SetDiscPtr(0);

    /*---------------------------------------------------------------
        Call Seek
    ----------------------------------------------------------------*/

    seek_retry_count = 0;
    while(1)
    {
        if (seek_retry_count > 2)
        {
            spdUnhold();
            READ_LDC_BIS = 0;
        #if (ENABLE_DISCARD_ICE == 1)
            HAL_BD_CODEC_Disable_DiscardIce();
        #endif
            BUILD_SENSE(0x03,0x02,0x00,0x36);   // NO SEEK COMPLETE
            return;
        }

        spdHoldRPM(spdGetTargetRPM(svoPBA2Radius(StartPSN.PSN), svoSpeedVar.SpdCurrentSpeed));
        spdWaitSpdInRange();
        if (ServoSeek((StartPSN.PSN -5 * ONE_BD_CLUSTER), StartPSN.Layer) == PASS)
        {
            spdUnhold();
            ClearAddressRead();

            /* improve atip seek performance */
            if (Iram_Cmd_block.bf[2] & 0x01)
            {
                Svr_pause_reqire = TRUE;
                return;
            }
            else
            {   //TBD ??
                //if (ServoSeek(StartPSN.PSN,StartPSN.Layer) == PASS)
                {
                    ClearAddressRead();
                    Svr_pause_reqire = FALSE;
                    break;
                }
            }
        }
        seek_retry_count++;
    }

    HAL_BD_CODEC_ENABLE_TARGET_MISS_INT();

    WRITE_REG(HAL_BDC_AUNFLY_REGISTER, 0x1E);   //preset fly AUN
    WRITE_FIELD(HAL_BDC_STATUPRAT, 0);          //update error statistics per cluster
    WRITE_FIELD(HAL_BDC_STATENCAP, 1);          //enable error statistics
}


void diaBDBLERCommand()
{
    ULONG clock, clock2;
    ULONG clock3;
    BYTE  ftimeout;
    BYTE decodeErr;
   // int     tempData;

    ULONG CurrentDataPSN;
    ULONG AUNFLYRegister, AUNFLYStatus, AUNStatusRegister, AUNRegister, AUNEndRegister;
    BYTE AUNDecodingStatus;
    ULONG LSN0StartPSN;

#if (MSG_OUT_MODE == MSG_USBPORT)
    if (hifDebugMsgFlag & (SEND_QCODE_DATAID|SEND_ATIP_WOBBLE))
        Send_timef = 1;
#endif

    decodeErr = 0;

#if (BypassDecoderEr == 1)
    IGNORE_DECODER_ERROR = FALSE;
#endif

    switch(pdbGetDiscKind())
    {
        case eBDROMDisc:
        case eBDROMDLDisc:
            LSN0StartPSN = BD_DATAZONE_START_PSN;
            break;

        default:
            LSN0StartPSN = pdbGetBDLsn0StartPSN();
            break;
    }
    if ((Iram_Cmd_block.bf[5] == 0x0A))
    {
    #if (ENABLE_DISCARD_ICE == 1)
        HAL_BD_CODEC_Disable_DiscardIce();
    #endif
        READ_LDC_BIS = 0;
        Send_timef = 0;
        ResetDecoder = FALSE;
        bdStopDiscTransfer();
        bdResetDiscFlags();
        Svr_pause_reqire = TRUE;
        St_diag_ena = FALSE;        /* Diagnostics Disabled */
        AutoCommandComplete();
        return;
    }

    if(SkipErrRateNum)
    {
        //sig.mux1 = halAFE_MUX1ADSEL_VREF_FCTS;
        //ServoADCSignal = AfeRouteSignalToAD(AFE_MUXTOAD1,sig);
    }

    // BD disc
    if ((pdbGetSpeed() & (e_Partial|e_CAV)) == (e_Partial|e_CAV))
    {
       //SetSCRegField_1B(f_rw_OPCDISCK,0x0C); // 6.25 mv/us
    }

    StartTimer(&clock);
    while (1)
    {
        if (ReadTimer(&clock) > 1000L )
        {
            CurrentDataPSN = Last_PSN;
            break;
        }

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
                    //CurrentDataPSN = AUNRegister;
                    CurrentDataPSN = AUNFLYRegister;
                    SendMsgCn(HEADER_ID_MSG,4,0x3B060D,A4B(svoIramVar.currentID.SectorNum.all32));//CheckHeaderID AUN Address :
                    break;
                }
            }
        }

        if( pdbGetIDIntStatus() == TRUE )
        {
            pdbSetIDIntStatus(FALSE);
            if(pdbGetIDOk() == TRUE)
            {   // Monitor Wobble(ADIP/LPP) Address
                CurrentDataPSN = pdbGetWblAddress()<<3; //PAA->PSN
                //send_msg5SValue(SEND_ATAPI_CMD, "CurAUN1", 4, CurrentDataPSN);
                break;
            }
        }
        ExitProcess();
    }
    Last_PSN = CurrentDataPSN;

    HAL_BD_CODEC_Enable_Buffering();

    StartTimer(&clock);
    StartTimer(&clock2);

    StartTimer(&clock3);
    ftimeout = 0;

    while(1)
    {
        if (EjectKeyInfo.PressedValid)
        {
        #if (ENABLE_DISCARD_ICE == 1)
            HAL_BD_CODEC_Disable_DiscardIce();
        #endif
            READ_LDC_BIS = 0;
            Send_timef = 0;
            bdStopDiscTransfer();
            bdResetDiscFlags();
            Svr_pause_reqire = TRUE;
            BUILD_SENSE(0x02,0x3A,0x00,0x37);       // NOT READY, MEDIUM NOT PRESENT
            AutoCommandComplete();
            return;
        }

        if (ReadTimer(&clock3) > 30000L) //30sec
        {
        #if (ENABLE_DISCARD_ICE == 1)
            HAL_BD_CODEC_Disable_DiscardIce();
        #endif
            READ_LDC_BIS = 0;
            Send_timef = 0;
            bdStopDiscTransfer();
            bdResetDiscFlags();
            Svr_pause_reqire = TRUE;
            BUILD_SENSE(0x03,0x02,0x00,0x37);   // NO SEEK COMPLETE
            AutoCommandComplete();
            return;
        }
        if(ReadTimer(&clock) > 1000L)
        {
            ftimeout = 1;
            StartTimer(&clock);
        }

        if((WAIT_LDC_BIS == 1) && (ResetDecoder == FALSE))
        {
            if(LDC_UN_C == 0)
            {
                if(bdErrorFlag.fUNCOR_LDC_ECC)
                {
                    bdErrorFlag.fUNCOR_LDC_ECC = 0;
                }
                else if(bdErrorFlag.fEDC_ERROR)
                {
                    bdErrorFlag.fEDC_ERROR = 0;
                    LDC_UN_C = 1111;        //PO_UN_C
                    WAIT_LDC_BIS = 1;
                }
                else if(bdErrorFlag.fSEQ_ID_ERROR)
                {
                    bdErrorFlag.fSEQ_ID_ERROR = 0;
                    LDC_UN_C = 3333;        //PO_UN_C
                    WAIT_LDC_BIS = 1;
                }
                else if (bdErrorFlag.fTID_MISS)
                {
                    bdErrorFlag.fTID_MISS = 0;
                    LDC_UN_C = 4444;        //PO_UN_C
                    WAIT_LDC_BIS = 1;
                }
                else if (bdErrorFlag.fID_MISMATCH)
                {
                    bdErrorFlag.fID_MISMATCH = 0;
                    LDC_UN_C = 6666;        //PO_UN_C
                    WAIT_LDC_BIS = 1;
                }
                else if (dvdErrorFlag.fID_ERROR)
                {
                    bdErrorFlag.fID_ERROR = 0;
                    LDC_UN_C = 7777;        //PO_UN_C
                    WAIT_LDC_BIS = 1;
                }
                else if (bdErrorFlag.fSEEK_FAIL)
                {
                    bdErrorFlag.fSEEK_FAIL = 0;
                    LDC_UN_C = 8888;        //PO_UN_C
                    WAIT_LDC_BIS = 1;
                }
            }

            CurrentDataPSN= BlerId.Long;

            break;
        }
        else if((ReadTimer(&clock2) > 200L) || (ResetDecoder == TRUE) || (ftimeout))
        {
            WAIT_LDC_BIS = 0;
            ResetDecoder = FALSE;

            if ( BD_BUFFERING() == ON )
            {
                bdStopDiscTransfer();
            }

            /* TBD !!
            if (bdErrorFlag.fSYNC_LOST == 1)
            {
                bdErrorFlag.fSYNC_LOST = 0;

                if(HAL_GLOBAL_GetRfPresent())
                {
                    LDC_UN_C = 2222;        //PO_UN_C
                }
                else
                {
                    PI_C = 32767;
                    PO_C = 182;
                    PI_UN_C = 32767;
                    PO_UN_C = 182;
                }
                WAIT_LDC_BIS = 1;
            } */

            if(ftimeout)
            {
                ftimeout = 0;
                LDC_C = 208;

                /* TBD !!
                if(HAL_GLOBAL_GetRfPresent())
                {
                      LDC_UN_C = 9999;
                }
                else
                {
                    LDC_C = 208;    //PI_E

                    PI_C = 32767;
                    PO_C = 182;
                    PI_UN_C = 32767;
                    PO_UN_C = 182;
                } */
                WAIT_LDC_BIS = 1;
            }


            bdResetDiscFlags();
            if(pdbGetBDLayers() == BD_DUAL_LAYER)
            {
                CmdSeekOk = 0;

                send_msg5SValue(1,"LstPSN",4,Last_PSN);
                if(Last_PSN <= (pdbGetBDMaxPSN(0)-0x20)) //layer 0
                {
                    if(pdbGetBDStartPSN(0) > Last_PSN)
                        Last_PSN = pdbGetBDStartPSN(0);
                    Iram_Cmd_block.bf[2] = 0x02; //PSN mode
                    Iram_Cmd_block.bf[8] = 0x00;
                    Iram_Cmd_block.bf[4] = Last_PSN>>24;
                    Iram_Cmd_block.bf[5] = Last_PSN>>16;//StartLBA.byte.h;
                    Iram_Cmd_block.bf[6] = Last_PSN>>8;//StartLBA.byte.m;
                    Iram_Cmd_block.bf[7] = Last_PSN;//StartLBA.byte.l;
                    diaBDVendorSetSpeedSeek();
                }
                else
                {
                    if(pdbGetBDStartPSN(1) > Last_PSN)
                        Last_PSN = pdbGetBDStartPSN(1);
                    Iram_Cmd_block.bf[2] = 0x06;  //layer 1,PSN mode
                    Iram_Cmd_block.bf[8] = 0x00;
                    Iram_Cmd_block.bf[4] = Last_PSN>>24;
                    Iram_Cmd_block.bf[5] = Last_PSN>>16;//StartLBA.byte.h;
                    Iram_Cmd_block.bf[6] = Last_PSN>>8;//StartLBA.byte.m;
                    Iram_Cmd_block.bf[7] = Last_PSN;//StartLBA.byte.l;
                    diaBDVendorSetSpeedSeek();
                }
            }
            else

            {
                HAL_BD_CODEC_ENABLE_DECODER();  /* Turn on BD decoder */

                HAL_BD_CODEC_SetDiscPtr(0);
                HAL_BD_CODEC_CLEAR_DECODER_STATUS();

                //HAL_BD_CODEC_ENABLE_DECODER_INTS(); //ENABLE_DVD_DECODER_INTS();
                WRITE_REG(HAL_BDCREG_MAP->BDDINT1E, HAL_BDC_ERSTE|HAL_BDC_FDEDCFAIE|HAL_BDC_FDLDCUNCE|HAL_BDC_FDBISUNCE|HAL_BDC_CONTERRE);
                WRITE_REG(HAL_BDCREG_MAP->BDDINT2E, HAL_BDC_FDSTPADFE);
            }
            HAL_BD_CODEC_Enable_Buffering();    //ENABLE_DVD_BUFFERING();
            StartTimer(&clock2);
        }

        ExitProcess();

    } //while(1)

    Iram_Buf_dram_addr = BUFFER_SCRATCH;

    if(SkipErrRateNum)
    {
        //iBeta_modulation[0] = iBeta_modulation[1] = iBeta_modulation[2] = tempData;
    }
    else
    {
        //iBeta_modulation[0] = iBeta_modulation[1];
        //iBeta_modulation[1] = iBeta_modulation[2];
        //iBeta_modulation[2] = tempData;
    }

    if(Addr_mode == 0) //LBA mode
    {
        if(bdDiscRequest.TargetLayer == eLayer1)
            CurrentDataPSN = CurrentDataPSN - pdbGetBDStartPSN(1) + pdbGetBDEndLBALayer0() + 1L;
        else
            CurrentDataPSN -= LSN0StartPSN;
    }

    WriteSDRAM( CurrentDataPSN >> 16);
    WriteSDRAM( CurrentDataPSN >> 8);
    WriteSDRAM( CurrentDataPSN);

    if(SkipErrRateNum)
    {
        SkipErrRateNum--;
        WriteSDRAM( 0);
        WriteSDRAM( 0);
        WriteSDRAM( 0);
        WriteSDRAM( 0);
        WriteSDRAM( 0);
        WriteSDRAM( 0);
        WriteSDRAM( 0);
        WriteSDRAM( 0);
        WriteSDRAM( 0);
    }
    else
    {
        WriteSDRAM(BHIGH(LDC_ERRA));    //PI_C
        WriteSDRAM(BLOW(LDC_ERRA));     //PI_C
        WriteSDRAM(BHIGH(LDC_UN_C));    //PO_UN_C
        WriteSDRAM(BLOW(LDC_UN_C));     //PO_UN_C
        WriteSDRAM(BHIGH(LDC_ERAS));    //PI_UN_C
        WriteSDRAM(BLOW(LDC_ERAS));     //PI_UN_C
        WriteSDRAM(BHIGH(LDC_C));       //PO_C
        WriteSDRAM(BLOW(LDC_C));        //PO_C
        if (pdbGetDiscKind() == eBDROMDisc || pdbGetDiscKind() == eBDROMDLDisc)
            WriteSDRAM(LDC_AUN_ERRATA);                 //AUN Error Amount
        else
            WriteSDRAM(Iram_AtipAdipLppErrorAmount);    //ADIP Error Amount
    }

    WriteSDRAM(0);  //Beta
    WriteSDRAM(0);  //Beta

    WAIT_LDC_BIS = 0;

    //JITTER
#if (USE_REALTIME_JITTER == 1)
        //measure jitter
    tempData = HAL_DFE_ReadJitterX10();
    WriteSDRAM(tempData>>8);
    WriteSDRAM(tempData);
#else
    WriteSDRAM(0);
    WriteSDRAM(0);
#endif /* USE_REALTIME_JITTER */

    if(SkipErrRateNum)
    {
        WriteSDRAM(0);
        WriteSDRAM(0);
    }
    else
    {
        WriteSDRAM(MX_DFT_LEN);   //Maximum defect length estimation
        WriteSDRAM(LDC_MAX_ERR);  //Maximum nr of LDC erasures in a window
    }

    /* set up parameters and xfer mechenism status data to host */
    SendParameterDataToHost(BUFFER_SCRATCH, 18, TRUE);  /* xfer data and post cmd complete */
}

#endif
