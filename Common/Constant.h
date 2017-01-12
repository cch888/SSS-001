/****************************************************************************
*                (c) Copyright 2001,2002 2003, 2004, 2005 SUNEXT DESIGN, INC.
*                           All Right Reserved
*
*
* FILENAME:   CONSTANT.H
*
* DESCRIPTION
*   This file contains all global definitions be used by the creators
*   of a CD-R/RW application.
*
* NOTES:
*
**************************** SUNEXT CONFIDENTIAL *******************************/


#ifndef __CONSTANT_H__
#define __CONSTANT_H__

#define WR_METHOD_TAO   1               /* track at once */
#define WR_METHOD_FP    2               /* fixed packet */
#define WR_METHOD_VP    3               /* variable packet */
#define WR_METHOD_BLK   0x0F            /* blank disc */

/* return this value instead of return(TRKNO_NOTVALID) in audio disc */
#define AUDIO_TRK			0x0E			/* audio track */
#define TRKNO_NOTFOUND		0xFD			/* data is not found in any track */

#define TRKNO_NOTVALID  0xFF            /* track number not valid */
#define TRKNO_INITVALID 0xAA           /* track number initial valid *///  modify for read Q time-out time

#define AUD_IN_PROGRESS 0x11
#define AUD_PAUSED      0x12
#define AUD_COMPLETED   0x13
#define AUD_ERR_STOP    0x14
#define AUD_NO_STATUS   0x15

#define SUBC_MODE_MASK  0x0F            /* mask subc-mode out of conad BYTE */
#define SUBC_MODE_0     0x00            /* erased                           */
#define SUBC_MODE_1     0x01            /* normal                           */
#define SUBC_MODE_2     0x02            /* catalog number and disc_id       */
#define SUBC_MODE_3     0x03            /* ISRC number                      */
#define SUBC_MODE_5     0x05            /* multi session subc leadin/out    */

#endif /* __CONSTANT_H__ */
