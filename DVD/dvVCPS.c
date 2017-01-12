/****************************************************************************
*                (c) Copyright 2000-2005, SUNEXT DESIGN, INC.
*                            All Rights Reserved
*
*
*
*   FILE NAME:     dvVCPS.c
*
* DESCRIPTION
*
* Contains the VCPS for DVD+R/+RW.
*
*   $Revision: 19 $
*   $Date: 07/08/24 3:08p $
*
**************************** SUNEXT CONFIDENTIAL *******************************/

/*********************************************************************
  INCLUDE FILES
*********************************************************************/

#include ".\dvd\dvGvars.h"
#include ".\hif\at_util.h"


#if (VCPS_TEST ==1)

VcpsKeys DecryptedInitVector;
vcpsAuthenticateInfoS *vcpsAuth;
BYTE *DKBInfoPtr;
BYTE *DKBInfoStatusPtr;
BYTE VcpsKeysValid;
USHORT DKBInfoLength;
USHORT ADIPInfoBaseAddr;
vpcsAdipInfoTypeE DKBInfoType;

extern BYTE RandomNumberGenerator(BYTE seedp);


/*******************************************************************************

    FUNCTION        Get_DKB

    DESCRIPTION

********************************************************************************/
BYTE Get_DKB(void)
{
    DiscAccessStatusE DiscReadStatus;
    ULONG BlockAddress;
    BYTE *BufferPtr;


    switch (vcpsDKB.State)
    {
        case DKB_NOT_READ:

            if (vcpsData.DKBLocation == DKB_ADIP)
            {
                if (GetVcpsInfoInAdip(vcpsDKB.DKBInAdipStartAddr, (USHORT)vcpsDKB.size, ADIP_INFO_DKB) == TRUE)
                {
                    vcpsDKB.State = DKB_VALID;

                    // Copy DKB data to DKB_START_ADDRESS
                    CopyBufferToBuffer(0, 0, DKB_START_PAGE, 0, ((USHORT)vcpsDKB.size + 2047)/2048);

                    // Write DKB to Buffer Zone 2
                    if (DVDWriteBufferZone2() == TRUE)
                    {
                        vcpsDKB.State = DKB_VALID;
                        vcpsData.DKBLocation = DKB_BUFFER_ZONE2;
                        return (PASS);
                    }
                }
            }
            break;

        case DKB_HEADER_VALID:

            if (vcpsData.DKBLocation == DKB_BUFFER_ZONE2)
            {
                BlockAddress = DKB_BUFFERZONE2_ADDRESS1; // DKB#1 position in Buffer Zone 2

                while (1)
                {
                    ClearDataBuffer();  // Clar the DVD buffer

                    /*---------------------------------------------------------------
                        Set up the Disc Transfer control parameters for reading
                        the lead in block
                    ----------------------------------------------------------------*/
                    dvdDiscRequest.Type             = INIT_READ;
                    dvdDiscRequest.TargetID.Long    = BlockAddress;
                    dvdDiscRequest.EccBlockCount    = (vcpsDKB.size + 32767L)/32768L;
                    dvdDiscRequest.TargetLayer      = eLayer0;

                    /*---------------------------------------------------------------
                        Call the main DVD disc transfer engine to load the blocks
                    ----------------------------------------------------------------*/
                    DiscReadStatus = dvdStartDiscTransfer();

                    /*---------------------------------------------------------------
                        Check if the Lead In data has been properly buffered
                    ----------------------------------------------------------------*/
                    if ((DiscReadStatus == BUF_GOOD) || (BlockAddress == DKB_BUFFERZONE2_ADDRESS2))
                    {
                        break;
                    }
                    else /* DiscReadStatus != BUF_GOOD */
                    {
                        dvdResetErrorFlags();
                        BlockAddress = DKB_BUFFERZONE2_ADDRESS2; // DKB#2 position in Buffer Zone 2
                    }
                }

                vcpsDKB.State = DKB_INVALID;

                if (DiscReadStatus == BUF_GOOD)
                {
                    BufferPtr = (BYTE *)(SCSDRAM_START_ADDR);
                    BlockAddress = MAKE_LONG(*BufferPtr,*(BufferPtr+1),*(BufferPtr+2),*(BufferPtr+3));
                    if (BlockAddress == 0x56435053) // Check EKB Signature "VCPS_EKB"
                    {
                        BlockAddress = MAKE_LONG(*(BufferPtr+4),*(BufferPtr+5),*(BufferPtr+6),*(BufferPtr+7));
                        if (BlockAddress == 0x5f454b42)
                        {
                            vcpsDKB.State = DKB_VALID;

                            // Copy DKB data to DKB_START_ADDRESS
                            CopyBufferToBuffer(0, 0, DKB_START_PAGE, 0, ((USHORT)vcpsDKB.size + 2047)/2048);

                            return (PASS);
                        }
                    }
                }
            }
            break;
    }
    return (FAIL);
}

