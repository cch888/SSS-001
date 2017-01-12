/****************************************************************************
*                (c) Copyright 2001 - 2007 SUNEXT DESIGN, INC.
*                           All Right Reserved
*
*
* FILENAME:   SVO_STEP.H
*
* DESCRIPTION
*
*   $Revision: 92 $
*   $Date: 09/05/25 2:39p $
*
**************************** SUNEXT CONFIDENTIAL ****************************/

#ifndef _SVO_STEP_
#define _SVO_STEP_

typedef enum _eStepMode
{
    STEP_NULL_MODE = 0,
    STEP_STOP_MODE,
    STEP_HOMING_MODE,
    STEP_FULL_MODE,
    STEP_SAFE_SLOW_MODE,
    STEP_NORMAL_MODE,
    STEP_NO_CE_SLOW_MODE,
    STEP_SEARCH_RF_MODE,
#if(Customer_A1==1)
		STEP_HOMING2_MODE,
#endif		
    STEP_NEW_HOME_MODE,
    STEP_MODE_MAX = STEP_NEW_HOME_MODE,
#if COLLI_SWITCH == 1
    COLLI_NORMAL_MODE,
    COLLI_MICRO_MODE,
    COLLI_NORMAL_NO_REST_MODE,
    COLLI_MICRO_NO_REST_MODE,
    COLLI_NEW_HOME_MODE,
    COLLI_MODE_MAX = COLLI_NEW_HOME_MODE,
#endif
} eStepMode;

typedef enum _eStepProfile
{
    STEP_HOMING_PROFILE = 0,
    STEP_FULL_PROFILE,
    STEP_SAFE_SLOW_PROFILE,
    STEP_NORMAL_PROFILE,
    STEP_NORMAL_SHORT_PROFILE,
    STEP_NO_CE_SLOW_PROFILE,
#if COLLI_SWITCH == 1
    COLLI_NORMAL_PROFILE,
    COLLI_MICRO_PROFILE,
#endif
#if(Customer_A1==1)	
    STEP_HOMING2_PROFILE,
#endif    
    PROFILE_MAX
} eStepProfile;

//--------------------------------------;
// define type for STEP MOTOR SUPPORT   ;
//--------------------------------------;
typedef struct _HW_speed_profile
{
    BOOL   accele_phase;
    BOOL   decele_phase;
    SHORT  profile_speed;
} HW_speed_profile_t;

typedef enum _eStepTimerMode
{
    STEP_DISABLE_TIMER     = 0x00,
    STEP_READ_TIMER        = 0x01,
    STEP_ROUGH_TIMER       = 0x02,
    STEP_ROUGH_TIMER_READ  = 0x03,
    STEP_REST_TIMER        = 0x04,
#if (ACTIVE_SLED_FOR_FINE_JUMP == 0)
    STEP_FINE_TIMER        = 0x05,
    STEP_ROUGH_TIMER_START = STEP_ROUGH_TIMER,
    STEP_ROUGH_TIMER_STOP  = STEP_DISABLE_TIMER,
#else
    STEP_ROUGH_TIMER_START = 0x80,
    STEP_ROUGH_TIMER_STOP  = 0x81,
#endif
} eStepTimerMode;

#define STP_BUSY_MASK       0x01
#define STP_DEVICE_MASK     0x02
#define STP_DEVICE_NORMAL   0x00
#define STP_DEVICE_COLLI    0x02

#define svoStp_busy()       (Iram_stp_Status & STP_BUSY_MASK)

typedef enum _eInitialized
{
    STEP_UNINITIALIZED = 0,
    STEP_INITIALIZED
} eInitialized;

/* Prototypes */
#if (COLLI_SWITCH == 1)
#define svoStp_JumpSteps(X,Y) svoStp_JumpToPosition(X, ((X > STEP_MODE_MAX)? Iram_colli_CurrentPosition : Iram_stp_CurrentPosition) + (Y))
#else
#define svoStp_JumpSteps(X,Y) svoStp_JumpToPosition(X, Iram_stp_CurrentPosition + (Y))
#endif

