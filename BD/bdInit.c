/*****************************************************************************
*                (c) Copyright 2000 - 2007 SUNEXT DESIGN, INC.
*                            All Rights Reserved
*
*
* FILENAME:     $Workfile: bdInit.c $
*
* DESCRIPTION   This file contains the initialization routines for a power-on,
*               reset, or BD disc detection.
*
* $Revision: 176 $
* $Date: 11/03/15 1:25p $
*
**************************** SUNEXT CONFIDENTIAL ****************************/

#include ".\common\globtype.h"
#include ".\hif\flash.h"
#include ".\oem\oem_spd_tbl.h"
#include ".\bd\bdDef.h"
#include ".\bd\bdMacro.h"
#include ".\bd\bdVars.h"
#include ".\bd\bdInit.h"
#include ".\hif\GP_mac.h"
#include ".\bd\bdseek.h"
#include ".\bd\bdUtil.h"

#if (ENABLE_AACS)
#include ".\crypt\aacs.h"
#endif

#include ".\AL\REG_HAL.h"
#include ".\AL\REG_global.h"
#include ".\AL\REG_dfe.h"
#include ".\AL\HAL_bd_codec.h"
#include ".\AL\HAL_dfe.h"
#include ".\AL\HAL_wobble.h"
#include ".\al\reg_intctrl.h"
#include ".\common\intctl.h"

#include ".\AL\LAL_wobble.h"
#include ".\AL\REG_demod.h"
#include ".\AL\HAL_global.h"
#if (EN_POWER_CONTROL)
#include ".\hif\Powerctl.h"
#endif

#include ".\servo\svo_lib.h"
#include ".\AL\HAL_afe.h"

#if (BD_ENABLE)
const PICInfoAddrS BDROM_PIC_Info[] =
{
    {0x0B9200, 1},
    {0x0BFC00, 1},
    {0x0C6600, 1},
    {0x0CD000, 1},
    {0x0D3A00, 1}
};

const PICInfoAddrS BDWRITABLE_PIC_Info[] =
{
    {0x0D8EC0, 1},
    {0x0D9300, 1},
    {0x0D9740, 1},
    {0x0D9B80, 1},
    {0x0D9FC0, 1}
};

#if (ENABLE_AACS == 1)
const MKBInfoAddrS BDROM_MKB_Info[] =
{
    {0x0B9220, 1},
    {0x0BFC20, 1},
    {0x0C6620, 1},
    {0x0CD020, 1},
    {0x0D3A20, 1}
};

const MKBInfoAddrS BDWRITABLE_MKB_Info[] =
{
    {0x0DC000, 1},
    {0x0DCC00, 1}
};

extern ULONG RomMarkKeyBuffer[4];

#endif  /* ENABLE_AACS == 1 */

const AddrStEndS DMA_DDS_Info[] =
{
    {0x0DD400, 0x0DD480},
    {0x0FF400, 0x0FF480}
};

const AddrStEndS PAC_Info[] =
{
    {0x0DD000, 0x0DD400},
    {0x0FFC00, 0x100000}
};

#if (ENABLE_AACS)
BYTE HasRomMarkIvIndicator;
extern ULONG MkbVersionOnDisc;
extern ULONG MkbVersionOnFlash;
#endif

#if (USE_EEPROM_COLLI_POS == 1)
extern void SaveColliPosIntoEEPROM(void);
#endif
/*******************************************************************************
    INTERNAL FUNCTIONS
********************************************************************************/
#if BDRE_BYPASS_LEADIN
void BdCreateDefaultPIC(void);
#endif

BYTE BdCreateDefaultDMA(void);
BYTE CheckBDStartAddr(void);
void BdCreateDefaultSRRI(void);
void BdCreateDefaultDFL(void);

ULONG DFLStrAddr;
/*****************************************************************************

    FUNCTION        BdStartUpSuccessful

    DESCRIPTION     Sets variables and values for a successful start up and
                    read lead in of a BD disc.

    PARAMETERS

    RETURNS

*****************************************************************************/
void BdStartUpSuccessful(void)
{
    /*-------------------------------------------------------------------
        Set LED
    --------------------------------------------------------------------*/
    ReadyLED(LED_ON);

    /*-------------------------------------------------------------------
        Initialize buffer
    --------------------------------------------------------------------*/
    ClearDataBuffer();

    /*-------------------------------------------------------------------
       Update system flags and set player mode
    --------------------------------------------------------------------*/
    SupportMaximumSpeed();
    Svr_pause_reqire = TRUE;

    /*-------------------------------------------------------------------
       Set up proper LED status
    --------------------------------------------------------------------*/
#if (LED_NUM == 1)
    ReadyLED(LED_OFF);
#elif (LED_NUM == 2)
    ReadyLED(LED_ON);
#endif /* LED_NUM == 2 */

    St_drv_ready    = TRUE;         /* Indicate media is accessible */
    St_no_disc      = FALSE;        /* Clear no disc flag */
    st_disc_change    = TRUE;       /* Mark disc changed */
#if (ENABLE_SATA_FE == 1)	/* Notify host that media change happens */
    if((St_cmd_inproc == FALSE) && (SATA_FE & bASYNNOTI))
    {
        if (READ_REG(HAL_HSTREG_MAP->SATASTAT0)&(HAL_HST_SLUMBER|HAL_HST_PARTIAL) || SATA_intf_pwr_state&(bIPMACK_P|bIPMACK_S))
        {
            SATA_intf_pwr_state = 0;
            WRITE_FIELD(HAL_HST_DWCLKON, 1);
            WRITE_FIELD(HAL_HST_TXWKUPREQ, 1);
            DelaymS(1);
        }
        if((READ_REG(HAL_HSTREG_MAP->SATASTAT1)&(HAL_HST_LINKRDY|HAL_HST_PHYRDY))==(HAL_HST_LINKRDY|HAL_HST_PHYRDY))
        {
            WRITE_REG(HAL_HSTREG_MAP->RQTXFSTYP,0xA1);         /* Device to Host Set Device Bits FIS */
            WRITE_FIELD(HAL_HST_NOTIFYREQ, 1);
        }
    }
#endif
    ataMediaIsChanged = TRUE;       /* for ATA command ataGetMediaStatus() in at_cmds.c */
    St_become_ready = FALSE;
    Iram_Svr_player_mode = READING_MODE;    /* Disc is in reading mode */

    if(St_load_disc == TRUE)
    {
        /*-------------------------------------------------------------------
           If Start/Stop command issued
        --------------------------------------------------------------------*/
        Iram_Svr_player_mode = START_UP_MODE;
        St_load_disc = FALSE;
    }
}


#if (BD_LAYER_VERIFY == 1)
/*****************************************************************************

    FUNCTION        BdVerifyDisc

    DESCRIPTION     Verifies if the player DB has the correct BD disc kind

    PARAMETERS

    RETURNS         TRUE    PlayerDB contains correct Disc Kind information,
                    FALSE   PlayerDB had incorrect Disc Kind information and
                            was changed to proper disc kind

*****************************************************************************/
BYTE BdVerifyDisc(void)
{
    BOOL TempB;
    discKindT DiscKind, DetectedDisc;

    /*-------------------------------------------------------------------
       Determine Disc type from lead in information
    --------------------------------------------------------------------*/
    DetectedDisc.Bit.Layers = eLayersSingle;
    DetectedDisc.Bit.DiscStd = eDiscBD;

    switch (pdbGetBDType())
    {
        case BD_ROM:
            DetectedDisc.fDiscKind = eBDROMDisc;
            break;

        case BD_R:
            DetectedDisc.fDiscKind = eBDRDisc;
            break;

        case BD_RE:
            DetectedDisc.fDiscKind = eBDREDisc;
            break;

        default:
            DetectedDisc.fDiscKind = eUnknownKind;
    }

    /*-------------------------------------------------------------------
       If this is a dual layer disc insure dual layer identifier
       is set in disc kind
    --------------------------------------------------------------------*/
    if(pdbGetBDLayers() == BD_DUAL_LAYER)
        DetectedDisc.Bit.Layers = eLayersDouble;

    DiscKind.fDiscKind = pdbGetDiscKind();

    /*-------------------------------------------------------------------
        Check for mis-detection
    --------------------------------------------------------------------*/
    if(DiscKind.Bit.Layers!=DetectedDisc.Bit.Layers) //only for layer detect error.
    {
        /*-------------------------------------------------------------------
           Misdetection. Servo was not able to correctly detect type of disc.
           Set the correct disc kind in the player DB and
           send a call for servo subsystem to readjust for new disc kind
        --------------------------------------------------------------------*/
        SendMsgCn(SEND_ATAPI_CMD,1,0x800400,DetectedDisc.fDiscKind);
        pdbServoSetfDiscKind(DetectedDisc.fDiscKind);
        pdbSetDiscKind(DetectedDisc.fDiscKind);

        TempB = ServoReAdjust(DetectedDisc);
        SendMsgCn(SEND_ATAPI_CMD,1,0x800401,TempB);
        return TempB;
    }
    else
        return e_NONE;
}
#endif

/*******************************************************************************

    FUNCTION        BdCheckVideo

    DESCRIPTION     Check if BD Video information exist

********************************************************************************/
void BdCheckVideo(void)
{
    BYTE result;

    result = SpecialDiscInformationParser(VideoBDInformationTable);

    if(result == eVideoBD)
        bdVideoFlag = TRUE;

    SendMsgCnB(SEND_READ_DVD_MSG,1,0x800473,B1B(bdVideoFlag)); //bdVideoFlag
}

