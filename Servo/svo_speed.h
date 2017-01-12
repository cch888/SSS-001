/****************************************************************************
*                (c) Copyright 2001 - 2007 SUNEXT DESIGN, INC.
*                           All Right Reserved
*
*
* FILENAME:   SVO_SPEED.H
*
* DESCRIPTION
*
*   $Revision: 87 $
*   $Date: 10/12/17 6:24p $
*
**************************** SUNEXT CONFIDENTIAL ****************************/

#ifndef _SVO_SPEED_
#define _SVO_SPEED_

#include ".\common\macros.h"
#include ".\al\REG_servo.h"
#include ".\common\iram.h"
#include ".\servo\svo_step.h"
#include ".\servo\svo_seek.h"
#include ".\servo\svoVar.h"
#include ".\servo\svo_tbl.h"
#include ".\servo\svo_lib.h"

/****************************************************************
        Prototype Functions
*****************************************************************/
void spdSetFGFreq(USHORT rpm);
    #if (DVD_RAM_READ == 1)
USHORT spdGetDVDRAMOverspeedX100(BYTE zone);
    #endif
USHORT spdGetTargetRPM(USHORT StepPos, ESpindleSpeed speed);
USHORT spdGetCurrentRPM(void);
void   spdDiscSpeedWaitStable(BYTE errorratio);
USHORT spdGetOverSpdX100(USHORT rpm, USHORT StepPosition);

/****************************************************************
    Externed variable
*****************************************************************/

/****************************************************************
    Externed Functions
*****************************************************************/
extern void   SpdInit(void);
extern void   spdStop(void);
extern void   spdChangeDiscSpeed(ESpindleSpeed speed);
extern void   spdHoldRPM(USHORT rpm);
extern void   spdUnhold(void);
extern void   spdSetMaxRPM(USHORT rpm);
extern void   spdUpdateRPM(USHORT stp_position);
extern void   spdSetPara(void);
extern void   spdSetCAVM(USHORT MinRpm);
extern void   spdSetKd14(SHORT kd14);
#if (NEW_SPINDLE_CONTROL == 1)
extern void   spdSetIntegrator(USHORT rpm);
extern void   spdAdaptIntegrator();
#endif
extern void   spdSetFGFreq(USHORT rpm);
extern USHORT spdGetTargetOverspeedX100(USHORT StepPos,ESpindleSpeed speed);
extern USHORT spdGetTargetRPM(USHORT StepPos, ESpindleSpeed speed);
extern USHORT spdGetCurrentRPM(void);
extern USHORT spdGetCavSwitchPos(ESpindleSpeed speed, USHORT* rpm);
extern void   spdDiscSpeedWaitStable(BYTE errorratio);
extern USHORT spdGetOverSpdX100(USHORT rpm, USHORT StepPosition);
extern BOOL   spdInCLVArea(SHORT step_pos);
extern void   spdMonitorRPM(void);
extern void   spdWaitSpdInRange(void);
#endif /* _SVO_SPEED_ */

/*------------------------------------------------------------------------*/
/* The end of file                                                        */
/*------------------------------------------------------------------------*/
