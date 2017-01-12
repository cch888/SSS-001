/****************************************************************************
*                (c) Copyright 2001,2002,2003, 2004, 2005 SUNEXT DESIGN, INC.
*                           All Rights Reserved
*
*
* FILENAME: math2d.c
*
* DESCRIPTION
* This file contains the math 2d functions implemented in the servo layer
*
* NOTES:
*
*   $Revision: 9 $
*   $Date: 10/12/21 4:21p $
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

#include ".\servo\math2d.h"

#include <stdio.h>

/*****************************************************************************
*                    Macro Definitions
*****************************************************************************/

#define TO_BE_PORTED 0

// Depending on the resolution you need (for the calculations), you can choose between new/old method.
// This code was ported from Philips BD project and the 2 methods kept available until further
// debugging is done to check which method is the best.

#define NEW_METHOD_MIN_MAX          1     // 1: new method were it is possible to have neg/pos values for min/max bound
                                          // 0: old method, only pos values for min/max bound allowed (regarding sqrt)

#define FAC1_0                            ( (float) 1.0 )
#define FAC0_5                            ( (float) 0.5 )

/*****************************************************************************
*                    Type Definitions
*****************************************************************************/

typedef struct
{
   float MultiplyFactorParam[NR_OF_DIMS];
} MultiplyFactorT;

typedef struct
{
   SHORT       ZeroPoint;//negative ZeroPoint is not allowed when relative = TRUE
   SHORT       DeltaFac1_0;
   float       PrevStretchFactor;
   float       StretchFactor;
   SHORT       MinBound;
   SHORT       MaxBound;
   BOOL        Relative; // if true, sample = ZeroPoint * (200 + DeltaFac1_0 * StretchFactor) / (200 - DeltaFac1_0 * StretchFactor)
   BYTE 	      DebugIdx; // for printf
} ParamDefT;

typedef struct
{
   float s;  // shift factor
   float k;  // contraction (expansion) factor
   float r;  // shrink factor
   BOOL  m_min; // max border flag min
   BOOL  m_max; // max border flag max
} IterationFactors;

/****************************************************************************
*                    Static Data Declarations
****************************************************************************/

static BYTE                         mIteration;
static BYTE                         mValidSearch;
static Math2d_StepT                 mStep;
static ParamDefT                    mSequence[2];
static IterationFactors             mIndicators[2];

static MultiplyFactorT mMultiplyFactors[NR_OF_MODE_2D][NR_OF_2D_EXPERIMENTS] =
{
   {  // Normal mode
      {{       0, -FAC1_0}}, // EXP0
      {{       0,       0}}, // EXP1
      {{ -FAC0_5, +FAC0_5}}, // EXP2
      {{ +FAC1_0,       0}}, // EXP3
      {{ +FAC0_5, -FAC0_5}}, // EXP4
      {{ -FAC1_0,       0}}, // EXP5
      {{       0, +FAC1_0}}, // EXP6
      {{ -FAC0_5, -FAC0_5}}, // EXP7
      {{ +FAC0_5, +FAC0_5}}, // EXP8
      {{ -FAC1_0, -FAC1_0}}, // EXP9
      {{ -FAC1_0, +FAC1_0}}, // EXP10
      {{ +FAC1_0, +FAC1_0}}, // EXP11
      {{ +FAC1_0, -FAC1_0}}  // EXP12
   },
   {  // Rotated mode
      {{ -FAC0_5, -FAC0_5}}, // EXP0
      {{       0,       0}}, // EXP1
      {{       0, +FAC0_5}}, // EXP2
      {{ +FAC0_5, -FAC0_5}}, // EXP3
      {{       0, -FAC0_5}}, // EXP4
      {{ -FAC0_5, +FAC0_5}}, // EXP5
      {{ +FAC0_5, +FAC0_5}}, // EXP6
      {{ -FAC0_5,       0}}, // EXP7
      {{ +FAC0_5,       0}}, // EXP8
      {{ -FAC1_0,       0}}, // EXP9
      {{       0, +FAC1_0}}, // EXP10
      {{ +FAC1_0,       0}}, // EXP11
      {{       0, -FAC1_0}}  // EXP12
   }
};

// Optimalisation for FAST and SLOW adjustment parameters (for example collimator is slow)
// Step from Horizontal to vertical (zigzag)
// Item0 = FAST, Item1 = SLOW
static BYTE mExperimentalPlanOrder[NR_OF_MODE_2D][NR_OF_2D_EXPERIMENTS] =
{
   { 9,0,12,7,4,5,1,3,2,8,10,6,11 }, // Normal  mode
   { 12,0,4,3,9,7,1,8,11,5,2,6,10 }  // Rotated mode
};

static Math2d_ExperimentResultT mExperimentResult[NR_OF_2D_EXPERIMENTS]; //Only to store the results in a different order (removed from function because of stack consumption)
static char                     msExperimentPlan[2][NR_OF_2D_EXPERIMENTS * 20 + 1];
static char                     msCost[NR_OF_2D_EXPERIMENTS * 4 + 1];


/*****************************************************************************
*                    Static Function Prototypes
*****************************************************************************/

static void  CalcNewZeroPoint            ( IterationFactors *pIndicators, ParamDefT *pParamDef, SHORT *pStretchDelta );
static void  ResetIterationFactors       ( IterationFactors *pIterationFac );
static void  CalcIterationFactorsKS      ( Math2d_MeasQualityDefT *pQualityDef, float fac, float b, SHORT DeltaCost, BOOL CostInverted, IterationFactors *pIndicators);
static SHORT GetCostDirectionCriterium   ( Math2d_ExperimentResultT *pQualityVal, SHORT dir, EMath2D_Mode eMode );
static BOOL  GetPQCriterium              ( Math2d_MeasQualityDefT *pQualityDef, float det, float b4, float b5, Math2d_ExperimentResultT *pQualityVal, SHORT DeltaCost, SHORT dir , EMath2D_Mode eMode , BOOL CostInverted );
static BOOL  CheckParametersChanged      ( IterationFactors indicators1[2], IterationFactors indicators2[2], ParamDefT defs1[2], ParamDefT defs2[2]);
static void  PrintfStepInfo              ( void );
static void  CalcStretchFactor           ( IterationFactors *pIndicators, ParamDefT *pParamDef );
static BOOL  SearchForOptimum            ( Math2d_MeasQualityDefT *pQualityDef, Math2d_ExperimentResultT *pQualityVal, Math2d_ResultT *pResult, IterationFactors *pIndicators );
static void  CalcCoefs                   ( Math2d_ExperimentResultT *pQualityVal, Math2d_ResultT *pResult , EMath2D_Mode eMode );
static BOOL  IsExperimentResultsValid    ( Math2d_ExperimentResultT *pExperimentResult );
static BOOL  SearchForValidMeasurement   ( Math2d_MeasQualityDefT *pQualityDef, Math2d_ExperimentResultT *pQualityVal, IterationFactors *pIndicators, EMath2D_Mode *peMode );
static void  CalcOptimum                 ( Math2d_ResultT *pResult2d );
static BOOL  CheckWithinBoundary         ( IterationFactors *pIndicator );

/*****************************************************************************
*                    Extern Data Declarations
*****************************************************************************/

/*****************************************************************************
*                    Static Function Definitions
*****************************************************************************/

static void CalcIterationFactorsKS(Math2d_MeasQualityDefT *pQualityDef, float fac, float b, SHORT DeltaCost, BOOL CostInverted, IterationFactors *pIndicators)
{
   double AbsFac;

   if ( ( ( (b < 0) && !CostInverted ) || ( (b > 0) && CostInverted ) )  || (DeltaCost < pQualityDef->MinDiff))
   {
      pIndicators->k = 1;
      pIndicators->s = 0;
      if (DeltaCost >= pQualityDef->MinDiff)// MATH2D retry
      {//if not DeltaCost too small, b < 0 means we only got hald curve. shift to the opposite direction
         if (fac < 0)
         {
            pIndicators->s = 1;
         }
         else
         {
            pIndicators->s = -1;
         }
      }
   }
   else
   {
      pIndicators->k = 0;
      AbsFac = ABS((double)fac);
      if (AbsFac <= 1.0)
      {
         pIndicators->s = 0;
      }
      else if (AbsFac <= 1.5)
      {
         pIndicators->s = 0.75;
      }
      else
      {
         pIndicators->s = 1;
      }
      pIndicators->s = (fac < 0) ? -pIndicators->s : pIndicators->s;
   }
}

/*--------------------------------------------------------------------------*/

static void ResetIterationFactors( IterationFactors *pIterationFac )
{
   pIterationFac->k = 0;
   pIterationFac->s = 0;
   pIterationFac->r = 0;
}

/*--------------------------------------------------------------------------*/

// Formula is described in the algorithm document. It is meant to prevent toggling
// basically, it is a smart analysis of the Cost data to prevent the algorithm iterate
// in the wrong direction

