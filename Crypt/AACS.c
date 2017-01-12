/*****************************************************************************
*             (c) Copyright 2001 - 2011 Sunext Technology Co., Ltd.
*                            All Rights Reserved
*
*
* FILENAME:     $Workfile: AACS.c $
*
* DESCRIPTION   This module contains functions related to AACS algorithm
*
* $Revision: 31 $
* $Date: 11/04/26 1:40p $
*
**************************** SUNEXT CONFIDENTIAL ****************************/

#include "..\OEM\oem_info.h"
#include "..\HIF\flash.h"
#include "..\HIF\gp_mac.h"
#include "..\DVD\dvdef.h"
#include "..\DVD\dvmacros.h"
#include "..\Player\plrdb.h"
#include "..\DefMgmt\BDWritable\bddfmg.h"
#include "..\BD\bddiscrd.h"
#include "..\BD\bdvars.h"
#include "..\BD\bdmacro.h"
#if (ENABLE_AACS == 1)
#include "..\HIF\rd_stru.h"
#include "..\Crypt\aacs.h"
#include "..\Crypt\cmac.h"
#if (CEProduct == 1)
#include "..\DVD\dvgvars.h"
#include "..\DVD\dvrpc.h"
#endif
#endif


#if (ENABLE_AACS == 1)
// ***************************************************************************
//                               VARIABLES
// ***************************************************************************

AacsAuthenticateInfoS AacsAuth;
AacsS AacsStatus;

ULONG MkbVersionOnDisc;
ULONG MkbVersionOnFlash;

extern ULONG AacsKeyModificationMask;

#if (CEProduct == 1)
extern void GenSDDriveResponse(BYTE *DataPtr, AacsAuthenticateInfoS *AacsAuthPtr);
extern BOOL VerifySDHostResponse(BYTE *SrcPtr, BYTE *DestPtr, AacsAuthenticateInfoS *AacsAuthPtr);
extern void GenVUDriveResponse(BYTE BEType, BYTE *DataPtr, AacsAuthenticateInfoS *AacsAuthPtr);
extern BOOL VerifyVUHostResponse(BYTE BEType, BYTE *SrcPtr, BYTE *DestPtr, AacsAuthenticateInfoS *AacsAuthPtr);
#endif

// ***************************************************************************
//                              FUNCTIONS
// ***************************************************************************

/*********************************************************************
* Name:         InitializeAacsFlags
*
* Description:  Initialize Aacs related flags
*
* Params:       None
*
* Returns:      None
*
* Notes:
*
*********************************************************************/
void InitializeAacsFlags(void)
{
    // Initialize AACS Authentication Flags
    AacsStatus.AuthenticationGrantId            = CLEAR;
    AacsStatus.AacsFlag.IsMediumCompliant       = FALSE;
    AacsStatus.AacsFlag.IsHrlValid              = FALSE;
    AacsStatus.AacsFlag.AgidInUse               = FALSE;
    AacsStatus.AacsFlag.AuthenticationSuccess   = FALSE;
    AacsStatus.eAacsState                       = AACS_START;

    // Clear secure item modification mask
    AacsKeyModificationMask = 0;
    ClearKeysStorageMask();
}

/*********************************************************************
* Name:         AacsValidateAgid
*
* Description:  Validate AGID for AACS authentication
*
* Returns:      TRUE    if Agid valid
*               FALSE   if Agid invalid
*
* Notes:        The Agid passed in cmd packet is checked
*               for validity.
*
*********************************************************************/
BOOL AacsValidateAgid(BYTE SentAgid)
{
    if (SentAgid == AacsStatus.AuthenticationGrantId)
        return (TRUE);
    else
        return (FALSE);
}

/*********************************************************************
* Name:         AacsInvalidateAgid
*
* Description:  Invalidate AGID for AACS authentication
*
* Params:       none
*
* Returns:      none
*
* Global outputs:
*               fAgidInUse, AacsStatus.AuthenticationGrantId, bdAacs.eAacsState
*
* Notes:        The AGID passed in the cmd packet must
*               first be checked for validity
*
*********************************************************************/
void AacsInvalidateAgid(void)
{
    AacsStatus.AuthenticationGrantId    = CLEAR;
    AacsStatus.AacsFlag.AgidInUse       = FALSE;
    AacsStatus.eAacsState               = AACS_START;
}

/*********************************************************************
* Name:         AacsGetAgid
*
* Description:  Get the AuthenticationGrantId for AACS authentication
*
* Params:       none
*
* Returns:      0 = AGID (if not granted)
*               1 = AGID granted (AGID = 0)
*
* Global outputs: fAgidInUse set to 1
*
* Notes:        AGID hard-coded as zero
*               This function merely checks if AGID is in use,
*               and if not, mark as in use (set fAgidInUse to 1)
*********************************************************************/
BYTE AacsGetAgid(void)
{
    if (AacsStatus.AacsFlag.AgidInUse == FALSE)
    {
        AacsStatus.AacsFlag.AgidInUse = TRUE;
        return (TRUE);
    }
    else
    {
        return (FALSE);
    }
}

/*********************************************************************
* Name:         GetBindingNonce
*
* Description:  Get the Binding Nonce and calculate its MAC
*
* Params:       Ptr : Pointer to the location where Binding Nonce
                      will be stored.
*
* Returns:      none
*
* Notes:
*
*********************************************************************/
extern PSNs  bdConvertLBAtoPSN(ULONG LBA);
void GetBindingNonce(BYTE *Ptr, StLongU StartLba, BYTE Type)
{
    ReplacementInfoType ReplacementInfo;
    ULONG   ClusterAddress;
    BYTE    *UcdPtr;
    DiscAccessStatusE result;
    PSNs    StartPsn;
    BYTE    Layer, i, Msg[20], Mac[16], offset;

    switch(pdbDiscLoaded())
    {
        case BD_DISC:
            ClusterAddress = pdbGetBDClusterStart(StartLba.Long);
            StartPsn = bdConvertLBAtoPSN(ClusterAddress);
            offset = (BYTE)(StartLba.Long % ONE_BD_CLUSTER);
            if (SearchReplacementInfo(StartPsn.PSN, &ReplacementInfo))
                StartPsn.PSN = ReplacementInfo.ReplacementPba;

            Layer = ((StartPsn.PSN & 0x1000000)? eLayer1 : eLayer0);

            for (i = 0; i < 5; i++)
            {
                /*---------------------------------------------------------------
                    Set up the Disc Transfer control parameters
                ----------------------------------------------------------------*/
                ClearDataBuffer();
                bdDiscRequest.Type          = INIT_READ;
                bdDiscRequest.TargetLayer   = Layer;
                bdDiscRequest.TargetID.Long = StartPsn.PSN;
                bdDiscRequest.ClusterCount  = 1L;
                BdwtDisc.ReadDataMode = eNOT_USERDATA;

                result = bdStartDiscTransfer();

                if (result == BUF_GOOD)
                    break;
            }

            if (result == BUF_GOOD)
            {
                UcdPtr = (BYTE *)GetUcdPointer((WORD)offset);
                for (i = 0; i < 16; i++)
                    Msg[i] = UcdPtr[i+2];

                Msg[16] = StartLba.byte.u; 
                Msg[17] = StartLba.byte.h; 
                Msg[18] = StartLba.byte.m; 
                Msg[19] = StartLba.byte.l; 

                GenerateCmac(&AacsAuth.BusKey[0], &Msg[0], sizeof(Msg), &Mac[0]);

                if (Type == KfBcmAacsReadBindingNonce)
                    GenBcmBindingNonce(&Msg[0]);

                // Put the binding nonce and its MAC into the transfer area
                for (i = 0; i < 16; i++)
                    *Ptr++ = Msg[i];

                for (i = 0; i < 16; i++)
                    *Ptr++ = Mac[i];
            }
            break;

    }
}

