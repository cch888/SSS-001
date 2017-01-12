/*****************************************************************************
*                  (c) Copyright 2000 - 2008 SUNEXT DESIGN, INC.
*                           All Right Reserved
*
* FILENAME:   dvCSS.h
*
* DESCRIPTION
* Header file for CSS.C (utility functions and error codes for CSS)
*
*   $Revision: 5 $
*   $Date: 08/02/12 3:32a $
*
**************************** SUNEXT CONFIDENTIAL *******************************/

#ifndef __DVCSS_H__
#define __DVCSS_H__

/*********************************************************************
 GLOBAL VARIABLES
*********************************************************************/
/****************************************************************************/
/* Header data length                           */
/****************************************************************************/
#define DVD_HDR_LEN         4           /*Number of Header byte*/

/****************************************************************************/
/*  Disc Key defines                                                        */
/****************************************************************************/
#define DISC_KEY_LEN        2052        /*Number of Disc Key data byte*/

/****************************************************************************/
/*  CSS defines                     */
/****************************************************************************/
/*8M Dram*/
/* #define   CSS_WK_ADR          0x7F0000    / *CSS dram addr*/
/*2M Dram*/
#define CSS_WK_ADR              0x1F2400    /*CSS dram addr*/

#define TITLE_KEY_OFFSET        CPR_MAI_OFFSET   /* BYTE OFFSET INTO SECTOR ENTRY IN AUX BLOCK */

extern void cssInit(void);
extern BYTE cssWait(void);
extern void CSSRset(void);
extern int cssValidateAgid(BYTE);
extern BOOL cssCommon(void);
extern void cssInvalidateAgid(void);
extern BYTE cssRegionMatch(void);
extern BYTE cssGetAgid(void);
extern void cssValidateRegion(void);
extern BYTE WaitForCssCompleted(void);
extern BYTE TitleKeyStatus();
#endif