static SHORT GetCostDirectionCriterium( Math2d_ExperimentResultT *pQV, SHORT dir, EMath2D_Mode eMode )
{
   SHORT CostPct;

   if ( eMode == MODE_2D_NORMAL )
   {
      // LAYOUT of experiment points (point 10 = (-1,-1), point 12 = (1,1), point 1 = (0,0)
      //EXP   NORMAL
      //EXP   10     6     11
      //          2     8
      //       5     1     3
      //          7     4
      //       9     0     12
      // This extra horizontal or vertical Cost criterium is implemented to prevent bouncing
      // (see algorithm documentation for more information)
      if (dir == 0) // horizontal
      {
         // HOR criterium
	     //Lint: without(SHORT), Loss of precision
         CostPct  = (SHORT) ABS( ABS(pQV[EXP10].Cost - pQV[EXP6].Cost ) -
                                 ABS(pQV[EXP6].Cost  - pQV[EXP11].Cost) +
                                 ABS(pQV[EXP2].Cost  - pQV[EXP8].Cost ) -
                                 ABS(pQV[EXP3].Cost  - pQV[EXP1].Cost ) +
                                 ABS(pQV[EXP5].Cost  - pQV[EXP1].Cost ) -
                                 ABS(pQV[EXP7].Cost  - pQV[EXP4].Cost ) +
                                 ABS(pQV[EXP12].Cost - pQV[EXP0].Cost ) -
                                 ABS(pQV[EXP9].Cost  - pQV[EXP0].Cost ) );
      }
      else // vertical
      {
         // VER criterium
         CostPct  = (SHORT) ABS( ABS(pQV[EXP10].Cost - pQV[EXP5].Cost) -
                                 ABS(pQV[EXP5].Cost  - pQV[EXP9].Cost) +
                                 ABS(pQV[EXP2].Cost  - pQV[EXP7].Cost) -
                                 ABS(pQV[EXP6].Cost  - pQV[EXP1].Cost) +
                                 ABS(pQV[EXP0].Cost  - pQV[EXP1].Cost) -
                                 ABS(pQV[EXP8].Cost  - pQV[EXP4].Cost) +
                                 ABS(pQV[EXP11].Cost - pQV[EXP3].Cost) -
                                 ABS(pQV[EXP3].Cost  - pQV[EXP12].Cost) );
      }
   }
   else
   {
      // LAYOUT of experiment points (point 10 = (-1,-1), point 12 = (1,1), point 1 = (0,0)
      //EXP   ROTATED
      //EXP          10
      //          5   2   6
      //      9   7   1   8   11
      //          0   4   3
      //             12
      // This extra horizontal or vertical Cost criterium is implemented to prevent bouncing
      // (see algorithm documentation for more information)
      if (dir == 0) // horizontal
      {
         // HOR criterium
	     //Lint: without(SHORT), Loss of precision
         CostPct  = (SHORT) ABS( ABS(pQV[EXP5].Cost  - pQV[EXP2].Cost ) -
                                 ABS(pQV[EXP6].Cost  - pQV[EXP2].Cost)  +
                                 ABS(pQV[EXP9].Cost  - pQV[EXP1].Cost ) -
                                 ABS(pQV[EXP11].Cost - pQV[EXP1].Cost ) +
                                 ABS(pQV[EXP0].Cost  - pQV[EXP4].Cost ) -
                                 ABS(pQV[EXP3].Cost  - pQV[EXP4].Cost ) );
      }
      else // vertical
      {
         // VER criterium
         CostPct  = (SHORT) ABS( ABS(pQV[EXP5].Cost  - pQV[EXP7].Cost) -
                                 ABS(pQV[EXP7].Cost  - pQV[EXP0].Cost) +
                                 ABS(pQV[EXP10].Cost - pQV[EXP1].Cost) -
                                 ABS(pQV[EXP12].Cost - pQV[EXP1].Cost) +
                                 ABS(pQV[EXP6].Cost  - pQV[EXP8].Cost) -
                                 ABS(pQV[EXP3].Cost  - pQV[EXP8].Cost) );
      }
   }
   return CostPct;
}

/*--------------------------------------------------------------------------*/

// Formula is described in the algorithm document. It is meant to prevent toggling
// basically, it is a smart analysis of the Cost data to prevent the algorithm iterate
// in the wrong direction

static BOOL GetPQCriterium( Math2d_MeasQualityDefT *pQualityDef, float det, float b4, float b5, Math2d_ExperimentResultT *pQV, SHORT DeltaCost, SHORT dir , EMath2D_Mode eMode , BOOL CostInverted )
{
   BOOL PorQ;
   if (dir == 0) // use Q criterium
   {
      if ( eMode == MODE_2D_NORMAL )

      {
         PorQ   = (det < 0) &&
                  ( ( (b4 > 0) && !CostInverted ) || ( (b4 < 0) && CostInverted ) ) &&
                  ( ( (b5 < 0) && !CostInverted ) || ( (b5 > 0) && CostInverted ) ) &&
                  (
                     (ABS( 2 * pQV[EXP3].Cost - pQV[EXP11].Cost - pQV[EXP12].Cost) < pQualityDef->MaxDiffPQ) ||
                     (ABS( 2 * pQV[EXP5].Cost - pQV[EXP9].Cost  - pQV[EXP10].Cost) < pQualityDef->MaxDiffPQ)
                  ) &&
                  DeltaCost > pQualityDef->MinDiffPQ;
      }
      else
      {
         PorQ   = (det < 0) &&
                  ( ( (b4 > 0) && !CostInverted ) || ( (b4 < 0) && CostInverted ) ) &&
                  ( ( (b5 < 0) && !CostInverted ) || ( (b5 > 0) && CostInverted ) ) &&
                  (
                     (ABS( 2 * pQV[EXP8].Cost - pQV[EXP6].Cost - pQV[EXP3].Cost) < pQualityDef->MaxDiffPQ) ||
                     (ABS( 2 * pQV[EXP7].Cost - pQV[EXP5].Cost - pQV[EXP0].Cost) < pQualityDef->MaxDiffPQ)
                  ) &&
                  DeltaCost > pQualityDef->MinDiffPQ;
      }
   }
   else // dir == 1 use P criterium
   {
      if ( eMode == MODE_2D_NORMAL )
      {
         PorQ   = (det < 0) &&
                  ( ( (b4 < 0) && !CostInverted ) || ( (b4 > 0) && CostInverted ) ) &&
                  ( ( (b5 > 0) && !CostInverted ) || ( (b5 < 0) && CostInverted ) ) &&
                  (
                     (ABS( 2 * pQV[EXP6].Cost - pQV[EXP10].Cost - pQV[EXP11].Cost) < pQualityDef->MaxDiffPQ) ||
                     (ABS( 2 * pQV[EXP0].Cost - pQV[EXP9].Cost  - pQV[EXP12].Cost) < pQualityDef->MaxDiffPQ)
                  ) &&
                  DeltaCost > pQualityDef->MinDiffPQ;
      }
      else
      {
         PorQ   = (det < 0) &&
                  ( ( (b4 < 0) && !CostInverted ) || ( (b4 > 0) && CostInverted ) ) &&
                  ( ( (b5 > 0) && !CostInverted ) || ( (b5 < 0) && CostInverted ) ) &&
                  (
                     (ABS( 2 * pQV[EXP2].Cost - pQV[EXP5].Cost - pQV[EXP6].Cost) < pQualityDef->MaxDiffPQ) ||
                     (ABS( 2 * pQV[EXP4].Cost - pQV[EXP3].Cost - pQV[EXP0].Cost) < pQualityDef->MaxDiffPQ)
                  ) &&
                  DeltaCost > pQualityDef->MinDiffPQ;
      }
   }
   return PorQ;
}

/*--------------------------------------------------------------------------*/

static BOOL CheckParametersChanged(IterationFactors indicators1[2], IterationFactors indicators2[2], ParamDefT defs1[2], ParamDefT defs2[2])
{
   BYTE i;

   for (i = 0 ;i < NR_OF_DIMS; i++)
   {
      SendMsgCnB(DEBUG_MATH2D_MSG,8,0x450000+i,F4B(indicators1[i].s),F4B(indicators2[i].s));
      SendMsgCnB(DEBUG_MATH2D_MSG,8,0x450004+i,F4B(defs1[i].StretchFactor),F4B(defs2[i].StretchFactor));
   }
   // check if any parameters have changed.
   return  (ABS((double)indicators1[0].s       - (double)indicators2[0].s)       > 0.1) ||
           (ABS((double)indicators1[1].s       - (double)indicators2[1].s)       > 0.1) ||
           (ABS((double)defs1[0].StretchFactor - (double)defs2[0].StretchFactor) > 0.1) ||
           (ABS((double)defs1[1].StretchFactor - (double)defs2[1].StretchFactor) > 0.1);
}

/*--------------------------------------------------------------------------*/