/*****************************************************************************

    FUNCTION        BdDiscSetup

    DESCRIPTION     Manages the appropriate calls to gather disc information based
                    on the BD disc kind.

    PARAMETERS

    RETURNS         PASS    A BD disc kind is verified and all format information
                            has been gathered and stored. This status implies that
                            the disc is properly mounted and can be accessed

*****************************************************************************/
BYTE BdDiscSetup(void)
{
    eDiscKind disc;
    BYTE ReadStatus;
#if (ENABLE_AACS == 1)
    BYTE RomMarkReadFail;
#endif
    DiscAccessStatusE result;

    WRITE_REG(HAL_HSTREG_MAP->RXFIFOC, 0x08);//Set RXFIFO, RBSIZE 16 bytes
//start ==To judge BD BCA valid or not.
#if(JudgeBDBCAValid == 1)
    ReadBDBCAValid = FALSE;
#endif
//end ==To judge BD BCA valid or not.
    bdVideoFlag = FALSE;
    /* initial BD Speed Control before Start Up success */
    Disc_Speed.Read = MAX_READ_BD_SPEED;

    // Obtain the disc kind
    disc = pdbGetDiscKind();

#if (EN_POWER_CONTROL)
    PowerControl(eBDReadWblPower);     /* Initialize Chip to BD read wobble power */
#endif

    UpdateDiscInfoData();           /* Update general disc information  */

    pdbInitBDDataBase();

    ReadStatus = 0;
    result = GEN_FAIL;
    WRITE_FIELD(HAL_BDC_ADNOTAUN, 0); //Initial SD CFG, use AUN lock as default.
#if (HF_BASED_CUTOFF == 1)
    WRITE_FIELD(HAL_DMOD_RELINKCUT, 0);  //Initial relinking cut off base on demodulator
    WRITE_FIELD(HAL_DMOD_DISDMDFRC, 0);  //Initial don't force in run mode in runin area
#endif

#if (ENABLE_DISCARD_ICE == 1)
    HAL_BD_CODEC_Disable_DiscardIce();          /* Disable discard iced clusters */
#endif

    switch (disc)
    {
        /*----------------------------------------------
            BD ROM
        ---------------------------------------------- */
        case eBDROMDisc:
        case eBDROMDLDisc:
            if (BdReadPIC() == PASS)
            {
//add readjust layer flow for BD layer information mismatch between servo and hif.
#if (BD_LAYER_VERIFY == 1)
                ReadStatus = BdVerifyDisc();
                if(ReadStatus == e_READJUSTPASS)
                {
                    if (BdReadPIC() == FAIL)
                    {
                        return FAIL;
                    }
                }
                else if(ReadStatus == e_READJUSTFAIL)
                {
                    return FAIL;
                }

#endif
                #if (ENABLE_AACS == 1)
                RomMarkReadFail = TRUE;

                if (HasRomMarkIvIndicator == TRUE)
                {
                    if (BDReadVomueIDFromDisc() == PASS)
                    {
                        // Read Volume ID in Rom-Mark successfully
                        RomMarkReadFail = FALSE;

#if (CEProduct == 1)
                        if (BDReadVomueKCDFromDisc() == FAIL)
                            RomMarkReadFail = TRUE;
#endif  /* CEProduct == 1 */
                        SendMsgCnB(SEND_ATAPI_CMD,4,0x800402,B2B(READ_FIELD(HAL_DFE_BDRMSTREN)),B2B(READ_FIELD(HAL_DFE_BDRMDETTM)));

#if (ENABLE_AACS_BEF == 1)
                        // Calculate Read Data Key
#endif  /* ENABLE_AACS_BEF == 1 */
                    }
                }

                if (RomMarkReadFail == TRUE)
                {
                    AacsStatus.AacsFlag.IsMediumCompliant = FALSE;
                    if (IsSecureItemAvailable(AACS_LA_PUBLIC_KEY) == TRUE)
                    {
                        ClearAacsKey();
                        return FAIL;
                    }
                }
                else
                {
                #if (CEProduct == 0)
                    if (BDReadMkbFromDisc() == FAIL)
                    {
                        if (MkbVersionOnFlash == 0)
                        {
                            AacsStatus.AacsFlag.IsMediumCompliant = FALSE;
                            return FAIL;
                        }
                    }
                #endif  /* CEProduct == 0 */
                    AacsStatus.AacsFlag.IsMediumCompliant = TRUE;
                }
                #endif  /* ENABLE_AACS */

                // Get PAC information
                result = BdReadPAC();
                if (result == GEN_FAIL)
                {
                    send_msg5S(1,"ReadPAC FAIL");
                }

                // Update Session & Track information
                pdbSetBDFinalSessCloseInfo(eSessionFinalized);
                pdbSetBDLowestRecordedTrackNumber(1);
                pdbSetBDHighestRecordedTrackNumber(1);
                pdbSetBDFinalSessionNumber(1);
                pdbSetBDFinalSessFirstTrackNumber(1);
                pdbSetBDFinalSessLastTrackNumber(1);
                pdbSetBDTrackStartPSN(1, SWAPEND_ULONG(Info.Media.BD.DiscInfoP_L0_Fmt1->Disc.Bdrom.FAUOfDataZone));
                pdbSetBDTrackEndPSN(1, SWAPEND_ULONG(Info.Media.BD.DiscInfoP_L0_Fmt1->Disc.Bdrom.LastUserDataAddress));
//                pdbSetBDMaxPSN(0,SWAPEND_ULONG(Info.Media.BD.DiscInfoP_L0_Fmt1->Disc.Bdrom.LAUOfDataZone));
//                if(pdbGetDiscKind() == eBDROMDLDisc)
//                    pdbSetBDMaxPSN(1,SWAPEND_ULONG(Info.Media.BD.DiscInfoP_L1_Fmt1->Disc.Bdrom.LAUOfDataZone));
                pdbSetBDLogicalData();
            }
            else
                return (FAIL);

#if (EN_POWER_CONTROL)
            SetDiscReadPowerMode();     /* Initialize Chip to BD read only power */
#endif
            break;

        /*----------------------------------------------
            BD-R/RE
        ---------------------------------------------- */
        case eBDRDisc:
        case eBDRDLDisc:
        case eBDREDisc:
        case eBDREDLDisc:
    #if (ENABLE_BDRLTH == 1)
            BDCheckLTHDisc(); /*Check BDR LTH disc*/
    #endif

    #if (ENABLE_BDIH == 1)
            if ((disc == eBDREDisc)||(disc ==eBDREDLDisc))
            {
                //Call ReadBCA for BD-IH header check       2009.0925
                svoIramVar.IHDisclikely = 1;
                BDCheckIHDisc();
            }
    #endif

            /* Initial settings and variables for BD Writable Discs before reading PIC. */
            BdwtDisc.ReadDataMode = eNOT_USERDATA;
#if (ENABLE_BDRLTH == 1)
            if (svoIramVar.LTHDisc == 1)
                Defect_Enable(DEFECT_STATE_FREEZE_ALL_OFF);
#endif
            if (BdReadPIC() == FAIL)
            {
#if (ENABLE_BDRLTH == 1)
                if (svoIramVar.LTHDisc == 1)
                    Defect_Enable(DEFECT_STATE_UNFREEZE);
#endif
                return FAIL;
            }
#if (BD_LAYER_VERIFY == 1)
            else
            {
                ReadStatus = BdVerifyDisc();
                if(ReadStatus == e_READJUSTPASS)
                {
                    if (BdReadPIC() == FAIL)
                    {
    #if (ENABLE_BDRLTH == 1)
                        if (svoIramVar.LTHDisc == 1)
                            Defect_Enable(DEFECT_STATE_UNFREEZE);
    #endif
                        return FAIL;
                    }
                }
                else if(ReadStatus == e_READJUSTFAIL)
                {
    #if (ENABLE_BDRLTH == 1)
                    if (svoIramVar.LTHDisc == 1)
                        Defect_Enable(DEFECT_STATE_UNFREEZE);
    #endif
                    return FAIL;
                }
            }
#endif
            switch (pdbGetBDType())
            {
                case BD_ROM:
                    switch (pdbGetBDLayers())
                    {
                        case BD_SINGLE_LAYER:
                        default:
                            pdbSetDiscKind(eBDROMDisc);
                            WRITE_FIELD(HAL_GLOBAL_DL,halGLOBAL_SingleLayer);
                            break;

                        case BD_DUAL_LAYER:
                            pdbSetDiscKind(eBDROMDLDisc);
                            WRITE_FIELD(HAL_GLOBAL_DL,halGLOBAL_DualLayer);
                            break;
                    }
                    WRITE_FIELD(HAL_GLOBAL_DISCFMT, 0);
                    break;

                case BD_R:
                    switch (pdbGetBDLayers())
                    {
                        case BD_SINGLE_LAYER:
                        default:
                            pdbSetDiscKind(eBDRDisc);
                            WRITE_FIELD(HAL_GLOBAL_DL,halGLOBAL_SingleLayer);
                            break;

                        case BD_DUAL_LAYER:
                            pdbSetDiscKind(eBDRDLDisc);
                            WRITE_FIELD(HAL_GLOBAL_DL,halGLOBAL_DualLayer);
                            break;
                    }
                    WRITE_FIELD(HAL_GLOBAL_DISCFMT, 2);
                    break;

                case BD_RE:
                    switch (pdbGetBDLayers())
                    {
                        case BD_SINGLE_LAYER:
                        default:
                            pdbSetDiscKind(eBDREDisc);
                            WRITE_FIELD(HAL_GLOBAL_DL,halGLOBAL_SingleLayer);
                            break;

                        case BD_DUAL_LAYER:
                            pdbSetDiscKind(eBDREDLDisc);
                            WRITE_FIELD(HAL_GLOBAL_DL,halGLOBAL_DualLayer);
                            break;
                    }
                    WRITE_FIELD(HAL_GLOBAL_DISCFMT, 4);
                    break;

                default:
                    SendMsgCn(SEND_ATAPI_CMD,1,0x80040F,pdbGetBDType());
                    return FAIL;
            }
            SendMsgCn(SEND_ATAPI_CMD,2,0x800410,pdbGetBDType(),pdbGetDiscKind());
            //set coef again , according to BDR/RE SL/DL
            svoVar_discKindT.fDiscKind  = pdbGetDiscKind();
            LAL_StoreParams(LAL_DetectDiscKind(svoVar_discKindT),SVO_CAL_SPEED_BD);
            LAL_SetReadMode();

            //not update in current status until we finish checking all the BDR coef values
            //svoVar_discKindT.fDiscKind = pdbGetDiscKind();
            //LAL_StoreParams(LAL_DetectDiscKind(svoVar_discKindT), SVO_CAL_SPEED_BD);
            //LAL_SetReadMode();
            svo_bReadZoneSpeedX10 = 0;//force UpdateReadFilters()
            SetReadFilters();
            HAL_WOBBLE_SetPllOnHold(halWBL_WblPllNotInHold);
#if (ENABLE_BDRLTH == 1)
            if (svoIramVar.LTHDisc == 1)
                Defect_Enable(DEFECT_STATE_UNFREEZE);
#endif
            /* Initial settings and variables for BD Writable Discs after reading PIC. */
            WRITE_FIELD(HAL_BDC_ADNOTAUN, 1);  //wobble disc, use ADIP lock
//            WRITE_FIELD(HAL_DFE_DISHLDAGC, 0);  //prevent RFAGC/AOC mis action in blank area
//            WRITE_FIELD(HAL_DFE_DISHLDAOC, 0);


            if (pdbGetDiscKind() == eBDRDisc || pdbGetDiscKind() == eBDRDLDisc)
            {
                HAL_DFE_EnDisAgcAoc(halDFE_AgcAocDisable); /* Hold AGC on PIC signals*/
                HAL_DFE_EnDisBlankDetection(halDFE_BlankDetectEnable);/* Enable blank detection */
                result = BdReadDMA();

                if (result == NO_DMA)
                {
                    HAL_DFE_EnDisAgcAoc(halDFE_AgcAocDisable); /* Hold AGC on PIC signals*/
                    result = BdReadTDMA();
                    if (result == NO_TDMA)
                    {
                        //empty disc
                        //Fix the bug that wrong disc/track information
                        //with blank BD-R/RE discs are reported.

                        // Blank disc
                        BdCreateDefaultDMA();

                        pdbSetBDLowestRecordedTrackNumber(1);
                        pdbSetBDHighestRecordedTrackNumber(1);
                        pdbSetBDTrackStartPSN(1, BD_DATAZONE_START_PSN);
                        if (pdbGetBDLayers() == BD_DUAL_LAYER)
                            pdbSetBDTrackEndPSN(1, (pdbGetBDL1EndPAA() << 3)+0x0F);
                        else
                            pdbSetBDTrackEndPSN(1, (pdbGetBDL0EndPAA() << 3)+0x0F);

                        pdbSetBDTrackState(1, eTRACK_STATE_EMPTY);
                        pdbSetBDTrackStatus(1, eTS_BlankTrack);

                        pdbSetBDTrackSessionNumber(1, 1);
                        pdbSetBDFinalSessFirstTrackNumber(1);
                        pdbSetBDFinalSessLastTrackNumber(1);
                        pdbSetBDFinalSessionNumber(1);
                        pdbSetBDFinalSessCloseInfo(eEmptySession);
                        pdbSetBDLogicalData();
                    }
                    else if (result == BUF_GOOD)
                    {
                        /* Update incomplete track information */
                        BdUpdateIncompleteTracks();
                    }
                    else//GEN_FAIL
                    {
                        result = BdCreateDefaultDMA();
                        if(result == BUF_GOOD)
                        {
                            BdCreateDefaultSRRI();
                            if(UpdateDBFromSRRI() == FAIL)
                            {
                                send_msg80(SEND_SStpTask_MSG,0x7100A3);
                                return FAIL;
                            }
                             send_msg80(SEND_SStpTask_MSG,0x7100A5);
                        }
                    }
                }
                else if (result == BUF_GOOD)
                {
                    //finalized disc
                    pdbSetBDLogicalData();
                    pdbSetBDFinalSessCloseInfo(eSessionFinalized);
                }
                else if (result == GEN_FAIL)
                {
                    pdbSetBDFinalSessCloseInfo(eEmptySession);
                    return FAIL;
                }

                //BdReadPAC();
//                HAL_DFE_EnDisBlankDetection(halDFE_BlankDetectDisable);/* Disable blank detection */
            }
            else    /* BD-RE */
            {
#if (BDRE_BYPASS_LEADIN == 0)
                HAL_DFE_EnDisBlankDetection(halDFE_BlankDetectEnable);/* Enable blank detection */
                ReadStatus = BdReadDMA();
                if (ReadStatus == NO_DMA)
                {
                    //empty disc
                    //Fix the bug that wrong disc/track information
                    //with blank BD-R/RE discs are reported.

                    // Blank disc
                    BdCreateDefaultDMA();

                    pdbSetBDLowestRecordedTrackNumber(1);
                    pdbSetBDHighestRecordedTrackNumber(1);
                    pdbSetBDTrackStartPSN(1, BD_DATAZONE_START_PSN);
                    if (pdbGetBDLayers() == BD_DUAL_LAYER)
                        pdbSetBDTrackEndPSN(1, (pdbGetBDL1EndPAA() << 3)+0x0F);
                    else
                        pdbSetBDTrackEndPSN(1, (pdbGetBDL0EndPAA() << 3)+0x0F);

                    pdbSetBDTrackState(1, eTRACK_STATE_EMPTY);
                    pdbSetBDTrackStatus(1, eTS_BlankTrack);

                    pdbSetBDTrackSessionNumber(1, 1);
                    pdbSetBDFinalSessFirstTrackNumber(1);
                    pdbSetBDFinalSessLastTrackNumber(1);
                    pdbSetBDFinalSessionNumber(1);
                    pdbSetBDFinalSessCloseInfo(eEmptySession);
                    pdbSetBDLogicalData();
                }
                else if (ReadStatus == BUF_GOOD)
                {
                    //finalized disc
                    UpdateDBForBDRE();
                    pdbSetBDLogicalData();
                    SendMsgCn(SEND_ATAPI_CMD,4,0x800302,A4B(pdbGetBDEndLBALayer0()));
                }
                else if (ReadStatus == GEN_FAIL)
                    return FAIL;

                //BdReadPAC();
//                HAL_DFE_EnDisBlankDetection(halDFE_BlankDetectDisable);/* Disable blank detection */
#else
                BdCreateDefaultPIC();
                if(BdCreateDefaultDMA() != BUF_GOOD)
                    return FAIL;

                pdbSetBDFinalSessCloseInfo(eSessionFinalized);
#endif
            }
//            WRITE_FIELD(HAL_DFE_DISHLDAGC, 1);  //restore init value
//            WRITE_FIELD(HAL_DFE_DISHLDAOC, 1);

#if (HF_BASED_CUTOFF == 1)
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
#endif
            HAL_DFE_EnableICEDetect(); //enable Iced area detect
#if (ENABLE_DFE_ICE_DETECT == 1)
            HAL_DFE_SetIceDetectStAddr(0x20000L);
            HAL_DFE_ClusterStateEnableInt();
#if (ENABLE_DISCARD_ICE == 1)
//            HAL_BD_CODEC_Enable_DiscardIce();
#endif
#endif

#if (ENABLE_BDAV == 1)
            AacsStatus.AacsFlag.IsMediumCompliant = TRUE;
#endif

            break;

        default:
            return FAIL;
    }

//check BDMV disc
    if (pdbGetBDType() == BD_ROM)
    {
        BdCheckVideo();
    }
    else // BD writable discs
    {
        BdwtInitializeVar();
        bdSeek(0x120100,0);
        ServoPause();
    }

    if (St_door_open == TRUE || TrayManagerInfo.Executing)       //Startup time exceed 40sec, AP WinFinal send EjectDisc command
        return FAIL;

    /* Update blank disc status flag */
    St_blank_disc = pdbGetBDBlankDiscInfo();
    BdStartUpSuccessful();  /* Completes successful start up initializations */
    /*-------------------------------------------------------
        Set the speed mode
    --------------------------------------------------------*/
#if 0 //[W0Q] for A1 BD-ROM DL hang on issue
  if((pdbGetBDType() == BD_ROM) && (pdbGetBDMaxPSN(0) == 11811903) && (pdbGetBDMaxPSN(1) == 32505855))//Bios [V00P] for BD-ROM "Cloudy with a change of meatballs"
  {
    send_msg5S(1,"Set BD 4CAV");
    ServoSetSpeed(e4XDCAV);
  }
  else
#endif
    SetReadSpeed(eBDSpinUpSpeed);   //sync SVO speed

#if (USE_EEPROM_COLLI_POS == 1)
    SaveColliPosIntoEEPROM();
#endif

    return PASS;
}


/*****************************************************************************

    FUNCTION        InitializeBdSdramBuffer

    DESCRIPTION     Initialize internal memory variables and flags

    PARAMETERS

    RETURNS

*****************************************************************************/
void InitializeBdSdramBuffer(void)
{
    vBOOL temp_IE;

    temp_IE = GET_INT_ENABLE_MASK();
    DISABLE_INTERRUPT();

    if (pdbGetDiscKind() == eBDROMDisc || pdbGetDiscKind() == eBDROMDLDisc)
        BUF_TOTAL_BLK_COUNT = MAX_BDROM_SECTORS;
    else
        BUF_TOTAL_BLK_COUNT = MAX_BDWRITABLE_SECTORS;

    Iram_usiRingSP  = 0;
    Iram_usiRingEP  = BUF_TOTAL_BLK_COUNT - 1;

    HAL_BD_CODEC_InitializeBDSdramBuffer();     /* Specify the ranges of SDFD and FDHost buffers */

    WRITE_FIELD(HAL_HST_HSTBUFSPG, Iram_usiRingSP);         /* Host start blk. */
    WRITE_FIELD(HAL_HST_HSTBUFEPG, Iram_usiRingEP);         /* Host end blk. */

#if (ENABLE_AACS_BEF == 1)
    HAL_HOST_AesSetBEFBufferStart( ((ULONG)READ_FIELD(HAL_BDC_FDHSTBISS) << 11) + 80 );
    HAL_HOST_AesSetBEFBufferEnd( ((ULONG)READ_FIELD(HAL_BDC_FDHSTBISS) << 11) + (((ULONG)READ_FIELD(HAL_BDC_FDHSTCLRN)) << 16) + 17 * 31 + 80 );
    HAL_HOST_AesSetBEFBitOffset( halHST_BEFBitOffset_BD );
    HAL_HOST_AesSetBEFSecLength( halHST_BEFSecLen_BD );
    HAL_HOST_AesSetBEFBlkLength( halHST_BEFBlkLen_BD );
    HAL_HOST_AesSetBEFMaxSec( halHST_BEFMaxSecOffset_BD );
#endif  // ENABLE_AACS_BEF

    InitCopyStartEndPages();                    /* Initialize Copy block Start and End pages */

    SET_INT_ENABLE_MASK(temp_IE);
}
/*****************************************************************************

    FUNCTION        UpdateDBFromPIC

    DESCRIPTION     Save DI into bd database

    PARAMETERS      Data pointer that contains Disc Info

    RETURNS         PASS / FAIL

*****************************************************************************/
BOOL UpdateDBFromPIC(void)
{

    tBdDiscInformationS *temp;
    BYTE AunFrmCnt,DILayer,DIFmt,i;

    if(pdbGetBDType() == BD_ROM)
    {
        Info.Media.BD.DiscInfoP_L0_Fmt1 = (tBdDiscInformationS *) (BD_DI_ADR);
        if (pdbGetBDLayers() == BD_DUAL_LAYER)
        {
            Info.Media.BD.DiscInfoP_L1_Fmt1 = (tBdDiscInformationS *) (BD_DI_ADR + 64);
        }
    }
    else
    {
        temp = (tBdDiscInformationS *) (BD_DI_ADR);
        AunFrmCnt = (temp->NumberOfDIUnits)>>3; //bit7~bit3
        for(i=0;i<AunFrmCnt;i++)
        {
            temp = (tBdDiscInformationS *) (BD_DI_ADR+(i*DI_SIZE));
            if((temp->DIIdH == 'D') && (temp->DIIdL == 'I'))
            {
                DILayer = temp->NumberOfDIUnits & 0x03; //bit2~bit0
                DIFmt = temp->DIFormatNumber;
                if(DIFmt == 1)
                {
                    //format1
                    if(DILayer == 0)
                        Info.Media.BD.DiscInfoP_L0_Fmt1 = temp;
                     else
                        Info.Media.BD.DiscInfoP_L1_Fmt1 = temp;
                }
                else
                {
                    // format2
                    if(DILayer == 0)
                        Info.Media.BD.DiscInfoP_L0_Fmt2 = temp;
                     else
                        Info.Media.BD.DiscInfoP_L1_Fmt2 = temp;
                }
            }
            else
            {
                send_msg5S(1,"Update DI fail");
                return FAIL;
            }
        }
#if (ENABLE_BDIH ==1)
        Info.Media.BD.DiscInfoP_L1_Fmt2 = Info.Media.BD.DiscInfoP_L0_Fmt2; //woody.test
#endif
        SendMsgCn(SEND_ATAPI_CMD,8,0x800403,A4B((ULONG)Info.Media.BD.DiscInfoP_L0_Fmt1),A4B((ULONG)Info.Media.BD.DiscInfoP_L0_Fmt2));
        SendMsgCn(SEND_ATAPI_CMD,8,0x800404,A4B((ULONG)Info.Media.BD.DiscInfoP_L1_Fmt1),A4B((ULONG)Info.Media.BD.DiscInfoP_L1_Fmt2));
    }
    return PASS;
}

