/****************************************************************************
*                (c) Copyright 2001 - 2008 SUNEXT DESIGN, INC.
*                           All Right Reserved
*
*
* File name    : App_IF.C
*
* Description  : This module contains the functions used to interface
*                to the PLAYER interface's Player Database (SRAM).
*
* NOTES:
*
* $Revision: 32 $
* $Date: 11/03/28 2:37p $
*
**************************** SUNEXT CONFIDENTIAL *************************/

#include ".\common\globals.h"
#include ".\common\globtype.h"
#include ".\common\chip.h"
#include ".\common\error.h"
#include ".\common\commath.h"

#include ".\servo\svo.h"    /* This should be the only servo include file */
#include ".\player\plrdb.h"
#include ".\player\plrServo.h"
#include ".\player\plrinfo.h"
#include ".\common\constant.h"
#include ".\hif\mmc.h"
#include ".\hif\gp_mac.h"
#include ".\hif\app_if.h"
#include ".\hif\bf_util.h"

extern void SaveTOCToSDRAM(void);
extern void GetTOCFormat2Length(void);

TimeS GetMode1TimeOfLeadIn(BYTE point , BYTE Sess);

#define P_A0    100
#define P_A1    101
#define P_A2    102


/*********************************************************************
* Function:    RequestCatalogNumber()
*
* Description: Retrieves the catalog number.
*
* Returns:     PASS/FAIL
*
* Params:      none
*********************************************************************/
BOOL RequestCatalogNumber(void)
{
BYTE    catalog_no[10];
SHORT   i;

    if(pdbGetCatalogCode(&catalog_no[0]) == TRUE){
        for(i = 0;i < 10;i++){
            Iram_Player_output[i]= catalog_no[i];
        }
        return(PASS);
    }else{
        return(FAIL);
    }
}

/*********************************************************************
* Function:    RequestISRCNumber()
*
* Description: Retrieves the ISRC Number.
*
* Returns:     PASS/FAIL
*
* Params:      none
*********************************************************************/
BOOL RequestISRCNumber(BYTE tno)
{
BYTE    isrc[10];
SHORT   i;
    if(pdbGetISRCCode(&isrc[0]) == HEX2BCD(tno)){
        for(i = 0;i < 10;i++){
            Iram_Player_output[i]= isrc[i];
        }
        return(PASS);
    }else{
        return(FAIL);
    }
}
/*********************************************************************
* Function:    RequestRIDNumber()
*
* Description: Retrieves the RID Number.
*
* Returns:     PASS/FAIL
*
* Params:      none
*********************************************************************/
BOOL RequestRIDNumber(BYTE tno)
{
BYTE    rid[10];
SHORT   i;
    if(pdbGetRIDCode(&rid[0]) == HEX2BCD(tno)){
        for(i = 0;i < 10;i++){
            Iram_Player_output[i]= rid[i];
        }
        return(PASS);
    }else{
        return(FAIL);
    }
}


/*********************************************************************
* Function:    RequestSubQInfo()
*
* Description: Retrieves SubQ info.
*
* Returns:     READY
*
* Params:      none
*********************************************************************/
BOOL RequestSubQInfo(void)
{
SubQCodeS subcd;

    pdbGetSubQCode(&subcd);

    Iram_Player_output[0]= (subcd.Con << 4) | (subcd.Addr);
    Iram_Player_output[1]= subcd.Tno;
    Iram_Player_output[2]= subcd.Point;
    Iram_Player_output[3]= subcd.Minute;
    Iram_Player_output[4]= subcd.Second;
    Iram_Player_output[5]= subcd.Frame;
    Iram_Player_output[6]= subcd.Zero;
    Iram_Player_output[7]= subcd.PMinute;
    Iram_Player_output[8]= subcd.PSecond;
    Iram_Player_output[9]= subcd.PFrame;
    return(READY);
}