static void PrintfStepInfo( void )
{
   // print step info: par.name <min .. ZeroPoint +/- range * StretchFactor .. max>
   // print STEP+1 for consistency with algorithm documentation
   SendMsg80(DEBUG_MATH2D_MSG,0x450008);//MATH2D
   SendMsg80(DEBUG_MATH2D_MSG,0x450009);//STEP
   if(( mStep.eMode == MODE_2D_NORMAL ))
   {
       SendMsgCnB(DEBUG_MATH2D_MSG,1,0x45000A,B1B('N'));// Mode = N
   }
   else
   {
       SendMsgCnB(DEBUG_MATH2D_MSG,1,0x45000A,B1B('R'));// Mode = R
   }
   SendMsgCn(DEBUG_MATH2D_MSG,3,0x45000B,A1B(mIteration),A1B(mValidSearch),A1B(mStep.MaxIterations));
   // Iteration, Valid Search, Max Iterations
   SendMsg80(DEBUG_MATH2D_MSG,0x45000E);//PARAM0
   SendMsgCn(DEBUG_MATH2D_MSG,3,0x450108,A1B(mSequence[0].DebugIdx),A1B(mSequence[0].MinBound),A1B(mSequence[0].ZeroPoint));
   // debug idx, Bound Min, Zero Point
   SendMsgCn(DEBUG_MATH2D_MSG,4,0x45000C,A1B(mSequence[0].DeltaFac1_0),A1B(mSequence[0].Relative),A1B(mSequence[0].StretchFactor),A1B(mSequence[0].MaxBound));
   // Delta Factor 1-0, Relative, Stretch Factor, Bound Max
   SendMsg80(DEBUG_MATH2D_MSG,0x45000F);//PARAM1
   SendMsgCn(DEBUG_MATH2D_MSG,3,0x450108,A1B(mSequence[0].DebugIdx),A1B(mSequence[1].MinBound),A1B(mSequence[1].ZeroPoint));
   SendMsgCn(DEBUG_MATH2D_MSG,4,0x45000C,A1B(mSequence[1].DeltaFac1_0),A1B(mSequence[1].Relative),A1B(mSequence[1].StretchFactor),A1B(mSequence[1].MaxBound));
//   DEBUG_system_dma_printf(("MATH2D STEP %s Opt %d Search %d (Max %d) PARAM0(idx %d) :<%d..(%d+/-%d%s)*%4.2f..%d> , PARAM1(idx %d) :<%d..(%d+/-%d%s)*%4.2f..%d>",
//      ( mStep.eMode == MODE_2D_NORMAL ) ? "N" : "R", mIteration, mValidSearch, mStep.MaxIterations,
//      mSequence[0].DebugIdx, mSequence[0].MinBound, mSequence[0].ZeroPoint,
//      mSequence[0].DeltaFac1_0, mSequence[0].Relative? "%":"",
//      mSequence[0].StretchFactor, mSequence[0].MaxBound,
//
//      mSequence[1].DebugIdx, mSequence[1].MinBound, mSequence[1].ZeroPoint,
//      mSequence[1].DeltaFac1_0, mSequence[1].Relative? "%":"",
//      mSequence[1].StretchFactor, mSequence[1].MaxBound));
}

/*--------------------------------------------------------------------------*/

static BOOL SearchForOptimum( Math2d_MeasQualityDefT *pQualityDef, Math2d_ExperimentResultT *pQualityVal, Math2d_ResultT *pResult, IterationFactors *pIndicators )
{
   BYTE  i;
   BOOL  pq[2];
   SHORT CostDir[2];
   SHORT DeltaCost;
   SHORT MinCost, MaxCost;

   MinCost = 255;
   MaxCost = 0;

   pq[0]      = pq[1]      = FALSE;
   CostDir[0] = CostDir[1] = 0;

   for (i = 0; i < NR_OF_2D_EXPERIMENTS; i++)
   {
      MinCost = (pQualityVal[i].Cost < MinCost) ? pQualityVal[i].Cost : MinCost;
      MaxCost = (pQualityVal[i].Cost > MaxCost) ? pQualityVal[i].Cost : MaxCost;
   }
   DeltaCost = MaxCost - MinCost;

   ResetIterationFactors(&pIndicators[0]);
   ResetIterationFactors(&pIndicators[1]);

   if (pResult->Det < 0.0)  /* && ((b4 * b5) > 0.0)) */ //still don't known what to do with this part in comment /* && ((b4 * b5) > 0.0)) */
   {//strange result, increase scan range on both directions.
      pIndicators[0].k = 1;
      pIndicators[1].k = 1;
   }
   else
   {
      CalcIterationFactorsKS( pQualityDef, pResult->Fac[0], pResult->b[4], DeltaCost, mStep.CostInverted, &pIndicators[0] );
      CalcIterationFactorsKS( pQualityDef, pResult->Fac[1], pResult->b[5], DeltaCost, mStep.CostInverted, &pIndicators[1] );
   }

   // If DeltaCost > max AND jitter (HOR or VER) > min
   // The GetCostDirectionCriterium and GetPQCriterium are
   // refinements to prevent bouncing/toggling

   for (i = 0; i < NR_OF_DIMS; i++)
   {
      CostDir[i] = GetCostDirectionCriterium( pQualityVal, i , mStep.eMode );
      pq[i]       = GetPQCriterium( pQualityDef, pResult->Det, pResult->b[4], pResult->b[5], pQualityVal, DeltaCost, i , mStep.eMode , mStep.CostInverted );
      // K = 1
      // if [(delta_jit > 10% & HOR > 5)] OR Q=true
      //
      // if [(delta_jit > 10% & VER > 5)] OR P=true
      //

      if ( pIndicators[i].k != 0) // Check if K1 or K2 == 1
      {
         if (
               ((DeltaCost > pQualityDef->MaxDiff) && (CostDir[i] > pQualityDef->MinDiffHV))
               ||
               pq[i] // added P/Q criterium, an extra test to prevent bouncing
            )
         {
            pIndicators[i].r = 1;
            pIndicators[i].k = 0;
         }
      }
   }
   SendMsg80(DEBUG_MATH2D_MSG,0x450008);//MATH2D
   SendMsgCn(DEBUG_MATH2D_MSG,5,0x450305,A1B(DeltaCost),A1B(pq[0]),A1B(pq[1]),A1B(CostDir[0]),A1B(CostDir[1]));
   SendMsgCnB(DEBUG_MATH2D_MSG,12,0x450306,F4B(pIndicators[0].r),F4B(pIndicators[0].k),F4B(pIndicators[0].s));
   SendMsgCnB(DEBUG_MATH2D_MSG,12,0x450307,F4B(pIndicators[1].r),F4B(pIndicators[1].k),F4B(pIndicators[1].s));
   //DEBUG_system_dma_printf(("MATH2D R1=%.2f K1=%.2f S1=%.2f R2=%.2f K2=%.2f S2=%.2f", pIndicators[0].r, pIndicators[0].k, pIndicators[0].s,
   //                                                                                   pIndicators[1].r, pIndicators[1].k, pIndicators[1].s));

   return (pIndicators[0].r != 0) || (pIndicators[0].k != 0) || (pIndicators[0].s != 0) ||
          (pIndicators[1].r != 0) || (pIndicators[1].k != 0) || (pIndicators[1].s != 0);
}

/*--------------------------------------------------------------------------*/

static BOOL CheckWithinBoundary( IterationFactors *pIndicator )
{
   BOOL result = TRUE;

   if ( ( pIndicator->k != 0 ) && ( ( pIndicator->m_min ) && ( pIndicator->m_max ) ) )
   {
      result = FALSE;
   }
   else if ( ( pIndicator->s < 0 ) && ( pIndicator->r == 0 ) && ( pIndicator->m_min ) )
   {
      result = FALSE;
   }
   else if ( ( pIndicator->s > 0 ) && ( pIndicator->r == 0 ) && ( pIndicator->m_max ) )
   {
      result = FALSE;
   }
   SendMsgCnB(DEBUG_MATH2D_MSG,12,0x450400,F4B(pIndicator->r),F4B(pIndicator->k),F4B(pIndicator->s));
   SendMsgCn(DEBUG_MATH2D_MSG,3,0x450401,A1B(result),A1B(pIndicator->m_min),A1B(pIndicator->m_max));
   //DEBUG_system_dma_printf(("MATH2D CheckWithinBoundary result=%d k=%0.2f s=%0.2f r=%0.f min=%d max=%d", result, indicator->k, indicator->s, indicator->r, indicator->m_min, indicator->m_max ));
   return result;
}

/*--------------------------------------------------------------------------*/

