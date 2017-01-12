/****************************************************************************
*                (c) Copyright 2001,2002,2003,2004 SUNEXT DESIGN, INC.
*                           All Right Reserved
*
*
*
* $Revision: 71 $
* $Date: 11/03/28 4:07p $
*
* DESCRIPTION
*
*
**************************** SUNEXT CONFIDENTIAL ****************************/

#ifndef _SVO_TBL_
#define _SVO_TBL_

/* table end code */
#define SVOREGTBL_END   0xFFFF      /* Servo Register end address */
#define EQ_END          0xFFFF      /* for EQ */

typedef enum    {BIT_OFF = 0, BIT_ON, BYTESET, WORDSET} eSvoRegInitMode;

typedef struct{
    ULONG           addr;
    USHORT          data;
    eSvoRegInitMode mode;
}svoRegInitTblT;

/* Fine Seek */
typedef struct{
    USHORT  jump_count;
    USHORT  gain_data;
}SEEK_GAIN;

extern const svoRegInitTblT (*const svoRegInitTbl[MAX_SPEED_CD+1]);
extern const SEEK_GAIN      (*const svokt1BKickTbl[MAX_SPEED_CD]);
#if (ACTIVE_SLED_FOR_FINE_JUMP == 0)
extern const SEEK_GAIN      (*const svoks17KickTbl[2][MAX_SPEED_CD]);
#endif
extern const SEEK_GAIN      (*const svoKICKLVLTbl[MAX_SPEED_CD]);
extern const SEEK_GAIN      (*const svoBRAKELVLTbl[MAX_SPEED_CD]);
extern const WORD           svoBRAKESPDTbl[MAX_SPEED_CD];
extern const BYTE           svoBRAKETIMETbl[MAX_SPEED_CD];
extern WORD                 (*const EQ_addr[EQ_TBL_MAX+1]);
extern const WORD           Ce_Eq[];
extern const WORD           Ce_Low_Eq[];
extern const WORD           Ce_Lpf_Eq[];
extern const WORD           Tracking_U_Eq[];
#if (SUPPORT_DPD == 1) || (CD_MB_ONLY_OPU == 1)
extern const WORD           Tracking_Eq[];
#endif
extern const WORD           Focus_U_Eq[];
extern const WORD           Tracking_W_Eq[];
extern const WORD           Focus_W_Eq[];
extern const svoWordU       (*const svoEQCoefTbl[MAX_SPEED_CD+1][EQ_TBL_MAX+1]);
extern const svoWordU       (*const FineCEEQCoefTbl[MAX_SPEED_CD]);
extern const svoWordU       RoughCEEQ[];
extern const svoWordU       RoughCELFG[];
#if (CHIP_REV >= 0xC0)
extern const svoWordU       CELPF_100Hz[];
extern const svoWordU       CELPF_1kHz[];
//extern const WORD           CELPF_allpass[];
#endif

/*------------------------------------------------------------------------*/
/* including servo parameters based on of project                         */
/*------------------------------------------------------------------------*/
#if (PCB == 690)
#include ".\servo\svo_para_690.h"
#include ".\servo\svo_coef_690.h"
#elif (PCB == 692)
#include ".\servo\svo_para_692.h"
#include ".\servo\svo_coef_692.h"
#elif (PCB == 815)
#include ".\servo\svo_para_815.h"
#include ".\servo\svo_coef_815.h"
#elif (PCB == 710)
#include ".\servo\svo_para_710.h"
#include ".\servo\svo_coef_710.h"
#elif (PCB == 711)
#include ".\servo\svo_para_711.h"
#include ".\servo\svo_coef_711.h"
#elif (PCB == 812)
#include ".\servo\svo_para_812.h"
#include ".\servo\svo_coef_812.h"
#elif (PCB == 1600)
#include ".\servo\svo_para_1600.h"
#include ".\servo\svo_coef_1600.h"
#elif (PCB == 691)
#include ".\servo\svo_para_691.h"
#include ".\servo\svo_coef_691.h"
#elif (PCB == 817)
#include ".\servo\svo_para_817.h"
#include ".\servo\svo_coef_817.h"
#endif // PCB
#endif /* _SVO_TBL_ */


/*------------------------------------------------------------------------*/
/* The end of file                                                        */
/*------------------------------------------------------------------------*/
