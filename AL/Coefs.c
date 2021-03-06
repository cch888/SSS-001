/****************************************************************************
*                (c) Copyright 2006 - 2007 SUNEXT DESIGN, INC.
*                           All Rights Reserved
*
*
* FILENAME:   coefs.c
*
* DESCRIPTION
*
* NOTES:
*
* $Revision: 55 $
* $Date: 11/03/16 4:46p $
*
*
**************************** SUNEXT CONFIDENTIAL ****************************/
009
/*****************************************************************************
*                    Include Definitions
*****************************************************************************/

#include ".\oem\oem_info.h"
#include ".\al\coefs.h"
#include ".\Common\globals.h"
#include ".\oem\coef_values.h"

#include ".\oem\oem_tbl.h"
#include ".\al\REG_HAL.h"
#include ".\al\reg_global.h"
#include ".\al\REG_cd_decoder.h"

#include ".\al\lal.h"
#include ".\DVD\dvMacros.h"

004
005
006

007
008



/*****************************************************************************
*                    Debug Definitions
*****************************************************************************/


/*****************************************************************************
*                    Macro Definitions
*****************************************************************************/

/*****************************************************************************
*                    Static Data Definitions
*****************************************************************************/

static BOOL bCoefsInitialised = FALSE;

static CONST ULONG CoreBaseAddresses[E_CORE_COUNT] =
{
   /* E_CORE_AFE     */ HAL_AFE_BaseAddress,
   /* E_CORE_DFE     */ HAL_DFE_BaseAddress,
   /* E_CORE_WOBBLE  */ HAL_WBL_BaseAddress,
   /* E_CORE_SRV     */ HAL_SRV_BaseAddress,
   /* E_CORE_SRVDSP  */ HAL_SRVDSP_BaseAddress
};

static CONST DiscGroupT CoefDiscKindToDiscGroup[LAST_MEDIA]=
{
    /*  e_CD_ROM    */      DiscGroupCD     ,
    /*  e_CD_R      */      DiscGroupCD     ,
    /*  e_CD_RW     */      DiscGroupCD     ,

    /*  e_DVD_ROM   */      DiscGroupDVD    ,
    /*  e_DVD_pR    */      DiscGroupDVD    ,
    /*  e_DVD_pRW   */      DiscGroupDVD    ,
    /*  e_DVD_mR    */      DiscGroupDVD    ,
    /*  e_DVD_mRW   */      DiscGroupDVD    ,
    /*  e_DVD_DL    */      DiscGroupDVD    ,
    /*  e_DVD_pR_DL */      DiscGroupDVD    ,
    /*  e_DVD_pRW_DL*/      DiscGroupDVD    ,
    /*  e_DVD_mR_DL */      DiscGroupDVD    ,
    /*  e_DVD_mRW_DL*/      DiscGroupDVD    ,

    /*  e_DVDRAM_RAM*/      DiscGroupDVDRAM ,

#if (BD_ENABLE == 1)
    /*  e_BD_RO_SL  */      DiscGroupBD     ,
    /*  e_BD_RO_DL  */      DiscGroupBD     ,
    /*  e_BD_R_SL   */      DiscGroupBD     ,
    /*  e_BD_R_DL   */      DiscGroupBD     ,
    /*  e_BD_RE_SL  */      DiscGroupBD     ,
    /*  e_BD_RE_DL  */      DiscGroupBD     ,
#endif // (BD_ENABLE == 1)

#if (ENABLE_HDDVD == 1)
    /*  e_HD_RO_SL  */      DiscGroupHD     ,
    /*  e_HD_RO_DL  */      DiscGroupHD     ,
#if (ENABLE_CBHD == 1)
    /*  e_CBHD_RO_SL  */    DiscGroupHD     ,
    /*  e_CBHD_RO_DL  */    DiscGroupHD     ,
#endif
    /*  e_HD_R_SL   */      DiscGroupHD     ,
    /*  e_HD_R_DL   */      DiscGroupHD     ,
    /*  e_HD_RW_SL  */      DiscGroupHD     ,
    /*  e_HD_RW_DL  */      DiscGroupHD
    ,
    /*  e_HD_RAM    */      DiscGroupHD
#endif //(ENABLE_HDDVD == 1)
};