/*********************************************************************
* Name:         ProcessKeysFromHost
*
* Description:  Decrypt the keys from host and encrypt the decrypted
*               keys with the secret key in the chip
*
* Params:       Addr : Address on which the parameter data sent by the
*                      host is located
*
* Returns:      True  : Store AACS Keys successfully
*               False : Store AACS Keys fail
*
* Notes:
*
*********************************************************************/
BOOL ProcessKeysFromHost(ULONG Addr)
{
    BYTE *source, *dest;

    source = (BYTE *)(SCSDRAM + Addr);
    dest = (BYTE *)SCSDRAM;

    if (ProcessAacsKey(source, dest) == FALSE)
        return FALSE;
    else
        return TRUE;
}

/*********************************************************************
* Name:         ValidateTypeAndVersionRecord
*
* Description:  Validate Type and Version Record in the MKB
*
* Params:       DataPtr: Pointer to the start of the memory address
*                        where MKBs are located.
*
* Returns:      True:  Valid
*               False: Invalid
*
* Notes:
*
*********************************************************************/
BOOL ValidateTypeAndVersionRecord(BYTE *DataPtr)
{
    BYTE    Type;
    StLongU Length;

    Type = *DataPtr;

    Length.byte.u = 0x00;
    Length.byte.h = *(DataPtr+1);
    Length.byte.m = *(DataPtr+2);
    Length.byte.l = *(DataPtr+3);

    if (Type == TYPE_AND_VERSION_RECORD_ID && Length.Long == TYPE_AND_VERSION_RECORD_LENGTH)
        return TRUE;
    else
        return FALSE;
}

