/****************************************************************************
*           (c) Copyright 2001 - 2010 Sunext Technology Co., Ltd.
*                           All Right Reserved
*
* FILENAME:   ramNewDfmgt.c
*
*  $Revision: 2 $
*  $Date: 10/02/21 11:39a $
*
* DESCRIPTION
*   This file contains defect management utility for DVD-RAM
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
#include ".\defmgmt\dvdram\ramExtern.h"
#include ".\dvd\dvHostRd.h"
#include ".\dvd\dvBufMgr.h"

/*************************************************************************/
/*                         Declarations                                  */
/*************************************************************************/



/*************************************************************************/
/*                         Functions                                     */
/*************************************************************************/

BYTE RAM_GetDefectInfo(LONG LSN, LONG Length)
{
    ULONG   *SDLp;
    ULONG   StartBlkLSN, EndBlkLSN, EndHostLSN, CurrentPSN;
    ULONG   SdlEntry_Full, SdlEntry;
    USHORT  PDLNumbers;
    BYTE    BlkCnt, ContiguousBlocksAvailable;
    BYTE    offset;
    BYTE    index;

    StartBlkLSN = LSN & DVD_BLOCK_MASK;
    EndBlkLSN = ((LSN + Length - 1) & DVD_BLOCK_MASK);
    BlkCnt = (EndBlkLSN - StartBlkLSN)/ONE_DVD_BLOCK + 1;

    EndHostLSN = LSN + Length - 1;

    index = 0;

    while (BlkCnt > 0)
    {
        CurrentPSN = ConvertLSNToPSN(LSN & DVD_BLOCK_MASK);
        SDLp = SearchSdlEntry(CurrentPSN);
        SdlEntry_Full = SWAPEND_ULONG(*SDLp);  //Full means the variable contains SLR bit and Defect PSN.
        SdlEntry  = SdlEntry_Full & 0x00FFFFFF;
        if (SdlEntry == 0x00FFFFFF)  //There is no SDL block behind CurrentPSN
        {
            ContiguousBlocksAvailable = BlkCnt;
        }
        else
        {
            PDLNumbers = CalcPDLNumberBetween2PSN(CurrentPSN,SdlEntry);
            ContiguousBlocksAvailable =  ( ((SdlEntry - CurrentPSN - PDLNumbers) / SECTORS_PER_BLOCK) < BlkCnt)
                             ? ((SdlEntry - CurrentPSN - PDLNumbers) / SECTORS_PER_BLOCK) : BlkCnt;

            send_msg5SValue(SEND_MRW_MSG,"SdlEntry = ",4,SdlEntry);
            send_msg5SValue(SEND_MRW_MSG,"PDLNumbers = ",2,PDLNumbers);
            send_msg5SValue(SEND_MRW_MSG,"CBA = ",1,ContiguousBlocksAvailable);
        }

        if (ContiguousBlocksAvailable >= BlkCnt)
        {
            RWDisc.dvRAM.REntry[index].LSN = LSN;
            RWDisc.dvRAM.REntry[index].Len = Length;
            RWDisc.dvRAM.REntry[index].Attr = eDA;
            Length = 0;
            BlkCnt = 0;
        }
        else if (ContiguousBlocksAvailable > 0)	   // BlkCnt > CBA > 0, SDLEntry is between requested blocks
        // Mark the sectors before SDLEntry as no defect, and move the Host pointer to SDLEntry
        {
            RWDisc.dvRAM.REntry[index].LSN = LSN;
            RWDisc.dvRAM.REntry[index].Len = ConvertPSNToLSN(SdlEntry) - LSN;
            RWDisc.dvRAM.REntry[index].Attr = eDA;

            offset = LSN & LAST_LBA_OF_DVDBLOCK;

            LSN += RWDisc.dvRAM.REntry[index].Len;
            Length -= RWDisc.dvRAM.REntry[index].Len;
            BlkCnt -= (RWDisc.dvRAM.REntry[index].Len + offset)/ONE_DVD_BLOCK;
        }
        else //CBA = 0, it means this is a SDL block
        {
            RWDisc.dvRAM.REntry[index].LSN = LSN;
            if (index == 0) //First Fragment
            {
                offset = LSN & LAST_LBA_OF_DVDBLOCK;
                if ( EndHostLSN >= ((LSN & DVD_BLOCK_MASK) + ONE_DVD_BLOCK) ) //End > Next block start
                    RWDisc.dvRAM.REntry[index].Len = ONE_DVD_BLOCK - offset;
                else
                    RWDisc.dvRAM.REntry[index].Len = Length;
                LSN = (LSN & DVD_BLOCK_MASK) + ONE_DVD_BLOCK; //shift to next block
                Length -= RWDisc.dvRAM.REntry[index].Len;
            }
            else
            {
                RWDisc.dvRAM.REntry[index].Len = (Length < ONE_DVD_BLOCK) ? Length : ONE_DVD_BLOCK;
                LSN += ONE_DVD_BLOCK;
                Length -= ONE_DVD_BLOCK;
            }
            RWDisc.dvRAM.REntry[index].Attr = eSA;
            RWDisc.dvRAM.REntry[index].DefPSN = SdlEntry_Full;

            BlkCnt -= 1;
        }
        index ++;
    }

    //PrintDefectInfo(index);

	  return index;
}

// TBD!
void DVDRAMMarkSLRBit(ULONG DefPSN, BYTE SLR_BIT)
{
    ULONG   *SDLp;

    SDLp = SearchSdlEntry(DefPSN & 0x00FFFFFF);

    send_msg5SValue(SEND_MRW_MSG,"*SDLp = ",4,*SDLp);

    if((*SDLp & SLR_BYTE) == (SLR_BIT << 6))
        return;

    if (SLR_BIT)
        *SDLp |= SLR_BYTE;  //no swap, byte 0 is in right side.
    else
        *SDLp &= ~SLR_BYTE;

    RWDisc.dvRAM.SdlUpdateFlag = TRUE;
}


#endif /* #if (DVD_RAM_READ == 1) */

