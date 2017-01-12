/****************************************************************************
*                (c) Copyright 2001 - 2008 SUNEXT DESIGN, INC.
*                           All Right Reserved
*
*
* FILENAME:   SVO_AFE.H
*
* DESCRIPTION
*
*   $Revision: 172 $
*   $Date: 11/03/10 6:24p $
*
**************************** SUNEXT CONFIDENTIAL ****************************/

#ifndef _SVO_AFE_
#define _SVO_AFE_

#include ".\oem\oem_info.h"
#include ".\common\vu_cfg.h"
#include ".\servo\SVO.H"
#include ".\servo\SVOVAR.H"
#include ".\al\REG_HAL.h"
#include ".\al\REG_afe.h"

//set MUXOUT channel
typedef enum
{
    AFE_MUXOUT1 = 0,
    AFE_MUXOUT2,
    AFE_MUXOUT3,
    AFE_MUXTOAD1,
    AFE_MUXTOAD2,
    AFE_MUXTOAD3
} AFEMUXBank_t;

typedef union
{
    BYTE               mux;
    halAFE_MUX1ADSEL_t mux1;
    halAFE_MUX2ADSEL_t mux2;
    halAFE_MUX3ADSEL_t mux3;
} ADSignal_t;


/* path selection for afe offset calibration */
#define TEMP_OS             0    /* Don't need to do WGATE ON offset calibration and don't update calibration table */
#define NO_WGOS             1    /* Don't need to do WGATE ON offset calibration */
#define DO_WGOS             2    /* Need to do WGATE ON offset calibration */

#define WRITE_OFFSET        0
#define GROOVE_OFFSET       1
#define NORMAL_OFFSET       GROOVE_OFFSET
#define LAND_OFFSET         2
#define WRITE_OFFSET_AVG    3
#define EXTRA_OFFSET        4
#define EXTRA_OFFSET_AVG    5
#define LI_OFFSET           6

#define WRITE_GAIN          WRITE_OFFSET
#define GROOVE_GAIN         GROOVE_OFFSET
#define NORMAL_GAIN         NORMAL_OFFSET
#define LAND_GAIN           LAND_OFFSET
#define WRITE_GAIN_AVG      WRITE_OFFSET_AVG
#define LI_GAIN             LI_OFFSET

#define RECDL               0
#define RECDH               1

#define TYPE1               0
#define TYPE2               1
#define TYPE2BAL            2
#define MISC                3
#if (AFE_NORMALIZER == 1)
#define NOR_BS              4
#endif
#define TYPE3               5
#define TYPE4               6

/* signal selection for afe offset calibration */
//TYPE1
#define FE_OS                0
#define TE_OS                1
#define MAX_TYPE1_OFS_NO    (TE_OS+1)

//TYPE2
#define MPP_OS              0
#define SPP1_OS             1
#define FEO_OS              2
#define SPP2_OS             3
#define SBAD_OS             4
#define SRF_OS              5
#define MBVREF_OS           6
#define SBVREF_OS           7
#define FE2_OS              8
#define TE2_OS              9
#define BS2_OS              10
#define RFCP_OS             11
#define RFCN_OS             12
#define RADO_OS             13
#define RBCO_OS             14
#define BS_OS               15 //write=blank; normal=written
#define MAX_TYPE2_OFS_NO    (BS_OS+1)
    #if (WRITE_MBSBVREF_OS == 0)
#define MAX_TYPE2EX_OFS_NO  (SRF_OS+1)
    #else
#define MAX_TYPE2EX_OFS_NO  (SBVREF_OS+1)
    #endif
#if (MPPBG_SPPBG_CALIB != 0)
#define MAX_TYPE2BAL_OFS_NO (SPP1_OS+1)
#endif

//TYPE3
#define CE_OS               0
#define PHS_OS              1
#define BHS_OS              2
#define RP_OS               3
#define RP2_OS              4
#define RFP_OS              5
#define VREF_OS             6
#define RFPGA_OS            7
#define DPDCH1_OS           8
#define DPDCH2_OS           9
#define DPDCH3_OS           10
#define DPDCH4_OS           11
    #if (CHIP_REV < 0xC0)
#define MAX_TYPE3_OFS_NO    (DPDCH4_OS+1)
    #else
#define TX_OS               12
#define MAX_TYPE3_OFS_NO    (TX_OS+1)
    #endif

//TYPE4
#define DPDGM_OS            0
#define APC_OS              1
#if (CHIP_REV < 0xC0)
#define MAX_TYPE4_OFS_NO    (APC_OS+1)
#else
#define RFREFP_OS           2
#define RFREFN_OS           3
#define MAX_TYPE4_OFS_NO    (RFREFN_OS+1)
#endif

