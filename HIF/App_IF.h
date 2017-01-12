/****************************************************************************
*                (c) Copyright 2001 - 2005  SUNEXT DESIGN, INC.
*                           All Rights Reserved
*
*
* FILENAME: app_if.h
*
* DESCRIPTION:
*
* NOTES:
*
*   $Revision: 3 $
*   $Date: 06/04/03 5:37a $
*
**************************** SUNEXT CONFIDENTIAL *************************/

#ifndef __APP_IF_H__
#define __APP_IF_H__

#include ".\common\error.h"
#include ".\player\plrdb.h"

extern BOOL RequestCatalogNumber(void);
extern BOOL RequestISRCNumber(BYTE tno);
extern BOOL RequestRIDNumber(BYTE tno);
extern BOOL RequestSubQInfo(void);
extern void BuildTOCFormat2(void);
extern void GetCapacity(void);
extern void ClearMaxLBA(void);
extern void WriteSDRAM_4B(ULONG);
extern void SetMaxLBA(long max_lba, BOOL flag);

#endif /* __APP_IF_H__ */
