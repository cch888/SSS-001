/****************************************************************************
*                (c) Copyright 2001  -  2005 SUNEXT DESIGN, INC.
*                           All Right Reserved
*
*
* FILENAME:   rd_track.c
*
* DESCRIPTION
*
*   $Revision: 30 $
*   $Date: 11/03/16 5:59p $
*
*
************************ SUNEXT CONFIDENTIAL *******************************/

/********************************************************/
/*                                                      */
/*   Read Track Information  Command                    */
/*                                                      */
/********************************************************/

#include ".\common\globals.h"
#include ".\common\globtype.h"
#include ".\common\chip.h"
#include ".\common\error.h"
#include ".\common\constant.h"//Data mode return 0x0F when No TDB.
#include ".\servo\svo.h"    /* Please include no servo file except svo.h */
#include ".\player\plrDb.h"
#include ".\common\commath.h"
#include ".\hif\mmc.h"
#include ".\hif\gp_mac.h"
#include ".\hif\gp_util.h"
#include ".\hif\app_if.h"
#include ".\hif\rd_util.h"
#include ".\hif\sv_util.h"
#include ".\hif\at_knl.h"
#include ".\hif\at_util.h"
#include ".\hif\bf_util.h"

extern BYTE track_info_data[TRACK_INFO_LEN_BYTE];

extern USHORT track_info_len;

extern  BYTE DVDSessionNum;
extern struct dvdSessionS   volatile  dvdSession;
extern struct dvdDiscS          volatile    dvdDisc;

void    Read_track_info_CDi(BYTE tno);  /* for cdi disc */
BOOL CheckIfDiscIsCDText(BYTE tno);


