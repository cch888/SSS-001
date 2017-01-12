/****************************************************************************
*                (c) Copyright 2001,2002,2003, 2004, 2005 SUNEXT DESIGN, INC.
*                           All Rights Reserved
*
*
* FILENAME: adjust.c
*
* DESCRIPTION
* This file contains the adjust functions implemented in the servo layer
*
* NOTES:
*
*   $Revision: 10 $
*   $Date: 10/12/21 4:20p $
*
**************************** SUNEXT CONFIDENTIAL ****************************/

#include ".\oem\oem_info.h"
#include ".\common\globals.h"
#include ".\common\globtype.h"
#include ".\common\vu_cfg.h"
#include ".\Common\ComMath.h"
#include ".\common\stdlib.h"
#include ".\AL\REG_HAL.h"
#include ".\AL\REG_global.h"
#include ".\servo\svo_com.h"
#include ".\Servo\svo_lib.h"
#include ".\AL\coefs.h"

#include ".\servo\adjust.h"
#include ".\servo\math2d.h"
#include ".\servo\SVO.H" /* Needed for types of parameters */
#include ".\servo\svo_step.h"

#include <stdio.h>

/*****************************************************************************
*                    Macro Definitions
*****************************************************************************/

#define TO_BE_PORTED    0
#define TEMP_NOT_IN_USE 0 //will be in use when TO_BE_PORTED for some parts are solved

/* maximum representable finite number */
#define FLT_MAX                         1.0e28//7.23700514597312e75
#define LONG_MAX                        +2147483647L


#define  STORAGE_MAX 40
#define  FLANK_MAX   2

#define  MAX_LONG    ((LONG) 0x7fffffffUL)
#define  MIN_LONG    ((LONG) 0x80000000UL)

#define  TIMEOUT_WATCHDOG              200   /* for a 100ms-timer */
#define  TIMEOUT_SERVO_RECOVER         100   /* for a 5ms-timer */
#define  TIMEOUT_PLL_RECOVER           10    /* for a 5ms-timer */
#define  TIMEOUT_OVER_PAUSE_RECOVER    20    /* for a 5ms-timer */

#define  FINE_SEEK_NO_OF_RETRIES       3


#define  RESULT_BUFFER_LENGTH           ((USHORT)(sizeof(user_result.item_optimum) + sizeof(user_result.cost_optimum)))
#define  RESULT_BUFFER_LENGTH_2D        ((USHORT)((2* sizeof(user_result.item_optimum)) + sizeof(user_result.cost_optimum)))
#define  ADJUST_SIZE_OF_STORAGE         (sizeof(data.buf))
#define  ADJUST_SIZE_DESTINATION_BUFFER (sizeof(USHORT))
// Enable state transition printf by removing comment for next define.
//#define  STATE_TRANSITION_PRINTF

#define NR_OF_RECOVERIES 1
#define FOCUS_OFFSET_PRESET_NEEDED()    ( ((curr.DiscKind.Bit.DiscStd == eDiscBD)) &&  \
                                          ((curr.config & ADJ_CONFIG_SEEK_MODE_MASK) == ADJ_CONFIG_FINE_RESEEK) && (curr.item[0]==ADJUST_ITEM_FOCUS_OFFSET))
#define VALIDITY_CHECK(i) (!((data.buf[0][i].hf_recoveries > NR_OF_RECOVERIES) \
                              || (data.buf[0][i].servo_recoveries > 0 ) \
                              || (data.buf[0][i].c < coef.clim_val_min) \
                              || (data.buf[0][i].c > coef.clim_val_max)))

//#define ADJUST_2D_FINE_STEP_LIMIT 10 /* when servo limits are aplied, the calculated steps can be too big
//                                        to have enough valid measure points--> 8 points + 2 on boundary
//                                        which are not measured
//                                     */
// WOBBLE AMPLITUDE
#define ADJUST_2D_SERVO_OPTIMUM_LIMIT_FOO_WOBBLE    90
#define ADJUST_2D_SERVO_OPTIMUM_LIMIT_SPA_WOBBLE    90
#define ADJUST_2D_SERVO_MEASURE_LIMIT_FOO_WOBBLE    90 //at the moment, the limits are set the same.
#define ADJUST_2D_SERVO_MEASURE_LIMIT_SPA_WOBBLE    90
// PUSH PULL AMPLITUDE
#define ADJUST_2D_SERVO_OPTIMUM_LIMIT_FOO_PUSH_PULL 90
#define ADJUST_2D_SERVO_OPTIMUM_LIMIT_SPA_PUSH_PULL 90
#define ADJUST_2D_SERVO_MEASURE_LIMIT_FOO_PUSH_PULL 90 //at the moment, the limits are set the same.
#define ADJUST_2D_SERVO_MEASURE_LIMIT_SPA_PUSH_PULL 90
// RADIAL TILT
//#define SERVO_LIMIT_PUSH_PULL                       90
// TO BE ENABLED LATER :
//#define ADJUST_2D_SERVO_LIMIT_FOO_JITTER 110
//#define ADJUST_2D_SERVO_LIMIT_SPA_JITTER 110


typedef enum
{
   SRV_CALC_P_STEP_DELAY_TLT,
   SRV_CALC_P_DELAY_NEXT_SIDE_TLT,
   SRV_CALC_P_STEP_COST_IS_HIGH_TLT_JITTER,
   SRV_CALC_P_STEP_COST_IS_LOW_TLT_JITTER,
   SRV_CALC_P_STEP_COST_IS_HIGH_TLT_PP,
   SRV_CALC_P_STEP_COST_IS_LOW_TLT_PP,
   SRV_CALC_P_MAX_REL_TLT,
   SRV_CALC_P_MAX_TLT,
   SRV_CALC_P_STEP_DELAY_SPA,
   SRV_CALC_P_DELAY_NEXT_SIDE_SPA,
   SRV_CALC_P_STEP_COST_IS_HIGH_SPA_JITTER,
   SRV_CALC_P_STEP_COST_IS_LOW_SPA_JITTER,
   SRV_CALC_P_STEP_COST_IS_HIGH_SPA_PP,
   SRV_CALC_P_STEP_COST_IS_LOW_SPA_PP,
   SRV_CALC_P_MAX_REL_SPA,
   SRV_CALC_P_MAX_SPA,
   SRV_CALC_P_MIN_SPA,
   SRV_CALC_P_OFFSET_SPA,
   SRV_CALC_P_STEP_DELAY_FOO,
   SRV_CALC_P_DELAY_NEXT_SIDE_FOO,
   SRV_CALC_P_STEP_COST_IS_HIGH_FOO_JITTER,
   SRV_CALC_P_STEP_COST_IS_LOW_FOO_JITTER,
   SRV_CALC_P_MAX_REL_FOO,
   SRV_CALC_P_MAX_FOO,
   SRV_CALC_P_STEP_DELAY_ALP,
   SRV_CALC_P_DELAY_NEXT_SIDE_ALP,
   SRV_CALC_P_STEP_COST_IS_HIGH_ALP_JITTER,
   SRV_CALC_P_STEP_COST_IS_LOW_ALP_JITTER,
   SRV_CALC_P_MAX_REL_ALP,
   SRV_CALC_P_MAX_ALP,
   SRV_CALC_P_MIN_ALP,
   SRV_CALC_CLIM_STEP_PP,
   SRV_CALC_CLIM_MEAS_PP,
   SRV_CALC_CLIM_VAL_MAX_PP,
   SRV_CALC_CLIM_VAL_MIN_PP,
   SRV_CALC_CLIM_REL_MIN_PP,
   SRV_CALC_CLIM_STEP_WOBBLE,
   SRV_CALC_CLIM_MEAS_WOBBLE,
   SRV_CALC_CLIM_VAL_MAX_WOBBLE,
   SRV_CALC_CLIM_VAL_MIN_WOBBLE,
   SRV_CALC_CLIM_REL_MIN_WOBBLE,
   SRV_CALC_P_STEP_COST_IS_HIGH_SPA_WOBBLE,
   SRV_CALC_P_STEP_COST_IS_LOW_SPA_WOBBLE,
   SRV_CALC_P_DELAY_OPEN_LOOP_SPA_WOBBLE,
   SRV_CALC_P_RANGE_2D_COST_IS_FOO_WOBBLE,
   SRV_CALC_P_RANGE_2D_COST_IS_SPA_WOBBLE,
   SRV_CALC_P_RANGE_2D_COST_IS_FOO_JITTER,
   SRV_CALC_CLIM_STEP_JITTER,
   SRV_CALC_CLIM_MEAS_JITTER,
   SRV_CALC_CLIM_VAL_MAX_JITTER,
   SRV_CALC_CLIM_VAL_MIN_JITTER,
   SRV_CALC_CLIM_REL_MIN_JITTER,
} ETempCoefs; //Untill coefs are in the coef-table

/*****************************************************************************
*                    Type Definitions
*****************************************************************************/

/*
   The states used in the state machine of this module
*/
typedef enum
{
   ADJUST_S_IDLE,
   ADJUST_S_INITIAL_SEEK,
   ADJUST_S_FRONT_BACK_INIT,
   ADJUST_S_P_INIT,
   ADJUST_S_P_NEXT,
   ADJUST_S_P_SET,
   ADJUST_S_P_INIT_RESTORE,
   ADJUST_S_INTERM_SEEK,
   ADJUST_S_COST_INIT,
   ADJUST_S_COST_MEASURE,
   ADJUST_S_RECOVER,
   ADJUST_S_PROCESS_RESULTS,
   ADJUST_S_FRONT_BACK_RESTORE,
   ADJUST_S_FINAL,
   ADJUST_S_LAST
} ADJUST_S_t;

/*
   The sub states used in the state machine of this module
*/
typedef enum
{
   ADJUST_S_P_SET_SUBST_INIT,
   ADJUST_S_P_SET_SUBST_SET,
   ADJUST_S_P_SET_SUBST_WAIT_INIT,
   ADJUST_S_P_SET_SUBST_WAIT_FINISH,
   ADJUST_S_P_SET_SUBST_READY
}ADJUST_S_P_SET_SUBST_t;

typedef enum
{
   ADJUST_S_INITIAL_SEEK_SUBST_START = ADJUST_S_P_SET_SUBST_INIT,// Initiates the seek.
   ADJUST_S_INITIAL_SEEK_SUBST_WAIT,                             // Waits until the seek is ready.
   ADJUST_S_INITIAL_SEEK_SUBST_FINAL                             // Determines in which state to proceed.
}ADJUST_S_INITIAL_SEEK_SUBST_t;

typedef enum
{
   ADJUST_S_INTERM_SEEK_SUBST_START = ADJUST_S_P_SET_SUBST_INIT,// Initiates the seek.
   ADJUST_S_INTERM_SEEK_SUBST_CLOSE_LOOP,                       // close radial loop, if necessary
   ADJUST_S_INTERM_SEEK_SUBST_WAIT,                             // Waits until the seek is ready.
   ADJUST_S_INTERM_SEEK_FINE_SEEK,                              // Waits until the exact address
   ADJUST_S_INTERM_FINE_SEEK_RETRY,                             // Initiate retry
   ADJUST_S_INTERM_SEEK_SUBST_OPEN_LOOP,                        // open radial loop, if necessary
   ADJUST_S_INTERM_SEEK_SUBST_FINAL                             // Determines in which state to proceed.
}ADJUST_S_INTERM_SEEK_SUBST_t;

typedef enum
{
   ADJUST_S_FRONT_BACK_INIT_SUBST_START = ADJUST_S_P_SET_SUBST_INIT,
   ADJUST_S_FRONT_BACK_INIT_SUBST_COND_OPEN_LOOP,
   ADJUST_S_FRONT_BACK_INIT_SUBST_FINAL
}ADJUST_S_FRONT_BACK_INIT_SUBST_t;

typedef enum
{
   ADJUST_S_FRONT_BACK_RESTORE_SUBST_START = ADJUST_S_P_SET_SUBST_INIT,
   ADJUST_S_FRONT_BACK_RESTORE_SUBST_WAIT,
   ADJUST_S_FRONT_BACK_RESTORE_SUBST_FINAL
}ADJUST_S_FRONT_BACK_RESTORE_SUBST_t;

typedef union
{
   ADJUST_S_P_SET_SUBST_t              s_p_set;
   ADJUST_S_INITIAL_SEEK_SUBST_t       s_initial_seek;
   ADJUST_S_INTERM_SEEK_SUBST_t        s_interm_seek;
   ADJUST_S_FRONT_BACK_INIT_SUBST_t    s_front_back_init;
   ADJUST_S_FRONT_BACK_RESTORE_SUBST_t s_front_back_restore;
}ADJUST_SUB_S_t;

typedef enum
{
   ADJUST_SS_START,
   ADJUST_SS_WAIT_SERVO,
   ADJUST_SS_WAIT_DELAY
} ADJUST_SUBSUB_S_t;


typedef struct
{
   BYTE          delay_small;       //Settle time before cost is measured.
   BYTE          delay_big;         //Settle time before cost is measured.
   LONG          step_hi;
   LONG          step_lo;
   LONG          max_rel;
   LONG          max;
   LONG          min;
   LONG          result_min;
   LONG          result_max;
}ADJUST_SESSION_ITEM_COEF_t;

typedef struct
{
   ADJUST_SESSION_ITEM_COEF_t    p[MAX_NO_ITEMS];
   LONG                          clim_step;     //cost limit to decide on increase or decrease next step
   LONG                          clim_meas;     //cost limit
   LONG                          clim_val_min;  //cost limit
   LONG                          clim_val_max;  //cost limit
   LONG                          clim_rel_min;
   LONG                          retry_max;
   float                         a1_min;
   float                         a1_max;
   float                         a2_min;
   float                         a2_max;
   LONG                          i_min;
   BOOL                          delta_relative;
   BOOL                          curve_up;
} ADJUST_SESSION_COEF_t;

typedef enum
{
   ADJUST_MODE_IDLE,
   ADJUST_MODE_1D,
   ADJUST_MODE_2D,
   ADJUST_MODE_ITEMSCAN,
   ADJUST_MODE_LAST
}ADJUST_MODE_t;

typedef struct
{
   FPTR              client_callback_func;
   BYTE *            buffer;
   USHORT            buffer_size;
   USHORT *          nrof_bytes_copied;
   ULONG             start;
   BYTE              config;
   LONG              no_of_items;
   ADJUST_ITEM_t     item[MAX_NO_ITEMS];
   ADJUST_COST_t     cost;
   discKindT         DiscKind;
   ESpindleSpeed     speed;
   ADJUST_MODE_t     mode;
} ADJUST_SESSION_INFO_t;

typedef struct
{
   LONG           p[MAX_NO_ITEMS];
   LONG           c;
   BYTE          hf_recoveries;
   BYTE          servo_recoveries;
   BYTE          seek_recoveries;
} ADJUST_SESSION_BUF_DATA_t;


typedef struct
{
   LONG                       flank;            // flank counter
   LONG                       i;                // p-counter inside current flank
   LONG                       i_sub;            // sub-measurements inside current p of current flank
   LONG                       retry;            // retry-counter
   LONG                       p[MAX_NO_ITEMS];  // value of p
   BOOL                       p_start_valid;
   LONG                       p_start_first[MAX_NO_ITEMS]; // very first start value of p
   LONG                       p_start_actual[MAX_NO_ITEMS];// start value of actual bathtub
   ADJUST_SESSION_BUF_DATA_t  buf[FLANK_MAX][STORAGE_MAX];
   LONG                       c_min;
   LONG                       c_max;
   RAMP_MODE_t                ramp_mode;
} ADJUST_SESSION_DATA_t;

typedef enum
{
   WOB_PP_PREPARE,
   WOB_PP_RESTORE
}ADJUST_WOB_PP_SETTING_t;

typedef enum
{
   SERVO_LIMIT_MEASURE,
   SERVO_LIMIT_OPTIMUM
}ADJUST_SERVO_LIMIT_MODE_t;
/*****************************************************************************
*                    Public Data Definitions
*****************************************************************************/
static CONST ADJUST_QUAL_t cost2qual[ADJUST_COST_LAST] =
{
   ADJUST_QUAL_HIGH,  //ADJUST_COST_STREAM_SAMPLED_JITTER,
   ADJUST_QUAL_HIGH,  //ADJUST_COST_ARM_SAMPLED_JITTER,
   ADJUST_QUAL_LOW,   //ADJUST_COST_PUSHPULL_AMPL,
   ADJUST_QUAL_LOW,   //ADJUST_COST_WOBBLE_AMPL,
   ADJUST_QUAL_HIGH,  //ADJUST_COST_STREAM_SAMPLED_JITTER_ITEMSCAN,
   ADJUST_QUAL_HIGH,  //ADJUST_COST_ARM_SAMPLED_JITTER_ITEMSCAN,
   ADJUST_QUAL_LOW,   //ADJUST_COST_PUSHPULL_AMPL_ITEMSCAN,
   ADJUST_QUAL_LOW,   //ADJUST_COST_WOBBLE_AMPL_ITEMSCAN,
   ADJUST_QUAL_LOW,   //ADJUST_COST_ABORT,
   ADJUST_QUAL_LOW,   //ADJUST_COST_NONE
};

static CONST ADJUST_MODE_t cost2mode[ADJUST_COST_LAST] =
{
   ADJUST_MODE_1D,      //ADJUST_COST_STREAM_SAMPLED_JITTER,
   ADJUST_MODE_1D,      //ADJUST_COST_ARM_SAMPLED_JITTER,
   ADJUST_MODE_1D,      //ADJUST_COST_PUSHPULL_AMPL,
   ADJUST_MODE_1D,      //ADJUST_COST_WOBBLE_AMPL,
   ADJUST_MODE_ITEMSCAN,//ADJUST_COST_STREAM_SAMPLED_JITTER_ITEMSCAN,
   ADJUST_MODE_ITEMSCAN,//ADJUST_COST_ARM_SAMPLED_JITTER_ITEMSCAN,
   ADJUST_MODE_ITEMSCAN,//ADJUST_COST_PUSHPULL_AMPL_ITEMSCAN,
   ADJUST_MODE_ITEMSCAN,//ADJUST_COST_WOBBLE_AMPL_ITEMSCAN,
   ADJUST_MODE_1D,      //ADJUST_COST_ABORT,
   ADJUST_MODE_1D,      //ADJUST_COST_NONE
};
/*****************************************************************************
*                    Static Data Definitions
*****************************************************************************/

static   BOOL                    active;
static   EStatus                 status;
static   ADJUST_S_t              state;
static   ADJUST_SUB_S_t          sub_state;
static   ADJUST_SUBSUB_S_t       ss_state;
static   ADJUST_S_t              previous_state;
static   BOOL                    abort_initiated;

static   ADJ_RESULT_t            user_result;

static   ADJUST_SESSION_INFO_t   curr;
static   ADJUST_SESSION_INFO_t   new;

static   ADJUST_SESSION_DATA_t   data;
static   ADJUST_SESSION_COEF_t   coef;
#if TEMP_NOT_IN_USE
static   SHORT                   init_rad_offset_perc;
static   LOCKTYPE_t              orig_lock_mode;
static   BOOL                    orig_pll_kick;
#endif
static   BOOL                    initiate_reseek;
#if TEMP_NOT_IN_USE
static   LONG                    fine_seek_border[/*HAL_MEDIUM_LAST+1*/] =  //To Do Define Length and mapping Sunext
{
   6,     //HAL_MEDIUM_CD_RO     ,DISC_KIND_CD, per 3 addresses
   6,     //HAL_MEDIUM_CD_R
   6,     //HAL_MEDIUM_CD_RW
   8,     //HAL_MEDIUM_DVD_SL    ,DISC_KIND_DVD
   8,     //HAL_MEDIUM_DVD_DL
   8,     //HAL_MEDIUM_DVDpR_SL  ,DISC_KIND_DVDp, per 4 adresses
   8,     //HAL_MEDIUM_DVDpRW
   8,     //HAL_MEDIUM_DVDpR_DL
   32,    //HAL_MEDIUM_DVDmR     ,DISC_KIND_DVDm, per 16 adresses
   32,    //HAL_MEDIUM_DVDmRW
   32,    //HAL_MEDIUM_DVDmR_DL
   32,    //HAL_MEDIUM_BD_RO_SL  ,DISC_KIND_BD, 3 per ecc block, every 4th address missing
   32,    //HAL_MEDIUM_BD_RO_DL
   32,    //HAL_MEDIUM_BD_R_SL
   32,    //HAL_MEDIUM_BD_R_DL
   32,    //HAL_MEDIUM_BD_RE_SL
   32,    //HAL_MEDIUM_BD_RE_DL
   2,     //HAL_MEDIUM_LAST      ,DISC_KIND_UNKNOWN
};
#endif

static  ADJUST_LOGGING_DATA_t    adjust_logging_data[ADJUST_ITEM_LAST_ACTIVE+1];
static  LONG                     p_width;

static Math2d_ExperimentT        experiment_plan_2d[NR_OF_2D_EXPERIMENTS];
static ADJUST_SERVO_LIMITS_t     adjust_servo_limits[ADJUST_ITEM_LAST][eLayersLast+1];
static eLayers                   servo_limit_layer;
static BOOL                      optimal_algorithm;

static CONST LONG nrofflanks[ADJUST_MODE_LAST] =
{
   0,          //ADJUST_MODE_IDLE,
   FLANK_MAX,  //ADJUST_MODE_1D,
   1,          //ADJUST_MODE_2D,
   1,          //ADJUST_MODE_ITEMSCAN,
};


static CONST char * adjust_cost_to_string[ADJUST_COST_LAST +1]=
{
   "stream_sampled_jitter",
   "arm_sampled_jitter",
   "pushpull_ampl",
   "cost_wobble_ampl",
   "stream_sampled_jitter_itemscan",
   "arm_sampled_jitter_itemscan",
   "pushpull_ampl_itemscan",
   "cost_wobble_ampl_itemscan",
   "abort",
   "none",
   "last"
};

CONST char * adjust_item_to_string[(ULONG)ADJUST_ITEM_LAST+1UL] =
{
   "spher_aber",
   "tang_tilt",
   "rad_tilt",
   "focus_offset",
   "re_gain_offset",
   "sspp_gain",
   "rad_offset",
   "wopc",
   "foc_int",
   "re",
   "reflection",
   "frd_ref",
   "erase_reflection",
   "dutycycle_5_12v",
   "laser_temp_act_disp",
   "laser_treshold_current",
   "abort",
   "none",
   "last"
};


static CONST char * adjust_state_2_str[ADJUST_S_LAST]=
{
   "s_idle",
   "s_initial_seek",
   "s_front_back_init",
   "s_p_init",
   "s_p_next",
   "s_p_set",
   "s_p_init_restore",
   "s_interm_seek",
   "s_cost_init",
   "s_cost_measure",
   "s_recover",
   "s_process_results",
   "s_front_back_restore",
   "s_final"
};

//WRAPPERS ==> later create HAL Functions ???
void FOCUS_SetOffset( BYTE offset);
BYTE FOCUS_GetOffset();

void FOCUS_SetOffset ( BYTE offset)
{
   WRITE_FIELD(HAL_AFE_FCSO,offset);
}

BYTE FOCUS_GetOffset()
{
   return READ_FIELD(HAL_AFE_FCSO);
}

//END OF WRAPPERS

/*****************************************************************************
*                    Static Function Prototypes
*****************************************************************************/
static void adjust_calculate_servo_limits(ADJUST_SERVO_LIMIT_MODE_t mode,Math2d_ResultT *result_2d_calc,Math2d_ExperimentT *experiment_plan_2d );

/*****************************************************************************
*                    Static Function Definitions
*****************************************************************************/

static void adjust_state_change(ADJUST_S_t new_state)
{
   previous_state = state;
   state = new_state;
   // Init sub_state for next state.
   sub_state.s_p_set = ADJUST_S_P_SET_SUBST_INIT;
   ss_state = ADJUST_SS_START;
   send_msg5SValue(1,"previous_state",1,previous_state);
   if ( previous_state < ADJUST_S_LAST )
   {
      send_msg5S(1,adjust_state_2_str[previous_state]);
   }
   else
   {
      send_msg5SValue(1,"Err previous_state",1,previous_state);
   }
   send_msg5SValue(1,"new_state",1,state);
   if ( state < ADJUST_S_LAST )
   {
      send_msg5S(1,adjust_state_2_str[state]);
   }
   else
   {
      send_msg5SValue(1,"Err new_state",1,state);
   }
   //DEBUG_printf(("ADJUST:state %s->%s",adjust_state_2_str[previous_state], adjust_state_2_str[state]));
   // Attach to perform state change.
   //KRNL_attach (TIMEBASE_0_MS, ADJUST_TASKID);
}

