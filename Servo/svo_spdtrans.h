/****************************************************************************
*                (c) Copyright 2001,2002,2003,2004 SUNEXT DESIGN, INC.
*                           All Right Reserved
*
*
* FILENAME:   svo_spdtrans.h
*
* DESCRIPTION: Translation table from Speed to actual speed value
*
*   $Revision: 7 $
*   $Date: 09/08/28 12:39p $
*
**************************** SUNEXT CONFIDENTIAL ****************************/

#ifndef _SVO_TRANS_
#define _SVO_TRANS_

#include ".\Common\globals.h" /* Needed for types of parameters */

extern const USHORT SpeedIndexTableCD[];
extern const USHORT SpeedIndexTableDVD[];
extern const USHORT SpeedIndexTableDVDRAM[];
extern const USHORT SpeedIndexTableBD[];
extern const USHORT SpeedIndexTableHDDVD[];

#if (DVD_710Run815New == 1)
extern USHORT GetSpeedIndexTableDVD(BYTE spd);
#endif

#endif /* _SVO_TRANS_ */
/*------------------------------------------------------------------------*/
/* The end of file                                                        */
/*------------------------------------------------------------------------*/
