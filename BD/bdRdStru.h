/*****************************************************************************
*                (c) Copyright 2000 - 2007 SUNEXT DESIGN, INC.
*                            All Rights Reserved
*
*
* FILENAME:     $Workfile: bdRdStru.h $
*
* DESCRIPTION   Header File for bdRdStru.c
*
* $Revision: 11 $
* $Date: 09/05/19 4:26p $
*
**************************** SUNEXT CONFIDENTIAL ****************************/

#ifndef __BDRDSTRU_H__
#define __BDRDSTRU_H__

/*********************************************************************
   FUNCTION PROTOTYPES
*********************************************************************/
extern void ReadBDStructureCommand(void);
extern BYTE CheckPACHeader(BYTE *HeaderType);
extern BYTE BDReadBCA(void);
extern BYTE BDReadDFL(void);
/*********************************************************************
        Physical define
*********************************************************************/

/*********************************************************************
        Global Structures
*********************************************************************/
#if(BCA_ENABLE == 1)
#define BCA_TARGET_BD UM_2_SLED_STEP(21600)  //define bd read BCA radius to 21.6mm; //BCA area from 21.3mm to 22mm
#define bdBCASeek   0xff

#endif

#endif
