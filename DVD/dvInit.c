/****************************************************************************
*                (c) Copyright 2001 - 2008 SUNEXT DESIGN, INC.
*                           All Right Reserved
*
*
* FILENAME      dvInit.c
*
* DESCRIPTION   This file contains the initialization routines for a power-on,
*               reset, or DVD disc detection.
*
* $Revision: 330 $
* $Date: 11/03/17 8:48p $
*
*
**************************** SUNEXT CONFIDENTIAL ****************************/


#include ".\common\globals.h"
#include ".\common\globtype.h"
#include ".\common\chip.h"
#include ".\hif\sv_util.h"
#include ".\hif\gp_util.h"
#include ".\hif\gp_init.h"
#include ".\hif\rd_util.h"
#include ".\hif\mmc.h"
#include ".\hif\gp_key.h"
#include ".\dvd\dvInit.h"
#include ".\dvd\dvgvars.h"
#include ".\dvd\dvdiscinit.h"
#include ".\player\plrdb.h"

#if (ENABLE_AACS == 1)
#include ".\crypt\aacs.h"
#endif

#include ".\oem\oem_spd_tbl.h"
#if (DVD_RAM_READ == 1)
#include ".\servo\svo_speed.h"
#include ".\defmgmt\dvdram\dvDefMgr.h"
#include ".\defmgmt\common\mrw_ram.h"
#endif

#include ".\al\lal.h"
#include ".\al\reg_hal.h"
#include ".\al\reg_dvd_decoder.h"
#include ".\al\reg_dvd_encoder.h"
#include ".\al\reg_cd_encoder.h"
#include ".\al\reg_cd_decoder.h"
#include ".\al\reg_intctrl.h"
#include ".\al\reg_global.h"
#include ".\al\HAL_dfe.h"

#include ".\common\intctl.h"

#include ".\dvd\dvMacros.h"

#if (EN_POWER_CONTROL)
#include ".\hif\powerctl.h"
#endif

void DvdCheckBookType(eDiscKind DvdDiscKind);

BYTE gDVDReAdjustOK;
BYTE gDspInitForSyncErr;

#if (BCA_ENABLE == 1)
extern BYTE gBcaInfoAvailable;
#endif

/*******************************************************************************
    INTERNAL FUNCTIONS
********************************************************************************/
void LPP2DiscInfo(void);
void InitializeDVDSdramBuffer(void);