static CONST eDiscKind CoefDiscKindToServo[LAST_MEDIA]=
{
    /*  e_CD_ROM    */      eCDROMDisc,
    /*  e_CD_R      */      eCDRDisc,
    /*  e_CD_RW     */      eCDRWDisc,

    /*  e_DVD_ROM   */      eDVDROMDisc,
    /*  e_DVD_pR    */      eDVDPlusRDisc,
    /*  e_DVD_pRW   */      eDVDPlusRWDisc,
    /*  e_DVD_mR    */      eDVDMinusRDisc,
    /*  e_DVD_mRW   */      eDVDMinusRWDisc,
    /*  e_DVD_DL    */      eDVDROMDLDisc,
    /*  e_DVD_pR_DL */      eDVDPlusRDLDisc,
    /*  e_DVD_pRW_DL*/      eDVDPlusRWDLDisc,
    /*  e_DVD_mR_DL */      eDVDMinusRDLDisc,
    /*  e_DVD_mRW_DL*/      eDVDMinusRWDLDisc,

    /*  e_DVDRAM_RAM*/      eDVDRAMDisc,

#if (BD_ENABLE == 1)
    /*  e_BD_RO_SL  */      eBDROMDisc,
    /*  e_BD_RO_DL  */      eBDROMDLDisc,
    /*  e_BD_R_SL   */      eBDRDisc,
    /*  e_BD_R_DL   */      eBDRDLDisc,
    /*  e_BD_RE_SL  */      eBDREDisc,
    /*  e_BD_RE_DL  */      eBDREDLDisc,
#endif // (BD_ENABLE == 1)

#if (ENABLE_HDDVD == 1)
    /*  e_HD_RO_SL  */      eHDROMDisc,
    /*  e_HD_RO_DL  */      eHDROMDLDisc,
#if (ENABLE_CBHD == 1)
    /*  e_CBHD_RO_SL  */    eCBHDROMDisc,
    /*  e_CBHD_RO_DL  */    eCBHDROMDLDisc,
#endif
    /*  e_HD_R_SL   */      eHDRDisc,
    /*  e_HD_R_DL   */      eHDRDLDisc,
    /*  e_HD_RW_SL  */      eHDRWDisc,
    /*  e_HD_RW_DL  */      eHDRWDLDisc,
    /*  e_HD_RAM    */      DiscGroupHD
#endif //(ENABLE_HDDVD == 1)
};

static CONST COEFS_SPEED_t SpeedToCoefSpeedTypeCD[MAX_SPEED_CD+1]=
{
    /*   NONE   0   ILLEGAL_SPEED   */      {   ILLEGAL_SPEED },
    /*   _4LX   1   4xCLV   */              {   CD_4xCLV      },
    /*   _8LX   2   8xCLV   */              {   CD_8xCLV      },
    /*   _12LX  3   12xCLV  */              {   CD_12xCLV     },
    /*   _16LX  4   16xCLV  */              {   CD_16xCLV     },
    /*   _10AX  5   10xCAV  */              {   CD_10xCAV     },
    /*   _16AX  6   16xCAV  */              {   CD_16xCAV     },
    /*   _24AX  7   24xCAV  */              {   CD_24xCAV     },
    /*   _32AX  8   32xCAV  */              {   CD_32xCAV     },
    /*   _40AX  9   40xCAV  */              {   CD_40xCAV     },
    /*   _48AX  10  48xCAV  */              {   CD_48xCAV     },
    /*   _52AX  11  52xCAV  */              {   CD_52xCAV     }
};

static CONST COEFS_SPEED_t SpeedToCoefSpeedTypeDVD[MAX_SPEED_DVD+1]=
{
    /*   NONE       0   ILLEGAL_SPEED*/ {   ILLEGAL_SPEED },
    /*   _2LX_DVD   1   2X CLV  */      {   DVD_2xCLV     },
    /*   _3LX_DVD   2   3X CLV  */      {   DVD_3xCLV     },
    /*   _4LX_DVD   3   4X CLV  */      {   DVD_4xCLV     },
    #if (ENABLE_DVD_4XCAV == 1)
    /*   _4AX_DVD   4   4xCAV   */      {   DVD_4xCAV     },
    /*   _6AX_DVD   5   6xCAV   */      {   DVD_6xCAV     },
    /*   _8AX_DVD   6   8xCAV   */      {   DVD_8xCAV     },
    /*   _10AX_DVD  7   10xCAV  */      {   DVD_10xCAV    },
    /*   _12AX_DVD  8   12xCAV  */      {   DVD_12xCAV    },
    /*   _16AX_DVD  9   16xCAV  */      {   DVD_16xCAV    },
    /*   _20AX_DVD  10  20xCAV  */      {   DVD_20xCAV    },
    #else
    /*   _6AX_DVD   4   6X CAV  */      {   DVD_6xCAV     },
    /*   _8AX_DVD   5   8X CAV  */      {   DVD_8xCAV     },
    /*   _10AX_DVD  6   10X CAV */      {   DVD_10xCAV    },
    /*   _12AX_DVD  7   12X CAV */      {   DVD_12xCAV    },
    /*   _16AX_DVD  8   16X CAV */      {   DVD_16xCAV    },
    /*   _20AX_DVD  9   20X CAV */      {   DVD_20xCAV    }
    #endif
};

static CONST COEFS_SPEED_t SpeedToCoefSpeedTypeDVDRAM[MAX_SPEED_DVDRAM+1]=
{
    /* NONE 0   ILLEGAL_SPEED   */      {   ILLEGAL_SPEED   },
    /* _1LX_DVDRAM      1       */      {   DVDRAM_1xCLV    },
    /* _2LX_DVDRAM      2       */      {   DVDRAM_2xCLV    },
    /* _3LX_DVDRAM      3       */      {   DVDRAM_3xCLV    },
    /* _5LX_DVDRAM      4       */      {   DVDRAM_5xCLV    },
    /* _5AX_DVDRAM      5       */      {   DVDRAM_5xCAV    },
    /* _8AX_DVDRAM      6       */      {   DVDRAM_8xCAV    },
    /* _12AX_DVDRAM     7       */      {   DVDRAM_12xCAV   },
    /* _16AX_DVDRAM     8       */      {   DVDRAM_16xCAV   }
};