/*********************************************************************
* Name:         AACSReportKey
*
* Description:  Handle Report Key Key Class 0x02.
*
* Returns:      void
*
* Notes:
*
*********************************************************************/
void AACSReportKey(void)
{
    USHORT      HostTransferLength;
    BYTE        *DramAddressP;
    BYTE        i, KeyFormat, SentAgid;
    StLongU     StartLba;
    BYTE        BlockCount;
#if (CEProduct == 1)
    RPC_NON_VOLATILE_VARS RpcSettings;
    BYTE        rpcStateByte0,rpcStateByte1,rpcStateByte2;  /* RPC data variables */
    BYTE        VendorID;
#endif

    BHIGH(HostTransferLength) = Iram_Cmd_block.bf[8];  // Get Allocation length from Command Descriptor Block
    BLOW (HostTransferLength) = Iram_Cmd_block.bf[9];
    KeyFormat = Iram_Cmd_block.bf[10] & 0x3F;

#if (CEProduct == 1)
    VendorID = (Iram_Cmd_block.bf[11] & 0xC0) >> 6;
#endif

    if ((AacsStatus.AacsFlag.IsMediumCompliant == TRUE) || (KeyFormat == 0x08))
    {
        SentAgid = (Iram_Cmd_block.bf[10] & 0xC0) >> 6; //  Get AGID (Authentication Grant ID) from command block

        /* Get Key Format */


        switch (KeyFormat)
        {
            case KfNewAgid:
                if (AacsGetAgid() == TRUE)
                {
                    /* AGID is granted */
                    AacsStatus.eAacsState = AACS_START;

                    // Set up scratch area for parameter transfer
                    DramAddressP = (BYTE *) (SCSDRAM_ADDRESS + DVD_CSS_SCRATCH);   // Point to work area in DRAM
                    *DramAddressP++ = 0;
                    *DramAddressP++ = 6;

                    for (i=0;i<5;i++)
                    {
                        *DramAddressP++ = 0;      // Fill reserved bytes.
                    }

                    *DramAddressP++ = (AacsStatus.AuthenticationGrantId << 6);

                    // Set up for transfer to host
                    SendParameterDataToHost(DVD_CSS_SCRATCH ,MINIMUM(8,HostTransferLength), TRUE);
                    return;
                }
                else
                {
                    /* AGID is not granted */
                    BUILD_SENSE(0x05,0x2C,0x00,0x10);               /* AGID is already in use */
                    AacsStatus.eAacsState = AACS_START;
                    AacsStatus.AacsFlag.AgidInUse = FALSE;
                }
                break;

#if (CEProduct == 1)
            case KfAacsRegionCode:
                RpcSettings = RpcGetRegion();    // Get RPC settings from FLASH or Serial EEPROM
                /*****************************
                *       Get RPC Type Code
                *       Byte 0 : bits 6 & 7
                *****************************/
                rpcStateByte0 = RpcSettings.RPC.State << 6;

                /********************************************************
                *       Get number of user controlled changes available
                *       Byte 0 : bits 3 - 5
                ********************************************************/
                rpcStateByte0 |= (RpcSettings.RPC.VendorResetsAvailable & 0x07) << 3;

                /********************************************************
                *       Get number of user controlled changes available
                *       Byte 0 : bits 0 - 2
                ********************************************************/
                rpcStateByte0 |= (RpcSettings.RPC.nCount & 0x07);

                /*********************************
                *       Get region mask. Byte 1
                *********************************/
                rpcStateByte1 = RpcSettings.RPC.LuRegion;

                /********************************************
                *       Determine RPC functionality. Byte 2
                *******************************************/
                #define RPC_SCHEME_UNKNOWN      0x00
                #define RPC_SCHEME_PHASE_II     0x01
                rpcStateByte2 = RPC_SCHEME_PHASE_II;

                /***********************************
                *Set DRAM pointer to scratch area
                **********************************/
                DramAddressP = (BYTE *) (SCSDRAM_ADDRESS + DVD_CSS_SCRATCH);   // Point to work area in DRAM

                /*********************************
                *Write RPC data to scratch area
                ********************************/
                *DramAddressP++ = 0;
                *DramAddressP++ = 6;
                *DramAddressP++ = 0;
                *DramAddressP++ = 0;
                *DramAddressP++ = rpcStateByte0;
                *DramAddressP++ = rpcStateByte1;
                *DramAddressP++ = rpcStateByte2;
                *DramAddressP++ = 0;

                SendParameterDataToHost(DVD_CSS_SCRATCH ,MINIMUM(8,HostTransferLength) , FALSE);

                break;

            case KfBcmAacsDriveChallenge:
                if (AacsValidateAgid(SentAgid))
                {
                    if (AacsStatus.eAacsState != AACS_DRIVE_CHALLENGE)
                    {
                        BUILD_SENSE(0x05,0x2C,0x00,0x11);            // COMMAND SEQUENCE ERROR
                        break;
                    }

                    AacsStatus.eAacsState = AACS_DRIVE_RESPONSE;

                    DramAddressP = (BYTE *) (SCSDRAM_ADDRESS + DVD_CSS_SCRATCH);   // Point to work area in DRAM

                    *DramAddressP++ = 0x00;      // Allocation
                    *DramAddressP++ = 22;        // Length
                    *DramAddressP++ = 0x00;      // Reserved
                    *DramAddressP   = 0x00;      // Reserved

                    DramAddressP = (BYTE *) (SCSDRAM_ADDRESS + DVD_CSS_SCRATCH + 4);

                    // Call library function to generate Drive Challenge
                    GenBcmDriveChallenge(DramAddressP, &AacsAuth);

                    // Set up for transfer to host
                    SendParameterDataToHost(DVD_CSS_SCRATCH ,MINIMUM(24,HostTransferLength), FALSE);

                    AacsStatus.eAacsState = AACS_AUTHENTICATION_COMPLETE;
                }
                else
                {
                    BUILD_SENSE(0x05,0x24,0x00,0xc2);                   /* INVALID FIELD IN CDB */
                }
                break;

            case KfSDAacsDriveResponse:
                if (VendorID != 0x02)
                {
                    BUILD_SENSE(0x05,0x24,0x00,0xc1);                   /* INVALID FIELD IN CDB */
                    break;
                }

                if (AacsStatus.eAacsState != AACS_DRIVE_CHALLENGE)
                {
                    BUILD_SENSE(0x05,0x2C,0x00,0x14);            // COMMAND SEQUENCE ERROR
                    break;
                }

                AacsStatus.eAacsState = AACS_DRIVE_RESPONSE;

                DramAddressP = (BYTE *) (SCSDRAM_ADDRESS + DVD_CSS_SCRATCH);   // Point to work area in DRAM

                *DramAddressP++ = 0x00;      // Allocation
                *DramAddressP++ = 0x32;      // Length
                *DramAddressP++ = 0x00;      // Reserved
                *DramAddressP   = 0x00;      // Reserved

                DramAddressP = (BYTE *) (SCSDRAM_ADDRESS + DVD_CSS_SCRATCH + 4);

                // Call library function to generate Drive Challenge
                GenSDDriveResponse(DramAddressP, &AacsAuth);

                // Set up for transfer to host
                SendParameterDataToHost(DVD_CSS_SCRATCH ,MINIMUM(54,HostTransferLength), FALSE);

                AacsStatus.eAacsState = AACS_DRIVE_RESPONSE_COMPLETE;

                break;

            case KfSDAacsReadBindingNonce:

                switch(pdbDiscLoaded())
                {
                    case BD_DISC:
                        if (pdbGetBDType() == BD_ROM)
                        {
                            BUILD_SENSE(0x05,0x20,0x00,0x03);               /* INCOMPATIBLE MEDIUM INSTALLED */
                            AutoCommandComplete();
                            return;
                        }
                        break;
                    default:
                        break;
                }

                if (AacsStatus.eAacsState != AACS_AUTHENTICATION_COMPLETE)
                {
                    BUILD_SENSE(0x05,0x6F,0x02,0x07);              // COPY PROTECTION KEY EXCHANGE FAILURE - KEY NOT ESTABLISHED
                    break;
                }

                // Get the host request starting LBA from the command block
                StartLba.byte.u = Iram_Cmd_block.bf[2];
                StartLba.byte.h = Iram_Cmd_block.bf[3];
                StartLba.byte.m = Iram_Cmd_block.bf[4];
                StartLba.byte.l = Iram_Cmd_block.bf[5];

                // Get the host request block count from the command block
                BlockCount = Iram_Cmd_block.bf[6];

                DramAddressP = (BYTE *) (SCSDRAM_ADDRESS + DVD_CSS_SCRATCH);   // Point to work area in DRAM

                *DramAddressP++ = 0x00;      // Allocation
                *DramAddressP++ = 0x22;      // Length
                *DramAddressP++ = 0x00;      // Reserved
                *DramAddressP   = 0x00;      // Reserved

                DramAddressP = (BYTE *) (SCSDRAM_ADDRESS + DVD_CSS_SCRATCH + 4);

                // Call library function to get Binding Nonce and MAC
                GetBindingNonce(DramAddressP, StartLba, 0);

                // Set up for transfer to host
                SendParameterDataToHost(DVD_CSS_SCRATCH ,MINIMUM(36,HostTransferLength), FALSE);

                break;
#endif  /* CEProduct == 1 */

            case KfAacsDriveChallenge:
                if (AacsValidateAgid(SentAgid))
                {
                    if (AacsStatus.eAacsState != AACS_DRIVE_CHALLENGE)
                    {
                        BUILD_SENSE(0x05,0x2C,0x00,0x11);            // COMMAND SEQUENCE ERROR
                        break;
                    }

                    AacsStatus.eAacsState = AACS_DRIVE_RESPONSE;

                    DramAddressP = (BYTE *) (SCSDRAM_ADDRESS + DVD_CSS_SCRATCH);   // Point to work area in DRAM

                    *DramAddressP++ = 0x00;      // Allocation
                    *DramAddressP++ = 0x72;      // Length
                    *DramAddressP++ = 0x00;      // Reserved
                    *DramAddressP   = 0x00;      // Reserved

                    DramAddressP = (BYTE *) (SCSDRAM_ADDRESS + DVD_CSS_SCRATCH + 4);

                    // Call library function to generate Drive Challenge
                    GenerateDriveChallenge(DramAddressP, &AacsAuth);

                    // Set up for transfer to host
                    SendParameterDataToHost(DVD_CSS_SCRATCH ,MINIMUM(116,HostTransferLength), FALSE);

                    AacsStatus.eAacsState = AACS_DRIVE_RESPONSE_COMPLETE;
                }
                else
                {
                    BUILD_SENSE(0x05,0x24,0x00,0xc2);                   /* INVALID FIELD IN CDB */
                }
                break;

            case KfAacsDriveKey:
                if (AacsValidateAgid(SentAgid))
                {
                    if (AacsStatus.eAacsState != AACS_DRIVE_RESPONSE_COMPLETE)
                    {
                        BUILD_SENSE(0x05,0x2C,0x00,0x12);            // COMMAND SEQUENCE ERROR
                        break;
                    }

                    AacsStatus.eAacsState = AACS_DRIVE_KEY_RSP;

                    DramAddressP = (BYTE *) (SCSDRAM_ADDRESS + DVD_CSS_SCRATCH);   // Point to work area in DRAM

                    *DramAddressP++ = 0x00;      // Allocation
                    *DramAddressP++ = 0x52;      // Length
                    *DramAddressP++ = 0x00;      // Reserved
                    *DramAddressP   = 0x00;      // Reserved

                    DramAddressP = (BYTE *) (SCSDRAM_ADDRESS + DVD_CSS_SCRATCH + 4);

                    // Call library function to generate Drive Key
                    GenerateDriveKey(DramAddressP, &AacsAuth);

                    // Set up for transfer to host
                    SendParameterDataToHost(DVD_CSS_SCRATCH ,MINIMUM(84,HostTransferLength), FALSE);

                    AacsStatus.eAacsState = AACS_DRIVE_KEY_RSP_COMPLT;
                }
                else
                {
                    BUILD_SENSE(0x05,0x24,0x00,0xc3);                   /* INVALID FIELD IN CDB */
                }
                break;

            case KfAacsReadBindingNonce:
            case KfBcmAacsReadBindingNonce:
                if(AacsValidateAgid(SentAgid))
                {
                    switch(pdbDiscLoaded())
                    {
                        case BD_DISC:
                            if (pdbGetBDType() == BD_ROM)
                            {
                                BUILD_SENSE(0x05,0x20,0x00,0x03);               /* INCOMPATIBLE MEDIUM INSTALLED */
                                AutoCommandComplete();
                                return;
                            }
                            break;

                        default:
                            break;
                    }

                    if (AacsStatus.eAacsState != AACS_AUTHENTICATION_COMPLETE)
                    {
                        BUILD_SENSE(0x05,0x6F,0x02,0x07);              // COPY PROTECTION KEY EXCHANGE FAILURE - KEY NOT ESTABLISHED
                        break;
                    }

                    // Get the host request starting LBA from the command block
                    StartLba.byte.u = Iram_Cmd_block.bf[2];
                    StartLba.byte.h = Iram_Cmd_block.bf[3];
                    StartLba.byte.m = Iram_Cmd_block.bf[4];
                    StartLba.byte.l = Iram_Cmd_block.bf[5];

                    // Get the host request block count from the command block
                    BlockCount = Iram_Cmd_block.bf[6];

                    DramAddressP = (BYTE *) (SCSDRAM_ADDRESS + DVD_CSS_SCRATCH);   // Point to work area in DRAM

                    *DramAddressP++ = 0x00;      // Allocation
                    *DramAddressP++ = 0x22;      // Length
                    *DramAddressP++ = 0x00;      // Reserved
                    *DramAddressP   = 0x00;      // Reserved

                    DramAddressP = (BYTE *) (SCSDRAM_ADDRESS + DVD_CSS_SCRATCH + 4);

                    // Call library function to get Binding Nonce and MAC
                    if (KeyFormat == KfBcmAacsReadBindingNonce)
                        GetBindingNonce(DramAddressP, StartLba, KfBcmAacsReadBindingNonce);
                    else
                        GetBindingNonce(DramAddressP, StartLba, 0);

                    // Set up for transfer to host
                    SendParameterDataToHost(DVD_CSS_SCRATCH ,MINIMUM(36,HostTransferLength), FALSE);

                    // Authentication Complete. Invalidates AGID and returns to initial state.
                    //AacsStatus.AacsFlag.AgidInUse = FALSE;
                    //AacsStatus.eAacsState = AACS_START;
                }
                else
                {
                    BUILD_SENSE(0x05,0x24,0x00,0xc5);                   /* INVALID FIELD IN CDB */
                }
                break;

            case KfInvalidateAgid:
                //******************************************************
                // Service this command only if AGID valid. Invalid AGID
                // will not be considered as an error, but command is
                // not serviced.
                //******************************************************
                if (AacsValidateAgid(SentAgid))
                {
                    AacsInvalidateAgid();
                }
                break;

            default:
                BUILD_SENSE(0x05,0x24,0x00,0xc6);                   /* INVALID FIELD IN CDB */
                break;
        }
        if (pdbDiscLoaded() == BD_DISC && BD_BUFFERING() == ON)
        {
            bdStopDiscTransfer();
        }
        else 
        if ((pdbDiscLoaded() == DVD_DISC || pdbDiscLoaded() == HD_DISC) && (DVD_BUFFERING() == ON))
        {
            dvdStopDiscTransfer();
        }

        ClearDataBuffer();
        Svr_pause_reqire = TRUE;
    }
    else
    {
        BUILD_SENSE(0x05,0x6F,0x01,0x05);                           /* COPY PROTECTION KEY EXCHANGE FAILURE - KEY NOT PRESENT */
    }

    AutoCommandComplete();
}


