/****************************************************************************
*                (c) Copyright 2001 - 2007 SUNEXT DESIGN, INC.
*                           All Right Reserved
*
*
* FILENAME:   bdSVO_TBL.H
*
* DESCRIPTION
*
*   $Revision: 8 $
*   $Date: 08/12/24 5:15p $
*
**************************** SUNEXT CONFIDENTIAL ****************************/

#ifndef _bdSVO_TBL_
#define _bdSVO_TBL_

#define bdEQ_addr EQ_addr

extern const svoRegInitTblT (*const bdsvoRegInitTbl[MAX_SPEED_BD+1]);
extern const SEEK_GAIN      (*const bdrsvokt1BKickTbl[MAX_SPEED_BD]);
extern const SEEK_GAIN      (*const bdsvokt1BKickTbl[MAX_SPEED_BD]);
#if (ACTIVE_SLED_FOR_FINE_JUMP == 0)
extern const SEEK_GAIN      (*const bdsvoks17KickTbl[2][MAX_SPEED_BD]);
extern const SEEK_GAIN      (*const bdrsvoks17KickTbl[2][MAX_SPEED_BD]);
#endif
extern const SEEK_GAIN      (*const bdrsvoKICKLVLTbl[MAX_SPEED_BD]);
extern const SEEK_GAIN      (*const bdrsvoBRAKELVLTbl[MAX_SPEED_BD]);
extern const SEEK_GAIN      (*const bdsvoKICKLVLTbl[MAX_SPEED_BD]);
extern const SEEK_GAIN      (*const bdsvoBRAKELVLTbl[MAX_SPEED_BD]);
extern const BYTE           bdsvoBRAKESPDTbl[MAX_SPEED_BD];
extern const BYTE           bdrsvoBRAKESPDTbl[MAX_SPEED_BD];
extern const BYTE           bdrsvoBRAKETIMETbl[MAX_SPEED_BD];
extern const BYTE           bdsvoBRAKETIMETbl[MAX_SPEED_BD];
extern WORD                 (*const bdEQ_addr[EQ_TBL_MAX+1]);
extern const svoWordU       (*const bdSvoEQCoefTbl[MAX_SPEED_BD+1][EQ_TBL_MAX+1]);
extern const svoWordU       (*const bdrSvoEQCoefTbl[MAX_SPEED_BD+1][EQ_TBL_MAX+1]);

#if (FINE_SEEK_SURF_TELFG == 0)
extern const svoWordU       (*const bdFineCEEQCoefTbl[MAX_SPEED_BD]);
extern const svoWordU       (*const bdrFineCEEQCoefTbl[MAX_SPEED_BD]);
#endif
extern const svoWordU       bdRoughCEEQ[];
extern const svoWordU       bdRoughCELFG[];

#endif /* _bdSVO_TBL_ */

/*------------------------------------------------------------------------*/
/* The end of file                                                        */
/*------------------------------------------------------------------------*/