/*******************************************************************************

    FUNCTION        vcpsReportKey

    DESCRIPTION     Manages VCPS Host requests through Report Key Command

********************************************************************************/
void VCPS_ReportKey(void)
{
    WORD    HostTransferLength;
    BYTE    *BufferPtr;
    BYTE    DeviceMessage[32];
    BYTE    Result[16];

    switch (Iram_Cmd_block.bf[6])
    {
        case RPTKEY_DKB:
            if (vcpsDKB.State == DKB_NOT_READ || vcpsDKB.State == DKB_HEADER_VALID)
            {
                if (Get_DKB() == FAIL)
                {
                    BUILD_SENSE(0x05,0x55,0x00,0x09);    // ILLEGAL REQUEST/SYSTEM RESOURCE FAILURE
                    return;
                }
            }
            else if (vcpsDKB.State == DKB_INVALID)
            {
                BUILD_SENSE(0x05,0x55,0x00,0x0a);    // ILLEGAL REQUEST/SYSTEM RESOURCE FAILURE
                return;
            }

            {
                BYTE    i;
                USHORT  Data_len;

                // Prepare returned data
                BufferPtr = (BYTE *)(DKB_WORK_AREA_ADDRESS);

                // Because the Allocation Length of Report Key command is less than 0xffff
                if (vcpsDKB.size > 0xFFFAL) // add header (4 bytes) = 0xfffe
                {
                    Data_len = 0xFFFAL;
                    // Set Data Length
                    *BufferPtr++ = 0xFF;
                    *BufferPtr++ = 0xFC;
                    i = 0;
                }
                else
                {
                    i = (vcpsDKB.size % 4);
                    if( i == 0)
                        Data_len = (USHORT)vcpsDKB.size;
                    else
                        Data_len = (USHORT)vcpsDKB.size + (4-i);

                    // Set Data Length
                    *BufferPtr++ = BHIGH(Data_len);
                    *BufferPtr++ = BLOW(Data_len);
                }
                *BufferPtr++ = 0x00;
                *BufferPtr   = 0x00;

                // Set the Padding data to 0x00
                if (i != 0)
                {
                    BufferPtr = (BYTE *)(vcpsDKB.size + DKB_START_ADDRESS);
                    *BufferPtr++ = 0;
                    *BufferPtr++ = 0;
                    *BufferPtr= 0;
                }

                BHIGH(HostTransferLength) = Iram_Cmd_block.bf[8];  // Get Allocation length from Command Descriptor Block
                BLOW (HostTransferLength) = Iram_Cmd_block.bf[9];

                Data_len += 2;

                SendParameterDataToHost(DKB_WORK_AREA_ADDRESS ,MINIMUM(Data_len, HostTransferLength), FALSE);
            }

            break;

        case RPTKEY_DEVICE_ID:
            vcpsData.State = VCPS_IDLE;
            vcpsData.Authorized = FALSE;
            vcpsData.Invalid = FALSE;
            vcpsAuth = (vcpsAuthenticateInfoS *) SHADOW_DB_INFO_ADR;

            SendVcpsInfo(RPTKEY_DEVICE_ID, (BYTE *)DecryptedInitVector.Set.DeviceID);

            vcpsData.State = VCPS_SEND_DEVICE_ID;
            break;

        case RPTKEY_KEY_CONTRIBUTION:
            if (ValidateSequence(VCPS_SEND_DRIVE_CONTRIBUTION_KEY) == FALSE)
                return;

            // Generate Device random number and key contribution
            GenerateRandomNumber(vcpsAuth->DeviceRndNum, 8);
            GenerateRandomNumber(vcpsAuth->DeviceKey, 16);

            // Encrypt Random Numbers and store into first 16 bytes of DeviceMessage
            ArrayXor( vcpsAuth->HostRndNum, (BYTE *)DecryptedInitVector.Set.Keys, Result, 16); // (HostRndNum+DeviceRndNum) XOR InitVector)
            AESEncrypt(vcpsAuth->RootKey, Result, DeviceMessage, 0);

            // Encrypt Drive Key Contribution and store into last 16 bytes of DeviceMessage
            ArrayXor( vcpsAuth->DeviceKey, DeviceMessage, Result, 16);
            AESEncrypt(vcpsAuth->RootKey, Result, &DeviceMessage[16], 1);

            SendVcpsInfo(RPTKEY_KEY_CONTRIBUTION, DeviceMessage);
            break;

        case RPTKEY_DKBHASH_UNIQUEID:

            if (ValidateSequence(VCPS_SEND_DKBHASH_UNIQUEID) == FALSE)
                return;

            // Collect DKB Hash value from Hash region in ADIP
            if (GetVcpsInfoInAdip(vcpsDKB.HashInAdipStartAddr, VCPS_HASH_SIZE, ADIP_INFO_HASH) == FALSE)
            {
                vcpsData.State = VCPS_IDLE;
                BUILD_SENSE(0x05,0x2C,0x00,0x0d);                // Command Sequence Error
                return;
            }

            // Encrypt DKB Hash Value
            ArrayXor(vcpsData.DKBHash, (BYTE *)DecryptedInitVector.Set.Keys, Result, 16);
            AESEncrypt(vcpsAuth->BusKey, Result, DeviceMessage, 0);

            // Encrypt Unique ID
            ArrayXor(vcpsData.UniqueID, DeviceMessage, Result, 16);
            AESEncrypt(vcpsAuth->BusKey, Result, &DeviceMessage[16], 1);

            SendVcpsInfo(RPTKEY_DKBHASH_UNIQUEID, DeviceMessage);

            break;

         case RPTKEY_DKB_INFO:
            if(vcpsDKB.State == DKB_NOT_READ)
            {
                if(Get_DKB() == FAIL)
                {
                    BUILD_SENSE(0x05,0x55,0x00,0x0b);    // ILLEGAL REQUEST/SYSTEM RESOURCE FAILURE
                    return;
                }
            }
            else if(vcpsDKB.State == DKB_INVALID)
            {
                BUILD_SENSE(0x05,0x55,0x00,0x0c);    // ILLEGAL REQUEST/SYSTEM RESOURCE FAILURE
                return;
            }

            BufferPtr = (BYTE *)(BUFFER_SCRATCH);
            *BufferPtr++ = 0x00;
            *BufferPtr++ = 0x0E;                    // Data Length
            *BufferPtr++ = 0x00;
            *BufferPtr++ = 0x00;
            *BufferPtr++ = GET_BYTE3(vcpsDKB.size); //DKB Size
            *BufferPtr++ = GET_BYTE2(vcpsDKB.size);
            *BufferPtr++ = GET_BHIGH(vcpsDKB.size);
            *BufferPtr++ = GET_BLOW(vcpsDKB.size);
            *BufferPtr++ = GET_BYTE3(vcpsDKB.size); //DKB Bytes Collected
            *BufferPtr++ = GET_BYTE2(vcpsDKB.size);
            *BufferPtr++ = GET_BHIGH(vcpsDKB.size);
            *BufferPtr++ = GET_BLOW(vcpsDKB.size);
            *BufferPtr++ = (vcpsDKBLocE)vcpsData.DKBLocation; // byte 8
            *BufferPtr++ = 0;
            *BufferPtr++ = 0;
            *BufferPtr = 0;

            BHIGH(HostTransferLength) = Iram_Cmd_block.bf[8];  // Get Allocation length from Command Descriptor Block
            BLOW (HostTransferLength) = Iram_Cmd_block.bf[9];

            SendParameterDataToHost(BUFFER_SCRATCH ,MINIMUM(16, HostTransferLength), FALSE);
            break;

        default:
            BUILD_SENSE(0x05,0x24,0x00,0x05);    // INVALID PARAMETER IN CDB
    }

    return;
}


