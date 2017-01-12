/****************************************************************************
*                (c) Copyright 2000,2001,2002,2003, 2004 SUNEXT DESIGN, INC.
*                           All Right Reserved
*
*
* FILENAME:   dvRdStr.h
*
* DESCRIPTION
*
*
*
* CHANGES:
* VERSION   DATE        WHO     DETAIL
* -------   --------    ------  ---------------------------------------------
*    1.00   May-25-01   Wata    Create
**************************** SUNEXT CONFIDENTIAL *******************************/
/* FILE_VERSION : 1.00 2001-05-25 */

#ifndef __DVRDSTR_H__
#define __DVRDSTR_H__

/*********************************************************************
   FUNCTION PROTOTYPES
*********************************************************************/
extern void ReadDVDStructureCommand(void);
#if (BCA_ENABLE == 1)
extern BYTE DVDReadBCA(void);
#define ATAPI_BCA_RETRY_TIMES       3
#endif

/*********************************************************************
        Physical define
*********************************************************************/

/*********************************************************************
        Global Structures
*********************************************************************/

#endif
