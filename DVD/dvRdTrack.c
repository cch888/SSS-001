/****************************************************************************
*                (c) Copyright 2005 - 2008 SUNEXT DESIGN, INC.
*                           All Rights Reserved
*
* FILENAME:     dvrRdTrack.c
*
* DESCRIPTION : This module contains functions related to the
*               Read Track Information command.
*
*
* $Revision: 90 $
* $Date: 11/03/11 3:25p $
*
************************ SUNEXT CONFIDENTIAL *******************************/

// *************************************************************************
//                               HEADERS
// *************************************************************************
#include ".\common\globals.h"
#include ".\common\globtype.h"
#include ".\servo\svo.h"
#include ".\player\plrDb.h"
#include ".\dvd\dvrdtrack.h"
#include ".\dvd\dvdiscinit.h"
#include ".\hif\mmc.h"
#include ".\hif\bf_util.h"
#include ".\hif\at_util.h"
#include ".\hif\at_knl.h"
#include ".\hif\gp_mac.h"
#include ".\dvd\dvutil.h"
#include ".\al\REG_dvd_decoder.h"


// *************************************************************************
//                               EXTERNS
// *************************************************************************
extern BYTE     track_info_data[TRACK_INFO_LEN_BYTE];
extern USHORT   track_info_len;
extern volatile CMD_BLOCK_T Iram_Cmd_block;

// *************************************************************************
//                              FUNCTIONS
// *************************************************************************

