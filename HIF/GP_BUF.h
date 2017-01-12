/************************** SUNEXT CONFIDENTIAL *********************************
*                (c) Copyright 1998 - 2003, 2004 SUNEXT TECHNOLOGY, INC.
*                            All Rights Reserved
*
*
*
*   FILE NAME:     $Workfile: GP_BUF.h $
*
*   DESCRIPTION:    Header file for file Gp_Buf.c
*
*
*   FUNCTIONS:
*
*   $Revision: 8 $
*   $Date: 11/02/25 11:42a $
*
************************** SUNEXT CONFIDENTIAL *********************************/
#ifndef __GP_BUF_H__
#define __GP_BUF_H__

#if (SUN_FUNC_TEST == 0)
/* Command description equates */
#define BUFFER_MODE_HDR_DATA        0x00
#define BUFFER_MODE_DATA			0x02
#define	BUFFER_MODE_DESCRIPTOR		0x03

#define BUFFER_ID_0					0x00

#define RDBUFFER_LEN_BYTE			0x04
#define RDBUFFER_LEN_WORD			0x02

/* Externals */
extern void ReadBufferCommand(void);
extern void WriteBufferCommand(void);
#endif
#endif
