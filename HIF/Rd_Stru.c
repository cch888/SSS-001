/*****************************************************************************
*                (c) Copyright 2000 - 2008 SUNEXT DESIGN, INC.
*                            All Rights Reserved
*
*
* FILENAME:     $Workfile: Rd_Stru.c $
*
* DESCRIPTION   This file contains Read DVD Structure command - Generic Disc Structure
*
* $Revision: 39 $
* $Date: 11/01/17 6:30p $
*
**************************** SUNEXT CONFIDENTIAL ****************************/

/*********************************************************************
  INCLUDE FILES
*********************************************************************/
#include ".\common\globals.h"
#include ".\common\globtype.h"
#include ".\oem\oem_info.h"

#include ".\hif\Rd_Stru.h"

#include ".\player\Plrdb.h"
#include ".\dvd\dvGvars.h"
#include ".\dvd\dvMacros.h"
#if (BD_ENABLE)
#include ".\bd\bdDef.h"
#include ".\bd\bdMacro.h"
#include ".\bd\bdvars.h"
#endif  /* BD_ENABLE */

#if (ENABLE_AACS == 1)
#include ".\crypt\aacs.h"
#include ".\crypt\cmac.h"
#endif

const STRU_FMT_LIST_T StruFmtList[] =
{
    /*   Fmt    SDS/RDS    Length
        -----   -------   --------  */
        0x00,    0x40,     0x0804,
        0x01,    0x40,     0x0008,
        0x02,    0x40,     0x0804,
        0x03,    0x40,     0x00C0,
        0x04,    0xC0,     0x0804,
        0x05,    0xC0,     0x0008,
        0x06,    0x40,     0x0018,
        0x07,    0x40,     0x6004,
#if (DVD_RAM_READ == 1)
        0x08,    0x40,     0x0804,
        0x09,    0xC0,     0x0008,
        0x0A,    0x40,     0x0010,
        0x0B,    0x40,     0x0008,
#endif
#if (BD_ENABLE == 1)
        0x80,    0x40,     0x0020,
        0x81,    0x40,     0x0020,
        0x82,    0x40,     0x0020,
#endif
        0xFF,    0x00,     0x0000
};


#ifdef SEC_BDP_AUTH                             // Enable SEC BD-Player Authentication
extern BYTE ReadDiscStructureCmd_AACS(void);
#endif