/*********************************************************************
* Function:   BuildTOCFormat2()
*
* Description: Builds TOC Format 2
*
* Returns:     none
*
* Params:      none
*********************************************************************/
void BuildTOCFormat2(void)
{
BYTE        total_sess,i,j,k;
TimeS       frame;
TimeS       next_session;
DiscContAxS DiscContAx;
BYTE        ReadMethod;//Variable ReadInfoMethod is disorder BuildTOCFormat2() 
BYTE        tno,start_tno,end_tno;


    St_no_toc_info = FALSE;                 /* No TOC Information available */
    total_sess = pdbGetNumberOfSessionsOnDisc();

/* To protect the disc has the last session exist but without track inside, add this solusion can avoid
     return the empty session to the host.*/
    if (St_cdrom_audio){
        if (pdbGetFirstTrackOfSession(total_sess)>pdbGetHighestRecordedTrkNumber()){
            total_sess = total_sess -1;
        }
    }

    Iram_Buf_dram_addr = TOC_DRAM_HI + NUM_OF_SESS;
    WriteSDRAM(total_sess);

    if(total_sess > 0){
        for(i = 1;i <= total_sess; i++){
    /* point A0 */
            _Nr_of_sessions  = i;
            
            ReadMethod = pdbGetReadInfoMethod(); //Variable ReadInfoMethod is disorder BuildTOCFormat2()
            
            if (ReadMethod == eReadTOC){   //Variable ReadInfoMethod is disorder BuildTOCFormat2()
                DiscContAx = pdbGetDiscContAx(i);
            }else{
                start_tno = pdbGetFirstTrackOfSession(i);
                end_tno   = pdbGetLastTrackOfSession(i);
                DiscContAx.ContA0 = (pdbGetTrackContAddr(start_tno) & 0xF0) >> 4;
                DiscContAx.ContA2 = (pdbGetTrackContAddr(end_tno) & 0xF0) >> 4;
                for (tno = start_tno; tno < end_tno + 1; tno++){
                    DiscContAx.ContA1 = (pdbGetTrackContAddr(tno) & 0xF0) >> 4;
                    if (DiscContAx.ContA0 != DiscContAx.ContA1){
                        DiscContAx.ContA1 = 0x00;
                        break;
                    }
                }
            }

            Iram_Player_output[0] = DiscContAx.ContA0 | (SUBC_MODE_1 << 4);         /* subcd.Con; */
            Iram_Player_output[1] = 0;                                              /* subcd.Tno; */
            Iram_Player_output[2] = 0xA0;                                           /* subcd.Point; */

            Iram_Player_output[3] = 0;                                              /* subcd.Minute; */
            Iram_Player_output[4] = 0;                                              /* subcd.Second; */
            Iram_Player_output[5] = 0;                                              /* subcd.Frame; */
            Iram_Player_output[6] = 0;                                              /* subcd.Zero; */
 
            Iram_Player_output[7] = pdbGetFirstTrackOfSession(i);                   /* subcd.PMinute; */

            if(pdbGetDiscModeType(i) == 0xFF){ //TBD? What is this?
                Iram_Rw_read_mode = eMode1;
                Iram_Player_output[8] = 0x00;
            }
            else
            {

                switch( pdbGetDiscModeType(i) ){
                    case eCDRomCDDA:
                        Iram_Rw_read_mode = eMode1;
                        Iram_Player_output[8] = 0x00;
                        break;
                    case eCDIMode:
                        St_cdi = TRUE;
                        Iram_Rw_read_mode = eMode2Form1;
                        if (Iram_Player_output[7] == 0x01)
                        {
                        pdbSetLowestRecordedTrackNumber(2);
                        pdbSetFinalSessFirstTrackNumber(2);
                        pdbSetFirstTrackInLastSession(2);
                        Iram_Player_output[7] = 0x02;
                        }
                        Iram_Player_output[8] = 0x10;
                        {
                        TimeS tmp_time;

                        tmp_time.Minute = 0;
                        tmp_time.Second = 2;
                        tmp_time.Frame  = 0;
                        pdbSetStartTime(tmp_time, 1);
                        pdbSetTrackContAddr(1,0x41);

                        if (!(pdbGetTrackConAd(Iram_Player_output[7]) & 0x40))
                        St_cdi_audio = TRUE;
                        }
                    break;
                    default: /* case eCDRomXA: */
                        Iram_Rw_read_mode = eMode2Form1;
                        Iram_Player_output[8] = 0x20;
                    break;
                }
            }
            Iram_Player_output[9] = 0;  /* subcd.PFrame; */
            SaveTOCToSDRAM();

    /* point A1 */
            Iram_Player_output[0] = DiscContAx.ContA1 | (SUBC_MODE_1 << 4);         /* subcd.Con; */
            Iram_Player_output[2] = 0xA1;                                           /* subcd.Point; */

            Iram_Player_output[3] = 0;                                              /* subcd.Minute; */
            Iram_Player_output[4] = 0;                                              /* subcd.Second; */
            Iram_Player_output[5] = 0;                                              /* subcd.Frame; */

            Iram_Player_output[7] = pdbGetLastTrackOfSession(i);
            Iram_Player_output[8] = 0;                                              /* subcd.PSecond; */
            Iram_Player_output[9] = 0;                                              /* subcd.PFrame; */
            SaveTOCToSDRAM();

    /* point A2 */
            Iram_Player_output[0] = DiscContAx.ContA2 | (SUBC_MODE_1 << 4);         /* subcd.Con; */
            Iram_Player_output[2] = 0xA2;                                           /* subcd.Point; */

            Iram_Player_output[3] = 0;                                              /* subcd.Minute; */
            Iram_Player_output[4] = 0;                                              /* subcd.Second; */
            Iram_Player_output[5] = 0;                                              /* subcd.Frame; */

            frame = pdbGetSessionLeadOutStart(i);
            Iram_Player_output[7] = frame.Minute;                                   /* subcd.PMinute; */
            Iram_Player_output[8] = frame.Second;                                   /* subcd.PSecond; */
            Iram_Player_output[9] = frame.Frame;                                    /* subcd.PFrame; */

            SaveTOCToSDRAM();

            Iram_Buf_dram_addr = TOC_DRAM_HI + MAX_LEADOUT;
            WriteSDRAM(Iram_Player_output[7]);
            WriteSDRAM(Iram_Player_output[8]);
            WriteSDRAM(Iram_Player_output[9]);

            if(i == 1){

                Iram_Buf_dram_addr = TOC_DRAM_HI + FIRST_LEADOUT;
                WriteSDRAM(Iram_Player_output[7]);
                WriteSDRAM(Iram_Player_output[8]);
                WriteSDRAM(Iram_Player_output[9]);
            }

            j = pdbGetFirstTrackOfSession(i);
            k = pdbGetLastTrackOfSession(i);

        if (((!St_cdi) || (St_cdi_audio == TRUE)) && (j != 0))  /*for cdi_plus_audio disc */
        {
            while(j <= k){
                Iram_Player_output[0] = pdbGetTrackConAd(j) | (SUBC_MODE_1 << 4); /* subcd.Con; */
                if(Iram_Player_output[0] & 0x04){
                    St_cdrom_data = TRUE;
                }else{
                    St_cdrom_audio = TRUE;
                }
                Iram_Player_output[2] = j;              /* subcd.Point; */

                Iram_Player_output[3] = 0;                                              /* subcd.Minute; */
                Iram_Player_output[4] = 0;                                              /* subcd.Second; */
                Iram_Player_output[5] = 0;                                              /* subcd.Frame; */

                frame = pdbGetStartTime(j);
                Iram_Player_output[7] = frame.Minute;   /* subcd.PMinute; */
                Iram_Player_output[8] = frame.Second;   /* subcd.PSecond; */
                Iram_Player_output[9] = frame.Frame;    /* subcd.PFrame; */
                SaveTOCToSDRAM();
                j++;
            } /* while(j<=k) */
            }


            if(((pdbGetDiscStatus() == eFinalizedDisc) && (i < total_sess))
            || ((pdbGetCDRW ()      == TRUE          ) && (pdbGetDiscStatus() == eNonFinalizedDisc))
            || ( pdbGetDiscStatus() == eNonFinalizedDisc) || ((PMANotOk & 0x80) == 0x80)  ){/* PMA Read Failed protect */
        /* point B0 */
                Iram_Player_output[0] = pdbGetTrackConAd(pdbGetLastTrackOfSession(i)) | (SUBC_MODE_5 << 4); /* subcd.Con */
                Iram_Player_output[2] = 0xB0;                                                               /* subcd.Point; */
                if(i == 1){
                    next_session.Minute = 2;
                    next_session.Second = 30;
                    next_session.Frame  = 0;
                    if(pdbGetCDRW() == TRUE){
                        Iram_Player_output[6] = 3;
                    }else{
                        if(pdbGetDiscStatus() == eNonFinalizedDisc){
                            Iram_Player_output[6] = 2;
                        }else{
                            Iram_Player_output[6] = 1;  /* multisession completed disc : no C0 pointer. */
                        }
                    }
                }else{
                    next_session.Minute = 1;
                    next_session.Second = 30;
                    next_session.Frame  = 0;
                    Iram_Player_output[6]    = 1;               /* subcd.Zero; */
                }
                frame = pdbGetSessionLeadOutStart(i);
                mathAddMSFValues(&next_session,&frame,&next_session);
                Iram_Player_output[3] = next_session.Minute;    /* subcd.Minute; */
                Iram_Player_output[4] = next_session.Second;    /* subcd.Second; */
                Iram_Player_output[5] = next_session.Frame;     /* subcd.Frame; */
                frame = pdbGetATIPLeadOut();
                Iram_Player_output[7] = frame.Minute;           /* subcd.PMinute; */
                Iram_Player_output[8] = frame.Second;           /* subcd.PSecond; */
                Iram_Player_output[9] = frame.Frame;            /* subcd.PFrame; */
                SaveTOCToSDRAM();
            }

            if(((pdbGetCDRW() == TRUE) && (i == total_sess) && (pdbGetDiscStatus() == eFinalizedDisc)) ||
            ((i == total_sess) && (fB0valid == TRUE))){
                Iram_Player_output[0] = pdbGetTrackConAd(pdbGetLastTrackOfSession(i)) | (SUBC_MODE_5 << 4); /* subcd.Con */
                Iram_Player_output[2] = 0xB0;                                                               /* subcd.Point; */
                Iram_Player_output[3] = 0xFF;                                                               /* subcd.Minute; */
                Iram_Player_output[4] = 0xFF;                                                               /* subcd.Second; */
                Iram_Player_output[5] = 0xFF;                                                               /* subcd.Frame; */
                if(i == 1){
                    Iram_Player_output[6] = 3;
                }else{
                    Iram_Player_output[6] = 1;
                }

                frame = pdbGetATIPLeadOut();
                Iram_Player_output[7] = frame.Minute;                                                       /* subcd.PMinute; */
                Iram_Player_output[8] = frame.Second;                                                       /* subcd.PSecond; */
                Iram_Player_output[9] = frame.Frame;                                                        /* subcd.PFrame; */
                SaveTOCToSDRAM();
            }

     /* point C0 */
            if((i == 1) && (pdbServoGetDiscType() == eATIPDisc)){
                if((pdbGetCDRW() == TRUE) || (!((pdbGetDiscStatus() == eFinalizedDisc) && (total_sess == 1)))){
                    Iram_Player_output[0] = pdbGetTrackConAd(pdbGetLastTrackOfSession(i)) | (SUBC_MODE_5 << 4); /* subcd.conad; */
                    Iram_Player_output[2] = 0xC0;                                                               /* subcd.Point; */
                    Iram_Player_output[3] = 0;
                    Iram_Player_output[4] = 0;
                    Iram_Player_output[5] = 0x00;
                    Iram_Player_output[6] = 0;
                    Iram_Player_output[7] = 0;                                                       /* subcd.PMinute; */
                    Iram_Player_output[8] = 0;                                                       /* subcd.PSecond; */
                    Iram_Player_output[9] = 0;                                                        /* subcd.PFrame; */
                    SaveTOCToSDRAM();
                }
            }
            else if ((i == 1) && (pdbServoGetDiscType() == eNonATIPDisc) && (pdbGetSubCodeMode5(PointC0,(SubQCodeS *)&Iram_Player_output[0]) == TRUE)){
                SaveTOCToSDRAM();
            }

     /* point C1 */
            if((i == 1) && (pdbGetCDRW() == TRUE)){
                Iram_Player_output[0] = pdbGetTrackConAd(pdbGetLastTrackOfSession(i)) | (SUBC_MODE_5 << 4); /* subcd.Con; */
                Iram_Player_output[2] = 0xC1;                                                               /* subcd.Point; */
                Iram_Player_output[3] = 0;
                Iram_Player_output[4] = 0;
                Iram_Player_output[5] = 0;
                Iram_Player_output[6] = 0;
                Iram_Player_output[7] = 0;                                                                  /* subcd.PMin; */
                Iram_Player_output[8] = 0;                                                                  /* subcd.PSec; */
                Iram_Player_output[9] = 0;                                                                  /* subcd.PFrame; */
                SaveTOCToSDRAM();
            }

        } /* for */

/* Prepare 4 bytes header for format 2 */
        GetTOCFormat2Length();
        WriteSDRAM(1);
        WriteSDRAM(total_sess);

        k = pdbGetLastTrackOfSession(total_sess);

        Iram_Buf_dram_addr = TOC_DRAM_HI + MAX_TRACK;
        WriteSDRAM(k);

    } /* (total_sess > 0) */

/*
 * total_sess=0; the disc either blank or have 1 unclosed session
 *      return if it is blank, otherwise check for data or audio disc.
 */
    else{
        St_no_toc_info = TRUE ;             /* No TOC Information available */
        Toc_lead_out = 0;
        total_sess = pdbGetSessionCounter();
        j = pdbGetFirstTrackOfSession(total_sess);
        if(j == 0){
            return;
        }
        switch( pdbGetDiscModeType(i) ){
            case eCDRomCDDA:
                Iram_Rw_read_mode = eMode1;
                break;
            case eCDIMode:
                St_cdi = TRUE;
                Iram_Rw_read_mode = eMode2Form1;
                return;
            default: /* case eCDRomXA: */
                Iram_Rw_read_mode = eMode2Form1;
                break;
        }
        if(St_cdi == TRUE){
            return;
        }
        k = pdbGetLastTrackOfSession(total_sess);
        while(j <= k){
            Iram_Player_output[0]= pdbGetTrackConAd(j);
            if(Iram_Player_output[0] & 0x04){
                St_cdrom_data = TRUE;
            }else{
                St_cdrom_audio = TRUE;
            }
            j++;
        } /* while (j<=k) */
    }
}

