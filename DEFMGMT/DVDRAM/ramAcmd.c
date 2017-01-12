/****************************************************************************
*              (c) Copyright 2001 - 2010 Sunext Technology Co., Ltd.
*                            All Right Reserved
*
*
* FILENAME:   ramAcmd.c
*
* DESCRIPTION
*   This file contains handlers of commands other than read/write for DVD-RAM
*
*   $Revision: 2 $
*   $Date: 10/02/21 11:34a $
*
**************************** SUNEXT CONFIDENTIAL ***************************/

#include ".\common\globtype.h"
#include ".\common\globals.h"
#include ".\common\chip.h"
#include ".\common\error.h"
#include ".\common\commath.h"
#include ".\common\system.h"
#include ".\common\stdlib.h"
#include ".\hif\mmc.h"
#include ".\hif\gp_equ.h"
#include ".\hif\at_equ.h"
#include ".\hif\gp_mac.h"
#include ".\hif\app_if.h"
#include ".\hif\at_knl.h"
#include ".\hif\gp_util.h"
#include ".\hif\sv_util.h"
#include ".\hif\gp_rdtoc.h"
#include ".\dvd\dvrddisc.h"

#if (DVD_RAM_READ == 1)
#include ".\defmgmt\dvdram\dvDefMgr.h"
#include ".\defmgmt\common\mrw_ram.h"
#include ".\defmgmt\dvdram\ramread.h"
#include ".\defmgmt\dvdram\ramExtern.h"


/*************************************************************************/
/*                         Declarations                                  */
/*************************************************************************/
BOOL CheckDVDRAMCommand(void);
void ReadDVDRAMTOC(void);
void DVDRAMReadDiscInfoCommand(void);
void DVDRAMReadTrackInfoCommand(void);
void DVDRAMVerify10Command(void);

extern BYTE disc_info[];
extern USHORT   track_info_len;
extern BYTE     track_info_data[];
/*************************************************************************/
/*                         Defines                                       */
/*************************************************************************/


/*************************************************************************/
/*                         Functions                                     */
/*************************************************************************/

/*******************************************************************************
  Function: CheckDVDRAMCommand()
  Description:
      Dispatcher for commands that need special process for DVD-RAM disc
  Input    : None
  Output   : TRUE  - Commands have been processed by DVD-RAM handler
             FALSE - Commands for normal process
*******************************************************************************/
BOOL CheckDVDRAMCommand(void)
{
    BYTE cmd;

    cmd = Iram_Cmd_block.bf[0];

    /* When disc's DMA is not reliable, some commands should be reject */
    if ((RamDiscStatus == eDiscDMAFail) || (RamDiscStatus == eDiscInProgress))
    {
        switch (cmd)
        {
            case SBC_READ_10:
            case SBC_READ_12:
            case SBC_VERIFY_10:
            case MMC_READ_TOC:
            case MMC_READ_TRACK_INFO:
            case MMC_WRITE_10:
            case MMC_WRITE_12:
            case MMC_WRITE_VERIFY_10:
            case MMC_SYNC_CACHE:
            case MMC_READ_CAPACITY:
            case MMC_SET_STREAMING:
                BUILD_SENSE(0x03,0x31,0x00,0x01);  // Medium Format Corrupted
                AutoCommandComplete();
                return TRUE;

            default:
                break;
        }
    }

    switch (cmd)
    {
        case SBC_READ_10:
        case SBC_READ_12:
            Cmd_read_10_DVDRAM();
            break;

        case SBC_VERIFY_10:
            DVDRAMVerify10Command();
            break;

        case MMC_READ_TOC:
            ReadDVDRAMTOC();
            break;

        case MMC_READ_DISK_INFO:
            DVDRAMReadDiscInfoCommand();
            break;

        case  MMC_READ_TRACK_INFO:
            DVDRAMReadTrackInfoCommand();
            break;

        default:
            return FALSE;
    }

    return TRUE;
}