/*****************************************************************************

    FUNCTION        UpdateDBFromSRRI

    DESCRIPTION     Save SRRI Info into bd database

    PARAMETERS      Data pointer that contains SRRI Info

    RETURNS         PASS / FAIL

*****************************************************************************/
BOOL UpdateDBFromSRRI(void)
{
    ULONG StartPsn, Addr;
    USHORT SRRI_Cnt, Tno, idx;
    BYTE Sno, i, IsOpen;

    Sno = 0;
    Tno = 0;

    bdConvert_init();

    SRRI_Cnt = (USHORT)(SWAPEND_ULONG(pSRRI->SRRI_Cnt));

    SendMsgCn(SEND_ATAPI_CMD,3,0x800405,A2B(SRRI_Cnt),pSRRI->Open_Cnt);

    if (SRRI_Cnt == 0)
    {
        send_msg80(SEND_SStpTask_MSG,0x7100D3);  //Err! No SRRI Entry
        return FAIL;
    }

    if (pSRRI->Open_Cnt > 16)
    {
        send_msg80(SEND_SStpTask_MSG,0x7100D4);  //Err! Value of Open Count > 16
        return FAIL;
    }

    if (pdbGetSRRIEntrySessStart(0) != 1)
    {
        send_msg80(SEND_SStpTask_MSG,0x7100D5);  //Err! SRR #1 isn't the start of the session
        return FAIL;
    }

    /* Scan all tracks listed in the Srri */
    for (idx = 0; idx < SRRI_Cnt; idx++)
    {
        Tno++;
        if (pdbGetSRRIEntrySessStart(idx))
        {
            Sno++;
            pdbSetBDFinalSessFirstTrackNumber(Tno);
        }

        StartPsn = pdbGetSRRIEntryStartPSN(idx);
        if (StartPsn == NULL_VALUE)
        {
            SendMsgCn(SEND_ATAPI_CMD,2,0x800406,A2B(idx));
            return FAIL;
        }

        SendMsgCn(SEND_ATAPI_CMD,2,0x800407,A2B(idx));

        /* Set Track start PSN */
        pdbSetBDTrackStartPSN(Tno, StartPsn);
        if (pdbGetSRRIEntrySessStart(idx))
            pdbSetBDFinalSessFirstTrackPSN(StartPsn);

        if (StartPsn > pdbGetBDMaxPSN(0))
            pdbSetBDTrackStartPSNLayer(Tno, eLayer1);
        else
            pdbSetBDTrackStartPSNLayer(Tno, eLayer0);

        /* Set recorded PSN */
        Addr = pdbGetSRRIEntryLRA(idx);

        if (Addr == 0)
            Addr = StartPsn;
        else
            Addr = (Addr / ONE_BD_CLUSTER + 1) * ONE_BD_CLUSTER;

        pdbSetBDTrackNextWritableAddress(Tno, bdConvertPSNtoLBA(Addr));
        pdbSetBDTrackRecordedPSN(Tno, Addr - 1);

        if (Tno > 1)
        {
            if ((pdbGetBDLayers() == BD_DUAL_LAYER)&&(StartPsn == pdbGetBDStartPSN(1)))
                pdbSetBDTrackEndPSN(Tno - 1, pdbGetBDMaxPSN(0));     /* set previous track end PSN = Max PSN Layer 0 */
            else
                pdbSetBDTrackEndPSN(Tno - 1, StartPsn - 1);     /* set previous track end PSN = current track start - 1 */

            if((StartPsn - 1) > pdbGetBDMaxPSN(0))
                pdbSetBDTrackEndPSNLayer(Tno - 1, eLayer1);
            else
                pdbSetBDTrackEndPSNLayer(Tno - 1, eLayer0);
        }

        /* Check if the track is already closed */
        IsOpen = FALSE;
        for (i = 0; i < pSRRI->Open_Cnt; i++)
        {
            if (Tno == SWAPEND_UWORD(pSRRI->OpenSRRNum[i]))
            {   /* Open Track */
                IsOpen = TRUE;
                break;
            }
        }

        if (IsOpen == TRUE)
        {   /* Open Track */
            IsOpen =  FALSE;

            SendMsgCn(SEND_ATAPI_CMD,2,0x80040D,A2B(Tno));

            /* update track state */
            if (Tno == SRRI_Cnt)
                pdbSetBDTrackStatus(Tno, eTS_InvisibleTrack | eTS_FixedPacket);
            else
                pdbSetBDTrackStatus(Tno, eTS_ReservedTrack | eTS_FixedPacket);
            pdbSetBDTrackState(Tno, eTRACK_STATE_INCOMPLETE);
            pdbSetBDTrackDiagStatus2(Tno, (eTS_NWA_done | eTS_NWA_valid));
        }
        else
        {   /* Closed Track */
            SendMsgCn(SEND_ATAPI_CMD,2,0x80040E,A2B(Tno));

            /* update track state */
            pdbSetBDTrackStatus(Tno, eTS_ReservedTrack | eTS_FixedPacket);
            pdbSetBDTrackState(Tno, eTRACK_STATE_COMPLETE);
            pdbSetBDTrackDiagStatus(Tno, (eTS_Diag_All_done | eTS_Diag_All_valid));
            pdbSetBDTrackDiagStatus2(Tno, eTS_NWA_done);
            pdbSetBDTrackNextWritableAddress(Tno, 0xFFFFFFFF);
        }
        pdbSetBDTrackSessionNumber(Tno, Sno);
    }

    /* set last track end = lead out PSN - 1 */
    if (pdbGetBDLayers() == BD_DUAL_LAYER)
    {
        if ((pdbGetBDRecMode() == SRM_NO_LOW) && (pdbGetBDFinalSessCloseInfo() == eSessionFinalized))
            pdbSetBDTrackEndPSN(Tno,pdbGetBDTrackRecordedPSN(Tno));
        else
            pdbSetBDTrackEndPSN(Tno, pdbGetBDMaxPSN(1));

        pdbSetBDTrackEndPSNLayer(Tno, eLayer1);
    }
    else
    {
        if ((pdbGetBDRecMode() == SRM_NO_LOW) && (pdbGetBDFinalSessCloseInfo() == eSessionFinalized) )
            pdbSetBDTrackEndPSN(Tno,pdbGetBDTrackRecordedPSN(Tno));
        else
            pdbSetBDTrackEndPSN(Tno, pdbGetBDMaxPSN(0));

        pdbSetBDTrackEndPSNLayer(Tno, eLayer0);
    }

    pdbSetBDLowestRecordedTrackNumber(1);
    pdbSetBDFinalSessionNumber(Sno);
    pdbSetBDFinalSessLastTrackNumber(Tno);
    pdbSetBDHighestRecordedTrackNumber(Tno);

    // If there are no open tracks exist and DMA is empty,
    // update the close info of the final session right here.
    if (pSRRI->Open_Cnt == 0 && pdbGetBDFinalSessCloseInfo() == eEmptySession)
    {
        if (pdbGetBDTrackState(pdbGetBDFinalSessFirstTrackNumber()) == eTRACK_STATE_COMPLETE)
            pdbSetBDFinalSessCloseInfo(eSessionClosed);
    }

    return PASS;
}


/*****************************************************************************

    FUNCTION        UpdateDBForBDRE

    DESCRIPTION     Save Track Info into bd database.
                    BD-RE is always one session/one track finalized disc.

    PARAMETERS      None

    RETURNS         None

*****************************************************************************/
void UpdateDBForBDRE(void)
{
    pdbSetBDFinalSessCloseInfo(eSessionFinalized);
    pdbSetBDLowestRecordedTrackNumber(1);
    pdbSetBDHighestRecordedTrackNumber(1);
    pdbSetBDFinalSessionNumber(1);
    pdbSetBDFinalSessFirstTrackNumber(1);
    pdbSetBDFinalSessLastTrackNumber(1);

    /* Set Track start PSN */
    pdbSetBDTrackStartPSN(1, pdbGetBDLsn0StartPSN());
    pdbSetBDFinalSessFirstTrackPSN(pdbGetBDLsn0StartPSN());

    if (pdbGetBDLayers() == BD_DUAL_LAYER)
    {
        pdbSetBDTrackRecordedPSN(1, pdbGetBDMaxPSN(1));
        pdbSetBDTrackEndPSN(1, pdbGetBDMaxPSN(1));
    }
    else
    {
        pdbSetBDTrackRecordedPSN(1, pdbGetBDMaxPSN(0));
        pdbSetBDTrackEndPSN(1, pdbGetBDMaxPSN(0));
    }

    pdbSetBDTrackSessionNumber(1, 1);

    pdbSetBDTrackStatus(1, eTS_FixedPacket);
    pdbSetBDTrackState(1 , eTRACK_STATE_COMPLETE);
    pdbSetBDTrackDiagStatus(1 , (eTS_Diag_All_done | eTS_Diag_All_valid));
    pdbSetBDTrackDiagStatus2(1, eTS_NWA_done);

    pdbSetBDFinalSessLeadoutPSN(pdbGetBDLeadOutStartPSN());
}


/*****************************************************************************

    FUNCTION        CheckHeader

    DESCRIPTION     Check Header for different types

    PARAMETERS      Data start pointer, Header type

    RETURNS         Buffer page that contains the header

*****************************************************************************/
USHORT CheckHeader(BYTE * DataPtr, HeaderTypeE  * HeaderType)
{
    BYTE *ptr;
    BYTE pg_no, found;

    found = 0;

    /* check for header */
    for(pg_no = 0; pg_no < ONE_BD_CLUSTER && !found; pg_no++)
    {
        ptr = (BYTE *) (DataPtr + pg_no * BD_SECTOR_SIZE);

        switch(*HeaderType)
        {
            case eDI:
                if(*(ptr) == 'D' && *(ptr + 1) == 'I')
                    found = 1;
                break;

            case ePRM:
                if(*(ptr) == 'P' && *(ptr + 1) == 'R' && *(ptr + 2) == 'M')
                    found = 1;
                break;

            case eSRRI:
                if(*(ptr) == 'S' && *(ptr + 1) == 'R')
                    found = 1;
                break;

            case eDDS:
                if(*(ptr) == 'D' && *(ptr + 1) == 'S')
                    found = 1;
                break;

            case eDFL:
                if(*(ptr) == 'D' && *(ptr + 1) == 'L')
                    found = 1;
                break;

            case eDWP:
                if(*(ptr) == 'D' && *(ptr + 1) == 'W' && *(ptr + 2) == 'P')
                    found = 1;
                break;
        }
    }

    if(found == 1)
        return (pg_no + ((ULONG) DataPtr - SCSDRAM_ADDRESS)  / BD_SECTOR_SIZE - 1);
    else
    {
        *HeaderType = eHeader_None;
        return 0xFFFF;
    }
}


/*****************************************************************************

    FUNCTION        BdReadPIC

    DESCRIPTION     Read PIC area

    PARAMETERS      None

    RETURNS         PASS / FAIL

*****************************************************************************/
BOOL BdReadPIC(void)
{
    BYTE *DataPtr;
    ULONG BDERCCFG1bak;
    USHORT pg_offset;
    BYTE i, Valid_Info;
    DiscAccessStatusE result;
    HeaderTypeE Header;

#if (ENABLE_AACS)
    BYTE RomMarkFlag;
#endif

    send_msg80(SEND_SStpTask_MSG,0x7100CD);  //Read PIC Start

    /*---------------------------------------------------------------
        Stop buffering if on and clear the buffer
    ----------------------------------------------------------------*/
    if ( BD_BUFFERING() == ON )
        bdStopDiscTransfer();

    Valid_Info = FALSE;

    if((pdbGetDiscKind() != eBDROMDisc) && (pdbGetDiscKind() != eBDROMDLDisc))
    {
        BDERCCFG1bak = READ_REG(HAL_BDCREG_MAP->BDERCCFG1);
    }

    bdDecFlag.fREADING_LEADIN = TRUE;

    for (i = BD_PIC_IF_NUMBER; i > 0; i--)
    {
        if(EjectKeyInfo.PressedValid || TrayManagerInfo.Executing)
        {
            //BUILD_SENSE(0x02,0x3A,0x00,0x05);            /* MEDIUM NOT PRESENT */
            result = GEN_FAIL;
            break;
        }
        /*---------------------------------------------------------------
            Set up the Disc Transfer control parameters
        ----------------------------------------------------------------*/
        ClearDataBuffer();
        bdDiscRequest.Type            = INIT_READ;
        bdDiscRequest.TargetLayer     = eLayer0;
        if (pdbGetDiscKind() == eBDROMDisc || pdbGetDiscKind() == eBDROMDLDisc)
        {
            bdDiscRequest.TargetID.Long = BDROM_PIC_Info[i-1].PSN;
            bdDiscRequest.ClusterCount  = BDROM_PIC_Info[i-1].LEN;
            bdDecFlag.fREADING_PIC = SET;
            result = bdStartDiscTransfer();
            bdDecFlag.fREADING_PIC = CLEAR;
        }
        else
        {
            bdDiscRequest.TargetID.Long = BDWRITABLE_PIC_Info[i-1].PSN;
            bdDiscRequest.ClusterCount  = BDWRITABLE_PIC_Info[i-1].LEN;
            bdDecFlag.fREADING_PIC = SET;
            //HAL_WOBBLE_SetResetPIC(eSET_PIC);
            result = bdStartDiscTransfer();
            //HAL_WOBBLE_SetResetPIC(eRESET_PIC);
            //bdDecFlag.fREADING_PIC = CLEAR;
        }

        if (result == BUF_GOOD)
        {
            DataPtr = (BYTE*) SCSDRAM_ADDRESS;
            /* check for DI header */
            Header = eDI;
            pg_offset = CheckHeader(DataPtr, &Header);

            //copy data info to DI area in SDRAM
            if(Header == eDI)
            {
                CopyBufferToBuffer(pg_offset, 0, BD_DI_PAGE, 0, 2);
                Valid_Info = TRUE;
                bdDecFlag.fREADING_PIC = CLEAR;

                #if (ENABLE_BDIH == 1)
                    //Header check in DI for Layer adjudgment       2009.0925
                    if(*(BYTE*)(SCSDRAM_ADDRESS+8) == 'B' && *(BYTE*)(SCSDRAM_ADDRESS+9) == 'D' && *(BYTE*)(SCSDRAM_ADDRESS+10) == 'w')
                    {
                        send_msg5S(SHOW_DEBUG_MSG,"BDIH-BDRE");
                    }
                    else if(*(BYTE*)(SCSDRAM_ADDRESS+8) == 'B' && *(BYTE*)(SCSDRAM_ADDRESS+9) == 'D' && *(BYTE*)(SCSDRAM_ADDRESS+10) == 'o')
                    {
                        send_msg5S(SHOW_DEBUG_MSG,"BDIH-BDROM");
                    }
                #endif

                break;
            }
        }
    }

    Defect_Enable(DEFECT_SOURCE_UNFREEZE);
    bdDecFlag.fREADING_LEADIN = FALSE;
    bdDecFlag.fREADING_PIC = CLEAR;

    if((pdbGetDiscKind() != eBDROMDisc) && (pdbGetDiscKind() != eBDROMDLDisc))
    {
        WRITE_REG(HAL_BDCREG_MAP->BDERCCFG1, BDERCCFG1bak);
    }

    if (result != BUF_GOOD || !Valid_Info)
    {
        send_msg80(SEND_SStpTask_MSG,0x7100CF);  //Read PIC End, NG
        return FAIL;
    }

    send_msg80(SEND_SStpTask_MSG,0x7100CE);  //Read PIC End, OK

    // Point DiscInfoP_L0_Fmt1/Fmt2 or DiscInfoP_L1_Fmt1/Fmt2 to DI page
    if(UpdateDBFromPIC() == FAIL)
    {
        send_msg80(SEND_SStpTask_MSG,0x7100D0);  //Save DI Err
        return GEN_FAIL;
    }

#if (ENABLE_AACS)
    if (pdbGetBDType() == BD_ROM)
    {
        // Check if the ROM-Mark IV Indicator has been set
        DataPtr = (BYTE *)(SCSDRAM_ADDRESS + BDROM_PIC_CPS_SECTOR_OFFSET * BD_SECTOR_SIZE);
        RomMarkFlag = *DataPtr;
        if ((RomMarkFlag & BD_ROM_MARK_MASK) == 0x80)
        {
            send_msg80(SEND_SStpTask_MSG,0x7100D1);  //Rom Mark Exist
            HasRomMarkIvIndicator = TRUE;
        }
        else
        {
            send_msg80(SEND_SStpTask_MSG,0x7100D2);  //No Rom Mark
            HasRomMarkIvIndicator = FALSE;
        }
    }
#endif  /* ENABLE_AACS */


    return PASS;
}

#if (ENABLE_AACS == 1)
/*****************************************************************************

    FUNCTION        BDReadVomueIDFromDisc

    DESCRIPTION     Read Volume ID from the ROM-Mark

    PARAMETERS      None

    RETURNS         PASS / FAIL

*****************************************************************************/
BOOL BDReadVomueIDFromDisc(void)
{
    ULONG   clock, delay3, WOBAPC15_S, WOBAPC19_S, WOBAPC21_S, WOBAPC23_S, WOBAPC25_S;
    BYTE    i, j, k, temp, nRetries, seek_retry;

    send_msg80(SEND_SStpTask_MSG,0x7100CA);  //Read Volume ID Start

    WOBAPC15_S = READ_REG(HAL_AFEREG_MAP->WOBAPC15);
    WOBAPC19_S = READ_REG(HAL_AFEREG_MAP->WOBAPC19);
    WOBAPC21_S = READ_REG(HAL_AFEREG_MAP->WOBAPC21);
    WOBAPC23_S = READ_REG(HAL_AFEREG_MAP->WOBAPC23);
    WOBAPC25_S = READ_REG(HAL_AFEREG_MAP->WOBAPC25);

    LAL_WriteCoef(AFE_WOBAPC15_BDROMMARK);
    LAL_WriteCoef(AFE_WOBAPC19_BDROMMARK);
    LAL_WriteCoef(AFE_WOBAPC21_BDROMMARK);
    LAL_WriteCoef(AFE_WOBAPC23_BDROMMARK);
    LAL_WriteCoef(AFE_WOBAPC25_BDROMMARK);

    /* Setup ROM-Mark decoder */
    ConfigureRomMarkDecoder(0);

#if (ENABLE_DFE_ICE_DETECT == 0)
    ENABLE_DFEINT();
#endif

    i = 0;
    nRetries = 0;
    seek_retry = 0;

    while( i < BD_PIC_IF_NUMBER && seek_retry < BD_PIC_IF_NUMBER)
    {
        if (bdSeek(BDROM_PIC_Info[i].PSN , eLayer0) == Ready)      // Jump to the start of each PIC IF successfully
        {
            delay3 = 0;
            //take 2^10 samples
            for (j = 0; j < 8; j++)
            {
                for (k = 0; k < 128; k++)
                {
                    delay3 += READ_FIELD(HAL_DFE_INTEGRAT);
                }
            }
            delay3 = delay3 >> (10 - 7);
            delay3 = 128 + ((ULONG)3210 * (TWO_TO_POWER_13 << 7)) / delay3;
            HAL_DFE_RomMarkSetDelay( (USHORT)delay3);

            StartTimer(&clock);
            while(1)
            {
                if (fRomMarkInfoGet == TRUE)
                {
                    fRomMarkInfoGet = FALSE;

                    // Step 1 : store detected rom mark key into AacsAuth.ID.VolumeID
                    for (j = 0; j < 4; j++)
                    {
                        AacsAuth.ID.VolumeID[j*4 + 0] = (BYTE)(RomMarkKeyBuffer[j] >> 24);
                        AacsAuth.ID.VolumeID[j*4 + 1] = (BYTE)(RomMarkKeyBuffer[j] >> 16);
                        AacsAuth.ID.VolumeID[j*4 + 2] = (BYTE)(RomMarkKeyBuffer[j] >> 8);
                        AacsAuth.ID.VolumeID[j*4 + 3] = (BYTE)(RomMarkKeyBuffer[j]);
                    }

                    // Step 2 : reverse all bytes
                    for (j = 0; j < 8; j++)
                    {
                        temp = AacsAuth.ID.VolumeID[j];
                        AacsAuth.ID.VolumeID[j] = AacsAuth.ID.VolumeID[15-j];
                        AacsAuth.ID.VolumeID[15-j] = temp;
                    }

                    // Step 3 : reverse all bits per byte
                    for (j = 0; j < 16; j++)
                    {
                        temp = 0;
                        for (k = 0; k < 8; k++)
                            temp |= (BYTE)((AacsAuth.ID.VolumeID[j] >> k & 0x01) << (7 - k));

                        AacsAuth.ID.VolumeID[j] = temp;
                        //send_msg5SValue(SEND_SStpTask_MSG, "BDROMRKEY", 1,  AacsAuth.ID.VolumeID[j]);
                    }

                    send_msg80(SEND_SStpTask_MSG,0x7100CB);  //Read Volume ID End, OK
//                    send_msg5L(SEND_SStpTask_MSG, ReadTimer(&clock));
//                    send_msg5L(SEND_SStpTask_MSG, 0x00500);

                    DisableRomMarkDecoder();

#if (ENABLE_DFE_ICE_DETECT == 0)
                    DISABLE_DFEINT();
#endif
                    WRITE_REG(HAL_AFEREG_MAP->WOBAPC15, WOBAPC15_S);
                    WRITE_REG(HAL_AFEREG_MAP->WOBAPC19, WOBAPC19_S);
                    WRITE_REG(HAL_AFEREG_MAP->WOBAPC21, WOBAPC21_S);
                    WRITE_REG(HAL_AFEREG_MAP->WOBAPC23, WOBAPC23_S);
                    WRITE_REG(HAL_AFEREG_MAP->WOBAPC25, WOBAPC25_S);

                    return PASS;
                }
                else if (ReadTimer(&clock) > 500)     // X ms is expired
                {
                    if (nRetries < 3)
                    {
                        nRetries++;
                    }
                    else
                    {
                        i += 2;
                        nRetries = 0;
                        send_msg80(SEND_SStpTask_MSG,0x7100C9);  //Next Window
                    }

                    break;
                }
                ExitProcess();
            }
        }
        else
        {
            seek_retry++;
        }
    }

    DisableRomMarkDecoder();

#if (ENABLE_DFE_ICE_DETECT == 0)
    DISABLE_DFEINT();
#endif

    send_msg80(SEND_SStpTask_MSG,0x7100CC);  //Read Volume ID End, NG

    WRITE_REG(HAL_AFEREG_MAP->WOBAPC15, WOBAPC15_S);
    WRITE_REG(HAL_AFEREG_MAP->WOBAPC19, WOBAPC19_S);
    WRITE_REG(HAL_AFEREG_MAP->WOBAPC21, WOBAPC21_S);
    WRITE_REG(HAL_AFEREG_MAP->WOBAPC23, WOBAPC23_S);
    WRITE_REG(HAL_AFEREG_MAP->WOBAPC25, WOBAPC25_S);

    return FAIL;
}

