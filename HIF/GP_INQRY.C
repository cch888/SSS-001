/****************************************************************************
*                (c) Copyright 2001 - 2005 SUNEXT DESIGN, INC
*                           All Rights Reserved
*
*
* $Revision: 146 $
* $Date: 11/03/28 2:37p $
*
* DESCRIPTION
*   This file contains command handler for information commands
*
* NOTES:
*
**************************** SUNEXT CONFIDENTIAL *******************************/
#include ".\common\globals.h"
#include ".\common\globtype.h"
#include ".\common\chip.h"
#include ".\common\error.h"
#include ".\firmw_id.h"
#include ".\hif\gp_ram.h"
#include ".\hif\gp_mac.h"
#include ".\hif\gp_mdsns.h"
#include ".\hif\at_knl.h"
#include ".\hif\at_util.h"
#include ".\hif\bf_util.h"
#include ".\hif\gp_inqry.h"
#include ".\servo\svo.h"
#include ".\player\plrDb.h"
#include ".\common\constant.h"

#if (EN_LS == 1)
#include ".\LS\LS_hif.h"
#endif // End of (EN_LS == 1)

#if (BD_ENABLE)
#include ".\bd\bdVars.h"
#include ".\bd\bdMacro.h"
#endif


#if (MSG_OUT_MODE == MSG_USBPORT && ENABLE_UART_RXD == 1)
#include ".\common\scomm.h"
#endif

/*******************************************************************
*           Declaration of CD-ROM information tables
********************************************************************/

