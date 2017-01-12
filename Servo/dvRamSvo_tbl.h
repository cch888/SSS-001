/****************************************************************************
*                (c) Copyright 2001 - 2007 SUNEXT DESIGN, INC.
*                           All Right Reserved
*
*
* FILENAME:   dvRamSVO_TBL.H
*
* DESCRIPTION
*
*   $Revision: 1 $
*   $Date: 09/12/10 6:33p $
*
**************************** SUNEXT CONFIDENTIAL ****************************/

#ifndef _dvRamSVO_TBL_
#define _dvRamSVO_TBL_

extern const svoRegInitTblT (*const dvRamSvoRegInitTbl[MAX_SPEED_DVDRAM+1]);
extern const SEEK_GAIN      (*const dvRamSvoKICKLVLTbl[MAX_SPEED_DVDRAM]);
extern const SEEK_GAIN      (*const dvRamSvoBRAKELVLTbl[MAX_SPEED_DVDRAM]);
extern const SEEK_GAIN      (*const dvRamSvokt1BKickTbl[MAX_SPEED_DVDRAM]);
#if (ACTIVE_SLED_FOR_FINE_JUMP == 0)
extern const SEEK_GAIN      (*const dvRamSvoks17KickTbl[2][MAX_SPEED_DVDRAM]);
#endif
extern const WORD           dvRamSvoBRAKESPDTbl[MAX_SPEED_DVDRAM];
extern const BYTE           dvRamSvoBRAKETIMETbl[MAX_SPEED_DVDRAM];

extern const svoWordU       (*const dvRamSvoEQCoefTbl[MAX_SPEED_DVDRAM+1][EQ_TBL_MAX+1]);
#if (FINE_SEEK_SURF_TELFG == 0)
extern const svoWordU       (*const dvRamFineCEEQCoefTbl[MAX_SPEED_DVDRAM]);
#endif
extern const svoWordU       dvRamRoughCEEQ[];
extern const svoWordU       dvRamRoughCELFG[];

#endif /* _dvRamSVO_TBL_ */

/*------------------------------------------------------------------------*/
/* The end of file                                                        */
/*------------------------------------------------------------------------*/

