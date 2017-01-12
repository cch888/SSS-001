/****************************************************************************
*                (c) Copyright 2001,2002,2003, 2004 SUNEXT DESIGN, INC
*                           All Rights Reserved
*
*
*
* FILENAME:   gp_inqry.h
*
* DESCRIPTION:
*   This file contains command handler for information commands
*
* NOTES:
*
**************************** SUNEXT CONFIDENTIAL *******************************/

#ifndef __GP_INQRY_H__
#define __GP_INQRY_H__

extern BYTE const Identify_tbl[];
extern BYTE const Inquiry_tbl[];
extern BYTE const Request_sense_tbl[];

extern void ReadCapacityCommand(void);
extern void ataIdentifyPacketDeviceCommand(void);
extern void InquiryCommand(void);
extern void RequestSenseCommand(void);
extern void VendorATAIdentifyPacketDeviceCommand();
extern ULONG ErasecurPSN;
#endif /* __GP_INQRY_H__ */
