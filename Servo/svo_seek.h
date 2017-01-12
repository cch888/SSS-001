/****************************************************************************
*                (c) Copyright 2001 - 2007 SUNEXT DESIGN, INC.
*                           All Right Reserved
*
*
* FILENAME:   SVO_SEEK.H
*
* DESCRIPTION
*
*   $Revision: 105 $
*   $Date: 11/01/06 11:14a $
*
**************************** SUNEXT CONFIDENTIAL ****************************/

#ifndef _SVO_SEEK_
#define _SVO_SEEK_

#define PRESET_TARGET_LAYER_FCSG       0

#define INTERFACE_DELAY_DIST_CDROM     3         // (frames)
#define INTERFACE_DELAY_DIST_CDR_W     3         // (frames)
#define INTERFACE_DELAY_DIST_DVDROM    5         // (sectors)
#define INTERFACE_DELAY_DIST_DVDR_W    5         // (sectors)
#define INTERFACE_DELAY_DIST_DVDRAM    5         // (sectors)
#if (ENABLE_HDDVD == 1)
#define INTERFACE_DELAY_DIST_HDROM     5         //
#define INTERFACE_DELAY_DIST_HDR_W     5         //
#endif
#if (BD_ENABLE == 1)
#define INTERFACE_DELAY_DIST_BDROM     5         // (sectors)
#define INTERFACE_DELAY_DIST_BDR_W     5         // (sectors)
#endif

//range 0~255
#define INTERFACE_DELAY_TIME_CDROM     70        // (100uS)
#define INTERFACE_DELAY_TIME_CDR_W     70        // (100uS)
#define INTERFACE_DELAY_TIME_DVDROM    42        // (100uS)
#define INTERFACE_DELAY_TIME_DVDR_W    50        // (100uS)
#define INTERFACE_DELAY_TIME_DVDRAM    20        // (100uS)
#if (ENABLE_HDDVD == 1)
#define INTERFACE_DELAY_TIME_HDROM     42        // (100uS)
#define INTERFACE_DELAY_TIME_HDR_W     50        // (100uS)
#endif
#if (BD_ENABLE == 1)
#define INTERFACE_DELAY_TIME_BDROM     42        // (100uS)
#define INTERFACE_DELAY_TIME_BDR_W     90        // (100uS)
#endif

#define JUMP_DELAY_FIX_CD              55        // (100uS)
#define JUMP_DELAY_PER_TRACK_CD        75        // (2uS)
#define PLL_LOCK_LENGTH_CDROM          4         // (frames)
#define PLL_LOCK_LENGTH_CDR_W          8         // (frames)

#define JUMP_DELAY_FIX_DVD             65        // (100uS)
#define JUMP_DELAY_PER_TRACK_DVDROM    55        // (2uS)
#define JUMP_DELAY_PER_TRACK_DVDR_W    55        // (2uS)
#define JUMP_DELAY_PER_TRACK_DVDRAM    75        // (2uS)
#define PLL_LOCK_LENGTH_DVDROM         5         // (sectors)
#define PLL_LOCK_LENGTH_DVDR_W         24        // (sectors)
#define PLL_LOCK_LENGTH_DVDRAM         10        // (sectors)

#if (ENABLE_HDDVD == 1)
#define JUMP_DELAY_FIX_HD              65        // (100uS)
#define JUMP_DELAY_PER_TRACK_HDROM     10        // (100uS)
#define JUMP_DELAY_PER_TRACK_HDR_W     10        // (100uS)
#define PLL_LOCK_LENGTH_HDROM          6         // (sectors)
#define PLL_LOCK_LENGTH_HDR_W          24        // (sectors)
#endif

#if (BD_ENABLE == 1)
#define JUMP_DELAY_FIX_BDROM           50        // (100uS)
#define JUMP_DELAY_FIX_BDR_W           65        // (100uS)
#define JUMP_DELAY_PER_TRACK_BDROM     60        // (100uS)
#define JUMP_DELAY_PER_TRACK_BDR_W     55        // (100uS)
#define PLL_LOCK_LENGTH_BDROM          40        // (sectors)
#define PLL_LOCK_LENGTH_BDR_W          35        // (sectors)

#define PBA_BDLAYER1_MASK              0x01000000 /* Flag of layer1 (yet only 1 layer) */
#endif

/* Prototype */
USHORT   svoPBA2Radius(LONG pba);
LONG     svoRadius2PBA(USHORT radius,BYTE layer);
BYTE     svoSectorsPerRev(USHORT radius);
LONG     svoSectors2Duration(LONG sectors);
LONG     svoDuration2Sectors(LONG duration);
void     seekSetCEEq(eTrackEQType type, BYTE gain_ratio);
void     seekSetSeekParas(void);
void     seekSetFineJumpParas(USHORT TracksToGo);

#endif /* _SVO_SEEK_ */

/*------------------------------------------------------------------------*/
/* The end of file                                                        */
/*------------------------------------------------------------------------*/
