/****************************************************************************
*                (c) Copyright 2001 -  2005 SUNEXT DESIGN, INC.
*                            All Rights Reserved
*
*
*    FILE NAME:     $Workfile: GP_Buf.c $
*
*    DESCRIPTION:   MMC Commands
*                        ReadBufferCommand
*                        WriteBufferCommand
*
*
*   FUNCTIONS:
*   ReadBufferCommand()
*   WriteBufferCommand()
*
*   $Revision: 17 $
*   $Date: 11/02/25 11:42a $
*
*
*
************************** SUNEXT CONFIDENTIAL ********************************/


#include ".\common\globtype.h"
#include ".\common\sdram.h"
#include ".\hif\gp_ram.h"
#include ".\hif\gp_equ.h"
#include ".\hif\gp_mac.h"
#include ".\hif\bf_util.h"
#include ".\hif\at_util.h"
#include ".\hif\at_knl.h"
#include ".\hif\mmc.h"
#include ".\hif\gp_util.h"
#include ".\hif\gp_buf.h"


#define MAX_DATA_LEN 0x1000
extern void CheckDataTransferLimit( USHORT);
extern void GetParameterDataFromHost(ULONG, ULONG);

#if (SUN_FUNC_TEST == 0)
/*********************************************************************
* ReadBufferCommand()
*
*  Follows Mt. Fuji READ BUFFER command syntax
*   CDB byte 0 (Op-code) = 3Ch;
*   CDB byte 1 (Mode) = 1 (data only);
*   CDB byte 1 (Mode) = 3 (descriptor);
*   CDB byte 2 (Buf ID) = 0;
*   CDB bytes 3 (MSB) - 5 (LSB) = start address in buffer {truncated to word boundary};
*   CDB bytes 6 (MSB) - 8 (LSB) = bytes to transfer {truncated to even # of words};
*
* Returns:  void
* Params:   none
*********************************************************************/
void ReadBufferCommand()
{
    ULONG AllocationLength, StartAddress;
    BYTE * BufferP;

    StartAddress = PacketCommandToLBALEN3(3);     // Get Start Address

    AllocationLength = PacketCommandToLBALEN3(6);   // Get length
    AllocationLength &= 0xFFFFFE;                   // Force length to be multiple number of words  (i.e. mask Least Significant bit)

    if ( AllocationLength == 0x00 )
    {
        AutoCommandComplete();       // Just return if length is zero.
        return;
    }


    // Special handling for descriptor mode
    if ( Iram_Cmd_block.bf[1] == BUFFER_MODE_DESCRIPTOR )
    {
        // Build descriptor image in DRAM
        BufferP = (BYTE *)(BUFFER_SCRATCH + SCSDRAM_START_ADDR);   // Point to Scratch area in the buffer to
                                            //   assemble data returned to host.
        if ( Iram_Cmd_block.bf[2] == BUFFER_ID_0 )
        {
            // valid buffer ID
            *BufferP++ = (0x01);                //  Byte 0:  Offset boundary:  even bytes
            *BufferP++ = (SYS_START_PAGE + 1);  //  Byte 1:  Buffer capacity  MSB
            *BufferP++ = (0x00);                //  Byte 2:  Buffer capacity  MID
            *BufferP   = (0x00);                //  Byte 3:  Buffer capacity  LSB
        }
        else
        {   // invalid buffer ID:  pass all 00's
            *BufferP++ = (0x00);                //  Byte 0
            *BufferP++ = (0x00);                //  Byte 1
            *BufferP++ = (0x00);                //  Byte 2
            *BufferP   = (0x00);                //  Byte 3
        }

        // Send descriptor image to host */
        CheckDataTransferLimit(MINIMUM( RDBUFFER_LEN_BYTE , AllocationLength));     // Check if transfer data in burst mode
        SendParameterDataToHost(BUFFER_SCRATCH,MINIMUM( RDBUFFER_LEN_BYTE , AllocationLength) , TRUE);  // Transfer data and do Command Complete.
        return;

    }

    if ( Iram_Cmd_block.bf[1] != BUFFER_MODE_DATA )
    {
        BUILD_SENSE(0x05,0x24,0x00,0x37);    // Fail. Invalid mode
        AutoCommandComplete();
        return;
    }

    if ( Iram_Cmd_block.bf[2] != BUFFER_ID_0 )
    {
        BUILD_SENSE(0x05,0x24,0x00,0x38);    // Fail. Invalid Buffer ID
        AutoCommandComplete();
        return;
    }

    if ( StartAddress > ( (SYS_START_PAGE + 1) * 0x10000) )
    {
        BUILD_SENSE(0x05,0x24,0x00,0x39);    // Fail. Start Address too big
        AutoCommandComplete();
        return;
    }

    /* Change length if start + length extends beyond phsyical buffer */
    if ( (StartAddress + AllocationLength) > ((SYS_START_PAGE + 1) * 0x10000) )
    {
        AllocationLength = ( ( (SYS_START_PAGE + 1) * 0x10000) - StartAddress );
    }
    SendParameterDataToHost((ULONG)(StartAddress),AllocationLength , TRUE);               // Send data to host and send Command Complete to host.
    return;
}