/*******************************************************************************

    FUNCTION        dvdDspSystemInit

    DESCRIPTION     Initializes DVD DSP registers on system power on.

********************************************************************************/
extern BYTE Iram_EncoderStarted;
void dvdDspSystemInit(void)
{
#if (DVD_RAM_READ == 1)
    int i;
    #if (CHIP_REV >= 0xC1)
    BYTE temp_h, temp_l;
    #endif

    if (DVDRAM_rewritable)
    {
        if (Iram_EncoderStarted)
        return;

        // When DVD-decoder is reset, LAND state is cleared to LOW.
        // Prevent LAND state from changed after reset of DVD decoder, save current LAND state
        // and restore it after finish of reset.

        DISABLE_INTERRUPT();

    #if (CHIP_REV >= 0xC1)
        temp_h = READ_REG(HAL_DVDDREG_MAP->PIDMESURH);
        temp_l = READ_REG(HAL_DVDDREG_MAP->PIDMESURL);
    #endif

        /* Synchronize to header. */
        i = 0;
        while ((READ_FIELD(HAL_DVDD_DVDRAMHDR) == 0) && (i < 5000)) i++;    // Wait for start of header

        i = 0;
        while (READ_FIELD(HAL_DVDD_DVDRAMHDR) && (i < 1000)) i++;           // Wait for end of header

        // Save current LAND state.
        i = READ_FIELD(HAL_DVDD_LAND);

    #if (CHIP_REV >= 0xC0) && ((DVDRAM_FREEZE_FLYWHEEL == 1) || (DVDRAM_FREEZE_FLYWHEEL == 2))
        if (land_flywheel_state == LAND_FLYWHEEL_FREEZE)
        {
            land_flywheel_state = LAND_FLYWHEEL_HUNT;
            WRITE_FIELD(HAL_DVDD_TRACKCTRL, 0);
            WRITE_FIELD(HAL_WBL_HDRFLYRD, 0);
        }
    #endif
    }
#endif

    /*-------------------------------------------------------
            Reset the DVD DSP(Reset the DVD Decoder)
    --------------------------------------------------------*/
    WRITE_REG(HAL_DVDDREG_MAP->DVDRST,(HAL_DVDD_RSTDVDD|HAL_DVDD_RSTCSS));
    WRITE_REG(HAL_DVDDREG_MAP->DVDRST,0x00);
#if (DVD_RAM_READ == 1)
    if (DVDRAM_rewritable)
    {
        // Restore LAND state.
        WRITE_FIELD(HAL_DVDD_LANDOUT,i);
        WRITE_FIELD(HAL_DVDD_MANLAND,1);
        WRITE_FIELD(HAL_DVDD_MANLAND,0);

        ENABLE_INTERRUPT();
    }
#endif
    /*-------------------------------------------------------
            Insure all DVD interrupts are disabled for when
            the DVD DSP is turned on.
    --------------------------------------------------------*/
        DISABLE_DVD_DECODER_INTS();

    /*-------------------------------------------------------
           Configure the DVD Buffer for data, pi/po
           and defect management areas
    --------------------------------------------------------*/
    WRITE_FIELD(HAL_DVDD_STARTBK,DVD_START_BLOCK);
    WRITE_FIELD(HAL_DVDD_ENDBK,DVD_END_BLOCK);
    WRITE_FIELD(HAL_DVDD_PIPOSRT,DVD_PIPO_START_BLOCK);
    WRITE_FIELD(HAL_DVDD_PDLSRT,DVD_RAM_PDL_START_BLOCK);

    /*-------------------------------------------------------
           Enable DVD system for Error Checking and Target
           Matching.  Initial with DVD Read Long disabled
    --------------------------------------------------------*/
    WRITE_REG(HAL_DVDDREG_MAP->DVDCTRL,0x00);
    WRITE_FIELD(HAL_DVDD_BUFMODE, 1);

    /*-------------------------------------------------------
           Set read control registers
    --------------------------------------------------------*/

    // CSS Scrambling function shall be turned on just
    // before the data to be scrambled is transfered to the Host.
    // After the scrambled data transfer is completed, the Scrambling
    // function shall be turned off again.
    WRITE_FIELD(HAL_HST_CSSSCREN, 0);   // Disable CSS Scrambling

#if (DVD_RAM_READ == 1)
    if (DVDRAM)
    {
    #if (Zero_Multipass_For_Write_Verify == 1)
        if(RWDisc.dvRAM.VERIFY_MODE == TRUE)
        {
            WRITE_FIELD(HAL_DVDD_ECCCNT,ZERO_MULTI_PASS_COUNT);
        }
        else
    #endif
    	{
            WRITE_FIELD(HAL_DVDD_ECCCNT,DVDRAM_MULTI_PASS_COUNT);
    	}
        set_DVDRAM_decoder_registers();
    #if (CHIP_REV >= 0xC1)
        WRITE_REG(HAL_DVDDREG_MAP->PIDMESURH, temp_h);
        WRITE_REG(HAL_DVDDREG_MAP->PIDMESURL, temp_l);
    #endif
    }
    else
#endif
    {
        WRITE_FIELD(HAL_DVDD_ECCCNT,DVD_MULTI_PASS_COUNT);
    }
    WRITE_FIELD(HAL_DVDD_PISTH,DVD_PI_ROW_ERROR_THRESHOLD); /*PI ROW Error Threshold*/
    WRITE_FIELD(HAL_DVDD_CIDERRTH, DVD_CID_ERROR_THRESHOLD);

    WRITE_FIELD(HAL_DVDD_SEEDMISTHR,2);

    /*-------------------------------------------------------
        Turn on the DVD decoder
    --------------------------------------------------------*/
    WRITE_FIELD(HAL_DVDD_ENDVDD, 1);
    //send_msg5S(1, "DspInit");
    send_msg80(1,0x71012C);
}