static BOOL SearchForValidMeasurement( Math2d_MeasQualityDefT *pQualityDef, Math2d_ExperimentResultT *pQualityVal, IterationFactors *pIndicators, EMath2D_Mode *peMode )
{
   BYTE i;
   BOOL  vw, ve, hn, hs;
   SHORT DeltaCost;
   SHORT MinCost, MaxCost;
   BOOL  invalid_we = FALSE;
   BOOL  invalid_ns = FALSE;
   BOOL  result = TRUE;

   MinCost = 255;
   MaxCost = 0;

   ResetIterationFactors(&pIndicators[0]);
   ResetIterationFactors(&pIndicators[1]);

   for (i = 0; i < NR_OF_2D_EXPERIMENTS; i++)
   {
      if ( pQualityVal[i].Valid )
      {
         MinCost = (pQualityVal[i].Cost < MinCost) ? pQualityVal[i].Cost : MinCost;
         MaxCost = (pQualityVal[i].Cost > MaxCost) ? pQualityVal[i].Cost : MaxCost;
      }
   }
   DeltaCost = MaxCost - MinCost;

   if ( ( !pQualityVal[EXP10].Valid || !pQualityVal[EXP12].Valid || !pQualityVal[EXP9].Valid || !pQualityVal[EXP11].Valid ) &&
        ( pQualityVal[EXP0].Valid   &&  pQualityVal[EXP3].Valid  &&  pQualityVal[EXP5].Valid &&  pQualityVal[EXP6].Valid  ) &&
        ( *peMode == MODE_2D_NORMAL ) )
   {
      *peMode = MODE_2D_ROTATED;
      result   = TRUE;
   }
   else
   {
      if ( *peMode == MODE_2D_NORMAL )
      {
         vw = ( !pQualityVal[EXP10].Valid || !pQualityVal[EXP5].Valid || !pQualityVal[EXP9].Valid  ) ? TRUE : FALSE;
         ve = ( !pQualityVal[EXP11].Valid || !pQualityVal[EXP3].Valid || !pQualityVal[EXP12].Valid ) ? TRUE : FALSE;
         hn = ( !pQualityVal[EXP10].Valid || !pQualityVal[EXP6].Valid || !pQualityVal[EXP11].Valid ) ? TRUE : FALSE;
         hs = ( !pQualityVal[EXP9].Valid  || !pQualityVal[EXP0].Valid || !pQualityVal[EXP12].Valid ) ? TRUE : FALSE;
      }
      else
      {
         vw = ( !pQualityVal[EXP9].Valid  ) ? TRUE : FALSE;
         ve = ( !pQualityVal[EXP11].Valid ) ? TRUE : FALSE;
         hn = ( !pQualityVal[EXP10].Valid ) ? TRUE : FALSE;
         hs = ( !pQualityVal[EXP12].Valid ) ? TRUE : FALSE;
      }
      if ( !vw && ve )
      {
         if ( DeltaCost > pQualityDef->DiffValidSearch )
         {
            pIndicators[0].s = -0.25;
            pIndicators[0].r = 0.354; // = sqrt(2)/4
         }
         else
         {
            pIndicators[0].s = -1;
         }
      }
      else if ( vw && !ve )
      {
         if ( DeltaCost > pQualityDef->DiffValidSearch )
         {
            pIndicators[0].s = 0.25;
            pIndicators[0].r = 0.354; // = sqrt(2)/4
         }
         else
         {
            pIndicators[0].s = 1;
         }
      }
      else if ( vw && ve )
      {
         pIndicators[0].r = 1;
      }
      else
      {
         invalid_we = TRUE;
      }
      if ( !hn && hs )
      {
         if ( DeltaCost > pQualityDef->DiffValidSearch )
         {
            pIndicators[1].s = 0.25;
            pIndicators[1].r = 0.354; // = sqrt(2)/4
         }
         else
         {
            pIndicators[1].s = 1;
         }
      }
      else if ( hn && !hs )
      {
         if ( DeltaCost > pQualityDef->DiffValidSearch )
         {
            pIndicators[1].s = -0.25;
            pIndicators[1].r = 0.354; // = sqrt(2)/4
         }
         else
         {
            pIndicators[1].s = -1;
         }
      }
      else if ( hn && hs )
      {
         pIndicators[1].r = 1;
      }
      else
      {
         invalid_ns = TRUE;
      }
      result = ( invalid_we && invalid_ns ) ? FALSE : TRUE;
   }

   SendMsg80(DEBUG_MATH2D_MSG,0x450008);//MATH2D
   SendMsgCn(DEBUG_MATH2D_MSG,5,0x450406,A1B(*peMode));
   SendMsgCnB(DEBUG_MATH2D_MSG,12,0x450306,F4B(pIndicators[0].r),F4B(pIndicators[0].k),F4B(pIndicators[0].s));
   SendMsgCnB(DEBUG_MATH2D_MSG,12,0x450307,F4B(pIndicators[1].r),F4B(pIndicators[1].k),F4B(pIndicators[1].s));
   //DEBUG_system_dma_printf(("MATH2D R1=%.2f K1=%.2f S1=%.2f R2=%.2f K2=%.2f S2=%.2f", pIndicators[0].r, pIndicators[0].k, pIndicators[0].s,
   //                                                                                   pIndicators[1].r, pIndicators[1].k, pIndicators[1].s));
   return result;
}

/*--------------------------------------------------------------------------*/

static void CalcCoefs( Math2d_ExperimentResultT *pQualityVal, Math2d_ResultT *pResult , EMath2D_Mode eMode )
{
   float  b0, b1, b2, b3, b4, b5, Fac1, Fac2, Det;

   if ( eMode == MODE_2D_NORMAL )
   {
      b0 = (float)(( (pQualityVal[EXP0].Cost + pQualityVal[EXP3].Cost + pQualityVal[EXP5].Cost + pQualityVal[EXP6].Cost - pQualityVal[EXP9].Cost - pQualityVal[EXP10].Cost - pQualityVal[EXP11].Cost - pQualityVal[EXP12].Cost)
                    + 2.0 * (pQualityVal[EXP2].Cost + pQualityVal[EXP4].Cost + pQualityVal[EXP7].Cost + pQualityVal[EXP8].Cost) + 3.0 * pQualityVal[EXP1].Cost ) / 11.0);
      b1 = (float)((-(pQualityVal[EXP2].Cost - pQualityVal[EXP4].Cost + pQualityVal[EXP7].Cost - pQualityVal[EXP8].Cost)
                   + 2.0 * (pQualityVal[EXP3].Cost - pQualityVal[EXP5].Cost - pQualityVal[EXP9].Cost - pQualityVal[EXP10].Cost + pQualityVal[EXP11].Cost + pQualityVal[EXP12].Cost) ) / 14.0);
      b2 = (float)(( (pQualityVal[EXP2].Cost + pQualityVal[EXP8].Cost - pQualityVal[EXP4].Cost - pQualityVal[EXP7].Cost)
                   - 2.0 * (pQualityVal[EXP0].Cost - pQualityVal[EXP6].Cost + pQualityVal[EXP9].Cost - pQualityVal[EXP10].Cost - pQualityVal[EXP11].Cost + pQualityVal[EXP12].Cost) ) / 14.0);
      b3 = (float)(( (pQualityVal[EXP7].Cost + pQualityVal[EXP8].Cost - pQualityVal[EXP2].Cost - pQualityVal[EXP4].Cost)
                   + 4.0 * (pQualityVal[EXP9].Cost - pQualityVal[EXP10].Cost + pQualityVal[EXP11].Cost - pQualityVal[EXP12].Cost) ) / 17.0);
      b4 = (float)(- 0.26299 * (pQualityVal[EXP0].Cost + pQualityVal[EXP6].Cost) - 2.0 * pQualityVal[EXP1].Cost / 11.0
                   - 0.09740 * (pQualityVal[EXP2].Cost + pQualityVal[EXP4].Cost + pQualityVal[EXP7].Cost + pQualityVal[EXP8].Cost)
                   + 0.15584 * (pQualityVal[EXP9].Cost + pQualityVal[EXP10].Cost + pQualityVal[EXP11].Cost + pQualityVal[EXP12].Cost) + 0.23701 * (pQualityVal[EXP3].Cost + pQualityVal[EXP5].Cost));
      b5 = (float)(- 0.26299 * (pQualityVal[EXP3].Cost + pQualityVal[EXP5].Cost) - 2.0 * pQualityVal[EXP1].Cost / 11.0
                   - 0.09740 * (pQualityVal[EXP2].Cost + pQualityVal[EXP4].Cost + pQualityVal[EXP7].Cost + pQualityVal[EXP8].Cost)
                   + 0.15584 * (pQualityVal[EXP9].Cost + pQualityVal[EXP10].Cost + pQualityVal[EXP11].Cost + pQualityVal[EXP12].Cost) + 0.23701 * (pQualityVal[EXP0].Cost + pQualityVal[EXP6].Cost));
   }
   else
   {
      b0 = (float)(( (pQualityVal[EXP0].Cost + pQualityVal[EXP3].Cost + pQualityVal[EXP5].Cost + pQualityVal[EXP6].Cost - pQualityVal[EXP9].Cost - pQualityVal[EXP10].Cost - pQualityVal[EXP11].Cost - pQualityVal[EXP12].Cost)
                    + 2.0 * (pQualityVal[EXP2].Cost + pQualityVal[EXP4].Cost + pQualityVal[EXP7].Cost + pQualityVal[EXP8].Cost) + 3.0 * pQualityVal[EXP1].Cost ) / 11.0);
      b1 = (float)(( (pQualityVal[EXP3].Cost - pQualityVal[EXP0].Cost + pQualityVal[EXP6].Cost - pQualityVal[EXP5].Cost + pQualityVal[EXP8].Cost - pQualityVal[EXP7].Cost)
                   + 2.0 * (pQualityVal[EXP11].Cost - pQualityVal[EXP9].Cost) ) / 7.0);
      b2 = (float)(( (pQualityVal[EXP2].Cost - pQualityVal[EXP0].Cost - pQualityVal[EXP3].Cost - pQualityVal[EXP4].Cost + pQualityVal[EXP5].Cost + pQualityVal[EXP6].Cost )
                   + 2.0 * (pQualityVal[EXP10].Cost - pQualityVal[EXP12].Cost) ) / 7.0);
      b3 = pQualityVal[EXP0].Cost + pQualityVal[EXP6].Cost - pQualityVal[EXP3].Cost - pQualityVal[EXP5].Cost;
      b4 = (float)(- 2.0 * (pQualityVal[EXP0].Cost + pQualityVal[EXP3].Cost + pQualityVal[EXP5].Cost + pQualityVal[EXP6].Cost) / 77.0
                   - 4.0 * pQualityVal[EXP1].Cost / 11.0 - 0.2536287 * (pQualityVal[EXP2].Cost + pQualityVal[EXP4].Cost) - 0.1359817 * (pQualityVal[EXP7].Cost + pQualityVal[EXP8].Cost)
                   + 0.5469824 * (pQualityVal[EXP9].Cost + pQualityVal[EXP11].Cost) + 0.0763942 * (pQualityVal[EXP10].Cost + pQualityVal[EXP12].Cost));
      b5 = (float)(- 2.0 * (pQualityVal[EXP0].Cost + pQualityVal[EXP3].Cost + pQualityVal[EXP5].Cost + pQualityVal[EXP6].Cost) / 77.0
                   - 4.0 * pQualityVal[EXP1].Cost / 11.0  - 0.2536287 * (pQualityVal[EXP7].Cost + pQualityVal[EXP8].Cost) - 0.1359817 * (pQualityVal[EXP2].Cost + pQualityVal[EXP4].Cost)
                   + 0.5469824 * (pQualityVal[EXP10].Cost + pQualityVal[EXP12].Cost) + 0.0763942 * (pQualityVal[EXP9].Cost + pQualityVal[EXP11].Cost));
   }
   Det                =   (b4 * b5 - b3 * b3 / 4.0);
   Fac1               = - (b5 * b1 - b3 * b2 / 2.0) / (2.0 * Det);
   Fac2               = - (b4 * b2 - b3 * b1 / 2.0) / (2.0 * Det);
   pResult->Det        = Det;
   pResult->Fac[0]     = Fac1;
   pResult->Fac[1]     = Fac2;
   pResult->MinCost   = (SHORT)(b0 + b1 * Fac1 + b2 * Fac2 + b3 * Fac1 * Fac2 + b4 * Fac1 * Fac1 + b5 * Fac2 * Fac2);
   pResult->b[0]       = b0;
   pResult->b[1]       = b1;
   pResult->b[2]       = b2;
   pResult->b[3]       = b3;
   pResult->b[4]       = b4;
   pResult->b[5]       = b5;
   SendMsg80(DEBUG_MATH2D_MSG,0x450008);//MATH2D
   SendMsgCn(DEBUG_MATH2D_MSG,5,0x450407,A1B(pResult->MinCost));
   SendMsgCnB(DEBUG_MATH2D_MSG,12,0x450408,F4B(b0),F4B(b1),F4B(b2));
   SendMsgCnB(DEBUG_MATH2D_MSG,12,0x45040B,F4B(b3),F4B(b4),F4B(b5));
   SendMsgCnB(DEBUG_MATH2D_MSG,12,0x45040E,F4B(Det),F4B(Fac1),F4B(Fac2));
   //DEBUG_system_dma_printf(("MATH2D MinCost=%d coefs: b0~5=[%.2e %.2e %.2e %.2e %.2e %.2e] det=%.2e fac1=%.2f fac2=%.2f",
   //                         result->MinCost, b0, b1, b2, b3, b4, b5, det, fac1, fac2));
}

