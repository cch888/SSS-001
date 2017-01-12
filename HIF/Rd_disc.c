/****************************************************************************
*                (c) Copyright 2001 - 2008 SUNEXT DESIGN, INC.
*                           All Right Reserved
*
* FILENAME:   rd_disc.c
*
* DESCRIPTION
*
* $Revision: 46 $
* $Date: 11/03/16 5:59p $
*
*
**************************** SUNEXT CONFIDENTIAL *******************************/
#include ".\common\globtype.h"
#include ".\common\globals.h"
#include ".\common\chip.h"
#include ".\common\error.h"
#include ".\servo\svo.h"    /* Please include no servo file except svo.h */
#include ".\player\plrDb.h"
#include ".\common\commath.h"
#include ".\hif\mmc.h"
#include ".\hif\gp_mac.h"
#include ".\hif\app_if.h"
#include ".\hif\rd_util.h"
#include ".\hif\at_knl.h"
#include ".\hif\at_util.h"
#include ".\hif\bf_util.h"
#include ".\dvd\dvGvars.h"
#include ".\dvd\dvRdDisc.h"

extern  BYTE DVDSessionNum;
extern struct dvdDiscS          volatile    dvdDisc;
extern  BOOL AdjustDiscSpeed(long position);
extern BYTE chkDuplicateDVDSpeed( BYTE data_cnt );
extern void UpdateSendDVDOpcData( BYTE data_cnt );

/*--- MMC Send OPC Command definition ---*/
/* The Parameter List Length value shall be an integral multiple of 8bytes. */
#define TRN_SP_NUM          5
#define DVDTRN_SP_NUM       3
#define OPC_DATA_LEN        8
#define OPC_DATA_LEN_2      (OPC_DATA_LEN * 2)
#define OPC_DATA_LEN_3      (OPC_DATA_LEN * 3)
#define OPC_DATA_LEN_4      (OPC_DATA_LEN * 4)

const DiscSpeedT wr_speed[TRN_SP_NUM] = {e4XCLV,e8XCLV,e10XCLV,e12XCLV,e16XCLV};
const unsigned trns_speed[TRN_SP_NUM] = {SP4X,SP8X,SP10X,SP12X,SP16X};
BYTE    entries_opc_table[2][TRN_SP_NUM];       /* entries_opc_table[0][x] : Real OPC Done Mark */
                                                /* entries_opc_table[1][x] : Recv OPC Done Mark */

const DiscSpeedT DVDwr_speed[DVDTRN_SP_NUM] = {e2XDCLV, e4XDCLV, e6XDCLV};
const unsigned DVDtrns_speed[DVDTRN_SP_NUM] = {SP2XDVD, SP4XDVD, SP6XDVD};
BYTE    DVDentries_opc_table[2][DVDTRN_SP_NUM]; /* entries_opc_table[0][x] : Real OPC Done Mark */
                                                /* entries_opc_table[1][x] : Recv OPC Done Mark */
BYTE    disc_info[96];