/*******************************************************************************

    FUNCTION        dvdDspDiscDetectedInit

    DESCRIPTION     Initializes DVD DSP registers on detection of a DVD disc.

********************************************************************************/
void dvdDspDiscDetectedInit()
{

    /*-------------------------------------------------------
        Disable CD decoder/encoder functions or ints
    --------------------------------------------------------*/
    DISABLE_CDINT();                    /* Disable CD interrupts */
    WRITE_FIELD(HAL_CDD_SCEN, 0);       /* Disable subcode logic for CD decoder */
    WRITE_FIELD(HAL_CDD_SCOREN, 0);     /* Disable SUB-Q interrupt */

    /*-------------------------------------------------------
        Enable DVD decoder
    --------------------------------------------------------*/
    if ((pdbGetDiscKind() == eDVDRAMDisc) && (svoVar_TrackingMethod == TRK_METHOD_DPP))
    {
        /*-------------------------------------------------------
           If DVD RAM disc set the DVD DSP for DVD RAM functions
        --------------------------------------------------------*/
        WRITE_FIELD(HAL_DVDD_SLIPEN, 0);   //  Disable the Sector Slip mechanism in the decoder.
    }

    /*-------------------------------------------------------
           Set number of error correction passes
    --------------------------------------------------------*/
    WRITE_FIELD(HAL_DVDD_ECCCNT, DVD_MULTI_PASS_COUNT);

    /*-------------------------------------------------------
           Set period for block interrupts.
    --------------------------------------------------------*/
    WRITE_FIELD(HAL_DVDD_ECCIPRD, DVD_ECC_INTERRUPT_PERIOD);

    /*-------------------------------------------------------
           For DVD set burst size to 64 bytes
    --------------------------------------------------------*/
    WRITE_REG(HAL_HSTREG_MAP->RXFIFOC, 0x18); //RBSize_64

    /*-------------------------------------------------------
           Disable DVD-Decoder buffer overrun protection
           Disable ID error correction (after PI/PO)
    --------------------------------------------------------*/
    WRITE_FIELD(HAL_DVDD_BUFMODE, 1);
}

/*******************************************************************************

    FUNCTION        DvdVarsInit

    DESCRIPTION     Initializes DVD variables

********************************************************************************/
void DvdVarsInit(void)
{
#if (E_DIAGS == 1)
    /*-------------------------------------------------------
        Initial PI PO read flag for error gathering code
    --------------------------------------------------------*/
        READ_PI_PO = 0;
#endif

#if (VCPS_TEST ==1)
    VCPS_disc = FALSE;
    vcpsDKB.State = DKB_NOT_READ;
    vcpsData.DKBLocation = DKB_UNDETERMINED;
#endif

    /*-------------------------------------------------------
        Initial all dvd flags
    --------------------------------------------------------*/
    dvdInitFlags();

    /*-------------------------------------------------------
        Initialize last read ahead target block
    --------------------------------------------------------*/
    dvdLastReadAheadTarget = 0xFFFFFFFF;

#if (DVD_RAM_READ == 1)
    RWDisc.dvRAM.BgPreBuffer = 0;
#endif
}

