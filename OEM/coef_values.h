/******************************************************************************
*
*     TEMPORARY BRIDGING FILE  ---  DO NOT EDIT
*   ====================================================
*   This file provides a bridge to the coefs for Deneb A0 or Deneb B0 and the
*   FW stack
*
******************************************************************************/
#ifndef _COEF_VALUES_BRIDGE_H_
#define _COEF_VALUES_BRIDGE_H_

#include ".\oem\oem_info.h"
#if( PCB == 710)
    #if(CHIP_REV >= 0xC0)
        #if(CEProduct==1)
            #if(OPU == SFBD414)
                #include ".\oem\coef_values_C1_710_CE_SFBD414.h"
            #else
                #include ".\oem\coef_values_C1_710_CE.h"
            #endif
        #else
            #if(OPU == SFBD414)
                #include ".\oem\coef_values_C1_710_SFBD414.h"
            #else
                #include ".\oem\coef_values_C1_710.h"
            #endif
        #endif
    #else
    #if(CEProduct==1)
        #include ".\oem\coef_values710_CE_B3.h"
    #else
        #include ".\oem\coef_values710_B3.h"
    #endif
    #endif
#elif(PCB == 711)
    #if(CHIP_REV >= 0xC0)
        #if(CEProduct==1)
            #if(OPU == SFBD414)
                #include ".\oem\coef_values_C1_711_CE_SFBD414.h"
            #else
                #include ".\oem\coef_values_C1_711_CE.h"
            #endif
        #else
            #if(OPU == SFBD414)
                #include ".\oem\coef_values_C1_711_SFBD414.h"
            #else
                #include ".\oem\coef_values_C1_711.h"
            #endif
        #endif
    #else
        #if(CEProduct==1)
            #include ".\oem\coef_values711_CE_B3.h"
        #else
            #include ".\oem\coef_values711_B3.h"
        #endif
    #endif
#elif (PCB == 815)
#include ".\oem\coef_values815_B3.h"
#elif (PCB == 817)
#include ".\oem\coef_values_C1_817_CE.h"
#else
#include ".\oem\coef_values_B3.h"
#endif // PCB == 710 || PCB == 711
#endif // _COEF_VALUES_BRIDGE_H_



/******************************************************************************
*  End of file.
******************************************************************************/
