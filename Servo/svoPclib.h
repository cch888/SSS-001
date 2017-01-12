/****************************************************************************
*                (c) Copyright 2001 - 2007 SUNEXT DESIGN, INC.
*                           All Right Reserved
*
*
* FILENAME:   svoPclib.H
*
* $Revision: 35 $
* $Date: 11/03/10 6:25p $
*
* DESCRIPTION: Power Calibbration
*
* NOTES:
*
* PROGRAMMER(s):
*
* CHANGES:
* VERSION   DATE        WHO     DETAIL
* -------   --------    ------  ---------------------------------------------
*    1.00   Aug-27-04           Create
*
**************************** SUNEXT CONFIDENTIAL ****************************/
#ifndef _SVOPCLIB_
#define _SVOPCLIB_
#include ".\oem\oem_tbl.h"

#define VWDC2ATTLEV_CD      0xBF
#define VWDC2ATTLEV_DVD     0x80

#define VWPOW_LOWGAIN       0x00 // VWPOW GAIN  -8dB
#define VWPOW_MIDGAIN       0x10 // VWPOW GAIN  0dB
#define VWPOW_HIGHGAIN      0x1F // VWPOW GAIN  7.5dB

#define WRITE_INITIAL_CAL  600
#if (PersistentTable_Revision > 6)
#define READ_CH_START_DAC   1
#define WRITE_CH_START_DAC  1
#endif

#define DAC_VOLTAGE         3300                    //DAC Output
#define DAC_RANGE           32767                   //15bits
#define DAC_RESOLUTION      DAC_VOLTAGE/DAC_RANGE

#if (PCB == 673)
#define CD_LIMIT_VOL        2100
#define MAX_CD_THRESHOLD    CD_LIMIT_VOL*DAC_RANGE/DAC_VOLTAGE
#define MAX_CD_DELTA0       MAX_CD_THRESHOLD
#define MAX_CD_DELTA1       MAX_CD_THRESHOLD
#define MAX_CD_DELTA2       MAX_CD_THRESHOLD

#define DVD_LIMIT_VOL       2800
#define MAX_DVD_THRESHOLD   DVD_LIMIT_VOL*DAC_RANGE/DAC_VOLTAGE
#define MAX_DVD_DELTA0      MAX_DVD_THRESHOLD
#define MAX_DVD_DELTA1      MAX_DVD_THRESHOLD
#define MAX_DVD_DELTA2      MAX_DVD_THRESHOLD
#endif

#if (OPU == HOP7432TL || OPU == HOP7232TL) //Protect HOP-7432TL and HOP7232(CD channel VRDC <1.6V)
#define LIMIT_MV           1600
#elif OPU == HOPH353L //Protect HOPH353L (CD channel VRDC <2.0V)
#define LIMIT_MV            2000
#endif

#define DVD_BIOS_POWER      70      // DVD Bios Power = value/100 in mW
#define CD_BIOS_POWER       70      // CD Bios Power = value/100 in mW

//TBD? COMMENT! //temp setting for current AP and it will be deleted when new AP is ready (FPDO)
#define FPDO_CD1MW         13
#define FPDO_CD7MW         94
#define FPDO_CD30MW        431

#define FPDO_DVD1MW         13
#define FPDO_DVD7MW         94
#define FPDO_DVD30MW        431
#define FPDO_DVD50MW        744
#define CD_FPDOGAIN         (35.3/100) //mW
//------------------------------------------------

/************************************
    Prototype Functions
*************************************/
void   Cal_LoadOfflineCalibrationParam(void);
void   z_calRead_fgDoneBits(void);

#endif //_SVOPCLIB_

