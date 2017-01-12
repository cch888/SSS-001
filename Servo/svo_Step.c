/****************************************************************************
*              (c) Copyright 2001 - 2010 Sunext Technology Co., Ltd.
*                            All Rights Reserved
*
*
* FILENAME:   SVO_STEP.C
*
* DESCRIPTION Stepping motor control
*
*   $Revision: 381 $
*   $Date: 11/04/27 11:03a $
*
**************************** SUNEXT CONFIDENTIAL ****************************/

#include ".\common\globals.H"
#include ".\common\globtype.H"
#include ".\common\chip.h"
#include ".\common\stdlib.h"
#include ".\servo\svo.h"
#include ".\servo\svo_afe.h"
#include ".\servo\svoMacro.h"
#include ".\common\error.h"
#include ".\common\commath.h"
#include ".\common\system.h"
#include ".\common\DAC.h"
#include ".\player\plrdb.h"
#include ".\hif\gp_ram.h"
#include ".\servo\svo_com.h"
#include ".\servo\svo_tbl.h"
#include ".\servo\svomath.h"
#include ".\servo\svo_cmd.h"
#include ".\servo\svo_lib.h"
#include ".\servo\svoVar.h"
#include ".\servo\svo_step.h"
#include ".\servo\svo_sta.h"
#include ".\servo\svo_seek.h"
#include ".\servo\svo_dsp.h"
#if (SRC_ENABLE == 1)
#include ".\AL\HAL_dsp.h"
#endif
#if (COLLI_SWITCH == 1)
#include ".\AL\REG_global.h"
#include ".\AL\REG_periph.h"
#include ".\AL\REG_HAL.h"
#endif
#include ".\al\REG_intctrl.h"

//extern vBYTE Iram_WGateUp;

#define SLEDGE_DEBUG_MSG         1      //svo 09
#define DAC_A_B_OFFSET          (-32)

static SHORT stp_BaseDac[4][32] = {
    {   0.00*(511-SLED_DZ)/511-0.49, -25.07*(511-SLED_DZ)/511-0.49, -50.09*(511-SLED_DZ)/511-0.49, -74.98*(511-SLED_DZ)/511-0.49,
      -99.69*(511-SLED_DZ)/511-0.49,-124.16*(511-SLED_DZ)/511-0.49,-148.34*(511-SLED_DZ)/511-0.49,-172.15*(511-SLED_DZ)/511-0.49,
     -195.55*(511-SLED_DZ)/511-0.49,-218.48*(511-SLED_DZ)/511-0.49,-240.88*(511-SLED_DZ)/511-0.49,-262.71*(511-SLED_DZ)/511-0.49,
     -283.90*(511-SLED_DZ)/511-0.49,-304.40*(511-SLED_DZ)/511-0.49,-324.17*(511-SLED_DZ)/511-0.49,-343.17*(511-SLED_DZ)/511-0.49,
     -361.33*(511-SLED_DZ)/511-0.49,-378.63*(511-SLED_DZ)/511-0.49,-395.01*(511-SLED_DZ)/511-0.49,-410.44*(511-SLED_DZ)/511-0.49,
     -424.88*(511-SLED_DZ)/511-0.49,-438.30*(511-SLED_DZ)/511-0.49,-450.66*(511-SLED_DZ)/511-0.49,-461.94*(511-SLED_DZ)/511-0.49,
     -472.10*(511-SLED_DZ)/511-0.49,-481.13*(511-SLED_DZ)/511-0.49,-489.00*(511-SLED_DZ)/511-0.49,-495.69*(511-SLED_DZ)/511-0.49,
     -501.18*(511-SLED_DZ)/511-0.49,-505.47*(511-SLED_DZ)/511-0.49,-508.54*(511-SLED_DZ)/511-0.49,-510.38*(511-SLED_DZ)/511-0.49},
    {-512.00*(511-SLED_DZ)/511-0.49,-510.38*(511-SLED_DZ)/511-0.49,-508.54*(511-SLED_DZ)/511-0.49,-505.47*(511-SLED_DZ)/511-0.49,
     -501.18*(511-SLED_DZ)/511-0.49,-495.69*(511-SLED_DZ)/511-0.49,-489.00*(511-SLED_DZ)/511-0.49,-481.13*(511-SLED_DZ)/511-0.49,
     -472.10*(511-SLED_DZ)/511-0.49,-461.94*(511-SLED_DZ)/511-0.49,-450.66*(511-SLED_DZ)/511-0.49,-438.30*(511-SLED_DZ)/511-0.49,
     -424.88*(511-SLED_DZ)/511-0.49,-410.44*(511-SLED_DZ)/511-0.49,-395.01*(511-SLED_DZ)/511-0.49,-378.63*(511-SLED_DZ)/511-0.49,
     -361.33*(511-SLED_DZ)/511-0.49,-343.17*(511-SLED_DZ)/511-0.49,-324.17*(511-SLED_DZ)/511-0.49,-304.40*(511-SLED_DZ)/511-0.49,
     -283.90*(511-SLED_DZ)/511-0.49,-262.71*(511-SLED_DZ)/511-0.49,-240.88*(511-SLED_DZ)/511-0.49,-218.48*(511-SLED_DZ)/511-0.49,
     -195.55*(511-SLED_DZ)/511-0.49,-172.15*(511-SLED_DZ)/511-0.49,-148.34*(511-SLED_DZ)/511-0.49,-124.16*(511-SLED_DZ)/511-0.49,
      -99.69*(511-SLED_DZ)/511-0.49, -74.98*(511-SLED_DZ)/511-0.49, -50.09*(511-SLED_DZ)/511-0.49, -25.07*(511-SLED_DZ)/511-0.49},
    {   0.00*(511-SLED_DZ)/511+0.49,  25.07*(511-SLED_DZ)/511+0.49,  50.09*(511-SLED_DZ)/511+0.49,  74.98*(511-SLED_DZ)/511+0.49,
       99.69*(511-SLED_DZ)/511+0.49, 124.16*(511-SLED_DZ)/511+0.49, 148.34*(511-SLED_DZ)/511+0.49, 172.15*(511-SLED_DZ)/511+0.49,
      195.55*(511-SLED_DZ)/511+0.49, 218.48*(511-SLED_DZ)/511+0.49, 240.88*(511-SLED_DZ)/511+0.49, 262.71*(511-SLED_DZ)/511+0.49,
      283.90*(511-SLED_DZ)/511+0.49, 304.40*(511-SLED_DZ)/511+0.49, 324.17*(511-SLED_DZ)/511+0.49, 343.17*(511-SLED_DZ)/511+0.49,
      361.33*(511-SLED_DZ)/511+0.49, 378.63*(511-SLED_DZ)/511+0.49, 395.01*(511-SLED_DZ)/511+0.49, 410.44*(511-SLED_DZ)/511+0.49,
      424.88*(511-SLED_DZ)/511+0.49, 438.30*(511-SLED_DZ)/511+0.49, 450.66*(511-SLED_DZ)/511+0.49, 461.94*(511-SLED_DZ)/511+0.49,
      472.10*(511-SLED_DZ)/511+0.49, 481.13*(511-SLED_DZ)/511+0.49, 489.00*(511-SLED_DZ)/511+0.49, 495.69*(511-SLED_DZ)/511+0.49,
      501.18*(511-SLED_DZ)/511+0.49, 505.47*(511-SLED_DZ)/511+0.49, 508.54*(511-SLED_DZ)/511+0.49, 510.38*(511-SLED_DZ)/511+0.49},
    { 511.00*(511-SLED_DZ)/511+0.49, 510.38*(511-SLED_DZ)/511+0.49, 508.54*(511-SLED_DZ)/511+0.49, 505.47*(511-SLED_DZ)/511+0.49,
      501.18*(511-SLED_DZ)/511+0.49, 495.69*(511-SLED_DZ)/511+0.49, 489.00*(511-SLED_DZ)/511+0.49, 481.13*(511-SLED_DZ)/511+0.49,
      472.10*(511-SLED_DZ)/511+0.49, 461.94*(511-SLED_DZ)/511+0.49, 450.66*(511-SLED_DZ)/511+0.49, 438.30*(511-SLED_DZ)/511+0.49,
      424.88*(511-SLED_DZ)/511+0.49, 410.44*(511-SLED_DZ)/511+0.49, 395.01*(511-SLED_DZ)/511+0.49, 378.63*(511-SLED_DZ)/511+0.49,
      361.33*(511-SLED_DZ)/511+0.49, 343.17*(511-SLED_DZ)/511+0.49, 324.17*(511-SLED_DZ)/511+0.49, 304.40*(511-SLED_DZ)/511+0.49,
      283.90*(511-SLED_DZ)/511+0.49, 262.71*(511-SLED_DZ)/511+0.49, 240.88*(511-SLED_DZ)/511+0.49, 218.48*(511-SLED_DZ)/511+0.49,
      195.55*(511-SLED_DZ)/511+0.49, 172.15*(511-SLED_DZ)/511+0.49, 148.34*(511-SLED_DZ)/511+0.49, 124.16*(511-SLED_DZ)/511+0.49,
       99.69*(511-SLED_DZ)/511+0.49,  74.98*(511-SLED_DZ)/511+0.49,  50.09*(511-SLED_DZ)/511+0.49,  25.07*(511-SLED_DZ)/511+0.49},
};

typedef enum _eStepPhase
{
    ACC_PHASE = 0,
    VMAX_PHASE,
    DEC_PHASE,
    END_PHASE
} eStepPhase;

typedef enum _eSpecialCase
{
    NORMAL_CASE = 0,
    HOMING_STEP1,
    HOMING_STEP2,
#if (OPU_HomeSW > 0)
    HOMING_STEP3,
#endif
    SEARCH_RF,
#if (COLLI_SWITCH == 1)
    COLLI_HOMING_STEP1,
    COLLI_HOMING_STEP2,
    COLLI_HOMING_STEP3,
    COLLI_HOMING_STEP4,
#endif
} eSpecialCase;

#define     COLLI_SA_PREINIT_VALUE  1   /* others from High to low */

/* sledge jump profile:
   |      ________________
 v |     /     VMax       \
 e |    /                  \
 l |   /                    \
 o |  /Acc(slope)  Dec(slope)\
 c | /                        \
 i |/_  _  _  _  _  _  _  _  _ \___    __:SLED_JUMP_MIN_VEL
 t |                            ###|
 y |                            ###|   ##:SLED_JUMP_END_DIST
   |-----------------------------------
  0              time
*/

private BYTE   CE_Gain_Down_Ratio;
private ULONG  Rough_CEGain_SwitchPoint;
private BYTE   CE_Gain_stage;

typedef struct _tSettings
{
    BYTE VMax;   //maximum velocity.           unit:1.0 step / ms      => 4.6875     um/ms
    BYTE Acc;    //acceleration.               unit:0.0625 step / ms^2 => 0.29296875 um/ms^2
    BYTE Dec;    //deceleration.               unit:0.0625 step / ms^2 => 0.29296875 um/ms^2
    BYTE VFull;  //sin,square wave transition. unit:1.0 step / ms      => 4.6875     um/ms
    BYTE Gain;   //sin,square wave amplitude.  0~128
} tSettings;