BYTE const Identify_tbl[] = {
    0xC0,0x85,                                          /* word 00:     000 ~ 001 General configuration bit */
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,            /* word 01-04:  002 ~ 009 */
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,            /* word 05-08:  010 ~ 017 */
    0x00,0x00,                                          /* word 09:     018 ~ 019 */
    FIRMW_ID_SN,                                        /* word 10-19:  020 ~ 039 */
    0x00,0x00,                                          /* word 20:     040 ~ 041 */
    0x00,0x00,                                          /* word 21:     042 ~ 043 */
    0x00,0x00,                                          /* word 22:     044 ~ 045 */
    FIRMW_ID_REVISION,                                  /* word 23-26:  046 ~ 053 Firmware revision */
    FIRMW_ID_MODEL,                                     /* word 27-46:  054 ~ 093 Model number */
    0x00,0x00,                                          /* word 47:     094 ~ 095 */
    0x00,0x00,                                          /* word 48:     096 ~ 097 */
    0x00,0x0F,                                          /* word 49:     098 ~ 099 Capabilities */
    0x00,ID_WORD50_HI,                                  /* word 50:     100 ~ 101 Capabilities */
    0x00,ID_WORD51_HI,                                  /* word 51:     102 ~ 103 PIO cycle timing */
    0x00,ID_WORD52_HI,                                  /* word 52:     104 ~ 105 DMA cycle timing */
    ID_WORD53_LO,0x00,                                  /* word 53:     106 ~ 107 Field validity */
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,            /* word 54-57:  108 ~ 115 */
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,            /* word 58-61:  116 ~ 123 */
    ID_WORD62_LO,ID_WORD62_HI,                          /* word 62:     124 ~ 125 Singleword DMA transfer mode */
    ID_WORD63_LO,ID_WORD63_HI,                          /* word 63:     126 ~ 127 Multiword DMA transfer mode */
    ID_WORD64_LO,0x000,                                 /* word 64:     128 ~ 129 Advanced PIO transfer mode */
    ID_WORD65_LO,ID_WORD65_HI,                          /* word 65:     130 ~ 131 Min. Multiword DMA cycle timing */
    ID_WORD66_LO,ID_WORD66_HI,                          /* word 66:     132 ~ 133 Manufacture recommended Multiword DMA cycle timing */
    ID_WORD67_LO,ID_WORD67_HI,                          /* word 67:     134 ~ 135 Min. PIO cycle timing w/o flow control */
    ID_WORD68_LO,ID_WORD68_HI,                          /* word 68:     136 ~ 137 Min. PIO cycle timing w/ IORDY flow control */
    0x00,0x00,                                          /* word 69:     138 ~ 139 */
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,            /* word 70-73:  140 ~ 147 */
#if (PCB == 710)||(PCB == 711)||(PCB == 817)
    0x00,0x00,0x00,0x00,ID_WORD76_LO,ID_WORD76_HI,0x00,0x00,            /* word 74-77:  148 ~ 155 */
    ID_WORD78_LO,ID_WORD78_HI,ID_WORD79_LO,ID_WORD79_HI,                                /* word 78-79:  156 ~ 159 SATA support*/
#else
    0x00,0x00,0x00,0x00,0x02,0x02,0x00,0x00,            /* word 74-77:  148 ~ 155 */
    0x48,0x00,0x40,0x00,                                /* word 78-79:  156 ~ 159 SATA support*/
#endif
    ID_WORD80_LO,ID_WORD80_HI,                          /* word 80:     160 ~ 161 Minor version number */
    ID_WORD81_LO,ID_WORD81_HI,                          /* word 81:     162 ~ 163 Minor version number */
    ID_WORD82_LO,ID_WORD82_HI,ID_WORD83_LO,ID_WORD83_HI,/* word 82-83:  164 ~ 167 Command set supported */
    ID_WORD84_LO,ID_WORD84_HI,                          /* word 84:     168 ~ 169 Command set/feature supported extension */
    ID_WORD85_LO,ID_WORD85_HI,ID_WORD86_LO,ID_WORD86_HI,/* word 85-86:  170 ~ 173 Command set/feature enabled */
    ID_WORD87_LO,ID_WORD87_HI,                          /* word 87:     174 ~ 175 Command set/feature default */
    ID_WORD88_LO,ID_WORD88_HI,                          /* word 88:     176 ~ 177 Ultra DMA transfer mode */
    0x00,0x00,                                          /* word 89:     178 ~ 179 */
    0x00,0x00,0x00,0x00,0x00,0x00,                      /* word 90-92:  180 ~ 185 */
    0x00,0x00,                                          /* word 93:     186 ~ 187 Hardware reset result */
    ID_WORD94_LO,ID_WORD94_HI,                          /* word 94:     188 ~ 189 Current AAM value */
    0x00,0x00,0x00,0x00,0x00,0x00,                      /* word 96-97:  190 ~ 195 */
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,            /* word 98-101: 196 ~ 203 */
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,            /* word 102-105:204 ~ 211 */
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,            /* word 106-109:212 ~ 219 */
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,            /* word 110-113:220 ~ 227 */
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,            /* word 114-117:228 ~ 235 */
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,            /* word 118-121:236 ~ 243 */
    0x00,0x00,0x00,0x00,0x00,0x00,                      /* word 122-124:244 ~ 249 */
    0x00,0x00,                                          /* word 125:    250 ~ 251 ATAPI BYTE count = 0*/
    0x00,0x00,                                          /* word 126:    252 ~ 253 */
    0x00,0x00,                                          /* word 127:    254 ~ 255 Removable Media Status Notification feature set supprt */
    0x00,0x00,                                          /* word 128:    256 ~ 257 Security status */
    0x00,0x00,                                          /* word 129:    258 ~ 259 */
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,            /* word 130-133:260 ~ 267 */
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,            /* word 134-137:268 ~ 275 */
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,            /* word 138-141:276 ~ 283 */
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,            /* word 142-145:284 ~ 291 */
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,            /* word 146-149:292 ~ 299 */
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,            /* word 150-153:300 ~ 307 */
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,            /* word 154-157:308 ~ 315 */
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,            /* word 158-161:316 ~ 323 */
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,            /* word 162-165:324 ~ 331 */
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,            /* word 166-169:332 ~ 339 */
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,            /* word 170-173:340 ~ 347 */
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,            /* word 174-177:348 ~ 355 */
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,            /* word 178-181:356 ~ 363 */
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,            /* word 182-185:364 ~ 371 */
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,            /* word 186-189:372 ~ 379 */
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,            /* word 190-193:380 ~ 387 */
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,            /* word 194-197:388 ~ 395 */
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,            /* word 198-201:396 ~ 403 */
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,            /* word 202-205:404 ~ 411 */
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,            /* word 206-209:412 ~ 419 */
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,            /* word 210-213:420 ~ 427 */
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,            /* word 214-217:428 ~ 435 */
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,            /* word 218-221:436 ~ 443 */
    //start == IDVER329 testing fail
    #if(PCB == 710)||(PCB == 711)||(PCB == 817)
    ID_WORD222_LO,0x00,0x00,0x00,0x00,0x00,0x00,0x00,            /* word 222-225:444 ~ 451 */
    #else
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,            /* word 222-225:444 ~ 451 */
    #endif
    //end ==
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,            /* word 226-229:452 ~ 459 */
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,            /* word 230-233:460 ~ 467 */
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,            /* word 234-237:468 ~ 475 */
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,            /* word 238-241:476 ~ 483 */
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,            /* word 242-245:484 ~ 491 */
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,            /* word 246-249:492 ~ 499 */
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,            /* word 250-253:500 ~ 507 */
    0x00,0x00,                                          /* word 254:    508 ~ 509 */
    ID_WORD255_LO,0x00                                  /* word 255:    510 ~ 511 Integrity word */
};

