/******************************************************************************
*
*     TEMPORARY BRIDGING FILE  ---  DO NOT EDIT
*   ====================================================
*   This file provides a bridge to the applicable ServoDSP code for Deneb Bx or Cx
*
******************************************************************************/
#ifndef _SVO_DSP_BRIDGE_H_
#define _SVO_DSP_BRIDGE_H_

#include "..\oem\oem_info.h"

    #if (CHIP_REV >= 0xC0)

#include "..\servo\Cx_svo_dsp_bist.h"
#include "..\servo\Cx_svo_dsp.h"

#define  MAX_DSP_INSTRUCTIONS       Cx_MAX_DSP_INSTRUCTIONS
#define  MAX_COEF_RAM_SIZE          Cx_MAX_COEF_RAM_SIZE
#define  DSP_POWER_MODE             Cx_DSP_POWER_MODE
#define  DSP_PHAADDR                Cx_DSP_PHAADDR
#define  DSP_PHB1ADDR               Cx_DSP_PHB1ADDR
#define  DSP_PHB2ADDR               Cx_DSP_PHB2ADDR
#define  DSP_PHC1ADDR               Cx_DSP_PHC1ADDR
#define  DSP_PHC2ADDR               Cx_DSP_PHC2ADDR
#define  DSP_PHC3ADDR               Cx_DSP_PHC3ADDR
#define  DSP_PHC4ADDR               Cx_DSP_PHC4ADDR
#define  DSP_PHDADDR                Cx_DSP_PHDADDR

#define  BIST_DSP_INSTRUCTION_SIZE  Cx_BIST_DSP_INSTRUCTION_SIZE
#define  BIST_COEF_INPUT_SIZE       Cx_BIST_COEF_INPUT_SIZE
#define  BIST_CHECK_START_ADDRESS   Cx_BIST_CHECK_START_ADDRESS
#define  BIST_CHECK_SIZE            Cx_BIST_CHECK_SIZE
#define  BIST_LOOP_COUNTER_ADDRESS  Cx_BIST_LOOP_COUNTER_ADDRESS
#define  BIST_DSP_POWER_MODE        Cx_BIST_DSP_POWER_MODE
#define  BIST_DSP_PHAADDR           Cx_BIST_DSP_PHAADDR
#define  BIST_DSP_PHB1ADDR          Cx_BIST_DSP_PHB1ADDR
#define  BIST_DSP_PHB2ADDR          Cx_BIST_DSP_PHB2ADDR
#define  BIST_DSP_PHC1ADDR          Cx_BIST_DSP_PHC1ADDR
#define  BIST_DSP_PHC2ADDR          Cx_BIST_DSP_PHC2ADDR
#define  BIST_DSP_PHC3ADDR          Cx_BIST_DSP_PHC3ADDR
#define  BIST_DSP_PHC4ADDR          Cx_BIST_DSP_PHC4ADDR
#define  BIST_DSP_PHDADDR           Cx_BIST_DSP_PHDADDR


    #else

#include "..\servo\Bx_svo_dsp.h"
#include "..\servo\Bx_svo_dsp_bist.h"

#define  MAX_DSP_INSTRUCTIONS       Bx_MAX_DSP_INSTRUCTIONS
#define  MAX_COEF_RAM_SIZE          Bx_MAX_COEF_RAM_SIZE
#define  DSP_POWER_MODE             Bx_DSP_POWER_MODE
#define  DSP_PHAADDR                Bx_DSP_PHAADDR
#define  DSP_PHB1ADDR               Bx_DSP_PHB1ADDR
#define  DSP_PHB2ADDR               Bx_DSP_PHB2ADDR
#define  DSP_PHC1ADDR               Bx_DSP_PHC1ADDR
#define  DSP_PHC2ADDR               Bx_DSP_PHC2ADDR
#define  DSP_PHC3ADDR               Bx_DSP_PHC3ADDR
#define  DSP_PHC4ADDR               Bx_DSP_PHC4ADDR
#define  DSP_PHDADDR                Bx_DSP_PHDADDR

#define  BIST_DSP_INSTRUCTION_SIZE  Bx_BIST_DSP_INSTRUCTION_SIZE
#define  BIST_COEF_INPUT_SIZE       Bx_BIST_COEF_INPUT_SIZE
#define  BIST_CHECK_START_ADDRESS   Bx_BIST_CHECK_START_ADDRESS
#define  BIST_CHECK_SIZE            Bx_BIST_CHECK_SIZE
#define  BIST_LOOP_COUNTER_ADDRESS  Bx_BIST_LOOP_COUNTER_ADDRESS
#define  BIST_DSP_POWER_MODE        Bx_BIST_DSP_POWER_MODE
#define  BIST_DSP_PHAADDR           Bx_BIST_DSP_PHAADDR
#define  BIST_DSP_PHB1ADDR          Bx_BIST_DSP_PHB1ADDR
#define  BIST_DSP_PHB2ADDR          Bx_BIST_DSP_PHB2ADDR
#define  BIST_DSP_PHC1ADDR          Bx_BIST_DSP_PHC1ADDR
#define  BIST_DSP_PHC2ADDR          Bx_BIST_DSP_PHC2ADDR
#define  BIST_DSP_PHC3ADDR          Bx_BIST_DSP_PHC3ADDR
#define  BIST_DSP_PHC4ADDR          Bx_BIST_DSP_PHC4ADDR
#define  BIST_DSP_PHDADDR           Bx_BIST_DSP_PHDADDR

    #endif

#endif // #ifndef _SVO_DSP_BRIDGE_H_

/******************************************************************************
*  End of file.
******************************************************************************/