/*--------------------------------------------------------------------------*/

#if TEMP_NOT_IN_USE
static void adjust_callback_func (void)
{
   //KRNL_attach (TIMEBASE_0_MS, ADJUST_TASKID);
}
#endif

/*--------------------------------------------------------------------------*/

static void adjust_clear_session_info (ADJUST_SESSION_INFO_t * i)
{
   LONG item_number;

   i->config               = 0;
   i->start                = 0;
   i->cost                 = ADJUST_COST_NONE;
   for(item_number=0; item_number < MAX_NO_ITEMS; item_number++)
   {
      i->item[item_number]     = ADJUST_ITEM_NONE;
   }
   i->client_callback_func = NULL;
   i->mode                 = ADJUST_MODE_IDLE;
}

/*--------------------------------------------------------------------------*/

static void adjust_set_item_cost_coefs(void)
{
   LONG item_number;

   for(item_number=0; item_number < curr.no_of_items; item_number++)
   {
      // item-specific coefs
      switch (curr.item[item_number])
      {
         case ADJUST_ITEM_RAD_TILT:
            coef.p[item_number].delay_small   = (BYTE)COEF_GetValueParams(curr.DiscKind,curr.speed, SRV_CALC_P_STEP_DELAY_TLT);
            coef.p[item_number].delay_big     = (BYTE)COEF_GetValueParams(curr.DiscKind,curr.speed, SRV_CALC_P_DELAY_NEXT_SIDE_TLT);
            if ((curr.cost == ADJUST_COST_ARM_SAMPLED_JITTER) ||
                (curr.cost == ADJUST_COST_STREAM_SAMPLED_JITTER) ||
                (curr.cost == ADJUST_COST_ARM_SAMPLED_JITTER_ITEMSCAN) ||
                (curr.cost == ADJUST_COST_STREAM_SAMPLED_JITTER_ITEMSCAN)   )
            {

               coef.p[item_number].step_hi       = (LONG)((SHORT)COEF_GetValueParams(curr.DiscKind,curr.speed, SRV_CALC_P_STEP_COST_IS_HIGH_TLT_JITTER));
               coef.p[item_number].step_lo       = (LONG)((SHORT)COEF_GetValueParams(curr.DiscKind,curr.speed, SRV_CALC_P_STEP_COST_IS_LOW_TLT_JITTER));

            }
            else
            {
               coef.p[item_number].step_hi       = (LONG)((SHORT)COEF_GetValueParams(curr.DiscKind,curr.speed, SRV_CALC_P_STEP_COST_IS_HIGH_TLT_PP));
               coef.p[item_number].step_lo       = (LONG)((SHORT)COEF_GetValueParams(curr.DiscKind,curr.speed, SRV_CALC_P_STEP_COST_IS_LOW_TLT_PP));
            }

            coef.p[item_number].max_rel       = (LONG)(COEF_GetValueParams(curr.DiscKind,curr.speed, SRV_CALC_P_MAX_REL_TLT));
            coef.p[item_number].max           = (LONG)((SHORT)COEF_GetValueParams(curr.DiscKind,curr.speed, SRV_CALC_P_MAX_TLT));
            coef.p[item_number].min           = -coef.p[item_number].max;
            break;

         case ADJUST_ITEM_SPHER_ABER:
            coef.p[item_number].delay_small   = (BYTE)COEF_GetValueParams(curr.DiscKind,curr.speed, SRV_CALC_P_STEP_DELAY_SPA);
            coef.p[item_number].delay_big     = (BYTE)COEF_GetValueParams(curr.DiscKind,curr.speed, SRV_CALC_P_DELAY_NEXT_SIDE_SPA);
            if ((curr.cost == ADJUST_COST_ARM_SAMPLED_JITTER) ||
                (curr.cost == ADJUST_COST_STREAM_SAMPLED_JITTER) ||
                (curr.cost == ADJUST_COST_ARM_SAMPLED_JITTER_ITEMSCAN) ||
                (curr.cost == ADJUST_COST_STREAM_SAMPLED_JITTER_ITEMSCAN)   )
            {
               coef.p[item_number].step_hi       = (LONG)((SHORT)COEF_GetValueParams(curr.DiscKind,curr.speed, SRV_CALC_P_STEP_COST_IS_HIGH_SPA_JITTER));
               coef.p[item_number].step_lo       = (LONG)((SHORT)COEF_GetValueParams(curr.DiscKind,curr.speed, SRV_CALC_P_STEP_COST_IS_LOW_SPA_JITTER));
            }
            else
            {
               if ((curr.cost == ADJUST_COST_PUSHPULL_AMPL)         ||
                   (curr.cost == ADJUST_COST_PUSHPULL_AMPL_ITEMSCAN)  )
               {
                  coef.p[item_number].step_hi       = (LONG)((SHORT)COEF_GetValueParams(curr.DiscKind,curr.speed, SRV_CALC_P_STEP_COST_IS_HIGH_SPA_PP));
                  coef.p[item_number].step_lo       = (LONG)((SHORT)COEF_GetValueParams(curr.DiscKind,curr.speed, SRV_CALC_P_STEP_COST_IS_LOW_SPA_PP));
               }
               else
               {
                  coef.p[item_number].step_hi       = (LONG)((SHORT)COEF_GetValueParams(curr.DiscKind,curr.speed, SRV_CALC_P_STEP_COST_IS_HIGH_SPA_WOBBLE));
                  coef.p[item_number].step_lo       = (LONG)((SHORT)COEF_GetValueParams(curr.DiscKind,curr.speed, SRV_CALC_P_STEP_COST_IS_LOW_SPA_WOBBLE));
               }
            }
            coef.p[item_number].max_rel       = (LONG)(COEF_GetValueParams(curr.DiscKind,curr.speed, SRV_CALC_P_MAX_REL_SPA));
            if (adjust_servo_limits[ADJUST_ITEM_SPHER_ABER][servo_limit_layer].determined)
            {
               coef.p[item_number].max           = (LONG)adjust_servo_limits[ADJUST_ITEM_SPHER_ABER][servo_limit_layer].max_measure;
               coef.p[item_number].min           = (LONG)adjust_servo_limits[ADJUST_ITEM_SPHER_ABER][servo_limit_layer].min_measure;
            }
            else
            {
               coef.p[item_number].max           = (LONG)((SHORT)COEF_GetValueParams(curr.DiscKind,curr.speed, SRV_CALC_P_MAX_SPA));
               coef.p[item_number].min           = (LONG)((SHORT)COEF_GetValueParams(curr.DiscKind,curr.speed, SRV_CALC_P_MIN_SPA));
            }
            break;
         case ADJUST_ITEM_TANG_TILT:
            coef.p[item_number].delay_small   = (BYTE)COEF_GetValueParams(curr.DiscKind,curr.speed, SRV_CALC_P_STEP_DELAY_ALP);
            coef.p[item_number].delay_big     = (BYTE)COEF_GetValueParams(curr.DiscKind,curr.speed, SRV_CALC_P_DELAY_NEXT_SIDE_ALP);
            coef.p[item_number].step_hi       = (LONG)((SHORT)COEF_GetValueParams(curr.DiscKind,curr.speed, SRV_CALC_P_STEP_COST_IS_HIGH_ALP_JITTER));
            coef.p[item_number].step_lo       = (LONG)((SHORT)COEF_GetValueParams(curr.DiscKind,curr.speed, SRV_CALC_P_STEP_COST_IS_LOW_ALP_JITTER));
            coef.p[item_number].max_rel       = (LONG)(COEF_GetValueParams(curr.DiscKind,curr.speed, SRV_CALC_P_MAX_REL_ALP));
            coef.p[item_number].max           = (LONG)((SHORT)COEF_GetValueParams(curr.DiscKind,curr.speed, SRV_CALC_P_MAX_ALP));
            coef.p[item_number].min           = (LONG)((SHORT)COEF_GetValueParams(curr.DiscKind,curr.speed, SRV_CALC_P_MIN_ALP));
            break;
         case ADJUST_ITEM_FOCUS_OFFSET:
            coef.p[item_number].delay_small   = (BYTE)COEF_GetValueParams(curr.DiscKind,curr.speed, SRV_CALC_P_STEP_DELAY_FOO);
            coef.p[item_number].delay_big     = (BYTE)COEF_GetValueParams(curr.DiscKind,curr.speed, SRV_CALC_P_DELAY_NEXT_SIDE_FOO);
            coef.p[item_number].max_rel       = (LONG)(COEF_GetValueParams(curr.DiscKind,curr.speed, SRV_CALC_P_MAX_REL_FOO));
            coef.p[item_number].step_hi       = (LONG)((SHORT)COEF_GetValueParams(curr.DiscKind,curr.speed, SRV_CALC_P_STEP_COST_IS_HIGH_FOO_JITTER));
            coef.p[item_number].step_lo       = (LONG)((SHORT)COEF_GetValueParams(curr.DiscKind,curr.speed, SRV_CALC_P_STEP_COST_IS_LOW_FOO_JITTER));

            coef.p[item_number].max           = (LONG)((SHORT)COEF_GetValueParams(curr.DiscKind,curr.speed, SRV_CALC_P_MAX_FOO));
            coef.p[item_number].min           = -coef.p[item_number].max;
            coef.p[item_number].step_hi       = (LONG)((SHORT)COEF_GetValueParams(curr.DiscKind,curr.speed, SRV_CALC_P_STEP_COST_IS_HIGH_FOO_JITTER));
            coef.p[item_number].step_lo       = (LONG)((SHORT)COEF_GetValueParams(curr.DiscKind,curr.speed, SRV_CALC_P_STEP_COST_IS_LOW_FOO_JITTER));

            if (optimal_algorithm)
            {
               coef.p[item_number].max           =0x2000;
               coef.p[item_number].step_lo       =0x3E8;
            }
            break;
         case ADJUST_ITEM_RAD_OFFSET:
            coef.p[item_number].delay_small   = (BYTE)COEF_GetValueParams(curr.DiscKind,curr.speed, SRV_CALC_P_STEP_DELAY_FOO);
            coef.p[item_number].delay_big     = (BYTE)COEF_GetValueParams(curr.DiscKind,curr.speed, SRV_CALC_P_DELAY_NEXT_SIDE_FOO);
            coef.p[item_number].step_hi       = 25;
            coef.p[item_number].step_lo       = 25;
            coef.p[item_number].max_rel       = 70;
            coef.p[item_number].max           = 70;
            coef.p[item_number].min           = -70;
            break;
         case ADJUST_ITEM_RE_GAIN_OFFSET:
         case ADJUST_ITEM_WOPC:
         case ADJUST_ITEM_SSPP_GAIN:
         case ADJUST_ITEM_FOC_INT:
         case ADJUST_ITEM_RE:
         case ADJUST_ITEM_REFLECTION:
         case ADJUST_ITEM_FRD_REF:
         case ADJUST_ITEM_ERASE_REFLECTION:
         case ADJUST_ITEM_DUTYCYCLE_5_12V:
         case ADJUST_ITEM_LASER_TEMP_ACT_DISP:
         case ADJUST_ITEM_LASER_TRESHOLD_CURRENT:
         case ADJUST_ITEM_ABORT:
         case ADJUST_ITEM_NONE:
         case ADJUST_ITEM_LAST:
         default:
            break;
      }
   }//end for loop
   // item-specific coefs
   switch (curr.cost)
   {
      case ADJUST_COST_STREAM_SAMPLED_JITTER:
      case ADJUST_COST_ARM_SAMPLED_JITTER:
      case ADJUST_COST_STREAM_SAMPLED_JITTER_ITEMSCAN:
      case ADJUST_COST_ARM_SAMPLED_JITTER_ITEMSCAN:
         if (optimal_algorithm)
         {
            coef.clim_step       = (LONG)((SHORT)COEF_GetValueParams(curr.DiscKind,curr.speed, SRV_CALC_CLIM_STEP_JITTER));
            coef.clim_meas       = (LONG)((SHORT)COEF_GetValueParams(curr.DiscKind,curr.speed, SRV_CALC_CLIM_MEAS_JITTER));
            coef.clim_val_max    = (LONG)((SHORT)COEF_GetValueParams(curr.DiscKind,curr.speed, SRV_CALC_CLIM_VAL_MAX_JITTER));
            coef.clim_val_min    = (LONG)((SHORT)COEF_GetValueParams(curr.DiscKind,curr.speed, SRV_CALC_CLIM_VAL_MIN_JITTER));
            coef.clim_rel_min    = (LONG)0x26;
            coef.delta_relative  = FALSE;
         }
         else
         {
            coef.clim_step       = (LONG)((SHORT)COEF_GetValueParams(curr.DiscKind,curr.speed, SRV_CALC_CLIM_STEP_JITTER));
            coef.clim_meas       = (LONG)((SHORT)COEF_GetValueParams(curr.DiscKind,curr.speed, SRV_CALC_CLIM_MEAS_JITTER));
            coef.clim_val_max    = (LONG)((SHORT)COEF_GetValueParams(curr.DiscKind,curr.speed, SRV_CALC_CLIM_VAL_MAX_JITTER));
            coef.clim_val_min    = (LONG)((SHORT)COEF_GetValueParams(curr.DiscKind,curr.speed, SRV_CALC_CLIM_VAL_MIN_JITTER));
            coef.clim_rel_min    = (LONG)((SHORT)COEF_GetValueParams(curr.DiscKind,curr.speed, SRV_CALC_CLIM_REL_MIN_JITTER));
         }
         break;
      case ADJUST_COST_PUSHPULL_AMPL:
      case ADJUST_COST_PUSHPULL_AMPL_ITEMSCAN:
         if (optimal_algorithm)
         {
            coef.clim_step       = (LONG)((SHORT)COEF_GetValueParams(curr.DiscKind,curr.speed, SRV_CALC_CLIM_STEP_PP));
            coef.clim_meas       = (LONG)0x0;
            coef.clim_val_max    = (LONG)0x2710;
            coef.clim_val_min    = (LONG)((SHORT)COEF_GetValueParams(curr.DiscKind,curr.speed, SRV_CALC_CLIM_VAL_MIN_PP));
            coef.clim_rel_min    = (LONG)0x1E;
            coef.delta_relative  = TRUE;
         }
         else
         {
            coef.clim_step       = (LONG)((SHORT)COEF_GetValueParams(curr.DiscKind,curr.speed, SRV_CALC_CLIM_STEP_PP));
            coef.clim_meas       = (LONG)((SHORT)COEF_GetValueParams(curr.DiscKind,curr.speed, SRV_CALC_CLIM_MEAS_PP));
            coef.clim_val_max    = (LONG)((SHORT)COEF_GetValueParams(curr.DiscKind,curr.speed, SRV_CALC_CLIM_VAL_MAX_PP));
            coef.clim_val_min    = (LONG)((SHORT)COEF_GetValueParams(curr.DiscKind,curr.speed, SRV_CALC_CLIM_VAL_MIN_PP));
            coef.clim_rel_min    = (LONG)((SHORT)COEF_GetValueParams(curr.DiscKind,curr.speed, SRV_CALC_CLIM_REL_MIN_PP));
         }
         break;
      case ADJUST_COST_WOBBLE_AMPL:
      case ADJUST_COST_WOBBLE_AMPL_ITEMSCAN:
         if (optimal_algorithm)
         {
            coef.clim_step       = (LONG)((SHORT)COEF_GetValueParams(curr.DiscKind,curr.speed, SRV_CALC_CLIM_STEP_WOBBLE));
            coef.clim_meas       = (LONG)((SHORT)COEF_GetValueParams(curr.DiscKind,curr.speed, SRV_CALC_CLIM_MEAS_WOBBLE));
            coef.clim_val_max    = (LONG)0xC8;
            coef.clim_val_min    = (LONG)0x32;
            coef.clim_rel_min    = (LONG)((SHORT)COEF_GetValueParams(curr.DiscKind,curr.speed, SRV_CALC_CLIM_REL_MIN_WOBBLE));
            coef.delta_relative  = TRUE;
         }
         else
         {
            coef.clim_step       = (LONG)((SHORT)COEF_GetValueParams(curr.DiscKind,curr.speed, SRV_CALC_CLIM_STEP_WOBBLE));
            coef.clim_meas       = (LONG)((SHORT)COEF_GetValueParams(curr.DiscKind,curr.speed, SRV_CALC_CLIM_MEAS_WOBBLE));
            coef.clim_val_max    = (LONG)((SHORT)COEF_GetValueParams(curr.DiscKind,curr.speed, SRV_CALC_CLIM_VAL_MAX_WOBBLE));
            coef.clim_val_min    = (LONG)((SHORT)COEF_GetValueParams(curr.DiscKind,curr.speed, SRV_CALC_CLIM_VAL_MIN_WOBBLE));
            coef.clim_rel_min    = (LONG)((SHORT)COEF_GetValueParams(curr.DiscKind,curr.speed, SRV_CALC_CLIM_REL_MIN_WOBBLE));
         }
         break;
      case ADJUST_COST_ABORT:
      case ADJUST_COST_NONE:
      case ADJUST_COST_LAST:
      default:
         break;
   }
   send_msg5S(1,"ADJUST start");
   send_msg5S(1,adjust_item_to_string[curr.item[0]]);
   send_msg5S(1,adjust_item_to_string[curr.item[1]]);
   send_msg5S(1,adjust_cost_to_string[curr.cost]);
   //DEBUG_system_dma_printf((DBG_ADJUST"ADJUST start[%s,%s/%s] on 0x%x",
   //                                       ADJUST_ITEM_to_string[curr.item[0]],
   //                                       ADJUST_ITEM_to_string[curr.item[1]],
   //                                       ADJUST_ITEM_to_string[curr.item[0]],
   //                                       curr.start));
}

/*--------------------------------------------------------------------------*/

static BOOL adjust_idle_parse_parameters (void)
{
   BOOL     rtnval   = TRUE;  // until proven otherwise...
   LONG     item_number;

   // generic (?)
   coef.i_min     = 5;
   coef.retry_max = 3;

   curr.no_of_items = MAX_NO_ITEMS;
   if(curr.item[1] == ADJUST_ITEM_NONE)
   {
      curr.no_of_items = 1;
   }
   // TODO: add all valid combinations, filling 'coef' from coef-file
   if((curr.config & ADJ_CONFIG_COEF_USAGE_FROM_COMMAND_MASK) != ADJ_CONFIG_COEF_USAGE_FROM_COMMAND)
   {
      adjust_set_item_cost_coefs();
   }

   if ((curr.cost == ADJUST_COST_PUSHPULL_AMPL) ||
       (curr.cost == ADJUST_COST_WOBBLE_AMPL)   ||
       (curr.cost == ADJUST_COST_PUSHPULL_AMPL_ITEMSCAN) ||
       (curr.cost == ADJUST_COST_WOBBLE_AMPL_ITEMSCAN)     )
   {
      if((curr.config & ADJ_CONFIG_SEEK_MODE_MASK) == ADJ_CONFIG_RESEEK)
      {
         // reseeks not allowed push pull is measured in radial open loop.
         // Reseek also not needed because we keep on the same disc location.
         rtnval = FALSE;
      }
   }

   if ( (cost2mode[curr.cost] == ADJUST_MODE_ITEMSCAN) && (curr.item[1] != ADJUST_ITEM_NONE))
   {
      //itemscan not allowed when doing 2d
      rtnval = FALSE;
   }

   // item&cost-specific coefs (check that the combination is valid)
   for(item_number=0; item_number < curr.no_of_items; item_number++)
   {
      // item-specific coefs
      switch (curr.item[item_number])
      {
         case ADJUST_ITEM_RAD_TILT:
         case ADJUST_ITEM_FOCUS_OFFSET:
         case ADJUST_ITEM_SPHER_ABER:
         case ADJUST_ITEM_TANG_TILT:
         case ADJUST_ITEM_RAD_OFFSET:
            coef.p[item_number].result_min      = MIN_LONG;
            coef.p[item_number].result_max      = MAX_LONG;
            break;
         case ADJUST_ITEM_NONE:
            if(item_number == 0)
            {
               send_msg80(1,0x710073);//DEBUG_system_dma_printf((DBG_ADJUST"WARNING invalid 1st item= NONE"));
               rtnval = FALSE;
            }
            break;
         case ADJUST_ITEM_ABORT:
            break;
         case ADJUST_ITEM_RE_GAIN_OFFSET:
         case ADJUST_ITEM_SSPP_GAIN:
         case ADJUST_ITEM_WOPC:
         case ADJUST_ITEM_FOC_INT:
         case ADJUST_ITEM_RE:
         case ADJUST_ITEM_REFLECTION:
         case ADJUST_ITEM_FRD_REF:
         case ADJUST_ITEM_ERASE_REFLECTION:
         case ADJUST_ITEM_DUTYCYCLE_5_12V:
         case ADJUST_ITEM_LASER_TEMP_ACT_DISP:
         case ADJUST_ITEM_LASER_TRESHOLD_CURRENT:
         case ADJUST_ITEM_LAST:
         default:
            send_msg80(1,0x710074);//DEBUG_system_dma_printf((DBG_ADJUST"WARNING invalid item=%d,%d cost=%d", curr.item[0],curr.item[1], curr.cost));
            rtnval = FALSE;
            break;
      }
   }

   switch (curr.cost)
   {
      case ADJUST_COST_STREAM_SAMPLED_JITTER:
      case ADJUST_COST_ARM_SAMPLED_JITTER:
      case ADJUST_COST_STREAM_SAMPLED_JITTER_ITEMSCAN:
      case ADJUST_COST_ARM_SAMPLED_JITTER_ITEMSCAN:
         coef.a1_min                         = 0.0;
         coef.a1_max                         = FLT_MAX;
         coef.a2_min                         = -1*FLT_MAX;
         coef.a2_max                         = FLT_MAX;
         coef.curve_up                       = TRUE;
         break;
      case ADJUST_COST_PUSHPULL_AMPL:
      case ADJUST_COST_WOBBLE_AMPL:
      case ADJUST_COST_PUSHPULL_AMPL_ITEMSCAN:
      case ADJUST_COST_WOBBLE_AMPL_ITEMSCAN:
         coef.a1_min                         = -1*FLT_MAX;
         coef.a1_max                         = 0.0;
         coef.a2_min                         = -1*FLT_MAX;
         coef.a2_max                         = FLT_MAX;
         coef.curve_up                       = FALSE;
         break;
      case ADJUST_COST_ABORT:
      case ADJUST_COST_NONE:
      case ADJUST_COST_LAST:
      default:
         send_msg80(1,0x710074);//DEBUG_system_dma_printf((DBG_ADJUST"WARNING invalid item=%d,%d cost=%d", curr.item[0],curr.item[1], curr.cost));
         rtnval = FALSE;
         break;
   }
   return (rtnval);
}

/*--------------------------------------------------------------------------*/