/*********************************************************************
* Name:         AACSSendKey
*
* Description:  Handle Send Key Command Key Class 0x02.
*
* Returns:      void
*
* Notes:
*
*********************************************************************/
void AACSSendKey(void)
{
    BYTE    *DramAddressP;
    USHORT  HostTransferLength;
    BYTE    KeyFormat, SentAgid;
#if (CEProduct == 1)
    BYTE    i;
    BYTE    *DestPtr, VendorID;
#endif

    BHIGH(HostTransferLength) = Iram_Cmd_block.bf[8];  // Get Allocation length from Command Descriptor Block
    BLOW (HostTransferLength) = Iram_Cmd_block.bf[9];

    if (AacsStatus.AacsFlag.IsMediumCompliant == TRUE)
    {
        SentAgid = (Iram_Cmd_block.bf[10] & 0xC0) >> 6; //  Get AGID (Authentication Grant ID) from command block

        /* Get Key Format */
        KeyFormat = Iram_Cmd_block.bf[10] & 0x3F;
#if (CEProduct == 1)
        VendorID = (Iram_Cmd_block.bf[11] & 0xC0) >> 6;
#endif
        switch (KeyFormat)
        {
#if (CEProduct == 1)
            case KfBcmAacsHostChallenge:
                if (AacsValidateAgid(SentAgid))
                {
                    if (AacsStatus.eAacsState != AACS_START)
                    {
                        BUILD_SENSE(0x05,0x2C,0x00,0x14);            // COMMAND SEQUENCE ERROR
                        break;
                    }

                    if (HostTransferLength != 0x18)
                    {
                        BUILD_SENSE(0x05,0x24,0x00,0xc7);     // INVALID FIELD IN CDB
                        break;
                    }

                    GetParameterDataFromHost(DVD_CSS_SCRATCH, HostTransferLength);

                    DramAddressP = (BYTE *) (SCSDRAM_ADDRESS + DVD_CSS_SCRATCH + 4);   // Point to work area in DRAM

                    // Store Host Nonce(Hn)
                    for (i = 0; i < sizeof(NonceType); i++)
                        AacsAuth.HostNonce[i] = *DramAddressP++;

                    AacsStatus.eAacsState = AACS_DRIVE_CHALLENGE;
                }
                else
                {
                    BUILD_SENSE(0x05,0x24,0x00,0xc8);                   /* INVALID FIELD IN CDB */
                }
                break;

            case KfSDAacsHostNonce:
                if (VendorID != 0x02)
                {
                    BUILD_SENSE(0x05,0x24,0x00,0xc1);                   /* INVALID FIELD IN CDB */
                    break;
                }

                if (HostTransferLength != 0x14)
                {
                    BUILD_SENSE(0x05,0x24,0x00,0xc7);     // INVALID FIELD IN CDB
                    break;
                }

                GetParameterDataFromHost(DVD_CSS_SCRATCH, HostTransferLength);

                DramAddressP = (BYTE *) (SCSDRAM_ADDRESS + DVD_CSS_SCRATCH + 4);   // Point to work area in DRAM

                // Store Host Nonce(Hn)
                for (i = 0; i < 16; i++)
                    AacsAuth.HostNonce[i] = *DramAddressP++;

                AacsStatus.eAacsState = AACS_DRIVE_CHALLENGE;

                break;

           case KfSDAacsHostResponse:
                if (VendorID != 0x02)
                {
                    BUILD_SENSE(0x05,0x24,0x00,0xc1);                   /* INVALID FIELD IN CDB */
                    break;
                }

                if (AacsStatus.eAacsState != AACS_DRIVE_RESPONSE_COMPLETE)
                {
                    BUILD_SENSE(0x05,0x2C,0x00,0x14);            // COMMAND SEQUENCE ERROR
                    break;
                }

                if (HostTransferLength != 0x34)
                {
                    BUILD_SENSE(0x05,0x24,0x00,0xc7);     // INVALID FIELD IN CDB
                    break;
                }

                GetParameterDataFromHost(DVD_CSS_SCRATCH, HostTransferLength);

                DramAddressP = (BYTE *) (SCSDRAM_ADDRESS + DVD_CSS_SCRATCH + 4);   // Point to work area in DRAM
                DestPtr = (BYTE *)SCSDRAM_ADDRESS;

                // Decrypt and verify if transfered Nd and Nh are valid or not
                if (VerifySDHostResponse(DramAddressP, DestPtr, &AacsAuth) == FALSE)
                {
                     BUILD_SENSE(0x05,0x6F,0x01,0x11);
                     break;
                }

                AacsStatus.eAacsState = AACS_AUTHENTICATION_COMPLETE;

                break;
#endif  /* CEProduct == 1 */

            case KfAacsHostChallenge:
                if (AacsValidateAgid(SentAgid))
                {
                    if (AacsStatus.eAacsState != AACS_START)
                    {
                        BUILD_SENSE(0x05,0x2C,0x00,0x14);            // COMMAND SEQUENCE ERROR
                        break;
                    }

                    if (HostTransferLength != 116)
                    {
                        BUILD_SENSE(0x05,0x24,0x00,0xc7);     // INVALID FIELD IN CDB
                        break;
                    }

                    GetParameterDataFromHost(DVD_CSS_SCRATCH, HostTransferLength);

                    DramAddressP = (BYTE *) (SCSDRAM_ADDRESS + DVD_CSS_SCRATCH + 4);   // Point to work area in DRAM

                    // Call library function to validate Host Challenge
                    if (ValidateHostChallenge((ChallengeType *)DramAddressP, &AacsAuth) == FALSE)
                    {
                        AacsStatus.AacsFlag.AuthenticationSuccess = FALSE;
                        AacsStatus.eAacsState = AACS_AUTHENTICATION_FAILED;
                        BUILD_SENSE(0x05,0x6F,0x00,0x05);                   // COPY PROTECTION KEY EXCHANGE FAILURE - AUTHENTICATION FAILURE
                        break;
                    }
                    else
                        AacsStatus.eAacsState = AACS_DRIVE_CHALLENGE;
                }
                else
                {
                    BUILD_SENSE(0x05,0x24,0x00,0xc8);                   /* INVALID FIELD IN CDB */
                }
                break;


            case KfAacsHostKey:
                if (AacsValidateAgid(SentAgid))
                {
                    if (AacsStatus.eAacsState != AACS_DRIVE_KEY_RSP_COMPLT)
                    {
                        BUILD_SENSE(0x05,0x2C,0x00,0x15);            // COMMAND SEQUENCE ERROR
                        break;
                    }

                    if (HostTransferLength != 84)
                    {
                        BUILD_SENSE(0x05,0x24,0x00,0xc9);     // INVALID FIELD IN CDB
                        break;
                    }

                    GetParameterDataFromHost(DVD_CSS_SCRATCH, HostTransferLength);

                    AacsStatus.eAacsState = AACS_HOST_KEY_VALIDATION;

                    DramAddressP = (BYTE *) (SCSDRAM_ADDRESS + DVD_CSS_SCRATCH + 4);

                    // Call library function to validate Host Key and generate Bus Key
                    if (ValidateHostKey((KeyType *)DramAddressP, &AacsAuth) == FALSE)
                    {
                        AacsStatus.AacsFlag.AuthenticationSuccess = FALSE;
                        AacsStatus.eAacsState = AACS_AUTHENTICATION_FAILED;
                        BUILD_SENSE(0x05,0x6F,0x00,0x06);                   // COPY PROTECTION KEY EXCHANGE FAILURE - AUTHENTICATION FAILURE
                        break;
                    }
                    else
                    {
                        AacsStatus.AacsFlag.AuthenticationSuccess = TRUE;
                        AacsStatus.eAacsState = AACS_AUTHENTICATION_COMPLETE;
                    }
                }
                else
                {
                    BUILD_SENSE(0x05,0x24,0x00,0xca);                   /* INVALID FIELD IN CDB */
                }
                break;

            case KfInvalidateAgid:
                //******************************************************
                // Service this command only if AGID valid. Invalid AGID
                // will not be considered as an error, but command is
                // not serviced.
                //******************************************************
                if (AacsValidateAgid(SentAgid))
                {
                    AacsInvalidateAgid();
                }
                break;

            default:
                BUILD_SENSE(0x05,0x24,0x00,0xcb);                   /* INVALID FIELD IN CDB */
                break;
        }
        if (pdbDiscLoaded() == BD_DISC && BD_BUFFERING() == ON)
        {
            bdStopDiscTransfer();
        }
        else 
        if ((pdbDiscLoaded() == DVD_DISC || pdbDiscLoaded() == HD_DISC) && (DVD_BUFFERING() == ON))
        {
            dvdStopDiscTransfer();
        }

        ClearDataBuffer();
        Svr_pause_reqire = TRUE;
    }
    else
    {
        BUILD_SENSE(0x05,0x6F,0x01,0x06);                           /* COPY PROTECTION KEY EXCHANGE FAILURE - KEY NOT PRESENT */
    }

    AutoCommandComplete();
}

