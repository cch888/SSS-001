/****************************************************************************
*                (c) Copyright 2001 - 2005 SUNEXT DESIGN, INC
*                           All Rights Reserved
*
*
* FILENAME:   bf_util.c
*
* DESCRIPTION
*   This file contains buffer related routines
*
* NOTES:
*
* $Revision: 29 $
* $Date: 11/01/18 10:30a $
*
**************************** SUNEXT CONFIDENTIAL *******************************/

#include ".\common\globals.h"
#include ".\common\globtype.h"
#include ".\common\chip.h"
#include ".\common\system.h"
#include ".\hif\mmc.h"
#include ".\hif\gp_mac.h"
#include ".\hif\bf_util.h"
#include ".\al\reg_hal.h"
#include ".\al\reg_global.h"
#include ".\al\reg_intctrl.h"
#include ".\al\reg_cpycmp.h"

/********************************************************************
* WriteSDRAM()
*
* Write data into the DRAM area
*
* Returns:  void
* Params:   (input) BYTE data
* Globals:  Sys_page, Iram_Buf_dram_addr - DRAM address
*********************************************************************/
void WriteSDRAM(BYTE v1)
{
    SCSDRAM[Iram_Buf_dram_addr] = v1;
    if (Iram_Buf_dram_addr < END_DRAM_BUFFER)   /* increase dram address by 1 */
        Iram_Buf_dram_addr++;
}

/********************************************************************
* WriteSDRAM2B()
*
* Write data into the DRAM area
*
* Returns:  void
* Params:   (input) USHORT data
* Globals:  Sys_page, Iram_Buf_dram_addr - DRAM address
*********************************************************************/
void WriteSDRAM2B(USHORT v1)
{
    SCSDRAM16[Iram_Buf_dram_addr/sizeof(USHORT)] = v1;
    if (Iram_Buf_dram_addr < END_DRAM_BUFFER)   /* increase dram address by 1 USHORT location*/
       Iram_Buf_dram_addr += sizeof(USHORT);
}

/********************************************************************
* WriteSDRAM4B()
*
* Write data into the DRAM area
*
* Returns:  void
* Params:   (input) ULONG data
* Globals:  Sys_page, Iram_Buf_dram_addr - DRAM address
*********************************************************************/
void WriteSDRAM4B(ULONG v1)
{
    SCSDRAM32[Iram_Buf_dram_addr/sizeof(ULONG)] = v1;
    if (Iram_Buf_dram_addr < END_DRAM_BUFFER)   /* increase dram address by 1 ULONG location*/
       Iram_Buf_dram_addr += sizeof(ULONG);
}

/*********************************************************************
* ReadSDRAM
*
* Read data from SDRAM area
*
* Returns:  BYTE - Dram data
* Params:   none
* Globals:  Iram_Buf_dram_addr - DRAM address
*********************************************************************/
BYTE ReadSDRAM(void)
{
    BYTE v1;

    v1 = (SCSDRAM[Iram_Buf_dram_addr]);
    if (Iram_Buf_dram_addr < END_DRAM_BUFFER)       /* increase dram address by 1 */
        Iram_Buf_dram_addr++;

    return (v1);
}

/*********************************************************************
* ReadSDRAM2B
*
* Read data from SDRAM area
*
* Returns:  USHORT - Dram data
* Params:   none
* Globals:  Iram_Buf_dram_addr - DRAM address
*********************************************************************/
USHORT ReadSDRAM2B(void)
{
    USHORT v1;

    v1 = (SCSDRAM16[Iram_Buf_dram_addr/sizeof(USHORT)]);
    if (Iram_Buf_dram_addr < END_DRAM_BUFFER)       /* increase dram address by 1 USHORT location */
       Iram_Buf_dram_addr += sizeof(USHORT);
    return (v1);
}

/*********************************************************************
* ReadSDRAM4B
*
* Read data from SDRAM area
*
* Returns:  ULONG - Dram data
* Params:   none
* Globals:  Iram_Buf_dram_addr - DRAM address
*********************************************************************/
ULONG ReadSDRAM4B(void)
{
    ULONG v1;

    v1 = (SCSDRAM32[Iram_Buf_dram_addr/sizeof(ULONG)]);
    if (Iram_Buf_dram_addr < END_DRAM_BUFFER)       /* increase dram address by 1 ULONG location */
       Iram_Buf_dram_addr += sizeof(ULONG);
    return (v1);
}

/************************************************************************
    Name:       PatternFill()

    Description: Function to fill data pattern
    Returns:
            None
    note: program rwCpyPCnt need adjust by 1.
          program rwCpyWCnt need adjust by 1?
*************************************************************************/
void PatternFill(USHORT trgpage,USHORT trgofset, USHORT pgnum, USHORT wordnum, USHORT pattern)
{
    BOOL IE;

    WRITE_FIELD(HAL_GLOBAL_CPYCKSTP,0);    // Turn on Copy clock
    WRITE_FIELD(HAL_CPYCMP_CPYTGT, trgpage);
    WRITE_FIELD(HAL_CPYCMP_CPYTOF, trgofset);
    WRITE_FIELD(HAL_CPYCMP_CPYPCNT, pgnum - 1);
    WRITE_FIELD(HAL_CPYCMP_CPYWCNT, wordnum);
    WRITE_FIELD(HAL_CPYCMP_CPYPAT, pattern);

    St_pfil_done = FALSE;
    IE = GET_INT_ENABLE_MASK();
    ENABLE_INTERRUPT();

    WRITE_FIELD(HAL_CPYCMP_PFILSTRT, 1);
    WRITE_FIELD(HAL_CPYCMP_TFLAG, 1);
    WRITE_FIELD(HAL_CPYCMP_SFLAG, 1);

    /* Make sure interrupt is enabled before calling */
    while (!St_pfil_done)
    {
//      ExitProcess();
    }
    St_pfil_done = FALSE;

    WRITE_FIELD(HAL_GLOBAL_CPYCKSTP,1); /* turn off Copy clock */

    SET_INT_ENABLE_MASK(IE);
 }