static void adjust_init_2d_measurements (void)
{
   LONG           item_number;
   Math2d_StepT   steps_adjust_2d;

   if(data.retry == 0)
   {
      for(item_number=0; item_number < MAX_NO_ITEMS; item_number++)
      {
         steps_adjust_2d.MaxIterations               = 5;
         steps_adjust_2d.Par[item_number].Max        = (SHORT)coef.p[item_number].max;
         steps_adjust_2d.Par[item_number].Min        = (SHORT)coef.p[item_number].min;

         switch (curr.cost)
         {
            case ADJUST_COST_PUSHPULL_AMPL    :
            case ADJUST_COST_WOBBLE_AMPL      :
            {
               switch ( curr.item[item_number] )
               {
                  case ADJUST_ITEM_FOCUS_OFFSET:
                     //overwrite min / max from coef values
                     steps_adjust_2d.Par[item_number].Max       = (LONG)((SHORT)COEF_GetValueParams(curr.DiscKind,curr.speed, SRV_CALC_P_MAX_FOO));
                     steps_adjust_2d.Par[item_number].Min       = -steps_adjust_2d.Par[item_number].Max;
                     steps_adjust_2d.Par[item_number].DeltaFac  = (LONG)((SHORT)COEF_GetValueParams(curr.DiscKind,curr.speed, SRV_CALC_P_RANGE_2D_COST_IS_FOO_WOBBLE));
                     break;
                  case ADJUST_ITEM_SPHER_ABER:
                     //overwrite min / max from coef values
                     steps_adjust_2d.Par[item_number].Max       = (LONG)((SHORT)COEF_GetValueParams(curr.DiscKind,curr.speed, SRV_CALC_P_MAX_SPA));
                     steps_adjust_2d.Par[item_number].Min       = (LONG)((SHORT)COEF_GetValueParams(curr.DiscKind,curr.speed, SRV_CALC_P_MIN_SPA));
                     steps_adjust_2d.Par[item_number].DeltaFac  = (LONG)((SHORT)COEF_GetValueParams(curr.DiscKind,curr.speed, SRV_CALC_P_RANGE_2D_COST_IS_SPA_WOBBLE));
                     break;
                  default:
                     steps_adjust_2d.Par[item_number].DeltaFac  = (SHORT)(2*coef.p[item_number].step_lo);
                     break;
               }//lint !e788
            }
            break;

            case ADJUST_COST_STREAM_SAMPLED_JITTER :
               {
                  switch ( curr.item[item_number] )
                  {
                     case ADJUST_ITEM_FOCUS_OFFSET:
                        steps_adjust_2d.Par[item_number].DeltaFac  = (LONG)((SHORT)COEF_GetValueParams(curr.DiscKind,curr.speed, SRV_CALC_P_RANGE_2D_COST_IS_FOO_JITTER));
                        break;
                     case ADJUST_ITEM_SPHER_ABER:
                        steps_adjust_2d.Par[item_number].DeltaFac  = (LONG)((SHORT)COEF_GetValueParams(curr.DiscKind,curr.speed, SRV_CALC_P_RANGE_2D_COST_IS_SPA_WOBBLE));
                        break;
                     default:
                        steps_adjust_2d.Par[item_number].DeltaFac  = (SHORT)(2*coef.p[item_number].step_lo);
                        break;
                  }//lint !e788
               }
               break;

            case ADJUST_COST_ARM_SAMPLED_JITTER                 :
            case ADJUST_COST_STREAM_SAMPLED_JITTER_ITEMSCAN        :
            case ADJUST_COST_ARM_SAMPLED_JITTER_ITEMSCAN        :
            case ADJUST_COST_PUSHPULL_AMPL_ITEMSCAN              :
            case ADJUST_COST_WOBBLE_AMPL_ITEMSCAN                :
            case ADJUST_COST_ABORT                               :
            case ADJUST_COST_NONE                                :
            case ADJUST_COST_LAST                                :
            default                                              :
               steps_adjust_2d.Par[item_number].DeltaFac  = (SHORT)(2*coef.p[item_number].step_lo);
               break;
         }

         //DEBUG_printf(("init_2d_measurements delta_fac = %d (0x%x)",steps_adjust_2d.Par[item_number].delta_fac,steps_adjust_2d.Par[item_number].delta_fac));
         steps_adjust_2d.Par[item_number].ZeroPoint = (SHORT)data.p_start_first[item_number];
         //DEBUG_printf(("init_2d_measurements zero_point = %d (0x%x)",steps_adjust_2d.Par[item_number].zero_point,steps_adjust_2d.Par[item_number].zero_point));
         steps_adjust_2d.Par[item_number].Relative  = FALSE;

         if ( (curr.config & ADJ_CONFIG_2D_MODE_MASK)== ADJ_CONFIG_2D_MODE_ROTATED)
         {
            steps_adjust_2d.eMode                   = MODE_2D_ROTATED;
            //DEBUG_printf(("MODE_2D_ROTATED"));
         }
         else
         {
            steps_adjust_2d.eMode                   = MODE_2D_NORMAL;
            //DEBUG_printf(("MODE_2D_NORMAL"));
         }
         if ((curr.cost == ADJUST_COST_WOBBLE_AMPL)|| (curr.cost == ADJUST_COST_PUSHPULL_AMPL))
         {
            steps_adjust_2d.CostInverted         = TRUE;
         }
         else
         {
            steps_adjust_2d.CostInverted         = FALSE;
         }
      }
      MATH2D_Init( &steps_adjust_2d );
   }

}
/*--------------------------------------------------------------------------*/

static void adjust_prepare_2d_measurements (void)
{
   LONG cnt;
   MATH2D_GetExperimentPlan( experiment_plan_2d );
   for(cnt = 0; cnt < NR_OF_2D_EXPERIMENTS; cnt++)
   {
      data.buf[0][cnt].p[0] = experiment_plan_2d[cnt].Param[0];
      data.buf[0][cnt].p[1] = experiment_plan_2d[cnt].Param[1];
   }
   // for 2D we need to follow the measurement plan :
   // this means that all (2 items) the initial measure-poinst also need to be overruled:
   data.p_start_actual[0] = data.buf[0][0].p[0] ;
   data.p_start_actual[1] = data.buf[0][0].p[1] ;
   data.p_start_valid     = TRUE;

}

/*--------------------------------------------------------------------------*/

static void adjust_prepare_itemscan_measurements (void)
{
   data.buf[0][0].p[0]    = data.p_start_first[0];

   if ( (curr.DiscKind.fDiscKind == eBDRDLDisc)||(curr.DiscKind.fDiscKind == eBDREDLDisc) &&
        (curr.item[0] == ADJUST_ITEM_SPHER_ABER) )
   {
      data.buf[0][1].p[0]     = data.buf[0][0].p[0] + 80 ;
      data.buf[0][2].p[0]     = data.buf[0][0].p[0] - 80 ;
      //DEBUG_system_dma_printf(("BD itemscan lowering step size to 80 p0:%d p1:%d p2:%d ",data.buf[0][0].p[0], data.buf[0][1].p[0], data.buf[0][2].p[0]));
   }
   else
   {
      data.buf[0][1].p[0]    = data.buf[0][0].p[0] + 2*coef.p[0].step_lo;
      data.buf[0][2].p[0]    = data.buf[0][0].p[0] - 2*coef.p[0].step_lo;
   }

   // clip data
   if (data.buf[0][1].p[0] >  coef.p[0].max) data.buf[0][1].p[0] =  coef.p[0].max;
   if (data.buf[0][2].p[0] <  coef.p[0].min) data.buf[0][2].p[0] =  coef.p[0].min;
}

/*--------------------------------------------------------------------------*/

static BOOL adjust_p_is_end_of_flank (void)
{
   // end of flank:
   //    - all points filled OR
   //    - last point gave an error OR
   //    - cost of last two points too high
   //          (provided that there are (more than) 2 points on this flank)
   //    - a next p will fall outside domain

   BYTE data_limit = (optimal_algorithm)?(STORAGE_MAX - 1):(STORAGE_MAX/2 - 1);

   if (data.i >= (LONG) data_limit)
   {
      return (TRUE);
   }

   if (curr.mode != ADJUST_MODE_2D)
   {
      if (data.buf[data.flank][data.i].servo_recoveries > 0)
      {
         if ((data.flank == 1) && (data.i == 0) && (data.buf[data.flank][data.i].servo_recoveries == 1))
         {
            //DEBUG_system_dma_printf (("zero point for 2nd flank, allow 1 servorecovery"));
         }
         else
         {
            //DEBUG_system_dma_printf (("servo recovery"));
            return (TRUE);
         }
      }

      if (data.buf[data.flank][data.i].hf_recoveries > NR_OF_RECOVERIES)
      {//same should not be measured forever
         //DEBUG_printf (("too many HF recoveries at %d", data.p[0]));
         if (optimal_algorithm)
         {
            coef.retry_max = 1;
         }
         return (TRUE);
      }
   }

   switch(curr.mode)
   {
      case ADJUST_MODE_1D:
         if (optimal_algorithm)
         {
            USHORT temp_min_delta_index;
            SHORT  opt_index;
            BYTE  loop;
            BYTE opt_point;
            LONG   x[STORAGE_MAX];
            LONG   y[STORAGE_MAX];
            BYTE  csize;

            for (loop = 0; loop < STORAGE_MAX; loop++)
            {
               x[loop] = (LONG)data.buf[data.flank][loop].p[0];
               y[loop] = (LONG)data.buf[data.flank][loop].c;
            }
            csize = (BYTE)data.i +1;
#if TO_BE_PORTED
            opt_point = LIB_curve_fit_data_filter1( x, y, &csize, (LONG)LONG_MAX-1, (LONG)0, coef.curve_up);
#endif
            //(algorithm) calculate optimize_min_delta_index according to opt_index
            if (data.i == 0)
            {
               opt_index = (coef.a1_min >0)? (SHORT)32767:(SHORT)-32768;
            }
            else
            {
               opt_index = (SHORT)y[opt_point];
            }
            temp_min_delta_index = coef.delta_relative? (USHORT)(ABS(opt_index) * coef.clim_rel_min / 100) : (USHORT)coef.clim_rel_min;

#if TO_BE_PORTED
            data.ramp_mode= LIB_curve_fit_data_analyser(x, y, csize, (BYTE)coef.i_min, (SHORT)coef.p[0].min, (SHORT)coef.p[0].max, temp_min_delta_index, coef.curve_up);
#endif

            for (loop = 0; loop < STORAGE_MAX; loop++)
            {
               data.buf[data.flank][loop].p[0] = x[loop];
               data.buf[data.flank][loop].c = y[loop];
            }
            data.i = csize -1;
            if((data.ramp_mode == RAMPING_END)||(data.ramp_mode == RAMPING_FAIL))
            {
               return (TRUE);
            }
         }
         else if (data.buf[data.flank][data.i].c != MAX_LONG)
         {
            if (data.buf[data.flank][data.i].c  > coef.clim_meas)
            {
               // last point gave too high cost
               //DEBUG_printf (("cost > clim meas"));
               return (TRUE);
            }

            if ((data.i >  2) &&
                  ((data.buf[data.flank][data.i-1].c*100/data.c_max)< coef.clim_rel_min) &&
                  ((data.buf[data.flank][data.i-2].c*100/data.c_max)< coef.clim_rel_min)   )
            {
               //DEBUG_printf (("costrel < clim rel min"));
               return (TRUE);
            }

            // check if computing a new p gives a valid p
            if ((((data.p[0] + coef.p[0].step_lo >                        coef.p[0].max) ||
                  (data.p[0] + coef.p[0].step_lo > data.p_start_actual[0] +  coef.p[0].max_rel) ||
                  (data.p[0] + coef.p[0].step_hi >                        coef.p[0].max) ||
                  (data.p[0] + coef.p[0].step_hi > data.p_start_actual[0] +  coef.p[0].max_rel) )
                 && (data.flank == 0) /*plus direction*/                        ) ||
                (((data.p[0] - coef.p[0].step_lo <                        coef.p[0].min) ||
                  (data.p[0] - coef.p[0].step_lo < data.p_start_actual[0] -  coef.p[0].max_rel) ||
                  (data.p[0] - coef.p[0].step_hi <                        coef.p[0].min) ||
                  (data.p[0] - coef.p[0].step_hi < data.p_start_actual[0] -  coef.p[0].max_rel))
                 && (data.flank == 1) /*min direction*/                         )    )
            {
               //DEBUG_printf (("p out of domain 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x",data.p[0], data.p_start_actual[0],
               //               coef.p[0].min , coef.p[0].max, coef.p[0].max_rel, coef.p[0].step_lo));
               return (TRUE);
            }
         }
         break;
      case ADJUST_MODE_2D:
         if ((data.i >= (NR_OF_2D_EXPERIMENTS - 1)) || (data.flank > 0))
         {
            return (TRUE);
         }
         break;
      case ADJUST_MODE_ITEMSCAN:
         if ((data.i >= 2) || (data.flank > 0))
         {
            return (TRUE);
         }
         break;
      case ADJUST_MODE_IDLE:
      case ADJUST_MODE_LAST:
      default:
         return (TRUE);
   }
   // not end of flank
   return (FALSE);
}

/*--------------------------------------------------------------------------*/

static BOOL adjust_p_set (LONG p, ADJUST_ITEM_t item)
{
   BOOL ready=FALSE;
   switch (item)
   {
      case ADJUST_ITEM_RAD_TILT:
#if TO_BE_PORTED
         TILT_set_tilt ((SHORT) p);
#endif
         ready= TRUE;
         break;

      case ADJUST_ITEM_FOCUS_OFFSET:
         FOCUS_SetOffset ((SHORT) p);
         ready= TRUE;
         break;

      case ADJUST_ITEM_SPHER_ABER:
         switch (svoStp_busy())
         {
            case FALSE :
#if COLLI_SWITCH ==1
               svoStp_JumpToPosition(COLLI_NORMAL_NO_REST_MODE,(SHORT) p);
#endif
               ready= TRUE;
               break;
            case TRUE:
            default:
               ready= FALSE; // retry later !
               break;
         }
         break;

      case ADJUST_ITEM_TANG_TILT:
#if TO_BE_PORTED
         switch (TANGTILT_get_status())
         {
            case READY :
               (void)TANGTILT_jump_init(TANGTILT_frame_to_crank((TANGTILT_ANGLE_FRAME_t)p));
               //KRNL_subscribe(OSAL_EVENT_BE_STEP_COLLI_ALPHA, ADJUST_TASKID);
               ready= TRUE;
               break;
            case ERROR :
               //DEBUG_system_dma_printf(("ADJUST: TANGTILT error"));
               ready = TRUE; // avoid hang, but what to do ?
               break;
            case BUSY:
            default:
               ready= FALSE; // retry later !
               break;
         }
#else
         ready= TRUE;
#endif
         break;

      case ADJUST_ITEM_RAD_OFFSET:
#if TO_BE_PORTED
         RADIAL_set_radial_offset_percent(init_rad_offset_perc + (SHORT) p);
#endif
         ready= TRUE;
         break;

      case ADJUST_ITEM_WOPC:
      case ADJUST_ITEM_RE_GAIN_OFFSET:
         //DEBUG_system_dma_printf(("%s is not a calibration that should be done using ADJUST",
         //                          ADJUST_ITEM_to_string[item]));
         ready= TRUE;
         break;

      case ADJUST_ITEM_SSPP_GAIN:
      case ADJUST_ITEM_FOC_INT:
      case ADJUST_ITEM_RE:
      case ADJUST_ITEM_REFLECTION:
      case ADJUST_ITEM_FRD_REF:
      case ADJUST_ITEM_ERASE_REFLECTION:
      case ADJUST_ITEM_DUTYCYCLE_5_12V:
      case ADJUST_ITEM_LASER_TEMP_ACT_DISP:
      case ADJUST_ITEM_LASER_TRESHOLD_CURRENT:
      case ADJUST_ITEM_ABORT:
      case ADJUST_ITEM_NONE:
      case ADJUST_ITEM_LAST:
      default:
         ready= TRUE;
         break;
   }
   return(ready);
}

/*--------------------------------------------------------------------------*/

static void adjust_conditional_push_out_of_lock (void)
{
   if ( (curr.DiscKind.Bit.DiscStd == eDiscBD) &&
         ((curr.cost == ADJUST_COST_ARM_SAMPLED_JITTER) ||
          (curr.cost == ADJUST_COST_STREAM_SAMPLED_JITTER) ||
          (curr.cost == ADJUST_COST_ARM_SAMPLED_JITTER_ITEMSCAN) ||
          (curr.cost == ADJUST_COST_STREAM_SAMPLED_JITTER_ITEMSCAN)   ) )
   {
#if TO_BE_PORTED
      C_DEC_push_out_of_lock();
#endif
   }
}

/*--------------------------------------------------------------------------*/

static void adjust_set_settle_timer (BYTE delay)
{
   // wait for settle-time
   // special situation when settle time is 0
   // (proceed immediately because TIMER-module wouldn't call the callback)

   if (delay == 0)
   {
      //KRNL_attach (TIMEBASE_0_MS, ADJUST_TASKID);
   }
   else
   {
      SetTimeout(2, MS_1*delay); //TIMER_start_timer (TIMER_ID_1_MS_ADJUST, delay, adjust_callback_func);
   }

}

/*--------------------------------------------------------------------------*/

static void adjust_p_next_1D(void)
{
   //only 1 item => index 0
   if ((!optimal_algorithm && (data.flank == 0)) || (optimal_algorithm && (data.ramp_mode == RAMPING_UP)))
   {
      if (data.buf[data.flank][data.i].c > coef.clim_step)
      {
         data.p[0] = (optimal_algorithm) ? (data.buf[0][data.i].p[0] + coef.p[0].step_hi) : (data.p[0] + coef.p[0].step_hi);
      }
      else
      {
         data.p[0] = (optimal_algorithm) ? (data.buf[0][data.i].p[0] + coef.p[0].step_lo) : (data.p[0] + coef.p[0].step_lo);
      }
   }
   else if (optimal_algorithm && (data.ramp_mode == RAMPING_DOWN))
   {
      if (data.buf[0][0].c > coef.clim_step)
      {
         data.p[0] = data.buf[0][0].p[0] - coef.p[0].step_hi;
      }
      else
      {
         data.p[0] = data.buf[0][0].p[0] - coef.p[0].step_lo;
      }
   }
   else
   {
      DEBUG_ASSERT((data.flank != FLANK_MAX-1),eASSERT_FUNCTION_NOT_IMPLEMENTED);//DEBUG_assert ((data.flank == FLANK_MAX-1) ? TRUE : FALSE);
      if (data.buf[data.flank][data.i].c > coef.clim_step)
      {
         data.p[0] -= coef.p[0].step_hi;
      }
      else
      {
         data.p[0] -= coef.p[0].step_lo;
      }
   }

   // clip data.p[item_number]
   if (data.p[0] >  coef.p[0].max) data.p[0] =  coef.p[0].max;
   if (data.p[0] <  coef.p[0].min) data.p[0] =  coef.p[0].min;
}

/*--------------------------------------------------------------------------*/

static void adjust_p_next_2D(void)
{
   LONG  item_number;

   for(item_number=0; item_number < MAX_NO_ITEMS; item_number++)
   {
      data.p[item_number] = data.buf[data.flank][data.i+1].p[item_number];
   }
}

/*--------------------------------------------------------------------------*/

static void adjust_p_next_itemscan(void)
{
   data.p[0] = data.buf[data.flank][data.i+1].p[0];
}

/*--------------------------------------------------------------------------*/

static BOOL adjust_p_next_determine (void)
{
   LONG  item_number = 0;
   if (optimal_algorithm)
   {
      data.ramp_mode = RAMPING_UP;
   }
   // TODO: determine & set next p
   if ((data.flank == 0) && (data.i == -1))
   {
      data.i++;
      // very first point
      for(item_number=0; item_number < MAX_NO_ITEMS; item_number++)
      {
         data.p[item_number] = data.p_start_actual[item_number];
      }
   }
   else
   {
      DEBUG_ASSERT((data.i >= 0) ? TRUE : FALSE,eASSERT_FUNCTION_NOT_IMPLEMENTED);//DEBUG_assert ((data.i >= 0) ? TRUE : FALSE);
      if (adjust_p_is_end_of_flank ())
      {
         if (optimal_algorithm && (data.i >= 0))
         {
            for(item_number=0; item_number < MAX_NO_ITEMS; item_number++)
            {
               data.p[item_number] = data.p_start_actual[item_number];
            }
            return (FALSE);
         }
         else
         {
         //DEBUG_system_dma_printf (("next flank [%d %d] 0x%x 0x%x",data.flank, data.i,
         //     data.buf[data.flank][data.i].c, coef.clim_meas));

         if (data.flank >= (nrofflanks[curr.mode] - 1))
         {
            // end of last flank: ready
            //DEBUG_printf (("last flank"));
            return (FALSE);
         }
         // start new flank
         else
         {
          data.flank++;
          data.i = 0;
         }
         for(item_number=0; item_number < MAX_NO_ITEMS; item_number++)
         {
            data.p[item_number] = data.p_start_actual[item_number];
            }
         }
      }
      else
      {
         // not end of flank: determine next data.p[item_number]
         if ((curr.mode == ADJUST_MODE_2D) || //for 2D we don't retry per item, but MATH2D calculates the retry pattern
             !(  (data.buf[data.flank][data.i].c == MAX_LONG) &&
                ((data.buf[data.flank][data.i].hf_recoveries > 0) ||
                 (data.buf[data.flank][data.i].servo_recoveries > 0)) ))
         {
            // point has no recovery problem
            switch(curr.mode)
            {
               case ADJUST_MODE_1D:
                  adjust_p_next_1D();
                  break;
               case ADJUST_MODE_2D:
                  adjust_p_next_2D();
                  break;
               case ADJUST_MODE_ITEMSCAN:
                  adjust_p_next_itemscan();
                  break;
               case ADJUST_MODE_IDLE:
               case ADJUST_MODE_LAST:
               default:
                  break;
            }
            data.i++;
         }
         // else: c of previous p is Max_long, and a recovery is needed:
         // do this point once again (i.e. no update of data.p[item_number])
      }
   }

   DEBUG_ASSERT(((data.i >= 0) && (data.i < STORAGE_MAX) && (data.flank >= 0) && (data.flank < FLANK_MAX) && (data.ramp_mode >= RAMPING_UP)) ? TRUE : FALSE,eASSERT_FUNCTION_NOT_IMPLEMENTED);//DEBUG_assert (((data.i >= 0) && (data.i < STORAGE_MAX) && (data.flank >= 0) && (data.flank < FLANK_MAX) && (data.ramp_mode >= RAMPING_UP)) ? TRUE : FALSE);
   if (curr.mode == ADJUST_MODE_1D)
   {//1D
      data.buf[data.flank][data.i].p[0] = data.p[0];
   }//for 2D buf.p is filled out in front

   return (TRUE);
}

/*-----------------------------------------------------------------------*/

void adjust_wob_amp_setting(ADJUST_WOB_PP_SETTING_t wob_state)
{
#if TO_BE_PORTED
   static BYTE wobble_ki;
   /*lint -e641*/
   BYTE wobble_ki_input[NO_OF_DISC_KINDS] =
            {
               0x00, //DISC_KIND_UNKNOWN,
               0x07, //DISC_KIND_CD,
               0x00, //DISC_KIND_DVD,
               0x00, //DISC_KIND_BD
            };

   USHORT wobble_freq_input[NO_OF_DISC_KINDS] =
            {
               0x0005,  //DISC_KIND_UNKNOWN,
               0x0005,  //DISC_KIND_CD,
               0x000F,  //DISC_KIND_DVD,
               0x0400,  //DISC_KIND_BD
            };
#endif

   switch (wob_state)
   {
      case WOB_PP_PREPARE:
#if TO_BE_PORTED
         if (curr.DiscKind.Bit.DiscStd == eDiscBD)
         {
            if( !FRONT_is_sspp_tracking_enabled())
            {
               wobble_ki = READ_PROP(TMHAL_CCWOBREG_KI);
               WRITE_PROP(TMHAL_CCWOBREG_PRESETWOBBLEAGC,TRUE);
               WRITE_PROP(TMHAL_CCWOBREG_PRESETWOBBLEAGCDATA,0x08);
               WRITE_PROP(TMHAL_CCWOBREG_WOBBLEAGCON,FALSE);
               WRITE_PROP(TMHAL_CCWOBREG_KI,wobble_ki_input[curr.DiscKind]);
               WRITE_PROP(TMHAL_CCWOBREG_BDWOBBLEFREQ,wobble_freq_input[curr.DiscKind]);
            }
            else// BD-DL
            {
               wobble_ki = READ_PROP(TMHAL_CCWOBREG_KI);
               WRITE_PROP(TMHAL_CCWOBREG_PRESETWOBBLEAGC,TRUE);
               WRITE_PROP(TMHAL_CCWOBREG_PRESETWOBBLEAGCDATA,0x0C);
               WRITE_PROP(TMHAL_CCWOBREG_WOBBLEAGCON,FALSE);
               //DEBUG_system_dma_printf(("ADJUST START WOB FREQ : 0x%x",READ_PROP(TMHAL_CCWOBREG_BDWOBBLEFREQ)));
            }
         }
         else
         {
            wobble_ki = READ_PROP(hal_cc_WOB_LOOPKI);
            WRITE_PROP(hal_cc_WOB_PRESETWOBBLEAGC,TRUE);
            WRITE_PROP(hal_cc_WOB_PRESETWOBBLEAGCDATA,0x08);
            WRITE_PROP(hal_cc_WOB_WOBBLEAGCON,FALSE);
            WRITE_PROP(hal_cc_WOB_LOOPKI,wobble_ki_input[curr.DiscKind]);
            WRITE_PROP(hal_cc_WOB_DVDWOBBLEFREQUENCY,wobble_freq_input[curr.DiscKind]);
         }
#endif
         break;
      case WOB_PP_RESTORE:
#if TO_BE_PORTED
         if (curr.DiscKind.Bit.DiscStd == eDiscBD)
         {
            WRITE_PROP(TMHAL_CCWOBREG_KI,wobble_ki);
            WRITE_PROP(TMHAL_CCWOBREG_PRESETWOBBLEAGC,FALSE);
            WRITE_PROP(TMHAL_CCWOBREG_WOBBLEAGCON,TRUE);
         }
         else
         {
            WRITE_PROP(hal_cc_WOB_LOOPKI,wobble_ki);
            WRITE_PROP(hal_cc_WOB_PRESETWOBBLEAGC,FALSE);
            WRITE_PROP(hal_cc_WOB_WOBBLEAGCON,TRUE);
         }
#endif
         break;
   }
   /*lint +e641*/

}

