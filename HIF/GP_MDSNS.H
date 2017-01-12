/****************************************************************************
*                (c) Copyright 2001,2002,2003, 2004 SUNEXT DESIGN, INC.
*                           All Right Reserved
*
*
*
* FILENAME:   gp_mdsns.h
*
* DESCRIPTION
*   This file contains command handler for information commands
*
* NOTES:
*
* $Revision: 15 $
* $Date: 08/09/26 10:58a $
*
************************ SUNEXT CONFIDENTIAL *******************************/

#ifndef __GP_MDSNS_H__
#define __GP_MDSNS_H__

extern BYTE const Mode_sense_def_tbl[];
extern BYTE const mode_sense_def_1[];
extern BYTE const mode_sense_def_5[];
extern BYTE const mode_sense_def_5_for_RDVD[];
extern BYTE const mode_sense_def_8[];
extern BYTE const mode_sense_def_d[];
extern BYTE const mode_sense_def_e[];
extern BYTE const mode_sense_def_1a[];
extern BYTE const mode_sense_def_1d[];
extern BYTE const mode_sense_def_2a[];

#if (BD_ENABLE)
extern const USHORT SelBDReqSpeed_tbl[];
#endif  //(BD_ENABLE)

#if (ENABLE_HDDVD)
extern const USHORT SelHDReqSpeed_tbl[];
#endif  //(HD_ENABLE)

extern const USHORT SelDVDReqSpeed_tbl[];
extern const USHORT SelDVDReqWrSpeed_tbl[];
extern const USHORT SelCDReqSpeed_tbl[];

#if (EN_LS == 1)
extern BYTE const mode_sense_def_31[];
extern BYTE const mode_sense_def_32[];
#endif // End of (EN_LS == 1)

extern void ModeSelectCommand(void);
extern void ModeSenseCommand(void);
extern void CopySDRAMData(ULONG *, ULONG *, BYTE);
extern void ModeSenseCommand(void);
extern void SetCDSpeedCommand(void);
extern void CheckSelectedSpeed(BYTE md, USHORT cd_Speed );

#define RD      1
#define WR      0

#define AWRE    0x80
#define DCR     0x01

#define TestBitsModeP1_B2(MASK)   ((SCSDRAM[MSENSE_PAGE_1 + 2]&MASK)==MASK)
#endif /* __GP_MDSNS_H__ */