#if (BD_ENABLE == 1)
static CONST COEFS_SPEED_t SpeedToCoefSpeedTypeBD[MAX_SPEED_BD+1]=
{
#if(NUMBER_OF_BD_DISC_SPEEDS == 3)
    /* NONE 0   ILLEGAL_SPEED   */      {   ILLEGAL_SPEED },
    /*  _1LX_BD     1           */      {   BD_1xCLV      },
    /*  _2LX_BD     2           */      {   BD_2xCLV      },
    /*  _4AX_BD     3           */      {   BD_4xCAV      }
#elif(NUMBER_OF_BD_DISC_SPEEDS == 8)
    /* NONE 0   ILLEGAL_SPEED   */      {   ILLEGAL_SPEED },
    /*  _1LX_BD     1           */      {   BD_1xCLV      },
    /*  _2LX_BD     2           */      {   BD_2xCLV      },
    /*  _4AX_BD     3           */      {   BD_4xCAV      },
    /*  _6AX_BD     4           */      {   BD_6xCAV      },
    /*  _8AX_BD     5           */      {   BD_8xCAV      },
    /*  _10AX_BD    6           */      {   BD_10xCAV     },
    /*  _12AX_BD    7           */      {   BD_12xCAV     },
    /*  _14AX_BD    8           */      {   BD_14xCAV     }
#elif(NUMBER_OF_BD_DISC_SPEEDS == 9)
    /* NONE 0   ILLEGAL_SPEED   */      {   ILLEGAL_SPEED },
    /*  _1LX_BD     1           */      {   BD_1xCLV      },
    /*  _2LX_BD     2           */      {   BD_2xCLV      },
    /*  _3LX_BD     3           */      {   BD_3xCLV      },
    /*  _4AX_BD     4           */      {   BD_4xCAV      },
    /*  _6AX_BD     5           */      {   BD_6xCAV      },
    /*  _8AX_BD     6           */      {   BD_8xCAV      },
    /*  _10AX_BD    7           */      {   BD_10xCAV     },
    /*  _12AX_BD    8           */      {   BD_12xCAV     },
    /*  _14AX_BD    9           */      {   BD_14xCAV     }
#endif
};
#endif

#if (ENABLE_HDDVD == 1)
static CONST COEFS_SPEED_t SpeedToCoefSpeedTypeHD[MAX_SPEED_HD+1]=
{
    /* NONE 0   ILLEGAL_SPEED   */      {   ILLEGAL_SPEED },
    /*  _1LX_HD     4           */      {   HD_1xCLV      },
    /*  _2LX_HD     5           */      {   HD_2xCLV      },
    /*  _4AX_HD     1           */      {   HD_4xCAV      }
};
#endif

BlockT Masks[BLOCK_BIT_SIZE + 1] = // Required to increase the performance while calculating the MASK of coef-value, if not using table a cast to 1LL is required and this consumes too much CPU-load (40 microsec iso 5)
{                                  // To Do: Need to find better location of this table (lib-function)
   /* Size 0  */ 0x00000000,
   /* Size 1  */ 0x00000001,
   /* Size 2  */ 0x00000003,
   /* Size 3  */ 0x00000007,
   /* Size 4  */ 0x0000000F,
   /* Size 5  */ 0x0000001F,
   /* Size 6  */ 0x0000003F,
   /* Size 7  */ 0x0000007F,
   /* Size 8  */ 0x000000FF,
   /* Size 9  */ 0x000001FF,
   /* Size 10 */ 0x000003FF,
   /* Size 11 */ 0x000007FF,
   /* Size 12 */ 0x00000FFF,
   /* Size 13 */ 0x00001FFF,
   /* Size 14 */ 0x00003FFF,
   /* Size 15 */ 0x00007FFF,
   /* Size 16 */ 0x0000FFFF,
   /* Size 17 */ 0x0001FFFF,
   /* Size 18 */ 0x0003FFFF,
   /* Size 19 */ 0x0007FFFF,
   /* Size 20 */ 0x000FFFFF,
   /* Size 21 */ 0x001FFFFF,
   /* Size 22 */ 0x003FFFFF,
   /* Size 23 */ 0x007FFFFF,
   /* Size 24 */ 0x00FFFFFF,
   /* Size 25 */ 0x01FFFFFF,
   /* Size 26 */ 0x03FFFFFF,
   /* Size 27 */ 0x07FFFFFF,
   /* Size 28 */ 0x0FFFFFFF,
   /* Size 29 */ 0x1FFFFFFF,
   /* Size 30 */ 0x3FFFFFFF,
   /* Size 31 */ 0x7FFFFFFF,
   /* Size 32 */ 0xFFFFFFFF
};

/*****************************************************************************
*                    Type Definitions
*****************************************************************************/

/*****************************************************************************
*                    Public Data Definitions
*****************************************************************************/