#if (CEProduct == 1)
/*****************************************************************************

    FUNCTION        BDReadVomueKCDFromDisc

    DESCRIPTION     Read KCD(Key Conversion Data) from the ROM-Mark

    PARAMETERS      None

    RETURNS         PASS / FAIL

*****************************************************************************/
BOOL BDReadVomueKCDFromDisc(void)
{
    ULONG   clock, delay3, WOBAPC15_S, WOBAPC19_S, WOBAPC21_S, WOBAPC23_S, WOBAPC25_S;
    BYTE    i, j, k, temp, nRetries, seek_retry;


    send_msg80(SEND_SStpTask_MSG,0x7100C6);  //Read KCD Start

    WOBAPC15_S = READ_REG(HAL_AFEREG_MAP->WOBAPC15);
    WOBAPC19_S = READ_REG(HAL_AFEREG_MAP->WOBAPC19);
    WOBAPC21_S = READ_REG(HAL_AFEREG_MAP->WOBAPC21);
    WOBAPC23_S = READ_REG(HAL_AFEREG_MAP->WOBAPC23);
    WOBAPC25_S = READ_REG(HAL_AFEREG_MAP->WOBAPC25);

    LAL_WriteCoef(AFE_WOBAPC15_BDROMMARK);
    LAL_WriteCoef(AFE_WOBAPC19_BDROMMARK);
    LAL_WriteCoef(AFE_WOBAPC21_BDROMMARK);
    LAL_WriteCoef(AFE_WOBAPC23_BDROMMARK);
    LAL_WriteCoef(AFE_WOBAPC25_BDROMMARK);

    /* Setup ROM-Mark decoder */
    ConfigureRomMarkDecoder(0xC3F1B451);

#if (ENABLE_DFE_ICE_DETECT == 0)
    ENABLE_DFEINT();
#endif

    i = 1;
    nRetries = 0;
    seek_retry = 0;

    while( i < BD_PIC_IF_NUMBER && seek_retry < BD_PIC_IF_NUMBER)
    {
        if (bdSeek(BDROM_PIC_Info[i].PSN , eLayer0) == Ready)      // Jump to the start of each PIC IF successfully
        {
            delay3 = 0;
            //take 2^10 samples
            for (j = 0; j < 8; j++)
            {
                for (k = 0; k < 128; k++)
                {
                    delay3 += READ_FIELD(HAL_DFE_INTEGRAT);
                }
            }
            delay3 = delay3 >> (10 - 7);
            delay3 = 128 + ((ULONG)3210 * (TWO_TO_POWER_13 << 7)) / delay3;
            HAL_DFE_RomMarkSetDelay( (USHORT)delay3);

            StartTimer(&clock);
            while(1)
            {
                if (fRomMarkInfoGet == TRUE)
                {
                    fRomMarkInfoGet = FALSE;

                    // Step 1 : store detected rom mark key into AacsAuth.ID.VolumeID
                    for (j = 0; j < 4; j++)
                    {
                        AacsAuth.KCD[j*4 + 0] = (BYTE)(RomMarkKeyBuffer[j] >> 24);
                        AacsAuth.KCD[j*4 + 1] = (BYTE)(RomMarkKeyBuffer[j] >> 16);
                        AacsAuth.KCD[j*4 + 2] = (BYTE)(RomMarkKeyBuffer[j] >> 8);
                        AacsAuth.KCD[j*4 + 3] = (BYTE)(RomMarkKeyBuffer[j]);
                    }

                    // Step 2 : reverse all bytes
                    for (j = 0; j < 8; j++)
                    {
                        temp = AacsAuth.KCD[j];
                        AacsAuth.KCD[j] = AacsAuth.KCD[15-j];
                        AacsAuth.KCD[15-j] = temp;
                    }

                    // Step 3 : reverse all bits per byte
                    for (j = 0; j < 16; j++)
                    {
                        temp = 0;
                        for (k = 0; k < 8; k++)
                            temp |= (BYTE)((AacsAuth.KCD[j] >> k & 0x01) << (7 - k));

                        AacsAuth.KCD[j] = temp;
                        //send_msg5SValue(SEND_SStpTask_MSG, "KCD", 1,  AacsAuth.KCD[j]);

                    }

                    send_msg80(SEND_SStpTask_MSG,0x7100C7);  //Read KCD End, OK
//                    send_msg5L(SEND_SStpTask_MSG, ReadTimer(&clock));
//                    send_msg5L(SEND_SStpTask_MSG, 0x00500);

                    DisableRomMarkDecoder();

#if (ENABLE_DFE_ICE_DETECT == 0)
                    DISABLE_DFEINT();
#endif
                    WRITE_REG(HAL_AFEREG_MAP->WOBAPC15, WOBAPC15_S);
                    WRITE_REG(HAL_AFEREG_MAP->WOBAPC19, WOBAPC19_S);
                    WRITE_REG(HAL_AFEREG_MAP->WOBAPC21, WOBAPC21_S);
                    WRITE_REG(HAL_AFEREG_MAP->WOBAPC23, WOBAPC23_S);
                    WRITE_REG(HAL_AFEREG_MAP->WOBAPC25, WOBAPC25_S);

                    return PASS;
                }
                else if (ReadTimer(&clock) > 500)     // X ms is expired
                {
                    if (nRetries < 3)
                    {
                        nRetries++;
                    }
                    else
                    {
                        i += 2;
                        nRetries = 0;
                        send_msg80(SEND_SStpTask_MSG,0x7100C9);  //Next Window
                    }

                    break;
                }
                ExitProcess();
            }
        }
        else
        {
            seek_retry++;
        }
    }

    DisableRomMarkDecoder();

#if (ENABLE_DFE_ICE_DETECT == 0)
    DISABLE_DFEINT();
#endif

    send_msg80(SEND_SStpTask_MSG,0x7100C8);  //Read KCD End, NG

    WRITE_REG(HAL_AFEREG_MAP->WOBAPC15, WOBAPC15_S);
    WRITE_REG(HAL_AFEREG_MAP->WOBAPC19, WOBAPC19_S);
    WRITE_REG(HAL_AFEREG_MAP->WOBAPC21, WOBAPC21_S);
    WRITE_REG(HAL_AFEREG_MAP->WOBAPC23, WOBAPC23_S);
    WRITE_REG(HAL_AFEREG_MAP->WOBAPC25, WOBAPC25_S);

    return FAIL;
}
#endif  /* CEProduct == 1 */

/*****************************************************************************

    FUNCTION        BDReadMkbFromDisc

    DESCRIPTION     Read MKB

    PARAMETERS      None

    RETURNS         PASS / FAIL

*****************************************************************************/
BOOL BDReadMkbFromDisc(void)
{
    BYTE i, MkbCount;
    BYTE *DataPtr;
    vBYTE *FlashAddr;
    DiscAccessStatusE result;

    send_msg80(SEND_SStpTask_MSG,0x7100C3);  //Read MKB Start
    /*---------------------------------------------------------------
        Stop buffering if on and clear the buffer
    ----------------------------------------------------------------*/
    if ( BD_BUFFERING() == ON )
        bdStopDiscTransfer();

    FlashAddr = (vBYTE *)FLASH_BASE + MKB_BLK_START_OFFSET + 8;
    MkbVersionOnFlash = SWAPEND_ULONG(*((ULONG *)FlashAddr));

    /*---------------------------------------------------------------
        For BD_ROM, MKBs are located on the second cluster of each
        PIC IF. For BD-R/RE, MKBs are located on the first cluster
        of INFO2/Reserved 8 and INFO2/Reserved 5. So there are
        different cluster numbers where MKBs are stored between
        BD-ROM and BD-R/RE.
    ----------------------------------------------------------------*/
    if (pdbGetBDType() == BD_ROM)
        MkbCount = sizeof(BDROM_MKB_Info)/sizeof(MKBInfoAddrS);
    else
        MkbCount = sizeof(BDWRITABLE_MKB_Info)/sizeof(MKBInfoAddrS);

    bdDecFlag.fREADING_LEADIN = TRUE;

    for (i = 0; i < MkbCount; i++)
    {
        ClearDataBuffer();
        bdDiscRequest.Type              = INIT_READ;
        bdDiscRequest.TargetLayer       = eLayer0;
        if (pdbGetBDType() == BD_ROM)
        {
            bdDiscRequest.TargetID.Long = BDROM_MKB_Info[i].PSN;
            bdDiscRequest.ClusterCount  = BDROM_MKB_Info[i].LEN;
        }
        else
        {
            bdDiscRequest.TargetID.Long = BDWRITABLE_MKB_Info[i].PSN;
            bdDiscRequest.ClusterCount  = BDWRITABLE_MKB_Info[i].LEN;
        }

        result = bdStartDiscTransfer();
        if (result == BUF_GOOD)
            break;
    }

    bdDecFlag.fREADING_LEADIN = FALSE;

    if (result == BUF_GOOD)
    {
        DataPtr = (BYTE *)(SCSDRAM_ADDRESS);

        // Validate Type and Version Record
        if (ValidateTypeAndVersionRecord(DataPtr) == TRUE)
        {
            DataPtr = (BYTE *)(SCSDRAM_ADDRESS + HOST_REVOCATION_LIST_RECORD_OFFSET);

            // Validate Host Revocation List Record
            if (ValidateHrlRecord(DataPtr) == TRUE)
            {
                AacsStatus.AacsFlag.IsHrlValid = TRUE;

                DataPtr = (BYTE *)(SCSDRAM_ADDRESS + 8);    // Point to the Version Number field
                MkbVersionOnDisc = SWAPEND_ULONG(*((ULONG *)DataPtr));

                SendMsgCn(SEND_ATAPI_CMD,8,0x800412,A4B(MkbVersionOnFlash),A4B(MkbVersionOnDisc));

                if (MkbVersionOnDisc > MkbVersionOnFlash)
                {
                    MkbVersionOnFlash = MkbVersionOnDisc;
                    CopyBufferToBuffer(0, 0, 32, 0, 32);
                    CloseWritableSegmentTable(eMKB_Table);
                }
            }
        }

        send_msg80(SEND_SStpTask_MSG,0x7100C4);  //Read MKB End, OK
        return PASS;
    }
    else
    {
        send_msg80(SEND_SStpTask_MSG,0x7100C5);  //Read MKB End, NG
        return FAIL;
    }
}

/*****************************************************************************

    FUNCTION        ConfigureRomMarkDecoder

    DESCRIPTION     Initialize the settings of ROM-Mark decoder

    PARAMETERS      ULONG seed

    RETURNS         None

*****************************************************************************/
void ConfigureRomMarkDecoder(ULONG seed)
{
    // Prepare for reading the ROM_MARK code.

    // Init reading the ROM_MARK
    HAL_DFE_RomMarkEnableDetection();

    LAL_PLL_SetPll();

    HAL_DFE_RomMarkProgramSeed(seed);           // Program seed to the decoder
    HAL_DFE_RomMarkSetDetector();
    HAL_DFE_RomMarkEnKeyDetectedInt();
}

/*****************************************************************************

    FUNCTION        DisableRomMarkDecoder

    DESCRIPTION     Disable ROM-Mark decoder

    PARAMETERS      None

    RETURNS         None

*****************************************************************************/
void DisableRomMarkDecoder(void)
{
    HAL_DFE_RomMarkDisableDetection();
    LAL_PLL_SetPll();
}
#endif  /* ENABLE_AACS == 1 */