#if (CEProduct == 1)
void VendorReportKey(void)
{
    StLongU StartLba;
    USHORT  HostTransferLength;
    BYTE    BlockCount;
    BYTE    KeyFormat, VendorID;
    BYTE    *DramAddressP;

    if (pdbDiscLoaded() == BD_DISC)
    {
        bdStopDiscTransfer();
        bdDecFlag.fDISABLE_READ_AHEAD = TRUE;
    }
    else if (pdbDiscLoaded() == DVD_DISC)
    {
        if ( DVD_BUFFERING() == ON )
            dvdStopDiscTransfer();

        dvdDecFlag.fDISABLE_READ_AHEAD = TRUE;
    }
    else
    {
        BUILD_SENSE(0x05,0x30,0x00,0x06);    /* INCOMPATIBLE MEDIUM INSTALLED */
        AutoCommandComplete();
        return;
    }

    // Disable Read Ahead will make the buffer is almost empty, and that will cause speed up.
    // Clear SpeedCount to avoid speeding up.
    SpeedCount = 0;

    ClearDataBuffer();
    Svr_pause_reqire = TRUE;

    BHIGH(HostTransferLength) = Iram_Cmd_block.bf[8];  // Get Allocation length from Command Descriptor Block
    BLOW (HostTransferLength) = Iram_Cmd_block.bf[9];

    if (AacsStatus.AacsFlag.IsMediumCompliant == TRUE)
    {
        KeyFormat = Iram_Cmd_block.bf[10] & 0x3F;
        VendorID = (Iram_Cmd_block.bf[11] & 0xF8) >> 3;

        switch (KeyFormat)
        {
            case KfVendorAacsDriveResponse:
                if (AacsStatus.eAacsState != AACS_DRIVE_CHALLENGE)
                {
                    BUILD_SENSE(0x05,0x2C,0x00,0x17);            // COMMAND SEQUENCE ERROR
                    break;
                }

                AacsStatus.eAacsState = AACS_DRIVE_RESPONSE;

                DramAddressP = (BYTE *) (SCSDRAM_ADDRESS + BUFFER_SCRATCH);   // Point to work area in DRAM

                *DramAddressP++ = 0x00;      // Allocation
                *DramAddressP++ = 0x32;      // Length
                *DramAddressP++ = 0x00;      // Reserved
                *DramAddressP   = 0x00;      // Reserved

                DramAddressP = (BYTE *) (SCSDRAM_ADDRESS + BUFFER_SCRATCH + 4);

                if (VendorID == 0x01)
                {
                    // Call library function to generate Drive Challenge
                    GenVUDriveResponse(BE_TYPE_MARVELL, DramAddressP, &AacsAuth);
                }
                else if (VendorID == 0x02)
                {
                    // Call library function to generate Drive Challenge
                    GenVUDriveResponse(BE_TYPE_INTEL, DramAddressP, &AacsAuth);
                }
                else
                {
                    BUILD_SENSE(0x05,0x24,0x00,0xd5);       // INVALID FIELD IN CDB
                    break;
                }

                // Set up for transfer to host
                SendParameterDataToHost(BUFFER_SCRATCH ,MINIMUM(52,HostTransferLength), FALSE);

                AacsStatus.eAacsState = AACS_DRIVE_RESPONSE_COMPLETE;
                break;
            case KfVendorAacsReadBindingNonce:
                switch(pdbDiscLoaded())
                {
                    case BD_DISC:
                        if (pdbGetBDType() == BD_ROM)
                        {
                            BUILD_SENSE(0x05,0x30,0x00,0x08);    /* INCOMPATIBLE MEDIUM INSTALLED */
                            AutoCommandComplete();
                            return;
                        }
                        break;
                    default:
                        break;
                }

                if (AacsStatus.eAacsState != AACS_AUTHENTICATION_COMPLETE)
                {
                    BUILD_SENSE(0x05,0x6F,0x02,0x20);              // COPY PROTECTION KEY EXCHANGE FAILURE - KEY NOT ESTABLISHED
                    break;
                }

                // Get the host request starting LBA from the command block
                StartLba.byte.u = Iram_Cmd_block.bf[2];
                StartLba.byte.h = Iram_Cmd_block.bf[3];
                StartLba.byte.m = Iram_Cmd_block.bf[4];
                StartLba.byte.l = Iram_Cmd_block.bf[5];

                // Get the host request block count from the command block
                BlockCount = Iram_Cmd_block.bf[6];

                DramAddressP = (BYTE *) (SCSDRAM_ADDRESS + BUFFER_SCRATCH);   // Point to work area in DRAM

                *DramAddressP++ = 0x00;      // Allocation
                *DramAddressP++ = 0x22;      // Length
                *DramAddressP++ = 0x00;      // Reserved
                *DramAddressP   = 0x00;      // Reserved

                DramAddressP = (BYTE *) (SCSDRAM_ADDRESS + BUFFER_SCRATCH + 4);

                // Call library function to get Binding Nonce and MAC
                GetBindingNonce(DramAddressP, StartLba, 0);

                // Set up for transfer to host
                SendParameterDataToHost(BUFFER_SCRATCH ,MINIMUM(36,HostTransferLength), FALSE);
                break;
            default:
                BUILD_SENSE(0x05,0x24,0x00,0xd7);                   /* INVALID FIELD IN CDB */
                break;
        }
    }
    else
    {
        BUILD_SENSE(0x05,0x6F,0x01,0x08);               /* COPY PROTECTION KEY EXCHANGE FAILURE - KEY NOT PRESENT */
    }

    AutoCommandComplete();
}

