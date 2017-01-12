/*****************************************************************************
*                (c) Copyright 2000 - 2007 SUNEXT DESIGN, INC.
*                            All Rights Reserved
*
*
* FILENAME:     $Workfile: bdRdTOC.c $
*
* DESCRIPTION   This module contains functions related to the
*               Read Table of Contents command.
*
* $Revision: 10 $
* $Date: 09/08/20 6:55p $
*
**************************** SUNEXT CONFIDENTIAL ****************************/

// ***************************************************************************
//                               HEADERS
// ***************************************************************************
#include ".\bd\bdRdTOC.h"
#include ".\bd\bdVars.h"

#if (BD_ENABLE)
// ***************************************************************************
//                               DEFINES
// ***************************************************************************

// ***************************************************************************
//                               EXTERNS
// ***************************************************************************

// ***************************************************************************
//                              FUNCTIONS
// ***************************************************************************

/*****************************************************************************

    FUNCTION        BDReadTOCCommand

    DESCRIPTION     Processes ATAPI Read Table of Contents Command.
                    Fabricates TOC data for format 0 and format 1.

    PARAMETERS

    RETURNS

    NOTES           Used only when a BD disc is loaded for compatibility
                    with legacy host environments.

*****************************************************************************/
void BDReadTOCCommand(void)
{
    TimeS   Ttime;
    StLongU TempL;
    WORD    dataLength,allocLength, i;
    BYTE    lastTrackNum, firstTrackNum, cdbTrackNum;
    BYTE    tocFormat;
    BYTE    loopCount;

    //-------------------------------------------
    // Initialize variables ...
    //-------------------------------------------
    /* check for LBA or MSF format, set st_MSF flag if MSF format */
    St_MSF = FALSE;      /* assume MSF format */
    if( TEST_BIT(Iram_Cmd_block.bf[1], 1) ){            /* if MSF format 0:LBA 1:MSF*/
        St_MSF = TRUE;
    }

    tocFormat = ((Iram_Cmd_block.bf[2] & 0x0F) << 4);   // determine TOC Format data to return

    if (tocFormat == 0x00)
        tocFormat = ((Iram_Cmd_block.bf[9] & 0xC0) >> 2);

    cdbTrackNum = Iram_Cmd_block.bf[6];                 // requested track from command block
    dataLength = (Iram_Cmd_block.bf[7] << 8) + Iram_Cmd_block.bf[8];

    /*-------------------------------------------------------------------
        Read TOC format 0 and 1 are supported if a BD media is mounted.
        This is for legacy systems. For other formats the device reports
        an INVALID FIELD IN CDB error. Format 0 and 1 information is
        fabricated for BD
    ---------------------------------------------------------------------*/
    if (tocFormat == TOC_FORMAT_TOC)
    {
        Iram_Buf_dram_addr = RDTOC_TOC_ADDR;    // start address for return data
        for(i= 0; i < dataLength; i++)
            WriteSDRAM(0x00);

        Iram_Buf_dram_addr = RDTOC_TOC_ADDR;    // start address for return data
    }
    else if (tocFormat == TOC_FORMAT_SESSIONINFO)
    {
        Iram_Buf_dram_addr = RDTOC_SESSION_ADDR;
    }
    else
    {
        BUILD_SENSE(0x05,0x24,0x00,0x95);       // INVALID FIELD IN CDB
        AutoCommandComplete();
        return;
    }

    //-------------------------------------------
    // Blank disc... return INVALID FIELD
    //-------------------------------------------
    if ((pdbGetBDFinalSessionNumber() == 1) &&
        ((pdbGetBDFinalSessCloseInfo() == eEmptySession)))// || (pdbGetBDFinalSessCloseInfo() == eNotSessionClosed))
    {
        BUILD_SENSE(0x05,0x24,0x00,0x96);       // INVALID FIELD IN CDB
        AutoCommandComplete();
        return;
    }
    //-------------------------------------------
    // Prepare Return Data...
    //-------------------------------------------

    if (tocFormat == TOC_FORMAT_TOC)
    {
        /*------------------------------------------------------------------------*/
        /*                      DVD FORMAT 0 : TOC INFO                           */
        /*------------------------------------------------------------------------*/
        switch(pdbGetDiscKind())
        {
            case eBDROMDisc:
            case eBDROMDLDisc:
            case eBDREDisc:
            case eBDREDLDisc:
                dataLength = 0x12;
                WriteSDRAM(dataLength >> 8);    // Byte0: length MSB
                WriteSDRAM(dataLength);         // Byte1: length LSB
                WriteSDRAM(0x01);               // Byte2: first track
                WriteSDRAM(0x01);               // Byte3: last track
                WriteSDRAM(0x00);               // Byte4: reserved
                WriteSDRAM(0x14);               // Byte5: ADR/CTL
                WriteSDRAM(0x01);               // Byte6: track number
                WriteSDRAM(0x00);               // Byte7: reserved

                if(St_MSF)                      // Byte8-11: length MSB
                {   /* MSF */
                    WriteSDRAM(0x00);
                    WriteSDRAM(0x00);
                    WriteSDRAM(0x02);
                    WriteSDRAM(0x00);
                }
                else
                {   /* LBA */
                    WriteSDRAM(0x00);
                    WriteSDRAM(0x00);
                    WriteSDRAM(0x00);
                    WriteSDRAM(0x00);
                }


                WriteSDRAM(0x00);               // Byte12: reserved
                WriteSDRAM(0x14);               // Byte13: ADR/CTL
                WriteSDRAM(0xaa);               // Byte14: track number (AA: LeadOut)
                WriteSDRAM(0x00);               // Byte15: reserved

                TempL.Long = Info.Media.BD.MaxLBA + 1L;
                if(St_MSF)                      // Byte16: length LSB
                {   /* MSF */
                    Ttime = LBA2MSF((long)(TempL.Long));
                    WriteSDRAM(0x00);
                    WriteSDRAM(Ttime.Minute);
                    WriteSDRAM(Ttime.Second);
                    WriteSDRAM(Ttime.Frame);
                }
                else
                {   /* LBA */
                    WriteSDRAM(TempL.byte.u);
                    WriteSDRAM(TempL.byte.h);
                    WriteSDRAM(TempL.byte.m);
                    WriteSDRAM(TempL.byte.l);
                }
                break;

            case eBDRDisc:
            case eBDRDLDisc:
                switch(pdbGetBDRecMode())
                {
                    case SRM_NO_LOW:
                        /* last track = last closed session # */
                        if ((pdbGetBDFinalSessCloseInfo() == eSessionFinalized) ||
                            (pdbGetBDFinalSessCloseInfo() == eSessionClosed))
                        {
                            lastTrackNum = pdbGetBDFinalSessionNumber();
                        }
                        else
                        {
                            if(pdbGetBDFinalSessionNumber()==1)
                            {
                                lastTrackNum = 1;
                            }
                            else
                            {
                                lastTrackNum = pdbGetBDFinalSessionNumber() - 1;
                            }
                        }
                        if(lastTrackNum == 1)
                            dataLength = (2 + 8 + 8 );      /* header + 1 Session descriptor + LeadOut descriptor*/
                        else
                            dataLength = (2 + 2 * 8 + 8 );  /*header + 2 Session descriptor + LeadOut descriptor*/

                        firstTrackNum = 1;
                        WriteSDRAM(dataLength >> 8);    // Byte0: MSB of length
                        WriteSDRAM((BYTE)dataLength);   // Byte1: LSB of length

                        WriteSDRAM(firstTrackNum);      // Byte2: First Track Number
                        if(lastTrackNum == 1)
                            WriteSDRAM(0x01);           // Byte3: Last Track Number
                        else
                            WriteSDRAM(0x02);           // Byte3: Last Track Number

                        for(loopCount = 1; loopCount <= 2;  loopCount++)
                        {
                            WriteSDRAM(0x00);
                            WriteSDRAM(0x14);

                            if(loopCount == 1)  // first N-1 sessions viewed as track 1
                                TempL.Long = bdConvertPSNtoLBA(pdbGetBDTrackStartPSN(firstTrackNum));  // Track Start Address
                            else
                                TempL.Long = bdConvertPSNtoLBA(pdbGetBDTrackStartPSN(lastTrackNum));  // Track Start Address

                            WriteSDRAM(loopCount);      //session number
                            WriteSDRAM(0x00);           //reserved
                            if (St_MSF)
                            {   /* MSF format */
                                Ttime = LBA2MSF(TempL.Long);
                                WriteSDRAM((BYTE)(0x00));
                                WriteSDRAM((BYTE)(Ttime.Minute));
                                WriteSDRAM((BYTE)(Ttime.Second));
                                WriteSDRAM((BYTE)(Ttime.Frame));
                            }
                            else
                            {   /* LBA format */
                                WriteSDRAM(TempL.byte.u);
                                WriteSDRAM(TempL.byte.h);
                                WriteSDRAM(TempL.byte.m);
                                WriteSDRAM(TempL.byte.l);
                            }

                            if(lastTrackNum == 1)
                            {
                                send_msg5S(SEND_READ_DVD_MSG, "one track");
                                break;
                            }
                        }
                        break;

                    case SRM_AND_LOW:   //view as a closed disc with 1 session
                        dataLength = 0x12;
                        /* Header */
                        WriteSDRAM(dataLength >> 8);    // Byte0: MSB of length
                        WriteSDRAM((BYTE)dataLength);   // Byte1: LSB of length
                        WriteSDRAM(0x01);   // Byte2: First Track Number
                        WriteSDRAM(0x01);   // Byte3: Last Track Number

                        /* Track 1 */
                        WriteSDRAM(0x00);   // Byte0: MSB of length
                        WriteSDRAM(0x14);   // Byte1: LSB of length
                        WriteSDRAM(0x01);   // Byte2: First Track Number
                        WriteSDRAM(0x01);   // Byte3: Last Track Number

                        if (St_MSF)
                        {   /* MSF format */
                            WriteSDRAM(0x00);
                            WriteSDRAM(0x02);
                            WriteSDRAM(0x00);
                            WriteSDRAM(0x00);
                        }
                        else
                        {   /* LBA format */
                            WriteSDRAM(0x00);
                            WriteSDRAM(0x00);
                            WriteSDRAM(0x00);
                            WriteSDRAM(0x00);
                        }
                        break;

                    default:
                        send_msg5S(SEND_READ_DVD_MSG, "! support");
                        BUILD_SENSE(0x05,0x24,0x00,0x90);   // INVALID FIELD IN CDB
                        AutoCommandComplete();
                        return;
                }

                TempL.Long = pdbGetBDEndLSN() + 1;

                /* LeadOut descriptor */
                WriteSDRAM(0x00);           // Byte12/20: Reserved
                WriteSDRAM(0x14);           // Byte13/21: Lead-out ADR, CONTROL (14h)
                WriteSDRAM(0xAA);           // Byte14/22: Lead-out Track Number (AAh)
                WriteSDRAM(0x00);           // Byte15/23: Reserved

                if (St_MSF)                     // Byte16-19: Lead-out Track Start Address
                {   /* MSF format */
                    Ttime = LBA2MSF(TempL.Long);
                    WriteSDRAM((BYTE)(0x00));
                    WriteSDRAM((BYTE)(Ttime.Minute));
                    WriteSDRAM((BYTE)(Ttime.Second));
                    WriteSDRAM((BYTE)(Ttime.Frame));
                }
                else
                {   /* LBA format */
                    WriteSDRAM(TempL.byte.u);
                    WriteSDRAM(TempL.byte.h);
                    WriteSDRAM(TempL.byte.m);
                    WriteSDRAM(TempL.byte.l);
                }
                break;
        }
    }
    else if (tocFormat == TOC_FORMAT_SESSIONINFO)
    {
        /*------------------------------------------------------------------------*/
        /*                     DVD FORMAT 1 : SESSION INFO                        */
        /*------------------------------------------------------------------------*/
        dataLength = 0x0A;
        WriteSDRAM(dataLength >> 8);            // Byte0: length MSB
        WriteSDRAM(dataLength);                 // Byte1: length LSB
        WriteSDRAM(0x01);                       // Byte2: first session number
        WriteSDRAM(0x01);                       // Byte3: last session number
        WriteSDRAM(0x00);                       // Byte4: reserved
        WriteSDRAM(0x14);                       // Byte5: ADR/CTL
        WriteSDRAM(0x01);                       // Byte6: first track number in last complete
        WriteSDRAM(0x00);                       // Byte7: reserved

        if(St_MSF)                              // Byte8-11: length MSB
        {   /* MSF */
            WriteSDRAM(0x00);
            WriteSDRAM(0x00);
            WriteSDRAM(0x02);
            WriteSDRAM(0x00);
        }
        else
        {   /* LBA */
            WriteSDRAM(0x00);
            WriteSDRAM(0x00);
            WriteSDRAM(0x00);
            WriteSDRAM(0x00);
        }
    }

    //-------------------------------------------
    // Transfer Data to Host ...
    //-------------------------------------------

    if(dataLength == 0x0000)
    {
        BUILD_SENSE(0x05,0x24,0x00,0x98);       /* INVALID FIELD IN CDB */
        AutoCommandComplete();
        return;
    }

    if(dataLength & 0x0001)
        dataLength++;

    dataLength += 2;
    BHIGH(allocLength) = Iram_Cmd_block.bf[7];
    BLOW (allocLength) = Iram_Cmd_block.bf[8];

    /* transfer paramater data to host and completion the command */

    if (tocFormat == TOC_FORMAT_TOC)
        Iram_Linear_Xfer_addr = RDTOC_TOC_ADDR;
    else
        Iram_Linear_Xfer_addr = RDTOC_SESSION_ADDR;

    SendParameterDataToHost(Iram_Linear_Xfer_addr, MINIMUM( dataLength, allocLength), TRUE);   // Send data to host. Send Command Complete
}

#endif  //(BD_ENABLE)