/* afe offset calibration */
#define SIGNAL_UNKNOW       0
#define SIGNAL_ABOVE_TARGET 1
#define SIGNAL_BELOW_TARGET 2

/* signal selection for afe offset calibration */
//TYPE1
#define FE_GN               0
#define TE_GN               1
#define MAX_TYPE1_GN_NO     (TE_GN+1)

//TYPE2
#define BS_GN               0
#define RF_GN               1
#define MB_GN               2
#define SB_GN               3
#define RAD_GN              4
#define RBC_GN              5
#define RADP_GN             6
#define RBCP_GN             7
#define MAX_TYPE2_GN_NO     (RBCP_GN+1)

//TYPE3
#define CE_GN               0
#define RP_GN               1
#define DPD_GN              2
#define WOB_GN              3
#define RFPGA_GN            4
#define RFIN_GN             5
#define TEB_GN              6
#define RPB_GN              7
#define CEB_GN              8
#define FEOB_GN             9
#define SPP2B_GN            10
    #if (MPPBG_SPPBG_CALIB == 0)
#define MAX_TYPE3_GN_NO     (SPP2B_GN+1)
    #else
#define MPPB_GN             11
#define SPP1B_GN            12
#define MAX_TYPE3_GN_NO     (SPP1B_GN+1)
    #endif

    #if (AFE_NORMALIZER == 1)
// BS
#define TAR_BS              0
#define THR_BS              1
#define MAX_NOR_BS_NO (THR_BS+1)
    #endif

/* afe gain calibration */
#define P2P_UNKNOW          0
#define GREATER_THAN_TARGET 1
#define SMALLER_THAN_TARGET 2

extern struct   svoCalOfsT1VarT     svoCalOfsT1Var[2][MAX_TYPE1_OFS_NO];
extern struct   svoCalOfsT2VarT     svoCalOfsT2Var[2][MAX_TYPE2_OFS_NO];
extern struct   svoCalOfsT3VarT     svoCalOfsT3Var[2][MAX_TYPE3_OFS_NO];
extern struct   svoCalOfsT4VarT     svoCalOfsT4Var[MAX_TYPE4_OFS_NO];
    #if (AFE_NORMALIZER == 1)
extern struct   svoCalBsNorVarT     svoCalBsNorVar[2][MAX_NOR_BS_NO];
    #endif
    #if (MPPBG_SPPBG_CALIB != 0)
extern struct   svoCalBalOfsT2VarT  svoCalBalOfsT2Var[MAX_TYPE2BAL_OFS_NO][2];
    #endif
extern struct   svoCalGnT1VarT      svoCalGnT1Var[2][MAX_TYPE1_GN_NO];
extern struct   svoCalGnT2VarT      svoCalGnT2Var[2][MAX_TYPE2_GN_NO];
extern struct   svoCalGnT3VarT      svoCalGnT3Var[2][MAX_TYPE3_GN_NO];

/*--------------------------------------------------------------*/
/* Prototype                                                    */
/*--------------------------------------------------------------*/
    #if (CHIP_REV <= 0xB3)
void    AfeMUXOUT(BYTE mux_channel, USHORT mux_field);
    #else
void    AfeMUXOUT(BYTE mux_channel, USHORT mux_field, USHORT mux_field2);
    #endif
BYTE    afe_GetRegData(BYTE addr,BYTE type);
void    afe_ajust_clr(void);
void    InitAfeRegsByteForASpeed(ESpindleSpeed speed);
BYTE    afeSetCalibMUX(BYTE type, BYTE adjust_signal);
void    AfeSetMUXTOAD(BYTE select_channel);
void    afeSetTrackingMethod(ETrackingMethod tracking_method);

/*--------------------------------------------------------------*/
/* External Functions                                           */
/*--------------------------------------------------------------*/
#if (CHIP_REV <= 0xB3)
extern void     AfeMUXOUT(BYTE mux_channel, USHORT mux_field);
#else
extern void     AfeMUXOUT(BYTE mux_channel, USHORT mux_field, USHORT mux_field2);
#endif
extern BYTE     AfeRouteSignalToAD(AFEMUXBank_t AFEMUXBank, ADSignal_t MuxField);
extern BYTE     afe_GetRegData(BYTE addr,BYTE type);
extern void     svoAFEInit(void);
extern void     svoAFEInitOnly(void);
extern void     afe_ajust_clr(void);
#if (AFE_POWER_DOWN == 1)
extern void     AFEPowerOn(void);
extern void     AFEPowerDown(void);
#endif /* AFE_POWER_DOWN */
extern BYTE     afeSetCalibMUX(BYTE type, BYTE adjust_signal);
extern void     AfeSetMUXTOAD(BYTE select_channel);
extern void     afeSetTrackingMethod(ETrackingMethod tracking_method);
extern void     afeSetWobbleForASpeed(ESpindleSpeed sp);
extern void     afeSetSvognMode(BYTE WrGainMode);

