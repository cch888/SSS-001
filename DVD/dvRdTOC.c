/****************************************************************************
*                (c) Copyright 2005 SUNEXT DESIGN, INC.
*                           All Rights Reserved
*
* FILENAME:     dvrRdTOC.c
*
* DESCRIPTION : This module contains functions related to the
*               Read Table of Contents command.
*
*   $Revision: 31 $
*   $Date: 11/04/25 6:11p $
*
************************ SUNEXT CONFIDENTIAL *******************************/

// *************************************************************************
//                               HEADERS
// *************************************************************************
#include ".\common\globals.h"
#include ".\common\globtype.h"
#include ".\common\Com_im.h"
#include ".\player\plrdb.h"
#include ".\hif\gp_rdtoc.h"
#include ".\hif\at_knl.h"
#include ".\hif\at_util.h"
#include ".\hif\gp_mac.h"
#include ".\hif\bf_util.h"
#include ".\dvd\dvrdtoc.h"
#include ".\dvd\dvutil.h"
#include ".\al\REG_dvd_decoder.h"


// *************************************************************************
//                               DEFINES
// *************************************************************************

// *************************************************************************
//                               EXTERNS
// *************************************************************************

// *************************************************************************
//                              FUNCTIONS
// *************************************************************************

/*--------------------------------------------------------------------
  Function:     DVDReadTOCCommand()

  Description:  Processes ATAPI Read Table of Contents Command.
                     Fabricates TOC data for format 0 and format 1.

  Returns:

  Notes:          Used only when a DVD disc is loaded for compatibility
                     with legacy host environments.
--------------------------------------------------------------------*/
void DVDReadTOCCommand()
{
    TimeS   Ttime;
    StLongU TempL;
    WORD    dataLength,allocLength;
    BYTE    lastTrackNum, firstTrackNum, cdbTrackNum;
    BYTE    tocFormat;
    BYTE    loopCount;
    BYTE    borderCount;
    BYTE    trackno;

    //-------------------------------------------
    // Initialize variables...
    //-------------------------------------------
    /* check for LBA or MSF format, set st_MSF flag if MSF format */
    St_MSF = FALSE;      /* assume MSF format */
    if( TEST_BIT(Iram_Cmd_block.bf[1], 1) ){               /* if MSF format 0:LBA 1:MSF*/
        St_MSF = TRUE;
    }

    tocFormat = ((Iram_Cmd_block.bf[2] & 0x0F) << 4);  // determine TOC Format data to return
    cdbTrackNum = Iram_Cmd_block.bf[6];            // requested track from command block

    if (tocFormat == 0x00)
        tocFormat = ((Iram_Cmd_block.bf[9] & 0xC0) >> 2);

    if (tocFormat == TOC_FORMAT_TOC)
        Iram_Buf_dram_addr = RDTOC_TOC_ADDR;    // bufferArea;    // start address for return data
    else if (tocFormat == TOC_FORMAT_SESSIONINFO)
        Iram_Buf_dram_addr = RDTOC_SESSION_ADDR;
    else
    {
        BUILD_SENSE(0x05,0x24,0x00,0x01);                    // INVALID FIELD IN CDB
        AutoCommandComplete();
        return;
    }
    /*-------------------------------------------------------------------
        Read TOC format 0 and 1 are supported if a DVD media is mounted.
        This is for legacy systems. For other formats the device reports
        an INVALID FIELD IN CDB error. Format 0 and 1 information is
        fabricated for DVD
    ---------------------------------------------------------------------*/
    if (tocFormat > TOC_FORMAT_SESSIONINFO)
    {
        BUILD_SENSE(0x05,0x24,0x00,0x02);              // INVALID FIELD IN CDB
        AutoCommandComplete();
        return;
    }
    //-------------------------------------------
    // Blank disc... return INVALID FIELD
    //-------------------------------------------
    if ((pdbGetDVDFinalSessionNumber() == 1) &&
        ((pdbGetDVDFinalSessCloseInfo() == eEmptySession) || (pdbGetDVDFinalSessCloseInfo() == eNotSessionClosed)))
    {
        BUILD_SENSE(0x05,0x24,0x00,0x03);              // INVALID FIELD IN CDB
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
        if((pdbGetDiscKind() == eDVDPlusRDisc)||(pdbGetDiscKind() == eDVDPlusRWDisc)||(pdbGetDiscKind() == eDVDPlusRDLDisc))
        {
            if ((pdbGetDVDFinalSessCloseInfo() == eSessionFinalized) ||
                (pdbGetDVDFinalSessCloseInfo() == eSessionClosed))
                lastTrackNum = pdbGetDVDFinalSessionNumber();
            else
                lastTrackNum = pdbGetDVDFinalSessionNumber() - 1;

            dataLength = (2 + (lastTrackNum*8) + 8 );/*header + Session descriptor + LeadOut descriptor*/

            firstTrackNum = 1;
            WriteSDRAM(dataLength >> 8);                    // MSB
            WriteSDRAM((BYTE)dataLength);                   // LSB

            WriteSDRAM(firstTrackNum);                      // Byte[2] : First Track Number
            WriteSDRAM(lastTrackNum);                       // Byte[3] : Last Track Number

            for(loopCount = 1; loopCount <= lastTrackNum;  loopCount++)
            {
                WriteSDRAM(0x00);
                WriteSDRAM(0x14);
                
                TempL.Long = dvdConvertPSNtoLBA(pdbGetDVDTrackStartPSN(loopCount));  // Track Start Address

                WriteSDRAM(loopCount);      //session number
                WriteSDRAM(0x00);   //reserve
                if (St_MSF == 0x00)
                {
                    WriteSDRAM(TempL.byte.u);
                    WriteSDRAM(TempL.byte.h);
                    WriteSDRAM(TempL.byte.m);
                    WriteSDRAM(TempL.byte.l);
                }
                else
                {
                    Ttime = LBA2MSF(TempL.Long);
                    WriteSDRAM((BYTE)(0x00));
                    WriteSDRAM((BYTE)(Ttime.Minute));
                    WriteSDRAM((BYTE)(Ttime.Second));
                    WriteSDRAM((BYTE)(Ttime.Frame));
                }
            }
            WriteSDRAM(0x00);                               // Byte[12] : Reserved
            WriteSDRAM(0x14);                               // Byte[13] : Lead-out ADR, CONTROL (14h)
            WriteSDRAM(0xAA);                               // Byte[14] : Lead-out Track Number (AAh)
            WriteSDRAM(0x00);                               // Byte[15] : Reserved

            TempL.Long = dvdConvertPSNtoLBA(pdbGetDVDTrackEndPSN(lastTrackNum)+1);         // Byte[16]-[19] : Lead-out Track Start Address in LBA

            if (St_MSF == 0x00)
            {
                WriteSDRAM(TempL.byte.u);
                WriteSDRAM(TempL.byte.h);
                WriteSDRAM(TempL.byte.m);
                WriteSDRAM(TempL.byte.l);
            }
            else
            {
                Ttime = LBA2MSF(TempL.Long);
                WriteSDRAM((BYTE)(0x00));
                WriteSDRAM((BYTE)(Ttime.Minute));
                WriteSDRAM((BYTE)(Ttime.Second));
                WriteSDRAM((BYTE)(Ttime.Frame));
            }
        }
        else if ((pdbGetDiscKind() == eDVDMinusRDisc) ||(pdbGetDiscKind() == eDVDMinusRDLDisc)||(pdbGetDiscKind() == eDVDMinusRWDisc) )
        {

            if((pdbGetDVDRMADiscStatus() == DiscAtOnce)||(pdbGetDVDRMADiscStatus() == FinalIncDisc))
                borderCount = 0x02;
            else
                borderCount = 0x03;

            if (pdbGetDVDBorderNumber() >= borderCount)
            {
                dataLength   = 0x1A;
                lastTrackNum = 0x02;
                loopCount    = 0x02;
            }
            else
            {
                dataLength   = 0x12;
                lastTrackNum = 0x01;
                loopCount    = 0x01;
            }

            firstTrackNum = 1;

            // ------------------------------------------------------------
            // Bytes[0]-[1] : TOC Data Length
            WriteSDRAM(dataLength >> 8);                    // MSB
            WriteSDRAM((BYTE)dataLength);                   // LSB

            WriteSDRAM(firstTrackNum);                      // Byte[2] : First Track Number
            WriteSDRAM(lastTrackNum);                       // Byte[3] : Last Track Number

            for (loopCount = 1 ; loopCount <= lastTrackNum ; loopCount++)
            {
                WriteSDRAM(0x00);                           // Byte[4] : Reserved
                WriteSDRAM(0x14);                           // Byte[5] : ADR, CONTROL
                WriteSDRAM(loopCount);                      // Byte[6] : Track Number
                WriteSDRAM(0x00);                           // Byte[7] : Reserved

                if (loopCount == 1)
                    TempL.Long  = dvdConvertPSNtoLBA(pdbGetDVDTrackStartPSN(loopCount));  // Byte[8]-[11] : Track Start Address
                else
                {
                    if((pdbGetDVDRMADiscStatus() == DiscAtOnce)||(pdbGetDVDRMADiscStatus() == FinalIncDisc))
                        TempL.Long  = dvdConvertPSNtoLBA(pdbGetDVDTrackStartPSN(pdbGetDVDHighestRecordedTrackNumber()));  // Byte[8]-[11] : Track Start Address
                    else
                        TempL.Long  = dvdConvertPSNtoLBA(pdbGetDVDTrackStartPSN(pdbGetDVDHighestRecordedTrackNumber()));  // Byte[8]-[11] : Track Start Address
                        //TempL.Long  = dvdConvertPSNtoLBA(pdbGetDVDTrackStartPSN(pdbGetDVDHighestRecordedTrackNumber()-1));  // Byte[8]-[11] : Track Start Address
                }

                if (St_MSF == 0x00)
                {
                    WriteSDRAM(TempL.byte.u);
                    WriteSDRAM(TempL.byte.h);
                    WriteSDRAM(TempL.byte.m);
                    WriteSDRAM(TempL.byte.l);
                }
                else
                {
                    Ttime = LBA2MSF(TempL.Long);
                    WriteSDRAM((BYTE)(0x00));
                    WriteSDRAM((BYTE)(Ttime.Minute));
                    WriteSDRAM((BYTE)(Ttime.Second));
                    WriteSDRAM((BYTE)(Ttime.Frame));
                }
            }

            WriteSDRAM(0x00);                               // Byte[12] : Reserved
            WriteSDRAM(0x14);                               // Byte[13] : Lead-out ADR, CONTROL (17h)
            WriteSDRAM(0xAA);                               // Byte[14] : Lead-out Track Number (AAh)
            WriteSDRAM(0x00);                               // Byte[15] : Reserved

            if (Info.Media.DVD.RMAData.RMD.Format2.DiscStatus == RestricedOverWrite && pdbGetDVDRW() == TRUE)
            {
                TempL.Long = dvdConvertPSNtoLBA(pdbGetDVDTrackEndPSN(pdbGetDVDHighestRecordedTrackNumber())+1);
            }
            else
            {
                if ((lastTrackNum != 0x02) && (pdbGetDiscStatus() == eNonFinalizedDisc))
                    TempL.Long = dvdConvertPSNtoLBA(pdbGetDVDTrackEndPSN(1)+1);         // Byte[16]-[19] : Lead-out Track Start Address in LBA
                else
                {
                        if((pdbGetDVDRMARMDFormat() == Format4_RMD) && (pdbGetDiscKind() == eDVDMinusRDLDisc))
                            TempL.Long = dvdConvertPSNtoLBA(pdbGetDVDTrackEndPSN(1)+1);         // Byte[16]-[19] : Lead-out Track Start Address in LBA
                        else
                            TempL.Long = dvdConvertPSNtoLBA(pdbGetDVDTrackEndPSN(pdbGetDVDHighestRecordedTrackNumber())+1);         // Byte[16]-[19] : Lead-out Track Start Address in LBA
                    }	
                }

            if (St_MSF == 0x00)
            {
                WriteSDRAM(TempL.byte.u);
                WriteSDRAM(TempL.byte.h);
                WriteSDRAM(TempL.byte.m);
                WriteSDRAM(TempL.byte.l);
            }
            else
            {
                Ttime = LBA2MSF(TempL.Long);
                WriteSDRAM((BYTE)(0x00));
                WriteSDRAM((BYTE)(Ttime.Minute));
                WriteSDRAM((BYTE)(Ttime.Second));
                WriteSDRAM((BYTE)(Ttime.Frame));
            }
        }
         else/* Other Kind Disc */
        {
            dataLength   = 0x12;
            firstTrackNum = 1;
            lastTrackNum =1;
        // ------------------------------------------------------------
        // Bytes[0]-[1] : TOC Data Length
            WriteSDRAM(dataLength >> 8);                    // MSB
            WriteSDRAM(dataLength);                         // LSB

            WriteSDRAM(firstTrackNum);                      // Byte[2] : First Track Number
            WriteSDRAM(lastTrackNum);                       // Byte[3] : Last Track Number

            WriteSDRAM(0x00);                               // Byte[4] : Reserved
            WriteSDRAM(0x14);                               // Byte[5] : ADR, CONTROL
            WriteSDRAM(0x01);                               // Byte[6] : Track Number
            WriteSDRAM(0x00);

            if (St_MSF == 0x00)// LBA                       // Byte[8]-[11] : Track Start Address
            {
                WriteSDRAM(0x00);
                WriteSDRAM(0x00);
                WriteSDRAM(0x00);
                WriteSDRAM(0x00);
            }
            else
            {
                WriteSDRAM(0x00);
                WriteSDRAM(0x00);
                WriteSDRAM(0x02);
                WriteSDRAM(0x00);
            }
            WriteSDRAM(0x00);                               // Byte[12] : Reserved
            WriteSDRAM(0x14);                               // Byte[13] : Lead-out ADR, CONTROL (14h)
            WriteSDRAM(0xAA);                               // Byte[14] : Lead-out Track Number (AAh)
            WriteSDRAM(0x00);                               // Byte[15] : Reserved

            TempL.Long = pdbGetDVDMaxLBA()+1;
            if (St_MSF == 0x00)// LBA                       // Byte[16]-[19] : Lead-out Track Start Address
            {
                WriteSDRAM(TempL.byte.u);
                WriteSDRAM(TempL.byte.h);
                WriteSDRAM(TempL.byte.m);
                WriteSDRAM(TempL.byte.l);
            }
            else
            {
                Ttime = LBA2MSF(TempL.Long);
                WriteSDRAM((BYTE)(0x00));
                WriteSDRAM((BYTE)(Ttime.Minute));
                WriteSDRAM((BYTE)(Ttime.Second));
                WriteSDRAM((BYTE)(Ttime.Frame));
            }
        }
    }
    else if (tocFormat == TOC_FORMAT_SESSIONINFO)
    {
        /*------------------------------------------------------------------------*/
        /*                     DVD FORMAT 1 : SESSION INFO                        */
        /*------------------------------------------------------------------------*/
        // Bytes[0]-[1] : TOC Data Length (0Ah)
        dataLength = 0x0A;                                  // do not include data length field of 2 bytes
        WriteSDRAM(0x00);
        WriteSDRAM(dataLength);
        // ------------------------------------------------------------
        // Byte[2] : First Complete Track Number
        // Byte[3] : Last Complete Track Number
        // ------------------------------------------------------------
        WriteSDRAM(0x01);

        if ((pdbGetDVDFinalSessCloseInfo() == eSessionFinalized) ||
            (pdbGetDVDFinalSessCloseInfo() == eSessionClosed))
            lastTrackNum = pdbGetDVDFinalSessionNumber();
        else
            lastTrackNum = pdbGetDVDFinalSessionNumber() - 1;

        if ((pdbGetDiscKind() == eDVDMinusRDisc) ||(pdbGetDiscKind() == eDVDMinusRDLDisc)||(pdbGetDiscKind() == eDVDMinusRWDisc) )
        {
            if((pdbGetDVDRMADiscStatus() == DiscAtOnce)||(pdbGetDVDRMADiscStatus() == FinalIncDisc))
                borderCount = 0x02;
            else
                borderCount = 0x03;

            if (pdbGetDVDBorderNumber() >= borderCount)
                WriteSDRAM(0x02);
            else
                WriteSDRAM(0x01);
        }
        else
            WriteSDRAM(lastTrackNum);

        // ------------------------------------------------------------
        // Byte[4] : Reserved
        // ------------------------------------------------------------
        WriteSDRAM(0x00);

        // ------------------------------------------------------------
        // Byte[5] : ADR, CONTROL
        // ------------------------------------------------------------
        if ((pdbGetDiscKind() == eDVDPlusRDisc)||(pdbGetDiscKind() == eDVDPlusRWDisc)||(pdbGetDiscKind() == eDVDPlusRDLDisc))
            WriteSDRAM(0x17);
        else
            WriteSDRAM(0x14);

        // ------------------------------------------------------------
        // Byte[6] : Track Number
        // ------------------------------------------------------------
        if ((pdbGetDiscKind() == eDVDMinusRDisc) ||(pdbGetDiscKind() == eDVDMinusRDLDisc)||(pdbGetDiscKind() == eDVDMinusRWDisc) )
        {
            if((pdbGetDVDRMADiscStatus() == DiscAtOnce)||(pdbGetDVDRMADiscStatus() == FinalIncDisc))
                borderCount = 0x02;
            else
                borderCount = 0x03;

            if (pdbGetDVDBorderNumber() >= borderCount)
                WriteSDRAM(0x02);
            else
                WriteSDRAM(0x01);
        }
        else
            WriteSDRAM(lastTrackNum);

        // ------------------------------------------------------------
        // Byte[7] : Reserved
        // ------------------------------------------------------------
        WriteSDRAM(0x00);

        // ------------------------------------------------------------
        // Byte[8]-[11] : Track Start Address
        // ------------------------------------------------------------
        if ((pdbGetDiscKind() == eDVDMinusRDisc) ||(pdbGetDiscKind() == eDVDMinusRDLDisc)||(pdbGetDiscKind() == eDVDMinusRWDisc) )
        {
            if(pdbGetDVDRMADiscStatus() == FinalIncDisc)
            {
                if (pdbGetDVDBorderNumber() >= 0x02)
                    TempL.Long = dvdConvertPSNtoLBA(pdbGetDVDTrackStartPSN(pdbGetDVDHighestRecordedTrackNumber()));         // Byte[16]-[19] : Lead-out Track Start Address in LBA
                else
                    TempL.Long = 0x00L;         // Byte[16]-[19] : Lead-out Track Start Address in LBA
            }
            else
            {
                trackno = pdbGetDVDHighestRecordedTrackNumber();
                if (pdbGetDVDFinalSessionNumber() >= 0x03)
                {
                    do
                    {
                        if (pdbGetDVDBorderOutPSN(pdbGetDVDFinalSessionNumber() - 1) < pdbGetDVDTrackStartPSN(trackno))
                            trackno--;
                        else
                        {
                            trackno++;
                            break;
                        }
                    }while(trackno>= 1);

                    TempL.Long = dvdConvertPSNtoLBA(pdbGetDVDTrackStartPSN(trackno));         // Byte[16]-[19] : Lead-out Track Start Address in LBA
                }
                else
                    TempL.Long = 0x00L;         // Byte[16]-[19] : Lead-out Track Start Address in LBA
            }
        }
        else
            TempL.Long = dvdConvertPSNtoLBA(pdbGetDVDTrackStartPSN(lastTrackNum));      // Track Start Address

        if (St_MSF == 0x00)
        {
            WriteSDRAM(TempL.byte.u);
            WriteSDRAM(TempL.byte.h);
            WriteSDRAM(TempL.byte.m);
            WriteSDRAM(TempL.byte.l);
        }
        else
        {
            Ttime = LBA2MSF(TempL.Long);
            WriteSDRAM((BYTE)(0x00));
            WriteSDRAM((BYTE)(Ttime.Minute));
            WriteSDRAM((BYTE)(Ttime.Second));
            WriteSDRAM((BYTE)(Ttime.Frame));
        }
    }

    //-------------------------------------------
    // Transfer Data to Host...
    //-------------------------------------------

    if(dataLength == 0x0000)
    {
        BUILD_SENSE(0x05,0x24,0x00,0x04);          /* INVALID FIELD IN CDB */
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

    if(allocLength == 0xFFFE)
    { 
        if(dataLength < 0x30)
        {
            BYTE i,j;
            j = 0x30;
            for(i=0; i<j; i++)
                WriteSDRAM(0);	
            dataLength = 0x30;
        }
    }
    
    SendParameterDataToHost(Iram_Linear_Xfer_addr, MINIMUM( dataLength, allocLength), TRUE);   // Send data to host. Send Command Complete
}

