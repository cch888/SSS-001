/****************************************************************************
*                (c) Copyright 2000 - 2008 SUNEXT DESIGN, INC.
*                           All Right Reserved
*
*
*   FILE NAME:     $Workfile: dvMacros.h $
*
*   DESCRIPTION:   DVD DSP related code macros only
*
*
*   $Revision: 30 $
*   $Date: 09/07/29 4:16p $
*
**************************** SUNEXT CONFIDENTIAL *******************************/
#ifndef __DVMACROS_H__
#define __DVMACROS_H__


/*******************************************************************
    INCLUDES
*******************************************************************/
#include ".\hif\gp_ram.h"
#include ".\dvd\dvdef.h"
#include ".\al\REG_HAL.h"
#include ".\al\REG_demod.h"
#include ".\al\REG_dvd_decoder.h"
#include ".\player\plrDB.h"    /* Player:DB      */

/*-----------------------------------------------------------------
    MACROS
------------------------------------------------------------------*/
#if (ENABLE_HDDVD == 1)
#define DVD_READ_IRQC1   ((pdbDiscLoaded() == HD_DISC)?\
    (\
        (BYTE)(HAL_DVDD_ECCBLKEN|HAL_DVDD_UNECCEN|HAL_DVDD_BUFDNEN|HAL_DVDD_ECCDNEN|HAL_DVDD_ECCEN) \
    )\
    :\
    (\
            (BYTE)(HAL_DVDD_UNECCEN  | HAL_DVDD_BUFDNEN | HAL_DVDD_ECCDNEN | HAL_DVDD_ECCEN) \
    ))
#else
#define DVD_READ_IRQC1      (BYTE)(HAL_DVDD_UNECCEN  | HAL_DVDD_ECCDNEN | HAL_DVDD_ECCEN)            /* UNCORRECTABLE ECC, ECC DONE and ECC BLOCK */
#endif
#define DVD_READ_IRQC2      (BYTE)(HAL_DVDD_TIDMISEN | HAL_DVDD_EDCEREN | HAL_DVDD_DMDFIFLEN)                               /* TARGET ID MISS, EDC ERROR */
#define DVD_READ_IRQC3      (BYTE)(0)

/*-----------------------------------------------------------------
    DVD BUFFERING MACROS
------------------------------------------------------------------*/
#define SET_TARGET_ID(tid)              WRITE_FIELD( HAL_DVDD_TID , tid)    /* TBD  needs to be changed from TID0 to TID with next release of REG_dvd_decoder.h */

#define DISABLE_DVD_BUFFERING()         (WRITE_REG(HAL_DVDDREG_MAP->BUFCTRL,0x00))

#define ENABLE_DVD_BUFFERING()          (WRITE_REG(HAL_DVDDREG_MAP->BUFCTRL, HAL_DVDD_ENECCE | HAL_DVDD_ENBUF))

#define DVD_BUFFERING()                 ((READ_REG( HAL_DVDDREG_MAP->BUFCTRL ) & ( HAL_DVDD_BUFRNG | HAL_DVDD_ENECCE)) ? ON : OFF)

#define ENABLE_RAWDATA_BUFFERING()      (WRITE_REG(HAL_DVDDREG_MAP->BUFCTRL, HAL_DVDD_BUFRNG | HAL_DVDD_ENBUF ))

#define CLEAR_DVD_BLOCKS_TO_BUFFER()    WRITE_FIELD(HAL_DVDD_BKTOBUF ,0)

#define SET_DVD_BLOCKS_TO_BUFFER(count) WRITE_FIELD(HAL_DVDD_BKTOBUF , count)

#define SET_DISC_BUFFER_PTR(x)          WRITE_FIELD(HAL_DVDD_CURBUF , x )

/*-----------------------------------------------------------------
    DVD INTERRUPT CONTROL AND STATUS MACROS
------------------------------------------------------------------*/
#define DISABLE_DVD_DECODER_INTS()      WRITE_REG(HAL_DVDDREG_MAP->DVDIRQC1 ,0x00);\
                                        WRITE_REG(HAL_DVDDREG_MAP->DVDIRQC2 ,0x00);\
                                        WRITE_REG(HAL_DVDDREG_MAP->DVDIRQC3 ,0x00)

#define ENABLE_DVD_DECODER_INTS()       WRITE_REG(HAL_DVDDREG_MAP->DVDIRQC1 , DVD_READ_IRQC1 );\
                                        WRITE_REG(HAL_DVDDREG_MAP->DVDIRQC2 , DVD_READ_IRQC2 );\
                                        WRITE_REG(HAL_DVDDREG_MAP->DVDIRQC3 , DVD_READ_IRQC3 )

#define CLEAR_DVD_DECODER_STATUS()      WRITE_REG( HAL_DVDDREG_MAP->DVDIRQS1 ,0x00);\
                                        WRITE_REG( HAL_DVDDREG_MAP->DVDIRQS2 ,0x00);\
                                        WRITE_REG( HAL_DVDDREG_MAP->DVDIRQS3 ,0x00)


// Interrupt in Demodulator block for substitute capability

#define ENABLE_DVD_SYNC_LOST_INT()      WRITE_FIELD(HAL_DMOD_DMODOLKE , 1)      /* This is the Demodulator Sync Out of Lock Interrupt. */
                                                                                /*  Name ENABLE_DVD_SYNC_LOST_INT is retained for now to avoid confusion in existing code */
#define DISABLE_DVD_SYNC_LOST_INT()     WRITE_FIELD(HAL_DMOD_DMODOLKE , 0)      /* This is the Demodulator Sync Out of Lock Interrupt. */
                                                                                /*  Name DISABLE_DVD_SYNC_LOST_INT is retained for now to avoid confusion in existing code */
