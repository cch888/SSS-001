/*****************************************************************************
*                (c) Copyright 2000 - 2007 SUNEXT DESIGN, INC.
*                            All Rights Reserved
*
*
* FILENAME:     $Workfile: bdRdDisc.c $
*
* DESCRIPTION   This module contains functions related to the
*               Read Disc Information command for BD disc.
*
* $Revision: 17 $
* $Date: 11/03/07 4:33p $
*
**************************** SUNEXT CONFIDENTIAL ****************************/

// ***************************************************************************
//                               HEADERS
// ***************************************************************************
#include ".\common\globals.h"
#include ".\common\globtype.h"
#include ".\hif\Gp_ram.h"
#include ".\player\Plrdb.h"
#include ".\servo\svo.h"
#include ".\bd\bdDef.h"
#include ".\bd\bdrddisc.h"
#include ".\dvd\dvRdDisc.h"

#if (BD_ENABLE)
// ***************************************************************************
//                               EXTERNS
// ***************************************************************************
extern volatile CMD_BLOCK_T Iram_Cmd_block;


// ***************************************************************************
//                              FUNCTIONS
// ***************************************************************************

/*****************************************************************************

    FUNCTION        BDReadDiscInfoCommand

    DESCRIPTION     Processes ATAPI Read Disc Info command for BD

    PARAMETERS      None

    RETURNS         None

*****************************************************************************/
void BDReadDiscInfoCommand(void)
{
    USHORT  DiscInfoTotalLength;
    USHORT  DiscInfoLength;
    USHORT  DiscInfoOpcLength;
    BYTE    NumberOfOpcTableEntries;
    BYTE    Index;
    BYTE    discStatus;
    BYTE    discAppCode;
    BYTE    DataType;
//    LONG    tempAddr1, tempAddr2;

    //Fix the bug that wrong disc/track information
    //with blank BD-R/RE discs are reported.

    DataType = Iram_Cmd_block.bf[1] & 0x07;

    //------------------------------------------
    //     Initialize Parameters/Variables
    //------------------------------------------
    BHIGH(DiscInfoLength) = Iram_Cmd_block.bf[7];
    BLOW (DiscInfoLength) = Iram_Cmd_block.bf[8];

    if (DiscInfoLength == 0)
    {
        AutoCommandComplete();      // An Allocation Length of 0 is not considered an error.
        return;                     //  Just complete command and return.
    }

    DiscInfoOpcLength = 0;
    NumberOfOpcTableEntries = 0;

    DiscInfoTotalLength = DISC_INFO_LEN_BYTE + ((SHORT)(NumberOfOpcTableEntries) * 8);

    for(Index=0; Index<(DiscInfoTotalLength+2); Index++)
        disc_info[Index] = 0;                             // Initialize return data area array to 0.

    switch(DataType)
    {
        case STANDARD_DISC_INFORMATION:
            // ---------------------------------------------------------------------------------
            // Byte[0][1] : Data Length
            if(DiscInfoLength > DiscInfoTotalLength)
                DiscInfoLength = DiscInfoTotalLength;

            disc_info[0] = (BYTE) ((DiscInfoTotalLength-2) >> 8);   // The first two bytes
            disc_info[1] = (BYTE) (DiscInfoTotalLength-2);          //   are the Disc Information Length excludes length field.

            // ---------------------------------------------------------------------------------
            // Byte[2] : Bit4-Erasable, Bits3,2-State of Last Session, Bits1,0-Disc Status
            discStatus = pdbGetBDFinalSessCloseInfo();
            switch(discStatus)
            {
                case eSessionFinalized:     // 0000 1110 = Complete Session, Finalized/Complete Disc
                    disc_info[2] |= 0x0E;
                    break;
                case eSessionClosed:        // 0000 1101 = Complete Session, Incomplete Disc
                    disc_info[2] |= 0x0D;
                    break;
                case eNotSessionClosed:     // 0000 0101 = Incomplete Session, Incomplete Disc
                    disc_info[2] |= 0x05;
                    break;
                case eEmptySession:         // Empty Disc... further checking required
                    if(pdbGetBDFinalSessionNumber() == 1)
                    {
                         disc_info[2] &= 0x00;      // 0000 0000 = Blank Disc
                    }
                    else
                    {
                        // At least 1 Session is CLOSED, Current Session is Empty....
//                        tempAddr1 = pdbGetBDLeadOutStartPSN();
//                        tempAddr2 = pdbGetBDNextSessStartPSN();
//                        if ( tempAddr1 <= tempAddr2 )
//                            disc_info[2] |= 0x0D;   // 0000 1101 = Complete Session, Incomplete Disc
//                        else
                            disc_info[2] |= 0x01;   // 0000 0001 = Empty Session, Incomplete Disc

                    }
                    break;
                default:    // Empty session
                    break;
            }

            if(pdbGetDiscKind() == eBDREDisc || pdbGetDiscKind() == eBDREDLDisc)
                disc_info[2] |= 0x10;               /* If BD-RE, set Eraseable bit */

            disc_info[2] |= (DataType << 5);                   /* Disc Information Data Type  */

            // ---------------------------------------------------------------------------------
            // Byte[3] : Number of First Track on Disc
            disc_info[3] = pdbGetBDLowestRecordedTrackNumber();

            // ---------------------------------------------------------------------------------
            // Byte[4] : Number of Sessions (LSB)
            // Byte[9] : Number of Sessions (MSB)
            disc_info[4] = pdbGetBDFinalSessionNumber();

            // ---------------------------------------------------------------------------------
            // Byte[5]  : First Track Number in Last Session (LSB)
            // Byte[10] : First Track Number in Last Session (MSB)
            disc_info[5] = pdbGetBDFinalSessFirstTrackNumber();
            disc_info[10] = 0;

            // ---------------------------------------------------------------------------------
            // Byte[6]  : Last Track Number in Last Session (LSB)
            // Byte[11] : Last Track Number in Last Session (MSB)
            disc_info[6] = pdbGetBDFinalSessLastTrackNumber();
            disc_info[11] =  0;

            // ---------------------------------------------------------------------------------
            // Byte[7] : Bit7=DID_V, Bit6-DBC_V, Bit5-URU,
            //           Bit4-DAC_V, Bit2-Dbit, Bit1,0-BGFormatStatus
            // *** BD-R: Bit7-DID_V = 0, Bit6-DBC_V = 0, Bit5-URU = 1
            //           Bit4-DAC_V = 0, Bit2-Dbit = 0, Bit1,Bit0-BG Format Status = 00b
            disc_info[7] = 0x20;

            //discAppCode = pdbGetBDDiscApplicationCode();
            //if (discAppCode)
            //    disc_info[7] = disc_info[7] | 0x10;

            // ---------------------------------------------------------------------------------
            // Byte[8] : Disc Type
            // As defined in MMC: For BD = 0x00
            // Initialized to 0x00 see above

            // ---------------------------------------------------------------------------------
            // Byte[12]-[15] : Disc Identification
            // This here is a BD. Bytes 12,13,14,15 are 0x00
            // Initialized to 0x00 see above

            // ---------------------------------------------------------------------------------
            // Byte[16]-[19]  : Last Session Lead In Start Address, MMC: BD = 0x00
            // Bytes[20]-[23] : Last Possible Lead Out Start Address, MMC BD = 0x00

            // ---------------------------------------------------------------------------------
            // Byte[24]-[31] : Disc Bar Code, MMC: BD = 0x00

            // ---------------------------------------------------------------------------------
            // Byte[32] : Disc Application Code
            if(pdbGetDiscKind() == eBDROMDisc || pdbGetDiscKind() == eBDROMDLDisc )
            {
                disc_info[32] = 0x00;
            }
            else
            {
                disc_info[32] = discAppCode;
            }

            // ---------------------------------------------------------------------------------
            // Byte[33] : Number of OPC Tables
            disc_info[33] = NumberOfOpcTableEntries;

            if ( NumberOfOpcTableEntries > 0 )
            {
                //Wait till OPC table is finialized.
            }

            //------------------------------------------
            // Transfer Return Data from Array Into DRAM
            //------------------------------------------
            Iram_Buf_dram_addr = DISC_INFO_ADR;

            for(Index=0; Index < DiscInfoTotalLength; Index++)
                WriteSDRAM(disc_info[Index]);

            #if (PCB == 815) //Please turn it on manually for debugging
            #if (MSG_OUT_MODE == MSG_USBPORT)
            {
                BYTE idx;
                send_msg54(SEND_READ_DVD_MSG, 'D', 's', 'c', 'I');
                for (idx = 0; idx < 32; idx += 4)
                    send_msg54(SEND_READ_DVD_MSG, disc_info[idx+0], disc_info[idx+1], disc_info[idx+2], disc_info[idx+3]);
            }
            #endif
            #endif
            SendParameterDataToHost(DISC_INFO_ADR, DiscInfoLength, TRUE);  // Send Data transfer to host with Command Complete.
            break;

        default:
            InvalidCommand();
            break;
    }

}

#endif  //(BD_ENABLE)
