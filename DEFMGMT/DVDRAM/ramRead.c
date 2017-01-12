/****************************************************************************
*            (c) Copyright 2001 - 2010 Sunext Technology Co., Ltd.
*                           All Right Reserved
*
* FILENAME:   mrw_read.c
*
*  $Revision: 2 $
*  $Date: 10/02/21 11:36a $
*
* DESCRIPTION
*   This file contains read utility for DVD-RAM
*
* Notes:
*
**************************** SUNEXT CONFIDENTIAL ****************************/

#include ".\common\globals.h"
#include ".\common\globtype.h"
#include ".\common\sdram.h"
#include ".\common\com_im.h"
#include ".\hif\gp_mac.h"
#include ".\hif\rd_util.h"
#include ".\hif\sv_util.h"
#include ".\dvd\dvMacros.h"
#include ".\dvd\dvisr.h"
#include ".\dvd\dvGvars.h"
#include ".\dvd\dvseek.h"
#include ".\defmgmt\dvdram\ramread.h"

#if (DVD_RAM_READ == 1)
#include ".\defmgmt\dvdram\dvDefMgr.h"
#include ".\defmgmt\dvdram\ramdef.h"
#include ".\defmgmt\common\mrw_ram.h"
#include ".\dvd\dvHostRd.h"
#include ".\dvd\dvBufMgr.h"

#include ".\AL\REG_intctrl.h"
#include ".\al\REG_dvd_decoder.h"

/*************************************************************************/
/*                         Declarations                                  */
/*************************************************************************/
ULONG dvdramLastDecodedLSN;
BOOL CheckPSNInDataArea(ULONG InputPSN);
void RamReadSkip(void);
BYTE GetSpareAreaFullFlags(void);

/*************************************************************************/
/*                         Defines                                       */
/*************************************************************************/
#define ROW_ERROR_THRESHOLD               4     /* bytes */
#define SECTOR_ERROR_THRESHOLD            4     /* rows  */
#define BLK_ROW_ERROR_THRESHOLD_SLIP      6     /* rows  */
#define BLK_ROW_ERROR_THRESHOLD_LINEAR    8     /* rows  */

#define DVDRAM_MAX_SEEK_RETRY     3

/*************************************************************************/
/*                         Functions                                     */
/*************************************************************************/

void SwitchBackDVDRAMBuffer(void)
{
    DISABLE_INTERRUPT();
    DelaymS(10);
    Iram_usiRingSP = 0;
    Iram_usiRingEP = DVDRAM_RING_END_PAGE;

    WRITE_REG(HAL_DVDDREG_MAP->STARTBKL, Iram_usiRingSP);           /* ring start blk.  */
    WRITE_REG(HAL_DVDDREG_MAP->ENDBKL, Iram_usiRingEP);             /* ring end blk.    */
    WRITE_REG(HAL_DVDDREG_MAP->PIPOSRTL, DVDRAM_PIPO_START_BLOCK);  /* PI/PO start blk. */

    DataBuffer.CurDiscPtr = 0;
    ENABLE_INTERRUPT();

}


/*************************************************************************************/
/*  ConvertLSNToPSNForSAOnly()                                                       */
/*                                                                                   */
/*  Description: This function is suitable for Primary Spare Area only               */
/*  Parameter  : Offset - effective sector number counted from 0x31000               */
/*  Return     : RefPSN - pysical sector number                                      */
/*************************************************************************************/
ULONG ConvertLSNToPSNForSAOnly(USHORT Offset)
{
    ULONG  StartPSN, RefPSN;
    USHORT RemainPDL;

    StartPSN = ZONE0_START_PSN;
    RefPSN = StartPSN + Offset;       /* This is 'Q' */

    while(1)
    {
        RemainPDL = CalcPDLNumberBetween2PSN(StartPSN,RefPSN);     /* This is 'R' */
        if (RemainPDL == 0)
            break;
        StartPSN = RefPSN + 1;  /* P = Q +1 */
        RefPSN += RemainPDL;    /* Q = Q +R */
    }

    return RefPSN;
}