/*******************************************************************************

    FUNCTION        vcpsSendKey

    DESCRIPTION     Manages VCPS Host requests through Send Key Command

********************************************************************************/
void VCPS_SendKey(void)
{
    BYTE i;
    BYTE HostMessage[32];
    BYTE Result[16];
    BYTE RandomNumber[8];

    switch (Iram_Cmd_block.bf[6])
    {
        case SNDKEY_AUTHORIZATION_KEY:
            if (ValidateSequence(VCPS_GET_AUTHORIZATION_KEY) == FALSE)
                return;

            if (GetVcpsInfo(SNDKEY_AUTHORIZATION_KEY, NULL) == FALSE)
                return;

            // Get the associated Device Node Key
            for(i=0; i<16; i++)
                vcpsAuth->NodeKey[i] = DecryptedInitVector.Set.NodeKeyTable[ vcpsAuth->NodeKeyIndex ][i];

            // Produce the root key
            AESEncrypt(vcpsAuth->NodeKey, vcpsAuth->AuthKey, vcpsAuth->RootKey, 0);

            break;

        case SNDKEY_KEY_CONTRIBUTION:

            if (ValidateSequence(VCPS_GET_HOST_CONTRIBUTION_KEY) == FALSE)
                return;

            // Get the encrypted host message
            if (GetVcpsInfo(SNDKEY_KEY_CONTRIBUTION, HostMessage) == FALSE)
                return;

            // Decrypt Host contribution key
            AESDecrypt(vcpsAuth->RootKey, &HostMessage[16], Result, 1);
            ArrayXor(Result, HostMessage, vcpsAuth->HostKey, 16);

            // Decrypt Random Numbers
            AESDecrypt(vcpsAuth->RootKey, HostMessage, Result, 1);
            ArrayXor(Result, (BYTE *)DecryptedInitVector.Set.Keys, RandomNumber, 8);

            // Validate Device Random number
            for ( i = 0; i < 8; i++)
            {
                if (RandomNumber[i] != vcpsAuth->DeviceRndNum[i])
                {
                    vcpsData.State = VCPS_IDLE;
                    BUILD_SENSE(0x05,0x6F,0x00,0x0d);    // Copy Protection Key Exchange / Authentication Failure
                    break;
                }
            }

            // Produce bus key where BusKey = AESHash( DeviceKey+HostKey )
            AESEncrypt(vcpsAuth->DeviceKey, vcpsAuth->HostKey, Result, 0);
            ArrayXor(Result, vcpsAuth->HostKey, vcpsAuth->BusKey, 16);

            break;

        default:
            BUILD_SENSE(0x05,0x24,0x00,0x06);    // INVALID PARAMETER IN CDB

    }

     return;
}