COEFS_MEDIA_t mCoefDiscKind;
COEFS_SPEED_t mCoefSpeed;
discKindT     mDiscKind;
ESpindleSpeed mSpeed;

/*****************************************************************************
*                    Static Function Prototypes
*****************************************************************************/

__inline BlockT * GetAddressBlock(COEFS_MEDIA_t CoefDiscKind, COEFS_SPEED_t CoefSpeed,COEFS_t eCoef, ECores eCore);
__inline ULONG    GetValue(COEFS_MEDIA_t CoefDiscKind, COEFS_SPEED_t CoefSpeed, COEFS_t eCoef, ECores eCore);
__inline void     SetValue(COEFS_MEDIA_t CoefDiscKind, COEFS_SPEED_t CoefSpeed, COEFS_t eCoef, ECores eCore, ULONG Value);
__inline BlockT * GetInitAddress(COEFS_t eCoef, ECores eCore);
__inline BlockT * GetFixedAddress(USHORT eCoef, ECores eCore);
__inline BlockT * GetDiscGroupAddress(COEFS_MEDIA_t CoefDiscKind, COEFS_t eCoef, ECores eCore);
__inline BlockT * GetDiscKindAddress (COEFS_MEDIA_t CoefDiscKind, COEFS_t eCoef, ECores eCore);
__inline BlockT * GetSpeedDiscGroupAddress(COEFS_MEDIA_t CoefDiscKind, COEFS_SPEED_t CoefSpeed,COEFS_t eCoef, ECores eCore);
__inline BlockT * GetSpeedDiscKindAddress(COEFS_MEDIA_t CoefDiscKind, COEFS_SPEED_t CoefSpeed,COEFS_t eCoef, ECores eCore);

/*****************************************************************************
*                    Static Function Definitions
*****************************************************************************/

/*-----------------------------------------------------------------------------------------------------
    Name: GetAddressBlock

    Description:

    Inputs:
    Outputs:
    Global Inputs:
    Global Outputs:
    Returns:
-----------------------------------------------------------------------------------------------------*/

__inline BlockT * GetAddressBlock(COEFS_MEDIA_t CoefDiscKind, COEFS_SPEED_t CoefSpeed,COEFS_t eCoef, ECores eCore)
{
    BlockT *pCoefAddress;

    switch (CoefsRegTable[eCoef].eDependency)
    {
        case E_DEPENDENCY_INIT:
            pCoefAddress = GetInitAddress(eCoef,eCore);
            break;
        case E_DEPENDENCY_FIXED:
            pCoefAddress = GetFixedAddress(eCoef,eCore);
            break;
        case E_DEPENDENCY_DISCGROUP:
            pCoefAddress = GetDiscGroupAddress(CoefDiscKind, eCoef, eCore);
            break;
        case E_DEPENDENCY_DISCKIND:
            pCoefAddress = GetDiscKindAddress(CoefDiscKind, eCoef, eCore);
            break;
        case E_DEPENDENCY_SPEED_DISCGROUP:
            pCoefAddress = GetSpeedDiscGroupAddress(CoefDiscKind, CoefSpeed, eCoef, eCore);
            break;
        case E_DEPENDENCY_SPEED_DISCKIND:
            pCoefAddress = GetSpeedDiscKindAddress(CoefDiscKind, CoefSpeed, eCoef, eCore);
            break;
        default:
            //ASSERT
            break;
    }
    return pCoefAddress;
}

/*-----------------------------------------------------------------------------------------------------
    Name: ConvertParams

    Description:

    Inputs:
    Outputs:
    Global Inputs:
    Global Outputs:
    Returns:
-----------------------------------------------------------------------------------------------------*/

void ConvertParams(discKindT DiscKind, ESpindleSpeed Speed, COEFS_MEDIA_t *CoefDiscKind, COEFS_SPEED_t *CoefSpeed  )
{
    COEFS_MEDIA_t i;

    /* Get the index number of CoefDiscKind in the lookup table */
    for (i = 0; i < LAST_MEDIA; i++)
    {
        if (CoefDiscKindToServo[i] == DiscKind.fDiscKind)
        {
            break;
        }
    }

    /* Set the CoefDiscKind equal to the index number */
    *CoefDiscKind = i;

    if (*CoefDiscKind == e_ILLEGAL_MEDIUM)
    {   // Something went wrong !!
        DEBUG_ASSERT(FALSE, eASSERT_IMPOSSIBLE_COEF_DISCKIND );
    }

    /* Get the CoefSpeed */
    switch (CoefDiscKindToDiscGroup[i])
    {
        case DiscGroupCD:
            *CoefSpeed = SpeedToCoefSpeedTypeCD[Speed];
            break;
        case DiscGroupDVD:
            *CoefSpeed = SpeedToCoefSpeedTypeDVD[Speed];
            break;
        case DiscGroupDVDRAM:
            *CoefSpeed = SpeedToCoefSpeedTypeDVDRAM[Speed];
            break;
#if (BD_ENABLE == 1)
        case DiscGroupBD:
            *CoefSpeed = SpeedToCoefSpeedTypeBD[Speed];
            break;
#endif
#if (ENABLE_HDDVD == 1)
        case DiscGroupHD:
            *CoefSpeed = SpeedToCoefSpeedTypeHD[Speed];
            break;
#endif
        case DiscGroupUnknown :
        default:
            // Unknown Hal Disc Kind
            DEBUG_ASSERT(FALSE, eASSERT_IMPOSSIBLE_COEF_DISCKIND );
            break;
    }
    /* Check if the Coef Speed is illegal */
    if (*CoefSpeed == ILLEGAL_SPEED)
    {
        /* Something went wrong !! */
        DEBUG_ASSERT(FALSE, eASSERT_ILLEGAL_COEF_SPEED );
    }    // end if
}

