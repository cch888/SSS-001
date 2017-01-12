/*****************************************************************************
*           (c) Copyright 2000 - 2006 SUNEXT DESIGN, INC.
*                           All Right Reserved
*
*   FILE NAME:     $Workfile: dvUtil.h $
*
*   DESCRIPTION     Header File for dvUtil.c
*
*
*   $Revision: 8 $
*   $Date: 08/03/06 4:15p $
*
**************************** SUNEXT CONFIDENTIAL *******************************/

/*********************************************************************
    FILE DEFINITION
*********************************************************************/
#ifndef __DVDUTIL_H__
#define __DVDUTIL_H__

/*******************************************************************
    EXTERNAL FUNCTIONS / FUNCTION PROTOTYPES
*******************************************************************/
extern LONG  dvdConvertPSNtoLBA(ULONG);
extern PSNs  dvdConvertLBAtoPSN(LONG);
extern ULONG dvdGetBlockAddress(ULONG);

/********* Proto Types ***************/
BYTE DVDReturnMediaOffsetForRead(void);
#if PRN_DVD_DATABASE == 1
    extern void DisplayDvdDataBase();
#endif

#endif