/*******************************************************************************

    FUNCTION        SendVcpsInfo

    DESCRIPTION     Constructs and sends VCPS related information to the host
                    based on the data type parameter


    NOTE            Size of the data is assumed by the data type

********************************************************************************/
void SendVcpsInfo(BYTE type, BYTE *data)
{
    BYTE *BufferPtr;
    BYTE i, ReserveBytes, vcpsDataSize;
    WORD HostTransferLength;

    BufferPtr = (BYTE *)(BUFFER_SCRATCH);

    // Data Length
    *BufferPtr++ = 0;
    *BufferPtr++ = 38;

    // Header reserve bytes
    *BufferPtr++ = 0;
    *BufferPtr++ = 0;

    // Set data construct parameters
    switch (type)
    {
        case RPTKEY_DEVICE_ID:

            ReserveBytes    = 31;
            vcpsDataSize    = 5;
            break;

        case RPTKEY_KEY_CONTRIBUTION:
        case RPTKEY_DKBHASH_UNIQUEID:

            ReserveBytes    = 4;
            vcpsDataSize    = 32;
            break;

        default:
            return;
    }


    for( i = 0; i < ReserveBytes; i++)      // Reserve Data (if any)
        *BufferPtr++ = 0;

    for( i = 0; i < vcpsDataSize; i++)      // Data
        *BufferPtr++ = data[i];

    BHIGH(HostTransferLength) = Iram_Cmd_block.bf[8];  // Get Allocation length from Command Descriptor Block
    BLOW (HostTransferLength) = Iram_Cmd_block.bf[9];

    SendParameterDataToHost(BUFFER_SCRATCH, MINIMUM(40, HostTransferLength), FALSE );

}

/*******************************************************************************

    FUNCTION        ValidateSequence

    DESCRIPTION     Insures proper VCPS state sequence during authentication

    RETURNS         TRUE    - valid sequence
                    FALSE   - invalid sequence

********************************************************************************/
BYTE ValidateSequence(vcpsStateE CurrentState)
{

    if (++vcpsData.State == CurrentState)
        return TRUE;                            // Command Sequence Valid

    vcpsData.State = VCPS_IDLE;
    BUILD_SENSE(0x05,0x2C,0x00,0x0e);                // Command Sequence Error
    return FALSE;

}