/*-----------------------------------------------------------------------------------------------------
    Name: GetValue

    Description:

    Inputs:
    Outputs:
    Global Inputs:
    Global Outputs:
    Returns:
-----------------------------------------------------------------------------------------------------*/

__inline ULONG GetValue(COEFS_MEDIA_t CoefDiscKind, COEFS_SPEED_t CoefSpeed, COEFS_t eCoef, ECores eCore)
{
   ULONG  Value;
   BlockT Mask                  = Masks[CoefsRegTable[eCoef].MSB - CoefsRegTable[eCoef].LSB + 1];
   USHORT BlockPos              = CoefsRegTable[eCoef].BitOffset / BLOCK_BIT_SIZE;
   BYTE   RelativeBitPosition   = (CoefsRegTable[eCoef].BitOffset - BLOCK_BIT_SIZE * BlockPos) % BLOCK_BIT_SIZE;

   // Retrieve ULONG Value
   Value = *((unsigned long *) GetAddressBlock(CoefDiscKind, CoefSpeed, eCoef, eCore) + BlockPos);
   // Move the value to the correct position in the bit-stream
   Value = Value >> RelativeBitPosition;
   // Clear all unwanted data, keep only the data from the coef
   Value &= Mask;
   return Value;
}

/*-----------------------------------------------------------------------------------------------------
    Name: GetValueParams

    Description:

    Inputs:
    Outputs:
    Global Inputs:
    Global Outputs:
    Returns:
-----------------------------------------------------------------------------------------------------*/

ULONG GetValueParams(COEFS_MEDIA_t CoefDiscKind, COEFS_SPEED_t CoefSpeed, COEFS_t eCoef)
{
    ULONG  Value;
    ECores eCore;

    DEBUG_ASSERT( bCoefsInitialised, eASSERT_COEFS_NOT_INITIALISED);

    eCore = CoefsRegTable[eCoef].eCore;
    Value = GetValue(CoefDiscKind, CoefSpeed, eCoef, eCore);

    return Value;
}

/*-----------------------------------------------------------------------------------------------------
    Name: SetValue

    Description:

    Inputs:
    Outputs:
    Global Inputs:
    Global Outputs:
    Returns:
-----------------------------------------------------------------------------------------------------*/

__inline void SetValue(COEFS_MEDIA_t CoefDiscKind, COEFS_SPEED_t CoefSpeed, COEFS_t eCoef, ECores eCore, ULONG Value)
{
   BlockT *Address;
   BlockT  Mask                = Masks[CoefsRegTable[eCoef].MSB - CoefsRegTable[eCoef].LSB + 1];
   USHORT  BlockPos            = CoefsRegTable[eCoef].BitOffset / BLOCK_BIT_SIZE;
   BYTE    RelativeBitPosition = (CoefsRegTable[eCoef].BitOffset - BLOCK_BIT_SIZE * BlockPos) % BLOCK_BIT_SIZE;

   //In case somebody writes a too long value
   DEBUG_ASSERT( ( Value & ~Mask ) == 0 , eASSERT_ILLEGAL_COEF_VALUE_OUT_OF_BOUND);

   Address = GetAddressBlock(CoefDiscKind, CoefSpeed, eCoef, eCore) + BlockPos;

   // Move the value to the correct position in the bit-stream
   Value = Value << RelativeBitPosition;
   // Clear the content and add the new value (read-modify-write)
   *Address = (*Address & ~(Mask << RelativeBitPosition)) | Value;
}

/*-----------------------------------------------------------------------------------------------------
    Name: SetValueParams

    Description:

    Inputs:
    Outputs:
    Global Inputs:
    Global Outputs:
    Returns:
-----------------------------------------------------------------------------------------------------*/

void SetValueParams(COEFS_MEDIA_t CoefDiscKind, COEFS_SPEED_t CoefSpeed, COEFS_t eCoef, ULONG Value)
{
   ECores      eCore;

   DEBUG_ASSERT( bCoefsInitialised, eASSERT_COEFS_NOT_INITIALISED);

   eCore = CoefsRegTable[eCoef].eCore;
   SetValue(CoefDiscKind, CoefSpeed, eCoef, eCore, Value);
}

/*-----------------------------------------------------------------------------------------------------
    Name: GetInitAddress

    Description:

    Inputs:
    Outputs:
    Global Inputs:
    Global Outputs:
    Returns:
-----------------------------------------------------------------------------------------------------*/