BYTE const Inquiry_tbl[]={
    0x05,0x80,0x00,0x33,0x33,0x00,0x00,0x00,            /* 000 ~ 007 */  /* change byte 3 & 4, MMC-5 spec */

    FIRMW_INQ_VENDOR,                                   /* 008 ~ 015 */
    FIRMW_INQ_PRODUCT,                                  /* 016 ~ 031 */
    FIRMW_INQ_REVISION,                                 /* 032 ~ 035 */
    FIRMW_ID_SN                                         /* 036 ~ 055 */
};

BYTE const Request_sense_tbl[]={
    0x70,0x00,0x00,0x00,0x00,0x00,0x00,0x0A,            /* 000 ~ 007*/
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,            /* 008 ~ 015*/
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00             /* 016 ~ 023*/
};
ULONG ErasecurPSN;
/*************************************************************************
* ReadCapacityCommand
*
* Prepare the capacity data and transfer data to host.
* The capacity is read everytime power-on or tray-in, and saved
* at variable Maxlba
*
* Returns:  void
* Params:   none
*************************************************************************/
extern vBYTE rd_method;
void ReadCapacityCommand()
{
    StLongU   TempL;
#if (BD_ENABLE)
    ULONG tmp1, tmp2;
#endif
    ULONG dest, src;

    BYTE      TrackNum, session;
    Iram_Buf_dram_addr = CAPACITY_ADR;      /* save capacity data into dram */

    switch(pdbDiscLoaded())
    {
        case CD_DISC:
                rd_method = CheckTrackWriteMethod(375L) & 0x0F;
                send_msg5SValue(1,"WRMETH",2,rd_method);
                if(rd_method == WR_METHOD_FP)
                {
                    TempL.Long = Maxlba;
                    TempL.Long=((TempL.Long+6)/39)*32-1;
                    Iram_Buf_dram_addr = CAPACITY_ADR;
                    WriteSDRAM(TempL.byte.u);
                    WriteSDRAM(TempL.byte.h);
                    WriteSDRAM(TempL.byte.m);
                    WriteSDRAM(TempL.byte.l);

                }
                //start == read capacity fail of command set
                else if(rd_method == WR_METHOD_TAO)
                {
                    TempL.Long = Maxlba -2;
                    Iram_Buf_dram_addr = CAPACITY_ADR;
                    WriteSDRAM(TempL.byte.u);
                    WriteSDRAM(TempL.byte.h);
                    WriteSDRAM(TempL.byte.m);
                    WriteSDRAM(TempL.byte.l);
                }
                //end == read capacity fail of command set
                else
                {
                    src = MAXLBA_OFFSET;
                    dest = CAPACITY_ADR;
                    CopySDRAMData((ULONG *)&dest,(ULONG *)&src, 4);
                }
                break;
        case DVD_DISC:
        #if (DVD_RAM_READ == 1)
            if (pdbGetDVDType() == DVD_RAM) /* DVD_RAM  */
            {
                TempL.Long = GetDVDRAMMaxLBA(); //This valu should come from SDL - NumberOfLSN
                Iram_Buf_dram_addr = CAPACITY_ADR;
                WriteSDRAM(TempL.byte.u);
                WriteSDRAM(TempL.byte.h);
                WriteSDRAM(TempL.byte.m);
                WriteSDRAM(TempL.byte.l);
            }
            else
        #endif
            {
                //start == Correct read capacity data.
                if ((pdbGetDVDType() == DVD_MINUS_RW)||(pdbGetDVDType() == DVD_MINUS_R))
                {
                    send_msg5SValue(1,"DiscStatus",4,Info.Media.DVD.RMAData.RMD.Format2.DiscStatus);
                    if(((Info.Media.DVD.RMAData.RMD.Format2.DiscStatus !=  RestricedOverWrite)&&(pdbGetDVDType() == DVD_MINUS_RW))||(pdbGetDVDType() == DVD_MINUS_R))
                    {
                        if (St_blank_disc)
                        {
                            TempL.Long = 0x00;
                        }
                        else
                        {
                            if (pdbGetDVDLayers() == SINGLE_LAYER)
                            {
                                TempL.Long = 0;

                                session = pdbGetDVDFinalSessionNumber();

                                for(TrackNum = pdbGetDVDHighestRecordedTrackNumber(); TrackNum >= 1; TrackNum--)
                                {
                                    if(pdbGetDVDTrackSessionNumber(TrackNum) == session)
                                    {
                                        TempL.Long = dvdConvertPSNtoLBA(pdbGetDVDTrackEndPSN(TrackNum));
                                        break;
                                    }
                                }
                            }
                            else
                            {
                                if (pdbGetDVDRMADiscStatus() == DiscAtOnce)
                                {
                                    TrackNum    = pdbGetDVDHighestRecordedTrackNumber();
                                    TempL.Long  = dvdConvertPSNtoLBA(pdbGetDVDTrackEndPSN(TrackNum));
                                }
                                else
                                    TempL.Long = pdbGetDVDMaxLBA();
                            }
                        }

                        WriteSDRAM(TempL.byte.u);
                        WriteSDRAM(TempL.byte.h);
                        WriteSDRAM(TempL.byte.m);
                        WriteSDRAM(TempL.byte.l);
                    }
                    else
                    {
                        src = MAXLBA_OFFSET;
                        dest = CAPACITY_ADR;
                        CopySDRAMData((ULONG *)&dest,(ULONG *)&src, 4);
                    }
                }
                else if((pdbGetDVDType() == DVD_PLUS_RW)||(pdbGetDVDType() == DVD_PLUS_R)||(pdbGetDVDType() == DVD_PLUS_R_DUAL))//20090309_i.23==start== Correct DVDPR DL disc capacity.
                {
                    if(pdbGetDVDType() == DVD_PLUS_RW && pdbGetDVDFinalSessCloseInfo() != eEmptySession)
                    {
                        TempL.Long = DVD_PLUSRW_DATAZONE_SIZE - 1;
                        //start == Show incorrect capacity
                        if(pdbGetDVDTrackNextWritableAddress(1) != 0)
                        {
                            TempL.Long = pdbGetDVDTrackNextWritableAddress(1) - 1;
                        }
                        //end ==
                    }
                    else if(pdbGetDVDType() == DVD_PLUS_RW_DUAL && pdbGetDVDFinalSessCloseInfo() != eEmptySession)
                    {
                        TempL.Long = 2 * (DVD_PLUSRW_DATAZONE_END_L0 - DVD_START_OF_DATA_ZONE + 1) - 1;
                    }
                    else
                    {
                        TempL.Long = 0;
                        if((pdbGetDVDFinalSessCloseInfo() == eEmptySession) || (pdbGetDVDFinalSessCloseInfo() == eNotSessionClosed))
                            session = pdbGetDVDFinalSessionNumber() - 1;
                        else
                            session = pdbGetDVDFinalSessionNumber();

                        for(TrackNum = pdbGetDVDHighestRecordedTrackNumber(); TrackNum >= 1; TrackNum--)
                        {
                            if(pdbGetDVDTrackSessionNumber(TrackNum) == session)
                            {
                                TempL.Long = dvdConvertPSNtoLBA(pdbGetDVDTrackEndPSN(TrackNum));
                                break;
                            }
                        }
                    }

                    Iram_Buf_dram_addr = CAPACITY_ADR;
                    WriteSDRAM(TempL.byte.u);
                    WriteSDRAM(TempL.byte.h);
                    WriteSDRAM(TempL.byte.m);
                    WriteSDRAM(TempL.byte.l);

                }
                else    /* DVD_ROM */
                {
                    //start=Fixed DVD PTP DISC capacity issue from c.23 item2.
                    TrackNum = pdbGetDVDHighestRecordedTrackNumber();
                    if(pdbGetDVDFinalSessCloseInfo() == eNotSessionClosed)
                        TrackNum -= 1;
                    TempL.Long = dvdConvertPSNtoLBA(pdbGetDVDTrackEndPSN(TrackNum));
                    if ((pdbGetDVDLayers() == DUAL_LAYER) && (pdbGetDVDTrackPath() == DVDHD_PTP_DISC))
                    {
                        TempL.Long = dvdConvertPSNtoLBA(pdbGetDVDMaxPSN(0))+dvdConvertPSNtoLBA((pdbGetDVDMaxPSN(1)))+1;
                    }
                    Iram_Buf_dram_addr = CAPACITY_ADR;
                    WriteSDRAM(TempL.byte.u);
                    WriteSDRAM(TempL.byte.h);
                    WriteSDRAM(TempL.byte.m);
                    WriteSDRAM(TempL.byte.l);
                    //end=
                }
                //end ==
            }
            break;

#if (BD_ENABLE)
        case BD_DISC:
            switch(pdbGetDiscKind())
            {
                case eBDROMDisc:
                    tmp1 = Info.Media.BD.DiscInfoP_L0_Fmt1->Disc.Bdrom.LAUOfDataZone;
                    TempL.Long = SWAPEND_ULONG(tmp1) + 1L;
                    tmp1 = Info.Media.BD.DiscInfoP_L0_Fmt1->Disc.Bdrom.FAUOfDataZone;
                    TempL.Long -= SWAPEND_ULONG(tmp1);
                    break;

                case eBDROMDLDisc:
                    tmp1 = Info.Media.BD.DiscInfoP_L0_Fmt1->Disc.Bdrom.LAUOfDataZone;
                    TempL.Long = SWAPEND_ULONG(tmp1) + 1L;
                    tmp1 = Info.Media.BD.DiscInfoP_L0_Fmt1->Disc.Bdrom.FAUOfDataZone;
                    TempL.Long -= SWAPEND_ULONG(tmp1);

                    tmp1 = Info.Media.BD.DiscInfoP_L1_Fmt1->Disc.Bdrom.LAUOfDataZone;
                    tmp2 = SWAPEND_ULONG(tmp1) + 1L;
                    tmp1 = Info.Media.BD.DiscInfoP_L1_Fmt1->Disc.Bdrom.FAUOfDataZone;
                    tmp2 -= SWAPEND_ULONG(tmp1);
                    TempL.Long += tmp2;
                    TempL.Long += 1L;
                    break;

                case eBDREDisc:
                case eBDREDLDisc:
                    //return last recordable LBA from DDS
                    TempL.Long = pdbGetBDEndLSN();
                    break;

                case eBDRDisc:
                case eBDRDLDisc:
                    if(St_blank_disc == TRUE)
                        TempL.Long = 0x00;
                    else
                    {
                        if(pdbGetBDRecMode() == SRM_NO_LOW)
                        {
                            BYTE track;

                            switch(pdbGetBDFinalSessCloseInfo())
                            {
                                case eSessionFinalized:     // 0000 1110 = Complete Session, Finalized/Complete Disc
                                case eSessionClosed:        // 0000 1101 = Complete Session, Incomplete Disc
                                    track = pdbGetBDFinalSessLastTrackNumber();
                                    TempL.Long = bdConvertPSNtoLBA(pdbGetBDTrackRecordedPSN(track));
                                    break;
                                case eNotSessionClosed:     // 0000 0101 = Incomplete Session, Incomplete Disc
                                case eEmptySession:         // Empty Disc... further checking required
                                    track = pdbGetBDFinalSessLastTrackNumber();
                                    if(track!=1)
                                        TempL.Long = bdConvertPSNtoLBA(pdbGetBDTrackRecordedPSN(track-1));
                                    else
                                        TempL.Long = pdbGetBDEndLSN();
                                    break;
                            }
                        }
                        else
                            TempL.Long = pdbGetBDEndLSN();
                    }
                    break;
            }

            Iram_Buf_dram_addr = CAPACITY_ADR;
            WriteSDRAM(TempL.byte.u);
            WriteSDRAM(TempL.byte.h);
            WriteSDRAM(TempL.byte.m);
            WriteSDRAM(TempL.byte.l);
            break;
#endif
#if (ENABLE_HDDVD)
        case HD_DISC:
            src = MAXLBA_OFFSET;
            dest = CAPACITY_ADR;
            CopySDRAMData((ULONG *)&dest,(ULONG *)&src, 4);
            break;
#endif
        default:
            break;
    }

    /* write block length in bytes (800h) */
    WriteSDRAM(0x00);
    WriteSDRAM(0x00);
    WriteSDRAM(0x08);
    WriteSDRAM(0x00);

    Iram_Linear_Xfer_addr = CAPACITY_ADR;
    Iram_shATB  = CAPACITY_LEN_BYTE;
    CheckDataTransferLimit(Iram_shATB); /* check if transfer data in burst mode */

    Knl_send_compl = TRUE;
    Iram_Buf_hxfer_blk = 1;

    if(Ata_sub_burst == TRUE)
    {
        Send_pardata_piece_by_piece();
    }

    St_send_pardata = TRUE;
    Knl_Atapi_Avail = TRUE;
    TransferToHost();
#if(MSG_OUT_MODE == MSG_USBPORT && ENABLE_UART_RXD == 1)
    if (s2b_atapi_cmd_received == TRUE)
    {
        Knl_Atapi_Avail = FALSE;
        BUILD_SENSE(0x00,0x00,0x00,0x16);    // NO ADDITIONAL SENSE INFO
        AutoCommandComplete();
    }
#endif
}