/*--------------------------------------------------------------------
  Name: CDReadTrackInfoCommand()
  Description:
    Processes ATAPI Read Track Info command
  Inputs:
    none
  Outputs:
    none
  Returns:
    none
--------------------------------------------------------------------*/
void CDReadTrackInfoCommand(void)
{
    BYTE  cont_addr,tno,last_tno,data_mode,i,sno;
    BYTE  TrackState,TrackStatus,TrackDiagStatus2;
    long  start_lba,stop_lba,target_lba,lba;
    long  free_block,packet_size,track_size;
    TimeS start_time,stop_time;
    BOOL  cdrom_cdi;

    if( pdbServoGetDiscDiameter() == eNoDisc )      /* No disc ? */
    {
        BUILD_SENSE(0x02,0x3A,0x00,0x13);           /* Medium not present */
        AutoCommandComplete();
        return;
    }

    SET_INT_BYTE_HIGH(track_info_len) = Iram_Cmd_block.bf[7];
    SET_INT_BYTE_LOW(track_info_len)  = Iram_Cmd_block.bf[8];

    if (track_info_len == 0){
        AutoCommandComplete();
        return;
    }

    if ((pdbGetDiscStatus() == eFinalizedDisc) && (pdbGetDiscModeType(0) == eCDIMode) &&
        (pdbGetLowestRecordedTrackNumber() > 1) ){
        cdrom_cdi = TRUE;
    }else{
        cdrom_cdi = FALSE;
    }

    if(track_info_len > TRACK_INFO_LEN_BYTE)
        track_info_len = TRACK_INFO_LEN_BYTE;

    for(i=0; i < track_info_len; i++)
        track_info_data[i] = 0;     /* track information   */

    if( pdbServoGetDiscType() == eNonATIPDisc )                         /*  NO ATIP DISC        */
    {
                                                                    /*** Byte[0],[1] : Data length       ***/
        if((Iram_Cmd_block.bf[1] & 0x03) == 0x01){                          /* Track bit            */
            tno = Iram_Cmd_block.bf[5];                                     /* Track number         */

            if(tno != 0x00){
                if (CheckIfDiscIsCDText(tno) == FALSE) return;
                if(tno == 0xFF){                                        /* invisible track      */
                    tno = pdbGetFinalSessLastTrackNumber();
                }else{
                    if((tno > pdbGetFinalSessLastTrackNumber() || tno > 99)){
                        BUILD_SENSE(0x05,0x21,0x00,0x99);                  /* LOGICAL BLOCK ADDRESS OUT OF RANGE */
                        AutoCommandComplete();
                        return;
                    }
                }
                track_info_data[2] = tno;                           /*** Byte[3] : Track number          ***/
                start_time = pdbGetStartTime(tno);                      /* start time of track */
                stop_time  = pdbGetStopTime(tno);                       /* stop time of track  */
                start_lba  = MSF2LBA_Plus(&start_time);                 /* start lba of track  */
                stop_lba   = MSF2LBA_Plus(&stop_time);                  /* stop lba of track   */
            }else{
                track_info_data[2] = 0x00;                          /*** Byte[3] : Track number          ***/
                tno = pdbGetLowestRecordedTrackNumber();
                start_time.Minute = 0x63;
                start_time.Second = 0x00;
                start_time.Frame = 0x00;
                start_lba  = MSF2LBA(&start_time);                /* start lba of track  */
            }

        }else if ((Iram_Cmd_block.bf[1] & 0x03) == 0x02){
            sno = Iram_Cmd_block.bf[5];                                     /* Session number      */
            last_tno = pdbGetFinalSessLastTrackNumber();                /* last track number   */

            for(tno = 1; tno <= last_tno ; tno++){
                if (pdbGetTrackSessionNumber(tno) == sno) break;
            }

            if(tno > last_tno){
                BUILD_SENSE(0x05,0x21,0x00,0x9a);                          /* LOGICAL BLOCK ADDRESS OUT OF RANGE */
                AutoCommandComplete();
                return;
            }

            if (CheckIfDiscIsCDText(tno) == FALSE) return;

            start_time = pdbGetStartTime(tno);                          /* start time of track */
            stop_time  = pdbGetStopTime(tno);                           /* stop time of track  */
            start_lba  = MSF2LBA_Plus(&start_time);                     /* start lba of track  */
            stop_lba   = MSF2LBA_Plus(&stop_time);                      /* stop lba of track   */
            track_info_data[2] = tno;                               /*** Byte[3] : Track number          ***/

        }else if ((Iram_Cmd_block.bf[1] & 0x03) == 0x00){
            target_lba = (long)(PacketCommandToLBALEN4(2));
            last_tno = pdbGetFinalSessLastTrackNumber();                /* last track number   */

            for(tno = 1; tno <= last_tno ; tno++){
                start_time = pdbGetStartTime(tno);                      /* start time of track */
                stop_time  = pdbGetStopTime(tno);                       /* stop time of track  */
                start_lba  = MSF2LBA_Plus(&start_time);                 /* start lba of track  */
                stop_lba   = MSF2LBA_Plus(&stop_time);                  /* stop lba of track   */
                if((start_lba <= target_lba) && (stop_lba >= target_lba)) break;
            }

            if(tno > last_tno){
                BUILD_SENSE(0x05,0x21,0x00,0x9b);                          /* LOGICAL BLOCK ADDRESS OUT OF RANGE */
                AutoCommandComplete();
                return;
            }

            if (CheckIfDiscIsCDText(tno) == FALSE) return;

            track_info_data[2] = tno;                               /*** Byte[3] : Track number          ***/
        }else{
            BUILD_SENSE(0x05,0x26,0x00,0x9c);                                /* INVALID FIELD IN PARAMETER LIST  */
            AutoCommandComplete();
            return;

        }

    }else{ /* CD-R,CD-RW */

        tno = Iram_Cmd_block.bf[5];                                         /* Track number         */
        
        if((Iram_Cmd_block.bf[1] & 0x03) == 0x01){                          /* Track bit            */
            tno = Iram_Cmd_block.bf[5];                                     /* Track number        */

            if(tno != 0x00){
                if (CheckIfDiscIsCDText(tno) == FALSE) return;
                if(tno == 0xFF){                                        /* invisible track     */
                    tno = pdbGetFinalSessLastTrackNumber();
                }else{
                    if((tno > pdbGetFinalSessLastTrackNumber()) || (tno > 99)){
                        BUILD_SENSE(0x05,0x21,0x00,0x9d);                  /* LOGICAL BLOCK ADDRESS OUT OF RANGE */
                        AutoCommandComplete();
                        return;
                    }
                }

                track_info_data[2] = tno;                           /*** Byte[3] : Track number          ***/
                start_time = pdbGetStartTime(tno);                      /* start time of track */
                stop_time  = pdbGetStopTime(tno);                       /* stop time of track  */
                start_lba  = MSF2LBA_Plus(&start_time);                 /* start lba of track  */
                stop_lba   = MSF2LBA_Plus(&stop_time);                  /* stop lba of track   */
            }
            else
            {
                track_info_data[2] = 0x00;                          /*** Byte[3] : Track number          ***/
                tno = pdbGetLowestRecordedTrackNumber();
                start_lba  = 0;                /* start lba of track  */

            }

        }else if ((Iram_Cmd_block.bf[1] & 0x03) == 0x02){
            sno = Iram_Cmd_block.bf[5];                                     /* Session number      */
            last_tno = pdbGetFinalSessLastTrackNumber();                /* last track number   */

            for(tno = 1; tno <= last_tno ; tno++){
                if (pdbGetTrackSessionNumber(tno) == sno) break;
            }

            if(tno > last_tno){
                BUILD_SENSE(0x05,0x21,0x00,0x9e);                          /* LOGICAL BLOCK ADDRESS OUT OF RANGE */
                AutoCommandComplete();
                return;
            }

            if (CheckIfDiscIsCDText(tno) == FALSE) return;

            start_time = pdbGetStartTime(tno);                          /* start time of track */
            stop_time  = pdbGetStopTime(tno);                           /* stop time of track  */
            start_lba  = MSF2LBA_Plus(&start_time);                     /* start lba of track  */
            stop_lba   = MSF2LBA_Plus(&stop_time);                      /* stop lba of track   */

            track_info_data[2] = tno;                               /*** Byte[3] : Track number          ***/

        }else if ((Iram_Cmd_block.bf[1] & 0x03) == 0x00){
            target_lba = (long)(PacketCommandToLBALEN4(2));

            last_tno = pdbGetFinalSessLastTrackNumber();                /* last track number   */

            {
                for(tno = 1; tno <= last_tno ; tno++){
                    start_time = pdbGetStartTime(tno);                  /* start time of track */
                    stop_time  = pdbGetStopTime(tno);                   /* stop time of track  */
                    start_lba  = MSF2LBA_Plus(&start_time);             /* start lba of track  */
                    stop_lba   = MSF2LBA_Plus(&stop_time);              /* stop lba of track   */
                    if((start_lba <= target_lba) && (stop_lba >= target_lba)) break;
                }
            }

            if(tno > last_tno){
                BUILD_SENSE(0x05,0x21,0x00,0x9f);                          /* LOGICAL BLOCK ADDRESS OUT OF RANGE */
                AutoCommandComplete();
                return;
            }

            if (CheckIfDiscIsCDText(tno) == FALSE) return;

            track_info_data[2] = tno;                               /*** Byte[3] : Track number          ***/
        }else{
            BUILD_SENSE(0x05,0x26,0x00,0xa0);                                /* INVALID FIELD IN PARAMETER LIST  */
            AutoCommandComplete();
            return;
        }
    }

    if (((cdrom_cdi == TRUE) || (St_cdi == TRUE)) && (Iram_Cmd_block.bf[5] != 0xFF)){       /* for cdi_plus_audio disc */
        data_mode = pdbGetTrackDataMode(tno);
        if(data_mode == 0x0F)
        {
            CDCheckTrackDataMode(tno);
        }
        Read_track_info_CDi(tno);
        return;
    }

    {
        data_mode = pdbGetTrackDataMode(tno);
        if(data_mode == 0x0F)
        {
            //start == Data mode return 0x0F when No TDB.
            if((Iram_Cmd_block.bf[1] & 0x03) == 0x01)  /*Track bit*/
            {
                if(CheckTrackWriteMethod(start_lba) != TRKNO_NOTVALID)//read TDB
                {
                    if((pdbGetTrackDiagStatus(tno) & eTS_Diag_TDB) == eTS_Diag_TDB)
	             {
	                 CDCheckTrackDataMode(tno);
                    }    
                }    
	    }
            else            
            //end == 
                CDCheckTrackDataMode(tno);
        }
    }
    if(track_info_data[2] == 0)
    {
        TrackDiagStatus2 = 0;
        TrackStatus = 0;
        TrackState = 0;
        data_mode = 0;
        cont_addr = 0;
    }
    else
    {
        TrackDiagStatus2 = pdbGetTrackDiagStatus2(tno);
        TrackStatus = pdbGetTrackStatus(tno);
        TrackState = pdbGetTrackState(tno);
        data_mode = pdbGetTrackDataMode(tno);                               /* Data Mode            */
        cont_addr = pdbGetTrackContAddr(tno);                               /* Get Track Mode       */
    }

/************************************************************************************************/
/*                                  Set Track Information                                       */
/************************************************************************************************/

    /*  Set Track Information   */
    track_info_data[0] = 0x00;
    track_info_data[1] = TRACK_INFO_LEN_BYTE - 2;

    track_info_data[3] = pdbGetTrackSessionNumber(tno);             /*** Byte[3] : Session number        ***/

/************************************************************************************************/
/*                              Set Write method,Data Mode                                      */
/************************************************************************************************/
                                                                    /*** Byte[6] : Set Write method ***/
    track_info_data[6] = (TrackStatus & 0xF0);                          /* RT,Blank,Packet,FP   */
    track_info_data[6] |= data_mode;                                    /* Data Mode            */
    track_info_data[5] &= (~0x10);                                      /* Set copyright        */
    track_info_data[5] |= cont_addr >>4;

/************************************************************************************************/
/*                                  Set Track Start Address                                     */
/************************************************************************************************/
    track_info_data[8]  = (char)((char *)&start_lba)[3];            /*** Byte[8][9][10][11] : Track Start Address ***/
    track_info_data[9]  = (char)((char *)&start_lba)[2];
    track_info_data[10] = (char)((char *)&start_lba)[1];
    track_info_data[11] = (char)((char *)&start_lba)[0];

/************************************************************************************************/
/*                              Set Track Size and Free Block Size                              */
/************************************************************************************************/
    if(track_info_data[2] != 0x00)
    {
        switch(TrackState)
        {
            case eTRACK_STATE_COMPLETE:
                track_info_data[7] = 0x00;                              /* NWA is invalid   */
                /* Clear NWA Value */
                track_info_data[12]  = track_info_data[13] = track_info_data[14] = track_info_data[15] = 0x00;
                if( pdbServoGetDiscType() == eNonATIPDisc ){            /*  NO ATIP DISC    */
                    stop_time  = pdbGetStopTime(tno);
                    stop_lba   = MSF2LBA_Plus(&stop_time);
                    track_size = stop_lba - start_lba + 1L;
                }else{                                                  /* ATIP DISC        */
                    if(TrackStatus & eTS_FixedPacket){
                        stop_time  = pdbGetStopTime(tno);
                        stop_lba   = MSF2LBA_Plus(&stop_time);
                        lba        = stop_lba - start_lba -1L;
                        track_size = (lba + 5L + 2L) / (packet_size + 7L) * packet_size;
                    }else{
                        stop_time  = pdbGetStopTime(tno);
                        stop_lba   = MSF2LBA_Plus(&stop_time);
                        if (pdbGetReadInfoMethod() == eReadTOC){
                            track_size = stop_lba - start_lba + 1L;     /* DAO */
                        }else{
                            track_size = stop_lba - start_lba - 1L;
                        }
                    }
                }
                free_block = 0x0L;
                break;
        }
    }else{
        track_size = -151L;                                             /*  99:59:74    */
        track_size -= MSF2LBA_Plus(&start_time);
        track_size += 1L;
        track_size -= 150L;

        if ((pdbGetFinalSessCloseInfo() == eEmptySession) && (pdbGetFinalSessNumber() == 1)) {
            free_block = track_size;
            track_info_data[7] |= 0x01;
        }else{
            free_block = 0L;
        }
    }

                                                                    /*** Byte[24][25][26][27] : Track Size  ***/
    track_info_data[24] = (char)((char *)&track_size)[3];
    track_info_data[25] = (char)((char *)&track_size)[2];
    track_info_data[26] = (char)((char *)&track_size)[1];
    track_info_data[27] = (char)((char *)&track_size)[0];

    /*** copy track_info_data[] to DRAM ***/
    Iram_Buf_dram_addr = TRACK_INFO_ADR;

    for(i=0;i<TRACK_INFO_LEN_BYTE;i++)
    {
        WriteSDRAM(track_info_data[i]);
    }

    /*** now send the data back to the host.***/
    SendParameterDataToHost(TRACK_INFO_ADR, track_info_len , TRUE);     // Send parameters and Command Complete.
}


