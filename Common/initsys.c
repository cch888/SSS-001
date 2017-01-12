/****************************************************************************
*         (c) Copyright 2001 - 2010 Sunext Technology Co., Ltd.
*                           All Right Reserved
*
*
* FILENAME:   initsys.c
*
* DESCRIPTION
*   This file contains all modules used to initialize at start up.
*
*
* $Revision: 29 $
* $Date: 10/11/30 9:50a $
*
*
**************************** SUNEXT CONFIDENTIAL *******************************/

#include "..\common\globtype.h"

void InitializeSystem(void);
void InitializeIO(void);

#if (BOOT_CODE_SUPPORT == 1)
void BootMain(void);
#endif


/****************************************************************************
* Name:     init_system_first
*
* Description:  This function initializes the ARM microcontroller
*               It was called in startup asm code.
*****************************************************************************/
void init_system_first(void)
{
    InitializeSystem();
    InitializeIO();

#if (BOOT_CODE_SUPPORT == 1)
    BootMain();
#endif
}

/*****************************************************************************
*                       End Of File
*****************************************************************************/
