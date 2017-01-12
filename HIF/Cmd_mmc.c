/****************************************************************************
*                (c) Copyright 2001 - 2005 SUNEXT DESIGN, INC.
*                           All Right Reserved
*
*
* FILENAME:   Cmd_mmc.C                   /gp_e
*
* DESCRIPTION
*   This file contains MMC write command and related operation.
*
* NOTES:
*
* $Revision: 44 $
* $Date: 08/12/24 9:47a $
*
************************ SUNEXT CONFIDENTIAL *******************************/

#include ".\common\globals.h"
#include ".\common\globtype.h"
#include ".\common\chip.h"
#include ".\common\error.h"
#include ".\common\commath.h"

#include ".\servo\svo.h"    /* Please include no servo file except svo.h */
#include ".\player\plrDb.h"
#include ".\hif\gp_mac.h"
#include ".\hif\at_knl.h"
#include ".\hif\mmc.h"
#include ".\hif\at_util.h"
#include ".\hif\bf_util.h"

#if (DVD_RAM_READ == 1)
#include ".\defmgmt\common\mrw_ram.h"
#include ".\defmgmt\dvdram\ramDef.h"
#endif

#if (EN_LS == 1)
#include ".\LS\LS_hif.h"
#endif // End of (EN_LS == 1)

#include ".\al\REG_intctrl.h"
#include ".\al\REG_dvd_encoder.h"

/*--------------------------------------------------------------------
  Name: ReadBufferCapacityCommand()
  Description:
    Processes ATAPI Read Buffer Capacity command
  Inputs:
    none
  Outputs:
    none
  Returns:
    none
--------------------------------------------------------------------*/
void ReadBufferCapacityCommand(void)
{
    ULONG   buff_blocks;
    ULONG   buff_bytes;
    ULONG   free_buff_bytes;
    BYTE    dt,data[4];
    SHORT   length;

    length  = ((SHORT)Iram_Cmd_block.bf[7]<<8) & 0xFF;
    length += (SHORT)Iram_Cmd_block.bf[8] & 0xFF;

    if(length > BUF_CAPACITY_LEN_BYTE){
        length = BUF_CAPACITY_LEN_BYTE;
    }else if(length == 0){
        AutoCommandComplete();
        return;
    }

    DISABLE_INTERRUPT();

    /* send it to dram and to host;*/
    Iram_Buf_dram_addr = BUF_CAPACITY_ADR;
    WriteSDRAM(0);
    dt = (BYTE)BUF_CAPACITY_LEN_BYTE - 2;
    WriteSDRAM(dt);

#if (EN_LS == 1)
    if (pdbGetDiscKind() == eLSDisc)
    {/* ReadBufferCapacityCommand for LigthScribe always ignores Block bit and return data in legacy mode(Block bit = 0) */
        WriteSDRAM(0);
        WriteSDRAM(0);

        buff_bytes = LS_PRINT_TOTAL_PAGE * LS_ONE_PAGE_LENGTH;
        free_buff_bytes = (LS_PRINT_TOTAL_PAGE - LSRecordInfo.BufferedPageCount) * LS_ONE_PAGE_LENGTH;

        /* Length of the Buffer */
        WriteSDRAM((BYTE)(buff_bytes>>24));
        WriteSDRAM((BYTE)(buff_bytes>>16));
        WriteSDRAM((BYTE)(buff_bytes>>8));
        WriteSDRAM((BYTE)buff_bytes);

        /* Available Length of Buffer */
        WriteSDRAM((BYTE)(free_buff_bytes>>24));
        WriteSDRAM((BYTE)(free_buff_bytes>>16));
        WriteSDRAM((BYTE)(free_buff_bytes>>8));
        WriteSDRAM((BYTE)free_buff_bytes);

        SendParameterDataToHost(BUF_CAPACITY_ADR, length, TRUE);    // Transfer Data and Command Complete.
        return;
    }
#endif // End of (EN_LS == 1)

    /* Max Buffer Size */
    {
    }

    if((Iram_Cmd_block.bf[1] & 0x01) == 0x01){                  /* BLOCK Bit On ? */
        WriteSDRAM(0);
        WriteSDRAM(1);                                             /* Set BLOCK Bit On */

        data[0]         = 0x00;
        data[1]         = 0x00;
        data[2]         = 0x00;
        data[3]         = 0x00;
    }else{
        WriteSDRAM(0);
        WriteSDRAM(0);                                             /* Set BLOCK Bit Off */

        data[0]         = (char)((char *)&buff_bytes)[3];
        data[1]         = (char)((char *)&buff_bytes)[2];
        data[2]         = (char)((char *)&buff_bytes)[1];
        data[3]         = (char)((char *)&buff_bytes)[0];
    }

    WriteSDRAM(data[0]);
    WriteSDRAM(data[1]);
    WriteSDRAM(data[2]);
    WriteSDRAM(data[3]);

    /* Free Buffer Size */
    free_buff_bytes = buff_bytes;

    if((Iram_Cmd_block.bf[1] & 0x01) == 0x01){                  /* BLOCK Bit On ? */
        data[0] = (char)((char *)&buff_blocks)[3];
        data[1] = (char)((char *)&buff_blocks)[2];
        data[2] = (char)((char *)&buff_blocks)[1];
        data[3] = (char)((char *)&buff_blocks)[0];
    }else{
        data[0] = (char)((char *)&free_buff_bytes)[3];
        data[1] = (char)((char *)&free_buff_bytes)[2];
        data[2] = (char)((char *)&free_buff_bytes)[1];
        data[3] = (char)((char *)&free_buff_bytes)[0];
    }

    /* fill byte 8~11 */
    WriteSDRAM(data[0]);
    WriteSDRAM(data[1]);
    WriteSDRAM(data[2]);
    WriteSDRAM(data[3]);

    ENABLE_INTERRUPT();
    SendParameterDataToHost(BUF_CAPACITY_ADR, length, TRUE);    // Transfer Data and Command Complete.
}