/*-----------------------------------------------------------------------*/

static void adjust_cost_measure_end (void)
{
#if TO_BE_PORTED
   C_DEC_HoldOnDefect(C_DEC_HOLDEN_RESTORE);

   switch (curr.cost)
   {
      case ADJUST_COST_PUSHPULL_AMPL:
      case ADJUST_COST_WOBBLE_AMPL:
      case ADJUST_COST_STREAM_SAMPLED_JITTER:
      case ADJUST_COST_ARM_SAMPLED_JITTER:
      case ADJUST_COST_PUSHPULL_AMPL_ITEMSCAN:
      case ADJUST_COST_WOBBLE_AMPL_ITEMSCAN:
      case ADJUST_COST_STREAM_SAMPLED_JITTER_ITEMSCAN:
      case ADJUST_COST_ARM_SAMPLED_JITTER_ITEMSCAN:
         FAST_MEAS_measure_stop();
         //KRNL_unsubscribe (OSAL_EVENT_BE_ANG_TIMER, ADJUST_TASKID);
         //KRNL_detach (ADJUST_TASKID);
         break;

      case ADJUST_COST_ABORT:
      case ADJUST_COST_NONE:
      case ADJUST_COST_LAST:
      default:
         DEBUG_ASSERT(FALSE, eASSERT_FUNCTION_NOT_IMPLEMENTED);
         break;
   }
#endif
}

/*-----------------------------------------------------------------------*/
#if TEMP_NOT_IN_USE
static void adjust_sort_x_ascending(LONG * x,LONG * y, LONG k)
{
   LONG temp;
   LONG i,j;

   for(j=0; j < k; j++)
   {
      for(i=0; (i+1 < k-j); i++)
      {
         if(x[i] > x[i+1])
         {
            //swap values
            temp   = x[i+1];
            x[i+1] = x[i];
            x[i]   = temp;
            temp   = y[i+1];
            y[i+1] = y[i];
            y[i]   = temp;
         }
      }
   }
}
#endif
/*-----------------------------------------------------------------------*/
#if TEMP_NOT_IN_USE
static LONG adjust_search_i_y_min(LONG * y, LONG k)
{
   LONG i_y_min=-1, i, y_min;

   y_min = MAX_LONG;
   for(i=0; i<k; i++)
   {
      if(y[i] < y_min)
      {
         y_min   = y[i];
         i_y_min = i;
      }
   }
   return i_y_min;
}
#endif
/*-----------------------------------------------------------------------*/
#if TEMP_NOT_IN_USE
static LONG adjust_search_left_step_lo(LONG * x_tmp, LONG i_y_min)
{
   LONG x_search, i;

   x_search = x_tmp[i_y_min] - coef.p[curr.item[0]].step_lo;
   i=i_y_min;
   while(i >= 0 && x_tmp[i] > x_search )
   {
      i--;
   }
   return i;
}
#endif
/*-----------------------------------------------------------------------*/
#if TEMP_NOT_IN_USE
static LONG adjust_search_right_step_lo(LONG * x_tmp, LONG  i_y_min, LONG k)
{
   LONG x_search, i;

   x_search = x_tmp[i_y_min] + coef.p[curr.item[0]].step_lo;
   i=i_y_min;
   while(i <= k && x_tmp[i] < x_search )
   {
      i++;
   }
   return i;
}
#endif
/*-----------------------------------------------------------------------*/
#if TEMP_NOT_IN_USE
static BOOL adjust_check_nodyn_and_calc(LONG * x, LONG * y, BYTE k, LONG * x_result, LONG * y_result)
{
   LONG x_tmp[FLANK_MAX * STORAGE_MAX];
   LONG y_tmp[FLANK_MAX * STORAGE_MAX];
   LONG i_y_min, cnt;
   BOOL result = FALSE,foundl = FALSE,foundr = FALSE;
   #define DYNAMICS 25
   #define ALLOWED  20

   //Measured points do not correlate curve fit will give bad optimum!
   memcpy(x_tmp, x, sizeof(x_tmp));
   memcpy(y_tmp, y, sizeof(y_tmp));
   //sort x ascending
   adjust_sort_x_ascending(x_tmp, y_tmp, k);
   //Search for minimum y index
   i_y_min = adjust_search_i_y_min(y_tmp, k);
   *y_result = y_tmp[i_y_min];
   //check left
   for(cnt = 0; ((cnt < i_y_min) && !foundl); cnt++)
   {
      if(y_tmp[cnt] >= (y_tmp[i_y_min]+ DYNAMICS))
      {
         foundl = TRUE;
      }
   }
   //check right
   for(cnt = i_y_min; ((cnt < (LONG)k) && !foundr); cnt++)
   {
      if(y_tmp[cnt] >= (y_tmp[i_y_min]+ DYNAMICS))
      {
         foundr = TRUE;
      }
   }
   result = !(foundr && foundl);
   if (result)
   {
      LONG counter = 0;
      *x_result = 0;
      //not enough dynamics, search new optimum
      //DEBUG_system_dma_printf(("not enough dynamics, next points taken into account"));
      for(cnt = 0; (cnt < (LONG)k); cnt++)
      {
         if(y_tmp[cnt] < (y_tmp[i_y_min]+ ALLOWED))
         {
            counter++;
            //DEBUG_system_dma_printf(("0x%4x\t(\t%d\t)",x_tmp[cnt], x_tmp[cnt] ));
            *x_result += x_tmp[cnt];
         }
      }
      if( counter == 0)
      {
         *x_result = x_tmp[i_y_min];
      }
      else
      {
         *x_result /= counter;
      }
   }
   return result;
}
#endif
/*-----------------------------------------------------------------------*/
#define ADJ_MAX_DELTA_JITTER  20         // ADJ_MAX_DELTA_JITTER / 10 -> jitter
static void adjust_process_1d_results(void)
{
#if TO_BE_PORTED
   int               i, j, k;
   int               k_max_flank[FLANK_MAX];
   int               flank_no_points[FLANK_MAX];
   LONG              c;
   LONG              p;
   LONG              cmin, cmax, pmin, pmax;
   LONG              x [FLANK_MAX * STORAGE_MAX];
   LONG              y [FLANK_MAX * STORAGE_MAX];
   lib_curve_fit_t   curve;
   float             optimum;

   //DEBUG_printf (("P-coefs delay=%d,%d hi/lo=0x%x/0x%x max=0x%x/0x%x rel=0x%x min=%d\nC-coefs step=0x%x meas=0x%x",
   //   coef.p[0].delay_small,coef.p[0].delay_big, coef.p[0].step_hi, coef.p[0].step_lo, coef.p[0].min , coef.p[0].max, coef.p[0].max_rel, coef.i_min,
   //   coef.clim_step, coef.clim_meas));
   for (i = STORAGE_MAX-1; (i >= 0) ; i--)
   {
      if (!((data.buf[1][i].p[0] == MAX_LONG) && (data.buf[1][i].c == MAX_LONG)))
      {
         //DEBUG_system_dma_printf (("1.%02d %6d %6d", i, data.buf[1][i].p[0], data.buf[1][i].c));
      }
   }
   for (i = 0; (i < STORAGE_MAX); i++)
   {
      if (!((data.buf[0][i].p[0] == MAX_LONG) && (data.buf[0][i].c == MAX_LONG)))
      {
         //DEBUG_system_dma_printf (("0.%02d %6d %6d", i, data.buf[0][i].p[0], data.buf[0][i].c));
      }
   }

   k = 0;
   y[0]=x[0]=0;
   k_max_flank[0] = k_max_flank[1] = 0;
   flank_no_points[0] = flank_no_points[1] = 0;
   cmin = MAX_LONG;
   cmax = MIN_LONG;
   pmin = MAX_LONG;
   pmax = MIN_LONG;
   // check data.c_buf[] and put validated points in x[] and y[]
   for (i = 0; i < FLANK_MAX; i++)
   {
      for (j = 0; j < STORAGE_MAX; j++)
      {
         c = data.buf[i][j].c;
         p = data.buf[i][j].p[0];
         if ((c >= coef.clim_val_min) && (c <= coef.clim_val_max))
         {
            x [k] = p;
            y [k] = c;
            if (c < cmin) cmin = c;
            if (c > cmax) cmax = c;
            if (p < pmin) pmin = p;
            if (p > pmax) pmax = p;
            k_max_flank[i] = k;
            flank_no_points[i]++;
            k++;
         }
         else
         {
            ////DEBUG_printf (("skip[%d,%d]p=0x%x c=0x%x/0x%x/0x%x", i, j, p, c, coef.clim_val_min, coef.clim_val_max));
         }
      }
   }

   user_result.ok = FALSE;
   if ((k < coef.i_min) ||
       ( (flank_no_points[0] < 2) && (x[k_max_flank[0]] + coef.p[0].step_lo < coef.p[0].max) &&
                  (x[k_max_flank[0]] + coef.p[0].step_hi < coef.p[0].max)) ||
       ( (flank_no_points[1] < 2) && (x[k_max_flank[1]] - coef.p[0].step_lo > coef.p[0].min) &&
                  (x[k_max_flank[1]] - coef.p[0].step_hi > coef.p[0].min)) )
   {
      // not enough points collected
      //DEBUG_system_dma_printf((DBG_ADJUST"WARNING npoints #%d<%d or bad flank", k, coef.i_min));
   }
   else

   {
      if(  (curr.item[0] == ADJUST_ITEM_FOCUS_OFFSET)
           && (curr.DiscKind.Bit.DiscStd == eDiscDVD)
           && ((curr.cost == ADJUST_COST_ARM_SAMPLED_JITTER) ||
               (curr.cost == ADJUST_COST_STREAM_SAMPLED_JITTER))
           && adjust_check_nodyn_and_calc(x, y, (BYTE) k,&(user_result.item_optimum[0]), &(user_result.cost_optimum)))
      {
        user_result.ok = TRUE;
      }
      else
      {
        // do the curve-fitting
        //DEBUG_printf (("p:min/max=%d,%d c:min/max=%d,%d #=%d", pmin, pmax, cmin, cmax, k));
        LIB_curve_fit (x, y, (BYTE) k, &curve);
        //DEBUG_system_dma_printf((DBG_ADJUST"ADJUST interp: c2=%.3e c1=%.3e c0=%.2f", curve.coefa, curve.coefb, curve.coefc));
        if (curve.coefa == 0.0)
        {
           //DEBUG_system_dma_printf((DBG_ADJUST"WARNING interp.failed a1 = 0"));
        }
        else
        {
           // check that the curve-fit results are within bounds
           optimum = -curve.coefb / (2.0 * curve.coefa);
           user_result.item_optimum[0] = (LONG) optimum;
           if( (curve.coefa < coef.a1_min) ||
               (curve.coefa > coef.a1_max) )
           {
              //DEBUG_system_dma_printf((DBG_ADJUST"WARNING interp.result a1 out-of-range (a1=%d)",curve.coefa));
           }
           else
           {
              if ((user_result.item_optimum[0] <  coef.p[0].result_min) ||
                  (user_result.item_optimum[0]  > coef.p[0].result_max) ||
                  (curve.coefb < coef.a2_min) ||
                  (curve.coefb > coef.a2_max)
                 )
              {
                 //DEBUG_system_dma_printf((DBG_ADJUST"WARNING interp.result c2 or range p nok:0x%x/0x%x/0x%x c2:%e/%e/%e c1:%e/%e/%e",
                 //      user_result.item_optimum[0], coef.p[0].result_min, coef.p[0].result_max,
                 //      curve.coefa, coef.a1_min, coef.a1_max,
                 //      curve.coefb, coef.a2_min, coef.a2_max));
              }
              else
              {
                 if ((user_result.item_optimum[0] > x[k_max_flank[0]]) || ((user_result.item_optimum[0] < x[k_max_flank[1]])))
                 { //clipping
                    //DEBUG_system_dma_printf((DBG_ADJUST"WARNING opt=0x%x out-of-range",user_result.item_optimum[0]));
                    if (adjust_servo_limits[curr.item[0]][servo_limit_layer].determined == TRUE)  // 1D calib, only check item 0 !
                    {
                       long delta;

                       if(user_result.item_optimum[0] > x[k_max_flank[0]])
                       {
                          if(x[k_max_flank[0]] > (LONG)adjust_servo_limits[curr.item[0]][servo_limit_layer].max_optimum)
                          {
                             delta = 0;
                          }
                          else
                          {
                             delta = ABS((LONG)adjust_servo_limits[curr.item[0]][servo_limit_layer].max_optimum - x[k_max_flank[0]]);
                          }

                          if(delta < coef.p[0].step_hi)
                          {
                             //DEBUG_system_dma_printf((DBG_ADJUST"WARNING clipped optimum to upper limit %d --> %d",
                             //                         user_result.item_optimum[0],
                             //                         (LONG)adjust_servo_limits[curr.item[0]][servo_limit_layer].max_optimum ));
                             user_result.item_optimum[0] = (LONG)adjust_servo_limits[curr.item[0]][servo_limit_layer].max_optimum;
                             user_result.ok = TRUE;
                          }
                          else
                          {
                             //DEBUG_system_dma_printf((DBG_ADJUST"WARNING max flank %d, servo limit %d, step hi %d",
                             //                         x[k_max_flank[0]], adjust_servo_limits[curr.item[0]][servo_limit_layer].max_optimum, coef.p[0].step_hi));
                          }
                       }
                       else if ( user_result.item_optimum[0] < x[k_max_flank[1]])
                       {
                          if(x[k_max_flank[1]] > (LONG)adjust_servo_limits[curr.item[0]][servo_limit_layer].min_optimum)
                          {
                             delta = ABS(x[k_max_flank[1]] - (LONG)adjust_servo_limits[curr.item[0]][servo_limit_layer].min_optimum);
                          }
                          else
                          {
                             delta = 0;
                          }

                          if(delta < coef.p[0].step_hi)
                          {
                             //DEBUG_system_dma_printf((DBG_ADJUST"WARNING clipped optimum to lower limit %d --> %d",
                             //                         user_result.item_optimum[0],
                             //                         (LONG)adjust_servo_limits[curr.item[0]][servo_limit_layer].min_optimum ));
                             user_result.item_optimum[0] = (LONG)adjust_servo_limits[curr.item[0]][servo_limit_layer].min_optimum;
                             user_result.ok = TRUE;
                          }
                          else
                          {
                             //DEBUG_system_dma_printf((DBG_ADJUST"WARNING max flank %d, servo limit %d, step hi %d",
                             //                         x[k_max_flank[1]], adjust_servo_limits[curr.item[0]][servo_limit_layer].min_optimum, coef.p[0].step_hi));
                          }
                       }
                       //Determine the cost optimum  y = a x^2  + bx + c

                       user_result.cost_optimum = (LONG)((curve.coefa * user_result.item_optimum[0] * user_result.item_optimum[0]) +
                                                         (curve.coefb * user_result.item_optimum[0]) +
                                                          curve.coefc);
                    }
                 }
                 else
                 {//NORMAL
                    if( curve.coefr < 0.9 &&
                         DB_is_BD() &&
                        (curr.item[0] == ADJUST_ITEM_SPHER_ABER) &&
                        (curr.cost == ADJUST_COST_ARM_SAMPLED_JITTER || curr.cost == ADJUST_COST_STREAM_SAMPLED_JITTER) )
                    {
                       LONG x_tmp[FLANK_MAX * STORAGE_MAX];
                       LONG y_tmp[FLANK_MAX * STORAGE_MAX];
                       LONG i_y_min,i_x_left_step_lo,i_x_right_step_lo,delta_jitter_l, delta_jitter_r;

                       //Measured points do not correlate curve fit will give bad optimum!
                       memcpy(x_tmp, x, sizeof(x_tmp));
                       memcpy(y_tmp, y, sizeof(y_tmp));
                       //sort x ascending
                       adjust_sort_x_ascending(x_tmp, y_tmp, k);
                       for(i=0;i<k;i++)
                       {
                          //DEBUG_system_dma_printf(("x %d, y %d", x_tmp[i],y_tmp[i]));
                       }
                       //Search for minimum y index
                       i_y_min = adjust_search_i_y_min(y_tmp, k);
                       //Validate minimum point on jitter marge
                       //search for point left which is step_lo away from minimum
                       i_x_left_step_lo = adjust_search_left_step_lo(x_tmp, i_y_min);
                       //search for point right which is step_lo away from minimum
                       i_x_right_step_lo = adjust_search_right_step_lo(x_tmp, i_y_min, k);
                       //determine delta jitter over the 2 points found.
                       delta_jitter_l = ABS(y_tmp[i_y_min] - y_tmp[i_x_left_step_lo]);
                       delta_jitter_r = ABS(y_tmp[i_y_min] - y_tmp[i_x_right_step_lo]);
                       //DEBUG_system_dma_printf(("i_x_left_step_lo %d, i_y_min %d, i_x_right_step_lo %d",i_x_left_step_lo, i_y_min, i_x_right_step_lo));
                       //DEBUG_system_dma_printf(("delta_jitter_l %d, delta_jitter_r %d", delta_jitter_l, delta_jitter_r));
                       if(delta_jitter_l < ADJ_MAX_DELTA_JITTER && delta_jitter_r < ADJ_MAX_DELTA_JITTER)
                       {
                          user_result.ok = TRUE;
                          user_result.item_optimum[0] = x_tmp[i_y_min];
                          user_result.cost_optimum    = y_tmp[i_y_min];
                       }
                    }
                    else
                    {
                       //Determine the cost optimum
                       if (adjust_servo_limits[curr.item[0]][servo_limit_layer].determined == TRUE)  // 1D calib, only check item 0 !
                       {
                          if(user_result.item_optimum[0] >  adjust_servo_limits[curr.item[0]][servo_limit_layer].max_optimum)
                          {
                             //DEBUG_system_dma_printf((DBG_ADJUST"WARNING clipped optimum to upper limit %d --> %d",
                             //                         user_result.item_optimum[0],
                             //                         adjust_servo_limits[curr.item[0]][servo_limit_layer].max_optimum   ));
                             user_result.item_optimum[0] = (LONG)adjust_servo_limits[curr.item[0]][servo_limit_layer].max_optimum;
                          }
                          else if (user_result.item_optimum[0] < adjust_servo_limits[curr.item[0]][servo_limit_layer].min_optimum)
                          {
                             //DEBUG_system_dma_printf((DBG_ADJUST"WARNING clipped optimum to lower limit %d --> %d",
                             //                    user_result.item_optimum[0],
                             //                         adjust_servo_limits[curr.item[0]][servo_limit_layer].min_optimum ));
                             user_result.item_optimum[0] = (LONG)adjust_servo_limits[curr.item[0]][servo_limit_layer].min_optimum;
                          }
                       }
                       user_result.cost_optimum = (LONG)((curve.coefa * user_result.item_optimum[0] * user_result.item_optimum[0]) +
                                                         (curve.coefb * user_result.item_optimum[0]) +
                                                          curve.coefc);
                       user_result.ok = TRUE;
                       if ((curr.cost == ADJUST_COST_ARM_SAMPLED_JITTER) ||
                           (curr.cost == ADJUST_COST_STREAM_SAMPLED_JITTER) )
                       {
                          //calc measured jitter
                          LONG meas_jitter;
                          int k_min, k_max, ind;
                          LONG delta_min, delta_plus;
                          #define  ABSDIF(x,y)       ( ( (x) < (y) ) ? (LONG)((y)-(x)) : (LONG)((x)-(y)) )

                          k_max = k_max_flank[0];
                          k_min = k_max_flank[1];
                          delta_min = user_result.item_optimum[0] - x[k_min];
                          delta_plus = x[k_max] - user_result.item_optimum[0];
                          for(ind =0; ind<k; ind++)
                          {
                             if((x[ind] < user_result.item_optimum[0]) && (delta_min > (user_result.item_optimum[0] - x[ind])))
                             {
                                k_min = ind;
                                delta_min = user_result.item_optimum[0] - x[k_min];
                             }
                             if((x[ind] > user_result.item_optimum[0]) && (delta_plus > (x[ind] - user_result.item_optimum[0])))
                             {
                                k_max = ind;
                                delta_plus = x[k_max] - user_result.item_optimum[0];
                             }
                          }

                          if(k_min == k_max)
                          {
                             meas_jitter = y[k_min];
                          }
                          else
                          {
                             meas_jitter = LIB_linear_interpolation(x[k_min],y[k_min],x[k_max],y[k_max], user_result.item_optimum[0]);
                             //DEBUG_printf(("Meas_jitter %d (x%d,y%d,x%d,y%d)",meas_jitter,x[k_min],y[k_min],x[k_max],y[k_max]));
                          }

                          if (ABSDIF(meas_jitter, user_result.cost_optimum) > 15)
                          {
                             //DEBUG_system_dma_printf((" WARNING big dif meas vs calculated jitter is%d vs %d",meas_jitter, user_result.cost_optimum));
                          }
                          if (ABSDIF(data.buf[0][0].c,data.buf[1][0].c) > 15)
                          {
                             //DEBUG_system_dma_printf((" WARNING big dif meas zero point %d(0,0) vs %d(1,0)",data.buf[0][0].c,data.buf[1][0].c));
                          }
                       }
                    }
                 }
              }
           }
        }
      }
   }

   p_width = 0;
   if (user_result.ok)
   {
      if ( FRONT_is_sspp_tracking_enabled() &&
          (curr.DiscKind.Bit.DiscStd == eDiscBD) &&
          ( curr.item[0] == ADJUST_ITEM_RAD_TILT) &&
          ( curr.cost    == ADJUST_COST_PUSHPULL_AMPL) )
      {

         Math2d_ServoLimitPair servo_pp_limit;
         float temp;
         // 0x7FFF =15 mRad -->  13106 = 6mRad
         servo_pp_limit.param_max = user_result.item_optimum[0] - 13106;/*10000*/
         servo_pp_limit.param_min = user_result.item_optimum[0] + 13106;/*10000*/
         if (servo_pp_limit.param_min > servo_pp_limit.param_max)
         {
            temp             = servo_pp_limit.param_min;
            servo_pp_limit.param_min  = servo_pp_limit.param_max;
            servo_pp_limit.param_max  = temp;
         }
         //DEBUG_system_dma_printf(("fixed  (+/- 10000) RADIAL TILT LIMITS %d --> %d",(LONG)servo_pp_limit.param_min,(LONG)servo_pp_limit.param_max));
         // make sure that calculated limits do not exceed system limits.
         adjust_servo_limits[ADJUST_ITEM_RAD_TILT][eLayersSingle].determined = TRUE ;
         adjust_servo_limits[ADJUST_ITEM_RAD_TILT][eLayersDouble].determined = TRUE ;
         if (servo_pp_limit.param_min < (float)((SHORT)-COEF_GetValueParams(curr.DiscKind,curr.speed, SRV_CALC_P_MAX_TLT)) )
         {
            //DEBUG_system_dma_printf(("CLIP min boundary Push pull"));
            servo_pp_limit.param_min = (float)((SHORT)-COEF_GetValueParams(curr.DiscKind,curr.speed, SRV_CALC_P_MAX_TLT));
         }
         if (servo_pp_limit.param_max > (float)((SHORT)COEF_GetValueParams(curr.DiscKind,curr.speed, SRV_CALC_P_MAX_TLT)))
         {
            //DEBUG_system_dma_printf(("CLIP max boundary Push pull"));
            servo_pp_limit.param_max = (float)((SHORT)COEF_GetValueParams(curr.DiscKind,curr.speed, SRV_CALC_P_MAX_TLT));
         }
         // set in local DB  (set L0 because layer independant)
         adjust_servo_limits[ADJUST_ITEM_RAD_TILT][eLayersSingle].min_measure = servo_pp_limit.param_min;
         adjust_servo_limits[ADJUST_ITEM_RAD_TILT][eLayersSingle].max_measure = servo_pp_limit.param_max;
         adjust_servo_limits[ADJUST_ITEM_RAD_TILT][eLayersSingle].min_optimum = servo_pp_limit.param_min;
         adjust_servo_limits[ADJUST_ITEM_RAD_TILT][eLayersSingle].max_optimum = servo_pp_limit.param_max;
         adjust_servo_limits[ADJUST_ITEM_RAD_TILT][eLayersDouble].min_measure = servo_pp_limit.param_min;
         adjust_servo_limits[ADJUST_ITEM_RAD_TILT][eLayersDouble].max_measure = servo_pp_limit.param_max;
         adjust_servo_limits[ADJUST_ITEM_RAD_TILT][eLayersDouble].min_optimum = servo_pp_limit.param_min;
         adjust_servo_limits[ADJUST_ITEM_RAD_TILT][eLayersDouble].max_optimum = servo_pp_limit.param_max;

      }
      p_width = x[k_max_flank[1]] - x[k_max_flank[0]];
      adjust_state_change(ADJUST_S_P_INIT_RESTORE);
   }
   else
   {
      // curve-fitting NOK
      data.retry++;

      if (data.retry < coef.retry_max)
      {
         LONG offset;
         offset = (coef.p[0].max - coef.p[0].min)/16;

         // try again
         if (data.retry == 1)
         {
            data.p_start_actual[0] = data.p_start_first[0] - offset;
         }
         else
         {
            if (data.retry == 2)
            {
               data.p_start_actual[0] = data.p_start_first[0] + offset;
            }
            else
            {
               data.p_start_actual[0] = data.p_start_first[0];
            }
         }
         if (data.p_start_actual[0] < coef.p[0].min)
         {
            data.p_start_actual[0] = coef.p[0].min;
         }
         if (data.p_start_actual[0] > coef.p[0].max)
         {
            data.p_start_actual[0] = coef.p[0].max;
         }

         //DEBUG_system_dma_printf (("RETRY adjust %d(max=%d) start=%d", data.retry, coef.retry_max,data.p_start_actual[0]));
         adjust_state_change(ADJUST_S_P_INIT);
      }
      else
      {
         // no retries left: quit the loop
         //DEBUG_system_dma_printf((DBG_ADJUST"ERROR no retries left %d>=%d", data.retry, coef.retry_max, data.p_start_actual[0]));
         //ERROR_set_player_error(ERROR_CALIBRATION_FAILED_ERROR);
         status = ERROR;
         adjust_state_change(ADJUST_S_P_INIT_RESTORE);
      }
   }
#endif
}
/*-----------------------------------------------------------------------*/