/*--------------------------------------------------------------------------*/

static BOOL IsExperimentResultsValid( Math2d_ExperimentResultT *pExperimentResult )
{
   BYTE i;
   BOOL Valid = TRUE;
   for ( i = 0 ; i < NR_OF_2D_EXPERIMENTS ; i++ )
   {
      Valid &= pExperimentResult[i].Valid;
   }

   return Valid;
}

/*--------------------------------------------------------------------------*/

void GetExperimentFactors(EMath2D_Experiments ExperimentNr, float fac[NR_OF_DIMS], EMath2D_Mode eMode )
{
   BYTE i;

   for (i = 0; i < NR_OF_DIMS; i++)
   {
      fac[i] = mMultiplyFactors[eMode][ExperimentNr].MultiplyFactorParam[i];
   }
}

/*--------------------------------------------------------------------------*/

static void CalcOptimum( Math2d_ResultT *pResult2d )
{
   BYTE i;

   // No iteration is required, we found an Optimum
   for (i = 0; i < NR_OF_DIMS; i++)
   {
      // calculate the real ws values:
      // ZeroPoint + StretchFactor * Optimum * range
      // and store it back on the correct location in the ws (use index '->i' ws access)
      if (!mSequence[i].Relative)
      {
         pResult2d->Optimum[i] = mSequence[i].ZeroPoint + float2short(mSequence[i].StretchFactor * pResult2d->Fac[i] * mSequence[i].DeltaFac1_0);
      }
      else
      {
#if ( NEW_METHOD_MIN_MAX == 1 )
         pResult2d->Optimum[i] = float2short((float)((float)mSequence[i].ZeroPoint * (100. + mSequence[i].StretchFactor * pResult2d->Fac[i] * mSequence[i].DeltaFac1_0) /
                                                          ((float)100. )));
#else
         pResult2d->Optimum[i] = float2short((float)((float)mSequence[i].ZeroPoint * (200. + mSequence[i].StretchFactor * pResult2d->Fac[i] * mSequence[i].DeltaFac1_0) /
                                                          ((float)200. - mSequence[i].StretchFactor * pResult2d->fac[i] * mSequence[i].DeltaFac1_0)));
#endif
      }
      if (pResult2d->Optimum[i] > mSequence[i].MaxBound)
      {
         pResult2d->Optimum[i] = mSequence[i].MaxBound;
         //DEBUG_system_dma_printf(("MATH2D opt%d CLIPPED TO MAX BOUNDARY",i));
      }

      if (pResult2d->Optimum[i] < mSequence[i].MinBound)
      {
         pResult2d->Optimum[i] = mSequence[i].MinBound;
         //DEBUG_system_dma_printf(("MATH2D opt%d CLIPPED TO MIN BOUNDARY",i));
      }
      SendMsg80(DEBUG_MATH2D_MSG,0x450501+i);//MATH2D OPT PARAM0 / MATH2D OPT PARAM1
      SendMsgCnB(DEBUG_MATH2D_MSG,12,0x450104,F4B(mSequence[i].StretchFactor),F4B(pResult2d->Fac[i]),F4B(mSequence[i].DeltaFac1_0));
      SendMsgCn(DEBUG_MATH2D_MSG,1,0x450103,A1B(mSequence[i].Relative));
      //DEBUG_system_dma_printf(("MATH2D OPT PARAM%d=%d SF=%f fac=%.2f delta=%d%s", i,pResult2d->Optimum[i],
      //                         mSequence[i].StretchFactor, pResult2d->fac[i], mSequence[i].DeltaFac1_0,
      //                         mSequence[i].Relative? "%":""));
      // reset stretch factor (because we go to a next STEP, and start with the first iteration)
      mSequence[i].StretchFactor = 1.0;
   }
}

/*--------------------------------------------------------------------------*/

