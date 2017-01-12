/****************************************************************************
*           (c) Copyright 2001 - 2010 Sunext Technology Co., Ltd.
*                           All Right Reserved
*
*
*
* FILENAME:   com_im.c
*
* DESCRIPTION: This module contains functions that will reside in the
*               Intruction Memory
*
* NOTES:
*
*   $Revision: 22 $
*   $Date: 10/02/21 11:17a $
*
**************************** SUNEXT CONFIDENTIAL *****************************/

#include ".\common\globals.h"
#include ".\common\globtype.h"
#include ".\common\commath.h"
#include ".\hif\gp_ram.h"
#if (DVD_RAM_READ == 1)
#include ".\hif\gp_mac.h"
#endif
#include ".\player\plrdb.h"
#if (DVD_RAM_READ == 1)
#include ".\defmgmt\common\mrw_ram.h"
#include ".\defmgmt\dvdram\ramDef.h"

ULONG *SearchSdlEntry(LONG PsnValue);
ULONG *SearchPdlEntry(LONG PsnValue);
#endif


/***********************************************************
Name: HEX2BCD
Description: Converts a HEX value to a BCD value
Inputs: HEX Value
Output: BCD Value

************************************************************/
BYTE HEX2BCD(BYTE hex)
{
    return((hex / 10) << 4 | (hex % 10 ));
}

/***********************************************************
Name: BCD2MSF
Description: Converts a BCD number to a Hex value
Inputs: BCD pointer
Output: Hex Value

************************************************************/
TimeS BCD2MSF(TimeS *bcd)
{
    TimeS msf;

    msf.Minute = BCD2HEX(bcd->Minute);
    msf.Second = BCD2HEX(bcd->Second);
    msf.Frame  = BCD2HEX(bcd->Frame );

    return msf;
}

/***********************************************************
Name: MSF2BCD
Description: Converts a MSF Hex number to a BCD value
Inputs: MSF pointer
Output: BCD Value

************************************************************/
TimeS MSF2BCD(TimeS *msf)
{
TimeS  bcd;

    bcd.Minute = HEX2BCD(msf->Minute);
    bcd.Second = HEX2BCD(msf->Second);
    bcd.Frame  = HEX2BCD(msf->Frame );

    return bcd;
}

/***********************************************************
Name: MSF2Count
Description: Converts a MSF Hex number to block count
Inputs: MSF pointer
Output: block count
************************************************************/
long MSF2Count(TimeS *MSF)
{
    return( ((long)MSF->Minute * 4500L) + ((long)MSF->Second * 75L) + ((long)MSF->Frame) );
}

/*------------------------------------------------------------------------
    Name:       MSF2ADR

    Description: Convert an MSF value to an linear address

    Inputs:    Hex MSF

    Returns: address
------------------------------------------------------------------------*/
long MSF2ADR(TimeS *msf)   /* MSF time -> ADDRESS  */
{
    long addr;

    addr  = MSF2Count(msf);

    if ( (addr > 405000L) && (St_cdrom_video == FALSE) )
    {   /* 90:00:00~ is Inside Disc */
        addr -= 450000L;
    }
    else if ( (addr > 434250L) && (St_cdrom_video == TRUE) )
    {   /* 96:30:00~ is Inside Disc */
        addr -= 450000L;
    }

    return addr;
}


/*------------------------------------------------------------------------
    Name:       LBA2MSF

    Description: Convert an LBA value to MSF

    Inputs:    LBA value

    Returns: MSF value
------------------------------------------------------------------------*/
TimeS LBA2MSF(long lba)   /* LBA      -> MSF time */
{
    TimeS msf;
    long  min, sec, frm;

    lba += 150L;

    #if (BD_ENABLE)
    if( pdbDiscLoaded() == DVD_DISC || pdbDiscLoaded() == BD_DISC)
    #else
    if( pdbDiscLoaded() == DVD_DISC )
    #endif
    {
        if(lba >= 0x1193FF)
        {
            msf.Minute = 0xFF;
            msf.Second = 0x3B;
            msf.Frame = 0x4A;
            return msf;
        }
    }

    if(lba < 0)
    {
        lba += 450000L;
    }

    min  = lba / 4500L;
    lba -= (min * 4500L);
    sec  = lba / 75L;
    frm  = lba - (sec * 75L);

    msf.Minute = (BYTE)min;
    msf.Second = (BYTE)sec;
    msf.Frame  = (BYTE)frm;

    return msf;
}


