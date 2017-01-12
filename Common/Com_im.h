/****************************************************************************
*                (c) Copyright 2001,2002,2003, 2004, 2005 SUNEXT DESIGN, INC.
*                           All Right Reserved
*
* FILENAME:   COM_IM.H
*
* DESCRIPTION
*
*
* CHANGES:
* REF VERSION DATE      WHO         DETAIL
* --- ------- --------- ----------  ---------------------------------------------
*       1.00  Aug-01-02 YaoC        Create
**************************** SUNEXT CONFIDENTIAL ****************************/


#ifndef _COM_IM_H_
#define _COM_IM_H_
extern BYTE   HEX2BCD(BYTE hex);
extern TimeS  BCD2MSF(TimeS *bcd); /* BCD time -> MSF time */
extern TimeS  MSF2BCD(TimeS *msf); /* MSF time -> BCD time */
extern long   MSF2Count(TimeS *msf);
extern long   MSF2ADR(TimeS *msf);  /* MSF time -> ADDRESS  */
extern TimeS  LBA2MSF(long lba);
extern BOOL   CheckBCDTime(TimeS *bcd); /* Check BCD time       */
    #if (DVD_RAM_READ == 1)
extern BYTE   DVDRAM_LSN2ZONE(ULONG LSN);
extern BYTE   DVDRAM_PSN2ZONE(ULONG PSN);
extern ULONG  *SearchSdlEntry(LONG PsnValue);
extern ULONG  *SearchPdlEntry(LONG PsnValue);
extern ULONG  ConvertLSNToPSN(ULONG LsnValue);
extern ULONG  ConvertPSNToLSN(ULONG);
extern USHORT CalcPDLNumberBetween2PSN(ULONG StartPSN,ULONG EndPSN);
    #endif
#endif /* _COM_IM_H_ */