/*********************************************************************
* Function:    GetCapacity()
*
* Description: Retrieves disc capacity.
*
* Returns:     none
*
* Params:      none
*********************************************************************/
void GetCapacity(void)
{
    BYTE closed_sess;
    ULONG lba;

    TimeS   TmpTime,st_time;
    LONG    start_lba,stop_lba,packet_size,packet_num;
    BYTE    Tno,status;
    BOOL    ExcludeRunOut;

    ExcludeRunOut = FALSE;

#if (DVD_RAM_READ == 1)

    if( pdbDiscLoaded() == DVD_DISC )
    {
        if (pdbGetDVDType() == DVD_RAM)
        {
            lba = GetDVDRAMMaxLBA();
            SetMaxLBA( lba, ExcludeRunOut );
            return;
        }
    }
#endif

    closed_sess = pdbGetNumberOfSessionsOnDisc();
    if(closed_sess > 0){
        Tno = pdbGetHighestRecordedTrkNumber();
        if ( Tno == 0) {
            TmpTime = pdbGetATIPLeadOut();
        }else if( (closed_sess > pdbGetSessionCounter()) &&
            (pdbGetDiscStatus() == eNonFinalizedDisc)) {
            TmpTime = pdbGetATIPLeadOut();
        }
        else {
            TmpTime = pdbGetStopTime(pdbGetLastTrackOfSession(closed_sess));

            status = pdbGetTrackDiagStatus(Tno);
            if((status & eTS_Diag_TDB_done) && (status & eTS_Diag_TDB)){
                if (pdbGetTrackWriteMethod(Tno) == 0x91){       /* METHOD FP */

                    st_time  = pdbGetStartTime(Tno);
                    start_lba   = MSF2LBA_Plus(&st_time);
                    stop_lba    = MSF2LBA_Plus(&TmpTime);
                    packet_size = pdbGetTrackPacketSize(Tno);

                    if(packet_size == 0L){
                        packet_size = 32L;
                    }

                    packet_num  = (stop_lba - start_lba) / (packet_size + 7L);
                    stop_lba = stop_lba - (packet_num * 7L);
                    TmpTime = LBA2MSF(stop_lba);
                }
            }
            mathAddAddressToMSF(&TmpTime, 1L, &TmpTime);
        }
        lba   = MSF2Count(&TmpTime)-150-1;
    }else{
        lba = 0;
    }
    SetMaxLBA(lba, ExcludeRunOut);
}