/*************************************************************************************/
/*  CalcPrimarySAEmptySpace()                                                        */
/*                                                                                   */
/*  Description: Caculate how many blocks are left in Primary SA                     */
/*  Parameter  : None                                                                */
/*  Return     : None                                                                */
/*************************************************************************************/
void CalcPrimarySAEmptySpace(void)
{
    ULONG  *SDLp;
    ULONG  MinPSN,RepPSN;
    ULONG  UserDataSize,StartPSNSsa;
    USHORT NumOfPDLZone0,NumOfPDLSsa,num;

    UserDataSize = SWAPEND_ULONG(RWDisc.dvRAM.StDDSp->StartLsn[1]);  //Zone0 size
    NumOfPDLZone0 = CalcPDLNumberBetween2PSN(ZONE0_START_PSN,ZONE0_END_PSN);
    StartPSNSsa = SWAPEND_ULONG(RWDisc.dvRAM.StSDLp->StartPSNOfSupSpareArea);
    NumOfPDLSsa = CalcPDLNumberBetween2PSN(StartPSNSsa,DVD_RAM_END_OF_DATA_ZONE);

    if (GetDiscCertificationFlag() & eInProgress)  /* This is during Format, if PDL Entry is Full */
    {
        RWDisc.dvRAM.PrimarySARemainBlk = (PHYSICAL_SECTOR_NUMBER_OF_ZONE0 - ((NumOfPDLZone0+15)/16)*16 - UserDataSize) / ONE_DVD_BLOCK;
    }
    else /* This is during Start-up */
    {
        if (GetTotalSdlEntry() == 0)
        {
            RWDisc.dvRAM.PrimarySARemainBlk = (PHYSICAL_SECTOR_NUMBER_OF_ZONE0 - ((NumOfPDLZone0+15)/16)*16 - UserDataSize) / ONE_DVD_BLOCK;
        }
        else  /* Find Min. RepPSN in SDL Entry */
        {
            if ((GetSpareAreaFullFlags() & ePrimaryFull))  //PSA exhausted
            {
                RWDisc.dvRAM.PrimarySARemainBlk = 0;
                if (!(GetSpareAreaFullFlags() & eSupplementaryFull)) //SSA not full
                {
                    SDLp = (ULONG *)(DRAM_START_SDL + 24);  /* First Entry of SDL */
                    MinPSN = 0x00FFFFFF;
                    for (num = 0; num < GetTotalSdlEntry(); num++)
                    {
                        RepPSN = SWAPEND_ULONG(*(SDLp + 1)) & 0x00FFFFFF;
                        if (RepPSN > StartPSNSsa)  //Replaced in SSA
                            if (RepPSN < MinPSN)
                                MinPSN = RepPSN;
                        SDLp += 2;  /* Shift to next SDL Entry */
                    }

                    if (MinPSN == 0x00FFFFFF)  /* There is no Spare Area space used by Linear Replacement, all entries are SLR=1 and RepPSN=0 */
                        RWDisc.dvRAM.SupSARemainBlk = (DVD_RAM_END_OF_DATA_ZONE + 1 - StartPSNSsa - NumOfPDLSsa) / ONE_DVD_BLOCK;
                    else
                        RWDisc.dvRAM.SupSARemainBlk = (MinPSN - StartPSNSsa - CalcPDLNumberBetween2PSN(StartPSNSsa,MinPSN)) / ONE_DVD_BLOCK;
                }
            }
            else
            {
                SDLp = (ULONG *)(DRAM_START_SDL + 24);  /* First Entry of SDL */
                MinPSN = 0x00FFFFFF;
                for (num = 0; num < GetTotalSdlEntry(); num++)
                {
                        RepPSN = SWAPEND_ULONG(*(SDLp + 1)) & 0x00FFFFFF;
                        if(RepPSN != 0)
                        {
                            if (RepPSN < MinPSN)
                                MinPSN = RepPSN;
                        }
                    SDLp += 2;  /* Shift to next SDL Entry */
                }

                if (MinPSN == 0x00FFFFFF)  /* There is no Spare Area space used by Linear Replacement, all entries are SLR=1 and RepPSN=0 */
                    RWDisc.dvRAM.PrimarySARemainBlk = (PHYSICAL_SECTOR_NUMBER_OF_ZONE0 - ((NumOfPDLZone0+15)/16)*16 - UserDataSize) / ONE_DVD_BLOCK;
                else
                    RWDisc.dvRAM.PrimarySARemainBlk = (MinPSN - DVD_RAM_START_OF_DATA_ZONE - CalcPDLNumberBetween2PSN(ZONE0_START_PSN,MinPSN)) / ONE_DVD_BLOCK;
            }
        }
    }
}