static void adjust_process_1d_results_optimal(void)
{
#if TO_BE_PORTED
   SHORT             opt_target, opt_index;
   USHORT            temp_min_delta_index;
   LONG              x [STORAGE_MAX];
   LONG              y [STORAGE_MAX];
   BYTE             opt_point;
   lib_curve_fit_t   curve;
   float             optimum;
   LONG              worst_index;
   BYTE             loop, nr_of_valid_points;

   //DEBUG_printf (("P-coefs delay=%d,%d hi/lo=0x%x/0x%x max=0x%x/0x%x rel=0x%x min=%d\nC-coefs step=0x%x meas=0x%x",
      coef.p[0].delay_small,coef.p[0].delay_big, coef.p[0].step_hi, coef.p[0].step_lo, coef.p[0].min , coef.p[0].max, coef.p[0].max_rel, coef.i_min,
      coef.clim_step, coef.clim_meas));

   for (loop = 0; loop < (BYTE)(data.i + 1); loop++)
   {
      x[loop] = (LONG)data.buf[0][loop].p[0];
      y[loop] = (LONG)data.buf[0][loop].c;
   }
   nr_of_valid_points = (BYTE)data.i +1;

   opt_point = LIB_curve_fit_data_filter1( x, y, &nr_of_valid_points, (LONG)LONG_MAX-1, (LONG)0, coef.curve_up);
   //(algorithm) calculate optimize_min_delta_index according to opt_index
   if (data.i == 0)
   {
      opt_target = 0;
      opt_index = (coef.a1_min >0)? (SHORT)32767:(SHORT)-32768;
   }
   else
   {
      opt_target = (SHORT)x[opt_point];
      opt_index = (SHORT)y[opt_point];
   }
   temp_min_delta_index = coef.delta_relative? (USHORT)(ABS(opt_index) * coef.clim_rel_min / 100) : (USHORT)coef.clim_rel_min;
   user_result.ok = FALSE;

   if (data.ramp_mode == RAMPING_FAIL)
   {
      //DEBUG_system_dma_printf((DBG_ADJUST"WARNING npoints #%d<%d", nr_of_valid_points, coef.i_min));
   }
   else
   {
      // do the curve-fitting
     // if (curr.item[0] != ADJUST_ITEM_SPHER_ABER) //2006 1109 SC Chen remove SA
      //{
         if (data.i < 1)//possible measure error(ex:no HF)
         {
            worst_index = 0;
         }
         else
         {
            if (coef.curve_up)
            {
               worst_index = MIN(y[0],y[nr_of_valid_points - 1]);
               if (worst_index < (LONG) (opt_index + temp_min_delta_index))
               {
                  worst_index = opt_index + temp_min_delta_index;
               }
               if (worst_index > (LONG)(coef.clim_meas + temp_min_delta_index * 1.2))
               {//(algorithm) Factory yield rate improvement
                  worst_index = (LONG)(coef.clim_meas + temp_min_delta_index * 1.2);
               }
            }
            else
            {
               worst_index = MAX(y[0],y[nr_of_valid_points - 1]);
            }
         }
         (void)LIB_curve_fit_data_filter2( x, y, &nr_of_valid_points, worst_index, coef.curve_up);
     // }
#ifdef DEBUG_ENABLE
      //DEBUG_system_dma_printf(("%s   %s", ADJUST_ITEM_to_string[curr.item[0]], adjust_cost_to_string[curr.cost]));
      for (loop = 0; loop < nr_of_valid_points; loop++)
      {
         //DEBUG_system_dma_printf(("%6d  %6d", x[loop],y[loop]));
      }
      //DEBUG_system_dma_printf(("=================="));
#endif
      if (nr_of_valid_points < (BYTE) coef.i_min)
      {
         //DEBUG_system_dma_printf(("nr_of_valid_points = %d < %d",nr_of_valid_points,coef.i_min));
         coef.p[0].step_hi -= (coef.p[0].step_hi / coef.i_min);
         coef.p[0].step_lo -= (coef.p[0].step_lo / coef.i_min);
      }
      else
      {
         LIB_curve_fit (x, y, nr_of_valid_points, &curve);
         //DEBUG_system_dma_printf((DBG_ADJUST"ADJUST interp: c2=%.3e c1=%.3e c0=%.2f", curve.coefa, curve.coefb, curve.coefc));

         if (curve.coefa == 0.0)
         {
            //DEBUG_system_dma_printf((DBG_ADJUST"WARNING interp.failed a1 = 0"));
         }
         else
         {
            // check that the curve-fit results are within bounds
            optimum = -curve.coefb / (2.0 * curve.coefa);
            user_result.item_optimum[0] = (LONG) optimum;
            if( (curve.coefa < coef.a1_min) ||
                (curve.coefa > coef.a1_max) )
            {
               //DEBUG_system_dma_printf((DBG_ADJUST"WARNING interp.result a1 out-of-range (a1=%d)",curve.coefa));
            }
            else
            {
               if ((user_result.item_optimum[0] <  coef.p[0].result_min) ||
                   (user_result.item_optimum[0]  > coef.p[0].result_max) ||
                   (curve.coefb < coef.a2_min) ||
                   (curve.coefb > coef.a2_max)
                  )
               {
                  //DEBUG_system_dma_printf((DBG_ADJUST"WARNING interp.result c2 or range p nok:0x%x/0x%x/0x%x c2:%e/%e/%e c1:%e/%e/%e",
                  //      user_result.item_optimum[0], coef.p[0].result_min, coef.p[0].result_max,
                  //      curve.coefa, coef.a1_min, coef.a1_max,
                  //      curve.coefb, coef.a2_min, coef.a2_max));

               }
               else
               {
                  if (((user_result.item_optimum[0] > x[nr_of_valid_points-1]) || ((user_result.item_optimum[0] < x[0]))))
                  { //clipping
                     //DEBUG_system_dma_printf((DBG_ADJUST"WARNING opt=0x%x out-of-range",user_result.item_optimum[0]));
                     if(data.retry == (coef.retry_max -1))
                     {
                        user_result.item_optimum[0] = opt_target;
                        user_result.cost_optimum = (LONG)((curve.coefa * user_result.item_optimum[0] * user_result.item_optimum[0]) +
                                                       (curve.coefb * user_result.item_optimum[0]) +
                                                        curve.coefc);
                        user_result.ok = TRUE;
                        //DEBUG_system_dma_printf((DBG_ADJUST"move the optimal value to minimum jitter"))
                     }
                  }
                  else
                  {//NORMAL
                     //Determine the cost optimum
                     user_result.cost_optimum = (LONG)((curve.coefa * user_result.item_optimum[0] * user_result.item_optimum[0]) +
                                                       (curve.coefb * user_result.item_optimum[0]) +
                                                        curve.coefc);
                     user_result.ok = TRUE;
                  }
               }
            }
         }
      }
   }

   p_width = 0;
   if (user_result.ok)
   {
      (void)LIB_curve_fit_data_filter1( x, y, &nr_of_valid_points, (LONG)coef.clim_val_max, (LONG)coef.clim_val_min, coef.curve_up);
      p_width = x[nr_of_valid_points - 1] - x[0];
      adjust_state_change(ADJUST_S_P_INIT_RESTORE);
   }
   else
   {
      // curve-fitting NOK
      data.retry++;

      if (data.retry < coef.retry_max)
      {
         LONG offset;
         offset = (coef.p[0].max - coef.p[0].min)/16;

         // try again
         if (data.retry == 1)
         {
            data.p_start_actual[0] = data.p_start_first[0] - offset;
         }
         else
         {
            if (data.retry == 2)
            {
               data.p_start_actual[0] = + offset;
            }
            else
            {
               data.p_start_actual[0] = opt_target;
            }
         }
         if (data.p_start_actual[0] < coef.p[0].min)
         {
            data.p_start_actual[0] = coef.p[0].min;
         }
         if (data.p_start_actual[0] > coef.p[0].max)
         {
            data.p_start_actual[0] = coef.p[0].max;
         }

         //DEBUG_system_dma_printf (("RETRY adjust %d(max=%d) start=%d", data.retry, coef.retry_max,data.p_start_actual[0]));
         adjust_state_change(ADJUST_S_P_INIT);
      }
      else
      {
         // no retries left: quit the loop
         //DEBUG_system_dma_printf((DBG_ADJUST"ERROR no retries left %d>=%d", data.retry, coef.retry_max, data.p_start_actual[0]));
         //ERROR_set_player_error(ERROR_CALIBRATION_FAILED_ERROR);
         status = ERROR;
         adjust_state_change(ADJUST_S_P_INIT_RESTORE);
      }
   }
#endif
}
/*-----------------------------------------------------------------------*/

void adjust_calculate_servo_limits(ADJUST_SERVO_LIMIT_MODE_t  mode,
                                   Math2d_ResultT             *result_2d_calc,
                                   Math2d_ExperimentT         *sl_experiment_plan_2d )
{
   SHORT                           boundary_item0 = 1 ,
                                   boundary_item1 = 1 ; //init @ full range (1% = almost full curve)
   Math2d_ServoLimitsT             servo_limit;
   int j;

   //store results
   servo_limit.b[0]       = result_2d_calc->b[0];
   servo_limit.b[1]       = result_2d_calc->b[1];
   servo_limit.b[2]       = result_2d_calc->b[2];
   servo_limit.b[3]       = result_2d_calc->b[3];
   servo_limit.b[4]       = result_2d_calc->b[4];
   servo_limit.b[5]       = result_2d_calc->b[5];
   servo_limit.ItemOpt[0] = result_2d_calc->Optimum[0];
   servo_limit.ItemOpt[1] = result_2d_calc->Optimum[1];
   servo_limit.Fac[0]     = result_2d_calc->Fac[0];
   servo_limit.Fac[1]     = result_2d_calc->Fac[1];

   if ( (curr.cost == ADJUST_COST_WOBBLE_AMPL)   ||
        (curr.cost == ADJUST_COST_PUSHPULL_AMPL) )
   {
      if (mode == SERVO_LIMIT_MEASURE)
      {
         if (curr.cost == ADJUST_COST_WOBBLE_AMPL)
         {
            boundary_item0 = ADJUST_2D_SERVO_MEASURE_LIMIT_FOO_WOBBLE;
            boundary_item1 = ADJUST_2D_SERVO_MEASURE_LIMIT_SPA_WOBBLE;
         }
         else //(curr.cost == ADJUST_COST_PUSHPULL_AMPL)
         {
            boundary_item0 = ADJUST_2D_SERVO_MEASURE_LIMIT_FOO_PUSH_PULL;
            boundary_item1 = ADJUST_2D_SERVO_MEASURE_LIMIT_SPA_PUSH_PULL;
         }
      }
      else //SERVO_LIMIT_OPTIMUM
      {
         if (curr.cost == ADJUST_COST_WOBBLE_AMPL)
         {
            boundary_item0 = ADJUST_2D_SERVO_OPTIMUM_LIMIT_FOO_WOBBLE;
            boundary_item1 = ADJUST_2D_SERVO_OPTIMUM_LIMIT_SPA_WOBBLE;
         }
         else //(curr.cost == ADJUST_COST_PUSHPULL_AMPL)
         {
            boundary_item0 = ADJUST_2D_SERVO_OPTIMUM_LIMIT_FOO_PUSH_PULL;
            boundary_item1 = ADJUST_2D_SERVO_OPTIMUM_LIMIT_SPA_PUSH_PULL;
         }
      }
      //DEBUG_system_dma_printf(("servo_limit_layer mode(%d) for layer %d -> %d / %d",mode,servo_limit_layer,boundary_item0,boundary_item1));
      MATH2D_GetServoLimits(sl_experiment_plan_2d,  &servo_limit, boundary_item0, boundary_item1);

      for (j = 0; j< MAX_NO_ITEMS;j++)
      {
         adjust_servo_limits[curr.item[j]][servo_limit_layer].determined = TRUE ;
         // make sure we do not cross HW boundaries :
         if (curr.item[j] == ADJUST_ITEM_SPHER_ABER)
         {
            if (servo_limit.Limit[j].param_min < (float)((SHORT)COEF_GetValueParams(curr.DiscKind,curr.speed, SRV_CALC_P_MIN_SPA)))
                servo_limit.Limit[j].param_min =  (float)((SHORT)COEF_GetValueParams(curr.DiscKind,curr.speed, SRV_CALC_P_MIN_SPA));
            if (servo_limit.Limit[j].param_max > (float)((SHORT)COEF_GetValueParams(curr.DiscKind,curr.speed, SRV_CALC_P_MAX_SPA)))
                servo_limit.Limit[j].param_max =  (float)((SHORT)COEF_GetValueParams(curr.DiscKind,curr.speed, SRV_CALC_P_MAX_SPA));
         }
         else if (curr.item[j] == ADJUST_ITEM_FOCUS_OFFSET)
         {
            if (optimal_algorithm)
            {
               if (servo_limit.Limit[j].param_min < (-(float)((SHORT)0x2000)))
                   servo_limit.Limit[j].param_min =  (-(float)((SHORT)0x2000));
               if (servo_limit.Limit[j].param_max  >  (float)((SHORT)0x2000))
                   servo_limit.Limit[j].param_max   =  (float)((SHORT)0x2000);
            }
            else
            {
            if (servo_limit.Limit[j].param_min < (-(float)((SHORT)COEF_GetValueParams(curr.DiscKind,curr.speed, SRV_CALC_P_MAX_FOO))))
                servo_limit.Limit[j].param_min =  (-(float)((SHORT)COEF_GetValueParams(curr.DiscKind,curr.speed, SRV_CALC_P_MAX_FOO)));
            if (servo_limit.Limit[j].param_max  >  (float)((SHORT)COEF_GetValueParams(curr.DiscKind,curr.speed, SRV_CALC_P_MAX_FOO)))
                servo_limit.Limit[j].param_max   =  (float)((SHORT)COEF_GetValueParams(curr.DiscKind,curr.speed, SRV_CALC_P_MAX_FOO));
            }
         }

         //copy in local DB
         if (mode == SERVO_LIMIT_MEASURE)
         {
            adjust_servo_limits[curr.item[j]][servo_limit_layer].min_measure = servo_limit.Limit[j].param_min;
            adjust_servo_limits[curr.item[j]][servo_limit_layer].max_measure = servo_limit.Limit[j].param_max;
         }
         else //SERVO_LIMIT_OPTIMUM
         {
            adjust_servo_limits[curr.item[j]][servo_limit_layer].min_optimum = servo_limit.Limit[j].param_min;
            adjust_servo_limits[curr.item[j]][servo_limit_layer].max_optimum = servo_limit.Limit[j].param_max;
         }
      }
   }
}

static void adjust_process_2d_results(void)
{
   Math2d_MeasQualityDefT     q_def;
   Math2d_ResultT             result_2d_calc;
   Math2d_ExperimentResultT   experiment_result[NR_OF_2D_EXPERIMENTS];
   LONG                       cnt;
   BOOL                       optimum_detected;

   for (cnt = 0 ; cnt < NR_OF_2D_EXPERIMENTS; cnt ++)
   {
      experiment_result[cnt].Cost= -1;
      experiment_result[cnt].Valid = FALSE;
   }
   if (curr.cost == ADJUST_COST_WOBBLE_AMPL)
   {
      // units amplitute
      q_def.MinDiff         = 10;
      q_def.MaxDiff         = 30;
      q_def.MinDiffHV       = 15;
      q_def.MinDiffPQ       = 15;
      q_def.MaxDiffPQ       = 3;
      q_def.DiffValidSearch = 60;
   }
   else if (curr.cost == ADJUST_COST_PUSHPULL_AMPL)
   {
      // units amplitute
      q_def.MinDiff         = 1000;
      q_def.MaxDiff         = 3000;
      q_def.MinDiffHV       = 1500;
      q_def.MinDiffPQ       = 1500;
      q_def.MaxDiffPQ       = 300;
      q_def.DiffValidSearch = 6000;
   }
   else
   {   // units jitter in 0.1 %
      q_def.MinDiff          = 30;   /*0*/
      q_def.MaxDiff          = 100; /*255*/
      q_def.MinDiffHV       = 50;
      q_def.MinDiffPQ       = 50;
      q_def.MaxDiffPQ       = 10;
      q_def.DiffValidSearch = 60;
   }

   for(cnt = 0; cnt < NR_OF_2D_EXPERIMENTS; cnt++)
   {
      if ( ((data.buf[0][cnt].c != MAX_LONG) && (data.buf[0][cnt].c != MIN_LONG))  // valid measurement
           && (  !(data.buf[0][cnt].hf_recoveries > 0)  // hf (pll lock ok)
              || !(data.buf[0][cnt].servo_recoveries > 0 )))           // servo ok during measurement
      {
         experiment_result[cnt].Cost   = (SHORT)(data.buf[0][cnt].c);
         experiment_result[cnt].Valid  = TRUE;
         //DEBUG_printf(("%d  -->  %d",data.buf[0][cnt].c,experiment_result[cnt].valid));
      }
      else
      {
         experiment_result[cnt].Valid  = FALSE;
         //DEBUG_printf(("%d  -->  %d",data.buf[0][cnt].c,experiment_result[cnt].valid));
      }
   }
   optimum_detected           = MATH2D_GetOptimum( &result_2d_calc , experiment_result , &q_def );

   if ( !optimum_detected )
   {
      if ( result_2d_calc.IterationRequired )
      {
         // The parameters changed, so we can do another iteration (in the same STEP)
         //DEBUG_system_dma_printf(("ADJUST2D: STEP_RESULT_ITERATION_REQUIRED"));
         user_result.ok = FALSE;
         adjust_state_change(ADJUST_S_P_INIT);
      }
      else
      {
         // The parameters should have been changed, but didn't, we reached a dead end in the algorithm
         //DEBUG_system_dma_printf(("ADJUST2D: BOUNDARY_REACHED"));
         user_result.ok = FALSE;
         status = ERROR;
         //ERROR_set_player_error(ERROR_FOCUS_OFFSET_OPTIMIZATION);
         adjust_state_change(ADJUST_S_P_INIT_RESTORE);
      }
   }
   else
   {
      // set limits for measure range for further 1D calibrations
      adjust_calculate_servo_limits( SERVO_LIMIT_MEASURE,&result_2d_calc,experiment_plan_2d );
      // set limits for optimum of further 1D calibrations
      adjust_calculate_servo_limits( SERVO_LIMIT_OPTIMUM,&result_2d_calc,experiment_plan_2d );

      user_result.item_optimum[0] = result_2d_calc.Optimum[0];
      user_result.item_optimum[1] = result_2d_calc.Optimum[1];
      user_result.cost_optimum    = result_2d_calc.MinCost;
      user_result.ok              = optimum_detected;
      status                      = READY;

      adjust_state_change(ADJUST_S_P_INIT_RESTORE);
   }
}

/*-----------------------------------------------------------------------*/