/*******************************************************************************
  Function: ReadDVDRAMTOC
  Description:
      Read_Toc command handler for DVD-RAM disc
  Input:    None
  Output:   TRUE/FALSE
*******************************************************************************/
void ReadDVDRAMTOC(void)
{
    ULONG   XferAddress;
    USHORT  XferLength;
    BYTE    toc_format;
    LONG    LeadoutStartAddr;
    TimeS   msf;
    USHORT  toc_length;

    St_MSF = FALSE;      /* assume MSF format */
    if( TEST_BIT(Iram_Cmd_block.bf[1], 1) ){               /* if MSF format 0:LBA 1:MSF*/
        St_MSF = TRUE;
    }

    /* check for requested format (0, 1, or 2) */
    toc_format = (Iram_Cmd_block.bf[2] << 4) | ((Iram_Cmd_block.bf[9] & 0xc0) >> 2);

    /*-------------------------------------------------------------------
        Read TOC format 0 and 1 are supported if a DVD media is mounted.
        This is for legacy systems. For other formats the device reports
        an INVALID FIELD IN CDB error. Format 0 and 1 information is
        fabricated for DVD
    ---------------------------------------------------------------------*/
    if (toc_format > TOC_FORMAT_SESSIONINFO)
    {
        BUILD_SENSE(0x05,0x24,0x00,0x8d);              // INVALID FIELD IN CDB
        AutoCommandComplete();
        return;
    }


    switch (toc_format)
    {
        case 0:                                 /* toc format 0 (TOC ) */
            toc_length = 0x14;

            Iram_Buf_dram_addr = DVDRAM_TOC_00_ADR;

            WriteSDRAM(0);                         /* 0:Length(h) = 0 */
            WriteSDRAM(0x12);                      /* 1:Length(L) = 18 */
            WriteSDRAM(1);                         /* 2:First Track Number = 1 */
            WriteSDRAM(1);                         /* 3:Last  Track Number = 1 */
            WriteSDRAM(0);                         /* 4:Reserve */
            WriteSDRAM(0x14);                      /* 5:Addr = 0001b, Control = 0100b */
            WriteSDRAM(1);                         /* 6:Track Numer = 1 */
            WriteSDRAM(0);                         /* 7:Reserve */

            if (St_MSF == TRUE)
            {
                WriteSDRAM(0x00);                  /* 8:  */
                WriteSDRAM(0x00);                  /* 9:  */
                WriteSDRAM(0x02);                  /* 10: */
                WriteSDRAM(0x00);                  /* 11: */
            }
            else
            {
                WriteSDRAM(0x00);                  /* 8:  */
                WriteSDRAM(0x00);                  /* 9:  */
                WriteSDRAM(0x00);                  /* 10: */
                WriteSDRAM(0x00);                  /* 11: */
            }

            WriteSDRAM(0x00);                      /* 12:Reserve */
            WriteSDRAM(0x14);                      /* 13:Addr = 0001b, Control = 0100b */
            WriteSDRAM(0xAA);                      /* 14:Track Numer = AA */
            WriteSDRAM(0x00);                      /* 15:  */


            LeadoutStartAddr = GetDVDRAMMaxLBA() + 1;

            Iram_Buf_dram_addr = DVDRAM_TOC_00_ADR+16;

            if (St_MSF == TRUE) /* Time bit = 1 ... Method 2 -> 1 .. return with  MSF */
            {
                msf = LBA2MSF(LeadoutStartAddr);
                WriteSDRAM (0);                /* 16: Reserve */
                WriteSDRAM (msf.Minute);       /* 17: M */
                WriteSDRAM (msf.Second);       /* 18: S */
                WriteSDRAM (msf.Frame);        /* 19: F */
            }
            else                    /* Time bit = 0 .... LWA + 1 */
            {
                WriteSDRAM_4B(LeadoutStartAddr);  /* 7,8,9,10 : Long data to DRAM */
            }
                                                   /* Process TOC format 0 */

            /* setup the shadow registers for xfer.*/
            XferAddress = DVDRAM_TOC_00_ADR;     /* WORKING AREA*/
            break;

        case 0x10:  /* Toc Format 1 (Session Information) */
            toc_length = 0x0c;

            Iram_Buf_dram_addr = DVDRAM_TOC_01_ADR;
            WriteSDRAM(0);         /* 0:Lentht(h) = 0 */
            WriteSDRAM(10);        /* 1:Length(L) = 10 */
            WriteSDRAM(1);         /* 2:First Track Number = 1 */
            WriteSDRAM(1);         /* 3:Last  Track Number = 1 */
            WriteSDRAM(0);         /* 4:Reserve */
            WriteSDRAM(0x14);      /* 5:Addr = 0001b, Control = 0100b */
            WriteSDRAM(1);         /* 6:Track Numer = 1 */
            WriteSDRAM(0);         /* 7:Reserve */

            Iram_Buf_dram_addr = DVDRAM_TOC_01_ADR+8;

            /* Time bit = 1 ... Method 2 -> 1 .. return with  MSF */
            if (St_MSF == TRUE)
            {
                WriteSDRAM (0x00);        /* 7: Reserve */
                WriteSDRAM (0x00);        /* 8: M */
                WriteSDRAM (0x02);        /* 9: S */
                WriteSDRAM (0x00);        /*10: F */

            }
            else                    /* Time bit = 0 .... LWA + 1 */
            {
                WriteSDRAM_4B(0);  /* 7,8,9,10 : Long data to DRAM */
            }

            /* setup the shadow registers for xfer.*/

            XferAddress = DVDRAM_TOC_01_ADR;     /* WORKING AREA*/
            break;

        default:
            break;
    }   /* end switch(toc_format) */

    XferLength = (USHORT)(PacketCommandToLBALEN2(7));
    XferLength = MINIMUM( XferLength, toc_length);  // Number of bytes to xfer. Compare, return minimum.
    SendParameterDataToHost(XferAddress, XferLength, TRUE);   // Send data and Command Complete.
}