void VendorSendKey(void)
{
    USHORT  HostTransferLength;
    BYTE    KeyFormat, VendorID;
    BYTE    i, *DramAddressP, *DestPtr;

    if (pdbDiscLoaded() == BD_DISC)
    {
        bdStopDiscTransfer();
        bdDecFlag.fDISABLE_READ_AHEAD = TRUE;
    }
    else if (pdbDiscLoaded() == DVD_DISC)
    {
        if ( DVD_BUFFERING() == ON )
            dvdStopDiscTransfer();

        dvdDecFlag.fDISABLE_READ_AHEAD = TRUE;
    }
    else
    {
        BUILD_SENSE(0x05,0x30,0x00,0x07);    /* INCOMPATIBLE MEDIUM INSTALLED */
        AutoCommandComplete();
        return;
    }

    // Disable Read Ahead will make the buffer is almost empty, and that will cause speed up.
    // Clear SpeedCount to avoid speeding up.
    SpeedCount = 0;

    ClearDataBuffer();
    Svr_pause_reqire = TRUE;

    BHIGH(HostTransferLength) = Iram_Cmd_block.bf[8];   // Get Parameter List length from Command Descriptor Block
    BLOW (HostTransferLength) = Iram_Cmd_block.bf[9];

    if (AacsStatus.AacsFlag.IsMediumCompliant == TRUE)
    {
        KeyFormat = Iram_Cmd_block.bf[10] & 0x3F;
        VendorID = (Iram_Cmd_block.bf[11] & 0xF8) >> 3;

        switch (KeyFormat)
        {
            case KfVendorAacsHostNonce:
                if (HostTransferLength != 0x14)
                {
                    BUILD_SENSE(0x05,0x24,0x00,0xd1);       // INVALID FIELD IN CDB
                    break;
                }

                GetParameterDataFromHost(BUFFER_SCRATCH, HostTransferLength);

                DramAddressP = (BYTE *) (SCSDRAM_ADDRESS + BUFFER_SCRATCH + 4);   // Point to work area in DRAM

                // Store Host Nonce(Hn)
                for (i = 0; i < 16; i++)
                    AacsAuth.HostNonce[i] = *DramAddressP++;

                AacsStatus.eAacsState = AACS_DRIVE_CHALLENGE;
                break;
            case KfVendorAacsHostResponse:
                if (AacsStatus.eAacsState != AACS_DRIVE_RESPONSE_COMPLETE)
                {
                    BUILD_SENSE(0x05,0x2C,0x00,0x16);       // COMMAND SEQUENCE ERROR
                    break;
                }

                if (HostTransferLength != 0x34)
                {
                    BUILD_SENSE(0x05,0x24,0x00,0xd2);       // INVALID FIELD IN CDB
                    break;
                }

                GetParameterDataFromHost(BUFFER_SCRATCH, HostTransferLength);

                DramAddressP = (BYTE *) (SCSDRAM_ADDRESS + BUFFER_SCRATCH + 4);   // Point to work area in DRAM
                DestPtr = (BYTE *)SCSDRAM_ADDRESS;

                // Decrypt and verify if transfered Nd and Nh are valid or not
                if (VendorID == 0x01)
                {
                    if (VerifyVUHostResponse(BE_TYPE_MARVELL, DramAddressP, DestPtr, &AacsAuth) == FALSE)
                    {
                         BUILD_SENSE(0x05,0x6F,0x01,0x20);
                         break;
                    }
                }
                else if (VendorID == 0x02)
                {
                    if (VerifyVUHostResponse(BE_TYPE_INTEL, DramAddressP, DestPtr, &AacsAuth) == FALSE)
                    {
                         BUILD_SENSE(0x05,0x6F,0x01,0x23);
                         break;
                    }
                }
                else
                {
                    BUILD_SENSE(0x05,0x24,0x00,0xd3);       // INVALID FIELD IN CDB
                    break;
                }

                AacsStatus.eAacsState = AACS_AUTHENTICATION_COMPLETE;
                break;
            default:
                BUILD_SENSE(0x05,0x24,0x00,0xd4);           /* INVALID FIELD IN CDB */
                break;
        }
    }
    else
    {
        BUILD_SENSE(0x05,0x6F,0x01,0x21);               /* COPY PROTECTION KEY EXCHANGE FAILURE - KEY NOT PRESENT */
    }

    AutoCommandComplete();
}