/*--------------------------------------------------------------------
  Name: Read_track_info_CDi()
  Description:
    Reads track info for CDi
  Inputs:
    none
  Outputs:
    none
  Returns:
    none
--------------------------------------------------------------------*/
void Read_track_info_CDi(BYTE tno)
{
    BYTE cont_addr,data_mode,i;
    BYTE  TrackStatus;
    TimeS stop_time, start_time;
    long  track_size, start_lba;

    /*  Set Track Information   */
    track_info_data[0] = 0x00;                                      /*** Byte[ 0],[ 1] : Data length    ***/
    track_info_data[1] = TRACK_INFO_LEN_BYTE - 2;
    track_info_data[2] = tno;
    track_info_data[3]  = 0x01;                                     /*** Byte[ 3] : Session number      ***/

    cont_addr = pdbGetTrackContAddr(tno);
    track_info_data[5] &= (~0x10);                                      /* Set copyright        */
    track_info_data[5] |= cont_addr >>4;

    TrackStatus = pdbGetTrackStatus(tno);
    data_mode = pdbGetTrackDataMode(tno);
    track_info_data[6] = (TrackStatus & 0xF0);                          /* RT,Blank,Packet,FP   */
    track_info_data[6] |= data_mode;                                    /* Data Mode            */

    track_info_data[7]  = 0x00;                                     /*** Byte[ 7] : NWA is valid        ***/

    start_time = pdbGetStartTime(tno);                  /* start time of track */
    start_lba  = MSF2LBA_Plus(&start_time);             /* start lba of track  */

    track_info_data[8]  = (char)((char *)&start_lba)[3];         /*** Byte[ 8][ 9][10][11] : Track Start Address   ***/
    track_info_data[9]  = (char)((char *)&start_lba)[2];
    track_info_data[10] = (char)((char *)&start_lba)[1];
    track_info_data[11] = (char)((char *)&start_lba)[0];

    track_info_data[16] = 0x00;                                     /*** Byte[16][17][18][19] : Free blocks Size      ***/
    track_info_data[17] = 0x00;
    track_info_data[18] = 0x00;
    track_info_data[19] = 0x00;

    track_info_data[20] = 0x00;                                     /*** Byte[20][21][22][23] : Fixed packet Size     ***/
    track_info_data[21] = 0x00;
    track_info_data[22] = 0x00;
    track_info_data[23] = 0x00;

    stop_time = pdbGetStopTime(tno) ;
    track_size = MSF2LBA_Plus(&stop_time) + 1L;

                                                                    /*** Byte[12][13][14][15] : Next Writable Address ***/
                                                                    /*** Byte[24][25][26][27] : Track Size            ***/
    track_info_data[12] = track_info_data[24] = (char)((char *)&track_size)[3];
    track_info_data[13] = track_info_data[25] = (char)((char *)&track_size)[2];
    track_info_data[14] = track_info_data[26] = (char)((char *)&track_size)[1];
    track_info_data[15] = track_info_data[27] = (char)((char *)&track_size)[0];

    /*** copy track_info_data[] to DRAM ***/
    Iram_Buf_dram_addr = TRACK_INFO_ADR;

    for(i=0;i<TRACK_INFO_LEN_BYTE;i++)
    {
        WriteSDRAM(track_info_data[i]);
    }

    /*** now send the data back to the host.***/
    SendParameterDataToHost(TRACK_INFO_ADR, track_info_len , TRUE);     // Send parameters and Command Complete.
}