/*********************************************************************
* Function:    ClearMaxLBA()
*
* Description: Clears MAX LBA to 0.
*
* Returns:     void
*
* Params:      none
*********************************************************************/
void ClearMaxLBA(void)
{
    Iram_Buf_dram_addr = TOC_DRAM_HI + DISC_CAPACITY_LBA;
    WriteSDRAM_4B(0);
    SetMaxLBA(0, FALSE);
}

/*********************************************************************
* Function:    WriteSDRAM_4B()
*
* Description: Writes a DWord to SDRAM.
*
* Returns:     void
*
* Params:      DWord Value to write,
*              Iram_Buf_dram_addr contains address
*********************************************************************/
void WriteSDRAM_4B(ULONG l)
{
    WriteSDRAM((BYTE)(l >> 24));
    WriteSDRAM((BYTE)((l >> 16)&0xff));
    WriteSDRAM((BYTE)((l >> 8)&0xff));
    WriteSDRAM((BYTE)((l)&0xff));
}

/*********************************************************************
* Function:    SetMaxLBA()
*
* Description: Writes a DWord to SDRAM.
*
* Returns:     void
*
* Params:      Max LBA - Maximum LBA
*              Flag - if true then add 2 to Maximum LBA of finalilzed disc
*********************************************************************/
void SetMaxLBA(long max_lba, BOOL flag)
{
TimeS time;
TimeS time_NextSessStart,time_sub;
BYTE sign;
BYTE tno;

    Iram_Buf_dram_addr = MAXLBA_OFFSET;
    WriteSDRAM_4B(max_lba);

    if (pdbGetDiscStatus() == eFinalizedDisc){
        tno = pdbGetHighestRecordedTrkNumber();
        if((pdbGetTrackContAddr(tno) & 0x40) == 0x40){
            if(flag == TRUE){
                Maxlba = max_lba + 2;
            }else{
                Maxlba = max_lba;
            }
        }else{
            Maxlba = max_lba;   //Maxlba = max_lba + 2;
        }
    }else{
        time = pdbGetATIPLeadOut();
        time_NextSessStart = pdbGetNextSessStartTime();
        sign = mathSubtractMSFValues(&time,&time_NextSessStart,&time_sub);
        if(sign!=eMinus){
            Maxlba  = MSF2LBA_Plus(&time);
            Maxlba += 6750L;
        }else{
            Maxlba  = MSF2LBA_Plus(&time_NextSessStart);
        }
    }

}

