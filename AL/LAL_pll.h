/****************************************************************************
*                (c) Copyright 2001,2002,2003, 2004, 2005 SUNEXT DESIGN, INC.
*                           All Rights Reserved
*
*
* FILENAME:   LAL_pll.h
*
* DESCRIPTION
*   This file supports the LAL PLL funtions implemented in the LAL PLL layer
*
* NOTES:
*
* $Revision: 17 $
* $Date: 11/03/11 2:32p $
*
*
**************************** SUNEXT CONFIDENTIAL ****************************/
#ifndef _LALPLL_H_
#define _LALPLL_H_

#include ".\oem\oem_info.h"
#include ".\servo\SVO.H"            /* Needed for types of parameters */
#include ".\al\coefs.h"
#include ".\al\HAL_dsp.h"
#include ".\al\HAL_pll.h"

#define CD_BITRATEDIV100        43218
#define DVD_BITRATEDIV100       261562
#define DVDRAM_BITRATEDIV100    291800
#define BD_BITRATEDIV100        660000
#define HDDVD_BITRATEDIV100     648000

enum                                /* Select the reference clock input */
{
    e_SATCLKIN_EXT,                 /* use the external clock (xtali)  */
    e_SATCLKIN_INT                  /* use the internal clock (intxtal), */
};

enum                                /* PLL Charge Pump Bias current */
{
    e_SATPLLBIS_100,                /* 100uA (min),  */
    e_SATPLLBIS_200 = 4,            /* 200uA (default) */
    e_SATPLLBIS_275 = 7             /* 275uA (max) */
};

enum                                /* Change PLL parameters */
{
    e_SATPLLCFG_NOCHANGE,           /* no change */
    e_SATPLLCFG_REXTREFCHANGE,      /* change REXT reference */
    e_SATPLLCFG_PLLBWCHANGE,        /* change PLL bandwidth */
    e_SATPLLCFG_BW_REF_CHANGE       /* change REXT reference and change PLL bandwidth */
};

enum                                /* Enable PLL/VCO test */
{
    e_SATPLLTST_TESTMODE_OFF,
    e_SATPLLTST_TESTMODE_ON
};

enum                                /* reference clock frequency */
{
    e_REFCLKMOD_25MHz,
    e_REFCLKMOD_100MHz
};

enum                                /* Control/Test SSC circuits */
{
    e_SSCCTL_OFF,
    e_SSCCTL_ON = 15
};

enum                        // Define a related enum in REG_a7soc.h?
{
    e_ADCCLKDIV_DIV_BY_1,
    e_ADCCLKDIV_DIV_BY_2,
    e_ADCCLKDIV_DIV_BY_4,
    e_ADCCLKDIV_DIV_BY_8,
    e_ADCCLKDIV_DIV_BY_16,
    e_ADCCLKDIV_DIV_BY_32
};

enum                        // Define a related enum in REG_a7soc.h?
{
    e_RSET_128K,
    e_RSET_96K,
    e_RSET_64K,             // ! under discussion
    e_RSET_32K              // ! under discussion
};

enum                        // Define a related enum in REG_a7soc.h?
{
    e_WOCLKDIV_DIV_BY_1,
    e_WOCLKDIV_DIV_BY_2,
    e_WOCLKDIV_DIV_BY_4
};

enum                        // Define a related enum in REG_a7soc.h?
{
    e_WIFCLKDIV_DIV_BY_1,
    e_WIFCLKDIV_DIV_BY_2,
    e_WIFCLKDIV_DIV_BY_4
};

enum                        // Define a related enum in REG_a7soc.h?
{
    e_WRTCLKDIV_DIV_BY_1,
    e_WRTCLKDIV_DIV_BY_2,
    e_WRTCLKDIV_DIV_BY_4
};

/* define servo PLL table
 * COEFS values for CD
 * COEFS values DVD
 * COEFS values DVDRAM
 * COEFS values BD
 * COEFS values HDDVD
 *
 *                      BaseN BaseM AdcClkDiv WrtClkDivValue Qpr Qpl RSet WblAdcClkDivValue Bitrate
 *                           WoClkDiv[SIZE_WOCLKDIV_FIELD]
 *                           WifClkDiv[SIZE_WIFCLKDIV_FIELD]
 *                           WrtClkDiv[SIZE_WRTCLKDIV_FIELD]
 */