__inline BlockT * GetInitAddress(COEFS_t eCoef, ECores eCore)
{
    BlockT *pCoefBaseAddress;

    /* Get the corresponding start addresses in the coef table */
    pCoefBaseAddress = CoefDependencyProperties[eCore].Init.Address;

    return pCoefBaseAddress;
}

/*-----------------------------------------------------------------------------------------------------
    Name: GetFixedAddress

    Description:

    Inputs:
    Outputs:
    Global Inputs:
    Global Outputs:
    Returns:
-----------------------------------------------------------------------------------------------------*/

__inline BlockT * GetFixedAddress(USHORT eCoef, ECores eCore)
{
    BlockT *pCoefBaseAddress;

    /* Get the corresponding start addresses in the coef table */
    pCoefBaseAddress = CoefDependencyProperties[eCore].Fixed.Address;

    return pCoefBaseAddress;
}

/*-----------------------------------------------------------------------------------------------------
    Name: GetDiscGroupAddress

    Description:

    Inputs:
    Outputs:
    Global Inputs:
    Global Outputs:
    Returns:
-----------------------------------------------------------------------------------------------------*/

__inline BlockT * GetDiscGroupAddress(COEFS_MEDIA_t CoefDiscKind, COEFS_t eCoef, ECores eCore)
{
    BlockT *pCoefBaseAddress;

    /* Get the corresponding start addresses in the coef table */
    pCoefBaseAddress = CoefDependencyProperties[eCore].DiscGroup.Address[CoefDiscKindToDiscGroup[CoefDiscKind]];

    return pCoefBaseAddress;
}

/*-----------------------------------------------------------------------------------------------------
    Name: GetDiscKindAddress

    Description:

    Inputs:
    Outputs:
    Global Inputs:
    Global Outputs:
    Returns:
-----------------------------------------------------------------------------------------------------*/

__inline BlockT * GetDiscKindAddress (COEFS_MEDIA_t CoefDiscKind, COEFS_t eCoef, ECores eCore)
{
    BlockT *pCoefBaseAddress;
    USHORT  CoefRelDiscKind;

    /* Get the corresponding start addresses per disc kind in the coef table */
    pCoefBaseAddress = CoefDependencyProperties[eCore].DiscKind.Address[CoefDiscKindToDiscGroup[CoefDiscKind]];
    switch(CoefDiscKindToDiscGroup[CoefDiscKind])
    {
        case DiscGroupCD:
            CoefRelDiscKind = CoefDiscKind - OFFSET_CD_DISCGROUP;
            break;
        case DiscGroupDVD:
            CoefRelDiscKind = CoefDiscKind - OFFSET_DVD_DISCGROUP;
            break;
        case DiscGroupDVDRAM:
            CoefRelDiscKind = CoefDiscKind - OFFSET_DVDRAM_DISCGROUP;
            break;
#if (BD_ENABLE == 1)
        case DiscGroupBD:
            CoefRelDiscKind = CoefDiscKind - OFFSET_BD_DISCGROUP;
            break;
#endif //(BD_ENABLE == 1)

#if (ENABLE_HDDVD == 1)
        case DiscGroupHD:
            CoefRelDiscKind = CoefDiscKind - OFFSET_HD_DISCGROUP;
            break;
#endif

        case DiscGroupUnknown:
        default:
            // wrong DiscGroup
            DEBUG_ASSERT(FALSE, eASSERT_IMPOSSIBLE_COEF_DISCKIND ); //already asserted in StoreParams
            break;
    }

    // select the correct size depending on the element size
    return (BlockT *) ( (BYTE * ) ( (BYTE * )pCoefBaseAddress + CoefDependencyProperties[eCore].DiscKind.StructSize * (CoefRelDiscKind) ) );
}