/*******************************************************************************

    FUNCTION        GetVcpsInfo

    DESCRIPTION     Gets VCPS information from the host. Information is specified
                    by data type

    PARAMETERS      type    Type of data to receive
                    data    pointer to data

    RETURN          TRUE    Data was received succesfully
                    FALSE   Data could not be received

********************************************************************************/
BYTE GetVcpsInfo(BYTE type, BYTE *data)
{
    BYTE *BufferPtr;
    BYTE i;
    WORD HostTransferLength;


    // Get VCPS Data from host

    BHIGH(HostTransferLength) = Iram_Cmd_block.bf[8];
    BLOW (HostTransferLength) = Iram_Cmd_block.bf[9];

    GetParameterDataFromHost(BUFFER_SCRATCH, HostTransferLength);

    // Store data
    switch (type)
    {

        case SNDKEY_AUTHORIZATION_KEY:

            // Point to information
            BufferPtr = (BYTE *)(BUFFER_SCRATCH + VCPS_AUTHKEY_INFO_OFFSET);

            // Get information
            vcpsAuth->NodeKeyIndex = *BufferPtr++;

            for( i = 0; i < VCPS_HOSTKEY_SIZE; i++)
                vcpsAuth->HostRndNum[i] = *BufferPtr++;

            for( i = 0; i < VCPS_AUTHKEY_SIZE; i++)
                vcpsAuth->AuthKey[i] = *BufferPtr++;

            break;

        case SNDKEY_KEY_CONTRIBUTION:

            // Point to information
            BufferPtr = (BYTE *)(BUFFER_SCRATCH + VCPS_KEYCONTRIBUTION_OFFSET);

            // Get information
            for( i = 0; i < VCPS_KEYENCRYPTION_SIZE; i++)
                *data++ = *BufferPtr++;

            break;

        default:
            BUILD_SENSE(0x05,0x55,0x00,0x0f);    // SYSTEM RESOURCE FAILURE if data transfer did not succeed
            return FALSE;

    }

    return TRUE;
}

/*******************************************************************************

    FUNCTION        ArrayXor

    DESCRIPTION     XOR's two arrays of bytes

    PARAMETERS      din1    address of data1
                    din2    address of data2
                    dout    address of data output
                    size    size of arrays



********************************************************************************/
void ArrayXor(BYTE *din1, BYTE *din2, BYTE *dout, BYTE size)
{
    BYTE x;

    for( x = 0; x < size; x++)
        dout[x] = din1[x] ^ din2[x];
}