#if (HIGH_ADC_CLK_BD == 1)
#define PLL_COEFS   { { {    20, 12, 3, 16, e_ADCCLKDIV_DIV_BY_2, 1, 1, 12, e_RSET_128K, 2, CD_BITRATEDIV100, },    \
                          { e_WOCLKDIV_DIV_BY_2,  e_WOCLKDIV_DIV_BY_2,  e_WOCLKDIV_DIV_BY_2,  e_WOCLKDIV_DIV_BY_2},     \
                          { e_WIFCLKDIV_DIV_BY_1, e_WIFCLKDIV_DIV_BY_1, e_WIFCLKDIV_DIV_BY_1, e_WIFCLKDIV_DIV_BY_1},    \
                          { e_WRTCLKDIV_DIV_BY_1, e_WRTCLKDIV_DIV_BY_1, e_WRTCLKDIV_DIV_BY_1, e_WRTCLKDIV_DIV_BY_1}, }, \
                      { {    20, 14, 16, 14, e_ADCCLKDIV_DIV_BY_2, 1, 1, 12, e_RSET_128K, 2, DVD_BITRATEDIV100, }, \
                          { e_WOCLKDIV_DIV_BY_2,  e_WOCLKDIV_DIV_BY_2,  e_WOCLKDIV_DIV_BY_2,  e_WOCLKDIV_DIV_BY_2},     \
                          { e_WIFCLKDIV_DIV_BY_2, e_WIFCLKDIV_DIV_BY_2, e_WIFCLKDIV_DIV_BY_2, e_WIFCLKDIV_DIV_BY_2},    \
                          { e_WRTCLKDIV_DIV_BY_1, e_WRTCLKDIV_DIV_BY_1, e_WRTCLKDIV_DIV_BY_1, e_WRTCLKDIV_DIV_BY_1}, }, \
                      { {    20, 16, 18, 14, e_ADCCLKDIV_DIV_BY_2, 1, 1, 12, e_RSET_128K, 2, DVDRAM_BITRATEDIV100, },   \
                          { e_WOCLKDIV_DIV_BY_2,  e_WOCLKDIV_DIV_BY_2,  e_WOCLKDIV_DIV_BY_2,  e_WOCLKDIV_DIV_BY_2},     \
                          { e_WIFCLKDIV_DIV_BY_2, e_WIFCLKDIV_DIV_BY_2, e_WIFCLKDIV_DIV_BY_2, e_WIFCLKDIV_DIV_BY_2},    \
                          { e_WRTCLKDIV_DIV_BY_1, e_WRTCLKDIV_DIV_BY_1, e_WRTCLKDIV_DIV_BY_1, e_WRTCLKDIV_DIV_BY_1}, }, \
                      { {    32, 10, 25, 13, e_ADCCLKDIV_DIV_BY_1, 4, 1, 12, e_RSET_128K, 4, BD_BITRATEDIV100, }, \
                          { e_WOCLKDIV_DIV_BY_4,  e_WOCLKDIV_DIV_BY_2,  e_WOCLKDIV_DIV_BY_1,  e_WOCLKDIV_DIV_BY_1},     \
                          { e_WIFCLKDIV_DIV_BY_4, e_WIFCLKDIV_DIV_BY_2, e_WIFCLKDIV_DIV_BY_1, e_WIFCLKDIV_DIV_BY_1},    \
                          { e_WRTCLKDIV_DIV_BY_4, e_WRTCLKDIV_DIV_BY_2, e_WRTCLKDIV_DIV_BY_1, e_WRTCLKDIV_DIV_BY_1}, }, \
                      { {    20, 10, 25, 13, e_ADCCLKDIV_DIV_BY_1, 4, 1, 12, e_RSET_128K, 4, HDDVD_BITRATEDIV100, },  \
                          { e_WOCLKDIV_DIV_BY_4,  e_WOCLKDIV_DIV_BY_2,  e_WOCLKDIV_DIV_BY_1,  e_WOCLKDIV_DIV_BY_1},     \
                          { e_WIFCLKDIV_DIV_BY_4, e_WIFCLKDIV_DIV_BY_2, e_WIFCLKDIV_DIV_BY_1, e_WIFCLKDIV_DIV_BY_1},    \
                          { e_WRTCLKDIV_DIV_BY_4, e_WRTCLKDIV_DIV_BY_2, e_WRTCLKDIV_DIV_BY_1, e_WRTCLKDIV_DIV_BY_1}, }  \
                    }