/*--------------------------------------------------------------------
  Function:     DVDReadTrackInfoCommand()

  Description:  Processes ATAPI Read Track Command

  Returns:

  Notes:
--------------------------------------------------------------------*/
void DVDReadTrackInfoCommand(void)
{
    StLongU     FreeBlocks;
    StLongU     TempL;
    LONG        RecordedLBA;
    ULONG       TempULONG;
    LONG        TempLONG;
    LONG        TrackStartLBA;
    LONG        TrackStopLBA;
    LONG        LeadOutStartLBA;
    LONG        LayerJumpLBA;
    LONG        TrackStartLBAL1;
    USHORT      XferLen;
    BYTE        TrackNumber, SessionNo;
    BYTE        AddrDefinition;
    BYTE        Index;
    BYTE        TrackState;
    BYTE        TrackStatus;
    BYTE        TrackDiagStatus2;
    BYTE        LastTrackNumber;


    #if PRN_DVD_DATABASE == 1
        DisplayDvdDataBase();
    #endif

    //------------------------------------------
    //          Environmental Check
    //------------------------------------------
    if( pdbServoGetDiscDiameter() == eNoDisc )
    {
        BUILD_SENSE(0x02,0x3A,0x00,0x01);                // Medium Not Present
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
            LastTrackNumber = pdbGetDVDFinalSessLastTrackNumber();
            TempLONG = (LONG)(PacketCommandToLBALEN4(2));
            for(TrackNumber = 1; TrackNumber <= LastTrackNumber ; TrackNumber++)
            {
                TrackStartLBA = dvdConvertPSNtoLBA(pdbGetDVDTrackStartPSN(TrackNumber));    // start PSN of track "i"
                TrackStopLBA  = dvdConvertPSNtoLBA(pdbGetDVDTrackEndPSN(TrackNumber));      // stop PSN of track "i"
                if((TrackStartLBA <= TempLONG) && (TrackStopLBA >= TempLONG))
                    break;
            }

            if(TrackNumber > LastTrackNumber)
            {
                BUILD_SENSE(0x05,0x21,0x00,0x11);                          /* LOGICAL BLOCK ADDRESS OUT OF RANGE */
                AutoCommandComplete();
                return;
            }
            break;
        case 0x01:  //Tno
            TrackNumber = Iram_Cmd_block.bf[5];             // track number requested
            LastTrackNumber = pdbGetDVDFinalSessLastTrackNumber();
            if((TrackNumber > LastTrackNumber && TrackNumber != INVISIBLE_TRACK )
                ||(pdbServoGetDiscType() == eLPPDisc) && (TrackNumber > LastTrackNumber))
            {
                BUILD_SENSE(0x05,0x24,0x00,0x07);                          /* LOGICAL BLOCK ADDRESS OUT OF RANGE */
                AutoCommandComplete();
                return;
            }
            break;
        case 0x02:  //Session no
            SessionNo = Iram_Cmd_block.bf[5];               // Session number requested

            LastTrackNumber = pdbGetDVDFinalSessLastTrackNumber();
            for(TrackNumber = 1; TrackNumber <= LastTrackNumber ; TrackNumber++)
            {
                if (pdbGetDVDTrackSessionNumber(TrackNumber) == SessionNo) break;
            }

            if(TrackNumber > LastTrackNumber)
            {
                BUILD_SENSE(0x05,0x21,0x00,0x12);                          /* LOGICAL BLOCK ADDRESS OUT OF RANGE */
                AutoCommandComplete();
                return;
            }
            break;
        default:
            BUILD_SENSE(0x05,0x24,0x00,0x08);                          /* Invalid CDB */
            AutoCommandComplete();
            return;
    }

    if (dvdCheckValidTrackNumber(TrackNumber) == FALSE) // track: valid?
        return;

    if(TrackNumber == INVISIBLE_TRACK)
    {
        TrackNumber = pdbGetDVDFinalSessLastTrackNumber();
    }


    TempULONG = pdbGetDVDTrackStartPSN(TrackNumber);    // track: start address
    TrackStartLBA = dvdConvertPSNtoLBA(TempULONG);

    TempULONG = pdbGetDVDTrackEndPSN(TrackNumber);      // track: stop address
    TrackStopLBA = dvdConvertPSNtoLBA(TempULONG);

    TempULONG = pdbGetDVDLeadOutStartPSN();             // lead out start address
    LeadOutStartLBA = dvdConvertPSNtoLBA(TempULONG);

    TrackDiagStatus2 = pdbGetDVDTrackDiagStatus2(TrackNumber);

    for(Index=0; Index < TRACK_INFO_LEN_BYTE; Index++)       // initialize return array to all zeros
        track_info_data[Index] = 0;

    switch(pdbGetDiscKind())
    {
        case eDVDROMDisc:
        case eDVDROMDLDisc:
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

            track_info_data[8]  = TempL.byte.u;    // Track Start Address
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
            track_info_data[20] = 0x00;
            track_info_data[21] = 0x00;
            track_info_data[22] = 0x00;
            track_info_data[23] = 0x10;

            // ---------------------------------------------------------------------------------
            // BYTE[24]-[27]: Track Size
            // ---------------------------------------------------------------------------------
            if (( pdbGetDVDLayers() == DUAL_LAYER ) && ( pdbGetDVDTrackPath() == DVDHD_OTP_DISC ))
            {
                TempL.Long = ( pdbGetDVDMaxPSN(0) -pdbGetDVDTrackStartPSN(TrackNumber) + 1 ) +
                                      ( pdbGetDVDTrackEndPSN(TrackNumber) - pdbGetDVDStartPSN(1) + 1 );
            }
            else if(( pdbGetDVDLayers() == DUAL_LAYER) && (pdbGetDVDTrackPath() == DVDHD_PTP_DISC))
            {
                TempL.Long = ( pdbGetDVDMaxPSN(0) -pdbGetDVDTrackStartPSN(TrackNumber) + 1 ) +
                                      ( pdbGetDVDMaxPSN(1) - pdbGetDVDStartPSN(1) + 1 );
            }
            else if ( pdbGetDVDLayers() == SINGLE_LAYER )
            {
                TempL.Long = pdbGetDVDTrackEndPSN(TrackNumber) - pdbGetDVDTrackStartPSN(TrackNumber) + 1;
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

        case eDVDPlusRDisc:
        case eDVDPlusRDLDisc:
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
            track_info_data[3] = pdbGetDVDTrackSessionNumber(TrackNumber);

            // ---------------------------------------------------------------------------------
            // BYTE[5]: Bit5-Damage, Bit4-Copy, Bits3-0-TrackMode
            // *** DVD+R: Damage = 0b, Copy = 0b, Track Mode = 07h
            // ---------------------------------------------------------------------------------
            track_info_data[5] = 0x07;

            // ---------------------------------------------------------------------------------
            // BYTE[6]: Bit7-RT, Bit6-Blank, Bit5-Packet/Inc, Bit4-FP, Bits3-0-DataMode
            // *** DVD+R: Bit7-Reserved
            // ---------------------------------------------------------------------------------
            if (pdbGetDVDTrackStatus(TrackNumber) & eTS_ReservedTrack)
            track_info_data[6] = track_info_data[6] | eTS_ReservedTrack;      // 0x80

            // *** DVD+R: Bit6-Blank = 1 if fragment is unwritten (invisible track)
            //                         0 if fragment is written
            // ---------------------------------------------------------------------------------
            if (pdbGetDVDTrackStatus(TrackNumber) & eTS_BlankTrack)
            track_info_data[6] = track_info_data[6] | eTS_BlankTrack;         // 0x40

            // *** DVD+R: Bit5   Packet/Inc = 1   (ALWAYS, MMC5 r3)
            // *** DVD+R: Bit4   FP         = 0   (ALWAYS, MMC5 r3)
            // *** DVD+R: Bit3-0 Datamode   = 01h (ALWAYS)
            #if (CEProduct == 1) && (MTK_BE == 1)
            // MTK backend set bit5-4 = 0
            track_info_data[6] = track_info_data[6] | 0x01;
            #else
            track_info_data[6] = track_info_data[6] | 0x21;
            #endif

            // ---------------------------------------------------------------------------------
            // BYTE[7]: Bit1-LRA V, Bit0-NWA Valid
            // *** DVD+R: See BYTE[12]-[15]: NWA Below -> These 2 bits will be set there as well
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

            if (St_blank_disc == TRUE)                      // if disc is blank, then for any track NWA = 0
            {
                // NWA = VALID
                track_info_data[7] = 0x01;              // set NWA_V bit
                RecordedLBA = 0L;
            }
            else
            {
                if (pdbGetDVDTrackDiagStatus2(TrackNumber) & eTS_NWA_valid)       // 0x08
                {
                    // NWA = VALID
                    track_info_data[7] = 0x01;              // set NWA_V bit
                    RecordedLBA = pdbGetDVDTrackNextWritableAddress(TrackNumber);
                }
                else
                {
                    // NWA = NOT VALID
                    track_info_data[7] = 0x00;              // clear NWA_V and LRA_V bits
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

            TrackState  = pdbGetDVDTrackState(TrackNumber);
            TrackStatus = pdbGetDVDTrackStatus(TrackNumber);

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
                                FreeBlocks.Long = TrackStopLBA - pdbGetDVDTrackNextWritableAddress(TrackNumber) + 1;
                            }
                            else if(TrackStatus == eTS_InvisibleTrack)
                            {
                                FreeBlocks.Long = LeadOutStartLBA - pdbGetDVDTrackNextWritableAddress(TrackNumber);
                            }
                            else
                            {
                                FreeBlocks.Long = LeadOutStartLBA - pdbGetDVDTrackNextWritableAddress(TrackNumber);
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
            // *** DVD+R: 16 (ALWAYS)
            // ---------------------------------------------------------------------------------
            track_info_data[23] = DVDPLUSR_PACKETSIZE;

            // ---------------------------------------------------------------------------------
            // BYTE[24]-[27]: Track Size
            // ---------------------------------------------------------------------------------
            if (pdbGetDVDTrackSessionNumber(TrackNumber) >= pdbGetDVDFinalSessionNumber())
            {
                // Fragment from Open Session
                // MMC5 Spec: FragmentStopAddress - FragmentStartAddress + 1
                if (Iram_Cmd_block.bf[5] == 0x01 && St_blank_disc == 0)
                {
                    if((TrackNumber == pdbGetDVDFinalSessLastTrackNumber()) && (TrackState == eTRACK_STATE_INCOMPLETE))
                        TempL.Long = LeadOutStartLBA - TrackStartLBA;
                    else
                        TempL.Long = TrackStopLBA - TrackStartLBA + 1;
                }
                else
                {
                    if ( pdbGetDVDTrackStatus(TrackNumber) == eTS_InvisibleTrack)
                        TempL.Long = LeadOutStartLBA - TrackStartLBA;
                    else
                        TempL.Long = TrackStopLBA - TrackStartLBA + 1;
                }
            }
            else
            {
                // Fragment from Closed Session = Collection of Fragments in a Closed Session
                // Session# of Logical TrackNumber < Final Session#
                // MMC5 Spec: SessionEndAddress - SessionStartAddress + 1
                TempL.Long = dvdConvertPSNtoLBA(pdbGetDVDTrackEndPSN(TrackNumber)) - dvdConvertPSNtoLBA(pdbGetDVDTrackStartPSN(TrackNumber)) + 1;
            }

            track_info_data[24] = TempL.byte.u;   // Track Size
            track_info_data[25] = TempL.byte.h;
            track_info_data[26] = TempL.byte.m;
            track_info_data[27] = TempL.byte.l;

            // ---------------------------------------------------------------------------------
            // BYTE[28]-[31]: Last Recorded Address
            // ---------------------------------------------------------------------------------
            #if (CEProduct == 1) && (MTK_BE == 1)
            TempL.Long = 0x26053F;  //for MTK backend
            #else
            TempL.Long = 0x00L;
            #endif

            track_info_data[28] = TempL.byte.u;   // Last Recorded Address
            track_info_data[29] = TempL.byte.h;
            track_info_data[30] = TempL.byte.m;
            track_info_data[31] = TempL.byte.l;

            // ---------------------------------------------------------------------------------
            // BYTE[36]-[39]:Read Compatibility LBA
            TempL.Long = 0x40000L;
            track_info_data[36] = TempL.byte.u;
            track_info_data[37] = TempL.byte.h;
            track_info_data[38] = TempL.byte.m;
            track_info_data[39] = TempL.byte.l;

            #if (MSG_OUT_MODE == MSG_USBPORT)
            {
                BYTE idx;
                send_msg54(SEND_READ_DVD_MSG, 'D', '+', 'R', ' ');
                for (idx = 0; idx < 40; idx += 4)
                    send_msg54(SEND_READ_DVD_MSG, track_info_data[idx+0], track_info_data[idx+1], track_info_data[idx+2], track_info_data[idx+3]);
            }
            #endif
            break;

        case eDVDPlusRWDisc:
        case eDVDPlusRWDLDisc:
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
            track_info_data[3] = pdbGetDVDTrackSessionNumber(TrackNumber);

            // ---------------------------------------------------------------------------------
            // BYTE[5]: Bit5-Damage, Bit4-Copy, Bits3-0-TrackMode
            // *** DVD+RW: Damage = 0b, Copy = 0b, Track Mode = 04h (MMC5 r3)
            // ---------------------------------------------------------------------------------
            track_info_data[5] = 0x04;

            // ---------------------------------------------------------------------------------
            // BYTE[6]: Bit7-RT, Bit6-Blank, Bit5-Packet/Inc, Bit4-FP, Bits3-0-DataMode
            // *** DVD+RW: Bit7-Reserved
            // *** DVD+RW: Bit6-Blank = 1 if entire disc is blank
            //                         0 if entire disc is not blank
            // ---------------------------------------------------------------------------------
            if (St_blank_disc == TRUE)
                track_info_data[6] = track_info_data[6] | eTS_BlankTrack;         // 0x40

            // *** DVD+RW: Bit7 RT (Reserved Track) = 0   (Default,MMC5 r3)
            // *** DVD+RW: Bit6 Blank
            // *** DVD+RW: Bit5 Packet/Inc          = 1   (ALWAYS, MMC5 r3)
            // *** DVD+RW: Bit4 FP (Fixed Packet)   = 0   (ALWAYS, MMC5 r3)
            // *** DVD+RW: Bit3-0 Data Mode         = 01h (ALWAYS, MMC5 r3)
            #if (CEProduct == 1) && (MTK_BE == 1)
            // MTK backend set bit5-4 = 0
            track_info_data[6] = track_info_data[6] | 0x01;
            #else
            track_info_data[6] = track_info_data[6] | 0x21;
            #endif

            // ---------------------------------------------------------------------------------
            // BYTE[7]: Bit1-LRA V, Bit0-NWA Valid
            // *** DVD+RW: See BYTE[12]-[15]: NWA Below -> These 2 bits will be set there as well
            // ---------------------------------------------------------------------------------
            //track_info_data[7] |= 0x01; //for CDT write test only, please do not remove it

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
            // BYTE[16]-[19]: Free Blocks
            // ---------------------------------------------------------------------------------
            if (St_blank_disc != TRUE)
                FreeBlocks.Long = 0x00;
            else if (pdbGetDiscKind() == eDVDPlusRWDLDisc)
            {   /* DVD+RW9 */
                if(St_12_cm == TRUE)
                    FreeBlocks.Long = 2*(DVD_PLUSRW_DATAZONE_END_L0 - pdbGetDVDTrackStartPSN(TrackNumber) + 1);
                else
                    FreeBlocks.Long = 2*(DVD_PLUSRW8cm_DATAZONE_END_L0 - pdbGetDVDTrackStartPSN(TrackNumber) + 1);
            }
            else
            {   /* DVD+RW5 */
                if(St_12_cm == TRUE)
                    FreeBlocks.Long = DVD_PLUSRW_DATAZONE_END - pdbGetDVDTrackStartPSN(TrackNumber) + 1;
                else
                    FreeBlocks.Long = DVD_PLUS8cm_DATAZONE_END - pdbGetDVDTrackStartPSN(TrackNumber) + 1;
            }

            track_info_data[16] = FreeBlocks.byte.u;    // Free Blocks
            track_info_data[17] = FreeBlocks.byte.h;
            track_info_data[18] = FreeBlocks.byte.m;
            track_info_data[19] = FreeBlocks.byte.l;

            // ---------------------------------------------------------------------------------
            // BYTE[20]-[23]: Fixed Packet Size/Blocking Factor
            // *** DVD+RW: 16 (ALWAYS)
            // ---------------------------------------------------------------------------------
            track_info_data[23] = DVDPLUSR_PACKETSIZE;

            // ---------------------------------------------------------------------------------
            // BYTE[24]-[27]: Track Size
            // ---------------------------------------------------------------------------------
            if (pdbGetDiscKind() == eDVDPlusRWDLDisc)
            {   /* DVD+RW9 */
                if(St_12_cm == TRUE)
                    TempL.Long = 2*(DVD_PLUSRW_DATAZONE_END_L0 - pdbGetDVDTrackStartPSN(TrackNumber) + 1);
                else
                    TempL.Long = 2*(DVD_PLUSRW8cm_DATAZONE_END_L0 - pdbGetDVDTrackStartPSN(TrackNumber) + 1);
            }
            else
            {   /* DVD+RW5 */
                if(St_12_cm == TRUE)
                    TempL.Long = DVD_PLUSRW_DATAZONE_END - pdbGetDVDTrackStartPSN(TrackNumber) + 1;
                else
                    TempL.Long = DVD_PLUS8cm_DATAZONE_END - pdbGetDVDTrackStartPSN(TrackNumber) + 1;
            }

            track_info_data[24] = TempL.byte.u;   // Track Size
            track_info_data[25] = TempL.byte.h;
            track_info_data[26] = TempL.byte.m;
            track_info_data[27] = TempL.byte.l;

            // ---------------------------------------------------------------------------------
            // BYTE[28]-[31]: Last Recorded Address (MMC5 r3: only valid for -R/-RW, HD DVD-R, BD-R)
            // ---------------------------------------------------------------------------------
#if (CEProduct == 1) && (MTK_BE == 1)
            if (St_12_cm == TRUE)
                TempL.Long = DVD_PLUSRW_DATAZONE_END - pdbGetDVDTrackStartPSN(TrackNumber) + 1;
            else
                TempL.Long = 0x230140;	// TBD? use #define
            track_info_data[28] = TempL.byte.u;   // Last Recorded Address
            track_info_data[29] = TempL.byte.h;
            track_info_data[30] = TempL.byte.m;
            track_info_data[31] = TempL.byte.l;
#else
            /* TempL.Long = pdbGetDVDLastRecordedAddress();
            track_info_data[28] = TempL.byte.u;   // Last Recorded Address
            track_info_data[29] = TempL.byte.h;
            track_info_data[30] = TempL.byte.m;
            track_info_data[31] = TempL.byte.l; */
#endif

            // ---------------------------------------------------------------------------------
            // BYTE[36]-[39]:Read Compatibility LBA (MMC5 r3: only valid for +R invisible track)
            // ---------------------------------------------------------------------------------

            break;

        case eDVDMinusRDisc:
        case eDVDMinusRDLDisc:
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
            track_info_data[3] = pdbGetDVDTrackSessionNumber(TrackNumber); // open session: return session number

            // ---------------------------------------------------------------------------------
            // BYTE[5]: Bit7-6-LJRS Bit5-Damage, Bit4-Copy, Bits3-0-TrackMode
            // *** DVD+R: Damage = 0b, Copy = 0b, Track Mode = 07h
            // ---------------------------------------------------------------------------------
            if(pdbGetDVDRMARMDFormat() == Format4_RMD)
                track_info_data[5] = 0x44;  /*LJRS set 01b*/
            else
                track_info_data[5] = 0x04;

            // ---------------------------------------------------------------------------------
            // BYTE[6]: Bit7-RT, Bit6-Blank, Bit5-Packet/Inc, Bit4-FP, Bits3-0-DataMode
            // *** DVD+R: Bit7-Reserved
            // ---------------------------------------------------------------------------------
            if (pdbGetDVDTrackStatus(TrackNumber) & eTS_ReservedTrack)
                track_info_data[6] = track_info_data[6] | eTS_ReservedTrack;      // 0x80

            if (pdbGetDVDTrackStatus(TrackNumber) & eTS_BlankTrack)
                track_info_data[6] = track_info_data[6] | eTS_BlankTrack;         // 0x40

            if (pdbGetDVDTrackStatus(TrackNumber) & eTS_Packet)
                track_info_data[6] = track_info_data[6] | eTS_Packet;             // 0x20

            track_info_data[6] = track_info_data[6] | 0x01;

            // ---------------------------------------------------------------------------------
            // BYTE[7]: Bit1-LRA V, Bit0-NWA Valid
            // *** DVD+R: See BYTE[12]-[15]: NWA Below -> These 2 bits will be set there as well
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

            TrackState  = pdbGetDVDTrackState(TrackNumber);
            TrackStatus = pdbGetDVDTrackStatus(TrackNumber);

    //        if (St_blank_disc == TRUE)                      // if disc is blank, then for any track NWA = 0
    //        {
    //        	  track_info_data[7] = 0x01;
    //            RecordedLBA = 0L;
    //        }
    //        else if (TrackNumber == INVISIBLE_TRACK)
    //        {
    //            RecordedLBA = 0x00;      // ?
    //        }
    //        else if (TrackNumber != 0)

            if (TrackNumber != 0)
            {
                // ------------------------------------
                // TRACK NUMBER IS NOT 0 = WRITTEN DISC
                // ------------------------------------
                if (pdbGetDVDTrackDiagStatus2(TrackNumber) & eTS_NWA_valid)       // 0x08
                {
                    {
                        track_info_data[7] = 0x01;
                        RecordedLBA = pdbGetDVDTrackNextWritableAddress(TrackNumber);
                    }
                }
                else
                {
                    track_info_data[7] = 0x02;              // clear NWA_V and LRA_V bits
                    RecordedLBA = 0x00;
                }
            }
            else
            {
                // ------------------------------------
                // TRACK NUMBER IS 0 = BLANK/EMPTY DISC
                // ------------------------------------
                track_info_data[7] = 0x01;              // clear NWA_V and LRA_V bits
                RecordedLBA = 0x00;

            }

            TempL.Long = RecordedLBA;

            track_info_data[12] = TempL.byte.u;     // Next Writable Address
            track_info_data[13] = TempL.byte.h;
            track_info_data[14] = TempL.byte.m;
            track_info_data[15] = TempL.byte.l;

            // ---------------------------------------------------------------------------------
            // BYTE[16]-[19]: Free Blocks
            // ---------------------------------------------------------------------------------

            if (TrackNumber)
            {
                // ------------------------------------
                // TRACK NUMBER IS NOT 0 = WRITTEN DISC
                // ------------------------------------
                if(pdbGetDVDRMARMDFormat() == Format4_RMD)
                {

                    TempULONG = pdbGetDVDRZoneLJA(TrackNumber);    // Layer Jump PSN
                    LayerJumpLBA = dvdConvertPSNtoLBA(TempULONG);
                    TempULONG = (~TempULONG)&0x00FFFFFFL;
                    TrackStartLBAL1 = dvdConvertPSNtoLBA(TempULONG);

                    switch (TrackState)
                    {
                        case eTRACK_STATE_EMPTY:
                        case eTRACK_STATE_BLANK:
                        case eTRACK_STATE_INCOMPLETE:
                            FreeBlocks.Long = (LayerJumpLBA -TrackStartLBA) +1 + (TrackStopLBA -TrackStartLBAL1) + 1;
                            break;

                        case eTRACK_STATE_COMPLETE:
                            FreeBlocks.Long = 0x00;
                            break;
                    }
                }
                else
                {   //Format1_RMD

                    switch (TrackState)
                    {
                        case eTRACK_STATE_EMPTY:
                            if(pdbGetDiscKind() == eDVDMinusRDisc)
                                FreeBlocks.Long = (pdbGetDVDMaxPSN(0) & 0xFFFFF0) - 0x30000 - Truncated_BorderOut_Size - TrackStartLBA ;
                            else    //DVD-R DL
                                FreeBlocks.Long = dvdConvertPSNtoLBA(pdbGetDVDMaxPSN(1)) + 1 - TrackStartLBA ;

                             break;

                        case eTRACK_STATE_BLANK:
                             FreeBlocks.Long = dvdConvertPSNtoLBA(pdbGetDVDTrackStartPSN(TrackNumber+1)) - TrackStartLBA - 0x10;
                             break;

                        case eTRACK_STATE_INCOMPLETE:
                            if (TrackNumber == pdbGetDVDHighestRecordedTrackNumber())
                            {
                                if(pdbGetDiscKind() == eDVDMinusRDisc)
                                    FreeBlocks.Long = (pdbGetDVDMaxPSN(0) & 0xFFFFF0) - 0x30000 - Truncated_BorderOut_Size - RecordedLBA ;
                                else    //DVD-R DL
                                    FreeBlocks.Long = dvdConvertPSNtoLBA(pdbGetDVDMaxPSN(1) & 0xFFFFF0) - RecordedLBA ;
                            }
                            else
                                FreeBlocks.Long = dvdConvertPSNtoLBA(pdbGetDVDTrackStartPSN(TrackNumber+1)) - RecordedLBA -0x10;

                             break;

                        case eTRACK_STATE_COMPLETE:
                             FreeBlocks.Long = 0x00;
                             break;

                    }

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
            // *** DVD+R: 16 (ALWAYS)
            // ---------------------------------------------------------------------------------
            track_info_data[23] = DVDMINUSR_PACKETSIZE;

            // ---------------------------------------------------------------------------------
            // BYTE[24]-[27]: Track Size
            // ---------------------------------------------------------------------------------
            if(pdbGetDVDRMARMDFormat() == Format4_RMD)
            {
                TempL.Long = TrackStopLBA;
            }
            else
            {
                switch(pdbGetDVDTrackState(TrackNumber))
                {
                    case eTRACK_STATE_EMPTY:
                            if(pdbGetDiscKind() == eDVDMinusRDisc)
                                FreeBlocks.Long = (pdbGetDVDMaxPSN(0) & 0xFFFFF0) - 0x30000 - Truncated_BorderOut_Size - TrackStartLBA ;
                            else    //DVD-R DL
                                FreeBlocks.Long = dvdConvertPSNtoLBA(pdbGetDVDMaxPSN(1)) + 1 - TrackStartLBA ;
                    break;
                    case eTRACK_STATE_INCOMPLETE:
                        if( pdbGetDVDTrackStatus(TrackNumber) & eTS_ReservedTrack)
                            TempL.Long = dvdConvertPSNtoLBA(pdbGetDVDTrackEndPSN(TrackNumber)) - TrackStartLBA + 1;
                        else
                        {
                            if(pdbGetDiscKind() == eDVDMinusRDisc)
                                TempL.Long = (pdbGetDVDTrackEndPSN(TrackNumber) & 0xFFFFF0) - 0x30000 - Truncated_BorderOut_Size - TrackStartLBA ;
                            else
                                TempL.Long = dvdConvertPSNtoLBA(pdbGetDVDMaxPSN(1)) + 1 - TrackStartLBA ;
                        }
                    break;

                    case eTRACK_STATE_COMPLETE:
                    case eTRACK_STATE_BLANK:
                        TempL.Long = dvdConvertPSNtoLBA(pdbGetDVDTrackEndPSN(TrackNumber)) - TrackStartLBA + 1;
                    break;
                }
            }

            track_info_data[24] = TempL.byte.u;   // Track Size
            track_info_data[25] = TempL.byte.h;
            track_info_data[26] = TempL.byte.m;
            track_info_data[27] = TempL.byte.l;

            // ---------------------------------------------------------------------------------
            // BYTE[28]-[31]: Last Recorded Address
            // ---------------------------------------------------------------------------------
            if(pdbGetDVDRMARMDFormat() == Format4_RMD)
            {
                TempL.Long = dvdConvertPSNtoLBA(pdbGetDVDTrackRecordedPSN(TrackNumber));
            }
            else
            {
                TempL.Long = dvdConvertPSNtoLBA(pdbGetDVDTrackRecordedPSN(TrackNumber));
            }

            track_info_data[28] = TempL.byte.u;   // Last Recorded Address
            track_info_data[29] = TempL.byte.h;
            track_info_data[30] = TempL.byte.m;
            track_info_data[31] = TempL.byte.l;

            track_info_data[32] = (pdbGetDVDHighestRecordedTrackNumber()) >> 8;
            track_info_data[33] = (pdbGetDVDBorderNumber()) >> 8;

            // ---------------------------------------------------------------------------------
            // BYTE[36]-[39]:Read Compatibility LBA
            // ---------------------------------------------------------------------------------
            TempL.Long = 0;
            track_info_data[36] = TempL.byte.u;
            track_info_data[37] = TempL.byte.h;
            track_info_data[38] = TempL.byte.m;
            track_info_data[39] = TempL.byte.l;

            // ---------------------------------------------------------------------------------
            // BYTE[40]-[43]: Next Layer Jump Address
            // ---------------------------------------------------------------------------------
            TempL.Long = dvdConvertPSNtoLBA(pdbGetDVDRZoneLJA(TrackNumber));
            track_info_data[40] = TempL.byte.u;
            track_info_data[41] = TempL.byte.h;
            track_info_data[42] = TempL.byte.m;
            track_info_data[43] = TempL.byte.l;

            // ---------------------------------------------------------------------------------
            // BYTE[44]-[47]: Last Layer Jump Address
            // ---------------------------------------------------------------------------------
            TempL.Long = 0;
            track_info_data[44] = TempL.byte.u;
            track_info_data[45] = TempL.byte.h;
            track_info_data[46] = TempL.byte.m;
            track_info_data[47] = TempL.byte.l;

            break;

        case eDVDMinusRWDisc://///////////////////////////////////////////////////////////////////////
            TrackState  = pdbGetDVDTrackState(TrackNumber);
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
            track_info_data[3] = pdbGetDVDTrackSessionNumber(TrackNumber);

            // ---------------------------------------------------------------------------------
            // BYTE[5]: Bit5-Damage, Bit4-Copy, Bits3-0-TrackMode
            // ---------------------------------------------------------------------------------
            track_info_data[5] = 0x04;

            // ---------------------------------------------------------------------------------
            // BYTE[6]: Bit7-RT, Bit6-Blank, Bit5-Packet/Inc, Bit4-FP, Bits3-0-DataMode
            // ---------------------------------------------------------------------------------
            if (pdbGetDVDTrackStatus(TrackNumber) & eTS_ReservedTrack)
                track_info_data[6] = track_info_data[6] | eTS_ReservedTrack;      // 0x80

            if (pdbGetDVDTrackStatus(TrackNumber) & eTS_BlankTrack)
                track_info_data[6] = track_info_data[6] | eTS_BlankTrack;         // 0x40

            if (pdbGetDVDTrackStatus(TrackNumber) & eTS_Packet)
                track_info_data[6] = track_info_data[6] | eTS_Packet;             // 0x20

           if (pdbGetDVDTrackStatus(TrackNumber) & eTS_Packet)
                track_info_data[6] = track_info_data[6] | eTS_FixedPacket;             // 0x10
            track_info_data[6] = track_info_data[6] | 0x01;

            // ---------------------------------------------------------------------------------
            // BYTE[7]: Bit1-LRA V, Bit0-NWA Valid
            // ---------------------------------------------------------------------------------
            if (St_blank_disc == TRUE)
               track_info_data[7] = 0x01;
            else
            {
                switch (pdbGetDVDRMADiscStatus())
                {
                    case DiscAtOnce:
                    case FinalIncDisc:
                    case ResOverWriteComBorder:
                        track_info_data[7] = 0x00;
                        break;
                    case IncRecMode:
                        switch (TrackState)
                        {
                            case eTRACK_STATE_EMPTY:
                            case eTRACK_STATE_BLANK:
                                track_info_data[7] = 0x01;
                                break;
                            case eTRACK_STATE_INCOMPLETE:
                                track_info_data[7] = 0x03;
                                break;
                            case eTRACK_STATE_COMPLETE:
                                track_info_data[7] = 0x02;
                                break;
                        }
                        break;
                    case ResOverWriteIntBorder:
                        if (TrackState == eTRACK_STATE_EMPTY)
                            track_info_data[7] = 0x01;
                        else
                            track_info_data[7] = 0x03;
                        break;
                }
            }

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
            if(track_info_data[7] & 0x01)
            {
                if (St_blank_disc == TRUE)
                    TempL.Long = 0x00;
                else
                {
                    switch (pdbGetDVDRMADiscStatus())
                    {
                        case IncRecMode:
                            TempL.Long = pdbGetDVDTrackNextWritableAddress(TrackNumber);   /*Return NWA do not include LLA*/
                            break;

                        case ResOverWriteComBorder:
                            TempL.Long = dvdConvertPSNtoLBA (pdbGetDVDTrackStartPSN(pdbGetDVDHighestRecordedTrackNumber()));
                            break;

                        case ResOverWriteIntBorder:
                            if (TrackState == eTRACK_STATE_EMPTY)
                                TempL.Long = TrackStartLBA;
                            else
                                TempL.Long = dvdConvertPSNtoLBA (pdbGetDVDTrackRecordedPSN(1) + 1);
                            break;
                    }
                }
            }
            else
                TempL.Long = 0x00;

            track_info_data[12] = TempL.byte.u;     // Next Writable Address
            track_info_data[13] = TempL.byte.h;
            track_info_data[14] = TempL.byte.m;
            track_info_data[15] = TempL.byte.l;

            // ---------------------------------------------------------------------------------
            // BYTE[16]-[19]: Free Blocks
            // ---------------------------------------------------------------------------------
            if (St_blank_disc == TRUE)
                FreeBlocks.Long = pdbGetDVDLeadOutStartPSN() - Truncated_BorderOut_Size - 0x10 - 0x30000;
            else
            {
                switch (pdbGetDVDRMADiscStatus())
                {
                    case DiscAtOnce:
                    case FinalIncDisc:
                    case ResOverWriteComBorder:
                        FreeBlocks.Long = 0x00;
                        break;
                    case IncRecMode:
                        switch (TrackState)
                        {
                            case eTRACK_STATE_EMPTY: //must be the highest track
                                FreeBlocks.Long = pdbGetDVDLeadOutStartPSN() - Truncated_BorderOut_Size - 0x10 - 0x30000 - TrackStartLBA;
                                break;
                            case eTRACK_STATE_BLANK: //may be in any track, except for highest track
                                FreeBlocks.Long = dvdConvertPSNtoLBA(pdbGetDVDTrackStartPSN(TrackNumber+1)) - TrackStartLBA - 0x10;
                                break;
                            case eTRACK_STATE_INCOMPLETE:
                                if (TrackNumber == pdbGetDVDHighestRecordedTrackNumber())
                                    FreeBlocks.Long = pdbGetDVDLeadOutStartPSN() - Truncated_BorderOut_Size-0x10- (pdbGetDVDTrackRecordedPSN(TrackNumber) + 0x11);
                                else //in the reserved track
                                    FreeBlocks.Long = dvdConvertPSNtoLBA(pdbGetDVDTrackStartPSN(TrackNumber+1)) - RecordedLBA -0x10;
                                break;
                            case eTRACK_STATE_COMPLETE:
                                FreeBlocks.Long = 0x00;
                                break;
                        }
                        break;
                    case ResOverWriteIntBorder:
                        if (TrackState == eTRACK_STATE_EMPTY)
                            FreeBlocks.Long = pdbGetDVDLeadOutStartPSN() - Truncated_BorderOut_Size - 0x10 - 0x30000 - TrackStartLBA;
                        else
                            FreeBlocks.Long = pdbGetDVDLeadOutStartPSN() - Truncated_BorderOut_Size-0x10- (pdbGetDVDTrackRecordedPSN(TrackNumber) + 1);
                        break;
                }
            }

            track_info_data[16] = FreeBlocks.byte.u;    // Free Blocks
            track_info_data[17] = FreeBlocks.byte.h;
            track_info_data[18] = FreeBlocks.byte.m;
            track_info_data[19] = FreeBlocks.byte.l;

            // ---------------------------------------------------------------------------------
            // BYTE[20]-[23]: Fixed Packet Size/Blocking Factor
            // *** : 16 (ALWAYS)
            // ---------------------------------------------------------------------------------
            track_info_data[23] = DVDMINUSR_PACKETSIZE;

            // ---------------------------------------------------------------------------------
            // BYTE[24]-[27]: Track Size
            // ---------------------------------------------------------------------------------
            if (St_blank_disc == TRUE)
                TempL.Long = pdbGetDVDLeadOutStartPSN() - Truncated_BorderOut_Size - 0x10 - 0x30000;
            else
            {
                switch (pdbGetDVDRMADiscStatus())
                {
                    case DiscAtOnce:
                    case FinalIncDisc:
                    case ResOverWriteComBorder:
                        TempL.Long = pdbGetDVDTrackEndPSN(TrackNumber)- pdbGetDVDTrackStartPSN(TrackNumber) + 1;
                        break;
                    case IncRecMode:
                        switch (TrackState)
                        {
                            case eTRACK_STATE_INCOMPLETE:
                            case eTRACK_STATE_EMPTY:
                                if( pdbGetDVDTrackStatus(TrackNumber) & eTS_ReservedTrack)
                                    TempL.Long = dvdConvertPSNtoLBA(pdbGetDVDTrackEndPSN(TrackNumber)) - TrackStartLBA + 1;
                                else
                                    TempL.Long = (pdbGetDVDTrackEndPSN(TrackNumber) & 0xFFFFF0) - 0x30000 - Truncated_BorderOut_Size - TrackStartLBA ;
                                break;

                            case eTRACK_STATE_COMPLETE:
                            case eTRACK_STATE_BLANK:
                                TempL.Long = dvdConvertPSNtoLBA(pdbGetDVDTrackEndPSN(TrackNumber)) - TrackStartLBA + 1;
                                break;
                        }
                        break;
                    case ResOverWriteIntBorder:
                        TempL.Long = pdbGetDVDLeadOutStartPSN() -0x30000- Truncated_BorderOut_Size-0x10;
                        break;
                }
            }

            track_info_data[24] = TempL.byte.u;   // Track Size
            track_info_data[25] = TempL.byte.h;
            track_info_data[26] = TempL.byte.m;
            track_info_data[27] = TempL.byte.l;

            // ---------------------------------------------------------------------------------
            // BYTE[28]-[31]: Last Recorded Address
            // ---------------------------------------------------------------------------------
            if(track_info_data[7] & 0x02)
            {
                switch (pdbGetDVDRMADiscStatus())
                {
                    case DiscAtOnce:
                    case IncRecMode:
                    case FinalIncDisc:
                    case ResOverWriteIntBorder:
                        TempL.Long = dvdConvertPSNtoLBA(pdbGetDVDTrackRecordedPSN(TrackNumber));
                        break;
                    case ResOverWriteComBorder:
                         TempL.Long = dvdConvertPSNtoLBA(pdbGetDVDTrackEndPSN(TrackNumber));
                        break;
                }
            }
            else
                TempL.Long = 0;

            track_info_data[28] = TempL.byte.u;   // Last Recorded Address
            track_info_data[29] = TempL.byte.h;
            track_info_data[30] = TempL.byte.m;
            track_info_data[31] = TempL.byte.l;

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

/*--------------------------------------------------------------------
  Function:     dvdCheckValidTrackNumber()

  Description:

  Returns:

  Notes:
--------------------------------------------------------------------*/

BOOL dvdCheckValidTrackNumber(BYTE trackNum)
{
    BYTE LowestRecordedTno, HighestRecordedTno;

    if((pdbServoGetDiscType() == eLPPDisc) && (trackNum == INVISIBLE_TRACK))
    {
        BUILD_SENSE(0x05,0x24,0x00,0x35);                               /* Invalid CDB */
        AutoCommandComplete();
        return FALSE;
    }

    LowestRecordedTno  = pdbGetDVDLowestRecordedTrackNumber();
    HighestRecordedTno = pdbGetDVDHighestRecordedTrackNumber();

    if ( (pdbGetDiscStatus() == eFinalizedDisc) &&
         ( (trackNum == INVISIBLE_TRACK) || (trackNum == HighestRecordedTno+1) ) )
    {
        // attempt to access the invisible track on a finalized disc
        BUILD_SENSE(0x05,0x21,0x00,0x13);                               /*LOGICAL BLOCK ADDRESS OUT OF RANGE */
        AutoCommandComplete();
        return FALSE;
    }

    if (HighestRecordedTno == 0xA9)
    {
        if ((trackNum == (HighestRecordedTno+1)) || (trackNum == INVISIBLE_TRACK))
        {
            // attempt to access track
            //  written the maximum number of tracks allowed
            BUILD_SENSE(0x05,0x63,0x00,0x14);                           /* END OF USER AREA ENCOUNTER ON THIS TRACK */
            AutoCommandComplete();
            return FALSE;
        }
    }

    // HighestRecordedTno+1 is the last empty track
    if (((trackNum == 0) || (trackNum < LowestRecordedTno) || (trackNum > HighestRecordedTno+1)) && (trackNum != INVISIBLE_TRACK)){
        BUILD_SENSE(0x05,0x21,0x00,0x15);                               /* LOGICAL BLOCK ADDRESS OUT OF RANGE */
        AutoCommandComplete();
        return FALSE;
    }

    return TRUE;
}