void VendorReadDiscStructure(void)
{
    USHORT  HostTransferLength;
    BYTE    FormatField, VendorID;
    BYTE    *DramAddressP;
    BYTE    i, Mac[16];

    switch (pdbDiscLoaded())
    {
        case BD_DISC:
            if( BD_BUFFERING() == ON )
            {
                bdStopDiscTransfer();
            }
            bdDecFlag.fDISABLE_READ_AHEAD = TRUE;
            break;
        case DVD_DISC:
            if( DVD_BUFFERING() == ON )
            {
                dvdStopDiscTransfer();
            }
            dvdDecFlag.fDISABLE_READ_AHEAD = TRUE;
            break;
        default:
            BUILD_SENSE(0x05,0x30,0x00,0x09);    /* INCOMPATIBLE MEDIUM INSTALLED */
            AutoCommandComplete();
            return;
    }
    // Disable Read Ahead will make the buffer is almost empty, and that will cause speed up.
    // Clear SpeedCount to avoid speeding up.
    SpeedCount = 0;

    ClearDataBuffer();
    Svr_pause_reqire = TRUE;

    BHIGH(HostTransferLength) = Iram_Cmd_block.bf[8];  // Get Allocation length from Command Descriptor Block
    BLOW (HostTransferLength) = Iram_Cmd_block.bf[9];

    /* Get Format */
    FormatField = Iram_Cmd_block.bf[7];

    /* Get Vendor ID */
    VendorID = (Iram_Cmd_block.bf[11] & 0xF8) >> 3;

    switch(FormatField)
    {
        case GEN_AACS_VOLUM_ID:
            //-------------------------------------------------
            // AACS Volume Identifier
            //-------------------------------------------------

            if (pdbDiscLoaded() == BD_DISC && pdbGetBDType() != BD_ROM)
            {
                BUILD_SENSE(0x05,0x30,0x00,0x0a);    /* INCOMPATIBLE MEDIUM INSTALLED */
                AutoCommandComplete();
                return;
            }

            if (pdbDiscLoaded() == DVD_DISC && pdbGetDVDType() != DVD_ROM)
            {
                BUILD_SENSE(0x05,0x30,0x00,0x0b);    /* INCOMPATIBLE MEDIUM INSTALLED */
                AutoCommandComplete();
                return;
            }

            if (AacsStatus.eAacsState != AACS_AUTHENTICATION_COMPLETE)
            {
                BUILD_SENSE(0x05,0x6F,0x02,0x21);                   // COPY PROTECTION KEY EXCHANGE FAILURE - KEY NOT ESTABLISHED
                AutoCommandComplete();
                return;
            }

            // Generate Message Authentication Code(MAC) of Volume ID
            GenerateCmac(&AacsAuth.BusKey[0], &AacsAuth.ID.VolumeID[0], sizeof(AacsAuth.ID.VolumeID), &Mac[0]);

            DramAddressP       = (BYTE *) (SCSDRAM_ADDRESS + BUFFER_SCRATCH);   // Point to work area in DRAM
            *DramAddressP++ = 0;        // Disc Structure
            *DramAddressP++ = 0x22;     // Data Length.
            *DramAddressP++ = 0;        // Reserved.
            *DramAddressP++ = 0;        // Reserved.

            // Volume ID Field
            for (i = 0; i < sizeof(AacsAuth.ID.VolumeID); i++)
                *DramAddressP++ = AacsAuth.ID.VolumeID[i];

            // Message Authentication Code(MAC) Field
            for (i = 0; i < sizeof(Mac); i++)
                *DramAddressP++ = Mac[i];

            // Return Data to Host
            SendParameterDataToHost(BUFFER_SCRATCH, MINIMUM(36,HostTransferLength), FALSE);    // Transfer data complete.
            break;

    #if (BCA_ENABLE == 1)
        case GEN_AACS_MEDIA_SN:
            //-------------------------------------------------
            // AACS Media serial number
            //-------------------------------------------------
            if (pdbGetDiscKind() == eBDROMDisc || pdbGetDiscKind() == eBDROMDLDisc)
            {
                if (BDReadBCA() == FAIL)
                {
                    BUILD_SENSE(0x05,0x6F,0x02,0x22);               // COPY PROTECTION KEY EXCHANGE FAILURE - KEY NOT ESTABLISHED
                    AutoCommandComplete();
                    return;
                }
            }

            if (AacsStatus.eAacsState != AACS_AUTHENTICATION_COMPLETE)
            {
                BUILD_SENSE(0x05,0x6F,0x02,0x23);                   // COPY PROTECTION KEY EXCHANGE FAILURE - KEY NOT ESTABLISHED
                AutoCommandComplete();
                return;
            }

            // Generate Message Authentication Code(MAC) of PMSN
            GenerateCmac(&AacsAuth.BusKey[0], &AacsAuth.MediaSN[0], sizeof(AacsAuth.MediaSN), &Mac[0]);

            DramAddressP       = (BYTE *) (SCSDRAM_ADDRESS + BUFFER_SCRATCH);   // Point to work area in DRAM
            *DramAddressP++ = 0;        // Disc Structure
            *DramAddressP++ = 0x22;     // Data Length.
            *DramAddressP++ = 0;        // Reserved.
            *DramAddressP++ = 0;        // Reserved.

            // Pre-recorded Media Serial Number Field
            for (i = 0; i < sizeof(AacsAuth.MediaSN); i++)
                *DramAddressP++ = AacsAuth.MediaSN[i];

            // Message Authentication Code(MAC) Field
            for (i = 0; i < sizeof(Mac); i++)
                *DramAddressP++ = Mac[i];

            // Return Data to Host
            SendParameterDataToHost(BUFFER_SCRATCH, MINIMUM(36,HostTransferLength), FALSE);    // Transfer data cmplete.
            break;

        case GEN_AACS_MEDIA_ID:
            //-------------------------------------------------
            // AACS Media Identifer
            //-------------------------------------------------
            if (pdbDiscLoaded() == BD_DISC)
            {
                if (pdbGetBDType() == BD_ROM)
                {
                    BUILD_SENSE(0x05,0x30,0x00,0x0c);    /* INCOMPATIBLE MEDIUM INSTALLED */
                    AutoCommandComplete();
                    return;
                }
                else if (BDReadBCA() == FAIL)
                {
                    BUILD_SENSE(0x05,0x6F,0x02,0x24);              // COPY PROTECTION KEY EXCHANGE FAILURE - KEY NOT ESTABLISHED
                    AutoCommandComplete();
                    return;
                }
            }

        #if (ENABLE_AVCREC == 1)
            if (pdbDiscLoaded() == DVD_DISC)
            {
                if (DVDReadBCA() == FAIL)
                {
                    BUILD_SENSE(0x05,0x6F,0x02,0x25);              // COPY PROTECTION KEY EXCHANGE FAILURE - KEY NOT ESTABLISHED
                    AutoCommandComplete();
                    return;
                }
            }
        #endif

            if (AacsStatus.eAacsState != AACS_AUTHENTICATION_COMPLETE)
            {
                BUILD_SENSE(0x05,0x6F,0x02,0x26);              // COPY PROTECTION KEY EXCHANGE FAILURE - KEY NOT ESTABLISHED
                AutoCommandComplete();
                return;
            }

            // Generate Message Authentication Code(MAC) of Media ID
            GenerateCmac(&AacsAuth.BusKey[0], &AacsAuth.ID.MediaID[0], sizeof(AacsAuth.ID.MediaID), &Mac[0]);

            DramAddressP       = (BYTE *) (SCSDRAM_ADDRESS + BUFFER_SCRATCH);   // Point to work area in DRAM
            *DramAddressP++ = 0;        // Disc Structure
            *DramAddressP++ = 0x22;     // Data Length.
            *DramAddressP++ = 0;        // Reserved.
            *DramAddressP++ = 0;        // Reserved.

            // Media ID Field
            for (i = 0; i < sizeof(AacsAuth.ID.MediaID); i++)
                *DramAddressP++ = AacsAuth.ID.MediaID[i];

            // Message Authentication Code(MAC) Field
            for (i = 0; i < sizeof(Mac); i++)
                *DramAddressP++ = Mac[i];

            // Return Data to Host
            SendParameterDataToHost(BUFFER_SCRATCH, MINIMUM(36,HostTransferLength), FALSE);    // Transfer data complete.
            break;
    #endif  /* BCA_ENABLE == 1 */

        case GEN_MARVELL_AACS_KCD:
            //-------------------------------------------------
            // AACS Key Conversion Data
            //-------------------------------------------------
            if (pdbDiscLoaded() == BD_DISC && pdbGetBDType() != BD_ROM)
            {
                BUILD_SENSE(0x05,0x30,0x00,0x0d);    /* INCOMPATIBLE MEDIUM INSTALLED */
                AutoCommandComplete();
                return;
            }

            if (AacsStatus.eAacsState != AACS_AUTHENTICATION_COMPLETE)
            {
                BUILD_SENSE(0x05,0x6F,0x02,0x27);              // COPY PROTECTION KEY EXCHANGE FAILURE - KEY NOT ESTABLISHED
                AutoCommandComplete();
                return;
            }

            // Generate Message Authentication Code(MAC) of KCD
            if (FormatField == GEN_MARVELL_AACS_KCD)
            {
                GenerateCmacForKcd(&Mac[0], BE_TYPE_MARVELL, &AacsAuth);
            }
            else
                GenerateCmacForKcd(&Mac[0], BE_TYPE_DEFAULT, &AacsAuth);

            DramAddressP       = (BYTE *) (SCSDRAM_ADDRESS + BUFFER_SCRATCH);   // Point to work area in DRAM
            *DramAddressP++ = 0;        // Disc Structure
            *DramAddressP++ = 0x22;     // Data Length.
            *DramAddressP++ = 0;        // Reserved.
            *DramAddressP++ = 0;        // Reserved.

            // KCD Field
            for (i = 0; i < 16; i++)
            {
                if (FormatField == GEN_MARVELL_AACS_KCD)
                    *DramAddressP++ = AacsAuth.eKCD[i];
                else
                    *DramAddressP++ = AacsAuth.KCD[i];
            }

            // Message Authentication Code(MAC) Field
            for (i = 0; i < sizeof(Mac); i++)
                *DramAddressP++ = Mac[i];

            // Return Data to Host
            SendParameterDataToHost(BUFFER_SCRATCH, MINIMUM(36,HostTransferLength), FALSE);    // Transfer data complete.
            break;
        default:
            BUILD_SENSE(0x05,0x24,0x00,0xd6);       // INVALID FIELD IN CDB
            break;
    }
    AutoCommandComplete();
}
#endif  /* CEProduct == 1 */

#endif  /* ENABLE_AACS == 1 */