#else
#define PLL_COEFS   { { {    20, 12, 3, 16, e_ADCCLKDIV_DIV_BY_2, 1, 1, 12, e_RSET_128K, 2, CD_BITRATEDIV100, },    \
                          { e_WOCLKDIV_DIV_BY_2,  e_WOCLKDIV_DIV_BY_2,  e_WOCLKDIV_DIV_BY_2,  e_WOCLKDIV_DIV_BY_2},     \
                          { e_WIFCLKDIV_DIV_BY_1, e_WIFCLKDIV_DIV_BY_1, e_WIFCLKDIV_DIV_BY_1, e_WIFCLKDIV_DIV_BY_1},    \
                          { e_WRTCLKDIV_DIV_BY_1, e_WRTCLKDIV_DIV_BY_1, e_WRTCLKDIV_DIV_BY_1, e_WRTCLKDIV_DIV_BY_1}, }, \
                      { {    20, 14, 16, 14, e_ADCCLKDIV_DIV_BY_2, 1, 1, 12, e_RSET_128K, 2, DVD_BITRATEDIV100, }, \
                          { e_WOCLKDIV_DIV_BY_2,  e_WOCLKDIV_DIV_BY_2,  e_WOCLKDIV_DIV_BY_2,  e_WOCLKDIV_DIV_BY_2},     \
                          { e_WIFCLKDIV_DIV_BY_2, e_WIFCLKDIV_DIV_BY_2, e_WIFCLKDIV_DIV_BY_2, e_WIFCLKDIV_DIV_BY_2},    \
                          { e_WRTCLKDIV_DIV_BY_1, e_WRTCLKDIV_DIV_BY_1, e_WRTCLKDIV_DIV_BY_1, e_WRTCLKDIV_DIV_BY_1}, }, \
                      { {    20, 16, 18, 14, e_ADCCLKDIV_DIV_BY_2, 1, 1, 12, e_RSET_128K, 2, DVDRAM_BITRATEDIV100, },   \
                          { e_WOCLKDIV_DIV_BY_2,  e_WOCLKDIV_DIV_BY_2,  e_WOCLKDIV_DIV_BY_2,  e_WOCLKDIV_DIV_BY_2},     \
                          { e_WIFCLKDIV_DIV_BY_2, e_WIFCLKDIV_DIV_BY_2, e_WIFCLKDIV_DIV_BY_2, e_WIFCLKDIV_DIV_BY_2},    \
                          { e_WRTCLKDIV_DIV_BY_1, e_WRTCLKDIV_DIV_BY_1, e_WRTCLKDIV_DIV_BY_1, e_WRTCLKDIV_DIV_BY_1}, }, \
                      { {    20, 10, 25, 13, e_ADCCLKDIV_DIV_BY_1, 4, 1, 12, e_RSET_128K, 4, BD_BITRATEDIV100, }, \
                          { e_WOCLKDIV_DIV_BY_4,  e_WOCLKDIV_DIV_BY_2,  e_WOCLKDIV_DIV_BY_1,  e_WOCLKDIV_DIV_BY_1},     \
                          { e_WIFCLKDIV_DIV_BY_4, e_WIFCLKDIV_DIV_BY_2, e_WIFCLKDIV_DIV_BY_1, e_WIFCLKDIV_DIV_BY_1},    \
                          { e_WRTCLKDIV_DIV_BY_4, e_WRTCLKDIV_DIV_BY_2, e_WRTCLKDIV_DIV_BY_1, e_WRTCLKDIV_DIV_BY_1}, }, \
                      { {    20, 10, 25, 13, e_ADCCLKDIV_DIV_BY_1, 4, 1, 12, e_RSET_128K, 4, HDDVD_BITRATEDIV100, },  \
                          { e_WOCLKDIV_DIV_BY_4,  e_WOCLKDIV_DIV_BY_2,  e_WOCLKDIV_DIV_BY_1,  e_WOCLKDIV_DIV_BY_1},     \
                          { e_WIFCLKDIV_DIV_BY_4, e_WIFCLKDIV_DIV_BY_2, e_WIFCLKDIV_DIV_BY_1, e_WIFCLKDIV_DIV_BY_1},    \
                          { e_WRTCLKDIV_DIV_BY_4, e_WRTCLKDIV_DIV_BY_2, e_WRTCLKDIV_DIV_BY_1, e_WRTCLKDIV_DIV_BY_1}, }  \
                    }
#endif
extern void LAL_PLL_InitServo(void);
extern void LAL_PLL_InitHif(void);
extern void LAL_PLL_SetPll( void );

#endif /* _LALPLL_H_ */

