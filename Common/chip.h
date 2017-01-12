/****************************************************************************
*                (c) Copyright 2001,2002 2003, 2004, 2005 SUNEXT DESIGN, INC.
*                            All Rights Reserved
*
* FILE NAME:   CHIP.H
*
* DESCRIPTION
*   This file contains all the definitions for the ARM Memory Map
*
* $Revision: 15 $
* $Date: 08/07/30 8:25p $
*
*************************** SUNEXT CONFIDENTIAL ********************************/

#ifndef __CHIP_H__
#define __CHIP_H__

#define ASIC_BASE       (0x10000000L)

#include ".\oem\oem_info.h"
#include ".\common\vu_cfg.h"
#include ".\common\timer.h"
#include ".\common\adc.h"
#include ".\common\dac.h"
#include ".\common\sci.h"
#include ".\common\intctl.h"
#include ".\common\hwdef.h"


#define SCREG       ((vBYTE *)(ASIC_BASE))

#define ENC_DEC_OPC_SRAM       0x10008000L      // 6 kBytes internal SRAM for Encoder / Decoder / Auto-OPC
#define ENC_DEC_OPC_SRAM_SIZE  0x1800           //   Size

#define FLASH_BASE                 0x30000000L

#define SCSDRAM_ADDRESS            0x60000000L
#define SCSDRAM_START_ADDR         SCSDRAM_ADDRESS
#define SCSDRAM   (  (vBYTE *)    (SCSDRAM_ADDRESS))
#define SCSDRAM16 ((vUSHORT *)    (SCSDRAM_ADDRESS))
#define SCSDRAM16_START_ADDR      (SCSDRAM_ADDRESS))
#define SCSDRAM32 ( (vULONG *)    (SCSDRAM_ADDRESS))
#define SCSDRAM32_START_ADDR      (SCSDRAM_ADDRESS))

#if (ARMC == DISABLE_CACHE)                         // Total Internal Memory is 16K Bytes
#define INTERNAL_SRAM              0x70000000L          // 12 kBytes internal SRAM
#define CACHE_ADDRESS              0x70003000L          // 4 K not used
#elif (ARMC == ENABLE_CACHE_4K)
#define INTERNAL_SRAM              0x70000000L          // 12 kBytes internal SRAM
#define CACHE_ADDRESS              0x70003000L          // 4 K internal Cache
#elif (ARMC == ENABLE_CACHE_8K)
#define INTERNAL_SRAM              0x70000000L          // 8 kBytes internal SRAM
#define CACHE_ADDRESS              0x70002000L          // 8 K internal Cache
#endif // ARMC

#define SRAM_BASE                  INTERNAL_SRAM

#define INTERRUPT_CONTROLLER       0xFFFFF000L

#endif /* __CHIP_H__ */
/*** END of CHIP.H ***/
