/****************************************************************************
*                (c) Copyright 2001 - 2006  SUNEXT DESIGN, INC.
*                           All Right Reserved
*
*
* FILENAME:   BD_Diag.h
*
* DESCRIPTION
*   Header file for file Diag.c
*
*   $Revision: 9 $
*   $Date: 08/10/23 3:36p $
*
* NOTES:
*
**************************** SUNEXT  CONFIDENTIAL *******************************/

#ifndef _BD_Diag_H_
#define _BD_Diag_H_

#include ".\common\Globtype.h"
#include ".\common\Vu_cfg.h"

#define LDC_C       BlerLDC.word.l
#define LDC_UN_C    BlerLDC.word.h
#define LDC_ERRA    BlerERR.word.l
#define LDC_ERAS    BlerERR.word.h
#define BIS_C       BlerBIS.word.l
#define BIS_UN_C    BlerBIS.word.h
#define READ_LDC_BIS    BlerControl.byte.l
#define WAIT_LDC_BIS    BlerControl.byte.m
#define MX_DFT_LEN      BlerControl.byte.h  /* MXDFTLEN */
#define LDC_MAX_ERR     BlerControl.byte.u  /* LDCMAXERR */
#define LDC_AUN_ERRATA  BlerAun.byte.l
#define ERCO_CURRENT_ID BlerId        //Address



#if E_DIAGS
extern StLongU BlerLDC;
extern StLongU BlerERR;
extern StLongU BlerBIS;
extern StLongU BlerControl;
extern StLongU BlerId;
extern StLongU BlerAun;

#endif


#endif /* _BD_Diag_H_ */
