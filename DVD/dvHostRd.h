/****************************************************************************
*                (c) Copyright 2000,2001,2002,2003, 2004 SUNEXT DESIGN, INC.
*                           All Right Reserved
*
*
*
*   FILE NAME:     $Workfile: dvHostRd.h $
*
*   DESCRIPTION:
*   DVD Read stuff at the Host end of the code.
*
*   $Revision: 9 $
*   $Date: 07/11/13 11:17a $

**************************** SUNEXT CONFIDENTIAL *******************************/

#ifndef __dvHOSTRD_H__
#define __dvHOSTRD_H__

#include ".\common\globals.h"


/*******************************************************************
    STRUCTURES AND TYPES
*******************************************************************/

/*******************************************************************
    ENUMERATIONS
*******************************************************************/

/*******************************************************************
    DEFINES
*******************************************************************/
#define FUA 0x08                            /* Byte 1/bit 3 in Read 10 cmd for FUA */

/*----------------------------------------
    NORMAL DVD HOST TRANSFER PARAMETERS
-----------------------------------------*/
#define DVD_HOST_SECTOR_OFFSET      0x0000  /* Offset into sector for transferring - always 0 for DVD */
#define DVD_HOST_SECTOR_SIZE        0x0800  /* DVD host sector is always 2K bytes */
#define DVD_HOST_WORDS_PER_SECTOR   0x03FF  /* Number of words per DVD sector minus 1 */

/*----------------------------------------
    DVD READ LONG HOST TRANSFER PARAMETERS
-----------------------------------------*/
#define DVD_SECTORS_PER_READ_LONG_BLOCK 0x13


#define DVD_HOST_TRANSFER_LIMIT         0x1F    /* Maximum number of sectors that can be transferred at one time */

/*******************************************************************
    EXTERNAL FUNCTIONS
*******************************************************************/
extern void DVDRead10Command(void);
extern BYTE dvdFastCacheTransfer(void);
extern void dvdSetSenseCode( DiscAccessStatusE );
extern void dvdReadExit(void);
extern DiscAccessStatusE FullCacheHit(void);
extern DiscAccessStatusE PartialCacheHit(void);
extern DiscAccessStatusE CacheMiss(void);
#endif
