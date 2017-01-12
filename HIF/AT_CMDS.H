/****************************************************************************
*                (c) Copyright 2001,2002,2003, 2004 SUNEXT DESIGN, INC.
*                           All Right Reserved
*
*
* FILENAME:   at_cmd.h
*
* DESCRIPTION
*   This file contains command handler of AT commands
*
* NOTES:
*
**************************** SUNEXT CONFIDENTIAL *******************************/
#ifndef __AT_CMDS_H__
#define __AT_CMDS_H__

extern void ataCheckPowerModeCommand(void);
extern void ataSleepCommand(void);
extern void ataStandbyImmedCommand(void);
extern void ataIdleImmedCommand(void);
extern void ataSetFeatureCommand(void);
extern void ATACommandComplete(void);
extern void ATACommandFail(void);
extern void ataStandbyCommand(void);
extern void ataGetMediaStatus(void);
extern void ataFlushCacheCommand(void);
#if (CHIP_REV < 0xC0)  
extern void ManualATACommandComplete(void);
#endif

//extern void ManualATACommandComplete(void);

/* Vendor SATA-ATA Command */
extern void VendorATASetFeatureCommand(void);
extern void VendorATAStandbyImmedCommand(void);

#endif /* __AT_CMDS_H__ */