/********************************************************************/
/*                                                                  */
/*   Read Disc Information                                          */
/*                                                                  */
/********************************************************************/
void CDReadDiscInfoCommand(void)
{
    TimeS   leadin_time,leadout_time, next_start_time;
    long    next_start_lba,leadout_lba;
    USHORT  disc_info_len, disc_info_opc_len, disc_info_total_len,  Tracks, tmpTrk;
    BYTE    SessionInf,i, opc_table_entries, DataType;
    BOOL    cdrom_cdi;

    DataType = Iram_Cmd_block.bf[1] & 0x07;

    SET_INT_BYTE_HIGH(disc_info_len) = Iram_Cmd_block.bf[7];
    SET_INT_BYTE_LOW(disc_info_len)  = Iram_Cmd_block.bf[8];

    if (disc_info_len == 0){
        AutoCommandComplete();
        return;
    }

    switch(DataType)
    {
        case STANDARD_DISC_INFORMATION:
            if ((pdbGetDiscStatus() == eFinalizedDisc) && (pdbGetDiscModeType(0) == eCDIMode) &&
                (pdbGetLowestRecordedTrackNumber() > 1) ){
                cdrom_cdi = TRUE;
            }else{
                cdrom_cdi = FALSE;
            }

            disc_info_opc_len = 0;
            opc_table_entries = 0;

            /************************************************/
            /*         Initialize Return Data Array         */
            /************************************************/
            disc_info_total_len = DISC_INFO_LEN_BYTE + (SHORT)(opc_table_entries * 8);
            for(i=0;i<(disc_info_total_len+2);i++) disc_info[i] = 0;

            /************************************************/
            /*        Bytes[0,1] : Disc Info Length         */
            /************************************************/
            if(disc_info_len > disc_info_total_len) disc_info_len = disc_info_total_len;

            disc_info[0] = (BYTE)((disc_info_total_len-2) >> 8);
            disc_info[1] = (BYTE)(disc_info_total_len-2);

            /************************************************/
            /*        Bytes[2] : Disc/Session Status        */
            /************************************************/
            leadout_time = pdbGetATIPLeadOut();         /* Last Possible Start Time for Start of lead-out   */

            SessionInf = pdbGetFinalSessCloseInfo();
            switch(SessionInf){
                case eSessionFinalized:
                    disc_info[2] |= 0x0E;               /* Complete Session     Complete Disc   */
                    break;
                case eSessionClosed:
                    disc_info[2] |= 0x0D;               /* Complete Session     Incomplete Disc */
                    break;
                case eNotSessionClosed:
                    disc_info[2] |= 0x05;               /* Incomplete Session   Incomplete Disc */
                    break;
                case eEmptySession:
                    if(pdbGetFinalSessNumber() == 1){
                        disc_info[2] &= 0x00;           /* Empty Session        Empty Disc      */
                    }
                    else{
                        leadout_lba     = MSF2LBA_Plus(&leadout_time);
                        next_start_time = pdbGetNextSessStartTime();
                        next_start_lba  = MSF2LBA_Plus(&next_start_time);

                        if (leadout_lba <= next_start_lba){
                            disc_info[2] |= 0x0D;       /* Complete Session     Incomplete Disc */
                        }else{
                            disc_info[2] |= 0x01;       /* Empty session        Incomplete Disc */
                        }
                    }
                    break;
                default:                                /* Empty session        */
                    break;
            }

            if(pdbGetCDRW()) /* CD-RW ?              */
                disc_info[2] |= 0x10;                   /* Eraseable bit        */

            disc_info[2] |= (DataType << 5);                   /* Disc Information Data Type  */
            /************************************************/
            /*      Bytes[3,4,5,6] : Session Status         */
            /************************************************/
            if (cdrom_cdi == TRUE){
                /* cdi_plus_audio disc */
                if (St_cdi_audio == FALSE)
                {
                    disc_info[3] = 1;                   /* Number of First Track on Disc        */
                    disc_info[4] = 1;                       /* Number of Sessions                   */
                    disc_info[5] = 1;                       /* First Track Number in Last Session   */
                    disc_info[6] = 1;                       /* Last  Track Number in Last Session   */
                }
                else
                {
                    disc_info[3] = pdbGetLowestRecordedTrackNumber();
                    disc_info[4] = pdbGetFinalSessNumber();
                    disc_info[5] = pdbGetFinalSessFirstTrackNumber();
                    disc_info[6] = pdbGetFinalSessLastTrackNumber();
                }
            }else{
                disc_info[3] = pdbGetLowestRecordedTrackNumber();
                disc_info[4] = pdbGetFinalSessNumber();
                disc_info[5] = pdbGetFinalSessFirstTrackNumber();
                if (pdbGetHighestRecordedTrkNumber() == 99){
                    disc_info[6] = 99;                  /* Written 99 tracks */
                }else{
                    disc_info[6] = pdbGetFinalSessLastTrackNumber();
                }
            }

            /*******************************************************************************/
            /* Bytes[7] : Valid Bits-ID,BarCode,AppCode,Dirty and Background Format Status */
            /*******************************************************************************/
            disc_info[7] = 0x20;    //URU=1 except special restricted use CD-R/RW,DVD-R/RW.


            /************************************************/
            /*           Bytes[8] : Disc Type               */
            /************************************************/
            disc_info[8] = pdbGetFinalSessFormat();     /* Disc Type                            */

            /************************************************/
            /*    Bytes[12,13,14,15] : Disc Identification  */
            /************************************************/

                disc_info[7] &= (~0x80);    /* DID_V                                */
                disc_info[12] = 0x00;
                disc_info[13] = 0x00;
                disc_info[14] = 0x00;
                disc_info[15] = 0x00;

            /***********************************************************/
            /* Bytes[16,17,18,19] : Last Session Lead In Start Address */
            /***********************************************************/
            if (SessionInf == eSessionFinalized){
                disc_info[16] = 0xFF;
                disc_info[17] = 0xFF;                               /* Last Session lead-in Start Time                  */
                disc_info[18] = 0xFF;
                disc_info[19] = 0xFF;
                disc_info[20] = 0xFF;
                disc_info[21] = 0xFF;                               /* Last Possible Start Time for Start of lead-out   */
                disc_info[22] = 0xFF;
                disc_info[23] = 0xFF;
            }else{
                leadin_time   = pdbGetFinalSessLeadinTime();        /* Last Session lead-in Start Time                  */
                disc_info[17] = leadin_time.Minute;
                disc_info[18] = leadin_time.Second;
                disc_info[19] = leadin_time.Frame;
                disc_info[21] = leadout_time.Minute;
                disc_info[22] = leadout_time.Second;
                disc_info[23] = leadout_time.Frame;
            }

            /***********************************************************/
            /*                       OPC Information                   */
            /***********************************************************/
            if(opc_table_entries == 0){                             /* Empty Session        Empty Disc      */
                opc_table_entries    = 0;
                disc_info_total_len -= disc_info_opc_len;
                SET_INT_BYTE_HIGH(disc_info_len) = Iram_Cmd_block.bf[7];
                SET_INT_BYTE_LOW(disc_info_len)  = Iram_Cmd_block.bf[8];
                if(disc_info_len > disc_info_total_len) disc_info_len = disc_info_total_len;
                disc_info[0] = (BYTE)((disc_info_total_len-2) >> 8);
                disc_info[1] = (BYTE)(disc_info_total_len-2);
            }

            /************************************************/
            /*       Bytes[33] : Number of OPC Tables       */
            /************************************************/
            disc_info[33] = opc_table_entries;

            break;

        case TRACK_RESOURCES_INFORMATION:
            disc_info_len = 0x0A;
            disc_info[0] = 0x00;
            disc_info[1] = disc_info_len;

            disc_info[2] = (DataType << 5); /* Disc Information Data Type  */
            disc_info[3] = 0x00;

            Tracks = 0x63;  /* 99 track*/
            disc_info[4] = Tracks >> 8; /*Maximum possible number of the tracks */
            disc_info[5] = Tracks;

            Tracks = pdbGetFinalSessLastTrackNumber();
            disc_info[6] = 0x00; /*Number of the assigned tracks */
            disc_info[7] = Tracks;

            Tracks = 0;
            if( pdbServoGetDiscType() == eATIPDisc ) /*  CD-R/RW  */
            {
                tmpTrk = pdbGetFinalSessLastTrackNumber();
                for(tmpTrk = pdbGetFinalSessFirstTrackNumber(); tmpTrk<=pdbGetFinalSessLastTrackNumber(); tmpTrk++)
                {
                    if(pdbGetTrackState(tmpTrk) != eTRACK_STATE_COMPLETE)
                        Tracks++;
                }

                    Tracks = 99 - pdbGetFinalSessFirstTrackNumber() + Tracks;
            }
            disc_info[8] = 0x00; /*Maximum possible number of appendanle tracks */
            disc_info[9] = Tracks;

            Tracks = 0;
            for(tmpTrk = pdbGetFinalSessFirstTrackNumber(); tmpTrk<=pdbGetFinalSessLastTrackNumber(); tmpTrk++)
            {
                if(pdbGetTrackState(tmpTrk) != eTRACK_STATE_COMPLETE)
                    Tracks++;
            }
            disc_info[10] = 0x00; /*Current number of appendanle tracks */
            disc_info[11] = Tracks;
            break;

        case POW_RESOURCES_INFORMATION:
        default:
            InvalidCommand();
            return;
            break;
    }

    Iram_Buf_dram_addr = DISC_INFO_ADR;

    for(i=0;i<disc_info_total_len;i++){
        WriteSDRAM(disc_info[i]);
    }
    SendParameterDataToHost(DISC_INFO_ADR, disc_info_len, TRUE);   // Transfer Data and Command Complete.
}

