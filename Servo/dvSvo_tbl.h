/****************************************************************************
*                (c) Copyright 2001 - 2007 SUNEXT DESIGN, INC.
*                           All Right Reserved
*
*
* FILENAME:   dvSVO_TBL.H
*
* DESCRIPTION
*
*   $Revision: 46 $
*   $Date: 08/12/24 5:16p $
*
**************************** SUNEXT CONFIDENTIAL ****************************/

#ifndef _dvSVO_TBL_
#define _dvSVO_TBL_

#define dvEQ_addr EQ_addr

extern const svoRegInitTblT (*const dvsvoRegInitTbl[MAX_SPEED_DVD+1]);
extern const SEEK_GAIN      (*const rdvsvokt1BKickTbl[MAX_SPEED_DVD]);
extern const SEEK_GAIN      (*const dvsvokt1BKickTbl[MAX_SPEED_DVD]);
#if (ACTIVE_SLED_FOR_FINE_JUMP == 0)
extern const SEEK_GAIN      (*const dvsvoks17KickTbl[2][MAX_SPEED_DVD]);
extern const SEEK_GAIN      (*const rdvsvoks17KickTbl[2][MAX_SPEED_DVD]);
#endif
extern const SEEK_GAIN      (*const rdvsvoKICKLVLTbl[MAX_SPEED_DVD]);
extern const SEEK_GAIN      (*const rdvsvoBRAKELVLTbl[MAX_SPEED_DVD]);
extern const SEEK_GAIN      (*const dvsvoKICKLVLTbl[MAX_SPEED_DVD]);
extern const SEEK_GAIN      (*const dvsvoBRAKELVLTbl[MAX_SPEED_DVD]);
extern const WORD           dvsvoBRAKESPDTbl[MAX_SPEED_DVD];
extern const WORD           rdvsvoBRAKESPDTbl[MAX_SPEED_DVD];
extern const BYTE           rdvsvoBRAKETIMETbl[MAX_SPEED_DVD];
extern const BYTE           dvsvoBRAKETIMETbl[MAX_SPEED_DVD];
extern WORD                 (*const dvEQ_addr[EQ_TBL_MAX+1]);
extern const svoWordU       (*const dvSvoEQCoefTbl[MAX_SPEED_DVD+1][EQ_TBL_MAX+1]);
extern const svoWordU       (*const rdvSvoEQCoefTbl[MAX_SPEED_DVD+1][EQ_TBL_MAX+1]);

#if (FINE_SEEK_SURF_TELFG == 0)
extern const svoWordU       (*const dvFineCEEQCoefTbl[MAX_SPEED_DVD]);
extern const svoWordU       (*const rdvFineCEEQCoefTbl[MAX_SPEED_DVD]);
#endif
extern const svoWordU       dvRoughCEEQ[];
extern const svoWordU       dvRoughCELFG[];

#endif /* _dvSVO_TBL_ */

/*------------------------------------------------------------------------*/
/* The end of file                                                        */
/*------------------------------------------------------------------------*/