/*********************************************************************
* WriteBufferCommand
*
*  Follows Mt. Fuji WRITE BUFFER command syntax
*   CDB byte 0 (Op-code) = 3B;
*   CDB byte 1 (Mode) = 2 (data only);
*   CDB byte 2 (Buf ID) = 0;
*   CDB bytes 3 (MSB) - 5 (LSB) = start address in buffer {truncated to word boundary};
*   CDB bytes 6 (MSB) - 8 (LSB) = bytes to transfer {truncated to even # of words};
*
* Returns:  void
* Params:   none
*********************************************************************/
void WriteBufferCommand()
{
    ULONG StartAddress, TransferLength;

    StartAddress = PacketCommandToLBALEN3(3);   // Get Start Address
    StartAddress &= 0xFFFFFFFE;  // Force start to be word boundary (i.e. mask Least Significant bit)

    TransferLength = PacketCommandToLBALEN3(6);  // Get length

    TransferLength &= 0xFFFFFFFE;  // Fforce length to be multiple number of words  (i.e. mask Least Significant bit)

    if ( TransferLength == 0x00 )
    {
        AutoCommandComplete();   // Just return. Length is zero.
        return;
    }

    if ( TransferLength >= 0x10000 )  //Tx length can not be over than 64K 
    {
        BUILD_SENSE(0x05,0x24,0x00,0x3e);
        AutoCommandComplete();   // Just return. Length is zero.
        return;
    }    

    if ( Iram_Cmd_block.bf[1] != BUFFER_MODE_DATA )
    {
        BUILD_SENSE(0x05,0x24,0x00,0x3a);       // Fail. Invalid mode.
        AutoCommandComplete();
        return;
    }

    if ( Iram_Cmd_block.bf[2] != BUFFER_ID_0 )
    {
        BUILD_SENSE(0x05,0x24,0x00,0x3b);   // Fail. Invalid Buffer ID.
        AutoCommandComplete();
        return;
    }

    if ( StartAddress > ( (SYS_START_PAGE + 1) * 0x10000) )
    {
        BUILD_SENSE(0x05,0x24,0x00,0x3c);
        AutoCommandComplete();
        return;
    }

    // fail if start + length extends beyond phsyical buffer.
    if ( (StartAddress + TransferLength) > ((SYS_START_PAGE  + 1) * 0x10000) )
    {
        BUILD_SENSE(0x05,0x24,0x00,0x3d);
        AutoCommandComplete();
        return;
    }
    GetParameterDataFromHost(StartAddress, TransferLength);  // This functions transfers data from Host to Buffer.
    AutoCommandComplete();
    return;
}
#endif