/*********************************************************************
* CalcHostTransferBlocks
*
* This routine calculate number of transfer pgaes for the succeeding
*  host transfer
*
* Params:   buf_drqblk   - maximum number of blocks host can transfer
*           DataBuffer.SectorCount   - number of valid blocks in buffer
*           HostXfer.BlocksLeft - number of remaining host request block
*********************************************************************/
void CalcHostTransferBlocks(USHORT buf_drqblk)
{
    /* if buf_blkcnt >= buf_drqblk => buf_hxfer_blk=buf_drqblk
          mark xfr required,  goto (1) */

    /* if mode change bit set, only allow to transfer one block at the time,
       the buf_hxfer_blk will be set to 1. */

    if(Rw_mode_change == FALSE)
    {
        /* compare number of valid blocks in buffer (buf_blkcnt) with number of
           blocks host can transfer (buf_drqblk), buf_hxfer_blk is assigned
           to whichever smaller. */

        if (DataBuffer.SectorCount >= buf_drqblk)
            Iram_Buf_hxfer_blk = buf_drqblk;
        else
            Iram_Buf_hxfer_blk = DataBuffer.SectorCount;
    }
    else
        Iram_Buf_hxfer_blk = 0x01;           /* set single block transfer */

    /* If number of blocks should be transfer (buf_hxfer_blk) is equal or greater
       than number of remaining host request block (buf_h2go_blk), assign
       buf_hxfer_blk=buf_h2go_blk, and set auto completion because it will be
       the last transfer. */
#if (AUTO_STATU_CPLT==1)
    fKernelAutoCompletionStatus =FALSE;
#endif

    if ( Iram_Buf_hxfer_blk >= (USHORT)HostXfer.BlocksLeft )
    {/* assign to remained host block */
#if (AUTO_STATU_CPLT==1)
        if (MRW_CheckDisc()==FALSE)
            fKernelAutoCompletionStatus =TRUE;
#endif
        Iram_Buf_hxfer_blk = (USHORT)HostXfer.BlocksLeft;
    }

}

/*******************************************************************************

    FUNCTION        CopyBufferToBuffer

    DESCRIPTION   Copy a nuber of pages from one area of the Buffer Memory to another
    PARAMETERS

    GLOBAL UPDATES

********************************************************************************/
void CopyBufferToBuffer(USHORT SourcePage,USHORT SourceOffset, USHORT TargetPage, USHORT TargetOffset, USHORT PagesToCopy)
{
    WRITE_FIELD(HAL_GLOBAL_CPYCKSTP,0); /* turn on Copy clock */

    St_copy_done = FALSE;

    WRITE_REG(HAL_CPYCMPREG_MAP->CPYCTL, 0);
    WRITE_FIELD(HAL_CPYCMP_CPYSRC, SourcePage);              // Source Page: Start of DRAM Buffer.
    WRITE_FIELD(HAL_CPYCMP_CPYSOF, SourceOffset);            // Source Offset.
    WRITE_FIELD(HAL_CPYCMP_CPYTGT, TargetPage);              // Target Page in DRAM.
    WRITE_FIELD(HAL_CPYCMP_CPYTOF, TargetOffset);            // Target Offset.
    WRITE_FIELD(HAL_CPYCMP_CPYPCNT,(PagesToCopy - 1));       // Copy x pages.
    WRITE_FIELD(HAL_CPYCMP_CPYWCNT, (DRAM_PAGE_SIZE / 2 ));  // Page size in words.
    WRITE_FIELD(HAL_CPYCMP_CPYSTRT, 1);                      // Start the copying hardware.

    while (!St_copy_done)
    {
        ExitProcess();
    }

    WRITE_FIELD(HAL_GLOBAL_CPYCKSTP,1); /* turn off Copy clock */
}
#if (STORE_MSG_TO_SDRAM == 1)
/********************************************************************
* MsgWriteSDRAM()
*
* Write Msg into the DRAM area
*
* Returns:  void
* Params:   (input) data
* Globals:  
*********************************************************************/
void MsgWriteSDRAM(BYTE *message,BYTE cnt)
{
    
    unsigned char  length, i;
    length = cnt;
    for (i = 0; i < length; i++)
    {
        SCSDRAM[Iram_Store_Msg_dram_addr] = message[i];
        if (Iram_Store_Msg_dram_addr < END_DRAM_BUFFER)   /* increase dram address by 1 */
            Iram_Store_Msg_dram_addr++;
    
        if(Iram_Store_Msg_dram_addr > MsgEdAddr)
        {
            Iram_Store_Msg_dram_addr  =  MsgStAddr;
            LoggingFull = TRUE;
        }
    }
}
#endif