/*******************************************************************************

    FUNCTION        dvdStartUpSuccessful

    DESCRIPTION     Sets variables and values for a successful start up and
                    read lead in of a DVD disc.

********************************************************************************/
void dvdStartUpSuccessful()
{
    /*-------------------------------------------------------------------
        Set LED
    --------------------------------------------------------------------*/
    ReadyLED(LED_ON);

#if (Detect_AVCHD_DISC == 1)
    if (pdbServoGetDiscType() == eADIPDisc && AVCHDFlag == TRUE)
    {
        DvdCheckBookType(pdbGetDiscKind());
    }
#endif

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
#endif /* (LED_NUM == 1) */

#if (LED_NUM == 2)
        ReadyLED(LED_ON);
#endif /* LED_NUM == 2 */

    St_drv_ready    = TRUE;                 /* Indicate media is accessible */
    St_no_disc      = FALSE;                /* Clear no disc flag */
    st_disc_change    = TRUE;               /* Mark disc changed */
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
    ataMediaIsChanged = TRUE;               /* for ATA command ataGetMediaStatus() in at_cmds.c */
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

/*******************************************************************************

    FUNCTION        dvdVerifyDisc

    DESCRIPTION     Verifies if the player DB has the correct DVD disc kind

    RETURN          TRUE    PlayerDB contains correct Disc Kind information,
                    FALSE   PlayerDB had incorrect Disc Kind information and
                            was changed to proper disc kind
********************************************************************************/
BOOL dvdVerifyDisc()
{
    discKindT DiscKind, DetectedDisc;
    BYTE  NumOfPhy, DiscType;

    DiscKind = pdbServoGetDiscKind();
    /*-------------------------------------------------------------------
       Determine Disc type from lead in information
    --------------------------------------------------------------------*/
    DetectedDisc.Bit.Layers = eLayersSingle;
    DetectedDisc.Bit.DiscStd = eDiscDVD;

    switch (pdbGetDVDType())
    {
        case DVD_ROM:
            if(DiscKind.Bit.Media != eMediaROM)
            {
                //== start == FWtester2  Item13 No.12/13/14 DVD+R SL closed/DL open/DL closed compare error
                //Number of physical format information bytes in use
                NumOfPhy = (*(BYTE *)(SCSDRAM_START_ADDR + DVD_PHYSICAL_FORMAT_INFORMATION+31));
                if (pdbGetDVDLayers() & 0x03)//DL
                {
                    if (pdbGetDVDLayers() & 0x01)//PTP/OTP
                    {
                        if ((NumOfPhy == 0x40) || (NumOfPhy == 0x37))//Plus R
                            DetectedDisc.fDiscKind = eDVDPlusRDLDisc;
                        else
                            DetectedDisc.fDiscKind = eDVDROMDisc;
                    }
                    else
                    {
                        DetectedDisc.fDiscKind = eDVDROMDisc;
                    }
                }
                else
                {
                    if ((NumOfPhy == 0x39) || (NumOfPhy == 0x20))//Plus RW
                    {
                        DetectedDisc.fDiscKind = eDVDPlusRWDisc;
                    }
                    else if ((NumOfPhy == 0x38) || (NumOfPhy == 0x37))
                    {
                        DetectedDisc.fDiscKind = eDVDPlusRDisc;
                    }
                    else
                    {
                        DetectedDisc.fDiscKind = eDVDROMDisc;
                    }
                }
                //== end
                DvdCheckBookType(DetectedDisc.fDiscKind);
            }
            else
            {
                DetectedDisc.fDiscKind = eDVDROMDisc;
            }
            break;

        case DVD_RAM:
            DetectedDisc.fDiscKind = eDVDRAMDisc;
            break;

        case DVD_PLUS_R:
            DetectedDisc.fDiscKind = eDVDPlusRDisc;
            break;

        case DVD_PLUS_R_DUAL:
            DetectedDisc.fDiscKind = eDVDPlusRDLDisc;
            break;

        case DVD_PLUS_RW:
            DetectedDisc.fDiscKind = eDVDPlusRWDisc;
            break;

        case DVD_PLUS_RW_DUAL:
            DetectedDisc.fDiscKind = eDVDPlusRWDLDisc;
            break;

        case DVD_MINUS_R:
            if (pdbGetDVDLayers() == DUAL_LAYER)
                DetectedDisc.fDiscKind = eDVDMinusRDLDisc;
            else
                DetectedDisc.fDiscKind = eDVDMinusRDisc;
            break;

        case DVD_MINUS_RW:
            if (pdbGetDVDLayers() == DUAL_LAYER)
                DetectedDisc.fDiscKind = eDVDMinusRWDLDisc;
            else
                DetectedDisc.fDiscKind = eDVDMinusRWDisc;
            break;

        default:
            DetectedDisc.fDiscKind = eUnknownKind;
    }


    /*-------------------------------------------------------------------
       If this is a dual layer disc insure dual layer identifier
       is set in disc kind
    --------------------------------------------------------------------*/
    if(pdbGetDVDLayers() == DUAL_LAYER)
        DetectedDisc.Bit.Layers = eLayersDouble;

    ForBCALoadedDiscKind.fDiscKind = DetectedDisc.fDiscKind;

    if(DetectedDisc.fDiscKind != eDVDRAMDisc)
    {
        pdbSetDiscKind(DetectedDisc.fDiscKind);

        DetectedDisc.fDiscKind = eDVDROMDisc; // force as ROM

        if(pdbGetDVDLayers() == DUAL_LAYER)
            DetectedDisc.Bit.Layers = eLayersDouble;
    }

    /*-------------------------------------------------------------------
        Check for mis-detection
    --------------------------------------------------------------------*/
    gDVDReAdjustOK = FALSE;

    SendMsgCn(SEND_SStpTask_MSG,2,0x800492, A2B(DetectedDisc.fDiscKind));
    pdbServoSetfDiscKind(DetectedDisc.fDiscKind);
    DiscType = eNonADIPLPPDisc;
    switch (pdbGetDiscKind())
    {
        case eDVDPlusRDisc:
        case eDVDPlusRDLDisc:
        case eDVDPlusRWDisc:
        case eDVDPlusRWDLDisc:
            DiscType = eADIPDisc;
            break;

        case eDVDMinusRDisc:
        case eDVDMinusRDLDisc:
        case eDVDMinusRWDisc:
        case eDVDMinusRWDLDisc:
            DiscType = eLPPDisc;
            break;
    }
    pdbServoSetDiscType(DiscType);
    return(PASS);
}
/*******************************************************************************

    FUNCTION        dvdDiscSetup

    DESCRIPTION     Manages the appropriate calls to gather disc information based
                    on the DVD disc kind.

    RETURNS         PASS    A DVD disc kind is verified and all format information
                            has been gathered and stored. This status implies that
                            the disc is properly mounted and can be accessed

                    FAIL    DVD disc could not be verified. This status implies
                            that the disc is not mounted and cannot be accessed

    NOTES

********************************************************************************/
BYTE MinusNoEmbossedArea;
BYTE dvdDiscSetup(void)
{
#if (VCPS_TEST == 1)
    BYTE FlagByte;
#endif
    eDiscKind disc;
    BYTE TOC_ReadFail;
    BYTE *DataPtr;
    DiscAccessStatusE DiscStatus;

    dvdDspDiscDetectedInit();       /*  Initialize necessary general DVD components */

    /* initial DVD Speed Control before Start Up success */
#if (DVD_RAM_READ == 1)
    if(pdbGetDiscKind() == eDVDRAMDisc)
        Disc_Speed.Read = MAX_READ_RAM_SPEED;
    else
#endif
        Disc_Speed.Read = MAX_READ_DVD_SPEED;

    // Obtain the disc kind
    disc = pdbGetDiscKind();

#if (EN_POWER_CONTROL)
    SetDiscReadPowerMode(); /* Initialize Chip to DVD power */
#endif

#if (BCA_ENABLE == 1)
    gBcaInfoAvailable = FALSE;
#endif

    UpdateDiscInfoData();    /* Update general disc information  */

    TOC_ReadFail = FALSE;
    MinusNoEmbossedArea = FALSE;

    //-------------------------------------------------------------
    // Stop bufferring and clear data buffer
    //-------------------------------------------------------------
    if( DVD_BUFFERING() == ON)
        dvdStopDiscTransfer();

    ClearDataBuffer();

    //-------------------------------------------------------------
    // Read a control data block from the control data zone
    //-------------------------------------------------------------
    DiscStatus = dvdBufferControlDataBlock(0, START_ADDRESS_OF_CONTROL_DATA);
    if (DiscStatus == BUF_GOOD)
    {
        /* Get the absolute address of the control data block */
        DataPtr = (BYTE *)CUR_ECC_ADDRESS();

        /* Save the pre-recorded physical format information of the CDZ in Lead-in */
        pdbSetDVDLogicalFormatInfo(DataPtr);

        /* Update physical format information in dvd database for layer 0 */
        pdbSetDVDPhysicalFormatInfo(DataPtr);

        /* Obtain CPRM information */
        #if (ENABLE_CPRM == 1 && BCA_ENABLE == 1)
        if (pdbGetDVDType() == DVD_MINUS_R || pdbGetDVDType() == DVD_MINUS_RW || pdbGetDVDType() == DVD_RAM)
            dvdSetCPRMInfo(READ_FIELD(HAL_DVDD_CURECC));
        #endif

        /* Verify if the disc kind from lead in. (Adjusts if necessary) */
        if (dvdVerifyDisc() == FAIL || gDVDReAdjustOK == TRUE)
            return FAIL;
    }
    else
        return FAIL;

    disc = pdbGetDiscKind();
    TOC_ReadFail = TRUE;

    /* Read the lead in */
    if (dvdReadControlDataZone(TOC_ReadFail) == BUF_GOOD )
    {
        //send_msg5S(SEND_READ_DVD_MSG, "RdCtrlData OK!");
        send_msg80(SEND_READ_DVD_MSG,0x71012D);
#if (DVD_RAM_READ == 1)
        if(disc != eDVDRAMDisc)
#endif
        {
            // move ServoSeek(0x30000L,eLayer0)&ServoPause() from dvdReadControlDataZone()'s "if (DiscReadStatus == BUF_GOOD)"  to dvdDiscSetup()'s "RdCtrlData OK!"
            ServoSeek(0x30000L,eLayer0);
            ServoPause();                /* execute pause operation */
        }
        /* Update blank disc status flag */
        St_blank_disc = pdbGetDVDBlankDiscInfo();
#if (CEProduct == 1)
        dvdCheckVideo();
#endif

#if (Detect_AVCHD_DISC == 1)
        dvdCheckAVCHD();
#endif
        switch(disc)
        {
            case eDVDROMDisc :
            case eDVDROMDLDisc :
#if (CEProduct == 0)
                dvdCheckVideo();
#endif
                break;

            case eDVDMinusRDisc:
            case eDVDMinusRDLDisc:
                 break;

            case eDVDMinusRWDisc :
            case eDVDMinusRWDLDisc :
            case eDVDPlusRDisc :
            case eDVDPlusRDLDisc:
            case eDVDPlusRWDisc :
            case eDVDPlusRWDLDisc :
            case eDVDRAMDisc:
                 break;
        }
    }
    else
    {
        //send_msg5S(SEND_READ_DVD_MSG, "RdCtrlData NG!");
        send_msg80(SEND_READ_DVD_MSG,0x71012E);
        return FAIL;  /* If lead-in expected but could not read it fail the disc startup */
    }

#if (VCPS_TEST == 1)
    if (VcpsKeysValid == TRUE && VCPS_disc == TRUE)
    {
        if (GetVcpsInfoInAdip(EFI_TOC_START_ADDRESS, VCPS_ETOC_SIZE, ADIP_INFO_ETOC) == FALSE)
            if (vcpsData.DKBLocation == DKB_ADIP)
                VCPS_disc = FALSE;
    }
#endif

#if (DVD_RAM_READ == 1)
    /*-------------------------------------------------------
        READ DVD RAM DEFECT LIST
    --------------------------------------------------------*/
    if (pdbGetDiscKind() == eDVDRAMDisc)
    {
        if ( DvdRamStartUp() != TRUE )  /* Read DMA (Defect lists) off the disc and enable defect slipping hardware. */
            return FAIL;
        WRITE_FIELD(HAL_DVDD_ECCCNT,DVDRAM_MULTI_PASS_COUNT);
    #if (ENABLE_RAM_DFE_ICE_DETECT == 1)
        ClearDVDRAMIceIndicatorAll();
    #endif
    }
#endif

    if (St_door_open == TRUE || TrayManagerInfo.Executing)//Startup time exceed 40sec, AP WinFinal send EjectDisc command
        return FAIL;

#if (ENABLE_AVCREC == 1)
    disc = pdbGetDiscKind();
    if ((disc != eDVDROMDisc) && (disc != eDVDROMDLDisc))
        AacsStatus.AacsFlag.IsMediumCompliant = TRUE;
#endif
    /*-------------------------------------------------------
        Set the speed mode
    --------------------------------------------------------*/
#if (Detect_AVCHD_DISC == 1)
    if (AVCHDFlag == TRUE)
        SetReadSpeed(MAX_READ_AVCHD_SPEED);
    else
#endif
    SetReadSpeed(eDVDSpinUpSpeed);  //sync SVO speed

    return PASS;
}

/*----------------------------------------------------------------------------
  InitializeDVDVariables()

  Initialize internal memory variables and flags
----------------------------------------------------------------------------*/
void InitializeDVDSdramBuffer(void)
{
    vBOOL temp_IE;

    temp_IE = GET_INT_ENABLE_MASK();
    DISABLE_INTERRUPT();

    Iram_usiRingSP  = 0;
    RING_END_PAGE  = DVD_END_BLOCK;
    Iram_usiRingEP  = RING_END_PAGE;
    BUF_TOTAL_BLK_COUNT = RING_END_PAGE + 1;

    WRITE_FIELD(HAL_HST_HSTBUFSPG, Iram_usiRingSP);          /* host start blk. */
    WRITE_FIELD(HAL_DVDD_STARTBK, Iram_usiRingSP);           /* DVD ring start blk. */
    WRITE_FIELD(HAL_DVDD_PIPOSRT, DVD_PIPO_START_BLOCK);     /* PI/PO start blk. */
    WRITE_FIELD(HAL_DVDD_PDLSRT, DVD_RAM_PDL_START_BLOCK);   /* DVDRAM PDL */
    WRITE_FIELD(HAL_HST_HSTBUFEPG, Iram_usiRingEP);          /* Host end blk. */
    WRITE_FIELD(HAL_DVDD_ENDBK, Iram_usiRingEP);             /* DVD ring end blk. */

    InitCopyStartEndPages();                                /* Initialize Copy block Start and End pages */

    SET_INT_ENABLE_MASK(temp_IE);
}

/*----------------------------------------------------------------------------
  DvdCheckBookType()
----------------------------------------------------------------------------*/
void DvdCheckBookType(eDiscKind DvdDiscKind)
{
        tPhysicalFormatInformationS *DataPtr;
        BYTE booktype;

        switch (DvdDiscKind)
        {
            case eDVDPlusRDisc:
                booktype = DVD_PLUS_R;
                break;
            case eDVDPlusRWDisc:
                booktype = DVD_PLUS_RW;
                break;
            case eDVDPlusRDLDisc:
                booktype = DVD_PLUS_R_DUAL;
                break;
            case eDVDPlusRWDLDisc:
                booktype = DVD_PLUS_RW_DUAL;
                break;
            default:
                return;
        }
        DataPtr = (tPhysicalFormatInformationS *) (SCSDRAM_ADDRESS + DVD_LOGICAL_FORMAT_INFORMATION);
        DataPtr->DiscTypeVersion = (DataPtr->DiscTypeVersion & 0x0F) + booktype;
        pdbSetDVDType(booktype);

}