#if (AFE_NORMALIZER == 1)
/*-----------------------------------------------------------------------------
 *
 * Module:       afeSvognInit()
 * Description:
 *               Initialises the circuitry that controls the BS-, FE- and TE-
 *               gain. This feature is designed to keep loop gain constant independant
 *               on write power and refelectivity changes over the disc.
 *
 *               pre  :
 *                      AFE SVOGN hardware not initilised
 *
 *               post :
 *                      SVOGN hardware initilised but switched off
 * Input:
 *               void
 * Output:
 *               void
 *
 *----------------------------------------------------------------------------*/
extern __inline void afeSvognInit(void)
{
    WRITE_FIELD(HAL_AFE_SETSVOGN, READ_FIELD(HAL_AFE_BS2GNREG));
    WRITE_FIELD(HAL_AFE_BS2AUTOEN, 1);
}

/*-----------------------------------------------------------------------------
 *
 * Module:       afeSvognEnable()
 * Description:
 *               closes the SVOGN loop but the  FE- and TE gains are still fixed.
 *               (NO DYNAMIC CONTROL OF TE AND FE YET)
 *
 *               pre  :
 *                      Initialisation done
 *
 *               post :
 *                      SVOGN hardware loop closed servo loop are still controled by a fixed gain
 * Input:
 *               void
 * Output:
 *               void
 *
 *----------------------------------------------------------------------------*/
extern __inline void afeSvognEnable(void)
{
    #if (BD_ENABLE == 1) && (ENABLE_BDRLTH == 1)
    if ((svoVar_discKindT.Bit.DiscStd != eDiscBD) || ((svoIramVar.LTHDisclikely == 0) && (svoIramVar.LTHDisc == 0)))
    #endif
    {
        WRITE_FIELD(HAL_AFE_ENAVGSRV, 1);
    }
}

/*-----------------------------------------------------------------------------
 *
 * Module:       afeSvognDisable()
 *
 * Description:
 *               SVOGN hardware loop will be opened
 *
 *               pre  :
 *                      Dynamic control of BS, TE and FE is disabled but the SVOGN loop is still enabled
 *               post :
 *                      SVOGN is disabled and SVOGN is kept constant.
 * Input:
 *               void
 * Output:
 *               void
 *----------------------------------------------------------------------------*/
extern __inline void afeSvognDisable(void)
{
    WRITE_FIELD(HAL_AFE_ENAVGSRV, 0);
    WRITE_FIELD(HAL_AFE_SETSVOGN, READ_FIELD(HAL_AFE_BS2GNREG));
}

/*-----------------------------------------------------------------------------
 *
 * Module:       afeSvognCtrlOn()
 * Description:
 *               FE & TE are being controlled by SVOGN.
 *
 *               pre  :
 *                      SVOGN hardware loop closed servo loop are still controled by a fixed gain
 *               post :
 *                      Gain of servo loops is controlled by SVOGN
 * Input:
 *               void
 * Output:
 *               void
 *
 *----------------------------------------------------------------------------*/
extern __inline void afeSvognCtrlOn(void)
{
    WRITE_FIELD(HAL_AFE_FE2AUTOEN, 1);
    if (svoVar_TrackingMethod != TRK_METHOD_DPD)
    {
        WRITE_FIELD(HAL_AFE_TE2AUTOEN, 1);
    }
}

/*-----------------------------------------------------------------------------
 *
 * Module:       afeSvognCtrlOff()
 *
 * Description:
 *               FE & TE gains are no longer controlled by SVOGN
 *
 *               pre  :
 *                      Gain of servo loops is controlled by SVOGN
 *               post :
 *                      Dynamic gain control by SVGON is switched off
 *                      SVOGN FE- and TE-gain setting fixed with defaultGn.
 * Input:
 *               void
 * Output:
 *               void
 *----------------------------------------------------------------------------*/
extern __inline void afeSvognCtrlOff(void)
{
    WRITE_FIELD(HAL_AFE_FE2AUTOEN, 0);
    WRITE_FIELD(HAL_AFE_TE2AUTOEN, 0);
}
#endif

#endif /* _SVO_AFE_ */
/*------------------------------------------------------------------------*/
/* The end of file                                                        */
/*------------------------------------------------------------------------*/