/*****************************************************************************

    FUNCTION        BdReadDMA

    DESCRIPTION     Read DMA area

    PARAMETERS      None

    RETURNS         DiscAccessStatusE

*****************************************************************************/
DiscAccessStatusE BdReadDMA(void)
{
    BYTE * DataPtr;
    ULONG temp;
    USHORT pg_offset;
    BYTE Info_Max, i, Valid_Info, Valid_DDS, Valid_SRR, DFL_Idx;
    DiscAccessStatusE result;
    HeaderTypeE Header;
#if (ENABLE_BDIH == 1)
    #if (FORCE_READ_BDIH == 1)
    StLongU tmpValue;
    #endif
#endif

    send_msg80(SEND_SStpTask_MSG,0x7100C0);  //Read DMA Start

    temp = BdRetrySearchRecordingState(UNRECORDED_PSN,
                    DMA_DDS_Info[0].StartPSN, DMA_DDS_Info[0].StartPSN + ONE_BD_CLUSTER - 1);

    if (temp == SEARCH_ERROR)
    {
        send_msg80(SEND_SStpTask_MSG,0x7100A0);
        return GEN_FAIL;
    }

    if(temp == DMA_DDS_Info[0].StartPSN)
    {
        send_msg80(SEND_SStpTask_MSG,0x7100A1);
        return NO_DMA;
    }

    /* read DMA Info */
    Info_Max = sizeof(DMA_DDS_Info) / sizeof(AddrStEndS);
    Valid_Info = FALSE;
    Valid_DDS = FALSE;
    Valid_SRR = FALSE;

    bdDecFlag.fREADING_LEADIN = TRUE;

    /* read DDS */
    for(i = 0 ; i < Info_Max && !Valid_Info; i++)
    {
        /*---------------------------------------------------------------
            Set up the Disc Transfer control parameters
        ----------------------------------------------------------------*/
        ClearDataBuffer();
        bdDiscRequest.Type            = INIT_READ;
        bdDiscRequest.TargetID.Long   = DMA_DDS_Info[i].StartPSN;
        bdDiscRequest.TargetLayer     = eLayer0;

        if(pdbGetDiscKind() == eBDRDLDisc)
            bdDiscRequest.ClusterCount = 2;
        else
            bdDiscRequest.ClusterCount = 1;

        result = GEN_FAIL;

        while(bdDiscRequest.TargetID.Long < DMA_DDS_Info[i].EndPSN)
        {
            result = bdStartDiscTransfer();
            DataPtr = (BYTE*) SCSDRAM_ADDRESS;

            if (result == BUF_GOOD)
            {
                /* check for DDS header */
                Header = eDDS;
                pg_offset = CheckHeader(DataPtr, &Header);

                //copy data info to DDS area in SDRAM
                if(Header == eDDS)  /* DDS is in both BD-R/RE */
                {
                #if (ENABLE_BDIH == 1)
                    if(svoIramVar.IHDisc)   //DDS for BDIH      2009.0925
                    {
                        CopyBufferToBuffer(pg_offset, 512, (BD_TDDS_ADR - SCSDRAM_ADDRESS) /  BD_SECTOR_SIZE, 0, 1);

                        DataPtr = (BYTE *) (BD_TDDS_ADR);
                        send_msg5SValue(1,"BDIH DDS",4, (*(LONG *)(DataPtr)));
                        send_msg5SValue(1,"BDIH DS36",4, (*(LONG *)(DataPtr+36)));
                    }
                    else
                #endif
                    {
                        CopyBufferToBuffer(pg_offset, 0, (BD_TDDS_ADR - SCSDRAM_ADDRESS) /  BD_SECTOR_SIZE, 0, 1);
                    }

                    Valid_DDS = TRUE;
                }

                if(pdbGetBDType() == BD_R)
                {   /* SRRI is at first 1-4 cluster */
                    /* check for SRRI header */
                    Header = eSRRI;
                    pg_offset = CheckHeader(DataPtr, &Header);

                    //copy data info to SRRI area in SDRAM
                    if(Header == eSRRI)
                    {
                        CopyBufferToBuffer(pg_offset, 0, (BD_SRRI_SBM0_ADR - SCSDRAM_ADDRESS) /  BD_SECTOR_SIZE, 0, 1);
                        Valid_SRR = TRUE;
                    }

                    if (Valid_DDS && Valid_SRR)
                        Valid_Info = TRUE;
                }
                else    /* BD_RE */
                {
                    if (Valid_DDS)
                        Valid_Info = TRUE;
                }
            }

            if(Valid_Info)
                break;

            bdDiscRequest.TargetID.Long += bdDiscRequest.ClusterCount * ONE_BD_CLUSTER;
        }
    }

    bdDecFlag.fREADING_LEADIN = FALSE;

#if (ENABLE_BDIH == 1)
    #if (FORCE_READ_BDIH == 1)

     DataPtr = (BYTE *) (BD_TDDS_ADR);

    tmpValue.Long = 0x00A354DF;
    *(DataPtr + 36) = tmpValue.byte.u; /* last LSN of user data area */
    *(DataPtr + 37) = tmpValue.byte.h;
    *(DataPtr + 38) = tmpValue.byte.m;
    *(DataPtr + 39) = tmpValue.byte.l;

    return BUF_GOOD;
    #endif
#endif

    if (result != BUF_GOOD || !Valid_Info)
    {
#if (SET_BDW_DISC_DEFAULT_DATA == 1)
        //set DDS default vaule
        if(pdbGetBDType() == BD_RE)
        {
            result = BdCreateDefaultDMA();
            if(result == BUF_GOOD)
                send_msg80(SEND_SStpTask_MSG,0x7100A2);
            return result;
        }
        else
        {
            return NO_DMA;
        }
#else
        return GEN_FAIL;
#endif
    }
    send_msg80(SEND_SStpTask_MSG,0x7100DB);  //DDS Valid

    if(pdbGetBDType() == BD_R)
    {
        //extract SRRI info
        if(UpdateDBFromSRRI() == FAIL)
        {
            send_msg80(SEND_SStpTask_MSG,0x7100A6);
            return GEN_FAIL;
        }
    }

    /* set last track as complete track */
    pdbSetBDTrackState(pdbGetBDFinalSessLastTrackNumber(), eTRACK_STATE_COMPLETE);

    //disc is error free / no defect list
    if (pdbGetBDDflPSN() == NULL_VALUE)
    {
        return BUF_GOOD;
    }

    /* read DFL Info */
    bdDecFlag.fREADING_LEADIN = TRUE;
    Valid_Info = FALSE;

    /* DFL is 4 cluster long for BD SL, 8 cluster long for BD DL*/
    /* DFL valid location is specified at DDS */
    /*---------------------------------------------------------------
        Set up the Disc Transfer control parameters
    ----------------------------------------------------------------*/
    bdDiscRequest.Type            = INIT_READ;
    bdDiscRequest.TargetLayer     = eLayer0;

    switch (pdbGetBDLayers())
    {
        case BD_SINGLE_LAYER:
        default:
            bdDiscRequest.ClusterCount = 4;
            break;

        case BD_DUAL_LAYER:
            bdDiscRequest.ClusterCount = 8;
            break;
    }

    switch (pdbGetBDType())
    {
        case BD_R:
        default:
            for(i = 0 ; i < Info_Max && !Valid_Info; i++)
            {
                DFL_Idx = 1;
                result = GEN_FAIL;

                for(DFL_Idx = 1; DFL_Idx <= 7 && !Valid_Info; DFL_Idx++)
                {
                    ClearDataBuffer();
                    bdDiscRequest.TargetID.Long = DMA_DDS_Info[i].StartPSN + DFL_Idx * bdDiscRequest.ClusterCount * ONE_BD_CLUSTER;

                    temp = BdRetrySearchRecordingState(UNRECORDED_PSN,
                                    bdDiscRequest.TargetID.Long, bdDiscRequest.TargetID.Long + ONE_BD_CLUSTER - 1);

                    if (temp == SEARCH_ERROR)
                    {
                        //send_msg5S(SEND_SStpTask_MSG, "SEEK FAIL");
                        continue;
                    }

                    if(temp == bdDiscRequest.TargetID.Long)
                    {
                        send_msg80(SEND_SStpTask_MSG,0x7100D6);  //DFL Empty
                        break;
                    }

                    SendMsgCn(SEND_ATAPI_CMD,4,0x800413,A4B(bdDiscRequest.TargetID.Long));
                    DFLStrAddr = bdDiscRequest.TargetID.Long;
                    result = bdStartDiscTransfer();
                    DataPtr = (BYTE*) SCSDRAM_ADDRESS;

                    if (result == BUF_GOOD)
                    {
                        /* check for DFL header */
                        Header = eDFL;
                        pg_offset = CheckHeader(DataPtr, &Header);

                        if(Header == eDFL)
                        {   /* Valid DFL Info */
                            //pg_offset is the DFL start page
                            SendMsgCn(SEND_ATAPI_CMD,2,0x800415,A2B(pg_offset));

                            DataPtr = (BYTE *) (SCSDRAM_ADDRESS + pg_offset * BD_SECTOR_SIZE);
                            Valid_Info = TRUE;
                        }
                    }
                }
            }
            break;

        case BD_RE: /* BD-RE read DFL only 1 time */
            /*---------------------------------------------------------------
                Set up the Disc Transfer control parameters
            ----------------------------------------------------------------*/
            ClearDataBuffer();
            bdDiscRequest.TargetID.Long   = pdbGetBDDflPSN();

            SendMsgCn(SEND_ATAPI_CMD,4,0x800413,A4B(bdDiscRequest.TargetID.Long));

            result = bdStartDiscTransfer();
            DataPtr = (BYTE*) SCSDRAM_ADDRESS;

            if (result == BUF_GOOD)
            {
                /* check for DFL header */
                Header = eDFL;
                pg_offset = CheckHeader(DataPtr, &Header);

                if(Header == eDFL)
                {   /* Valid DFL Info */
                    //pg_offset is the DFL start page
                    SendMsgCn(SEND_ATAPI_CMD,2,0x800416,A2B(pg_offset));

                    DataPtr = (BYTE *) (SCSDRAM_ADDRESS + pg_offset * BD_SECTOR_SIZE);
                    Valid_Info = TRUE;
                }
            }
            break;
    }

    bdDecFlag.fREADING_LEADIN = FALSE;

    if (result != BUF_GOOD || !Valid_Info)
    {
#if (SET_BDW_DISC_DEFAULT_DATA == 1)
        BdCreateDefaultDFL();
#else
        return GEN_FAIL;
#endif
    }
    send_msg80(SEND_SStpTask_MSG,0x7100A7);

    result = GEN_FAIL;

    //DataPtr is a valid DFL info pointer
    switch(pdbGetDiscKind())
    {
        case eBDRDisc:
        case eBDRDLDisc:
            if (SaveBdrDefectListContent((ULONG) DataPtr))
                result = BUF_GOOD;
            break;

        case eBDREDisc:
        case eBDREDLDisc:
            if (SaveBdreDefectListContent((ULONG) DataPtr))
                result = BUF_GOOD;
            break;
    }

    send_msg80(SEND_SStpTask_MSG,0x7100A8);

    return result;
}


/*****************************************************************************

    FUNCTION        BdReadTDMA

    DESCRIPTION     Read TDMA area

    PARAMETERS      None

    RETURNS         DiscAccessStatusE

*****************************************************************************/
DiscAccessStatusE BdReadTDMA(void)
{
    BYTE * DataPtr;
    ULONG temp;
    ULONG * pULvalue;
    tBdDdsS *pTempDDS;
    ULONG ISA0_TDMAsize, OSA_TDMAsize, ISA1_TDMAsize;
    ULONG SearchStart, SearchStop;
    USHORT pg_offset;
    DiscAccessStatusE result;
    HeaderTypeE Header;
    BYTE TDMA_zone, search_len, i;

    send_msg80(SEND_SStpTask_MSG,0x7100B8);  //Read TDMA Start

    switch (pdbGetBDLayers())
    {
        case BD_SINGLE_LAYER:
        default:
            TDMA_zone = 3;
            search_len = 3 * ONE_BD_CLUSTER;
            break;

        case BD_DUAL_LAYER:
            TDMA_zone = 6;
            search_len = 6 * ONE_BD_CLUSTER;
            break;
    }

    //search TDMA indicator
    bdDiscRequest.TargetID.Long = BdRetrySearchRecordingState(RECORDED_PSN,
                                    BD_TDMA0_START_ADDR, BD_TDMA0_START_ADDR + search_len - 1);

    // Search could not be preformed
    if (bdDiscRequest.TargetID.Long == SEARCH_ERROR)
    {
        //send_msg5S(SEND_SStpTask_MSG, "SEEK FAIL");
        return GEN_FAIL;
    }

    bdDiscRequest.TargetID.Long &= BD_CLUSTER_MASK;

    if (bdDiscRequest.TargetID.Long == 0)
        TDMA_zone = 0;
    else
        TDMA_zone -= (bdDiscRequest.TargetID.Long - BD_TDMA0_START_ADDR) / ONE_BD_CLUSTER;

    if(bdDiscRequest.TargetID.Long == BD_TDMA0_START_ADDR)
    {   /* DMA indicator */
        //DDS is in BD_TDMA0_START_ADDR
    }
    else
    {
        //buffer TDMA indicator to get temp DDS
        /* real TDMA is located in TDMA0 */
        if(TDMA_zone > 0)
        {
            /*---------------------------------------------------------------
                Set up the Disc Transfer control parameters
            ----------------------------------------------------------------*/
            bdDecFlag.fREADING_LEADIN = TRUE;
            ClearDataBuffer();
            bdDiscRequest.Type            = INIT_READ;
            bdDiscRequest.TargetLayer     = eLayer0;
            bdDiscRequest.ClusterCount    = 1;

            result = bdStartDiscTransfer();
            bdDecFlag.fREADING_LEADIN = FALSE;
            if (result != BUF_GOOD)
                return result;

            DataPtr = (BYTE*) SCSDRAM_ADDRESS;

            /* check for TDDS header */
            Header = eDDS;
            pg_offset = CheckHeader(DataPtr, &Header);

            if(Header == eDDS)
            {
                pTempDDS = (tBdDdsS *) (SCSDRAM_ADDRESS + pg_offset * BD_SECTOR_SIZE);
                ISA0_TDMAsize = SWAPEND_ULONG(pTempDDS->BdRReDds.ISA0_TDMASize);
                OSA_TDMAsize = SWAPEND_ULONG(pTempDDS->BdRReDds.OSA_TDMASize);
                ISA1_TDMAsize = SWAPEND_ULONG(pTempDDS->BdRReDds.ISA1_TDMASize);

                switch (pdbGetBDLayers())
                {
                    case BD_SINGLE_LAYER:
                    default:
                        switch(TDMA_zone)
                        {
                            case 1: /* TDMA 1 */
                                SearchStart = BD_DATAZONE_START_PSN;
                                SearchStop = SearchStart + ISA0_TDMAsize * ONE_BD_CLUSTER - 1;
                                break;

                            case 2: /* TDMA 2 */
                                SearchStop = ((pdbGetBDL0EndPAA() << 3) & BD_CLUSTER_MASK) + SECTORS_PER_CLUSTER - 1;
                                SearchStart = SearchStop - OSA_TDMAsize * ONE_BD_CLUSTER + 1;
                                break;

                            default:
                                return GEN_FAIL;
                        }
                        break;

                    case BD_DUAL_LAYER:
                        switch(TDMA_zone)
                        {
                            case 1: /* TDMA 1 */
                                SearchStart = BD_TDMAL1_START_ADDR;
                                SearchStop = SearchStart + BD_TDMA0_LENGTH - 1;
                                break;

                            case 2: /* TDMA 2 */
                                SearchStart = BD_DATAZONE_START_PSN;
                                SearchStop = SearchStart + ISA0_TDMAsize * ONE_BD_CLUSTER - 1;
                                break;

                            case 3: /* TDMA 3 */
                                SearchStop = ((pdbGetBDL0EndPAA() << 3) & BD_CLUSTER_MASK) + SECTORS_PER_CLUSTER - 1;
                                SearchStart = SearchStop - OSA_TDMAsize * ONE_BD_CLUSTER + 1;
                                break;

                            case 4: /* TDMA 4 */
                                SearchStart = pdbGetBDStartPSN(1);
                                SearchStop = SearchStart + OSA_TDMAsize * ONE_BD_CLUSTER - 1;
                                break;

                            case 5: /* TDMA 5 */
                                SearchStop = ((pdbGetBDL1EndPAA() << 3) & BD_CLUSTER_MASK) + SECTORS_PER_CLUSTER - 1;
                                SearchStart = SearchStop - ISA1_TDMAsize * ONE_BD_CLUSTER + 1;
                                break;

                            default:
                                return GEN_FAIL;
                        }
                }
            }
            else
            {
                send_msg80(SEND_SStpTask_MSG,0x7100B9);  //TDDS Identifer Err
                return GEN_FAIL;
            }
        }
        else
        {
            /* real TDMA is located in TDMA0 */
            SearchStart = BD_TDMA0_START_ADDR + search_len;
            SearchStop = BD_TDMA0_START_ADDR + BD_TDMA0_LENGTH - 1;
        }

        //search TDMA first UNRECORDED_PSN and will buffer previous one
        bdDiscRequest.TargetID.Long = BdRetrySearchRecordingState(UNRECORDED_PSN, SearchStart, SearchStop);

        // Search could not be preformed
        if (bdDiscRequest.TargetID.Long == SEARCH_ERROR)
        {
            //send_msg5S(SEND_SStpTask_MSG, "SEEK FAIL");
            return GEN_FAIL;
        }

        bdDiscRequest.TargetID.Long &= BD_CLUSTER_MASK;

        if(bdDiscRequest.TargetID.Long == (BD_TDMA0_START_ADDR + search_len))
        {
            send_msg80(SEND_SStpTask_MSG,0x7100BA);  //TDMA Empty
            return NO_TDMA;
        }

        //read last recorded cluster (contains DDS info)
        bdDiscRequest.TargetID.Long -= ONE_BD_CLUSTER;
    }

    /*---------------------------------------------------------------
        Set up the Disc Transfer control parameters
    ----------------------------------------------------------------*/
    bdDecFlag.fREADING_LEADIN = TRUE;
    ClearDataBuffer();
    bdDiscRequest.Type            = INIT_READ;
    bdDiscRequest.TargetLayer     = eLayer0;
    bdDiscRequest.ClusterCount    = 1;

    result = bdStartDiscTransfer();
    bdDecFlag.fREADING_LEADIN = FALSE;
    if (result != BUF_GOOD)
        return result;

    DataPtr = (BYTE*) SCSDRAM_ADDRESS;

    /* check for TDDS header */
    Header = eDDS;
    pg_offset = CheckHeader(DataPtr, &Header);

    //copy data info to TDDS area in SDRAM
    if(Header == eDDS)
    {
        CopyBufferToBuffer(pg_offset, 0, (BD_TDDS_ADR - SCSDRAM_ADDRESS) /  BD_SECTOR_SIZE, 0, 1);
    }
    else
    {
        send_msg80(SEND_SStpTask_MSG,0x7100B9);  //TDDS Identifer Err
        return GEN_FAIL;
    }

    send_msg80(SEND_SStpTask_MSG,0x7100BB);      //TDDS Valid
    bdConvert_init();

    ////////// Read SRRI and DFL clusters indicated from DDS

    /* read SRRI Info */
    temp = SWAPEND_ULONG(pDDS->BdRReDds.PSN_SRRI_L0);
    SendMsgCn(SEND_ATAPI_CMD,4,0x800417,A4B(temp));
    if(pdbGetBDClusterStart(temp) != pdbGetBDClusterStart(bdDiscRequest.TargetID.Long))
    {
        /*---------------------------------------------------------------
            Set up the Disc Transfer control parameters
        ----------------------------------------------------------------*/
        bdDecFlag.fREADING_LEADIN = TRUE;
        ClearDataBuffer();
        bdDiscRequest.Type            = INIT_READ;
        bdDiscRequest.TargetID.Long   = pdbGetBDClusterStart(temp);
        bdDiscRequest.TargetLayer     = eLayer0;
        bdDiscRequest.ClusterCount    = 1;
        result = bdStartDiscTransfer();
        bdDecFlag.fREADING_LEADIN = FALSE;
        if (result != BUF_GOOD)
            return result;
    }

    /* check for SRRI header */
    Header = eSRRI;
    pg_offset = CheckHeader(DataPtr, &Header);

    if(Header == eSRRI)
    {
        pULvalue = (ULONG *) (SCSDRAM_ADDRESS + pg_offset * BD_SECTOR_SIZE + SRRI_CNT_OFFSET);   //SRRI count value
        temp = SWAPEND_ULONG(*pULvalue);

        //copy data info to SRRI area in SDRAM
        CopyBufferToBuffer(pg_offset, 0,
            (BD_SRRI_SBM0_ADR - SCSDRAM_ADDRESS) / BD_SECTOR_SIZE, 0, (temp * 8 + 64 ) / BD_SECTOR_SIZE + 1);

        if(UpdateDBFromSRRI() == FAIL)
        {
            send_msg80(SEND_SStpTask_MSG,0x7100BC);  //Save SRRI Err
            return GEN_FAIL;
        }
    }
    else
    {
        send_msg80(SEND_SStpTask_MSG,0x7100BD);  //SRRI Identifer Err
        return GEN_FAIL;
    }

    send_msg80(SEND_SStpTask_MSG,0x7100BE);  //SRRI Valid

    /* read DFL Info */
    pULvalue = (ULONG *) (BD_TDDS_ADR + DDS_1DFL_OFFSET);
    temp = SWAPEND_ULONG(*pULvalue);
    bdDiscRequest.TargetID.Long = 0;

    for(i = 0; i < 8; i++)
    {
        /*---------------------------------------------------------------
            Set up the Disc Transfer control parameters
        ----------------------------------------------------------------*/
        ClearDataBuffer();
        bdDiscRequest.Type            = INIT_READ;
        bdDiscRequest.ClusterCount    = 1;

        if(temp != 0 && pdbGetBDClusterStart(temp) != pdbGetBDClusterStart(bdDiscRequest.TargetID.Long))   /* DFL is not empty */
        {
            SendMsgCn(SEND_ATAPI_CMD,4,0x800418,A4B(temp));
            DFLStrAddr = temp + 0x10000000;
            bdDiscRequest.TargetID.Long   = temp;

            if(pdbGetBDLayers() == BD_DUAL_LAYER && temp > pdbGetBDMaxPSN(0))
                bdDiscRequest.TargetLayer = eLayer1;
            else
                bdDiscRequest.TargetLayer = eLayer0;
            bdDecFlag.fREADING_LEADIN = TRUE;
            result = bdStartDiscTransfer();
            bdDecFlag.fREADING_LEADIN = FALSE;
            DataPtr = (BYTE*) SCSDRAM_ADDRESS;

            if (result != BUF_GOOD)
                return result;

            /* check for DFL header */
            Header = eDFL;
            pg_offset = CheckHeader(DataPtr, &Header);

            if(Header == eDFL)
            {
                /* DataPtr is a valid DFL info pointer.
                DFL data start from page 0 */
                DataPtr = (BYTE *) (SCSDRAM_ADDRESS + pg_offset * BD_SECTOR_SIZE);

                if (SaveBdrDefectListContent((ULONG) DataPtr))
                    result = BUF_GOOD;
            }
        }

        pULvalue++;
        temp = SWAPEND_ULONG(*pULvalue);
    }

    send_msg80(SEND_SStpTask_MSG,0x7100BF);  //TDFL Valid

    return result;
}


