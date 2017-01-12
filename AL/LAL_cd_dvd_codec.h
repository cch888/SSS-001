/****************************************************************************
*                (c) Copyright 2001,2002,2003, 2004, 2005 SUNEXT DESIGN, INC.
*                           All Rights Reserved
*
*
* FILENAME:   LAL_cd_dvd_codec.h
*
* DESCRIPTION
*   This file supports the LAL CD&DVD CODEC funtions implemented in the LAL CD&DVD CODEC layer
*
* NOTES:
*
* $Revision: 2 $
* $Date: 07/09/19 3:34p $
*
*
**************************** SUNEXT CONFIDENTIAL ****************************/
#ifndef _LALCDDVDCODEC_H_
#define _LALCDDVDCODEC_H_


//definition for HAL_CDD_C2CG
#define FOUR_SYM_C2_ECC_SRCHX   0x00
#define THREE_SYM_C2_ECC_SRCHX  0x01
#define FOUR_SYM_C2_ECC_SRCH0   0x02
#define TWO_SYM_C2_ECC_SRCH1_2  0x02
#define THREE_SYM_C2_ECC_SRCH0  0x03
#define TWO_SYM_C2_ECC_SRCH1_3  0x03

enum    //definition for HAL_CDD_C1CG
{
    e_C1CG1_0,
    e_C1CG1_1,
    e_C1CG1_2,
    e_C1CG1_3
};

enum    //definition for HAL_CDD_C2P
{
    e_C2_PARITY = 0x00,
    e_OR_ALL_C2_BYTES = 0x01,
    e_C2_ERROR_COUNT = 0x02
};

enum    //definition for HAL_CDD_MLY
{
    e_Page3K = 0x02,
    e_Page2_HalfK
};


extern void LAL_CD_DVD_CODEC_Init(void);

#endif /* _LALPLL_H_ */