/*************************************************************************
* InquiryCommand
*
* Inquiry command handler
* (in interrupt)
*
* Returns:  void
* Params:   none
*************************************************************************/
void InquiryCommand(void)
{
    BYTE    allocLength;

    /* check allocation length, return command completed if allocation length
    equal zero */
    if ((Iram_Cmd_block.bf[4] & 0xfe) == 0) /* if allocation length=0 */
    {
        AutoCommandComplete();
        return;
    }

    /* setup registers to return standard inquiry data to host */
    Iram_Linear_Xfer_addr = INQUIRY_ADR;
    Iram_shATB  = INQUIRY_LEN_BYTE;

    /* round down if allocation length is odd */
    allocLength = Iram_Cmd_block.bf[4];

    //For WHQL Optical Logo test, Drive serial number test error.
    //SATA drive should not truncate transfer length for Inquiry command
    //if ( allocLength & 0x01)
    //allocLength--;

    Iram_shATB = MINIMUM( INQUIRY_LEN_BYTE, (USHORT)allocLength);  // Number of bytes to xfer. Compare, return minimum.
                                        /* chk allocation & actual data len */
    CheckDataTransferLimit(Iram_shATB); /* check if transfer data in burst mode */

    Knl_send_compl = TRUE;

    Iram_Buf_hxfer_blk = 1;
                                        /* setup H/W xfr */
    if(Ata_sub_burst == TRUE)
    {
        Send_pardata_piece_by_piece();
    }

    St_send_pardata = TRUE;
    Knl_Atapi_Avail = TRUE;
    TransferToHost();

#if(MSG_OUT_MODE == MSG_USBPORT && ENABLE_UART_RXD == 1)
    if (s2b_atapi_cmd_received == TRUE)
    {
        Knl_Atapi_Avail = FALSE;
        BUILD_SENSE(0x00,0x00,0x00,0x16);    // NO ADDITIONAL SENSE INFO
        AutoCommandComplete();
    }
#endif

}


