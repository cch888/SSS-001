/****************************************************************************
*                (c) Copyright 2001,2002,2003, 2004, 2005 SUNEXT DESIGN, INC.
*                           All Rights Reserved
*
*
* FILENAME:   HAL_bd_codec.h
*
* DESCRIPTION
*   This file supports the HAL BD Codec funtions implemented in the BD
*   Codec HAL layer
*
* NOTES:
*
* $Revision: 27 $
* $Date: 11/01/07 4:57p $
*
*
**************************** SUNEXT CONFIDENTIAL ****************************/
#ifndef _HAL_BD_CODEC_H_
#define _HAL_BD_CODEC_H_

#include ".\common\globals.h"
#include ".\AL\REG_bd_codec.h"

//*** BD Decoder Interrupt and Status Macros
#define HAL_BD_CODEC_ENABLE_DECODER()           READ_MOD_WRITE(HAL_BDCREG_MAP->BDCMODEN,HAL_BDC_ERASTRGON|HAL_BDC_ERRSTATON|\
                                                                                        HAL_BDC_ERCOSHON|HAL_BDC_ARBITERON|\
                                                                                        HAL_BDC_BDFDON|HAL_BDC_BDSDON|\
                                                                                        HAL_BDC_BDINON|HAL_BDC_AUNDECON,\
                                                                                        HAL_BDC_ERASTRGON|HAL_BDC_ERRSTATON|\
                                                                                   HAL_BDC_ERCOSHON|HAL_BDC_ARBITERON|\
                                                                                   HAL_BDC_BDFDON|HAL_BDC_BDSDON|\
                                                                                   HAL_BDC_BDINON|HAL_BDC_AUNDECON)

#define HAL_BD_CODEC_DISABLE_DECODER()          WRITE_REG(HAL_BDCREG_MAP->BDCMODEN, 0x00) //???

#define HAL_BD_CODEC_ENABLE_DECODER_INTS()      WRITE_REG(HAL_BDCREG_MAP->BDDINT1E, HAL_BDC_ERAOVFLE|HAL_BDC_AEDCFAE|\
                                                                                    HAL_BDC_LWREDCFAE|HAL_BDC_ALDCUNCE|\
                                                                                    HAL_BDC_LWRLDCUNE|HAL_BDC_ASERE|\
                                                                                    HAL_BDC_LWRSERE|\
                                                                                    HAL_BDC_AUNFLRESE|HAL_BDC_AUNFLOLKE|\
                                                                                    HAL_BDC_AUNFLILKE|\
                                                                                    HAL_BDC_FDEDCFAIE|HAL_BDC_FDLDCUNCE|\
                                                                                    HAL_BDC_FDBISUNCE|HAL_BDC_FDCLUSDNE|\
                                                                                    HAL_BDC_CONTERRE);\
                                                WRITE_REG(HAL_BDCREG_MAP->BDDINT2E, HAL_BDC_FDSTPADFE|HAL_BDC_FDSTRADFE)

#define HAL_BD_CODEC_ENABLE_ENCODER_INTS()

#define HAL_BD_CODEC_ENABLE_TARGET_MISS_INT()   WRITE_FIELD(HAL_BDC_FDSTRADME, SET);\
                                                WRITE_FIELD(HAL_BDC_FDSTPADME, SET)

#define HAL_BD_CODEC_DISABLE_TARGET_MISS_INT()  WRITE_FIELD(HAL_BDC_FDSTRADME, CLEAR)

#define HAL_BD_CODEC_GET_DECODER_INT1_STATUS()  READ_REG(HAL_BDCREG_MAP->BDDINT1)
#define HAL_BD_CODEC_GET_DECODER_INT2_STATUS()  READ_REG(HAL_BDCREG_MAP->BDDINT2)

//*** BD Buffering Macros
#define HAL_BD_CODEC_STREAM_PASS_ALL_OFF()      WRITE_FIELD(HAL_BDC_STRMPASS, CLEAR)

#define HAL_BD_CODEC_SET_RING_BUFFER_START(x)   WRITE_FEILD(HAL_BDC_FDHSTLDCS, (x >> 16))

#define HAL_BD_CODEC_SetStartAddress(x)         WRITE_REG(HAL_BDCREG_MAP->FDSSTRAD, x)
#define HAL_BD_CODEC_GetStartAddress(x)         READ_REG(HAL_BDCREG_MAP->FDSSTRAD)

#define HAL_BD_CODEC_SetStopAddress(x)          WRITE_REG(HAL_BDCREG_MAP->FDSSTPAD, x)
#define HAL_BD_CODEC_GetStopAddress()           READ_REG(HAL_BDCREG_MAP->FDSSTPAD)

#define HAL_BD_CODEC_SetCurrentMask(x)          WRITE_REG(HAL_BDCREG_MAP->FDSSECM, x)
#define HAL_BD_CODEC_GetCurrentMask()           READ_REG(HAL_BDCREG_MAP->FDSSECM)

