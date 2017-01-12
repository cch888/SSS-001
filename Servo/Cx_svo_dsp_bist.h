/****************************************************************************
*                (c) Copyright 2001 - 2007 SUNEXT DESIGN, INC.
*                           All Rights Reserved
*
*
* FILENAME:   svo_dsp_bist.h
*
* DESCRIPTION: Servo DSP BIST configuration data
*
* $Revision:
* DATE:    03-18-2010
* TIME:    12:55:04
*
**************************** SUNEXT CONFIDENTIAL ****************************/
#ifndef  _Cx_SVO_DSP_BIST_
#define  _Cx_SVO_DSP_BIST_

#define  Cx_BIST_DSP_VERSION_STRING     "1.04"
#define  Cx_BIST_DSP_INSTRUCTION_SIZE    512 
#define  Cx_BIST_COEF_INPUT_SIZE         16 
#define  Cx_BIST_CHECK_START_ADDRESS     0x0100
#define  Cx_BIST_CHECK_SIZE              64 
#define  Cx_BIST_LOOP_COUNTER_ADDRESS    0x003C

extern const ULONG Cx_BIST_ServoDSP_code[512];
extern const USHORT Cx_BIST_ServoDSP_input[16];
extern const USHORT Cx_BIST_ServoDSP_output[64];
extern const char *Cx_BIST_ServoDSP_Error[64];

#define  Cx_BIST_DSP_POWER_MODE         0x00     //power mode: 0=normal 1=low_pwr 2= full_spd
// ADDRESS INFORMATION
#define  Cx_BIST_DSP_PHAADDR            0x50000   // phase A start address and length 
#define  Cx_BIST_DSP_PHB1ADDR           0x50005   // phase B1 start address and length 
#define  Cx_BIST_DSP_PHB2ADDR           0x5000A   // phase B2 start address and length 
#define  Cx_BIST_DSP_PHC1ADDR           0x5000F   // phase C1 start address and length 
#define  Cx_BIST_DSP_PHC2ADDR           0x50014   // phase C2 start address and length 
#define  Cx_BIST_DSP_PHC3ADDR           0x50019   // phase C3 start address and length 
#define  Cx_BIST_DSP_PHC4ADDR           0x5001E   // phase C4 start address and length 
#define  Cx_BIST_DSP_PHDADDR            0x2DD0023   // phase D start address and length

#endif /* _SVO_DSP_BIST_ */
// END OF FILE
