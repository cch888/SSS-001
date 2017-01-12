/****************************************************************************
*                (c) Copyright 2001,2002,2003,2004 SUNEXT DESIGN, INC.
*                           All Right Reserved
*
* FILENAME: svo_cmd.h
*
* $Revision: 60 $
* $Date: 10/01/28 10:57a $
*
* DESCRIPTION
*   Servo Module global variable and constant definitions.
*
*
**************************** SUNEXT CONFIDENTIAL ****************************/

#ifndef _SVO_CMD_
#define _SVO_CMD_

/* player command */
#define CNOP        0xFFFF
#define COPEN       TRN_017
#define CCLOSE      TRN_015
#if (NEW_PIC_HOME_FLOW)
#define CSTART      TRN_002
#else
#define CSTART      TRN_003
#endif
#define CSTOP       TRN_014
#define CPAUSE      TRN_063
#define CRELSE      TRN_058
#define CSEEK       TRN_057
#if (BCA_ENABLE == 1)
#define CBCAJP      TRN_040
#endif
#define CJUMP       TRN_060
#define CVELO       TRN_048
#if (CALIBRATION_MANAGEMENT == 1)
#define CCALIB      TRN_052
#endif
#if (HORROR_DISC_IMPROVEMENT == 1)
#define CHGRFEQ     TRN_054
#endif
#define CLDAJ       TRN_004
#define CDIAG       TRN_053
#if (EN_LS == 1)
#define CLSSEEK     TRN_028
#endif // End of (EN_LS == 1)
#if (SERVICE_MODE==1)
/* Service servo commands */
#define SV_TRACK_OFF   TRN_017
#define SV_TRACK_ON    TRN_018
#define SV_FOCUS_OFF   TRN_019
#define SV_FOCUS_ON    TRN_020
#define SV_SPINDLE_OFF TRN_023
#define SV_SPINDLE_ON  TRN_024
//#define SV_LASER_OFF   TRN_023
//#define SV_LASER_ON    TRN_024
#endif

#define FEBG_ADJ        (00)
#if (EN_Persistent_table_8 == 1)
#define SPP2BG_ADJ      (01)//for new AP
#else
#define SPP2BG_ADJ      (01)//for new AP
#endif/*(EN_Persistent_table_8 == 1)*/
#define FEBC_TE_ADJ     (10)
#define FEBC_RRF_ADJ    (11)
#define FEBC_JIT_ADJ    (12)
#define RRFG_ADJ        (20)
#define BETA_ADJ        (30)
#define TILT_TE_ADJ     (40)
#define TILT_RRF_ADJ    (41)
#define TILT_JIT_ADJ    (42)
#define READ_BETA       (50)
#define MOVE_SLED_OUT   (150)
#define MOVE_SLED_IN    (151)

/* SIF */
#define STATE           0
#define EXECUTIONSTATE  1
#define COMMAND         2
#define PARAMETER1      3
#define PARAMETER2      4
#define PARAMETER3      5
#define PARAMETER4      6
#define PARAMETER5      7
#define PARAMETER6      8
#define PARAMETER7      9
#define PARAMETER8      10
#define PARAMETER9      11
#define PARAMETER10     12

/*******************************
       Prototype Functions
********************************/
void svoCmdDiscOutProc(void);
void svoCmdStartProc(void);
void svoCmdStopProc(void);
void svoCmdPauseProc(void);
void svoCmdSeekProc(void);
void svoCmdJumpGroovesProc(void);
void svoPowerOnInit(void);
void svoCmdSetSpeedProc(void);
void svoCmdGetStatusInformationProc(void);
void svoCmdGetErrorInformationProc(void);
void svoCmdVeloAdjustProc(void);
void svoCmdIllegalProc(char type);
BYTE svoCmdStatecheck(ServoStatesT *p);
void svoCmdDecode(void);
void set_SIF(BYTE type, BYTE code);
BYTE get_SIF(BYTE type);
BYTE disc_error_status_check(BYTE cmd);
void svoCmdReAdjustProc(void);
void svoCmdReCalibrateProc(void);
#if (FORCE_DISK_KIND == 1)
void svoCmdForceDiscKindProc(void);
#endif
void svoCmdAdjFocActDcActSensProc(void);
#if (HORROR_DISC_IMPROVEMENT == 1)
void svoCmdChgRFParameterProc(void);
#endif
#if(BCA_ENABLE == 1)
void svoCmdBCASeekProc(void);
#endif
#if (SERVICE_MODE==1)
void svoCmdLoopControlProc(void);
void svoCmdServiceModeProc(void);
#endif

#if CMD_DRY_READ == 1
void svoCmdDryReadProc(void);
#endif
#if CMD_SVO_CMD_READ_GPADC == 1
void svoCmdReadGpAdcProc(void);
#endif

#if TEST_AVERAGING_SERVO
void svoCmdTestAvgServoProc(void);
#endif

void svoCmdLDAdjustProc(void);
void svoCmdDiagnosticProc(void);
#if (EN_LS == 1)
void svoCmdLSSeekProc(void);
#endif // End of (EN_LS == 1)
void SetReadFilters(void);
void UpdateReadFilters(USHORT OverspeedX10);
void ChkUpdateReadFilters(void);

/*******************************
    External Functions
********************************/
extern void set_SIF(BYTE type, BYTE code);
extern BYTE get_SIF(BYTE type);
extern void svoCmdDecode(void);

/** External Variables **/
#if EN_LS == 1
extern BOOL  bLSDetected, bLSFOOROC;
#endif // End of (EN_LS == 1)

#endif /* _SVO_CMD_ */


/*------------------------------------------------------------------------*/
/* The end of file                                                        */
/*------------------------------------------------------------------------*/
