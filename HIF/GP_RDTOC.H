/****************************************************************************
*                (c) Copyright 2001 - 2005 SUNEXT DESIGN, INC.
*                           All Right Reserved
*
*
*
* FILENAME:   gp_rdtoc.h
*
* DESCRIPTION:
*   Include file for the read toc file
*
*   $Revision: 7 $
*   $Date: 08/07/29 8:00a $
*
************************ SUNEXT CONFIDENTIAL *******************************/

#ifndef __GP_RDTOC_H__
#define __GP_RDTOC_H__

extern void CDReadTOCCommand(void);
extern void UpdateTOCData(void);
extern void SaveTOCToSDRAM(void);
extern vBOOL CheckPlayerData(BYTE);
extern void ClearDiscAttributes(void);
extern void ClearCDType(void);

#define TOC_FORMAT_TOC          0x00
#define TOC_FORMAT_SESSIONINFO  0x10
#define TOC_FORMAT_FULLTOC      0x20
#define TOC_FORMAT_PMA          0x30
#define TOC_FORMAT_ATIP         0x40
#define TOC_FORMAT_CDTEXT       0x50


#endif /* __GP_RDTOC_H__ */
