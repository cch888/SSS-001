/****************************************************************************
*                (c) Copyright 2001 - 2006 SUNEXT DESIGN, INC.
*                           All Right Reserved
* FILENAME      : plrSERVO.h
* DESCRIPTION   : This file contains the servo module definitions
*
*   $Revision: 11 $
*   $Date: 08/12/23 12:46a $
*
*
**************************** SUNEXT CONFIDENTIAL *******************************/
#ifndef _PLRSERVO_H_
#define _PLRSERVO_H_
/*************************************************************************/
/*                        Function Prototypes                            */
/*************************************************************************/
/* Servo command */
extern void plrServoSeek(long SeekTimeLBA, eSeekMode SeekMode, BYTE NextPhase);
extern void plrServoPause(BYTE Mode, BYTE NextPhase);
extern void plrServoSetSpeed(BYTE speed, BYTE NextPhase);
extern void plrServoReAdjust(discKindT DiscKind, BYTE NextPhase);
extern BYTE plrRealServoSeek(long SeekTimeLBA, eSeekMode SeekMode);
extern BYTE plrRealServoPause(BYTE Mode);

/* Inquiry command */
extern public void plrGetServoStatusInfo(void);

extern ExecutionStateT plrServoExecuteState(void);


#endif /* _PLRSERVO_H_ */