/*******************************************************************************

    FUNCTION        ReadGenericDiscStructure

    DESCRIPTION     Read Generic Disc Structure command handler

    RETURNS         None

********************************************************************************/
BYTE ReadGenericDiscStructure(void)
{
    STRU_FMT_LIST_T    const    *base;
    WORD        HostTransferLength;
    USHORT      i;
    BYTE       *DramAddressP;
    BYTE        SentAgid;
    BYTE        FormatField;
    BYTE        MediaType;
    BYTE        CmdValid;
    BYTE        Layers;
#if (BD_ENABLE)
    BYTE        LayerFormat;
#endif
#if (ENABLE_AACS == 1)
    BYTE        Mac[16];
#endif  /* ENABLE_AACS == 1 */
#if (CEProduct == 1)
    BYTE        VendorID;
#endif

    if (pdbDiscLoaded() == BD_DISC)
    {
#if (BD_ENABLE == 1)
        if( BD_BUFFERING() == ON )
        {
            bdStopDiscTransfer();
            Svr_pause_reqire = TRUE;
        }
        bdDecFlag.fDISABLE_READ_AHEAD = TRUE;
#endif  /* BD_ENABLE == 1 */
    }
#if (ENABLE_HDDVD == 1)
    else if (pdbDiscLoaded() == HD_DISC)
    {
        if( DVD_BUFFERING() == ON )
        {
            dvdStopDiscTransfer();
            Svr_pause_reqire = TRUE;
        }
        dvdDecFlag.fDISABLE_READ_AHEAD = TRUE;
    }
#endif  /* ENABLE_HDDVD == 1 */
    else if (pdbDiscLoaded() == DVD_DISC)
    {
        if( DVD_BUFFERING() == ON )
        {
            dvdStopDiscTransfer();
            Svr_pause_reqire = TRUE;
        }
        dvdDecFlag.fDISABLE_READ_AHEAD = TRUE;
    }
    else if (pdbDiscLoaded() == CD_DISC)
    {
        return FALSE;
    }

    ClearDataBuffer();

    BHIGH(HostTransferLength) = Iram_Cmd_block.bf[8];  // Get Allocation length from Command Descriptor Block
    BLOW (HostTransferLength) = Iram_Cmd_block.bf[9];

    /* Media Type */
    MediaType = Iram_Cmd_block.bf[1] & 0x0F;

    /* Get Format */
    FormatField = Iram_Cmd_block.bf[7];

    /* Get AGID */
    SentAgid = (Iram_Cmd_block.bf[10] & 0xC0) >> 6;

#ifdef SEC_BDP_AUTH                             // Enable SEC BD-Player Authentication
    if (ReadDiscStructureCmd_AACS())            // Samsung AACS.
    {
        return TRUE;
    }
#endif

    if (FormatField < 0x80)     //independent media type
        return FALSE;

    CmdValid = TRUE;

    /* Check valid command */
    switch(pdbDiscLoaded())
    {
        #if (BD_ENABLE)
        case BD_DISC:
            Layers = pdbGetBDLayers();
            if (MediaType != 0x01 || Iram_Cmd_block.bf[6] > Layers)
                CmdValid = FALSE;

            break;
        #endif

        #if (ENABLE_HDDVD)
        case HD_DISC:
            if (MediaType != 0x00 || Iram_Cmd_block.bf[6] > Layers)
                CmdValid = FALSE;
            break;
        #endif

        case DVD_DISC:
            Layers = pdbGetDVDLayers();
        #if (ENABLE_AACS)
            if (Iram_Cmd_block.bf[6] > Layers)
        #else
            if (MediaType != 0x00 || Iram_Cmd_block.bf[6] > Layers)
        #endif
                CmdValid = FALSE;

        #if (ENABLE_AACS)
            if (MediaType != 0x00 && Info.Media.DVD.fCSS_PROTECTED != eAACS_CPS)
                CmdValid = FALSE;
            else
                CmdValid = TRUE;
        #endif

            break;
    }

    if (CmdValid == TRUE)
    {
        SentAgid = (Iram_Cmd_block.bf[10] & 0xC0) >> 6;

#if (CEProduct == 1)
        VendorID = (Iram_Cmd_block.bf[11] & 0xC0) >> 6;
#endif

        switch(FormatField)
        {
#if (ENABLE_AACS == 1)
        #if (CEProduct == 1)
            case GEN_BCM_AACS_VOLUM_ID:
        #endif  /* CEProduct == 1 */
            case GEN_AACS_VOLUM_ID:
                //-------------------------------------------------
                // AACS Volume Identifier
                //-------------------------------------------------

                #if (BD_ENABLE)
                if (pdbDiscLoaded() == BD_DISC && pdbGetBDType() != BD_ROM)
                {
                    BUILD_SENSE(0x05,0x30,0x00,0x02);    /* INCOMPATIBLE MEDIUM INSTALLED */
                    AutoCommandComplete();
                    return TRUE;
                }
                #endif

                if (pdbDiscLoaded() == DVD_DISC && pdbGetDVDType() != DVD_ROM)
                {
                    BUILD_SENSE(0x05,0x30,0x00,0x05);    /* INCOMPATIBLE MEDIUM INSTALLED */
                    AutoCommandComplete();
                    return TRUE;
                }

                if (AacsValidateAgid(SentAgid))
                {
                    if (AacsStatus.eAacsState != AACS_AUTHENTICATION_COMPLETE)
                    {
                        BUILD_SENSE(0x05,0x6F,0x02,0x03);              // COPY PROTECTION KEY EXCHANGE FAILURE - KEY NOT ESTABLISHED
                        AutoCommandComplete();
                        return TRUE;
                    }

                    // Generate Message Authentication Code(MAC) of Volume ID
                    GenerateCmac(&AacsAuth.BusKey[0], &AacsAuth.ID.VolumeID[0], sizeof(AacsAuth.ID.VolumeID), &Mac[0]);

                    DramAddressP       = (BYTE *) (SCSDRAM_ADDRESS + DVD_CSS_SCRATCH);   // Point to work area in DRAM
                    *DramAddressP++ = 0;        // Disc Structure
                    *DramAddressP++ = 0x22;     // Data Length.
                    *DramAddressP++ = 0;        // Reserved.
                    *DramAddressP++ = 0;        // Reserved.

                    // Volume ID Field
                    for (i = 0; i < sizeof(AacsAuth.ID.VolumeID); i++)
                    {
                    #if (CEProduct == 1)
                        if (FormatField == GEN_BCM_AACS_VOLUM_ID)
                            *DramAddressP++ = AacsAuth.eVolumeID[i];
                        else
                    #endif  /* CEProduct == 1 */
                            *DramAddressP++ = AacsAuth.ID.VolumeID[i];
                    }

                    // Message Authentication Code(MAC) Field
                    for (i = 0; i < sizeof(Mac); i++)
                        *DramAddressP++ = Mac[i];

                    // Return Data to Host
                    SendParameterDataToHost(DVD_CSS_SCRATCH, MINIMUM(36,HostTransferLength), FALSE);    // Transfer data complete.
                }
                else
                {
                    BUILD_SENSE(0x05,0x24,0x00,0xbe);           // INVALID FIELD IN CDB
                }
                break;

#if (BCA_ENABLE == 1)
        #if (CEProduct == 1)
            case GEN_BCM_AACS_MEDIA_SN:
        #endif
            case GEN_AACS_MEDIA_SN:
                //-------------------------------------------------
                // AACS Media serial number
                //-------------------------------------------------
    #if (BD_ENABLE == 1)
                if (pdbGetDiscKind() == eBDROMDisc || pdbGetDiscKind() == eBDROMDLDisc)
                {
                    if (BDReadBCA() == FAIL)
                    {
                        BUILD_SENSE(0x05,0x6F,0x02,0x11);              // COPY PROTECTION KEY EXCHANGE FAILURE - KEY NOT ESTABLISHED
                        AutoCommandComplete();
                        return TRUE;
                    }
                }
    #endif

                if (AacsValidateAgid(SentAgid))
                {
                    if (AacsStatus.eAacsState != AACS_AUTHENTICATION_COMPLETE)
                    {
                        BUILD_SENSE(0x05,0x6F,0x02,0x04);              // COPY PROTECTION KEY EXCHANGE FAILURE - KEY NOT ESTABLISHED
                        AutoCommandComplete();
                        return TRUE;
                    }

                    // Generate Message Authentication Code(MAC) of PMSN
                    GenerateCmac(&AacsAuth.BusKey[0], &AacsAuth.MediaSN[0], sizeof(AacsAuth.MediaSN), &Mac[0]);

                    // Encrypt PMSN
                    #if (CEProduct == 1)
                    if (FormatField == GEN_BCM_AACS_MEDIA_SN)
                        GenBcmMediaSN();
                    #endif  /* CEProduct == 1 */

                    DramAddressP       = (BYTE *) (SCSDRAM_ADDRESS + DVD_CSS_SCRATCH);   // Point to work area in DRAM
                    *DramAddressP++ = 0;        // Disc Structure
                    *DramAddressP++ = 0x22;     // Data Length.
                    *DramAddressP++ = 0;        // Reserved.
                    *DramAddressP++ = 0;        // Reserved.

                    // Pre-recorded Media Serial Number Field
                    for (i = 0; i < sizeof(AacsAuth.MediaSN); i++)
                    {
                    #if (CEProduct == 1)
                        if (FormatField == GEN_BCM_AACS_MEDIA_SN)
                            *DramAddressP++ = AacsAuth.eMediaSN[i];
                        else
                    #endif  /* CEProduct == 1 */
                        *DramAddressP++ = AacsAuth.MediaSN[i];
                    }

                    // Message Authentication Code(MAC) Field
                    for (i = 0; i < sizeof(Mac); i++)
                        *DramAddressP++ = Mac[i];

                    // Return Data to Host
                    SendParameterDataToHost(DVD_CSS_SCRATCH, MINIMUM(36,HostTransferLength), FALSE);    // Transfer data cmplete.

                    // Authentication Complete. Invalidates AGID and returns to initial state.
                    //AacsStatus.AacsFlag.AgidInUse = FALSE;
                    //AacsStatus.eAacsState = AACS_START;
                }
                else
                {
                    BUILD_SENSE(0x05,0x24,0x00,0xbf);           // INVALID FIELD IN CDB
                }
                break;

        #if (CEProduct == 1)
            case GEN_BCM_AACS_MEDIA_ID:
        #endif
            case GEN_AACS_MEDIA_ID:
                //-------------------------------------------------
                // AACS Media Identifer
                //-------------------------------------------------

    #if (BD_ENABLE)
                if (pdbDiscLoaded() == BD_DISC)
                {
                    if (pdbGetBDType() == BD_ROM)
                    {
                        BUILD_SENSE(0x05,0x30,0x00,0x03);    /* INCOMPATIBLE MEDIUM INSTALLED */
                        AutoCommandComplete();
                        return TRUE;
                    }
                    else if (BDReadBCA() == FAIL)
                    {
                        BUILD_SENSE(0x05,0x6F,0x02,0x10);              // COPY PROTECTION KEY EXCHANGE FAILURE - KEY NOT ESTABLISHED
                        AutoCommandComplete();
                        return TRUE;
                    }
                }
    #endif

    #if (ENABLE_AVCREC == 1)
                if (pdbDiscLoaded() == DVD_DISC)
                {
                    if (DVDReadBCA() == FAIL)
                    {
                        BUILD_SENSE(0x05,0x6F,0x02,0x10);              // COPY PROTECTION KEY EXCHANGE FAILURE - KEY NOT ESTABLISHED
                        AutoCommandComplete();
                        return TRUE;
                    }
                }
    #endif

                if (AacsValidateAgid(SentAgid))
                {
                    if (AacsStatus.eAacsState != AACS_AUTHENTICATION_COMPLETE)
                    {
                        BUILD_SENSE(0x05,0x6F,0x02,0x05);              // COPY PROTECTION KEY EXCHANGE FAILURE - KEY NOT ESTABLISHED
                        AutoCommandComplete();
                        return TRUE;
                    }

                    // Generate Message Authentication Code(MAC) of Media ID
                    GenerateCmac(&AacsAuth.BusKey[0], &AacsAuth.ID.MediaID[0], sizeof(AacsAuth.ID.MediaID), &Mac[0]);

                    // Encrypt Media ID
                    #if (CEProduct == 1)
                    if (FormatField == GEN_BCM_AACS_MEDIA_ID)
                        GenBcmMediaID();
                    #endif  /* CEProduct == 1 */

                    DramAddressP       = (BYTE *) (SCSDRAM_ADDRESS + DVD_CSS_SCRATCH);   // Point to work area in DRAM
                    *DramAddressP++ = 0;        // Disc Structure
                    *DramAddressP++ = 0x22;     // Data Length.
                    *DramAddressP++ = 0;        // Reserved.
                    *DramAddressP++ = 0;        // Reserved.

                    // Media ID Field
                    for (i = 0; i < sizeof(AacsAuth.ID.MediaID); i++)
                    {
                    #if (CEProduct == 1)
                        if (FormatField == GEN_BCM_AACS_MEDIA_ID)
                            *DramAddressP++ = AacsAuth.eMediaID[i];
                        else
                    #endif  /* CEProduct == 1 */
                        *DramAddressP++ = AacsAuth.ID.MediaID[i];
                    }

                    // Message Authentication Code(MAC) Field
                    for (i = 0; i < sizeof(Mac); i++)
                        *DramAddressP++ = Mac[i];

                    // Return Data to Host
                    SendParameterDataToHost(DVD_CSS_SCRATCH, MINIMUM(36,HostTransferLength), FALSE);    // Transfer data complete.

                    // Authentication Complete. Invalidates AGID and returns to initial state.
                    //AacsStatus.AacsFlag.AgidInUse = FALSE;
                    //AacsStatus.eAacsState = AACS_START;
                }
                else
                {
                    BUILD_SENSE(0x05,0x24,0x00,0xc0);           // INVALID FIELD IN CDB
                }
                break;
#endif  /* BCA_ENABLE == 1 */

#if (CEProduct == 0)
            case GEN_AACS_MKB:
                //-------------------------------------------------
                // AACS Media Key Block
                //-------------------------------------------------
                DramAddressP       = (BYTE *) (SCSDRAM_ADDRESS + DVD_CSS_SCRATCH);   // Point to work area in DRAM
                *DramAddressP++ = 0x80;     // Disc Structure
                *DramAddressP++ = 0x02;     // Data Length
                *DramAddressP++ = 0;        // Reserved
                *DramAddressP++ = 1;        // Total Packs

                // Transfer Header to Host first
                SendParameterDataToHost(DVD_CSS_SCRATCH, MINIMUM(4,HostTransferLength), FALSE);

                // Load MKB from flash into the data buffer
                LoadMkbFromFlash();

                // Return Data to Host
                SendParameterDataToHost(0, MINIMUM(32768,HostTransferLength), TRUE);    // Transfer data and send Command Complete.
                return TRUE;
#endif  /* CEProduct == 0 */

            #if (CEProduct == 1)
            case GEN_SIGMA_AACS_KCD:
            case GEN_BCM_AACS_KCD:
            case GEN_INTEL_AACS_KCD:
                //-------------------------------------------------
                // AACS Key Conversion Data
                //-------------------------------------------------

                #if (BD_ENABLE)
                if (pdbDiscLoaded() == BD_DISC && pdbGetBDType() != BD_ROM)
                {
                    BUILD_SENSE(0x05,0x30,0x00,0x04);    /* INCOMPATIBLE MEDIUM INSTALLED */
                    AutoCommandComplete();
                    return TRUE;
                }
                #endif

                if (FormatField == GEN_INTEL_AACS_KCD)
                {
                    GenerateCmacForKcd(&Mac[0], BE_TYPE_INTEL, &AacsAuth);

                    DramAddressP       = (BYTE *) (SCSDRAM_ADDRESS + DVD_CSS_SCRATCH);   // Point to work area in DRAM
                    *DramAddressP++ = 0;        // Disc Structure
                    *DramAddressP++ = 0x22;     // Data Length.
                    *DramAddressP++ = 0;        // Reserved.
                    *DramAddressP++ = 0;        // Reserved.

                    // KCD Field
                    for (i = 0; i < 16; i++)
                        *DramAddressP++ = AacsAuth.eKCD[i];

                    // Message Authentication Code(MAC) Field
                    for (i = 0; i < sizeof(Mac); i++)
                        *DramAddressP++ = Mac[i];

                    // Return Data to Host
                    SendParameterDataToHost(DVD_CSS_SCRATCH, MINIMUM(36,HostTransferLength), FALSE);    // Transfer data complete.
                    break;
                }

                if (AacsValidateAgid(SentAgid))
                {
                    if (AacsStatus.eAacsState != AACS_AUTHENTICATION_COMPLETE)
                    {
                        BUILD_SENSE(0x05,0x6F,0x02,0x09);              // COPY PROTECTION KEY EXCHANGE FAILURE - KEY NOT ESTABLISHED
                        AutoCommandComplete();
                        return TRUE;
                    }

                    // Generate Message Authentication Code(MAC) of KCD
                    if (FormatField == GEN_SIGMA_AACS_KCD)
                    {
                        if (VendorID == 0x02)
                            GenerateCmacForKcd(&Mac[0], BE_TYPE_SIGMA_2, &AacsAuth);
                        else
                            GenerateCmacForKcd(&Mac[0], BE_TYPE_SIGMA, &AacsAuth);
                    }
                    else if (FormatField == GEN_BCM_AACS_KCD)
                        GenerateCmacForKcd(&Mac[0], BE_TYPE_BCM, &AacsAuth);
                    else
                        GenerateCmacForKcd(&Mac[0], BE_TYPE_DEFAULT, &AacsAuth);

                    DramAddressP       = (BYTE *) (SCSDRAM_ADDRESS + DVD_CSS_SCRATCH);   // Point to work area in DRAM
                    *DramAddressP++ = 0;        // Disc Structure
                    if (FormatField == GEN_SIGMA_AACS_KCD)
                    {
                        if (VendorID == 0x02)
                            *DramAddressP++ = 0x22;     // Data Length.
                        else
                            *DramAddressP++ = 0x24;     // Data Length.
                    }
                    else
                        *DramAddressP++ = 0x22;     // Data Length.
                    *DramAddressP++ = 0;        // Reserved.
                    *DramAddressP++ = 0;        // Reserved.

                    // KCD Field
                    for (i = 0; i < 16; i++)
                    {
                        if (FormatField == GEN_SIGMA_AACS_KCD || FormatField == GEN_BCM_AACS_KCD)
                            *DramAddressP++ = AacsAuth.eKCD[i];
                        else
                            *DramAddressP++ = AacsAuth.KCD[i];
                    }

                    // Message Authentication Code(MAC) Field
                    for (i = 0; i < sizeof(Mac); i++)
                        *DramAddressP++ = Mac[i];

                    // Return Data to Host
                    SendParameterDataToHost(DVD_CSS_SCRATCH, MINIMUM(36,HostTransferLength), FALSE);    // Transfer data complete.
                }
                else
                {
                    BUILD_SENSE(0x05,0x24,0x00,0xbe);           // INVALID FIELD IN CDB
                }
                break;
            #else  /* CEProduct != 1 */
            case GEN_AACS_DATA_KEY:
                //-------------------------------------------------
                // AACS Data Key
                //-------------------------------------------------
                if (AacsStatus.AacsFlag.IsMediumCompliant == FALSE)
                {
                    BUILD_SENSE(0x05,0x6F,0x01,0x07);                                   /* COPY PROTECTION KEY EXCHANGE FAILURE - KEY NOT PRESENT */
                    AutoCommandComplete();
                    return TRUE;
                }

                if (AacsValidateAgid(SentAgid))
                {
                    if (AacsStatus.eAacsState != AACS_AUTHENTICATION_COMPLETE)
                    {
                        BUILD_SENSE(0x05,0x6F,0x02,0x08);              // COPY PROTECTION KEY EXCHANGE FAILURE - KEY NOT ESTABLISHED
                        AutoCommandComplete();
                        return TRUE;
                    }

                    DramAddressP       = (BYTE *) (SCSDRAM_ADDRESS + DVD_CSS_SCRATCH);   // Point to work area in DRAM
                    *DramAddressP++ = 0;        // Disc Structure
                    *DramAddressP++ = 0x22;     // Data Length.
                    *DramAddressP++ = 0;        // Reserved.
                    *DramAddressP++ = 0;        // Reserved.

                    // Read Data Key Field
                    for (i = 0; i < sizeof(AacsAuth.ReadDataKey); i++)
                        *DramAddressP++ = AacsAuth.ReadDataKey[i];

                    // Write Data Key Field
                    for (i = 0; i < sizeof(AacsAuth.WriteDataKey); i++)
                        *DramAddressP++ = AacsAuth.WriteDataKey[i];

                    // Return Data to Host
                    SendParameterDataToHost(DVD_CSS_SCRATCH, MINIMUM(36,HostTransferLength), FALSE);    // Transfer data complete.

                    // Authentication Complete. Invalidates AGID and returns to initial state.
                    //AacsStatus.AacsFlag.AgidInUse = FALSE;
                    //AacsStatus.eAacsState = AACS_START;
                }
                else
                {
                    BUILD_SENSE(0x05,0x24,0x00,0xcc);           // INVALID FIELD IN CDB
                }

                break;
            #endif  /* CEProduct == 1 */

            case GEN_AACS_BEF_SECTOR_EXTENTS:
                //-------------------------------------------------
                // AACS Bus Encryption Sector Extents
                //-------------------------------------------------
                break;
#endif  /* ENABLE_AACS */

#if (BD_ENABLE == 1)
            //could it use DVD_CSS_SCRATCH area when BD disc loaded ?
            case GEN_RECOG_FMT_LAYER:
                //-------------------------------------------------
                // List of recognized format layers
                //-------------------------------------------------
                if (pdbDiscLoaded() == BD_DISC)
                {
                    /* Insure that maximum transfer length is not exceeded */
                    i = 6 + Layers*2;
                    HostTransferLength = MINIMUM(i,HostTransferLength);

                    HostTransferLength -= 2;

                    // Point to work area in DRAM
                    DramAddressP       = (BYTE *) (SCSDRAM_ADDRESS + DVD_CSS_SCRATCH);

                    // Prepare Header for host
                    *DramAddressP++ = BHIGH(HostTransferLength);      // DVD Structure
                    *DramAddressP++ = BLOW(HostTransferLength);       // Data Length
                    *DramAddressP++ = 0;                              // Reserved
                    *DramAddressP++ = 0;                              // Reserved

                    // number of recognized format layers
                    *DramAddressP++ = Layers;

                    // deault and online format layers
                    *DramAddressP++ = 0x00;     /* bit5-4: default format layer, bit1-0: online format layer */

                    for (i = 0 ; i < Layers; i++)
                    {
                        *DramAddressP++ = 0x00;

                        LayerFormat = pdbGetBDHybridDiscId();

                        if (LayerFormat & DI_CD_MASK)
                            *DramAddressP++ = FMT_LR_CD;
                        else if (LayerFormat & DI_DVD_MASK)
                            *DramAddressP++ = FMT_LR_DVD;
                        else
                            *DramAddressP++ = FMT_LR_BD;
                    }

                    // Transfer Disc Manufacturing Information to host.
                    SendParameterDataToHost(SCSDRAM_ADDRESS + DVD_CSS_SCRATCH, HostTransferLength + 2, TRUE);    // Transfer data and send Command Complete.
                    return TRUE;
                }
                else
                {
                     BUILD_SENSE(0x05,0x24,0x00,0xcd);       // INVALID FIELD IN CDB
                     break;
                }
#endif  // #if (BD_ENABLE == 1)

#if (WRITE_PROTECT == 1)
            case GEN_WRITE_PROTECT_STS:  /*Write Protection*/
                DramAddressP       = (BYTE *) (SCSDRAM_ADDRESS + DVD_CSS_SCRATCH);   // Point to work area in DRAM
                *DramAddressP++ = 0;    // DVD Structure
                *DramAddressP++ = 6;    // Data Length.
                *DramAddressP++ = 0;    // Reserved.
                *DramAddressP++ = 0;    // Reserved.

                if((pdbGetDiscKind() == eDVDMinusRWDisc) && (Info.Media.DVD.RMAData.RMD.Format2.DiscStatus & bit7))
                    *DramAddressP++ = bit1;
                else
                    *DramAddressP++ = 0;
                *DramAddressP++ = 0;                            // Reserved.
                *DramAddressP++ = 0;                            // Reserved.
                *DramAddressP++ = 0;                            // Reserved.
                SendParameterDataToHost(DVD_CSS_SCRATCH, MINIMUM(8,HostTransferLength), TRUE);    // Transfer data and send Command Complete.
                return TRUE;
#endif

            case GEN_DISC_STRU_LIST:  /* Structure List */
                base = StruFmtList;
                HostTransferLength = MINIMUM(sizeof(StruFmtList), HostTransferLength);
                HostTransferLength -= 2;

                DramAddressP    = (BYTE *) (SCSDRAM_ADDRESS + DVD_CSS_SCRATCH);
                *DramAddressP++ = BHIGH(HostTransferLength);                      //  this data will later be transferred to host.
                *DramAddressP++ = BLOW(HostTransferLength);
                *DramAddressP++ = 0;
                *DramAddressP++ = 0;

                for(; base->FmtCode < 0xFF; base++)
                {
                    if (base->FmtCode <= 0x07 || base->FmtCode > 0x0B)
                    {
                        *DramAddressP++ = base->FmtCode;
                        *DramAddressP++ = base->SDS_RDS;
                        *DramAddressP++ = BHIGH(base->Length);
                        *DramAddressP++ = BLOW(base->Length);
                    }

                    if (base->FmtCode >= 0x08 && base->FmtCode <= 0x0B)
                    {
#if (DVD_RAM_READ == 1)
                        *DramAddressP++ = base->FmtCode;

                        if (pdbGetDiscKind() == eDVDRAMDisc)
                            *DramAddressP++ = base->SDS_RDS;
                        else
                            *DramAddressP++ = 0x00;

                        *DramAddressP++ = BHIGH(base->Length);
                        *DramAddressP++ = BLOW(base->Length);
#endif
                    }
                }

                SendParameterDataToHost(DVD_CSS_SCRATCH, HostTransferLength + 2, TRUE);
                return TRUE;

            default:
                BUILD_SENSE(0x05,0x24,0x00,0xc1);       // INVALID FIELD IN CDB
        }
    }
    else
    {
        BUILD_SENSE(0x05,0x24,0x00,0xbc);       // INVALID FIELD IN CDB
    }

    AutoCommandComplete();
    return TRUE;
}