/*****************************************************************************

    FUNCTION        BdReadPAC

    DESCRIPTION     Read PAC area

    PARAMETERS      None

    RETURNS         DiscAccessStatusE

*****************************************************************************/
DiscAccessStatusE BdReadPAC(void)
{
    BYTE * DataPtr;
    USHORT pg_offset;
    BYTE PAC_Max, i, j, Valid_Info;
    DiscAccessStatusE result;
    HeaderTypeE Header;
    ULONG temp;

    send_msg80(SEND_SStpTask_MSG,0x7100B5);    //Read PAC Start
    /*---------------------------------------------------------------
        Stop buffering if on and clear the buffer
    ----------------------------------------------------------------*/
    if ( BD_BUFFERING() == ON )
        bdStopDiscTransfer();

    PAC_Max = sizeof(PAC_Info) / sizeof(AddrStEndS);
    Valid_Info = FALSE;

    bdDecFlag.fREADING_LEADIN = TRUE;

    for(i=0 ; i < PAC_Max && !Valid_Info; i++)
    {
        if(EjectKeyInfo.PressedValid || TrayManagerInfo.Executing)
        {
            //BUILD_SENSE(0x02,0x3A,0x00,0x07);            /* MEDIUM NOT PRESENT */
            return GEN_FAIL;
        }
        /*---------------------------------------------------------------
            Set up the Disc Transfer control parameters
        ----------------------------------------------------------------*/
        ClearDataBuffer();
        bdDiscRequest.Type            = INIT_READ;
        bdDiscRequest.TargetID.Long   = PAC_Info[i].StartPSN;
        bdDiscRequest.TargetLayer     = eLayer0;
        bdDiscRequest.ClusterCount    = 1;
        result = GEN_FAIL;

        while (bdDiscRequest.TargetID.Long < PAC_Info[i].EndPSN)
        {
            SendMsgCn(SEND_ATAPI_CMD,4,0x800419,A4B(bdDiscRequest.TargetID.Long));
            result = bdStartDiscTransfer();
            if (result == BUF_GOOD)
            {
                DataPtr = (BYTE*) SCSDRAM_ADDRESS;

                /* check for PAC header */
                if (pdbGetBDType() == BD_ROM)
                    Header = ePRM;
                else
                    Header = eDWP;

                pg_offset = CheckHeader(DataPtr, &Header);

                //copy data info to PAC area in SDRAM
                if(Header == eDWP || Header == ePRM)
                {
                    CopyBufferToBuffer(pg_offset, 0, BD_PRM_PAC_PAGE, 0, 1);
                    if ( BD_BUFFERING() == ON )
                        bdStopDiscTransfer();
                    ClearDataBuffer();
                    bdDiscRequest.Type            = INIT_READ;
                    //bdDiscRequest.TargetID.Long   = PACStrAddr;
                    bdDiscRequest.TargetLayer     = eLayer0;
                    bdDiscRequest.ClusterCount    = 0;
                    temp = SWAPEND_ULONG(pPAC ->StsOfPAC1Location_L0);

                    SendMsgCn(1,7,0x800300,pPAC ->PACIDH,pPAC ->PACIDM,pPAC ->PACIDL, A4B(temp));

                    for(j=0;j<32;j++)
                    {
                        if(temp & bit0)
                            bdDiscRequest.ClusterCount++;
                        temp >>= 1;
                    }
                    PACcnt = bdDiscRequest.ClusterCount;
                    SendMsgCn(1,4,0x800301,A4B(PACcnt));
                    result = GEN_FAIL;
                    result = bdStartDiscTransfer();
                    if (result == BUF_GOOD)
                    {
                        for(j=0;j<PACcnt;j++)
                        {
                            CopyBufferToBuffer(j*SECTORS_PER_CLUSTER, 0,BD_PRM_PAC_PAGE+j, 0, 1);
                        }
                        Valid_Info = TRUE;
                        break;
                    }
                }
            }

            bdDiscRequest.TargetID.Long += ONE_BD_CLUSTER;
        }
    }

    bdDecFlag.fREADING_LEADIN = FALSE;

    if (result != BUF_GOOD || !Valid_Info)
    {
        send_msg80(SEND_SStpTask_MSG,0x7100B6);    //Read PAC End, NG
        return GEN_FAIL;
    }

    send_msg80(SEND_SStpTask_MSG,0x7100B7);    //Read PAC End, OK

    return result;
}


/*****************************************************************************

    FUNCTION        BdUpdateTrackInfo

    DESCRIPTION     Updates Track information based on start/end psn of track

    PARAMETERS      track - Track number

    RETURNS         None

*****************************************************************************/
void BdUpdateTrackInfo(USHORT track)
{
    ULONG sPSN, ePSN, uPSN, SearchEnd, tmpEndPSN, SearchStart;
    PSNs tmpStartPSN;

    // Avoid updating completed tracks
    if (pdbGetBDTrackState(track) == eTRACK_STATE_COMPLETE)
        return;

    // Get start and end PSN of track
    sPSN = pdbGetBDTrackStartPSN( track );
    ePSN = pdbGetBDTrackEndPSN( track );
    tmpStartPSN = bdConvertLBAtoPSN(pdbGetBDTrackNextWritableAddress(track));
    tmpEndPSN = ePSN;

    uPSN = BdRetrySearchRecordingState(UNRECORDED_PSN, tmpStartPSN.PSN, (tmpStartPSN.PSN + 5 * ONE_BD_CLUSTER - 1));

    if (uPSN == SEARCH_ERROR)
    {
        pdbSetBDTrackState(track, eTRACK_STATE_UNKNOWN);
    }
    else if (uPSN == tmpStartPSN.PSN)
    {
        if (tmpStartPSN.PSN == sPSN)
        {
            // Set status and states of track
            if( pdbGetBDTrackStatus(track) & eTS_ReservedTrack)
            {
                pdbSetBDTrackState(track, eTRACK_STATE_BLANK);
                pdbSetBDTrackDiagStatus(track, eTS_Diag_All_done);
                pdbSetBDTrackStatus(track, ( eTS_ReservedTrack | eTS_BlankTrack | eTS_FixedPacket ));

                if (track >= pdbGetBDFinalSessFirstTrackNumber())
                    pdbSetBDFinalSessCloseInfo(eNotSessionClosed);
            }
            else
            {
                pdbSetBDTrackState(track, eTRACK_STATE_EMPTY);
                pdbSetBDTrackDiagStatus(track,(eTS_Diag_Pregap_done|eTS_Diag_Start_done));
                pdbSetBDTrackStatus(track, ( eTS_InvisibleTrack | eTS_BlankTrack | eTS_FixedPacket ));

                if (track == pdbGetBDFinalSessFirstTrackNumber())
                    pdbSetBDFinalSessCloseInfo(eEmptySession);
                else
                    pdbSetBDFinalSessCloseInfo(eNotSessionClosed);
            }

            pdbSetBDTrackDiagStatus2( track, (eTS_NWA_done | eTS_NWA_valid));
        }
        else
        {
            if (track >= pdbGetBDFinalSessFirstTrackNumber())
                pdbSetBDFinalSessCloseInfo(eNotSessionClosed);
        }


        // Set next writable LBA
        pdbSetBDTrackNextWritableAddress( track, bdConvertPSNtoLBA(tmpStartPSN.PSN) );
        SendMsgCnB(SEND_SStpTask_MSG,5,0x800474,B1B(track), B4B(pdbGetBDTrackNextWritableAddress(track))); //NWA1
    }
    else
    {
        /* check track end has recorded or not */
        SearchStart = ePSN & BD_CLUSTER_MASK;     // Search Start PSN
        SearchEnd = (ePSN & BD_CLUSTER_MASK) + ONE_BD_CLUSTER - 1;

        uPSN = BdRetrySearchRecordingState(RECORDED_PSN, SearchStart, SearchEnd);

        if(SearchStart <= uPSN && uPSN <= SearchEnd)
        {
            if (((pdbGetBDLayers() == SINGLE_LAYER) && (SearchEnd == pdbGetBDMaxPSN(0))) ||
                ((pdbGetBDLayers() == BD_DUAL_LAYER) && (SearchEnd == pdbGetBDMaxPSN(1))))
            {
                pdbSetBDHighestRecordedTrackNumber(track);
                pdbSetBDFinalSessLastTrackNumber(track);
                pdbSetBDTrackStatus(track, eTS_ReservedTrack);
                pdbSetBDFinalSessCloseInfo(eNotSessionClosed);
            }

            /* data has been recorded till end of track */
            pdbSetBDTrackState(track, eTRACK_STATE_COMPLETE );
            pdbSetBDTrackDiagStatus(track, (eTS_Diag_All_done | eTS_Diag_All_valid));
            pdbSetBDTrackDiagStatus2( track, eTS_NWA_done);
            return;
        }
        else if(uPSN == SEARCH_ERROR)
            return;

        if (pdbGetDiscKind() == eBDRDLDisc && sPSN <= pdbGetBDMaxPSN(0) && ePSN >= pdbGetBDStartPSN(1))
        {
            SearchStart = pdbGetBDMaxPSN(0) & BD_CLUSTER_MASK;
            SearchEnd   = pdbGetBDMaxPSN(0);
            uPSN = BdRetrySearchRecordingState(RECORDED_PSN, SearchStart, SearchEnd);

            if (SearchStart <= uPSN && uPSN <= SearchEnd)
            {
                tmpStartPSN.PSN = pdbGetBDStartPSN(1);
                uPSN = BdRetrySearchRecordingState(UNRECORDED_PSN, tmpStartPSN.PSN, (tmpStartPSN.PSN + 5*ONE_BD_CLUSTER - 1));
                if (uPSN == SEARCH_ERROR)
                {
                    pdbSetBDTrackState(track, eTRACK_STATE_UNKNOWN);
                    return;
                }
                else if (uPSN == tmpStartPSN.PSN)
                {
                    pdbSetBDTrackState(track, eTRACK_STATE_INCOMPLETE);
                    pdbSetBDTrackDiagStatus(track, (eTS_Diag_All_done | eTS_Diag_All_valid));
                    pdbSetBDTrackDiagStatus2( track, (eTS_NWA_done | eTS_NWA_valid));

                    // Set next writable LBA
                    pdbSetBDTrackNextWritableAddress( track, bdConvertPSNtoLBA(tmpStartPSN.PSN));
                    SendMsgCnB(SEND_SStpTask_MSG,5,0x800475,B1B(track), B4B(pdbGetBDTrackNextWritableAddress(track))); //NWA2

                    // Set Last recorded PSN
                    pdbSetBDTrackRecordedPSN(track, pdbGetBDMaxPSN(0));
                    pdbSetBDFinalSessCloseInfo(eNotSessionClosed);
                    return;
                }
                else if (uPSN == 0)
                {
                    tmpStartPSN.PSN += (5 * ONE_BD_CLUSTER);
                }
                else
                {
                    pdbSetBDTrackState(track, eTRACK_STATE_INCOMPLETE);
                    pdbSetBDTrackDiagStatus(track, (eTS_Diag_All_done | eTS_Diag_All_valid));
                    pdbSetBDTrackDiagStatus2(track, (eTS_NWA_done | eTS_NWA_valid));

                    // Set next writable LBA
                    pdbSetBDTrackNextWritableAddress(track, bdConvertPSNtoLBA(uPSN));
                    SendMsgCnB(SEND_SStpTask_MSG,5,0x800476,B1B(track), B4B(pdbGetBDTrackNextWritableAddress(track))); //NWA3

                    // Set Last recorded PSN
                    pdbSetBDTrackRecordedPSN(track, uPSN - 1);

                    pdbSetBDFinalSessCloseInfo(eNotSessionClosed);
                    return;
                }
            }
            else if(uPSN == SEARCH_ERROR)
                return;
            else
                tmpEndPSN = pdbGetBDMaxPSN(0);
        }

        /* Track partially recoreded. Start binary search */
        BdStartBinarySearch(track, tmpStartPSN.PSN, tmpEndPSN, sPSN, ePSN);
    }
}


/*****************************************************************************

    FUNCTION        BdUpdateIncompleteTracks

    DESCRIPTION     Updates all incomplete track information

    PARAMETERS      None

    RETURNS         None

*****************************************************************************/
void BdUpdateIncompleteTracks()
{
    BYTE track;
    pdbSetBDLogicalData();

    for (track = 1; track <= pdbGetBDHighestRecordedTrackNumber(); track++)
        BdUpdateTrackInfo(track);
}


/*****************************************************************************

    FUNCTION        BdGetCurrentPSN

    DESCRIPTION     This function read the PSN from wobble / Data ID

    PARAMETERS      PSN from decoder flag

    RETURNS         Current PSN

*****************************************************************************/
ULONG BdGetCurrentPSN(BYTE * BdDecoder_ID)
{
    ULONG CurrentPSN, clock;
    ULONG AUNFLYRegister, AUNFLYStatus, AUNStatusRegister, AUNRegister, AUNEndRegister;
    BYTE AUNDecodingStatus;

    *BdDecoder_ID = FALSE;

    StartTimer(&clock);
    while (1)
    {
        if (ReadTimer(&clock) > 1000L )
        {
            CurrentPSN = 0xFFFFFFFFL;
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
                    //CurrentPSN = AUNRegister;
                    CurrentPSN = AUNFLYRegister;
                    SendMsgCn(HEADER_ID_MSG,4,0x3B060D,A4B(svoIramVar.currentID.SectorNum.all32));//CheckHeaderID AUN Address :
                    *BdDecoder_ID = TRUE;
                    break;
                }
            }
        }
#if (EN_WOBBLE == 1 )
        //--------------------------------------------------------------------------;
        // check Wobble address                                                     ;
        //--------------------------------------------------------------------------;
        if(pdbGetIDOk())
        {
            DISABLE_INTERRUPT();
            CurrentPSN = (Iram_DiscInfo.BD.ClusterAddress << 3) & PSNMaskBD;
            svoIramVar.currentID.SectorNum.all32 = CurrentPSN;
//            send_msg84L(DEBUG_WBL_MSG,0x610108,CurrentPSN); //BdGetCurrentPSN
            pdbSetIDOk(FALSE);
            ENABLE_INTERRUPT();
            //*BdDecoder_ID = TRUE;
            break;
        }
#endif
    }

    return (CurrentPSN);
}


