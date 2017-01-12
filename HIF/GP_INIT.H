/****************************************************************************
*                (c) Copyright 2001 - 2006 SUNEXT DESIGN, INC.
*                           All Right Reserved
*
*
*
* FILENAME:   gp_init.h
*
* DESCRIPTION
*   This file contains the initialization routine which is executed
*   on power or reset stage.
*
* NOTES:
*
*
* $Revision: 13 $
* $Date: 08/09/17 11:56p $
*
**************************** SUNEXT CONFIDENTIAL ****************************/

#ifndef __GP_INIT_H__
#define __GP_INIT_H__

extern void InitializeChip(void);
extern void InitializeTables(void);
extern void InitializeVariables(void);
extern void SetDefaultReadSpeed(void);
extern void InitializeReadSpeed(void);
extern void InitializeModeSense5DataForRDVD(void);
extern void InitCopyStartEndPages(void);

#endif /* __GP_INIT_H__ */