/*--------------------------------------------------------------------
  Name: DVDRAMReadDiscInfoCommand()
  Description:
    Processes ATAPI Read Disc Info command for DVD-RAM
  Inputs:    none
  Outputs:   none
  Returns:   none
----------------------------------------------------------------------*/
void DVDRAMReadDiscInfoCommand(void)
{
    USHORT  DiscInfoTotalLength;
    USHORT  DiscInfoLength;
    USHORT  DiscInfoOpcLength;
    BYTE    NumberOfOpcTableEntries;
    BYTE    Index;

    //if (CheckForWriteError() == FALSE) return;

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

    // ---------------------------------------------------------------------------------
    // Byte[0][1] : Data Length
    if(DiscInfoLength > DiscInfoTotalLength)
        DiscInfoLength = DiscInfoTotalLength;

    disc_info[0] = (BYTE)((DiscInfoTotalLength-2) >> 8);   // The first two bytes
    disc_info[1] = (BYTE) (DiscInfoTotalLength-2);         //   are the Disc Information Length(the number of bytes retured to the Initiator.

    // ---------------------------------------------------------------------------------
    // Byte[2] : Bit4-Erasable, Bits3,2-State of Last Session, Bits1,0-Disc Status
    disc_info[2] |= 0x10;    // Set erasable bit.
    disc_info[2] |= 0x0F;    // 0000 1111 = Complete Session, Others Disc (Random access)

    // ---------------------------------------------------------------------------------
    // Byte[3] : Number of First Track on Disc
    disc_info[3] = 1;

    // ---------------------------------------------------------------------------------
    // Byte[4] : Number of Sessions (LSB)
    // Byte[9] : Number of Sessions (MSB)
    disc_info[4] = 1;
    disc_info[9] = 0;

    // ---------------------------------------------------------------------------------
    // Byte[5]  : First Track Number in Last Session (LSB)
    // Byte[10] : First Track Number in Last Session (MSB)
    disc_info[5]  = 1;
    disc_info[10] = 0;

    // ---------------------------------------------------------------------------------
    // Byte[6]  : Last Track Number in Last Session (LSB)
    // Byte[11] : Last Track Number in Last Session (MSB)
    disc_info[6]  = 1;
    disc_info[11] = 0;

    // ---------------------------------------------------------------------------------
    // Byte[7] : Bit7=DID_V, Bit6-DBC_V, Bit5-URU,
    //           Bit4-DAC_V, Bit2-Dbit, Bit1,0-BGFormatStatus
    disc_info[7] = 0;

    // ---------------------------------------------------------------------------------
    // Byte[8] : Disc Type
    // As defined in MMC: For DVD = 0xFF
    disc_info[8] = 0xFF;

    // ---------------------------------------------------------------------------------
    // Byte[12]-[15] : Disc Identification
    // This here is a DVD. Bytes 12,13,14,15 are 0x00
    // Initialized to 0x00 see above

    // ---------------------------------------------------------------------------------
    // Byte[16]-[19]  : Last Session Lead In Start Address
    // Bytes[20]-[23] : Last Possible Lead Out Start Address
    // For DVD-RAM this field shall be filled with zeros. P.S. already done during initialization of array.


    // ---------------------------------------------------------------------------------
    // Byte[24]-[31] : Disc Bar Code
    // Platform cannot read Bar Code. Set bytes to 0x00

    // ---------------------------------------------------------------------------------
    // Byte[32] : Disc Application Code
    disc_info[32] = 0;

    // ---------------------------------------------------------------------------------
    // Byte[33] : Number of OPC Tables
    disc_info[33] = 0;

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

    SendParameterDataToHost(DISC_INFO_ADR, DiscInfoLength, TRUE);  // Send Data transfer to host with Command Complete.
}