/*****************************************************************************

    FUNCTION        BdSearchRecordingState

    DESCRIPTION     Returns the first PSN of the ECC block that matches the
                    specified state.

    PARAMETERS      State           Target state to search
                                    UNRECORDED_PSN - search for first unrecorded PSN
                                    RECORDED_PSN - search for first recorded PSN
                    StartPsn        Start PSN of search
                    EndPsn          End PSN of search

    RETURNS         CurrentPSN      PSN of first ECC block within the
                                    specified range (StartPsn to EndPsn) that matches
                                    the target state. Set to 0 if there were no ECC
                                    blocks matching the target state

                    SEARCH_ERROR    Seek failure

    NOTE            Target state is checked against the hardware
                    signal RECD. This signal is high if the area has been recorded and
                    low if area is unrecorded.

*****************************************************************************/
ULONG BdSearchRecordingState(SearchRecStateE State, ULONG StartPSN, ULONG EndPSN)
{
    ULONG search_timer, search_timer1;
    ULONG CurrentPSN, tPSN, LastRecordedPSN;
    BYTE layer, LayerSwitch, Searching;
    BYTE retry, SecondSearch;
    BYTE BdDecoder_ID;
    BYTE BdDecoder_IDcount;

    send_msg80(SEND_SStpTask_MSG,0x7100B1);    //Binary Search Start
    SendMsgCnB(SEND_SStpTask_MSG,9,0x800470,B1B(State),B4B(StartPSN),B4B(EndPSN));

    LayerSwitch = FALSE;
    Searching = TRUE;
    BdDecoder_ID = FALSE;
    BdDecoder_IDcount = 0;
    LastRecordedPSN = 0xFFFF0000L;
    SecondSearch = FALSE;

    // Determine which layer to search
    if (pdbGetBDLayers() == BD_DUAL_LAYER)
    {
        if (EndPSN <= pdbGetBDMaxPSN(0))           // Search only in layer 0
            layer = eLayer0;
        else if (StartPSN >= pdbGetBDStartPSN(1))  // Search only in layer 1
            layer = eLayer1;
        else
        {                                           // Search crosses layers
            LayerSwitch = TRUE;                     // Start search in layer 0
            tPSN = EndPSN;
            EndPSN = pdbGetBDMaxPSN(0);
            layer = eLayer0;
        }
    }
    else
    {
        layer = eLayer0;
    }
    retry = 0;
    // Search

    while (Searching)
    {
        //send_msg5SValue(SEND_SStpTask_MSG, "Seek PSN", 4, StartPSN);

        while(retry < 5)
        {
            if ( bdSeek(StartPSN, layer) != Error)
            {
                CurrentPSN = BdGetCurrentPSN(&BdDecoder_ID);
                if(((StartPSN - 32*ONE_BD_CLUSTER) < CurrentPSN) && (CurrentPSN < StartPSN))
                    break;
            }

            retry++;
        }
        if(retry >= 5)
        {
            send_msg80(SEND_SStpTask_MSG,0x7100B4);    //Binary Search, max. retries reached
            return (SEARCH_ERROR);
        }

        //send_msg5SValue(SEND_SStpTask_MSG, "End Seek", 4, BdGetCurrentPSN(&BdDecoder_ID));

        StartTimer(&search_timer);
        StartTimer(&search_timer1);

        // Determine the first ECC block within the specified range matching the target state
        while(CurrentPSN <= EndPSN)
        {
            // Get current PSN. Add one bd cluster to compensate wobble address delay
            CurrentPSN = BdGetCurrentPSN(&BdDecoder_ID);
            if(CurrentPSN == 0xFFFFFFFFL)
            {
                SendMsgCnB(SEND_SStpTask_MSG,4,0x80041E,B4B(CurrentPSN)); // SEARCH_ERROR
                return (SEARCH_ERROR);
            }

//            DelaymS(1);

            /* apply decoder id to help recorded/unrecorded detection */
            if ((BdDecoder_ID == TRUE)&&(HAL_GLOBAL_GetRfPresent() == RECORDED_PSN))//got decoder id
                LastRecordedPSN = (CurrentPSN & BD_CLUSTER_MASK);
            else if (State == RECORDED_PSN)//only woble id, no decoder id
                LastRecordedPSN=BdGetCurrentPSN(&BdDecoder_ID);

            if ((CurrentPSN >= StartPSN) && (CurrentPSN <= EndPSN))
            {
                if (BdDecoder_ID == TRUE)
                    BdDecoder_IDcount++;
            }
            if ( (StartPSN == 0x0DD400)||(StartPSN == 0x0EE060)||(StartPSN == 0x0EE0C0))
                StartPSN = StartPSN + 8 ;

            if ((CurrentPSN >= StartPSN) && (CurrentPSN <= EndPSN) && (HAL_GLOBAL_GetRfPresent() == State))
            {
                if ( (StartPSN == 0x0DD408)||(StartPSN == 0x0EE068)||(StartPSN == 0x0EE0C8))
                    StartPSN = StartPSN - 8  ;
                if (State==UNRECORDED_PSN)
                {
                    if ((HAL_GLOBAL_GetRfPresent() == 0)&&(BdDecoder_ID == 0))
                    {
                        //recorded disc
                        if (LastRecordedPSN != 0xFFFF0000L)
                        {
                            // Target state found
                            if (LastRecordedPSN < StartPSN)
                            {
                                LastRecordedPSN = 0xFFFF0000L;
                            }
                            else
                            {
                                CurrentPSN=LastRecordedPSN + ONE_BD_CLUSTER;
                                break;
                            }
                        }
                        //blank disc
                        if ((CurrentPSN >= StartPSN)&&(LastRecordedPSN == 0xFFFF0000L))
                        {
                            // Target state found
                            CurrentPSN=StartPSN;
                            break;
                        }
                    }
                }
                else//(State==RECORDED_PSN)
                {
                    // Target state found
                    CurrentPSN=BdGetCurrentPSN(&BdDecoder_ID);
                    if (BdDecoder_ID == TRUE)//got decoder id
                       break;
                }
            }
            /*--------------------------------------------------
                Over 500ms, Exit Process to return command.
            ---------------------------------------------------*/
            if(ReadTimer(&search_timer) > 500L)
            {
                ExitProcess();
                StartTimer(&search_timer);
            }

            /*--------------------------------------------------
                Search recording timer protect,
                the timer limit  depends on searching length!
            ---------------------------------------------------*/
            if(ReadTimer(&search_timer1) > (3000 + ((EndPSN - StartPSN)/ONE_BD_CLUSTER)*10))
            {
                send_msg80(SEND_SStpTask_MSG,0x7100B3);    //Binary Search, time out
                return (SEARCH_ERROR);
            }
        }
        if ( (StartPSN == 0x0DD408)||(StartPSN == 0x0EE068)||(StartPSN == 0x0EE0C8))
            StartPSN = StartPSN - 8  ;

        // There are no blocks within the specified range which match the target state
        if (CurrentPSN != 0xFFFFFFFFL)
        {
            if (CurrentPSN > EndPSN)
            {
            /*---------------------------------------------------------------------
                If No found the Target,
                do seacond search to avoid mis-judgment because of wobble ID loss.
            ----------------------------------------------------------------------*/
                CurrentPSN = 0;

                if(SecondSearch == FALSE)
                    SecondSearch = TRUE;
                else
                    SecondSearch = FALSE;

                if( (StartPSN == 0xDD400)&&(State==UNRECORDED_PSN)&&(BdDecoder_IDcount == 0)&&(HAL_GLOBAL_GetRfPresent() == 0))
                {
                    CurrentPSN=StartPSN;
                }
            }
            else
            {
                // No need for layer switch if target state found
                if (LayerSwitch == TRUE)
                    LayerSwitch = FALSE;
                // do second search to confirm if targetPSN == startPSN
                if (CurrentPSN != StartPSN)
                    Searching = FALSE;

                SecondSearch = FALSE;
            }

            // Search complete else go to next layer to finish search
            if (LayerSwitch == TRUE)
            {
                layer = eLayer1;
                StartPSN = pdbGetBDStartPSN(1);
                EndPSN = tPSN;
                LayerSwitch = FALSE;
            }
        }
        else
        {
            SecondSearch = FALSE;
        }

        /* No found Target Keep Searching */
        if(SecondSearch == TRUE)
            Searching = TRUE;
        else
            Searching = FALSE;

    }

    // Set pause
    Svr_pause_reqire        = TRUE;
    Iram_Svr_player_mode    = PAUSING_MODE;

    if(State == RECORDED_PSN)
    {
        SendMsgCnB(SEND_SStpTask_MSG,4,0x800471,B4B(CurrentPSN));
    }
    else
    {
        SendMsgCnB(SEND_SStpTask_MSG,4,0x800472,B4B(CurrentPSN));
    }

    return CurrentPSN;
}


/*****************************************************************************

    FUNCTION        BdRetrySearchRecordingState

    DESCRIPTION     Retry search recording state

    PARAMETERS      State           Target state to search
                                    UNRECORDED_PSN - search for first unrecorded PSN
                                    RECORDED_PSN - search for first recorded PSN
                                StartPsn        Start PSN of search
                                EndPsn          End PSN of search

    RETURNS         CurrentPSN      PSN of first ECC block within the
                                    specified range (StartPsn to EndPsn) that matches
                                    the target state. Set to 0 if there were no ECC
                                    blocks matching the target state

*****************************************************************************/
ULONG BdRetrySearchRecordingState(SearchRecStateE State, ULONG StartPSN, ULONG EndPSN)
{
    ULONG CurrentPSN;
    BYTE SearchErrorRetryCount;

    CurrentPSN = BdSearchRecordingState(State, StartPSN, EndPSN);

    if (CurrentPSN == SEARCH_ERROR)
    {
        SearchErrorRetryCount = 0;
        if (pdbServoGetDiscType() == eBDDisc)
        {
            while(SearchErrorRetryCount < MAX_SEARCH_ERROR_RETRY_COUNTER)
            {
                CurrentPSN = BdSearchRecordingState(State, StartPSN, EndPSN);
                if (CurrentPSN != SEARCH_ERROR)
                    break;

                SearchErrorRetryCount++;
            }
        }
    }

    return CurrentPSN;
}


/*****************************************************************************

    FUNCTION        BdStartBinarySearch

    DESCRIPTION     Track partially recoreded. Start binary search

    PARAMETERS      track           search track number
                    StartPsn        Start PSN of search
                    EndPsn          End PSN of search


    RETURNS

*****************************************************************************/
void BdStartBinarySearch(BYTE track, ULONG StartPSN, ULONG EndPSN, ULONG sPSN, ULONG ePSN)
{
    ULONG uPSN, SearchEnd;
    ULONG SearchStart, CurrentPSN, SecondStart;
    BOOL FinalSearch;
    SearchStart = (StartPSN + EndPSN) / 2;  // Search Start PSN
    SearchStart &= BD_CLUSTER_MASK;
    SearchEnd   = EndPSN;                   // Search End PSN
    FinalSearch = FALSE;

    while (1)
    {
        if(FinalSearch)
        {
            send_msg80(SEND_SStpTask_MSG,0x7100B2);    //Binary Search, final step
            uPSN = BdRetrySearchRecordingState(UNRECORDED_PSN, SearchStart, SearchEnd);
        }
        else
        {
            uPSN = BdRetrySearchRecordingState(UNRECORDED_PSN, SearchStart, (SearchStart + 5*ONE_BD_CLUSTER - 1));
        }

        CurrentPSN = pdbGetBDClusterAddress();

        if (uPSN == SEARCH_ERROR)
        {
            pdbSetBDTrackState(track, eTRACK_STATE_UNKNOWN);
            SendMsgCnB(SEND_SStpTask_MSG,1,0x80041F,B1B(track)); // eTRACK_STATE_UNKNOWN
            break;
        }
        else if (uPSN == 0)     /* this area was recoreded */
        {
            StartPSN = SearchStart + 5 * ONE_BD_CLUSTER;

            if (CurrentPSN > ePSN)      /* TRACK FULL */
            {
                if (track != pdbGetBDHighestRecordedTrackNumber())
                {
                    pdbSetBDTrackState(track, eTRACK_STATE_COMPLETE );
                    pdbSetBDTrackDiagStatus(track, (eTS_Diag_All_done | eTS_Diag_All_valid));
                    pdbSetBDTrackDiagStatus2( track, eTS_NWA_done);
                }
                else
                {
                    pdbSetBDTrackRecordedPSN(track, ePSN);
                    pdbSetBDTrackNextWritableAddress(track, bdConvertPSNtoLBA(ePSN+1));
                    pdbSetBDTrackDiagStatus2( track, eTS_NWA_done | eTS_NWA_valid);
                    pdbSetBDTrackDiagStatus(track, (eTS_Diag_All_done | eTS_Diag_All_valid));

                    pdbSetBDTrackState(track, eTRACK_STATE_INCOMPLETE );
                    pdbSetBDTrackStatus(track, eTS_InvisibleTrack);

                    pdbSetBDFinalSessCloseInfo(eNotSessionClosed);

                    SendMsgCnB(SEND_SStpTask_MSG,5,0x800477,B1B(track), B4B(pdbGetBDTrackNextWritableAddress(track))); //NWA4
                }
                break;
            }
            else                        /* for reserved / incomplete track */
            {
                if(FinalSearch)
                {
                    pdbSetBDTrackState(track, eTRACK_STATE_INCOMPLETE);
                    if(track == pdbGetBDFinalSessLastTrackNumber())
                        pdbSetBDTrackStatus(track, eTS_InvisibleTrack);
                    else
                        pdbSetBDTrackStatus(track, eTS_ReservedTrack);
                    pdbSetBDTrackDiagStatus(track, (eTS_Diag_All_done | eTS_Diag_All_valid));
                    pdbSetBDTrackDiagStatus2( track, (eTS_NWA_done | eTS_NWA_valid));

                    // Set next writable LBA
                    pdbSetBDTrackNextWritableAddress( track, bdConvertPSNtoLBA(SearchEnd + 1));
                    SendMsgCnB(SEND_SStpTask_MSG,5,0x800478,B1B(track), B4B(pdbGetBDTrackNextWritableAddress(track))); //NWA5

                    // Set Last recorded PSN
                    pdbSetBDTrackRecordedPSN(track, SearchEnd);
                    pdbSetBDFinalSessCloseInfo(eNotSessionClosed);
                    break;
                }
                else
                {
                    if ((SearchEnd - SearchStart) < 0x560)
                    {
                        FinalSearch = TRUE;
                    }
                    else
                    {
                        SecondStart = (SearchStart + SearchEnd) / 2;
                        SecondStart &= BD_CLUSTER_MASK;
                        uPSN = BdRetrySearchRecordingState(UNRECORDED_PSN, SecondStart, SecondStart + 5*ONE_BD_CLUSTER - 1);

                        CurrentPSN = pdbGetBDClusterAddress();

                        if (uPSN == SEARCH_ERROR)
                        {
                            pdbSetBDTrackState(track, eTRACK_STATE_UNKNOWN);
                            break;
                        }
                        else if(uPSN == 0)              /* middle point was recorded */
                        {
                            StartPSN = SecondStart + 5 * ONE_BD_CLUSTER;
                            if ((SearchEnd - StartPSN) < 0x560)
                            {
                                FinalSearch = TRUE;
                                SearchStart = StartPSN;
                            }
                            else
                            {
                                SearchStart = (StartPSN + SearchEnd) / 2;
                                SearchStart &= BD_CLUSTER_MASK;
                            }
                        }
                        else if(uPSN == SecondStart)    /* middle point was not recorded */
                        {
                            SearchEnd = SecondStart - 1;
                            if ((SearchEnd - StartPSN) < 0x560)
                            {
                                FinalSearch = TRUE;
                                SearchStart = StartPSN;
                            }
                            else
                            {
                                SearchStart = (StartPSN + SearchEnd)/2;
                                SearchStart &= BD_CLUSTER_MASK;
                            }
                        }
                        else                            /* boundary is inside */
                        {
                            SearchStart = SecondStart;
                            FinalSearch = TRUE;
                        }
                    }
                }
            }
        }
        else if (uPSN == SearchStart)
        {
            if (FinalSearch == TRUE)
            {
                pdbSetBDTrackState(track, eTRACK_STATE_INCOMPLETE);
                if(track == pdbGetBDFinalSessLastTrackNumber())
                    pdbSetBDTrackStatus(track, eTS_InvisibleTrack);
                else
                    pdbSetBDTrackStatus(track, eTS_ReservedTrack);
                pdbSetBDTrackDiagStatus(track, (eTS_Diag_All_done | eTS_Diag_All_valid));
                pdbSetBDTrackDiagStatus2(track, (eTS_NWA_done | eTS_NWA_valid));

                // Set next writable LBA
                pdbSetBDTrackNextWritableAddress(track, bdConvertPSNtoLBA(uPSN));
                SendMsgCnB(SEND_SStpTask_MSG,5,0x800479,B1B(track), B4B(pdbGetBDTrackNextWritableAddress(track))); //NWA6

                // Set Last recorded PSN
                pdbSetBDTrackRecordedPSN(track, uPSN - 1);

                pdbSetBDFinalSessCloseInfo(eNotSessionClosed);
                break;
            }
            else
            {
                SearchEnd = SearchStart - 1;
                if(SearchStart < (StartPSN + 0x560))
                {
                    SearchStart = StartPSN;
                    FinalSearch = TRUE;
                }
                else
                {
                    SearchStart = (StartPSN + SearchEnd) / 2;
                    SearchStart &= BD_CLUSTER_MASK;
                }
            }
        }
        else if (uPSN != SearchStart)
        {                               /* PARTIAL RECORDED */
            pdbSetBDTrackNextWritableAddress(track, bdConvertPSNtoLBA(uPSN));
            pdbSetBDTrackState(track, eTRACK_STATE_INCOMPLETE);
            if(track == pdbGetBDFinalSessLastTrackNumber())
                pdbSetBDTrackStatus(track, eTS_InvisibleTrack);
            else
                pdbSetBDTrackStatus(track, eTS_ReservedTrack);

            pdbSetBDTrackDiagStatus(track, (eTS_Diag_All_done | eTS_Diag_All_valid));
            pdbSetBDTrackDiagStatus2(track, (eTS_NWA_done | eTS_NWA_valid));

            // Set next writable LBA
            SendMsgCnB(SEND_SStpTask_MSG,5,0x80047A,B1B(track), B4B(pdbGetBDTrackNextWritableAddress(track))); //NWA7

            // Set Last recorded PSN
            pdbSetBDTrackRecordedPSN(track, uPSN - 1);

            pdbSetBDFinalSessCloseInfo(eNotSessionClosed);
            break;
        }
    }
}