static void CalcNewZeroPoint( IterationFactors *pIndicators, ParamDefT *pParamDef, SHORT *pStretchDelta)
{
   SHORT delta, OldParMin = 0, OldParMax = 0;
   SHORT NewParMin, NewParMax, Shift = 0;
   SHORT PrevStretchDeltaFac1_0;
   SHORT StretchDeltaFac1_0;

   // in case of rotated mode, calculate the same distance between 0-0.5 and 0.5-1
   // in case of rotated mode, calculate the same distance between 0-0.5 and 0.5-1
   PrevStretchDeltaFac1_0 = (SHORT) ( 2 * (SHORT)((pParamDef->PrevStretchFactor * pParamDef->DeltaFac1_0 + 1) / 2));
   StretchDeltaFac1_0     = (SHORT) ( 2 * (SHORT)((pParamDef->StretchFactor     * pParamDef->DeltaFac1_0 + 1) / 2));
   // purpose: StretchDeltaFac1_0 always >0, in steps 2,4,6,8.

   pIndicators->m_min = FALSE;
   pIndicators->m_max = FALSE;

   // calc previous border settings
   OldParMin = pParamDef->ZeroPoint - PrevStretchDeltaFac1_0;
   OldParMax = pParamDef->ZeroPoint + PrevStretchDeltaFac1_0;

   if (!pParamDef->Relative)
   {
      SendMsg80(DEBUG_MATH2D_MSG,0x450106); //MATH2D: absolute mode
      // calc new border settings
      NewParMin = pParamDef->ZeroPoint - StretchDeltaFac1_0 + (SHORT)(pIndicators->s * pParamDef->DeltaFac1_0);
      NewParMax = (SHORT)(NewParMin + 2 * StretchDeltaFac1_0);

      delta = NewParMax - NewParMin;
      // note: in this case delta in unit 4,8,12,16. Always possible to find equal distance 0 - 0.5 and 0.5 - 1
   		// in this case problem solved with negative zero-points and unwanted rounding up.
   		// makes window symmetric and modulo integer_4. Also solve very small windows delta < 4 (for example Tmp in BDR N-1 at 2x)

      if (NewParMin <= pParamDef->MinBound) //include boundary too
      {
         pIndicators->m_min = TRUE;
         NewParMin       = pParamDef->MinBound;
         NewParMax       = pParamDef->MinBound + delta;
      }
      else if (NewParMax >= pParamDef->MaxBound) //include boundary too
      {
         pIndicators->m_max = TRUE;
         NewParMin       = pParamDef->MaxBound - delta;
         NewParMax       = pParamDef->MaxBound;
      }
      if ((NewParMin <= pParamDef->MinBound) && (NewParMax >= pParamDef->MaxBound))
      {
         pIndicators->m_min = TRUE;
         pIndicators->m_max = TRUE;
      	NewParMin       = pParamDef->MinBound;
         NewParMax       = pParamDef->MaxBound;
         //"old code" pParamDef->StretchFactor = (float) ( (pParamDef->StretchFactor / delta) * 4 * (SHORT)((pParamDef->MaxBound - pParamDef->MinBound )/4) );
         delta = (SHORT)(4 * ((NewParMax - NewParMin) / 4));
       	NewParMax = NewParMin  + delta;
         StretchDeltaFac1_0 = delta / 2;
       	// In this case the stretch factor is reduced (because of clipping)
         // the calculated area size was 'delta', but is reduced to '..max - ..min' modulo 4
         // idea: center new_ZeroPoint in the middle of min_max_boundary
         // then actual min_max boundary must be integer_2 steps away from zero-point --> stretch factor must be rounddown modulo integer_4
      }
      // Make sure when we shift the area, there is no gap between the old
      // and the new area
      if (NewParMax < OldParMin)
      //      o--------------o         o---------------o
      //      |              |         |               |
      //   NewParMin  NewParMax  OldParMin       OldParMax
      {
         Shift = OldParMin - NewParMax;
      }
      else if (NewParMin > OldParMax)
      //      o--------------o          o----------------o
      //      |              |          |                |
      //   OldParMin  OldParMax  NewParMin           NewParMax
      {
         Shift = OldParMax - NewParMin;
      }
      // NewParMin - NewParMax = 4 x N --> average value is always integer
      pParamDef->ZeroPoint = (NewParMin + NewParMax) / 2 + Shift;

      // correct StretchFactor for the new modulo_4 NewParMin & max
   	pParamDef->StretchFactor = (float) StretchDeltaFac1_0 / pParamDef->DeltaFac1_0;
   }
   else
   {
      SendMsg80(DEBUG_MATH2D_MSG,0x450107); //MATH2D: relative mode

#if ( NEW_METHOD_MIN_MAX == 1 )
      pParamDef->ZeroPoint = float2short((float)pParamDef->ZeroPoint * ((float)100 + pIndicators->s * (float)pParamDef->DeltaFac1_0) / ((float)100 ));
      NewParMin = (SHORT)((float)pParamDef->ZeroPoint * ((float)100 - StretchDeltaFac1_0) / ((float)100 ));
      NewParMax = (SHORT)((float)pParamDef->ZeroPoint * ((float)100 + StretchDeltaFac1_0) / ((float)100 ));
#else
      pParamDef->ZeroPoint = float2short((float)pParamDef->ZeroPoint * ((float)200 + pIndicators->s * (float)pParamDef->DeltaFac1_0) / ((float)200 - pIndicators->s * (float)pParamDef->DeltaFac1_0));
      NewParMin = (SHORT)((float)pParamDef->ZeroPoint * ((float)200 - StretchDeltaFac1_0) / ((float)200 + StretchDeltaFac1_0));
      NewParMax = (SHORT)((float)pParamDef->ZeroPoint * ((float)200 + StretchDeltaFac1_0) / ((float)200 - StretchDeltaFac1_0));
#endif

      if (NewParMin < pParamDef->MinBound)
      {
         pIndicators->m_min       = TRUE;
         pParamDef->ZeroPoint   = (SHORT)RoundDivide((LONG)pParamDef->ZeroPoint * pParamDef->MinBound, (LONG)NewParMin);
         NewParMax             = (SHORT)((LONG)NewParMax * pParamDef->MinBound / NewParMin);
         NewParMin             = pParamDef->MinBound;
      }
      else if (NewParMax > pParamDef->MaxBound)
      {
         pIndicators->m_max       = TRUE;
         pParamDef->ZeroPoint   = (SHORT)RoundDivide((LONG)pParamDef->ZeroPoint * pParamDef->MaxBound, (LONG)NewParMax);
         NewParMin             = (SHORT)((LONG)NewParMin * pParamDef->MaxBound / NewParMax);
         NewParMax             = pParamDef->MaxBound;
      }
      if ((NewParMin <= pParamDef->MinBound) && (NewParMax >= pParamDef->MaxBound))
      {
         pIndicators->m_min       = TRUE;
         pIndicators->m_max       = TRUE;
         NewParMin             = pParamDef->MinBound;
         NewParMax             = pParamDef->MaxBound;
         // In this case the stretch factor is reduced (because of clipping)
         // the calculated area size was 'delta', but is reduced to '..max - ..min'

#if ( NEW_METHOD_MIN_MAX == 1 )
         pParamDef->ZeroPoint     = (NewParMin + NewParMax + 1) / 2;
         pParamDef->StretchFactor = (float)(pParamDef->MaxBound - pParamDef->ZeroPoint) /
                                     (float)(pParamDef->DeltaFac1_0 );
#else
         pParamDef->ZeroPoint     = (SHORT)sqrt((ULONG)(pParamDef->MinBound * pParamDef->MaxBound));
         pParamDef->StretchFactor = (float)(200 * (pParamDef->MaxBound - pParamDef->ZeroPoint)) /
                                     (float)(pParamDef->DeltaFac1_0 * (pParamDef->MaxBound + pParamDef->ZeroPoint));
#endif

      }
      // Make sure when we shift the area, there is no gap between the old
      // and the new area
      if (NewParMax < OldParMin)
      //      o--------------o         o---------------o
      //      |              |         |               |
      //   NewParMin  NewParMax  OldParMin       OldParMax
      {
         pParamDef->ZeroPoint = (SHORT)((LONG)pParamDef->ZeroPoint * OldParMin / NewParMax);
         Shift = (SHORT)((LONG)100 * (OldParMin - NewParMax) / NewParMax);
      }
      else if (NewParMin > OldParMax)
      //      o--------------o          o----------------o
      //      |              |          |                |
      //   OldParMin  OldParMax  NewParMin           NewParMax
      {
         pParamDef->ZeroPoint = (SHORT)((LONG)pParamDef->ZeroPoint * OldParMax / NewParMin);
         Shift = (SHORT)((LONG)100 * (OldParMax - NewParMin) / NewParMin);
      }
   }
   *pStretchDelta = StretchDeltaFac1_0;
   SendMsgCnB(DEBUG_MATH2D_MSG,7,0x450505,A1B(pIndicators->m_min),A1B(pIndicators->m_max),A1B(Shift),F4B(pIndicators->s));
   SendMsgCn(DEBUG_MATH2D_MSG,4,0x450508,A1B(OldParMin),A1B(OldParMax),A1B(NewParMin),A1B(NewParMax));
   SendMsgCnB(DEBUG_MATH2D_MSG,5,0x450504,A1B(pParamDef->ZeroPoint),F4B(pParamDef->StretchFactor));
   //DEBUG_system_dma_printf(("MATH2D Final stretch factor=%.2f", pParamDef->StretchFactor));
}

/*--------------------------------------------------------------------------*/

static void CalcStretchFactor( IterationFactors *pIndicators, ParamDefT *pParamDef )
{
   double Diff;

   if ((pIndicators->r != 0) || (pIndicators->k != 0) || (pIndicators->s != 0)) // only if an indicator set
   {
      pParamDef->PrevStretchFactor = pParamDef->StretchFactor;

      pIndicators->s = (float) ( pParamDef->StretchFactor * pIndicators->s );

      // calculate new stretch pParamDef
      // occurrence both k and r not 0 is impossible                          // see Generic2D doc v0.2/parag 4.3
      // Float equality doesn't work, so make diff and compare value if it is in a small range near to zero
      Diff = ABS((double)pIndicators->k - (double)1.000);
      pParamDef->StretchFactor *= ( ( -0.1 < Diff ) && ( Diff < 0.1 ) ) ? (1.500) : 1.0;  // ( 1 + (1/2))
      Diff = ABS((double)pIndicators->r - (double)1.000);
      pParamDef->StretchFactor *= ( ( -0.1 < Diff ) && ( Diff < 0.1 ) ) ? (0.500) : 1.0;  // ( 1 - (1/sqrt(2)))            --> changed from formula to experimental value of 0.5
      Diff = ABS((double)pIndicators->r - (double)0.354); // 0.354 = sqrt(2)/4
      pParamDef->StretchFactor *= ( ( -0.1 < Diff ) && ( Diff < 0.1 ) ) ? (0.750) : 1.0;  // ( 1 - (sqrt(2)/4*1/sqrt(2)))
   }
}

/*****************************************************************************
*                    Extern Function Definitions
*****************************************************************************/

