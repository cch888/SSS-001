/****************************************************************************
*                (c) Copyright 2006 - 2007 SUNEXT DESIGN, INC.
*                           All Rights Reserved
*
*
* FILENAME:   coefs.h
*
* DESCRIPTION
*
* NOTES:
*
* $Revision: 20 $
* $Date: 11/03/11 2:32p $
*
*
**************************** SUNEXT CONFIDENTIAL ****************************/
#ifndef _COEFS_H_
#define _COEFS_H_

/*****************************************************************************
*                    Include Definitions
*****************************************************************************/

#include ".\oem\oem_info.h"
#include ".\common\globtype.h"
#include ".\oem\coef_values.h"
#include ".\servo\SVO.H"

/*****************************************************************************
*                    Macro Definitions
*****************************************************************************/

/*****************************************************************************
*                    Type Definitions
*****************************************************************************/

typedef union
{
   BYTE   byte;
   USHORT ushort;
   ULONG  ulong;   
} CoefValueT;

typedef struct
{    
    BlockT *Address;    
    ULONG   Mask;
    BYTE    LSB;
    BYTE    RegSize;
} CoefPropertiesT;

/*****************************************************************************
*                    Public Data Declarations
*****************************************************************************/

extern COEFS_MEDIA_t mCoefDiscKind;
extern COEFS_SPEED_t mCoefSpeed;
extern discKindT     mDiscKind;
extern ESpindleSpeed mSpeed;
extern const CoefDependencyPropertiesT CoefDependencyProperties[E_CORE_COUNT];

/*****************************************************************************
*                    Public Function Prototypes
*****************************************************************************/

extern void  COEF_Init(void);
extern void  COEF_StoreParams(discKindT DiscKind, ESpindleSpeed Speed);
extern void  COEF_GetProperties(COEFS_t eCoef, CoefPropertiesT *CoefProperties);
extern ULONG GetValueParams(COEFS_MEDIA_t CoefDiscKind, COEFS_SPEED_t CoefSpeed, COEFS_t eCoef);
extern void  SetValueParams(COEFS_MEDIA_t CoefDiscKind, COEFS_SPEED_t CoefSpeed, COEFS_t eCoef, ULONG Value);
extern void  ConvertParams(discKindT DiscKind, ESpindleSpeed Speed, COEFS_MEDIA_t *CoefDiscKind, COEFS_SPEED_t *CoefSpeed  );

/*****************************************************************************
*                    Inline Definitions
*****************************************************************************/

/*-----------------------------------------------------------------------------------------------------
    Name: COEF_GetValue          

    Description:    

    Inputs:         
    Outputs:        
    Global Inputs:  
    Global Outputs: 
    Returns:        
-----------------------------------------------------------------------------------------------------*/

__inline ULONG COEF_GetValue(COEFS_t eCoef)
{
    return GetValueParams(mCoefDiscKind, mCoefSpeed, eCoef);
}

/*-----------------------------------------------------------------------------------------------------
    Name: COEF_GetValueParams          

    Description:    

    Inputs:         
    Outputs:        
    Global Inputs:  
    Global Outputs: 
    Returns:        
-----------------------------------------------------------------------------------------------------*/

__inline ULONG COEF_GetValueParams(discKindT DiscKind, ESpindleSpeed Speed, COEFS_t eCoef)
{
   COEFS_MEDIA_t CoefDiscKind;
   COEFS_SPEED_t CoefSpeed;
      
   ConvertParams(DiscKind, Speed, &CoefDiscKind, &CoefSpeed );    

   return GetValueParams(CoefDiscKind, CoefSpeed, eCoef);
}

/*-----------------------------------------------------------------------------------------------------
    Name: COEF_GetValueParamsDiag          

    Description:    

    Inputs:         
    Outputs:        
    Global Inputs:  
    Global Outputs: 
    Returns:        
-----------------------------------------------------------------------------------------------------*/

__inline ULONG COEF_GetValueParamsDiag(COEFS_MEDIA_t CoefDiscKind, COEFS_SPEED_t CoefSpeed, COEFS_t eCoef)
{
   return GetValueParams(CoefDiscKind, CoefSpeed, eCoef);
}

/*-----------------------------------------------------------------------------------------------------
    Name: COEF_SetValue          

    Description:    

    Inputs:         
    Outputs:        
    Global Inputs:  
    Global Outputs: 
    Returns:        
-----------------------------------------------------------------------------------------------------*/

__inline void COEF_SetValue(COEFS_t eCoef, ULONG Value)
{
   SetValueParams(mCoefDiscKind, mCoefSpeed, eCoef, Value);
}

/*-----------------------------------------------------------------------------------------------------
    Name: COEF_SetValueParams          

    Description:    

    Inputs:         
    Outputs:        
    Global Inputs:  
    Global Outputs: 
    Returns:        
-----------------------------------------------------------------------------------------------------*/

__inline void COEF_SetValueParams(discKindT DiscKind, ESpindleSpeed Speed, COEFS_t eCoef, ULONG Value)
{
   COEFS_MEDIA_t CoefDiscKind;
   COEFS_SPEED_t CoefSpeed;
   
   ConvertParams(DiscKind, Speed, &CoefDiscKind, &CoefSpeed );    

   SetValueParams(CoefDiscKind, CoefSpeed, eCoef, Value);
}

/*-----------------------------------------------------------------------------------------------------
    Name: COEF_SetValueParamsDiag          

    Description:    

    Inputs:         
    Outputs:        
    Global Inputs:  
    Global Outputs: 
    Returns:        
-----------------------------------------------------------------------------------------------------*/

__inline void COEF_SetValueParamsDiag(COEFS_MEDIA_t CoefDiscKind, COEFS_SPEED_t CoefSpeed, COEFS_t eCoef, ULONG Value)
{
   SetValueParams(CoefDiscKind, CoefSpeed, eCoef, Value);
}

#endif //_COEFS_H_

/*****************************************************************************
*                    End Of File
*****************************************************************************/