static void adjust_process_itemscan_results(void)
{
   LONG cnt, res;

   if ((curr.cost == ADJUST_COST_STREAM_SAMPLED_JITTER_ITEMSCAN) ||
       (curr.cost == ADJUST_COST_ARM_SAMPLED_JITTER_ITEMSCAN))
   {
      if(!VALIDITY_CHECK(0))
      {
         status = ERROR;
      }
      else
      {
         LONG min_delta;

         user_result.item_optimum[0] = data.buf[0][0].p[0];
         user_result.cost_optimum    = data.buf[0][0].c;
         res = 0;

         /**************/
         if( ((curr.DiscKind.fDiscKind == eBDRDLDisc) || (curr.DiscKind.fDiscKind == eBDREDLDisc))&&(curr.item[0] == ADJUST_ITEM_SPHER_ABER)&& (VALIDITY_CHECK(1)&&VALIDITY_CHECK(2)))
         {
            min_delta = 3;
         }
         /**************/
         else
         {
            min_delta = 5;
         }

         for(cnt = 1; cnt < 3; cnt++)
         {
            if(((user_result.cost_optimum) > (data.buf[0][cnt].c + min_delta)) && VALIDITY_CHECK(cnt))//there must be a delta of 0,3/0,5% jitter
            {
               user_result.item_optimum[0] = data.buf[0][cnt].p[0];
               user_result.cost_optimum    = data.buf[0][cnt].c;
               res = cnt;
            }
         }

         //DEBUG_system_dma_printf(("itemscan result: 2)%d\t0)%d\t1)%d\tres= index %d item,cost: 0x%x,%d",
         //               data.buf[0][2].c,data.buf[0][0].c,data.buf[0][1].c,
         //               res,user_result.item_optimum[0],user_result.cost_optimum));
         user_result.ok = TRUE;
      }
   }
   else
   {//wobble and pushpull
      if(!VALIDITY_CHECK(0))
      {
         status = ERROR;
      }
      else
      {
         user_result.item_optimum[0] = data.buf[0][0].p[0];
         user_result.cost_optimum    = data.buf[0][0].c;
         for(cnt = 1; cnt < 3; cnt++)
         {
            if(((user_result.cost_optimum*105/100) < data.buf[0][cnt].c) && VALIDITY_CHECK(cnt))
            {
               user_result.item_optimum[0] = data.buf[0][cnt].p[0];
               user_result.cost_optimum    = data.buf[0][cnt].c;
            }
         }
         user_result.ok = TRUE;
      }
   }
   adjust_state_change(ADJUST_S_P_INIT_RESTORE);
}
///*-----------------------------------------------------------------------*/
//
//static void
//adjust_process_hockeystick_results(void)
//{
//   LONG cnt, res;
//
//   for(cnt = 1; cnt < 3; cnt++)
//   {
//      if(!VALIDITY_CHECK(cnt))
//      {
//         status = ERROR;
//      }
//   }
//   if(status != ERROR)
//   {
//      //if you have a hockeystick, you try to go to the extreme
//      //   o                            o         o      o
//      //    \                          /           \    /
//      //     \                        /             \  /
//      //      o------x    or  x------o       or      x
//      //we always take x (difference need to be at least 1%
//
//      if((data.buf[0][2].c > (data.buf[0][0].c + 10)) && (data.buf[0][1].c > (data.buf[0][0].c + 10)))
//      {
//         res = 0;
//      }
//      else if((data.buf[0][2].c > (data.buf[0][0].c + 10)) && (data.buf[0][1].c <= (data.buf[0][0].c + 10)))
//      {
//         res = 1;
//      }
//      else if((data.buf[0][2].c < (data.buf[0][0].c + 10)) && (data.buf[0][1].c > (data.buf[0][0].c + 10)))
//      {
//         res = 2;
//      }
//      else
//      {
//         res = 1;
//      }
//      user_result.item_optimum[0] = data.buf[0][res].p[0];
//      user_result.cost_optimum    = data.buf[0][res].c;
//      //DEBUG_printf(("hockeystick result: 2)%d\t0)%d\t1)%d\tres= index %d item,cost: 0x%x,%d",
//                        data.buf[0][2].c,data.buf[0][0].c,data.buf[0][1].c,
//                        res,user_result.item_optimum[0],user_result.cost_optimum));
//      user_result.ok = TRUE;
//   }
//   adjust_state_change(ADJUST_S_P_INIT_RESTORE);
//}
/*--------------------------------------------------------------------------*/
static EStatus ss_conditional_open_loop(void)
{
   static BYTE delay;
   EStatus ss_status = BUSY;

   switch (ss_state)
   {
      case ADJUST_SS_START :
         // front
         switch (curr.cost)
         {
            case ADJUST_COST_PUSHPULL_AMPL:
            case ADJUST_COST_WOBBLE_AMPL:
            case ADJUST_COST_PUSHPULL_AMPL_ITEMSCAN:
            case ADJUST_COST_WOBBLE_AMPL_ITEMSCAN:
#if TO_BE_PORTED
               SERVO_radial_open_loop(adjust_callback_func,SRC_OFF);
#endif
               ss_state = ADJUST_SS_WAIT_SERVO;
               delay = (BYTE)(COEF_GetValueParams(curr.DiscKind,curr.speed, SRV_CALC_P_DELAY_OPEN_LOOP_SPA_WOBBLE));
                                             //it is defined as being specific for wobble and SA but it is always needed
                                             //when going openloop, the only one for which we do this now is SA vs wobble
               break;
            case ADJUST_COST_STREAM_SAMPLED_JITTER:
            case ADJUST_COST_ARM_SAMPLED_JITTER:
            case ADJUST_COST_STREAM_SAMPLED_JITTER_ITEMSCAN:
            case ADJUST_COST_ARM_SAMPLED_JITTER_ITEMSCAN:
            case ADJUST_COST_ABORT:
            case ADJUST_COST_NONE:
            case ADJUST_COST_LAST:
            default:
               ss_state = ADJUST_SS_WAIT_DELAY;
               delay = 0;
               break;
         }
         //KRNL_attach (TIMEBASE_0_MS, ADJUST_TASKID);
         break;

      case ADJUST_SS_WAIT_SERVO :
#if TO_BE_PORTED
          ss_status = SERVO_get_status();
#endif
         switch (ss_status)
         {
            case ERROR :
               DEBUG_ASSERT(FALSE,eASSERT_FUNCTION_NOT_IMPLEMENTED);//DEBUG_assert(FALSE);
               break;
            case READY :
               ss_state = ADJUST_SS_WAIT_DELAY;
               if((curr.cost == ADJUST_COST_WOBBLE_AMPL) || (curr.cost == ADJUST_COST_WOBBLE_AMPL_ITEMSCAN))
               {
                  adjust_wob_amp_setting(WOB_PP_PREPARE);
               }
               adjust_set_settle_timer(delay);
               break;
            case BUSY  :
               break;
            default:
               break;
         }
         break;
      case ADJUST_SS_WAIT_DELAY :
         if ((timeout_chk(2) == eTimerTimeout) || delay == 0) // if (TIMER_is_expired (TIMER_ID_1_MS_ADJUST) || delay == 0)
         {
            ss_state = ADJUST_SS_START;
            //KRNL_attach (TIMEBASE_0_MS, ADJUST_TASKID);
            ss_status = READY;
         }
         break;
      default:
         break;
   }
   return ss_status;
}

static EStatus ss_conditional_close_loop(void)
{
   EStatus ss_status = BUSY;

   switch (ss_state)
   {
      case ADJUST_SS_START :
         // restore front (including what have been set during cost-init!)
         switch (curr.cost)
         {
            case ADJUST_COST_WOBBLE_AMPL:
            case ADJUST_COST_WOBBLE_AMPL_ITEMSCAN:
               adjust_wob_amp_setting(WOB_PP_RESTORE);
               //lint -fallthrough
            case ADJUST_COST_PUSHPULL_AMPL:
            case ADJUST_COST_PUSHPULL_AMPL_ITEMSCAN:
#if TO_BE_PORTED
               SERVO_radial_closed_loop(adjust_callback_func);
#endif
               ss_state = ADJUST_SS_WAIT_SERVO;
               break;
            case ADJUST_COST_STREAM_SAMPLED_JITTER:
            case ADJUST_COST_ARM_SAMPLED_JITTER:
            case ADJUST_COST_STREAM_SAMPLED_JITTER_ITEMSCAN:
            case ADJUST_COST_ARM_SAMPLED_JITTER_ITEMSCAN:
            case ADJUST_COST_ABORT:
            case ADJUST_COST_NONE:
            case ADJUST_COST_LAST:
            default:
               ss_status = READY;
               break;
         }
         break;
      case ADJUST_SS_WAIT_SERVO :
#if TO_BE_PORTED
         ss_status = SERVO_get_status();
#endif
         switch (ss_status)
         {
            case ERROR :
            case READY :
               ss_state = ADJUST_SS_START;
               break;
            case BUSY  :
               break;
            default:
               break;
         }
         break;
      case ADJUST_SS_WAIT_DELAY:
      default:
         break;
   }
   return ss_status;
}
/*--------------------------------------------------------------------------*/
/* s_xxx(): handling per state                                              */
/*--------------------------------------------------------------------------*/

static void s_idle (void)
{
   if (new.mode != ADJUST_MODE_IDLE)
   {
      BOOL parameters_ok;
      // A new item, start.
      //DEBUG_printf (("idle-NEW"));
      //SERVODBG_pin_set (SERVODBG_PIN_15);

      curr = new;
      data.retry           = 0;
      data.p_start_valid   = FALSE;
      initiate_reseek      = FALSE;

      if ((curr.DiscKind.fDiscKind == eDVDPlusRDLDisc) && (curr.mode == ADJUST_MODE_1D) && (servo_limit_layer == eLayersSingle)
         && (curr.item[0] != ADJUST_ITEM_SPHER_ABER))
      {
         //DEBUG_system_dma_printf((DBG_ADJUST"ADJUST use optimal algorithm"));
         optimal_algorithm = TRUE;
      }
      adjust_clear_session_info (&new);
      parameters_ok = adjust_idle_parse_parameters();
      // set in correct address locking mode, for OPC (fine seek) we allways use wobble lock
      // lock mode should be correctly reset by next command
#if TO_BE_PORTED
      SUBC_get_address_mode(&orig_lock_mode, &orig_pll_kick);
      SUBC_set_address_mode(EFM_WOBBLE_MODE, TRUE);
#endif
#if TO_BE_PORTED
      if (parameters_ok && !SERVO_is_stopped())
#else
      if (parameters_ok)
#endif
      {
         // This is the only path to start a new session...
         SetTimeout(0, MS_100*TIMEOUT_WATCHDOG); //TIMER_start_timer (TIMER_ID_100_MS_ADJUST_WATCHDOG, TIMEOUT_WATCHDOG, adjust_callback_func);
         status = BUSY;
         adjust_state_change(ADJUST_S_INITIAL_SEEK);
      }
      else
      {
         // Invalid idle parameters, issue error.
         adjust_state_change(ADJUST_S_FINAL);
         if (!parameters_ok)
         {
            DEBUG_ASSERT(FALSE,eASSERT_FUNCTION_NOT_IMPLEMENTED);//ERROR_set_player_error( ERROR_ILLEGAL_PARAMETER );
         }
         else
         {
            send_msg80(1,0x710075);//DEBUG_system_dma_printf((DBG_ADJUST"WARNING servo is stopped (adjust)"));
         }
         status = ERROR;
      }
   }
   else
   {
      // No item is selected, stay in idle.
      send_msg80(1,0x710076);//DEBUG_printf (("adj:idle"));
   }
}

/*--------------------------------------------------------------------------*/

static void s_initial_seek(void)
{
   switch (sub_state.s_initial_seek)
   {
      case ADJUST_S_INITIAL_SEEK_SUBST_START:
         if((curr.config & ADJ_CONFIG_SEEK_MODE_MASK) != ADJ_CONFIG_NO_SEEK)
         {
#if TO_BE_PORTED
            SEEK_seek_address (curr.start, adjust_callback_func);
#endif
            //SERVODBG_pin_set (SERVODBG_PIN_2);
            sub_state.s_initial_seek = ADJUST_S_INITIAL_SEEK_SUBST_WAIT; // activation by SEEK's callback
         }
         else
         {
            sub_state.s_initial_seek = ADJUST_S_INITIAL_SEEK_SUBST_FINAL;
            //KRNL_attach (TIMEBASE_0_MS, ADJUST_TASKID);
         }
         break;
      case ADJUST_S_INITIAL_SEEK_SUBST_WAIT:
#if TO_BE_PORTED
         switch (SEEK_get_status())
         {
            case READY:
               sub_state.s_initial_seek = ADJUST_S_INITIAL_SEEK_SUBST_FINAL;
               //KRNL_attach (TIMEBASE_0_MS, ADJUST_TASKID);
               break;

            case BUSY:
               // activated by another cause...
               break;

            case ERROR:
            default:
               //DEBUG_system_dma_printf ((DBG_ADJUST"ADJ:seek NOK"));
               status = ERROR;
               adjust_state_change(ADJUST_S_P_INIT_RESTORE);
               break;
         }
#endif
         break;
      case ADJUST_S_INITIAL_SEEK_SUBST_FINAL:
         //SERVODBG_pin_clear (SERVODBG_PIN_2);
         adjust_state_change(ADJUST_S_FRONT_BACK_INIT);
         break;
      default:
         break;
   }
}

/*--------------------------------------------------------------------*/
static void s_front_back_init (void)
{
   LONG item_number;

   switch (sub_state.s_front_back_init)
   {
      case ADJUST_S_FRONT_BACK_INIT_SUBST_START :
         // back
         for(item_number=0; item_number < curr.no_of_items; item_number++)
         {
            switch (curr.item[item_number])
            {
               case ADJUST_ITEM_RAD_TILT:
#if TO_BE_PORTED
                  data.p_start_first[item_number] = TILT_get_tilt ();
                  TILT_limit_tilt (TILT_MEASURE_LIMIT);
#endif
                  break;
               case ADJUST_ITEM_SPHER_ABER:
#if TO_BE_PORTED
                  data.p_start_first[item_number] = COLLI_get_pos();
#endif
                  break;
               case ADJUST_ITEM_TANG_TILT:
#if TO_BE_PORTED
                  data.p_start_first[item_number] = TANGTILT_crank_to_frame(TANGTILT_get_pos());
#endif
                  break;
               case ADJUST_ITEM_FOCUS_OFFSET:
                  data.p_start_first[item_number] = FOCUS_GetOffset();
                  break;
               case ADJUST_ITEM_RAD_OFFSET:
#if TO_BE_PORTED
                  init_rad_offset_perc = RADIAL_get_radial_offset_percent();
                  data.p_start_first[item_number] = 0;
#endif
                  break;
               case ADJUST_ITEM_SSPP_GAIN:
               case ADJUST_ITEM_RE_GAIN_OFFSET:
               case ADJUST_ITEM_WOPC:
               case ADJUST_ITEM_FOC_INT:
               case ADJUST_ITEM_RE:
               case ADJUST_ITEM_REFLECTION:
               case ADJUST_ITEM_FRD_REF:
               case ADJUST_ITEM_ERASE_REFLECTION:
               case ADJUST_ITEM_DUTYCYCLE_5_12V:
               case ADJUST_ITEM_LASER_TEMP_ACT_DISP:
               case ADJUST_ITEM_LASER_TRESHOLD_CURRENT:
               case ADJUST_ITEM_ABORT:
               case ADJUST_ITEM_NONE:
               case ADJUST_ITEM_LAST:
               default:
                  break;
            }
            data.p_start_actual[item_number] = data.p_start_first[item_number];
            // for 2D calib, we overwrite the start values by the first point in the measurement plan.
            // this can be way out of boundaries,se wee keep an extra set to restore.
         }
         data.p_start_valid = TRUE;
         sub_state.s_front_back_init = ADJUST_S_FRONT_BACK_INIT_SUBST_COND_OPEN_LOOP;
         //lint -fallthrough
      case ADJUST_S_FRONT_BACK_INIT_SUBST_COND_OPEN_LOOP :
         switch (ss_conditional_open_loop())
         {
            case ERROR :
               DEBUG_ASSERT(FALSE,eASSERT_FUNCTION_NOT_IMPLEMENTED);//DEBUG_assert(FALSE);
               break;
            case READY :
               sub_state.s_front_back_init = ADJUST_S_FRONT_BACK_INIT_SUBST_FINAL;
               //KRNL_attach (TIMEBASE_0_MS, ADJUST_TASKID);
               break;
            case BUSY  :
               break;
            default:
               break;
         }
         break;
      case ADJUST_S_FRONT_BACK_INIT_SUBST_FINAL :
         //DEBUG_printf (("p_start_actual=0x%x,0x%x",
         //                  data.p_start_actual[0],data.p_start_actual[1]));
         //KRNL_subscribe (OSAL_EVENT_BE_HFPLL_LOCK, ADJUST_TASKID);
         switch (curr.mode)
         {
            case ADJUST_MODE_2D:
               adjust_init_2d_measurements();
               break;
            case ADJUST_MODE_ITEMSCAN:
            case ADJUST_MODE_1D:
            case ADJUST_MODE_IDLE:
            case ADJUST_MODE_LAST:
            default:
               break;
         }
         adjust_state_change(ADJUST_S_P_INIT);
         break;
      default:
         break;
   }
}

/*--------------------------------------------------------------------------*/

static void s_p_init (void)
{
   int   i;
   int   j;
   LONG item_number;

   //DEBUG_printf (("p-init"));

   data.i      = -1;
   data.flank  = 0;
   data.ramp_mode  = RAMPING_UP;
   for (i = 0; i < FLANK_MAX; i++)
   {
      for (j = 0; j < STORAGE_MAX; j++)
      {
         data.buf[i][j].c = MAX_LONG;
         for(item_number=0; item_number < MAX_NO_ITEMS; item_number++)
         {
            data.buf[i][j].p[item_number] = MAX_LONG;
         }
         data.buf[i][j].hf_recoveries     = 0;
         data.buf[i][j].servo_recoveries  = 0;
      }
   }
   switch (curr.mode)
   {
      case ADJUST_MODE_2D:
         adjust_prepare_2d_measurements();
         break;
      case ADJUST_MODE_ITEMSCAN:
         adjust_prepare_itemscan_measurements();
         break;
      case ADJUST_MODE_1D:
      case ADJUST_MODE_IDLE:
      case ADJUST_MODE_LAST:
      default:
         break;
   }
   for(item_number=0; item_number < MAX_NO_ITEMS; item_number++)
   {
      user_result.item_optimum[item_number] = 0;
   }
   user_result.cost_optimum = 0;
   adjust_state_change(ADJUST_S_P_NEXT);
}

/*--------------------------------------------------------------------------*/

static void s_p_next (void)
{
   ////DEBUG_printf (("n"));
   //SERVODBG_pin_toggle (SERVODBG_PIN_1);
   if (adjust_p_next_determine ())
   {
      // there is a next point to examine
      // input data.p for S_P_ACTION_READY has been set in function adjust_p_next_determine!
      if (FOCUS_OFFSET_PRESET_NEEDED())
      {
         //this is normally done in s_p_set, but for foo this is shortcutted, thats why we add it here
#if TO_BE_PORTED
         SERVO_set_recovery_detection(SERVO_RECOVERY_DETECTION_ON);
#endif
         (void)adjust_p_set( data.p_start_first[0] , ADJUST_ITEM_FOCUS_OFFSET);
         //DEBUG_printf(("Set FOO for seek 0x%4x",(SHORT) data.p_start_first[0]));
         adjust_state_change(ADJUST_S_INTERM_SEEK);
      }
      else
      {
         adjust_state_change(ADJUST_S_P_SET);
      }

   }
   else
   {
      // there are no more points to examine: process the results!
      adjust_state_change(ADJUST_S_PROCESS_RESULTS);
   }
}

/*-----------------------------------------------------------------------*/

static void s_p_set(void)
{
   static BYTE delay;
   static LONG  item_number;

   switch (sub_state.s_p_set)
   {
      case ADJUST_S_P_SET_SUBST_INIT :
         item_number = 0;
         //lint -fallthrough
      case ADJUST_S_P_SET_SUBST_SET :
         if ((data.p[item_number] == data.p_start_actual[item_number]) ||
             (curr.mode == ADJUST_MODE_2D) ||  (curr.mode == ADJUST_MODE_ITEMSCAN))
         {//change side
            delay = coef.p[item_number].delay_big;
         }
         else
         {//small step
            delay = coef.p[item_number].delay_small;
         }
#if TO_BE_PORTED
         SERVO_set_recovery_detection(SERVO_RECOVERY_DETECTION_ON);
#endif
         if (adjust_p_set (data.p[item_number], curr.item[item_number]))
         {
            sub_state.s_p_set = ADJUST_S_P_SET_SUBST_WAIT_INIT;
            //KRNL_attach (TIMEBASE_0_MS, ADJUST_TASKID);
         }
         else
         {
            //KRNL_attach (TIMEBASE_5_MS, ADJUST_TASKID);
         }
         break;
      case ADJUST_S_P_SET_SUBST_WAIT_INIT :
         switch (curr.item[item_number])
         {
            case ADJUST_ITEM_SPHER_ABER:
               switch (svoStp_busy())
               {
                  case FALSE:
                     sub_state.s_p_set = ADJUST_S_P_SET_SUBST_READY;
                     //KRNL_unsubscribe(OSAL_EVENT_BE_STEP_COLLI_ALPHA, ADJUST_TASKID);
                     adjust_conditional_push_out_of_lock();
                     adjust_set_settle_timer(delay);
                     break;
                  case TRUE:
                  default:
                     break;
               }
               break;
            case ADJUST_ITEM_TANG_TILT:
#if TO_BE_PORTED
               switch (TANGTILT_get_status())
               {
                  case READY:
                     (void)TANGTILT_jump_finish();
                     //KRNL_subscribe(OSAL_EVENT_BE_STEP_COLLI_ALPHA, ADJUST_TASKID);
                     sub_state.s_p_set = ADJUST_S_P_SET_SUBST_WAIT_FINISH;
                     //KRNL_attach (TIMEBASE_0_MS, ADJUST_TASKID);
                     break;
                  case BUSY:
                     break;
                  case ERROR:
                     // What to do in case an error occurs?
                     DEBUG_ASSERT(FALSE,eASSERT_FUNCTION_NOT_IMPLEMENTED);//DEBUG_assert(FALSE);
                     break;
               }
#else
               sub_state.s_p_set = ADJUST_S_P_SET_SUBST_WAIT_FINISH;
#endif
               break;
            default:
               sub_state.s_p_set = ADJUST_S_P_SET_SUBST_READY;
               adjust_conditional_push_out_of_lock();
               adjust_set_settle_timer(delay);
               break;
         }//lint !e788
         break;
      case ADJUST_S_P_SET_SUBST_WAIT_FINISH :
         switch (curr.item[item_number])
         {
            case ADJUST_ITEM_TANG_TILT:
#if TO_BE_PORTED
               switch (TANGTILT_get_status())
               {
                  case READY:
                     sub_state.s_p_set = ADJUST_S_P_SET_SUBST_READY;
                     //KRNL_unsubscribe(OSAL_EVENT_BE_STEP_COLLI_ALPHA, ADJUST_TASKID);
                     adjust_conditional_push_out_of_lock();
                     adjust_set_settle_timer(delay);
                     break;
                  case BUSY:
                     break;
                  case ERROR:
                     // What to do in case an error occurs?
                     DEBUG_ASSERT(FALSE,eASSERT_FUNCTION_NOT_IMPLEMENTED);//DEBUG_assert(FALSE);
                     break;
               }
#else
               sub_state.s_p_set = ADJUST_S_P_SET_SUBST_READY;
#endif
               break;
            default:
               DEBUG_ASSERT(FALSE,eASSERT_FUNCTION_NOT_IMPLEMENTED);//DEBUG_assert(FALSE);
               break;
         }//lint !e788
         break;
      case ADJUST_S_P_SET_SUBST_READY:
         // check if settle time has passed, and p action is ready.
         if ( (timeout_chk(2) == eTimerTimeout) || delay == 0) //if (TIMER_is_expired (TIMER_ID_1_MS_ADJUST) || delay == 0)
         {
            // settle time passed and p action is ready
            item_number++;
            if(item_number < curr.no_of_items)
            {
               sub_state.s_p_set = ADJUST_S_P_SET_SUBST_SET;
               //KRNL_attach (TIMEBASE_0_MS, ADJUST_TASKID);
            }
            else
            {//the end
               //Determine how to proceed based on previous state.
               switch (previous_state)
               {
                  case ADJUST_S_P_INIT_RESTORE:
                     adjust_state_change(ADJUST_S_FRONT_BACK_RESTORE);
                     break;
                  case ADJUST_S_COST_MEASURE:
                     //KRNL_attach (TIMEBASE_5_MS, ADJUST_TASKID);
                     SetTimeout(1, MS_5*TIMEOUT_SERVO_RECOVER); //TIMER_start_timer (TIMER_ID_5_MS_ADJUST, TIMEOUT_SERVO_RECOVER, adjust_callback_func);
                     adjust_state_change(ADJUST_S_RECOVER);
                     break;
                  case ADJUST_S_INTERM_SEEK:
                     if (FOCUS_OFFSET_PRESET_NEEDED())
                     {
                        adjust_state_change(ADJUST_S_COST_INIT);
                     }
                     else
                     {
                        //KRNL_attach (TIMEBASE_5_MS, ADJUST_TASKID);
                        SetTimeout(1, MS_5*TIMEOUT_SERVO_RECOVER); //TIMER_start_timer (TIMER_ID_5_MS_ADJUST, TIMEOUT_SERVO_RECOVER, adjust_callback_func);
                        adjust_state_change(ADJUST_S_RECOVER);
                     }
                     break;
                  case ADJUST_S_P_NEXT:
                     adjust_state_change(ADJUST_S_INTERM_SEEK);
                     break;
                  default:
                     break;
               }//lint !e788
            }
         }
         break;
      default:
         break;
   }
}

/*-----------------------------------------------------------------------*/

static void s_p_init_restore (void)
{
   LONG item_number;

   send_msg5S(1,"p-init restore");//DEBUG_printf (("p-init restore"));
   if (data.p_start_valid)
   {
      // re-setting of p_start_actual only when value in 'data' is valid:
      // we might have come in this state before the state front/back_init has
      // been completed.....
      for(item_number=0; item_number < curr.no_of_items; item_number++)
      {
         if ( ((curr.config & ADJ_CONFIG_P_OPTIMUM_MASK) == ADJ_CONFIG_P_OPTIMUM)
               && (status != ERROR) /*status is only set to ready at the end, but we immediately set error*/
               && user_result.ok)
         {
            // Keep optimum p value.
            data.p[item_number] = user_result.item_optimum[item_number];
         }
         else
         {
            if (curr.mode == ADJUST_MODE_2D)
            {
               data.p[item_number] = data.p_start_first[item_number];
            }
            else
            {  // ADJUST_MODE_IDLE,  ADJUST_MODE_1D,  ADJUST_MODE_ITEMSCAN, ADJUST_MODE_LAST
               // Restore start p value.
               data.p[item_number] = data.p_start_first[item_number];
            }
         }
         //DEBUG_system_dma_printf((DBG_ADJUST"ADJUST %s:=0x%04x",
         //                           ADJUST_ITEM_to_string[curr.item[item_number]],
         //                           data.p[item_number]));
      }
      adjust_state_change(ADJUST_S_P_SET);
   }
   else
   {
      // data.p_start_actual has not yet been initialised.
      adjust_state_change(ADJUST_S_FRONT_BACK_RESTORE);
   }
}