/*-----------------------------------------------------------------------------------------------------
    Name: GetSpeedDiscGroupAddress

    Description:

    Inputs:
    Outputs:
    Global Inputs:
    Global Outputs:
    Returns:
-----------------------------------------------------------------------------------------------------*/
__inline BlockT * GetSpeedDiscGroupAddress(COEFS_MEDIA_t CoefDiscKind, COEFS_SPEED_t CoefSpeed,COEFS_t eCoef, ECores eCore)
{
    BlockT *pCoefBaseAddress;

    /* Get the corresponding start addresses per spd in the coef table */
    pCoefBaseAddress = CoefDependencyProperties[eCore].SpeedDiscGroup.Address[CoefDiscKindToDiscGroup[CoefDiscKind]];

    return (BlockT *) ( (BYTE *) ( (BYTE *) pCoefBaseAddress + CoefDependencyProperties[eCore].SpeedDiscGroup.StructSize * (CoefSpeed) ) );
}
/*-----------------------------------------------------------------------------------------------------
    Name: GetSpeedDiscKindAddress

    Description:

    Inputs:
    Outputs:
    Global Inputs:
    Global Outputs:
    Returns:
-----------------------------------------------------------------------------------------------------*/
__inline BlockT * GetSpeedDiscKindAddress(COEFS_MEDIA_t CoefDiscKind, COEFS_SPEED_t CoefSpeed,COEFS_t eCoef, ECores eCore)
{
    BYTE     NrOfDiscKinds;
    BlockT  *pCoefBaseAddress;
    USHORT   CoefRelDiscKind;

    /* Get the corresponding start addresses per disc kind in the coef table */
    pCoefBaseAddress = CoefDependencyProperties[eCore].SpeedDiscKind.Address[CoefDiscKindToDiscGroup[CoefDiscKind]];
    switch(CoefDiscKindToDiscGroup[CoefDiscKind])
    {
        case DiscGroupCD:
            CoefRelDiscKind  = CoefDiscKind - OFFSET_CD_DISCGROUP;
            NrOfDiscKinds    = NUMBER_OF_CD_DISC_KINDS;
            break;
        case DiscGroupDVD:
            CoefRelDiscKind  = CoefDiscKind - OFFSET_DVD_DISCGROUP;
            NrOfDiscKinds    = NUMBER_OF_DVD_DISC_KINDS;
            break;
        case DiscGroupDVDRAM:
            CoefRelDiscKind  = CoefDiscKind - OFFSET_DVDRAM_DISCGROUP;
            NrOfDiscKinds    = NUMBER_OF_DVDRAM_DISC_KINDS;
            break;
#if (BD_ENABLE == 1)
        case DiscGroupBD:
            CoefRelDiscKind  = CoefDiscKind - OFFSET_BD_DISCGROUP;
            NrOfDiscKinds    = NUMBER_OF_BD_DISC_KINDS;
            break;
#endif //(BD_ENABLE == 1)

#if (ENABLE_HDDVD == 1)
        case DiscGroupHD:
            CoefRelDiscKind  = CoefDiscKind - OFFSET_HD_DISCGROUP;
            NrOfDiscKinds    = NUMBER_OF_HD_DISC_KINDS;
            break;
#endif //(ENABLE_HDDVD == 1)

        case DiscGroupUnknown :
        default:
            // wrong DiscKind
            DEBUG_ASSERT(FALSE, eASSERT_IMPOSSIBLE_COEF_DISCKIND ); //already asserted in StoreParams
            break;
    }

    return (BlockT *) ( (BYTE * ) ( (BYTE * )pCoefBaseAddress + CoefDependencyProperties[eCore].SpeedDiscKind.StructSize * (CoefSpeed * NrOfDiscKinds + CoefRelDiscKind) ) );
}

/*-----------------------------------------------------------------------------------------------------
    Name: CalculateBitOffset

    Description:

    Inputs:
    Outputs:
    Global Inputs:
    Global Outputs:
    Returns:
-----------------------------------------------------------------------------------------------------*/

void CalculateBitOffset(COEFS_t eCoefFirstElement, COEFS_t eCoefLastElement)
{
   COEFS_t eCoef;
#ifdef DEBUG_BIT_OFFSETS
   USHORT padding = 0;
#endif

   if ( eCoefLastElement >= eCoefFirstElement ) // it could be that there are not coefs inbetween first and last (core/dependency is empty)
   {
      CoefsRegTable[eCoefFirstElement].BitOffset = 0;
#ifdef DEBUG_BIT_OFFSETS
      SendMsgCnB(DEBUG_COEF_MSG,12,0x3FAA26,B4B(eCoefFirstElement),B4B((CoefsRegTable[eCoefFirstElement].MSB - CoefsRegTable[eCoefFirstElement].LSB + 1)),
        B4B(CoefsRegTable[eCoefFirstElement].BitOffset));
#endif
      for (eCoef = eCoefFirstElement + 1; eCoef <= eCoefLastElement; eCoef++)
      {
         // Check if the coef fits into the remaining bits of an unsigned-long, otherwise skip to the next unsigned-long
         if ( ( BLOCK_BIT_SIZE - ( ( CoefsRegTable[eCoef -1].BitOffset + (CoefsRegTable[eCoef - 1].MSB - CoefsRegTable[eCoef - 1].LSB + 1) ) % BLOCK_BIT_SIZE ) ) >= (CoefsRegTable[eCoef].MSB - CoefsRegTable[eCoef].LSB + 1) )
         {
            CoefsRegTable[eCoef].BitOffset = CoefsRegTable[eCoef - 1].BitOffset + (CoefsRegTable[eCoef - 1].MSB - CoefsRegTable[eCoef - 1].LSB + 1);
         }
         else
         {
#ifdef DEBUG_BIT_OFFSETS
            padding += ( BLOCK_BIT_SIZE - ( ( CoefsRegTable[eCoef -1].BitOffset + (CoefsRegTable[eCoef - 1].MSB - CoefsRegTable[eCoef - 1].LSB + 1) ) % BLOCK_BIT_SIZE ) );
#endif
            CoefsRegTable[eCoef].BitOffset = BLOCK_BIT_SIZE * ( 1  + ( CoefsRegTable[eCoef - 1].BitOffset / BLOCK_BIT_SIZE ) );
         }
#ifdef DEBUG_BIT_OFFSETS
         SendMsgCnB(DEBUG_COEF_MSG,12,0x3FAA26,B4B(eCoef),B4B((CoefsRegTable[eCoef].MSB - CoefsRegTable[eCoef].LSB + 1)),
            B4B(CoefsRegTable[eCoef].BitOffset));
#endif
      }
   }
#ifdef DEBUG_BIT_OFFSETS
   SendMsgCnB(DEBUG_COEF_MSG,12,0x3FAA27,B2B(padding),B2B(CoefsRegTable[eCoefLastElement].BitOffset + (CoefsRegTable[eCoefLastElement].MSB - CoefsRegTable[eCoefLastElement].LSB + 1)),
       B4B(eCoefFirstElement),B4B(eCoefLastElement));
#endif
}

