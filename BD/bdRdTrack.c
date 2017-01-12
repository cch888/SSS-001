/*****************************************************************************
*                (c) Copyright 2000 - 2007 SUNEXT DESIGN, INC.
*                            All Rights Reserved
*
*
* FILENAME:     $Workfile: bdRdTrack.c $
*
* DESCRIPTION   This module contains functions related to the
*               Read Track Information command.
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
#include ".\servo\svo.h"
#include ".\bd\bdDef.h"
#include ".\bd\bdMacro.h"
#include ".\bd\bdVars.h"
#include ".\bd\bdRdTrack.h"
#include ".\hif\gp_mac.h"
#if (BD_ENABLE)
// ***************************************************************************
//                               EXTERNS
// ***************************************************************************
extern BYTE     track_info_data[];
extern USHORT   track_info_len;

// ***************************************************************************
//                              FUNCTIONS
// ***************************************************************************

/*****************************************************************************

    FUNCTION        BDReadTrackInfoCommand

    DESCRIPTION     Processes ATAPI Read Track Command

    PARAMETERS

    RETURNS

*****************************************************************************/
void BDReadTrackInfoCommand(void)
{
    StLongU     FreeBlocks;
    StLongU     TempL;
    LONG        RecordedLBA;
    ULONG       TempULONG;
    LONG        TempLONG;
    LONG        TrackStartLBA;
    LONG        TrackStopLBA;
    LONG        LeadOutStartLBA;
    USHORT      XferLen;
    BYTE        TrackNumber, SessionNo;
    BYTE        AddrDefinition;
    BYTE        Index;
    BYTE        TrackState;
    BYTE        TrackStatus;
    BYTE        TrackDiagStatus2;
    BYTE        LastTrackNumber;

    //------------------------------------------
    //          Environmental Check
    //------------------------------------------
    if( pdbServoGetDiscDiameter() == eNoDisc )
    {
        BUILD_SENSE(0x02,0x3A,0x00,0x04);                // Medium Not Present
        AutoCommandComplete();
        return;
    }

    BHIGH(track_info_len) = Iram_Cmd_block.bf[7];
    BLOW(track_info_len) = Iram_Cmd_block.bf[8];

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
            LastTrackNumber = pdbGetBDFinalSessLastTrackNumber();
            TempLONG = (LONG)(PacketCommandToLBALEN4(2));
            for(TrackNumber = 1; TrackNumber <= LastTrackNumber ; TrackNumber++)
            {
                TrackStartLBA = bdConvertPSNtoLBA(pdbGetBDTrackStartPSN(TrackNumber));  // start PSN of track "i"
                TrackStopLBA  = bdConvertPSNtoLBA(pdbGetBDTrackEndPSN(TrackNumber));    // stop PSN of track "i"
                if((TrackStartLBA <= TempLONG) && (TrackStopLBA >= TempLONG))
                    break;
            }

            if(TrackNumber > LastTrackNumber)
            {
                BUILD_SENSE(0x05,0x21,0x00,0x14);                          /* LOGICAL BLOCK ADDRESS OUT OF RANGE */
                AutoCommandComplete();
                return;
            }
            break;
        case 0x01:  //Tno
            TrackNumber = Iram_Cmd_block.bf[5];             // track number requested
            LastTrackNumber = pdbGetBDFinalSessLastTrackNumber();
            if((TrackNumber > LastTrackNumber)
                && (TrackNumber > LastTrackNumber))
            {
                BUILD_SENSE(0x05,0x24,0x00,0x3f);                          /* LOGICAL BLOCK ADDRESS OUT OF RANGE */
                AutoCommandComplete();
                return;
            }
            break;
        case 0x02:  //Session no
            SessionNo = Iram_Cmd_block.bf[5];               // Session number requested

            LastTrackNumber = pdbGetBDFinalSessLastTrackNumber();
            for(TrackNumber = 1; TrackNumber <= LastTrackNumber ; TrackNumber++)
            {
                if (pdbGetBDTrackSessionNumber(TrackNumber) == SessionNo) break;
            }

            if(TrackNumber > LastTrackNumber)
            {
                BUILD_SENSE(0x05,0x21,0x00,0x16);                          /* LOGICAL BLOCK ADDRESS OUT OF RANGE */
                AutoCommandComplete();
                return;
            }
            break;
        default:
            BUILD_SENSE(0x05,0x24,0x00,0x4f);                          /* Invalid CDB */
            AutoCommandComplete();
            return;
    }

    if (bdCheckValidTrackNumber(TrackNumber) == FALSE)  // track: valid?
        return;

    TempULONG = pdbGetBDTrackStartPSN(TrackNumber);    // track: start address
    TrackStartLBA = bdConvertPSNtoLBA(TempULONG);

    TempULONG = pdbGetBDTrackEndPSN(TrackNumber);      // track: stop address
    TrackStopLBA = bdConvertPSNtoLBA(TempULONG);

    //TempULONG = pdbGetBDLeadOutStartPSN();             // lead out start address
    if (pdbGetBDLayers() == BD_DUAL_LAYER)
        TempULONG = pdbGetBDMaxPSN(1) + 1L;
    else if (pdbGetBDLayers() == BD_SINGLE_LAYER)
        TempULONG = pdbGetBDMaxPSN(0) + 1L;

    LeadOutStartLBA = bdConvertPSNtoLBA(TempULONG);

    TrackDiagStatus2 = pdbGetBDTrackDiagStatus2(TrackNumber);

    for(Index=0; Index < TRACK_INFO_LEN_BYTE; Index++)       // initialize return array to all zeros
        track_info_data[Index] = 0;

    switch(pdbGetDiscKind())
    {
        case eBDROMDisc:
        case eBDROMDLDisc:
            // ---------------------------------------------------------------------------------
            // Byte[0][1] : Data Length
            // ---------------------------------------------------------------------------------
            track_info_data[0] = 0x00;              /*** Byte[ 0],[ 1] : Data length    ***/
            track_info_data[1] = TRACK_INFO_LEN_BYTE - 2;

            track_info_data[2] = TrackNumber;       /*** Byte[ 2] : Track number (LSB)       ***/
            track_info_data[3] = TrackNumber;       /*** Byte[ 3] : Session number (LSB)     ***/

            track_info_data[4] = 0x00;              /*** Byte[ 4] : Reserved        ***/
            track_info_data[5] = 0x04;              /*** Byte[ 5] : Track mode      ***/

            track_info_data[6] = 0x01;              /*** Byte[ 6] : Data mode       ***/
            track_info_data[7] = 0x00;

            // ---------------------------------------------------------------------------------
            // BYTE[8]-[11]: Track Start Address
            //               0:LBA, 1:LeadIn/Track#, 2:Session#
            // ---------------------------------------------------------------------------------
            TempL.Long = TrackStartLBA;

            track_info_data[8]  = TempL.byte.u;     // Track Start Address
            track_info_data[9]  = TempL.byte.h;
            track_info_data[10] = TempL.byte.m;
            track_info_data[11] = TempL.byte.l;

            // ---------------------------------------------------------------------------------
            // BYTE[12]-[15]: Next Writable Address
            // ---------------------------------------------------------------------------------
            track_info_data[12] = 0x00;
            track_info_data[13] = 0x00;
            track_info_data[14] = 0x00;
            track_info_data[15] = 0x00;

            // ---------------------------------------------------------------------------------
            // BYTE[16]-[19]: Free Blocks
            // ---------------------------------------------------------------------------------
            track_info_data[16] = 0x00;
            track_info_data[17] = 0x00;
            track_info_data[18] = 0x00;
            track_info_data[19] = 0x00;

            // ---------------------------------------------------------------------------------
            // BYTE[20]-[23]: Fixed Packet Size/Blocking Factor
            // ---------------------------------------------------------------------------------
            //track_info_data[20] = 0x00;
            //track_info_data[21] = 0x00;
            //track_info_data[22] = 0x00;
            track_info_data[23] = 0x20;

            // ---------------------------------------------------------------------------------
            // BYTE[24]-[27]: Track Size
            // ---------------------------------------------------------------------------------
            if (pdbGetBDLayers() == BD_DUAL_LAYER)
            {
                TempL.Long = ( pdbGetBDMaxPSN(0) -pdbGetBDTrackStartPSN(TrackNumber) + 1 ) +
                                      ( pdbGetBDTrackEndPSN(TrackNumber) - pdbGetBDStartPSN(1) + 1 );
            }
            else if ( pdbGetBDLayers() == BD_SINGLE_LAYER )
            {
                TempL.Long = pdbGetBDTrackEndPSN(TrackNumber) - pdbGetBDTrackStartPSN(TrackNumber) + 1;
            }
            track_info_data[24] = TempL.byte.u;
            track_info_data[25] = TempL.byte.h;
            track_info_data[26] = TempL.byte.m;
            track_info_data[27] = TempL.byte.l;

            // ---------------------------------------------------------------------------------
            // BYTE[28]-[31]: Last Recorded Address
            // ---------------------------------------------------------------------------------
            track_info_data[28] = 0x00;
            track_info_data[29] = 0x00;
            track_info_data[30] = 0x00;
            track_info_data[31] = 0x00;

            track_info_data[32] = 0x00;     /*** Byte[32] : Track Number (MSB)***/
            track_info_data[33] = 0x00;     /*** Byte[33] : Session Number (MSB)***/
            track_info_data[34] = 0x00;     /*** Byte[34][35] : Reserved ***/
            track_info_data[35] = 0x00;
            break;

        case eBDRDisc:
        case eBDRDLDisc:
            // ---------------------------------------------------------------------------------
            // Byte[0][1] : Data Length
            // ---------------------------------------------------------------------------------
            track_info_data[0] = 0x00;
            track_info_data[1] = TRACK_INFO_LEN_BYTE - 2;

            // ---------------------------------------------------------------------------------
            // BYTE[2]:Track Number (LSB)
            // BYTE[32]:Track Number (MSB)  = 0x00 ok!
            // ---------------------------------------------------------------------------------
            track_info_data[2] = TrackNumber;

            // ---------------------------------------------------------------------------------
            // BYTE[3]:Session Number (LSB)
            // BYTE[33]:Session Number (MSB)    = 0x00 ok!
            // ---------------------------------------------------------------------------------
            track_info_data[3] = pdbGetBDTrackSessionNumber(TrackNumber);

            // ---------------------------------------------------------------------------------
            // BYTE[5]: Bit7-6 - LJRS, Bit5-Damage, Bit4-Copy, Bits3-0-TrackMode
            // *** BD-R: LJRS = 0b, Damage = 0b, Copy = 0b, Track Mode = 04h
            // ---------------------------------------------------------------------------------
            track_info_data[5] = 0x04;

            // ---------------------------------------------------------------------------------
            // BYTE[6]: Bit7-RT, Bit6-Blank, Bit5-Packet/Inc, Bit4-FP (0), Bits3-0-DataMode
            // *** BD-R: Bit7-Reserved Track = 0 (incomplete SRR), 1 (complete SRR)
            // ---------------------------------------------------------------------------------
            if (pdbGetBDTrackState(TrackNumber) & eTRACK_STATE_INCOMPLETE)
            {
                track_info_data[6] = 0x80;    // 0x80
            }

            // *** BD-R: Bit6-Blank = 1 (all writable unit are blank)
            //                         0 (some writable units is written)
            if (pdbGetBDTrackStatus(TrackNumber) & eTS_BlankTrack)
                track_info_data[6] = track_info_data[6] | eTS_BlankTrack;           // 0x40

            // *** BD-R: Bit5   Packet/Inc = 1   (ALWAYS, MMC6 r0)
            // *** BD-R: Bit4   FP         = 0   (ALWAYS, MMC6 r0)
            // *** BD-R: Bit3-0 Datamode   = 01h (ALWAYS)
            track_info_data[6] = track_info_data[6] | 0x21;

            // ---------------------------------------------------------------------------------
            // BYTE[7]: Bit1-LRA V (1), Bit0-NWA Valid
            // *** BD-R: See BYTE[12]-[15]: NWA Below -> These 2 bits will be set there as well
            // *** BD-R: See BYTE[28]-[31]: Last Recorded Address Below -> These 2 bits will be set there as well
            // ---------------------------------------------------------------------------------

            // ---------------------------------------------------------------------------------
            // BYTE[8]-[11]: Track Start Address
            //               0:LBA, 1:LeadIn/Track#, 2:Session#
            // ---------------------------------------------------------------------------------
            TempL.Long = TrackStartLBA;

            track_info_data[8]  = TempL.byte.u;     // Track Start Address
            track_info_data[9]  = TempL.byte.h;
            track_info_data[10] = TempL.byte.m;
            track_info_data[11] = TempL.byte.l;

            // ---------------------------------------------------------------------------------
            // BYTE[12]-[15]: Next Writable Address
            // ---------------------------------------------------------------------------------

            if (St_blank_disc == TRUE)                  // if disc is blank, then for any track NWA = 0
            {
                // NWA = VALID
                track_info_data[7] = 0x01;              // set NWA_V bit
                RecordedLBA = 0L;
            }
            else
            {
                if (pdbGetBDTrackDiagStatus2(TrackNumber) & eTS_NWA_valid)       // 0x08
                {
                    // NWA = VALID
                    track_info_data[7] = 0x01;          // set NWA_V bit
                    RecordedLBA = pdbGetBDTrackNextWritableAddress(TrackNumber);
                }
                else
                {
                    // NWA = NOT VALID
                    track_info_data[7] = 0x00;          // clear NWA_V and LRA_V bits
                    RecordedLBA = TrackStopLBA + 1;
                }
            }

            TempL.Long = RecordedLBA;

            track_info_data[12] = TempL.byte.u;     // Next Writable Address
            track_info_data[13] = TempL.byte.h;
            track_info_data[14] = TempL.byte.m;
            track_info_data[15] = TempL.byte.l;

            // ---------------------------------------------------------------------------------
            // BYTE[16]-[19]: Free Blocks
            // ---------------------------------------------------------------------------------

            TrackState  = pdbGetBDTrackState(TrackNumber);
            TrackStatus = pdbGetBDTrackStatus(TrackNumber);

            if (TrackNumber)
            {
                // ------------------------------------
                // TRACK NUMBER IS NOT 0 = WRITTEN DISC
                // ------------------------------------
                switch (TrackState)
                {
                    case eTRACK_STATE_EMPTY:
                        FreeBlocks.Long = LeadOutStartLBA - TrackStartLBA;
                        break;

                    case eTRACK_STATE_BLANK:
                        if (TrackStatus & eTS_ReservedTrack)
                            FreeBlocks.Long = TrackStopLBA - TrackStartLBA + 1;
                        else if (LeadOutStartLBA <= TrackStartLBA)
                        {
                            FreeBlocks.Long = 0L;
                            track_info_data[7] = track_info_data[7] & 0xFE;     // clear NWA_V
                        }

                        break;

                    case eTRACK_STATE_INCOMPLETE:
                        if (TrackDiagStatus2 & eTS_NWA_valid)
                        {
                            if (TrackStatus == eTS_ReservedTrack)
                            {
                                FreeBlocks.Long = TrackStopLBA - pdbGetBDTrackNextWritableAddress(TrackNumber) + 1;
                            }
                            else if(TrackStatus == eTS_InvisibleTrack)
                            {
                                FreeBlocks.Long = LeadOutStartLBA - pdbGetBDTrackNextWritableAddress(TrackNumber);
                            }
                            else
                            {
                                FreeBlocks.Long = LeadOutStartLBA - pdbGetBDTrackNextWritableAddress(TrackNumber);
                            }
                        }
                        else // trackDiagStatus != eTS_NWA_valid
                        {
                            FreeBlocks.Long = TrackStopLBA - RecordedLBA - 1L;
                        }

                        break;

                    case eTRACK_STATE_COMPLETE:
                        track_info_data[7] = 0x00;      // clear LRA_V and NWA_V bits

                        track_info_data[12] = 0x00;     // Next writable address to 0
                        track_info_data[13] = 0x00;
                        track_info_data[14] = 0x00;
                        track_info_data[15] = 0x00;

                        FreeBlocks.Long = 0x00;

                        break;

                }

                track_info_data[16] = FreeBlocks.byte.u;    // Free Blocks
                track_info_data[17] = FreeBlocks.byte.h;
                track_info_data[18] = FreeBlocks.byte.m;
                track_info_data[19] = FreeBlocks.byte.l;

            }
            else
            {
                // ------------------------------------
                // TRACK NUMBER IS 0 = BLANK/EMPTY DISC
                // ------------------------------------
            }

            // ---------------------------------------------------------------------------------
            // BYTE[20]-[23]: Fixed Packet Size/Blocking Factor
            // *** BD-R/RE: 32 (ALWAYS)
            // ---------------------------------------------------------------------------------
            //track_info_data[20] = 0x00;
            //track_info_data[21] = 0x00;
            //track_info_data[22] = 0x00;
            track_info_data[23] = 0x20;

            // ---------------------------------------------------------------------------------
            // BYTE[24]-[27]: Track Size
            // ---------------------------------------------------------------------------------
            if (pdbGetBDTrackSessionNumber(TrackNumber) >= pdbGetBDFinalSessionNumber())
            {
                // SRR from Open Session
                // MMC6 Spec: Track StopAddress - Track StartAddress + 1
                if (Iram_Cmd_block.bf[5] == 0x01 && St_blank_disc == 0)
                {
                    if((TrackNumber == pdbGetBDFinalSessLastTrackNumber()) && (TrackState == eTRACK_STATE_INCOMPLETE))
                        TempL.Long = LeadOutStartLBA - TrackStartLBA;
                    else
                        TempL.Long = TrackStopLBA - TrackStartLBA + 1;
                }
                else
                {
                    if ( pdbGetBDTrackStatus(TrackNumber) == eTS_InvisibleTrack)
                        TempL.Long = LeadOutStartLBA - TrackStartLBA;
                    else
                        TempL.Long = TrackStopLBA - TrackStartLBA + 1;
                }
            }
            else
            {
                // SRR from Closed Session = Collection of SRRs in a Closed Session
                // Session# of Logical TrackNumber < Final Session#
                // MMC6 Spec: SessionEndAddress - SessionStartAddress + 1
                TempL.Long = bdConvertPSNtoLBA(pdbGetBDTrackEndPSN(TrackNumber)) - bdConvertPSNtoLBA(pdbGetBDTrackStartPSN(TrackNumber)) + 1;
            }

            track_info_data[24] = TempL.byte.u;     // Track Size
            track_info_data[25] = TempL.byte.h;
            track_info_data[26] = TempL.byte.m;
            track_info_data[27] = TempL.byte.l;

            // ---------------------------------------------------------------------------------
            // BYTE[28]-[31]: Last Recorded Address
            // ---------------------------------------------------------------------------------
            TempL.Long = pdbGetBDTrackRecordedPSN(TrackNumber);

            if (TempL.Long != 0)     // 0x04
            {
                if((pdbGetBDFinalSessCloseInfo() != eSessionFinalized) || (pdbGetBDRecMode() != SRM_AND_LOW))
                {
                    // LRA = VALID
                    track_info_data[7] |= 0x02;          // set LRA_V bit
                    TempL.Long = bdConvertPSNtoLBA(TempL.Long);
                }
            }

            track_info_data[28] = TempL.byte.u;     // Last Recorded Address
            track_info_data[29] = TempL.byte.h;
            track_info_data[30] = TempL.byte.m;
            track_info_data[31] = TempL.byte.l;

            // ---------------------------------------------------------------------------------
            // BYTE[36]-[39]:Read Compatibility LBA (MMC6 r0: only valid for DVD+R invisible track)
            // ---------------------------------------------------------------------------------
            //TempL.Long = 0x00L;
            //track_info_data[36] = TempL.byte.u;
            //track_info_data[37] = TempL.byte.h;
            //track_info_data[38] = TempL.byte.m;
            //track_info_data[39] = TempL.byte.l;

            #if (PCB == 815)//Please turn it on manually for debugging
            #if (MSG_OUT_MODE == MSG_USBPORT)
            {
                BYTE idx;
                send_msg54(SEND_READ_DVD_MSG, 'B', 'D', '-', 'R');
                for (idx = 0; idx < 40; idx += 4)
                    send_msg54(SEND_READ_DVD_MSG, track_info_data[idx+0], track_info_data[idx+1], track_info_data[idx+2], track_info_data[idx+3]);
            }
            #endif
            #endif
            break;

        case eBDREDisc:
        case eBDREDLDisc:
            // ---------------------------------------------------------------------------------
            // Byte[0][1] : Data Length
            // ---------------------------------------------------------------------------------
            track_info_data[0] = 0x00;
            track_info_data[1] = TRACK_INFO_LEN_BYTE - 2;

            // ---------------------------------------------------------------------------------
            // BYTE[2]:Track Number (LSB)
            // BYTE[32]:Track Number (MSB)  = 0x00 ok!
            // ---------------------------------------------------------------------------------
            track_info_data[2] = TrackNumber;

            // ---------------------------------------------------------------------------------
            // BYTE[3]:Session Number (LSB)
            // BYTE[33]:Session Number (MSB)    = 0x00 ok!
            // ---------------------------------------------------------------------------------
            track_info_data[3] = pdbGetBDTrackSessionNumber(TrackNumber);

            // ---------------------------------------------------------------------------------
            // BYTE[5]: Bit7-6 - LJRS, Bit5-Damage, Bit4-Copy, Bits3-0-TrackMode
            // *** BD-RE: LJRS = 0b, Damage = 0b, Copy = 0b, Track Mode = 04h (MMC6 r0)
            // ---------------------------------------------------------------------------------
            track_info_data[5] = 0x04;

            // ---------------------------------------------------------------------------------
            // BYTE[6]: Bit7-RT, Bit6-Blank, Bit5-Packet/Inc, Bit4-FP, Bits3-0-DataMode
            // *** BD-RE: Bit7-Reserved Track = 0   (Default, MMC6 r0)
            // *** BD-RE: Bit6-Blank = 0 (has no meaning)
            // *** BD-RE: Bit5-Packet/Inc = 1 (ALWAYS, MMC6 r0)
            // *** BD-RE: Bit4-Fixed Packet = 0 (ALWAYS, MMC6 r0)
            // *** BD-RE: Bit3-0 Data Mode = 01h (ALWAYS, MMC6 r0)
            // ---------------------------------------------------------------------------------
            track_info_data[6] = 0x21;

            // ---------------------------------------------------------------------------------
            // BYTE[7]: Bit1-LRA V, Bit0-NWA Valid
            // *** BD-RE: See BYTE[12]-[15]: NWA Below -> These 2 bits will be set there as well
            // ---------------------------------------------------------------------------------

            // ---------------------------------------------------------------------------------
            // BYTE[8]-[11]: Track Start Address
            //               0:LBA, 1:LeadIn/Track#, 2:Session#
            // ---------------------------------------------------------------------------------
            TempL.Long = TrackStartLBA;

            track_info_data[8]  = TempL.byte.u;    // Track Start Address
            track_info_data[9]  = TempL.byte.h;
            track_info_data[10] = TempL.byte.m;
            track_info_data[11] = TempL.byte.l;

            // ---------------------------------------------------------------------------------
            // BYTE[12]-[15]: Next Writable Address
            // ---------------------------------------------------------------------------------
            TempL.Long = 0x00;

            track_info_data[12] = TempL.byte.u;     // Next Writable Address
            track_info_data[13] = TempL.byte.h;
            track_info_data[14] = TempL.byte.m;
            track_info_data[15] = TempL.byte.l;

            // ---------------------------------------------------------------------------------
            // BYTE[20]-[23]: Fixed Packet Size/Blocking Factor
            // *** BD-RE: 32 (ALWAYS)
            // ---------------------------------------------------------------------------------
            //track_info_data[20] = 0x00;
            //track_info_data[21] = 0x00;
            //track_info_data[22] = 0x00;
            track_info_data[23] = 0x20;

            // ---------------------------------------------------------------------------------
            // BYTE[24]-[27]: Track Size
            // ---------------------------------------------------------------------------------
            TempL.Long = pdbGetBDEndLSN() + 1;

            track_info_data[24] = TempL.byte.u;   // Track Size
            track_info_data[25] = TempL.byte.h;
            track_info_data[26] = TempL.byte.m;
            track_info_data[27] = TempL.byte.l;

            // ---------------------------------------------------------------------------------
            // BYTE[16]-[19]: Free Blocks
            // ---------------------------------------------------------------------------------
            FreeBlocks.Long = 0;
            track_info_data[16] = FreeBlocks.byte.u;    // Free Blocks
            track_info_data[17] = FreeBlocks.byte.h;
            track_info_data[18] = FreeBlocks.byte.m;
            track_info_data[19] = FreeBlocks.byte.l;

            // ---------------------------------------------------------------------------------
            // BYTE[28]-[31]: Last Recorded Address (MMC6 r0: only valid for -R/-RW, HD DVD-R, BD-R)
            // ---------------------------------------------------------------------------------
            /* TempL.Long = pdbGetBDLastRecordedAddress();
            track_info_data[28] = TempL.byte.u;   // Last Recorded Address
            track_info_data[29] = TempL.byte.h;
            track_info_data[30] = TempL.byte.m;
            track_info_data[31] = TempL.byte.l; */

            // ---------------------------------------------------------------------------------
            // BYTE[36]-[39]:Read Compatibility LBA (MMC6 r0: only valid for +R invisible track)
            // ---------------------------------------------------------------------------------

            #if (PCB == 815)//Please turn it on manually for debugging
            #if (MSG_OUT_MODE == MSG_USBPORT)
            {
                BYTE idx;
                send_msg54(SEND_READ_DVD_MSG, 'B', 'D', 'R', 'E');
                for (idx = 0; idx < 40; idx += 4)
                    send_msg54(SEND_READ_DVD_MSG, track_info_data[idx+0], track_info_data[idx+1], track_info_data[idx+2], track_info_data[idx+3]);
            }
            #endif
            #endif
            break;

        default:
            break;
    }
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

    return;
}


/*****************************************************************************

    FUNCTION        bdCheckValidTrackNumber

    DESCRIPTION

    PARAMETERS

    RETURNS

*****************************************************************************/
BOOL bdCheckValidTrackNumber(BYTE trackNum)
{
    BYTE LowestRecordedTno, HighestRecordedTno;

    LowestRecordedTno  = pdbGetBDLowestRecordedTrackNumber();
    HighestRecordedTno = pdbGetBDHighestRecordedTrackNumber();

    // HighestRecordedTno+1 is the last empty track
    if ((trackNum == 0) || (trackNum < LowestRecordedTno) || (trackNum > HighestRecordedTno+1))
    {
        BUILD_SENSE(0x05,0x21,0x00,0x17);               /* LOGICAL BLOCK ADDRESS OUT OF RANGE */
        AutoCommandComplete();
        return FALSE;
    }

    return TRUE;
}

#endif  //(BD_ENABLE)