#if 0
/*******************************************************************************

    FUNCTION        dvdReadDiscInfo

    DESCRIPTION     Function prepares the return data for
                    ATAPI Command Read Disc Information if a DVD type disc is in the tray.
                    It then the data to the host.

    PARAMETERS      None

    RETURNS         None

********************************************************************************/
void dvdReadDiscInfo(void)
{
    BYTE    *   DramAddressP;
    WORD    InfoLength;
    BYTE    Count;

    InfoLength  = Iram_Cmd_block.bf[7] << 8;
    InfoLength += Iram_Cmd_block.bf[8];
    // Prepare the Disc Information Block in DRAM Scratch Area
    DramAddressP       = (BYTE *) (SCSDRAM_ADDRESS + BUFFER_SCRATCH);   // Point to work area in DRAM

    /* correct dvd disc info */
    *DramAddressP++ = 0x00;                           // Byte  0  Disc Information Length
    *DramAddressP++ = 0x20;                           // Byte  1
    *DramAddressP++ = 0x0E;                           // Byte  2  Status of Last session = 11b, Disc Status Field = 10b
    *DramAddressP++ = 0x01;                           // Byte  3  Number of First Track / Zone on Disc

    /*--------------------------------------------------------------
        Byte  4  Number of Sessions / Borders (LSB)
            DVD-R/RW Disc Maximum Session = 2
    ---------------------------------------------------------------*/
    if(((pdbGetDVDType() == DVD_MINUS_R) ||(pdbGetDVDType() == DVD_MINUS_RW))&&(DVDSessionNum > 1))
        *DramAddressP++ = 2;
    else
        *DramAddressP++ = DVDSessionNum;

    /*--------------------------------------------------------------
        Byte  5 First Track / RZone in Last Session Border (LSB)
            DVD-R/RW Disc Maximum track no = 2
    ---------------------------------------------------------------*/
    if(((pdbGetDVDType() == DVD_MINUS_R) ||(pdbGetDVDType() == DVD_MINUS_RW))&&(DVDSessionNum > 1))
        *DramAddressP++ = 2;
    else
        *DramAddressP++ = DVDSessionNum;

    /*--------------------------------------------------------------
        Byte  6 Last  Track / RZone in Last Session Border (LSB)
            DVD-R/RW Disc Maximum track no = 2
    ---------------------------------------------------------------*/
    if(((pdbGetDVDType() == DVD_MINUS_R) ||(pdbGetDVDType() == DVD_MINUS_RW))&&(DVDSessionNum > 1))
        *DramAddressP++ = 2;
    else
        *DramAddressP++ = DVDSessionNum;

    *DramAddressP++ = 0x20;                           // Byte  7  URU bit = 1 (Unrestricted Use)
    *DramAddressP++ = 0xFF;                           // Byte  8  Disc Type (Undefined)

    for(Count = 0 ;Count < 7;Count++)                // The following bytes
        *DramAddressP++ = 0x00;                       //   are all 0
                                                      // Byte  9  Number of Sessions / Borders (MSB)
                                                      // Byte 10  First Track / RZone in Last Session Border (LSB)
                                                      // Byte 11  Last  Track / RZone in Last Session Border (LSB)
                                                      // Byte 12  Disc Identification (MSB)
                                                      // Byte 13  Disc Identification
                                                      // Byte 14  Disc Identification
                                                      // Byte 15  Disc Identification (LSB)

    if((pdbGetDVDType() == DVD_PLUS_R)||(pdbGetDVDType() == DVD_PLUS_R_DL))   //DVD+R
    {                                                               //Byte16 ~ Byte 23 all set 0xff
        for(Count = 0 ;Count < 8;Count++)
            *DramAddressP++ = 0xFF;
    }
    else                                                         //other
    {                                                              //Byte16 ~ Byte23 all set 0x00
        for(Count = 0 ;Count < 8;Count++)
            *DramAddressP++ = 0x00;
    }
                                                      // Byte 16  Lead-in Start Time of Last Session MSF  (MSB)
                                                      // Byte 17  Lead-in Start Time of Last Session MSF
                                                      // Byte 18  Lead-in Start Time of Last Session MSF
                                                      // Byte 19  Lead-in Start Time of Last Session MSF  (LSB)
                                                      // Byte 20  Last Possible Start Time for Start Lead-out MSF (MSB)
                                                      // Byte 21  Last Possible Start Time for Start Lead-out MSF
                                                      // Byte 22  Last Possible Start Time for Start Lead-out MSF
                                                      // Byte 23  Last Possible Start Time for Start Lead-out MSF  (LSB)

    for(Count = 0 ;Count < 10;Count++)                // The following bytes
        *DramAddressP++ = 0x00;                       //   are all 0

                                                      // Byte 24  Disc Bar Code (MSB)
                                                      // Byte 25  Disc Bar Code
                                                      // Byte 26  Disc Bar Code
                                                      // Byte 27  Disc Bar Code
                                                      // Byte 28  Disc Bar Code
                                                      // Byte 29  Disc Bar Code
                                                      // Byte 30  Disc Bar Code
                                                      // Byte 31  Disc Bar Code (LSB
                                                      // Byte 32  Reserved
                                                      // Byte 33  Number of OPC Table Entries

    // Send Data transfer to host
    SendParameterDataToHost(BUFFER_SCRATCH, MINIMUM(34,InfoLength), TRUE);  // Command Complete
}
#endif