//*** BCA Macros
#define HAL_BD_CODEC_SET_BCA_BUFFER_ADR(x)      WRITE_FIELD(HAL_BDC_BDBCASSA, x)
#define HAL_BD_CODEC_ENABLE_BCA_DECODER()       WRITE_REG(HAL_BDCREG_MAP->BDCMODEN,HAL_BDC_BCADECON|HAL_BDC_ERASTRGON|\
                                                                                   HAL_BDC_ERRSTATON|HAL_BDC_ERCOSHON|\
                                                                                   HAL_BDC_ARBITERON|HAL_BDC_BDFDON|\
                                                                                   HAL_BDC_BDSDON|HAL_BDC_BDINON|HAL_BDC_AUNDECON)

#define HAL_BD_CODEC_ENABLE_BCA_DECODER_INTS()  WRITE_REG(HAL_BDCREG_MAP->BDDINT1E, HAL_BDC_ERAOVFLE|HAL_BDC_AEDCFAE|\
                                                                                    HAL_BDC_LWREDCFAE|HAL_BDC_ALDCUNCE|\
                                                                                    HAL_BDC_LWRLDCUNE|HAL_BDC_ASERE|\
                                                                                    HAL_BDC_LWRSERE|\
                                                                                    HAL_BDC_AUNFLRESE|HAL_BDC_AUNFLOLKE|\
                                                                                    HAL_BDC_AUNFLILKE|HAL_BDC_AUNDECE|\
                                                                                    HAL_BDC_FDEDCFAIE|HAL_BDC_FDLDCUNCE|\
                                                                                    HAL_BDC_FDBISUNCE|HAL_BDC_FDCLUSDNE|\
                                                                                    HAL_BDC_CONTERRE);\
                                                WRITE_REG(HAL_BDCREG_MAP->BDDINT2E, HAL_BDC_BCAUNE|HAL_BDC_BCADONEE|HAL_BDC_FDSTPADFE)

#define HAL_BD_CODEC_BCA_START_DECODE()         WRITE_FIELD(HAL_BDC_BCASTART, 1)
#define HAL_BD_CODEC_BCA_STOP_DECODE()          WRITE_FIELD(HAL_BDC_BCASTART, 0)

//*** Global definitions regarding the DFE HAL functionality
//define header interrupt enabling facility
typedef enum
{
    HalBDC_HeaderInterrDisable,
    HalBDC_HeaderInterrEnable
} halDFE_HeaderInterr_t;

//*** Initial value
#define BDCMODEN_INIT       (HAL_BDC_ERASTRGON|HAL_BDC_ERRSTATON|HAL_BDC_ERCOSHON|HAL_BDC_ARBITERON|\
                             HAL_BDC_BDFDON|HAL_BDC_BDSDON|HAL_BDC_BDINON|HAL_BDC_AUNDECON)

#define SDCFG_INIT          (HAL_BDC_HLDSDE)
#define FDCFG_INIT          (HAL_BDC_FDSMTFLAG|HAL_BDC_CLSSMFIFO|HAL_BDC_SHLDFD|HAL_BDC_STRMSTRE|HAL_BDC_DSCRMON)

/*** Local defines ***/
#define DEFAULT_BD_AUN_FLYWHEEL_OUT_OF_LOCK_THRESHOLD       0x04
#define DEFAULT_BD_AUN_FLYWHEEL_IN_LOCK_THRESHOLD           0x0A
#define DEFAULT_BD_AUN_FLYWHEEL_MAX_NUM_ERRORS                      0x01
#define DEFAULT_BD_AUN_FLYWHEEL_MAX_JUMP_FORWARD            0x0F
#define DEFAULT_BD_ERRATA_NUM_FOR_ENDANGERED_BIS            0x0C
#define DEFAULT_BD_ERRATA_NUM_FOR_ENDANGERED_LDC            0x09

#define DEFAULT_BD_ERROR_LOWER_EDC_FAIL_THRESHOLD               0x00
#define DEFAULT_BD_ERROR_LOWER_EDC_UNCORRECTABLE_THRESHOLD      0x00
#define DEFAULT_BD_ERROR_LOWER_SER_THRESHOLD                    0x00

#define DEFAULT_BD_ERROR_ALERT_LOWER_EDC_FAIL_THRESHOLD             0x00
#define DEFAULT_BD_ERROR_ALERT_LOWER_EDC_UNCORRECTABLE_THRESHOLD    0x00
#define DEFAULT_BD_ERROR_ALERT_LOWER_SER_THRESHOLD                  0x00

//*** External function prototypes
extern void  HAL_BD_CODEC_Init(void);
extern void  HAL_BD_CODEC_InitializeBDSdramBuffer(void);
extern void  HAL_BD_CODEC_Enable_Buffering(void);
extern void  HAL_BD_CODEC_Disable_Buffering(void);
extern void  HAL_BD_CODEC_SetDiscPtr(USHORT x);
extern void  HAL_BD_CODEC_DISABLE_DECODER_INTS(void);
extern void  HAL_BD_CODEC_CLEAR_DECODER_STATUS(void);
extern BOOL  HAL_BD_CODEC_GET_FD_HOLD_STATUS(void);
extern ULONG HAL_BD_CODEC_GET_RING_BUFFER_START(void);
#if (ENABLE_DISCARD_ICE == 1)
extern void  HAL_BD_CODEC_Enable_DiscardIce(void);
extern void  HAL_BD_CODEC_Disable_DiscardIce(void);
#endif  /* ENABLE_DISCARD_ICE == 1 */
#endif /* _HAL_BD_CODEC_H_ */