/*--------------------------------------------------------------------------*/

static void s_interm_seek(void)
{
   static LONG fine_seek_delta;
#if TO_BE_PORTED
   ULONG  addr;
#endif

   switch (sub_state.s_interm_seek)
   {
      case ADJUST_S_INTERM_SEEK_SUBST_START:
         fine_seek_delta = 0;
         if(((curr.config & ADJ_CONFIG_SEEK_MODE_MASK) == ADJ_CONFIG_RESEEK)
             || ((curr.config & ADJ_CONFIG_SEEK_MODE_MASK) == ADJ_CONFIG_FINE_RESEEK)
             || initiate_reseek)
         {
            sub_state.s_interm_seek = ADJUST_S_INTERM_SEEK_SUBST_CLOSE_LOOP;
         }
         else
         {
            sub_state.s_interm_seek = ADJUST_S_INTERM_SEEK_SUBST_FINAL;
         }
         //KRNL_attach (TIMEBASE_0_MS, ADJUST_TASKID);
         break;

      case ADJUST_S_INTERM_SEEK_SUBST_CLOSE_LOOP:
         switch (ss_conditional_close_loop())
         {
            case ERROR :
            case READY :
               data.buf[data.flank][data.i].seek_recoveries = 0;
#if TO_BE_PORTED
               SEEK_seek_address (curr.start - fine_seek_border[curr.DiscKind.fDiscKind] , adjust_callback_func); // fine_seek_border as seek offset
#endif
               //KRNL_attach (TIMEBASE_5_MS, ADJUST_TASKID);//check every 5ms if there is no servo_recovery

               //SERVODBG_pin_set (SERVODBG_PIN_2);
               sub_state.s_interm_seek = ADJUST_S_INTERM_SEEK_SUBST_WAIT; // activation by SEEK's callback
               break;
            case BUSY  :
               break;
            default:
               break;
         }
         break;

      case ADJUST_S_INTERM_SEEK_SUBST_WAIT:
#if TO_BE_PORTED
         switch (SEEK_get_status())
         {
            case READY:
               //KRNL_detach (ADJUST_TASKID);// from 5ms-timebase
               if ((curr.config & ADJ_CONFIG_SEEK_MODE_MASK) == ADJ_CONFIG_FINE_RESEEK)
               {
                  SUBC_read_address (adjust_callback_func);
                  SetTimeout(1, MS_5*TIMEOUT_OVER_PAUSE_RECOVER); //TIMER_start_timer (TIMER_ID_5_MS_ADJUST, TIMEOUT_OVER_PAUSE_RECOVER, adjust_callback_func);
                  if (FOCUS_OFFSET_PRESET_NEEDED())
                  {
                     fine_seek_delta = (LONG)LIB_duration_2_pba(coef.p[0].delay_small);//this is the delay for focus offset
                     //DEBUG_printf ((DBG_ADJUST"focus offset pba delta %d" , fine_seek_delta));
                  }
                  sub_state.s_interm_seek = ADJUST_S_INTERM_SEEK_FINE_SEEK;
               }
               else
               {
                  sub_state.s_interm_seek = ADJUST_S_INTERM_SEEK_SUBST_OPEN_LOOP;
               }
               //KRNL_attach (TIMEBASE_0_MS, ADJUST_TASKID);
               break;

            case BUSY:
               // activated by timer
               if(SERVO_is_recovery_detected())
               {
                  //DEBUG_system_dma_printf((DBG_ADJUST"Recovery during interm seek"));
                  //KRNL_detach (ADJUST_TASKID);
                  SEEK_abort();
                  data.buf[data.flank][data.i].seek_recoveries = FINE_SEEK_NO_OF_RETRIES;
                  sub_state.s_interm_seek = ADJUST_S_INTERM_FINE_SEEK_RETRY;
                  //KRNL_attach (TIMEBASE_0_MS, ADJUST_TASKID);
               }
               break;

            case ERROR:
            default:
               //KRNL_detach (ADJUST_TASKID);// from 5ms-timebase
               data.buf[data.flank][data.i].seek_recoveries = FINE_SEEK_NO_OF_RETRIES;
               // clip the numeber of recoveries => end this measurement
               //DEBUG_system_dma_printf ((DBG_ADJUST"ADJ:seek NOK, next point"));
               sub_state.s_interm_seek = ADJUST_S_INTERM_FINE_SEEK_RETRY;
               //KRNL_attach (TIMEBASE_0_MS, ADJUST_TASKID);
               break;
         }
#endif
         break;

      case ADJUST_S_INTERM_SEEK_FINE_SEEK:
#if TO_BE_PORTED
         if (SERVO_is_recovery_detected() == 0)
         {
            if (SUBC_get_address (&addr))
            //ecc block boundary
            // you only have three adip per ECC block, and every 4th adip addr is missing
            {   //dvd minus needs more seek offset !
               ULONG   curr_start_internaddr;
               curr_start_internaddr = MISC_pba2intern_addr(curr.start, curr.DiscKind.Bit.DiscStd);
               if(((LONG)addr + fine_seek_border[curr.DiscKind.fDiscKind] + fine_seek_delta) >= (LONG)curr_start_internaddr)
               {
                  if (((LONG)addr - fine_seek_border[curr.DiscKind.fDiscKind]) <= (LONG)curr_start_internaddr)
                  {
                     sub_state.s_interm_seek = ADJUST_S_INTERM_SEEK_SUBST_OPEN_LOOP;
                     //KRNL_attach (TIMEBASE_0_MS, ADJUST_TASKID);
                  }
                  else
                  {
                     //DEBUG_system_dma_printf ((DBG_ADJUST"ADJ:fine seek, NOK, overshoot %x>%x, retry %d", addr, curr_start_internaddr, data.buf[data.flank][data.i].seek_recoveries));
                     sub_state.s_interm_seek = ADJUST_S_INTERM_FINE_SEEK_RETRY;
                     //KRNL_attach (TIMEBASE_0_MS, ADJUST_TASKID);
                  }
               }
               else
               {
                  if (timeout_chk(1) == eTimerTimeout) //if (TIMER_is_expired (TIMER_ID_5_MS_ADJUST))
                  {
                     //DEBUG_system_dma_printf ((DBG_ADJUST"ADJ:fine seek, NOK, deadpause %x>%x, retry %d", addr, curr_start_internaddr, data.buf[data.flank][data.i].seek_recoveries));
                     sub_state.s_interm_seek = ADJUST_S_INTERM_FINE_SEEK_RETRY;
                     //KRNL_attach (TIMEBASE_0_MS, ADJUST_TASKID);
                  }
                  else
                  {
                     //DEBUG_printf ((DBG_ADJUST"ADJ: addr=%x %x", curr_start_internaddr, ((LONG)addr + fine_seek_border[curr.DiscKind])))
                     SUBC_read_address (adjust_callback_func);
                  }
               }
            }
            else
            {
               if (timeout_chk(1) == eTimerTimeout) //if (TIMER_is_expired (TIMER_ID_5_MS_ADJUST))
               {
                  //DEBUG_system_dma_printf ((DBG_ADJUST"ADJ:fine seek, SUBC_read_addr takes too long %x",curr.start));
                  SUBC_cancel(adjust_callback_func);
                  data.buf[data.flank][data.i].seek_recoveries = FINE_SEEK_NO_OF_RETRIES;
                  sub_state.s_interm_seek = ADJUST_S_INTERM_FINE_SEEK_RETRY;
                  //KRNL_attach (TIMEBASE_0_MS, ADJUST_TASKID);
               }
            }
         }
         else
         {
            //DEBUG_system_dma_printf ((DBG_ADJUST"ADJ:fine seek, NOK, servo recovery >> %d ", SERVO_is_recovery_detected()));
            data.buf[data.flank][data.i].seek_recoveries = FINE_SEEK_NO_OF_RETRIES;
            sub_state.s_interm_seek = ADJUST_S_INTERM_FINE_SEEK_RETRY;
            //KRNL_attach (TIMEBASE_0_MS, ADJUST_TASKID);
         }
#endif
         break;

      case ADJUST_S_INTERM_FINE_SEEK_RETRY:
         {
            LONG item_number;

            SetTimeout(1, TIMER_STOP);//TIMER_clear_timer (TIMER_ID_5_MS_ADJUST);    // in case the DEADPAUSE happend
            if (data.buf[data.flank][data.i].seek_recoveries < FINE_SEEK_NO_OF_RETRIES)
            {
               data.buf[data.flank][data.i].seek_recoveries ++;
#if TO_BE_PORTED
               SEEK_seek_address (curr.start - fine_seek_border[curr.DiscKind.fDiscKind] , adjust_callback_func); // fine_seek_border as seek offset
#endif
               //KRNL_attach (TIMEBASE_5_MS, ADJUST_TASKID);//check every 5ms if there is no servo_recovery
               sub_state.s_interm_seek = ADJUST_S_INTERM_SEEK_SUBST_WAIT; // activation by SEEK's callback
            }
            else
            {
               //DEBUG_system_dma_printf ((DBG_ADJUST"ADJ:fine seek, NOK, RETRY OVER"));
               data.buf[data.flank][data.i].seek_recoveries = 0;
               // do the recovery with the best possible p, i.e. p_start_actual
               // and continue with the other flank (via MAX_LONG)
               data.buf[data.flank][data.i].c = MAX_LONG; // clear this measurement
               data.buf[data.flank][data.i].servo_recoveries++;
               for(item_number=0; item_number < curr.no_of_items; item_number++)
               {
                  data.p[item_number] = data.p_start_first[item_number]; //first set to start point and then recover
               }
               //SERVODBG_pin_clear (SERVODBG_PIN_2);
               if (FOCUS_OFFSET_PRESET_NEEDED())
               {//you were set to default before doing the seek
                  //KRNL_attach (TIMEBASE_5_MS, ADJUST_TASKID);
                  SetTimeout(1, MS_5*TIMEOUT_SERVO_RECOVER); //TIMER_start_timer (TIMER_ID_5_MS_ADJUST, TIMEOUT_SERVO_RECOVER, adjust_callback_func);
                  adjust_state_change(ADJUST_S_RECOVER);
               }
               else
               {
                  adjust_state_change(ADJUST_S_P_SET);
               }
            }
         }
         break;

      case ADJUST_S_INTERM_SEEK_SUBST_OPEN_LOOP:
         switch (ss_conditional_open_loop())
         {
            case ERROR :
               DEBUG_ASSERT(FALSE,eASSERT_FUNCTION_NOT_IMPLEMENTED);//DEBUG_assert(FALSE);
               break;
            case READY :
               sub_state.s_interm_seek = ADJUST_S_INTERM_SEEK_SUBST_FINAL;
               //KRNL_attach (TIMEBASE_0_MS, ADJUST_TASKID);
               break;
            case BUSY  :
               break;
            default:
               break;
         }
         break;

      case ADJUST_S_INTERM_SEEK_SUBST_FINAL:
         data.buf[data.flank][data.i].seek_recoveries = 0;
         SetTimeout(1, TIMER_STOP);//TIMER_clear_timer (TIMER_ID_5_MS_ADJUST);    // in case the DEADPAUSE happend
#if TO_BE_PORTED
         SERVO_set_recovery_detection(SERVO_RECOVERY_DETECTION_ON);
#endif
         //SERVODBG_pin_clear (SERVODBG_PIN_2);

         if(initiate_reseek)
         {
            initiate_reseek = FALSE;
            adjust_state_change(ADJUST_S_P_NEXT);
         }
         else if (FOCUS_OFFSET_PRESET_NEEDED())
         {
            adjust_state_change(ADJUST_S_P_SET);
         }
         else
         {
            adjust_state_change(ADJUST_S_COST_INIT);
         }
         break;

      default:
         break;
   }
}

/*--------------------------------------------------------------------*/

static void s_recover (void)
{
#if TO_BE_PORTED
   // activated by 5ms-timebase OR by software-timer OR by interrupt
   if ( (timeout_chk(1) == eTimerTimeout) /*(TIMER_is_expired (TIMER_ID_5_MS_ADJUST))*/ ||
       ((SERVO_get_status () == READY) && ADJUST_hf_valid_or_not_needed(curr.cost, curr.DiscKind)))
#endif
   {
      //DEBUG_system_dma_printf (("recover-end timer:%d servo:%d pll:%d(freq %d)",
      //       (TIMER_is_expired (TIMER_ID_5_MS_ADJUST) == TRUE)?1:0,
      //       (SERVO_get_status () == READY)?1:0,
      //       (ADJUST_hf_valid_or_not_needed(curr.cost, curr.DiscKind))?1:0,
      //       C_DEC_get_efm_pll_frequency()));

      if(initiate_reseek)
      {
         adjust_state_change(ADJUST_S_INTERM_SEEK);
      }
      else
      {
         adjust_state_change(ADJUST_S_P_NEXT);
      }

      // clean-up:
      //KRNL_detach (ADJUST_TASKID);                 // from 5ms-timebase
      SetTimeout(1, TIMER_STOP);//TIMER_clear_timer (TIMER_ID_5_MS_ADJUST);    // in case that SERVO/PLL became READY
   }
   // else: timer not yet expired and still problems for servo/pll:stay in this state
}

/*-----------------------------------------------------------------------*/

static void s_cost_init (void)
{
   //DEBUG_printf (("s"));

   //SERVODBG_pin_set (SERVODBG_PIN_14);

#if TO_BE_PORTED
   C_DEC_HoldOnDefect(C_DEC_HOLDEN_DISABLE);
#endif

   if(curr.DiscKind.Bit.DiscStd == eDiscBD)
   {
      //DEBUG_system_dma_printf(("HF gain %d, offset %d",READ_PROP(TMHAL_CCHFREG_GAINDYNMON),READ_PROP(TMHAL_CCHFREG_OFFSETCOMPVALUEMON)));
      //DEBUG_system_dma_printf(("demod %d, FWlocked %d",READ_PROP(TMHAL_CCDECREG_DEMODLOCKED),READ_PROP(TMHAL_CCDECREG_AUNFLYWHEELLOCKED)));
   }
   else
   {
      //DEBUG_system_dma_printf(("HF gain %d, offset %d",READ_PROP(hal_cc_HF_G3SET),READ_PROP(hal_cc_HF_OFFSETVALUE)));
   }
   switch (curr.cost)
   {
      case ADJUST_COST_STREAM_SAMPLED_JITTER:
      case ADJUST_COST_STREAM_SAMPLED_JITTER_ITEMSCAN:
#if TO_BE_PORTED
         (void)FAST_MEAS_measure_start(FAST_MEAS_ITEM_JITTER, FAST_MEAS_DURATION_ONE_REV_INT);
         //KRNL_subscribe (OSAL_EVENT_BE_ANG_TIMER, ADJUST_TASKID);
         //KRNL_attach (TIMEBASE_5_MS, ADJUST_TASKID);  // checking only
#endif
         break;
      case ADJUST_COST_ARM_SAMPLED_JITTER:
      case ADJUST_COST_ARM_SAMPLED_JITTER_ITEMSCAN:
         data.buf[data.flank][data.i].c = 0;
         data.i_sub = 0;
#if TO_BE_PORTED
         REAL_command(2, REAL_CMD_ARMANGLETIMER, 0x82);//1 revolution interrupt
         //KRNL_subscribe (OSAL_EVENT_BE_ANG_TIMER, ADJUST_TASKID);
         //KRNL_attach (TIMEBASE_1_MS, ADJUST_TASKID);  // checking and sampling
#endif
         break;
      case ADJUST_COST_PUSHPULL_AMPL:
      case ADJUST_COST_PUSHPULL_AMPL_ITEMSCAN:
#if TO_BE_PORTED
         (void)FAST_MEAS_measure_start(FAST_MEAS_ITEM_PUSHPULL, FAST_MEAS_DURATION_ONE_REV_INT);
         //KRNL_subscribe (OSAL_EVENT_BE_ANG_TIMER, ADJUST_TASKID);
         //KRNL_attach (TIMEBASE_5_MS, ADJUST_TASKID);  // checking only
#endif
         break;
      case ADJUST_COST_WOBBLE_AMPL:
      case ADJUST_COST_WOBBLE_AMPL_ITEMSCAN:
#if TO_BE_PORTED
         (void)FAST_MEAS_measure_start(FAST_MEAS_ITEM_WOBBLE_AMP, FAST_MEAS_DURATION_ONE_REV_INT);
         //KRNL_subscribe (OSAL_EVENT_BE_ANG_TIMER, ADJUST_TASKID);
         //KRNL_attach (TIMEBASE_5_MS, ADJUST_TASKID);  // checking only
#endif
         break;
      case ADJUST_COST_ABORT:
      case ADJUST_COST_NONE:
      case ADJUST_COST_LAST:
      default:
         break;
   }
   // one time immediate activation in COST_MEASURE to do the servo/pll checking
   // (this is on top of the 1 (or 5) ms timebase
   adjust_state_change(ADJUST_S_COST_MEASURE);
}

/*-----------------------------------------------------------------------*/

static void s_cost_measure (void)
{
   LONG *   c;
#if TO_BE_PORTED
   C_DEC_jitter_output_t jitter_out;

   if ((SERVO_get_status () != READY) || SERVO_is_recovery_detected())
   {
      LONG item_number;

      adjust_cost_measure_end();
      //DEBUG_system_dma_printf((DBG_ADJUST"servo NOK->recover"));

      if (DB_is_DUAL_LAYER() && DB_is_BD())
      {
         initiate_reseek = TRUE;
         //DEBUG_system_dma_printf((DBG_ADJUST"initiate a RESEEK for DUAL LAYER disc"));
      }
      // do the recovery with the best possible p, i.e. p_start_actual
      // and continue with the other flank (via .servo_recoveries)
      data.buf[data.flank][data.i].c = MAX_LONG; // clear this measurement
      data.buf[data.flank][data.i].servo_recoveries++;
      for(item_number=0; item_number < curr.no_of_items; item_number++)
      {
         data.p[item_number] = data.p_start_actual[item_number]; //first set to start point and then recover
      }
      if (SERVO_is_stopped())
      {
         //DEBUG_system_dma_printf((DBG_ADJUST"WARNING servo is stopped"));
         status = ERROR;
         adjust_state_change(ADJUST_S_P_INIT_RESTORE);
      }
      else
      {
         adjust_state_change(ADJUST_S_P_SET);
      }
      return;
   }
#endif

   if (!ADJUST_hf_valid_or_not_needed(curr.cost, curr.DiscKind))
   {
      adjust_cost_measure_end();
      //DEBUG_system_dma_printf((DBG_ADJUST"ADJUST: NO HF"));
      // do the recovery with the same p
      data.buf[data.flank][data.i].c = MAX_LONG; // clear this measurement
      data.buf[data.flank][data.i].hf_recoveries++;
      //in adjust_p_is_end_of_flank we will check the number of recoveries
      //in adjust_p_next_determine we will keep the same point to measure
      // (by testing c == MAX_LONG && nr of recoveries > 0)
      //KRNL_attach (TIMEBASE_5_MS, ADJUST_TASKID);
      SetTimeout(1, MS_5*TIMEOUT_PLL_RECOVER); //TIMER_start_timer (TIMER_ID_5_MS_ADJUST, TIMEOUT_PLL_RECOVER, adjust_callback_func);
#if TO_BE_PORTED
      PLL_hf_forced_autolock(PLL_HFLOOPS_DISABLED);
#endif
      adjust_state_change(ADJUST_S_RECOVER);
      return;
   }

   // SERVO and PLL OK


   c = &data.buf[data.flank][data.i].c;   // code-size optimization...
#if TO_BE_PORTED
   if (KRNL_is_event_set (OSAL_EVENT_BE_1_MSEC))
   {
      switch (curr.cost)
      {
         BYTE i;
         ULONG jitter_val;

         case ADJUST_COST_ARM_SAMPLED_JITTER:
         case ADJUST_COST_ARM_SAMPLED_JITTER_ITEMSCAN:
            data.i_sub++;

            jitter_val = 0;
            for (i = 0; i<= 9; i++)
            {
               C_DEC_get_jitter (C_DEC_JITTER_ZERO_X, C_DEC_JITTER_TYPE_PERC_SQR, &jitter_out);
               jitter_val += jitter_out.av_jitter.jitter;
            }
            *c += (LONG)LIB_sqrt_32int (100*jitter_val/i);
            break;

         case ADJUST_COST_STREAM_SAMPLED_JITTER:
         case ADJUST_COST_PUSHPULL_AMPL:
         case ADJUST_COST_WOBBLE_AMPL:
         case ADJUST_COST_STREAM_SAMPLED_JITTER_ITEMSCAN:
         case ADJUST_COST_PUSHPULL_AMPL_ITEMSCAN:
         case ADJUST_COST_WOBBLE_AMPL_ITEMSCAN:
         case ADJUST_COST_ABORT:
         case ADJUST_COST_NONE:
         case ADJUST_COST_LAST:
         default:
            break;
      }
   }

   if (KRNL_is_event_set (OSAL_EVENT_BE_ANG_TIMER))
   {
      //SERVODBG_pin_clear (SERVODBG_PIN_14);

      // TODO: measure...
      switch (curr.cost)
      {
         case ADJUST_COST_STREAM_SAMPLED_JITTER:
         case ADJUST_COST_STREAM_SAMPLED_JITTER_ITEMSCAN:
             {
               FAST_MEAS_RESULT_t result;

               FAST_MEAS_get_result(&result);
               //DEBUG_printf(("sample: 0x%4x", result.meas_out));
               //calculate jitter

               *c = (LONG)C_DEC_calc_jitter(result.meas_out, result.amplifier);
            }
            break;

         case ADJUST_COST_ARM_SAMPLED_JITTER:
         case ADJUST_COST_ARM_SAMPLED_JITTER_ITEMSCAN:
            if (data.i_sub == 0)
            {
               //DEBUG_system_dma_printf((DBG_ADJUST"WARNING no sub-sampling p(%d,%d)=0x%x,0x%x",
               //      data.flank, data.i,
               //      data.buf[data.flank][data.i].p[0],
               //      data.buf[data.flank][data.i].p[1]));
               *c = MAX_LONG;
            }
            else
            {
               *c /= data.i_sub;
            }
            break;

         case ADJUST_COST_PUSHPULL_AMPL:
         case ADJUST_COST_WOBBLE_AMPL:
         case ADJUST_COST_PUSHPULL_AMPL_ITEMSCAN:
         case ADJUST_COST_WOBBLE_AMPL_ITEMSCAN:
            {
               FAST_MEAS_RESULT_t result;
               FAST_MEAS_get_result(&result);
               *c = (SHORT)(result.meas_out/result.amplifier);
            }
            break;

         case ADJUST_COST_ABORT:
         case ADJUST_COST_NONE:
         case ADJUST_COST_LAST:
         default:
            break;
      }
      //DEBUG_system_dma_printf(("demod %d, FWlocked %d after meas",READ_PROP(TMHAL_CCDECREG_DEMODLOCKED),READ_PROP(TMHAL_CCDECREG_AUNFLYWHEELLOCKED)));
      //DEBUG_system_dma_printf (("p(%d,%d)=0x%x,0x%x c=0x%x(#%d) (pllfreq %d)", data.flank, data.i,
      //            data.buf[data.flank][data.i].p[0],
      //            data.buf[data.flank][data.i].p[1],
      //            *c,
      //            data.i_sub,
      //            C_DEC_get_efm_pll_frequency()));
      //set min and max
      if ((data.flank == 0) && (data.i == 0) && (data.ramp_mode == RAMPING_UP))
      {
         // very first point
         data.c_max = *c;
         data.c_min = *c;
      }
      else
      {
         if (*c > data.c_max)
         {
            data.c_max = *c;
         }
         if (*c < data.c_min)
         {
            data.c_min = *c;
         }
      }
      //DEBUG_printf(("-aft- AGCc:%x, OFFc:%x, AGCAOC:%x, Det:%x, Freq:%x, PLLsts:%x, AOff:%x, Pk:%x",
      //         READ_REG(TMHAL_CCREG_MAP->BDHFAGCConfig),
      //         READ_REG(TMHAL_CCREG_MAP->BDHFOffsetConfig),
      //         READ_REG(TMHAL_CCREG_MAP->BDHFAGCAOCMon),
      //         READ_REG(TMHAL_CCREG_MAP->BDHFDetectStatus),
      //         READ_REG(TMHAL_CCREG_MAP->BDHFPLLFreq),
      //         READ_REG(TMHAL_CCREG_MAP->BDHFPLLStatus),
      //         READ_REG(TMHAL_CCREG_MAP->BDHFAmplOffset),
      //         READ_REG(TMHAL_CCREG_MAP->BDHFPeak)
      //        ));

      adjust_cost_measure_end();

      //KRNL_detach (ADJUST_TASKID);  // detach from 1ms or 5ms timebase
      adjust_state_change(ADJUST_S_P_NEXT);
   }
#else
   *c = HAL_DFE_ReadJitterX10();
   adjust_state_change(ADJUST_S_P_NEXT);
#endif
}

