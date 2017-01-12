/******************************************************************************
*                (c) Copyright 2001 - 2006 SUNEXT DESIGN, INC.
*                           All Right Reserved
*
*
*
* $Revision: 33 $
* $Date: 11/03/28 2:37p $
*
* DESCRIPTION:  Provides visibility to functions and variables interfacing with
*               the servo module
*
*
**************************** SUNEXT CONFIDENTIAL *******************************/

#ifndef __SV_UTIL4_H__
#define __SV_UTIL4_H__
#include ".\servo\SVO.H"
extern void ErrorLED(BYTE led_status);
extern void ReadyLED(BYTE led_status);
extern void WriteLED(BYTE led_status);
extern BOOL ServoTrayIn(void);
extern BOOL ServoTrayOut(void);
extern BOOL ServoPassThrough(void);                    // Support for Player command Servo Pass Through.
extern BOOL ServoStartUp(void);
extern BOOL ServoSeek(long   seek_lba,BYTE mode);
extern BOOL ServoStop(void);
extern BOOL ServoPause(void);
extern BOOL ServoReadSubcode(BYTE mode,BYTE req_tno);
extern void ServoSaveSubcode(void);
extern BOOL ServoSearchISRCAndCatalog(BYTE tno,BYTE req);
extern BOOL svr_set_to_highest_speed(void);
extern BOOL ServoSetSpeed(BYTE speed);
extern BOOL SetReadSpeed(BYTE speed);
extern BOOL ServoCalibrate(void);
extern BOOL ServoSeekToLeadIn(void);
extern BOOL ServoReAdjust(discKindT ReAdjKind);
#if (CALIBRATION_MANAGEMENT == 1)
extern BOOL ServoReCalibrate(void);
#endif /* CALIBRATION_MANAGEMENT */
#if (HORROR_DISC_IMPROVEMENT == 1)
extern BOOL ServoChgRFParameter(void);
#endif
#if (P_DIAGS == 1)
extern BOOL ServoForceDiscKind(BYTE flag, USHORT disc_kind);
#endif
#if ((PCB == 690)||(PCB == 692)||(PCB == 710)||(PCB == 711)||(PCB == 812)||(PCB == 830)||(PCB == 1600)||(PCB == 815)||(PCB == 817))
extern BOOL ServoAdjFocDcActSens(BYTE disc_type, BYTE ExeCmd);
#endif
extern BOOL ServoCalibration(BYTE calib_no);
#if (DETECT_ECC_DISC == 1) || (DVD_UNBALANCE_DISC_CHECK == 1) || (CD_UNBALANCE_DISC_CHECK == 1)
extern BYTE CheckUnbalancedEccentricityDisc(BYTE Curspeed);
#endif
extern BOOL ServoAdjustWritePowerStart(void);
extern vBOOL WaitPlayerReady(void);
extern BOOL GetHIFToPlayerStatus(void);
extern BOOL ServoWriteStart(BYTE mode);
extern void SupportMaximumSpeed(void);
extern BYTE CheckSpeedLimit(BYTE);
extern const USHORT data_tran_speed_DVD[];
extern const USHORT data_tran_speed_DVDRAM[];
extern BOOL Redo_unb_needed;
extern BOOL ServoBCASeek(SHORT steps, BYTE flag);
#endif /* __SV_UTIL4_H__ */


/*------------------------------------------------------------------------*/
/* The end of file                                                        */
/*------------------------------------------------------------------------*/
