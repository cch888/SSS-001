/***************************************************************************
*                (c) Copyright 2001  -  2005 SUNEXT DESIGN, INC.
*                           All Right Reserved
*
* FILENAME:   mrw_ram.c
*
* DESCRIPTION
*   global variables definition
*
*
*   $Revision: 1 $
*   $Date: 09/12/10 6:22p $
*
**************************** SUNEXT CONFIDENTIAL ****************************/

#include ".\common\globtype.h"
#include ".\common\globals.h"
#include ".\common\error.h"
#include ".\common\commath.h"
#include ".\common\chip.h"
#include ".\common\system.h"
#include ".\common\stdlib.h"
#include ".\hif\mmc.h"
#include ".\hif\gp_equ.h"
#include ".\hif\gp_mac.h"

#include ".\defmgmt\common\mrw_def.h"
#include ".\defmgmt\common\mrw_ram.h"
/*------------------------------------------------------------------------
   Globle variables for CD-MRW / DVD+MRW / DVDRAM / DVD+RW
-------------------------------------------------------------------------*/
tRWDiscU RWDisc;

#if (DVD_RAM_READ == 1)
const ULONG DmaLocation[4][3] =    /* First dimension is DMA 1..4, Second dimension 0 = RAM 1.0, Second dimension 1 = RAM 2.0 */
{
/* These are the PSN addresses of the DMAs on the disc. */
/*      Ver.1.0     Ver.20        8cm Ver 2.0           */
    DMA_1_RAM_10, DMA_1_RAM_20, DMA_1_RAM_20_8CM,
    DMA_2_RAM_10, DMA_2_RAM_20, DMA_2_RAM_20_8CM,
    DMA_3_RAM_10, DMA_3_RAM_20, DMA_3_RAM_20_8CM,
    DMA_4_RAM_10, DMA_4_RAM_20, DMA_4_RAM_20_8CM
};
#endif
