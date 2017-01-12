/****************************************************************************
*          (c) Copyright 2001 - 2010 Sunext Technology Co., Ltd.
*                           All Right Reserved
*
* FILENAME:   FWSigCheck.h
*
* DESCRIPTION
*
*
* $Revision: 4 $
* $Date: 11/04/26 1:40p $
*
**************************** SUNEXT CONFIDENTIAL *******************************/

#ifndef _FWSIG_CHECK_H_
#define _FWSIG_CHECK_H_

typedef BOOL (*SigChkFuncPtr)(void);

void DecryptIM(ULONG StAdrs, ULONG Length, BYTE *IvPtr);
void EncryptIM(ULONG StAdrs, ULONG Length);
void AACS_Unlock(BYTE *IvPtr);
BOOL FWSigChk(void);
BOOL StartFWSigChk(void);

#endif /* _FWSIG_CHECK_H_ */