/*************************************************************************
* VendorATAIdentifyPacketDeviceCommand
*
* ATA Identify command handler
* (in interrupt)
*
* Returns:  void
* Params:   none
*************************************************************************/
void VendorATAIdentifyPacketDeviceCommand()
{
	UINT i;
	BYTE checksum = 0;

	send_msg80(SEND_ATAPI_CMD,0x710302);
	Iram_Buf_dram_addr = IDENTIFY_ADR;
	for(i=0; i<IDENTIFY_LEN_BYTE-1; i++)/* byte 0 ~ byte 510 */
		checksum += SCSDRAM[Iram_Buf_dram_addr+i];

	checksum = (~checksum) + 1;
	SCSDRAM[IDENTIFY_ADR+IDENTIFY_LEN_BYTE-1] = checksum;

	SendParameterDataToHost(IDENTIFY_ADR, IDENTIFY_LEN_BYTE , FALSE);
}


/*************************************************************************
* ataIdentifyPacketDeviceCommand
*
* ATA Identify command handler
* (in interrupt)
*
* Returns:  void
* Params:   none
*************************************************************************/
void ataIdentifyPacketDeviceCommand()
{
    St_dma_mode = FALSE;

    Iram_shHDDir &= 0xF3;                   /* set to PIO mode */

    WRITE_REG(HAL_HSTREG_MAP->HDDIR,Iram_shHDDir);  /* multiple bits changed/removed/added */

    Ata_sub_burst = FALSE;                  /* always not 2k case */
    Iram_Linear_Xfer_addr = IDENTIFY_ADR;
    Iram_shATB  = IDENTIFY_LEN_BYTE;        /* xfer length BYTE count */
    St_send_pardata = TRUE;                 /* mark wait transfer done */
    Iram_Buf_hxfer_blk = 1;                 /* force as single block */
    Knl_IdentAvail = TRUE;
    {
        UINT i;
        BYTE checksum = 0;
        Iram_Buf_dram_addr = IDENTIFY_ADR;
        for(i=0; i<IDENTIFY_LEN_BYTE-1; i++)/* byte 0 ~ byte 510 */
            checksum += SCSDRAM[Iram_Buf_dram_addr+i];

        checksum = (~checksum) + 1;
        SCSDRAM[IDENTIFY_ADR+IDENTIFY_LEN_BYTE-1] = checksum;
    }
    TransferToHost();                       /* sent data to host */
}


