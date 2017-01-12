/****************************************************************************
*                (c) Copyright 2005 - 2008 SUNEXT DESIGN, INC.
*                           All Rights Reserved
*
* FILENAME:     dvrRdDisc.c
*
* DESCRIPTION : This module contains functions related to the
*               Read Disc Information command.
*
*
* NOTES:
*
*   $Revision: 56 $
*   $Date: 11/04/25 6:11p $
*
************************ SUNEXT CONFIDENTIAL *******************************/

// *************************************************************************
//                               HEADERS
// *************************************************************************
#include ".\common\globals.h"
#include ".\common\globtype.h"
#include ".\servo\svo.h"
#include ".\player\plrDb.h"
#include ".\hif\mmc.h"
#include ".\hif\at_knl.h"
#include ".\hif\at_util.h"
#include ".\dvd\dvrddisc.h"

// *************************************************************************
//                               EXTERNS
// *************************************************************************
extern volatile CMD_BLOCK_T Iram_Cmd_block;
extern BYTE disc_info[];

// *************************************************************************
//                              FUNCTIONS
// *************************************************************************

/*--------------------------------------------------------------------
  Name: DVDReadDiscInfoCommand()
  Description:
    Processes ATAPI Read Disc Info command for DVD
  Inputs:
    none
  Outputs:
    none
  Returns:
    none
----------------------------------------------------------------------*/
void DVDReadDiscInfoCommand(void)
{
    StLongU TempAddress;
    LONG    tempAddr1, tempAddr2;
    USHORT  DiscInfoTotalLength, DiscInfoLength, DiscInfoOpcLength;
    BYTE    NumberOfOpcTableEntries, Index, discStatus, discAppCode, DataType;

#if (SUPPORT_DVD_WRITE_ABILITY == P_SL_DISC)
    if ((pdbServoGetDiscDiameter() != eNoDisc) &&(pdbServoGetDiscType() == eLPPDisc)&&
        ( St_blank_disc == TRUE))
#elif (SUPPORT_DVD_WRITE_ABILITY == PM_SL_DISC)
    if ((pdbServoGetDiscDiameter() != eNoDisc) &&(pdbGetDVDLayers() == DUAL_LAYER)&&
        ( St_blank_disc == TRUE))
#elif (SUPPORT_DVD_WRITE_ABILITY == P_DL_DISC)
    if ((pdbServoGetDiscDiameter() != eNoDisc) &&(pdbGetDiscKind()== eDVDMinusRDLDisc)&&
        ( St_blank_disc == TRUE))
#else
    if(0)
#endif
    {
        InvalidCommand();
        return;
    }


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

    for(Index=0;Index<(DiscInfoTotalLength+2);Index++)
        disc_info[Index] = 0;                             // Initialize return data area array to 0.

    switch(DataType)
    {
        case STANDARD_DISC_INFORMATION:
            // ---------------------------------------------------------------------------------
            // Byte[0][1] : Data Length
            if(DiscInfoLength > DiscInfoTotalLength)
                DiscInfoLength = DiscInfoTotalLength;

            disc_info[0] = (BYTE)((DiscInfoTotalLength-2) >> 8);   // The first two bytes
            disc_info[1] = (BYTE) (DiscInfoTotalLength-2);         //   are the Disc Information Length(the number of bytes retured to the Initiator.

            // ---------------------------------------------------------------------------------
            // Byte[2] : Bit4-Erasable, Bits3,2-State of Last Session, Bits1,0-Disc Status
            discStatus = pdbGetDVDFinalSessCloseInfo();
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
                    if(pdbGetDVDFinalSessionNumber() == 1)
                    {
                         disc_info[2] &= 0x00;      // 0000 0000 = Blank Disc
                    }
                    else
                    {
                        // At least 1 Session is CLOSED, Current Session is Empty....
                        tempAddr1 = pdbGetDVDLeadOutStartPSN();
                        tempAddr2 = pdbGetDVDNextSessStartPSN();
                        if ( tempAddr1 <= tempAddr2 )
                            disc_info[2] |= 0x0D;   // 0000 1101 = Complete Session, Incomplete Disc
                        else
                            disc_info[2] |= 0x01;   // 0000 0001 = Empty Session, Incomplete Disc

                    }
                    break;
                default:    // Empty session
                    break;
            }

        #if (SUPPORT_DVD_WRITE_ABILITY == P_SL_DISC)
            if(pdbGetDiscKind() == eDVDPlusRWDisc || pdbGetDiscKind() == eDVDPlusRWDLDisc)
                disc_info[2] |= 0x10;                           /* If DVD+RW, set Eraseable bit */
        #else
            if(pdbGetDVDRW())
                disc_info[2] |= 0x10;                           /* If DVD-RW/+RW, set Eraseable bit */
        #endif

            disc_info[2] |= (DataType << 5);                   /* Disc Information Data Type  */
            // ---------------------------------------------------------------------------------
            // Byte[3] : Number of First Track on Disc
            disc_info[3] = pdbGetDVDLowestRecordedTrackNumber();

            // ---------------------------------------------------------------------------------
            // Byte[4] : Number of Sessions (LSB)
            // Byte[9] : Number of Sessions (MSB)
            disc_info[4] = pdbGetDVDFinalSessionNumber();

            // ---------------------------------------------------------------------------------
            // Byte[5]  : First Track Number in Last Session (LSB)
            // Byte[10] : First Track Number in Last Session (MSB)
            disc_info[5] = pdbGetDVDFinalSessFirstTrackNumber();
            disc_info[10] = 0;

            // ---------------------------------------------------------------------------------
            // Byte[6]  : Last Track Number in Last Session (LSB)
            // Byte[11] : Last Track Number in Last Session (MSB)
            disc_info[6] = pdbGetDVDFinalSessLastTrackNumber();
            disc_info[11] =  0;

            // ---------------------------------------------------------------------------------
            // Byte[7] : Bit7=DID_V, Bit6-DBC_V, Bit5-URU,
            //           Bit4-DAC_V, Bit2-Dbit, Bit1,0-BGFormatStatus
            // *** DVD+R:Bit7-DID_V = 0, Bit6-DBC_V = 0, Bit5-URU = 1
            //           Bit2-Dbit = 0, Bit1,Bit0-BG Format Status = 00b
            #if (CEProduct == 1) && (MTK_BE == 1)
            //for MTK backend
            disc_info[7] = 0x00;
            #else
            disc_info[7] = 0x20;
            #endif


            if(pdbGetDVDType() == DVD_PLUS_RW || pdbGetDVDType() == DVD_PLUS_RW_DUAL)  /* report background format status for DVD+RW */
            {
                    if(St_blank_disc == FALSE)
                    {   /* BGF status only set when disc is not empty */
                        #if (DVD_PLUSRW_BGF == 1)
                        switch (RWDisc.DVDPLUSRW.FormatState)    /* Check DVD+RW Disc Information */
                        {
                            case eDVD_PLUSRW_NOT_FORMATTING:
                                if (DVDPlusRW_GetDiscInfo() & eDVD_PLUSRW_Disc_FullyFormatted)
                                    disc_info[7] |= 0x03;    /* BG format completed */
                                /* BGF currently not running but will restart soon
                                 * (due to write to blank area), so report BGF in progress */
                                else if (fDVDBGFRestartRequest == TRUE)
                                    disc_info[7] |= 0x02;    /* BG format in progress */
                                else
                                    disc_info[7] |= 0x01;    /* BG format currently not running */

                                break;

                            default:
                                disc_info[7] |= 0x02;    /* BG format in progress */
                                break;

                //          case eDVDPLUSRW_BGF_IP:
                //              disc_info[7] |= 0x02;    /* BG format in progress */
                //              break;
                //
                //          case eDVDPLUSRW_BGF_SUSPENDED:
                //              disc_info[7] |= 0x01;    /* BG format currently not running */
                        }
                        #else
                        disc_info[7] |= 0x01;            /* BG format currently not running */
                        #endif // DVD_PLUSRW_BGF
                    }
            }

            // ---------------------------------------------------------------------------------
            // Byte[8] : Disc Type
            // As defined in MMC: For DVD = 0xFF
            // Initialized to 0x00 see above
            // For new spec any none CD disc show disc type is 0
            disc_info[8] = 0x00;

            // ---------------------------------------------------------------------------------
            // Byte[12]-[15] : Disc Identification
            // This here is a DVD. Bytes 12,13,14,15 are 0x00
            // Initialized to 0x00 see above

            // ---------------------------------------------------------------------------------
            // Byte[16]-[19]  : Last Session Lead In Start Address
            // Bytes[20]-[23] : Last Possible Lead Out Start Address
            if (pdbGetDiscKind() == eDVDPlusRDisc || pdbGetDiscKind() == eDVDPlusRDLDisc)
            {
                if (discStatus & 0x02)          // Complete Disc
                {
                    // Written Complete Disc
                    disc_info[16] = 0xFF;       // Last Session Lead-In Start Address.
                    disc_info[17] = 0xFF;
                    disc_info[18] = 0xFF;
                    disc_info[19] = 0xFF;
                    disc_info[20] = 0xFF;       // Last Possible Lead Out Start Address.
                    disc_info[21] = 0xFF;
                    disc_info[22] = 0xFF;
                    disc_info[23] = 0xFF;
                }
                else if ((discStatus == 0x00) && (pdbGetDVDFinalSessionNumber() == 1))
                {
                    // Blank Disc
                    TempAddress.Long = pdbGetDVDLeadInStartPSN()-0x030000;
                    disc_info[16] = TempAddress.byte.u;       // Last Session Lead-In Start Address: 0x2EE80
                    disc_info[17] = TempAddress.byte.h;
                    disc_info[18] = TempAddress.byte.m;
                    disc_info[19] = TempAddress.byte.l;
                    // LBA found in the ADIP of disc's lead in.
                    TempAddress.Long  = pdbGetDVDFinalSessLeadoutLBA();  // Last Possible Lead Out Start Address
                    disc_info[20] = TempAddress.byte.u;                  // Store in Big Endian form.
                    disc_info[21] = TempAddress.byte.h;
                    disc_info[22] = TempAddress.byte.m;
                    disc_info[23] = TempAddress.byte.l;
               }
               else
               {
                    // Written Incomplete Disc
                    #if (CEProduct == 1) && (MTK_BE == 1)
                    // for MTK backend
                    TempAddress.Long  = 0x00L;   // Last Session Lead-In Start Address.
                    #else
                    TempAddress.Long  = pdbGetDVDFinalSessLeadinLBA();   // Last Session Lead-In Start Address.
                    #endif

                    disc_info[16] = TempAddress.byte.u;                  // Store in Big Endian form.
                    disc_info[17] = TempAddress.byte.h;
                    disc_info[18] = TempAddress.byte.m;
                    disc_info[19] = TempAddress.byte.l;
                    // LBA found in the ADIP of disc's lead in.
                    TempAddress.Long  = pdbGetDVDFinalSessLeadoutLBA();  // Last Possible Lead Out Start Address
                    disc_info[20] = TempAddress.byte.u;                  // Store in Big Endian form.
                    disc_info[21] = TempAddress.byte.h;
                    disc_info[22] = TempAddress.byte.m;
                    disc_info[23] = TempAddress.byte.l;
               }
            }
            else
            {
                // For other media types this field shall be filled with zeros. P.S. already done during initialization of array.
            }

            // ---------------------------------------------------------------------------------
            // Byte[24]-[31] : Disc Bar Code
            // Platform cannot read Bar Code. Set bytes to 0x00
            disc_info[24] = 0x00;
            disc_info[25] = 0x00;
            disc_info[26] = 0x00;
            disc_info[27] = 0x00;
            disc_info[28] = 0x00;
            disc_info[29] = 0x00;
            disc_info[30] = 0x00;
            disc_info[31] = 0x00;

            // ---------------------------------------------------------------------------------
            // Byte[32] : Disc Application Code
            if(pdbGetDVDType() == DVD_ROM)
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

            #if (MSG_OUT_MODE == MSG_USBPORT)
            {
                BYTE idx;
                send_msg54(SEND_READ_DVD_MSG, 'D', 's', 'c', 'I');
                for (idx = 0; idx < 32; idx += 4)
                    send_msg54(SEND_READ_DVD_MSG, disc_info[idx+0], disc_info[idx+1], disc_info[idx+2], disc_info[idx+3]);
            }
            #endif
            SendParameterDataToHost(DISC_INFO_ADR, DiscInfoLength, TRUE);  // Send Data transfer to host with Command Complete.
            break;

        case TRACK_RESOURCES_INFORMATION:
        case POW_RESOURCES_INFORMATION:
        default:
            InvalidCommand();
            break;

    }
}