/*--------------------------------------------------------------------
  Name: CheckIfDiscIsCDText()
  Description:
    Verifies if track number provided in the CDB is valid
  Inputs:
    tno - track number to verify
  Outputs:
    none
  Returns:
    TRUE - valid track number
    FALSE - invalid track number
--------------------------------------------------------------------*/
BOOL CheckIfDiscIsCDText(BYTE tno)
{
    BYTE LowestRecordedTno, HighestRecordedTno;

    LowestRecordedTno  = pdbGetLowestRecordedTrackNumber();
    HighestRecordedTno = pdbGetHighestRecordedTrkNumber();

    if ((pdbGetDiscStatus() == eFinalizedDisc) && ((tno == 0xFF) || (tno == HighestRecordedTno+1))){
        BUILD_SENSE(0x05,0x21,0x00,0xa3);                                    /*LOGICAL BLOCK ADDRESS OUT OF RANGE */
        AutoCommandComplete();
        return FALSE;
    }

    if(HighestRecordedTno == 99)
    {
        if ( (tno == (HighestRecordedTno+1)) || (tno == 0xFF))
        {
            BUILD_SENSE(0x05,0x63,0x00,0xa4);                                    /* END OF USER AREA ENCOUNTER ON THIS TRACK */
            AutoCommandComplete();
            return FALSE;
        }
    }

    /* CD-i */
    if ((pdbGetDiscStatus() == eFinalizedDisc) && (pdbGetDiscModeType(0) == eCDIMode) && (LowestRecordedTno > 1) ){
        if ((tno != 0x01) && ((St_cdi_audio == FALSE) || (tno < LowestRecordedTno))){
            BUILD_SENSE(0x05,0x21,0x00,0xa5);                                /* LOGICAL BLOCK ADDRESS OUT OF RANGE */
            AutoCommandComplete();
            return FALSE;
        }

    /* Not CD-i */
    }else{
        if (((tno == 0) || (tno < LowestRecordedTno) || (tno > HighestRecordedTno+1)) && (tno != 0xFF)){
            BUILD_SENSE(0x05,0x21,0x00,0xa6);                                /* LOGICAL BLOCK ADDRESS OUT OF RANGE */
            AutoCommandComplete();
            return FALSE;
        }
    }


    return TRUE;
}