/*****************************************************************************
*                    Public Function Definitions
*****************************************************************************/

/*-----------------------------------------------------------------------------------------------------
    Name: COEF_Init

    Description:

    Inputs:
    Outputs:
    Global Inputs:
    Global Outputs:
    Returns:
-----------------------------------------------------------------------------------------------------*/

void COEF_Init(void)
{
   ECores eCore;

#ifdef DEBUG_BIT_OFFSETS
   SendMsgCnB(DEBUG_COEF_MSG,4,0x3FAA25,B2B(sizeof(COEFS_TABLE_t)),B2B(sizeof(COEFS_REG_TABLE_t))); //Coef size values, struct
#endif

   // Later --> Calc of BitOffset could be moved to XLS-MACRO
   for (eCore = 0; eCore < E_CORE_COUNT; eCore++)
   {
#ifdef DEBUG_BIT_OFFSETS
      SendMsgCn(DEBUG_COEF_MSG,1,0x3FAA28,A1B(eCore));
      SendMsg80(DEBUG_COEF_MSG,0x3FAA29);
#endif
      CalculateBitOffset(CoefDependencyProperties[eCore].Init.Index.All.First, CoefDependencyProperties[eCore].Init.Index.All.Last);
#ifdef DEBUG_BIT_OFFSETS
      SendMsg80(DEBUG_COEF_MSG,0x3FAA2A);
#endif
      CalculateBitOffset(CoefDependencyProperties[eCore].Fixed.Index.All.First, CoefDependencyProperties[eCore].Fixed.Index.All.Last);
#ifdef DEBUG_BIT_OFFSETS
      SendMsg80(DEBUG_COEF_MSG,0x3FAA2B);
#endif
      CalculateBitOffset(CoefDependencyProperties[eCore].DiscGroup.Index.All.First, CoefDependencyProperties[eCore].DiscGroup.Index.All.Last);
#ifdef DEBUG_BIT_OFFSETS
      SendMsg80(DEBUG_COEF_MSG,0x3FAA2C);
#endif
      CalculateBitOffset(CoefDependencyProperties[eCore].DiscKind.Index.All.First, CoefDependencyProperties[eCore].DiscKind.Index.All.Last);
#ifdef DEBUG_BIT_OFFSETS
      SendMsg80(DEBUG_COEF_MSG,0x3FAA2D);
#endif
      CalculateBitOffset(CoefDependencyProperties[eCore].SpeedDiscGroup.Index.All.First, CoefDependencyProperties[eCore].SpeedDiscGroup.Index.All.Last);
#ifdef DEBUG_BIT_OFFSETS
      SendMsg80(DEBUG_COEF_MSG,0x3FAA2E);
#endif
      CalculateBitOffset(CoefDependencyProperties[eCore].SpeedDiscKind.Index.All.First, CoefDependencyProperties[eCore].SpeedDiscKind.Index.All.Last);
   }
   bCoefsInitialised = TRUE;

}

/*-----------------------------------------------------------------------------------------------------
    Name: COEF_StoreParams

    Description:

    Inputs:
    Outputs:
    Global Inputs:
    Global Outputs:
    Returns:
-----------------------------------------------------------------------------------------------------*/

void COEF_StoreParams(discKindT DiscKind, ESpindleSpeed Speed)
{
   ConvertParams(DiscKind, Speed, &mCoefDiscKind, &mCoefSpeed );
}

/*-----------------------------------------------------------------------------------------------------
    Name: COEF_GetProperties

    Description:

    Inputs:
    Outputs:
    Global Inputs:
    Global Outputs:
    Returns:
-----------------------------------------------------------------------------------------------------*/

void COEF_GetProperties(COEFS_t eCoef, CoefPropertiesT *CoefProperties)
{
    ECores eCore = CoefsRegTable[eCoef].eCore;

    DEBUG_ASSERT(CoefsRegTable[eCoef].Programmable, eASSERT_ILLEGAL_COEF_PROPERTIES);

    /* Get the RegSize out of the look-up table */
    CoefProperties->RegSize = CoefsRegTable[eCoef].RegSize;

    /* Get the Address out of the look-up table coef_table_reg */
    CoefProperties->Address = (void *) (CoreBaseAddresses[eCore] + CoefsRegTable[eCoef].RegOffset);

    /* Get the LSB */
    CoefProperties->LSB = CoefsRegTable[eCoef].LSB;

    /* Get the Mask */
    CoefProperties->Mask = Masks[CoefsRegTable[eCoef].MSB - CoefsRegTable[eCoef].LSB + 1] << CoefsRegTable[eCoef].LSB;
}
/*****************************************************************************
*                       End Of File
*****************************************************************************/