void MATH2D_Init( Math2d_StepT *pStep )
{
   BYTE i;

   mIteration   = 0;
   mValidSearch = 0;
   mStep        = *pStep;

   for ( i = 0 ; i < NR_OF_DIMS ; i++ )
   {
      // ws_index indicates which ws parameter nr that will be tuned
      mSequence[i].DebugIdx           = (BYTE) pStep->Par[i].DebugIdx;
      mSequence[i].ZeroPoint          = pStep->Par[i].ZeroPoint;
      mSequence[i].DeltaFac1_0        = pStep->Par[i].DeltaFac;
      mSequence[i].Relative           = pStep->Par[i].Relative;
      mSequence[i].MinBound           = pStep->Par[i].Min;
      mSequence[i].MaxBound           = pStep->Par[i].Max;
      mSequence[i].StretchFactor      = 1.0;
      mSequence[i].PrevStretchFactor  = 1.0;
      mIndicators[i].k                = 0;
      mIndicators[i].s                = 0;
      mIndicators[i].r                = 0;
   }

}

/*--------------------------------------------------------------------------*/

void MATH2D_AdjustBoundary( Math2d_StepT *pStep )
{
   BYTE i;

   for ( i = 0 ; i < NR_OF_DIMS ; i++ )
   {
      // ws_index indicates which ws parameter nr that will be tuned
      mSequence[i].MinBound = pStep->Par[i].Min;
      mSequence[i].MaxBound = pStep->Par[i].Max;
   }

}

/*--------------------------------------------------------------------------*/

void MATH2D_GetExperimentPlan( Math2d_ExperimentT *pExperimentPlan )
{
   float Fac[2];
   BYTE  i;
   BYTE  j;
   SHORT Param;
   BYTE  ExperimentOrder;
   SHORT StretchDeltaFac1_0[2];

   CalcNewZeroPoint(&(mIndicators[0]), &mSequence[0], &StretchDeltaFac1_0[0]);
   CalcNewZeroPoint(&(mIndicators[1]), &mSequence[1], &StretchDeltaFac1_0[1]);

   SendMsgCn(DEBUG_MATH2D_MSG,2,0x45010A,A1B(mSequence[0].StretchFactor),A1B(mSequence[1].StretchFactor));
   //DEBUG_system_dma_printf(("MATH2D adjusted stretch factor=%.2f %.2f", mSequence[0].StretchFactor , mSequence[1].StretchFactor ));

   sprintf(msExperimentPlan[0], "");
   sprintf(msExperimentPlan[1], "");

   for ( i = 0 ; i < NR_OF_2D_EXPERIMENTS ; i++ )
   {
      ExperimentOrder = mExperimentalPlanOrder[mStep.eMode][i];

      GetExperimentFactors(ExperimentOrder, Fac, mStep.eMode );
      for ( j = 0 ; j < NR_OF_DIMS ; j++ )
      {
         // in case of rotated mode, calculate the same distance between 0-0.5 and 0.5-1

         // calculate a new parameter
         if (!mSequence[j].Relative)
         {
            Param = (SHORT)(mSequence[j].ZeroPoint + Fac[j] * (float) StretchDeltaFac1_0[j] );
         }
         else
         {
#if ( NEW_METHOD_MIN_MAX == 1 )
            Param = float2short((float)((float)mSequence[j].ZeroPoint * (100. + Fac[j] * (float)StretchDeltaFac1_0[j]) / ((float)100. )));
#else
            Param = float2short((float)((float)mSequence[j].ZeroPoint * (200. + Fac[j] * (float)StretchDeltaFac1_0[j]) / ((float)200. - 2 * Fac[j] * (float) StretchDeltaFac1_0[j])));
#endif
         }
         // set back the parameter at the correct position in the writestrategy structure (use index access)
         pExperimentPlan[i].Param[j] = Param;
         sprintf(msExperimentPlan[j], "%s %d", msExperimentPlan[j], Param);
      }
   }
   PrintfStepInfo();
   SendMsg80(DEBUG_MATH2D_MSG,0x450008);//MATH2D
   SendMsg80(DEBUG_MATH2D_MSG,0x45000E);//PARAM0
   SendMsgCn(DEBUG_MATH2D_MSG,1,0x45000D,A1B(msExperimentPlan[0]));
   SendMsg80(DEBUG_MATH2D_MSG,0x45000F);//PARAM1
   SendMsgCn(DEBUG_MATH2D_MSG,1,0x45000D,A1B(msExperimentPlan[1]));
   //DEBUG_system_dma_printf(("MATH2D Param1 %s (strlen %05d/%05d)", msExperimentPlan[1], strlen(msExperimentPlan[1]), NR_OF_2D_EXPERIMENTS * 20 + 1 ));
}