/*************************************************************************************/
/*  CheckPSNInDataArea()                                                             */
/*                                                                                   */
/*  Description:  Check whether the input PSN is in Data Area or Spare Area          */
/*  Parameter  :  InputPSN - Any PSN from first zone to last zone                    */
/*  Return     :  TRUE - In Data Area                                                */
/*                FALSE - In Spare Area                                              */
/*************************************************************************************/
BOOL CheckPSNInDataArea(ULONG InputPSN)
{
    ULONG PSNOfLSN0;
    ULONG StartPSNOfSSA;

    PSNOfLSN0 = SWAPEND_ULONG(RWDisc.dvRAM.StDDSp->PhysicalLocationOfLsn0);
    StartPSNOfSSA = SWAPEND_ULONG(RWDisc.dvRAM.StSDLp->StartPSNOfSupSpareArea);
    if (StartPSNOfSSA  == 0) //SSA not allocatted
    {
        if (InputPSN >= PSNOfLSN0)
            return TRUE;
        else
            return FALSE;
    }
    else
    {
        if ((InputPSN >= PSNOfLSN0) && (InputPSN < StartPSNOfSSA))
            return TRUE;
        else
            return FALSE;
    }
}

/*******************************************************************************
* Name: RamReadSkip
* Description : Disbale read buffering at current zone end, and enable at next zone start
* Inputs:       none
* Outputs:      none
*******************************************************************************/
void RamReadSkip(void)
{
    DDSTYPE * pDDS;
    ULONG CurZoneEndPSN, NextZoneStartLSN, NextZoneStartPSN;
    ULONG LastLSN;
    BYTE CurZone, i;

    if(dvdDiscRequest.TargetID.Long < DVD_RAM_START_OF_DATA_ZONE)
        return;

    pDDS = (DDSTYPE *) DRAM_START_DDS;

    LastLSN = HostXfer.StartBlock.lba;
    i = DVDRAM_LSN2ZONE(HostXfer.StartBlock.lba);
    CurZone = DVDRAM_LSN2ZONE(DataBuffer.CacheStartLBA);

    if((CurZone - i) > 1)   //cache buffer exceed read command 2 zones
    {
        CurZone--;
    }
    else if( (i + 1) == CurZone)    //there is a guard zone
        CurZone = i;
    else if(i == CurZone)   //same zone, do nothing
    {
        return;
    }


    NextZoneStartLSN = SWAPEND_ULONG(pDDS->StartLsn[CurZone + 1]);
    NextZoneStartPSN = ConvertLSNToPSN(NextZoneStartLSN);           //Next data zone start
    CurZoneEndPSN = ConvertLSNToPSN(NextZoneStartLSN - 1);  //Previous data zone end

    if(DataBuffer.CacheStartLBA == NextZoneStartLSN)   //transfer current zone data complete
    {
        if(DVD_BUFFERING() == ON && RWDisc.dvRAM.CurrentPID < NextZoneStartPSN)   //buffering in guard zone
        {
            extern ULONG Last_PSN;

            dvdLastDecodedPSN = NextZoneStartPSN - 1;    //update read error retry seek PSN
            Last_PSN = NextZoneStartPSN - 0x10;
            ANEW_SEEK = TRUE;
            dvdStopDiscTransfer();
            send_msg5S(SEND_MRW_MSG, "Cros Zn");
        }
    }
    else
    {
        return;
    }
}


BYTE GetSpareAreaFullFlags(void)
{
    return RWDisc.dvRAM.StSDLp->SpareAreaFullFlag;
}

#endif /* #if (DVD_RAM_READ == 1) */

