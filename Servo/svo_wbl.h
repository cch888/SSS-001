/****************************************************************************
*                (c) Copyright 2000 - 2007 SUNEXT DESIGN, INC.
*                            All Rights Reserved
*
*
*
* FILENAME:     svo_wbl.h
*
* DESCRIPTION   This file is the header file for file Wobble.h
*
*
* $Revision: 40 $
* $Date: 08/09/18 12:15a $
*
**************************** SUNEXT CONFIDENTIAL *******************************/
#ifndef __SVO_WBL__
#define __SVO_WBL__


/****************************************************************
     Prototype
*****************************************************************/
#if (EN_WOBBLE == 1)
extern void UpdateWobbleFilters(USHORT OverspeedX10);

/****************************************************************
    Externed Functions
*****************************************************************/
extern void DvdInitWobbleDecoder(ESpindleSpeed Speed, long lba);
extern void IDValidIntProc(void);
extern void IDSyncLostIntProc(void);
extern void IDSynIntProc(void);

#endif // (EN_WOBBLE == 1)
#endif
