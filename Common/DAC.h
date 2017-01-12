/****************************************************************************
*                (c) Copyright 2001,2002 2003, 2004, 2005 SUNEXT DESIGN, INC.
*                           All Right Reserved
*
* $Revision: 60 $
* $Date: 11/04/06 2:04p $
*
* DESCRIPTION:
*   Common routines to deal with Serial and Parallel Digital to Analog Converters
*
* NOTES:
*
**************************** SUNEXT CONFIDENTIAL *******************************/

#ifndef _DAC_H_
#define _DAC_H_

#define DAC_MAX_LEVEL           0xFF
#define DAC_VREF_LEVEL          0x80
#define DAC_GND_LEVEL           0x00
#define DAC_MAX_LEVEL10         0x03FF
#define DAC_VREF_LEVEL10        0x0200  /* VREF for 10 bit DAC Center voltage */
#define DAC_GND_LEVEL10         0x0000

void InitGPDAC(void);

#if (COLLI_SWITCH == 1)
    #if (PCB == 815)
#define Colli_OutputDac(dac_a, dac_b)   (WRITE_FIELD(HAL_SRV_PDMALPHA1, (dac_b)),\
                                         WRITE_FIELD(HAL_SRV_PDMALPHA2, (dac_a)))
    #else
#define Colli_OutputDac(dac_a, dac_b)   (WRITE_FIELD(HAL_SRV_PDMALPHA1, (dac_a)),\
                                         WRITE_FIELD(HAL_SRV_PDMALPHA2, (dac_b)))
    #endif
#endif

#if ((PCB == 1600)||(PCB == 812))
#define svoStp_OutputDac(dac_a, dac_b)  (WRITE_FIELD(HAL_SRV_DACDAT1, (dac_b )), \
                                         WRITE_FIELD(HAL_SRV_DACDAT2, (dac_a )))
#else
#define svoStp_OutputDac(dac_a, dac_b)  (WRITE_FIELD(HAL_SRV_DACDAT1, (dac_a )), \
                                         WRITE_FIELD(HAL_SRV_DACDAT2, (dac_b )))
#endif//PCB

#if ((PCB == 690)||(PCB == 692)||(PCB == 691)||(PCB == 812) )
#define svoTray_OutputDac(dac)          (WRITE_FIELD(HAL_SRV_DACDAT1, -(dac) ))
#define svoGetTrayDac()                 (-READ_FIELD(HAL_SRV_DACDAT1))
#else
#define svoTray_OutputDac(dac)          (WRITE_FIELD(HAL_SRV_DACDAT1, (dac) ))
#define svoGetTrayDac()                 (READ_FIELD(HAL_SRV_DACDAT1))
#endif

#endif /* _DAC_H_ */


/*------------------------------------------------------------------------*/
/* The end of file                                                        */
/*------------------------------------------------------------------------*/