/*************************************************************************
* RequestSenseCommand()
*
* Request Sense command handler
*
* Returns:  void
* Params:   none
*************************************************************************/
void    RequestSenseCommand()
{
    USHORT  XferLength;
    USHORT  progressResult;
    BYTE    bSKSV;
    if(St_unit_atten == TRUE)
    {
        BUILD_SENSE(0x06,0x29,0x00,0x04);
        St_unit_atten = FALSE;
        st_disc_change = FALSE;
    }

    bSKSV = 0x00;
    progressResult = 0;

    /* check allocation length in command BYTE 4, return command completed
        if allocation length equal zero */

    if((Iram_Cmd_block.bf[4] & 0xfe) == 0)
    {       /* if allocation length = 0 */
        BUILD_SENSE(0x00,0x00,0x00,0x08);            /* init sense key */
        AutoCommandComplete();
        return;
    }

#if (EN_LS == 1)
    if (pdbGetDiscKind() == eLSDisc)
    {
        LightScribePrintDataFormat_T pdf;

        /* fill the information field */
        Iram_Buf_dram_addr = REQ_SENSE_ADR + 0x03;

        if (LSRecordInfo.State != eRECORD_IDLE)
            bSKSV = 0x80;
        else
            bSKSV = 0x00;

        LSGetPrintDataFormat(&pdf, LSRecordInfo.CurrentRecordablePage);
        WriteSDRAM(bSKSV);
        WriteSDRAM((BYTE)(pdf.TrackNumber>>8));
        WriteSDRAM((BYTE)pdf.TrackNumber);

        /* fill the sense key specific information field */
        bSKSV = 0;
        progressResult =0;
    }
#endif // End of (EN_LS == 1)
    Iram_Buf_dram_addr = REQ_SENSE_ADR + 0x0F;
    WriteSDRAM( bSKSV );
    WriteSDRAM( BHIGH( progressResult ) );
    WriteSDRAM( BLOW ( progressResult ) );

    /* Treat the disc only has one session and no session closed as unknown disc.*/
    if(St_no_disc == FALSE)
    {
        if (pdbDiscLoaded() == CD_DISC)
        {
            if ((pdbServoGetDiscDiameter() != eNoDisc) && ( pdbGetSessionCounter()== 1) &&
                (pdbGetFinalSessCloseInfo() < eSessionClosed) && ( St_blank_disc != TRUE))
            {
                Iram_Sense.key = 0x03;  /* Unknow Format */
                Iram_Sense.cod = 0x30;
                Iram_Sense.qul = 0x01;
            }
        }
    }
#if (SUPPORT_DVD_WRITE_ABILITY != ALL_DISC)
        if ((pdbDiscLoaded() == DVD_DISC)&&(St_become_ready == FALSE))
        {
    #if (SUPPORT_DVD_WRITE_ABILITY == P_SL_DISC)
            if ((pdbServoGetDiscDiameter() != eNoDisc) &&(pdbServoGetDiscType() == eLPPDisc)&&
                ( St_blank_disc == TRUE))
    #elif (SUPPORT_DVD_WRITE_ABILITY == PM_SL_DISC)
            if ((pdbServoGetDiscDiameter() != eNoDisc) &&(pdbGetDVDLayers() == DUAL_LAYER)&&
                ( St_blank_disc == TRUE))
    #elif (SUPPORT_DVD_WRITE_ABILITY == P_DL_DISC)
            if ((pdbServoGetDiscDiameter() != eNoDisc) &&(pdbGetDiscKind()== eDVDMinusRDLDisc)&&
                ( St_blank_disc == TRUE))
    #endif
            {
                Iram_Sense.key = 0x03;  /* Unknow Format */
                Iram_Sense.cod = 0x30;
                Iram_Sense.qul = 0x01;
            }
        }
    #endif

    /* Write the request sense info into dram and setup registers to
        transfer data to host */

    Iram_Buf_dram_addr = REQ_SENSE_ADR +2;      /* adjust to sense key address */
    WriteSDRAM(Iram_Sense.key);                    /* write sense key data to SDRAM */

    Iram_Buf_dram_addr = REQ_SENSE_ADR + 0x0C;  /* adjust to sense code dram address */
    WriteSDRAM(Iram_Sense.cod);                    /* write sense code to SDRAM */

    Iram_Buf_dram_addr = REQ_SENSE_ADR + 0x0D;  /* adjust to sense qualifier SDRAM address */
    WriteSDRAM(Iram_Sense.qul);                    /* write sense code to SDRAM */

    XferLength = REQ_SENSE_LEN_BYTE;

    XferLength = MINIMUM( REQ_SENSE_LEN_BYTE, (USHORT)Iram_Cmd_block.bf[4]);  // Number of bytes to xfer. Compare, return minimum.

    //send_msg54(SEND_ATAPI_CMD, 0xee, Iram_Sense.key, Iram_Sense.cod, Iram_Sense.qul);

    BUILD_SENSE(0x00,0x00,0x00,0x09);            /* init sense key */

    Iram_Linear_Xfer_addr = REQ_SENSE_ADR;
    Iram_shATB  = (USHORT)XferLength;
    CheckDataTransferLimit(Iram_shATB); /* check if transfer data in burst mode */

    Knl_send_compl = TRUE;
    Iram_Buf_hxfer_blk = 1;

    if(Ata_sub_burst == TRUE)
    {
        Send_pardata_piece_by_piece();
    }

    St_send_pardata = TRUE;
    Knl_Atapi_Avail = TRUE;
    TransferToHost();

#if(MSG_OUT_MODE == MSG_USBPORT && ENABLE_UART_RXD == 1)
    if (s2b_atapi_cmd_received == TRUE)
    {
        Knl_Atapi_Avail = FALSE;
        BUILD_SENSE(0x00,0x00,0x00,0x16);    // NO ADDITIONAL SENSE INFO
        AutoCommandComplete();
    }
#endif

#if (EN_LS == 1)
    if (LSRecordInfo.LS_Valid_bit)
    {
            Iram_Buf_dram_addr = REQ_SENSE_ADR;
            WriteSDRAM(0x70);
    }
#endif
}


/*------------------------------------------------------------------------*/
/* The end of file                                                        */
/*------------------------------------------------------------------------*/