/*--------------------------------------------------------------------
  Function:     DVDRAMReadTrackInfoCommand()

  Description:  Processes ATAPI Read Track Command

  Returns:

  Notes:
--------------------------------------------------------------------*/
void DVDRAMReadTrackInfoCommand(void)
{
    StLongU     TempL;
    USHORT      XferLen;
    BYTE        TrackNumber, SessionNo;
    BYTE        AddrDefinition;
    BYTE        Index;

    //------------------------------------------
    //          Environmental Check
    //------------------------------------------
    if( pdbServoGetDiscDiameter() == eNoDisc )
    {
        BUILD_SENSE(0x02,0x3A,0x00,0x51);                // Medium Not Present
        AutoCommandComplete();
        return;
    }

    SET_INT_BYTE_HIGH(track_info_len) = Iram_Cmd_block.bf[7];
    SET_INT_BYTE_LOW(track_info_len)  = Iram_Cmd_block.bf[8];

    if (track_info_len == 0)
    {
        AutoCommandComplete();
        return;
    }

    //------------------------------------------
    //     Initialize Parameters/Variables
    //------------------------------------------
    AddrDefinition = Iram_Cmd_block.bf[1] & 0x03;   // 0:LBA, 1:LeadIn/Track#, 2:1st Track in Session
    switch(AddrDefinition)
    {
        case 0x00:  //LBA
            TrackNumber = 1;
            break;
        case 0x01:  //Tno
            TrackNumber = Iram_Cmd_block.bf[5];             // track number requested
            if(TrackNumber > 1)
            {
                BUILD_SENSE(0x05,0x21,0x00,0xbc);                          /* LOGICAL BLOCK ADDRESS OUT OF RANGE */
                AutoCommandComplete();
                return;
            }

          break;
        case 0x02:  //Session no
            SessionNo = Iram_Cmd_block.bf[5];               // Session number requested
            TrackNumber = 1;
            if(SessionNo > 1)
            {
                BUILD_SENSE(0x05,0x21,0x00,0xbd);                          /* LOGICAL BLOCK ADDRESS OUT OF RANGE */
                AutoCommandComplete();
                return;
            }
            break;
        default:
            BUILD_SENSE(0x05,0x24,0x00,0x8e);                          /* Invalid CDB */
            AutoCommandComplete();
            return;
    }

    for(Index=0; Index < TRACK_INFO_LEN_BYTE; Index++)       // initialize return array to all zeros
        track_info_data[Index] = 0;

    // ---------------------------------------------------------------------------------
    // Byte[0][1] : Data Length
    // ---------------------------------------------------------------------------------
    track_info_data[0] = 0x00;
    track_info_data[1] = TRACK_INFO_LEN_BYTE - 2;

    // ---------------------------------------------------------------------------------
    // BYTE[2]:Track Number (LSB)
    // BYTE[32]:Track Number (MSB)  = 0x00 ok!
    // ---------------------------------------------------------------------------------
    track_info_data[2] = 0x01;

    // ---------------------------------------------------------------------------------
    // BYTE[3]:Session Number (LSB)
    // BYTE[33]:Session Number (MSB)    = 0x00 ok!
    // ---------------------------------------------------------------------------------
    track_info_data[3] = 0x01;

    // ---------------------------------------------------------------------------------
    // BYTE[5]: Bit5-Damage, Bit4-Copy, Bits3-0-TrackMode
    // *** DVD-RAM: Damage = 0b, Copy = 0b, Track Mode = 04h
    // ---------------------------------------------------------------------------------
    track_info_data[5] = 0x04;

    // ---------------------------------------------------------------------------------
    // BYTE[6]: Bit7-RT, Bit6-Blank, Bit5-Packet/Inc, Bit4-FP, Bits3-0-DataMode
    // *** DVD-RAM: Bit7-Reserved   = 0   (Default, MMC5 r3)
    // *** DVD-RAM: Bit6-Blank      = 0   (ALWAYS,  MMC5 r3)
    // *** DVD-RAM: Bit5-Packet/Inc = 1   (ALWAYS,  MMC5 r3)
    // *** DVD-RAM: Bit4-FP         = 0   (ALWAYS,  MMC5 r3)
    // *** DVD-RAM: Bit3-0-FP       = 01h (ALWAYS,  MMC5 r3)
    track_info_data[6] = 0x21;

    // ---------------------------------------------------------------------------------
    // BYTE[7]: Bit1-LRA Valid, Bit0-NWA Valid
    // *** DVD-RAM: = 0 (ALWAYS)
    // ---------------------------------------------------------------------------------

    // ---------------------------------------------------------------------------------
    // BYTE[8]-[11]: Track Start Address
    // ---------------------------------------------------------------------------------
    track_info_data[8]  = 0x00;    // Track Start Address
    track_info_data[9]  = 0x00;
    track_info_data[10] = 0x00;
    track_info_data[11] = 0x00;

    // ---------------------------------------------------------------------------------
    // BYTE[12]-[15]: Next Writable Address
    // ---------------------------------------------------------------------------------
    track_info_data[12] = 0x00;     // Next Writable Address
    track_info_data[13] = 0x00;
    track_info_data[14] = 0x00;
    track_info_data[15] = 0x00;

    // ---------------------------------------------------------------------------------
    // BYTE[16]-[19]: Free Blocks
    // ---------------------------------------------------------------------------------
    track_info_data[16] = 0x00;    // Free Blocks
    track_info_data[17] = 0x00;
    track_info_data[18] = 0x00;
    track_info_data[19] = 0x00;

    // ---------------------------------------------------------------------------------
    // BYTE[20]-[23]: Fixed Packet Size/Blocking Factor
    // *** DVD-RAM: 16 (ALWAYS)
    // ---------------------------------------------------------------------------------
    track_info_data[23] = 0x10;

    // ---------------------------------------------------------------------------------
    // BYTE[24]-[27]: Track Size
    // ---------------------------------------------------------------------------------
    TempL.Long = GetDVDRAMMaxLBA() + 1;

    track_info_data[24] = TempL.byte.u;   // Track Size
    track_info_data[25] = TempL.byte.h;
    track_info_data[26] = TempL.byte.m;
    track_info_data[27] = TempL.byte.l;

    // ---------------------------------------------------------------------------------
    // BYTE[28]-[31]: Last Recorded Address
    // ---------------------------------------------------------------------------------
    /* valid only for DVD-R/DL (LJRS = 0), -RW, HD DVD-R, BD-R (MMC5 r3) */
    /* other vendors return this value */
    TempL.Long--;
    track_info_data[28] = TempL.byte.u;   // Last Recorded Address
    track_info_data[29] = TempL.byte.h;
    track_info_data[30] = TempL.byte.m;
    track_info_data[31] = TempL.byte.l;

    // ---------------------------------------------------------------------------------
    // BYTE[36]-[39]:Read Compatibility LBA
    // ---------------------------------------------------------------------------------
    /* valid only for DVD+R invisible track (MMC5 r3) */


    //------------------------------------------
    // Transfer Return Data from Array Into DRAM
    //------------------------------------------
    Iram_Buf_dram_addr = TRACK_INFO_ADR;
    for(Index=0; Index < TRACK_INFO_LEN_BYTE; Index++)
        WriteSDRAM(track_info_data[Index]);

    if(TRACK_INFO_LEN_BYTE > track_info_len)
        XferLen = track_info_len;
    else
        XferLen = TRACK_INFO_LEN_BYTE;

    SendParameterDataToHost(TRACK_INFO_ADR, XferLen, TRUE);  // Send Data transfer to host with Command Complete.

}


void DVDRAMVerify10Command(void)
{
    fReadVerifyCommand = TRUE;
    Cmd_read_10_DVDRAM();
}

#endif /* #if (DVD_RAM_READ  == 1)*/