#define CLEAR_SYNC_LOST_STATUS()        CLEAR_INT_FIELD(HAL_DMOD_DMODOLKS , 0)

#define ENABLE_TARGET_MISS_INT()        WRITE_FIELD( HAL_DVDD_TIDMISEN , 1 )
#define DISABLE_TARGET_MISS_INT()       WRITE_FIELD( HAL_DVDD_TIDMISEN , 0 )

#define ENABLE_BUFI_INT()               WRITE_FIELD( HAL_DVDD_BUFFEN , 1 )
#define DISABLE_BUFI_INT()              WRITE_FIELD( HAL_DVDD_BUFFEN , 0 )

/*-----------------------------------------------------------------
    DVD DECODER CONTROL
------------------------------------------------------------------*/
#define DISABLE_DVD_DECODER()   WRITE_FIELD( HAL_DVDD_ENDVDD , 0 )
#define ENABLE_DVD_DECODER()    WRITE_FIELD( HAL_DVDD_ENDVDD , 1 )

/*-----------------------------------------------------------------
    GENERAL REGISTER MACROS
------------------------------------------------------------------*/
#define BIT_TEST(data,testbit)              (data & testbit)
#define BIT_SET(data,setbit)                (data |= setbit)
#define BIT_CLEAR(data,clearbit)            (data &= ~clearbit)

/*-----------------------------------------------------------------
    MACRO TO OBTAIN ABSOLUTE ADDRESS OF CURRENT DVD AUXILLARY BLOCK
    AND CURRENT DVD BLOCK BASED ON SOFTWARE POINTER DataBuffer.CurDiscPtr
------------------------------------------------------------------*/
#define CUR_ECC_BLK(ptr)    (ptr ? (ptr - ONE_DVD_BLOCK) : (MAX_DVD_SECTORS - ONE_DVD_BLOCK))
#define CUR_AUX_ADDRESS()   (DVD_ABS_AUX_START_ADDR + (CUR_ECC_BLK(DataBuffer.CurDiscPtr) * DVD_AUX_PTR_RESOLUTION))
#define CUR_ECC_ADDRESS()   (SCSDRAM_START_ADDR + (CUR_ECC_BLK(DataBuffer.CurDiscPtr) * DVD_DATA_BLK_PTR_RESOLUTION))
#define CUR_PIPO_ADDRESS()  (DVD_PIPO_START_ADDR + ((CUR_ECC_BLK(DataBuffer.CurDiscPtr)/ONE_DVD_BLOCK) * DVD_PIPO_PTR_RESOLUTION))

#if (ENABLE_HDDVD == 1)
#define CUR_HDECC_BLK(ptr)    (ptr ? (ptr - ONE_HDDVD_BLOCK) : (MAX_HDDVD_SECTORS - ONE_HDDVD_BLOCK))
#define CUR_HDAUX_ADDRESS()   (HDDVD_ABS_AUX_START_ADDR + (CUR_HDECC_BLK(DataBuffer.CurDiscPtr) * DVD_AUX_PTR_RESOLUTION))
#define CUR_HDECC_ADDRESS()   (SCSDRAM_START_ADDR + (CUR_HDECC_BLK(DataBuffer.CurDiscPtr) * DVD_DATA_BLK_PTR_RESOLUTION))
#define CUR_HDPIPO_ADDRESS()  (HDDVD_PIPO_START_ADDR + ((CUR_HDECC_BLK(DataBuffer.CurDiscPtr)/ONE_DVD_BLOCK) * DVD_PIPO_PTR_RESOLUTION))
#endif

/*-----------------------------------------------------------------
    MACRO TO OBTAIN ABSOLUTE ADDRESS OF DVD AUX BLOCK AND DATA BLOCKS
    x = Relative data block pointer (example: CURECC)
------------------------------------------------------------------*/
#define DVD_AUX_ADDRESS(x)    (DVD_ABS_AUX_START_ADDR + (x * DVD_AUX_PTR_RESOLUTION))
#define DVD_ECC_ADDRESS(x)    (SCSDRAM_START_ADDR + (x * DVD_DATA_BLK_PTR_RESOLUTION))

#if (ENABLE_HDDVD == 1)
#define HDDVD_AUX_ADDRESS(x)    (HDDVD_ABS_AUX_START_ADDR + (x * HDDVD_AUX_PTR_RESOLUTION))
#define HDDVD_ECC_ADDRESS(x)    (SCSDRAM_START_ADDR + (x * HDDVD_DATA_BLK_PTR_RESOLUTION))
#endif
/*-----------------------------------------------------------------
    READ LONG MACROS
------------------------------------------------------------------*/
#define DVD_ENABLE_READ_LONG()  WRITE_FIELD( HAL_DVDD_RDLONG , 1);\
                                dvdDecFlag.fREAD_LONG_COMMAND = TRUE

#define DVD_DISABLE_READ_LONG() WRITE_FIELD( HAL_DVDD_RDLONG , 0);\
                                dvdDecFlag.fREAD_LONG_COMMAND = FALSE

/*-----------------------------------------------------------------
    READ TIMER MACROS
------------------------------------------------------------------*/
#define START_DVD_BLOCK_TIMER(x)    dvdBlockInterruptTimer.count = x;\
                                    dvdBlockInterruptTimer.fTIMEOUT = CLEAR;\
                                    dvdBlockInterruptTimer.state = ON

#define STOP_DVD_BLOCK_TIMER()      dvdBlockInterruptTimer.state = OFF

#endif