/*------------------------------------------------------------------------
    Name:       CheckBCDTime

    Description: Check if valid BCD address

    Inputs:    Address value

    Returns: True or False
------------------------------------------------------------------------*/
BOOL CheckBCDTime(TimeS *bcd)
{
    if((bcd->Minute & 0xF0)>0xC0) return(FALSE); //support up to 129 min
    if((bcd->Minute & 0x0F)>0x09) return(FALSE);
    if((bcd->Second & 0xF0)>0x50) return(FALSE);
    if((bcd->Second & 0x0F)>0x09) return(FALSE);
    if((bcd->Frame  & 0xF0)>0x70) return(FALSE);
    if((bcd->Frame  & 0x0F)>0x09) return(FALSE);
    if(bcd->Frame  > 0x74)        return(FALSE);
    return TRUE;
}

#if (DVD_RAM_READ == 1)
/********************************************************************************************/
/*  DVDRAM_LSN2ZONE()                                                                       */
/*                                                                                          */
/*  Input     : LSN Value                                                                   */
/*  Return    : Zone Number                                                                 */
/*  Description : This function determine the Zone Number that the input LSN is located in. */
/********************************************************************************************/
BYTE DVDRAM_LSN2ZONE(ULONG LSN)
{
    BYTE Zone, LastZone;

    LastZone = LocalZoneNumber - 1;

    if (LSN >= SWAPEND_ULONG(RWDisc.dvRAM.StDDSp->StartLsn[LastZone]))   return LastZone;

    for ( Zone = 0; LSN >= (SWAPEND_ULONG(RWDisc.dvRAM.StDDSp->StartLsn[Zone])); Zone++)
    {
    }
    Zone--;

    return (Zone);
}

/********************************************************************************************/
/*  DVDRAM_PSN2ZONE()                                                                       */
/*                                                                                          */
/*  Input     : PSN Value                                                                   */
/*  Return    : Zone Number                                                                 */
/*  Description : This function determine the Zone Number that the input PSN is located in. */
/********************************************************************************************/
BYTE DVDRAM_PSN2ZONE(ULONG PSN)
{
    BYTE Zone, LastZone;

    LastZone = LocalZoneNumber - 1;

    if (PSN >= DVDRAMZoneTable[LastZone].StartSectorPSN) return LastZone;
    if (PSN <  DVDRAMZoneTable[1].StartSectorPSN)        return 0;

    for ( Zone = 1; PSN >= DVDRAMZoneTable[Zone].StartSectorPSN; Zone++)
    {
    }
    Zone--;
    return (Zone);
}

/*************************************************************************************/
/*                                                                                   */
/*  SearchSdlEntry(LONG PsnValue)                                                    */
/*                                                                                   */
/*  Function returns a pointer to an entry in a Secondary Defect List (SDL)          */
/*          of type DVD-RAM Ver 1.0 or Ver. 2.0                                      */
/*          that is equal or greater than that of the entry parameter.               */
/*  Parameter   PSN Value                                                            */
/*  Return    Pointer to next error entry after parameter PSN                        */
/*                                                                                   */
/*                                                                                   */
/*************************************************************************************/
ULONG * SearchSdlEntry(LONG SearchPSN)
{
    ULONG   *SDLp;
    ULONG   Entry;
    ULONG   EntryPrevious;
    USHORT *SDLCntp;
    USHORT  AddressMask;

    SDLCntp = (USHORT *)(DRAM_START_SDL + 22);  /* Get Address of number of SDL Entries */
    SDLp    = (ULONG *)(DRAM_START_SDL + 24);   /* Get Address of first SDL Entry (Byte 24) */

    if(*SDLCntp != 0)
    {
        AddressMask = 0x1000;

        /* Do a binary search. */
        do
        {
            SDLp += AddressMask;
            EntryPrevious = SWAPEND_ULONG(*(SDLp-2)) & 0x00FFFFFF; /*  Get Previous SDL Entry from Defect List in Buffer RAM and mask off Entry Type and Reserved bits      */
            Entry = SWAPEND_ULONG(*SDLp) & 0x00FFFFFF; /*  Get Current SDL Entry from Defect List in Buffer RAM and mask off Entry Type and Reserved bits      */
            if (( Entry >= SearchPSN) || ((ULONG)SDLp > DRAM_END_SDL) )   /* The last term of the equation is needed for discs with SDL almost full.  */
            {
                if (EntryPrevious < SearchPSN)
                    break;
                SDLp -= AddressMask;
            }
            AddressMask /= 2;
        }
        while (AddressMask > 1);        /* SDL has 64 bit entries as opposed to the 32 bit entries of PDL           */
                                        /*  the (1) in the while loop assures correct function for 64 bit entries.  */
    }

    return (SDLp);
}
/*************************************************************************************/
/*                                                                                   */
/*  SearchPdlEntry(LONG PsnValue)                                                    */
/*                                                                                   */
/*  Function returns a pointer to an entry in a Primary Defect List (PDL)            */
/*          of type DVD-RAM Ver 1.0 or Ver 2.0                                       */
/*          that is equal or greater than that of the entry parameter.               */
/*  Parameter   PSN Value                                                            */
/*  Return    Pointer to next first entry at or after parameter PSN                  */
/*                                                                                   */
/*                                                                                   */
/*************************************************************************************/
ULONG * SearchPdlEntry(LONG SearchPSN)
{
    ULONG   *PDLp;
    ULONG   Entry;
    ULONG   EntryPrevious;
    USHORT *PDLCntp;
    USHORT  AddressMask;

    PDLCntp = (USHORT *)(DRAM_START_PDL + 2);   /* Get Address of number of PDL Entries */
    PDLp    = (ULONG *)(DRAM_START_PDL + 4);    /* Get Address of first PDL Entry */

    if(*PDLCntp != 0)
    {
        AddressMask = 0x1000;

        /* Do a binary search. */
        do
        {
            PDLp += AddressMask ;
            EntryPrevious = SWAPEND_ULONG(*(PDLp-1)) & 0x00FFFFFF; /*  Get Previous PDL Entry from Defect List in Buffer RAM and mask off Entry Type and Reserved bits      */
            Entry = SWAPEND_ULONG(*PDLp) & 0x00FFFFFF; /*  Get Current PDL Entry from Defect List in Buffer RAM and mask off Entry Type and Reserved bits      */
            if (( Entry >= SearchPSN) || ((ULONG)PDLp > DRAM_END_PDL) )   /* The last term of the equation is needed for discs with PDL almost full.  */
            {
                if (EntryPrevious < SearchPSN)
                    break;
                PDLp -= AddressMask;
            }
            AddressMask /= 2;
        }
        while (AddressMask > 0);
    }

    return (PDLp);
}