//*****************************************************************************
//*                                                                           *
//*     micro step per ele. rev = 128(SW),64(HW)                              *
//*     distance per mach. rev  = 3 (mm)                                      *
//*     ele. rev. per mach. rev = 5                                           *
//*                                                                           *
//*****************************************************************************
#define UM_2_SLED_STEP(X) ((X) * MICRO_STEP_PER_ELE_REV * ELE_REV_PER_MACH_REV / DISTANCE_PER_MACH_REV)
#define SLED_STEP_2_UM(X) ((X) * DISTANCE_PER_MACH_REV / (MICRO_STEP_PER_ELE_REV * ELE_REV_PER_MACH_REV))
void svoStp_Init( void );
void svoStp_JumpToPosition( eStepMode StepMode, SHORT position );
void svoStp_HoldSledge(void);
void svoStp_MoveStep( LONG steps );
void svoStp_JumpControl( void );
void svoStp_SetGain( BYTE gain );
void svoStp_SetCurrentPosition(USHORT CurrentPosition);
void svoStp_SetTimerMode( eStepTimerMode TimerMode );
void svoStp_StopControl( void );
#if (TRAVERSE_TEST == 1 )
void svoStp_Traverse(BYTE,BYTE);
#endif

#if DO_HW_SUPPORT_STEP == 1
void FillMicroSteppingTable(SHORT);
extern vSHORT StepMotorSupport_SLED;
#endif //if DO_HW_SUPPORT_STEP == 1

/**********************************************
        External Variables
***********************************************/
extern vBYTE            Iram_stp_CurrentDirection;
extern vUSHORT          Iram_stp_JumpDistance;
extern vUSHORT          Iram_stp_CurrentDistance;
extern vUSHORT          Iram_stp_BrakeDistance;
extern vULONG           Iram_stp_CurrentTime;
extern vULONG           Iram_stp_MoveInterval;
extern vBYTE            Iram_stp_Status;
extern volatile eStepProfile    Iram_stp_CurrentProfile;
extern volatile eInitialized    Iram_stp_Initialized;
#if (COLLI_SWITCH == 1)
extern volatile eInitialized    Iram_colli_Initialized;
#endif
extern volatile eStepTimerMode  Iram_stp_TimerMode;
extern vBYTE            Iram_stp_CurrentSpeedPhase;
/**********************************************
    External Functions
***********************************************/
extern void svoStp_Init( void );

#if DO_HW_SUPPORT_STEP == 0
extern void svoStp_Homing(void);
extern void svoStp_JumpToPosition( eStepMode StepMode, SHORT position );
extern void svoStp_SetCurrentPosition(USHORT CurrentPosition);
extern void svoStp_SetTimerMode( eStepTimerMode TimerMode );
extern void svoStp_HoldSledge(void);
extern void svoStp_MoveStep( LONG steps );
extern void svoStp_JumpControl( void );
extern void svoStp_SetGain( BYTE gain );
    #if (TRAVERSE_TEST == 1 )
extern void svoStp_Traverse(BYTE,BYTE);
    #endif
#endif // DO_HW_SUPPORT_STEP == 0
extern SHORT svoStp_recovery_jump_dist(void);
extern void svoStp_StopControl( void );
extern void svoStp_verify();

//---------------------------------------------------------------------------------------;
//                                                                                       ;
// Table used to fill the "Micro-Stepping Table". This is the DAC1 part. DAC2 can be     ;
// generated based on DAC1.                                                              ;
//                                                                                       ;
//---------------------------------------------------------------------------------------;
#define SIZE_MIRCO_STEP_TBL 64

static const SBYTE       TblSin64[SIZE_MIRCO_STEP_TBL] = {  0,  12,  25,  37,  49,  60,  71,  81,  //  0 - 7
                                                           90,  98, 106, 112, 117, 122, 125, 126,  //  8 - 15
                                                          127, 126, 125, 122, 117, 112, 106,  98,  // 16 - 23
                                                           90,  81,  71,  60,  49,  37,  25,  12,  // 24 - 31
                                                            0, -12, -25, -37, -49, -60, -71, -81,  // 32 - 39
                                                          -90, -98,-106,-112,-117,-122,-125,-126,  // 40 - 47
                                                         -127,-126,-125,-122,-117,-112,-106, -98,  // 48 - 55
                                                          -90, -81, -71, -60, -49, -37, -25, -12}; // 56 - 63

#endif          /* #ifndef _SVO_STEP_ */

/*------------------------------------------------------------------------*/
/* The end of file                                                        */
/*------------------------------------------------------------------------*/