#if 0
/* DVD disc read track info */
void dvdReadTrackInfo(void)
{
    BYTE tno, last_tno, i;
    ULONG target_lba, start_lba, stop_lba, track_size;


    if((Iram_Cmd_block.bf[1] & 0x03) == 0x00)
    {
        target_lba = (long)(PacketCommandToLBALEN4(2));

        /*--------------------------------------------------------------
            last track number
            DVD-R/RW disc Maximum last track no = 2
        ---------------------------------------------------------------*/
        if(((dvdDisc.Type == DVD_MINUS_R) ||(dvdDisc.Type == DVD_MINUS_RW))&&(DVDSessionNum > 1))
            last_tno = 2;
        else
            last_tno = DVDSessionNum;           /* last track number   */

        for (tno = 1; tno <= last_tno; tno++)
        {
            if(dvdDisc.Type == DVD_ROM)
            {
                start_lba = 0;
                stop_lba = dvdDisc.MaxLBA;
            }
            else
            {
                /*--------------------------------------------------------------
                    DVD-R/RW Disc
                        return first track info & last track info only
                ---------------------------------------------------------------*/
               if(((dvdDisc.Type == DVD_MINUS_R) ||(dvdDisc.Type == DVD_MINUS_RW))&&(tno > 1))
               {
                    start_lba = dvdSession.MultiState[DVDSessionNum - 1].StartPSN - 0x30000;/* start lba of track  */
                    stop_lba = dvdSession.MultiState[DVDSessionNum- 1].EndPSN -0x30000 ;    /* stop lba of track   */
                }
               else
               {
                    start_lba = dvdSession.MultiState[tno - 1].StartPSN - 0x30000;/* start lba of track  */
                    stop_lba = dvdSession.MultiState[tno- 1].EndPSN -0x30000 ;    /* stop lba of track   */
                }
                if(start_lba > dvdDisc.LastLBAofLayer0)
                    start_lba = (dvdDisc.LastLBAofLayer0 + 1L) + (dvdSession.MultiState[tno- 1].StartPSN - dvdDisc.StartPSNLayer1);
                if(stop_lba > dvdDisc.LastLBAofLayer0)
                    stop_lba = (dvdDisc.LastLBAofLayer0 + 1L) + (dvdSession.MultiState[tno- 1].EndPSN - dvdDisc.StartPSNLayer1);
            }

            track_size = stop_lba - start_lba + 1L;

            if((start_lba <= target_lba) && (stop_lba >= target_lba))
                break;
        }

        if(tno > last_tno)
        {
            BUILD_SENSE(0x05,0x21,0x00,0xa7);                          /* LOGICAL BLOCK ADDRESS OUT OF RANGE */
            AutoCommandComplete();
            return;
        }

    }
    else if((Iram_Cmd_block.bf[1] & 0x03) == 0x01)
    {
        tno = Iram_Cmd_block.bf[5];                                         /* Track number         */

        if ((tno > DVDSessionNum)||tno == 0x00)
        {
            BUILD_SENSE(0x05,0x26,0x00,0xa8);                                /* INVALID FIELD IN PARAMETER LIST  */
            AutoCommandComplete();
            return;
        }

        if(dvdDisc.Type == DVD_ROM)
        {
            start_lba = 0;
            stop_lba = dvdDisc.MaxLBA;
        }
        else
        {
                /*--------------------------------------------------------------
                    DVD-R/RW Disc
                        return first track info & last track info only
                ---------------------------------------------------------------*/
               if(((dvdDisc.Type == DVD_MINUS_R) ||(dvdDisc.Type == DVD_MINUS_RW))&&(tno > 1))
               {
                    start_lba = dvdSession.MultiState[DVDSessionNum - 1].StartPSN - 0x30000;/* start lba of track  */
                    stop_lba = dvdSession.MultiState[DVDSessionNum- 1].EndPSN -0x30000 ;    /* stop lba of track   */
                }
               else
               {
                    start_lba = dvdSession.MultiState[tno - 1].StartPSN - 0x30000;/* start lba of track  */
                    stop_lba = dvdSession.MultiState[tno- 1].EndPSN -0x30000 ;    /* stop lba of track   */
                }
                if(start_lba > dvdDisc.LastLBAofLayer0)
                    start_lba = (dvdDisc.LastLBAofLayer0 + 1L) + (dvdSession.MultiState[tno- 1].StartPSN - dvdDisc.StartPSNLayer1);
                if(stop_lba > dvdDisc.LastLBAofLayer0)
                    stop_lba = (dvdDisc.LastLBAofLayer0 + 1L) + (dvdSession.MultiState[tno- 1].EndPSN - dvdDisc.StartPSNLayer1);
        }

        track_size = stop_lba - start_lba + 1L;

    }
    else
    {
            BUILD_SENSE(0x05,0x26,0x00,0xa9);                                /* INVALID FIELD IN PARAMETER LIST  */
            AutoCommandComplete();
            return;
    }


    track_info_data[0] = 0x00;      /*** Byte[ 0],[ 1] : Data length    ***/
    track_info_data[1] = TRACK_INFO_LEN_BYTE - 2;

    track_info_data[2] = tno;       /*** Byte[ 2] : Track number (LSB)       ***/
    track_info_data[3] = tno;       /*** Byte[ 3] : Session number (LSB)     ***/

    track_info_data[4] = 0x00;     /*** Byte[ 4] : Reserved        ***/

    if((dvdDisc.Type == DVD_PLUS_R)||(dvdDisc.Type == DVD_PLUS_R_DL))  /*** Byte[5] : Track mode        ***/
        track_info_data[5] = 0x07;
    else
        track_info_data[5] = 0x04;

    /* read DVD-R/RW's session&track number correctly */
    if((dvdDisc.Type == DVD_MINUS_R)||(dvdDisc.Type == DVD_MINUS_RW))
    {
        track_info_data[6] = 0x21;  /*** Byte[6] : Data mode        ***/
        track_info_data[7] = 0x02;
    }else{
        track_info_data[6] = 0x01;      /*** Byte[6] : Data mode        ***/
        track_info_data[7] = 0x00;
    }

    /*** Byte[8][9][10][11] : Track Start Address ***/
    track_info_data[8] =  start_lba >> 24;
    track_info_data[9] =  start_lba >> 16;
    track_info_data[10] = start_lba >> 8;
    track_info_data[11] = start_lba;

    /*** Byte[12][13][14][15] : NWA ***/
    track_info_data[12] = 0x00;
    track_info_data[13] = 0x00;
    track_info_data[14] = 0x00;
    track_info_data[15] = 0x00;

    /*** Byte[16][17][18][19] : Free Blocks ***/
    track_info_data[16] = 0x00;
    track_info_data[17] = 0x00;
    track_info_data[18] = 0x00;
    track_info_data[19] = 0x00;

    /*** Byte[20][21][22][23] : Fixed packet Size     ***/
    track_info_data[20] = 0x00;
    track_info_data[21] = 0x00;
    track_info_data[22] = 0x00;
    track_info_data[23] = 0x10;     /* read DVD-R/RW's session&track number correctly */

    /*** Byte[24][25][26][27] : Track Size            ***/
    track_info_data[24] = track_size >> 24;
    track_info_data[25] = track_size >> 16;
    track_info_data[26] = track_size >> 8;
    track_info_data[27] = track_size;

    /*** Byte[28][29][30][31] : Last Recorded Address ***/
    /* read DVD-R/RW's session&track number correctly */
    if((dvdDisc.Type == DVD_MINUS_R)||(dvdDisc.Type == DVD_MINUS_RW))
    {
        track_info_data[28] = stop_lba >> 24;
        track_info_data[29] = stop_lba >> 16;
        track_info_data[30] = stop_lba >> 8;
        track_info_data[31] = stop_lba;
    }else{
        track_info_data[28] = 0x00;
        track_info_data[29] = 0x00;
        track_info_data[30] = 0x00;
        track_info_data[31] = 0x00;
    }

    track_info_data[32] = 0x00;      /*** Byte[32] : Track Number (MSB)***/
    track_info_data[33] = 0x00;      /*** Byte[33] : Session Number (MSB)***/
    track_info_data[34] = 0x00;     /*** Byte[34][35] : Reserved ***/
    track_info_data[35] = 0x00;

    /*** copy track_info_data[] to DRAM ***/
    Iram_Buf_dram_addr = TRACK_INFO_ADR;

#if (CD_MRW_OEM == 1)
    for(i=0;i<track_info_len;i++)
#else /* CD_MRW_OEM */
    for(i=0;i<TRACK_INFO_LEN_BYTE;i++)
#endif /* CD_MRW_OEM */
    {
        WriteSDRAM(track_info_data[i]);
    }

    /*** now send the data back to the host.***/
    SendParameterDataToHost(TRACK_INFO_ADR, track_info_len , TRUE);     // Send parameters and Command Complete.

}
#endif
