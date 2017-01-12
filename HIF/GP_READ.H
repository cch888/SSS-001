/****************************************************************************
*                (c) Copyright 2001,2002 2003, 2004 SUNEXT DESIGN, INC.
*                           All Right Reserved
*
*
*
* FILENAME:   gp_read.h
*
* DESCRIPTION:
*   This file contains command handler of read sub-channel command
*
* NOTES:
*
**************************** SUNEXT CONFIDENTIAL *******************************/

#ifndef __GP_READ_H__
#define __GP_READ_H__

extern void ReadCDCommand(void);
extern void ReadCDMSFCommand(void);
extern void CDRead10Command(void);
#define CDRead12Command   CDRead10Command
extern void VerifyCommand(void);
extern void SetReadAheadCommand(void);
extern void ProcessEjectRequest(void);

#endif /* __GP_READ_H__ */