#if (SLEDGE_DEBUG_MSG==1)
tSettings stp_Settings[PROFILE_MAX] = {
#else
const tSettings stp_Settings[PROFILE_MAX] = {
#endif
    SLED_HOME_JUMP_PROFILE,   /* STEP_HOMING_PROFILE */ //this mode the SLED_GAIN will be smoothly reduced to SLED_HOMING_GAIN during jump
    SLED_FULL_JUMP_PROFILE,   /* STEP_FULL_PROFILE */   //this mode use higher gain to pull OPU away from the wall
    SLED_SAFE_JUMP_PROFILE,   /* STEP_SAFE_SLOW_PROFILE */
    SLED_NORMAL_JUMP_PROFILE, /* STEP_NORMAL_PROFILE */
    SLED_SHORT_JUMP_PROFILE,  /* STEP_NORMAL_SHORT_PROFILE */
    SLED_NO_CE_JUMP_PROFILE,  /* STEP_NO_CE_SLOW_PROFILE */
#if (COLLI_SWITCH == 1)
    COLLI_NORMAL_JUMP_PROFILE,/* COLLI_NORMAL_PROFILE */
    COLLI_MICRO_JUMP_PROFILE, /* COLLI_MICRO_PROFILE */
#endif
#if (Customer_A1 == 1)
		SLED_HOME2_JUMP_PROFILE,
#endif
};

private vUSHORT   svoPrvtVar_OutputFullDac[4];
private vULONG    max_velocity;
private ULONG     full_velocity;
private ULONG     min_velocity;
private ULONG     move_steps;
private BYTE      full_mode;
private vBYTE     CurrentGain;
private eSpecialCase volatile specialCase;
private eStepMode new_jump_mode;
private SHORT     new_jump_target;
#if (COLLI_SWITCH == 1)
private eStepMode new_jump_mode2;
private SHORT     new_jump_target2;
#endif
private vUSHORT   *StepPosition;
private vSHORT    *CurrentPosition;
#if ((COLLI_SWITCH == 1) && (COLLI_REST_TIME > 0))
private ULONG     ColliRestTimer;
#endif
#if ((COLLI_HomeSW > 1) || (OPU_HomeSW > 1))
private BYTE      HomeSW_det_cnt;
#endif
static void BrakeSledge(void);

#if (TRAVERSE_TEST == 1 )
SHORT Error_1;
#endif

/*-----------------------------------------------------------------------------
 *
 * Module: svoStp_Init()
 * Description:
 *
 *----------------------------------------------------------------------------*/
void svoStp_Init( void )
{
#if (DO_HW_SUPPORT_STEP == 1)
    BYTE  temp_IE;
#endif
#if (SLEDGE_DEBUG_MSG==1)
    /*output the dram address of stp_Settings[] for profile tuning*/
    SendMsgCnB(SHOW_DEBUG_MSG,4,0x340000,B4B(stp_Settings)); //stp_Settings(temp)
#endif

#if DO_HW_SUPPORT_STEP == 0
    CurrentGain = 0;
#else // DO_HW_SUPPORT_STEP == 0
    temp_IE = GET_INT_ENABLE_MASK();
    DISABLE_INTERRUPT();
#if (CHIP_REV <= 0xB3)
    WRITE_REG(HAL_SRVREG_MAP->STPLOC, 0x2710);//TBD:SE1 hard code
#else
    WRITE_FIELD(HAL_SRV_STPLOC, 0x2710);//TBD:SE1 hard code
#endif
    SET_INT_ENABLE_MASK(temp_IE);

    WRITE_REG(HAL_SRVREG_MAP->STPCTRL, 0xC0);    // set to external control mode and assign the POSVAL to 0  //TBD:SE1 hardcode
    WRITE_REG(HAL_SRVREG_MAP->STPCTRL, 0x40);    // set to internal control mode

    FillMicroSteppingTable(250);       // 256 --> 1.0
    //-----------------------------------------------------------;
    // init HW stepper motor support (setup)                     ;
    //-----------------------------------------------------------;
    WRITE_REG(HAL_SRVREG_MAP->STPGEN, 0x20);     // BYPSINT = 1, Bypass step motor integrator
                                                 // PSSEL   = 1, Power saving enable
                                                 // KSLO    = b110, integrator sample freq = 1378 divided by 8
    WRITE_REG (HAL_SRVREG_MAP->STPSIZE, 0x00);   // step size = 1
    WRITE_REG (HAL_SRVREG_MAP->STPDCO, 0x00);    // set DCO (step motor support clk also) 4*1378 Hz
    WRITE_REG (HAL_SRVREG_MAP->STPPWR, 20);      // 3.6 ms (given step motor support clk being 4*1378 Hz )

    //  PAC ---> PAM0 = 0 --> enable PAM, 100% on
    WRITE_REG(HAL_SRVREG_MAP->PAC, 0x00);  //100%

    //  DEADZ ---> do not use dead zone
    WRITE_REG(HAL_SRVREG_MAP->DEADZ, 0x00);

    // GPDACMUX <----- DAC1, DAC2 as stepper
    WRITE_REG(HAL_SRVREG_MAP->GPDACMUX, 0x14);   // for steper
#endif //DO_HW_SUPPORT_STEP == 0
#if (COLLI_SWITCH == 1)
    #if ((PCB == 690)||(PCB == 691))
    WRITE_FIELD(HAL_SRV_COLLISW,1);
    WRITE_FIELD(HAL_SRV_PDMCHSEL,1);
    #elif ((PCB == 692)||(PCB == 710)||(PCB == 711)||(PCB == 812)||(PCB == 830)||(PCB == 815)||(PCB == 817))
    WRITE_FIELD(HAL_GLOBAL_ALPSINOUT,1);
    WRITE_FIELD(HAL_GLOBAL_ALPCOSOUT,1);
    WRITE_FIELD(HAL_SRV_ALPHASW,1);
    WRITE_FIELD(HAL_SRV_PDMCHSEL,0);
    #endif
    #if (COLLI_ANALOG_OUTPUT == 0)
    //set to 10bits 65KHz PWM
    WRITE_FIELD(HAL_SRV_AL2PWMSEL,3);
    WRITE_FIELD(HAL_SRV_AL1PWMSEL,3);
    #endif
    Iram_colli_StepPosition = 0x4000;
    Iram_colli_CurrentPosition= 0x4000;
    Iram_colli_Initialized = STEP_UNINITIALIZED;
    #if (COLLI_REST_TIME > 0)
    ColliRestTimer = 0xFFFFFFFF - COLLI_REST_TIME;
    #endif
#endif
    svoStepVar.StepPosCal = FALSE;
    specialCase = NORMAL_CASE;
    Iram_stp_Status = 0;
    Iram_stp_Initialized = STEP_UNINITIALIZED;
    Iram_stp_CurrentPosition = STEP_TRAY_OUT_POSITION;
    Iram_stp_StepPosition = 0x4000 - ((STEP_OUTWARD_POSITION-STEP_TRAY_OUT_POSITION-STEP_OUTWARD_MICRO_STEP)&0x7F);
    full_mode = 0;
    new_jump_mode = STEP_NULL_MODE;
#if (COLLI_SWITCH == 1)
    new_jump_mode2 = STEP_NULL_MODE;
#endif
}// svoStp_Init

/*-----------------------------------------------------------------------------
 *
 * Module: svoStp_GetProfile()
 * Description: Chose the profile from selected mode
 *
 *----------------------------------------------------------------------------*/
eStepProfile svoStp_GetProfile(eStepMode StepMode)
{
    BYTE profile;

    switch (StepMode)
    {
        case STEP_HOMING_MODE:
            profile = STEP_HOMING_PROFILE;
            break;

        case STEP_FULL_MODE:
            profile = STEP_FULL_PROFILE;
            break;

	#if (Customer_A1==1)
        case STEP_HOMING2_MODE:
            profile = STEP_HOMING2_PROFILE;
            break;
	#endif			

        case STEP_SAFE_SLOW_MODE:
            profile = STEP_SAFE_SLOW_PROFILE;
            break;

        case STEP_NORMAL_MODE:
            if (Iram_stp_JumpDistance <= UM_2_SLED_STEP(3000))
            {
                profile = STEP_NORMAL_SHORT_PROFILE;
            }
            else
            {
                profile = STEP_NORMAL_PROFILE;
            }
            break;

        case STEP_NO_CE_SLOW_MODE:
        case STEP_SEARCH_RF_MODE:
            profile = STEP_NO_CE_SLOW_PROFILE;
            break;

#if COLLI_SWITCH ==1
        case COLLI_NORMAL_MODE:
        case COLLI_NORMAL_NO_REST_MODE:
            profile = COLLI_NORMAL_PROFILE;
            break;

        case COLLI_MICRO_MODE:
        case COLLI_MICRO_NO_REST_MODE:
            profile = COLLI_MICRO_PROFILE;
            break;
#endif
    }

    return profile;
}// svoStp_GetProfile


/*-----------------------------------------------------------------------------
 *
 * Module: svoStp_Homing()
 * Description:
 *
 *----------------------------------------------------------------------------*/
void svoStp_Homing(void)
{
    BYTE temp_IE;

    if (svoStp_busy())
    {
        temp_IE = GET_INT_ENABLE_MASK();
        DISABLE_INTERRUPT();
        // New jump command recived during jumping, brake the sledge and store the new jump target.
#if (COLLI_SWITCH == 1)
        if (((Iram_stp_Status & STP_DEVICE_MASK) == STP_DEVICE_NORMAL) && (svoStepVar.stepHomeCheck == FALSE))
#endif
        {
            //only brake when the new command is for same device
            BrakeSledge();
        }
#if (COLLI_SWITCH == 1)
        if (new_jump_mode > STEP_MODE_MAX)
        {
            new_jump_mode2 = STEP_NEW_HOME_MODE;
        }
        else
#endif
        {
            new_jump_mode = STEP_NEW_HOME_MODE;
        }
        SET_INT_ENABLE_MASK(temp_IE);
        return;
    }
    else
    {
#if (SLEDGE_DEBUG_MSG == 1)
        SendMsg80(SHOW_DEBUG_MSG,0x340102);//SLED home start
#endif
        svoStepVar.stepHomeCheck = TRUE;
        Iram_stp_StepPosition = 0x4000 - ((STEP_OUTWARD_POSITION-STEP_TRAY_OUT_POSITION-STEP_OUTWARD_MICRO_STEP)&0x7F);
        svoStepVar.StepPosCal = FALSE;    //reset sledge position.
#if (OPU_HomeSW > 0)
        if (ipHOME == 0)//already touched
        {
            svoStp_JumpSteps(STEP_SAFE_SLOW_MODE,HOMING_PRE_DISTANCE);
            specialCase = HOMING_STEP1;
        }
        else
        {
            svoStp_JumpSteps(STEP_HOMING_MODE,-HOMING_DISTANCE);
            specialCase = HOMING_STEP2;
        }
    #if (OPU_HomeSW > 1)
        HomeSW_det_cnt = 0;
    #endif
#else

#if (Customer_A1 ==1)
	
			if((CompleteStrokeCount<3)&&((CompleteStroke)||(AgingMode)))
			{
        svoStp_JumpSteps(STEP_HOMING_MODE,HOMING_DISTANCE);
///				SendMsgCn(SHOW_DEBUG_MSG,0,0x90,0x96,0x00); 
				SendMsgCn(SHOW_DEBUG_MSG,0,0x909600);
				
				CompleteStroke = FALSE;
				CompleteStrokeCount ++;
				PositionHomeErrorCount=0;
///				SendMsgCnB(SHOW_DEBUG_MSG,4,0x90,0x92,0x00,B4B(CompleteStrokeCount));
				RetryDiscLaser = 0;
			}
			else
			{
		        		svoStp_JumpSteps(STEP_HOMING_MODE,Short_HOMING_DISTANCE);
			}
#else
	    svoStp_JumpSteps(STEP_HOMING_MODE,HOMING_DISTANCE);
#endif

        specialCase = HOMING_STEP1;
#endif
    }
}

/*-----------------------------------------------------------------------------
 *
 * Module: svoColli_Homing()
 * Description:Colli does homing and presets a position for BD.
 *
 *----------------------------------------------------------------------------*/
#if (COLLI_SWITCH == 1)
void svoColli_Homing(void)
{
    BYTE temp_IE;

  #if (PCB == 690)
    // This code seems an experimental test code released by somebody.
    // BCM-1600 will disable this due to a malfucntion, because it doesn't use opPI_CONT either.
    // Other projects may have some other strange symptoms depending on the PC1 and PC3 usages. (BGR)

    //-----------------------------------turn on colli function--------------------------------------------
    WRITE_FIELD(HAL_GLOBAL_LANDGOUT,0);  //0724.1=0->GPIO
    WRITE_FIELD(HAL_PERIPH_PC1DIR,0);    //PC1 input/output(PC1 direction determined by 05C5.1)  0:output 1:input
    opPI_CONT=1;                         //OPU PI control
    //-----------------------------------read collimator status--------------------------------------------------
    WRITE_FIELD(HAL_GLOBAL_WGATEOUT,0);   //0724.3=0->GPIO
    WRITE_FIELD(HAL_PERIPH_PC3DIR,1);  //PC3 input/output(PC3 direction determined by 05C5.3)  0:output 1:input
  #elif (PCB == 830)                        //OPU PI control
    send_msg54(SEND_ATAPI_CMD,'C','L','E','D');

    HAL_LASC_SetCollimotorLED(OPTO_COUPLER_LED_ON);
  #endif // ((PCB == 690) || (PCB == 830))

    if (svoStp_busy())
    {
        temp_IE = GET_INT_ENABLE_MASK();
        DISABLE_INTERRUPT();
        // New jump command recived during jumping, brake the sledge and store the new jump target.
#if (COLLI_SWITCH == 1)
        if (((Iram_stp_Status & STP_DEVICE_MASK) == STP_DEVICE_COLLI) && (svoStepVar.stepHomeCheck == FALSE))
#endif
        {
            //only brake when the new command is for same device
            BrakeSledge();
        }
#if (COLLI_SWITCH == 1)
        if ((new_jump_mode != STEP_NULL_MODE) && (new_jump_mode <= STEP_MODE_MAX))
        {
            new_jump_mode2 = COLLI_NEW_HOME_MODE;
        }
        else
#endif
        {
            new_jump_mode = COLLI_NEW_HOME_MODE;
        }
        SET_INT_ENABLE_MASK(temp_IE);
        return;
    }
    else
    {
        SendMsg80(SHOW_DEBUG_MSG,0x340103);//COLLI home start
        svoStepVar.stepHomeCheck = TRUE;
    #if (COLLI_HOMING_PRE_DISTANCE > 0)
        #if (COLLI_HomeSW != 0)
        if (ipSA_INT == COLLI_SA_PREINIT_VALUE)
        {
            svoStp_JumpSteps(COLLI_NORMAL_NO_REST_MODE,COLLI_HOMING_PRE_DISTANCE);
            specialCase = COLLI_HOMING_STEP1;
        }
        else
        {
            svoStp_JumpSteps(COLLI_NORMAL_NO_REST_MODE,-COLLI_HOMING_DISTANCE);
            specialCase = COLLI_HOMING_STEP2;
        }
            #if (COLLI_HomeSW > 1)
        HomeSW_det_cnt = 0;
            #endif
        #else
        svoStp_JumpSteps(COLLI_MICRO_NO_REST_MODE,COLLI_HOMING_PRE_DISTANCE);
        specialCase = COLLI_HOMING_STEP1;
        #endif
    #else
        svoStp_JumpSteps(COLLI_NORMAL_NO_REST_MODE,-COLLI_HOMING_DISTANCE);
        specialCase = COLLI_HOMING_STEP2;
    #endif
    }
}
#endif

/*-----------------------------------------------------------------------------
 *
 * Module: svoStp_JumpToPosition()
 * Description:
 *
 *----------------------------------------------------------------------------*/
void svoStp_JumpToPosition( eStepMode StepMode,
                            SHORT     position ) // abs location in step count
{
    BYTE   temp_IE;
    LONG   temp_long;
    USHORT acc_dist_max,dec_dist_max,min_vel_2;

    if (svoStp_busy())
    {
        temp_IE = GET_INT_ENABLE_MASK();
        DISABLE_INTERRUPT();
        // New jump command recived during jumping, brake the sledge and store the new jump target.
#if (COLLI_SWITCH == 1)
        if (((StepMode > STEP_MODE_MAX) ^ ((Iram_stp_Status & STP_DEVICE_MASK) == STP_DEVICE_NORMAL)) && (svoStepVar.stepHomeCheck == FALSE))
#endif
        {
            //only brake when the new command is for same device
            BrakeSledge();
        }

#if (COLLI_SWITCH == 1)
        if ((new_jump_mode != STEP_NULL_MODE) && ((StepMode > STEP_MODE_MAX) ^ (new_jump_mode > STEP_MODE_MAX)))
        {
            new_jump_mode2 = StepMode;
            new_jump_target2 = position;
        }
        else
#endif
        {
            new_jump_mode = StepMode;
            new_jump_target = position;
        }
        SET_INT_ENABLE_MASK(temp_IE);
        return;
    }

#if (COLLI_SWITCH == 1)
    if (StepMode > STEP_MODE_MAX)
    {
    #if (COLLI_ANALOG_OUTPUT == 0)
        position = (position & 0xFFE0);
    #endif
        Iram_stp_Status = STP_DEVICE_COLLI | STP_BUSY_MASK;
        StepPosition = (&Iram_colli_StepPosition);
        CurrentPosition = (&Iram_colli_CurrentPosition);
        min_velocity = COLLI_JUMP_MIN_VEL * 4 * 8192;
        min_vel_2 = COLLI_JUMP_MIN_VEL * COLLI_JUMP_MIN_VEL;
    }
    else
#endif
    {
        Iram_stp_Status = STP_DEVICE_NORMAL | STP_BUSY_MASK;
        StepPosition = (&Iram_stp_StepPosition);
        CurrentPosition = (&Iram_stp_CurrentPosition);
        min_velocity = SLED_JUMP_MIN_VEL * 4 * 8192;
        min_vel_2 = SLED_JUMP_MIN_VEL * SLED_JUMP_MIN_VEL;
    }

    Iram_stp_JumpDistance = ABS((LONG)(*CurrentPosition) - position);
    if (Iram_stp_JumpDistance == 0)
    {
        Iram_stp_Status = 0;
#if (COLLI_SWITCH == 1)
        StepPosition    = (&Iram_stp_StepPosition);
        CurrentPosition = (&Iram_stp_CurrentPosition);
#endif
        return;
    }

    if (DMO_CLIP_SLED_MOVING < svoSpeedVar.Kd14Fg)
    {
        spdSetKd14(DMO_CLIP_SLED_MOVING);
    }

    if (StepMode == STEP_SEARCH_RF_MODE)
    {
        specialCase = SEARCH_RF;
    }
    else
    {
        specialCase = NORMAL_CASE;
    }

    Iram_stp_CurrentSpeedPhase = ACC_PHASE;
    Iram_stp_CurrentDistance   = 0;
    Iram_stp_CurrentDirection  = ((*CurrentPosition) >= position) ? BACK : FORW;
    Iram_stp_CurrentProfile    = svoStp_GetProfile(StepMode);
    full_velocity              = (ULONG)stp_Settings[Iram_stp_CurrentProfile].VFull * (4 * 8192);
    full_mode                  = (full_velocity == 0)? 1 : 0;
#if (COLLI_SWITCH == 1) && (COLLI_REST_TIME > 0)
    if ((StepMode > STEP_MODE_MAX) && (StepMode != COLLI_NORMAL_NO_REST_MODE) && (StepMode != COLLI_MICRO_NO_REST_MODE))
    {
        temp_long = ReadTimer(&ColliRestTimer);
        if (temp_long < COLLI_REST_TIME)
        {//colli rest time needed, the sled will not following during rest time, always track off before colli jump.
            temp_long = (COLLI_REST_TIME - temp_long);
            if (new_jump_target != position)
            {
                SendMsgCn(SHOW_DEBUG_MSG,2,0x340212,A2B(temp_long)); //COLLI rest delay
            }
            temp_long = CLK16_1000_US * temp_long;

            temp_long = temp_long > 0xFFFF? 0xFFFF : temp_long;
            Iram_stp_JumpDistance = 0;
            Iram_stp_CurrentDistance = 0;
            if ((new_jump_mode != STEP_NULL_MODE) && (new_jump_mode <= STEP_MODE_MAX))
            {
                new_jump_mode2 = new_jump_mode;
                new_jump_target2 = new_jump_target;
            }
            new_jump_mode = StepMode;
            new_jump_target = position;
            svoStp_SetTimerMode(STEP_REST_TIMER);
            Set_Timer_Int_NON_STOP(&TIMER5, temp_long);
            return;
        }
    }
#endif
    svoStp_SetGain(stp_Settings[Iram_stp_CurrentProfile].Gain);
    svoStp_MoveStep(0);
    //Calculate the distant needed to accelerate from SLED_JUMP_MIN_VEL to VMax and to decelerate from VMax to SLED_JUMP_MIN_VEL.
    temp_long = stp_Settings[Iram_stp_CurrentProfile].VMax;
    temp_long = (temp_long * temp_long - min_vel_2) * 8;
    acc_dist_max = temp_long / stp_Settings[Iram_stp_CurrentProfile].Acc;
    dec_dist_max = temp_long / stp_Settings[Iram_stp_CurrentProfile].Dec;
    if (Iram_stp_JumpDistance > (acc_dist_max + dec_dist_max + SLED_JUMP_END_DIST))
    {   //The jump distance is long enough to reach VMax.
        max_velocity = stp_Settings[Iram_stp_CurrentProfile].VMax * (4 * 8192);
        Iram_stp_BrakeDistance = Iram_stp_JumpDistance - dec_dist_max - SLED_JUMP_END_DIST;
    }
    else
    {   //The jump distance isn't long enough to reach VMax.
        temp_long = (Iram_stp_JumpDistance - SLED_JUMP_END_DIST);
        if (temp_long <= 0)
        {
            temp_long = 0;
            Iram_stp_CurrentSpeedPhase = END_PHASE;
        }
        else
        {
            temp_long = temp_long * stp_Settings[Iram_stp_CurrentProfile].Dec / (stp_Settings[Iram_stp_CurrentProfile].Acc + stp_Settings[Iram_stp_CurrentProfile].Dec);
        }
        //Calculate the max_velocity and brake distance based on given jump distance
        max_velocity = 512 * sqrt(512 * (temp_long * stp_Settings[Iram_stp_CurrentProfile].Acc + 8 * min_vel_2));
        Iram_stp_BrakeDistance = temp_long;
    }

    Iram_stp_CurrentTime = 0;
    move_steps = (full_mode? 32 : 1);
    Iram_stp_MoveInterval = ((ULONG)move_steps * (8192 * 8192)) / (min_velocity / 4);

    svoStp_SetTimerMode(STEP_ROUGH_TIMER_START);

    CE_Gain_stage = 0;

#if (COLLI_SWITCH == 1)
    if ((Iram_stp_Status & STP_DEVICE_MASK) == STP_DEVICE_NORMAL)
#endif
    {
        if (StepMode != STEP_NO_CE_SLOW_MODE)
        {
            switch (svoVar_discKindT.Bit.DiscStd)
            {
                case eDiscCD:
                    Rough_CEGain_SwitchPoint = (CE_GAIN_DOWN_VEL_CD * 4 * 8192);
                    CE_Gain_Down_Ratio = CE_GAIN_DOWN_RATIO_CD;
                    break;
#if (BD_ENABLE == 1)
                case eDiscBD:
                    Rough_CEGain_SwitchPoint = (CE_GAIN_DOWN_VEL_BD * 4 * 8192);
                    CE_Gain_Down_Ratio = CE_GAIN_DOWN_RATIO_BD;
                    break;
#endif
#if (ENABLE_HDDVD == 1)
                case eDiscHD:
                    Rough_CEGain_SwitchPoint = (CE_GAIN_DOWN_VEL_BD * 4 * 8192);
                    CE_Gain_Down_Ratio = CE_GAIN_DOWN_RATIO_BD;
                    break;
#endif
                case eDiscDVD:
                default:
                    Rough_CEGain_SwitchPoint = (CE_GAIN_DOWN_VEL_DVD * 4 * 8192);
                    CE_Gain_Down_Ratio = CE_GAIN_DOWN_RATIO_DVD;
                    break;
            }
#if (DVD_RAM_READ == 1)
            if (TrackPitchChange)
            {
                // Track-pitch changes during rough seek.
                // CE suffers from changes of offset and gain.
                // Use damper (does not control low-frequency component) to avoid offset and gain jump.
                seekSetCEEq(eDamper, CE_Gain_Down_Ratio);
            }
            else
#endif
            {
                seekSetCEEq(eRoughSeek, CE_Gain_Down_Ratio);
            }
            CE_Gain_stage = 1;

            if (FokStatusOK())
            {
#if (SRC_ENABLE == 1)
                /*learning of SRC is very fast, and RPM change during sled jump is not that much, don't need to reset.*/
                HAL_DSP_SRCHold(SRC_LOOP_TRACKING, ON, ON, ON);
#endif

                HAL_SERVO_CenterControlON(Iram_stp_CurrentDirection);
            }
        }
        if (svoStepVar.stepHomeCheck)
        {
            calUpdateRadius(svoStepVar.StepHomePos, FALSE);/* preset tilt to home position */
        }
        else
        {
            calUpdateRadius(position, FALSE);/* preset tilt to target position */
        }
#if (SLEDGE_DEBUG_MSG==1)
        SendMsgCn(DEBUG_SEEK_STEP_DATA_MSG,2,0x340200+Iram_stp_CurrentDirection,A2B(Iram_stp_JumpDistance));
#endif
    }
#if (COLLI_SWITCH == 1)
    else
    {
        COLLI_DRIVER_ON;
        SendMsgCn(SHOW_DEBUG_MSG,2,0x340213,A2B(position)); //COLLI jump to %4d
        SendMsgCn(DEBUG_SEEK_STEP_DATA_MSG,2,0x340210+Iram_stp_CurrentDirection,A2B(Iram_stp_JumpDistance));
    }
#endif
}// end of svoStp_JumpToPosition

/*-----------------------------------------------------------------------------
 *
 * Module: svoStp_MoveStep()
 * Description:
 *
 *----------------------------------------------------------------------------*/
void svoStp_MoveStep(LONG steps)
{
    ULONG   QuadrantA,QuadrantB;
    ULONG   QuadrantIndex;
    USHORT  dac_a, dac_b;
    ULONG   tempStepPosition;
    ULONG   sled_gain;

    (*CurrentPosition) += steps;
    (*StepPosition) += steps;
    tempStepPosition =(*StepPosition);

    /* Quadrant      = (tempStepPosition / 32) % 4 */
    /* QuadrantIndex = (tempStepPosition % 32)     */
    QuadrantA     = (tempStepPosition >> 5) & 0x03;
    tempStepPosition += DAC_A_B_OFFSET;
    QuadrantB     = (tempStepPosition >> 5) & 0x03;
    if (full_mode == 0)
    {
        QuadrantIndex = (tempStepPosition & 0x1F);
        if (svoStp_busy())
        {
            sled_gain = CurrentGain;
        }
        else
        {
            sled_gain = SLED_FOLLOW_GAIN;
        }
        dac_a = ((sled_gain * (ULONG)stp_BaseDac[QuadrantA][QuadrantIndex] + 64) / 128) & 0x3FF;
#if (SLED_DZ > 0)
        dac_a += ((QuadrantA < 2)? -SLED_DZ : SLED_DZ);
#endif
        dac_b = ((sled_gain * (ULONG)stp_BaseDac[QuadrantB][QuadrantIndex] + 64) / 128) & 0x3FF;
#if (SLED_DZ > 0)
        dac_b += ((QuadrantB < 2)? -SLED_DZ : SLED_DZ);
#endif
    }
    else
    {
        dac_a = svoPrvtVar_OutputFullDac[QuadrantA];
        dac_b = svoPrvtVar_OutputFullDac[QuadrantB];
    }

    switch (Iram_stp_Status & STP_DEVICE_MASK)
    {
#if (COLLI_SWITCH == 1)
        case STP_DEVICE_COLLI:
            Colli_OutputDac(dac_a, dac_b);
            break;
#endif

        case STP_DEVICE_NORMAL:
            svoStp_OutputDac(dac_a, dac_b);

            /* ======= Update Current Tilt ========== */
            if ((!svoStp_busy()) && (!DVDRAM))
            {   /* don't update tilt during sledge jump */
                calUpdateRadius(Iram_stp_CurrentPosition, FALSE);
            }
            break;
    }
}// end of svoStp_MoveStep

/*-----------------------------------------------------------------------------
 *
 * Module: svoStp_HoldSledge()
 * Description: Hold sledge with low gain
 *
 *----------------------------------------------------------------------------*/
void svoStp_HoldSledge()
{
    BYTE    QuadrantA,QuadrantB;
    BYTE    QuadrantIndex;
    USHORT  dac_a, dac_b;
    USHORT  tempStepPosition;

    tempStepPosition = (*StepPosition);
    QuadrantIndex = (tempStepPosition & 0x1F);
    QuadrantA     = (tempStepPosition >> 5) & 0x03;
    tempStepPosition += DAC_A_B_OFFSET;
    QuadrantB     = (tempStepPosition >> 5) & 0x03;

    switch (Iram_stp_Status & STP_DEVICE_MASK)
    {
#if (COLLI_SWITCH == 1)
        case STP_DEVICE_COLLI:
            COLLI_DRIVER_OFF;
    #if (COLLI_ANALOG_OUTPUT == 1)
        #if (COLLI_HOLD_GAIN == 0)
            Colli_OutputDac(0, 0);
        #else
            dac_a = ((COLLI_HOLD_GAIN * (ULONG)stp_BaseDac[QuadrantA][QuadrantIndex] + 64) / 128) & 0x3FF;
            #if (SLED_DZ > 0)
            dac_a += ((dac_a == 0)? 0 : ((QuadrantA < 2)? -SLED_DZ : SLED_DZ));
            #endif
            dac_b = ((COLLI_HOLD_GAIN * (ULONG)stp_BaseDac[QuadrantB][QuadrantIndex] + 64) / 128) & 0x3FF;
            #if (SLED_DZ > 0)
            dac_b += ((dac_b == 0)? 0 : ((QuadrantB < 2)? -SLED_DZ : SLED_DZ));
            #endif
            Colli_OutputDac(dac_a, dac_b);
        #endif
    #endif
            break;
#endif

        case STP_DEVICE_NORMAL:
            dac_a = ((SLED_HOLD_GAIN * (ULONG)stp_BaseDac[QuadrantA][QuadrantIndex] + 64) / 128) & 0x3FF;
#if SLED_DZ > 0
            dac_a += ((dac_a == 0)? 0 : ((QuadrantA < 2)? -SLED_DZ : SLED_DZ));
#endif
            dac_b = ((SLED_HOLD_GAIN * (ULONG)stp_BaseDac[QuadrantB][QuadrantIndex] + 64) / 128) & 0x3FF;
#if SLED_DZ > 0
            dac_b += ((dac_b == 0)? 0 : ((QuadrantB < 2)? -SLED_DZ : SLED_DZ));
#endif
            svoStp_OutputDac(dac_a, dac_b);
            break;
    }
    Iram_stp_Status = 0;
}

/*-----------------------------------------------------------------------------
 *
 * Module: svoStp_JumpControl()
 * Description: calculate sledge jump interval and step size
 *
 *----------------------------------------------------------------------------*/
void svoStp_JumpControl(void)
{
    eStepMode temp_jump_mode;
    SHORT     temp_jump_target;
    LONG  current_velocity;
    ULONG temp_ulong;

#if (OPU_HomeSW > 0)
    if ((specialCase == HOMING_STEP2) && (ipHOME == 0))
    {
    #if (OPU_HomeSW > 1)
        if (++HomeSW_det_cnt >= OPU_HomeSW)
    #endif
        {
            SendMsgCn(DEBUG_SEEK_STEP_DATA_MSG,2,0x340100,A2B(Iram_stp_StepPosition));
            specialCase = HOMING_STEP3;
            svoStp_SetCurrentPosition(STEP_INWARD_POSITION);
            BrakeSledge();
        }
    }
#endif
#if (COLLI_SWITCH == 1) && (COLLI_HomeSW != 0)
    if ((specialCase == COLLI_HOMING_STEP2) && (ipSA_INT == COLLI_SA_PREINIT_VALUE))
    {
    #if (COLLI_HomeSW > 1)
        if (++HomeSW_det_cnt >= COLLI_HomeSW)
    #endif // (PCB == 692)||(PCB == 710)||(PCB == 711)||(PCB == 812)||(PCB == 830)||(PCB == 1600)||(PCB == 815)||(PCB == 817)
        {
            specialCase = (((full_mode == 1) && (stp_Settings[COLLI_MICRO_PROFILE].VFull != 0))? COLLI_HOMING_STEP3 : NORMAL_CASE);
            if (specialCase == NORMAL_CASE)
            {
                SendMsgCn(DEBUG_SEEK_STEP_DATA_MSG,2,0x340101,A2B(Iram_stp_StepPosition));
                Iram_colli_Initialized = STEP_INITIALIZED;
                svoStepVar.stepHomeCheck = FALSE;
            }
            Iram_colli_CurrentPosition = 0;
            BrakeSledge();
        }
    }
    if ((specialCase == COLLI_HOMING_STEP4) && (ipSA_INT != COLLI_SA_PREINIT_VALUE))
    {
        SendMsgCn(DEBUG_SEEK_STEP_DATA_MSG,2,0x340101,A2B(Iram_stp_StepPosition));
        Iram_colli_Initialized = STEP_INITIALIZED;
        svoStepVar.stepHomeCheck = FALSE;
        specialCase = NORMAL_CASE;
        BrakeSledge();
    }
#endif
    if ((specialCase == SEARCH_RF) && FokStatusOK() && HAL_GLOBAL_GetRfPresent())
    {
        SendMsgCn(DEBUG_SEEK_STEP_DATA_MSG,2,0x340104,A2B(Iram_stp_StepPosition));
        specialCase = NORMAL_CASE;
        BrakeSledge();
        Iram_stp_JumpDistance += 30;
    }
    if (Iram_stp_CurrentDistance >= Iram_stp_JumpDistance)
    {
#if (SLEDGE_DEBUG_MSG==1)
        SendMsgCn(DEBUG_SEEK_STEP_DATA_MSG,2,0x340208,A2B(Iram_stp_CurrentDistance));
#endif
        full_mode = 0;
        Iram_stp_Status &= (~STP_BUSY_MASK);
        if (DMO_CLIP_SLED_MOVING < svoSpeedVar.Kd14Fg)
        {
            spdSetKd14(svoSpeedVar.Kd14Fg);
        }

        svoStp_SetTimerMode(STEP_ROUGH_TIMER_STOP);

        switch (specialCase)
        {
#if (COLLI_SWITCH == 1)
    #if (COLLI_HomeSW != 0)
            case COLLI_HOMING_STEP2:
                Iram_colli_CurrentPosition = 0;
                send_msg80(SHOW_DEBUG_MSG,0x540003);//WARNING: COLLI home switch not detected
            case COLLI_HOMING_STEP4:
                if (specialCase == COLLI_HOMING_STEP4)
                {
                    send_msg81(SHOW_DEBUG_MSG,0x540005,0x01);//WARNING: COLLI home switch always detected
                }
    #endif
#endif
            case SEARCH_RF:
                if (specialCase == SEARCH_RF)
                {
                    SendMsg80(DEBUG_SEEK_STEP_DATA_MSG,0x340105); //not found during jump
                }
            case NORMAL_CASE:
#if ((COLLI_SWITCH == 1) && (COLLI_REST_TIME > 0))
                if (((Iram_stp_Status & STP_DEVICE_MASK) == STP_DEVICE_COLLI) && (Iram_stp_JumpDistance >= COLLI_REST_MIN_DIST))
                {
                    SendMsg80(DEBUG_SEEK_STEP_DATA_MSG,0x3FAA15); //StartTimer
                    StartTimer(&ColliRestTimer);
                }
#endif
                if (new_jump_mode == STEP_NULL_MODE)
                {
                    svoStp_HoldSledge();
                }
                else
                {
                    do
                    {
                        temp_jump_mode = new_jump_mode;
                        temp_jump_target = new_jump_target;
#if (COLLI_SWITCH == 1)
                        if (new_jump_mode2 != STEP_NULL_MODE)
                        {
                            new_jump_mode = new_jump_mode2;
                            new_jump_target = new_jump_target2;
                            new_jump_mode2 = STEP_NULL_MODE;
                        }
                        else
#endif
                        {
                            new_jump_mode = STEP_NULL_MODE;
                        }

#if (COLLI_SWITCH == 1)
                        if ((temp_jump_mode > STEP_MODE_MAX) ^ ((Iram_stp_Status & STP_DEVICE_MASK) == STP_DEVICE_COLLI))
                        {
                            svoStp_HoldSledge();
                        }
#endif
                        switch (temp_jump_mode)
                        {
                            case STEP_STOP_MODE:
                                svoStp_StopControl();
                                break;

                            case STEP_NEW_HOME_MODE:
                                svoStp_Homing();
                                break;
#if (COLLI_SWITCH == 1)
                            case COLLI_NEW_HOME_MODE:
                                svoColli_Homing();
                                break;
#endif
                            default:
                                svoStp_JumpToPosition(temp_jump_mode,temp_jump_target);
                                break;
                        }
                    }
                    while ((new_jump_mode != STEP_NULL_MODE) && (!svoStp_busy()));
                }
                break;

            case HOMING_STEP1:
#if (OPU_HomeSW > 0)
    #if (MSG_OUT_MODE != MSG_DISABLE)
                if (ipHOME == 0)
                {
                    SendMsg80(SHOW_DEBUG_MSG,0x340004); //WARNING: SLED home switch always detected
                }
    #endif
                svoStp_JumpSteps(STEP_HOMING_MODE,-HOMING_DISTANCE);
#else
                Iram_stp_Initialized = STEP_INITIALIZED;
    #if (DOUBLE_LENS == 1)
                if (IS_OUTER_LENS(svoVar_discKindT.Bit.DiscStd))
                {
                    svoStp_SetCurrentPosition(STEP_OUTWARD_POSITION + STEP_LENS_OFFSET);
                }
                else
    #endif
                {
                    svoStp_SetCurrentPosition(STEP_OUTWARD_POSITION);
                }
    #if (HOMING_POST_DISTANCE != 0)
                svoStp_JumpSteps(STEP_FULL_MODE,-HOMING_POST_DISTANCE);
                specialCase = HOMING_STEP2;
    #else
                svoStp_JumpToPosition(STEP_SAFE_SLOW_MODE, svoStepVar.StepHomePos);
    #endif
                svoStepVar.stepHomeCheck = FALSE;
#endif
                break;

            case HOMING_STEP2:
#if (OPU_HomeSW > 0)
                SendMsg80(SHOW_DEBUG_MSG,0x340002); //WARNING: SLED home switch not detected
                svoStp_SetCurrentPosition(STEP_INWARD_POSITION);
            case HOMING_STEP3:
                Iram_stp_Initialized = STEP_INITIALIZED;
#endif
	#if (Customer_A1==1)
                svoStp_JumpToPosition(STEP_HOMING2_MODE, svoStepVar.StepHomePos);
	#else
                svoStp_JumpToPosition(STEP_SAFE_SLOW_MODE, svoStepVar.StepHomePos);
	#endif
#if (OPU_HomeSW > 0)
                svoStepVar.stepHomeCheck = FALSE;
#endif
                break;

#if (COLLI_SWITCH == 1)
            case COLLI_HOMING_STEP1:
    #if (COLLI_HomeSW != 0)
        #if (MSG_OUT_MODE != MSG_DISABLE)
                if (ipSA_INT == COLLI_SA_PREINIT_VALUE)
                {
                    send_msg81(SHOW_DEBUG_MSG,0x540005,0x00); //WARNING: COLLI home switch always detected
                }
        #endif
    #endif
                svoStp_JumpSteps(COLLI_NORMAL_NO_REST_MODE,-COLLI_HOMING_DISTANCE);
                specialCase = COLLI_HOMING_STEP2;
                break;
    #if (COLLI_HomeSW == 0)
            case COLLI_HOMING_STEP2:
        #if (PCB == 815)
                Iram_colli_CurrentPosition = -320;
        #else
                Iram_colli_CurrentPosition = 0;
        #endif
                Iram_colli_Initialized = STEP_INITIALIZED;
                svoStepVar.stepHomeCheck = FALSE;
                svoStp_JumpSteps(COLLI_MICRO_NO_REST_MODE, COLLI_HOMING_POST_DISTANCE);
                break;
    #else
            case COLLI_HOMING_STEP3:
                svoStp_JumpToPosition(COLLI_MICRO_NO_REST_MODE, COLLI_HOMING_POST_DISTANCE);
                specialCase = COLLI_HOMING_STEP4;
                break;
    #endif
#endif
        }
#if (COLLI_SWITCH == 1)
        if (Iram_stp_Status == 0)
        {
            StepPosition    = (&Iram_stp_StepPosition);
            CurrentPosition = (&Iram_stp_CurrentPosition);
        }
#endif
        return;
    }

    Iram_stp_CurrentDistance += move_steps;
    temp_ulong = Iram_stp_MoveInterval;
    Iram_stp_CurrentTime += temp_ulong;

    if ((Iram_stp_CurrentSpeedPhase < DEC_PHASE) && (Iram_stp_CurrentDistance >= Iram_stp_BrakeDistance))
    {
#if (SLEDGE_DEBUG_MSG==1)
        SendMsgCn(DEBUG_SEEK_STEP_DATA_MSG,2,0x340203,A2B(Iram_stp_CurrentDistance));
#endif
        Iram_stp_CurrentSpeedPhase = DEC_PHASE;
        Iram_stp_CurrentTime = (Iram_stp_CurrentDistance - Iram_stp_BrakeDistance) * temp_ulong / move_steps;
    }

    switch (Iram_stp_CurrentSpeedPhase)
    {
        case ACC_PHASE:
            current_velocity = stp_Settings[Iram_stp_CurrentProfile].Acc * Iram_stp_CurrentTime / 4 + min_velocity;

            if ((CE_Gain_stage == 1) && (current_velocity > Rough_CEGain_SwitchPoint) && ((Iram_stp_Status & STP_DEVICE_MASK) == STP_DEVICE_NORMAL))
            {
                seekSetCEEq(eRoughSeekRatioOnly, 128);
                CE_Gain_stage = 2;
            }

            if (current_velocity >= max_velocity)
            {
#if (SLEDGE_DEBUG_MSG==1)
                SendMsgCn(DEBUG_SEEK_STEP_DATA_MSG,2,0x340202,A2B(Iram_stp_CurrentDistance));
#endif
                current_velocity = max_velocity;
                Iram_stp_CurrentSpeedPhase = VMAX_PHASE;
                Iram_stp_CurrentTime = 0;
            }

#if (Customer_A1==1)
		if (Iram_stp_CurrentProfile == STEP_HOMING_PROFILE)
		{		
				svoStp_SetGain(65);
		}

		if ((svoVar_discKindT.Bit.DiscStd == eDiscBD)) 
		{
				if(Iram_stp_CurrentProfile == STEP_NORMAL_PROFILE)
				{
						svoStp_SetGain(72);					
				}
		}
#endif
            break;

        case VMAX_PHASE:
            current_velocity = max_velocity;

#if( Customer_A1==1)
						if ((svoVar_discKindT.Bit.DiscStd == eDiscBD)) 
						{
								if(Iram_stp_CurrentProfile == STEP_NORMAL_PROFILE)
								{
										svoStp_SetGain(35);
								}
						}
#endif

			
#if (OPU_HomeSW == 0)
            if ((Iram_stp_CurrentProfile == STEP_HOMING_PROFILE) && (CurrentGain > SLED_HOMING_GAIN))
            {
                /* smoothly reduce sledge gain from SLED_JUMP_GAIN to SLED_HOMING_GAIN for STEP_HOMING_PROFILE */
                svoStp_SetGain(CurrentGain - 1);
            }
#endif
#if( Customer_A1==1)
						if (Iram_stp_CurrentProfile == STEP_HOMING_PROFILE)
						{		
								svoStp_SetGain(40);
						}
#endif		
            break;

        case DEC_PHASE:
            current_velocity = max_velocity - stp_Settings[Iram_stp_CurrentProfile].Dec * (Iram_stp_CurrentTime + (temp_ulong / 2)) / 4;

            if ((CE_Gain_stage == 2) && (current_velocity <= Rough_CEGain_SwitchPoint) && ((Iram_stp_Status & STP_DEVICE_MASK) == STP_DEVICE_NORMAL))
            {
                seekSetCEEq(eRoughSeekRatioOnly, CE_Gain_Down_Ratio);
                CE_Gain_stage = 0;
            }

            if (full_mode == 1)
            {
                if (current_velocity < full_velocity)
                {
#if (SLEDGE_DEBUG_MSG==1)
                    SendMsgCn(DEBUG_SEEK_STEP_DATA_MSG,2,0x340205,A2B(Iram_stp_CurrentDistance));
#endif
                    full_mode = 0;
                }
            }

            if (current_velocity <= min_velocity)
            {
#if (SLEDGE_DEBUG_MSG==1)
                SendMsgCn(DEBUG_SEEK_STEP_DATA_MSG,2,0x340204,A2B(Iram_stp_CurrentDistance));
#endif
                current_velocity = min_velocity;
                Iram_stp_CurrentSpeedPhase = END_PHASE;
                Iram_stp_CurrentTime = 0;
            }
            break;

        case END_PHASE:
            current_velocity = min_velocity;
            break;
    }

    svoStp_MoveStep((Iram_stp_CurrentDirection == FORW)? move_steps : -move_steps);

    move_steps = current_velocity / (1000 * 4 * 8192 / MIN_MOVE_INTERVAL_US) + 1;
    if ((full_mode == 0) && (Iram_stp_CurrentSpeedPhase == ACC_PHASE) &&
        (current_velocity >= full_velocity))
    {
        //make sure we switch to full mode (square wave) at correct phase
        temp_ulong = *StepPosition & 0x001F;
        if (Iram_stp_CurrentDirection == FORW)
        {
            temp_ulong = 32 - temp_ulong;
        }

        if (temp_ulong >= 31)
        {
#if (SLEDGE_DEBUG_MSG==1)
            SendMsgCn(DEBUG_SEEK_STEP_DATA_MSG,2,0x340206,A2B(Iram_stp_CurrentDistance));
#endif
            full_mode = 1;
            svoStp_MoveStep(0);
        }
        else
        {
            if ((temp_ulong > move_steps) && (temp_ulong < (move_steps * 2)))
            {
                move_steps = temp_ulong + 1;
            }
        }
    }

    if (full_mode == 1)
    {
        move_steps = 32;
    }

    Iram_stp_MoveInterval = ((ULONG)move_steps * (8192 * 8192)) / (current_velocity / 4);
    temp_ulong = ((ULONG)move_steps * (CLK16_1000_US * 4 * 8192)) / current_velocity;

    if (Iram_stp_CurrentDistance >= Iram_stp_JumpDistance)
    {   //keep end dac output a while at the end of sledge jump
#if (COLLI_SWITCH == 1)
        temp_ulong = ((Iram_stp_Status & STP_DEVICE_MASK) == STP_DEVICE_NORMAL)? SLED_EXT_INTERVAL_CLK16 : COLLI_EXT_INTERVAL_CLK16;
#else
        temp_ulong = SLED_EXT_INTERVAL_CLK16;
#endif
        switch (specialCase)
        {
            default:
#if (COLLI_SWITCH == 1)
                if ((new_jump_mode == STEP_NULL_MODE) || ((new_jump_mode <= STEP_MODE_MAX) ^ ((Iram_stp_Status & STP_DEVICE_MASK) == STP_DEVICE_NORMAL)))
#else
                if (new_jump_mode == STEP_NULL_MODE)
#endif
                {
                    break;
                }
            case HOMING_STEP1:
            case HOMING_STEP2:
#if (OPU_HomeSW > 0)
            case HOMING_STEP3:
#endif
#if COLLI_SWITCH == 1
            case COLLI_HOMING_STEP1:
    #if COLLI_HomeSW == 0
            case COLLI_HOMING_STEP2:
    #else
            case COLLI_HOMING_STEP3:
    #endif
#endif
                Set_Timer_Int_NON_STOP(&TIMER5, 1);
                return;
        }
    }

    SetTimerXCompareValue(TIMER5,temp_ulong);
    current_velocity = GetTimerXValue(TIMER5) - temp_ulong;
    if (current_velocity >= 0)
    {
        /* This message frequently show up means FW loading too high, increase MIN_MOVE_INTERVAL_US */
        SendMsgCn(SHOW_DEBUG_MSG,2,0x340001,A2B(current_velocity)); // WARNING: stp control timer overload (%u)
        Set_Timer_Int_NON_STOP(&TIMER5, 1);
    }
}

/*-----------------------------------------------------------------------------
 *
 * Module: BrakeSledge()
 * Description: use STEP_NORMAL_PROFILE setting to brake the sledge
 *
 *----------------------------------------------------------------------------*/
static void BrakeSledge(void)
{
    ULONG  current_velocity;
    USHORT target_position;

    // sledge is still moving, use Dec of STEP_NORMAL_PROFILE to brake to stop
    switch (Iram_stp_Status & STP_DEVICE_MASK)
    {
#if (COLLI_SWITCH == 1)
        case STP_DEVICE_COLLI:
            Iram_stp_CurrentProfile = COLLI_NORMAL_PROFILE;
            break;
#endif
        case STP_DEVICE_NORMAL:
            Iram_stp_CurrentProfile = STEP_NORMAL_PROFILE;
            break;
    }
    svoStp_SetGain(stp_Settings[Iram_stp_CurrentProfile].Gain);
    current_velocity = min_velocity;
    if (Iram_stp_MoveInterval > 0)
    {
        current_velocity = ((ULONG)move_steps * (8192 * 4 * 8192)) / Iram_stp_MoveInterval;
        if (current_velocity < min_velocity)
        {
            current_velocity = min_velocity;
        }
    }

    max_velocity = current_velocity;
    current_velocity = (current_velocity - min_velocity) / 8192;
    full_velocity = (ULONG)stp_Settings[Iram_stp_CurrentProfile].VFull * (4 * 8192);
    Iram_stp_JumpDistance = Iram_stp_CurrentDistance;
    if (current_velocity > 0)
    {
        Iram_stp_JumpDistance += (current_velocity * current_velocity) / (2 * stp_Settings[Iram_stp_CurrentProfile].Dec) + SLED_JUMP_END_DIST;
        Iram_stp_CurrentSpeedPhase = DEC_PHASE;
    }
    else
    {
        Iram_stp_CurrentSpeedPhase = END_PHASE;
    }
    Iram_stp_BrakeDistance = Iram_stp_CurrentDistance;
    Iram_stp_CurrentTime = 0;

    target_position = (*CurrentPosition) + ((Iram_stp_CurrentDirection == FORW)? (Iram_stp_JumpDistance - Iram_stp_CurrentDistance) : (Iram_stp_CurrentDistance - Iram_stp_JumpDistance));
    switch (Iram_stp_Status & STP_DEVICE_MASK)
    {
#if (COLLI_SWITCH == 1)
        case STP_DEVICE_COLLI:
    #if (OPU == BDPL2)
            HAL_LASC_SetAPCTarget(target_position);
    #endif
            break;
#endif
        case STP_DEVICE_NORMAL:
            if (!svoStepVar.stepHomeCheck)
                calUpdateRadius(target_position, FALSE);
            break;
    }

#if (SLEDGE_DEBUG_MSG==1)
    SendMsgCn(DEBUG_SEEK_STEP_DATA_MSG,2,0x340207,A2B(Iram_stp_CurrentDistance));
#endif
}


/*-----------------------------------------------------------------------------
 *
 * Module: svoStp_SetGain()
 * Description: preset stp_BaseDac table for different gain
 *
 *----------------------------------------------------------------------------*/
void svoStp_SetGain(BYTE gain)
{
    if (CurrentGain != gain)
    {
        svoPrvtVar_OutputFullDac[0] = svoPrvtVar_OutputFullDac[1] =
            ((((ULONG)gain * (ULONG)stp_BaseDac[1][0] + 64) / 128) & 0x3FF) - SLED_DZ;
        svoPrvtVar_OutputFullDac[2] = svoPrvtVar_OutputFullDac[3] =
            ((((ULONG)gain * (ULONG)stp_BaseDac[3][0] + 64) / 128) & 0x3FF) + SLED_DZ;
        CurrentGain = gain;
    }
}


/*-----------------------------------------------------------------------------
 *
 * Module: svoStp_SetCurrentPosition()
 * Description:
 *
 *----------------------------------------------------------------------------*/
void svoStp_SetCurrentPosition(USHORT StpPosition)
{
    Iram_stp_CurrentPosition = StpPosition;
    if (!svoStepVar.stepHomeCheck)
        calUpdateRadius(StpPosition, TRUE);
}

/*-----------------------------------------------------------------------------
 *
 * Module: svoStp_SetStepThreshold()
 * Description: preset StepThreshold for step following
 *
 *----------------------------------------------------------------------------*/
void svoStp_SetStepThreshold(BYTE direction)
{
    SBYTE threshold_f,threshold_b;

    switch (svoVar_discKindT.Bit.DiscStd)
    {
        case eDiscDVD:
#if (DVD_RAM_READ == 1)
            if (DVDRAM)
            {
                threshold_f = DVRAM_THRESHOLD_F;
                threshold_b = DVRAM_THRESHOLD_B;
            }
            else
#endif
            {
                threshold_f = DV_THRESHOLD_F;
                threshold_b = DV_THRESHOLD_B;
            }
            svoStepVar.StepThresholdDelayTarget = SLED_THRESHOLD_DELAY_DVD;
            break;
#if (ENABLE_HDDVD == 1)
        case eDiscHD:
            threshold_f = HD_THRESHOLD_F;
            threshold_b = HD_THRESHOLD_B;
            svoStepVar.StepThresholdDelayTarget = SLED_THRESHOLD_DELAY_BDHD;
            break;
#endif
#if (BD_ENABLE == 1)
        case eDiscBD:
            threshold_f = BD_THRESHOLD_F;
            threshold_b = BD_THRESHOLD_B;
            svoStepVar.StepThresholdDelayTarget = SLED_THRESHOLD_DELAY_BDHD;
            break;
#endif
        case eDiscCD:
        default:
            threshold_f = CD_THRESHOLD_F;
            threshold_b = CD_THRESHOLD_B;
            svoStepVar.StepThresholdDelayTarget = SLED_THRESHOLD_DELAY_CD;
            break;
    }

    if (direction == FORW)
    {
        svoStepVar.StepThresholdPosTarget = threshold_f;
        svoStepVar.StepThresholdNegTarget = -threshold_b;
    }
    else
    {
        svoStepVar.StepThresholdPosTarget = threshold_b;
        svoStepVar.StepThresholdNegTarget = -threshold_f;
    }
    svoStepVar.StepThresholdPos = svoStepVar.StepThresholdPosTarget;
    svoStepVar.StepThresholdNeg = svoStepVar.StepThresholdNegTarget;
    svoStepVar.StepThresholdDelay = 0;
}

/*-----------------------------------------------------------------------------
 *
 * Module: svoStp_SetTimerMode()
 * Description:
 *
 *----------------------------------------------------------------------------*/
void svoStp_SetTimerMode(eStepTimerMode TimerMode)
{
    BYTE temp_IE;

    temp_IE = GET_INT_ENABLE_MASK();
    DISABLE_INTERRUPT();
#if (ACTIVE_SLED_FOR_FINE_JUMP == 1)
    if (TimerMode == STEP_ROUGH_TIMER_START)
    {
        TimerMode = (Iram_stp_TimerMode & STEP_READ_TIMER)? STEP_ROUGH_TIMER_READ : STEP_ROUGH_TIMER;
    }
    else if (TimerMode == STEP_ROUGH_TIMER_STOP)
    {
        TimerMode = (Iram_stp_TimerMode & STEP_READ_TIMER)? STEP_READ_TIMER : STEP_DISABLE_TIMER;
    }
    else
    {
        if (Iram_stp_TimerMode & STEP_ROUGH_TIMER)
        {
            Iram_stp_TimerMode = (TimerMode == STEP_READ_TIMER)? STEP_ROUGH_TIMER_READ : STEP_ROUGH_TIMER;
            SET_INT_ENABLE_MASK(temp_IE);
            return;
        }
    }
#endif

    if (Iram_stp_TimerMode == TimerMode)
    {
        SET_INT_ENABLE_MASK(temp_IE);
        return;
    }

    if (Iram_stp_TimerMode == STEP_DISABLE_TIMER)
    {
        STOP_TIMER(TIMER5);
        CLR_TIMER_STATUS(TIMER5);
        ENABLE_TIMER_INT(TIMER5);
    }

    switch(TimerMode)
    {
#if (ACTIVE_SLED_FOR_FINE_JUMP == 0)
        case STEP_FINE_TIMER:
            Set_Timer_Int_NON_STOP(&TIMER5, CLK16_1000_US);              // set to 1.0 ms
            break;
#else
        case STEP_ROUGH_TIMER_READ:
#endif
        case STEP_ROUGH_TIMER:
#if (COLLI_SWITCH == 1)
            Set_Timer_Int_NON_STOP(&TIMER5, ((Iram_stp_Status & STP_DEVICE_MASK) == STP_DEVICE_NORMAL)? SLED_INIT_INTERVAL_CLK16 : COLLI_INIT_INTERVAL_CLK16);
#else
            Set_Timer_Int_NON_STOP(&TIMER5, SLED_INIT_INTERVAL_CLK16);
#endif
            break;
#if ((COLLI_SWITCH == 1) && (COLLI_REST_TIME > 0))
        case STEP_REST_TIMER:
            TimerMode = (Iram_stp_TimerMode & STEP_READ_TIMER)? STEP_ROUGH_TIMER_READ : STEP_ROUGH_TIMER;
            break;
#endif
        case STEP_READ_TIMER:
            svoStepVar.StepThresholdPos = svoStepVar.StepThresholdPosTarget;
            svoStepVar.StepThresholdNeg = svoStepVar.StepThresholdNegTarget;
            svoStepVar.StepThresholdDelay = 0;
            svoStepVar.SledCounter = 0;
            Set_Timer_Int_NON_STOP(&TIMER5, CLK16_1000_US);  // set to 1 ms
            break;

        case STEP_DISABLE_TIMER:
            DISABLE_TIMER_INT(TIMER5);
            break;
    }// end switch(TimerMode)
    Iram_stp_TimerMode = TimerMode;
    SET_INT_ENABLE_MASK(temp_IE);
}

/*-----------------------------------------------------------------------------
 *
 * Module: svoStp_StopControl()
 * Description:
 *
 *----------------------------------------------------------------------------*/
void svoStp_StopControl( void )
{
    BYTE temp_IE;
#if DO_HW_SUPPORT_STEP == 1
    BYTE stp_pos;

    FillMicroSteppingTable(0);             // 192 --> or 75%   (unit 2^-8)

    stp_pos = READ_REG(HAL_SRVREG_MAP->STPPOS);
    WRITE_REG(HAL_SRVREG_MAP->STPCTRL, (0xC0 + stp_pos));   // switch to external control mode and use the
                                                   // orginal positon (don't move)
    WRITE_REG(HAL_SRVREG_MAP->STPCTRL, (0xC0 + stp_pos));   // switch to external control mode and use the
                                                   // orginal positon (don't move)
#else // DO_HW_SUPPORT_STEP == 1
    if (svoStp_busy())
    { // Sledge is moving, brake before stop.
#if (ACTIVE_SLED_FOR_FINE_JUMP == 1)
        if (Iram_stp_TimerMode == STEP_ROUGH_TIMER_READ)
        {
            Iram_stp_TimerMode = STEP_ROUGH_TIMER;
        }
#endif
        temp_IE = GET_INT_ENABLE_MASK();
        DISABLE_INTERRUPT();
        // New jump command recived during jumping, brake the sledge and store the new jump target.
#if (COLLI_SWITCH == 1)
        if (((Iram_stp_Status & STP_DEVICE_MASK) == STP_DEVICE_NORMAL) && (svoStepVar.stepHomeCheck == FALSE))
#endif
        {
            //only brake when the new command is for same device
            BrakeSledge();
        }
#if (COLLI_SWITCH == 1)
        if (new_jump_mode > STEP_MODE_MAX)
        {
            new_jump_mode2 = STEP_STOP_MODE;
        }
        else
#endif
        {
            new_jump_mode = STEP_STOP_MODE;
        }
        SET_INT_ENABLE_MASK(temp_IE);
        return;
    }
    svoStp_OutputDac(0x00, 0x00);
#if (COLLI_SWITCH == 1)
    COLLI_DRIVER_OFF;
    #if (COLLI_ANALOG_OUTPUT == 1)
    Colli_OutputDac(0x00, 0x00);
    #endif
#endif
    svoStp_SetTimerMode( STEP_DISABLE_TIMER );
    Iram_stp_Status = 0;
    if (DMO_CLIP_SLED_MOVING < svoSpeedVar.Kd14Fg)
    {
        spdSetKd14(svoSpeedVar.Kd14Fg);
    }
#endif // DO_HW_SUPPORT_STEP == 1
}

//****************************************************************************************
//****************************************************************************************
//                                                                                       *
// code developed below are for H/W with Step-Motor-Support                              *
//                                                                                       *
//****************************************************************************************
//****************************************************************************************

#if DO_HW_SUPPORT_STEP == 1
//*****************************************************************************
//                                                                            *
// FillMicroSteppingTable collect contents in TblSin64 and put it into        *
// micro stepping table. It is used to do H/W step motor driving              *
//                                                                            *
//*****************************************************************************
void FillMicroSteppingTable(SHORT gain  /* 2^-8 */)
{
    USHORT ux, uy;
    SHORT   temp,temp2;

    //---------------------------------------------------------------------;
    // keep the gain within working range                                  ;
    //---------------------------------------------------------------------;
    if (gain > 256) gain = 256;
    if (gain < 0  ) gain = 0;

    //---------------------------------------------------------------------;
    // copy the (every other entry) FW stepper tables into HW tables       ;
    //---------------------------------------------------------------------;
    for (ux = 0; ux < SIZE_MIRCO_STEP_TBL; ux++)
    {
        uy = ux << 1;
        WRITE_REG(HAL_SRVREG_MAP->STPADD, ux);    // select the entry
        //---------------------------------------------------------------------;
        // work on DAC1                                                        ;
        //---------------------------------------------------------------------;
        temp   = (SHORT)stp_BaseDac[uy/32][uy % 32];
        temp  *= gain;
        temp >>= 8;
        #if SLED_DZ > 0
        temp += ((uy < 64)? -SLED_DZ : SLED_DZ);
        #endif
        temp >>= 2;
        //---------------------------------------------------------------------;
        // work on DAC2                                                        ;
        //---------------------------------------------------------------------;
        uy = uy + 96;
        if (uy > 128) uy -= 128;
        temp2   = (SHORT)stp_BaseDac[uy/32][uy % 32];
        temp2  *= gain;
        temp2 >>= 8;
        #if SLED_DZ > 0
        temp2 += ((uy < 64)? -SLED_DZ : SLED_DZ);
        #endif
        temp2 >>= 2;
        WRITE_REG(HAL_SRVREG_MAP->STPDAC, (SHORT)((temp2 << 8) | temp));
    }// ux for loop
}// end of FillMicroSteppingTable
#endif //DO_HW_SUPPORT_STEP

#if DO_HW_SUPPORT_STEP == 1
#define INIT_VEL_SPEED       (700)   // unit: micro-step per ms
#define HW_STP_PROF_OFFSET   (400)   // unit in micro-step per ms
//*****************************************************************************
//                                                                            *
//                                                                            *
//                       - - - - - - - - - - - -                              *
//                     /                         \                            *
//                    /                           \                           *
//                   /                             \                          *
//              ---------------------------------------------------           *
//                                                 ^                          *
//                                                 |                          *
//                                               target                       *
//                                                                            *
//*****************************************************************************
HW_speed_profile_t HW_step_profile(SHORT start_loc,         // unit in (micro-step)
                                   SHORT target_loc,        // unit in (micro-step)
                                   SHORT curr_loc,          // unit in (micro-step)
                                   SHORT profile_limit)     // unit in (micro-step per 0.1 sec)
{
    LONG         sltemp;
    SHORT        sstemp, acc_temp, dec_temp;
    HW_speed_profile_t  res;

    sstemp = ABS(curr_loc - start_loc);                           // distance has been moved so far
    sltemp = ((LONG)sstemp * 16) + (LONG)INIT_VEL_SPEED;
    if (sltemp >= 32767)
        acc_temp = 32767;
    else
        acc_temp = (SHORT)sltemp;

    sstemp = ABS(target_loc - curr_loc);                              // distance to the target
    sltemp = ((LONG)sstemp * 16) + (LONG)HW_STP_PROF_OFFSET;
    if (sltemp >= 32767)
        dec_temp = 32767;
    else
        dec_temp = (SHORT)sltemp;

    if (acc_temp < dec_temp)
    {
        res.profile_speed = acc_temp;
        res.accele_phase  = TRUE;
        res.decele_phase  = FALSE;
    }
    else
    {
        res.profile_speed = dec_temp;
        res.accele_phase  = FALSE;
        res.decele_phase  = TRUE;
    }

    if (res.profile_speed >= (profile_limit*1.35))
    {
        res.profile_speed = profile_limit*1.35;
        res.accele_phase  = FALSE;
        res.decele_phase  = FALSE;
    }

    return(res);
}// end of HW_step_profile
#endif // DO_HW_SUPPORT_STEP == 1


#if DO_HW_SUPPORT_STEP == 1
#define LIM_GEAR_1_TO_2    (100)     /* unit in (0.1 ms) */
#define LIM_GEAR_2_TO_4    (200)     /* unit in (0.1 ms)    */
#define LIM_GEAR_4_TO_8    (400)    /* unit in (0.1 ms) */
#define LIM_GEAR_8_TO_16   (800)   /* unit in (0.1 ms)  */
#define MIN_DCO_T_IN_P1MS  (9)      /* unit in (0.1 ms) */
//*****************************************************************************
//                                                                            *
// "conver_vel_to_sled_dac" return the sled DAC value and set the step size   *
// based on the required velocity and the DCO clk                             *
//                                                                            *
//*****************************************************************************
SHORT conver_vel_to_sled_dac(SHORT vel) // vel: unit (micro-step per 0.1 s)
{
    LONG        sltemp;
    LONG        min_dco_out;
    BYTE      stp_dco;
    BYTE      stp_size;
    BYTE      desired_stp_size;
    BYTE      cur_pos;
    SHORT       dco_frq;
    SHORT       abs_vel;
    SHORT       abs_sled;
    SHORT       max_dco_in;

    stp_dco  = READ_REG(HAL_SRVREG_MAP->STPDCO);
    switch (stp_dco)
    {
        case 0:
            dco_frq = 5512;     // 5512 = 4*1378   unit: Hz
            break;

        case 1:
            dco_frq = 11024;    // 11024 = 8*1378  unit: Hz
            break;

        case 2:
            dco_frq = 22048;    // 22048 = 16*1378 unit: Hz
    }

    abs_vel = ABS(vel);

    //-----------------------------------------------------------;
    // determinate what gear it should be based on input mag     ;
    //-----------------------------------------------------------;
    if      (abs_vel < LIM_GEAR_1_TO_2)          // try to use step size as 1
        desired_stp_size = 1;
    else if (abs_vel < LIM_GEAR_2_TO_4)          // try to use step size as 2
        desired_stp_size = 2;
    else if (abs_vel < LIM_GEAR_4_TO_8)          // try to use step size as 4
        desired_stp_size = 4;
    else if (abs_vel < LIM_GEAR_8_TO_16)         // try to use step size as 8
        desired_stp_size = 8;
    else
        desired_stp_size = 16;                   // try to use step size as 16

    //-----------------------------------------------------------;
    // determinate what gear it should be based on input mag     ;
    //-----------------------------------------------------------;
    stp_size = READ_REG(HAL_SRVREG_MAP->STPSIZE) + 1;
    cur_pos  = READ_REG(HAL_SRVREG_MAP->STPPOS);
    if (desired_stp_size <= stp_size)
    {
        WRITE_REG(HAL_SRVREG_MAP->STPSIZE, (desired_stp_size - 1));
    }
    else
    {
        //-----------------------------------------------------------;
        // Do not alter the sequence of the case !! It has to start  ;
        // from the max step size and go lower one by one.           ;
        // Changing it will affect the acceleration wave form        ;
        //-----------------------------------------------------------;
        switch (desired_stp_size)
        {
            case 16:
                if ((cur_pos & 0x0F) == 0x08)
                {
                    WRITE_REG (HAL_SRVREG_MAP->STPSIZE, 0x0F); // assigned 16
                    break;
                }

            case 8:
                if ((cur_pos & 0x07) == 0x00)
                {
                    WRITE_REG (HAL_SRVREG_MAP->STPSIZE, 0x07); // assigned 8
                    break;
                }

            case 4:
                if ((cur_pos & 0x03) == 0x00)
                {
                    WRITE_REG (HAL_SRVREG_MAP->STPSIZE, 0x03); // assigned 4
                    break;
                }

            case 2:
                if ((cur_pos & 0x01) == 0x00)
                {
                    WRITE_REG (HAL_SRVREG_MAP->STPSIZE, 0x01); // assigned 2
                    break;
                }

            case 1:
                WRITE_REG (HAL_SRVREG_MAP->STPSIZE, 0x00); // assigned 1
        }// switch
    }//if (desired_stp_size <= stp_size)

    stp_size = ReadSCReg_1B(HAL_SRVREG_MAP->STPSIZE) + 1;

    //-----------------------------------------------------------;
    // calculate min_dco_out:                                    ;
    // input: min. peroid (unit in 0.1 ms)                       ;
    //                                                           ;
    //                 min_peroid                                ;
    // min_dco_out = --------------                              ;
    //                 dco_frq^-1                                ;
    //                                                           ;
    //             =   min_period * dco_frq                      ;
    //                                                           ;
    // ex. min_period = 1 ms = 10 (ms/10)                        ;
    //     dco_frq           = 22048 Hz                          ;
    //                                                           ;
    // --> min_dco_out = 10 * 22048 / 10000                      ;
    //                 = 22.048 = 22                             ;
    //                                                           ;
    //-----------------------------------------------------------;
    min_dco_out  = MIN_DCO_T_IN_P1MS;       // unit in 0.1 ms
    min_dco_out *= (LONG)dco_frq;

    if (stp_size == 16)
    {
        // this is a special case. In this case, system alway output two continue
        // same control effort. Therefore, min time duration can be cut to a half
        min_dco_out /= 2;
    }

    min_dco_out /= 10000;

    //-----------------------------------------------------------;
    // Calculate max_dco_in:                                     ;
    //                                                           ;
    // y = -4x + 262 (this is based on DCO table inside the chip);
    //                                                           ;
    // ex. if min_dco_out = 22                                   ;
    //        max_doc_out = (262 - 22) >> 2                      ;
    //                    = 60                                   ;
    //-----------------------------------------------------------;
    max_dco_in  = (262 - (SHORT)min_dco_out) >> 2;

    //---------------------------------------------------------------------------;
    // Calculate value send to DCO based on demanded velocity:                   ;
    //                                                                           ;
    //     stp_size (micro_step) * dco_frq (Hz)                                  ;
    //  ----------------------------------------- = Tbl_DCO output               ;
    //  vel (micro_step/100 ms) * 10 (100 ms/sec)                                ;
    //                                                                           ;
    //                                           ~= -(4 * Tbl_DCO input) + 262   ;
    //                                                                           ;
    // or                                                                        ;
    //                            stp_size (micro_step) * dco_frq (Hz)           ;
    //    input = (1/4)*[262 - -----------------------------------------]        ;
    //                         vel (micro_step/100 ms) * 10 (100 ms/sec)         ;
    //                                                                           ;
    //---------------------------------------------------------------------------;
    sltemp   = (LONG)stp_size * (LONG)dco_frq;
    sltemp  /= ((LONG)abs_vel * (LONG)10);
    abs_sled = (262 - (SHORT)sltemp )/4;


    if (abs_sled > max_dco_in)
    {
        abs_sled = max_dco_in;                               // limit the max input (or min period)
    }

    if (vel >= 0)
        return(abs_sled);
    else
        return(-abs_sled);
}// end of conver_vel_to_sled_dac
#endif // DO_HW_SUPPORT_STEP == 1



#if DO_HW_SUPPORT_STEP == 1
#define MAX_LOOP_CNT           (0x10000000)
//*****************************************************************************
//                                                                            *
// StepMotorSupport_seek do profile seeking based on the input max speed      *
// limitation                                                                 *
//                                                                            *
//*****************************************************************************
BOOL StepMotorSupport_seek(SHORT   micro_step_move,          // unit:   micro-step
                           SHORT   speed_limit)              // unit:   micro-step per 100 ms
{
    ULONG      loop_cnt;
    SHORT       target_location;
    SHORT       start_stp_loc;
    SHORT       vel_command;
    BYTE        stp_pos,temp_IE;
    BOOL               seek_out;
    signed_int_read_t  StepMotorSupport_Location;
    HW_speed_profile_t profile_res;

    if (micro_step_move == 0)
        return(TRUE);

    temp_IE = GET_INT_ENABLE_MASK();
    DISABLE_INTERRUPT();
    WRITE_REG (HAL_SRVREG_MAP->STPDCO, 0x02);
    SET_INT_ENABLE_MASK(temp_IE);

    FillMicroSteppingTable(250);                  // 192 --> or 75%   (unit 2^-8)

    WRITE_REG(HAL_SRVREG_MAP->STPSIZE, 0x00);               // alway assigned to 1. or there will be problem !
    stp_pos = READ_REG(HAL_SRVREG_MAP->STPPOS);
    WRITE_REG(HAL_SRVREG_MAP->STPCTRL, (0x40 + stp_pos));   // goes to internal control mode
    WRITE_REG(HAL_SRVREG_MAP->STPPWR, 80);                  // 3.6 ms (given step motor support clk being 4*1378 Hz )

    //------------------------------------------------;
    // prepare the target and seeking direction       ;
    //------------------------------------------------;
    temp_IE = GET_INT_ENABLE_MASK();
    DISABLE_INTERRUPT();
    StepMotorSupport_Location = read_stploc();
    target_location           = StepMotorSupport_Location.val + micro_step_move;
    if (target_location > StepMotorSupport_Location.val)
        seek_out = TRUE;
    else
        seek_out = FALSE;
    SET_INT_ENABLE_MASK(temp_IE);

    StepMotorSupport_Location = read_stploc();
    if (StepMotorSupport_Location.succ == FALSE)
        return(FALSE);

    start_stp_loc = StepMotorSupport_Location.val;
    for (loop_cnt = 0; loop_cnt < MAX_LOOP_CNT; loop_cnt++)
    {
        profile_res = HW_step_profile(start_stp_loc,                 // starting loc
                                      target_location,               // ending loc
                                      StepMotorSupport_Location.val, // curr loc
                                      speed_limit);                  // limitation speed

        //-------------------------------------------------;
        // apply the correct sign (or direction)           ;
        //-------------------------------------------------;
        if (seek_out == TRUE)
            vel_command =    profile_res.profile_speed;
        else
            vel_command = -1*profile_res.profile_speed;

        StepMotorSupport_SLED = conver_vel_to_sled_dac(vel_command);

        //-------------------;
        // program Ks17      ;
        //-------------------;
        WRITE_DSP_COEF(ks17,StepMotorSupport_SLED);

        //-----------------------;
        // PUFWD ---> TSV = 1    ;
        //-----------------------;
        WRITE_REG(HAL_SRVREG_MAP->PUFWD, HAL_SRV_TSVFWD);    /* CE ON */

        //---------------------------------------------------------------------;
        // Wait a period of time (period can be changed. HW will drive the     ;
        // step motor according to "StepMotor_Support_vel_command"             ;
        //---------------------------------------------------------------------;
        if ((profile_res.accele_phase == FALSE) && (profile_res.decele_phase == FALSE))
        {
            WaitTimer(0);
        }


        StepMotorSupport_Location = read_stploc();
        if (StepMotorSupport_Location.succ == FALSE)
        {
            temp_IE = GET_INT_ENABLE_MASK();
            DISABLE_INTERRUPT();
            WRITE_REG(HAL_SRVREG_MAP->STPDCO, 0x00);                  // set back to 4x1378 Hz
            WRITE_REG(HAL_SRVREG_MAP->STPPWR, 20);                   // 3.6 ms (given step motor support clk being 4*1378 Hz )
            SET_INT_ENABLE_MASK(temp_IE);
            svoStp_StopControl();
            return(FALSE);
        }

        //-----------------------------------------------------;
        // check if it's finished                              ;
        //-----------------------------------------------------;
        if (seek_out == TRUE)
        {
            if (StepMotorSupport_Location.val >= target_location)
            {
                temp_IE = GET_INT_ENABLE_MASK();
                DISABLE_INTERRUPT();
                WRITE_REG(HAL_SRVREG_MAP->STPDCO, 0x00);        // set back to 4x1378 Hz
                WRITE_REG(HAL_SRVREG_MAP->STPPWR, 20);          // 3.6 ms (given step motor support clk being 4*1378 Hz )
                SET_INT_ENABLE_MASK(temp_IE);

                svoStp_StopControl();
                return(TRUE);
            }
        }
        else
        {
            if (StepMotorSupport_Location.val <= target_location)
            {
                temp_IE = GET_INT_ENABLE_MASK();
                DISABLE_INTERRUPT();
                WRITE_REG(HAL_SRVREG_MAP->STPDCO, 0x00);        // set back to 4x1378 Hz
                WRITE_REG(HAL_SRVREG_MAP->STPPWR, 20);          // 3.6 ms (given step motor support clk being 4*1378 Hz )
                SET_INT_ENABLE_MASK(temp_IE);
                svoStp_StopControl();
                return(TRUE);
            }
        }
    }//for ms_cnt loop

    temp_IE = GET_INT_ENABLE_MASK();
    DISABLE_INTERRUPT();
    WRITE_REG(HAL_SRVREG_MAP->STPDCO, 0x00);                  // set back to 4x1378 Hz
    WRITE_REG(HAL_SRVREG_MAP->STPPWR, 20);                   // 3.6 ms (given step motor support clk being 4*1378 Hz )
    SET_INT_ENABLE_MASK(temp_IE);
    svoStp_StopControl();
    return(FALSE);
}// end of StepMotorSupport_seek
#endif // DO_HW_SUPPORT_STEP == 1

/*-----------------------------------------------------------------------------
 *
 * Module: svoStp_recovery_jump_dist()
 * Description: check current sled position, if it is dangerously close to disc edge,
 *              then return the distance toward safe zone.
 *
 *----------------------------------------------------------------------------*/
SHORT svoStp_recovery_jump_dist(void)
{
    SHORT jump_distance,min_position,max_position;

    min_position = UM_2_SLED_STEP(24000);
#if (DISC_8CM == 1)
    if (svoVar_discDiameter == e8cm)
    {
        max_position = UM_2_SLED_STEP(37000);
    }
    else
#endif /* DISC_8CM */
    {
        max_position = UM_2_SLED_STEP(57000);
    }

#if (DVD_RAM_READ == 1)
    if (DVDRAM)
    {
        if (svoVar_TrackingMethod == TRK_METHOD_DPP)
        {
            min_position = UM_2_SLED_STEP(24500);
        }
        else
        {
            min_position = UM_2_SLED_STEP(23250);
            max_position = UM_2_SLED_STEP(23450);
        }
    }
#endif

    jump_distance = 0;
    if (Iram_stp_CurrentPosition > max_position)
    {
        jump_distance = max_position - Iram_stp_CurrentPosition;
    }
    else if (Iram_stp_CurrentPosition < min_position)
    {
        jump_distance = min_position - Iram_stp_CurrentPosition;
    }
    return jump_distance;
}

#if 0
/*-----------------------------------------------------------------------------
 *
 * Module: svoStp_verify()
 * Description: send random sledge (colli) commands (1~4 commands in a row) to see if the module works fine.
 *
 *----------------------------------------------------------------------------*/
void svoStp_verify()
{
    BYTE cycle;
    eStepMode mode;
    SHORT last_sled = Iram_stp_CurrentPosition;
#if (COLLI_SWITCH == 1)
    SHORT last_colli = Iram_colli_CurrentPosition;
#endif

    cycle = rand() & 0x03;
    for (; cycle < 4; cycle++)
    {
#if (COLLI_SWITCH == 1)
        mode = (rand() % COLLI_MODE_MAX) + 1;
#else
        mode = (rand() % STEP_MODE_MAX) + 1;
#endif
        switch (mode)
        {
            case STEP_STOP_MODE:
                send_msg5S(SHOW_DEBUG_MSG,"SLED stop     ");
                svoStp_StopControl();
                last_sled = 0;
                break;

            case STEP_NEW_HOME_MODE:
                send_msg5S(SHOW_DEBUG_MSG,"SLED home     ");
                svoStp_Homing();
                last_sled = svoStepVar.StepHomePos;
                break;
#if (COLLI_SWITCH == 1)
            case COLLI_NEW_HOME_MODE:
                send_msg5S(SHOW_DEBUG_MSG,"COLLI home    ");
                svoColli_Homing();
                last_colli = 0;
                break;
#endif
            default:
#if (COLLI_SWITCH == 1)
                if (mode > STEP_MODE_MAX)
                {
                    last_colli = ((USHORT)rand() % (COLLI_MAX_POS - COLLI_MIN_POS)) + COLLI_MIN_POS;
    #if (COLLI_ANALOG_OUTPUT == 0)
                    last_colli = (last_colli & 0xFFE0);
    #endif
                    send_msg5SValue(SHOW_DEBUG_MSG,"COLLI jump",3,(mode << 16) | last_colli);
                    svoStp_JumpToPosition(mode,last_colli);
                }
                else
#endif
                {
                    last_sled = ((USHORT)rand() % (UM_2_SLED_STEP(57000) - UM_2_SLED_STEP(24000))) + UM_2_SLED_STEP(24000);
                    send_msg5SValue(SHOW_DEBUG_MSG,"SLED  jump",3,(mode << 16) | last_sled);
                    svoStp_JumpToPosition(mode,last_sled);
                }
                break;
        }
        ExitProcess();
    }
    while (svoStp_busy())
    {
        ExitProcess();
    }

    if (((last_sled == Iram_stp_CurrentPosition) || (last_sled == 0))
#if (COLLI_SWITCH == 1)
        && (last_colli == Iram_colli_CurrentPosition)
#endif
       )
    {
        send_msg5S(SHOW_DEBUG_MSG,"verify ok     ");
    }
    else
    {
        send_msg5S(SHOW_DEBUG_MSG,"verify NOK    ");
        send_msg5SValue(SHOW_DEBUG_MSG,"SLED  pos",4,(last_sled<<16)|Iram_stp_CurrentPosition);
#if (COLLI_SWITCH == 1)
        send_msg5SValue(SHOW_DEBUG_MSG,"COLLI pos",4,(last_colli<<16)|Iram_colli_CurrentPosition);
#endif
    }
}
#endif

#if (TRAVERSE_TEST == 1 )
/*-----------------------------------------------------------------------------
 *
 * Module: state_traverse()
 * Description:
 *
 *----------------------------------------------------------------------------*/
void svoStp_Traverse(BYTE force, BYTE times)
{
#if (EPO_SLOT_IN == 0)
    if ((ipOpenSW == 0) || (ipCloseSW == 1))
    {
        Error_1=1;
        return;
    }
#endif

    if (force > 0)
    {
        force = force / 2 + 1;//convert force to 1~128
    }

    switch (times)
    {
        case 0:
#if(Customer_A1==1)
            svoStp_JumpToPosition(STEP_NORMAL_MODE, (STEP_HOME_POSITION_CD - UM_2_SLED_STEP(3600)));
#else
            svoStp_JumpToPosition(STEP_SAFE_SLOW_MODE, (STEP_HOME_POSITION_CD - UM_2_SLED_STEP(3600)));
#endif
            if (force > 0) svoStp_SetGain(force);
            while(svoStp_busy());
            {
                ExitProcess();
            }
            break;

        case 0xFF:
            SetMotorDriverState(SHORT_BRAKE);
#if(Customer_A1==1)
            svoStp_JumpToPosition(STEP_NORMAL_MODE, UM_2_SLED_STEP(55200));  //not to collide with both ends
#else
            svoStp_JumpToPosition(STEP_SAFE_SLOW_MODE, UM_2_SLED_STEP(55200));  //not to collide with both ends
#endif
			
            if (force > 0) svoStp_SetGain(force);
            while(svoStp_busy());
            {
                ExitProcess();
            }
            break;

        default:
            while (times > 0)
            {
                SetMotorDriverState(SHORT_BRAKE);
#if(Customer_A1==1)
                svoStp_JumpToPosition(STEP_NORMAL_MODE, UM_2_SLED_STEP(55200));  //not to collide with both ends
#else
                svoStp_JumpToPosition(STEP_SAFE_SLOW_MODE, UM_2_SLED_STEP(55200));  //not to collide with both ends
#endif
				
                if (force > 0) svoStp_SetGain(force);
                while(svoStp_busy());
                {
                    ExitProcess();
                }
#if(Customer_A1==1)
                svoStp_JumpToPosition(STEP_NORMAL_MODE, (STEP_HOME_POSITION_CD - UM_2_SLED_STEP(3600)));
#else
                svoStp_JumpToPosition(STEP_SAFE_SLOW_MODE, (STEP_HOME_POSITION_CD - UM_2_SLED_STEP(3600)));
#endif

                if (force > 0) svoStp_SetGain(force);
                while(svoStp_busy());
                {
                    ExitProcess();
                }
                times--;
            }
            break;
    }
    svoStp_StopControl();
#if (ALWAYS_HOMING == 0)
    Iram_stp_Initialized = STEP_UNINITIALIZED;
#endif
}

#if ( EPO_SLOT_IN == 1)
/*-----------------------------------------------------------------------------
 *
 * Module: svoTrayOut3S()
 * Description:
 *
 * ATAPI DF 00 0B 01 level timeout 01b
 *                   max52   (ms)  Full lv
 *----------------------------------------------------------------------------*/
BOOL svoTrayOut3S(void)
{
   ULONG clock1;
   SHORT tray_out_level;
   SHORT tray_out_time;
//   SHORT btemp;

//    SendMsg84(1,0x020241,Iram_Cmd_block.bf[1],Iram_Cmd_block.bf[2],Iram_Cmd_block.bf[3],Iram_Cmd_block.bf[4]);
   tray_out_level = Iram_Cmd_block.bf[4]*4;
   tray_out_time  = Iram_Cmd_block.bf[5];

   if(Iram_Cmd_block.bf[6]==0x01)  //PWN duty 100%	7F 12V
   {
        tray_out_level = 510;
   }
   else
   {
       if(tray_out_level>328)
           tray_out_level = 328;   //ATAPI max 52 7.5V
   	}

   SetMotorDriverState(LODER_MUTE_OFF);
   svoTray_OutputDac(tray_out_level);


   if(tray_out_time == 0)
   {
       DelaymS(250);
   }
   else{
       if(tray_out_time>7)
       {
           tray_out_time = 7;   //max 7 S
       }

       StartTimer(&clock1);
       for(;;)
       {
    			if(ReadTimer(&clock1) >= (ULONG)(tray_out_time*1000))
                    break;
       }
   }
   SetMotorDriverState(ALL_MUTE_ON);
   svoTray_OutputDac(0);
//   SendMsg84(1,0x020241,indet,ipOpenSW,ipCloseSW,0x00);

   if(indet == 1 && ipOpenSW == 0 && ipCloseSW ==1)   //no disc H L H
   {
      return(PASS);
//     for(;;)
//     {
//            HAL_DSP_LED(ON);
//          DelaymS(50);
//          HAL_DSP_LED(OFF);
//          DelaymS(50);
//     }
   }
   else
   {
     //  SendMsg84(1,0x020241,indet,ipOpenSW,ipCloseSW,0xFF);

	   return(FAIL);

//       for(;;)
//       {
//           for(btemp=0 ; btemp<8 ; ++btemp)
//           {
//               HAL_DSP_LED(ON);
//               DelaymS(200);
//               HAL_DSP_LED(OFF);
//               DelaymS(200);
//            }
//           for(btemp=0 ; btemp<4 ; ++btemp)
//           {
//               DelaymS(250);
//           }

//           for(btemp=0 ; btemp<2 ; ++btemp)
//           {
//               HAL_DSP_LED(ON);
//               DelaymS(200);
//               HAL_DSP_LED(OFF);
//              DelaymS(200);
//           }
//           for(btemp=0 ; btemp<12 ; ++btemp)
//           {
//               DelaymS(250);
//           }
//       }
   }
}

#endif
#endif /* TRAVERSE_TEST */
/*------------------------------------------------------------------------*/
/* The end of file                                                        */
/*------------------------------------------------------------------------*/
