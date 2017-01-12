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
*   $Revision: 2 $
*   $Date: 08/06/30 8:51p $
*
**************************** SUNEXT CONFIDENTIAL ****************************/

#ifndef ADJUST_H
#define ADJUST_H

#include ".\oem\oem_info.h"
#include ".\common\globals.h"
#include ".\common\globtype.h"
#include ".\common\vu_cfg.h"
#include ".\servo\SVO.H" /* Needed for types of parameters */

/*****************************************************************************
*                    Macro Definitions
*****************************************************************************/
#define  MAX_NO_ITEMS   2

/*****************************************************************************
*                    Inline Definitions
*****************************************************************************/

/*****************************************************************************
*                    Type Definitions
*****************************************************************************/

// function-pointer typedef for a function without parameters returning void
// i.e. "void func (void)"
typedef void (* FPTR)(void);

typedef struct
{
   LONG           item_optimum[MAX_NO_ITEMS];
   LONG           cost_optimum;
   BOOL           ok;
} ADJ_RESULT_t;

typedef enum
{
   EFM_MODE,
   WOBBLE_MODE,
   EFM_WOBBLE_MODE
} LOCKTYPE_t;

typedef enum
{
   ADJUST_COST_STREAM_SAMPLED_JITTER,
   ADJUST_COST_ARM_SAMPLED_JITTER,
   ADJUST_COST_PUSHPULL_AMPL,
   ADJUST_COST_WOBBLE_AMPL,
   ADJUST_COST_STREAM_SAMPLED_JITTER_ITEMSCAN,
   ADJUST_COST_ARM_SAMPLED_JITTER_ITEMSCAN,
   ADJUST_COST_PUSHPULL_AMPL_ITEMSCAN,
   ADJUST_COST_WOBBLE_AMPL_ITEMSCAN,
   ADJUST_COST_ABORT, 
   ADJUST_COST_NONE,
   ADJUST_COST_LAST
} ADJUST_COST_t;

typedef enum
{
   ADJUST_ITEM_SPHER_ABER,
   ADJUST_ITEM_TANG_TILT, 
   ADJUST_ITEM_RAD_TILT, 
   ADJUST_ITEM_FOCUS_OFFSET, 
   ADJUST_ITEM_RE_GAIN_OFFSET,
   ADJUST_ITEM_SSPP_GAIN,
   ADJUST_ITEM_RAD_OFFSET, 
   ADJUST_ITEM_WOPC, 
   ADJUST_ITEM_FOC_INT, 
   ADJUST_ITEM_RE,
   ADJUST_ITEM_REFLECTION,
   ADJUST_ITEM_FRD_REF,
   ADJUST_ITEM_ERASE_REFLECTION,
   ADJUST_ITEM_DUTYCYCLE_5_12V,
   ADJUST_ITEM_LASER_TEMP_ACT_DISP,
   ADJUST_ITEM_LASER_TRESHOLD_CURRENT,
   ADJUST_ITEM_ABORT, 
   ADJUST_ITEM_NONE,
   ADJUST_ITEM_LAST
} ADJUST_ITEM_t;

typedef struct
{
   BOOL  determined;
   float max_optimum;
   float min_optimum;
   float max_measure;
   float min_measure;
}ADJUST_SERVO_LIMITS_t;

typedef struct
{
   LONG           p_opt;
   LONG           c_opt;
   LONG           p_width;
} ADJUST_LOGGING_DATA_t;

typedef enum
{
   ADJUST_QUAL_LOW = 0,
   ADJUST_QUAL_HIGH = 1,
} ADJUST_QUAL_t ;//1bit !!

#define ADJUST_ITEM_LAST_ACTIVE ADJUST_ITEM_RAD_OFFSET

/**
 *   fields for config-parameter
 */
// bitfields 0..1
#define ADJ_CONFIG_NO_SEEK                       0x00 // No seeks at all
#define ADJ_CONFIG_INITIAL_SEEK                  0x01 // Start measurement at start location no reseeks
#define ADJ_CONFIG_RESEEK                        0x02 // Initial seek and reseeks
#define ADJ_CONFIG_FINE_RESEEK                   0x03 // Initial seek and reseeks, accurate
#define ADJ_CONFIG_SEEK_MODE_MASK                0x03

// bitfield 2
#define ADJ_CONFIG_P_OPTIMUM                     0x04 // Keep the P optimum value do not restore start P
#define ADJ_CONFIG_P_OPTIMUM_MASK                0x04

// bitfield 3
#define ADJ_CONFIG_COEF_USAGE_FROM_COMMAND       0x08 // Use coefs set by command else default coefs
#define ADJ_CONFIG_COEF_USAGE_FROM_COMMAND_MASK  0x08

#define ADJ_CONFIG_2D_MODE_ROTATED               0x10 
#define ADJ_CONFIG_2D_MODE_MASK                  0x10 
/*****************************************************************************
*                    Extern Data Declarations
*****************************************************************************/

/*****************************************************************************
*                    Extern Function Prototypes
*****************************************************************************/

extern void             ADJUST_Init       (void);
extern void             ADJUST_Exec       (void);
extern BOOL             ADJUST_hf_valid_or_not_needed (ADJUST_COST_t cost, discKindT DiscKind);
extern EStatus          ADJUST_Abort      (FPTR callback_func);
extern EStatus          ADJUST_Activate   (FPTR callback_func);
extern void             ADJUST_Start      (discKindT       DiscKind,
                                           ESpindleSpeed   Speed,
                                           FPTR            callback_func,
                                           BYTE *          buffer, 
                                           USHORT          buffer_size, 
                                           USHORT *        nrof_bytes_copied,
                                           ADJUST_ITEM_t   new_item1,
                                           ADJUST_ITEM_t   new_item2,                                           
                                           ADJUST_COST_t   cost,
                                           BYTE            config,
                                           ULONG           start
                                          );

extern EStatus          ADJUST_GetStatus  (void);
extern ADJ_RESULT_t *   ADJUST_GetResult (void);
//extern void             ADJUST_set_coefs  (ADJUST_SESSION_COEF_t *coef_p);
extern BOOL             ADJUST_get_measured_points(BYTE * buffer, USHORT buffer_size, USHORT * nrof_bytes_copied);
extern ADJUST_QUAL_t    ADJUST_cost_to_qual(ADJUST_COST_t   cost);
extern ADJUST_LOGGING_DATA_t * ADJUST_get_log_data_p(void);
extern void             ADJUST_clear_log_data(void);
extern void             ADJUST_GetServoLimits(ADJUST_ITEM_t item, eLayers layer, ADJUST_SERVO_LIMITS_t *servo_limits);

extern void             ADJUST_ClearServoLimits(void);
#endif //#ifndef ADJUST_H  
/*****************************************************************************
*                    End Of File
*****************************************************************************/
/** 
*   @} End of ADJUST_H
*/