#if BDRE_BYPASS_LEADIN
void BdCreateDefaultPIC(void)
{
    BYTE * bPtr;
    ULONG i;
    StLongU tmpValue;

    //PIC data
    bPtr = (BYTE *) (BD_DI_ADR);
    LinearPatternFill((((ULONG) bPtr - SCSDRAM_ADDRESS) / 0x800), 0, ONE_BD_CLUSTER, 0x800/2, 0x0000);

    *(bPtr + 0) = 'D';
    *(bPtr + 1) = 'I';
    *(bPtr + 2) = 0x01;     /* DI format # */
    *(bPtr + 3) = 0x10;     /* # of DI Aux frames */

    *(bPtr + 4) = 0x01;     /* legacy info */
    *(bPtr + 5) = 0x00;     /* DI seq # */
    *(bPtr + 6) = 0x63;     /* # of DI bytes */

    *(bPtr + 8) = 'B';
    *(bPtr + 9) = 'D';
    *(bPtr + 10) = 'W';

    *(bPtr + 11) = 0x02;    /* disc size/class/version */

    *(bPtr + 12) = 0x14;    /* bd structure */
    *(bPtr + 13) = 0x01;    /* hybrid disc/ch bit length */

    *(bPtr + 16) = 0x01;    /* BCA descriptor */

    tmpValue.Long = 0x00020000;
    *(bPtr + 24) = tmpValue.byte.u; /* data zone allocation */
    *(bPtr + 25) = tmpValue.byte.h;
    *(bPtr + 26) = tmpValue.byte.m;
    *(bPtr + 27) = tmpValue.byte.l;

    tmpValue.Long = 0x00194e7e;
    *(bPtr + 28) = tmpValue.byte.u;
    *(bPtr + 29) = tmpValue.byte.h;
    *(bPtr + 30) = tmpValue.byte.m;
    *(bPtr + 31) = tmpValue.byte.l;

    send_msg80(SEND_SStpTask_MSG,0x7100B0);
    for(i = 0; i<100 ; i+=4)
        send_msg54(SEND_SStpTask_MSG, *(bPtr+i+0), *(bPtr+i+1), *(bPtr+i+2),*(bPtr+i+3));
}

#endif //BDRE_BYPASS_LEADIN
/*****************************************************************************

    FUNCTION        BdCreateDefaultDMA

    DESCRIPTION     set default value for DMA

    PARAMETERS

    RETURNS

*****************************************************************************/
BYTE BdCreateDefaultDMA(void)
{
    BYTE * bPtr;
    //ULONG i;
    StLongU tmpValue;//,ISA0size,ISA1size,OSAsize;
    BYTE result;


    bPtr = (BYTE *) (BD_TDDS_ADR);
    LinearPatternFill((((ULONG) bPtr - SCSDRAM_ADDRESS) / 0x800), 0, 1, 0x800/2, 0x0000);

    *(bPtr + 0) = 'D';
    *(bPtr + 1) = 'S';

    tmpValue.Long = 0x01;
    *(bPtr + 4) = tmpValue.byte.u;  /* DDS update count */
    *(bPtr + 5) = tmpValue.byte.h;
    *(bPtr + 6) = tmpValue.byte.m;
    *(bPtr + 7) = tmpValue.byte.l;

    *(bPtr + 16) = 0x00; /* first PSN of drive area */

    tmpValue.Long = 0x0DD500;//DFL_Info[0].StartPSN;
    *(bPtr + 24) = tmpValue.byte.u; /* first PSN of defect list */
    *(bPtr + 25) = tmpValue.byte.h;
    *(bPtr + 26) = tmpValue.byte.m;
    *(bPtr + 27) = tmpValue.byte.l;

    tmpValue.Long = 0x00100000;
    *(bPtr + 32) = tmpValue.byte.u; /* location of LSN0 of user data area */
    *(bPtr + 33) = tmpValue.byte.h;
    *(bPtr + 34) = tmpValue.byte.m;
    *(bPtr + 35) = tmpValue.byte.l;

    tmpValue.Long = 0x00B354DF - 0x00100000;
    *(bPtr + 36) = tmpValue.byte.u; /* last LSN of user data area */
    *(bPtr + 37) = tmpValue.byte.h;
    *(bPtr + 38) = tmpValue.byte.m;
    *(bPtr + 39) = tmpValue.byte.l;

    tmpValue.Long = 0;
    *(bPtr + 40) = tmpValue.byte.u; /* ISA0 size */
    *(bPtr + 41) = tmpValue.byte.h;
    *(bPtr + 42) = tmpValue.byte.m;
    *(bPtr + 43) = tmpValue.byte.l;

    *(bPtr + 44) = 0x00; /* OSA size */
    *(bPtr + 48) = 0x00; /* ISA1 size */

    *(bPtr + 52) = 0x0F; /* spare area full flag */
    *(bPtr + 54) = 0x00; /* disc certification flags */
    *(bPtr + 56) = 0x00; /* last verified address pointer */

    *(bPtr + 64) = 0x00; /* status bits of INFO1 locations on L0 */
    *(bPtr + 72) = 0x00; /* status bits of INFO2 locations on L0 */
    *(bPtr + 80) = 0x00; /* status bits of INFO1 locations on L1 */
    *(bPtr + 88) = 0x00; /* status bits of INFO2 locations on L1 */

    send_msg80(SEND_SStpTask_MSG,0x7100AF);  //Default DDS
//    for(i = 0; i<96 ; i+=4)
//        send_msg54(SEND_SStpTask_MSG, *(bPtr+i+0), *(bPtr+i+1), *(bPtr+i+2),*(bPtr+i+3));
//    send_msg5S(SEND_SStpTask_MSG, "Default DDS");

    //DFL data
    bPtr = (BYTE *) (SCSDRAM_ADDRESS);
    LinearPatternFill((((ULONG) bPtr - SCSDRAM_ADDRESS) / 0x800), 0, ONE_BD_CLUSTER, 0x800/2, 0x0000);

    *(bPtr + 0) = 'D';
    *(bPtr + 1) = 'L';
    tmpValue.Long = 0x01;
    *(bPtr + 4) = tmpValue.byte.u;  /* DFL update count */
    *(bPtr + 5) = tmpValue.byte.h;
    *(bPtr + 6) = tmpValue.byte.m;
    *(bPtr + 7) = tmpValue.byte.l;

    tmpValue.Long = 0xffffffff;
    *(bPtr + 64) = tmpValue.byte.u; /* DFL terminator */
    *(bPtr + 65) = tmpValue.byte.h;
    *(bPtr + 66) = tmpValue.byte.m;
    *(bPtr + 67) = tmpValue.byte.l;
    *(bPtr + 68) = *(bPtr + 4);     /* DFL update count */
    *(bPtr + 69) = *(bPtr + 5);
    *(bPtr + 70) = *(bPtr + 6);
    *(bPtr + 71) = *(bPtr + 7);

    send_msg80(SEND_SStpTask_MSG,0x7100AE);  //Default DFL
//    for(i = 0; i<80 ; i+=4)
//        send_msg54(SEND_SStpTask_MSG, *(bPtr+i+0), *(bPtr+i+1), *(bPtr+i+2),*(bPtr+i+3));
//    send_msg5S(SEND_SStpTask_MSG, "Default DFL");

    result = GEN_FAIL;

    switch(pdbGetDiscKind())
    {
        case eBDRDisc:
        case eBDRDLDisc:
            if(SaveBdrDefectListContent((ULONG) bPtr))
                result = BUF_GOOD;
            break;

        case eBDREDisc:
        case eBDREDLDisc:
            if(SaveBdreDefectListContent((ULONG) bPtr))
                result = BUF_GOOD;
            break;
    }

    return result;
}
/*****************************************************************************

    FUNCTION        BdCreateDefaultDFL

    DESCRIPTION     set default value for DFL

    PARAMETERS

    RETURNS

*****************************************************************************/
void BdCreateDefaultDFL(void)
{
    BYTE * bPtr;
    StLongU tmpValue;
    //DFL data
    bPtr = (BYTE *) (SCSDRAM_ADDRESS);
    LinearPatternFill((((ULONG) bPtr - SCSDRAM_ADDRESS) / 0x800), 0, ONE_BD_CLUSTER, 0x800/2, 0x0000);

    *(bPtr + 0) = 'D';
    *(bPtr + 1) = 'L';
    tmpValue.Long = 0x01;
    *(bPtr + 4) = tmpValue.byte.u;  /* DFL update count */
    *(bPtr + 5) = tmpValue.byte.h;
    *(bPtr + 6) = tmpValue.byte.m;
    *(bPtr + 7) = tmpValue.byte.l;

    tmpValue.Long = 0xffffffff;
    *(bPtr + 64) = tmpValue.byte.u; /* DFL terminator */
    *(bPtr + 65) = tmpValue.byte.h;
    *(bPtr + 66) = tmpValue.byte.m;
    *(bPtr + 67) = tmpValue.byte.l;
    *(bPtr + 68) = *(bPtr + 4);     /* DFL update count */
    *(bPtr + 69) = *(bPtr + 5);
    *(bPtr + 70) = *(bPtr + 6);
    *(bPtr + 71) = *(bPtr + 7);

    send_msg80(SEND_SStpTask_MSG,0x7100AE);    //Default DFL
}
/*****************************************************************************

    FUNCTION        BdCreateDefaultSRRI

    DESCRIPTION     set default value for SRRI

    PARAMETERS

    RETURNS

*****************************************************************************/
void BdCreateDefaultSRRI(void)
{
    BYTE * bPtr;
    StLongU tmpValue;


    bPtr = (BYTE *) (BD_SRRI_SBM0_ADR);
    LinearPatternFill((((ULONG) bPtr - SCSDRAM_ADDRESS) / 0x800), 0, 2, 0x800/2, 0x0000);

    *(bPtr + 0) = 'S';
    *(bPtr + 1) = 'R';

    *(bPtr + 2) = 0x00; //SRRI Format
    *(bPtr + 3) = 0x00;

    tmpValue.Long = 0x01;
    *(bPtr + 4) = tmpValue.byte.u;  /* SRRI update count */
    *(bPtr + 5) = tmpValue.byte.h;
    *(bPtr + 6) = tmpValue.byte.m;
    *(bPtr + 7) = tmpValue.byte.l;

    tmpValue.Long = 0x01;
    *(bPtr + 12) = tmpValue.byte.u;  /* number of SRR entries */
    *(bPtr + 13) = tmpValue.byte.h;
    *(bPtr + 14) = tmpValue.byte.m;
    *(bPtr + 15) = tmpValue.byte.l;

    tmpValue.Long = 0x00;
    *(bPtr + 16) = tmpValue.byte.u;  /* number of open SRRs */
    *(bPtr + 17) = tmpValue.byte.h;
    *(bPtr + 18) = tmpValue.byte.m;
    *(bPtr + 19) = tmpValue.byte.l;

    //List of SRRI entries
    tmpValue.Long = pdbGetBDLsn0StartPSN();
    *(bPtr + 64) = tmpValue.byte.u;  /* Start PSN of the SRR 1 */
    *(bPtr + 65) = tmpValue.byte.h;
    *(bPtr + 66) = tmpValue.byte.m;
    *(bPtr + 67) = tmpValue.byte.l;

    tmpValue.Long = (pdbGetBDEndLSN()|0x80000000);
    *(bPtr + 68) = tmpValue.byte.u;  /* LRA in the SRR 1 */
    *(bPtr + 69) = tmpValue.byte.h;
    *(bPtr + 70) = tmpValue.byte.m;
    *(bPtr + 71) = tmpValue.byte.l;

    tmpValue.Long = 0xFFFFFFFF;
    *(bPtr + 72) = tmpValue.byte.u;  /* End of SRR  */
    *(bPtr + 73) = tmpValue.byte.h;
    *(bPtr + 74) = tmpValue.byte.m;
    *(bPtr + 75) = tmpValue.byte.l;


    send_msg80(SEND_SStpTask_MSG,0x7100A4);
//    for(i = 0; i<96 ; i+=4)
//        send_msg54(SEND_SStpTask_MSG, *(bPtr+i+0), *(bPtr+i+1), *(bPtr+i+2),*(bPtr+i+3));
//    send_msg5S(SEND_SStpTask_MSG, "Default SRRI");

}
/*******************************************************************************

    FUNCTION        CheckBDStartAddr

    DESCRIPTION     Check BD Disc Start address

********************************************************************************/
BYTE CheckBDStartAddr()
{
    BYTE buffer_result,buffer_count;
    PSNs psn;
    ULONG timer,datasum,i;
    BYTE* mem_address;
    ADDRESS_T start_LBA;
    start_LBA.lba = 0x10;
    datasum = 0;
    buffer_count = 1;
   if(BD_BUFFERING() == ON)
        bdStopDiscTransfer();

    ClearDataBuffer();

    psn = bdConvertLBAtoPSN(pdbGetBDClusterStart(start_LBA.lba));

    bdDiscRequest.TargetID.Long = psn.PSN;
    bdDiscRequest.Type            = INIT_READ;
    bdDiscRequest.TargetLayer     = psn.Layer;
    bdDiscRequest.ClusterCount   =  1;

    bdInitializeHostBuffer(start_LBA.lba);
    buffer_result = bdStartDiscTransfer();
    SendMsgCnB(SEND_ATAPI_CMD,1,0x80041B,B1B(buffer_result)); // buffer_result

    if( buffer_result == BUF_GOOD)
    {
        StartTimer(&timer);
        while((DataBuffer.SectorCount < buffer_count) && (BD_BUFFERING() == ON))
        {
            ExitProcess();
            if(ReadTimer(&timer) > 100)
                break;
        }
    }
    else
    {
        return FALSE;
    }

    if(DataBuffer.SectorCount >= buffer_count)// check buffer again
        buffer_result = TRUE;
    if(buffer_result)
    {
        mem_address = (BYTE*)((2048 * (start_LBA.lba - pdbGetBDClusterStart(start_LBA.lba))) + SCSDRAM_ADDRESS);

        for(i=0;i<32;i++)
        {
            datasum+=*(mem_address+i);
        }
        if(datasum>0)
        {
            SendMsgCnB(SEND_ATAPI_CMD,4,0x80041C,B4B(datasum)); // datasum
            return TRUE;
        }
        else
        {
            return FALSE;
        }
    }
    else
        return FALSE;
}
/*******************************************************************************

    FUNCTION        BDCheckLTHDisc

    DESCRIPTION     Check if BD-R LTH Disc

********************************************************************************/
#if (ENABLE_BDRLTH == 1)
void BDCheckLTHDisc(void)
{
    BOOL LTH_setting;

    if (svoIramVar.LTHDisclikely == 0)
        return; // NO LTH Disc

    send_msg80(SEND_SStpTask_MSG|SHOW_DEBUG_MSG, 0x7100AA);

    LTH_setting = svoIramVar.LTHDisc;
    #if (BCA_ENABLE == 1)
    /*Check LTH Disc in BCA*/
    if (BDReadBCA() == PASS)
    {
        send_msg5S(SHOW_DEBUG_MSG,"Read BD BCA OK");
    }
    else
    {
        send_msg5S(SHOW_DEBUG_MSG,"Read BD BCA FAIL");
    }
    #endif

    /* LTH Disc, Readjust Servo*/
    if (svoIramVar.LTHDisc != LTH_setting)
    {
        SendMsgCn(SEND_SStpTask_MSG|SHOW_DEBUG_MSG,0,0x420017 + svoIramVar.LTHDisc); //BD Read - LTH(HTL) BD-R disc, Start to re-adjust Servo

        if (ServoReAdjust(svoVar_discKindT.fDiscKind) != PASS)
        {
            SendMsgCn(SEND_SStpTask_MSG|SHOW_DEBUG_MSG,0,0x420019); //BD Read, Re-adjust Servo NG
        }
    }
    svoIramVar.LTHDisclikely = 0;
}
#endif //(ENABLE_BDRLTH == 1)

/*******************************************************************************

    FUNCTION        BDCheckIHDisc

    DESCRIPTION     Check if BD IH Disc

********************************************************************************/
#if (ENABLE_BDIH == 1)
void BDCheckIHDisc(void)
{
    BOOL IH_setting;

    if (svoIramVar.IHDisclikely == 0)
    {
        send_msg5S(SHOW_DEBUG_MSG,"BDIH unlikely");
        return; // NO IH Disc
    }

    send_msg5S(SHOW_DEBUG_MSG,"BDIH likely");

    IH_setting = svoIramVar.IHDisc;
    #if (BCA_ENABLE == 1)
    /*Check LTH Disc in BCA*/
    if (BDReadBCA() == PASS)
    {
        send_msg5S(SHOW_DEBUG_MSG,"BDIH-Read BD BCA OK");
    }
    else
    {
        send_msg5S(SHOW_DEBUG_MSG,"BDIH-Read BD BCA FAIL");
    }
    #endif

    /* IH Disc, Readjust Servo*/
    if (svoIramVar.IHDisc != IH_setting)
    {
        SendMsgCn(SEND_SStpTask_MSG|SHOW_DEBUG_MSG,0,0x420017 + svoIramVar.LTHDisc); //BD Read - LTH(HTL) BD-R disc, Start to re-adjust Servo

        if (ServoReAdjust(svoVar_discKindT.fDiscKind) != PASS)
        {
            SendMsgCn(SEND_SStpTask_MSG|SHOW_DEBUG_MSG,0,0x420019); //BD Read, Re-adjust Servo NG
        }
    }
}
#endif //(ENABLE_BDRLTH == 1)
#endif  //(BD_ENABLE)