/*************************************************************************************/
/*  ConvertLSNToPSN()                                                                */
/*                                                                                   */
/*  Description: Convert LSN address to PSN address                                  */
/*  Parameter  : LsnValue - input LSN                                                */
/*  Return     : RefPSN - output PSN                                                 */
/*************************************************************************************/
ULONG ConvertLSNToPSN(ULONG LsnValue)
{
    ULONG StartPSN, RefPSN;
    USHORT RemainPDL;
    BYTE   Zone;

    Zone = DVDRAM_LSN2ZONE(LsnValue);

    if (Zone == 0)
        StartPSN = SWAPEND_ULONG(RWDisc.dvRAM.StDDSp->PhysicalLocationOfLsn0);
    else
        StartPSN = DVDRAMZoneTable[Zone].StartUserSectorPSN;
    RefPSN = StartPSN + LsnValue - (SWAPEND_ULONG(RWDisc.dvRAM.StDDSp->StartLsn[Zone]));       /* This is 'Q' */

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
/*  ConvertPSNToLSN()                                                                */
/*                                                                                   */
/*  Description: Convert PSN address to LSN address                                  */
/*  Parameter  : CurPSN - input PSN                                                  */
/*  Return     : CurLSN - output LSN                                                 */
/*************************************************************************************/
ULONG ConvertPSNToLSN(ULONG CurPSN)
{
    ULONG  StartPSN,StartLSN,CurLSN,Offset;
    USHORT PDLCount;
    BYTE   Zone;

    Zone = DVDRAM_PSN2ZONE(CurPSN);

    StartLSN  = SWAPEND_ULONG(RWDisc.dvRAM.StDDSp->StartLsn[Zone]);
    if (Zone == 0)
        StartPSN = SWAPEND_ULONG(RWDisc.dvRAM.StDDSp->PhysicalLocationOfLsn0);
    else
        StartPSN  = DVDRAMZoneTable[Zone].StartUserSectorPSN;
    Offset    = CurPSN - StartPSN;
    PDLCount  = CalcPDLNumberBetween2PSN(StartPSN,CurPSN);
    CurLSN = StartLSN + Offset - PDLCount;

    return CurLSN;
}

/*************************************************************************************/
/*                                                                                   */
/*  CalcPDLNumberBetween2PSN(ULONG StartPSN,ULONG EndPSN)                            */
/*                                                                                   */
/*  Function calculates the PDL entry numbers between two PSN.                       */
/*      Parameter: StartPSN, EndPSN                                                  */
/*      Return   : PDLEntryCount                                                     */
/*************************************************************************************/
USHORT CalcPDLNumberBetween2PSN(ULONG StartPSN,ULONG EndPSN)
{
    ULONG   *PDLp;
    USHORT  PDLEntryCount;

    if ((EndPSN & 0x00FFFFFF) == 0x00FFFFFF)  return 0;

    PDLp = SearchPdlEntry(StartPSN);

    for (PDLEntryCount = 0; ((SWAPEND_ULONG(*PDLp)) & 0x00FFFFFF) <= EndPSN ; PDLEntryCount++)
    {
        PDLp++;
    }
    return PDLEntryCount;
}
#endif // #if (DVD_RAM_READ == 1)
