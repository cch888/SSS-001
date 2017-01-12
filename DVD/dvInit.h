/****************************************************************************
*                (c) Copyright 2001,2002,2003, 2004 SUNEXT DESIGN, INC.
*                           All Right Reserved
*
*
* FILENAME:     dvInit.h
*
* DESCRIPTION   Header file for file dvInit.c
*
*
* $Revision: 22 $
* $Date: 08/12/24 9:47a $
*
**************************** SUNEXT CONFIDENTIAL ****************************/

#ifndef __DVINIT_H__
#define __DVINIT_H__
#if (ENABLE_HDDVD)
#include ".\hd\hdInit.h"
#endif

/************************************************
 Defines
*************************************************/


/************************************************
 External functions
*************************************************/

#if (DVD_AWG_TEST == TRUE)
extern void DvdAwgInit(void);
#endif

extern void dvdDspSystemInit(void);
extern void dvdDspDiscDetectedInit(void);
extern void DvdVarsInit(void);
extern void dvdStartUpSuccessful(void);
extern BOOL dvdVerifyDisc(void);
extern BYTE dvdDiscSetup(void);
#endif