/*--------------------------------------------------------------------------*/

static void s_process_results (void)
{
   switch(curr.mode)
   {
      case ADJUST_MODE_1D:
         if (optimal_algorithm)
         {
            adjust_process_1d_results_optimal();
         }
         else
         {
            adjust_process_1d_results();
         }
         break;
      case ADJUST_MODE_2D:
         adjust_process_2d_results();
         break;
      case ADJUST_MODE_ITEMSCAN:
         adjust_process_itemscan_results();
         break;
      case ADJUST_MODE_IDLE:
      case ADJUST_MODE_LAST:
      default:
         break;
   }
}

/*--------------------------------------------------------------------------*/

static void s_front_back_restore (void)
{
   LONG item_number;

   switch (sub_state.s_front_back_restore)
   {
      case ADJUST_S_FRONT_BACK_RESTORE_SUBST_START :
         adjust_cost_measure_end();

         // restore back (including re-setting initial value of 'item')
         for(item_number=0; item_number < curr.no_of_items; item_number++)
         {
            switch (curr.item[item_number])
            {
               case ADJUST_ITEM_RAD_TILT:
#if TO_BE_PORTED
                  TILT_limit_tilt (TILT_NORMAL_LIMIT);
#endif
                  break;
               case ADJUST_ITEM_SPHER_ABER:
               case ADJUST_ITEM_TANG_TILT:
               case ADJUST_ITEM_FOCUS_OFFSET:
               case ADJUST_ITEM_RAD_OFFSET:
               case ADJUST_ITEM_RE_GAIN_OFFSET:
               case ADJUST_ITEM_SSPP_GAIN:
               case ADJUST_ITEM_WOPC:
               case ADJUST_ITEM_FOC_INT:
               case ADJUST_ITEM_RE:
               case ADJUST_ITEM_REFLECTION:
               case ADJUST_ITEM_FRD_REF:
               case ADJUST_ITEM_ERASE_REFLECTION:
               case ADJUST_ITEM_DUTYCYCLE_5_12V:
               case ADJUST_ITEM_LASER_TEMP_ACT_DISP:
               case ADJUST_ITEM_LASER_TRESHOLD_CURRENT:
               case ADJUST_ITEM_ABORT:
               case ADJUST_ITEM_NONE:
               case ADJUST_ITEM_LAST:
               default:
                  break;
            }
         }
         sub_state.s_front_back_restore = ADJUST_S_FRONT_BACK_RESTORE_SUBST_WAIT;
         //lint -fallthrough
      case ADJUST_S_FRONT_BACK_RESTORE_SUBST_WAIT :
         switch (ss_conditional_close_loop())
         {
            case ERROR :
               status = ERROR;
               //lint -fallthrough
            case READY :
               sub_state.s_front_back_restore = ADJUST_S_FRONT_BACK_RESTORE_SUBST_FINAL;
               //KRNL_attach (TIMEBASE_0_MS, ADJUST_TASKID);
               break;
            case BUSY  :
               break;
            default:
               break;
         }
         break;
      case ADJUST_S_FRONT_BACK_RESTORE_SUBST_FINAL :
         if (status == ERROR)
         {
            if(curr.mode == ADJUST_MODE_2D)
            {
               //DEBUG_system_dma_printf((DBG_ADJUST"adjust failed [%s,%s/%s] on 0x%x",
               //                                       ADJUST_ITEM_to_string[curr.item[0]],
               //                                       ADJUST_ITEM_to_string[curr.item[1]],
               //                                       adjust_cost_to_string[curr.cost],
               //                                       curr.start));
               //ERROR_set_player_error(ERROR_CALIBRATION_FAILED_ERROR);
            }
            else
            {
               //DEBUG_system_dma_printf((DBG_ADJUST"adjust failed [%s/%s] on 0x%x",
               //                                       ADJUST_ITEM_to_string[curr.item[0]],
               //                                       adjust_cost_to_string[curr.cost],
               //                                       curr.start));
               //ERROR_set_player_error(ERROR_CALIBRATION_FAILED_ERROR);
            }

            user_result.ok = FALSE;
         }
         else
         {
            status = READY;
            if(curr.mode == ADJUST_MODE_2D)
            {
               //DEBUG_system_dma_printf((DBG_ADJUST"ADJUST result [L%d]= %d , %d ( 0x%04x , 0x%04x ) cost= %d [ %s , %s / %s ] on 0x%x",
               //                           servo_limit_layer,
               //                           user_result.item_optimum[0], user_result.item_optimum[1],
               //                           user_result.item_optimum[0], user_result.item_optimum[1],
               //                           user_result.cost_optimum,
               //                           ADJUST_ITEM_to_string[curr.item[0]],
               //                           ADJUST_ITEM_to_string[curr.item[1]],
               //                           adjust_cost_to_string[curr.cost],
               //                           curr.start));
            }
            else
            {
               //DEBUG_system_dma_printf((DBG_ADJUST"ADJUST result [L%d]= %d (0x%04x) cost = %d [ %s / %s ] on 0x%x",
               //                           servo_limit_layer,
               //                           user_result.item_optimum[0],user_result.item_optimum[0], user_result.cost_optimum,
               //                           ADJUST_ITEM_to_string[curr.item[0]], adjust_cost_to_string[curr.cost],curr.start));
            }
         }
         adjust_state_change(ADJUST_S_FINAL);
         break;
      default:
         break;
   }
}

/*--------------------------------------------------------------------------*/

static void s_final (void)
{
   if (curr.client_callback_func != NULL)
   {
      if ((curr.buffer_size >= RESULT_BUFFER_LENGTH)||
          ((curr.mode == ADJUST_MODE_2D) && (curr.buffer_size >= RESULT_BUFFER_LENGTH_2D)))
      {
         //transfer result -> buffer  (common)
         curr.buffer[0] = (BYTE)(user_result.item_optimum[0] >> 24);//lint !e704
         curr.buffer[1] = (BYTE)(user_result.item_optimum[0] >> 16);//lint !e704
         curr.buffer[2] = (BYTE)(user_result.item_optimum[0] >> 8);//lint !e704
         curr.buffer[3] = (BYTE)(user_result.item_optimum[0]);

         if (curr.mode == ADJUST_MODE_2D)
         {
            //transfer result -> buffer  (2D)
            curr.buffer[4]  = (BYTE)(user_result.item_optimum[1] >> 24);//lint !e704
            curr.buffer[5]  = (BYTE)(user_result.item_optimum[1] >> 16);//lint !e704
            curr.buffer[6]  = (BYTE)(user_result.item_optimum[1] >> 8);//lint !e704
            curr.buffer[7]  = (BYTE)(user_result.item_optimum[1]);
            curr.buffer[8]  = (BYTE)(user_result.cost_optimum >> 24);//lint !e704
            curr.buffer[9]  = (BYTE)(user_result.cost_optimum >> 16);//lint !e704
            curr.buffer[10] = (BYTE)(user_result.cost_optimum >> 8);//lint !e704
            curr.buffer[11] = (BYTE)(user_result.cost_optimum);

            *curr.nrof_bytes_copied = RESULT_BUFFER_LENGTH_2D;
            adjust_logging_data[curr.item[1]].p_opt   = user_result.item_optimum[1];
            adjust_logging_data[curr.item[1]].c_opt   = user_result.cost_optimum;
            adjust_logging_data[curr.item[1]].p_width = p_width;
         }
         else
         {
            //transfer result -> buffer  (remaining 1D)
            curr.buffer[4] = (BYTE)(user_result.cost_optimum >> 24);//lint !e704
            curr.buffer[5] = (BYTE)(user_result.cost_optimum >> 16);//lint !e704
            curr.buffer[6] = (BYTE)(user_result.cost_optimum >> 8);//lint !e704
            curr.buffer[7] = (BYTE)(user_result.cost_optimum);
            *curr.nrof_bytes_copied = RESULT_BUFFER_LENGTH;
         }
      }
      adjust_logging_data[curr.item[0]].p_opt   = user_result.item_optimum[0];
      adjust_logging_data[curr.item[0]].c_opt   = user_result.cost_optimum;
      adjust_logging_data[curr.item[0]].p_width = p_width;

      curr.client_callback_func ();
   }
#if TO_BE_PORTED
   SERVO_set_recovery_detection(SERVO_RECOVERY_DETECTION_OFF);
#endif
   // clear all other stuff
   SetTimeout(0, TIMER_STOP);//TIMER_clear_timer (TIMER_ID_100_MS_ADJUST_WATCHDOG);
   SetTimeout(1, TIMER_STOP);//TIMER_clear_timer (TIMER_ID_5_MS_ADJUST);
   SetTimeout(2, TIMER_STOP);//TIMER_clear_timer (TIMER_ID_1_MS_ADJUST);
   adjust_clear_session_info (&curr);
#if TO_BE_PORTED
   SERVO_switch_preset(ON);
   SUBC_set_address_mode(orig_lock_mode, orig_pll_kick);
#endif
   optimal_algorithm = FALSE;


   //KRNL_unsubscribe (OSAL_EVENT_BE_HFPLL_LOCK, ADJUST_TASKID);
   //KRNL_detach (ADJUST_TASKID);
   //SERVODBG_pin_clear (SERVODBG_PIN_15);
   adjust_state_change(ADJUST_S_IDLE);
}

/*****************************************************************************
*                    Public Function Definitions
*****************************************************************************/

ADJ_RESULT_t *ADJUST_GetResult (void)
{
   return (&user_result);
}

/*--------------------------------------------------------------------------*/

EStatus ADJUST_GetStatus (void)
{
   //send_msg5S(1,"ADJUST_GetStatus");
   return (status);
}

/*--------------------------------------------------------------------------*/

void ADJUST_Init (void)
{
   send_msg80(1,0x710077);
   SetTimeout(0, TIMER_STOP);//TIMER_clear_timer (TIMER_ID_100_MS_ADJUST_WATCHDOG);
   SetTimeout(1, TIMER_STOP);//TIMER_clear_timer (TIMER_ID_5_MS_ADJUST);
   SetTimeout(2, TIMER_STOP);//TIMER_clear_timer (TIMER_ID_1_MS_ADJUST);

   adjust_state_change(ADJUST_S_IDLE);
   user_result.ok       = FALSE;
   status               = READY;
   abort_initiated      = FALSE;
   adjust_clear_session_info (&curr);
   adjust_clear_session_info (&new);
}

/*--------------------------------------------------------------------------*/

EStatus ADJUST_Abort (FPTR callback_func)
{
   EStatus abort_status = BUSY;

   send_msg80(1,0x710078);

   //DEBUG_system_dma_printf (("ADJUST_aborts %s ss %d stat %d ai %d ",adjust_state_2_str[state],sub_state.s_p_set,status,abort_initiated));

#if TO_BE_PORTED
   SERVO_set_recovery_detection(SERVO_RECOVERY_DETECTION_OFF);
#endif
   if ((abort_initiated == FALSE) && (state != ADJUST_S_IDLE))
   {
      abort_initiated            = TRUE;
      curr.client_callback_func  = callback_func;
      //KRNL_attach( TIMEBASE_0_MS, DYNCAL_TASKID );
   }
   else
   {
      if (state == ADJUST_S_IDLE)
      {
         active = FALSE;
         abort_initiated = FALSE;
         abort_status = ERROR;
      }
   }
   return (abort_status);
}

/*--------------------------------------------------------------------------*/

void adjust_handle_abort (void)
{
   //DEBUG_system_dma_printf(("adjust_handle_abort s %s ss %d",adjust_state_2_str[state],sub_state.s_p_set));
   optimal_algorithm = FALSE;
   switch (state)
   {
      case ADJUST_S_IDLE:
         adjust_clear_session_info (&new);
         status = ERROR;
         break;
      case ADJUST_S_INITIAL_SEEK:
#if TO_BE_PORTED
         SEEK_abort();
#endif
         status = ERROR;
         adjust_state_change(ADJUST_S_FINAL);
         break;
      case ADJUST_S_INTERM_SEEK:
#if TO_BE_PORTED
         SEEK_abort();
#endif
         //lint -fallthrough
      case ADJUST_S_FRONT_BACK_INIT:
      case ADJUST_S_P_INIT:
      case ADJUST_S_P_NEXT:
      case ADJUST_S_COST_INIT:
      case ADJUST_S_RECOVER:
         status = ERROR;
         adjust_state_change(ADJUST_S_P_INIT_RESTORE);
         break;
      case ADJUST_S_P_SET:
      case ADJUST_S_P_INIT_RESTORE:
      case ADJUST_S_COST_MEASURE:
      case ADJUST_S_PROCESS_RESULTS:
      case ADJUST_S_FRONT_BACK_RESTORE:
      case ADJUST_S_FINAL:
         //dont do anything
         break;
      case ADJUST_S_LAST:
      default :
         //DEBUG_system_dma_printf(("adjust_handle_abort impossible state"));
         break;
   }
}

/*--------------------------------------------------------------------------*/

void ADJUST_Exec (void)
{
   //send_msg5S(1,"ADJUST_Exec");

   //SERVODBG_pin_set (SERVODBG_PIN_0);

   if ( (timeout_chk(0) == eTimerTimeout) /*TIMER_is_expired (TIMER_ID_100_MS_ADJUST_WATCHDOG)*/ && (state != ADJUST_S_IDLE) && (state != ADJUST_S_FINAL) && (state != ADJUST_S_FRONT_BACK_RESTORE) && (state != ADJUST_S_P_INIT_RESTORE))
   {
      // Watch dog timeout expired restore settings.
      //DEBUG_system_dma_printf((DBG_ADJUST"WARNING timer exp in %s", adjust_state_2_str[state]));
      adjust_state_change(ADJUST_S_FRONT_BACK_RESTORE);
      // Restart watchdog timer to be able to restore front,back. whithout ending up in endless loop.
      SetTimeout(0, MS_100*6); //TIMER_start_timer (TIMER_ID_100_MS_ADJUST_WATCHDOG, 6, adjust_callback_func);
      status = ERROR;
   }

   if(abort_initiated)
   {
      adjust_handle_abort();
   }

   switch (state)
   {
      case ADJUST_S_IDLE:
         s_idle ();
         break;
      case ADJUST_S_INITIAL_SEEK:
         s_initial_seek();
         break;
      case ADJUST_S_FRONT_BACK_INIT:
         s_front_back_init ();
         break;
      case ADJUST_S_P_INIT:
         s_p_init ();
         break;
      case ADJUST_S_P_NEXT:
         s_p_next ();
         break;
      case ADJUST_S_P_SET:
         s_p_set();
         break;
      case ADJUST_S_P_INIT_RESTORE:
         s_p_init_restore ();
         break;
      case ADJUST_S_INTERM_SEEK:
         s_interm_seek();
         break;
      case ADJUST_S_RECOVER:
         s_recover ();
         break;
      case ADJUST_S_COST_INIT:
         s_cost_init ();
         break;
      case ADJUST_S_COST_MEASURE:
         s_cost_measure ();
         break;
      case ADJUST_S_PROCESS_RESULTS:
         s_process_results ();
         break;
      case ADJUST_S_FRONT_BACK_RESTORE:
         s_front_back_restore ();
         break;
      case ADJUST_S_FINAL:
         s_final();
         break;
      case ADJUST_S_LAST:
      default:
         //DEBUG_system_dma_printf((DBG_ADJUST"EXCEPT state %d nok", state));
         (void) ADJUST_Abort (NULL);
         break;
   }
   //SERVODBG_pin_clear (SERVODBG_PIN_0);
}

/*-----------------------------------------------------------------------*/

BOOL ADJUST_hf_valid_or_not_needed (ADJUST_COST_t cost, discKindT DiscKind)
{
   BOOL result;

   switch (cost)
   {
      case ADJUST_COST_STREAM_SAMPLED_JITTER:
      case ADJUST_COST_ARM_SAMPLED_JITTER:
      case ADJUST_COST_STREAM_SAMPLED_JITTER_ITEMSCAN:
      case ADJUST_COST_ARM_SAMPLED_JITTER_ITEMSCAN:
#if TO_BE_PORTED
         result = C_DEC_is_efm_pll_phase_inlock();
#else
         result = TRUE;
#endif
         if (DiscKind.Bit.DiscStd != eDiscBD)
         {
#if TO_BE_PORTED
            result = result && OPC_get_efm_status();
#endif
         }

         if (!result)
         {
            //DEBUG_printf(("HF not valid: phase %d, efm %d",
            //                          C_DEC_is_efm_pll_phase_inlock(),
            //                          OPC_get_efm_status()));
         }
         break;
      case ADJUST_COST_PUSHPULL_AMPL:
      case ADJUST_COST_WOBBLE_AMPL:
      case ADJUST_COST_PUSHPULL_AMPL_ITEMSCAN:
      case ADJUST_COST_WOBBLE_AMPL_ITEMSCAN:
      case ADJUST_COST_ABORT:
      case ADJUST_COST_NONE:
      case ADJUST_COST_LAST:
      default:
         result = TRUE;
         break;
   }
   return(result);
}

/*--------------------------------------------------------------------------*/

void ADJUST_Start (discKindT       DiscKind,
                   ESpindleSpeed   Speed,
                   FPTR            new_callback_func,
                   BYTE *          new_buffer,
                   USHORT          new_buffer_size,
                   USHORT *        new_nrof_bytes_copied,
                   ADJUST_ITEM_t   new_item1,
                   ADJUST_ITEM_t   new_item2,
                   ADJUST_COST_t   new_cost,
                   BYTE            new_config,
                   ULONG           new_start
                  )
{
   send_msg5S(1,"ADJUST_start");
   //DEBUG_printf(("ADJUST_start(item:0x%x,0x%x config:0x%x cost:0x%x addr:0x%x[%d] callback=0x%x)", new_item1, new_item2, new_config, new_cost, new_start, new_config, new_callback_func));
   new.item[0]                = new_item1;
   new.item[1]                = new_item2;
   new.cost                   = new_cost;
   new.config                 = new_config;
   new.start                  = new_start;
   new.client_callback_func   = new_callback_func;
   new.buffer                 = new_buffer;
   new.buffer_size            = new_buffer_size;
   new.nrof_bytes_copied      = new_nrof_bytes_copied;
   new.DiscKind               = DiscKind;
   new.speed                  = Speed;

#if TO_BE_PORTED
   servo_limit_layer = MISC_pba2layer(new.start,new.DiscKind.Bit.DiscStd);
#endif

   if (new.item[1] == ADJUST_ITEM_NONE)
   {//1d
      new.mode = cost2mode[new.cost];
   }
   else
   {
      new.mode = ADJUST_MODE_2D;
   }
#if TO_BE_PORTED
   SERVO_switch_preset(OFF);
#endif

   //KRNL_attach (TIMEBASE_0_MS, ADJUST_TASKID);
}

/*--------------------------------------------------------------------------*/

EStatus ADJUST_Activate (FPTR callback_func)
{
#if TO_BE_PORTED
   BE_CMD_cmd_info_t * cmd_info;
#endif
   BYTE *              buffer;
   USHORT              buffer_size;
   USHORT *            nrof_bytes_copied;
   ADJUST_ITEM_t       item1,item2;
   ADJUST_COST_t       cost;
   BYTE                config;
   ULONG               addr;

   if (active == FALSE)
   {
      //DEBUG_printf(("ADJUST_Activate"));

#if TO_BE_PORTED
      cmd_info = EXEC_get_parameters();
      buffer               = cmd_info->buffer;
      buffer_size          = cmd_info->buffer_size;
      nrof_bytes_copied    = cmd_info->nrof_bytes_copied;
      item1                = (ADJUST_ITEM_t)cmd_info->par.measure_sensitivity.item0;
      item2                = (ADJUST_ITEM_t)cmd_info->par.measure_sensitivity.item1;
      cost                 = (ADJUST_COST_t)cmd_info->par.measure_sensitivity.cost;
      config               = cmd_info->par.measure_sensitivity.config;
      addr                 = cmd_info->par.measure_sensitivity.addr;
#endif
      ADJUST_Start (svoVar_discKindT, svoIramVar.svoCurrentSpeed, callback_func, buffer, buffer_size, nrof_bytes_copied, item1, item2,
                      cost, config, addr);
      status = BUSY;
      active = TRUE;
   }
   else
   {
      if ((status == ERROR) || (status == READY))
      {
         // we're ready with measuring
         // in MEASURE_exec() or MEASURE_end(), 'status' will be set
         active = FALSE;
         //DEBUG_printf(("ADJ de-act(%s)", EStatuso_string[status]));
      }
      else
      {
         // while being active and busy, it's allowed to call MEASURE_activate() again and again...
         //DEBUG_printf(("ADJ_act:BUSY"));
      }
   }

   return (status);
}

/*--------------------------------------------------------------------------*/
//
//void ADJUST_set_coefs(ADJUST_SESSION_COEF_t *coef_p)
//{
//   memcpy(&coef, coef_p, sizeof(coef));
//}
//
/*--------------------------------------------------------------------------*/

BOOL ADJUST_get_measured_points( BYTE * buffer, USHORT buffer_size, USHORT * nrof_bytes_copied )
{
   ULONG    flank_index;
   ULONG    value_index;
   BOOL     result = FALSE;


   if (buffer_size >= (ADJUST_SIZE_OF_STORAGE / ADJUST_SIZE_DESTINATION_BUFFER))//source buffer LONG destination USHORT
   {
      //Storage large enough : Tranfer local storage to buffer
      for (flank_index = 0; flank_index < FLANK_MAX; flank_index++)
      {
         for (value_index = 0; value_index < STORAGE_MAX; value_index++)
         {
            buffer[0] = (BYTE)(data.buf[flank_index][value_index].p[0] >> 8);//lint !e704
            buffer[1] = (BYTE)(data.buf[flank_index][value_index].p[0]);
            buffer[2] = (BYTE)(data.buf[flank_index][value_index].c >> 8);//lint !e704
            buffer[3] = (BYTE)(data.buf[flank_index][value_index].c);

            //Increase destination buffer
            buffer = buffer+4;
         }
      }
      *nrof_bytes_copied = (ADJUST_SIZE_OF_STORAGE / ADJUST_SIZE_DESTINATION_BUFFER);
      result = TRUE;
   }
   return(result);
}

/*--------------------------------------------------------------------------*/

ADJUST_QUAL_t ADJUST_cost_to_qual(ADJUST_COST_t   cost)
{
   return(cost2qual[cost]);
}

/*-------------------------------------------------------------------------*/

ADJUST_LOGGING_DATA_t * ADJUST_get_log_data_p(void)
{
   return adjust_logging_data;
}

/*-------------------------------------------------------------------------*/

void ADJUST_clear_log_data(void)
{
   memset((void *)adjust_logging_data, 0, sizeof(ADJUST_LOGGING_DATA_t)*((ULONG)ADJUST_ITEM_LAST_ACTIVE+1));
}

/*-------------------------------------------------------------------------*/

void ADJUST_GetServoLimits(ADJUST_ITEM_t item, eLayers layer, ADJUST_SERVO_LIMITS_t *servo_limits)
{
   *servo_limits = adjust_servo_limits[item][layer];
}

/*-------------------------------------------------------------------------*/

void ADJUST_ClearServoLimits(void)
{
   int i;

   for ( i=0 ; i < (int) ADJUST_ITEM_LAST; i++)
   {
      adjust_servo_limits[i][eLayersSingle].determined = FALSE;
      adjust_servo_limits[i][eLayersSingle].max_measure = 0;
      adjust_servo_limits[i][eLayersSingle].min_measure = 0;
      adjust_servo_limits[i][eLayersSingle].max_optimum = 0;
      adjust_servo_limits[i][eLayersSingle].min_optimum = 0;

      adjust_servo_limits[i][eLayersDouble].determined = FALSE;
      adjust_servo_limits[i][eLayersDouble].max_measure = 0;
      adjust_servo_limits[i][eLayersDouble].min_measure = 0;
      adjust_servo_limits[i][eLayersSingle].max_optimum = 0;
      adjust_servo_limits[i][eLayersSingle].min_optimum = 0;
   }
}

/*****************************************************************************
*                       End Of File
*****************************************************************************/
