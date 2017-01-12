/****************************************************************************
*                (c) Copyright 2001,2002,2003,2004 SUNEXT DESIGN, INC.
*                           All Right Reserved
*
*
* FILENAME:   gp_rdcd.h
*
* DESCRIPTION
*    This file contains read raw r-w subcode and read cdda data routine
*
* NOTES:
*
**************************** SUNEXT CONFIDENTIAL *******************************/

#ifndef __GP_RDCD_H__
#define __GP_RDCD_H__

extern void ReadAudioSubCode(void);
extern void ReadCDDAData(void);
extern void StopCDDADecoder(void);
extern void EnableCDDABuffering(void);


#endif /* __GP_RDCD_H__ */
