/****************************************************************************
*                (c) Copyright 2001,2002,2003,2004 SUNEXT DESIGN, INC.
*                           All Right Reserved
*
*
*
* FILENAME:   gp_spin.h
*
* DESCRIPTION:
*   This file contains command handler of ATAPI command
*
* NOTES:
*
************************ SUNEXT CONFIDENTIAL *******************************/

#ifndef __GP_SPIN_H__
#define __GP_SPIN_H__

extern void TestUnitReadyCommand(void);
extern void StartStopUnitCommand(void);
extern void PauseResumeCommand(void);
extern void PreventAllowMediumRemovalCommand(void);
extern void TrayinInitial(void);
extern void StartUpSuccessful(void);
extern BYTE UpdateDiscTypeSpeed(void);
extern void CheckForSpecialDisc(void);
//add Pre-do DPD calibration after change speed.
#if (DPD_COMPENSATE_OFFSET==1)
extern void ServoCalibDpdWholeDisc(void);//improve whole disc pre calibration
#endif
#if (CALIBRATION_MANAGEMENT == 1)
extern void ServoCalibWholeDisc(void);
#endif

#if(EN_DVD_HIGH_JITTER_DETECT ==1)
extern void JUDGE_HIGH_JITTER_DISC(void);
#endif

#endif /* __GP_SPIN_H__ */