/*******************************************************************************

    FUNCTION        GetVcpsInfoInAdip

    DESCRIPTION     Collect VCPS information stored in the auxiliary bytes of ADIP.

    NOTE            Size of the data is assumed by the data type

********************************************************************************/
BYTE GetVcpsInfoInAdip(ULONG StartAddr, USHORT Length, vpcsAdipInfoTypeE InfoType)
{
    ULONG WaitTimer;
    BYTE ret;
    BYTE retry;

    ret = FALSE;

    if (VCPS_disc == FALSE)
        return (ret);

    retry = 3;
    while (retry > 0)
    {
        if (dvdSeek(StartAddr, eLayer0) == Ready)
            break;

        retry--;
    }

    DKBInfoPtr = (BYTE *)(SCSDRAM_START_ADDR);
    DKBInfoStatusPtr = (BYTE *)(SCSDRAM_START_ADDR + 0x10000);
    DKBInfoLength = Length;
    ADIPInfoBaseAddr = (USHORT)(StartAddr >> 2);
    DKBInfoType = InfoType;

    ZeroFillDRAM(0,(USHORT)(MAX_BUFF_NUMBER+1));

    fVCPSInfoGet = TRUE;
    fVCPSInfoValid = FALSE;

    StartTimer(&WaitTimer);
    while (1)
    {
        if (fVCPSInfoValid == TRUE)
        {
            USHORT i;

            if (InfoType == ADIP_INFO_ETOC)
            {
                for (i=0;i<DKBInfoLength;i=i+16)
                {
                    if (DKBInfoPtr[i] == 'D' && DKBInfoPtr[i+1] == 'K' && DKBInfoPtr[i+2] == 'B')
                    {
                        vcpsDKB.DKBInAdipStartAddr = ((ULONG)DKBInfoPtr[i+4] << 8 | (ULONG)DKBInfoPtr[i+5]) << 10;
                        vcpsDKB.size = MAKE_LONG(DKBInfoPtr[i+6],DKBInfoPtr[i+7],DKBInfoPtr[i+8],DKBInfoPtr[i+9]);
                        vcpsDKB.DKBInAdipRepeatCnt = DKBInfoPtr[i+10];
                        vcpsDKB.DKBInAdipEndAddr = vcpsDKB.DKBInAdipStartAddr + ((vcpsDKB.size * (ULONG)vcpsDKB.DKBInAdipRepeatCnt + 3)/4) * ONE_DVD_BLOCK;
                    }
                    else if (DKBInfoPtr[i] == 'H' && DKBInfoPtr[i+1] == 'S' && DKBInfoPtr[i+2] == 'H')
                    {
                        vcpsDKB.HashInAdipStartAddr = ((ULONG)DKBInfoPtr[i+4] << 8 | (ULONG)DKBInfoPtr[i+5]) << 10;
                        vcpsDKB.HashInAdipRepeatCnt = DKBInfoPtr[i+10];
                        vcpsDKB.HashInAdipEndAddr = vcpsDKB.HashInAdipStartAddr + ((VCPS_HASH_SIZE * (ULONG)vcpsDKB.HashInAdipRepeatCnt + 3)/4) * ONE_DVD_BLOCK;
                    }
                }
            }
            else if (InfoType == ADIP_INFO_DKB)
            {
                vcpsDKB.size = MAKE_LONG(DKBInfoPtr[8],DKBInfoPtr[9],DKBInfoPtr[10],DKBInfoPtr[11]);
                vcpsDKB.seq_num = MAKE_LONG(DKBInfoPtr[12],DKBInfoPtr[13],DKBInfoPtr[14],DKBInfoPtr[15]);
                for (i=0; i<16; i++)
                    vcpsDKB.key_chk_data[i] = DKBInfoPtr[i+16];

                for (i=0; i<11; i++)
                    vcpsData.UniqueID[i] = 0;

                for (i=0; i<5; i++)
                    vcpsData.UniqueID[i+11] = RandomNumberGenerator(i);
            }
            else if (InfoType == ADIP_INFO_HASH)
            {
                for (i=0;i<DKBInfoLength;i++)
                    vcpsData.DKBHash[i] = DKBInfoPtr[i];
            }
            fVCPSInfoValid = FALSE;
            ret = TRUE;
            break;
        }

        if (ReadTimer(&WaitTimer) > 5000L)
        {
            break;
        }

        ExitProcess();
    }

    fVCPSInfoGet = FALSE;
    return (ret);
}

/*********************************************************************
* Name:         DecryptVCPSKeys
*
* Description:
*
* Params:       Input : None
*
* Returns:      output: None
*
* Global outputs: None
*
* Notes:
*
*********************************************************************/
void DecryptVCPSKeys(void)
{
    USHORT  HWKey, i;

    VcpsKeysValid = FALSE;

    DecryptedInitVector.Set.Checksum = 0;
    HWKey = ReadSCReg_2B(0x03EC);

    for (i=0;i<DEVICE_KEYS_TABLE_SIZE/2;i++)
    {
        // Swap first
        DecryptedInitVector.Array[2*i]   = InitVector.Array[2*i+1];
        DecryptedInitVector.Array[2*i+1] = InitVector.Array[2*i];

        // Xor with random key
        DecryptedInitVector.Array[2*i]   ^= BHIGH(InitVector.Set.RandomKey);
        DecryptedInitVector.Array[2*i+1] ^= BLOW(InitVector.Set.RandomKey);

        // Xor with undocumented register value
        DecryptedInitVector.Array[2*i]   ^= BHIGH(HWKey);
        DecryptedInitVector.Array[2*i+1] ^= BLOW(HWKey);

        DecryptedInitVector.Set.Checksum += (DecryptedInitVector.Array[2*i] + DecryptedInitVector.Array[2*i+1]);
    }

    if (DecryptedInitVector.Set.Checksum == InitVector.Set.Checksum)
        VcpsKeysValid = TRUE;
}

#endif  /* VCPS_TEST == 1 */