/*--------------------------------------------------------------------------*/
void MATH2D_GetServoLimits( Math2d_ExperimentT *pMeasurePlan,
                            Math2d_ServoLimitsT *pLimits,
                            SHORT BoundaryLimitItem0,
                            SHORT BoundaryLimitItem1 )
{
#if TO_BE_PORTED
   float  b0,b1,b2,b3,b4,b5 ;  // params fit curve

   float  optim_min_0,optim_min_1;  // to simplify formula
   float  a,b,c,term ;
   float temp   = 0;
   float min_x0 = (SHORT)0x7FFF,
         max_x0 = (SHORT)0x8000,
         min_x1 = (SHORT)0x7FFF,
         max_x1 = (SHORT)0x8000;
   float norm1, norm2, MinCost;
   BYTE  i;

   //we need to calculate with normalized x values.
   for (i = 0; i < NR_OF_2D_EXPERIMENTS; i++)
   {
      // param 1
      if (measure_plan[i].param[0] < min_x0)
         min_x0 = measure_plan[i].param[0];
      if (measure_plan[i].param[0] > max_x0)
         max_x0 = measure_plan[i].param[0];
      // param 2
      if (measure_plan[i].param[1] < min_x1)
         min_x1 = measure_plan[i].param[1];
      if (measure_plan[i].param[1] > max_x1)
         max_x1 = measure_plan[i].param[1];
      //DEBUG_system_dma_printf(("i=%d %d %d",i,min_x0,max_x0   ));
      //DEBUG_system_dma_printf(("i=%d %d %d",i,min_x1,max_x1   ));
   }
   SendMsgCnB(DEBUG_MATH2D_MSG,8,0x45010C,F4B(min_x0),F4B(max_x0));
   SendMsgCnB(DEBUG_MATH2D_MSG,8,0x45010D,F4B(min_x1),F4B(max_x1));

   // to simplify formula
   b0 = limits->b[0];   b1 = limits->b[1];   b2 = limits->b[2];   b3 = limits->b[3];   b4 = limits->b[4];   b5 = limits->b[5];

   // due to inaccuracy (float to SHORT) from Optimum calculation, we re-calculate Optimum ourselves :
   MinCost   = b0
                + b1 * limits->fac[0]
                + b2 * limits->fac[1]
                + b3 * limits->fac[0] * limits->fac[1]
                + b4 * limits->fac[0] * limits->fac[0]
                + b5 * limits->fac[1] * limits->fac[1];
   SendMsgCnB(DEBUG_MATH2D_MSG,4,0x450200,F4B(MinCost)); //MATH2D : calculated MinCost
   // define servo working area :
   optim_min_0   = ((float)boundary_limit_cost_item0 *  MinCost)/100;
   optim_min_1   = ((float)boundary_limit_cost_item1 *  MinCost)/100;
   SendMsgCnB(DEBUG_MATH2D_MSG,8,0x450201,F4B(optim_min_0),F4B(optim_min_1));
   // formula to solve   :
   //=====================
   //                 _________
   //               /  2
   //xmin = -b +- \/ b  - 4ac
   //       -------------------
   //               2a

   // x1 min
   //=================================

   a = b4;
   b = b1 + (b3*(limits->fac[1]));
   c = b0 + (b2*limits->fac[1]) +(b5*(limits->fac[1]*limits->fac[1])) - (float)optim_min_0 ;
   term =(b*b)-(4*a*c);

   //DEBUG_system_dma_printf(("MATH2D : (x0min) a = %f ; b = %f ; c = %f ; term = %f;",a,b,c,term));

   if (term <= 0 )     // crash and burn !
   {
      DEBUG_ASSERT(FALSE, eASSERT_FUNCTION_NOT_IMPLEMENTED);
   }

   limits->limit[0].param_min  =  ((-1*b) + fsqrt(term))/(2*a);//limits->limit[0].param_min  =  ((-1*b) + (float)LIB_sqrt_32int((ULONG)LIB_float_to_short(term)))/(2*a);
   limits->limit[0].param_max  =  ((-1*b) - fsqrt(term))/(2*a);//limits->limit[0].param_max  =  ((-1*b) - (float)LIB_sqrt_32int((ULONG)LIB_float_to_short(term)))/(2*a);

   // swap min and max to correct position
   if (limits->limit[0].param_min>limits->limit[0].param_max)
   {
      temp                       = limits->limit[0].param_min;
      limits->limit[0].param_min = limits->limit[0].param_max;
      limits->limit[0].param_max = temp;
   }
   SendMsgCnB(DEBUG_MATH2D_MSG,8,0x450203,F4B(limits->limit[0].param_min),F4B(limits->limit[0].param_max));
   norm1 = (max_x0 - min_x0)/2;
   norm2 = (max_x0 + min_x0)/2;
   SendMsgCnB(DEBUG_MATH2D_MSG,8,0x450205,F4B(norm1),F4B(norm2));
   limits->limit[0].param_min = (SHORT)((limits->limit[0].param_min*norm1) + norm2);
   limits->limit[0].param_max = (SHORT)((limits->limit[0].param_max*norm1) + norm2);

   // x2 min
   //=================================
   a = b5;
   b = b2 + (b3*(limits->fac[0]));
   c = b0 + (b1*limits->fac[0]) +(b4*(limits->fac[0]*limits->fac[0])) - (float)optim_min_1 ;

   term =(b*b)-(4*a*c);
   SendMsg80(DEBUG_MATH2D_MSG,0x45020b);
   SendMsgCnB(DEBUG_MATH2D_MSG,8,0x45020C,F4B(a),F4B(b));
   SendMsgCnB(DEBUG_MATH2D_MSG,8,0x45020D,F4B(c),F4B(term));
   //DEBUG_system_dma_printf(("MATH2D : (x1min) a = %f ; b = %f ; c = %f ; term = %f;",a,b,c,term));
   if (term < 0 )     // crash and burn !
   {
      DEBUG_ASSERT(FALSE, eASSERT_FUNCTION_NOT_IMPLEMENTED);
   }

   limits->limit[1].param_min  =  ((-1*b) + fsqrt(term))/(2*a);//limits->limit[1].param_min  =  ((-1*b) + (float)LIB_sqrt_32int((ULONG)LIB_float_to_short(term)))/(2*a);
   limits->limit[1].param_max  =  ((-1*b) - fsqrt(term))/(2*a);//limits->limit[1].param_max  =  ((-1*b) - (float)LIB_sqrt_32int((ULONG)LIB_float_to_short(term)))/(2*a);

   if (limits->limit[1].param_min>limits->limit[1].param_max)
   {
      temp                       = limits->limit[1].param_min;
      limits->limit[1].param_min = limits->limit[1].param_max;
      limits->limit[1].param_max = temp;
   }
   SendMsgCnB(DEBUG_MATH2D_MSG,8,0x450207,F4B(limits->limit[1].param_min),F4B(limits->limit[1].param_max));

   norm1 = ((float)max_x1 - (float)min_x1)/2;
   norm2 = ((float)max_x1 + (float)min_x1)/2;
   SendMsgCnB(DEBUG_MATH2D_MSG,8,0x450209,F4B(norm1),F4B(norm2));
   limits->limit[1].param_min = (SHORT)((limits->limit[1].param_min*norm1) + norm2);
   limits->limit[1].param_max = (SHORT)((limits->limit[1].param_max*norm1) + norm2);
   SendMsgCnB(DEBUG_MATH2D_MSG,8,0x450203,F4B(limits->limit[0].param_min),F4B(limits->limit[0].param_max));
   SendMsgCnB(DEBUG_MATH2D_MSG,8,0x450207,F4B(limits->limit[1].param_min),F4B(limits->limit[1].param_max));
#endif
}
/*--------------------------------------------------------------------------*/
BOOL MATH2D_GetOptimum( Math2d_ResultT *pResult2d , Math2d_ExperimentResultT *pExperimentResult , Math2d_MeasQualityDefT *pQualityDef )
{
   BOOL               result = TRUE;
   BYTE               ExperimentOrder;
   int                i;
   ParamDefT          OldSequence[2];
   IterationFactors   OldIndicators[2];
   EMath2D_Mode       eOldMode;
   double             AbsFac;

   // remember the old values, to check if any parameter changed during the calculation
   OldSequence[0]   = mSequence[0];
   OldSequence[1]   = mSequence[1];
   OldIndicators[0] = mIndicators[0];
   OldIndicators[1] = mIndicators[1];
   eOldMode         = mStep.eMode;

   sprintf(msCost, "");
   for (i = 0; i < NR_OF_2D_EXPERIMENTS; i++)
   {
      sprintf(msCost, "%s %d ", msCost, pExperimentResult[i].Cost);
      //sprintf(msCost, "%s %d", msCost, pExperimentResult[i].Valid);
   }
   for (i = 0; i < NR_OF_2D_EXPERIMENTS; i++)
   {
      ExperimentOrder                    = mExperimentalPlanOrder[mStep.eMode][i];
      mExperimentResult[ExperimentOrder] = pExperimentResult[i];
   }
   SendMsg80(DEBUG_MATH2D_MSG,0x450008);
   SendMsg80(DEBUG_MATH2D_MSG,0x450109);
   //DEBUG_system_dma_printf(("MATH2D Cost %s", msCost));

   if ( IsExperimentResultsValid( mExperimentResult ) )
   {
      CalcCoefs( mExperimentResult , pResult2d , mStep.eMode );
      pResult2d->IterationRequired = SearchForOptimum( pQualityDef , mExperimentResult , pResult2d , mIndicators );
      result                      = ( pResult2d->IterationRequired ) ? FALSE : TRUE;
      if ( result )
      {
         pResult2d->NrIterations = mValidSearch + mIteration + 1; // Nr of Iterations for module-test
         CalcOptimum( pResult2d );
      }
      if ( pResult2d->IterationRequired )
      {
         mIteration++;
         if ( mIteration >= mStep.MaxIterations )
         {
            pResult2d->IterationRequired = FALSE;
            result                       = FALSE;
            pResult2d->NrIterations      = 0x7F; // Iterations reached indicator for module-test
         }
      }
   }
   else
   {
      SendMsg80(DEBUG_MATH2D_MSG,0x450300);
      pResult2d->IterationRequired = SearchForValidMeasurement( pQualityDef , mExperimentResult , mIndicators, &mStep.eMode );
      // Init to zero Required for generic handling of boundary or interation exhausted
      for (i = 0; i < 6; i++)          pResult2d->b[i]   = 0;
      for (i = 0; i < NR_OF_DIMS; i++) pResult2d->Fac[i] = 0;
      pResult2d->Det                = 0;
      // End Init to zero
      result                        = FALSE;
      if ( pResult2d->IterationRequired )
      {
         mValidSearch++;
         if ( mValidSearch >= mStep.MaxIterations )
         {
            pResult2d->IterationRequired = FALSE;
            result                       = FALSE;
            pResult2d->NrIterations      = 0x7F; // Iterations reached indicator for module-test
         }
      }
   }
   SendMsgCn(DEBUG_MATH2D_MSG,1,0x450301,A1B(pResult2d->IterationRequired));
   if ( pResult2d->IterationRequired )
   {
      // any of the R (reduce range), S (shift), or K (expanding range) variables is non-zero: no Optimum yet
      CalcStretchFactor(&(mIndicators[0]), &mSequence[0]);
      CalcStretchFactor(&(mIndicators[1]), &mSequence[1]);
      SendMsgCnB(DEBUG_MATH2D_MSG,8,0x450302,F4B(mSequence[0].StretchFactor),F4B(mSequence[1].StretchFactor));
      //DEBUG_system_dma_printf(("MATH2D new stretch factor=%.2f %.2f", mSequence[0].StretchFactor , mSequence[1].StretchFactor ));

      if ( eOldMode == mStep.eMode )
      {
         pResult2d->IterationRequired = ( CheckParametersChanged(OldIndicators, mIndicators, OldSequence, mSequence) &&
                                          CheckWithinBoundary( &mIndicators[0] ) &&
                                          CheckWithinBoundary( &mIndicators[1] ) ) ;
      }
      // Recovery on boundary or interation exhausted
      if ( !pResult2d->IterationRequired && ( pResult2d->Det > 0 ) &&
           ( ( !mStep.CostInverted && ( pResult2d->b[4] > 0 ) && ( pResult2d->b[5] > 0 ) )  ||
             (  mStep.CostInverted && ( pResult2d->b[4] < 0 ) && ( pResult2d->b[5] < 0 ) ) ) &&
             ( ABS((double)pResult2d->Fac[0]) <= 1.5 ) && ( ABS((double)pResult2d->Fac[1]) <= 1.5 ) )
      {
         for (i = 0; i < NR_OF_DIMS; i++)
         {
            AbsFac = ABS((double)pResult2d->Fac[i]);
            if ( ( 1.0 < AbsFac ) && ( AbsFac < 1.5 ) )
            {
               pResult2d->Fac[i] = ( pResult2d->Fac[i] > 0 ) ? 1.0 : -1.0;
            }
         }
         CalcOptimum( pResult2d );
         result = TRUE;
      }

   }
   SendMsgCn(DEBUG_MATH2D_MSG,2,0x450304,A1B(result),A1B(pResult2d->IterationRequired));
   //DEBUG_system_dma_printf(("MATH2D_get_optimum valid_optimum %d iteration_req %d", result, pResult2d->IterationRequired ));
   if ( !result && pResult2d->IterationRequired && ( mIteration < mStep.MaxIterations ) )
   {
      pResult2d->NrIterations = 0xFF; // Boundary reached indicator for module-test
   }
   return result; // maybe later we need error-codes instead of true/false and interation-required
}

/*****************************************************************************
*                    End Of File
*****************************************************************************/
