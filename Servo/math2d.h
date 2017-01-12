/****************************************************************************
*                (c) Copyright 2001,2002,2003, 2004, 2005 SUNEXT DESIGN, INC.
*                           All Rights Reserved
*
*
* FILENAME: math2d.h
*
* DESCRIPTION
* This file contains the math 2d functions implemented in the servo layer
*
* NOTES:
*
*   $Revision: 6 $
*   $Date: 08/09/05 5:25p $
*
**************************** SUNEXT CONFIDENTIAL ****************************/

#ifndef MATH2D_H
#define MATH2D_H

#include ".\oem\oem_info.h"
#include ".\common\globals.h"
#include ".\common\globtype.h"
#include ".\common\vu_cfg.h"

/*****************************************************************************
*                    Macro Definitions
*****************************************************************************/

/*****************************************************************************
*                    Inline Definitions
*****************************************************************************/

/*****************************************************************************
*                    Type Definitions
*****************************************************************************/

typedef enum
{
   PARAM0,
   PARAM1,
   NR_OF_DIMS
} EMath2D_Dimensions;

typedef enum
{
   EXP0,
   EXP1,
   EXP2,
   EXP3,
   EXP4,
   EXP5,
   EXP6,
   EXP7,
   EXP8,
   EXP9,
   EXP10,
   EXP11,
   EXP12,
   NR_OF_2D_EXPERIMENTS
} EMath2D_Experiments;

typedef enum
{
   MODE_2D_NORMAL,
   MODE_2D_ROTATED,
   NR_OF_MODE_2D
} EMath2D_Mode;

typedef struct
{
   float                       Det;
   float                       Fac[NR_OF_DIMS];
   float                       b[6];
   SHORT                       MinCost;
   BOOL                        IterationRequired;
   BYTE                        NrIterations; // only for module-test purpose
   SHORT                       Optimum[NR_OF_DIMS];
} Math2d_ResultT;

typedef struct
{
   float param_min;
   float param_max;
} Math2d_ServoLimitPair;

typedef struct 
{
   float                       b[6];
   Math2d_ServoLimitPair       Limit[NR_OF_DIMS] ;
   float                       Fac[NR_OF_DIMS];
   SHORT                       ItemOpt[NR_OF_DIMS];
} Math2d_ServoLimitsT;

typedef struct
{
   SHORT MinDiff;    // for delta sigma
   SHORT MaxDiff;    // for delta sigma
   SHORT MinDiffHV; // for HOR VER criterium, set to zero if it should not be used
   SHORT MinDiffPQ; // for PQ criterium
   SHORT MaxDiffPQ; // for PQ criterium
   SHORT DiffValidSearch;
} Math2d_MeasQualityDefT;

typedef struct
{
   BYTE             DebugIdx;  // the ws parameter we are going to tune
   SHORT            Min;       // the minimum value for this step
   SHORT            Max;       // the maximum value for this step
   SHORT            ZeroPoint; // negative zero_point is not allowed when relative = TRUE
   SHORT            DeltaFac;  // the relative factor by which the zero point is multiplied
   BOOL             Relative;  // is the delta factor absolute, or relative
} StepDefT;

typedef struct
{
   BYTE              MaxIterations; // per step, the max iterations is configurable (so that we allow the first steps to do more)
   BOOL              CostInverted;
   EMath2D_Mode      eMode;
   StepDefT          Par[NR_OF_DIMS];
} Math2d_StepT;

typedef struct
{
   SHORT Param[NR_OF_DIMS];
} Math2d_ExperimentT;

typedef struct
{
   BOOL  Valid;
   SHORT Cost; // could be jitter, missed_syncs, ... 
} Math2d_ExperimentResultT;

/*****************************************************************************
*                    Extern Data Declarations
*****************************************************************************/

/*****************************************************************************
*                    Extern Function Prototypes
*****************************************************************************/

extern void MATH2D_Init( Math2d_StepT *pStep );
extern void MATH2D_GetExperimentPlan( Math2d_ExperimentT *pExperimentPlan );
extern void MATH2D_AdjustBoundary( Math2d_StepT *pStep );
extern BOOL MATH2D_GetOptimum( Math2d_ResultT *pResult_2d , Math2d_ExperimentResultT *pExperimentResult , Math2d_MeasQualityDefT *pQualityDef );
extern void MATH2D_GetServoLimits(Math2d_ExperimentT *pMeasurePlan,
                                  Math2d_ServoLimitsT *pLimits,
                                  SHORT BoundaryLimitItem0,
                                  SHORT BoundaryLimitItem1);
#endif //MATH2D_H

/*****************************************************************************
*                    End Of File
*****************************************************************************/


/**
*   @} End of LIB_H
*/